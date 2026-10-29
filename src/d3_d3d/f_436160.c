/* 0x436160, creates the z buffer, the device and the viewport. */

char d3d_CreateDevice(DWORD texbpp, DWORD fourcc, DWORD texbpp2, DWORD zbpp)
{
    DDSURFACEDESC2 ddsd;
    int i;

    d3d_ZEnabled = 0;
    d3d_MinMipSize = 4;
    d3d_TexShift = 0;
    d3d_balpha = 0;
    d3d_glow = 0;
    d3d_colorkey = 0;
    d3d_alphatest = 0;
    d3d_fog = 0;
    d3d_zbuffer = 1;
    d3d_Near = d3d_Far = d3d_ViewW = d3d_ViewH = d3d_ViewX = d3d_ViewY = -1.0f;
    if (d3d_UsedCard.info.caps & 0x20) {
        d3d_UsedZBufferFormat.bpp = zbpp;
#line 397
        D3DCHECK(d3d_D3D->lpVtbl->EnumZBufferFormats(d3d_D3D, &d3d_UsedCard.Guid, d3d_EnumZBufferFormatsCallback, (void *) &d3d_UsedZBufferFormat));
        D3_ZERO(&ddsd, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.ddsCaps.dwCaps = d3d_ZBufferCaps | DDSCAPS_ZBUFFER;
        ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
        ddsd.dwWidth = dd_Width;
        ddsd.dwHeight = dd_Height;
        D3_COPY(&ddsd.ddpfPixelFormat, offset d3d_UsedZBufferFormat.pf, sizeof(DDPIXELFORMAT));
#line 405
        D3DCHECK(dd_DD->lpVtbl->CreateSurface(dd_DD, &ddsd, &d3d_ZBuffer, NULL));
        if (d3d_hr)
            return 0;
        dd_SetSurfaceName(d3d_ZBuffer, "d3d_ZBuffer", 0);
#line 409
        D3DCHECK(dd_BackBuffer->lpVtbl->AddAttachedSurface(dd_BackBuffer, d3d_ZBuffer));
        if (d3d_hr)
            return 0;
    }
#line 413
    D3DCHECK(d3d_D3D->lpVtbl->CreateDevice(d3d_D3D, &d3d_UsedCard.Guid, dd_BackBuffer, &d3d_Device, NULL));
    if (d3d_hr)
        return 0;
#line 417
    D3DCHECK(d3d_D3D->lpVtbl->CreateViewport(d3d_D3D, &d3d_Viewport, NULL));
    if (d3d_hr)
        return 0;
#line 420
    D3DCHECK(d3d_Device->lpVtbl->AddViewport(d3d_Device, d3d_Viewport));
    if (d3d_hr)
        return 0;
#line 423
    D3DCHECK(d3d_Device->lpVtbl->SetCurrentViewport(d3d_Device, d3d_Viewport));
    if (d3d_hr)
        return 0;
    for (i = 0; i < 3; i++) {
        d3d_PrecacheBuffer[i].sx = (float)i;
        d3d_PrecacheBuffer[i].sy = (float)i;
        d3d_PrecacheBuffer[i].sz = 0.0f;
        d3d_PrecacheBuffer[i].rhw = 1.0f;
        d3d_PrecacheBuffer[i].color = 0xffffffff;
        d3d_PrecacheBuffer[i].tu = (float)i;
        d3d_PrecacheBuffer[i].tv = (float)i;
    }
    d3d_VertexBuffer = mem_Alloc(0x10000, "d3d:VertexBuffer");
    d3d_StatesSet = 0;
    d3d_FindTextureFormats(texbpp, fourcc, texbpp2);
    return 1;
}
