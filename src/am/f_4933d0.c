/* 0x4933d0, as 0x493390, and sets the kind of side c */

int ai_Unk4933d0(AGDUEL *d, AGCH *c, int k)
{
    int *q;
    int i;

    if (c == d->side1)
        q = &d->kind1;
    else if (c == d->side2)
        q = &d->kind2;
    else
        return 0;
    i = 0;
    while (ag_Pairs[i][0] != *q || ag_Pairs[i][1] != k) {
        i++;
        if (i >= 9)
            return 0;
    }
    *q = ag_Pairs[i][1];
    return 1;
}
