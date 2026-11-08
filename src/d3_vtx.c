/* d3_vtx.c -- 0x458e70..0x459d80, original file name unknown. Names are ours.
   Mesh vertex animation: key-frame blend weights, vertex and normal transforms into camera space,
   env-map coordinates, bounding box corners.
   Open: the file name, and whether it is one TU with d3_pose.c (the join compiles unchanged).
   unit of rea 8cef7d5b (src/game/unk_4591a0.c); boundaries in decl/text.dcl */
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_anim.h"
#include "d3_light.h"
#include "d3_vtx.h"
#include "bio.h"                      /* ai_FloatOne */

/* d3_EnvMapCoords (was d3_pose.c: only 0x459300 calls it) works on the pose views of d3_pose.h; its
   prototypes there are for the other file. */
#include "d3_pose.h"
#include "d3_engine.h"
#include "d3_vec.h"

/* 0x458e70, environment map coordinates of the vertices of a part that has a reflecting texture */
void d3_EnvMapCoords(XOBJ *obj, XPART *part)
{
    D3MATRIX m;
    D3VECTOR4 b;
    struct { float x, y; volatile float z; float w; } nv;
    volatile float d;
    int n, i, k;
    D3ANIMSLOT *slot;
    D3ANIMFRAME *fr;
    XVERT *v;

    if (obj && part && part->ops && (n = part->ops->count) > 0) {
        for (i = 0; i < n; i++) {
            if (part->tex[i] && (part->tex[i]->flags & 1))
                break;
        }
        if (i < n) {
            d3_WorldMatrix((D3OBJECT *)obj, (obj->flags & 0x80) ? 0 : d3_Camera, 1, &m);
            slot = d3anim_GetMainSlot(part);
            if (slot && (fr = &slot->data->frames[slot->frame]) != 0) {
                v = part->keys;
                for (i = 0; i < part->nkeys; i++, v++) {
                    if (v->used) {
                        b.x = ((float)fr->normals[i * 3] - 128.0f) * 0.007843138f;
                        b.y = ((float)fr->normals[i * 3 + 1] - 128.0f) * 0.007843138f;
                        b.z = ((float)fr->normals[i * 3 + 2] - 128.0f) * 0.007843138f;
                        nv.x = m._11 * b.x + m._21 * b.y + m._31 * b.z;
                        nv.y = m._12 * b.x + m._22 * b.y + m._32 * b.z;
                        nv.z = m._13 * b.x + m._23 * b.y + m._33 * b.z;
                        d = (nv.x * v->x + nv.y * v->y + nv.z * v->z) * -2.0f;
                        b.x = d * nv.x + v->x;
                        b.y = d * nv.y + v->y;
                        b.z = nv.z * d + v->z;
                        d3_Normalize(&b);
                        v->u = (b.x + ai_FloatOne) * 0.5f;
                        v->v = (b.y + ai_FloatOne) * 0.5f;
                    }
                }
                return;
            }
            v = part->keys;
            for (i = 0; i < part->nkeys; i++, v++) {
                if (v->used) {
                    nv.x = (m._11 * v->src->nx) + m._31 * v->src->nz + m._21 * v->src->ny;
                    nv.y = (m._12 * v->src->nx) + m._32 * v->src->nz + m._22 * v->src->ny;
                    nv.z = (m._13 * v->src->nx) + m._33 * v->src->nz + m._23 * v->src->ny;
                    d = (nv.x * v->x + nv.y * v->y + nv.z * v->z) * -2.0f;
                    b.x = d * nv.x + v->x;
                    b.y = d * nv.y + v->y;
                    b.z = nv.z * d + v->z;
                    d3_Normalize(&b);
                    v->u = (b.x + ai_FloatOne) * 0.5f;
                    v->v = (b.y + ai_FloatOne) * 0.5f;
                }
            }
        }
    }
}

/* the other files see D3OBJECT / D3LINFO */
#define D3_ENVMAPCOORDS(o, p) d3_EnvMapCoords((XOBJ *)(o), (XPART *)(p))

/* 0x4591a0, blend weights of the two key frames around the current time of an animation slot */
#define UNK64(s) (*(float *)&(s)->time)

