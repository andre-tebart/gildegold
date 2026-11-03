struct D3VECTOR4;
/* d3_anim.c types: vertex animations ("d3_anim_CalculateAnimNormals" allocation tags).
   Field names unkXXX are byte offsets whose meaning is not known yet. */
#ifndef D3_ANIM_H
#define D3_ANIM_H
#include "d3.h"                       /* the D3 records and typedefs this header names */

typedef struct D3ANIMFRAME {        /* 0xc0 bytes, one key frame of a vertex animation */
    int time;                       /* 0x00, start time of the frame (ch_CheckAniMorph blends over half the gap) */
    int count;                      /* 0x04 */
    D3VECTOR base;                  /* 0x08, packed positions: base + byte * scale */
    D3VECTOR scale;                 /* 0x14 */
    D3VECTOR move;                  /* 0x20, root motion of the whole segment (position) */
    D3VECTOR turn;                  /* 0x2c, (angles) */
    float radius;                   /* 0x38, mesh radius minus the frame's extent */
    D3VECTOR min;                   /* 0x3c, bounding box */
    D3VECTOR max;                   /* 0x48 */
    struct { D3VECTOR pos, rot; } bone[4];  /* 0x54, skeleton animations: bone transforms */
    unsigned char *packed;          /* 0xb4, 3 bytes per vertex */
    unsigned char *normals;         /* 0xb8, 3 bytes per vertex */
    D3VECTOR *pos;                  /* 0xbc, 12 bytes per vertex */
} D3ANIMFRAME;

typedef struct D3ANIMDATA D3ANIMDATA;
struct D3ANIMDATA {                 /* 0x16c bytes, cached animation (LRU list) */
    char name[0x140];
    int nverts;                     /* 0x140 */
    int nbones;                     /* 0x144, skeleton animations: bone names follow the name */
    int nframes;                    /* 0x148 */
    int refs;                       /* 0x14c */
    int first;                      /* 0x150, first frame of the animation range (D3BASTREAM.first) */
    int last;                       /* 0x154, last frame, -1: nframes (D3BASTREAM.last) */
    unsigned int lastuse;           /* 0x158, d3_FrameCount at the last use */
    D3ANIMFRAME *frames;            /* 0x15c */
    D3ANIMDATA *next;               /* 0x160 */
    D3ANIMDATA *prev;               /* 0x164 */
    char normals;                   /* 0x168, normals calculated */
    char relative;                  /* 0x169, points relative to the frame position (D3BASTREAM.relative) */
    char pad16a[0x2];
};

typedef struct D3ANIMVERT {         /* 0x18 bytes */
    D3VECTOR pos;
    char pad0c[0xc];
} D3ANIMVERT;

typedef struct D3ANIMFACE {         /* 0x38 bytes */
    char pad00[0x18];
    int v[3];                       /* 0x18, vertex indices */
    char pad24[0x14];
} D3ANIMFACE;

typedef struct D3ANIMBONE {         /* 0x58 bytes, a bone of the mesh in its rest pose */
    char name[0x40];
    D3VECTOR pos;                   /* 0x40 */
    D3VECTOR rot;                   /* 0x4c */
} D3ANIMBONE;

typedef struct D3ANIMMESH {         /* the animated mesh */
    char pad000[0x40];
    D3ANIMVERT *verts;              /* 0x40 */
    int nverts;                     /* 0x44 */
    D3ANIMFACE *faces;              /* 0x48 */
    int nfaces;                     /* 0x4c */
    char pad050[0x24];
    D3ANIMBONE bones[4];            /* 0x74 */
    float radius;                   /* 0x1d4 */
    char pad1d8[0x30];
    unsigned char nbones;           /* 0x208 */
} D3ANIMMESH;

typedef struct D3ANIMOBJ {
    char pad00[0x40];
    unsigned long time;             /* 0x40, d3_Time */
} D3ANIMOBJ;

