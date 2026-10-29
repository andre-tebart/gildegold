/* 0x4c78c0, handler of type 0x12: workshop y produces up to n of item from its ingredients (good + 0x26 amounts,
   + 0x2e ids, + 0x36 output per run) */

int ai_Unk4c78c0(G17MSG *m, G17STAT *s)
{
    char *y, *g, *r, *o, *st, *e;
    unsigned short *pp;
    unsigned short id;
    int n, i, v;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0xa];
    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(0, &y, 0, *(int *)&m->data[0]) == 0)
        return 1;
    if (*(short *)y != 0x2a && *(short *)y != 0x116)
        return 1;
    r = gm_Unk50a050(y);
    if (r == 0)
        return 1;
    g = ai_Goods + *(unsigned short *)&m->data[4] * 0x41;
    n = *(int *)&m->data[6];
    pp = (unsigned short *)(g + 0x26);
    for (i = 4; i != 0; i--, pp++) {
        if (pp[4] != 0) {
            o = gm_Query(*(int *)(y + 0x14), 1, 0, pp[4]);
            if (o != 0) {
                if (n >= gm_ListAmount(y, o) / *pp)
                    n = gm_ListAmount(y, o) / *pp;
            } else {
                n = 0;
            }
        }
    }
    if (n >= gm_FreeCapacity(y, *(unsigned short *)&m->data[4], *(unsigned short *)(g + 0x36) * n) / *(unsigned short *)(g + 0x36))
        n = gm_FreeCapacity(y, *(unsigned short *)&m->data[4], *(unsigned short *)(g + 0x36) * n) / *(unsigned short *)(g + 0x36);
    if (n == 0) {
        if (s != 0)
            s->unk01 = 8;
        return 1;
    }
    for (i = 0; i < 4; i++) {
        id = ((unsigned short *)(g + 0x2e))[i];
        if (id != 0) {
            o = gm_Query(*(int *)(y + 0x14), 1, 0, id);
            if (o == 0) {
                if (s != 0)
                    s->unk01 = 8;
                return 1;
            }
            *(int *)(o + 0xe) -= ((unsigned short *)(g + 0x26))[i] * n;
            st = gm_FindGoods(ai_OwnPlayer, id);
            if (st != 0)
                *(int *)(st + 0x14) += *(unsigned short *)(g + 0x36) * n;
            if (*(int *)(o + 0xe) == 0)
                gm_RemoveObject((int *)(y + 0x14), id);
        }
    }
    o = gm_Query(*(int *)(y + 0x14), 1, 0, *(unsigned short *)&m->data[4]);
    if (o != 0) {
        *(int *)(o + 0xe) += *(unsigned short *)(g + 0x36) * n;
    } else {
        o = gm_CreateObject(*(int *)(y + 2), *(unsigned short *)&m->data[4], *(unsigned short *)(g + 0x36) * n);
        if (o == 0) {
            if (s != 0)
                s->unk01 = 6;
            return 1;
        }
        if (*(short *)y == 0x2a)
            (*(int *)(o + 0xe))++;
        if ((g[0] == 0x17 || g[0] == 0x25) && (*(short *)y == 0x2a || *(short *)y == 0x116) && (r[2] == 6 || r[2] == 7))
            *(int *)(o + 0x1c) = 4;
    }
    v = (int)(gm_ItemPrice(*(unsigned short *)&m->data[4], 100) * n);
    e = gm_Unk50a0f0(y);
    if (e != 0)
        *(int *)(e + 0x4d) += v;
    st = gm_FindGoods(ai_OwnPlayer, *(unsigned short *)&m->data[4]);
    if (st != 0)
        *(int *)(st + 0x10) += *(unsigned short *)(g + 0x36) * n;
    e = gm_PlayerRecord(r);
    if (e != 0)
        *(int *)(e + 0x24) += v;
    if (s != 0) {
        s->state = 1;
        if (*(int *)&m->data[0] != -1) {
            s->unk01 = 3;
            s->value = (int)o;
        }
    }
    return 0;
}
