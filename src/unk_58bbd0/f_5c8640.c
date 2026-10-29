/* 0x5c8640, the privileges panel "panel_privilegien" 0x62d480 (1403 B, ROUGH SKELETON): the flag byte
   0x770d31 + 0x6f25b8 * 0x21c (already open -> 0x4f8200 (0x144153c, 0, 0x657c7)); the gate 0x5763c0 (2) and
   the state step 0x576040 (8, 0, 0, 0); the state array arr[0x88] (arr[0] = 1) and the 0x3a entry values
   cur[] (1) compared with 0x4f81c0 (i, 0x770b80 + 0x6f25b8 * 0x21c, arr); the refresh every 500 clock ticks
   (0x6333cc); the open char local_404 from the carrier walk 0x53e400 (1, 0, 0x45) / 0x505df0 (record +0xb0)
   against the character 0x770b84 + 0x6f25b8 * 0x21c (record +0xac); the pages 0x1cdd / 0x1cde / 0x1cdc /
   0x1cdb via 0x515960 and the buttons 0x475b80 / 0x475590; the entry controls 0xf8f080 + id * 0x2ec (+0x3c /
   +0x50 = 1) and the value 9 special case; the message 0x472270 (-1, -1, screen, screen, 0x767) when ten
   entries are shown; the pump 0x5309a0 and the end 0x4758f0 / 0x576040 (8, 0, 1, 0). ROUGH: the pages and
   the click / redraw split follow the pseudocode; the string globals 0x625150 / 0x602568 / 0x60c6c0. */

int ord_PrivilegeDialog();

