/* 0x5a8390, the record's recursive free (32 B): the child +0x48 first, then 0x422620 */

void ai_Unk5a8390(int param_1)
{
    if (*(int *)(param_1 + 0x48) != 0)
        ai_Unk5a8390(*(int *)(param_1 + 0x48));
    mem_Free(param_1);
}
