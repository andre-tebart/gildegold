/* A quad tree over the x / z plane of the objects of a scene (0x457c30..0x458810), built by 0x457f20; the
   original file is the d3_par one or a neighbour (unknown). Names are ours. */
#ifndef D3_QUAD_H
#define D3_QUAD_H

typedef struct D3QITEM {            /* 8 bytes, pool 0x633db4 (128 per block) */
    D3OBJECT *obj;
    struct D3QITEM *next;           /* 0x04 */
} D3QITEM;

typedef struct D3QNODE {            /* 0x48 bytes, pool 0x633db0 (64 per block) */
    struct D3QNODE *child[4];       /* 0x00, x / z quadrants */
    D3VECTOR4 min;                  /* 0x10, bounds of the objects in the node */
    D3VECTOR4 max;                  /* 0x20 */
    int count;                      /* 0x30, number of items */
    int minitems;                   /* 0x34 */
    int maxdepth;                   /* 0x38 */
    D3QITEM *first;                 /* 0x3c */
    D3QITEM *last;                  /* 0x40 */
    unsigned int mask;              /* 0x44, object mask the tree was made for */
} D3QNODE;

/* the object bounds in D3OBJECT::material (also used by the bounding helpers elsewhere) */
typedef struct D3QMAT {
    char pad00[0x8];
    char box[0x88];                 /* 0x08, passed to d3_GetBoxVerts */
    D3VECTOR4 min;                  /* 0x90 */
    D3VECTOR4 max;                  /* 0xa0 */
    int zone;                       /* 0xb0, id of the zone the object is in */
    char padb4[0x40];
    char list[0x10];                /* 0xf4 */
    void *ops;                      /* 0x104 */
} D3QMAT;
#define D3Q_MAT(o) ((D3QMAT *)(o)->material)

struct MPOOL;
extern struct MPOOL *d3_QItemPool;  /* 0x633db4 */
extern struct MPOOL *d3_QNodePool;  /* 0x633db0 */
void *pool_Alloc(struct MPOOL **pool, unsigned int size, int count);        /* 0x422a20, zeroed (mem.h) */
void pool_Free(struct MPOOL **pool, void *ptr);                               /* 0x422b30 (mem.h) */

void d3q_FreeNode(D3QNODE *n);                                              /* 0x457c30 */
char d3q_AddObject(D3OBJECT *obj, D3QNODE *n);                              /* 0x457c90 */
D3QNODE *d3q_MakeChild(D3QNODE *parent, float *min, float *max);           /* 0x457d00 */
char d3q_GetBounds(D3OBJECT *obj, float *min, float *max);                  /* 0x457db0 */
void d3q_Build(D3QNODE *n, unsigned int minitems, volatile unsigned int depth, volatile unsigned int maxdepth);   /* 0x457f20 */
extern D3BOXVERT d3_QBox[8];        /* 0x633b30 */
unsigned int d3_ClipBox(void *box, float *zmin, float *zmax);            /* 0x439650, @original: d3_engine.h types it unsigned char; the byte-matched d3q_Mark (d3_quad.c) reads an unsigned int result (no 25 ff 00 00 00 after the call) */

D3QNODE *d3q_Create(D3OBJECT *start, unsigned int mask, unsigned int minitems, unsigned int maxdepth);  /* 0x4582d0 */
D3BOXVERT *d3q_BoxCorners(D3QNODE *n, float *pos, float *m);                /* 0x458360 */
void d3q_Mark(D3QNODE *n, D3OBJECT *cam, int zone);                         /* 0x458650 */
char d3q_Remove(D3OBJECT *obj, D3QNODE *n);                                 /* 0x4586d0 */
void d3q_RemoveObject(D3OBJECT *obj, D3QNODE *n);                           /* 0x458780 */
void d3q_Rebuild(D3OBJECT *start, D3QNODE *n);                              /* 0x4587b0 */
#endif
