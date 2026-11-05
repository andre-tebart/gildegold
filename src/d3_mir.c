/* d3_mir.c -- mirrors ("d3_mir:" allocation tags), original file name unknown: 0x454780..0x455630.
   The outline of the mirror polygon becomes a set of clipping planes; the objects whose mirrored bounding box
   is inside them are drawn a second time, mirrored at the mirror plane. Names are ours.
   Open: d3_engine.h declares d3_MirBeginFrame / d3_MirDrawObject with other parameter types than d3_mir.h.
   unit of rea 8cef7d5b (src/game/d3_mir.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
/* d3_engine.h declares these two with other parameter types */
#define d3_MirBeginFrame d3_Unk4554b0_decl
#define d3_MirDrawObject d3_Unk4550e0_decl
#include "d3_engine.h"
#undef d3_MirBeginFrame
#undef d3_MirDrawObject
#include "d3_mir.h"
#include "data_views.h"
#include "ts_texture.h"
#include "d3_pose.h"
#define d3_TriNormalM ((void (*)(D3VECTOR *, D3PVERT *, D3PVERT *, D3VECTOR4 *))d3_TriNormal)  /* 0x454700 (d3_vec.h: D3VECTOR4 * x4): this unit passes the origin and its polygon vertices */
#define d3_DirToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_DirToRef)  /* 0x458950 (d3_pose.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */

#define D3M_SURF(o) ((D3MIRSURF *)(o)->info)
#define D3M_PTS(mesh) ((mesh)->mirrorPoints)                     /* "d3_mir:MirroredPoints" */
#define D3M_TRIS(mesh) ((mesh)->mirrorPolys)                     /* "d3_mir:MirroredPolys" */

/* 0x454780, the boundary edges of the point set pts: for every pair (i, j) not taken yet the triangle
   (origin, i, j) is a hull edge when all other points lie on its positive side; the pairs go to out (2 per
   edge, *count of them), used is the n x n matrix that marks the taken pairs */
char d3_MirBoundaryEdges(unsigned int n, unsigned char *used, D3PVERT **pts, unsigned int *count, D3PVERT **out)
{
    D3VECTOR4 nrm;
    D3PVERT *p;
    unsigned char *row;
    unsigned i, j, k;

    *count = 0;
    for (i = 0; i < n; i++) {
        row = used + i;
            for (j = 0; j < n; j++, row += n) {
                if (i != j && used[i * n + j] == 0) {
                    d3_TriNormalM((D3VECTOR *)&d3_Origin, pts[i], pts[j], &nrm);
                    for (k = 0; k < n; k++) {
                        if (k != i && k != j) {
                            p = pts[k];
                            if (nrm.x * p->x + nrm.y * p->y + nrm.z * p->z < -0.01)
                                break;
                        }
                    }
                    if (k == n) {
                        out[(*count)++] = pts[i];
                        out[(*count)++] = pts[j];
                        *row = 1;
                        used[i * n + j] = 1;
                    }
                }
            }
    }
    return 1;
}
/* 0x454890, d3_mir:CreateOutline: turns the boundary edges of the mirror polygon (d3_MirBoundaryEdges: pairs of
   vertices in edges[], nedge of them) into the outline segments: connected edges that are collinear are
   merged, *out gets the (start, end) vertex pairs, *nout their count (2 per segment). Returns 0 only for n < 3.
   used[0..nedge) = edge taken by the current chain, used[nedge..2*nedge) (mark) = edge taken by any chain */

