/* 0x4f3c50, the owners of the records that the character slot t holds (pairs in 0x91b6e0, record in state 1): counts how often each owner (+0x16) occurs and keeps the 32 best in ids[] / cnt[] sorted by count (descending, one
   insertion pass per owner); ids is first -1, cnt 0; returns the number of different owners (at most 0x20). ROUGH */

int unk_Unk4f3c50(int *ids, int *cnt, unsigned t)
{
    int local[512];
    int n, i, k, c, j, own, id, x;
    int *q, *w, *p;

    n = 0;
    for (i = 0; i < 0x20; i++) {
        ids[i] = -1;
        cnt[i] = 0;
    }
    for (i = 0; i < 0x200; i++)
        local[i] = -1;
    q = (int *)(unk_Tab91b6e0 + 4);
    w = local;
    id = *(int *)(ai_Chars[t & 0xffff] + 4);
    do {
        if (q[-1] == id) {
            k = unk_Unk4f36e0(*q);
            if (k != -1 && *(int *)(gv_Graveyard + 0x25 + k * 0x2d) == 1) {
                *w = *(int *)(gv_Graveyard + 0x16 + k * 0x2d);
                w++;
            }
        }
        q += 2;
    } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
    p = local;
    j = 1;
    do {
        own = *p;
        if (own != -1) {
            *p = -1;
            n++;
            c = 1;
            for (i = j; i < 0x200; i++) {
                if (local[i] == own) {
                    c++;
                    local[i] = -1;
                }
            }
            if (cnt[0x1f] < c) {
                cnt[0x1f] = c;
                ids[0x1f] = own;
            }
            for (i = 0x1f, q = cnt + 0x1e; i > 0; i--, q--) {
                x = q[1];
                if (*q < x) {
                    k = ids[i];
                    ids[i] = ids[i - 1];
                    q[1] = *q;
                    ids[i - 1] = k;
                    *q = x;
                }
            }
        }
        p++;
        i = j < 0x200;
        j++;
    } while (i);
    if (n > 0x20)
        n = 0x20;
    return n;
}
