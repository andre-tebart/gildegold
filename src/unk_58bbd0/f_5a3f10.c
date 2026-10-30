/* 0x5a3f10, the burglary window "locations\diebesgilde\diebesgilde_einbruch" (542 B, ROUGH): the owner's
   record (0x770b80 + owner * 0x21c); the gate 0x553cc0 (param_1) with the text 0x1a92 (the owner slot) and
   0x521150, the gate 0x509120 (0x6f2308) with the text 0x1440954; the window 0x4741f0 / 0x475600 / 0x476f80 /
   0x475f70 (1) with the text 0x19e4 (the record word, param_1) and the two buttons 0x475b80 (the text and +1);
   the pump 0x5309a0 (this, 0x677c7): the first button builds the AIMSG (type 0x3f at +4, the player id at +8,
   1 at +0x36, param_1 + 1 at +0x58, the [0x6f2308] record + 1 at +0x5c and, at +0x38, the ids of up to 8
   records with the byte +0x184 set of the 0x300), sends it 0x4c4b70 / 0x4c2d20 or shows the text 0x144094c with
   none, then 0x52ae40; the second button only sets 0x6f22a0; the exit 0x4758f0; returns 1 after a send. ROUGH:
   the frame slots of the message and the two gates are from the disassembly */

int he_Unk553cc0(); int gm_Unk509120(); void snd_PlayFavourComment(void); 
void sv_OpenPlayerBar(int a, char k); int ai_Unk5b2180(); void sv_ClosePlayerBar(void);

int ai_Unk5a3f10(int param_1)
{
    int win;
    int ctl1;
    int ctl2;
    int id;
    int done = 0;
    int n;
    int i;
    char *per;
    char buf[0x100];
    char msg[0x80];

    if (param_1 == 0)
        return 0;
    per = (char *)(unsigned)&ai_Chars + U_W(param_1 + 0x27) * 0x21c;
    if (he_Unk553cc0(param_1) == 0) {
        txt_Format(buf, 0x1a92, U_W(param_1 + 0x27));
        sv_MessageBox(buf, 0);
        return 0;
    }
    if (gm_Unk509120(U_I((unsigned)&ai_PlayerBuilding)) == 0) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_1440954), 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_einbruch");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    id = io_PrintText(0x19e4, U_W(per), param_1);
    ctl1 = d2_GetWinObj(win, 1, id);
    ctl2 = d2_GetWinObj(win, 1, id + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == ctl1) {
            *(unsigned char *)(msg + 4) = 0x3f;
            U_I(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
            *(unsigned char *)(msg + 0x36) = 1;
            U_I(msg + 0x58) = U_I(param_1 + 1);
            U_I(msg + 0x5c) = U_I(U_I((unsigned)&ai_PlayerBuilding) + 1);
            for (i = 0; i < 8; i++)
                U_I(msg + 0x38 + i * 4) = -1;
            n = 0;
            for (i = 0; i < 0x300; i++) {
                if (*(unsigned char *)((unsigned)&ai_CharInts + i * 0x21c + 0x184) != 0) {
                    U_I(msg + 0x38 + n * 4) = U_I((unsigned)&ai_CharInts + i * 0x21c);
                    n++;
                }
            }
            if (n != 0) {
                cm_SendLongMessage(msg);
                snd_PlayFavourComment();
            } else {
                sv_MessageBox((char *)U_I((unsigned)&full_data_144094c), 0);
            }
            sv_ClearSelection();
            U_I((unsigned)&sv_PopRequest) = 1;
            done = 1;
        } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk5a3f10, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    return done;
}
