/* d3_quad.c -- quad tree over the objects of a scene, 0x457c30..0x458810 (the zones d3q_Create .. d3q_Rebuild from 0x4582d0 on).
   Original file name unknown. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_quad.c); boundaries in decl/text.dcl, notes in notes/units/d3_quad.md */
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_quad.h"
#define d3_ClipBoxU ((unsigned int (*)(void *, float *, float *))d3_ClipBox)  /* 0x439650 (d3_engine.h: unsigned char): @original the byte-matched d3q_Mark reads the whole unsigned int result (no 25 ff 00 00 00 after the call) */

/* 0x457c30, free a node, its item list and its subtree */
void d3q_FreeNode(D3QNODE *n)
{
    D3QITEM *it, *next;
    unsigned int i;

    if (n) {
        for (it = n->first; it; it = next) {
            next = it->next;
            pool_Free(&d3_QItemPool, it);
        }
        for (i = 0; i < 4; i++) {
            if (n->child[i])
                d3q_FreeNode(n->child[i]);
        }
        pool_Free(&d3_QNodePool, n);
    }
}
/* 0x457c90, add an object that has bounds to the node's list */

char d3q_AddObject(D3OBJECT *obj, D3QNODE *n)
{
    D3QITEM *it;

    if (!obj->material || !D3Q_MAT(obj)->ops || !(obj->flags & 4))
        return 1;
    n->count++;
    it = pool_Alloc(&d3_QItemPool, 8, 0x80);
    it->obj = obj;
    if (n->last) {
        n->last->next = it;
        n->last = it;
    } else {
        n->first = it;
        n->last = it;
    }
    return 1;
}
/* 0x457d00, a child node with the objects of the parent that overlap [min, max] in x and z (0 if none) */

D3QNODE *d3q_MakeChild(D3QNODE *parent, float *min, float *max)
{
    D3QNODE *n;
    D3QITEM *it;
    D3QMAT *m;

    n = pool_Alloc(&d3_QNodePool, 0x48, 0x40);
    for (it = parent->first; it; it = it->next) {
        m = D3Q_MAT(it->obj);
        if (m->min.x <= max[0] && m->max.x >= min[0] && m->min.z <= max[2] && m->max.z >= min[2])
            d3q_AddObject(it->obj, n);
    }
    if (n->count == 0) {
        pool_Free(&d3_QNodePool, n);
        return 0;
    }
    return n;
}
/* 0x457db0, bounds of an object from its transformed box (also stored in its material) */

char d3q_GetBounds(D3OBJECT *obj, float *min, float *max)
{
    D3BOXVERT *v;
    int i;

    if (!(obj->stateFlags & 0x80))
        d3_UpdateWorldMatrix(obj, d3_Camera, 0);
    v = d3_GetBoxVerts(obj, (struct D3OBJINFO *)D3Q_MAT(obj)->list, D3Q_MAT(obj)->box);
    if (v) {
        min[0] = 1e10f;
        min[1] = 1e10f;
        min[2] = 1e10f;
        max[0] = -1e10f;
        max[1] = -1e10f;
        max[2] = -1e10f;
        for (i = 8; i != 0; i--, v++) {
            min[0] = min(min[0], v->x);
            min[1] = min(min[1], v->y);
            min[2] = min(min[2], v->z);
            max[0] = max(max[0], v->x);
            max[1] = max(max[1], v->y);
            max[2] = max(max[2], v->z);
        }
        D3Q_MAT(obj)->min.x = min[0];
        D3Q_MAT(obj)->min.y = min[1];
        D3Q_MAT(obj)->min.z = min[2];
        D3Q_MAT(obj)->max.x = max[0];
        D3Q_MAT(obj)->max.y = max[1];
        D3Q_MAT(obj)->max.z = max[2];
        return 1;
    }
    return 0;
}
/* 0x457f20, fills the node's bounds from its objects (depth 1: exact object bounds via d3q_GetBounds, deeper: the
   cached bounds in the material) and, below maxdepth and above minitems objects, splits it into four quadrants
   (x/z plane) and builds them recursively. A child that holds the same object count as its siblings (the objects
   straddle every split) is useless and freed again. depth / maxdepth are volatile: the original re-reads them
   from the stack (matching trick). The statement orders of the four child boxes are needed for the bytes. */

