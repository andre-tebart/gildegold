/* 0x4550e0, d3_ForEachObject callback (ctx = &d3_Mirror): draws the mirror image of obj */

char d3_MirDrawObject(D3OBJECT *obj, D3MIRROR *m)
{
    D3MIRSURF *s;
    D3MESH *mesh;
    D3PTRI *t;
    D3TEXTURE *tex, *tt;
    D3PVERT *a, *b, *c;
    char redo;
    int i, k, n;

    s = D3M_SURF(obj);
    if (!s)
        return 1;
    if (obj == d3s_Object)
        return 1;
    D3M_BITS(obj).mirrored = d3_MirBoxInPlanes(obj, m);
    if (D3M_BITS(obj).mirrored) {
        if (m->flags.on || (obj->state & 8))
            redo = 1;
        else
            redo = 0;
        if (s != D3M_SRC(D3_MESH(obj))) {
            if (D3M_SRC(D3_MESH(obj))) {
                mem_Free(D3M_PTS(D3_MESH(obj)));
                D3M_PTS(D3_MESH(obj)) = 0;
                mem_Free(D3M_TRIS(D3_MESH(obj)));
                D3M_TRIS(D3_MESH(obj)) = 0;
            }
            D3M_SRC(D3_MESH(obj)) = s;
            D3M_PTS(D3_MESH(obj)) = mem_Alloc(s->info.nkeys * 0x50, "d3_mir:MirroredPoints");
            D3M_TRIS(D3_MESH(obj)) = mem_Alloc(s->info.ntris * 0x28, "d3_mir:MirroredPolys");
            D3M_COPY(D3M_TRIS(D3_MESH(obj)), s->info.tris, s->info.ntris * 0x28);
            for (i = 0, t = D3M_TRIS(D3_MESH(obj)); i < s->info.ntris; i++, t++) {
                k = 0;
                /* do-while: the original's loop is the bottom-tested pointer walk (`dec edi / jne`) */
                do {
                    t->v[k] = &D3M_PTS(D3_MESH(obj))[((D3SRCPOLY *)t->src)->idx[k]];
                } while (++k < 3);
            }
            redo = 1;
        }
        if ((obj->state & 0x10) || (D3_MESH(obj)->flags & 0x40)) {
            D3_OPS(&s->info)->Transform(obj, &s->info, D3_MESH(obj)->cameraMatrix);
            obj->state &= 0xef;
            redo = 1;
        }
        if (redo) {
            D3M_COPY(D3M_PTS(D3_MESH(obj)), s->info.keys, s->info.nkeys * 0x50);
            m->Transform(D3M_PTS(D3_MESH(obj)), s->info.nkeys, 0);
        } else {
            n = s->info.nkeys;
            b = (D3PVERT *)s->info.keys;
            a = D3M_PTS(D3_MESH(obj));
            for (i = 0; i < n; i++, b++, a++) {
                a->color = b->color;
                d3_MaxZ = max(d3_MaxZ, a->z);
            }
        }
        n = min(s->info.ntris, d3_SortMax - (int)d3_SortCount);
        tex = m->tex;
        for (t = D3M_TRIS(D3_MESH(obj)); n > 0; n--, t++) {
            tt = t->tex;
            a = t->v[0];
            b = t->v[1];
            c = t->v[2];
            if (tt != tex && (a->rhw < 1e-7f || b->rhw < 1e-7f || c->rhw < 1e-7f)
                && ((t->drawFlags & 4) || ((a->sx - b->sx)) * (a->sy - c->sy) < ((a->sy - b->sy)) * (a->sx - c->sx))) {
                d3_SortPtr->item = t;
                if (tt) {
                    d3_SortPtr->key = (tt - (D3TEXTURE *)d3_Textures) + 1;
                    *(int *)((char *)tt + 0x54) = d3_FrameCount;
                } else
                    d3_SortPtr->key = 0;
                t->clip = m->planes;
                d3_SortPtr++;
                d3_SortCount++;
            }
        }
    }
    return 1;
}
