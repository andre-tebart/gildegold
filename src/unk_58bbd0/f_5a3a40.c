/* 0x5a3a40, the dungeon plan window "locations\kerker\kerkerplan" (219 B): the room 0x4ffe90 (param_1 +
   0x5d, 1, 0, 0xba); the window 0x4741f0 / 0x475600 / 0x475f70 (1) with the text 0x19ad, then every character
   record (0x770b80 + i * 0x21c .. 0x7d5f80) with 0x50ccd0 set gets the line 0x19ae (the count, the record word)
   and, with no line, 0x19af; the pump 0x5309a0 (this, 0x677c7), the exit 0x52d7a0 / 0x4758f0 */

int gm_Unk50ccd0();

void ai_DungeonPlanWindow(int param_1)
{
    int room;
    int win;
    int n = 0;
    char *p;

    room = gm_Query(U_I(param_1 + 0x5d), 1, 0, 0xba);
    if (room == 0)
        return;
    win = d2_OpenForm(0, 0, "locations\\kerker\\kerkerplan");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x19ad);
    d2_SetForm(win, 1);
    for (p = (char *)(unsigned)&ai_Chars; (int)p < (unsigned)((unsigned)ai_CharsV + 0x65400); p += 0x21c) {
        if (gm_Unk50ccd0(p) != 0)
            io_PrintText(0x19ae, ++n, U_W(p));
    }
    if (n == 0)
        io_PrintText(0x19af);
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_DungeonPlanWindow, 0x677c7) == 0)
            break;
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
