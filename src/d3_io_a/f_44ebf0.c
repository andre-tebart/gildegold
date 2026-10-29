/* 0x44ebf0, the texture coordinates of a polygon: three u, then three v */

void agi_PolyUV(void *unused, AGICTX *c)
{
    int i;

    for (i = 0; i < 3; i++)
        c->polys[c->polyidx].uv[i][0] = atof(AGI_TOK(" "));
    for (i = 0; i < 3; i++)
        c->polys[c->polyidx].uv[i][1] = atof(AGI_TOK(" "));
}
