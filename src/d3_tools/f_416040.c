/* 0x416040, creates the explosion: count "Explosion" particles around pos, with radius / height / z as the
   effect extents and size the particle size; duration (ms) is kept in ps->flags for the callback. Each particle
   starts on a sphere of radius radius (start.y = sin(i/count * pi/2) * height, x/z from the circle) with random
   jitter, and every count/6-th is pulled in by 0.4 / 0.2 / 0.4. Returns the system */

D3PARTSYS *d3t_CreateExplosion(void *parent, D3VECTOR *pos, int count, float radius, float height, float z,
                               float size, unsigned int duration)
{
    D3PARTSYS *ps;
    D3PARTICLE *p;
    D3TEXFLAGS flags;
    D3VECTOR4 r;
    float inv, t, ang, w;
    int i, n;

    flags.all = 0;
    flags.b[0] = 0xff;
    flags.b[2] |= 1;
    ps = par_Create(count, "Explosion", flags, size, parent, d3t_ExplosionCB);
    if (ps) {
        inv = 1.0f / ps->count;
        n = ps->count / 6;
        par_SetPos(ps, pos);
        ps->v00.x = radius;
        ps->v00.y = height;
        ps->v00.z = z;
        ps->flags = duration;
        p = ps->particles;
        for (i = 0; i < ps->count; i++, p++) {
            p->flags |= 1;
            r.x = (float)rand() / RAND_MAX;
            r.y = (float)rand() / RAND_MAX;
            r.z = (float)rand() / RAND_MAX;
            t = ps->v00.y * 0.1f;
            r.x *= t;
            r.y *= t;
            r.z *= t;
            r.x -= ps->v00.y * 0.05f;
            r.y -= ps->v00.y * 0.025f;
            r.z -= ps->v00.y * 0.05f;
            ang = (float)rand() * 6.2831855f / RAND_MAX;
            p->start.y = (float)sin(i * inv * 1.5707964f) * ps->v00.y;
            w = (float)sqrt(ps->v00.x * ps->v00.x - p->start.y * p->start.y);
            p->start.x = (float)sin(ang) * w + r.x;
            p->start.y += r.y;
            p->start.z = (float)cos(ang) * w + r.z;
            if (i % n == 0) {
                p->start.x *= 0.4f;
                p->start.y *= 0.2f;
                p->start.z *= 0.4f;
            }
        }
    }
    return ps;
}
