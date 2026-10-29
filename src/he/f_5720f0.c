/* 0x5720f0, the skill lottery of the record rec (the field at +0x2c): the chance f must beat 0x42ec20 (), then the kind 2 / 1 / other picks the free bit range a or b (2 / 3, 4 / 5, 6 and 7 / 8 / 9 by 0x42ec00 (2) and the taken bits 0x1800000 / 0x700000); a when b is 0, else b by 0x42ec00 (2); 0 when none. 0x5051d0 (a, buf) gives the max (buf + 8) of 0x42ec00; n = that * f; the field gets (v << shift ^ field) & mask for a; 0x4c3ba0 (rec, +4), 0x4c3c00 (4, 1, 0x2c, &field), 0x4c49a0 (); 0x4fca40 (*rec, -n) into out; a */

int gm_Unk5051d0();                 /* 0x5051d0 */
int cm_AddFieldDelta(int size, int n, short field, void *v);                 /* 0x4c3c00 */

int he_Unk5720f0(unsigned short *rec, float f, int kind, float *out)
{
    char buf[12];
    unsigned int field, v, x;
    int a = 0, b = 0, n;
    float res;

    if (f <= util_RandomF())
        return 0;
    if (kind == 2) {
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0xf0) == 0)
            a = 2;
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0xf00) == 0)
            b = 3;
    } else if (kind == 1) {
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0x3000) == 0)
            a = 4;
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0x1c000) == 0)
            b = 5;
    } else {
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0xe0000) == 0)
            a = 6;
        b = 8 - (util_Random(2) != 0);
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0x1800000) != 0 && b == 8)
            b = 0;
        if ((*(unsigned int *)((char *)rec + 0x2c) & ((unsigned)&he_Events + 0x54c0)) != 0 && b == 7)
            b = 0;
    }
    if (a == 0) {
        if (b == 0)
            return 0;
        a = b;
    } else if (b != 0 && util_Random(2) == 0)
        a = b;
    field = *(unsigned int *)((char *)rec + 0x2c);
    if (gm_Unk5051d0(a, buf) == 0)
        return a;
    v = util_Random(*(int *)(buf + 8)) & 0xffff;
    n = (int)((float)v * f);
    if (v == 0)
        return a;
    switch (a) {
    case 2: v = (v << 4 ^ field) & 0xf0; break;
    case 3: v = (v << 8 ^ field) & 0xf00; break;
    case 4: v = (v << 0xc ^ field) & 0x3000; break;
    case 5: v = (v << 0xe ^ field) & 0x1c000; break;
    case 6: v = (v << 0x11 ^ field) & 0xe0000; break;
    case 7: v = (v << 0x14 ^ field) & ((unsigned)&he_Events + 0x54c0); break;
    case 8: v = (v << 0x17 ^ field) & 0x1800000; break;
    case 9: v = (v << 0x19 ^ field) & 0x1e000000; break;
    default: goto done;
    }
    field ^= v;
done:
    cm_BeginFieldBlock(rec, *(int *)((char *)rec + 4));
    cm_AddFieldDelta(4, 1, 0x2c, &field);
    cm_SendFieldDeltas();
    res = ((float (*)(unsigned short, int))gm_AddHealth)(*rec, -n);
    if (out != 0)
        *out = res;
    return a;
}
