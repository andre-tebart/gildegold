/* 0x50f0b0, the opening objects of the buildings of the types 0x20 / 0x1f / 0x1e: for each building the objects 0xff / 0xfe are made (0x500190) and the goods of the type (0x1b7 .. 0x1c0) are put into them (0x4c4880), the first one with a random extra amount */


void init_CreateOpeningObjects(void)
{
    int types[4];
    int *tp;
    int t;
    INIT_BLD *b;
    INIT_OBJ *o1, *o2;
    int i, g, k;
    int goods[8];

    types[0] = 0x20;
    t = 0x20;
    types[1] = 0x1f;
    types[2] = 0x1e;
    types[3] = 0;
    tp = types;
    do {
        b = (INIT_BLD *)gm_BuildQuery(1, 0, t);
        while (b != 0) {
            o1 = (INIT_OBJ *)gm_Query(b->objList, 1, 0, 0xff);
            o2 = (INIT_OBJ *)gm_Query(b->objList, 1, 0, 0xfe);
            if (o1 == 0)
                o1 = (INIT_OBJ *)gm_CreateObject(b->id, 0xff, 1);
            if (o2 == 0)
                o2 = (INIT_OBJ *)gm_CreateObject(b->id, 0xfe, 1);
            for (i = 0; i < 8; i++)
                goods[i] = 0;
            switch (t) {
            case 0x1e:
                goods[1] = 0x1b8;
                goods[0] = 0x1b7;
                goods[2] = 0x1b9;
                break;
            case 0x1f:
                goods[1] = 0x1bb;
                goods[0] = 0x1ba;
                goods[2] = 0x1bc;
                break;
            case 0x20:
                goods[1] = 0x1bf;
                goods[0] = 0x1bd;
                goods[2] = 0x1be;
                goods[3] = 0x1c0;
                break;
            }
            i = 0;
            do {
                g = goods[i];
                if (g != 0) {
                    if (b->type == 0x1e) {
                        cm_SendExSellObject(o1->objId, -1, g, 2, ai_OwnPlayer, 0);
                    } else {
                        cm_SendExSellObject(o1->objId, -1, g, 2 - (i != 0), ai_OwnPlayer, 0);
                        if (i == 0) {
                            cm_SendExSellObject(o2->objId, -1, goods[0], (util_RandomU(0x14) & 0xffff) + 0x14, ai_OwnPlayer, 0);
                        }
                    }
                    switch (g) {
                    case 0x1bd:
                        k = 0x1d0;
                        break;
                    case 0x1bf:
                        k = 0x1d2;
                        break;
                    case 0x1be:
                        k = 0x1d1;
                        break;
                    case 0x1c0:
                        k = 0x1d3;
                        break;
                    case 0x1b7:
                        k = 0x1ca;
                        break;
                    case 0x1b8:
                        k = 0x1cb;
                        break;
                    case 0x1b9:
                        k = 0x1cc;
                        break;
                    case 0x1bb:
                        k = 0x1ce;
                        break;
                    case 0x1bc:
                        k = 0x1cf;
                        break;
                    default:
                        goto next;
                    }
                    cm_SendExSellObject(o1->objId, -1, k, 2 - (i != 0), ai_OwnPlayer, 0);
                }
            next:
                i++;
            } while (i < 8);
            b = (INIT_BLD *)gm_BuildNext();
        }
        tp++;
        t = *tp;
    } while (t != 0);
}
