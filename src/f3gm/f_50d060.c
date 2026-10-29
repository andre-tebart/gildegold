/* 0x50d060, 0x285 / 0x288 (1 + ..) */

int gm_Unk50d060(char *b)
{
    int n;

    n = 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x285) != 0)
        n = 2;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x288) != 0)
        n = n + 1;
    return n;
}
