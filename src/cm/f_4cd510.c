/* 0x4cd510, all players of the list (ai_Unk936424[8]) have state 1 and their number is the one asked for */

int ai_Unk4cd510()
{
    unsigned int n;
    int cnt = 0;
    int *p;
    char *q;

    if (ai_ModeMask == 0x3f)
        n = ai_Unk936496;
    else
        n = ai_Unk936496;
    if (ai_Unk936420[0] < n)
        return 0;
    for (p = ai_Unk936424; (int)p < (int)(ai_Unk936424 + 8); p++) {
        if (*p != 0) {
            q = ai_Unk4cd5e0(*p);
            if (q == 0)
                return 0;
            if (*(int *)(q + 4) == 1)
                cnt++;
        }
    }
    return cnt == n;
}
