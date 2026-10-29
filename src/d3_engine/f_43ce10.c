/* 0x43ce10, initializes one surface: flags (alpha 0xff, the modulation bit clear), empty geometry and the
   owner object; the three animation slots are reset (play flag off, no data, no frame). */

void d3_InitSurf(D3SURF *s, D3OBJECT *owner)
{
    int i;
    int *t;

    s->flags.i = 0;
    s->flags.f.b[0] = 0xff;
    s->info.keys = 0;
    s->info.nkeys = 0;
    s->info.tris = 0;
    s->info.ntris = 0;
    s->info.mesh = 0;
    s->owner = owner;
    for (i = 0, t = &s->slots[0].nextFrame; i < 3; i++, t += 0x1f) {
        ((D3SURFTEX *)(t - 1))->playFlags &= 0xfd;
        ((D3SURFTEX *)(t - 1))->data = 0;
        ((D3SURFTEX *)(t - 1))->nextFrame = 0;
        ((D3SURFTEX *)(t - 1))->subFrame = 0;
        ((D3SURFTEX *)(t - 1))->frame = 0;
        ((D3SURFTEX *)(t - 1))->flags = 0;
    }
}
