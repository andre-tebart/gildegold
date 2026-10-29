/* 0x4c88a0, handler of type 0x1e: the game time (an AITIME); when it is later than ours, the clock moves on */

int ai_Unk4c88a0(G17MSG *m, G17STAT *s)
{
    char *t = (char *)m + 0x10;
    char *p;

    if (gm_CompareTime(ai_Time, t) < 0) {
        *(G17T14 *)ai_Time = *(G17T14 *)t;
        if (ai_Unk624578)
            he_Unk53ed70();
        if (ai_Unk624574) {
            ord_AdvanceRatingBase();
            if (*(int *)&ai_Time[6] % 6 == 0)
                unk_Unk4fdfd0();
        }
        if (ai_Unk624570)
            ai_UpdateNeeds();
        memset(ai_Unkf895a0, 0, 0xb00);
        if (ai_Unk624570) {
            ai_TradeMsgCount = 0;
            for (p = ai_Chars[0]; (int)p < (int)ai_Chars[0x300]; p += 0x21c) {
                if (*(unsigned short *)p != 0xffff)
                    ai_Unk47e990(p);
            }
            ai_UpdateCharacters();
        }
        ai_Unk5ba750();
        sv_Unk52c420();
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
