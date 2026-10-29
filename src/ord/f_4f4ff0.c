/* 0x4f4ff0, an order handler for a player (+2 = 6) or a kind 5 character; kind 3 of a player goes to 0x5cd400 (| 2), kind 1 of a player writes text 0x1d35 into m + 0x14; 1 when not handled, else 2 */

int ord_EspionageResultOrder(char *c, int *m)
{
    int r;

    r = 2;
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if (c[2] != 6 && c[2] != 5)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        r = ai_EspionageResult(c, m);
        r |= 2;
    } else if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(ORD_MSG(m)->text, "%s", 0x1d35);
    return r;
}
