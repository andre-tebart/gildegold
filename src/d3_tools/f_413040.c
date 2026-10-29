/* 0x413040, widens mn/mx by the bounding boxes of obj and its children */

void d3t_GetBounds(D3VECTOR *mn, D3VECTOR *mx, D3OBJECT *obj)
{
    D3BOXVERT *v;
    D3OBJECT *child;
    int i;

    if (!obj->info)
        return;
    d3_TransformSurf(obj);
    d3_UpdateObject(obj, 1);
    v = (D3BOXVERT *)(obj->info->keys + obj->info->nkeys);
    for (i = 0; i < 8; i++, v++) {
        mn->x = min(mn->x, v->x);
        mn->y = min(mn->y, v->y);
        mn->z = min(mn->z, v->z);
        mx->x = max(mx->x, v->x);
        mx->y = max(mx->y, v->y);
        mx->z = max(mx->z, v->z);
    }
    for (child = obj->child; child; child = child->next)
        d3t_GetBounds(mn, mx, child);
}
