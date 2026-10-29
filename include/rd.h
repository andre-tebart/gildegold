struct PATHOUT;
struct PMAP;
struct D3SCENE;
struct D3TERRAIN;
struct D3SM;
struct XOBJ;
/* Declarations of rd.c (0x4bc2e0..): places for animals (door dummies, free cells, dummy lookup), the
   vegetation map, roads. Names are ours. */
#ifndef RD_H
#define RD_H

typedef struct G15FIND {                /* 0xa4 bytes, context of rd_CollectNamed */
    char name[0x20];                    /* 0x00 */
    char *objs[0x20];                   /* 0x20, the objects named so */
    int n;                              /* 0xa0 */
} G15FIND;

typedef struct G15TREE {                /* 0x46 bytes, the vegetation types veg_Types ("TANNE", "BUCHE", "EICHE", ..; id 0 ends) */
    unsigned short id;                  /* 0x00 */
    char name[0x40];                    /* 0x02 */
    unsigned char size;                /* 0x42, copied to G15PLANT.size (its size in cells) */
    unsigned char states;                /* 0x43, growth states per model */
    unsigned char lastState;                /* 0x44, the last growth state */
    char pad45;
} G15TREE;

typedef struct G15PLANT {               /* 0x18 bytes, 64 per vegetation map */
    int id;                             /* 0x00, from veg_NextId */
    int unk04;                          /* 0x04 */
    unsigned char x;                    /* 0x08, cell */
    unsigned char z;                    /* 0x09 */
    unsigned short type;                /* 0x0a, G15TREE.id; 0 removes it */
    char size;                         /* 0x0c, size in cells */
    char state;                         /* 0x0d, growth state; -1: free */
    char pad0e[2];
    int unk10;                          /* 0x10 */
    char *obj;                          /* 0x14, its 3D object */
} G15PLANT;

typedef struct G15MAP {                 /* the town map ai_MapRefs (am.h: AIMAPREF[]) */
    int w;                              /* 0x00, cells per row */
    char pad04[0x10];
    unsigned char *cells;               /* 0x14, one byte per cell (the ground type) */
    char pad18[0x78];
    float x0;                           /* 0x90 */
    char pad94[4];
    float z0;                           /* 0x98 */
    char pad9c[4];
    float cw;                           /* 0xa0, cell width */
    char pada4[0x14];
    float ch;                           /* 0xb8, cell depth */
    char padbc[0x19a8];
    char ground[8][0x40];               /* 0x1a64, the ground type names ("EINFACHER_WEG", "PRUNK_WEG", ..) */
} G15MAP;

typedef struct G15OBJ {                 /* a 3D object (d3.h: D3OBJECT) as rd reads it */
    char pad00[0x224];
    int mask;                           /* 0x224, the object's mask bits */
} G15OBJ;

typedef struct G15SCENE {               /* a scene of d3_Scenes (d3.h: D3SCENE) as rd reads it */
    char pad00[0xb0];
    struct G15GRID *hmap;               /* 0xb0, the terrain height map (d3.h: struct D3HMAP *) */
} G15SCENE;

typedef struct G15GRID {                /* d3sm_Create's grid (D3HMAP, d3_sm.c) */
    char pad00[0x20];
    int size;                           /* 0x20 */
    int *cells;                         /* 0x24, 0x1c bytes per cell, the first int its type (6 / 0xb: road) */
} G15GRID;

#pragma pack(push, 1)                   /* the fields sit at unaligned offsets (ai.h packs AIBLD too) */
typedef struct G15BLD {                 /* 0xa9 bytes, a building of ai_Blds[] (ai.h: AIBLD) */
    char pad00[0x61];
    char *object;                       /* 0x61, its 3D object in the scene */
    char pad65[0x44];
} G15BLD;
#pragma pack(pop)

typedef struct G15ROAD {                /* 0x1020 bytes, rd_Road */
    G15MAP *map;                        /* 0x00 */
    G15GRID *grid;                      /* 0x04 */
    int simple;                         /* 0x08, the ground type "EINFACHER_WEG" (2 if none) */
    int prunk;                          /* 0x0c, the ground type "PRUNK_WEG" (1 if none) */
    unsigned char path[0x400][2];       /* 0x10, path_Find's cells */
    int n2;                             /* 0x810 */
    unsigned char path2[0x400][2];      /* 0x814, the road so far */
    int n;                              /* 0x1014, path_Find's out: count, max, cells */
    int max;                            /* 0x1018 */
    void *pts;                          /* 0x101c */
} G15ROAD;

typedef struct G15BK {                  /* 0x60 bytes, a building site (ai_Bauplaetze[ai_NBauplaetze]) */
    char name[0x40];                    /* 0x00 */
    float x0, y0, z0;                   /* 0x40 */
    char pad4c[4];
    float x1, y1, z1;                   /* 0x50 */
    char pad5c[4];
} G15BK;

