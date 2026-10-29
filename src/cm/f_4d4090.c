/* 0x4d4090, the loot list of a won fight: for each of the 3 cart ids at +0x100 of ai_Unk6f0d04 the goods (mode 0 all,
   1 only those of the player, 2 only the others) get a cell of 5 per row (120 x 96 pixels from x, y): their picture
   (0x525 frame, 0xdf + id), the count "%i " plus a suffix, the value "%T"; returns the sum of the values */

int ai_Unk4d4090(int x, int y, int mode)
{
    int off;
    int j = 0;
    int total = 0;
    int px, py;
    int t, t2;
    char *q;
    char buf[0x100];

    for (off = 0x100; off < 0x10c; off += 4) {
        if (*(int *)(off + ai_Unk6f0d04) == -1)
            continue;
        q = gm_Query(0, 2, 7, 1, *(int *)(off + ai_Unk6f0d04));
        q = gm_Query(*(int *)(q + 0x14), 1, 0, 0x1dd);
        for (q = gm_Query(*(int *)(q + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {
            if (mode != 0 && !(mode == 1 && *(int *)(q + 0xa) == ai_PlayerId) &&
                !(mode == 2 && *(int *)(q + 0xa) != ai_PlayerId))
                continue;
            px = x + (j % 5) * 120;
            py = y + (j / 5) * 96;
            d2_WinOpenPic(px, py, 0x525, d2_LastWindow);
            d2_WinOpenPic(px, py, *(unsigned short *)q + 0xdf, d2_LastWindow);
            t = d2_CreateWinText(px - 0x10, py - 0xf, &ch_EmptyName, d2_LastWindow);
            G19_R(t, 0x74, short) = 0x56;
            G19_R(t, 0x14, short) = 0x50;
            G19_R(t, 0x5c, int) = 1;
            sprintf(buf, "%i ", *(int *)(q + 0xe));
            strcat(buf, ai_Unk144a160);
            d2_SetObjText(t, buf);
            t2 = d2_CreateWinText(px - 0x10, py + 0x34, &ch_EmptyName, d2_LastWindow);
            G19_R(t2, 0x74, short) = 0x56;
            G19_R(t2, 0x14, short) = 0x50;
            G19_R(t2, 0x5c, int) = 1;
            G19_5176D0(buf, "%T", (int)gm_GoodsPrice(*(unsigned short *)q, ai_OwnPlayer) * *(int *)(q + 0xe));
            d2_SetObjText(t2, buf);
            total = (int)(gm_GoodsPrice(*(unsigned short *)q, ai_OwnPlayer) * *(int *)(q + 0xe) + total);
            j++;
        }
    }
    return total;
}
