/* 0x438f60, projects the visible vertices of a mesh to the screen, sets fog and flat colors, and
   culls back faces. */

/* D3OBJECT 0x5c of the object types that use it as a color (5..7); d3_light.h's D3LOBJ view names
   the same field color. A field (or anonymous union) in D3OBJECT itself flips d3_MirDrawObject. */
typedef struct D3OBJCOLOR {
    char pad000[0x5c];
    D3VECTOR color;
} D3OBJCOLOR;

void d3_ProjectMesh(D3OBJECT *obj)
{
    unsigned int n = obj->info->nkeys;
    unsigned int i;
    D3PVERT *v;
    D3PTRI *t;
    D3PVERT *v0, *v1, *v2;
    float rhw;
    unsigned long col;

    if (d3_Hardware) {
        if (d3_FogOn) {
            for (i = 0, v = (D3PVERT *)obj->info->keys; i < n; i++, v++) {
                if (v->flags & 0x80) {
                    rhw = 1.0f / v->z;
                    v->sx = d3_ScaleX * v->x * rhw + d3_CenterX;
                    v->sy = d3_ScaleY * v->y * rhw + d3_CenterY;
                    v->color = v->specular;
                    v->rhw = rhw;
                    d3_Dist2 = v->x * v->x + v->y * v->y + v->z * v->z;
                    v->fog = (unsigned char)(d3_Dist2 > d3_FogStart2
                        ? 255.0 - min(255.0, (sqrt(d3_Dist2) - d3_FogStart) * d3_FogScale) : 255.0);
                }
            }
        } else {
            for (i = 0, v = (D3PVERT *)obj->info->keys; i < n; i++, v++) {
                if (v->flags & 0x80) {
                    rhw = 1.0f / v->z;
                    v->sx = d3_ScaleX * v->x * rhw + d3_CenterX;
                    v->sy = d3_ScaleY * v->y * rhw + d3_CenterY;
                    v->color = v->specular;
                    v->rhw = rhw;
                }
            }
        }
        if (obj->type >= 5) {
            if (obj->type == 8)
                col = 0x1f1fff;
            else
                col = RGB_MAKE((unsigned char)((D3OBJCOLOR *)obj)->color.x,
                               (unsigned char)((D3OBJCOLOR *)obj)->color.y,
                               (unsigned char)((D3OBJCOLOR *)obj)->color.z);
            for (i = 0, v = (D3PVERT *)obj->info->keys; i < n; i++, v++)
                v->color = col;
        }
    } else {
        for (i = 0, v = (D3PVERT *)obj->info->keys; i < n; i++, v++) {
            if (v->flags & 0x80) {
                rhw = 1.0f / v->z;
                v->c.r = v->s.r >> 2;
                v->sx = d3_ScaleX * v->x * rhw + d3_CenterX;
                v->sy = d3_ScaleY * v->y * rhw + d3_CenterY;
                v->rhw = rhw;
            }
        }
    }
    n = obj->info->ntris;
    for (i = 0, t = (D3PTRI *)obj->info->tris; i < n; i++, t++) {
        if (t->flags & 0x80) {
            v0 = t->v[0];
            v1 = t->v[1];
            v2 = t->v[2];
            if (t->flags & 0x10)
                t->flags |= 0x40;
            else if ((v0->sx - v1->sx) * (v0->sy - v2->sy) < (v0->sx - v2->sx) * (v0->sy - v1->sy)
                     && !(t->drawFlags & 4))
                t->flags &= 0x7f;
        }
    }
}
