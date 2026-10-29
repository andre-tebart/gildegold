/* 0x4e6680, sends the office message (cm_SendOfficeRecord, message 0x45): the office a, the ids of the characters b and c (their +4, or -1),
   the kind d and the code e; returns the send slot of the message, -1 when a is not one of the 42 office records */

int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e)
{
    AMTMSG m;
    int i, netIdB, netIdC;
    AMTREC *p;

    netIdB = -1;
    netIdC = -1;
    if (b != 0)
        netIdB = ((AMTCHARVIEW *)b)->netId;
    if (c != 0)
        netIdC = ((AMTCHARVIEW *)c)->netId;
    i = 0;
    p = amt_Table;
    do {
        if (p->idx == a)
            break;
        p++;
        i++;
        if ((int)p >= (int)&amt_Table[42])
            return -1;
    } while (1);
    if (i >= 42)
        return -1;
    m.office = a;
    m.a = netIdB;
    m.d = d;
    m.e = e;
    m.b = netIdC;
    return cm_SendOfficeRecord(&m);
}
