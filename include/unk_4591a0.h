struct XOBJ;
/* Declarations of unk_4591a0.c (0x4591a0..0x459d80): mesh vertex animation blending, normals and bounding box
   corners. The mesh types are d3_light.h's (D3LVERT / D3LINFO / D3LOPS). Names are ours. */
#ifndef UNK_4591A0_H
#define UNK_4591A0_H

void d3_WorldMatrix(struct XOBJ *obj, struct XOBJ *ref, char orientOnly, struct D3MATRIX *out);     /* 0x458da0 */

extern float d3_MinZ;                       /* 0xfc5344 */
extern float d3_MaxZ;                       /* 0xfc5338 */

#endif
