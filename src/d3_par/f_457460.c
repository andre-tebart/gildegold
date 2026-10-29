/* 0x457460, emitter type 2: particles drift on a sine wave around a moving base position */

char par_EmitterCB2(D3PARTSYS *ps, unsigned long time)
{
    int frames = 1;
    PARPART *p;
    D3EMITTER *e;
    D3VECTOR4 r;
    float s0, s1, s2, s3;
    float dt;
    int max, spawned, i, age;

    e = PAR_E(ps);
    if (D3PAR_TIME(ps)->lasttime == time)
        return 1;
    if (e->istrigger)
        max = (int)(ps->count * e->maxtrigger);
    else
        max = ps->count;
    if (ps->tex && ps->tex->frames > 0)
        frames = ps->tex->frames;
    D3PAR_INV(ps)->inv_alpha0 = 1.0f / e->alpha[0];
    D3PAR_INV(ps)->inv_alpha_gap = 1.0f / (e->alpha[2] - e->alpha[1]);
    spawned = 0;
    p = (PARPART *)ps->particles;
    for (i = 0; i < ps->count; i++, p++) {
        if (!(p->flags & 1)) {
            if (spawned < max && (!e->istrigger || e->trigger)) {
                spawned++;
                p->phase[0] = PAR_RND() * 6.2831855f;
                p->phase[1] = PAR_RND() * 6.2831855f;
                p->phase[2] = PAR_RND() * 6.2831855f;
                p->phase[3] = PAR_RND() * 6.2831855f;
                p->vel.w = PAR_RND() * 6.2831855f;
                p->frame = i % frames;
                p->r = (rand() & e->a) + e->r;
                p->g = (rand() & e->a) + e->g;
                p->b = (rand() & e->a) + e->b;
                p->a = 0;
                p->time = time;
                r.x = PAR_RND() * 2.0f - 1.0f;
                r.y = PAR_RND() * 2.0f - 1.0f;
                r.z = PAR_RND() * 2.0f - 1.0f;
                p->base[0] = r.x * e->size[0];
                p->base[1] = r.y * e->size[1];
                p->base[2] = r.z * e->size[2];
                if (e->rebirthfill) {
                    float f = PAR_RND();
                    p->birth = (unsigned long)(time - e->alpha[2] * f);
                }
                else
                    p->birth = time;
                p->vel.x = PAR_RND();
                p->vel.y = PAR_RND();
                p->vel.z = PAR_RND();
                p->vel.x = e->rnd[0] * p->vel.x;
                p->vel.y = e->rnd[1] * p->vel.y;
                p->vel.z = e->rnd[2] * p->vel.z;
                p->flags |= 1;
            }
        } else {
            dt = (float)(time - p->time);
            if (dt > 150.0f) {
                dt = 0.0f;
                p->birth += time - p->time;
            }
            p->time = time;
            if (e->texmode)
                p->frame = (int)(time - p->birth) / e->texmode % frames;
            p->phase[0] += dt * e->phase[0];
            p->phase[1] += dt * e->phase[1];
            p->phase[2] += dt * e->phase[2];
            p->phase[3] += dt * e->phase3;
            p->phase[0] = s0 = (float)fmod(p->phase[0], 6.2831855f);
            p->phase[1] = s1 = (float)fmod(p->phase[1], 6.2831855f);
            p->phase[2] = s2 = (float)fmod(p->phase[2], 6.2831855f);
            p->phase[3] = s3 = (float)fmod(p->phase[3], 6.2831855f);
            p->vel.w = (float)fmod(dt * e->phase4 + p->vel.w, 6.2831855f);
            p->vel.x += dt * e->acc[0];
            p->vel.y += dt * e->acc[1];
            p->vel.z += dt * e->acc[2];
            r.x = e->vel[0] + p->vel.x;
            r.y = e->vel[1] + p->vel.y;
            r.z = e->vel[2] + p->vel.z;
            p->base[0] += dt * r.x;
            p->base[1] += dt * r.y;
            p->base[2] += dt * r.z;
            r.x = (float)sin(s0);
            r.y = (float)sin(s1);
            r.z = (float)sin(s2);
            r.w = (float)sin(s3);
            p->pos.x = r.x * e->amp[0] + p->base[0];
            p->pos.y = r.y * e->amp[1] + p->base[1];
            p->pos.z = r.z * e->amp[2] + p->base[2];
            p->size = r.w * e->amp3 + ps->size;
            p->pos.w = (float)sin(p->vel.w) * e->amp4 + e->time;
            if ((e->plane3 < 0.0f && p->pos.z < e->plane3) || (e->plane3 > 0.0f && p->pos.z > e->plane3)) {
                p->pos.z = e->plane3;
                p->vel.x *= e->plane[0];
                p->vel.y *= e->plane[1];
                p->vel.z *= e->plane[2];
            }
            age = time - p->birth;
            if (age < e->alpha[0]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, (1.0f - (e->alpha[0] - age) * D3PAR_INV(ps)->inv_alpha0) * p->pos.w));
            } else if (age < e->alpha[1]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, p->pos.w));
            } else if (age < e->alpha[2]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, (e->alpha[2] - age) * D3PAR_INV(ps)->inv_alpha_gap * p->pos.w));
            } else {
                p->flags &= ~1;
                ps->flags++;
            }
        }
    }
    if (e->initfill)
        e->initfill = 0;
    if (e->trigger)
        e->trigger = 0;
    D3PAR_TIME(ps)->lasttime = time;
    return !(e->triggeronce && (int)ps->flags >= max);
}
