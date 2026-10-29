/* 0x406340, d3_ForEachObject callback: collects the dummy objects characters walk to */

char ch_FindDummyCB(D3OBJECT *obj, CHOBJLIST *list)
{
    char names[15][0x40] = {
        "dummy_WAIT",
        "dummy_FEGEN1",
        "dummy_FEGEN2",
        "dummy_FEGEN3",
        "dummy_FEGEN4",
        "dummy_PAUSE",
        "dummy_PAUSE2",
        "dummy_AUSGANG"     /* sic, no comma */
        "dummy_LAUFEN1",
        "dummy_LAUFEN2",
        "dummy_LAUFEN3",
        "dummy_LAUFEN4",
        "dummy_STEHPLATZ_01",
        "dummy_STEHPLATZ_02",
        "dummy_STEHPLATZ_03"
    };
    int i;

    for (i = 0; i < 15; i++) {
        if (names[i][0] && strstr(obj->name, names[i])) {
            list->objs[list->n] = obj;
            list->n++;
            break;
        }
    }
    return list->n < 32;
}
