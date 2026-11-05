/* ai.h's records other units need without the rest of ai.h: the game time (unk_4b39f0_types.h's G13MIS)
   and the offer row (f3gm.c's gm_Offers), the goods list AIRECIPE f3gm.c fills: types only. ai.h includes it. */
#ifndef AI_TYPES_H
#define AI_TYPES_H
#include "d3_types.h"                /* D3VECTOR */
#pragma pack(push, 2)
typedef struct AITIME {                 /* 14 bytes: the game clock ai_Time, the AI's last-run time ai_Update.last,
                                           the times of the events and messages (f3gm.c's clock functions) */
    int year;                           /* 0x0, the year (0x578 + it is the calendar year; % 4 the season) */
    unsigned short hour;                /* 0x4, the hour */
    int min;                            /* 0x6, the minute */
    int sec;                            /* 0xa, the second */
} AITIME;
#pragma pack(pop)

typedef struct AISTORE {                /* 24 bytes, a stock list per building, gm_Offers[] */
    struct AIBLD *bld;                  /* 0x00 */
    int market;                         /* 0x04, the market / stall object holding the goods (f3gm.c gm_CollectOffers) */
    int goodsObj;                       /* 0x08, the goods object of the offer (the f3gm.c note of 0x7e4b20) */
    int count;                          /* 0x0c, the supply: the stall's amount less a quarter of its stock times the goods' +0x1c, not below 0 */
    float price;                        /* 0x10, the goods price times the building's price factor; gm_CollectOffers sorts by it, highest first */
    int stored;                         /* 0x14, 1 when the building's store object (0xff) holds the goods */
} AISTORE;

