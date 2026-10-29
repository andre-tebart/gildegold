/* 0x4f3dd0, as 0x4f3c50 over all the records of the character slot t (any state): groups the pairs by owner, per owner the count of records (c2[]) and of those in state 1 (c1[]); the 32 owners with the most records are kept in
   ids[] / c1[] / c2[] sorted by c2 (descending); returns the number of owners (at most 0x20). ROUGH */

int unk_Unk4f3dd0(int *ids, int *c1, int *c2, unsigned t)
{
    char *rec[512];
    int n, i, j, k, m, own, tot, st, id, x, y;
    int *q, *q2, *q3;
    unsigned *s;
    char *r, *r2;

    for (i = 0; i < 0x20; i++)
        ids[i] = -1;
    n = 0;
    for (i = 0; i < 0x20; i++)
        c1[i] = 0;
    for (i = 0; i < 0x20; i++)
        c2[i] = 0;
    for (i = 0; i < 0x200; i++)
        rec[i] = 0;
    q = (int *)(unk_Tab91b6e0 + 4);
    m = 0;
    id = *(int *)(ai_Chars[t & 0xffff] + 4);
    do {
        if (q[-1] == id) {
            k = unk_Unk4f36e0(*q);
            if (k != -1 && *(int *)(gv_Graveyard + 0x25 + k * 0x2d) != 0) {
                rec[m] = gv_Graveyard + k * 0x2d;
                m++;
            }
        }
        q += 2;
    } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
    if (m > 0) {
        j = 1;
        for (i = 0; ; i++) {
            tot = 1;
            r = rec[i];
            if (r != 0) {
                own = *(int *)(r + 0x16);
                n++;
                st = (*(int *)(r + 0x25) == 1);
                for (k = j; k < 0x200; k++) {
                    r2 = rec[k];
                    if (r2 != 0 && *(int *)(r2 + 0x16) == *(int *)(rec[i] + 0x16)) {
                        tot++;
                        if (*(int *)(r2 + 0x25) == 1)
                            st++;
                        rec[k] = 0;
                    }
                }
                rec[i] = 0;
                s = (unsigned *)c1 + 0x1f;
                if ((int)c1[0x1f] < tot) {
                    *s = st;
                    c2[0x1f] = tot;
                    ids[0x1f] = own;
                }
                q2 = c2 + 0x1e;
                for (k = 0x1f; k != 0; k--) {
                    x = q2[1];
                    if (*q2 < x) {
                        y = *s;
                        id = *(int *)((char *)s + ((char *)ids - (char *)c1));
                        *(int *)((char *)s + ((char *)ids - (char *)c1)) = *(int *)((char *)q2 + ((char *)ids - (char *)c2));
                        q2[1] = *q2;
                        *s = *(unsigned *)((char *)q2 + ((char *)c1 - (char *)c2));
                        *(int *)((char *)q2 + ((char *)ids - (char *)c2)) = id;
                        *q2 = x;
                        *(int *)((char *)q2 + ((char *)c1 - (char *)c2)) = y;
                    }
                    q2--;
                    s--;
                }
            }
            if (!(j < m))
                break;
            j++;
        }
        if (n > 0x20)
            return 0x20;
    }
    return n;
}
