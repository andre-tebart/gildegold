/* 0x505230, lets the character c (words +0x2c: eight bit fields of 4 / 4 / 2 / 3 / 3 / 3 / 2 / 4 bits at 4, 8, 12, 14, 17, 20, 23, 25) change one of its empty fields: a random start field (0 .. 7), the first field from there whose bits are all 0; a random value (0 .. the record limit of 0x5051d0 (field + 2)) is xored in, the new word is sent (0x4c3c00 (4, 1, 0x2c)) and the health changes by the cost (0x4fca40 (id, -the float of the record)); *kind = the field number + 2, *health = the new health. 1 when a field changed, else 0. ROUGH */

int gm_Unk505230(short *c, unsigned *kind, float *health)
{
    unsigned w, f, v, s, x;
    int free_[8];
    int i, rec[3], cost;
    float h;

    w = *(unsigned *)(c + 0x16);
    free_[0] = (w & 0xf0) == 0;
    free_[1] = (w & 0xf00) == 0;
    free_[2] = (w & 0x3000) == 0;
    free_[3] = (w & 0x1c000) == 0;
    free_[4] = (w & 0xe0000) == 0;
    free_[5] = (w & ((unsigned)&he_Events + 0x54c0)) == 0;
    free_[6] = (w & 0x1800000) == 0;
    free_[7] = (w & 0x1e000000) == 0;
    f = util_RandomU(8) & 0xffff;
    i = 8;
    while (free_[f] != 1) {
        i--;
        f = (f + 1) % 8;
        if (i == 0)
            return 0;
    }
    if ((int)f < 0 || 7 < (int)f)
        return 0;
    s = *(unsigned *)(c + 0x16);
    f = f + 2;
    if (gm_Unk5051d0(f, rec) == 0)
        return 0;
    v = util_RandomU(rec[2]) & 0xffff;
    cost = (int)(float)rec[0];
    if (kind != 0)
        *kind = -(v != 0) & f;
    if (v == 0)
        return 0;
    if (f == 2)
        x = (v << 4 ^ s) & 0xf0;
    else if (f == 3)
        x = (v << 8 ^ s) & 0xf00;
    else if (f == 4)
        x = (v << 0xc ^ s) & 0x3000;
    else if (f == 5)
        x = (v << 0xe ^ s) & 0x1c000;
    else if (f == 6)
        x = (v << 0x11 ^ s) & 0xe0000;
    else if (f == 7)
        x = (v << 0x14 ^ s) & ((unsigned)&he_Events + 0x54c0);
    else if (f == 8)
        x = (v << 0x17 ^ s) & 0x1800000;
    else if (f == 9)
        x = (v << 0x19 ^ s) & 0x1e000000;
    else
        goto send;
    s = s ^ x;
send:
    cm_BeginFieldBlock(c, *(int *)(c + 2));
    cm_AddFieldDelta(4, 1, 0x2c, &s);
    cm_SendFieldDeltas();
    h = gm_AddHealth(*c, -cost);
    if (health == 0)
        return 1;
    *health = h;
    return 1;
}
