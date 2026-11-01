/* unk_458810.c -- object pose helpers, 0x458810..0x458e70 (the quad tree zones before them are in d3_quad.c).
   Original file name unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/unk_458810.c); boundaries in decl/text.dcl */
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "unk_458810.h"
#include "d3_vec.h"

/* 0x458810, rotates a direction by the object's matrix and then by those of its parents */
void d3_DirToWorld(XOBJ *obj, float *in, float *out)
{
    XOBJ *p;
    struct { float x, y; volatile float z; float w; } t;

    p = obj->parent;
    out[0] = obj->m._31 * in[2] + obj->m._11 * in[0] + obj->m._21 * in[1];
    out[1] = obj->m._32 * in[2] + obj->m._12 * in[0] + obj->m._22 * in[1];
    out[2] = obj->m._33 * in[2] + obj->m._13 * in[0] + obj->m._23 * in[1];
    for (; p; p = p->parent) {
        t.x = p->m._21 * out[1] + p->m._31 * out[2] + p->m._11 * out[0];
        t.y = p->m._22 * out[1] + p->m._32 * out[2] + p->m._12 * out[0];
        t.z = p->m._23 * out[1] + p->m._33 * out[2] + p->m._13 * out[0];
        out[0] = t.x;
        out[2] = t.z;
        out[1] = t.y;
    }
}
/* 0x458910, angles of the object's world matrix */

void d3_WorldAngles(XOBJ *obj, D3VECTOR4 *angles)
{
    D3MATRIX m;

    d3_WorldMatrix(obj, (obj->flags & 0x80) ? 0 : (XOBJ *)d3_Camera, 1, &m);
    d3_MatToAngles((D3VECTOR4 *)&m, angles);
}
/* 0x458950, a direction in the object's space, relative to ref unless the object is in world space */

void d3_DirToRef(XOBJ *obj, XOBJ *ref, float *in, float *out)
{
    D3VECTOR4 t;

    if (!(obj->flags & 0x80) && ref) {
        d3_DirToWorld(obj, in, &t.x);
        out[0] = t.x * ref->m._11 + t.y * ref->m._21 + t.z * ref->m._31;
        out[1] = t.x * ref->m._12 + t.y * ref->m._22 + t.z * ref->m._32;
        out[2] = t.x * ref->m._13 + t.y * ref->m._23 + t.z * ref->m._33;
        return;
    }
    d3_DirToWorld(obj, in, out);
}
/* 0x458a20, a point of the object in world space */

void d3_PointToWorld(D3OBJECT *obj_, D3VECTOR *in, D3VECTOR *out)
{
    XOBJ *obj = (XOBJ *)obj_;
    XOBJ *p;
    float x, y, z;

    out->x = obj->p78.x + in->x;
    out->y = obj->p78.y + in->y;
    out->z = obj->p78.z + in->z;
    for (p = obj->parent; p; p = p->parent) {
        x = p->p6c.x + out->x;
        y = p->p6c.y + out->y;
        z = p->p6c.z + out->z;
        out->x = x * p->m._11 + y * p->m._21 + z * p->m._31;
        out->y = x * p->m._12 + y * p->m._22 + z * p->m._32;
        out->z = x * p->m._13 + y * p->m._23 + z * p->m._33;
        out->x = out->x - p->p6c.x;
        out->y = out->y - p->p6c.y;
        out->z = out->z - p->p6c.z;
        out->x = p->p4c.x + out->x;
        out->y = p->p4c.y + out->y;
        out->z = p->p4c.z + out->z;
        out->x = p->p78.x + out->x;
        out->y = p->p78.y + out->y;
        out->z = p->p78.z + out->z;
    }
}
/* 0x458b20, d3_PointToWorld relative to ref unless the object is in world space */

