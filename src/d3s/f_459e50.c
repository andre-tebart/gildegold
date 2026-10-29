/* 0x459e50, as d3s_RightEdge with a second interpolated value (d3s_EdgeC, slope d3s_SlopeC, start d3s_CL) */

void d3s_LeftEdgeColor(int a, int b)
{
    int dy = d3s_EdgeY[b] - d3s_EdgeY[a];
    int q, d;

    if (dy < 0x10000) {
        q = 0x40000000 / dy;
        d3s_SlopeL = d3s_MulShr14(d3s_EdgeX[b] - d3s_EdgeX[a], q);
        d3s_SlopeC = d3s_MulShr14(d3s_EdgeC[b] - d3s_EdgeC[a], q);
    } else {
        d3s_SlopeL = d3s_ShlDiv16(d3s_EdgeX[b] - d3s_EdgeX[a], dy);
        d3s_SlopeC = d3s_ShlDiv16(d3s_EdgeC[b] - d3s_EdgeC[a], dy);
    }
    d = ((d3s_EdgeY[a] + 0xffff) & 0xffff0000) - d3s_EdgeY[a];
    d3s_XL = d3s_EdgeX[a] + d3s_MulShr16(d, d3s_SlopeL);
    d3s_CL = d3s_EdgeC[a] + d3s_MulShr16(d, d3s_SlopeC);
}