void d3q_Build(D3QNODE *n, unsigned int minitems, volatile unsigned int depth, volatile unsigned int maxdepth)
{
    char same = 1;
    D3QITEM *it;
    D3VECTOR4 bmin, bmax;
    D3VECTOR4 lo, hi, half;
    int cnt;
    unsigned int i;

    if (n->count > 0) {
        n->min.x = 1e10f;
        n->min.y = 1e10f;
        n->min.z = 1e10f;
        n->max.x = -1e10f;
        n->max.y = -1e10f;
        n->max.z = -1e10f;
        if (depth <= 1) {
            for (it = n->first; it; it = it->next) {
                if (d3q_GetBounds(it->obj, &bmin.x, &bmax.x)) {
                    n->min.x = min(n->min.x, bmin.x);
                    n->min.y = min(n->min.y, bmin.y);
                    n->min.z = min(n->min.z, bmin.z);
                    n->max.x = max(n->max.x, bmax.x);
                    n->max.y = max(n->max.y, bmax.y);
                    n->max.z = max(n->max.z, bmax.z);
                }
            }
        } else {
            for (it = n->first; it; it = it->next) {
                n->min.x = min(n->min.x, D3Q_MAT(it->obj)->min.x);
                n->min.y = min(n->min.y, D3Q_MAT(it->obj)->min.y);
                n->min.z = min(n->min.z, D3Q_MAT(it->obj)->min.z);
                n->max.x = max(n->max.x, D3Q_MAT(it->obj)->max.x);
                n->max.y = max(n->max.y, D3Q_MAT(it->obj)->max.y);
                n->max.z = max(n->max.z, D3Q_MAT(it->obj)->max.z);
            }
        }
    }
    if (depth < maxdepth && (unsigned int)n->count > minitems) {
        lo.x = n->min.x;
        cnt = -1;
        half.x = n->max.x - n->min.x;
        half.z = n->max.z - n->min.z;
        half.x *= 0.5f;
        lo.z = n->min.z;
        half.z *= 0.5f;
        hi.x = half.x + n->min.x;
        hi.z = half.z + n->min.z;
        n->child[0] = d3q_MakeChild(n, &lo.x, &hi.x);
        lo.x = half.x + n->min.x;
        lo.z = n->min.z;
        hi.x = n->max.x;
        hi.z = half.z + n->min.z;
        n->child[1] = d3q_MakeChild(n, &lo.x, &hi.x);
        hi.x = n->max.x;
        hi.z = n->max.z;
        lo.x = half.x + n->min.x;
        lo.z = half.z + n->min.z;
        n->child[2] = d3q_MakeChild(n, &lo.x, &hi.x);
        lo.z = half.x + n->min.z; /* sic: the original uses half.x (not half.z) here */
        lo.x = n->min.x;
        hi.z = n->max.z;
        hi.x = half.x + n->min.x;
        n->child[3] = d3q_MakeChild(n, &lo.x, &hi.x);
        for (i = 0; i < 4; i++) {
            if (n->child[i]) {
                if (cnt < 0)
                    cnt = n->child[i]->count;
                else if (cnt != n->child[i]->count)
                    same = 0;
            }
        }
        for (i = 0; i < 4; i++) {
            if (n->child[i]) {
                if (same) {
                    d3q_FreeNode(n->child[i]);
                    n->child[i] = 0;
                } else
                    {
                        unsigned int d = depth + 1;
                        d3q_Build(n->child[i], minitems, d, maxdepth);
                    }
            }
        }
    }
}
/* 0x4582d0, makes a tree over the objects of the default scene that match mask */

D3QNODE *d3q_Create(D3OBJECT *start, unsigned int mask, unsigned int minitems, unsigned int maxdepth)
{
    D3QNODE *n;
    D3QITEM *it;

    n = pool_Alloc(&d3_QNodePool, 0x48, 0x40);
    n->mask = mask;
    n->minitems = minitems;
    n->maxdepth = maxdepth;
    d3_ForEachObject(d3_DefaultScene, start, mask, (D3OBJECTCB)d3q_AddObject, n);
    d3q_Build(n, minitems, 1, maxdepth);
    if (n->count == 0) {
        pool_Free(&d3_QNodePool, n);
        return 0;
    }
    for (it = n->first; it; it = it->next)
        D3Q_MAT(it->obj)->zone = -1;
    return n;
}
/* 0x458360, the 8 corners of the node's box relative to pos, rotated by m (the first one also moved by m's translation) */

