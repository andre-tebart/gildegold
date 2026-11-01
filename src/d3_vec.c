/* d3_vec.c -- light budget counters, angle / vector / matrix helpers: 0x453920..0x454780 (0x454780 is d3_mir.c's
   by codegen). Original file name unknown; the name is a guess. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_vec.c); boundaries in decl/text.dcl */
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_vec.h"
#include "f3init.h"

extern unsigned int d3_LightKeysCulled;     /* 0x633b08 */
extern unsigned int d3_LightKeysVisible;    /* 0x633b0c */
extern unsigned int d3_LightGeneration;     /* 0x633b10 */
extern unsigned int d3_LightKeyBudgetCulled;  /* 0x5fcc6c */

/* 0x453920, fills d3fl_Shade with 1 - 2/pi * acos(i / 1024) and sets the per-frame light key
 * budgets for lights that are culled (out of view) and for the ones that pass the clip test */
void d3_ShadeInit(unsigned int visible_budget, unsigned int culled_budget)
{
    int i;

    for (i = 0; i < 0x400; i++)
        d3fl_Shade[i] = 1.0f - (float)acos(i * 0.0009765625f) * 0.63661975f;
    d3_LightKeyBudgetVisible = visible_budget;
    d3_LightKeyBudgetCulled = culled_budget;
    d3_LightKeysVisible = 0;
    d3_LightKeysCulled = 0;
    d3_LightGeneration = 0;
}
/* 0x453990, clears the light key budgets and the per-frame key counters and resets the lightmap generation */

void d3_ShadeReset(void)
{
    d3_LightKeyBudgetCulled = 0;
    d3_LightKeyBudgetVisible = 0;
    d3_LightKeysVisible = 0;
    d3_LightKeysCulled = 0;
    d3_LightGeneration = 0;
}
/* 0x4539b0, out = a + (b - a) * t */

void d3_Lerp3(float *a, float *b, float t, float *out)
{
    out[0] = (b[0] - a[0]) * t + a[0];
    out[1] = (b[1] - a[1]) * t + a[1];
    out[2] = (b[2] - a[2]) * t + a[2];
}
/* 0x4539f0, signed angle around the y axis from a to b (xz plane) */

float d3_VecAngle(D3VECTOR *a, D3VECTOR *b)
{
    D3VECTOR4 va, vb, nb;
    float c[3];

    va.x = a->x;
    va.z = a->z;
    va.y = 0.0f;                     /* dead store: ranks va.y before vb.y for VC6's operand order in the acos dot product (matching trick) */
    vb.y = 0.0f;
    va.y = 0.0f;
    vb.x = b->x;
    vb.z = b->z;
    d3_Normalize(&va);
    d3_Normalize(&vb);
    if (d3_VecEqual((D3VECTOR *)&va, (D3VECTOR *)&vb, 1e-7f))
        return 0.0f;
    nb.x = -vb.x;
    nb.y = -vb.y;
    nb.z = -vb.z;
    if (d3_VecEqual((D3VECTOR *)&va, &nb, 1e-7f))
        return -3.1415927f;
    c[0] = vb.z * va.y - va.z * vb.y;
    c[1] = vb.x * va.z - vb.z * va.x;
    c[2] = vb.y * va.x - va.y * vb.x;
    if (d3_YAxis.x * c[0] + d3_YAxis.y * c[1] + d3_YAxis.z * c[2] > 0.0f)
        /* the (float) cast on the first product orders the three products like the original (matching trick) */
        return -acos((float)(vb.z * va.z) + vb.y * va.y + vb.x * va.x);
    return acos((float)(vb.z * va.z) + vb.y * va.y + vb.x * va.x) - 6.2831855f;
}
/* 0x453b40, d3_VecAngle wrapped to [-pi, pi) */

float d3_VecAngle2(D3VECTOR *a, D3VECTOR *b)
{
    float r = d3_VecAngle(a, b);

    if (r < -3.1415927f)
        r += 6.2831855f;
    return r;
}
/* 0x453b70, rows 0..2 of m: right, up, forward (forward = normalized dir, up = (0,1,0) rolled by roll) */

