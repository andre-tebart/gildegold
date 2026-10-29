/* 0x4f5190, an order handler (c, m; kinds 1 / 3): not for characters with the flag 0x100 of +0x1c8; kind 3 goes to 0x5c9550, kind 1 of a player writes the text 0x1d54; else 2 */

int ord_BlackmailBuildingOrder(char *c, int *m)
{
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1c8 & 0x100) != 0)
        return 1;
    if (m[0] == 3) {
        ai_BlackmailBuildingListener(c, m);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d54);
    return 2;
}