typedef struct D3ANIMSLOT {         /* 0x7c bytes */
    int frame;                      /* 0x00, current key frame */
    int nextFrame;                  /* 0x04, the frame the blend interpolates to (d3anim_NextFrame) */
    int subFrame;                   /* 0x08, position inside the frame, in 1/count units */
    int lastFrame;                  /* 0x0c, last frame the root motion was applied at */
    int lastSub;                    /* 0x10, sub-frame within lastFrame */
    int attachFirst;                /* 0x14, frame the bone attach starts at, -1: none */
    int attachLast;                 /* 0x18, frame it ends at, -1: none */
    int attachBone;                 /* 0x1c, bone index of the attached object */
    char pad20[0x10];
    int attached;                   /* 0x30, the object attached to the bone, 0: none */
    int fadeStart;                  /* 0x34, fade-in start time (ms) */
    int fadeEnd;                    /* 0x38, fade-in end time */
    int timeout;                    /* 0x3c, how long the animation may run (ms), -1: no limit */
    float weight;                   /* 0x40, blend weight */
    float fadeFrom;                 /* 0x44, weight at fadeStart */
    float fadeTo;                   /* 0x48, weight at fadeEnd */
    char pad4c[0x14];
    float speed;                    /* 0x60, animation speed, frames per ms */
    float frameTime;                /* 0x64, sub-frame time accumulator (d3_AnimBlendWeights) */
    D3ANIMDATA *data;               /* 0x68 */
    union {
        D3ANIMFLAGS flags;          /* 0x6c, as passed to d3anim_StartAnim */
        struct {
            unsigned char pad6c[8];
            unsigned char unk74_0 : 1;
            unsigned char play : 1; /* 0x74 bit 1 */
        };
    };
    unsigned char map[4];           /* 0x78, bone index per child object (d3anim_MapBones), 0xff = none */
} D3ANIMSLOT;

typedef struct D3ANIMCTL {          /* what d3anim_StartAnim and d3_ReleaseTexture get as "list" */
    char pad00[0x8];
    int nverts;                     /* 0x08 */
    char pad0c[0x4];
    D3ANIMMESH *mesh;               /* 0x10 */
    char pad14[0x4];
    D3ANIMOBJ *obj;                 /* 0x18 */
    D3ANIMSLOT slots[3];            /* 0x1c */
    char pad190[0x4];
    char active;                    /* 0x194 */
} D3ANIMCTL;

int d3anim_GetSize(D3ANIMDATA *a);                                                     /* 0x434a80 */
char d3anim_Purge(int limit, int size);                                                 /* 0x434af0 */
void d3anim_Release(D3ANIMDATA *a, char now);                                           /* 0x434b70 */
float d3anim_MaxRadius(D3VECTOR4 *v, int n);                                            /* 0x434d00 */
void d3anim_CalculateAnimNormals(D3ANIMDATA *a, D3ANIMMESH *mesh);                     /* 0x434d60 */

int d3anim_NextFrame(D3ANIMFLAGS f, int frame, int a, int b, int c);                        /* 0x4324e0 */
char d3anim_MapBones(D3OBJECT *obj);                                                       /* 0x4319e0 */
void d3anim_PoseBones(D3OBJECT *obj);                                                       /* 0x431ae0 */

/* Object path animations (d3anim_CreatePath; D3ANIM in d3.h is the same struct, D3ANIMKEY the same key). */
typedef struct D3PATHKEY {          /* 0x58 bytes */
    int time;                       /* frames; multiplied by 3 when the path is created */
    D3VECTOR pos;                   /* 0x04 */
    float unk10;
    D3VECTOR rot;                   /* 0x14 */
    D3VECTOR posin;                 /* 0x20, spline tangents (d3path_Tangents) */
    float unk2c;
    D3VECTOR posout;                /* 0x30 */
    float timeout;
    D3VECTOR rotin;                 /* 0x40 */
    D3VECTOR rotout;                /* 0x4c */
} D3PATHKEY;

typedef struct D3PATH {             /* 0x44 bytes, at D3OBJECT + 0x1d0 */
    int nkeys;
    int key;                        /* 0x04 */
    int next;                       /* 0x08 */
    int time;                       /* 0x0c */
    int unk10;
    D3VECTOR pos;                   /* 0x14, the object's position and angles at the start */
    D3VECTOR rot;                   /* 0x20 */
    D3ANIMFLAGS flags;              /* 0x2c */
    float fadeEnd;
    int timeout;
    D3PATHKEY *keys;                /* 0x40 */
} D3PATH;

