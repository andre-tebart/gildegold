/* 0x4e6430, executes the application m (AMTAPP, see amt_CheckApplication): the character takes the kind in its +0x168 and the office
   (the two offices of a two-person kind) counts one applicant more; 0 when the character is unknown / 0xffff / has already applied, the
   office record or its kind record contradicts the message (kind id, two-person flag) or an office is not open
   @deep B3 45 differing instructions of 133 (raw 81 code-hunk instructions): the whole original 0x4e6430..0x4e65d8 was read against the source and every statement matches (the character guard 0/0xffff/+0x168, the two 30-record index searches with the 0x9253f0 bound, the kind byte local, the one-office and two-office paths with the k id / two-person checks and the cnt >= 4 bounds, the +0x168 store and the two cnt++); the rest is the prologue (the original pushes edi, ours uses esi and saves edi only around the k copy), the kind local slot (ours [esp+0x28], the original [esp+0x2c], the same slot under the extra push) and the register roles of the -1 / 4 compare values and the k.id reload - allocation, no menu rule. Tried one edit one build each: the search loops as do-while(1) with the named 30-record bound (156 -> 115 code hunks), i >= 30, cnt >= 4, the o2 != 0xff then-arm layout so the single-office path is the out-of-line else (115 -> 81), Q4 via AMTCHARVIEW.id. spell.py: slots 0x28/0x2c and reload [esp+0x20] (k.two) - both read the prologue's push edi. Readability Q1-Q7 pass (gm_FindChar, AMTCHARVIEW.id, amt_CheckApplication). */

int amt_AcceptApplication(AMTAPP *m)
{
    char *c;
    AMTREC *p;
    AMTKIND k;
    int i, j;
    unsigned char kind;

    c = gm_FindChar(m->id);
    if (c != 0 && ((AMTCHARVIEW *)c)->id != 0xffff && c[0x168] == 0) {
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
        kind = m->kind;
        if (kind != amt_Table[i].kind)
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
        k = amt_Kinds[amt_Table[i].kind];
        if (k.id != kind)
            return 0;
        if (k.two == 0)
            return 0;
        if (amt_Table[i].owner != -1)
            return 0;
        if (amt_Table[i].state != 3)
            return 0;
        if (amt_Table[i].cnt >= 4)
            return 0;
        if (amt_Table[j].owner != -1)
            return 0;
        if (amt_Table[j].cnt >= 4)
            return 0;
        c[0x168] = kind;
        amt_Table[i].cnt++;
        amt_Table[j].cnt++;
        return 1;
        }
        if (amt_Table[i].owner != -1)
            return 0;
        if (amt_Table[i].state != 3)
            return 0;
        if (amt_Table[i].cnt >= 4)
            return 0;
        c[0x168] = kind;
        amt_Table[i].cnt++;
        return 1;
    }
    return 0;
}
