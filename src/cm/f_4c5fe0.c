/* 0x4c5fe0, message 0x5c: the 10-byte trade record p (the office-swap request of amt_SendOfficeSwap) */

void cm_SendTradeMessage(char *p)
{
    G17MSG m;

    m.type = 0x5c;
    *(G17B10 *)m.data = *(G17B10 *)p;
    ai_Unk4c39c0(&m);
}
