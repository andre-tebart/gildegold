/* 0x5c6d00, the office application (1488 B, ROUGH): param_1 = the request record (+2 the id, +6 the
   byte 6 = the office route), param_2 = the building; the job walk 0x53e400 (1, 0, 0x41) for the id
   (+0xac) gives the state 1 .. 4 (+0xbb / +0xba the two bytes) and the record 0x505df0 (+0xb0); a
   non-6 record with a +0x214 candidate (0x505df0, the +8 byte) sends the AIMSG 0x41 (the clock, the id,
   +0x5c the candidate) and returns 0x10, else 0x20; state 1 -> 0x5c6aa0; else the window
   "privillegien_werbung2" 0x62d40c with the texts 0x1cf3 / 0x1ce7 / 0x1ce3 / 0x1d31 / 0x1cf4 ..
   0x1cf8 and the four buttons; the frame loop 0x5309a0 (this, 0x657c7): a button click (the 0x21040
   gold check) rolls 0x42ec00 for the new rank (the texts 0x1cf9 / 0x1d02), adds the rank to +0xb8,
   when +0xb9 is reached writes the clock into +0x52 and 0x4fe140, clears +0xba, 0x4c4c60 (0, the job)
   into +0x84 and 0x4c47e0 / 0x521150; the fourth button confirms 0x1cfa; the exit 0x4758f0, returns 2.
   ROUGH: the button / rank ladder is condensed from Ghidra */

int ai_OfficeApplication(short *param_1, int param_2)
{
    int win;
    char *job;
    char *cand;
    int state;
    int id;
    int i;
    int v;
    int u;
    char c;
    char text[0x200];
    int ids[15];

    state = 1;
    id = -1;
    job = (char *)he_FindEvent(1, 0, 0x41);
    while (job != 0) {
        if (U_I(job + 0xac) == U_I((char *)param_1 + 4)) {
            id = U_I(job + 0xb0);
            state = 2;
            if (job[0xbb] == 1)
                state = 3;
            if (job[0xba] == 1)
                state = 4;
            break;
        }
        job = (char *)he_NextEvent();
    }
    if (*((char *)param_1 + 2) != 6) {
        cand = (char *)gm_FindChar(U_I((char *)param_2 + 0x214));
        if (job == 0 && cand != 0 && cand[8] != 0) {
            /* ROUGH: the AIMSG 0x41 (memset 0xf8, +4 = 0x41, +8 the id, +0x28 .. the clock, +0x58 the
               id, +0x5c the candidate id) through 0x4c4b70; returns 0x10 */
            return 0x10;
        }
        return 0x20;
    }
    if (state == 1)
        return ai_Unk5c6aa0(param_1);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d40c);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x1cf3);
    if (state == 2) {
        io_PrintText(0x1ce7, *param_1, U_W((char *)gm_FindChar(id)), (unsigned char)(job[0xb9] - job[0xb8]), 0x5d);
    } else if (state == 3) {
        io_PrintText(0x1ce3);
        io_PrintText(0x1d31, U_W((char *)gm_FindChar(id)));
    } else if (state == 4) {
        /* ROUGH: the three amount buttons 0x1cf5 .. 0x1cf7 (the +0xc0 / +0xc4 / +0xc8 amounts) and
           the confirm 0x1cf8 through 0x475b80 (win, 1) */
        io_PrintText(0x1cf4, U_W((char *)gm_FindChar(id)), 0x5d, (unsigned char)(job[0xb9] - job[0xb8]), 0x5d);
        ids[6] = d2_GetWinObj(win, 1, io_PrintText(0x1cf5, job[0xbc] + 0x1d11, U_I(job + 0xc0)));
        ids[7] = d2_GetWinObj(win, 1, io_PrintText(0x1cf6, job[0xbd] + 0x1d11, U_I(job + 0xc4)));
        ids[8] = d2_GetWinObj(win, 1, io_PrintText(0x1cf7, job[0xbe] + 0x1d11, U_I(job + 0xc8)));
        ids[9] = d2_GetWinObj(win, 1, io_PrintText(0x1cf8));
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1 && state == 4) {
            for (i = 0; i < 4; i++) {
                if (U_I((unsigned)&ai_Unk5fe710) == ids[i + 6])
                    break;
            }
            if (i < 3) {
                u = ids[i];
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c, u, U_I((unsigned)&ai_OwnPlayer)) != 0) {
                    /* ROUGH: the rank gain (the __ftol, the 0x42ec00 rolls 5 / 4 / 3 with the offsets
                       5 / 9 / 0xc, the text 0x1cf9 with 0x1d02), +0xb8 += the gain, at +0xb9 the clock
                       into +0x52 .. +0x5e with 0x4fe140 (job + 0x52, 0, 1, 0), +0xba = 0, 0x4c4c60 (0,
                       job) into +0x84, 0x4c47e0 and 0x521150 (text, 0) */
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (i == 3) {
                /* ROUGH: the confirmation 0x1cfa (the job record), +0xba = 0, 0x4c4c60, 0x521150 */
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_OfficeApplication, 0x657c7) == 0)
            break;
    }
    d2_CloseView(win);
    return 2;
}
