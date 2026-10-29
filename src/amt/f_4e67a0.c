/* 0x4e67a0, executes the office message p (AMTSET: +0 the office, +1 the new owner or -1, +5 the new state, +7 the x14
   character; message 0x45, the only caller is cm.c's handler of the type 0x45): the old owner loses the rank / the status
   bit of the office, the new owner takes it (officeKind +0x166, prevOffice +0x167, secondOffice +0x169, status bits
   +0x1cc), the applicants' counts are settled; 0 when a character does not exist
   @deep B3: 136 of 286 instructions differ (135 code hunks). The whole original 0x4e67a0..0x4e6b33 was read against the source and the statement sequence is complete and equal: the x14/owner guards (gm_FindChar, 0 on a missing character), the 42-record scan (named bound &amt_Table[42], i >= 42), the old owner's officeKind/secondOffice/status-bit clears, the k > 0x1b dispatch (0x1c..0x22 second-office scan over the records 30..36 from &amt_Table[30].owner to &amt_Table[37].owner, the 0x23/0x24/0x25 status masks + the 768-record mask loop), the normal path (owner scan, kind level compare, officeKind/prevOffice/appliedOffice, 0x0f/0x15/0x1b status bits, the two-person applicant count), the done block and the ai_Unk5a8cb0 call. Left: the out-of-line block order (the original puts the two-scan body at 0x4e6a41 and the k > 0x1b dispatch at 0x4e6a7c after the return; ours keeps the dispatch inline) and the register roles from instruction 0 (original c=esi, x=[esp+0x10], kind reloaded per use; ours c=edi, x slot, k local) - allocation/scheduling, no menu rule. spell.py leads tried one edit one build each: the first scan as while(cond1 && cond2) (259 diff, reverted), as while + inner if-return (261, reverted), as do-while(1) with the named bound (146, kept); the kind level compare written from the other side (amt_Kinds[kind].level >= ... - 146 -> 136, kept); the Q3 bound 0x92550f -> &amt_Table[42] and 0x92549c -> &amt_Table[37].owner; the Q4/Q2 typed AMTCHARVIEW fields and ai_Chars (byte-neutral). Q2 left: ai_Unk5a8cb0 (0x5a8cb0) has no evidenced name - its only caller is this function, its body sends the text 0x20bb (the text table is loaded at runtime, not in the exe) with the x14 character's id to the new owner (event 0x5fd, only for a player character kind 6/7); naming it needs a match-deep decision. Unit note amt.md has the details. */

