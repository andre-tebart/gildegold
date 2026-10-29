/* 0x4b8a10, message: one of up to 8 jobs (0x53e400(1, 0, 0xf)) of c's objects whose ends (0xb0 / 0xb4, via
   0x4fea50) are 0x502520 buildings and one of kind 0x47; the object's byte 0x12 drops to 60..94 % (0x4c3c00), the
   text names the player of the 0x47 end (not with item 0x290) */

int ai_Unk4b8a10(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIBLD *w, *u;
    int n, r;
    unsigned char v;
    char *os[8];
    AIJOB *js[8];
    char buf[0x200];

    n = 0;
    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (ai_InventoryHas(c, 0x290))
        return 0x400;
    for (j = he_FindEvent(1, 0, 0xf); j != 0 && n < 8; j = he_NextEvent()) {
        o = gm_FindObjectInst(*(int *)((char *)j + 0xac));
        if (o != 0 && *(int *)(o + 0xa) == c->id) {
            G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
            G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
            if ((u != 0 && gm_IsHomeType(u)) || (w != 0 && gm_IsHomeType(w))) {
                if ((w != 0 && u != 0 && *(char *)w == 0x47) || *(char *)u == 0x47) {
                    js[n] = j;
                    os[n] = o;
                    n++;
                }
            }
        }
    }
    if (n == 0)
        return 0x400;
    r = util_Random(n);
    j = js[r];
    o = os[r];
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    v = (unsigned char)(((unsigned char *)o)[0x12] * ((util_Random(0x23) + 0x3c) * 0.01f));
    cm_BeginFieldBlock(o, *(int *)(o + 2));
    cm_AddFieldDelta(1, 1, 0x12, &v);
    cm_SendFieldDeltas();
    if (*(char *)w == 0x47)
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)w + 0x65)], ai_PlayerSlots[0]);
    else
        txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0], ai_PlayerSlots[*(int *)((char *)u + 0x65)]);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
