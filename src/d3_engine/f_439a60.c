/* 0x439a60, clips the vertices of a mesh against the planes selected in mask and marks the triangles
   (and their vertices) that are not completely outside one plane as visible. */

void d3_ClipMesh(D3CLIP mask, D3PVERT *v, int nverts, D3PTRI *t, int ntris)
{
    D3CLIP *c;
    D3PVERT *v0, *v1, *v2;
    int i;

    for (i = 0; i < nverts; i++) {
        c = &v[i].clip;
        c->raw = 0;
        if (mask.left)
            c->left = d3_Planes[0].x * v[i].x + d3_Planes[0].z * v[i].z < d3_Planes[0].w;
        if (mask.right)
            c->right = d3_Planes[1].x * v[i].x + d3_Planes[1].z * v[i].z < d3_Planes[1].w;
        if (mask.top)
            c->top = d3_Planes[2].z * v[i].z + d3_Planes[2].y * v[i].y < d3_Planes[2].w;
        if (mask.bottom)
            c->bottom = d3_Planes[3].z * v[i].z + d3_Planes[3].y * v[i].y < d3_Planes[3].w;
        if (mask.znear)
            c->znear = v[i].z < d3_ZNear;
        if (mask.zfar)
            c->zfar = v[i].z > d3_ZFar;
    }
    for (i = 0; i < ntris; i++) {
        t[i].flags = 0;
        if (t[i].v[0] && !(t[i].drawFlags & 2)) {
            v0 = t[i].v[0];
            v1 = t[i].v[1];
            v2 = t[i].v[2];
            if (!((unsigned char)v0->flags & (unsigned char)v1->flags & (unsigned char)v2->flags & 0x3f)) {
                t[i].flags = ((unsigned char)v0->flags | (unsigned char)v1->flags | (unsigned char)v2->flags) & 0x3f | 0x80;
                v0->rawFlags |= 0x80;
                v1->rawFlags |= 0x80;
                v2->rawFlags |= 0x80;
            }
        }
    }
}
