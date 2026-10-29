/* 0x4f6270, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1da4 (0x26f, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1da3 into m + 0x14; else 2 */

int ord_Job28Order(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x169] != 0x1c)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1da4, (c[9] != 0 ? 0x297 : 0x26f));
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1da3);
    return 2;
}
