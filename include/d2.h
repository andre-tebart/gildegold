struct VFSFILE;
/* Types and declarations of d2.c (0x4687a0..0x473c10..): the "d2" 2D GUI layer (memory tag "d2:d2_LoadObj", messages
   "d2_Show"): picture cache, object list, windows, dirty rectangle lists, inputs, sliders, buttons.
   Names are ours. */
#ifndef D2_H
#define D2_H
#include "d3.h"                       /* the D3 records and typedefs this header names */
struct D3OBJECT;

struct IDirectDrawSurface4;          /* <ddraw.h>, for the surface prototypes below */
struct _DDSURFACEDESC2;
#include "d2_types.h"

typedef struct D2PICHDR {           /* start of the data of a picture (what d2_GetPic returns); also a font */
    char pad00[0x2c];
    unsigned short w, h;            /* 0x2c; h of a font is its line height */
    char pad30[4];
    unsigned char bpp;              /* 0x34, depth of the palette (0 = none) */
} D2PICHDR;

typedef struct D2FRAME {            /* a frame of a picture (d2_GetFrame), or a glyph of a font */
    char pad0[6];
    unsigned short w;               /* 0x06 */
    char pad8[2];
    unsigned short h;               /* 0x0a */
    char pad0c[0xa];
    unsigned short kern;            /* 0x16, glyph: overlap with the previous one */
    char pad18[2];
    unsigned short adv;             /* 0x1a, glyph: advance */
} D2FRAME;

typedef struct D2PICHDR11 {         /* start of the data of a type 0x11 picture */
    char pad00[0xc];
    unsigned short w, h;            /* 0x0c */
} D2PICHDR11;

typedef struct D2RECT { int x, y, w, h; } D2RECT;

struct D2BUF;

typedef struct D2SHAPE {            /* 0x14 bytes, a rectangle that belongs to an object */
    int x, y;
    struct D2BUF *owner;            /* 0x08 */
    int w, h;
} D2SHAPE;

typedef struct D2BUF {              /* a drawing surface with its picture buffers (the D2VIEW offsets) */
    char pad000[0x190];
    int active;                     /* 0x190, nonzero: the buffer takes shapes (D2VIEW.active) */
    char pad194[4];
    int loaded;                     /* 0x198, nonzero: its two surfaces exist (D2VIEW.loaded) */
    char pad19c[8];
    D2SURF *dstSurf;                /* 0x1a4, where the saved background is restored to */
    D2SURF *srcSurf;                /* 0x1a8, the surface the shapes were drawn on */
} D2BUF;

#pragma pack(push, 1)
typedef struct D2SND {              /* 0x11 bytes, d2_AnimView[]: what a type 4 object (D2OBJ::input) plays */
    int unk00;                      /* 0x00 */
    char unk04;
    short x, y;                     /* 0x05 */
    char pad09[8];
} D2SND;
#pragma pack(pop)

typedef struct D2RGB { unsigned char r, g, b; } D2RGB;

typedef struct D2SEG {              /* 0x10c bytes, one piece of the text being laid out, d2_TextSegs[] */
    char text[0x100];               /* "$H" "$M" "$T" "$[" "$]" are commands */
    union {
        struct { unsigned char r, g, b; };  /* 0x100 */
        D2RGB col;                  /* 0x100 */
    };
    char pad103;
    int flags;                      /* 0x104, 0x40: coloured */
    short w;                        /* 0x108, width */
    char pad10a[2];
} D2SEG;

/* data */
/* views, form files, fades (0x4741f0..0x476c00, round 18) */
typedef struct D2TEXX {             /* full view of D2TEX */
    char pad00[0x60];
    void *surf;                     /* 0x60, the texture's DirectDraw surface (D3FLTEX.surf) */
    char pad64[0x10];
    unsigned int size;              /* 0x74 */
} D2TEXX;

