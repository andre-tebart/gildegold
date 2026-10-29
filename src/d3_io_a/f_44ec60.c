/* 0x44ec60, the material of a polygon; ends the polygon */

void agi_PolyMat(void *unused, AGICTX *c)
{
    char *t = AGI_TOK(" ");

    c->polys[c->polyidx].tex = -1;
    c->polys[c->polyidx].mat = atoi(t);
    c->polyidx++;
}
