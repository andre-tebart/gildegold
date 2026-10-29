/* 0x5bae10, the init of the table 0x6f3888 (64 B, ROUGH): the four dwords at -8 .. +4 (-1 / 0) */

int ai_Unk5bae10(void)
{
    U_I((unsigned)&full_data_6f3880) = 0;
    U_I((unsigned)&full_data_6f3884) = -1;
    /* ROUGH: the rest of the table */
    return 0;
}
