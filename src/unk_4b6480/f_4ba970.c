/* 0x4ba970, message: 0x4ba840 takes 13..25 % of the goods of one of c's buildings; the text names its 0x501190
   (1, 1, id) entry and the share in % (not with item 0x290) */

int ai_Unk4ba970(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    float f;
    int id;
    char buf[0x200];

    f = (float)((util_RandomU(0xd) + 13.0) * 0.01);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!ai_InventoryHas(c, 0x290) && ai_Unk4ba840(c, &id, f) && (b = gm_BuildQuery(1, 1, id)) != 0) {
        txt_Format(buf, m->type + 0x12e7, b, (int)(f * 100.0f));
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        return 0;
    }
    return 0x400;
}
