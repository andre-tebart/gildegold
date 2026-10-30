/* 0x53f5b0, the money of the figures (kind 5 / 4 pay to the level 0x640 * (cost class * 5 + 0x14), kind 2 on every 4th day): every 0x20 changes it waits for the last change (0x4c3ae0) */

unsigned he_Unk53f5b0(void)
{
    unsigned char b;
    unsigned u = 0, u3;
    int i4, i7, cnt, d;
    unsigned last = 0xffffffff;
    unsigned char *pb;
    int tgt;

    i4 = (int)U_I((unsigned)&ai_Time) / 10 + 1;
    u = (int)U_I((unsigned)&ai_Time) / i4;
    if ((int)U_I((unsigned)&ai_Time) % i4 == 0) {
        pb = (unsigned char *)((unsigned)&ai_Chars + 0x2);
        i7 = (U_I((unsigned)&ai_Difficulty) * 5 + 0x14) * 0x640;
        cnt = 0;
        do {
            b = *pb;
            u = b;
            if (b == 5) {
                u = gm_GetGoodsValue(pb - 2);
                if (i7 <= (int)u)
                    goto L_next;
                tgt = *(int *)(pb + 2);
L_pay:
                u = ((int (*)(int, int, unsigned char, int))cm_SendMoveGoods)(tgt, -1, U_I((unsigned)&ai_OwnPlayer), i7 - u);
                cnt++;
                last = u;
            } else if (b == 4) {
L_chk:
                u = gm_GetGoodsValue(pb - 2);
                if ((int)u < i7) {
                    tgt = *(int *)(pb + 2);
                    goto L_pay;
                }
            } else if (b == 2) {
                u3 = U_I((unsigned)&ai_Time) & 0x80000003;
                if ((int)u3 < 0)
                    u3 = (u3 - 1 | 0xfffffffc) + 1;
                if (u3 == (*(unsigned *)(pb + 2) & 3))
                    goto L_chk;
            }
L_next:
            if (cnt > 0x1f && last != 0xffffffff) {
                while ((u = cm_GetSlotState(last)) == 0)
                    sv_PumpNet();
                last = 0xffffffff;
                cnt = 0;
            }
            pb += 0x21c;
        } while ((int)pb < (unsigned)((unsigned)ai_Chars + 0x65402));
    }
    return u;
}
