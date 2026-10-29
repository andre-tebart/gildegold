/* 0x4eca00, the 'SsCa' handler: sets the share m->id (0 .. 3) of the record rec (+8 .. +0xb) to the value m->value.f times 252 (the inverse of 0x4eab20); 0 for
   another number */

int ahm_SetShare(int *p, char *rec)
{
    AHMORDER *m = (AHMORDER *)p;
    AHMREC *r = (AHMREC *)rec;

    switch (m->id) {
    case 0:
        r->share[0] = (char)(int)(m->value.f * 252.0f);
        return 1;
    case 1:
        r->share[1] = (char)(int)(m->value.f * 252.0f);
        return 1;
    case 2:
        r->share[2] = (char)(int)(m->value.f * 252.0f);
        return 1;
    case 3:
        r->share[3] = (char)(int)(m->value.f * 252.0f);
        return 1;
    default:
        return 0;
    }
}
