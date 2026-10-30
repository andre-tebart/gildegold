/* 0x4baa80, message: as 0x4ba970 (5..19 %), sent with 0x53dee0 and the building */

int ai_Unk4baa80(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    float f;
    int id;
    char buf[0x200];

    f = (float)((util_RandomU(0xf) + 5.0) * 0.01);
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (!ai_InventoryHas(c, 0x290) && ai_Unk4ba840(c, &id, f) && (b = gm_BuildQuery(1, 1, id)) != 0) {
        txt_Format(buf, m->type + 0x12e7, b, (int)(f * 100.0f));
        he_MessageBoxQuickjumpI(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, *(int *)((char *)b + 1), -1, 0, 0);
        return 0;
    }
    return 0x400;
}
