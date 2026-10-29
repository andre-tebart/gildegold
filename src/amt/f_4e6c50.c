/* 0x4e6c50, executes the swap message m (AMTTRADE: +0 the requester, +4 the first / +5 the second office index, +6 the
   price; message 0x5c, the only caller is cm.c's handler of the type 0x5c): the owners of the two offices exchange their
   office kinds and the office owners, the price is taken off the requester's money (+0x194); 0 when anything is missing */

int amt_ExecuteSwapMessage(AMTTRADE *m)
{
    char *c, *o1, *o2;
    AMTREC *p, *p1, *p2;
    int i, j, t;

    c = gm_FindChar(m->id);
    if (c == 0)
        return 0;
    if (((AMTCHARVIEW *)c)->money < m->v)
        return 0;
    i = 0;
    p = amt_Table;
    do {
        if (p->idx == m->a)
            break;
        p++;
        i++;
        if ((int)p >= (int)&amt_Table[30])
            return 0;
    } while (1);
    p1 = &amt_Table[i];
    if (p1 == 0)
        return 0;
    o1 = (char *)gm_FindChar(p1->owner);
    if (o1 == 0)
        return 0;
    j = 0;
    p = amt_Table;
    do {
        if (p->idx == m->b)
            break;
        p++;
        j++;
        if ((int)p >= (int)&amt_Table[30])
            return 0;
    } while (1);
    p2 = &amt_Table[j];
    if (p2 == 0)
        return 0;
    o2 = (char *)gm_FindChar(p2->owner);
    if (o2 == 0)
        return 0;
    ai_SendSwapText((unsigned short *)c, (unsigned short *)o1, (unsigned short *)o2);
    ((AMTCHARVIEW *)o1)->officeKind = p2->kind;
    ((AMTCHARVIEW *)o2)->officeKind = p1->kind;
    t = p1->owner;
    p1->owner = p2->owner;
    p2->owner = t;
    ((AMTCHARVIEW *)c)->money -= m->v;
    return 1;
}
