/* 0x4f59b0, an order handler (c the character, m the message, kind 1 or 3) for level 6 and up (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d95 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1d94 into m + 0x14; else 2 */

int ord_Level6UpOrder2(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 6)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d95, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d94);
    return 2;
}
