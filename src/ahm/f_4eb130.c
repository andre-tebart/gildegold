/* 0x4eb130, the factor of the place with the good id of the character c: 1.0 + share 1 * the value of the place; 1.0 when it has no place */

float ahm_PlaceFactor(char *c, int id)
{
    AHMSLOT *slot;

    slot = ahm_FindSlot(c, id);
    if (ahm_HasRecord(c) == 0)
        return 1.0f;
    if (slot == 0)
        return 1.0f;
    return ahm_Share(c, 1) * slot->v + 1.0f;
}
