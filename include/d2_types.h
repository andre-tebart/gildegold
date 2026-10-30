/* The record types of d2.c's tables d2_Wins, d2_Inputs, d2_Radios, d2_Views (and D2TEX / D2SURF they use), for the
   units that declare the tables without the rest of d2.h: types only. d2.h includes it. */
#ifndef D2_TYPES_H
#define D2_TYPES_H

struct IDirectDrawSurface4;          /* <ddraw.h> */

typedef struct D2TEX {              /* a sprite texture handle (tex_Find, d3if_DrawSprite) */
    char pad00[0x74];
    unsigned int size;              /* 0x74, size of one frame */
} D2TEX;

typedef struct D2SURF {             /* 0x40 bytes, a GFX.C surface (made by d2_MakeSurface): system memory pixels or a
                                       DirectDraw surface; the widgets are drawn on one */
    int makeFlags;                  /* 0x00, the flags this surface was made with (d2_MakeSurface's, handed on
                                       by d2_CopySurface so a copy is the same kind of surface) */
    int w;                          /* 0x04 */
    int h;                          /* 0x08 */
    int rowbytes;                   /* 0x0c, pitch in bytes */
    int pitch;                      /* 0x10, in pixels */
    unsigned char bpp;              /* 0x14, bits per pixel */
    char pad15[3];
    int unk18;
    unsigned short *bits;           /* 0x1c, pixels (system memory, or while locked) */
    struct IDirectDrawSurface4 *DDSurface;  /* 0x20 */
    int left;                       /* 0x24, clip rectangle */
    int top;                        /* 0x28 */
    int right;                      /* 0x2c */
    int bottom;                     /* 0x30 */
    int locked;                     /* 0x34 */
    int external;                   /* 0x38, 1: wraps an existing DirectDraw surface (its bits are
                                       DirectDraw's, nothing allocated or freed here) */
    int manualBlit;                     /* 0x3c, 1: blit by hand (hardware blit failed) */
} D2SURF;

typedef struct D2WIN {              /* 0x3c4 bytes, d2_Wins[0..0x60] */
    int id;                         /* 0x00, own index */
    short x, y, w, h;               /* 0x04 */
    unsigned int flags;             /* 0x0c, 1: sprite background + 2nd surface, 2: white border, 4: framed,
                                       8: title bar, 0x10: text, 0x20: scroll marker, 0x40: text right,
                                       0x80: text spread, 0x100: text centred, 0x200: scrolls, 0x400,
                                       0x1000: saved background, 0x2000: grows with the text, 0x4000: fits
                                       the text, 0x8000, 0x10000: 3rd surface */
    short ml;                       /* 0x10, text margins left, */
    short mr;                       /* 0x12, text margin right */
    short mt, mb;                   /* 0x14, top, bottom */
    int *objs;                      /* 0x18 */
    short nobjs;                    /* 0x1c, at most 0x180 */
    short pad1e;
    void *surf0;                    /* 0x20, saved background */
    void *surf1;                    /* 0x24 */
    D2SURF *surf2;                  /* 0x28, overlay image */
    char *text;                     /* 0x2c */
    int unk30;                      /* 0x30 */
    char pad34[8];
    int lines[0x7f];                /* 0x3c, text position of each laid out line */
    int firstline;                  /* 0x238 */
    int nlines;                     /* 0x23c */
    int pad240;
    int contentHeight;              /* 0x244, height of the content */
    union {
        int scrollY;                /* 0x248, scroll offset y */
        short scrolly;              /* 0x248, the same read as a short */
    };
    int appliedScrollY;             /* 0x24c, scroll offset y the objects are at */
    int scrollRemY;                 /* 0x250, scroll y still to go */
    int scrollRemX;                 /* 0x254, scroll x still to go */
    union {
        int scrollX;                /* 0x258, scroll offset x */
        short scrollx;              /* 0x258, the same read as a short */
    };
    int appliedScrollX;             /* 0x25c, scroll offset x the objects are at */
    unsigned char scrollStep;       /* 0x260, pixels per scroll step (d2_ScrollWindow) */
    char pad261[3];
    int visible;                    /* 0x264, also the scroll step of the buttons */
    short unk268;                   /* 0x268 */
    short pad26a;
    int parent;                     /* 0x26c, object of the window */
    int backgroundState;            /* 0x270, the window background's save/restore state, decremented after every
                                       draw: 2: this draw copies the window's area from the screen into surf0
                                       (save), 1: the next draw copies it back (restore), 0: nothing pending.
                                       d2_CreateWindow starts at 1; the writer of 2 is outside the drafted
                                       sources */
    int unk274;                     /* 0x274 */
    int frame;                      /* 0x278, frame picture */
    short font;                     /* 0x27c */
    char pad27e[2];
    int active;                     /* 0x280 */
    int args[0x41];                 /* 0x284, parameters of the text commands */
    int bgpic;                      /* 0x388, tiled background picture, -1 = none */
    int tileBackground;             /* 0x38c, 1: the background is the tiled d2_BackgroundTile */
    D2TEX *spr;                     /* 0x390, background texture (d3if_LoadSprite) */
    float alpha;                    /* 0x394 */
    int padl, padt, padr, padb;     /* 0x398 */
    int btnup;                      /* 0x3a8, scroll buttons / bar, -1: none */
    int btndown;                    /* 0x3ac */
    int bar;                        /* 0x3b0 */
    int sizedObjCount;              /* 0x3b4, number of objects when the buttons were sized (0: resize next draw) */
    int scrollBar;                  /* 0x3b8, scroll bar (slider object), -1: none */
    int cornerObj;                  /* 0x3bc, child object outside objs; when set, the top-left corner piece is not drawn; -1: none */
    int extraObj;                   /* 0x3c0, second child object outside objs (like scrollBar); -1: none */
} D2WIN;

