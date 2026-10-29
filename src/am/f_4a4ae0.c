/* 0x4a4ae0, eval callback: only the weights of the current need count */

int ai_Unk4a4ae0(float *a, float *b, int i, AIPER *p)
{
    G5ACT *act = &G5_ACTS[i];
    float d1;
    int k, found;

    *a = 0.0f;
    *b = 0.0f;
    if ((act->flags & G7_MASK(p)) == 0) {
        d1 = 50.0f;
        found = 0;
    } else {
        d1 = 40.0f;
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
        *a = (act->w1[k].k == p->sel ? act->w1[k].w / d1 * p->mood[act->w1[k].k][2] : 0.0f) + *a;
        *b = (act->w2[k].k == p->sel ? act->w2[k].w / d1 * p->mood[act->w2[k].k][2] : 0.0f) + *b;
    }
    return found;
}
