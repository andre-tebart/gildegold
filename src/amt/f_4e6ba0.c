/* 0x4e6ba0, sends the office-swap request (message 0x5c, the AMTTRADE m) of the requester a for the characters b and
   c (their offices from 0x4e6030) when both offices exist and their kinds have the same column and level; m.v = 6.
   Returns the message slot (the return value of cm_SendTradeMessage), -1 when a, b or c is 0 or has no office. The
   counter-espionage wrapper 0x5cb1e0 (unk_58bbd0.c) waits on the slot. */

int amt_SendOfficeSwap(char *requester, char *b, char *c)
{
    AMTKIND k1, k2;
    AMTREC o1, o2;
    AMTTRADE m;

    if (requester != 0 && b != 0 && c != 0) {
        if (amt_FindOffice(b, &k1, &o1) != 0) {
            if (amt_FindOffice(c, &k2, &o2) != 0 && k1.level == k2.level && k1.col == k2.col) {
                m.id = ((AMTCHARVIEW *)requester)->netId;
                m.a = o1.idx;
                m.b = o2.idx;
                m.v = 6;
                return ((int (*)(char *))cm_SendTradeMessage)(&m);
            }
        }
    }
    return -1;
}
