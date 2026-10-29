struct D3SM;
struct D3TERRAIN;
struct D3OBJECT;
struct VFSFILE;
/* d3_interface.c types and globals: object creation / deletion API of the d3 engine and the scene switch.
   __FILE__ string "D:\develop\Units\d3\d3_interface.c" at 0x5fbd94. Names are ours. */
#ifndef D3_INTERFACE_H
#define D3_INTERFACE_H

/* one of the 6 groups of three dwords (fog colour and two more) kept per scene and as globals */
typedef struct D3IFTRIPLE {
    unsigned long a, b, c;
} D3IFTRIPLE;

typedef struct D3IFLEVEL {          /* 0x60 bytes, 7 of them */
    D3VECTOR amb;                   /* 0x00, from 0x5fcc78 (ambient colour) */
    D3VECTOR dir;                   /* 0x0c, from 0x633b1c */
    D3IFTRIPLE fog[6];              /* 0x18 */
} D3IFLEVEL;

typedef struct D3IFSCENE {          /* 0x3e0 bytes: the per-scene copy of the engine's global state (d3_Scenes) */
    char pad000[0x80];
    D3OBJECT *first;                /* 0x80, d3_TopFirst */
    D3OBJECT *last;                 /* 0x84, d3_TopLast */
    D3OBJECT *cam;                  /* 0x88, d3if_MegaCam */
    D3VECTOR campos;                /* 0x8c, cam->pos */
    D3VECTOR camrot;                /* 0x98, cam->rotation */
    void *par1;                     /* 0xa4, d3_ParHead */
    void *par2;                     /* 0xa8, d3_ParTail */
    void *map;                      /* 0xac, d3_Terrain */
    void *hmap;                     /* 0xb0, the scene's D3HMAP (D3SCENE.hmap) */
    int mapSize;                    /* 0xb4, height map size: 0x80 with terrain, 0x40 without, from the file */
    void *sky;                      /* 0xb8, d3_Sky */
    D3VECTOR amb;                   /* 0xbc, 0x5fcc78.. */
    D3VECTOR dir;                   /* 0xc8, 0x633b1c.. */
    D3IFTRIPLE fog;                 /* 0xd4, 0x6334a0, 0xfc29bc, 0xfc5340 */
    D3IFTRIPLE fogs[6];             /* 0xe0, 0xfba960 */
    D3IFLEVEL levels[7];            /* 0x128, 0xfba9c0 */
    void *events;                   /* 0x3c8, the scene's event table (D3EVENT, "d3e:RegisterSceneEvent(eventtable)") */
    int unk3cc, unk3d0;             /* 0x5fb5c0, 0x5fb5c4 */
    char loaded, dirty;             /* 0x3d4 / 0x3d5, copied from D3SCENE */
    char pad3d6[0x2];
    unsigned int sceneFlags;        /* 0x3d8, d3_SceneFlags */
    unsigned char hardware;         /* 0x3dc, d3_Hardware */
    char pad3dd[0x3];
} D3IFSCENE;
#define D3IF_SCENE(i) (((D3IFSCENE *)d3_Scenes)[i])

extern float d3if_Gray;             /* 0x5fcc74, 0.3 r + 0.59 g + 0.11 b of the ambient colour */
extern float d3if_AmbR;                 /* 0x5fcc78 */
extern float d3if_AmbG;                 /* 0x5fcc7c */
extern float d3if_AmbB;                 /* 0x5fcc80 */
extern float d3if_Dir[3];           /* 0x633b1c */
extern D3IFTRIPLE d3if_Fog[6];      /* 0xfba960 */
extern D3IFLEVEL d3if_Levels[7];    /* 0xfba9c0 */
extern int d3if_CamOben;              /* 0x6338b8 */
extern int d3if_CamVorne;              /* 0x6338bc */
extern int d3if_CamSeite;              /* 0x6338c0 */
#ifndef DATA_VIEWS_H
extern int d3if_Unk5fb5c0;              /* 0x5fb5c0 */
#endif
extern int d3if_Unk5fb5c4;              /* 0x5fb5c4 */

extern int d3if_Unk633dcc;          /* 0x633dcc */
extern unsigned char d3if_Unk6339db;    /* 0x6339db */
#ifndef DATA_VIEWS_H
extern unsigned int d3if_Unk6339dc;     /* 0x6339dc, counts the scene switches */
#endif
#ifndef DATA_VIEWS_H
extern int d3if_Unkfba944;          /* 0xfba944 */
#endif

