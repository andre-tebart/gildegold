/* 0x5a23a0, the baptism window "Locations_Kirche_Taufe" (1360 B, ROUGH): the player's slot record
   (0x770b80 + slot * 0x21c) and the fee (int)(0x50a640 (the slot) * 0.01f); param_1 the building record
   (+0x27 the owner slot; 0xffff closes it). The player's own building: the fee * 0xf, the window 0x4741f0 /
   0x476f80 / 0x475600 / 0x475f70 / 0x52ea30 (the screen 0x6ed938), the text 0x1976 (the record word, the fee,
   0x197b - religion + 2 past the year 0x770ae0 >= 0x75), 0x1977 and the two buttons 0x475b80 (2); a click
   (0x5fe710) with 0x521040 (the record, the fee, the city) confirms: 0x4c4830 (-1, the id, 0, the fee),
   0x4c3ba0 / 0x4c3c00 (1, 1, 0xc, &1 - religion) / 0x4c49a0 for the record and its items +3 .. +7 (0x505df0),
   the text 0x1978 and the message 0x53dda0 (every other record of the kinds 6 / 7 at +2, 0x5fe), then the text
   0x1979 and 0x521150 (buf, 0). Another owner of another religion: the text 0x1971 (0x197a + the owner's
   religion), 0x1972 and its buttons; the click pays 0x4c4830 (the owner id, the id, 0, the fee) and sends 0x1974
   / 0x53dda0 (0x5ff) for the kinds 6 / 7, then 0x1973 and 0x521150. The same religion: only the text 0x1970.
   The pump 0x5309a0 (this, 0x677c7); the exit 0x4758f0 / 0x52d7a0. ROUGH: the button slots and the frame
   overlaps are from the disassembly, not checked */

void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int cm_AddFieldDelta(int size, int n, short field, void *v); short * gm_FindChar(int id); int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);

void ai_Unk5a23a0(int param_1, int param_2)
{
    int slot = U_W((unsigned)&ai_PlayerId);
    char *per = (char *)(unsigned)&ai_Chars + slot * 0x21c;
    char *own;
    int price;
    int win;
    int btn1;
    int btn2;
    int year;
    int id;
    int r;
    int i;
    int v;
    unsigned short owner;
    char buf[0x200];

    price = (int)(gm_GetWorth(slot) * 0.01f);
    if (param_1 == 0)
        return;
    owner = U_W(param_1 + 0x27);
    if (owner == 0xffff)
        return;
    if (owner == slot) {
        price *= 0xf;
        win = d2_OpenForm(0, 0, "Locations_Kirche_Taufe");
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        sv_FitForm(U_I((unsigned)&d2_LastWindow));
        d2_SetForm(win, 1);
        year = U_I((unsigned)&ai_Time) >= 0x75 ? 2 : 0;
        io_PrintText(0x1976, U_W(per), price,
                     0x197b - *(unsigned char *)(per + 0xc) + year);
        d2_SetForm(win, 2);
        v = io_PrintText(0x1977);
        btn1 = d2_GetWinObj(win, 2, v);
        btn2 = d2_GetWinObj(win, 2, v + 1);
        U_I((unsigned)&d2_ClickedId) = -1;
        for (;;) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == btn1) {
                    if (sv_CheckGoodAmount(per, price, U_I((unsigned)&ai_OwnPlayer)) != 0) {
                        v = 1 - *(unsigned char *)(per + 0xc);
                        cm_SendMoveGoodsUnchecked(-1, U_I(per + 4), 0, price);
                        cm_BeginFieldBlock(per, U_I(per + 4));
                        cm_AddFieldDelta(1, 1, 0xc, &v);
                        cm_SendFieldDeltas();
                        for (i = 3; i < 8; i++) {
                            id = U_I(((unsigned)&ai_Unk770b89 + 0x53) + slot * 0x21c + i * 4);
                            if (id != -1 && (r = gm_FindChar(id)) != 0) {
                                cm_BeginFieldBlock((char *)r, U_I(r + 4));
                                cm_AddFieldDelta(1, 1, 0xc, &v);
                                cm_SendFieldDeltas();
                            }
                        }
                        txt_Format(buf, 0x1978, U_W(per), param_1,
                                     0x197a + v + (U_I((unsigned)&ai_Time) >= 0x75 ? 2 : 0));
                        for (i = 0; i < 0x300; i++) {
                            id = U_I((unsigned)&ai_CharInts + i * 0x21c);
                            if (id != U_I(per + 4) &&
                                (*(unsigned char *)(((unsigned)&ai_Chars + 0x2) + i * 0x21c) == 6 ||
                                 *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + i * 0x21c) == 7))
                                he_ShowNews(id, -1, buf, 0, 0x5fe, 0);
                        }
                        txt_Format(buf, 0x1979,
                                     0x197a + v + (U_I((unsigned)&ai_Time) >= 0x75 ? 2 : 0));
                        sv_MessageBox(buf, 0);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                } else if (U_I((unsigned)&ai_Unk5fe710) == btn2) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
            if (sv_RunFrame(ai_Unk5a23a0, 0x677c7) == 0)
                break;
        }
        d2_CloseView(win);
        sv_CameraFollow();
        return;
    }
    own = (char *)(unsigned)&ai_Chars + owner * 0x21c;
    if (*(unsigned char *)(own + 0xc) != *(unsigned char *)(per + 0xc)) {
        win = d2_OpenForm(0, 0, "Locations_Kirche_Taufe");
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        sv_FitForm(U_I((unsigned)&d2_LastWindow));
        d2_SetForm(win, 1);
        year = U_I((unsigned)&ai_Time) >= 0x75 ? 2 : 0;
        io_PrintText(0x1971, U_W(per), price,
                     0x197a + *(unsigned char *)(own + 0xc) + year);
        d2_SetForm(win, 2);
        v = io_PrintText(0x1972);
        btn1 = d2_GetWinObj(win, 2, v);
        btn2 = d2_GetWinObj(win, 2, v + 1);
        U_I((unsigned)&d2_ClickedId) = -1;
        for (;;) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == btn1) {
                    if (sv_CheckGoodAmount(per, price, U_I((unsigned)&ai_OwnPlayer)) != 0) {
                        v = *(unsigned char *)(own + 0xc);
                        cm_SendMoveGoodsUnchecked(U_I(own + 4), U_I(per + 4), 0, price);
                        cm_BeginFieldBlock(per, U_I(per + 4));
                        cm_AddFieldDelta(1, 1, 0xc, &v);
                        cm_SendFieldDeltas();
                        if (*(unsigned char *)(own + 2) == 6 ||
                            *(unsigned char *)(own + 2) == 7) {
                            txt_Format(buf, 0x1974, U_W(per), price);
                            he_ShowNews(U_I(own + 4), -1, buf, 0, 0x5ff, 0);
                        }
                        txt_Format(buf, 0x1973,
                                     0x197a + *(unsigned char *)(own + 0xc) + year);
                        sv_MessageBox(buf, 0);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                } else if (U_I((unsigned)&ai_Unk5fe710) == btn2) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
            if (sv_RunFrame(ai_Unk5a23a0, 0x677c7) == 0)
                break;
        }
        d2_CloseView(win);
        sv_CameraFollow();
        return;
    }
    txt_Format(buf, 0x1970);
    sv_MessageBox(buf, 0);
}
