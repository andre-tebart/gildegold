/* 0x4957d0, chooses the next action of character i: the best short-term and the best long-term rating */

int ai_ChooseAction(int i)
{
    float best1, best2, a, b;
    int k;
    unsigned int id2, id1;
    int r1, r2, r;
    AIARG s2, s1, b2s1, b1s2, b1s1, b2s2;
    G5ACT *act;
    G5CH *p;

    best1 = -1e30f;
    best2 = -1e30f;
    id1 = 0;
    id2 = 0;
    r1 = 0;
    r2 = 0;
    b1s1.kind = 0;
    b2s1.kind = 0;
    b1s2.kind = 0;
    b2s2.kind = 0;
    p = &G5_CHARS[i];
    k = 0;
    for (act = G5_ACTS; act < G5_ACTS + 0x49; act++, k++) {
        if (act->id != 0 && act->id != p->act && (act->flags & p->mask) != 0) {
            s1.kind = 0;
            s1.unk04 = 0;
            s1.unk08 = 0;
            s2.kind = 0;
            s2.unk04 = 0;
            s2.unk08 = 0;
            if (act->eval != 0 && act->eval(&a, &b, k, p, 0, &s1, &s2) != 0 && (best1 <= a || best2 <= b) &&
                act->decide != 0 && (r = act->decide(0, p, 0, &s1, &s2)) != 0) {
                if (best1 <= a) {
                    b1s1 = s1;
                    best1 = a;
                    id1 = act->id;
                    b1s2 = s2;
                    r1 = r;
                }
                if (best2 <= b) {
                    best2 = b;
                    id2 = act->id;
                    b2s1 = s1;
                    b2s2 = s2;
                    r2 = r;
                }
            }
        }
    }
    if (best1 == 0.0f || best1 < best2) {
        p->prev = (unsigned char)id2;
        p->arg1 = b2s1;
        p->act = (unsigned char)r2;
        p->arg2 = b2s2;
    } else {
        p->prev = (unsigned char)id1;
        p->arg1 = b1s1;
        p->act = (unsigned char)r1;
        p->arg2 = b1s2;
    }
    return p->act;
}
