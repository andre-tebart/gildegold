/* 0x4baf30, message: c gains 8..24 of one of the skills 0x80..0x84 (up to 0xfc) through 0x4c6020 */

int ai_Unk4baf30(G14MSG *m)
{
    AICHAR *c;
    int s, d;
    char buf[0x100];

    s = util_Random(5);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    d = 0xfc - ((unsigned char *)c)[0x80 + s];
    if (d <= 0)
        return 0x400;
    ai_Unk4c6020(c->netId, s, (char)min(d, util_Random(0x11) + 8));
    txt_Format(buf, m->type + 0x12e7, s + 0x16b7);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
