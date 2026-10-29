/* 0x436de0, sets the viewport and the projection; both are cached. */

void d3d_SetViewport(float w, float h, float x, float y, float zn, float zf)
{
    if (d3d_Device && d3d_Viewport) {
        if (w != d3d_ViewW || h != d3d_ViewH || x != d3d_ViewX || y != d3d_ViewY) {
            D3DVIEWPORT2 viewData;

            D3_ZERO(&viewData, sizeof(viewData));
            viewData.dwSize = sizeof(viewData);
            viewData.dwX = (DWORD)x;
            viewData.dwY = (DWORD)y;
            viewData.dwWidth = (DWORD)w;
            viewData.dwHeight = (DWORD)h;
            viewData.dvClipX = -1.0f;
            viewData.dvClipY = h / w;
            viewData.dvClipWidth = 2.0f;
            viewData.dvClipHeight = viewData.dvClipY * 2.0f;
            viewData.dvMinZ = 0.0f;
            viewData.dvMaxZ = 1.0f;
#line 660
            D3DCHECK(d3d_Viewport->lpVtbl->SetViewport2(d3d_Viewport, &viewData));
            d3d_ViewW = w;
            d3d_ViewH = h;
            d3d_ViewX = x;
            d3d_ViewY = y;
        }
        if (zn != d3d_Near || zf != d3d_Far) {
            D3DMATRIX mat;

            D3_ZERO(&mat, sizeof(mat));
            mat._44 = 1.0f;
            mat._33 = 1.0f;
            mat._22 = 1.0f;
            mat._11 = 1.0f;
#line 673
            D3DCHECK(d3d_Device->lpVtbl->SetTransform(d3d_Device, D3DTRANSFORMSTATE_WORLD, &mat));
            D3DCHECK(d3d_Device->lpVtbl->SetTransform(d3d_Device, D3DTRANSFORMSTATE_VIEW, &mat));
            mat._44 = zn;
            mat._34 = 1.0f;
            mat._33 = zn / (zf - zn) + 1.0f;
            D3DCHECK(d3d_Device->lpVtbl->SetTransform(d3d_Device, D3DTRANSFORMSTATE_PROJECTION, &mat));
            d3d_Near = zn;
            d3d_Far = zf;
        }
    }
}
