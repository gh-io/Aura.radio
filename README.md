# Import and Playlist Daemon

[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](code_of_conduct.md)
[![latest release](https://gitlab.servus.at/aura/tank/-/badges/release.svg)](https://gitlab.servus.at/aura/tank/-/releases)
[![pipeline status](https://gitlab.servus.at/aura/tank/badges/main/pipeline.svg)](https://gitlab.servus.at/aura/tank/-/commits/main)
[![coverage report](https://gitlab.servus.at/aura/tank/badges/main/coverage.svg)](https://gitlab.servus.at/aura/tank/-/commits/main)

Tank implements an AURA import and playlist daemon. Uploads can be normalized
based on the [loudnorm](https://k.ylo.ph/2016/04/04/loudnorm.html) Loudness
Normalization. Resulting files are served to Aura Engine via some configured
directory.

## Requirements

- [Go](https://golang.org/) (v1.18+ recommended)
- Docker (v20.10+)
- GNU Make (v4)

## Development

Check out the repository and build the daemon:

```sh
$ git clone https://gitlab.servus.at/aura/tank
$ cd tank
$ make
```

## Configuration

Create a configuration file, `tank.yaml`, based on:
https://gitlab.servus.at/aura/tank/-/blob/main/contrib/sample-cfg.yaml

Make sure that the directory pointed to by `store.path` exists and is writeable
for the user running the daemon.

### OIDC Client

After [Registering clients at
Steering](https://docs.aura.radio/en/latest/developer/misc/oidc-client-config.html),
update `OIDC_CLIENT_ID` and `OIDC_CLIENT_SECRET` in `contrib/run.sh` to match
your installation.

## Database Setup

First, we need to start a Postgres container:

```sh
$ ./contrib/run_server.sh
```

Wait for the server to boot up and then initialize the database using a second
terminal:

```sh
$ ./contrib/run_client.sh
$$$ psql < init.sql
```

Once the database is created, you can quit by pressing `CTRL-C`.

Now, you may run the daemon using the following command:

```sh
$ ./contrib/run.sh
```

This script uses `tank.yaml` and depends on the postgres server running.

## Running behind reverse proxies

For production environments we highly recommend running `tank` behind an
SSL-enabling reverse proxy. If the base path for the api endpoints as well as
the authentication should be hosted under a subdirectory, the reverse proxy
needs to rewrite the request to be based on `/`. For Nginx the following
configuration can be used:

```
server {
    listen 443 ssl http2;
    listen [::]:443 ssl http2;
    server_name aura.example.com;

    ssl_certificate     /path/to/certificate;
    ssl_certificate_key /path/to/private-key;

    location /tank/ {
         proxy_buffering off;
         proxy_ignore_headers "X-Accel-Buffering";
         proxy_request_buffering off;
         proxy_http_version 1.1;

         proxy_pass http://127.0.0.1:8040/;
    }
    ## ... other locations ...
}
```
