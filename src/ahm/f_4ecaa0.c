/* 0x4ecaa0, the message 'Rdaa' (cm_SendCommand) of the character c with the table entry of id: its id and +8; -1 when c has no object or no free place (0x4eac20) or the entry does not exist */

int ahm_SendAddPlace(char *c, int id)
{
    AHMGRID *g;
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    if (ahm_HasFreeSlot(c) == 0)
        return -1;
    g = ahm_FindRecipe(id);
    if (g == 0)
        return -1;
    AHM_ZERO6(&m);
    m.charId = ((AHMCHAR *)c)->id;
    m.tag = AHM_CMD_ADD_PLACE;
    m.id = g->rcp.id;
    m.value.i = g->w[2];
    return ((int (*)(void *))cm_SendCommand)(&m);
}
