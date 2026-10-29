/* 0x416240, callback of d3t_CreateSmoke. Each particle rises from its start for ps->flags ms,
   fading in until unk10.w and out after it; ps->v00.x is the maximum alpha. */

static char d3t_SmokeCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p;
    float t;
    int i, dead;

    dead = 0;
    for (i = 0, p = ps->particles; i < ps->count; i++, p++) {
        if (time >= p->time && time <= p->time + ps->flags) {
            t = (float)(time - p->time) / ps->flags;
            p->pos.x = t * p->start.x;
            p->pos.y = t * p->start.y;
            p->pos.z = t * p->start.z;
            if (t < p->start.w)
                p->a = (unsigned char)(p->vel.x * ps->v00.x * t);
            else
                p->a = (unsigned char)((1.0 - (t - p->start.w) * p->vel.y) * ps->v00.x);
            p->flags |= 1;
        } else {
            p->flags &= ~1;
        }
        if (!(p->flags & 1))
            dead++;
    }
    return dead < ps->count;
}
