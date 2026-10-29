/* 0x4fe900, the number of the object prototype with the given name (case-insensitive; the name is at +1 of each 0x41-byte record), 0 when none */

int gm_FindObjectProto(char *name)
{
    int n, i;

    n = 0;
    i = 0;
    do {
        if (_stricmp(name, &ai_Goods[i + 1]) == 0)
            return n;
        i += 0x41;
        n++;
    } while (i < 0xb9dc);
    return 0;
}
