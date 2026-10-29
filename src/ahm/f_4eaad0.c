/* 0x4eaad0, 1 when the character c (flag 4 of +0x1cc) has an object (+0x16c) of the building kind 0x1e with a record at +0x65 */

int ahm_HasRecord(char *c)
{
    AHMCHAR *ch = (AHMCHAR *)c;

    if ((c[0x1cc] & 4) != 0 && ch->obj != 0 && ai_BTypes[ch->obj->kind * 0x24d] == 0x1e)
        return ch->obj->rec != 0;
    return 0;
}
