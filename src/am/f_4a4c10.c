/* 0x4a4c10, eval callback with the character's rank and the decision's value */

int ai_Unk4a4c10(float *a, float *b, int i, AIPER *p, AIDEC *dec)
{
    G5ACT *act;
    float v, rank, d1, d2, t;
    int n, k, found;
    unsigned char c;

    if (dec == 0)
        v = 1.0f;
    else
        v = (float)dec->unk0c;
    n = unk_Unk4f39e0(p->netId);
    rank = p->mood[11][0];
    *a = 0.0f;
    *b = 0.0f;
    act = &G5_ACTS[i];
    rank = (rank - 2.0f) * ((float)n + v);
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
        c = act->w1[k].k;
        if (act->w1[k].w <= 0.0f)
            t = act->w1[k].w / (c == p->sel ? d1 : d2) * p->mood[c][2];
        else if (c == p->sel)
            t = p->mood[c][2] * act->w1[k].w / (d1 + rank);
        else
            t = p->mood[c][2] * act->w1[k].w / (d2 + rank);
        *a = t + *a;
        c = act->w2[k].k;
        if (act->w2[k].w <= 0.0f)
            t = act->w2[k].w / (c == p->sel ? d1 : d2) * p->mood[c][2];
        else if (c == p->sel)
            t = p->mood[c][2] * act->w2[k].w / (d1 + rank);
        else
            t = p->mood[c][2] * act->w2[k].w / (d2 + rank);
        *b = t + *b;
    }
    return found;
}
