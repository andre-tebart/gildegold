struct D3VECTOR;
struct D3VECTOR4;
/* d3_vec.h -- vector / angle helpers and the light budget (0x453920..0x454890), see d3_vec.c.
   Names are ours. */
#ifndef UNK_453920_H
#define UNK_453920_H
#include "d3.h"                       /* the D3 records and typedefs this header names */

extern const D3VECTOR d3_ZAxis;          /* 0x5ebb74, (0, 0, 1) */
extern const D3VECTOR d3_XAxis;          /* 0x5ebb84, (1, 0, 0) */
extern const D3VECTOR d3_YAxis;          /* 0x5ebb94, (0, 1, 0) */
extern const D3VECTOR d3_Origin;         /* 0x5ebba4, (0, 0, 0) */

void d3_ShadeInit(unsigned int visible_budget, unsigned int culled_budget); /* 0x453920 */
void d3_ShadeReset(void);                                              /* 0x453990 */
void d3_Lerp3(float *a, float *b, float t, float *out);                /* 0x4539b0 */
float d3_VecAngle(D3VECTOR *a, D3VECTOR *b);                           /* 0x4539f0 */
float d3_VecAngle2(D3VECTOR *a, D3VECTOR *b);                          /* 0x453b40 */
void d3_MakeBasis(D3VECTOR *dir, float roll, D3MATRIX *m);             /* 0x453b70 */
void d3_UnwrapAngles(float *a, float *b, float *ra, float *rb);        /* 0x453d60 */
float d3_Hermite(float p0, float p1, float m0, float m1, float t);     /* 0x453e20 */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);                  /* 0x453e80 */
void d3_MatMul43(D3MATRIX *a, D3MATRIX *b, D3MATRIX *out);             /* 0x453ee0 */

void d3_MatTranspose(D3MATRIX *src, D3MATRIX *dst);                    /* 0x454040, transpose */
void d3_MatInverse(D3MATRIX *src, D3MATRIX *dst);                    /* 0x454070, inverse */
void d3_MatIdentity(D3MATRIX *m);                                     /* 0x454510, identity */
void d3_MatFromAngles(D3VECTOR4 *v, D3MATRIX *m);                       /* 0x4545c0, angles to matrix */
void d3_MatToAngles(D3VECTOR4 *m, D3VECTOR4 *angles);                 /* 0x454690, matrix rows to angles */
void d3_TriNormal(struct D3VECTOR4 *a, struct D3VECTOR4 *b, struct D3VECTOR4 *c, struct D3VECTOR4 *n);   /* 0x454700, face normal */
#endif
