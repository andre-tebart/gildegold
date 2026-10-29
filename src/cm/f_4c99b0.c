/* 0x4c99b0, handler of type 0x30: the person "sp_<data[0]>" is sent to a building / room (action 0x33) */

int ai_Unk4c99b0(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p, *b = 0, *q = 0, *r, *c, *o;
    int x = -1;
    int k, v, save;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    if (G17_I(*m, 4) != -1) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 4));
        if (b == 0)
            return 2;
    }
    if (G17_I(*m, 8) != -1) {
        q = gm_Query(*(int *)(b + 0x5d), 1, 1, G17_I(*m, 8));
        if (q == 0)
            return 3;
        if (*(unsigned short *)q != 0xfd) {
            k = init_ObjectSceneUniverse(q);
            goto chk;
        }
    }
    if (b != 0 && gm_IsHomeType(b) != 0) {
        k = init_BuildingSceneUniverse(b);
        q = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
        goto chk;
    }
    k = 0;
    goto body;
chk:
    if (k < 0)
        goto end;
body:
    v = -1;
    if (q != 0)
        v = *(int *)(q + 2);
    if (b != 0)
        x = *(int *)(b + 1);
    if (k != 0) {
        c = gm_FindChar(G17_I(*m, 0));
        if (c != 0)
            gm_SelectModelVariant(c);
    }
    ch_AddAction(p, 0x38, 0);
    if (k != 0) {
        r = ch_AddAction(p, 0x33, k, x, v);
        if (r != 0)
            strcpy(r + 0xf0, (char *)&m->data[0xc]);
    } else if (*(char **)(b + 0x61) != 0) {
        r = ch_AddAction(p, 0x33, 0, -1, -1);
        if (r != 0) {
            save = d3_CurrentScene;
            d3if_SelectScene(0, 1);
            o = d3_FindObject(*(int *)(b + 0x61), 0x100, 0, &m->data[0xc]);
            if (o != 0) {
                d3_PointToWorld(o, o + 0x4c, r + 0x180);
                r[0x17c] = 1;
            } else
                strcpy(r + 0xf0, (char *)&m->data[0xc]);
            d3if_SelectScene(save, 1);
        }
    }
    ch_AddAction(p, 0x38, 1);
end:
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
