/* 0x505ad0, clears one set field of the fields 4 / 5 / 6 / 1 (a random start of 4); returns the record number (6 / 7 / 8 / 3) or 0 */

int gm_Unk505ad0(char *c)
{
    unsigned w, f;
    int tab[4];
    int i, r;

    r = 0;
    w = *(unsigned *)(c + 0x2c);
    tab[0] = (w & 0xe0000) != 0;
    tab[1] = (w & ((unsigned)&he_Events + 0x54c0)) != 0;
    tab[2] = (w & 0x1800000) != 0;
    tab[3] = (w & 0xf00) != 0;
    f = util_RandomU(4) & 0xffff;
    i = 4;
    do {
        if (tab[f] == 1) {
            if (f == 0xffffffff)
                return 0;
            w = *(unsigned *)(c + 0x2c);
            if (f == 0) {
                w &= 0xfff1ffff;
                r = 6;
            } else if (f == 1) {
                w &= 0xff8fffff;
                r = 7;
            } else if (f == 2) {
                w &= 0xfe7fffff;
                r = 8;
            } else if (f == 3) {
                w &= 0xfffff0ff;
                r = 3;
            }
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldDelta(4, 1, 0x2c, &w);
            cm_SendFieldDeltas();
            return r;
        }
        i--;
        f = (f + 1) % 4;
    } while (i != 0);
    return 0;
}