typedef struct D2FORMH {            /* start of one window of a form file, both versions */
    short x, y, w, h;               /* 0x00 */
    unsigned int flags;             /* 0x08 */
    short ix[0x30];                 /* 0x0c, x of the items */
    short iy[0x30];                 /* 0x6c, y of the items */
    short n;                        /* 0xcc, number of items */
    short pad0ce;
    int type[0x30];                 /* 0xd0, kind of an item: < 0x40 picture, 0x41 input, 0x43 text, 0x45 slider */
    char name[0x30][0x40];          /* 0x190, picture / text key of an item */
    int kind[0x30];                 /* 0xd90, byte 0: flags, 1: w, 2: h of an input; 0x12 / 8: picture kinds */
    char key[0x40];                 /* 0xe50, text key of the window text */
} D2FORMH;

typedef struct D2FORM0 {            /* 0xed4 bytes, version 0 */
    D2FORMH h;
    int font;                       /* 0xe90 */
    int parent;                     /* 0xe94, 0 or the number of the window in the view this one is in */
    int bgpic;                      /* 0xe98, background picture of the window (D2WIN.bgpic) */
    short m[4];                     /* 0xe9c, text margins */
    char pad0ea4[0x30];
} D2FORM0;

typedef struct D2FORM2 {            /* 0x101c bytes, version 2 */
    D2FORMH h;
    char font[0x40];                /* 0xe90, name of the font */
    int parent;                     /* 0xed0 */
    char pic[0x40];                 /* 0xed4, name of a picture (kept with a reference) */
    short m[4];                     /* 0xf14, text margins */
    unsigned char slen[0x30];       /* 0xf1c, length of a slider item */
    char bg[0x40];                  /* 0xf4c, background picture of the window */
    union {
        float alpha;                /* 0xf8c */
        int alphaBits;              /* 0xf8c, the same bytes as an int (stored into D2WINF.alpha) */
    };
    char frame[0x40];               /* 0xf90, frame picture */
    char pad0fd0[0x4c];
} D2FORM2;

typedef struct D2WINF {             /* full view of D2WIN (0x3c4 bytes, d2_Wins) */
    int unk00;
    short x, y, w, h;               /* 0x04 */
    unsigned int flags;             /* 0x0c */
    short m[4];                     /* 0x10, text margins */
    int *objs;                      /* 0x18 */
    short nobjs;                    /* 0x1c */
    char pad1e[0xe];
    char *text;                     /* 0x2c */
    char pad30[0x210];
    int keyidx;                     /* 0x240 */
    char pad244[0x28];
    int parent;                     /* 0x26c */
    char pad270[8];
    int frame;                      /* 0x278 */
    short font;                     /* 0x27c */
    char pad27e[0x10a];
    int pic;                        /* 0x388 */
    char pad38c[8];
    int alpha;                      /* 0x394 */
    char pad398[0x2c];
} D2WINF;

typedef struct D2DISSOLVE {         /* 0x20 bytes, a picture being copied to the screen in steps (d2_DissolveStep) */
    void *mask;                     /* 0x00 */
    void *src;                      /* 0x04, pixels */
    int x, y;                       /* 0x08 */
    int w, h;                       /* 0x10 */
    int obj;                        /* 0x18 */
    int step;                       /* 0x1c */
} D2DISSOLVE;

#define D2VOP(o) ((D2VIEW *)((char *)d2_Views + (o)))

typedef struct D2FADE {             /* 100 bytes, d2_fadeinfo */
    unsigned int flags;             /* 0x00, 1 = in, 2 = out, 4 = done, 8 = remove when done */
    int pad04[0x10];
    int x, y, w, h;                 /* 0x44 */
    unsigned int dur;               /* 0x54 */
    unsigned int start;             /* 0x58 */
    unsigned int last;              /* 0x5c */
    D2TEX *sprite;                  /* 0x60 */
} D2FADE;

typedef struct D2FRCT { int id, amount, unk; } D2FRCT;

typedef struct D2PAINTWIN {         /* D2WIN as far as the paintbox functions use it */
    int unk00;
    short x, y, w, h;               /* 0x04 */
    char pad0c[0x1c];
    void *paint;                    /* 0x28 */
    char pad2c[0x254];
    int active;                     /* 0x280 */
    char pad284[0x140];
} D2PAINTWIN;