void ai_Unk5c8640(void)
{
    int arr[0x88];
    char title[520];
    int save0;
    int save1;
    int open;
    int win;
    int changed;
    int selected;
    int page;
    int ids[0x3a];
    unsigned int cur[0x3a];
    int i;
    int k;
    int v;
    int *p;
    unsigned int last;

    save0 = *(int *)(unsigned)&full_data_625150;
    save1 = *(int *)(unsigned)&full_data_625154;
    selected = -1;
    page = 2;
    open = 0;
    if (*(char *)(((unsigned)&ai_Selected + 0x29) + *(int *)(unsigned)&ai_PlayerId * 0x21c) != 0) {
        ord_PrivilegeDialog(*(int *)((unsigned)&ai_MessageFilterTexts + 0x3c), 0, 0x657c7);
        return;
    }
    if (!ai_Unk5763c0(2))
        return;
    ai_Unk576040(8, 0, 0, 0);
    for (p = arr, i = 0; i < 0x88; i++)
        *p++ = 0;
    arr[0] = 1;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62d480);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    changed = 1;
    for (i = 0; i < 0x3a; i++)
        cur[i] = 1;
    *(int *)(unsigned)&d2_ClickedId = -1;
    last = *(unsigned int *)(unsigned)&ai_Unk6333cc;
    do {
        int old;

        old = open;
        selected = 0;
        if (last <= *(unsigned int *)(unsigned)&ai_Unk6333cc || changed) {
            last = *(unsigned int *)(unsigned)&ai_Unk6333cc + 500;
            if ((*(unsigned int *)(((unsigned)&ai_Selected + 0x40) + *(int *)(unsigned)&ai_PlayerId * 0x21c) & 0x100) != 0 || open != 0) {
                p = (int *)he_FindEvent(1, 0, 0x45);
                while (p != 0) {
                    if (*(int *)((char *)p + 0xac) == *(int *)((unsigned)&ai_CharInts + *(int *)(unsigned)&ai_PlayerId * 0x21c)) {
                        open = (int)gm_FindChar(*(int *)((char *)p + 0xb0));
                        goto found;
                    }
                    p = (int *)he_NextEvent();
                }
                open = 0;
            }
found:
            if (open == old && (open != 0 || (arr[0] != 2 && selected == -1))) {
                arr[0] = (page == 2) ? 1 : 2;
            } else {
                changed = 1;
                page = 2;
                arr[0] = 1;
            }
            for (i = 0; i < 0x3a; i++) {
                v = ord_DispatchOrder(i, (char *)((unsigned)&ai_Chars + *(int *)(unsigned)&ai_PlayerId * 0x21c), arr);
                if ((unsigned int)v != cur[i])
                    changed = 1;
                cur[i] = v;
            }
            if (changed) {
                changed = 0;
                d2_SetForm(win, 2);
                io_PrintText((char *)(unsigned)"$C");
                selected = -1;
                *(int *)(((unsigned)&ai_Unkf8f13c + 0x228) + *(int *)(unsigned)&d2_LastWindow * 0x3c4) = 0;
                *(int *)(((unsigned)&ai_Unkf8f13c + 0x22c) + *(int *)(unsigned)&d2_LastWindow * 0x3c4) = 0;
                if (page == 2) {
                    d2_SetForm(win, 1);
                    io_PrintText(0x1cdd);
                    if (open != 0) {
                        d2_SetForm(win, 3);
                        v = io_PrintText(0x1cdc, *(short *)open);
                        selected = d2_GetWinObj(win, 3, v);
                    }
                } else if (page == 10) {
                    d2_SetForm(win, 1);
                    if (open == 0) {
                        io_PrintText(0x1cdd);
                        page = 2;
                    } else {
                        io_PrintText(0x1cde);
                        d2_SetForm(win, 3);
                        v = io_PrintText(0x1cdb, *(short *)open);
                        selected = d2_GetWinObj(win, 3, v);
                    }
                }
                d2_SetForm(win, 2);
                arr[0] = (page != 2) + 1;
                i = 0;
                do {
                    ids[i] = -1;
                    v = ord_DispatchOrder(i, (char *)((unsigned)&ai_Chars + *(int *)(unsigned)&ai_PlayerId * 0x21c), arr);
                    cur[i] = v;
                    if (v == page) {
                        if (selected != 0)
                            io_PrintText((char *)(unsigned)&ai_Unk60c6c0);
                        v = io_PrintText(&save0, title);
                        k = d2_GetWinObj(win, 2, v);
                        ids[i] = k;
                        selected++;
                    } else if (page == 2 && v == 9) {
                        if (selected != 0)
                            io_PrintText((char *)(unsigned)&ai_Unk60c6c0);
                        v = io_PrintText(&save0, title);
                        k = d2_GetWinObj(win, 2, v);
                        ids[i] = -1;
                        *(int *)((unsigned)&d2_Objs + k * 0x2ec + 0x3c) = 1;
                        *(int *)((unsigned)&d2_Objs + k * 0x2ec + 0x50) = 1;
                        selected++;
                    }
                    i++;
                } while (i < 0x3a);
                if (selected < 10) {
                    d2_SetForm(win, 4);
                    io_PrintText((char *)(unsigned)"$C");
                } else {
                    d2_SetForm(win, 2);
                    k = *(int *)(unsigned)&d2_LastWindow;
                    d2_SetForm(win, 4);
                    io_PrintText((char *)(unsigned)"$C");
                    d2_CreateScrollButtons(-1, -1, k, k, 0x767);
                }
                selected = open;
            }
        }
        if (*(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&ai_Unk5fe710 == selected) {
                changed = 1;
                page = (page == 2) ? 10 : 2;
            } else {
                for (i = 0; i < 0x3a; i++) {
                    if (*(int *)(unsigned)&ai_Unk5fe710 == ids[i]) {
                        d2_SetViewEnabled(win, 0);
                        arr[0] = (page != 2) + 3;
                        v = ord_DispatchOrder(i, (char *)((unsigned)&ai_Chars + *(int *)(unsigned)&ai_PlayerId * 0x21c), arr);
                        if ((char)v < 0) {
                            changed = 1;
                            cur[i] = v & 10;
                        }
                        arr[0] = (page != 2) + 1;
                        d2_SetViewEnabled(win, 1);
                        break;
                    }
                }
            }
        }
        if (*(int *)((unsigned)&dm_L + 0x20))
            *(int *)(unsigned)&sv_PopRequest = 1;
    } while (sv_RunFrame(ai_Unk5c8640, 0x657c7) != 0);
    d2_CloseView(win);
    ai_Unk576040(8, 0, 1, 0);
}
