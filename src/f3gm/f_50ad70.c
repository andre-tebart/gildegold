/* 0x50ad70, how many of the object id the character c may take (n wanted, at most 3 of one kind, at most 5 kinds); only the kinds 8 0xc 0x12 0x19 0x22 0x20 0x27 count (kind 9: n), none of the 0x179 / 0x17a and none that the town forbids (0x4cd9d0) for the kinds 6 / 7 */

int gm_Unk50ad70(char *c, int proto, int n)
{
    unsigned char k;
    int found, cnt, v;
    int tab[8] = { 8, 0xc, 0x12, 0x19, 0x22, 0x20, 0x27, 0 };
    int *t;
    int u;
    char *o;
    unsigned short *w;

    v = 0;
    found = 0;
    k = ai_Goods[proto * 0x41];
    if (k == 9)
        return n;
    u = 8;
    t = tab;
    do {
        if (u == k) {
            found = 1;
            break;
        }
        u = *++t;
    } while (u != 0);
    if (((c[2] == 6 || c[2] == 7) && ai_Unk4cd9d0(proto) != 0) || proto == 0x179 || proto == 0x17a || !found)
        return 0;
    o = gm_Query(*(void **)(c + 0x178), 1, 0, proto);
    if (o == 0) {
        cnt = 0;
        w = (unsigned short *)gm_Query(*(void **)(c + 0x178), 1, 5);
        if (w != 0) {
            do {
                if (ai_Goods[*w * 0x41] != 9)
                    cnt++;
                w = (unsigned short *)gm_QueryNext();
            } while (w != 0);
            if (cnt > 5)
                return v < 0 ? 0 : v;
        }
        v = 3;
        if (3 > n)
            v = n;
        return v < 0 ? 0 : v;
    } else {
        ai_Unk4cd9d0(proto);
        if (3 < *(int *)(o + 0xe) + n) {
            v = 3 - *(int *)(o + 0xe);
            return v < 0 ? 0 : v;
        }
    }
    v = n;
    return v < 0 ? 0 : v;
}
