/* 0x4cb250, handler of type 0x3d: creates a character (kind data[8]) for building data[0] under the boss data[4] */

int ai_Unk4cb250(G17MSG *m, G17STAT *s)
{
    char *b, *c, *bo = 0, *t;
    int *r;
    int k;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0xf);
    else
        G17_I(*m, 0x13) = rand();
    util_Seed(G17_I(*m, 0x13));
    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    c = gm_FindChar(G17_I(*m, 9));
    if (G17_I(*m, 4) != -1) {
        bo = gm_BuildQuery(1, 1, G17_I(*m, 4));
        if (bo == 0)
            return 1;
    } else if (c != 0)
        bo = *(char **)(c + 0x170);
    if (c == 0 || *(int *)(c + 0x16c) != 0)
        c = unk_Unk4fddb0(6, m->data[8], 0x12, m->data[0xe], m->data[0xd], bo);
    if (c == 0) {
        if (s != 0) {
            s->state = 2;
            s->unk01 = 1;
            s->value = 0;
        }
        return 2;
    }
    if (m->data[0xd] != 0)
        c[0x165] = m->data[0xd];
    if (m->data[0xe] != 0)
        c[0x164] = m->data[0xe];
    else
        c[0x164] = 0;
    c[2] = m->data[8];
    if (*(int *)(c + 0x184) == 0) {
        if (bo != 0) {
            t = unk_Unk4fdc10(bo);
            if (t != 0 && *(int *)(t + 2) != -1)
                gm_PlaceOnStage(*(int *)(c + 4), *(int *)(bo + 1), *(int *)(t + 2), 0, 0);
        } else {
            t = gm_BuildingMainObject(b);
            if (t != 0 && *(int *)(t + 2) != -1)
                gm_PlaceOnStage(*(int *)(c + 4), *(int *)(b + 1), *(int *)(t + 2), 0, 0);
        }
    }
    k = m->data[0xd];
    if (k != 2 && k != 1 && k != 0x10 && k != 0xf && k != 0x14 && k != 0x13) {
        r = (int *)ai_Unk4cda50(c);
        if (*r != 0) {
            gm_TakeObjekt(*(int *)(c + 4), *r, 1);
            ai_Unk4cde20(c);
        }
    } else {
        r = (int *)ai_Unk4cda50(c);
        if (*r == 0) {
            gm_CreateObject(*(int *)(c + 4), 0x154, 1);
            ai_Unk4cde20(c);
        }
    }
    *(char **)(c + 0x16c) = b;
    if (bo != 0)
        unk_Unk4fda10(c, bo);
    *(int *)(c + 0x174) = 0;
    *(int *)(c + 0x1c8) &= ~0x2000;
    ai_ResetJob(c);
    *(G17B16 *)(c + 0x1b4) = *(G17B16 *)(b + 0x99);
    if (m->data[0xe] != 0)
        *(int *)(c + 0x1b8) = *(int *)(b + 0x39);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)c;
    }
    return 0;
}