/* 0x4591a0 */
void d3_AnimBlendWeights(D3ANIMSLOT *s, float *a, float *b)
{
    D3ANIMDATA *d = s->data;
    int n;
    float t;
    volatile float r;

    if (s->flags.b[1] & 2) {
        t = s->subFrame * (1.0f / d->frames[s->nextFrame].count);
        if (s->flags.b[1] & 4) {
            n = d->nframes;
            if (n <= 2 && !(0 < s->nextFrame)) {
                t = ((float)cos(t * 3.1415927f) + 1.0f) * 0.5f;
                t = 1.0f - t;
            } else if (!(0 < s->nextFrame)) {
                t = (float)cos(t * 1.5707964f);
                t = 1.0f - t;
            } else if (s->frame >= n - 1) {
                t = (float)cos((t + 1.0f) * 1.5707964f) + 1.0f;
                t = 1.0f - t;
            }
        }
        r = t - (1.0f / d->frames[s->nextFrame].count) * ((float *)s)[25];
        /* the non-volatile read is needed: it makes VC6 load 1.0f first (fld 1.0f; fsub [r]) */
        *a = 1.0f - *(float *)&r;
        *b = r;
    } else {
        t = s->subFrame * (1.0f / d->frames[s->frame].count);
        if (s->flags.b[1] & 4) {
            n = d->nframes;
            if (n <= 2 && s->frame + 1 >= n - 1) {
                t = ((float)cos(t * 3.1415927f) + 1.0f) * 0.5f;
                t = 1.0f - t;
            } else if (s->nextFrame >= n - 1) {
                t = (float)cos((t + 1.0f) * 1.5707964f) + 1.0f;
                t = 1.0f - t;
            } else if (!(0 < s->frame)) {
                t = (float)cos(t * 1.5707964f);
                t = 1.0f - t;
            }
        }
        t = (1.0f / d->frames[s->frame].count) * ((float *)s)[25] + t;
        r = 1.0f - t;
        *a = t;
        *b = r;
    }
}

/* 0x459300, transforms the vertices of a mesh into camera space with the matrix m. An animated mesh (3 slots, each
   with two key frames) first blends the packed positions of the frames in use; the vertices then also add the
   source point. With D3OBJECT::objectFlags bit 5 the nearest and farthest z are tracked. */
typedef struct D3MVEC {             /* a 16-byte vector local; only z goes through memory */
    float x, y;
    volatile float z;
    float w;
} D3MVEC;

typedef struct D3MVEC2 {
    float x;
    volatile float y, z;
    float w;
} D3MVEC2;

