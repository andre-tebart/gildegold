/* 0x4eac20, 1 when the number of the places in use (ahm_UsedSlots) is below the limit (the share 2 times the six places) */

int ahm_HasFreeSlot(char *c)
{
    int n;

    if (ahm_HasRecord(c) == 0)
        return 0;
    n = ahm_UsedSlots(c);
    return (int)(ahm_Share(c, 2) * 6.0f) > n;
}
