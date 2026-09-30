/* SHA256U.C
** SHA-256 incremental update.
** Based on Brad Conte's crypto-algorithms (public domain).
*/
#include "sha256.h"

void
sha256_update(SHA256_CTX *ctx, const unsigned char data[], size_t len)
{
    unsigned int i;

    for (i = 0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64) {
            sha256_transform(ctx, ctx->data);
            ctx->bitlen[1] += 512;
            if (ctx->bitlen[1] < 512) ctx->bitlen[0]++;     /* carry */
            ctx->datalen = 0;
        }
    }
}
