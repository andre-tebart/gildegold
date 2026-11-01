/* d3_fl.c -- terrain ("floor") texture cache, allocation tag prefix "d3_fl:" (so probably d3_fl.c;
   no path string). Names are ours. */
#ifndef UNK_43EAB0_H
#define UNK_43EAB0_H
struct XOBJ;

typedef struct D3FLTEX {            /* view of the texture slot (TSTEXTURE in ts_texture.h) */
    char name[0x40];                /* "*fltx1/2/4" for cache textures */
    int refs;                       /* 0x40 */
    char pad44[0x8];
    struct D3FLCACHE *cache;        /* 0x4c */
    char pad50[0x4];
    unsigned long stamp;            /* 0x54, d3_FrameCount of the last use */
    int unk58;                      /* 0x58 */
    char pad5c[0x4];
    void *surf;                     /* 0x60, IDirectDrawSurface4 */
    void *d3dtex;                   /* 0x64, IDirect3DTexture2 */
    signed char state;              /* 0x68, bit 7: precached */
    char pad69[0x3];
    D3TEXFLAGS flags;               /* 0x6c, the flags tex_Load was called with */
    unsigned char frames;           /* 0x70, animation frames */
    unsigned char frame;
    unsigned char speed : 4;        /* 0x72, index into d3_AnimDiv */
    unsigned char : 4;
    char pad73[0xd];
} D3FLTEX;

typedef unsigned char *D3FLSRC[8];   /* source textures of the 8 tile types, 64x64 pixels of 4 bytes */

typedef struct D3FLTILES {         /* 0x30 bytes, the input of d3fl_Render */
    D3FLSRC *src;                   /* 3 mip levels (64, 32, 16 pixels per tile) */
    int x, y;                       /* 0x04, tile position in the map */
    unsigned int texsize;           /* 0x0c, of the texture drawn into */
    unsigned int n;                 /* 0x10, tiles per side */
    unsigned char tiles[5][5];      /* 0x14, (n + 1)^2 tile types */
    char pad2d[0x3];
} D3FLTILES;

typedef struct D3FLCACHE {          /* 0x44 bytes, one cached terrain texture */
    D3FLTEX *tex;
    unsigned int stamp;             /* 0x04, d3_FrameCount (frame) of the last use */
    unsigned int size;              /* 0x08, tiles per side (1, 2 or 4) */
    int hash;                       /* 0x0c, of the tile indices, + d3_CurrentScene */
    D3FLTILES t;                    /* 0x10, what d3fl_Render draws */
    char flag;                      /* 0x40 */
    char pad41[0x3];
} D3FLCACHE;

typedef struct D3FLUV {             /* a texture coordinate of d3fl_Jitter, 3 per triangle */
    float u, v;
} D3FLUV;
#define D3FL_2PI 6.2831855f

typedef void (*D3FLBLEND)(unsigned short *dst, unsigned long *w, unsigned char **src, unsigned int n);

/* ---- terrain map (d3_Terrain, D3SCENE.map); layout as far as this file uses it ---- */
typedef struct D3FLPOINT {          /* 0x50 bytes, a tile vertex (D3PVERT as the floor code builds it) */
    char pad00[0x40];
    unsigned long color;            /* 0x40, D3PVERT.color (D3COLOR b,g,r,a packed) */
    unsigned long specular;         /* 0x44, D3PVERT.specular */
    void *src;                      /* 0x48, D3PVERT.src: source point (D3SRCPOINT) */
    char flags;                     /* 0x4c, D3PVERT.flags: bit 7 = visible */
    char pad4d[0x3];
} D3FLPOINT;

#define D3FL_WHITE 0xffffffL        /* a D3FLPOINT color: r = g = b = 255, alpha 0 */

typedef struct D3FLPOLY {           /* 0x28 bytes, a triangle (D3PTRI as the floor code sees it) */
    D3FLPOINT *v[3];
    void *clip;                     /* 0x0c, D3PTRI.clip: &d3_ClipInfo[clip code] */
    float *uv;                      /* 0x10, d3fl_UV[0] or [1] */
    D3FLTEX *tex;                   /* 0x14, D3PTRI.tex: d3fl_GetTexture / D3FLREGION.tex2 */
    float zOfs;                     /* 0x18, depth offset added to the vertices (engine d3_ZOfs) */
    float uOfs;                     /* 0x1c, texture offset u (D3FLREGION.uOfs, mod 1) */
    float vOfs;                     /* 0x20, texture offset v (D3FLREGION.vOfs, mod 1) */
    char flags;                     /* 0x24, bit 7: visible */
    unsigned char region;           /* 0x25, water region */
    unsigned char flip;             /* 0x26, bit 0: split along the other diagonal (d3fl_Draw) */
    char pad27;
} D3FLPOLY;

