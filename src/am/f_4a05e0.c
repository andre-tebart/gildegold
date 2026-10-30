/* 0x4a05e0: one of the six quarters (1..6) whose residents (0x4e7040) like each other (mean of 0x50c550 both ways
   over 33), tried in an order that depends on the character's home (0x168 / 0x166 / 0x167, 0x4e6f80) and is
   shuffled within 1..3 and 4..6; 0 when none */

int ai_Unk4a05e0(G5CH *p)
{
    int order[6];
    unsigned char info[0x18];
    G8SEAT seat[6];
    AICHAR *c1, *c2;
    unsigned int k;
    int i, j, n, d, cnt, t, x, y;
    float sum;

    for (i = 0; i < 6; i++)
        order[i] = i + 1;
    if (((unsigned char *)p)[0x168] != 0)
        k = ((unsigned char *)p)[0x168];
    else if (((unsigned char *)p)[0x166] != 0)
        k = ((unsigned char *)p)[0x166];
    else if (((unsigned char *)p)[0x167] != 0)
        k = ((unsigned char *)p)[0x167];
    if (k != 0) {
        if (amt_GetKindB((unsigned char)k, info) == 0)
            return 0;
        t = (info[8] - 1) / 3;
        if (t == 1) {
            for (i = 0; i < 6; i++)
                order[i] = (i + 4) % 6 + 1;
        } else if (t == 2) {
            for (i = 0; i < 6; i++)
                order[i] = 6 - i;
        }
    }
    for (i = 6; i != 0; i--) {
        x = util_Random(3);
        y = util_Random(3);
        t = order[x];
        order[x] = order[y];
        order[y] = t;
        x = util_Random(3) + 3;
        y = util_Random(3) + 3;
        t = order[x];
        order[x] = order[y];
        order[y] = t;
    }
    for (d = 0; d < 5; d++) {
        cnt = 0;
        sum = 0.0f;
        n = amt_GetColumnOfficesOwned(order[d], 6, seat);
        if (n >= 3) {
            for (i = 0; i < n - 1; i++) {
                if (seat[i].id == (int)p->netId || seat[i].id == -1)
                    continue;
                c1 = gm_FindChar(seat[i].id);
                for (j = i + 1; j < n && c1 != 0; j++) {
                    if (seat[j].id == (int)p->netId || seat[j].id == -1)
                        continue;
                    c2 = gm_FindChar(seat[j].id);
                    if (c2 == 0)
                        continue;
                    sum = gm_Liking(*(unsigned short *)c2, *(unsigned short *)c1, 1)
                        + (gm_Liking(*(unsigned short *)c1, *(unsigned short *)c2, 1) + sum);
                    cnt += 2;
                }
            }
            if (cnt != 0 && sum / cnt > 33.0f)
                return order[d];
        }
    }
    return 0;
}
