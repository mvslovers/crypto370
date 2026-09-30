# CLAUDE.md — crypto370

Extends the ecosystem root `CLAUDE.md` (`~/repos/mvs/CLAUDE.md`); nothing
here contradicts it.

## What this is

A static library: SHA-256, Blowfish and base64, moved out of libc370 for
2.0 (mvslovers/libc370#244). Consumers: httpd (all three) and mvsMF
(base64). No FMID: the archive is linked statically and never installed on
MVS, like libc370 and lstring370.

## Build

mbt v2, `type = "library"`. `make lib` builds `build/crypto370.a`;
`make package` puts it with the headers into
`dist/crypto370-<version>-lib.tar.gz`, which is what a consumer's
`make deps` downloads from the GitHub release. No `[dependencies]`: the
C runtime is the cc370 sysroot (`-lc`).

`[toolchain] libc370` pins what a release is built against; `build.yml`
floats on main on purpose.

## Rules specific to this project

- **One function per TU.** ld370 autocalls whole members, so a TU that
  held two functions would link both into every caller of either: a base64
  user would carry Blowfish's 4 KB of S-boxes.
- **Symbols are this project's, not libc370's.** No `@@` names: that is
  libc370's namespace. External names stay at 8 characters or fewer.
- **Test inputs are bytes, not string literals.** `"abc"` is EBCDIC on
  MVS and hashes differently. See `test/mvs/tsthex.h`.
- **Every known answer is re-derived on the host** (`shasum`, `openssl
  enc -bf-ecb` with the legacy provider and the 8-byte key doubled,
  Python's `base64`) before it goes into a test.
