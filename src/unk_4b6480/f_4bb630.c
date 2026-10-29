/* 0x4bb630, message only (types 0x42..0x44) */

int ai_Unk4bb630(G14MSG *m)
{
    AICHAR *c;
    char buf[0x200];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    txt_Format(buf, m->type + 0x12e7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
