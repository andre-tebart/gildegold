/* 0x4c4c60, message 0x1d about p (three AITIMEs, its name from ai_Unk625820); p + 0xac as the long message */

void cm_SendExSetHE(unsigned char k, char *p)
{
    G17MSG m;

    m.type = 0x1d;
    G17_I(m, 0) = *(int *)(p + 4);
    *(G17T14 *)&m.data[4] = *(G17T14 *)(p + 0x44);
    *(G17T14 *)&m.data[0x12] = *(G17T14 *)(p + 0x52);
    *(G17T14 *)&m.data[0x20] = *(G17T14 *)(p + 0x60);
    m.data[0x2e] = k;
    strcpy((char *)&m.data[0x2f], ai_Unk625820[*(unsigned char *)p]);
    ai_Unk4c4480(0xa0, p + 0xac);
    ai_Unk4c39c0(&m);
}
