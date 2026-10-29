/* 0x43d860, bounding sphere of an object relative to camera. @deep B3 draft: 37 code hunks, all from the return flag's register. The original holds ok in bl (xor bl,bl at entry, every failure return becomes mov al,bl, the success path mov al,0x1); because ebx is taken by ok it reloads the radius pointer from [esp+0x30] and center stays in ebp. Ours constant-propagates the failures (xor al,al) and keeps radius in ebx from the entry, which shifts the allocation of the whole body (first hunk at instruction 1: xor al,al vs xor bl,bl + push edi order). Same behaviour. Tried now and per notes/units/d3_engine.md round 2: char/int ok, late ok = 0, return 0 / return 1 at the failure points, goto a shared return, ok = 1 before vs after the out-parameter stores; VC6 always folds the known-zero return. */

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
        } else {
            info = obj->info;
            if (!info || !info->mesh)
                return ok;
            d3_UpdateWorldMatrix(obj, camera, 0);
            box = D3_OPS(info)->GetBox(obj, info, D3_MESH(obj)->cameraMatrix);
            if (!box)
                return ok;
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
        }
        ok = 1;
        if (radius)
            *radius = r;
        if (center) {
            center->x = c.x;
            center->y = c.y;
            center->z = c.z;
        }
    }
    return ok;
}
