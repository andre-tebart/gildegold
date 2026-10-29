/* 0x4cd010, handler of type 0x64: the four-letter command data[0] ('SmSa', 'ErFi', 'SsCa', 'cRSa', 'Rdaa', 'Rdsa')
   for the character data[4] */

int ai_Unk4cd010(G17MSG *m, G17STAT *s)
{
    char *c, *g;
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    c = gm_FindChar(G17_I(*m, 4));
    if (c == 0)
        return -1;
    if (ahm_HasRecord(c) == 0)
        return -2;
    switch (G17_I(*m, 0)) {
    case 0x61536d53:
        r = ahm_AddValue(&m->data[0], *(int *)(*(char **)(c + 0x16c) + 0x65));
        break;
    case 0x46697245:
        g = *(char **)(c + 0x16c);
        r = ahm_DeleteAlchemist(&m->data[0], *(int *)(g + 0x65), g);
        break;
    case 0x61437353:
        r = ahm_SetShare(&m->data[0], *(int *)(*(char **)(c + 0x16c) + 0x65));
        break;
    case 0x61535263:
        r = ahm_SetPlaceValue(&m->data[0], *(int *)(*(char **)(c + 0x16c) + 0x65));
        break;
    case 0x61616452:
        g = *(char **)(c + 0x16c);
        r = ahm_AddPlace(&m->data[0], *(int *)(g + 0x65), g);
        break;
    case 0x61736452:
        g = *(char **)(c + 0x16c);
        r = ahm_RemovePlace(&m->data[0], *(int *)(g + 0x65), g);
        break;
    default:
        return -3;
    }
    if (s != 0 && r != 0)
        s->state = 1;
    return 0;
}
