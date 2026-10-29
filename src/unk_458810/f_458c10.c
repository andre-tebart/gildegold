/* 0x458c10, a point local to the object (before its own matrix) in world space: applies the object matrix and then the parent chain */

void d3_LocalToWorld(XOBJ *obj, float *in, float *out)
{
    D3VECTOR4 t;
    float x = in[0] + obj->p6c.x;
    float y = in[1] + obj->p6c.y;
    float z = in[2] + obj->p6c.z;

    t.x = x * obj->m._11 + y * obj->m._21 + z * obj->m._31;
    t.y = x * obj->m._12 + y * obj->m._22 + z * obj->m._32;
    t.z = x * obj->m._13 + y * obj->m._23 + z * obj->m._33;
    t.x = t.x - obj->p6c.x;
    t.y = t.y - obj->p6c.y;
    t.z = t.z - obj->p6c.z;
    t.x = t.x + obj->p4c.x;
    t.y = t.y + obj->p4c.y;
    t.z = t.z + obj->p4c.z;
    d3_PointToWorld((D3OBJECT *)obj, (D3VECTOR *)&t, (D3VECTOR *)out);
}
