/* 0x584010, the graveyard rent confirmation (587 B): rec = the record (its +0x27 the grave's owner word), a / b
   the values passed on, idx = the grave index; the cost 16000 when the owner is not the player (0x6f25b8), else
   0; the window "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the text
   0x4115 / 0x4114 (0x4112, the name 0x5eecd0[idx], the cost), the entries 0x4116 / +1 in 0x475b80 (window, 2,
   text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the click 0x14673a0 -> 0x6f22a0 = 1; the
   first entry: with a cost, 0x521040 (the character 0x770b80 + 0x6f25b8 * 0x21c, the cost, 0x6f1bc5) 0 -> the
   pump; 0x4f0e80 (the character, idx, rec, a, b, the cost) and its answer waited on 0x4c3ae0 (pump 0x20007),
   0x760638 = 0x677c7; the answer 2 -> the text 0x411b (0x4112, the name) through 0x5176d0 and 0x521150
   (0x100), else 0x6f22a0 = 1 and 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the
   result */

void unk_Unk4f0e80(int *g, int x, char *o, int v, char b, int y); int unk_Unk4f2330(); int unk_Unk4f0430(int *g, char *o, int v, short s, int x);

int ai_Unk584010(char *rec, int a, unsigned char b, int idx)
{
    char buf[0x200];
    int win;
    int b1;
    int b2;
    int v;
    int r;
    int h;
    unsigned int cost;

    r = 0;
    cost = -(unsigned int)(*(unsigned short *)(rec + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId) & 16000;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    if (*(unsigned short *)(rec + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId) {
        io_PrintText(0x4115, 0x4112, (int)rec, U_I(((unsigned)&ai_Unk5eeab8 + 0x218) + idx * 4));
    } else {
        io_PrintText(0x4114, 0x4112, U_I(((unsigned)&ai_Unk5eeab8 + 0x218) + idx * 4), cost);
    }
    d2_SetForm(win, 2);
    v = io_PrintText(0x4116);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                if (cost != 0) {
                    if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, cost, *(unsigned char *)(unsigned)&ai_OwnPlayer) == 0)
                        goto pump;
                }
                h = ((int (*)(void *, int, int, int, int, int))unk_Unk4f0e80)((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, idx, rec, a, b, cost);
                if (h != -1) {
                    while (cm_GetSlotState(h) == 0)
                        sv_RunFrame(0, 0x20007);
                }
                U_I((unsigned)&sv_FrameMask) = 0x677c7;
                if (cm_GetSlotState(h) == 2) {
                    txt_Format(buf, (char *)0x411b, 0x4112, U_I(((unsigned)&ai_Unk5eeab8 + 0x218) + idx * 4));
                    sv_MessageBox(buf, 0x100);
                } else {
                    U_I((unsigned)&sv_PopRequest) = 1;
                    r = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
pump:
        if (sv_RunFrame(ai_Unk584010, 0x677c7) == 0) {
            d2_CloseView(win);
            return r;
        }
    }
}
