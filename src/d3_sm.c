/* d3_sm.c -- original file name unknown; allocation tags "d3_sm:...", debug output "d3sm_...".
   Terrain height map with ground types and object shadows, sampled from a terrain.
   Extent in GildeGold.exe (inferred from .data and the call graph): 0x45db40..0x45f610.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_sm.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_sm.h"
#include "data_views.h"
#include "d3_engine.h"
#include "d3_vec.h"
#include "d3s.h"
#include "mem.h"
#include "ts_texture.h"
extern const char d3_ScrollPool[];


void d3_TransformMeshes(void);                                                                /* 0x43d840 */

char d3sm_GroundNames[15][9] = { "_ill*", "_unk*", "SAND", "ERDE", "WIESE", "MOOR", "PFLASTER", "KIESEL", "FELS", "EIS", "WASSER", "WEG", "WEG", "_ill*", "" };  /* 0x5fcfa0, "_ill*", "_unk*", "SAND", ...; TODO define here once .data is matched */

int d3sm_Ground[8] = {0};                   /* 0x633dd8, ground type per terrain material */
D3SM *d3sm_Shadow = 0;                      /* 0x633df8 */
D3TERRAIN *d3sm_Last = 0;                   /* 0x633dfc, terrain d3sm_Ground was built for */
/* 0x45db40, ground type at (x, z) of the terrain */
int d3sm_GetGround(unsigned int x, unsigned int z, D3TERRAIN *t)
{
    int idx, i, k;

    idx = (t->size - 1 & z) * t->size + (t->size - 1 & x);
    if (t->types[idx] & 0x80)
        return 0;
    if (d3sm_Last != t) {
        for (i = 0; i < 8; i++) {
            d3sm_Ground[i] = 1;
            if (D3TERRAIN_NAMES(t)[i][0]) {
                for (k = 0; k < 15; k++) {
                    if (strstr(_strupr(D3TERRAIN_NAMES(t)[i]), d3sm_GroundNames[k])) {
                        d3sm_Ground[i] = k;
                        break;
                    }
                }
            }
        }
        d3sm_Last = t;
    }
    return d3sm_Ground[t->types[idx]];
}
/* 0x45dc00, samples the heights and ground types of the terrain into the map
   @stuck draft 340 ins -> 160 this session. Read against the original: the midpoint fill re-reads h->data
   for the second average after the first store (the source had no a/b/c/d locals: the four corner bytes
   are read in place, VC6 CSEs them into the sum and the first average and reloads data[A] / data[C] for
   the second), the average is int (cdq/sar) and the /5 unsigned (mul 0xcccccccd): kept in the unsigned
   sum; the halving loop is for (; step > 1; step >>= 1) (step = half was 34 ins worse). The step == 0
   branch now matches. Left: the init loop's heights / data address order (order 6 + regs 18 at 0x45de8a:
   ours loads t->heights before the index, the original after) and the midpoint loop's strength-reduction
   temps (slots and the IV update order at its end, code 61 at 0x45df90, 54 at 0x45e10d). Tried: 19
   spellings of the init loop's heights / data index (t->heights[x + z * ts], pointer forms, step * (..)):
   all = control; half = step / 2, while forms (= control or = the for form), (x + 1) * step instead of x *
   step + step (= control), all 720 orders of the three data stores and three type copies (the current
   order is best; p012354 166 ins) (tiebreak: insensitive, 32 samples) prior (its "allocation only" was wrong for the a/b/c/d locals):
   behaviour: 544 code hunks, but all of them are the same operations with other register / stack-slot
   names or in another order (a full read of the asmdiff: 170 ours vs 181 original instructions, no
   statement missing); the instruction multiset differs only by 2 add / 2 xor / 4 mov / 2 lea, the same
   x*step+step masks and xor-zeroing strength-reduced differently, plus 5 fill nops; the C's loops (heights
   by __ftol, count[15] ground-type vote, step-halving midpoint fill) are the same as the Ghidra
   pseudocode, readability already readable; restoring the original's allocation is byte work */

