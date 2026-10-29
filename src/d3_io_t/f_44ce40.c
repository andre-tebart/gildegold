/* 0x44ce40, radius and bounding box (8 corners appended to the vertices) of a mesh */

void d3iot_CalcBounds(D3BGF *m)
{
    float r, d, dz, z1;
    float *v, *v0;
    int i, n;
    D3VECTOR4 mn, mx;

    r = 0.0f;
    n = m->nverts;
    if (n > 0) {
        v = m->verts;
        for (i = n; i; i--) {
            d = (float)sqrt(v[1] * v[1] + v[0] * v[0] + v[2] * v[2]);
            if (d > r)
                r = d;
            v += 6;
        }
    }
    m->radius = m->diagonal = r;
    m->cx = 0.0f;
    m->cy = 0.0f;
    m->cz = 0.0f;
    if (n > 0) {
        mx.x = -1e10f;
        z1 = -1e10f;
        mx.y = -1e10f;
        mn.x = 1e10f;
        mn.y = 1e10f;
        mn.z = 1e10f;
        v0 = m->verts;
        v = m->verts;
        for (i = n; i; i--) {
            if (!(mn.x < v[0])) mn.x = v[0];
            mn.y = min(mn.y, v[1]);
            mn.z = min(mn.z, v[2]);
            if (!(mx.x > v[0])) mx.x = v[0];
            mx.y = max(mx.y, v[1]);
            z1 = max(z1, v[2]);
            v += 6;
        }
        v0[n * 6] = mn.x;
        D3BGF_CORNER(m, 0)->y = mn.y;
        D3BGF_CORNER(m, 0)->z = mn.z;
        m->verts[(m->nverts + 1) * 6] = mx.x;
        D3BGF_CORNER(m, 1)->y = mn.y;
        D3BGF_CORNER(m, 1)->z = mn.z;
        m->verts[(m->nverts + 2) * 6] = mn.x;
        D3BGF_CORNER(m, 2)->y = mx.y;
        D3BGF_CORNER(m, 2)->z = mn.z;
        m->verts[(m->nverts + 3) * 6] = mx.x;
        D3BGF_CORNER(m, 3)->y = mx.y;
        D3BGF_CORNER(m, 3)->z = mn.z;
        m->verts[(m->nverts + 4) * 6] = mn.x;
        D3BGF_CORNER(m, 4)->y = mn.y;
        D3BGF_CORNER(m, 4)->z = z1;
        m->verts[(m->nverts + 5) * 6] = mx.x;
        D3BGF_CORNER(m, 5)->y = mn.y;
        D3BGF_CORNER(m, 5)->z = z1;
        m->verts[(m->nverts + 6) * 6] = mn.x;
        D3BGF_CORNER(m, 6)->y = mx.y;
        D3BGF_CORNER(m, 6)->z = z1;
        m->verts[(m->nverts + 7) * 6] = mx.x;
        D3BGF_CORNER(m, 7)->y = mx.y;
        D3BGF_CORNER(m, 7)->z = z1;
        v = m->verts + m->nverts * 6;
        mx.x -= mn.x;
        mx.y -= mn.y;
        dz = z1 - mn.z;
        m->diagonal = (float)sqrt(dz * dz + mx.y * mx.y + mx.x * mx.x);
        mn.x = 0.0f;
        mn.y = 0.0f;
        mn.z = 0.0f;
        for (i = 8; i; i--) {
            mn.x += v[0];
            mn.y += v[1];
            mn.z += v[2];
            v += 6;
        }
        m->cx = mn.x * 0.125f;
        m->cy = mn.y * 0.125f;
        m->cz = mn.z * 0.125f;
    }
}
