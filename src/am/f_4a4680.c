/* 0x4a4680, the current prices of the items of group m->key against their base prices */

int ai_Unk4a4680(G7MKT *m)
{
    G7GRP *g;
    int i, k, n;
    float sum, r;

    i = 0;
    for (g = ai_Unk61a348; g->key != m->key; g++) {
        i++;
        if (g + 1 > ai_Unk61a348 + 0x25)
            return 0;
    }
    if (i > 0x24)
        return 0;
    n = 0;
    for (k = 0; k < 10; k++) {
        if (ai_Unk61a348[i].items[k] != 0)
            m->items[n++] = ai_Unk61a348[i].items[k];
    }
    m->n = n;
    if (n == 0)
        return 0;
    m->hi = 0;
    m->lo = 0;
    sum = 0.0f;
    for (k = 0; k < m->n; k++) {
        m->price[k] = (int)gm_GoodsPrice(m->items[k], ai_OwnPlayer);
        m->base[k] = (int)gm_ItemPrice(m->items[k], 100);
        r = (float)m->price[k] / (float)m->base[k];
        m->ratio[k] = r;
        sum += r;
        if (m->ratio[m->hi] < r)
            m->hi = k;
        if (m->ratio[k] < m->ratio[m->lo])
            m->lo = k;
    }
    m->avg = sum / (float)m->n;
    return 1;
}
