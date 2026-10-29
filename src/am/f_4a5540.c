/* 0x4a5540: the first good of building type b->type that b's store does not hold yet while it holds both of its
   two ingredients (slot 0x40 + 2i, 0x41 + 2i); goods of kind 2 / 6 need a free cart (list 4) or one carrying it.
   *item gets the good, *obj the cart's id (-1 for none) */

int ai_Unk4a5540(int *item, int *obj, AIBLD *b, int f)
{
    AIBTYPE *t;
    unsigned short *s;
    short *in;
    AIOBJ *o, *cart;
    int list, n, i, found;
    char k;

    list = b->objList;
    t = &ai_BTypes[b->type];
    cart = 0;
    i = 0;
    if (t->nslots == 0)
        return 0;
    in = (short *)&t->slot[0x41];
    s = t->slot;
    do {
        k = ai_Goods[*s & 0x7fff].type;
        if (k != '!') {
            if (k == 2 || k == 6) {
                n = 0;
                found = 0;
                cart = 0;
                for (o = gm_Query(b->objList, 1, 4, 2); o != 0; o = gm_QueryNext()) {
                    if (o->id != 0xfd)
                        n++;
                    if (o->id == (*s & 0x7fff)) {
                        list = *(int *)((unsigned short *)o + 10);
                        found = 1;
                        cart = o;
                        break;
                    }
                }
                if (n < (unsigned char)t->pad001[0x20] && !found) {
                    for (o = gm_Query(b->objList, 1, 4, 6); o != 0; o = gm_QueryNext()) {
                        if (o->id == (*s & 0x7fff)) {
                            list = *(int *)((unsigned short *)o + 10);
                            found = 1;
                            cart = o;
                            break;
                        }
                    }
                    if (n < (unsigned char)t->pad001[0x20] && !found)
                        goto check;
                }
            } else {
check:
                if (gm_Query(list, 1, 0, *s & 0x7fff) == 0
                    && (in[-1] == 0 || gm_Query(list, 1, 0, in[-1]) != 0)
                    && (in[0] == 0 || gm_Query(list, 1, 0, in[0]) != 0)) {
                    *item = t->slot[i] & 0x7fff;
                    if (cart != 0) {
                        *obj = *(int *)((unsigned short *)cart + 1);
                        return 1;
                    }
                    *obj = -1;
                    return 1;
                }
            }
        }
        in += 2;
        i++;
        s++;
    } while (i < t->nslots);
    return 0;
}
