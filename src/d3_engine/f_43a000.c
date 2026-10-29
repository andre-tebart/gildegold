/* 0x43a000, the surface (level of detail) to draw: from the distance to the camera, or fixed by
   flags bits 4-5. */

D3OBJINFO *d3_SelectSurf(D3OBJECT *obj)
{
    D3SURF *s = 0;
    D3MESH *m = D3_MESH(obj);
    D3VECTOR4 d;
    float f, fn;
    int n, lod;

    if (m && m->nsurf > 0) {
        if (!(obj->flags & 0x30) && d3_Camera) {
            d.x = obj->pos.x;
            d.y = obj->pos.y;
            d.z = obj->pos.z;
            d.x -= d3_Camera->pos.x;
            d.y -= d3_Camera->pos.y;
            d.z -= d3_Camera->pos.z;
            n = D3_MESH(obj)->nsurf;
            fn = (float)n;
            f = ((float)sqrt(d.x * d.x + d.y * d.y + d.z * d.z) * fn) * d3_InvZFar;
            if ((int)f >= n)
                f = fn - 1.0f;
            else if (f < 0.0f)
                f = 0.0f;
            s = &m->surf[(int)f];
        } else {
            lod = ((obj->flags & 0x30) >> 4) - 1;
            s = &m->surf[max(0, min(m->nsurf - 1, lod))];
        }
        if (!s->info.nkeys || !s->info.ntris)
            s = 0;
        else if ((D3OBJINFO *)s != obj->info || d3_LightsChanged)
            obj->state |= 0x40;
    }
    return (D3OBJINFO *)s;
}
