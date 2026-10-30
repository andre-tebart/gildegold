/* 0x4b82c0, message: one of c's buildings (0x501190(1, 3, id), up to 8 of kind 1 / 2) and one of up to 6 characters
   working there (title set) are chosen; half the time a text and 0x4fca40 of p by -(0.1 .. 0.4 of p's 0x1c float)
   (not with item 0x290) */

int ai_Unk4b82c0(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i, x, y;
    void *a[8];
    char buf[0x200];

    n = 0;
    p = 0;
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
    k = 0;
    b = a[util_Random(n)];
    for (i = 0; i < 768 && k < 6; i++) {
        if (ai_Chars[i].kind != 0xffff && ai_Chars[i].bld == b && ai_Chars[i].title != 0)
            a[k++] = &ai_Chars[i];
    }
    if (k != 0)
        p = a[util_Random(k)];
    if (b == 0 || p == 0)
        return 0x400;
    if (util_Random(2)) {
        txt_Format(buf, m->type + 0x12e7, b, (((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title, p->id);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        x = (int)(*(float *)((char *)p + 0x1c) * 0.3);
        y = (int)(*(float *)((char *)p + 0x1c) * 0.1);
        gm_AddHealthS(p->id, -(util_Random(x) + y));
    }
    return 0;
}
