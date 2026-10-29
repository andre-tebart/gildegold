/* 0x50ab50, the stock a list object l holds: (amount + 2) * 5 for the stall 0x1dd, 0x50 for 3, else 0x14 per amount */

int gm_ListStock(short *l, int id)
{
    int a;

    a = *(int *)(l + 7);
    if (*l == 0x1dd) {
        a += 2;
        return a * 5;
    }
    if (a == 3)
        return 0x50;
    return a * 0x14;
}
