/* 0x4364f0, the vertex buffer and its size in vertices. */

int d3d_GetVertexBuffer(void **buf, int *n)
{
    *buf = d3d_VertexBuffer;
    *n = 0x800;
    return 0;
}
