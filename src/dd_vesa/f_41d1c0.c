/* 0x41d1c0, "dd_EnumVideoModesCallback": notes which of the larger resolutions exist at ctx bits */

HRESULT WINAPI dd_EnumVideoModesCallback(LPDDSURFACEDESC2 ddsd, LPVOID ctx)
{
    if (ddsd->ddpfPixelFormat.dwFlags & DDPF_RGB) {
        if (ddsd->ddpfPixelFormat.dwRGBBitCount == (DWORD)ctx) {
            if (ddsd->dwWidth == 800 && ddsd->dwHeight == 600)
                dd_Has800x600 = 1;
            if (ddsd->dwWidth == 1024 && ddsd->dwHeight == 768)
                dd_Has1024x768 = 1;
            if (ddsd->dwWidth == 1152 && ddsd->dwHeight == 864)
                dd_Has1152x864 = 1;
        }
    }
    return DDENUMRET_OK;
}
