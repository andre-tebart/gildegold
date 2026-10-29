/* 0x457b30. The first parameter is the parent (ecmd.h names it count), the third the particle count. */

D3PARTSYS *par_CreateEmitter(int parent, D3VECTOR *pos, int count, char *tex, OCMDTEXFLAGS flags, float size,
                             D3EMITTERCFG *cfg)
{
    D3PARTSYS *ps = 0;

    if (cfg->type == 0)
        ps = par_Create(count, tex, flags.tex, size, (void *)parent, par_EmitterCB0);
    else if (cfg->type == 1)
        ps = par_Create(count, tex, flags.tex, size, (void *)parent, par_EmitterCB1);
    else if (cfg->type == 2)
        ps = par_Create(count, tex, flags.tex, size, (void *)parent, par_EmitterCB2);
    if (ps) {
        par_SetPos(ps, pos);
        D3_COPY((char *)ps + 0x2c, cfg, sizeof(D3EMITTERCFG));
        par_SetDir(ps, (D3VECTOR *)PAR_EMITTER(ps)->dir, 0.0f);
        if (PAR_EMITTER(ps)->triggeronce) {
            ps->flags = 0;
            PAR_EMITTER(ps)->trigger = 1;
        }
    }
    return ps;
}