typedef struct AIJOB {                  /* 0x14c bytes, a job / event record (he_Events, he_CreateEvent; he_FindEvent / he_NextEvent) */
    char kind;                          /* 0x00, the message type it was made from, 0: a free record */
    char pad01[3];
    int id;                             /* 0x04, the event id (ai_NextEventId at its creation) */
    unsigned short chr;                 /* 0x08, the index in ai_Chars of the character it is for (cm_ExAddCrime: of a
                                           kind 0x18 the witness told of the crime) */
    char pad0a[2];
    int netId;                          /* 0x0c, the netId of that character, -1: none (the message's 'to') */
    int from;                           /* 0x10, the message's fromId */
    int words14[12];                    /* 0x14, the message's 12 words from +0x10 */
    AITIME start;                       /* 0x44, the start time (sv_RefreshPanel measures the progress from it) */
    AITIME end;                         /* 0x52, the end time of the kinds whose length is fixed at the start (0x40, the building jobs 0x1e / 0x1f) */
    AITIME clock60;                     /* 0x60, of a running income event 3 its second clock (the alloc handler 0x568ae0
                                           sets it to the start clock with the due clock +0x52) */
    char pad6e[2];
    int stage;                          /* 0x70, the stage the job has reached (sv_RefreshPanel shows progress only from a kind's timed stage on) */
    struct HEEVSLOT *slot;              /* 0x74, of an event shown in the event bar its slot (he.h he_EventSlots), 0: none */
    unsigned char opts;                 /* 0x78, the event's options from its message: 2 posts it when due, 4 not started yet,
                                           8 shown in the event bar, 0x10 runs every frame (he.c) */
    char pad79[3];
    char *text;                         /* 0x7c, of a message box event 0x11 its text ("messagebox", cm.c), 0 at creation */
    int textSize;                       /* 0x80, the size of that text (ls.c's loader reads it) */
    int exSlot;                         /* 0x84, the cm slot cm_SendExSetHE gave the event (cm_GetSlotState asks it), -1: none */
    struct HEGFXINFO *gfx;              /* 0x88, the gfx info shown over its object (he.h, he_CreateGfxInfo), 0: none */
    union {
        int ids[8];                     /* 0x8c, the message's ids[] */
        int helpers[4];                 /* 0x8c, of a helper job 0x8c the characters on it, -1: a free place
                                           (ai_GypsyHelperWindow counts them) */
    };
    union {                             /* 0xac..0xeb: the three unions, or a kind's own words over them */
        struct {
            union {
                int unkac;                      /* 0xac, a building id */
                int bldId;                      /* 0xac, of the building events (0x1b, 0x1c, 0x1e, 0x1f, 0x40) their building's id
                                                   (he_AddEventGfxInfo shows their gfx info over it) */
                int partner1;                   /* 0xac, of a pair event (kinds 0x41 / 0x6f / 0x45, he_EndPersonEvents) the first partner's netId */
                int ownerCh;                    /* 0xac, of an event 0x24 / 0x32 the netId of the character it is for (ai_DecideChangeProfession
                                                   skips a character that has one; the promotion handlers find it with gm_FindChar) */
                union {
                    int id;                     /* 0xac */
                    struct {
                        unsigned short lo;      /* 0xac */
                        unsigned short hi;      /* 0xae */
                    } w;
                } u;
            };
            union {
                int unkb0;                      /* 0xb0 */
                int graveSize;                  /* 0xb0, of the graveyard order events 0x92 / 0x93 (the master exam at
                                                   a graveyard): the size of the grave the order names, an index into
                                                   ai_GraveNames (he.c 0x5729c0 passes it as the order's size) */
                int partner2;                   /* 0xb0, of a pair event the second partner's netId */
                int id2;                        /* 0xb0, a second id: a building (kind 100) or a character (kind 0x89) as
                                                   sv_SpeakPicked looks them up */
                int effectObj;                  /* 0xb0, of an effect event 0x76 the effect object's handle, 0: none yet (its
                                                   alloc handler 0x54fb50 clears it) */
                int dropSlot;                   /* 0xb0, of a promotion event 0x32 the cm slot handle gm_DropRank leaves, -1: none
                                                   (its alloc handler 0x55dd70 sets -1) */
                int slotOrder1c;                /* 0xb0, of a kind 0x12 slot order event (the type 0x12 message
                                                   ai_BuySlots posts): the number of first-kind slots it adds, added
                                                   to AIOBJ.lvl1c and checked against AIBTYPE.min0 */
            };
            union {                             /* 0xb4..0xe7: the words, or the kind-specific times sv_RefreshPanel measures the progress to / from */
                struct {
                    union {
                        int unkb4;                      /* 0xb4 */
                        int slotOrder1d;                /* 0xb4, of a kind 0x12 slot order event: the number of
                                                           second-kind slots it adds, added to AIOBJ.lvl1d and checked
                                                           against AIBTYPE.min1 (ai_BuySlots) */
                    };
                    int unkb8;                          /* 0xb8, a job-kind specific payload word: of a harvest job (0x28) the
                                                           plant it works (veg_FindPlantId, ai_SendJobOrders), else a counter / an
                                                           id / a news id (he.c per kind); no single role name */
                    char padbc[8];
                    int workTotal;                      /* 0xc4, of a building job 0xd the total that argc8 counts up to (sv_RefreshPanel: progress argc8 / workTotal) */
                    int argc8;                          /* 0xc8, a job-kind specific word (he.c: a random table entry, a task, an
                                                           object); a workshop's chimney smokes only while its job has none
                                                           (sv.c 0x528490) */
                    char padcc[4];
                    unsigned short in[3];               /* 0xd0, the workshop kinds */
                    unsigned short out[3];              /* 0xd6 */
                    int amount[3];                      /* 0xdc */
                };
                AITIME end6c;                   /* 0xb4, of a job kind 0x6c: the end time */
                int cartBld;                    /* 0xb4, of the job a cart runs (AIOBJ.job24): the building (sv_RefreshPanel prints its kind) */
                int id3;                        /* 0xb4, a third id: of a kind 0x1a the character sv_SpeakPicked looks up */
                struct { char padid4[0x10]; int id4; };       /* 0xc4, of a kind 0x18: the character sv_SpeakPicked looks up */
                struct { char padt8f[4]; AITIME end8f; };     /* 0xb8, of the kinds 0x8f / 0x16 and the building job 0x1c: the end time */
                struct { char padt61[0x10]; AITIME start61; }; /* 0xc4, of a job kind 0x61: the start of its eight hours */
                struct { char padt48[0x18]; AITIME end48; };  /* 0xcc, of a job kind 0x48 (stage 1): the end time */
                struct { char padsteps[6]; unsigned char steps; };  /* 0xba, of a next trial step event 0x5f its step count
                                                              (its alloc handler 0x5496f0 clears it) */
                struct { char padeff[0xe]; unsigned short effectGood; };  /* 0xc2, of an effect event 0x76 the Domizil table id
                                                              (ai_FindUsableGood) whose duration sets its end time end6c */
                struct { char padb8[4]; unsigned char rounds; };  /* 0xb8, of a trial event 0x5e the round counter (he.c 0x549430:
                                                              1 at the first round, the third one moves the event to the state 1) */
                struct { char padidbc[8]; int idbc; };      /* 0xbc, a kind-specific id: of a kind 4 a building (shown with an exclamation mark
                                                               while argc8 is set), of a tribute event 0x35 the collector's netId (he.c) */
                struct { char padbldd0[0x1c]; int bldd0; };  /* 0xd0, of a kind 0x1a the building id the blows are shown over (he.c 0x53e8b0) */
                struct { char padgc4[0x10]; int members[4]; };  /* 0xc4, of a group job 'C' (0x43): the netIds of its up to four
                                                                  members, -1 = free (sv_SendSelectedToBuilding) */
                unsigned char graveEntry;       /* 0xb4, of the graveyard order events 0x92 / 0x93 (the master exam at a
                                                   graveyard): the grave entry the order names (gv_OrderSetGraveHolder's
                                                   entry, a byte) */
                struct { char padidx8[8]; int meetStart; };     /* 0xbc, of the meeting event 0xa1 (0x574550): the index
                                                                  into meetClasses its room query starts at */
                struct { char padmeet[0xc]; unsigned short meetClasses[5]; };  /* 0xc0, of the meeting event 0xa1: the
                                                                  words its room query 0x574fe0 hands to the building query
                                                                  0x501190 as its class (key 5), picked by 0x574550 from
                                                                  7 / 0x16 / 0xf / 0xa */
            };
            char pade8[4];
        };
        struct {                        /* 0xac, of the reaper event ("Sensenmann"; alloc handler 0x5509f0, run handler 0x550b60:
                                           its object flies from building to building) */
            int padreaper;
            int reaperFrom;             /* 0xb0, the id of the building the reaper starts from */
            int reaperTo;               /* 0xb4, the id of the building it flies to */
            int reaperNext;             /* 0xb8, the ai_Blds index the search for the next target starts at */
            int reaperStep;             /* 0xbc, the step of that search (mod 0x100) */
            int reaperLeft;             /* 0xc0, the targets left to visit (reasoned: the run handler goes on while it is > 0) */
            int reaperPlaced;           /* 0xc4, 1 once the object was created: reaperPos / reaperRot hold where */
            struct D3OBJECT *reaper;    /* 0xc8, the reaper object, 0: none */
            char padreapercc[4];
            D3VECTOR reaperPos;         /* 0xd0, the object's position (D3OBJECT.pos) */
            char padreaperdc[4];
            D3VECTOR reaperRot;         /* 0xe0, its rotation angles (D3OBJECT.rotation) */
        };
        int stormBlds[16];              /* 0xac, of a storm event 0x50 the ids of the 16 buildings its alloc handler 0x5652b0
                                           collected, -1: none */
        struct {                        /* 0xac, of the kind 6 sales-tour event (its run handler 0x554320, the
                                           single-ware variant 0x554860) */
            int padac;                  /* 0xac, the tour's ware / kind word */
            int tourCounters[12];       /* 0xb0 .. 0xdc, the twelve ware counters it hands over one by one;
                                           ai_UpdateStaff counts the nonzero ones as staff already busy */
        };
        struct { int padgrp; union { int groups[6]; AITIME homeUntil; }; };  /* 0xb0, of a six-group event 0x7d its six figure groups (its alloc
                                                   handler 0x551d00: gm_Shuffled, each + 1); homeUntil over them: of
                                                   the send-home event 0x77 the time it ends (its init 0x54fe80: now
                                                   + the duration of the goods 0x178) */
        struct {                        /* 0xac, of a running income event 3 (its alloc handler 0x568ae0) */
            unsigned short incomeGood;  /* 0xac, the goods (an ai_Goods index) */
            char padinc[2];
            int incomeAmount;           /* 0xb0, the stock amount the run handler pays out: the goods' work time */
            float incomeAmountF;        /* 0xb4, the same as a float */
            int incomeBld;              /* 0xb8, the building whose group its workers join */
            char padinc2[8];
            float incomeRate;           /* 0xc4, (gm_MeanLiking - 0.5) * 0.25 */
            unsigned char incomeCounter; /* 0xc8, 0 at the start */
        };
        struct {                        /* 0xac, of the feast event (its run handler 0x5437c0), read when its
                                           building has no feast table object (AIOBJ.festGuests) */
            int festGuests[4];          /* 0xac, the guests' character ids */
            unsigned char festEssen;    /* 0xbc, the menu pick (AIGRP.essen) */
            unsigned char festMusik;    /* 0xbd, the musician pick (AIGRP.musik) */
        };
        struct {                        /* 0xac, of the master exam at a feast, kind 0x36 (its run handler 0x543ac0) */
            char examText[0x10];        /* 0xac, a text the news 0x1770 / 0x1771 print (reasoned: io_PrintText gets its
                                           address among the characters' ids) */
            int examAt;                 /* 0xbc, where: 0 the feast table object of the building e->from (its owner
                                           examines), 1 the feast event examFeast (its host examines) */
            int examFeast;              /* 0xc0, the id of that feast event (kind 0x37, he_FindEvent) */
        };
        struct {                        /* 0xac, of the courtship event 0x41 (partner1 / partner2, of different sex; its
                                           alloc handler 0x544630, run handler 0x544840, the picks drawn by 0x544550) */
            char padpair[0xc];
            char pairRound;             /* 0xb8, the rounds held, 0 at the start (the run handler counts it up) */
            char pairRounds;            /* 0xb9, the rounds it takes (0x5c6620 of the two partners; the news 0x1cf4
                                           tells pairRounds - pairRound) */
            unsigned char pairDrawn;    /* 0xba, 1 once a round drew new picks (the run handler), 0 at the start */
            unsigned char pairDone;     /* 0xbb, 1 once the partners are each other's relatives[0] and the party
                                           message is posted (the run handler), 0 at the start */
            unsigned char pairPick[3];  /* 0xbc, three of twelve shuffled values from half the first partner's
                                           level (gm_CharLevel) on */
            char padpairbf;
            int pairPrice[3];           /* 0xc0, per pick (pick + 1) * the partners' worths (gm_GetWorth, each at
                                           least 32000) / 640 */
        };
        struct {                        /* 0xac, of the patrol event 0x43 ("STREIFE_INIT"; alloc handler 0x545670, run
                                           handler 0x545840) */
            int patrolTarget;           /* 0xac, the id of the target building */
            int patrolStart;            /* 0xb0, the id of the building it starts from */
            int patrolWay[4];           /* 0xb4, up to four buildings of kind 6 within 950 of the target, in a random
                                           order, -1: none */
            int patrolGuards[4];        /* 0xc4, its guards (the run handler) */
            int patrolWait;             /* 0xd4, -1 at the start; the run handler waits while it is set */
            int patrolStep;             /* 0xd8, the index into patrolWay of the next way point, 0 at the start */
            unsigned char patrolRound;  /* 0xdc, the rounds walked, 0 at the start */
        };
        struct {                        /* 0xac, of the raid event 0x44 ("Razzia"; alloc handler 0x546100, run handler
                                           0x5461c0; the raiders are ids[]) */
            int raidTarget;             /* 0xac, the id of the building raided */
            int raidStart;              /* 0xb0, the id of the building the raiders start from and go back to */
            int raidSlot;               /* 0xb4, the cm send slot of the raid's fight party (cm_SendNewParty) */
            int raidParty;              /* 0xb8, the id of that party (cm_GetSlotValue of the slot; cm_PartyById) */
        };
        struct {                        /* 0xac, of the tribute event 0x35 (alloc handler 0x544070, run handler 0x544330;
                                           the collector's netId is idbc) */
            AITIME tributeEnd;          /* 0xac, the end of the collection: a day after its start */
            char padtrib[0xa];
            int tributeNext;            /* 0xc4, the ai_Chars index the next round starts at, random at the start */
            int tributeStride;          /* 0xc8, the step from one character to the next (mod 0x300), a random
                                           ai_CharWalkSteps entry */
            int tributeLeft;            /* 0xcc, the characters left to visit, 0x300 at the start */
            float tributeSkill;         /* 0xd0, the event character's skill 4 (gm_GetSkill), mixed with each visited
                                           character's to the relation change */
        };
    };
    union {
        unsigned char stormDue;         /* 0xec, of a storm event 0x50: set by its alloc handler 0x5652b0, the run handler
                                           damages the buildings once, then clears it */
        unsigned long reaperClock;      /* 0xec, of the reaper event: d3_Time when its object was placed / last moved */
    };
    unsigned char modes;                /* 0xf0, of a kind 0x11 bit 0x40: the game pauses while its event window is active (he_PickActiveEvent) */
    char flags;                         /* 0xf1, a flags byte (nonzero defers the trade message posting; the bits are unknown) */
    char padf2[0x5a];                   /* up to 0x14c: the rest of the message's words from +0x58 (0xac..0x14c) */
} AIJOB;

