/* 0x59bad0, the money lender's credit giver window "..geldleihe_KreditGeber" (960 B): the window 0x4741f0
   (0x18d9), the screen texts 0x52e6d0 (0x73b, 0x73f), 0x475ff0 and 0x52e860 (0x206, 0x167), the rectangle
   0xe / 0x18 / 0xe / 0xe; 0x52e350 (screen, 0, out, 1, 0x1440534); on rebuild the 16 row records are freed
   (0x472ce0 / 0x46e710), 0x50a1c0 (slot, param_1, 0xffff, offers) collects the offers and their characters
   (0x505df0, the city word) into the 16 city slots, 0x18de (the city factor) / 0x48d (none), 0x50a880
   (count, cities, 0), then each city a row 0x18df (city, the count, the summed +0xb4) and the control
   0x475b80; a click on the release control (local_120[0]) opens 0x59b880 (param_1), one on a city row
   0x59b630 (param_1, city); 0x4758f0 / 0x52d7a0 on exit */

int gm_SumSlotAmounts(void *list, unsigned char i); int gm_GetGoodsValue(); int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
int d2_GetObjValue(); void d2_SetObjEnabled(); void d2_EnableViewObjects(); void d2_EnableObject();
int gm_Unk50a1c0(); int gm_Unk50a880(); int d2_RemoveWindow(); void d2_RemoveObj();
void d2_SetObjText(); void d2_MoveObj(short x, short y, int obj); int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win); void d2_SetObjFont();

void ai_Unk59bad0(int param_1)
{
    int recs[16 * 3];
    int cities[16];
    int pos[3];
    int sel[8];
    int offers[16];
    int win;
    int n;
    int cnt;
    int i;
    int j;
    int r;
    int sum;
    int v;
    int city;
    unsigned short *per;

    for (i = 0; i < 16; i++) {
        recs[i * 3] = -1;
        recs[i * 3 + 1] = -1;
        recs[i * 3 + 2] = 0;
    }
    d2_SetPointerSprite(0, 0);
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_KreditGeber");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x73b, 0x73f);
    v = d2_GetViewWin(win, 2, 0x78, &pos[0], &pos[1], &pos[2]);
    sv_CreateFormSlider(0x206, 0x167, U_I((unsigned)&d2_LastWindow), v);
    n = U_I((unsigned)&d2_LastWindow) * 0x3c4;
    U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + n) = 0xe;
    U_I(((unsigned)&ai_Unkf8f39c + 0x120) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x124) + n) = 0xe;
    U_I(((unsigned)&ai_Unkf8f39c + 0x128) + n) = 0xe;
    d2_SetForm(win, 1);
    io_PrintText(0x18d9);
    d2_SetForm(win, 3);
    sv_Unk52e350(U_I((unsigned)&d2_LastWindow), 0, sel, 1, U_I((unsigned)&full_data_1440534));
    cnt = 0;
    for (;;) {
        if (sv_RunFrame(ai_Unk59bad0, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (cnt == 0) {
            d2_SetForm(win, 2);
            for (i = 0; i < 16; i++) {
                if (recs[i * 3] != -1) {
                    d2_RemoveWindow(recs[i * 3]);
                    d2_RemoveObj(recs[i * 3 + 1]);
                    recs[i * 3] = -1;
                    recs[i * 3 + 1] = -1;
                    recs[i * 3 + 2] = 0;
                }
            }
            io_PrintText("$C");
            n = gm_Unk50a1c0(*(unsigned short *)(param_1 + 0x27), param_1, 0xffff, offers);
            j = 0;
            cnt = 0;
            for (i = 0; i < 16; i++)
                cities[i] = -1;
            for (i = 0; i < n; i++) {
                for (j = 0; j < 1 + cnt; j++) {
                    if (U_I((unsigned)&ai_CharInts + cities[j] * 0x87) == U_I(offers[i] + 0xb0))
                        break;
                    if (cities[j] == -1) {
                        per = (unsigned short *)gm_FindChar(U_I(offers[i] + 0xb0));
                        if (per != 0) {
                            cities[j] = *per;
                            cnt = j + 1;
                            break;
                        }
                    }
                }
            }
            d2_SetForm(win, 2);
            io_PrintText(0x18de, (unsigned int)U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4) * 2 + 0x9a8);
            if (cnt == 0)
                io_PrintText(0x48d);
            gm_Unk50a880(cnt, cities, 0);
            for (i = 0; i < cnt; i++) {
                r = gm_Unk50a1c0(*(unsigned short *)(param_1 + 0x27), param_1, cities[i], offers);
                sum = 0;
                for (j = 0; j < r; j++)
                    sum += U_I(offers[j] + 0xb4);
                v = d2_GetWinObj(win, 2, io_PrintText(0x18df, cities[i], r, sum));
                recs[i * 3] = v;
                recs[i * 3 + 1] = 0;
                recs[i * 3 + 2] = 0;
            }
        }
        cnt = 1;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            if (U_I((unsigned)&ai_Unk5fe710) == sel[0]) {
                ai_Unk59b880(param_1);
            } else {
                for (i = 0; i < 16 && recs[i * 3] != -1; i++) {
                    if (U_I((unsigned)&ai_Unk5fe710) == recs[i * 3]) {
                        ai_Unk59b630(param_1, cities[i]);
                        cnt = 0;
                    }
                }
            }
        }
    }
}
