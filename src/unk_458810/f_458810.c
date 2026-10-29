/* 0x458810, rotates a direction by the object's matrix and then by those of its parents */
void d3_DirToWorld(XOBJ *obj, float *in, float *out)
{
    XOBJ *p;
    struct { float x, y; volatile float z; float w; } t;

    p = obj->parent;
    out[0] = obj->m._31 * in[2] + obj->m._11 * in[0] + obj->m._21 * in[1];
    out[1] = obj->m._32 * in[2] + obj->m._12 * in[0] + obj->m._22 * in[1];
    out[2] = obj->m._33 * in[2] + obj->m._13 * in[0] + obj->m._23 * in[1];
    for (; p; p = p->parent) {
        t.x = p->m._21 * out[1] + p->m._31 * out[2] + p->m._11 * out[0];
        t.y = p->m._22 * out[1] + p->m._32 * out[2] + p->m._12 * out[0];
        t.z = p->m._23 * out[1] + p->m._33 * out[2] + p->m._13 * out[0];
        out[0] = t.x;
        out[2] = t.z;
        out[1] = t.y;
    }
}
