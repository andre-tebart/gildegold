/* 0x49a290, decide callback 0x1a: only actions that do not raise need 0 (APS) */

int ai_Unk49a290(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    G5ACT *act;
    int r, k;

    if (f != 0)
        return 0;
    s1.unk08 = 1;
    s2.unk08 = 1;
    s1.kind = 0;
    s1.unk04 = 0;
    s2.kind = 0;
    s2.unk04 = 0;
    r = ai_Unk495a50(0x1a, p->id, 0, &s1, &s2);
    if (r == 0)
        return 0;
    act = &G5_ACTS[r];
    for (k = 0; k < 4; k++) {
        if (act->w1[k].k == 0 && act->w1[k].w > 0.0f)
            return 0;
    }
    for (k = 0; k < 4; k++) {
        if (act->w2[k].k == 0 && act->w2[k].w > 0.0f)
            return 0;
    }
    *o1 = s1;
    *o2 = s2;
    return r;
}