/* 0x459300 */
void d3_TransformVerts(D3OBJECT *obj, D3LINFO *info, D3MATRIX *m)
{
    D3ANIMCTL *ctl = (D3ANIMCTL *)info;
    D3LVERT *v;
    D3VECTOR *sp;
    D3ANIMSLOT *sl;
    D3ANIMFRAME *f1, *f2;
    unsigned char *p1, *p2;
    float a, b;
    D3MVEC s1, b1, s2, b2, q1;
    D3MVEC2 q2;
    float z;
    float vx, vy, vz;
    int i, k;
    char first;

    if (ctl->active) {
        first = 1;
        for (k = 0; k < 3; k++) {
            sl = &ctl->slots[k];
            if (sl->data && sl->weight != 0.0f) {
                f1 = &sl->data->frames[sl->frame];
                f2 = &sl->data->frames[sl->nextFrame];
                p1 = f1->packed;
                p2 = f2->packed;
                /* scale / base of the two frames, pre-multiplied by the slot weight; the statement order (z copies
                   are volatile stores, x / y pairs share the FPU loads) is what the bytes need (hill climb) */
                s1.x = f1->scale.x;
                s1.y = f1->scale.y;
                s1.x = *(volatile float *)&sl->weight * s1.x;
                s1.z = f1->scale.z;
                s1.y = *(volatile float *)&sl->weight * s1.y;
                s2.z = f2->scale.z;
                b2.z = f2->base.z;
                s1.z = *(volatile float *)&sl->weight * s1.z;
                b1.x = f1->base.x;
                b1.y = f1->base.y;
                b1.x = *(volatile float *)&sl->weight * b1.x;
                b1.y = *(volatile float *)&sl->weight * b1.y;
                b1.z = f1->base.z;
                b1.z = *(volatile float *)&sl->weight * b1.z;
                s2.x = f2->scale.x;
                s2.y = f2->scale.y;
                s2.x = *(volatile float *)&sl->weight * s2.x;
                s2.y = *(volatile float *)&sl->weight * s2.y;
                s2.z = *(volatile float *)&sl->weight * s2.z;
                b2.x = f2->base.x;
                b2.y = f2->base.y;
                b2.x = *(volatile float *)&sl->weight * b2.x;
                b2.y = *(volatile float *)&sl->weight * b2.y;
                b2.z = *(volatile float *)&sl->weight * b2.z;
                d3_AnimBlendWeights(sl, &a, &b);
                v = info->keys;
                if (first) {
                    for (i = 0; i < info->nkeys; i++, p1 += 3, p2 += 3, v++) {
                        q1.x = ((float)p1[0] * s1.x + b1.x) * b;
                        q1.y = ((float)p1[1] * s1.y + b1.y) * b;
                        q1.z = ((float)p1[2] * s1.z + b1.z) * b;
                        q2.x = ((float)p2[0] * s2.x + b2.x) * a;
                        q2.y = ((float)p2[1] * s2.y + b2.y) * a;
                        q2.z = ((float)p2[2] * s2.z + b2.z) * a;
                        v->y = q2.y + q1.y;
                        v->x = q2.x + q1.x;
                        v->z = q2.z + q1.z;
                    }
                    first = 0;
                } else {
                    for (i = 0; i < info->nkeys; i++, p1 += 3, p2 += 3, v++) {
                        q1.x = ((float)p1[0] * s1.x + b1.x) * b;
                        q1.y = ((float)p1[1] * s1.y + b1.y) * b;
                        q1.z = ((float)p1[2] * s1.z + b1.z) * b;
                        q2.x = ((float)p2[0] * s2.x + b2.x) * a;
                        q2.y = ((float)p2[1] * s2.y + b2.y) * a;
                        q2.z = ((float)p2[2] * s2.z + b2.z) * a;
                        q1.x = q2.x + q1.x;
                        q1.y = q2.y + q1.y;
                        q1.z = q2.z + q1.z;
                        v->x += q1.x;
                        v->y += q1.y;
                        v->z += q1.z;
                    }
                }
            }
        }
        i = 0;
        if (obj->objectFlags & 0x20) {
            v = info->keys;
            if (0 < info->nkeys) {
                do {
                    sp = v->src;
                    vx = sp->x + v->x;
                    vy = sp->y + v->y;
                    vz = sp->z + v->z;
                    v->x = m->_11 * vx + vy * m->_21 + m->_31 * vz + m->_41;
                    v->y = (vz * m->_32) + vy * m->_22 + vx * m->_12 + m->_42;
                    z = (vz * m->_33) + vy * m->_23 + vx * m->_13 + m->_43;
                    v->z = z;
                    if (!(d3_MinZ < z))
                        d3_MinZ = z;
                    if (!(z < d3_MaxZ))
                        d3_MaxZ = *(volatile float *)&v->z; /* matching: re-read of the stored z (cuts 12 differing lines; the original copies it through the FPU) */
                    i++;
                    v++;
                } while (i < info->nkeys);
                D3_ENVMAPCOORDS(obj, info);
                return;
            }
        } else {
            v = info->keys;
            if (0 < info->nkeys) {
                do {
                    sp = v->src;
                    vx = sp->x + v->x;
                    vy = sp->y + v->y;
                    vz = sp->z + v->z;
                    v->x = vx * m->_11 + vz * m->_31 + vy * m->_21 + m->_41;
                    v->y = vz * m->_32 + vy * m->_22 + vx * m->_12 + m->_42;
                    v->z = vz * m->_33 + vy * m->_23 + vx * m->_13 + m->_43;
                    i++;
                    v++;
                } while (i < info->nkeys);
                D3_ENVMAPCOORDS(obj, info);
                return;
            }
        }
    } else {
        v = info->keys;
        i = 0;
        if (obj->objectFlags & 0x20) {
            if (0 < info->nkeys) {
                do {
                    sp = v->src;
                    v->x = sp->z * m->_31 + sp->y * m->_21 + sp->x * m->_11 + m->_41;
                    v->y = sp->x * m->_12 + m->_22 * sp->y + sp->z * m->_32 + m->_42;
                    z = m->_33 * sp->z + m->_23 * sp->y + sp->x * m->_13 + m->_43;
                    v->z = z;
                    if (!(d3_MinZ < z))
                        d3_MinZ = z;
                    if (!(z < d3_MaxZ))
                        d3_MaxZ = *(volatile float *)&v->z; /* matching: re-read of the stored z (cuts 12 differing lines; the original copies it through the FPU) */
                    i++;
                    v++;
                } while (i < info->nkeys);
                D3_ENVMAPCOORDS(obj, info);
                return;
            }
        } else if (0 < info->nkeys) {
            do {
                sp = v->src;
                v->x = sp->z * m->_31 + sp->y * m->_21 + sp->x * m->_11 + m->_41;
                v->y = sp->x * m->_12 + m->_22 * sp->y + sp->z * m->_32 + m->_42;
                v->z = m->_33 * sp->z + m->_23 * sp->y + sp->x * m->_13 + m->_43;
                i++;
                v++;
            } while (i < info->nkeys);
        }
    }
    D3_ENVMAPCOORDS(obj, info);
}
/* 0x4598d0, D3LOPS::fn1ec of the stock mesh driver (d3_io_t.c): the normals of a mesh without key frames, from the
   normal of each vertex's source point (a D3VECTOR at 0x0c of the source) in camera space */

