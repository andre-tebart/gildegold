/* d2.c -- 0x4687a0..0x476b90, probably D:\develop\Units\d2.v1\D2.C (memory tag "d2:d2_LoadObj", messages
   "d2_Show", "d2_OpenInputIcon()", "d2_OpenSlider"; GFX.C of the same directory follows): the 2D GUI layer.
   The picture cache, the object list and the dirty rectangle lists; input fields and sliders; slider and
   button drawing; radio groups, picture objects, drawing of all objects, hit tests, text width and drawing;
   windows (text layout, drawing, creation, scrolling, destruction, draw order) and views / forms.
   End: d2.c's .data runs to "d2:fadeinfo"; 0x4768a0 .. 0x476b40 use d2's globals and lists. Names are ours.
   Open: 0x476b70 (d2_ClearInput) may belong to gfx.c instead; the evidence does not decide, codegen neutral.
   Open: d2.h is not yet the only declaration of d2's functions (K&R copies in sv.c, unk_58bbd0.c,
   unk_5d33b0.c, unk_575860.c, cm.h, rt.h and others), and it declares vfs_ / err_ functions itself.
   Open: d2.h views D2WINF / D2PAINTWIN (of D2WIN) and D2TEXX (of D2TEX) are still to be folded into their types.
   unit of rea 8cef7d5b (src/game/d2.c); boundaries in decl/text.dcl */
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "err.h"
#include "di_input.h"
#include "txt.h"
#include "dd_vesa.h"
#include "d2.h"
#include "unk_47c5e0.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRACE */

#endif
#include "data_views.h"
#include "ch.h"
#include "cm.h"
#include "d3_interface.h"
#include "d3_io_a.h"
#include "d3s.h"
#include "gfx.h"
#include "io_text.h"
#include "mem.h"
#include "sv.h"
#include "tb.h"
#include "ts_texture.h"
#include "vfs.h"
#define d2_OpenPicS ((int (*)(short, short, int))d2_OpenPic)  /* 0x46cb20 (K&R, int x, y): @original d2_CreatePicObj (0x46cd80, byte) was compiled against the short prototype */
#define d2_MoveObjI ((void (*)(int, int, int))d2_MoveObj)  /* 0x46cda0 (d2.c: short x, short y): @original this unit passes the coordinates as ints (d2_CreatePicObj) */

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern struct D3OBJECT *d3s_Lights[4];

char d2_FormsDir[] = "forms\\";  /* 0x5fe608, "forms\\", prefix of a form file name */
unsigned int d2_PicLimit = 8000000;          /* 0x5fe70c, bytes of pictures kept in memory */
int d2_HitObjId = -1;  /* 0x5fe714 */
int d2_CurrentFont = -1;  /* 0x5fe718 */
char d2_NoBackgroundRestore = 1;  /* 0x5fe71c, 1: the alternate background path - captions are drawn without saving / restoring the background underneath (d2_DrawWindowBackground, d2_DrawObjects); it is only compared to 1, no writer is drafted */
char d2_Unk5fe71d = 2;  /* 0x5fe71d, a second 1-byte flag in the original's slot next to d2_NoBackgroundRestore: the value 2, never referenced by the code (no absolute address operand; the pad after d2_NoBackgroundRestore in our object would be zero) */
int d2_PixelFormat = 655;  /* 0x5fe720, display colour format code copied from dd_PixelFormat (888 / 565 / 555) */
int d2_SpaceWidth = 8;  /* 0x5fe724, width of a space */
int d2_LetterGap = 2;  /* 0x5fe728, gap between letters */
int d2_PointerWin = -1;  /* 0x5fe72c, window id under the pointer (d2_FindWindowAt scan) */
int d2_PointerWinVisible = -1;  /* 0x5fe730, visible window under the pointer (d2_HitTest object scan) */
int d2_ScrollMarkerPic = -1;              /* 0x5fe734 */
int d2_FramePic = -1;              /* 0x5fe738 */
int d2_DefaultFont = -1;              /* 0x5fe73c */
int d2_ButtonPulldownPic = -1;  /* 0x5fe740, "_BUTTON_PULLDOWN" picture */
int d2_WinBorderPic = -1;              /* 0x5fe744 */
int d2_WinBorderPergaPic = -1;              /* 0x5fe748 */
int d2_MiscPic = -1;  /* 0x5fe74c, _MISC picture, its frames are the 'i' command icons */
int d2_PointerPicIndex = -1;  /* 0x5fe750, "_MOUSE_CURSOR" */
int d2_ExtraSymbolsPic = -1;  /* 0x5fe754, "_EXTRA_SYMBOLS" */
int d2_TextCursorPic = -1;  /* 0x5fe758, "_TEXT_CURSOR" */
int d2_PointerMoved = 1;  /* 0x5fe75c */
int d2_ClipRight = 640;  /* 0x5fed7c, width of the screen */
int d2_ClipBottom = 480;  /* 0x5fed80, height of the screen */

int d2_LastPointerX = 0;  /* 0x633f10, last pointer x */
int d2_LastPointerY = 0;  /* 0x633f14, last pointer y */
D2FORM0 d2_Forms0[96] = {0};  /* 0x633f18 */
D2FORM2 d2_Forms2[96] = {0};  /* 0x68ce98 */
D2PIC *d2_Pics = 0;                 /* 0x6ed918 */
int d2_NPics = 0;                   /* 0x6ed91c */
int d2_PicBytes = 0;                /* 0x6ed920 */
void *d2_ScreenSurf = 0;               /* 0x6ed924 */
void *d2_FrontSurf = 0;               /* 0x6ed928 */
void *d2_BackSurf = 0;               /* 0x6ed92c, offscreen surface the layer draws into; blitted to d2_ScreenSurf */
D2SURF *d2_BackgroundTile = 0;          /* 0x6ed930 */
char d2_PicBase = 0;                /* 0x6ed934 */
int d2_LastWindow = 0;  /* 0x6ed938, window created last */
int d2_FrameCounter = 0;  /* 0x6ed940, frame counter */
D2PICHDR *d2_FontPic = 0;        /* 0x6ed948 */
int d2_HighestObj = 0;  /* 0x6ed94c, highest object index in use */
int d2_TextSegCount = 0;  /* 0x6ed954, number of text pieces */
int d2_CurrentView = 0;  /* 0x6ed958, the view a form was opened in */
D2TEX *d2_CaptionTex = 0;           /* 0x6ed960 */
D2OBJ **d2_DrawOrder = 0;          /* 0x6ed964, the objects in drawing order */
int d2_TextDrawnW = 0;  /* 0x6ed968 */
int d2_TextDrawnH = 0;  /* 0x6ed96c */
int d2_TextLeft = 0;  /* 0x6ed970, left edge of the text drawn last */
int d2_TextTop = 0;  /* 0x6ed974, top of the text drawn last */
unsigned int d2_PenSize = 0;  /* 0x6ed97c, pen size */
D2WIN * d2_LastWindowPtr = 0;  /* 0x6ed980, window created last */
int d2_FullFormPath = 0;  /* 0x6ed984, nonzero: the name given to d2_OpenForm is a full path (no d2_FormsDir prefix, no .form suffix); cleared after each load, nothing in the binary sets it */
FILE *d2_File = 0;                  /* 0x6ed988 */
int d2_PicShift = 0;                /* 0x6ed98c */
D2RECT *d2_Dirty[2] = {0};          /* 0x6ed994 */
void *d2_RedrawInfo2 = 0;           /* 0x6ed99c, third redraw info buffer ("d2:d2_RedrawInfo2") */
D2SHAPE *d2_Shapes = 0;             /* 0x6ed9a0 */
int d2_DirtyCount[2] = {0};         /* 0x6ed9a4 */
int d2_WindowsOpen = 0;  /* 0x6ed9b4, windows open */
D2WIN * d2_PendingCaptionWin = 0;  /* 0x6ed9b8, window of d2_PendingCaptionObj */
D2OBJ * d2_PendingCaptionObj = 0;  /* 0x6ed9bc, the object whose caption is drawn last
                                        (under the pointer / hit object): d2_DrawObjCaption, then 0 */
int d2_TextSegFlags = 0;  /* 0x6ed9c0, flags of the next text piece */
D2INPUT * d2_FocusedInput = 0;  /* 0x6ed9dc, the focused input */
short d2_ShpW = 0;  /* 0x6f044c */
short d2_ShpH = 0;  /* 0x6f044e */
int d2_ClipLeft = 0;  /* 0x6f0458, clip left */
int d2_ClipTop = 0;  /* 0x6f045c, clip top */
int d2_PaletteDepth = 0;  /* 0xf8a64c */
int d2_ClickedId = 0;  /* 0xf8a668 */
short d2_MouseX = 0;  /* 0xf8a674, mouse x */
short d2_MouseY = 0;  /* 0xf8a676, mouse y */
D2FRCT d2_PointerItems[6] = {0};  /* 0xf8ab80, the item list drawn next to the pointer */
D2DISSOLVE * d2_CurrentDissolve = 0;  /* 0xf8abc8 */
D2SEG d2_TextSegs[64] = {0};  /* 0xf8abe0 */
int d2_MainWindow = 0;  /* 0xf8eee0, the main window handle (HWND) given to d2_Open */
char d2_PointerTexName[128] = {0};  /* 0xf8ef00 */
D2OBJ * d2_Objs = 0;  /* 0xf8f080 */
int d2_FontHeight = 0;  /* 0xf8f084, font height */
int d2_HalfScreenH = 0;  /* 0xf8f088, half the screen height */
int d2_HalfScreenW = 0;  /* 0xf8f08c, half the screen width */
D2FADE * d2_Fades[32] = {0};  /* 0xf8f0a0, fades (d2_StartFade) */
D2WIN d2_Wins[0x60] = {0};  /* 0xf8f120, 96 windows: 0xfa5aa0 is d2_Radios */
D2RADIO d2_Radios[8] = {0};  /* 0xfa5aa0 */
/* d2_PointerX: view of ai_Unkfa5f04 (tools/data_views.py) */
int d2_PointerY = 0;  /* 0xfa5f0c, pointer y */
char d2_GfxPath[256] = {0};  /* 0xfa5f20 */
D2INPUT d2_Inputs[0x80] = {0};  /* 0xfa6020 */
D2VIEW d2_Views[0x30] = {0};  /* 0xfb1020 */
short d2_ScreenH = 0;  /* 0xfb9070 */
short d2_ScreenW = 0;  /* 0xfb9072 */
/* 0x143a1c0, the text pointer table: 0x4800 slots, the one block the text loader fills;
   d2_Strings (the first 122) and every named text pointer are views of it (txt_views.h) */
char *txt_Ptrs[0x4800] = {0};

/* 0x4687a0, drops the picture that was used longest ago (not locked, not in use); 0 if there is none */

int d2_FreeOldest(void)
{
    int i, best;
    unsigned int t;
    D2PIC *p;

    best = -1;
    t = d3_Time;
    for (i = 1; i < d2_NPics; i++) {
        p = &d2_Pics[i];
        if (p->lastuse < t && p->refs <= 0 && !(p->flags & 1) && p->data && p->size) {
            t = p->lastuse;
            best = i;
        }
    }
    if (best == -1)
        return 0;
    mem_Free(d2_Pics[best].data);
    d2_Pics[best].data = NULL;
    d2_Pics[best].data = NULL;     /* the original stores it twice (it reloads d2_Pics in between) */
    d2_PicBytes -= d2_Pics[best].size;
    return 1;
}
/* 0x468860, first free entry of the object list, -1 if it is full */

int d2_FindFreeObj(void)
{
    int i;

    for (i = 0; i < 0x2ff; i++)
        if (!d2_Objs[i].used)
            return i;
    return -1;
}
/* 0x468890, index of the picture with this name, -1 if there is none */

int d2_FindPic(char *name)
{
    int i;

    for (i = 0; i < 0x800; i++)
        if (d2_Pics[i].name[0] && !strcmp(name, d2_Pics[i].name))
            return i;
    return -1;
}
/* 0x468900, reads the picture idx from the file path (default: the archive) unless it is in memory */

int d2_LoadPic(int idx, char *path)
{
    char buf[128];
    void *data;

    if (!d2_Pics[idx].data) {
        while (d2_Pics[idx].size + d2_PicBytes > d2_PicLimit)
            if (!d2_FreeOldest())
                break;
        d2_Pics[idx].data = mem_Alloc(d2_Pics[idx].size, "d2:d2_LoadObj");
        if (!d2_Pics[idx].data) {
            sprintf(buf, "d2_Show: Memory could not be allocated for objectID(%i):%s", idx, d2_Pics[idx].name);
            return 0;
        }
        if (path)
            d2_File = fopen(path, "rb");
        else
            d2_File = fopen(d2_GfxPath, "rb");
        if (!d2_File)
            return 0;
        fseek(d2_File, d2_Pics[idx].offset, 0);
        fread(d2_Pics[idx].data, d2_Pics[idx].size, 1, d2_File);
        if (d2_Pics[idx].type < 0x10) {
            data = d2_Pics[idx].data;
            if (d2_PaletteDepth != ((D2PICHDR *)data)->bpp && ((D2PICHDR *)data)->bpp != 0)
                d2_Pics[idx].data = d2_BankConvertTo16(data, d2_PaletteDepth, 0);
        }
        d2_PicBytes += d2_Pics[idx].size;
    }
    fclose(d2_File);
    return 1;
}
/* 0x468a90, frees everything of the layer @deep Q4 blocks the byte match: the four raw *(int *) accesses of the D2PIC free loop rewritten as D2PIC *p = (D2PIC *)(i + base) with p->data / p->size flip the address operand order to [ecx+ebx*1+0x34] where the original has [ebx+ecx*1+0x34] (i+base and base+i both, all four accesses, build check REGRESSED d2_Exit) - the original uses the loop offset as the base register; Q2 needs 9 names (d2_RemoveWindow, d2_FreeFades, d2_FrontSurf/92c/930/948/960/964/99c, roles only partly evidenced in d2.h); Q3 0xfa5d20 is the window walk's pointer-compare constant: w starts at &d2_Wins[0].active (0xf8f120 + 0x280 = 0xf8f3a0) and the end is &d2_Wins[0x60].active (0xfa5d20), 0x60 windows; the typed spelling `while (w < &d2_Wins[0x60].active)` clears Q3 but loses the byte match (check 2609, the original compares against the immediate), so the literal stays */

void d2_Exit(void)
{
    int i, base, t;
    int *w;

    base = 0;
    d2_FreeFades();
    i = 0;
    w = &d2_Wins[0].active;
    do {
        if (*w == 1)
            d2_RemoveWindow(i);
        w += sizeof(D2WIN) / 4;
        i++;
    } while ((int)w < (int)((unsigned)&d2_Wins + 0x16c00));
    i = 0;
    do {
        d2_RemoveObj(i);
        i++;
    } while (i < 0x2ff);
    i = 0;
    do {
        base = (int)d2_Pics;
        t = *(int *)(i + 0x34 + base);
        if (t && *(int *)(i + 0x38 + base)) {
            mem_Free((void *)t);
            *(int *)(i + 0x34 + base) = 0;
            d2_PicBytes -= *(int *)(i + 0x38 + base);
            base = (int)d2_Pics;
        }
        i += 0x54;
    } while (i < 0x2a000);
    if (d2_BackSurf) {
        d2_FreeSurface(d2_BackSurf);
        d2_BackSurf = 0;
    }
    if (d2_ScreenSurf) {
        d2_FreeSurface(d2_ScreenSurf);
        d2_ScreenSurf = 0;
    }
    if (d2_FrontSurf) {
        d2_FreeSurface(d2_FrontSurf);
        d2_FrontSurf = 0;
    }
    if (d2_BackgroundTile) {
        d2_FreeSurface(d2_BackgroundTile);
        d2_BackgroundTile = 0;
    }
    d2_FreeLightTable();
    if (d2_CaptionTex) {
        d3if_FreeSprite((int)d2_CaptionTex);
        d2_CaptionTex = 0;
    }
    if (d2_File)
        fclose(d2_File);
    d2_FontPic = 0;
    d2_File = 0;
    if (d2_DrawOrder) {
        mem_Free(d2_DrawOrder);
        d2_DrawOrder = 0;
    }
    if (d2_Objs) {
        mem_Free(d2_Objs);
        d2_Objs = 0;
    }
    if (d2_Pics) {
        mem_Free(d2_Pics);
        d2_Pics = 0;
    }
    if (d2_Dirty[0]) {
        mem_Free(d2_Dirty[0]);
        d2_Dirty[0] = 0;
    }
    if (d2_Dirty[1]) {
        mem_Free(d2_Dirty[1]);
        d2_Dirty[1] = 0;
    }
    if (d2_RedrawInfo2) {
        mem_Free(d2_RedrawInfo2);
        d2_RedrawInfo2 = 0;
    }
    if (d2_Shapes) {
        mem_Free(d2_Shapes);
        d2_Shapes = 0;
    }
}
/* 0x468c70, draws the saved background back for the shapes of a buffer and removes them */

void d2_RestoreShapes(D2BUF *buf)
{
    D2SHAPE *s;
    int i;

    if (buf->active && buf->loaded) {
        for (i = 0; i < 0x300; i++) {
            s = &d2_Shapes[i];
            if (s->owner == buf) {
                d2_BlitPart(s->x, s->y, s->w, s->h, buf->srcSurf, s->x, s->y, buf->dstSurf);
                memset(s, 0, sizeof(D2SHAPE));
            }
        }
    }
}
/* 0x468cf0, puts an object on a window; 0 if it is full or the window does not exist */

int d2_AddToWin(int obj, int win)
{
    D2WIN *w;
    int t;

    if (win >= 0 && win <= 0x60) {
        w = &d2_Wins[win];
        if (w->active) {
            if (w->nobjs >= 0x180) {
                err_Message("Too many objects on window!");
                return 0;
            }
            d2_AddToDrawOrder(obj, w);
            d2_Objs[obj].win = w;
            d2_Objs[obj].buf = d2_Objs[w->parent].buf;
            d2_Objs[obj].clipy0 = w->y;
            d2_Objs[obj].clipy1 = w->h + w->y;
            d2_Objs[obj].clipx0 = w->x;
            d2_Objs[obj].clipx1 = w->w + w->x;
            d2_Objs[obj].disabled = d2_Objs[w->parent].disabled;
            t = (d2_Objs[obj].h + d2_Objs[obj].y) - w->y;
            if (t > w->contentHeight)
                w->contentHeight = t + 4;
            w->objs[w->nobjs] = obj;
            w->nobjs++;
            return 1;
        }
    }
    return 0;
}
/* 0x468e30, draws the saved background back for all dirty rectangles of the current page */

void d2_FlushDirty(void)
{
    int n = d2_DirtyCount[dd_Page];
    int i;
    D2RECT *r;

    if (n > 0) {
        i = 0;
        do {
            r = (D2RECT *)((char *)d2_Dirty[dd_Page] + i);
            d2_BlitPart(r->x, r->y, r->w, r->h, d2_BackSurf, r->x, r->y, d2_ScreenSurf);
            i += 0x10;
            n--;
        } while (n != 0);
    }
    d2_DirtyCount[dd_Page] = 0;
}
/* 0x468eb0, adds a rectangle (clipped, x made even) to the dirty list of the current page */

void d2_AddDirty(int x, int y, int w, int h)
{
    int i = dd_Page;
    int n = d2_DirtyCount[i];
    D2RECT **dp;

    if (n < 0x300) {
        if (x > d2_ClipRight)
            x = d2_ClipRight;
        if (x & 1) {
            x--;
            w = (w + 2) & ~1;
        }
        if (y + h > d2_ClipBottom)
            h = d2_ClipBottom - y;
        if (x + w > d2_ClipRight)
            w = d2_ClipRight - x;
        if (y < d2_ClipTop)
            y = d2_ClipTop;
        if (x < d2_ClipLeft)
            x = d2_ClipLeft;
        if (0 < w && 0 < h && (y <= d2_DirtyRectTop || y + h >= d2_DirtyRectBottom || x <= d2_DirtyRectLeft ||
                               x + w >= d2_DirtyRectRight)) {
            dp = &d2_Dirty[dd_Page];
            (*dp)[n].x = x;
            (*dp)[n].y = y;
            (*dp)[n].w = w;
            (*dp)[n].h = h;
            d2_DirtyCount[i]++;
        }
    }
}
/* 0x468fb0, adds a rectangle (clipped, x made even) to the shapes of a buffer */

void d2_AddShape(int x, int y, int w, int h, D2BUF *buf)
{
    int i;

    if (buf->active && buf->srcSurf) {
        if (x > d2_ClipRight)
            x = d2_ClipRight;
        if (x & 1) {
            x--;
            w = (w + 2) & ~1;
        }
        if (h + y > d2_ClipBottom)
            h = d2_ClipBottom - y;
        if (x + w > d2_ClipRight)
            w = d2_ClipRight - x;
        if (y < d2_ClipTop)
            y = d2_ClipTop;
        if (x < d2_ClipLeft)
            x = d2_ClipLeft;
        if (w < 0)
            w = 1;
        if (h < 0)
            h = 1;
        i = 0;
        while (d2_Shapes[i].owner) {
            if (i >= 0x300)
                return;
            i++;
        }
        if (i >= 0x300)
            return;
        d2_Shapes[i].x = x;
        d2_Shapes[i].y = y;
        d2_Shapes[i].w = w;
        d2_Shapes[i].h = h;
        d2_Shapes[i].owner = buf;
    }
}
/* 0x4690b0, data of the picture idx, loading it if needed; a frame (type 5 or 8) of another picture gives
   the data of that picture and sets d2_PicShift to the distance of the two */

void *d2_GetPic(int idx)
{
    int base;

    d2_PicShift = 0;
    if (d2_Pics[idx].flags & 2)
        idx += d2_PicBase;
    if ((d2_Pics[idx].type == 5 || d2_Pics[idx].type == 8) && d2_Pics[idx].offset == 0) {
        base = d2_Pics[idx].base;
        if (!d2_Pics[base].data)
            d2_LoadPic(base, 0);
        base = idx;
        idx = d2_Pics[idx].base;
        d2_PicShift = base - idx;
    } else if (!d2_Pics[idx].data)
        d2_LoadPic(idx, 0);
    return d2_Pics[idx].data;
}
/* 0x469180, the same, giving the data and the index of the picture used */

int d2_GetPicEx(int idx, void **pic, int *real)
{
    int j = idx;

    d2_PicShift = 0;
    if (d2_Pics[idx].flags & 2)
        idx += d2_PicBase;
    if ((d2_Pics[idx].type == 5 || d2_Pics[idx].type == 8) && d2_Pics[idx].offset == 0) {
        if (!d2_Pics[d2_Pics[idx].base].data)
            d2_LoadPic(d2_Pics[idx].base, 0);
        j = d2_Pics[idx].base;
        d2_PicShift = idx - j;
    } else if (!d2_Pics[idx].data)
        d2_LoadPic(idx, 0);
    *pic = d2_Pics[j].data;
    *real = idx;
    return 1;
}
/* 0x469250, gives an object another picture */

void d2_SetObjPic(int obj, int pic)
{
    D2OBJ *o = &d2_Objs[obj];

    if (o->pic != pic) {
        if (o->type < 0x40)
            d2_SubRefcount(o->pic);
        o->data = d2_GetPic(pic);
        o->input = d2_PicShift;
        o->pic = d2_PicShift + pic;
        o->w = d2_Pics[pic].w;
        o->h = d2_Pics[pic].h;
        o->clipy0 = 0;
        o->clipy1 = d2_ScreenH;
        o->clipx0 = 0;
        o->clipx1 = d2_ScreenW;
        if (o->type < 0x40)
            d2_AddRefcount(o->pic);
    }
}
/* 0x469310, draws the frame of a dialog with its caption number */

void d2_DrawFrame(D2INPUT *dlg, void *arg)
{
    D2OBJ *o = &d2_Objs[dlg->obj];
    void *pic = d2_GetPic(d2_FramePic);
    D2FRAME *f0 = d2_GetFrame(pic, 0);
    D2FRAME *f1 = d2_GetFrame(pic, 1);
    D2FRAME *f3;
    int x, y;
    char buf[24];

    d2_GetFrame(pic, 2);
    f3 = d2_GetFrame(pic, 3);
    x = o->x - 0x20;
    y = (dlg->frameY - f0->h) - 100;
    d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
    d2_DrawShape(x, y, arg, pic, 0);
    d2_DrawShape(x, f0->h + 100 + y, arg, pic, 1);
    d2_DrawShape(x, f0->h + y, arg, pic, 2);
    d2_AddDirty(x - 0xc, y, 0x40, f1->h + 100 + f0->h);
    y = dm_L.y - (f3->h >> 1);
    x += (f0->w >> 1) - (f3->w >> 1);
    d2_DrawShape(x, y, arg, pic, 3);
    d2_PointerX = x;
    d2_PointerY = y;
    if (dlg->value < 100000 && -100000 < dlg->value) {
        d2_SetFont(d2_DefaultFont + 1);
        itoa(dlg->value, buf, 10);
        d2_DrawTextBox(d2_PointerX, d2_PointerY, f3->w, f3->h, arg, buf, 8);
    }
}
/* 0x4694c0, draws an input field (and the slider-like frame around it) @deep B draft: 3 code hunks (6 instructions) at 0x4697a3/0x4697d6/0x4697e9 in the number-input frame block (in->w = p0->w + in->ndigits * 0xe + p1->w; o->w = in->w; bw = in->w - p0->w - p1->w; d2_DrawShape(o->x - p1->w + o->w, ...)): the original loads the WORD o->w ([edi+0x14]) where ours reuses the int in->w ([esi+0x10]) and reads o->x/o->w in the other order - same values, different loads; bw = o->w - p0->w - p1->w tried, 6 code hunks became 165 (whole-function allocation), reverted; Q2 needs 12 names (d2_DrawText, d2_DrawShapeMode2, d2_PointerObj/14, d2_ButtonPulldownPic, d2_PendingCaptionWin/9bc, D2OBJ.textShadow/greyed/picW/picH/1e4) */

void d2_DrawInput(int idx, void *surf)
{
    D2INPUT *in = &d2_Inputs[idx];
    D2OBJ *o;
    D2PICHDR *pic;
    D2PICHDR *ip;
    D2FRAME *p0, *p1, *p2;
    unsigned int flags;
    int n, v, bw, i, hi, x0, y1;
    volatile int hv;
    char text[32];
    char fmt[32];
    char pct[64];

    d2_SetFont(in->font);
    flags = in->flags;
    if (flags & 1) {
        if (in->icon == -1 && !(flags & 0x200))
            in->w = d2_TextWidth(in->text, in->font);
        else
            in->w = d2_Objs[in->obj].w;
        if (in->w < 0x30)
            in->w = 0x30;
        if (in->maxw > 0)
            in->w = min(in->w, in->maxw);
        d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
        d2_Objs[in->obj].w = in->w;
        if (d2_Objs[in->obj].highlighted == 0 && d2_PointerObj != in->obj)
            d2_DrawText(in->x, in->y, surf, in->text, 0);
        else
            d2_DrawText(in->x, in->y, surf, in->text, 2);
    } else if (flags & 2) {
        n = 0;
        v = in->maxval;
        do {
            if (v <= 0)
                break;
            v /= 10;
            n++;
        } while (n < 10);
        in->ndigits = n;
        if (flags & 4) {
            if (in->mult * in->value < 1000)
                sprintf(text, "%li g", in->mult * in->value);
            else if (in->mult * in->value < 1000000)
                sprintf(text, "%.1f kg", in->mult * in->value * 0.001);
            else
                sprintf(text, "%.1f t", in->mult * in->value * 1e-06);
        } else if (flags & 8) {
            sprintf(fmt, "%%0%ii Stk.", in->ndigits);
            sprintf(text, fmt, in->mult * in->value);
        } else {
            sprintf(text, "%i", in->value);
        }
        if (in->icon == -1) {
            o = &d2_Objs[in->obj];
            pic = d2_GetPic(d2_ButtonPulldownPic);
            p0 = d2_GetFrame(pic, 0);
            p1 = d2_GetFrame(pic, 1);
            p2 = d2_GetFrame(pic, 2);
            in->w = p0->w + in->ndigits * 0xe + p1->w;
            o->w = in->w;
            o->x = in->x;
            bw = in->w - p0->w - p1->w;
            d2_DrawShape(o->x, o->y, surf, pic, 0);
            d2_DrawShape(o->x - p1->w + o->w, o->y, surf, pic, 1);
            x0 = o->x + (unsigned)p0->w;
            d2_SetClipRect(x0 - 1, d2_ClipTop, x0 + bw, d2_ClipBottom);
            i = 0;
            if ((double)(bw / p2->w) + 0.5 > 0.0) {
                do {
                    d2_DrawShape(p2->w * i + p0->w + o->x, o->y, surf, pic, 2);
                    i++;
                } while ((double)i < (double)(bw / p2->w) + 0.5);
            }
            d2_SetFont(d2_DefaultFont);
            d2_DrawTextBox(o->x, o->y + 1, bw, o->h, surf, text, 0x10);
        } else {
            ip = d2_GetPic(in->icon);
            d2_Objs[in->obj].picW = ip->w;
            d2_Objs[in->obj].picH = ip->h;
            if (d2_Objs[in->obj].textShadow)
                d2_DrawShapeMode2(in->x + 8, in->y + 8, d2_ScreenSurf, ip, d2_PicShift);
            d2_DrawShape(in->x, in->y, surf, ip, d2_PicShift);
            o = &d2_Objs[in->obj];
            if (d2_Objs[in->obj].greyed) {
                if (o->fillFraction == 0.0f)
                    d2_DrawShapeMode2(in->x, in->y, surf, ip, d2_PicShift);
            }
            o = &d2_Objs[in->obj];
            if (d2_Objs[in->obj].fillFraction != 0.0f) {
                hv = hi = d2_ClipBottom;
                y1 = (int)((1.0 - o->fillFraction) * o->h + in->y);
                if (hi < y1)
                    y1 = hi;
                d2_SetClipRect(d2_ClipLeft, d2_ClipTop, d2_ClipRight, y1);
                d2_DrawShapeMode2(in->x, in->y, d2_ScreenSurf, ip, d2_PicShift);
                d2_SetClipRect(d2_ClipLeft, d2_ClipTop, d2_ClipRight, hv);
                if (d2_PointerObj != o->id) {
                    sprintf(pct, "%i%%", (int)((d2_Objs[in->obj].fillFraction > 1.0 ? 1.0 : d2_Objs[in->obj].fillFraction) * 100.0));
                    d2_SetFont(d2_DefaultFont + 1);
                    d2_DrawTextBox(in->x, o->y + o->h - 0xf, ip->w, in->h, surf, pct, 0x28);
                }
            }
        }
        if (in->icon == -1 || (in->flags & 0x100) || in->value == 0 || (in->flags & 0x20)) {
            strcpy(in->text, text);
        } else if (d2_Objs[in->obj].highlighted) {
            d2_DrawText(in->x, d2_Pics[in->icon].h + in->y, surf, text, 2);
        } else {
            d2_DrawText(in->x, d2_Pics[in->icon].h + in->y, surf, text, 0);
        }
    }
    if (d2_HitObjId != -1) {
        o = &d2_Objs[in->obj];
        if (d2_Objs[in->obj].label[0x20] && d2_Objs + d2_HitObjId == o && d2_FocusedInput == 0) {
            d2_PendingCaptionObj = o;
            d2_PendingCaptionWin = o->win;
        }
    }
}
/* 0x469ca0, opens a number input with an icon */

