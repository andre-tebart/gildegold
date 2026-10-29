/* 0x40d730, returns a random object of the default scene whose name contains *pattern (rand() picks among the
   matches); 0 when nothing matches */

int ocmd_GetRndObjectHandle(char **pattern)
{
    OCMDFIND f;

    memset(&f, 0, sizeof(f));
    strcpy(f.pattern, *pattern);
    d3_ForEachObject(d3_DefaultScene, 0, 0x140, (D3OBJECTCB)ocmd_FindCB, &f);
    if (!f.n)
        return 0;
    return (int)f.objs[rand() % f.n];
}