void d3_PointToRef(XOBJ *obj, XOBJ *ref, float *in, float *out)
{
    D3VECTOR4 t;

    if (!(obj->flags & 0x80) && ref) {
        d3_PointToWorld((D3OBJECT *)obj, (D3VECTOR *)in, &t);
        t.x = t.x - ref->p4c.x;
        t.y = t.y - ref->p4c.y;
        t.z = t.z - ref->p4c.z;
        t.x = t.x - ref->p78.x;
        t.y = t.y - ref->p78.y;
        t.z = t.z - ref->p78.z;
        out[0] = t.x * ref->m._11 + t.y * ref->m._21 + t.z * ref->m._31;
        out[1] = t.x * ref->m._12 + t.y * ref->m._22 + t.z * ref->m._32;
        out[2] = t.x * ref->m._13 + t.y * ref->m._23 + t.z * ref->m._33;
        return;
    }
    d3_PointToWorld((D3OBJECT *)obj, (D3VECTOR *)in, (D3VECTOR *)out);
}
/* 0x458c10, a point local to the object (before its own matrix) in world space: applies the object matrix and then the parent chain */

void d3_LocalToWorld(XOBJ *obj, float *in, float *out)
{
    D3VECTOR4 t;
    float x = in[0] + obj->p6c.x;
    float y = in[1] + obj->p6c.y;
    float z = in[2] + obj->p6c.z;

    t.x = x * obj->m._11 + y * obj->m._21 + z * obj->m._31;
    t.y = x * obj->m._12 + y * obj->m._22 + z * obj->m._32;
    t.z = x * obj->m._13 + y * obj->m._23 + z * obj->m._33;
    t.x = t.x - obj->p6c.x;
    t.y = t.y - obj->p6c.y;
    t.z = t.z - obj->p6c.z;
    t.x = t.x + obj->p4c.x;
    t.y = t.y + obj->p4c.y;
    t.z = t.z + obj->p4c.z;
    d3_PointToWorld((D3OBJECT *)obj, (D3VECTOR *)&t, (D3VECTOR *)out);
}
/* 0x458ce0, a matrix in the object's space in world space: the object's matrix, and then those of its
   parents, multiplied onto m. Side effects: rewrites the translation row of obj->m; with orientOnly 0 it
   also offsets m's translation row by the object's p6c (orientOnly 1: rotation only, no translation). */

void d3_MatToWorld(XOBJ *obj, char orientOnly, D3MATRIX *m, D3MATRIX *out)
{
    D3MATRIX t;
    XOBJ *p = obj->parent;

    if (orientOnly) {
        obj->m._41 = 0;
        obj->m._42 = 0;
        obj->m._43 = 0;
    } else {
        m->_41 = m->_41 + obj->p6c.x;
        m->_42 = m->_42 + obj->p6c.y;
        m->_43 = m->_43 + obj->p6c.z;
        obj->m._41 = (obj->p4c.x - obj->p6c.x) + obj->p78.x;
        obj->m._42 = (obj->p4c.y - obj->p6c.y) + obj->p78.y;
        obj->m._43 = (obj->p4c.z - obj->p6c.z) + obj->p78.z;
    }
    if (p) {
        d3_MatMul43(m, &obj->m, &t);
        d3_MatToWorld(p, orientOnly, &t, out);
        return;
    }
    d3_MatMul43(m, &obj->m, out);
}
/* 0x458da0, the world matrix of obj, in ref's space when ref is given (orientOnly 1: rotation only, no
   translation) */

void d3_WorldMatrix(XOBJ *obj, XOBJ *ref, char orientOnly, D3MATRIX *out)
{
    D3MATRIX t, id;

    d3_MatIdentity(&id);
    if (ref) {
        d3_MatToWorld(obj, orientOnly, &id, &t);
        t._41 = t._41 - ref->p4c.x;
        t._42 = t._42 - ref->p4c.y;
        t._43 = t._43 - ref->p4c.z;
        t._41 = t._41 - ref->p78.x;
        t._42 = t._42 - ref->p78.y;
        t._43 = t._43 - ref->p78.z;
        d3_MatMul43(&t, &ref->m, out);
        return;
    }
    d3_MatToWorld(obj, orientOnly, &id, out);
}
