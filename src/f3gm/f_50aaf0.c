/* 0x50aaf0, the amount of the object o in the list object l: minus one for the stall objects 0x2a / 0x116 / 0x1db / 0x1dc (the stall itself) */

int gm_ListAmount(short *l, char *o)
{
    short k;

    k = *l;
    if (k != 0x2a && k != 0x116 && k != 0x1db && k != 0x1dc)
        return *(int *)(o + 0xe);
    return *(int *)(o + 0xe) - 1;
}