typedef struct D3FLWREG {           /* 0x14 bytes, a water row of a tile; the list ends with y < 0 */
    int idx;                        /* first point */
    int y;
    int x0, x1;
    char flag;                      /* 0x10 */
    char pad11[0x3];
} D3FLWREG;

typedef struct D3FLTILE {           /* 0x64 bytes, 8 x 8 of them in D3FLMAP */
    D3VECTOR4 center;               /* 0x00, of the world bounding box */
    int npoints;                    /* 0x10, points of this tile used by d3fl_Draw (nwpoints: water) */
    int nwpoints;                   /* 0x14 */
    D3FLPOINT *points;              /* 0x18 */
    D3FLPOINT *wpoints;             /* 0x1c */
    int npolys;                     /* 0x20, triangles of this tile used by d3fl_Draw (nwpolys: water) */
    int nwpolys;                    /* 0x24 */
    D3FLPOLY *polys;                /* 0x28 */
    D3FLPOLY *wpolys;               /* 0x2c */
    void *split;                    /* 0x30 */
    D3FLWREG *wregions;             /* 0x34 */
    int nsplit;                     /* 0x38, D3FLSPLIT records in split (the extra edge vertices, d3fl_Draw) */
    void *bp;                       /* 0x3c */
    int nbpUsed;                    /* 0x40, uv triples of bp in use (the split triangles, d3fl_Draw) */
    int nbp;                        /* 0x44 */
    float lodDistBias;              /* 0x48, added to both lod distance thresholds (nothing writes it: 0) */
    float centerDist;               /* 0x4c, length of center: the tile box's distance from the camera */
    float zmin, zmax;               /* 0x50, view depth range (d3_ClipBox) */
    int lodDelay;                   /* 0x58, frames the pending lod change has been postponed (switches after 8) */
    unsigned char hmin;             /* 0x5c */
    unsigned char hmax;             /* 0x5d */
    unsigned char lodLevel;         /* 0x5e, detail level chosen this frame (0: culled; d3fl_UpdateTiles) */
    char lod;                       /* 0x5f, level the current mesh was built at */
    char water;                     /* 0x60 */
    char nextLod;                   /* 0x61, level to switch to once lodDelay runs out */
    union {
        unsigned char clip;         /* 0x62, clip flags of the box (0x40: invisible) */
        D3CLIP clipCode;            /* 0x62, the same byte as the D3CLIP code d3_ClipMesh takes (d3_engine.h) */
    };
    char pad63;
} D3FLTILE;

typedef struct D3FLREGION {         /* 0x158 bytes, a water region */
    D3FLTEX *tex;
    D3FLTEX *tex2;                  /* 0x04 */
    D3TEXFLAGS flags;               /* 0x08 */
    float scrollSpeedU;             /* 0x0c, speed of the texture offset uOfs */
    float scrollSpeedV;             /* 0x10, speed of the texture offset vOfs */
    float f14;
    float speed[4];                 /* 0x18, of the wave phases */
    float amp[4];                   /* 0x28, wave amplitudes */
    float wave[4][4][4];            /* 0x38, 16 wave vectors */
    float wavePhase[4];             /* 0x138, wave phases (mod 2 pi) */
    float uOfs;                     /* 0x148, texture offset u (mod 1), scrolled by scrollSpeedU */
    float vOfs;                     /* 0x14c, texture offset v (mod 1), scrolled by scrollSpeedV */
    unsigned long time;             /* 0x150 */
    int water;                      /* 0x154, water type, 0 = none */
} D3FLREGION;