void d3sm_Sample(D3TERRAIN *t, D3SM *h)
{
    unsigned int count[15];
    unsigned int n, ts, step, half, f, x, z, xx, zz, k, best, max, sum;

    float inv;

    n = h->size;
    ts = t->size;
    step = n / ts;
    inv = 1.0f / h->cell.y;
    if (step == 0) {
        f = ts / n;
        for (z = 0; z < n; z++) {
            for (x = 0; x < n; x++) {
                sum = 0;
                for (zz = z * f; zz < z * f + f; zz++) {
                    for (xx = x * f; xx < x * f + f; xx++)
                        sum += (int)((t->heights[zz * ts + xx] * t->axisy.y + t->origin.y - h->origin.y) * inv);
                }
                h->data[z * n + x] = sum / (f * f);
                D3_ZERO(count, sizeof(count));
                for (zz = z * f; zz < z * f + f; zz++) {
                    for (xx = x * f; xx < x * f + f; xx++)
                        count[d3sm_GetGround(xx, zz, t)]++;
                }
                max = 0;
                best = -1;
                for (k = 0; k < 15; k++) {
                    if (count[k] > max) {
                        max = count[k];
                        best = k;
                    }
                }
                if (best != -1)
                    h->entries[z * n + x].type = best;
                else
                    h->entries[z * n + x].type = 0;
            }
        }
        return;
    }
    for (z = 0; z < ts; z++) {
        for (x = 0; x < ts; x++) {
            h->data[(z * n + x) * step] = (int)((t->heights[z * ts + x] * t->axisy.y + t->origin.y - h->origin.y) * inv);
            h->entries[(z * n + x) * step].type = d3sm_GetGround(x, z, t);
        }
    }
    for (; step > 1; step >>= 1) {
        half = step >> 1;
        for (z = 0; z < n / step; z++) {
            for (x = 0; x < n / step; x++) {
                sum = (h->data[z * step * n + x * step] + h->data[z * step * n + (x * step + step & n - 1)]
                       + h->data[(z * step + step & n - 1) * n + x * step]
                       + h->data[(z * step + step & n - 1) * n + (x * step + step & n - 1)]) / 4;
                h->data[z * step * n + x * step + half] = (sum + (h->data[z * step * n + x * step] + h->data[z * step * n + (x * step + step & n - 1)]) * 2) / 5;
                h->data[(z * step + half) * n + x * step] = (sum + (h->data[z * step * n + x * step] + h->data[(z * step + step & n - 1) * n + x * step]) * 2) / 5;
                h->data[(z * step + half) * n + x * step + half] = sum;
                h->entries[z * step * n + x * step + half].type = h->entries[z * step * n + x * step].type;
                h->entries[(z * step + half) * n + x * step].type = h->entries[z * step * n + x * step].type;
                h->entries[(z * step + half) * n + x * step + half].type = h->entries[z * step * n + x * step].type;
            }
        }
    }
}
/* 0x45e180, d3_ForEachObject callback: grows box[0..1] (min, max) by the object's vertices */

char d3sm_BoundsCB(D3OBJECT *obj, D3VECTOR4 *box)
{
    D3SMMESH *m = (D3SMMESH *)obj->info;
    D3SMVERT *v;
    int i, k;

    if (m && (!(obj->stateFlags & 0xc) || (obj->stateFlags & 0x10))) {
        for (i = 0, v = m->keys; i < ((D3SMMESH *)obj->info)->nkeys; i++, v++) {
            for (k = 0; k < 3; k++) {
                (&box[0].x)[k] = min((&box[0].x)[k], (&v->x)[k]);
                (&box[1].x)[k] = max((&box[1].x)[k], (&v->x)[k]);
            }
        }
    }
    return 1;
}
/* 0x45e210, d3_ForEachObject callback: maps the object's vertices into the height map and queues its
   shadow casting triangles
   @stuck draft 123 ins -> shape 12 ins this session (meaning-neutral respellings read off the original: no
   mesh local before the vertex loop, the loop bound re-reads obj->info's nkeys; h->size compared in place
   (VC6's CSE temp is the original's [esp+0x10]); nrm scoped to the triangle loop body (shares inv's slot,
   frame 0x24 -> 0x18); one final return (callee-saved pushes at entry); tri->cast stored before tri->solid
   = 0 (the and 0x3f / xor bitfield form); hmax = a->h; if (hmax <= b->h) hmax = b->h (the int cmp/jg);
   (int)d3_SortCount in the min (jge, as d3_mir / d3_par). Left: the 6-product cross term's operand order
   (fpu 14): the original loads b before c before a in every product (b.gz*c.gx, b.gx*c.gz, b.gx*a.gz,
   b.gz*a.gx, c.gz*a.gx, c.gx*a.gz), ours always loads the gz field first. Tried: all 64 factor
   orientations (= control: VC6 canonicalizes them), 360 term orders and the 3 groupings x6 (VC6
   reassociates the sum; best p412305 8 ins), the same 360 orders combined with a/b/c load orders and the
   gz-before-gx vertex statement order (1440 variants, best 8 ins / fpu 4 regs 4), 30 declaration orders of
   v/a/b/c (= control), the fields as (&v->gx)[k] (= control). The orientation moves with code earlier in
   the function (the gx/gz statement order in the vertex loop changes it), so the operand rank seems to
   come from function-wide leaf numbering, not the expression's text (tiebreak: insensitive, 32 samples)
   prior: behaviour: no meaning error found; its frame / register / bitfield / max idiom hunks are the ones
   fixed above */

