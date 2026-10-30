/* d3_par.c -- particle systems ("d3_par:" allocation tags, 0x455630..0x457c30), original file name
   unknown: the list handling, the particle draw function, the three emitter callbacks and
   par_CreateEmitter. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_par.c); boundaries in decl/text.dcl, notes in notes/units/d3_par.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_vec.h"
/* d3_engine.h declares the list head / tail pointers as void *; d3_par.h has the real types */
#define d3_ParHead d3_ParHead_engine
#define d3_ParTail d3_ParTail_engine
#include "d3_engine.h"
#undef d3_ParHead
#undef d3_ParTail
#include "mem.h"
#include "ecmd.h"
#include "d3_par.h"
#include "data_views.h"
#define d3_WorldMatrixD ((void (*)(D3OBJECT *, D3OBJECT *, int, void *))d3_WorldMatrix)  /* 0x458da0 (unk_458810.h: XOBJ *, XOBJ *, char, D3MATRIX *): this unit's D3OBJECT view of the object */

/* 0x455630, empties the default scene's particle list (the head d3_ParHead becomes the end marker d3_ParEnd,
   the tail d3_ParTail the head node d3if_ParList) and sets the two quad UV tables */
void par_Init(void)
{
    d3_ParHead = &d3_ParEnd;
    d3_ParEnd.next = 0;
    D3PAR_HEADNODE->prev = 0;
    d3_ParTail = D3PAR_HEADNODE;
    d3_ParUV0[0] = 0.0f;
    d3_ParUV0[1] = 0.0f;
    d3_ParUV0[2] = 1.0f;
    d3_ParUV0[3] = 1.0f;
    d3_ParUV0[4] = 0.0f;
    d3_ParUV0[5] = 1.0f;
    d3_ParUV1[0] = 0.0f;
    d3_ParUV1[1] = 0.0f;
    d3_ParUV1[2] = 1.0f;
    d3_ParUV1[3] = 0.0f;
    d3_ParUV1[4] = 1.0f;
    d3_ParUV1[5] = 1.0f;
}
/* 0x4556b0, deletes every particle system of the default scene (from the head until the end marker) */

void par_DeleteAll(void)
{
    while (d3_ParHead != &d3_ParEnd)
        par_Delete(d3_ParHead);
}
/* 0x4556e0, unlinks a particle system: fixes the neighbours (or the scene's head / tail when it is the first /
   last system) and, in the default scene, the global list head / tail */

void par_Unlink(D3PARTSYS *ps)
{
    D3SCENE *s = ps->scene;

    if (ps->prev == D3PAR_HEADNODE)
        D3PAR_SCENE_PAR(s)->head = ps->next;
    else
        ps->prev->next = ps->next;
    if (ps->next == &d3_ParEnd)
        D3PAR_SCENE_PAR(s)->tail = ps->prev;
    else
        ps->next->prev = ps->prev;
    if (s == d3_DefaultScene) {
        d3_ParHead = D3PAR_SCENE_PAR(s)->head;
        d3_ParTail = D3PAR_SCENE_PAR(s)->tail;
    }
}
/* 0x455760 */

void par_Delete(D3PARTSYS *ps)
{
    D3SCENE *s = ps->scene;

    if (s == d3_DefaultScene) {
        ps->next->prev = ps->prev;
        ps->prev->next = ps->next;
        if (ps == D3PAR_SCENE_PAR(s)->head)
            D3PAR_SCENE_PAR(s)->head = ps->next;
        if (ps == D3PAR_SCENE_PAR(s)->tail)
            D3PAR_SCENE_PAR(s)->tail = ps->next;
    } else
        par_Unlink(ps);
    if (ps->tex)
        tex_Release(ps->tex);
    mem_Free(ps->particles);
    ps->particles = 0;
    mem_Free(ps->points);
    ps->points = 0;
    mem_Free(ps->polys);
    ps->polys = 0;
    mem_Free(ps);
}

/* particle at byte offset j (the original walks the array with an explicit offset, do-while loop) */
#define PAR_AT(ps, j) ((D3PARTICLE *)((char *)(ps)->particles + (j)))

/* 0x455830, allocates a particle system for count particles with texture tex, initializes every particle
   and appends the system to the default scene's list (d3_ParTail / d3_ParEnd). Returns 0 when size or
   count is not positive or the texture cannot be loaded. */
