struct VFSFILE;
struct D3OBJECT;
struct D3MIRROR;
struct D3FLMAP;                     /* the tag (d3_fl.h spells it out; MS compat needs the forward declaration) */
/* d3_engine.c types (only what the functions from 0x438c30 need so far). */
#ifndef D3_ENGINE_H
#define D3_ENGINE_H
#include <windows.h>                  /* HWND (d3_Init) */
#include "d3.h"                       /* the D3 records and typedefs this header names */
struct XOBJ;
struct D3SOUND;

typedef struct D3SCENELISTS {       /* view of the D3SCENE fields used by the object walks */
    char pad000[0x80];
    D3OBJECT *first;                /* 0x80, first top-level object (d3_TopFirst) */
    D3OBJECT *last;                 /* 0x84, last top-level object (d3_TopLast) */
    char pad088[0x1c];
    void *parHead;                  /* 0xa4, first of the particle list (d3_ParHead, D3PARTSYS * in d3_par.h) */
    void *parTail;                  /* 0xa8, last of the particle list (d3_ParTail, D3PARTSYS * in d3_par.h) */
} D3SCENELISTS;

typedef union D3CLIP {              /* clip code of a point or a box */
    unsigned char raw;              /* the code as a whole: cleared, and | / & of two codes */
    struct {
        unsigned char left : 1;
        unsigned char right : 1;
        unsigned char top : 1;
        unsigned char bottom : 1;
        unsigned char znear : 1;
        unsigned char zfar : 1;
        unsigned char out : 1;      /* all points outside one plane */
        unsigned char b7 : 1;
    };
} D3CLIP;

typedef struct D3PVERT {            /* 0x50 bytes, a mesh vertex after the transformation (D3VERTEX) */
    float x, y, z;                  /* camera space */
    float pad0c;
    float sx, sy;                   /* 0x10, screen position */
    float sz;
    float rhw;                      /* 0x1c */
    char pad20[0x20];
    union {
        unsigned long color;        /* 0x40 */
        D3COLOR c;
    };
    union {
        unsigned long specular;     /* 0x44 */
        D3COLOR s;
    };
    void *src;                      /* 0x48, source point (D3SRCPOINT) */
    union {
        signed char flags;          /* 0x4c, bit 7 = visible */
        D3CLIP clip;                /* the same byte as the clip code d3_ClipMesh computes */
        unsigned char rawFlags;
    };
    unsigned char unk4d;
    char pad4e[0x1];
    unsigned char fog;              /* 0x4f */
} D3PVERT;

typedef struct D3PTRI {             /* 0x28 bytes (D3TRI) */
    D3PVERT *v[3];
    void *clip;                     /* 0x0c, &d3_ClipInfo[clip code] */
    void *src;                      /* 0x10, source polygon (D3SRCPOLY); a particle tri's static UV table */
    D3TEXTURE *tex;                 /* 0x14 */
    int unk18;
    int unk1c;
    int unk20;
    unsigned char flags;            /* 0x24, bit 7 = visible, 0x40 = double sided drawn, 0x10 = double sided */
    unsigned char unk25;
    unsigned char drawFlags;        /* 0x26, bit 2 (4) = no culling, bit 3 (8) = wrap (D3TRIBITS) */
    char pad27[0x1];
} D3PTRI;

extern D3OBJECT d3_ObjEnd;                  /* 0xfc2700, list end marker */
/* d3_TopFirst and d3_TopLast are not storage of their own: the original's 0xfc52f0 is
   d3_TopHead.next (0xfc5100 + 0x1f0) and 0xfc28f4 is d3_ObjEnd.prev (0xfc2700 + 0x1f4).
   Both spellings are one object, which d3_LinkTop / d3_UnlinkTop require. */
#define d3_TopFirst (d3_TopHead.next)      /* 0xfc52f0, first top-level object */
#define d3_TopLast (d3_ObjEnd.prev)        /* 0xfc28f4, last top-level object */
#ifndef DATA_VIEWS_H
extern struct D3PARTSYS *d3_ParHead;                    /* 0xfb98f4, particle list head (D3PARTSYS * in d3_par.h) */
#endif
#ifndef DATA_VIEWS_H
extern struct D3PARTSYS *d3_ParTail;                    /* 0xfb9598, particle list tail (D3PARTSYS * in d3_par.h) */
#endif

extern unsigned char d3_Hardware;          /* 0x633450 */
extern char d3_FogOn;                       /* 0x6334a4 */
extern float d3_ScaleX;                 /* 0xfc5348, projection */
extern float d3_ScaleY;                 /* 0xfc2944 */
extern float d3_CenterX;                /* 0xfe75fc */
extern float d3_CenterY;                /* 0xfe7604 */
extern float d3_Dist2;                      /* 0xfc294c */
extern float d3_FogStart2;                  /* 0xfc5330, squared */
extern float d3_FogStart;                   /* 0xfc300c */
extern float d3_FogScale;                   /* 0xfc2948 */

/* ---- from 0x439000 on ---- */

/* D3OBJECT fields not in d3.h's declaration */
typedef struct D3OBJBITS {          /* view of D3OBJECT::state as bitfields */
    unsigned char top : 1;          /* starts a group in a top-level list */
    unsigned char linked : 1;
    unsigned char dirty : 1;
    unsigned char b3 : 5;
} D3OBJBITS;
__inline D3OBJBITS *d3_ObjBits(D3OBJECT *obj)
{
    return (D3OBJBITS *)&obj->state;
}
#define D3_OBJBITS(o) (*d3_ObjBits(o))

typedef struct D3OPS D3OPS;
struct D3OPS {                      /* the "driver" behind D3OBJINFO::unk10: a stock object, the same records as D3BGF (d3_io_t.h; not merged yet) */
    char pad000[0x1e0];
    int count;                      /* 0x1e0, number of textures */
    int nsets;                      /* 0x1e4, texture sets in the name table (D3BGF.nsets, d3_io_t.h) */
    void (*Release)(D3OPS *ops);    /* 0x1e8 */
    int unk1ec;
    void (*Transform)(D3OBJECT *obj, D3OBJINFO *info, void *ctx);       /* 0x1f0 */
    void *(*GetBox)(D3OBJECT *obj, D3OBJINFO *info, void *unk);         /* 0x1f4 */
    char pad1f8[0xc];               /* 0x1f8, the driver's other callbacks and stock list links (D3BGF, d3_io_t.h) */
    char *texnames;                 /* 0x204, nsets * count texture names of 0x40 bytes, set-major (D3BGF.texnames) */
};
#define D3_OPS(info) ((D3OPS *)(info)->mesh)

