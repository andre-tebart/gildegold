/* 0x4b8c80, message: as 0x4b8a10, but the chosen job is stamped with the time (0x52), 0x4c5010 / 0x4c4c60 are told,
   and the text gets the value s of the object and its 0x4ffe90 (2, 5, 6) goods (0x5087f0) */

int ai_MsgJobTime(G14MSG *m)
{
    AICHAR *c;
    AIJOB *j;
    char *o;
    AIOBJ *e;
    AIBLD *w, *u;
    int n, r, s;
    AIJOB *js[8];
    char *os[8];
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
    r = util_RandomU(n);
    o = os[r];
    j = js[r];
    s = (int)G14_5087F0(*(unsigned short *)o, o[0x12]);
    for (e = gm_Query(*(int *)(o + 0x14), 2, 5, 6); e != 0; e = gm_QueryNext())
        s = (int)(G14_5087F0(*(unsigned short *)e, o[0x12]) * *(int *)((char *)e + 0xe) + s);
    *(AITIME *)((char *)j + 0x52) = ai_TimeT;
    ai_Unk4c5010(-1, -1, *(int *)(o + 2));
    cm_SendExSetHEI(-1, j);
    G8_4FEA50((int *)&w, 0, 0, *(int *)((char *)j + 0xb0));
    G8_4FEA50((int *)&u, 0, 0, *(int *)((char *)j + 0xb4));
    if (w != 0 && u != 0) {
        if (*(char *)w == 0x47)
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[*(int *)((char *)w + 0x65)], ai_PlayerSlots[0], s);
        else
            txt_Format(buf, m->type + 0x12e7, ai_PlayerSlots[0], ai_PlayerSlots[*(int *)((char *)u + 0x65)], s);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x5ef, 0);
    }
    return 0;
}
