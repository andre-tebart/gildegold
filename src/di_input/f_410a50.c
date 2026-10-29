/* 0x410a50, unacquire and release the keyboard, mouse and DirectInput objects, close di_Event, clear di_Hwnd */

void di_Exit(void)
{
    di_Hwnd = 0;
    di_AcquireKeyboard(0);
    di_AcquireMouse(0);
    if (di_Keyboard) {
        di_Keyboard->lpVtbl->Release(di_Keyboard);
        di_Mouse = 0;
    } else if (di_Mouse) {
        di_Mouse->lpVtbl->Release(di_Mouse);
        di_Mouse = 0;
    }
    if (di_Main) {
        di_Main->lpVtbl->Release(di_Main);
        di_Main = 0;
    }
    if (di_Event) {
        CloseHandle(di_Event);
        di_Event = 0;
    }
}
