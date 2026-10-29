/* 0x4cd760, kind 2: the 0x57-byte record p (its id is set here) */

void ai_Unk4cd760(char *p)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    *(int *)p = ai_Unk9367d0;
    G17_I(m, 0) = 2;
    memcpy(&m.data[4], p, 0x57);
    ai_Unk4c39c0(&m);
}
