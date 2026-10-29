/* 0x4e6d80, takes the character c out of the offices (owner or second character of any of the 42: the owner's office
   becomes free (state 3, or 4 after the 12th hour of the game clock), a second office returns to state 1), takes back its
   application (the applicant counts of its kind) and clears its ranks (officeKind / secondOffice / appliedOffice);
   1 when it held an office or an application
   @deep B3: 32 of ~120 instructions differ (51 code hunks; ours 63 instructions, the original 57). The whole original 0x4e6d80..0x4e6e16 was read against the source and the statement sequence is equal: d = the clock hour (ai_TimeHour = ai_Time.hour 0x770ae4), the 42-record loop testing owner then x14 (owner: free state (hour > 11) + 3, cnt 0, r |= 1; x14: state 1), the appliedOffice loop (kind match + cnt > 0: r |= 1, cnt--), the three rank bytes cleared, return r. Left: the loop's induction base (the original walks &amt_Table[i].x14 with [ecx-0x10] = owner, [ecx-8] = cnt, [ecx-4] = state; ours uses &amt_Table[i].owner), the -1 constant (the original stores the immediate, ours hoists -1 into edi and pushes it - 3 extra instructions) and the netId cache in edx - allocation/strength-reduction, no menu rule. spell.py reload lead tried: the netId is already the typed expression at both uses (the original reloads [ebp+4] once per branch, ours CSEs it); the r = 1 -> r |= 1 spelling was applied (the original's or al,0x1) and the typed AMTCHARVIEW fields / ai_TimeHour view are byte-neutral. Readability Q1-Q7 pass (amt_ClearOffices, AMTCHARVIEW fields). */

int amt_ClearOffices(char *c)
{
    unsigned short d;
    int r, i;

    d = ai_TimeHour;
    r = 0;
    for (i = 0; i < 42; i++) {
        if (amt_Table[i].owner == ((AMTCHARVIEW *)c)->netId) {
            amt_Table[i].owner = -1;
            amt_Table[i].cnt = 0;
            amt_Table[i].state = (d > 11) + 3;
            r |= 1;
            amt_Table[i].x14 = -1;
        } else if (amt_Table[i].x14 == ((AMTCHARVIEW *)c)->netId) {
            amt_Table[i].state = 1;
            amt_Table[i].x14 = -1;
        }
    }
    if (((AMTCHARVIEW *)c)->appliedOffice != 0) {
        for (i = 0; i < 42; i++) {
            if (amt_Table[i].kind == ((AMTCHARVIEW *)c)->appliedOffice && amt_Table[i].cnt > 0) {
                r |= 1;
                amt_Table[i].cnt--;
            }
        }
    }
    ((AMTCHARVIEW *)c)->officeKind = 0;
    ((AMTCHARVIEW *)c)->secondOffice = 0;
    ((AMTCHARVIEW *)c)->appliedOffice = 0;
    return r;
}
