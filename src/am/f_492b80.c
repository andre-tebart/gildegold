/* 0x492b80, decision 8: a graveyard (building types 0x4b..0x4d) */

int ai_Unk492b80(AIPER *p, int a, int *out)
{
    AIDEC d;
    AIBLD *b;
    float f, x, u, v, w;
    int n, i, found, own;

    f = gm_GetSkill(p, 2);
    n = ord_GetRatingByte(2);
    x = (f - (n + 1.0f) / 252.0f) / 42.0f;
    u = (p->mood[9][0] - 2.0) * p->mood[9][2];
    v = (p->mood[10][0] - 2.0) * p->mood[10][2];
    w = (p->mood[11][0] - 2.0) * p->mood[11][2];
    found = 0;
    own = 0;
    b = ai_BldsT;
    for (i = 0; i < 256; i++, b++) {
        if (b->type == 0x4b || b->type == 0x4c || b->type == 0x4d) {
            found++;
            if (p->kind != 5)
                break;
            if (b->owner == p->id) {
                own = 1;
                break;
            }
        }
    }
    if (!found)
        return 0;
    gv_GetTableRecord(8, &d);
    if (p->kind == 5) {
        if (own) {
            if (d.cur == 1) {
                *out = 0;
                return 1;
            }
        } else if (d.cur == 0) {
            *out = 1;
            return 1;
        }
        return 0;
    }
    if (p->kind == 3)
        x *= 1.2f;
    else
        x *= 0.8f;
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
