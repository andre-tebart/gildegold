/* 0x4cd870, the handler of type 0x5f (also 0x4cce00): data[0] is the kind (0..6) of the player list message */

int ai_Unk4cd870(G17MSG *m, G17STAT *s)
{
    char *p;
    struct {
        int a, b;
        char name[0x40];
    } t;

    if (s != 0)
        s->state = 2;
    switch (G17_I(*m, 0)) {
    case 0:
        memcpy(ai_Unk936420, &m->data[4], 0x24);
        break;
    case 1:
        p = ai_Unk4cd380(G17_I(*m, 4));
        if (p == 0)
            return -2;
        *(int *)p = G17_I(*m, 4);
        *(int *)(p + 4) = G17_I(*m, 8);
        break;
    case 2:
        p = ai_Unk4cd350();
        if (p == 0)
            return -1;
        memcpy(p, &m->data[4], 0x57);
        break;
    case 3:
        p = ai_Unk4cd380(G17_I(*m, 4));
        if (p == 0)
            return -2;
        if (G17_I(*m, 8) == 1)
            ai_Unk936408 = 1;
        memset(p, 0, 0x57);
        break;
    case 4:
        p = ai_Unk4cd380(G17_I(*m, 4));
        if (p == 0)
            return -2;
        memcpy(p, &m->data[4], 0x57);
        break;
    case 5:
        p = ai_Unk4cd380(G17_I(*m, 4));
        if (p == 0)
            return -2;
        memcpy(p, &m->data[4], 0x57);
        break;
    case 6:
        memcpy(&t, &m->data[4], 0x48);
        ai_Unk4cd3c0(t.a, t.b, t.name);
        break;
    default:
        return 1;
    }
    if (s != 0)
        s->state = 1;
    ai_Unk936404 = 1;
    return 0;
}
