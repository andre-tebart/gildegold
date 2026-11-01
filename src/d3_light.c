/* d3_light.c -- dynamic lighting of mesh objects ("d3_light:cache", "d3_light:rlight_object"), original file
   name unknown: 0x451e80..0x453920 (the end next to d3_vec.c is not proven). One object view (D3OBJECT +
   D3_L, d3_light.h). Names are ours.
   Open: the bits d3_SetVertexColors / d3_RelightObject read (D3PTRI 0x26, D3TEXTURE 0x68 / 0x6c) go through
   d3_light.h's cast views; the typed fields flip d3_CloneObject / d3_MirDrawObject.
   Open: d3.h keeps its own char d3_RelightObject declaration; dropping it (or including d3_light.h there)
   flips d3_MirDrawObject, which needs re-matching first.
   unit of rea 8cef7d5b (src/game/d3_light.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_light.h"
#include "d3_fl.h"
#include "d3_vec.h"
#include "mem.h"
#include "txt.h"
#include "unk_458810.h"
#include "unk_4591a0.h"
#define d3_DirToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_DirToRef)  /* 0x458950 (unk_458810.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */
#define d3_PointToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_PointToRef)  /* 0x458b20 (unk_458810.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */
#define d3_WorldMatrixD ((void (*)(D3OBJECT *, D3OBJECT *, int, void *))d3_WorldMatrix)  /* 0x458da0 (unk_458810.h: XOBJ *, XOBJ *, char, D3MATRIX *): this unit's D3OBJECT view of the object */

/* full rebuild: the symbols whose original addresses the code above spells literally */

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

#define D3L_RGB_MASK 0xffffffu   /* the r/g/b bits of D3COLOR (white = all three set) */

/* 0x451f10, callback: flickering light, adds its contribution to the vertices it touches */
void d3_LightFlicker(D3OBJECT *obj)
{
    unsigned long now = d3_Time;
    unsigned long t = now - D3_L(obj)->light->time;
    unsigned int iv = D3_L(obj)->light->interval;
    D3LREC *n;
    D3OBJECT *o;
    D3LVERT *v;
    D3LHIT *p;
    D3COLOR c;
    unsigned int i, cnt;
    int j, k;
    float s, f, m, r, g, b, a, q;

    if (t >= iv) {
        m = D3_L(obj)->color.y;
        for (j = 0; j < 7; j++)
            m = max(m, D3_L(obj)->light->keys[j].val);
        s = (D3_L(obj)->light->flicker * D3_L(obj)->color.y) / m;
        if (!D3_L(obj)->light->time)
            D3_L(obj)->flickerStart = (float)((float)rand() / RAND_MAX) * s * 2 - s + 1.0f;
        else
            D3_L(obj)->flickerStart = D3_L(obj)->flickerEnd;
        D3_L(obj)->flickerEnd = (float)((float)rand() / RAND_MAX) * s * 2 - s + 1.0f;
        D3_L(obj)->light->time = now;
        t = 0;
    }
    f = (float)t / (float)iv;
    a = (1.0f - f) * D3_L(obj)->flickerStart + f * D3_L(obj)->flickerEnd;
    for (n = D3_L(obj)->light->list; n; n = n->next) {
        o = n->obj;
        if (o && D3_L(o)->mesh && !(D3_L(o)->mesh->flags & 0x40) && D3_L(o)->info) {
            cnt = n->n;
            if (D3_L(o)->info->nkeys >= (int)cnt) {
                if (d3_Hardware) {
                    for (i = 0, p = n->v; i < cnt; i++, p++) {
                        v = p->v;
                        if ((v->color & D3L_RGB_MASK) != D3L_RGB_MASK) {
                            c = p->c;
                            r = c.r * a + v->lit.r;
                            g = c.g * a + v->lit.g;
                            b = c.b * a + v->lit.b;
                            m = max(max(r, g), b);
                            if (m <= 255.0f) {
                                d3_FtoI(&k, r);
                                v->lit.r = k;
                                d3_FtoI(&k, g);
                                v->lit.g = k;
                                d3_FtoI(&k, b);
                                v->lit.b = k;
                            } else {
                                q = 255.0f / m;
                                r *= q;
                                d3_FtoI(&k, r);
                                v->lit.r = k;
                                g *= q;
                                d3_FtoI(&k, g);
                                v->lit.g = k;
                                d3_FtoI(&k, b * q);
                                v->lit.b = k;
                            }
                        }
                    }
                } else {
                    a *= 0.25;
                    for (i = 0, p = n->v; i < cnt; i++, p++) {
                        v = p->v;
                        if (v->lit.r != 0x3f) {
                            c = p->c;
                            d3l_FtoI2(c.r * a, &k);
                            v->lit.r = (unsigned)k < 0x3f ? k : 0x3f;
                        }
                    }
                }
            }
        }
    }
}
/* 0x4522b0, adds the point lights and then the directional lights (type 7) of set to the vertex colours of obj */

