/* 0x451e80, callback: resets the colours of every vertex the light touches */

void d3_LightResetVertexColors(D3OBJECT *obj)
{
    D3LREC *n;
    D3OBJECT *o;
    D3LHIT *p;
    int i, cnt;

    for (n = D3_L(obj)->light->list; n; n = n->next) {
        o = n->obj;
        if (o && D3_L(o)->mesh && !(D3_L(o)->mesh->flags & 0x40) && D3_L(o)->info) {
            cnt = n->n;
            if (D3_L(o)->info->nkeys >= cnt) {
                if (d3_Hardware) {
                    for (i = 0, p = n->v; i < cnt; i++, p++)
                        p->v->lit = p->v->base;
                } else {
                    for (i = 0, p = n->v; i < cnt; i++, p++)
                        p->v->lit.r = p->v->base.r >> 2;
                }
            }
        }
    }
}
