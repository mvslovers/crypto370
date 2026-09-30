/* tstsha.c - SHA-256 against the FIPS 180-2 known answers.
**
** The messages are ASCII bytes, never C string literals: on MVS "abc" is
** EBCDIC and hashes to something else.  The expected digests were
** re-derived with shasum -a 256 on the host.
*/
#include <stdio.h>
#include <string.h>
#include <mbtcheck.h>
#include "sha256.h"
#include "tsthex.h"

static int digest_is(const unsigned char *msg, size_t len, const char *hex)
{
    SHA256_CTX      ctx;
    unsigned char   hash[SHA256_BLOCK_SIZE];

    sha256_init(&ctx);
    sha256_update(&ctx, msg, len);
    sha256_final(&ctx, hash);
    return tst_hexeq(hash, sizeof hash, hex);
}

int main(void)
{
    static const unsigned char abc[] = { 0x61, 0x62, 0x63 };   /* "abc" */
    static unsigned char    msg[56];
    static unsigned char    chunk[1000];
    SHA256_CTX              ctx;
    unsigned char           hash[SHA256_BLOCK_SIZE];
    int                     i, j;

    printf("=== TSTSHA: SHA-256 known answers ===\n");

    CHECK(digest_is(abc, 0,
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"),
        "empty message");

    CHECK(digest_is(abc, sizeof abc,
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"),
        "\"abc\" (one block)");

    /* "abcdbcdecdefdefg...nopq": 14 groups of four, group i starts at
       'a'+i - 448 bits, so the padding needs a second block */
    for (i = 0; i < 14; i++)
        for (j = 0; j < 4; j++)
            msg[i * 4 + j] = (unsigned char)(0x61 + i + j);
    CHECK(digest_is(msg, sizeof msg,
        "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"),
        "448-bit message (two blocks)");

    /* one million 'a', fed a thousand at a time: 15625 blocks, most of
       them completed across an update boundary, and a bit count
       (8 000 000) the final block has to carry in its length field */
    memset(chunk, 0x61, sizeof chunk);
    sha256_init(&ctx);
    for (i = 0; i < 1000; i++)
        sha256_update(&ctx, chunk, sizeof chunk);
    sha256_final(&ctx, hash);
    CHECK(tst_hexeq(hash, sizeof hash,
        "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"),
        "one million 'a' in 1000-byte updates");

    return mbt_test_summary("TSTSHA");
}
