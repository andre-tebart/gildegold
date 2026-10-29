/* 0x576340, calls the run record's callback +0x3c with the four arguments as an int[4] and returns its result;
   1 when a gate fails (0x6f265c, the run record, [r+0x3c]), 0 while 0x6333e0 is set (a transition in progress) */

int ai_Unk576340(int a, int b, int c, int d)
{
    int p[4];
    char *r;

    if (!U_I((unsigned)&full_data_6f265c))
        return 1;
    if (!*(int *)(S + 0x18))
        return 1;
    if (U_I((unsigned)&tb_Paused))
        return 0;
    r = *(char **)(S + 0x18);
    if (!*(int *)(r + 0x3c))
        return 1;
    p[0] = a;
    p[1] = b;
    p[2] = c;
    p[3] = d;
    return ((int (*)(int *))*(int *)(r + 0x3c))(p);
}
