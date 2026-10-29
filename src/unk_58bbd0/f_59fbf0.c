/* 0x59fbf0, the guild cash window "locations\zunfthaus\gildenkasse" (176 B): the room 0x4ffe90 (param_1 +
   0x5d, 2, 6, 0, 0x115), its money 0x509e30 (room + 0x14, the player byte); the window, the text 0x1a0f
   (the money); the pump 0x5309a0 (0x59fbf0, 0x677c7), 0x52d7a0 and 0x4758f0 */

void ai_Unk59fbf0(int param_1)
{
    int room;
    int money;
    int win;

    room = gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0x115);
    money = ((int (*)(int, unsigned char))gm_Unk509e30)(U_I(room + 0x14), *(unsigned char *)(unsigned)&ai_OwnPlayer);
    win = d2_OpenForm(0, 0, "locations\\zunfthaus\\gildenkasse");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1a0f, money);
    while (sv_RunFrame(ai_Unk59fbf0, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