extern D2PIC *d2_Pics;              /* 0x6ed918 */
extern int d2_NPics;                /* 0x6ed91c */
extern int d2_PicBytes;             /* 0x6ed920 */
extern void *d2_ScreenSurf;         /* 0x6ed924, the screen surface the widgets are drawn on */
extern int d2_PicShift;             /* 0x6ed98c */
extern D2OBJ *d2_Objs;              /* 0xf8f080 */
extern int d2_FontHeight;            /* 0xf8f084, font height */
extern D2WIN d2_Wins[0x60];         /* 0xf8f120 */
extern D2INPUT d2_Inputs[0x80];     /* 0xfa6020 */
extern D2RADIO d2_Radios[8];        /* 0xfa5aa0 */

extern short d2_ScreenH;            /* 0xfb9070 */
extern short d2_ScreenW;            /* 0xfb9072 */
extern int d2_PaletteDepth;            /* 0xf8a64c */
extern short d2_MouseX;             /* 0xf8a674, mouse x */
extern short d2_MouseY;             /* 0xf8a676, mouse y */
extern char d2_GfxPath[];         /* 0xfa5f20 */
extern int d2_PointerX;            /* 0xfa5f08, pointer x */
extern int d2_PointerY;            /* 0xfa5f0c, pointer y */
extern int dd_Page;            /* 0x63120c, current page, = dd_Page (dd_vesa.h); d2.c uses dd_Page */
extern int d2_PointerObj;            /* 0x5fe710, focused object */
extern int d2_HitObjId;            /* 0x5fe714 */
extern int d2_CurrentFont;            /* 0x5fe718 */
extern int d2_DefaultFont;            /* 0x5fe73c, default font */
extern int d2_ButtonPulldownPic;            /* 0x5fe740, "_BUTTON_PULLDOWN" picture */
extern int d2_ClipRight;            /* 0x5fed7c, width of the screen */
extern int d2_ClipBottom;            /* 0x5fed80, height of the screen */
extern int d2_TextDrawnW;            /* 0x6ed968 */
extern int d2_TextDrawnH;            /* 0x6ed96c */
extern D2WIN *d2_PendingCaptionWin;  /* 0x6ed9b8, window of d2_PendingCaptionObj */
extern D2OBJ *d2_PendingCaptionObj;  /* 0x6ed9bc, the object whose caption is drawn last
                                        (under the pointer / hit object): d2_DrawObjCaption, then 0 */
