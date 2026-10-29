/* d3_io_t.c -- the "d3_io_t:" / "d3_io:" part of the d3 scene I/O: the BGF mesh loader and its helpers
   (0x44cd40..0x44e570; 0x44df40.. from worker d3io3). Names are ours. */
#ifndef UNK_44CD40_H
#define UNK_44CD40_H

typedef union D3BGFFLAGS {          /* tex_Load flags as the loader builds them */
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
} D3BGFFLAGS;
/* tex_Load with its real argument types (ts_texture.c); d3.h declares the last two as int */
#define D3BGF_TEXLOAD ((D3TEXHANDLE *(*)(char *, D3BGFFLAGS, unsigned char, unsigned char))tex_Load)

typedef struct D3BGFPOLY {          /* 0x38 bytes, D3BGF::polys */
    float uv[3][2];                 /* 0x00 */
    int v[3];                       /* 0x18, vertex indices */
    int tex;                        /* 0x24, index into d3_Textures or -1 */
    int mat;                        /* 0x28, material index while loading */
    char pad2c[0xc];
} D3BGFPOLY;

typedef struct D3BGFMAT {           /* 0xe0 bytes, a material as stored in the file */
    char name[3][0x40];             /* 0x00, 0x40, 0x80: texture names */
    unsigned char kind;             /* 0xc0 */
    unsigned char c1;               /* 0xc1 */
    char c2;                        /* 0xc2 */
    char c3;                        /* 0xc3 */
    char pad4;
    char c5;
    char c6;
    char c7;
    char c8;
    char c9;
    char padca[2];
    float su, sv;                   /* 0xcc: scale */
    float ou, ov;                   /* 0xd4: offset */
    float rot;                      /* 0xdc */
} D3BGFMAT;

typedef struct D3BGFATTACH {        /* 0x58 bytes */
    char name[0x40];
    float f[6];
} D3BGFATTACH;

typedef struct D3BGFDESC {          /* 0x198 bytes, what the file reader (0x44f870) fills in */
    char pad00;
    char flag01;                    /* 0x01 */
    char pad02[0xa];
    int nmat;                       /* 0x0c */
    D3BGFMAT *mats;                 /* 0x10 */
    char pad14[0x8];
    int npoly;                      /* 0x1c */
    D3BGFPOLY *polys;               /* 0x20 */
    char pad24[0x8];
    int npts;                       /* 0x2c */
    float *pts;                     /* 0x30, 6 floats each */
    int nattach;                    /* 0x34 */
    D3BGFATTACH attach[4];          /* 0x38 */
} D3BGFDESC;

typedef struct D3BGF D3BGF;
struct D3BGF {                      /* 0x20c bytes, a loaded mesh ("d3_io:LoadAGI_obj") */
    char name[0x40];
    float *verts;                   /* 0x40, 6 floats each, followed by the 8 corners of the bounding box */
    int nverts;                     /* 0x44 */
    D3BGFPOLY *polys;               /* 0x48 */
    int npolys;                     /* 0x4c */
    char pad50[0x18];
    float cx, cy, cz;               /* 0x68, centre of the box */
    D3BGFATTACH attach[4];          /* 0x74 */
    float radius;                   /* 0x1d4 */
    float diagonal;                 /* 0x1d8 */
    int refs;                       /* 0x1dc */
    int ntex;                       /* 0x1e0 */
    int nsets;                      /* 0x1e4, texture sets of the name table (D3OPS.nsets, d3_engine.h) */
    char pad1e8[0x14];
    D3BGF *prev;                    /* 0x1fc */
    D3BGF *next;                    /* 0x200 */
    char *texnames;                 /* 0x204, ntex names of 0x40 bytes */
    char pad208[4];
};

/* The eight bounding-box corners are appended to the vertex array as D3BGFVERT records (d3_io_fl.h):
   D3BGF_CORNER(m, k) is corner k (x, y, z; its normals stay unset) */