D3PARTSYS *par_Create(int count, char *tex, D3TEXFLAGS flags, float size, void *parent, D3PARTCB cb)
{
    D3PARTSYS *ps = 0;
    D3TEXHANDLE *h;
    int i;
    unsigned long now;
    unsigned int j;

    if (size > 0.0 && (unsigned)count > 0) {
        h = (D3TEXHANDLE *)tex_Load(tex, flags, 0, 0);
        if (h) {
            tex_Reload(h, 0);
            ps = mem_Alloc(0x31c, "d3_par:ParticleSystem");
            ps->tex = h;
            ps->texindex = ((char *)h - (char *)d3_Textures) >> 7;
            ps->count = count;
            ps->size = size;
            ps->callback = cb;
            ps->particles = mem_Alloc(count * 0x54, "d3_par:Particles");
            ps->points = mem_Alloc(ps->count * 0x140, "d3_par:ParticlePoints");
            ps->polys = mem_Alloc(ps->count * 0x50, "d3_par:ParticlePolys");
            i = 0;
            ps->v00.x = 0;
            ps->v00.y = 0;
            ps->v00.z = 0;
            ps->v00.w = 0;
            ps->v10.x = 0;
            ps->v10.y = 0;
            ps->v10.z = 0;
            ps->v10.w = 0;
            D3PAR_TIME(ps)->lasttime = 0;
            ps->flags = 0;
            now = d3_Time;
            if (i < ps->count) {
                j = 0;
                do {
                    PAR_AT(ps, j)->pos.x = 0;
                    PAR_AT(ps, j)->pos.y = 0;
                    PAR_AT(ps, j)->pos.z = 0;
                    PAR_AT(ps, j)->vel.x = 0;
                    PAR_AT(ps, j)->vel.y = 0;
                    PAR_AT(ps, j)->vel.z = 0;
                    PAR_AT(ps, j)->time = now;
                    PAR_AT(ps, j)->size = ps->size;
                    PAR_AT(ps, j)->flags &= 0xfe;
                    PAR_AT(ps, j)->frame = 0;
                    PAR_AT(ps, j)->color = 0xffffffff;
                    i++;
                    j += 0x54;
                } while (i < ps->count);
            }
            *((unsigned char *)ps + 0x2fb) |= 8;
            d3_InitObject((D3OBJECT *)ps->obj);
            ps->parent = parent;
            ps->scene = d3_DefaultScene;
            ps->prev = d3_ParTail;
            ps->next = &d3_ParEnd;
            d3_ParTail->next = ps;
            d3_ParTail = ps;
        }
    }
    return ps;
}
/* 0x455a20, moves the position of the system's embedded object (the particles are placed relative to it) */

void par_SetPos(D3PARTSYS *ps, D3VECTOR *pos)
{
    if (ps)
        d3_SetObjectPos((D3OBJECT *)ps->obj, pos);
}
/* 0x455a40, orientation from a direction and a roll angle */

void par_SetDir(D3PARTSYS *ps, D3VECTOR *dir, float roll)
{
    D3MATRIX m;
    D3VECTOR rot;

    if (ps) {
        d3_MakeBasis(dir, roll, &m);
        d3_MatToAngles((D3VECTOR4 *)&m, (D3VECTOR4 *)&rot);
        d3_SetObjectRot((D3OBJECT *)ps->obj, &rot);
    }
}
/* 0x455a90, per-frame update and draw of a particle system: runs its callback (no callback or a zero return deletes
   the system), builds one camera-facing quad (two tris) per live particle and puts the visible tris on the sort list */

