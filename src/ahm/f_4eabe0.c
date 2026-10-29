/* 0x4eabe0, the place of the record of the character c with the good id; 0 when there is none */

AHMSLOT *ahm_FindSlot(char *c, int id)
{
    int i;

    if (ahm_HasRecord(c) != 0) {
        for (i = 0; i < 6; i++) {
            if (((AHMCHAR *)c)->obj->rec->slots[i].id == id)
                return ((AHMCHAR *)c)->obj->rec->slots + i;
        }
    }
    return 0;
}
