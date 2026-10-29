/* 0x407690 */

void *ch_MapAlloc(int unk)
{
    CHMAPNODE *p = mem_Alloc(sizeof(CHMAPNODE), "ch_map");

    p->unk10 = unk;
    return p;
}
