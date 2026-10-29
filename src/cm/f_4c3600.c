/* 0x4c3600, sends message 3 (0xa03ef57c, 0) */

void ai_Unk4c3600(void)
{
    G17MSG m;

    m.type = 3;
    *(int *)&m.data[0] = 0xa03ef57c;
    *(int *)&m.data[4] = 0;
    ai_Unk4c39c0(&m);
}