void d3_AddChild(D3OBJECT *parent, D3OBJECT *obj);                                      /* 0x43cb80, d3_engine.c */
D3OBJECT *d3_NextGroup(D3OBJECT *obj);                                                  /* 0x438c30, d3_engine.c */
void d3_Unlink(D3OBJECT *obj);                                                          /* 0x43d0e0, d3_engine.c */
char d3_ForEachObject2(D3SCENE *scene, D3OBJECT *obj, unsigned int mask, void (*cb)(D3OBJECT *obj)); /* 0x438e60 */
int d3io_FindStock(char *name);                                                           /* 0x44c5d0 */
void d3if_CreateCameras(void);                                                                /* 0x448910 */
void d3_SetFog(float start, float end, unsigned long color);                    /* 0x43a6f0 = d3_SetFog, the floats passed as dwords */  /* @original: d3_interface.c passes the D3FOGCOL struct (byte-matched call); the other units pass the dword */
struct D3SM * d3sm_Create(struct D3TERRAIN *t, int size, int mask);                                /* 0x45eeb0 */
void d3sm_Destroy(struct D3SM *h);                                                          /* 0x45ef60 */
void tex_GetParams(void *tex, void *opts, void *fmt);                                   /* 0x460aa0, ts_texture.c */
void d3s_ResetShadows(D3OBJECT *obj);                                                   /* 0x45ce00, d3s.c */

char d3if_Delete(D3OBJECT *obj);                                                        /* 0x447190 */
char d3if_Show(struct D3OBJECT *obj, char show);                                               /* 0x4471b0 */
char d3if_ShowTree(D3OBJECT *obj, char show);                                           /* 0x447300 */


extern char d3if_ParList[];         /* 0xfb95e0 */
extern unsigned long d3if_Unkfc29bc;    /* 0xfc29bc, with d3_FogColor the fog triple */
extern unsigned long d3if_Unkfc5340;    /* 0xfc5340 */
typedef char d3if_chk_size[sizeof(D3IFSCENE) == 0x3e0 ? 1 : -1];
typedef char d3if_chk_levels[(int)&((D3IFSCENE *)0)->levels == 0x128 ? 1 : -1];
typedef char d3if_chk_sceneFlags[(int)&((D3IFSCENE *)0)->sceneFlags == 0x3d8 ? 1 : -1];

/* --- 0x447e60..0x448910 (worker d3if2) --- */
char d3_GetBounds(D3OBJECT *obj, D3OBJECT *camera, D3VECTOR *center, float *radius);   /* 0x43d860, d3_engine.c */
void d3_SetBlendState(unsigned char balpha, unsigned char glow, unsigned char colorkey);   /* 0x4387b0, d3_d3d.c (d3.h: int version) */
#define D3IF_OBJ068(o) ((float *)(o))[0x68 / 4]     /* size of a type-0 object on screen */
#define D3IF_RGB_MASK 0xffffff      /* the r/g/b bits of an ARGB colour (sprite vertex colour) */

typedef struct D3IFMOVE {           /* context of d3if_MoveCB */
    D3IFSCENE *from, *to;
} D3IFMOVE;

typedef struct D3IFSPRITE {         /* a D3TEXHANDLE as d3if_DrawSprite reads it */
    char pad00[0x40];
    int frames;                     /* 0x40 */
    char pad44[0x20];
    struct IDirect3DTexture2 *surftext; /* 0x64 */
    char pad68[0x4];
    unsigned char colorkey;         /* 0x6c, 0xff = none */
    char pad6d;
    unsigned char flags;            /* 0x6e, bit 0 alpha, bit 1 glow */
    char pad6f[0x5];
    unsigned int size;              /* 0x74 */
} D3IFSPRITE;

typedef union D3IFTEXFLAGS {        /* D3TEXFLAGS with the bits named */
    D3TEXFLAGS raw;                 /* what tex_Load takes by value */
    struct {
        unsigned char alpha;
        unsigned char b1;
        unsigned char f0 : 1;
        unsigned char glow : 1;
        unsigned char f2 : 6;
        unsigned char b3;
    } bits;
} D3IFTEXFLAGS;

typedef struct D3IFHIT {
    D3OBJECT *obj;
    float dist;
} D3IFHIT;

