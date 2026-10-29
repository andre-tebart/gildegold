/* 0x412800, widens q->lo/hi by the triangles of obj and its children that lie in the box;
   returns 0 if any mesh was examined */

typedef struct D3HQUERY {           /* height query over a box */
    D3OBJECT *obj;                  /* 0x00, the object being placed */
    float lo0, hi0;                 /* 0x04, its own height range */
    float t;                        /* 0x0c, footprint height */
    float lo;                       /* 0x10, lowest triangle height found */
    float hi;                       /* 0x14, highest */
    D3VECTOR4 c0, c1, c2, c3;       /* 0x18, footprint corners */
    D3VECTOR a;                     /* 0x58, box min */
    char pad64[0x4];
    D3VECTOR b;                     /* 0x68, box max */
    char pad74[0x4];
} D3HQUERY;

int d3t_QueryHeight(D3HQUERY *q, D3OBJECT *obj)
{
    D3VECTOR4 mx, mn;
    D3BOXVERT *v;
    D3TRI *tri;
    D3OBJECT *child;
    int i, r = 1;

    if (obj->type == 4) {
        if (!obj->info)
            return r;
        d3_TransformSurf(obj);
        d3_UpdateObject(obj, 1);
        v = (D3BOXVERT *)(obj->info->keys + obj->info->nkeys);
        mn.x = v->x;
        mn.y = v->y;
        mn.z = v->z;
        mx.x = v->x;
        mx.y = v->y;
        mx.z = v->z;
        for (i = 1, v++; i < 8; i++, v++) {
            mn.x = min(mn.x, v->x);
            mn.y = min(mn.y, v->y);
            mn.z = min(mn.z, v->z);
            mx.x = max(mx.x, v->x);
            mx.y = max(mx.y, v->y);
            mx.z = max(mx.z, v->z);
        }
        if (mn.x < q->b.x && q->a.x < mx.x && q->b.y > mn.y && q->b.z > mn.z && q->a.z < mx.z) {
            tri = obj->info->tris;
            for (i = 0; i < obj->info->ntris; tri++, i++) {
                if (d3t_InTriBox(tri->v, &q->a, &q->b)) {
                    q->lo = min(q->lo, tri->v[0]->y);
                    q->lo = min(q->lo, tri->v[1]->y);
                    q->lo = min(q->lo, tri->v[2]->y);
                    q->hi = max(q->hi, tri->v[0]->y);
                    q->hi = max(q->hi, tri->v[1]->y);
                    q->hi = max(q->hi, tri->v[2]->y);
                }
            }
        }
        r = 0;
    }
    for (child = obj->child; child; child = child->next)
        r &= d3t_QueryHeight(q, child);
    return r;
}
