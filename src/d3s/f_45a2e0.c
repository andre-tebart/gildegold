/* 0x45a2e0, a shadow triangle: edges from the three points p (x, y pairs), colours c, radius pixels / rows around @deep B code hunks 215: the edge selection and the two span calls are the same operations with b/c/d/l and y in other registers / homes (original b in ebx, d in memory, y in ebp; ours d in esi, b in memory, y in esi) and the same stores in another order; no missing statement, constant or call - 72 selection/entry statement orders were tried (notes/units/d3s.md [shfix]). Q4: 2 raw byte views of flags remain */

void d3s_ShadowTriangle(float *p, unsigned char *c, unsigned char *dst, int pitch, unsigned int flags, char *base, int radius)
{
    int i, t, b, d, e, y, n, l;
    int minY;
    int left;
    float area;

    minY = pitch << 16;
    i = 0;
    t = -1;
    if (p[0] * p[5] + ((p[2] * p[1] + (p[3] * p[4] - p[2] * p[5])) - p[0] * p[3]) - p[4] * p[1] > 0.0) {
        do {
            d3s_EdgeX[i] = (int)(p[4 - i * 2] * 65536.0f);
            d3s_EdgeY[i] = (int)(p[5 - i * 2] * 65536.0f);
            d3s_EdgeC[i] = c[2 - i] << 16;
            if (d3s_EdgeY[i] <= minY) {
                minY = d3s_EdgeY[i];
                t = i;
            }
            i++;
        } while (i < 3);
    } else {
        do {
            d3s_EdgeX[i] = (int)(p[i * 2] * 65536.0f);
            d3s_EdgeY[i] = (int)(p[i * 2 + 1] * 65536.0f);
            d3s_EdgeC[i] = c[i] << 16;
            if (d3s_EdgeY[i] <= minY) {
                minY = d3s_EdgeY[i];
                t = i;
            }
            i++;
        } while (i < 3);
    }
    if (t != -1) {
        if (base == 0)
            *(unsigned char *)&flags &= 2;
        if (dst == 0)
            *(unsigned char *)&flags &= 5;
        if ((unsigned char)flags != 0) {
            e = d3s_Next[t * 2];
            if (d3s_EdgeY[e] == minY) {
                b = d3s_Next[t * 2 + 1];
                d = e;
                e = d3s_Next[e * 2];
                l = t;
            } else {
                l = d3s_Next[t * 2 + 1];
                if (d3s_EdgeY[l] == minY) {
                    b = d3s_Next[l * 2 + 1];
                    d = t;
                } else {
                    b = l;
                    d = t;
                    l = t;
                }
            }
            area = (p[0] - p[2]) * (p[5] - p[3]) - (p[4] - p[2]) * (p[1] - p[3]);
            if (area == 0.0f)
                d3s_Area = 0;
            else
                d3s_FtoI(&d3s_Area, 65536.0f / area * ((float)(c[0] - c[1]) * (p[5] - p[3]) - (float)(c[2] - c[1]) * (p[1] - p[3])));
            if (d3s_EdgeY[b] - d3s_EdgeY[l] > 0) {
                d3s_LeftEdgeColor(l, b);
                if (d3s_EdgeY[e] - d3s_EdgeY[d] > 0) {
                    d3s_RightEdge(d, e);
                    y = minY + 0xffff >> 16;
                    d3s_Dst = dst + y * pitch;
                    d3s_SpanRow = (D3SPANPIX *)(base + y * pitch * 0x1c);
                    d3s_ShadowRadius = radius;
                    left = d3s_EdgeY[b] < d3s_EdgeY[e];
                    n = ((left ? d3s_EdgeY[b] : d3s_EdgeY[e]) + 0xffff >> 16) - y;
                    d3s_DrawSpans(n, pitch, flags, 1, y);
                    if (d3s_EdgeY[b] != d3s_EdgeY[e]) {
                        if (left) {
                            d3s_LeftEdgeColor(b, e);
                            i = (d3s_EdgeY[e] + 0xffff >> 16) - (d3s_EdgeY[b] + 0xffff >> 16);
                        } else {
                            d3s_RightEdge(e, b);
                            i = (d3s_EdgeY[b] + 0xffff >> 16) - (d3s_EdgeY[e] + 0xffff >> 16);
                        }
                        d3s_DrawSpans(i, pitch, flags, 0, y + n);
                    }
                }
            }
        }
    }
}
