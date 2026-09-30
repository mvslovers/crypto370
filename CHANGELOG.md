# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/).

## [Unreleased]

### Added
- SHA-256, Blowfish and base64, moved from libc370 1.0.8
  (mvslovers/libc370#244). Blowfish compiles to the same assembler as it
  did in libc370.
- Known-answer tests: FIPS 180-2 (SHA-256), Eric Young's ECB vectors
  (Blowfish), RFC 4648 section 10 (base64).

### Changed (against libc370)
- `clibb64.h` is `base64.h`; the functions keep their C names, their
  external names are `B64ENC`/`B64DEC` instead of `@@B64ENC`/`@@B64DEC`.
  The table `__b64tbl` is internal.
- SHA-256 counts message bits in two `unsigned int` words instead of
  libc370's `__64`. `sha256.h` no longer includes `<clib64.h>`, which moves
  in libc370 2.0, so crypto370 builds against 1.x and 2.0 alike, and the
  SHA-256 test runs on the host too (`__64` only works big-endian).
  `SHA256_CTX` keeps its size; callers only use it through the API.
- `base64_decode()` calls `memset()` with a prototype in scope; libc370's
  copy called it undeclared, so it went through the out-of-line `MEMSET`
  instead of the inline one.
