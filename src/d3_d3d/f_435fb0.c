/* 0x435fb0, clears the z buffer and the back buffer unless already done this frame. */

void d3d_Clear(D3RECT *r)
{
    D3DRECT mr;
    DWORD fl;

    if (!dd_ZCleared || !dd_BackCleared) {
        fl = 0;
        if (d3d_ZBuffer && !dd_ZCleared)
            fl = D3DCLEAR_ZBUFFER;
        if (dd_BackBuffer && !dd_BackCleared)
            fl |= D3DCLEAR_TARGET;
        if (fl) {
            mr.x1 = d3_Viewport.left;
            mr.y1 = d3_Viewport.top;
            mr.x2 = d3_Viewport.right;
            mr.y2 = d3_Viewport.bottom;
#line 325
            D3DCHECK(d3d_Viewport->lpVtbl->Clear2(d3d_Viewport, 1, &mr, fl, d3_fog_color.col, 1.0f, 0));
        }
        dd_BackCleared = 1;
        dd_ZCleared = 1;
    }
}