int d2_OpenInputIcon(int x, int y, int icon, unsigned char flags)
{
    int i;
    int obj;
    D2INPUT *in;
    D2PIC *ic;
    int sh;
    int n, v;
    char fmt[8];
    char buf[16];

    i = 1;
    if (d2_Inputs[1].used) {
        while (d2_Inputs[++i].used)
            ;
        if (i > 0x7f) {
            err_Message("d2_OpenInputIcon(): Too many inputs on screen...");
            return -1;
        }
    }
    in = &d2_Inputs[i];
    in->x = x;
    in->y = y;
    in->icon = icon;
    in->id = i;
    ic = &d2_Pics[icon];
    in->w = ic->w;
    in->h = ic->h;
    in->minval = 0;
    in->value = 0;
    in->flags = flags | 0x100;
    in->maxval = 999;
    in->ndigits = 3;
    in->used = 1;
    in->font = d2_FindPic("_FONT");
    d2_AddRefcount(icon);
    if (flags & 2) {
        n = 0;
        v = in->maxval;
        do {
            if (v <= 0)
                break;
            v /= 10;
            n++;
        } while (n < 10);
        in->ndigits = n;
        sprintf(fmt, "%%0%ii", in->ndigits);
        sprintf(buf, fmt, in->mult * in->value);
    }
    if (flags & 0x80) {
        in->maxval = max(in->maxval, 0x19);
    }
    obj = d2_NewObject();
    d2_Objs[obj].type = 0x41;
    d2_Objs[obj].x = in->x;
    d2_Objs[obj].y = in->y;
    d2_Objs[obj].w = in->w;
    d2_Objs[obj].h = in->h;
    d2_Objs[obj].pic = icon;
    d2_Objs[obj].input = i;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    d2_Objs[obj].data = in;
    d2_Objs[obj].font = d2_DefaultFont;
    in->obj = obj;
    return obj;
}
/* 0x469ee0, the same on a window */

int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win)
{
    D2WIN *w = &d2_Wins[win];
    int obj;

    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    obj = d2_OpenInputIcon(w->x + x, w->y - w->scrollY + y, icon, flags);
    d2_AddToDrawOrder(obj, w);
    d2_Objs[obj].win = w;
    d2_Objs[obj].buf = d2_Objs[w->parent].buf;
    d2_Inputs[d2_Objs[obj].input].font = w->font;
    w->objs[w->nobjs] = obj;
    w->nobjs++;
    if (y > w->contentHeight)
        w->contentHeight = y + 4;
    return obj;
}
/* 0x469fe0, puts string number str (d2_Strings) into the input's text buffer, in front of the window text */

void d2_InputSetString(int in, int str)
{
    D2INPUT *p = &d2_Inputs[in];
    D2WIN *w = d2_Objs[p->obj].win;

    if (w && p->buf) {
        if (p->buflen)
            memmove(p->buf, p->buf + p->buflen, strlen(w->text) + (w->text - p->buf) + 1);
        p->buflen = strlen(txt_Ptrs[str]);
        memmove(p->buf + p->buflen, p->buf, strlen(w->text) + (w->text - p->buf) + 1);
        memcpy(p->buf, txt_Ptrs[str], p->buflen);
        p->w = d2_TextWidth(txt_Ptrs[str], p->font);
        d2_Objs[p->obj].w = p->w;
    }
}
/* 0x46a110, opens a number input without icon @deep Q2 only: D2INPUT.unk144 is written once (in->unk144 = 1 beside mult = 1 in d2_OpenInput's slot setup; Ghidra 0x46a110 stores 1 at D2INPUT+0x144) and has no reader in the drafted sources, no string, caller or data evidence and no header comment - a name would be invented; the other identifiers cleared with D2OBJ.hidden -> hidden */

int d2_OpenInput(int x, int y, int w, int h, unsigned char flags)
{
    int i;
    int obj;
    D2INPUT *in;
    int n, v;
    char fmt[8];
    char buf[16];

    i = 1;
    if (d2_Inputs[1].used) {
        while (d2_Inputs[++i].used)
            ;
        if (i > 0x7f) {
            err_Message("d2_OpenInputIcon(): Too many inputs on screen...");
            return -1;
        }
    }
    in = &d2_Inputs[i];
    in->id = i;
    in->x = x;
    in->y = y;
    in->w = w;
    in->h = h;
    in->maxval = 999;
    in->minval = 0;
    in->value = 0;
    in->mult = 1;
    in->unk144 = 1;
    in->ndigits = 3;
    in->flags = flags;
    in->used = 1;
    in->font = d2_FindPic("_FONT");
    in->icon = -1;
    in->buf = 0;
    obj = d2_NewObject();
    if (flags & 2) {
        n = 0;
        v = in->maxval;
        do {
            if (v <= 0)
                break;
            v /= 10;
            n++;
        } while (n < 10);
        in->ndigits = n;
        sprintf(fmt, "%%0%ii", in->ndigits);
        sprintf(buf, fmt, in->value * in->mult);
        in->w = d2_TextWidth(buf, in->font);
    }
    if (flags & 1)
        in->maxval = 0x20;
    if (flags & 0x30)
        d2_Objs[obj].hidden = 1;
    d2_Objs[obj].type = 0x41;
    d2_Objs[obj].x = in->x;
    d2_Objs[obj].y = in->y;
    d2_Objs[obj].w = in->w;
    d2_Objs[obj].h = in->h;
    d2_Objs[obj].pic = 0;
    d2_Objs[obj].input = in->id;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].clipy0 = in->y;
    d2_Objs[obj].clipx0 = in->x;
    d2_Objs[obj].clipx1 = in->w + in->x;
    d2_Objs[obj].clipy1 = in->h + in->y;
    d2_Objs[obj].data = in;
    in->obj = obj;
    return obj;
}
/* 0x46a360, the same on a window */

int d2_WinOpenInput(int x, int y, int w, int h, unsigned char flags, int win)
{
    D2WIN *wn = &d2_Wins[win];
    int obj;

    if (!wn->active)
        return -1;
    if (wn->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    obj = d2_OpenInput(wn->x + x, wn->y - wn->scrollY + y, w, h, flags);
    d2_AddToDrawOrder(obj, wn);
    d2_Objs[obj].win = wn;
    d2_Objs[obj].buf = d2_Objs[wn->parent].buf;
    d2_Objs[obj].clipy0 = wn->y;
    d2_Objs[obj].clipy1 = wn->h + wn->y;
    d2_Objs[obj].clipx0 = wn->x;
    d2_Objs[obj].clipx1 = wn->x + wn->w;
    d2_Inputs[d2_Objs[obj].input].font = wn->font;
    d2_Inputs[d2_Objs[obj].input].maxw = wn->x - d2_Objs[obj].x - wn->mr + wn->w;
    wn->objs[wn->nobjs] = obj;
    wn->nobjs++;
    return obj;
}
/* 0x46a4c0, opens a slider: flags 1 vertical, 2 horizontal; pic is the first of the icons for its end caps,
   body and thumb */

int d2_OpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags)
{
    int obj;
    int i, cnt;
    int mw, mh;
    D2PIC *ic0, *ic1, *ic3;
    D2PICHDR *frame;
    int desc[16];

    obj = d2_NewObject();
    ic1 = &d2_Pics[pic + 1];
    ic0 = &d2_Pics[pic];
    ic3 = &d2_Pics[pic + 3];
    if (flags & 1) {
        cnt = len / ic1->h;
        if (len % ic1->h)
            cnt++;
        mh = ic1->h * cnt + ic0->h * 2;
        mw = max(ic1->w, ic0->w);
        mw = max(mw, ic3->w);
        d2_Objs[obj].w = mw;
        d2_Objs[obj].h = ic0->h * 2 + len;
    } else if (flags & 2) {
        cnt = len / ic1->w;
        if (len % ic1->w)
            cnt++;
        mw = ic1->w * cnt + ic0->w * 2;
        mh = max(ic1->h, ic0->h);
        mh = max(mh, ic3->h);
        d2_Objs[obj].w = len + ic0->w * 2;
        d2_Objs[obj].h = mh;
    }
    d2_Objs[obj].type = 0x45;
    d2_Objs[obj].interactive = 1;
    d2_Objs[obj].x = x;
    d2_Objs[obj].y = y;
    d2_Objs[obj].pic = 0;
    d2_Objs[obj].u.s.value = p4;
    d2_Objs[obj].u.s.max = p5;
    d2_Objs[obj].u.s.flags = flags;
    d2_Objs[obj].u.s.min = p4;
    d2_Objs[obj].u.s.len = len;
    d2_Objs[obj].u.s.value2 = p5 / 2;
    d2_Objs[obj].u.s.firstPic = pic;
    d2_Objs[obj].scale = 1;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    memset(desc, 0, sizeof desc);
    desc[1] = mw * 3;
    desc[2] = mh * 3;
    d2_Objs[obj].u.s.img = d2_MakeSurface(desc, 0xb);
    if (!d2_Objs[obj].u.s.img) {
        err_Message("d2_OpenSlider:Could not create surface...");
        return -1;
    }
    frame = d2_GetPic(pic);
    d2_Objs[obj].data = frame;
    d2_ClearSurface(d2_Objs[obj].u.s.img);
    d2_LockSurface(d2_Objs[obj].u.s.img);
    for (i = 0; i < cnt; i++) {
        if (flags & 2) {
            d2_DrawShape(ic1->w * i, 0, d2_Objs[obj].u.s.img, frame, 2);
            d2_DrawShape(ic1->w * i, ic1->h, d2_Objs[obj].u.s.img, frame, 1);
            d2_DrawShape(ic1->w * i, ic1->h * 2, d2_Objs[obj].u.s.img, frame, 4);
        } else if (flags & 1) {
            d2_DrawShape(0, ic1->h * i, d2_Objs[obj].u.s.img, frame, 2);
            d2_DrawShape(ic1->w, ic1->h * i, d2_Objs[obj].u.s.img, frame, 1);
            d2_DrawShape(ic1->w * 2, ic1->h * i, d2_Objs[obj].u.s.img, frame, 4);
        }
    }
    d2_UnlockSurface(d2_Objs[obj].u.s.img);
    return obj;
}
/* 0x46a960, the same on a window */

int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win)
{
    D2WIN *w;
    int obj;

    if (win >= 0 && win <= 0x60 && (w = &d2_Wins[win])->active) {
        if (w->nobjs >= 0x180) {
            err_Message("Too many objects on window!");
        } else {
            obj = d2_OpenSlider(w->x + x, w->y - w->scrollY + y, len, p4, p5, pic, flags);
            d2_AddToWin(obj, win);
            return obj;
        }
    }
    return -1;
}
/* 0x46aa00, draws a slider (flags 2: horizontal, 1: vertical): track, filled part, thumb, arrows, value @deep B draft, 8 differing instructions (6 code + order at 0x46b5d5 / 0x46b60c): the member load order and ecx/edx split of w->x + w->w + 4 in the two max-label calls (original: x->ecx, w->edx; ours: w->ecx, x->edx; lea [ecx+edx*1+0x4] symmetric, only the loads differ). spell.py: no menu rule matched either hunk ('escalate with the diffclass verdict'); reading found no meaning error (same values, same order of the y/x/w loads otherwise). Leads tried: 13 source forms in rounds 15-19 (both operand orders, parentheses, casts, temp, dead refs, nested struct, d2_Objs[id]. form) and the ~60-variant member-load-order campaign of d2_DrawInput / d2_DrawButton. Readability done (d2_DrawSlider, d2_DrawText, d2_ListCopyItem, d2_PutPixel/Clipped, d2_TextDrawnW/H, D2CTRL.loaded). */

void d2_DrawSlider(int id, D2SURF *surf)
{
    D2OBJ *w;
    D2FRAME *spr0, *spr1, *spr3, *spr6;
    void *sprites;
    unsigned int flags;
    int len, value, max, min, range, value2;
    float flen, scale, pos1, pos2, d, t;
    int hilo = 0, hihi = 0;
    int i, j, k, mx, my, tx, ty, tw, dy, color, x1, y1, x2, y2;
    DWORD c;
    unsigned char rgb[3];
    char buf2[32];
    char buf[512];

    sprites = d2_Objs[id].data;
    w = &d2_Objs[id];
    spr1 = d2_GetFrame(sprites, 1);
    spr0 = d2_GetFrame(sprites, 0);
    spr3 = d2_GetFrame(sprites, 3);
    spr6 = d2_GetFrame(sprites, 6);
    value = w->u.s.value;
    flags = w->u.s.flags;
    len = w->u.s.len;
    max = w->u.s.max;
    min = w->u.s.min;
    value2 = w->u.s.value2;
    range = max - min;
    if (range < 1)
        range = 1;
    flen = (float)len;
    scale = flen / range;
    pos1 = (value - min) * scale;
    pos2 = (value2 - min) * scale;
    if (pos1 > 0.0f && pos1 < 1.0f)
        pos1 = 1.0f;
    if (pos2 > 0.0f && pos2 < 1.0f)
        pos2 = 1.0f;
    if (flags & 2) {
        if ((d = flen - pos1) > 0.0f && !(flags & 8))
            d2_BlitPart((int)pos1, spr1->h, (int)(d + 1.0f), spr1->h, w->u.s.img,
                         (int)(w->x + spr0->w + pos1), (spr3->h - spr1->h) / 2 + w->y, surf);
        if (!(flags & 0x400)) {
            if ((flags & 0x10) && pos2 > 0.0f)
                d2_BlitPart(0, spr1->h * 2, (int)pos2, spr1->h, w->u.s.img,
                             spr0->w + w->x, (spr3->h - spr1->h) / 2 + w->y, surf);
            if (pos1 > 0.0f) {
                if (!(flags & 8))
                    d2_BlitPart(0, 0, (int)pos1, spr1->h, w->u.s.img,
                                 spr0->w + w->x, (spr3->h - spr1->h) / 2 + w->y, surf);
                else
                    d2_BlitPart(0, spr1->h, (int)pos1, spr1->h, w->u.s.img,
                                 spr0->w + w->x, (spr3->h - spr1->h) / 2 + w->y, surf);
            }
        } else {
            dd_RGB(0xff, 0x80, 0);
            t = pos1 / flen * 2.0f;
            if (t < 1.0) {
                rgb[0] = 0xc4 - (int)(-24.0f * t);
                rgb[1] = (int)(220.0f * t);
                rgb[2] = (int)(t * 0.0f);
            } else {
                t -= 1.0;
                rgb[0] = 0xdc - (int)(194.0 * t);
                rgb[1] = 0xdc - (int)(135.0 * t);
                rgb[2] = (int)(t * 0.0);
            }
            if (w->h + w->y <= surf->bottom && w->y >= surf->top) {
                for (j = 0; j < w->h; j++)
                    for (i = 0; i < pos1; i++) {
                        c = dd_RGB(rgb[0], rgb[1], rgb[2]);
                        d2_PutPixel(w->x + (i + spr0->w), w->y + (j + (spr3->h - spr1->h) / 2), c, surf);
                    }
            } else {
                for (j = 0; j < w->h; j++)
                    for (i = 0; i < pos1; i++) {
                        c = dd_RGB(rgb[0], rgb[1], rgb[2]);
                        d2_PutPixelClipped(w->x + (i + spr0->w), w->y + (j + (spr3->h - spr1->h) / 2), c, surf);
                    }
            }
            d2_SetClipRect(w->x, surf->top, spr0->w + w->x + len, surf->bottom);
            for (k = 0; k < len / spr1->w + 0.5; k++)
                d2_DrawShape(spr1->w * k + spr0->w + w->x, (spr3->h - spr1->h) / 2 + w->y, surf, sprites, 1);
            d2_SetClipRect(w->clipx0, w->clipy0, w->clipx1, w->clipy1);
        }
        if (!(flags & 0x20)) {
            if (d2_PointerObj == w->id) {
                if (w->buf && w->buf->loaded)
                    mx = d2_PointerX;
                else
                    mx = d2_MouseX;
                if (mx >= w->x && mx < spr6->w + w->x)
                    hilo = 1;
                else if (mx >= w->w - spr6->w + w->x && mx < w->w + w->x)
                    hihi = 1;
            }
            w->hilo = hilo;
            w->hihi = hihi;
            d2_DrawShape(w->x, (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 0);
            if (hilo)
                d2_DrawShapeMode3(w->x, (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 6);
            if (!d2_DrawShape(w->x + (len + spr0->w), (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 5))
                d2_DrawShape(w->x + (len + spr0->w), (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 0);
            if (hihi)
                d2_DrawShapeMode3(w->w - spr6->w + w->x, (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 7);
        }
        if (!(flags & 0x48)) {
            d2_DrawShape((int)((float)spr0->w + w->x + pos1 - spr3->w / 2 + 0.5f), w->y, surf, sprites, 3);
            if (w->list[0]) {
                d2_ListCopyItem(id, value, buf);
                if (w->font)
                    d2_SetFont(w->font);
                else
                    d2_SetFont(d2_DefaultFont + 1);
                x1 = (int)((float)spr0->w + w->x + pos1 - spr3->w / 2);
                y1 = spr3->h / 2 - d2_FontHeight / 2 + w->y + 1;
                d2_DrawTextBox(x1, y1, spr3->w, spr3->h, surf, buf, 8);
                d2_AddDirty(x1, y1, d2_TextDrawnW, d2_TextDrawnH);
            } else {
                if (w->font)
                    d2_SetFont(w->font);
                else
                    d2_SetFont(d2_DefaultFont + 1);
                x2 = (int)((float)spr0->w + w->x + pos1 - spr3->w / 2);
                y2 = spr3->h / 2 - d2_FontHeight / 2 + w->y + 2;
                if (flags & 0x1000)
                    sprintf(buf, "%i%%", w->scale * value);
                else if (flags & 0x800)
                    sprintf(buf, "%i%%", (int)(value * 100.0f / max + 0.5));
                else
                    sprintf(buf, "%i", w->scale * value);
                d2_DrawTextBox(x2, y2, spr3->w, spr3->h, surf, buf, 8);
                d2_AddDirty(x2, y2, d2_TextDrawnW, d2_TextDrawnH);
            }
        }
        if (flags & 4) {
            if (w->font)
                d2_SetFont(w->font);
            else if (w->win)
                d2_SetFont(w->win->font);
            else
                d2_SetFont(d2_DefaultFont + 1);
            if (w->dirty) {
                _itoa(w->scale * value, buf2, 10);
                d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
                d2_DrawText((int)(w->x + pos1), w->y - 0x10, surf, buf2, 0);
                d2_AddDirty((int)(w->x + pos1), w->y - 0x10, d2_TextDrawnW, d2_TextDrawnH);
            }
            sprintf(buf2, "%i", min);
            dy = (d2_FontHeight - spr0->h) / 2 - 1;
            d2_DrawText(w->x - d2_TextWidth(buf2, d2_CurrentFont) - 4, w->y - dy, surf, buf2, 0);
            d2_AddDirty(w->x - 0x16, w->y + dy, d2_TextDrawnW, d2_TextDrawnH);
            sprintf(buf2, "%i", max);
            d2_DrawText(w->x + w->w + 4, w->y - dy, surf, buf2, 0);
            d2_AddDirty(w->x + w->w + 4, w->y - dy, d2_TextDrawnW, d2_TextDrawnH);
        }
        if (w->label[0]) {
            color = 8;
            if (w->font)
                d2_SetFont(w->font);
            else
                d2_SetFont(d2_DefaultFont + 1);
            tx = w->x;
            ty = w->y - 0xd;
            tw = w->w;
            if (flags & 0x80) {
                tw = d2_TextWidth(w->label, d2_DefaultFont);
                ty = w->y;
                tx = w->x - tw - 0x18;
                color = 0x10;
            }
            d2_DrawTextBox(tx, ty, tw, 0x20, surf, w->label, color);
            d2_AddDirty(tx, ty, d2_TextDrawnW, d2_TextDrawnH);
        }
    } else if (flags & 1) {
        if ((d = flen - pos1) > 0.0f && !(flags & 8))
            d2_BlitPart(spr1->w, 0, spr1->w, (int)(d + 1.0f + 1.5), w->u.s.img,
                         (spr3->w - spr1->w) / 2 + w->x, spr0->h + w->y, surf);
        if ((flags & 0x10) && pos2 > 0.0f)
            d2_BlitPart(spr1->w * 2, (int)(flen - pos2 + 1.0f + 0.5), spr1->w, (int)(pos2 + 1.5), w->u.s.img,
                         (spr3->w - spr1->w) / 2 + w->x, (int)(w->h + w->y - pos2 - spr0->h + 0.5), surf);
        if (pos1 > 0.0f) {
            if (!(flags & 8))
                d2_BlitPart(0, (int)(d + 1.0f + 0.5), spr1->w, (int)(pos1 + 1.5), w->u.s.img,
                             (spr3->w - spr1->w) / 2 + w->x, (int)(w->h + w->y - pos1 - spr0->h + 0.5), surf);
            else
                d2_BlitPart(spr1->w, 0, spr1->w, (int)(pos1 + 1.5), w->u.s.img,
                             (spr3->w - spr1->w) / 2 + w->x, (int)(w->h + w->y - pos1 - spr0->h + 0.5), surf);
        }
        if (!(flags & 0x20)) {
            if (d2_PointerObj == w->id) {
                if (w->buf && w->buf->loaded)
                    my = d2_PointerY;
                else
                    my = d2_MouseY;
                if (my >= w->y && my < spr6->h + w->y)
                    hilo = 1;
                else if (my >= w->h - spr6->h + w->y && my < w->h + w->y)
                    hihi = 1;
            }
            w->hilo = hilo;
            w->hihi = hihi;
            d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y, surf, sprites, 0);
            if (!d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y + (len + spr0->h), surf, sprites, 5))
                d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y + (len + spr0->h), surf, sprites, 0);
            if (hilo)
                d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y, surf, sprites, 6);
            if (hihi)
                d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y + (len + spr0->h), surf, sprites, 7);
        }
        if (!(flags & 8) && !(flags & 0x40))
            d2_DrawShape(w->x, (int)(w->h - spr0->h + w->y - pos1 - spr3->h / 2), surf, sprites, 3);
        if (flags & 4) {
            if (w->dirty && !(flags & 8)) {
                _itoa(value, buf2, 10);
                d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
                d2_SetFont(d2_DefaultFont + 1);
                d2_DrawText(w->x - 0x18, (int)(w->h + w->y - pos1), surf, buf2, 0);
                d2_AddDirty(w->x - 0x18, (int)(w->h + w->y - pos1), d2_TextDrawnW, d2_TextDrawnH);
            }
        }
    }
    w->dirty = 0;
}
/* 0x46bc70, sets the widget's label (label, 0x3f bytes) */

void d2_SetLabel(int id, char *s)
{
    strncpy(d2_Objs[id].label, s, 0x3f);
}
/* 0x46bca0, sets the widget's item list: the n strings joined as "a|b|c|" */

void d2_SetList(int id, int n, ...)
{
    D2OBJ *w;
    va_list ap;
    int i;

    w = &d2_Objs[id];
    w->list[0] = 0;
    va_start(ap, n);
    for (i = 0; i < n; i++) {
        strcat(w->list, va_arg(ap, char *));
        strcat(w->list, "|");
    }
    va_end(ap);
}
/* 0x46bd30, copies item idx of the widget's list to out */

void d2_ListCopyItem(int id, int idx, char *out)
{
    char buf[1024];
    char *p, *q;
    int i;

    i = 0;
    strcpy(buf, d2_Objs[id].list);
    p = buf;
    q = strchr(buf + 1, '|');
    for (; i < idx; i++) {
        if (!q)
            return;
        p = q;
        q = strchr(q + 1, '|');
    }
    if (q) {
        *q = 0;
        strcpy(out, p);
    }
}
/* 0x46bdd0, sets the slider's flags (u.s.flags) */

void d2_SetSliderFlags(int id, int flags)
{
    d2_Objs[id].u.s.flags = flags;
}
/* 0x46be00, creates a slider widget */

int d2_CreateSlider(int x, int y, int kind, int unk)
{
    int id;

    id = d2_NewObject();
    d2_Objs[id].type = 9;
    d2_Objs[id].interactive = 1;
    d2_Objs[id].style = 3;
    d2_Objs[id].x = x;
    d2_Objs[id].h = d2_Pics[kind].h;
    d2_Objs[id].y = y;
    d2_Objs[id].pic = kind;
    d2_Objs[id].clipy0 = 0;
    d2_Objs[id].clipy1 = d2_ScreenH;
    d2_Objs[id].clipx0 = 0;
    d2_Objs[id].clipx1 = d2_ScreenW;
    d2_Objs[id].font = d2_DefaultFont;
    d2_Objs[id].data = d2_GetPic(kind);
    d2_SetCaption(id, (char *)unk);
    d2_AddRefcount(kind);
    return id;
}
/* 0x46bf10, creates a slider in a dialog (position relative to the dialog) */

int d2_WinCreateSlider(int x, int y, int kind, int unk, int dlg)
{
    int id;

    id = d2_CreateSlider(d2_Wins[dlg].x + x, d2_Wins[dlg].y + y, kind, unk);
    d2_AddToWin(id, dlg);
    return id;
}
/* 0x46bf60, draws a button: left cap, right cap, the middle tiled, then the caption @deep B draft: 222 instructions, 16 differ (26 code hunks) - register allocation / member load order, no meaning error. (1) 0x46c039..0x46c059 len = w->w - r->w - l->w: original subtracts l->w first (ecx = [esp+0x40] = l, ebx = r), ours r first; source form w->w - l->w - r->w compiles to identical bytes (VC6 reassociates). (2) 0x46c091 / 0x46c0a3 w->w - r->w + w->x: original loads w->w (ecx) then w->x (edx), ours w->x then w->w; the commuted spelling w->x + w->w - r->w compiles to identical bytes. spell.py: one 'reload' prescription at 0x46c091 (drop the local) - the reload is already equal on both sides (asmdiff shows the same mov eax,[esp+0x48]), so it does not touch the differing loads. Recorded attempts: ~21 variants on this function (dead stores, dead member loads into reused locals, d2_Objs[id]. form, frame pointer array, casts via 0x47c6f0, renaming l/r, declaration order) and ~60 on the siblings, plus this session's two source orders - all no change. Readability done (d2_DrawButton; D2OBJ.checked/clickDisabled/greyed/textShadow/rightAligned; callees named). */

