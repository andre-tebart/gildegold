/* 0x508660, the sum of the amounts of the objects of the list (prototype kind 9, group 1) whose prototype word is the one of the player slot p */

int gm_SumSlotAmounts(void *list, unsigned char p)
{
    int s;
    unsigned short *o;

    s = 0;
    o = (unsigned short *)gm_Query(list, 1, 4, 9);
    if (o) {
        do {
            if (PLW54(p) == *o)
                s += *(int *)(o + 7);
            o = (unsigned short *)gm_QueryNext();
        } while (o);
    }
    return s;
}
