/* 0x4d4d70, the end of a fight (a: the fight record, its +0x7c bits 1 / 2 / 4 say how it was fought): the dialog
   "cutscenes\combat_end" with the winner's text (0x1189 .. 0x118f, 0x1191, 0x1193, 0x1197), the portrait rows of both
   sides (0x4d4340), then by the kind of the fight: the loot of the carts (bit 1: money 0x4d4090, goods moved to the
   winner), the destruction of a building (bit 4: the 0x5db cells with the bars, money by 0x4c47e0), or the capture
   of the defeated character (bit 2: texts 0x119f .. 0x11a7 pick one of four outcomes: 1 the building is taken,
   2 a share of money and goods is taken, 3 the winner loses face, 0 the guards of the town are called). Rough draft. */

#define G20_US(p) (*(unsigned short *)(p))
#define G20_NAME(id) (((char **)ai_Unk143c860)[(id) * 2])
#define G20_ME (ai_CharInts[ai_PlayerId][0])
#define G20_ATT ((char *)ai_Unk6f0ce8)
#define G20_DEF ((char *)ai_Unk6f0cec)
/* the pump of the dialog: the click on a button (f8a668 == 0) or the abort flag closes it */
#define G20_PUMP()                                                                                      \
    while (sv_RunFrame(ai_Unk4d4d70, 0x7d6c6)) {                                                       \
        if (dm_L.rup != 0 || d2_ClickedId == 0)                                                    \
            sv_PopRequest = 1;                                                                           \
    }
/* the winner takes a share of the loser's money (message 0x4c47e0) and of the goods of ob (80 .. 99 %), the text lines
   go to ba; the money part v is added to the winner's record afterwards (0x506430, +0x4d of the building) */
