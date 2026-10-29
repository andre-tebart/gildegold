/* 0x4faa00, for each active character: the three ints at +0x1d8 .. summed with the 0x50a640 value of the character (the best one is found by 0x4fad60), written through 0x4c3c00 (4, 1, ..) to +0x1d4 .. and +0x1e0, and the float messages 0x4c4ac0 (0x1e4) / 0x4c3eb0. ROUGH, the float code is far off */

void unk_Unk4faa00(void)
{
    short id;
    int top;
    char *c;
    int i, sum, a, loc;
    float cnt, fv, q;
    double lim;
    int *pi;

    gm_FindRichest(&id, &top);
    ai_Unk6f1b98 = 0;
    for (c = ai_Chars[0]; (int)c < (unsigned)((unsigned)ai_Chars + 0x65400); c += 0x21c) {
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
        if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[8] != 0) {
            sum = 0;
            cnt = 0.0f;
            a = I50A640(*(short *)c);
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            pi = (int *)(c + 0x1d8);
            for (i = 0; i < 3; i++) {
                loc = *pi;
                sum += loc;
                cnt = (float)((int)cnt + (loc != 0));
                cm_AddFieldDelta(4, 1, 0x1d4 + i * 4 + (int)c, &loc);
                pi++;
            }
            cnt = (float)((a + sum) / ((int)cnt + 1));
            cm_AddFieldDelta(4, 1, 0x1e0 + (int)c, &a);
            fv = (float)(int)cnt / (float)a;
            if (fv >= 1.0f)
                lim = 1.0;
            else
                lim = (double)fv;
            q = 1.0f - (float)a / (float)top;
            cm_SendFieldDeltas();
            ai_Unk6f1b98++;
            ai_Unk4c4ac0(*(int *)(c + 4), 0x1e4, -(*(float *)(c + 0x1e4) - ((float)lim + q) * 0.5));
            q = -(*(float *)(c + 0xb4) - *(float *)(c + 0xb4) * 0.95);
            ai_Unk4c3e70(c);
            ai_Unk4c3eb0(3, q);
            ai_Unk4c4a20();
            ai_Unk6f1b98++;
        }
    }
    ai_Unk6f1b98 = 0;
}
