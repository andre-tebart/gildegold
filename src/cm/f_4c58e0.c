/* 0x4c58e0, message 0x45: sends the 15-byte office record p (amt_SendOfficeMessage's AMTMSG; the int id, the two character ids, the
   kind and the code) and returns the send slot of 0x4c39c0 */

int cm_SendOfficeRecord(char *p)
{
    G17MSG m;

    m.type = 0x45;
    *(G17B15 *)m.data = *(G17B15 *)p;
    return ai_Unk4c39c0(&m);
}