typedef struct D3IFPICK {           /* context of d3if_PickCB */
    D3IFHIT hits[9];                /* the nearest hit per object type */
    D3OBJECT *cam;                  /* 0x48 */
    int mask;                       /* 0x4c, tested against obj->mask */
    float dx, dy;                   /* 0x50, screen centre minus the point */
} D3IFPICK;

void d3_Frame(unsigned int mask, char flag, char tick);                               /* 0x43e680, d3_engine.c */
void d3if_ClearView(void);                                                         /* 0x448a50 */

/* 0x448ac0..0x44aa10 (d3_obj.h until layout 2026-10-22) */
typedef struct D3FINDCTX {          /* context of the name lookups */
    char *name;                     /* compared with the object's name (a leading '!' ignored), may be 0 */
    char *obj;                      /* when set, only this object matches (compared by address) */
    char *found;
} D3FINDCTX;

extern D3VECTOR d3_ZAxis;                   /* 0x5ebb74, (0, 0, 1) */
typedef struct D3CORNERPT { int x, y; } D3CORNERPT;
typedef struct D3CORNERCTX {        /* 0x88 bytes: context of d3_CornerCB */
    void *tex;                      /* 0x00, the texture whose polygons are examined */
    D3VECTOR4 v[4];                 /* 0x04, the 3D vertex nearest to each screen corner of the texture's area */
    float uv[4][2];                 /* 0x44, its texture coordinates */
    D3CORNERPT pt[4];               /* 0x64, its screen position: top left, top right, bottom left, bottom right */
    char found;                     /* 0x84 */
    char pad85[3];
} D3CORNERCTX;

typedef struct D3CORNERVERT {       /* a transformed vertex (D3VERTEX) with its screen position */
    float x, y, z;
    float pad0c;
    float sx, sy;                   /* 0x10 */
    char pad18[0x38];
} D3CORNERVERT;

typedef struct D3CORNERTRI {        /* D3TRI as the corner search reads it */
    D3CORNERVERT *v[3];
    char pad0c[0x4];
    float *uv;                      /* 0x10, (u, v) per corner */
    void *tex;                      /* 0x14 */
    char pad18[0xc];
    signed char flag24;             /* 0x24 */
    char pad25;
    unsigned char flag26;           /* 0x26 */
    char pad27;
} D3CORNERTRI;

void d3_MatTranspose(D3MATRIX *src, D3MATRIX *dst);                    /* 0x454040 */
void d3_MatInverse(D3MATRIX *src, D3MATRIX *dst);                    /* 0x454070 */
void d3_MatToAngles(D3VECTOR4 *m, D3VECTOR4 *angles);                 /* 0x454690, matrix rows to angles (d3_anim.h too) */
float d3_YawTo(D3OBJECT *obj, float *p);                          /* 0x448ac0 */
char d3_ScreenToUV(float sx, float sy, D3OBJECT *obj, void *tex, float *outu, float *outv);   /* 0x448e60 */
D3OBJECT *d3_FindObjectNoCase(int a, int flags, int c, char *name);  /* 0x449770, case-insensitive */
void d3_WorldToParent(D3OBJECT *obj, float *pos, D3MATRIX *rot, D3VECTOR *outpos, D3VECTOR *outrot);  /* 0x4497f0 */
void d3_SetWorldPosRot(D3OBJECT *obj, float *pos, D3MATRIX *rot);        /* 0x449920 */

typedef struct D3LIGHTKEY {         /* 0x38 bytes, D3LIGHTENTRY: one of the 7 day-time keys of a light */
    D3VECTOR pos;                   /* 0x00, relative to the light's parent */
    D3VECTOR rot;                   /* 0x0c */
    D3VECTOR color;                 /* 0x18, -> obj lightView color (0x5c) */
    float intensity;                /* 0x24, -> obj lightView power (0x94); 0 = off */
    float range;                    /* 0x28, -> obj lightView range (0x90) */
    float atten;                    /* 0x2c, -> obj lightView atten (0x98) */
    int unk30;
    int unk34;
} D3LIGHTKEY;

typedef struct D3LIGHTKEYS {        /* 0x1ac bytes, D3LIGHTINFO */
    D3LIGHTKEY k[7];
    char pad188[0xc];
    int unk194;
    int unk198;
    int unk19c;
    int unk1a0;
    float phase;                    /* 0x1a4, added to the day time */
    char pad1a8[0x4];
} D3LIGHTKEYS;
#define D3O_LIGHT(o) ((D3LIGHTKEYS *)D3_X(o)->light)

