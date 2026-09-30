/* tstbfish.c - Blowfish against Eric Young's published ECB vectors.
**
** Every vector was re-derived with openssl enc -bf-ecb (legacy provider,
** the 8-byte key given twice: OpenSSL pads a short key with zeros, and
** Blowfish's key schedule cycles the key, so KK is the same key as K).
** Each block is also decrypted back.
*/
#include <stdio.h>
#include <mbtcheck.h>
#include "blowfish.h"
#include "tsthex.h"

static const char *const vec[][3] = {
    /* key                  plaintext           ciphertext */
    { "0000000000000000", "0000000000000000", "4EF997456198DD78" },
    { "FFFFFFFFFFFFFFFF", "FFFFFFFFFFFFFFFF", "51866FD5B85ECB8A" },
    { "3000000000000000", "1000000000000001", "7D856F9A613063F2" },
    { "1111111111111111", "1111111111111111", "2466DD878B963C9D" },
    { "0123456789ABCDEF", "1111111111111111", "61F9C3802281B096" },
    { "FEDCBA9876543210", "0123456789ABCDEF", "0ACEAB0FC6A0A28D" },
};

int main(void)
{
    static BLOWFISH_KEY     ks;         /* 4168 bytes: not on the stack */
    unsigned char           key[8], pt[8], ct[8], back[8];
    char                    msg[64];
    unsigned                i;

    printf("=== TSTBFISH: Blowfish ECB known answers ===\n");

    for (i = 0; i < sizeof vec / sizeof vec[0]; i++) {
        tst_unhex(vec[i][0], key, sizeof key);
        tst_unhex(vec[i][1], pt, sizeof pt);

        blowfish_key_setup(key, &ks, sizeof key);
        blowfish_encrypt(pt, ct, &ks);
        sprintf(msg, "vector %u encrypts", i + 1);
        CHECK(tst_hexeq(ct, sizeof ct, vec[i][2]), msg);

        blowfish_decrypt(ct, back, &ks);
        sprintf(msg, "vector %u decrypts back", i + 1);
        CHECK(tst_hexeq(back, sizeof back, vec[i][1]), msg);
    }

    return mbt_test_summary("TSTBFISH");
}
