/* d3_engine.c -- original path D:\develop\Units\d3\d3_engine.c (assert ref 0x43a939)
   0x438c30..0x43e6b0: its .data starts at 0x5fb5b0 (d3_DefaultScene) right after d3_d3d.c's strings and
   holds the d3_engine.c path, and the first functions (0x438c30..) use d3_DefaultScene; its strings end with
   "_S" / "_1" / "_0" (0x5fb7fc); the event functions after it are d3_event.c's. Functions are kept in their
   original order. Names are ours.
   Open: d3_engine functions are still declared in other units' headers (d3.h, rd.h, ch.h, cm.h,
   d3_interface.h, d3_int_io.h; K&R aliases in f3init.c); their callers must include d3_engine.h first.
   Open: d3_CloneObject (and d3_mir.c's d3_MirDrawObject) flip with TU-global changes - definitions added
   above them, declarations added to or removed from d3_engine.h / d3.h - so header moves need both rechecked.
   Open: D3TEXTURE's tex_Load flag bytes 0x6c (alpha) / 0x6e (blend) are read through the D3TEXV view: the same
   fields split out of d3.h's pad69 move he_PostEvent67 and ai_TutorialInitChapter2/3 off byte (struct text alone).
   unit of rea 8cef7d5b (src/game/d3_engine.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3_fl.c defines these with char / unsigned int parameters; the 0x43c4e0 caller pushes ints. */
#define d3fl_SetMips d3fl_SetMips
#define d3fl_SetDetail d3fl_SetDetail
/* d3.h declares d3_ForEachObject as void; it returns a char. */
#define d3_ForEachObject d3_ForEachObject_void
/* and d3_UpdateObject's second parameter as int; it is a byte. */
#define d3_UpdateObject d3_UpdateObject_int
/* and d3_UpdateWorldMatrix's third parameter as int; it is a char. */
#define d3_UpdateWorldMatrix d3_UpdateWorldMatrix_int
#include "d3.h"
#undef d3_ForEachObject
#undef d3_UpdateObject
#undef d3_UpdateWorldMatrix
#include "d3_sky.h"
#include "d3_vec.h"
#include "d3_engine.h"
extern unsigned char d3_NoTextures;  /* 0x63345d, defined below; ecmd.c declares it int (it stores a dword) */
#include "d3_anim.h"
/* d3_mir.h types d3_MirDrawObject / d3_MirBeginFrame with D3MIRROR *; d3_engine.h keeps the D3OBJECT * views. */
#define d3_MirDrawObject d3_Unk4550e0_mirview
#define d3_MirBeginFrame d3_Unk4554b0_mirview
#include "d3_mir.h"
#undef d3_MirDrawObject
#undef d3_MirBeginFrame
#include "d3_light.h"
#include "data_views.h"
#include "d3_d3d.h"
#include "dd_vesa_types.h"
#include "d3_fl.h"
#include "d3_int_io.h"
#include "d3_interface.h"
#include "d3_io_a.h"
#include "d3_io_t.h"
#include "d3_par.h"
#include "d3_sm.h"
#include "d3_snd.h"
#include "d3_tools.h"
#include "d3s.h"
#include "f3gm.h"
#include "f3init.h"
#include "mem.h"
#include "tb.h"
#include "ts_texture.h"
#include "d3_pose.h"
#include "d3_vtx.h"
#include "poker.h"
#include "ch.h"
#include "bio.h"
void dd_Clear(D3RECT *rect, unsigned char r, unsigned char g, unsigned char b);  /* 0x41e100, @original: dd_vesa.c defines it with (RECT *, int r, int g, int b) and this unit does not include dd_vesa.h; it passes the fog colour's bytes unwidened (mov cl,al / mov al,ah / mov dl,[..]; push) and its D3RECT viewport */
/* The stock list head lives inside its 0xfc2e00 head sentinel (the original's +0x1fc relationship). As a
   separate COMMON pointer the linker would not keep it adjacent, so the loader would link the first object
   into one address while the readers start from another. */
extern D3INITCFG d3_Cfg;
extern D3PARTSYS d3_ParEnd;
void d3fl_FreeMap(struct D3FLMAP *map);     /* 0x43ff90, d3_fl.c (d3.h types d3_Terrain as void *) */
/* 0x438c30, the next sibling that starts a new group (state bit 0), for top-level objects. */
D3OBJECT *d3_NextGroup(D3OBJECT *obj)
{
    D3OBJECT *parent = obj->parent;

    obj = obj->next;
    if (!parent)
        while (obj && !(obj->state & 1))
            obj = obj->next;
    return obj;
}
/* 0x438c60, whether an object type is selected by a d3_ForEachObject mask. */

char d3_TypeInMask(int type, unsigned int mask)
{
    switch (type) {
    case 0:
        return (mask & 0x20) == 0x20;
    case 3:
        return (mask & 0x01) == 0x01;
    case 4:
        return (mask & 0x40) == 0x40;
    case 1:
        return (mask & 0x80) == 0x80;
    case 2:
        return (mask & 0x100) == 0x100;
    case 5:
        return (mask & 0x02) == 0x02;
    case 6:
        return (mask & 0x04) == 0x04;
    case 7:
        return (mask & 0x08) == 0x08;
    case 8:
        return (mask & 0x10) == 0x10;
    }
    return 0;
}
/* 0x438d40, calls cb for every object of the masked types below obj (all top-level objects of the
   scene if obj is 0). 0x200 in mask: obj only, not its siblings. cb returns 0 to stop, < 0 to skip
   the children. */

char d3_ForEachObject(D3SCENE *scene, D3OBJECT *obj, unsigned int mask, D3OBJECTCB cb, void *ctx)
{
    D3SCENELISTS *s = (D3SCENELISTS *)scene;
    D3OBJECT *p;
    char r;

    if (!mask)
        return 0;
    if (obj) {
        do {
            r = 1;
            if (d3_TypeInMask(obj->type, mask) && !(r = cb(obj, ctx)))
                return 0;
            if (obj->child && !(r & 0x80))
                r = d3_ForEachObject(scene, obj->child, mask & ~0x200, cb, ctx);
            if (!r)
                return 0;
            if (mask & 0x200)
                return 1;
            obj = obj->next;
            if (!obj)
                return 1;
        } while (!(obj->state & 1));
        return 1;
    }
    if (scene == d3_DefaultScene) {
        s->first = d3_TopFirst;
        s->last = d3_TopLast;
        s->parHead = d3_ParHead;
        s->parTail = d3_ParTail;
    }
    for (p = s->first; p && p != &d3_ObjEnd; p = p->next)
        if (!d3_ForEachObject(scene, p, mask, cb, ctx))
            return 0;
    return 1;
}
/* 0x438e60, the same without a context and without stopping. */

char d3_ForEachObject2(D3SCENE *scene, D3OBJECT *obj, unsigned int mask, void (*cb)(D3OBJECT *obj))
{
    D3SCENELISTS *s = (D3SCENELISTS *)scene;
    D3OBJECT *p;

    if (!mask)
        return 0;
    if (obj) {
        do {
            if (d3_TypeInMask(obj->type, mask))
                cb(obj);
            if (obj->child)
                d3_ForEachObject2(scene, obj->child, mask & ~0x200, cb);
            if (mask & 0x200)
                break;
            obj = obj->next;
            if (!obj)
                break;
        } while (!(obj->state & 1));
    } else {
        if (scene == d3_DefaultScene) {
            s->first = d3_TopFirst;
            s->last = d3_TopLast;
            s->parHead = d3_ParHead;
            s->parTail = d3_ParTail;
        }
        for (p = s->first; p && p != &d3_ObjEnd; p = p->next)
            d3_ForEachObject2(scene, p, mask, cb);
    }
    return 1;
}

/* D3OBJECT 0x5c of the object types that use it as a color (5..7); d3_light.h's D3LOBJ view names
   the same field color. A field (or anonymous union) in D3OBJECT itself flips d3_MirDrawObject. */
typedef struct D3OBJCOLOR {
    char pad000[0x5c];
    D3VECTOR color;
} D3OBJCOLOR;

/* 0x438f60, projects the visible vertices of a mesh to the screen, sets fog and flat colors, and
culls back faces. */
void d3_ProjectMesh(D3OBJECT *obj)
{
    unsigned int n = obj->info->nkeys;
    unsigned int i;
    D3PVERT *v;
    D3PTRI *t;
    D3PVERT *v0, *v1, *v2;
    float rhw;
    unsigned long col;

    if (d3_Hardware) {
        if (d3_FogOn) {
            for (i = 0, v = (D3PVERT *)obj->info->keys; i < n; i++, v++) {
                if (v->flags & 0x80) {
                    rhw = 1.0f / v->z;
                    v->sx = d3_ScaleX * v->x * rhw + d3_CenterX;
                    v->sy = d3_ScaleY * v->y * rhw + d3_CenterY;
                    v->color = v->specular;
                    v->rhw = rhw;
                    d3_Dist2 = (double)v->x * v->x + v->y * v->y + (double)v->z * v->z;
                    v->fog = (unsigned char)(d3_Dist2 > d3_FogStart2
                        ? 255.0 - min(255.0, (sqrt(d3_Dist2) - d3_FogStart) * d3_FogScale) : 255.0);
                }
            }
        } else {
            for (i = 0, v = (D3PVERT *)obj->info->keys; i < n; i++, v++) {
                if (v->flags & 0x80) {
                    rhw = 1.0f / v->z;
                    v->sx = d3_ScaleX * v->x * rhw + d3_CenterX;
                    v->sy = d3_ScaleY * v->y * rhw + d3_CenterY;
                    v->color = v->specular;
                    v->rhw = rhw;
                }
            }
        }
        if (obj->type >= 5) {
            if (obj->type == 8)
                col = 0x1f1fff;
            else
                col = RGB_MAKE((unsigned char)((D3OBJCOLOR *)obj)->color.x,
                               (unsigned char)((D3OBJCOLOR *)obj)->color.y,
                               (unsigned char)((D3OBJCOLOR *)obj)->color.z);
            for (i = 0, v = (D3PVERT *)obj->info->keys; i < n; i++, v++)
                v->color = col;
        }
    } else {
        for (i = 0, v = (D3PVERT *)obj->info->keys; i < n; i++, v++) {
            if (v->flags & 0x80) {
                rhw = 1.0f / v->z;
                v->c.r = v->s.r >> 2;
                v->sx = d3_ScaleX * v->x * rhw + d3_CenterX;
                v->sy = d3_ScaleY * v->y * rhw + d3_CenterY;
                v->rhw = rhw;
            }
        }
    }
    n = obj->info->ntris;
    for (i = 0, t = (D3PTRI *)obj->info->tris; i < n; i++, t++) {
        if (t->flags & 0x80) {
            v0 = t->v[0];
            v1 = t->v[1];
            v2 = t->v[2];
            if (t->flags & 0x10)
                t->flags |= 0x40;
            else if (((double)v0->sx - v1->sx) * ((double)v0->sy - v2->sy) < (v0->sx - v2->sx) * (v0->sy - v1->sy)
                     && !(t->drawFlags & 4))
                t->flags &= 0x7f;
        }
    }
}
/* 0x4391d0, sets up the view frustum planes (d3_Planes), the corner directions (d3_Frustum) and the
   plane lists of the 64 clip codes (d3_ClipInfo).
   @deep s36: the M8 lines of dcp grade on d3_Frustum are alignment artifacts of the 2-instruction gap
   (both store [0..3].x/y/z in the same order), no meaning bug. Tried the note's two ideas: s reused for nb
   (P3.z = s; rot.x = s = -tb; second group rot.x = s): frame 0x5c as the original (no frame hunk) but 84
   ins (regs 40, code 43); rot.x = nb = -b: 37 ins (fpu 56). Left as before: fsin after fcos in both pairs,
   the s / nb slot sharing (frame 0x60 vs 0x5c), the P4 / P5 constant stores. prior: size queue s5: kept
   the two s copies late: d3_Planes[1].z = s after tb = atan2 (s4's p1at) and d3_Planes[3].z = s after
   rot.x / rot.y: 98 -> 24 ins, raw 1141 vs 1145 (padded 1152 = the original's). c2fpu: the original's s
   webs both go to memory because each outlives an atan2 result it does not nest with (s1 vs tb, s2 vs tb
   dying at rot.x = nb = -tb); a P3.z store before nb keeps s2 on the FPU. Left: (1) the frame is 4 bytes
   bigger (0x60 vs 0x5c): ours s now overlaps nb, the original shares s and nb in one slot (the second
   group reads nb from s's slot), so in the original's IL P3.z reads s before nb is defined and s2 is still
   in memory: what spills s2 there is open (all orders of P3.z / rot.x = nb / rot.y = na: 24-88 ins; split
   nb = -tb; rot.x = nb = same code); (2) fsin before fcos in both pairs (s = sin(..) first, sf2 forms:
   80-89 ins without the late P3.z, 25-30 with it); (3) the P4 / P5 constant stores among the loop setup.
   Ideas: nb reusing s's variable, rot.x = nb = -b (tb dying at b).



   */

void d3_SetupFrustum(void)
{
    D3MATRIX m;
    D3VECTOR rot;
    float a, b, na, nb, s, c;
    double ta, tb;
    unsigned int i, bits;
    int n, k;
    D3CLIPINFO *info;

    /* */
    d3_Planes[0].y = 0.0f;
    d3_Planes[0].w = -2e-6f;
    d3_Planes[1].y = 0.0f;
    d3_Planes[1].w = 2e-6f;
    d3_Planes[2].x = 0.0f;
    d3_Planes[2].w = 2e-6f;
    d3_Planes[3].x = 0.0f;
    d3_Planes[3].w = -2e-6f;
    rot.z = 0.0f;
    ta = atan2(d3_HalfW, d3_ScaleX);
    d3_FovX = a = (float)ta;
    d3_Planes[0].x = (c = cos(ta));
    d3_Planes[0].z = (s = sin(ta));
    d3_Planes[1].x = -c;
    tb = atan2(d3_HalfH, -d3_ScaleY);
    d3_Planes[1].z = s;
    d3_FovY = b = (float)tb;
    d3_Planes[2].y = (c = cos(tb));
    d3_Planes[2].z = (s = sin(tb));
    d3_Planes[3].y = -c;
    rot.x = nb = -tb;
    rot.y = na = -ta;
    d3_Planes[3].z = s;
    d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
    d3_Frustum[0].x = 0.0f * m._11 + 0.0f * m._21 + m._31;
    d3_Frustum[0].y = 0.0f * m._12 + 0.0f * m._22 + m._32;
    d3_Frustum[0].z = 0.0f * m._13 + 0.0f * m._23 + m._33;
    rot.x = nb;
    rot.y = a;
    rot.z = 0.0f;
    d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
    d3_Frustum[1].x = 0.0f * m._11 + 0.0f * m._21 + m._31;
    d3_Frustum[1].y = 0.0f * m._12 + 0.0f * m._22 + m._32;
    d3_Frustum[1].z = 0.0f * m._13 + 0.0f * m._23 + m._33;
    rot.x = b;
    rot.y = na;
    rot.z = 0.0f;
    d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
    d3_Frustum[2].x = 0.0f * m._11 + 0.0f * m._21 + m._31;
    d3_Frustum[2].y = 0.0f * m._12 + 0.0f * m._22 + m._32;
    d3_Frustum[2].z = 0.0f * m._13 + 0.0f * m._23 + m._33;
    rot.x = b;
    rot.y = a;
    rot.z = 0.0f;
    d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
    d3_Frustum[3].x = 0.0f * m._11 + 0.0f * m._21 + m._31;
    d3_Frustum[3].y = 0.0f * m._12 + 0.0f * m._22 + m._32;
    d3_Frustum[3].z = 0.0f * m._13 + 0.0f * m._23 + m._33;
    d3_Frustum[4].x = 0.0f;
    d3_Frustum[4].y = 0.0f;
    d3_Frustum[4].z = 1.0f;
    d3_Planes[4].x = 0.0f;
    d3_Planes[4].y = 0.0f;
    d3_Planes[4].z = 1.0f;
    d3_Planes[4].w = d3_ZNear;
    d3_Planes[5].x = 0.0f;
    d3_Planes[5].y = 0.0f;
    d3_Planes[5].z = -1.0f;
    d3_Planes[5].w = -d3_ZFar;
    for (i = 0; i < 64; i++) {
        info = (D3CLIPINFO *)d3_ClipInfo[i];
        n = 0;
        for (bits = i; bits; bits >>= 1)
            if (bits & 1)
                n++;
        info->n = n;
        info->mirror = 0;
        k = 0;
        if (i & 1) {
            info->plane[k].x = d3_Planes[0].x;
            info->plane[k].y = d3_Planes[0].y;
            info->plane[k].z = d3_Planes[0].z;
            info->plane[k].w = d3_Planes[0].w;
            k++;
        }
        if (i & 2) {
            info->plane[k].x = d3_Planes[1].x;
            info->plane[k].y = d3_Planes[1].y;
            info->plane[k].z = d3_Planes[1].z;
            info->plane[k].w = d3_Planes[1].w;
            k++;
        }
        if (i & 4) {
            info->plane[k].x = d3_Planes[2].x;
            info->plane[k].y = d3_Planes[2].y;
            info->plane[k].z = d3_Planes[2].z;
            info->plane[k].w = d3_Planes[2].w;
            k++;
        }
        if (i & 8) {
            info->plane[k].x = d3_Planes[3].x;
            info->plane[k].y = d3_Planes[3].y;
            info->plane[k].z = d3_Planes[3].z;
            info->plane[k].w = d3_Planes[3].w;
            k++;
        }
        if (i & 0x10) {
            info->plane[k].x = 0.0f;
            info->plane[k].y = 0.0f;
            info->plane[k].z = 1.0f;
            info->plane[k].w = d3_ZNear;
            k++;
        }
        if (i & 0x20) {
            info->plane[k].x = 0.0f;
            info->plane[k].y = 0.0f;
            info->plane[k].z = -1.0f;
            info->plane[k].w = d3_Planes[5].w;
            k++;
        }
    }
}
/* 0x439650, clips the 8 corners of a box (camera space) against the view frustum; returns the
   combined clip code and the box's z range. */