char d3sm_ShadowCB(D3OBJECT *obj, D3SM *h)
{
    D3SMMESH *m;
    D3SMTRI *tri;
    D3SMVERT *v, *a, *b, *c;
    float inv[3];
    int i, n, hmax;

    if (obj->info && !((h->mask & (obj->stateFlags >> 2 & 3)) && !(obj->stateFlags & 0x10))) {
        for (i = 0; i < 3; i++)
            inv[i] = 1.0f / (&h->cell.x)[i];
        for (i = 0, v = ((D3SMMESH *)obj->info)->keys; i < ((D3SMMESH *)obj->info)->nkeys; i++, v++) {
            v->gx = (v->x - h->origin.x) * inv[0] + 0.875f;
            v->gz = (v->z - h->origin.z) * inv[2] + 0.875f;
            v->h = (unsigned char)min(254.0f, max(1.0f, (v->y - h->origin.y) * inv[1]));
            v->flags = (!(h->mask & (obj->stateFlags >> 2 & 3)) | ((obj->stateFlags & 0x20) == 0x20) << 1) << 1 | (obj->stateFlags & 0x10) == 0x10;
        }
        m = (D3SMMESH *)obj->info;
        n = min(m->ntris, d3_SortMax - (int)d3_SortCount);
        for (tri = m->tris; n > 0; n--, tri++) {
            D3VECTOR4 nrm;

            a = tri->v[0];
            b = tri->v[1];
            c = tri->v[2];
            d3_TriNormal(a, b, c, &nrm);
            if (fabs(nrm.y) < 0.05f && !(obj->stateFlags & 0x40)) {
                tri->solid = 1;
                tri->cast = 1;
            } else {
                tri->cast = (tri->f26 & 4) || b->gz * c->gx - b->gx * c->gz + b->gx * a->gz - b->gz * a->gx + c->gz * a->gx - c->gx * a->gz < 0.0f;
                tri->solid = 0;
            }
            if (tri->cast && !(tri->f26 & 2)) {
                if (a->gx >= 0.0f && a->gx < h->size && a->gz >= 0.0f && a->gz < h->size
                    && b->gx >= 0.0f && b->gx < h->size && b->gz >= 0.0f && b->gz < h->size
                    && c->gx >= 0.0f && c->gx < h->size && c->gz >= 0.0f && c->gz < h->size) {
                    d3_SortPtr->item = tri;
                    d3_SortCount++;
                    hmax = a->h;
                    if (hmax <= b->h)
                        hmax = b->h;
                    hmax = max(hmax, c->h);
                    d3_SortPtr->key = hmax * 0x300;
                    d3_SortPtr++;
                }
            }
        }
    }
    return 1;
}
/* 0x45e560, cells of type from enclosed by from/to on all four sides become to @deep behaviour: 8 code hunks from one scheduling difference, the whole asmdiff read (only one real difference: we hoist the parameter 'to' out of [esp+0x20] into esi before the first cell test, 'mov esi,[esp+0x20]' at 45e5af + 'cmp eax,esi', the original reads it lazily at 45e5c7 and compares 'cmp eax,[esp+0x20]' at 45e5be; every value, the check order e, e[-size], e[+size], e[-1], e[+1], and the e->type = to store are identical), plus 2 fill nops; the same operation in another register form, so a source variant is needed, not a meaning fix; readability already readable */