typedef struct D3SURFTEX {     /* 0x7c bytes, one of a surface's three animation slots: the same memory as
                                       D3ANIMSLOT (d3_anim.h), which d3anim_StartAnim fills through D3ANIMCTL
                                       (the mesh's surf[0] area) */
    int frame;                      /* 0x00, D3ANIMSLOT.frame: current key frame */
    int nextFrame;                  /* 0x04, D3ANIMSLOT.nextFrame: the frame the blend interpolates to */
    int subFrame;                   /* 0x08, D3ANIMSLOT.subFrame: position inside the frame, in 1/count units */
    char pad0c[0x5c];
    void *data;                     /* 0x68, D3ANIMSLOT.data: the D3ANIMDATA; its name string is the first bytes */
    int flags;                      /* 0x6c, first dword of D3ANIMSLOT.flags (D3ANIMFLAGS) */
    char pad70[0x4];
    unsigned char playFlags;        /* 0x74, D3ANIMSLOT flag byte 8: bit 1 = play (CHANIMFLAGS.b8) */
    char pad75[0x7];
} D3SURFTEX;

typedef struct D3SURF {             /* 0x198 bytes, a mesh part with its own geometry */
    D3OBJINFO info;                 /* points, polys, counts, ops */
    void *texHandles;               /* 0x14, array of the surf's texture handles (D3TEXHANDLE *, ops->count of them) */
    D3OBJECT *owner;                /* 0x18, the object whose mesh this surf belongs to (set by d3_InitSurf) */
    D3SURFTEX slots[3];        /* 0x1c, the three animation slots (D3ANIMSLOT) */
    union {
        int i;
        D3TEXFLAGS f;               /* f.b[0] = alpha */
    } flags;                        /* 0x190 */
    unsigned char unk194;           /* 0x194, nonzero: transforms / draws the surf even when nothing changed (D3IOSURF.f194) */
    unsigned char texSet;           /* 0x195, current texture set: index into the ops' name table (D3IOSURF.texSet) */
    char pad196[0x2];
} D3SURF;

typedef struct D3MESHSUB {          /* 0x80 bytes, D3SSLOT (d3s.h): the shadow of one light, 4 per mesh at
                                       D3S_SLOTS; only cleared here */
    D3TEXHANDLE *tex;               /* 0x00, D3SSLOT.tex */
    D3OBJECT *light;                /* 0x04, D3SSLOT.light */
    char pad08[0x10];
    D3VECTOR dir;                   /* 0x18, D3SSLOT.dir: light direction the shadow was made for */
    char pad24[0x5c];
} D3MESHSUB;

typedef struct D3MESH {             /* 0x970 bytes, what D3OBJECT::material points to ("d3:AddObjectDrawData") */
    void (*freeCallback)(D3OBJECT *obj);    /* 0x000, optional object callback, called before the mesh is freed
                                       (d3_DisposeObject); only cleared in the drafted code */
    D3OBJECT *camera;               /* 0x004, the camera cameraMatrix was transformed for */
    char worldMatrix[0x40];         /* 0x008, the object's world matrix (D3MATRIX, d3_WorldMatrix writes it) */
    char cameraMatrix[0x40];        /* 0x048, worldMatrix with the camera transform applied (D3MATRIX);
                                       GetBox / Transform take it to put the box / the verts in camera space */
    float zMin;                     /* 0x088, nearest z of the box in camera space (d3_ClipBox writes it) */
    float zMax;                     /* 0x08c, farthest z of the box in camera space */
    char pad090[0x20];
    int frameStamp;                 /* 0x0b0, the frame the mesh was last transformed/clipped in (d3_FrameCount);
                                       equal to the current frame: already current, only flag 0x40 is set;
                                       -1 = never (d3_AddObjectDrawData) */
    char boneName[0x40];            /* 0x0b4, the parent's bone the object is attached to (d3_AttachToBone writes it,
                                       d3anim_MapBones / d3anim_PoseBones match the children by it); the length is the
                                       room to surf */
    D3SURF surf[4];                 /* 0x0f4, surf[3] at 0x5bc */
    D3MESHSUB sub[4];               /* 0x754 */
    int groundOffsetRaw;            /* 0x954, the ground offset written as a whole dword (cleared and copied) */
    float lightScale;               /* 0x958, light brightness scale (ch.c: 1.8 for characters) */
    unsigned int lightGeneration;   /* 0x95c */
    D3PVERT *mirrorPoints;          /* 0x960, mirrored point array ("d3_mir:MirroredPoints") */
    D3PTRI *mirrorPolys;            /* 0x964, mirrored polygon array ("d3_mir:MirroredPolys") */
    D3SURF *mirrorSource;           /* 0x968, the surf the mirror copy was made from */
    unsigned char nsurf;            /* 0x96c */
    unsigned char flags;            /* 0x96d, 0x40 = no vertex animation (D3ANIMMATV.flags in d3_anim.c) */
    char pad96e[0x2];
} D3MESH;
#define D3_MESH(o) ((D3MESH *)(o)->material)

extern const int d3_TriEdge[3][2];        /* 0x5eba80, per triangle vertex: [0] the next one, [1] the previous one */
extern D3OBJECT *d3s_Object;              /* 0x633448 */
extern struct D3SKY *d3_Sky;                /* 0x633dd4, d3_sky.c (D3SKY, d3_sky.h) */
extern int d3s_NumUVs;                /* 0xfb925c */
extern int d3s_NumVerts;                /* 0xfb9220 */
extern int d3_ClipExtraTris;                    /* 0x63346c */
#include "d3_types.h"                /* D3SORTENT */
extern unsigned int d3_SortCount;           /* 0xfc2dcc */
extern D3SORTENT *d3_SortBuf;               /* 0xfc2de0 */
extern D3SORTENT *d3_SortPtr;               /* 0xfc2dc8 */
extern int d3_TriCount;                /* 0xfc7b60 */
extern int d3_VertCount;                /* 0xfc2dc4 */
extern int d3_DrawnVerts;              /* 0xfe75b0, vertices drawn this frame (d3_DrawSorted resets it) */
/* a byte read as an int bitfield (mov dword; and 0xff): d3_Init passes d3_D3DActive so */
struct D3BITS8 { unsigned int on : 8; };
extern unsigned char d3_EngineReady;          /* 0x633451, engine up: set at the end of d3_Init, cleared in d3_Shutdown */
struct D3BITS3 { unsigned char mode : 3; };  /* d3_ShadowMode (0xfe7671) is a view of d3d_Config + 1 (data_views.h): bit 0 = object shadows on (d3_UpdateObject / d3_Render); set by d3_ApplyGraphicsOptions */
extern unsigned char d3_MipSkip;          /* 0x63342c, d3_d3d.c */
extern int d3_FrameCount;                    /* 0x63343c */
extern unsigned int d3_SceneFlags;           /* 0x63349c */

