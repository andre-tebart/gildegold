/* 0x4f65c0, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dac (0x253 + the job, + 0x28 with +9) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1dab into m + 0x14; else 2 */

int ord_JobMultiOrder2(char *c, int *m)
{
    char buf[0x400];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x14 && (unsigned char)c[0x166] != 0x18 && (unsigned char)c[0x166] != 0x19)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dac, (c[9] != 0 ? 0x27b : 0x253) + (unsigned char)c[0x166]);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dab);
    return 2;
}
