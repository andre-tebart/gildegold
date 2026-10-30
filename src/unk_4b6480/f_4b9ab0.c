/* 0x4b9ab0, message: as 0x4b8f70, the text also gets 2..4 */

int ai_Unk4b9ab0(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n, s;
    char *os[8];
    AIJOB *js[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        G14_4FEA50(0, &o, 0, *(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if (*(char *)w == 0x47 || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    s = util_RandomU(3) + 2;
    n = util_RandomU(n);
    j = js[n];
    o = os[n];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (*(char *)w == 0x47)
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)w + 0x65)], ai_PlayerSlots[0], s, ai_PlayerSlots[0]);
    else
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0], ai_PlayerSlots[*(int *)((char *)u + 0x65)], s,
                     ai_PlayerSlots[*(int *)((char *)u + 0x65)]);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    return 0;
}
