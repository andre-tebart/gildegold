/* 0x5a45c0, the spy ring window "locations\diebesgilde\diebesgilde_spionagering" (1540 B, ROUGH SKELETON):
   the window 0x4741f0 / 0x475600 / 0x475f70 (1) with the text 0x19fb, 0x476f80 / 0x476b70, the text 0x1ba6 and
   the button 0x475b80 (3), the list 0x475ff0 (2, 0x767) and 0x472270 (-1, -1, the screen 0x6ed938); on a
   rebuild the guild's rooms 0x4ffe90 (param_1 + 0x5d, 1, 0, 0xca) / 0x4ffcc0 collect up to 8 whose building
   0x501190 (1, 1, room + 0x2a) is known, each row 0x515960 (0x19fc, the kind byte + 0x427, the owner slot, the
   building, the room + 0x1c, 0x5091a0 (the building), the room + 0x37 / + 0x36) with the control 0x475b80 and
   the window table 0xf8f080 (+0x2e8 the building, +0x28 / +0x2c = 1), the list 0x46c490 (1) / 0x46c560 /
   0x46c600; a click on the button picks the list index 0x46c6a0 and pays 0x4c5010 (param_1 + 1, -1, the room
   + 2) with 0x4c3ae0 / 0x52fe00; the pump 0x5309a0 (this, 0x677c7), the exit 0x52d7a0 / 0x4758f0. ROUGH
   SKELETON: the list bookkeeping (0x2ec-byte window records, the 0x23-dword table 0xfa5aa8) is condensed */

unsigned char * gm_BuildQuery(int n, ...); int gm_Unk5091a0(); int d2_CreateRadioGroup(int n, ...); void d2_RadioAdd(int g, int obj);
void d2_RadioClear(int g); int d2_RadioGetSel(); void ai_Unk4c5010(int a, int b, int c); void ai_Unk4c47b0(short a);
int d2_GetViewWin(); void d2_CreateScrollButtons(); void d2_ClearInput(void); void sv_PumpNet();

void ai_Unk5a45c0(int param_1)
{
    int rows[8];
    int ctl[16];
    int win;
    int list;
    int h;
    int i;
    int n;
    int j;
    int rebuild = 1;
    int idx = -1;
    int room;
    int bld;
    int v;
    char *p;
    char buf[0x200];

    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_spionagering");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x19fb);
    d2_SetPointerSprite(0, 0);
    d2_ClearInput();
    d2_SetForm(win, 3);
    h = d2_GetWinObj(win, 3, io_PrintText(0x1ba6));
    v = d2_GetViewWin(win, 2, 0x767);
    d2_CreateScrollButtons(-1, -1, U_I((unsigned)&d2_LastWindow), v);
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (rebuild) {
            for (i = 0; i < 8; i++)
                rows[i] = 0;
            if (idx != -1) {
                d2_RadioClear(idx);
                idx = -1;
            }
            d2_SetForm(win, 2);
            io_PrintText((char *)(unsigned)"$C");
            n = 0;
            room = gm_Query(U_I(param_1 + 0x5d), 1, 0, 0xca);
            while (room != 0 && n < 8) {
                bld = gm_BuildQuery(1, 1, U_I(room + 0x2a));
                if (bld != 0 && U_W(bld + 0x27) != 0xffff) {
                    rows[n] = room;
                    n++;
                }
                room = gm_QueryNext();
            }
            for (j = 0; j < 8; j++) {
                if (rows[j] != 0) {
                    p = (char *)gm_BuildQuery(1, 1, U_I(rows[j] + 0x2a));
                    if (p == 0) {
                        ai_Unk4c47b0(U_I(rows[j] + 2));
                    } else if (U_W(p + 0x27) != 0xffff) {
                        gm_Unk5091a0(p, &i);
                        ctl[n] = d2_GetWinObj(win, 2,
                                  io_PrintText(0x19fc, *(unsigned char *)p + 0x427,
                                               U_W(p + 0x27), p, rows[j] + 0x1c, i,
                                               *(unsigned char *)(rows[j] + 0x37),
                                               *(unsigned char *)(rows[j] + 0x36)));
                        U_I((unsigned)&d2_Objs + ctl[n] * 0x2ec + 0x2e8) = (int)p;
                        if (idx == -1) {
                            idx = d2_CreateRadioGroup(1);
                            U_I((unsigned)&d2_Objs + ctl[n] * 0x2ec + 0x28) = 1;
                            U_I((unsigned)&d2_Objs + ctl[n] * 0x2ec + 0x2c) = 1;
                            U_I((unsigned)&d2_Objs + ctl[n] * 0x2ec + 0x2e8) = (int)p;
                            U_I((unsigned)&d2_RadioSel + idx * 0x8c) = 0;
                        } else {
                            d2_RadioAdd(idx, ctl[n]);
                        }
                        n++;
                    }
                }
            }
            rebuild = 0;
        }
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == h && idx != -1 &&
            (i = d2_RadioGetSel(idx)) > -1) {
            v = ((int (*)(int, int, int))ai_Unk4c5010)(U_I(param_1 + 1), -1, U_I(rows[i] + 2));
            while (cm_GetSlotState(v) == 0)
                sv_PumpNet();
            rebuild = 1;
        }
        if (sv_RunFrame(ai_Unk5a45c0, 0x677c7) == 0)
            break;
    }
    if (idx != -1)
        d2_RadioClear(idx);
    sv_CameraFollow();
    d2_CloseView(win);
}
