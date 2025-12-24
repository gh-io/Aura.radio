 /*
   2  * Copyright (c) 2015 Kyle Swanson <k@ylo.ph>.
   3  *
   4  * This file is part of FFmpeg.
   5  *
   6  * FFmpeg is free software; you can redistribute it and/or
   7  * modify it under the terms of the GNU Lesser General Public License
   8  * as published by the Free Software Foundation; either
   9  * version 2.1 of the License, or (at your option) any later version.
  10  *
  11  * FFmpeg is distributed in the hope that it will be useful,
  12  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  13  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  14  * GNU Lesser General Public License for more details.
  15  *
  16  * You should have received a copy of the GNU Lesser General Public License
  17  * along with FFmpeg; if not, write to the Free Software Foundation, Inc.,
  18  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
  19  */
  20 
  21 #include "libavutil/opt.h"
  22 #include "audio.h"
  23 #include "avfilter.h"
  24 #include "internal.h"
  25 #include "libavutil/lfg.h"
  26 #include "libavutil/random_seed.h"
  27 
  28 typedef struct {
  29     const AVClass *class;
  30     int sample_rate;
  31     double amplitude;
  32     int64_t duration;
  33     int64_t color;
  34     int64_t seed;
  35     int nb_samples;
  36 
  37     int64_t pts;
  38     int infinite;
  39     double (*filter)(double white, double *buf);
  40     double buf[7];
  41     AVLFG c;
  42 } ANoiseSrcContext;
  43 
  44 #define OFFSET(x) offsetof(ANoiseSrcContext, x)
  45 #define FLAGS AV_OPT_FLAG_AUDIO_PARAM|AV_OPT_FLAG_FILTERING_PARAM
  46 
  47 static const AVOption anoisesrc_options[] = {
  48     { "sample_rate",  "set sample rate",  OFFSET(sample_rate),  AV_OPT_TYPE_INT,       {.i64 = 48000},  15,  INT_MAX,    FLAGS },
  49     { "r",            "set sample rate",  OFFSET(sample_rate),  AV_OPT_TYPE_INT,       {.i64 = 48000},  15,  INT_MAX,    FLAGS },
  50     { "amplitude",    "set amplitude",    OFFSET(amplitude),    AV_OPT_TYPE_DOUBLE,    {.dbl = 1.},     0.,  1.,         FLAGS },
  51     { "a",            "set amplitude",    OFFSET(amplitude),    AV_OPT_TYPE_DOUBLE,    {.dbl = 1.},     0.,  1.,         FLAGS },
  52     { "duration",     "set duration",     OFFSET(duration),     AV_OPT_TYPE_DURATION,  {.i64 =  0},      0,  INT64_MAX,  FLAGS },
  53     { "d",            "set duration",     OFFSET(duration),     AV_OPT_TYPE_DURATION,  {.i64 =  0},      0,  INT64_MAX,  FLAGS },
  54     { "color",        "set noise color",  OFFSET(color),        AV_OPT_TYPE_INT,       {.i64 =  1},      0,  2,          FLAGS, "color" },
  55     { "colour",       "set noise color",  OFFSET(color),        AV_OPT_TYPE_INT,       {.i64 =  1},      0,  2,          FLAGS, "color" },
  56     { "c",            "set noise color",  OFFSET(color),        AV_OPT_TYPE_INT,       {.i64 =  0},      0,  2,          FLAGS, "color" },
  57     {     "white",    0,                  0,                    AV_OPT_TYPE_CONST,     {.i64 =  0},      0,  0,          FLAGS, "color" },
  58     {     "pink",     0,                  0,                    AV_OPT_TYPE_CONST,     {.i64 =  1},      0,  0,          FLAGS, "color" },
  59     {     "brown",    0,                  0,                    AV_OPT_TYPE_CONST,     {.i64 =  2},      0,  0,          FLAGS, "color" },
  60     { "seed",         "set random seed",  OFFSET(seed),         AV_OPT_TYPE_INT64,     {.i64 = -1},     -1,  UINT_MAX,   FLAGS },
  61     { "s",            "set random seed",  OFFSET(seed),         AV_OPT_TYPE_INT64,     {.i64 = -1},     -1,  UINT_MAX,   FLAGS },
  62     { "nb_samples",   "set the number of samples per requested frame", OFFSET(nb_samples), AV_OPT_TYPE_INT, {.i64 = 1024}, 1, INT_MAX, FLAGS },
  63     { "n",            "set the number of samples per requested frame", OFFSET(nb_samples), AV_OPT_TYPE_INT, {.i64 = 1024}, 1, INT_MAX, FLAGS },
  64     {NULL}
  65 };
  66 
  67 AVFILTER_DEFINE_CLASS(anoisesrc);
  68 
  69 static av_cold int query_formats(AVFilterContext *ctx)
  70 {
  71     ANoiseSrcContext *s = ctx->priv;
  72     static const int64_t chlayouts[] = { AV_CH_LAYOUT_MONO, -1 };
  73     int sample_rates[] = { s->sample_rate, -1 };
  74     static const enum AVSampleFormat sample_fmts[] = {
  75         AV_SAMPLE_FMT_DBL,
  76         AV_SAMPLE_FMT_NONE
  77     };
  78 
  79     AVFilterFormats *formats;
  80     AVFilterChannelLayouts *layouts;
  81     int ret;
  82 
  83     formats = ff_make_format_list(sample_fmts);
  84     if (!formats)
  85         return AVERROR(ENOMEM);
  86     ret = ff_set_common_formats (ctx, formats);
  87     if (ret < 0)
  88         return ret;
  89 
  90     layouts = avfilter_make_format64_list(chlayouts);
  91     if (!layouts)
  92         return AVERROR(ENOMEM);
  93     ret = ff_set_common_channel_layouts(ctx, layouts);
  94     if (ret < 0)
  95         return ret;
  96 
  97     formats = ff_make_format_list(sample_rates);
  98     if (!formats)
  99         return AVERROR(ENOMEM);
 100     return ff_set_common_samplerates(ctx, formats);
 101 }
 102 
 103 static double white_filter(double white, double *buf)
 104 {
 105     return white;
 106 };
 107 
 108 static double pink_filter(double white, double *buf)
 109 {
 110     double pink;
 111 
 112     /* http://www.musicdsp.org/files/pink.txt */
 113     buf[0] = 0.99886 * buf[0] + white * 0.0555179;
 114     buf[1] = 0.99332 * buf[1] + white * 0.0750759;
 115     buf[2] = 0.96900 * buf[2] + white * 0.1538520;
 116     buf[3] = 0.86650 * buf[3] + white * 0.3104856;
 117     buf[4] = 0.55000 * buf[4] + white * 0.5329522;
 118     buf[5] = -0.7616 * buf[5] - white * 0.0168980;
 119     pink = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + white * 0.5362;
 120     buf[6] = white * 0.115926;
 121     return pink * 0.11;
 122 }
 123 
 124 static double brown_filter(double white, double *buf)
 125 {
 126     double brown;
 127 
 128     brown = ((0.02 * white) + buf[0]) / 1.02;
 129     buf[0] = brown;
 130     return brown * 3.5;
 131 }
 132 
 133 static av_cold int config_props(AVFilterLink *outlink)
 134 {
 135     AVFilterContext *ctx = outlink->src;
 136     ANoiseSrcContext *s = ctx->priv;
 137 
 138     if (s->seed == -1)
 139         s->seed = av_get_random_seed();
 140     av_lfg_init(&s->c, s->seed);
 141 
 142     if (s->duration == 0)
 143         s->infinite = 1;
 144     s->duration = av_rescale(s->duration, s->sample_rate, AV_TIME_BASE);
 145 
 146     switch (s->color) {
 147     case 0: s->filter = white_filter; break;
 148     case 1: s->filter = pink_filter;  break;
 149     case 2: s->filter = brown_filter; break;
 150     }
 151 
 152     return 0;
 153 }
 154 
 155 static int request_frame(AVFilterLink *outlink)
 156 {
 157     AVFilterContext *ctx = outlink->src;
 158     ANoiseSrcContext *s = ctx->priv;
 159     AVFrame *frame;
 160     int nb_samples, i;
 161     double *dst;
 162 
 163     if (!s->infinite && s->duration <= 0) {
 164         return AVERROR_EOF;
 165     } else if (!s->infinite && s->duration < s->nb_samples) {
 166         nb_samples = s->duration;
 167     } else {
 168         nb_samples = s->nb_samples;
 169     }
 170 
 171     if (!(frame = ff_get_audio_buffer(outlink, nb_samples)))
 172         return AVERROR(ENOMEM);
 173 
 174     dst = (double *)frame->data[0];
 175     for (i = 0; i < nb_samples; i++) {
 176         double white;
 177         white = s->amplitude * ((2 * ((double) av_lfg_get(&s->c) / 0xffffffff)) - 1);
 178         dst[i] = s->filter(white, s->buf);
 179     }
 180 
 181     if (!s->infinite)
 182         s->duration -= nb_samples;
 183 
 184     frame->pts = s->pts;
 185     s->pts    += nb_samples;
 186     return ff_filter_frame(outlink, frame);
 187 }
 188 
 189 static const AVFilterPad anoisesrc_outputs[] = {
 190     {
 191         .name          = "default",
 192         .type          = AVMEDIA_TYPE_AUDIO,
 193         .request_frame = request_frame,
 194         .config_props  = config_props,
 195     },
 196     { NULL }
 197 };
 198 
 199 AVFilter ff_asrc_anoisesrc = {
 200     .name          = "anoisesrc",
 201     .description   = NULL_IF_CONFIG_SMALL("Generate a noise audio signal."),
 202     .query_formats = query_formats,
 203     .priv_size     = sizeof(ANoiseSrcContext),
 204     .inputs        = NULL,
 205     .outputs       = anoisesrc_outputs,
 206     .priv_class    = &anoisesrc_class,
 207 };
