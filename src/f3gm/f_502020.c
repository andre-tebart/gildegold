/* 0x502020, the building with the id (+1) in the building table, 0 when none */

char *gm_FindBuildingById(int id)
{
    int i;
    char *p;

    i = 0;
    p = ai_Blds;
    while (*p == 0 || *(int *)(p + 1) != id) {
        i++;
        p += 0xa9;
        if (i >= 0x100)
            return 0;
    }
    return ai_Blds + i * 0xa9;
}