char d3_LightApply(D3OBJECT *obj, D3LCTX *set)
{
    D3LINFO *info = D3_L(obj)->info;
    D3OBJECT *l;
    D3LVERT *v;
    D3LTRI *t;
    D3VECTOR4 d, c, nrm, acc;
    D3MATRIX m;
    D3VECTOR *p;
    unsigned int i, j;
    int k, iv;
    float a, s, r, r2, dist, f, sc;

    if (!info)
        return 1;
    {
        for (i = 0; i < set->count; i++) {
            l = set->list[i];
            if (l->type != 7) {
                s = D3_L(obj)->mesh->lightScale * D3_L(l)->power * 10.0f;
                a = D3_L(l)->atten;
                r = D3_L(l)->range;
                r2 = r * r;
                if (set->normals) {
                    for (k = 0, v = info->keys; k < info->nkeys; k++, v++) {
                        d.x = v->x - D3_L(l)->cam.x;
                        d.y = v->y - D3_L(l)->cam.y;
                        d.z = v->z - D3_L(l)->cam.z;
                        dist = d.x * d.x + d.y * d.y + d.z * d.z;
                        if (dist < r2) {
                            d3_Normalize(&d);
                            f = d.x * v->nx + d.y * v->ny + d.z * v->nz;
                            if (f < 0.0) {
                                d3l_FtoI2(f * -1023.0f, &iv);
                                f = ((float)(1.0f / (dist * a)) * s) * d3fl_Shade[iv];
                                c.x = f * D3_L(l)->color.x;
                                c.y = f * D3_L(l)->color.y;
                                c.z = f * D3_L(l)->color.z;
                                v->r = c.x + v->r;
                                v->g = c.y + v->g;
                                v->b = c.z + v->b;
                            }
                        }
                    }
                } else {
                    for (k = 0, v = info->keys; k < info->nkeys; k++, v++) {
                        d.x = v->x - D3_L(l)->cam.x;
                        d.y = v->y - D3_L(l)->cam.y;
                        d.z = v->z - D3_L(l)->cam.z;
                        dist = d.x * d.x + d.y * d.y + d.z * d.z;
                        if (dist < r2) {
                            f = (float)(1.0f / (dist * a)) * s;
                            c.x = f * D3_L(l)->color.x;
                            c.y = f * D3_L(l)->color.y;
                            c.z = f * D3_L(l)->color.z;
                            v->r = c.x + v->r;
                            v->g = c.y + v->g;
                            v->b = c.z + v->b;
                        }
                    }
                }
            }
        }
        for (i = 0; i < set->count; i++)
            if (set->list[i]->type == 7)
                break;
        if (i < set->count) {
            if (obj->kind == 1) {
                d3_WorldMatrixD(obj, (obj->state & 0x80) ? 0 : d3_Camera, 1, &m);
                sc = 20.0f / info->ops->radius;
                acc.y = info->ops->originY;
                for (j = i; j < set->count; j++) {
                    l = set->list[j];
                    if (l->type == 7) {
                        s = D3_L(obj)->mesh->lightScale * D3_L(l)->power * 0.01f;
                        nrm.x = D3_L(l)->light->dir.x;
                        nrm.y = D3_L(l)->light->dir.y;
                        nrm.z = D3_L(l)->light->dir.z;
                        for (k = 0, v = info->keys; k < info->nkeys; k++, v++) {
                            p = v->src;
                            d.x = p->x;
                            d.y = p->y;
                            d.z = p->z;
                            d.y = (d.y - acc.y) * sc;
                            c.x = m._11 * d.x + m._21 * d.y + m._31 * d.z;
                            c.y = m._12 * d.x + m._22 * d.y + m._32 * d.z;
                            c.z = m._13 * d.x + m._23 * d.y + m._33 * d.z;
                            d3_Normalize(&c);
                            f = nrm.x * c.x + nrm.y * c.y + nrm.z * c.z;
                            if (f < 0.0) {
                                d3l_FtoI2(f * -1023.0f, &iv);
                                f = s * d3fl_Shade[iv];
                                c.x = f * D3_L(l)->color.x;
                                c.y = f * D3_L(l)->color.y;
                                c.z = f * D3_L(l)->color.z;
                                v->r = c.x + v->r;
                                v->g = c.y + v->g;
                                v->b = c.z + v->b;
                            }
                        }
                    }
                }
            } else {
                for (k = 0, t = info->tris; k < info->ntris; k++, t++) {
                    d3_TriNormal(t->v[0], t->v[1], t->v[2], &nrm);
                    acc.x = 0.0f;
                    acc.y = 0.0f;
                    acc.z = 0.0f;
                    for (j = i; j < set->count; j++) {
                        l = set->list[j];
                        if (l->type == 7) {
                            s = D3_L(obj)->mesh->lightScale * D3_L(l)->power * 0.001f;
                            f = nrm.x * D3_L(l)->light->dir.x + nrm.y * D3_L(l)->light->dir.y + nrm.z * D3_L(l)->light->dir.z;
                            if (f < 0.0f) {
                                d3l_FtoI2(f * -1023.0f, &iv);
                                f = s * d3fl_Shade[iv];
                                c.x = f * D3_L(l)->color.x;
                                c.y = f * D3_L(l)->color.y;
                                c.z = f * D3_L(l)->color.z;
                                acc.x = acc.x + c.x;
                                acc.y = acc.y + c.y;
                                acc.z = acc.z + c.z;
                            }
                        }
                    }
                    for (j = 0; j < 3; j++) {
                        t->v[j]->r = acc.x + t->v[j]->r;
                        t->v[j]->g = acc.y + t->v[j]->g;
                        t->v[j]->b = acc.z + t->v[j]->b;
                    }
                }
            }
        }
    }
    return 1;
}
/* 0x452940, adds the light ctx->light to the vertices of obj it reaches and records them in the light's list */

