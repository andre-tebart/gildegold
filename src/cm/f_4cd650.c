/* 0x4cd650, kind 1 */

void ai_Unk4cd650(int v)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    G17_I(m, 4) = ai_Unk9367d0;
    m.type = 0x5f;
    G17_I(m, 0) = 1;
    G17_I(m, 8) = v;
    ai_Unk4c39c0(&m);
}
