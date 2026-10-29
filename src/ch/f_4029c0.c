/* 0x4029c0 */

CHARACTER *ch_Alloc(void)
{
    int i = 0;
    CHARACTER *ch = mem_Alloc(sizeof(CHARACTER), "ch:Create:Character");

    memset(ch, 0, sizeof(CHARACTER));
    for (; ch_List[i] && i < CH_MAX; i++)
        ;
    if (i < CH_MAX) {
        ch_List[i] = ch;
        ch->id = i;
        return ch;
    }
    mem_Free(ch);
    err_Message("ch_Create:Too many characters!");
    return 0;
}
