/* 0x5741d0, the event of the type 0xa0 whose +0x10 argument is the word after `a` (0x53e400 (2, 0, 0xa0, 3, ..)), 0 when none */

char *ai_Unk5741d0(char *a)
{
    return he_FindEvent(2, 0, 0xa0, 3, *(int *)(a + 1));
}
