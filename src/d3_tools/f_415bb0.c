/* 0x415bb0, callback of d3t_CreateBlood */

static char d3t_BloodCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p, *q;
    float dt, y;
    int i, dead, spawn;

    spawn = ps->flags >> 1;
    dead = 0;
    q = ps->particles;
    for (i = 0; i < ps->count; i++) {
        if (q[i].flags & 1) {
            dt = (time - ps->particles[i].time) * 0.001f;
            ps->particles[i].pos.x = ps->particles[i].start.x + ps->particles[i].vel.x;
            ps->particles[i].pos.y = ps->particles[i].start.y + ps->particles[i].vel.y;
            ps->particles[i].pos.z = ps->particles[i].start.z + ps->particles[i].vel.z;
            ps->particles[i].size += 0.8f * dt;
            ps->particles[i].vel.y -= dt;
            if (ps->particles[i].pos.y < -10.0)
                q[i].flags &= ~1;
            q[i].start.x = q[i].pos.x;
            q[i].start.y = q[i].pos.y;
            q[i].start.z = q[i].pos.z;
            q[i].a = (unsigned char)max(0.0f, q[i].pos.y * 1.5f + 50.0f);
        }
        if (!(q[i].flags & 1))
            dead++;
    }
    if (!(ps->flags & 1) && dead >= ps->count)
        return 0;
    if ((ps->flags & 1) && dead > spawn) {
        p = ps->particles;
        for (i = 0; i < ps->count && i < spawn; i++, p++) {
            if (!(p->flags & 1)) {
                p->flags |= 1;
                p->pos.x = (float)rand() / RAND_MAX - 0.5f;
                p->pos.y = (float)rand() * (ps->v00.y + 1.0f) / RAND_MAX + ps->v00.w;
                p->pos.z = (float)rand() / RAND_MAX - 0.5f;
                p->vel.x = (float)rand() * ps->v00.y * 2.0f / RAND_MAX - ps->v00.y;
                y = (p->pos.y - 55.0f) * ps->v00.z;
                p->vel.y = y - ((float)rand() / RAND_MAX * 0.1f + 0.2f);
                p->vel.z = (float)rand() / RAND_MAX * 0.05f + ((p->pos.y - 55.0f) + p->vel.y) * ps->v00.x + 0.4f;
                p->time = time;
                p->size = ps->size;
                p->start.x = p->pos.x;
                p->start.y = p->pos.y;
                p->start.z = p->pos.z;
                p->a = (unsigned char)max(0.0f, p->pos.y * 1.5f + 50.0f);
            }
        }
        ps->flags &= ~1;
    }
    return 1;
}
