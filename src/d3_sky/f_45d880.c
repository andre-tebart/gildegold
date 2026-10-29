/* 0x45d880, creates a sky layer with texture tex and links it into s: at the head when first, else at the
   tail; returns the layer, 0 when s or tex is missing or the allocation fails */

D3SKYLAYER *d3sky_CreateLayer(D3SKY *s, int first, char *tex, D3TEXFLAGS flags, float scale, float offset,
                              float vscale, float voffset)
{
    D3SKYLAYER *l, *ret = 0;
    D3SKYLAYER *p;

    if (s && tex) {
      if (l = mem_Alloc(sizeof(D3SKYLAYER), "d3_sky:CreateLayer")) {
        l->scale = scale;
        l->offset = offset;
        l->speed = 0;
        l->vscale = vscale;
        l->voffset = voffset;
        l->alpha0 = flags.b[0];
        l->alpha1 = flags.b[0];
        l->fade = 1.0f;
        l->fadespeed = 0;
        l->flags = flags;
        l->t = tex_Load(tex, flags, 0, 0);
        if (l->t)
            tex_Reload(l->t, 0);
        if (!first) {
            l->next = 0;
            if (s->layers) {
                for (p = s->layers; p->next; p = p->next)
                    ;
                p->next = l;
                return l;
            }
            /* s->layers = l in both arms, not once after the if: the extra s references give s edi (VC6) */
            s->layers = l;
        } else {
            l->next = s->layers;
            s->layers = l;
        }
      }
      ret = l;
    }
    return ret;
}
