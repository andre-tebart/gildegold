/* 0x4f6c60, an order handler (c, m; kinds 1 / 3) for one job (+0x166; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dc5 (ORD_CHAR(c)->id) shown through 0x4f8200 (mode 2); kind 1 writes text 0x1dc4 into m + 0x14; else 2 */

int ord_Job10Order(char *c, int *m)
{
    char buf[0x800];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0xa)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dc5, ORD_CHAR(c)->id);
        ord_PrivilegeDialog(buf, 2, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dc4);
    return 2;
}
