/* 0x491860, decision 10 */

int ai_Unk491860(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    float f;
    int t, n, m, k;

    t = gm_TitleGroup(p->jobId);
    k = 0;
    if (ai_TimeT.year >= 10) {
        if (gm_BuildQuery(1, 5, 0x17))
            k = 1;
        if (gm_BuildQuery(1, 5, 0x18))
            k++;
        if (gm_BuildQuery(1, 5, 0x19))
            k++;
        if (gm_BuildQuery(1, 5, 0x1a))
            k++;
        if (k) {
            gv_GetTableRecord(10, &d);
            f = ord_BuildingCapacityRatio();
            k = d.max - d.min;
            if (f < 0.33f)
                n = (int)(k * 0.22f + d.min);
            else if (f < 0.66)
                n = (int)(k * 0.44f + d.min);
            else
                n = (int)(k * 0.66f + d.min);
            if (t == 1 || t == 2 || t == 3 || t == 5)
                n--;
            b = gm_FirstBuilding(1);
            o = gm_Query(b->objList, 2, 6, 0, 0x115);
            m = gm_Unk509e30(o->unk14, ai_OwnPlayer);
            if (m < amt_GetOwnedOfficePay() * 3)
                n += util_Random(3) + 1;
            if (d.cur - 3 > n) {
                *out = d.cur - util_Random(2) - 1;
                *out = max(d.min, *out);
                return 1;
            }
            if (d.cur + 3 < n) {
                *out = util_Random(2) + d.cur + 1;
                *out = min(d.max, *out);
                return 1;
            }
        }
    }
    return 0;
}
