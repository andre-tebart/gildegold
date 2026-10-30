/* The records of ls.c's ls_SavePath (LSSAVE) and the network game record in it (NETHDR, also the head
   of cm's cm_NetGame), for the units that need them without the rest of ls.h: types only. ls.h
   includes it. */
#ifndef LS_TYPES_H
#define LS_TYPES_H

#pragma pack(push, 2)
typedef struct NETHDR {             /* 0x6a bytes, the network game record (main_HostNetworkGame 0x5328f0
                                       fills it: +4 the size 0x6a, +6 the name, +0x26 the player count) */
    short a, b, size;               /* 0x00 */
    char name[0x20];                /* 0x06 */
    unsigned char players;          /* 0x26 */
    char pad27;
    short x;                        /* 0x28 */
    char rest[0x40];                /* 0x2a */
} NETHDR;
#pragma pack(pop)

typedef struct LSSAVE {             /* 0x280 bytes at 0x75e640 (ls_SavePath) */
    char path[0x80];                /* 0x000, the path of the save game (ls_SaveGame), its first 0x60 bytes
                                       are written into the save header */
    int netLib;                     /* 0x080, main: the network DLL (LoadLibraryA), 0 = none */
    short cur;                      /* 0x084, main: read into NETHDR.x, passed to net_Connect */
    char pad086[0x1a];
    NETHDR net;                     /* 0x0a0, main: the network game record */
    char pad10a[0x16];
    char version[0x40];             /* 0x120, main: the version text (main_BuildVersionString) */
    char rest[0x120];               /* 0x160 */
} LSSAVE;
typedef char ls_AssertLSSAVE[sizeof(LSSAVE) == 0x280 ? 1 : -1];

#endif
