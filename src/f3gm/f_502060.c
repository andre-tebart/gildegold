/* 0x502060, the number of the building kind with the name (case-insensitive; the name is at +1 of each 0x24d-byte record), 0 when none */

int gm_FindBuildingKind(char *name)
{
    int n, i;

    n = 0;
    i = 0;
    do {
        if (_stricmp(name, (char *)(i + 1 + (int)ai_BTypes)) == 0)
            return n;
        i += 0x24d;
        n++;
    } while (i < 0xca78);
    return 0;
}
