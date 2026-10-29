/* 0x4f39e0, the sum of the +0x18 ints of the table records (0x4f2720 of the byte +0x1c) of the records of the character c in state 1 */

int unk_Unk4f39e0(int c)
{
    int *p;
    int sum;
    int rec[9];

    p = (int *)(gv_Graveyard + 0x25);
    sum = 0;
    do {
        if (*(int *)((char *)p - 0xf) == c && *p == 1) {
            gv_GetTableRecord(*((unsigned char *)p - 9), rec);
            sum += rec[8];
        }
        p = (int *)((char *)p + 0x2d);
    } while ((int)p < ((unsigned)&amt_Table + 0x5));
    return sum;
}
