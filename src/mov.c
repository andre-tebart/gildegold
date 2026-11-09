/* mov.c -- original file name unknown ("MOVIEWNDCLASS").
   Movie playback through DirectShow: a filter graph renders the file into an own or a given window.
   Extent in GildeGold.exe (inferred, not proven): 0x422c90..0x423770.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/mov.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <objbase.h>
#include <strmif.h>
#include <control.h>
#include <evcode.h>
#include "mov.h"

extern char ch_EmptyName[];   /* 0x631090, the shared empty string "" (ch.c) */

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
/* 0x422d60, initializes COM for the movie module, resets the id counter and the window class; returns 1 */

int mov_Init(void)
{
    CoInitialize(NULL);
    mov_NextId = 666;
    mov_Class = 0;
    return 1;
}
/* 0x422d90, releases and frees every movie of the list, then uninitializes COM */

void mov_Exit(void)
{
    while (mov_List) {
        mov_Release(mov_List);
        mov_Free(mov_List);
    }
    CoUninitialize();
}
/* 0x422dc0
   @stuck the zero constant and id: in the original id = 0 and every literal 0 (fopen/hwnd/m tests, the
   NULL pushes) share esi (return paths mov eax, esi; later id = mov_NextId++ lands in esi), file is ebx, m
   ebp, hwnd edi; ours keeps the zero in ebx/ebp, spills id to [esp+0x28] (the extra slot pushes wc and the
   later slots 4 down) and swaps file/m (asmdiff --regs ebp=ebx,ebx=ebp leaves the zero/id hunks, the wc
   slot shift and a store schedule around strcpy). headers sample: insensitive (32, one codegen). dcp try:
   int id; id = 0; first, long id, the id declaration at every position, id = m->id = mov_NextId++,
   separate id = mov_NextId; mov_NextId++: = control; id = 0 after fopen: 1244 B; early return 0 for
   fopen/IsWindow/mov_Alloc with return id at the end: 1232-1257 B (no zero register at all then); id = 0
   at each of 60 statement positions (changes meaning on the early paths): best 999 B, none byte. Fixed
   here: CLSID_FilgraphManager (the original's strmiids symbol, M8) and ch_EmptyName for the window title
   (M9). (tiebreak: not measured (dcp headers sample mov_Open)) prior: 460 code hunks: the whole register
   map is shifted (the MOVIE pointer is ebp in the original and ebx in ours, the zero constant esi/ebx) and
   our frame is 4 bytes larger; `int id = 0` is a real stack store in ours and absent in the original.
   Every hunk read is the same operation with ebx/ebp/esi exchanged (argument copies, the five
   QueryInterface calls, the window setup); no missing or extra statement found. A 1472-byte register/frame
   job, not a meaning fix. */

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
            hwnd = CreateWindowEx(WS_EX_TOPMOST, "MOVIEWNDCLASS", ch_EmptyName, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                  0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hinst, NULL);
            ShowCursor(FALSE);
        } else
            hwnd = CreateWindowEx(WS_EX_TOPMOST, "MOVIEWNDCLASS", ch_EmptyName, WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hinst, NULL);
        flags |= MOV_OWNWND;
    }
    if (IsWindow(hwnd)) {
    m = mov_Alloc();
    if (m) {
    strcpy(m->file, file);
    m->hwnd = hwnd;
    m->flags = flags;
    if (FAILED(CoCreateInstance(&CLSID_FilgraphManager, NULL, CLSCTX_INPROC_SERVER, &IID_IGraphBuilder, (void **)&m->graph)))
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
        sprintf(buf, "hwnd ung\xfc" "ltig !\nfilename: %s\nHWND:%d", file, hwnd);
    }
    return id;
fail:
    mov_Release(m);
    mov_Free(m);
    return 0;
}
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
/* 0x4234a0, returns the next event of the movie's graph with its two parameters, stopping after EC_COMPLETE
   or 0x0e; 0 when no movie has that id or no event is left */

long mov_GetEvent(int id, long *p1, long *p2)
{
    long ev;
    MOVIE *m;

    m = mov_Find(id);
    ev = 0;
    if (m) {
        while (SUCCEEDED(m->event->lpVtbl->GetEvent(m->event, &ev, p1, p2, 0))) {
            m->event->lpVtbl->FreeEventParams(m->event, ev, *p1, *p2);
            if (ev == EC_COMPLETE || ev == 0x0e)
                break;
        }
    }
    return ev;
}
/* 0x423530, releases and frees the movie; returns 0 when no movie has that id */

int mov_Close(int id)
{
    MOVIE *m;

    m = mov_Find(id);
    if (m) {
        mov_Release(m);
        mov_Free(m);
        return 1;
    }
    return 0;
}

#define MOV_RELEASE(p) if (p) { (p)->lpVtbl->Release(p); (p) = NULL; }

/* 0x423560, releases every COM interface of the movie */
void mov_Release(MOVIE *m)
{
    MOV_RELEASE(m->iface[3]);
    MOV_RELEASE(m->iface[4]);
    MOV_RELEASE(m->iface[2]);
    MOV_RELEASE(m->iface[1]);
    MOV_RELEASE(m->iface[0]);
    MOV_RELEASE(m->video);
    MOV_RELEASE(m->audio);
    MOV_RELEASE(m->window);
    MOV_RELEASE(m->control);
    MOV_RELEASE(m->event);
    MOV_RELEASE(m->graph);
}
/* 0x423660, allocates a zeroed MOVIE and appends it to mov_List; NULL when malloc fails */

MOVIE *mov_Alloc(void)
{
    MOVIE *m, *p;

    m = malloc(sizeof(MOVIE));
    if (m) {
        memset(m, 0, sizeof(MOVIE));
        if (mov_List) {
            for (p = mov_List; p->next; p = p->next)
                ;
            p->next = m;
            m->prev = p;
            return m;
        }
        mov_List = m;
    }
    return m;
}
/* 0x4236b0, unlinks the movie from mov_List and frees it */

void mov_Free(MOVIE *m)
{
    if (!m)
        return;
    if (m->prev) {
        if (m->next) {
            m->next->prev = m->prev;
            m->prev->next = m->next;
        } else
            m->prev->next = NULL;
    } else if (m->next) {
        m->next->prev = NULL;
        mov_List = m->next;
    } else
        mov_List = NULL;
    free(m);
}
/* 0x423720, finds the movie with the given id; NULL when there is none */

MOVIE *mov_Find(int id)
{
    MOVIE *m = mov_List, *found = NULL;

    if (m != NULL) {
        do {
            if (m->id == id)
                break;
            m = m->next;
        } while (m != NULL);
        found = m;
    }
    return found;
}
/* 0x423740, finds the movie that was opened with the given window; NULL when there is none */

MOVIE *mov_FindWnd(HWND hwnd)
{
    MOVIE *m = mov_List, *found = NULL;

    if (m != NULL) {
        do {
            if (m->hwnd == hwnd)
                break;
            m = m->next;
        } while (m != NULL);
        found = m;
    }
    return found;
}

MOVIE * mov_List = 0;  /* 0x6323a4 */
ATOM mov_Class = {0};  /* 0x14613f8 */
int mov_NextId = 0;  /* 0x14613fc */
