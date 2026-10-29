/* 0x4a7f50, rates the hand of owner: 0 nothing, 1 pair, 2 two pairs, 3 three, 4 straight, 5 full house, 6 flush,
   7 poker, 8 straight flush (the names at 0x600490); the groups into g1 / g2, the highest ranks into r1 / r2 */

char ai_Unk4a7f50(int owner, int *g1, int *g2, int *r1, int *r2)
{
    int cnt[6];
    int rcnt[4];
    int g, r, n, i;
    int pairs, rpairs, maxn, maxr, bestr, kind, a, b;
    G10CARD *c, *q, *q2;

    pairs = 0;
    rpairs = 0;
    maxn = 0;
    maxr = 0;
    bestr = 0;
    kind = 0;
    g = 0;
    c = ai_Unkf83660;
    b = -1;
    a = 0;
    do {
        n = 0;
        for (i = 4; i != 0; i--) {
            if (c->owner == owner)
                n++;
            c++;
        }
        cnt[g] = n;
        if (n > 1) {
            pairs++;
            if (n > maxn) {
                b = a;
                a = g;
                maxn = n;
            } else if (b < 0) {
                b = g;
            }
        }
        g++;
    } while (c < &ai_Unkf83660[24]);
    r = 0;
    c = ai_Unkf83660;
    do {
        n = 0;
        q = c;
        for (i = 6; i != 0; i--) {
            if (q->owner == owner)
                n++;
            q += 4;
        }
        rcnt[r] = n;
        if (n > 1)
            rpairs++;
        if (maxr < n) {
            maxr = n;
            bestr = r;
        }
        c++;
        r++;
    } while (c < &ai_Unkf83660[4]);
    if (rpairs == 1 && maxr == 5) {
        kind = 6;
        *r1 = bestr;
    }
    if (pairs == 0) {
        if (cnt[0] == 0 || cnt[5] == 0)
            return (kind == 6) * 4 + 4;
        if (kind == 6)
            return 6;
    } else if (pairs == 1) {
        if (maxn == 4) {
            *g1 = a;
            return 7;
        }
        if (maxn == 3) {
            *g1 = a;
            return 3;
        }
        if (maxn == 2) {
            *g1 = a;
            q = &ai_Unkf83660[a * 4 + 3];
            for (r = 3; r >= 0; r--) {
                if (q->owner == owner) {
                    *r1 = r;
                    return 1;
                }
                q--;
            }
            return 1;
        }
    } else if (pairs == 2) {
        if (maxn == 3) {
            *g1 = a;
            *g2 = b;
            return 5;
        }
        if (maxn == 2) {
            *g1 = a;
            *g2 = b;
            q2 = &ai_Unkf83660[b * 4 + 3];
            q = &ai_Unkf83660[a * 4 + 3];
            for (r = 3; r >= 0; r--) {
                if (q->owner == owner)
                    *r1 = r;
                if (q2->owner == owner)
                    *r2 = r;
                q--;
                q2--;
            }
            return 2;
        }
    }
    return 0;
}
