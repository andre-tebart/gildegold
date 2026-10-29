/* 0x52fc10, the network sync wait screen: sends the "sv_NetworkSync" message (0x4c4db0 (0xe, text)) and runs the message loop (0x5309a0 (0, fl | 0x300000)) until the answer arrives (0x4c3ae0); after 1.5 s
   (0x5dc ms) it shows the waiting dialog (0x521900 (0x143a3a8 = the text)); fl is the key mask kept in 0x760638 */

extern int ai_Unk623ec4, ai_Unk623ec8;  /* 0x623ec4, 0x623ec8 */
extern unsigned ai_Unk60d360;  /* 0x60d360 */
void cm_SendExSysMessage(unsigned char id, void *msg);                     /* 0x4c4db0 */
int cm_GetSlotState();                     /* 0x4c3ae0 */
void d2_SetViewEnabled(int view, int on);  /* 0x475590 */
int cm_FlushSendQueue();                     /* 0x4c3680 */
int cm_ReceiveMessages();                     /* 0x4c3fd0 */
int cm_ExecCommands();                     /* 0x4c4190 */
void d2_PlaceMainWindow();                     /* 0x475850 */
void d2_SetObjValue();                     /* 0x475cb0 */
void d2_SetObjFont();                     /* 0x4760b0 */
int io_PrintText(char *fmt, ...);                     /* 0x515960 */
int d2_GetWinObj();                     /* 0x475b80 */
void d2_SetObjMax();                     /* 0x475f40 */
int d2_GetObjValue();                     /* 0x475980 */
void d2_FocusInput();                     /* 0x478320 */
void ai_Unk4c5aa0(void *p);                     /* 0x4c5aa0 */
int sv_Unk52fe10(void);

void sv_NetworkSyncWait(unsigned fl)
{
    unsigned t0;
    char msg[0x7c];
    int m, dlg;

    t0 = timeGetTime();
    memset(msg, 0, sizeof(msg));
    strcpy(msg, "sv_NetworkSync");
    m = ((int (*)(int, int))cm_SendExSysMessage)(0xe, msg);
    sv_FrameMask = fl;
    dlg = sv_OpenMessageBox(ai_Unk143a3a8, 0);
    d2_SetViewEnabled(dlg, 0);
    while (cm_GetSlotState(m) == 0) {
        if (t0 + 0x5dc < timeGetTime())
            d2_SetViewEnabled(dlg, 1);
        sv_RunFrame(0, fl | 0x300000);
    }
    d2_CloseView(dlg);
}
