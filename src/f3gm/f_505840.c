/* 0x505840, clears one set field of the word +0x2c of the character c (a random start field of 8, the first that is not 0) and sends the word; returns the record number (2 .. 9) or 0 */

int gm_Unk505840(char *c)
{
    unsigned w, f;
    int tab[8];
    int i, r;

    r = 0;
    w = *(unsigned *)(c + 0x2c);
    tab[0] = (w & 0xf0) != 0;
    tab[1] = (w & 0xf00) != 0;
    tab[2] = (w & 0x3000) != 0;
    tab[3] = (w & 0x1c000) != 0;
    tab[4] = (w & 0xe0000) != 0;
    tab[5] = (w & ((unsigned)&he_Events + 0x54c0)) != 0;
    tab[6] = (w & 0x1800000) != 0;
    tab[7] = (w & 0x1e000000) != 0;
    f = util_RandomU(8) & 0xffff;
    i = 8;
    do {
        if (tab[f] == 1) {
            if (f == 0xffffffff)
                return 0;
            w = *(unsigned *)(c + 0x2c);
            if (f == 0) {
                w &= 0xffffff0f;
                r = 2;
            } else if (f == 1) {
                w &= 0xfffff0ff;
                r = 3;
            } else if (f == 2) {
                w &= 0xffffcfff;
                r = 4;
            } else if (f == 3) {
                w &= 0xfffe3fff;
                r = 5;
            } else if (f == 4) {
                w &= 0xfff1ffff;
                r = 6;
            } else if (f == 5) {
                w &= 0xff8fffff;
                r = 7;
            } else if (f == 6) {
                w &= 0xfe7fffff;
                r = 8;
            } else if (f == 7) {
                w &= 0xe1ffffff;
                r = 9;
            }
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldDelta(4, 1, 0x2c, &w);
            cm_SendFieldDeltas();
            return r;
        }
        i--;
        f = (f + 1) % 8;
    } while (i != 0);
    return 0;
}
