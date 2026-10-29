/* 0x410770 */

int di_Init(HWND hwnd, HINSTANCE hinst, unsigned int flags)
{
    HRESULT hr;
    DIPROPDWORD dp;

    di_Hinst = hinst;
    di_Flags = flags;
    di_Hwnd = hwnd;
    hr = DirectInputCreate(hinst, 0x700, &di_Main, NULL);
    if (hr < 0) {
        if (hr == 0x80070481) {
            di_Error(0, hr, "DirectInputCreate-BetadiVersion");
            return 0;
        }
        if (hr == 0x80070057) {
            di_Error(0, hr, "DirectInputCreate-Invalidparam");
            return 0;
        }
        if (hr == 0x8007047e) {
            di_Error(0, hr, "DirectInputCreate-OldVersion");
            return 0;
        }
        if (hr == 0x8007000e)
            di_Error(0, hr, "DirectInputCreate-OutOfMemory");
        return 0;
    }
    hr = di_Main->lpVtbl->CreateDevice(di_Main, &GUID_SysMouse, &di_Mouse, NULL);
    if (hr < 0) {
        di_Error(0, hr, "CreateDevice(SysMouse)");
        return 0;
    }
    hr = di_Mouse->lpVtbl->SetDataFormat(di_Mouse, &c_dfDIMouse);
    if (hr < 0) {
        di_Error(0, hr, "SetDataFormat(SysMouse, dfDIMouse)");
        return 0;
    }
    if (flags & 2)
        hr = di_Mouse->lpVtbl->SetCooperativeLevel(di_Mouse, hwnd, 0xa);
    else
        hr = di_Mouse->lpVtbl->SetCooperativeLevel(di_Mouse, hwnd, 5);
    if (hr != 0) {
        if (hr == 0x80070057)
            di_Error(0, hr, "SetCooperativeLevel(SysMouse):InvalidParam");
        else if (hr == 0x80070015)
            di_Error(0, hr, "SetCooperativeLevel(SysMouse):DIERR_NOTINITIALIZED ");
        else
            di_Error(0, hr, "SetCooperativeLevel(SysMouse):Unknown-error");
    }
    hr = di_Mouse->lpVtbl->SetEventNotification(di_Mouse, NULL);
    if (hr != 0) {
        di_Error(0, hr, "SetEventNotification(SysMouse)");
        return 0;
    }
    dp.diph.dwSize = sizeof(DIPROPDWORD);
    dp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dp.diph.dwObj = 0;
    dp.diph.dwHow = DIPH_DEVICE;
    dp.dwData = 0x80;
    hr = di_Mouse->lpVtbl->SetProperty(di_Mouse, DIPROP_BUFFERSIZE, &dp.diph);
    if (hr < 0) {
        di_Error(0, hr, "Set buffer size(SysMouse)");
        return 0;
    }
    di_AcquireMouse(1);
    dm_S.x = 200;
    dm_S.y = 200;
    dm_S.z = 0;
    hr = di_Main->lpVtbl->CreateDevice(di_Main, &GUID_SysKeyboard, &di_Keyboard, NULL);
    if (hr < 0) {
        di_Error(0, hr, "CreateDevice(GUID_SysKeyboard)");
        return 0;
    }
    hr = di_Keyboard->lpVtbl->SetDataFormat(di_Keyboard, &c_dfDIKeyboard);
    if (hr < 0) {
        di_Error(0, hr, "SetDataFormat(&c_dfDIKeyboard)");
        return 0;
    }
    if (flags & 1)
        hr = di_Keyboard->lpVtbl->SetCooperativeLevel(di_Keyboard, hwnd, 0x15);
    else
        hr = di_Keyboard->lpVtbl->SetCooperativeLevel(di_Keyboard, hwnd, 0xa);
    if (hr != 0)
        return 0;
    dp.diph.dwSize = sizeof(DIPROPDWORD);
    dp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dp.diph.dwObj = 0;
    dp.diph.dwHow = DIPH_DEVICE;
    dp.dwData = 0x80;
    hr = di_Keyboard->lpVtbl->SetProperty(di_Keyboard, DIPROP_BUFFERSIZE, &dp.diph);
    if (hr < 0)
        return 0;
    di_AcquireKeyboard(1);
    kb_BuildMap();
    return 1;
}
