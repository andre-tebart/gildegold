/* The message records of cm's rings ai_Unka69140 / ai_Unk9367e0 (G17MSG) and ai_Unkf31160 (G17STAT) and the
   network game record cm_NetGame (CMNETGAME), for the units that declare them without the rest of cm.h:
   types only. cm.h includes it. */
#ifndef CM_TYPES_H
#define CM_TYPES_H

#pragma pack(push, 1)
typedef struct G17MSG {                 /* 0x99 bytes, a game message (its head is net.h's NETPKT) */
    unsigned char type;                 /* 0x00 */
    unsigned short size;                /* 0x01, ai_Unk4c3180 */
    unsigned char unk03;                /* 0x03 */
    int idx;                            /* 0x04, its slot in the send ring */
    unsigned int seq;                   /* 0x08 */
    unsigned int next7;                 /* 0x0c, seq of the next part of a split message (type 7), 0: none */
    unsigned char data[0x81];           /* 0x10 */
    struct G17MSG *prev;                /* 0x91 */
    struct G17MSG *next;                /* 0x95 */
} G17MSG;

typedef struct G17STAT {                /* 10 bytes, ai_Unkf31160[]: the state of a sent message */
    unsigned char state;                /* 0x00, 0 sent, 1 free, 2 failed */
    unsigned char unk01;
    int idx;                            /* 0x02 */
    int value;                          /* 0x06 */
} G17STAT;
#pragma pack(pop)

#include "ls_types.h"                     /* NETHDR */
typedef struct CMNETGAME {              /* 0x80 bytes at 0x936460: the network game hosted or joined (main_SearchNetworkGame
                                           copies a game list entry over it as 0x20 dwords) */
    char pad00[0x10];
    NETHDR hdr;                         /* 0x10, the game record (main_HostNetworkGame copies ls_SavePath.net here);
                                           hdr.name at 0x16, hdr.players at 0x36 */
    char pad7a[2];
    int mode;                           /* 0x7c, 0x3f a new game, 0x7f a saved one (was ai_ModeMask) */
} CMNETGAME;
typedef char cm_AssertCMNETGAME[sizeof(CMNETGAME) == 0x80 ? 1 : -1];

typedef struct CMJOIN {                 /* 0x30 bytes at 0x9367a0 (inside ai_Unk9364e0's extent, past its 8 records
                                           of 0x57 bytes): the character of a joined saved game, from its LSHDR
                                           (main_JoinNetworkGame) */
    char text1[0x10];                   /* 0x00, LSHDR.charText1 */
    char text2[0x10];                   /* 0x10, LSHDR.charText2 */
    int char164;                        /* 0x20 */
    int char09;                         /* 0x24 */
    int char0c;                         /* 0x28 */
    int char54;                         /* 0x2c */
} CMJOIN;

#endif