void par_Draw(D3PARTSYS *ps)
{
    D3MATRIX m;
    D3PARTICLE *p;
    D3PVERT *v, *v1, *v2, *v3;
    D3PTRI *t;
    D3TEXTURE *tex;
    PARCOL col;
    D3VECTOR4 c;
    float rz, half, sx, sy, hw, hh;
    unsigned char fog;
    char vis;
    int i, n;
    D3PTRI *t1;

    if (ps->callback && ps->callback(ps, d3_Time)) {
        d3_TriCount += ps->count * 2;
        d3_VertCount += ps->count * 4;
        d3_WorldMatrixD(PAR_OBJ(ps), (PAR_OBJ(ps)->state & 0x80) ? 0 : d3_Camera, 0, &m);
        for (i = 0; i < ps->count; i++) {
            p = &ps->particles[i];
            vis = 0;
            t = &PAR_TRIS(ps)[i * 2];
            if ((p->flags & 1) && p->a > 0) {
                rz = 0.0f;
                half = p->size * 0.5f;
                v = &PAR_VERTS(ps)[i * 4];
                c.x = (m._11 * p->pos.x) + m._31 * p->pos.z + m._21 * p->pos.y + m._41;
                c.y = (m._12 * p->pos.x) + m._32 * p->pos.z + m._22 * p->pos.y + m._42;
                c.z = (m._13 * p->pos.x) + m._33 * p->pos.z + m._23 * p->pos.y + m._43;
                rz = 1.0f / c.z;
                d3_Dist2 = c.x * c.x + c.y * c.y + c.z * c.z;
                fog = (unsigned char)(d3_Dist2 > d3_FogStart2
                    ? 255.0 - min(255.0, (sqrt(d3_Dist2) - d3_FogStart) * d3_FogScale) : 255.0);
                if (!(d3_MinZ < c.z))
                    d3_MinZ = c.z;
                if (!(d3_MaxZ > c.z))
                    d3_MaxZ = c.z;
                vis = 1;
                sx = c.x * (d3_ScaleX * rz) + d3_CenterX;
                sy = c.y * (d3_ScaleY * rz) + d3_CenterY;
                hw = (d3_ScaleX * rz) * half;
                hh = -((d3_ScaleY * rz) * half);
                if (sx - hw >= d3_Viewport.right || sx + hw < d3_Viewport.left
                    || sy - hh >= d3_Viewport.bottom || sy + hh < d3_Viewport.top
                    || c.z >= d3_ZFar || c.z < d3_ZNear)
                    vis = 0;
            }
            t1 = t + 1;
            PAR_TF(&t[0])->vis = PAR_TF(t1)->vis = vis;
            if (vis) {
                v1 = v + 1;
                v2 = v + 2;
                v3 = v + 3;
                if (p->frame > 0)
                    tex = tex_GetFrame(ps->texindex, p->frame);
                else
                    tex = (D3TEXTURE *)ps->tex;
                t[0].v[0] = v;
                t[0].v[1] = v1;
                t[0].v[2] = v2;
                t[0].src = d3_ParUV0;
                t[0].tex = tex;
                t1->v[0] = v;   /* through t1: needed for VC6's order (the tex test after this store) */
                t[1].v[1] = v3;
                t[1].v[2] = v1;
                t[1].src = d3_ParUV1;
                t[1].tex = tex;
                if (tex && (PAR_TEXBITS(tex) & 1)) {
                    col.c.r = p->r * p->a >> 8;
                    col.c.g = p->g * p->a >> 8;
                    col.c.b = p->b * p->a >> 8;
                } else
                    col.color = p->color;
                v->color = v1->color = v2->color = v3->color = col.color;
                v->fog = v1->fog = v2->fog = v3->fog = fog;
                PAR_VF(v)->b4 = PAR_VF(v1)->b4 = PAR_VF(v2)->b4 = PAR_VF(v3)->b4 = 0;
                PAR_VF(v)->b5 = PAR_VF(v1)->b5 = PAR_VF(v2)->b5 = PAR_VF(v3)->b5 = 0;
                PAR_VF(v)->vis = PAR_VF(v1)->vis = PAR_VF(v2)->vis = PAR_VF(v3)->vis = 1;
                v->sx = sx - hw;
                v->sy = sy - hh;
                v->rhw = rz;
                v->z = c.z;
                v->x = c.x - half;
                v->y = c.y + half;
                PAR_VF(v)->l = PAR_VF(v2)->l = v->sx < d3_Viewport.left;
                PAR_VF(v)->r = PAR_VF(v2)->r = v->sx >= d3_Viewport.right;
                PAR_VF(v)->t = PAR_VF(v3)->t = v->sy < d3_Viewport.top;
                PAR_VF(v)->b = PAR_VF(v3)->b = v->sy >= d3_Viewport.bottom;
                v1->sx = sx + hw;
                v1->sy = sy + hh;
                v1->rhw = rz;
                v1->z = c.z;
                v1->x = c.x + half;
                v1->y = c.y - half;
                PAR_VF(v1)->l = PAR_VF(v3)->l = v1->sx < d3_Viewport.left;
                PAR_VF(v1)->r = PAR_VF(v3)->r = v1->sx >= d3_Viewport.right;
                PAR_VF(v1)->t = PAR_VF(v2)->t = v1->sy < d3_Viewport.top;
                PAR_VF(v1)->b = PAR_VF(v2)->b = v1->sy >= d3_Viewport.bottom;
                v2->sx = sx - hw;
                v2->sy = sy + hh;
                v2->rhw = rz;
                v2->x = c.x - half;
                v2->y = c.y - half;
                v2->z = c.z;
                v3->sx = sx + hw;
                v3->sy = sy - hh;
                v3->rhw = rz;
                v3->x = c.x + half;
                v3->y = c.y + half;
                v3->z = c.z;
            }
        }
        n = min(ps->count * 2, d3_SortMax - (int)d3_SortCount);
        if (d3_Hardware) {
            for (t = PAR_TRIS(ps); n > 0; n--, t++) {
                if (t->flags & 0x80) {
                    D3TEXTURE *tt = t->tex;
                    d3_SortPtr->item = t;
                    if (tt) {
                        d3_SortPtr->key = (tt - (D3TEXTURE *)d3_Textures) + 0x8000;
                        D3PAR_TEXSTAMP(tt) = d3_FrameCount;
                    } else
                        d3_SortPtr->key = 0;
                    t->clip = &d3_ClipNone;
                    d3_SortPtr++;
                    d3_SortCount++;
                }
            }
        } else {
            v = PAR_VERTS(ps);
            for (i = 0; i < ps->count * 4; i++, v++) {
                if (v->flags & 0x80)
                    v->c.r = (unsigned char)((v->c.b * 0.11f + v->c.r * 0.3f + v->c.g * 0.59f) * 0.25f);
            }
            for (t = PAR_TRIS(ps); n > 0; n--, t++) {
                if (t->flags & 0x80) {
                    d3_SortPtr->item = t;
                    t->clip = &d3_ClipNone;
                    d3_SortPtr->key = (unsigned long)((d3_InvZFar * 16777216.0f) * t->v[0]->z * 3.0);
                    d3_SortPtr++;
                    d3_SortCount++;
                }
            }
        }
    } else
        par_Delete(ps);
}
/* 0x456350, emitter type 0: particles start on a sphere shell (radius between size[0] and size[1]) and
   fly outwards (inwards if size[0] > size[1]) with the speed of dir */

