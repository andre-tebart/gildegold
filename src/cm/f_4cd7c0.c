/* 0x4cd7c0, kind 4 */

void ai_Unk4cd7c0(char *p)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    *(int *)p = ai_Unk9367d0;
    G17_I(m, 0) = 4;
    memcpy(&m.data[4], p, 0x57);
    ai_Unk4c39c0(&m);
}
