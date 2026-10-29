/* 0x598170, the tavern dice game window (2566 B, ROUGH SKELETON from the Ghidra pseudocode): the player's
   record 0x770b80 + player * 0x21c, the dice state local_378[0x15]; the money 0x50a640 / 0x5085b0, the
   character 0x506430; the intro window "locations\wirtshaus\WSIntro" (0x62b7c8) with 0x52ea30 / 0x475f70 /
   0x476f80, the texts 0x515960 (0x1874 .. 0x1882), the odds 0x42ec20 against the record +0x5c and the day/hour
   0x770ae4 / the money; the buttons 0x475b80; the game window "locations\wirtshaus\WS" (0x62b7b0) with the dice
   driver 0x597a10 and 0x4937a0 / 0x493a40 / 0x493890 / 0x493390 (the dice sets), 0x597e70 (the display),
   0x4c4830 (the result), 0x475590 / 0x5976a0 (the rules); the selection reads 0xf8a668 / 0x5fe710; the pumps
   0x5309a0 (this, 0x677c7 / 0x777c7); the exit 0x4758f0 / 0x52d7a0 */

int gm_GetWorth(unsigned short id); int gm_ConvertMoney(int a, unsigned char k); short * gm_PlayerRecord(char *c); int ai_Unk493200();
int ai_Unk4937a0(); void ai_Unk493a40(struct AGDUEL *d); int ai_Unk493890(); int ai_Unk493390();