char d3_LightAddObject(D3OBJECT *obj, D3LCTX2 *ctx)
{
    D3LINFO *info = D3_L(obj)->info;
    D3OBJECT *light;
    D3LVERT *v;
    D3LREC *rec, *p;
    D3LHIT *hit;
    D3VECTOR4 d, c;
    float dd, rr, z, k, t, m, f, s;
    int n, i, idx;

    if (info) {
        d3_PointToRefD(obj, d3_Camera, &obj->pos, (D3VECTOR4 *)&D3_L(obj)->cam);
        D3_L(obj)->radius2 = (d3_LightRadiusMargin + info->ops->radius) * (d3_LightRadiusMargin + info->ops->radius);
        n = 0;
        light = ctx->light;
        d.x = D3_L(obj)->cam.x - D3_L(light)->cam.x;
        d.y = D3_L(obj)->cam.y - D3_L(light)->cam.y;
        d.z = D3_L(obj)->cam.z - D3_L(light)->cam.z;
        if (sqrt(d.x * d.x + d.y * d.y + d.z * d.z) < (D3_L(light)->radius2) + D3_L(obj)->radius2) {
            rr = D3_L(light)->range * D3_L(light)->range;
            k = D3_L(obj)->mesh->lightScale * D3_L(light)->power;
            z = D3_L(light)->atten;
            if (ctx->normals) {
                v = info->keys;
                for (i = 0; i < info->nkeys; i++, v++) {
                    v->flags &= 0x7f;
                    d.x = v->x - D3_L(light)->cam.x;
                    d.y = v->y - D3_L(light)->cam.y;
                    d.z = v->z - D3_L(light)->cam.z;
                    s = d.x * d.x + d.y * d.y + d.z * d.z;   /* s, not dd: the original reuses s (scale below) */
                    if (s < rr) {
                        d3_Normalize((D3VECTOR4 *)&d);
                        f = d.x * v->nx + d.y * v->ny + d.z * v->nz;
                        if (f < 0.0) {
                            f = f * -1023.0f;
                            v->flags |= 0x80;
                            d3_FtoI(&idx, f);
                            t = z;   /* through t: VC6 then loads atten before s, as the original */
                            t = (float)((float)(1.0f / (t * s)) * k * 10.0f) * d3fl_Shade[idx];
                            n++;
                            v->light.x = t * D3_L(light)->color.x;
                            v->light.y = t * D3_L(light)->color.y;
                            v->light.z = t * D3_L(light)->color.z;
                        }
                    }
                }
            } else {
                v = info->keys;
                for (i = 0; i < info->nkeys; i++, v++) {
                    v->flags &= 0x7f;
                    d.x = v->x - D3_L(light)->cam.x;
                    d.y = v->y - D3_L(light)->cam.y;
                    d.z = v->z - D3_L(light)->cam.z;
                    dd = d.x * d.x + d.y * d.y + d.z * d.z;
                    if (dd < rr) {
                        v->flags |= 0x80;
                        n++;
                        t = (float)(1.0f / (z * dd)) * k * 10.0f;
                        v->light.x = t * D3_L(light)->color.x;
                        v->light.y = t * D3_L(light)->color.y;
                        v->light.z = t * D3_L(light)->color.z;
                    }
                }
            }
            if (n) {
                if (D3_L(light)->light->list) {
                    rec = D3_L(light)->light->list;
                    while (rec->next)
                        rec = rec->next;
                    p = rec->next = (D3LREC *)mem_Alloc(n * 8 + 0xc, "d3_light:rlight_object");
                } else {
                    D3_L(light)->light->list = (D3LREC *)mem_Alloc(n * 8 + 0xc, "d3_light:rlight_object");
                    p = D3_L(light)->light->list;
                }
                p->obj = obj;
                p->n = n;
                v = info->keys;
                hit = p->v;
                for (i = 0; i < info->nkeys; i++, v++) {
                    if (v->flags & 0x80) {
                        hit->v = v;
                        c.x = v->light.x;
                        c.y = v->light.y;
                        c.z = v->light.z;
                        m = c.x;
                        if (c.y > m)
                            m = c.y;
                        if (c.z > m)
                            m = c.z;
                        if (m > 255.0f) {
                            s = 255.0f / m;
                            c.x *= s;
                            c.y *= s;
                            c.z *= s;
                        }
                        if (d3_Hardware) {
                            hit->c.r = (int)c.x;
                            hit->c.g = (int)c.y;
                            hit->c.b = (int)c.z;
                        } else {
                            hit->c.r = (int)(c.x * 0.3f + c.y * 0.59f + c.z * 0.11f);
                        }
                        hit++;
                    }
                }
            }
        }
    }
    return 1;
}
/* 0x452d90, d3_ForEachObject callback over the lights: obj gets this light */

