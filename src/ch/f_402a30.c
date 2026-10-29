/* 0x402a30 */

CHARACTER *ch_AllocAt(int id)
{
    CHARACTER *ch = mem_Alloc(sizeof(CHARACTER), "ch:Create:Character");

    memset(ch, 0, sizeof(CHARACTER));
    if (ch_List[id])
        return 0;
    ch_List[id] = ch;
    ch->id = id;
    return ch;
}
