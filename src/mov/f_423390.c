/* 0x423390, shows the movie window and runs its graph; for an own window it pumps the window's messages
   until the window is destroyed and returns 1, otherwise it returns whether Run succeeded */

int mov_Play(int id)
{
    MSG msg;
    MOVIE *m;
    HRESULT hr;

    m = mov_Find(id);
    if (m) {
    ShowWindow(m->hwnd, SW_SHOW);
    BringWindowToTop(m->hwnd);
    SetWindowPos(m->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    UpdateWindow(m->hwnd);
    hr = m->control->lpVtbl->Run(m->control);
    if (m->flags & MOV_OWNWND) {
        for (;;) {
            if (PeekMessage(&msg, m->hwnd, 0, 0, PM_NOREMOVE)) {
                if (!GetMessage(&msg, m->hwnd, 0, 0))
                    break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                if (msg.message == WM_DESTROY)
                    break;
            } else {
                if (!IsWindow(m->hwnd))
                    break;
                Sleep(100);
            }
        }
        return 1;
    }
    return SUCCEEDED(hr);
    }
    return 0;
}
