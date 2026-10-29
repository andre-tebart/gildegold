/* 0x509b10, walks the building b's object words from +0x25: for a kind 2 / 6 object found in the instance, the price (0x5087f0) of the next one that is not 2 / 6 is added up. ROUGH */

int gm_EquipmentValue(unsigned char *b, int unused)
{
    char k;
    int found, i;
    unsigned id;
    unsigned short *l, *o, *w;
    float tot;

    l = 0;
    found = 0;
    i = 0;
    tot = 0.0f;
    w = (unsigned short *)(ai_BTypes + (unsigned)*b * 0x24d + 0x25);
    do {
        if (w[-1] == 0)
            break;
        id = w[-1] & 0x7fff;
        if (found) {
            if (l != 0) {
                o = (unsigned short *)gm_Query(*(void **)(l + 10), 1, 0, id);
                if (o != 0)
                    tot += gm_ItemPrice(*o, *(unsigned char *)(o + 9));
                if (i < 0x3f && ((k = ai_Goods[(*w & 0x7fff) * 0x41]) == 2 || k == 6))
                    found = 0;
            }
        } else {
            k = ai_Goods[id * 0x41];
            if (k == 2 || k == 6) {
                l = (unsigned short *)gm_Query(*(void **)(b + 0x5d), 1, 0, id);
                if (l != 0) {
                    tot += gm_ItemPrice(*l, *(unsigned char *)(l + 9));
                    if (i < 0x3f && (k = ai_Goods[(*w & 0x7fff) * 0x41]) != 2 && k != 6)
                        found = 1;
                }
            }
        }
        i++;
        w++;
    } while (i < 0x40);
    return (int)tot;
}
