/* 0x40ef90, fills the emitter template d3_EmitterCfg (type *a, the default speeds / sizes, the direction d3_YAxis,
   the flags from *f0 / *f1) and creates a *count emitter of the texture *tex and size *size at the origin,
   attached to *parent; returns it */

int ocmd_CreateEmitter(int *a, int *count, int *parent, int *size, char **tex, int *f0, int *f1)
{
    OCMDTEXFLAGS flags;

    D3_ZERO_AT(offset d3_EmitterCfg, sizeof(d3_EmitterCfg));
    d3_EmitterCfg.type = *a;
    d3_EmitterCfg.f34 = 10.0f;
    d3_EmitterCfg.f38 = 50.0f;
    d3_EmitterCfg.f3c = 30.0f;
    d3_EmitterCfg.dir = d3_YAxis;
    d3_EmitterCfg.f80 = 1.0f;
    d3_EmitterCfg.f8c = 255.0f;
    d3_EmitterCfg.i90 = 20;
    d3_EmitterCfg.i94 = 160;
    d3_EmitterCfg.i98 = 180;
    d3_EmitterCfg.c9e = 0xff;
    d3_EmitterCfg.c9d = 0xff;
    d3_EmitterCfg.c9c = 0xff;
    d3_EmitterCfg.c9f = 0;
    d3_EmitterCfg.flags |= 0x20;
    flags.f0 = *f1;
    flags.f1 = 1;
    flags.b0 = *f0;
    return (int)par_CreateEmitter(*count, &d3_Origin, *parent, *tex, flags, (float)*size, &d3_EmitterCfg);
}
