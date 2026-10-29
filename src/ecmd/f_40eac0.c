/* 0x40eac0, callback of CreateParticle: a stream of particles falling from y = 100 */

static void ocmd_ParticleCB(D3PARTSYS *ps, unsigned long time)
{
    int i;
    double dt;

    for (i = 0; i < ps->count; i++) {
        dt = (time - ps->particles[i].time) * 0.001;
        ps->particles[i].pos.x = ps->particles[i].start.x + ps->particles[i].vel.x;
        ps->particles[i].pos.y = ps->particles[i].start.y + ps->particles[i].vel.y;
        ps->particles[i].pos.z = ps->particles[i].start.z + ps->particles[i].vel.z;
        ps->particles[i].size += dt * 2.0;
        ps->particles[i].vel.y -= dt;
        if (ps->particles[i].pos.y < 0.0) {
            ps->particles[i].pos.x = 0.0f;
            ps->particles[i].pos.y = 100.0f;
            ps->particles[i].pos.z = 0.0f;
            ps->particles[i].vel.x = (float)rand() / RAND_MAX * 0.2 - 0.1;
            ps->particles[i].vel.y = 0.1 - ((float)rand() / RAND_MAX + 1.0) * 0.2;
            ps->particles[i].vel.z = -0.5 - ps->particles[i].vel.y;
            ps->particles[i].time = time;
            ps->particles[i].size = ps->size;
        }
        ps->particles[i].start.x = ps->particles[i].pos.x;
        ps->particles[i].start.y = ps->particles[i].pos.y;
        ps->particles[i].start.z = ps->particles[i].pos.z;
        ps->particles[i].a = (unsigned char)max(0.0, ps->particles[i].pos.y * 1.5 - 20.0);
    }
}
