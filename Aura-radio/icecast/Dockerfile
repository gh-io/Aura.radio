FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get install -y liquidsoap vorbis-tools icecast2 && \
    rm -rf /var/lib/apt/lists/*

COPY icecast.xml /etc/icecast2/icecast.xml
COPY sample.ogg /music/sample.ogg
COPY loop.liq /loop.liq

CMD icecast -c /etc/icecast2/icecast.xml & \
    liquidsoap /loop.liq
