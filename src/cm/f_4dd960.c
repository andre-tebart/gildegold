/* 0x4dd960 */

/* 0x4dd960, the court's (and torture's) question dialogs of the player for a request m of kinds 0 .. 6 ("cutscenes\\prozess": a yes / no
   box, "cutscenes\\folterwahl": three choices); r +0x10 the asking character, +0x94 the answer, +0x98 a value. The bodies stand in the order
   0, 1, 2, 3, 6, 4, 5 (jump table 0x4de324). ROUGH */
#define CUT_BOX() d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow)

void ai_Unk4dd960(char *r, int *m)
{
    char tab[8] = {8, 10, 12, 15, 18, 21, 24, 0};
    int sel[7] = {-1, -1, -1, -1, -1, -1, -1};
    int btn[7];
    char *si, *di;
    int v, dlg, n, i, t, sum, k;

    *(int *)(r + 0x94) = -1;
    si = (char *)gm_FindChar(*(int *)(r + 0x10));
    di = (char *)gm_FindChar(m[6]);
    switch (m[0]) {
    case 0:
        if (*(int *)(r + 0x10) != m[6])
            return;
        *(float *)(r + 0x98) = (float)*(int *)(di + 0x24) + *(float *)(di + 0x1c);
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x14bc, *(unsigned short *)si);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 0;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 0;
                else if (d2_ClickedId == 0)
                    *(int *)(r + 0x94) = 1;
            }
        }
        d2_CloseView(v);
        return;
    case 1:
        if (*(int *)(r + 0x10) != m[5])
            return;
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x14cb, *(unsigned short *)si, *(unsigned short *)di);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 0;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 1;
                else if (d2_ClickedId == 0)
                    *(int *)(r + 0x94) = 0;
            }
        }
        d2_CloseView(v);
        return;
    case 2:
        if (*(int *)(r + 0x10) != m[5])
            return;
        gm_Shuffled(7, btn);
        sum = gm_GetWorth(*(unsigned short *)si) + gm_GetWorth(*(unsigned short *)di);
        sum /= 1000;
        v = d2_OpenForm(0, 0, "cutscenes\\folterwahl");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x14db, *(unsigned short *)si);
        for (i = 0; i < 3; i++) {
            k = btn[i];
            t = io_PrintText(0x14dc, k * 2 + 0xb20, (unsigned char)tab[k] * sum);
            sel[i] = d2_GetWinObj(v, 0, t);
        }
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = btn[ai_Unk4e52f0(3)];
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                for (i = 0; i < 3; i++)
                    if (ai_Unk5fe710 == sel[i])
                        break;
                if (i < 3)
                    *(int *)(r + 0x94) = btn[i];
                sv_PopRequest = 1;
            }
        }
        d2_CloseView(v);
        return;
    case 3:
        if (*(int *)(r + 0x10) != m[10])
            return;
        v = d2_OpenForm(0, 0, "cutscenes\\folterwahl");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x14e3, *(unsigned short *)si, *(unsigned short *)di, *(unsigned short *)di);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 0;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 0;
                else if (d2_ClickedId == 0) {
                    *(int *)(r + 0x94) = 1;
                    cm_SendRelations(*(int *)(si + 4), *(int *)(di + 4), 0x23, 1);
                }
            }
        }
        d2_CloseView(v);
        return;
    case 6:
        if (*(int *)(r + 0x10) != m[6])
            return;
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x1512, *(unsigned short *)si);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 0;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 1;
                else if (d2_ClickedId == 0)
                    *(int *)(r + 0x94) = 0;
            }
        }
        d2_CloseView(v);
        return;
    case 4:
        t = *(int *)(r + 0x10);
        if (t != m[7] && t != m[8] && t != m[9])
            return;
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x1529, *(unsigned short *)si);
        io_PrintText(0x152a);
        io_PrintText(0x152b);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 1;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 1;
                else if (d2_ClickedId == 0)
                    *(int *)(r + 0x94) = 0;
            }
        }
        d2_CloseView(v);
        return;
    case 5:
        if (*(int *)(r + 0x10) != m[7])
            return;
        sel[0] = sel[1] = sel[2] = -1;
        n = ai_Unk4945c0(0);
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x1532, *(unsigned short *)si);
        for (i = 0; i < 3; i++) {
            t = io_PrintText(0x1533, n * 3 + 0x1534 + i * 3);
            sel[i] = d2_GetWinObj(v, 0, t);
            if (unk_Unk4f4420(di, i + n) != 0)
                d2_SetObjEnabled(sel[i], 0);
        }
        if (sel[0] != -1)
            *(int *)(r + 0x94) = n + 1;
        else if (ai_Unk4e52f0(2) != 0)
            *(int *)(r + 0x94) = n + 2;
        else
            *(int *)(r + 0x94) = n;
        d2_ClickedId = -1;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (ai_Unk5fe710 == sel[0])
                    *(int *)(r + 0x94) = n;
                else if (ai_Unk5fe710 == sel[1])
                    *(int *)(r + 0x94) = n + 1;
                else if (ai_Unk5fe710 == sel[2])
                    *(int *)(r + 0x94) = n + 2;
            }
        }
        d2_CloseView(v);
        return;
    }
}