void d3s_ResetShadows(D3OBJECT *obj);       /* 0x45ce00, d3s.c, d3_ForEachObject2 callback */
void d3s_FreeShadows(D3OBJECT *obj);        /* 0x45ccd0, d3s.c, d3_ForEachObject2 callback */
void d3_WorldMatrix(struct XOBJ *obj, struct XOBJ *ref, char orientOnly, D3MATRIX *out);   /* 0x458da0 */
void d3fl_SetMips(char a);               /* 0x43f930, d3_fl.c (takes char; the 0x43c4e0 caller pushes an int) */
void d3fl_SetDetail(unsigned int n); /* 0x43f8a0, d3_fl.c (takes unsigned int; the 0x43c4e0 caller pushes an int) */
void tex_ReloadAll(char keep, void (*cb)(int, int, int));       /* 0x461090, ts_texture.c: void tex_ReloadAll(char, void (*)(int, int, int)) */
void d3fl_FreeTiles(struct D3FLMAP *map);   /* 0x441db0, d3_fl.c */

/* d3_engine.c functions referenced before their definition */
D3OBJINFO *d3_SelectSurf(D3OBJECT *obj);                                                 /* 0x43a000 */
void d3_DrawSortedD3D(void);                                                                /* 0x43a7d0 */
void d3_DrawSortedSW(void);                                                                /* 0x43b080 */
void d3_RadixSort(unsigned int n, unsigned char twobytes);                              /* 0x43b300 */
void d3_UpdateAll(unsigned char full);                                                  /* 0x43b6e0 */
void d3_LinkTop(D3OBJECT *obj);                                                         /* 0x43ca20 */
void d3_SetCamera(D3OBJECT *obj);                                                       /* 0x43cbe0 */
void d3_FreeSurf(D3SURF *s);                                                            /* 0x43cc90 */
void d3_DeflateObject(D3OBJECT *obj);                                                       /* 0x43da00 */

/* ---- round 2 (0x439000..0x43e000) ---- */
/* view and fog state */
extern float d3_ViewW;                  /* 0xfe73c8 */
extern float d3_ViewH;                  /* 0xfe73c4 */
extern float d3_ViewX;                  /* 0xfe73a8 */
extern float d3_ViewY;                  /* 0xfc5328 */
extern float d3_ZNear;                  /* 0xfe73b4 */
extern float d3_ZFar;                   /* 0xfc2564 */
extern int d3_Unkfc2df4;                /* 0xfc2df4 */
extern int d3_Unkfc2df0;                /* 0xfc2df0 */
extern float d3_FogEnd;                     /* 0xfc2dd8 */
extern float d3_SavedZFar;                  /* 0xfe73bc, d3_ZFar while the fog is off */
extern float d3_FogEnd2;                    /* 0xfc5340 */
extern float d3_FogStart1;                  /* 0xfc29bc */
extern D3COLORU d3_fog_color;               /* 0x6334a0 */
#ifndef DATA_VIEWS_H
extern unsigned char d3d_Caps98d;           /* 0xfe798d, bit 5 = fog supported */
#endif
extern struct D3SM *d3sm_Shadow;                  /* 0x633df8 */

typedef struct D3RELOADCTX {        /* context of d3_ReloadMeshCB */
    void (*progress)(int a, int i, int n);
    int type;                       /* 0x04, compared with D3OBJECT::kind */
    int n;                          /* 0x08 */
    int i;                          /* 0x0c */
    char count;                     /* 0x10, only count the objects */
} D3RELOADCTX;

unsigned char d3_ClipBox(void *box, float *zmin, float *zmax);                        /* 0x439650 */
void d3_SetFogRange(float start, float end);                                            /* 0x43a610 */
void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b); /* 0x43b950 */
void d3_FreeObjectData(D3OBJECT *obj);                                                  /* 0x43c670 */
void d3fl_Inflate(struct D3FLMAP *map);                                                 /* 0x441ea0, d3_fl.c */
struct D3SM; struct D3TERRAIN; void d3sm_Build(struct D3SM *h, struct D3TERRAIN *t);                                                    /* 0x45e620, d3_sm.c: void d3sm_Build(D3SM *, D3TERRAIN *) */
void d3_AttachMesh(D3OBJECT *obj, int unk, char *name);                                  /* 0x44cbb0 */
struct D3BGF *d3io_LoadObject(char *a, char *b);                                         /* 0x44e080, d3_io_t.c (d3io_LoadObject; d3_io_t.h declares it too) */

typedef struct D3LIGHTENTRY {       /* 0x38 bytes */
    char pad00[0x30];
    float unk30;
    int unk34;
} D3LIGHTENTRY;

typedef struct D3LIGHTINFO {        /* 0x1ac bytes, "d3:SpawnObject(lightinfo)", objects of type >= 5 */
    D3LIGHTENTRY e[7];
    char pad188[0xc];
    int time;                       /* 0x194, last flicker step (d3_Time); D3LDATA.time in d3_light.h */
    int unk198;
    float flicker;                  /* 0x19c, D3LDATA.flicker (d3_light.h); 0.3 from d3_InitObject, else the scene file's */
    int interval;                   /* 0x1a0, D3LDATA.interval (d3_light.h); 15 from d3_InitObject, else the scene file's */
    int unk1a4;
    char pad1a8[0x4];
} D3LIGHTINFO;

