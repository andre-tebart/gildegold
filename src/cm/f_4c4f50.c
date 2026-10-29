/* 0x4c4f50, "cm_RequestSendCutInfo": message 0x26 */

void ai_Unk4c4f50(int id, int b)
{
    G17MSG m;
    char buf[0x100];

    m.type = 0x26;
    G17_I(m, 0) = id;
    G17_I(m, 4) = b;
    sprintf(buf, "cm_RequestSendCutInfo(): PlayerId:%i Time:%i", id, d3_Time);
    ai_Unk4c39c0(&m);
}
