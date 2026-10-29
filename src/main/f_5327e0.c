/* 0x5327e0, the player count dialog ("Menu\\CHOOSE_PLAYERCOUNT"): a count selector (default 2, 0x62489c), the heading text 0x1ab5, "%s$A%s" texts 0x7d / 0x7e; runs 0x5309a0 until a button is pressed (button 0 = ok:
   stores the count and returns 1); returns 1 for ok, 0 for cancel */

int main_Unk5327e0(void)
{
    int r, f, sel, run;
    unsigned char cnt;

    r = 0;
    f = d2_OpenForm(0, 0, "Menu\\CHOOSE_PLAYERCOUNT");
    d2_CentreViewWindows(f);
    sel = d2_GetWinObj(f, 0, 0);
    cnt = *(unsigned char *)(unsigned)&ai_NumPlayers;
    if (cnt <= 1)
        cnt = 2;
    *(unsigned char *)(unsigned)&ai_NumPlayers = cnt;
    d2_SetObjValue(sel, 2, 8, cnt);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(f, 1);
    io_PrintText(0x1ab5);
    d2_SetForm(f, 2);
    io_PrintText("%s$A%s", 0x7d, 0x7e);
    run = sv_RunFrame((int)main_Unk5327e0, 0x24087);
    while (run != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) == 0) {
            *(unsigned char *)(unsigned)&ai_NumPlayers = d2_GetObjValue(sel);
            d2_SetViewEnabled(f, 0);
            r = 1;
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        run = sv_RunFrame((int)main_Unk5327e0, 0x24087);
    }
    if (f != -1)
        d2_CloseView(f);
    return r;
}
