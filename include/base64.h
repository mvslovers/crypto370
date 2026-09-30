#ifndef BASE64_H
#define BASE64_H
/* base64.h - base64 encode/decode (RFC 1341 alphabet).
**
** Both return a buffer from calloc() that the caller frees, or NULL.
**
** base64_encode() writes a '\n' after every 72 characters and ends the
** text with a '\0' that *out_len does not count; out_len may be NULL.
** base64_decode() skips characters outside the alphabet (line breaks
** included), answers NULL for a count of symbols that is not a multiple
** of four or for bad padding, and always writes *out_len: it must not be
** NULL.
**
** The alphabet is held as C character literals, so on MVS the encoded
** text is EBCDIC; the binary side is taken and given as it is.
*/
#include <stddef.h>

unsigned char *
base64_encode(const unsigned char *src, size_t len, size_t *out_len)  asm("B64ENC");

unsigned char *
base64_decode(const unsigned char *src, size_t len, size_t *out_len)  asm("B64DEC");

#endif
