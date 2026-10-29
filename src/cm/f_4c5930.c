/* 0x4c5930, message 0x46 (two ints, char of p) */

void ai_Unk4c5930(char *p)
{
    G17MSG m;

    m.type = 0x46;
    *(G17B9 *)m.data = *(G17B9 *)p;
    ai_Unk4c39c0(&m);
}
