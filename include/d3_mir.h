/* d3_mir.c -- mirrors ("d3_mir:" allocation tags): the clipping planes of a mirror polygon's outline, the
   mirrored copy of the objects behind it. Original file name unknown (d3_mir.c?). Names are ours. */
#ifndef UNK_454890_H
#define UNK_454890_H

typedef struct D3MIRPLANES {        /* mem_Alloc'd: n planes of the mirror outline plus the view clip planes */
    unsigned int n;                 /* 0x00 */
    unsigned char mirror;           /* 0x04, set to 1: the planes are the mirror outline's (D3CLIPINFO.mirror) */
    char pad05[0x3];
    D3VECTOR4 p[1];                 /* 0x08, n of them: normal and distance */
} D3MIRPLANES;

typedef struct D3MIRFLAGS {
    unsigned char visible : 1;      /* a visible polygon with the mirror texture */
    unsigned char on : 1;           /* the object's state bit 3 */
    unsigned char b2 : 6;
} D3MIRFLAGS;

typedef struct D3MIRROR {           /* d3_Mirror (0xfb9900), the mirror of the frame */
    D3OBJECT *obj;                  /* 0x00 */
    D3OBJINFO *info;                /* 0x04, obj->info when the polygon was looked up */
    D3MIRPLANES *planes;            /* 0x08 */
    D3TEXTURE *tex;                 /* 0x0c, the mirror texture */
    D3PTRI *tri;                    /* 0x10, the first polygon with it */
    void (*Transform)(D3PVERT *v, int n, int unk);  /* 0x14, d3_MirTransform */
    D3VECTOR4 plane;                /* 0x18, mirror plane (camera space) */
    D3MIRFLAGS flags;               /* 0x28 */
} D3MIRROR;

typedef struct D3MIRSURF {          /* D3SURF as the mirror code sees it */
    D3OBJINFO info;
    D3TEXTURE **tex;                /* 0x14, D3_OPS(info)->count of them */
} D3MIRSURF;

typedef struct D3MIRBITS {          /* view of D3OBJECT::objectFlags */
    unsigned char b0 : 6;
    unsigned char mirrored : 1;     /* 0x40, its mirror image is visible */
    unsigned char b7 : 1;
} D3MIRBITS;

/* Inline-asm copy as D3_COPY3 (d3_d3d.h): size, src and dst through temps, declared in this order. */
#define D3M_COPY(dst, src, size) {                              \
    int _cn = (size); void *_cs = (src); void *_cd = (dst);     \
    __asm mov edi, _cd                                          \
    __asm mov esi, _cs                                          \
    __asm mov ebx, _cn                                          \
    __asm lea ecx, [edi+edi*2]                                  \
    __asm and ecx, 3                                            \
    __asm sub ebx, ecx                                          \
    __asm jle D3_COPY_LABEL(__LINE__)                           \
    __asm rep movsb                                             \
    __asm mov ecx, ebx                                          \
    __asm and ebx, 3                                            \
    __asm shr ecx, 2                                            \
    __asm rep movsd                                             \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                 \
    __asm rep movsb                                             \
}

/* Inline-asm clear as D3_ZERO (d3.h) with the destination and the size named directly in the asm. */
#define D3M_ZERO_VAR(dst, size) {                                   \
    __asm mov edi, dst                                              \
    __asm mov ebx, size                                             \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_ZERO_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm D3_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}

extern D3MIRROR d3_Mirror;        /* 0xfb9900 (d3_Unkfb9900.. in d3_engine.h) */

void d3_DirToRef(struct XOBJ *obj, struct XOBJ *ref, float *in, float *out);             /* 0x458950 */
void d3_TriNormal(struct D3VECTOR4 *a, struct D3VECTOR4 *b, struct D3VECTOR4 *c, struct D3VECTOR4 *n);   /* 0x454700, face normal (d3_vec.h; d3_mir.c calls it through its vertex view d3_TriNormalM) */
/* 0x454780: the outline edges of the convex point set pts (pairs into edges, 2 * count into *ne) */
char d3_MirBoundaryEdges(unsigned int n, unsigned char *conn, D3PVERT **pts, unsigned int *ne, D3PVERT **edges);

/* 0x454890, "d3_mir:CreateOutline*": joins collinear outline edges; *out gets the corner pairs */
char d3_MirCreateOutline(D3PVERT **pts, unsigned int n, unsigned int *nout, D3PVERT ***out);

D3MIRPLANES *d3_MirCreatePlanes(D3OBJECT *obj, D3TEXTURE *tex);      /* 0x454be0, "d3_mir:CrClippingPlanesFinal" */
void d3_MirTransform(D3PVERT *v, int n, int unk);                  /* 0x454ec0, mirrors points at d3_Mirror.plane */
char d3_MirBoxInPlanes(D3OBJECT *obj, D3MIRROR *m);                  /* 0x454f70, mirrored bounding box visible? */
char d3_MirDrawObject(D3OBJECT *obj, D3MIRROR *m);                  /* 0x4550e0, d3_ForEachObject callback */
void d3_MirBeginFrame(D3OBJECT *obj, D3MIRROR *m);                  /* 0x4554b0, finds the mirror polygon */

#endif
