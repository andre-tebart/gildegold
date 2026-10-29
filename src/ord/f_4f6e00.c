/* 0x4f6e00, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0xf, 0x15, 0x1b (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cb250, a player's text 0x1dc8 (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_CounterEspionageWindowOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dc8);
        j = ORD_CHAR(c)->job;
        if (j != 0xf && j != 0x15 && j != 0x1b)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_CounterEspionageWindow(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0xf && j != 0x15 && j != 0x1b)
            return 1;
        if (m[0] == 4) {
            ai_CounterEspionageWindow(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dc8);
        return 0xa;
    }
    return 1;
}
