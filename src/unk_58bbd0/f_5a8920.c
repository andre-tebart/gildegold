/* 0x5a8920, the settings freed (29 B): 0x5a8390 over the list 0x6f26c0 */

void ai_FreeSettings(void)
{
    if (*(int *)((unsigned)&full_data_6f26c0) != 0)
        ai_Unk5a8390(*(int *)((unsigned)&full_data_6f26c0));
    *(int *)((unsigned)&full_data_6f26c0) = 0;
}