void d3_MakeBasis(D3VECTOR *dir, float roll, D3MATRIX *m)
{
    D3VECTOR4 u;
    float s, c, d, t;

    s = (float)sin(roll);
    u.z = d3_YAxis.z;
    c = (float)cos(roll);
    u.x = d3_YAxis.x * c - d3_YAxis.y * s;
    u.y = d3_YAxis.y * c + d3_YAxis.x * s;
    m->_31 = dir->x;
    m->_32 = dir->y;
    m->_33 = dir->z;
    d3_Normalize((D3VECTOR4 *)&m->_31);
    d = -(u.x * m->_31 + u.z * m->_33 + u.y * m->_32);
    m->_21 = d * m->_31 + u.x;
    m->_22 = d * m->_32 + u.y;
    m->_23 = d * m->_33 + u.z;
    if ((float)sqrt(m->_21 * m->_21 + m->_22 * m->_22 + m->_23 * m->_23) < 1e-7f) {
        t = -m->_32;
        m->_21 = t * m->_31 + d3_YAxis.x;
        m->_22 = t * m->_32 + d3_YAxis.y;
        m->_23 = t * m->_33 + d3_YAxis.z;
        if ((float)sqrt(m->_22 * m->_22 + m->_21 * m->_21 + m->_23 * m->_23) < 1e-7f) {
            t = -m->_33;
            m->_21 = t * m->_31 + d3_ZAxis.x;
            m->_22 = t * m->_32 + d3_ZAxis.y;
            m->_23 = t * m->_33 + d3_ZAxis.z;
            if ((float)sqrt(m->_22 * m->_22 + m->_21 * m->_21 + m->_23 * m->_23) < 1e-7f) {
                d3_MatIdentity(m);
                return;
            }
        }
    }
    d3_Normalize((D3VECTOR4 *)&m->_21);
    m->_34 = 0.0f;
    m->_24 = 0.0f;
    m->_14 = 0.0f;
    m->_44 = 1.0f;
    m->_11 = m->_22 * (m->_33) - (m->_32) * m->_23;
    m->_12 = m->_31 * m->_23 - (m->_21) * m->_33;
    m->_13 = m->_21 * m->_32 - m->_31 * m->_22;
}
/* 0x453d60, ra = a mod 2pi, rb = b mod 2pi, then ra shifted by +-2pi to be the nearest to rb */

void d3_UnwrapAngles(float *a, float *b, float *ra, float *rb)
{
    int i;
    float x;
    float d;

    for (i = 0; i < 3; i++) {
        ra[i] = (float)fmod(a[i], 6.2831855f);
        x = (float)fmod(b[i], 6.2831855f);
        rb[i] = x;
        d = (float)fabs(x - ra[i]);
        if (d > fabs(x - (ra[i] + 6.2831855f))) {
            ra[i] = ra[i] + 6.2831855f;
        } else if (d > fabs(x - (ra[i] - 6.2831855f))) {
            ra[i] = ra[i] - 6.2831855f;
        }
    }
}
/* 0x453e20, cubic Hermite spline between p0 and p1 with tangents m0, m1 (missed by Ghidra) */

float d3_Hermite(float p0, float p1, float m0, float m1, float t)
{
    float t2 = t * t, t3 = t2 * t;

    float r = p0 * (2.0f * t3 - 3.0f * t2 + 1.0f) + p1 * (3.0f * t2 - 2.0f * t3);

    r += m0 * (t3 - 2.0f * t2 + t);
    r += m1 * (t3 - t2);
    return r;
}
/* 0x453e80, all components within eps */

int d3_VecEqual(D3VECTOR *a, D3VECTOR *b, float eps)
{
    if (fabs(b->x - a->x) <= eps && fabs(b->y - a->y) <= eps && fabs(b->z - a->z) <= eps)
        return 1;
    return 0;
}
/* 0x453ee0, out = a * b for 4x3 affine matrices (row vectors) */

void d3_MatMul43(D3MATRIX *a, D3MATRIX *b, D3MATRIX *out)
{
    out->_11 = a->_13 * b->_31 + a->_12 * b->_21 + a->_11 * b->_11;
    out->_12 = b->_32 * a->_13 + b->_12 * a->_11 + a->_12 * b->_22;
    out->_13 = b->_33 * a->_13 + b->_23 * a->_12 + b->_13 * a->_11;
    out->_14 = 0.0f;
    out->_21 = a->_22 * b->_21 + a->_23 * b->_31 + a->_21 * b->_11;
    out->_22 = b->_12 * a->_21 + b->_22 * a->_22 + b->_32 * a->_23;
    out->_23 = b->_33 * a->_23 + b->_23 * a->_22 + b->_13 * a->_21;
    out->_24 = 0.0f;
    out->_31 = a->_32 * b->_21 + a->_33 * b->_31 + a->_31 * b->_11;
    out->_32 = b->_12 * a->_31 + b->_22 * a->_32 + b->_32 * a->_33;
    out->_33 = b->_33 * a->_33 + b->_23 * a->_32 + b->_13 * a->_31;
    out->_34 = 0.0f;
    out->_41 = a->_42 * b->_21 + a->_43 * b->_31 + a->_41 * b->_11 + b->_41;
    out->_42 = b->_12 * a->_41 + b->_22 * a->_42 + b->_32 * a->_43 + b->_42;
    out->_43 = b->_33 * a->_43 + b->_23 * a->_42 + b->_13 * a->_41 + b->_43;
    out->_44 = 1.0f;
}
/* 0x454040, transpose */

