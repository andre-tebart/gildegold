/* 0x491e80, decision 16 */

int ai_Unk491e80(AIPER *p, int a, int *out)
{
    AIDEC d;
    float x, u, w;
    int t;

    t = gm_TitleGroup(p->jobId);
    x = max(p->mood[6][0] - 2.0, p->mood[7][0] - 2.0);
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xb || t == 0xc)
        x *= 1.2f;
    else if (t == 7)
        x *= 0.8f;
    else
        x *= 1.1f;
    gv_GetTableRecord(16, &d);
    if (x < 0.5f && d.cur == 0) {
        if (u > w) {
            *out = 1;
            return 1;
        }
    } else if (x > 0.5 && d.cur == 1 && w > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
