/* 0x43c670, frees the drawable data of an object (the mirror state when obj is the mirror, its mirror
   mesh, light records, shadows and mesh) before the object is reloaded. */

void d3_FreeObjectData(D3OBJECT *obj)
{
    D3SCENE *scene;

    if (obj) {
        scene = obj->scene;
        if (d3_Mirror.obj == obj) {
            if (d3_Mirror.planes) {
                mem_Free(d3_Mirror.planes);
                d3_Mirror.planes = 0;
            }
            d3_Mirror.obj = 0;
            d3_Mirror.info = 0;
        }
        if (obj->type == 1 && D3_X(obj)->type0 != 1)
            obj->type = D3_X(obj)->type0;
        if (obj->material && D3_MESH(obj)->mirrorSource) {
            if (D3_MESH(obj)->mirrorPoints) {
                mem_Free(D3_MESH(obj)->mirrorPoints);
                D3_MESH(obj)->mirrorPoints = 0;
            }
            if (D3_MESH(obj)->mirrorPolys) {
                mem_Free(D3_MESH(obj)->mirrorPolys);
                D3_MESH(obj)->mirrorPolys = 0;
            }
        }
        if (obj->type == 6 || (obj->type == 5 && obj->name[0] == 'r'))
            d3_LightRemoveRecords(obj, 0);
        d3_ForEachObject(scene, 0, 7, (D3OBJECTCB)d3_LightRemoveRecords, obj);
        if (obj->type == 8)
            d3_ForEachObject(scene, 0, 0x40, (D3OBJECTCB)d3s_FreeLightCB, obj);
        if (obj->objectFlags & 4)
            d3s_FreeShadows(obj);
        d3_FreeMesh(obj);
    }
}