void d2_DrawButton(int id, D2SURF *surf)
{
    D2OBJ *w;
    D2FRAME *l, *r, *m;
    unsigned int style;
    void *sprites;
    int len, i;
    int frame = 0;
    int color = 8;

    w = &d2_Objs[id];
    style = w->style;
    if ((style & 2) && !d2_FocusedInput) {
        if ((&d2_Objs[d2_PointerObj] == w || w->checked) && !w->clickDisabled) {
            if (style & 1)
                w->highlighted = 1;
        } else if (style & 1)
            w->highlighted = 0;
    }
    if (!(style & 2) && w->checkable && !w->clickDisabled)
        w->highlighted = w->checked;
    if (w->highlighted)
        color = 10;
    else if (w->greyed)
        color = 9;
    d2_SetFont(w->font);
    sprites = w->data;
    l = d2_GetFrame(sprites, 0);
    r = d2_GetFrame(sprites, 1);
    m = d2_GetFrame(sprites, 2);
    len = w->w - r->w - l->w;
    if (w->highlighted)
        frame = 3;
    d2_DrawShape(w->x, w->y, surf, sprites, frame);
    d2_DrawShape(w->w - r->w + w->x, w->y, surf, sprites, frame + 1);
    d2_SetClipRect(w->x + l->w - 1, d2_ClipTop, w->x + l->w + len, d2_ClipBottom);
    for (i = 0; i < len / m->w + 0.5; i++)
        d2_DrawShape(m->w * i + w->x + l->w, w->y, surf, sprites, frame + 2);
    d2_DrawTextBox(w->x, (l->h >> 1) - d2_FontHeight / 2 + 1 + w->y, w->w, w->h, surf, w->u.text, color);
}
/* 0x46c1d0, centres an object (flag centred) horizontally on its window or on the screen @deep draft (59/144 bytes; 67 instructions both sides): after the dup-tail pass only register allocation and epilogue placement differ - ours keeps obj in edi and win in ebx where the original has obj in ebx and win in edi (one bijective swap), the entry pushes are esi,edi vs ebx,esi,edi, and our two arms share pop edi/pop esi/ret where the original duplicates the whole tail; semantics read equal against 0x46c1d0 and its Ghidra pseudocode. spell.py dup-tail tried: statements in both arms (116 bytes differ, worse prologue), pull after the branch (best, 59), swap branch polarity (86); also tried d2_Objs[obj] subscripts instead of the local pointer, a named D2WIN *win local, the early-return !centred form and an a?b:c argument (all the same 59). Readability passes Q1..Q7. Left: VC6 register allocation and the tail merge - listing/c2prio/siblings. */

void d2_CenterObj(int obj)
{
    D2OBJ *o = &d2_Objs[obj];
    int x;

    if (o->centred) {
        if (o->win)
            x = o->win->w / 2 - o->w / 2 + o->win->x;
        else
            x = d2_ScreenW / 2 - o->w / 2;
        d2_MoveObjI(x, o->y, obj);
    }
}
/* 0x46c260, sets the width of an object and centres it again */

void d2_SetObjWidth(int obj, short w)
{
    d2_Objs[obj].w = w;
    d2_CenterObj(obj);
}
/* 0x46c290, opens an object with w x h 16-bit pixels of its own, copied from src if given */

int d2_OpenRaw(int x, int y, int w, int h, void *src)
{
    int obj;
    int n;

    obj = d2_NewObject();
    d2_Objs[obj].type = 0x47;
    d2_Objs[obj].x = x;
    d2_Objs[obj].y = y;
    d2_Objs[obj].h = h;
    d2_Objs[obj].w = w;
    d2_Objs[obj].pic = -1;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    n = w * h * 2;
    d2_Objs[obj].u.raw = mem_Alloc(n, "d2:raw");
    if (src)
        memcpy(d2_Objs[obj].u.raw, src, n);
    return obj;
}
/* 0x46c390, the same on a window */

int d2_WinOpenRaw(int x, int y, int w, int h, void *src, int win)
{
    int obj;

    obj = d2_OpenRaw(d2_Wins[win].x + x, d2_Wins[win].y + y, w, h, src);
    d2_AddToWin(obj, win);
    return obj;
}
/* 0x46c3f0, copies the pixels of a raw object to the surface (clipped at the top and bottom) */

void d2_DrawRaw(int obj, D2SURF *surf)
{
    D2OBJ *o = &d2_Objs[obj];
    int i;
    int top = 0;
    int h = o->h;

    if (o->y + h > o->clipy1)
        h = o->clipy1 - o->y;
    else if (o->y < o->clipy0)
        top = o->clipy0 - o->y;
    for (i = top; i < h; i++)
        memcpy(surf->bits + (o->y + i) * surf->pitch + o->x, o->u.raw + o->w * i, o->w * 2);
}
/* 0x46c490, makes a radio button group of the n objects given; -1 if there is no free group */

int d2_CreateRadioGroup(int n, ...)
{
    va_list ap;
    int i;
    int j;

    for (i = 0; i < 8; i++)
        if (!d2_Radios[i].n)
            break;
    if (i >= 8)
        return -1;
    if (n > 0x20) {
        err_Message("d2_CreateRadioGroup: Too many objecthandles...");
        return -1;
    }
    d2_Radios[i].n = n;
    d2_Radios[i].sel = -1;
    va_start(ap, n);
    for (j = 0; j < n; j++) {
        d2_Radios[i].objs[j] = va_arg(ap, int);
        d2_Objs[d2_Radios[i].objs[j]].checkable = 1;
        d2_Objs[d2_Radios[i].objs[j]].style &= ~2;
    }
    va_end(ap);
    return i;
}
/* 0x46c560, adds an object to a radio button group (the first one gets selected) */

void d2_RadioAdd(int g, int obj)
{
    d2_Radios[g].objs[d2_Radios[g].n] = obj;
    d2_Objs[obj].checkable = 1;
    d2_Objs[obj].style &= ~2;
    d2_Radios[g].n++;
    if (d2_Radios[g].sel == -1)
        d2_RadioSelect(g, 0);
}
/* 0x46c600, frees a radio button group */

void d2_RadioClear(int g)
{
    memset(&d2_Radios[g], 0, sizeof(D2RADIO));
}
/* 0x46c630, selects button sel of a radio button group */

void d2_RadioSelect(int g, int sel)
{
    int i;

    for (i = 0; i < d2_Radios[g].n; i++)
        if (i != sel)
            d2_SetObjValue(d2_Radios[g].objs[i], 0);  /* @original: this call site pushes 2 arguments (0x46c661: push 0; push eax) against the 5-parameter definition */
        else
            d2_SetObjValue(d2_Radios[g].objs[i], 1);  /* @original: this call site pushes 2 arguments (0x46c66a: push 1; push ecx) against the 5-parameter definition */
    d2_Radios[g].sel = sel;
}
/* 0x46c6a0, the selected button of a radio button group */

int d2_RadioGetSel(int g)
{
    return d2_Radios[g].sel;
}
/* 0x46c6c0, keyboard / pointer navigation through a menu (arrow keys, enter) */

void d2_MenuNavigate(menu)
int menu;
{
    int count, cur, sel, i, id;
    D2OBJ *o;

    count = d2_Radios[menu].n;
    if (count <= 0)
        return;
    cur = d2_Radios[menu].sel;
    sel = d2_Radios[menu].sel;
    if (d2_PointerMoved == 0) {
        if (d2_LastPointerX != dm_L.x || d2_LastPointerY != dm_L.y) {
            d2_PointerMoved = 1;
            goto check;
        }
    } else {
check:
        if (d2_FocusedInput == 0) {
            for (i = 0; i < count; i++) {
                if (d2_Radios[menu].objs[i] == d2_PointerObj &&
                    ((d2_Radios[menu].flags & 1) == 0 || dm_L.lup != 0))
                    sel = i;
            }
            if (sel == cur)
                d2_PointerMoved = 0;
        }
    }
    if (kb_Last == 0xc8 || (dm_L.wup != 0 && d2_PointerWinVisible == -1)) {
        sel--;
        d2_PointerMoved = 0;
        d2_LastPointerX = dm_L.x;
        d2_LastPointerY = dm_L.y;
        if (sel < 0)
            sel = count - 1;
        sel = sel % count;
        if (dm_L.wup != 0) {
            o = &d2_Objs[d2_Radios[menu].objs[sel]];
            di_SetMousePos((int)(o->w * 0.5f + o->x), (int)(o->h * 0.5f + o->y));
        }
    } else if (kb_Last == 0xd0 || (dm_L.wdown != 0 && d2_PointerWinVisible == -1)) {
        sel++;
        d2_PointerMoved = 0;
        d2_LastPointerX = dm_L.x;
        d2_LastPointerY = dm_L.y;
        if (sel < 0)
            sel = count - 1;
        sel = sel % count;
        if (dm_L.wdown != 0) {
            o = &d2_Objs[d2_Radios[menu].objs[sel]];
            di_SetMousePos((int)(o->w * 0.5f + o->x), (int)(o->h * 0.5f + o->y));
        }
    }
    if (sel != -1) {
        if (sel < 0)
            sel = d2_Radios[menu].n - 1;
        sel = sel % d2_Radios[menu].n;
        if (sel != d2_Radios[menu].sel || d2_PointerMoved == 0)
            d2_RadioSelect(menu, sel);
        if (kb_Last == 0x1c) {
            dm_L.lclick = 1;
            kb_Last = 0;
            id = d2_Radios[menu].objs[sel];
            d2_PointerObj = id;
            o = &d2_Objs[id];
            if (o->type == 9)
                if ((char)o->style & 0x10)
                    d2_ClickedId = 1;
                else
                    d2_ClickedId = 0;
            else
                d2_ClickedId = o->pic;
        }
    } else {
        if (d2_Radios[menu].sel != -1)
            d2_RadioSelect(menu, -1);
    }
}
/* 0x46c9e0, forwards to d2_RemoveObj (removes the object) */

void d2_DeleteObject(int obj)
{
    d2_RemoveObj(obj);
}
/* 0x46c9f0, takes a free object and puts it into the draw order list */

int d2_NewObject(void)
{
    int n, i;
    D2OBJ *o;

    n = d2_FindFreeObj();
    if (n == -1)
        return -1;
    o = &d2_Objs[n];
    for (i = 0x2ff; i >= 0; i--)
        if (d2_DrawOrder[i]) {
            i++;
            if (i >= 0)
                goto found;
            break;
        }
    i = 0;
found:
    d2_DrawOrder[i] = o;
    if (n > d2_HighestObj)
        d2_HighestObj = n;
    d2_Objs[n].used = 1;
    return n;
}
/* 0x46ca60, one more user of a picture (a frame counts for its base picture) */

void d2_AddRefcount(int pic)
{
    D2PIC *p = &d2_Pics[pic];

    if ((p->type == 5 || p->type == 8) && p->offset == 0)
        pic = p->base;
    d2_Pics[pic].refs++;
    d2_Pics[pic].lastuse = d3_Time;
}
/* 0x46cac0, one user less */

void d2_SubRefcount(int pic)
{
    D2PIC *p = &d2_Pics[pic];
    int *r;

    if ((p->type == 5 || p->type == 8) && p->offset == 0)
        pic = p->base;
    r = &d2_Pics[pic].refs;
    if (*r > 0)
        (*r)--;
    else
        err_Message("d2_SubRefcount: invalid refcount!");
}
/* 0x46cb20, creates an object showing a picture at x, y */

int d2_OpenPic(x, y, pic)
int x;
int y;
int pic;
{
    int n;
    D2OBJ *o;
    void *data = 0;
    int real, p;
    D2PICHDR *set;
    D2PICHDR11 *set17;
    D2FRAME *spr;

    if (pic > d2_NPics)
        return -1;
    n = d2_NewObject();
    o = &d2_Objs[n];
    if (n >= 0x300)
        return -1;
    d2_GetPicEx(pic, &data, &real);
    o->data = data;
    p = real;
    o->pic = p;
    o->type = d2_Pics[p].type;
    o->x = (short)x;
    o->y = (short)y;
    o->clipx0 = (short)d2_ClipLeft;
    o->clipx1 = (short)d2_ClipRight;
    o->clipy0 = (short)d2_ClipTop;
    o->clipy1 = (short)d2_ClipBottom;
    o->drawFrames = 2;
    d2_AddRefcount(p);
    if (o->type == 8)
        o->interactive = 1;
    o->icon = -1;
    o->iconx = 0;
    o->icony = 0;
    switch (o->type) {
    case 1:
        set = (D2PICHDR *)d2_Objs[n].data;
        o->w = set->w;
        o->h = set->h;
        return n;
    case 4:
        d2_Objs[n].input = d2_StartAnim(x, y, d2_Objs[n].data, 0);
        set = (D2PICHDR *)d2_Objs[n].data;
        d2_Objs[n].w = set->w;
        d2_Objs[n].h = set->h;
        return n;
    case 0x11:
        set17 = (D2PICHDR11 *)d2_Objs[n].data;
        d2_Objs[n].w = set17->w;
        d2_Objs[n].h = set17->h;
        d2_Objs[n].input = 2;
        return n;
    case 5:
    case 8:
        o->input = d2_PicShift;
        set = (D2PICHDR *)d2_Objs[n].data;
        spr = d2_GetFrame(set, (short)d2_Objs[n].input);
        o->w = spr->w;
        o->h = spr->h;
        o->captionX = -1;
        o->captionY = -1;
        o->picW = set->w;
        o->picH = set->h;
        d2_Objs[n].shadowX = 8;
        d2_Objs[n].shadowY = 8;
        break;
    }
    return n;
}
/* 0x46cd80, thunk */

int d2_CreatePicObj(x, y, pic)
short x;
short y;
int pic;
{
    return d2_OpenPicS(x, y, pic);
}
/* 0x46cfd0, redraws a picture and flips */

void d2_ShowPicture(int pic)
{
    d2_DrawPic(0, 0, pic, d2_ScreenSurf);
    d2_BlitPart(0, 0, ((D2SURF *)d2_ScreenSurf)->w, ((D2SURF *)d2_ScreenSurf)->h, d2_ScreenSurf, 0, 0, d2_BackSurf);
    d2_DrawFades();
    dd_Flip();
    d2_BlitPart(0, 0, ((D2SURF *)d2_ScreenSurf)->w, ((D2SURF *)d2_ScreenSurf)->h, d2_BackSurf, 0, 0, d2_ScreenSurf);
}

/* 0x46cda0, moves an object (and, for a window, the objects on it) to x, y and recomputes its clip rectangle */
void d2_MoveObj(short x, short y, int obj);

/* 0x46cda0 */
void d2_MoveObj(short x, short y, int obj)
{
    D2OBJ *o;
    D2WIN *win, *w;
    int type, n, i, id;
    int oldx, oldy;

    type = d2_Objs[obj].type;
    o = &d2_Objs[obj];
    o->x = x;
    o->y = y;
    o->shadowX = 8;
    o->shadowY = 8;
    if (type != 0x40) {
        o->clipy0 = 0;
        o->clipy1 = d2_ScreenH;
        o->clipx0 = 0;
        o->clipx1 = d2_ScreenW;
    } else {
        o->clipy0 = y;
        o->clipy1 = o->h + y;
        o->clipx0 = x;
        o->clipx1 = o->w + x;
    }
    win = o->win;
    if (win) {
        o->clipy0 = max(win->y, o->clipy0);
        o->clipy1 = min(win->h + win->y, o->clipy1);
        o->clipy1 = min(o->clipy1, d2_ScreenH);
        o->clipx0 = max(win->x, o->clipx0);
        o->clipx1 = min(win->w + win->x, o->clipx1);
        o->clipx1 = min(o->clipx1, d2_ScreenW);
    }
    switch (type) {
    case 0x41:
        d2_Inputs[o->input].x = x;
        d2_Inputs[o->input].y = o->y;
        break;
    case 0x40:
        n = o->input;
        w = &d2_Wins[n];
        oldx = w->x;
        oldy = w->y;
        w->y = y;
        w->x = x;
        if (o->clipx1 > d2_ScreenW)
            o->clipx1 = d2_ScreenW;
        if (o->clipy1 > d2_ScreenH)
            o->clipy1 = d2_ScreenH;
        for (i = 0; i < w->nobjs; i++) {
            id = w->objs[i];
            d2_MoveObj(d2_Objs[id].x - oldx + w->x, d2_Objs[id].y - oldy + w->y, id);
        }
        break;
    case 4:
        d2_AnimView[o->input].x = x;
        d2_AnimView[o->input].y = o->y;
        break;
    }
}
/* 0x46d040, draws a picture (or a frame of one) at x, y on a surface @deep draft (25 differing instructions of 159; 13 code + 12 regs + 4 order hunks): the code hunks are the x/y register swap read in the previous base-3 session (0x46d157: ours mov esi,[esp+0x18] + mov edi,[esp+0x14], original esi=[esp+0x14] + edi=[esp+0x18], and the movsx/push at 0x46d17a/0x46d18c follow the swap); no meaning error, the source passes (short)x, (short)y, surf, data in that order. spell.py: reload at 0x46d157 (drop the local / use the parameter itself) is already the source form - x and y are the parameters, re-read at the call, no local holds them - and the regs/code entries name no source form. Q2 fails only on d3io_NoopCallback, an empty no-op triangle filler with no evidence at all (assigned to d3_TriFuncs[0..5] with different signatures, @deep in d3_io_a.c/d3_engine.c), so no name can be given here. Left: the x/y allocation and the tail at 0x46d208 (listing/c2prio/siblings) plus that name. */

void d2_DrawPic(x, y, pic, surf)
int x;
int y;
int pic;
void *surf;
{
    int sel, type, t;
    void *data = 0;

    if (pic > d2_NPics)
        return;
    sel = -1;
    t = d2_Pics[pic].type;
    if ((t == 5 || t == 8) && d2_Pics[pic].offset == 0) {
        if (d2_Pics[d2_Pics[pic].base].data == 0)
            data = d2_GetPic(pic);
        sel = pic;
        pic = d2_Pics[pic].base;
    } else {
        if (d2_Pics[pic].data == 0)
            data = d2_GetPic(pic);
    }
    if (d2_Pics[pic].flags & 2)
        pic += d2_PicBase;
    if (data == 0)
        data = d2_Pics[pic].data;
    if (sel == -1)
        type = d2_Pics[pic].type;
    else
        type = d2_Pics[sel].type;
    if (d2_LockSurface(surf) == 0)
        return;
#ifdef REA_FRAME_DUMP
    REA_DUMP_TRACE((rd_f, "%u DrawPic %d type=%d at %d,%d %dx%d sel=%d\n", GetTickCount(), pic, type, x, y,
                    d2_Pics[pic].w, d2_Pics[pic].h, sel));
#endif
    switch (type) {
    case 1:
    case 4:
        d2_DrawShape((short)x, (short)y, surf, data, 0);
        break;
    case 0x11:
        ((void (*)(int, int, void *, void *))d3io_NoopCallback)(x, y, surf, data);
        break;
    case 5:
    case 8:
        if (sel != -1)
            d2_DrawShape((short)x, (short)y, surf, data, (char)(sel - pic));
        else
            d2_DrawShape((short)x, (short)y, surf, data, 0);
        break;
    }
    d2_AddDirty((short)x, (short)y, d2_Pics[pic].w, d2_Pics[pic].h);
    d2_UnlockSurface(surf);
}
/* 0x46d220, draws the caption of an object (style bit 4: below it, bit 0x20: with a sprite behind) @deep read whole against the original: the caption call is two calls, 'if (style & 4)' at o->x - 0x30 / o->y + o->h, else at the computed x / y (the original has two d2_DrawTextBox call sites with an epilogue after the first); every flag test, offset and argument matches. Left: allocation - the original keeps flags in the dead o argument slot ('+ mov dword ptr [esp + 0x1c], 8', 'or dword ptr [esp + 0x20], ebx') and x / y in ebp / edi with the 0x20 constant in ebx ('test bl, al'), ours keeps flags in ebp and x in that slot ('- mov ebp, 8', '- add dword ptr [esp + 0x1c], eax'); tried the declaration order (no change) */

void d2_DrawObjCaption(o)
D2OBJ *o;
{
    int flags, x, y, font;

    if (o->caption[0] != 0 && (o->style & 0x40) == 0) {
        flags = 8;
        if (o->greyed)
            flags = 9;
        if (o->rightAligned)
            flags |= 0x10;
        if (o->textShadow)
            flags |= 4;
        if (o->style & 8)
            flags |= 0x20;
        x = o->x;
        if (o->captionX != -1)
            x += o->captionX;
        y = o->y;
        if (o->captionY != -1)
            y += o->captionY;
        else
            y -= 0x10;
        font = o->capfont;
        if (font == 0)
            font = d2_DefaultFont + 1;
        d2_SetFont(font);
        if (o->style & 0x20) {
            d2_UnlockSurface(d2_ScreenSurf);
            d3if_DrawSprite(d2_CaptionTex, o->picW / 2 + o->x, o->picH / 2 + o->y, 0,
                         (float)o->picW / (float)d2_CaptionTex->size,
                         (float)o->picH / (float)d2_CaptionTex->size, 0.5f);
            d2_LockSurface(d2_ScreenSurf);
        }
        if (o->style & 4)
            d2_DrawTextBox(o->x - 0x30, o->y + o->h, o->picW + 0x60, o->h, d2_ScreenSurf, o->caption, flags);
        else
            d2_DrawTextBox(x - 0x30, y, o->picW + 0x60, o->h, d2_ScreenSurf, o->caption, flags);
    }
}
/* 0x46d3d0, draws all objects in drawing order, each clipped to its windows @deep draft (93 differing instructions of 1137 vs 1156; 997/3456 bytes). The reading is register allocation and scheduling only: the mode flags variable sits in edi where the original uses ebp (spell regs at 46db68, the whole 46db68..46dbf5 block), the caption/dirty blocks push the same arguments in another order (46dbfa..46dc7f), and the 46e0d4..46e102 stretch is instruction-stream misalignment (garbage decode on one side) with 46e14c.. alignment fill. spell dup-tail at 46dbb0 (5 instructions, the caption bounds) tried as if/else instead of the ternaries tx = captionX != -1 ? ... : ...: identical bytes, reverted; pull-after-branch and polarity swap do not apply (the arms compute different expressions, no common statement). code at 46e0fc: no menu rule, the bytes are a misaligned decode. Q1 fixed (renamed d2_Unk46d3d0 this session); Q2 blocked: d3io_NoopCallback (empty no-op, no evidence, @deep in d3_io_a.c), d2_NoBackgroundRestore (char flag only compared to 1, no writer in our source), d2_SetSurfaceClip (defined in gfx.c 0x479cd0, its prototype would have to move unit), D2OBJ/D2WIN unk00/unk58/unk6e (no evidence in this function's reads); fillFraction named this session. Left: the ebp/edi allocation and the push scheduling (listing/c2prio/siblings). */

void d2_DrawObjects(void)
{
    D2OBJ *o, *p;
    D2WIN *win;
    D2PICHDR *pic;
    void *frame;
    D2FRAME *part;
    unsigned int fx, fy;
    int i, x, y, x0, x1, y0, y1, yy, mode, font, tx, ty, tw, th;
    char buf[64];

    i = 0;
    d2_PendingCaptionWin = 0;
    d2_PendingCaptionObj = 0;
    d2_StepAnims();
    for (; d2_DrawOrder[i] && i < 0x2fe; i++) {
        o = d2_DrawOrder[i];
        x = o->x;
        y = o->y;
        if ((!o->buf || !o->buf->loaded) && o->used && !o->disabled) {
            x0 = o->clipx0;
            x1 = o->clipx1;
            y0 = o->clipy0;
            y1 = o->clipy1;
            for (win = o->win; win; win = p->win) {
                fx = 0;
                fy = 0;
                if ((win->flags & 4) && !o->unk6e) {
                    frame = d2_GetPic(win->frame);
                    part = d2_GetFrame(frame, 4);
                    fy = part->h;
                    part = d2_GetFrame(frame, 6);
                    fx = part->w;
                }
                p = &d2_Objs[win->parent];
                if (x0 <= (int)(p->clipx0 + fx))
                    x0 = p->clipx0 + fx;
                if (x1 >= (int)(p->clipx1 - fx))
                    x1 = p->clipx1 - fx;
                if (y0 <= (int)(p->clipy0 + fy))
                    y0 = p->clipy0 + fy;
                if (y1 >= (int)(p->clipy1 - fy))
                    y1 = p->clipy1 - fy;
            }
            if (d2_PendingCaptionObj && (o->win != d2_PendingCaptionWin || !d2_PendingCaptionObj->win)) {
                d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
                d2_DrawObjCaption(d2_PendingCaptionObj);
                d2_PendingCaptionWin = o->win;
                d2_PendingCaptionObj = 0;
            }
            d2_SetClipRect(x0, y0, x1, y1);
            switch (o->type) {
            case 1:
                if (o->icon != -1) {
                    frame = d2_GetPic(o->icon);
                    d2_DrawShape(o->x + o->iconx, o->y + o->icony, d2_ScreenSurf, frame, (char)d2_PicShift);
                }
                if (o->style & 2) {
                    if ((&d2_Objs[d2_PointerObj] == o || o->checked) && !o->clickDisabled) {
                        if (o->style & 1)
                            o->highlighted = 1;
                    } else if (o->style & 1)
                        o->highlighted = 0;
                }
                if (o->picScale != 0 && o->picScale != 1 && o->picScale != -1) {
                    if (o->highlighted)
                        d2_DrawShapeScaled(x, y, d2_ScreenSurf, o->data, 0, (char)o->picScale, 1);
                    else
                        d2_DrawShapeScaled(x, y, d2_ScreenSurf, o->data, 0, (char)o->picScale, 0);
                } else {
                    if (o->textShadow)
                        d2_DrawShapeMode2(x + o->shadowX, y + o->shadowY, d2_ScreenSurf, o->data, 0);
                    if (!o->unk58)
                        d2_DrawShape(x, y, d2_ScreenSurf, o->data, 0);
                    if (o->highlighted)
                        d2_DrawShapeMode3(x, y, d2_ScreenSurf, o->data, 0);
                    else if (o->unk58)
                        d2_DrawShapeHalfDark(x, y, d2_ScreenSurf, o->data, 0);
                    else if (o->greyed)
                        d2_DrawShapeMode2(x, y, d2_ScreenSurf, o->data, 0);
                }
                if (!o->win)
                    d2_AddDirty(x, y, d2_ShpW, d2_ShpH);
                break;
            case 0x11:
                if (o->drawFrames > 0) {
                    ((void (*)(int, int, void *, void *))d3io_NoopCallback)(x, y, d2_ScreenSurf, o->data);
                    if (o->drawFrames == 2) {
                        if (d2_NoBackgroundRestore == 1)
                            break;
                        ((void (*)(int, int, void *, void *))d3io_NoopCallback)(x, y, d2_BackSurf, o->data);
                    }
                    if (d2_NoBackgroundRestore != 1)
                        o->drawFrames--;
                }
                break;
            case 4:
                d2_DrawAnim(o->input, d2_ScreenSurf);
                if (!o->win) {
                    if (d2_AnimView[o->input].unk00)
                        d2_AddDirty(x, y, o->w + 8, o->h + 8);
                    else
                        d2_AddDirty(x, y, o->w, o->h);
                }
                break;
            case 5:
            case 8:
                if (o->icon != -1) {
                    frame = d2_GetPic(o->icon);
                    d2_DrawShape(o->x + o->iconx, o->y + o->icony, d2_ScreenSurf, frame, (char)d2_PicShift);
                }
                if (&d2_Objs[d2_PointerObj] != o && !o->checked) {
                    if (o->style & 1)
                        o->highlighted = 0;
                } else if (o->style & 1)
                    o->highlighted = 1;
                if (o->picScale != 0 && o->picScale != 1 && o->picScale != -1) {
                    if (o->highlighted)
                        d2_DrawShapeScaled(x, y, d2_ScreenSurf, o->data, (char)o->input, (char)o->picScale, 1);
                    else
                        d2_DrawShapeScaled(x, y, d2_ScreenSurf, o->data, (char)o->input, (char)o->picScale, 0);
                } else {
                    if (o->textShadow)
                        d2_DrawShapeMode2(x + o->shadowX, y + o->shadowY, d2_ScreenSurf, o->data, (char)o->input);
                    if (!o->unk58)
                        d2_DrawShape(x, y, d2_ScreenSurf, o->data, (char)o->input);
                    if (o->highlighted) {
                        if (o->style & 0x80)
                            d2_DrawShape(x, y, d2_ScreenSurf, o->data, (char)(o->input + 1));
                        else
                            d2_DrawShapeMode3(x, y, d2_ScreenSurf, o->data, (char)o->input);
                    } else if (o->unk58)
                        d2_DrawShapeHalfDark(x, y, d2_ScreenSurf, o->data, (char)o->input);
                    else if (o->greyed) {
                        if (o->fillFraction == 0.0f)
                            d2_DrawShapeMode2(x, y, d2_ScreenSurf, o->data, (char)o->input);
                    }
                    if (o->fillFraction != 0.0f) {
                        pic = o->data;
                        yy = (int)((1.0f - o->fillFraction) * o->h + y);
                        if (y1 < yy)
                            yy = y1;
                        d2_SetClipRect(x0, y0, x1, yy);
                        d2_DrawShapeMode2(x, y, d2_ScreenSurf, pic, (char)o->input);
                        d2_SetClipRect(x0, y0, x1, y1);
                        if (d2_PointerObj != o->id) {
                            sprintf(buf, "%i%%", (int)((o->fillFraction > 1.0 ? 1.0 : o->fillFraction) * 100.0));
                            d2_SetFont(d2_DefaultFont + 1);
                            d2_DrawTextBox(o->x, o->y + o->h - 15, pic->w, o->h, d2_ScreenSurf, buf, 0x28);
                        }
                    }
                }
                if (o->u.text[0]) {
                    if (o->font)
                        d2_SetFont(o->font);
                    if (p)
                        d2_SetClipRect(x0, y0, x1, y1);
                    else
                        d2_SetClipRect(0, 0, 800, 600);
                    mode = 0;
                    if (o->highlighted)
                        mode = 2;
                    else if (o->greyed)
                        mode = 1;
                    if (o->centred)
                        mode |= 8;
                    if (o->rightAligned)
                        mode |= 0x10;
                    d2_GetObjPicSize(o->id, &tw, &th);
                    tx = o->captionX != -1 ? o->x + o->captionX : o->x + tw + 6;
                    ty = o->captionY != -1 ? o->y + o->captionY : o->y - 1;
                    d2_DrawTextBox(tx, ty, o->w, o->h, d2_ScreenSurf, o->u.text, mode);
                }
                if (o->caption[0] && &d2_Objs[d2_PointerObj] == o && !d2_FocusedInput && !(o->style & 0x40)) {
                    d2_PendingCaptionObj = o;
                    d2_PendingCaptionWin = o->win;
                }
                if (!o->win) {
                    if (o->textShadow)
                        d2_AddDirty(x, y, d2_ShpW + 8, d2_ShpH + 8);
                    else
                        d2_AddDirty(x, y, d2_ShpW, d2_ShpH);
                }
                break;
            case 0x40:
                if (o->drawFrames > 0) {
                    d2_SetSurfaceClip(x0, y0, x1 - x0, y1 - y0, d2_ScreenSurf);
                    d2_DrawWindow(o->input, d2_ScreenSurf);
                    if ((d2_Wins[o->input].flags & 0x1000) && d2_Wins[o->input].drawFrames == 1)
                        d2_DrawWindow(o->input, d2_BackSurf);
                    if (!o->win) {
                        if (d2_Wins[o->input].flags & 0x8000)
                            d2_AddDirty(o->x - 2, o->y - 2, o->w + 4, o->h + 4);
                        else
                            d2_AddDirty(o->x, o->y, o->w, o->h);
                    }
                    d2_SetSurfaceClip(0, 0, d2_ScreenW, d2_ScreenH, d2_ScreenSurf);
                }
                break;
            case 0x41:
                d2_DrawInput(o->input, d2_ScreenSurf);
                break;
            case 0x43:
                if (o->drawFrames > 0 && !o->hidden) {
                    if (o->style & 2) {
                        if ((&d2_Objs[d2_PointerObj] == o || o->checked)) {
                            if (o->style & 1)
                                o->highlighted = 1;
                        } else if (o->style & 1)
                            o->highlighted = 0;
                    }
                    if (o->drawFrames == 2)
                        o->drawFrames = 1;
                    font = d2_CurrentFont;
                    d2_SetFont(o->font);
                    mode = 0;
                    if (o->highlighted)
                        mode = 2;
                    else if (o->greyed)
                        mode = 1;
                    if (o->centred)
                        mode |= 8;
                    if (o->rightAligned)
                        mode |= 0x10;
                    if (o->textShadow)
                        mode |= 4;
                    if (o->u.text[0]) {
                        d2_SetTextColour(o->u.text[1], o->u.text[2], o->u.text[3]);
                        d2_DrawTextBox(o->x, o->y, o->w, o->h, d2_ScreenSurf, (char *)o->input, mode | 0x40);
                    } else
                        d2_DrawTextBox(o->x, o->y, o->w, o->h, d2_ScreenSurf, (char *)o->input, mode);
                    o->h = (short)d2_TextDrawnH;
                    if (!o->win)
                        d2_AddDirty(o->x, o->y, o->w, o->h);
                    d2_SetFont(font);
                }
                break;
            case 0x44:
                d2_DissolveStep(o->data);
                break;
            case 0x45:
                if (o->win)
                    d2_SetSurfaceClip(o->clipx0, o->clipy0, o->win->w, o->win->h, d2_ScreenSurf);
                d2_DrawSlider(o->id, d2_ScreenSurf);
                if (!o->win)
                    d2_AddDirty(o->x, o->y - 4, o->w, o->h + 8);
                d2_SetSurfaceClip(0, 0, d2_ScreenW, d2_ScreenH, d2_ScreenSurf);
                break;
            case 9:
                d2_DrawButton(o->id, d2_ScreenSurf);
                if (!o->win)
                    d2_AddDirty(o->x, o->y, o->w, o->h);
                break;
            case 0x47:
                o->clipy0 = (short)y0;
                o->clipy1 = (short)y1;
                d2_DrawRaw(o->id, d2_ScreenSurf);
                if (!o->win)
                    d2_AddDirty(o->x, o->y, o->w, o->h);
                break;
            }
        }
        d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
    }
    if (d2_PendingCaptionObj) {
        d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
        d2_DrawObjCaption(d2_PendingCaptionObj);
        d2_PendingCaptionWin = 0;
        d2_PendingCaptionObj = 0;
    }
    if (d2_FocusedInput && (d2_FocusedInput->flags & 0x80) && d2_FocusedInput->value != -1 && d2_FocusedInput->maxval > d2_FocusedInput->minval
        && dm_L.lhold)
        d2_DrawFrame(d2_FocusedInput, d2_ScreenSurf);
    d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
    d2_FontPic = d2_GetPic(d2_FindPic("_FONT"));
    d2_FontHeight = 0x18;
    d2_FrameCounter++;
}
/* 0x46e150, draws all objects and shows the frame */

