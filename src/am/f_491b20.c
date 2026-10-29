/* 0x491b20, decision 12 */

int ai_Unk491b20(AIPER *p, int a, int *out)
{
    AIDEC d;
    int t, k;

    t = gm_TitleGroup(p->jobId);
    if (ai_Time.year >= 4) {
        gv_GetTableRecord(12, &d);
        k = d.max - d.min;
        if (t == 8) {
            if (d.cur > (int)(k * 0.33f + d.min)) {
                *out = util_Random(3) + d.min;
                return 1;
            }
        } else if (d.cur < (int)(k * 0.66f + d.min)) {
            *out = d.max - util_Random(3);
            return 1;
        }
    }
    return 0;
}
