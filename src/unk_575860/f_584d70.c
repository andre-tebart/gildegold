/* 0x584d70, the graveyard "create helpers" window (1549 B, ROUGH: the calls and the flow are complete): b = the
   building record (byte +0 the building kind, +1 the id, +0x27 the owner word, +0x5d the owner record).
   0x4ffe90 (b+0x5d, 2, 6, 0, 0x2a) kept; 0x520fe0 (b) ends the handler when it fails. The building table
   0x7e3f30 + kind * 0x24d: the kind 0x1f and the limit byte +0x247 - two grave kinds 0x225 / 0x226 (0x293 /
   0x294 for the kind 0x1f) and a third 0x227 / 0x295 with the limit >= 2 (n = 2 / 3). The running 0x8c events
   0x53e400 (2, 0, 0x8c, 3, b + 1) whose +0xac word is a kind count their non-(-1) +0x8c[4] entries. The window
   "HANDEL\\HANDEL_SUCHEN" (0x62a634) with 0x475600, 0x475f70 (window, 0 / 3), 0x52e6d0 (0x6ed938, 0x70f,
   0x741) and the text 0x421e / 0x4128 (kind 0x1f), 0x525220 (b, 0), 0x58bbd0 (), 0x475600, then the buttons
   0x4731d0 (x, i * 0x50 + 0x19, kinds[i] + 0xdf, 0x6ed938) with x = *(short *)(0xf8f128 + 0x6ed938 * 0x3c4)
   / 2 - 0x80, the table 0xf8f080 +0x4c = 1 / +0x1c0 = 3 / +0x1cc = 100 / +0x1d0 = 0x10 / word +0x74 = 0x54,
   the icon U_I (0x143c860 + kinds[i] * 8) via 0x473540 and 0x475e40 (id, 1). Each frame 0x58d720 (b, the
   0x4ffe90 record) and 0x58dd00 (window, 1, 0, b), the counts recounted, 0x475f70 (window, 2), the button texts
   by the counts (0 / 1 / >= 2: 0x421f / 0x4220 / "%s" 0x5f8318 with kinds[i] * 2 + 0x9a8 for the kind 0x1f,
   0x4129 / 0x412a / "%s" for the others) into the line and 0x473540 (id, line). The pump loop 0x5309a0 (this
   window, 0x677c7): the click 0x5fe710 on a button -> the message (type 0x8c at +4, 1 at +0x36, the character
   0x770b84 + b[0x27] * 0x21c at +8, b + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at
   +0x28, the kind word at +0x58, the first two selected characters (byte +0x188) at +0x5c / +0x64, the free
   slots -1) through 0x4c4b70 and 0x4c3ae0 (pump 0x20007), 0x760638 = 0x677c7, 0x52ae40 (); no character ->
   0x521150 (U_I (0x144a40c), 4) and 0x52ae40 (); the escape 0x14673a0 -> 0x6f22a0 = 1; the pump 0 ->
   0x52ae40 (), 0x525440 (), 0x4758f0 (window), 0x52d7a0 () */

int sv_CheckCharacterFlag(); char * he_FindEvent(int n, ...); char * he_NextEvent(void); int sv_Unk52e6d0(); void d2_SetCaption();
void ai_Unk58bbd0(void); int ai_Unk58d720(); void ai_Unk58dd00(int param_1, int param_2, int param_3, int param_4);

