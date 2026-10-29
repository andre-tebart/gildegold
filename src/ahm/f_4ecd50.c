/* 0x4ecd50, the message 'SmSa' (0x4c61b0) of the character c with the value v; -1 when c has no object */

int ahm_SendAddValue(char *c, int v)
{
    AHMORDER m;

    if (ahm_HasRecord(c) == 0)
        return -1;
    AHM_ZERO6(&m);
    m.tag = AHM_CMD_ADD_VALUE;
    m.charId = ((AHMCHAR *)c)->id;
    m.value.i = v;
    return ((int (*)(void *))cm_SendCommand)(&m);
}
