/* 0x598b80, the regulars' table window "locations\Wirtshaus_Stammtisch" (563 B): the window 0x4741f0 /
   0x475600 / 0x475f70 (0) / 0x52ea30 / 0x476f80; per frame the four character slots param_2 + 0x1c (0x505df0)
   are compacted into the 4 records of 0xe ints: a change redraws (0x475f70 (1), 0x472d20, 0x515960 (0x188c,
   param_1, and 0x188e when empty)) and places each record with 0x5be1d0 / 0x5be3b0 (x = ((i % 2) + 1) *
   (width / 3) - 10 + (width % 3) / 2 + (the margin + 10) * (i % 2), y = (i / 2) * 0x82 + 100, font 0x54);
   0x1892 when four sit at the table; the pump 0x5309a0 (this, 0x677c7); the exit 0x4758f0 */

void ai_Unk598b80(int param_1, int param_2)
{
    int chars[0x38];
    int win;
    int n;
    int i;
    int changed;
    int c;
    int w;
    int *p;
    int *q;
    int margin;

    memset(chars, 0, sizeof(chars));
    win = d2_OpenForm(0, 0, "locations\\Wirtshaus_Stammtisch");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetPointerSprite(0, 0);
    do {
        n = 0;
        changed = 0;
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
            }
        }
        if (n < 4) {
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
            if (n == 4)
                io_PrintText(0x1892);
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    } while (sv_RunFrame(ai_Unk598b80, 0x677c7) != 0);
    d2_CloseView(win);
}
