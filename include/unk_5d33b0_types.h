/* The option dword unk_5d33b0's profile reader fills (ai_GfxDetail, ai_GfxFloor, ai_GfxResolution,
   ai_GamePanel ..), for the units that declare those options without the rest of unk_5d33b0.h: types
   only. unk_5d33b0.h includes it. */
#ifndef UNK_5D33B0_TYPES_H
#define UNK_5D33B0_TYPES_H

typedef union OPTIONBYTES {
    int word;                               /* the whole dword */
    unsigned char byte[4];                  /* the four option bytes */
} OPTIONBYTES;

#endif
