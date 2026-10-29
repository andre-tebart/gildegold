/* 0x497660, decide: whether a good (o1, kind 2) can be made from what o2 holds (action 11) */

int ai_Unk497660(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIOBJ *obj;
    AIBLD *bld;
    AIPER *c;
    AIBTX *t;
    int i, n, v;
    unsigned short k;

    if (b == 1 && o1->kind == 2 && !AI_GOODX[o1->unk04].unk40 && AI_GOODX[o1->unk04].type != 2) {
        c = 0;
        bld = 0;
        obj = 0;
        if (o2->kind == 1) {
            obj = (AIOBJ *)gm_FindObjectInst(o2->unk04);
            if (!obj)
                return 0;
            if (AI_GOODX[obj->id].type == 2 || AI_GOODX[obj->id].type == 6) {
                if (!gm_Query(obj->unk14, 1, 0, o1->unk04) && (bld = gm_FindBuildingById(obj->unk06)) != 0) {
                    t = &AI_BTX[bld->type];
                    n = t->n;
                    for (i = 0; i < n; i++)
                        if ((t->k[i] & 0x7fff) == obj->id)
                            break;
                    if (i < n) {
                        for (i++; i < n; i++)
                            if ((t->k[i] & 0x7fff) == o1->unk04)
                                break;
                        if (i < n && (!t->p[i].a || gm_Query(obj->unk14, 1, 0, t->p[i].a))) {
                            k = t->p[i].b;
                            if (!k || gm_Query(obj->unk14, 1, 0, k))
                                goto rate;
                        }
                    }
                }
                return 0;
            }
        } else if (o2->kind == 4) {
            bld = gm_FindBuildingById(o2->unk04);
            if (bld && !gm_Query(bld->objList, 1, 0, o1->unk04)) {
                t = &AI_BTX[bld->type];
                n = t->n;
                for (i = 0; i < n; i++)
                    if ((t->k[i] & 0x7fff) == o1->unk04)
                        break;
                if (i < n && (!t->p[i].a || gm_Query(bld->objList, 1, 0, t->p[i].a))) {
                    k = t->p[i].b;
                    if (!k || gm_Query(bld->objList, 1, 0, k))
                        goto rate;
                }
            }
            return 0;
        } else if (o2->kind == 7) {
            c = AI_505DF0(o2->unk04);
            if (!c)
                return 0;
        } else
            return 0;
rate:
        if (a != 0x39) {
            if (obj)
                c = gm_Unk509ff0(obj);
            else if (bld) {
                if (bld->owner == 0xffff)
                    return 0;
                c = (AIPER *)&ai_Chars[bld->owner];
            } else if (!c)
                return 0;
            if (!c)
                return 0;
            v = gm_Unk509ee0(c, ai_OwnPlayer);
            o1->unk10 = 0;
        } else {
            v = o2->unk10;
            o1->unk10 = 0x39;
        }
        if (o1->unk14 > 0.0f && o1->unk14 < 1.0f)
            v = (int)(v * o1->unk14);
        return v < (int)gm_GoodsPrice(o1->unk04, ai_OwnPlayer) ? 0 : 11;
    }
    return 0;
}
