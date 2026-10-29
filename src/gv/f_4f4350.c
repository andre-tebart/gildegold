/* 0x4f4350, clears the graveyard game state: 0x6f1b88 = 0, the 512 records of 0x91f700 (ids -1, +0x16 = -1, int +0x25 = 0 ..), the 2048 pairs -1 */

void gv_ClearGraveyard(void)
{
    int *p;

    p = (int *)(gv_Graveyard + 0x12);
    *(int *)unk_Var6f1b88 = 0;
    do {
        p[1] = -1;
        p[0] = -1;
        *(int *)((char *)p - 0x12) = -1;
        *(int *)((char *)p + 0x13) = 0;
        p = (int *)((char *)p + 0x2d);
    } while ((int)p < (int)(gv_Graveyard + 0x5a12));
    p = (int *)(unk_Tab91b6e0 + 4);
    do {
        p[-1] = -1;
        p[0] = -1;
        p += 2;
    } while ((int)p < ((unsigned)unk_Tab91b6e0 + 0x4004));
}