typedef struct D3OBJX {             /* 0x228 bytes, the whole D3OBJECT as far as known */
    char name[0x40];
    int offTime;                    /* 0x40, d3_Time when d3_ToggleRLightCB switched the light off (type 6);
                                       cleared in d3_InitObject, no reader in the sources so far */
    int unk044;
    char pad048[0x4];
    D3VECTOR pos;                   /* 0x04c */
    int unk058;
    D3VECTOR unk05c;
    float radius;                   /* 0x068 */
    D3VECTOR unk06c;
    D3VECTOR unk078;
    D3VECTOR rot;                   /* 0x084 */
    D3VECTOR unk090;
    struct { int v[6]; } unk09c[10];
    D3MATRIX matrix;                /* 0x18c */
    D3OBJINFO *info;                /* 0x1cc */
    void *path;                     /* 0x1d0, the object path animation (d3anim_FreePath) */
    void *events;                   /* 0x1d4, D3E_NEVENTS event handlers (d3e:RegisterEvent(eventtable)) */
    char pad1d8[0x10];
    D3LIGHTINFO *light;             /* 0x1e8 */
    D3MESH *mesh;                   /* 0x1ec */
    D3OBJECT *next;                 /* 0x1f0 */
    D3OBJECT *prev;
    D3OBJECT *parent;
    D3OBJECT *child;
    char pad200[0x8];
    D3SCENE *scene;                 /* 0x208 */
    void *sound;                    /* 0x20c */
    unsigned char state;
    unsigned char objectFlags;      /* 0x211 (D3OBJECT::objectFlags) */
    unsigned char stateFlags;
    unsigned char flags;
    int meshMode;                   /* 0x214, D3OBJECT.meshMode (d3.h) */
    int type;                       /* 0x218 */
    int type0;                      /* 0x21c, the object's real type (D3OBJECT::type0): saved while d3if_Show hides it, set at spawn */
    int kind;                       /* 0x220 */
    int mask;
} D3OBJX;
#define D3_X(o) ((D3OBJX *)(o))

extern char d3_Unk633438;                   /* 0x633438 */
extern float d3_InvZFar;                    /* 0xfc2dd4, 1 / d3_ZFar */
extern char d3_LightsChanged;                   /* 0x633b14 */
char d3s_FreeLightCB(D3OBJECT *obj, D3OBJECT *light);                                         /* 0x45cd60, d3s.c, d3_ForEachObject callback */
void d3anim_PoseBones(D3OBJECT *obj);                                                       /* 0x431ae0 */
void d3_InitObject(D3OBJECT *obj);                                                      /* 0x43ce60 */
void d3_FreeMesh(D3OBJECT *obj);                                                        /* 0x43cd80 */

extern float d3_HalfW;                  /* 0xfe75f8 */
extern float d3_HalfH;                  /* 0xfe75b8 */
extern int d3_Unkfe73a4;                    /* 0xfe73a4 */
extern float d3_Unkfc2ddc;                  /* 0xfc2ddc */
extern char dd_Cleared;                     /* 0x631222, dd_vesa.c */
extern char dd_ZCleared;                   /* 0x631223, dd_vesa.c */
void d3d_SetViewport(float w, float h, float x, float y, float zn, float zf);          /* 0x436de0 */
void d3_SetupFrustum(void);                                                             /* 0x4391d0, sets d3_Planes / d3_Frustum / d3_ClipInfo from the viewport */

void d3_UpdateWorldMatrix(struct D3OBJECT *obj, struct D3OBJECT *camera, char force);                       /* 0x439870 */

void d3_PointToRef(struct XOBJ *obj, struct XOBJ *ref, float *in, float *out);             /* 0x458b20 */

typedef struct D3TEXMAP {           /* d3_SetObjectAlpha: a texture and its replacement */
    D3TEXTURE *from;
    D3TEXTURE *to;
} D3TEXMAP;
struct TSTEXTURE; struct TSTEXTURE *tex_ReleaseAlias(struct TSTEXTURE *tex, char group);                                /* 0x461610, ts_texture.c */
struct TSTEXTURE; struct TSTEXTURE *tex_AliasForFlags(struct TSTEXTURE *tex, D3TEXFLAGS flags, char group);  /* 0x4616a0, ts_texture.c (TSFLAGS is D3TEXFLAGS) */

extern D3PARTSYS d3_ParEnd;                 /* 0xfb9280, particle system list end marker */
void par_Delete(D3PARTSYS *ps);                                                         /* 0x455760 */
void d3snd_Free(struct D3SOUND *s);                                                     /* 0x4179d0, d3_snd.c */
void d3_DisposeObject(D3OBJECT *obj);                                                   /* 0x43c7a0 */
void d3_UnlinkTop(D3OBJECT *obj);                                                       /* 0x43cab0 */

#define D3_SCENE3C8(s) ((s)->events)   /* D3SCENE.events (d3.h); the typed view is the field itself */
extern unsigned int d3s_MaxPolys;           /* 0x633dc4 */
extern int d3if_MegaCam;                    /* 0x6338c4 */
extern struct D3BGF *d3_StockListHead;      /* 0xfc2ffc, head of the stock object (mesh driver, D3BGF in d3_io_t.h) list, walked with ->Release (a view of d3_Unkfc2e00+0x1fc in the units that use it) */
extern struct D3BGF d3_StockListEnd;       /* 0xfc4a40, its end sentinel */
extern struct D3ANIMDATA d3anim_Head;      /* 0xfc2580, the animation cache list (D3ANIMDATA, d3_anim.h): its head sentinel */
extern struct D3ANIMDATA d3anim_Tail;      /* 0xfe7440, and its end sentinel */
extern D3SORTENT *d3_SortTmp;               /* 0xfe75c0 */
extern unsigned int d3_RadixCount[256];     /* 0xfc29c0 */
extern D3SORTENT *d3_RadixSrc;          /* 0xfe73c0 */
extern D3SORTENT *d3_RadixDst;          /* 0xfc50c8 */
struct D3SKY; void d3sky_Destroy(struct D3SKY *s);                /* 0x45daa0, d3_sky.c */
void d3sky_Free(void);                      /* 0x45db20, d3_sky.c */
void par_DeleteAll(void);                   /* 0x4556b0, d3_par.c */
void d3_ExitShadows(void);                    /* 0x45aee0 */
void d3fl_Exit(void);                       /* 0x43fed0, d3_fl.c */
void tex_Exit(void);                        /* 0x45fd00, ts_texture.c */
void d3d_Exit(void);                        /* 0x4370e0, d3_d3d.c */
void dd_Exit(void);                         /* 0x41be00, dd_vesa.c */

