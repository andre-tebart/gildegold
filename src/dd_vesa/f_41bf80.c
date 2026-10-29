/* 0x41bf80, DirectDrawEnumerateEx callback: collects up to 8 drivers in dd_Devices */

BOOL WINAPI dd_EnumDriverCallback(GUID *guid, LPSTR desc, LPSTR name, LPVOID ctx, HMONITOR mon)
{
    strcpy(dd_Devices[dd_NumDevices].desc, desc);
    strcpy(dd_Devices[dd_NumDevices].name, name);
    if (!guid) {
        dd_Devices[dd_NumDevices].guid = NULL;
        D3_ZERO(&dd_Devices[dd_NumDevices].guiddata, sizeof(GUID));
    } else {
        D3_COPY(&dd_Devices[dd_NumDevices].guiddata, guid, sizeof(GUID));
        dd_Devices[dd_NumDevices].guid = &dd_Devices[dd_NumDevices].guiddata;
    }
    dd_NumDevices++;
    return dd_NumDevices != 8;
}