void d2_DrawAll(void)
{
    d2_FlushDirty();
    if (d2_LockSurface(d2_ScreenSurf)) {
        d2_DrawObjects();
        d2_UnlockSurface(d2_ScreenSurf);
        dd_Flip();
    }
}
/* 0x46e190, window (type 0x40 object) under the point, -1 if there is none */

int d2_FindWindowAt(short x, short y)
{
    int i, busy;
    D2OBJ *o;

    d2_PointerWin = -1;
    for (i = 0x2ff; i >= 0; i--) {
        busy = 0;
        o = d2_DrawOrder[i];
        if (o) {
            if (o->buf)
                busy = o->buf->loaded;
            if (x >= o->x && x <= o->x + o->w && y >= o->y && y <= o->y + o->h && !busy
                && y >= o->clipy0 && y <= o->clipy1 && !o->clickDisabled && !o->disabled && d2_Objs[o->id].type == 0x40) {
                d2_PointerWin = o->input;
                break;
            }
        }
    }
    return d2_PointerWin;
}
/* 0x46e260, hit test through the textures of the 3D views: the object under the point on a texture */

int d2_HitTestViews(int x, int y)
{
    D2VIEW *v;
    D2TEX *t;
    D2WIN *win;
    D2OBJ *o;
    char *s;
    float u, w;
    int n, j, k, i, px;

    d2_PointerX = 0;
    d2_PointerY = 0;
    d2_PointerTexName[0] = 0;
    for (n = 0; n < 0x30; n++) {
        v = &d2_Views[n];
        if (v->active && v->loaded && v->shown) {
            for (j = 0; j < 2; j++) {
                if (j == 0)
                    s = v->tex1;
                else if (j == 1)
                    s = v->tex2;
                else
                    continue;
                if (*s && (t = tex_Find(s)) != NULL && d3_ScreenToUV((float)x, (float)y + 5.0f, 0, t, &u, &w)) {
                    px = (int)(u * t->size);
                    y = (int)(w * t->size);
                    if (j == 1)
                        y += 0x100;
                    d2_PointerX = px;
                    d2_PointerY = y;
                    strcpy(d2_PointerTexName, (char *)t);
                    for (k = v->nwins - 1; k >= 0; k--) {
                        win = &d2_Wins[v->wins[k]];
                        for (i = 0; i < win->nobjs; i++) {
                            o = &d2_Objs[win->objs[i]];
                            if (px >= o->x && px <= o->x + o->w && y >= o->y && y <= o->y + o->h) {
                                if (o->type != 0x40) {
                                    d2_PointerObj = win->objs[i];
                                    if (o->win)
                                        d2_PointerWin = o->win->id;
                                    return o->pic;
                                }
                                d2_PointerWin = o->input;
                            }
                        }
                    }
                    d2_PointerObj = -1;
                    return -1;
                }
            }
        }
    }
    return -1;
}
/* 0x46e460, the object under the pointer: sets d2_PointerObj / 714 / 72c / 730, returns its unk08 */

int d2_HitTest(short x, short y)
{
    int i, busy, ret;
    int x0, x1, y0, y1;
    D2OBJ *o, *p;
    D2WIN *win;

    d2_FindWindowAt(x, y);
    d2_PointerWinVisible = -1;
    d2_HitObjId = -1;
    for (i = 0x2ff; i >= 0; i--) {
        busy = 0;
        o = d2_DrawOrder[i];
        if (o) {
            if (o->buf)
                busy = o->buf->loaded;
            if (x >= o->x && x <= o->x + o->w && y >= o->y && y <= o->y + o->h && !busy
                && y >= o->clipy0 && y <= o->clipy1 && !o->clickDisabled && !o->disabled && d2_Objs[o->id].type == 0x40
                && d2_Wins[o->input].visible) {
                d2_PointerWinVisible = o->input;
                break;
            }
        }
    }
    for (i = 0x2ff; i >= 0; i--) {
        o = d2_DrawOrder[i];
        busy = 0;
        if (o) {
            if (o->buf)
                busy = o->buf->loaded;
            if (x >= o->x && x <= o->x + o->w && y >= o->y && y <= o->y + o->h && !busy) {
                win = o->win;
                if (win) {
                    p = &d2_Objs[win->parent];
                    x0 = win->x > p->clipx0 ? win->x : p->clipx0;
                    x1 = win->x + win->w < p->clipx1 ? win->x + win->w : p->clipx1;
                    y0 = win->y > p->clipy0 ? win->y : p->clipy0;
                    y1 = win->y + win->h < p->clipy1 ? win->y + win->h : p->clipy1;
                } else {
                    x0 = o->clipx0;
                    x1 = o->clipx1;
                    y0 = o->clipy0;
                    y1 = o->clipy1;
                }
                if (y >= y0 && y <= y1 && x >= x0 && x <= x1 && !o->disabled
                    && (o->interactive || o->checkable || o->type == 0x41 || o->hitTestable)) {
                    if (!o->clickDisabled) {
                        d2_HitObjId = d2_PointerObj = o->id;
                        if (o->win)
                            d2_PointerWin = o->win->id;
                        if (o->type == 9)
                        {
                            if ((char)o->style & 0x10)
                                return 1;
                            return 0;
                        }
                        return o->pic;
                    }
                    if (d2_HitObjId == -1)
                        d2_HitObjId = o->id;
                    d2_PointerObj = -1;
                }
            }
        }
    }
    ret = d2_HitTestViews(x, y);
    if (ret == -1)
        ret = d2_PointerObj = -1;
    return ret;
}
/* 0x46e710, removes object obj: frees what its type owns, unlinks it from its window and the draw list,
   marks its rectangle dirty and clears the entry
   @deep draft (105 differing instructions of 260 vs 263; 402/656 bytes). The stream is the same statements under another register map: ours materializes the constant 0 once in ebp and reuses it for the stores (mov [x+0x3b4],ebp / mov [x+0x7c],ebp / mov [x+0xbfc],ebp) and the compares (cmp eax,ebp), the original stores immediates (mov [x+0xbfc],0x0) and keeps its zero in edi (cmp eax,edi, test edi,edi); the object-cursor computation (46e724) and the window object scan (46e816: ours increments a pointer, the original indexes [edx+eax*4-0x4], both the i++/i-- find loop) are the same operations. spell.py reload at 46e710 (drop the off local, re-read obj at the memset and the id write) tried: identical bytes, reverted; dup-tail at 46e7c9 is the same call's argument pushed from another register, no repeated source statement; code at 46e724/46e78f are the address computations, registers only. No meaning error found (the loop bounds, the memmove, d2_HighestObj and the final id write read equal). Readability done (Q1..Q7: sizedObjCount field, typed id write). Left: the constant-0 register choice and the cascade (listing/c2prio/siblings). */

void d2_RemoveObj(int obj)
{
    D2OBJ *o;
    D2WIN *w;
    D2INPUT *in;
    int i, n, off;

    off = obj * sizeof(D2OBJ);
    o = (D2OBJ *)((char *)d2_Objs + off);
    if (!o->used)
        return;
    switch (o->type) {
    case 0x43:
        if (o->input)
            mem_Free(o->input);
        break;
    case 4:
        d2_FreeAnim(o->input);
        break;
    case 0x40:
        d2_CloseWindow(o->input);
        break;
    case 0x41:
        in = &d2_Inputs[o->input];
        if (d2_FocusedInput == in)
            d2_FocusedInput = 0;
        if (in->icon != -1)
            d2_SubRefcount(in->icon);
        memset(in, 0, sizeof(D2INPUT));
        break;
    case 0x45:
        d2_FreeSurface(o->u.s.img);
        break;
    case 9:
        if (o->win)
            o->win->sizedObjCount = 0;
        break;
    case 0x47:
        if (o->u.raw) {
            mem_Free(o->u.raw);
            o->u.raw = 0;
        }
        break;
    }
    if (o->type < 0x40)
        d2_SubRefcount(o->pic);
    w = o->win;
    if (w) {
        n = w->nobjs;
        for (i = 0; i < n;)
            if (w->objs[i++] == obj)
                break;
        i--;
        if (i != n)
            memmove(&w->objs[i], &w->objs[i + 1], (n - i) * 4);
        w->nobjs--;
    }
    if (o->buf) {
        if (o->buf->loaded)
            d2_AddShape(o->x, o->y, o->w, o->h, (D2BUF *)o->buf);
        else
            d2_AddDirty(o->x, o->y, o->w, o->h);
    } else
        d2_AddDirty(o->x, o->y, o->w, o->h);
    for (i = 0; i < 0x2ff; i++) {
        if (d2_DrawOrder[i] == o) {
            if (i == 0x2ff)
                d2_DrawOrder[0x2ff] = 0;
            else
                memmove(&d2_DrawOrder[i], &d2_DrawOrder[i + 1], (0x2fe - i) * 4);
            break;
        }
    }
    if (obj == d2_HighestObj)
        d2_HighestObj = obj - 1;
    memset((char *)d2_Objs + off, 0, sizeof(D2OBJ));
    ((D2OBJ *)((char *)d2_Objs + off))->id = obj;
}
/* 0x46e990, clears the screen (fades drawn), flips, clears both surfaces again */

void d2_ClearScreen(void)
{
    if (d2_LockSurface(d2_ScreenSurf)) {
        d2_ClearSurface(d2_ScreenSurf);
        d2_UnlockSurface(d2_ScreenSurf);
    }
    d2_DrawFades();
    dd_Flip();
    if (d2_LockSurface(d2_ScreenSurf)) {
        d2_ClearSurface(d2_ScreenSurf);
        d2_UnlockSurface(d2_ScreenSurf);
    }
    d2_ClearSurface(d2_BackSurf);
}
/* 0x46ea10, width in pixels of text in the font with index font ('~' is skipped) */

int d2_TextWidth(char *text, int font)
{
    D2PICHDR *f;
    D2FRAME *g;
    int i, w, len;
    unsigned char c;
    w = 0;
    len = strlen(text);
    f = d2_GetPic(font);
    for (i = 0; i < len; i++) {
        c = text[i];
        if (c != '~') {
            g = d2_GetFrame(f, c);
            if (i != 0 && text[i - 1] != ' ')
                w -= g->kern;
            if (c != ' ')
                w += g->adv + d2_LetterGap;
            else
                w += g->adv + d2_SpaceWidth + d2_LetterGap;
        }
    }
    return d2_LetterGap + w;
}
/* 0x46ead0, draws the pieces of text collected in d2_TextSegs in one line at y of window win, starting at x and
   ending at right: justified by the window's flags (0x80: spread, 0x40: right, 0x100: centered), pieces move
   right past the objects of the window; "$T" jumps to a position, "$H" draws with a second pass, "$[" "$]" draw
   glyph 0
   @deep draft (481 differing instructions of 441 vs 438; 1109/1344 bytes). Same statements, another register map: the original keeps the zero constant in edi (xor edi,edi at 46ead7, cmp/test against edi) where ours keeps layout values in ebp, and the whole 46ead7..46f1c0 stream is scheduled differently (the strlen scas loop at 46ee7b, the d2_TextSegCount comparisons at 46ee67 vs 46ee9d). spell.py: reload at 46ee15 (the original re-reads the char locals [esp+0x5c] where ours keeps them in ebp) - the lead asks to drop a local and re-read its expression, but the value is hl/skip set in the '$H'/'/'$M' switch arms, not a re-readable expression; dup-tail at 46ead7 (3 instructions) and 46ee63 (7) are the same calls/compares under the other registers, no repeated statement in the source. No meaning error read in the justification block (gap/extra/k), the object-width walk (start/n/pobj) or the glyph switch. Readability done this session (Q1..Q7: d2_DrawTextLine, d2_TextSegCount, d2_LayoutMode). Left: the register map and the scheduling (listing/c2prio/siblings). */

void d2_DrawTextLine(int a, int y, void *surf, int win, char nojustify, int flags, int x, int right)
{
    D2WIN *w;
    D2OBJ *o;
    D2FRAME *g;
    D2PICHDR *gfont;
    int (*draw)(int, int, void *, void *, unsigned char);
    int gap, extra, saved, total, start, glyph, i, j, k, n, savedflags;
    int *pobj;
    char hl, skip;
    unsigned char c;                /* a glyph byte: the original zero-extends it into d2_GetFrame's index
                                       (0x46ee33 movzx dx,dl / 0x46eef3 xor dx,dx; mov dl,al); a signed
                                       char sign-extended 0xfc to 0xfffc and read 256 KB past the font bank */

    total = 0;
    extra = 0;
    hl = 0;
    gap = d2_SpaceWidth;
    w = &d2_Wins[win];
    savedflags = w->flags;
    if (flags != 0)
        w->flags = flags;
    if (d2_LayoutMode != 1) {
        for (i = 0; i < d2_TextSegCount; i++)
            total += d2_TextSegs[i].w;
        saved = w->flags;
        start = ((int)(char)saved & 8) >> 3;
        if (start < w->nobjs) {
            n = w->nobjs - start;
            pobj = &w->objs[start];
            do {
                o = &d2_Objs[*pobj];
                if (o->hidden == 0 && o->y < d2_FontHeight + y && y < o->y + o->h && flags != 0x40)
                    total += 6 + o->w + d2_LetterGap;
                pobj++;
                n--;
            } while (n);
        }
        if ((char)saved < 0 && d2_TextSegCount > 1 && nojustify == 0) {
            k = right - total - x;
            gap = k / (d2_TextSegCount - 1);
            extra = k % (d2_TextSegCount - 1);
        } else if (!(saved & 0x40)) {
            if (saved & 0x100) {
                k = right - (d2_TextSegCount - 1) * d2_SpaceWidth - total - x;
                x = x + k / 2;
            }
        } else
            x = right - (d2_TextSegCount - 1) * d2_SpaceWidth - total;
        for (j = 0; j < d2_TextSegCount; j++) {
            glyph = -1;
            skip = 0;
            start = ((int)(char)w->flags & 8) >> 3;
            if (d2_TextSegs[j].text[0] == '$') {
                switch (d2_TextSegs[j].text[1]) {
                case 'H':
                    hl = 1;
                case 'M':
                    skip = 1;
                    break;
                case 'T':
                    x = d2_TextSegs[j].w;
                    skip = 1;
                    break;
                case '[':
                case ']':
                    gfont = d2_FontPic;
                    glyph = 0;
                }
            }
            if (!(d2_TextSegs[j].flags & 0x40))
                draw = d2_DrawShape;
            else {
                d2_SetTextColour(d2_TextSegs[j].r, d2_TextSegs[j].g, d2_TextSegs[j].b);
                draw = d2_DrawShapeMode5;
            }
            if (!skip) {
                if (start < w->nobjs) {
                    n = w->nobjs - start;
                    pobj = &w->objs[start];
                    do {
                        o = &d2_Objs[*pobj];
                        if (o->hidden == 0) {
                            while (o->x < d2_TextSegs[j].w + x && x < o->x + o->w + 6
                                   && d2_FontHeight + y > o->y && y < o->y + o->h && flags != 0x40)
                                x = o->x + o->w + 6;
                        }
                        pobj++;
                        n--;
                    } while (n);
                }
                if (glyph == -1) {
                    if (hl) {
                        for (i = 0; i < (int)strlen(d2_TextSegs[j].text); i++) {
                            c = d2_TextSegs[j].text[i];
                            if (c != '~') {
                                g = d2_GetFrame(d2_FontPic, c);
                                if (i != 0)
                                    x -= g->kern;
                                if (d2_DrawShape(x, y, surf, d2_FontPic, c)) {
                                    d2_DrawShapeMode3(x, y, surf, d2_FontPic, c);
                                    x += g->adv + d2_LetterGap;
                                }
                            }
                            if (right < x)
                                break;
                        }
                        hl = 0;
                    } else {
                        for (i = 0; i < (int)strlen(d2_TextSegs[j].text); i++) {
                            c = d2_TextSegs[j].text[i];
                            if (c != '~') {
                                g = d2_GetFrame(d2_FontPic, c);
                                if (i != 0)
                                    x -= g->kern;
                                if (draw(x, y, surf, d2_FontPic, c))
                                    x += g->adv + d2_LetterGap;
                            }
                            if (right < x)
                                break;
                        }
                    }
                } else {
                    g = d2_GetFrame(gfont, glyph);
                    if (d2_DrawShape(x, y, surf, gfont, glyph))
                        x += g->adv + d2_LetterGap;
                }
            }
            if (c != '~' && !skip) {
                x += gap;
                if (extra != 0) {
                    x++;
                    extra--;
                }
            }
        }
        d2_TextSegCount = 0;
    }
    w->flags = savedflags;
}
/* 0x46f000, draws text at (x, y) in the font "_FONT"; flags 4: shadow, 2 and 1: extra passes; '~' is skipped;
   stops at the right screen edge; leaves the size drawn in d2_TextDrawnW / d2_TextDrawnH */

void d2_DrawText(int x, int y, void *surf, char *text, unsigned char flags)
{
    D2FRAME *g;
    unsigned int i;
    int x0;
    unsigned char c;

    x0 = x;
    if (d2_FontPic == 0)
        d2_FontPic = d2_GetPic(d2_FindPic("_FONT"));
    d2_FontHeight = d2_FontPic->h;
    for (i = 0; i < strlen(text); i++) {
        c = text[i];
        g = d2_GetFrame(d2_FontPic, c);
        x -= g->kern;
        if (c != '~') {
            if (c != ' ') {
                if (flags & 4)
                    d2_DrawShapeMode2(x + 6, y + 6, surf, d2_FontPic, c);
                d2_DrawShape(x, y, surf, d2_FontPic, c);
                if (flags & 2)
                    d2_DrawShapeMode3(x, y, surf, d2_FontPic, c);
                if (flags & 1)
                    d2_DrawShapeMode2(x, y, surf, d2_FontPic, c);
                x += g->adv + d2_LetterGap;
            } else
                x += d2_SpaceWidth;
        }
        if (x >= d2_ScreenW)
            break;
    }
    d2_TextDrawnH = d2_FontHeight;
    d2_TextDrawnW = x - x0;
}
/* 0x46f180, sets the text colour (a, b, c) of the 5-bit modes */

void d2_SetTextColour(unsigned char a, unsigned char b, unsigned char c)
{
    d2_SetMode5Colour(a, b, c);
}
/* 0x46f1a0, draws text word-wrapped into the box x, y, w, h (flags 8: centered, 0x10: right aligned, 4 / 2 / 1:
   shadow and extra passes, 0x40: colored, 0x20 / 0x80: background sprite, 0x80 keeps the box on the screen,
   0x100: box is given); with surf 0 it only measures. Leaves left edge, top and size in d2_TextDrawnW..74
   @deep far layout draft: 528 vs 522 instructions, 584 differing asm lines. Same statements, another allocation: original frame 0x638, lines[] at [esp+0x48], x/y home slots [esp+0x64c]/[esp+0x650], ebx=y esi=x ebp=0; ours 0x630, lines[] at [esp+0x40], x kept in a register, ebx=x ebp=y esi=0, plus extra spills (mov [esp+0x10],[esp+0x24],[esp+0x28]) and the whole wrap loop scheduled in another order. Read against the original disassembly: prologue (0x46f1a0-46f23d), measure/clip block (0x46f240-46f3c0), sprite blit, word split with the goto fits case, glyph loop and tail - every statement corresponds, no wrong type/signedness/offset/width found. spell.py: rep-stos already satisfied (both sides rep stos 0x180 after lea of lines[]), dup-tail at 46f82b is the shared epilogue (add esp/pops, no source form), cmp-test lead tried: if (ln > 1) -> if (ln >= 2) gives the original cmp reg,2; jl (kept, 586 -> 584). Remaining: VC6 locals layout and spill/register choice (vargen/listing), no meaning-class lead left. Readable Q1..Q7. */

void d2_DrawTextBox(int x, int y, int w, int h, void *surf, char *text, unsigned int flags)
{
    char lines[16][0x60];
    int (*draw)(int, int, void *, void *, unsigned char);
    D2FRAME *g;
    char *sp, *p, *q, *word;
    int right, lw, len, pos, ln, wd, x0, y0, bx, bw, bh, vis, sw, top, yy, k, wl, n, j, i, cx;
    unsigned char c;
    char ch;

    lw = 0;
    x0 = x;
    y0 = y;
    d2_TextDrawnW = 0;
    d2_TextDrawnH = 0;
    d2_TextLeft = 9999;
    d2_TextTop = y;
    if (d2_FontPic == 0)
        d2_FontPic = d2_GetPic(d2_FindPic("_FONT"));
    d2_FontHeight = d2_FontPic->h;
    memset(lines, 0, sizeof(lines));
    if (surf != 0 && (flags & 0xa0)) {
        if (!(flags & 0x100)) {
            d2_DrawTextBox(x, y, w, h, 0, text, flags);
            bx = d2_TextLeft;
            bh = d2_TextDrawnH;
            y = d2_TextTop;
            bw = d2_TextDrawnW - d2_TextLeft + x;
        } else {
            bx = x;
            bh = h;
            bw = w;
        }
        if ((char)flags < 0) {
            sw = d2_ScreenW;
            if (bx + bw > sw) {
                bx = sw - bw;
                d2_TextLeft = sw - d2_TextDrawnW;
                x = d2_TextLeft;
                x0 = x;
            }
            if (d2_ScreenH < y + bh) {
                y = d2_ScreenH - bh;
                d2_TextTop = y;
                y0 = y;
            }
        }
        vis = 1;
        top = d2_ClipTop;
        yy = y;
        if (y + bh < top)
            vis = 0;
        if (y > d2_ClipBottom)
            vis = 0;
        if (y < top) {
            bh = top - y;
            yy = top;
        }
        if (yy + bh > d2_ClipBottom)
            bh = d2_ClipBottom - yy;
        if (vis) {
            d2_UnlockSurface(surf);
            d3if_DrawSprite(d2_CaptionTex, bw / 2 + bx, bh / 2 + yy, 0, (float)bw / (float)d2_CaptionTex->size,
                         (float)bh / (float)d2_CaptionTex->size, 0.7f);
            d2_LockSurface(surf);
        }
        d2_TextDrawnH = bh;
        d2_TextDrawnW = bw;
    }
    pos = 0;
    ln = 0;
    draw = d2_DrawShapeMode5;
    if (!(flags & 0x40))
        draw = d2_DrawShape;
    len = strlen(text);
    right = x + w;
    if (len > 0) {
        do {
            word = lines[ln];
            p = text + pos;
            q = word;
            do {
                ch = *p;
                *q++ = ch;
                p++;
            } while (ch);
            sp = strchr(word, ' ');
            if (sp)
                *sp = 0;
            else
                sp = strchr(word, 0);
            pos = pos + (sp - word) + 1;
            wd = d2_TextWidth(word, d2_CurrentFont);
            x += wd;
            lw += wd;
            if (pos <= len) {
                lw += d2_SpaceWidth;
                x += d2_SpaceWidth;
            }
            ln++;
            if (x >= right || pos >= len || sp[-1] == '~') {
                if (lw > w) {
                    word = lines[ln - 1];
                    wl = strlen(word);
                    for (k = wl; k >= 0; k--) {
                        ch = word[k];
                        if (ch == '~' || ch == '-') {
                            if (ch == '~')
                                word[k] = '-';
                            word[k + 1] = 0;
                            if (d2_TextWidth(word, d2_CurrentFont) + (lw - wd) <= w) {
                                if (k >= 0) {
                                    pos += k - wl;
                                    lw = 0;
                                    for (j = 0; j < ln; j++)
                                        lw += d2_TextWidth(lines[j], d2_CurrentFont) + d2_SpaceWidth;
                                    goto fits;
                                }
                                break;
                            }
                        }
                    }
                    if (ln >= 2) {
                        lw = 0;
                        pos += -1 - wl;
                        ln--;
                        for (j = 0; j < ln; j++)
                            lw += d2_TextWidth(lines[j], d2_CurrentFont) + d2_SpaceWidth;
                        goto fits;
                    }
                } else {
fits:
                    lw -= d2_SpaceWidth;
                }
                if ((flags & 8) && lw <= w)
                    cx = (w - lw) / 2 + x0;
                else {
                    cx = x0;
                    if (flags & 0x10)
                        cx = x0 - lw + w;
                }
                yy = y;
                if (d2_TextLeft >= cx)
                    d2_TextLeft = cx;
                for (n = ln; n > 0; n--) {
                    word = lines[ln - n];
                    for (i = 0; i < (int)strlen(word); i++) {
                        c = word[i];
                        if (c != '~') {
                            g = d2_GetFrame(d2_FontPic, c);
                            if (i != 0)
                                cx -= g->kern;
                            if (c != ' ') {
                                if (surf != 0) {
                                    if (flags & 4)
                                        d2_DrawShapeMode2(cx + 6, yy + 6, surf, d2_FontPic, c);
                                    draw(cx, yy, surf, d2_FontPic, c);
                                    if (flags & 2)
                                        d2_DrawShapeMode3(cx, yy, surf, d2_FontPic, c);
                                    else if (flags & 1)
                                        d2_DrawShapeMode2(cx, yy, surf, d2_FontPic, c);
                                }
                                cx += g->adv + d2_LetterGap;
                            }
                        }
                        if (cx > right + d2_LetterGap) {
                            yy += d2_FontHeight;
                            cx = x0;
                        }
                    }
                    if (d2_TextDrawnW < cx - x0)
                        d2_TextDrawnW = cx - x0;
                    cx += d2_SpaceWidth;
                }
                y += d2_FontHeight;
                lw = 0;
                x = x0;
                ln = 0;
            }
        } while (pos < len);
    }
    if (d2_TextDrawnH <= y - y0)
        d2_TextDrawnH = y - y0;
}
/* 0x46f840, scrolls a window one step (scrollStep pixels) towards its target, follows its scroll bar and moves its
   objects by the change */

