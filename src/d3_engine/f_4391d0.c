/* 0x4391d0, sets up the view frustum planes (d3_Planes), the corner directions (d3_Frustum) and the
   plane lists of the 64 clip codes (d3_ClipInfo).
   @deep behaviour draft: the 152 code hunks are the same instructions scheduled differently - the original keeps the atan2 results a/b on the FPU stack through sin/cos (fld st(0)/fsin/fld st(1)/fcos at 0x439242..0x4392b8) where VC6 reloads from the local; the je/jbe in the popcount loop (0x4394fd) is the same test (cmp edi,esi with esi=0) and the info->plane store order (+8/+0xc, +0x10/+0x14) is swapped only; recorded attempts: chained/parenthesized assignments, s/c locals, double CSE of sin/cos, 15 statement orders (notes/units/d3_engine.md) */

void d3_SetupFrustum(void)
{
    D3MATRIX m;
    D3VECTOR rot;
    float a, b, na, nb, s, c;
    unsigned int i, bits;
    int n, k;
    D3CLIPINFO *info;

    /* */
    d3_Planes[0].y = 0.0f;
    d3_Planes[0].w = -2e-6f;
    d3_Planes[1].y = 0.0f;
    d3_Planes[1].w = 2e-6f;
    d3_Planes[2].x = 0.0f;
    d3_Planes[2].w = 2e-6f;
    d3_Planes[3].x = 0.0f;
    d3_Planes[3].w = -2e-6f;
    rot.z = 0.0f;
    d3_FovX = (a = (float)atan2(d3_HalfW, d3_ScaleX));
    d3_Planes[0].x = cos(a);
    d3_Planes[0].z = (s = sin(a));
    d3_Planes[1].x = -cos(a);
    d3_Planes[1].z = s;
    d3_FovY = (b = (float)atan2(d3_HalfH, -d3_ScaleY));
    d3_Planes[2].y = cos(b);
    d3_Planes[2].z = (s = sin(b));
    d3_Planes[3].y = -cos(b);
    d3_Planes[3].z = s;
    rot.x = nb = -b;
    rot.y = na = -a;
    d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
    d3_Frustum[0].x = 0.0f * m._11 + 0.0f * m._21 + m._31;
    d3_Frustum[0].y = 0.0f * m._12 + 0.0f * m._22 + m._32;
    d3_Frustum[0].z = 0.0f * m._13 + 0.0f * m._23 + m._33;
    rot.x = nb;
    rot.y = a;
    rot.z = 0.0f;
    d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
    d3_Frustum[1].x = 0.0f * m._11 + 0.0f * m._21 + m._31;
    d3_Frustum[1].y = 0.0f * m._12 + 0.0f * m._22 + m._32;
    d3_Frustum[1].z = 0.0f * m._13 + 0.0f * m._23 + m._33;
    rot.x = b;
    rot.y = na;
    rot.z = 0.0f;
    d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
    d3_Frustum[2].x = 0.0f * m._11 + 0.0f * m._21 + m._31;
    d3_Frustum[2].y = 0.0f * m._12 + 0.0f * m._22 + m._32;
    d3_Frustum[2].z = 0.0f * m._13 + 0.0f * m._23 + m._33;
    rot.x = b;
    rot.y = a;
    rot.z = 0.0f;
    d3_MatFromAngles((D3VECTOR4 *)&rot, &m);
    d3_Frustum[3].x = 0.0f * m._11 + 0.0f * m._21 + m._31;
    d3_Frustum[3].y = 0.0f * m._12 + 0.0f * m._22 + m._32;
    d3_Frustum[3].z = 0.0f * m._13 + 0.0f * m._23 + m._33;
    d3_Frustum[4].x = 0.0f;
    d3_Frustum[4].y = 0.0f;
    d3_Frustum[4].z = 1.0f;
    d3_Planes[4].x = 0.0f;
    d3_Planes[4].y = 0.0f;
    d3_Planes[4].z = 1.0f;
    d3_Planes[4].w = d3_ZNear;
    d3_Planes[5].x = 0.0f;
    d3_Planes[5].y = 0.0f;
    d3_Planes[5].z = -1.0f;
    d3_Planes[5].w = -d3_ZFar;
    for (i = 0; i < 64; i++) {
        info = (D3CLIPINFO *)d3_ClipInfo[i];
        n = 0;
        for (bits = i; bits; bits >>= 1)
            if (bits & 1)
                n++;
        info->n = n;
        info->mirror = 0;
        k = 0;
        if (i & 1) {
            info->plane[k].x = d3_Planes[0].x;
            info->plane[k].y = d3_Planes[0].y;
            info->plane[k].z = d3_Planes[0].z;
            info->plane[k].w = d3_Planes[0].w;
            k++;
        }
        if (i & 2) {
            info->plane[k].x = d3_Planes[1].x;
            info->plane[k].y = d3_Planes[1].y;
            info->plane[k].z = d3_Planes[1].z;
            info->plane[k].w = d3_Planes[1].w;
            k++;
        }
        if (i & 4) {
            info->plane[k].x = d3_Planes[2].x;
            info->plane[k].y = d3_Planes[2].y;
            info->plane[k].z = d3_Planes[2].z;
            info->plane[k].w = d3_Planes[2].w;
            k++;
        }
        if (i & 8) {
            info->plane[k].x = d3_Planes[3].x;
            info->plane[k].y = d3_Planes[3].y;
            info->plane[k].z = d3_Planes[3].z;
            info->plane[k].w = d3_Planes[3].w;
            k++;
        }
        if (i & 0x10) {
            info->plane[k].x = 0.0f;
            info->plane[k].y = 0.0f;
            info->plane[k].z = 1.0f;
            info->plane[k].w = d3_ZNear;
            k++;
        }
        if (i & 0x20) {
            info->plane[k].x = 0.0f;
            info->plane[k].y = 0.0f;
            info->plane[k].z = -1.0f;
            info->plane[k].w = d3_Planes[5].w;
            k++;
        }
    }
}
