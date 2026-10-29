/* 0x5059d0, clears one set field of the first four (a random start field of 4); returns the record number (2 .. 5) or 0 */

int gm_Unk5059d0(char *c)
{
    unsigned w, f;
    int tab[4];
    int i, r;

    r = 0;
    w = *(unsigned *)(c + 0x2c);
    tab[0] = (w & 0xf0) != 0;
    tab[1] = (w & 0xf00) != 0;
    tab[2] = (w & 0x3000) != 0;
    tab[3] = (w & 0x1c000) != 0;
    f = util_Random(4) & 0xffff;
    i = 4;
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
