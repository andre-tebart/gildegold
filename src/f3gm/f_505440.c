/* 0x505440, lets the character c (when the low 4 bits of +0x2c are 0) get a random first field value (record 1 of 0x5051d0), sent as in 0x505230, and pays its cost (0x4fca40). Returns the new health or 0. ROUGH */

float gm_Unk505440(short *c)
{
    unsigned w, v, nw;
    int rec[3];
    int cost;

    w = *(unsigned *)(c + 0x16);
    if ((w & 0xf) != 0)
        return 0.0f;
    if (gm_Unk5051d0(1, rec) == 0)
        return 0.0f;
    v = util_RandomU(rec[2]);
    if ((v & 0xffff) == 0)
        return 0.0f;
    nw = ((v & 0xffff) ^ w) & 0xf ^ w;
    cm_BeginFieldBlock(c, *(int *)(c + 2));
    cm_AddFieldDelta(4, 1, 0x2c, &nw);
    cm_SendFieldDeltas();
    cost = (int)(float)rec[0];
    return gm_AddHealth(*c, -cost);
}
