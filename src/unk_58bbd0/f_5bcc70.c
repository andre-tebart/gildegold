/* 0x5bcc70, the player stats panel ("Panel_player_stats" 0x62d134, 1000 B): the radio group 0x46c490 over
   the three entry controls of 0x5bc420 / 0x5bc6d0 / 0x5bc9c0, the record 0xfa5aa0 + id * 0x8c (the flags at
   +0, the selected entry at +8), the carrier record of the player (0x770cf8 + player * 0x21c), the click
   dispatch 0xf8a668 / 0x5fe710 with the window list 0xf8f080 + id * 0x2ec, the text control rows
   0x6f40e4 .. 0x6f4cf4 (six dwords per row: the id -4, the cached value -1 and the three texts) and the
   pump 0x5309a0 (this, 0x657c7). The record 0x6f2f84 keeps the last selected entry */

void * d2_SetFont(int font); void d2_DrawTextBox(); void d2_FillPaintbox(int x, int y, int w, int h); int gv_GetTableRecord();
int gm_EquipmentValue(); void d2_RadioSelect(int g, int sel);
int ai_Unk5c33c0(unsigned short param_1, unsigned short param_2, unsigned int *param_3, int param_4);

int ai_Unk5bcc70(void)
{
    int win;
    int ctrlA, ctrlB, ctrlC;
    int id;
    int off;
    int sel;
    int i;
    int h;
    int out[3];
    int *p;
    int k;

    if (ai_Unk5763c0(0x800) == 0)
        return 0;
    ai_Unk576040(0x13, 0, 0, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d134);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 3);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0, out, 3, U_I((unsigned)&full_data_144a134), U_I((unsigned)&full_data_144a138), U_I((unsigned)&full_data_144a13c));
    ctrlA = ai_Unk5bc420();
    ctrlB = ai_Unk5bc6d0();
    ctrlC = ai_Unk5bc9c0();
    id = d2_CreateRadioGroup(3, out[0], out[1], out[2]);
    off = id * 0x8c;
    U_I((unsigned)&d2_Radios + off) |= 1;
    d2_RadioSelect(id, U_I((unsigned)&full_data_6f2f84));
    if (ctrlC == -1)
        d2_SetObjEnabled(out[2], 0);
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        sel = U_I((unsigned)&d2_RadioSel + off);
        if (sel == 0) {
            d2_SetViewEnabled(ctrlA, 1);
            d2_SetViewEnabled(ctrlB, 0);
            d2_SetViewEnabled(ctrlC, 0);
        } else if (sel == 1) {
            d2_SetViewEnabled(ctrlB, 1);
            d2_SetViewEnabled(ctrlA, 0);
            d2_SetViewEnabled(ctrlC, 0);
        } else if (sel == 2) {
            d2_SetViewEnabled(ctrlC, 1);
            d2_SetViewEnabled(ctrlB, 0);
            d2_SetViewEnabled(ctrlA, 0);
        }
        if (ctrlA != -1) {
            d2_SetForm(ctrlA, 1);
            ai_Unk59c120(U_I(((unsigned)&ai_Unk770cec + 0xc) + U_W((unsigned)&ai_PlayerId) * 0x21c), (int *)(unsigned)&full_data_6f3680, ctrlA, 1, 0);
        }
        if (ctrlB != -1 && U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&full_data_6f2f80) > 0) {
            for (i = 0; i < U_I((unsigned)&full_data_6f2f80); i++) {
                if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_6f4ce0 + i * 4)) {
                    d2_EnableViewObjects(win, 0);
                    ai_Unk5bc790((unsigned char *)U_I((unsigned)&d2_Objs + U_I((unsigned)&ai_Unk5fe710) * 0x2ec + 0x2e8));
                    d2_EnableViewObjects(win, 1);
                    break;
                }
            }
        }
        if (ctrlC != -1) {
            d2_SetForm(ctrlC, 0);
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                k = d2_GetViewWin(ctrlC, 3);
                h = d2_GetViewWin(ctrlC, 2);
                if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_6f3870)) {
                    d2_ScrollWindowBy(0, 0x3e, k);
                    k = 0x3e;
                } else if (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&full_data_6f4080)) {
                    d2_ScrollWindowBy(0, -0x3e, k);
                    k = -0x3e;
                }
                d2_ScrollWindowBy(0, k, h);
            }
            for (p = (int *)(unsigned)&full_data_6f40f4; p < (int *)(unsigned)((unsigned)full_data_6f40f4 + 0xc00); p += 6) {
                if (p[-4] != -1 && p[-1] != d2_GetObjValue(p[-4])) {
                    h = d2_GetObjValue(p[-4], *(unsigned char *)(unsigned)&ai_OwnPlayer);
                    i = ((int (*)(int))gm_PlayerFactor)(h);
                    cm_BeginFieldBlock((char *)*p, U_I(*p + 1));
                    cm_AddFieldDelta(4, 1, (short)*p - (short)U_I((unsigned)&cm_FieldObject) + 0x39, &i);
                    cm_SendFieldDeltas();
                    k = gm_FindCharacterByMaster(*p);
                    i = i - U_I(*p + 0x39);
                    cm_BeginFieldBlock((char *)k, U_I(k + 4));
                    cm_AddFieldValues(4, 1, (k - U_I((unsigned)&cm_FieldObject)) + 0x1b8, &i);
                    cm_SendFieldDeltas();
                    p[-1] = d2_GetObjValue(p[-4]);
                }
            }
        }
    } while (sv_RunFrame(ai_Unk5bcc70, 0x657c7) != 0);
    U_I((unsigned)&full_data_6f2f84) = U_I((unsigned)&d2_RadioSel + off);
    d2_RadioClear(id);
    d2_CloseView(win);
    if (ctrlA != -1)
        d2_CloseView(ctrlA);
    if (ctrlB != -1)
        d2_CloseView(ctrlB);
    if (ctrlC != -1)
        d2_CloseView(ctrlC);
    ai_Unk576040(0x13, 0, 1, 0);
    return 0;
}
