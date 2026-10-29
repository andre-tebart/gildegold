/* 0x5099e0, the same list with the other loop shape (the kinds 0x17 only when kind != 0, the others only when kind == 0) */

int gm_Unk5099e0(char *b, unsigned *out, int kind)
{
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
    do {
        if (o == 0) {
            out[0x2d] = (unsigned)l;
            return 1;
        }
        if (ai_Goods[*o * 0x41] == 0x17) {
            if (kind != 0) {
                v = gm_ListAmount(l, o);
                out[out[1] + 0x1a] = v;
                v = gm_ListStock(l, *o);
                out[out[1] + 2] = v;
                *(unsigned short *)((char *)out + out[1] * 2 + 0x48) = *o;
                out[1]++;
            }
        } else if (kind == 0) {
            v = gm_ListAmount(l, o);
            out[out[1] + 0x1a] = v;
            v = gm_ListStock(l, *o);
            out[out[1] + 2] = v;
            *(unsigned short *)((char *)out + out[1] * 2 + 0x48) = *o;
            out[1]++;
        }
        o = (unsigned short *)gm_QueryNext();
    } while (1);
}
