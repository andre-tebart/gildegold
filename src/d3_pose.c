/* d3_pose.c -- object pose helpers, 0x458810..0x458e70 (the quad tree zones before them are in d3_quad.c).
   Original file name unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/unk_458810.c); boundaries in decl/text.dcl */
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_pose.h"
#include "d3_vec.h"

/* 0x458810, rotates a direction by the object's matrix and then by those of its parents */
void d3_DirToWorld(D3OBJECT *obj, D3VECTOR *in, D3VECTOR *out)
{
    D3OBJECT *p;
    struct { float x, y; volatile float z; float w; } t;

    p = obj->parent;
    out->x = obj->matrix._31 * in->z + obj->matrix._11 * in->x + obj->matrix._21 * in->y;
    out->y = obj->matrix._32 * in->z + obj->matrix._12 * in->x + obj->matrix._22 * in->y;
    out->z = obj->matrix._33 * in->z + obj->matrix._13 * in->x + obj->matrix._23 * in->y;
    for (; p; p = p->parent) {
        t.x = p->matrix._21 * out->y + p->matrix._31 * out->z + p->matrix._11 * out->x;
        t.y = p->matrix._22 * out->y + p->matrix._32 * out->z + p->matrix._12 * out->x;
        t.z = p->matrix._23 * out->y + p->matrix._33 * out->z + p->matrix._13 * out->x;
        out->x = t.x;
        out->z = t.z;
        out->y = t.y;
    }
}
/* 0x458910, angles of the object's world matrix */

void d3_WorldAngles(D3OBJECT *obj, D3VECTOR4 *angles)
{
    D3MATRIX m;

    d3_WorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 1, &m);
    d3_MatToAngles((D3VECTOR4 *)&m, angles);
}
/* 0x458950, a direction in the object's space, relative to ref unless the object is in world space */

void d3_DirToRef(D3OBJECT *obj, D3OBJECT *ref, D3VECTOR *in, D3VECTOR *out)
{
    D3VECTOR4 t;

    if (!(obj->state & 0x80) && ref) {
        d3_DirToWorld(obj, in, (D3VECTOR *)&t);
        out->x = t.x * ref->matrix._11 + t.y * ref->matrix._21 + t.z * ref->matrix._31;
        out->y = t.x * ref->matrix._12 + t.y * ref->matrix._22 + t.z * ref->matrix._32;
        out->z = t.x * ref->matrix._13 + t.y * ref->matrix._23 + t.z * ref->matrix._33;
        return;
    }
    d3_DirToWorld(obj, in, out);
}
/* 0x458a20, a point of the object in world space */

void d3_PointToWorld(D3OBJECT *obj, D3VECTOR *in, D3VECTOR *out)
{
    D3OBJECT *p;
    float x, y, z;

    out->x = obj->posOffset.x + in->x;
    out->y = obj->posOffset.y + in->y;
    out->z = obj->posOffset.z + in->z;
    for (p = obj->parent; p; p = p->parent) {
        x = p->pivot.x + out->x;
        y = p->pivot.y + out->y;
        z = p->pivot.z + out->z;
        out->x = x * p->matrix._11 + y * p->matrix._21 + z * p->matrix._31;
        out->y = x * p->matrix._12 + y * p->matrix._22 + z * p->matrix._32;
        out->z = x * p->matrix._13 + y * p->matrix._23 + z * p->matrix._33;
        out->x = out->x - p->pivot.x;
        out->y = out->y - p->pivot.y;
        out->z = out->z - p->pivot.z;
        out->x = p->pos.x + out->x;
        out->y = p->pos.y + out->y;
        out->z = p->pos.z + out->z;
        out->x = p->posOffset.x + out->x;
        out->y = p->posOffset.y + out->y;
        out->z = p->posOffset.z + out->z;
    }
}
/* 0x458b20, d3_PointToWorld relative to ref unless the object is in world space */