typedef char ai_AssertAIJOB[sizeof(AIJOB) == 0x14c ? 1 : -1];

typedef struct AIRECIPE {               /* 0xb8 bytes, a goods list: filled by gm_StallGoodsList / gm_BuildingGoodsList
                                           / gm_BuildingGoodsList17 (f3gm.c, which clear and fill it as 0x2e ints) */
    int n;                              /* 0x00, the capacity of the stall (its cap / lvl1c); ai.c walks the entries up to it */
    int count;                          /* 0x04, the number of entries filled */
    int max[16];                        /* 0x08, per entry its stock (gm_ListStock): ai.c takes max - amount as the room left */
    unsigned short item[16];            /* 0x48, per entry the goods id */
    int amount[16];                     /* 0x68, per entry its amount (gm_ListAmount; the object's count of a stall) */
    int pada8[4];                       /* 0xa8, gm_StallGoodsList: the sum of the stocks, the sum of the amounts, the goods'
                                           value (gm_ItemPrice at 100 times the count); all three fillers: [3] the stall
                                           object (an ORDOBJ *). One array: split into four fields, the declaration set
                                           flips VC6 tie-breaks in d3_engine.c / unk_575860.c */
} AIRECIPE;

#pragma pack(push, 1)
typedef struct AIOBJ {                  /* 0x43 bytes, an object (table behind ai_Objects): carts, stores, the
                                           goods in a store, a workshop's level object; what gm_Query finds */
    union {
        unsigned short id;              /* 0x00, index into ai_Goods */
        unsigned short type;            /* 0x00, 0x116: ... */
    };
    int objId;                          /* 0x02, the object's own id (0x500190 gives a new one; messages name the
                                           object by it, 0x4fea50 finds it by it) */
    int ownerId;                        /* 0x06, the id of the building / object / character holding it (gv.h ORDOBJ.ownerId) */
    int fightSide;                      /* 0x0a, a good shown as a fight scene's "WARE_" object: the id (AICHAR.id) of the
                                           fight side's leader holding it (cm.c: cm_CheckDeath hands it to the defender) */
    union {
        int unk0e;                      /* 0x0e */
        int count;                      /* 0x0e */
    };
    unsigned char fill;                 /* 0x12, how full the cart is in percent (msg.n = 100 - fill; below 0x21 a cart is free for a load, ai_MeisterTrade / ai_UnloadCarts) */
    char pad13;
    union {
        int unk14;                      /* 0x14, the object list it holds */
        int list;                       /* 0x14 */
    };
    struct D3OBJECT *obj;               /* 0x18, the scene object that shows it (sv_DrawItemNames draws its name above it) */
    union {
        struct {
            union {
                int unk1c;                      /* 0x1c, a building id / a state / a count */
                int bldId;                      /* 0x1c, of a cart (gm_Query kind 0x1d): the id of its building (main_EndRound looks it up with gm_BuildQuery(1, 1, id)) */
                int cnt;                        /* 0x1c */
                int reserve;                    /* 0x1c, of goods in a store: the quarters of the store's stock (gm_ListStock)
                                                   kept from buyers other than its owner (cm.c 0x4c70b0); 4 when created
                                                   there for a character of kind 6 / 7 */
                struct {
                    unsigned char lvl1c;        /* 0x1c, a workshop's levels, compared with AIENTRY need1c / need20 */
                    unsigned char lvl1d;        /* 0x1d */
                };
                struct {
                    int pad1c20;
                    unsigned int flags20;       /* 0x20, of a cart (kind 0x1d): bit 1 (2) is cleared for the carts of the current building when the selection resets (sv_ResetSelection) */
                };
                struct {
                    int pad1c20w;
                    int usesLeft;               /* 0x20, of a weapon item (kind 5 in a character's inventory): the uses left of
                                                   a kind 1 / 2 weapon (cm_CharWeapon refuses it at 0; the portrait box shows it) */
                };
                struct {
                    int pad1c24[2];
                    struct AIJOB *job24;        /* 0x24, of a cart: a job whose cartBld names a building (sv_RefreshPanel prints that building's kind; reasoned: the job the cart runs) */
                };
                int recs[3];                    /* 0x1c..0x27, of a gm_Query kind 300 object in a shop: three record ids for gv_EvidencePrice (0x4ab080 tries them in turn) */
                struct {                        /* of a room of a building (the objects of AIBLD.objList the enter-building driver walks) */
                    int locked;                 /* 0x1c, nonzero: the room's contacts open ai_UnlockRoomWindow, which clears it when paid */
                    int unlockPrice;            /* 0x20, the money ai_UnlockRoomWindow asks; cleared with locked */
                };
            };
            char pad28[2];
            int reportBld;                      /* 0x2a, of a spy report (gm_Query kind 0xca in a building): the id of the
                                                   building it reports on (ai.c 0x4817a0, he.c's raid 0x5461c0) */
        };
        struct {
            char pad1c29[0xd];
            struct RTPOINT *route;              /* 0x29, of a cart: its route, a list of rt.c's RTPOINT (rt_CopyPoints; message 0x68 sets it) */
        };
        int seats[4];                   /* 0x1c..0x2b, of a table (kind 0x12d in a building): the ids of the characters at its
                                           four seats, -1 free (message 0x54 'join' / 'leav' / 'cler', cm_SendTableRequest) */
        struct {                        /* of a feast table object (gm_Query kind 0x17 in a building, he.c 0x5437c0) */
            int festGuests[4];          /* 0x1c..0x2b, the guests' character ids, -1: none */
            unsigned char festEssen;    /* 0x2c, the menu pick (AIGRP.essen) */
            unsigned char festMusik;    /* 0x2d, the musician pick (AIGRP.musik) */
        };
    };
    char pad2e[8];
    unsigned char unk36;                /* 0x36 */
    unsigned char reportKnow;           /* 0x37, of a spy report: how much it knows, in percent (ai.c spies again below a
                                           random 50 .. 99; the round settlement lowers it by twice the building's rank and
                                           drops the report at 5 or less, main.c; the raid adds it to its chance) */
    char pad38[3];
    void *sceneObj;                     /* 0x3b, the scene character (ch.h CHARACTER) that shows a cart */
    struct AIOBJ *next;                 /* 0x3f, the next object of its list (an inventory, a store) */
} AIOBJ;
#pragma pack(pop)

