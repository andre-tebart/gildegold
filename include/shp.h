struct D2SHP;
struct D2FRAME;
struct D2SURF;
/* Declarations of unk_47ab30.c (0x47ab30..0x47c5e0, the shape ("shp") part of D2.C): grabbing a rectangle of a
   surface into a run-length coded shape, clipped drawing of shapes, shape conversion between 8 / 16 / 24 bit.
   d2.h lacks these. Names are ours. */
#ifndef UNK_47AB30_H
#define UNK_47AB30_H
#include "d2_types.h"                 /* D2SURF */
#include "d3.h"                       /* the D3 records and typedefs this header names */
struct D2RGB;

#pragma pack(push, 1)
typedef struct D2SHP {              /* a shape: this 0x32 byte header, then the lines */
    unsigned int size;              /* 0x00, bytes used */
    unsigned short x0, w, y0, h;    /* 0x04, bounding box (w, h = right / bottom + 1) */
    unsigned char type;             /* 0x0c, 0 = 8 bit, 1 = 16 bit, 2 = 24 bit */
    unsigned char mode;             /* 0x0d, 0 = copy, 1 = half dark (d2_ShpBlit16HalfDark / 47bec0), 2, 3 */
    unsigned short ax0, ay0, ax1, ay1;  /* 0x0e */
    unsigned short bx0, by0, bx1, by1;  /* 0x16, marker points (colours at 0x5fed74 / 0x5fed78) */
    char pad1e[8];
    int nruns;                      /* 0x26, number of runs, -1 = plain rows */
    int lineoff;                    /* 0x2a, offset of the table of line offsets */
    int npix;                       /* 0x2e, number of pixels in the runs */
} D2SHP;                            /* 0x32 */

typedef struct D2BANK {             /* a bank of shapes (0x845 byte header) */
    char pad00[0x2a];
    unsigned short n;               /* 0x2a, number of shapes */
    unsigned short w, h;            /* 0x2c, widest / highest shape added */
    int size;                       /* 0x30 */
    char kind;                      /* 0x34 */
    char pad35[9];
    int pal;                        /* 0x3e */
    char pad42;
    unsigned short npal;            /* 0x43 */
    int offs[0x200];                /* 0x45, offsets of the shapes */
} D2BANK;
#pragma pack(pop)

typedef struct D2KEY {              /* a colour key: 3 compared bytes, padded to a 4 byte stride */
    unsigned char r;
    unsigned short gb;
} D2KEY;

extern D2KEY d2_TransparentColour;          /* 0x6f0454 */
extern D2KEY d2_EndMarkerColour;          /* 0x5fed74 */
extern D2KEY d2_StartMarkerColour;          /* 0x5fed78 */
extern unsigned char d2_Palette[256][4];  /* 0xf8a120, palette */
#ifndef DATA_VIEWS_H
extern unsigned short *d2_ShpLightTable;    /* 0x6f0460, colour table */
#endif
extern int d2_SurfPitch;            /* 0x5fed84 */
extern int d2_LightTableExternal;            /* 0xf8a630 */

void d2_SetLineTable(int *lines, D2SHP *s);                                    /* 0x479f10 */
struct D2SHP * d2_GrabShape24Runs(int x, int y, int w, int h, unsigned char *pix, unsigned char flag, unsigned short pitch); /* 0x479f40 */
struct D2SHP * d2_GrabShape24NoRuns(int x, int y, int w, int h, unsigned char *pix, unsigned char flag, unsigned short pitch); /* 0x47a460 */
struct D2SHP * d2_GrabShape16Runs(int x, int y, int w, int h, unsigned short *pix, unsigned char flag, unsigned short pitch); /* 0x47a7d0 */
void d2_ShpBlit16ClipY(int x, int y, void *bits, D2SHP *s);   /* 0x47cf00, 16 bit shape, clipped top / bottom */
void d2_ShpBlit16ClipXY(int x, int y, void *bits, D2SHP *s);   /* 0x47d210, 16 bit shape, clipped left / right too */
void d2_ShpBlit8(int x, int y, void *bits, D2SHP *s);                      /* 0x47d600 */
void d2_ShpBlit8To16(int x, int y, void *bits, D2SHP *s);                      /* 0x47d710 */
int d2_BankAppendShape(D2SHP *s, D2BANK *bank);                                   /* 0x47c610 */