extern D3VECTOR4 d3_Planes[6];              /* 0xfc5068, side planes of the view frustum (normal, distance); the code writes 6 (near/far) */
extern float d3_MinZ;                   /* 0xfc5344 */
extern float d3_MaxZ;                   /* 0xfc5338 */

typedef struct D3TRIBITS {          /* D3PTRI::drawFlags as bitfields */
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;           /* 4 = no culling */
    unsigned char b3 : 1;
    unsigned char b47 : 4;
} D3TRIBITS;

typedef struct D3SRCPOLY {          /* 0x38 bytes */
    char pad00[0x18];
    int idx[3];                     /* 0x18, point indices */
    int mat;                        /* 0x24, material index, < 0: none */
    int slot;                       /* 0x28, texture slot: index into the surf's texture handles (D3IOPOLY.slot) */
    D3VECTOR normal;                /* 0x2c, the plane normal: d3_MirBeginFrame turns it (a direction) into the
                                       mirror plane; the name is a reasoned guess from that use */
} D3SRCPOLY;

typedef struct D3MESHSRC {          /* view of D3OPS: the loaded mesh data */
    char pad00[0x40];
    char *points;                   /* 0x40, 0x18 bytes each */
    int npoints;
    D3SRCPOLY *polys;               /* 0x48 */
    int npolys;
} D3MESHSRC;

/* Inline-asm byte fill: value and size through temps (declared in this order), dst named directly. */
#define D3E_FILL(dvar, val, size) {                                 int _fv = (val); int _fn = (size);                              __asm mov edi, dvar                                             __asm mov eax, _fv                                              __asm mov ebx, _fn                                              __asm mov ah, al                                                __asm shl eax, 16                                               __asm mov ecx, eax                                              __asm shr ecx, 16                                               __asm add eax, ecx                                              __asm lea ecx, [edi+edi*2]                                      __asm and ecx, 3                                                __asm sub ebx, ecx                                              __asm jle D3_COPY_LABEL(__LINE__)                               __asm rep stosb                                                 __asm mov ecx, ebx                                              __asm and ebx, 3                                                __asm shr ecx, 2                                                __asm rep stosd                                                 __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     __asm rep stosb                                             }

#define D3E_LBL(name, n) D3_ZERO_CAT(name, n)
/* One pass of the radix sort (inline asm): sorts n D3SORTENTs from d3_RadixSrc into d3_RadixDst by key byte b;
   d3_RadixCount must be zero. */
#define D3E_RADIX(n, b) {                                           int _rb = (b);                                                  __asm mov ecx, n                                                __asm mov edi, _rb                                              __asm xor eax, eax                                              __asm mov esi, d3_RadixSrc                                      __asm add esi, edi                                              __asm D3E_LBL(d3_rx1_, __LINE__): mov al, [esi]                 __asm lea ebx, d3_RadixCount[eax*4]                             __asm inc dword ptr [ebx]                                       __asm add esi, 8                                                __asm dec ecx                                                   __asm jne D3E_LBL(d3_rx1_, __LINE__)                            __asm mov ecx, 0xff                                             __asm lea ebx, d3_RadixCount                                    __asm D3E_LBL(d3_rx2_, __LINE__): mov eax, [ebx]                __asm add [ebx+4], eax                                          __asm add ebx, 4                                                __asm dec ecx                                                   __asm jne D3E_LBL(d3_rx2_, __LINE__)                            __asm mov ecx, n                                                __asm dec ecx                                                   __asm mov esi, d3_RadixSrc                                      __asm mov edx, esi                                              __asm add esi, edi                                              __asm mov edi, d3_RadixDst                                      __asm xor eax, eax                                              __asm D3E_LBL(d3_rx3_, __LINE__): mov al, [esi+ecx*8]           __asm lea ebx, d3_RadixCount[eax*4]                             __asm dec dword ptr [ebx]                                       __asm mov ebx, [ebx]                                            __asm mov eax, [edx+ecx*8]                                      __asm mov [edi+ebx*8], eax                                      __asm mov eax, [edx+ecx*8+4]                                    __asm mov [edi+ebx*8+4], eax                                    __asm xor eax, eax                                              __asm dec ecx                                                   __asm jns D3E_LBL(d3_rx3_, __LINE__)                        }

extern int d3_Unk633494;                /* 0x633494, objects tested / drawn */
extern int d3_Unk633498;                /* 0x633498 */
extern unsigned int d3_LightKeysCulled;     /* 0x633b08, light keys this frame spent on culled lights */
extern unsigned int d3_LightKeysVisible;    /* 0x633b0c, light keys this frame spent on visible lights */
extern unsigned int d3_LightGeneration;     /* 0x633b10, lightmap generation, D3MESH::lightGeneration holds the one drawn */
extern unsigned int d3_LightKeyBudgetCulled;  /* 0x5fcc6c, per-frame key budget for culled lights */
extern unsigned int d3_LightKeyBudgetVisible; /* 0x5fcc70, per-frame key budget for visible lights */
extern int d3s_Count;              /* 0x633dc0, d3s.c */
#ifndef DATA_VIEWS_H
extern int d3_SortMax;                      /* 0xfe73f8 */
#endif
extern char d3_ClipInfo[64][0x68];          /* 0xfc3020 */
void d3s_UpdateObject(D3OBJECT *obj);       /* 0x45ca20, d3s.c */
void d3s_UpdateObjectAt(D3OBJECT *obj);     /* 0x45ca60, d3s.c */
struct D3OBJECT; struct D3LINFO; struct D3MATRIX; void d3_TransformVerts(struct D3OBJECT *obj, struct D3LINFO *info, struct D3MATRIX *m);                             /* 0x459300 */
void d3_MirBeginFrame(D3OBJECT *obj, struct D3MIRROR *m);                                   /* 0x4554b0, d3_mir.h (d3_engine.c calls it through its D3OBJECT ** view d3_MirBeginFrameE) */
void d3_ProjectMesh(D3OBJECT *obj);                                                     /* 0x438f60 */

/* float to int with fistp (current rounding mode) */
__inline void d3_FtoI(int *dst, float f)
{
    __asm fld f
    __asm mov edx, dst
    __asm fistp dword ptr [edx]
}

