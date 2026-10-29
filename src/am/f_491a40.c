/* 0x491a40, decision 11 */

int ai_Unk491a40(AIPER *p, int a, int *out)
{
    AIDEC d;
    int t, n, k;

    t = gm_TitleGroup(p->jobId);
    gv_GetTableRecord(11, &d);
    k = d.max - d.min;
    if (t == 7) {
        n = (int)((p->mood[3][0] - 2.0) * k + d.min);
        n = min(max(d.min, n), d.max);
        if (n > d.cur) {
            *out = n;
            return 1;
        }
    } else {
        n = (int)((3.0f - p->mood[3][0]) * k + d.min);
        n = min(max(d.min, n), d.max);
        if (n < d.cur) {
            *out = n;
            return 1;
        }
    }
    return 0;
}
