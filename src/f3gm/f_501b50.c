/* 0x501b50, the main object of the building b by its building class (0x501d80): classes 1 / 7 / 8: by type 0x1d the object 0x205, 0x16 the 0x12e, 4 the 0x54, 0x1f the 0xfd; class 4: 0xf7; homes (0x502520): 0xfd; else the first object of the kind 2 (not 0xfd) in the building's prototype list. ROUGH */

int gm_BuildingMainObject(unsigned char *b)
{
    int cls, i, o, id;
    char *k;
    unsigned short *w;
    unsigned u;

    o = 0;
    cls = gm_BuildingClass(*b);
    k = ai_BTypes + *b * 0x24d;
    if (cls != 1 && cls != 7 && cls != 8) {
        if (cls == 4)
            return (int)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xf7);
        if (gm_IsHomeType(b) == 0) {
            w = (unsigned short *)(k + 0x23);
            i = 0;
            while (*w == 0xffff || (u = *w & 0x7fff, ai_Goods[u * 0x41] != 2) || u == 0xfd) {
                i++;
                w++;
                if (i > 0x3f)
                    return 0;
            }
            return (int)gm_Query(*(int *)(b + 0x5d), 1, 0, u);
        }
        u = 0xfd;
        goto done;
    }
    if (*k == 0x1d) {
        id = 0x205;
    use_list:
        o = (int)gm_Query(*(int *)(b + 0x5d), 1, 0, id);
        if (o != 0)
            return o;
    } else {
        if (*k == 0x16) {
            id = 0x12e;
            goto use_id;
        }
        if (*k == 4) {
            id = 0x54;
            goto use_id;
        }
        if (*k == 0x1f) {
            id = 0xfd;
            goto use_list;
        }
    }
    goto rest;
use_id:
    o = (int)gm_Query(*(int *)(b + 0x5d), 1, 0, id);
    if (o != 0)
        return o;
rest:
    w = (unsigned short *)(k + 0x23);
    i = 0;
    while (*w == 0xffff || (u = *w & 0x7fff, ai_Goods[u * 0x41] != 2) || u == 0xfd) {
        i++;
        w++;
        if (i > 0x3f)
            return o;
    }
done:
    return (int)gm_Query(*(int *)(b + 0x5d), 1, 0, u);
}
