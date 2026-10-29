/* 0x439650, clips the 8 corners of a box (camera space) against the view frustum; returns the
   combined clip code and the box's z range. */

unsigned char d3_ClipBox(void *box, float *zmin, float *zmax)
{
    D3CLIP any, all, c;
    D3BOXVERT *v;
    float mn, mx;
    int i;

    any.raw = 0;
    all.raw = 0x3f;
    for (i = 0, v = box; i < 8; i++, v++) {
        c.raw = 0;
        c.left = d3_Planes[0].x * v->x + d3_Planes[0].y * v->y + d3_Planes[0].z * v->z < d3_Planes[0].w;
        c.right = d3_Planes[1].x * v->x + d3_Planes[1].y * v->y + d3_Planes[1].z * v->z < d3_Planes[1].w;
        c.top = d3_Planes[2].x * v->x + d3_Planes[2].y * v->y + d3_Planes[2].z * v->z < d3_Planes[2].w;
        c.bottom = d3_Planes[3].x * v->x + d3_Planes[3].y * v->y + d3_Planes[3].z * v->z < d3_Planes[3].w;
        c.znear = v->z < d3_ZNear;
        c.zfar = v->z > d3_ZFar;
        any.raw |= c.raw;
        all.raw &= c.raw;
    }
    any.out = all.raw != 0;
    if (!any.out) {
        mx = -1e10f;
        mn = 1e10f;
        for (i = 0, v = box; i < 8; i++, v++) {
            mn = min(mn, v->z);
            mx = max(mx, v->z);
        }
        if (zmin) {
            *zmin = mn;
            d3_MinZ = min(d3_MinZ, mn);
        }
        if (zmax) {
            *zmax = mx;
            d3_MaxZ = max(d3_MaxZ, mx);
        }
    }
    return any.raw;
}
