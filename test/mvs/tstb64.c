/* tstb64.c - base64 against RFC 4648 section 10, plus the behaviour
** base64.h documents: a '\n' after 72 characters, line breaks skipped on
** decode, NULL for a bad symbol count or bad padding.
**
** The binary side is written as ASCII bytes ("foobar" = 66 6F 6F 62 61
** 72); the text side as C literals, because the encoder emits the
** alphabet in the native character set.  Both hold on MVS and the host.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mbtcheck.h>
#include "base64.h"

static const unsigned char foobar[] = { 0x66, 0x6F, 0x6F, 0x62, 0x61, 0x72 };

static const char *const rfc[] = {
    "", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"
};

int main(void)
{
    static unsigned char    bytes[57];
    unsigned char           *enc, *dec;
    size_t                  n, m;
    char                    msg[64];
    int                     i;

    printf("=== TSTB64: base64 ===\n");

    /* RFC 4648 section 10, both ways */
    for (i = 0; i <= 6; i++) {
        enc = base64_encode(foobar, (size_t)i, &n);
        sprintf(msg, "encode %d bytes of foobar", i);
        CHECK(enc && n == strlen(rfc[i]) && strcmp((char *)enc, rfc[i]) == 0,
              msg);
        free(enc);

        if (i == 0) continue;           /* decode of "" is NULL, below */
        dec = base64_decode((const unsigned char *)rfc[i], strlen(rfc[i]), &m);
        sprintf(msg, "decode \"%s\"", rfc[i]);
        CHECK(dec && m == (size_t)i && memcmp(dec, foobar, m) == 0, msg);
        free(dec);
    }

    enc = base64_encode(foobar, 6, NULL);
    CHECK(enc != NULL, "encode takes a NULL out_len");
    free(enc);

    /* 57 bytes = 76 characters: a '\n' after the 72nd */
    for (i = 0; i < 57; i++) bytes[i] = (unsigned char)i;
    enc = base64_encode(bytes, sizeof bytes, &n);
    CHECK(enc && n == 77 && enc[72] == '\n'
          && memcmp(enc, "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8g"
                         "ISIjJCUmJygpKissLS4vMDEyMzQ1", 72) == 0
          && strcmp((char *)enc + 73, "Njc4") == 0,
          "57 bytes: 72 characters, '\\n', 4 characters");

    /* and back, across the line break */
    dec = enc ? base64_decode(enc, n, &m) : NULL;
    CHECK(dec && m == sizeof bytes && memcmp(dec, bytes, m) == 0,
          "decode skips the '\\n' and round-trips 57 bytes");
    free(dec);
    free(enc);

    dec = base64_decode((const unsigned char *)"", 0, &m);
    CHECK(dec == NULL, "decode of no symbols is NULL");
    free(dec);

    dec = base64_decode((const unsigned char *)"Zm9", 3, &m);
    CHECK(dec == NULL, "decode of 3 symbols is NULL");
    free(dec);

    dec = base64_decode((const unsigned char *)"Z===", 4, &m);
    CHECK(dec == NULL, "decode of three '=' is NULL");
    free(dec);

    return mbt_test_summary("TSTB64");
}
