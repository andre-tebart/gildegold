/* 0x5a5530, the kidnapping ransom window (992 B, ROUGH): 0x520fe0 (param_1); the victim 0x505df0 (param_1 +
   0x65) or the text 0x14408e0; a record of the type 0x3e with the victim +0xac shows the text 0x19c9; else a
   type 0x3d record with the victim +0xac (or the text 0x14408e8); the share factor by the victim's byte
   0x1b1 (>= 4: 0.1f, 3: 0.06f, 2: 0.04f, 1: 0.02f); the relation 0x50a640 (the word) rounded, and the ransom
   min((random 0x42ec20 * 0.5 + 0.75) * 1600000.0, the relation) * the factor; the window 0x4741f0 /
   0x476f80 / 0x475600 / 0x475f70 (1) with three controls from 0x515960 (0x19c4, the word, the record +0xb4,
   2, the amount, the word, the word) / 0x475b80; the pump 0x5309a0 (this, 0x677c7): a click on the first
   control sends the AIMSG type 0x3e (+8 / +0x58 the victim, +0xc the owner, 9 at +0x3a, the player id at
   +0x60, the amount at +0x64) and shows 0x19c5, the second releases the victim through 0x4c3ba0 / 0x4c3c00
   (4, 1, param_1 - 0xa68d2c + 0x65, -1) / 0x4c49a0 / 0x4c5fa0 (the victim, -the byte, 0) and shows 0x19c6,
   the third shows 0x19c7; the exit 0x52d7a0 / 0x4758f0 */

void ai_Unk4c5fa0(int a, int b, int c);

void ai_Unk5a5530(int param_1)
{
    float factor;
    int total;
    double roll;
    int value;
    int amount;
    int win;
    int id;
    int ctl1;
    int ctl2;
    int ctl3;
    int rec;
    int out;
    char *ch;
    char msg[0xf4];
    char buf1[0x200];
    char buf2[0x200];
    char buf3[0x400];

    if (sv_CheckCharacterFlag(param_1) != 0)
        return;
    ch = (char *)gm_FindChar(U_I(param_1 + 0x65));
    if (ch == 0) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_14408e0), 0);
        return;
    }
    rec = he_FindEvent(1, 0, 0x3e);
    while (rec != 0) {
        if (U_I(rec + 0xac) == U_I(ch + 4)) {
            txt_Format(buf3, 0x19c9, U_W(ch));
            sv_MessageBox(buf3, 0);
            return;
        }
        rec = he_NextEvent();
    }
    rec = he_FindEvent(1, 0, 0x3d);
    while (rec != 0) {
        if (U_I(rec + 0xac) == U_I(ch + 4))
            break;
        rec = he_NextEvent();
    }
    if (rec == 0) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_14408e8), 0);
        return;
    }
    if (*(unsigned char *)(ch + 0x1b1) >= 4)
        factor = 0.1f;
    else if (*(unsigned char *)(ch + 0x1b1) == 3)
        factor = 0.06f;
    else if (*(unsigned char *)(ch + 0x1b1) == 2)
        factor = 0.04f;
    else if (*(unsigned char *)(ch + 0x1b1) == 1)
        factor = 0.02f;
    total = gm_GetWorth(U_W(ch));
    total = (int)(float)total;
    roll = ((double)util_RandomF() * 0.5 + 0.75) * 1600000.0;
    value = (int)(roll < (double)total
                  ? ((double)util_RandomF() * 0.5 + 0.75) * 1600000.0 : (double)total);
    amount = (int)((float)value * factor);
    win = d2_OpenForm(0, 0, "locations\diebesgilde\diebesgilde_entfuehrung");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    id = io_PrintText(0x19c4, U_W(ch), rec + 0xb4, 2, amount, U_W(ch), U_W(ch));
    ctl1 = d2_GetWinObj(win, 1, id);
    ctl2 = d2_GetWinObj(win, 1, id + 1);
    ctl3 = d2_GetWinObj(win, 1, id + 2);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl1) {
                U_I(msg + 8) = U_I(ch + 4);
                U_I(msg + 0x58) = U_I(ch + 4);
                U_I(msg + 0xc) = U_I(param_1 + 1);
                *(unsigned char *)(msg + 4) = 0x3e;
                *(unsigned char *)(msg + 0x3a) = 9;
                U_I(msg + 0x64) = amount;
                U_I(msg + 0x60) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
                cm_SendLongMessage(msg);
                txt_Format(buf1, 0x19c5, amount, U_W(ch));
                sv_MessageBox(buf1, 0);
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl2) {
                out = -1;
                cm_BeginFieldBlock(param_1, U_I(param_1 + 1));
                cm_AddFieldDelta(4, 1, (param_1 - U_I((unsigned)&cm_FieldObject)) + 0x65, &out);
                cm_SendFieldDeltas();
                ai_Unk4c5fa0(U_I(ch + 4), -(unsigned char)*(ch + 0x1b1), 0);
                txt_Format(buf2, 0x19c6, U_W(ch));
                sv_MessageBox(buf2, 0);
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&ai_Unk5fe710) == ctl3) {
                txt_Format(buf3, 0x19c7, U_W(ch));
                sv_MessageBox(buf3, 0);
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a5530, 0x677c7) == 0)
            break;
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
