/* 0x5324f0, registers the window class "Die Gilde" (the window procedure 0x531780, the icon 0x65, the arrow cursor, a black brush) and creates and shows the game window (title "<version text>", full screen
   topmost popup for the mode 3, else a captioned window sized to the client area 0x62d7d0 x 0x62d7d4 and centred); returns 1 on success */

int main_Unk5324f0(HINSTANCE inst, int unused, int mode)
{
    WNDCLASSA wc;
    RECT rc, work;
    char title[0x80];
    DWORD style, exs, flags;
    int w, h, nh, nw, ax, ay;
    HWND hw;

    memset(&wc, 0, sizeof(wc));
    exs = 0;
    wc.style = 0;
    wc.lpfnWndProc = main_Unk531780;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = inst;
    wc.hIcon = LoadIconA(inst, (LPCSTR)0x65);
    wc.hCursor = LoadCursorA(0, (LPCSTR)0x7f00);
    wc.hbrBackground = (HBRUSH)GetStockObject(4);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "Die Gilde";
    if (RegisterClassA(&wc) == 0)
        return 0;
    main_Unk531a00((char *)((unsigned)&ls_SavePath + 0x120));
    crt_Sprintf(title, (char *)(unsigned)&full_data_60da10, (char *)((unsigned)&ls_SavePath + 0x120));
    SystemParametersInfoA(0x30, 0, &work, 0);
    w = work.right - work.left;
    h = work.bottom - work.top;
    if (mode == 3) {
        style = 0x92000000;
        exs = 8;
        work.top = 0;
        work.left = 0;
        w = work.right;
        h = work.bottom;
    } else
        style = 0x6000000;
    hw = CreateWindowExA(exs, "Die Gilde", title, style, work.left, work.top, w, h, 0, 0, inst, 0);
    U_I((unsigned)&main_Window) = (int)hw;
    if (hw == 0)
        return 0;
    ShowWindow(hw, 5);
    UpdateWindow(hw);
    SetFocus(hw);
    if (mode == 3) {
        flags = 0;
        w = GetSystemMetrics(1);
        h = GetSystemMetrics(0);
        hw = (HWND)-1;
        SetWindowPos((HWND)U_I((unsigned)&main_Window), hw, 0, 0, h, w, flags);
    } else {
        GetClientRect((HWND)U_I((unsigned)&main_Window), &rc);
        nh = U_I((unsigned)&ai_ScreenModeH) - rc.bottom + h;
        ay = h - nh;
        nw = U_I((unsigned)&ai_ScreenModeW) - rc.right + w;
        ax = w - nw;
        if (ay < 0)
            ay = -ay;
        if (ax < 0)
            ax = -ax;
        MoveWindow((HWND)U_I((unsigned)&main_Window), ax / 2, ay / 2, nw, nh, 1);
        SetWindowPos((HWND)U_I((unsigned)&main_Window), 0, 0, 0, 0, 0, 3);
    }
    return 1;
}
