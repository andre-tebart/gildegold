/* 0x4ecd10, the 'cRSa' handler: sets the value of the place m->id in the record rec to m->value.f; 0 when there is none */

int ahm_SetPlaceValue(int *p, char *rec)
{
    AHMORDER *m = (AHMORDER *)p;
    AHMREC *r = (AHMREC *)rec;
    int i;

    for (i = 0; i < 6; i++) {
        if (r->slots[i].id == m->id) {
            r->slots[i].v = m->value.f;
            return 1;
        }
    }
    return 0;
}