typedef struct D3FLMAP {
    int size;                       /* 0x00, height map side */
    int n;                          /* 0x04, points per tile side */
    int idxMask;                    /* 0x08, mask of a flat map index: size*size - 1 | size - 1 */
    unsigned int mask;              /* 0x0c, size - 1 */
    unsigned char *height;          /* 0x10 */
    unsigned char *types;           /* 0x14, tile type per map point */
    unsigned char *water;           /* 0x18 */
    unsigned char *light;           /* 0x1c */
    void *lightofs;                 /* 0x20, "d3_fl:LightOffset", 4 bytes per map point */
    unsigned char *divide[3];       /* 0x24 */
    D3FLSRC src[3];                 /* 0x30 */
    /* 0x90..0xcc: the map transform, three axis vectors + origin (D3MATRIX rows; LoadMap writes it through
       a D3MATRIX pointer): xAxis = map x in world space, zAxis = map y, yAxis = height */
    float orgx, orgy, orgz;         /* 0x90, world position of map point (0, 0), height 0 */
    char pad9c[0x4];
    float scale;                    /* 0xa0, world x per map x (xAxis.x) */
    float xAxisY, xAxisZ;           /* 0xa4, xAxis.y / xAxis.z */
    char padac[0x4];
    float zAxisX, zAxisY;           /* 0xb0, map y axis in world space */
    float negScale;                 /* 0xb8, -scale (zAxis.z) */
    char padbc[0x4];
    float yAxisX;                   /* 0xc0, height axis in world space */
    float hscale;                   /* 0xc4, world height per height unit (yAxis.y) */
    float yAxisZ;                   /* 0xc8 */
    char padcc[0x4];
    float ambient[3];               /* 0xd0 */
    char paddc[0x4];
    D3FLTILE tiles[8][8];           /* 0xe0 */
    D3FLREGION *regions;            /* 0x19e0 */
    char name[0x40];                /* 0x19e4, D3FLDESC.name */
    char texname[0x40];             /* 0x1a24, D3FLDESC.texname */
    char typenames[8][0x40];        /* 0x1a64, texture names of the tile types */
    float lodDist2;                 /* 0x1c64, 20: distance above which a tile gets level 2 */
    float lodDist4;                 /* 0x1c68, 40: distance above which a tile gets level 4 */
    unsigned char watertype;        /* 0x1c6c, tile type of water ("WASSER"), 0xff = none */
    unsigned char nregions;         /* 0x1c6d */
    char pad1c6e[0x2];
    unsigned char dirty : 1;        /* 0x1c70 */
    unsigned char nolight : 1;
    unsigned char lod : 3;
    unsigned char : 3;
    unsigned char shift : 4;        /* 0x1c71 */
    unsigned char : 4;
    char pad1c72[0x6];
} D3FLMAP;

extern float d3fl_LightMargin;            /* 0x633740, world units added to map->scale when a light's
                                             ground radius is converted to map cells (d3fl_LightCB; no writer) */
char map_Pick(D3FLMAP *map, D3VECTOR *pos, char lod, int *x, int *y, float *h);      /* 0x4463b0 */
extern D3VECTOR d3fl_Down;                  /* 0x5ebb74, {0, 0, 1} */
extern float d3fl_Shade[1024];              /* 0xfb9940, sun shade by -cos(angle) * 1023 */
/* float to int with fistp (current rounding mode), as d3_FtoI in d3_engine.h */
__inline void d3fl_FtoI(int *dst, float f)
{
    __asm fld f
    __asm mov edx, dst
    __asm fistp dword ptr [edx]
}
__inline void d3fl_FtoI2(float f, int *dst)
{
    __asm fld f
    __asm mov edx, dst
    __asm fistp dword ptr [edx]
}
struct TSTEXTURE *tex_Find(char *name);                                                 /* 0x45f980, ts_texture.c (d3_fl.c reads it through its D3FLTEX view tex_FindF) */
struct TSTEXTURE; void tex_AddRef(struct TSTEXTURE *tex, char all);                                                   /* 0x4601b0, ts_texture.c */
/* Inline-asm fill in the style of D3_ZERO (d3.h): size and value through temps (declared in this order),
   the destination named directly in the asm (as D3E_FILL in d3_engine.h). */
#define D3FL_FILL(dvar, val, size) {                                \
    int _fn = (size); int _fv = (val);                              \
    __asm mov edi, dvar                                             \
    __asm mov eax, _fv                                              \
    __asm mov ebx, _fn                                              \
    __asm mov ah, al                                                \
    __asm shl eax, 16                                               \
    __asm mov ecx, eax                                              \
    __asm shr ecx, 16                                               \
    __asm add eax, ecx                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_COPY_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}
