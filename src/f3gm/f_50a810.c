/* 0x50a810, the first kind 6 / 7 / 1 / 2 character with a job whose master (+0x16c) is m */


char *gm_FindCharacterByMaster(int m)
{
    char *p;
    int i;
    char k;

    if (m == 0)
        return 0;
    i = 0;
    p = ai_Chars[0] + 2;
    while (p[0x162] == 0 || *(int *)(p + 0x16a) != m || ((k = *p) != 6 && k != 7 && k != 1 && k != 2)) {
        p += 0x21c;
        i++;
        if ((int)p > ((unsigned)((unsigned)ai_Chars + 0x65400) + 0x1))
            return 0;
    }
    return ai_Chars[i];
}
