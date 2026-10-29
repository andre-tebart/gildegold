/* 0x491560, decision 5 */

int ai_Unk491560(AIPER *p, int a, int *out)
{
    AIDEC d;
    int n, t;

    n = min((int)((3.0f - p->mood[3][0]) * 5.0f), 4);
    t = gm_TitleGroup(p->jobId);
    if ((t == 8 || t == 9) && n > 1)
        n = 1;
    gv_GetTableRecord(5, &d);
    if (n != d.cur) {
        *out = n;
        return 1;
    }
    return 0;
}
