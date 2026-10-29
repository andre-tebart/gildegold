/* 0x4e3800, the highest index below n (1 ..) of the ids m +0x34 that is not -1; 0 without */

int ai_Unk4e3800(char *m, int n)
{
    int i;

    for (i = n - 1; i > 0; i--)
        if (*(int *)(m + 0x34 + i * 4) != -1)
            return i;
    return 0;
}
