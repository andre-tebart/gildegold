/* 0x576410, the same predicate as 0x5763c0 on the run record's word +0x6c */

int ai_Unk576410(int mask)
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
    return (*(int *)(*(char **)(st + 0x18) + 0x6c) & mask) != 0;
}