extern D2INPUT *d2_FocusedInput;       /* 0x6ed9dc, the focused input */
extern int d2_ClipLeft;            /* 0x6f0458, clip left */
extern int d2_ClipTop;            /* 0x6f045c, clip top */
extern int d2_DirtyRectLeft[4];         /* 0xfb9060, the rectangle d2_AddDirty tests against: left, top, right, bottom (one object: one memset clears it) */
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
extern D2PICHDR *d2_FontPic;      /* 0x6ed948, the font "_FONT" */
extern D2TEX *d2_CaptionTex;         /* 0x6ed960, sprite behind captions / texts */
extern D2OBJ **d2_DrawOrder;        /* 0x6ed964, objects in drawing order (0x300) */
extern void *d2_BackSurf;          /* 0x6ed92c */
extern char d2_PicBase;             /* 0x6ed934 */
extern int d2_FrameCounter;            /* 0x6ed940, frame counter */
extern int d2_HighestObj;            /* 0x6ed94c, highest object index in use */
extern int d2_TextSegCount;            /* 0x6ed954, number of text pieces */
extern int d2_TextLeft;            /* 0x6ed970, left edge of the text drawn last */
extern int d2_TextTop;            /* 0x6ed974, top of the text drawn last */
extern int d2_LayoutMode;            /* 0x6ed978, 1 while a window is measured/laid out (drawing skipped) */
extern short d2_ShpW;          /* 0x6f044c */
extern short d2_ShpH;          /* 0x6f044e */
extern int d2_LastPointerX;            /* 0x633f10, last pointer x */
extern int d2_LastPointerY;            /* 0x633f14, last pointer y */
extern char d2_NoBackgroundRestore;  /* 0x5fe71c, 1: the alternate background path - captions are drawn without saving / restoring the background underneath (d2_DrawWindowBackground, d2_DrawObjects); it is only compared to 1, no writer is drafted */
extern int d2_SpaceWidth;            /* 0x5fe724, width of a space */
extern int d2_LetterGap;            /* 0x5fe728, gap between letters */
extern int d2_PointerWin;            /* 0x5fe72c, window id under the pointer (d2_FindWindowAt scan) */
extern int d2_PointerWinVisible;     /* 0x5fe730, visible window under the pointer (d2_HitTest object scan) */
extern int d2_PointerMoved;            /* 0x5fe75c */
extern int d2_ClickedId;            /* 0xf8a668 */
extern D2SEG d2_TextSegs[];        /* 0xf8abe0 */
extern char d2_PointerTexName[];         /* 0xf8ef00 */
extern D2VIEW d2_Views[0x30];   /* 0xfb1020 */
extern int d2_ScrollMarkerPic;            /* 0x5fe734, scroll marker picture (3 frames) */
extern int d2_WinBorderPic;            /* 0x5fe744, frame picture of a window */
extern int d2_WinBorderPergaPic;            /* 0x5fe748, another 8 pixel frame picture */
extern int d2_MiscPic;            /* 0x5fe74c, _MISC picture, its frames are the 'i' command icons */
extern char ch_EmptyName[];         /* 0x631090, "" (ch.h) */
extern D2SURF *d2_BackgroundTile;        /* 0x6ed930, background tile */
extern int d2_LastWindow;            /* 0x6ed938, window created last */
extern D2WIN *d2_LastWindowPtr;         /* 0x6ed980, window created last */
extern int d2_WindowsOpen;            /* 0x6ed9b4, windows open */
extern int d2_TextSegFlags;            /* 0x6ed9c0, flags of the next text piece */
extern D2FADE *d2_Fades[32];        /* 0xf8f0a0, fades (d2_StartFade) */
extern int cm_PickCellX;            /* 0xfa5f00, text layout x, running x of the piece being laid out */
extern int cm_PickCellZ;            /* 0xfa5f04, text layout y, top of the line being laid out */
extern float d2_ScreenScale;          /* 0x5fe708, screen width / 800 */
extern int d2_PixelFormat;            /* 0x5fe720, display colour format code copied from dd_PixelFormat (888 / 565 / 555) */
extern int d2_PointerPicIndex;            /* 0x5fe750, "_MOUSE_CURSOR" */
extern int d2_ExtraSymbolsPic;            /* 0x5fe754, "_EXTRA_SYMBOLS" */
extern int d2_TextCursorPic;            /* 0x5fe758, "_TEXT_CURSOR" */
extern void *d2_FrontSurf;          /* 0x6ed928, front buffer surface */
extern int d2_MainWindow;            /* 0xf8eee0, the main window handle (HWND) given to d2_Open */
extern char d2_GfxDir[];         /* 0xf8ef80, directory of the gfx file (set in d2_Open) */
extern int d2_HalfScreenH;            /* 0xf8f088, half the screen height */
extern int d2_HalfScreenW;            /* 0xf8f08c, half the screen width */

