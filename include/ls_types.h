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
    unsigned char joined;           /* 0x27, the search list disables a new game with joined >= players (a
                                       reasoned name: the players already in) */
    short x;                        /* 0x28 */
    char save[0x20];                /* 0x2a, the save game's name (main_JoinNetworkGame: main_NetSaveName) */
    int party[8];                   /* 0x4a, the party table of the save header (LSHDR.party) */
} NETHDR;
#pragma pack(pop)

typedef struct LSHDR {              /* 0xfc bytes, the save game header: ls_WriteSaveHeader 0x5194c0 writes it
                                       field by field, ls_ReadSaveHeader 0x51cee0 reads it (each field from the
                                       version its reader names); "char" means the player's character */
    int version;                    /* 0x00, 0x1005a now (ls_Version) */
    unsigned char flags;            /* 0x04, the writer's argument */
    char player[0x20];              /* 0x05, the player's name */
    char pad25[3];
    int playerWords[2];             /* 0x28, the player slot's +0x40 / +0x44, its map place (GMPLAYERSLOT.mapX / mapY) */
    unsigned char playerByte;       /* 0x30, the player slot's +0x48 (GMPLAYERSLOT.b48) */
    unsigned char players;          /* 0x31, ai_NumPlayers */
    char time[0xe];                 /* 0x32, ai_Time */
    char charText1[0x10];           /* 0x40, the char's +0x30 text, "SCENARIO" without a character */
    char charText2[0x10];           /* 0x50, the char's +0x40 text (versions >= 0x10054) */
    unsigned char char164;          /* 0x60, the char's +0x164 byte */
    unsigned char char166;          /* 0x61, the char's +0x166 byte */
    unsigned char char0d;           /* 0x62, the char's +0xd byte */
    unsigned char char0c;           /* 0x63, the char's +0xc byte (versions >= 0x10054) */
    int char54;                     /* 0x64, the char's +0x54 word (versions >= 0x10054) */
    int worth;                      /* 0x68, gm_GetWorth through gm_ConvertMoney */
    unsigned char char09;           /* 0x6c, the char's +9 byte */
    unsigned char slot;             /* 0x6d, ls_SaveSlot */
    char pad6e[2];
    int building;                   /* 0x70, the player's building id, -1 none */
    int panel;                      /* 0x74, the shown panel's id, -1 none */
    int party[8];                   /* 0x78, ai_Unk76fa24 (versions >= 0x10034) */
    int difficulty;                 /* 0x98, ai_Difficulty (2 before 0x10038) */
    char path[0x60];                /* 0x9c, ls_SavePath.path ("Savegame" before 0x10039) */
} LSHDR;
typedef char ls_AssertLSHDR[sizeof(LSHDR) == 0xfc ? 1 : -1];

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
