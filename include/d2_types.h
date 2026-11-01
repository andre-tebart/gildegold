/* The record types of d2.c's tables d2_Wins, d2_Inputs, d2_Radios, d2_Views, d2_Pics, d2_Objs (and D2TEX / D2SURF /
   D2CTRL they use), for the
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
    short drawFrames;               /* 0x268, 2 after the window is created (d2_CreateWindow) or its text grew (io_PrintText); == 1 with flags 0x1000 also draws it to d2_BackSurf (a reasoned guess: the windows' D2OBJ.drawFrames) */
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
    int args[0x40];                 /* 0x284, parameters of the text commands ("%l" "%t" "%m" "%i" tokens of io_PrintText) */
    int nargs;                      /* 0x384, args used */
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

typedef struct D2PIC {              /* 0x54 bytes, one picture of the archive; d2_Pics has 0x800 */
    char name[0x30];
    int offset;                     /* 0x30, file offset of the data */
    void *data;                     /* 0x34, NULL = not loaded */
    int size;                       /* 0x38 */
    int type;                       /* 0x3c, 5 and 8 = frame of another picture */
    int refs;                       /* 0x40, reference count (0 = may be dropped) */
    unsigned int flags;             /* 0x44, 1 = locked, 2 = relative to d2_PicBase */
    unsigned int lastuse;           /* 0x48 */
    int base;                       /* 0x4c, picture holding the data of a frame */
    short w;                        /* 0x50 */
    short h;                        /* 0x52 */
} D2PIC;

typedef struct D2CTRL {             /* what D2OBJ::buf points to (a D2BUF) */
    int view;                       /* 0x00, the view index (d2_RedrawView; a D2VIEW's id) */
    char pad004[0x194];
    int loaded;                     /* 0x198, nonzero: its two surfaces exist (D2VIEW.loaded), so the
                                       pointer position is d2_PointerX / 0c, not the mouse */
} D2CTRL;

typedef struct D2OBJ {              /* 0x2ec bytes, d2_Objs has 0x2ff: pictures, inputs, sliders, buttons */
    int id;                         /* 0x00, own index */
    int used;                       /* 0x04 */
    int pic;                        /* 0x08, index into d2_Pics */
    void *data;                     /* 0x0c, picture data / sprite set / input */
    short x, y, w, h;               /* 0x10 */
    int type;                       /* 0x18 */
    short drawFrames;               /* 0x1c, frames left to draw: 2 right after creation (the first draw
                                       also goes to the back buffer), 1 normal, 0 stops */
    short clipx0, clipx1;           /* 0x1e */
    short clipy0, clipy1;           /* 0x22 */
    short pad26;
    int checked;                    /* 0x28, the checkbox / radio state (d2_SetObjValue / d2_GetObjValue), a click toggles it */
    int highlighted;                /* 0x2c, 1: drawn in the highlight colour */
    D2WIN *win;                     /* 0x30 */
    int unk34;
    int disabled;                   /* 0x38, 1: the object does not take the mouse */
    int clickDisabled;              /* 0x3c, 1: not clickable right now (set with greyed by d2_SetObjEnabled /
                                       d2_EnableViewObjects: the view or scroll button is inactive) */
    D2CTRL *buf;                    /* 0x40, the object's drawing buffer (D2BUF); 0: none */
    int textShadow;                 /* 0x44, 1: the object's text is drawn with a shadow (d2_DrawTextBox flag 4) */
    int checkable;                  /* 0x48, nonzero: a checkbox / radio button, its state is in checked and a
                                       click toggles it (d2_SetObjValue); not 'interactive' */
    int interactive;                /* 0x4c, nonzero: takes mouse clicks and highlighting (sliders,
                                       type 9 dialogs, text 'b' buttons, type 8 pictures) */
    int greyed;                     /* 0x50, 1: drawn in the inactive colour 9 / flags 9 (set with
                                       clickDisabled) */
    int hitTestable;                /* 0x54, nonzero: takes part in the hit test even when not
                                       interactive / checkable (only reader known, writer in the original) */
    int unk58;                      /* 0x58 */
    int centred;                    /* 0x5c, centred (d2_CenterObj) */
    int rightAligned;               /* 0x60, 1: the object's text is right aligned (d2_DrawTextBox flag 0x10) */
    int dirty;                      /* 0x64 */
    int hidden;                     /* 0x68, nonzero: skipped by the window's text layout and not
                                       drawn by the text-case (an input / link object in the text draws itself) */
    unsigned char shadowX, shadowY; /* 0x6c, shadow offset x / y (drawn at x + shadowX) */
    char unk6e;                     /* 0x6e */
    char pad6f;
    int picScale;                   /* 0x70, picture scale: 0 / 1 / -1 draw unscaled, > 1 larger, < -1 smaller (d2_ScaleObject) */
    short font;                     /* 0x74 */
    short pad76;
    int input;                      /* 0x78, input index / picture shift */
    union {
        char text[0x20];            /* 0x7c, button caption */
        unsigned short *raw;        /* 0x7c, 16-bit pixels of a raw object (d2_OpenRaw) */
        struct {
            int value;              /* 0x7c */
            int min;                /* 0x80 */
            int max;                /* 0x84 */
            int flags;              /* 0x88 */
            int len;                /* 0x8c, track length in pixels */
            int value2;             /* 0x90 */
            int firstPic;           /* 0x94, first picture of the slider (its end cap) */
            void *img;              /* 0x98, track image (surface) */
        } s;
    } u;
    union {
        struct {
            char label[0x40];       /* 0x9c; label[0x20] (0xbc) is the caption below */
            char list[0xe4];        /* 0xdc, "a|b|c|" */
        };
        struct {
            char pad9c[0x20];
            char caption[0x100];    /* 0xbc, caption of a picture / button object */
            int capfont;            /* 0x1bc, its font (0: default) */
        };
        struct {
            char pad9c2[0x24];
            int insertedLen;        /* 0xc0, length of the text this input object last inserted into the
                                       window text (d2_ReplaceInputText removes it again before the next) */
        };
    };
    int style;                      /* 0x1c0 */
    int picW;                       /* 0x1c4, whole picture width (o->w / h are the frame's) */
    int picH;                       /* 0x1c8, whole picture height */
    int captionX;                   /* 0x1cc, caption / text x offset, -1: default */
    int captionY;                   /* 0x1d0, caption / text y offset, -1: default */
    int icon;                       /* 0x1d4, -1: none */
    int iconx;                      /* 0x1d8 */
    int icony;                      /* 0x1dc */
    int winIndex;                   /* 0x1e0, index of the window that owns the object */
    float fillFraction;             /* 0x1e4, 0: not a fill object; else the fill factor of the picture: the
                                       part below (1 - fillFraction) * h is clipped away (so 1.0 leaves only the
                                       top edge) and the value drawn under the picture is fillFraction * 100
                                       percent */
    char pad1e8[0xf8];
    char hilo;                      /* 0x2e0 */
    char hihi;                      /* 0x2e1 */
    short pad2e2;
    int scale;                      /* 0x2e4 */
    void *user;                     /* 0x2e8, unused by d2: a dialog keeps its own record of the object here
                                       (unk_4b39f0's mission type, unk_5d33b0's gypsy row) */
} D2OBJ;

#endif