void d2_ScrollWindow(int win)
{
    D2WIN *w = &d2_Wins[win];
    D2OBJ *o;
    int i, id, v, f;

    if (w->scrollRemY < 0) {
        w->scrollY -= w->scrollStep;
        w->scrollRemY += w->scrollStep;
        if (w->scrollRemY > 0) {
            w->scrollY += w->scrollRemY;
            w->scrollRemY = 0;
        }
        if (w->scrollY < 0) {
            w->scrollY = 0;
            w->scrollRemY = 0;
        }
    } else if (w->scrollRemY > 0) {
        w->scrollY += w->scrollStep;
        w->scrollRemY -= w->scrollStep;
        if (w->scrollRemY < 0) {
            w->scrollY += w->scrollRemY;
            w->scrollRemY = 0;
        }
        if (w->scrollY > w->contentHeight) {
            w->scrollY = w->contentHeight;
            w->scrollRemY = 0;
        }
    }
    if (w->scrollRemX < 0) {
        w->scrollX -= w->scrollStep;
        w->scrollRemX += w->scrollStep;
        if (w->scrollRemX > 0) {
            w->scrollX += w->scrollRemX;
            w->scrollRemX = 0;
        }
    } else if (w->scrollRemX > 0) {
        w->scrollX += w->scrollStep;
        w->scrollRemX -= w->scrollStep;
    }
    if (w->scrollBar != -1) {
        v = d2_GetObjValue(w->scrollBar);
        if (d2_Objs[w->scrollBar].u.s.flags & 1)
            w->scrollY = (int)((float)(w->contentHeight - w->h) * (100 - v) * 0.01f);
        if (w->contentHeight < w->h)
            d2_EnableObject(w->scrollBar, 0);
        else if (!d2_Objs[w->parent].disabled)
            d2_EnableObject(w->scrollBar, 1);
        w->scrollY = w->scrollY < 0 ? 0 : w->scrollY;
    }
    if (w->scrollY != w->appliedScrollY || w->scrollX != w->appliedScrollX) {
        f = w->flags;
        if ((char)f & 8)
            i = 1;
        else
            i = 0;
        if (f & 0x20)
            i += 2;
        for (; i < w->nobjs; i++) {
            id = w->objs[i];
            o = &d2_Objs[id];
            d2_MoveObj(o->x - w->scrollX + w->appliedScrollX, o->y - w->scrollY + w->appliedScrollY, id);
            if (o->type == 0x11)
                o->input = 2;
        }
    }
    w->appliedScrollY = w->scrollY;
    w->appliedScrollX = w->scrollX;
}
/* 0x46fad0, gives all buttons (type 9) of a window the width of the widest one plus 8 (at least 0x80) and
   centres them */

void d2_LayoutButtons(int win)
{
    D2WIN *w = &d2_Wins[win];
    D2OBJ *o;
    int i, max = 0;

    for (i = 0; i < w->nobjs; i++) {
        o = &d2_Objs[w->objs[i]];
        if (o->type == 9 && o->w > max)
            max = o->w;
    }
    max += 8;
    if (max < 0x80)
        max = 0x80;
    for (i = 0; i < w->nobjs; i++) {
        o = &d2_Objs[w->objs[i]];
        if (o->type == 9) {
            o->centred = 1;
            d2_SetObjWidth(w->objs[i], max);
        }
    }
}
/* 0x46fba0, lays out and draws the text of window win onto surf: words are wrapped (at '~' and '-' too) and
   collected in d2_TextSegs, d2_DrawTextLine draws each line. Commands start with '%' or '$' and an optional
   digit n: A (new line, n lines high), N / Y (paragraph, half line), < > (right / left margin n "M"s),
   T (tab), B Z R L (spread, centered, right, left), F (font n, FF: color n), C (clear), M, [ ], i (picture
   or button), l, m / mt (inputs), t (text of an input); the arguments of i, l, m, t come from w->args
   @deep far layout draft: 1865 vs 1890 instructions, 565 differing asm lines, hunks code 603 / slots 198 / order 22 / regs 14. Same statements, another allocation and schedule: the prologue spills left/right/top/bottom to [ebp-0x8]/[-0x14]/[-0x34]/[-0x68] where ours uses -0x10/-0x14/-0x38/-0x64, the original repeats the four stores inside the style&4 arm, the command switch and the glyph loop run in another order. Read the head (margins + style&4), the command cases (A N Y C m i l t, the F/color path), the object creation, the wrap loop and the tail: every statement corresponds, no wrong type/signedness/offset/width found. spell.py leads: reload x6 (46fdb2 46fdfe 46fe4a 46fe6f 46fec8 46feed - the original re-reads id [ebp-0x8] and align [ebp-0x28] from their slots; all our uses are already the bare local, both are call results / command state, so there is no expression to re-read and no parameter to use instead), dup-tail 46fc3b (the four local stores the original repeats in both paths - the source already writes them once before the branch and once in the arm), dup-tail 46ff06 (6 instructions), slots swaps 46fe8d / 470098. Q2 left: D2WIN.unk30 (0x30) is written once by the C (clear) command (d2.c:3029) and read nowhere in src/ or include/ - no evidence for a role (name gap). Named this session: d2_TextLayoutX/Y, d2_TextSegFlags, d2_MiscPic, d2_ResizeWindow, d2_ReplaceInputText. */

void d2_DrawWindowText(void *surf, int win)
{
    char tmp1[256];
    char tmp2[256];
    char errbuf[256];
    char buf128[128];
    char word[256];
    int c3, c2, c1, c0;
    int bottom, hasinputs, maxw;
    int pctflag, len;
    D2WIN *w;
    int top;
    D2RGB col;
    int width, extra, num, nlines, align, right, i, argi, left, pos;
    int arg, id, h, xx, yy, k, t;
    short pw;
    char *text, *start, *end, *p, *q;
    D2OBJ *o;
    D2INPUT *in;

    argi = 0;
    extra = 0;
    hasinputs = 0;
    nlines = 0;
    align = 0;
    pctflag = 0;
    w = &d2_Wins[win];
    if (w->font)
        d2_SetFont(w->font);
    else
        d2_SetFont(d2_FindPic("_FONT"));
    d2_TextSegFlags = 0;
    if (w->text) {
        left = w->ml + w->x;
        right = w->w - w->mr + w->x;
        top = w->mt + w->y;
        bottom = w->h - w->mb + top;
        if (w->flags & 4) {
            left += 3;
            right -= 3;
            top += 3;
            bottom -= 3;
        }
        maxw = w->w;
        if (w->flags & 8)
            top = w->y + 0xc;
        c0 = d2_ClipLeft;
        c1 = d2_ClipTop;
        c2 = d2_ClipRight;
        c3 = d2_ClipBottom;
        d2_TextLayoutY = top - w->scrollY;
        d2_TextLayoutX = left;
        d2_FontHeight = d2_FontPic->h;
        d2_TextSegCount = 0;
        pos = 0;
        len = strlen(w->text);
        {
            for (; pos < len;) {
                if (!w->text[pos])
                    break;
                d2_TextSegs[d2_TextSegCount].flags = 0;
                while (w->text[pos] == '%' || w->text[pos] == '$') {
                    pos++;
                    if (isdigit(w->text[pos])) {
                        num = w->text[pos] - '0';
                        pos++;
                    } else
                        num = -1;
                    text = w->text;
                    switch (text[pos]) {
                    case 'A':
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        k = w->text[pos - 1] - '0';
                        if (k > 0 && k < 9)
                            h = d2_FontHeight * k;
                        else
                            h = d2_FontHeight;
                        if (extra) {
                            h = extra;
                            extra = 0;
                        }
                        d2_TextLayoutX = left;
                        d2_TextLayoutY += h;
                        d2_TextSegFlags = 0;
                        pos++;
                        nlines++;
                        break;
                    case 'N':
                        if (text[pos - 1] != '$')
                            break;
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        d2_TextLayoutX = left;
                        h = d2_FontHeight * 3 / 2;
                        if (extra) {
                            h = extra;
                            extra = 0;
                        }
                        d2_TextSegFlags = 0;
                        d2_TextLayoutY += h;
                        pos++;
                        nlines++;
                        break;
                    case 'Y':
                        if (text[pos - 1] != '$')
                            break;
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        d2_TextLayoutX = left;
                        h = d2_FontHeight / 2;
                        if (extra) {
                            h = extra;
                            extra = 0;
                        }
                        d2_TextLayoutY += h;
                        pos++;
                        nlines++;
                        break;
                    case 'i':
                        if (text[pos + 1] == 'a' || text[pos + 1] == 'n') {
                            hasinputs = 1;
                            if (w->args[argi] == -1) {
                                k = d2_FindPic("_BUTTON_RED");
                                id = d2_WinCreateSlider(d2_TextLayoutX - w->x, d2_TextLayoutY - w->y, k, (int)ch_EmptyName, win);
                                if (w->text[pos + 2] == '[') {
                                    char buf512[512];
                                    D3_ZERO(buf512, 0x200);
                                    start = w->text + pos + 3;
                                    q = strchr(start, ']');
                                    if (!q) {
                                        sprintf(errbuf, "Missing ']'");
                                        break;
                                    }
                                    strncpy(buf512, start, q - start);
                                    d2_SetCaption(id, buf512);
                                    if (!q[1])
                                        w->text[pos + 2] = 0;
                                    else
                                        memmove(w->text + pos + 2, q + 1, strlen(q + 1) + 2);
                                }
                                if (w->text[pos + 1] == 'n')
                                    d2_Objs[id].style |= 0x10;
                                w->args[argi] = -1;
                                d2_TextLayoutX += d2_Objs[id].w;
                                w->args[argi] = id;
                                h = d2_Objs[id].h;
                            } else
                                h = d2_Objs[w->args[argi]].h;
                            extra = h + 7;
                        } else {
                            arg = w->args[argi];
                            if (arg != -1) {
                                yy = w->scrollY + d2_TextLayoutY;
                                t = d2_TextLayoutX;
                                if (arg == 0 || arg == d2_MiscPic || arg == d2_MiscPic + 1)
                                    yy += 2;
                                pw = d2_Pics[arg].w;
                                if (pw + d2_TextLayoutX > right) {
                                    xx = left;
                                    yy += d2_FontHeight;
                                } else
                                    xx = t;
                                if (pw > w->w) {
                                    pos++;
                                    break;
                                }
                                if (d2_TextLayoutX > left)
                                    xx += 6;
                                id = d2_WinOpenPic(xx - w->x, yy - w->y, arg, win);
                                if (id != -1) {
                                    if (w->text[pos + 1] == 'c') {
                                        d2_Objs[id].interactive = 0;
                                        d2_Objs[id].checkable = 1;
                                    } else if (w->text[pos + 1] == 'i') {
                                        d2_Objs[id].interactive = 0;
                                        d2_Objs[id].checkable = 0;
                                    } else if (w->text[pos + 1] == 'b') {
                                        d2_Objs[id].interactive = 1;
                                        d2_Objs[id].checkable = 0;
                                    } else if ((w->text[pos + 1] == 't' || w->text[pos + 1] == 'a' || w->text[pos + 1] == 'n')
                                               && w->text[pos + 2] == '[') {
                                        D3_ZERO(buf128, 0x80);
                                        d2_Objs[id].interactive = 1;
                                        d2_Objs[id].font = w->font;
                                        d2_Objs[id].style = 3;
                                        start = w->text + pos + 3;
                                        q = strchr(start, ']');
                                        if (!q) {
                                            sprintf(errbuf, "Missing ']'");
                                            break;
                                        }
                                        strncpy(buf128, start, q - start);
                                        d2_SetCaption(id, buf128);
                                        if (!q[1])
                                            w->text[pos + 2] = 0;
                                        else
                                            memmove(w->text + pos + 2, q + 1, strlen(q + 1) + 2);
                                    }
                                }
                                w->args[argi] = -1;
                                d2_TextLayoutX += d2_Objs[id].w;
                            }
                        }
                        pos += 2;
                        argi++;
                        break;
                    case 'C':
                        memmove(text, text + pos + 1, strlen(text + pos + 1) + 1);
                        d2_TextLayoutY = top - w->scrollY;
                        d2_TextLayoutX = left;
                        pos = 0;
                        w->unk30 = 0;
                        break;
                    case 'm':
                        if (text[pos + 1] != 't') {
                            pos++;
                            if (w->args[argi] == -1) {
                                xx = d2_TextLayoutX;
                                if (xx > left)
                                    xx += 6;
                                id = d2_WinOpenInput(xx - w->x, d2_TextLayoutY - w->y, 0x10, d2_FontHeight, 0x82, win);
                                k = d2_Objs[id].input;
                                w->args[argi] = k;
                                d2_TextLayoutX = d2_TextLayoutX + 6 + d2_Inputs[k].w;
                            }
                            argi++;
                        } else {
                            pos += 2;
                            if (w->args[argi] != -1) {
                                id = d2_WinOpenInput(d2_TextLayoutX - w->x, d2_TextLayoutY - w->y, 8, d2_FontHeight, 0x10, win);
                                d2_Objs[id].hidden = 1;
                                k = d2_Objs[id].input;
                                d2_Inputs[k].firstTextId = w->args[argi];
                                d2_Inputs[k].textCount = num;
                                d2_Inputs[k].currentTextId = w->args[argi + 1];
                                d2_Inputs[k].buf = w->text + pos;
                                d2_InputSetString(k, w->args[argi + 1]);
                                w->args[argi] = -1;
                                w->args[argi + 1] = k;
                                len = strlen(w->text);
                            }
                            d2_Inputs[w->args[argi + 1]].buf = w->text + pos;
                            d2_MoveObj((short)d2_TextLayoutX, (short)d2_TextLayoutY,
                                                     d2_Inputs[w->args[argi + 1]].obj);
                            argi += 2;
                        }
                        break;
                    case 'l':
                        pos++;
                        if (w->args[argi] != -1) {
                            id = d2_CreateWinCaption(d2_TextLayoutX - w->x, d2_TextLayoutY - w->y, (char *)w->args[argi], win);
                            d2_Objs[id].hidden = 1;
                            d2_Objs[id].u.raw = (unsigned short *)(w->text + pos);
                            d2_ReplaceInputText(id, (char *)w->args[argi]);
                            w->args[argi] = -1;
                            len = strlen(w->text);
                        }
                        argi++;
                        break;
                    case 'M':
                        memmove(text + pos - 1, text + pos + 1, strlen(text) - pos + 1);
                        strcpy(d2_TextSegs[d2_TextSegCount].text, "$M");
                        d2_TextSegs[d2_TextSegCount++].w = 0;
                        break;
                    case 't':
                        if (w->args[argi] != -1
                            && ((text[pos - 1] == '%' && num == -1) || (text[pos - 2] == '%' && num != -1))) {
                            xx = d2_TextLayoutX;
                            if (xx > left)
                                xx += 6;
                            id = d2_WinOpenInput(xx - w->x, d2_TextLayoutY - w->y, 0, d2_FontHeight, 1, win);
                            in = &d2_Inputs[d2_Objs[id].input];
                            arg = w->args[argi];
                            if (arg > 0x4800)
                                strcpy(in->text, (char *)arg);
                            else
                                strcpy(in->text, txt_Ptrs[arg]);
                            in->w = d2_TextWidth(in->text, in->font);
                            d2_Objs[in->obj].w = in->w;
                            w->args[argi] = -1;
                        }
                        pos++;
                        argi++;
                        break;
                    case 'T':
                        if (num == -1)
                            num = 1;
                        xx = left + d2_TextWidth("M", w->font) * num * 3;
                        if (xx > d2_TextLayoutX) {
                            d2_TextLayoutX = xx;
                            if (xx > right) {
                                d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                                d2_TextLayoutY += d2_FontHeight;
                                d2_TextLayoutX = left;
                            }
                            strcpy(d2_TextSegs[d2_TextSegCount].text, "$T");
                            d2_TextSegs[d2_TextSegCount].w = xx;
                            d2_TextSegCount++;
                        }
                        pos++;
                        break;
                    case '>':
                        if (num == -1)
                            num = 0;
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        d2_TextLayoutX = left = w->x + d2_TextWidth("M", w->font) * num * 2 + w->ml;
                        pos++;
                        break;
                    case '<':
                        if (num == -1)
                            num = 0;
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        right = w->x - w->mr - d2_TextWidth("M", w->font) * num * 2 + w->w;
                        pos++;
                        break;
                    case '[':
                        strcpy(d2_TextSegs[d2_TextSegCount].text, "$[");
                        d2_TextSegs[d2_TextSegCount++].w = 0;
                        pos++;
                        break;
                    case ']':
                        strcpy(d2_TextSegs[d2_TextSegCount].text, "$]");
                        d2_TextSegs[d2_TextSegCount++].w = 0;
                        pos++;
                        break;
                    case 'Z':
                        align = 0x100;
                        pos++;
                        break;
                    case 'R':
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        align = 0x40;
                        pos++;
                        break;
                    case 'B':
                        align = 0x80;
                        pos++;
                        break;
                    case 'L':
                        align = 0;
                        d2_TextLayoutX = left;
                        pos++;
                        break;
                    case 'F':
                        if (text[pos + 1] != 'F') {
                            d2_SetFont(d2_DefaultFont + num);
                            pos++;
                        } else {
                            pos++;
                            {
                            unsigned char tab[27] = {
                                0xdc, 0, 0,  0, 0xdc, 0,  0, 0, 0xdc,  0xff, 0xff, 0,  0xff, 0, 0xa8,
                                0xff, 0xff, 0xff,  0xa8, 0xa8, 0xa8,  0, 0xff, 0xc4,  0x96, 3, 3
                            };
                            if (num == 9) {
                                d2_TextSegFlags &= ~0x40;
                            } else {
                                d2_TextSegFlags |= 0x40;
                                col.r = tab[num * 3];
                                col.g = tab[num * 3 + 1];
                                col.b = tab[num * 3 + 2];
                            }
                            }
                            pos++;
                        }
                        break;
                    case '$':
                    case '%':
                        pctflag = 1;
                        break;
                    default:
                        sprintf(errbuf, "Unknown textparameter: %%%c", text[pos - 1]);
                        break;
                    }
                }
                if (w->text[pos]) {
                    int wl;
                    while (w->text[pos] == ' ')
                        pos++;
                    start = w->text + pos;
                    end = strchr(start, ' ');
                    if (!end)
                        end = w->text + len;
                    if (!pctflag && (p = strchr(start, '%')) != 0 && p <= end && p[1] != '%')
                        end = p;
                    pctflag = 0;
                    p = strchr(start, '$');
                    if (p && p < end)
                        end = p;
                    wl = end - start;
                    if (wl) {
                        memset(word, 0, sizeof(word));
                        if (start && wl >= 0) {
                            memcpy(word, start, wl);
                            word[wl + 1] = 0;
                        }
                        width = d2_TextWidth(word, w->font);
                        if (width + d2_TextLayoutX > right) {
                            if (w->flags & 0x2000) {
                                if (width + d2_TextLayoutX < d2_ScreenW)
                                    d2_ResizeWindow(w->w + width, w->h, win);
                                else {
                                    d2_ResizeWindow(d2_ScreenW - w->x, w->h, win);
                                    d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 0, align, left, right);
                                    d2_TextLayoutY += d2_FontHeight;
                                    d2_TextLayoutX = left;
                                    w->lines[w->firstline + nlines] = pos;
                                    nlines++;
                                }
                            } else {
                                t = 0;
                                id = strlen(word);
                                strcpy(tmp1, word);
                                strcpy(tmp2, word);
                                for (; id >= 0; id--) {
                                    if (word[id] == '~' || word[id] == '-') {
                                        strcpy(tmp1, &word[id]);
                                        t = 0;
                                        if (word[id] == '-')
                                            t = 1;
                                        word[id] = '-';
                                        word[id + 1] = 0;
                                        if (d2_TextWidth(word, w->font) + d2_TextLayoutX <= right)
                                            break;
                                    }
                                }
                                if (id > 0) {
                                    id += t;
                                    wl = strlen(word);
                                    memcpy(d2_TextSegs[d2_TextSegCount].text, word, wl + 1);
                                    d2_TextSegs[d2_TextSegCount++].w = d2_TextWidth(word, w->font);
                                    pos = pos + t + wl - 1;
                                } else
                                    strcpy(word, tmp2);
                                d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 0, align, left, right);
                                d2_TextLayoutY += d2_FontHeight;
                                d2_TextLayoutX = left;
                                w->lines[w->firstline + nlines] = pos;
                                nlines++;
                                if (id > 0) {
                                    strcpy(word, &tmp2[id]);
                                    wl = strlen(word);
                                    width = d2_TextWidth(word, w->font);
                                }
                            }
                        }
                        w->nlines = w->firstline + nlines;
                        for (t = ((int)(char)w->flags & 8U) >> 3; t < w->nobjs; t++) {
                            o = &d2_Objs[w->objs[t]];
                            while (!o->hidden && width + d2_TextLayoutX > o->x) {
                                id = o->x + o->w;
                                if (d2_TextLayoutX >= id || d2_FontHeight + d2_TextLayoutY <= o->y
                                    || d2_TextLayoutY >= o->h + o->y)
                                    break;
                                d2_TextLayoutX = id + 6;
                                if (width + d2_TextLayoutX > right) {
                                    d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 0, align, left, right);
                                    d2_TextLayoutX = left;
                                    d2_TextLayoutY += d2_FontHeight;
                                    w->lines[w->firstline + nlines] = pos;
                                    nlines++;
                                }
                            }
                        }
                        xx = d2_SpaceWidth - w->x + width + d2_TextLayoutX;
                        if (xx > maxw)
                            maxw = xx;
                        if (wl)
                            d2_TextLayoutX += d2_SpaceWidth + width;
                        memcpy(d2_TextSegs[d2_TextSegCount].text, word, wl + 1);
                        d2_TextSegs[d2_TextSegCount].w = width;
                        d2_TextSegs[d2_TextSegCount].flags = d2_TextSegFlags;
                        d2_TextSegs[d2_TextSegCount].col = col;
                        d2_TextSegCount++;
                        pos += wl;
                        if (d2_TextSegCount > 0x20)
                            d2_TextSegCount = 0;
                    }
                }
            }
        }
        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
        if (w->flags & 0x4000) {
            if (d2_FontHeight + d2_TextLayoutY - 8 > d2_ScreenH) {
                D2WIN *pw2 = d2_Objs[w->parent].win;
                if (pw2)
                    d2_MoveObj(pw2->x, (short)(pw2->y - d2_FontHeight - d2_TextLayoutY + d2_ScreenH + 8),
                                             pw2->parent);
                else
                    d2_MoveObj(w->x, (short)(d2_ScreenH - d2_FontHeight - d2_TextLayoutY + w->y - 8),
                                             w->parent);
            }
            d2_ResizeWindow(w->w, d2_FontHeight - w->y + d2_TextLayoutY, win);
        }
        d2_TextLayoutY += d2_FontHeight;
        if (w->flags & 0x2000)
            d2_ResizeWindow(maxw, w->h, win);
        if (d2_TextLayoutY - w->y > w->contentHeight)
            w->contentHeight = d2_TextLayoutY - w->y;
        d2_SetClipRect(c0, c1, c2, c3);
        if (w->flags & 0x20) {
            yy = (bottom - top - 0x10) / w->contentHeight + top;
            if (!d2_LayoutMode)
                d2_DrawShape(right + 1, yy, surf, d2_GetPic(d2_ScrollMarkerPic), 5);
        }
        if (w->sizedObjCount != w->nobjs && hasinputs) {
            d2_LayoutButtons(win);
            w->sizedObjCount = w->nobjs;
        }
    }
}
/* 0x4713c0, draws the background of window w onto surf: frame, saved images, sprite backgrounds, the tiled
   pictures (scrolled with the text) and the overlay image. The window's backgroundState picks the saved-image
   path (2: save the window's area into surf0, 1: put the saved image back, 0: nothing pending); the global
   d2_NoBackgroundRestore (char, role from its six tests: 1 draws the caption texture without the
   saved-background path; nothing in the sources writes it) forces the alternate path. */

void d2_DrawWindowBackground(D2WIN *w, void *surf)
{
    int x0, y0, ww, hh;
    void *pic, *fp;
    D2FRAME *fr;
    D2PIC *p;
    int top, left, c0, c1, c2, c3;
    int i, j, yy, xx, th, tw, rows, cols;

    if (w->flags & 0x400)
        d2_ClearRect(w->x, w->y, w->w, w->h, surf);
    if (!(w->flags & 0x400) && !(w->flags & 1) && d2_NoBackgroundRestore != 1 && w->backgroundState < 2 && (w->flags & 0x1000))
        d2_BlitPart(0, 0, w->w, w->h, w->surf0, w->x, w->y, surf);
    if (w->backgroundState == 2 && (w->flags & 0x1000) && d2_NoBackgroundRestore != 1)
        d2_BlitPart(w->x, w->y, w->w, w->h, surf, 0, 0, w->surf0);
    x0 = w->padl + w->x;
    y0 = w->padt + w->y;
    ww = w->w - w->padr - w->padl;
    hh = w->h - w->padb - w->padt;
    if ((w->flags & 1) && w->backgroundState == 2 && d2_NoBackgroundRestore != 1) {
        if (d2_CaptionTex && surf == d2_ScreenSurf) {
            d2_UnlockSurface(d2_ScreenSurf);
            d3if_DrawSprite(d2_CaptionTex, ww / 2 + x0, hh / 2 + y0, 0,
                         (float)ww / (float)d2_CaptionTex->size, (float)hh / (float)d2_CaptionTex->size, w->alpha);
            d2_LockSurface(d2_ScreenSurf);
        } else
            d2_FillRect(w->x, w->y, w->w, w->h, surf);
        if (w->flags & 0x1000)
            d2_BlitPart(w->x, w->y, w->w, w->h, surf, 0, 0, w->surf1);
    }
    if ((w->flags & 1) && d2_NoBackgroundRestore == 1) {
        if (d2_CaptionTex && surf == d2_ScreenSurf) {
            d2_UnlockSurface(d2_ScreenSurf);
            d3if_DrawSprite(d2_CaptionTex, ww / 2 + x0, hh / 2 + y0, 0,
                         (float)ww / (float)d2_CaptionTex->size, (float)hh / (float)d2_CaptionTex->size, w->alpha);
            d2_LockSurface(d2_ScreenSurf);
        } else
            d2_FillRect(w->x, w->y, w->w, w->h, surf);
    }
    if (w->spr && surf == d2_ScreenSurf) {
        d2_UnlockSurface(d2_ScreenSurf);
        d3if_DrawSprite(w->spr, ww / 2 + x0, hh / 2 + y0, 0,
                     (float)ww / (float)w->spr->size, (float)hh / (float)w->spr->size, w->alpha);
        d2_LockSurface(d2_ScreenSurf);
    }
    if (w->flags & 2)
        d2_DrawRectOutline(w->x, w->y, w->w - 1, w->h - 1, 0xff, 0xff, 0xff, surf);
    if (w->bgpic != -1 && (pic = d2_GetPic(w->bgpic)) != 0) {
        top = 0;
        left = 0;
        c0 = d2_ClipLeft;
        c1 = d2_ClipTop;
        c2 = d2_ClipRight;
        c3 = d2_ClipBottom;
        if (w->flags & 4) {
            fp = d2_GetPic(w->frame);
            top = d2_GetFrame(fp, 4)->h;
            fr = d2_GetFrame(fp, 6);
            left = fr->w;
        }
        d2_SetClipRect(w->x + left, w->y + top, w->w - left + w->x, w->h - top + w->y);
        p = &d2_Pics[w->bgpic];
        if (w->scrollY >= 0)
            yy = w->y - w->scrollY % p->h;
        else
            yy = w->y - w->scrollY % p->h - p->h;
        for (j = 0; (float)j < (float)(w->h / p->h) + 1.5f; j++)
            for (i = 0; (float)i < (float)(w->w / p->w) + 1.5f; i++)
                d2_DrawShape(w->x + left + p->w * i, p->h * j + yy + top, surf, pic, 0);
        d2_SetClipRect(c0, c1, c2, c3);
    }
    if (w->tileBackground) {
        th = d2_BackgroundTile->h;
        tw = d2_BackgroundTile->w;
        d2_SetSurfaceClip(w->x, w->y, w->w, w->h, surf);
        if (w->scrollY >= 0)
            yy = w->y - w->scrollY % th;
        else
            yy = w->y - w->scrollY % th - th;
        if (w->scrollX >= 0)
            xx = w->x - w->scrollX % tw;
        else
            xx = w->x - w->scrollX % tw - tw;
        rows = w->h / th + 2;
        cols = w->w / tw + 2;
        for (j = 0; j < rows; j++)
            for (i = 0; i < cols; i++)
                d2_BlitPart(0, 0, d2_BackgroundTile->w, d2_BackgroundTile->h, d2_BackgroundTile, xx + i * tw, yy + j * th, surf);
        d2_SetSurfaceClip(0, 0, d2_ScreenW, d2_ScreenH, surf);
    }
    if (w->surf2)
        d2_BlitPartMode(0, 0, w->surf2->w, w->surf2->h, w->surf2, w->x, w->y, surf, 1);
}

