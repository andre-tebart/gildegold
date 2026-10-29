/* 0x43b920, sets the object rotation from x / y / z through d3_SetObjectRot. */

void d3_SetObjectRotXYZ(D3OBJECT *obj, float x, float y, float z)
{
    D3VECTOR v;

    v.x = x;
    v.y = y;
    v.z = z;
    d3_SetObjectRot(obj, &v);
}
