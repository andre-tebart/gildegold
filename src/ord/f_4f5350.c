/* 0x4f5350, an order handler (c, m; kinds 1 / 3): not for level below 3 or the flag 0x400 of +0x1c8; kind 3 goes to 0x5c9c50, kind 1 of a player writes the text 0x1d65; else 2 */

int ord_BlackmailContactOrder(char *c, int *m)
{
    if ((m[0] == 3 || m[0] == 1) && (unsigned char)c[0xd] >= 3 && (ORD_CHAR(c)->flags1c8 & 0x400) == 0) {
        if (m[0] == 3) {
            ai_BlackmailContact(c, m);
            return 2;
        }
        if (m[0] == 1 && c[2] == 6)
            txt_Format(m + 5, "%s", 0x1d65);
        return 2;
    }
    return 1;
}
