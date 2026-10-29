/* 0x43b7f0, sets the object position from x / y / z through d3_SetObjectPos. */

void d3_SetObjectPosXYZ(D3OBJECT *obj, float x, float y, float z)
{
    D3VECTOR4 v;

    v.x = x;
    v.y = y;
    v.z = z;
    d3_SetObjectPos(obj, (D3VECTOR *)&v);
}