D2SHP *d2_GrabShape16NoRuns(int x, int y, int w, int h, unsigned short *src, unsigned char mode, unsigned short pitch);
D2SHP *d2_GrabShape8Runs(int x, int y, int w, int h, unsigned char *src, unsigned char mode, unsigned short pitch);
struct D2SHP * d2_GrabShape(int x, int y, int w, int h, struct D2SURF *surf, unsigned char mode, char grab8);
int d2_ShpBlit16Scaled(int x, int y, struct D2SURF *surf, struct D2SHP *s, unsigned char n);
int d2_ShpBlit16ScaledLit(int x, unsigned int y, struct D2SURF *surf, struct D2SHP *s, unsigned int n);
void d2_ShpBlit16HalfDark(unsigned int x, unsigned int y, D2SURF *surf, D2SHP *s);
void d2_ShpBlit16Rows(int x, int y, D2SURF *surf, D2SHP *s);
int d2_ShpBlit(int x, int y, D2SURF *surf, D2SHP *s);          /* 0x47bd70, draws a shape by its type */
D2SHP *d2_ShpConvert8To16(D2SHP *s);
D2SHP *d2_ShpConvert24To16(D2SHP *s);
void *d2_ShpConvertTo16(D2SHP *s, int p);


/* views of gfx.c functions (unk_479610.h until layout 2026-10-22; gfx.h declares 0x478930 / 0x478960 with
   char * / D2RGB * parameters) */
struct D2SURF * d2_CopySurface(struct D2SURF *surf);                         /* 0x479780 */
int d2_MakeLightTable(char add);                                 /* 0x479d00 */

/* declared in other units' headers before (moved to the owner) */
extern unsigned char d2_BlitN;  /* 0xf8a648 */
extern unsigned int d2_HighBitMask32;  /* 0xf8a640 */
extern unsigned int d2_LastAnimTick;  /* 0x6f0464 */
extern unsigned int d2_Mode5Colour;  /* 0xf8a638 */
extern int d2_ShpRowGap;  /* 0xf8a114 */
extern int d2_ShpRowX;  /* 0xf8a110 */
extern int d2_ShpX;  /* 0xf8a108 */
extern int d2_ShpY;  /* 0xf8a10c */
int d2_BankSetPalette(D2BANK *bank, void *pal);  /* 0x47c5e0 */
extern unsigned int d2_BlitH;  /* 0xf8a63c */
void d2_StepAnims(void);  /* 0x47cb90 */
int d2_SetColourFormat(int external, int paletteDepth);  /* 0x479df0 */
void d2_SetMode5Colour(unsigned char r, unsigned char g, unsigned char b);  /* 0x47ca10 */
int d2_StartAnim(short x, short y, void *data, unsigned char z);  /* 0x47cd20 */
int d2_DrawShape(int x, int y, void *surf, void *pic, unsigned char i);  /* 0x47c6f0 */
void d2_SetClipRect(int x0, int y0, int x1, int y1);  /* 0x47cb40 */
void *d2_BankConvertTo16(void *pic, int a, int b);  /* 0x47c410 */
void d2_DrawAnim(int n, void *surf);  /* 0x47cdb0 */
int d2_DrawShapeHalfDark(int x, int y, void *surf, void *pic, char i);  /* 0x47c770 */
int d2_DrawShapeMode2(int x, int y, void *surf, void *pic, unsigned char i);  /* 0x47c970 */
int d2_DrawShapeMode3(int x, int y, void *surf, void *pic, char i);  /* 0x47cae0 */
int d2_DrawShapeMode5(int x, int y, void *surf, void *pic, unsigned char i);  /* 0x47ca40 */
int d2_DrawShapeScaled(int x, int y, void *surf, void *pic, char i, char mode, int on);  /* 0x47c830 */
void d2_FillRect(int x, int y, int w, int h, void *surf);  /* 0x47ce60 */
void d2_FreeAnim(int n);  /* 0x47ce30 */
void d2_FreeLightTable(void);  /* 0x479ee0 */
struct D2FRAME *d2_GetFrame(void *pic, short i);  /* 0x47cb70 */
void d2_SetBlitParams(void *dst, void *src, void *mask, short w, short h, short n);  /* 0x47ced0 */

#endif
