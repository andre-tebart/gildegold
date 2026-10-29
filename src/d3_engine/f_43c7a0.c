/* 0x43c7a0, deletes an object with its children and following siblings of its group. */

void d3_DisposeObject(D3OBJECT *obj)
{
    int idx;
    int cur;
    char msg[0x400];
    D3SCENE *scene;
    D3PARTSYS *ps, *next;

    if (obj) {
        cur = d3_CurrentScene;
        scene = obj->scene;
        idx = -1;
        if (d3_DefaultScene != scene) {
            sprintf(msg, "d3_DisposeObject(): invalid universe by object %s", obj);
            idx = (unsigned int)((char *)scene - (char *)d3_Scenes) / sizeof(D3SCENE);
            d3if_SelectScene(idx, 1);
        }
        if (obj->material && D3_MESH(obj)->freeCallback)
            D3_MESH(obj)->freeCallback(obj);
        if (D3_X(obj)->events) {
            mem_Free(D3_X(obj)->events);
            D3_X(obj)->events = 0;
        }
        if (obj->type == 1 && D3_X(obj)->type0 != 1)
            obj->type = D3_X(obj)->type0;
        if (d3_Mirror.obj == obj) {
            if (d3_Mirror.planes) {
                mem_Free(d3_Mirror.planes);
                d3_Mirror.planes = 0;
            }
            d3_Mirror.obj = 0;
            d3_Mirror.info = 0;
        }
        for (ps = ((D3SCENELISTS *)scene)->parHead; ps != &d3_ParEnd; ps = next) {
            next = ps->next;
            if (ps->parent == obj)
                par_Delete(ps);
        }
        if (D3_X(obj)->path)
            d3anim_FreePath(obj);
        if (obj->state & 2)
            d3_UnlinkTop(obj);
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
        d3_ForEachObject(scene, 0, 0x87, (D3OBJECTCB)d3_LightRemoveRecords, obj);
        if (obj->type == 8)
            d3_ForEachObject(scene, 0, 0xc0, (D3OBJECTCB)d3s_FreeLightCB, obj);
        if (obj->objectFlags & 4)
            d3s_FreeShadows(obj);
        if (D3_X(obj)->light) {
            mem_Free(D3_X(obj)->light);
            D3_X(obj)->light = 0;
        }
        if (obj->child)
            d3_DisposeObject(obj->child);
        if (obj->next && !(obj->next->state & 1))
            d3_DisposeObject(obj->next);
        d3snd_Free(obj->sound);
        d3_FreeMesh(obj);
        mem_Free(obj);
        if (idx != -1)
            d3if_SelectScene(cur, 1);
    }
}
