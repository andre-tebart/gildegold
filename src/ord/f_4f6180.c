/* 0x4f6180, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1da2 (0x253 + the job, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1da1 into m + 0x14; else 2 */

int ord_JobMultiOrder(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x1b && (unsigned char)c[0x166] != 0xf && (unsigned char)c[0x166] != 0x15 && (unsigned char)c[0x166] != 0xd)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1da2, (c[9] != 0 ? 0x27b : 0x253) + (unsigned char)c[0x166]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1da1);
    return 2;
}
