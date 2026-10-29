/* 0x490fc0, decision 1 */

int ai_Unk490fc0(AIPER *p, int a, int *out)
{
    AIDEC d;
    unsigned short *o;
    float f, x, u, v, w, g;
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
        x += 7.0f;
    else if (t == 9 || t == 1 || t == 3 || t == 2)
        x += 3.0f;
    else
        x += 5.0f;
    n = 40 - p->unk00a;
    x = r / (n * 0.1f + x);
    gv_GetTableRecord(1, &d);
    if (x > 1.0f && d.cur == 0) {
        if (x > 1.4f || w > u) {
            *out = 1;
            return 1;
        }
    } else if (x < 0.5f && d.cur == 1 && (v > u || p->unk166 == 0x11)) {
        *out = 0;
        return 1;
    } else {
        o = amt_GetOfficeHolder(0x11);
        if (o) {
            g = gm_Liking(*o, p->id, 1);
            if (d.cur == 1) {
                if (g > 0.75f) {
                    *out = 0;
                    return 1;
                }
            } else if (d.cur == 0 && g < 0.25f) {
                *out = 1;
                return 1;
            }
        }
    }
    return 0;
}
