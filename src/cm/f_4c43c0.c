/* 0x4c43c0, checks the messages of the group grp (5 .. 6): all pass: 1 at both ends, else 2 for all */

int ai_Unk4c43c0(G17MSG *grp)
{
    G17MSG *p;
    int ok;

    ok = 1;
    for (p = grp->next; p->type != 6; p = p->next) {
        if (ai_Unk60bf40[p->type](p) != 0) {
            ok = 0;
            break;
        }
    }
    if (ok) {
        grp->type = 1;
        while (grp->type != 6)
            grp = grp->next;
        grp->type = 1;
        return 1;
    }
    while (grp->type != 6) {
        grp->type = 2;
        grp = grp->next;
    }
    grp->type = 2;
    return 1;
}