char d3_LightAttach(D3OBJECT *light, D3OBJECT *obj)
{
    D3LCTX2 ctx;

    ctx.light = light;
    ctx.normals = (obj->state >> 5) & 1;
    d3_LightRemoveRecords(light, obj);
    if (D3_L(light)->power != 0.0) {
        d3_PointToRefD(light, d3_Camera, &light->pos, (D3VECTOR4 *)&D3_L(light)->cam);
        D3_L(light)->radius2 = D3_L(light)->range * D3_L(light)->range;
        d3_LightAddObject(obj, &ctx);
    }
    return 1;
}
/* 0x452e10, the per-frame light step: resets the vertex colours of the lit objects and then adds the
   flickering contribution (both over the default scene's light objects) */

void d3_LightUpdate(void)
{
    d3_ForEachObject2(d3_DefaultScene, 0, 4, d3_LightResetVertexColors);
    d3_ForEachObject2(d3_DefaultScene, 0, 4, d3_LightFlicker);
}
/* 0x452e40, forgets the light records of obj (all, or the one of the object src) */

char d3_LightRemoveRecords(D3OBJECT *obj, void *src)
{
    D3LREC *p, *n;

    if (obj && D3_L(obj)->light) {
        if (!src) {
            for (p = D3_L(obj)->light->list; p; p = n) {
                n = p->next;
                mem_Free(p);
            }
            D3_L(obj)->light->list = 0;
            return 1;
        }
        p = D3_L(obj)->light->list;
        if (p) {
            if (p->obj == src) {
                D3_L(obj)->light->list = p->next;
                mem_Free(p);
                return 1;
            }
            while ((n = p->next, n == 0 || n->obj != src)) {
                p = n;
                if (!n)
                    return 1;
            }
            p->next = n->next;
            mem_Free(n);
        }
    }
    return 1;
}
/* 0x452ef0, camera space position and squared radius of obj */

