/* d3_sm.c -- terrain height/shadow map ("d3_sm:", "d3sm_"), original file name unknown.
   Names are ours. */
#ifndef UNK_45DB40_H
#define UNK_45DB40_H
#include "d3.h"                       /* the D3 records and typedefs this header names */

typedef struct D3SMENTRY {          /* 0x1c bytes, one per height map cell */
    int type;                       /* ground type, index into d3sm_GroundNames; 0xd = border */
    char pad04[0x18];
} D3SMENTRY;

typedef struct D3SM {               /* 0x34 bytes; d3_tools.c sees it as D3HMAP */
    D3VECTOR4 origin;               /* 0x00 */
    D3VECTOR4 cell;                 /* 0x10, cell size; cell.y = height per unit */
    int size;                       /* 0x20, cells per side */
    D3SMENTRY *entries;             /* 0x24 */
    unsigned char *data;            /* 0x28, heights */
    int pad2c;
    int mask;                       /* 0x30, object shadow mask (D3OBJECT.stateFlags >> 2 & 3), tested as a byte */

} D3SM;

typedef struct D3SMBLOCK {          /* 0x64 bytes, 8x8 of them in a terrain */
    unsigned char lo, hi;           /* height range */
    unsigned char lod;              /* 0x02, sample step of the block (1, 2, 4, 8); 0 = not drawn */
    char pad03[0x61];
} D3SMBLOCK;

typedef struct D3TERRAIN {          /* the terrain a height map is sampled from */
    unsigned int size;              /* 0x000 */
    char pad004[0xc];
    unsigned char *heights;         /* 0x010 */
    unsigned char *types;           /* 0x014, index into the names, bit 7 = none */
    char pad018[0x78];
    D3VECTOR4 origin;               /* 0x090 */
    D3VECTOR4 axisx;                /* 0x0a0 */
    D3VECTOR4 axisz;                /* 0x0b0 */
    D3VECTOR4 axisy;                /* 0x0c0, axisy.y = height per unit */
    char pad0d0[0x6c];
    D3SMBLOCK blocks[8][8];         /* 0x13c */
} D3TERRAIN;

#define D3TERRAIN_NAMES(t) ((char (*)[0x40])((char *)(t) + 0x1a64))    /* 8 material names, overlaps blocks */

typedef struct D3SMVERT {           /* 0x50 bytes, D3OBJINFO.keys */
    float x, y, z;
    float pad0c;
    float gx, gz;                   /* 0x10, position in height map cells */
    char pad18[0x29];
    unsigned char flags;            /* 0x41 */
    unsigned char h;                /* 0x42, height in map units */
    char pad43[0xd];
} D3SMVERT;

typedef struct D3SMTRI {            /* 0x28 bytes */
    D3SMVERT *v[3];
    char pad0c[0x18];
    unsigned char f0 : 6;           /* 0x24 */
    unsigned char solid : 1;        /* no shadow test (steep or flagged) */
    unsigned char cast : 1;         /* casts a shadow */
    unsigned char pad25;
    unsigned char f26;              /* 0x26, 2: skip, 4: always */
    char pad27;
} D3SMTRI;

typedef struct D3SMMESH {           /* D3OBJECT.info */
    D3SMVERT *keys;
    D3SMTRI *tris;                  /* 0x04 */
    int nkeys;                      /* 0x08 */
    int ntris;                      /* 0x0c */
} D3SMMESH;

char d3sm_FindGround(D3SM *h, int x, int z, int *ox, int *oz);           /* 0x45efc0, the cell nearest to
                                                                            (x, z) with a real ground type */
char d3_GetHeightAt(D3SM *h, D3VECTOR *pos, int *px, int *pz, float *py);        /* 0x45f170, the cell of pos and
                                                                            (h != 0) the interpolated height there */

/* declared in other units' headers before (moved to the owner) */
void d3sm_Destroy(D3SM *h);  /* 0x45ef60 */
extern D3SM *d3sm_Shadow;  /* 0x633df8 */

#endif
