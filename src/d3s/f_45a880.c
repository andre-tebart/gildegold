/* 0x45a880, draws one polygon of the shadow mesh into the shadow texture (bpp bits per pixel) @deep B code hunks 248: the edge selection, the bpp checks and the two span fills are the same operations in other registers / homes (original top in ebx loaded from the bpp slot per branch, minY in the size slot cached in ebp; ours minY in ebx, top in memory); 360 entry/selection statement orders, volatile minY and the <= 8 / < 9 forms were tried (notes/units/d3s.md [shfix]). Q4: 2 raw dword views of the byte bpp remain */

void d3s_ShadowPoly(D3SPOLY *poly, unsigned char bpp, int size)
{
    D3SVERT *v0, *v1, *v2;
    int i, top, lim;
    int a, b, c, y, n;
    int left;

    lim = size << 16;
    v0 = poly->v[0];
    v1 = poly->v[1];
    v2 = poly->v[2];
    size = lim;
    if ((v0->tu - v1->tu) * (v0->tv - v2->tv) < (v0->tu - v2->tu) * (v0->tv - v1->tv)) {
        if (!(poly->flags & 4))
            return;
        top = *(int *)&bpp;
        for (i = 0; i < 3; i++) {
            d3s_EdgeX[i] = (int)(poly->v[2 - i]->tu * 65536.0f);
            d3s_EdgeY[i] = (int)(poly->v[2 - i]->tv * 65536.0f);
            if (d3s_EdgeY[i] <= size) {
                size = d3s_EdgeY[i];
                top = i;
            }
        }
    } else {
        top = *(int *)&bpp;
        for (i = 0; i < 3; i++) {
            d3s_EdgeX[i] = (int)(poly->v[i]->tu * 65536.0f);
            d3s_EdgeY[i] = (int)(poly->v[i]->tv * 65536.0f);
            if (d3s_EdgeY[i] <= size) {
                size = d3s_EdgeY[i];
                top = i;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (d3s_EdgeX[i] < 0 || d3s_EdgeX[i] > lim - 1 || d3s_EdgeY[i] < 0 || d3s_EdgeY[i] > lim - 1)
            return;
    }
    a = d3s_Next[top * 2];
    if (d3s_EdgeY[a] == size) {
        b = d3s_Next[top * 2 + 1];
        c = top;
        top = a;
        a = d3s_Next[top * 2];
    } else {
        c = d3s_Next[top * 2 + 1];
        if (d3s_EdgeY[c] == size) {
            b = d3s_Next[c * 2 + 1];
        } else {
            b = c;
            c = top;
        }
    }
    if (d3s_EdgeY[b] - d3s_EdgeY[c] > 0) {
        d3s_LeftEdge(c, b);
        if (d3s_EdgeY[a] - d3s_EdgeY[top] > 0) {
            d3s_RightEdge(top, a);
            d3s_Byte = (unsigned char)bpp < 9;
            y = size + 0xffff >> 16;
            if (d3s_Byte) {
                d3s_Dst = (unsigned char *)d3s_ShadowBits + y * d3_Pitch8;
                d3s_Fill = 1;
            } else {
                d3s_Fill = 0xffff;
                d3s_Dst16 = (unsigned short *)d3s_ShadowBits + y * d3_Pitch16;
            }
            left = d3s_EdgeY[b] < d3s_EdgeY[a];
            n = d3s_EdgeY[left ? b : a] + 0xffff >> 16;
            d3s_FillRows(n - y);
            if (d3s_EdgeY[b] != d3s_EdgeY[a]) {
                if (left) {
                    d3s_LeftEdge(b, a);
                    d3s_FillRows((d3s_EdgeY[a] + 0xffff >> 16) - (d3s_EdgeY[b] + 0xffff >> 16));
                } else {
                    d3s_RightEdge(a, b);
                    d3s_FillRows((d3s_EdgeY[b] + 0xffff >> 16) - (d3s_EdgeY[a] + 0xffff >> 16));
                }
            }
        }
    }
}
