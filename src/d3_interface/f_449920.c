/* 0x449920, sets an object's local position and rotation from a world-space position and rotation (converted
   into the space of the object's parent) */

void d3_SetWorldPosRot(D3OBJECT *obj, float *pos, D3MATRIX *rot)
{
    D3VECTOR4 v;
    D3VECTOR a;

    d3_WorldToParent(obj, pos, rot, (D3VECTOR *)&v, &a);
    d3_SetObjectPos(obj, (D3VECTOR *)&v);
    d3_SetObjectRot(obj, &a);
}
