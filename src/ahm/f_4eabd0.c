/* 0x4eabd0, the place of c's record with the id (a forwarding function to 0x4eabe0) */

AHMSLOT *ahm_SlotOf(char *c, int id)
{
    return ahm_FindSlot(c, id);
}
