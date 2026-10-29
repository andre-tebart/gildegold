/* 0x4eaa90, the number of the places in use (the six AHMSLOT at +0xc of the record) of the object of the character c (0x4eaad0), 0 when it has none */

int ahm_UsedSlots(char *c)
{
    int n, i;
    AHMSLOT *p;

    if (ahm_HasRecord(c) == 0)
        return 0;
    n = 0;
    p = ((AHMCHAR *)c)->obj->rec->slots;
    for (i = 6; i != 0; i--) {
        if (p->id != 0)
            n++;
        p++;
    }
    return n;
}
