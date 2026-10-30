/* 0x4aaef0, decide callback: a building of p (type with cap > 1) with a free workplace object (lists 0x120 / 300)
   and no job 0x74 yet, into a (kind 4) and b (kind 1) */

int ai_Unk4aaef0(AIPER *p, unsigned char *a, unsigned char *b)
{
    AIBLD *bld;
    AIOBJ *o, *o2;

    for (bld = gm_BuildQuery(2, 4, p->id, 5, 0x16); bld != 0; bld = gm_BuildNext()) {
        if (ai_BTypesT[bld->type].cap247 > 1 && (o = gm_Query(bld->objList, 1, 0, 0x120)) != 0
            && (o2 = gm_Query(o->list, 1, 0, 300)) != 0 && he_FindEvent(2, 0, 0x74, 3, bld->id) == 0) {
            a[0] = 4;
            *(int *)(a + 4) = bld->id;
            b[0] = 1;
            *(int *)(b + 4) = o->objId;
            return 1;
        }
    }
    return 0;
}
