/* 0x593f40, the burglar-guild break-in callback (562 B, ROUGH SKELETON from the disassembly; an unnamed Ghidra
   start, passed as the callback of 0x5b2180 by 0x594180): the name param_1 (0 = return 0); the window
   "locations\diebesgilde\diebesgilde_einbruch" (0x4741f0 / 0x475600 / 0x476f80 / 0x475f70 (1)), the two
   controls 0x475b80 (0x515960 (0x1a95) and its successor); the click 0xf8a668 / 0x5fe710 on the first control
   searches the building of the record 0x6f230c (0x53e400 (3, 0, 0x48, 2, the record +0x27, 3, the record +1),
   0x53e300) whose +0xac name equals param_1; when none is found the message (kind 0x48, the character
   0x770b84, the record +1, the clock 0x770ae0 .., the name at +0x74, the -1 mask) goes through 0x4c2d20 /
   0x4c4b70 (the table 0x7603a0 .. 0x760420) and 0x52ae40; the click on the second control sets 0x6f22a0; the
   pump 0x5309a0 (this, 0x677c7); the exit 0x4758f0 (the window), returns 1 */

int ai_Unk593f40(char *name)
{
    char buf[0x104];
    int win;
    int ctl1;
    int ctl2;
    int id;
    int b;
    int rec;
    int i;
    int msg[0x20];
    int *p;

    if (name == 0)
        return 0;
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_einbruch");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    id = io_PrintText(0x1a95);
    ctl1 = d2_GetWinObj(win, 1, id);
    ctl2 = d2_GetWinObj(win, 1, id + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    b = 0;
    while (sv_RunFrame(ai_Unk593f40, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) == -1)
            continue;
        if (U_I((unsigned)&ai_Unk5fe710) != ctl1) {
            if (U_I((unsigned)&ai_Unk5fe710) == ctl2)
                U_I((unsigned)&sv_PopRequest) = 1;
            continue;
        }
        rec = U_I((unsigned)&ai_FallbackBuilding);
        b = he_FindEvent(3, 0, 0x48, 2, *(unsigned short *)(rec + 0x27), 3, U_I(rec + 1));
        while (b != 0) {
            if (strcmp((char *)(b + 0xac), name) == 0)
                break;
            b = he_NextEvent();
        }
        if (b == 0) {
            /* no burglar-guild building: the "0x48" message */
            memset(msg, 0, 0x80);
            *(unsigned char *)((char *)msg + 4) = 0x48;
            *(int *)((char *)msg + 8) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x87);
            *(int *)((char *)msg + 0xc) = U_I(rec + 1);
            *(int *)((char *)msg + 0x28) = U_I((unsigned)&ai_Time);
            *(int *)((char *)msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
            *(int *)((char *)msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
            *(unsigned short *)((char *)msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
            *(unsigned char *)((char *)msg + 0x36) = 2;
            strcpy((char *)msg + 0x58, name);
            snd_PlayFavourComment();
            for (i = 0, p = (int *)(unsigned)&ai_Unk7603a0; (int)p < (unsigned)&ai_Unk760420; p++) {
                if (*p != 0 && *(unsigned char *)(*p + 0x188) != 0) {
                    /* ROUGH: the entries' +4 go to the message argument list */
                    msg[0x16 + i] = *(int *)(*p + 4);
                    i++;
                }
            }
            cm_SendLongMessage(msg);
        }
        sv_Unk52ae40();
        U_I((unsigned)&sv_PopRequest) = 1;
    }
    d2_CloseView(win);
    return 1;
}
