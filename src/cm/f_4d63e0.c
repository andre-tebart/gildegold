/* 0x4d63e0, a fight starts (a: the fight record): characters that are gone are dropped from both id lists, a cart
   without goods or no defender ends it (bad = 2); with the player present (0x6f0d8c == 0) the fight scene is built and
   fought (0x4d0fa0 .. 0x4d1e00), 0x4d4540 asks for the ransom and 0x4d4d70 shows the end, else (remote) it is decided
   at once by strength (sum of 0x4cdcf0 + random 30 per side, losers lose up to 20 * random 10 health) and
   the loot goes through 0x4d3d10 / 0x4d3ea0. Rough draft. */

void ai_Unk4d63e0(char *a)
{
    char buf[0x800];
    int rc[0x2e];
    int bad = 0, nb = 0, seed, i, id, sa, sb, w, n, r;
    char *c, *p, *win, *e1, *e2;
    int *e;

    seed = ai_Unk4e52c0();
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0x80 + i * 4);
        if (id != -1 && gm_FindChar(id) == 0) {
            bad = 1;
            *(int *)(a + 0x80 + i * 4) = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0xc0 + i * 4);
        if (id != -1) {
            if (gm_FindChar(id) == 0) {
                bad = 1;
                *(int *)(a + 0xc0 + i * 4) = -1;
            } else
                nb++;
        }
    }
    if (a[0x7c] & 1) {
        for (i = 0; i < 3; i++) {
            id = *(int *)(a + 0x100 + i * 4);
            if (id != -1) {
                p = (char *)gm_FindObjectInst(id);
                if (p != 0) {
                    gm_StallGoodsList(p, rc);
                    if (rc[0x2c] > 0)
                        continue;
                }
                bad = 2;
            }
        }
    }
    if (gm_BuildQuery(1, 1, *(int *)(a + 0x10c)) == 0 || bad == 2 || nb == 0)
        return;
    if (ai_Unk6f0d8c == 0) {
        ai_Unk60d364 = 200;
        ai_Unk60cb5c = *(int *)(a + 0xc);
        ai_Unk4d0fa0(a);
        ai_Unk4d0a30();
        ai_Unk4d1c20(a + 0xc0, (int)"OFF");
        ai_Unk4d1c20(a + 0x80, (int)"DEF");
        ai_Unk4d1e00(a);
        if (ai_Unk6f244c != 0)
            d3_SetLightSet(2, 0, 0x3f800000, 1);
        else
            sv_Unk525ad0(ai_Time);
        ai_Unk6f0cf4 = 0;
        ai_Unk6f0cf8 = 0;
        ai_Unk6f0cfc = 0;
        ai_Unk4d4540(a);
        ai_Unk4e52a0(seed);
        ai_Unk4d0a30();
        ai_Unk4d0a30();
        ai_Unk4e3830(a, 0);
        if (ai_Unk6f0cf4 != 0 || ai_Unk6f0cf8 == 0) {
            if (ai_Unk6f0cf4 == 0)
                ai_Unk4e3830(a, 0);
        } else
            ai_Unk4e3830(a, 0);
        if (ai_Unk6f0cf8 == 0) {
            for (i = 0; i < 16; i++) {
                if (ai_Unk9304c0[i].h != -1)
                    d2_RemoveObj(ai_Unk9304c0[i].h);
                ai_Unk9304c0[i].h = -1;
                ai_Unk9304c0[i].v = 0;
                ai_Unk9304c0[i].c = 0;
            }
            ai_Unk4d4d70(a);
            ai_Unk4d1eb0(a);
            ai_Unk4d1a10(a);
            return;
        }
        if (ai_Unk6f0cfc != 0 && G20_DEF[2] == 6) {
            e1 = 0;
            c = gm_BuildQuery(1, 1, *(int *)(ai_Unk6f0d04 + 0x10c));
            if (c != 0)
                e1 = gm_Query(*(int *)(c + 0x5d), 2, 6, 0, 0x116);
            if (ai_Unk6f0cfc > 0) {
                if (e1 == 0)
                    txt_Format(buf, 0x1144, ai_Unk6f0cfc);
                else
                    txt_Format(buf, 0x1145, ai_Unk6f0cfc);
            } else
                txt_Format(buf, 0x1146);
            ai_Unk4e5470(buf, 0x10);
        }
        ai_Unk4d1a10(a);
        return;
    }
    sa = 0;
    sb = 0;
    ai_Unk6f0cec = gm_FindChar(*(int *)(a + 0x34));
    ai_Unk6f0ce8 = gm_FindChar(*(int *)(a + 0x38));
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0x80 + i * 4);
        if (id != -1 && (c = gm_FindChar(id)) != 0 && c[8] != 0 && c[8] != 4) {
            sa = (int)(ai_Unk4cdcf0(c) + sa);
            r = (unsigned short)util_Random(10);
            gm_AddHealth(G20_US(c), -r * 20);
        }
    }
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0xc0 + i * 4);
        if (id != -1 && (c = gm_FindChar(id)) != 0 && c[8] != 0 && c[8] != 4) {
            sb = (int)(ai_Unk4cdcf0(c) + sb);
            r = (unsigned short)util_Random(10);
            gm_AddHealth(G20_US(c), -r * 20);
        }
    }
    if ((unsigned short)util_Random(30) + sb > (unsigned short)util_Random(30) + sa)
        win = G20_DEF;
    else
        win = G20_ATT;
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0x80 + i * 4);
        if (id != -1 && (c = gm_FindChar(id)) != 0)
            ai_Unk4cdb80(c);
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(a + 0xc0 + i * 4) != -1 && (c = gm_FindChar(*(int *)(a + 0x80 + i * 4))) != 0)
            ai_Unk4cdb80(c);
    }
    if (a[0x7c] & 2) {
        if (win == G20_DEF) {
            e1 = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
            e2 = gm_BuildQuery(1, 1, *(int *)(a + 0x14));
            ai_Unk4d3d10(e2, e1, G20_DEF, G20_ATT);
        }
    } else if (!(a[0x7c] & 4) && (a[0x7c] & 1) && win == G20_DEF) {
        c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
        ai_Unk4d3ea0(a, c, G20_DEF, ai_Unk6f0ce8, 100.0f);
    }
}
