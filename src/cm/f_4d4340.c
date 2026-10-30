/* 0x4d4340, the portrait row of the fight: for each of the 16 ids (-1 = none) a column (width = (0x6ed980 + 8 - 0x30) /
   (count + 1), 6 per row, rows of 100 pixels from x, y) with the frame 0x5da, the portrait, the health bar (0x504e00),
   the name and health text ("%1N1 %i%"), the building icon and the state icon (0x3b / 0x37) of the character */

void ai_Unk4d4340(int *ids, int x, int y)
{
    int i, n = 0;
    int w, px, py, v, t, f, ic;
    char *c, *o;
    char buf[0x100];

    for (i = 0; i < 16; i++) {
        if (ids[i] != -1)
            n++;
    }
    w = (*(short *)(d2_LastWindowPtrC + 8) - 0x30) / (n + 1);
    for (i = 0; i < 16; i++) {
        if (ids[i] == -1)
            continue;
        c = ai_Unk4ce130(ids[i]);
        px = (i % 6 + 1) * w + x;
        py = y + i / 6 * 100;
        d2_WinOpenPic(px - 0x10, py, 0x5da, d2_LastWindow);
        d2_WinOpenPic(px - 4, py, *(int *)(c + 0x18c), d2_LastWindow);
        v = gm_Unk504e00(c, 3);
        sv_Unk52eaa0(px, py + 0x3c, (int)(double)(float)v, 0x4eb);
        t = d2_CreateWinText(px - 0x24, py - 0x11, &ch_EmptyName, d2_LastWindow);
        G19_R(t, 0x14, short) = 0x78;
        G19_R(t, 0x5c, int) = 1;
        G19_R(t, 0x74, short) = 0x56;
        G19_5176D0(buf, "%1N1 %i%", *(unsigned short *)c, (int)(unk_Unk4fc8e0(c) * 100.0));
        d2_SetObjText(t, buf);
        o = ai_Unk4cda50(c);
        if (o != 0 && *(int *)o != 0) {
            ic = d2_WinOpenPic(px + 0x28, py + 0xf, *(int *)o + 0xdf, d2_LastWindow);
            d2_ScaleObject(ic, -2);
        }
        if (c[8] == 0) {
            f = (int)-*(float *)(c + 0x1c);
            if ((double)*(int *)(c + 0x24) == (double)f - 20.0)
                d2_WinOpenPic(px, py + 6, 0x3b, d2_LastWindow);
            else
                d2_WinOpenPic(px, py + 6, 0x37, d2_LastWindow);
        }
    }
}
