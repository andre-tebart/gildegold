/* unk_476c00.h -- what d2.h lacks for unk_476c00.c (0x476c00..0x479610 of the "d2" 2D layer, GFX.C: item list
   above the mouse cursor, cursor, hover / scroll wheel handling, input field editing, mouse hit rectangles, bmp
   loading, surfaces). Names are ours. */
#ifndef UNK_476C00_H
#define UNK_476C00_H
struct SBBANK;

struct SBSAMPLE;
#include "d2.h"

typedef struct D2SLOT {             /* 12 bytes, a surface for the picture of an item list */
    int key0;                       /* 0x00, -1 = empty (set on create / free); a reasoned name: nothing in the binary reads it */
    int key1;                       /* 0x04, -1 = empty, as key0; never read */
    void *surf;                     /* 0x08 */
} D2SLOT;

#define D2CUR d2_FocusedInput     /* the input field being edited */

/* data */
extern int d2_PointerSprite;            /* 0x5febc8, sprite of the pointer, -1: none */
extern int d2_TextCursorOffset;            /* 0x5febcc, -7 or 0 */
extern int d2_HoverX;            /* 0x5febdc */
extern int d2_HoverY;            /* 0x5febe0 */
extern int d2_LimitsChanged;            /* 0x5febe4 */
extern int d2_TextCursorSprite;            /* 0x5febe8, sprite of the text cursor, -1: none */
extern int d2_DragSlider;            /* 0x5febec */
extern int d2_DragStartValue;            /* 0x6ed9c4 */
extern void *d2_PointerPic;          /* 0x6ed9cc, "_MOUSE_CURSOR" data */
extern int d2_RightHolding;            /* 0x6ed9e4 */
extern int d2_DragStartX;            /* 0x6ed9e8 */
extern int d2_DragStartY;            /* 0x6ed9ec */
extern int d2_ClickUsed;            /* 0x6ed9f0 */
extern int d2_DragActive;            /* 0x6ed9f4 */
extern short d2_LastMouseX;          /* 0xf8a650 */
extern short d2_LastMouseY;          /* 0xf8a652 */
extern int d2_HoverTick;            /* 0xf8a654 */
extern int d2_FocusObjIndex;            /* 0xf8a658 */
extern int d2_InputDragOffsetY;            /* 0xf8a660 */
extern int d2_SavedMaxY;            /* 0xf8a664 */
extern int d2_SavedMinY;            /* 0xf8a66c */
extern D2SLOT d2_ItemListSurf256;         /* 0xf8aa90 */
extern D2SLOT d2_ItemListSurf128[3];      /* 0xf8aaa0 */
extern int d2_RightHeldObj;            /* 0xf8aac8 */
extern int d2_HoverObj;            /* 0xf8aacc */
extern int d2_TextCursorX;            /* 0xf8aad4 */
extern int d2_TextCursorY;            /* 0xf8aad8 */
extern void *d2_ItemListSurf64;          /* 0xf8aadc */
extern int d2_InputDragValue;            /* 0xf8ab60 */
extern int d2_PrevHoverObj;            /* 0xf8ab64 */
extern int d2_InputDragReleased;            /* 0xf8ab68 */

/* functions of other units, or declared differently by d2.h */
void d2_CopyCaption(char *caption);                   /* 0x478870 */
int d2_CountPointerItems(void);                             /* 0x476c90 */

/* d2 functions of this file */
void d2_SetPointerItem(int id, int amount);              /* 0x476c00 */
void d2_ClearPointerItems(void);                            /* 0x476c70 */
void d2_DrawPointer(void *surf, int x, int y, int a, int b);  /* 0x476cb0 */
int d2_DrawMousePointer(int x, int y);                     /* 0x476f00 */
void d2_SetPointerSprite(int a, int pic);                  /* 0x476f80 */
void d2_UpdateHoverScroll(void);                            /* 0x477000 */
void d2_CreateItemListSurfaces(void);                            /* 0x477320 */
void d2_FreeItemListSurfaces(void);                            /* 0x477420 */
void d2_DragInputValue(D2INPUT *in, int val, int y0, int y1);    /* 0x4774a0 */
void d2_EndInputEdit(void);                            /* 0x477530 */
void d2_InputKey(void);                            /* 0x477590 */
void d2_DragInput(void);                            /* 0x477a70 */
void d2_SliderInput(void);                            /* 0x477ba0 */
int d2_UpdateInput(void);                             /* 0x477ed0 */

