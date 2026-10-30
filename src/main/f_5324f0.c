/* 0x5324f0, registers the window class "Die Gilde" (the window procedure main_WindowProc, the icon 0x65, the arrow cursor, a black brush) and creates and shows the game window (title " <version text>"
   built by main_BuildVersionString into the program's version buffer at ls_SavePath + 0x120; a full screen topmost popup for the mode 3, else a captioned window that is resized so its client area is
   ai_ScreenModeW x ai_ScreenModeH and centred on the work area); returns 1 on success @deep body complete and readable (class registration, the ' %s' title from main_BuildVersionString's buffer at ls_SavePath + 0x120, work area / mode 3 popup, client-area resize + centre with abs(..)/2); 178 vs 175 ins. Left (asmdiff 181 ins, all allocation): frame 0xcc vs 0xc8 and wc at [esp+0x20] vs [esp+0x1c]: ours spills exs (the extended style) to a 4-byte stack slot ([esp+0x10]) because it holds work.right in ebx and work.bottom in ebp and the zero in esi; the original keeps exs in ebx (xor ebx,ebx doubles as the zero constant for every push 0 / mov [wc.x],ebx), only work.right in ebp (mode 3 reloads work.bottom with mov edi,[esp+0x1c]) and w / h in esi / edi. Tried: all 8 declaration orders of wc / rc / work / title (no effect, VC6 sorts by size); the source reads exactly as ghidra/the asm do. Needs the variable-lifetime variant (e.g. which of w / h / exs are assigned before the branch) */

int main_CreateWindow(HINSTANCE inst, int unused, int mode)
{
    WNDCLASSA wc;
    RECT rc, work;
    char title[0x80];
    DWORD style, exs;
    int w, h, nw, nh;

    memset(&wc, 0, sizeof(wc));
    exs = 0;
    wc.style = 0;
    wc.lpfnWndProc = main_WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = inst;
    wc.hIcon = LoadIconA(inst, (LPCSTR)0x65);
    wc.hCursor = LoadCursorA(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "Die Gilde";
    if (RegisterClassA(&wc) == 0)
        return 0;
    main_BuildVersionString(ls_SavePath.version);
    sprintf(title, " %s", ls_SavePath.version);
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);
    w = work.right - work.left;
    h = work.bottom - work.top;
    if (mode == 3) {
        style = WS_POPUP | WS_VISIBLE | 0x12000000;
        exs = WS_EX_TOPMOST;
        work.top = 0;
        work.left = 0;
        w = work.right;
        h = work.bottom;
    } else
        style = 0x6000000;
    main_Window = CreateWindowExA(exs, "Die Gilde", title, style, work.left, work.top, w, h, 0, 0, inst, 0);
    if (main_Window == 0)
        return 0;
    ShowWindow(main_Window, SW_SHOW);
    UpdateWindow(main_Window);
    SetFocus(main_Window);
    if (mode == 3)
        SetWindowPos(main_Window, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0);
    else {
        GetClientRect(main_Window, &rc);
        nh = ai_ScreenModeH - rc.bottom + h;
        nw = ai_ScreenModeW - rc.right + w;
        MoveWindow(main_Window, abs(w - nw) / 2, abs(h - nh) / 2, nw, nh, 1);
        SetWindowPos(main_Window, 0, 0, 0, 0, 0, 3);
    }
    return 1;
}
