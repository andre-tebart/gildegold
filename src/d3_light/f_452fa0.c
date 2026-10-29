/* 0x452fa0, sets the colours (c and s) of the vertices of the triangles of surface s from the texture's
   flags: the grey of the accumulated light scaled by the texture's two flag bytes, or the alpha byte alone.
   The callers pass on and fl, the body reads neither. */

void d3_SetVertexColors(D3OBJECT *obj, D3SURF *s, char on, D3TEXFLAGS fl)
{
    int i, k, g;
    D3PTRI *tri;
    D3TEXTURE *tex;
    D3TEXFLAGS f;
    D3PVERT *v;
    D3COLOR c;
    float x, b0, b1;
    unsigned char fl212 = obj->stateFlags;
    char keep = !(fl212 & 1);
    char pos = !(obj->objectFlags & 0x80);

    if (!(fl212 & 2)) {
        tri = (D3PTRI *)s->info.tris;
        for (i = 0; i < s->info.ntris; i++, tri++) {
            tex = tri->tex;
            if (tex) {
                if (keep)
                    ((D3BITS *)&tri->drawFlags)->b2 = ((D3BITS *)&tex->flags)->b4;
                if (pos)
                    f = ((D3TEXVIEW *)tex)->flags;
                else
                    f = s->flags.f;
                if (f.b[2] & 1) {
                    k = 0;
                    b1 = f.b[1];
                    b0 = f.b[0];
                    for (; k < 3; k++) {
                        v = tri->v[k];
                        x = min(255.0f, (float)((((D3LVERT *)v)->light.x * 0.3f + ((D3LVERT *)v)->light.y * 0.59f + ((D3LVERT *)v)->light.z * 0.11f + b1) * 0.00390625f) * b0);
                        d3_FtoI(&g, x);
                        c.r = c.g = c.b = c.a = (unsigned char)g;
                        v->c = c;
                        v->s = c;
                    }
                } else if (f.b[0] < 255) {
                    for (k = 0; k < 3; k++) {
                        v = tri->v[k];
                        v->c.a = f.b[0];
                        v->s.a = f.b[0];
                    }
                }
            }
        }
    }
}
