/* 0x4f5c30, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4): clears the text at m + 0x14; kinds 3 / 1: only for characters with +0x166 != 0 (text 0x1d98 for a player), 9 when they have the flag 0x200 of +0x1c8, 1 with 0x200000, kind 3 runs 0x5ca560, result | 2; kinds 4 / 2: only when c has the flag 0x100 and the message's character m[0x87] has +0x166 != 0 and not 0x200000; kind 4 runs 0x5ca560, result | 0xa */

int ord_EmbezzlementOrder(char *c, int *m)
{
    int r;
    char *t;

    r = 0;
    ORD_MSG(m)->text[0] = 0;
    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->job == 0)
            return 1;
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(ORD_MSG(m)->text, "%s", 0x1d98);
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1c8 & 0x200000) != 0)
            return 1;
        if (m[0] == 3)
            r = ai_EmbezzlementPrivilege(c, m);
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if (ORD_CHAR(t)->job == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1c8 & 0x200000) != 0)
            return 1;
        if (m[0] == 4) {
            r = ai_EmbezzlementPrivilege(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            txt_Format(ORD_MSG(m)->text, "%s", 0x1d98);
        return r | 0xa;
    }
    return 1;
}