void d3_MatTranspose(D3MATRIX *src, D3MATRIX *dst)
{
    int i, j;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            ((float *)dst)[i * 4 + j] = ((float *)src)[j * 4 + i];
}
/* 0x454070, inverse of a 4x4 matrix (cofactor expansion; falls back to the transpose when |det| < 1e-7).
   Each m[k] is spelled either m[k] or src->_ij: the spelling changes VC6's operand order of the products
   (matching trick, found by search; no meaning). */

void d3_MatInverse(D3MATRIX *src, D3MATRIX *dst)
{
    float *m = (float *)src, *o = (float *)dst;
    float det;

    det = (src->_22 * (src->_33 * src->_44 - src->_43 * src->_34) - src->_32 * (m[6] * src->_44 - src->_43 * m[7]) + m[13] * (m[6] * src->_34 - m[7] * src->_33)) * src->_11 - (m[1] * (src->_33 * src->_44 - src->_43 * src->_34) - src->_32 * (src->_44 * src->_13 - src->_43 * m[3]) + m[13] * (src->_34 * src->_13 - m[3] * src->_33)) * src->_21 + (m[1] * (m[6] * src->_44 - src->_43 * m[7]) - src->_22 * (src->_44 * src->_13 - src->_43 * m[3]) + m[13] * (m[7] * src->_13 - m[3] * m[6])) * src->_31 - (m[1] * (m[6] * src->_34 - m[7] * src->_33) - src->_22 * (src->_34 * src->_13 - m[3] * src->_33) + src->_32 * (m[7] * src->_13 - m[3] * m[6])) * src->_41;
    if (fabs(det) < 1e-07) {
        d3_MatTranspose(src, dst);
        return;
    }
    det = 1.0f / det;
    o[0] = (src->_22 * (src->_33 * src->_44 - m[14] * src->_34) - src->_32 * (src->_23 * src->_44 - m[14] * m[7]) + src->_42 * (src->_23 * src->_34 - m[7] * src->_33)) * det;
    o[1] = -(((src->_33 * src->_44 - m[14] * src->_34) * m[1] - (src->_44 * src->_13 - m[14] * m[3]) * src->_32 + (src->_34 * src->_13 - m[3] * src->_33) * src->_42) * det);
    o[2] = ((src->_23 * src->_44 - m[14] * m[7]) * m[1] - (src->_44 * src->_13 - m[14] * m[3]) * src->_22 + (m[7] * src->_13 - m[3] * src->_23) * src->_42) * det;
    o[3] = -(((src->_23 * src->_34 - m[7] * src->_33) * m[1] - (src->_34 * src->_13 - m[3] * src->_33) * src->_22 + (m[7] * src->_13 - m[3] * src->_23) * src->_32) * det);
    o[4] = -(((src->_33 * src->_44 - m[14] * src->_34) * src->_21 - (src->_23 * src->_44 - m[14] * m[7]) * src->_31 + (src->_23 * src->_34 - m[7] * src->_33) * src->_41) * det);
    o[5] = ((src->_33 * src->_44 - m[14] * src->_34) * m[0] - (src->_44 * src->_13 - m[14] * m[3]) * src->_31 + (src->_34 * src->_13 - m[3] * src->_33) * src->_41) * det;
    o[6] = -(((src->_23 * src->_44 - m[14] * m[7]) * m[0] - (src->_44 * src->_13 - m[14] * m[3]) * src->_21 + (m[7] * src->_13 - m[3] * src->_23) * src->_41) * det);
    o[7] = ((src->_23 * src->_34 - m[7] * src->_33) * m[0] - (src->_34 * src->_13 - m[3] * src->_33) * src->_21 + (m[7] * src->_13 - m[3] * src->_23) * src->_31) * det;
    o[8] = ((src->_32 * src->_44 - src->_42 * src->_34) * src->_21 - (src->_44 * src->_22 - m[7] * src->_42) * src->_31 + (src->_34 * src->_22 - src->_32 * m[7]) * src->_41) * det;
    o[9] = -(((src->_32 * src->_44 - src->_42 * src->_34) * m[0] - (m[1] * src->_44 - m[3] * src->_42) * src->_31 + (m[1] * src->_34 - src->_32 * m[3]) * src->_41) * det);
    o[10] = ((src->_44 * src->_22 - m[7] * src->_42) * m[0] - (m[1] * src->_44 - m[3] * src->_42) * src->_21 + (m[7] * m[1] - m[3] * src->_22) * src->_41) * det;
    o[11] = -(((src->_34 * src->_22 - (src->_32) * m[7]) * m[0] - (m[1] * src->_34 - src->_32 * m[3]) * src->_21 + (m[7] * m[1] - m[3] * src->_22) * src->_31) * det);
    o[12] = -(((m[14] * src->_32 - src->_42 * src->_33) * src->_21 - (m[14] * src->_22 - src->_42 * src->_23) * src->_31 + (src->_33 * src->_22 - src->_32 * src->_23) * src->_41) * det);
    o[13] = ((m[14] * src->_32 - src->_42 * src->_33) * m[0] - (m[14] * m[1] - src->_42 * src->_13) * src->_31 + (m[1] * src->_33 - src->_32 * src->_13) * src->_41) * det;
    o[14] = -(((m[14] * src->_22 - src->_42 * src->_23) * m[0] - (m[14] * m[1] - src->_42 * src->_13) * src->_21 + (m[1] * src->_23 - src->_22 * src->_13) * src->_41) * det);
    o[15] = ((src->_33 * src->_22 - src->_32 * src->_23) * m[0] - (m[1] * src->_33 - src->_32 * src->_13) * src->_21 + (m[1] * src->_23 - src->_22 * src->_13) * src->_31) * det;
}
/* 0x454510, identity */

