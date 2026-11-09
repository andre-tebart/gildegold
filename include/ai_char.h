/* ai.h's character record (ai_Chars) and its scene character view for the units that read characters without the
   rest of ai.h (he.c): types only. ai.h includes it. */
#ifndef AI_CHAR_H
#define AI_CHAR_H
#include "ai_bld.h"                  /* AIBLD */
#include "ai_types.h"                /* AIJOB */

typedef struct AITASK {                 /* the character's scene character (ch.h CHARACTER: scriptId, group, scriptGroup, scene) as ai.h reads it */
    char pad00[0x28];
    int scriptId;                       /* 0x28, the event script it runs, -1 = none (CHARACTER.scriptId) */
    int bldId;                          /* 0x2c, the building the character was sent to (f3gm.c 0x4fbd10; CHARACTER.group) */
    int scriptGroup;                    /* 0x30, the group of the script that created / moved it (CHARACTER.scriptGroup) */
    struct D3OBJECT *body;              /* 0x34, its body object (CHARACTER.obj) */
    char pad38[0x50];
    struct D3SCENE *scene;              /* 0x88, the scene it stands in (CHARACTER.scene) */
} AITASK;

typedef struct AICHAR {                 /* 0x21c bytes, ai_Chars[768] */
    unsigned short id;                  /* 0x000 */
    unsigned char kind;                 /* 0x002 */
    char pad003;
    unsigned int netId;                /* 0x004 */
    unsigned char health;               /* 0x008, 100 at creation (ahm.c), > 0 = alive (0x4adff0 'lives') */
    unsigned char sex;                  /* 0x009, 0 / non-0 (reasoned: 0x4a8ad0 asks the partner search for flags 0xf00502 / 0xf00501 by it, the other sex) */
    unsigned short w00a;                /* 0x00a, 0x10 for a raised dead person (gv_RaiseDead), a value of the order for the alchemist (ahm_CreateAlchemist) */
    unsigned char b00c;                 /* 0x00c, a kind the person tooltip names (text 0x486 + it, ai_OpenCharTooltip) */
    unsigned char b00d;                 /* 0x00d, 1 .. 2 (gv: kept in a grave as GVGRAVE.v30) */
    char pad00e[2];
    float f010;                         /* 0x010, start values set by the character creators (ahm_CreateAlchemist, gv_RaiseDead) */
    float f014;                         /* 0x014, the step gm_AgeCharacter ages f01c by; a raised dead person loses it daily (gv_DecayRaisedDead) */
    float f018;                         /* 0x018 */
    float f01c;                         /* 0x01c, a life value: 1000.0 at creation, aged by gm_AgeCharacter, the character dies when it is used up (f3gm.c 0x4fbe90, gv_DecayRaisedDead) */
    float f020;                         /* 0x020 */
    int v024;                           /* 0x024, a value ai.c decays every frame (sent as message field 0x24), 0 at creation */
    short w028;                         /* 0x028, 0 at creation */
    char pad02a[2];
    int v02c;                         /* 0x02c, the ailment counters: the diseases 2 .. 9 as bit fields (f3gm.c gm_CatchDisease), 0 at creation */
    union {
        char name[0x58];                /* 0x030 */
        struct {
            char first[0x10];           /* 0x030, the first name (gv: copied to the grave, GVGRAVE.first) */
            char family[0x10];          /* 0x040, the family name (GVGRAVE.family) */
            unsigned short w050;        /* 0x050, must equal the grave entry's GVENTRY.w08 to be buried there (gv_BuryCharacter) */
        };
        struct {
            char padname80[0x50];
            unsigned char skills[5];    /* 0x080, five values (gv: copied to the grave, GVGRAVE.traits) */
        };
        struct {
            char padname[0x51];
            unsigned char sk81;         /* 0x081, skills */
            unsigned char sk82;         /* 0x082 */
            unsigned char sk83;         /* 0x083 */
            unsigned char sk84;         /* 0x084 */
        };
        struct {
            char padname54[0x24];
            int ref54;                  /* 0x054, a reference word (the save adds the base 0x59a back,
                                           ls_ReadCharacters); its low byte picks a cart's texture set
                                           ('g' + it, ls_ReadSceneRecords) */
            unsigned char b058;         /* 0x058, 0 at creation (hmt_Create) */
        };
        struct {
            char padname5c[0x2c];
            int relatives[8];           /* 0x05c, the netIds of the character's relatives (gm_KillSpieler clears
                                           them when one dies, ai_DecideUseDynastyObject walks [3..7]); relatives[0] is never
                                           picked as a partner (ai_DecideOfficePartner) */
        };
        struct {
            char padname7c[0x4c];
            float f07c;                 /* 0x07c, set to 1.1 for the partner of sex 1 when a courtship ends in the
                                           party (he.c 0x544840, a float delta of 1.1 - it) */
        };
    };
    float mood[14][3];                  /* 0x088, 13 triples (a, b, value), the current one at 0x124 */
    unsigned char sel;                  /* 0x130 */
    char pad131;
    unsigned char act;                  /* 0x132, the current action */
    char pad133[0x31];
    unsigned char jobId;               /* 0x164 */
    char title;                         /* 0x165, the title rank (f3gm.c 0x4fda70 / 0x504840) */
    unsigned char officeKind;           /* 0x166, the kind of the office held, 0 = none (amt.h AMTCHARVIEW.officeKind;
                                           amt_KindColumn / amt_GetKind take it) */
    unsigned char prevOffice;           /* 0x167, the office kind held before the current one (amt.h AMTCHARVIEW.prevOffice) */
    unsigned char appliedOffice;        /* 0x168, the office kind applied for, 0 = none (amt.h AMTCHARVIEW.appliedOffice) */
    unsigned char secondOffice;         /* 0x169, the second office kind, the deputy one (amt.h AMTCHARVIEW.secondOffice) */
    char pad16a[2];
    AIBLD *bld;                         /* 0x16c */
    AIBLD *bld2;                        /* 0x170, the character's own building (the f3gm.c 0x4fdcb0 comment: its home), 0 when none */
    int v174;                           /* 0x174, 0 when hired (cm_ExHire); ai.c 0x487b40 wants it >= 4 with a title
                                           (reasoned: rounds in the job) */
    int inventory;                      /* 0x178, the character's goods inventory (a G8 object list of item ids,
                                           next at +0x3f): gm_Query finds a prototype kind in it, gm_SumSlotAmounts
                                           sums a need slot over it (am.c 0x4a5760) */
    AIJOB *job;                       /* 0x17c */
    unsigned char jobKind;              /* 0x180, the kind (first byte) of job, refreshed on load */
    char pad181[3];
    union {
        AITASK *model;                  /* 0x184, the character's model record; its +0x2c is the building the
                                           character was sent to (f3gm.c 0x4fbd10) */
        struct CHARACTER *scene;        /* 0x184, the same: the scene character (ch.h), as cm_UpdateHandObject reads it */
    };
    unsigned char selected;             /* 0x188, nonzero: the character is in the player's selection (sv_ClearSelection clears it, sv_HandleObjects sets it) */
    char pad189[3];
    int picture;                        /* 0x18c, the portrait picture (sv.c: the person panel, the member list) */
    int v190;                         /* 0x190, a sum the round change prints on the player's balance page (text 0x1eff, main.c 0x5384e0) */
    int money;                          /* 0x194, the character's money (amt.c) */
    int spent198;                       /* 0x198, the money paid for goods bought (cm.c 0x4c70b0 adds count * price, unless the
                                           goods go to an object of kind 0x1db / 0x1dc) */
    int wage;                           /* 0x19c, the wage the character was paid in the last round settlement (main_SettleRound sends it as a field delta) */
    int v1a0;                           /* 0x1a0, the money of sales from an object of kind 0x1dc / purchases into one of kind
                                           0x1db (cm.c 0x4c70b0) */
    int v1a4;                           /* 0x1a4, the same for sales from 0x1db / purchases into 0x1dc */
    int v1a8;                           /* 0x1a8, saved and loaded (ls_WriteCharacters); no reader read yet */
    int v1ac;                           /* 0x1ac, a scene object of the fighter (cm.c 0x4d1xxx puts it at the body, shows it and
                                           clears it; the fighter copy cm_AddFighter starts with 0) */
    unsigned char b1b0;                 /* 0x1b0, saved and loaded (ls_WriteCharacters); no reader read yet */
    unsigned char heldRounds;           /* 0x1b1, rounds the character is held: message 0x5b (cm.c 0x4ccb10) adds to it, each
                                           round adds 0xff (main.c); nonzero: the AI chooses no new action (ai.c
                                           0x47e360), the character takes no office (amt.c), the player's room click
                                           does not switch rooms (sv.c); reasoned (a jail term or the like) */
    char pad1b2[2];
    unsigned int flags1b4;              /* 0x1b4 */
    int budget;                         /* 0x1b8, money */
    union {
        int prodAll;                    /* 0x1bc, prod read as the whole int (main_SettleRound copies flags1b4 .. target as four ints) */
        unsigned short prod;            /* 0x1bc, the item the workshop makes */
    };
    union {
        int target;                     /* 0x1c0, a building id or -1 */
        int unk1c0;                     /* 0x1c0 */
    };
    unsigned char formation;            /* 0x1c4, the fighter's formation type 0 .. 5 (cm.c 0x4d3120 switches on it, 0x4d2e30 rates by it) */
    unsigned char b1c5;                 /* 0x1c5, a per-worker value am.c 0x49a390 averages over a building's workers (at least 6 with a random 0..3 added lets the AI use the good 0x161 there); the meaning is not known */
    char pad1c6[2];
    unsigned int flags1c8;              /* 0x1c8 */
    unsigned int flags1cc;              /* 0x1cc */
    int v1d0;                           /* 0x1d0, saved and loaded (ls_WriteCharacters); no reader read yet */
    char block1d4[0x10];                /* 0x1d4, saved and loaded as one block (ls_WriteCharacters); no reader read yet */
    float mood3Rate;                    /* 0x1e4, scales the base growth of mood need 3; message 0x1a keeps it in sync */
    int v1e8;                           /* 0x1e8, saved and loaded (ls_WriteCharacters); no reader read yet */
    unsigned int goodsMask;             /* 0x1ec, the classes of the Domizil goods in use (am.h / lc.c goodsMask: ai_UseGood sets them), cleared for every character at the round start (main_BeginPlayerRound) */
    int v1f0;                         /* 0x1f0, 0 at creation */
    union {
        char ownName[0x1c];             /* 0x1f4, the character's own name, empty: the job's name record
                                           (gm_CharNameRecord) */
        struct {
            char padOwnName[0x18];
            int partyId;                /* 0x20c, the scene party the character belongs to (sv_RunFrame passes it to
                                           cm_PartyById); reasoned: it overlays the last 4 bytes of ownName */
        };
    };
    int partner;                        /* 0x210 */
    signed char relationBias;           /* 0x214, added to the partner's relation memory of the character when the partner search advances (ai_FindPartner 0x47ddf0: the search advances while the sum is > 0); reasoned name */
    unsigned char b215;                 /* 0x215, a step counter of the partner search (ai_FindPartner: 0 when a partner is picked, advanced towards b216); reasoned */
    unsigned char b216;                 /* 0x216, the steps the partner search needs (ai_FindPartner: from the liking, clamped by ai_Difficulty); reasoned */
    unsigned char b217;                 /* 0x217, sent as field 0x218 when the partner search is done (ai_FindPartner) */
    unsigned char mask;                 /* 0x218, the action groups it may use */
    char pad219[3];
} AICHAR;

typedef char ai_AssertAICHAR[sizeof(AICHAR) == 0x21c ? 1 : -1];

#endif