/* Inline-asm copy with size, src and dst through temps, declared in this order (as D3_COPY3 in d3_d3d.h). */
#define D3E_COPY3(dst, src, size) {                                 int _cn = (size); void *_cs = (src); void *_cd = (dst);         __asm mov edi, _cd                                              __asm mov esi, _cs                                              __asm mov ebx, _cn                                              __asm lea ecx, [edi+edi*2]                                      __asm and ecx, 3                                                __asm sub ebx, ecx                                              __asm jle D3_COPY_LABEL(__LINE__)                               __asm rep movsb                                                 __asm mov ecx, ebx                                              __asm and ebx, 3                                                __asm shr ecx, 2                                                __asm rep movsd                                                 __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     __asm rep movsb                                             }

/* ---- round 3 (worker d3_left) ---- */
typedef struct D3CLIPINFO {         /* 0x68 bytes, d3_ClipInfo[clip code]: the planes a triangle must be clipped at */
    unsigned int n;                 /* number of planes */
    unsigned char mirror;           /* 0x04, nonzero: the planes are the mirror outline's (d3_MirCreatePlanes):
                                       always run the software clipper, with the winding test flipped */
    char pad05[0x3];
    D3VECTOR4 plane[6];             /* 0x08, normal and distance */
} D3CLIPINFO;
typedef struct D3UV { float u, v; } D3UV;
typedef struct D3CVERT {            /* D3PVERT as the polygon clipper and the draw loops see it */
    float x, y, z;                  /* camera space */
    float u;                        /* 0x0c, texture coordinates while clipping */
    float sx, sy, sz;               /* 0x10, screen position */
    float rhw;                      /* 0x1c */
    union {
        struct { float tu, tv; };   /* 0x20, texture coordinates */
        D3UV uv;                    /* the same two floats as a texture coordinate */
    };
    float pad28;
    float v;                        /* 0x2c */
    char pad30[0x10];
    unsigned char col[4];           /* 0x40 */
    unsigned long specular;         /* 0x44 */
    void *src;                      /* 0x48 */
    signed char flags;              /* 0x4c, clip code, bit 7 = visible */
    unsigned char unk4d;
    char pad4e[0x1];
    unsigned char fog;              /* 0x4f */
} D3CVERT;
extern unsigned int d3_ClipOut;            /* 0x633454, vertices in the clipped polygon */
extern unsigned int d3_ClipIn;              /* 0x633458, vertices in the polygon to clip */
extern D3CVERT *d3_ClipList[2][0x80];       /* 0xfc4c60, polygon vertex lists (ping-pong) */
extern D3CVERT d3_ClipPool[];               /* 0xfc5360, vertices made by the clipper */
D3CVERT **d3_ClipPoly(D3CLIPINFO *info);                                               /* 0x439c90 */
#define D3_CV(p) ((D3CVERT *)(p))
#define D3_TEXB(t, off) (((unsigned char *)(t))[off])  /* D3TEXTURE bytes: 0x68 bit 0 wrap, 0x6c alpha, 0x6e bit 0 */
typedef void (*D3TRIFUNC)(D3PTRI *t);
extern D3TRIFUNC d3_TriFuncs[];             /* 0xfe75e0, software triangle fillers by mode */
extern int d3_TextureSwitches;                    /* 0xfe75b4 */
char dd_LockNoWait(void);                   /* 0x41df80, dd_vesa.c */
void dd_Unlock(void);                       /* 0x41e050, dd_vesa.c */
extern D3VECTOR4 d3_Frustum[5];             /* 0xfc2960, corner directions of the view, then (0, 0, 1) */
#ifndef DATA_VIEWS_H
extern float d3_FovX, d3_FovY;              /* 0xfc4a20, 0xfc4c4c, half view angles */
#endif
typedef struct D3INITCFG {          /* 0x60 bytes, d3_Init's configuration (copied to d3_Cfg); main_OpenScreen 0x531c50 fills it on the stack with dword stores */
    unsigned int w, h;              /* screen size */
    int bpp;                        /* 0x08 */
    int mode;                       /* 0x0c, dd mode, 0: none */
    int d3d;                        /* 0x10, use Direct3D */
    int triple;                     /* 0x14, dd_SetMode's last argument (d3_Init reads its low byte) */
    int maxpolys;                   /* 0x18, sort buffer entries (d3_SortMax) */
    int texcount;                   /* 0x1c */
    int maxhicol;                   /* 0x20 */
    int texParam;                   /* 0x24, tex_Init's 3rd argument */
    int zbpp;                       /* 0x28 */
    int texbpp;                     /* 0x2c */
    int flFlag;                     /* 0x30, d3fl_Init's 4th argument (read as a byte) */
    int flShift;                    /* 0x34, d3fl_Init's 2nd argument is 0x40 >> flShift */
    int flArg;                      /* 0x38, d3fl_Init's 3rd argument (read as a byte) */
    int texShift;                   /* 0x3c, d3_MipSkip (read as a byte) */
    int shadowSize;                 /* 0x40, d3_InitShadows' first argument */
    float viewW, viewH;             /* 0x44 */
    float viewX, viewY;             /* 0x4c */
    float zn, zf;                   /* 0x54 */
    float scale;                    /* 0x5c */
} D3INITCFG;
extern D3INITCFG d3_Cfg;                    /* 0xfe73e0 */
char d3_Init(D3INITCFG *cfg, HWND wnd, int unk);  /* 0x43bc20, starts the engine (unk: the instance, handed to dd_Init / d3d_Init / dd_SetMode) */
void d3_Shutdown(void);                     /* 0x43c240, stops the engine */
/* 0xfe73f8 / 0xfe7400 lie inside D3INITCFG (.maxpolys / .maxhicol), but they stay separate definitions:
   spelling them as struct fields costs d3_MirDrawObject's and d3sm_GetGround's byte grade (the load
   schedule changes).  The two ledger overlaps are the price; the macro form is in the note. */
