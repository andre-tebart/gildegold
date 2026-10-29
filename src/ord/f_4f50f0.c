/* 0x4f50f0, an order handler (c, m; kinds 1 / 3): characters with the byte +0x166 = 0xd are not handled; so are those that already own an order 0x2c (0x53e400 (1, 0, 0x2c) .. +0xac = c's id); kind 3 goes to 0x5cde90, kind 1 of a player gets the text 0x1d44; else 2 */

int ord_SpyTargetOrder(char *c, int *m)
{
    char *o;

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if (ORD_CHAR(c)->job == 0xd)
        return 1;
    o = he_FindEvent(1, 0, 0x2c);
    if (o != 0) {
        do {
            if (((ORDEVENT *)o)->ownerId == ORD_CHAR(c)->netId)
                return 1;
            o = he_NextEvent();
        } while (o != 0);
    }
    if (m[0] == 3) {
        ai_SpyTargetHandler(c, m);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d44);
    return 2;
}
