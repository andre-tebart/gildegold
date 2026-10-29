/* 0x43ca20, appends obj to the top-level list of the default scene. */

void d3_LinkTop(D3OBJECT *obj)
{
    if (obj->type == 3 && !d3_Camera) {
        d3_SetCamera(obj);
        d3_SetObjectRot(obj, &obj->rotation);
    }
    if (obj->type) {
        obj->prev = d3_TopLast;
        obj->next = &d3_ObjEnd;
        d3_TopLast->next = obj;
        d3_TopLast = obj;
        obj->state |= 3;
    }
    obj->scene = d3_DefaultScene;
}
