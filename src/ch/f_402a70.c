/* 0x402a70 */

CHARACTER *ch_FindByName(char *name)
{
    int i;

    for (i = 0; i < CH_MAX; i++)
        if (ch_List[i] && !_strcmpi(ch_List[i]->obj->name, name))
            break;
    if (i < CH_MAX)
        return ch_List[i];
    return 0;
}
