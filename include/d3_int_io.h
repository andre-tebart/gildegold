struct TSTEXTURE;
/* d3_int_io.c -- scene file reader of the d3 interface ("d3_int_io:" allocation tag, "rd_object()" in an error
   message): objects (0x44abb0) and the floor (0x44b920). Its own TU (see d3_obj.c). Names are ours
   except rd_object. */
#ifndef UNK_44ABB0_H
#define UNK_44ABB0_H
#include "d3_engine.h"                /* D3CLIP / D3SRCPOLY / D3PTRI */
#include "d3.h"                       /* the D3 records and typedefs this header names */
struct VFSFILE;

typedef union D3IOFLAGS {           /* tex_Load flags as the floor reader builds them */
    unsigned int all;
    D3TEXFLAGS f;
    struct {
        unsigned char b0;
        unsigned char b1;
        unsigned char c0 : 1;
        unsigned char c1 : 1;
        unsigned char c2 : 6;
        unsigned char b3;
    } s;
} D3IOFLAGS;

typedef struct D3IOOBJFLAGS {      /* D3OBJECT::state..stateFlags as the reader writes them */
    unsigned char a0 : 1;           /* 0x210 */
    unsigned char a1 : 1;
    unsigned char a2 : 1;
    unsigned char a3 : 1;
    unsigned char a4 : 1;
    unsigned char a5 : 1;
    unsigned char a6 : 1;
    unsigned char a7 : 1;
    unsigned char b0 : 1;           /* 0x211 */
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
    unsigned char c0 : 1;           /* 0x212 */
    unsigned char c1 : 1;
    unsigned char c23 : 2;
    unsigned char c4 : 1;
    unsigned char c5 : 1;
    unsigned char c6 : 1;
    unsigned char c7 : 1;
} D3IOOBJFLAGS;
#define D3IO_FL(o) ((D3IOOBJFLAGS *)&(o)->state)
#define D3IO_200(o) (*(int *)((char *)(o) + 0x200))

typedef struct D3IOLIGHTKEY {       /* D3LIGHTENTRY (0x38 bytes) as the reader fills it (d3_io_t.h's D3IOKEY is the animation key) */
    D3VECTOR v00;
    D3VECTOR v0c;
    D3VECTOR v18;
    float f24;
    float f28;
    float f2c;
    float f30;
    int f34;
} D3IOLIGHTKEY;
#define D3IO_KEYS(o) ((D3IOLIGHTKEY *)D3_X(o)->light)

/* tex_Load with its real argument types (ts_texture.c); d3.h declares the last two as int */
#define D3IO_TEXLOAD ((D3TEXHANDLE *(*)(char *, D3IOFLAGS, unsigned char, unsigned char))tex_Load)

#ifndef UNK_401000_H                /* bio.h clashes with d3_fl.h (vfs declarations) */
void bio_rd_c(struct VFSFILE *f, char *c);                                                        /* 0x401180 */
void bio_rd_v3(struct VFSFILE *f, int *v);                                                        /* 0x401260 */
struct VFSFILE; void bio_rd_v4(struct VFSFILE *f, int *v);                                                        /* 0x401290 */
int bio_rd_array_quick(struct VFSFILE *f, int size, void **out);                                  /* 0x401380 */
#endif

extern char d3_LodMissing;                  /* 0x6339e4, set when an object LOD is missing */

void d3e_LoadEvents(void *f, D3OBJECT *obj);                                            /* 0x43e970, d3_engine.c */
D3OBJECT *rd_object(void *f, D3OBJECT *parent, unsigned int version, char reload);  /* 0x44abb0 */
struct D3FLMAP *d3io_ReadFloor(void *f, unsigned int version);                           /* 0x44b920 */

/* --- 0x44be60..0x44cd40 (worker d3io): scene / group loading, stock objects --- */
typedef struct D3IOSTOCK D3IOSTOCK;
struct D3IOSTOCK {                  /* a loaded stock object (mesh driver, D3OPS / D3MESHSRC view) */
    char name[0x40];                /* 0x000 */
    char *points;                   /* 0x040, 0x18 bytes each */
    int npoints;                    /* 0x044 */
    D3SRCPOLY *polys;               /* 0x048 */
    int npolys;                     /* 0x04c */
    char pad050[0x18c];
    int refs;                       /* 0x1dc */
    int ntex;                       /* 0x1e0 */
    char pad1e4[0x18];
    D3IOSTOCK *next;                /* 0x1fc */
    int unk200;
    char (*texnames)[0x40];         /* 0x204 */
};

extern void *d3io_StockDir;                 /* 0xfba940, the stock object directory */
extern char d3io_StockPath[];               /* 0x633a00 */
struct VFSDIR * vfs_FindDir(char *path, struct VFSDIR *d);                                                 /* 0x42f7e0, vfs.c */
char * vfs_GetPath(char *name, struct VFSDIR *d, struct VFSDIR *stop, char *out);                          /* 0x42fca0, vfs.c */

void d3io_SetStockDir(char *path);                                                      /* 0x44c510 */
char *d3io_StockFile(char *name, char *ext);                                            /* 0x44c530 */

