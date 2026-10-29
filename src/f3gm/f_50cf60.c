/* 0x50cf60, the number of the kitchen / bath objects (0x27a, 0x27b, 0x292) the building b has: 1 + .. */

int gm_Unk50cf60(char *b)
{
    int n;

    n = 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x27a) != 0)
        n = 2;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x27b) != 0)
        n = n + 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x292) != 0)
        n = n + 1;
    return n;
}
