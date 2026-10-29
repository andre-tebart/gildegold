/* 0x4f7a60, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4): clears the text at m + 0x14; characters need the flag 0x800 of +0x1cc (and m[0x87] for 2 / 4), 0x80000 of +0x1cc blocks; text 0x1e07 for a player, 9 with the flag 0x200 of +0x1c8, kind 3 / 4 run 0x5ce610, result | 2 (| 0xa for 4 / 2) */

int ord_InterrogationOrder(char *c, int *m)
{
    int r;
    char *t;

    r = 0;
    ORD_MSG(m)->text[0] = 0;
    if (m[0] == 3 || m[0] == 1) {
        if ((ORD_CHAR(c)->flags1cc & 0x800) == 0)
            return 1;
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1e07);
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1cc & 0x80000) != 0)
            return 1;
        if (m[0] == 3)
            r = ai_InterrogationWindow(c, m);
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1cc & 0x800) == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1cc & 0x80000) != 0)
            return 1;
        if (m[0] == 4) {
            r = ai_InterrogationWindow(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1e07);
        return r | 0xa;
    }
    return 1;
}
