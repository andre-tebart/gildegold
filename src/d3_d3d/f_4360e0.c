/* 0x4360e0, draws one invisible triangle with the texture so that the driver loads it. */

void d3d_PrecacheTexture(LPDIRECT3DTEXTURE2 t)
{
    if (t) {
#line 359
        D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, t));
        D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3D_VERTEX_STRUCT, (void*)d3d_PrecacheBuffer, 3, 0));
    }
}
