/* 0x4f75e0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x12, 0x16 (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5ce1a0, a player's text 0x1dec (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_SpyTargetFlagOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dec);
        j = ORD_CHAR(c)->job;
        if (j != 0x12 && j != 0x16)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_SpyTargetFlagHandler(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x12 && j != 0x16)
            return 1;
        if (m[0] == 4) {
            ai_SpyTargetFlagHandler(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dec);
        return 0xa;
    }
    return 1;
}
