/* 0x415f30, callback of d3t_CreateExplosion. ps->flags holds the duration. */

static char d3t_ExplosionCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p;
    float t;
    int i;

    p = ps->particles;
    t = (float)(time - p->time) / ps->flags;
    if (t >= 0.0 && t <= 1.0) {
        t = 1.0f - ((float)cos((t + 1.0f) * 1.5707964f) + 1.0f);
        for (i = 0; i < ps->count; i++, p++) {
            p->pos.x = t * p->start.x;
            p->pos.y = (p->start.y + ps->v00.z) * t;
            p->pos.z = t * p->start.z;
            p->size = (t * 0.4f + 1.0f) * ps->size;
            p->a = (unsigned char)((1.0f - t) * 255.0f);
        }
    }
    return time < ps->particles->time + ps->flags;
}
