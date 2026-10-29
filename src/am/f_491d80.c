/* 0x491d80, decision 15 */

int ai_Unk491d80(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    int m, k, n;

    if (ai_Time.year < 4)
        return 0;
    gv_GetTableRecord(15, &d);
    ord_BuildingCapacityRatio();
    b = gm_FirstBuilding(1);
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    m = gm_Unk509e30(o->unk14, ai_OwnPlayer);
    k = amt_GetOwnedOfficePay();
    if (m < k * 3)
        n = 4;
    else if (m > k * 12)
        n = 0;
    else if (m > k * 8)
        n = 1;
    else if (m > k * 6)
        n = 2;
    else
        n = 3;
    if (abs(d.cur - n) < 2)
        return 0;
    if (n > d.cur)
        n = d.cur + 1;
    else if (n < d.cur)
        n = d.cur - 1;
    *out = max(min(n, d.max), d.min);
    return 1;
}
