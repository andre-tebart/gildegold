/* 0x4ab2c0, execute callback "buy dunkle ecke": p buys item a[1] for a[4] in the shop b if it still sells it */

int ai_Unk4ab2c0(AIPER *p, int *a, int *b)
{
    int rec[5];
    AIBLD *bld;
    AIOBJ *o;
    int *q;
    int i;

    bld = gm_BuildQuery(1, 1, b[1]);
    if (bld == 0)
        return 0;
    o = gm_Query(bld->objList, 1, 0, 0x120);
    if (o == 0)
        return 0;
    o = gm_Query(o->list, 1, 0, 300);
    if (o == 0)
        return 0;
    i = 0;
    q = (int *)((unsigned char *)o + 0x1c);
    rec[3] = a[1];
    do {
        if (*q == rec[3])
            break;
        i++;
        q++;
    } while (i < 3);
    if (i == 3)
        return 0;
    rec[0] = 0x62757920;
    rec[2] = bld->id;
    rec[1] = p->netId;
    rec[4] = a[4];
    cm_BeginGroup("buy dunkle ecke");
    ai_Unk4c5d40(rec);
    cm_SendProofExecuted(a[1], p->netId);
    cm_SendMoveGoodsUnchecked(((AIPER *)ai_Chars)[bld->owner].netId, p->netId, 0, a[4]);
    cm_EndGroup("buy dunkle ecke");
    return 1;
}
