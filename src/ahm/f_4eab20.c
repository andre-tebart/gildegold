/* 0x4eab20, the share (byte / 252) number i (0 .. 3) of the record of the character c's object (+8 .. +0xb); -1.0 when it has none */

float ahm_Share(char *c, int i)
{
    if (ahm_HasRecord(c) != 0) {
        if (i == 0)
            return (float)((AHMCHAR *)c)->obj->rec->share[0] * 0.003968254f;
        if (i == 1)
            return (float)((AHMCHAR *)c)->obj->rec->share[1] * 0.003968254f;
        if (i == 2)
            return (float)((AHMCHAR *)c)->obj->rec->share[2] * 0.003968254f;
        if (i == 3)
            return (float)((AHMCHAR *)c)->obj->rec->share[3] * 0.003968254f;
    }
    return -1.0f;
}
