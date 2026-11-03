/* ai.h's records other units need without the rest of ai.h: the game time (unk_4b39f0_types.h's G13MIS)
   and the offer row (f3gm.c's gm_Offers): types only. ai.h includes it. */
#ifndef AI_TYPES_H
#define AI_TYPES_H
#pragma pack(push, 2)
typedef struct AITIME {                 /* 14 bytes: the game clock ai_Time, the AI's last-run time ai_LastUpdateTime,
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
    char pad60[0x10];
    int stage;                          /* 0x70, the stage the job has reached (sv_RefreshPanel shows progress only from a kind's timed stage on) */
    struct HEEVSLOT *slot;              /* 0x74, of an event shown in the event bar its slot (he.h he_EventSlots), 0: none */
    unsigned char opts;                 /* 0x78, the event's options from its message: 2 posts it when due, 4 not started yet,
                                           8 shown in the event bar, 0x10 runs every frame (he.c) */
    char pad79[3];
    char *text;                         /* 0x7c, of a message box event 0x11 its text ("messagebox", cm.c), 0 at creation */
    int textSize;                       /* 0x80, the size of that text (ls.c's loader reads it) */
    int exSlot;                         /* 0x84, the cm slot cm_SendExSetHE gave the event (cm_GetSlotState asks it), -1: none */
    char pad88[4];
    union {
        int ids[8];                     /* 0x8c, the message's ids[] */
        int helpers[4];                 /* 0x8c, of a helper job 0x8c the characters on it, -1: a free place
                                           (ai_GypsyHelperWindow counts them) */
    };
    union {
        int unkac;                      /* 0xac, a building id */
        int partner1;                   /* 0xac, of a pair event (kinds 0x41 / 0x6f / 0x45, he_EndPersonEvents) the first partner's netId */
        int ownerCh;                    /* 0xac, of an event 0x24 the netId of the character it is for (ai_DecideChangeProfession skips a character that has one) */
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
        int partner2;                   /* 0xb0, of a pair event the second partner's netId */
        int id2;                        /* 0xb0, a second id: a building (kind 100) or a character (kind 0x89) as
                                           sv_SpeakPicked looks them up */
    };
    union {                             /* 0xb4..0xe7: the words, or the kind-specific times sv_RefreshPanel measures the progress to / from */
        struct {
            int unkb4;                          /* 0xb4 */
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
        struct { char padgc4[0x10]; int members[4]; };  /* 0xc4, of a group job 'C' (0x43): the netIds of its up to four
                                                          members, -1 = free (sv_SendSelectedToBuilding) */
    };
    char pade8[8];
    unsigned char modes;                /* 0xf0, of a kind 0x11 bit 0x40: the game pauses while its event window is active (he_PickActiveEvent) */
    char flags;                         /* 0xf1, a flags byte (nonzero defers the trade message posting; the bits are unknown) */
    char padf2[0x5a];                   /* up to 0x14c: the rest of the message's words from +0x58 (0xac..0x14c) */
} AIJOB;

typedef char ai_AssertAIJOB[sizeof(AIJOB) == 0x14c ? 1 : -1];

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
    union {
        int arg[11];                    /* 0x58 */
        unsigned short itemId;          /* 0x58, the good a buy / move message is about (the 16-bit stores) */
        struct {                        /* the trade kinds */
            int car;                    /* 0x58 */
            int n;                      /* 0x5c */
            int ch;                     /* 0x60 */
            int dst[6];                 /* 0x64, 0..2 sold, 3..5 bought */
            union {
                unsigned short item[6]; /* 0x7c */
                struct {
                    int pad7c[2];
                    int unk84;          /* 0x84 */
                } s;
            } u;
        };
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
} AIMSG;

typedef char ai_AssertAIMSG[sizeof(AIMSG) == 0xf8 ? 1 : -1];

#endif
