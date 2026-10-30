/* 0x4ba470, message: as 0x4ba2d0, the worker loses 5..14 of skill 0x81 (not with item 0x290) */

int ai_Unk4ba470(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, i, s;
    void *a[8];
    char buf[0x100];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        if (gm_BuildingClass(*(unsigned char *)b) == 1)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind == 1 && ai_CharsT[i].bld == b) {
            p = &ai_CharsT[i];
            break;
        }
    }
    if (b == 0 || p == 0)
        return 0x400;
    s = p->sk81;
    if (s <= 0)
        return 0x400;
    ai_Unk4c6020(p->netId, 1, -(char)min(s, util_RandomU(10) + 5));
    txt_Format(buf, m->type + 0x12e7, p->id, b, p->id, 0x16b8);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