#define D2WOP(o) ((D2WIN *)((char *)d2_Wins + (o)))   /* window at byte offset o (win * 0x3c4); a pointer, so
                                                         the original's offset-in-a-register addressing stays */

/* 0x471a90, draws a window: its objects, scroll buttons, background and frame @deep near: 613 vs 613 instructions, 45/1872 bytes differ, 54 asm lines - only stack-slot permutations of the frame locals (the (int)((float)..+1.5f) casts use [esp+0x40] where the original has [esp+0x44] at 471d86/471de8/471e99/471ecf, and the scroll-button loop locals are permuted at 471e18/471e38/471eba/471f15/471fbc/[0x50]/[0x54]/[0x58]/[0x68]/[0x6c]) plus two moved add/sub pairs at 471fa7 and 471ff2 - i.e. declaration-order / spill allocation (vargen, listing). Tried: rewriting w->w + w->x - pw as w->w - pw + w->x at 471fa7/471ff2 - identical bytes, reverted; spell.py reload entries name the loop locals re-read at their uses (sw/sh/pw/ph/x are locals used as-is, nothing to drop - 471e38/471e4f/471eba/471f15/471fdb), iv-shift is the loop base suggestion only. Q2 clean (all fields named; the cornerObj / extraObj child objects are the 0x3bc / 0x3c0 pair). Named this session: d2_WinBorderPic / d2_WinBorderPergaPic (d2_FindPic of _WIN_BORDER / _WIN_BORDER_PERGA), D2OBJ.winIndex -> winIndex, d2_SetObjEnabled -> d2_SetObjEnabled. */
void d2_DrawWindow(win, surf)
int win;
void *surf;
{
    int saved;
    D2WIN *w = &d2_Wins[win];
    D2WIN *w2;
    int i, ob, in, v, n, p, pw, ph, sw, sh, x;
    char buf[32];
    void *pic;

    if (w->active) {
        saved = d2_LayoutMode;
        i = 0;
        d2_LayoutMode = 1;
        if (w->nobjs > 0) {
            do {
                ob = w->objs[i];
                if (d2_Objs[ob].type == 0x40) {
                    in = d2_Objs[ob].input;
                    if (d2_Wins[in].flags & 0x4000)
                        d2_DrawWindowText(0, in);
                }
                i++;
            } while (i < w->nobjs);
        }
        if (w->flags & 0x4000)
            d2_DrawWindowText(0, win);
        v = w->visible;
        d2_LayoutMode = saved;
        if (v != 0) {
            if (v % 8 == 0)
                w->scrollStep = 8;
            else if (v % 6 == 0)
                w->scrollStep = 6;
            else if (v % 4 == 0)
                w->scrollStep = 4;
            else if (v % 2 == 0)
                w->scrollStep = 2;
        }
        if (w->btnup != -1 || w->btndown != -1) {
            ob = w->btndown;
            w2 = &d2_Wins[d2_Objs[ob].winIndex];
            if (ob != -1) {
                if (w2->contentHeight < w2->h + w2->visible)
                    d2_SetObjEnabled(ob, 0);
                else
                    d2_SetObjEnabled(ob, 1);
                if (w2->scrollRemY + w2->scrollY + w2->h < w2->contentHeight)
                    d2_SetObjEnabled(w->btndown, 1);
                else
                    d2_SetObjEnabled(w->btndown, 0);
            }
            if (w->btnup != -1) {
                if (w2->scrollY == 0)
                    d2_SetObjEnabled(w->btnup, 0);
                else
                    d2_SetObjEnabled(w->btnup, 1);
            }
            if (w->bar != -1) {
                _itoa((int)((float)w2->contentHeight / w2->visible + 0.5f), buf, 10);
                d2_SetObjText(w->bar, buf);
            }
        }
        d2_ScrollWindow(win);
        if (d2_LayoutMode == 0)
            d2_DrawWindowBackground(w, surf);
        if (w->flags & 0x10)
            d2_DrawWindowText(surf, win);
        d2_SetClipRect(w->x, w->y, w->w + w->x, w->h + w->y);
        if ((w->flags & 4) && d2_LayoutMode == 0) {
            pic = d2_GetPic(w->frame);
            p = w->frame;
            if (p == d2_WinBorderPic || p == d2_WinBorderPergaPic) {
                if (w->cornerObj == -1)
                    d2_DrawShape(w->x, w->y, surf, pic, 0);
                d2_DrawShape(w->w - 8 + w->x, w->y, surf, pic, 1);
                d2_DrawShape(w->x, w->h - 8 + w->y, surf, pic, 2);
                d2_DrawShape(w->w - 8 + w->x, w->h - 8 + w->y, surf, pic, 3);
                n = (w->w - 0x10) >> 3;
                for (i = 0; i < n; i++) {
                    d2_DrawShape(w->x + 8 + i * 8, w->y, surf, pic, 5);
                    d2_DrawShape(w->x + 8 + i * 8, w->h - 8 + w->y, surf, pic, 8);
                }
                n = (w->h - 0x10) >> 3;
                for (i = 0; i < n; i++) {
                    d2_DrawShape(w->x, w->y + 8 + i * 8, surf, pic, 0xb);
                    d2_DrawShape(w->w - 8 + w->x, w->y + 8 + i * 8, surf, pic, 0xd);
                }
                if (w->flags & 0x8000) {
                    d2_DrawShape(w->x - 2, w->y - 2, surf, pic, 0x10);
                    d2_DrawShape(w->w - 6 + w->x, w->y - 2, surf, pic, 0x10);
                    d2_DrawShape(w->x - 2, w->h - 6 + w->y, surf, pic, 0x10);
                    d2_DrawShape(w->w - 6 + w->x, w->h - 6 + w->y, surf, pic, 0x10);
                }
            } else {
                pw = d2_Pics[p].w;
                ph = d2_Pics[p].h;
                d2_SetClipRect(w->x, w->y, w->w - pw + w->x, w->h + w->y);
                sh = d2_Pics[w->frame + 4].h;
                sw = d2_Pics[w->frame + 4].w;
                n = (int)((float)((w->w - pw * 2) / sw) + 1.5f);
                if (n > 0) {
                    for (x = 0; n; n--, x += sw) {
                        d2_DrawShape(w->x + x + pw, w->y, surf, pic, 4);
                        d2_DrawShape(w->x + x + pw, w->h + w->y - sh, surf, pic, 5);
                    }
                }
                sw = d2_Pics[w->frame + 7].w;
                sh = d2_Pics[w->frame + 7].h;
                d2_SetClipRect(w->x, w->y, w->w + w->x, w->h - ph + w->y);
                n = (int)((float)((w->h - ph * 2) / sh) + 1.5f);
                if (n > 0) {
                    for (x = 0; n; n--, x += sh) {
                        d2_DrawShape(w->x, w->y + x + ph, surf, pic, 6);
                        d2_DrawShape(w->w + w->x - sw, w->y + x + ph, surf, pic, 7);
                    }
                }
                d2_SetClipRect(w->x, w->y, w->w + w->x, w->h + w->y);
                if (w->cornerObj == -1)
                    d2_DrawShape(w->x, w->y, surf, pic, 0);
                d2_DrawShape(w->w + w->x - pw, w->y, surf, pic, 1);
                d2_DrawShape(w->x, w->h + w->y - ph, surf, pic, 2);
                d2_DrawShape(w->w + w->x - pw, w->h + w->y - ph, surf, pic, 3);
            }
        }
        if (w->backgroundState)
            w->backgroundState--;
        d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
    }
}
/* 0x4721e0, d2_SetBackgroundTexture (error string): sets (or removes) the background texture of a window */

void d2_SetBackgroundTexture(name, win)
char *name;
int win;
{
    char buf[1024];
    D2WIN *w = &d2_Wins[win];

    if (w->spr) {
        d3if_FreeSprite((int)w->spr);
        w->spr = 0;
    }
    if (name && *name) {
        w->spr = (D2TEX *)d3if_LoadSprite(name, 1);
        if (!w->spr) {
            sprintf(buf, "d2_SetBackgroundTexture(): Could not open %s.BMP name");
            err_Message(buf);
        }
        w->alpha = 0.5f;
    }
}
/* 0x472270, creates the two scroll buttons of a window. The body reaches the window through D2WOP(o), the
   window at the byte offset o = win * 0x3c4: the original computes the address as lea/shl from the byte
   offset, so the offset form is what the bytes need (the index form d2_Wins[win] loses the match:
   238/304 bytes differ). */

void d2_CreateScrollButtons(x, y, win, obj, pic)
int x;
int y;
int win;
int obj;
int pic;
{
    int i;
    int o = win * 0x3c4;

    if (x == -1 && y == -1) {
        x = D2WOP(o)->w - 0x20;
        y = D2WOP(o)->h - 0x20;
    }
    i = d2_WinOpenPic(x, y, pic, win);
    D2WOP(o)->btndown = i;
    d2_Objs[i].winIndex = obj;
    d2_Objs[D2WOP(o)->btndown].style = 3;
    i = d2_WinOpenPic(x - 0x20, y, pic + 1, win);
    D2WOP(o)->btnup = i;
    d2_Objs[i].winIndex = obj;
    d2_Objs[D2WOP(o)->btnup].style = 3;
    D2WOP(o)->scrollY = 0;
    d2_Wins[obj].visible = 0x30;
}
/* 0x472390, creates a window; returns its index, -1 if there is no free one, 0 if a surface could not be made @deep draft: 638/976 bytes differ, 121 asm lines after this session's two meaning fixes (win->w = w, the surface desc[1] = w; 740 bytes / 167 lines before), hunks code 121 / regs 58. Read against the original 0x472390..: the free-window scan (counter/pointer in ebp/eax vs eax/ecx), the index scaling lea chain, the flags register (ours ecx/bh vs original ebx), the -1 materialisation (or ebx,-1 vs or eax,-1) and the y+h add order at 472611 are register allocation / scheduling; the 4724a1 desc push order now reads right. Tried: the operand order win->h + win->y -> win->y + win->h at 472611 (identical bytes, reverted). Q2 left, no writer anywhere in src/ (name gaps): D2WIN.unk274 (written -1 here only). The two child-object slots are cornerObj / extraObj (0x3bc / 0x3c0). Named this session: d2_CreateWindow (from d2_CreateWindow), d2_LastWindow / d2_LastWindowPtr (d2_LastWindow / d2_LastWindowPtr, the window created last). */

int d2_CreateWindow(x, y, w, h, flags)
short x;
short y;
short w;
short h;
unsigned int flags;
{
    int desc[16];
    int i, j, ob;
    D2WIN *win;
    int *p;
    void *surf;

    d2_WindowsOpen++;
    if (d2_WindowsOpen > 0x60)
        d2_WindowsOpen = 1;
    j = 0;
    p = &d2_Wins[0].active;
    while (1) {
        if (!*p)
            break;
        p += 0xf1;
        j++;
        if ((int)p >= (int)&d2_Wins[0x60].active)
            return -1;
    }
    if (j == -1)
        return j;
    i = j;
    win = &d2_Wins[i];
    win->y = y;
    win->x = x;
    win->w = w;
    win->h = h;
    win->flags = flags;
    win->ml = 4;
    win->mr = 4;
    win->mt = 4;
    win->mb = 4;
    win->spr = 0;
    win->scrollStep = 4;
    win->visible = 0;
    if (flags & 4)
        win->frame = d2_WinBorderPic;
    if (flags & 0x200) {
        win->contentHeight = h;
        win->scrollY = -h;
        win->appliedScrollY = -h;
    }
    if (flags & 1)
        win->alpha = 0.65f;
    if (flags & 0x1000) {
        memset(desc, 0, sizeof desc);
        desc[1] = w;
        desc[2] = h;
        win->surf0 = d2_MakeSurface(desc, 0xb);
        if (!win->surf0)
            return 0;
    }
    if ((win->flags & 1) && (win->flags & 0x1000)) {
        memset(desc, 0, sizeof desc);
        desc[1] = win->w;
        desc[2] = win->h;
        win->surf1 = d2_MakeSurface(desc, 0xb);
        if (!win->surf1)
            return 0;
    }
    if (win->flags & 0x10000) {
        memset(desc, 0, sizeof desc);
        desc[1] = win->w;
        desc[2] = win->h;
        surf = d2_MakeSurface(desc, 0xb);
        win->surf2 = surf;
        d2_ClearSurface(surf);
        if (!win->surf2)
            return 0;
    }
    ob = d2_NewObject();
    d2_Objs[ob].type = 0x40;
    d2_Objs[ob].x = win->x;
    d2_Objs[ob].y = win->y;
    d2_Objs[ob].w = win->w;
    d2_Objs[ob].h = win->h;
    d2_Objs[ob].pic = i + 0x400;
    d2_Objs[ob].input = i;
    d2_Objs[ob].clipy0 = win->y;
    d2_Objs[ob].clipx1 = win->w + win->x;
    d2_Objs[ob].clipx0 = win->x;
    d2_Objs[ob].clipy1 = win->h + win->y;
    d2_Objs[ob].drawFrames = 2;
    d2_Objs[ob].data = win;
    d2_Objs[ob].win = 0;
    win->parent = ob;
    win->backgroundState = 1;
    win->drawFrames = 2;
    win->bgpic = -1;
    win->font = d2_DefaultFont;
    win->scrollBar = -1;
    win->cornerObj = -1;
    win->extraObj = -1;
    win->unk274 = -1;
    win->active = 1;
    win->btndown = -1;
    win->btnup = -1;
    win->bar = -1;
    win->objs = mem_Alloc(0x600, "d2:win_obj");
    win->padl = 0;
    win->padt = 0;
    win->padr = 0;
    win->padb = 0;
    if (flags & 8)
        d2_WinOpenPic(3, 3, d2_ScrollMarkerPic, i);
    if (flags & 0x20) {
        d2_WinOpenPic(win->w - 0xc, 3, d2_ScrollMarkerPic + 1, i);
        d2_WinOpenPic(win->w - 0xc, win->h - 10, d2_ScrollMarkerPic + 2, i);
    }
    if (flags & 0x10)
        win->text = mem_Alloc(0x17d0, "d2_t:win->txt");
    d2_LastWindowPtr = win;
    d2_LastWindow = i;
    return i;
}
/* 0x472760, scrolls the content of a window. The body reads/writes the window through D2WOP(o), the window
   at the byte offset o = win * 0x3c4, eleven times - that form is what the bytes need (the index form
   D2WOP(o) -> d2_Wins[win] costs 19/160 bytes); one access, d2_Wins[win].scrollRemY += t, is the index form
   and is kept for the same reason (comment in the body). */

void d2_ScrollWindowBy(a, b, win)
int a;
int b;
int win;
{
    int o = win * 0x3c4;
    int t;

    if (D2WOP(o)->scrollY + D2WOP(o)->scrollRemY > 0 && b < 0) {
        t = -(D2WOP(o)->scrollY + D2WOP(o)->scrollRemY);
        if (b > t)
            t = b;
        D2WOP(o)->scrollRemY = t;
    } else if (D2WOP(o)->h + D2WOP(o)->scrollY + D2WOP(o)->scrollRemY < D2WOP(o)->contentHeight && b > 0) {
        t = D2WOP(o)->contentHeight - D2WOP(o)->h - D2WOP(o)->scrollY - D2WOP(o)->scrollRemY;
        if (b < t)
            t = b;
        d2_Wins[win].scrollRemY += t;     /* the index form: no reuse of the loaded scrollRemY, add [mem] */
    }
    D2WOP(o)->scrollRemX += a;
}
/* 0x472800, resizes a window (grows in height so that all objects fit) @deep draft, readable (Q1/Q2/Q4 done this session): 438/608 bytes, 29 asm lines, hunks code 40 / order 6 / fill 1 - same instructions, scheduling and operand order only. spell.py: no hunk matched the menu (escalate with the diffclass verdict). Left: 47284c (the delta store moved across the jle), 472877 (the object-loop body loads d2_Objs[].h before wn->h in the original; ours loads wn->h first), 47298a / 472a01 (our o->clipy1 = wn->y + wn->h emits y then h, the original h then y; o->clipx1 similarly), 472a5f alignment fill. Tried: swapping the two clip expressions to wn->h + wn->y / wn->x + wn->w - identical bytes, reverted. Q4 fixed byte-neutrally here: int *p = wn->objs; *p; p++ instead of the int-as-byte-pointer walk win = (int)wn->objs / *(int *)win / win += 4 (438/608 bytes and 29 lines unchanged). Q1/Q2 clean (named d2_ResizeWindow earlier this session, was d2_ResizeWindow): the typed int *p walk replaced the two *(int *)win raw accesses. */

void d2_ResizeWindow(short w, short h, int win)
{
    D2WIN *wn = &d2_Wins[win];
    int delta, dw, n, i, oh, oy;
    int desc[16];
    int *p;
    D2OBJ *o;
    void *ns, *old;

    if (wn->active) {
        dw = w - wn->w;
        delta = h - wn->h;
        n = wn->nobjs;
        if (n > 0) {
            p = wn->objs;
            do {
                oh = d2_Objs[*p].h;
                oy = d2_Objs[*p].y;
                if (wn->y + wn->h + delta < oy + oh)
                    delta = oy - wn->y - wn->h + 8 + oh;
                p++;
                n--;
            } while (n);
        }
        if (wn->surf0) {
            mem_Free(wn->surf0);
            wn->surf0 = 0;
        }
        if (wn->surf1) {
            mem_Free(wn->surf1);
            wn->surf1 = 0;
        }
        if (wn->surf2) {
            memset(desc, 0, sizeof desc);
            desc[1] = w;
            desc[2] = h;
            ns = d2_MakeSurface(desc, 0xb);
            old = wn->surf2;
            d2_BlitPart(0, 0, ((int *)old)[1], ((int *)old)[2], old, 0, 0, ns);
            d2_FreeSurface(wn->surf2);
            wn->surf2 = ns;
        }
        d2_AddDirty(wn->x, wn->y, wn->w, wn->h);
        wn->h += delta;
        wn->w += dw;
        o = &d2_Objs[wn->parent];
        o->clipy1 = wn->y + wn->h;
        o->clipx1 = wn->w + wn->x;
        if (wn->y + wn->h > d2_ScreenH)
            wn->h = d2_ScreenH - wn->y;
        if (wn->w + wn->x > d2_ScreenW)
            wn->w = d2_ScreenW - wn->x;
        for (i = 0; i < wn->nobjs; i++) {
            o = &d2_Objs[wn->objs[i]];
            o->clipy1 = wn->y + wn->h;
            o->clipx1 = wn->w + wn->x;
        }
        o = &d2_Objs[wn->parent];
        o->w = wn->w;
        o->h = wn->h;
    }
}
/* 0x472a60, ends every live fade: frees the ones flagged remove-when-done (8), marks the others done (4) */

void d2_FinishFades(void)
{
    int i;
    D2FADE *p;

    for (i = 0; i < 32; i++) {
        p = d2_Fades[i];
        if (p) {
            if (p->flags & 8) {
                d2_ReleaseFade(p);
                d2_Fades[i] = 0;
            } else
                p->flags |= 4;
        }
    }
}
/* 0x472aa0, creates a window inside another one (relative to its scroll position) */

int d2_CreateChildWindow(short x, short y, short pw, short h, unsigned int flags, int win)
{
    D2WIN *w = &d2_Wins[win];
    int n;
    int ww, hh;

    if (!w->active)
        return -1;
    n = d2_CreateWindow(w->x - w->scrollX + x, w->y - w->scrollY + y, pw, h, flags);
    d2_AddToDrawOrder(d2_Wins[n].parent, w);
    w->objs[w->nobjs] = d2_Wins[n].parent;
    d2_Objs[d2_Wins[n].parent].win = w;
    d2_Objs[d2_Wins[n].parent].clipy0 = w->y + y;
    d2_Objs[d2_Wins[n].parent].clipy1 = y + h + w->y;
    d2_Objs[d2_Wins[n].parent].clipx0 = w->x + x;
    d2_Objs[d2_Wins[n].parent].clipx1 = w->x + x + pw;
    d2_Objs[d2_Wins[n].parent].buf = d2_Objs[w->parent].buf;
    w->nobjs++;
    d2_GetFrameSize(w, &ww, &hh);
    if (d2_Wins[n].h + y > w->contentHeight)
        w->contentHeight = d2_Wins[n].h + ww * 2 + y;
    return n;
}
/* 0x472c80, size of the frame pieces of a window (0 if it has no frame) */

void d2_GetFrameSize(win, w, h)
D2WIN *win;
int *w;
int *h;
{
    void *pic;

    if (win->flags & 4) {
        pic = d2_GetPic(win->frame);
        *h = d2_GetFrame(pic, 4)->h;
        *w = d2_GetFrame(pic, 6)->w;
        return;
    }
    *h = 0;
    *w = 0;
}
/* 0x472ce0, removes the object of a window */

int d2_RemoveWindow(win)
int win;
{
    if (d2_Wins[win].active) {
        d2_RemoveObj(d2_Wins[win].parent);
        return 1;
    }
    return 0;
}
/* 0x472d20, removes the objects of a window (all of them, or all but the frame) */

int d2_RemoveWindowObjects(win, all)
int win;
int all;
{
    int i, ob;

    if (win > 0x60)
        return 0;
    if (!d2_Wins[win].active)
        return 0;
    {
        for (i = d2_Wins[win].nobjs - 1; i >= 0; i--) {
            ob = d2_Wins[win].objs[i];
            if (ob == d2_Wins[win].btnup)
                d2_Wins[win].btnup = -1;
            else if (ob == d2_Wins[win].btndown)
                d2_Wins[win].btndown = -1;
            else if (ob == d2_Wins[win].bar)
                d2_Wins[win].bar = -1;
            ob = d2_Wins[win].objs[i];
            if (d2_Objs[ob].type != 0x40 || all != 0)
                d2_RemoveObj(ob);
        }
        if (d2_Wins[win].text)
            *d2_Wins[win].text = 0;
        d2_Wins[win].scrollY = 0;
        d2_Wins[win].contentHeight = 0;
        d2_Wins[win].scrollRemY = 0;
        d2_Wins[win].appliedScrollY = 0;
        return 1;
    }
}
/* 0x472e00, closes a window and everything on it; 0 if there is no such window */

int d2_CloseWindow(int win)
{
    D2WIN *w;

    if (win > 0x60)
        return 0;
    w = &d2_Wins[win];
    if (w->active == 0)
        return 0;
    while (w->nobjs != 0)
        d2_RemoveObj(w->objs[0]);
    if (w->objs) {
        mem_Free(w->objs);
        w->objs = 0;
    }
    if (w->flags & 0x1000)
        d2_BlitPart(0, 0, w->w, w->h, w->surf0, 0, 0, d2_BackSurf);
    if (w->surf0)
        d2_FreeSurface(w->surf0);
    if (w->surf1)
        d2_FreeSurface(w->surf1);
    if (w->surf2)
        d2_FreeSurface(w->surf2);
    if (w->spr)
        d3if_FreeSprite((int)w->spr);
    if (w->text) {
        mem_Free(w->text);
        w->text = 0;
    }
    if (w->scrollBar != -1)
        d2_RemoveObj(w->scrollBar);
    if (w->cornerObj != -1)
        d2_RemoveObj(w->cornerObj);
    if (w->extraObj != -1)
        d2_RemoveObj(w->extraObj);
    memset(w, 0, sizeof(D2WIN));
    w->id = win;
    w->bar = -1;
    d2_WindowsOpen--;
    return 1;
}
/* 0x472f50, moves an object up to the end of its run in the draw order list */

void d2_RaiseObject(int obj)
{
    int i, first;

    first = -1;
    for (i = 0x2ff; i > 0; i--) {
        if (d2_DrawOrder[i] == &d2_Objs[obj]) {
            if (first != -1) {
                d2_DrawOrder[first] = d2_DrawOrder[i];
                memmove(&d2_DrawOrder[i], &d2_DrawOrder[i + 1], 0xbf8 - i * 4);
            }
            return;
        }
        if (d2_DrawOrder[i] != 0 && first == -1)
            first = i + 1;
    }
}
/* 0x472fd0, moves the objects of a window (and the window's parent run) to the top of the draw order list @deep B3: two code hunks (0x472fe9, 0x473027), reordered only - same operations both sides. Original hoists j=-1 out of the loop (or esi,-1 before it), keeps i in ecx and j in esi; ours keeps the mov ecx,-1 at the loop bottom and swaps i=esi / j=ecx, so the loop head and exit order differ. Read against tools/disasm.py 472fd0 and the VC6 listing: the loop, the break path (j=i) and the post-loop first=j are semantically right; the j=-1-at-bottom form is what the source needs. spell.py: no menu rule matched. Tried (each one edit + build, reverted): move j=i into the break block and drop j=-1 (211/272 WORSE), and the same with the original redundant !=0 text (211/272 WORSE). Left: register allocation / loop-invariant -1 store hoisting (vargen / scheduling). */

void d2_RaiseWindow(int win)
{
    D2WIN *w = &d2_Wins[win];
    D2OBJ *tmp[385];
    D2OBJ *o;
    int i, j, first, n;

    first = -1;
    j = -1;
    i = 0x2ff;
    do {
        o = d2_DrawOrder[i];
        if (o != 0) {
            j = i;
            if (o->id == w->parent)
                break;
            if (((D2OBJ *)d2_DrawOrder[i]) != 0 && ((int *)o)[0xc] == (int)w && first == -1)
                first = i;
        }
        i--;
        j = -1;
    } while (i >= 0);
    if (first == -1)
        first = j;
    n = (first - j) * 4 + 4;
    memcpy(tmp, &d2_DrawOrder[j], n);
    memmove(&d2_DrawOrder[j], &d2_DrawOrder[first + 1], 0xbf8 - first * 4);
    i = 0x2ff;
    do {
        if (d2_DrawOrder[i] != 0)
            break;
        i--;
    } while (i >= 0);
    memcpy(&d2_DrawOrder[i + 1], tmp, n);
    for (i = 0; i < w->nobjs; i++)
        d2_RaiseObject(w->objs[i]);
}
/* 0x4730e0, puts an object into the draw order list just above its window */

void d2_AddToDrawOrder(int obj, D2WIN *win)
{
    D2OBJ *p;
    int i;

    i = 0;
    do {
        if (d2_DrawOrder[i] == &d2_Objs[obj]) {
            if (i == 0x2ff)
                d2_DrawOrder[0x2ff] = 0;
            else
                memmove(&d2_DrawOrder[i], &d2_DrawOrder[i + 1], 0xbf8 - i * 4);
            break;
        }
        i++;
    } while (i < 0x2ff);
    i = 0x2ff;
    while ((p = d2_DrawOrder[i], p == 0 || ((p->win != win || p->type == 0x40) && p->id != win->parent))) {
        i--;
        if (i < 0) {
            err_Message("Toby, wir haben ein Problem...");
            return;
        }
    }
    if (i >= 0) {
        memmove(&d2_DrawOrder[i + 2], &d2_DrawOrder[i + 1], 0xbf8 - i * 4);
        d2_DrawOrder[i + 1] = &d2_Objs[obj];
        return;
    }
    err_Message("Toby, wir haben ein Problem...");
}

/* the callers above pass shorts (see d2.h), the calls below take them too */
int d2_CreatePicObj(short x, short y, int pic);  /* @original: K&R definition (movsx reads) with a short prototype for its callers (16-bit arithmetic in d2_WinOpenPic); C89 forbids the pair, the bytes need it */

