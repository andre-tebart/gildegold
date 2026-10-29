/* 0x581020, the spy / comment window (981 B, ROUGH: the three amounts v[0..2] are three float -> int
   conversions (0x5de518 = __ftol) whose FPU arguments Ghidra lost, they are 0 here): arg 1 = the record; the
   window "Locations_Domizil_kabinett_spy" (0x62a3f8) with 0x476f80 (0, 0) / 0x475600, the sound 0x52ea30
   (0x6ed938), a slider 0x46a960 (0x57, 0x14, 200, 0 .. 100, 0x7b, 2) with 0x46bca0 and 3 steps, the two entries
   0x475b80 (window, 3, text 0x4024 / +1), 0xf8a668 = -1; the pump loop 0x5309a0 (this handler, 0x677c7): the
   slider's step 0x475980, the texts 0x602568 / 0x401e / 0x401c / 0x401f, the click 0x14673a0 -> 0x6f22a0 = 1;
   the first entry: 0x521040 (the character 0x770b80 + 0x6f25b8 * 0x21c, the amount, 0x6f1bc5), 0x580fe0 (rec),
   0x506430 (the character) +0x28 += the amount, 0x4c4830 (-1, the character, 0x6f1bc5, the amount) and the
   message 0xf8 bytes (type 0x99 at +4, 2 at +0x36, the character 0x770b84 at +8, the record + 1 at +0xc, the game
   time 0x770ae0 at +0x28, -1 at +0x60 / +0x64, 3 at +0x68, the record + 1 at +0x5c, the amount at +0x6c, 0x4a63f0
   (the character, 0, 0x7b4c4a0, 0, 0, 100.0, &the value at +0x58)) and 0x4c2760 (-7, 0, "KOMMENTARE" 0x62a3ec, 0)
   then 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x52d7a0 () */

int gm_GetWorth(unsigned short id); int d2_WinOpenSlider(); int sv_CheckGoodAmount(); short * gm_PlayerRecord(char *c);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int ai_Unk4a63f0(struct AIPER *p, int a, __int64 mask, int c, float f, int *out); int snd_PlayVoice(int id, int bank, char *text, int var); void sv_CameraFollow();

void ai_Unk581020(char *rec)
{
    char msg[0xf8];
    int v[3];
    int win;
    int slider;
    int id;
    int i;
    int r;
    int b1;
    int b2;
    int lv;

    gm_GetWorth(U_I((unsigned)&ai_PlayerId));
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3f8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 2);
    id = d2_CreateWinText(0, 3, (char *)U_I((unsigned)&full_data_144a240), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + id * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    d2_SetObjFont(id, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 200, 0, 100, 0x7b, 2, U_I((unsigned)&d2_LastWindow));
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, U_I((unsigned)&full_data_144a244), U_I((unsigned)&full_data_144a248), U_I((unsigned)&full_data_144a24c));
    d2_SetForm(win, 3);
    i = io_PrintText(0x4024);
    b1 = d2_GetWinObj(win, 3, i);
    b2 = d2_GetWinObj(win, 3, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText(0x401e, 0x401c);
        id = v[i];
        io_PrintText(0x401f, id);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, id, *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0) {
                    if (ai_Unk580fe0(rec) != 0) {
                        r = gm_PlayerRecord((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
                        if (r != 0)
                            *(int *)(r + 0x28) += id;
                        cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, id);
                        memset(msg, 0, 0xf8);
                        *(unsigned char *)(msg + 0x04) = 0x99;
                        *(unsigned char *)(msg + 0x36) = 2;
                        *(int *)(msg + 0x60) = -1;
                        *(int *)(msg + 0x64) = -1;
                        *(int *)(msg + 0x68) = 3;
                        *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                        *(int *)(msg + 0x0c) = U_I(rec + 1);
                        *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                        *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                        *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                        *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                        *(int *)(msg + 0x5c) = U_I(rec + 1);
                        *(int *)(msg + 0x6c) = i;
                        lv = -1;
                        AI_4A63F0_7((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0, (char *)0x7b4c4a0, 0, 0, 100.0f, &lv);
                        *(int *)(msg + 0x58) = lv;
                        id = cm_SendLongMessage(msg);
                        if (id != -1)
                            while (cm_GetSlotState(id) == 0)
                                sv_RunFrame(0, 0x20007);
                        snd_PlayVoice(-7, 0, (char *)(unsigned)&full_data_62a3ec, 0);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk581020, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
