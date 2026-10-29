/* 0x5763c0, 1 when the run record's word +0x68 has a bit of `mask` (the run mask), else 0; 1 while 0x6f265c is 0
   and when there is no run record, 0 in the mode 2 / state 5 and while a transition 0x6333e0 is pending */

int ai_Unk5763c0(int mask)
{
    char *st;

    if (!U_I((unsigned)&full_data_6f265c))
        return 1;
    st = S;
    if (*(int *)(st + 0x3c) == 2 || U_I(st) == 5)
        return 0;
    if (!*(int *)(st + 0x18))
        return 1;
    if (U_I((unsigned)&tb_Paused))
        return 0;
    return (*(int *)(*(char **)(st + 0x18) + 0x68) & mask) != 0;
}
