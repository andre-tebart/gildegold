/* 0x5a1af0, the church donation window "Locations\Kirche\Spenden" (1141 B): param_1 is the building record (+0x27
   the owner slot; unknown 0xffff / the player's own slot close it), the owner record 0x4ffe90 (param_1 + 0x5d, 2,
   6, 0, 0xe8); the relation total sum = max(0x50a640 (the player slot), 1) + max(0x50a640 (the owner slot), 1)
   (the windows.h max macro calls twice) and the donation percent (int)(sum * 0.01f) against the player's money
   0x509e60; too little money shows the text 0x1987, else the window 0x4741f0, 0x476f80 / 0x475600 and 0x52ea30
   (the screen 0x6ed938) with the texts 0x1986 / 0x1988 (the owner slot, 0x48c, the city factor 0x7e3f94 + city *
   0x2f4), the amount 0x5bd060 (the percent, the player's money, 0x20), the slider 0x475b80 / 0x475cb0 (the
   0x5085b0 amount chain) and the two buttons 0x475b80 (0x1989, +1); the pump 0x5309a0 (this, 0x677c7): a click
   (0x5fe710) reads the slider 0x475980 / 0x5085f0 and asks 0x521040 (the player, the amount, the city), then
   pays 0x4c47e0 (the owner record + 2, the player id, the city, the amount), gives the relation 0x4c4b00 (the
   player, the owner, the amount * 0xff / the sum) and a fifth of it (0.2f) to every same-religion character
   (0x770b8c) of the 0x300 records, then the text 0x5176d0 ("%s%s", 0x1986, 0x198a) and 0x521150; 0x4758f0 /
   0x52d7a0 at the end */

char * gm_Query(void *list, int n, ...); int gm_GetWorth(unsigned short id); int gm_GetGoodsValue(); int ai_Unk5bd060();
int d2_OpenForm(); void d2_SetPointerSprite(int a, int pic); int d2_SetForm(int view, int n);
void sv_FitForm(int f); int io_PrintText(char *fmt, ...); int d2_GetWinObj(); void d2_SetObjValue();
int gm_ConvertMoney(int a, unsigned char k); int d2_GetObjValue(); int gm_PlayerFactor(int a, unsigned char k); int sv_CheckGoodAmount();
void cm_SendMoveGoods(int a, int b, unsigned char c, int d); void cm_SendRelations(int a, int b, int c, int d); void txt_Format(char *out, int id0, ...); int sv_MessageBox();
int sv_RunFrame(int cb, unsigned mask); void d2_CloseView(int view); void sv_CameraFollow(); int ai_Unk5de518();

void ai_Unk5a1af0(int param_1)
{
    int room;
    int sum;
    int pct;
    int win;
    int ctl = -1;
    int btn1 = -1;
    int btn2 = -1;
    int amount;
    int money;
    int v;
    int i;
    int id;
    char k;
    float sf;
    float f;
    char text[0x200];

    if (param_1 == 0)
        return;
    if (U_W(param_1 + 0x27) == 0xffff || U_W(param_1 + 0x27) == U_W((unsigned)&ai_PlayerId))
        return;
    room = gm_Query(U_I(param_1 + 0x5d), 2, 6, 0, 0xe8);
    if (room == 0)
        return;
    k = *(char *)(unsigned)&ai_OwnPlayer;
    sum = max(gm_GetWorth(U_W((unsigned)&ai_PlayerId)), 1) +
          max(gm_GetWorth(U_W(param_1 + 0x27)), 1);
    sf = (float)sum;
    pct = (int)(sf * 0.01f);
    money = gm_GetGoodsValue((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Spenden");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x1986);
    if (money < pct) {
        io_PrintText(0x1987, U_W((unsigned)&ai_PlayerId),
                     (unsigned int)U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4) * 2 + 0x9a8);
    } else {
        v = io_PrintText(0x1988, U_W((unsigned)&ai_PlayerId), U_W(param_1 + 0x27), 0x48c,
                         (unsigned int)U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4) * 2 + 0x9a8);
        ai_Unk5bd060(pct, money, 0x20, &amount);
        ctl = d2_GetWinObj(win, 1, v);
        d2_SetObjValue(ctl, ((int (*)(int, int, int))gm_ConvertMoney)(pct, k,
                            ((int (*)(int, int, int))gm_ConvertMoney)(amount, k, gm_ConvertMoney(pct, k))));
        d2_SetForm(win, 2);
        v = io_PrintText(0x1989);
        btn1 = d2_GetWinObj(win, 2, v);
        btn2 = d2_GetWinObj(win, 2, v + 1);
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == btn1) {
                amount = gm_PlayerFactor(d2_GetObjValue(ctl), *(unsigned char *)(unsigned)&ai_OwnPlayer);
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, amount,
                                 U_I((unsigned)&ai_OwnPlayer)) != 0) {
                    cm_SendMoveGoods(U_I(room + 2),
                                 U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c), k, amount);
                    f = (float)(amount * 0xff) / sf;
                    ((void (*)(int, int, int))cm_SendRelations)(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c),
                                 U_I((unsigned)&ai_CharInts + U_W(param_1 + 0x27) * 0x21c), (int)f);
                    f = f * 0.2f;
                    for (i = 0; i < 0x300; i++) {
                        id = U_I((unsigned)&ai_CharInts + i * 0x21c);
                        if (i != U_W((unsigned)&ai_PlayerId) && id != -1 &&
                            id != U_I((unsigned)&ai_CharInts + U_W(param_1 + 0x27) * 0x21c) &&
                            *(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x3) + U_W(param_1 + 0x27) * 0x21c) ==
                            *(unsigned char *)(((unsigned)&ai_Unk770b89 + 0x3) + i * 0x21c))
                            ((void (*)(int, int, int))cm_SendRelations)(U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c), id,
                                         (int)f);
                    }
                    txt_Format(text, "%s%s", 0x1986, 0x198a);
                    sv_MessageBox(text, 0);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == btn2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5a1af0, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
