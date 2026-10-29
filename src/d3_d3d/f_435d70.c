/* 0x435d70, gets IDirect3D3 and picks the device: the one saved in the registry, else asks. */

char d3d_Init(int bitplanes, int a, int b)
{
    char guidstr[256];
    GUID guid;
    int h;
    int sel;
    char ask;
    char again;
    int i;

    if (dd_DD) {
#line 228
        D3DCHECK(dd_DD->lpVtbl->QueryInterface(dd_DD, &IID_IDirect3D3, &d3d_D3D));
        if (!d3d_hr) {
            d3d_NumCards = 0;
#line 233
            D3DCHECK(d3d_D3D->lpVtbl->EnumDevices(d3d_D3D, d3d_EnumDeviceCallback, (void *)bitplanes));
            if (!d3d_hr && d3d_NumCards) {
                for (sel = 0; sel < d3d_NumCards; sel++)
                    if (!(d3d_Cards[sel].info.flags & 4))
                        break;
                if (sel >= d3d_NumCards)
                    sel = 0;
                if (d3d_NumCards > 1) {
                    ask = 1;
                    again = 0;
                    for (;;) {
                        if (again)
                            ask = 0;
                        h = reg_OpenKey("dd_vesa", 1);
                        if (h != -1) {
                            if (reg_GetString(h, "d3d_Guid", guidstr) && dd_StringToGuid(guidstr, &guid)) {
                                for (i = 0; i < d3d_NumCards; i++) {
                                    if (!memcmp(&guid, &d3d_Cards[i].Guid, sizeof(GUID))) {
                                        ask = 0;
                                        sel = i;
                                    }
                                }
                            }
                            reg_CloseKey(h);
                        }
                        if (!ask)
                            break;
                        dd_DeviceDialog(bitplanes, b, a, 1, d3d_D3D);
                        again = 1;
                    }
                }
                D3_COPY_TO(offset d3d_UsedCard, &d3d_Cards[sel], sizeof(D3DCARD));
                d3d_Mipmaps = d3d_UsedCard.info.maxMipFilter > 1;
                d3d_ZBufferCaps = (d3d_UsedCard.info.flags & 4) ? 0x800 : 0x6000;
                return 1;
            }
        }
    }
    return 0;
}
