/* 0x491650, decision 7 */

int ai_Unk491650(AIPER *p, int a, int *out)
{
    AIDEC d;
    int n;

    gm_TitleGroup(p->jobId);
    n = min((int)((p->mood[3][0] - 2.0f) * 5.0f), 4);
    n = max(1, n);
    gv_GetTableRecord(7, &d);
    if (n != d.cur) {
        *out = n;
        return 1;
    }
    return 0;
}
