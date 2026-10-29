/* 0x4cb850, handler of type 0x43 */

int ai_Unk4cb850(G17MSG *m, G17STAT *s)
{
    char *c;

    c = gm_FindChar(G17_I(*m, 0));
    if (c != 0 && *(char **)(c + 0x170) != 0)
        gm_ShowBuildingObjects(*(char **)(c + 0x170));
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
