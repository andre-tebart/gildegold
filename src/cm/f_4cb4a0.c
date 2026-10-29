/* 0x4cb4a0, handler of type 0x3e: the character data[0] gets a new 3D object group (name data[4], like
   "<kind>_<a>_<b>"); its names are split into +0x130 / +0x170 of the record at +0x184 */

int ai_Unk4cb4a0(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    float v1[3], v2[3];
    char *c, *g, *o, *t, *p, *q;
    int save, e;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0 || *(char **)(c + 0x184) == 0)
        return 1;
    g = *(char **)(c + 0x184);
    save = d3_CurrentScene;
    d3if_SelectScene(d3t_SceneIndex(*(int *)(g + 0x88)), 1);
    v1[0] = *(float *)(*(char **)(g + 0x34) + 0x4c);
    v1[1] = *(float *)(*(char **)(g + 0x34) + 0x50);
    v1[2] = *(float *)(*(char **)(g + 0x34) + 0x54);
    v2[0] = *(float *)(*(char **)(g + 0x34) + 0x84);
    v2[1] = *(float *)(*(char **)(g + 0x34) + 0x88);
    v2[2] = *(float *)(*(char **)(g + 0x34) + 0x8c);
    e = *(int *)(*(char **)(g + 0x34) + 0x200);
    t = d3_CreateObject(0, v1, v2, &m->data[4]);
    if (t == 0) {
        d3if_SelectScene(save, 1);
        return 1;
    }
    strcpy(t, *(char **)(g + 0x34));
    strcpy(g + 8, (char *)&m->data[4]);
    d3if_Delete((struct D3OBJECT *)*(int *)(g + 0x34));
    *(char **)(g + 0x34) = t;
    *(int *)(*(char **)(g + 0x34) + 0x200) = e;
    *(int *)(*(char **)(g + 0x34) + 0x48) = 0x3000000;
    d3_RelightObject(*(D3OBJECT **)(g + 0x34));
    o = *(char **)(g + 0x34);
    o[0x212] = (o[0x212] & 0xf7) | 4;
    *(int *)(*(char **)(g + 0x34) + 0x220) = 2;
    strcpy(buf, (char *)&m->data[4]);
    p = strchr(buf, '_');
    if (p != 0) {
        p++;
        q = strchr(p, '_');
        if (q != 0)
            *q = 0;
        strcpy(g + 0x130, p);
        p[-1] = 0;
        strcpy(g + 0x170, buf);
    } else
        g[0x130] = ch_EmptyName;
    d3if_SelectScene(save, 1);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 1;
        s->value = (int)c;
    }
    return 0;
}
