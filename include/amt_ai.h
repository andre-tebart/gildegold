/* amt_ai.h: the public prototypes and extern declarations of unit amt_ai (am's second object, 0x4a3a90..0x4a7140) */
#ifndef AMT_AI_H
#define AMT_AI_H

struct AIPER;
int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out);  /* 0x4a63f0, a building of the kinds in mask for p (6: also its partner's ..), with a rating between lo and hi; its id in *out */
extern const int ai_RingStrideCount;  /* 0x5ed1ac, the entries of ai_RingStrides */

#endif