char par_EmitterCB0(D3PARTSYS *ps, unsigned long time)
{
    int frames = 1;
    PARPART *p;
    D3EMITTER *e;
    D3VECTOR4 r;
    float dt, s, speed, rad;
    int max, spawned, i, age;
    unsigned char fl;
    float *sz;

    e = PAR_E(ps);
    if (D3PAR_TIME(ps)->lasttime == time)
        return 1;
    s = (float)sqrt(e->dir[0] * e->dir[0] + e->dir[1] * e->dir[1] + e->dir[2] * e->dir[2]);
    speed = s;
    if (e->size[0] > e->size[1])
        speed = -speed;
    if (e->istrigger)
        max = (int)(ps->count * e->maxtrigger);
    else
        max = ps->count;
    if (ps->tex && ps->tex->frames > 0)
        frames = ps->tex->frames;
    D3PAR_INV(ps)->inv_alpha0 = 1.0f / e->alpha[0];
    D3PAR_INV(ps)->inv_alpha_gap = 1.0f / (e->alpha[2] - e->alpha[1]);
    D3PAR_INV(ps)->inv_size = 1.0f / (float)fabs(e->size[1] - e->size[0]);
    spawned = 0;
    p = (PARPART *)ps->particles;
    for (i = 0; i < ps->count; i++, p++) {
        if (!(p->flags & 1)) {
            if (spawned < max && (!e->istrigger || e->trigger)) {
                spawned++;
                p->phase[0] = PAR_RND() * 6.2831855f;
                p->phase[1] = PAR_RND() * 6.2831855f;
                p->phase[2] = PAR_RND() * 6.2831855f;
                p->phase[3] = PAR_RND() * 6.2831855f;
                p->vel.w = PAR_RND() * 6.2831855f;
                p->frame = i % frames;
                p->r = (rand() & e->a) + e->r;
                p->g = (rand() & e->a) + e->g;
                p->b = (rand() & e->a) + e->b;
                p->a = 0;
                p->time = time;
                r.x = PAR_RND() - 0.5f;
                r.y = PAR_RND() - 0.5f;
                r.z = PAR_RND() - 0.5f;
                d3_Normalize(&r);
                if (!e->initfill && !e->rebirthfill) {
                    p->birth = time;
                    p->base[0] = (e->size[0]) * r.x;
                    p->base[1] = (e->size[0]) * r.y;
                    p->base[2] = (e->size[0]) * r.z;
                } else {
                    float f = PAR_RND();
                    p->birth = (unsigned long)(time - e->alpha[2] * f);
                    rad = min(e->size[0], e->size[1]);
                    rad += (max(e->size[0], e->size[1]) - rad) * f;
                    p->base[0] = r.x * rad;
                    p->base[1] = r.y * rad;
                    p->base[2] = r.z * rad;
                }
                p->vel.x = r.x * speed;
                p->vel.y = r.y * speed;
                p->vel.z = r.z * speed;
                r.x = PAR_RND();
                r.y = PAR_RND();
                r.z = PAR_RND();
                r.x *= e->rnd[0];
                r.y *= e->rnd[1];
                r.z *= e->rnd[2];
                p->vel.x += r.x;
                p->vel.y += r.y;
                p->vel.z += r.z;
                r.x = (float)sin(p->phase[0]);
                r.y = (float)sin(p->phase[1]);
                r.z = (float)sin(p->phase[2]);
                r.w = (float)sin(p->phase[3]);
                p->pos.x = r.x * e->amp[0] + p->base[0];
                p->pos.y = r.y * e->amp[1] + p->base[1];
                p->pos.z = r.z * e->amp[2] + p->base[2];
                /* mark alive; the flags read before the size store and the size written through a pointer are
                   needed for VC6's order (flags load hoisted, byte store after the size fstp) */
                fl = p->flags | 1;
                sz = &p->size;
                *sz = r.w * e->amp3 + ps->size;
                p->flags = fl;
            }
        } else {
            dt = (float)(time - p->time);
            if (dt > 150.0f) {
                dt = 0.0f;
                p->birth += time - p->time;
            }
            age = time - p->birth;
            p->time = time;
            if (e->texmode)
                p->frame = age / e->texmode % frames;
            p->phase[0] += dt * e->phase[0];
            p->phase[1] += dt * e->phase[1];
            p->phase[2] += dt * e->phase[2];
            p->phase[3] += dt * e->phase3;
            p->phase[0] = (float)fmod(p->phase[0], 6.2831855f);
            p->phase[1] = (float)fmod(p->phase[1], 6.2831855f);
            p->phase[2] = (float)fmod(p->phase[2], 6.2831855f);
            p->phase[3] = (float)fmod(p->phase[3], 6.2831855f);
            p->vel.w = (float)fmod(dt * e->phase4 + p->vel.w, 6.2831855f);
            p->vel.x += dt * e->acc[0];
            p->vel.y += dt * e->acc[1];
            p->vel.z += dt * e->acc[2];
            r.x = e->vel[0] + p->vel.x;
            r.y = e->vel[1] + p->vel.y;
            r.z = e->vel[2] + p->vel.z;
            p->base[0] += dt * r.x;
            p->base[1] += dt * r.y;
            p->base[2] += dt * r.z;
            r.x = (float)sin(p->phase[0]);
            r.y = (float)sin(p->phase[1]);
            r.z = (float)sin(p->phase[2]);
            r.w = (float)sin(p->phase[3]);
            p->pos.x = r.x * e->amp[0] + p->base[0];
            p->pos.y = r.y * e->amp[1] + p->base[1];
            p->pos.z = r.z * e->amp[2] + p->base[2];
            p->size = r.w * e->amp3 + ps->size;
            p->pos.w = (float)sin(p->vel.w) * e->amp4 + e->time;
            if ((e->plane3 < 0.0f && p->pos.z < e->plane3) || (e->plane3 > 0.0f && p->pos.z > e->plane3)) {
                p->pos.z = e->plane3;
                p->vel.x *= e->plane[0];
                p->vel.y *= e->plane[1];
                p->vel.z *= e->plane[2];
            }
            if (age < e->alpha[0]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, (1.0f - (e->alpha[0] - age) * D3PAR_INV(ps)->inv_alpha0) * p->pos.w));
            } else if (age < e->alpha[1]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, p->pos.w));
            } else if (age < e->alpha[2]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, ((e->alpha[2] - age) * p->pos.w) * D3PAR_INV(ps)->inv_alpha_gap));
            } else {
                p->flags &= ~1;
                ps->flags++;
            }
        }
    }
    if (e->initfill)
        e->initfill = 0;
    if (e->trigger)
        e->trigger = 0;
    D3PAR_TIME(ps)->lasttime = time;
    return !(e->triggeronce && (int)ps->flags >= max);
}
/* 0x456b60, emitter type 1: particles start on a disc of radius size[0] and fly towards the disc of
   radius size[1] at height size[2] (a cone) */

