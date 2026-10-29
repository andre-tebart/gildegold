/* 0x458950, a direction in the object's space, relative to ref unless the object is in world space */

void d3_DirToRef(XOBJ *obj, XOBJ *ref, float *in, float *out)
{
    D3VECTOR4 t;

    if (!(obj->flags & 0x80) && ref) {
        d3_DirToWorld(obj, in, &t.x);
        out[0] = t.x * ref->m._11 + t.y * ref->m._21 + t.z * ref->m._31;
        out[1] = t.x * ref->m._12 + t.y * ref->m._22 + t.z * ref->m._32;
        out[2] = t.x * ref->m._13 + t.y * ref->m._23 + t.z * ref->m._33;
        return;
    }
    d3_DirToWorld(obj, in, out);
}