extern int d3_ClipNone;                    /* 0xfc5060 */
extern unsigned char d3_Unkfc5064, d3_Unkfc5065;  /* 0xfc5064, 0xfc5065 */
extern D3OBJECT d3_TopHead;               /* 0xfc5100, list start marker */
#ifndef DATA_VIEWS_H
extern int d3_Unkfc3000, d3_Unkfe7400;  /* 0xfc3000, 0xfe7400 */
#endif
extern char d3_Unkfc2e00[];                 /* 0xfc2e00, the head sentinel; its next field at +0x1fc is d3_StockListHead */
extern char d3_DefaultD3DConfig[];          /* 0x5fb5b4 */
extern unsigned char dd_BBits, dd_GBits, dd_RBits;  /* 0x1464fbd, 0x1463580, 0x1463570 */
extern float d3_Unkfe7600;              /* 0xfe7600 */
extern float d3_Unkfe75bc;              /* 0xfe75bc */
extern unsigned char d3_Noise[256][256];    /* 0xfc7b80 */
void d3io_NoopCallback(void *t);               /* 0x44ee40 */
char dd_Init(int mode, int bpp, HWND parent, int unk);                                  /* 0x41cfb0, dd_vesa.c */
void dd_SetClipRect(int left, int top, int right, int bottom);                          /* 0x41d190, dd_vesa.c */
char dd_SetMode(int xres, int yres, int bpp, int mode, HWND wnd, int unk6, char triple); /* 0x41d2e0, dd_vesa.c */
char d3d_Init(int bitplanes, int a, int b);                                             /* 0x435d70, d3_d3d.c */
char d3d_CreateDevice(DWORD texbpp, DWORD fourcc, DWORD texbpp2, DWORD zbpp);           /* 0x436160, d3_d3d.c */
struct D3DCONFIG; void d3d_SetConfig(struct D3DCONFIG *c);                                                            /* 0x436560, d3_d3d.c */
void d3d_SetWBuffer(char w);                                                            /* 0x437060, d3_d3d.c */
void tex_Init(int count, int maxhicol, int mips, unsigned int cache);                   /* 0x45fa80, ts_texture.c */
void d3sky_InitColors(void);                                                            /* 0x45dae0, d3_sky.c */
void d3fl_Init(unsigned int n, unsigned int detail, char mips, char b);                 /* 0x43fe00, d3_fl.c */
void d3_InitShadows(unsigned int size, unsigned int maxsize, int ncache, int age, char mips, int polys, int mapsize);                     /* 0x45ad80 */
void par_Init(void);                                                                    /* 0x455630, d3_par.c */
typedef struct D3TEXV {             /* D3PTRI::tex as the draw loop sees it */
    char pad00[0x64];
    struct IDirect3DTexture2 *d3dtex;   /* 0x64 */
    unsigned char flags;            /* 0x68, bit 0 wrap, bit 3 compressed (size / 8) */
    unsigned char uofs, vofs;       /* 0x69, 0x6a: low 5 bits index d3_UOfs/d3_VOfs */
    char pad6b[0x1];
    unsigned char alpha;            /* 0x6c */
    char pad6d[0x1];
    unsigned char unk6e;
    char pad6f[0x5];
    int w;                          /* 0x74 */
    char pad78[0x4];
    unsigned char bytes;            /* 0x7c, bytes per pixel */
} D3TEXV;
typedef struct D3TLV {              /* D3DTLVERTEX as filled by the draw macros */
    float sx, sy, sz, rhw;
    unsigned long color, specular;
    float tu, tv;
} D3TLV;
extern int d3_Unkfc2dc0;                    /* 0xfc2dc0, texture bytes used this frame */
extern float d3_ZScale;                     /* 0xfc532c */
extern float d3_ZOfs;                       /* 0xfc7b64 */
extern float d3_UOfs, d3_VOfs;              /* 0xfe73ac, 0xfc26ec */
extern float ts_ScrollV[32], ts_ScrollU[32];    /* 0xfb9120, 0xfb90a0 */
/* 0x63345d: the first byte of d3_NoTextures (d3.h), read as a byte */
#define d3_Unk63345d (*(unsigned char *)((char *)&d3_Active + 1))  /* 0x63345d */
int d3d_GetVertexBuffer(void **buf, int *n);                                            /* 0x4364f0, d3_d3d.c */
void d3d_DrawTriangles(void *unk, struct _D3DTLVERTEX *buf, DWORD num);                                /* 0x436510, d3_d3d.c */
/* Inline asm: one transformed vertex from a D3PVERT and a texture coordinate pair (u, v at uv). */
#define D3E_TLV(dst, src, uv) {                                     \
    void *_td = (dst); void *_ts = (src); void *_tu = (uv);         \
    __asm mov edi, _td                                              \
    __asm mov esi, _ts                                              \
    __asm mov edx, _tu                                              \
    __asm mov eax, [esi+0x10]                                       \
    __asm mov [edi], eax                                            \
    __asm mov eax, [esi+0x14]                                       \
    __asm mov [edi+0x4], eax                                        \
    __asm mov eax, [esi+0x1c]                                       \
    __asm mov [edi+0xc], eax                                        \
    __asm mov eax, [esi+0x40]                                       \
    __asm mov [edi+0x10], eax                                       \
    __asm mov eax, [esi+0x4c]                                       \
    __asm mov [edi+0x14], eax                                       \
    __asm fld dword ptr [esi+0x8]                                   \
    __asm fadd d3_ZOfs                                              \
    __asm fmul d3_ZScale                                            \
    __asm fstp dword ptr [edi+0x8]                                  \
    __asm fld dword ptr [edx]                                       \
    __asm fadd d3_UOfs                                              \
    __asm fstp dword ptr [edi+0x18]                                 \
    __asm fld dword ptr [edx+0x4]                                   \
    __asm fadd d3_VOfs                                              \
    __asm fstp dword ptr [edi+0x1c]                                 \
}
/* Same for a clipped vertex: rhw = 1 / z, u and v through pointers. */
#define D3E_TLV2(dst, src, pu, pv) {                                \
    void *_td = (dst); void *_ts = (src); void *_tu = (pu); void *_tv = (pv); \
    __asm mov edi, _td                                              \
    __asm mov esi, _ts                                              \
    __asm mov ecx, _tu                                              \
    __asm mov edx, _tv                                              \
    __asm fld1                                                      \
    __asm fdiv dword ptr [esi+0x8]                                  \
    __asm mov eax, [ecx]                                            \
    __asm mov [edi+0x18], eax                                       \
    __asm mov eax, [edx]                                            \
    __asm mov [edi+0x1c], eax                                       \
    __asm mov eax, [esi+0x10]                                       \
    __asm mov [edi], eax                                            \
    __asm mov eax, [esi+0x14]                                       \
    __asm mov [edi+0x4], eax                                        \
    __asm mov eax, [esi+0x40]                                       \
    __asm mov [edi+0x10], eax                                       \
    __asm mov eax, [esi+0x4c]                                       \
    __asm mov [edi+0x14], eax                                       \
    __asm fstp dword ptr [edi+0xc]                                  \
    __asm fld dword ptr [esi+0x8]                                   \
    __asm fadd d3_ZOfs                                              \
    __asm fmul d3_ZScale                                            \
    __asm fstp dword ptr [edi+0x8]                                  \
}
D3OBJECT *d3_SpawnObject(int type, char *name);                                         /* 0x43c570 */
void d3_AllocObjectPolys(D3OBJINFO *info);                                              /* 0x43cc10 */
void d3_AddObjectDrawData(D3OBJECT *obj);                                               /* 0x43cfb0 */
/* Inline-asm copy with both operands named directly in the asm (D3_COPY_AT in d3_d3d.h). */
#define D3E_COPY_AT(dvar, svar, size) {                             \
    int _cn = (size);                                               \
    __asm mov edi, dvar                                             \
    __asm mov esi, svar                                             \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_COPY_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

