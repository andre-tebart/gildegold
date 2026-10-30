/* 0x5483f0 (the Ghidra function 0x548410 starts here with a free in the state -2), the event "a figure in the party of the player is hit": the news 0x10c2 (the player name 0x7e3f60 + 0x1fc * party, 0x10c4 + random(3)) and for every character (0x770b80 table, kind 5 / 6 / 7 with a level above 1) the 0x4a63f0 search, the amount 0x50a640 * 0.. * (random(4) + 2) (0x4c4830) and the news 0x10c3 (ROUGH) */

void he_Unk5483f0(int ev)
{
    char text[0x200];
    int rec, o = 0, a, v, lv;

    if (*(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    txt_Format(text, 0x10c2, ((unsigned)&ai_PlayerSlots + 0x20) + U_I((unsigned)&ai_OwnPlayer) * 0x1fc, (unsigned short)util_Random(3) + 0x10c4);
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
        if (*(unsigned short *)rec != 0xffff && *(unsigned char *)(rec + 8) >= 2
            && (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7 || *(char *)(rec + 2) == 5)) {
            float lim;
            int n;
            int fb;
            n = AI_4A63F0_7(rec, 4, 0x400080, 0, 0, (int)0x42c80000, &fb);
            if (n != 0)
                gm_FindById(&o, 0, 0, fb);
            if (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7)
                he_ShowNews(*(int *)(rec + 2 + 2), -1, text, 0x20, 0x601, (char *)0);
            if (o != 0) {
                v = gm_GetWorth(*(unsigned short *)rec);
                lv = (int)((float)v * 1.0f);
                lv *= (unsigned short)util_Random(4) + 2;
                cm_SendMoveGoodsUnchecked(*(int *)(rec + 4), -1, U_I((unsigned)&ai_OwnPlayer), lv);
                if (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7) {
                    txt_Format(text, 0x10c3, o, lv);
                    he_ShowNews(*(int *)(rec + 4), -1, text, 0, 0x5ff, (char *)0);
                }
            }
        }
    }
    he_Unk53e1f0((int *)ev);
}
