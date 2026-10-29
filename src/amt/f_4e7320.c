/* 0x4e7320, the same candidate list as amt_GetOfficeCandidates, with the full offices counted: a rated office with 4 applicants is skipped for the list and sets
   *full to 1 (when full is not 0). Returns the number of candidates, capped at n; the town hall window 0x5a0590 uses *full to tell "all offices full" from "no
   office open"
   @deep B3 212 code hunks + 3 fill + 2 slots (359/432 bytes), the meaning read against the whole original and complete: the amt_GetOfficeCandidates search plus the full-office report (a rated office with cnt >= 4 sets f and is skipped, *full = f at the end when full != 0). Menu/leads tried one build each: n < 1 -> n <= 0 (gain 384 -> 359 bytes, 165 -> 137 asm lines); the insertion rate test as the continue-guard form (the original's test ah,0x41 / jp; 137 -> 135 lines, 228 -> 212 hunks); the pointer table walk (WORSE here: 135 -> 139 lines, reverted - unlike 0x4e7150 the original uses different roles). Left: the frame slot order (ours cnt [ebp-4] / rate [ebp-8]; original cnt [ebp-0xc] / rate [ebp-4] / record pointer [ebp-4]?) and the original's search-2 found flag in esi (mov esi,1 before the insert, ours has none) with its i == -1 out[0] re-test - allocation/scheduling (c2prio/slotmap). spell.py menu: reload at 0x4e732a ([ebp-0xc] kept in ebx across the call) and 0x4e7466 ([ebp+0x10] reloaded into edi), dup-tail at 0x4e736f. Q1-Q7 pass, renamed amt_GetOfficeCandidatesFull from the town hall window caller 0x5a0590 (0x14409d8 'all full' vs 0x14409c8 'none'). */

int amt_GetOfficeCandidatesFull(char *c, int n, AMTCAND *out, int *full)
{
    int cnt, i, j, k, kind, f;
    float rate;

    cnt = 0;
    f = 0;
    if (n <= 0)
        return 0;
    AI_ZERO(out, n * 4);
    for (i = n - 1; i >= 0; i--) {
        out[i].rate = -100.0f;
        out[i].kind = 0;
        out[i].level = 0;
    }
    for (j = 0; j < 30; j++) {
        if (amt_Table[j].state != 3)
            continue;
        kind = amt_Table[j].kind;
        if (amt_GetOfficeRate(c, kind, &rate) == 0)
            continue;
        if (amt_Table[j].cnt >= 4) {
            f = 1;
            continue;
        }
        for (i = n - 1; i >= 0; i--) {
            if (out[i].kind == kind)
                break;
        }
        if (i >= 0)
            continue;
        for (i = n - 1; i >= 0; i--) {
            if (amt_Kinds[kind].level > out[i].level)
                continue;
            if (out[i].level == amt_Kinds[kind].level && out[i].rate <= rate)
                continue;
            break;
        }
        if (i == -1)
            i = 0;
        cnt++;
        for (k = n - 1; k > i; k--) {
            out[k].level = out[k - 1].level;
            out[k].rate = out[k - 1].rate;
            out[k].kind = out[k - 1].kind;
        }
        out[i].level = amt_Kinds[amt_Table[j].kind].level;
        out[i].rate = rate;
        out[i].kind = amt_Table[j].kind;
    }
    if (full != 0)
        *full = f;
    if (n <= cnt)
        cnt = n;
    return cnt;
}
