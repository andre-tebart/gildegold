/* 0x598dc0, the regulars' table with the join button (1152 B): as 0x598b80 (the window
   "locations\Wirtshaus_Stammtisch", the four character slots param_2 + 0x1c, the records of 0xe ints redrawn by
   0x5be1d0 / 0x5be3b0); the mode local: 1 = the player sits at the table (0x1890), 2 = full (0x1892), 3 = free
   (0x189f / 0x188f + the join button), 4 = the player's own table elsewhere (0x1891); the join click 0 on the
   button posts the "stammtisch_join_H_Plr" message (0x4c4540 / 0x4c5d80 / 0x4c4580 / 0x4c3ae0) and the text
   0x189e / 0x189f; the pump 0x5309a0 (this, 0x677c7); the exit 0x4758f0 */
void ai_Unk4c5d80(int *p); void cm_BeginGroup(const char *s); void cm_EndGroup(const char *s); unsigned char * gm_BuildQuery(int n, ...);
unsigned char * gm_BuildNext(void);


void ai_Unk598dc0(int param_1, int param_2)
{
    int chars[0x38];
    int win;
    int n;
    int i;
    int changed;
    int c;
    int w;
    int margin;
    int mode;
    int ctl;
    int b;
    int it;
    int h;
    int *p;
    int *q;
    int msg[3];
    char text[0x200];

    memset(chars, 0, sizeof(chars));
    mode = -1;
    ctl = -1;
    win = d2_OpenForm(0, 0, "locations\\Wirtshaus_Stammtisch");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetPointerSprite(0, 0);
    do {
        n = 0;
        changed = 0;
        mode = 0;
        p = chars;
        q = (int *)(param_2 + 0x1c);
        for (i = 0; i < 4; i++, q++) {
            c = gm_FindChar(*q);
            if (c != *p)
                changed = 1;
            if (c != 0) {
                *p = c;
                n++;
                p += 0xe;
                if (*(int *)(c + 4) == U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87))
                    mode = 1;
            }
        }
        if (n == 4) {
            if (mode == 0)
                mode = 2;
        } else if (n < 4) {
            if (mode == 0) {
                mode = 3;
                for (it = gm_BuildQuery(1, 0, 0x37); it != 0; it = gm_BuildNext()) {
                    b = gm_Query(*(int *)(it + 0x5d), 2, 6, 0, 0x12d);
                    if (b != 0) {
                        for (i = 0; i < 4; i++) {
                            if (*(int *)(b + 0x1c + i * 4) ==
                                U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87)) {
                                mode = 4;
                                break;
                            }
                        }
                    }
                }
            }
            p = chars + n * 0xe;
            for (i = 4 - n; i != 0; i--, p += 0xe)
                *p = 0;
        }
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            io_PrintText(0x188c, param_1);
            if (n == 0) {
                io_PrintText(0x188e);
            } else {
                margin = *(short *)((unsigned)&d2_Pics + 0x1ebd8);
                w = (int)*(short *)((unsigned)&d2_LastWindowPtr + 8) + margin * -2;
                for (i = 0; i < n; i++) {
                    p = chars + i * 0xe;
                    p[0xc] = 1;
                    p[4] = ((i % 2) + 1) * (w / 3) - 10 + (w % 3) / 2 + (margin + 10) * (i % 2);
                    p[5] = (i / 2) * 0x82 + 100;
                    ai_Unk5be1d0(p);
                    ai_Unk5be3b0(p[4], p[5], U_I((unsigned)&d2_LastWindow), 0x54, p, 1);
                }
            }
            d2_SetForm(win, 2);
            d2_RemoveWindowObjects(U_I((unsigned)&d2_LastWindow), 1);
            io_PrintText((char *)(unsigned)"$C");
            ctl = -1;
            if (mode == 2) {
                io_PrintText(0x1892);
            } else if (mode == 4) {
                io_PrintText(0x1891);
            } else if (mode == 3) {
                ctl = d2_GetWinObj(win, 2, io_PrintText(0x188f));
            } else if (mode == 1) {
                ctl = d2_GetWinObj(win, 2, io_PrintText(0x1890));
            }
        }
        if (U_I(((unsigned)&dm_L + 0x20)) == 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == ctl && mode == 3) {
                memset(text, 0, 0x200);
                if (U_I((unsigned)&d2_ClickedId) == 0) {
                    msg[0] = 0x6a6f696e;
                    msg[1] = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                    msg[2] = U_I(param_1 + 1);
                    cm_BeginGroup("stammtisch_join_H_Plr");
                    h = ((int (*)(int))ai_Unk4c5d80)(msg);
                    cm_EndGroup("stammtisch_join_H_Plr");
                    b = cm_GetSlotState(h);
                    while (b == 0) {
                        sv_RunFrame(0, 0x20007);
                        b = cm_GetSlotState(h);
                    }
                    txt_Format(text, b == 1 ? 0x189e : 0x189f, param_1);
                }
                if (text[0] != 0)
                    sv_MessageBox(text, 0);
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
    } while (sv_RunFrame(ai_Unk598dc0, 0x677c7) != 0);
    d2_CloseView(win);
}
