/* 0x596ff0, the master-exam window "locations\wohnsitz\meisterpruefung" (897 B, ROUGH SKELETON from the Ghidra
   pseudocode): the exam level 0x504ce0 (the character's byte 0x770ce4) + 1; the window 0x4741f0 / 0x475600 /
   0x475f70 (0) / 0x52ea30 / 0x520f20, the skill 0x504510 (the player, the name local); the exam building
   0x53e400 (2, 2, the player, 0, 0x26): without it the failure / level texts 0x15bb / 0x15bc (the slider
   0x46a960, 0x475cb0) / 0x15be, with it 0x504ad0 (the level) and the course texts 0x15bf / 0x15c0 / 0x15c1
   (the day / hour of the exam); the frame loop: the click 0 0x5176d0 (0x74, 4) + 0x521150 (4) when the level
   is below 4, else 0x4c5f60 (the character, -4) and the message record (type 0x26, the character, the clock,
   0x4fe140, 0x4c4b70) and 0x6f22a0; the pump 0x5309a0 (this, 0x677c7); the exit 0x4758f0 / 0x52d7a0 */

int gm_TitlesLeft(); float gm_Unk504510(int n, int *out); int d2_WinOpenSlider();
void d2_SetObjValue(); int gm_TitleGroup(); void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);

void ai_Unk596ff0(void)
{
    int win;
    int b;
    int n;
    int c;
    unsigned int player;
    float total;
    char text[0x100];
    char name[0x10];
    unsigned char msg[0xf8];

    player = U_W((unsigned)&ai_PlayerId);
    n = gm_TitlesLeft(*(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + player * 0x21c)) + 1;
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\meisterpruefung");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    sv_ResetScreenClip();
    total = gm_Unk504510(U_W((unsigned)&ai_PlayerId), name);
    b = he_FindEvent(2, 2, U_W((unsigned)&ai_PlayerId), 0, 0x26);
    if (b == 0) {
        if (total < 1.0f || n > 6) {
            d2_SetForm(win, 1);
            io_PrintText(0x15bb);
            d2_SetForm(win, 2);
            if (n < 7) {
                io_PrintText(0x15bc, U_W((unsigned)&ai_PlayerId), name);
                c = d2_WinOpenSlider(0x1c, *(short *)((unsigned)&d2_LastWindowPtr + 10) - 0x3c,
                                 *(short *)((unsigned)&d2_LastWindowPtr + 8) - 0x40, 0, 100, 0x83, 0x42,
                                 U_I((unsigned)&d2_LastWindow));
                d2_SetObjValue(c, 0, 100, (int)total);
            } else {
                io_PrintText(0x15be, U_W((unsigned)&ai_PlayerId));
            }
        } else {
            c = *(unsigned char *)((unsigned)&ai_Unk770b89 + player * 0x21c);
            gm_TitleGroup(*(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + player * 0x21c));
            d2_SetForm(win, 1);
            io_PrintText(0x15bb);
            d2_SetForm(win, 2);
            io_PrintText(0x15bf, U_W((unsigned)&ai_PlayerId), 4,
                         *(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + player * 0x21c) +
                             (c != 0 ? 0x5f : 0) + 0x12f);
            if (n % 2 != 0)
                io_PrintText(0x15c0);
            d2_SetForm(win, 3);
            io_PrintText(0x15c1);
        }
    } else {
        d2_SetForm(win, 1);
        io_PrintText(0x15bb);
        d2_SetForm(win, 2);
        io_PrintText(0x15bd, U_W((unsigned)&ai_PlayerId),
                     *(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + player * 0x21c) +
                         (*(unsigned char *)((unsigned)&ai_Unk770b89 + player * 0x21c) != 0 ? 0x5f : 0) + 0x12f);
    }
    while (sv_RunFrame(ai_Unk596ff0, 0x677c7) != 0) {
        if (U_I((unsigned)&d2_ClickedId) != -1 && total >= 1.0f) {
            if (U_I((unsigned)&d2_ClickedId) == 0) {
                if (U_I(((unsigned)&ai_Selected + 0xc) + player * 0x21c) < 4) {
                    txt_Format(text, 0x74, 4);
                    sv_MessageBox(text, 4);
                } else {
                    cm_SendMoneyDelta(U_I((unsigned)&ai_CharInts + player * 0x87), -4);
                    memset(msg, 0, 0xf8);
                    *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + player * 0x87);
                    *(int *)(msg + 0xc) = -1;
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(unsigned char *)(msg + 4) = 0x26;
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(unsigned char *)(msg + 0x36) = 9;
                    *(int *)(msg + 0x58) = *(int *)(msg + 8);
                    gm_AddTime(msg + 0x28, 2, 0, 0);
                    cm_SendLongMessage(msg);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&d2_ClickedId) == 1) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
    sv_CameraFollow();
}
