/* 0x4f70f0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x17, 0x13, 0x14, 0xd (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cba90, a player's text 0x1dd6 (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_EspionagePaymentOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dd6);
        j = ORD_CHAR(c)->job;
        if (j != 0x17 && j != 0x13 && j != 0x14 && j != 0xd)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EspionagePayment(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x17 && j != 0x13 && j != 0x14 && j != 0xd)
            return 1;
        if (m[0] == 4) {
            ai_EspionagePayment(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dd6);
        return 0xa;
    }
    return 1;
}
