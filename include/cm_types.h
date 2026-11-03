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

typedef struct G17GATEMSG {             /* 12 bytes, the data of message 0x36 (cm_SendChrUseGates, its handler
                                           0x4ca8a0 cm_ExChrUseGates) */
    int charId;                         /* 0x00, the character ("sp_<id>") */
    int bldId;                          /* 0x04, the building it is in */
    int objId;                          /* 0x08, the room (object) it goes to */
} G17GATEMSG;

typedef struct G17HOMEMSG {             /* 8 bytes, the data of message 0x39 (cm_SendBoss, its handler 0x4cab40
                                           cm_ExSetHome) */
    int bldId;                          /* 0x00, the building the character lives in from now */
    int charId;                         /* 0x04, the character */
} G17HOMEMSG;

typedef struct G17UPGRADEMSG {          /* 8 bytes, the data of message 0x3a (sent by 0x4c54e0, its handler 0x4cac40
                                           cm_ExGebUpgrade) */
    int bldId;                          /* 0x00, the building upgraded */
    int nextObjId;                      /* 0x04, ai_NextObjectId from now, taken while connected (reasoned: the server's) */
} G17UPGRADEMSG;

typedef struct G17MODELMSG {            /* 0x24 bytes, the data of message 0x3e (cm_SendCharModel, its handler 0x4cb4a0
                                           cm_ExCharModel) */
    int charId;                         /* 0x00, the character */
    char name[0x20];                    /* 0x04, its new model ("stallbursche_MANN") */
} G17MODELMSG;

typedef struct G17SPAWNMSG {            /* 0x26 bytes, the data of message 0x49 (sent by 0x4c59f0, its handler 0x4cba10
                                           cm_ExSpawnCharacter) */
    int objectId;                       /* 0x00, the next object id (the server's, read when connected) */
    unsigned char kind;                 /* 0x04, the kind of character */
    char pad05[2];
    int bldId;                          /* 0x07, the building it appears at (cm_ResolveId), -1: none */
    int objId;                          /* 0x0b, the room (object) in it, -1: none */
    unsigned char job;                  /* 0x0f, gm_CreateCharacter's job (a6) */
    unsigned char title;                /* 0x10, its title (a7) */
    unsigned char item;                 /* 0x11, an item it holds, 0: none */
    int seed;                           /* 0x12, util_Seed (timeGetTime() when not connected) */
    char text[0x10];                    /* 0x16, a tag ("SPIONAGE"), not read by the handler */
} G17SPAWNMSG;

typedef struct G17CHATMSG {             /* 0x80 bytes, the data of message 0x4b (cm_SendChat, its handler 0x4cbbd0
                                           cm_ExChat) */
    int to[8];                          /* 0x00, the netIds of the characters it is for */
    char text[0x60];                    /* 0x20, the line ("$A" breaks lines) */
} G17CHATMSG;

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

