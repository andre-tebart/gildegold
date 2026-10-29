/* 0x4f67c0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the jobs 0x13 / 0x14 / 0x18 / 0x1a, runs 0x5cae00, text 0x1db6 */

int ord_CounterEspionageOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (c[2] == 6)
            txt_Format(m + 5, "%s", 0x1db6);
        j = ORD_CHAR(c)->job;
        if (j != 0x13 && j != 0x14 && j != 0x18 && j != 0x1a)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_CounterEspionage(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x13 && j != 0x14 && j != 0x18 && j != 0x1a)
            return 1;
        if (m[0] == 4) {
            ai_CounterEspionage(c, m);
            return 0xa;
        }
        if (c[2] == 6)
            txt_Format(m + 5, "%s", 0x1db6);
        return 0xa;
    }
    return 1;
}
