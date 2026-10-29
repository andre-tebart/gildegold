/* 0x4f69a0, an order handler (c, m; kinds 1 / 3) for one job (+0x166; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dbe ((c[9] != 0 ? 0x28c : 0x264)) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1dbd into m + 0x14; else 2 */

int ord_Job17Order2(char *c, int *m)
{
    char buf[0x400];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x11)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dbe, (c[9] != 0 ? 0x28c : 0x264));
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dbd);
    return 2;
}
