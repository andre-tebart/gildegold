/* 0x5909a0, the construction window "BAUEN_GEB_BAUEN" (1988 B, ROUGH SKELETON from the Ghidra pseudocode): the
   window (0x5763c0 (0x40), the event 0x576040, the controls 0x4731d0 for the kinds 1 .. 9, the frames
   (0x475f70), the size / position 0x475ff0 + 0x52e860 and the text 0x515960); the kind 3 / 7 are gated by the
   player's bytes 0x770ce6 / 0x770ce4; the selected kind (0x62af98) switches the building list (0x501d80,
   0x5020e0, the controls 0x473650 / 0x4760b0 / 0x4734d0) and the placement call 0x58ff00; the frame pump
   0x5309a0 (this, 0x657c7) drives the loop; the exit frees the window (0x4758f0) and posts 0x576040 (0xe, 0, 1) */

int ai_Unk5763c0(); int d2_OpenForm();
int d2_SetForm(int view, int n); int d2_GetViewWin(); int sv_CreateFormSlider(); int io_PrintText(char *fmt, ...);
int d2_WinOpenPic(short x, short y, int pic, int win); int d2_CreateWinText(); void d2_SetObjFont(); void d2_SetObjText();  /* @original: the calls at 18534 (2), 18620 (3) and 6884 (3) pass fewer arguments than d2.h's 4-parameter prototype */
void d2_SetViewEnabled(); void d2_CloseView(int view); int cm_SendLongMessage(char *msg);
int sv_CheckGoodAmount(); int sv_CheckStock(); void txt_Format(char *out, int id0, ...); int sv_MessageBox();
int d3sm_GetVertex();  /* @original: the original callers read an int result (gv.h declares it int) */
int sv_Unk527640(); int gm_Unk509ee0(char *c, char p); int gm_BuildingClass(); char unk_Unk5020e0(int k, int c);

void ai_Unk5909a0(void)
{
    int ctrls[10];
    int sel;
    int chosen;
    int last;
    int slot;
    int i;
    int k;
    int u;
    int t;
    int n;
    int v;
    char text[0x40];
    char buf[0x104];
    char name[0x80];
    float pos[3];
    int a[3];
    int b[3];

    last = -1;
    sel = -1;
    chosen = 0;
    t = ai_Unk5763c0(0x40);
    if (t == 0)
        return;
    ai_Unk576040(0xe, 0, 0, 0);
    k = d2_OpenForm(0, 0, "BAUEN_GEB_BAUEN");
    d2_CentreViewWindows(k);
    d2_SetForm(k, 0);
    n = U_I((unsigned)&d2_LastWindow) * 0x3c4;
    U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x120) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x124) + n) = 0x18;
    U_I(((unsigned)&ai_Unkf8f39c + 0x128) + n) = 0x18;
    v = d2_GetViewWin(k, 3, 0x5a, a, b, &a[2]);
    sv_CreateFormSlider(0x20c, 0x168, U_I((unsigned)&d2_LastWindow), v);
    d2_SetForm(k, 1);
    io_PrintText(0x17eb);
    d2_SetForm(k, 2);
    u = 0;
    for (i = 1; i < 10; i++) {
        if (i == 2 || i == 5)
            continue;
        v = 1;
        if (i == 3) {
            if (*(unsigned char *)(((unsigned)&ai_CharInts + 0x162) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0xf &&
                *(unsigned char *)(((unsigned)&ai_CharInts + 0x162) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0xa)
                v = 0;
        } else if (i == 7) {
            if (*(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + U_W((unsigned)&ai_PlayerId) * 0x21c) < 0x34 ||
                0x39 < *(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + U_W((unsigned)&ai_PlayerId) * 0x21c))
                v = 0;
        }
        ctrls[i] = d2_WinOpenPic((u & 1) * 0x46 + 0x30, (u / 2) * 0x5a + 0xf, i + 0x6d4,
                                U_I((unsigned)&d2_LastWindow));
        n = ctrls[i] * 0x2ec;
        if (v == 0) {
            U_I(U_I((unsigned)&d2_Objs) + n + 0x3c) = 1;
            U_I(U_I((unsigned)&d2_Objs) + n + 0x50) = 1;
        } else {
            U_I(U_I((unsigned)&d2_Objs) + n + 0x4c) = 1;
            strcpy((char *)(U_I((unsigned)&d2_Objs) + n + 0xbc), (char *)U_I(((unsigned)&ai_BuildingNames + 0x1338) + i * 4));
            U_I(U_I((unsigned)&d2_Objs) + n + 0x1c0) = 3;
            U_I(U_I((unsigned)&d2_Objs) + n + 0x1bc) = 0x54;
            U_I(U_I((unsigned)&d2_Objs) + n + 0x1d0) = -1;
        }
        u++;
    }
    U_I(U_I((unsigned)&d2_Objs) + U_I((unsigned)&full_data_62af98) * 0x2ec + 0x28) = 1;
    /* ROUGH: the selection loop and the frame loop follow -- the input 0x6f8a668 (- 0x6d4) selects the kind
       0x62af98, the building list is rebuilt (0x501d80 / 0x5020e0, 0x473650 / 0x4760b0 / 0x4734d0), the
       placement call 0x58ff00 (the kind, the buffer), the message record (0x4c4b70) and the pump
       0x5309a0 (this, 0x657c7); the exit: 0x4758f0 (the window), 0x576040 (0xe, 0, 1) */
    while (sv_RunFrame(ai_Unk5909a0, 0x657c7) != 0)
        ;
    if (t != -1)
        d2_CloseView(t);
    U_I((unsigned)&sv_ResearchClass) = 0;
    ai_Unk576040(0xe, 0, 1, 0);
}