#define D3_OBJPATH(obj) (*(D3PATH **)((char *)(obj) + 0x1d0))

/* Skeleton animations of a mesh: D3MATERIAL.tex[3] (0x178, 0x7c bytes each) hold them, the child objects
   are the bones, matched by name. */
typedef struct D3BONEANIM {
    char pad000[0x40];
    char names[4][0x40];            /* 0x40, bone names; real count unknown */
    char pad140[0x4];
    int nbones;                     /* 0x144 */
} D3BONEANIM;

typedef struct D3BONESLOT {         /* 0x7c bytes, at D3MATERIAL + 0x178 */
    D3BONEANIM *anim;
    char pad04[0xc];
    union {
        unsigned char map[4];       /* 0x10, indices of the bones that are child objects, 0xff = none */
        unsigned int mapAll;        /* the four map bytes at once; -1 = no bone mapped */
    };
    char pad14[0x68];
} D3BONESLOT;

/* Inline-asm copy as D3_COPY (d3.h), with the destination temp declared (and stored) first. */
#define D3PATH_COPY(dst, src, size) {                               \
    void *_cd = (dst); int _cn = (size);                            \
    __asm mov edi, _cd                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3PATH_COPY_LABEL(__LINE__)                           \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3PATH_COPY_LABEL(__LINE__): add ecx, ebx                 \
    __asm rep movsb                                                 \
}
#define D3PATH_COPY_LABEL(n) D3_ZERO_CAT(d3path_copy_tail, n)

void d3path_Tangents(D3PATHKEY *k0, D3PATHKEY *k1, D3PATHKEY *k2);                      /* 0x432530 */
void d3path_Init(D3PATH *p);                                                            /* 0x433b50 */


/* ---- round 3 (worker d3_left) ---- */
/* Inline-asm copy with size, src and dst through temps, declared in this order (D3_COPY3 in d3_d3d.h). */
#define D3ANIM_COPY3(dst, src, size) {                              \
    int _cn = (size); void *_cs = (src); void *_cd = (dst);         \
    __asm mov edi, _cd                                              \
    __asm mov esi, _cs                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3ANIM_COPY3_LABEL(__LINE__)                          \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3ANIM_COPY3_LABEL(__LINE__): add ecx, ebx                \
    __asm rep movsb                                                 \
}
#define D3ANIM_COPY3_LABEL(n) D3_ZERO_CAT(d3anim_copy3_tail, n)
#define D3ANIM_BONENAME(a, i) ((a)->name + 0x40 * ((i) + 1))     /* bone names follow the name */
typedef struct D3ANIMMESHX {        /* D3ANIMMESH with the root position and angles */
    char pad000[0x40];
    D3ANIMVERT *verts;              /* 0x40 */
    int nverts;                     /* 0x44 */
    char pad048[0x8];
    D3VECTOR pos;                   /* 0x50 */
    D3VECTOR rot;                   /* 0x5c */
} D3ANIMMESHX;
D3ANIMDATA *d3anim_CreateMorphAnim(D3ANIMMESH *mesh, D3ANIMDATA *src, D3ANIMFRAME *frame, D3ANIMDATA *dst,
                                   D3ANIMFRAME *dframe, D3ANIMMESH *dmesh, char *name, int n);  /* 0x433f80 */

/* declared in other units' headers before (moved to the owner) */
void d3anim_ApplyRootMotion(D3OBJECT *obj, D3ANIMSLOT *s, int from, int to, int flags);  /* 0x4315c0 */
void d3anim_PauseAnim(D3ANIMCTL *ctl, char *name);  /* 0x435810 */
void d3anim_PlayAnim(D3ANIMCTL *ctl, char *name, char on);  /* 0x435870 */
void d3anim_SetFrame(D3OBJECT *obj, D3ANIMSLOT *s, int frame);  /* 0x435950 */
char d3anim_StepObject(D3OBJECT *obj, void *ctx);  /* 0x432750 */

#endif
