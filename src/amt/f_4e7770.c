/* 0x4e7770, the open offices (state 3) of the column of the office kind c holds (+0x166) that c is eligible for by level: a level
   divisible by 3 or one below the level of c's office (none when that level is 1 mod 3), into out[] at most n; the
   scan stops after an office that takes two persons; returns their number. Only reached through the page fill 0x5bfd90
   (the special-amount window), which counts the characters who applied for these offices */

int amt_GetEligibleOffices(char *c, int n, int *out)
{
    AMTKIND a;
    AMTREC list[6];
    int m, i, cnt, want, id, level;

    if (c[0x166] == 0)
        return 0;
    if (amt_GetKind((unsigned char)c[0x166], &a) == 0)
        return 0;
    m = amt_GetColumnOffices(a.col, 6, list);
    if (m == 0)
        return 0;
    level = a.level;
    if (level % 3 == 1)
        want = 0;
    else
        want = level - 1;
    cnt = 0;
    for (i = 0; i < m && cnt < n; i++) {
        if (list[i].state == 3) {
            id = list[i].kind;
            amt_GetKind(id, &a);
            level = a.level;
            if (level % 3 == 0)
                out[cnt++] = id;
            else if (want == level) {
                out[cnt++] = id;
                if (a.two != 0)
                    break;
            }
        }
    }
    return cnt;
}
