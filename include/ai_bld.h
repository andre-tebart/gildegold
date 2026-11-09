/* ai.h's building record and building type for the units that read buildings without the rest of ai.h (he.c): types only.
   ai.h includes it. */
#ifndef AI_BLD_H
#define AI_BLD_H
#pragma pack(push, 1)
typedef struct AIWORKSET {              /* 0x10 bytes, a building's work settings (AIBLD.workSet), AICHAR.flags1b4 .. target */
    unsigned int flags;                 /* 0x0, the order flags (AICHAR.flags1b4; ls_ReadBuildings sets bit 0 for saves
                                           below 0x10043) */
    int budget;                         /* 0x4, AICHAR.budget */
    int prod;                           /* 0x8, AICHAR.prod */
    int target;                         /* 0xc, AICHAR.target, a building id or -1 */
} AIWORKSET;
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
    int saved2b;                        /* 0x2b, saved and loaded whole (ls_WriteBuildings); no reader read yet */
    unsigned char saved2f;              /* 0x2f, saved and loaded (ls_WriteBuildings); no reader read yet */
    int travel30;                       /* 0x30, he_AllocTransport sums it over the start and the goal building into
                                           HETRANSPORT.travel (reasoned guess: a travel weight; no other reader read yet) */
    unsigned char side;                 /* 0x34, of a foreign city (kind 'G') the side of the town map it lies at,
                                           0 .. 7 (lc.c ai_BuildTownMap's direction table: north, then clockwise) */
    int saved35;                        /* 0x35, saved and loaded (ls_WriteBuildings); no reader read yet */
    int budget;                         /* 0x39, the money the building may spend this round: main_EndRound spreads its owner's goods value (x 75) over the buildings by AIBTYPE.level (2 / 4 / 8 shares, capped) and copies it to the AICHAR.budget of the local jobs; ai.c buys up to it */
    int wageRate;                       /* 0x3d, the percent of the job wages its workers get (main_SettleRound pays wage * rate / 100; below 100 lowers their opinion of the owner, above raises it) */
    int workRate;                       /* 0x41, a percent setting whose distance from 100 moves the workers' opinion the other way (main_SettleRound; reasoned guess: the working hours) */
    int saved45;                        /* 0x45, saved and loaded (ls_WriteBuildings); no reader read yet */
    float priceFactor;                  /* 0x49, the factor on the goods price a trade cart pays when it buys here (he_RunTrade; a
                                           market (AIBTYPE.kind 0xa) asks 1.1 instead) */
    int roundSums[3];                   /* 0x4d, cleared at every round settlement (main_SettleRound); reasoned guess: per-round sums */
    char prodScroll;                    /* 0x59, the scroll position of the production window, in rows (lc.c ai_ProductionWindow restores and saves it) */
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
        char kindData[0x30];            /* 0x65 .. 0x95, the part the building's kind gives its meaning (ls_WriteBuildings
                                           saves it whole for the kinds without a writer of their own) */
        struct {
            union {
                struct {
                    union {
                        int unk65;                      /* 0x65, moved by +-1 per event towards AIPOS.unk18 (ai.c 0x4836a0) */
                        int victimId;                   /* 0x65, of the kidnapping room: the victim character id (lc.c; -1 none) */
                        int buyOutBase;                 /* 0x65, of a class 2 building: with buyOutExtra what its buy-out pays
                                                           half of, and set to that payment (lc.c ai_BuildingWindow; reasoned) */
                        int playerSlot;                 /* 0x65, of a kind 0x47 building: its ai_PlayerSlots index (sv_OpenBuildingPanel) */
                        int interest;                   /* 0x65, of a bank: the interest rate its owner sets, 5 .. 0x28 (lc.c ai_CreditReleaseWindow
                                                           sends it as the field delta 0x65) */
                        unsigned char residents;        /* 0x65, of a home (AIBTYPE.kind 1): the characters whose bld2 it is, below the
                                                           capacity of its ord kind record (cm_ExSetHome) */
                        struct GVYARD *yard;            /* 0x65, of a kind 0x1d building: its graveyard record (gv.h, gv_AllocGraveyard) */
                        struct AHMREC *ahm;             /* 0x65, of the alchemist's building: its alchemist record (ahm_types.h,
                                                           ahm.c's ahm_AllocAhmInfo) */
                        int hostage;                    /* 0x65, of a thieves' guild: -1 while it holds no kidnapped character (reasoned:
                                                           ai_CalcMeisterDiebe posts the kidnap event 0x3c only then, he_AllocEvent3c
                                                           cancels one otherwise) */
                    };
                    union {
                        int daySum;                         /* 0x69, a per-day sum (gv.h ORDBLD.daySum; gm_BuildingBalance adds it to a kind 7 building's income), cleared for the kind 7 buildings at every round start (main_BeginPlayerRound) */
                        int messengerFee;                   /* 0x69, of a bank: the money messenger's fee in percent, 3 .. 0xf
                                                               (lc.c ai_ExchangeFeesWindow sends it as the field 0x69) */
                        int buyOutExtra;                    /* 0x69, of a class 2 building: see buyOutBase (lc.c ai_BuildingWindow) */
                    };
                    int exchangeFee;                        /* 0x6d, of a bank: the exchange fee in percent, 3 .. 0xf (lc.c
                                                               ai_ExchangeWindow takes it of the exchanged value) */
                };
                struct {                        /* 0x65, of an inn (kind 5 / 0x16): the round's counters of the inn simulation
                                                   (he.c 0x54d260 adds to them per sale, he_InnReport reports and resets them) */
                    int takings;                /* 0x65, what the guests paid this round (the prices of the sales) */
                    short guests;               /* 0x69, the guests of the round */
                    short served;               /* 0x6b, the guests who bought (served / guests below 0.5: the bad report) */
                    int lastTakings;            /* 0x6d, the takings of the last round (he_InnReport copies them) */
                } inn;
            };
            union {
                int plants;                     /* 0x71, the 64 G15PLANT records (0x18 bytes each, rd.h) of the
                                                   building's vegetation map (rd.c: veg_Fits / veg_FindPlantId) */
                struct {                        /* 0x71, of a bank: the credit offers kept for the player (ORDCREDIT,
                                                   gv_types.h / gv.h ORDBLD.credits, 12 bytes each) */
                    int amount;
                    int term;
                    int interest;
                } credits[3];
            };
        };
    };
    int smokeEffect;                    /* 0x95, the id of the running chimney smoke effect (evt_FindById), -1 none (sv.c 0x528490) */
    AIWORKSET workSet;                  /* 0x99, the work settings a hired worker takes over into AICHAR flags1b4 ..
                                           target (cm_ExHire copies the 16 bytes; lc.c LCWORKSET) */
} AIBLD;

