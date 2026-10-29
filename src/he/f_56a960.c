/* 0x56a960, the run handler of the trade event (ROUGH, a skeleton from the disassembly; the two slot loops are simplified): +0xb0 / +0xb4 / +0xac the buildings and the good looked up (0x4fea50 A / C / B),
   +0xd0 (3 good ids, short) and +0xd6 (3 short) with the owners +0xb8 / +0xc4 are the sell / buy slots, +0xe8 / +0xec the transport / message handles, +0xf0 / +0xf1 flags; state -2 resets the
   state and runs 0x541250 for the first player; while a handle is still running the record is due again in 1 unit; a record of the same good that is already running (0x53e400 (2, 0, 0xf, 2, owner)) frees this
   one; the amounts are ordered by 0x4c4880 from the goods buildings (0x4ffe90 ...) and the rest is posted as a message (0x4c4b70) */

int ai_Unk5ae8e0();
int gm_FindMarketObject(int id);
float gm_GoodsPrice(unsigned id, unsigned char p);
int gm_ListStock();
int gm_Unk509ee0(char *c, char p);

void he_Unk56a960(int ev)
{
    int A, B, C, bo[2], g, g2, s, i, k, n, m, flags, amount, left, hit, e2, per;
    unsigned short *sl;
    char msg[0xf8];
    char buf[0x100];

    A = B = C = 0;
    flags = 0;
    left = 0;
    if (*(int *)(ev + 0x70) == -2) {
        *(int *)(ev + 0x70) = 0;
        he_Unk541250(ev, *(unsigned short *)U_I((unsigned)&ai_PlayerChar));
    }
    if (*(int *)(ev + 0xe8) != -1) {
        if (cm_GetSlotState(*(int *)(ev + 0xe8)) == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            return;
        }
        *(int *)(ev + 0xe8) = -1;
    }
    if (*(int *)(ev + 0xec) != -1) {
        if (cm_GetSlotState(*(int *)(ev + 0xec)) == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            return;
        }
        *(int *)(ev + 0xec) = -1;
    }
    gm_FindById(0, bo, 0, *(int *)(ev + 0xac));
    B = bo[0];
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xb0));
    A = bo[0];
    gm_FindById(0, 0, bo, *(int *)(ev + 0xb4));
    C = bo[0];
    if (B == 0 || A == 0)
        goto done_free;
    for (e2 = (int)he_FindEvent(2, 0, 0xf, 2, *(unsigned short *)(ev + 8)); e2 != 0; e2 = he_NextEvent()) {
        if (*(int *)(e2 + 0xac) == *(int *)(ev + 0xac)) {
            if (*(int *)(e2 + 0xb4) == *(int *)(ev + 0xb0))
                goto rearm;
            goto done_free;
        }
    }
    if (*(char *)(ev + 0xf1) == 0 && ai_Unk5ae8e0(A, B) == 0) {
        ai_Unk5ae8e0(A, B);
        he_Unk53e1f0((int *)ev);
        return;
    }
    g = gm_Query(*(int *)(B + 0x14), 1, 0, 0x1dd);
    s = *(unsigned char *)(g + 0x1c) < 3 ? *(unsigned char *)(g + 0x1c) : 3;
    hit = 0;
    for (i = 0; i < s && !hit; i++)
        if (*(unsigned short *)(ev + 0xd0 + i * 2) != 0 || *(unsigned short *)(ev + 0xd6 + i * 2) != 0)
            hit = 1;
    if (!hit) {
        /* nothing to trade: the transport is ordered from the first store of the kind 0x2a / 0x116 (0x4c4880) */
        k = gm_Query(*(int *)(A + 0x5d), 2, 6, 0x2a);
        if (k == 0)
            k = gm_Query(*(int *)(A + 0x5d), 2, 6, 0x116);
        for (g2 = k != 0 ? gm_Query(*(int *)(g + 0x14), 1, 5, 0) : 0; g2 != 0; g2 = gm_QueryNext())
            *(int *)(ev + 0xe8) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(k + 2), *(int *)(g + 2), *(unsigned short *)g2, *(int *)(g2 + 0xe), *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        if (*(int *)(ev + 0xe8) == -1)
            goto done_free;
        goto rearm;
    }
    for (sl = (unsigned short *)(ev + 0xd0), i = 0; i < s; i++, sl++) {
        if (*sl != 0 && *(int *)(ev + 0xb8 + i * 4) == *(int *)(A + 1)) {
            g2 = gm_Query(*(int *)(g + 0x14), 1, 0, *sl);
            if (g2 != 0) {
                k = *(char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)A * 0x24d) == 0xa ? gm_FindMarketObject(*sl) : gm_Query(*(int *)(A + 0x5d), 2, 6, 0x2a);
                if (k == 0)
                    k = gm_Query(*(int *)(A + 0x5d), 2, 6, 0x116);
                if (k != 0) {
                    n = gm_ListAmount(B, g2);
                    m = gm_FreeCapacity(k, *(unsigned short *)g2, n);
                    amount = (int)(gm_GoodsPrice(*(unsigned short *)g2, *(unsigned char *)(unsigned)&ai_OwnPlayer) * 0.01);
                    if (n < m)
                        m = n;
                    cm_SendExSellObject(*(int *)(k + 2), *(unsigned short *)g2, m, amount, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                    if (C != 0)
                        *(int *)(C + 0x1b8) += amount * m;
                    if (n - m != 0) {
                        amount = (int)(gm_GoodsPrice(*(unsigned short *)g2, *(unsigned char *)(unsigned)&ai_OwnPlayer) * 0.01);
                        *(int *)(ev + 0xe8) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(B + 2), -1, *(unsigned short *)g2, n - m, *(unsigned char *)(unsigned)&ai_OwnPlayer, amount);
                    }
                }
            }
            *sl = 0;
            *(int *)(ev + 0xb8 + i * 4) = -1;
        }
    }
    if (C != 0 && *(int *)(C + 0x16c) != 0)
        left = gm_Unk509ee0((unsigned)&ai_Chars + *(unsigned short *)(*(int *)(C + 0x16c) + 0x27) * 0x21c, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    if (left > *(int *)(C + 0x1b8))
        left = *(int *)(C + 0x1b8);
    for (sl = (unsigned short *)(ev + 0xd6), i = 0; i < s; i++, sl++) {
        if (*sl != 0 && *(int *)(ev + 0xc4 + i * 4) == *(int *)(A + 1)) {
            k = *(char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)A * 0x24d) == 0xa ? gm_FindMarketObject(*sl) : gm_Query(*(int *)(A + 0x5d), 2, 6, 0x2a);
            if (k == 0)
                k = gm_Query(*(int *)(A + 0x5d), 2, 6, 0x116);
            n = 0;
            if (k != 0) {
                g2 = gm_Query(*(int *)(k + 0x14), 0, 1, *sl);
                if (g2 != 0) {
                    if (*(char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)A * 0x24d) == 0xa)
                        amount = (int)(gm_GoodsPrice(*(unsigned short *)g2, *(unsigned char *)(unsigned)&ai_OwnPlayer) * 0.01);
                    else
                        amount = (int)(gm_GoodsPrice(*(unsigned short *)g2, *(unsigned char *)(unsigned)&ai_OwnPlayer) * *(float *)(A + 0x49));
                    per = amount;
                    n = gm_ListStock(k, *(unsigned short *)g2) * *(int *)(g2 + 0x1c) >> 2;
                    n = gm_ListAmount(k, g2) - n;
                    if (left / per < n)
                        n = left / per;
                    if (n < 0)
                        n = 0;
                    if (n != 0)
                        *(int *)(ev + 0xe8) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(B + 2), *(unsigned short *)g2, n, per, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                    if (C != 0 && *(int *)(C + 0x16c) != 0)
                        sprintf(buf, "%s %s %li %li %s", (char *)(C + 0x30), (char *)(*(int *)(C + 0x16c) + 5), n * per, n, (char *)((char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)g2 * 0x41 + 1));
                    left -= n * per;
                    if (C != 0)
                        *(int *)(C + 0x1b8) -= n * per;
                }
            }
            if (n == 0 && *(unsigned char *)(ev + 0xf0) != 0)
                flags |= 1;
            else {
                *sl = 0;
                *(int *)(ev + 0xc4 + i * 4) = -1;
            }
        }
    }
    if (*(unsigned char *)(ev + 0xf0) != 0 && flags != 0) {
        if (*(unsigned short *)(unsigned)&ai_TimeHour < 0x14) {
            gm_AddTime(ev + 0x52, 0, 0x1e, 0);
            *(unsigned char *)(ev + 0xf0) -= 1;
        } else {
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(unsigned char *)(ev + 0xf0) = 0;
        }
        return;
    }
    *(int *)(ev + 0xb0) = -1;
    for (i = 0; i < s && *(int *)(ev + 0xb0) == -1; i++)
        if (*(unsigned short *)(ev + 0xd0 + i * 2) != 0)
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb8 + i * 4);
    for (i = 0; i < s && *(int *)(ev + 0xb0) == -1; i++)
        if (*(unsigned short *)(ev + 0xd6 + i * 2) != 0)
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xc4 + i * 4);
    if (*(int *)(ev + 0xb0) == -1) {
        gm_FindById(bo, 0, 0, *(int *)(ev + 0x10));
        *(int *)(ev + 0xb0) = *(int *)(bo[0] + 1);
    }
    if (*(char *)(ev + 0xf1) != 0) {
        gm_AddTime(ev + 0x52, 1, 0x1e, 0);
        return;
    }
    memset(msg, 0, sizeof(msg));
    msg[4] = 0xf;
    *(int *)(msg + 0xc) = -1;
    *(int *)(msg + 8) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c);
    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
    msg[0x36] = 2;
    *(int *)(msg + 0x58) = *(int *)(B + 2);
    *(int *)(msg + 0x5c) = *(int *)(A + 1);
    *(int *)(msg + 0x60) = *(int *)(ev + 0xb0);
    *(int *)(msg + 0xa0) = *(int *)(ev + 4);
    if (*(int *)(A + 1) == *(int *)(ev + 0xb0)) {
        gm_AddTime(ev + 0x52, 0, 5, 0);
        return;
    }
    *(int *)(ev + 0xec) = cm_SendLongMessage(msg);
    gm_AddTime(ev + 0x52, 0, 0xa, 0);
    return;
rearm:
    gm_AddTime(ev + 0x52, 0, 5, 0);
    return;
done_free:
    he_Unk53e1f0((int *)ev);
}
