/* 0x5ad000, a second "misc\Messagebox" with a percent slider (336 B): as 0x5a9f80 with the text 0x4141 and the
   buffer 0x1444a6c8 (0x521040 gates, 0x4741f0 the window, 0x509e60 the value clamped at 0x12c0 / 32 as the slider,
   the pump 0x5309a0 (the mask 0x760638): a click 0xf8a668 == 0 confirms (0x521150, 0x475980), 1 or 0x1468410 == 1
   cancels; 0x14673a0 sets 0x6f22a0) */

void d2_RadioSelect(int g, int sel);
int ai_Unk5ad000(void)
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
    ctrl = d2_GetWinObj(win, 0, io_PrintText(0x4141, *(unsigned char *)(unsigned)&ai_OwnPlayer));
    v = gm_GetGoodsValue((char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c));
    if (v > 0x12bf)
        v = 0x12c0;
    d2_SetObjValue(ctrl, 10, v / 32, 10);
    d2_CentreViewWindows(win);
    while (sv_RunFrame(ai_Unk5ad000, mask) != 0) {
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            sv_MessageBox(U_I((unsigned)&full_data_144a6c8), 4);
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