typedef struct AIBTYPE {                /* 0x24d bytes, ai_BTypes[] */
    unsigned char kind;                 /* 0x000, the building class (gm_BuildingClass; zero-extended at the reads) */
    char name[0x20];                    /* 0x001, the type's name: its models "gb_<NAME>[_A..].ogr" (cm_ExGebUpgrade,
                                           amt_ExecGebaeudeBauen), its ground plan "riss_<name>.bmp" (sv.c) */
    unsigned char extensions;           /* 0x021, how many room upgrades a building of the type takes (lc.c
                                           ai_RoomUpgradeWindow: the built and running ones count against it) */
    unsigned char nslots;               /* 0x022 */
    union {
        unsigned short slot[0x100];     /* 0x023, needed goods (bit 15 = flag) */
        struct {
            unsigned short objs[0x40];  /* 0x023, the objects the type can hold, goods ids (bit 15 = flag; f3gm.c 0x509300) */
            char pad0a3[0x100];
            unsigned char objClass[0x40];   /* 0x1a3, per object its effect class (0x509300 sums by it) */
            unsigned char objValue[0x40];   /* 0x1e3, per object its effect value */
        };
    };
    union {
        unsigned char jobBytes;           /* 0x223, the first of the six job ids of the kind (the bytes 0x223 .. 0x228,
                                            0-terminated: f3gm.c gm_PermissionBits scans them for a job) */
        unsigned char jobs[6];            /* 0x223, the six job ids (he_RunHire gives the hired character jobs[i]) */
    };
    char pad229[6];
    unsigned char title;                /* 0x22f, the title rank of the type's first job (f3gm_types.h GMBTYPE.title:
                                           f3gm.c's worker lists take the characters whose AICHAR.title is this) */
    union {
        unsigned char title2;           /* 0x230, the title rank of the type's second job (cm_SendMessage3d asks a worker of it) */
        unsigned char k230;             /* 0x230 */
    };
    unsigned char k231;                 /* 0x231 */
    unsigned char k232;                 /* 0x232 */
    char pad233[0xd];
    unsigned char min0;                 /* 0x240 */
    unsigned char min1;                 /* 0x241 */
    unsigned char max116;               /* 0x242, the limit of both slot levels for an object of type 0x116 (he_RunSlotOrder) */
    int v243;                           /* 0x243, a value the type's tooltip shows (ai_OpenBuildingTooltip, text 0x29) */
    union {
        unsigned char level;            /* 0x247, the type's level 1 .. 3 (am.h AIBTYPEX: a count (level)); main_EndRound weighs a building's budget share 2 / 4 / 8 by it */
        unsigned char cap247;           /* 0x247, read as unsigned (0x489cb0) */
    };
    unsigned char maxLevel;             /* 0x248, the highest level of the type's line (reasoned: ai_DecideUpgradeGuild builds the next type only while cap247 is below it) */
    char pad249[4];
} AIBTYPE;
#pragma pack(pop)
#endif
