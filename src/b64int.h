#ifndef B64INT_H
#define B64INT_H
/* b64int.h - internal to crypto370: the table b64enc.c and b64dec.c share */
#include "base64.h"

extern const unsigned char base64_table[65]                          asm("B64TBL");

#endif