void d3_LightPrepare(D3OBJECT *obj)
{
    float r = 0.0f;
    if (!obj->info) {
        obj->info = d3_SelectSurf(obj);
        if (!obj->info)
            return;
    }
    d3_PointToRefD(obj, d3_Camera, &obj->pos, (D3VECTOR4 *)&D3_L(obj)->cam);
    if (D3_L(obj)->info->ops)
        r = D3_L(obj)->info->ops->Radius(obj, obj->info);
    D3_L(obj)->radius2 = r * r;
    if (obj->state & 4)
        d3_TransformSurf(obj);
    if (obj->state & 0x20)
        if (D3_L(obj)->info->ops)
            D3_L(obj)->info->ops->fn1ec(obj, obj->info);
}
/* 0x453580, recomputes the vertex colours (d3_RelightObject) of the objects of obj's scene that d3_ForEachObject2 selects
   from obj with mask 0x240; nothing for a null obj */

void d3_RelightObjects(D3OBJECT *obj)
{
    if (obj)
        d3_ForEachObject2(obj->scene, obj, 0x240, (void (*)(D3OBJECT *))d3_RelightObject);
}
/* 0x453110, d3_ForEachObject callback over the lights: pass 1 (list == 0) counts the ones that reach
   ctx->obj, pass 2 collects them */

char d3_LightCollectCB(D3OBJECT *obj, D3LCTX *ctx)
{
    if (!ctx->list) {
        D3_L(obj)->light->on = 0;
        d3_PointToRefD(obj, d3_Camera, &obj->pos, (D3VECTOR4 *)&D3_L(obj)->cam);
        if (obj->type == 7) {
            if (D3_L(obj)->power != 0.0 && !(obj->objectFlags & 0x10)) {
                d3_DirToRefD(obj, d3_Camera, &d3_ZAxis, (D3VECTOR4 *)&D3_L(obj)->light->dir);
                ctx->count++;
                D3_L(obj)->light->on = 1;
            }
        } else {
            float r = D3_L(obj)->radius2 = D3_L(obj)->range;
            float dx = D3_L(obj)->cam.x - D3_L(ctx->obj)->cam.x;
            float dy = D3_L(obj)->cam.y - D3_L(ctx->obj)->cam.y;
            float dz = D3_L(obj)->cam.z - D3_L(ctx->obj)->cam.z;
            float s;
            if (D3_L(obj)->power != 0.0 && (s = (float)sqrt(dx * dx + dy * dy + dz * dz), s < r + D3_L(ctx->obj)->radius2)) {
                ctx->count++;
                D3_L(obj)->light->on = 1;
            }
        }
    } else if (D3_L(obj)->light->on)
        ctx->list[ctx->count++] = obj;
    return 1;
}
/* 0x452fa0, sets the colours (c and s) of the vertices of the triangles of surface s from the texture's
   flags: the grey of the accumulated light scaled by the texture's two flag bytes, or the alpha byte alone.
   The callers pass on and fl, the body reads neither. */

void d3_SetVertexColors(D3OBJECT *obj, D3SURF *s, char on, D3TEXFLAGS fl)
{
    int i, k, g;
    D3PTRI *tri;
    D3TEXTURE *tex;
    D3TEXFLAGS f;
    D3PVERT *v;
    D3COLOR c;
    float x, b0, b1;
    unsigned char fl212 = obj->stateFlags;
    char keep = !(fl212 & 1);
    char pos = !(obj->objectFlags & 0x80);

    if (!(fl212 & 2)) {
        tri = (D3PTRI *)s->info.tris;
        for (i = 0; i < s->info.ntris; i++, tri++) {
            tex = tri->tex;
            if (tex) {
                if (keep)
                    ((D3BITS *)&tri->drawFlags)->b2 = ((D3BITS *)&tex->flags)->b4;
                if (pos)
                    f = ((D3TEXVIEW *)tex)->flags;
                else
                    f = s->flags.f;
                if (f.b[2] & 1) {
                    k = 0;
                    b1 = f.b[1];
                    b0 = f.b[0];
                    for (; k < 3; k++) {
                        v = tri->v[k];
                        x = min(255.0f, (float)((((D3LVERT *)v)->light.x * 0.3f + ((D3LVERT *)v)->light.y * 0.59f + ((D3LVERT *)v)->light.z * 0.11f + b1) * 0.00390625f) * b0);
                        d3_FtoI(&g, x);
                        c.r = c.g = c.b = c.a = (unsigned char)g;
                        v->c = c;
                        v->s = c;
                    }
                } else if (f.b[0] < 255) {
                    for (k = 0; k < 3; k++) {
                        v = tri->v[k];
                        v->c.a = f.b[0];
                        v->s.a = f.b[0];
                    }
                }
            }
        }
    }
}
/* 0x453280, recomputes the vertex colours of obj from the lights that reach it */

