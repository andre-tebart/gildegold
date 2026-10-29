/* 0x4cd820, kind 5 */

void ai_Unk4cd820(int v)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    G17_I(m, 0) = 5;
    G17_I(m, 4) = v;
    ai_Unk4c39c0(&m);
}
