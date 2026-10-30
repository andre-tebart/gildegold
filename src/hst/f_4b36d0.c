/* 0x4b36d0, the same with 0x4b35e0 (commands if cmd) */

void ai_Unk4b36d0(int cmd)
{
    char text[0x1fb8];
    char name[0x100];
    int view, win;
    int *vis;

    AI_ZERO(name, 0x100);
    AI_ZERO(text, 0x1fb8);
    if (hst_NextEventCmd(name, text, cmd) != 1)
        return;
    view = sv_Unk52fa10();
    d2_SetPointerSprite(0, 0);
    d2_SetForm(view, 1);
    win = d2_LastWindow;
    d2_SetForm(view, 2);
    io_PrintText((char *)0x10b0);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
    vis = &d2_Wins[win].visible;
    *vis = 0x18;
    d2_SetForm(view, 1);
    io_PrintText("$C%s", text);
    d2_SetForm(view, 3);
    io_PrintText("$C$Z$[%s$]", name);
    d2_ClickedId = -1;
    do {
        sv_Unk52fad0();
        if (d2_ClickedId == 0) {
            AI_ZERO(text, 0x1fb8);
            AI_ZERO(name, 0x100);
            if (hst_NextEventCmd(name, text, cmd) != 1) {
                sv_PopRequest = 1;
            } else {
                d2_SetForm(view, 2);
                io_PrintText((char *)0x10b0);
                d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
                *vis = 0x18;
                d2_SetForm(view, 1);
                io_PrintText("$C%s", text);
                d2_LastWindowPtr->scrollY = 0;
                d2_LastWindowPtr->scrollStep = 2;
                d2_SetForm(view, 3);
                io_PrintText("$C$Z$[%s$]", name);
            }
        }
    } while (sv_RunFrame((void (*)(void))ai_Unk4b36d0, 0x66087));
    sv_Unk52fa90();
}
