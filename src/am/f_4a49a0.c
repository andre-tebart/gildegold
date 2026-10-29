/* 0x4a49a0, eval callback: the action's weights against the moods */

int ai_Unk4a49a0(float *a, float *b, int i, AIPER *p, int f, void *x, void *y)
{
    G5ACT *act = &G5_ACTS[i];
    float d1, d2, d;
    int k, found;

    *a = 0.0f;
    *b = 0.0f;
    if ((act->flags & G7_MASK(p)) == 0) {
        d1 = 50.0f;
        d2 = 100.0f;
        found = 0;
    } else {
        d1 = 40.0f;
        d2 = 50.0f;
        found = 1;
    }
    for (k = 0; !found; k++) {
        if (k > 3)
            break;
        if (act->w1[k].k == p->sel && act->w1[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; !found; k++) {
        if (k > 3)
            break;
        if (act->w2[k].k == p->sel && act->w2[k].w > 0.0f)
            found = 1;
    }
    for (k = 0; k < 4; k++) {
        d = d2;
        if (act->w1[k].k == p->sel)
            d = d1;
        *a = act->w1[k].w / d * p->mood[act->w1[k].k][2] + *a;
        d = d2;
        if (act->w2[k].k == p->sel)
            d = d1;
        *b = act->w2[k].w / d * p->mood[act->w2[k].k][2] + *b;
    }
    return found;
}
