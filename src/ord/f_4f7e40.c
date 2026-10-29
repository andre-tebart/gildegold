/* 0x4f7e40, an order handler (c, m; kinds 1 / 3): clears the text at m + 0x14; not for characters with the flag 0x100 of +0x1c8, they need 0x200 of +0x1cc; text 0x1e15 for a player, 9 with the flag 0x200, 1 with 0x40000 of +0x1c8; kind 3 runs 0x5cede0, result | 2 */

int ord_InterrogationOrder2(char *c, int *m)
{
    int r;

    r = 0;
    ORD_MSG(m)->text[0] = 0;
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1c8 & 0x100) != 0)
        return 1;
    if ((ORD_CHAR(c)->flags1cc & 0x200) == 0)
        return 1;
    if (ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1e15);
    if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
        return 9;
    if ((ORD_CHAR(c)->flags1c8 & 0x40000) != 0)
        return 1;
    if (m[0] == 3)
        r = ai_InterrogationWindow1(c, m);
    return r | 2;
}
