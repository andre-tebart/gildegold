/* 0x5056a0, like 0x5054f0 for the fields 3 .. 6 (records 5 .. 8) */

unsigned gm_Unk5056a0(short *c)
{
    unsigned w, f, x, v;
    unsigned tab[8];
    int i, cost;
    int rec[3];

    w = *(unsigned *)(c + 0x16);
    tab[0] = 5;
    tab[1] = 6;
    tab[2] = 7;
    tab[3] = 8;
    tab[4] = (w & 0x1c000) == 0;
    tab[5] = (w & 0xe0000) == 0;
    tab[6] = (w & ((unsigned)&he_Events + 0x54c0)) == 0;
    tab[7] = (w & 0x1800000) == 0;
    f = util_Random(4) & 0xffff;
    i = 4;
    while (tab[f + 4] != 1) {
        i--;
        f = (f + 1) % 4;
        if (i == 0)
            return 0;
    }
    f = tab[f];
    if (gm_Unk5051d0(f, rec) == 0)
        return 0;
    v = util_Random((int)(float)rec[2]) & 0xffff;
    if (v == 0)
        return 0;
    cost = (int)(float)rec[0];
    w = *(unsigned *)(c + 0x16);
    if (f == 5 || f == 8)
        x = (v << 0x17 ^ w) & 0x1800000;
    else if (f == 6)
        x = (v << 0x11 ^ w) & 0xe0000;
    else if (f == 7)
        x = (v << 0x14 ^ w) & ((unsigned)&he_Events + 0x54c0);
    else
        goto send;
    w = w ^ x;
send:
    cm_BeginFieldBlock(c, *(int *)(c + 2));
    cm_AddFieldDelta(4, 1, 0x2c, &w);
    cm_SendFieldDeltas();
    gm_AddHealth(*c, -cost);
    return f;
}
