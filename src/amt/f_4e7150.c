/* 0x4e7150, the offices the character c may apply for, best first: the open ones (state 3, fewer than 4 applicants) among the first 30 table records (the offices of the
   guild columns) whose kind has a rate for c (amt_GetOfficeRate); one entry per kind, sorted by level and, at the same level, by rate (an empty place is -100.0 / kind 0 /
   level 0). Writes up to n entries to out (n * 4 bytes of it are cleared first), returns their number capped at n; the callers are the application list 0x4ea8e0 and the
   AI decision 0x497490 (am.c), which applies for a random candidate
   @deep B3 205 code hunks + 2 slots (352/400 bytes), the meaning read against the whole original 0x4e7150..0x4e731f and complete: guard n <= 0 (original cmp 0/jg), AI_ZERO(out, n*4), the backwards init loop (rate -100.0f, kind 0, level 0), the pointer walk over the first 30 records, state 3 / cnt < 4, amt_GetOfficeRate, the duplicate-kind search, the insertion search (continue form: level < kind level, or same level and rate <= rate), if (i == -1) i = 0, cnt++, the backwards shift, the insert from the kind record, min(cnt, n). Menu/leads tried one build each: n < 1 -> n <= 0 (gain 368 -> 353 bytes); rate > rate -> !(rate <= rate) as continue guards (the original's test ah,0x41 / jp; gain 353 -> 352, 127 lines); the found-flag search form (for i >= 0 && !found) gave 184 code + 12 regs but grew the function to 432 bytes vs the original's 400 - reverted; table index walk -> pointer walk (p = amt_Table .. amt_Table + 30, the original's [ebp-8], 400 bytes, 207 -> 205 hunks). Left: frame slot order (ours rate [ebp-0xc] / cnt [ebp-4], original rate [ebp-4] / cnt [ebp-0xc] / found [ebp-0x10]), the search-1 found flag in a register vs our memory slot, the insert/shift block allocation, and the original's redundant out[0] re-test at i == -1 (semantically equal to our i = 0) - allocation/scheduling, c2prio/slotmap. Q1-Q7 pass, renamed amt_GetOfficeCandidates from the application-list callers (0x4ea8e0, 0x497490 in am.c, the town hall window in unk_58bbd0.c). */

int amt_GetOfficeCandidates(char *c, int n, AMTCAND *out)
{
    int cnt, i, k, kind;
    float rate;
    AMTREC *p;

    cnt = 0;
    if (n <= 0)
        return 0;
    AI_ZERO(out, n * 4);
    for (i = n - 1; i >= 0; i--) {
        out[i].rate = -100.0f;
        out[i].kind = 0;
        out[i].level = 0;
    }
    for (p = amt_Table; p < amt_Table + 30; p++) {
        if (p->state != 3 || p->cnt >= 4)
            continue;
        kind = p->kind;
        if (amt_GetOfficeRate(c, kind, &rate) == 0)
            continue;
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
        out[i].level = amt_Kinds[p->kind].level;
        out[i].rate = rate;
        out[i].kind = p->kind;
    }
    if (n <= cnt)
        cnt = n;
    return cnt;
}
