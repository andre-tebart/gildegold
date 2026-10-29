/* 0x4f52e0, an order handler (c, m; kinds 1 / 3): characters below level 3 (c[0xd]) are not handled; kind 3 goes to 0x5c9ff0, kind 1 of a player writes the text 0x1d60; else 2 */

int ord_BlackmailContactVariantOrder(char *c, int *m)
{
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 3)
        return 1;
    if (m[0] == 3) {
        ai_BlackmailContactVariant(c, m);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d60);
    return 2;
}
