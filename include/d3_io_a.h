/* d3_io_a.h -- AGI / AGF text model reader of the d3 engine ("d3_io_a:AGIMaterialBuffer", "d3_io_a:AGIPointBuffer",
   "d3_io_a:AGIPolyBuffer" allocation tags): the line handlers that fill a reader context. The TU starts at
   0x44e570 and continues past 0x44ee50 (worker scopes end there). Names are ours. */
#ifndef UNK_44E570_H
#define UNK_44E570_H
struct VFSFILE;

typedef struct AGIMAT {             /* 0xe0 bytes */
    char name[3][0x40];             /* 0x00, 0x40, 0x80: bitmap names (extension stripped) */
    unsigned char mode;             /* 0xc0, 0 = none, 1 = "MIX", 2 = "ADD" */
    unsigned char b1;               /* 0xc1 */
    unsigned char b2;               /* 0xc2 */
    unsigned char pad3;
    unsigned char b4;               /* 0xc4 */
    unsigned char pad5[3];
    unsigned char b8;               /* 0xc8 */
    unsigned char pad9[3];
    float f[5];                     /* 0xcc..0xdc */
} AGIMAT;

typedef struct AGIPOLY {            /* 0x38 bytes */
    float uv[3][2];                 /* 0x00 */
    int v[3];                       /* 0x18, point indices (plus the base of the current object) */
    int tex;                        /* 0x24, index into d3_Textures or -1 (as D3BGFPOLY.tex) */
    int mat;                        /* 0x28, material number */
    char pad2c[0xc];
} AGIPOLY;

typedef struct AGIPOINT {           /* 0x18 bytes */
    float x, y, z;
    float pad0c[3];
} AGIPOINT;

typedef struct AGICTX {             /* reader context */
    int unk00;
    int geomInt;                    /* 0x04, the object geometry int (chunk 0x18 of the binary format) */
    int cur;                        /* 0x08, current material */
    int nmats;                      /* 0x0c */
    AGIMAT *mats;                   /* 0x10 */
    int npoly;                      /* 0x14, polygons of the current object */
    int polyidx;                    /* 0x18, next polygon */
    int polybase;                   /* 0x1c, polygons of the earlier objects */
    AGIPOLY *polys;                 /* 0x20 */
    int npts;                       /* 0x24 */
    int ptidx;                      /* 0x28 */
    int ptbase;                     /* 0x2c */
    AGIPOINT *pts;                  /* 0x30 */
} AGICTX;

char *tex_FindFile(char *name);                                                 /* 0x45f9f0 */

char agi_IsPow2(int n);                                                         /* 0x44e570 */
int agi_CheckBitmap(char *name, char report);                                   /* 0x44e5a0 */
void agi_Bitmap0(void *unused, AGICTX *c);                                      /* 0x44e6f0 */
void agi_Bitmap1(void *unused, AGICTX *c);                                      /* 0x44e760 */
void agi_Bitmap2(void *unused, AGICTX *c);                                      /* 0x44e7d0 */
void agi_Blend(void *unused, AGICTX *c);                                        /* 0x44e840 */
void agi_MatB1(void *unused, AGICTX *c);                                        /* 0x44e8d0 */
void agi_MatB2(void *unused, AGICTX *c);                                        /* 0x44e910 */
void agi_MatB4(void *unused, AGICTX *c);                                        /* 0x44e950 */
void agi_MatB8(void *unused, AGICTX *c);                                        /* 0x44e990 */
void agi_MatF0(void *unused, AGICTX *c);                                        /* 0x44e9d0 */
void agi_MatF1(void *unused, AGICTX *c);                                        /* 0x44ea10 */
void agi_MatF2(void *unused, AGICTX *c);                                        /* 0x44ea50 */
void agi_MatF3(void *unused, AGICTX *c);                                        /* 0x44ea90 */
void agi_MatF4(void *unused, AGICTX *c);                                        /* 0x44ead0 */
void agi_Materials(void *unused, AGICTX *c);                                    /* 0x44eb10 */
void agi_PolyVerts(void *unused, AGICTX *c);                                    /* 0x44eb60 */
void agi_PolyUV(void *unused, AGICTX *c);                                       /* 0x44ebf0 */
void agi_PolyMat(void *unused, AGICTX *c);                                      /* 0x44ec60 */
void agi_EndObject(void *unused, AGICTX *c);                                    /* 0x44ecb0 */
void agi_GeomInt(void *unused, AGICTX *c);                                    /* 0x44ece0 */
void agi_NumPoints(void *unused, AGICTX *c);                                    /* 0x44ed00 */
void agi_NumPolys(void *unused, AGICTX *c);                                     /* 0x44ed50 */
void agi_Points(void *f, AGICTX *c);                                            /* 0x44eda0 */
void d3io_NoopCallback(void *t);                                                        /* 0x44ee40, empty */

#endif
