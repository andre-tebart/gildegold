/* ai.h's building record for the units that read buildings without the rest of ai.h (he.c): types only.
   ai.h includes it. */
#ifndef AI_BLD_H
#define AI_BLD_H
#pragma pack(push, 1)
typedef struct AIBLD {                  /* 0xa9 bytes, ai_Blds[256] */
    unsigned char type;                 /* 0x00, index into ai_BTypes */
    int id;                             /* 0x01 */
    union {
        char name[0x22];                /* 0x05 */
        struct {
            char padname[0x20];
            unsigned short masterId;    /* 0x25, the character id of the building's master (the AI looks its
                                           buildings up by it, am.c 0x497cf0; the upgrade message goes to it) */
        };
    };
    unsigned short owner;               /* 0x27, index into ai_Chars */
    unsigned short mainRoom;            /* 0x29, the room (scene record) the building's panel opens with: sv_UpdatePanel passes it to ai_TechTreeWindow where it passes *sv_PanelShown for a shown room (reasoned) */
    char pad2b[0xe];
    int budget;                         /* 0x39, the money the building may spend this round: main_EndRound spreads its owner's goods value (x 75) over the buildings by AIBTYPE.level (2 / 4 / 8 shares, capped) and copies it to the AICHAR.budget of the local jobs; ai.c buys up to it */
    int wageRate;                       /* 0x3d, the percent of the job wages its workers get (main_SettleRound pays wage * rate / 100; below 100 lowers their opinion of the owner, above raises it) */
    int workRate;                       /* 0x41, a percent setting whose distance from 100 moves the workers' opinion the other way (main_SettleRound; reasoned guess: the working hours) */
    char pad45[8];
    int roundSums[3];                   /* 0x4d, cleared at every round settlement (main_SettleRound); reasoned guess: per-round sums */
    char pad59;
    union {
        unsigned char flags5a;          /* 0x5a */
        unsigned short flags5aw;        /* 0x5a, read as a word: bits 0x3800 are cleared at the round settlement (cm_SendSetField(id, 0x5a, 2, 0, 0x3800)) */
    };
    char level;                         /* 0x5c, the building's level 0 .. 100 (renovation below a rolled
                                           threshold, 100 - level in the renovation message, am.c) */
    int objList;                        /* 0x5d, head of the building's object-instance list (gm_Query 0x4ffe90,
                                           released with gm_RemoveObject) */
    char *object;                       /* 0x61, its 3D object in the scene */
    union {
        int unk65;                      /* 0x65, moved by +-1 per event towards AIPOS.unk18 (ai.c 0x4836a0) */
        int playerSlot;                 /* 0x65, of a kind 0x47 building: its ai_PlayerSlots index (sv_OpenBuildingPanel) */
        unsigned char residents;        /* 0x65, of a home (AIBTYPE.kind 1): the characters whose bld2 it is, below the
                                           capacity of its ord kind record (cm_ExSetHome) */
        struct GVYARD *yard;            /* 0x65, of a kind 0x1d building: its graveyard record (gv.h, gv_AllocGraveyard) */
    };
    int unk69;                          /* 0x69, cleared for the kind 7 buildings at every round start (main.c 0x53b2e0) */
    char pad6d[4];
    int plants;                         /* 0x71, the 64 G15PLANT records (0x18 bytes each, rd.h) of the
                                           building's vegetation map (rd.c: veg_Fits / veg_FindPlantId) */
    char pad75[0x20];
    int smokeEffect;                    /* 0x95, the id of the running chimney smoke effect (evt_FindById), -1 none (sv.c 0x528490) */
    char workSet[0x10];                 /* 0x99, the work settings a hired worker takes over into AICHAR flags1b4 ..
                                           target (cm_ExHire); begins with flags (ls.c) */
} AIBLD;
#pragma pack(pop)
#endif
