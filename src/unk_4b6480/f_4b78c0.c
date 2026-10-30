/* 0x4b78c0, message: a character p found by 0x4a5b50 (flags 0x600c00, 0..100) gets 0x4c4b00 4..10 from c */

int ai_Unk4b78c0(G14MSG *m)
{
    AICHAR *c, *p;
    unsigned short id;
    int fl, n;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    fl = (unsigned)&full_data_600c00;
    if (!ai_Unk4a5b50((AIPER *)c, 1, &fl, 0.0f, 100.0f, &id))
        return 0x400;
    p = &ai_CharsT[id];
    n = util_Random(4) * 2 + 4;
    cm_SendRelations(c->netId, p->netId, n, 1);
    txt_Format(buf, m->type + 0x12e7, p->id, p->id);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