/* functions of other units */
void d3if_FreeSprite(D3TEXHANDLE *t);                                                    /* 0x4481e0 */
int d2_NewObject(void);                                                         /* 0x46c9f0, allocates an object */
void d2_AddRefcount(int pic);                                                   /* 0x46ca60 */
void d2_SubRefcount(int pic);                                                   /* 0x46cac0 */
#define d2_Unk46ca60 d2_AddRefcount /* old names (round 16), until the round 17 d2 files use the new ones */
#define d2_Unk46cac0 d2_SubRefcount
void d2_MoveObj(short x, short y, int obj);                                       /* 0x46cda0, moves an object (d2.c); d2_CreatePicObj passes ints (d2_MoveObjI) */
void d2_RemoveObj(int obj);                                                      /* 0x46e710, removes the object */
int d2_TextWidth(char *text, int font);                                         /* 0x46ea10, text width */
void d2_DrawText(int x, int y, void *surf, char *text, unsigned char flags);              /* 0x46f000, draws a text */
void d2_DrawTextBox(int x, int y, int w, int h, void *surf, char *text, unsigned int flags); /* 0x46f1a0, text in a box */
int d2_RemoveWindow(int win);                                                      /* 0x472ce0, 1 if the window was open */
void d2_AddToDrawOrder(int obj, D2WIN *win);                                         /* 0x4730e0 */
void d2_SetCaption(int obj, char *text);                                         /* 0x473540, button caption */
void d2_SetObjValue();                                             /* 0x475cb0 (int obj, int a, int b, int c, int d); d2.c calls it with 2 */
void *d2_SetFont(int font);                                                    /* 0x476030, selects the font */
void d2_FreeFades(void);                                                        /* 0x476b40 */
struct D2SURF * d2_MakeSurface(struct D2SURF *desc, int flags);                                  /* 0x478cc0, creates a surface */
int d2_FreeSurface(struct D2SURF *s);                                                 /* 0x478fc0, frees a surface */
int d2_LockSurface(D2SURF *surf);                                                 /* 0x479020, locks */
int d2_UnlockSurface(D2SURF *surf);                                                 /* 0x4790e0, unlocks */
void d2_BlitPart(int sx, int sy, int w, int h, struct D2SURF *src, int x, int y, struct D2SURF *dst); /* 0x479480, blits a part */
int d2_ClearSurface(D2SURF *surf);                                                 /* 0x4796c0, clears */
int d2_PutPixel(int x, int y, unsigned short color, D2SURF *surf);             /* 0x479b60, plots a pixel */
int d2_PutPixelClipped(int x, int y, unsigned short color, D2SURF *surf);             /* 0x479b90, the same, clipped */

struct D3IFSPRITE; char d3if_DrawSprite(struct D3IFSPRITE *t, int x, int y, float angle, float sx, float sy, float alpha); /* 0x448270 */
char d3_ScreenToUV(float sx, float sy, struct D3OBJECT *obj, void *tex, float *outu, float *outv); /* 0x448e60 */
void d3io_NoopCallback(void *t);                                                            /* 0x44ee40, (x, y, surf, pic) */
struct TSTEXTURE * tex_Find(char *name);                                                    /* 0x45f980 */
void d2_DrawWindow(int win, void *surf);                                         /* 0x471a90, draws a window */
int d2_CloseWindow(int win);                                                      /* 0x472e00, closes a window */
int d2_GetObjPicSize(int obj, int *w, int *h);                                    /* 0x4759f0, size of a picture object, 0 if it has none */
int d2_GetObjValue(int obj);                                                      /* 0x475980, slider value */
void d2_EnableObject(int obj, int on);                                             /* 0x475aa0 */
void d2_DissolveStep(D2DISSOLVE *d);                                              /* 0x476110, one step of a dissolve */
void d2_DrawFades(void);                                                        /* 0x476b10 */
void d2_SetSurfaceClip(int x, int y, int w, int h, D2SURF *surf);                    /* 0x479cd0, sets the clip rectangle */
D3TEXHANDLE *d3if_LoadSprite(char *name, char glow);                                      /* 0x448200 */
void d2_SetObjEnabled(int obj, int on);                                             /* 0x475e40 */
void d2_ScaleObject(int obj, int a);                                            /* 0x475e80, scales a picture object; a > 1 larger, a < -1 smaller */
void d2_ReleaseFade(D2FADE *f);                                             /* 0x476860 */
int d2_BlitPartMode(int sx, int sy, int w, int h, struct D2SURF *src, int x, int y, struct D2SURF *dst, unsigned char mode); /* 0x479150 */
int d2_ClearRect(int x, int y, int w, int h, D2SURF *surf);                     /* 0x4797d0, clears a rectangle */
void d2_DrawRectOutline(int x, int y, int w, int h, unsigned int r, unsigned int g, unsigned int b, D2SURF *surf); /* 0x479bf0, outline of w x h */
int d2_GetSurfaceDesc(D2SURF *surf, struct _DDSURFACEDESC2 *desc);                   /* 0x479120, 0x7c byte surface desc */