char par_EmitterCB1(D3PARTSYS *ps, unsigned long time)
{
    int frames = 1;
    PARPART *p;
    D3EMITTER *e;
    D3VECTOR4 r;
    float dt, s, speed, rad;
    int max, spawned, i, age;

    e = PAR_E(ps);
    if (D3PAR_TIME(ps)->lasttime == time)
        return 1;
    s = (float)sqrt(e->dir[0] * e->dir[0] + e->dir[1] * e->dir[1] + e->dir[2] * e->dir[2]);
    speed = s;
    if (e->istrigger)
        max = (int)(ps->count * e->maxtrigger);
    else
        max = ps->count;
    if (ps->tex && ps->tex->frames > 0)
        frames = ps->tex->frames;
    D3PAR_INV(ps)->inv_alpha0 = 1.0f / e->alpha[0];
    D3PAR_INV(ps)->inv_alpha_gap = 1.0f / (e->alpha[2] - e->alpha[1]);
    D3PAR_INV(ps)->inv_size = 1.0f / e->size[2];
    D3PAR_INV(ps)->size_ratio = e->size[1] / e->size[0];
    spawned = 0;
    p = (PARPART *)ps->particles;
    for (i = 0; i < ps->count; i++, p++) {
        if (!(p->flags & 1)) {
            if (spawned < max && (!e->istrigger || e->trigger)) {
                spawned++;
                p->phase[0] = PAR_RND() * 6.2831855f;
                p->phase[1] = PAR_RND() * 6.2831855f;
                p->phase[2] = PAR_RND() * 6.2831855f;
                p->phase[3] = PAR_RND() * 6.2831855f;
                p->vel.w = PAR_RND() * 6.2831855f;
                p->frame = i % frames;
                p->r = (rand() & e->a) + e->r;
                p->g = (rand() & e->a) + e->g;
                p->b = (rand() & e->a) + e->b;
                p->a = 0;
                p->time = time;
                r.x = PAR_RND() - 0.5f;
                r.y = (float)rand() / RAND_MAX - 0.5f;
                r.z = 0.0f;
                d3_Normalize(&r);
                if (!e->initfill && !e->rebirthfill) {
                    p->birth = time;
                    rad = PAR_RND() * e->size[0];
                    p->base[0] = r.x * rad;
                    p->base[1] = r.y * rad;
                    p->base[2] = r.z * rad;
                    if (e->size[0] != 0.0f)
                        rad *= D3PAR_INV(ps)->size_ratio;
                    else {
                        p->base[0] = 0.0f;
                        p->base[1] = 0.0f;
                        p->base[2] = 0.0f;
                        rad = PAR_RND() * e->size[1];
                    }
                    r.x *= rad;
                    r.y *= rad;
                    r.z = e->size[2];
                    r.x -= p->base[0];
                    r.y -= p->base[1];
                    r.z -= p->base[2];
                    d3_Normalize(&r);
                    p->vel.x = r.x * speed;
                    p->vel.y = r.y * speed;
                    p->vel.z = r.z * speed;
                } else {
                    float f;
                    if (e->size[0] != 0.0f) {
                        rad = PAR_RND() * e->size[0];
                        p->base[0] = r.x * rad;
                        p->base[1] = r.y * rad;
                        p->base[2] = r.z * rad;
                        rad *= D3PAR_INV(ps)->size_ratio;
                    } else {
                        p->base[0] = 0.0f;
                        p->base[1] = 0.0f;
                        p->base[2] = 0.0f;
                        rad = PAR_RND() * e->size[1];
                    }
                    r.x *= rad;
                    r.y *= rad;
                    r.z = e->size[2];
                    r.x -= p->base[0];
                    r.y -= p->base[1];
                    r.z -= p->base[2];
                    d3_Normalize(&r);
                    f = PAR_RND();
                    p->birth = (unsigned long)(time - e->alpha[2] * f);
                    f *= e->size[2];
                    p->base[0] += r.x * f;
                    p->base[1] += r.y * f;
                    p->base[2] += f * r.z;
                    /* the velocity written in both arms (VC6 tail-merges them) gives speed the weight
                       that puts it into the dead `time` slot, as in the original */
                    p->vel.x = r.x * speed;
                    p->vel.y = r.y * speed;
                    p->vel.z = r.z * speed;
                }
                r.x = PAR_RND();
                r.y = PAR_RND();
                r.z = PAR_RND();
                r.x *= e->rnd[0];
                r.y *= e->rnd[1];
                r.z *= e->rnd[2];
                p->vel.x += r.x;
                p->vel.y += r.y;
                p->vel.z += r.z;
                r.x = (float)sin(p->phase[0]);
                r.y = (float)sin(p->phase[1]);
                r.z = (float)sin(p->phase[2]);
                r.w = (float)sin(p->phase[3]);
                p->pos.x = r.x * e->amp[0] + p->base[0];
                p->pos.y = r.y * e->amp[1] + p->base[1];
                p->pos.z = r.z * e->amp[2] + p->base[2];
                p->size = r.w * e->amp3 + ps->size;
                p->flags |= 1;
            }
        } else {
            dt = (float)(time - p->time);
            if (dt > 150.0f) {
                dt = 0.0f;
                p->birth += time - p->time;
            }
            age = time - p->birth;
            p->time = time;
            if (e->texmode)
                p->frame = age / e->texmode % frames;
            p->phase[0] += dt * e->phase[0];
            p->phase[1] += dt * e->phase[1];
            p->phase[2] += dt * e->phase[2];
            p->phase[3] += dt * e->phase3;
            p->phase[0] = (float)fmod(p->phase[0], 6.2831855f);
            p->phase[1] = (float)fmod(p->phase[1], 6.2831855f);
            p->phase[2] = (float)fmod(p->phase[2], 6.2831855f);
            p->phase[3] = (float)fmod(p->phase[3], 6.2831855f);
            p->vel.w = (float)fmod(dt * e->phase4 + p->vel.w, 6.2831855f);
            p->vel.x += dt * e->acc[0];
            p->vel.y += dt * e->acc[1];
            p->vel.z += dt * e->acc[2];
            r.x = e->vel[0] + p->vel.x;
            r.y = e->vel[1] + p->vel.y;
            r.z = e->vel[2] + p->vel.z;
            p->base[0] += dt * r.x;
            p->base[1] += dt * r.y;
            p->base[2] += dt * r.z;
            r.x = (float)sin(p->phase[0]);
            r.y = (float)sin(p->phase[1]);
            r.z = (float)sin(p->phase[2]);
            r.w = (float)sin(p->phase[3]);
            p->pos.x = r.x * e->amp[0] + p->base[0];
            p->pos.y = r.y * e->amp[1] + p->base[1];
            p->pos.z = r.z * e->amp[2] + p->base[2];
            p->size = r.w * e->amp3 + ps->size;
            p->pos.w = (float)sin(p->vel.w) * e->amp4 + e->time;
            if ((e->plane3 < 0.0f && p->pos.z < e->plane3) || (e->plane3 > 0.0f && p->pos.z > e->plane3)) {
                p->pos.z = e->plane3;
                p->vel.x *= e->plane[0];
                p->vel.y *= e->plane[1];
                p->vel.z *= e->plane[2];
            }
            if (age < e->alpha[0]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, (1.0f - (e->alpha[0] - age) * D3PAR_INV(ps)->inv_alpha0) * p->pos.w));
            } else if (age < e->alpha[1]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, p->pos.w));
            } else if (age < e->alpha[2]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, ((e->alpha[2] - age) * p->pos.w) * D3PAR_INV(ps)->inv_alpha_gap));
            } else {
                p->flags &= ~1;
                ps->flags++;
            }
        }
    }
    if (e->initfill)
        e->initfill = 0;
    if (e->trigger)
        e->trigger = 0;
    D3PAR_TIME(ps)->lasttime = time;
    return !(e->triggeronce && (int)ps->flags >= max);
}
/* 0x457460, emitter type 2: particles drift on a sine wave around a moving base position */