typedef union D3IOLOD {             /* 0x5fbf78 (initialized to 2): which stock object LODs are loaded */
    unsigned int all;               /* the whole dword: main_OpenScreen stores 2 (0x531fcb mov dword [d3io_Lod], 2) */
    struct {
        unsigned int mode : 7;      /* 1: all three, 2: the best one found */
        unsigned int shadow : 1;    /* load the "_s" variant for lod < 0 */
    };
} D3IOLOD;
extern D3IOLOD d3io_Lod;                    /* 0x5fbf78, which stock object LODs are loaded (d3_io_t.c and main.c include this header) */
char d3io_LodName(char *name, char *name2, int lod, char *out, char *out2);            /* 0x44c9d0 */
typedef union D3IOCOL {             /* a vertex colour: the b, g, r, a bytes, or the whole dword (-1 = unset) */
    int all;
    struct {
        unsigned char b, g, r, a;
    };
} D3IOCOL;

typedef struct D3IOVERT {           /* 0x50 bytes, D3MESHKEY / D3VERTEX as the stock attach fills it */
    char pad00[0x40];
    D3IOCOL col40;                  /* 0x40 */
    D3IOCOL col44;                  /* 0x44 */
    char *src;                      /* 0x48, the stock object's point (0x18 bytes) */
    unsigned char f4c;              /* 0x4c, 0x80: used by a triangle */
    unsigned char f4d;              /* 0x4d */
    char pad4e[0x2];
} D3IOVERT;

typedef struct D3IOPOLY {           /* 0x38 bytes, D3SRCPOLY */
    char pad00[0x18];
    int idx[3];                     /* 0x18 */
    int tex;                        /* 0x24, index into d3_Textures, < 0: none */
    int slot;                       /* 0x28, index into the stock object's texture set */
    char pad2c[0xc];
} D3IOPOLY;

typedef struct D3IOTRIFL {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 4;
} D3IOTRIFL;

typedef struct D3IOTEX {            /* D3TEXTURE (0x80 bytes) as the stock attach reads it */
    char pad00[0x68];
    unsigned char f68;              /* 0x68 */
    char pad69[0x3];
    unsigned char colorkey;         /* 0x6c, 0xff = none */
    char pad6d;
    unsigned char flags;            /* 0x6e */
    char pad6f[0x11];
} D3IOTEX;

typedef struct D3IOTRI {            /* 0x28 bytes, D3TRI */
    D3IOVERT *v[3];
    int pad0c;
    D3IOPOLY *src;                  /* 0x10 */
    D3IOTEX *tex;                   /* 0x14 */
    int f18, f1c, f20;              /* 0x18 */
    unsigned char f24;              /* 0x24 */
    char pad25;
    D3IOTRIFL fl;                   /* 0x26 */
    char pad27;
} D3IOTRI;

typedef struct D3IOSURF {           /* 0x198 bytes, D3SURF as the stock attach fills it */
    D3IOVERT *keys;
    D3IOTRI *tris;                  /* 0x04 */
    int nkeys;                      /* 0x08 */
    int ntris;                      /* 0x0c */
    D3IOSTOCK *stock;               /* 0x10 */
    D3IOTEX **texset;               /* 0x14 */
    char pad18[0x17c];
    unsigned char f194;             /* 0x194 */
    unsigned char texSet;           /* 0x195, current texture set (D3SURF.texSet, d3_engine.h) */
    char pad196[0x2];
} D3IOSURF;

#define D3IO_214(o) (D3_X(o)->unk214)
void d3io_AttachStock(D3OBJECT *obj, D3SURF *surf, char *name);                        /* 0x44c610 */
typedef struct D3IOFOG {            /* D3IFTRIPLE as the scene reader writes it */
    int col;
    float start;
    float end;
} D3IOFOG;
typedef struct D3IOSET {            /* D3IFLEVEL (0x60 bytes) */
    D3VECTOR amb;
    D3VECTOR dir;
    D3IOFOG fog[6];
} D3IOSET;
#define D3IO_SETS ((D3IOSET *)d3if_Levels)
#define D3IO_SCENEB4(s) (*(int *)((char *)(s) + 0xb4))
#define D3IO_SCENEAC(s) (*(void **)((char *)(s) + 0xac))
#define D3IO_SCENE3D8(s) (*(int *)((char *)(s) + 0x3d8))
typedef void (*D3IOPROGRESS)(int a, int i, int n);

D3OBJECT *d3_CloneObject(D3OBJECT *src);                                                /* 0x43d180, d3_engine.c */
char d3_LoadScene(char *name, void *f, D3IOPROGRESS progress);                    /* 0x44be60 */
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                                    /* 0x430470, vfs.c */
int vfs_fclose(struct VFSFILE *f);                                                                /* 0x430aa0, vfs.c */
D3OBJECT *d3_LoadGroup(char *name, void *f, D3OBJECT *parent);                    /* 0x44c3f0 */

/* declared in other units' headers before (moved to the owner) */
void d3_AttachMesh(D3OBJECT *obj, int unk, char *name);  /* 0x44cbb0 */
int d3io_FindStock(char *name);  /* 0x44c5d0 */

#endif
