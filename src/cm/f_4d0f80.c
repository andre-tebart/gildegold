/* 0x4d0f80, a callback: an object of kind 3 gets +0x224 = 0; returns 1 */

char ai_Unk4d0f80(char *o)
{
    if (*(int *)(o + 0x220) == 3)
        *(int *)(o + 0x224) = 0;
    return 1;
}
