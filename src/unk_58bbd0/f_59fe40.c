/* 0x59fe40, the citizens' right window "Locations\Rathaus\Buergerrecht" (992 B, ROUGH: the message loop is
   condensed): the player's money 0x509e60 (0x770b80 + player * 0x21c) and the room 0x4ffe90 (param_1 + 0x5d,
   2, 6, 0, 0x115); below 16000 the text 0x1a00 goes to 0x521150 and 0x52d7a0 returns; else the window, the text
   0x19fe (the city, 16000); on confirm 0x521040 (the player, 16000, the player byte): the payment 0x4c4830
   (room + 2, the character, 0, 16000), 0x4c4a60 (the character, 0x1c8, 4, 0, 0x4000), the citizens' right
   message 0x69 to every other kind 6 / 7 character of the town, the text 0x19ff via 0x521150 and the event
   0x576040 (0x2e, 0, 0 / 1); the pump 0x5309a0 (0x59fe40, 0x677c7) */

void ai_Unk59fe40(int param_1)
{
    char msg[0x200];
    char text[0x100];
    char date[0x100];
    char buf[0x100];
    int money;
    int room;
    int win;
    int rec;
    int n;
    int i;
    int done;

    money = gm_GetGoodsValue((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    room = gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0x115);
    if (money < 16000) {
        txt_Format(text, 0x1a00);
        sv_MessageBox(text, 0);
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, "Locations\\Rathaus\\Buergerrecht");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x19fe, *(unsigned short *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c), 16000,
                 (char *)((unsigned)&ai_PlayerSlots + 0x20) + U_I((unsigned)&ai_OwnPlayer) * 0x2f4);
    U_I((unsigned)&d2_ClickedId) = -1;
    done = 0;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_ClickedId) == 0 &&
                sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, 16000, U_I((unsigned)&ai_OwnPlayer)) != 0) {
                /* ROUGH: the citizenship message 0x69 to the other kind 6 / 7 characters */
                cm_SendMoveGoodsUnchecked(U_I(room + 2), U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), 0, 16000);
                cm_SendSetField(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87), 0x1c8, 4, 0, 0x4000);
                txt_Format(date, 0x19ff);
                ai_Unk576040(0x2e, 0, 0, 0);
                done = 1;
                sv_MessageBox(date, 0);
            }
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk59fe40, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    sv_CameraFollow();
    if (done)
        ai_Unk576040(0x2e, 0, 1, 0);
}
