/* 0x50cfd0, the same with 0x274 / 0x27e / 0x28e / 0x291 (2 + ..) */

int gm_CurseObjectCount(char *b)
{
    int n;

    n = 2;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x274) != 0)
        n = 3;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x27e) != 0)
        n = n + 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x28e) != 0)
        n = n + 1;
    if (gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x291) != 0)
        n = n + 1;
    return n;
}
