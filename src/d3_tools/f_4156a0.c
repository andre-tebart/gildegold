/* 0x4156a0, callback of d3t_CreateEmitter. The speed sum is cast to double (squares come out as
   x, z, y), and goes through a second local (root moved to st(3), dead values popped, then stored).
   z is volatile because the original keeps it in memory (fld z; fmul z, and fst z; fstp vel.z)
   where x and y follow from the parenthesized assignments. */

/* Random value in [-r, r]. The inner cast keeps the 1/RAND_MAX multiply ahead of (r + r). */
#define D3T_RAND(r) ((float)((float)rand() / RAND_MAX) * ((r) + (r)) - (r))

static char d3t_EmitterCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p;
    D3VECTOR4 n;
    float k, dt, speed, x, y, a, d, s;
    volatile float z;
    unsigned int spawned, alive;
    int i;

    spawned = 0;
    alive = 0;
    k = ps->v10.x / (ps->v00.w * 2.0f + ps->v00.y);
    ps->flags &= ~2;
    p = ps->particles;
    for (i = 0; i < ps->count; i++, p++) {
        if ((ps->flags & 1) || (!(p->flags & 1) && spawned < ps->spawnCount)) {
            p->pos.x = (float)((float)rand() / RAND_MAX * 10.0) - 5.0f;
            p->pos.y = (float)((float)rand() / RAND_MAX * 10.0) - 5.0f;
            p->pos.z = (float)((float)rand() / RAND_MAX * 10.0) - 5.0f;
            p->pos.x += ps->v00.x;
            p->pos.y = (ps->v00.y) + p->pos.y;
            p->pos.z += ps->v00.z;
            p->vel.y = -(ps->v00.y - ps->v00.w);
            p->vel.x = D3T_RAND(ps->v00.w * 2.0f);
            p->vel.z = D3T_RAND(ps->v00.w * 2.0f);
            p->vel.x = k * p->vel.x;
            p->vel.y = k * p->vel.y;
            p->vel.z = k * p->vel.z;
            p->color = ps->color;
            p->start.x = 255.0f;
            p->size = ps->size;
            p->time = time;
            p->flags |= 1;
            spawned++;
        } else if (p->flags & 1) {
            dt = (time - p->time) / 3.0f;
            p->time = time;
            p->pos.x = (x = p->pos.x + dt * p->vel.x);
            p->pos.y = (y = p->pos.y + dt * p->vel.y);
            p->pos.z = (z = p->pos.z + dt * p->vel.z);
            s = (float)sqrt((double)((p->vel.x * p->vel.x) + p->vel.y * p->vel.y + p->vel.z * p->vel.z));
            speed = s;
            if ((float)sqrt(x * x + y * y + z * z) <= ps->v00.w && speed > ps->v10.y) {
                n.x = x;
                n.y = y;
                n.z = z;
                d3_Normalize(&n);
                if (n.y < 0.0f) {
                    p->flags &= ~1;
                } else {
                    p->pos.x = (ps->v00.w) * n.x;
                    p->pos.y = (ps->v00.w) * n.y;
                    p->pos.z = (ps->v00.w) * n.z;
                    d = ((n.x * p->vel.x) + n.y * p->vel.y + n.z * p->vel.z) * -2.0f;
                    p->vel.x = (x = p->vel.x + n.x * d);
                    p->vel.y = (y = p->vel.y + n.y * d);
                    p->vel.z = (z = p->vel.z + d * n.z);
                    if (speed > ps->v10.y) {
                        p->vel.x = (ps->v10.z) * x;
                        p->vel.y = (ps->v10.z) * y;
                        p->vel.z = (ps->v10.z) * z;
                    }
                }
            } else {
                p->vel.y -= dt * ps->v10.w;
            }
            a = min(1.0f, max(0.0f, p->pos.y / ps->v00.y) + 0.2f) * 255.0f;
            p->start.x = a;
            if (a > 0.0 && p->pos.y > 0.0f)
                p->a = (unsigned char)a;
            else
                p->flags &= ~1;
            alive++;
        }
    }
    ps->flags &= ~1;
    if (alive == 0)
        ps->flags |= 2;
    return 1;
}
