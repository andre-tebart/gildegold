/* dd_vesa_types.h -- the records and mode numbers of the DirectDraw display layer (dd_vesa.h), for the units
   that use them without its functions (d3_interface.c reads d3d_UsedCard's D3DCARD). Needs <windows.h> and <d3d.h>. */
#ifndef DD_VESA_TYPES_H
#define DD_VESA_TYPES_H
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

typedef struct D3DCARDINFO {        /* 0x2c bytes, the device description condensed by d3d_GetDeviceInfo */
    unsigned char alphablend : 1;   /* 0x00 bit 0, alpha gouraud or flat blending */
    unsigned char stipple : 1;      /* bit 1, alpha stippling or a stipple raster (only asked without alphablend) */
    unsigned char perspective : 1;  /* bit 2, perspective correct textures */
    unsigned char clipping : 1;     /* bit 3, the device clips */
    unsigned char bilinear : 1;     /* bit 4, linear texture filtering */
    unsigned char zbuffer : 1;      /* bit 5, a z buffer is needed (not ZBUFFERLESSHSR) */
    unsigned char antialias : 1;    /* bit 6, sort independent antialiasing */
    unsigned char dither : 1;       /* bit 7 */
    unsigned char subpixel : 1;     /* 0x01 bit 0 */
    unsigned char multitexture : 1; /* bit 1, two simultaneous textures (hardware only) */
    unsigned char software : 1;     /* bit 2, the description is the HEL's: textures in system memory */
    unsigned char stippled : 1;     /* bit 3, stipple without alphablend: transparency by stippling */
    unsigned char wbuffer : 1;      /* bit 4, a w buffer (zmode 2 when it is asked for) */
    unsigned char fogvertex : 1;    /* bit 5 */
    char pad02[0x2];
    DWORD minw, minh;               /* 0x04, texture size limits */
    DWORD maxw, maxh;               /* 0x0c */
    int zmode;                      /* 0x14, D3DRENDERSTATE_ZENABLE value */
    int trans_destblend;            /* 0x18 */
    int trans_srcblend;             /* 0x1c */
    int glow_destblend;             /* 0x20 */
    int glow_srcblend;              /* 0x24 */
    int maxMipFilter;               /* 0x28, highest D3DTSS_MIPFILTER value the card supports */
} D3DCARDINFO;

typedef struct D3DCARD {            /* 0x338 bytes, one Direct3D device: 8 at d3d_Devices (d3d_EnumDeviceCallback),
                                       the one in use copied to d3d_UsedCard (d3_d3d.c) */
    GUID Guid;                      /* the name the D3DCHECK texts leak ("&d3d_UsedCard.Guid") */
    char desc[0x100];               /* 0x010 */
    char name[0x100];               /* 0x110 */
    D3DDEVICEDESC caps;             /* 0x210, hardware or HEL description */
    D3DCARDINFO info;               /* 0x30c */
} D3DCARD;

#endif
