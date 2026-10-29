/* 0x459980, normals of the object from the packed normals of key frame f (3 bytes per vertex) in camera space;
   without a frame the driver does it. Not the matrix d3_light.h declares for the third argument: it is a D3ANIMFRAME. */

void d3_FrameNormals(D3OBJECT *obj, D3LINFO *info, D3ANIMFRAME *f)
{
    D3MATRIX m;
    D3LVERT *v;
    float x, y, z;
    int i;

    if (!f) {
        info->ops->fn1ec(obj, (D3OBJINFO *)info);
        return;
    }
    d3_WorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 1, &m);
    v = info->keys;
    for (i = 0; i < info->nkeys; i++, v++) {
        x = (f->normals[i * 3] - 128.0f) * 0.007843138f;
        y = (f->normals[i * 3 + 1] - 128.0f) * 0.007843138f;
        z = (f->normals[i * 3 + 2] - 128.0f) * 0.007843138f;
        v->nx = m._11 * x + m._21 * y + m._31 * z;
        v->ny = m._12 * x + m._22 * y + m._32 * z;
        v->nz = m._13 * x + m._23 * y + m._33 * z;
    }
}
