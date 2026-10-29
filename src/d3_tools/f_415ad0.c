/* 0x415ad0, creates the emitter particle system of an object's mesh: par_Create with the caller's texture, flags,
   size and count, then the world position, the mesh radius and the four effect parameters; returns the system,
   0 when the object has no mesh */

D3PARTSYS *d3t_CreateEmitter(D3OBJECT *obj, char *tex, char flag, unsigned long color, D3VECTOR *pos, int count,
                             int unused, float size, float a, float b, float c, float d)
{
    D3PARTSYS *ps = 0;
    D3TEXFLAGS flags;

    if (obj && obj->info && obj->info->mesh) {
        flags.all = 0;
        flags.b[0] = 0x7f;
        flags.b[2] = flag & 1 | 2;
        ps = par_Create(count, tex, flags, size, obj, d3t_EmitterCB);
        if (ps) {
            par_SetPos(ps, &d3_Origin);
            ps->v00.x = pos->x;
            ps->v00.y = pos->y;
            ps->v00.z = pos->z;
            ps->spawnCount = 2;
            ps->v00.y += ps->v00.w = obj->info->mesh->radius;
            ps->v10.x = a;
            ps->v10.y = b;
            ps->v10.z = c;
            ps->v10.w = d;
            ps->color = color;
            ps->v00.w *= 0.75f;
        }
    }
    return ps;
}
