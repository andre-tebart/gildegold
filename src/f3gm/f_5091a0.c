/* 0x5091a0, the value of the building b: out[0] the price of its owner's estate (0x50a640 of the owner capped at 2048000, 7 % for the kind 2 else 3 %), out[1] the sum of the prices of the objects of its first kind 0x2a object. ROUGH */

int gm_Unk5091a0(unsigned char *b, int *out)
{
    float f, tot;
    char *p, *l, *o;
    int v;
    unsigned short id;

    f = 0.0f;
    p = ai_BTypes + (unsigned)*b * 0x24d;
    id = *(unsigned short *)(b + 0x27);
    if (id != 0xffff) {
        f = I50A640(id);
        if (f > 2048000.0f)
            f = 2048000.0f;
    }
    if (*p == 2)
        f *= 0.07f;
    else
        f *= 0.03f;
    if (f < 0.0f)
        f = 0.0f;
    out[0] = (int)f;
    tot = 0.0f;
    l = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x2a);
    if (l) {
        o = gm_Query(*(void **)(l + 0x14), 1, 5);
        while (o) {
            v = (int)(gm_ListAmount(l, o) * 0.3);
            if (v < 1)
                v = 1;
            tot = gm_ItemPrice(*(unsigned short *)o, 100) * v + tot;
            o = (char *)gm_QueryNext();
        }
    }
    out[1] = (int)tot;
    return 0;
}
