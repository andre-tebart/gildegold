/* 0x4cd6e0, kind 6: a text line (0x3f characters) of this player */

void ai_Unk4cd6e0(char *text, int v)
{
    G17MSG m;
    struct {
        int a, b;
        char name[0x40];
    } t;

    memset(m.data, 0, 0x80);
    t.a = ai_Unk9367d0;
    t.b = v;
    strncpy(t.name, text, 0x3f);
    m.type = 0x5f;
    t.name[0x3f] = 0;
    G17_I(m, 0) = 6;
    memcpy(&m.data[4], &t, 0x48);
    ai_Unk4c39c0(&m);
}
