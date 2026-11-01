/* mov.c -- movie playback through DirectShow ("MOVIEWNDCLASS"), original file name unknown.
   Extent in GildeGold.exe (inferred): 0x422c90..0x423770; 0x422c90 is the window procedure (not a
   Ghidra function). Needs <windows.h>, <strmif.h> and <control.h> first. Names are ours. */
#ifndef UNK_422C90_H
#define UNK_422C90_H
#include <objbase.h>                  /* COM */
#include <strmif.h>                   /* IGraphBuilder */
#include <control.h>                  /* IMediaControl / IMediaEvent */
#include <windows.h>                  /* Win32 types */

#define MOV_CENTER   0x01           /* mov_Open flags: center in the window */
#define MOV_FILL     0x02           /* use the whole client area */
#define MOV_ASPECT   0x04           /* keep the video's aspect ratio */
#define MOV_NOTIFY   0x08           /* send WM_GRAPHNOTIFY to the window */
#define MOV_FULL     0x80           /* own window: full screen popup */
#define MOV_OWNWND   0x100          /* own window: mov_Play runs a message loop */

#define WM_GRAPHNOTIFY 0x9021

typedef struct MOVIE {              /* 0x148 bytes, doubly linked list at mov_List */
    struct MOVIE *prev;
    struct MOVIE *next;             /* 0x04 */
    int id;                         /* 0x08 */
    unsigned int flags;             /* 0x0c */
    char file[0x104];               /* 0x10 */
    HWND hwnd;                      /* 0x114 */
    int unk118;
    IGraphBuilder *graph;           /* 0x11c */
    IMediaControl *control;         /* 0x120 */
    IMediaEventEx *event;           /* 0x124 */
    IBasicVideo *video;             /* 0x128 */
    IBasicAudio *audio;             /* 0x12c */
    IVideoWindow *window;           /* 0x130 */
    IUnknown *iface[5];             /* 0x134..0x144, released by mov_Release; this file never obtains them */
} MOVIE;

extern MOVIE *mov_List;                     /* 0x6323a4 */
extern ATOM mov_Class;                      /* 0x14613f8 */
extern int mov_NextId;                      /* 0x14613fc */

LRESULT CALLBACK mov_WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);               /* 0x422c90 */
int mov_Init(void);                                                                     /* 0x422d60 */
void mov_Exit(void);                                                                    /* 0x422d90 */
int mov_Open(char *file, HWND hwnd, HINSTANCE hinst, int x, int y, int w, int h, unsigned int flags); /* 0x422dc0, returns the id */
int mov_Play(int id);                                                                   /* 0x423390 */
long mov_GetEvent(int id, long *p1, long *p2);                                          /* 0x4234a0 */
int mov_Close(int id);                                                                  /* 0x423530 */
void mov_Release(MOVIE *m);                                                             /* 0x423560 */
MOVIE *mov_Alloc(void);                                                                 /* 0x423660 */
void mov_Free(MOVIE *m);                                                                /* 0x4236b0 */
MOVIE *mov_Find(int id);                                                                /* 0x423720 */
MOVIE *mov_FindWnd(HWND hwnd);                                                          /* 0x423740 */

#endif
