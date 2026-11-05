/* The option dword menu's profile reader fills (ai_GfxDetail, ai_GfxFloor, ai_GfxResolution,
   ai_GamePanel ..), for the units that declare those options without the rest of menu.h: types
   only. menu.h includes it. */
#ifndef MENU_TYPES_H
#define MENU_TYPES_H

typedef union OPTIONBYTES {
    int word;                               /* the whole dword */
    unsigned char byte[4];                  /* the four option bytes */
} OPTIONBYTES;

#endif
