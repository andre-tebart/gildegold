/* 0x406790, a random dummy object of the default scene */

D3OBJECT *ch_FindDummy(void)
{
    CHOBJLIST list;

    memset(&list, 0, sizeof(list));
    d3_ForEachObject(d3_DefaultScene, 0, 0x100, (D3OBJECTCB)ch_FindDummyCB, &list);
    if (!list.n)
        return 0;
    return list.objs[(unsigned short)ch_Random((unsigned short)list.n)];
}
