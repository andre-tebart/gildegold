/* 0x4c52e0, message 0x35 about character id and building bld; c -1 and not f: the building's object (or -1, flag 1) */

int ai_Unk4c52e0(int id, int bld, int c, const char *s, int f, const char *s2)
{
    G17MSG m;
    char *ch, *b, *o;

    m.type = 0x35;
    G17_I(m, 0) = id;
    G17_I(m, 4) = bld;
    G17_I(m, 8) = c;
    m.data[0xc] = (unsigned char)f;
    if (s2 == 0)
        m.data[0x2d] = 0;
    else
        strncpy((char *)&m.data[0x2d], s2, 0x1f);
    if (s == 0)
        m.data[0xd] = 0;
    else
        strcpy((char *)&m.data[0xd], s);
    ch = gm_FindChar(id);
    if (ch != 0 && ch[8] == 0)
        return -1;
    if (f == 0 && c == -1) {
        o = 0;
        b = gm_FindBuildingById(bld);
        if (b != 0) {
            if (gm_IsHomeType(b) == 0)
                o = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(b);
            else
                o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
        }
        if (o != 0) {
            G17_I(m, 8) = *(int *)(o + 2);
        } else {
            m.data[0xc] = 1;
            G17_I(m, 8) = -1;
        }
    }
    return ai_Unk4c39c0(&m);
}