/* d2.c */
int d2_FreeOldest(void);                                    /* 0x4687a0 */
int d2_FindFreeObj(void);                                   /* 0x468860 */
int d2_FindPic(char *name);                                 /* 0x468890 */
int d2_LoadPic(int idx, char *path);                        /* 0x468900 */
void d2_Exit(void);                                         /* 0x468a90 */
void d2_RestoreShapes(D2BUF *buf);                          /* 0x468c70 */
int d2_AddToWin(int obj, int win);                          /* 0x468cf0 */
void d2_FlushDirty(void);                                   /* 0x468e30 */
void d2_AddDirty(int x, int y, int w, int h);               /* 0x468eb0 */
void d2_AddShape(int x, int y, int w, int h, D2BUF *buf);   /* 0x468fb0 */
void *d2_GetPic(int idx);                                   /* 0x4690b0 */
int d2_GetPicEx(int idx, void **pic, int *real);            /* 0x469180 */
void d2_SetObjPic(int obj, int pic);                        /* 0x469250 */
void d2_DrawFrame(D2INPUT *dlg, void *arg);                 /* 0x469310 */
void d2_DrawInput(int in, void *surf);                      /* 0x4694c0 */
int d2_OpenInputIcon(int x, int y, int icon, unsigned char flags);  /* 0x469ca0 */
int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win);  /* 0x469ee0 */
void d2_InputSetString(int in, int str);                    /* 0x469fe0 */
int d2_OpenInput(int x, int y, int w, int h, unsigned char flags);  /* 0x46a110 */
int d2_WinOpenInput(int x, int y, int w, int h, unsigned char flags, int win);  /* 0x46a360 */
int d2_OpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags);  /* 0x46a4c0 */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win);  /* 0x46a960 */
void d2_DrawSlider(int id, D2SURF *surf);                    /* 0x46aa00, draws a slider */
void d2_SetLabel(int id, char *s);                         /* 0x46bc70, sets the widget's label */
void d2_SetList(int id, int n, ...);                      /* 0x46bca0, sets the widget's "a|b|c|" item list */
void d2_ListCopyItem(int id, int idx, char *out);              /* 0x46bd30 */
void d2_SetSliderFlags(int id, int flags);                       /* 0x46bdd0, sets the slider's flags */
int d2_CreateSlider(int x, int y, int pic, int unk);           /* 0x46be00 */
int d2_WinCreateSlider(int x, int y, int pic, int unk, int win);  /* 0x46bf10 */
void d2_DrawButton(int id, D2SURF *surf);                    /* 0x46bf60, draws a button */
void d2_CenterObj(int obj);                                 /* 0x46c1d0 */
void d2_SetObjWidth(int obj, short w);                      /* 0x46c260 */
int d2_OpenRaw(int x, int y, int w, int h, void *src);      /* 0x46c290 */
int d2_WinOpenRaw(int x, int y, int w, int h, void *src, int win);  /* 0x46c390 */
void d2_DrawRaw(int obj, D2SURF *surf);                     /* 0x46c3f0 */
int d2_CreateRadioGroup(int n, ...);                        /* 0x46c490 */
void d2_RadioAdd(int g, int obj);                           /* 0x46c560 */
void d2_RadioClear(int g);                                  /* 0x46c600 */
void d2_RadioSelect(int g, int sel);                        /* 0x46c630 */
int d2_RadioGetSel(int g);                                  /* 0x46c6a0 */
void d2_MenuNavigate(int g);                                   /* 0x46c6c0, keyboard / pointer navigation in a radio group */
int d2_OpenPic(int x, int y, int pic);                    /* 0x46cb20, opens a picture object (d2.c: K&R, int x, y) */
int d2_CreatePicObj(int x, int y, int pic);                    /* 0x46cd80, @original: the K&R definition is compiled against this int prototype, its callers in d2.c against a short one; C89 forbids the pair, the bytes need it */                    /* 0x46cd80 */
void d2_ShowPicture(int pic);                                 /* 0x46cfd0 */
void d2_DrawPic(int x, int y, int pic, void *surf);       /* 0x46d040, draws a picture */
void d2_DrawObjCaption(D2OBJ *o);                                /* 0x46d220, draws a caption */
void d2_DrawObjects(void);                                    /* 0x46d3d0, draws all objects */
void d2_DrawAll(void);                                    /* 0x46e150 */
int d2_FindWindowAt(short x, short y);                         /* 0x46e190 */
int d2_HitTestViews(int x, int y);                             /* 0x46e260 */
int d2_HitTest(short x, short y);                         /* 0x46e460 */
void d2_ClearScreen(void);                                    /* 0x46e990 */
void d2_DrawTextLine(int a, int y, void *surf, int win, char nojustify, int flags, int x, int right); /* 0x46ead0, draws one line of d2_TextSegs in a window */
void d2_SetTextColour(unsigned char r, unsigned char g, unsigned char b);   /* 0x46f180 */
void d2_ScrollWindow(int win);                                 /* 0x46f840, scrolls a window one step */
void d2_LayoutButtons(int win);                                 /* 0x46fad0, gives the buttons of a window one width */
void d2_DrawWindowText(void *surf, int win);                     /* 0x46fba0, lays out and draws the text of a window */
void d2_DrawWindowBackground(D2WIN *w, void *surf);                    /* 0x4713c0, draws the background of a window */
void d2_SetBackgroundTexture(char *name, int win);          /* 0x4721e0 */
void d2_CreateScrollButtons(int x, int y, int win, int obj, int pic); /* 0x472270, creates the scroll buttons */
int d2_CreateWindow();                                         /* 0x472390, creates a window (x, y, w, h short; flags);
                                                               called unprototyped */
