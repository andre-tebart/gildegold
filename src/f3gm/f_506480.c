/* 0x506480, counts the active relatives (the five ids from +0x68, 0x505df0, +8 set) of the character c into *n and returns 1 when one of them is older than 0xb */

int gm_Unk506480(char *c, int *n)
{
    int r, cnt, k;
    short *p;
    int *q;

    r = 0;
    cnt = 0;
    q = (int *)(c + 0x68);
    for (k = 5; k != 0; k--, q++) {
        p = gm_FindChar(*q);
        if (p != 0 && ((char *)p)[8] != 0 && (cnt++, 0xb < *(unsigned short *)((char *)p + 10)))
            r = 1;
    }
    if (n != 0)
        *n = cnt;
    return r;
}