/* Inline-asm copy with the destination named directly in the asm, size and source through temps
   (as D3_COPY_TO in d3_d3d.h). */
#define D3FL_COPY_TO(var, src, size) {                              \
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
extern unsigned int d3fl_Count;             /* 0x633744 */
extern D3FLCACHE *d3fl_Cache;               /* 0x63374c */
extern unsigned int d3fl_DetailScale;       /* 0x633748, power of two from the detail level (0 when off) */
extern unsigned char d3fl_DetailShift;      /* 0x633750, 6 - log2(detail scale): 0x40 >> it gives d3fl_DetailScale */
extern unsigned char d3fl_DetailSizeShift;  /* 0x633751, detail level >> 7 (at most 2): d3fl_DetailScale << it is the detail size */
extern char d3fl_Mips;                      /* 0x63373c */
extern char d3fl_MapShift;                  /* 0x63373d, texture shift copied into D3FLMAP.shift (d3fl_Init argument) */
extern unsigned long d3fl_W64[64 * 64];     /* 0xfbcd40, blend weights for 64 pixels per tile */
extern unsigned long d3fl_W32[32 * 32];     /* 0xfbbd40 */
extern unsigned long d3fl_W16[16 * 16];     /* 0xfbb900 */
extern unsigned long d3fl_ColorTab[3][256]; /* 0xfbac60, 8-bit channel -> 16-bit pixel bits */
extern float d3fl_Jitter[];                 /* 0xfc0d60 */
extern float d3fl_UV[2][8];                 /* 0xfbb8a0 */
extern D3FLBLEND d3fl_Blend;                /* 0x5fb970 */
extern unsigned int d3fl_TileN;             /* 0xfbb870, pixels per tile in the texture being drawn */
extern int d3fl_DstPitch;                   /* 0xfbb874, bytes per texture row */
extern int d3fl_SrcPitch;                   /* 0xfbbd10, source bytes per row step */
extern int d3fl_SrcStep;                    /* 0xfc0d40, source bytes per pixel step */
extern int d3fl_WPitch;                     /* 0xfc2560, weight bytes per row step */
void _m_empty(void);                        /* MMX emms intrinsic (Processor Pack) */
#pragma intrinsic(_m_empty)

void d3fl_Invalidate(D3FLMAP *map);         /* 0x440140 */
struct TSTEXTURE; struct TSTEXTURE *tex_CreateHi(char *name, int size, D3TEXFLAGS flags, unsigned char f0, unsigned char f6a, char mips); /* 0x4613b0, ts_texture.c (TSFLAGS is D3TEXFLAGS) */

/* d3_fl.c functions used before their definition */
void d3fl_BlendC(unsigned short *dst, unsigned long *w, unsigned char **src, unsigned int n);       /* 0x43eab0 */
void d3fl_BlendMMX(unsigned short *dst, unsigned long *w, unsigned char **src, unsigned int n);     /* 0x43ed70 */
void d3fl_MakeJitter(float *out);                                                       /* 0x43f300 */
void d3fl_Flush(void);                                                                  /* 0x43f990 */
void d3fl_PrepareWater(D3FLMAP *map);                                                    /* 0x440360 */

