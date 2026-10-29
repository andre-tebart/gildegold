/* 0x448540, d3_ForEachObject callback of d3if_Pick: keeps, per object type, the object whose projected
   bounding sphere contains the point and whose centre is nearest to it */

char d3if_PickCB(D3OBJECT *obj, D3IFPICK *p)
{
    D3VECTOR4 pos;
    float r, f, x, y, d;
    D3IFHIT *hit;

    if (p->mask && !(obj->mask & p->mask))
        return 1;
    if (D3_MESH(obj) && (D3_MESH(obj)->flags & 0x40) && D3_MESH(obj)->camera == p->cam)
        return 1;
    if (obj->type == 4 && !D3_MESH(obj))
        return 1;
    if (!d3_GetBounds(obj, p->cam, (D3VECTOR *)&pos, &r))
        return 1;
    f = 1.0f / pos.z;
    r = d3_ScaleX * f * r;
    x = d3_ScaleX * pos.x * f + p->dx;
    y = d3_ScaleY * pos.y * f + p->dy;
    hit = &p->hits[obj->type];
    d = x * x + y * y;
    if (d < hit->dist && (float)sqrt(d) < r) {
        hit->dist = d;
        hit->obj = obj;
    }
    return 1;
}
