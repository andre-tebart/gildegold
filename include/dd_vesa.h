/* dd_vesa.h -- DirectDraw display layer (D:\develop\Units\dd_vesa\Dd_vesa.c, 0x41bdd0..0x41ebc0).
   Names in quotes come from the leaked D3DCHECK texts ("dd_DD", "dd_FrontBuffer", "dd_BackBuffer",
   "dd_Window", "dd_BitPlanes", "d3d_EnumDeviceCallback", "dd_EnumVideoModesCallback"); the rest are ours.
   Needs <windows.h>, <ddraw.h> and <d3d.h> first. */
#ifndef DD_VESA_H
#define DD_VESA_H
#include <windows.h>                  /* Win32 types */
#include <d3d.h>                      /* Direct3D / DirectDraw types */

/* dd_Mode: how the frame gets to the screen */
#define DD_GDI          0           /* DIB section blitted with BitBlt */
#define DD_WINDOW       1           /* windowed DirectDraw, back buffer blitted to the front buffer */
#define DD_WINDOWMEM    2           /* same, drawing into dd_MemBuff which is copied to the back buffer */
#define DD_FULLSCREEN   3           /* exclusive mode, page flipping */
#define DD_FULLMEM      4           /* same with dd_MemBuff */

typedef struct DDDEVICE {           /* 0x214 bytes, one per DirectDraw driver, 8 at dd_Devices */
    GUID *guid;                     /* NULL = primary display driver, else &guiddata */
    GUID guiddata;                  /* 0x04 */
    char desc[0x100];               /* 0x14 */
    char name[0x100];               /* 0x114 */
} DDDEVICE;

typedef struct D3DDEVINFO {         /* 0x338 bytes, one per Direct3D device, 8 at d3d_Devices */
    GUID guid;
    char desc[0x100];               /* 0x010 */
    char name[0x100];               /* 0x110 */
    D3DDEVICEDESC caps;             /* 0x210, hardware or HEL description */
    /* 0x30c..0x337 filled by d3d_GetDeviceInfo; the same 0x2c bytes as D3DCARDINFO in d3_d3d.h,
       whose field names the ints below take */
    unsigned char unk30c_0 : 1;
    unsigned char unk30c_1 : 1;
    unsigned char unk30c_2 : 1;
    unsigned char unk30c_3 : 1;
    unsigned char unk30c_4 : 1;
    unsigned char unk30c_5 : 1;
    unsigned char unk30c_6 : 1;
    unsigned char unk30c_7 : 1;
    unsigned char unk30d_0 : 1;
    unsigned char unk30d_1 : 1;
    unsigned char software : 1;      /* 0x30d bit 2, set when caps is the HEL description */
    unsigned char unk30d_3 : 1;
    unsigned char unk30d_4 : 1;
    unsigned char unk30d_5 : 1;
    char pad30e[2];
    int minw;
    int minh;
    int maxw;
    int maxh;
    int zmode;
    int trans_destblend;
    int trans_srcblend;
    int glow_destblend;
    int glow_srcblend;
    int maxMipFilter;
} D3DDEVINFO;

extern HWND dd_DevCombo;                        /* 0x6311f8, Direct3D device list in the dialog */
extern HWND dd_DrvCombo;                        /* 0x6311fc, DirectDraw driver list */
extern int dd_LastDriver;                       /* 0x5f3c48, initialized to -1 */



extern LPDIRECTDRAW dd_DDBase;                  /* 0x631200 */
extern LPDIRECTDRAW4 dd_DD;                     /* 0x631204 */
extern LPDIRECTDRAWSURFACE4 dd_BackBuffer;      /* 0x631208 */
extern int dd_Page;                             /* 0x63120c, toggled by every flip */
extern LPDIRECTDRAWSURFACE4 dd_FrontBuffer;     /* 0x631210 */
extern LPDIRECTDRAWCLIPPER lpClipper;          /* 0x631214 */
extern LPDIRECT3D3 dd_D3D3;                      /* 0x631218, enumerated by the dialog when dd_DlgFlag is set */
extern int dd_BitPlanes;                        /* 0x63121c */
extern char dd_HasMMX;                          /* 0x631220 */
extern char dd_TripleBuffer;                    /* 0x631221 */
extern char dd_Cleared;                         /* 0x631222 */
extern char dd_ZCleared;                       /* 0x631223 */
extern char dd_DlgFlag;                         /* 0x631224 */
extern char dd_Has800x600;                      /* 0x631225 */
extern char dd_Has1024x768;                     /* 0x631226 */
extern char dd_Has1152x864;                     /* 0x631227 */

