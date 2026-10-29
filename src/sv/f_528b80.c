/* 0x528b80, opens the info panel for the item p (not the ids 0x92 .. 0x97 and 0x2db): the title text with the item's name (0x9a8 + 2 * id; the selected building's name and owner before it), the image (0xdf + id), and for items of the class 2 / 6 the owner's buttons
   (0x528800: 0x8c at 0x623894 when the item may be changed (0x508cc0), 0xa6 at 0x623888) */

void sv_Unk528b80(unsigned short *p)
{
    unsigned char *b, *sel;
    short *owner;
    int w;
    unsigned fl;
    char c;

    if (sv_PanelForm == -1 && (*p < 0x92 || 0x97 < *p) && *p != 0x2db) {
        sv_PanelForm = d2_OpenForm(d2_ScreenW - 0x68, 0x106, "panel\\infopanel_gebaeude");
        d2_SetForm(sv_PanelForm, 1);
        sel = (unsigned char *)ai_PlayerBuilding;
        b = sel;
        if (sel == 0) {
            b = (unsigned char *)ai_FallbackBuilding;
            sel = 0;
        }
        if (b == 0)
            io_PrintText("$Z%s", *p * 2 + 0x9a8);
        else if (b[5] == 0)
            io_PrintText("$Z%s$A%s", *b * 0xe + 0x48e, *p * 2 + 0x9a8);
        else
            io_PrintText("$Z%s$A>%s<$A%s", *b * 0xe + 0x48e, b + 5, *p * 2 + 0x9a8);
        d2_SetForm(sv_PanelForm, 0);
        w = d2_WinOpenPic(((*(short *)(ai_Unkf8f128 + d2_LastWindow * 0x3c4) - 0x30) >> 1) - 1, 0x48, *p + 0xdf, d2_LastWindow);
        *(int *)(d2_Objs + 0x4c + w * 0x2ec) = 1;
        if (b != 0 && ((c = ai_Goods[*p * 0x41]) == 2 || c == 6)) {
            owner = (short *)gm_Unk50a050(p);
            fl = unk_Unk503530(ai_PlayerId, b, 0, 0);
            if ((*owner == (short)ai_PlayerId || (fl & 0x10) != 0) && gm_Unk508cc0(b, p, 0) != 0)
                ai_Unk623894 = sv_Unk528800(0, 0x8c, ai_Unk1441f78);
            ai_Unk623888 = sv_Unk528800(0, 0xa6, ai_Unk1441f7c);
        }
    }
}
