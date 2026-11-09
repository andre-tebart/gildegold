/* Declarations of d3_tools.c (D:\develop\Units\d3_tools\d3_tools.c, 0x411680..0x417650): object placement, bounds
   and heights, the camera paths, rain, snow, the particle emitters and the sun rays. Names are ours. */
#ifndef D3_TOOLS_H
#define D3_TOOLS_H
#include "d3.h"                       /* the D3 records and typedefs this header names */

struct D3OBJECT;
struct D3VECTOR;
struct D3SCENE;
struct D3TMAP;
struct D3HQUERY;
struct D3ANIM;
struct D3TRAIN;
struct D3TSNOW;
struct D3PARTSYS;
struct D3COLOR;

int d3t_PlaceObject(struct D3OBJECT *camera, float px, float py, float pz, float dx, float dy, float dz, int unk, char flags); /* 0x411680, starts an animation that moves the object to pos and turns it to dir */
int d3t_PlaceObjectV(struct D3OBJECT *camera, struct D3VECTOR *pos, struct D3VECTOR *dir, int unk, char flags); /* 0x411860, d3t_PlaceObject with the target position and direction given as */
int d3t_PlaceAtObject(struct D3OBJECT *obj, int unk);                                   /* 0x4118a0, places the camera at the pose a zoom target object stores (its 0x5c */
struct D3OBJECT *d3t_FindObject(char *name, char flags);                                /* 0x411900, the first object of the default scene whose name matches, searched */
int d3t_PlayAnim(struct D3OBJECT *obj, char *name, unsigned int flags);                 /* 0x411960, starts the animation <name> on obj: an already loaded one by its base */
int d3t_ReleaseObjectTexture(struct D3OBJECT *obj, char *name);                         /* 0x411b00, releases the named texture of the object's material (up to 3) and */
int d3t_ReleaseObjectTextures(struct D3OBJECT *obj);                                    /* 0x411b70, releases every named texture of the object's material */
int d3t_FindFreeScene(void);                                                     /* 0x411bb0, the index of a scene slot that is neither loaded nor dirty, or -1 */
int d3t_SceneIndex(struct D3SCENE *scene);                                              /* 0x411be0, the index of scene in d3_Scenes, -1 when it is not in the array */
void d3t_UpdateCamera(void);                                                     /* 0x411c10, per frame: ends a finished camera move and keeps the sun rays on the */
int d3t_GetHeightRange(struct D3OBJECT *obj, float *lo, float *hi);                     /* 0x411c70, height range of a mesh's bounding box */
int d3t_GetFootprint(struct D3OBJECT *obj, float t, struct D3VECTOR *c0, struct D3VECTOR *c1, struct D3VECTOR *c2, struct D3VECTOR *c3); /* 0x411d10, the four lower corners of the box around the mesh keys up to time t */
char d3t_GetBoundingSphere(struct D3OBJECT *obj, struct D3OBJECT *camera, struct D3VECTOR *center, float *radius); /* 0x411eb0, center and radius of a mesh's 8-corner box as seen from the camera */
int d3t_GetRowRange(struct D3TMAP *map, int *lo, int *hi, int x0, int x1, int y);       /* 0x412000, value range of row y between columns x0 and x1 */
int d3t_GetTriRange(struct D3TMAP *map, float *lo, float *hi, struct D3VECTOR *a, struct D3VECTOR *b, struct D3VECTOR *c); /* 0x412090, height range of the map under the triangle a, b, c (x/z in world */
int d3t_GetQuadRange(struct D3TMAP *map, float *lo, float *hi, struct D3VECTOR *a, struct D3VECTOR *b, struct D3VECTOR *c, struct D3VECTOR *d); /* 0x412480, height range of the map under the quad a, b, c, d */
float d3t_GetMeanHeight(struct D3VECTOR *pos);                                          /* 0x412550, mean terrain height over the 8x8 cells around pos */
int d3t_InTriBox(struct D3VECTOR **tri, struct D3VECTOR *a, struct D3VECTOR *b);                      /* 0x412640, does the triangle's bounding box overlap the box a..b in x/z and */
int d3t_QueryHeight(struct D3HQUERY *q, struct D3OBJECT *obj);                                 /* 0x412800, widens q->lo/hi by the triangles of obj and its children that lie in */
int d3t_DropObject(struct D3OBJECT *obj, int maponly, int top);                         /* 0x412a70, sets obj on the ground (the terrain map and all other meshes under */
void d3t_GetBounds(struct D3VECTOR *mn, struct D3VECTOR *mx, struct D3OBJECT *obj);                   /* 0x413040, widens mn/mx by the bounding boxes of obj and its children */
void d3t_GetObjectBounds(struct D3VECTOR *mn, struct D3VECTOR *mx, struct D3OBJECT *obj);             /* 0x413150, bounding box of obj and its children with the camera at the origin */
void d3t_GetMeshBounds(struct D3VECTOR *mn, struct D3VECTOR *mx, struct D3OBJECT *obj);               /* 0x413310, bounding box of obj alone with the camera at the origin */
void d3t_SetGrey(struct D3OBJECT *obj, int on);                                         /* 0x4134a0, on: sets every vertex color of obj to grey, off: lets 0x453280 */
void d3t_SetColor(struct D3OBJECT *obj, unsigned char r, unsigned char g, unsigned char b); /* 0x413550, sets every vertex color of obj to r/g/b */
void d3t_SetTreeColor(struct D3OBJECT *obj, unsigned char r, unsigned char g, unsigned char b); /* 0x413640, d3t_SetColor on obj and its subtree */
int d3t_MoveCameraPath(int time, int n, ...);                                    /* 0x4136d0, moves the camera along the n objects named in the varargs (an empty */
struct D3ANIM *d3t_MoveObjectPath(struct D3OBJECT *obj, int time, int n, struct D3OBJECT **path);     /* 0x413960, moves obj along the n objects in path (null entries are skipped) */
void d3t_InflateObjectTree(struct D3OBJECT *obj);                                       /* 0x413b00, inflates obj's geometry (d3_InflateObject) and then every object of */
struct D3TRAIN *d3t_CreateRain(int count);                                              /* 0x413b30, allocates and initialises the rain system for count drops: the */
void d3t_DeleteRain(struct D3TRAIN *rain);                                              /* 0x413d00, frees the rain system: its drop list (D3TRAIN.drops) and the struct D3TRAIN */
void d3t_SetRain(struct D3TRAIN *rain, int n, ...);                                     /* 0x413d30 */
void d3t_UpdateRain(struct D3TRAIN *rain, float dt);                                    /* 0x413f30 */
void d3t_DrawRain(struct D3TRAIN *rain);                                                /* 0x414590, per frame: eases count and wind toward their targets over */
struct D3TSNOW *d3t_CreateSnow(int count);                                              /* 0x4148f0, allocates and initialises the snow system for count flakes: the */
void d3t_FreeSnow(struct D3TSNOW *snow);                                                /* 0x414b50, frees the snow system: the flake list and texture, then resets the */
void d3t_SetSnow(struct D3TSNOW *snow, int n, ...);                                     /* 0x414c60 */
void d3t_DrawSnow(struct D3TSNOW *Snow);                                                /* 0x415380, per frame: eases count and wind toward their targets over */
struct D3PARTSYS *d3t_CreateEmitter(struct D3OBJECT *obj, char *tex, char flag, unsigned long color, struct D3VECTOR *pos, int count, int unused, float size, float a, float b, float c, float d); /* 0x415ad0, creates the emitter particle system of an object's mesh: par_Create */
struct D3PARTSYS *d3t_CreateBlood(void *parent, int n, float a, float b, float c, float d); /* 0x415e90, creates the blood spray: 200 "Blut" particles with the effect */
struct D3PARTSYS *d3t_CreateExplosion(void *parent, struct D3VECTOR *pos, int count, float radius, float height, float z, float size, unsigned int duration); /* 0x416040, creates the explosion: count "Explosion" particles around pos, with */
struct D3PARTSYS *d3t_CreateSmoke(void *parent, struct D3VECTOR *pos, int count, float size, float unused, float height, float y, float alpha, float spread, struct D3VECTOR *start, unsigned int duration, int delay, struct D3COLOR color); /* 0x416340 */
struct D3PARTSYS *d3t_CreateEarth(void *parent, struct D3VECTOR *pos, int count, float size, float a, float b, float c, float d, float e, float f, float g, struct D3COLOR color); /* 0x4168e0, creates the earth-brocken debris: count "Erdbrocken_an0" particles */
void d3t_CreateSnowMipmaps(void);                                                /* 0x416980 */
void d3t_FadeOutSunRays(struct D3OBJECT *unused, int b);                         /* 0x417150, fades the sun rays out (SunFade = -1 from now) */
void d3t_FadeInSunRays(struct D3OBJECT *unused);                                        /* 0x417170, fades the sun rays in: SunFade = 1 from now and the value */
struct D3OBJECT *d3t_CreateSunRays(void);                                               /* 0x417190, creates the "sonnenstrahlen" object at the camera position, rotated */
float d3t_SmoothStep(float a, float b, float x);                                 /* 0x417270, smooth Hermite step: t = clamp((x - a) / (b - a), 0, 1), returns (3 - */
void d3t_UpdateSunRays(struct D3OBJECT *obj);                                           /* 0x4172e0, updates the sun rays each frame: follows the camera position, turns */
void d3t_RemoveSunRays(struct D3OBJECT *obj);                                           /* 0x417560, removes the sun rays object obj: switches to the object's scene */
void d3t_FreeSnowMipmaps(void);                                                  /* 0x4175c0, frees the surfaces of the four software snow mipmap chains */

extern int d3t_SunFadeStart;                                 /* 0x5f345c */
extern int d3t_CamMoving;                                    /* 0x6310e4 */
extern struct D3ANIM * d3t_CamMove;                                 /* 0x6310e8 */
extern struct D3OBJECT * d3t_SunRays;                               /* 0x631120 */
#endif
