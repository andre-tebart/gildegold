/* 0x4370e0 */

void d3d_Exit(void)
{
    mem_Free(d3d_VertexBuffer);
    d3d_VertexBuffer = 0;
    if (d3d_ZBuffer) {
        if (dd_BackBuffer) {
#line 737
            D3DCHECK(dd_BackBuffer->lpVtbl->DeleteAttachedSurface(dd_BackBuffer, 0, d3d_ZBuffer));
        }
        d3d_RefCount = d3d_ZBuffer->lpVtbl->Release(d3d_ZBuffer);
        d3d_ZBuffer = 0;
    }
    if (d3d_Viewport) {
#line 742
        D3DCHECK(d3d_Device->lpVtbl->DeleteViewport(d3d_Device, d3d_Viewport));
        d3d_RefCount = d3d_Viewport->lpVtbl->Release(d3d_Viewport);
        d3d_Viewport = 0;
    }
    if (d3d_Device) {
        d3d_RefCount = d3d_Device->lpVtbl->Release(d3d_Device);
        d3d_Device = 0;
    }
    if (d3d_D3D) {
        d3d_RefCount = d3d_D3D->lpVtbl->Release(d3d_D3D);
        d3d_D3D = 0;
    }
    d3d_ZEnabled = 0;
}
