/* 0x4d4540, the dialog "cutscenes\combat_start" that ends a fight (a: the fight record; its +0x7c bits 2 / 1 / 4 pick the
   text): the line of the winner (texts 0x1138 .. 0x113c, 0x1125 + role), the loot list 0x4d4090,
   the portrait row 0x4d4340, and the buttons: the player picks the share 10 / 50 / 90 (chance share <= random 10 .. 99)
   or the AI side picks at random; the loot goes through 0x4d3ea0, the result is 0x6f0cfc (-1: refused), 0x6f0cf8 = 1 when
   the loot was taken. Returns 1 when the fight goes on. Rough draft. */

int ai_Unk4d4540(char *a)
{
    int res = 1;
    int st = 0;
    int win, ca, cb, role, edi, i, n, r, v, sel, esi, total;
    int b1, b2, b3, b4;
    float fv;
    char *p;
    int rc[0x2e];
    char tb[0x3f4];
    char tc[0x3f0];

    win = d2_OpenForm(0, 0, "cutscenes\\combat_start");
    ((void (*)(int, int))d2_PlaceMainWindow)(win, 3);
    ai_Unk6f0cf8 = 0;
    ca = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x14));
    cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
    role = ai_Unk4d4050((unsigned char *)cb);
    d2_SetForm(win, 1);
    if (*(char *)(ai_Unk6f0cec + 2) == 6) {
        if (a[0x7c] & 2) {
            io_PrintText(0x1138, role + 0x1125, cb, *(unsigned short *)ai_Unk6f0ce8, ca);
            d2_SetForm(win, 2);
            edi = io_PrintText(0x113a, *(unsigned short *)ai_Unk6f0ce8, ca);
        } else if (a[0x7c] & 1) {
            edi = st;
            for (i = 0; i < 3; i++) {
                if (*(int *)(a + 0x100 + i * 4) != -1)
                    edi++;
            }
            st = edi;
            fv = gm_GetSkill(ai_Unk6f0cec, 3);
            io_PrintText(0x112e, cb, *(unsigned short *)ai_Unk6f0ce8);
            d2_SetForm(win, 2);
            io_PrintText(0x112f, edi, 0xc10);
            if (fv >= 0.5) {
                if (*(int *)(a + 0x110) != 0)
                    io_PrintText(0x1134);
                else
                    io_PrintText(0x1135);
            }
            if (fv >= 0.75) {
                v = gm_Unk504e00(ai_Unk6f0ce8, 3);
                r = (int)(v * 0.023809524f);
                if (r > 4)
                    r = 4;
                r = r < 0 ? 0 : r;
                io_PrintText(0x1131, r + 0x1128);
            }
            if (*(char *)(ai_Unk6f0ce8 + 9) == 0)
                edi = io_PrintText(0x1133, 0x1136);
            else
                edi = io_PrintText(0x1133, 0x1137);
        } else if (a[0x7c] & 4) {
            io_PrintText(0x113b, cb, *(unsigned short *)ai_Unk6f0ce8);
            d2_SetForm(win, 2);
            edi = io_PrintText(0x113c, *(unsigned short *)ai_Unk6f0ce8, ca);
        }
        d2_SetForm(win, 1);
        io_PrintText(0x1132);
        ai_Unk4d4340((int *)(a + 0xc0), 0, 0xb4);
        d2_GetWinObj(win, 2, edi);
        edi++;
        d2_GetWinObj(win, 2, edi);
        while (sv_RunFrame(ai_Unk4d4540, 0x7d6c6)) {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                ai_Unk6f0cf8 = 1;
                res = 0;
            }
            if (d2_ClickedId == 0) {
                res = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1) {
                ai_Unk6f0cf8 = 1;
                res = 0;
                sv_PopRequest = 1;
            }
        }
    }
    if ((a[0x7c] & 1) && *(char *)(ai_Unk6f0ce8 + 2) == 6 && ai_Unk6f0cf8 == 0) {
        memset(rc, 0, sizeof(rc));
        total = 0;
        for (i = 0; i < 3; i++) {
            if (*(int *)(a + 0x100 + i * 4) != -1)
                st++;
        }
        p = gm_Query(0, 2, 7, 1, *(int *)(a + 0x100));
        p = *(char **)(p + 0x24);
        d2_SetForm(win, 1);
        io_PrintText("$C");
        if (p != 0) {
            p = gm_FindBuildingById(*(int *)(p + 0xb4));
            esi = 0;
            for (i = 0; i < 16; i++) {
                if (*(int *)(a + 0xc0 + i * 4) != -1)
                    esi++;
            }
            if (p != 0)
                io_PrintText(0x113d, st, 0xc10, p, *(unsigned short *)ai_Unk6f0cec, esi);
        }
        v = gm_Unk504e00(ai_Unk6f0ce8, 3);
        r = (int)(v * 0.023809524f);
        if (r > 4)
            r = 4;
        if (*(int *)(a + 0x110) != 0)
            io_PrintText(0x113e, (r < 0 ? 0 : r) + 0x1128);
        else
            io_PrintText(0x113f);
        ai_Unk4d4090(0x10, 0x96, 0);
        for (i = 0; i < 3; i++) {
            n = *(int *)(a + 0x100 + i * 4);
            if (n != -1) {
                p = (char *)gm_FindObjectInst(n);
                if (p != 0) {
                    gm_StallGoodsList(p, rc);
                    for (esi = 0; esi < rc[1]; esi++)
                        total = (int)(gm_GoodsPrice(*(unsigned short *)((char *)rc + 0x48 + esi * 2), ai_OwnPlayer) * rc[0x1c + esi] + total);
                }
            }
        }
        d2_SetForm(win, 2);
        io_PrintText("$C");
        esi = io_PrintText(0x1141, (int)(v * 0.1), (int)(v * 0.5), (int)(v * 0.8));
        b1 = d2_GetWinObj(win, 2, esi);
        b2 = d2_GetWinObj(win, 2, esi + 1);
        b3 = d2_GetWinObj(win, 2, esi + 2);
        edi = 0;
        for (i = 0; i < 16; i++) {
            if (*(int *)(a + 0x80 + i * 4) != -1)
                edi++;
        }
        b4 = d2_GetWinObj(win, 2, esi + 3);
        if (edi == 0)
            d2_EnableObject(b4, 0);
        while (sv_RunFrame(ai_Unk4d4540, 0x7d6c6)) {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                res = 0;
            }
            if (d2_ClickedId == -1)
                continue;
            sel = ai_Unk5fe710;
            if (sel == b4) {
                sv_PopRequest = 1;
                res = 1;
            } else if (sel == b1 || sel == b2 || sel == b3) {
                st = 0;
                if (sel == b1)
                    st = 10;
                else if (sel == b2)
                    st = 50;
                else if (sel == b3)
                    st = 90;
                if ((unsigned short)util_RandomU(0x5a) + 10 > st) {
                    txt_Format(tc, 0x1143);
                    ai_Unk4e5470(tc, 0);
                    sv_PopRequest = 1;
                    res = 1;
                } else {
                    cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                    txt_Format(tb, 0x1142);
                    ai_Unk4e5470(tb, 0);
                    fv = (float)(st * 0.01);
                    sv_PopRequest = 1;
                    b4 = 0;
                    ai_Unk6f0cf8 = 1;
                    ai_Unk6f0cfc = ai_Unk4d3ea0(ai_Unk6f0d04, cb, ai_Unk6f0cec, ai_Unk6f0ce8, fv);
                }
            }
        }
    } else if ((a[0x7c] & 1) && *(char *)(ai_Unk6f0ce8 + 2) != 6 && ai_Unk6f0cf8 == 0) {
        n = 0;
        for (i = 0; i < 16; i++) {
            if (*(int *)(a + 0x80 + i * 4) != -1)
                n++;
        }
        if (n == 0 || (unsigned short)util_RandomU(5) > *(int *)(a + 0x110)) {
            st = 0;
            r = (unsigned short)util_RandomU(4);
            if (r == 0)
                st = 10;
            else if (r == 1)
                st = 90;
            else if (r >= 2)
                st = 50;
            if ((unsigned short)util_RandomU(0x5a) + 10 > st) {
                ai_Unk6f0cfc = -1;
                res = 1;
            } else {
                cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                fv = (float)(st * 0.01);
                ai_Unk6f0cf8 = 1;
                ai_Unk6f0cfc = ai_Unk4d3ea0(ai_Unk6f0d04, cb, ai_Unk6f0cec, ai_Unk6f0ce8, fv);
            }
        }
    }
    if (win != -1)
        d2_CloseView(win);
    return res;
}
