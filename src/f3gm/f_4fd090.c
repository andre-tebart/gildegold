/* 0x4fd090, 1 when the character c (not a player, kind 6 / 7) may be removed: no other player (kind 6 / 7) has it as one of the three direct relations (+0x5a, +0x5e, +0x62) and, when the five slots from +0x66 hold it, they are all filled */

int unk_Unk4fd090(char *c)
{
    int id, n, found;
    char *p;
    int *q;
    int k;

    if (c[2] == 6 || c[2] == 7)
        return 0;
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
            for (k = 5; k != 0; k--) {
                if (*q == id) {
                    found = 1;
                    n++;
                } else if (*q != -1)
                    n++;
                q++;
            }
            if (found && n < 5)
                return 0;
        }
    }
    return 1;
}
