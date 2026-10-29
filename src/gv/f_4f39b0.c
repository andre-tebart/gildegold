/* 0x4f39b0, how many records of 0x91f700 belong to the character c (+0x16) and are in state 1 */

int unk_Unk4f39b0(int c)
{
    int cnt;
    int *p;

    cnt = 0;
    p = (int *)(gv_Graveyard + 0x25);
    do {
        if (*(int *)((char *)p - 0xf) == c && *p == 1)
            cnt++;
        p = (int *)((char *)p + 0x2d);
    } while ((int)p < (int)((unsigned)&amt_Table + 0x5));
    return cnt;
}