typedef struct AIMSG {                  /* 0xf8 bytes, given to cm_SendLongMessage */
    int unk00;
    unsigned char type;                 /* 0x04 */
    char pad05[3];
    int to;                             /* 0x08, the receiving player's netId (G8MSG's 'to'; every sender
                                           writes the building owner's netId) */
    int fromId;                         /* 0x0c, the sending building's id */
    int nIds;                           /* 0x10, how many of ids[] are set (msg.ids[msg.nIds]++) */
    int unk14;                          /* 0x14, only ever written 0 (hst.c: -1), no reader names it */
    char pad18[0x10];
    AITIME time;                        /* 0x28 */
    unsigned char flags;                /* 0x36, the message flags (1 / 2 / 9; he.md's message record) */
    char pad37;
    int ids[8];                         /* 0x38 */
    union {                             /* 0x58..0xf7: the kinds' words */
        struct {
            union {
                int arg[11];                    /* 0x58 */
                unsigned short itemId;          /* 0x58, the good a buy / move message is about (the 16-bit stores) */
                unsigned char thiefClass;       /* 0x58, of the thief message 0x16 (ai_SendFreeThieves): 3 when the
                                                   candidate's sk83 is below its sk82, else 2 */
                struct {                        /* the trade kinds */
                    int car;                    /* 0x58 */
                    int n;                      /* 0x5c */
                    int ch;                     /* 0x60 */
                    int dst[6];                 /* 0x64, 0..2 sold, 3..5 bought */
                    union {
                        unsigned short item[6]; /* 0x7c */
                        struct {
                            int pad7c[2];
                            int cost;           /* 0x84, the beat-up message 0x1a: the amount the decide callback
                                                   (ai_DecideBeatUp's worth estimate) puts on the brawl
                                                   (ai_ExecuteBeatUp); the trade messages set it 0 */
                        } s;
                    } u;
                };
                struct {                        /* the AI message 0x42 (ai_PostRandomMessages): the record a G14MSG view points
                                                   at (id at 0x58, type at 0x5c) */
                    int car;                    /* 0x58, the character the message is for (G14MSG.id) */
                    unsigned short type;        /* 0x5c, the message type 0 .. 0x44 (G14MSG.type) */
                } aiMsg;
                struct {                        /* the rank message 0x69 (gv_CheckRankUp) */
                    unsigned char rank;         /* 0x58, the new rank (AICHAR.b00d) */
                    char pad59[3];
                    int ch;                     /* 0x5c, the netId of the character who rose */
                } rankUp;
                struct {                        /* the pamphlet message 0x35 (0x4aa940) */
                    AITIME when;                /* 0x58, the game time it was sent */
                    char pad66[2];
                    int about;                  /* 0x68, the netId of the character it is about */
                    char pad6c;
                    unsigned char variant;      /* 0x6d, a random text variant 0..0x1c */
                } pamphlet;
                struct {                        /* the tavern message 0x2b (he_PostEvent2b, one per tavern building) */
                    int pad58;
                    unsigned char mode;         /* 0x5c, posted 0; the event keeps it at +0xb0 (a reasoned guess: a mode byte, its
                                                   reader is not read yet) */
                } tavern;
                struct {                        /* the grave lease offer 0x92 to a player (gv_GraveyardTurn; he_Unk5728d0 answers it with gv_OrderExtendGrave) */
                    int bld;                    /* 0x58, the graveyard building's id */
                    int size;                   /* 0x5c, the grave size */
                    unsigned char entry;        /* 0x60, the entry of that size */
                    char pad61[3];
                    int price;                  /* 0x64, the lease price (gv_LeasePrice) */
                    int years;                  /* 0x68, the years the lease is extended by (10) */
                } lease;
                struct {                        /* the graveyard work 0x8d on a grave (ai_GraveCare; he_HasGraveWork finds its event) */
                    int pad58;
                    int size;                   /* 0x5c, the grave's size (list) */
                    unsigned char index;        /* 0x60, its index in that list */
                } graveWork;
                struct {                        /* the study message 0x5f ("Studium", ai_ExecuteStudy) */
                    int ch;                     /* 0x58, the netId of the character who pays */
                    int pad5c;
                    int child;                  /* 0x60, the child's value from the decide callback (s1->arg2) */
                    unsigned char subject;      /* 0x64, the offer chosen (s2->ref) */
                    unsigned char level;        /* 0x65, the offer's second value (s2->arg2) */
                } study;
            };
            union {
                int amount[3];                  /* 0x88, of the bought goods */
                struct {
                    char pad88;
                    char unk89;                 /* 0x89 */
                };
            };
            char pad94[8];
            unsigned char textFlags;            /* 0x9c, of a note with a text (cm_SendPartyNote): 0xc, 0xe with the party's building,
                                                   | 0x10 when a second text follows the first */
            unsigned char unk9d;                /* 0x9d */
            char pad9e[2];
            int unka0;                          /* 0xa0 */
            char pada4[0xc];
            unsigned char extraFlag;            /* 0xb0, 1 in the explosion message 0x4f (gv_ExplodeShot); the meaning is not known */
            char padb1[0x47];
        };
        struct {                        /* the build message 0xd (ai_ExecBuildBuilding 0x4a5320) */
            char padbuild58[0x28];
            float buildPos[4];          /* 0x80, the plot's world position (ai_ObjectPosition, a 0x10-byte vector) */
            float buildRot[3];          /* 0x90, the plot's angles (ai_ObjectAngle: [1] the turn) */
            char buildPath[0x5c];       /* 0x9c, the path of the building's model group ("...gebaeude/*gb_<NAME>.ogr");
                                           the size is reasoned: up to the record's end */
        };
    };
} AIMSG;

typedef char ai_AssertAIMSG[sizeof(AIMSG) == 0xf8 ? 1 : -1];

#endif
