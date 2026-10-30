/* 0x4e7c60, settles the n offices of the list recs (a table of records as AMTREC): the offices in the state 2 (two candidates: the owner and the x14 character) are
   decided by the votes of the other office holders (0x4aab90 gives 0 / 1 / 2 per voter; opinions change by 0x4c4b00), the offices in the state 3 (applications, the
   applicants are the characters whose appliedOffice is the office kind) by their votes (0x4aad30), a tie by chance; each result is sent with 0x4e6680 and waited for
   (0x4c3ae0 / 0x52fe00). The first parameter is not used. The whole original (Ghidra 0x4e7c60 + the disassembly) was read against this draft: the statement sequence
   is equal (the two passes, the vote loops, the 4-candidate scan ai_Chars .. ai_Chars + 768, the best / tie / random pick, the message sends and the slot waits);
   what differs is the frame (ours 0x60, the original 0x58: cnt2 / i / cnt3 spill in ours, the original keeps them in registers) and the register roles, i.e.
   allocation only
   @deep B3: 669 code hunks / 4 regs, 1137/1312 bytes, 499 differing asm lines - allocation (frame 0x60 vs 0x58, cnt2/i/cnt3 spill in ours); the whole original was read in the previous session and the statement sequence is equal. Readability Q1-Q7 pass except Q3: 0x7d5f80 is the end of ai_Chars (+ 768 * 0x21c). Symbol spellings measured worse: (char *)ai_Chars + 768*0x21c -> 1182/1328 bytes, 510 lines; (char *)ai_CharInts + 768*0x21c - 4 the same; the literal keeps 1137/1312, 499 (its init emits mov OFFSET;sub 4 and the second loop's register allocation then matches better), so the no-WORSE rule blocks it - same dead end as ls.c's 0x7d6a42 bound. This session: Q1 amt_ElectOffices (the unit's own comment 'lost the election 0x4e7c60'); Q2 ai_CharInts -> ai_CharInts project-wide (80 refs, the character records ai_Chars + 4 as int[][0x87], byte-neutral); Q4 the 8 raw *(int *)(x + 4) -> AMTCHARVIEW netId (byte-neutral); Q5 ROUGH removed; Q7 amt.h's K&R ai_TrustedOfTwo / ai_TrustedOfFour / cm_GetSlotState -> real prototypes (struct AIPER *, int; lint E 0, X 0 new). Typed fields for c[0x166] / ch[0x168] cost +4 asm lines and stay raw. spell.py: no menu rule on the function. */

