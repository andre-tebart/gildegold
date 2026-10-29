/* 0x4f7990, an order handler (c, m; kind 1 or 3) for level 9 and up (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1e06 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e05 into m + 0x14; else 2 */

int ord_Level9UpOrder(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 9)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1e06, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1e05);
    return 2;
}
