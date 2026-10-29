/* 0x4f5f00, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f5d50: the table record 2 of 0x4f2870 (the byte rec[0] goes to m[1] and, for a player, into the text at m + 0x14 through 0x4f2fe0), the flag 0x100000 of +0x1c8 of the character (and of m[0x87] for 2 / 4) blocks; kind 3 / 4 sets ORD_MSG(m)->answerFlag = 2 and runs 0x5ca150; result | 2 (| 0xa for 4 / 2) */

int ord_LawOrder2(char *c, int *m)
{
    int rec[9];
    int r;
    char *t;

    r = 0;
    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->job == 0)
            return 1;
        if (gv_FindTableRecord(c, 2, rec) == 0)
            return 1;
        if (ORD_CHAR(c)->kind == 6)
            gv_TableRecordText(ORD_MSG(m)->text, rec[0] & 0xff);
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1c8 & 0x100000) != 0)
            return 1;
        m[1] = rec[0] & 0xff;
        if (m[0] == 3) {
            ORD_MSG(m)->answerFlag = 2;
            r = ai_LawPrivilegeWindow(c, m);
        }
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if (ORD_CHAR(t)->job == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1c8 & 0x100000) != 0)
            return 1;
        if (gv_FindTableRecord(t, 2, rec) == 0)
            return 1;
        m[1] = rec[0] & 0xff;
        if (m[0] == 4) {
            ORD_MSG(m)->answerFlag = 2;
            r = ai_LawPrivilegeWindow(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            gv_TableRecordText(ORD_MSG(m)->text, rec[0] & 0xff);
        return r | 0xa;
    }
    return 1;
}