void d3_TransformNormals(D3OBJECT *obj, D3LINFO *info)
{
    D3MATRIX m;
    D3LVERT *v;
    float *n;
    int i;

    d3_WorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 0, &m);
    v = info->keys;
    for (i = 0; i < info->nkeys; i++, v++) {
        n = (float *)v->src;
        v->nx = m._11 * n[3] + m._21 * n[4] + m._31 * n[5];
        v->ny = m._12 * n[3] + m._22 * n[4] + m._32 * n[5];
        v->nz = m._13 * n[3] + m._23 * n[4] + m._33 * n[5];
    }
}
/* 0x459980, normals of the object from the packed normals of key frame f (3 bytes per vertex) in camera space;
   without a frame the driver does it. Not the matrix d3_light.h declares for the third argument: it is a D3ANIMFRAME. */

void d3_FrameNormals(D3OBJECT *obj, D3LINFO *info, D3ANIMFRAME *f)
{
    D3MATRIX m;
    D3LVERT *v;
    float x, y, z;
    int i;

    if (!f) {
        info->ops->fn1ec(obj, (D3OBJINFO *)info);
        return;
    }
    d3_WorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 1, &m);
    v = info->keys;
    for (i = 0; i < info->nkeys; i++, v++) {
        x = (f->normals[i * 3] - 128.0f) * 0.007843138f;
        y = (f->normals[i * 3 + 1] - 128.0f) * 0.007843138f;
        z = (f->normals[i * 3 + 2] - 128.0f) * 0.007843138f;
        v->nx = m._11 * x + m._21 * y + m._31 * z;
        v->ny = m._12 * x + m._22 * y + m._32 * z;
        v->nz = m._13 * x + m._23 * y + m._33 * z;
    }
}
/* 0x459ab0, transforms the 8 corners of the bounding box that follow the vertices of the mesh by the matrix m,
   from the corners of the source (no animation) or of the union of the boxes of the key frames in use.
   Returns the corners (0 if the mesh has no vertices). info is a D3ANIMCTL (d3_anim.h) here. */

D3BOXVERT *d3_GetBoxVerts(D3OBJECT *obj, D3OBJINFO *info, void *unk)
{
    D3ANIMCTL *ctl = (D3ANIMCTL *)info;
    D3MATRIX *m = (D3MATRIX *)unk;
    D3LVERT *v;
    D3VECTOR4 mn, mx;
    D3VECTOR4 c[8];
    D3ANIMFRAME *fr;
    int i;
    int n = info->nkeys;

    if (n == 0)
        return 0;
    {
        if (ctl->active) {
            mn.x = 1e10f;
            mn.y = 1e10f;
            mn.z = 1e10f;
            mx.x = -1e10f;
            mx.y = -1e10f;
            mx.z = -1e10f;
            for (i = 0; i < 3; i++) {
                if (ctl->slots[i].data) {
                    fr = &ctl->slots[i].data->frames[ctl->slots[i].frame];
                    mn.x = min(mn.x, fr->min.x);
                    mn.y = min(mn.y, fr->min.y);
                    mn.z = min(mn.z, fr->min.z);
                    mx.x = max(mx.x, fr->max.x);
                    mx.y = max(mx.y, fr->max.y);
                    mx.z = max(mx.z, fr->max.z);
                }
            }
            c[0].x = mn.x; c[0].y = mn.y; c[0].z = mn.z;
            c[1].x = mx.x; c[1].y = mn.y; c[1].z = mn.z;
            c[2].x = mn.x; c[2].y = mx.y; c[2].z = mn.z;
            c[3].x = mx.x; c[3].y = mx.y; c[3].z = mn.z;
            c[4].x = mn.x; c[4].y = mn.y; c[4].z = mx.z;
            c[5].x = mx.x; c[5].y = mn.y; c[5].z = mx.z;
            c[6].x = mn.x; c[6].y = mx.y; c[6].z = mx.z;
            c[7].x = mx.x; c[7].y = mx.y; c[7].z = mx.z;
            v = (D3LVERT *)info->keys + n;
            for (i = 0; i < 8; i++, v++) {
                v->x = c[i].z * m->_31 + c[i].y * m->_21 + c[i].x * m->_11 + m->_41;
                v->y = c[i].z * m->_32 + c[i].y * m->_22 + c[i].x * m->_12 + m->_42;
                v->z = c[i].z * m->_33 + c[i].y * m->_23 + c[i].x * m->_13 + m->_43;
            }
    
        } else {
            v = (D3LVERT *)info->keys + n;
            for (i = 0; i < 8; i++, v++) {
                v->x = v->src->y * m->_21 + m->_11 * v->src->x + v->src->z * m->_31 + m->_41;
                v->y = v->src->y * (m->_22) + (m->_32) * (v->src->z) + (m->_12) * v->src->x + m->_42;
                v->z = v->src->y * m->_23 + m->_33 * (v->src->z) + m->_13 * v->src->x + m->_43;
            }
        }
        return (D3BOXVERT *)((D3LVERT *)info->keys + info->nkeys);
    }
}
