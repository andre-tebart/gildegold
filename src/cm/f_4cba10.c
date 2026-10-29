/* 0x4cba10, handler of type 0x49: creates a character (kind data[4], boss building data[7], place data[0xb]) in the
   slot ai_Chars[] */

int ai_Unk4cba10(G17MSG *m, G17STAT *s)
{
    char *bo = 0, *t;
    int id, k;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0);
    else
        G17_I(*m, 0x12) = timeGetTime();
    if (s != 0) {
        s->state = 2;
        s->unk01 = 1;
        s->value = 0;
    }
    util_Seed(G17_I(*m, 0x12));
    if (G17_I(*m, 7) != -1) {
        k = ai_Unk4c4450((int)m, G17_I(*m, 7));
        G17_I(*m, 7) = k;
        bo = gm_BuildQuery(1, 1, k);
        if (bo == 0)
            return 1;
    }
    id = gm_Unk507690(m->data[4], -1, -1, 0x14, 0, m->data[0xf], m->data[0x10], 0);
    if (id == 0xffff)
        return 1;
    if (G17_I(*m, 0xb) != -1 && (t = gm_FindObjectInst(G17_I(*m, 0xb))) != 0)
        gm_PlaceOnStage(ai_CharInts[id][0], *(int *)(bo + 1), *(int *)(t + 2), 0, 0);
    else
        gm_PlaceOnStage(ai_CharInts[id][0], *(int *)(bo + 1), -1, 0, 0);
    *(int *)(ai_Chars[id] + 0x16c) = 0;
    ai_Chars[id][2] = 0xd;
    if (m->data[0x11] != 0) {
        gm_CreateObject(ai_CharInts[id][0], m->data[0x11], 1);
        ai_Unk4cde20(ai_Chars[id]);
    }
    ai_Unk60bd88 = ai_CharInts[id][0];
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)ai_Chars[id];
    }
    return 0;
}