unsigned char d3_ClipBox(void *box, float *zmin, float *zmax)
{
    D3CLIP any, all, c;
    D3BOXVERT *v;
    float mn, mx;
    int i;

    any.raw = 0;
    all.raw = 0x3f;
    for (i = 0, v = box; i < 8; i++, v++) {
        c.raw = 0;
        c.left = d3_Planes[0].x * v->x + d3_Planes[0].y * v->y + d3_Planes[0].z * v->z < d3_Planes[0].w;
        c.right = d3_Planes[1].x * v->x + d3_Planes[1].y * v->y + d3_Planes[1].z * v->z < d3_Planes[1].w;
        c.top = d3_Planes[2].x * v->x + d3_Planes[2].y * v->y + d3_Planes[2].z * v->z < d3_Planes[2].w;
        c.bottom = d3_Planes[3].x * v->x + d3_Planes[3].y * v->y + d3_Planes[3].z * v->z < d3_Planes[3].w;
        c.znear = v->z < d3_ZNear;
        c.zfar = v->z > d3_ZFar;
        any.raw |= c.raw;
        all.raw &= c.raw;
    }
    any.out = all.raw != 0;
    if (!any.out) {
        mx = -1e10f;
        mn = 1e10f;
        for (i = 0, v = box; i < 8; i++, v++) {
            mn = min(mn, v->z);
            mx = max(mx, v->z);
        }
        if (zmin) {
            *zmin = mn;
            d3_MinZ = min(d3_MinZ, mn);
        }
        if (zmax) {
            *zmax = mx;
            d3_MaxZ = max(d3_MaxZ, mx);
        }
    }
    return any.raw;
}
/* 0x439870, updates the object's world matrix (mesh->worldMatrix) and its matrix relative to the
   camera (mesh->cameraMatrix) unless they are current. */

void d3_UpdateWorldMatrix(D3OBJECT *obj, D3OBJECT *camera, char force)
{
    D3MATRIX m;

    if (!(obj->stateFlags & 0x80) || force) {
        d3_WorldMatrix(obj, 0, 0, D3_MESH(obj)->worldMatrix);
        obj->stateFlags |= 0x80;
    }
    if (!(obj->flags & 1) || force || D3_MESH(obj)->camera != camera) {
        if (camera && !(obj->state & 0x80)) {
            D3E_COPY3(&m, D3_MESH(obj)->worldMatrix, 0x40);
            m._41 -= camera->pos.x;
            m._42 -= camera->pos.y;
            m._43 -= camera->pos.z;
            m._41 -= camera->posOffset.x;
            m._42 -= camera->posOffset.y;
            m._43 -= camera->posOffset.z;
            d3_MatMul43(&m, &camera->matrix, (D3MATRIX *)D3_MESH(obj)->cameraMatrix);
        } else
            D3E_COPY3(D3_MESH(obj)->cameraMatrix, D3_MESH(obj)->worldMatrix, 0x40);
        D3_MESH(obj)->camera = camera;
        obj->flags |= 1;
    }
}
/* 0x4399e0, the object's clip code: clips its bounding box against the view frustum (0x40 = outside)
   and stores the box's z range in the mesh (zMin / zMax). */

unsigned char d3_ClipObject(D3OBJECT *obj, D3OBJINFO *info, D3OBJECT *camera)
{
    unsigned char clip;
    void *box;

    d3_UpdateWorldMatrix(obj, camera, 0);
    if (obj->objectFlags & 0x20) {
        clip &= ~0x40;
        clip |= 0x3f;
        return clip;
    }
    box = 0;
    if (info && info->mesh)
        box = D3_OPS(info)->GetBox(obj, info, D3_MESH(obj)->cameraMatrix);
    if (!box) {
        clip |= 0x40;
        return clip;
    }
    return d3_ClipBox(box, &D3_MESH(obj)->zMin, &D3_MESH(obj)->zMax);
}
/* 0x439a60, clips the vertices of a mesh against the planes selected in mask and marks the triangles
   (and their vertices) that are not completely outside one plane as visible. */

void d3_ClipMesh(D3CLIP mask, D3PVERT *v, int nverts, D3PTRI *t, int ntris)
{
    D3CLIP *c;
    D3PVERT *v0, *v1, *v2;
    int i;

    for (i = 0; i < nverts; i++) {
        c = &v[i].clip;
        c->raw = 0;
        if (mask.left)
            c->left = d3_Planes[0].x * v[i].x + d3_Planes[0].z * v[i].z < d3_Planes[0].w;
        if (mask.right)
            c->right = d3_Planes[1].x * v[i].x + d3_Planes[1].z * v[i].z < d3_Planes[1].w;
        if (mask.top)
            c->top = d3_Planes[2].z * v[i].z + d3_Planes[2].y * v[i].y < d3_Planes[2].w;
        if (mask.bottom)
            c->bottom = d3_Planes[3].z * v[i].z + d3_Planes[3].y * v[i].y < d3_Planes[3].w;
        if (mask.znear)
            c->znear = v[i].z < d3_ZNear;
        if (mask.zfar)
            c->zfar = v[i].z > d3_ZFar;
    }
    for (i = 0; i < ntris; i++) {
        t[i].flags = 0;
        if (t[i].v[0] && !(t[i].drawFlags & 2)) {
            v0 = t[i].v[0];
            v1 = t[i].v[1];
            v2 = t[i].v[2];
            if (!((unsigned char)v0->flags & (unsigned char)v1->flags & (unsigned char)v2->flags & 0x3f)) {
                t[i].flags = ((unsigned char)v0->flags | (unsigned char)v1->flags | (unsigned char)v2->flags) & 0x3f | 0x80;
                v0->rawFlags |= 0x80;
                v1->rawFlags |= 0x80;
                v2->rawFlags |= 0x80;
            }
        }
    }
}

__inline void d3_FtoI2(float f, int *dst)
{
    __asm fld f
    __asm mov edx, dst
    __asm fistp dword ptr [edx]
}

/* 0x439c90, clips the polygon in d3_ClipList[0] (d3_ClipIn vertices) at the planes of info
(Sutherland-Hodgman); returns the vertex list of the result, d3_ClipOut vertices. @deep deep s44: 12 -> 6 ins. The fog / &c order hunk was the 81-instruction region cut of the intersection block (c2dag block 163 ended at the col[3] delta): parenthesised products in the four pool stores, pool->x = (d.x * t) + a->x .. pool->u (m15 of the 127 subsets of the pool / t / d1 parens), move the cut so the lea &c and the fild of the fog delta fall as in the original. Left: (1) one fstp [esp+0x44] (d.w) one instruction later than the original (parens on the FtoI2 arguments / df / pool->v on top: = this); (2) the d1 sum: the original computes plane.x * b->x first and then loads b->z (fld [edx+8]; fmul [ebx+4]); ours computes the parenthesised (b->z * plane.z) first with plane.z loaded first. 144 spellings of d1 (product order x operand order x grouping) give 7 codes, none the original's; operand order inside a product never changes the code; (px*bx + bz*pz) + by*py moves the strength-reduced plane pointer's base from .y to .x (the s43 last-field finding). Also tried: (float) casts on the deltas (= control), d3_FtoI(&c, f) (slot swap), the 120 orders of the delta lines, colour deltas in their own vector / array, df / c block-scoped; climb 3 rounds before the parens: nothing. prior: deep s23: left the d1 plane sum's first product (the original multiplies plane.z by b->z after the x product: fld [edx+8]; fmul [ebx+4]) and the fild of b->fog - a->fog scheduled before the byte loads. climb round 1 (241 mutants): nothing. */
D3CVERT **d3_ClipPoly(D3CLIPINFO *info)
{
    D3CVERT **in, **out;
    D3CVERT *a, *b;
    D3CVERT *pool;
    unsigned int i, j;
    int ins, ins1;
    float d0, d1, t;
    D3VECTOR4 d;
    float df;
    int c;

    d3_ClipOut = 0;
    if (!((int)info->plane | info->n | (int)info))
        return 0;
    pool = d3_ClipPool;
    for (j = 0; j < info->n; j++) {
        d3_ClipOut = 0;
        in = d3_ClipList[j & 1];
        a = in[0];
        d0 = a->z * info->plane[j].z + a->y * info->plane[j].y + a->x * info->plane[j].x;
        in[d3_ClipIn] = a;
        out = d3_ClipList[(j - 1) & 1];
        ins = 1;
        if (!(d0 >= info->plane[j].w))
            ins = 0;
        for (i = 0; i < d3_ClipIn; i++, in++) {
            a = in[0];
            b = in[1];
            d1 = info->plane[j].x * b->x + (b->z * info->plane[j].z) + b->y * info->plane[j].y;
            ins1 = d1 >= info->plane[j].w;
            if (ins) {
                *out++ = a;
                d3_ClipOut++;
            }
            if (ins != ins1) {
                t = (info->plane[j].w - d0) / (d1 - d0);
                d.x = b->x - a->x;
                d.y = b->y - a->y;
                d.z = b->z - a->z;
                d.w = b->u - a->u;
                pool->x = (d.x * t) + a->x;
                pool->y = (d.y * t) + a->y;
                pool->z = (d.z * t) + a->z;
                pool->u = (d.w * t) + a->u;
                pool->v = (b->v - a->v) * t + a->v;
                d.x = (float)(b->col[0] - a->col[0]);
                d.y = (float)(b->col[1] - a->col[1]);
                d.z = (float)(b->col[2] - a->col[2]);
                d.w = (float)(b->col[3] - a->col[3]);
                df = (float)(b->fog - a->fog);
                d3_FtoI2(d.x * t + a->col[0], &c);
                pool->col[0] = (unsigned char)c;
                d3_FtoI2(d.y * t + a->col[1], &c);
                pool->col[1] = (unsigned char)c;
                d3_FtoI2(d.z * t + a->col[2], &c);
                pool->col[2] = (unsigned char)c;
                d3_FtoI2(d.w * t + a->col[3], &c);
                pool->col[3] = (unsigned char)c;
                d3_FtoI2(df * t + a->fog, &c);
                pool->fog = (unsigned char)c;
                *out++ = pool;
                d3_ClipOut++;
                pool++;
            }
            ins = ins1;
            d0 = d1;
        }
        d3_ClipIn = d3_ClipOut;
    }
    return d3_ClipList[j & 1];
}
/* 0x43a000, the surface (level of detail) to draw: from the distance to the camera, or fixed by
   flags bits 4-5. */

D3OBJINFO *d3_SelectSurf(D3OBJECT *obj)
{
    D3SURF *s = 0;
    D3MESH *m = D3_MESH(obj);
    D3VECTOR4 d;
    float f, fn;
    int n, lod;

    if (m && m->nsurf > 0) {
        if (!(obj->flags & 0x30) && d3_Camera) {
            d.x = obj->pos.x;
            d.y = obj->pos.y;
            d.z = obj->pos.z;
            d.x -= d3_Camera->pos.x;
            d.y -= d3_Camera->pos.y;
            d.z -= d3_Camera->pos.z;
            n = D3_MESH(obj)->nsurf;
            fn = (float)n;
            f = ((float)sqrt(d.x * d.x + d.y * d.y + d.z * d.z) * fn) * d3_InvZFar;
            if ((int)f >= n)
                f = fn - 1.0f;
            else if (f < 0.0f)
                f = 0.0f;
            s = &m->surf[(int)f];
        } else {
            lod = ((obj->flags & 0x30) >> 4) - 1;
            s = &m->surf[max(0, min(m->nsurf - 1, lod))];
        }
        if (!s->info.nkeys || !s->info.ntris)
            s = 0;
        else if ((D3OBJINFO *)s != obj->info || d3_LightsChanged)
            obj->state |= 0x40;
    }
    return (D3OBJINFO *)s;
}
/* 0x43a160, d3_ForEachObject callback: draws a visible mesh object. Counts it, picks its surf (d3_SelectSurf),
   takes over its dirty bit and clips its box anew when it changed (a mesh already clipped this frame counts as
   culled). A culled object only updates its shadow and is relit within the culled key budget. A visible one is
   transformed (through its stock driver or d3_TransformVerts), clipped and projected when it changed, has a
   shadow or is always drawn, is relit within the visible key budget, opens the mirror pass, and puts its visible
   triangles into the sort buffer: keyed by texture with Direct3D (alpha / shadow / flagged surfs after the
   opaque ones), by the far depth in software. Returns 1. */

