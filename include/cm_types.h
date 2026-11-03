/* The message records of cm's rings cm_SendRing / cm_RecvPool (G17MSG) and cm_SendStates (G17STAT) and the
   network game record cm_NetGame (CMNETGAME) and the crime record CRIME (ai_Crimes), for the units that declare them without the rest of cm.h:
   types only. cm.h includes it. */
#ifndef CM_TYPES_H
#define CM_TYPES_H
#include "ai_types.h"                /* AITIME, for CRIME */
#include "d3_types.h"                /* D3VECTOR, D3VECTOR4 */

#pragma pack(push, 1)
typedef struct G17B7 { char b[7]; } G17B7;      /* blocks copied as one */

typedef struct G17OBJMSG {              /* 0xf bytes, the data of message 0x2d (its handler 0x4c9740 creates an
                                           object of kind 0x1b5 in the building owner) */
    int objectId;                       /* 0x00, the next object id (the server's, read when connected) */
    int owner;                          /* 0x04, the building's id */
    G17B7 value;                        /* 0x08, copied to the new object's +0x1c */
} G17OBJMSG;

typedef struct G17GOTOMSG {             /* 0x4d bytes, the data of message 0x35 (cm_SendChrGotoBuilding, its handler
                                           0x4c9dc0 "cm_ExChrGotoBuilding") */
    int charId;                         /* 0x00, the character sent ("sp_<id>") */
    int bldId;                          /* 0x04, the building it goes to, -1: none */
    int objId;                          /* 0x08, the object (room) in it, -1: none */
    unsigned char noObject;             /* 0x0c, 1: no object given (objId -1) */
    char text[0x20];                    /* 0x0d, (reasoned guess: an action name; every caller passes 0) */
    char reason[0x20];                  /* 0x2d, why it goes ("Unfall", "Pest", "RndGeselle") */
} G17GOTOMSG;

typedef struct G17HIREMSG {             /* 0x17 bytes, the data of message 0x3d (cm_SendMessage3d, its handler
                                           0x4cb250 creates the character) */
    int bldId;                          /* 0x00, the building the character works in */
    int homeId;                         /* 0x04, its home, -1: none (the applicant's own) */
    unsigned char kind;                 /* 0x08, the kind of character */
    int charId;                         /* 0x09, the applicant taken over, -1: a new one */
    unsigned char title;                /* 0x0d, gm_FindApplicant's title */
    unsigned char job;                  /* 0x0e, gm_FindApplicant's job */
    int objectId;                       /* 0x0f, the next object id (the server's, read when connected) */
    int seed;                           /* 0x13, util_Seed (rand() when not connected) */
} G17HIREMSG;

typedef struct G17BUILDMSG {            /* 0x40 bytes, the data of message 10 (cm_SendMessage10, its handler
                                           0x4c6b20 creates the building) */
    int objectId;                       /* 0x00, the next object id (the server's, read when connected) */
    unsigned short kind;                /* 0x04, the building kind (gm_CreateBuilding) */
    int owner;                          /* 0x06, the owner character's id, -1: none */
    short flags;                        /* 0x0a, the building's flags (AIBLD +0x5a) */
    int hasBlock;                       /* 0x0c, 1: block holds the building's 0x30 bytes at +0x65 */
    unsigned char block[0x30];          /* 0x10 */
} G17BUILDMSG;

typedef struct G17GOODSMSG {            /* 0x1f bytes, the data of message 0x40 (cm_SendGoodsDelta, its handler 0x4cb730
                                           adds the deltas to the goods record gm_FindGoods(player, item)) */
    unsigned char player;               /* 0x00 */
    unsigned short item;                /* 0x01, GMSTOCK.item */
    int daily[3];                       /* 0x03, the deltas of GMSTOCK.daily[0..2] */
    float demand;                       /* 0x0f, the deltas of the floats of GMSTOCK */
    float mean;                         /* 0x13 */
    float f2c;                          /* 0x17 */
    float price;                        /* 0x1b */
} G17GOODSMSG;

typedef struct G17GEBMSG {              /* 0x25 bytes, the data of message 0x4c (cm_RequestCreateGeb, its handler
                                           0x4cbca0 "cm_ExCreateGeb" creates the building and loads its model) */
    int owner;                          /* 0x00, the owner character's id */
    int objectId;                       /* 0x04, the next object id (the server's, read when connected) */
    unsigned char kind;                 /* 0x08, the building kind */
    D3VECTOR4 pos;                      /* 0x09, the position */
    int rot[3];                         /* 0x19, the rotation (d3_SetObjectRot) */
} G17GEBMSG;

