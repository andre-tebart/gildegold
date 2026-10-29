/* 0x5098a0, the goods list of the building b: out (0x2e ints) gets the max count (out[0]), the number of entries (out[1]), per entry the goods id (words at +0x48), its stock (+8 ints: 0x50ab50) and its price (+0x68: 0x50aaf0); out[0x2d] the building's market object. kind 0: the objects that are not 0x17 / 0x25; else those. 0 when it has no such object (0xfd). */

int gm_BuildingGoodsList(char *b, unsigned *out, int kind)
{
    char k;
    short *l;
    unsigned short *o;
    unsigned v;
    int i;

    for (i = 0; i < 0x2e; i++)
        out[i] = 0;
    if (gm_Query(*(void **)(b + 0x5d), 1, 0, 0xfd) == 0)
        return 0;
    l = (short *)gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a);
    if (l == 0 && (l = (short *)gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x116)) == 0)
        return 0;
    if (kind == 0) {
        v = *(unsigned char *)((char *)l + 0x1d);
        out[0] = v;
        if (*l == 0x116) {
            if (v <= *(unsigned char *)(l + 0xe))
                v = *(unsigned char *)(l + 0xe);
            out[0] = v;
        }
    } else {
        if (*l == 0x116)
            return 0;
        v = *(unsigned char *)(l + 0xe);
        out[0] = v;
    }
    o = (unsigned short *)gm_Query(*(void **)(l + 10), 1, 5);
    while (o) {
        k = ai_Goods[*o * 0x41];
        if ((((k == 0x17 || k == 0x25) && kind != 0) || ((k != 0x17 && k != 0x25) && kind == 0)) || *l == 0x116) {
            v = gm_ListAmount(l, o);
            out[out[1] + 0x1a] = v;
            v = gm_ListStock(l, *o);
            out[out[1] + 2] = v;
            *(unsigned short *)((char *)out + out[1] * 2 + 0x48) = *o;
            out[1]++;
        }
        o = (unsigned short *)gm_QueryNext();
    }
    out[0x2d] = (unsigned)l;
    return 1;
}
