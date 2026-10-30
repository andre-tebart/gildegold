/* 0x452ef0, camera space position and squared radius of obj */

void d3_LightPrepare(D3OBJECT *obj)
{
    float r = 0.0f;
    if (!obj->info) {
        obj->info = d3_SelectSurf(obj);
        if (!obj->info)
            return;
    }
    d3_PointToRefD(obj, d3_Camera, &obj->pos, (D3VECTOR4 *)&D3_L(obj)->cam);
    if (D3_L(obj)->info->ops)
        r = D3_L(obj)->info->ops->Radius(obj, obj->info);
    D3_L(obj)->radius2 = r * r;
    if (obj->state & 4)
        d3_TransformSurf(obj);
    if (obj->state & 0x20)
        if (D3_L(obj)->info->ops)
            D3_L(obj)->info->ops->fn1ec(obj, obj->info);
}