typedef struct CMFIGHTSLOT {            /* 0x2c bytes, a fighter slot of a fight record (16 at its +0x94, ai_Unk4d76a0) */
    int id;                             /* 0x00, the fighter's id (AICHAR.netId), -1: free */
    int active;                         /* 0x04 */
    char pad08[8];
    int target;                         /* 0x10, the target's id */
    int x, y;                           /* 0x14, the attacked place */
    char pad1c;
    unsigned char hit;                  /* 0x1d */
    char pad1e[2];
    int kill;                           /* 0x20 */
    int damage;                         /* 0x24 */
    int health;                         /* 0x28, the target's new health (its AICHAR.v024) */
} CMFIGHTSLOT;

typedef struct G17FIGHTMSG {            /* 0x51 bytes, the data of message 0x55 (cm_SendFightAction, its handler 0x4cc7b0
                                           runs the command for the fighter slot) */
    int fight;                          /* 0x00, the fight's id (the first int of its record) */
    unsigned char cmd;                  /* 0x04, the command 1 .. 7 */
    D3VECTOR pos;                       /* 0x05, the position of the fighter's body */
    char pad11[4];
    D3VECTOR rot;                       /* 0x15, its rotation */
    int health;                         /* 0x21, the fighter's AICHAR.v024 */
    CMFIGHTSLOT slot;                   /* 0x25 */
} G17FIGHTMSG;

typedef struct G17CHARMSG {             /* 0x1a bytes, the data of message 0xb (cm_SendCreateCharacter, its handler 0x4c6bf0
                                           calls gm_CreateCharacter with the fields) */
    int objectId;                       /* 0x00, the next object id (the server's, read when connected) */
    unsigned char type;                 /* 0x04, the character type */
    int parent1;                        /* 0x05, the parents' ids, -1: none (f3gm.c passes two characters' netIds) */
    int parent2;                        /* 0x09 */
    unsigned short age;                 /* 0x0d */
    int bldId;                          /* 0x0f, the building, -1: none (-2 .. -4: the last created, cm_ResolveId) */
    unsigned char job;                  /* 0x13 */
    unsigned char title;                /* 0x14 */
    unsigned char a8;                   /* 0x15, gm_CreateCharacter's last argument (2 from f3gm.c) */
    int seed;                           /* 0x16, util_Seed (rand() when not connected) */
} G17CHARMSG;

typedef struct G17NAMEDCHARMSG {        /* 0x39 bytes, the data of message 0xc (cm_SendMessage0c, its handler 0x4c6cd0 creates
                                           a named character with gm_CreateCharacter) */
    int objectId;                       /* 0x00, the next object id (the server's, read when connected) */
    int parent1;                        /* 0x04, the parents' ids, -1: none */
    int parent2;                        /* 0x08 */
    union {
        struct {
            unsigned short age;         /* 0x0c */
            unsigned char job;          /* 0x0e */
            int ref54;                  /* 0x0f, the new character's AICHAR.ref54 */
            unsigned char a8;           /* 0x13, gm_CreateCharacter's last argument */
            unsigned char b00c;         /* 0x14, the new character's AICHAR.b00c */
            char first[0x10];           /* 0x15, its first name */
        };
        struct {
            char pad0c[6];
            int seedRead;               /* 0x12, the dword the handler seeds the generator with (over ref54 / a8 / b00c) */
        };
    };
    char family[0x10];                  /* 0x25, its family name (also its player record's and its relatives[1]'s) */
    int seed;                           /* 0x35, rand() when not connected (the handler seeds with the dword at 0x12) */
} G17NAMEDCHARMSG;

typedef struct G17TAKEOVERMSG {         /* 8 bytes, the data of message 0xd (0x4c4770, its handler 0x4c6e10): gm_TakeOverCharacter */
    int taker;                          /* 0x00, the character (netId) that takes over */
    int taken;                          /* 0x04, the character taken over */
} G17TAKEOVERMSG;

typedef struct G17SELLMSG {             /* 0x17 bytes, the data of message 0x11 (cm_SendExSellObject, its handler 0x4c70b0
                                           "cm_ExSellObjekt") */
    int buyer;                          /* 0x00, the id (building, object or character) the goods go to, -1: none */
    int seller;                         /* 0x04, the id they come from, -1: none */
    unsigned short item;                /* 0x08, the goods prototype */
    int objectId;                       /* 0x0a, the next object id (the server's, read when connected) */
    unsigned char player;               /* 0x0e, the player slot whose money good pays */
    int count;                          /* 0x0f */
    int price;                          /* 0x13, per unit, 0: no payment */
} G17SELLMSG;