void d2_ScrollWindowBy(int a, int b, int win);                   /* 0x472760, scrolls a window */
void d2_ResizeWindow(short w, short h, int win);               /* 0x472800, resizes a window */
void d2_FinishFades(void);                                    /* 0x472a60 */
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);  /* 0x472aa0, window in a window */
void d2_GetFrameSize(D2WIN *win, int *w, int *h);              /* 0x472c80, size of the frame pieces */
int d2_RemoveWindowObjects(int win, int all);                         /* 0x472d20, removes the objects of a window */
void d2_RaiseObject(int obj);                                 /* 0x472f50 */
void d2_RaiseWindow(int win);                                 /* 0x472fd0 */
int d2_WinOpenPic(short x, short y, int pic, int win);       /* 0x4731d0, picture object on a window (defined with
                                                               int x, y; callers above it pass shorts) */
int d2_WinOpenPicStyled(int x, int y, int pic, int win, int a);    /* 0x473350 */
void d2_SetObjText(int obj, char *text);                     /* 0x4734d0, sets the text of a text object */
int d2_CreateWinText(short x, short y, char *text, int win);    /* 0x473650, text label on a window */
int d2_CreateScreenText(short x, short y, char *text);             /* 0x473860, text label on the screen */
int d2_CreateWinCaption(short x, short y, char *text, int win);    /* 0x473980, caption object on a window */
void d2_ReplaceInputText(int obj, char *text);                     /* 0x473b30 */
int d2_Open(int unk, int w, int h, char *dir, char *file);  /* 0x473c10, init: allocations, reads the gfx file */
void d2_RaiseViewWindows(int view);                                /* 0x4741a0, raises the windows of a view */


