/* 0x4921d0, decision 19 */

int ai_Unk4921d0(AIPER *p, int a, int *out)
{
    AIDEC d;
    float f, x, u, w;
    int n;

    gm_TitleGroup(p->jobId);
    f = gm_GetSkill(p, 4);
    n = ord_GetRatingByte(4);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    x *= 0.8f;
    gv_GetTableRecord(19, &d);
    if (x < 1.0f && d.cur == 0) {
        if (u > w) {
            *out = 1;
            return 1;
        }
    } else if (x > 1.0 && d.cur == 1 && w > u) {
        *out = 0;
        return 1;
    }
    return 0;
}
