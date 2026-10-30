/* 0x4b3350, the history dialog: shows the events of last year (0x4b3280) one after the other */


extern D2WIN d2_Wins[];            /* 0xf8f120 */
extern int d2_LastWindow;                /* 0x6ed938 */
extern struct D2WIN *d2_LastWindowPtr;            /* 0x6ed980 */
extern int d2_ClickedId;                /* 0xf8a668 */
void d2_SetPointerSprite(int a, int pic);                                               /* 0x476f80 */
int d2_SetForm(int view, int n);                                                 /* 0x475f70 */
void d2_CreateScrollButtons(int x, int y, int win, int obj, int pic);                      /* 0x472270 */
int sv_Unk52fa10(void);                                                          /* 0x52fa10, defined elsewhere */
void sv_Unk52fad0(void);                                                         /* 0x52fad0, defined elsewhere */
void sv_Unk52fa90(void);                                                         /* 0x52fa90, defined elsewhere */
int io_PrintText(char *fmt, ...);                                               /* 0x515960, defined elsewhere */
int sv_RunFrame(int cb, unsigned mask);                                      /* 0x5309a0, defined elsewhere */
extern int sv_PopRequest;                /* 0x6f22a0 */

void ai_Unk4b3350(void)
{
    char text[0x1fb8];
    char name[0x100];
    int view, win;
    int *vis;

    AI_ZERO(name, 0x100);
    AI_ZERO(text, 0x1fb8);
    if (hst_NextEvent(name, text) != 1)
        return;
    view = sv_Unk52fa10();
    d2_SetPointerSprite(0, 0);
    d2_SetForm(view, 1);
    win = d2_LastWindow;
    d2_SetForm(view, 2);
    io_PrintText((char *)0x10b0);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
    vis = &d2_Wins[win].visible;
    *vis = 0x18;
    d2_SetForm(view, 1);
    io_PrintText("$C%s", text);
    d2_SetForm(view, 3);
    io_PrintText("$C$Z$[%s$]", name);
    d2_ClickedId = -1;
    do {
        sv_Unk52fad0();
        if (d2_ClickedId == 0) {
            AI_ZERO(text, 0x1fb8);
            AI_ZERO(name, 0x100);
            if (hst_NextEvent(name, text) != 1) {
                sv_PopRequest = 1;
            } else {
                d2_SetForm(view, 2);
                io_PrintText((char *)0x10b0);
                d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
                *vis = 0x18;
                d2_SetForm(view, 1);
                io_PrintText("$C%s", text);
                d2_LastWindowPtr->scrollY = 0;
                d2_LastWindowPtr->scrollStep = 2;
                d2_SetForm(view, 3);
                io_PrintText("$C$Z$[%s$]", name);
            }
        }
    } while (sv_RunFrame(ai_Unk4b3350, 0x66087));
    sv_Unk52fa90();
}
