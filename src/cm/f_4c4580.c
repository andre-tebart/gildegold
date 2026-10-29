/* 0x4c4580, message 6: ends a group */

void cm_EndGroup(const char *s)
{
    G17MSG m;

    m.type = 6;
    strncpy((char *)m.data, s, 0x80);
    ai_Unk4c39c0(&m);
}
