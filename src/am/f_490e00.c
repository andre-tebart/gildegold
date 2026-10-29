/* 0x490e00, decision 0 */

int ai_Unk490e00(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, v, w;
    int t, n, r;

    t = gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 4);
    n = ord_GetRatingByte(4);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    r = unk_Unk4f39e0(p->netId);
    if (t == 0xb || t == 0xc)
        x += 6.0f;
    else if (t == 9 || t == 7)
        x += 2.0f;
    else
        x += 4.0f;
    x = r / x;
    gv_GetTableRecord(0, &d);
    if (x > 1.0f) {
        if (w > u && d.cur < 2) {
            *out = util_Random(2) + d.cur + 1;
            return 1;
        }
        if (x > 1.4f) {
            *out = 4;
            return 1;
        }
    } else if (u > v && d.cur > 2) {
        *out = d.cur - util_Random(3) - 1;
        return 1;
    }
    return 0;
}