void amt_ElectOffices(int unused, int n, AMTREC *recs)
{
    int cnt2, cnt3, i, j, m, k, r, sel, left;
    int cnt[4];
    char *cand[4];
    int ids[4];
    int votes[6];
    int best, bi, id, v, s;
    int tie;
    char *o, *x, *c, *ch;
    AMTREC *p;
    char buf[4];

    cnt2 = 0;
    cnt3 = 0;
    for (i = 0; i < n; i++) {
        if (recs[i].state == 2)
            cnt2++;
    }
    for (i = 0; i < n; i++) {
        if (cnt2 == 0)
            break;
        cnt[0] = 0;
        cnt[1] = 0;
        cnt[2] = 0;
        if (recs[i].state == 2) {
            cnt2--;
            o = gm_FindChar(recs[i].owner);
            x = gm_FindChar(recs[i].x14);
            if (o != 0 && x != 0) {
                for (j = 0; j < n; j++) {
                    votes[j] = 3;
                    c = gm_FindChar(recs[j].owner);
                    if (c != 0 && c[0x166] != 0 && c[0x1b1] == 0) {
                        s = ai_TrustedOfTwo(c, o, x, buf);
                        votes[j] = s;
                        cnt[s]++;
                    }
                }
                if (cnt[1] < cnt[0]) {
                    for (j = 0; j < n; j++) {
                        id = recs[j].owner;
                        if (id != ((AMTCHARVIEW *)o)->netId && id != -1 && (unsigned int)votes[j] < 3) {
                            if (votes[j] == 0)
                                cm_SendRelations(id, ((AMTCHARVIEW *)o)->netId, -10, 1);
                            if (votes[j] == 1)
                                cm_SendRelations(id, ((AMTCHARVIEW *)o)->netId, 10, 1);
                            else
                                cm_SendRelations(id, ((AMTCHARVIEW *)o)->netId, -4, 1);
                        }
                    }
                    r = amt_SendOfficeMessage((char)recs[i].idx, 0, 0, 3, 0xff);
                } else {
                    for (j = 0; j < n; j++) {
                        id = recs[j].owner;
                        if (id != ((AMTCHARVIEW *)o)->netId && id != -1 && (unsigned int)votes[j] < 3) {
                            if (votes[j] == 0)
                                v = -20;
                            else if (votes[j] == 1)
                                v = 20;
                            else
                                v = 4;
                            cm_SendRelations(id, ((AMTCHARVIEW *)o)->netId, v, 1);
                        }
                    }
                    r = amt_SendOfficeMessage((char)recs[i].idx, o, 0, 1, 0xff);
                }
                if (r != -1) {
                    while (cm_GetSlotState(r) == 0)
                        sv_PumpNet();
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        if (recs[i].state == 3)
            cnt3++;
    }
    p = recs;
    for (i = 0; i < n; i++, p++) {
        if (cnt3 == 0)
            return;
        cnt[0] = 0;
        cnt[1] = 0;
        cnt[2] = 0;
        cnt[3] = 0;
        for (j = 0; j < 6; j++)
            votes[j] = -1;
        if (p->state == 3 && p->cnt != 0) {
            cnt3--;
            k = 0;
            ch = (char *)ai_CharInts - 4;
            do {
                if (p->cnt <= k || k > 3)
                    break;
                if (ch[0x168] == (char)p->kind) {
                    ids[k] = ((AMTCHARVIEW *)ch)->netId;
                    cand[k] = ch;
                    k++;
                }
                ch += 0x21c;
            } while ((int)ch < (unsigned)((unsigned)ai_CharInts + 0x653fc));
            if (k != 0) {
                if (k == 1) {
                    r = amt_SendOfficeMessage((char)p->idx, cand[0], 0, 1, 0xff);
                    if (r != -1) {
                        while (cm_GetSlotState(r) == 0)
                            sv_PumpNet();
                    }
                } else {
                    for (m = k; m < 4; m++) {
                        ids[m] = -1;
                        cand[m] = 0;
                    }
                    for (j = 0; j < n; j++) {
                        votes[j] = -1;
                        c = gm_FindChar(recs[j].owner);
                        if (c != 0 && amt_CanTakeOffice(c, p->kind) != 0 && c[0x1b1] == 0)
                            votes[j] = ai_TrustedOfFour((char)p->idx, c, ids, buf);
                    }
                    for (j = 0; j < n; j++) {
                        v = votes[j];
                        if (v != -1) {
                            for (m = 0; m < k; m++) {
                                if (((AMTCHARVIEW *)cand[m])->netId == v)
                                    cnt[m]++;
                            }
                        }
                    }
                    m = 0;
                    best = -1;
                    bi = -1;
                    tie = 1;
                    do {
                        if (best < cnt[m]) {
                            tie = 0;
                            bi = m;
                            best = cnt[m];
                        } else if (cnt[m] == best)
                            tie = 1;
                        m++;
                    } while (m < k);
                    if (!tie) {
                        r = amt_SendOfficeMessage((char)p->idx, cand[bi], 0, 1, 0xff);
                        if (r != -1) {
                            while (cm_GetSlotState(r) == 0)
                                sv_PumpNet();
                        }
                        continue;
                    }
                    m = util_RandomU(k) & 0xffff;
                    left = k;
                    do {
                        sel = m;
                        if (cnt[m] == best)
                            break;
                        left--;
                        m = (m + 1) % k;
                        sel = bi;
                    } while (left != 0);
                    r = amt_SendOfficeMessage((char)p->idx, cand[sel], 0, 1, 0xff);
                    if (r != -1) {
                        while (cm_GetSlotState(r) == 0)
                            sv_PumpNet();
                    }
                }
            }
        }
    }
}
