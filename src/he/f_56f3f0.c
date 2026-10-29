/* 0x56f3f0, the domicile message of the construction run (a = the building id, b = the record: the word at 0 is one person, the dword at +4 the other): when 0x56f3b0 (b) is free and 0x501190 (1, 1, a) finds, the player word at building +0x27 selects the character record; the checks (0x504ec0 (rec, 2) - 0x504ec0 (b, 2) * 0.5f) + 0x42ec20 () * 0.5f >= 0.8 and the same over the field 3 >= 0.7: both -> the texts 0x4031 / 0x4033 and a message 0x9b (0x4c4b70), else the texts 0x4032 / 0x4034 */

int he_Unk56f3f0(int a, unsigned short *b)
{
    char text[0x400];
    char msg[0xf8];
    int bld, p;
    float f1, f2, f3;

    if (he_Unk56f3b0((int)b) != 0) {
        bld = gm_BuildQuery(1, 1, a);
        if (bld != 0) {
            p = *(unsigned short *)(bld + 0x27);
            f1 = gm_GetSkill((unsigned)&ai_Chars + p * 0x21c, 2);
            f2 = gm_GetSkill(b, 2);
            f3 = util_RandomF();
            if (f1 - f2 * 0.5f + f3 * 0.5 >= 0.8) {
                f1 = gm_GetSkill((unsigned)&ai_Chars + p * 0x21c, 3);
                f2 = gm_GetSkill(b, 3);
                f3 = util_RandomF();
                if (f1 - f2 * 0.5f + f3 * 0.5 >= 0.7) {
                    txt_Format(text, 0x4031, *b);
                    he_ShowNews(*(int *)((unsigned)&ai_CharInts + p * 0x21c), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_628e28);
                    txt_Format(text, 0x4033);
                    he_ShowNews(*(int *)(b + 2), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e10);
                    memset(msg, 0, 0xf8);
                    msg[4] = 0x9b;
                    HM_I(msg, 0xc) = a;
                    msg[0x36] = 2;
                    HM_I(msg, 8) = *(int *)((unsigned)&ai_CharInts + p * 0x21c);
                    HM_I(msg, 0x58) = *(int *)(b + 2);
                    HM_I(msg, 0x5c) = *(int *)((unsigned)&ai_CharInts + p * 0x21c);
                    cm_SendLongMessage(msg);
                    return 1;
                }
            }
            txt_Format(text, 0x4032, *b, *b);
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + p * 0x21c), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_628df8);
            txt_Format(text, 0x4034);
            he_ShowNews(*(int *)(b + 2), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e10);
        }
    }
    return 0;
}
