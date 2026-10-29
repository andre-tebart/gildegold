/* 0x50aa00, the list of the stall object 0x1dd of the object c: out[0] the capacity, out[1] the number of entries, per entry the amount (+0x68), the stock (0x50ab50) .., out[0x2a] / [0x2b] the sums, out[0x2c] the last price. ROUGH */


int gm_StallGoodsList(char *c, unsigned *out)
{
    int i;
    char *l;
    unsigned short *o;
    unsigned v;

    for (i = 0; i < 0x2e; i++)
        out[i] = 0;
    l = gm_Query(*(void **)(c + 0x14), 1, 0, 0x1dd);
    if (l != 0) {
        o = (unsigned short *)gm_Query(*(void **)(l + 0x14), 1, 5);
        out[0] = (unsigned char)l[0x1c];
        while (o) {
            out[out[1] + 0x1a] = *(unsigned *)(o + 7);
            v = gm_ListStock((short *)l, *o);
            out[out[1] + 2] = v;
            out[0x2a] += out[out[1] + 2];
            out[0x2b] += *(unsigned *)(o + 7);
            *(unsigned short *)((char *)out + out[1] * 2 + 0x48) = *o;
            out[1]++;
            out[0x2c] = (int)gm_ItemPrice(*o, 100);
            o = (unsigned short *)gm_QueryNext();
        }
        out[0x2d] = (unsigned)l;
        return 1;
    }
    return 0;
}
