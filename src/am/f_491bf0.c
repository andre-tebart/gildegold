/* 0x491bf0, decision 13 */

int ai_Unk491bf0(AIPER *p, int a, int *out)
{
    AIDEC d;
    float x;
    int k, n;

    x = max(min((3.0f - p->mood[3][0]) * (p->unk01c / 700.0f), 1.0f), 0.0f);
    gv_GetTableRecord(13, &d);
    k = d.max - d.min;
    n = (int)(k * x + d.min);
    if (abs(n - d.cur) > 3) {
        *out = n;
        return 1;
    }
    return 0;
}
