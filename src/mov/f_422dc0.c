/* 0x422dc0 @deep 460 code hunks: the whole register map is shifted (the MOVIE pointer is ebp in the original and ebx in ours, the zero constant esi/ebx) and our frame is 4 bytes larger; `int id = 0` is a real stack store in ours and absent in the original. Every hunk read is the same operation with ebx/ebp/esi exchanged (argument copies, the five QueryInterface calls, the window setup); no missing or extra statement found. A 1472-byte register/frame job, not a meaning fix. */

int mov_Open(char *file, HWND hwnd, HINSTANCE hinst, int x, int y, int w, int h, unsigned int flags)
{
    RECT r;
    long vw, vh;
    RECT cr;
    char buf[256];
    WCHAR wfile[MAX_PATH];
    FILE *f;
    MOVIE *m;
    int id = 0;

    f = fopen(file, "rb");
    if (f) {
    fclose(f);
    if (hwnd == NULL) {
        if (mov_Class == 0) {
            WNDCLASSEX wc = {sizeof(WNDCLASSEX)};
            wc.hbrBackground = NULL;
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.lpfnWndProc = mov_WndProc;
            wc.lpszClassName = "MOVIEWNDCLASS";
            wc.hInstance = hinst;
            mov_Class = RegisterClassEx(&wc);
        }
        if (flags & MOV_FULL) {
            hwnd = CreateWindowEx(WS_EX_TOPMOST, "MOVIEWNDCLASS", "", WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                  0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hinst, NULL);
            ShowCursor(FALSE);
        } else
            hwnd = CreateWindowEx(WS_EX_TOPMOST, "MOVIEWNDCLASS", "", WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinst, NULL);
        flags |= MOV_OWNWND;
    }
    if (IsWindow(hwnd)) {
    m = mov_Alloc();
    if (m) {
    strcpy(m->file, file);
    m->hwnd = hwnd;
    m->flags = flags;
    if (FAILED(CoCreateInstance(&CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, &IID_IGraphBuilder, (void **)&m->graph)))
        goto fail;
    if (FAILED(m->graph->lpVtbl->QueryInterface(m->graph, &IID_IBasicVideo, (void **)&m->video)))
        goto fail;
    if (FAILED(m->graph->lpVtbl->QueryInterface(m->graph, &IID_IBasicAudio, (void **)&m->audio)))
        goto fail;
    if (FAILED(m->graph->lpVtbl->QueryInterface(m->graph, &IID_IMediaControl, (void **)&m->control)))
        goto fail;
    if (FAILED(m->graph->lpVtbl->QueryInterface(m->graph, &IID_IMediaEventEx, (void **)&m->event)))
        goto fail;
    MultiByteToWideChar(CP_ACP, MB_COMPOSITE, file, strlen(file) + 1, wfile, MAX_PATH);
    if (FAILED(m->graph->lpVtbl->RenderFile(m->graph, wfile, NULL)))
        goto fail;
    if (FAILED(m->graph->lpVtbl->QueryInterface(m->graph, &IID_IVideoWindow, (void **)&m->window)))
        goto fail;
    if (FAILED(m->window->lpVtbl->put_Owner(m->window, (OAHWND)m->hwnd)))
        goto fail;
    if (FAILED(m->window->lpVtbl->put_WindowStyle(m->window, WS_CHILD)))
        goto fail;
    if (flags & MOV_FILL) {
        GetClientRect(hwnd, &r);
        if (flags & MOV_ASPECT) {
            if (FAILED(m->video->lpVtbl->GetVideoSize(m->video, &vw, &vh)))
                goto fail;
            h = (int)((float)((r.right - r.left) * vh) / vw);
            r.top = (r.bottom - r.top >> 1) - (h >> 1);
            r.bottom = r.top + h;
        }
    } else if (w && h) {
        if (!(flags & MOV_ASPECT)) {
            r.left = x;
            r.top = y;
            r.right = x + w;
            r.bottom = y + h;
            if (flags & MOV_CENTER) {
                GetClientRect(hwnd, &cr);
                r.left = (cr.right - cr.left >> 1) - (r.right - r.left >> 1);
                r.top = (cr.bottom - cr.top >> 1) - (r.bottom - r.top >> 1);
            }
        } else {
            if (FAILED(m->video->lpVtbl->GetVideoSize(m->video, &vw, &vh)))
                goto fail;
            h = (int)((float)(vh * w) / vw);
            if (flags & MOV_CENTER) {
                GetClientRect(hwnd, &cr);
                r.left = (cr.right - cr.left >> 1) - (w >> 1);
                r.top = (cr.bottom - cr.top >> 1) - (h >> 1);
                r.right = r.left + w;
                r.bottom = r.top + h;
            } else {
                r.left = x;
                r.top = y;
                r.right = x + w;
                r.bottom = y + h;
            }
        }
    } else {
        GetClientRect(hwnd, &r);
        if (FAILED(m->video->lpVtbl->GetVideoSize(m->video, &vw, &vh)))
            goto fail;
        r.left = (r.right - r.left >> 1) - (vw >> 1);
        r.top = (r.bottom - r.top >> 1) - (vh >> 1);
        r.right = r.left + vw;
        r.bottom = r.top + vh;
    }
    if (FAILED(m->window->lpVtbl->SetWindowPosition(m->window, r.left, r.top, r.right - r.left, r.bottom - r.top)))
        goto fail;
    m->id = mov_NextId;
    id = mov_NextId++;
    if (flags & MOV_NOTIFY)
        m->event->lpVtbl->SetNotifyWindow(m->event, (OAHWND)hwnd, WM_GRAPHNOTIFY, id);
    m->window->lpVtbl->SetWindowForeground(m->window, -1);
    m->window->lpVtbl->put_BackgroundPalette(m->window, 0);
    }
    } else
        sprintf(buf, "%s: invalid window %x", file, hwnd);
    }
    return id;
fail:
    mov_Release(m);
    mov_Free(m);
    return 0;
}
