/* 0x4e6180, applies character c for an office of the kind `kind` (the first of the 30 offices with that kind, a second one for the kinds that take two): when an office
   is open (owner -1, state 3, fewer than 4 applicants) sends the application (cm_SendApplication) between the "Amtsbewerbung" group
   brackets (cm_BeginGroup / cm_EndGroup); -1 when not. The search walks the kind byte of the office records (0x18 apart, the original's
   pointer induction, hence the do-while: a while is rotated and peels the first test); the two-person kinds then repeat it for the second
   office (f1: the first office took it, f2/f1 together: an office was open) and send the second office's index as m.o2. The final checks
   repeat the open test of the plain kind.
   @deep B3 61 differing instructions of 165: the whole original 0x4e6180..0x4e635f was read against the source and the statements match (guard c[0x168]; kind search over 30 records; kind record copy; the two-person second search with f1/f2; the three send paths; the plain-path open checks) - the rest is register allocation and basic-block order (the original keeps c in ecx and re-reads [esp+0x34] before m.id, re-reads [edx+0x4] / [eax+0x4] at the send sites, hoists the bracket push above the m stores and merges the argument cleanup into one add esp,0xc; the two 0xff stores go through cl) and those were tried one edit one build each: kind-pointer and record-pointer search loops (do-while(1) avoids the first-test peel, 100 -> 61 asm lines), i >= 30 and cnt >= 4 spellings, m.o2 = m.o1 / m.o1 = m.o2 / chained 0xff stores, if (f2) arm polarity, goto-found layout, bracket call before the stores (worse, reverted); spell.py: reload [esp+0x34] / [edx+0x4] / [eax+0x4] and dup-tail, no local holds those values (the CSE is of the parameter and the netId expression). Readability Q1-Q7 pass (the three cm callees named cm_BeginGroup / cm_EndGroup / cm_SendApplication, the latter now int). */

int amt_ApplyForOffice(char *c, unsigned int kind)
{
    unsigned char *p;
    int i, f1, f2;
    AMTAPP m;
    AMTKIND k;

    if (c[0x168] != 0)
        return -1;
    i = 0;
    p = &amt_Table[0].kind;
    do {
        if (*p == kind)
            break;
        p += sizeof(AMTREC);
        i++;
        if ((int)p >= (int)&amt_Table[30].kind)
            return -1;
    } while (1);
    if (i >= 30)
        return -1;
    m.kind = (unsigned char)kind;
    k = amt_Kinds[amt_Table[i].kind];
    m.o1 = 0xff;
    m.o2 = 0xff;
    if (k.two != 0) {
        f2 = 0;
        f1 = 0;
        if (amt_Table[i].owner == -1 && amt_Table[i].state == 3 && amt_Table[i].cnt < 4) {
            m.o1 = amt_Table[i].idx;
            f2 = 1;
            m.id = ((AMTCHARVIEW *)c)->netId;
            f1 = 1;
        }
        i++;
        if (i < 30) {
            do {
                if (amt_Table[i].kind == kind) {
                    if (i < 30 && amt_Table[i].owner == -1 && amt_Table[i].state == 3 && amt_Table[i].cnt < 4) {
                        if (!f2) {
                            m.o1 = amt_Table[i].idx;
                            m.id = ((AMTCHARVIEW *)c)->netId;
                            cm_BeginGroup("Amtsbewerbung");
                            goto send;
                        }
                        m.o2 = amt_Table[i].idx;
                    }
                    break;
                }
                i++;
            } while (i < 30);
        }
        if (f1) {
            cm_BeginGroup("Amtsbewerbung");
            goto send;
        }
    }
    if (amt_Table[i].owner != -1)
        return -1;
    if (amt_Table[i].state != 3)
        return -1;
    if (amt_Table[i].cnt >= 4)
        return -1;
    m.o1 = amt_Table[i].idx;
    m.id = ((AMTCHARVIEW *)c)->netId;
    cm_BeginGroup("Amtsbewerbung");
send:
    i = cm_SendApplication(&m);
    cm_EndGroup("Amtsbewerbung");
    return i;
}
