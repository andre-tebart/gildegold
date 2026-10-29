/* 0x5746b0, the scene of the event 0xa1 (the meeting, ROUGH): the figure +0xc (0x505df0) and the building +0x10 (0x4fea50); the
   non-zero words +0xc8 .. +0xce are looked up (0x4a5760 (per, n, words, out)); each found record pays the figure through
   0x4c4880 (per +4, -1, 1, 0x6f1bc5, 0, word) or, from the level +0xe 3 and the sixth entry on, raises +0xac by the value
   (0x5089c0 / 0x5de518); text 0x41f2 (0x41f1 when none paid, 0x41f0 with the count, 0x5176d0), the payment (0x4c4830) and the
   message 0x5ff (0x53dda0) to per +4 */

void he_Unk5746b0(int ev)
{
    char text[0x200];
    char text2[0x100];
    char *per, *bld;
    int tab[4][4];
    int words[4], pay[4];
    int n, i, k, np;

    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per == 0)
        return;
    gm_FindById(&bld, 0, 0, HM_I(ev, 0x10));
    if (bld == 0)
        return;
    memset(tab, 0, sizeof tab);
    n = 0;
    for (k = 0; k < 4; k++) {
        unsigned short v = *(unsigned short *)(ev + 0xc8 + k * 2);
        if (v != 0)
            tab[n++][0] = v;
    }
    np = 0;
    if (n != 0) {
        ai_Unk4a5760(per, n, tab, &words[0]);
        for (i = 0; i < n; i++) {
            char *rec = (char *)tab[i][1];
            if (rec != 0) {
                if (HM_I(rec, 0xe) >= 3) {
                    gm_GoodsPrice(tab[i][0], *(unsigned char *)(unsigned)&ai_OwnPlayer);
                    HM_I(ev, 0xac) = HM_I(ev, 0xac) + _ftol();
                } else {
                    cm_SendExSellObject(HM_I(per, 4), -1, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0, tab[i][0]);
                    pay[np++] = tab[i][0];
                }
            } else if (words[0] >= 6) {
                gm_GoodsPrice(tab[i][0], *(unsigned char *)(unsigned)&ai_OwnPlayer);
                HM_I(ev, 0xac) = HM_I(ev, 0xac) + _ftol();
            } else {
                cm_SendExSellObject(HM_I(per, 4), -1, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0, tab[i][0]);
                pay[np++] = tab[i][0];
            }
        }
        if (np != 0)
            txt_Format(text, 0x41f2, HM_I(ev, 0xac));
        else
            txt_Format(text, 0x41f1, HM_I(ev, 0xac));
    } else {
        txt_Format(text, 0x41f1, HM_I(ev, 0xac));
    }
    cm_SendMoveGoodsUnchecked(HM_I(per, 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, HM_I(ev, 0xac));
    txt_Format(text2, 0x41f0, np, text);
    he_ShowNews(HM_I(per, 4), -1, text2, 0, 0x5ff, 0);
}
