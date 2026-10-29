/* 0x43b860, sets the rotation angles (degrees) and rebuilds the object matrix; a type-3 object
   (the camera, d3_int_io.c) gets the matrix of the negated angles. */

void d3_SetObjectRot(D3OBJECT *obj, D3VECTOR *rot)
{
    D3VECTOR4 v;

    d3_UpdateObject(obj, 1);
    obj->rotation.x = rot->x;
    obj->rotation.y = rot->y;
    obj->rotation.z = rot->z;
    if (obj->type != 3) {
        d3_MatFromAngles((D3VECTOR4 *)&obj->rotation, &obj->matrix);
        d3_ForEachObject2(d3_DefaultScene, obj, 0xc0, d3s_ResetShadows);
        return;
    }
    if (obj == d3_Camera)
        d3_UpdateAll(0);
    v.x = -obj->rotation.x;
    v.y = -obj->rotation.y;
    v.z = -obj->rotation.z;
    d3_MatFromAngles(&v, &obj->matrix);
}
