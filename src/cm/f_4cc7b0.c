/* 0x4cc7b0, handler of type 0x55: sub-commands data[4] (1..7) for the character data[0x25] when the message is
   for our player */

int ai_Unk4cc7b0(G17MSG *m, G17STAT *s)
{
    char *c;

    if (ai_Unk6f0d90 == 0 || *ai_Unk6f0d90 != G17_I(*m, 0))
        return 1;
    c = ai_Unk4ce130(G17_I(*m, 0x25));
    switch (m->data[4]) {
    case 2:
        if (c[8] != 0) {
            ai_Unk4cc720(c, (float *)&m->data[5], (float *)&m->data[0x15], G17_I(*m, 0x21));
            ai_Unk4d6ba0(&m->data[0x25]);
        }
        break;
    case 6:
        if (c[8] != 0) {
            ai_Unk4cc720(c, (float *)&m->data[5], (float *)&m->data[0x15], G17_I(*m, 0x21));
            ai_Unk4d6f60(&m->data[0x25]);
        }
        break;
    case 1:
        if (c[8] != 0)
            ai_Unk4d6f90(&m->data[0x25]);
        break;
    case 4:
        if (c[8] != 0)
            ai_Unk4d7000(&m->data[0x25]);
        break;
    case 5:
        if (c[8] != 0)
            ai_Unk4d70a0(&m->data[0x25]);
        break;
    case 7:
        ai_Unk4d7150(&m->data[0x25]);
        break;
    case 3:
        if (c[8] != 0) {
            ai_Unk4cc720(c, (float *)&m->data[5], (float *)&m->data[0x15], G17_I(*m, 0x21));
            ai_Unk4d7190(&m->data[0x25]);
        }
        break;
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = 0;
    }
    return 0;
}
