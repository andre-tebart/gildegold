/* 0x4cd6a0, kind 3 */

void ai_Unk4cd6a0()
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    G17_I(m, 0) = 3;
    G17_I(m, 4) = ai_Unk9367d0;
    ai_Unk4c39c0(&m);
}
