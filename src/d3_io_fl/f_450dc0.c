/* 0x450dc0, face normals (stored behind each polygon) and the smoothed vertex normals */

void d3_CalcNormals(D3BGF *m)
{
    int i, j;
    float *v;
    D3BGFPOLY *p;
    D3VECTOR4 s;
    D3VECTOR *nrm;
    D3BGFVERT *vt;

    p = m->polys;
    for (i = 0; i < m->npolys; i++) {
        v = m->verts;
        d3_TriNormal((D3VECTOR4 *)(v + p[i].v[0] * 6), (D3VECTOR4 *)(v + p[i].v[1] * 6),
                     (D3VECTOR4 *)(v + p[i].v[2] * 6), (D3VECTOR4 *)p[i].pad2c);
    }
    vt = (D3BGFVERT *)m->verts;
    for (i = 0; i < m->nverts; i++, vt++) {
        s.x = 0.0f;
        s.y = 0.0f;
        s.z = 0.0f;
        p = m->polys;
        for (j = 0; j < m->npolys; j++) {
            if (p[j].v[0] == i || p[j].v[1] == i || p[j].v[2] == i) {
                s.x += ((D3VECTOR *)p[j].pad2c)->x;
                s.y += ((D3VECTOR *)p[j].pad2c)->y;
                s.z += ((D3VECTOR *)p[j].pad2c)->z;
            }
        }
        d3_Normalize(&s);
        vt->nx = s.x;
        vt->ny = s.y;
        vt->nz = s.z;
    }
}
