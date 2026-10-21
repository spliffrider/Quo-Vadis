# Development

Repository: [spliffrider/Quo-Vadis](https://github.com/spliffrider/Quo-Vadis).
It remains private. The visible product name is **Quo Vadis¿**.

The new repository starts from the current 2.72.4 source snapshot; it does not
import the retired repository's history or release tags. Its default branch is
`main`. The initial local source branch is `codex/quo-vadis`; use short-lived
`codex/` branches for subsequent work.

Preserve parameter and port IDs, plugin/model slugs and backup identifiers.
Existing patches and automation rely on them. Historical test/reference headers
remain where the source depends on them; this snapshot is not a claim that every
legacy reference has been refactored away.

## Windows

Install Rack Free 2.6.6, then run from the repository root:

```powershell
./scripts/setup-windows.ps1
./scripts/build.ps1 -Test -Package
./scripts/test-rack.ps1
./scripts/test-production.ps1
./scripts/test-panel-styles.ps1
```

Setup downloads the SDK and prepares the local compiler. Packages are created in
`dist/`. Application checks use isolated profiles under `tmp/` and require normal
Rack sessions to be closed. QA libraries belong only in those test profiles.
Do not edit source or build files while a compiler is running.

For interface changes, run the relevant targeted checks and visually inspect
actual Rack renders. `test-panel-styles.ps1` covers the two current themes, stage
controls, exact alignment, button targets and save/reopen. Broader gesture and
page checks are available through `scripts/test-ui.ps1`; sequencing or state
changes also require their engine/integration tests.

## Linux and macOS

Unpack the matching Rack SDK and build with:

```sh
make RACK_DIR=/absolute/path/to/Rack-SDK dist native-test-build
```

The workflows under `.github/workflows/` define verified SDK downloads, native
library paths and package audits. They run 25 portable engine suites plus Linux
x64, Intel Mac and Apple Silicon package jobs on `main` pushes, pull requests and
manual dispatch. Linux additionally exercises Rack under Xvfb. Workflow results
are evidence for the specific commit they test, not automatic certification of
all platforms or versions.

See [platform coverage](PLATFORMS.md) and [production runtime checks](RUNTIME-VALIDATION.md)
for existing evidence and limits. The 2.72.4 redesign's current native preview
validation is on Windows; human Linux/macOS GUI testing remains open.

## Repository and release contents

Commit source, tests, reusable patches, current documentation and useful native
preview evidence. Keep SDKs, compiler output, profiles, logs, credentials,
reference photographs, old logo assets and personal media out of Git. The
retired project retains its design/archive history separately.

`Makefile` explicitly lists packaged documents and font assets. Check package
contents, stable IDs, production build markers and checksums before uploading a
Windows preview to the [private releases](https://github.com/spliffrider/Quo-Vadis/releases).
A release or repository upload does not change the proprietary software license
or authorize a visibility change.
