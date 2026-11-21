/* d3_pose.h -- object pose helpers (0x458810..0x4591a0), see d3_pose.c. Names are ours. */
#ifndef D3_POSE_H
#define D3_POSE_H
#include "d3.h"                       /* the D3 records and typedefs this header names */
struct D3ANIMCTL;

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

void d3_DirToWorld(struct D3OBJECT *obj, struct D3VECTOR *in, struct D3VECTOR *out);                        /* 0x458810 */
void d3_WorldAngles(struct D3OBJECT *obj, D3VECTOR4 *angles);                            /* 0x458910 */
void d3_DirToRef(struct D3OBJECT *obj, struct D3OBJECT *ref, struct D3VECTOR *in, struct D3VECTOR *out);             /* 0x458950 */
/* d3_PointToWorld is declared in d3.h: (D3OBJECT *obj, D3VECTOR *v, D3VECTOR *out) */
void d3_PointToRef(struct D3OBJECT *obj, struct D3OBJECT *ref, struct D3VECTOR *in, struct D3VECTOR *out);             /* 0x458b20 */
void d3_LocalToWorld(struct D3OBJECT *obj, struct D3VECTOR *in, struct D3VECTOR *out);                        /* 0x458c10 */
void d3_MatToWorld(struct D3OBJECT *obj, char orientOnly, D3MATRIX *m, D3MATRIX *out);  /* 0x458ce0 */
void d3_WorldMatrix(struct D3OBJECT *obj, struct D3OBJECT *ref, char orientOnly, D3MATRIX *out);   /* 0x458da0 */
#endif
