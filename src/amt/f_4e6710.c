/* 0x4e6710, checks the office message m (AMTMSG: +0 the office, +1 / +7 two character ids or -1, +6 the code): with a code (not 0xff) the
   office is one of the 42 records, in the state 1 and without a second candidate (+0x14), and both characters exist; 1 when it may be
   executed (amt_ExecuteOfficeMessage) */

int amt_CheckOfficeMessage(AMTMSG *m)
{
    AMTREC *q;
    int i;

    if (m->e != 0xff) {
        i = 0;
        q = amt_Table;
        do {
            if (q->idx == m->office)
                break;
            q++;
            i++;
            if ((int)q >= (int)&amt_Table[42])
                return 0;
        } while (1);
        if (i >= 42)
            return 0;
        if (amt_Table[i].state != 1)
            return 0;
        if (amt_Table[i].x14 != -1)
            return 0;
        if (m->b != -1) {
            if (gm_FindChar(m->b) == 0)
                return 0;
        }
        if (m->a != -1) {
            if (gm_FindChar(m->a) == 0)
                return 0;
        }
    }
    return 1;
}