D3BOXVERT *d3q_BoxCorners(D3QNODE *n, float *pos, float *m)
{
    float x0 = n->min.x - pos[0];
    float y0 = n->min.y - pos[1];
    float z0 = n->min.z - pos[2];
    D3VECTOR4 h;

    h.x = n->max.x - pos[0];
    h.y = n->max.y - pos[1];
    h.z = n->max.z - pos[2];

    d3_QBox[0].x = y0 * m[4] + z0 * m[8] + x0 * m[0] + m[12];
    d3_QBox[0].y = y0 * m[5] + z0 * m[9] + x0 * m[1] + m[13];
    d3_QBox[0].z = x0 * m[2] + y0 * m[6] + z0 * m[10] + m[14];
    d3_QBox[1].x = y0 * m[4] + z0 * m[8] + h.x * m[0];
    d3_QBox[1].y = y0 * m[5] + z0 * m[9] + h.x * m[1];
    d3_QBox[1].z = h.x * m[2] + y0 * m[6] + z0 * m[10];
    d3_QBox[2].x = z0 * m[8] + h.y * m[4] + x0 * m[0];
    d3_QBox[2].y = z0 * m[9] + h.y * m[5] + x0 * m[1];
    d3_QBox[2].z = x0 * m[2] + z0 * m[10] + h.y * m[6];
    d3_QBox[3].x = z0 * m[8] + h.y * m[4] + h.x * m[0];
    d3_QBox[3].y = z0 * m[9] + h.y * m[5] + h.x * m[1];
    d3_QBox[3].z = h.x * m[2] + z0 * m[10] + h.y * m[6];
    d3_QBox[4].x = y0 * m[4] + h.z * m[8] + x0 * m[0];
    d3_QBox[4].y = y0 * m[5] + h.z * m[9] + x0 * m[1];
    d3_QBox[4].z = x0 * m[2] + y0 * m[6] + h.z * m[10];
    d3_QBox[5].x = y0 * m[4] + h.z * m[8] + h.x * m[0];
    d3_QBox[5].y = y0 * m[5] + h.z * m[9] + h.x * m[1];
    d3_QBox[5].z = h.x * m[2] + y0 * m[6] + h.z * m[10];
    d3_QBox[6].x = h.y * m[4] + h.z * m[8] + x0 * m[0];
    d3_QBox[6].y = h.y * m[5] + h.z * m[9] + x0 * m[1];
    d3_QBox[6].z = x0 * m[2] + h.y * m[6] + h.z * m[10];
    d3_QBox[7].x = h.y * m[4] + h.z * m[8] + h.x * m[0];
    d3_QBox[7].y = h.y * m[5] + h.z * m[9] + h.x * m[1];
    d3_QBox[7].z = h.x * m[2] + h.y * m[6] + h.z * m[10];
    return d3_QBox;
}
/* 0x458650, sets the zone of the objects in the nodes that the camera box touches */

void d3q_Mark(D3QNODE *n, D3OBJECT *cam, int zone)
{
    unsigned int r, i;
    D3QITEM *it;
    D3BOXVERT *box;

    if (n) {
        box = d3q_BoxCorners(n, &cam->pos.x, &cam->matrix._11);
        r = d3_ClipBoxU(box, 0, 0);
        if (r & 0x40) {
            for (it = n->first; it; it = it->next) {
                if (it->obj->material)
                    D3Q_MAT(it->obj)->zone = zone;
            }
        } else if (r & 0x3f) {
            for (i = 0; i < 4; i++) {
                if (n->child[i])
                    d3q_Mark(n->child[i], cam, zone);
            }
        }
    }
}
/* 0x4586d0, takes an object out of the node's list and of its children */

char d3q_Remove(D3OBJECT *obj, D3QNODE *n)
{
    D3QITEM *prev = 0, *it;
    D3QNODE *c;
    unsigned int i;

    if (n->first) {
        for (it = n->first; it; prev = it, it = it->next) {
            if (it->obj == obj)
                break;
        }
        if (!it)
            return 1;
        if (prev)
            prev->next = it->next;
        else
            n->first = it->next;
        if (n->last == it)
            n->last = prev;
        n->count--;
        pool_Free(&d3_QItemPool, it);
        i = 0;
        do {
            c = n->child[i];
            if (c) {
                d3q_Remove(obj, c);
                if (c->count == 0) {
                    pool_Free(&d3_QNodePool, c);
                    n->child[i] = 0;
                }
            }
            i++;
        } while (i < 4);
        obj->flags &= ~4;
    }
    return 1;
}
/* 0x458780, removes obj from the node's list (and from deeper nodes) through d3_ForEachObject */

void d3q_RemoveObject(D3OBJECT *obj, D3QNODE *n)
{
    if (n && obj)
        d3_ForEachObject(d3_DefaultScene, obj, n->mask | 0x280, (D3OBJECTCB)d3q_Remove, n);
}
/* 0x4587b0, builds the tree again from the objects of the scene */

void d3q_Rebuild(D3OBJECT *start, D3QNODE *n)
{
    int i;

    if (n) {
        i = 0;
        do {
            d3q_FreeNode(n->child[i]);
            i++;
        } while (i < 4);
        d3_ForEachObject(d3_DefaultScene, start, n->mask | 0x200, (D3OBJECTCB)d3q_AddObject, n);
        d3q_Build(n, n->minitems, 1, n->maxdepth);
    }
}

D3BOXVERT d3_QBox[8] = {0};  /* 0x633b30 */
struct MPOOL * d3_QNodePool = 0;  /* 0x633db0 */
struct MPOOL * d3_QItemPool = 0;  /* 0x633db4 */
