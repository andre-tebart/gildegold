/* 0x4554b0 (from d3_DrawObjectCB, m = &d3_Mirror): if obj has a polygon with a mirror texture, makes it the
   frame's mirror and builds the clipping planes */

void d3_MirBeginFrame(D3OBJECT *obj, D3MIRROR *m)
{
    D3MIRSURF *s;
    D3TEXTURE **tp;
    D3TEXTURE *tex;
    D3PTRI *t;
    unsigned int i, n;
    int k, ntris;

    s = D3M_SURF(obj);
    if (s && s->info.mesh && !d3_MirrorObject) {
        n = D3_OPS(&s->info)->count;
        tp = s->tex;
        for (i = 0; i < n; i++) {
            tex = *tp++;
            if (tex && (tex->flags & 0x20))
                goto found;
        }
        return;
found:
        ntris = s->info.ntris;
        if (m->flags.on)
            m->flags.on = 0;
        m->tex = tex;
        if (m->info != &s->info) {
            for (k = 0, t = (D3PTRI *)s->info.tris; k < ntris; k++, t++) {
                if (t->tex == tex) {
                    m->info = &s->info;
                    m->obj = obj;
                    m->tri = t;
                    m->Transform = d3_MirTransform;
                    break;
                }
            }
        }
        m->flags.on = obj->state >> 3;
        if (m->flags.on) {
            for (k = 0, t = (D3PTRI *)s->info.tris; k < ntris; k++, t++)
                if ((t->flags & 0x80) && t->tex == tex)
                    break;
            m->flags.visible = k < s->info.ntris;
            if (m->planes) {
                mem_Free(m->planes);
                m->planes = 0;
            }
            if (m->flags.visible) {
                d3_DirToRefD(obj, d3_Camera, (D3VECTOR *)((char *)m->tri->src + 0x2c), &m->plane);
                m->plane.w = m->plane.x * m->tri->v[0]->x + m->plane.y * m->tri->v[0]->y + m->plane.z * m->tri->v[0]->z;
                m->planes = d3_MirCreatePlanes(m->obj, m->tex);
            }
        }
        if (m->planes)
            d3_MirrorObject = (int)obj;
    }
}