typedef struct CMFIGHTSLOT {            /* 0x2c bytes, a fighter slot of a fight record (16 at its +0x94, cm_RunCutOrders) */
    int id;                             /* 0x00, the fighter's id (AICHAR.netId), -1: free */
    int active;                         /* 0x04 */
    char pad08[8];
    union {
        struct {
            int target;                 /* 0x10, the target's id */
            int x, y;                   /* 0x14, the attacked place (where a pot or a bomb is thrown) */
            unsigned char held;         /* 0x1c, as CMCUTENTRY.held: the formation types 1 .. 3, such a fighter does not
                                           walk to its target (cm_Attack) */
            unsigned char hit;          /* 0x1d, a blow is struck (a bomb thrown) */
            char pad1e[2];
            int kill;                   /* 0x20, the blow lands (the pot is laid) */
            int damage;                 /* 0x24 */
            int health;                 /* 0x28, the target's new health (its AICHAR.v024) */
        };
        char dummy[0x1c];               /* 0x10, the object name of the escape dummy (cm_PlantEscapeFlag) */
    };
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
typedef char cm_AssertCRIME[sizeof(CRIME) == 0x2d ? 1 : -1];

typedef struct G17SKILLMSG {            /* 12 bytes, the data of message 0x5d (cm_SendAddSkill sends, the handler 0x4ccc20
                                           applies it) */
    int charId;                         /* 0x00, the character (gm_FindChar) */
    int skill;                          /* 0x04, the index into AICHAR.skills, 0 .. 4 */
    int delta;                          /* 0x08, added to the skill (the sum clamped to 0 .. 252) */
} G17SKILLMSG;

typedef struct CMCUTENTRY {             /* 0x2c bytes, a character's entry of a cutscene record (messages 0x50 / 0x51
                                           copy one whole) */
    int charId;                         /* 0x00, the character's netId, -1 when free */
    int state;                          /* 0x04, 0 frees the slot (cm_ExCutEntry); message 0x51 updates only an entry
                                           in the same state */
    int orderSlot;                      /* 0x08, the send slot of the order message for the entry (cm_OrderAttack's
                                           result), -1: none; cm_GetSlotState tells whether it is still pending */
    unsigned char b0c;                  /* 0x0c, cleared in the record itself before an order copies it (cm_OrderAttack) */
    char pad0d[3];
    union {
        struct {
            int stateArg;               /* 0x10, in state 2 message 0x51 also has to match it; in state 2 the netId of
                                           the partner the fighter turns towards (cm_UpdateSceneChars), 0 for an
                                           attack on a ground cell (cm_OrderAttack); in states 1 / 3 / 7 with x the
                                           cell walked to (cm_RunCutOrders) */
            int x;                      /* 0x14, in state 2 the ground cell attacked (cm_OrderAttack: weapon kinds 0x15e .. 0x176) */
            int z;                      /* 0x18 */
        };
        char dummy[0xc];                /* 0x10, in states 4 / 6 the name of the dummy object gone to (cm_RunCutOrders) */
    };
    unsigned char held;                 /* 0x1c, cm_OrderAttack sets it for the formation types 1 .. 3 (AICHAR.formation);
                                           reasoned name: the fighter holds its place */
    unsigned char turn;                 /* 0x1d, in state 2: nonzero, the fighter turns towards stateArg (cm_UpdateSceneChars);
                                           the target is in reach (cm_RunCutOrders) */
    char pad1e[2];
    union {
        struct {                        /* state 2, as CMFIGHTSLOT (the entry is sent as the fight command's slot) */
            int kill;                   /* 0x20, the blow lands */
            int damage;                 /* 0x24 */
            int health;                 /* 0x28 */
        };
        struct {                        /* state 4 */
            int cellX;                  /* 0x20, the cell walked to near the dummy */
            int cellZ;                  /* 0x24 */
        };
        struct {                        /* state 6 */
            unsigned char storeX;       /* 0x20, the cell of the goods store walked to */
            unsigned char storeZ;       /* 0x21 */
            unsigned char storeStep;    /* 0x22, 0 to the dummy, 1 to the store, 2 back */
        };
    };
} CMCUTENTRY;

typedef struct G17CUTENTRYMSG {         /* 0x34 bytes, the data of messages 0x50 / 0x51 (cm_SendCutEntry / cm_SendCutEntryUpdate
                                           send, the handlers 0x4cc1b0 / 0x4cc270 store the entry) */
    int playerId;                       /* 0x00, the player whose cutscene record (CMCUTINFO.playerId) it is */
    CMCUTENTRY entry;                   /* 0x04, the character's entry, copied whole */
    int fightId;                        /* 0x30, the sender's *cm_CurrentFight (-1: none); the receiver drops a message
                                           whose fight is not its player's AICHAR.partyId */
} G17CUTENTRYMSG;

typedef struct CMCUTINFO {              /* 0x35c bytes, cm_CutInfos[16]: a player's cutscene (fight) record, received
                                           whole by cm_ExSendCutInfo */
    int unk00;                          /* 0x00 */
    int ready;                          /* 0x04, "Ready" in cm_ExSendCutInfo's trace */
    int valid;                          /* 0x08, "Valid" */
    int recvFlag;                       /* 0x0c, cleared when the record is received (cm_ExSendCutInfo; reasoned guess: a
                                           flag of the local copy only, no other use known) */
    int playerId;                       /* 0x10, "PlayerId" */
    char text[0x80];                    /* 0x14, in a session: the member's answer text (shown by 0x4e37a0, cleared by the member callbacks) */
    union {
        CMCUTENTRY entries[16];         /* 0x94, of a fight */
        struct {                        /* 0x94, of a council session (the member callbacks cm_SessionAskPlayer / ai_Unk4dae90,
                                           cm_RunCouncilSession counts them) */
            int answer;                 /* 0x94, mode 1: 0 yes, 1 no, 2 abstain; mode 2: the netId voted for; -1 none */
            int click;                  /* 0x98, how it was given (1 / 2 pick the remark texts) */
            int observer;               /* 0x9c, 1: a present member without a vote (no answer is asked) */
        } session;
    };
    char pad354[8];
} CMCUTINFO;
typedef char cm_AssertCMCUTINFO[sizeof(CMCUTINFO) == 0x35c ? 1 : -1];

typedef struct CMSESSIONREQ {       /* 0x1c bytes, the request of a council session vote that cm_RunSceneParty hands to the
                                       member callbacks (cm_SessionAskPlayer) */
    int mode;                       /* 0x00, 1: deposition, 2: election */
    unsigned char office;           /* 0x04, the office's index (AMTREC.idx) */
    char pad05[3];
    int kind;                       /* 0x08, the office kind */
    int ids[4];                     /* 0x0c, mode 1: the holder and the challenger, mode 2: the applicants (-1 none) */
} CMSESSIONREQ;
typedef char cm_AssertCMSESSIONREQ[sizeof(CMSESSIONREQ) == 0x1c ? 1 : -1];

typedef struct G17CUTMSG {              /* 8 bytes, the data of message 0x26 ("cm_ExSendCutInfo", handler 0x4c9170; the
                                           record itself follows as the joined long message cm_LongMsg) */
    int playerId;                       /* 0x00, the player whose cutscene record (CMCUTINFO.playerId) it is */
    int fightId;                        /* 0x04, the fight it belongs to (*cm_CurrentFight) */
} G17CUTMSG;

typedef struct G17REMOVEMSG {           /* 12 bytes, the data of message 0x2b (handler 0x4c9450) */
    int holder;                         /* 0x00, the id of the building, character or object holding it (gm_FindById) */
    int objId;                          /* 0x04, of a building: the id of its object whose list holds it (gm_Query key 1),
                                           -1: the building's own object list */
    int entry;                          /* 0x08, what gm_RemoveOwned removes */
} G17REMOVEMSG;

typedef struct G17PLANTMSG {            /* 0x11 bytes, the data of message 0x2c (handler 0x4c9520): a plant of a building's
                                           vegetation map (rd.h G15PLANT) */
    int id;                             /* 0x00, the plant id (veg_NextId) when connected */
    int bldId;                          /* 0x04, the building */
    unsigned char x;                    /* 0x08, cell */
    unsigned char z;                    /* 0x09 */
    unsigned short type;                /* 0x0a, G15TREE.id */
    unsigned char fall;                 /* 0x0c, veg_SetPlant's fall model */
    int fallId;                         /* 0x0d, G15PLANT.fallId: set with fall, else the fall model to undo (-1 none) */
} G17PLANTMSG;

typedef struct G17SYSMSG {              /* 0x81 bytes, the data of message 0x20 (the system message, 0x4c4db0; its handler
                                           cm_ExSysMessage 0x4c89b0) */
    unsigned char kind;                 /* 0x00, 0..0x14 */
    union {
        int arg[0x20];                  /* 0x01, the int arguments (unaligned) */
        unsigned char b;                /* 0x01, kinds 6 and 9: a byte argument */
        AITIME time;                    /* 0x01, kind 3: the game clock */
        struct {
            int slot;                   /* 0x01 */
            char name[0x7c];            /* 0x05 */
        } save;                         /* 0x01, kind 15: the save requested */
    };
} G17SYSMSG;

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

typedef struct G17ROUTEMSG {            /* 8 bytes, the data of message 0x68 (ai_Unk4c62a0, its handler 0x4cd220); the route
                                           points (rt.h RTPOINT, 0x40 bytes each) follow as the joined long message cm_LongMsg */
    int objId;                          /* 0x00, the cart (AIOBJ.objId, gm_FindObjectInst) */
    int count;                          /* 0x04, the points */
} G17ROUTEMSG;

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
typedef char cm_AssertCMLOBBYPLAYER[sizeof(CMLOBBYPLAYER) == 0x57 ? 1 : -1];

typedef struct G17LOBBYCHAT {           /* 0x48 bytes, a lobby chat line (message 0x5f kind 6) */
    int from;                           /* 0x00, the sender's id */
    int to;                             /* 0x04, the receiver's id, 0: everybody */
    char text[0x40];                    /* 0x08 */
} G17LOBBYCHAT;

typedef struct G17LOBBYMSG {            /* 0x5b bytes, the data of message 0x5f (the network lobby; its handler 0x4cd870) */
    int kind;                           /* 0x00, 0 the player list, 1 ready, 2 join, 3 leave, 4 a changed player, 5 (0x4cd820), 6 chat */
    union {
        unsigned int list[9];           /* 0x04, kind 0: the count, then the eight player ids (cm_LobbyList) */
        struct {
            int id;                     /* 0x04, the player */
            int value;                  /* 0x08, kind 1: the ready flag (CMLOBBYPLAYER.ready); kind 3: 1 sets cm_LobbyClosed */
        } state;                        /* 0x04, kinds 1 and 3: copied whole over the player's record head */
        CMLOBBYPLAYER player;           /* 0x04, kinds 2, 4 and 5: the player's record */
        G17LOBBYCHAT chat;              /* 0x04, kind 6 (cm_LobbySendChat) */
    };
} G17LOBBYMSG;

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
        G17GATEMSG gate;                /* 0x10, message 0x36 */
        G17HOMEMSG home;                /* 0x10, message 0x39 */
        G17UPGRADEMSG upgrade;          /* 0x10, message 0x3a */
        G17MODELMSG model;              /* 0x10, message 0x3e */
        G17SPAWNMSG spawn;              /* 0x10, message 0x49 */
        G17CHATMSG chat;                /* 0x10, message 0x4b */
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
        G17SYSMSG sys;                  /* 0x10, message 0x20 */
        CRIME crime;                    /* 0x10, message 0x22 */
        G17CUTMSG cut;                  /* 0x10, message 0x26 */
        G17CUTENTRYMSG cutEntry;        /* 0x10, messages 0x50 / 0x51 */
        G17SKILLMSG skill;              /* 0x10, message 0x5d */
        G17REMOVEMSG remove;            /* 0x10, message 0x2b */
        G17PLANTMSG plant;              /* 0x10, message 0x2c */
        G17TABLEMSG table;              /* 0x10, message 0x54 */
        G17FIGHTMSG fight;              /* 0x10, message 0x55 */
        G17ROUTEMSG route;              /* 0x10, messages 0x68 / 0x69 */
        G17LOBBYMSG lobby;              /* 0x10, message 0x5f */
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


#endif