/* 0x4731d0, creates a picture object on a window (position scrolled in both directions) */
int d2_WinOpenPic(short x, short y, int pic, int win)
{
    D2WIN *w = &d2_Wins[win];
    int obj, bw, bh;

    bw = 0;
    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    w->objs[w->nobjs] = d2_CreatePicObj(w->x - w->scrollx + x, w->y - w->scrolly + y, pic);
    obj = w->objs[w->nobjs];
    if (obj != -1) {
        d2_AddToDrawOrder(obj, w);
        d2_Objs[obj].win = w;
        d2_Objs[obj].clipy0 = 0;
        d2_Objs[obj].clipy1 = d2_ScreenH;
        d2_Objs[obj].clipx0 = 0;
        d2_Objs[obj].clipx1 = d2_ScreenW;
        d2_Objs[obj].buf = d2_Objs[w->parent].buf;
        d2_Objs[obj].disabled = d2_Objs[w->parent].disabled;
        w->nobjs++;
        d2_GetFrameSize(w, &bw, &bh);
        if ((short)y + d2_Objs[obj].h > w->contentHeight)
            w->contentHeight = (short)y + bw * 2 + d2_Objs[obj].h;
    }
    return obj;
}
/* 0x473350, the same without the horizontal scroll, and with a style */

int d2_WinOpenPicStyled(int x, int y, int pic, int win, int a)
{
    D2WIN *w = &d2_Wins[win];
    int obj;


    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    w->objs[w->nobjs] = d2_CreatePicObj(w->x + x, w->y - w->scrolly + y, pic);
    obj = w->objs[w->nobjs];
    if (obj != -1) {
        d2_AddToDrawOrder(obj, w);
        d2_Objs[obj].win = w;
        d2_Objs[obj].clipy0 = 0;
        d2_Objs[obj].clipy1 = d2_ScreenH;
        d2_Objs[obj].clipx0 = 0;
        d2_Objs[obj].clipx1 = d2_ScreenW;
        d2_Objs[obj].buf = d2_Objs[w->parent].buf;
        d2_Objs[obj].disabled = d2_Objs[w->parent].disabled;
        d2_ScaleObject(obj, a);
        w->nobjs++;
        d2_GetFrameSize(w, &x, &win);
        if ((short)y + d2_Objs[obj].h > w->contentHeight)
            w->contentHeight = (short)y + x * 2 + d2_Objs[obj].h;
    }
    return obj;
}
/* 0x4734d0, sets the text of a text object */

void d2_SetObjText(int obj, char *text)
{

    strcpy((char *)d2_Objs[obj].input, text);
    if (d2_Objs[obj].centred == 0 && d2_Objs[obj].rightAligned == 0)
        d2_Objs[obj].w = d2_TextWidth(text, d2_Objs[obj].font);
}
/* 0x473540, sets the caption of a button and its width */

void d2_SetCaption(int obj, char *text)
{
    D2OBJ *o = &d2_Objs[obj];
    void *pic;
    D2FRAME *f0, *f1;
    int a, b;

    if (d2_Objs[obj].type == 9) {
        pic = o->data;
        f0 = d2_GetFrame(pic, 0);
        f1 = d2_GetFrame(pic, 1);
        a = d2_TextWidth(text, d2_DefaultFont) + f1->w + 4 + f0->w;
        strncpy(o->u.text, text, 0x3f);
        d2_Objs[obj].w = a;
        return;
    }
    if (d2_GetObjPicSize(obj, &a, &b) == 0)
        a = 0x18;
    strncpy(d2_Objs[obj].u.text, text, 0x3f);
    if (d2_Objs[obj].centred == 0) {
        d2_Objs[obj].w = d2_TextWidth(text, d2_Objs[obj].font) + 6 + a;
        return;
    }
    d2_Objs[obj].w = a;
}
/* 0x473650, creates a text label on a window */

int d2_CreateWinText(short x, short y, char *text, int win)
{
    D2WIN *w = &d2_Wins[win];
    int obj;

    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    obj = d2_NewObject();
    d2_Objs[obj].input = (int)mem_Alloc(0x101, "d2:ShowTxt");
    if ((char *)d2_Objs[obj].input == 0)
        return -1;
    strcpy((char *)d2_Objs[obj].input, text);
    if (w->font != 0)
        d2_Objs[obj].font = w->font;
    else
        d2_Objs[obj].font = d2_DefaultFont;
    d2_Objs[obj].type = 0x43;
    d2_Objs[obj].x = w->x + x;
    d2_Objs[obj].y = (w->y - w->scrolly) + y;
    d2_Objs[obj].w = d2_TextWidth(text, d2_Objs[obj].font) + 0x60;
    d2_Objs[obj].h = d2_FontHeight;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].win = w;
    d2_Objs[obj].data = w;
    d2_Objs[obj].clipy0 = w->y;
    d2_Objs[obj].clipy1 = w->h + w->y;
    d2_Objs[obj].clipx0 = w->x;
    d2_Objs[obj].clipx1 = w->w + w->x;
    d2_Objs[obj].buf = d2_Objs[w->parent].buf;
    d2_Objs[obj].u.text[0] = 0;
    if (y + d2_Objs[obj].h > w->contentHeight)
        w->contentHeight = y + d2_Objs[obj].h;
    d2_AddToDrawOrder(obj, w);
    return w->objs[w->nobjs++] = obj;
}
/* 0x473860, creates a text label on the screen */

int d2_CreateScreenText(short x, short y, char *text)
{
    int obj;
    short tw;

    obj = d2_NewObject();
    d2_Objs[obj].input = (int)mem_Alloc(0x101, "d2:ShowTxt");
    if ((char *)d2_Objs[obj].input == 0)
        return -1;
    strcpy((char *)d2_Objs[obj].input, text);
    d2_Objs[obj].font = d2_DefaultFont;
    d2_Objs[obj].type = 0x43;
    d2_Objs[obj].x = x;
    d2_Objs[obj].y = y;
    tw = d2_TextWidth(text, d2_Objs[obj].font);
    d2_Objs[obj].w = tw + 0x60;
    d2_Objs[obj].h = d2_FontHeight;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    return obj;
}
/* 0x473980, creates a caption object on a window */

int d2_CreateWinCaption(short x, short y, char *text, int win)
{
    D2WIN *w = &d2_Wins[win];
    int obj;

    if (!w->active)
        return -1;
    if (w->nobjs >= 0x180) {
        err_Message("Too many objects on window!");
        return -1;
    }
    obj = d2_NewObject();
    strcpy(&d2_Objs[obj].u.text[4], text);
    if (w->font != 0)
        d2_Objs[obj].font = w->font;
    else
        d2_Objs[obj].font = d2_DefaultFont;
    d2_Objs[obj].type = 0x46;
    d2_Objs[obj].x = w->x + x;
    d2_Objs[obj].y = (w->y - w->scrolly) + y;
    d2_Objs[obj].w = d2_TextWidth(text, d2_Objs[obj].font) + 0x60;
    d2_Objs[obj].h = d2_FontHeight;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].win = w;
    d2_Objs[obj].data = w;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    d2_Objs[obj].buf = d2_Objs[w->parent].buf;
    d2_AddToDrawOrder(obj, w);
    w->objs[w->nobjs] = obj;
    w->nobjs++;
    return obj;
}
/* 0x473b30, replaces the string of an input object inside the window text */

void d2_ReplaceInputText(int obj, char *text)
{
    D2OBJ *o = &d2_Objs[obj];
    D2WIN *w = o->win;
    char *buf;
    int len, old;

    if (w && (buf = (char *)o->u.s.value) != 0) {
        old = o->insertedLen;
        if (old)
            memmove(buf, buf + old, strlen(w->text) + (w->text - buf) + 1);
        len = strlen(text);
        o->insertedLen = len;
        memmove((char *)o->u.s.value + len, (char *)o->u.s.value,
                strlen(w->text) + (w->text - (char *)o->u.s.value) + 1);
        memcpy((char *)o->u.s.value, text, len);
        o->w = d2_TextWidth(text, o->font);
        strcpy(&o->u.text[4], text);
    }
}
/* 0x473c10, d2_Open (error strings): allocates the lists, reads the picture table of the gfx file dir\gfx\file,
   loads the locked pictures, makes the surfaces and finds the standard pictures; 0 on failure */

int d2_Open(int hwnd, int w, int h, char *dir, char *file)
{
    int desc[16];
    char path[256];
    char sd0[0x7c];
    char sd1[0x7c];
    int i;
    char *p;

    d2_MainWindow = 0;
    d2_DrawOrder = mem_Alloc(0xc00, "d2:zorder");
    d2_Objs = mem_Alloc(0x8c400, "d2:obj");
    d2_Pics = mem_Alloc(0x2a000, "d2:fileobj");
    d2_Dirty[0] = mem_Alloc(0x3000, "d2:d2_RedrawInfo0");
    d2_Dirty[1] = mem_Alloc(0x3000, "d2:d2_RedrawInfo1");
    d2_RedrawInfo2 = mem_Alloc(0x3000, "d2:d2_RedrawInfo2");
    d2_Shapes = mem_Alloc(0x3c00, "d2:RedrawInfoVirtual");
    memset(d2_Wins, 0, 0x60 * sizeof(D2WIN));
    d2_Objs[0].id = 0;
    memset(d2_Pics, 0, 4);   /* the original clears the first 4 bytes with one dword store */
    d2_DrawOrder[0] = 0;
    memset(&d2_DirtyRectLeft, 0, 16);   /* d2_DirtyRectLeft..6c, four ints (one memset, not four stores) */
    memset(d2_Fades, 0, sizeof(d2_Fades));
    for (i = 0; i < 0x30; i++)
        d2_Views[i].id = i;
    for (i = 0; i < 0x60; i++)
        d2_Wins[i].id = i;
    for (i = 0; i < 0x300; i++)
        d2_Objs[i].id = i;
    d2_PixelFormat = dd_PixelFormat;
    if (d2_File)
        return 0;
    sprintf(path, "%sgfx\\%s", dir, file);
    strcpy(d2_GfxPath, path);
    strcpy(d2_GfxDir, path);
    p = strrchr(d2_GfxDir, '\\');
    if (p)
        p[1] = 0;
    d2_File = fopen(path, "rb");
    if (!d2_File)
        return 0;
    fread(&d2_NPics, 4, 1, d2_File);
    if (d2_NPics > 0x800) {
        fclose(d2_File);
        err_Message("d2_Open:Too many Objects in GFX-File...");
        return 0;
    }
    for (i = 0; i < d2_NPics; i++)
        fread(&d2_Pics[i], 0x54, 1, d2_File);
    fclose(d2_File);
    d2_SetColourFormat(0, 1);
    for (i = 0; i < d2_NPics; i++)
        if ((d2_Pics[i].flags & 1) && d2_Pics[i].size)
            d2_LoadPic(i, 0);
    d2_ScreenScale = (float)w * 0.00125f;
    if (w == 800)
        d2_PicBase = 0;
    else if (w == 0x400)
        d2_PicBase = 1;
    else if (w == 0x480)
        d2_PicBase = 2;
    d2_MainWindow = hwnd;
    d2_ScreenH = h;
    d2_ScreenW = w;
    d2_SetClipRect(0, 0, w, h);
    d2_HalfScreenW = dd_Width / 2;
    d2_HalfScreenH = dd_Height / 2;
    memset(desc, 0, sizeof desc);
    desc[1] = dd_Width;
    desc[2] = dd_Height;
    d2_BackSurf = d2_MakeSurface(desc, 0xb);
    if (!d2_BackSurf)
        return 0;
    memset(desc, 0, sizeof desc);
    desc[1] = 0x100;
    desc[2] = 0x100;
    d2_BackgroundTile = d2_MakeSurface(desc, 3);
    if (!d2_BackgroundTile)
        return 0;
    memset(desc, 0, sizeof desc);
    desc[1] = dd_Width;
    desc[2] = dd_Height;
    desc[8] = (int)dd_BackBuffer;
    d2_ScreenSurf = d2_MakeSurface(desc, 4);
    if (!d2_ScreenSurf)
        return 0;
    memset(desc, 0, sizeof desc);
    desc[1] = dd_Width;
    desc[2] = dd_Height;
    desc[8] = (int)dd_FrontBuffer;
    d2_FrontSurf = d2_MakeSurface(desc, 6);
    if (!d2_FrontSurf)
        return 0;
    d2_GetSurfaceDesc(d2_ScreenSurf, sd0);
    d2_GetSurfaceDesc(d2_BackSurf, sd1);
    if (d2_LockSurface(d2_ScreenSurf)) {
        d2_ClearSurface(d2_ScreenSurf);
        d2_UnlockSurface(d2_ScreenSurf);
    }
    d2_SetFont(d2_FindPic("_FONT"));
    d2_CaptionTex = (D2TEX *)d3if_LoadSprite("BLACK", 0);
    if (!d2_CaptionTex)
        err_Message("d2_Open: Could not init transparency...");
    d2_ScrollMarkerPic = d2_FindPic("_WIN_ICONS");
    d2_FramePic = d2_FindPic("_SLIDER_INPUT");
    d2_DefaultFont = d2_FindPic("_FONT");
    d2_ButtonPulldownPic = d2_FindPic("_BUTTON_PULLDOWN");
    d2_WinBorderPic = d2_FindPic("_WIN_BORDER");
    d2_WinBorderPergaPic = d2_FindPic("_WIN_BORDER_PERGA");
    d2_MiscPic = d2_FindPic("_MISC");
    d2_PointerPicIndex = d2_FindPic("_MOUSE_CURSOR");
    i = d2_FindPic("_EXTRA_SYMBOLS");
    d2_ExtraSymbolsPic = i;
    d2_TextCursorPic = d2_FindPic("_TEXT_CURSOR");
    if (d2_TextCursorPic == -1)
        d2_TextCursorPic = i + 8;
    return 1;
}
/* 0x4741a0, raises the windows of view v to the top of the draw order */

void d2_RaiseViewWindows(int v)
{
    int i;

    for (i = 0; i < d2_Views[v].nwins; i++)
        d2_RaiseWindow(d2_Views[v].wins[i]);
}

/* 0x474b30 in the order of the file: 0x4741f0 first (loads the view), below */


/* 0x4741f0, loads the form file x (forms\x.form) as a view: one window per record, the items (pictures, texts,
   inputs, sliders) become objects; the view is moved to (x, y) unless one of them is 0 and kept inside the screen;
   returns the number of the view, or -1 @deep read whole against the original: fixed the view-scan bound (the signed jge against &d2_Views[0x30].nwins = 0xfb91e4, the array end, not a d3s_Lights address), r = &d2_Forms2[i].h first and &d2_Forms0[i].h for version 0 (the original's default for other versions), the child-window branches first (parent != 0), the alpha test as (flags & 1) && alpha == 0.0f -> 0x3f266666; the k loop (pictures, 0x43 texts, 0x41 inputs, 0x45 sliders), the key text, the version-0 margins / font / picture, the view flags and the move to (x, y) were compared and match. Left: allocation and placement - our VC6 caches the constant 1 in edi ('- push edi' against '+ push 1' at every vfs_fread), the original strength-reduces the records to stepped slots (d2_Forms0 entry [esp+0x34], d2_Forms2 entry + 0xed0 [esp+0x28], wins pointer [esp+0x10]) where ours steps d2_Forms0 + 6 in ebp; the original does not thread its second 'parent' test in version 0 (pw / pww computed, then 'test eax, eax; je' before the two calls at 0x474579), ours does; the two -1 returns of the view scan share one epilogue in the original; frame 0x244 vs 0x248 */
int d2_OpenForm(int x, int y, char *name)
{
    char buf[0x100] = "";
    char msg[0x100];
    char hdr[5];
    unsigned char fl;
    void *f;
    int n, ver, view, i, k, win, pw, o, pic, idx, parent;
    D2WINF *ww, *pww;
    D2FORMH *r;
    int *pn;

    strcpy(buf, name);
    if (d2_FullFormPath == 0) {
        strcpy(buf, d2_FormsDir);
        strcat(buf, name);
        strcat(buf, ".form");
    }
    f = vfs_Tfopen(buf, "rb", vfs_Root);
    if (f == 0) {
        sprintf(msg, "Konnte die Form\n %s\nnicht laden...", name);
        return -1;
    }
    vfs_fread(hdr, 1, 4, f);
    ver = hdr[3] - '0';
    hdr[4] = 0;
    if (ver == 2) {
        vfs_fread(&n, 4, 1, f);
        vfs_fread(d2_Forms2, 0x101c, n, f);
    } else {
        ver = 0;
        vfs_fseek(f, 0, 0);
        vfs_fread(&n, 4, 1, f);
        vfs_fread(d2_Forms0, 0xed4, n, f);
    }
    vfs_fclose(f);
    view = 1;
    if (d2_Views[1].nwins != 0) {
        pn = &d2_Views[1].nwins;
        do {
            if ((int)pn >= (int)&d2_Views[0x30].nwins)  /* 0xfb91e4: the end of the 0x30 views, a signed compare (jge) */
                return -1;
            pn += 0xab;
            view++;
        } while (*pn != 0);
        if (view >= 0x30)
            return -1;
    }
    d2_Views[view].firstObj = d2_HighestObj - 1;
    for (i = 0; i < n; i++) {
        r = &d2_Forms2[i].h;
        if (ver == 0) {
            r = &d2_Forms0[i].h;
            parent = d2_Forms0[i].parent;
            if (parent != 0) {
                pw = ((int *)&d2_Views[view])[parent];
                pww = (D2WINF *)&d2_Wins[pw];
            }
            if (parent != 0)
                win = d2_CreateChildWindow(r->x - pww->x, r->y - pww->y, r->w, r->h, r->flags, pw);
            else
                win = d2_CreateWindow(r->x, r->y, r->w, r->h, r->flags);
            d2_Views[view].wins[i] = win;
        } else if (ver == 2) {
            if (d2_Forms2[i].parent != 0) {
                pw = ((int *)&d2_Views[view])[d2_Forms2[i].parent];
                pww = (D2WINF *)&d2_Wins[pw];
                win = d2_CreateChildWindow(r->x - pww->x, r->y - pww->y, r->w, r->h, r->flags, pw);
            } else
                win = d2_CreateWindow(r->x, r->y, r->w, r->h, r->flags);
            d2_Views[view].wins[i] = win;
            if (d2_Forms2[i].bg[0] != 0)
                d2_SetBackgroundTexture(d2_Forms2[i].bg, d2_LastWindow);
            if ((d2_Wins[d2_LastWindow].flags & 1) && d2_Forms2[i].alpha == 0.0f)
                ((D2WINF *)&d2_Wins[d2_LastWindow])->alpha = 0x3f266666;
            else
                ((D2WINF *)&d2_Wins[d2_LastWindow])->alpha = d2_Forms2[i].alphaBits;
        }
        ww = (D2WINF *)&d2_Wins[d2_Views[view].wins[i]];
        if (ver == 2) {
            char *fn = d2_Forms2[i].font;

            if (fn[0] == 0)
                fn = "_FONT";
            ww->font = d2_FindPic(fn);
            if (d2_Forms2[i].pic[0] != 0) {
                ww->pic = d2_FindPic(d2_Forms2[i].pic);
                d2_AddRefcount(ww->pic);
            }
            ww->m[0] = d2_Forms2[i].m[0];
            ww->m[1] = d2_Forms2[i].m[1];
            ww->m[3] = d2_Forms2[i].m[3];
            ww->m[2] = d2_Forms2[i].m[2];
            if (ww->flags & 4) {
                idx = d2_FindPic(d2_Forms2[i].frame);
                ww->frame = idx;
                if (idx == -1)
                    ww->frame = d2_WinBorderPic;
            }
        }
        k = 0;
        d2_Objs[ww->parent].buf = (D2CTRL *)&d2_Views[view];
        win = d2_Views[view].wins[i];
        d2_Views[view].nwins++;
        for (k = 0; k < r->n; k++) {
            if (r->type[k] < 0x40 && (pic = d2_FindPic(r->name[k])) != -1) {
                o = d2_WinOpenPic(r->ix[k], r->iy[k], pic, win);
                if (r->kind[k] == 0x12) {
                    d2_Objs[o].checkable = 1;
                    d2_Objs[o].interactive = 0;
                }
                if (r->kind[k] == 8) {
                    d2_Objs[o].checkable = 0;
                    d2_Objs[o].interactive = 1;
                }
                d2_Objs[o].buf = (D2CTRL *)&d2_Views[view];
            }
            if (r->type[k] == 0x43) {
                if (strcmp(r->name[k], "") != 0) {
                    idx = txt_FindKey(r->name[k]);
                    if (idx != -1) {
                        o = d2_CreateWinText(r->ix[k], r->iy[k], d2_Strings[idx], win);
                        d2_Objs[o].pic = txt_FindKey(r->name[k]);
                        d2_Objs[o].buf = (D2CTRL *)&d2_Views[view];
                    }
                    continue;
                }
            }
            if (r->type[k] == 0x41) {
                fl = (unsigned char)r->kind[k];
                o = d2_WinOpenInput(r->ix[k], r->iy[k], ((unsigned char *)&r->kind[k])[1],
                                    ((unsigned char *)&r->kind[k])[2], fl, win);
            } else if (r->type[k] == 0x45) {
                fl = (unsigned char)r->kind[k];
                o = d2_WinOpenSlider(r->ix[k], r->iy[k], d2_Forms2[i].slen[k], 0, 100, d2_FindPic(r->name[k]), fl, win);
            } else
                continue;
            d2_Objs[o].buf = (D2CTRL *)&d2_Views[view];
        }
        if (r->key[0] == 0)
            ((int *)ww)[0x90] = -1;
        else {
            idx = txt_FindKey(r->key);
            if (idx != -1)
                strcpy(ww->text, d2_Strings[idx]);
            ((int *)ww)[0x90] = idx;
        }
        if (ver == 0) {
            short fnt;

            ww->m[1] = d2_Forms0[i].m[1];
            ww->m[0] = d2_Forms0[i].m[0];
            ww->m[2] = d2_Forms0[i].m[2];
            fnt = (short)d2_Forms0[i].font;
            ww->m[3] = d2_Forms0[i].m[3];
            if (fnt == 0)
                fnt = d2_FindPic("_FONT");
            ww->font = fnt;
            if (d2_Forms0[i].bgpic > 0)
                ww->pic = d2_Forms0[i].bgpic;
        }
    }
    d2_Views[view].active = 1;
    d2_Views[view].shown = 1;
    d2_Views[view].enabled = 1;
    d2_Views[view].id = view;
    d2_CurrentView = view;
    d2_FullFormPath = 0;
    strcpy(d2_Views[view].pad22c, name);
    ww = (D2WINF *)&d2_Wins[d2_Views[view].wins[0]];
    if ((short)x != 0 && (short)y != 0)
        d2_MoveObj(x, y, ww->parent);
    if (d2_ScreenW < ww->x + ww->w)
        d2_MoveObj(d2_ScreenW - ww->w, ww->y, ww->parent);
    return view;
}
/* 0x474b30, loads a view and makes it ready: its objects are hidden, the two images are created */

int d2_LoadView(int x, int y, char *name)
{
    int view, i, j;
    D2VIEW *v;
    D2WIN *w;
    int desc[16];

    view = d2_OpenForm(x, y, name);
    if (view != 0) {
    v = &d2_Views[view];
    for (i = 0; i < v->nwins; i++) {
        w = &d2_Wins[v->wins[i]];
        d2_Objs[w->parent].disabled = 1;
        for (j = 0; j < w->nobjs; j++)
            d2_Objs[w->objs[j]].disabled = 1;
    }
    v->loaded = 1;
    w = &d2_Wins[v->wins[0]];
    memset(desc, 0, sizeof(desc));
    desc[1] = w->w;
    desc[2] = w->h;
    v->viewSurf = (int)d2_MakeSurface(desc, 3);
    memset(desc, 0, sizeof(desc));
    desc[1] = w->w;
    desc[2] = w->h;
    v->texSurf = (int)d2_MakeSurface(desc, 3);
    return view;
    }
    return 0;
}
/* 0x474ca0, sets the two textures of a view from two picture names: clears the old textures, fills in the new
   pictures and redraws the view; returns 0 when the view is not active or not loaded, else 1 */

int d2_SetViewTextures(int view, char *tex1, char *tex2)
{
    D2VIEW *v = &d2_Views[view];

    if (v->active == 0)
        return 0;
    if (v->loaded == 0)
        return 0;
    if (tex1 != 0 && tex2 != 0) {
        d2_ClearViewTextures(view);
        d2_FillViewTextures(view, tex1, tex2);
        d2_RedrawView(view);
    }
    return 1;
}
/* 0x474d10, forgets the two textures of a view: their images are cleared */

void d2_ClearViewTextures(int view)
{
    D2VIEW *v = &d2_Views[view];
    int i;

    for (i = 0; i < 2; i++) {
        char *name;
        D2TEXX *t;

        if (i == 0)
            name = v->tex1;
        else if (i == 1)
            name = v->tex2;
        else
            continue;
        if (*name != 0) {
            t = (D2TEXX *)tex_Find(name);
            if (t != 0) {
                unsigned int sz = t->size;

                if (i == 0)
                    d2_BlitPartToDD(0, i, sz, sz, (void *)v->texSurf, 0, 0, t->surf);
                else
                    d2_BlitPartToDD(0, sz, sz, sz, (void *)v->texSurf, 0, 0, t->surf);
            }
        }
    }
    v->tex1[0] = 0;
    v->tex2[0] = 0;
}
/* 0x474d90, copies the two pictures named tex1 / tex2 into the view's two textures and the view's image (one
   above the other) */

void d2_FillViewTextures(int view, char *tex1, char *tex2)
{
    D2VIEW *v = &d2_Views[view];
    D2TEXX *t1 = 0;
    D2TEXX *t2;
    char *s, *d;
    char c;

    if (tex1 != 0) {
        s = tex1;
        d = v->tex1;
        do {
            c = *s;
            d[s - tex1] = c;
            s++;
        } while (c != 0);
        t1 = (D2TEXX *)tex_Find(tex1);
        if (t1 != 0) {
            tex_SetHiColor((D3TEXTURE *)t1, 1);
            d2_BlitPartFromDD(0, 0, t1->size, t1->size, t1->surf, 0, 0, (void *)v->texSurf);
        }
    }
    if (tex2 != 0) {
        s = tex2;
        d = v->tex2;
        do {
            c = *s;
            d[s - tex2] = c;
            s++;
        } while (c != 0);
        t2 = (D2TEXX *)tex_Find(tex2);
        if (t2 != 0) {
            tex_SetHiColor((D3TEXTURE *)t2, 1);
            d2_BlitPartFromDD(0, 0, t2->size, ((D2SURF *)v->texSurf)->h - t1->size, t2->surf, 0, t1->size,
                         (void *)v->texSurf);
        }
    }
    d2_BlitPart(0, 0, ((D2SURF *)v->viewSurf)->w, ((D2SURF *)v->viewSurf)->h, (void *)v->texSurf, 0, 0,
                 (void *)v->viewSurf);
}
/* 0x4754d0, sets D2OBJ::clickDisabled of every object on the windows of a view */

void d2_EnableViewObjects(int view, int on)
{
    D2VIEW *v = &d2_Views[view];
    int i, j;

    if (v->active != 0) {
        for (i = 0; i < v->nwins; i++) {
            D2WIN *w = &d2_Wins[v->wins[i]];

            for (j = 0; j < w->nobjs; j++)
                d2_Objs[w->objs[j]].clickDisabled = (on == 0);
        }
        v->shown = on;
    }
}
/* 0x474e80, draws the windows of a view onto its image and copies the image onto the textures @deep B3 far draft, readability now clean (Q1-Q7: this session named d2_NoBackgroundRestore and D2VIEW.viewX/viewY); 568/592 instructions, 114 differing (code 114, regs 36, order 18, fill 8). Every hunk read: 4751e5 (clip-rect loads reordered, original edx/eax/ecx vs ours eax/ecx/edx in the 5/8 caption block), 47529e / 47542a / 4754c8 (case 0x43 text-call h/w load order and the two caption tails), 4753cb (no menu rule: ours hoists the test, original branches first), 475403 (reload: original re-reads [esp+0x24] into esi, ours keeps it). spell.py menu read and its prescriptions (dup-tail x4, reload) are the same statement-order / local-form edits the d2m note tried with no effect (18 permutations of the x/w/y statement order, locals for font/width, reuse of x/y, 5 tw*3 spellings, 3 local sizes for 0x43, 2 declaration orders, int mode; reverted). Remaining: whole-function register allocation and scheduling -> deep toolbox (listing/slotmap/vargen) */

