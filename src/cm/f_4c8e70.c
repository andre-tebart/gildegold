/* 0x4c8e70, handler of type 0x22, "cm_ExAddStraftat": a crime record (0x2d bytes) in ai_Unk91f700, the matching
   jobs are ended */

int ai_Unk4c8e70(G17MSG *m, G17STAT *s)
{
    char buf[0x1f0];
    char *c, *rec, *o, *j, *c2;
    int slot, v;
    int a, b;
    int pos[3];

    if (ai_Unk1460ae0 != -1)
        ai_Unk6f1b88 = G17_I(*m, 0);
    else
        ai_Unk6f1b88 = ai_Unk6f1b88 + 1;
    slot = unk_Unk4f36b0();
    if (slot == -1)
        return -1;
    c = gm_FindChar(G17_I(*m, 0x16));
    if (c != 0 && c[2] == 6)
        ai_Unk4b44e0(c, 0x17);
    rec = ai_Unk91f700 + slot * 0x2d;
    memcpy(rec, &m->data[0], 0x2d);
    *(int *)rec = ai_Unk6f1b88;
    sprintf(buf, "cm_ExAddStraftat(): Executed---- st_id = %i, taeter = %i, opfer = %i", ai_Unk6f1b88,
            G17_I(*m, 0x16), G17_I(*m, 0x12));
    v = G17_I(*m, 0x21);
    if (v == -1) {
        c = gm_FindChar(G17_I(*m, 0x12));
        if (c != 0) {
            if (*(char **)(c + 0x170) != 0)
                v = *(int *)(*(char **)(c + 0x170) + 1);
            else if (*(char **)(c + 0x16c) != 0)
                v = *(int *)(*(char **)(c + 0x16c) + 1);
            else
                util_Seed(G17_I(*m, 0x29));
        } else
            util_Seed(G17_I(*m, 0x29));
    }
    ord_AddGridAmount(v, m->data[0x1c], &b, &a);
    gm_FindById(&o, 0, 0, v);
    if (o == 0)
        unk_Unk4f86a0(b, a, pos);
    else if (*(char **)(o + 0x61) != 0) {
        pos[0] = *(int *)(*(char **)(o + 0x61) + 0x4c);
        pos[1] = *(int *)(*(char **)(o + 0x61) + 0x50);
        pos[2] = *(int *)(*(char **)(o + 0x61) + 0x54);
    }
    for (j = he_FindEvent(1, 0, 0x18); j != 0; j = he_NextEvent()) {
        if (*(int *)(j + 0xc4) == *(int *)(rec + 0x16)) {
            if (unk_Unk4f3640(*(int *)rec, ai_CharInts[*(unsigned short *)(j + 8)][0]) == 0)
                break;
            if (s != 0) {
                c2 = gm_FindChar(*(int *)(j + 0xc4));
                if (c2 != 0)
                    ai_Unk5a9170(c2, ai_Chars[*(unsigned short *)(j + 8)]);
            }
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 6;
        s->value = (int)rec;
    }
    return 0;
}
