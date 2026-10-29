/* 0x4598d0, D3LOPS::fn1ec of the stock mesh driver (d3_io_t.c): the normals of a mesh without key frames, from the
   normal of each vertex's source point (a D3VECTOR at 0x0c of the source) in camera space */

void d3_TransformNormals(D3OBJECT *obj, D3LINFO *info)
{
    D3MATRIX m;
    D3LVERT *v;
    float *n;
    int i;

    d3_WorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 0, &m);
    v = info->keys;
    for (i = 0; i < info->nkeys; i++, v++) {
        n = (float *)v->src;
        v->nx = m._11 * n[3] + m._21 * n[4] + m._31 * n[5];
        v->ny = m._12 * n[3] + m._22 * n[4] + m._32 * n[5];
        v->nz = m._13 * n[3] + m._23 * n[4] + m._33 * n[5];
    }
}
