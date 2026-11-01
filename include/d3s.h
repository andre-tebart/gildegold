/* Types of d3s.c (0x459d80..0x45ce60): the object shadow TU ("d3s:" tags, "Schatten" texture). Names are ours. */
#ifndef D3S_H
#define D3S_H

typedef struct D3SPANPIX {          /* 0x1c bytes per pixel of the rasterizer's span buffer (first dword = value) */
    int v;
    char pad04[0x18];
} D3SPANPIX;

typedef struct D3SCENT {            /* 0x14 bytes, one cached shadow texture (d3s_Cache); D3SSLOT.cache */
    D3TEXHANDLE *tex;               /* 0x00 */
    void **owner;                   /* 0x04, the slot that uses it (its tex pointer is cleared when taken over) */
    unsigned int time;              /* 0x08, frame of last use */
    unsigned int size;              /* 0x0c, 0 = free */
    char flag;                      /* 0x10 */
    char pad11[3];
} D3SCENT;

typedef struct D3SSLOT {            /* 0x80 bytes, 4 per material at +0x754: the shadow of one light */
    D3TEXHANDLE *tex;               /* 0x00 */
    D3OBJECT *light;                /* 0x04 */
    D3SCENT *cache;                 /* 0x08 */
    unsigned char *map;             /* 0x0c, height map data */
    int size;                       /* 0x10 */
    unsigned int maxh;              /* 0x14 */
    D3VECTOR dir;                   /* 0x18, light direction the shadow was made for */
    int pad24;
    D3VECTOR cell;                  /* 0x28 */
    int pad34;
    D3VECTOR origin;                /* 0x38 */
    int pad44;
    float x0, x1, z0, z1;           /* 0x48, shadow rectangle */
    float y;                        /* 0x58, ground height */
    int ix0, ix1, iz0, iz1;         /* 0x5c, in map cells */
    int bz0, bx0, bz1, bx1;         /* 0x6c, in 8x8 blocks; bz0 -1 = none */
    unsigned char simple;           /* 0x7c */
    char pad7d[0x3];
} D3SSLOT;

#define D3S_SLOTS(mat) ((D3SSLOT *)((char *)(mat) + 0x754))

typedef struct D3SVERT {            /* 0x50 bytes, like D3BOXVERT */
    float x, y, z;                  /* 0x00 */
    float w;                        /* 0x0c */
    float tu, tv;                   /* 0x10, position in the shadow texture (texels) */
    char pad18[0x28];
    unsigned long c0;               /* 0x40 */
    unsigned long c1;               /* 0x44 */
    float *pos;                     /* 0x48 */
    char pad4c[0x4];
} D3SVERT;

typedef struct D3SPOLY {            /* 0x28 bytes */
    D3SVERT *v[3];                  /* 0x00 */
    int pad0c;
    float *uv;                      /* 0x10 */
    D3TEXHANDLE *tex;               /* 0x14 */
    float z;                        /* 0x18 */
    char pad1c[0xa];
    unsigned char flags;            /* 0x26, 2 = steep, 4 = both sides */
    char pad27;
} D3SPOLY;

typedef struct D3SBUF {             /* the shadow mesh being built (at material + 0xf4) */
    D3SVERT *verts;                 /* 0x00 (0xf4) */
    D3SPOLY *polys;                 /* 0x04 (0xf8) */
    int nverts;                     /* 0x08 (0xfc) */
    int npolys;                     /* 0x0c (0x100) */
} D3SBUF;

typedef struct D3STBLOCK {          /* 0x64 bytes, 8x8 of them in a terrain (d3_sm.h: D3SMBLOCK, which lacks lod) */
    unsigned char lo, hi;           /* height range */
    unsigned char lod;              /* 0x02, sample step of the block (1, 2, 4, 8); 0 = not drawn */
    char pad03[0x61];
} D3STBLOCK;

typedef struct D3STERR {            /* the parts of a terrain (d3_sm.h: D3TERRAIN, which lacks bs / mip / lod) used by
                                       the shadow mesh builder */
    unsigned int size;              /* 0x00 */
    unsigned int bs;                /* 0x04, block size in cells */
    char pad08[8];
    unsigned char *heights;         /* 0x10 */
    char pad14[0x10];
    unsigned char *mip[8];          /* 0x24, heights at lod 1, 2, 4, 8 live at mip[lod >> 1] */
    char pad44[0x4c];
    D3VECTOR4 origin;               /* 0x90 */
    D3VECTOR4 axisx;                /* 0xa0 */
    D3VECTOR4 axisz;                /* 0xb0 */
    D3VECTOR4 axisy;                /* 0xc0 */
    char pad0d0[0x6c];
    D3STBLOCK blocks[8][8];         /* 0x13c */
} D3STERR;

typedef struct D3SMESH {            /* the parts of a mesh (d3_engine.h: D3MESH) the shadow code reads through
                                       D3OBJECT::material */
    char pad000[0x954];
    float groundOffset;             /* 0x954, offset added to the mesh's ground height (D3MESH::groundOffsetRaw) */
} D3SMESH;
#define D3S_MESH(m) ((D3SMESH *)(m))

/* declared in other units' headers before (moved to the owner) */
void d3_ExitShadows(void);  /* 0x45aee0 */
void d3_InitShadows(unsigned int size, unsigned int maxsize, int ncache, int age, char mips, int polys, int mapsize);  /* 0x45ad80 */
void d3s_CollectLights(void);  /* 0x45c9f0 */
extern int d3s_Count;  /* 0x633dc0 */
char d3s_FreeLightCB(D3OBJECT *obj, D3OBJECT *light);  /* 0x45cd60 */
void d3s_FreeShadows(D3OBJECT *obj);  /* 0x45ccd0 */
extern unsigned int d3s_MaxPolys;  /* 0x633dc4 */
extern int d3s_NumUVs;  /* 0xfb925c */
extern int d3s_NumVerts;  /* 0xfb9220 */
extern D3OBJECT * d3s_Object;  /* 0x633448 */
void d3s_ResetShadows(D3OBJECT *obj);  /* 0x45ce00 */
void d3s_UpdateObject(D3OBJECT *obj);  /* 0x45ca20 */
void d3s_UpdateObjectAt(D3OBJECT *obj);  /* 0x45ca60 */

#endif
