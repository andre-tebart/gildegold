/* 0x458b20, d3_PointToWorld relative to ref unless the object is in world space */

void d3_PointToRef(XOBJ *obj, XOBJ *ref, float *in, float *out)
{
    D3VECTOR4 t;

    if (!(obj->flags & 0x80) && ref) {
        d3_PointToWorld((D3OBJECT *)obj, (D3VECTOR *)in, &t);
        t.x = t.x - ref->p4c.x;
        t.y = t.y - ref->p4c.y;
        t.z = t.z - ref->p4c.z;
        t.x = t.x - ref->p78.x;
        t.y = t.y - ref->p78.y;
        t.z = t.z - ref->p78.z;
        out[0] = t.x * ref->m._11 + t.y * ref->m._21 + t.z * ref->m._31;
        out[1] = t.x * ref->m._12 + t.y * ref->m._22 + t.z * ref->m._32;
        out[2] = t.x * ref->m._13 + t.y * ref->m._23 + t.z * ref->m._33;
        return;
    }
    d3_PointToWorld((D3OBJECT *)obj, (D3VECTOR *)in, (D3VECTOR *)out);
}
