/* 0x53ef80, the sum of the values (+8) of the AP events with key (+4) `key` */

int he_Unk53ef80(int key)
{
    int sum = 0;
    int *p = (int *)((unsigned)&he_RunHandlers + 0x6a8);

    do {
        if (*p != 0 && p[-1] == key)
            sum += *p;
        p += 0x14;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x56a8));
    return sum;
}