void d3_PointToRef(D3OBJECT *obj, D3OBJECT *ref, D3VECTOR *in, D3VECTOR *out)
{
    D3VECTOR4 t;

    if (!(obj->state & 0x80) && ref) {
        d3_PointToWorld(obj, in, (D3VECTOR *)&t);
        t.x = t.x - ref->pos.x;
        t.y = t.y - ref->pos.y;
        t.z = t.z - ref->pos.z;
        t.x = t.x - ref->posOffset.x;
        t.y = t.y - ref->posOffset.y;
        t.z = t.z - ref->posOffset.z;
        out->x = t.x * ref->matrix._11 + t.y * ref->matrix._21 + t.z * ref->matrix._31;
        out->y = t.x * ref->matrix._12 + t.y * ref->matrix._22 + t.z * ref->matrix._32;
        out->z = t.x * ref->matrix._13 + t.y * ref->matrix._23 + t.z * ref->matrix._33;
        return;
    }
    d3_PointToWorld(obj, in, out);
}
/* 0x458c10, a point local to the object (before its own matrix) in world space: applies the object matrix and then the parent chain */

void d3_LocalToWorld(D3OBJECT *obj, D3VECTOR *in, D3VECTOR *out)
{
    D3VECTOR4 t;
    float x = in->x + obj->pivot.x;
    float y = in->y + obj->pivot.y;
    float z = in->z + obj->pivot.z;

    t.x = x * obj->matrix._11 + y * obj->matrix._21 + z * obj->matrix._31;
    t.y = x * obj->matrix._12 + y * obj->matrix._22 + z * obj->matrix._32;
    t.z = x * obj->matrix._13 + y * obj->matrix._23 + z * obj->matrix._33;
    t.x = t.x - obj->pivot.x;
    t.y = t.y - obj->pivot.y;
    t.z = t.z - obj->pivot.z;
    t.x = t.x + obj->pos.x;
    t.y = t.y + obj->pos.y;
    t.z = t.z + obj->pos.z;
    d3_PointToWorld(obj, (D3VECTOR *)&t, out);
}
/* 0x458ce0, a matrix in the object's space in world space: the object's matrix, and then those of its
   parents, multiplied onto m. Side effects: rewrites the translation row of obj->matrix; with orientOnly 0 it
   also offsets m's translation row by the object's pivot (orientOnly 1: rotation only, no translation). */

void d3_MatToWorld(D3OBJECT *obj, char orientOnly, D3MATRIX *m, D3MATRIX *out)
{
    D3MATRIX t;
    D3OBJECT *p = obj->parent;

    if (orientOnly) {
        obj->matrix._41 = 0;
        obj->matrix._42 = 0;
        obj->matrix._43 = 0;
    } else {
        m->_41 = m->_41 + obj->pivot.x;
        m->_42 = m->_42 + obj->pivot.y;
        m->_43 = m->_43 + obj->pivot.z;
        obj->matrix._41 = (obj->pos.x - obj->pivot.x) + obj->posOffset.x;
        obj->matrix._42 = (obj->pos.y - obj->pivot.y) + obj->posOffset.y;
        obj->matrix._43 = (obj->pos.z - obj->pivot.z) + obj->posOffset.z;
    }
    if (p) {
        d3_MatMul43(m, &obj->matrix, &t);
        d3_MatToWorld(p, orientOnly, &t, out);
        return;
    }
    d3_MatMul43(m, &obj->matrix, out);
}
/* 0x458da0, the world matrix of obj, in ref's space when ref is given (orientOnly 1: rotation only, no
   translation) */

void d3_WorldMatrix(D3OBJECT *obj, D3OBJECT *ref, char orientOnly, D3MATRIX *out)
{
    D3MATRIX t, id;

    d3_MatIdentity(&id);
    if (ref) {
        d3_MatToWorld(obj, orientOnly, &id, &t);
        t._41 = t._41 - ref->pos.x;
        t._42 = t._42 - ref->pos.y;
        t._43 = t._43 - ref->pos.z;
        t._41 = t._41 - ref->posOffset.x;
        t._42 = t._42 - ref->posOffset.y;
        t._43 = t._43 - ref->posOffset.z;
        d3_MatMul43(&t, &ref->matrix, out);
        return;
    }
    d3_MatToWorld(obj, orientOnly, &id, out);
}
