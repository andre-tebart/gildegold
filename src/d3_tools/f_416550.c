/* 0x416550, callback of d3t_CreateEarth. NONMATCHING: 15 bytes, the original puts dead in the ps
   parameter slot and a in the slot of s (ours: the reverse), and loads ps->tex before c.a.
   Clods are thrown up once, bounce on the ground at v00.w and fade out. */

static int d3t_EarthCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p;
    D3COLOR c;
    unsigned int frames;
    int mask, i, dead;
    float len, s, k, dt;
    volatile float a;

    if (ps->flags & 1) {
        ps->flags &= ~1;
        frames = 1;
        c = ps->color24;
        mask = c.a;
        if (ps->tex && ps->tex->frames > 0)
            frames = ps->tex->frames;
        for (i = 0, p = ps->particles; i < ps->count; i++, p++) {
            if (!(p->flags & 1)) {
                p->r = (rand() & mask) + c.r;
                p->g = (rand() & mask) + c.g;
                p->b = (rand() & mask) + c.b;
                p->frame = i % frames;
                p->pos.x = (float)((float)rand() / RAND_MAX * 4.0) - 2.0f;
                p->pos.y = (float)((float)rand() / RAND_MAX) * 1.5f;
                p->pos.z = (float)((float)rand() / RAND_MAX * 4.0) - 2.0f;
                p->vel.x = D3T_RAND(ps->v10.x);
                p->vel.y = (float)((float)rand() / RAND_MAX) * ps->v10.z + ps->v10.y;
                p->vel.z = D3T_RAND(ps->v10.x);
                len = (float)sqrt(p->vel.x * p->vel.x + p->vel.z * p->vel.z);
                if (len > ps->v10.x || len < ps->v10.x * 0.4) {
                    s = ps->v10.x * 0.25f;
                    k = (D3T_RAND(s) + ps->v10.x) / len;
                    p->vel.x = k * p->vel.x;
                    p->vel.z = k * p->vel.z;
                }
                p->start.x = (float)((rand() & 0x3f) + 0xc0);
                p->size = D3T_RAND(ps->v00.x) + ps->size;
                p->time = time;
                p->flags |= 1;
            }
        }
    }
    dead = 0;
    for (i = 0, p = ps->particles; i < ps->count; i++, p++) {
        if (p->flags & 1) {
            dt = (time - p->time) / 3.0f;
            p->time = time;
            p->pos.x += dt * p->vel.x;
            p->pos.y += dt * p->vel.y;
            p->pos.z += dt * p->vel.z;
            if (p->pos.y < ps->v00.w) {
                p->pos.y = ps->v00.w;
                p->vel.x -= p->vel.x * 0.29f;
                p->vel.y *= -0.34f;
                p->vel.z -= p->vel.z * 0.31f;
                if (fabs(p->vel.y) < 0.2) {
                    p->vel.x = 0;
                    p->vel.y = 0;
                    p->vel.z = 0;
                }
            } else if (p->vel.y != 0.0f) {
                p->vel.y -= dt * ps->v00.y;
            }
            p->start.x = (a = p->start.x - dt * ps->v00.z);
            if (a > 0.0)
                p->a = (unsigned char)a;
            else
                p->flags &= ~1;
        }
        if (!(p->flags & 1))
            dead++;
    }
    if (dead >= ps->count && !(ps->flags & 2))
        return 0;
    return 1;
}
