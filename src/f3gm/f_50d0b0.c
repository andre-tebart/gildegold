/* 0x50d0b0, 0x284 (4 -> 7) / 0x289 (+ 3) */

int gm_CurseTargetCount(char *b)
{
    int n;

    n = 4;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x284) != 0)
        n = 7;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x289) != 0)
        n = n + 3;
    return n;
}
