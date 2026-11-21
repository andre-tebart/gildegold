/* shp_bank.h -- what d2.h / unk_47ab30.h lack for 0x47c5e0..0x47d960 (in unk_47ab30.c and shp_asm.c since round 20; the shape part
   continued: adding a shape to a bank, drawing shapes of a bank, animated shapes, the shape drawing loops written
   in inline assembly). Names are ours. */
#ifndef SHP_BANK_H
#define SHP_BANK_H

#include "d2.h"
#include "shp.h"

#pragma pack(push, 1)
typedef struct D2ANIM {             /* 0x11 bytes, d2_Anims[16]: a playing animation of a bank */
    D2BANK *bank;                   /* 0x00, 0: free */
    signed char frame;              /* 0x04, entry in the bank's table at pal, -1: stopped */
    char timer;                     /* 0x05 */
    unsigned short shape;           /* 0x06, shape drawn now */
    int shadow;                     /* 0x08, bit 0: draw a half dark copy first */
    char back;                      /* 0x0c, 1: play backwards */
    short x, y;                     /* 0x0d */
} D2ANIM;                           /* 0x11 */
#pragma pack(pop)

extern D2ANIM d2_Anims[16];     /* 0xf8a520 */
extern unsigned int d2_BlitW;   /* 0xf8a634 */
extern unsigned int d2_ShpPalTable;   /* 0xf8a644 */
extern unsigned int d2_ShpRows;   /* 0xf8a118 */
extern int d2_ShpDataOfs;            /* 0x5fef00 */

void d2_ShpDarken16(void *dst, int w, int h);                                 /* 0x47d960 */

#endif
