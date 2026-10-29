/* 0x4f6b40, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) for the job 0xe, text 0x1dc1, blocked by the flag 0x400000 of +0x1c8, runs 0x5ca860, result | 2 (| 0xa) */

int ord_MisappropriationOrder(char *c, int *m)
{
    int r;
    char *t;

    r = 0;
    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(ORD_MSG(m)->text, "%s", 0x1dc1);
        if (ORD_CHAR(c)->job != 0xe)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1c8 & ORD_FLAG_BLOCKED) != 0)
            return 1;
        if (m[0] == 3)
            r = ai_MisappropriationWindow(c, m);
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if (ORD_CHAR(t)->job != 0xe)
            return 1;
        if ((ORD_CHAR(t)->flags1c8 & ORD_FLAG_BLOCKED) != 0)
            return 1;
        if (m[0] == 4) {
            r = ai_MisappropriationWindow(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            txt_Format(ORD_MSG(m)->text, "%s", 0x1dc1);
        return r | 0xa;
    }
    return 1;
}
