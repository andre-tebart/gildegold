/* d3_d3d.c types and globals (Direct3D 6 device, render states, textures).
   Names in the D3DCHECK call texts are original (d3d_Device, d3d_UsedCard, dd_DD, ...); the rest are ours.
   Field names unkXXX are byte offsets whose meaning is not known yet. */
#ifndef D3_D3D_H
#define D3_D3D_H
#include "d3.h"                       /* D3RECT and the D3 records */
#include "d3_types.h"                 /* D3COLOR */
#include <windows.h>                  /* Win32 types */
#include <d3d.h>                      /* Direct3D / DirectDraw types */

/* D3_COPY (src named directly in the asm) and D3_COPY_LABEL are in d3.h; these are its other operand shapes. */

/* Same with the destination named directly in the asm (e.g. offset global) and src through a temp. */
#define D3_COPY_TO(var, src, size) {                                \
    int _cn = (size); void *_cs = (src);                            \
    __asm mov edi, var                                              \
    __asm mov esi, _cs                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_COPY_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

/* Same with both operands through temps (size, src, dst in this order). */
#define D3_COPY3(dst, src, size) {                              \
    int _cn = (size); void *_cs = (src); void *_cd = (dst);     \
    __asm mov edi, _cd                                          \
    __asm mov esi, _cs                                          \
    __asm mov ebx, _cn                                          \
    __asm lea ecx, [edi+edi*2]                                  \
    __asm and ecx, 3                                            \
    __asm sub ebx, ecx                                          \
    __asm jle D3_COPY_LABEL(__LINE__)                           \
    __asm rep movsb                                             \
    __asm mov ecx, ebx                                          \
    __asm and ebx, 3                                            \
    __asm shr ecx, 2                                            \
    __asm rep movsd                                             \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                 \
    __asm rep movsb                                             \
}

/* Same with both operands named directly in the asm. */
#define D3_COPY_AT(dvar, svar, size) {                              \
    int _cn = (size);                                               \
    __asm mov edi, dvar                                             \
    __asm mov esi, svar                                             \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_COPY_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

typedef struct D3DCONFIG {          /* 12 bytes, render options; d3d_Config holds the ones set */
    unsigned char antialias : 1;
    unsigned char filter : 1;       /* bilinear */
    unsigned char dither : 1;
    unsigned char perspective : 1;
    unsigned char subpixel : 1;
    unsigned char unk5 : 1;
    unsigned char mirrors : 1;      /* bit 6: mirror reflections drawn (d3_Render, d3_MirDrawObject) */
    char pad01[3];
    int fillmode;                   /* 0x04 */
    int mipfilter;                  /* 0x08 */
} D3DCONFIG;

#define D3D_VERTEX_STRUCT D3DFVF_TLVERTEX

/* 24-bit source surface pixel layout used when an image is loaded: red in the lowest byte, then green, blue */
#define D3D_RGB24_RMASK 0xff
#define D3D_RGB24_GMASK 0xff00
#define D3D_RGB24_BMASK 0xff0000

typedef struct D3DTEXFMTCTX {       /* 0x50 bytes, d3d_UsedTexFormat */
    DWORD bpp;                      /* 0x00, wanted bit depth */
    DWORD fourcc;                   /* 0x04, wanted FOURCC, 0 = none */
    DDPIXELFORMAT pf;               /* 0x08, best format for bpp */
    DWORD bpp2;                     /* 0x28 */
    DDPIXELFORMAT pf2;              /* 0x2c, best format for bpp2 */
    char exact;                     /* 0x4c, pf2 has the back buffer's channel sizes */
    char same;                      /* 0x4d, pf and pf2 are the same */
    char found;                     /* 0x4e, fourcc found */
    char pad4f;
} D3DTEXFMTCTX;

typedef struct D3DZBUFCTX {         /* 0x28 bytes, d3d_UsedZBufferFormat */
    DWORD bpp;                      /* 0x00, wanted depth */
    DWORD unk04;
    DDPIXELFORMAT pf;               /* 0x08 */
} D3DZBUFCTX;