char par_EmitterCB2(D3PARTSYS *ps, unsigned long time)
{
    int frames = 1;
    PARPART *p;
    D3EMITTER *e;
    D3VECTOR4 r;
    float s0, s1, s2, s3;
    float dt;
    int max, spawned, i, age;

    e = PAR_E(ps);
    if (D3PAR_TIME(ps)->lasttime == time)
        return 1;
    if (e->istrigger)
        max = (int)(ps->count * e->maxtrigger);
    else
        max = ps->count;
    if (ps->tex && ps->tex->frames > 0)
        frames = ps->tex->frames;
    D3PAR_INV(ps)->inv_alpha0 = 1.0f / e->alpha[0];
    D3PAR_INV(ps)->inv_alpha_gap = 1.0f / (e->alpha[2] - e->alpha[1]);
    spawned = 0;
    p = (PARPART *)ps->particles;
    for (i = 0; i < ps->count; i++, p++) {
        if (!(p->flags & 1)) {
            if (spawned < max && (!e->istrigger || e->trigger)) {
                spawned++;
                p->phase[0] = PAR_RND() * 6.2831855f;
                p->phase[1] = PAR_RND() * 6.2831855f;
                p->phase[2] = PAR_RND() * 6.2831855f;
                p->phase[3] = PAR_RND() * 6.2831855f;
                p->vel.w = PAR_RND() * 6.2831855f;
                p->frame = i % frames;
                p->r = (rand() & e->a) + e->r;
                p->g = (rand() & e->a) + e->g;
                p->b = (rand() & e->a) + e->b;
                p->a = 0;
                p->time = time;
                r.x = PAR_RND() * 2.0f - 1.0f;
                r.y = PAR_RND() * 2.0f - 1.0f;
                r.z = PAR_RND() * 2.0f - 1.0f;
                p->base[0] = r.x * e->size[0];
                p->base[1] = r.y * e->size[1];
                p->base[2] = r.z * e->size[2];
                if (e->rebirthfill) {
                    float f = PAR_RND();
                    p->birth = (unsigned long)(time - e->alpha[2] * f);
                }
                else
                    p->birth = time;
                p->vel.x = PAR_RND();
                p->vel.y = PAR_RND();
                p->vel.z = PAR_RND();
                p->vel.x = e->rnd[0] * p->vel.x;
                p->vel.y = e->rnd[1] * p->vel.y;
                p->vel.z = e->rnd[2] * p->vel.z;
                p->flags |= 1;
            }
        } else {
            dt = (float)(time - p->time);
            if (dt > 150.0f) {
                dt = 0.0f;
                p->birth += time - p->time;
            }
            p->time = time;
            if (e->texmode)
                p->frame = (int)(time - p->birth) / e->texmode % frames;
            p->phase[0] += dt * e->phase[0];
            p->phase[1] += dt * e->phase[1];
            p->phase[2] += dt * e->phase[2];
            p->phase[3] += dt * e->phase3;
            p->phase[0] = s0 = (float)fmod(p->phase[0], 6.2831855f);
            p->phase[1] = s1 = (float)fmod(p->phase[1], 6.2831855f);
            p->phase[2] = s2 = (float)fmod(p->phase[2], 6.2831855f);
            p->phase[3] = s3 = (float)fmod(p->phase[3], 6.2831855f);
            p->vel.w = (float)fmod(dt * e->phase4 + p->vel.w, 6.2831855f);
            p->vel.x += dt * e->acc[0];
            p->vel.y += dt * e->acc[1];
            p->vel.z += dt * e->acc[2];
            r.x = e->vel[0] + p->vel.x;
            r.y = e->vel[1] + p->vel.y;
            r.z = e->vel[2] + p->vel.z;
            p->base[0] += dt * r.x;
            p->base[1] += dt * r.y;
            p->base[2] += dt * r.z;
            r.x = (float)sin(s0);
            r.y = (float)sin(s1);
            r.z = (float)sin(s2);
            r.w = (float)sin(s3);
            p->pos.x = r.x * e->amp[0] + p->base[0];
            p->pos.y = r.y * e->amp[1] + p->base[1];
            p->pos.z = r.z * e->amp[2] + p->base[2];
            p->size = r.w * e->amp3 + ps->size;
            p->pos.w = (float)sin(p->vel.w) * e->amp4 + e->time;
            if ((e->plane3 < 0.0f && p->pos.z < e->plane3) || (e->plane3 > 0.0f && p->pos.z > e->plane3)) {
                p->pos.z = e->plane3;
                p->vel.x *= e->plane[0];
                p->vel.y *= e->plane[1];
                p->vel.z *= e->plane[2];
            }
            age = time - p->birth;
            if (age < e->alpha[0]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, (1.0f - (e->alpha[0] - age) * D3PAR_INV(ps)->inv_alpha0) * p->pos.w));
            } else if (age < e->alpha[1]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, p->pos.w));
            } else if (age < e->alpha[2]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, (e->alpha[2] - age) * D3PAR_INV(ps)->inv_alpha_gap * p->pos.w));
            } else {
                p->flags &= ~1;
                ps->flags++;
            }
        }
    }
    if (e->initfill)
        e->initfill = 0;
    if (e->trigger)
        e->trigger = 0;
    D3PAR_TIME(ps)->lasttime = time;
    return !(e->triggeronce && (int)ps->flags >= max);
}
/* 0x457b30. The first parameter is the parent (ecmd.h names it count), the third the particle count. */