typedef struct G15BKFIND {              /* context of rd_FindSiteCB */
    char name[0x40];                    /* 0x00 */
    float pos[4];                       /* 0x40 */
    char *found;                        /* 0x50 */
} G15BKFIND;

#ifndef DATA_VIEWS_H
extern G15MAP *ai_MapRefs;            /* 0xfd7c4c, am.h: AIMAPREF[] */
#endif
extern G15ROAD rd_Road;            /* 0xf81300 */
extern int ai_NBauplaetze;                /* 0xf812e0, the number of building sites */
extern G15BK *ai_Bauplaetze;             /* 0xf812e4 */
extern int ai_MapSize;                /* 0x6f4ee8 */
#ifndef DATA_VIEWS_H
extern unsigned char ai_Unkfd7f78;      /* 0xfd7f78 */
#endif

extern G15TREE veg_Types[];          /* 0x602860 */
extern int veg_NextId;                /* 0x6f0748, the next plant id */
extern char ai_DataPath[];             /* 0x75dca0, the data path */
extern void *d3_Terrain;              /* 0x633738, a height map with a transform at 0x90 (as d3.h / f3snd.h) */

/* views of other units */
struct D3SM * d3sm_Create(struct D3TERRAIN *t, int size, int mask);                         /* 0x45eeb0, d3_sm.h: D3HMAP * */
void d3sm_Destroy(struct D3SM *h);                                                   /* 0x45ef60 */
void d3_LocalToWorld(struct XOBJ *obj, float *in, float *out);                             /* 0x458c10 */
int ai_BuildingName(int name, char *out);                                           /* 0x58f640, am.h */
int ai_FindBauplaetze(char *name, int *out);                                          /* 0x58f7a0, am.h */
unsigned short path_Find(struct PATHOUT *out, struct PMAP *map, int sx, int sy, int ex, int ey, int type); /* 0x424f30, path.h: PATHOUT *, PMAP * */
void d3fl_MakeDivide(G15MAP *m);                                                 /* 0x441300, d3_fl.c */
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);                                          /* 0x447190, d3_interface.h */
char d3if_ShowTree(struct D3OBJECT *obj, char show);                                        /* 0x447300, d3_interface.h */
struct D3OBJECT * d3_LoadGroup(char *name, void *f, struct D3OBJECT *parent);                     /* 0x44c3f0, d3_int_io.h: D3OBJECT * */
void err_Message(char *msg);                                                     /* 0x4222b0 */
void d3_SetObjectPos(struct D3OBJECT *obj, struct D3VECTOR *pos);                                     /* 0x43b790, d3.h: D3OBJECT *, D3VECTOR * */
struct D3OBJECT * d3_FindObject(int a, int flags, int c, char *name);                /* 0x449690, d3.h: int a, D3OBJECT * */
char d3sm_GetVertex(struct D3SM *h, int x, int z, struct D3VECTOR *out);                          /* 0x45f100, ch.h: D3SM *, D3VECTOR * */
char ch_FindFreeCell(struct D3SM *h, struct D3VECTOR *from, struct D3VECTOR *to, int x, int z, int *px, int *pz, int max, int flags); /* 0x4072a0, ch.h */
char d3_ForEachObject(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, char (*cb)(struct D3OBJECT *, void *), void *ctx); /* 0x438d40, d3.h: D3OBJECTCB */

int rd_DoorPos(float *pos);
int rd_PointsNear(float *pos, int n, float (*pts)[4]);
int rd_CollectNamed(char *obj, G15FIND *f);
char *rd_RandomNamedObject(char *name);
G15TREE *veg_FindType(int type);
G15PLANT *veg_FindPlantAt(G15PLANT *p, int x, int z);
G15PLANT *veg_FindPlantObject(G15PLANT *p, char *obj);
G15PLANT *veg_FindPlantId(G15PLANT *p, int id);
G15PLANT *veg_FindPlantArea(G15PLANT *p, int x, int z);
int veg_Fits(G15PLANT *p, int x, int z, int size);
G15PLANT *veg_SetPlant(G15PLANT *p, int x, int z, int type, int fall);
void veg_GrowPlants(G15PLANT *p);
void veg_LoadModel(G15PLANT *q, int fall);
void veg_ShowPlants(G15PLANT *p);
void veg_HidePlants(G15PLANT *p);
int rd_BuildRoad(char *obj, int name);
int rd_PathToRoad(G15ROAD *r, int x, int z, int tx, int tz);
void rd_AddBauplaetze(G15ROAD *r);
void rd_AllBauplatzToSupermap(G15ROAD *r, char *name);
char rd_FindSiteCB(char *obj, G15BKFIND *f);
G15BK *rd_GetBKSize(char *name);
void rd_WorldToCell(float *x, float *z, float *v, char *obj, G15ROAD *r);
void rd_FillQuad(G15ROAD *r, float *c, char v, int t);

#endif
