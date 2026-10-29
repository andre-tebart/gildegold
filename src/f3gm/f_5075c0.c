/* 0x5075c0, 1 when the character c (not a player of kind 6 / 7) may be given up for the party table: like 0x4fd090 (no player has it as a direct relation or among a full set of five), then, when fewer than 0x20 character slots are free (0x300 - 0x6f1bc0), the table at 0x91f725 (records of 0x2d bytes, the id at -0x13) must not hold it as an active entry; 0 otherwise */

int gm_Unk5075c0(char *c)
{
    int id, n, found, k;
    char *p;
    int *q;

    if (c[2] != 6 && c[2] != 7) {
        id = *(int *)(c + 4);
        for (p = ai_Chars[0] + 2; (int)p < (unsigned)((unsigned)ai_Chars + 0x65402); p += 0x21c) {
            found = 0;
            n = 0;
            if (*(int *)(p + 2) != id && (*p == 6 || *p == 7)) {
                if (*(int *)(p + 0x5a) == id)
                    return 0;
                if (*(int *)(p + 0x5e) == id)
                    return 0;
                if (*(int *)(p + 0x62) == id)
                    return 0;
                q = (int *)(p + 0x66);
                for (k = 5; k != 0; k--, q++) {
                    if (*q == id) {
                        found = 1;
                        n++;
                    } else if (*q != -1)
                        n++;
                }
                if (found && n < 5)
                    return 0;
            }
        }
        if (0x300 - ai_CharCount < 0x20)
            return 1;
        p = (char *)((unsigned)&ai_Unk91f716 + 0xf);
        while (*(int *)(p - 0x13) != id || *(int *)p == 0) {
            p += 0x2d;
            if ((int)p > ((unsigned)((unsigned)ai_Unk91f716 + 0x5a0a) + 0x4))
                return 1;
        }
    }
    return 0;
}