D3PARTSYS *par_CreateEmitter(int parent, D3VECTOR *pos, int count, char *tex, OCMDTEXFLAGS flags, float size,
                             D3EMITTERCFG *cfg)
{
    D3PARTSYS *ps = 0;

    if (cfg->type == 0)
        ps = par_Create(count, tex, flags.tex, size, (void *)parent, par_EmitterCB0);
    else if (cfg->type == 1)
        ps = par_Create(count, tex, flags.tex, size, (void *)parent, par_EmitterCB1);
    else if (cfg->type == 2)
        ps = par_Create(count, tex, flags.tex, size, (void *)parent, par_EmitterCB2);
    if (ps) {
        par_SetPos(ps, pos);
        D3_COPY((char *)ps + 0x2c, cfg, sizeof(D3EMITTERCFG));
        par_SetDir(ps, (D3VECTOR *)PAR_EMITTER(ps)->dir, 0.0f);
        if (PAR_EMITTER(ps)->triggeronce) {
            ps->flags = 0;
            PAR_EMITTER(ps)->trigger = 1;
        }
    }
    return ps;
}

float d3_ParUV0[6] = {0};  /* 0xfb95a0, (0,0) (1,1) (0,1) */
float d3_ParUV1[6] = {0};  /* 0xfb95c0, (0,0) (1,0) (1,1) */