/* ---- round 3, worker d3_engine2 (0x43e090..0x43eab0) ---- */
extern int d3_Busy;                         /* 0x633b04, nesting guard of d3_Render/d3_Animate */
extern unsigned char d3_FogByDistance;          /* 0x6334a5 */
extern int d3_MirrorObject;                    /* 0x63344c, the object the frame's mirror was built for (0 none) */
#ifndef DATA_VIEWS_H
extern unsigned char d3_Unkfb9928;          /* 0xfb9928 */
#endif
extern int d3_TextureSwitches;                /* 0xfe75b4 */
extern int d3_Unkfc2dc0;                /* 0xfc2dc0 */
extern int d3_Unk633470;                /* 0x633470, frame statistics */
extern int d3_Unk633474;                /* 0x633474 */
extern int d3_Unk633478;                /* 0x633478 */
extern int d3_Unk63347c;                /* 0x63347c */
extern int d3_Unk633480;                /* 0x633480 */
extern int d3_Unk633484;                /* 0x633484 */
extern int d3_Unk633488;                /* 0x633488 */
extern unsigned int d3_Fps60Time;       /* 0x6334a8 */
extern unsigned int d3_Fps60Frames;       /* 0x6334ac */
extern unsigned int d3_Fps10Time;       /* 0x6334b0 */
extern unsigned int d3_Fps10Frames;       /* 0x6334b4 */
extern unsigned int d3_Fps60;       /* 0x63348c, frames per second (two windows) */
extern unsigned int d3_Fps10;       /* 0x633490 */
extern unsigned int tb_Delay;           /* 0xfe7a88, tb.c (the script clock factor) */
void d3d_Clear(D3RECT *r);                                                              /* 0x435fb0, d3_d3d.c */
void dd_Clear(RECT *rect, int r, int g, int b);                                         /* 0x41e100, dd_vesa.c (d3_engine.c / d3_interface.c call it through dd_ClearB) */
float d3fl_FarDist(struct D3FLMAP *p, D3VECTOR *pos, D3MATRIX *m);                      /* 0x446990 */
void d3fl_Draw(struct D3FLMAP *map, char water);                                      /* 0x4439e0 */
void d3s_CollectLights(void);                                                           /* 0x45c9f0, d3s.c */
char d3_DrawObjectCB(D3OBJECT *obj);                                                    /* 0x43a160 */
char d3_MirDrawObject(D3OBJECT *obj, struct D3MIRROR *m);                                   /* 0x4550e0, d3_mir.h, a d3_ForEachObject callback */
void par_Draw(D3PARTSYS *ps);                                                       /* 0x455a90 */
struct D3SKY; char d3sky_Draw(struct D3SKY *s);                                                             /* 0x45d500 */
void d3_DrawSorted(void);                                                                /* 0x43b600 */
void ts_UpdateScroll(unsigned long now);                                                   /* 0x460cb0 */
int ai_AlwaysTrue(void);                                                                /* 0x4a8c10 */

/* event tables ("d3e:") */
/* D3E_NEVENTS, D3EVHANDLER, D3EVENT: d3_types.h */
typedef struct D3EVNAME {           /* 0x24 bytes */
    char name[0x20];
    int id;
} D3EVNAME;
typedef D3EVHANDLER (*D3EVRESOLVE)(void *obj, int id, char *name);
extern D3EVNAME d3e_Events[8];              /* 0x5fb800 */
extern D3EVRESOLVE d3e_Resolve;             /* 0x5fb920 */
void d3e_SaveEvents(void *f, D3EVENT *t);                                                       /* 0x43e900 */
void bio_rd_s(struct VFSFILE *f, char *s);      /* 0x4011a0 */
void bio_rd_i(struct VFSFILE *f, int *i);         /* 0x4011d0 */
void bio_wr_s(struct VFSFILE *f, char *s);       /* 0x401210 */
void bio_wr_i(struct VFSFILE *f, int i);         /* 0x401240 */

/* declared in other units' headers before (moved to the owner) */
void d3_Unlink(D3OBJECT *obj);  /* 0x43d0e0 */
extern int d3if_Unk5fb5c4;  /* 0x5fb5c4 */
char d3_GetBounds(D3OBJECT *obj, D3OBJECT *camera, D3VECTOR *center, float *radius);  /* 0x43d860 */
D3OBJECT *d3_NextGroup(D3OBJECT *obj);  /* 0x438c30 */
char d3_ForEachObject2(D3SCENE *scene, D3OBJECT *obj, unsigned int mask, void (*cb)(D3OBJECT *obj));  /* 0x438e60 */
void d3e_LoadSceneEvents(void *f);  /* 0x43ea10 */
void d3_ClipMesh(D3CLIP mask, D3PVERT *v, int nverts, D3PTRI *t, int ntris);  /* 0x439a60 */
void d3_AppendSibling(D3OBJECT *first, D3OBJECT *obj);  /* 0x43cb20 */
void d3_SetAlpha(D3OBJECT *obj, D3TEXFLAGS *f);  /* 0x43d690, the alpha flags f on obj and its group */
char d3_AttachToBone(D3OBJECT *obj, char *name);  /* 0x43d6c0 */
char d3_DeflateScene(unsigned int flags);  /* 0x43db30 */
char d3_InflateScene(unsigned int flags);  /* 0x43deb0 */
void d3_SetObject078(D3OBJECT *obj, D3VECTOR *v);  /* 0x43b820 */

#endif
