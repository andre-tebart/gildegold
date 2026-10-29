/* 0x4385a0, d3.h: d3_BeginScene */

void d3_BeginScene(unsigned char fog, DWORD fogcol, unsigned char zbuffer)
{
#line 1227
    D3DCHECK(d3d_Device->lpVtbl->BeginScene(d3d_Device));
    D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, NULL));
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHAREF, 0xbf));
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER));
    if (fog) {
#line 1233
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGCOLOR, fogcol));
    }
#line 1234
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_FOGENABLE, fog));
#line 1236
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZENABLE, zbuffer ? d3d_UsedCard.info.zmode : D3DZB_FALSE));
    D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZWRITEENABLE, zbuffer && !d3d_balpha));
    d3d_zbuffer = zbuffer;
    d3d_fog = fog;
}