void d3sm_Replace(D3SM *h, int from, int to)
{
    D3SMENTRY *e;
    int x, z;

    for (z = 1; z < h->size - 1; z++) {
        for (x = 1, e = &h->entries[h->size * z + 1]; x < h->size - 1; x++, e++) {
            if (e->type == from
                && (e[-h->size].type == from || e[-h->size].type == to)
                && (e[h->size].type == from || e[h->size].type == to)
                && (e[-1].type == from || e[-1].type == to)
                && (e[1].type == from || e[1].type == to))
                e->type = to;
        }
    }
}
/* 0x45e620, (re)builds the map from the terrain and the objects of the default scene
   @stuck re-read the frame with dcp frame / framemap: the code matches apart from slots (both keep mx.z on
   the FPU and store only mx.x/y, both pack bot or the outer block counter into t's dead slot [ebp+0xc] but
   swapped: the original bot, ours the counter i; the original packs i with ax at [ebp-4], ours gives ax
   its own slot at -0x60). Array order from ebp, the original: mx/nrm -0x20 (shared), uv -0x38, j -0x3c,
   box -0x5c, mn -0x6c, rot -0x78, pos -0x88, corner -0x108; ours: uv -0x30, mn -0x3c, box -0x5c, ax -0x60,
   nrm/mx -0x70, pos -0x7c, rot -0x88, corner. Tried: top -= bot right after the block loop (= control: the
   early sub is not a source lever), D3VECTOR rot, pos order (= control), every subset of hoisting the
   t-block locals (corner, mn/mx, ax..z0) and the sort-loop locals (nrm, dx/dz/lim) to function scope (31
   variants: all = control except nrm hoisted: frame 0x114, worse). Next: the mn/mx reference counts (the
   original packs mx with nrm densest, mn below box: something references mx more or mn less than our
   min/max macros do) (tiebreak: insensitive) prior: whole asmdiff read and fixed: top/bot are int (signed
   jg/jl, fild dword), D3TERRAIN.size is int (fild dword), mn/mx D3VECTOR, base x0/y0/z0 locals copied into
   corner[0], min/max operand order (and 0x4100 vs test ah,5 senses), the edge-pick test is A < B (it was
   >=), the camera pos/rot copied field by field, entries init walks a pointer, d3s_ShadowTriangle flags is
   unsigned char (no xor before mov cl), nrm scoped in the sort loop (frame now 0x108 = original). Left:
   only stack-slot assignment and one scheduling place: the locals land in other slots (uv at ebp-0x30 vs
   -0x38, mn -0x3c vs -0x6c, pos/rot swapped -0x7c/-0x88, bot in [ebp-8] vs the dead t slot [ebp+0xc], the
   D3_ZERO temp frame+0x44 vs +0x58), and the original computes top - bot into [ebp-8] right after the
   block loop ('sub edx, dword ptr [ebp + 0xc]' at 0x45e7b9) where ours sinks it to the fild; ours=687
   original=685, no differs/ref lines */

