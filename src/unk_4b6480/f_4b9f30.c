/* 0x4b9f30, message: a worker (kind 0) of one of c's buildings of kind 1 / 2 is named in the text */

int ai_Unk4b9f30(G14MSG *m)
{
    AICHAR *c, *p;
    AIBLD *b;
    int n, k, i, t;
    void *a[8];
    char buf[0x200];

    n = 0;
    p = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
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
        if (ai_Chars[i].kind == 0 && ai_Chars[i].bld == b && ai_Chars[i].title != 0)
            a[k++] = &ai_Chars[i];
    }
    if (k > 1)
        p = a[util_Random(k)];
    if (b == 0 || p == 0)
        return 0x400;
    t = (((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title;
    txt_Format(buf, m->type + 0x12e7, t, p->id, p->id, t);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
