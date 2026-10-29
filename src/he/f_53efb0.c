/* 0x53efb0, the sum of the values (+0xc) of the AP events with key (+4) `key` */

int he_Unk53efb0(int key)
{
    int sum = 0;
    int *p = (int *)((unsigned)&he_RunHandlers + 0x6ac);

    do {
        if (*p != 0 && p[-2] == key)
            sum += *p;
        p += 0x14;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x56ac));
    return sum;
}
