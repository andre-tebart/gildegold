/* 0x4b86c0, message: as 0x4b82c0, but needs any worker (title set) in the chosen building; 0x4fca40 of c by
   -(0.05 .. 0.30 of c's 0x1c float) (not with item 0x290) */

int ai_Unk4b86c0(G14MSG *m)
{
    AICHAR *c;
    AIBLD *b;
    int n, i, w, x, y;
    void *a[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        i = gm_BuildingClass(*(unsigned char *)b);
        if (i == 1 || i == 2)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    w = -1;
    b = a[util_RandomU(n)];
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind != 0xffff && ai_CharsT[i].bld == b && ai_CharsT[i].title != 0)
            w = i;
    }
    if (w == -1 || b == 0)
        return 0x400;
    if (util_RandomU(2)) {
        txt_Format(buf, m->type + 0x12e7, b);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        x = (int)(*(float *)((char *)c + 0x1c) * 0.25);
        y = (int)(*(float *)((char *)c + 0x1c) * 0.05);
        gm_AddHealthS(c->id, -(util_RandomU(x) + y));
    }
    return 0;
}