/* ---- 0x441f50..0x4439e0 (worker map) ---- */
extern char d3fl_EmptyName;                 /* 0x631090, the shared empty string (ch.h's ch_EmptyName, "") */
extern const char d3fl_Suffix[3][11];             /* 0x5ebaa8, "", "_high_1", "_high_2": source texture mip levels */
char *tex_FindFile(char *name);                                                         /* 0x45f9f0, ts_texture.c */
void d3fl_ReloadTextures(D3FLMAP *map);                                                 /* 0x442280 */
extern D3VECTOR4 d3fl_Origin;               /* 0xfbbd30, map point (0, 0, 0) in world space */
extern D3VECTOR4 d3fl_AxisX;                /* 0xfc0d50, world step per map x */
extern D3VECTOR4 d3fl_AxisY;                /* 0xfbbd00, world step per height unit */
extern D3VECTOR4 d3fl_AxisZ;                /* 0xfbb8e0, world step per map y */
extern D3BOXVERT d3fl_Box[8];               /* 0x6334b8, result of d3fl_TileBox */
typedef struct D3FLDESC {           /* input of d3fl_LoadMap; null pointers / size <= 0 mean load from files */
    char name[0x40];                /* height map "boden/<name>.bmp", water map "boden/<name>_w.bmp" */
    unsigned char *height;          /* 0x40 */
    unsigned char *water;           /* 0x44 */
    char texname[0x40];             /* 0x48, tile type map "boden/<texname>.bmp" */
    unsigned char *types;           /* 0x88 */
    void *lightofs;                 /* 0x8c */
    float scale;                    /* 0x90, world units per map point */
    float hscale;                   /* 0x94, world units per height unit */
    int nregions;                   /* 0x98 */
    int size;                       /* 0x9c, side of the in-memory maps */
    D3FLREGION *regions;            /* 0xa0 */
    char typenames[8][0x40];        /* 0xa4, floor textures */
} D3FLDESC;
/* Inline-asm copy with size, source and destination through temps */
#define D3FL_COPY(dst, src, size) {                                 \
    int _cn = (size); void *_cs = (src); void *_cd = (dst);         \
    __asm mov edi, _cd                                              \
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
extern struct VFSDIR *vfs_Root;                      /* 0x6333fc, vfs.c */
D3FLMAP *d3fl_LoadMap(D3FLDESC *desc);                                                  /* 0x442390 */
struct TSTEXTURE *tex_GetFrame(int index, unsigned char frame);                         /* 0x460b00, ts_texture.c (d3_fl.c reads it through tex_GetFrameF) */
#ifndef DATA_VIEWS_H
extern unsigned int d3_AnimDiv[];           /* 0x5ebc60, ts_texture.c */
#endif
extern D3VECTOR4 d3fl_LightCol;             /* 0xfbb860, color per light-map unit */
extern D3VECTOR4 d3fl_Ambient;              /* 0xfbbd20 */
extern D3VECTOR4 d3fl_SunCol;               /* 0xfbb880, added where the light map has bit 7 */
void d3_ClipMesh(D3CLIP mask, D3PVERT *v, int nverts, D3PTRI *t, int ntris);          /* 0x439a60, d3_engine.c (d3_engine.h's types: the d3_fl callers pass their D3FLPOINT / D3FLPOLY views) */

/* ---- 0x4439e0..0x446990 (worker d3fl3) ---- */
int map_PickScreen(D3FLMAP *map, int x, int y, float r, D3VECTOR *out);                 /* 0x446750 */
int map_Region(D3FLMAP *map, int x, int y);                                             /* 0x446910 */
void * dd_LockSurfaceFlags(LPDIRECTDRAWSURFACE4 surf, DWORD flags);                                     /* 0x41de30, dd_vesa.c */
void d3d_PrecacheTexture(LPDIRECT3DTEXTURE2 t);                                                      /* 0x4360e0, d3_d3d.c */
void d3d_BuildMipmaps(LPDIRECTDRAWSURFACE4 tex, DDSURFACEDESC2 *ddsd);                                           /* 0x4371d0, d3_d3d.c */
struct TSTEXTURE; struct TSTEXTURE *tex_Prepare(struct TSTEXTURE *tex);                                                     /* 0x4614c0, ts_texture.c */

/* ---- 0x4439e0 (worker d3fl4) ---- */
typedef struct D3FLSPLIT {          /* 12 bytes, D3FLTILE.split: a vertex added on a tile edge next to a finer tile */
    D3FLPOINT *p;
    int x, y;                       /* map position */
} D3FLSPLIT;
typedef struct D3FLTRIUV {          /* the texture coordinates of one triangle (D3FLPOLY.uv, D3FLTILE.bp) */
    float uv[6];
} D3FLTRIUV;
extern D3VECTOR d3_AmbientColor;               /* 0x5fcc78, ambient light color */
extern D3VECTOR d3_Unk633b1c;               /* 0x633b1c, point light color */
void d3fl_Draw(D3FLMAP *map, char water);                                               /* 0x4439e0 */


/* ---- 0x446990..0x446e10 (was unk_446990.h) ---- */
extern D3VECTOR4 d3fl_Rays[5];                      /* 0xfc2960, view ray directions (x, y, z, w) */
void d3fl_FreeTiles(D3FLMAP *map);                  /* 0x441db0 */
void d3fl_Inflate(D3FLMAP *map);                    /* 0x441ea0 */
void d3fl_MakeLight(D3FLMAP *map);                  /* 0x441860 */

#endif
