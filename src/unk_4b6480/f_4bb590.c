/* 0x4bb590, message if 0x505ad0 of c */

int ai_Unk4bb590(G14MSG *m)
{
    AICHAR *c;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!gm_Unk505ad0(c))
        return 0x400;
    txt_Format(buf, m->type + 0x12e7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
