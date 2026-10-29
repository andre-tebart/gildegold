/* d3_tools
   unit of rea 8cef7d5b (src/game/d3_tools.c); boundaries in decl/text.dcl, notes in src/d3_tools/open.md */
#line 1 "D:\\develop\\Units\\d3_tools\\d3_tools.c"
/* d3_tools.c -- original path D:\develop\Units\d3_tools\d3_tools.c
   Extent in GildeGold.exe (inferred from data/bss/call-graph, not proven): 0x411680..0x417650.
   Functions are kept in their original order. Names are ours.
   @named_by leaked
   @boundary plausible */
struct D3VECTOR;
struct D3SM;
struct D3OBJECT;
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_d3d.h"
#include "d3_io_t.h"
#include "surf.h"

/* the pose a camera / zoom target stores in D3OBJECT 0x5c and 0x90 (serialized for object types 2/3;
   objectFlags bit 0 marks a key). For a light the same bytes are D3LOBJ.color and D3LOBJ.range/power/atten;
   a union view in D3OBJECT flips other units' code, so this is a local cast view (type debt). */
typedef struct D3OBJPOSE {
    char pad000[0x5c];
    D3VECTOR pos;                   /* 0x5c */
    char pad068[0x28];
    D3VECTOR rot;                   /* 0x90, the camera angles in degrees */
} D3OBJPOSE;
#define D3_POSE(o) ((D3OBJPOSE *)(o))

int d3t_PlaceObject(struct D3OBJECT *camera, float px, float py, float pz, float dx, float dy, float dz, int unk, char flags);    /* 0x411680 */
void d3t_UpdateSunRays(D3OBJECT *obj);                                     /* 0x4172e0 */

int d3t_CamMoving = 0;                   /* 0x6310e4 */
D3ANIM *d3t_CamMove = 0;                 /* 0x6310e8, running camera animation */
static D3OBJECT *d3t_FoundObject = 0;    /* 0x6310ec */
D3OBJECT *d3t_SunRays = 0;               /* 0x631120, reached from main.c by its address */
static int d3t_SunFade = 0;              /* 0x631124, 1 fading in, -1 fading out */
static int d3t_Unk631128 = 0;            /* 0x631128 */
int d3t_SunFadeStart = -1;  /* 0x5f345c, initialized to -1; TODO define here once .data is matched */

