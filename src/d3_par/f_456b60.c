/* 0x456b60, emitter type 1: particles start on a disc of radius size[0] and fly towards the disc of
   radius size[1] at height size[2] (a cone) */

char par_EmitterCB1(D3PARTSYS *ps, unsigned long time)
{
    int frames = 1;
    PARPART *p;
    D3EMITTER *e;
    D3VECTOR4 r;
    float dt, s, speed, rad;
    int max, spawned, i, age;

    e = PAR_E(ps);
    if (D3PAR_TIME(ps)->lasttime == time)
        return 1;
    s = (float)sqrt(e->dir[0] * e->dir[0] + e->dir[1] * e->dir[1] + e->dir[2] * e->dir[2]);
    speed = s;
    if (e->istrigger)
        max = (int)(ps->count * e->maxtrigger);
    else
        max = ps->count;
    if (ps->tex && ps->tex->frames > 0)
        frames = ps->tex->frames;
    D3PAR_INV(ps)->inv_alpha0 = 1.0f / e->alpha[0];
    D3PAR_INV(ps)->inv_alpha_gap = 1.0f / (e->alpha[2] - e->alpha[1]);
    D3PAR_INV(ps)->inv_size = 1.0f / e->size[2];
    D3PAR_INV(ps)->size_ratio = e->size[1] / e->size[0];
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
                r.x = PAR_RND() - 0.5f;
                r.y = (float)rand() / RAND_MAX - 0.5f;
                r.z = 0.0f;
                d3_Normalize(&r);
                if (!e->initfill && !e->rebirthfill) {
                    p->birth = time;
                    rad = PAR_RND() * e->size[0];
                    p->base[0] = r.x * rad;
                    p->base[1] = r.y * rad;
                    p->base[2] = r.z * rad;
                    if (e->size[0] != 0.0f)
                        rad *= D3PAR_INV(ps)->size_ratio;
                    else {
                        p->base[0] = 0.0f;
                        p->base[1] = 0.0f;
                        p->base[2] = 0.0f;
                        rad = PAR_RND() * e->size[1];
                    }
                    r.x *= rad;
                    r.y *= rad;
                    r.z = e->size[2];
                    r.x -= p->base[0];
                    r.y -= p->base[1];
                    r.z -= p->base[2];
                    d3_Normalize(&r);
                    p->vel.x = r.x * speed;
                    p->vel.y = r.y * speed;
                    p->vel.z = r.z * speed;
                } else {
                    float f;
                    if (e->size[0] != 0.0f) {
                        rad = PAR_RND() * e->size[0];
                        p->base[0] = r.x * rad;
                        p->base[1] = r.y * rad;
                        p->base[2] = r.z * rad;
                        rad *= D3PAR_INV(ps)->size_ratio;
                    } else {
                        p->base[0] = 0.0f;
                        p->base[1] = 0.0f;
                        p->base[2] = 0.0f;
                        rad = PAR_RND() * e->size[1];
                    }
                    r.x *= rad;
                    r.y *= rad;
                    r.z = e->size[2];
                    r.x -= p->base[0];
                    r.y -= p->base[1];
                    r.z -= p->base[2];
                    d3_Normalize(&r);
                    f = PAR_RND();
                    p->birth = (unsigned long)(time - e->alpha[2] * f);
                    f *= e->size[2];
                    p->base[0] += r.x * f;
                    p->base[1] += r.y * f;
                    p->base[2] += f * r.z;
                    /* the velocity written in both arms (VC6 tail-merges them) gives speed the weight
                       that puts it into the dead `time` slot, as in the original */
                    p->vel.x = r.x * speed;
                    p->vel.y = r.y * speed;
                    p->vel.z = r.z * speed;
                }
                r.x = PAR_RND();
                r.y = PAR_RND();
                r.z = PAR_RND();
                r.x *= e->rnd[0];
                r.y *= e->rnd[1];
                r.z *= e->rnd[2];
                p->vel.x += r.x;
                p->vel.y += r.y;
                p->vel.z += r.z;
                r.x = (float)sin(p->phase[0]);
                r.y = (float)sin(p->phase[1]);
                r.z = (float)sin(p->phase[2]);
                r.w = (float)sin(p->phase[3]);
                p->pos.x = r.x * e->amp[0] + p->base[0];
                p->pos.y = r.y * e->amp[1] + p->base[1];
                p->pos.z = r.z * e->amp[2] + p->base[2];
                p->size = r.w * e->amp3 + ps->size;
                p->flags |= 1;
            }
        } else {
            dt = (float)(time - p->time);
            if (dt > 150.0f) {
                dt = 0.0f;
                p->birth += time - p->time;
            }
            age = time - p->birth;
            p->time = time;
            if (e->texmode)
                p->frame = age / e->texmode % frames;
            p->phase[0] += dt * e->phase[0];
            p->phase[1] += dt * e->phase[1];
            p->phase[2] += dt * e->phase[2];
            p->phase[3] += dt * e->phase3;
            p->phase[0] = (float)fmod(p->phase[0], 6.2831855f);
            p->phase[1] = (float)fmod(p->phase[1], 6.2831855f);
            p->phase[2] = (float)fmod(p->phase[2], 6.2831855f);
            p->phase[3] = (float)fmod(p->phase[3], 6.2831855f);
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
            r.x = (float)sin(p->phase[0]);
            r.y = (float)sin(p->phase[1]);
            r.z = (float)sin(p->phase[2]);
            r.w = (float)sin(p->phase[3]);
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
            if (age < e->alpha[0]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, (1.0f - (e->alpha[0] - age) * D3PAR_INV(ps)->inv_alpha0) * p->pos.w));
            } else if (age < e->alpha[1]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, p->pos.w));
            } else if (age < e->alpha[2]) {
                p->a = (unsigned char)max(0.0f, min(255.0f, ((e->alpha[2] - age) * p->pos.w) * D3PAR_INV(ps)->inv_alpha_gap));
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
