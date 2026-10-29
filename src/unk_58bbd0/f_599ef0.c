/* 0x599ef0, the "Handel\Handel_Stadtinfo" city window of a market stand (976 B): for a record whose first byte
   is 'G' it shows the object "dummy_Kartentisch_RECHTS_UNTEN" (0x100, flag 0x623814) and "ob_KARTENTISCH"
   (0x40); the pump 0x5309a0 (0x67fff) runs the window: "Handel\Handel_Stadtinfo" 0x4741f0, the rectangle
   0xf8f4b8 .. 0xf8f4c4 (0x18 / 0x18 / 0x1c / 0xb), the texts 0x1a11, 0x1a14 (the city name 0x7e3f40 +
   city * 0x2f4 and 0x7d6a60 + city * 0xaa4), the coat of arms "Wappen128_ns_Stadt_%s" (0x5de652 / 0x4721e0),
   the two groups 0x4ffe90 (param_1 + 0x5d, 2, 0, 0x1db / 0x1dc, 6) and the texts 0x1a12 / 0x1a13; each good of
   a group (0x4ffe90 / 0x4ffcc0, the character amount 0x4ffc40) becomes one row 0x599e00 in a 4-column grid
   (x = (i % 4) * 0x3c + 0xe, y = (i / 4) * 0x50 + 0x1e); the progress bar of the control +0x394
   (0xf8f120 + index * 0x3c4) is min((0x6333cc - tick) * 1/650, 0.35) */

void ai_Unk599ef0(char *param_1, int param_2)
{
    char text[0x104];
    int obj1;
    int obj2;
    int win = -1;
    int first = -1;
    int tick = 0;
    char *ctl = 0;
    int r;
    int g1;
    int g2;
    int n;
    int n4;
    int v;

    if (*param_1 != 'G')
        return;
    obj1 = d3_FindObject(0, 0x100, 0, "dummy_Kartentisch_RECHTS_UNTEN");
    if (obj1 != 0)
        d3t_PlaceAtObject(obj1, U_I((unsigned)&ai_CameraStep));
    obj2 = d3_FindObject(0, 0x40, 0, "ob_KARTENTISCH");
    if (sv_RunFrame(ai_Unk599ef0, 0x677c7) == 0)
        goto done;
    do {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (first != 1) {
            if (win != -1)
                d2_CloseView(win);
            win = d2_OpenForm(0, 0, "Handel\\Handel_Stadtinfo");
            d2_CentreViewWindows(win);
            d2_SetForm(win, 0);
            n = U_I((unsigned)&d2_LastWindow) * 0x3c4;
            U_I(((unsigned)&ai_Unkf8f39c + 0x11c) + n) = 0x18;
            U_I(((unsigned)&ai_Unkf8f39c + 0x120) + n) = 0x18;
            U_I(((unsigned)&ai_Unkf8f39c + 0x124) + n) = 0x1c;
            U_I(((unsigned)&ai_Unkf8f39c + 0x128) + n) = 0xb;
            d2_SetForm(win, 1);
            io_PrintText(0x1a11);
            d2_SetForm(win, 5);
            io_PrintText(0x1a14, (char *)(unsigned)&ai_PlayerSlots + U_I(param_1 + 0x65) * 0x2f4,
                         *(unsigned short *)((unsigned)&ai_Unk7d6a60 + U_I(param_1 + 0x65) * 0xaa4));
            n = U_I(((unsigned)&d2_Views + 0xc) + win * 0x2ac);
            ctl = (char *)((unsigned)&d2_Wins + n * 0x3c4);
            sprintf(text, "Wappen128_ns_Stadt_%s", (char *)(unsigned)&ai_PlayerSlots + U_I(param_1 + 0x65) * 0x2f4);
            d2_SetBackgroundTexture(text, U_I(ctl));
            tick = U_I((unsigned)&ai_Unk6333cc);
            U_I(((unsigned)&ai_Unkf8f39c + 0x118) + n * 0x3c4) = 0;
            g1 = gm_Query(U_I(param_1 + 0x5d), 2, 0, 0x1db, 6);
            if (g1 == 0)
                return;
            g2 = gm_Query(U_I(param_1 + 0x5d), 2, 0, 0x1dc, 6);
            if (g2 == 0)
                return;
            d2_SetForm(win, 3);
            io_PrintText(0x1a12);
            for (n = 0, v = (int)gm_Query(U_I(g1 + 0x14), 1, 5); v != 0;
                 v = (int)gm_QueryNext()) {
                r = gm_FindGoods(U_I(param_1 + 0x65), *(unsigned short *)v);
                if (r != 0 && U_I(r + 4) != 0) {
                    n4 = n % 4;
                    ai_Unk599e00(n4 * 0x3c + 0xe, (n / 4) * 0x50 + 0x1e, U_I((unsigned)&d2_LastWindow),
                                 *(unsigned short *)v, U_I(param_1 + 0x65));
                    n++;
                }
            }
            d2_SetForm(win, 4);
            io_PrintText(0x1a13);
            for (n = 0, v = (int)gm_Query(U_I(g2 + 0x14), 1, 5); v != 0;
                 v = (int)gm_QueryNext()) {
                r = gm_FindGoods(U_I(param_1 + 0x65), *(unsigned short *)v);
                if (r != 0 && U_I(r + 4) != 0) {
                    n4 = n % 4;
                    ai_Unk599e00(n4 * 0x3c + 0xe, (n / 4) * 0x50 + 0x1e, U_I((unsigned)&d2_LastWindow),
                                 *(unsigned short *)v, U_I(param_1 + 0x65));
                    n++;
                }
            }
            first = 1;
        }
        *(float *)(ctl + 0x394) = (float)(U_I((unsigned)&ai_Unk6333cc) - tick) * 0.0015384615384615385f;
        if (*(float *)(ctl + 0x394) >= 0.35f)
            *(float *)(ctl + 0x394) = 0.35f;
    } while (sv_RunFrame(ai_Unk599ef0, 0x677c7) != 0);
    if (win != -1)
        d2_CloseView(win);
done:
    if (obj2 != 0)
        d3t_PlaceAtObject(obj2, U_I((unsigned)&ai_CameraStep));
}