char d3_DrawObjectCB(D3OBJECT *obj)
{
    D3SURF *s;
    D3MESH *m;
    unsigned char dirty;
    D3CLIP clip;
    int base;
    int n;
    D3PTRI *t;
    D3TEXTURE *tex;
    float z;

    d3_ObjectsTested++;
    s = (D3SURF *)d3_SelectSurf(obj);
    if (s) {
        m = D3_MESH(obj);
        d3_TriCount += s->info.ntris;
        d3_VertCount += s->info.nkeys;
        obj->bits.b4 = 0;
        dirty = (obj->bits.dirty);
        obj->bits.drawDirty = dirty;
        obj->bits.dirty = 0;
        if (m->frameStamp == d3_FrameCount)
            m->flags |= 0x40;
        else if (dirty || obj->bits.reclip)
            m->flags = d3_ClipObject(obj, &s->info, d3_Camera);
        clip.raw = (m->flags);
        if (!(clip.raw & 0x40)) {
            d3_DrawnVerts += s->info.nkeys;
            d3_ObjectsDrawn++;
            base = 1;
            if (obj->bits.reclip) {
                obj->info = &s->info;
                dirty = 1;
                obj->bits.dirty = dirty;
                if (obj->type == 4 && obj != d3s_Object && (D3_MESH(obj)->lightGeneration < d3_LightGeneration || d3_LightsChanged))
                    d3_RelightObject(obj);
                obj->bits.reclip = 0;
            } else if (!obj->bits.dirty) {
                if (!(m->zMin > d3_MinZ))
                    d3_MinZ = (m->zMin);
                if (!(m->zMax < d3_MaxZ))
                    d3_MaxZ = (m->zMax);
            }
            if (obj->objectFlags >> 2 & d3_Hardware & d3_ShadowMode.mode & 1) {
                d3s_Count = 0;
                d3s_UpdateObject(obj);
                if (d3s_Count > 0)
                    goto draw;
            }
            if (dirty || s->alwaysDraw) {
    draw:
                if (s->info.mesh)
                    D3_OPS(&s->info)->Transform(obj, &s->info, m->cameraMatrix);
                else
                    d3_TransformVerts(obj, &s->info, m->cameraMatrix);
                d3_ClipMesh(clip, (D3PVERT *)s->info.keys, s->info.nkeys, (D3PTRI *)s->info.tris, s->info.ntris);
                d3_ProjectMesh(obj);
            }
            if (obj != d3s_Object && obj->type == 4 && m->lightGeneration < d3_LightGeneration && d3_LightKeysVisible < d3_LightKeyBudgetVisible) {
                d3_LightKeysVisible += s->info.nkeys;
                d3_RelightObject(obj);
            }
            d3_MirBeginFrame(obj, &d3_Mirror);
            if (obj == d3s_Object || s->flags.f.b[0] < 0xff || (s->flags.f.b[2] & 1))
                base = 0x4000;
            n = min(s->info.ntris, d3_Cfg.maxpolys - (int)d3_SortCount);
            if (d3_Hardware) {
                for (t = (D3PTRI *)s->info.tris; n > 0; n--, t++) {
                    if (t->flags & 0x80) {
                        tex = (t->tex);
                        d3_SortPtr->item = t;
                        if (tex) {
                            d3_SortPtr->key = (tex - (D3TEXTURE *)d3_Textures) + base;
                            tex->stamp = d3_FrameCount;
                        } else
                            d3_SortPtr->key = 0;
                        t->clip = d3_ClipInfo[t->flags & 0x3f];
                        d3_SortPtr++;
                        d3_SortCount++;
                    }
                }
            } else {
                for (t = (D3PTRI *)s->info.tris; n > 0; n--, t++) {
                    if (t->flags & 0x80) {
                        d3_SortPtr->item = t;
                        z = (t->v[0]->z);
                        if (z < t->v[1]->z)
                            z = (t->v[1]->z);
                        if (z < t->v[2]->z)
                            z = (t->v[2]->z);
                        d3_FtoI((int *)&d3_SortPtr->key, (d3_InvZFar * 16777216.0f) * z * 3.0f);
                        if (t->tex)
                            t->tex->stamp = d3_FrameCount;
                        t->clip = d3_ClipInfo[t->flags & 0x3f];
                        d3_SortPtr++;
                        d3_SortCount++;
                    }
                }
            }
        } else {
            if ((obj->flags >> 1 | dirty) & obj->objectFlags >> 2 & d3_ShadowMode.mode & d3_Hardware & 1) {
                d3s_Count = 0;
                if (dirty)
                    d3s_UpdateObjectAt(obj);
                else
                    d3s_UpdateObject(obj);
            }
            if (obj != d3s_Object && obj->type == 4 && m->lightGeneration < d3_LightGeneration && d3_LightKeysCulled < d3_LightKeyBudgetCulled) {
                d3_LightKeysCulled += s->info.nkeys;
                obj->bits.dirty = 1;
                d3_RelightObject(obj);
            }
        }
    }
    return 1;
}
/* 0x43a610, sets the fog range: the fog scale (255 / (end - start)), the squared start, and when the
   fog is off, shrinks the far plane to the fog end; then refreshes all objects.
   d3_ScreenH / d3_ScreenW (0xfc2df0, 0xfc2df4) are the int screen height / width d3_SetViewport
   (0x43b950) last took (every caller passes d2_ScreenW, d2_ScreenH or d3_Cfg.w, d3_Cfg.h); they are
   stored only for its change check, so this function passes them back unchanged */

void d3_SetFogRange(float start, float end)
{
    float zf;

    if (d3_FogStart != start || d3_FogEnd != end) {
        d3_FogStart = start;
        d3_FogScale = 255.0f / (end - start);
        d3_FogEnd = end;
        d3_FogStart2 = start * start;
        if (d3_FogOn)
            zf = d3_SavedZFar;
        else
            zf = d3_SavedZFar = d3_ZFar;
        if (end < zf)
            d3_SetViewport(d3_ViewW, d3_ViewH, d3_ViewX, d3_ViewY, d3_ZNear, end, d3_ScaleX, d3_ScreenW, d3_ScreenH);
        d3_UpdateAll(0);
    }
}
/* 0x43a6f0, turns the fog on (start < end: sets the range through d3_SetFogRange) or off (start >= end:
   restores the viewport when the fog was on), then stores the fog range, the on flag and the colour
   @deep deep s35: no new lever found; read again: the M8 lines of the d3_SetViewport call and the tail
   stores are the register rotation behind the missing tail duplicate (the original's pushes and stores
   name the same globals in the same order). Rounds of the s13 note not repeated. prior: s13 (size queue):
   the then-arm copy of the tail is NOT a post-allocation duplicate: its registers differ from the shared
   tail's (copy: eax=end, edx=start, eax=color; tail: ecx=start, edx=end, eax=color) and it keeps on in bl,
   so VC6 duplicated the 4-store tail into the start<end arm BEFORE allocation (as the single-return tail
   rule of main_ChooseHistory) and scheduled it with the add esp,8. Tried s13 (dcp try): on = 0 at the top
   (30 ins), on = 0; if (start < end) on = 1; if (on) .. else if (FogOn) .. (24), on = start < end + if
   (on) / else if, an empty if (!FogOn) {} else, guards as two early returns, the outer && as nested ifs:
   all = control. flip.py --want two pop ebx (every single delete / duplicate / swap / constant): no mutant
   duplicates the tail. Open: what makes the original's tail cheap enough for the pre-allocation duplicator
   (its IL size or predecessor count).














   */

void d3_SetFog(float start, float end, D3COLOR color)
{
    char on;

    if (d3_Hardware && d3d_UsedCard.info.fogvertex) {
        if (start < end) {
            on = 1;
            d3_SetFogRange(start, end);
        } else {
            on = 0;
            if (d3_FogOn)
                d3_SetViewport(d3_ViewW, d3_ViewH, d3_ViewX, d3_ViewY, d3_ZNear, d3_SavedZFar, d3_ScaleX, d3_ScreenW, d3_ScreenH);
        }
        d3_FogOn = on;
        d3_FogStart1 = start;
        d3_FogEnd2 = end;
        d3_fog_color.c = color;
    }
}
/* 0x43a7d0, Direct3D rendering: draws the sorted triangles in texture order through the vertex buffer,
   clipping the ones that cross a frustum plane into fans.
   @deep match-deep (dcp try, nothing kept; tiebreak sample: insensitive, 32 samples one codegen). Read:
   the original keeps the constant 0 in edi inside the texture-switch block (n = 0, the cur tests, push edi
   for SetTexture's stage, the fild high dword: cmp eax,edi where ours test eax,eax / push 0) and edi = cur
   at the loop top (mov edi,[ebp-0x18]; cmp edi,ecx), t in esi with esi pushed only around the loop (push
   esi after the d3_SortCount > 0 test, pop before the last d3d_DrawTriangles: VC6 shrink-wraps esi because
   nothing outside the loop uses it), p in ebx with home [ebp-0x14]; ours puts the zero in esi before the
   loop (so esi is pushed in the prologue), t in ebx, p in ecx / [ebp-0x10]. The texture size estimate in
   the original loads cur->bytes and cur->w before the flags & 8 test and multiplies in each arm. Not
   moved: cur != t->tex operand order, the init order of wrap / oldtext / cur, n before p, the five other
   orders of the flag OR (all = control); unsigned SortCount compare 505, the bytes estimate as a ternary
   507 or with the negated test 509. Left: allocation throughout (code 484, slots 82, regs 22, fpu 15),
   ours 678 vs 673 instructions, the 4-byte body shift from the late push esi. (tiebreak: insensitive)
   prior: read the whole function against the original 0x43a7d0..0x43b073 and fixed the meaning: the
   texture size estimate (D3TEXV.w unsigned: shr and the fild qword of an unsigned product; the factor
   0.16666667f, not 1.3333334f), d3_SetBlendState called with its unsigned char prototype (the original's
   setb al / shr dl,1 byte forms; the int view macro is gone), D3PTRI 0x18/0x1c/0x20 typed as the float
   zofs/uofs/vofs (no *(float *) casts), D3TEXV.blend, renamed d3_NoTexturesByte / d3_TextureBytes. Every
   statement lines up (the texture switch, the scroll offsets, the 3 unclipped TLV paths, the clip list,
   the projection loop, the fan with its winding test, the flush); ours 678 vs 673. */

void d3_DrawSortedD3D(void)
{
    char wrap;
    int max;
    D3TLV *vb;
    unsigned int n;
    D3TLV *p;
    D3TEXV *cur;
    float *uv;
    int h;
    float zmin;
    D3PTRI *t;
    int i;
    struct IDirect3DTexture2 *oldtext;
    D3CLIPINFO *clip;
    D3CVERT **vl;
    D3CVERT *q;
    unsigned int k;
    float rhw;
    D3TLV *a, *b;

    wrap = 0;
    oldtext = 0;
    cur = 0;
    d3_TextureBytes = 0;
    d3_TextureSwitches = 0;
    if (d3_MinZ > 0.0f)
        zmin = d3_MinZ;
    else
        zmin = 0.0f;
    d3_ZScale = 1.0f / (d3_MaxZ + 50.0f - zmin);
    d3_BeginScene(d3_FogOn, d3_fog_color.col, 1);
    h = d3d_GetVertexBuffer(&vb, &max);
    max -= 0x80;
    p = vb;
    n = 0;
    for (i = 0; i < (int)d3_SortCount; i++) {
        t = d3_SortBuf[i].item;
        uv = t->src;
        clip = t->clip;
        if ((D3TEXV *)t->tex != cur) {
            d3d_DrawTriangles((void *)h, vb, n);
            h = d3d_GetVertexBuffer(&vb, &max);
            max -= 0x80;
            p = vb;
            n = 0;
            cur = (D3TEXV *)t->tex;
            if (!d3_NoTextures && cur && cur->d3dtex != oldtext) {
                oldtext = cur->d3dtex;
#line 1219 "D:\\develop\\Units\\d3\\d3_engine.c"
                D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, oldtext));
                d3_TextureSwitches++;
                if (cur->flags & 8)
                    d3_TextureBytes += (cur->bytes * cur->w * cur->w) >> 3;
                else
                    d3_TextureBytes += (int)(cur->bytes * cur->w * cur->w * 0.16666667f);
            }
            if (cur) {
                wrap = cur->flags & 1;
                d3_SetBlendState(cur->alpha < 0xff, cur->blend & 1, (cur->blend >> 1) & 1);
            }
        }
        if (cur) {
            d3_UOfs = ts_ScrollV[cur->uofs & 0x1f] + t->uofs;
            d3_VOfs = ts_ScrollU[cur->vofs & 0x1f] + t->vofs;
        } else {
            d3_VOfs = 0.0f;
            d3_UOfs = 0.0f;
        }
        d3_ZOfs = t->zofs - zmin;
        if (((D3_CV(t->v[0])->flags | D3_CV(t->v[1])->flags | D3_CV(t->v[2])->flags) & 0x3f) + clip->mirror == 0) {
            if (wrap) {
                D3E_TLV(p++, t->v[0], &D3_CV(t->v[0])->tu);
                D3E_TLV(p++, t->v[1], &D3_CV(t->v[1])->tu);
                D3E_TLV(p++, t->v[2], &D3_CV(t->v[2])->tu);
                n += 3;
            } else {
                D3E_TLV(p++, t->v[0], uv);
                D3E_TLV(p++, t->v[1], uv + 2);
                D3E_TLV(p++, t->v[2], uv + 4);
                n += 3;
            }
        } else {
            d3_ClipList[0][0] = D3_CV(t->v[0]);
            d3_ClipList[0][1] = D3_CV(t->v[1]);
            d3_ClipList[0][2] = D3_CV(t->v[2]);
            if (wrap) {
                D3_CV(t->v[0])->u = d3_UOfs + D3_CV(t->v[0])->tu;
                D3_CV(t->v[0])->v = d3_VOfs + D3_CV(t->v[0])->tv;
                D3_CV(t->v[1])->u = d3_UOfs + D3_CV(t->v[1])->tu;
                D3_CV(t->v[1])->v = d3_VOfs + D3_CV(t->v[1])->tv;
                D3_CV(t->v[2])->u = d3_UOfs + D3_CV(t->v[2])->tu;
                D3_CV(t->v[2])->v = d3_VOfs + D3_CV(t->v[2])->tv;
            } else {
                D3_CV(t->v[0])->u = d3_UOfs + uv[0];
                D3_CV(t->v[0])->v = d3_VOfs + uv[1];
                D3_CV(t->v[1])->u = d3_UOfs + uv[2];
                D3_CV(t->v[1])->v = d3_VOfs + uv[3];
                D3_CV(t->v[2])->u = d3_UOfs + uv[4];
                D3_CV(t->v[2])->v = d3_VOfs + uv[5];
            }
            d3_ClipIn = 3;
            vl = d3_ClipPoly(clip);
            if (d3_ClipOut >= 3) {
                D3TLV tmp;

                d3_ClipExtraTris += d3_ClipOut - 3;
                for (k = 0; k < d3_ClipOut; k++) {
                    q = vl[k];
                    if (!(q->flags & 0x80)) {
                        rhw = 1.0f / q->z;
                        q->sx = d3_ScaleX * q->x * rhw + d3_CenterX;
                        q->sy = d3_ScaleY * q->y * rhw + d3_CenterY;
                    }
                }
                q = vl[0];
                D3E_TLV2(&tmp, q, &q->u, &q->v);
                for (k = 1; k < d3_ClipOut - 1; k++) {
                    D3E_COPY3(p++, &tmp, sizeof(tmp));
                    q = vl[k];
                    a = p;
                    D3E_TLV2(p++, q, &q->u, &q->v);
                    q = vl[k + 1];
                    b = p;
                    D3E_TLV2(p++, q, &q->u, &q->v);
                    n += 3;
                    if (((t->flags >> 6) & 1) - ((t->drawFlags >> 2) & 1) - clip->mirror == 1
                        && (tmp.sy - b->sy) * (tmp.sx - a->sx) < (tmp.sy - a->sy) * (tmp.sx - b->sx)) {
                        p -= 3;
                        n -= 3;
                    }
                }
            }
        }
        if (n >= (unsigned int)max) {
            d3d_DrawTriangles((void *)h, vb, n);
            h = d3d_GetVertexBuffer(&vb, &max);
            max -= 0x80;
            p = vb;
            n = 0;
        }
    }
    d3d_DrawTriangles((void *)h, vb, n);
    d3_EndScene();
}
/* 0x43b080, software rendering: draws the sorted triangles back to front, clipping the ones that cross
   a frustum plane into fans; a triangle whose clip info is the mirror outline's (D3CLIPINFO.mirror) always
   goes through d3_ClipPoly and its winding test is flipped. */

