/* 0x40d7b0, as GetRndObjectHandle, but only among the children of *parent; 0 when *parent is NULL / not a live
   object or nothing matches */

int ocmd_GetRndSubObjectHandle(D3OBJECT **parent, char **pattern)
{
    OCMDFIND f;

    if (!*parent)
        return 0;
    if (!mem_IsValid(*parent))
        return 0;
    memset(&f, 0, sizeof(f));
    strcpy(f.pattern, *pattern);
    d3_ForEachObject(d3_DefaultScene, *parent, 0x340, (D3OBJECTCB)ocmd_FindCB, &f);
    if (!f.n)
        return 0;
    return (int)f.objs[rand() % f.n];
}
