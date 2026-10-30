/* 0x454be0: the clipping planes of the mirror: one per outline edge of the visible polygons with tex (through
   the eye point), followed by the view clip planes those polygons touch */

D3MIRPLANES *d3_MirCreatePlanes(D3OBJECT *obj, D3TEXTURE *tex)
{
    D3OBJINFO *info;
    D3PTRI *t;
    D3PTRI **polys;
    D3PVERT **pts;
    D3PVERT **uniq;
    D3PVERT *p;
    D3MIRPLANES *ci, *planes;
    unsigned int n, cnt, np, nu, ne, k, j;
    unsigned char clip;

    if (obj && (info = obj->info) && tex) {
        n = info->ntris;
        cnt = 0;
        for (k = 0, t = (D3PTRI *)info->tris; k < n; k++, t++)
            if (t->tex == tex)
                cnt++;
        if (cnt) {
            polys = mem_Alloc(cnt * 4, "d3_mir:CrClippingPolys");
            cnt = 0;
            for (k = 0, t = (D3PTRI *)info->tris; k < n; k++, t++)
                if (t->tex == tex && (t->flags & 0x80))
                    polys[cnt++] = t;
            pts = mem_Alloc(cnt * 3 * 8, "d3_mir:CrClippingPoints");
            uniq = pts + cnt * 3;
            np = 0;
            clip = 0;
            for (k = 0; k < cnt; k++) {
                if (!(polys[k]->v[0]->flags & polys[k]->v[1]->flags & polys[k]->v[2]->flags & 0x3f)) {
                    clip |= (polys[k]->v[0]->flags | polys[k]->v[1]->flags | polys[k]->v[2]->flags) & 0x3f;
                    for (j = 0; j < 3; j++)
                        pts[np++] = polys[k]->v[j];
                }
            }
            mem_Free(polys);
            nu = 0;
            for (k = 0; k < np; k++) {
                p = pts[k];
                for (j = 0; j < np; j++)
                    if (!uniq[j] || uniq[j] == p || d3_VecEqual((D3VECTOR *)uniq[j], (D3VECTOR *)p, 0.001f))
                        break;
                if (j < np && !uniq[j]) {
                    uniq[j] = p;
                    nu++;
                }
            }
            if (nu < 3)
                mem_Free(pts);
            else {
                if (!d3_MirCreateOutline(uniq, nu, &ne, &uniq)) {
                    mem_Free(pts);
                    return 0;
                }
                if (ne < 6) {
                    mem_Free(uniq);
                    mem_Free(pts);
                    return 0;
                }
                mem_Free(pts);
                ne /= 2;
                ci = (D3MIRPLANES *)d3_ClipInfo[clip & 0x3f];
                planes = mem_Alloc((ci->n + ne) * 16 + 8, "d3_mir:CrClippingPlanesFinal");
                planes->n = ci->n + ne;
                planes->mirror = 1;
                for (j = 0; j < ne; j++) {
                    d3_TriNormalM(&d3_Origin, uniq[j * 2], uniq[j * 2 + 1], &planes->p[j]);
                    p = uniq[j * 2];
                    planes->p[j].w = -(p->x * (planes->p[j].x) + p->y * planes->p[j].y + (p->z) * planes->p[j].z);
                }
                mem_Free(uniq);
                D3M_COPY(&planes->p[ne], ci->p, ci->n * 16);
                return planes;
            }
        }
    }
    return 0;
}