extern D3DTEXFMTCTX d3d_UsedTexFormat;      /* 0xfe79c0 */
extern D3DZBUFCTX d3d_UsedZBufferFormat;    /* 0xfe7620 */
extern D3DCONFIG d3d_Config;               /* 0xfe7670 */
struct D3DCARD; extern struct D3DCARD d3d_UsedCard;   /* 0xfe7680, the device in use (D3DCARD, dd_vesa.h) */
extern int d3d_NumDevices;                    /* 0x1464fa8, Dd_vesa.c */
extern D3DTLVERTEX d3d_PrecacheBuffer[3];   /* 0xfe7a20 */
extern void *d3d_VertexBuffer;              /* 0x633410, 0x800 bytes */
extern LPDIRECT3D3 d3d_D3D;                 /* 0x633414 */
extern LPDIRECT3DVIEWPORT3 d3d_Viewport;    /* 0x63341c */
extern LPDIRECTDRAWSURFACE4 d3d_ZBuffer;    /* 0x633420 */
#ifndef DATA_VIEWS_H
extern char d3d_ZEnabled;                   /* 0x63342d */
#endif
extern ULONG dd_Ref;                   /* 0x146356c, result of the last Release (dd_vesa.c) */
extern LPDIRECTDRAW4 dd_DD;                 /* 0x631204, Dd_vesa.c */
extern LPDIRECTDRAWSURFACE4 dd_BackBuffer;  /* 0x631208, Dd_vesa.c */
extern char dd_Cleared;                 /* 0x631222, Dd_vesa.c */
extern char dd_ZCleared;                    /* 0x631223, Dd_vesa.c */
extern unsigned char dd_RBits, dd_GBits, dd_BBits;   /* 0x1463570, 0x1463580, 0x1464fbd, dd_vesa.c (d3_d3d.c reads them signed: dd_RBitsC / dd_GBitsC / dd_BBitsC) */
extern D3COLORU d3_fog_color;               /* 0x6334a0, d3_engine.c */
extern int dd_Width, dd_Height;             /* 0x1463598, 0x1463594, Dd_vesa.c */
extern DWORD d3d_MinMipSize;                   /* 0x5f942c, smallest mip dimension kept (4) */
extern float d3d_ViewW;                 /* 0x5f9430, last viewport set, -1 = none */
extern float d3d_ViewH;                 /* 0x5f9434 */
extern float d3d_ViewX;                 /* 0x5f9438 */
extern float d3d_ViewY;                 /* 0x5f943c */
extern float d3d_Near;                  /* 0x5f9440, last projection set */
extern float d3d_Far;                   /* 0x5f9444 */
extern unsigned char d3d_zbuffer;                    /* 0x5f9448, z buffer enabled (render state cache) */
extern unsigned char d3_MipSkip;                 /* 0x63342c, shift applied to texture width/height, from d3_Cfg.texShift */
#ifndef DATA_VIEWS_H
extern unsigned char d3d_balpha;                     /* 0x63342e, alpha blending enabled */
#endif
#ifndef DATA_VIEWS_H
extern unsigned char d3d_glow;                       /* 0x63342f, additive blending */
#endif
extern unsigned char d3d_colorkey;                   /* 0x633430 */
extern unsigned char d3d_alphatest;                  /* 0x633431 */
extern unsigned char d3d_fog;                        /* 0x633432 */
extern char d3d_StatesSet;                  /* 0x633433 */
extern char d3d_Mipmaps;                    /* 0x633434, mipmapping usable: d3d_UsedCard.info.maxMipFilter > 1 */
extern int d3d_ZBufferCaps;                 /* 0xfe7608, memory caps for the z buffer surface: 0x800 system, 0x6000 video memory | 3D device */

HRESULT CALLBACK d3d_EnumDeviceCallback(GUID *guid, LPSTR desc, LPSTR name,
    LPD3DDEVICEDESC hal, LPD3DDEVICEDESC hel, LPVOID ctx);                             /* 0x41c460, Dd_vesa.c */
char dd_SetSurfaceName(LPDIRECTDRAWSURFACE4 surf, char *data, char unk);                      /* 0x41eb00, Dd_vesa.c */
void err_Message(char *msg);                                                               /* 0x4222b0 */
unsigned char bmp_Load(char *path, unsigned char *data, unsigned char *pal, int flags); /* 0x4014d0, bmp.h */
char dd_StringToGuid(char *s, GUID *guid);                                                 /* 0x41c760, Dd_vesa.c */
void dd_DeviceDialog(int bpp, int unused, HWND parent, char flag, LPDIRECT3D3 d3d3);              /* 0x41cf60, Dd_vesa.c */

char d3d_Restore(void);                                                /* 0x4370d0 */

/* declared in other units' headers before (moved to the owner) */
void d3d_Clear(D3RECT *r);  /* 0x435fb0 */
char d3d_CreateDevice(DWORD texbpp, DWORD fourcc, DWORD texbpp2, DWORD zbpp);  /* 0x436160 */
void d3d_DrawTriangles(void *unk, D3DTLVERTEX *buf, DWORD num);  /* 0x436510 */
void d3d_Exit(void);  /* 0x4370e0 */
int d3d_GetVertexBuffer(void **buf, int *n);  /* 0x4364f0 */
char d3d_Init(int bitplanes, int a, int b);  /* 0x435d70 */
void d3d_SetViewport(float w, float h, float x, float y, float zn, float zf);  /* 0x436de0 */
void d3d_SetWBuffer(char w);  /* 0x437060 */
void d3d_BuildMipmaps(LPDIRECTDRAWSURFACE4 tex, DDSURFACEDESC2 *ddsd);  /* 0x4371d0 */
void d3d_PrecacheTexture(LPDIRECT3DTEXTURE2 t);  /* 0x4360e0 */

#endif