void ai_Unk598170(int param_1)
{
    unsigned int player;
    int win;
    int rec;
    int money;
    int day;
    int b;
    int i;
    int ok;
    int quit;
    int state[0x15];
    int ctl[3];
    unsigned short *sets[7];
    char text[0x100];

    player = U_W((unsigned)&ai_PlayerId);
    memset(state, 0, sizeof(state));
    rec = gm_PlayerRecord((void *)((unsigned)&ai_Chars + player * 0x21c));   /* ai_Chars = 0x770b80, 0x21c per character (0x5981a5: 540 * player + 0x770b80) */
    money = ((int (*)(int))gm_ConvertMoney)(gm_GetWorth(U_W((unsigned)&ai_PlayerId)));
    win = d2_OpenForm(0, 0, "locations\\wirtshaus\\WSIntro");
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    d2_SetPointerSprite(0, 0);
    io_PrintText(0x1874);
    if (*(float *)(rec + 0x5c) <= (float)util_RandomF() || (unsigned short)U_I((unsigned)&ai_TimeHour) < 0xc ||
        money < 10000) {
        d2_SetForm(win, 2);
        io_PrintText(0x1875);
        if ((unsigned short)U_I((unsigned)&ai_TimeHour) > 0xb)
            *(float *)(rec + 0x5c) = *(float *)(rec + 0x5c) * 0.66f - 0.02f;
    } else {
        d2_SetForm(win, 2);
        io_PrintText(0x1876);
        d2_SetForm(win, 3);
        ctl[0] = d2_GetWinObj(win, 3, io_PrintText(0x1877));
        ctl[1] = d2_GetWinObj(win, 3, io_PrintText(0x1878));
        ctl[2] = d2_GetWinObj(win, 3, io_PrintText(0x1879));
        *(float *)(rec + 0x5c) = *(float *)(rec + 0x5c) * 0.66f - 0.1f;
    }
    quit = 0;
    U_I((unsigned)&d2_ClickedId) = -1;
    while (sv_RunFrame(ai_Unk598170, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) == 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == ctl[2]) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                    quit = 0;
                } else if (U_I((unsigned)&ai_Unk5fe710) == ctl[1]) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                    quit = 1;
                } else if (U_I((unsigned)&ai_Unk5fe710) == ctl[0]) {
                    d2_SetViewEnabled(win, 0);
                    ai_Unk5976a0();
                    d2_SetViewEnabled(win, 1);
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
            quit = 0;
        }
    }
    if (quit == 0 && *(char *)(((unsigned)&ai_CharInts + 0x4) + player * 0x21c) != 0) {
        /* ROUGH: the stakes 0x187a / 0x187b / 0x187c, 0x493200 (the player's record, the state, the stake
           1 / 2 / 4), the result text 0x187d / 0x187e with the state's first word, the shell texts 0x187f, and
           the two game panes (0x5176d0 0x1880 / 0x1881 / 0x1882, the dice sets apuStack_370 / apuStack_34c):
           the window "locations\wirtshaus\WS", 0x597a10 (mode 1, 0x96 / 0x14), 0x4937a0 / 0x493a40 (the
           throws), 0x493890 (the result), 0x493390 (the offers), 0x597e70 (the display), 0x4c4830 (the
           payout of the character) and the final pump loop until 0x14673a0 */
        d2_SetForm(win, 3);
        io_PrintText((char *)(unsigned)"$C");
        i = d2_GetWinObj(win, 3, io_PrintText(0x187a));
        b = d2_GetWinObj(win, 3, io_PrintText(0x187b));
        U_I((unsigned)&d2_ClickedId) = -1;
        do {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                i = 1;
                if (U_I((unsigned)&ai_Unk5fe710) == b)
                    i = 2;
                ai_Unk493200((void *)((unsigned)&ai_Chars + player * 0x21c), state, i);
                U_I((unsigned)&sv_PopRequest) = 1;
            }
            money = sv_RunFrame(ai_Unk598170, 0x777c7);
        } while (money != 0);
        d2_SetForm(win, 2);
        io_PrintText((char *)(unsigned)"$C");
        /* ROUGH: 0x187e when the first dice set is the player's own record, 0x187d otherwise */
        io_PrintText(0x187d, state[0]);
        d2_SetForm(win, 3);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText(0x187f);
        U_I((unsigned)&d2_ClickedId) = -1;
        do {
            if (U_I((unsigned)&d2_ClickedId) == 0)
                U_I((unsigned)&sv_PopRequest) = 1;
            money = sv_RunFrame(ai_Unk598170, 0x777c7);
        } while (money != 0);
        if (*(char *)(((unsigned)&ai_CharInts + 0x4) + player * 0x21c) != 0) {
            d2_CloseView(win);
            memset(sets, 0, sizeof(sets));
            txt_Format(text, 0x1880, state[0]);
            win = d2_OpenForm(0, 0, "locations\\wirtshaus\\WS");
            d2_SetForm(win, 0);
            sv_FitForm(U_I((unsigned)&d2_LastWindow));
            d2_CentreViewWindows(win);
            d2_SetForm(win, 1);
            d2_SetPointerSprite(0, 0);
            io_PrintText(0x1874);
            ai_Unk597a10(state, (int *)sets, 1, win, 2, 0x96, 0);
            if (*(char *)(((unsigned)&ai_CharInts + 0x4) + player * 0x21c) != 0) {
                ai_Unk4937a0(state, sets[0], 1);
                ai_Unk493a40(state);
                ai_Unk597a10(state, (int *)sets, 1, win, 2, 0x14, 1);
                ai_Unk597a10(state, (int *)sets, 1, win, 3, 0x96, 0);
                ai_Unk4937a0(state, sets[0], 1);
                ai_Unk493a40(state);
                ai_Unk597a10(state, (int *)sets, 1, win, 3, 0x14, 1);
                ai_Unk597e70(state, win);
                cm_SendMoveGoodsUnchecked(U_I((unsigned)&ai_CharInts + player * 0x87), 0xffffffff, 0, 0);
                while (sv_RunFrame(ai_Unk598170, 0x777c7) != 0) {
                    if (U_I(((unsigned)&dm_L + 0x20)) != 0)
                        U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
        }
    }
    d2_CloseView(win);
    sv_CameraFollow();
}
