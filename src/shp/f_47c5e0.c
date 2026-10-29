/* 0x47c5e0, stores a palette into a bank */

/* d2.h has other types for these three */
int d2_DrawShapeHalfDark(int x, int y, void *surf, void *pic, char i);              /* 0x47c770, d2.h has void */
int d2_StartAnim(short x, short y, void *data, unsigned char z);             /* 0x47cd20, d2.h has int z */
int d2_DrawShapeMode2(int x, int y, void *surf, void *pic, unsigned char i);     /* 0x47c970, d2.h has void */

int d2_BankSetPalette(D2BANK *bank, void *pal)
{
    if (bank == 0)
        return 0;
    memcpy((char *)bank + 0x445, pal, 0x400);
    return 1;
}