void d3_MatIdentity(D3MATRIX *m)
{
    D3_ZERO_AT(m, sizeof(D3MATRIX));
    m->_44 = 1.0f;
    m->_33 = 1.0f;
    m->_22 = 1.0f;
    m->_11 = 1.0f;
}
/* 0x454560, scales v to unit length in place; a zero vector is left as the zero vector */

void d3_Normalize(D3VECTOR4 *v)
{
    float l = (float)sqrt(v->z * v->z + v->y * v->y + v->x * v->x);

    if (l != 0.0f) {
        l = 1.0f / l;
        v->x = l * v->x;
        v->y = l * v->y;
        v->z = l * v->z;
    } else {
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
    }
}
/* 0x4545c0, angles to matrix */

void d3_MatFromAngles(D3VECTOR4 *v, D3MATRIX *m)
{
    double sx = sin(v->x), sy = sin(v->y), sz = sin(v->z);
    float cx = (float)cos(v->x), cy = (float)cos(v->y), t;
    double cz = cos(v->z), a;

    a = cz * sy;
    t = (float)(sz * sy);
    m->_34 = 0.0f; m->_24 = 0.0f; m->_14 = 0.0f;
    m->_43 = 0.0f; m->_42 = 0.0f; m->_41 = 0.0f;
    m->_44 = 1.0f;
    /* the (float) casts on cz here and on a in _13 set VC6's operand order of the mixed products (matching trick) */
    m->_11 = (float)((float)cz * cy);
    m->_12 = (float)(a * sx - cx * sz);
    m->_13 = (float)((float)a * cx + sz * sx);
    m->_21 = (float)(cy * sz);
    m->_22 = (float)(t * sx + cx * cz);
    m->_23 = (float)(t * cx - cz * sx);
    m->_31 = (float)-sy;
    m->_32 = (float)(cy * sx);
    m->_33 = cy * cx;
}
/* 0x454690, matrix rows to angles */

void d3_MatToAngles(D3VECTOR4 *m, D3VECTOR4 *angles)
{
    float *f = (float *)m;
    double l = sqrt((double)f[4] * f[4] + (double)f[0] * f[0]);

    if (l > 1.9073486328125e-06) {
        angles->x = (float)atan2(f[9], f[10]);
        angles->y = (float)atan2(-f[8], l);
        angles->z = (float)atan2(f[4], f[0]);
    } else {
        angles->x = (float)atan2(-f[6], f[5]);
        angles->y = (float)atan2(-f[8], l);
        angles->z = 0.0f;
    }
}
/* 0x454700, normal of the triangle a, b, c */

void d3_TriNormal(D3VECTOR4 *a, D3VECTOR4 *b, D3VECTOR4 *c, D3VECTOR4 *n)
{
    D3VECTOR4 u, v;

    u.x = b->x - a->x; u.y = b->y - a->y; u.z = b->z - a->z;
    v.x = c->x - a->x; v.y = c->y - a->y; v.z = c->z - a->z;
    n->x = v.z * u.y - v.y * u.z;
    n->y = v.x * u.z - v.z * u.x;
    n->z = v.y * u.x - v.x * u.y;
    d3_Normalize(n);
}

const D3VECTOR d3_YAxis = { 0.0f, 1.0f, 0.0f };  /* 0x5ebb94, (0, 1, 0), read-only: the original keeps it in .rdata */