char d3_RelightObject(D3OBJECT *obj)
{
    D3LINFO *info;
    D3LCTX ctx;
    D3TEXFLAGS f;
    D3VECTOR4 c;
    int i, g;
    float m;
    D3LVERT *k;

    if (!obj->info)
        obj->info = d3_SelectSurf(obj);
    info = D3_L(obj)->info;
    if (info && !(obj->stateFlags & 2)) {
        ((D3TEXPACK *)&f)->i = 0;
        f.b[0] = 0xff;
        d3_Busy++;
        d3_LightPrepare(obj);
        d3_LightRadiusMargin = 0.0f;
        k = info->keys;
        for (i = 0; i < info->nkeys; i++) {
            k[i].light.x = d3_AmbientColor.x;
            k[i].light.y = d3_AmbientColor.y;
            k[i].light.z = d3_AmbientColor.z;
        }
        ctx.count = 0;
        ctx.list = 0;
        ctx.obj = obj;
        ctx.normals = (obj->state >> 5) & 1;
        d3_ForEachObject(obj->scene, 0, 10, (D3OBJECTCB)d3_LightCollectCB, &ctx);
        if (ctx.count > 0) {
            ctx.list = (D3OBJECT **)mem_Alloc(ctx.count << 2, "d3_light:cache");
            ctx.count = 0;
            d3_ForEachObject(obj->scene, 0, 10, (D3OBJECTCB)d3_LightCollectCB, &ctx);
            d3_LightApply(obj, &ctx);
            mem_Free(ctx.list);
            ctx.list = 0;
        }
        if (d3_Hardware) {
            k = info->keys;
            for (i = 0; i < info->nkeys; i++) {
                c.x = k[i].light.x;
                c.y = k[i].light.y;
                c.z = k[i].light.z;
                m = c.x;
                if (!(m > c.y))
                    m = c.y;
                if (!(m > c.z))
                    m = c.z;
                if (m > 255.0f) {
                    float q = 255.0f / m;
                    c.x *= q;
                    c.y *= q;
                    c.z *= q;
                }
                d3_FtoI(&g, c.x);
                k[i].base.r = g;
                d3_FtoI(&g, c.y);
                k[i].base.g = g;
                d3_FtoI(&g, c.z);
                k[i].base.b = g;
                k[i].color = k[i].specular;
            }
        } else {
            k = info->keys;
            for (i = 0; i < info->nkeys; i++) {
                c.x = k[i].light.x;
                c.y = k[i].light.y;
                c.z = k[i].light.z;
                d3_FtoI(&g, c.x * 0.3f + c.y * 0.59f + c.z * 0.11f);
                k[i].base.r = min((unsigned int)g, 0xff);
                k[i].color = k[i].specular;
            }
        }
        d3_SetVertexColors(obj, (D3SURF *)info, 0, f);
        d3_ForEachObject(obj->scene, 0, 4, (D3OBJECTCB)d3_LightAttach, obj);
        d3_LightRadiusMargin = 0.0f;
        obj->state |= 4;
        D3_L(obj)->time = d3_Time;
        D3_L(obj)->mesh->lightGeneration = d3_LightGeneration;
        d3_Busy--;
        return 1;
    }
    return 0;
}
/* 0x4535b0, like d3_RelightObject for an object whose vertices and normals were just posed by the animation
   frame: lights of group mask 0xe, the frame's radius as the reach margin */

