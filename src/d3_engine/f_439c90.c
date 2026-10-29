/* 0x439c90, clips the polygon in d3_ClipList[0] (d3_ClipIn vertices) at the planes of info
   (Sutherland-Hodgman); returns the vertex list of the result, d3_ClipOut vertices. */

__inline void d3_FtoI2(float f, int *dst)
{
    __asm fld f
    __asm mov edx, dst
    __asm fistp dword ptr [edx]
}

D3CVERT **d3_ClipPoly(D3CLIPINFO *info)
{
    D3CVERT **in, **out;
    D3CVERT *a, *b;
    D3CVERT *pool;
    unsigned int i, j;
    int ins, ins1;
    float d0, d1, t;
    D3VECTOR4 d;
    float df;
    int c;

    d3_ClipOut = 0;
    if (!((int)info->plane | info->n | (int)info))
        return 0;
    pool = d3_ClipPool;
    for (j = 0; j < info->n; j++) {
        d3_ClipOut = 0;
        in = d3_ClipList[j & 1];
        a = in[0];
        d0 = a->z * info->plane[j].z + a->y * info->plane[j].y + a->x * info->plane[j].x;
        in[d3_ClipIn] = a;
        out = d3_ClipList[(j - 1) & 1];
        ins = 1;
        if (!(d0 >= info->plane[j].w))
            ins = 0;
        for (i = 0; i < d3_ClipIn; i++, in++) {
            a = in[0];
            b = in[1];
            d1 = info->plane[j].x * b->x + (b->z * info->plane[j].z) + b->y * info->plane[j].y;
            ins1 = d1 >= info->plane[j].w;
            if (ins) {
                *out++ = a;
                d3_ClipOut++;
            }
            if (ins != ins1) {
                t = (info->plane[j].w - d0) / (d1 - d0);
                d.x = b->x - a->x;
                d.y = b->y - a->y;
                d.z = b->z - a->z;
                d.w = b->u - a->u;
                pool->x = d.x * t + a->x;
                pool->y = d.y * t + a->y;
                pool->z = d.z * t + a->z;
                pool->u = d.w * t + a->u;
                pool->v = (b->v - a->v) * t + a->v;
                d.x = (float)(b->col[0] - a->col[0]);
                d.y = (float)(b->col[1] - a->col[1]);
                d.z = (float)(b->col[2] - a->col[2]);
                d.w = (float)(b->col[3] - a->col[3]);
                df = (float)(b->fog - a->fog);
                d3_FtoI2(d.x * t + a->col[0], &c);
                pool->col[0] = (unsigned char)c;
                d3_FtoI2(d.y * t + a->col[1], &c);
                pool->col[1] = (unsigned char)c;
                d3_FtoI2(d.z * t + a->col[2], &c);
                pool->col[2] = (unsigned char)c;
                d3_FtoI2(d.w * t + a->col[3], &c);
                pool->col[3] = (unsigned char)c;
                d3_FtoI2(df * t + a->fog, &c);
                pool->fog = (unsigned char)c;
                *out++ = pool;
                d3_ClipOut++;
                pool++;
            }
            ins = ins1;
            d0 = d1;
        }
        d3_ClipIn = d3_ClipOut;
    }
    return d3_ClipList[j & 1];
}
