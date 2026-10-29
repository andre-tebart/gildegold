/* 0x4e6e20, amt_ClearOffices without the result: the owner's office is released only when `all` is set (the second
   office always returns to state 1), the application is always taken back, the ranks (officeKind / secondOffice) are
   cleared only when `all` is set
   @deep B3: the sibling of amt_ClearOffices (0x4e6d80) with the same body and the same causes: 51 code hunks from the loop's induction base (the original walks &amt_Table[i].x14 with [ecx-0x10] = owner, [ecx-8] = cnt, [ecx-4] = state; ours uses &amt_Table[i].owner), the -1 constant hoisted into a pushed register instead of the original's immediate stores, and the netId cache in edx - allocation / strength reduction, no menu rule. The whole original 0x4e6e20..0x4e6e2b was read against the source: the owner branch gated by all != 0 (release, cnt 0, state (hour > 11) + 3, x14 -1), the x14 branch (state 1), the appliedOffice count take-back, appliedOffice = 0 and the officeKind / secondOffice clears under all != 0 - every statement matches. spell.py: the reload lead (the netId is the typed expression at both tests) and the r = 1 spelling (no r here) tried; typed AMTCHARVIEW fields and ai_TimeHour are byte-neutral. Readability Q1-Q7 pass (amt_ClearOfficesIfAll). */

void amt_ClearOfficesIfAll(char *c, int all)
{
    unsigned short d;
    int i;

    d = ai_TimeHour;
    for (i = 0; i < 42; i++) {
        if (amt_Table[i].owner == ((AMTCHARVIEW *)c)->netId && all != 0) {
            amt_Table[i].owner = -1;
            amt_Table[i].cnt = 0;
            amt_Table[i].state = (d > 11) + 3;
            amt_Table[i].x14 = -1;
        } else if (amt_Table[i].x14 == ((AMTCHARVIEW *)c)->netId) {
            amt_Table[i].state = 1;
            amt_Table[i].x14 = -1;
        }
    }
    if (((AMTCHARVIEW *)c)->appliedOffice != 0) {
        for (i = 0; i < 42; i++) {
            if (amt_Table[i].kind == ((AMTCHARVIEW *)c)->appliedOffice && amt_Table[i].cnt > 0)
                amt_Table[i].cnt--;
        }
    }
    ((AMTCHARVIEW *)c)->appliedOffice = 0;
    if (all != 0) {
        ((AMTCHARVIEW *)c)->officeKind = 0;
        ((AMTCHARVIEW *)c)->secondOffice = 0;
    }
}
