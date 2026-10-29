/* 0x540e40, the inn report: for every inn (kind 5 / 0x16) with an owner (flag 1 at +0x5a clear) whose figure is kind 6 / 7 and has guests (+0x69, +0x6b shorts): message 0x18a1 (ratio < 0.5) or 0x18a2 with the numbers, then resets the counters (+0x65 -> +0x6d, 0 -> +0x65, +0x69, +0x6b) (ROUGH) */

void he_Unk540e40(void)
{
    int f;
    int c1, c2, v1, v2;
    char buf1[512], buf2[512];
    char *txt;
    char *t2;

    f = gm_BuildQuery(1, 5, 0x16);
    while (f != 0) {
        if (*(unsigned short *)(f + 0x27) != 0xffff && (*(unsigned char *)(f + 0x5a) & 1) == 0) {
            c2 = 0;
            c1 = 0;
            if ((*(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(f + 0x27) * 0x21c) == 6 || *(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(f + 0x27) * 0x21c) == 7)
                && *(short *)(f + 0x69) > 0) {
                v2 = (int)*(short *)(f + 0x6b);
                v1 = (int)*(short *)(f + 0x69);
                if (0.5 <= (double)v2 / (double)v1) {
                    txt_Format(buf2, 0x18a2, f, v1, *(int *)(f + 0x65));
                    t2 = (char *)(unsigned)&ch_EmptyName;
                    txt = buf2;
                } else {
                    txt_Format(buf1, 0x18a1, f, v1, *(int *)(f + 0x65));
                    t2 = "_NACHRICHTEN_HS_82";
                    txt = buf1;
                }
                he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(f + 0x27) * 0x21c), -1, txt, 0, 0x5fc, *(int *)(f + 1), -1, 0, t2);
            }
            cm_BeginFieldBlock(f, *(int *)(f + 1));
            cm_AddFieldDelta(4, 1, f - U_I((unsigned)&cm_FieldObject) + 0x6d, f + 0x65);
            cm_AddFieldDelta(4, 1, f - U_I((unsigned)&cm_FieldObject) + 0x65, &c2);
            cm_AddFieldDelta(2, 1, f - U_I((unsigned)&cm_FieldObject) + 0x6b, &c1);
            cm_AddFieldDelta(2, 1, f - U_I((unsigned)&cm_FieldObject) + 0x69, &c1);
            cm_SendFieldDeltas();
        }
        f = gm_BuildNext();
    }
}