void d3_DrawSortedSW(void)
{
    int i;
    int mode;
    D3PTRI *t;
    D3CLIPINFO *clip;
    float *uv;
    D3TEXTURE *tex;
    unsigned char wrap;
    D3CVERT **v;
    unsigned int k;
    float rhw;
    D3PTRI tri;
    float tuv[14];
    D3CVERT *p;

    d3_TextureSwitches = 0;
    dd_LockNoWait();
    for (i = d3_SortCount - 1; i >= 0; i--) {
        mode = 4;
        t = d3_SortBuf[i].item;
        wrap = 0;
        tex = t->tex;
        clip = t->clip;
        uv = t->src;
        if (tex) {
            if (D3_TEXB(tex, 0x6c) - (D3_TEXB(tex, 0x6e) & 1) < 0xff)
                mode = 3;
            wrap = D3_TEXB(tex, 0x68) & 1;
        }
        p = D3_CV(t->v[0]);
        if (!((p->flags | D3_CV(t->v[1])->flags | D3_CV(t->v[2])->flags) & 0x3f) && !clip->mirror) {
            if (wrap) {
                ((D3UV *)uv)[0] = D3_CV(t->v[0])->uv;
                ((D3UV *)uv)[1] = D3_CV(t->v[1])->uv;
                p = D3_CV(t->v[2]);
                uv[4] = p->tu;
                uv[5] = p->tv;
            }
            d3_TriFuncs[mode](t);
        } else {
            d3_ClipList[0][0] = D3_CV(t->v[0]);
            d3_ClipList[0][1] = D3_CV(t->v[1]);
            d3_ClipList[0][2] = D3_CV(t->v[2]);
            if (wrap) {
                p = D3_CV(t->v[0]);
                p->u = p->tu;
                p->v = p->tv;
                p = D3_CV(t->v[1]);
                p->u = p->tu;
                p->v = p->tv;
                p = D3_CV(t->v[2]);
                p->u = p->tu;
                p->v = p->tv;
            } else {
                p = D3_CV(t->v[0]);
                p->u = uv[0];
                p->v = uv[1];
                p = D3_CV(t->v[1]);
                p->u = uv[2];
                p->v = uv[3];
                p = D3_CV(t->v[2]);
                p->u = uv[4];
                p->v = uv[5];
            }
            d3_ClipIn = 3;
            v = d3_ClipPoly(clip);
            if (d3_ClipOut >= 3) {
                d3_ClipExtraTris += d3_ClipOut - 3;
                for (k = 0; k < d3_ClipOut; k++) {
                    p = v[k];
                    if (!(p->flags & 0x80)) {
                        rhw = 1.0f / p->z;
                        p->sx = d3_ScaleX * rhw * p->x + d3_CenterX;
                        p->sy = d3_ScaleY * p->y * rhw + d3_CenterY;
                    }
                }
                D3_COPY(&tri, t, sizeof(tri));
                p = v[0];
                tri.v[0] = (D3PVERT *)p;
                tri.src = tuv;
                tuv[0] = p->u;
                tuv[1] = p->v;
                for (k = 0; k < d3_ClipOut - 2; k++) {
                    p = v[k + 1];
                    tri.v[1] = (D3PVERT *)p;
                    tuv[2] = p->u;
                    tuv[3] = p->v;
                    p = v[k + 2];
                    tri.v[2] = (D3PVERT *)p;
                    tuv[4] = p->u;
                    tuv[5] = p->v;
                    d3_TriFuncs[mode](&tri);
                }
            }
        }
    }
    dd_Unlock();
}
/* 0x43b300, sorts the n entries of d3_SortBuf by key (radix sort on the low two or all four bytes,
   ping-ponging through d3_SortTmp). */

void d3_RadixSort(unsigned int n, unsigned char twobytes)
{
    if (n > 1) {
        d3_RadixSrc = d3_SortBuf;
        d3_RadixDst = d3_SortTmp;
        D3_ZERO_AT(offset d3_RadixCount, 0x400);
        D3E_RADIX(n, 0);
        d3_RadixSrc = d3_SortTmp;
        d3_RadixDst = d3_SortBuf;
        D3_ZERO_AT(offset d3_RadixCount, 0x400);
        D3E_RADIX(n, 1);
        if (!twobytes) {
            d3_RadixSrc = d3_SortBuf;
            d3_RadixDst = d3_SortTmp;
            D3_ZERO_AT(offset d3_RadixCount, 0x400);
            D3E_RADIX(n, 2);
            d3_RadixSrc = d3_SortTmp;
            d3_RadixDst = d3_SortBuf;
            D3_ZERO_AT(offset d3_RadixCount, 0x400);
            D3E_RADIX(n, 3);
        }
    }
}
/* 0x43b600, draws the sort buffer: sorts it back to front, calls the D3D or the software draw and
   resets the frame counters (sort buffer, triangle / vertex counts). */

void d3_DrawSorted(void)
{
    d3_ClipExtraTris = 0;
    if (d3_Hardware) {
        d3_RadixSort(d3_SortCount, 1);
        d3_DrawSortedD3D();
    } else {
        d3_RadixSort(d3_SortCount, 0);
        d3_DrawSortedSW();
    }
    d3_SortCount = 0;
    d3_TriCount = 0;
    d3_VertCount = 0;
    d3_DrawnVerts = 0;
    d3_SortPtr = d3_SortBuf;
}
/* 0x43b670, d3_ForEachObject callback of d3_UpdateObject: marks an object for an update. */

char d3_MarkDirtyCB(D3OBJECT *obj, unsigned char full)
{
    obj->state |= 4;
    if (full)
        obj->stateFlags &= 0x7f;
    obj->flags &= 0xfe;
    return 1;
}
/* 0x43b6b0, marks obj and its group in the default scene for an update (d3_MarkDirtyCB: state
   bit 2, and with full also the world matrix bit). */

void d3_UpdateObject(D3OBJECT *obj, unsigned char full)
{
    d3_ForEachObject(d3_DefaultScene, obj, 0x1ff, (D3OBJECTCB)d3_MarkDirtyCB, (void *)full);
}
/* 0x43b6e0, marks all objects of the default scene for an update. */

void d3_UpdateAll(unsigned char full)
{
    d3_ForEachObject(d3_DefaultScene, 0, 0x1ff, (D3OBJECTCB)d3_MarkDirtyCB, (void *)full);
    if (d3_Sky)
        d3sky_Init(d3_Sky, 0);
    if (d3_Terrain)
        ((unsigned char *)d3_Terrain)[0x1c70] |= 1;
    if (d3s_Object && d3s_Object->material) {
        D3_MESH(d3s_Object)->surf[0].info.ntris = 0;
        D3_MESH(d3s_Object)->surf[0].info.nkeys = 0;
        d3s_Object->state |= 4;
        d3s_Object->flags &= 0xfe;
        d3s_NumUVs = 0;
        d3s_NumVerts = 0;
    }
}
/* 0x43b790, moves an object to pos, redoing its transform (the whole scene for the camera) and its shadows */

void d3_SetObjectPos(D3OBJECT *obj, D3VECTOR *pos)
{
    if (obj == d3_Camera)
        d3_UpdateAll(1);
    else
        d3_UpdateObject(obj, 1);
    d3_ForEachObject2(d3_DefaultScene, obj, 0xc0, d3s_ResetShadows);
    obj->pos.x = pos->x;
    obj->pos.y = pos->y;
    obj->pos.z = pos->z;
}
/* 0x43b7f0, sets the object position from x / y / z through d3_SetObjectPos. */

void d3_SetObjectPosXYZ(D3OBJECT *obj, float x, float y, float z)
{
    D3VECTOR4 v;

    v.x = x;
    v.y = y;
    v.z = z;
    d3_SetObjectPos(obj, (D3VECTOR *)&v);
}
/* 0x43b820, sets the object position offset (D3OBJECT.posOffset, added to pos by the update). */

void d3_SetObject078(D3OBJECT *obj, D3VECTOR *v)
{
    d3_UpdateObject(obj, 1);
    d3_ForEachObject2(d3_DefaultScene, obj, 0xc0, d3s_ResetShadows);
    obj->posOffset.x = v->x;
    obj->posOffset.y = v->y;
    obj->posOffset.z = v->z;
}
/* 0x43b860, sets the rotation angles (degrees) and rebuilds the object matrix; a type-3 object
   (the camera, d3_int_io.c) gets the matrix of the negated angles. */

void d3_SetObjectRot(D3OBJECT *obj, D3VECTOR *rot)
{
    D3VECTOR4 v;

    d3_UpdateObject(obj, 1);
    obj->rotation.x = rot->x;
    obj->rotation.y = rot->y;
    obj->rotation.z = rot->z;
    if (obj->type != 3) {
        d3_MatFromAngles((D3VECTOR4 *)&obj->rotation, &obj->matrix);
        d3_ForEachObject2(d3_DefaultScene, obj, 0xc0, d3s_ResetShadows);
        return;
    }
    if (obj == d3_Camera)
        d3_UpdateAll(0);
    v.x = -obj->rotation.x;
    v.y = -obj->rotation.y;
    v.z = -obj->rotation.z;
    d3_MatFromAngles(&v, &obj->matrix);
}
/* 0x43b920, sets the object rotation from x / y / z through d3_SetObjectRot. */

void d3_SetObjectRotXYZ(D3OBJECT *obj, float x, float y, float z)
{
    D3VECTOR v;

    v.x = x;
    v.y = y;
    v.z = z;
    d3_SetObjectRot(obj, &v);
}
/* 0x43b950, sets the view rectangle, the depth range and the projection scale. d3_ScreenH / d3_ScreenW
   (0xfc2df0, 0xfc2df4) are the int screen height / width last set (every caller passes d2_ScreenW,
   d2_ScreenH or d3_Cfg.w, d3_Cfg.h); they are write-only - this function's change check compares them and
   d3_SetFogRange / d3_SetFog pass them back unchanged. d3_ViewBottomFx is the bottom edge as 16.16 fixed, d3_ViewZScale a scale
   set to 1 (a reasoned guess: neither has a reader). */

void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b)
{
    int left, top, right, bottom;
    float hw, hh;

    if (d3_ViewW != w || d3_ViewH != h || d3_Viewport.left != x || d3_Viewport.top != y || d3_ScaleX != scale
        || d3_ZNear != zn || d3_ZFar != zf || d3_ScreenW != a || d3_ScreenH != b) {
        if (d3_D3DActive)
            d3d_SetViewport(w, h, x, y, zn, zf);
        d3_HalfW = hw = w * 0.5f;
        d3_ScreenW = a;
        d3_ScreenH = b;
        d3_ViewW = w;
        d3_CenterX = hw + x;
        d3_ViewH = h;
        d3_ViewBottomFx = (int)(65536.0f * (h + y));
        d3_HalfH = hh = h * 0.5f;
        d3_ZNear = zn;
        d3_ZFar = zf;
        d3_CenterY = hh + y;
        d3_InvZFar = 1.0f / zf;
        left = (int)(x + 0.5f);
        top = (int)(y + 0.5f);
        right = (int)(w + x);
        bottom = (int)(h + y);
        if (left != d3_Viewport.left || right != d3_Viewport.right || top != d3_Viewport.top || bottom != d3_Viewport.bottom) {
            d3_Viewport.left = left;
            d3_Viewport.top = top;
            d3_Viewport.right = right;
            d3_Viewport.bottom = bottom;
            dd_Cleared = 0;
            dd_ZCleared = 0;
        }
        d3_ScaleY = scale * -1.0f;
        d3_ViewX = x;
        d3_ViewY = y;
        d3_ViewZScale = 1.0f;
        d3_ScaleX = scale;
        d3_SetupFrustum();
        d3_UpdateAll(0);
    }
}
/* 0x43bb70, switches the hardware renderer on / off: switching on flushes the texture cache and
   re-inits the sky; when the mode changed it updates all objects, reloads the textures and the lights. */

void d3_SetHardwareMode(int on)
{
    int old = d3_Hardware;

    if (d3_D3DActive && on) {
        d3_Hardware = 1;
        if (d3_EngineReady)
            d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3s_FreeShadows);
        d3fl_Flush();
        if (d3_Sky)
            d3sky_Init(d3_Sky, 1);
    } else
        d3_Hardware = 0;
    if (old != d3_Hardware && d3_EngineReady) {
        d3_UpdateAll(0);
        tex_ReloadAll(1, 0);
        d3_LightInvalidate(1);
    }
}
/* 0x43bc20, starts the engine: copies the configuration, sets the display mode (and Direct3D), allocates
   the sort buffers and the shadow object, sets the viewport and fills the noise table; clears
   d3_Cfg.maxhicol and the write-only flags. unk is the instance, handed to dd_Init / d3d_Init / dd_SetMode. */

char d3_Init(D3INITCFG *cfg, HWND wnd, int unk)
{
    int i, k, n;
    unsigned char x, y, c;

    d3_D3DActive = 0;
    d3_EditorMode = 0;
    D3E_COPY_AT(offset d3_Cfg, cfg, sizeof(D3INITCFG));
    if (d3_Cfg.viewW + d3_Cfg.viewX > (float)d3_Cfg.w)
        d3_Cfg.viewW = (float)d3_Cfg.w - d3_Cfg.viewX;
    if (d3_Cfg.viewH + d3_Cfg.viewY > (float)d3_Cfg.h)
        d3_Cfg.viewH = (float)d3_Cfg.h - d3_Cfg.viewY;
    if ((d3_Cfg.mode == 0 || d3_Cfg.mode == 2 || d3_Cfg.mode == 4) && d3_Cfg.d3d)
        d3_Cfg.d3d = 0;
    if (d3_Cfg.mode) {
        if (!dd_Init(d3_Cfg.mode, d3_Cfg.bpp, wnd, unk))
            return 0;
        if (d3_Cfg.d3d)
            d3_D3DActive = d3d_Init(d3_Cfg.bpp, (int)wnd, unk);
        else
            d3_D3DActive = 0;
    }
    if (!dd_SetMode(d3_Cfg.w, d3_Cfg.h, d3_Cfg.bpp, d3_Cfg.mode, wnd, unk, d3_Cfg.triple))
        return 0;
    for (i = 0; i < 128; i++)
        d3_ClipPool[i].flags &= 0x7f;
    d3_ClipNone = 6;
    d3_ClipFlagA = 0;
    d3_ClipFlagB = 0;
    dd_SetClipRect((int)d3_Cfg.viewX, (int)d3_Cfg.viewY, (int)(d3_Cfg.viewW + d3_Cfg.viewX - 1.0f),
                   (int)(d3_Cfg.viewH + d3_Cfg.viewY - 1.0f));
    d3_TriFuncs[0] = d3io_NoopCallback;
    d3_TriFuncs[1] = d3io_NoopCallback;
    d3_TriFuncs[3] = d3io_NoopCallback;
    d3_TriFuncs[2] = d3io_NoopCallback;
    d3_TriFuncs[4] = d3io_NoopCallback;
    d3_TriFuncs[5] = d3io_NoopCallback;
    d3_SortBuf = mem_Alloc(d3_Cfg.maxpolys * sizeof(D3SORTENT), "d3:PolyList1");
    d3_SortTmp = mem_Alloc(d3_Cfg.maxpolys * sizeof(D3SORTENT), "d3:PolyList2");
    d3_SortPtr = d3_SortBuf;
    d3_SortCount = 0;
    d3_ObjEnd.flags |= 8;
    d3_TopHead.flags |= 8;
    d3_InitObject(&d3_TopHead);
    d3_InitObject(&d3_ObjEnd);
    d3_TopFirst = &d3_ObjEnd;
    d3_TopLast = &d3_TopHead;
    d3_Camera = 0;
    d3_StockListStart.next = &d3_StockListEnd;
    d3_StockListEnd.next = 0;
    d3_StockListStart.prev = 0;
    d3_StockListEnd.prev = &d3_StockListStart;
    d3anim_Tail.next = 0;
    d3anim_Head.prev = 0;
    d3anim_Head.next = &d3anim_Tail;
    d3anim_Tail.prev = &d3anim_Head;
    d3io_StockDir = 0;
    d3_ShadeInit(0x200, 1);
    d3_FrameCount = 0;
    d3_Cfg.maxhicol = 0;
    tex_Init(d3_Cfg.texcount, 0, d3_Cfg.texParam, 0x40);
    if (d3_D3DActive) {
        d3_D3DActive = d3d_CreateDevice(8, d3_Cfg.texbpp, dd_RBits + dd_GBits + dd_BBits, d3_Cfg.zbpp);
        if (d3_D3DActive) {
            d3d_SetConfig(d3_DefaultD3DConfig);
            d3d_SetWBuffer(1);
            d3_fog_color.col = 0;
            d3_MipSkip = d3_Cfg.texShift;
        }
    }
    d3fl_Init(0x400, 0x40 >> d3_Cfg.flShift, d3_Cfg.flArg, d3_Cfg.flFlag);
    d3_InitShadows(d3_Cfg.shadowSize, 0x80, 0x40, 0x20, 0, 0x40, 0x20);
    d3sky_InitColors();
    d3s_Object = d3_SpawnObject(4, "shadow_fake");
    D3_X(d3s_Object)->meshMode = 3;
    d3s_Object->objectFlags |= 0x20;
    d3_AddObjectDrawData(d3s_Object);
    D3_MESH(d3s_Object)->nsurf = 1;
    d3s_Object->info = &D3_MESH(d3s_Object)->surf[0].info;
    d3s_Object->info->mesh = 0;
    d3s_Object->info->nkeys = (d3s_MaxPolys >> 4) * 25;
    d3s_Object->info->ntris = d3s_MaxPolys * 2;
    d3_AllocObjectPolys(d3s_Object->info);
    d3s_Object->info->nkeys = 0;
    d3s_Object->info->ntris = 0;
    d3_SetHardwareMode(d3_D3DActive);
    d3_EngineReady = 1;
    d3_SetViewport(d3_Cfg.viewW, d3_Cfg.viewH, d3_Cfg.viewX, d3_Cfg.viewY, d3_Cfg.zn, d3_Cfg.zf, d3_Cfg.scale,
                   d3_Cfg.w, d3_Cfg.h);
    d3_CurrentScene = 0;
    d3_HalfAngleX = (float)atan2(d3_HalfW, d3_ScaleX);
    d3_HalfAngleY = (float)atan2(d3_HalfH, d3_ScaleY);
    D3_ZERO_AT(offset d3_Scenes, sizeof(d3_Scenes));
    for (i = 0; i < 256; i++)
        for (k = 0; k < 256; k++)
            d3_Noise[i][k] = rand() * 256 / RAND_MAX;
    for (n = 15; n; n--) {
        for (i = 0; i < 256; i++) {
            for (k = 0; k < 256; k++) {
                x = rand() * 256 / RAND_MAX;
                y = rand() * 256 / RAND_MAX;
                c = d3_Noise[x][y];
                d3_Noise[x][y] = d3_Noise[i][k];
                d3_Noise[i][k] = c;
            }
        }
    }
    par_Init();
    return 1;
}
/* 0x43c240, frees all scenes and the engine: the shadow object, then every scene's object, emitter, terrain,
   sky and event lists, then the two global lists (the mesh drivers from d3_StockListHead, the animations from d3anim_Head), the
   sort buffers, the mirror planes, the sky, the shadows, the textures and finally Direct3D. */

