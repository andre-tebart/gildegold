/* 0x41c460, "d3d_EnumDeviceCallback": collects up to 8 Direct3D devices that can render at ctx bits @deep B2/B3: the remaining code hunks are the same instructions with exchanged scratch registers and stack slots (instr 110 test DWORD PTR [ecx+0x9c],eax vs [eax+0x9c],ecx; instr 140 the lea *3/*2/*8 chain with eax/ecx/edx exchanged); the slot-20/24/28 allocation and every recorded attempt (notes/units/dd_vesa.md: scope experiments, declaration order, D3_COPY mixes, c2prio) leave it at 279 B - VC6 allocation, not a meaning error */

HRESULT WINAPI d3d_EnumDeviceCallback(GUID *guid, LPSTR desc, LPSTR name,
                                      LPD3DDEVICEDESC hw, LPD3DDEVICEDESC hel, LPVOID ctx)
{
    D3DCOLORMODEL model = hw->dcmColorModel;
    DWORD depth = dd_BppToDDBD((int)ctx);

    if (model) {
        if (!(hw->dwDeviceRenderBitDepth & depth))
            return D3DENUMRET_OK;
        if (model == D3DCOLOR_MONO)
            return D3DENUMRET_OK;
        if (!(hw->dwFlags & D3DDD_TRICAPS))
            return D3DENUMRET_OK;
        D3_COPY(&d3d_Devices[d3d_NumDevices].guid, guid, sizeof(GUID));
        strcpy(d3d_Devices[d3d_NumDevices].desc, desc);
        strcpy(d3d_Devices[d3d_NumDevices].name, name);
        D3_COPY(&d3d_Devices[d3d_NumDevices].caps, hw, sizeof(D3DDEVICEDESC));
        d3d_Devices[d3d_NumDevices].software = 0;
        d3d_GetDeviceInfo(&d3d_Devices[d3d_NumDevices].caps, &d3d_Devices[d3d_NumDevices]);
        d3d_NumDevices++;
    } else {
        if (!(hel->dwDeviceRenderBitDepth & depth))
            return D3DENUMRET_OK;
        if (hel->dcmColorModel == D3DCOLOR_MONO)
            return D3DENUMRET_OK;
        if (!(hel->dwFlags & D3DDD_TRICAPS))
            return D3DENUMRET_OK;
        D3_COPY(&d3d_Devices[d3d_NumDevices].guid, guid, sizeof(GUID));
        strcpy(d3d_Devices[d3d_NumDevices].desc, desc);
        strcpy(d3d_Devices[d3d_NumDevices].name, name);
        D3_COPY(&d3d_Devices[d3d_NumDevices].caps, hel, sizeof(D3DDEVICEDESC));
        d3d_Devices[d3d_NumDevices].software = 1;
        d3d_GetDeviceInfo(&d3d_Devices[d3d_NumDevices].caps, &d3d_Devices[d3d_NumDevices]);
        d3d_NumDevices++;
    }
    return d3d_NumDevices != 8;
}
