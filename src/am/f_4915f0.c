/* 0x4915f0, decision 6 */

int ai_Unk4915f0(AIPER *p, int a, int *out)
{
    AIDEC d;
    int n;

    n = min((int)((p->mood[3][0] - 2.0f) * 5.0f), 4);
    gv_GetTableRecord(6, &d);
    if (n != d.cur) {
        *out = n;
        return 1;
    }
    return 0;
}
