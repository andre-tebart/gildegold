/* 0x499400, decide: find a good to sell (action 0x15) */

int ai_Unk499400(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    AIARG s1, s2;
    AIBLD *y;
    LARGE_INTEGER m;
    int x, z, w, r;

    if (a != 0)
        return 0;
    if (p->kind == 3)
        return 0;
    if (b == 0) {
        if (o1->kind != 0)
            return 0;
    } else if (o1->kind == 0) {
        /* nothing */
    } else
        return 0;
    m.LowPart = 0xa07d7bb0;
    m.HighPart = 1;
    if (!ai_Unk4a63f0(p, 4, m.QuadPart, 0, 100.0f, &x))
        return 0;
    gm_FindById((int)&y, 0, 0, x);
    if (!ai_Unk4a5540(&w, &z, y, 0))
        return 0;
    s1.kind = 2;
    s1.unk04 = w;
    s1.unk08 = 1;
    s1.unk14 = 0.44f;
    if (z != -1) {
        s2.kind = 1;
        s2.unk04 = z;
    } else {
        s2.kind = 4;
        s2.unk04 = y->id;
    }
    s2.unk08 = 1;
    s2.unk14 = 0.44f;
    r = ai_Unk495a50(0x15, p->id, 1, &s1, &s2);
    if (!r)
        return 0;
    *o1 = s1;
    *o2 = s2;
    return r;
}
