/* 0x4fca40, changes the health of the character id by d (kind 0x14: the float +0x1c directly, dies below 0 through the message 0x72; others: the health of 0x4fc820's formula plus d, the int +0x24 = d through 0x4c3d00): -1 for a free slot, kind 10 or an inactive one; returns the new health, 0 when the character died. ROUGH */

float gm_AddHealth(unsigned id, int d)
{
    char m[0xf8];
    char b;
    char *c;
    float a, f, fc, lim;

    id &= 0xffff;
    c = ai_Chars[id];
    if (*(short *)c == -1 || c[2] == 10 || c[8] == 0)
        return -1.0f;
    if (c[2] == 0x14) {
        a = (float)d;
        fc = *(float *)(c + 0x1c) + a;
        if (fc <= 0.0f) {
            b = 0;
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldValue(1, 1, (int)c + (8 - cm_FieldObjectI), &b);
            cm_SendFieldValues();
            G_MSG72(m, c);
            return 0.0f;
        }
        ai_Unk4c4ac0(*(int *)(c + 4), 0x1c, a);
        return fc;
    }
    a = (float)*(unsigned short *)(c + 10);
    lim = *(float *)(c + 0x20);
    if (a > lim)
        f = *(float *)(c + 0x1c);
    else
        f = ((*(float *)(c + 0x1c) - *(float *)(c + 0x10)) / lim) * a + *(float *)(c + 0x10);
    f = *(int *)(c + 0x24) + d + f;
    if (f <= 0.0f)
        f = 0.0f;
    fc = f;
    if (f <= 0.0f) {
        b = 0;
        cm_BeginFieldBlock(c, *(int *)(c + 4));
        cm_AddFieldValue(1, 1, (int)c + (8 - cm_FieldObjectI), &b);
        cm_SendFieldValues();
        G_MSG72(m, c);
        return 0.0f;
    }
    cm_BeginFieldBlock(c, *(int *)(c + 4));
    cm_AddFieldValues(4, 1, 0x24, &d);
    cm_SendFieldDeltas();
    return fc;
}