typedef struct G17MAKEMSG {             /* 0xe bytes, the data of message 0x12 (sent by 0x4c48e0, its handler 0x4c78c0
                                           makes goods in a workshop object) */
    int objId;                          /* 0x00, the workshop object (kind 0x2a / 0x116), resolved by cm_ResolveId */
    unsigned short item;                /* 0x04, the goods prototype made */
    int runs;                           /* 0x06, the production runs asked for (AIGOOD.batch each) */
    int objectId;                       /* 0x0a, the next object id (the server's, read when connected) */
} G17MAKEMSG;

typedef struct G17MOVEMSG {             /* 12 bytes, the data of message 0x13 (sent by 0x4c4920, its handler 0x4c7c30
                                           moves an object from one holder's list to another's) */
    int to;                             /* 0x00, the new holder (building, object or character id) */
    int from;                           /* 0x04, the holder whose list holds it */
    int objId;                          /* 0x08, the object moved (AIOBJ.objId) */
} G17MOVEMSG;

typedef struct G17FIELDS {              /* 0x7c bytes, a field block (0xa68c20, 0xa68ca0); the data of the messages
                                           0x16 (deltas, its handler 0x4c7e90), 0x17 (values) and 0x18 */
    int id;                             /* 0x00, the object (building, object or character id) */
    unsigned char n;                    /* 0x04, fields */
    unsigned char buf[0x77];            /* 0x05, per field: size (1, 2, 4), count, offset (short), values */
} G17FIELDS;

typedef struct G17RELMSG {              /* 0x18 bytes, the data of message 0x1b (cm_SendRelations, its handler 0x4c8240) */
    int from;                           /* 0x00, the character (netId) whose relation changes */
    int to;                             /* 0x04, the character (netId) it is towards */
    int delta;                          /* 0x08, added to ai_Relations[from][to] (kinds 0..2) */
    int kind;                           /* 0x0c, 0..2 add delta (1 also to the memory, 2 clears it), 3 scales the
                                           memory of everyone towards `to`, 4 drifts all relations */
    float scale;                        /* 0x10, ai_DifficultyScale (kind 3) */
    unsigned int level;                 /* 0x14, (int)ai_DifficultyValue: the drift's strength (kind 4) */
} G17RELMSG;

typedef struct G17TABLEMSG {            /* 12 bytes, the data of message 0x54 (cm_SendTableRequest, its check 0x4c6870 and
                                           handler 0x4cc510) */
    int op;                             /* 0x00, 'join', 'leav' or 'cler' */
    int charId;                         /* 0x04, the character that takes or leaves a seat */
    int bldId;                          /* 0x08, the building whose table (object kind 0x12d) it is */
} G17TABLEMSG;

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
        G17BUILDMSG build;              /* 0x10, message 10 */
        G17OBJMSG obj;                  /* 0x10, message 0x2d */
        G17GOTOMSG go;                  /* 0x10, message 0x35 */
        G17CHARMSG chr;                 /* 0x10, message 0xb */
        G17NAMEDCHARMSG named;          /* 0x10, message 0xc */
        G17TAKEOVERMSG takeover;        /* 0x10, message 0xd */
        G17SELLMSG sell;                /* 0x10, message 0x11 */
        G17MAKEMSG make;                /* 0x10, message 0x12 */
        G17MOVEMSG move;                /* 0x10, message 0x13 */
        G17FIELDS fields;               /* 0x10, messages 0x16 / 0x17 / 0x18 */
        G17HIREMSG hire;                /* 0x10, message 0x3d */
        G17GOODSMSG goods;              /* 0x10, message 0x40 */
        G17GEBMSG geb;                  /* 0x10, message 0x4c */
        G17RELMSG rel;                  /* 0x10, message 0x1b */
        G17TABLEMSG table;              /* 0x10, message 0x54 */
        G17FIGHTMSG fight;              /* 0x10, message 0x55 */
    };
    struct G17MSG *prev;                /* 0x91 */
    struct G17MSG *next;                /* 0x95 */
} G17MSG;

typedef struct G17STAT {                /* 10 bytes, cm_SendStates[]: the state of a sent message */
    unsigned char state;                /* 0x00, 0 sent, 1 free, 2 failed */
    unsigned char kind;                 /* 0x01, what value holds: 0 nothing, 1 a character, 2 a building */
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
