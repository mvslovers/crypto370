# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/).

## [Unreleased]

### Added
- SHA-256, Blowfish and base64, moved from libc370 1.0.8
  (mvslovers/libc370#244). SHA-256 and Blowfish compile to the same
  assembler as they did in libc370.
- Known-answer tests: FIPS 180-2 (SHA-256), Eric Young's ECB vectors
  (Blowfish), RFC 4648 section 10 (base64).

### Changed (against libc370)
- `clibb64.h` is `base64.h`; the functions keep their C names, their
  external names are `B64ENC`/`B64DEC` instead of `@@B64ENC`/`@@B64DEC`.
  The table `__b64tbl` is internal.
- `base64_decode()` calls `memset()` with a prototype in scope; libc370's
  copy called it undeclared, so it went through the out-of-line `MEMSET`
  instead of the inline one.
