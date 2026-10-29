/* 0x41cf60, driver/device selection dialog (resource 101 in d3_resource.dll) */

void dd_DeviceDialog(int bpp, int unused, HWND parent, char flag, LPDIRECT3D3 d3d3)
{
    HINSTANCE lib = LoadLibrary("d3_resource.dll");

    dd_DlgFlag = flag;
    dd_D3D3 = d3d3;
    dd_BitPlanes = bpp;
    if (lib) {
        DialogBoxParam(lib, MAKEINTRESOURCE(101), parent, dd_DlgProc, 0);
        FreeLibrary(lib);
    }
}
