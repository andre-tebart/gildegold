/* 0x5a9f80, the "misc\Messagebox" with a percent slider (287 B): 0x521040 (the character record 0x770b80 +
   0x6f25b8 * 0x21c, 0x140, the player 0x6f1bc5) gates the box; the window 0x4741f0 and its text 0x1a34, the
   value 0x509e60 of the record (clamped at 0x12c0) / 32 as the slider, then the frame pump 0x5309a0 (the mask
   0x760638): a click 0xf8a668 == 0 confirms (0x521150 on 0x1440a94, 0x475980), 1 or 0x1468410 == 1 cancels;
   0x14673a0 still sets 0x6f22a0; returns the click result */

int ai_Unk5a9f80(void)
{
    int mask;
    int ret;
    int win;
    int ctrl;
    int v;

    mask = U_I((unsigned)&sv_FrameMask);
    ret = 0;
    if (sv_CheckGoodAmount((char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c), 0x140, *(unsigned char *)(unsigned)&ai_OwnPlayer) == 0)
        return 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)"misc\\Messagebox");
    ctrl = d2_GetWinObj(win, 0, io_PrintText(0x1a34, *(unsigned char *)(unsigned)&ai_OwnPlayer));
    v = gm_GetGoodsValue((char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c));
    if (v > 0x12bf)
        v = 0x12c0;
    d2_SetObjValue(ctrl, 10, v / 32, 10);
    d2_CentreViewWindows(win);
    while (sv_RunFrame(ai_Unk5a9f80, mask) != 0) {
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            sv_MessageBox(U_I((unsigned)&full_data_1440a94), 4);
            U_I((unsigned)&sv_PopRequest) = 1;
            ret = d2_GetObjValue(ctrl);
        } else if (U_I((unsigned)&d2_ClickedId) == 1 || *(unsigned char *)(unsigned)&kb_Last == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
            ret = 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
    return ret;
}