void d3sm_Build(D3SM *h, D3TERRAIN *t)
{
    D3VECTOR pos, rot;
    D3VECTOR4 box[2];
    float f, lo, hi;
    int top, bot;
    int i, j, k, radius, last;
    D3SMTRI *tri;
    D3SMBLOCK *blk;
    D3SMENTRY *e;

    if (!h || h->size <= 0 || !h->data || !d3_Camera)
        return;
    d3_Busy++;
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    d3_TransformMeshes();
    box[0].x = 1e10f;
    box[0].y = 1e10f;
    box[0].z = 1e10f;
    box[1].x = -1e10f;
    box[1].y = -1e10f;
    box[1].z = -1e10f;
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, (D3OBJECTCB)d3sm_BoundsCB, box);
    if (h->entries) {
        D3_ZERO(h->entries, h->size * h->size * sizeof(D3SMENTRY));
        for (i = 0, e = h->entries; i < h->size * h->size; i++, e++)
            e->type = 1;
    }
    if (t) {
        D3VECTOR4 corner[8];
        D3VECTOR mn, mx;
        float ax, ay, az, x0, y0, z0;

        top = 0;
        bot = 0xff;
        blk = &t->blocks[0][0];
        for (i = 8; i; i--) {
            for (j = 8; j; j--, blk++) {
                top = max(top, blk->hi);
                bot = min(bot, blk->lo);
            }
        }
        x0 = bot * t->axisy.x + t->origin.x;
        y0 = bot * t->axisy.y + t->origin.y;
        z0 = bot * t->axisy.z + t->origin.z;
        corner[0].x = x0;
        corner[0].y = y0;
        corner[0].z = z0;
        f = (float)t->size;
        corner[1].x = (ax = f * t->axisx.x) + x0;
        corner[1].y = (ay = f * t->axisx.y) + y0;
        corner[1].z = (az = f * t->axisx.z) + z0;
        corner[2].x = f * t->axisz.x + x0;
        corner[2].y = f * t->axisz.y + y0;
        corner[2].z = f * t->axisz.z + z0;
        corner[3].x = corner[2].x + ax;
        corner[3].y = corner[2].y + ay;
        corner[3].z = corner[2].z + az;
        f = (float)(top - bot);
        for (i = 0; i < 4; i++) {
            corner[i + 4].x = f * t->axisy.x + corner[i].x;
            corner[i + 4].y = f * t->axisy.y + corner[i].y;
            corner[i + 4].z = f * t->axisy.z + corner[i].z;
        }
        mn.x = 1e10f;
        mn.y = 1e10f;
        mn.z = 1e10f;
        mx.x = -1e10f;
        mx.y = -1e10f;
        mx.z = -1e10f;
        for (i = 0; i < 8; i++) {
            mn.x = min(mn.x, corner[i].x);
            mn.y = min(mn.y, corner[i].y);
            mn.z = min(mn.z, corner[i].z);
            mx.x = max(mx.x, corner[i].x);
            mx.y = max(mx.y, corner[i].y);
            mx.z = max(mx.z, corner[i].z);
        }
        box[0].y = min(box[0].y, mn.y);
        box[1].y = max(box[1].y, mx.y);
        box[0].x = mn.x;
        box[1].x = mx.x;
        box[0].z = mn.z;
        box[1].z = mx.z;
    }
    h->origin.y = box[0].y + 1.0f;
    h->origin.x = box[0].x;
    h->origin.z = box[1].z;               /* z runs from the far edge: origin at the maximum, a negative cell (0x45ea01 / 0x45ea27) */
    h->cell.x = (box[1].x - box[0].x) / ((float)h->size - 1.75f);
    h->cell.y = (box[1].y - box[0].y) * (1.0f / 253.0f);
    h->cell.z = (box[0].z - box[1].z) / ((float)h->size - 1.75f);
    radius = (int)(50.0f / h->cell.x + 0.5f) / 4;
    if (radius < 1)
        radius = 1;
    else if (radius > 4)
        radius = 4;
    if (t)
        d3sm_Sample(t, h);
    else
        D3_ZERO(h->data, h->size * h->size);
    d3_SortCount = 0;
    d3_SortPtr = d3_SortBuf;
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, (D3OBJECTCB)d3sm_ShadowCB, h);
    d3_RadixSort(d3_SortCount, 0);
    for (k = 0; k < (int)d3_SortCount; k++) {
        float uv[3][2];
        unsigned char hs[3];
        D3VECTOR4 nrm;
        float dx, dz, lim;

        tri = d3_SortBuf[k].item;
        if (tri->solid) {
            lo = 1e10f;
            hi = -1e10f;
            for (i = 0; i < 3; i++) {
                f = tri->v[i]->y;
                uv[i][0] = tri->v[i]->gx;
                uv[i][1] = tri->v[i]->gz;
                hs[i] = tri->v[i]->h;
                if (f > hi) {
                    hi = f;
                    last = i;
                }
                if (f < lo) {
                    lo = f;
                    j = i;
                }
            }
            d3_TriNormal(tri->v[0], tri->v[1], tri->v[2], &nrm);
            if (fabs(tri->v[d3_TriEdge[j][1]]->y + tri->v[d3_TriEdge[j][0]]->y - (lo + lo))
                < fabs(tri->v[d3_TriEdge[last][1]]->y + tri->v[d3_TriEdge[last][0]]->y - (hi + hi)))
                j = last;
            dx = nrm.x * 0.875f;
            uv[j][0] -= dx;
            dz = nrm.z * 0.875f;
            uv[j][1] -= dz;
            uv[(j + 1) % 3][0] += dx;
            uv[(j + 1) % 3][1] += dz;
            uv[(j + 2) % 3][0] += dx;
            uv[(j + 2) % 3][1] += dz;
            for (i = 0; i < 3; i++) {
                lim = (float)(h->size - 1);
                uv[i][0] = max(0.0f, min(lim, uv[i][0]));
                uv[i][1] = max(0.0f, min(lim, uv[i][1]));
                hs[i] = tri->v[i]->h;
            }
        } else {
            for (i = 0; i < 3; i++) {
                lim = (float)(h->size - 1);
                uv[i][0] = max(0.0f, min(lim, tri->v[i]->gx));
                uv[i][1] = max(0.0f, min(lim, tri->v[i]->gz));
                hs[i] = tri->v[i]->h;
            }
        }
        d3s_ShadowTriangle(&uv[0][0], hs, h->data, h->size, tri->v[0]->flags, h->entries, radius);
    }
    d3_SortCount = 0;
    d3_SortPtr = d3_SortBuf;
    if (h->entries) {
        d3sm_Replace(h, 0xb, 0xc);
        for (i = 0; i < h->size; i++) {
            h->entries[i].type = 0xd;
            h->entries[(h->size - 1) * h->size + i].type = 0xd;
            h->entries[h->size * i].type = 0xd;
            h->entries[h->size * (i + 1) - 1].type = 0xd;
        }
    }
    d3_SetObjectPos(d3_Camera, &pos);
    d3_SetObjectRot(d3_Camera, &rot);
    d3_Busy--;
}
/* 0x45eeb0, size < 0: shadow map (no ground types) */