#define D3BGF_CORNER(m, k) (((D3BGFVERT *)((char *)(m)->verts + (m)->nverts * 0x18)) + (k))

extern struct VFSDIR *vfs_Root;                                                                  /* 0x6333fc */
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                                    /* 0x430470 */
int vfs_fclose(struct VFSFILE *f);                                                                /* 0x430aa0 */
void bio_rd_i(struct VFSFILE *f, int *i);                                                         /* 0x4011d0 */
void bio_rd_s(struct VFSFILE *f, char *s);                                                        /* 0x4011a0 */
char *d3io_StockFile(char *name, char *ext);                                             /* 0x44c530, d3_int_io.c */
void d3_CalcNormals(D3BGF *m);                                                           /* 0x450dc0 */
D3BGF *d3_FindBGF(char *path, char *name, char *texnames, int unk1e4, int ntex);       /* 0x450860 */
char d3_Unk4506f0(char *path);                                                          /* 0x4506f0, d3_io_b.c */
void d3_AddFLChunk(char *path, char weld);                                               /* 0x451250, d3_io_fl.c */
void d3_ReadBGF(void *f, D3BGFDESC *desc);                                              /* 0x44f870 */

void d3iot_DeleteStockObject(D3BGF *m);                                                 /* 0x44cd40 */
void d3iot_CalcBounds(D3BGF *m);                                                        /* 0x44ce40 */
void d3iot_Finish(D3BGF *m, char bounds);                                               /* 0x44d1c0 */
void d3iot_LoadTexSet(char *name, char **out, int *w, int *h);                          /* 0x44d1f0 */
D3BGF *d3iot_LoadBGF(char *path, char *name, char *texnames, int unk1e4, int ntex);     /* 0x44d330 */
float d3iot_GetRadius(int unused, D3OBJINFO *info);                                     /* 0x44df20 */

/* 0x44df40..0x44e570 (worker d3io3, joined at the round-10 close) */

typedef struct D3IOANIMDATA D3IOANIMDATA;
struct D3IOANIMDATA {               /* a loaded animation stream: D3BASTREAM (d3_io_b.h) as the list code sees it */
    char pad000[0x160];
    D3IOANIMDATA *prev;             /* 0x160, list of the loaded animations */
    D3IOANIMDATA *next;             /* 0x164 */
};

typedef struct D3IOOBJ D3IOOBJ;
struct D3IOOBJ {                    /* a D3OBJECT as the loaders see it (0x220+ bytes) */
    char pad000[0x40];
    int unk40;                      /* 0x40, set to d3_Unk6333cc by d3_LoadObjectAnim */
    char pad044[0x8];
    float pos[3];                   /* 0x4c */
    char pad058[0x2c];
    float rot[3];                   /* 0x84 */
    char pad090[0x140];
    struct D3IOPATH *path;          /* 0x1d0 */
    char pad1d4[0x8];
    int refs;                       /* 0x1dc, reference count of a stock object */
    char pad1e0[0x8];
    void *fn1e8;                    /* 0x1e8..0x1f8: the stock object's callbacks */
    void *fn1ec;
    void *fn1f0;
    void *fn1f4;
    void *fn1f8;
    D3IOOBJ *prev;                  /* 0x1fc, list of stock objects */
    D3IOOBJ *next;                  /* 0x200 */
    void *data;                     /* 0x204, buffer returned by d3_Unk44d1f0 */
    char pad208[0x10];
    int type;                       /* 0x218, 3 / 4 = mesh */
};

