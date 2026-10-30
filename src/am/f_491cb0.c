/* 0x491cb0, decision 14 */

int ai_Unk491cb0(AIPER *p, int a, int *out)
{
    AIDEC d;
    int t, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_TimeT.year >= 4) {
        gv_GetTableRecord(14, &d);
        k = d.max - d.min;
        if (t == 9) {
            if (d.cur < (int)(k * 0.55f + d.min)) {
                *out = d.max - util_Random(6);
                return 1;
            }
        } else if (d.cur > (int)(k * 0.45f + d.min)) {
            *out = util_Random(6) + d.min;
            return 1;
        }
    }
    return 0;
}