void ai_Unk584d70(char *b)
{
    char line[0x100];
    char msg[0xf8];
    int btns[3];
    char *bldg;
    char *e;
    int res;
    int win;
    int counts[3];
    int kinds[3];
    int n;
    int i;
    int j;
    int k;
    int id;
    int h;
    int x;

    res = gm_Query(b + 0x5d, 2, 6, 0, 0x2a);
    bldg = (char *)(unsigned)&ai_BTypes + *(unsigned char *)b * 0x24d;
    n = 2;
    if (*(unsigned char *)(bldg + 0x247) >= 2)
        n = 3;
    if (*(unsigned char *)bldg == 0x1f) {
        kinds[0] = 0x293;
        kinds[1] = 0x294;
        kinds[2] = 0x295;
    } else {
        kinds[0] = 0x225;
        kinds[1] = 0x226;
        kinds[2] = 0x227;
    }
    if (sv_CheckCharacterFlag(b) != 0)
        return;
    counts[0] = 0;
    counts[1] = 0;
    counts[2] = 0;
    e = (char *)he_FindEvent(2, 0, 0x8c, 3, b + 1);
    while (e != 0) {
        for (i = 0; i < 3; i++) {
            if (*(unsigned short *)(e + 0xac) == kinds[i]) {
                for (j = 0; j < 4; j++)
                    if (*(int *)(e + 0x8c + j * 4) != -1)
                        counts[i]++;
            }
        }
        e = (char *)he_NextEvent();
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)"HANDEL\\HANDEL_SUCHEN");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x70f, 0x741);
    d2_SetForm(win, 3);
    if (*(unsigned char *)bldg == 0x1f)
        io_PrintText(0x421e);
    else
        io_PrintText(0x4128);
    sv_OpenPlayerBar(b, 0);
    ai_Unk58bbd0();
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    x = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2 - 0x80;
    for (i = 0; i < n; i++) {
        id = d2_WinOpenPic(x, i * 0x50 + 0x19, (char *)(kinds[i] + 0xdf), U_I((unsigned)&d2_LastWindow));
        *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + id * 0x2ec) = 3;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1cc + id * 0x2ec) = 100;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1d0 + id * 0x2ec) = 0x10;
        *(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x74 + id * 0x2ec) = 0x54;
        d2_SetCaption(id, (char *)U_I((unsigned)&ai_Unk143c860 + kinds[i] * 8));
        btns[i] = id;
        d2_SetObjEnabled(id, 1);
    }
    d2_SetPointerSprite(0, 0);
    ai_Unk58d720(b, res);
    ai_Unk58dd00(win, 1, 0, b);
    for (;;) {
        if (sv_RunFrame(ai_Unk584d70, 0x677c7) == 0)
            break;
        ai_Unk58d720(b, res);
        ai_Unk58dd00(win, 1, 0, b);
        counts[0] = 0;
        counts[1] = 0;
        counts[2] = 0;
        e = (char *)he_FindEvent(2, 0, 0x8c, 3, b + 1);
        while (e != 0) {
            for (i = 0; i < 3; i++) {
                if (*(unsigned short *)(e + 0xac) == kinds[i]) {
                    for (j = 0; j < 4; j++)
                        if (*(int *)(e + 0x8c + j * 4) != -1)
                            counts[i]++;
                }
            }
            e = (char *)he_NextEvent();
        }
        d2_SetForm(win, 2);
        for (i = 0; i < n; i++) {
            id = btns[i];
            d2_SetObjEnabled(id, 1);
            if (*(unsigned char *)bldg == 0x1f) {
                if (counts[i] == 1)
                    txt_Format(line, (char *)0x421f, 0x22d, kinds[i] * 2 + 0x9a8);
                else if (counts[i] <= 1)
                    txt_Format(line, "%s", kinds[i] * 2 + 0x9a8);
                else
                    txt_Format(line, (char *)0x4220, counts[i], 0x22d, kinds[i] * 2 + 0x9a8);
            } else {
                if (counts[i] == 1)
                    txt_Format(line, (char *)0x4129, 0x228, kinds[i] * 2 + 0x9a8);
                else if (counts[i] <= 1)
                    txt_Format(line, "%s", kinds[i] * 2 + 0x9a8);
                else
                    txt_Format(line, (char *)0x412a, counts[i], 0x228, kinds[i] * 2 + 0x9a8);
            }
            d2_SetCaption(id, line);
        }
        if (U_I((unsigned)&d2_ClickedId) == -1) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0)
                U_I((unsigned)&sv_PopRequest) = 1;
        } else {
            for (i = 0; i < n; i++)
                if (U_I((unsigned)&ai_Unk5fe710) == btns[i])
                    break;
            if (i != n) {
                memset(msg, 0, 0xf8);
                *(unsigned char *)(msg + 0x04) = 0x8c;
                *(unsigned char *)(msg + 0x36) = 1;
                *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(b + 0x27) * 0x21c);
                *(int *)(msg + 0x0c) = U_I(b + 1);
                *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                *(unsigned short *)(msg + 0x58) = kinds[i];
                k = 0;
                for (j = 0; j < 0x300 && k < 4; j++) {
                    if (*(char *)((unsigned)&ai_CharInts + j * 0x21c + 0x184)) {
                        *(int *)(msg + 0x5c + k * 4) = *(int *)((unsigned)&ai_CharInts + j * 0x21c);
                        k++;
                    }
                }
                for (j = k; j < 4; j += 2)
                    *(int *)(msg + 0x5c + j * 4) = -1;
                if (k == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 4);
                    sv_ClearSelection();
                } else {
                    h = cm_SendLongMessage(msg);
                    if (h != -1) {
                        while (cm_GetSlotState(h) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    U_I((unsigned)&sv_FrameMask) = 0x677c7;
                    sv_ClearSelection();
                }
            }
        }
    }
    sv_ClearSelection();
    sv_ClosePlayerBar();
    d2_CloseView(win);
    sv_CameraFollow();
}
