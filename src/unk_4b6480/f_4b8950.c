/* 0x4b8950, message: c gains 3..5 0x50cc50 (the text gets 0x5085f0(1, ai_OwnPlayer) and the amount) */

int ai_Unk4b8950(G14MSG *m)
{
    AICHAR *c;
    int s;
    char buf[0x200];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    s = util_Random(3) + 3;
    txt_Format(buf, m->type + 0x12e7, gm_PlayerFactor(1, ai_OwnPlayer), s);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    gm_SendMoneyDelta(c->netId, s);
    return 0;
}