void d3_Shutdown(void)
{
    int cur;
    int i;

    if (d3_EngineReady) {
        d3_EngineReady = 0;
        cur = d3_CurrentScene;
        D3_MESH(d3s_Object)->surf[0].info.nkeys = (d3s_MaxPolys >> 4) * 25;
        D3_MESH(d3s_Object)->surf[0].info.ntris = d3s_MaxPolys * 2;
        d3_DisposeObject(d3s_Object);
        d3s_Object = 0;
        for (i = 0; i < D3_MAX_SCENES; i++) {
            if (i != cur && d3_Scenes[i].first) {
                d3if_SelectScene(i, 1);
                while (d3_TopFirst != &d3_ObjEnd)
                    d3_DisposeObject(d3_TopFirst);
                d3if_MegaCam = 0;
                while (d3_ParHead != &d3_ParEnd)
                    par_Delete(d3_ParHead);
                if (d3_Terrain) {
                    d3fl_FreeMap(d3_Terrain);
                    d3_Terrain = 0;
                }
                if (d3_Sky) {
                    d3sky_Destroy(d3_Sky);
                    d3_Sky = 0;
                }
                if (d3_DefaultScene->events) {
                    mem_Free(d3_DefaultScene->events);
                    d3_DefaultScene->events = 0;
                }
            }
        }
        if (cur != d3_CurrentScene)
            d3if_SelectScene(cur, 1);
        while (d3_TopFirst != &d3_ObjEnd)
            d3_DisposeObject(d3_TopFirst);
        if (d3_Terrain) {
            d3fl_FreeMap(d3_Terrain);
            d3_Terrain = 0;
        }
        if (d3_Sky) {
            d3sky_Destroy(d3_Sky);
            d3_Sky = 0;
        }
        par_DeleteAll();
        if (d3_DefaultScene->events) {
            mem_Free(d3_DefaultScene->events);
            d3_DefaultScene->events = 0;
        }
        while (d3_StockListStart.next != &d3_StockListEnd)
            d3_StockListStart.next->Release(d3_StockListStart.next);
        while (d3anim_Head.next != &d3anim_Tail)
            d3anim_Release(d3anim_Head.next, 1);
        if (d3_Mirror.obj && d3_Mirror.planes) {
            mem_Free(d3_Mirror.planes);
            d3_Mirror.planes = 0;
            d3_Mirror.obj = 0;
            d3_Mirror.info = 0;
        }
        mem_Free(d3_SortBuf);
        d3_SortBuf = 0;
        mem_Free(d3_SortTmp);
        d3_SortTmp = 0;
        d3sky_Free();
        d3_ExitShadows();
        d3fl_Exit();
        tex_Exit();
        d3_ShadeReset();
        d3io_StockDir = 0;
        if (d3_D3DActive)
            d3d_Exit();
        dd_Exit();
    }
}
/* 0x43c4e0, applies the graphics options: the mip mode (a), the texture detail (b), the texture shift (c)
   and the shadow mode; a changed mode resets the object shadows, a changed shift flushes and reloads the
   textures (e = reload flag). */

void d3_ApplyGraphicsOptions(unsigned char a, unsigned char b, unsigned char c, unsigned char mode, int e)
{
    if (d3_ShadowMode.mode != mode || d3_MipSkip != c) {
        d3_ShadowMode.mode = mode;
        d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, d3s_ResetShadows);
    }
    d3fl_SetMips(a);
    d3fl_SetDetail(b);
    if (d3_MipSkip != c) {
        d3fl_Flush();
        d3_MipSkip = c;
        tex_ReloadAll(1, e);
    }
}
/* 0x43c570, allocates an object; types >= 5 are lights, the kind taken from the name's first letter. d3_EditorMode
   (0x633438) is a reasoned guess: its only uses are editor-like (the ortho Oben/Vorne/Seite cameras, type-6 "r"
   objects as type 5 here, the on=0 relight of d3_LightInvalidate) and this build only ever stores 0. The other
   identifier unk194 is D3LIGHTINFO.time now (D3LDATA.time in d3_light.h). */

D3OBJECT *d3_SpawnObject(int type, char *name)
{
    D3OBJECT *obj = mem_Alloc(0x228, "d3:SpawnObject");

    if (type >= 5)
        D3_X(obj)->light = mem_Alloc(0x1ac, "d3:SpawnObject(lightinfo)");
    d3_InitObject(obj);
    if (type >= 5) {
        switch (name[0]) {
        case 'p':
            obj->type = 7;
            break;
        case 'r':
            obj->type = 6;
            break;
        case 's':
            obj->type = 8;
            break;
        default:
            obj->type = 5;
            break;
        }
        if (d3_EditorMode && obj->type == 6)
            obj->type = 5;
    } else
        obj->type = type;
    switch (obj->type) {
    case 6:
        D3_X(obj)->light->time = 0;
        break;
    case 8:
        obj->objectFlags |= 4;
        break;
    }
    D3_X(obj)->type0 = obj->type;
    strncpy(obj->name, name, 0x40);
    obj->scene = d3_DefaultScene;
    return obj;
}
/* 0x43c670, frees the drawable data of an object (the mirror state when obj is the mirror, its mirror
   mesh, light records, shadows and mesh) before the object is reloaded. */

void d3_FreeObjectData(D3OBJECT *obj)
{
    D3SCENE *scene;

    if (obj) {
        scene = obj->scene;
        if (d3_Mirror.obj == obj) {
            if (d3_Mirror.planes) {
                mem_Free(d3_Mirror.planes);
                d3_Mirror.planes = 0;
            }
            d3_Mirror.obj = 0;
            d3_Mirror.info = 0;
        }
        if (obj->type == 1 && D3_X(obj)->type0 != 1)
            obj->type = D3_X(obj)->type0;
        if (obj->material && D3_MESH(obj)->mirrorSource) {
            if (D3_MESH(obj)->mirrorPoints) {
                mem_Free(D3_MESH(obj)->mirrorPoints);
                D3_MESH(obj)->mirrorPoints = 0;
            }
            if (D3_MESH(obj)->mirrorPolys) {
                mem_Free(D3_MESH(obj)->mirrorPolys);
                D3_MESH(obj)->mirrorPolys = 0;
            }
        }
        if (obj->type == 6 || (obj->type == 5 && obj->name[0] == 'r'))
            d3_LightRemoveRecords(obj, 0);
        d3_ForEachObject(scene, 0, 7, (D3OBJECTCB)d3_LightRemoveRecords, obj);
        if (obj->type == 8)
            d3_ForEachObject(scene, 0, 0x40, (D3OBJECTCB)d3s_FreeLightCB, obj);
        if (obj->objectFlags & 4)
            d3s_FreeShadows(obj);
        d3_FreeMesh(obj);
    }
}
/* 0x43c7a0, deletes an object with its children and following siblings of its group. */

void d3_DisposeObject(D3OBJECT *obj)
{
    int idx;
    int cur;
    char msg[0x400];
    D3SCENE *scene;
    D3PARTSYS *ps, *next;

    if (obj) {
        cur = d3_CurrentScene;
        scene = obj->scene;
        idx = -1;
        if (d3_DefaultScene != scene) {
            sprintf(msg, "d3_DisposeObject(): invalid universe by object %s", obj);
            idx = (unsigned int)((char *)scene - (char *)d3_Scenes) / sizeof(D3SCENE);
            d3if_SelectScene(idx, 1);
        }
        if (obj->material && D3_MESH(obj)->freeCallback)
            D3_MESH(obj)->freeCallback(obj);
        if (D3_X(obj)->events) {
            mem_Free(D3_X(obj)->events);
            D3_X(obj)->events = 0;
        }
        if (obj->type == 1 && D3_X(obj)->type0 != 1)
            obj->type = D3_X(obj)->type0;
        if (d3_Mirror.obj == obj) {
            if (d3_Mirror.planes) {
                mem_Free(d3_Mirror.planes);
                d3_Mirror.planes = 0;
            }
            d3_Mirror.obj = 0;
            d3_Mirror.info = 0;
        }
        for (ps = ((D3SCENELISTS *)scene)->parHead; ps != &d3_ParEnd; ps = next) {
            next = ps->next;
            if (ps->parent == obj)
                par_Delete(ps);
        }
        if (D3_X(obj)->path)
            d3anim_FreePath(obj);
        if (obj->state & 2)
            d3_UnlinkTop(obj);
        if (obj->material && D3_MESH(obj)->mirrorSource) {
            if (D3_MESH(obj)->mirrorPoints) {
                mem_Free(D3_MESH(obj)->mirrorPoints);
                D3_MESH(obj)->mirrorPoints = 0;
            }
            if (D3_MESH(obj)->mirrorPolys) {
                mem_Free(D3_MESH(obj)->mirrorPolys);
                D3_MESH(obj)->mirrorPolys = 0;
            }
        }
        if (obj->type == 6 || (obj->type == 5 && obj->name[0] == 'r'))
            d3_LightRemoveRecords(obj, 0);
        d3_ForEachObject(scene, 0, 0x87, (D3OBJECTCB)d3_LightRemoveRecords, obj);
        if (obj->type == 8)
            d3_ForEachObject(scene, 0, 0xc0, (D3OBJECTCB)d3s_FreeLightCB, obj);
        if (obj->objectFlags & 4)
            d3s_FreeShadows(obj);
        if (D3_X(obj)->light) {
            mem_Free(D3_X(obj)->light);
            D3_X(obj)->light = 0;
        }
        if (obj->child)
            d3_DisposeObject(obj->child);
        if (obj->next && !(obj->next->state & 1))
            d3_DisposeObject(obj->next);
        d3snd_Free(obj->sound);
        d3_FreeMesh(obj);
        mem_Free(obj);
        if (idx != -1)
            d3if_SelectScene(cur, 1);
    }
}
/* 0x43ca20, appends obj to the top-level list of the default scene. */

void d3_LinkTop(D3OBJECT *obj)
{
    if (obj->type == 3 && !d3_Camera) {
        d3_SetCamera(obj);
        d3_SetObjectRot(obj, &obj->rotation);
    }
    if (obj->type) {
        obj->prev = d3_TopLast;
        obj->next = &d3_ObjEnd;
        d3_TopLast->next = obj;
        d3_TopLast = obj;
        obj->state |= 3;
    }
    obj->scene = d3_DefaultScene;
}
/* 0x43cab0, unlinks obj from its sibling list and clears its state bit 2 (linked); for the camera
   (type 3) d3_Camera is cleared too. */

void d3_UnlinkTop(D3OBJECT *obj)
{
    switch (obj->type) {
    case 3:
        if (d3_Camera == obj)
            d3_Camera = 0;
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        obj->next->prev = obj->prev;
        obj->prev->next = obj->next;
    }
    obj->state &= 0xfd;
}
/* 0x43cb20, appends obj after the last sibling of first. */

void d3_AppendSibling(D3OBJECT *first, D3OBJECT *obj)
{
    D3OBJECT *last;

    obj->bits.top = !first->parent;
    obj->parent = first->parent;
    last = first;
    while (last->next)
        last = last->next;
    last->next = obj;
    obj->prev = last;
}
/* 0x43cb80, makes obj a child of parent (a top-level object when parent is NULL) */

void d3_AddChild(D3OBJECT *parent, D3OBJECT *obj)
{
    D3_OBJBITS(obj).top = !parent;
    if (parent) {
        if (parent->child) {
            d3_AppendSibling(parent->child, obj);
            obj->parent = parent;
        } else {
            parent->child = obj;
            obj->parent = parent;
        }
    } else
        d3_LinkTop(obj);
}
/* 0x43cbe0, makes obj the active camera (d3_Camera) when it is a camera object (type 3) and is not the
   current one; the default scene is then rebuilt through d3_UpdateAll(0) (not the full variant). */

void d3_SetCamera(D3OBJECT *obj)
{
    if (obj != d3_Camera && obj->type == 3) {
        d3_Camera = obj;
        d3_UpdateAll(0);
    }
}
/* 0x43cc10, (re)allocates the mesh data of info from its counts: ntris D3TRIs and nkeys + 8 D3MESHKEYs
   (the last 8 are the bounding box corners that follow the keys); the previous blocks are freed. */

void d3_AllocObjectPolys(D3OBJINFO *info)
{
    if (info->nkeys && info->ntris) {
        if (info->tris) {
            mem_Free(info->tris);
            info->tris = 0;
        }
        info->tris = mem_Alloc(info->ntris * 0x28, "d3:AllocObjectPolys");
        if (info->keys) {
            mem_Free(info->keys);
            info->keys = 0;
        }
        info->keys = mem_Alloc((info->nkeys + 8) * 0x50, "d3:AllocObjectPoints");
    }
}
/* 0x43cc90, frees one surface of a mesh: fades the surface out through d3_SetObjectAlpha (alpha 0xff, not
   modulated) while it is still visible, then frees the keys, the triangles, the texture handles and the
   three texture names. */

void d3_FreeSurf(D3SURF *s)
{
    union {
        int i;
        D3TEXFLAGS f;
    } flags;
    int i;
    D3SURFTEX *t;

    if (s->flags.f.b[0] < 0xff || (s->flags.f.b[2] & 1)) {
        flags.i = 0;
        flags.f.b[2] = 0;
        flags.f.b[0] = 0xff;
        d3_SetObjectAlpha(s->owner, &s->info, flags.f);
    }
    if (s->info.nkeys > 0 && s->info.keys) {
        mem_Free(s->info.keys);
        s->info.keys = 0;
        s->info.nkeys = 0;
    }
    if (s->info.ntris > 0 && s->info.tris) {
        mem_Free(s->info.tris);
        s->info.tris = 0;
        s->info.ntris = 0;
    }
    if (s->texHandles) {
        if (s->info.mesh)
            for (i = 0; i < D3_OPS(&s->info)->count; i++)
                if (((void **)s->texHandles)[i])
                    tex_Release(((void **)s->texHandles)[i]);
        mem_Free((void *)s->texHandles);
        s->texHandles = 0;
    }
    if (s->info.mesh) {
        D3_OPS(&s->info)->Release((D3OPS *)s->info.mesh);
        s->info.mesh = 0;
    }
    for (i = 0, t = s->slots; i < 3; i++, t++)
        if (t->data) {
            d3anim_Release(t->data, 0);
            t->data = 0;
        }
}
/* 0x43cd80, frees the mesh of obj: the nsurf loaded surfaces, surf[3] (the stock surface d3io_AttachStock
   fills, which is not counted in nsurf) and the mesh block itself (D3MESH, obj->material); obj->info is
   cleared. */

