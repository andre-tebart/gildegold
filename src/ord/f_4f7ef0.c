/* 0x4f7ef0, an order handler (c, m; kinds 1 / 3) for characters with the flag 0x200 of +0x1cc: a player (+2 = 6) with kind 3 gets the text 0x1e1b (0x276, + 0x28 with +9, extra argument 0x16ba) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e1a into m + 0x14; else 2 */

int ord_Flag200Order(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1cc & 0x200) == 0)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        txt_Format(buf, 0x1e1b, (ORD_CHAR(c)->textVariant != 0 ? 0x29e : 0x276), 0x16ba);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1e1a);
    return 2;
}
