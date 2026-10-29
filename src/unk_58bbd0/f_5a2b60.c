/* 0x5a2b60, the confession window "Locations\Kirche\Beichte" (625 B): closed for the unknown owner (0xffff);
   the player's fame 0x4f39b0 (the id); the window 0x4741f0 / 0x475600 / 0x475f70 / 0x52ea30 (the screen
   0x6ed938) / 0x476f80 with the text 0x197f and, without fame, 0x1980; else the text 0x1981 (the fame) and the
   slider 0x475b80 / 0x475cb0 (2, 6, 2), the texts 0x1982 / 0x1983 and their buttons; the pump 0x5309a0 (this,
   0x677c7): the slider pays 0x520f90 (the player, the amount): 0x4c5f60 (the id, -amount), for another owner
   0x4c5f60 (the owner id, 1) and 0x5a8c00 (the player, the owner, the building), 0x4f3a30 (the id, amount / 2),
   the text 0x14407d0 and 0x521150; the cancel button only sets 0x6f22a0; the exit 0x52d7a0 / 0x4758f0 */

int sv_CheckMoney(); void cm_SendMoneyDelta(int a, int b); void ai_Unk5a8c00(); void d2_SetObjValue();

void ai_Unk5a2b60(int param_1)
{
    int win;
    int fame;
    int amount;
    int ctl = -1;
    int ok = -1;
    int cancel = -1;
    int active = 0;
    unsigned short owner;

    if (U_W(param_1 + 0x27) == 0xffff)
        return;
    fame = unk_Unk4f39b0(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c));
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Beichte");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x197f);
    if (fame == 0) {
        io_PrintText(0x1980);
        U_I((unsigned)&d2_ClickedId) = -1;
    } else {
        ctl = d2_GetWinObj(win, 1, io_PrintText(0x1981, fame));
        d2_SetObjValue(ctl, 2, 6, 2);
        ok = d2_GetWinObj(win, 1, io_PrintText(0x1982));
        cancel = d2_GetWinObj(win, 1, io_PrintText(0x1983));
        active = 1;
        U_I((unsigned)&d2_ClickedId) = -1;
    }
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1 && active) {
            if (U_I((unsigned)&ai_Unk5fe710) == ok) {
                amount = d2_GetObjValue(ctl);
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, amount) != 0) {
                    cm_SendMoneyDelta(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c), -amount);
                    owner = U_W(param_1 + 0x27);
                    if (owner != 0xffff && owner != U_W((unsigned)&ai_PlayerId)) {
                        cm_SendMoneyDelta(U_I((unsigned)&ai_CharInts + owner * 0x21c), 1);
                        ai_Unk5a8c00((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c,
                                     (char *)(unsigned)&ai_Chars + U_W(param_1 + 0x27) * 0x21c,
                                     (char *)param_1);
                    }
                    unk_Unk4f3a30(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c), amount / 2);
                    sv_MessageBox((char *)U_I((unsigned)&full_data_14407d0), 0);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == cancel) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a2b60, 0x677c7) == 0)
            break;
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