#define G20_TAKE()                                                                                      \
    ba[0] = ch_EmptyName;                                                                               \
    memset(ba + 1, 0, 0x3ff);                                                                           \
    txt_Format(ba, 0x11a5, G20_US(G20_DEF), ca);                                                      \
    ai_Unk4e5470(ba, 0);                                                                                \
    ob = gm_Query(*(int *)(ca + 0x5d), 2, 6, 0, 0x2a);                                              \
    if (ob == 0)                                                                                        \
        ob = gm_Query(*(int *)(ca + 0x5d), 2, 6, 0, 0x116);                                         \
    v = gm_GetGoodsValue(ai_Unk6f0ce8);                                                                     \
    v = v < 0x9c400 ? v : 0x9c400;                                                                      \
    v = (int)(((double)(unsigned int)ai_Unk4e52f0(0x32) * 0.001 + 0.07) * (v < 0 ? 0 : v));             \
    txt_Format(ba, "$Z%T$A", v);                                                                      \
    cm_SendMoveGoods(*(int *)(G20_DEF + 4), *(int *)(G20_ATT + 4), ai_OwnPlayer, v);                        \
    if (ob != 0 && obj != 0) {                                                                          \
        for (q = gm_Query(*(int *)(ob + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {         \
            cnt = (int)((double)(unsigned int)(ai_Unk4e52f0(0x14) + 0x50) * 0.01 * *(int *)(q + 0xe));  \
            if (cnt < 1)                                                                                \
                cnt = 1;                                                                                \
            sprintf(tmp, "%i %s$A", cnt, G20_NAME(G20_US(q)));                                          \
            strcat(ba, tmp);                                                                            \
            if (G20_ME == ai_Unk60cb5c)                                                                 \
                G19_4C4880U(*(int *)(obj + 2), *(int *)(ob + 2), G20_US(q), cnt, ai_OwnPlayer, 0);      \
            v = (int)(gm_GoodsPrice(G20_US(q), ai_OwnPlayer) * cnt + v);                                 \
        }                                                                                               \
    }                                                                                                   \
    d2_SetForm(win, 1);                                                                                 \
    io_PrintText(ba);                                                                                   \
    q = gm_PlayerRecord(G20_DEF);                                                                          \
    if (q != 0)                                                                                         \
        *(int *)(q + 0x1c) += v;                                                                        \
    if (*(int *)(a + 0x10c) != -1) {                                                                    \
        q = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));                                                    \
        *(int *)(q + 0x4d) += v;                                                                        \
    }
/* the winner loses face: the text 0x11a6 and a drop of 0x14 - random of the defender's mood (0x50cad0) */
#define G20_FACE()                                                                                      \
    ba[0] = ch_EmptyName;                                                                               \
    memset(ba + 1, 0, 0x3ff);                                                                           \
    txt_Format(ba, 0x11a6, G20_US(G20_DEF), ca);                                                      \
    ai_Unk4e5470(ba, 0);                                                                                \
    if (G20_ME == ai_Unk60cb5c)                                                                         \
        gm_Unk50cad0(ca, 0x14 - ai_Unk4e52f0(0x14));
/* nothing taken: the text 0x11a7 and the fight with the tavern keepers' men: every character of the table of 0x770b80
   that is a guard (kinds 10 .. 12) gets 0x4c4b00 */
#define G20_GUARDS()                                                                                    \
    txt_Format(bb, 0x11a7, G20_US(G20_DEF), G20_US(G20_DEF));                                         \
    ai_Unk4e5470(bb, 0);                                                                                \
    for (p = (char *)ai_CharInts - 4; p < (char *)(unsigned)((unsigned)ai_CharInts + 0x653fc); p += 0x21c) {                                  \
        if (*(short *)p != -1 && p[8] != 0 &&                                                           \
            ((k = gm_TitleGroup((unsigned char)p[0x164])) == 10 || k == 11 || k == 12))                  \
            cm_SendRelations(*(int *)(G20_DEF + 4), *(int *)(p + 4), 6, 1);                                 \
    }

void ai_Unk4d4d70(char *a)
{
    char ba[0x400];
    char bb[0x400];
    char tmp[0x100];
    char *obj, *side, *c, *ca, *ob, *q, *p, *kr;
    int win, i, n, role = 0, v, cnt, total, na, nb, id, k;
    int h, row, col, px, py;

    d2_CopyCaption(&ch_EmptyName);
    sv_SetStatusText(&ch_EmptyName);
    obj = 0;
    if (*(int *)(a + 0x10c) != -1) {
        c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
        obj = gm_Query(*(int *)(c + 0x5d), 2, 6, 0, 0x116);
        if (obj == 0)
            obj = gm_Query(*(int *)(c + 0x5d), 2, 6, 0, 0x2a);
    }
    if (ai_Unk60cb5c == G20_ME) {
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0x80 + i * 4);
            if (id != -1 && (c = gm_FindChar(id)) != 0)
                ai_Unk4cdb80(c);
        }
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0xc0 + i * 4);
            if (id != -1 && (c = gm_FindChar(id)) != 0)
                ai_Unk4cdb80(c);
        }
    }
    if (ai_Unk6f0cf4 == 0) {
        na = 0;
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0x80 + i * 4);
            if (id != -1) {
                c = ai_Unk4ce130(id);
                if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                    na++;
            }
        }
        nb = 0;
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0xc0 + i * 4);
            if (id != -1) {
                c = ai_Unk4ce130(id);
                if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                    nb++;
            }
        }
        if (nb > na)
            side = G20_DEF;
        else
            side = G20_ATT;
    } else {
        side = G20_ATT;
        if (ai_Unk6f0cf4 != ai_Unk6f0cec)
            side = G20_DEF;
    }
    win = d2_OpenForm(0, 0, "cutscenes\\combat_end");
    ((void (*)(int, int))d2_PlaceMainWindow)(win, 3);
    d2_SetForm(win, 2);
    io_PrintText(0x7d);
    d2_SetForm(win, 1);
    if (G20_ME == *(int *)(side + 4)) {
        if (side == G20_ATT) {
            if (a[0x7c] & 1) {
                io_PrintText(0x1189, G20_US(G20_ATT), G20_US(G20_DEF));
                io_PrintText(0x118a);
            } else
                io_PrintText(0x1189, G20_US(G20_ATT), G20_US(G20_DEF));
        } else
            io_PrintText(0x118e, G20_US(G20_DEF), G20_US(G20_ATT));
    } else if (side == G20_DEF) {
        io_PrintText(0x118c, G20_US(G20_ATT), G20_US(G20_DEF));
        if (a[0x7c] & 1) {
            q = gm_Query(0, 2, 7, 1, *(int *)(a + 0x100));
            if (*(int *)(q + 0x24) != 0 && gm_FindBuildingById(*(int *)(*(char **)(q + 0x24) + 0xb4)) != 0)
                io_PrintText(0x118d);
        }
    } else
        io_PrintText(0x118f, G20_US(G20_DEF), G20_US(G20_ATT));
    G20_PUMP();
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1191);
    c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
    kr = ai_BTypes + (unsigned char)c[0] * 0x24d;
    if (*kr == 0x13)
        role = 0;
    else if (*kr == 0x10)
        role = 1;
    else if (*kr == 4)
        role = 2;
    io_PrintText(0x1193, G20_US(G20_DEF), role + 0x1125);
    ai_Unk4d4340((int *)(a + 0xc0), 0, 100);
    ca = gm_BuildQuery(1, 1, *(int *)(a + 0x14));
    if (ca != 0) {
        kr = ai_BTypes + (unsigned char)ca[0] * 0x24d;
        if (*kr == 0x13)
            role = 0;
        else if (*kr == 0x10)
            role = 1;
        else if (*kr == 4)
            role = 2;
    } else
        role = 0;
    io_PrintText("$7A");
    io_PrintText(0x1193, G20_US(G20_ATT), role + 0x1125);
    ai_Unk4d4340((int *)(a + 0x80), 0, 0x12c);
    G20_PUMP();
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1197);
    if (a[0x7c] & 1) {
        if (G20_US(&ai_PlayerId) == G20_US(G20_ATT))
            ai_Unk4d4090(0x24, 0x32, 2);
        else if (G20_US(&ai_PlayerId) == G20_US(G20_DEF)) {
            v = ai_Unk4d4090(0x24, 0x32, side == G20_DEF ? 0 : 1);
            q = gm_PlayerRecord(G20_DEF);
            if (q != 0)
                *(int *)(q + 0x1c) += v;
            if (*(int *)(a + 0x10c) != -1) {
                q = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                *(int *)(q + 0x4d) += v;
            }
        }
        for (i = 0; i < 3; i++) {
            id = *(int *)(a + 0x100 + i * 4);
            if (id == -1)
                continue;
            p = gm_Query(0, 2, 7, 1, id);
            p = gm_Query(*(int *)(p + 0x14), 1, 0, 0x1dd);
            for (q = gm_Query(*(int *)(p + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {
                if (side == G20_DEF)
                    *(int *)(q + 0xa) = G20_US(G20_DEF);
                if (*(int *)(q + 0xa) == G20_US(G20_DEF)) {
                    if (obj != 0 && ai_Unk60cb5c == G20_ME) {
                        gm_Unk50a050(q);
                        G19_4C4880U(*(int *)(obj + 2), *(int *)(q + 6), G20_US(q), *(int *)(q + 0xe), ai_OwnPlayer, 0);
                    }
                    gm_GoodsPrice(G20_US(q), ai_OwnPlayer);
                }
            }
        }
        G20_PUMP();
    } else if ((a[0x7c] & 2) && side == G20_DEF && G20_US(side) == ai_PlayerId) {
        ca = gm_BuildQuery(1, 1, *(int *)(a + 0x14));
        txt_Format(ba, 0x119f, ca, G20_US(G20_DEF), ca);
        k = ca[0];
        if (k != 0x1e && k != 0x20 && k != 0x1f) {
            txt_Format(bb, 0x11a0, (unsigned char)ca[0] * 14 + 0x48e);
            strcat(ba, bb);
        }
        txt_Format(bb, 0x11a1);
        strcat(ba, bb);
        txt_Format(bb, 0x11a2);
        strcat(ba, bb);
        txt_Format(bb, 0x11a3);
        strcat(ba, bb);
        n = ai_Unk4e5470(ba, 0x11);
        k = ca[0];
        if (k == 0x1e || k == 0x20 || k == 0x1f)
            n++;
        if (n == 1) {
            kr = ai_BTypes + (unsigned char)ca[0] * 0x24d;
            if (*kr == 4 || *kr == 0x10)
                ai_Unk4b44e0(G20_DEF, 0x1c);
            cm_SendOwnership(*(int *)(ca + 1), *(int *)(G20_DEF + 4), *(int *)(G20_DEF + 4));
            txt_Format(ba, 0x11a4, G20_US(G20_DEF), ca);
            ai_Unk4e5470(ba, 0);
        } else if (n == 2) {
            G20_TAKE();
            G20_PUMP();
        } else if (n == 3) {
            G20_FACE();
        } else if (n == 0) {
            G20_GUARDS();
        }
    } else if (a[0x7c] & 4) {
        c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
        if (ai_BTypes[(unsigned char)c[0] * 0x24d] == 0x13) {
            total = 0;
            for (i = 0; i < 16; i++) {
                id = *(int *)(a + 0x80 + i * 4);
                if (id == -1)
                    continue;
                c = ai_Unk4ce130(id);
                col = i % 4;
                row = i / 4 + 1;
                px = col * 80 + 0x30;
                py = row * 100;
                d2_WinOpenPic(px, py, 0x5db, d2_LastWindow);
                d2_WinOpenPic(px + 2, py, *(int *)(c + 0x18c), d2_LastWindow);
                sv_Unk52eaa0K(px, py + 0x3c, (unsigned char)c[0x83], 0x4eb);
                h = 0;
                if (c[8] == 0) {
                    v = (int)((unsigned char)c[0x83] * 0.023809524f + 1.0f);
                    d2_WinOpenPic(px + 6, py + 6, 0x3b, d2_LastWindow);
                    h = gm_PlayerFactor(v * 40, d2_LastWindow);
                }
                if (side == G20_DEF) {
                    if ((unsigned char)c[8] > 0) {
                        v = 0;
                        cm_BeginFieldBlock(c, *(int *)(c + 4));
                        cm_AddFieldValue(4, 1, (int)c - (int)cm_FieldObject + 0x16c, &v);
                        cm_SendFieldDeltas();
                    }
                    txt_Format(tmp, "%T", h);
                    if (h != 0)
                        d2_CreateWinText(px, py + 0x50, tmp, d2_LastWindow);
                    total += h;
                }
            }
            if (total != 0)
                cm_SendMoveGoods(*(int *)(G20_DEF + 4), -1, ai_OwnPlayer, total);
            else if (G20_US(&ai_PlayerId) == G20_US(G20_DEF))
                io_PrintText(0x11b6);
            if (*(int *)(a + 0x10c) != -1) {
                q = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                *(int *)(q + 0x4d) += total;
            }
            G20_PUMP();
        }
    } else if ((a[0x7c] & 2) && side == G20_DEF && G20_DEF[2] != 6 && G20_DEF[2] != 7) {
        gm_Unk5044b0(G20_US(G20_DEF), gm_BuildingJob((unsigned char)*kr));
        k = (unsigned short)util_RandomU(3) + 1;
        if (!gm_Unk5044b0(G20_US(G20_DEF), 0) || *kr == 0xd || *kr == 0xc || *kr == 0xb) {
            if (k == 2) {
                G20_TAKE();
                G20_PUMP();
            } else if (k == 3) {
                G20_FACE();
            } else if (k == 1) {
                G20_GUARDS();
            } else {
                txt_Format(ba, 0x11a4, G20_US(G20_DEF), ca);
                ai_Unk4e5470(ba, 0);
            }
        } else {
            txt_Format(ba, 0x11a4, G20_US(G20_DEF), ca);
            ai_Unk4e5470(ba, 0);
        }
    }
    di_ClearEvents(0x10);
    d2_CloseView(win);
}
