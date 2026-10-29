/* 0x40ec70, creates a 100-particle "blut" system attached to *parent with ocmd_ParticleCB (a stream falling from
   y = 100) and lays its particles out; returns it */

int ocmd_CreateParticle(void **parent)
{
    D3PARTSYS *ps;
    D3TEXFLAGS flags;
    int i;

    flags.all = 0;
    flags.b[0] = 0x7f;
    flags.b[2] |= 3;
    ps = par_Create(100, "blut", flags, 1.0f, *parent, (D3PARTCB)ocmd_ParticleCB);
    for (i = 0; i < 100; i++) {
        ps->particles[i].pos.x = 0.0f;
        ps->particles[i].pos.y = i * 0.16f;
        ps->particles[i].pos.z = 0.0f;
        ps->particles[i].start.x = ps->particles[i].pos.x;
        ps->particles[i].start.y = ps->particles[i].pos.y;
        ps->particles[i].start.z = ps->particles[i].pos.z;
        ps->particles[i].vel.x = (float)rand() / RAND_MAX * 0.2 - 0.1;
        ps->particles[i].vel.y = 0.1 - ((float)rand() / RAND_MAX + 1.0) * 0.2;
        ps->particles[i].vel.z = -0.5 - ps->particles[i].vel.y;
        ps->particles[i].a = (unsigned char)max(0.0, ps->particles[i].pos.y * 1.5 - 20.0);
    }
    return (int)ps;
}
