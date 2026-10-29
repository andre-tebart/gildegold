/* 0x422c90, window procedure of a movie window: closes the movie on WM_DESTROY, stops and closes it on
   space/escape, and closes it when its graph reports EC_COMPLETE */
LRESULT CALLBACK mov_WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    MOVIE *m;
    long p1, p2;

    switch (msg) {
    case WM_DESTROY:
        m = mov_FindWnd(hwnd);
        if (m)
            mov_Close(m->id);
        break;
    case WM_KEYDOWN:
        if (wp == VK_SPACE || wp == VK_ESCAPE) {
            m = mov_FindWnd(hwnd);
            if (m) {
                m->control->lpVtbl->Stop(m->control);
                mov_Close(m->id);
                DestroyWindow(hwnd);
            }
        }
        break;
    case WM_GRAPHNOTIFY:
        if (mov_GetEvent(lp, &p1, &p2) == EC_COMPLETE) {
            mov_Close(lp);
            DestroyWindow(hwnd);
        }
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}