/* 0x4741f0..0x476c00 (round 18) */
int d2_BlitPartToDD(int sx, int sy, int w, int h, D2SURF *src, int x, int y, struct IDirectDrawSurface4 *dst); /* 0x4794c0 */
int d2_BlitPartFromDD(int sx, int sy, int w, int h, struct IDirectDrawSurface4 *src, int x, int y, D2SURF *dst); /* 0x479610, blits a part of a DirectDraw surface onto a D2SURF */
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                            /* 0x430470 */
int vfs_fread(void *buf, int size, int n, struct VFSFILE *f);                             /* 0x430bd0 */
int vfs_fseek(struct VFSFILE *f, long offset, int origin);                                    /* 0x430fa0 */
int vfs_fclose(struct VFSFILE *f);                                                        /* 0x430aa0 */
extern struct VFSDIR *vfs_Root;                                                          /* 0x6333fc */
extern char d2_FormsDir[];                                                     /* 0x5fe608, "forms\\", prefix of a form file name */
extern int d2_CurrentView;                                                        /* 0x6ed958, the view a form was opened in */
extern int d2_FullFormPath;                                                        /* 0x6ed984, nonzero: the name given to d2_OpenForm is a full path (no d2_FormsDir prefix, no .form suffix); cleared after each load, nothing in the binary sets it */
extern D2FORM0 d2_Forms0[];                                                     /* 0x633f18 */
extern D2FORM2 d2_Forms2[];                                                     /* 0x68ce98 */
int d2_OpenForm(int x, int y, char *name);                                     /* 0x4741f0, loads a view */
int d2_LoadView(int x, int y, char *name);                                     /* 0x474b30 */
int d2_SetViewTextures(int view, char *tex1, char *tex2);                             /* 0x474ca0 */
void d2_ClearViewTextures(int view);                                                    /* 0x474d10 */
void d2_FillViewTextures(int view, char *tex1, char *tex2);                            /* 0x474d90 */
void d2_RedrawView(int view);                                                    /* 0x474e80 */
void d2_EnableViewObjects(int view, int on);                                            /* 0x4754d0 */
void d2_SetViewEnabled(int view, int on);                /* 0x475590 */
void d2_CentreViewWindows(int view);                        /* 0x475600 */
void d2_PlaceWindow(int win, unsigned char flags);    /* 0x4756b0 */
void d2_PlaceWindowScaled();                                /* 0x475730 (int win, unsigned char flags), K&R */
void d2_PlaceMainWindow();   /* 0x475850 (int view, unsigned char flags), K&R: the original tail-jumps to d2_PlaceWindowScaled, so callers pass the flags */
void d2_PlaceViewWindows(int view, unsigned char flags);   /* 0x475870 */
void d2_CloseView(int view);                        /* 0x4758f0 */
int d2_GetWinObj(int view, int idx, int k);         /* 0x475b80 */
int d2_InputStep(D2INPUT *in);                      /* 0x475bf0 */
void d2_SetObjMax(int obj, int v);                  /* 0x475f40 */
int d2_SetForm(int view, int n);                    /* 0x475f70 */
extern unsigned char d2_PenSize;  /* 0x6ed97c, the pen size of d2_Plot (0: one pixel, 1: a cross) */
extern D2DISSOLVE *d2_CurrentDissolve;    /* 0xf8abc8 */
extern D2FRCT d2_PointerItems[6];   /* 0xf8ab80, the item list drawn next to the pointer */
extern volatile int tb_Calls;       /* 0x6333d8, timer callbacks */
int d2_PutPixelRGB(int x, int y, unsigned int r, unsigned int g, unsigned int b, D2SURF *surf); /* 0x4799e0 */
int d2_GetViewWin(int view, int i);                  /* 0x475ff0 */
void d2_SetObjFont(int obj, int font);               /* 0x4760b0 */
void d2_Plot(int x, int y, int r, int g, int b);   /* 0x476220 */
void d2_PlotLine(int x0, int y0, int x1, int y1, int r, int g, int b); /* 0x4764f0 */
void d2_ClearPaintbox(void);                            /* 0x476710 */
void d2_FillPaintbox(int x, int y, int w, int h);      /* 0x476760 */
D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0 */
void d2_DrawFade(D2FADE *f, D2SURF *surf);         /* 0x4768a0 */
void d2_ClearInput(void);                            /* 0x476b70 */

/* declared in other units' headers before (moved to the owner) */
extern char *txt_Ptrs[0x4800];  /* 0x143a1c0 */
extern unsigned char d2_HoverBlocked;  /* 0x6ed9c8 */

#endif
