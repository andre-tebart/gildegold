/* 0x41cfb0, creates dd_DDBase/dd_DD on the driver stored as "dd_Guid" (asks with the dialog if unset) */

char dd_Init(int mode, int bpp, HWND parent, int unk)
{
    GUID *guid;
    HKEY cfg;
    GUID g;
    char buf[0x100];
    char ask, shown;

    dd_NumDevices = 0;
    if (DirectDrawEnumerateEx(dd_EnumDriverCallback, NULL, mode == DD_FULLSCREEN || mode == DD_FULLMEM ? 7 : 0) != DD_OK
        || !dd_NumDevices)
        return 0;
    guid = NULL;
    if (dd_NumDevices > 1) {
        ask = 1;
        shown = 0;
        while (1) {
            if (shown)
                ask = 0;
            cfg = reg_OpenKey("dd_vesa", 1);
            if (cfg != -1) {
                if (reg_GetString(cfg, "dd_Guid", buf)) {
                    if (!strcmp("00000000-0000-0000-0000000000000000", buf)) {
                        guid = NULL;
                        ask = 0;
                    } else if (dd_StringToGuid(buf, &g)) {
                        int i;
                        for (i = 0; i < dd_NumDevices; i++)
                            if (!memcmp(&g, &dd_Devices[i].guiddata, sizeof(GUID))) {
                                guid = dd_Devices[i].guid;
                                ask = 0;
                            }
                    }
                }
                reg_CloseKey(cfg);
            }
            if (!ask)
                break;
            dd_DeviceDialog(bpp, unk, parent, 0, 0);
            shown = 1;
        }
    } else
        guid = dd_Devices[0].guid;
    if (DirectDrawCreate(guid, &dd_DDBase, NULL) != DD_OK) {
        err_Message("Unable to create DirectDraw-Object");
        return 0;
    }
#line 1073
    D3DCHECK(dd_DDBase->lpVtbl->QueryInterface(dd_DDBase, &IID_IDirectDraw4, &dd_DD));
    if (d3d_hr == 0)
        return 1;
    dd_Ref = dd_DDBase->lpVtbl->Release(dd_DDBase);
    dd_DDBase = NULL;
    return 0;
}