void d3_RelightObjectFrame(D3OBJECT *obj, struct D3ANIMFRAME *frame)
{
    D3LINFO *info;
    D3LCTX ctx;
    D3VECTOR4 c;
    int i, g;
    float s;
    D3LVERT *k;

    if (!(obj->stateFlags & 2)) {
        if (!obj->info) {
            obj->info = d3_SelectSurf(obj);
            if (!obj->info)
                return;
        }
        info = D3_L(obj)->info;
        d3_Busy++;
        d3_PointToRefD(obj, d3_Camera, &obj->pos, (D3VECTOR4 *)&D3_L(obj)->cam);
        if (frame)
            d3_LightRadiusMargin = ((D3LFRAME *)frame)->radius;
        else
            d3_LightRadiusMargin = 0.0f;
        s = d3_LightRadiusMargin + info->ops->radius;
        D3_L(obj)->radius2 = s * s;
        d3_FrameNormals(obj, (D3OBJINFO *)info, frame);
        k = info->keys;
        for (i = 0; i < info->nkeys; i++) {
            k[i].light.x = d3_AmbientColor.x;
            k[i].light.y = d3_AmbientColor.y;
            k[i].light.z = d3_AmbientColor.z;
        }
        ctx.count = 0;
        ctx.list = 0;
        ctx.obj = obj;
        ctx.normals = (obj->state >> 5) & 1;
        d3_ForEachObject(obj->scene, 0, 0xe, (D3OBJECTCB)d3_LightCollectCB, &ctx);
        if (ctx.count > 0) {
            ctx.list = (D3OBJECT **)mem_Alloc(ctx.count << 2, "d3_light:cache");
            ctx.count = 0;
            d3_ForEachObject(obj->scene, 0, 0xe, (D3OBJECTCB)d3_LightCollectCB, &ctx);
            d3_LightApply(obj, &ctx);
            mem_Free(ctx.list);
            ctx.list = 0;
        }
        if (d3_Hardware) {
            k = info->keys;
            for (i = 0; i < info->nkeys; i++) {
                c.x = k[i].light.x;
                c.y = k[i].light.y;
                c.z = k[i].light.z;
                s = c.x;
                if (!(s > c.y))
                    s = c.y;
                if (!(s > c.z))
                    s = c.z;
                if (s > 255.0) {
                    float q = 255.0f / s;
                    c.x *= q;
                    c.y *= q;
                    c.z *= q;
                }
                d3_FtoI(&g, c.x);
                k[i].base.r = g;
                d3_FtoI(&g, c.y);
                k[i].base.g = g;
                d3_FtoI(&g, c.z);
                k[i].base.b = g;
            }
        } else {
            k = info->keys;
            for (i = 0; i < info->nkeys; i++) {
                c.x = k[i].light.x;
                c.y = k[i].light.y;
                c.z = k[i].light.z;
                d3_FtoI(&g, c.x * 0.3f + c.y * 0.59f + c.z * 0.11f);
                k[i].base.r = min((unsigned int)g, 0xff);
            }
        }
        D3_L(obj)->time = d3_Time;
        D3_L(obj)->mesh->lightGeneration = d3_LightGeneration;
        d3_Busy--;
    }
}
/* 0x4538a0, last function of the TU: invalidates the cached scene lighting (light generation ++, all light
   records dropped, the lights-changed flag set) and rebuilds the terrain light map. on > 1 relights every
   object of the default scene at once; on 0 it does nothing unless 0x633438 is set.
   @deep Q2: 0x633438 has no evidenced name. The whole exe references it four times (4 raw hits): d3_Init only sets it 0; it gates the three orthographic view cameras Oben/Vorne/Seite in d3if_CreateCameras (0x448910) and makes type-6 spawns type 5 in d3_SpawnObject (0x43d..., case 'r'); here it skips the relight when on == 0. No code writes it to 1, so what the condition stands for (editor/view mode?) is unproven. Q1 fixed (d3_LightInvalidate), the other identifiers named (633738 -> d3_Terrain, 633b14 -> d3_LightsChanged). */

void d3_LightInvalidate(unsigned char on)
{
    d3_Busy++;
    d3_LightGeneration++;
    if (on == 0) {
        if (d3_Unk633438 == 0)
            goto done;
    } else if (on > 1) {
        d3_ForEachObject2(d3_DefaultScene, 0, 0x40, (void (*)(D3OBJECT *))d3_RelightObject);
        goto done;
    }
    d3_ForEachObject(d3_DefaultScene, 0, 2, (D3OBJECTCB)d3_LightRemoveRecords, 0);
    d3_LightsChanged = 1;
done:
    if (d3_Terrain)
        d3fl_MakeLight(d3_Terrain);
    d3_Busy--;
}

float d3_LightRadiusMargin = 0;  /* 0x633b18, added to the mesh radius for the light reach:
                                               the matrix z offset (m->_43) or 0 */
