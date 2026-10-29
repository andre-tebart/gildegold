/* 0x5ae240, the transport purchase window "Handel\Handel_Transport_neuer_Kauf" (791 B): 0x509060 counts the
   selected characters (more than 2: the text 0xf1 and out); the window 0x4741f0, the three controls 0xd8 of the
   list 0x46c490 (3, ...), the page 0x52e6d0 (0x70b, 0x741); a click on the 0xdb control takes the quantity
   0x5087f0 (0x134 + the entry, 100) and asks 0x521040 (the character, the quantity, the player): the text 0xd9 in
   the order of 0x5190a0 and, confirmed (0x521150, 0x101), the payment 0x4c4830 / 0x506430 (+0x50) / the order
   0x4c4880 (the +1 field of param_1, the entry + 0x134, the player); the 0x5e2 .. 0x5e4 keys pick the entry; the
   caption 0xda of the control 0x473540 with the quantity; the pump 0x5309a0 (0x677c7) */


void d2_CenterObj(); int gm_Unk5034b0(); int gm_Unk50a970();

void ai_Unk5ae240(int param_1)
{
    int ids[3];
    char text[512];
    char text2[1024];
    int win;
    int list;
    int ctrl;
    int qty;
    int v;
    int i;
    int k;
    int fam;

    i = 0;
    if (gm_Unk509060(param_1, 0, 0) > 2) {
        txt_Format(text, 0xf1, 3);
        sv_MessageBox(text, 0);
        return;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62cc6c);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    io_PrintText(0xd8);
    ids[0] = d2_GetWinObj(win, 1, 0);
    ids[1] = d2_GetWinObj(win, 1, 1);
    ids[2] = d2_GetWinObj(win, 1, 2);
    d2_SetForm(win, 0);
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x70b, 0x741);
    d2_SetForm(win, 3);
    ctrl = d2_GetWinObj(win, 3, io_PrintText(0xdb));
    list = d2_CreateRadioGroup(3, ids[0], ids[1], ids[2]);
    d2_RadioSelect(list, 0);
    while (1) {
        if (U_I((unsigned)&d2_ClickedId) != 0 && (U_I((unsigned)&d2_ClickedId) < 0x5e2 || U_I((unsigned)&d2_ClickedId) > 0x5e4)) {
            if (U_I((unsigned)&d2_ClickedId) == 0 && U_I((unsigned)&ai_Unk5fe710) == ctrl) {
                qty = (int)gm_ItemPrice(i + 0x134, 100);
                if (sv_CheckGoodAmount((char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c), qty,
                                 *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0) {
                    v = ls_GetLanguage();
                    if (v == 0)
                        txt_Format(text2, 0xd9, qty, i * 2 + 0xc10);
                    else if (v == 1)
                        txt_Format(text2, 0xd9, i * 2 + 0xc10, qty);
                    if (sv_MessageBox(text2, 0x101) != 0) {
                        cm_SendMoveGoodsUnchecked(-1, U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c),
                                     *(unsigned char *)(unsigned)&ai_OwnPlayer, qty);
                        fam = gm_PlayerRecord((char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c));
                        if (fam != 0)
                            *(int *)(fam + 0x50) += qty;
                        cm_SendExSellObject(U_I(param_1 + 1), -1, i + 0x134, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                }
            }
        } else if (U_I((unsigned)&d2_ClickedId) >= 0x5e2 && U_I((unsigned)&d2_ClickedId) <= 0x5e4) {
            for (k = 0; k < 3; k++)
                if (U_I((unsigned)&ai_Unk5fe710) == ids[k])
                    i = k;
        }
        if (U_I((unsigned)&d2_RadioSel + list * 0x8c) != -1) {
            txt_Format(text, 0xda, i * 2 + 0xc10, (int)gm_ItemPrice(i + 0x134, 100));
            d2_SetCaption(ctrl, text);
            d2_CenterObj(ctrl);
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk5ae240, 0x677c7) == 0) {
            d2_CloseView(win);
            d2_RadioClear(list);
            return;
        }
    }
}
