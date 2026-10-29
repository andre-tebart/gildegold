/* 0x5a3b90, the bribe window "locations\kerker\kerker_besteche" (599 B): the record 0x4e6ec0 (0x11, {?, the
   id}) and its character 0x505df0; without one the text 0x1440894 and out; with the object 0x53e400 (2, 0, 0x39,
   2, the player slot) the text 0x19b6 (the character word) and out; else the window 0x4741f0 / 0x475600 /
   0x475f70 (1) with the text 0x19b1 (the city twice, the character word), the slider 0x475b80 / 0x475cb0 (0x96,
   0x5085b0 (0x509f20 (the slot), the city, 0xfa)) and 0x476f80; a click on the second control builds the AIMSG
   (type 0x39 at +4, the character id at +8, -1 at +0xc, 9 at +0x36, the player id at +0x58, the character id
   at +0x5c, the slider amount 0x475980 / 0x5085f0 at +0x60), 0x4c4540 ("kerkermeister_bestechen"), pays
   0x4c47e0 (the character id, the player id, the city, the amount), sends 0x4c4b70 and 0x4c4580; the pump
   0x5309a0 (this, 0x677c7), the exit 0x4758f0 / 0x52d7a0 */

int amt_GetFirstOffice(unsigned int k, struct AMTREC *out); int gm_Unk509f20(); void cm_BeginGroup(const char *s); void cm_EndGroup(const char *s);

void ai_Unk5a3b90(int param_1)
{
    int rec[2];
    int win;
    int ctl;
    int btn;
    int amount;
    int id;
    char *priest;
    char msg[0x108];
    char buf[0x800];

    if (amt_GetFirstOffice(0x11, rec) == 0)
        return;
    priest = (char *)gm_FindChar(rec[1]);
    if (priest == 0) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_1440894), 0);
        sv_CameraFollow();
        return;
    }
    if (he_FindEvent(2, 0, 0x39, 2, U_W((unsigned)&ai_PlayerId)) != 0) {
        txt_Format(buf, 0x19b6, U_W(priest));
        sv_MessageBox(buf, 0);
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, "locations\\kerker\\kerker_besteche");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    id = io_PrintText(0x19b1, U_I((unsigned)&ai_OwnPlayer), U_I((unsigned)&ai_OwnPlayer), U_W(priest));
    ctl = d2_GetWinObj(win, 1, id);
    btn = d2_GetWinObj(win, 1, id + 1);
    amount = ((int (*)(int, int, int))gm_ConvertMoney)(gm_Unk509f20(U_W((unsigned)&ai_PlayerId)), U_I((unsigned)&ai_OwnPlayer), 0xfa);
    d2_SetObjValue(ctl, 0x96, amount);
    d2_SetPointerSprite(0, 0);
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == btn) {
            U_I(msg + 4) = 0x39;
            U_I(msg + 8) = U_I(priest + 4);
            U_I(msg + 0xc) = -1;
            *(unsigned char *)(msg + 0x36) = 9;
            U_I(msg + 0x58) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
            U_I(msg + 0x5c) = U_I(priest + 4);
            amount = ((int (*)(int))gm_PlayerFactor)(d2_GetObjValue(ctl, U_I((unsigned)&ai_OwnPlayer)));
            U_I(msg + 0x60) = amount;
            cm_BeginGroup("kerkermeister_bestechen");
            cm_SendMoveGoods(U_I(priest + 4), U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c),
                         U_I((unsigned)&ai_OwnPlayer), amount);
            cm_SendLongMessage(msg);
            cm_EndGroup("kerkermeister_bestechen");
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk5a3b90, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    sv_CameraFollow();
}
