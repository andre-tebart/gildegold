/* 0x5a41d0, the espionage window "locations\diebesgilde\diebesgilde_spionage" (777 B, ROUGH): the owner
   guard (0xffff closes it) and the gate 0x553cc0 (param_1) with the text 0x1a92 (the owner slot, param_1);
   the guild record 0x6f2308 or 0x6f230c and the building kind record 0x7e3f30 + its byte * 0x24d; the walk of
   the guild's rooms 0x4ffe90 (rec + 0x5d, 1, 0, 0xca) / 0x4ffcc0 collects those whose +0x2a is not param_1 + 1
   (up to 0x40), then the carriers 0x53e400 (2, 2, the player slot, 0, 0x40) / 0x53e300: every carrier whose
   +0xac matches no collected +0x2a counts; with the total < 2 * the kind byte +0x247 the text 0x19fa (the
   total, 0x6f2308) and out; else the window 0x4741f0 / 0x475600 / 0x476f80 / 0x475f70 (1) with the text 0x19f7
   (the record word, param_1) and the two buttons 0x475b80 (the text and +1); the pump 0x5309a0 (this, 0x677c7):
   the first button builds the AIMSG (type 0x40 at +4, the player id at +8, 1 at +0x36, param_1 + 1 at +0x58,
   0x6f2308 + 1 at +0x5c and at +0x38 up to 8 ids with the byte +0x184) and sends it 0x4c4b70 / 0x4c2d20, or
   shows the text 0x1440b38 with no crew; the exit 0x4758f0 and 0x52ae40; returns 1 after a send */

char * he_NextEvent(void);

int ai_Unk5a41d0(int param_1)
{
    int list[0x40];
    int n = 0;
    int cnt = 0;
    int win;
    int ctl1;
    int ctl2;
    int done = 0;
    int i;
    int total;
    char *rec;
    char *kind;
    char *p;
    char buf[0x100];
    char msg[0x80];

    if (param_1 == 0)
        return 0;
    if (U_W(param_1 + 0x27) == 0xffff)
        return 0;
    if (he_Unk553cc0(param_1) == 0) {
        txt_Format(buf, 0x1a92, U_W(param_1 + 0x27), param_1);
        sv_MessageBox(buf, 0);
        return 0;
    }
    rec = (char *)U_I((unsigned)&ai_PlayerBuilding);
    if (rec == 0) {
        rec = (char *)U_I((unsigned)&ai_FallbackBuilding);
    }
    kind = (char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)U_I((unsigned)&ai_PlayerBuilding) * 0x24d);
    if (rec != 0) {
        for (p = (char *)gm_Query(U_I(rec + 0x5d), 1, 0, 0xca); p != 0;
             p = (char *)gm_QueryNext()) {
            if (U_I(p + 0x2a) != U_I(param_1 + 1))
                list[n++] = (int)p;
        }
    }
    for (p = (char *)he_FindEvent(2, 2, U_W((unsigned)&ai_PlayerId), 0, 0x40); p != 0;
         p = (char *)he_NextEvent()) {
        for (i = 0; i < n; i++) {
            if (U_I(p + 0xac) == U_I(list[i] + 0x2a))
                break;
        }
        if (i >= n)
            cnt++;
    }
    total = n + cnt;
    if (total < 2 * *(unsigned char *)(kind + 0x247)) {
        txt_Format(buf, 0x19fa, total, U_I((unsigned)&ai_PlayerBuilding));
        sv_MessageBox(buf, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_spionage");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    i = io_PrintText(0x19f7, U_W((char *)(unsigned)&ai_Chars + U_W(param_1 + 0x27) * 0x21c), param_1);
    ctl1 = d2_GetWinObj(win, 1, i);
    ctl2 = d2_GetWinObj(win, 1, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == ctl1) {
            *(unsigned char *)(msg + 4) = 0x40;
            U_I(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
            *(unsigned char *)(msg + 0x36) = 1;
            U_I(msg + 0x58) = U_I(param_1 + 1);
            U_I(msg + 0x5c) = U_I(U_I((unsigned)&ai_PlayerBuilding) + 1);
            for (i = 0; i < 8; i++)
                U_I(msg + 0x38 + i * 4) = -1;
            i = 0;
            for (p = (char *)(unsigned)&ai_CharInts; (int)p < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2) && i < 8; p += 0x21c) {
                if (*(unsigned char *)(p + 0x184) != 0) {
                    U_I(msg + 0x38 + i * 4) = U_I(p);
                    i++;
                }
            }
            if (i != 0) {
                cm_SendLongMessage(msg);
                done = 1;
                snd_PlayFavourComment();
                U_I((unsigned)&sv_PopRequest) = 1;
            } else {
                sv_MessageBox((char *)U_I((unsigned)&full_data_1440b38), 0);
                U_I((unsigned)&sv_PopRequest) = 1;
                done = 1;
            }
        } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk5a41d0, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    sv_Unk52ae40();
    return done;
}