void d3_FreeMesh(D3OBJECT *obj)
{
    int i;

    if (obj->material) {
        for (i = 0; i < D3_MESH(obj)->nsurf; i++)
            d3_FreeSurf(&D3_MESH(obj)->surf[i]);
        d3_FreeSurf(&D3_MESH(obj)->surf[3]);
        D3_MESH(obj)->nsurf = 0;
        mem_Free(obj->material);
        obj->material = 0;
    }
    obj->info = 0;
}
/* 0x43ce10, initializes one surface: flags (alpha 0xff, the modulation bit clear), empty geometry and the
   owner object; the three animation slots are reset (play flag off, no data, no frame). */

void d3_InitSurf(D3SURF *s, D3OBJECT *owner)
{
    int i;
    int *t;

    s->flags.i = 0;
    s->flags.f.b[0] = 0xff;
    s->info.keys = 0;
    s->info.nkeys = 0;
    s->info.tris = 0;
    s->info.ntris = 0;
    s->info.mesh = 0;
    s->owner = owner;
    for (i = 0, t = &s->slots[0].nextFrame; i < 3; i++, t += 0x1f) {
        ((D3SURFTEX *)(t - 1))->playFlags &= 0xfd;
        ((D3SURFTEX *)(t - 1))->data = 0;
        ((D3SURFTEX *)(t - 1))->nextFrame = 0;
        ((D3SURFTEX *)(t - 1))->subFrame = 0;
        ((D3SURFTEX *)(t - 1))->frame = 0;
        ((D3SURFTEX *)(t - 1))->flags = 0;
    }
}
/* 0x43ce60, initialises an object: clears the ten preset camera views and the pivot / posOffset, unlinks it
   (prev / next / parent / child / sound), gives it the type-1 defaults (objectFlags, state, stateFlags, flags,
   kind, mask, meshMode), clears its path, events and last-light time, and gives a light the flicker defaults 0.3
and every 15 (the light and its seven keys); finally the angles and position are zeroed. @deep deep s23: stateFlags &= 0xc1 first (before objectFlags): 10 -> 6 ins. Left: the light keys loop stores [ebx+eax-4] where the original has [eax+ebx-4] (IV first). Loop body swapped, &e[0] + i, i[e], int i, chain assignments of the field clears, light != 0: no lever or worse. flip.py: deleting any one store above the loop or moving lightTime = 0 behind the light block flips it (lightTime there costs 4 bytes elsewhere): a value-numbering tie, not the loop's spelling. */
void d3_InitObject(D3OBJECT *obj)
{
    unsigned int i;

    for (i = 0; i < 10; i++) {
        D3_X(obj)->views[i].pos.x = 0;
        D3_X(obj)->views[i].pos.y = 0;
        D3_X(obj)->views[i].pos.z = 0;
        D3_X(obj)->views[i].dir.x = 0;
        D3_X(obj)->views[i].dir.y = 0;
        D3_X(obj)->views[i].dir.z = 0;
    }
    obj->stateFlags &= 0xc1;
    obj->objectFlags = obj->objectFlags & 0xa2 | 2;
    D3_X(obj)->meshMode = 0;
    obj->prev = 0;
    obj->next = 0;
    obj->child = 0;
    obj->parent = 0;
    obj->sound = 0;
    obj->state = obj->state & 0x18 | 0x65;
    obj->flags &= 0xcf;
    obj->type = 1;
    obj->kind = 0;
    obj->mask = 0;
    D3_X(obj)->time = 0;
    D3_X(obj)->path = 0;
    D3_X(obj)->events = 0;
    D3_X(obj)->lightTime = 0;
    if (D3_X(obj)->light) {
        D3_X(obj)->light->list = 0;
        D3_X(obj)->light->interval = 15;
        D3_X(obj)->light->flicker = 0.3f;
        D3_X(obj)->light->ramp = 0;
        for (i = 0; i < 7; i++) {
            D3_X(obj)->light->e[i].interval = 15;
            D3_X(obj)->light->e[i].flicker = 0.3f;
        }
    }
    D3_X(obj)->pivot.x = 0;
    D3_X(obj)->pivot.y = 0;
    D3_X(obj)->pivot.z = 0;
    D3_X(obj)->posOffset.x = 0;
    D3_X(obj)->posOffset.y = 0;
    D3_X(obj)->posOffset.z = 0;
    d3_SetObjectRotXYZ(obj, 0, 0, 0);
    d3_SetObjectPosXYZ(obj, 0, 0, 0);
}
/* 0x43cfb0, gives obj an empty mesh block (D3MESH, 0x970 bytes, "d3:AddObjectDrawData") when it has none:
   the four surfaces and four shadow slots are initialized, the counters, mirror pointers and the copy
   pointers are cleared, frameStamp is -1 (never transformed) and flags gets 0x40 (no vertex animation). */

void d3_AddObjectDrawData(D3OBJECT *obj)
{
    int i;

    if (!obj->material) {
        obj->material = mem_Alloc(0x970, "d3:AddObjectDrawData");
        for (i = 0; i <= 3; i++)
            d3_InitSurf(&D3_MESH(obj)->surf[i], obj);
        for (i = 0; i < 4; i++) {
            D3_MESH(obj)->sub[i].dir.x = 0;
            D3_MESH(obj)->sub[i].dir.y = 0;
            D3_MESH(obj)->sub[i].dir.z = 0;
            D3_MESH(obj)->sub[i].light = 0;
            D3_MESH(obj)->sub[i].tex = 0;
        }
        D3_MESH(obj)->nsurf = 0;
        D3_MESH(obj)->freeCallback = 0;
        D3_MESH(obj)->mirrorPoints = 0;
        D3_MESH(obj)->mirrorPolys = 0;
        D3_MESH(obj)->mirrorSource = 0;
        D3_MESH(obj)->groundOffsetRaw = 0;
        D3_MESH(obj)->lightScale = 1.0f;
        D3_MESH(obj)->lightGeneration = 0;
        D3_MESH(obj)->frameStamp = -1;
        D3_MESH(obj)->flags |= 0x40;
    }
}
/* 0x43d0e0, unlinks obj from its siblings and parent. */

void d3_Unlink(D3OBJECT *obj)
{
    if (obj->next) {
        obj->next->prev = obj->prev;
        if (obj->prev)
            obj->prev->next = obj->next;
        else if (obj->parent->child == obj)
            obj->parent->child = obj->next;
    } else if (obj->prev)
        obj->prev->next = 0;
    else if (obj->parent->child == obj)
        obj->parent->child = 0;
    obj->parent = 0;
    obj->prev = 0;
    obj->next = 0;
    obj->state &= 0xfd;
}
/* 0x43d180, copies an object (name + "_"), without its children: position, the saved camera / light fields,
   angles, pivot, offset, the light keys, the views and the flags; a mesh object gets its draw data and its
   first surface's mesh again */

D3OBJECT *d3_CloneObject(D3OBJECT *src)
{
    D3OBJECT *obj = 0;

    if (src) {
        obj = mem_Alloc(0x228, "d3:CloneObject");
        if (src->type >= 5)
            D3_X(obj)->light = mem_Alloc(0x1ac, "d3:CloneObject(lightinfo)");
        d3_InitObject(obj);
        strcpy(obj->name, src->name);
        strcat(obj->name, "_");
        D3_X(obj)->pos.x = D3_X(src)->pos.x;
        D3_X(obj)->pos.y = D3_X(src)->pos.y;
        D3_X(obj)->pos.z = D3_X(src)->pos.z;
        D3_X(obj)->flickerEnd = D3_X(src)->flickerEnd;
        D3_X(obj)->savedPos.x = D3_X(src)->savedPos.x;
        D3_X(obj)->savedPos.y = D3_X(src)->savedPos.y;
        D3_X(obj)->savedPos.z = D3_X(src)->savedPos.z;
        D3_X(obj)->radius = D3_X(src)->radius;
        D3_X(obj)->rot.x = D3_X(src)->rot.x;
        D3_X(obj)->rot.y = D3_X(src)->rot.y;
        D3_X(obj)->rot.z = D3_X(src)->rot.z;
        D3_X(obj)->savedRot.x = D3_X(src)->savedRot.x;
        D3_X(obj)->savedRot.y = D3_X(src)->savedRot.y;
        D3_X(obj)->savedRot.z = D3_X(src)->savedRot.z;
        D3_X(obj)->pivot.x = D3_X(src)->pivot.x;
        D3_X(obj)->pivot.y = D3_X(src)->pivot.y;
        D3_X(obj)->pivot.z = D3_X(src)->pivot.z;
        D3_X(obj)->posOffset.x = D3_X(src)->posOffset.x;
        D3_X(obj)->posOffset.y = D3_X(src)->posOffset.y;
        D3_X(obj)->posOffset.z = D3_X(src)->posOffset.z;
        D3_X(obj)->path = 0;
        D3_X(obj)->info = 0;
        if (D3_X(src)->light)
            D3E_COPY3(D3_X(obj)->light, D3_X(src)->light, 0x188);
        D3E_COPY3(D3_X(obj)->views, D3_X(src)->views, 0xf0);
        D3E_COPY3(&obj->state, &src->state, 0x18);
        if (src->material) {
            d3_AddObjectDrawData(obj);
            D3_MESH(obj)->groundOffsetRaw = D3_MESH(src)->groundOffsetRaw;
            D3_MESH(obj)->lightScale = D3_MESH(src)->lightScale;
            if (D3_MESH(src)->nsurf > 0)
                d3_AttachMesh(obj, 0, (char *)D3_MESH(src)->surf[0].info.mesh);
        }
        obj->objectFlags &= 0x9f;
        obj->state = obj->state & 0xfc | 4;
        d3_SetObjectPos(obj, &D3_X(obj)->pos);
        d3_SetObjectRot(obj, &D3_X(obj)->rot);
    }
    return obj;
}
/* 0x43d430, sets the alpha of a surface: textures are swapped for alpha aliases (or back). @deep deep s26: climb round 1 (87 mutants): nothing. Toy compiles (scratch, 10 spellings of the tris[i].tex load / store: index, &tris[i]->, i * sizeof byte offsets, raw +0x14 int arithmetic, unsigned / short i, != 0 test, continue): VC6 always emits [tris + off] (the reloaded pointer first, the strength-reduced offset second) as ours; the original's [off + tris] comes from none of them. Same class as ai_AlchemistWindow's function-wide SIB order (s25). Left: the 4 base / index swaps. prior: B3 draft: after the reading fixes (the three raw *(int*)((char*)s->info.mesh+0x1e0) are now D3_OPS(&s->info)->count, the function is readable) only the address encoding of the two tris[i].tex accesses per loop remains, 4 hunks / 4 of 512 B: ours [edx+ecx+0x14] and [eax+ecx+0x14], original [ecx+edx+0x14] and [ecx+eax+0x14] - the same instructions with base and index swapped. Tried in this state: the store through the local tris[i].tex (469/528 B, reverted); earlier attempts: tris[i] directly, (tris + i)->, i[tris], 0x28 byte offsets, integer arithmetic i*0x28+(int)tris, unsigned i, a pointer IV. */

void d3_SetObjectAlpha(D3OBJECT *obj, D3OBJINFO *info, D3TEXFLAGS f)
{
    D3SURF *s = (D3SURF *)info;
    D3TEXMAP *map;
    D3PTRI *tris;
    D3TEXTURE *t;
    int i, j, n;

    if (obj && s && D3_OPS(&s->info)->count >= 0) {
        if (f.b[0] >= 0xff && !(f.b[2] & 1)) {
            map = mem_Alloc(D3_OPS(&s->info)->count * 8, "!d3:ChangeObjectTransparency()");
            obj->objectFlags &= 0x7f;
            s->flags.i = 0;
            s->flags.f.b[2] &= 0xfe;
            s->flags.f.b[0] = 0xff;
            n = 0;
            for (i = 0; i < s->info.ntris; i++) {
                tris = (D3PTRI *)s->info.tris;
                if (t = tris[i].tex) {
                    for (j = 0; j < n; j++)
                        if (map[j].from == t)
                            break;
                    if (j == n) {
                        map[j].from = t;
                        map[j].to = tex_ReleaseAlias(t, 1);
                        n++;
                    }
                    ((D3PTRI *)s->info.tris)[i].tex = map[j].to;
                }
            }
            mem_Free(map);
            d3_SetVertexColors(obj, s, 0, f);
            return;
        }
        if (s->flags.f.b[0] == 0xff && !(s->flags.f.b[2] & 1)) {
            obj->objectFlags |= 0x80;
            map = mem_Alloc(D3_OPS(&s->info)->count << 3, "!d3:ChangeObjectTransparency()");
            n = 0;
            for (i = 0; i < s->info.ntris; i++) {
                tris = (D3PTRI *)s->info.tris;
                if (t = tris[i].tex) {
                    for (j = 0; j < n; j++)
                        if (map[j].from == t)
                            break;
                    if (j == n) {
                        map[j].from = t;
                        map[j].to = (D3TEXTURE *)tex_AliasForFlags((struct TSTEXTURE *)t, f, 1);
                        n++;
                    }
                    ((D3PTRI *)s->info.tris)[i].tex = map[j].to;
                }
            }
            mem_Free(map);
        }
        s->flags.f = f;
        d3_SetVertexColors(obj, s, 1, f);
    }
}
/* 0x43d630, d3_ForEachObject callback: sets the alpha of all surfaces. */

char d3_SetAlphaCB(D3OBJECT *obj, D3TEXFLAGS *f)
{
    unsigned int i;

    if (obj->material)
        for (i = 0; i < D3_MESH(obj)->nsurf; i++)
            d3_SetObjectAlpha(obj, (D3OBJINFO *)&D3_MESH(obj)->surf[i], *f);
    return 1;
}
/* 0x43d690, applies the alpha flags f to obj and its group in the default scene (object mask 0x240;
   d3_SetAlphaCB sets the alpha of every surface). */

void d3_SetAlpha(D3OBJECT *obj, D3TEXFLAGS *f)
{
    if (obj)
        d3_ForEachObject(d3_DefaultScene, obj, 0x240, (D3OBJECTCB)d3_SetAlphaCB, f);
}
/* 0x43d6c0, attaches obj to the bone `name` of its parent's animated mesh: the driver's bone table
   (D3ANIMMESH bones at +0x74, 0x58 bytes each, nbones at +0x208) is searched by name, the name is stored
   in the object's mesh at +0xb4 (the field d3anim_MapBones matches its children by) and the parent is
   re-mapped and posed. Returns 1 when the bone was found, 0 otherwise. */

char d3_AttachToBone(D3OBJECT *obj, char *name)
{
    D3OBJECT *parent;
    char *t;
    int i;

    if (obj && name && obj->parent && obj->material) {
        strcpy((char *)D3_MESH(obj) + 0xb4, "");
        parent = obj->parent;
        if (parent->material && (t = (char *)D3_MESH(parent)->surf[0].info.mesh)) {
            for (i = 0; i < ((unsigned char *)t)[0x208]; i++)
                if (!strcmp(t + 0x74 + i * 0x58, name))
                    break;
            if (i < ((unsigned char *)t)[0x208]) {
                strcpy((char *)D3_MESH(obj) + 0xb4, name);
                d3anim_MapBones(parent);
                d3anim_PoseBones(parent);
                return 1;
            }
        }
    }
    return 0;
}
/* 0x43d7c0, switches the object to the surface of its current level of detail, transforms it with the
   mesh driver and marks the object dirty. */

void d3_TransformSurf(D3OBJECT *obj)
{
    D3OBJINFO *info;
    char buf[0x40];

    info = d3_SelectSurf(obj);
    if (info) {
        if (info != obj->info)
            obj->info = info;
        d3_WorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 0, buf);
        D3_OPS(info)->Transform(obj, info, buf);
        obj->state |= 4;
    }
}
/* 0x43d840, transforms every mesh object (type 4, the 0x40 type mask) of the default scene through
   d3_TransformSurf. */