/* D3OBJECT as the light code of this unit reads the light fields (object types 5..8; the same bytes are
   the stored position / rotation of a camera dummy, types 0/2/3 with objectFlags bit 0). The names are
   d3_light.h's D3LOBJ view; a union in D3OBJECT itself (d3.h) is type debt. */
typedef struct D3IFLIGHT {
    char pad000[0x5c];
    D3VECTOR color;                 /* 0x5c, light colour (r, g, b) */
    char pad068[0x28];
    float range;                    /* 0x90 */
    float power;                    /* 0x94, 0 = off */
    float atten;                    /* 0x98 */
} D3IFLIGHT;
#define D3IF_LIGHT(o) ((D3IFLIGHT *)(o))

/* d3_WriteMap writes these float fields as their bit pattern (the typed views are local to this unit: a union
   in d3_fl.h's D3FLMAP / D3FLREGION, which d3_fl.c's byte functions use, is type debt). */
typedef union D3IFBITS { float f; int bits; } D3IFBITS;
typedef struct D3IFREGION {         /* D3FLREGION, 0x158 bytes */
    char pad000[0x0c];
    D3IFBITS scrollSpeedU;          /* 0x0c */
    D3IFBITS scrollSpeedV;          /* 0x10 */
    D3IFBITS f14;                   /* 0x14 */
    char pad018[0x140];
} D3IFREGION;
typedef struct D3IFMAP {            /* D3FLMAP, the fields the map writer serializes */
    char pad000[0xa0];
    D3IFBITS scale;                 /* 0xa0 */
    char pad0a4[0x20];
    D3IFBITS hscale;                /* 0xc4 */
} D3IFMAP;

typedef struct D3FOGCOL {
    unsigned char b, g, r, a;
} D3FOGCOL;

typedef struct D3FOGSET {           /* 12 bytes, D3IFTRIPLE seen as colour + range */
    D3FOGCOL col;
    float start, end;
} D3FOGSET;

typedef struct D3LIGHTSET {         /* 0x60 bytes, D3IFLEVEL */
    D3VECTOR amb;
    D3VECTOR dir;
    D3FOGSET fog[6];
} D3LIGHTSET;

extern D3FOGSET d3o_Fog[6];         /* 0xfba960 (d3if_Fog) */
extern D3LIGHTSET d3o_Sets[7];      /* 0xfba9c0 (d3if_Levels) */
extern D3VECTOR d3o_Amb;            /* 0x5fcc78 (d3if_AmbR..) */
extern D3VECTOR d3o_Dir;            /* 0x633b1c (d3if_Dir) */
extern int d3o_LightSet;            /* 0x5fbcfc, the current light set */
extern int d3o_FogSet;              /* 0x5fbd00, the current fog set */
extern float d3o_FogScale;          /* 0x5fbd04 */
extern D3FOGCOL d3o_SkyCol[7];      /* 0xfb91c0, sky colour per light set */

char d3_SetParent(D3OBJECT *obj, D3OBJECT *parent);                                    /* 0x449960 */
char d3_SetLightKey(D3OBJECT *obj, unsigned int key, float t);                          /* 0x449b20 */
char d3_ScaleLightCB(D3OBJECT *obj, float *scale);                                      /* 0x449d80 */
char d3_SetLightKeyCB(struct D3OBJECT *obj, float *time);                                      /* 0x449da0 */
char d3_SetLightSet(unsigned int set, float t, float scale, char calc);                 /* 0x449de0 */
char d3_SetFogSet(unsigned int a, unsigned int b, float t);                             /* 0x44a170 */
struct VFSFILE; struct D3FLMAP;        /* vfs.h / d3_fl.h: the tags, so the prototypes below do not declare new ones */
void d3_WriteObject(struct VFSFILE *f, struct D3OBJECT *obj);                                         /* 0x44a2c0 */
void d3_WriteMap(struct VFSFILE *f, struct D3FLMAP *map);                                                /* 0x44a770 */
int d3_SceneVersion(unsigned int version);                                              /* 0x44a9e0 */
void d3_SaveScene(char *name, struct VFSFILE *f);                                              /* 0x44aa10 */

#endif
