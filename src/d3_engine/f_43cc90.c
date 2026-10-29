/* 0x43cc90, frees one surface of a mesh: fades the surface out through d3_SetObjectAlpha (alpha 0xff, not
   modulated) while it is still visible, then frees the keys, the triangles, the texture handles and the
   three texture names. */

void d3_FreeSurf(D3SURF *s)
{
    union {
        int i;
        D3TEXFLAGS f;
    } flags;
    int i;
    D3SURFTEX *t;

    if (s->flags.f.b[0] < 0xff || (s->flags.f.b[2] & 1)) {
        flags.i = 0;
        flags.f.b[2] = 0;
        flags.f.b[0] = 0xff;
        d3_SetObjectAlpha(s->owner, &s->info, flags.f);
    }
    if (s->info.nkeys > 0 && s->info.keys) {
        mem_Free(s->info.keys);
        s->info.keys = 0;
        s->info.nkeys = 0;
    }
    if (s->info.ntris > 0 && s->info.tris) {
        mem_Free(s->info.tris);
        s->info.tris = 0;
        s->info.ntris = 0;
    }
    if (s->texHandles) {
        if (s->info.mesh)
            for (i = 0; i < D3_OPS(&s->info)->count; i++)
                if (((void **)s->texHandles)[i])
                    tex_Release(((void **)s->texHandles)[i]);
        mem_Free((void *)s->texHandles);
        s->texHandles = 0;
    }
    if (s->info.mesh) {
        D3_OPS(&s->info)->Release((D3OPS *)s->info.mesh);
        s->info.mesh = 0;
    }
    for (i = 0, t = s->slots; i < 3; i++, t++)
        if (t->data) {
            d3anim_Release(t->data, 0);
            t->data = 0;
        }
}
