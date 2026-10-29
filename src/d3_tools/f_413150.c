/* 0x413150, bounding box of obj and its children with the camera at the origin */

void d3t_GetObjectBounds(D3VECTOR *mn, D3VECTOR *mx, D3OBJECT *obj)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3BOXVERT *v;
    D3OBJECT *child;
    int i;

    if (!obj->info)
        return;
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    d3_TransformSurf(obj);
    d3_UpdateObject(obj, 1);
    d3_UpdateWorldMatrix(obj, d3_Camera, 1);
    v = (D3BOXVERT *)(obj->info->keys + obj->info->nkeys);
    mn->x = v->x;
    mn->y = v->y;
    mn->z = v->z;
    mx->x = v->x;
    mx->y = v->y;
    mx->z = v->z;
    for (i = 1, v++; i < 8; i++, v++) {
        mn->x = min(mn->x, v->x);
        mn->y = min(mn->y, v->y);
        mn->z = min(mn->z, v->z);
        mx->x = max(mx->x, v->x);
        mx->y = max(mx->y, v->y);
        mx->z = max(mx->z, v->z);
    }
    for (child = obj->child; child; child = child->next)
        d3t_GetBounds(mn, mx, child);
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
    d3_SetObjectRot(d3_Camera, &rot);
}
