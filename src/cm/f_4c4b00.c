/* 0x4c4b00, message 0x1b */

void cm_SendRelations(int a, int b, int c, int d)
{
    G17MSG m;

    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    m.type = 0x1b;
    G17_I(m, 8) = c;
    G17_I(m, 0xc) = d;
    *(float *)&m.data[0x10] = ai_DifficultyScale;
    G17_I(m, 0x14) = (int)ai_DifficultyValue;
    ai_Unk4c39c0(&m);
}
