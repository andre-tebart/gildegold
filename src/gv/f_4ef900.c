/* 0x4ef900, the first entry of the list 0xff of the object o whose +4 word is the callback's (0x4ef780) match for the entry e; 0 when e is 0 or e[0] is -1 */

unsigned gv_LoadGraveyard(unsigned char *o, int *e)
{
    if (e != 0 && e[0] != -1)
        return gv_FindEntry(e, o, 0, gv_EntryHasId, 0xff);
    return 0;
}