D3SM *d3sm_Create(D3TERRAIN *t, int size, int mask)
{
    D3SM *h, *ret = 0;

    if (size) {
        h = mem_Alloc(sizeof(D3SM), "d3_sm:Create");
        h->mask = mask;
        h->size = abs(size);
        h->data = mem_Alloc(h->size * h->size, "d3_sm:CreateHeight");
        if (size > 0)
            h->entries = mem_Alloc(h->size * h->size * sizeof(D3SMENTRY), "d3_sm:CreateEntries");
        else
            h->entries = 0;
        if (h->data)
            d3sm_Build(h, t);
        if (size < 0) {
            OutputDebugString("d3sm_Create(shadow)\n");
            return h;
        }
        OutputDebugString("d3sm_Create\n");
        ret = h;
    }
    return ret;
}
/* 0x45ef60, frees a height map with its entries and data; prints "d3sm_Destroy(shadow)" when it is the
   global d3sm_Shadow map */

void d3sm_Destroy(D3SM *h)
{
    if (h) {
        if (h == d3sm_Shadow)
            OutputDebugString("d3sm_Destroy(shadow)\n");
        else
            OutputDebugString("d3sm_Destroy\n");
        if (h->entries) {
            mem_Free(h->entries);
            h->entries = 0;
        }
        if (h->data) {
            mem_Free(h->data);
            h->data = 0;
        }
        mem_Free(h);
    }
}
/* 0x45efc0, the cell nearest to (x, z) (in rings of growing distance) with a real ground type */

char d3sm_FindGround(D3SM *h, int x, int z, int *ox, int *oz)
{
    int r = 0, xx, zz, x1, z1, w;

    if (h && h->entries && ox && oz && h->size > 0) {
        do {
            z1 = min(h->size - 1, z + r);
            for (zz = max(0, z - r); zz <= z1; zz++) {
                w = r - abs(z - zz);
                x1 = min(h->size - 1, x + w);
                for (xx = max(0, x - w); xx <= x1; xx++) {
                    if (zz >= 0 && zz < h->size && xx >= 0 && xx < h->size
                        && h->entries[zz * h->size + xx].type != 0 && h->entries[zz * h->size + xx].type != 0xd) {
                        *ox = xx;
                        *oz = zz;
                        return 1;
                    }
                }
            }
            r++;
        } while (r < h->size);
    }
    return 0;
}
/* 0x45f100, world position of map vertex (x, z) */
char d3sm_GetVertex(D3SM *h, int x, int z, D3VECTOR *out)
{
    if (h && x >= 0 && x < h->size && z >= 0 && z < h->size) {
        out->x = x * h->cell.x + h->origin.x;
        out->y = h->data[h->size * z + x] * h->cell.y + h->origin.y;
        out->z = z * h->cell.z + h->origin.z;
        return 1;
    }
    return 0;
}
/* 0x45f170, cell of pos and (h != 0) the interpolated height there @deep behaviour: 18 code + 6 order hunks from two scheduling differences only, the whole asmdiff read (28 lines): (1) our fdiv [esi+0x18] is emitted before and ebx,eax / imul ebx,edx, the original after them; (2) around the second corner height the original pops ebp and loads 'mov al,[edi+ecx]' earlier and takes the int temp from [esp+0x14] where we use [esp+0x18] - the same local, other slot, the same byte values and the same interpolation (y + dz*(top-y) + dx*(right-y)) in both; no statement missing, readability already readable; needs FPU scheduling / slot work */

