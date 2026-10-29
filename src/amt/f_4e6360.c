/* 0x4e6360, checks the application m (AMTAPP: +0 the character, +4 the first office, +5 the second or 0xff, +6 the kind): the character exists
   and has not applied yet, the office m.o1 (and m.o2, when not 0xff) is one of the first 30 records, open (owner -1, state 3, fewer than 4
   applicants) and of that kind; 1 when it may be executed (amt_AcceptApplication) */

int amt_CheckApplication(AMTAPP *m)
{
    char *c;
    AMTREC *p;
    int i, j;

    c = gm_FindChar(m->id);
    if (c != 0 && c[0x168] == 0) {
        i = 0;
        p = amt_Table;
        do {
            if (p->idx == m->o1)
                break;
            p++;
            i++;
            if ((int)p >= (int)&amt_Table[30])
                return 0;
        } while (1);
        if (i >= 30)
            return 0;
        if (amt_Table[i].owner != -1)
            return 0;
        if (amt_Table[i].state != 3)
            return 0;
        if (amt_Table[i].cnt >= 4)
            return 0;
        if (m->kind != amt_Table[i].kind)
            return 0;
        if (m->o2 != 0xff) {
            j = 0;
            p = amt_Table;
            do {
                if (p->idx == m->o2)
                    break;
                p++;
                j++;
                if ((int)p >= (int)&amt_Table[30])
                    return 0;
            } while (1);
            if (j >= 30)
                return 0;
            if (amt_Table[i].kind != amt_Table[j].kind)
                return 0;
            if (amt_Table[j].owner != -1)
                return 0;
            if (amt_Table[j].state != 3)
                return 0;
            if (amt_Table[j].cnt >= 4)
                return 0;
        }
        return 1;
    }
    return 0;
}
