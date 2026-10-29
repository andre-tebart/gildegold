/* 0x5ae6b0, a transport order message (415 B): every carrier of the kind 0x11 (0x53e400 (2, 3, the +2 field of
   param_2, 0, 0x11)) gets the clock (+0x52 .. +0x5e) and +0x70 = -1; 0x576340 (0x24, param_4, 0, 0) must answer 1;
   the AIMSG 0xf8 bytes: type 0xf (+4), the character 0x770b84 + param_1 * 0x21c (+8), -1 (+0xc), the clock
   (+0x28 .. +0x36 = 2), the +2 of param_2 (+0x58), the +1 of param_3 (+0x5c) and of param_4 (+0x60), and the
   char +0x28 of param_2 at +0x78 (also the global 0x6f2f64); 0x4c4b70 sends it; the amount 0x50aa00 / 0x50a970 of
   the +0x28 field is paid to 0x770b84 + param_1 * 0x21c (0x4c4830); for the player 0x6f25b8 the event 0x24 is
   confirmed (0x576040) */

int ai_Unk5ae6b0(unsigned int param_1, int param_2, int param_3, int param_4)
{
    char msg[0xf8];
    char rec[176];
    int carrier;
    int v;

    carrier = he_FindEvent(2, 3, *(int *)(param_2 + 2), 0, 0x11);
    while (carrier != 0) {
        *(int *)(carrier + 0x52) = U_I((unsigned)&ai_Time);
        *(int *)(carrier + 0x56) = U_I((unsigned)&ai_TimeHour);
        *(int *)(carrier + 0x5a) = U_I(((unsigned)&ai_Time + 0x8));
        U_W(carrier + 0x5e) = U_W(((unsigned)&ai_Time + 0xc));
        *(int *)(carrier + 0x70) = -1;
        carrier = he_NextEvent();
    }
    if (ai_Unk576340(0x24, param_4, 0, 0) != 1)
        return 0;
    memset(msg, 0, 0xf8);
    *(unsigned char *)(msg + 4) = 0xf;
    *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + param_1 * 0x21c);
    *(int *)(msg + 0xc) = -1;
    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    U_W(msg + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
    *(unsigned char *)(msg + 0x36) = 2;
    *(int *)(msg + 0x58) = *(int *)(param_2 + 2);
    *(int *)(msg + 0x5c) = *(int *)(param_3 + 1);
    *(int *)(msg + 0x60) = *(int *)(param_4 + 1);
    U_I((unsigned)&full_data_6f2f64) = *(char *)(param_2 + 0x28);
    *(int *)(msg + 0x78) = U_I((unsigned)&full_data_6f2f64);
    cm_SendLongMessage((char *)msg);
    v = 0;
    if (*(char *)(param_2 + 0x28) != 0) {
        gm_StallGoodsList((void *)param_2, (void *)rec);
        v = gm_Unk50a970(U_I(rec + 0xb0), *(char *)(param_2 + 0x28));
        cm_SendMoveGoodsUnchecked(-1, U_I((unsigned)&ai_CharInts + param_1 * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
    }
    if (param_1 == U_W((unsigned)&ai_PlayerId))
        ai_Unk576040(0x24, param_4, 0, 0);
    return v;
}
