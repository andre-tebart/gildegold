/* 0x43b820, sets the object position offset (D3OBJECT.posOffset, added to pos by the update). */

void d3_SetObject078(D3OBJECT *obj, D3VECTOR *v)
{
    d3_UpdateObject(obj, 1);
    d3_ForEachObject2(d3_DefaultScene, obj, 0xc0, d3s_ResetShadows);
    obj->posOffset.x = v->x;
    obj->posOffset.y = v->y;
    obj->posOffset.z = v->z;
}
