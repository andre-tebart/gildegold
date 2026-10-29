/* 0x495a50, rates the actions that lead to action kind for character id (recursion guarded by the stack) */

/* ---- out of address order: 0x495a50 belongs between 0x4957d0 and 0x495ce0. Defined above its callers, the
   draft's register use changes 0x496e40 / 0x499660 (matched) and 0x496610 / 0x499c10; move it back once it
   matches (layout 2026-10-22). ---- */

int ai_Unk495a50(int kind, int id, int c, AIARG *o1, AIARG *o2)
{
    float best1, best2, a, b;
    int k, r, r1, r2;
    AIARG s1, s2, b1s1, b2s1, b1s2, b2s2;
    G5ACT *act;
    G5CH *p;

    best1 = -1e30f;
    best2 = -1e30f;
    r1 = 0;
    r2 = 0;
    b1s1.kind = 0;
    b2s1.kind = 0;
    b1s2.kind = 0;
    b2s2.kind = 0;
    p = &G5_CHARS[id];
    if (ai_Unk4957a0(kind) != 0)
        return 0;
    if (p->mask == 0)
        p->mask = 1;
    k = 0;
    for (act = G5_ACTS; act < G5_ACTS + 0x49; act++, k++) {
        if (act->id != 0 && act->id != p->act && ai_Unk4957a0(act->id) == 0) {
            ai_Unk495770(act->id);
            s1 = *o1;
            s2 = *o2;
            if (act->eval != 0)
                act->eval(&a, &b, k, p, c, &s1, &s2);
            if ((best1 <= a || best2 <= b) && act->decide != 0) {
                r = act->decide(kind, p, c, &s1, &s2);
                if (r != 0) {
                    if (best1 <= a) {
                        b1s1 = s1;
                        best1 = a;
                        b1s2 = s2;
                        r1 = r;
                    }
                    if (best2 <= b) {
                        best2 = b;
                        b2s1 = s1;
                        b2s2 = s2;
                        r2 = r;
                    }
                }
            }
            ai_Unk495790();
        }
    }
    if (best1 == 0.0f || best1 < best2) {
        *o1 = b2s1;
        *o2 = b2s2;
        return r2;
    }
    *o1 = b1s1;
    *o2 = b1s2;
    return r1;
}
