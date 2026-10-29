/* 0x4c4540, message 5: starts a group */

void cm_BeginGroup(const char *s)
{
    G17MSG m;

    m.type = 5;
    strncpy((char *)m.data, s, 0x80);
    ai_Unk4c39c0(&m);
}
