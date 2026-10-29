/* 0x50b3a0, lays out the tree of the items that follow the item *b in the list of the building a (record +0x22 the count, +0x23 the ids (& 0x7fff), +0xa5 pairs of parents): the nodes (until an item of class 2 / 6) go into
   0x76fde0 (id, the two parents, rank = 0x50b2f0, x / y unset, links -1), are bubble-sorted by rank, the rank starts go to 0x770b00, then the x positions are spread over the width w (averages of the parents' x,
   spreading of equal x, a minimum distance of 0x50, a shift when the tree leaves the area), and the y positions over the height h (0x60 per rank at most); 1 when there is nothing to lay out, else 0. ROUGH */

typedef struct {
    int id, p1, p2;
    unsigned short rank, pad;
    int x, y, z;
    int l[5];
    unsigned char flag;
} RTNODE;       /* 0x34 bytes, the nodes of the research tree layout 0x76fde0 */
#define NODES ((RTNODE *)ai_Unk76fde0)
int ai_Unk889f70;  /* 0x889f70 */
unsigned short ai_Unk770b00[64];  /* 0x770b00 */
unsigned gm_Unk50b2f0();                /* 0x50b2f0 */

int gm_Unk50b3a0(unsigned short *a, unsigned short *b, int w, int h)
{
    char *rec;
    int n, i, j, k, lv, cnt, ranks, step, sum, num, lo, hi, d;
    unsigned short *ids;
    unsigned short rmax;
    unsigned s, e, t, u;
    int again;
    RTNODE tmp;
    int *pp;

    rec = ai_BTypes + (unsigned char)*(char *)a * 0x24d;
    n = 0;
    cnt = (unsigned char)rec[0x22];
    ids = (unsigned short *)(rec + 0x23);
    if (cnt != 0) {
        do {
            if ((ids[n] & 0x7fff) == *b)
                break;
            n++;
        } while (n < cnt);
    }
    if (cnt <= n)
        return 1;
    ai_Unk7d6a40 = 0;
    n++;
    for (; n < (unsigned char)rec[0x22]; n++) {
        char cls = *(char *)(ai_Goods + (ids[n] & 0x7fff) * 0x41);
        RTNODE *r;
        if (cls == 2 || cls == 6)
            break;
        r = &NODES[ai_Unk7d6a40];
        r->id = ids[n] & 0x7fff;
        r->p1 = *(unsigned short *)(rec + 0xa3 + n * 4);
        r->p2 = *(unsigned short *)(rec + 0xa5 + n * 4);
        r->rank = 0xffff;
        r->x = 0;
        r->y = 0;
        for (k = 0; k < 5; k++)
            r->l[k] = -1;
        r->flag = 0xff;
        ai_Unk7d6a40++;
    }
    cnt = ai_Unk7d6a40;
    if (cnt == 0)
        return 1;
    rmax = 0;
    for (i = 0; i < cnt; i++) {
        NODES[i].rank = (unsigned short)gm_Unk50b2f0(i);
        if (rmax <= NODES[i].rank)
            rmax = NODES[i].rank;
    }
    ai_Unk889f70 = rmax + 1;
    do {
        again = 0;
        for (i = 0; i < cnt; i++)
            for (j = i + 1; j < cnt; j++)
                if (NODES[j].rank < NODES[i].rank) {
                    tmp = NODES[i];
                    NODES[i] = NODES[j];
                    NODES[j] = tmp;
                    again = 1;
                }
    } while (again);
    lv = 0;
    ai_Unk770b00[0] = 0;
    for (i = 0; i < cnt; i++) {
        if (lv < NODES[i].rank) {
            lv++;
            ai_Unk770b00[lv] = (unsigned short)i;
        }
    }
    ranks = ai_Unk889f70;
    ai_Unk770b00[ranks] = (unsigned short)cnt;
    t = ai_Unk770b00[1];
    step = w / (int)(t * 2);
    if (t != 0) {
        d = step - 0x18;
        for (i = 0; i < (int)t; i++) {
            NODES[i].x = d;
            d += step * 2;
        }
    }
    for (lv = 1; lv < ranks; lv++) {
        s = ai_Unk770b00[lv];
        e = ai_Unk770b00[lv + 1];
        for (i = s; i < (int)e; i++) {
            sum = 0;
            num = 0;
            for (j = 0; j < (int)s; j++) {
                if (NODES[j].id == NODES[i].p1 || NODES[j].id == NODES[i].p2) {
                    sum += NODES[j].x;
                    num++;
                }
            }
            if (num != 0)
                sum /= num;
            NODES[i].x = sum;
        }
        do {
            again = 0;
            for (i = s; i < (int)e; i++)
                for (j = i + 1; j < (int)e; j++)
                    if (NODES[j].x < NODES[i].x) {
                        tmp = NODES[i];
                        NODES[i] = NODES[j];
                        NODES[j] = tmp;
                        again = 1;
                    }
        } while (again);
        for (i = s; i < (int)e; i++) {
            u = i;
            for (j = i; j < (int)e; j++)
                if (NODES[i].x != NODES[j].x)
                    break;
            if (i + 1 < j) {
                num = j - i;
                d = (w / (int)t >> (lv - 1)) + (num * 3 + 3) * -8;
                if (num > 0) {
                    k = 0;
                    for (u = 0; (int)u < num; u++) {
                        NODES[i + u].x += k / (num - 1) - (d >> 1);
                        k += d;
                    }
                }
            }
        }
        for (i = s; i < (int)e; i++)
            for (j = i + 1; j < (int)e; j++) {
                lo = NODES[i].x;
                if (NODES[j].x - lo < 0x50) {
                    d = (lo - NODES[j].x + 0x50) >> 1;
                    NODES[i].x = lo - d;
                    NODES[j].x += d;
                }
            }
    }
    lo = w + 1;
    hi = -1;
    for (lv = 0; lv < ranks; lv++)
        for (i = ai_Unk770b00[lv]; i < ai_Unk770b00[lv + 1]; i++) {
            if (NODES[i].x <= lo)
                lo = NODES[i].x;
            if (hi <= NODES[i].x)
                hi = NODES[i].x;
        }
    if (lo - 0x18 < 0 || w <= hi + 0x60) {
        d = (int)((w - (hi - lo)) * 0.5);
        for (lv = 0; lv < ranks; lv++)
            for (i = ai_Unk770b00[lv]; i < ai_Unk770b00[lv + 1]; i++)
                NODES[i].x = (int)(NODES[i].x - lo) + d;
    }
    step = ranks * 0x60;
    if (h - 0x10 <= step)
        step = h - 0x10;
    for (i = 0; i < ai_Unk7d6a40; i++)
        NODES[i].y = (int)((unsigned)NODES[i].rank * step) / ranks + 0x10;
    return 0;
}
