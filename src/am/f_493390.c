/* 0x493390, 1 when (the kind of side c, k) is a pair the duel accepts */

int ai_Unk493390(AGDUEL *d, AGCH *c, int k)
{
    int v;
    const int *q;

    if (c == d->side1)
        v = d->kind1;
    else if (c == d->side2)
        v = d->kind2;
    else
        return 0;
    q = &ag_Pairs[0][1];
    while (q[-1] != v || *q != k) {
        q += 2;
        if ((int)q >= (int)&ag_Pairs[9][1])
            return 0;
    }
    return 1;
}
