/* 0x4cce10, handler of type 0x60: kind data[0] (0..6) picks the creator */

int ai_Unk4cce10(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0x17);
    else
        G17_I(*m, 0x1b) = rand();
    switch (G17_I(*m, 0)) {
    case 0:
        r = unk_Unk4f0240(&m->data[0]);
        break;
    case 1:
        r = unk_Unk4f0490(&m->data[0]);
        break;
    case 2:
        r = unk_Unk4f0770(&m->data[0]);
        break;
    case 3:
        r = unk_Unk4f0970(&m->data[0]);
        break;
    case 4:
        r = unk_Unk4f0af0(&m->data[0]);
        break;
    case 5:
        r = unk_Unk4f0c80(&m->data[0]);
        break;
    case 6:
        r = unk_Unk4f0dd0(&m->data[0]);
        break;
    default:
        return -1;
    }
    if (s != 0 && r == 0)
        s->state = 1;
    return r;
}
