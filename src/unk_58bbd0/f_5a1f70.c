/* 0x5a1f70, the indulgence window "Locations\Kirche\Ablass2" (1063 B, ROUGH): the player's record id
   (0x770b84 + slot * 0x21c), its fame 0x4f39b0 (n2) and rank 0x4f39e0 (n1); with n1 the price
   w = (int)(n1 * x * 0x50a640 (the player slot) * 0.005f), x = 1.5f for the player's own building and
   (200.0f - 0x50c550 (the player slot, the owner slot, 1)) * 0.01f otherwise, then the clamp
   t = w * 0.1 into 3200.0 .. 320000.0 (double) and w / 2 for 0x5ba6b0 (the player, 0x2c6); the window
   0x4741f0, 0x476f80 / 0x475600 / 0x475f70 and 0x52ea30 (the screen 0x6ed938), the texts 0x1960, then
   0x1962 (n2, w) / 0x1963 with the control 0x475b80 (2) or 0x1961 (the slot) without a price; the pump
   0x5309a0 (this, 0x677c7): a click on the control (0x5fe710) with 0x521040 (the player, w, the city)
   confirms: 0x4c3ba0 (the player, the id), 0x4c3d00 (4, 1, 0x1ac), 0x4c49a0, 0x4f3a30 (the id, n2) and
   for an owner the relation d = (int)(w / log10 (w)): 0x4c47e0 (-1, the id, the city, w),
   0x4c47e0 (the owner id, -1, the city, d) and 0x5a8b80 (the player, the owner, the building, d, w),
   else 0x4c47e0 (-1, the id, the city, w); the answer 0x4c3ae0 / 0x52fe00, the text 0x5176d0 (0x1964,
   the record +0x1ac, 0x50b170 (the slot, 0)) and 0x521150 (buf, 0x100); the exit 0x4758f0 and 0x5a28f0
   (the "Locations\Kirche\Top5" window) after a donation, else 0x52d7a0. ROUGH: the clamp is written as
   two ifs; the register roles of the price block are not checked */

int unk_Unk4f39b0(); int unk_Unk4f39e0(int c); int gm_GetWorth(unsigned short id); float gm_Liking();
int ai_InventoryHas(void *ch, int id); int d2_OpenForm(); void d2_SetPointerSprite(int a, int pic); 
int d2_SetForm(int view, int n); void sv_FitForm(int f); int io_PrintText(char *fmt, ...); int d2_GetWinObj();
int sv_CheckGoodAmount(); void cm_BeginFieldBlock(char *o, int id); int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v); void cm_SendFieldDeltas(void);
int unk_Unk4f3a30(int p, int n); void cm_SendMoveGoods(int a, int b, unsigned char c, int d); void ai_Unk5a8b80(); int cm_GetSlotState();
void sv_PumpNet(); int gm_Unk50b170(); void txt_Format(char *out, int id0, ...); int sv_MessageBox();
int sv_RunFrame(int cb, unsigned mask); void d2_CloseView(int view); void sv_CameraFollow(); int ai_Unk5de518();
void ai_Top5Window();

void ai_IndulgenceWindow(int param_1, int param_2)
{
    int n1;
    int n2;
    int win;
    int ctl = -1;
    int w = 0;
    int d;
    int h;
    int v;
    int ok = 0;
    int id;
    unsigned short owner;
    char k;
    char buf[0x200];
    float x;
    float a;
    double t;

    id = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x21c);
    n2 = unk_Unk4f39b0(id);
    n1 = unk_Unk4f39e0(id);
    if (n1 != 0) {
        owner = U_W(param_1 + 0x27);
        a = gm_GetWorth(U_W((unsigned)&ai_PlayerId)) * 0.005f;
        if (owner == U_W((unsigned)&ai_PlayerId))
            x = 1.5f;
        else
            x = (200.0f - gm_Liking(U_W((unsigned)&ai_PlayerId), owner, 1)) * 0.01f;
        w = (int)(n1 * x * a);
        t = w * 0.1;
        if (t > 3200.0) {
            if (!(t < 320000.0))
                t = 320000.0;
        } else
            t = 3200.0;
        w = (int)t;
        if (ai_InventoryHas((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, 0x2c6))
            w /= 2;
    }
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Ablass2");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x1960);
    if (w > 0) {
        io_PrintText(0x1962, n2, w);
        d2_SetForm(win, 2);
        ctl = d2_GetWinObj(win, 2, io_PrintText(0x1963));
    } else {
        io_PrintText(0x1961, U_W((unsigned)&ai_PlayerId));
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == ctl) {
            k = *(char *)(unsigned)&ai_OwnPlayer;
            if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, w,
                             U_I((unsigned)&ai_OwnPlayer)) != 0) {
                cm_BeginFieldBlock((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, id);
                cm_AddFieldValues(4, 1, 0x1ac, &v);
                cm_SendFieldDeltas();
                unk_Unk4f3a30(id, n2);
                owner = U_W(param_1 + 0x27);
                if (owner != 0xffff && owner != U_W((unsigned)&ai_PlayerId)) {
                    d = (int)(w / log10((double)w));
                    h = ((int (*)(int, int, int, int))cm_SendMoveGoods)(-1, id, k, w);
                    h = ((int (*)(void *, int, int, int))cm_SendMoveGoods)(U_I((unsigned)&ai_CharInts + owner * 0x21c), -1, k, d);
                    ai_Unk5a8b80((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c,
                                 (char *)(unsigned)&ai_Chars + owner * 0x21c, (char *)param_1, d, w);
                } else {
                    h = ((int (*)(int, int, int, int))cm_SendMoveGoods)(-1, id, k, w);
                }
                if (h != -1) {
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                }
                txt_Format(buf, 0x1964,
                             U_I((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c + 0x1ac),
                             gm_Unk50b170(U_W((unsigned)&ai_PlayerId), 0));
                sv_MessageBox(buf, 0x100);
                ok = 1;
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_IndulgenceWindow, 0x677c7) == 0)
            break;
    }
    d2_CloseView(win);
    if (ok)
        ai_Top5Window();
    else
        sv_CameraFollow();
}
