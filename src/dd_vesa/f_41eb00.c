/* 0x41eb00, tags a surface with a name for debugging */

char dd_SetSurfaceName(LPDIRECTDRAWSURFACE4 surf, char *data, char unk)
{
    GUID userdata = {0xf0eb1210, 0xaa9c, 0x4919, {0x9c, 0x15, 0x65, 0x99, 0x04, 0xd9, 0x8f, 0x51}};

    if (!surf)
        return 0;
    userdata.Data4[7] += unk;
#line 2609
    D3DCHECK(surf->lpVtbl->SetPrivateData(surf, &userdata, (void *)data, strlen(data) + 1, 0));
    return d3d_hr == 0;
}
