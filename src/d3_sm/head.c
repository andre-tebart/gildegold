/* d3_sm.c -- original file name unknown; allocation tags "d3_sm:...", debug output "d3sm_...".
   Terrain height map with ground types and object shadows, sampled from a terrain.
   Extent in GildeGold.exe (inferred from .data and the call graph): 0x45db40..0x45f610.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_sm.c); boundaries in decl/text.dcl, notes in src/d3_sm/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_sm.h"
#include "data_views.h"
extern char d3_Cfg[];
extern const char d3_ScrollPool[];


void d3_RadixSort(unsigned int n, unsigned char twobytes);                                                      /* 0x43b300, sorts d3_SortBuf */
void d3_TransformMeshes(void);                                                                /* 0x43d840 */
struct D3VECTOR4; void d3_TriNormal(struct D3VECTOR4 *a, struct D3VECTOR4 *b, struct D3VECTOR4 *c, struct D3VECTOR4 *n);               /* 0x454700 */

void d3s_ShadowTriangle(float *p, unsigned char *c, unsigned char *dst, int pitch, unsigned int flags, char *base, int radius);                                       /* 0x45a2e0 */
void d3_MatTranspose(D3MATRIX *src, D3MATRIX *dst);                                        /* 0x454040 */
extern int d3_Busy;                         /* 0x633b04 */
const int d3_TriEdge[3][2] = { { 1, 2 }, { 2, 0 }, { 0, 1 } };  /* 0x5eba80, {1, 2}, {2, 0}, {0, 1} */
extern struct D3SORTENT *d3_SortBuf;                /* 0xfc2de0 */
extern struct D3SORTENT *d3_SortPtr;                /* 0xfc2dc8 */
extern unsigned int d3_SortCount;                    /* 0xfc2dcc */
extern float d3_CenterX;                   /* 0xfe75fc */
extern float d3_CenterY;                   /* 0xfe7604 */
extern float d3_ScaleX;                    /* 0xfc5348 */
extern float d3_ScaleY;                    /* 0xfc2944 */
char d3sm_GroundNames[15][9] = { "_ill*", "_unk*", "SAND", "ERDE", "WIESE", "MOOR", "PFLASTER", "KIESEL", "FELS", "EIS", "WASSER", "WEG", "WEG", "_ill*", "" };  /* 0x5fcfa0, "_ill*", "_unk*", "SAND", ...; TODO define here once .data is matched */

int d3sm_Ground[8] = {0};                   /* 0x633dd8, ground type per terrain material */
D3SM *d3sm_Shadow = 0;                      /* 0x633df8 */
D3TERRAIN *d3sm_Last = 0;                   /* 0x633dfc, terrain d3sm_Ground was built for */
#define d3_SortBufV (*(D3SMSORT * (*))&d3_SortBuf)  /* 0xfc2de0, this unit reads it as `D3SMSORT *d3_SortBuf` */
#define d3_SortCountI (*(int (*))&d3_SortCount)  /* 0xfc2dcc, this unit reads it as `int d3_SortCount` */
#define d3_SortPtrV (*(D3SMSORT * (*))&d3_SortPtr)  /* 0xfc2dc8, this unit reads it as `D3SMSORT *d3_SortPtr` */
