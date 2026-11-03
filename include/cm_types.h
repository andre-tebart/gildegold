/* The message records of cm's rings cm_SendRing / cm_RecvPool (G17MSG) and cm_SendStates (G17STAT) and the
   network game record cm_NetGame (CMNETGAME) and the crime record CRIME (ai_Crimes), for the units that declare them without the rest of cm.h:
   types only. cm.h includes it. */
#ifndef CM_TYPES_H
#define CM_TYPES_H
#include "ai_types.h"                /* AITIME, for CRIME */

#pragma pack(push, 1)
typedef struct G17MSG {                 /* 0x99 bytes, a game message (its head is net.h's NETPKT) */
    unsigned char type;                 /* 0x00 */
    unsigned short size;                /* 0x01, cm_MessageSize */
    unsigned char sender;               /* 0x03, 0 when sent; the parts of one split message match by it and by seq
                                           (reasoned guess: the sender's slot, filled in by the server) */
    int idx;                            /* 0x04, its slot in the send ring */
    unsigned int seq;                   /* 0x08 */
    unsigned int next7;                 /* 0x0c, seq of the next part of a split message (type 7), 0: none */
    union {
        unsigned char data[0x81];       /* 0x10 */
        unsigned short splitLen;        /* 0x10, in the first part of a split message (type 7 chain): the length
                                           of the whole, its bytes follow at data + 2 */
    };
    struct G17MSG *prev;                /* 0x91 */
    struct G17MSG *next;                /* 0x95 */
} G17MSG;

typedef struct G17STAT {                /* 10 bytes, cm_SendStates[]: the state of a sent message */
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

#pragma pack(push, 1)
typedef struct CMLOBBYPLAYER {          /* 0x57 bytes, a player of the network lobby: the 8 records of ai_Unk9364e0,
                                           sent whole by the lobby messages of type 0x5f (kinds 2 and 4) */
    int id;                             /* 0x00, the player's net id (ai_Unk9367d0 for this player), 0: a free record */
    int ready;                          /* 0x04, 1: ready to start (kind 1; cm_LobbyAllReady counts them) */
    char pad08[2];                      /* 0x08, no user seen */
    unsigned char status;               /* 0x0a, 0x10 as INIT_PLAYER.status of a new profile */
    unsigned char job;                  /* 0x0b, the profession (INIT_PLAYER.job, the profile key "Beruf") */
    int coat;                           /* 0x0c, the coat of arms index (INIT_PLAYER.coat - 0x59a, "Wappen") */
    unsigned char sex;                  /* 0x10, 0 male / 1 female ("Geschlecht") */
    unsigned char faith;                /* 0x11, "Glauben" */
    char firstName[0x10];               /* 0x12, "Name" */
    char familyName[0x10];              /* 0x22, "Familienname" */
    int reserved;                       /* 0x32, -1 for a new player; no reader seen */
    unsigned char talents[5];           /* 0x36, 0x5a 0x46 0x78 0x46 0x5a for a new player (a reasoned guess: the
                                           five talents of INIT_PLAYER.talents) */
    char pad3b[0x1c];                   /* 0x3b, no user seen */
} CMLOBBYPLAYER;
#pragma pack(pop)
typedef char cm_AssertCMLOBBYPLAYER[sizeof(CMLOBBYPLAYER) == 0x57 ? 1 : -1];

/* the scene party records (0x925520, 96 of 0x114 bytes: +0 id (-1 free), +8 kind, +0x14 the thing, +0x26 flags, +0x30 used) */
typedef struct G28REC {
    int id;
    char pad04[4];
    unsigned char kind;                 /* 0x08 */
    char pad09[3];
    int host;                           /* 0x0c, the netId of the player's character who holds it (gv_PassSentence, he.c) */
    char pad10[4];
    int thing;                          /* 0x14 */
    AITIME time;                        /* 0x18, when it takes place (ai_Time plus some hours) */
    unsigned char flags;                /* 0x26 */
    char pad27[9];
    unsigned char used;                 /* 0x30, the number of members[] (ai_Unk4e51b0: at most 16) */
    char pad31[3];
    int members[16];                    /* 0x34, the members' netIds (-1 for one removed, ai_Unk4e5210) */
    char pad74[8];
    unsigned char variant;              /* 0x7c, written by gv_PassSentence with a random 0 .. 4 */
    unsigned char zero7d;               /* 0x7d, only seen written 0 (gv_PassSentence) */
    char pad7e[0x114 - 0x7e];
} G28REC;
typedef char cm_AssertG28REC[sizeof(G28REC) == 0x114 ? 1 : -1];

#pragma pack(push, 1)
typedef struct CRIME {                  /* 0x2d bytes, a crime record of ai_Crimes (message 0x22, "cm_ExAddStraftat"; saved by gv_WriteCrimes) */
    int id;                             /* 0x00, ai_NextCrimeId at the record's creation, -1 when free */
    AITIME time;                        /* 0x04, the game time of the crime (ai_Time) */
    int victim;                         /* 0x12, the victim's netId ("opfer") */
    int culprit;                        /* 0x16, the culprit's netId ("taeter"), -1 when free */
    short count;                        /* 0x1a, counts down to the record's retirement (0x4f37e0) */
    unsigned char law;                  /* 0x1c, the AI decision (law) record broken (gv_GetTableRecord) */
    int v1d;                            /* 0x1d */
    int place;                          /* 0x21, the object id of the scene (ord_AddGridAmount), -1 for none */
    unsigned int state;                 /* 0x25, 0 free, 1 open, above 1 handed over (a job id; unsigned: gv_SendCrimes, gv_FindAccused) */
    int seed;                           /* 0x29, a random seed (util_Seed when there is no place) */
} CRIME;
#pragma pack(pop)
typedef char cm_AssertCRIME[sizeof(CRIME) == 0x2d ? 1 : -1];

#endif
