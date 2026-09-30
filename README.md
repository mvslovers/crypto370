# crypto370

SHA-256, Blowfish and base64 for MVS 3.8j, as a static library built with
the cc370 toolchain.

The code used to live in libc370. It is not part of a C runtime, so it
moved here for libc370 2.0 (mvslovers/libc370#244). Consumers declare it
as a dependency instead of getting it from the sysroot.

## Use

```toml
[dependencies]
"mvslovers/crypto370" = ">=1.0.0"
```

`make deps` stages `crypto370.a` and the three headers. There are no
dependencies of its own; the C runtime comes from the cc370 sysroot.

| Header | Functions |
|---|---|
| `sha256.h` | `sha256_init()`, `sha256_update()`, `sha256_final()` |
| `blowfish.h` | `blowfish_key_setup()`, `blowfish_encrypt()`, `blowfish_decrypt()` (ECB, one 8-byte block) |
| `base64.h` | `base64_encode()`, `base64_decode()` |

One function per translation unit, so the linker's autocall pulls only what
a program calls.

### Moving over from libc370

| libc370 | crypto370 |
|---|---|
| `#include <sha256.h>` | unchanged |
| `#include <blowfish.h>` | unchanged |
| `#include <clibb64.h>` | `#include <base64.h>` |
| `@@B64ENC`, `@@B64DEC` | `B64ENC`, `B64DEC` (C names unchanged) |
| `__b64tbl` (`@@B64TBL`) | internal, no longer declared |

Everything else is source-compatible; a consumer recompiles.

## Character set

SHA-256 and Blowfish work on bytes and do not care. base64 does: the
alphabet is written as C character literals, so on MVS the encoded text is
EBCDIC, while the binary side goes in and out unchanged. The contract is in
`base64.h`.

## Build and test

```
make            the library (build/crypto370.a)
make test       the test load modules
make test-host  run the portable tests natively
make test-mvs   run all tests on MVS (needs .env)
```

The tests are known answers: FIPS 180-2 for SHA-256, Eric Young's ECB
vectors for Blowfish, RFC 4648 section 10 for base64.

## Origin and licenses

- SHA-256 and Blowfish: Brad Conte's crypto-algorithms, public domain.
- base64: Jouni Malinen, BSD license (notice kept in `src/b64enc.c` and
  `src/b64dec.c`).
- Everything else: MIT, see `LICENSE`.
