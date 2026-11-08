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

void d3s_ShadowTriangle(float *p, unsigned char *c, unsigned char *dst, int pitch, unsigned int flags, char *base, int radius);                                       /* 0x45a2e0 */
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
/* 0x45dc00, samples the heights and ground types of the terrain into the map @deep behaviour: 544 code hunks, but all of them are the same operations with other register / stack-slot names or in another order (a full read of the asmdiff: 170 ours vs 181 original instructions, no statement missing); the instruction multiset differs only by 2 add / 2 xor / 4 mov / 2 lea, the same x*step+step masks and xor-zeroing strength-reduced differently, plus 5 fill nops; the C's loops (heights by __ftol, count[15] ground-type vote, step-halving midpoint fill) are the same as the Ghidra pseudocode, readability already readable; restoring the original's allocation is byte work */

void d3sm_Sample(D3TERRAIN *t, D3SM *h)
{
    unsigned int count[15];
    unsigned int n, ts, step, half, f, x, z, xx, zz, k, best, max, sum;
    unsigned int a, b, c, d;
    int avg;
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
    for (; step > 1; step = half) {
        half = step >> 1;
        for (z = 0; z < n / step; z++) {
            for (x = 0; x < n / step; x++) {
                a = h->data[z * step * n + x * step];
                b = h->data[z * step * n + (x * step + step & n - 1)];
                c = h->data[(z * step + step & n - 1) * n + x * step];
                d = h->data[(z * step + step & n - 1) * n + (x * step + step & n - 1)];
                avg = (int)(a + b + c + d) / 4;
                h->data[z * step * n + x * step + half] = (avg + (a + b) * 2) / 5;
                h->data[(z * step + half) * n + x * step] = (avg + (a + c) * 2) / 5;
                h->data[(z * step + half) * n + x * step + half] = avg;
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
   @deep behaviour: 193 code hunks, the whole 213-line asmdiff read, no statement missing - the same operations in other registers / stack slots / idioms: frame sub esp 0x24 vs 0x18 and 3 vs 4 callee-saved regs (the object and the mesh pointer kept in different regs), the tri->cast bitfield update is the same shl/xor bit-7 idiom with other registers (and bit 6 already cleared by solid = 0), max(a->h,b->h) as byte cmp+jbe+and 0xff vs zero-extended dword cmp+jg, the 0xc0 solid/cast pair as load/or/store vs or byte [mem],0xc0; no meaning error found */

char d3sm_ShadowCB(D3OBJECT *obj, D3SM *h)
{
    D3SMMESH *m = (D3SMMESH *)obj->info;
    D3SMTRI *tri;
    D3SMVERT *v, *a, *b, *c;
    D3VECTOR4 nrm;
    float inv[3];
    float size;
    int i, n, hmax;

    if (!m)
        return 1;
    if ((h->mask & (obj->stateFlags >> 2 & 3)) && !(obj->stateFlags & 0x10))
        return 1;
    for (i = 0; i < 3; i++)
        inv[i] = 1.0f / (&h->cell.x)[i];
    for (i = 0, v = m->keys; i < m->nkeys; i++, v++) {
        v->gx = (v->x - h->origin.x) * inv[0] + 0.875f;
        v->gz = (v->z - h->origin.z) * inv[2] + 0.875f;
        v->h = (unsigned char)min(254.0f, max(1.0f, (v->y - h->origin.y) * inv[1]));
        v->flags = (!(h->mask & (obj->stateFlags >> 2 & 3)) | ((obj->stateFlags & 0x20) == 0x20) << 1) << 1 | (obj->stateFlags & 0x10) == 0x10;
    }
    m = (D3SMMESH *)obj->info;
    n = min(m->ntris, d3_SortMax - d3_SortCount);
    for (tri = m->tris; n > 0; n--, tri++) {
        a = tri->v[0];
        b = tri->v[1];
        c = tri->v[2];
        d3_TriNormal(a, b, c, &nrm);
        if (fabs(nrm.y) < 0.05f && !(obj->stateFlags & 0x40)) {
            tri->solid = 1;
            tri->cast = 1;
        } else {
            tri->solid = 0;
            tri->cast = (tri->f26 & 4) || b->gz * c->gx - b->gx * c->gz + b->gx * a->gz - b->gz * a->gx + c->gz * a->gx - c->gx * a->gz < 0.0f;
        }
        if (tri->cast && !(tri->f26 & 2)) {
            size = (float)h->size;
            if (a->gx >= 0.0f && a->gx < size && a->gz >= 0.0f && a->gz < size
                && b->gx >= 0.0f && b->gx < size && b->gz >= 0.0f && b->gz < size
                && c->gx >= 0.0f && c->gx < size && c->gz >= 0.0f && c->gz < size) {
                d3_SortPtr->item = tri;
                d3_SortCount++;
                hmax = max(a->h, b->h);
                hmax = max(hmax, c->h);
                d3_SortPtr->key = hmax * 0x300;
                d3_SortPtr++;
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
/* 0x45e620, (re)builds the map from the terrain and the objects of the default scene */

void d3sm_Build(D3SM *h, D3TERRAIN *t)
{
    D3VECTOR4 corner[8];
    D3VECTOR pos, rot;
    D3VECTOR4 box[2];
    D3VECTOR4 mn, mx;
    D3VECTOR4 nrm;
    float uv[3][2];
    unsigned char hs[3];
    float ax, ay, az, f, lo, hi, x0, y0, z0;
    unsigned int k, top, bot;
    int i, j, radius, last;
    D3SMTRI *tri;
    D3SMBLOCK *blk;

    if (!h || h->size <= 0 || !h->data || !d3_Camera)
        return;
    d3_Busy++;
    pos = *(D3VECTOR *)&d3_Camera->pos;
    rot = *(D3VECTOR *)&d3_Camera->rotation;
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
        for (i = 0; i < h->size * h->size; i++)
            h->entries[i].type = 1;
    }
    if (t) {
        top = 0;
        bot = 0xff;
        blk = &t->blocks[0][0];
        for (i = 8; i; i--) {
            for (j = 8; j; j--, blk++) {
                top = max(top, blk->hi);
                bot = min(bot, blk->lo);
            }
        }
        corner[0].x = bot * t->axisy.x + t->origin.x;
        corner[0].y = bot * t->axisy.y + t->origin.y;
        corner[0].z = bot * t->axisy.z + t->origin.z;
        f = (float)t->size;
        corner[1].x = (ax = f * t->axisx.x) + corner[0].x;
        corner[1].y = (ay = f * t->axisx.y) + corner[0].y;
        corner[1].z = (az = f * t->axisx.z) + corner[0].z;
        corner[2].x = f * t->axisz.x + corner[0].x;
        corner[2].y = f * t->axisz.y + corner[0].y;
        corner[2].z = f * t->axisz.z + corner[0].z;
        corner[3].x = corner[2].x + ax;
        corner[3].y = corner[2].y + ay;
        corner[3].z = corner[2].z + az;
        f = (float)(int)(top - bot);
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
            mn.x = min(corner[i].x, mn.x);
            mn.y = min(corner[i].y, mn.y);
            mn.z = min(corner[i].z, mn.z);
            mx.x = max(corner[i].x, mx.x);
            mx.y = max(corner[i].y, mx.y);
            mx.z = max(corner[i].z, mx.z);
        }
        box[0].y = min(mn.y, box[0].y);
        box[1].y = max(mx.y, box[1].y);
        box[0].x = mn.x;
        box[1].x = mx.x;
        box[0].z = mn.z;
        box[1].z = mx.z;
    }
    h->origin.y = box[0].y + 1.0f;
    h->origin.x = box[0].x;
    h->origin.z = box[1].z;               /* z runs from the far edge: origin at the maximum, a negative cell (0x45ea01 / 0x45ea27) */
    f = (float)h->size - 1.75f;
    h->cell.x = (box[1].x - box[0].x) / f;
    h->cell.y = (box[1].y - box[0].y) * (1.0f / 253.0f);
    h->cell.z = (box[0].z - box[1].z) / f;
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
    for (k = 0; k < (unsigned int)d3_SortCount; k++) {
        tri = d3_SortBuf[k].item;
        if (tri->solid) {
            hi = -1e10f;
            lo = 1e10f;
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
                >= fabs(tri->v[d3_TriEdge[last][1]]->y + tri->v[d3_TriEdge[last][0]]->y - (hi + hi)))
                j = last;
            x0 = nrm.x * 0.875f;
            uv[j][0] -= x0;
            z0 = nrm.z * 0.875f;
            uv[j][1] -= z0;
            uv[(j + 1) % 3][0] += x0;
            uv[(j + 1) % 3][1] += z0;
            uv[(j + 2) % 3][0] += x0;
            uv[(j + 2) % 3][1] += z0;
            for (i = 0; i < 3; i++) {
                y0 = (float)(h->size - 1);
                uv[i][0] = max(0.0f, min(y0, uv[i][0]));
                uv[i][1] = max(0.0f, min(y0, uv[i][1]));
                hs[i] = tri->v[i]->h;
            }
        } else {
            for (i = 0; i < 3; i++) {
                y0 = (float)(h->size - 1);
                uv[i][0] = max(0.0f, min(y0, tri->v[i]->gx));
                uv[i][1] = max(0.0f, min(y0, tri->v[i]->gz));
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
/* 0x45f2b0, the map cell under the screen point (sx, sy): steps along the view ray @deep behaviour: the two meaning errors found are fixed (loop cell read now rounds like the original: iz/ix = (int)(z+0.5f)/(int)(x+0.5f) with the 0.5f constant at 0x5eb7ac, index iz*h->size+ix instead of the transposed ix*h->size+iz, and *px/*pz use the same rounded values as 45f5cc-45f5ff; asmdiff 239 -> 219 lines); what remains is codegen: frame sub esp 0x80 vs 0x68 and the whole local slot layout, fld x; fcomp limit; test ah,0x5; jp in the bound checks vs our fld limit; fmul 1.75; fcomp x; and eax,0x100; je (operand order), step*dx/dy/dz hoisted once in the original (fst [esp+0xc]; fmul) vs our per-iteration fmul, fdivr 1.0 vs fld 1.0/fdiv, and x87 stack order; readability already readable */

char d3sm_Pick(D3SM *h, int sx, int sy, int *px, int *pz)
{
    D3MATRIX m;
    float x, y, z, dx, dy, dz, fx, fy, step, lim;
    int ix, iz;

    if (!h)
        return 0;
    x = (d3_Camera->pos.x - h->origin.x) / h->cell.x;
    y = (d3_Camera->pos.y - h->origin.y) / h->cell.y;
    z = (d3_Camera->pos.z - h->origin.z) / h->cell.z;
    fx = (sx - d3_CenterX) / d3_ScaleX;
    fy = (sy - d3_CenterY) / d3_ScaleY;
    d3_MatTranspose(&d3_Camera->matrix, &m);
    dx = (m._11 * fx + m._21 * fy + m._31) / h->cell.x;
    dy = (m._12 * fx + m._22 * fy + m._32) / h->cell.y;
    dz = (m._13 * fx + m._23 * fy + m._33) / h->cell.z;
    if (fabs(dx) < d3_SnapEpsilon && fabs(dz) < d3_SnapEpsilon) {
        ix = (int)x;
        iz = (int)z;
        if (dy > 0.0f && h->data[h->size * iz + ix] < y)
            return 0;
        *px = ix;
        *pz = iz;
        return 1;
    }
    step = 1.0f / max(fabs(dx), fabs(dz));
    lim = -h->size * 0.75f;
    while (x >= lim) {
        if (x >= h->size * 1.75f || z <= -h->size * 1.75f || z >= h->size * 1.75f)
            return 0;
        if (x >= 0.0f && x < h->size && z >= 0.0f && z < h->size) {
            iz = (int)(z + 0.5f);
            ix = (int)(x + 0.5f);
            if (h->data[iz * h->size + ix] >= y) {
                *px = (int)(x + 0.5f);
                *pz = (int)(z + 0.5f);
                return 1;
            }
        }
        x += step * dx;
        y += step * dy;
        z += step * dz;
    }
    return 0;
}
