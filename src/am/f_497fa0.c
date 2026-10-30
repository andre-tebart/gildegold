/* 0x497fa0, decide: pay off a building's job when it is cheap enough (action 16) */

int ai_Unk497fa0(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];
    int ok[4];
    AIBLDX *bl;
    AIJOB *j;
    AIBTYPE *bt;
    unsigned int lo, hi;
    int i, n, lim, v, r;

    if (p->kind == 3)
        return 0;
    memset(buf, 0, sizeof(buf));
    buf[0] = 1;
    if (b == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (b == 1) {
        if (o1->kind != 5 && o1->kind != 4)
            return 0;
    }
    if (he_FindEvent(2, 0, 0xd, 2, p->id))
        return 0;
    bl = AI_BLDX;
    for (i = 0; i < 0x100; i++, bl++) {
        if (bl->type) {
            lo = 0;
            hi = 0;
            bt = &ai_BTypesT[bl->type];
            if ((unsigned char)bt->kind > 0x1f)
                hi = 1 << ((unsigned char)bt->kind - 0x20);
            else
                lo = 1 << (unsigned char)bt->kind;
            if (bl->owner == p->id && ((lo & ai_BuildingTypeMasks[2]) || (hi & ai_BuildingTypeMasks[3])))
                goto found;
        }
    }
    return 0;
found:
    lim = (int)(gm_Unk509ee0(p, ai_OwnPlayer) * 0.66f);
    r = ord_DispatchOrder(1, p, buf);
    if (r == 2 || r == 4)
        ok[0] = 1;
    else
        ok[0] = 0;
    r = ord_DispatchOrder(5, p, buf);
    if (r == 2 || r == 4)
        ok[1] = 1;
    else
        ok[1] = 0;
    r = ord_DispatchOrder(9, p, buf);
    if (r == 2 || r == 4)
        ok[2] = 1;
    else
        ok[2] = 0;
    r = ord_DispatchOrder(0xc, p, buf);
    if (r == 2 || r == 4)
        ok[3] = 1;
    else
        ok[3] = 0;
    if (p->bld2) {
        n = AI_BTX[p->bld2->type].unk247;
        for (i = 0; i <= n - 1; i++)
            ok[i] = 0;
    }
    if (o1->kind == 0) {
        for (bl = AI_501190_2(1, 6); o1->kind == 0 && bl; bl = AI_501030()) {
            if (ai_BTypesT[bl->type].kind == 2 && (bl->flags5a & 2)
                && ((bl->type == 4 && ok[0]) || (bl->type == 5 && ok[1]) || (bl->type == 6 && ok[2])
                    || (bl->type == 7 && ok[3]))) {
                for (j = he_FindEvent(1, 0, 0x1b); j; j = he_NextEvent()) {
                    if (j->unkac == bl->id) {
                        v = ((AIJOBX *)j)->unkb0;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            if ((float)(v - lim) / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->unk08 = v - lim;
                                return ai_Unk49eec0(0x10, p, b, o1, o2);
                            }
                            return 0;
                        }
                        o1->kind = 4;
                        o1->unk04 = bl->id;
                        o1->unk08 = 1;
                        break;
                    }
                }
            }
        }
        if (o1->kind != 0)
            return 16;
        return 0;
    }
    if (o1->kind == 5) {
        if (o1->unk04 == 4 && !ok[0])
            return 0;
        if (o1->unk04 == 5 && !ok[1])
            return 0;
        if (o1->unk04 == 6 && !ok[2])
            return 0;
        if (o1->unk04 == 7 && !ok[3])
            return 0;
        for (bl = AI_501190_2(1, 6); o1->kind == 5 && bl; bl = AI_501030()) {
            if ((unsigned char)ai_BTypesT[bl->type].kind == o1->unk04) {
                for (j = he_FindEvent(1, 0, 0x1b); j; j = he_NextEvent()) {
                    if (j->unkac == bl->id) {
                        v = ((AIJOBX *)j)->unkb0;
                        if (lim < v) {
                            r = gm_EquipmentValue(bl, bl->level);
                            if ((float)(v - lim) / v < (float)((float)(v - r) / r * -0.5f) + 0.25f) {
                                o1->kind = 0x12;
                                o1->unk08 = v - lim;
                                return ai_Unk49eec0(0x10, p, b, o1, o2);
                            }
                            return 0;
                        }
                        o1->kind = 4;
                        o1->unk04 = bl->id;
                        o1->unk08 = 1;
                        break;
                    }
                }
            }
        }
        if (o1->kind != 5)
            return 16;
        return 0;
    }
    return 0;
}