char d3_MirCreateOutline(D3PVERT **pts, unsigned int n, unsigned int *nout, D3PVERT ***out)
{
    D3PVERT **edges;
    unsigned char *used;
    unsigned char *mark;
    D3PVERT **res;
    D3PVERT *start, *end, *p, *q;
    D3VECTOR4 dir, d, nd;
    unsigned int i, j, nres, tail, head, nedge;

    /* dead stores: they rank mark / tail for the operand order of VC6's addresses (2 of 4 orders) */
    mark = 0;
    tail = 0;
    if (n < 3)
        return 0;
    edges = mem_Alloc(n << 5, "d3_mir:CreateOutlineMaxOut");
    used = mem_Alloc((n + 8) * n, "d3_mir:track_connections");
    d3_MirBoundaryEdges(n, used, pts, &nedge, edges);
    i = nedge;
    res = mem_Alloc(i * 4, "d3_mir:CreateOutlineOutput");
    nres = 0;
    if (i > 0) {
        mark = used + i;
        D3_ZERO_AT(used, i * 2);
        mark[0] = 1;
        used[0] = 1;
        end = edges[1];
        head = tail = 0;
        start = edges[0];
        dir.x = end->x - start->x;
        dir.y = end->y - start->y;
        dir.z = end->z - start->z;
        d3_Normalize(&dir);
        do {
            for (i = 0; i < nedge; i += 2) {
                if (!used[i] && (p = edges[i]) == end) {
                    q = edges[i + 1];
                    d.x = p->x - q->x;
                    d.y = p->y - q->y;
                    d.z = p->z - q->z;
                    d3_Normalize(&d);
                    nd.x = -d.x;
                    nd.y = -d.y;
                    nd.z = -d.z;
                    if (d3_VecEqual((D3VECTOR *)&dir, (D3VECTOR *)&d, 0.001f)
                        || d3_VecEqual((D3VECTOR *)&dir, (D3VECTOR *)&nd, 0.001f)) {
                        used[i] = 1;
                        mark[tail] = 1;
                        tail = i;
                        end = edges[i + 1];
                        break;
                    }
                }
                if (!used[i] && (p = edges[i + 1]) == start) {
                    q = edges[i];
                    d.x = p->x - q->x;
                    d.y = p->y - q->y;
                    d.z = p->z - q->z;
                    d3_Normalize(&d);
                    nd.x = -d.x;
                    nd.y = -d.y;
                    nd.z = -d.z;
                    if (d3_VecEqual((D3VECTOR *)&dir, (D3VECTOR *)&d, 0.001f)
                        || d3_VecEqual((D3VECTOR *)&dir, (D3VECTOR *)&nd, 0.001f)) {
                        used[i] = 1;
                        /* used[nedge + head], not mark[head]: the form that gives VC6's operand order */
                        used[nedge + head] = 1;
                        head = i;
                        start = edges[i];
                        break;
                    }
                }
            }
            if (i >= nedge) {
                for (j = 0; j < nres; j += 2) {
                    q = res[j + 1];
                    if ((res[j] == start && res[j + 1] == end) || (res[j + 1] == start && res[j] == end))
                        break;
                }
                if (j >= nres) {
                    res[nres++] = start;
                    res[nres++] = end;
                }
                /* next unmarked edge starts a new outline; none left ends the do loop (start stays NULL;
                   this form, not a test after the scan, gives VC6's threaded exit) */
                start = NULL;
                for (i = 0; i < nedge; i += 2) {
                    if (!mark[i]) {
                        D3M_ZERO_VAR(used, pts);
                        tail = head = i;
                        mark[i] = 1;
                        used[i] = 1;
                        end = edges[i + 1];
                        start = edges[i];
                        dir.x = end->x - start->x;
                        dir.y = end->y - start->y;
                        dir.z = end->z - start->z;
                        d3_Normalize(&dir);
                        break;
                    }
                }
            }
        } while (start);
    }
done:
    mem_Free(used);
    mem_Free(edges);
    *nout = nres;
    *out = res;
    return 1;
}
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
/* 0x454ec0, d3_Mirror.Transform: mirrors camera-space points at the mirror plane and projects them again */

void d3_MirTransform(D3PVERT *v, int n, int unk)
{
    int i;
    float d, z;

    for (i = 0; i < n; i++, v++) {
        d = (d3_Mirror.plane.x * v->x + d3_Mirror.plane.y * v->y + d3_Mirror.plane.z * v->z - d3_Mirror.plane.w) * -2.0f;
        v->x += d * d3_Mirror.plane.x;
        v->y += d * d3_Mirror.plane.y;
        v->z += d * d3_Mirror.plane.z;
        v->rhw = d;
        z = 1.0f / v->z;
        v->sx = d3_ScaleX * v->x * z + d3_CenterX;
        v->sy = d3_ScaleY * v->y * z + d3_CenterY;
    }
}
/* 0x454f70: is the mirrored bounding box of obj inside the mirror's clipping planes? Widens d3_MinZ / d3_MaxZ. */