int amt_ExecuteOfficeMessage(AMTSET *p)
{
    int i, j, owner;
    char *c, *o, *x;
    unsigned int m, f;
    unsigned char k;
    AMTREC *q;
    int *r;

    i = 0;
    owner = -1;
    x = 0;
    if (p->x14 != -1) {
        x = (char *)gm_FindChar(p->x14);
        if (x == 0)
            return 0;
    }
    c = 0;
    if (p->owner != -1) {
        c = (char *)gm_FindChar(p->owner);
        if (c == 0)
            return 0;
        owner = ((AMTCHARVIEW *)c)->netId;
    }
    q = amt_Table;
    do {
        if (q->idx == p->idx)
            break;
        q++;
        i++;
        if ((int)q >= (int)&amt_Table[42])
            return 0;
    } while (1);
    if (i >= 42)
        return 0;
    o = (char *)gm_FindChar(amt_Table[i].owner);
    if (o != 0 && ((AMTCHARVIEW *)o)->netId != p->owner) {
        if (((AMTCHARVIEW *)o)->officeKind == amt_Table[i].kind)
            ((AMTCHARVIEW *)o)->officeKind = 0;
        else if (((AMTCHARVIEW *)o)->secondOffice == amt_Table[i].kind)
            ((AMTCHARVIEW *)o)->secondOffice = 0;
        else {
            if (amt_Table[i].kind == 0x23)
                f = ((AMTCHARVIEW *)o)->status & 0xfffffdff;
            else if (amt_Table[i].kind == 0x24)
                f = ((AMTCHARVIEW *)o)->status & 0xfffffbff;
            else if (amt_Table[i].kind == 0x25)
                f = ((AMTCHARVIEW *)o)->status & 0xfffff7ff;
            else
                goto next;
            ((AMTCHARVIEW *)o)->status = f;
        }
    }
next:
    if (c == 0)
        goto done;
    k = amt_Table[i].kind;
    if (k > 0x1b) {
        if (k > 0x1b && k < 0x23) {
            if (((AMTCHARVIEW *)c)->secondOffice != 0) {
                r = &amt_Table[30].owner;
                do {
                    if (((unsigned char *)r)[4] == ((AMTCHARVIEW *)c)->secondOffice &&
                        *r == ((AMTCHARVIEW *)c)->netId)
                        *r = -1;
                    r += sizeof(AMTREC) / sizeof(int);
                } while ((int)r < (int)&amt_Table[37].owner);
            }
            ((AMTCHARVIEW *)c)->secondOffice = amt_Table[i].kind;
            goto done;
        }
        m = 0;
        f = ((AMTCHARVIEW *)c)->status & 0xfffff1ff;
        ((AMTCHARVIEW *)c)->status = f;
        if (amt_Table[i].kind == 0x23) {
            f = f | 0x200;
            m = 0x4000;
            ((AMTCHARVIEW *)c)->status = f;
        } else {
            if (amt_Table[i].kind == 0x24) {
                f = f | 0x400;
                m = 0x8000;
                ((AMTCHARVIEW *)c)->status = f;
            } else if (amt_Table[i].kind == 0x25) {
                f = f | 0x800;
                m = 0x10000;
                ((AMTCHARVIEW *)c)->status = f;
            }
        }
        for (j = 0; j < 768; j++)                       /* the 0x300 character records */
            ((AMTCHARVIEW *)ai_Chars)[j].status &= ~m;
        goto done;
    }
    if (((AMTCHARVIEW *)c)->officeKind != 0) {
        for (j = 0; j < 30; j++) {
            if (amt_Table[j].owner == owner) {
                amt_Table[j].owner = -1;
                amt_Table[j].x14 = -1;
                amt_Table[j].state = 4;
                amt_Table[j].cnt = 0;
                break;
            }
        }
    }
    if (amt_Kinds[amt_Table[i].kind].level >= amt_Kinds[((AMTCHARVIEW *)c)->officeKind].level)
        ((AMTCHARVIEW *)c)->prevOffice = amt_Table[i].kind;
    ((AMTCHARVIEW *)c)->officeKind = amt_Table[i].kind;
    if (((AMTCHARVIEW *)c)->appliedOffice == amt_Table[i].kind)
        ((AMTCHARVIEW *)c)->appliedOffice = 0;
    if (amt_Table[i].kind == 0x0f)
        ((AMTCHARVIEW *)c)->status |= 0x40;
    else if (amt_Table[i].kind == 0x15)
        ((AMTCHARVIEW *)c)->status |= 0x80;
    else if (amt_Table[i].kind == 0x1b)
        ((AMTCHARVIEW *)c)->status |= 0x100;
    if (amt_Kinds[amt_Table[i].kind].two != 0) {
        for (j = 0; j < 30; j++) {
            if (amt_Table[j].kind == amt_Table[i].kind && j != i) {
                if (amt_Table[j].state == 3 && amt_Table[j].cnt != 0) {
                    amt_Table[j].cnt--;
                    if (amt_Table[j].cnt == 0)
                        amt_Table[j].state = 4;
                }
                break;
            }
        }
    }
done:
    amt_Table[i].owner = owner;
    amt_Table[i].x14 = p->x14;
    amt_Table[i].state = p->state;
    if (p->state == 3)
        amt_Table[i].cnt = 0;
    if (x != 0 && c != 0 && ((AMTCHARVIEW *)ai_Chars)[ai_PlayerId].netId == ((AMTCHARVIEW *)c)->netId)
        ai_Unk5a8cb0((unsigned short *)x, c);
    return 1;
}
