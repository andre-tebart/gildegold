struct XOBJ;
/* d3_pose.h -- object pose helpers (0x458810..0x4591a0), see d3_pose.c. Names are ours. */
#ifndef D3_POSE_H
#define D3_POSE_H
#include "d3.h"                       /* the D3 records and typedefs this header names */
struct D3ANIMCTL;

/* D3OBJECT as the pose code sees it */
typedef struct XOBJ XOBJ;
struct XOBJ {
    char pad00[0x4c];
    D3VECTOR p4c;                   /* 0x4c */
    char pad58[0x14];
    D3VECTOR p6c;                   /* 0x6c */
    D3VECTOR p78;                   /* 0x78 */
    char pad84[0x108];
    D3MATRIX m;                     /* 0x18c */
    char pad1cc[0x2c];
    XOBJ *parent;                   /* 0x1f8 */
    char pad1fc[0x14];
    unsigned char flags;           /* bit 7: world space */
};

typedef struct XSRC {               /* source point of a vertex */
    char pad00[0xc];
    float nx, ny, nz;               /* 0x0c */
} XSRC;
typedef struct XVERT {              /* 0x50 bytes */
    float x, y, z;                  /* 0x00, camera space */
    char pad0c[0x14];
    float u, v;                     /* 0x20, environment map coordinates */
    char pad28[0x20];
    XSRC *src;                      /* 0x48 */
    char flags;                     /* 0x4c */
    char used;                      /* 0x4d */
    char pad4e[0x2];
} XVERT;

typedef struct XPART {              /* a mesh part, what D3OBJINFO has before its ops */
    struct XVERT *keys;             /* 0x00, stride 0x50 */
    void *tris;
    int nkeys;                      /* 0x08 */
    int ntris;
    struct XOPS *ops;               /* 0x10 */
    struct XTEX **tex;              /* 0x14 */
} XPART;
typedef struct XOPS {
    char pad000[0x1e0];
    int count;                      /* 0x1e0 */
} XOPS;
typedef struct XTEX {
    char pad00[0x68];
    unsigned char flags;            /* 0x68 */
} XTEX;

struct D3ANIMSLOT * d3anim_GetMainSlot(struct D3ANIMCTL *ctl);                                         /* 0x435900 */
void d3_MatMul43(D3MATRIX *a, D3MATRIX *b, D3MATRIX *out);                  /* 0x453ee0 */
void d3_MatToAngles(D3VECTOR4 *m, D3VECTOR4 *angles);                         /* 0x454690 */

void d3_DirToWorld(XOBJ *obj, float *in, float *out);                        /* 0x458810 */
void d3_WorldAngles(XOBJ *obj, D3VECTOR4 *angles);                            /* 0x458910 */
void d3_DirToRef(struct XOBJ *obj, struct XOBJ *ref, float *in, float *out);             /* 0x458950 */
/* d3_PointToWorld is declared in d3.h: (D3OBJECT *obj, D3VECTOR *v, D3VECTOR *out) */
void d3_PointToRef(XOBJ *obj, XOBJ *ref, float *in, float *out);             /* 0x458b20 */
void d3_LocalToWorld(struct XOBJ *obj, float *in, float *out);                        /* 0x458c10 */
void d3_MatToWorld(XOBJ *obj, char orientOnly, D3MATRIX *m, D3MATRIX *out);  /* 0x458ce0 */
void d3_WorldMatrix(XOBJ *obj, XOBJ *ref, char orientOnly, D3MATRIX *out);   /* 0x458da0 */
#endif
