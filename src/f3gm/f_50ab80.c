/* 0x50ab80, how many of the object id the list object l can still take (at most n), by the free places of its kind (0x50a0f0 gets its owner, 0x501d80 the class of the owner's building). ROUGH */

int gm_FreeCapacity(short *l, int id, int n)
{
    char k;
    unsigned char *o;
    int cls, cap, cnt, in;
    int pre;
    unsigned short *w;
    char *p;
    int t;

    o = (unsigned char *)gm_Unk50a0f0((int)l);
    if (o == 0)
        return 0;
    cls = gm_BuildingClass(*o);
    cap = gm_ListStock(l, id);
    in = (int)gm_Query(*(void **)(l + 10), 1, 0, id);
    if (in == 0) {
        k = ai_Goods[id * 0x41];
        if (cls != 2)
            pre = (k == 0x17 || k == 0x25);
        else
            pre = (k == 0x17);
        cnt = 0;
        if (*l == 0x2a) {
            w = (unsigned short *)gm_Query(*(void **)(l + 10), 1, 5);
            while (w != 0) {
                p = ai_Goods + *w * 0x41;
                k = *p;
                if (cls != 2) {
                    if (pre) {
                        if (k == 0x17 || k == 0x25)
                            cnt++;
                    } else if (k != 0x17 && k != 0x25)
                        cnt++;
                } else {
                    if (pre) {
                        if (k == 0x17)
                            cnt++;
                    } else if (k != 0x17)
                        cnt++;
                }
                w = (unsigned short *)gm_QueryNext();
            }
            if (pre) {
                if (cnt >= *(unsigned char *)(l + 0xe))
                    return 0;
            } else if (cnt >= *(unsigned char *)((char *)l + 0x1d))
                return 0;
        } else {
            t = (int)gm_Query(*(void **)(l + 10), 1, 5);
            while (t != 0) {
                cnt++;
                t = (int)gm_QueryNext();
            }
            if (cnt >= *(unsigned char *)(l + 0xe))
                return 0;
        }
        if (cap < n)
            n = cap;
    } else {
        t = cap - gm_ListAmount(l, (char *)in);
        if (t <= n)
            return t;
    }
    return n;
}