extern int dd_BytesPerPixel;                    /* 0x1463568 */
extern ULONG dd_Ref;                            /* 0x146356c, result of the last Release */
extern unsigned char dd_RBits;                  /* 0x1463570 */
extern HWND dd_Window;                          /* 0x1463574 */
extern unsigned char dd_GShift;                 /* 0x1463578 */
extern unsigned char dd_RShift;                 /* 0x1463579 */
extern int dd_LockPitch;                        /* 0x146357c */
extern unsigned char dd_GBits;                  /* 0x1463580 */
#ifndef DATA_VIEWS_H
extern unsigned char dd_GLoss;                  /* 0x1463581 */
#endif
extern void *dd_MemBuff;                        /* 0x1463584 */
extern int dd_Pitch;                            /* 0x1463588, of dd_Surface in bytes */
extern int dd_MemSize;                          /* 0x146358c */
extern void *dd_Surface;                        /* 0x1463590, locked drawing surface */
extern int dd_Height;                           /* 0x1463594 */
extern int dd_Width;                            /* 0x1463598 */
extern unsigned int dd_BMask;                   /* 0x146359c */
extern void *dd_MemBottom;                      /* 0x14635a0, last row of a bottom-up DIB */
extern int dd_PixelFormat;                       /* 0x14635a4, colour format code 888 - RLoss*100 - GLoss*10 - BLoss (888 / 565 / 555) */
extern unsigned int dd_Unk14635a8;              /* 0x14635a8 */
extern int dd_Mode;                             /* 0x14635ac, DD_GDI.. */
extern int dd_NumDevices;                       /* 0x14635b0 */
extern RECT dd_ClipRect;                        /* 0x14635c0 */
extern D3DDEVINFO d3d_Devices[8];               /* 0x14635e0 */
extern int dd_LockBytes;                        /* 0x1464fa0 */
extern unsigned int dd_RMask;                   /* 0x1464fa4 */
extern int d3d_NumDevices;                      /* 0x1464fa8 */
extern int dd_MemPitch;                         /* 0x1464fac */
extern unsigned int dd_GMask;                   /* 0x1464fb0 */
extern int dd_LockBits;                         /* 0x1464fb4 */
extern unsigned int dd_Unk1464fb8;              /* 0x1464fb8 */
extern unsigned char dd_BShift;                 /* 0x1464fbc */
extern unsigned char dd_BBits;                  /* 0x1464fbd */
extern int dd_Unk1464fc0;                       /* 0x1464fc0 */
extern unsigned char dd_BLoss;                  /* 0x1464fc4 */
extern HBITMAP dd_Bitmap;                       /* 0x14650e0 */
extern unsigned char dd_RLoss;                  /* 0x14650e4 */
extern DDDEVICE dd_Devices[8];                  /* 0x1465100 */
extern char dd_Unk1464fe0[0x100];                      /* 0x1464fe0, passed to d3_InitKeyCodes */
extern int dd_PitchPixels;                      /* 0x14661a0 */

/* outside this file */
void d3_InitKeyCodes(unsigned char *p);                                                              /* 0x418ce0 */

char dd_Restore(void);                                                                  /* 0x41bdd0 */
void dd_Exit(void);                                                                     /* 0x41be00 */
BOOL WINAPI dd_EnumDriverCallback(GUID *guid, LPSTR desc, LPSTR name, LPVOID ctx, HMONITOR mon); /* 0x41bf80 */
void d3d_GetDeviceInfo(D3DDEVICEDESC *caps, D3DDEVINFO *info);                          /* 0x41c080 */
DWORD dd_BppToDDBD(int bpp);                                                            /* 0x41c420 */
HRESULT WINAPI d3d_EnumDeviceCallback(GUID *guid, LPSTR desc, LPSTR name,
                                      LPD3DDEVICEDESC hw, LPD3DDEVICEDESC hel, LPVOID ctx); /* 0x41c460 */
