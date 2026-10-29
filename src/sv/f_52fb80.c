/* 0x52fb80, pumps the window messages of 0x6f2588 (the message struct 0x75ec40); WM_QUIT sets 0x6f25b4; returns 1 */

int sv_PumpMessages(void)
{
    int r;

    r = PeekMessageA((MSG *)(unsigned)&full_data_75ec40, (HWND)SV_I((unsigned)&main_Window), 0, 0, 0);
    while (r != 0) {
        if (GetMessageA((MSG *)(unsigned)&full_data_75ec40, (HWND)SV_I((unsigned)&main_Window), 0, 0) == 0)
            SV_I((unsigned)&full_data_6f25b4) = 1;
        else {
            TranslateMessage((MSG *)(unsigned)&full_data_75ec40);
            DispatchMessageA((MSG *)(unsigned)&full_data_75ec40);
        }
        r = PeekMessageA((MSG *)(unsigned)&full_data_75ec40, (HWND)SV_I((unsigned)&main_Window), 0, 0, 0);
    }
    return 1;
}
