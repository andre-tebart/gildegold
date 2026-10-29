/* 0x4cd610, sends message 0x5f, kind 0 */

void ai_Unk4cd610()
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    G17_I(m, 0) = 0;
    m.type = 0x5f;
    ai_Unk4c39c0(&m);
}
