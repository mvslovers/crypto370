#ifndef TSTHEX_H
#define TSTHEX_H
/* tsthex.h - shared by the crypto370 tests: compare bytes against a hex
** string.  The digits are matched as character literals, so the same
** test source works on MVS (EBCDIC) and on the host (ASCII). */
#include <stddef.h>

static int tst_hexval(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    switch (c) {
    case 'a': case 'A': return 10;
    case 'b': case 'B': return 11;
    case 'c': case 'C': return 12;
    case 'd': case 'D': return 13;
    case 'e': case 'E': return 14;
    case 'f': case 'F': return 15;
    }
    return -1;
}

/* hex -> bytes; returns the byte count, or -1 on a bad digit/overflow */
static int tst_unhex(const char *hex, unsigned char *out, size_t max)
{
    size_t  n = 0;
    int     hi, lo;

    while (hex[0] && hex[1]) {
        hi = tst_hexval(hex[0]);
        lo = tst_hexval(hex[1]);
        if (hi < 0 || lo < 0 || n >= max) return -1;
        out[n++] = (unsigned char)(hi * 16 + lo);
        hex += 2;
    }
    return hex[0] ? -1 : (int)n;
}

/* 1 when the n bytes at got are the bytes the hex string spells */
static int tst_hexeq(const unsigned char *got, size_t n, const char *hex)
{
    unsigned char   want[64];
    size_t          i;

    if (tst_unhex(hex, want, sizeof want) != (int)n) return 0;
    for (i = 0; i < n; i++) {
        if (got[i] != want[i]) return 0;
    }
    return 1;
}

#endif
