/* 0x44eb60, the three point indices of a polygon */

void agi_PolyVerts(void *unused, AGICTX *c)
{
    char *t;

    t = AGI_TOK(" ");
    c->polys[c->polyidx].v[0] = atoi(t) + c->ptbase;
    t = AGI_TOK(" ");
    c->polys[c->polyidx].v[1] = atoi(t) + c->ptbase;
    t = AGI_TOK(" ");
    c->polys[c->polyidx].v[2] = atoi(t) + c->ptbase;
}
