/* 0x578170, the gate predicate (14 B): 1 while the gate 0x6f265c is clear, else 0 */

int ai_Unk578170(void)
{
    return !U_I((unsigned)&full_data_6f265c);
}
