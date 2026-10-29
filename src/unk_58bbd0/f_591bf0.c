/* 0x591bf0, the tech tree window (1593 B, ROUGH SKELETON from the Ghidra pseudocode): "techtree\TechTree"
   (0x50c340), the window (0x5763c0 (0x200), the event 0x576040 (0x11), the controls 0x475b80 / 0x515960
   (0x17e2)); the tech record 0x4ffe90 (the building +0x5d, 1, 0, param_2) or (1, 4, 2), the item list
   0x7d6a40 / 0x76fe04 (0xd dwords each), the goods 0x91aaf0 + i * 0x41, the upgrade message 0x4c4540 /
   0x4c4580 ("upgrade"), the cost 0x5087f0 / 0x5049e0 / 0x5210a0 / 0x521040, the message record 0x4c4b70 and
   the confirmation 0x4c3ae0; the frame pump 0x5309a0 (this, 0x657c7); the exit 0x50c4d0 and 0x576040 (0x11) */

void ai_TechTreeWindow(int param_1, int param_2)
{
    int win;
    int rec;
    short *tr;
    int i;
    int it;
    int n;
    int all;
    int msg[0x3e];
    char name[0x80];
    char text[0x100];
    int u;
    int amount;

    if (param_1 == U_I((unsigned)&ai_PlayerBuilding)) {
        win = ai_Unk5763c0(0x200);
        if (win == 0)
            return;
        ai_Unk576040(0x11, param_1, 0, param_2);
    } else {
        win = 0;
    }
    tr = (short *)gm_Query(*(int *)(param_1 + 0x5d), 1, 0, param_2);
    if (tr == 0)
        tr = (short *)gm_Query(*(int *)(param_1 + 0x5d), 1, 4, 2);
    if (tr != 0) {
        i = gm_Unk50c340("techtree\\TechTree", param_1, tr);
        if (i < 0) {
            sv_MessageBox(U_I(((unsigned)&d2_Strings + 0x78)), 0);
            return;
        }
        d2_CentreViewWindows(i);
        d2_SetForm(i, 4);
        u = io_PrintText(0x17e2);
        i = d2_GetWinObj(i, 4, u);
        sv_SetResearchClass(param_1);
        it = sv_RunFrame(ai_TechTreeWindow, 0x657c7);
        while (it != 0) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0)
                U_I((unsigned)&sv_PopRequest) = 1;
            if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == i)
                ai_Unk5b7dd0(param_1);
            n = U_I((unsigned)&d2_ClickedId);
            if (n > 0xde && n < 0x3bc && U_I((unsigned)&ai_Unk7d6a40) > 0) {
                /* ROUGH: the item list (0x76fe04, 0xd dwords per entry) is scanned; the clicked item i = n - 0xdf;
                   the 0x3e-dword message local is zeroed, 0x576340 (0x2b, ...) checks the click, the goods record
                   0x91aaf0 + i * 0x41 gives the kind (6: the building upgrade with the cost 0x5087f0 / 0x5049e0 /
                   0x5210a0 / 0x521040, the message 0x5176d0 / 0x521150 / 0x4c4540 / 0x4c4580 / 0x4c4b70 and the
                   confirmation 0x4c3ae0; else 0x5176d0 / 0x521150 with the cost text 0x74) and 0x50b9f0
                   (the item window) */
            }
            it = sv_RunFrame(ai_TechTreeWindow, 0x657c7);
        }
        gm_Unk50c4d0();
        U_I((unsigned)&sv_ResearchClass) = 0;
        if (param_1 == U_I((unsigned)&ai_PlayerBuilding))
            ai_Unk576040(0x11, param_1, 1, param_2);
    }
}
