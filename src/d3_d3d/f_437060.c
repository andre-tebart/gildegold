/* 0x437060, switches between z buffer (D3DZB_TRUE) and w buffer (D3DZB_USEW). */

void d3d_SetWBuffer(char w)
{
    if (d3d_Device && d3d_ZBuffer && (d3d_UsedCard.info.flags & 0x10) && w != d3d_ZEnabled) {
        d3d_UsedCard.info.zmode = (w != 0) + 1;
#line 699
        D3DCHECK(d3d_Device->lpVtbl->SetRenderState(d3d_Device, D3DRENDERSTATE_ZENABLE, d3d_UsedCard.info.zmode));
        d3d_ZEnabled = w;
    }
}