char d3_GetHeightAt(D3SM *h, D3VECTOR *pos, int *px, int *pz, float *py)
{
    int x, z;
    float y;

    if (h) {
        x = (int)((pos->x - h->origin.x) / h->cell.x);
        z = (int)((pos->z - h->origin.z) / h->cell.z);
        if (x >= 0 && x < h->size && z >= 0 && z < h->size && px && pz) {
            *px = x;
            *pz = z;
            if (py) {
                y = (h->data[h->size * z + x] + 0.5f) * h->cell.y + h->origin.y;
                *py = ((h->data[(z + 1 & h->size - 1) * h->size + x] + 0.5f) * h->cell.y + h->origin.y - y)
                        * ((pos->z - (z * h->cell.z + h->origin.z)) / h->cell.z)
                    + ((h->data[h->size * z + (x + 1 & h->size - 1)] + 0.5f) * h->cell.y + h->origin.y - y)
                        * ((pos->x - (x * h->cell.x + h->origin.x)) / h->cell.x)
                    + y;
            }
            return 1;
        }
    }
    return 0;
}
/* 0x45f2b0, the map cell under the screen point (sx, sy): steps along the view ray */

char d3sm_Pick(D3SM *h, int sx, int sy, int *px, int *pz)
{
    D3MATRIX m;
    D3VECTOR4 s, p, d;              /* the screen point, the camera position and the ray, in cells */
    float step;
    int ix, iz;

    if (!h)
        return 0;
    p.x = d3_Camera->pos.x - h->origin.x;
    p.y = d3_Camera->pos.y - h->origin.y;
    p.z = d3_Camera->pos.z - h->origin.z;
    p.x /= h->cell.x;
    p.y /= h->cell.y;
    p.z /= h->cell.z;
    s.x = (sx - d3_CenterX) / d3_ScaleX;
    s.y = (sy - d3_CenterY) / d3_ScaleY;
    d3_MatTranspose(&d3_Camera->matrix, &m);
    d.x = m._21 * s.y + m._11 * s.x + m._31;
    d.y = m._22 * s.y + m._12 * s.x + m._32;
    d.z = m._23 * s.y + m._13 * s.x + m._33;
    d.x /= h->cell.x;
    d.y /= h->cell.y;
    d.z /= h->cell.z;
    if (fabs(d.x) < d3_SnapEpsilon && fabs(d.z) < d3_SnapEpsilon) {
        ix = (int)p.x;
        iz = (int)p.z;
        if (d.y > 0.0f && h->data[h->size * iz + ix] < p.y)
            return 0;
        *px = ix;
        *pz = iz;
        return 1;
    }
    step = 1.0f / (float)max(fabs(d.x), fabs(d.z));
    d.x *= step;
    d.y *= step;
    d.z *= step;
    while (p.x >= -h->size * 0.75f) {
        /* off the map and its margin: give up */
        if (!(p.x < h->size * 1.75f && p.z >= -h->size * 1.75f && p.z < h->size * 1.75f))
            return 0;
        if (p.x >= 0.0f && p.x < h->size && p.z >= 0.0f && p.z < h->size) {
            iz = (int)(p.z + 0.5f);
            ix = (int)(p.x + 0.5f);
            if (h->data[iz * h->size + ix] >= p.y) {
                *px = (int)(p.x + 0.5f);
                *pz = (int)(p.z + 0.5f);
                return 1;
            }
        }
        p.x += d.x;
        p.y += d.y;
        p.z += d.z;
    }
    return 0;
}
