/* 0x509f80, the price of the goods of the building b's first kind 0x2a object (the last one counts) */

int gm_Unk509f80(char *b)
{
    int v;
    char *l;
    unsigned short *o;

    v = 0;
    l = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a);
    if (l) {
        o = (unsigned short *)gm_Query(*(void **)(l + 0x14), 1, 5);
        while (o) {
            v = (int)gm_ItemPrice(*o, *(unsigned char *)(o + 9));
            o = (unsigned short *)gm_QueryNext();
        }
    }
    return v;
}
