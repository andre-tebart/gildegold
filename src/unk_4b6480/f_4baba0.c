/* 0x4baba0, message: as 0x4baa80 (5..24 %, the share first in the text) */

int ai_Unk4baba0(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    float f;
    int id;
    char buf[0x200];

    f = (float)((util_Random(0x14) + 5.0) * 0.01);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!ai_InventoryHas(c, 0x290) && ai_Unk4ba840(c, &id, f) && (b = gm_BuildQuery(1, 1, id)) != 0) {
        txt_Format(buf, m->type + 0x12e7, (int)(f * 100.0f), b);
        he_MessageBoxQuickjumpI(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, *(int *)((char *)b + 1), -1, 0, 0);
        return 0;
    }
    return 0x400;
}