typedef struct D2INPUT {            /* 0x160 bytes, d2_Inputs[1..0x7f] */
    int id;                         /* 0x00 */
    int x;                          /* 0x04 */
    int y;                          /* 0x08 */
    int unk0c;
    int w;                          /* 0x10 */
    int h;                          /* 0x14 */
    int maxval;                     /* 0x18 */
    int minval;                     /* 0x1c, lower bound of the value */
    int frameY;                     /* 0x20, y of the frame drawn by d2_DrawFrame */
    unsigned char ndigits;          /* 0x24 */
    char pad25[3];
    unsigned int flags;             /* 0x28 */
    char text[0x100];               /* 0x2c */
    int value;                      /* 0x12c */
    int used;                       /* 0x130 */
    int obj;                        /* 0x134, index into d2_Objs */
    int font;                       /* 0x138 */
    int icon;                       /* 0x13c, -1: none */
    int mult;                       /* 0x140 */
    int unk144;
    int firstTextId;                     /* 0x148 */
    int currentTextId;                     /* 0x14c */
    int textCount;                     /* 0x150 */
    char *buf;                      /* 0x154 */
    int buflen;                     /* 0x158 */
    int maxw;                       /* 0x15c */
} D2INPUT;

typedef struct D2RADIO {            /* 0x8c bytes, a radio button group, d2_Radios[8] */
    int flags;                      /* 0x00, bit 0: the pointer selects only on a mouse release (dm_L.lup) */
    int n;                          /* 0x04, number of objects, 0 = free */
    int sel;                        /* 0x08, selected one, -1 = none */
    int objs[0x20];                 /* 0x0c */
} D2RADIO;

typedef struct D2VIEW {             /* 0x2ac bytes, a 3D view whose textures show windows, d2_Views[0x30] */
    int id;                         /* 0x00, own view index (set in d2_Open) */
    int wins[0x60];                 /* 0x004 */
    int nwins;                      /* 0x184 */
    int viewX, viewY;               /* 0x188 / 0x18c, the view's drawing origin: subtracted from a window's /
                                       object's screen coordinates to place its shape in the view's image
                                       (d2_RedrawView); no writer is drafted in d2.c */
    int active;                     /* 0x190 */
    int firstObj;                   /* 0x194, d2_HighestObj - 1 when the view was opened */
    int loaded;                     /* 0x198, its two surfaces exist (D2VIEW.loaded) */
    int shown;                      /* 0x19c, the view is displayed (d2_OpenForm / d2_EnableViewObjects) */
    int enabled;                    /* 0x1a0, its objects are enabled (d2_SetViewEnabled) */
    int viewSurf;                   /* 0x1a4, the composed view surface (blitted to the DD surface); int as the
                                       original keeps the D2SURF pointer */
    int texSurf;                    /* 0x1a8, the two view textures stacked on one surface */
    char tex1[0x40];                /* 0x1ac */
    char tex2[0x40];                /* 0x1ec */
    char pad22c[0x80];
} D2VIEW;

#endif