typedef struct D3IOANIMFLAGS {      /* 12 bytes passed by value to d3anim_NextFrame */
    unsigned char b0;
    unsigned char c0 : 1;           /* 0x2d */
    unsigned char rev : 1;          /* 0x2d bit 1: play backwards, starts at the last key */
    unsigned char c2 : 3;
    unsigned char c5 : 1;           /* 0x2d bit 5, cleared after loading */
    unsigned char c6 : 2;
    unsigned char b2;
    unsigned char b3;
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    unsigned char d0 : 1;           /* 0x34 */
    unsigned char d1 : 1;           /* set from the reload argument */
    unsigned char d2 : 6;
    unsigned char b9;
    unsigned char b10;
    unsigned char b11;
} D3IOANIMFLAGS;

typedef struct D3IOKEY {            /* 0x58 bytes */
    int time;
    char pad04[0x54];
} D3IOKEY;

typedef struct D3IOPATH {           /* 0x44 bytes, the animation of an object (obj->path) */
    int nkeys;
    int key;                        /* 0x04 */
    int next;                       /* 0x08 */
    int time;                       /* 0x0c */
    int unk10;
    float pos[3];                   /* 0x14, the object's position and angles at the start */
    float rot[3];                   /* 0x20 */
    D3IOANIMFLAGS flags;            /* 0x2c */
    float unk38;
    int unk3c;
    D3IOKEY *keys;                  /* 0x40 */
} D3IOPATH;

int d3io_FindStock(char *name);                                                   /* 0x44c5d0 */
char d3io_LodName(char *a, char *b, int lod, char *out1, char *out2);           /* 0x44c9d0 */
void d3_TransformNormals(void *obj, void *info);                                                        /* 0x4598d0, stock object callback */
extern D3IOOBJ d3_StockHead;                                                    /* 0xfc4a40, list sentinel */
#ifndef DATA_VIEWS_H
extern D3IOOBJ *d3_StockList;                                                   /* 0xfc4c40 */
#endif
/* 0x5fbf78 as d3_int_io.h declares it (d3io_Lod, initialized to 2): which stock object LODs are loaded */
typedef struct D3IOLODBITS {
    unsigned int mode : 7;          /* 1: load all three LODs, 2: only the best one found */
    unsigned int shadow : 1;        /* load the "_s" variant for lod < 0 */
} D3IOLODBITS;
extern D3IOLODBITS d3io_Lod;                                                    /* 0x5fbf78 */
extern char d3_AnimDir[];                                                       /* 0x5fbf80, "animations/" */
extern int d3_Unk6333cc;                                                        /* 0x6333cc */
extern void *d3_Unk6333fc;                                                      /* 0x6333fc */
#ifndef DATA_VIEWS_H
extern D3IOANIMDATA *d3_AnimList;                                                    /* 0xfe75a4 */
#endif
extern D3IOANIMDATA d3_AnimHead;                                                     /* 0xfe7440 */

void err_Message(char *msg);                                                    /* 0x4222b0 */
int bio_rd_array_quick(struct VFSFILE *f, int size, void **out);                          /* 0x401380 */
int d3anim_NextFrame(D3IOANIMFLAGS f, int frame, int a, int b, int c);              /* 0x4324e0, @original: d3_anim.c defines D3ANIMFLAGS f (12 bytes); d3_io_t.c passes its field-wise D3IOANIMFLAGS (12 bytes), as the original TU did */
struct D3BASTREAM *d3_LoadBinaryAnimStream(char *path, char *name, char relative);   /* 0x44f8c0, d3_io_b.c */

D3IOOBJ *d3io_LoadObjectFile(char *name, char *file);                           /* 0x44df40 */
D3IOOBJ *d3io_LoadObject(char *a, char *b);                                     /* 0x44e080 */
D3IOOBJ *d3io_LoadObjectRef(char *a, char *b);                                  /* 0x44e190 */
void d3io_ReleaseObject(D3IOOBJ *obj);                                          /* 0x44e2c0 */
D3IOPATH *d3_LoadObjectAnim(char *file, D3IOOBJ *obj, char reload);             /* 0x44e2e0 */
D3IOANIMDATA *d3_LoadAnimStream(char *name, char *file, char unk);                    /* 0x44e4a0 */


#endif
