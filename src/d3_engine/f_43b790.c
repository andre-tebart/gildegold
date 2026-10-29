/* 0x43b790 */

void d3_SetObjectPos(D3OBJECT *obj, D3VECTOR *pos)
{
    if (obj == d3_Camera)
        d3_UpdateAll(1);
    else
        d3_UpdateObject(obj, 1);
    d3_ForEachObject2(d3_DefaultScene, obj, 0xc0, d3s_ResetShadows);
    obj->pos.x = pos->x;
    obj->pos.y = pos->y;
    obj->pos.z = pos->z;
}