void d3_TransformMeshes(void)
{
    d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3_TransformSurf);
}
/* 0x43d860, bounding sphere of an object relative to camera. */

char d3_GetBounds(D3OBJECT *obj, D3OBJECT *camera, D3VECTOR *center, float *radius)
{
    char ok = 0;
    D3VECTOR4 c;
    D3OBJINFO *info;
    D3BOXVERT *box, *v;
    float r, d, dx, dy, dz;
    int i;

    if (obj && (center || radius)) {
        if (!obj->type) {
            d3_PointToRef(obj, camera, &obj->pos, &c);
            r = D3_X(obj)->radius;
            ok = 1;
        } else {
            info = obj->info;
            if (info && info->mesh) {
                d3_UpdateWorldMatrix(obj, camera, 0);
                box = D3_OPS(info)->GetBox(obj, info, D3_MESH(obj)->cameraMatrix);
                if (box) {
                    c.x = box->x;
                    c.y = box->y;
                    c.z = box->z;
                    for (i = 1, v = box + 1; i < 8; i++, v++) {
                        c.x += v->x;
                        c.y += v->y;
                        c.z += v->z;
                    }
                    c.x *= 0.125f;
                    c.y *= 0.125f;
                    c.z *= 0.125f;
                    r = 0.0f;
                    for (i = 0, v = box; i < 8; i++, v++) {
                        dx = c.x - v->x;
                        dy = c.y - v->y;
                        dz = c.z - v->z;
                        d = dx * dx + dy * dy + dz * dz;
                        r = max(r, d);
                    }
                    r = (float)sqrt(r);
                    ok = 1;
                }
            }
        }
        if (ok) {
            if (radius)
                *radius = r;
            if (center) {
                center->x = c.x;
                center->y = c.y;
                center->z = c.z;
            }
        }
    }
    return ok;
}
/* 0x43da00, d3_ForEachObject2 callback: the inverse of d3_InflateObject, frees the geometry (keys and
   triangles) it built for every surface; a visible surface is faded out through d3_SetObjectAlpha first. */

void d3_DeflateObject(D3OBJECT *obj)
{
    char any;
    D3TEXFLAGS f;
    int i;

    if (obj && obj->material) {
        any = 0;
        for (i = 0; i <= 3; i++) {
            if (D3_MESH(obj)->surf[i].info.nkeys > 0) {
                any = 1;
                if (D3_MESH(obj)->surf[i].flags.f.b[0] < 0xff || (D3_MESH(obj)->surf[i].flags.f.b[2] & 1)) {
                    memset(&f, 0, sizeof(f));
                    f.b[2] &= 0xfe;
                    f.b[0] = 0xff;
                    d3_SetObjectAlpha(obj, &D3_MESH(obj)->surf[i].info, f);
                }
                D3_MESH(obj)->surf[i].info.ntris = 0;
                D3_MESH(obj)->surf[i].info.nkeys = 0;
                if (D3_MESH(obj)->surf[i].info.keys) {
                    mem_Free(D3_MESH(obj)->surf[i].info.keys);
                    D3_MESH(obj)->surf[i].info.keys = 0;
                }
                if (D3_MESH(obj)->surf[i].info.tris) {
                    mem_Free(D3_MESH(obj)->surf[i].info.tris);
                    D3_MESH(obj)->surf[i].info.tris = 0;
                }
            }
        }
        obj->info = 0;
        if (any)
            d3_ForEachObject(d3_DefaultScene, 0, 0x86, (D3OBJECTCB)d3_LightRemoveRecords, obj);
    }
}
/* 0x43db30, unloads the scene data and sets d3_SceneFlags to flags: flag 2 frees the terrain tiles
   (d3fl_FreeTiles), flag 1 deflates every object's geometry (d3_DeflateObject). The inverse of
   d3_InflateScene. The sprintf target is a debug trace left over from the original. */

char d3_DeflateScene(unsigned int flags)
{
    char msg[0x100];

    sprintf(msg, "du %i %s (%i df: %i) -> %i", d3_FrameCount, d3_DefaultScene, d3_CurrentScene, d3_SceneFlags, flags);
    if ((flags & 2) && d3_Terrain)
        d3fl_FreeTiles(d3_Terrain);
    if (flags & 1)
        d3_ForEachObject2(d3_DefaultScene, 0, 0x1ff, d3_DeflateObject);
    d3_SceneFlags = flags;
    return 1;
}
/* 0x43dbc0, d3_ForEachObject2 callback: builds the drawable points and triangles of all surfaces
   from the loaded mesh data.
   @deep s47: the triangle loop as a plain index loop (t = &((D3PTRI *)s->info.tris)[k] per pass, j = 0 in
   the for-init / before the loop / at the surface top, and p = &src->polys[k] named first): all worse,
   131-177 ins and frame 0x24 against 0x20 (the hand t++ walk is the original's: VC6 does not
   strength-reduce t back). What the original's zero is: k, j and the poly offset are stored 0 from ecx
   before the loop, the loop head re-zeroes ecx (the entry jmp skips that xor) and only vofs / uofs / zofs
   take it; flags = 0 is an immediate after the inner loop. c2trace: const#0 is cut into 45 pieces in pass
   1 (s#1 takes esi), const#0#16 (lines of the surface head, 14 refs) and #18 (the triangle loop, 13 refs)
   survive to ebx. Open as before: the reference that keeps #18 alive past the inner vertex loop. prior:
   headers sample: insensitive (32 samples, one codegen). Read again: the original's zero is a loop-local
   candidate in ecx (xor ecx,ecx at the loop head, skipped by a jmp on entry where k = 0 already left ecx
   zero; vofs/uofs/zofs stored from ecx, flags = 0 and tex = 0 as immediates, the mat tests as test
   reg,reg), and ebx is a plain temp (bl for the b0 and rawFlags byte updates, the blend test, mat[] base);
   ours gives the zero ebx for the whole triangle loop (cmp reg,ebx, byte stores from bl, xor ebx,ebx after
   each inner loop). Same meaning. dcp try (this session): p->mat for the t->src casts 123; m split into a
   mat-index local and/or a second inner-loop counter = control; flags = 0 moved to each of 9 positions
   110-160; b0 = 0 first = control, last 106 (no structural gain, not kept); vofs/uofs/zofs chained 108
   (regs 30, order 10); int 0 for the floats = control; mat test as (m = ..) < 0 = control; j = 0 before
   the loop = control; t->src = p dropped = control; int made 114; col.l = ~0 at 9 positions or first
   110-131. Next: what makes const#0 a whole-loop candidate in ours (c2prio-style count of its references
   per block). (tiebreak: insensitive)


   */

void d3_InflateObject(D3OBJECT *obj)
{
    D3SURF *s;
    D3MESHSRC *src;
    D3PVERT *v;
    D3PTRI *t;
    D3SRCPOLY *p;
    D3TEXTURE *tex;
    int *mat;
    int i, k, m, j;
    char made;
    union {
        unsigned long l;
        D3COLOR c;
    } col;

    if (obj && obj->material) {
        obj->state |= 4;
        d3s_ResetShadows(obj);
        for (i = 0; i <= 3; i++) {
            s = &D3_MESH(obj)->surf[i];
            if (src = (D3MESHSRC *)s->info.mesh) {
                made = 0;
                if (!s->info.tris && src->npolys > 0) {
                    s->info.tris = mem_Alloc((s->info.ntris = src->npolys) * 0x28, "d3:InflateObject(allpolys)");
                    made = 1;
                }
                if (!s->info.keys && src->npoints > 0) {
                    s->info.keys = mem_Alloc(((s->info.nkeys = src->npoints) + 8) * 0x50, "d3:InflateObject(allpoints)");
                } else if (!made)
                    continue;
                col.l = 0xffffffff;
                if (src->npoints > 0)
                    for (k = 0; k < s->info.nkeys + 8; k++) {
                        ((D3PVERT *)s->info.keys)[k].src = src->points + k * 0x18;
                        ((D3PVERT *)s->info.keys)[k].color = 0xffffffff;
                        ((D3PVERT *)s->info.keys)[k].specular = 0xffffffff;
                    }
                mat = mem_Alloc(d3_TexCount * 4, "d3:!InflateObject(mat)");
                D3E_FILL(mat, -1, d3_TexCount * 4);
                for (k = 0, j = 0, t = (D3PTRI *)s->info.tris; k < s->info.ntris; k++, t++) {
                    t->vofs = 0.0f;
                    ((D3TRIBITS *)&t->drawFlags)->b0 = 0;
                    t->uofs = 0.0f;
                    t->zofs = 0.0f;
                    t->src = p = &src->polys[k];
                    for (m = 0; m < 3; m++) {
                        t->v[m] = &((D3PVERT *)s->info.keys)[((D3SRCPOLY *)t->src)->idx[m]];
                        t->v[m]->rawFlags |= 0x80;
                    }
                    t->flags = 0;
                    if (((D3SRCPOLY *)t->src)->mat >= 0) {
                        m = mat[((D3SRCPOLY *)t->src)->mat];
                        if (m < 0)
                            mat[((D3SRCPOLY *)t->src)->mat] = m = ((D3TEXTURE **)s->texHandles)[j++] - (D3TEXTURE *)d3_Textures;
                        t->tex = tex = &((D3TEXTURE *)d3_Textures)[m];
                        if (((D3TEXV *)tex)->alpha < 0xff || (((D3TEXV *)tex)->blend & 3)) {
                            col.c.a = ((D3TEXV *)tex)->alpha;
                            if (((D3TEXV *)tex)->blend & 1) {
                                col.c.b = ((D3TEXV *)tex)->alpha;
                                col.c.g = ((D3TEXV *)tex)->alpha;
                                col.c.r = ((D3TEXV *)tex)->alpha;
                            } else {
                                col.c.b = 0xff;
                                col.c.g = 0xff;
                                col.c.r = 0xff;
                            }
                            for (m = 0; m < 3; m++) {
                                t->v[m]->color = col.l;
                                t->v[m]->specular = col.l;
                            }
                        }
                        ((D3TRIBITS *)&t->drawFlags)->b3 = tex->flags;
                        for (m = 0; m < 3; m++)
                            t->v[m]->wrap = tex->flags & 1;
                    } else
                        t->tex = 0;
                }
                mem_Free(mat);
            }
        }
    }
}
/* 0x43deb0, loads the scene data and sets d3_SceneFlags to ~flags & 3: flag 2 inflates the terrain tiles
   (d3fl_Inflate), flag 1 inflates every object's geometry (d3_InflateObject). The inverse of
   d3_DeflateScene; when the load leaves no scene flag set and the default scene is not the terrain scene,
   the scene's map is released. */

char d3_InflateScene(unsigned int flags)
{
    char msg[0x100];

    sprintf(msg, "iu %i %s (%i df: %i) -> %i", d3_FrameCount, d3_DefaultScene, d3_CurrentScene, d3_SceneFlags, flags);
    if ((flags & 2) && d3_Terrain)
        d3fl_Inflate(d3_Terrain);
    if (flags & 1)
        d3_ForEachObject2(d3_DefaultScene, 0, 0x1ff, d3_InflateObject);
    d3_SceneFlags = ~flags & 3;
    if (!d3_SceneFlags && d3sm_Shadow && !d3_Terrain)
        d3sm_Build(d3sm_Shadow, 0);
    return 1;
}
/* 0x43df60, d3_ForEachObject callback: reloads the mesh of an object. */

char d3_ReloadMeshCB(D3OBJECT *obj, D3RELOADCTX *ctx)
{
    char name[0x100];
    int len;
    char *ext;

    if (obj->material && D3_MESH(obj)->surf[0].info.mesh && obj->kind == ctx->type) {
        if (ctx->count) {
            ctx->n++;
            return 1;
        }
        if (ctx->progress)
            ctx->progress(0, ctx->i++, ctx->n);
        strcpy(name, (char *)D3_MESH(obj)->surf[0].info.mesh);
        len = strlen(name);
        if (len > 2) {
            ext = &name[len - 2];
            if (!strcmp(ext, "_0") || !strcmp(ext, "_1") || !strcmpi(ext, "_S"))
                *ext = 0;
        }
        d3_FreeObjectData(obj);
        if (d3io_LoadObject(name, name))
            d3_AttachMesh(obj, 0, name);
        d3_UpdateObject(obj, 1);
    }
    return 1;
}
/* 0x43e090, reloads the meshes of all objects of the given type: counts them first (for the progress
   callback), then reloads. */

void d3_ReloadMeshes(unsigned int mask, int type, void (*progress)(int a, int i, int n))
{
    D3RELOADCTX ctx;

    ctx.count = 1;
    ctx.i = 0;
    ctx.n = 0;
    ctx.progress = progress;
    ctx.type = type;
    d3_ForEachObject(d3_DefaultScene, 0, mask, (D3OBJECTCB)d3_ReloadMeshCB, &ctx);
    ctx.count = 0;
    d3_ForEachObject(d3_DefaultScene, 0, mask, (D3OBJECTCB)d3_ReloadMeshCB, &ctx);
    tex_ReloadAll(0, (int)progress);
    d3_LightInvalidate(1);
}
/* 0x43e110, d3_ForEachObject callback (Ghidra missed it): switches the type 5 lights named "r..." to type 6
   (on) and the type 6 ones back to type 5; the light is updated either way and a switched-off one gets
   D3OBJX.time = d3_Time, a switched-on one loses its light records. */

char d3_ToggleRLightCB(D3OBJECT *obj, char on)
{
    if (on) {
        if (obj->type == 5 && obj->name[0] == 'r') {
            obj->type = 6;
            D3_X(obj)->time = d3_Time;
            d3_UpdateObject(obj, 0);
        }
    } else if (obj->type == 6) {
        obj->type = 5;
        d3_LightRemoveRecords(obj, 0);
        d3_UpdateObject(obj, 0);
    }
    return 1;
}
/* 0x43e180, switches all "r" lights on (on != 0) or off (d3_ToggleRLightCB per object) and relights the
   scene. */

void d3_SetRLights(unsigned char on)
{
    d3_ForEachObject(d3_DefaultScene, 0, 6, (D3OBJECTCB)d3_ToggleRLightCB, (void *)on);
    d3_LightInvalidate(1);
}
/* 0x43e1b0, end of a frame */

void d3_EndFrame(char tick)
{
    if (tick)
        d3_FrameCount++;
    d3_LightsChanged = 0;
    D3_MESH(d3s_Object)->surf[0].info.ntris = 0;
    D3_MESH(d3s_Object)->surf[0].info.nkeys = 0;
    d3s_NumUVs = 0;
    d3s_NumVerts = 0;
}
/* 0x43e200, draws the default scene: clears the view, the terrain (with the fog range by distance), the
   objects, the shadow object, the particle systems, the sky, the lights and the mirror, then sorts and draws
   the polygons and keeps the frame's counters in the d3_Stat... statistics (write-only) */

