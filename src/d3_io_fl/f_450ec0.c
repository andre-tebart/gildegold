/* 0x450ec0, radius and the 8 corners of the bounding box (appended to the vertices) of a mesh @deep Q4: the 24 corner stores are *(float *)((char *)m->verts + N + m->nverts * 0x18); a typed D3BGFVERT view (vt[c].x/.y/.z) changes the codegen and loses the byte match (tried, reverted to the byte-identical form); behaviour byte */

void d3_CalcBounds(D3BGF *m)
{
    float r, d;
    float *v;
    int i, n;
    unsigned int k;
    float b[6];
    float z1;

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
    m->radius = r;
    if (n > 0) {
        for (k = 0; (int)k < 6; k++) {
            if (k & 1)
                b[k] = -1e10f;
            else
                b[k] = 1e10f;
        }
        z1 = b[5];
        if (m->nverts > 0) {
        v = m->verts;
        for (i = n; i; i--) {
            if (v[0] < b[0])
                b[0] = v[0];
            if (v[0] > b[1])
                b[1] = v[0];
            if (v[1] < b[2])
                b[2] = v[1];
            if (v[1] > b[3])
                b[3] = v[1];
            if (v[2] < b[4])
                b[4] = v[2];
            if (z1 < v[2])
                z1 = v[2];
            v += 6;
        }
        }
        m->verts[m->nverts * 6] = b[0];
        *(float *)((char *)m->verts + 4 + m->nverts * 0x18) = b[2];
        *(float *)((char *)m->verts + 8 + m->nverts * 0x18) = b[4];
        m->verts[(m->nverts + 1) * 6] = b[1];
        *(float *)((char *)m->verts + 0x1c + m->nverts * 0x18) = b[2];
        *(float *)((char *)m->verts + 0x20 + m->nverts * 0x18) = b[4];
        m->verts[(m->nverts + 2) * 6] = b[0];
        *(float *)((char *)m->verts + 0x34 + m->nverts * 0x18) = b[3];
        *(float *)((char *)m->verts + 0x38 + m->nverts * 0x18) = b[4];
        m->verts[(m->nverts + 3) * 6] = b[1];
        *(float *)((char *)m->verts + 0x4c + m->nverts * 0x18) = b[3];
        *(float *)((char *)m->verts + 0x50 + m->nverts * 0x18) = b[4];
        m->verts[(m->nverts + 4) * 6] = b[0];
        *(float *)((char *)m->verts + 0x64 + m->nverts * 0x18) = b[2];
        *(float *)((char *)m->verts + 0x68 + m->nverts * 0x18) = z1;
        m->verts[(m->nverts + 5) * 6] = b[1];
        *(float *)((char *)m->verts + 0x7c + m->nverts * 0x18) = b[2];
        *(float *)((char *)m->verts + 0x80 + m->nverts * 0x18) = z1;
        m->verts[(m->nverts + 6) * 6] = b[0];
        *(float *)((char *)m->verts + 0x94 + m->nverts * 0x18) = b[3];
        *(float *)((char *)m->verts + 0x98 + m->nverts * 0x18) = z1;
        m->verts[(m->nverts + 7) * 6] = b[1];
        *(float *)((char *)m->verts + 0xac + m->nverts * 0x18) = b[3];
        *(float *)((char *)m->verts + 0xb0 + m->nverts * 0x18) = z1;
    }
}
