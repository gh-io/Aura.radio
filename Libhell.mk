#define preloaded_symbols lt_libhell_LTX_preloaded_symbols

static int hell_preload_callback (lt_dlhandle handle);

int
hell_init (void)
{
  ...
  if (lt_dlpreload (&preloaded_symbols) == 0)
    {
      lt_dlpreload_open ("libhell", preload_callback);
    }
  ...
}
