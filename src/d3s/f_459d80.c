/* 0x459d80, x slope (d3s_SlopeR) and the x at the first pixel row of the edge from vertex a to vertex b */
void d3s_RightEdge(int a, int b)
{
    int dy = d3s_EdgeY[b] - d3s_EdgeY[a];

    if (dy < 0x10000)
        d3s_SlopeR = d3s_MulShr14(d3s_EdgeX[b] - d3s_EdgeX[a], 0x40000000 / dy);
    else
        d3s_SlopeR = d3s_ShlDiv16(d3s_EdgeX[b] - d3s_EdgeX[a], dy);
    d3s_XR = d3s_EdgeX[a]
        + d3s_MulShr16(((d3s_EdgeY[a] + 0xffff) & 0xffff0000) - d3s_EdgeY[a], d3s_SlopeR);
}
