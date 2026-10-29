/* 0x584b70, the graveyard "Gehilfen erschaffen" window (501 B, a start Ghidra missed: the callback is reached
   only through a table; the ledger found the dword 0x584d3b): o / ev the event record (its +0 a pointer to a
   dword, +8 a byte, +0xc a pointer to a dword, +0x10 a dword). 0x4f1d50 (o, &n) counts the graveyard's living
   characters: 1 (below the limits) -> the window "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 /
   0x475600, the sound 0x52ea30, the texts 0x4132 (0x4131, 0x229) and 0x4133 / +1 in 0x475b80 (window, 2,
   text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1;
   the first entry: when *(int *)*(ev + 0xc) == -1 -> 0x6f22a0 = 1; else 0x4f1170 (o, *(int *)*(int *)ev,
   ev[8], *(int *)(ev + 0x10), 0x1c) raises the dead, its answer waited on 0x4c3ae0 (pump 0x20007) and
   0x4c3b10 (h) gives the helper, its value +4 the line "sp_%u" (0x62a62c), then the script
   "LOCATIONS\\FRIEDHOF\\Gehilfen_erschaffen_FRIEDHOF.esc" (0x62a5f8) run with the one argument (0x4287b0 /
   0x429ab0) and 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the result; 0 (the limit
   reached) -> the text 0x4135 (n, 0x229, o) through 0x5176d0 and 0x521150 (buf, 0), 0; -1 -> 0 */

int unk_Unk4f1d50(unsigned char *o, int *n); int unk_Unk4f1170(); int cm_GetSlotValue(int h); int sprintf(char *buf, const char *fmt, ...);

int ai_Unk584b70(char *o, int *ev)
{
    char buf[0x200];
    char line[0x40];
    int win;
    int b1;
    int b2;
    int n;
    int r;
    int v;
    int h;
    char *a;

    r = 0;
    v = unk_Unk4f1d50(o, &n);
    if (v == 1) {
        win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        sv_FitForm(U_I((unsigned)&d2_LastWindow));
        d2_SetForm(win, 1);
        io_PrintText(0x4132, 0x4131, 0x229);
        d2_SetForm(win, 2);
        h = io_PrintText(0x4133);
        b1 = d2_GetWinObj(win, 2, h);
        b2 = d2_GetWinObj(win, 2, h + 1);
        U_I((unsigned)&d2_ClickedId) = -1;
        for (;;) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                    if (*(int *)*(int *)((char *)ev + 0xc) == -1) {
                        U_I((unsigned)&sv_PopRequest) = 1;
                    } else {
                        h = unk_Unk4f1170(o, *(int *)*(int *)ev, *(unsigned char *)((char *)ev + 8), *(int *)((char *)ev + 0x10), 0x1c);
                        if (h != -1) {
                            while (cm_GetSlotState(h) == 0)
                                sv_RunFrame(0, 0x20007);
                        }
                        a = (char *)cm_GetSlotValue(h);
                        if (a != 0) {
                            sprintf(line, (char *)(unsigned)&full_data_62a62c, *(int *)(a + 4));
                            evt_RunScriptArgs(evt_Load((char *)(unsigned)&full_data_62a5f8), 1, line);
                        }
                        r = 1;
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
            if (sv_RunFrame(ai_Unk584b70, 0x677c7) == 0) {
                d2_CloseView(win);
                return r;
            }
        }
    }
    if (v == 0) {
        txt_Format(buf, (char *)0x4135, n, 0x229, o);
        sv_MessageBox(buf, 0);
    }
    return 0;
}