void d2_RedrawView(int view)
{
    D2VIEW *v = &d2_Views[view];
    D2OBJ *o;
    D2WIN *w;
    D2TEXX *t;
    void *surf;
    int i, j, x, y, tx, ty, tw, th;
    unsigned int mode;

    if (v->active != 0 && v->loaded != 0) {
        d2_RestoreShapes((D2BUF *)v);
        surf = (void *)v->viewSurf;
        d2_LockSurface(surf);
        for (i = 0; i < v->nwins; i++) {
            int win = v->wins[i];

            w = &d2_Wins[win];
            d2_DrawWindow(win, surf);
            if (d2_Objs[w->parent].win == 0)
                d2_AddShape(w->x - v->viewX, w->y - v->viewY, w->w, w->h, (D2BUF *)v);
            for (j = 0; j < w->nobjs; j++) {
                o = &d2_Objs[w->objs[j]];
                x = o->x - v->viewX;
                y = o->y - v->viewY;
                if (o->used != 0) {
                    d2_SetClipRect(o->clipx0, o->clipy0, o->clipx1, o->clipy1);
                    switch (o->type) {
                    case 1:
                        d2_DrawShape(x, y, surf, o->data, 0);
                        if (o->highlighted != 0)
                            d2_DrawShapeMode3(x, y, surf, o->data, 0);
                        if (o->win == 0)
                            d2_AddShape(x, y, d2_ShpW, d2_ShpH, (D2BUF *)v);
                        break;
                    case 0x11:
                        if (o->drawFrames > 0 && d2_NoBackgroundRestore != 1)
                            o->drawFrames--;
                        break;
                    case 4:
                        d2_DrawAnim(o->input, surf);
                        if (o->win == 0)
                            d2_AddShape(x, y, d2_ShpW, d2_ShpH, (D2BUF *)v);
                        break;
                    case 5:
                    case 8:
                        if (o->style & 2) {
                            if (&d2_Objs[d2_PointerObj] == o || o->checked != 0) {
                                if (o->style & 1)
                                    o->highlighted = 1;
                            } else if (o->style & 1)
                                o->highlighted = 0;
                        }
                        if (o->textShadow != 0)
                            d2_DrawShapeMode2(x + 8, y + 8, surf, o->data, (unsigned char)o->input);
                        d2_DrawShape(x, y, surf, o->data, (unsigned char)o->input);
                        if (o->highlighted != 0)
                            d2_DrawShapeMode3(x, y, surf, o->data, (char)o->input);
                        if (o->win == 0) {
                            if (o->textShadow != 0)
                                d2_AddShape(x, y, d2_ShpW + 8, d2_ShpH + 8, (D2BUF *)v);
                            else
                                d2_AddShape(x, y, d2_ShpW, d2_ShpH, (D2BUF *)v);
                        }
                        if (o->u.text[0] != 0) {
                            int ex, ey;

                            if (o->font != 0)
                                d2_SetFont(o->font);
                            mode = 0;
                            if (o->highlighted)
                                mode = 2;
                            else if (o->greyed)
                                mode = 1;
                            if (o->centred != 0)
                                mode |= 8;
                            if (o->rightAligned != 0)
                                mode |= 0x10;
                            d2_GetObjPicSize(o->id, &tw, &th);
                            ex = o->captionX;
                            tx = o->w;
                            if (ex == -1) {
                                ex = o->x + 6 + tw;
                                tx = tx + tw * 3;
                            }
                            ey = o->captionY;
                            if (ey == -1)
                                ey = o->y - 1;
                            d2_DrawTextBox(ex, ey, tx, o->h, surf, o->u.text, mode);
                        }
                        break;
                    case 0x41:
                        d2_DrawInput(o->input, surf);
                        break;
                    case 0x43:
                        if (o->drawFrames > 0) {
                            int font;

                            d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
                            font = d2_CurrentFont;
                            d2_SetFont(o->font);
                            mode = 0;
                            if (o->highlighted)
                                mode = 2;
                            else if (o->greyed)
                                mode = 1;
                            if (o->centred != 0)
                                mode |= 8;
                            d2_DrawTextBox(o->x, o->y, o->w, o->h, surf, (char *)o->input, mode);
                            d2_SetFont(font);
                        }
                        break;
                    case 0x45:
                        d2_SetClipRect(o->clipx0, o->clipy0, o->clipx1, o->clipy1);
                        if (o->win != 0)
                            d2_SetSurfaceClip(o->clipx0, o->clipy0, o->win->w, o->win->h, surf);
                        d2_DrawSlider(o->id, (D2SURF *)surf);
                        if (o->win == 0)
                            d2_AddShape(o->x, o->y - 4, o->w, o->h + 8, (D2BUF *)v);
                        d2_SetSurfaceClip(0, 0, ((D2SURF *)surf)->w, ((D2SURF *)surf)->h, surf);
                        break;
                    }
                }
                d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
            }
        }
        d2_UnlockSurface(surf);
        t = 0;
        if (v->tex1[0] != 0) {
            t = (D2TEXX *)tex_Find(v->tex1);
            if (t != 0)
                d2_BlitPartToDD(0, 0, t->size, t->size, (void *)v->viewSurf, 0, 0, t->surf);
        } else if (v->tex2[0] == 0)
            return;
        if (v->tex2[0] != 0) {
            D2TEXX *t2 = (D2TEXX *)tex_Find(v->tex2);

            if (t2 != 0)
                d2_BlitPartToDD(0, t->size, t2->size, ((D2SURF *)v->viewSurf)->h - t->size, (void *)v->viewSurf, 0, 0,
                             t2->surf);
        }
    }
}
/* 0x475590, enables / disables the objects of a view */

void d2_SetViewEnabled(int view, int on)
{
    D2VIEW *v = &d2_Views[view];
    int i;

    if (v->active) {
        for (i = 0; i < 0x300; i++) {
            if (d2_Objs[i].type != 0 && d2_Objs[i].buf == (D2CTRL *)v)
                d2_EnableObject(i, on);
        }
        v->enabled = on;
    }
}
/* 0x475600, centres the windows of a view */

void d2_CentreViewWindows(int view)
{
    D2VIEW *v = &d2_Views[view];
    int i;

    for (i = 0; i < v->nwins; i++) {
        D2WIN *w = &d2_Wins[v->wins[i]];
        if (d2_Objs[w->parent].win == 0)
            d2_MoveObj(d2_HalfScreenW - w->w / 2, d2_HalfScreenH - w->h / 2, w->parent);
    }
}
/* 0x4756b0, places a window: flags 1 = centred in x, 2 = centred in y @deep B3 draft, readable; 57/52 instructions, 27 differing. All hunks read: the whole difference is the parent load - ours hoists mov edx,[ecx+0x26c] (w->parent) above test bl,2 and folds the two d2_MoveObj tails (push edx once), the original re-reads w->parent inside each arm between the y computation and the pushes (menu: reload at 4756e8, dup-tail at 475728). Tried per the d2n note with no change: locals per branch, early return, d2_Wins[win] / D2VOP forms for one or both parent reads, re-taking w, dummy volatile; the menu prescriptions are the same class. x/y computation and the flags meaning match. Remaining: VC6 CSE / tail-merge scheduling -> deep (listing/slotmap) */

void d2_PlaceWindow(int win, unsigned char flags)
{
    D2WIN *w = &d2_Wins[win];
    int x;

    if (flags & 1)
        x = d2_HalfScreenW - w->w / 2;
    else
        x = w->x;
    if (flags & 2)
        d2_MoveObj(x, d2_HalfScreenH - w->h / 2, w->parent);
    else
        d2_MoveObj(x, w->y, w->parent);
}
/* 0x475730, the same with float coordinates, kept on the screen */

void d2_PlaceWindowScaled(win, fl)
int win;
int fl;
{
    D2WIN *w = &d2_Wins[win];
    float x, y;
    int h, h2;
    unsigned char flags = (unsigned char)fl;

    if (flags & 1) {
        h = w->w / 2;
        x = (float)(w->x + h) - d2_HalfScreenW / d2_ScreenScale + d2_HalfScreenW - h;
    } else {
        x = w->x;
    }
    if (flags & 2) {
        h2 = w->h / 2;
        y = (float)(w->y + h2) * d2_ScreenScale - h2;
    } else {
        y = w->y;
    }
    if ((float)(d2_ScreenW - w->w) < x)
        x = (float)(d2_ScreenW - w->w);
    y = min((float)(d2_ScreenH - w->h), y);
    d2_MoveObj((int)x, (int)y, w->parent);
}
/* 0x475850, places the view's first window (the form's main window) with the placement flags, screen-scaled */

void d2_PlaceMainWindow(int view)
{
    d2_PlaceWindowScaled(d2_Views[view].wins[0]);
}
/* 0x475870, places every window of a view that is not a child window, flags as in d2_PlaceWindow */

void d2_PlaceViewWindows(int view, unsigned char flags)
{
    D2VIEW *v = &d2_Views[view];
    int i;

    for (i = 0; i < v->nwins; i++) {
        if (d2_Objs[d2_Wins[v->wins[i]].parent].win == 0)
            d2_PlaceWindow(v->wins[i], flags);
    }
}
/* 0x4758f0, closes a view. The view is reached through D2VOP(view * 0x2ac), the original's byte-offset
   form: the typed index access d2_Views[view] loses the byte match (the offset stays in a register). */

void d2_CloseView(int view)
{
    D2VIEW *v;
    int i;

    if (view <= 0x30 || view >= 1) {
        v = D2VOP(view * 0x2ac);
        if (D2VOP(view * 0x2ac)->active) {
            for (i = D2VOP(view * 0x2ac)->nwins - 1; i >= 0; i--)
                d2_RemoveWindow(d2_Views[view].wins[i]);
            if (v->viewSurf)
                d2_FreeSurface((void *)v->viewSurf);
            if (v->texSurf)
                d2_FreeSurface((void *)v->texSurf);
            memset(v, 0, sizeof(D2VIEW));
        }
    }
}
/* 0x475980, value of an input / slider / counter object; -1 if it has none */

int d2_GetObjValue(int obj)
{
    D2OBJ *o = &d2_Objs[obj];
    D2INPUT *in;

    if (o->type == 0x41) {
        in = (D2INPUT *)o->data;
        if (in->flags & 0x10)
            return in->currentTextId;
        if (in->flags & 1)
            return (int)in->text;
        if (in->flags & 2)
            return in->value;
    }
    if (o->checkable)
        return o->checked;
    if (o->type == 0x45)
        return o->u.s.value;
    return -1;
}
/* 0x4759f0, size of a picture object; 0 if it has none. The picture's per-frame offset table starts at
   byte 0x45 of the picture header; its entry for the object's input holds the frame's offset. */

int d2_GetObjPicSize(int obj, int *w, int *h)
{
    D2OBJ *o;
    char *p;
    int *frameOffsets;
    D2FRAME *f;

    if (obj != -1) {
        o = &d2_Objs[obj];
        if (o->type == 1 || o->type == 8 || o->type == 5) {
            p = (char *)d2_GetPic(o->pic);
            if (p) {
                if (o->type != 8 && o->type != 5) {
                    *w = ((D2PICHDR *)p)->w;
                    *h = ((D2PICHDR *)p)->h;
                    return 1;
                }
                frameOffsets = (int *)(p + 0x45);
                f = (D2FRAME *)(p + frameOffsets[o->input]);
                if (f) {
                    *w = f->w;
                    *h = f->h;
                }
                return 1;
            }
        }
    }
    return 0;
}
/* 0x475aa0, marks an object and everything on it as disabled (on = 0) or enabled */

void d2_EnableObject(int obj, int on)
{
    D2OBJ *o = &d2_Objs[obj];
    D2WIN *w;
    int i;

    o->disabled = (on == 0);
    if (o->type == 0x40) {
        w = &d2_Wins[o->input];
        for (i = 0; i < w->nobjs; i++)
            d2_EnableObject(w->objs[i], on);
        if (w->scrollBar != -1)
            d2_EnableObject(w->scrollBar, on);
        if (w->cornerObj != -1)
            d2_EnableObject(w->cornerObj, on);
        if (w->extraObj != -1)
            d2_EnableObject(w->extraObj, on);
    }
}
/* 0x475b80, k-th object of window idx (of a view if view != -1); -1 if there is none */

int d2_GetWinObj(int view, int idx, int k)
{
    D2WIN *w;

    if (view != -1)
        w = &d2_Wins[d2_Views[view].wins[idx]];
    else
        w = &d2_Wins[idx];
    if (k > w->nobjs)
        return -1;
    return w->objs[k];
}
/* 0x475bf0, step of a counter by its range */

int d2_InputStep(D2INPUT *in)
{
    int r = 1;
    int d = in->maxval - in->minval;

    if (d >= 125000)
        return 10000;
    if (d >= 100000)
        return 5000;
    if (d >= 75000)
        return 4000;
    if (d >= 50000)
        return 3000;
    if (d >= 37500)
        return 2000;
    if (d >= 25000)
        return 1500;
    if (d >= 12500)
        return 1000;
    if (d >= 5000)
        return 500;
    if (d >= 2500)
        return 200;
    if (d >= 1000)
        return 100;
    if (d >= 250)
        return 50;
    if (d >= 100)
        r = 10;
    return r;
}
/* 0x475cb0, sets the value (and range) of an input / slider / counter object */

void d2_SetObjValue(int obj, int a, int b, int c, int d)
{
    D2OBJ *o;
    D2INPUT *in;
    int k, mn, mx, v;
    unsigned int fl;

    if (obj == -1)
        return;
    if (obj > 0x300)
        return;
    o = &d2_Objs[obj];
    if (o->type == 0x41) {
        in = (D2INPUT *)o->data;
        if (in->flags & 1) {
            if (strlen((char *)a) > 0xfe)
                return;
            strcpy(in->text, (char *)a);
            return;
        }
        if (in->flags & 2) {
            in->minval = a;
            in->maxval = b;
            in->value = c;
            _itoa(c, in->text, 10);
            fl = in->flags;
            if (!(fl & 0x80))
                return;
            k = d2_InputStep(in);
            in->maxval = in->maxval - (in->maxval - in->minval) % k;
            in->minval = (in->maxval - in->minval) % k + in->minval;
            in->value = (in->value / k) * k;
            if ((in->maxval - in->minval) / k > 0x19)
                in->maxval = k * 0x19;
            if (in->value == -1 && (fl & 0x40))
                return;
            v = in->minval;
            if (in->minval <= in->value)
                v = in->value;
            in->value = v;
            return;
        }
    }
    if (o->type == 0x45) {
        o->u.s.min = a;
        o->u.s.max = b;
        o->u.s.value = c;
        fl = o->u.s.flags;
        if (fl & 0x10) {
            o->u.s.value2 = d;
            return;
        }
    } else if (o->checkable != 0 || o->interactive != 0) {
        o->checked = a & 0xff;
        o->highlighted = a & 0xff;
    }
}
/* 0x475e40, enables / disables an object: clickDisabled and greyed are set when on is 0; obj -1 is ignored */

void d2_SetObjEnabled(int obj, int on)
{
    if (obj != -1) {
        d2_Objs[obj].clickDisabled = (on == 0);
        d2_Objs[obj].greyed = (on == 0);
    }
}
/* 0x475e80, scales a picture object: a > 1 times, a < -1 times smaller */

void d2_ScaleObject(int obj, int a)
{
    int w, h, n;

    if (obj != -1) {
        if (d2_GetObjPicSize(obj, &w, &h) == 0) {
            d2_GetObjPicSize(obj, &w, &h);
            return;
        }
        d2_Objs[obj].picScale = a;
        if (a > 1) {
            d2_Objs[obj].w = (short)w * (short)a;
            d2_Objs[obj].h = (short)h * (short)a;
        }
        if (a < -1) {
            n = abs(a);
            d2_Objs[obj].w = (short)(w / n);
            d2_Objs[obj].h = (short)(h / n);
        }
    }
}
/* 0x475f40, sets the maximum value (the range) of the input / slider object obj belongs to */

void d2_SetObjMax(int obj, int v)
{
    d2_Inputs[d2_Objs[obj].input].maxval = v;
}
/* 0x475f70, selects the current window by its number in a view */

int d2_SetForm(int view, int n)
{
    if (view != 0)
        d2_CurrentView = view;
    if (n <= d2_Views[d2_CurrentView].nwins && n >= 0) {
        d2_LastWindow = d2_Views[d2_CurrentView].wins[n];
        d2_LastWindowPtr = &d2_Wins[d2_Views[d2_CurrentView].wins[n]];
        return 1;
    }
    err_Message("d2_SetForm(): invalid win number");
    return 0;
}

#define WIN(i) ((D2PAINTWIN *)&d2_Wins[i])

/* 0x475ff0, window i of a view, -1 if out of range */
int d2_GetViewWin(int view, int i)
{
    if (i <= d2_Views[view].nwins && i >= 0)
        return d2_Views[view].wins[i];
    return -1;
}
/* 0x476030, selects the font; 0 if the picture is no font */

void *d2_SetFont(int font)
{
    if (d2_Pics[font].type != 7)
        return 0;
    d2_FontPic = (D2PICHDR *)d2_GetPic(font);
    d2_CurrentFont = font;
    d2_FontHeight = d2_FontPic->h;
    if (font == d2_DefaultFont || font == d2_DefaultFont + 2)
        d2_SpaceWidth = 8;
    else
        d2_SpaceWidth = 4;
    d2_LetterGap = 2;
    return d2_FontPic;
}
/* 0x4760b0, sets the font of an object (and of its window / input) */

void d2_SetObjFont(int obj, int font)
{
    D2OBJ *o = &d2_Objs[obj];

    o->font = (short)font;
    switch (o->type) {
    case 0x40:
        d2_Wins[o->input].font = (short)font;
        break;
    case 0x41:
        d2_Inputs[o->input].font = font;
        break;
    }
}
/* 0x476110, one step of the dissolve of a picture onto the screen */

void d2_DissolveStep(D2DISSOLVE *d)
{
    int i;
    unsigned short *dst;

    if (d != 0 && d->mask != 0) {
        d->step += 8;
#ifdef REA_FRAME_DUMP
        REA_DUMP_TRACE((rd_f, "%u Dissolve step=%d %dx%d at %d,%d pitchpix=%d\n", GetTickCount(), d->step, d->w, d->h,
                        d->x, d->y, dd_PitchPixels));
#endif
        if (d->step <= 0xff) {
            d2_SetBlitParams(((D2SURF *)d2_ScreenSurf)->bits + d->y * dd_PitchPixels + d->x, d->src, d->mask,
                         (short)d->w, (short)d->h, (short)d->step);
        } else {
            for (i = 0; i < d->h; i++) {
                dst = ((D2SURF *)d2_ScreenSurf)->bits + (d->y + i) * dd_PitchPixels + d->x;
                memcpy(dst, (unsigned short *)d->src + d->w * i, d->w * 2);
            }
        }
        if (d->step > 0x120) {
            if (d->mask != 0) {
                mem_Free(d->mask);
                d->mask = 0;
            }
            if (d->src != 0) {
                mem_Free(d->src);
                d->src = 0;
            }
            d2_RemoveObj(d->obj);
            mem_Free(d);
            d2_CurrentDissolve = 0;
        }
    }
}
/* 0x476220, plots a point (pen size 0, 1 or 2) on the paintbox of the current window */

void d2_Plot(int x, int y, int r, int g, int b)
{
    D2PAINTWIN *w = WIN(d2_LastWindow);
    int pen;

    if (w->active == 0)
        return;
    if (w->paint == 0) {
        err_Message("d2_Plot:Window is not a paintbox!");
        return;
    }
    pen = d2_PenSize & 0xff;
    if (x < pen || x > w->w - pen || y < pen || y > w->h - pen)
        return;
    d2_LockSurface(w->paint);
    switch (d2_PenSize & 0xff) {
    case 0:
        d2_PutPixelRGB(x, y, r, g, b, w->paint);
        break;
    case 1:
        d2_PutPixelRGB(x - 1, y, r, g, b, w->paint);
        d2_PutPixelRGB(x + 1, y, r, g, b, w->paint);
        d2_PutPixelRGB(x, y + 1, r, g, b, w->paint);
        d2_PutPixelRGB(x, y - 1, r, g, b, w->paint);
        d2_PutPixelRGB(x, y, r, g, b, w->paint);
        break;
    case 2:
        d2_PutPixelRGB(x - 1, y, r, g, b, w->paint);
        d2_PutPixelRGB(x + 1, y, r, g, b, w->paint);
        d2_PutPixelRGB(x - 2, y, r, g, b, w->paint);
        d2_PutPixelRGB(x + 2, y, r, g, b, w->paint);
        d2_PutPixelRGB(x, y, r, g, b, w->paint);
        d2_PutPixelRGB(x, y + 1, r, g, b, w->paint);
        d2_PutPixelRGB(x, y - 1, r, g, b, w->paint);
        d2_PutPixelRGB(x, y + 2, r, g, b, w->paint);
        d2_PutPixelRGB(x, y - 2, r, g, b, w->paint);
        d2_PutPixelRGB(x + 1, y + 1, r, g, b, w->paint);
        d2_PutPixelRGB(x - 1, y - 1, r, g, b, w->paint);
        d2_PutPixelRGB(x - 1, y + 1, r, g, b, w->paint);
        d2_PutPixelRGB(x + 1, y - 1, r, g, b, w->paint);
        break;
    }
    d2_UnlockSurface(w->paint);
}
/* 0x4764f0, line on the paintbox (Bresenham) @deep Read whole function against original: axis-parallel cases, sign/delta setup, and both Bresenham loops now mirror it (increments straight/diagonal precomputed before the loop, if (e >= 0) step+diagonal else straight; instruction count 226 = 226). Remaining hunks are allocation: the second vertical loop keeps its counter in the y1 parameter slot (ours: dec eax; mov [esp+0x28], eax) where the original copies it to edi; the original keeps straight/diagonal in eax/ecx at loop top and stores them at [esp+0x20]/[esp+0x1c] (ours [esp+0x1c]/[esp+0x10], loading one inside the branch); x0/dx in ebx/ebp swapped. y0+=sy before/after e+= flips VC6 into tail-merging the add (code 192 -> 156 with e first). */

void d2_PlotLine(int x0, int y0, int x1, int y1, int r, int g, int b)
{
    int e, straight, diagonal, sx, sy, dx, dy;

    if (y0 == y1) {
        if (x1 < x0) {
            x0 = x0 - x1;
            if (0 < x0) {
                do {
                    d2_Plot(x1++, y0, r, g, b);
                } while (--x0 != 0);
                return;
            }
        } else {
            x1 = x1 - x0;
            if (0 < x1) {
                do {
                    d2_Plot(x0++, y0, r, g, b);
                } while (--x1 != 0);
                return;
            }
        }
    } else if (x0 == x1) {
        if (y1 < y0) {
            y0 = y0 - y1;
            if (0 < y0) {
                do {
                    d2_Plot(x1, y1++, r, g, b);
                } while (--y0 != 0);
                return;
            }
        } else {
            y1 = y1 - y0;
            if (0 < y1) {
                do {
                    d2_Plot(x1, y0++, r, g, b);
                } while (--y1 != 0);
                return;
            }
        }
    } else {
        if (x0 < x1) {
            sx = 1;
            dx = x1 - x0;
        } else {
            sx = -1;
            dx = x0 - x1;
        }
        if (y0 < y1) {
            sy = 1;
            dy = y1 - y0;
        } else {
            sy = -1;
            dy = y0 - y1;
        }
        d2_Plot(x0, y0, r, g, b);
        if (dx > dy) {
            straight = dy * 2;
            diagonal = (dy - dx) * 2;
            e = straight - dx;
            do {
                x0 += sx;
                if (e >= 0) {
                    e += diagonal;
                    y0 += sy;
                } else {
                    e += straight;
                }
                d2_Plot(x0, y0, r, g, b);
            } while (x0 != x1);
            return;
        }
        straight = dx * 2;
        diagonal = (dx - dy) * 2;
        e = straight - dy;
        do {
            y0 += sy;
            if (e >= 0) {
                e += diagonal;
                x0 += sx;
            } else {
                e += straight;
            }
            d2_Plot(x0, y0, r, g, b);
        } while (y0 != y1);
    }
}
/* 0x476710, clears the paintbox of the current window */

void d2_ClearPaintbox(void)
{
    D2PAINTWIN *w = WIN(d2_LastWindow);

    if (w->active == 0) {
        err_Message("d2_ClearPaintbox:Invalidate window!");
        return;
    }
    if (w->paint == 0) {
        err_Message("d2_ClearPaintbox:Window has no paintbox!");
        return;
    }
    d2_ClearSurface(w->paint);
}
/* 0x476760, fills a rectangle of the paintbox of the current window */

void d2_FillPaintbox(int x, int y, int w, int h)
{
    D2PAINTWIN *win = WIN(d2_LastWindow);

    if (win->active == 0) {
        err_Message("d2_ClearPaintbox:Invalidate window!");
        return;
    }
    if (win->paint == 0) {
        err_Message("d2_ClearPaintbox:Window has no paintbox!");
        return;
    }
    d2_ClearRect(x, y, w, h, win->paint);
}
/* 0x4767d0, starts a fade of a sprite over a rectangle; NULL if all 0x20 slots are used @deep B3 close, readable: 83/144 bytes, 9 differing instructions, ONE code hunk left plus fill. The whole difference is the inner 'if (i >= 0x20) return 0' epilogue: the original keeps a second out-of-line xor eax,eax / pop edi / ret (jge to the shared end block not taken), ours jumps to the loop-exhausted return block (jge L65659). Reading verified: the do/while slot scan, the redundant i >= 0x20 check, mem_Alloc('d2_fadeinfo'), the fields x/y/w/h at 0x44..0x50, flags, dur 0x54, sprite = d3if_LoadSprite(name, 1), start/last = tb_Calls, the store into d2_Fades[i], return f. Gained this session: the struct-order assignments f->x, f->y, f->w, f->h (21 -> 9 differing instructions). Menu tried: the 'empty then / write the block inline' form if (i < 0x20) {body} -> 98 bytes / 11 instructions, reverted; 'write the block inline vs after the branch' is the same epilogue-placement decision. Remaining: VC6 block placement / tail-merge of the two return 0s -> deep (listing/c2prio) */

D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags)
{
    D2FADE *f;
    int i;

    i = 0;
    do {
        if (d2_Fades[i] == 0) {
            if (i >= 0x20)
                return 0;
            f = (D2FADE *)mem_Alloc(100, "d2_fadeinfo");
            f->x = x;
            f->y = y;
            f->w = w;
            f->h = h;
            f->flags = flags;
            f->dur = dur;
            f->sprite = (D2TEX *)d3if_LoadSprite(name, 1);
            f->start = tb_Calls;
            f->last = tb_Calls;
            d2_Fades[i] = f;
            return f;
        }
        i++;
    } while (i < 0x20);
    return 0;
}
/* 0x476860, frees a fade */

void d2_ReleaseFade(D2FADE *f)
{
    int i;

    for (i = 0; i < 0x20; i++) {
        if (d2_Fades[i] == f) {
            d2_Fades[i] = 0;
            d3if_FreeSprite((int)f->sprite);
            mem_Free(f);
            return;
        }
    }
}
/* 0x4768a0, one frame of a fade: draws the sprite with the alpha of the elapsed time */

void d2_DrawFade(D2FADE *f, D2SURF *surf)
{
    float e, a;
    int on, cx, cy;

    on = 0;
    e = (float)(tb_Calls - f->start);
    if (tb_Calls - f->last > 3)
        e = (float)(int)(f->last - f->start + 3);
    if (f->flags & 1)
        a = e / (float)(int)f->dur;
    else if (f->flags & 2)
        a = 1.0f - e / (float)(int)f->dur;
    if (!(a > 0.0f))
        a = 0.0f;
    else if (!(a < 1.0f))
        a = 1.0f;
    cx = f->w / 2 + f->x;
    cy = f->h / 2 + f->y;
    if (surf->locked != 0) {
        d2_UnlockSurface(surf);
        on = 1;
    }
    d3if_DrawSprite(f->sprite, cx, cy, 0, (float)f->w / (float)f->sprite->size, (float)f->h / (float)f->sprite->size, a);
    if (on)
        d2_LockSurface(surf);
    if ((f->flags & 4) && (f->flags & 8)) {
        if (f->w + f->x > d2_DirtyRectRight)
            d2_AddDirty(d2_DirtyRectRight, 0, d2_ScreenW - d2_DirtyRectRight, d2_ScreenH);
        if (f->y < d2_DirtyRectTop)
            d2_AddDirty(d2_DirtyRectRight, f->y, d2_ScreenW - d2_DirtyRectRight, d2_DirtyRectTop - f->y);
        if (f->h + f->y > d2_DirtyRectBottom)
            d2_AddDirty(d2_DirtyRectRight, d2_DirtyRectBottom, d2_ScreenW - d2_DirtyRectRight, f->h + f->y - d2_DirtyRectBottom);
        d2_ReleaseFade(f);
        return;
    }
    if (((f->flags & 1) && a == 1.0) || ((f->flags & 2) && a == 0.0))
        f->flags |= 4;
    if (f->w + f->x > d2_DirtyRectRight)
        d2_AddDirty(d2_DirtyRectRight, 0, d2_ScreenW - d2_DirtyRectRight, d2_ScreenH);
    if (f->y + f->h > d2_DirtyRectBottom)
        d2_AddDirty(0, d2_DirtyRectBottom, d2_ScreenW, f->y + f->h - d2_DirtyRectBottom);
    f->last = tb_Calls;
}
/* 0x476b10, draws all fades */

void d2_DrawFades(void)
{
    int i;

    for (i = 0; i < 0x20; i++)
        if (d2_Fades[i] != 0) {
#ifdef REA_FRAME_DUMP
            REA_DUMP_TRACE((rd_f, "%u Fade %d\n", GetTickCount(), i));
#endif
            d2_DrawFade(d2_Fades[i], d2_ScreenSurf);
        }
}
/* 0x476b40, frees all fades */

void d2_FreeFades(void)
{
    int i;

    for (i = 0; i < 0x20; i++)
        if (d2_Fades[i] != 0) {
            d2_ReleaseFade(d2_Fades[i]);
            d2_Fades[i] = 0;
        }
}
/* 0x476b70, clears the pending input state */

void d2_ClearInput(void)
{
    d2_ClickedId = -1;
    di_ClearButtons();
    kb_Last = 0;
}
