/* 0x4916d0, decision 9 */

int ai_Unk4916d0(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    AIOBJ *o;
    float f;
    int t, n, m, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_TimeT.year >= 4) {
        gv_GetTableRecord(9, &d);
        f = ord_BuildingCapacityRatio();
        k = d.max - d.min;
        if (f < 0.33f)
            n = (int)(k * 0.22f + d.min);
        else if (f < 0.66)
            n = (int)(k * 0.44f + d.min);
        else
            n = (int)(k * 0.66f + d.min);
        if (t != 1 && t != 2 && t != 3 && t != 5)
            n--;
        b = gm_FirstBuilding(1);
        o = gm_Query(b->objList, 2, 6, 0, 0x115);
        m = gm_Unk509e30(o->unk14, ai_OwnPlayer);
        if (m < (int)(amt_GetOwnedOfficePay() * 0.25f) * 3)
            n += util_Random(3) + 1;
        if (d.cur - 3 > n) {
            *out = d.cur - util_Random(3) - 1;
            *out = max(d.min, *out);
            return 1;
        }
        if (d.cur + 3 < n) {
            *out = util_Random(3) + d.cur + 1;
            *out = min(d.max, *out);
            return 1;
        }
    }
    return 0;
}
