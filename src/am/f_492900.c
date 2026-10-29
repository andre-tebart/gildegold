/* 0x492900, decision 25 */

int ai_Unk492900(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 3);
    n = ord_GetRatingByte(3);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    if (t == 0xc)
        x *= 1.3f;
    else
        x *= 0.9f;
    gv_GetTableRecord(25, &d);
    if (x < 1.0f && d.cur == 0) {
        if (w > u) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && v > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