/* 0x478320..0x479610 (unk_478320.h until round 20) */
typedef struct D2HIT {              /* 0x10 bytes, a mouse hit rectangle, d2_HitRects[] */
    short x, y, w, h;               /* 0x00 */
    int user;                       /* 0x08 */
    int used;                       /* 0x0c */
} D2HIT;

extern int d2_ClickedSlot;            /* 0xf8a65c */
extern D2OBJ *d2_ClickedObj;         /* 0xf8a670 */
extern D2HIT d2_HitRects[];        /* 0xf8a680 */
extern int d2_PointerObjSlot;            /* 0xf8aa80 */
extern int d2_HitCount;            /* 0xf8aa84 */
extern int d2_LastClickedId;            /* 0xf8aa9c */
extern int d2_ClickedForm;            /* 0xf8aac4 */
extern char d2_CaptionCopy[];         /* 0xf8aae0 */
extern int d2_HitClicked;            /* 0x5febd0 */
extern int d2_HitUnderPointer;            /* 0x5febd4 */
extern int d2_ClickBank;            /* 0x6ed990 */

struct SBSAMPLE *sb_PlaySample(struct SBBANK *bank, char *name, int loops, int pan, int volume, int fade, int delay); /* 0x426fa0, sblib.c (this header's units: sb_PlaySampleI) */

void d2_FocusInput(int obj);                                         /* 0x478320, gives an input object the focus */
void d2_MouseUpdate(void);                                            /* 0x4784c0, mouse handling of the gui */
int d2_AddHitRect(short x, short y, short w, short h, int user);     /* 0x478890, adds a hit rectangle */
void d2_RemoveHitRect(int i);                                           /* 0x4788f0 */
int d2_CompareColour(char *a, char *b);                                 /* 0x478930 */
void d2_SetColour(D2RGB *c, unsigned char r, unsigned char g, unsigned char b);     /* 0x478960 */
unsigned int d2_GetBmpSize(char *name);                              /* 0x478980, size of a bmp */
int d2_ReadBmpPixels(char *name, unsigned char *dst);               /* 0x478a00, reads the pixels of a 24 bit bmp */
D2SURF *d2_LoadBmpSurface(char *name);                             /* 0x478b50, surface from a bmp */
void d2_ConvertRgbTo16(int a, int b, unsigned int w, unsigned int h, unsigned char *src, int c, int d, D2SURF *s);   /* 0x478bd0, rgb bytes to 16 bit pixels */
void d2_Convert16ToRgb(int a, int b, unsigned int w, unsigned int h, unsigned short *src, int c, int d, unsigned char *dst);   /* 0x478c50, 16 bit pixels to rgb bytes */

/* declared in other units' headers before (moved to the owner) */
int d2_PutPixelClipped(int x, int y, unsigned short color, D2SURF *surf);  /* 0x479b90 */
int d2_PutPixelRGB(int x, int y, unsigned char r, unsigned char g, unsigned char b, D2SURF *surf);  /* 0x4799e0 */
void d2_SetSurfaceClip(int x, int y, int w, int h, D2SURF *surf);  /* 0x479cd0 */
int d2_UnlockSurface(D2SURF *s);  /* 0x4790e0 */
void d2_GetPixelColour(int x, int y, D2SURF *surf, unsigned char *rgb);  /* 0x4798f0 */
int d2_LockSurface(D2SURF *s);  /* 0x479020 */
int d2_PutPixel(int x, int y, unsigned short color, D2SURF *surf);  /* 0x479b60 */
int d2_FreeSurface(D2SURF *s);  /* 0x478fc0 */
int d2_ClearRect(int x, int y, int w, int h, D2SURF *surf);  /* 0x4797d0 */
void d2_DrawRectOutline(int x, int y, int w, int h, unsigned int r, unsigned int g, unsigned int b, D2SURF *surf);  /* 0x479bf0 */
int d2_BlitPartMode(int sx, int sy, int w, int h, D2SURF *src, int x, int y, D2SURF *dst, unsigned char mode);  /* 0x479150 */
int d2_ClearSurface(D2SURF *surf);  /* 0x4796c0 */
void d2_AddPointerItem(int id, int amount);  /* 0x476b90 */
void d2_BlitPart(int sx, int sy, int w, int h, D2SURF *src, int x, int y, D2SURF *dst);  /* 0x479480 */

#endif
