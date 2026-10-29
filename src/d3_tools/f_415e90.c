/* 0x415e90, creates the blood spray: 200 "Blut" particles with the effect parameters a..d; n is the number
   spawned per frame (ps->flags = n * 2 | 1, the callback reads n from flags >> 1 and bit 0 enables spawning).
   Returns the system */

D3PARTSYS *d3t_CreateBlood(void *parent, int n, float a, float b, float c, float d)
{
    D3PARTSYS *ps;
    D3TEXFLAGS flags;
    int i;

    flags.all = 0;
    flags.b[0] = 0x7f;
    flags.b[2] |= 2;
    ps = par_Create(200, "Blut", flags, 1.0f, parent, d3t_BloodCB);
    if (ps) {
        ps->flags = n * 2 | 1;
        ps->v00.x = a;
        ps->v00.y = b;
        ps->v00.z = c;
        ps->v00.w = d;
        for (i = 0; i < ps->count; i++)
            ps->particles[i].flags &= ~1;
    }
    return ps;
}
