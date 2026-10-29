/* 0x50ab20, the amount (0x50aaf0) of the object id in the list of l */

int gm_ListAmountById(char *l, int id)
{
    char *o;

    o = gm_Query(*(void **)(l + 0x14), 1, 0, id);
    if (o == 0)
        return 0;
    return gm_ListAmount((short *)l, o);
}
