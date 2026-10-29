/* 0x576460, 1 while the gate 0x6f265c is 0; else 0 in the mode 2 / state 5, 1 otherwise */

int ai_Unk576460(void)
{
    char *st;

    if (!U_I((unsigned)&full_data_6f265c))
        return 1;
    st = S;
    if (*(int *)(st + 0x3c) == 2 || U_I(st) == 5)
        return 0;
    return 1;
}
