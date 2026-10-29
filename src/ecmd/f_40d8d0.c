/* 0x40d8d0, creates the object *name at (*x, *y, *z) with a zero rotation and relights it; returns the object,
   0 on failure */

int ocmd_CreateObject(float *x, float *y, float *z, char **name)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3OBJECT *obj;

    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = *x;
    pos.y = *y;
    pos.z = *z;
    obj = d3_CreateObject(0, (D3VECTOR *)&pos, &rot, *name);
    if (obj)
        d3_RelightObject(obj);
    return (int)obj;
}
