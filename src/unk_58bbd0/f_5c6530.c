/* 0x5c6530, the marriage test of two characters (240 B): param_1 / param_2 the two character ids;
   0x505df0 resolves both; the errors 0xfffffc00 (missing), 0xfffffbff / 0xfffffbfe (the +8 byte 0),
   0xfffffbfd / 0xfffffbfc (the +0x5c partner word of the other), 0xfffffbfb / 0xfffffbfa (the +0x1b1
   flag); the age difference 0x505e60 (the id) under 6 -> 1, else 0 */

unsigned int ai_Unk5c6530(int param_1, int param_2)
{
    char *a;
    char *b;
    int i;
    int j;
    int d;

    a = (char *)gm_FindChar(param_1);
    b = (char *)gm_FindChar(param_2);
    if (a == 0 || b == 0)
        return 0xfffffc00;
    if (a[8] == 0)
        return 0xfffffbff;
    if (b[8] == 0)
        return 0xfffffbfe;
    if (U_I(b + 0x5c) != -1 && U_I(b + 0x5c) != param_1)
        return 0xfffffbfd;
    i = U_I(a + 0x5c);
    if (i != -1 && i != param_2)
        return 0xfffffbfc;
    if (a[0x1b1] != 0)
        return 0xfffffbfb;
    if (b[0x1b1] != 0)
        return 0xfffffbfa;
    i = gm_Unk505e60(*(unsigned short *)a, 0);
    j = gm_Unk505e60(*(unsigned short *)b, 0);
    d = i - j;
    return (unsigned int)((d < 0 ? -d : d) < 6);
}