void d3_Render(unsigned int mask, char flag)
{
    float d;
    D3PARTSYS *ps, *next;

    if (d3_Busy < 0)
        d3_Busy = 0;
    if (d3_EngineReady && !d3_Busy && d3_Camera) {
        d3_Busy = 1;
        if (d3_Hardware)
            d3d_Clear(&d3_Viewport);
        else
            dd_Clear(&d3_Viewport, GetBValue(d3_fog_color.col), GetGValue(d3_fog_color.col), GetRValue(d3_fog_color.col));
        d3_LightKeysCulled = 0;
        d3_LightKeysVisible = 0;
        d3_MaxZ = 0.0f;
        d3_MinZ = 1e10f;
        D3_MESH(d3s_Object)->surf[0].info.ntris = 0;
        D3_MESH(d3s_Object)->surf[0].info.nkeys = 0;
        if (d3_Terrain) {
            if (d3_FogOn && d3_FogByDistance) {
                d = d3fl_FarDist(d3_Terrain, &D3_X(d3_Camera)->pos, &d3_Camera->matrix);
                if (d < d3_FogEnd2)
                    d3_SetFogRange((1.0f - (d3_FogEnd2 - d) / d3_FogEnd2) * d3_FogStart1, d);
                else
                    d3_SetFogRange(d3_FogStart1, d3_FogEnd2);
            }
            d3fl_Draw(d3_Terrain, flag);
        }
        d3_MirrorObject = 0;
        d3s_CollectLights();
        d3_ObjectsDrawn = 0;
        d3_ObjectsTested = 0;
        d3_ForEachObject(d3_DefaultScene, 0, mask, (D3OBJECTCB)d3_DrawObjectCB, 0);
        if (D3_MESH(d3s_Object)->surf[0].info.ntris > 0 && d3_ShadowMode.mode)
            ((D3OBJECTCB)d3_DrawObjectCB)(d3s_Object, 0);
        for (ps = d3_ParHead; ps != &d3_ParEnd; ps = next) {
            next = ps->next;
            par_Draw(ps);
        }
        if (d3_Sky)
            d3sky_Draw(d3_Sky);
        if (flag)
            d3_LightUpdate();
        if (d3d_Config.mirrors == 1 && d3_MirrorObject && d3_Mirror.info && d3_Mirror.planes && d3_Mirror.flags.visible)
            d3_ForEachObject(d3_DefaultScene, 0, mask, (D3OBJECTCB)d3_MirDrawObject, &d3_Mirror.obj);
        if (d3_MinZ == 1e10f)
            d3_MinZ = 0.0f;
        d3_StatPolys = d3_SortCount;
        d3_StatDrawnVerts = d3_DrawnVerts;
        d3_StatTris = d3_TriCount;
        d3_StatVerts = d3_VertCount;
        d3_StatShadowTris = d3s_Object->info->ntris;
        d3_DrawSorted();
        d3_StatTexBytes = d3_TextureBytes;
        d3_StatTexSwitches = d3_TextureSwitches;
        d3_Busy--;
    }
}
/* 0x43e4d0, animates the objects of the default scene (and of all other scenes) @deep s25: what the original does: d3_Fps60Time is read into esi (a leftover callee-saved register for the expression temp), then tb_Delay is loaded into esi after the sub, so fr's live range starts after dt's load; ours loads fr first (esi) and the Time temp gets edi. c2trace: dt has 8 refs in the control (the product dt * fr formed in dt's register, prio 86 -> ecx) and fr prio 30 -> esi. Every order with fr after dt or n (all 6 orders x fr * dt / dt * fr in each rate, 23 variants) is control or 51 ins: fr = tb_Delay after the Fps60Frames store is substituted (fr then = no-fr: tb_Delay a global candidate in ecx, dt 6 refs in eax); dt, fr, n keeps fr (5 refs) but the product goes to fr's register (dt 6 refs, eax). No fr (tb_Delay in both rates): 49 / 51. Left: code 8, regs 2 at 0x43e5c9 (which register the Time temp gets). Next: what makes fr a real local defined late with the product in dt's register (a kill between fr's def and its uses that is a statement of the original). prior: one code hunk at 0x43e5c9 (6 vs 6 instructions, same ops up to the temp register): original keeps the 0x6334a8 temp in esi and reuses esi for tb_Delay, ours loads tb_Delay into esi early and uses edi for the temp with inc/sub scheduled after - register allocation only, no missing/extra statement, constant, callee, type or field in the 96 instructions; statement orders fr,n,dt / n,fr,dt / fr,dt,n / dt,n,fr and a no-fr spelling (tb_Delay in both rates) tried, all worse or equal; Q2 cleared by the renames */

void d3_Animate(unsigned int mask, char anim, char tick, char all)
{
    unsigned long now;
    unsigned int flags, n, dt, fr;
    int i;
    D3OBJECT *o;

    if (d3_Busy < 0)
        d3_Busy = 0;
    if (d3_EngineReady && !d3_Busy && d3_Camera) {
        d3_Busy = 1;
        if (anim) {
            now = d3_Time;
            ts_UpdateScroll(now);
            flags = mask | 0x181;
            d3_ForEachObject(d3_DefaultScene, 0, flags, (D3OBJECTCB)d3anim_StepObject, (void *)now);
            if (all) {
                for (i = 0; i < D3_MAX_SCENES; i++) {
                    if (i != d3_CurrentScene && (o = ((D3SCENELISTS *)&d3_Scenes[i])->first) && o != &d3_ObjEnd) {
                        do {
                            d3_ForEachObject(d3_DefaultScene, o, flags, (D3OBJECTCB)d3anim_StepObject, (void *)(now | 0x80000000));
                            o = o->next;
                        } while (o != &d3_ObjEnd);
                    }
                }
            }
            fr = tb_Delay;
            n = ++d3_Fps60Frames;
            dt = now - d3_Fps60Time;
            if (dt > 60 && dt > 0) {
                d3_Fps60Frames = 0;
                d3_Fps60Time = now;
                d3_Fps60 = n * 1000 / (dt * fr);
            }
            n = ++d3_Fps10Frames;
            dt = now - d3_Fps10Time;
            if (dt > 10 && dt > 0) {
                d3_Fps10Frames = 0;
                d3_Fps10Time = now;
                d3_Fps10 = n * 1000 / (dt * fr);
            }
        }
        d3_EndFrame(tick);
        d3_Busy--;
    }
}
/* 0x43e680, one frame: renders, then animates, the objects the mask selects */

void d3_Frame(unsigned int mask, char flag, char tick)
{
    d3_Render(mask, flag);
    d3_Animate(mask, flag, tick, 1);
}
/* 0x5eba80, per triangle vertex i: [i][0] the next vertex, [i][1] the previous one (the edge opposite i);
   d3s.c and d3_sm.c read it, its .rdata lies between vfs.c's and d3_fl.c's, among this unit's constants */
const int d3_TriEdge[3][2] = { { 1, 2 }, { 2, 0 }, { 0, 1 } };

D3SCENE *d3_DefaultScene = d3_Scenes;  /* 0x5fb5b0, the current scene (d3.h) */
/* the initial D3D mode: bpp / mode / device flags, then d3if_SceneState0 (the last dword is that global,
   which the interface's scene select saves and restores; d3d_SetConfig reads only the first three) */
char d3_DefaultD3DConfig[] = { 94, 1, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 0 };  /* 0x5fb5b4 */
int d3if_SceneState1 = 15;  /* 0x5fb5c4, the scene pair's second dword (d3_interface.c's write-only state) */
char d3_EditorMode = 0;  /* 0x633438, editor / tool mode (reasoned guess: the ortho view cameras, the type-6 lights
                            and the on=0 relight; only ever set 0 in this build) */
int d3_FrameCount = 0;  /* 0x63343c */
int d3_UnreadBss1 = 0;  /* 0x633440, no surviving instruction or pointer refers to it: data of functions /OPT:REF removed; size from the neighbours' addresses */
int d3_CurrentScene = 0;  /* 0x633444 */
D3OBJECT * d3s_Object = 0;  /* 0x633448, object the shadow polygons go to */
int d3_MirrorObject = 0;  /* 0x63344c, the object the frame's mirror was built for (0 none) */
unsigned char d3_Hardware = 0;  /* 0x633450 */
unsigned char d3_EngineReady = 0;  /* 0x633451, engine up: set at the end of d3_Init, cleared in d3_Shutdown */
unsigned int d3_ClipOut = 0;  /* 0x633454, vertices in the clipped polygon */
unsigned int d3_ClipIn = 0;  /* 0x633458, vertices in the polygon to clip */
unsigned char d3_D3DActive = 0;  /* 0x63345c, a Direct3D device was created */
unsigned char d3_NoTextures = 0;  /* 0x63345d, textures off (ecmd.c's ocmd_SetNoTextures; d3_DrawSortedD3D tests the byte) */
int d3_UnreadBss2[2] = {0};  /* 0x633464, no surviving instruction or pointer refers to it: data of functions /OPT:REF removed; size from the neighbours' addresses */
int d3_ClipExtraTris = 0;  /* 0x63346c */
int d3_StatPolys = 0;  /* 0x633470, frame statistics */
int d3_StatTris = 0;  /* 0x633474 */
int d3_StatShadowTris = 0;  /* 0x633478 */
int d3_StatTexSwitches = 0;  /* 0x63347c */
int d3_StatTexBytes = 0;  /* 0x633480 */
int d3_StatVerts = 0;  /* 0x633484 */
int d3_StatDrawnVerts = 0;  /* 0x633488 */
unsigned int d3_Fps60 = 0;  /* 0x63348c, frames per second (two windows) */
unsigned int d3_Fps10 = 0;  /* 0x633490 */
int d3_ObjectsTested = 0;  /* 0x633494, objects tested / drawn */
int d3_ObjectsDrawn = 0;  /* 0x633498 */
unsigned int d3_SceneFlags = 0;  /* 0x63349c */
D3COLORU d3_fog_color = {0};  /* 0x6334a0, the fog colour (the name: d3d_Clear's D3DCHECK string "d3_fog_color.col") */
char d3_FogOn = 0;  /* 0x6334a4 */
unsigned char d3_FogByDistance = 0;  /* 0x6334a5, the byte right after d3_FogOn: when set, d3_Render clamps the fog end to the
                                        terrain's far distance (0x43e200); the scene setups of f3init.c / cm.c switch it off / on */
unsigned int d3_Fps60Time = 0;  /* 0x6334a8 */
unsigned int d3_Fps60Frames = 0;  /* 0x6334ac */
unsigned int d3_Fps10Time = 0;  /* 0x6334b0 */
unsigned int d3_Fps10Frames = 0;  /* 0x6334b4 */
extern D3PARTSYS d3_ParEnd;
/* d3_ParTail: view of d3_ParEnd (tools/data_views.py) */
/* d3_ParHead: view of d3if_ParList (the head node's next field) */
extern struct VFSDIR *d3io_StockDir;
float d3_ZFar;  /* 0xfc2564 */
D3ANIMDATA d3anim_Head; /* 0xfc2580, the head sentinel of the animation cache list (d3_anim.c): its next is the first animation */

float d3_VOfs;  /* 0xfc26ec */
D3OBJECT d3_ObjEnd;  /* 0xfc2700, list end marker */
/* d3_TopLast is d3_ObjEnd.prev (0xfc28f4 = 0xfc2700 + 0x1f4): a macro in d3_engine.h */
float d3_ScaleY;  /* 0xfc2944 */
float d3_FogScale;  /* 0xfc2948 */
float d3_Dist2;  /* 0xfc294c */
D3VECTOR4 d3_Frustum[5]; /* 0xfc2960, corner directions of the view, then (0, 0, 1) */
float d3_FogStart1; /* 0xfc29bc */
unsigned int d3_RadixCount[256];  /* 0xfc29c0 */
int d3_TextureBytes;  /* 0xfc2dc0, texture bytes used this frame */
int d3_VertCount;  /* 0xfc2dc4 */
D3SORTENT * d3_SortPtr;  /* 0xfc2dc8 */
unsigned int d3_SortCount;  /* 0xfc2dcc */
float d3_InvZFar;  /* 0xfc2dd4, 1 / d3_ZFar */
float d3_FogEnd;  /* 0xfc2dd8 */
float d3_ViewZScale;  /* 0xfc2ddc */
D3SORTENT * d3_SortBuf;  /* 0xfc2de0 */
D3OBJECT *d3_Camera;  /* 0xfc2dec, the camera object */
int d3_ScreenH;  /* 0xfc2df0 */
int d3_ScreenW;  /* 0xfc2df4 */
D3BGF d3_StockListStart;  /* 0xfc2e00, the head sentinel of the stock object list (D3BGF, d3_io_t.h; 0x20c bytes up to
                                   d3_FogStart): its next at 0xfc2ffc is the list's first stock object, its prev at 0xfc3000 */
float d3_FogStart;  /* 0xfc300c */
char d3_ClipInfo[64][0x68];  /* 0xfc3020 */
float d3_FovX;  /* 0xfc4a20 */
D3BGF d3_StockListEnd;  /* 0xfc4a40, the end sentinel of the stock object list (D3BGF, d3_io_t.h): d3io_LoadObjectFile links new ones in before it */
float d3_FovY;  /* 0xfc4c4c, half the vertical view angle (d3_FovX the horizontal one) */
D3CVERT * d3_ClipList[2][0x80];  /* 0xfc4c60, polygon vertex lists (ping-pong) */
D3CLIPSTATE d3_ClipState;  /* 0xfc5060, d3_ClipNone, d3_ClipFlagA / B and the frustum planes d3_Planes (d3_engine.h) */
D3SORTENT * d3_RadixDst;  /* 0xfc50c8 */
int d3s_EdgeX[4];  /* 0xfc50d0; d3s.c's edge table (d3s.c declares it extern), placed with d3_engine's commons */
int d3s_EdgeY[4];  /* 0xfc50e0; with d3s_EdgeX */
D3OBJECT d3_TopHead;  /* 0xfc5100, list start marker */
/* d3_TopFirst is d3_TopHead.next (0xfc52f0 = 0xfc5100 + 0x1f0): a macro in d3_engine.h */
float d3_ViewY;  /* 0xfc5328 */
float d3_ZScale;  /* 0xfc532c */
float d3_FogStart2;  /* 0xfc5330, squared */
float d3_MaxZ;  /* 0xfc5338 */
float d3_FogEnd2; /* 0xfc5340 */
float d3_MinZ;  /* 0xfc5344 */
float d3_ScaleX;  /* 0xfc5348, projection */
D3CVERT d3_ClipPool[128];  /* 0xfc5360, vertices made by the clipper */
int d3_TriCount;  /* 0xfc7b60 */
float d3_ZOfs;  /* 0xfc7b64 */
unsigned char d3_Noise[256][256];  /* 0xfc7b80 */
int d3_ViewBottomFx;  /* 0xfe73a4 */
float d3_ViewX;  /* 0xfe73a8 */
float d3_UOfs;  /* 0xfe73ac */
float d3_ZNear;  /* 0xfe73b4 */
float d3_SavedZFar;  /* 0xfe73bc, d3_ZFar while the fog is off */
D3SORTENT * d3_RadixSrc;  /* 0xfe73c0 */
float d3_ViewH;  /* 0xfe73c4 */
float d3_ViewW;  /* 0xfe73c8 */
D3INITCFG d3_Cfg;  /* 0xfe73e0 */
D3ANIMDATA d3anim_Tail; /* 0xfe7440, the end sentinel of the animation cache list (d3_anim.c): new animations are linked in before it */
int d3_DrawnVerts;  /* 0xfe75b0, vertices drawn this frame (d3_DrawSorted resets it) */
int d3_TextureSwitches;  /* 0xfe75b4 */
float d3_HalfH;  /* 0xfe75b8 */
float d3_HalfAngleY;  /* 0xfe75bc */
D3SORTENT * d3_SortTmp;  /* 0xfe75c0 */
D3TRIFUNC d3_TriFuncs[6];  /* 0xfe75e0, software triangle fillers by mode */
float d3_HalfW;  /* 0xfe75f8 */
float d3_CenterX;  /* 0xfe75fc */
float d3_HalfAngleX;  /* 0xfe7600 */
float d3_CenterY;  /* 0xfe7604 */
D3RECT d3_Viewport;  /* 0xfc2930 */
unsigned char * d3s_Dst;  /* 0xfc2940 */
int d3s_EdgeC[3];  /* 0xfc29b0 */
int d3s_SlopeC;  /* 0xfc2dd0, slope of the interpolated value along the left edge */
int d3s_SlopeL;  /* 0xfc2de4 */
int d3s_CL;  /* 0xfc2de8, interpolated value at the left edge */
int d3s_SlopeR;  /* 0xfc2df8 */
int d3s_XL;  /* 0xfc5334 */
int d3s_XR;  /* 0xfc533c */
D3SCENE d3_Scenes[D3_MAX_SCENES]; /* 0xfd7ba0 */
int d3s_SpanLen;  /* 0xfe73a0 */
unsigned short * d3s_Dst16;  /* 0xfe73b0 */
int d3s_Area;  /* 0xfe73b8 */
int d3s_Fill;  /* 0xfe75ac */
/* d3_ShadowMode: view of d3d_Config (tools/data_views.py) */
