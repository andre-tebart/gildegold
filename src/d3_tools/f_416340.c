/* 0x416340. NONMATCHING: 21 bytes, scheduling in the p->time line (the original loads now after
   adding rand() & 7 and the delay, and increments i later); no operand order or type changes it.
   mask is an int local: color.a is loaded as a zero-extended dword before par_SetPos. */

D3PARTSYS *d3t_CreateSmoke(void *parent, D3VECTOR *pos, int count, float size, int unused, float height,
                           float y, float alpha, float spread, D3VECTOR *start, unsigned int duration,
                           int delay, D3COLOR color)
{
    D3PARTSYS *ps;
    D3PARTICLE *p;
    D3TEXFLAGS flags;
    D3VECTOR4 r;
    unsigned long now;
    int mask;

    int i;

    flags.all = 0;
    flags.b[0] = 0x7f;
    flags.b[2] |= 2;
    ps = par_Create(count, "Rauch", flags, size, parent, d3t_SmokeCB);
    if (ps) {
        now = d3_Time;
        mask = color.a;
        par_SetPos(ps, pos);
        ps->v00.x = alpha;
        ps->v00.y = height;
        ps->v10.x = start->x;
        ps->v10.y = start->y;
        ps->v10.z = start->z;
        ps->flags = duration;
        for (i = 0, p = ps->particles; i < ps->count; i++, p++) {
            p->start.x = start->x;
            p->start.y = start->y;
            p->start.z = start->z;
            p->start.y = y;
            r.x = (float)rand() / RAND_MAX;
            r.y = (float)rand() / RAND_MAX;
            r.z = (float)rand() / RAND_MAX;
            r.x *= spread;
            r.y *= spread;
            r.z *= spread;
            r.x -= spread * 0.5f;
            r.y -= spread * 0.5f;
            r.z -= spread * 0.5f;
            p->start.x += r.x;
            p->start.y += r.y;
            p->start.z += r.z;
            p->start.w = (p->start.y - ps->v00.y) / p->start.y;
            p->vel.x = 1.0f / p->start.w;
            p->a = 0;
            p->vel.y = 1.0f / (1.0f - p->start.w);
            p->r = (rand() & mask) + color.r;
            p->g = (rand() & mask) + color.g;
            p->b = (rand() & mask) + color.b;
            p->time = (rand() & 7) + i * delay + now;
        }
    }
    return ps;
}
