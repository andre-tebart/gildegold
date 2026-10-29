/* 0x4c5d80, message 0x54 (three ints of p) */

void ai_Unk4c5d80(int *p)
{
    G17MSG m;

    m.type = 0x54;
    *(G17B12 *)m.data = *(G17B12 *)p;
    ai_Unk4c39c0(&m);
}
