/* 0x4fb0e0, every fourth day (the clock int % 4 == 0): by the record 2 (rate 2: a random loyalty shift for the characters above 0xb, 0x4c3c00 (1, 1, 0xc, ..); other rates: 0x4f2e30 (2, ..) and a loyalty drop of 0.1) over the characters of kinds 1 .. 9 except 6 / 7 / 8. ROUGH */

void unk_Unk4fb0e0(void)
{
    int r[9];
    int i, t, m;
    char *c;
    unsigned loc;

    if (*(int *)ai_Time % 4 != 0)
        return;
    gv_GetTableRecord(2, r);
    if (r[6] == 2) {
        ai_Unk6f1b98 = 0;
        for (i = 0; i < 0x300; i++) {
            c = ai_Chars[i];
            if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[2] != 0 && c[2] != 6 && c[2] != 7 && c[2] != 8 && *(unsigned short *)(c + 10) > 0xb && *(float *)(c + 0x1d0) < util_RandomF2() * 0.2) {
                loc = c[0xc] == 0;
                ai_Unk4c4ac0(*(int *)(c + 4), 0x1d0, (float)(util_RandomF2() * 0.5 + 0.25));
                ai_Unk6f1b98++;
                cm_BeginFieldBlock(c, *(int *)(c + 4));
                cm_AddFieldDelta(1, 1, 0xc, &loc);
                cm_SendFieldDeltas();
                ai_Unk6f1b98++;
                if (ai_Unk6f1b98 > 0x40) {
                    sv_PumpNet();
                    ai_Unk6f1b98 = 0;
                }
            }
        }
        ai_Unk6f1b98 = 0;
        return;
    }
    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[2] != 0 && c[2] != 6 && c[2] != 7 && c[2] != 8 && *(unsigned short *)(c + 10) > 0xb && (c[0xc] != 0) != (r[6] != 0)) {
            t = -1;
            if (*(int *)(c + 0x170) != 0)
                t = *(int *)(*(int *)(c + 0x170) + 1);
            m = unk_Unk4f2e30(2, r[6] == 0, *(int *)(c + 4), -1, t);
            if (m != -1)
                ai_Unk6f1b98++;
            if (*(float *)(c + 0x1d0) > 0.1)
                ai_Unk4c4ac0(*(int *)(c + 4), 0x1d0, -0.1f);
            ai_Unk6f1b98++;
            if (ai_Unk6f1b98 > 0x40) {
                sv_PumpNet();
                ai_Unk6f1b98 = 0;
            }
        }
    }
    ai_Unk6f1b98 = 0;
}
