/* 0x5054f0, raises one of the four fields 0 .. 3 (records 2 .. 5 of 0x5051d0) of the character c that is empty (a random start), by a random amount up to the record's limit (+0x4 scaled by 0x5051d0); the word is sent (0x4c3c00 (4, 1, 0x2c)) and the cost paid (0x4fca40). Returns the record number or 0. ROUGH */

/* the bit fields of the word +0x2c of a character (the skill levels): field k starts at the bit FIELD_SHIFT[k] with the mask FIELD_MASK[k] (4, 4, 2, 3, 3, 3, 2, 4 bits) */

unsigned gm_Unk5054f0(short *c)
{
    unsigned w, f, x, v;
    unsigned tab[8];
    int i, cost;
    int rec[3];

    w = *(unsigned *)(c + 0x16);
    tab[0] = 2;
    tab[1] = 3;
    tab[2] = 4;
    tab[3] = 5;
    tab[4] = (w & 0xf0) == 0;
    tab[5] = (w & 0xf00) == 0;
    tab[6] = (w & 0x3000) == 0;
    tab[7] = (w & 0x1c000) == 0;
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
    if (f == 2)
        x = (v << 4 ^ w) & 0xf0;
    else if (f == 3)
        x = (v << 8 ^ w) & 0xf00;
    else if (f == 4)
        x = (v << 0xc ^ w) & 0x3000;
    else {
        if (f != 5)
            goto send;
        x = (v << 0xe ^ w) & 0x1c000;
    }
    w = w ^ x;
send:
    cm_BeginFieldBlock(c, *(int *)(c + 2));
    cm_AddFieldDelta(4, 1, 0x2c, &w);
    cm_SendFieldDeltas();
    gm_AddHealth(*c, -cost);
    return f;
}
