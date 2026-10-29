/* 0x4f3610, fills out[] with the indexes of the (up to 32) 0x2d-byte records of 0x91f700 whose int at +0x25 equals v */

void unk_Unk4f3610(int v, int *out)
{
    int n, i;
    int *p;

    n = 0;
    i = 0;
    p = (int *)(gv_Graveyard + 0x25);
    do {
        if (n > 0x1f)
            return;
        if (*p == v) {
            out[n] = i;
            n++;
        }
        p = (int *)((char *)p + 0x2d);
        i++;
    } while ((int)p < ((unsigned)&amt_Table + 0x5));
}