char *dd_GuidToString(GUID *guid, char *s);                                             /* 0x41c6f0 */
char dd_StringToGuid(char *s, GUID *guid);                                              /* 0x41c760 */
BOOL CALLBACK dd_DlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp);                     /* 0x41c830 */
void dd_DeviceDialog(int bpp, int unused, HWND parent, char flag, LPDIRECT3D3 d3d3);            /* 0x41cf60 */
char dd_Init(int mode, int bpp, HWND parent, int unk);                                      /* 0x41cfb0 */
void dd_SetClipRect(int left, int top, int right, int bottom);                          /* 0x41d190 */
HRESULT WINAPI dd_EnumVideoModesCallback(LPDDSURFACEDESC2 ddsd, LPVOID ctx);            /* 0x41d1c0 */
char dd_CheckModes(int mode, int bpp, HWND parent, int c);                              /* 0x41d230 */
char dd_SetMode(int xres, int yres, int bpp, int mode, HWND wnd, int unk6, char triple);    /* 0x41d2e0 */
void *dd_LockSurface(LPDIRECTDRAWSURFACE4 surf);                                        /* 0x41ddb0 */
void * dd_LockSurfaceFlags(LPDIRECTDRAWSURFACE4 surf, DWORD flags);                      /* 0x41de30 */
char dd_Lock(void);                                                                     /* 0x41dec0 */
char dd_LockNoWait(void);                                                               /* 0x41df80 */
void dd_Unlock(void);                                                                   /* 0x41e050 */
char dd_FillSurface(LPDIRECTDRAWSURFACE4 surf, RECT *rect, DWORD color);                /* 0x41e0a0 */
void dd_Clear(RECT *rect, int r, int g, int b);                                         /* 0x41e100, NULL = everything */
void dd_Flip(void);                                                                     /* 0x41e390 */
DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b);                        /* 0x41e670 */
void dd_SplitRGB(DWORD c, unsigned char *r, unsigned char *g, unsigned char *b);        /* 0x41e6d0 */
void dd_ReleaseSurface(LPDIRECTDRAWSURFACE4 surf);                                      /* 0x41e720 */
void dd_UnlockSurface(LPDIRECTDRAWSURFACE4 surf);                                       /* 0x41e740 */
void dd_DrawLine16(int x0, int y0, int x1, int y1, int color);              /* 0x41e760 */
void dd_Line(int x0, int y0, int x1, int y1, int color);                    /* 0x41e8f0 */
void dd_GetMaskShifts(DWORD rmask, DWORD gmask, DWORD bmask, unsigned char *rloss, unsigned char *rshift,
                      unsigned char *gloss, unsigned char *gshift, unsigned char *bloss, unsigned char *bshift); /* 0x41e920 */
void dd_MakeColorTables(DWORD rmask, DWORD gmask, DWORD bmask, DWORD *tab);              /* 0x41ea20 */
char dd_SetSurfaceName(LPDIRECTDRAWSURFACE4 surf, char *data, char unk);               /* 0x41eb00 */


/* D3_ZERO and D3_COPY with the destination and the size named directly in the asm (plain variables or globals,
   no temps); used where the original keeps the pointer and size in memory across a loop. */
#define DD_ZERO_LABEL(n) D3_ZERO_CAT(dd_zero_tail, n)
#define DD_ZERO(dst, size) {                                        \
    __asm mov edi, dst                                              \
    __asm mov ebx, size                                             \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle DD_ZERO_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm DD_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}
#define DD_COPY_LABEL(n) D3_ZERO_CAT(dd_copy_tail, n)
#define DD_COPY(dst, src, size) {                                   \
    __asm mov edi, dst                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, size                                             \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle DD_COPY_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm DD_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}
#endif
