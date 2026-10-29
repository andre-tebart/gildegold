/* 0x595a00, the trade search window of the kind 0x15 (1222 B, ROUGH SKELETON from the Ghidra pseudocode): as
   0x595480 with the searched goods 0x1c1 / 0x1c2 / 0x1c3, the building 0x53e400 (2, 0, 0x15, 3), the text
   0x515960 (0x1806), always three entries, the texts 0x5176d0 (0x1807 / 0x5f8318 / 0x1808) and the message type
   0x15 */

void ai_AquariumSearchWindow(int param_1, int param_2)
{
    unsigned int kinds[7];
    int sums[3];
    int win;
    int ctrls[4];
    int b;
    int i;
    int n;
    unsigned int kind;
    unsigned short h;
    char text[0x100];
    unsigned char msg[0xf8];

    kinds[0] = 0x1c1;
    kinds[1] = 0x1c2;
    kinds[2] = 0x1c3;
    kinds[6] = gm_Query(*(int *)(param_1 + 0x5d), 2, 6, 0, 0x2a);
    if (sv_CheckCharacterFlag(param_1) != 0)
        return;
    sums[0] = 0;
    sums[1] = 0;
    sums[2] = 0;
    for (b = he_FindEvent(2, 0, 0x15, 3, U_I(param_1 + 1)); b != 0; b = he_NextEvent()) {
        for (i = 0; i < 3; i++) {
            if (*(unsigned short *)(b + 0xae) == kinds[i]) {
                sums[i] += *(unsigned char *)(b + 0xac);
                break;
            }
        }
    }
    win = d2_OpenForm(0, 0, "HANDEL\\HANDEL_SUCHEN");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x70f, 0x741);
    d2_SetForm(win, 3);
    io_PrintText(0x1806);
    sv_OpenPlayerBar(param_1, 0);
    ai_Unk58bbd0();
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    h = *(unsigned short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    for (i = 0; i < 3; i++) {
        int id = d2_WinOpenPic((int)h / 2 - 0x80, i * 0x50 + 0x19, kinds[i] + 0xdf, U_I((unsigned)&d2_LastWindow));
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x4c) = 1;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1c0) = 3;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1cc) = 100;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1d0) = 0x10;
        *(unsigned short *)(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x74) = 0x54;
        d2_SetCaption(id, U_I((unsigned)&ai_Unk143c860 + kinds[i] * 8));
        ctrls[i] = id;
        d2_SetObjEnabled(id, 1);
    }
    d2_SetPointerSprite(0, 0);
    ai_Unk58d720(param_1, kinds[6]);
    ai_Unk58dd00(win, 1, 0, param_1);
    while (sv_RunFrame(ai_AquariumSearchWindow, 0x677c7) != 0) {
        ai_Unk58d720(param_1, kinds[6]);
        ai_Unk58dd00(win, 1, 0, param_1);
        sums[0] = 0;
        sums[1] = 0;
        sums[2] = 0;
        for (b = he_FindEvent(2, 0, 0x15, 3, U_I(param_1 + 1)); b != 0; b = he_NextEvent()) {
            for (i = 0; i < 3; i++) {
                if (*(unsigned short *)(b + 0xae) == kinds[i]) {
                    sums[i] += *(unsigned char *)(b + 0xac);
                    break;
                }
            }
        }
        d2_SetForm(win, 2);
        for (i = 0; i < 3; i++) {
            d2_SetObjEnabled(ctrls[i], 1);
            if (sums[i] == 1)
                txt_Format(text, 0x1807, 1);
            else if ((int)sums[i] < 2)
                txt_Format(text, "%s", kinds[i] * 2 + 0x9a8);
            else
                txt_Format(text, 0x1808, sums[i]);
            d2_SetCaption(ctrls[i], text);
        }
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            for (i = 0; i < 3; i++) {
                if (ctrls[i] == U_I((unsigned)&ai_Unk5fe710))
                    break;
            }
            if (i < 3) {
                memset(msg, 0, 0xf8);
                *(unsigned char *)(msg + 4) = 0x15;
                *(unsigned char *)(msg + 0x36) = 1;
                *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(param_1 + 0x27) * 0x87);
                *(int *)(msg + 0xc) = U_I(param_1 + 1);
                *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                *(unsigned short *)(msg + 0x5a) = kinds[i];
                *(int *)(msg + 0x5c) = U_I(param_2 + 2);
                /* ROUGH: the six character slots and 0x4c2d20 / 0x4c4b70 / 0x4c3ae0 / 0x4c3b10 */
                kind = cm_SendLongMessage(msg);
                n = cm_GetSlotState(kind);
                while (n == 0) {
                    sv_RunFrame(0, 0x20007);
                    n = cm_GetSlotState(kind);
                }
                cm_GetSlotValue(kind);
            }
        } else if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    }
    sv_ClearSelection();
    sv_ClosePlayerBar();
    d2_CloseView(win);
    sv_CameraFollow();
}
