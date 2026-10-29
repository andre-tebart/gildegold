/* 0x436510, draws a triangle list from the vertex buffer. */

void d3d_DrawTriangles(void *unk, D3DTLVERTEX *buf, DWORD num)
{
    if (num > 2) {
#line 496
        D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3D_VERTEX_STRUCT, buf, num, D3DDP_DONOTUPDATEEXTENTS | D3DDP_DONOTCLIP | D3DDP_DONOTLIGHT));
    }
}
