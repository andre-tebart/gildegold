/* 0x4e75c0, 1 when the character c may take the office of the kind k (1 .. 27): the kind must exist, its column must be the column of c's current office rank
   (+0x166) and the levels must follow the cycle of three - level % 3 of k vs of c's rank, (0 / not 0, 2 / 0, 1 / 2); 0 otherwise. The office-application checks
   (cm.c 0x4dada0 / 0x4dae90) and the candidate scan 0x4e76b0 use it
   @deep B3 99 code hunks + 6 slots + 3 fill (68 differing of 89 instructions). Meaning read against the whole original 0x4e75c0..0x4e76a7 and complete: the k != 0 / k <= 0x1b guard, amt_GetKind(k, &a), amt_GetKind(c[0x166], &b), a.col == b.col, m = a.level % 3 and the three cycle tests (m 0 -> b%3 != 0, m 2 -> b%3 == 0, m 1 -> b%3 == 2), the three return 1 sites, return 0. Left: the original keeps the dword load of b.level (+padding) in esi across the three idiv sites and the divisor 3 in edi (push esi/push edi at entry; ours uses no callee-saved register and reloads), and the frame base of a / b differs by 0x10 ([esp+8] / [esp+0x20] vs [esp+0x18] / [esp+0x30]) - allocation/scheduling, no menu rule (spell.py dup-tail: the return-1 epilogues are already duplicated per site in our source; the tail difference is exactly the missing esi/edi). Q1-Q7 pass, renamed amt_CanTakeOffice from the application checks (cm.c 0x4dada0 / 0x4dae90) and the candidate scan 0x4e76b0. */

int amt_CanTakeOffice(char *c, int k)
{
    AMTKIND a, b;
    unsigned int m;

    if (k != 0 && k <= 0x1b) {
        if (amt_GetKind(k, &a) != 0) {
            if (amt_GetKind((unsigned char)c[0x166], &b) != 0 && a.col == b.col) {
                m = a.level % 3;
                if (m == 0 && b.level % 3 != 0)
                    return 1;
                if (m == 2 && b.level % 3 == 0)
                    return 1;
                if (m == 1 && b.level % 3 == 2)
                    return 1;
            }
        }
    }
    return 0;
}
