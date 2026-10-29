/* 0x50a250, hands the owner of the building b the person p's wage entries: up to `cnt` triples (value, kind 2 .. 7, amount) written to out (0x0 means the building has no owner / p is no person / cnt is 0; the first 3 triples of a kind 6 person's own record +0x71 are reused when they are still valid below the owner's budget 0x508660, else the whole table is built anew: from the random numbers (0x42ec00(0x1e), (6)) and the per-class tables 0x4f81c0 (0xf / 0xb) and the wage 0x50a640; a kind 6 person's table is stored back at +0x71, a kind 6 / 7 person employed by a non-6 / 7 owner is rated with 0x50c550). Returns 3 when written, 0 else. ROUGH: the float operands (the __ftol calls) are lost */

int gm_Unk50a250(int b, unsigned short *p, unsigned cnt, int *out)
{
    int budget, i, r, t, v;
    int *src, *dst;
    int tmp[0x88];
    char buf[36];
    unsigned k;
    int all = 0;

    if (p != 0 && b != 0 && *(short *)(b + 0x27) != -1 && cnt != 0) {
        budget = gm_SumSlotAmounts(*(int *)(b + 0x5d), ai_OwnPlayer);
        if (*((char *)p + 2) == 6) {
            if (*(int *)ai_Time < 6) {
                if (ai_Chars[*(unsigned short *)(b + 0x27)][2] != 6
                    && ai_Chars[*(unsigned short *)(b + 0x27)][2] != 7
                    && (float)budget < (float)gm_GetWorth(*p) * 0.2f) {
                    budget = (int)(float)budget;     /* lost */
                    cm_SendMoveGoods(*(int *)(b + 1), -1, ai_OwnPlayer, (int)(float)budget);
                }
            }
            src = (int *)(b + 0x71);
            dst = out;
            for (i = 3; i != 0; i--) {
                v = *src;
                if (v < 1) {
                    if (v == -1) {
                        all = -1;
                    }
                    *dst = -1;
                } else if (budget <= v) {
                    all = -1;
                    *dst = -1;
                } else {
                    dst[0] = src[0];
                    dst[1] = src[1];
                    dst[2] = src[2];
                    all = -1;
                }
                src += 3;
                dst += 3;
            }
            if (all == -1)
                return 3;
        }
        r = gm_Unk50a1c0(*(unsigned short *)(b + 0x27), 0, 0xffff, buf);
        if (r < 9) {
            memset(out, 0, cnt * 12);
            gm_GetWorth(*p);
            memset(tmp, 0, sizeof(tmp));
            tmp[0] = 1;
            k = ord_DispatchOrder(0xf, p, tmp);
            if ((k & 2) == 0)
                ord_DispatchOrder(0xb, p, tmp);
            for (k = 0; k < cnt; k++) {
                util_Random(0x1e);
                v = (int)(float)0;               /* lost */
                if (k == 0 || k == 1)
                    v = (int)(float)0;           /* lost */
                t = util_Random(6);
                r = (int)(float)0;               /* lost */
                if (ai_Chars[*(unsigned short *)(b + 0x27)][2] != 6
                    && ai_Chars[*(unsigned short *)(b + 0x27)][2] != 7
                    && (*((char *)p + 2) == 6 || *((char *)p + 2) == 7)) {
                    gm_Liking(*p, *(unsigned short *)(b + 0x27), 1);
                    r = (int)(float)0;           /* lost */
                }
                out[k * 3 + 2] = r;
                out[k * 3] = v;
                out[k * 3 + 1] = (unsigned short)t + 2;
            }
            if (*((char *)p + 2) == 6) {
                dst = (int *)(b + 0x71);
                for (i = 3; i != 0; i--) {
                    dst[0] = out[0];
                    dst[1] = out[1];
                    dst[2] = out[2];
                    out += 3;
                    dst += 3;
                }
            }
            return 3;
        }
    }
    return 0;
}
