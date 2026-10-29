/* 0x4f7000, an order handler (c, m; kinds 1 / 3) for the jobs 0x17 / 0x13 (+0x166): a player (+2 = 6) with kind 3 gets the text 0x1dd5 (0x253 + the job, + 0x28 with +9) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1dd4 into m + 0x14; else 2 */

int ord_JobMultiOrder3(char *c, int *m)
{
    char buf[0x400];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x17 && (unsigned char)c[0x166] != 0x13)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dd5, (c[9] != 0 ? 0x27b : 0x253) + (unsigned char)c[0x166]);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dd4);
    return 2;
}
