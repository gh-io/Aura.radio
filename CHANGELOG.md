# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0-alpha9] - 2025-12-07

### Added

- Provide ability to use `/etc/aura/tank.yaml` as an optional configuration override (tank#51)
- Option for batch query for "files" API endpoints by passing list of IDs (tank#104)

### Changed

- Rename user in Dockerfile from `app` to `aura` (tank#76)

## [1.0.0-alpha8] - 2025-09-17

### Added

- The `File` model has a `Format` field (tank#96)
- Config option to disable normalization for uploads (tank#92)
- Config option to disable conversion for uploads (tank#96)

### Changed

- **Breaking**: Changed API user `engine` to `read-api` and env var `AURA_ENGINE_SECRET` to `AURA_TANK_API_SECRET`. This secret is now used for Engine and Dashboard Clock.
- **Breaking**: To keep using files stored before this version, ensure the `format` field in the `files` table has the correct value: https://gitlab.servus.at/aura/tank/-/issues/96#note_25596
- Update Go to version to 1.24 (tank#89)
- Update Audio Store Structure in `tank` (tank#94)
- `/files` route now exposes `format` (tank#96)

### Fixed

- Permission denied on media upload - local file (tank#95)
- Uploading files through dashboard assigns metadata from different files (tank#97)
- CBA media import (tank#98)
- Media uploads through dashboard fails (#100)

## [1.0.0-alpha7] - 2025-05-22

### Added

- Description field to the file model (tank#87)

### Changed

- `files` route now accepts a description (tank#87)

## [1.0.0-alpha6] - 2025-02-21

### Added

- Database SQLite support. (tank#52)

### Changed

- **Breaking**: There is only one migration step. You need to re-create the database and delete the existing files.
- Update Go version to 1.23 (tank#84)

### Fixed

- /imports endpoint doesn’t return running imports (tank#85)

### Removed

- `Playlist` and `PlaylistEntry` models and migrations, `playlists` API routes and related code. (tank#83)
- Generated docs.go from version control were removed (tank#30)

## [1.0.0-alpha5] - 2024-10-23

### Added

- The setting `DATABASE_URL` is now read from the environment.
- The settings `CORS_ALLOWED_ORIGINS`, `CORS_ALLOWD_HEADER`, `CORS_ALLOWED_METHODS`,
 `TANK_CORS_ALLOW_CREDENTIALS` and `TANK_CORS_DEBUG` are read from the environment (tank#78)
- `make release` target to the Makefile (tank#70)

### Changed

- Changed the aura user and group ID from 2872 to 872.
- **BREAKING:** Upgrade to GORM v2 and gormigrate v2. (tank#36)
  The data volume needs to be deleted and re-created.
  The files and folders from the audio store need to be deleted.
- Setup pre-commit and add configuration for Tank (tank#63)
- Update Go image to 1.22 (tank#73)
- Properly authorize requests to read a playlist (tank#77)
- GET /imports is now a valid request, without `showId` (tank#81)

### Fixed

- as a host, I can neither view nor upload audio files (tank#79)

## [1.0.0-alpha4] - 2024-04-17

### Changed

- The job-timeout for the importer is now thirty minutes for Docker.

### Removed

- Nested routes for show/files, show/files and show/playlists (aura#126)

## [1.0.0-alpha3] - 2024-02-26

### Added

- Un-nested routes for playlists, files, imports, etc. (tank#65)

### Changed

- **BREAKING:** The database and directory structure has changed. Since there is no way to migrate the
  data for this update, you need to re-create the database and delete the existing files and folders
  from the audio store.
- **BREAKING:** The endpoints to create files and playlists now require a `showId` in the JSON payload.
- **BREAKING:** API endpoints now require `showId` (int) instead of the `showName` (string).
- The duration is now in seconds and is stored as a float.
- The job-timeout for the importer is now three minutes for Docker.
- Show uploads are stored in folders named after the ids instead of the slugs. (tank#37)
- error and info log files now have timestamps (tank#68)
- The endpoint to upload a file with Flow JS no longer requires a `showId`.
- The endpoint to list the playlists now lists all, the ones by a show or the ones that include a file.

### Deprecated

- Nested routes for show/files and show/playlists will be removed with alpha-4.

## [1.0.0-alpha2] - 2023-06-20

### Added

- Add default directory `/var/audio/import` used for audio file imports via filesystem (aura#172)
- Write access, error and info logs into `TANK_LOGS` directory (tank#55)

### Changed

- Provide properties in API schemas in CamelCase notation (aura#141)
- Update all APIs to return attributes / properties in camelCase notation (aura#141)

## [1.0.0-alpha1] - 2023-02-24

Initial release.
