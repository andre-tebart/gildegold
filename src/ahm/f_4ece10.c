/* 0x4ece10, the message 'ErFi' (cm_SendCommand) of the character c; -1 when c has no object */

int ahm_SendDelete(char *c)
{
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    AHM_ZERO6(&m);
    m.tag = AHM_CMD_DELETE;
    m.charId = ((AHMCHAR *)c)->id;
    return ((int (*)(void *))cm_SendCommand)(&m);
}
