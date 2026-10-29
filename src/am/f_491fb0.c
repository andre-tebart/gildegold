/* 0x491fb0, decision 17 */

int ai_Unk491fb0(AIPER *p, int a, int *out)
{
    AIDEC d;
    float x, u, w;
    int t;

    t = gm_TitleGroup(p->jobId);
    x = (float)(p->mood[10][0] - 2.0);
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 5 || t == 6)
        x *= 1.5f;
    else if (t == 7)
        x *= 0.9f;
    gv_GetTableRecord(17, &d);
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