char d3_MirBoxInPlanes(D3OBJECT *obj, D3MIRROR *m)
{
    D3OBJINFO *info;
    D3MIRPLANES *pl;
    D3BOXVERT *box;
    D3VECTOR4 pts[8];
    D3VECTOR4 *p;
    float d;
    int i;
    unsigned int j, k;

    if (obj && (info = obj->info) && info->mesh && m && (pl = m->planes)) {
        if (obj->state & 0x10)
            box = D3_OPS(info)->GetBox(obj, info, D3_MESH(obj)->cameraMatrix);
        else
            box = (D3BOXVERT *)&info->keys[info->nkeys];
        for (i = 0; i < 8; i++, box++) {
            d = (box->x * m->plane.x + box->y * (m->plane.y) + (box->z) * (m->plane.z) - m->plane.w) * -2.0f;
            pts[i].x = d * m->plane.x + box->x;
            pts[i].y = d * m->plane.y + box->y;
            pts[i].z = d * m->plane.z + box->z;
        }
        for (j = 0; j < pl->n; j++) {
            for (k = 0; k < 8; k++)
                if (pts[k].x * pl->p[j].x + pts[k].y * pl->p[j].y + (pts[k].z * pl->p[j].z) >= pl->p[j].w)
                    break;
            if (k >= 8)
                return 0;
        }
        for (i = 0; i < 8; i++) {
            d3_MinZ = min(d3_MinZ, pts[i].z);
            d3_MaxZ = max(d3_MaxZ, pts[i].z);
        }
        return 1;
    }
    return 0;
}
/* 0x4550e0, d3_ForEachObject callback (ctx = &d3_Mirror): draws the mirror image of obj */

char d3_MirDrawObject(D3OBJECT *obj, D3MIRROR *m)
{
    D3MIRSURF *s;
    D3MESH *mesh;
    D3PTRI *t;
    D3TEXTURE *tex, *tt;
    D3PVERT *a, *b, *c;
    D3MIRBITS *bits;
    char redo;
    int i, k, n;

    s = D3M_SURF(obj);
    if (!s)
        return 1;
    if (obj == d3s_Object)
        return 1;
    bits = (D3MIRBITS *)&obj->objectFlags;
    bits->mirrored = d3_MirBoxInPlanes(obj, m);
    if (bits->mirrored) {
        if (m->flags.on || (obj->state & 8))
            redo = 1;
        else
            redo = 0;
        if (s != (D3MIRSURF *)D3_MESH(obj)->mirrorSource) {
            if (D3_MESH(obj)->mirrorSource) {
                mem_Free(D3M_PTS(D3_MESH(obj)));
                D3M_PTS(D3_MESH(obj)) = 0;
                mem_Free(D3M_TRIS(D3_MESH(obj)));
                D3M_TRIS(D3_MESH(obj)) = 0;
            }
            D3_MESH(obj)->mirrorSource = (D3SURF *)s;
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
                    ((TSTEXTURE *)tt)->stamp = d3_FrameCount;
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
                d3_DirToRefD(obj, d3_Camera, &((D3SRCPOLY *)m->tri->src)->normal, &m->plane);
                m->plane.w = m->plane.x * m->tri->v[0]->x + m->plane.y * m->tri->v[0]->y + m->plane.z * m->tri->v[0]->z;
                m->planes = d3_MirCreatePlanes(m->obj, m->tex);
            }
        }
        if (m->planes)
            d3_MirrorObject = (int)obj;
    }
}

D3MIRROR d3_Mirror = {0};  /* 0xfb9900 (d3_Unkfb9900.. in d3_engine.h) */
