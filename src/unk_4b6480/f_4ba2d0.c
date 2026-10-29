/* 0x4ba2d0, message: the first worker of kind 1 in one of c's buildings of kind 1 gains 5..14 of skill 0x81 (up to
   0xfc) through 0x4c6020 */

int ai_Unk4ba2d0(G14MSG *m)
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
    for (b = gm_BuildQuery(1, 3, c->id); b != 0 && n < 8; b = gm_BuildNext()) {
        if (gm_BuildingClass(*(unsigned char *)b) == 1)
            a[n++] = b;
    }
    if (n == 0)
        return 0x400;
    b = a[util_Random(n)];
    for (i = 0; i < 768; i++) {
        if (ai_Chars[i].kind == 1 && ai_Chars[i].bld == b) {
            p = &ai_Chars[i];
            break;
        }
    }
    if (b == 0 || p == 0)
        return 0x400;
    s = 0xfc - p->sk81;
    if (s <= 0)
        return 0x400;
    ai_Unk4c6020(p->netId, 1, (char)min(s, util_Random(10) + 5));
    txt_Format(buf, m->type + 0x12e7, 0x16b8, p->id, b);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
