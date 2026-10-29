/* 0x411eb0, center and radius of a mesh's 8-corner box as seen from the camera; returns 1 when the
   box was read. The squared corner distance is stored through a volatile view and read back through
   a plain one: the original writes it to its slot (fstp) and compares it from memory (fcom [slot]),
   while a direct spelling keeps the sum on the FPU and compares r itself. Only the fsqrt position
   still differs (1 order hunk). */

typedef struct D3MESHLIST { char pad[0x10]; D3MESHOPS *ops; } D3MESHLIST;

char d3t_GetBoundingSphere(D3OBJECT *obj, D3OBJECT *camera, D3VECTOR *center, float *radius)
{
    char ok = 0;
    D3MESHLIST *list;
    D3BOXVERT *v;
    D3VECTOR4 c;
    float dx, dy, dz, r;
    union {
        volatile float sum;         /* squared distance, stored: the original keeps it in memory */
        float dist;                 /* the same slot, read by the compare (fcom [slot]) */
    } d;
    D3BOXVERT *w;
    int i;

    if (obj && (center || radius) && obj->material->ops) {
        list = (D3MESHLIST *)obj->material->animCtl;
        d3_UpdateWorldMatrix(obj, camera, 0);
        v = list->ops->GetBox(obj, list, obj->material->meshData);
        if (v) {
            c.x = v[0].x;
            c.y = v[0].y;
            c.z = v[0].z;
            for (i = 0, w = v + 1; i < 7; i++, w++) {
                c.x += w->x;
                c.y += w->y;
                c.z += w->z;
            }
            c.x *= 0.125f;
            c.y *= 0.125f;
            c.z *= 0.125f;
            r = 0.0f;
            for (i = 0; i < 8; i++) {
                dx = c.x - v[i].x;
                dy = c.y - v[i].y;
                dz = c.z - v[i].z;
                d.sum = dx * dx + dy * dy + dz * dz;
                r = max(r, d.dist);
            }
            ok = 1;
            if (radius)
                *radius = (float)sqrt(r);
            if (center) {
                center->x = c.x;
                center->y = c.y;
                center->z = c.z;
            }
        }
    }
    return ok;
}
