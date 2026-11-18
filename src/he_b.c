/* he_b.c -- the second object of the event system (0x568aa0..0x575860): the alloc / run handlers of the running
   income 3, the production, the searches 8 / 9, trade 0xb, research 0xc and construction 0xd events, the alchemist
   events 0x95 .. 0x98, the secret room 0x99 .. 0x9b, the graveyard and meeting events 0x88 .. 0x94, the gypsy /
   curse / trial / circus events 0x9d .. 0xa4 and the message filter. The records and views are he.c's (he.h).
   Its .data block starts with he_BuildingModels 0x627e98, right after he's last strings (0x568310's "Trainieren",
   "Heilen"): a plain datum after literals, a new object; its strings follow from 0x628a28 ("_NACHRICHTEN_HS_07",
   0x568be0's). The first function lies in 0x568310 (exclusive) .. 0x568be0; 0x568aa0 is a guess (the first after
   he's registration function). The name is ours.
   Open: the first function (bprobe: he_RunProduktion 122 -> 28 for any split from 0x568220 to 0x569210, no other change).
   Open: head: the prototypes of this unit's functions still sit in he.h (scattered among he's).
   split from he (rea 8cef7d5b, src/game/he.c); boundaries in decl/text.dcl */
struct AHMSLOT;
struct D3HMAP;
struct SBBANK;
struct SBSAMPLE;
union AHMGRID;
struct D3SM;
#include "d2_types.h"
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <d3d.h>
#include "he.h"
#include "he_b.h"
#include "hmt_types.h"
#include "d3.h"
#include "d3_quad.h"
#include "d3.h"
#include "d3_light.h"
#include "txt.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#define GAPDATA __declspec(align(1)) unsigned char
#define ai_PlagueMessageK ((void (*)(AICHAR *, AIBLD *))ai_PlagueMessage)  /* 0x5a9240 (lc.c: G8CHARREC *c): @original the caller here pushes two arguments (0x54a4ba..0x54a4bb, add esp,8): the character (read as the first) and the building (unused) */
#define ai_DomizilMessageK ((void (*)(AICHAR *, AICHAR *, void *, int))ai_DomizilMessage)  /* 0x5a9c30 (lc.c: G8CHARREC *to, G8CHARREC *c, G8OBJ *obj, int level; lc.c-local types the header cannot spell): @original the caller here pushes four arguments (0x56d492..0x56d495, add esp,0x10) */
#define cm_SendExSetHEI ((int (*)(int, char *))cm_SendExSetHE)  /* 0x4c4c60 (cm.h: unsigned char k): @original the callers here push the key as an int and read the return (byte-matched he_EndEventNow / he_AllocQuarrel / he_AllocInsultNews / he_RunMeeting) */


extern int ai_TransportWindow();
/* full rebuild: the symbols whose original addresses the code above spells literally */
extern void (*he_RunHandlers[5544])(unsigned char *);
extern const volatile unsigned __int64 gypsyFilter;  /* 0x5eed68, menu.c */
#define gypsyFilterValue (*(unsigned __int64 *)&gypsyFilter)  /* 0x5eed68, gypsyFilter read as a plain value (he_AllocSpying passes it by value, the high word loaded first) */
#include "menu_types.h"
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_5ff710[];

extern GAPDATA full_data_5f320c[];
extern int gm_PlayerRecords[688];
extern int d2_ClickedForm;
extern unsigned char ai_GameState;
extern GAPDATA full_data_5fe7d0[];
extern const int sv_PhaseMinutes[24];
extern GAPDATA full_data_60d0d4[];
extern struct G16TRACKHANDLES snd_TrackHandles;
extern int * d3t_Rain;
extern GAPDATA full_data_5f1890[];
extern int ai_PermsDirty;

extern unsigned char ai_GameState;
extern int snd_SystemBank;
extern const int sv_PhaseMinutes[24];
#include "sv_types.h"
extern float gm_DayStartHours[];
extern char main_GamePath[];  /* 0x624690, main.c: the game directory */
extern float gm_DayEndHours[];

extern unsigned char ai_GameState;
extern int gm_PlayerRecords[688];
#include "cm_types.h"
extern int d2_ClickedForm;
#include "mission_types.h"
#include "ahm_types.h"                /* AHMSLOT */
extern float gm_DayStartHours[];
extern float gm_DayEndHours[];
extern unsigned char he_Events[339968];
extern int d2_ClickedForm;
extern GAPDATA full_data_602568[];
extern const int sv_PhaseMinutes[24];
extern float gm_DayStartHours[];
extern float gm_DayEndHours[];
extern const float gm_MoodRanges[476];
extern const float gv_SaleShares[];
extern int gm_PlayerRecords[688];

struct AIPER; struct AMTREC; struct CHARACTER; struct CHACTION; struct EVTSCRIPT; struct G15PLANT; struct G11REC; struct AITIME;


#define U_I(a) (*(int *)(a))
typedef void (*HEALLOC)(AIJOB *ev);     /* an event type's alloc handler (registered with its run handler by he_SetEventHandlers) */
#define he_AllocHandlers (*(HEALLOC (*)[0xa7])((char *)&he_QuickList + 0x400))  /* 0x74df40 */

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_PickActionBuilding)(p, a, fl, b, c, f, o)
#define ai_PickActionBuildingI ((int (*)(struct AIPER *, int, __int64, int, float, int *))ai_PickActionBuilding)  /* 0x4a63f0 with lo an int: @original he_AllocBrawl passes its (null) home building there, the register as it is (push edi; a float parameter would convert it) */

#define AICHAR_FLAGS1C8 0x71  /* the ai_CharInts index of AICHAR.flags1c8 (ai.h, +0x1c8; ai_CharInts starts at +4) */
#define SLOT(i) ((int *)((unsigned)&he_EventSlots + (i) * 12))

/* one of the ten 16-byte slots of the walk event 0x7a at AIJOB +0xac (he_StartWalk clears them) */
typedef struct HEWALKSLOT {
    int chr;                            /* 0x0, the walking character's netId (gm_FindChar), -1 free */
    int target;                         /* 0x4, the building it walks to (gm_FindById's building) */
    unsigned char stage;                /* 0x8, the walk's stage counter */
    unsigned char state;                /* 0x9, flags: 1 counted, 4 done, 0x10 played, 0x20 a character with +0x1f0, 0x40 held */
    unsigned char step;                 /* 0xa, the step through ai_Blds (a random ai_RandomStrides entry) */
    unsigned char bldIndex;             /* 0xb, the ai_Blds index of the building */
    char holdHour;                      /* 0xc, the hour the character was held at, 0xff none (a char: he_StepWalk's test
                                           for 0xff never holds) */
    unsigned char pad[3];
} HEWALKSLOT;
typedef struct HEWALKS {                /* the walk event 0x7a's ten slots over AIJOB's union (its alloc handler
                                           he_StartWalk clears them, its run handler steps them) */
    char pad00[0xac];
    HEWALKSLOT slots[10];               /* 0xac, the walking characters */
} HEWALKS;
typedef struct HEEXAMBOX {              /* the master exam question 0x7e's words over AIJOB's union (from HEEXAMMSG) */
    char pad00[0xac];
    int chr;                            /* 0xac, the netId of the character asked */
    int examId;                         /* 0xb0, the id of the exam offer event 0x7f that asks (HEEXAMOFFER) */
} HEEXAMBOX;
typedef struct HEEXAMOFFER {            /* the exam offer event 0x7f's words over AIJOB's union (he_RunExamOffer) */
    char pad00[0xac];
    int chr;                            /* 0xac, the netId of the character it is for */
    int accepted;                       /* 0xb0, 0 at the start (he_StartExamOffer), 1 when the player said yes (he_RunExamQuestion) */
} HEEXAMOFFER;
typedef union HEEXAMMSG {               /* the message 0x7e he_RunExamOffer posts (AIMSG has no names at +0x58 .. for it) */
    AIMSG msg;
    struct {
        char pad00[0x58];
        int chr;                        /* 0x58, the netId of the character asked */
        int examId;                     /* 0x5c, the offer event's id */
        AITIME due;                     /* 0x60, the answer is due: now + 2 hours */
    } q;
} HEEXAMMSG;
typedef struct HECONVERT {              /* the conversion event 0x81's words over AIJOB's union (he_RunConversion) */
    char pad00[0xac];
    int left;                           /* 0xac, the characters still to turn */
    int rounds;                         /* 0xb0, the rounds (days) left */
    int from;                           /* 0xb4, the AICHAR.b00c kind turned (0 / 1) into the other */
} HECONVERT;
typedef struct HEDELAY {                /* the delay event 0x80's words over AIJOB's union (he_StartDue starts it,
                                           he_RunDelay counts the days down) */
    char pad00[0xac];
    int days;                           /* 0xac, the days still to wait, one counted off per day, 0 frees the event */
} HEDELAY;
typedef struct HESKILLBONUS {           /* the skill-bonus event 0x82's words over AIJOB's union (he_StartSkillBonus
                                           starts it, 0x552820 runs it): GMSKILLEV (f3gm_types.h) and the duration */
    char pad00[0xb8];
    int hours;                          /* 0xb8, the hours the bonus lasts (its due clock) */
} HESKILLBONUS;
typedef struct HEAPPLICANTS {           /* the applicants event 0x86's words over AIJOB's union (he_RunApplicants) */
    char pad00[0xac];
    int office;                         /* 0xac, the office kind applied for (amt_GetOpenOffice), 0 none */
    int maxKind;                        /* 0xb0, the highest office kind looked at (0x1b at a restart) */
    int applicant[2];                   /* 0xb4, the handles of the two created characters (amt_CreateApplicant), -1 none */
} HEAPPLICANTS;
typedef struct HEOFFICEAWARD {          /* the imperial office award event 0xa5's words over AIJOB's union (he_StartOfficeAward /
                                           he_RunOfficeAward) */
    char pad00[0xac];
    AITIME when;                        /* 0xac, the clock of the award (now, he_StartOfficeAward), shown in the news */
    char padba[2];
    int ch;                             /* 0xbc, the netId of the character who gets the office (gm_FindChar) */
    int office;                         /* 0xc0, the office kind 0x23 .. 0x25 (the voice and news by it) */
} HEOFFICEAWARD;
typedef struct HEWATCH {                /* the watch event 0x43's words over AIJOB's union (he_DetectAttack): its four
                                           characters and the attack it was set on */
    char pad00[0xc4];
    int guard[4];                       /* 0xc4, the netIds of its characters, -1 none */
    int attackId;                       /* 0xd4, the id of the attack event it is bound to, -1 free (the attack's
                                           run handler 0x566830 looks for it by that) */
} HEWATCH;
typedef struct HEDETECT {               /* he_DetectAttack's question and answer (he_RunRobbery builds it on its stack) */
    struct AIBLD *bld;                  /* 0x0, the building attacked */
    float range;                        /* 0x4, how near a watch character has to stand to see it (5000.0f) */
    AIJOB *ev;                          /* 0x8, the attack event */
    AIJOB *found;                       /* 0xc, the watch event that saw it, 0 at the start */
} HEDETECT;
typedef struct HESALE {                 /* the sale event 2's words over AIJOB's union (he_AllocSale / he_RunSale); the
                                           building is AIJOB.from */
    char pad00[0xac];
    unsigned short good;                /* 0xac, the good sold (ai_Goods index; its production time sets the due clock) */
    char padae[2];
    int obj;                            /* 0xb0, the object sold (its objId: gm_Query key 1 in the building's list) */
    int slot;                           /* 0xb4, the send slot of the sale (cm_SendExSellObjectH), asked in stage 2 */
    int notify;                         /* 0xb8, nonzero: a master's kind 1 character with flags1b4 bit 0 makes the news
                                           0x60b (reasoned: who sets it is not read yet) */
} HESALE;
typedef struct HEEV1F {                 /* the building event 0x1f's words over AIJOB's union (he_AllocEvent1f /
                                           he_RunEvent1f); the building is AIJOB.bldId */
    char pad00[0xb0];
    unsigned short good;                /* 0xb0, the good it makes (cm_SendExSellObject's goods, the news text 0x9a8 + 2 * good) */
} HEEV1F;
typedef struct HEBUYBLD {               /* the building purchase event 0x21's words over AIJOB's union (he_AllocBuyBuilding /
                                           he_RunBuyBuilding); the building is AIJOB.bldId */
    char pad00[0xb0];
    int price;                          /* 0xb0, the price offered (paid with cm_SendMoveGoodsUnchecked) */
    int buyer;                          /* 0xb4, the buyer's netId (gm_FindChar) */
} HEBUYBLD;
typedef struct HEEV24 {                 /* the event 0x24's word over AIJOB's union (he_AllocDueIsStart / he_RunEvent24); the
                                           character is AIJOB.ownerCh */
    char pad00[0xb8];
    int daysLeft;                       /* 0xb8, the runs left: counted down once per run, below 0 the event ends */
} HEEV24;
typedef struct HEDISMISS {              /* the dismissal event 0x31's word over AIJOB's union (he_AllocDueInOne /
                                           he_RunDismissal) */
    char pad00[0xac];
    int worker;                         /* 0xac, the netId of the worker dismissed (gm_FindChar) */
} HEDISMISS;
typedef struct HEEV3C {                 /* the kidnap event 0x3c's words over AIJOB's union (he_AllocEvent3c / he_RunKidnap) */
    char pad00[0xac];
    int chr;                            /* 0xac, the netId of the victim (gm_FindChar) */
    int target;                         /* 0xb0, the id of the building the gang goes to for the victim */
    int guild;                          /* 0xb4, the thieves' guild's id (gm_BuildQuery(1, 1, id)) */
} HEEV3C;
typedef struct HEEV39 {                 /* the event 0x39's words over AIJOB's union (he_AllocEvent39_3e / he_RunEvent39) */
    char pad00[0xac];
    int held;                           /* 0xac, the netId of the held character (AICHAR.heldRounds) the payment frees */
    int payer;                          /* 0xb0, the netId of the character asked to pay; kind 6 / 7 (a player) answers in
                                           the message box, any other decides by gm_LikingChance */
    int amount;                         /* 0xb4, the sum asked for */
} HEEV39;
typedef struct HEEV38 {                 /* the event 0x38's word over AIJOB's union (he_AllocEvent38 / he_RunEvent38); the
                                           building is AIJOB.from */
    char pad00[0xac];
    int amount;                         /* 0xac, divided by gm_PlayerFactor (1000) and times 20: the relation change */
} HEEV38;
typedef struct HEEV27 {                 /* the payment event 0x27's words over AIJOB's union (he_AllocEvent27 /
                                           he_RunEvent27); the building is AIJOB.bldId */
    char pad00[0xb0];
    unsigned char player;               /* 0xb0, the paying player's slot (ai_PlayerSlots: its name in the news, its
                                           money good paid) */
    char padb1[3];
    int amount;                         /* 0xb4, the amount paid */
} HEEV27;
typedef struct HEEXAMPASS {             /* the passed master exam event 0x26's words over AIJOB's union (he_AllocMasterExamPass /
                                           he_RunMasterExamPassed); the character is AIJOB.ownerCh */
    char pad00[0xb0];
    unsigned char job;                  /* 0xb0, the character's job (AICHAR.jobId) when the event first ran */
    unsigned char newJob;               /* 0xb1, the job it gets (cm_SendSetJob): job - 1; 0 until the first run sets both */
} HEEXAMPASS;
typedef union HESOLDMSG {               /* the data of the sys message 4 he_RunSale sends: an hour from now as AITIME, of
                                           which the building and the object overwrite the first 8 bytes (the
                                           original's order) */
    AITIME due;
    struct {
        int bld;                        /* 0x0, the building's id */
        int objId;                      /* 0x4, the sold object's id (AIOBJ.objId) */
    } sold;
    char data[0x7c];                    /* the 0x7c bytes cm_SendExSysMessage copies into its message */
} HESOLDMSG;
typedef struct HECUSTOMERS {            /* the customers event 6's words over AIJOB's union (he_AllocCustomersHire / he_RunCustomers);
                                           the building is AIJOB.from */
    char pad00[0xac];
    unsigned char kind;                 /* 0xac, the kind of the customers spawned (cm_SendMessage3d) */
    char padad[3];
    int count[12];                      /* 0xb0, the customers left, per kind (one is served per round) */
    int target;                         /* 0xe0, the objId the customer walks to, -1: the building's main object */
    int slot;                           /* 0xe4, the spawn's send slot, -1 none */
    int person;                         /* 0xe8, the netId of the spawned customer */
} HECUSTOMERS;
typedef struct HEHIRE {                 /* the hire event 7's words over AIJOB's union (he_AllocCustomersHire / he_RunHire); the building
                                           is AIJOB.from */
    char pad00[0xac];
    unsigned char job;                  /* 0xac, the index of the job in the building type's jobs[] */
    char padad[3];
    int count[1];                       /* 0xb0, the hires left (he_RunCustomers' loops over one count) */
    int target;                         /* 0xb4, the objId the hired character walks to; 0: the building's main room, -1: its 0xfd object */
    int slot;                           /* 0xb8, the spawn's send slot */
    int person;                         /* 0xbc, the netId of the hired character */
} HEHIRE;
typedef struct HERESEARCH {             /* the research event 0xc's words over AIJOB's union (its alloc 0x56b380, run handler
                                           he_RunResearch): a building orders the good it researches and hooks the object
                                           the order made */
    char pad00[0xac];
    int buyer;                          /* 0xac, the id of the building the good is ordered to (the message's buyer) */
    char padb0[4];
    int good;                           /* 0xb4, the goods prototype researched (f3gm.c GMJOB.research) */
    int objId;                          /* 0xb8, the objId of the object the order made (the reply's AIOBJ.objId) */
    int wbc;                            /* 0xbc, 0 at the allocation (no reader known) */
    int limit;                          /* 0xc0, the rounds waited for that object (0x20) */
    int rounds;                         /* 0xc4, the rounds since the order (one per minute) */
    int slot;                           /* 0xc8, the send slot of the order (cm_SendExSellObjectH) */
} HERESEARCH;
typedef struct HEBUILD {                /* the building event 0xd's words over AIJOB's union (he_AllocGebaeudeBauen and
                                           its run handler he_RunConstruction): the building type it builds and the position it
                                           searches the plot around */
    char pad00[0xb0];
    int bldType;                        /* 0xb0, the building kind id (ai_BuildingName's id) */
    char padb4[0x1c];
    unsigned char count;                /* 0xd0, the placements left (the object callback counts it down; reasoned) */
    char padd1[3];
    float pos[3];                       /* 0xd4, the position ai_FindNearestObject measures the plots from */
} HEBUILD;
typedef struct HETRANSPORT {            /* the transport event 15's words over AIJOB's union (he_AllocTransport / he_RunTransport):
                                           a cart driven from one building to another */
    char pad00[0xac];
    int cart;                           /* 0xac, the cart's objId (gm_FindById) */
    int start;                          /* 0xb0, the id of the start building */
    int goal;                           /* 0xb4, the id of the goal building (a kind 0xa goal of an escorted cart: the
                                           nearest one) */
    int travel;                         /* 0xb8, the two buildings' AIBLD.travel30 summed */
    char padbc[4];
    float speed;                        /* 0xc0, the cart's walk speed (its CHARACTER.walkSpeed after the driver's skill) */
    int at;                             /* 0xc4, the id of the building the cart is at (the start) */
    int mode;                           /* 0xc8, by gm_IsHomeType of start / goal: 3 neither, 2 the start, 1 the goal, 4 both */
    int escorts;                        /* 0xcc, the escort kind 0 .. 2 (the goods' value / 16000 + random 0 .. 1) */
    unsigned char started;              /* 0xd0, 1 once the leg's first round sent the cart off, 0 at the start */
    char padd1[3];
    int rounds;                         /* 0xd4, the rounds the cart has been under way (a leg ends after 6 at the latest) */
    D3VECTOR lastPos;                   /* 0xd8, the cart body's position at the last round (arrived: it did not move) */
    char pade4[4];
    int slotE8;                         /* 0xe8, -1 at the start (its run handler's slot) */
    int route;                          /* 0xec, the send slot of the cart's route (cm_SendCartRouteH), -1 at the start */
    unsigned char ambushed;             /* 0xf0, 1 once an ambush (he_RunAmbush) picked the transport, 0 at the start */
    unsigned char weatherStage;         /* 0xf1, 2: the cart slowed once more (he_SetCartSpeed) */
    unsigned char quiet;                /* 0xf2, nonzero: no arrival news */
    char padf3;
    int order;                          /* 0xf4, the id of the event that ordered the transport, 0 / -1 none */
} HETRANSPORT;
typedef struct HECARTREPAIR {           /* the event 16's words over AIJOB's union (he_AllocCartRepair / he_RunCartRepair); reasoned: a
                                           cart repair */
    char pad00[0xac];
    int cart;                           /* 0xac, the cart's objId (gm_FindById) */
    int points;                         /* 0xb0, the points the cart's byte +0x12 rises by */
    int target;                         /* 0xb4, that byte after the work, 100 at most */
    int per;                            /* 0xb8, the minutes per point */
    char padbc[4];
    AITIME done;                        /* 0xc0, the start + per * points minutes */
    char padce[0xa];
    int notify;                         /* 0xd8, nonzero: the news 0x1c49 when done (he_RunCartRepair) */
} HECARTREPAIR;
typedef struct HEMSGBOX {               /* the message box event 17's words over AIJOB's union (he_AllocMessageBox / he_RunMessageBox); the
                                           text is AIJOB.text (after its NUL the text to speak) */
    char pad00[0xac];
    int pic;                            /* 0xac, the picture of the window (he_OpenEventSlot) */
    int bld;                            /* 0xb0, the quick jump's building id, -1: none */
    int obj;                            /* 0xb4, the quick jump's object in it (sv_SelectPanelObject), -1: the building */
    int party;                          /* 0xb8, of a cutscene box: the party it shows (cm_PartyById) */
    char sel;                           /* 0xbc, sv_SelectPanelObject's third argument (written by it) */
    char padbd[0x2f];
    int voice;                          /* 0xec, the spoken text's sample (snd_QueueVoice), 0: none */
    unsigned char flags;                /* 0xf0, 1 big, 2 quick jump, 4 cutscene, 8 activate, 0x10 speak, 0x20 history, 0x40 help */
} HEMSGBOX;
typedef struct HESLOTORDER {            /* the slot order event 18's words over AIJOB's union (he_AllocSlotOrderStore / he_RunSlotOrder; AIJOB's
                                           slotOrder1c / slotOrder1d) */
    char pad00[0xac];
    int obj;                            /* 0xac, the objId of the workshop object (gm_FindById) */
    int add1c;                          /* 0xb0, the first-kind slots still to add (AIOBJ.lvl1c) */
    int add1d;                          /* 0xb4, the second-kind slots still to add (AIOBJ.lvl1d) */
} HESLOTORDER;
typedef struct HESTOREEXT {             /* the store extension event 19's words over AIJOB's union (he_AllocSlotOrderStore /
                                           he_RunStoreExtension) */
    char pad00[0xac];
    int obj;                            /* 0xac, the objId of the store object (gm_FindById) */
    int rounds;                         /* 0xb0, the extension rounds still to run, one capacity unit each */
} HESTOREEXT;
typedef struct HEHERBS {                /* the herb gathering event 20's words over AIJOB's union (he_AllocHerbGathering /
                                           he_RunHerbGathering); the gatherers' netIds are AIJOB.ids[] */
    char pad00[0xac];
    unsigned char n;                    /* 0xac, the gatherers in AIJOB.ids[] */
    char padad;
    unsigned short good;                /* 0xae, the good gathered (gm_FreeCapacity; its name is the text 0x9a8 + 2 * good), 0: none */
    int room;                           /* 0xb0, the object (room) of the event's building the gatherers come back to
                                           (cm_SendChrGotoBuilding's objId, the news' quick jump) */
    int field;                          /* 0xb4, the id of the nearest building of type 0xb they gather at */
} HEHERBS;
typedef struct HEHUNT {                 /* the animal hunt event 21's words over AIJOB's union (he_AllocAnimalHunt /
                                           he_RunAnimalHunt); the hunters' netIds are AIJOB.ids[] */
    char pad00[0xac];
    unsigned char n;                    /* 0xac, the hunters in AIJOB.ids[] */
    char padad;
    unsigned short good;                /* 0xae, the good caught (cm_SendExSellObject; its name is the text 0x9a8 + 2 * good) */
    char padb0[4];
    int room;                           /* 0xb4, the object (room) of each hunter's building (AICHAR.bld) it goes to */
} HEHUNT;
typedef struct HESTUDY {                /* the study event 22's words over AIJOB's union (he_AllocStudy / he_RunStudy;
                                           reasoned: a stay at a building that raises one skill of the character) */
    char pad00[0xac];
    unsigned char skill;                /* 0xac, the skill raised: the character's byte 0x80 + skill, up to 252 */
    char padad[3];
    int chr;                            /* 0xb0, the netId of the character (gm_FindChar) */
    int room;                           /* 0xb4, the objId of its room in the building (an object of type 1), -1: none */
    AITIME until;                       /* 0xb8, the latest end: 6 units after the first due clock */
} HESTUDY;
typedef struct HEEV17 {                 /* the event type 0x17's words over AIJOB's union (he_AllocEvent17 / he_RunEvent17) */
    char pad00[0xb4];
    AITIME due;                         /* 0xb4, the due clock the allocation handler copies to AIJOB.end */
} HEEV17;
typedef struct HESABOTAGE {             /* the sabotage event 25's words over AIJOB's union (he_AllocSabotage / he_RunSabotage);
                                           AIJOB.from is the building the saboteur is hired at and goes back to */
    char pad00[0xac];
    int target;                         /* 0xac, the id of the building attacked */
    char padb0[0x12];
    unsigned char blasts;               /* 0xc2, the explosions set off so far (stage 5) */
    char padc3;
    int saboteur;                       /* 0xc4, the netId of the saboteur (the character the spawn request made), -1: none */
    int slot;                           /* 0xc8, the cm slot of the pending request (the spawn, the walk, the accusation), -1: none */
    int watch;                          /* 0xcc, the id of the watch event that caught the saboteur (HEDETECT.found, HEWATCH) */
    int fee;                            /* 0xd0, the money the event's character pays for it */
} HESABOTAGE;
typedef struct HESPYING {               /* the spying event 24's words over AIJOB's union (he_AllocSpying / 0x55a030) */
    char pad00[0xac];
    int target;                         /* 0xac, the id of the building spied on (gm_FindById; none: ai_PickActionBuilding
                                           picks one of the gypsyFilter kinds) */
    char padb0[4];
    int at;                             /* 0xb4, the id of the building the walk is at, -1 at the start */
    int b8;                             /* 0xb8, 0 at the start (reasoned: a counter of the run handler) */
    int cursor;                         /* 0xbc, the walk over the 0x100 buildings: the index it is at, random */
    int step;                           /* 0xc0, the walk's step, a random ai_RandomStrides entry */
    int victim;                         /* 0xc4, the netId of the character spied on */
    AITIME until;                       /* 0xc8, the end: 3 days (4 when order 0x26 answers 2) + HESPYING.days from now */
    char padd6[2];
    int spy;                            /* 0xd8, the cm slot of the spy's creation, then its netId; -1: none */
    char paddc[8];
    int fee;                            /* 0xe4, the money the event's character pays for the spy */
    int days;                           /* 0xe8, the days of spying asked for */
} HESPYING;
typedef struct HEBRAWL {                /* the brawl event 26's words over AIJOB's union (he_AllocBrawl / he_RunBrawl);
                                           AIJOB.from is the building the thug is hired at and goes back to */
    char pad00[0xac];
    int place;                          /* 0xac, a building id from the event's message: the place the accusations name
                                           (gv_Accuse), which must still exist when the watch's proof is sent */
    char padb0[4];
    int victim;                         /* 0xb4, the netId of the character beaten up (gm_FindChar; the run handler's
                                           money moves and accusation name it) */
    char padb8[0x10];
    int thug;                           /* 0xc8, the netId of the thug (the run handler takes it from the spawn slot), -1: none */
    int slot;                           /* 0xcc, the cm slot of the pending request (the spawn, the walk) */
    int meet;                           /* 0xd0, the id of the building the thug goes to: the victim's home, else one of
                                           its buildings, else one of the gypsyFilter kinds near it */
    int watch;                          /* 0xd4, the id of the watch event that caught the thug (HEDETECT.found, HEWATCH) */
    int fee;                            /* 0xd8, the money the event's character pays for it */
} HEBRAWL;
typedef struct HETHEFT {                /* the theft event 27's words over AIJOB's union (he_AllocTheft / he_RunTheft) */
    char pad00[0xac];
    int bld;                            /* 0xac, the id of the building robbed */
    int loot;                           /* 0xb0, the value taken (the news 0x17f4 prints it; reasoned: the run handler's
                                           chance of a second round is it over the building's equipment value) */
    char padb4[4];
    int rounds;                         /* 0xb8, 0 at the start (the run handler acts from 2 on) */
} HETHEFT;
typedef struct HEREPAIR {               /* the repair event 28's words over AIJOB's union (he_AllocRepair / he_RunRepair):
                                           the run handler raises the building's state (gm_ChangeBuildingState) by the
                                           minutes passed since AIJOB.clock60 */
    char pad00[0xac];
    int bld;                            /* 0xac, the id of the building repaired */
    int points;                         /* 0xb0, the state points still to repair (the run handler counts them down) */
    char padb4[4];
    AITIME until;                       /* 0xb8, the planned end: 10 minutes per point, less for a higher ranked character */
} HEREPAIR;
typedef struct HEROUTEREFS {            /* the building ids an ordering event names (HETRANSPORT.order): he_AllocTransport
                                           moves a replaced goal in them */
    char pad00[0xb0];
    int bld;                            /* 0xb0 */
    char padb4[4];
    int stopsB8[3];                     /* 0xb8 */
    int stopsC4[3];                     /* 0xc4 */
} HEROUTEREFS;
#define HE_WALK_PARTNERS1 0x20a0        /* the two partner masks of he_FillWalkSlot's ai_FindPartnersRandomWalk (bits not read yet) */
#define HE_WALK_PARTNERS2 0x402c20
#define HE_WALK_BUILDINGS 0x1a7f4c3a0i64 /* the building kinds of a walk (bit k: ai_BTypes kind k; he_StepWalk tests the same mask) */


int he_RegisterEventTypes22(void);
int he_RegisterEventTypes02();
int he_RegisterEventTypes03();
int he_RegisterEventTypes88();
int he_RegisterEventTypes95();
int he_RegisterEventTypes99();
int he_RegisterEventTypes9d();
int he_RegisterEventTypeA2();


#include "ls.h"
#include "f3init.h"
#include "d2.h"
#include "d3_anim.h"
#include "d3_engine.h"
#include "d3_int_io.h"
#include "d3_interface.h"
#include "d3_io_a.h"
#include "d3_par.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "err.h"
#include "gfx.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "rd.h"
#include "tb.h"
#include "tr.h"
#include "udp.h"
#include "d3_pose.h"
#include "sv_t.h"
#include "lc.h"
#include "ai_bld.h"
#include "f3gm_carry.h"
#include "ai_char.h"
#include "f3gm_types.h"              /* GMGOOD / GM_GOOD (the goods prototypes) */
#include "gv_types.h"
#include "ch.h"
#include "am_types.h"
#include "net.h"
#include "amt_types.h"
#include "rt_types.h"
#include "rt.h"
#include "d3_tools.h"
#include "d3_snd.h"
#include "sblib.h"
#include "vfs.h"
#include "sv.h"
#include "f3gm.h"
#include "cm.h"
#include "gv.h"
#include "ahm.h"
#include "am.h"
#include "amt.h"
#include "mission.h"
#include "rmsg.h"
#define rt_UnloadAtPointC ((void (*)(char *, char))rt_UnloadAtPoint)  /* 0x4bfa40 (rt.c: int pl): @original he_RunTransport pushes its char player slot unextended (0x556bb9) */
#define d2_SetObjValue2 ((void (*)(int, int))d2_SetObjValue)  /* 0x475cb0: @original he.c's two calls push only obj and the value (0x53dcd1, 0x53dd44) */
#define d2_PlaceMainWindowF ((void (*)(int, int))d2_PlaceMainWindow)  /* 0x475850 (d2.h: int view): @original he_OpenEventSlot pushes the placement flags too (0x53d7c3 push 3), which the tail jump leaves on the stack for d2_PlaceWindowScaled */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define cm_SendSpawnCharacterR ((int (*)(unsigned char, int, int, unsigned char, unsigned char, int, const char *))cm_SendSpawnCharacter)  /* 0x4c59f0: @original he_BeginPlague keeps what it returns (eax after the call: the cm slot of the new character), cm.c's definition is void; f an int: he_AllocBrawl pushes 0x154 whole */
#define gm_SumListAmountsL ((int (*)(void *))gm_SumListAmounts)  /* 0x509e30 with the list alone: @original he_AllocTheft pushes only the list (add esp, 4), as lc.c / sv.c call it */
#define util_RandomU ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original he_SpawnFounders and he_AllocTransport read the 16-bit result (mov cx, ax / and 0xffff) */
#define ai_PlayerSlotsT (*(GMPLAYERSLOT (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (cm.h: char[][0x2f4]) as the four player slots */
#define snd_CurrentTrack (*(char **)&snd_TrackHandles)  /* 0x6f0b6c, snd_TrackHandles.current (f3snd.h G16TRACKHANDLES, not included here): the music track playing */
#define he_PlayerCh (*(AICHAR **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) as the AICHAR it points to */
#define ai_TimeHourW (*(unsigned *)((char *)&ai_Time + 4))  /* 0x770ae4, ai_Time's hour with the low word of the minute int packed over it (0x565410 masks it to 0xffff) */
#define gm_NearestBuildingK ((int (*)())gm_NearestBuilding)  /* 0x50xxxx: @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
#define gm_LevelPayoutK ((int (*)())gm_LevelPayout)  /* 0x50a970: @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
typedef struct HEJOBTABLE {             /* the job table (object 0x12c) of a building's object list as he.c reads it: AIOBJ's
                                           +0x28 is an AIOBJ view of its own only here (adding a member to AIOBJ flips 8 matches) */
    char pad00[0x1c];
    int proofs[3];                      /* 0x1c, the crime ids (CRIME.id) the last robbery took from it, -1: none (he_RunDarkCorner) */
    int year;                           /* 0x28, the year the job was started (he_StartDarkCorner copies ai_Time's year into it) */
} HEJOBTABLE;
#define ai_CrimesT (*(CRIME (*)[0x200])&ai_Crimes)  /* 0x91f700, ai_Crimes (char[]) as the 0x200 CRIME records it holds */
typedef struct HESTAMMTISCH {           /* the inn meeting event 0x2a's words over AIJOB's union (AIJOB has no name at
                                           +0xb0: a field there flips d3_CloneObject's bytes, the unit's lead comment) */
    char pad00[0xac];
    int innId;                          /* 0xac, the inn building's id (gm_FindById) */
    AITIME meeting;                     /* 0xb0, when the meeting is held: the hour set to a random 0x13 .. 0x16 */
    char padbe[2];
    int state;                          /* 0xc0, 1 after the start, counted down by the run handler 0x542100 */
} HESTAMMTISCH;
typedef struct HEDUEL {                 /* the duel challenge's words over AIJOB's union (he_RunDuelChallenge): the challenge
                                           event 0x46 names the duel event, the duel event keeps two values for its news */
    char pad00[0xac];
    int duelId;                         /* 0xac, of the challenge 0x46: the id of the duel event (he_FindEvent(1, 1, id)) */
    char padb0[4];
    unsigned short newsArg1;            /* 0xb4, of the duel event: the first value of the challenge's news 0x1d6b */
    unsigned short newsArg2;            /* 0xb6, of the duel event: the third value of that news (both not read further) */
} HEDUEL;
typedef struct HEGESELLE {              /* the "RndGeselle" (a random journeyman) event's words over AIJOB's union (event 0x4a:
                                           its init 0x547ee0, run handler he_RunGeselle) */
    char pad00[0xac];
    int chrId;                          /* 0xac, the character the news goes to (gm_FindById's character) */
    int bldId;                          /* 0xb0, the building the journeyman is sent to (gm_FindById's building) */
    int bldArg;                         /* 0xb4, cm_SendChrGotoBuilding's third value with that building, also the
                                           news box's jump target's second word */
    unsigned short newsId;              /* 0xb8, the text id of the news */
    char padba[2];
    int slot;                           /* 0xbc, the cm slot of the journeyman's creation (cm_SendMessage3d); its value
                                           is the new character */
} HEGESELLE;
typedef struct HEFEEMSG {               /* the AI message 0x56 he_RunMasterExamPay posts (AIMSG has no names at +0x58 /
                                           +0x5c for it; a member added to AIMSG can flip other units' matches) */
    char pad00[0x58];
    unsigned char what;                 /* 0x58, 4 (reasoned: the kind of payment, its reader is not read yet) */
    char pad59[3];
    int amount;                         /* 0x5c, the amount paid */
} HEFEEMSG;
typedef struct HEEXAMPAY {              /* the master exam payment event's word over AIJOB's union (he_RunMasterExamPay) */
    char pad00[0xb0];
    int fee;                            /* 0xb0, the fee the message box names (5% of the character's worth), paid on yes */
} HEEXAMPAY;
typedef struct HEPAYMENT {              /* the payment event 0x56's words over AIJOB's union (its init 0x5482c0, run handler he_RunPayment) */
    char pad00[0xac];
    unsigned char wait;                 /* 0xac, the runs left before it pays, counted down */
    char padad[3];
    unsigned int amount;                /* 0xb0, the base amount: twice it or half of it is paid */
} HEPAYMENT;
typedef struct HEPUNISHMENT {            /* the trial-punishment events 0x5b (fine, he_RunFinePayment) and 0x5c (pillory,
                                           he_RunPilloryPunishment)'s words over AIJOB's union (their shared alloc handler
                                           he_AllocPunishment): the amount of each payment and the payments left */
    char pad00[0xac];
    int amount;                         /* 0xac, the amount of each payment (the message's first argument): taken from the
                                           character by the fine, the negated income of the pillory's AP event */
    int days;                           /* 0xb0, the payments left (the message's second argument), counted down */
} HEPUNISHMENT;
typedef struct HECOUNTDOWN {            /* the countdown event 0x51's word over AIJOB's union (its init he_AllocNextDay, run
                                           handler he_RunCountdown): counted down once per run, 0 frees the event */
    char pad00[0xac];
    unsigned char runs;                 /* 0xac, the runs left (the message's first word) */
} HECOUNTDOWN;
typedef struct HECOMMENT {              /* the history comment event 0x52's words over AIJOB's union (the KOMMENTARE text
                                           command ai_CommandComments posts message 0x52; its init he_AllocComment, run
                                           handler 0x548380) */
    char pad00[0xac];
    int entry;                          /* 0xac, the index into the history list (AIMSG.car) */
    int next;                           /* 0xb0, the next index (entry + 1) */
} HECOMMENT;
typedef struct HEFESTPAYOUT {           /* the festival payouts event 0x53's word over AIJOB's union (the FEST text command
                                           posts message 0x53; its init he_AllocCharPayouts, run handler he_RunCharPayouts) */
    char pad00[0xb0];
    int bld;                            /* 0xb0, -1 at the init (reasoned: no building; the run handler picks one per
                                           character and keeps no event-level word, so nothing reads it back) */
} HEFESTPAYOUT;
typedef struct HEOFFSPRING {            /* the offspring event's word over AIJOB's union (its run handler he_RunOffspring) */
    char pad00[0xac];
    float chance;                       /* 0xac, stage 0: the chance built up run by run; stage 1: the runs waited (to 3) */
} HEOFFSPRING;
typedef struct HETRIAL {                /* the trial event 0x5e's words over AIJOB's union (its alloc handler he_StartTrialEvent,
                                           run handler he_RunTrialEvent; rounds is AIJOB's) */
    char pad00[0xac];
    int teacher;                        /* 0xac, the netId of the character the trial is held with (gm_FindChar); the
                                           rounds' cost goes to its AP events, the news 0x1a9c to it */
    unsigned char job;                  /* 0xb0, the job the character gets at the end (cm_SendSetJob; its title
                                           group picks the two abilities raised, the job title text 0x130 + it) */
    char padb1[3];
    int cost;                           /* 0xb4, the cost of a round (he_AddApEvent's cost, negated) */
} HETRIAL;
typedef struct HETRIALSTEP {            /* the next trial step event 0x5f's words over AIJOB's union (its alloc handler
                                           he_StartNextTrialStep, run handler he_RunTrialSteps; steps is AIJOB's) */
    char pad00[0xac];
    int teacher;                        /* 0xac, the netId of the teaching character (gm_FindChar) */
    char padb0[4];
    int cost;                           /* 0xb4, the cost of a step (he_AddApEvent's cost, negated) */
    unsigned char skill[2];             /* 0xb8, the two skills each step raises (ai_ApplyTrialStep's index 0 .. 4) */
} HETRIALSTEP;
typedef struct HETRAINING {             /* the training event 0x60's words over AIJOB's union (its alloc handler
                                           he_StartTraining, run handler 0x549aa0) */
    char pad00[0xac];
    unsigned char base;                 /* 0xac, the most the skill rises: 0x15 */
    unsigned char range;                /* 0xad, the random amount taken off it (util_Random), 2 * (2 + skill / 42 + 1) */
    unsigned char skill;                /* 0xae, the skill trained (AICHAR.skills index) */
} HETRAINING;
typedef struct HESKILLMSG {             /* the AI message 0x6a he_RunTraining posts when a player's skill reaches a new
                                           level (AIMSG has no names at +0x58 / +0x5c for it) */
    char pad00[0x58];
    unsigned char skill;                /* 0x58, the skill (AICHAR.skills index) */
    char pad59[3];
    int level;                          /* 0x5c, its new level: the value / 42 */
} HESKILLMSG;
typedef struct HESKILLUP {              /* the skill level-up event 0x6a's words over AIJOB's union: HESKILLMSG's words as the
                                           event takes them over (+0x58 -> +0xac; its init he_StartSkillUp, run handler
                                           he_RunSkillUp) */
    char pad00[0xac];
    unsigned char skill;                /* 0xac, the skill (AICHAR.skills index) */
    char padad[3];
    int level;                          /* 0xb0, its new level: below 6 the news 0x16e6 + level, else the skill's master news */
    int variant;                        /* 0xb4, one of six texts per skill (random(6), its init) */
} HESKILLUP;
typedef struct HEBARD {                 /* the bard event's words over AIJOB's union (its init he_StartBard, run handler
                                           he_RunBard) */
    char pad00[0xac];
    unsigned short rec;                 /* 0xac, first the number of hmt records (hmt_RecordCount), then the record
                                           drawn from it (hmt_RandomRecord): the song's subject */
    unsigned short textOff;             /* 0xae, its text offset (hmt_RandomTextOffset) */
    char padb0[4];
    int script;                         /* 0xb4, the id of the bard's script (barde_create.esc), -1: none */
} HEBARD;
typedef struct HEBARDSCRIPT {           /* the bard's script as he_RunBard marks it: 'DrBh' over the first bytes of its name */
    unsigned int mark;                  /* 0x00 */
} HEBARDSCRIPT;
#define HE_BARD_MARK 0x68427244         /* 'DrBh' */
typedef struct HEPOPULATION {           /* the population event 0x6d's words over AIJOB's union (its init he_StartPopulation,
                                           run handler he_RunPopulation) */
    char pad00[0xac];
    int count;                          /* 0xac, the newcomers and the dead of this round */
    float chance;                       /* 0xb0, the chance of the next newcomer (or death) */
    float step;                         /* 0xb4, what each failed draw takes off chance */
    int slot;                           /* 0xb8, the cm slot of the newcomer being created, -1: none */
} HEPOPULATION;
typedef struct HELOVER {                /* the lover event 0x6f's words over AIJOB's union ("Geliebte"; its init he_StartLover,
                                           run handler he_RunLover) */
    char pad00[0xac];
    int keeper;                         /* 0xac, the netId of the character who keeps the lover (and pays) */
    int lover;                          /* 0xb0, the netId of the lover (flagged HE_LOVER_FLAG while it lasts) */
    int rounds;                         /* 0xb4, the days paid, 0 at the start */
    int pay;                            /* 0xb8, what the keeper pays a day, 2 at the start */
} HELOVER;
typedef struct HEDARKCORNER {           /* the "Dunkle Ecke" robbery event 0x74's words over AIJOB's union (its init
                                           he_StartDarkCorner, run handler 0x54f160) */
    char pad00[0xac];
    int carrier;                        /* 0xac, the cm slot of the carrier figure, then its id; -1: none */
    int victim;                         /* 0xb0, the id of the victim building */
    int amount;                         /* 0xb4, the amount */
    int target;                         /* 0xb8, the next target building, -1 at the start */
    int steps;                          /* 0xbc, the robbery steps left, 5 + random(4) at the start */
    int loot;                           /* 0xc0, the loot level 0 .. 3, 0 at the start */
    int cursor;                         /* 0xc4, the walk over the 0x100 buildings: the index it is at, random */
    int step;                           /* 0xc8, the walk's step, a random ai_RandomStrides entry */
} HEDARKCORNER;
#define HE_LOVER_FLAG 0x800000          /* AICHAR.flags1c8: has a keeper (he_StartLover sets it, he_RunLover clears it) */
typedef struct HEPLAGUE {               /* the plague event 0x59's words over AIJOB's union (he_BeginPlague, its run handler 0x54a020) */
    char pad00[0xac];
    int left;                           /* 0xac, counted down by the run handler at every infected target */
    int walk;                           /* 0xb0, the walk over the 0x100 buildings: the index it is at, random at the start */
    int step;                           /* 0xb4, the walk's step, a random ai_RandomStrides entry */
    char padb8[4];
    int carriers[4];                    /* 0xbc, the four carriers: the cm slots of their creation, then their ids */
    int target;                         /* 0xcc, the id of the building the carriers are sent to */
    int home;                           /* 0xd0, the id of the start building, where they go back after each target */
} HEPLAGUE;
typedef struct HEPLAGUEMSG {            /* the AI message 0x78 he_BeginPlague posts to every player character (AIMSG
                                           has no names at +0x58 .. +0x6c for it) */
    char pad00[0x58];
    int bld;                            /* 0x58, the start building's id */
    int target;                         /* 0x5c, the target building's id: the start building */
    int next;                           /* 0x60, -1 */
    int walk;                           /* 0x64, the event's walk index */
    int step;                           /* 0x68, the event's walk step */
    int left;                           /* 0x6c, the event's countdown */
} HEPLAGUEMSG;
typedef struct HEGANG {                 /* the pickpocket gang event 0x61's words over AIJOB's union (its alloc handler
                                           he_StartGang, run handler 0x54a7b0; the gang is AIJOB.ids[0..5]) */
    char pad00[0xac];
    int targets[6];                     /* 0xac, the buildings the members are sent to, -1 at the start */
    AITIME start;                       /* 0xc4, the start (AIJOB.start61): after 0x1e0 minutes the gang is collected */
    unsigned char next;                 /* 0xd2, the member the run handler handles next */
    char padd3;
    int leader;                         /* 0xd4, the netId of the gang's leader */
    unsigned char members;              /* 0xd8, the members the gang has (ids[] not -1) */
} HEGANG;
typedef struct HEPROTECTION {           /* the protection money event 0x62's words over AIJOB's union (its run handler
                                           0x54b1e0; the question event 0x63 he_RunProtectionAsk answers it); the
                                           collector is AIJOB.chr */
    char pad00[0xac];
    int collector;                      /* 0xac, the collector figure's netId */
    int victim;                         /* 0xb0, the victim building's id (its owner pays) */
    int amount;                         /* 0xb4, the protection money */
    int answer;                         /* 0xb8, -1 open, 0 refused, 1 paid, 2 asked a player (the event 0x63) */
    int home;                           /* 0xbc, the collector's building, which counts the money, -1: none */
    int homeRoom;                       /* 0xc0, the room object there the collector goes back to */
} HEPROTECTION;
typedef struct HEAUDIT {                /* the audit event 0x64's words over AIJOB's union (he_StartAudit / he_RunAudit);
                                           the one who ordered it is AIJOB.chr */
    char pad00[0xac];
    int inspector;                      /* 0xac, the inspector figure's netId */
    int bld;                            /* 0xb0, the audited building's id */
    int home;                           /* 0xb4, the building the inspector goes back to */
    int homeRoom;                       /* 0xb8, the room object there */
} HEAUDIT;
typedef struct HEGATHERING {            /* the patrol gathering event 0x65's words over AIJOB's union (he_StartGathering /
                                           he_RunGathering; its figures are AIJOB.ids[0..5]) */
    char pad00[0xac];
    char place[0x20];                   /* 0xac, the name of the scene object the figures gather at; an event 0x48 of
                                           another character at the same place is its opponent */
    int home;                           /* 0xcc, the building the figures go back to (the fight party's raidHome) */
    int homeRoom;                       /* 0xd0, the room object there */
    int party;                          /* 0xd4, the id of the fight party (cm_GetSlotValue), -1 at the start */
    int partySlot;                      /* 0xd8, the cm send slot of the fight party (cm_SendNewParty) */
    unsigned char dc;                   /* 0xdc, 0 at the start (no reader known) */
} HEGATHERING;
typedef struct HESIM {                  /* the words of the simulation events 0x66 (he_RunAccidentSim) and 0x70 over
                                           AIJOB's union (their alloc handler 0x54c4c0 clears them) */
    char pad00[0xac];
    int cursor;                         /* 0xac, the ai_Chars index the next run starts at */
    int step;                           /* 0xb0, the stride through ai_Chars, one of 16 numbers prime to 0x300; 0: not
                                           started (the next run picks one and a random cursor) */
    int done;                           /* 0xb4, the characters visited this round */
} HESIM;
typedef struct HEHINTEVENT {            /* the tutorial hint event 0x83's words over AIJOB's union (its alloc handler
                                           0x5679b0, run handler 0x567a80) */
    char pad00[0xac];
    int entry;                          /* 0xac, the index into he_Hints the run handler is at */
} HEHINTEVENT;
typedef struct HETRADEMSG {             /* the trade message 0xf's words over AIMSG (he_AllocTrade) */
    char pad00[0xa0];
    int jobId;                          /* 0xa0, the id of the event that posted it */
} HETRADEMSG;
typedef struct HETRADE {                /* the trade event 0xb's words over AIJOB's union (he_AllocTrade / he_RunTrade) */
    char pad00[0xac];
    int cart;                           /* 0xac, the objId of the trading cart (gm_FindObjectInst; its 0x1dd object holds
                                           the cargo and the slot count) */
    int bld;                            /* 0xb0, the id of the building it trades at, then the next one it goes to */
    int chr;                            /* 0xb4, the trading character (its budget pays the buys) */
    int sellBld[3];                     /* 0xb8, per sell slot the building that buys its goods, -1: done */
    int buyBld[3];                      /* 0xc4, per buy slot the building to buy at, -1: done */
    unsigned short sell[3];             /* 0xd0, per sell slot the good, 0: none */
    unsigned short buy[3];              /* 0xd6, per buy slot the good, 0: none */
    int buyAmount[3];                   /* 0xdc, per buy slot the amount wanted */
    int moveSlot;                       /* 0xe8, the cm send slot of the last goods move (cm_SendExSellObject), -1: none */
    int msgSlot;                        /* 0xec, the cm send slot of the move-on message 0xf, -1: none */
    unsigned char tries;                /* 0xf0, AIJOB.modes: the half-hour retries left for a buy that found nothing
                                           (4 from the alloc handler when the character exists) */
    char deferred;                      /* 0xf1, AIJOB.flags: nonzero defers the move-on message (and the cart check) */
} HETRADE;
typedef struct HEPRODUCTION {           /* the production event's words over AIJOB's union (he_AllocProduktion 0x5690c0,
                                           he_RunProduktion) */
    char pad00[0xac];
    unsigned short good;                /* 0xac, the good produced (an ai_Goods index) */
    char padg[2];
    char padb0[4];
    float amount;                       /* 0xb4, the work left on the current unit: starts at the good's work time
                                           (GMGOOD.workTime), worn down by the work minutes, one unit made per work time */
    float amount2;                      /* 0xb8, the same, set when a unit starts */
    int bld;                            /* 0xbc, the producing building's id */
    int store;                          /* 0xc0, the id of the object the goods are made in (kind 0x2a / 0x116: its list
                                           holds the inputs, cm_SendMakeGoods' target) */
    int slot;                           /* 0xc4, the cm send slot of the last make / order message (cm_GetSlotState), -1: none */
    int lacking;                        /* 0xc8, 1 once a shortage was reported (its news box event 0x11 is freed when the work resumes) */
    float rate;                         /* 0xcc, (the owner's mean liking - 0.5) * 0.25 */
    unsigned char counter;              /* 0xd0, the work minutes since the workers were last rated (every 60) */
    char padd1[3];
    float staff;                        /* 0xd4, the workers' mean factor (health, skill, place), 1.0 at the allocation */
} HEPRODUCTION;
typedef struct HESEARCH {               /* the search event 8's words over AIJOB's union (he_AllocSearchEvent / he_RunSearchEvent) */
    char pad00[0xac];
    unsigned short good;                /* 0xac, the good searched for (an ai_Goods index) */
    char padg[2];
    int bld;                            /* 0xb0, the id of the building the workers belong to */
    int store;                          /* 0xb4, the id of the object the found goods go to */
} HESEARCH;
typedef struct HESEARCH9 {              /* the search event 9's words over AIJOB's union (he_AllocSearch9 / he_RunSearch9) */
    char pad00[0xac];
    int bld;                            /* 0xac, the id of the building the workers belong to */
    int source;                         /* 0xb0, the id of the object searched (its list holds the kind 0x17 goods) */
    int store;                          /* 0xb4, the id of the object the found goods go to */
} HESEARCH9;
typedef struct HEVORKOMMEN {            /* the new-occurrence debug event 0xa's word over AIJOB's union (its run handler
                                           he_RunNewOccurrenceMsg) */
    char pad00[0xac];
    int good;                           /* 0xac, the goods index the bug text names (0x9a8 + 2 * good) */
} HEVORKOMMEN;
typedef struct HECONSTRUCTION {         /* the building construction event 0xd's words over AIJOB's union (its run handler
                                           he_RunConstruction, he_ShowConstruction) */
    char pad00[0xac];
    int payer;                          /* 0xac, the netId of the character paying for the building */
    int bldType;                        /* 0xb0, the building kind id (HEBUILD.bldType), sent with the message 10 */
    int carry;                          /* 0xb4, the price paid in stage 0; from stage 6 on the work minutes carried over
                                           to the next stage */
    int bld;                            /* 0xb8, the id of the building under construction */
    int script;                         /* 0xbc, the id of the event script playing (evt_FindById), -1: none */
    int minutes;                        /* 0xc0, the work minutes per construction stage */
    int stages;                         /* 0xc4, the last construction stage (AIJOB.workTotal) */
    int stage;                          /* 0xc8, the stage reached, -1: not started (AIJOB.argc8) */
    int slot;                           /* 0xcc, the cm slot of the payment / the building message (cm_GetSlotState) */
    unsigned char placing;              /* 0xd0, the "vg_" objects still being placed (HEBUILD.count) */
    char padd1[3];
    D3VECTOR pos;                       /* 0xd4, where the building stands (HEBUILD.pos) */
    char pade0[4];
    D3VECTOR rot;                       /* 0xe4, its rotation */
    char model[0x40];                   /* 0xf0, the building's group model file (the length reasoned: what fits in AIJOB) */
} HECONSTRUCTION;
typedef struct HEMISSIONCHECK {         /* the mission check event 0x4c's words over AIJOB's union (he_AllocMissionCheck / he_RunMissionCheck) */
    char pad00[0xac];
    int slot;                           /* 0xac, the cm slot of the system message 8 that tells the mission's owner, -1: none */
} HEMISSIONCHECK;
typedef struct HERECIPESEARCH {         /* the research event 0x95's words over AIJOB's union (he_AllocRecipeResearch / he_RunRecipeResearch) */
    char pad00[0xac];
    int alchemist;                      /* 0xac, the netId of the researching character */
    int recipe;                         /* 0xb0, the recipe's id (the first word of ahm_RandomRecipe's record) */
    int left;                           /* 0xb4, the research minutes left (ahm_ResearchMinutes at the start) */
    int total;                          /* 0xb8, the research minutes at the start */
    AITIME worked;                      /* 0xbc, the clock the minutes worked are counted from */
} HERECIPESEARCH;
typedef struct HEAHMRECIPE {            /* the first word of ahm_RandomRecipe's AHMGRID entry (ahm.h, not included here) */
    int id;                             /* 0x00, RCPID: the recipe's object id */
} HEAHMRECIPE;
typedef struct HEAHMDAY {               /* the alchemist's daily laboratory event 0x98's words over AIJOB's union (he_AllocAlchemistDay /
                                           he_RunAlchemistDay) */
    char pad00[0xac];
    int bonus;                          /* 0xac, nonzero: the next run adds a bonus to the alchemist record's value
                                           instead of the roll (0 at the start; he_LabIncident sets it to the mishap it ran) */
    int payloadB0;                      /* 0xb0, a kind-specific payload word the alloc clears to -1 (none); no handler of
                                           event 0x98 reads it, cm_SendExSetHE sends it in the event's data block */
    int payloadB4;                      /* 0xb4, the second such word, -1: none */
} HEAHMDAY;
typedef struct HEHAUNTING {             /* the graveyard owner's haunting event 0x88's words over AIJOB's union (the message's
                                           arg[0 .. 3]: am.c ai_ExecGraveyardOwner0, sv_t.c; he_AllocHaunting / he_RunHaunting) */
    char pad00[0xac];
    int home;                           /* 0xac, the id of the victim's building the haunter goes to */
    int victim;                         /* 0xb0, the netId of the character haunted */
    int haunter;                        /* 0xb4, the netId of the character who haunts (the one selected), -1: none */
    int paid;                           /* 0xb8, the money paid for it (the slider's amount) */
    int walkSlot;                       /* 0xbc, the cm slot of the haunter's walk, -1: none */
} HEHAUNTING;
typedef struct HEDECAY {                /* the graveyard owner's "breath of decay" event 0x89's words over AIJOB's union (the
                                           message's arg[0 .. 3]: am.c ai_ExecGraveyardOwner2, sv_t.c; he_AllocDecay / he_RunDecay) */
    char pad00[0xac];
    int home;                           /* 0xac, the id of the victim's building the agent goes to */
    int victim;                         /* 0xb0, the netId of the character it is against */
    int agent;                          /* 0xb4, the netId of the character who goes (the one selected) */
    int paid;                           /* 0xb8, the price (sv_t.c: 4) */
    int aged;                           /* 0xbc, 1 when the victim aged, 0 when it failed */
} HEDECAY;
typedef struct HESECRETROOM {           /* the secret room event 0x99's words over AIJOB's union (am.c ai_ExecSecretRoom0 .. 3
                                           post it; he_AllocSecretRoom / he_RunSecretRoom) */
    char pad00[0xac];
    int target;                         /* 0xac, the id of the building the agent goes to (of the way 3: the spied
                                           building of the spy hunted, or one ai_PickActionBuilding picks) */
    int home;                           /* 0xb0, the id of the secret room's building: the agent is spawned there and
                                           goes back to it */
    int victim;                         /* 0xb4, the netId of the character the deed is against */
    int agent;                          /* 0xb8, the netId of the agent ("ASSASSIN", spawned by he_AllocSecretRoom) */
    int way;                            /* 0xbc, 1 an attack, 2 an accusation, 3 a spy hunt ("Spy Killer"), 4 the domicile
                                           event (am.c's ways 1, 2, 3, 0) */
    int strength;                       /* 0xc0, of an attack the strength (he_SecretRoomAttack; the news 0x4007 + it) */
    int spyEvent;                       /* 0xc4, of a spy hunt the id of the spying event followed, -1: none */
    int hunting;                        /* 0xc8, of a spy hunt 1 while it follows a spy, 0 while it searches */
    int walkSlot;                       /* 0xcc, the cm slot of the agent's spawn, then of its walk; -1: none */
} HESECRETROOM;
typedef struct HECHILDLIST {            /* the children of a scene object, collected by he_CollectChild */
    D3OBJECT *root;                     /* 0x000, the object walked */
    D3OBJECT *items[0x40];              /* 0x004 */
    int count;                          /* 0x104 */
} HECHILDLIST;
typedef struct HETIPEVENT {             /* the tips event 0x84's words over AIJOB's union (its alloc handler 0x567b80,
                                           run handler 0x567c00) */
    char pad00[0xac];
    int entry;                          /* 0xac, the index into he_TipOrder the run handler is at */
} HETIPEVENT;
typedef struct HENEWWORKER {            /* the new-worker event 0x6e's words over AIJOB's union (its alloc handler 0x565850,
                                           run handler 0x565940) */
    char pad00[0xac];
    int slot;                           /* 0xac, the cm send slot of the hire message (cm_GetSlotState / cm_GetSlotValue) */
    char padb0[4];
    int bld;                            /* 0xb4, the id of the building that gets the worker (gm_FindBuildingById) */
    char padb8[4];
    int share;                          /* 0xbc, the share of the reward paid to the building's owner, 0: none */
} HENEWWORKER;
typedef struct HESTORM {                /* the storm event 0x50's words after AIJOB.stormBlds (its alloc handler 0x5652b0) */
    char pad00[0xf0];
    int radius;                         /* 0xf0, the distance a follower building must be within (the message's value,
                                           1000: G12EVENTMSG.value of the WIRBELSTURM command) */
    char padf4[0xc];
    int w100;                           /* 0x100, 0 at the allocation (no reader known) */
    int w104;                           /* 0x104, 0 at the allocation (no reader known) */
} HESTORM;
typedef struct HEROUTINE {              /* the daily routine event 0x3a's words over AIJOB's union (he_PostEvent3a posts
                                           it at the start of the day; its run handler he_RunRoutine walks ai_Chars) */
    char pad00[0xac];
    int chrIdx;                         /* 0xac, the ai_Chars index of the last character sent (he_RunRoutine stores its loop
                                           index here and clears it for the next stage), 0 at the allocation */
    int step;                           /* 0xb0, 3 at the allocation (reasoned: the unit's note calls it the routine step;
                                           he_RunRoutine never reads it) */
} HEROUTINE;
typedef struct HEAUCTION {              /* the auction event 0x67's words over AIJOB's union (he_StartAuctionEvent /
                                           he_RunAuctionEvent) */
    char pad00[0xac];
    int bld;                            /* 0xac, the id of the building auctioned */
    int price;                          /* 0xb0, the start price; a bidder must be worth at least as much */
} HEAUCTION;
typedef struct HETITLE {                /* the title ceremony event 0x69's words over AIJOB's union (he_StartTitleCeremony /
                                           he_RunTitleCeremony; the player is AIJOB.chr) */
    char pad00[0xac];
    unsigned char title;                /* 0xac, the title given: 2 the citizenship, 7 .. 9 the highest ones */
    char padad[3];
    int ch;                             /* 0xb0, the netId of the character who gets it */
    int sent;                           /* 0xb4, 1 once the title was sent as the character's field 0xd, 0 at the start */
} HETITLE;
typedef struct HEFINVIEW {              /* the player record gm_PlayerRecord finds (ai.h AIFIN, not included here) at +0x18 / +0x64 */
    char pad00[0x14];
    int burglaryTakings;                /* 0x14, the money its character's burglaries took (he_RunBurglary adds it) */
    int gangTakings;                    /* 0x18, the money the pickpocket gangs brought its character (he_RunGang adds it) */
    char pad1c[4];
    int ransomTakings;                  /* 0x20, the ransoms its character took as a thieves' guild master (he_RunRansomDemand adds them) */
    char pad24[4];
    int spyPaid;                        /* 0x28, the money paid for spies and saboteurs (ai.h AIFIN.spyPaid; he_RunSabotage adds the fee) */
    char pad2c[0x20];
    int customerTakings;                /* 0x4c, the money its character's customers paid (he_RunCustomers adds it) */
    char pad50[4];
    int buildPaid;                      /* 0x54, the money its character paid for new buildings (the construction event's
                                           run handler he_RunConstruction adds the price) */
    char pad58[0xc];
    int taught;                         /* 0x64, the trial step courses its character finished as the teacher (he_RunTrialSteps
                                           counts it; reasoned, its reader is not read yet) */
} HEFINVIEW;
#pragma pack(push, 1)
typedef struct HEOFFER {                /* the offer event 0x2c's words over AIJOB's union (AIJOB has no names at these
                                           odd offsets): the kind, the offer clock and the offer handle */
    char pad00[0xac];
    int who[6];                         /* 0xac, the character ids of the wedding offer (he_RunWeddingOffer, the party's members):
                                           the suitor, the partner, then four guests, the last -1 for none (reasoned
                                           from the news: 0x1bc0 to the partner names the suitor, 0x1bbf the other way);
                                           the master exam event 0x2f keeps its three characters here too (he_RunMasterExam) */
    char kind;                          /* 0xc4, the offer kind compared between the pending offers */
    AITIME clock;                       /* 0xc5, the offer's clock: now + 0x18 units, its hour 0xa and minute 0 */
    char pad_d3;
    int offer;                          /* 0xd4, the offer handle cm_SendHandOverCrimes gave (cm_GetSlotState asks it) */
} HEOFFER;
typedef union HEOBJ1B5VALUE {           /* the 7 byte value cm_SendObject1b5 copies to the new object's +0x1c, as
                                           he_AllocTavern fills it (byte, byte, int; their meaning is not read yet) */
    G17B7 block;
    struct {
        char c1c;                       /* +0x1c of the object */
        char c1d;                       /* +0x1d */
        int v1e;                        /* +0x1e */
    } f;
} HEOBJ1B5VALUE;
typedef struct HETAVERNOBJ {            /* the tavern's object 0x1b5 (he_AllocTavern creates it) as the tavern run handler
                                           reads it: an AIOBJ, its kind's words at +0x1c over AIOBJ's union */
    char pad00[2];
    int objId;                          /* 0x02, AIOBJ.objId */
    char pad06[0x16];
    char c1c;                           /* 0x1c, -1 when no helper (the meaning is not read yet) */
    char c1d;                           /* 0x1d, -1 when no helper */
    int helperId;                       /* 0x1e, the character hired as the tavern's helper, -1: none */
    char hired;                         /* 0x22, nonzero while a helper is hired */
} HETAVERNOBJ;
typedef struct HEAHMRESEARCH {          /* the alchemist research event 0x96's words over AIJOB's union (he_AllocResearchRecipe
                                           and its run handler he_RunResearchRecipe); ahm.h's AHMRESEARCH names the same words from ahm.c */
    char pad00[0xac];
    int chr;                            /* 0xac, the id of the character researching (gm_FindChar) */
    int id;                             /* 0xb0, the recipe / share researched (ahm_FindRecipe) */
    float progress;                     /* 0xb4, the progress of this round (ahm_ResearchProgress; the run handler adds it to the share) */
    int amount;                         /* 0xb8, the amount left (the run handler counts it down by the elapsed minutes) */
    int start;                          /* 0xbc, the amount the research started with (the minutes the progress is measured in) */
    AITIME clock;                       /* 0xc0, the clock the elapsed minutes are measured from (ai_Time) */
} HEAHMRESEARCH;
typedef struct HEPLUNDER {              /* the plunder event 0x8a's words over AIJOB's union (its alloc he_AllocPlunder and run
                                           handler 0x5707a0): the buildings the character plunders one after the other */
    char pad00[0xac];
    int agent;                          /* 0xac, the netId of the character who walks the round (the run handler finds it
                                           with gm_FindChar, sends it to the buildings and names it in the message 0x8b) */
    int at;                             /* 0xb0, the index of the building being plundered */
    int bld[5];                         /* 0xb4, the ids of the buildings to plunder (ai_PickActionBuildings' out) */
    unsigned short victims[5];          /* 0xc8, the character picked at each building (ai_PickPartnerRandomWalk's
                                           exclusion list: the run handler passes it with n = 4) */
} HEPLUNDER;
typedef struct HEPLUNDERDEAL {          /* the plunder offer 0x8b's words over AIJOB's union (the message's arg[0..3] from
                                           he_RunPlunder; its run handler 0x570cc0) */
    char pad00[0xac];
    int deal[4];                        /* 0xac, the deal he_PlunderTakePrice / he_PlunderFailNews take: [0] the client's
                                           netId (the plunder event's character), [1] the agent's, [2] the victim's,
                                           [3] the price */
} HEPLUNDERDEAL;
typedef struct HEMASTERWORK {           /* a master's production job 0x8c's words over AIJOB's union (its alloc handler
                                           0x570ef0 and run handler 0x571000; the workers are ids[0..3], the building from) */
    char pad00[0xac];
    unsigned short good;                /* 0xac, the goods produced (an ai_Goods index; its name is the text 0x9a8 + 2 * good) */
    char padae[2];
    float made;                         /* 0xb0, the units made so far, 0 at the start */
    int left;                           /* 0xb4, the working minutes left, 0x168 at the start */
    AITIME since;                       /* 0xb8, the time the last production step counted up to */
} HEMASTERWORK;
typedef struct HEGRAVECARE {            /* the grave care job 0x8d's words over AIJOB's union (the message's graveWork words
                                           from ai_GraveCare; its alloc handler 0x571550, run handler 0x571640; the worker is ids[0]) */
    char pad00[0xac];
    float care;                         /* 0xac, the care done so far, 0 at the start (handed to gv_OrderAdjustEntry at the end) */
    int size;                           /* 0xb0, the grave's size: the list of the building's graveyard (gv_ListEntry's i) */
    unsigned char entry;                /* 0xb4, the entry in that list */
    char padb5[3];
    int left;                           /* 0xb8, the working minutes left, 0x1fe at the start */
    AITIME since;                       /* 0xbc, the time the last care step counted up to */
} HEGRAVECARE;
typedef struct HEDEATHDUE {             /* the event 0x8e's words over AIJOB's union (its alloc handler 0x571990, run handler 0x5719e0) */
    char pad00[0xac];
    int price;                          /* 0xac, the amount the character pays when it can (cm_SendMoveGoods) */
} HEDEATHDUE;
typedef struct HEGRAVETHEFT {           /* the graveyard worker's theft 0x94's words over AIJOB's union (the message's
                                           arg[0] from he_RunGraveyardWorkers; run handler 0x572fd0) */
    char pad00[0xac];
    int worker;                         /* 0xac, the netId of the worker */
} HEGRAVETHEFT;
typedef struct HEGRAVETRAINING {        /* the training job 0x8f's words over AIJOB's union (the message's arg[0..2] from
                                           ai_SendFreeSpies; its alloc handler 0x571ac0, run handler 0x571c30) */
    char pad00[0xac];
    int trainee;                        /* 0xac, the netId of the character trained */
    int skill;                          /* 0xb0, the index of the skill trained (AICHAR.skills: 1, 2 or 4) */
    int objId;                          /* 0xb4, the id of the building's object it trains at (0x211), -1: none */
    AITIME until;                       /* 0xb8, the time the training ends: 3 hours after the first due time */
} HEGRAVETRAINING;
#define MEETING_KINDS_LO 0x807c4116    /* the building kinds 1, 2, 4, 8, 0xa, 0xe, 0x12 .. 0x16 and 0x1f whose people a
                                           meeting's disease reaches (he_Unk572310's 1 << kind mask, kinds below 0x20) */
#define MEETING_KINDS_HI 1             /* the same for the kinds from 0x20 on: 0x20 */
#define PLUNDER_PARTNER_FLAGS 0x700408  /* the partner-chain flags the plunder event's victim search passes
                                           ai_PickPartnerRandomWalk (an immediate, no address; the bits are not read) */
typedef struct HEMEETING {              /* the meeting event 0x90's words over AIJOB's union (its alloc he_AllocMeeting and
                                           run handler 0x5725b0) */
    char pad00[0xac];
    int bld;                            /* 0xac, the id of the building the meeting is held in */
    int radius;                         /* 0xb0, the radius kind of the meeting search (0x572310's b), capped at 2 */
    int kind;                           /* 0xb4, the kind of the meeting (0x572310's c), capped at 2 */
    int rounds;                         /* 0xb8, the rounds held (the run handler holds up to 4) */
} HEMEETING;
typedef struct HEMEETSCENE {            /* the meeting scene object event 0x91's words over AIJOB's union (its alloc
                                           handler he_AllocMeetingScene and run handler 0x5726b0) */
    char pad00[0xac];
    int partner;                        /* 0xac, the id of the partner event 0x90 the scene is shown for */
    int obj;                            /* 0xb0, the handle of the scene object the event keeps, -1: none */
    int kind;                           /* 0xb4, the meeting kind: 2 / 1 / other loads the big / middle / small script */
} HEMEETSCENE;
typedef struct HEMEETNAME {             /* the scene object event 0x91 loads: its name's first two words are the marker
                                           0x66675073 and the building id (the run handler 0x5726b0) */
    int tag;                            /* 0x00, 0x66675073 */
    int bld;                            /* 0x04, the id of the building the meeting is held in */
} HEMEETNAME;
typedef struct HEGRAVELEASE {           /* the grave lease offer event 0x92's words over AIJOB's union (the message
                                           0x92 lease gv_GraveyardTurn posts; its run handler 0x572a50 and the answer
                                           0x5728d0) */
    char pad00[0xac];
    int bld;                            /* 0xac, the graveyard building's id */
    int size;                           /* 0xb0, the grave size */
    unsigned char entry;                /* 0xb4, the entry of that size */
    char padb5[3];
    int price;                          /* 0xb8, the lease price (gv_LeasePrice) */
    unsigned short years;               /* 0xbc, the years the lease is extended by (10) */
} HEGRAVELEASE;
typedef struct HEGYPSY {                /* the gypsy event 0x9d's words over AIJOB's union (its alloc he_AllocGypsyApproach,
                                           run he_RunGypsyApproach; he_GypsyApproach dispatches the approach) */
    char pad00[0xac];
    int approach;                       /* 0xac, the approach he_GypsyApproach dispatches (0 disease .. 5+ slowdown) */
    int figure;                         /* 0xb0, the netId of the gypsy character (gm_FindChar) */
} HEGYPSY;
typedef struct HEGYPSYCURSE {           /* the gypsy curse event 0x9e's words over AIJOB's union (the message 0x9e
                                           he_GypsyCurse posts: arg[0] at 0xac, arg[1] at 0xb0; its alloc
                                           he_AllocGypsyCurse, run he_RunGypsyCurse) */
    char pad00[0xac];
    int gypsy;                          /* 0xac, the netId of the gypsy character the curse pays, -1: the cursed
                                           character is the gypsy himself (he_GypsyCurse sends -1 then) */
    int cursed;                         /* 0xb0, the netId of the cursed character (he_GypsyCurse's arg[1]) */
} HEGYPSYCURSE;
typedef struct HETRIALCASE {            /* the trial event 0xa0's words over AIJOB's union (its alloc he_AllocTrialScene,
                                           run he_RunTrialScene and the trial he_TrialPayment) */
    char pad00[0xac];
    unsigned int level;                 /* 0xac, the trial's level (below 4 / 7 raises the payment's bar; compared unsigned) */
} HETRIALCASE;
typedef struct HEMEET4 {                /* the circus show event 0xa1's words over AIJOB's union (its alloc he_AllocMeeting4, run
                                           he_RunCircusShow and the room query he_FindMeetingRoom); the four figures are AIJOB.ids */
    char pad00[0xac];
    int income;                         /* 0xac, the income of the meeting (he_CircusShowIncome adds to it) */
    int room;                           /* 0xb0, the show's time left (0xf0, 0x78 less per round); > 0: the room is looked up (he_FindMeetingRoom) */
    int stop;                           /* 0xb4, cm_SendChrStop's argument */
    char padb8[4];
    int meetStart;                      /* 0xbc, the index into classes the room query starts at */
    unsigned short classes[4];          /* 0xc0, the building classes the room query hands to gm_BuildQuery */
    unsigned short objs[4];             /* 0xc8, the meeting object ids (he_CircusShowIncome fills the first free one) */
    int bld;                            /* 0xd0, the id of the building the show is at (he_RunCircusShow), -1 at the allocation */
} HEMEET4;
#pragma pack(pop)

/* dcp unit split: the declarations of he before the moved functions */
void err_Message(char *text);
#include <stdarg.h>
#include <math.h>
void he_AllocDueIsStart(AIJOB *ev);
void he_SetLifeByAge(AICHAR *c, int num);
void d3t_SetColor(D3OBJECT *obj, unsigned char r, unsigned char g, unsigned char b);
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);  /* 0x447190, d3_interface.h */
int d3t_GetHeightRange();
void sv_PumpNet();
int sprintf(char *buf, const char *fmt, ...);
void mem_Free(void *p);
void err_Message(char *text);
int rd_PointsNear();
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);
void ai_ShowTributeDelayedNews(int param_1);
void ai_ShowTributeCollectNews(int param_1);
void ai_ShowTributeClaimNews(int param_1, int param_2);
int ai_ChooseObjectTarget();
void ai_ShowTavernHelperNews(char *param_1, char *param_2, int param_3);
int ai_EvalFest();
int ai_MasterExamPassed();
unsigned int ai_MarriageTest(int param_1, int param_2);
int gv_HeldCrimeIdsOf(int culprit, int *out, unsigned short t);
void amt_ChangeDistrictOpinion();
int ai_DrawAmountRow();
void gm_ChangeRelationToJobs(unsigned short a, int b);
void ai_PlagueNews(void);
int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out);
void amt_ChangeDistrictOpinion();
int sv_IsAtDoor();
void ai_PlagueMessage(int param_1);
float util_RandomF2();
int ls_GetLanguage();
int ai_IsLocalWorker();
int ai_CanAct();
typedef struct HEINN {                  /* he_RunInnSim's list of the inns (he_FindGuestInn walks it, 12 bytes each) */
    AIBLD *inn;                         /* 0x0, the inn (a building of kind 0x16), 0: the end of the list */
    AIOBJ *drink1;                      /* 0x4, its bar's good 0x179, 0: none */
    AIOBJ *drink2;                      /* 0x8, its bar's good 0x17a, 0: none */
} HEINN;
unsigned short hmt_RecordCount(void);
unsigned short hmt_RandomRecord(short k);
unsigned short hmt_RandomTextOffset(unsigned short k);
struct G11REC *hmt_GetRecord(unsigned short k);
float ord_GetStatistics(int *o);
int mem_IsValid();
void par_Delete();
int d3t_PlayAnim();
float d3_VecAngle2();
int d3t_PlaceObjectV(struct D3OBJECT *camera, struct D3VECTOR *pos, struct D3VECTOR *dir, int unk, char flags);
void ai_PlagueMessageAll(void);
void udp_Send(char *buf, int len);
#pragma pack(push, 1)
typedef struct HERANKHEAD {             /* 0xcf bytes, the head packet of the ranking he_RunRanking sends (udp_Send) */
    unsigned short one;                 /* 0x00, 1 */
    unsigned short kind;                /* 0x02, 3: the head */
    unsigned short size;                /* 0x04, 0xcf, the bytes sent */
    char titles[5][0x18];               /* 0x06, the five column titles (texts 0x16b7 .. 0x16bb) */
    char player[0x20];                  /* 0x7e, the own player's name */
    char worthTitle[0x18];              /* 0x9e, "Vermögen" */
    char pointsTitle[0x18];             /* 0xb6, "Gewinnpunkte" */
    unsigned char count;                /* 0xce, the players that follow */
} HERANKHEAD;
typedef struct HERANKLINE {             /* 0xb9 bytes, one ranked player's packet (after HERANKHEAD) */
    unsigned short one;                 /* 0x00, 1 */
    unsigned short kind;                /* 0x02, 2: a line */
    unsigned short size;                /* 0x04, 0xb9, the bytes sent */
    unsigned int netId;                 /* 0x06, the character's netId */
    int worth;                          /* 0x0a, GMRANK.worth in the own player's money (gm_ConvertMoney) */
    int points;                         /* 0x0e, GMRANK.points */
    unsigned char count;                /* 0x12, the players ranked */
    unsigned char player;               /* 0x13, 1 for a player's character (kind 6 / 7), else 2 */
    unsigned char skills[5];            /* 0x14, AICHAR.skills */
    char name[0x18];                    /* 0x19, the name */
    char job[0x20];                     /* 0x31, the profession (text 0x130, + 0x5f for a woman, + jobId) */
    char rank[0x20];                    /* 0x51, the rank (text 0x114, + 0xa for a woman, + b00d) */
    char office[0x24];                  /* 0x71, the office (text 0x253, + 0x28 for a woman, + officeKind) */
    char office2[0x24];                 /* 0x95, the second office (the same texts, + secondOffice) */
} HERANKLINE;
#pragma pack(pop)
int sv_IsAtDoor();
int he_PickNearBuilding(AICHAR *c, int n, AIBLD **blds, int *bldId, int *objId);
int amt_InitApplicant(char *c, int k);
int amt_GetOpenOffice(int k);
int amt_CreateApplicant(int k);
int ai_TutorialSetHost();
int ai_TutorialOpenIntro();
int ai_TutorialTick();
int ai_TutorialCloseIntro();
void ai_TutorialClearHost(void);
void sv_PopTo(int v);
void he_FreeEventNow();
#define cm_SendExSellObjectH ((int (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880: it returns the slot handle of its message (cm_QueueSend's result stays in eax), which the void prototype drops; @original he_RunSale keeps it (as f3gm.c's cm_SendExSellObjectH) */
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2);
int gm_LevelPayout(int v, int kind);
typedef struct SVFRAME {                /* 8 bytes, an entry of the frame stack sv_Frames (sv.c sv_RunFrame: "MASTER()"; as sv.c / lc.c declare it) */
    int cb;                             /* 0x00, the frame callback registered at this depth */
    char pad04[4];
} SVFRAME;
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);
int sv_IsCartAtDoor();
int init_BuildingSceneUniverse();
void sv_SelectPanelObject(unsigned char *o, int b, char *c);
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);
int sv_IsAtDoor();
int ord_DispatchOrder(int id, char *c, int *m);
int ai_BrawlWin();
struct SBBANK * sb_FindBank();
struct G15PLANT * veg_FindPlantId(struct G15PLANT *p, int id);
int init_ObjectSceneUniverse();
float gm_StockPrice(int p, int n);  /* 0x4ffa10 (f3gm.c; f3gm.h does not declare it) */
typedef struct HEEV3D {                 /* the ransom event 0x3d's words over AIJOB's union (he_AllocRansom / he_RunRansom;
                                           he_RunKidnap's ransom message 0x3d posts it: victim, guild) */
    char pad00[0xac];
    int victim;                         /* 0xac, the netId of the kidnapped character (gm_FindChar) */
    int guild;                          /* 0xb0, the id of the thieves' guild that holds it (gm_BuildQuery(1, 1, id)) */
    AITIME since;                       /* 0xb4, the time of the kidnap (he_AllocRansom: now), shown by the news 0x19c2 */
} HEEV3D;
unsigned char *gm_NearestBuilding(unsigned char *b, unsigned kind);
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);
typedef struct HEEV3E {                 /* the ransom demand event 0x3e's words over AIJOB's union (he_RunRansom's message 0x3e) */
    char pad00[0xac];
    int victim;                         /* 0xac, the netId of the kidnapped character (gm_FindChar), who is asked to pay */
    int master;                         /* 0xb0, the netId of the guild's master (gm_FindChar), who is paid */
    int ransom;                         /* 0xb4, the ransom */
} HEEV3E;
typedef struct HEBURGLARY {             /* the burglary event 0x3f's words over AIJOB's union (he_AllocBurglary / he_RunBurglary);
                                           the gang is AIJOB.ids */
    char pad00[0xac];
    int target;                         /* 0xac, the id of the building broken into */
    int guild;                          /* 0xb0, the id of the thieves' guild the gang comes from */
    int slot;                           /* 0xb4, the cm slot of the accusation (gv_Accuse) when the watch saw it, -1: none */
    int watch;                          /* 0xb8, the id of the patrol event 0x43 that saw it (HEDETECT.found) */
    int guardBld;                       /* 0xbc, that patrol's start building (patrolStart), which counts the reward */
} HEBURGLARY;
typedef struct HEBLDSPY {               /* the building spying event 0x40's words over AIJOB's union (he_AllocDueInOne /
                                           he_RunBuildingSpying); the spies are AIJOB.ids */
    char pad00[0xac];
    int target;                         /* 0xac, the id of the building spied on */
    int home;                           /* 0xb0, the id of the spies' building (its owner is told), where the report goes */
} HEBLDSPY;
typedef struct HEAMBUSH {               /* the ambush event 0x48's words over AIJOB's union (he_AllocAmbush / he_RunAmbush): the
                                           gang (AIJOB.ids) lies in wait at a scene object for passing transports */
    char pad00[0xac];
    char place[0x20];                   /* 0xac, the name of the scene object the gang waits at (a name with "STADT":
                                           the fight's variant flag 8) */
    AITIME until;                       /* 0xcc, the time it gives up (the alloc handler: 48 hours ahead) */
    char padda[2];
    int partySlot;                      /* 0xdc, the cm send slot of the fight party (cm_SendNewParty) */
    int party;                          /* 0xe0, the id of that party (cm_GetSlotValue; cm_PartyById) */
    unsigned int skill;                 /* 0xe4, the gang's mean skills[2] */
} HEAMBUSH;
typedef struct HEFIRE {                 /* the fire event 0x4e's words over AIJOB's union (he_AllocFire / the run handler
                                           0x5644e0), laid out as the storm event's (AIJOB.stormBlds, HESTORM) */
    char pad00[0xac];
    int blds[16];                       /* 0xac, [0] the burning building's id (the message's, else a random pick),
                                           [1 ..] the buildings within radius of it, -1: none */
    int center;                         /* 0xec, the id of the building of blds[0] */
    int radius;                         /* 0xf0, the distance a neighbour building must be within (the message's value) */
} HEFIRE;
typedef struct HEBURN {                 /* the burning event 0x4f's words over AIJOB's union (its alloc handler he_AllocBurn,
                                           its run handler 0x5649b0) */
    char pad00[0xac];
    int bld;                            /* 0xac, the id of the burning building (the message's arg[0]) */
    int duration;                       /* 0xb0, the burning time (the message's arg[1]), scaled by the alloc handler */
    unsigned char state;                /* 0xb4, the run handler's state, 0 at the start */
    char padb5[3];
    int helpers[16];                    /* 0xb8, the netIds of the characters the run handler sends to fetch water
                                           (cm_SendChrGotoBuilding "Brand"), -1: none */
    int fire;                           /* 0xf8, the id of the fire event 0x4e whose message 0x4f started it (AIMSG.srcEvent) */
    unsigned voiceAt;                   /* 0xfc, the tb_Calls tick of the last "Brand_Zerstoert" voice */
    char pad100[4];
    unsigned char news;                 /* 0x104, nonzero: the owner gets the message box (the message's extraFlag) */
} HEBURN;
typedef struct HEHEAL {                 /* the healing event 0x6c's words over AIJOB's union (he_AllocHealing / he_RunHealing) */
    char pad00[0xac];
    int charId;                         /* 0xac, the netId of the character healed (gm_FindChar) */
    unsigned amount;                    /* 0xb0, the health left to heal, 200 at the allocation (the run's step takes it down) */
    AITIME end;                         /* 0xb4, the next run (AIJOB.end6c) */
    char padc2[2];
    int room;                           /* 0xc4, the room object the character is sent to (gm_FindObjectInst), -1: none */
} HEHEAL;
typedef struct HEKILL {                 /* the kill-player event 0x72's words over AIJOB's union (its alloc handler 0x565d10,
                                           run handler he_RunKillPlayer) */
    char pad00[0xac];
    int chr;                            /* 0xac, the netId of the character to die (gm_FindChar) */
    int party;                          /* 0xb0, the id of its funeral party (cm_PartyById), -1: none */
    int slot;                           /* 0xb4, the cm send slot of the party (cm_SendNewParty), -1: none */
} HEKILL;
typedef struct HENIGHTWATCH {           /* the night watchman event 0x73's words over AIJOB's union (its alloc handler
                                           he_AllocNightWatch, run handler he_RunNightWatch) */
    char pad00[0xac];
    int speaker;                        /* 0xac, the voice source of the announcements (snd_QueueVoice's first argument) */
} HENIGHTWATCH;
typedef struct HEROBBERY {                 /* the robbery event 0x75's words over AIJOB's union (he_AllocDueInOne / he_RunRobbery),
                                           laid out as the burglary's HEBURGLARY; the gang is AIJOB.ids */
    char pad00[0xac];
    int target;                         /* 0xac, the id of the building raided */
    int home;                           /* 0xb0, the id of the gang's building, where it goes back to */
    int slot;                           /* 0xb4, the cm slot of the accusation (gv_Accuse) when the watch saw it, -1: none */
    int watch;                          /* 0xb8, the id of the patrol event 0x43 that saw it (HEDETECT.found) */
    int guardBld;                       /* 0xbc, that patrol's start building (patrolStart), which counts the reward */
} HEROBBERY;
float gm_TitlePeriodServed(int n, int *out);

/* dcp unit split: the definitions of he the functions of he_b use */
void he_AllocDueIsStart(AIJOB *ev);
int he_PickNearBuilding(AICHAR *c, int n, AIBLD **blds, int *bldId, int *objId);

/* 0x568aa0, ends every started loan event 0x23 (he_EndEventNow) whose partner (HELOAN.partner, the other party's
   netId) is the character c: gm_ReleaseCharacter calls it before the character is released */

void he_EndKind23Events(int rec)
{
    AICHAR *c = (AICHAR *)rec;
    AIJOB *e;

    for (e = (AIJOB *)he_FindEvent(1, 0, 0x23); e != 0; e = (AIJOB *)he_NextEvent()) {
        if ((e->opts & 3) != 0 && ((HELOAN *)e)->partner == c->netId)
            he_EndEventNow((int)e);
    }
}

float gm_MeanLiking(int c, int n, int *ids);

/* 0x568ae0, the allocation handler of the event type 3 (the running income, its run handler 0x568be0): the stock amount +0xb0
   and its float at +0xb4 are the work time of the goods +0xac (ai_Goods), both clocks +0x52 / +0x60 are the start clock +0x44,
   the rate +0xc4 is (gm_MeanLiking (+8, 8, +0x8c) - 0.5) * 0.25 and the counter +0xc8 is 0; every character whose job +0x17c is
   this event joins the group of the building +0xb8; the next run is one minute ahead */

void he_AllocIncomeEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *b;
    AICHAR *rec;
    double d;
    int v;

    v = (int)GM_GOOD(e->incomeGood)->workTime;
    e->incomeAmount = v;
    e->end = e->start;
    e->incomeAmountF = (float)v;
    e->clock60 = e->start;
    d = gm_MeanLiking(e->chr, 8, e->ids) - 0.5;
    e->incomeCounter = 0;
    e->incomeRate = (float)(d * 0.25);
    b = (AIBLD *)gm_FindBuildingById(e->incomeBld);
    for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
        if (rec->job == e)
            sv_MoveToGroup((unsigned char *)b, 0, rec->id, (char *)ev);
    }
    gm_AddTime(&e->end, 0, 0, 1);
}
/* 0x568be0, the run handler of the running income event 3 (its alloc handler he_AllocIncomeEvent): without its
   building (incomeBld) every character on the event leaves the group and the event is freed. The building's owner
   counts unless it is the player's character. Stage 0 goes to 1. Stage 1 goes to 2 when no workers are left (the
   words +0x14 / +0x18); else the factor is 1 + 0.01 * the building's skill (gm_BuildingSkill); every 60 minutes
   (incomeCounter) the owner sends his relations to the eight characters ids[] (2 - workRate, unless it is 2) and
   the rate becomes (gm_MeanLiking - 0.5) * 0.25; the minutes since clock60 add to the counter. While the due clock
   is past, the stock incomeAmount loses the work minutes since clock60 times (2 * +0x14 + +0x18) times
   (factor + rate); a used-up stock takes the next batch of the goods (at most the goods' batch) from incomeFrom
   to incomeTo and adds the goods' work time, until it is positive; without the goods left the owner gets the
   news 0x1bb7 ("_NACHRICHTEN_HS_07"), the characters leave the group and the event is freed; clock60 takes the
   due clock, which moves 5 seconds on. Stage 2 goes back to 1 while workers are left, else the characters leave
   the building's group and the event is freed (stage -1 when the building is gone); -1 / -2 free it too */
void he_RunIncomeEvent(AIJOB *ev)
{
    AIBLD *bld;
    float factor;
    float amount;
    AIOBJ *from;
    AIOBJ *to;
    char text[0x100];
    AICHAR *owner;
    AICHAR *rec;
    GMGOOD *good;
    AIOBJ *stock;
    int i;
    int n;

    owner = 0;
    gm_FindById((int *)&bld, 0, 0, ev->incomeBld);
    if (bld == 0) {
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup(0, 0, rec->id, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    }
    if (bld->owner != 0xffff) {
        rec = &ai_CharsT[bld->owner];
        if (rec != he_PlayerCh)
            owner = rec;
    }
    switch (ev->stage) {
    case 0:
        ev->stage++;
        return;
    case 1:
        if (ev->words14[0] == 0 && ev->words14[1] == 0) {
            ev->stage = 2;
            return;
        }
        gm_FindById((int *)&bld, 0, 0, ev->incomeBld);
        factor = gm_BuildingSkill(bld, 1, 1) * 0.01 + 1.0;
        if (ev->incomeCounter >= 60) {
            for (i = 0; i < 8 && owner != 0; i++) {
                if (ev->ids[i] != -1 && bld->workRate != 2)
                    cm_SendRelations(owner->netId, ev->ids[i], 2 - bld->workRate, 0);
            }
            if (owner != 0)
                ev->incomeRate = (float)((gm_MeanLiking(owner->id, 8, ev->ids) - 0.5) * 0.25);
            ev->incomeCounter = 0;
        }
        n = gm_WorkMinutesBetween(&ev->clock60, &ai_Time, 0);
        if (n > 0)
            ev->incomeCounter += n;
        factor += ev->incomeRate;
        while (gm_CompareTime(&ev->end, &ai_Time) < 0) {
            good = GM_GOOD(ev->incomeGood);
            amount = (float)ev->incomeAmount;
            n = gm_WorkMinutesBetween(&ev->clock60, &ev->end, 0) * (ev->words14[0] * 2 + ev->words14[1]);
            ev->incomeAmount = (int)(amount - n * factor);
            while (ev->incomeAmount <= 0) {
                gm_FindById(0, (int *)&from, 0, ev->incomeFrom);
                stock = (AIOBJ *)gm_Query((void *)from->list, 1, 0, ev->incomeGood);
                gm_FindById(0, (int *)&to, 0, ev->incomeTo);
                if (stock == 0) {
                    txt_Format(text, 0x1bb7, ev->incomeGood * 2 + 0x9a8, bld);
                    he_MessageBoxQuickjump(ai_CharsT[ev->chr].netId, -1, text, 0, 0x5f1, bld->id, -1, 0,
                                           "_NACHRICHTEN_HS_07");
                    for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
                        if (rec->job == ev)
                            sv_MoveToGroup((unsigned char *)bld, 0, rec->id, 0);
                    }
                    he_FreeEvent((int *)ev);
                    return;
                }
                n = good->batch;
                if (n >= stock->count)
                    n = stock->count;
                cm_SendExSellObject(ev->incomeTo, ev->incomeFrom, ev->incomeGood, n, ai_OwnPlayer, 0);
                ev->incomeAmount += good->workTime;
                ev->incomeAmountF = (float)ev->incomeAmount;
            }
            ev->clock60 = ev->end;
            gm_AddTime(&ev->end, 0, 0, 5);
        }
        return;
    case 2:
        if (ev->words14[0] > 0 || ev->words14[1] > 0) {
            ev->stage = 1;
            return;
        }
        gm_FindById((int *)&bld, 0, 0, ev->incomeBld);
        if (bld == 0) {
            ev->stage = -1;
            return;
        }
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup((unsigned char *)bld, 0, rec->id, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup(0, 0, rec->id, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    default:
        return;
    }
}
/* 0x5690c0, the allocation handler of the production event ("he_AllocProduktion()"): the building (0xbc) and the good (0xac) are taken; both clocks (0x52 / 0x60) are the start clock (0x44), the amount (0xb4 / 0xb8) is the good's work time (its record's 0x22), due 1 unit later; the state 0xc4 = -1 and 0xc8 = 0; every character whose job (0x17c) is this event moves to the building's group; the rate 0xcc = (the owner's mean liking - 0.5) * 0.25 and 0xd4 = 1.0; an unset owner (chr 0xffff) with a found building prints the debug text */

void he_AllocProduktion(AIJOB *ev)
{
    HEPRODUCTION *p = (HEPRODUCTION *)ev;
    char buf[0x200];
    AIBLD *b;
    GMGOOD *g;
    int o, id, rec;
    float v;

    if (ev->chr == 0xffff) {
        o = (int)gm_FindBuildingById(p->bld);
        if (o != 0)
            txt_Format(buf, "he_AllocProduktion(): geb:%G", o);
    }
    b = (AIBLD *)gm_FindBuildingById(p->bld);
    g = (GMGOOD *)ai_Goods;
    id = p->good;
    v = (float)g[id].workTime;
    ev->end = ev->start;
    ev->clock60 = ev->start;
    p->amount = v;
    p->amount2 = v;
    gm_AddTime(&ev->end, 0, 0, 1);
    p->slot = -1;
    p->lacking = 0;
    for (rec = (int)ai_Chars; rec < (int)ai_Chars + 0x65400; rec += 0x21c) {
        if (((AICHAR *)rec)->job == ev)
            sv_MoveToGroup(b, 0, ((AICHAR *)rec)->id, ev);
    }
    p->rate = (gm_MeanLiking(ev->chr, 8, ev->ids) - 0.5) * 0.25;
    p->staff = 1.0f;
}

void sv_CountGroupMembers(char *g);
int he_HasEvent9f(AICHAR *a);

#define cm_SendMakeGoodsH ((int (*)(int, int, int))cm_SendMakeGoods)  /* 0x4c48e0: it returns the send slot of its message (cm_QueueSend's result stays in eax), which the void prototype drops; @original he_RunProduktion keeps it (HEPRODUCTION.slot) and passes the good as an int (zero-extended) */

/* 0x569210, the run handler of the production event (he_AllocProduktion; "he_RunProduktion()"): without a character
   it frees itself (the debug text names the building). Stage 1 goes to 2 when no workers are left (the words +0x14 /
   +0x18). A make message sent (slot) is waited for; once it failed (state 2) and a worker is on the event, the master
   is not away (flags1b4 bit 1) and no shortage was reported yet, the owner of kind 6 / 7 gets the news why: the
   store holds the good and has no room for it (0x1bb3, "_NACHRICHTEN_HS_04"), no room at all (0x1bb4, "_HS_05") or
   the inputs were missing (0x1bb2, the shortage news of the building type, cm_SendStates kind 8); the smoke stops.
   Without a pending message the work runs: the factor is 1 + 0.01 * the building's skill, plus the master's skill
   sk81 / 1008 for an owner of kind 6 / 7, halved with flag 0x20 and the item 0x178; every 60 minutes the owner sends
   his relations to the workers ids[], their mean factor (health, skill for the titles 0x1b / 0x1c, the place factor,
   x 1.25 with the item 0x2ba; x 0.9 with an event 0x9f) and the liking rate are taken anew. While the due clock is
   past and the store (kind 0x2a / 0x116) has the inputs (GMGOOD.in / need) and room, the work minutes times
   (1.3 * +0x14 + +0x18) times the rate wear the amount down; every work time used up makes one unit
   (cm_SendMakeGoods); without units and the good no longer in the store, the good is ordered (cm_SendExSellObject).
   A shortage reports as above (once), stops the smoke and waits 5 minutes. Stage 2 goes back to 1 while workers are
   left, else the workers leave the building's group, the smoke stops and the event is freed; -1 / -2 free it
   @deep rebuilt from the whole original (frame 0x724: name[MAX_PATH] at 0x20, text[0x600] at 0x124; the
   case order 1/0/2/-1; txt_Format(text, id, bld, good*2+0x9a8); the shortage news chains; the make / order
   slots; while amount <= 0.0f; the minutes product with the call converted in place). 1147 vs 1151
   instructions. Left is allocation: VC6 keeps &ev->clock60 (the CSE, or the clock variable) in ebx where
   the original keeps it in the stack slot 0x8 (mov [esp+0x24], eax at +0x79e) and caches the constant 0 in
   ebx instead (xor ebx, ebx at +0x8cc / +0x96b; cmp ax, bx / push ebx / cmp [ebp+0xc8], ebx / mov
   [esp+0x28], ebx for the qword high half): every code hunk from +0x792 to +0xd95 is that (push ebx vs
   push 0, mov ecx, ebx vs mov edx, [esp+0x18]), and the slots hunks follow from it (factor f at 0x8 vs
   0xc). The tail hunk: case 1's no-worker path shares case 0's stage++ inline in the original (+0x87) and
   as a tail block in ours (+0xda0), the cause of the one switch entry that differs; goto / fall-through
   spellings gave the same layout (size: 8 of the instruction difference are shared / inline tails) */
void he_RunProduktion(AIJOB *ev)
{
    HEPRODUCTION *p = (HEPRODUCTION *)ev;
    AIBLD *bld;
    AIOBJ *store;
    float f;
    float fv;
    float sum;
    AIBTYPE *bt;
    char name[MAX_PATH];
    char text[0x600];
    AICHAR *rec;
    AICHAR *worker;
    AICHAR *master;
    AIOBJ *mainObj;
    AIOBJ *o;
    AIJOB *box;
    GMGOOD *g;
    int news;
    int n;
    int i;
    int units;

    name[0] = 0;
    if (ev->chr == 0xffff) {
        bld = (AIBLD *)gm_FindBuildingById(p->bld);
        if (bld != 0)
            txt_Format(text, "he_RunProduktion(): geb:%G", bld);
        he_FreeEvent((int *)ev);
        return;
    }
    switch (ev->stage) {
    case 1:
        if (ev->words14[0] > 0 || ev->words14[1] > 0)
            goto work;
        /* no workers left: on to stage 2 */
    case 0:
        ev->stage++;
        return;
work:
        if (p->slot != -1) {
            if (cm_GetSlotState(p->slot) == 0)
                return;
            gm_FindById((int *)&bld, 0, 0, p->bld);
            if (bld == 0) {
                ev->stage = -1;
                return;
            }
            bt = &ai_BTypesT[bld->type];
            if (cm_GetSlotState(p->slot) == 2) {
                worker = 0;
                name[0] = 0;
                master = (AICHAR *)gm_FindCharacterByMaster(bld);
                for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
                    if (rec->job == ev)
                        worker = rec;
                }
                if (worker != 0 && (master == 0 || (master->flags1b4 & 1) == 0) && p->lacking == 0) {
                    gm_FindById(0, (int *)&store, 0, p->store);
                    mainObj = (AIOBJ *)gm_BuildingMainObject((unsigned char *)bld);
                    if (gm_Query((void *)store->list, 1, 0, p->good) != 0 && gm_FreeCapacity(store, p->good, 1) == 0) {
                        txt_Format(text, 0x1bb3, bld, p->good * 2 + 0x9a8);
                        strcpy(name, "_NACHRICHTEN_HS_04");
                    } else if (gm_FreeCapacity(store, p->good, 1) == 0) {
                        txt_Format(text, 0x1bb4, bld, p->good * 2 + 0x9a8);
                        strcpy(name, "_NACHRICHTEN_HS_05");
                    } else {
                        if (cm_SendStates[p->slot].kind != 8)
                            goto wait;
                        txt_Format(text, 0x1bb2, bld, p->good * 2 + 0x9a8);
                        if (bt->kind == 7)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_01");
                        else if (bt->kind == 8)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_03");
                        else if (bt->kind == 0xb)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_05");
                        else if (bt->kind == 0xc)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_07");
                        else if (bt->kind == 0xd)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_09");
                        else if (bt->kind == 0xe)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_11");
                        else if (bt->kind == 0x12)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_13");
                        else if (bt->kind == 0x14)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_15");
                        else if (bt->kind == 0x15)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_17");
                        else if (bt->kind == 0x16)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_19");
                        else if (bt->kind == 0x1e)
                            strcpy(name, "_NACHRICHTEN_DOMIZIL_02");
                        else if (bt->kind == 0x20 || bt->kind == 0x1f || bt->kind == 0x1d)
                            strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_20");
                    }
                    if (ai_CharsT[bld->owner].kind == 6 || ai_CharsT[bld->owner].kind == 7) {
                        news = 0x5f1;
                        master = (AICHAR *)gm_FindCharacterByMaster(bld);
                        if (master != 0 && (master->flags1b4 & 1))
                            news = 0x60d;
                        if (name[0] != 0)
                            he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, ev->id, text, 0, news, bld->id,
                                                   mainObj->objId, 0, name);
                        else
                            he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, ev->id, text, 0, news, bld->id,
                                                   mainObj->objId, 0, 0);
                    }
                    if (bld != 0 && bld->smokeEffect != -1)
                        cm_SendUpdateSmoke(bld->id);
                    p->lacking = 1;
                }
wait:
                p->slot = -1;
                gm_AddTime(&ev->end, 0, 5, 0);
                return;
            }
        }
        gm_FindById((int *)&bld, 0, 0, p->bld);
        if (bld == 0) {
            ev->stage = -1;
            return;
        }
        if (bld->smokeEffect == -1)
            cm_SendUpdateSmoke(bld->id);
        bt = &ai_BTypesT[bld->type];
        f = gm_BuildingSkill(bld, 1, 1) * 0.01 + 1.0;
        master = (AICHAR *)gm_FindCharacterByMaster(bld);
        if (master == 0)
            master = &ai_CharsT[bld->owner];
        f += master->kind == 6 || master->kind == 7 ? (float)(master->sk81 * 0.000992063492063492) : 0.0f;
        if ((bld->flags5a & 0x20) && ai_BuildingHasItem(bld, 0x178))
            f = f * 0.5;
        if (p->counter >= 60) {
            n = 0;
            sum = 0;
            for (i = 0; i < 8; i++) {
                if (ev->ids[i] == -1)
                    continue;
                if (bld->workRate != 2)
                    cm_SendRelations(ai_CharsT[bld->owner].netId, ev->ids[i], 2 - bld->workRate, 0);
                worker = (AICHAR *)gm_FindChar(ev->ids[i]);
                if (worker != 0) {
                    fv = gm_HealthRatio((short *)worker);
                    if (worker->title == 0x1b || worker->title == 0x1c) {
                        fv = (gm_GetSkill((char *)worker, 1) + 0.5) * fv;
                        if (worker->flags1b4 & 1)
                            fv = 0.01f;
                    }
                    if (worker->flags1cc & 4)
                        fv = ahm_PlaceFactor(worker, p->good) * fv;
                    sum += ai_InventoryHas(&ai_CharsT[bld->owner], 0x2ba) ? fv * 1.25f : fv;
                }
                n++;
            }
            if (n != 0) {
                p->staff = sum / n;
                if (he_HasEvent9f(&ai_CharsT[bld->owner]))
                    p->staff = p->staff * 0.9f;
            }
            sv_CountGroupMembers((char *)ev);
            p->rate = (gm_MeanLiking(bld->owner, 8, ev->ids) - 0.5) * 0.25;
            p->counter = 0;
        }
        n = gm_WorkMinutesBetween(&ev->clock60, &ai_Time, 0);
        if (n > 0)
            p->counter += n;
        f = ((bld->flags5aw & 0x1000 ? f + 0.1f : f) + p->rate + (bld->workRate - 2) * 0.1) * p->staff;
        if (gm_FindById(0, (int *)&store, 0, p->store) == 0 || (store->type != 0x2a && store->type != 0x116)) {
            he_FreeEvent((int *)ev);
            return;
        }
        if (gm_Query((void *)store->list, 1, 0, p->good) != 0 && gm_FreeCapacity(store, p->good, 1) == 0) {
            txt_Format(text, 0x1bb3, bld, p->good * 2 + 0x9a8);
            strcpy(name, "_NACHRICHTEN_HS_04");
        } else if (gm_FreeCapacity(store, p->good, 1) == 0) {
            txt_Format(text, 0x1bb4, bld, p->good * 2 + 0x9a8);
            strcpy(name, "_NACHRICHTEN_HS_05");
        } else {
            g = GM_GOOD(p->good);
            for (i = 0; i < 4; i++) {
                if (g->in[i] != 0) {
                    o = (AIOBJ *)gm_Query((void *)store->list, 1, 0, g->in[i]);
                    if (o == 0 || gm_ListAmount(store, o) < g->need[i])
                        goto lacking;
                }
            }
            if (p->lacking != 0) {
                box = (AIJOB *)he_FindEvent(2, 0, 0x11, 3, ev->id);
                if (box != 0)
                    he_FreeEvent((int *)box);
                p->lacking = 0;
                if (ev->gfx != 0) {
                    he_FreeGfxInfo((int *)ev->gfx);
                    ev->gfx = 0;
                }
                ev->clock60 = ev->end;
            }
            while (gm_CompareTime(&ev->end, &ai_Time) < 0) {
                g = GM_GOOD(p->good);
                p->amount = p->amount - gm_WorkMinutesBetween(&ev->clock60, &ev->end, 0)
                                        * (ev->words14[0] * 1.3f + ev->words14[1]) * f;
                units = 0;
                while (p->amount <= 0.0f) {
                    p->amount = g->workTime + p->amount;
                    p->amount2 = p->amount;
                    units++;
                }
                if (units != 0)
                    p->slot = cm_SendMakeGoodsH(p->store, p->good, units);
                else if (p->slot == -1 && gm_Query((void *)store->list, 1, 0, p->good) == 0)
                    p->slot = cm_SendExSellObjectH(store->objId, -1, p->good, 0, ai_OwnPlayer, 0);
                ev->clock60 = ev->end;
                gm_AddTime(&ev->end, 0, 1, 0);
            }
            return;
lacking:
            txt_Format(text, 0x1bb2, bld, p->good * 2 + 0x9a8);
            if (bt->kind == 7)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_01");
            else if (bt->kind == 8)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_03");
            else if (bt->kind == 0xb)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_05");
            else if (bt->kind == 0xc)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_07");
            else if (bt->kind == 0xd)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_09");
            else if (bt->kind == 0xe)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_11");
            else if (bt->kind == 0x12)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_13");
            else if (bt->kind == 0x14)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_15");
            else if (bt->kind == 0x15)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_17");
            else if (bt->kind == 0x16)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_19");
            else if (bt->kind == 0x20 || bt->kind == 0x1f || bt->kind == 0x1d)
                strcpy(name, "_NACHRICHTEN_ROHSTOFFMANGEL_HS_20");
        }
        mainObj = (AIOBJ *)gm_BuildingMainObject((unsigned char *)bld);
        if (p->lacking == 0 && (ai_CharsT[bld->owner].kind == 6 || ai_CharsT[bld->owner].kind == 7)) {
            news = 0x5f1;
            master = (AICHAR *)gm_FindCharacterByMaster(bld);
            if (master != 0 && (master->flags1b4 & 1))
                news = 0x60d;
            if (mainObj != 0)
                he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, ev->id, text, 0, news, bld->id, mainObj->objId, 0,
                                       name);
            else
                he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, ev->id, text, 0, news, bld->id, -1, 0, name);
        }
        if (bld != 0 && p->lacking == 0 && bld->smokeEffect != -1)
            cm_SendUpdateSmoke(bld->id);
        p->lacking = 1;
        p->slot = -1;
        gm_AddTime(&ev->end, 0, 5, 0);
        ev->clock60 = ev->end;
        return;
    case 2:
        if (ev->words14[0] > 0 || ev->words14[1] > 0) {
            ev->stage = 1;
            return;
        }
        gm_FindById((int *)&bld, 0, 0, p->bld);
        if (bld == 0) {
            ev->stage = -1;
            return;
        }
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup((unsigned char *)bld, 0, rec->id, 0);
        }
        if (bld != 0)
            cm_SendUpdateSmoke(bld->id);
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        gm_FindById((int *)&bld, 0, 0, p->bld);
        if (bld != 0)
            cm_SendUpdateSmoke(bld->id);
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup(0, 0, rec->id, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    default:
        return;
    }
}
/* 0x56a100, the allocation handler of a search event 8 / 9: both clocks (end and clock60) start at the event's
   start clock, and the end is a random 0 .. 59 minutes later (kind 4) */

void he_AllocSearchEvent(AIJOB *e)
{
    e->end = e->start;
    e->clock60 = e->start;
    gm_AddTime(&e->end, 4, (unsigned short)util_Random(0x3c), 0);
}
/* 0x56a170, the run handler of the search event 8 ("Suchen", he_AllocSearchEvent): stage 0 goes to 1; stage 1 frees
   the event when no workers are left (the words +0x14 / +0x18), else waits for the due clock; then, with its building
   (else stage -1), the workers find (random(batch * 40) + batch * 40) times their number of the good (GMGOOD.batch),
   handed to the event's store (cm_SendExSellObject from nobody), the building's owner gets the news 0x1bb8
   ("_NACHRICHTEN_HS_08"), the workers join the building's group and the event is freed; -1 / -2 send the workers off
   and free it */
void he_RunSearchEvent(AIJOB *ev)
{
    HESEARCH *s = (HESEARCH *)ev;
    AIBLD *bld;
    char text[0x200];
    GMGOOD *g;
    AICHAR *rec;
    int amount;

    switch (ev->stage) {
    case 0:
        ev->stage++;
        return;
    case 1:
        if (ev->words14[0] == 0 && ev->words14[1] == 0)
            break;
        if (gm_CompareTime(&ev->end, &ai_Time) >= 0)
            return;
        gm_FindById((int *)&bld, 0, 0, s->bld);
        if (bld == 0) {
            ev->stage = -1;
            return;
        }
        g = GM_GOOD(s->good);
        amount = (util_RandomU(g->batch * 40) + g->batch * 40) * (ev->words14[0] + ev->words14[1]);
        cm_SendExSellObject(s->store, -1, s->good, amount, ai_OwnPlayer, 0);
        txt_Format(text, 0x1bb8, s->good * 2 + 0x9a8, bld, amount);
        he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, -1, text, 0, 0x5f1, bld->id, -1, 0, "_NACHRICHTEN_HS_08");
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup((unsigned char *)bld, 0, rec->id, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup(0, 0, rec->id, 0);
        }
        break;
    default:
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x56a360, the allocation handler of the search event 9: both clocks (end and clock60) start at the event's start
   clock, and the end is 7 hours and a random 30 .. 59 minutes later */

void he_AllocSearch9(AIJOB *e)
{
    e->end = e->start;
    e->clock60 = e->start;
    gm_AddTime(&e->end, 7, (unsigned short)util_Random(0x1e) + 0x1e, 0);
}


/* 0x56a3d0, the run handler of the search event 9 (the second "Suchen" activity, he_AllocSearch9): stage 0 goes to 1;
   stage 1 frees the event when no workers are left (the words +0x14 / +0x18), else waits for the due clock; then,
   with its building (else stage -1), the first good of the kind 0x17 with a count of 1 in the source object's list
   (gm_Query key 4) is found: one more of it goes to the source, three of every good the building can make
   (gm_BuildKindObjects, not of the kind 0x17) whose other inputs the source holds at least twice, and
   (random(batch * 30) + batch * 30) times the workers of it to the event's store; the owner gets the news 0x1bb8
   ("_NACHRICHTEN_HS_08"). The workers join the building's group and the event is freed; -1 / -2 send the
   workers off and free it */
void he_RunSearch9(AIJOB *ev)
{
    HESEARCH9 *s = (HESEARCH9 *)ev;
    AIBLD *bld;
    AIOBJ *src;
    int i;
    char text[0x200];
    AIOBJ *found;
    AIOBJ *o;
    GMGOOD *g;
    AICHAR *rec;
    int j;
    int ok;
    int amount;

    switch (ev->stage) {
    case 0:
        ev->stage++;
        return;
    case 1:
        if (ev->words14[0] == 0 && ev->words14[1] == 0)
            break;
        if (gm_CompareTime(&ev->end, &ai_Time) >= 0)
            return;
        gm_FindById((int *)&bld, 0, 0, s->bld);
        if (bld == 0) {
            ev->stage = -1;
            return;
        }
        gm_FindById(0, (int *)&src, 0, s->source);
        for (found = (AIOBJ *)gm_Query((void *)src->list, 1, 4, 0x17); found != 0;
             found = (AIOBJ *)gm_QueryNext()) {
            if (found->count == 1)
                break;
        }
        if (found != 0) {
            cm_SendExSellObject(src->objId, -1, found->id, 1, ai_OwnPlayer, 0);
            gm_BuildKindObjects((char *)bld);
            for (i = 0; i < gm_KindObjCount; i++) {
                g = GM_GOOD(gm_KindObjs[i]);
                if (g->type == 0x17)
                    continue;
                ok = 1;
                for (j = 0; ok && j < 4; j++) {
                    if (g->in[j] != 0 && g->in[j] != found->id) {
                        o = (AIOBJ *)gm_Query((void *)src->list, 1, 0, g->in[j]);
                        if (o == 0 || o->count < 2)
                            ok = 0;
                    }
                }
                if (ok)
                    cm_SendExSellObject(src->objId, -1, gm_KindObjs[i], 3, ai_OwnPlayer, 0);
            }
            g = GM_GOOD(found->id);
            amount = (util_RandomU(g->batch * 30) + g->batch * 30) * (ev->words14[0] + ev->words14[1]);
            cm_SendExSellObject(s->store, -1, found->id, amount, ai_OwnPlayer, 0);
            txt_Format(text, 0x1bb8, found->id * 2 + 0x9a8, bld, amount);
            he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, -1, text, 0, 0x5f1, bld->id, -1, 0, "_NACHRICHTEN_HS_08");
        }
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup((unsigned char *)bld, 0, rec->id, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        for (rec = ai_CharsT; (int)rec < (int)&ai_CharsT[768]; rec++) {
            if (rec->job == ev)
                sv_MoveToGroup(0, 0, rec->id, 0);
        }
        break;
    default:
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x56a710, the run handler of the new-occurrence debug event 0xa ("HE_NEUE_VORKOMMEN_SUCHEN_MSG"): stage 0 opens
   the message panel "Misc\EventMessagebox" (he_OpenEventSlot, its slot at +0x74), shows the bug text with the name of
   the good and goes to stage 1; stage 1 frees the event once that panel's window was clicked on (d2_ClickedForm, the
   id 0); -1 / -2 release the slot and free it */

void he_RunNewOccurrenceMsg(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEVORKOMMEN *v = (HEVORKOMMEN *)ev;
    int good;

    switch (e->stage) {
    case -2:
    case -1:
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    case 0:
        good = v->good;
        he_OpenEventSlot(ev, 0, 0, "Misc\\EventMessagebox", 0x5f2);
        d2_SetForm(e->slot->win, 0);
        io_PrintText("HE_NEUE_VORKOMMEN_SUCHEN_MSG: Ich bin ein Bug !!! %s gefunden!!", good * 2 + 0x9a8);
        e->stage++;
        return;
    case 1:
        if (d2_ClickedForm == e->slot->win && d2_ClickedId != -1 && d2_ClickedId == 0) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
        }
        return;
    }
}


/* 0x56a7b0, the allocation handler of the trade event 0xb: when another event of the same kind already works for the same good (0xac) of another sender (0x10) this record is freed; else both clocks (0x52 / 0x60) are the start clock (0x44), due 1 unit later, the handles 0xe8 / 0xec = -1, and a move-goods message 0xf (AIMSG: type 0xf, to the character chr, fromId -1, the clock, the good (arg[0]), the buyer (from, arg[1]), the building (0xb0, arg[2]), the event id (0xa0)) is posted unless the flag 0xf1 is set (then due 1 + 0x1e later); modes 0xf0 = 4 when the character (0xb4) exists
   @deep read the whole function against the original and rebuilt it on AIJOB / AIMSG: the same-kind /
   same-good check over he_FindEvent, the two clocks, the handles -1, the move-goods message 0xf (type, to
   = the character's netId, fromId -1, the clock, arg[0] the good, arg[1] the buyer, arg[2] the building,
   0xa0 the event id), the flag 0xf1 branch, modes 4. Only scheduling / allocation is left: the msg setup
   is one scheduling region - the original hoists ev->start.year into edx above the memset (mov edx,[ebx]
   before rep stosd) and interleaves the clock stores (year 0x28, secword 0x34, min 0x30, hourword 0x2c)
   with type / fromId / to / flags / arg[], ours stores the four clock fields together after the memset;
   plus the eax / ecx register swaps in the arg stores. 130 vs 129 instructions. Tried: the typed AIMSG
   field assignments in the source order and the HETRADEMSG view for 0xa0; the store set is identical. */
void he_AllocTrade(AIJOB *ev)
{
    HETRADE *p = (HETRADE *)ev;
    AIMSG msg;
    AIJOB *e2;

    for (e2 = (AIJOB *)he_FindEvent(2, 0, 0xb, 3, ev->from); e2 != 0; e2 = (AIJOB *)he_NextEvent()) {
        if (p->cart == ((HETRADE *)e2)->cart && e2->id != ev->id) {
            he_FreeEvent((int *)ev);
            return;
        }
    }
    ev->end = ev->start;
    ev->clock60 = ev->start;
    p->moveSlot = -1;
    p->msgSlot = -1;
    gm_AddTime(&ev->end, 0, 1, 0);
    memset(&msg, 0, sizeof(msg));
    msg.time = ev->start;
    msg.type = 0xf;
    msg.fromId = -1;
    msg.to = ai_CharsT[ev->chr].netId;
    msg.flags = 2;
    msg.arg[2] = p->bld;
    msg.arg[1] = ev->from;
    msg.arg[0] = p->cart;
    ((HETRADEMSG *)&msg)->jobId = ev->id;
    if (gm_FindObjectInst(p->cart) != 0) {
        if (ev->flags != 0)
            gm_AddTime(&ev->end, 1, 0x1e, 0);
        else
            p->msgSlot = cm_SendLongMessage((char *)&msg);
    } else
        he_FreeEvent((int *)ev);
    ev->modes = 0;
    if (gm_FindChar(p->chr) != 0)
        ev->modes = 4;
}


/* 0x56a960, the run handler of the trade event 0xb (he_AllocTrade): a cart (t->cart) of the character t->chr trades
   at the building t->bld. Stage -2 restarts it for the player's character. While its move or message send is still
   open it waits a minute. Without the cart or the building it is freed; a transport event 0xf of the character
   for the same cart makes it wait 5 minutes (going to this building) or frees it. A cart not at the building
   (ai_CartAtBuilding, unless deferred) frees it. With nothing to sell or buy in its slots (at most the cart
   object's slot count, 3) the cargo of the cart object (0x1dd) goes into the building's store (kind 0x2a, else
   0x116). Else the sell slots for this building sell their goods: what the store has room for at 0.9 of the
   price (booked to the character's money +0x1b8), the rest at 0.8 to nobody; or the buy slots buy up to their
   amount of what the store (the market object of a market building) holds beyond its reserve, at 1.1 of the
   price (a market) or the building's price factor, as far as the money of the character and its master reach
   ("Meister %s (%s) kauft ..."); a buy that found nothing is tried again every half hour up to t->tries times
   (before 20:00). Then the cart moves on to the building of the first slot left, else home (ev->from): the
   message 0xf, unless that is this building (5 minutes) or the trade is deferred (1 1/2 hours)
   @deep size queue s20: 420 -> 224 ins, frame 0x1b0 -> 0x1a8 (the original's), raw 2603 -> 2580 (padded
   2592 = the original's): the sell / buy loops as plain index loops over t->sell[i] / t->sellBld[i] (no
   hand-advanced slotBld / slotGood pointers: 218 alone) and home a local of the if (t->bld == -1) block
   (escaped by &home; a function-scope escaped local conflicts with every slot, the original shares it).
   buf in the sprintf block: worse (code 240). climb.py round 1: only a meaning change (t->bld through a
   float). Left: regs 148 (the zero constant in ebp in the original, edi in ours), slots 152, code 80 (the
   transport-match block placement noted below). prior: rebuilt from the whole original over HETRADE (the
   trade event's words named: cart, bld, chr, the sell / buy slots, buyAmount, the two send slots, tries /
   deferred) and AIBLD.priceFactor (0x49): the waits on the two send slots, the transport check,
   ai_CartAtBuilding, the cargo object 0x1dd and its slot count, the store move of the cargo, the sell loop
   (0.9 / 0.8 prices, the budget), the buy loop (min() of the amount wanted and the stock beyond the
   reserve, evaluated twice as the original does; the money of the character and its master; the debug
   sprintf), the half-hour retries, the next building and the move-on message. 857 vs 851 instructions.
   Left is allocation and block placement: the zero constant is ebp in the original and edi in ours (cmp
   esi, ebp / jle vs cmp esi, edi), the loop pointers and the shared stack slots differ (frame 0x1b0 vs
   0x1a8: the original shares left / the sell slot pointer at 0x4 and n / the price at 0x8), and the
   original keeps the transport-match block (+0x9e8: cmp goal, bld; jne free; then the wait tail) at the
   end where ours places it after the ai_CartAtBuilding free (+0x16b); the goto / break spellings gave the
   same layout (references: M8 at +0x2ff: ours refers to gm_AddTime (0x4fe140), the original's field to
   0x53e1f0 (he_FreeEvent), explained: the M8 at +0x2ff (gm_AddTime vs he_FreeEvent) is that block
   placement: the alignment pairs our wait call with the original's free call of the nothing path (if
   moveSlot == -1 free, else wait); both calls exist on both sides) */
void he_RunTrade(AIJOB *ev)
{
    HETRADE *t = (HETRADE *)ev;
    AIBLD *bld;
    int left;
    int n;
    AIOBJ *store;
    AIOBJ *cart;
    AICHAR *chr;
    int price;
    AIOBJ *cargo;
    int slots;
    int flags;
    AIMSG m;
    char buf[0x80];
    AIJOB *e;
    AIOBJ *g;
    int i;
    int found;
    int have;
    int put;

    n = 0;
    left = 0;
    flags = 0;
    cart = 0;
    bld = 0;
    chr = 0;
    if (ev->stage == -2) {
        ev->stage = 0;
        he_SetEventChar(ev, he_PlayerCh->id);
    }
    if (t->moveSlot != -1) {
        if (cm_GetSlotState(t->moveSlot) == 0) {
            gm_AddTime(&ev->end, 0, 1, 0);
            return;
        }
        t->moveSlot = -1;
    }
    if (t->msgSlot != -1) {
        if (cm_GetSlotState(t->msgSlot) == 0) {
            gm_AddTime(&ev->end, 0, 1, 0);
            return;
        }
        t->msgSlot = -1;
    }
    gm_FindById(0, (int *)&cart, 0, t->cart);
    gm_FindById((int *)&bld, 0, 0, t->bld);
    gm_FindById(0, 0, (int *)&chr, t->chr);
    if (cart == 0 || bld == 0)
        goto done;
    for (e = (AIJOB *)he_FindEvent(2, 0, 0xf, 2, ev->chr); e != 0; e = (AIJOB *)he_NextEvent()) {
        if (((HETRANSPORT *)e)->cart == t->cart)
            goto moving;
    }
    if (t->deferred == 0 && ai_CartAtBuilding(bld, cart) == 0) {
        ai_CartAtBuilding(bld, cart);
        he_FreeEvent((int *)ev);
        return;
    }
    cargo = (AIOBJ *)gm_Query((void *)cart->list, 1, 0, 0x1dd);
    slots = cargo->lvl1c < 3 ? cargo->lvl1c : 3;
    found = 0;
    for (i = 0; found == 0 && i < slots; i++) {
        if (t->sell[i] != 0 || t->buy[i] != 0)
            found = 1;
    }
    if (found == 0) {
        if ((store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a)) != 0) {
            for (g = (AIOBJ *)gm_Query((void *)cargo->list, 1, 5); g != 0; g = (AIOBJ *)gm_QueryNext())
                t->moveSlot = cm_SendExSellObjectH(store->objId, cargo->objId, g->id, g->count, ai_OwnPlayer, 0);
        } else if ((store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x116)) != 0) {
            for (g = (AIOBJ *)gm_Query((void *)cargo->list, 1, 5); g != 0; g = (AIOBJ *)gm_QueryNext())
                t->moveSlot = cm_SendExSellObjectH(store->objId, cargo->objId, g->id, g->count, ai_OwnPlayer, 0);
        }
        if (t->moveSlot == -1)
            goto done;
        goto wait;
    }
    found = 0;
    for (i = 0; found == 0 && i < slots; i++) {
        if (t->sell[i] != 0)
            found = 1;
    }
    if (found != 0) {
        for (i = 0; i < slots; i++) {
            if (t->sell[i] != 0 && t->sellBld[i] == bld->id) {
                g = (AIOBJ *)gm_Query((void *)cargo->list, 1, 0, t->sell[i]);
                if (g != 0) {
                    if (ai_BTypesT[bld->type].kind == 0xa)
                        store = (AIOBJ *)gm_FindMarketObject(t->sell[i]);
                    else if ((store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a)) == 0)
                        store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x116);
                    if (store != 0) {
                        have = gm_ListAmount(cargo, g);
                        put = gm_FreeCapacity(store, g->id, have);
                        price = (int)(gm_GoodsPrice(g->id, ai_OwnPlayer) * 0.9);
                        if (have < put)
                            put = have;
                        cm_SendExSellObject(store->objId, cargo->objId, g->id, put, ai_OwnPlayer, price);
                        if (chr != 0)
                            chr->budget += price * put;
                        if ((have -= put) != 0)
                            t->moveSlot = cm_SendExSellObjectH(-1, cargo->objId, g->id, have, ai_OwnPlayer,
                                                               (int)(gm_GoodsPrice(g->id, ai_OwnPlayer) * 0.8));
                    }
                }
                t->sell[i] = 0;
                t->sellBld[i] = -1;
            }
        }
    } else {
        if (chr != 0 && chr->bld != 0) {
            left = gm_CharCurrency((char *)&ai_CharsT[chr->bld->owner], ai_OwnPlayer);
            if (left >= chr->budget)
                left = chr->budget;
        }
        for (i = 0; i < slots; i++) {
            if (t->buy[i] != 0 && t->buyBld[i] == bld->id) {
                if (ai_BTypesT[bld->type].kind == 0xa)
                    store = (AIOBJ *)gm_FindMarketObject(t->buy[i]);
                else if ((store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a)) == 0)
                    store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x116);
                if (store != 0) {
                    g = (AIOBJ *)gm_Query((void *)store->list, 1, 0, t->buy[i]);
                    if (g == 0)
                        continue;
                    if (ai_BTypesT[bld->type].kind == 0xa)
                        price = (int)(gm_GoodsPrice(g->id, ai_OwnPlayer) * 1.1);
                    else
                        price = (int)(gm_GoodsPrice(g->id, ai_OwnPlayer) * bld->priceFactor);
                    n = min(t->buyAmount[i], gm_ListAmount(store, g) - ((unsigned)gm_ListStock(store, g->id) * g->reserve >> 2));
                    n = min(n, left / price);
                    n = n < 0 ? 0 : n;
                    if (n != 0)
                        t->moveSlot = cm_SendExSellObjectH(cargo->objId, store->objId, g->id, n, ai_OwnPlayer, price);
                    if (chr != 0 && chr->bld != 0)
                        sprintf(buf, "Meister %s (%s) kauft f\xfcr %li Credits (hat %li) %li Einheiten %s", chr->name,
                                chr->bld->name, price * n, left, n, GM_GOOD(g->id)->name);
                    left -= price * n;
                    if (chr != 0)
                        chr->budget -= price * n;
                }
                if (n == 0 && t->tries != 0) {
                    flags |= 1;
                } else {
                    t->buy[i] = 0;
                    t->buyBld[i] = -1;
                }
            }
        }
    }
    if (t->tries != 0 && flags != 0) {
        if (ai_Time.hour < 20) {
            gm_AddTime(&ev->end, 0, 30, 0);
            t->tries--;
        } else {
            gm_AddTime(&ev->end, 0, 5, 0);
            t->tries = 0;
        }
        return;
    }
    t->bld = -1;
    for (i = 0; t->bld == -1 && i < slots; i++) {
        if (t->sell[i] != 0)
            t->bld = t->sellBld[i];
    }
    if (t->bld == -1) {
        for (i = 0; t->bld == -1 && i < slots; i++) {
            if (t->buy[i] != 0)
                t->bld = t->buyBld[i];
        }
        if (t->bld == -1) {
            AIBLD *home;

            gm_FindById((int *)&home, 0, 0, ev->from);
            t->bld = home->id;
        }
    }
    if (t->deferred != 0) {
        gm_AddTime(&ev->end, 1, 30, 0);
        return;
    }
    memset(&m, 0, sizeof(m));
    m.type = 0xf;
    m.fromId = -1;
    m.flags = 2;
    m.to = ai_CharsT[ev->chr].netId;
    m.time = ai_Time;
    m.car = cart->objId;
    m.n = bld->id;
    m.ch = t->bld;
    ((HETRADEMSG *)&m)->jobId = ev->id;
    if (bld->id == t->bld) {
        gm_AddTime(&ev->end, 0, 5, 0);
        return;
    }
    t->msgSlot = cm_SendLongMessage((char *)&m);
    gm_AddTime(&ev->end, 0, 10, 0);
    return;
moving:                                 /* a transport of the cart runs: wait for it when it comes here */
    if (((HETRANSPORT *)e)->goal != t->bld)
        goto done;
wait:
    gm_AddTime(&ev->end, 0, 5, 0);
    return;
done:
    he_FreeEvent((int *)ev);
}
/* 0x56b380, the allocation handler of the research event 0xc (he_RunResearch): both clocks (end and clock60) start
   at the event's start clock, the end is one minute later, and the order's countdown words (wbc, limit, rounds)
   are cleared */

void he_AllocResearch(AIJOB *ev)
{
    HERESEARCH *r = (HERESEARCH *)ev;

    ev->end = ev->start;
    ev->clock60 = ev->start;
    gm_AddTime(&ev->end, 0, 1, 0);
    r->wbc = 0;
    r->limit = 0;
    r->rounds = 0;
}

void init_HookObject(struct D3OBJECT *o, void *p, int kind);

/* 0x56b3f0, the run handler of the research event 0xc (HERESEARCH): stage 0, once due, orders the good it
   researches (cm_SendExSellObjectH: the building +0xac as buyer, the good +0xb4, one piece) and keeps the send
   slot (+0xc8); stage 1 waits for that slot (cm_GetSlotState): an error (2) frees the event, else the objId of
   the object the order made (+0xb8, the reply's AIOBJ.objId) is kept and 0x20 rounds (+0xc0 / +0xc4) are
   counted from now; stage 2 looks the object up each minute (gm_FindById) and at the end of the countdown hooks
   its scene object into the building (init_HookObject) and frees the event */
void he_RunResearch(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HERESEARCH *r = (HERESEARCH *)ev;
    AIOBJ *o;

    switch (e->stage) {
    case 0:
        if (gm_CompareTime(&e->end, &ai_Time) > 0)
            return;
        r->slot = cm_SendExSellObjectH(r->buyer, -1, r->good, 1, ai_OwnPlayer, 0);
        e->stage++;
        return;
    case 1:
        if (cm_GetSlotState(r->slot) == 0)
            return;
        if (cm_GetSlotState(r->slot) == 2)
            goto done;
        o = (AIOBJ *)cm_GetSlotValue(r->slot);
        r->objId = o->objId;
        r->limit = 0x20;
        r->rounds = 0;
        gm_AddTime(&e->end, 0, 1, 0);
        e->stage++;
        return;
    case 2:
        if (gm_CompareTime(&e->end, &ai_Time) >= 0)
            return;
        do {
            gm_FindById(0, (int *)&o, 0, r->objId);
            gm_AddTime(&e->end, 0, 1, 0);
            if (r->rounds > r->limit) {
                init_HookObject(o->obj, o, 1);
                he_FreeEvent(ev);
                return;
            }
        } while (gm_CompareTime(&e->end, &ai_Time) < 0);
        return;
    case -2:
    case -1:
        goto done;
    }
    return;
done:
    he_FreeEvent(ev);
}

int ai_FindBauplaetze();
int ai_FindNearestObject(float *ref, int *list);  /* 0x58fe10 (lc.c) */

/* 0x56b570, "he_AllocGebaeudeBauen", the alloc of an event of the type 0xd (when another one of the type is still
   below the stage 5 the record is freed): both clocks (end / clock60) = the start clock, due now + 1, workTotal /
   argc8 = 0; the name of the building type (bldType) is looked up (ai_BuildingName into buf) and its plots are
   searched in the scene 0 (ai_FindBauplaetze into tab), each checked against the position pos (ai_FindNearestObject);
   without plots or with a miss the record is freed, else the error text is formatted into buf and exSlot = -1 */
void he_AllocGebaeudeBauen(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEBUILD *b = (HEBUILD *)ev;
    char buf[0x80];
    char tab[0x400];
    int e2, n, i, scene;

    for (e2 = (int)he_FindEvent(1, 0, 0xd); e2 != 0; e2 = he_NextEvent()) {
        if (e2 != ev && ((AIJOB *)e2)->stage < 5)
            goto done;
    }
    e->end = e->start;
    e->clock60 = e->start;
    gm_AddTime(&e->end, 0, 1, 0);
    e->workTotal = 0;
    e->argc8 = 0;
    if (ai_BuildingName(b->bldType, buf) == 0)
        goto done;
    scene = d3_CurrentScene;
    memset(tab, 0, sizeof(tab));
    d3if_SelectScene(0, 1);
    n = ai_FindBauplaetze(buf, tab);
    if (n == 0) {
        d3if_SelectScene(scene, 1);
        he_FreeEvent((int *)ev);
        return;
    }
    for (i = 0; i < n; i++) {
        if (ai_FindNearestObject(b->pos, tab) == 0) {
            he_FreeEvent((int *)ev);
            d3if_SelectScene(scene, 1);
            return;
        }
    }
    txt_Format(buf, "he_AllocGebaeudeBauen: %1N1", e->chr);
    d3if_SelectScene(scene, 1);
    e->exSlot = -1;
    return;
done:
    he_FreeEvent((int *)ev);
}
/* 0x56b700, the object callback of the building event 0xd (o: its pos +0x4c and rotation +0x84): places the object
   at its position with its rotation lifted by 1.47 (d3t_PlaceObjectV, the kind 0x46) and counts the event's
   placements (+0xd0) down */
void he_PlaceGebaeudeObject(struct D3OBJECT *o, HEBUILD *ev)
{
    D3VECTOR t;

    t.x = o->rotation.x + 1.47;
    t.y = o->rotation.y;
    t.z = o->rotation.z;
    d3t_PlaceObjectV(o, &o->pos, &t, 0x46, 0x20);
    ev->count -= 1;
}
/* 0x56b750, scene callback: appends the object o to the list (0x40 entries, the count at +0x104); true while there is room */

char he_CollectChild(int o, int *list)
{
    list[list[0x41] + 1] = o;
    list[0x41]++;
    return list[0x41] < 0x40;
}


/* 0x56b780, shows the construction stage of the building event ev on its scene object "he_<id>" (in the default
   scene, the current one restored at the end): not started (stage -1) every child (he_CollectChild) becomes a kind 5
   object out of the quad tree and the tree is hidden; below the last stage each child is shown or hidden by the
   numbers its name ends in ("..._n": from stage n on, "..._n1_n2": for the stages n2 .. n1); past the last stage
   the building's own object shows and the construction object is deleted */
void he_ShowConstruction(int ev)
{
    HECONSTRUCTION *c = (HECONSTRUCTION *)ev;
    int i;
    int scene;
    char digits[0x10];
    int nums[8];
    HECHILDLIST list;
    char part[0x80];
    char name[0x100];
    D3OBJECT *obj;
    D3OBJECT *o;
    AIBLD *bld;
    char *p;
    int n;

    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    sprintf(name, "he_%i", ((AIJOB *)ev)->id);
    obj = d3_FindObject(0, 0x1c0, 0, name);
    if (c->stage == -1 && obj != 0) {
        list.count = 0;
        list.root = obj;
        d3_ForEachObject(d3_DefaultScene, obj, 0x2c0, (D3OBJECTCB)he_CollectChild, &list);
        for (i = 0; i < list.count; i++) {
            o = list.items[i];
            if (o->kind != 5 || (o->flags & 4)) {
                o->kind = 5;
                if ((list.items[i]->flags & 4) && d3_QuadTree != 0)
                    d3q_RemoveObject(list.items[i], (D3QNODE *)d3_QuadTree);
                list.items[i]->flags &= ~4;
            }
        }
        d3if_ShowTree(obj, 0);
    } else if (c->stage < c->stages && obj != 0) {
        list.count = 0;
        list.root = obj;
        d3_ForEachObject(d3_DefaultScene, obj, 0x2c0, (D3OBJECTCB)he_CollectChild, &list);
        for (i = 0; i < list.count; i++) {
            o = list.items[i];
            if (o->kind != 5 || (o->flags & 4)) {
                if ((o->flags & 4) && d3_QuadTree != 0)
                    d3q_RemoveObject(o, (D3QNODE *)d3_QuadTree);
                list.items[i]->kind = 5;
                list.items[i]->flags &= ~4;
            }
            n = 0;
            list.items[i]->mask = 0;
            strcpy(part, list.items[i]->name);
            for (p = strrchr(part, '_'); p != 0 && n < 2; p = strrchr(part, '_')) {
                strncpy(digits, p + 1, 2);
                digits[2] = 0;
                if (isdigit(digits[0]))
                    nums[n++] = atoi(digits);
                *p = 0;
            }
            if (n == 1) {
                if (c->stage >= nums[0])
                    d3if_ShowTree(list.items[i], 1);
                else
                    d3if_ShowTree(list.items[i], 0);
            } else if (n == 2) {
                if (c->stage <= nums[0] && c->stage >= nums[1])
                    d3if_ShowTree(list.items[i], 1);
                else
                    d3if_ShowTree(list.items[i], 0);
            }
        }
    } else if (c->stage > c->stages) {
        bld = (AIBLD *)gm_FindBuildingById(c->bld);
        if (bld != 0 && bld->object != 0)
            d3if_ShowTree((D3OBJECT *)bld->object, 1);
        if (obj != 0) {
            if (d3_QuadTree != 0)
                d3q_RemoveObject(obj, (D3QNODE *)d3_QuadTree);
            d3if_Delete(obj);
        }
    }
    d3if_SelectScene(scene, 1);
}
/* 0x56bac0, the construction stage the scene object "he_<id>" of the building event ev shows: the highest number
   among the "_nn" suffixes of its children's names (he_CollectChild, in the default scene, the current one restored
   at the end); after cutting a suffix the next search starts at the cut, so only the last suffix of each name is read */
int he_ConstructionStage(int ev)
{
    int best;
    int scene;
    char digits[0x10];
    int nums[8];
    HECHILDLIST list;
    char part[0x80];
    char name[0x80];
    char *p;
    int i, n;

    best = 0;
    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    sprintf(name, "he_%i", ((AIJOB *)ev)->id);
    list.root = d3_FindObject(0, 0x1c0, 0, name);
    list.count = 0;
    d3_ForEachObject(d3_DefaultScene, list.root, 0x2c0, (D3OBJECTCB)he_CollectChild, &list);
    for (i = 1; i < list.count; i++) {
        n = 0;
        list.items[i]->mask = 0;
        strcpy(part, list.items[i]->name);
        for (p = strrchr(part, '_'); p != 0 && n < 2; p = strrchr(p, '_')) {
            strncpy(digits, p + 1, 2);
            digits[2] = 0;
            if (isdigit(digits[0]))
                nums[n++] = atoi(digits);
            *p = 0;
        }
        if (n != 0 && nums[0] > best)
            best = nums[0];
    }
    d3if_SelectScene(scene, 1);
    return best;
}

/* 0x627e98, the construction models of the building types (he_b.c's he_CreateConstruction): the types of each record, then
   its scaffolding model; the record with type 0 ends the table and names the model for the types not listed */
HEBLDMODEL he_BuildingModels[37] = {
    { { 8, 9, 10, 0 }, "*bau_BRUNNEN_a.ogr" },
    { { 5, 0, 0, 0 }, "*bau_BUERGERHAUS_a.ogr" },
    { { 17, 0, 0, 0 }, "*bau_GEFAENGNIS_a.ogr" },
    { { 18, 19, 0, 0 }, "*bau_GEFAENGNIS_b.ogr" },
    { { 50, 51, 52, 0 }, "*bau_HANDWERK_b.ogr" },
    { { 33, 34, 35, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 14, 15, 16, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 41, 42, 43, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 26, 27, 28, 0 }, "*bau_HANDWERK_d.ogr" },
    { { 4, 2, 0, 0 }, "*bau_HAUS_a.ogr" },
    { { 3, 1, 0, 0 }, "*bau_HAUS_a.ogr" },
    { { 22, 0, 0, 0 }, "*bau_KIRCHE_GROSS_a.ogr" },
    { { 20, 0, 0, 0 }, "*bau_KIRCHE_KLEIN_a.ogr" },
    { { 21, 0, 0, 0 }, "*bau_KIRCHE_MITTEL_a.ogr" },
    { { 44, 0, 0, 0 }, "*bau_STADTWACHE_a.ogr" },
    { { 45, 0, 0, 0 }, "*bau_STADTWACHE_b.ogr" },
    { { 46, 0, 0, 0 }, "*bau_STADTWACHE_b.ogr" },
    { { 11, 12, 13, 0 }, "*bau_STADTWACHE_b.ogr" },
    { { 23, 24, 25, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 47, 48, 49, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 53, 54, 55, 0 }, "*bau_WIRTSHAUS_a.ogr" },
    { { 6, 7, 0, 0 }, "*bau_WOHNSITZ_GROSS_a.ogr" },
    { { 57, 58, 56, 0 }, "*bau_ZUNFTHAUS_a.ogr" },
    { { 60, 61, 59, 0 }, "*bau_ZUNFTHAUS_a.ogr" },
    { { 63, 64, 62, 0 }, "*bau_ZUNFTHAUS_a.ogr" },
    { { 66, 67, 65, 0 }, "*bau_ZUNFTHAUS_a.ogr" },
    { { 75, 0, 0, 0 }, "*bau_DOMIZIL_LANDSITZ.ogr" },
    { { 76, 0, 0, 0 }, "*bau_DOMIZIL_BURG.ogr" },
    { { 77, 0, 0, 0 }, "*bau_DOMIZIL_SCHLOSS.ogr" },
    { { 84, 85, 86, 0 }, "*bau_BRUNNEN_a.ogr" },
    { { 81, 0, 0, 0 }, "*bau_HANDWERK_SCHNEIDER_a.ogr" },
    { { 82, 83, 0, 0 }, "*bau_HANDWERK_SCHNEIDER_b.ogr" },
    { { 79, 78, 80, 0 }, "*bau_ZIGEUNER_ZIGEUNER_a.ogr" },
    { { 72, 0, 0, 0 }, "*bau_FRIEDHOF_TOTENFELD.ogr" },
    { { 73, 0, 0, 0 }, "*bau_FRIEDHOF_GOTTESACKER.ogr" },
    { { 74, 0, 0, 0 }, "*bau_FRIEDHOF_FRIEDHOF.ogr" },
    { { 0, 0, 0, 0 }, "*bau_HAUS_a.ogr" }
};

/* 0x56bc50, creates the two scene objects of the building event ev in the default scene (the current one restored
   at the end): the construction model of the building type *type (he_BuildingModels; a type not listed gets the
   model of the closing record), named "he_<id>", and the building's own group model, hidden, dropped on the ground
   (not below pos) and hooked to the building (init_HookObject); both are placed at pos and rot, the construction
   shows its first stage (he_ShowConstruction, not started); returns the building's object, 0 when a load failed */
D3OBJECT *he_CreateConstruction(D3VECTOR *rot, D3VECTOR *pos, unsigned char *type, char *model, int ev)
{
    int scene;
    char name[0x100];
    D3OBJECT *obj;
    int n, k, found;

    n = 0;
    found = 0;
    if (he_BuildingModels[0].types[0] != 0) {
        for (; he_BuildingModels[n].types[0] != 0; n++) {
            if (found)
                break;
            for (k = 0; k < 4; k++) {
                if (*type == he_BuildingModels[n].types[k]) {
                    strcpy(name, he_BuildingModels[n].model);
                    found = 1;
                    break;
                }
            }
        }
    }
    if (!found)
        strcpy(name, he_BuildingModels[n].model);
    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    obj = d3_LoadGroup(name, 0, 0);
    if (obj == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3_SetObjectPos(obj, pos);
    d3_SetObjectRot(obj, rot);
    obj->objectFlags = obj->objectFlags & ~4 | 2;
    sprintf(obj->name, "he_%i", ((AIJOB *)ev)->id);
    obj = d3_LoadGroup(model, 0, 0);
    if (obj == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3_SetObjectPos(obj, pos);
    d3_SetObjectRot(obj, rot);
    init_StripTorches(obj);
    d3t_DropObject(obj, 1, 0);
    if (obj->pos.y < pos->y) {
        obj->pos.x = pos->x;
        obj->pos.y = pos->y;
        obj->pos.z = pos->z;
    }
    d3if_ShowTree(obj, 0);
    d3_RelightObjects(obj);
    ((HECONSTRUCTION *)ev)->stage = -1;
    ((HECONSTRUCTION *)ev)->script = -1;
    he_ShowConstruction(ev);
    init_HookObject(obj, type, 2);
    d3if_SelectScene(scene, 1);
    return obj;
}


/* @original the construction event's run handler stores what gm_AddToTreasury leaves in eax as the payment's slot
   (0x56bf1a .. 0x56bf22): it called the function without a prototype, as returning int */
#define gm_AddToTreasuryI ((int (*)(int))gm_AddToTreasury)
void sv_CreateBanners(char *b);

/* 0x56be30, the run handler of the building construction event 0xd, by its stage: 0 pays the price when due (the
   player's own money to the treasury, else moved from the payer), 1 waits for the payment, 2 sends the building
   message 10, 3 takes the new building from its answer, 4 creates the construction objects (he_CreateConstruction)
   and sets the work minutes per stage, 5 sends the "vg_" objects of the site to the building nearest to it
   (SpecialEvents\\Hausbau.esc), 6 deletes them once placed, 7 counts the work minutes into stages (the workers'
   script SpecialEvents\\Hausbau_2.esc from stage 2 on) and at the last one shows the building, builds its road,
   sends the news and moves the owner in; -1 / -2 tear the construction down; every stage but the waiting ones goes
   on through the cm slot cm_SendExSetHE gives
   @deep size queue s16: 202 -> 70 ins, raw 4000 = the original's padded size. Kept: stage 0's two exits
   (payer gone, the success send) end with break (retgen sites 0+1: the original's stage 0 jumps into stage
   3's copies), stage 3's bld test written positive (if (bld != 0) { ..; send(stage + 1); return; }
   send(-1); return;: its failure send cross-jumps into stage 1's copy as in the original), the bounding
   sphere into its own center / radius locals (the original's lea [esp+0x20] / [esp+0x14]; &v / &best kept
   best address-taken and moved every slot). Left: only frame slots (frame 0x3a4 vs 0x38c): the original
   shares [0x14] best / i / radius, [0x18] dist / nearest, [0x30] v / now, center at [0x20]. Block-scoping
   center+radius, now, at, v (31 subsets) gives frame 0x388 at best (sphere+now+at 67 ins, but raw 4004,
   padded too big); a sphere with &at / &v (any radius) brings back 2400 bytes of slot hunks. Next: c2frame
   on sphere+now+at for the 4 missing bytes. prior: reconstructed the whole function (1213 vs 1211
   instructions) against the original 0x56be30..0x56cda7, every stage 0..7 and -1/-2 read and written
   (HECONSTRUCTION words, HEFINVIEW.buildPaid, gm_AddToTreasury's eax kept @original); no call, store or
   reference differs (diffclass: no differs/ref lines). Left: (1) VC6 cross-jumping of the identical
   cm_SendExSetHE tails in stages 0..3 lands the other way: the original's stage 0 jumps forward into stage
   3's copies (0x56bf0e jmp 0x56c050, je 0x56c00a) and stage 3's bld==0 into stage 1's (je 0x56bf6f), ours
   keeps stage 0's copies and stage 3 jumps back (code hunks +0x9c, +0x10c, +0x200..+0x225; gotos to labels
   in stage 3 tried: worse, 1228 instructions, VC6 moves labelled blocks out of line); (2) the frame 0x394
   vs 0x38c and the slot packing: the original shares [esp+0x14] best / i / radius, [esp+0x18] dist /
   nearest, [esp+0x30] the PointToWorld ... */
void he_RunConstruction(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HECONSTRUCTION *c = (HECONSTRUCTION *)ev;
    AIBLD *bld;
    float best;
    float dist;
    float radius;
    D3VECTOR center;
    int scene;
    D3VECTOR v;
    D3VECTOR at;
    AITIME now;
    int minutes;
    AIBUILDSITE site;
    AIVGLIST list;
    union {
        char name[0x80];                /* the construction object's name "he_<id>" */
        int msg[0x1f];                  /* the data of the system message 0xc (cm_SendExSysMessage copies 0x7c bytes) */
    } u;
    char text[0x200];
    AIBLD *nearest;
    D3OBJECT *door;
    D3OBJECT *o;
    D3OBJECT *house;
    EVTSCRIPT *sc;
    AICHAR *ch;
    AIBLD *home;
    HEFINVIEW *fin;
    AIBTYPE *bt;
    int i, size, season;

    scene = d3_CurrentScene;
    bld = 0;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    switch (e->stage) {
    case 0:
        if (gm_CompareTime(&e->end, &ai_Time) > 0)
            break;
        if (!(e->opts & 2))
            break;
        if (c->payer != he_PlayerCh->netId) {
            ch = (AICHAR *)gm_FindChar(c->payer);
            if (ch == 0) {
                e->exSlot = cm_SendExSetHEI(-1, ev);
                break;
            }
            c->slot = cm_SendMoveGoods(-1, c->payer, ai_OwnPlayer, c->carry);
            fin = (HEFINVIEW *)gm_PlayerRecord((char *)ch);
            if (fin != 0)
                fin->buildPaid += c->carry;
        } else {
            c->slot = gm_AddToTreasuryI(c->carry);
        }
        e->exSlot = cm_SendExSetHEI(e->stage + 1, ev);
        break;
    case 1:
        e->exSlot = -1;
        if (e->opts & 4)
            break;
        if (cm_GetSlotState(c->slot) == 0)
            break;
        if (cm_GetSlotState(c->slot) == 2) {
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        e->exSlot = cm_SendExSetHEI(e->stage + 1, ev);
        return;
    case 2:
        if (!(e->opts & 2))
            break;
        c->slot = cm_SendMessage10((unsigned char)c->bldType, c->payer, 0, 1);
        e->exSlot = cm_SendExSetHEI(e->stage + 1, ev);
        return;
    case 3:
        e->exSlot = -1;
        if (e->opts & 4)
            break;
        if (cm_GetSlotState(c->slot) == 0)
            break;
        if (cm_GetSlotState(c->slot) == 2) {
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        bld = (AIBLD *)cm_GetSlotValue(c->slot);
        if (bld != 0) {
            c->bld = bld->id;
            e->from = bld->id;
            e->exSlot = cm_SendExSetHEI(e->stage + 1, ev);
            return;
        }
        e->exSlot = cm_SendExSetHEI(-1, ev);
        return;
    case 4:
        e->exSlot = -1;
        gm_FindById((int *)&bld, 0, 0, c->bld);
        if (bld == 0) {
            e->stage = -1;
            return;
        }
        if (he_CreateConstruction(&c->rot, &c->pos, &bld->type, c->model, ev) == 0) {
            e->exSlot = cm_SendExSetHEI(-1, ev);
            d3if_SelectScene(scene, 1);
            return;
        }
        init_HookObject((D3OBJECT *)bld->object, bld, 2);
        bt = &ai_BTypesT[bld->type];
        if (gm_AllowAll != 0) {
            c->stages = he_ConstructionStage(ev);
            c->minutes = 10;
        } else {
            c->stages = he_ConstructionStage(ev);
            c->minutes = (unsigned)(bt->v243 * 0x3fc) / c->stages;
        }
        c->stage = 0;
        gm_AddTime(&e->end, 0, 1, 0);
        e->stage++;
        return;
    case 5:
        e->exSlot = -1;
        d3if_SelectScene(0, 1);
        gm_FindById((int *)&bld, 0, 0, c->bld);
        if (bld == 0 || lc_GetGebaeudeBauplatz(&site, bld) == 0) {
            e->exSlot = cm_SendExSetHEI(-1, ev);
            d3if_SelectScene(scene, 1);
            return;
        }
        nearest = 0;
        best = 1e35f;
        for (i = 0; i < 0x100; i++) {
            if (&ai_BldsT[i] != bld && ai_BldsT[i].type != 0 && ai_BldsT[i].object != 0) {
                house = (D3OBJECT *)ai_BldsT[i].object;
                d3_PointToWorld(house, &house->pos, &v);
                v.x -= site.pos[0];
                v.y -= site.pos[1];
                v.z -= site.pos[2];
                dist = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
                if (dist < best && d3_FindObject((int)ai_BldsT[i].object, 0x100, 0, "dummy_TUER") != 0) {
                    best = dist;
                    nearest = &ai_BldsT[i];
                }
            }
        }
        if (nearest == 0) {
            e->exSlot = cm_SendExSetHEI(-1, ev);
            d3if_SelectScene(scene, 1);
            return;
        }
        memset(&list, 0, sizeof(list));
        d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)site.object, 0x2c0, (D3OBJECTCB)ai_VgFilter, &list);
        c->placing = 0;
        for (i = 0; i < list.count; i++) {
            o = (D3OBJECT *)list.objs[i];
            door = d3_FindObject((int)nearest->object, 0x100, 0, "dummy_TUER");
            if (o != 0 && o->type != 1 && door != 0) {
                sc = evt_Load("SpecialEvents\\Hausbau.esc");
                if (sc != 0) {
                    evt_RunScriptArgs(sc, 4, door, o, he_PlaceGebaeudeObject, ev);
                    sc->group = 0;
                    sc->ownScene = 1;
                    evt_Step(sc);
                }
                c->placing++;
            }
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 10, 0);
        e->stage++;
        d3if_SelectScene(scene, 1);
        return;
    case 6:
        e->exSlot = -1;
        gm_AddTime(&e->end, 0, 0x14, 0);
        if (c->placing != 0)
            break;
        d3if_SelectScene(0, 1);
        gm_FindById((int *)&bld, 0, 0, c->bld);
        if (lc_GetGebaeudeBauplatz(&site, bld) == 0) {
            e->exSlot = cm_SendExSetHEI(-1, ev);
            d3if_SelectScene(scene, 1);
            return;
        }
        memset(&list, 0, sizeof(list));
        d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)site.object, 0x2c0, (D3OBJECTCB)ai_VgFilter, &list);
        for (i = 0; i < list.count; i++) {
            if (d3_QuadTree != 0)
                d3q_RemoveObject((D3OBJECT *)list.objs[i], (D3QNODE *)d3_QuadTree);
            d3if_Delete((D3OBJECT *)list.objs[i]);
        }
        e->clock60 = ai_Time;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 10, 0);
        e->stage++;
        c->carry = 0;
        d3if_SelectScene(scene, 1);
        return;
    case 7:
        minutes = gm_FixedWorkMinutesBetween(&e->clock60, &ai_Time) + c->carry;
        if (minutes < c->minutes) {
            gm_AddTime(&e->end, 0, 0x1e, 0);
            return;
        }
        e->clock60 = e->end;
        if (bld == 0)
            gm_FindById((int *)&bld, 0, 0, c->bld);
        if (c->script == -1 && c->stage >= 2) {
            d3if_SelectScene(0, 1);
            sprintf(u.name, "he_%i", e->id);
            door = 0;
            o = d3_FindObject(0, 0x1c0, 0, u.name);
            d3_PointToWorld(o, &o->pos, &at);
            best = 1e35f;
            for (i = 0; i < 0x100; i++) {
                if (&ai_BldsT[i] != bld && ai_BldsT[i].type != 0 && ai_BldsT[i].object != 0) {
                    house = (D3OBJECT *)ai_BldsT[i].object;
                    d3_PointToWorld(house, &house->pos, &v);
                    v.x -= at.x;
                    v.y -= at.y;
                    v.z -= at.z;
                    dist = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
                    if (dist < best && (house = d3_FindObject((int)ai_BldsT[i].object, 0x100, 0, "dummy_TUER")) != 0) {
                        best = dist;
                        door = house;
                    }
                }
            }
            if (door != 0 && o != 0) {
                sc = evt_Load("SpecialEvents\\Hausbau_2.esc");
                if (sc != 0) {
                    evt_RunScriptArgs(sc, 3, o, door, 2);
                    sc->ownScene = 1;
                    sc->group = 0;
                    c->script = sc->id;
                    evt_Step(sc);
                }
            }
            d3if_SelectScene(scene, 1);
        }
        c->stage += minutes / c->minutes;
        c->carry = minutes % c->minutes;
        gm_AddTime(&e->end, 0, c->minutes, 0);
        if (d3_CurrentScene == 0) {
            if (c->stage <= c->stages)
                he_ShowConstruction(ev);
            if (c->stage == 2 && (e->opts & 2)) {
                memset(u.msg, 0, sizeof(u.msg));
                cm_SendExSysMessage(0xc, u.msg);
            }
        }
        if (c->stage <= c->stages)
            break;
        d3if_SelectScene(0, 1);
        if (c->script != -1) {
            sc = evt_FindById(c->script);
            if (sc != 0)
                evt_Stop(sc);
            c->script = -1;
        }
        if (scene != 0) {
            d3t_InflateObjectTree((D3OBJECT *)bld->object);
            ((D3OBJECT *)bld->object)->flags |= 4;
        }
        he_ShowConstruction(ev);
        if (bld->object != 0) {
            season = gm_ClockSeason(&ai_Time);
            if (season == 1)
                init_TextureSet = 1;
            else if (season == 0)
                init_TextureSet = 0;
            else if (season == 2)
                init_TextureSet = 2;
            else if (season == 3)
                init_TextureSet = 3;
            init_TextureSetHook((int)bld->object);
        }
        if (d3_QuadTree != 0)
            d3q_Rebuild((D3OBJECT *)bld->object, (D3QNODE *)d3_QuadTree);
        if (bld->object != 0 && ((D3OBJECT *)bld->object)->type == 1)
            d3if_ShowTree((D3OBJECT *)bld->object, 1);
        if (ai_CharsT[bld->owner].kind == 6 || ai_CharsT[bld->owner].kind == 5 || ai_CharsT[bld->owner].kind == 7)
            sv_CreateBanners((char *)bld);
        ai_CheckBuildSites();
        if (((D3OBJECT *)bld->object)->info == 0)
            d3_InflateObject((D3OBJECT *)bld->object);
        if (((D3OBJECT *)bld->object)->material != 0
            && d3t_GetBoundingSphere((D3OBJECT *)bld->object, d3_Camera, &center, &radius) != 0) {
            size = (int)(radius * 1.8);
            sc = evt_Load("Effekte/geb_bauen.esc");
            if (sc != 0) {
                evt_RunScriptArgs(sc, 6, (int)center.x, (int)center.y, (int)center.z, (int)radius, size, bld->object);
                sc->group = 0;
            }
        }
        rd_BuildRoad(bld->object, bld->type);
        if (e->opts & 2) {
            cm_SendSetField(bld->id, 0x5a, 2, 0, 1);
            if (bld->type >= 4 && bld->type <= 7) {
                now = ai_Time;
                cm_BeginFieldBlock((char *)bld, bld->id);
                cm_AddFieldDelta(1, 0xe, 0x69, &now);
                cm_SendFieldDeltas();
            }
            if (ai_CharsT[bld->owner].kind == 6) {
                if (bld->type >= 4 && bld->type <= 7) {
                    txt_Format(text, 0x1c2f, bld);
                    he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, e->from, text, 1, 0x5f6, bld->id, -1, 0,
                                           "_NACHRICHTEN_HS_49");
                } else {
                    txt_Format(text, 0x1c27, bld);
                    he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, e->from, text, 0, 0x5f6, bld->id, -1, 0,
                                           "_NACHRICHTEN_HS_49");
                }
                sb_PlaySample(snd_SystemBank, "NeuesGebaeude_s", 1, 0x3f, 0x7f, 0, 0);
            }
            if (ai_CharsT[bld->owner].kind == 6 || ai_CharsT[bld->owner].kind == 5) {
                for (ch = ai_CharsT; (int)&ch->netId < (int)&ai_CharsT[0x300].netId; ch++) {
                    if ((ch->kind == 6 || ch->kind == 7) && ch->netId != ai_CharsT[bld->owner].netId) {
                        txt_Format(text, 0x1c30, bld->owner, bld->type * 14 + 0x48e);
                        he_MessageBoxQuickjump(ch->netId, -1, text, 0, 0x607, bld->id, -1, 0, "_NACHRICHTEN_HS_50");
                    }
                }
            }
            memset(u.msg, 0, sizeof(u.msg));
            cm_SendExSysMessage(0xc, u.msg);
        }
        if (bld->type >= 4 && bld->type <= 7) {
            home = ai_CharsT[bld->owner].bld2;
            if (home == 0 || home->type <= bld->type)
                ai_CharsT[bld->owner].bld2 = bld;
        }
        gm_MoveIntoHome((char *)&ai_CharsT[bld->owner], (char *)bld);
        d3if_SelectScene(scene, 1);
        e->text = 0;
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        gm_FindById((int *)&bld, 0, 0, c->bld);
        d3if_SelectScene(0, 1);
        if (c->script != -1) {
            sc = evt_FindById(c->script);
            if (sc != 0)
                evt_Stop(sc);
            c->script = -1;
        }
        c->stage = c->stages + 1;
        he_ShowConstruction(ev);
        if ((e->opts & 2) && bld != 0) {
            cm_SendBuildingDestroyed(bld->id);
            memset(u.msg, 0, sizeof(u.msg));
            cm_SendExSysMessage(0xc, u.msg);
        }
        d3if_SelectScene(scene, 1);
        he_FreeEvent((int *)ev);
        break;
    }
}
/* 0x56cdd0, the allocation handler of the mission check event 0x4c (he_RunMissionCheck): both clocks (end and
   clock60) start at the event's start clock, the end is 10 minutes later, the stage is 0 and the slot is -1 */

void he_AllocMissionCheck(AIJOB *ev)
{
    HEMISSIONCHECK *m = (HEMISSIONCHECK *)ev;

    ev->end = ev->start;
    ev->clock60 = ev->start;
    ev->stage = 0;
    m->slot = -1;
    gm_AddTime(&ev->end, 0, 0xa, 0);
}

/* 0x56ce40, the run handler of the mission check event 0x4c (alloc he_AllocMissionCheck): without a mission mode
   (ai_MissionMode negative) it frees the event; it finds the running mission (ai_Missions) of its character, none:
   due in 30 minutes; by its stage (-2 / -1 free) 0 checks the mission before 22 h (ai_IsMissionFulfilled: fulfilled,
   stage 1), 1 tells its owner with the system message 8 (before 22 h, outside a network mission: stage 2), 2 waits
   for that message's slot (1: stage 0, due in a day; else back to stage 1) */
void he_RunMissionCheck(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEMISSIONCHECK *m = (HEMISSIONCHECK *)ev;
    int msg[0x1f];
    int i, h;

    if (ai_MissionMode <= -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type != 0 && ai_Missions[i].owner == ai_CharsT[e->chr].netId)
            break;
    }
    if (i >= 0x80) {
        gm_AddTime(&e->end, 0, 0x1e, 0);
        return;
    }
    switch (e->stage) {
    case 0:
        if (ai_Missions[i].fulfilled == 0 && ai_Time.hour < 0x16) {
            if (ai_IsMissionFulfilled(&ai_Missions[i]) != 0) {
                ai_Missions[i].fulfilled = 1;
                gm_AddTime(&e->end, 0, 1, 0);
                e->stage = 1;
                return;
            }
        }
        gm_AddTime(&e->end, 0, 0xa, 0);
        break;
    case 1:
        gm_AddTime(&e->end, 0, 1, 0);
        if (ai_Missions[i].fulfilled == 1 && ai_Time.hour < 0x16 && ai_MissionPartner == -1) {
            msg[0] = ai_Missions[i].owner;
            m->slot = cm_SendExSysMessage(8, msg);
            e->stage = 2;
            return;
        }
        gm_AddTime(&e->end, 1, 0, 0);
        e->stage = 1;
        break;
    case 2:
        h = cm_GetSlotState(m->slot);
        gm_AddTime(&e->end, 0, 1, 0);
        if (h == 0)
            return;
        if (h == 1) {
            e->stage = 0;
            gm_AddTime(&e->end, 0x18, 0, 0);
            return;
        }
        e->stage = 1;
        gm_AddTime(&e->end, 0, 0x1e, 0);
        break;
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        break;
    }
}
/* 0x56d050, registers the event types of the construction / production group (called by 0x53d4b0): the alloc / run pairs of 3, 4, 5, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0x4c, 0x4d and the type texts of 3, 4, 8, 9; 0 = ok, 1 = a registration failed */

int he_RegisterEventTypes03(void)
{
    if (he_SetEventHandlers(3, (void *)he_AllocIncomeEvent, (void *)he_RunIncomeEvent) != 0)
        return 1;
    sv_RegisterActivity(3, "Abbau", 1, 0x501, 0xac);
    if (he_SetEventHandlers(4, (void *)he_AllocProduktion, (void *)he_RunProduktion) != 0)
        return 1;
    sv_RegisterActivity(4, "Produktion", 1, 0x4ff, 0xac);
    if (he_SetEventHandlers(5, (void *)he_FreeEventNow, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_SetEventHandlers(8, (void *)he_AllocSearchEvent, (void *)he_RunSearchEvent) != 0)
        return 1;
    sv_RegisterActivity(8, "Suchen", 1, 0x500, 0xac);
    if (he_SetEventHandlers(9, (void *)he_AllocSearch9, (void *)he_RunSearch9) != 0)
        return 1;
    sv_RegisterActivity(9, "Suchen", 1, 0x500, -1);
    if (he_SetEventHandlers(10, (void *)d3io_NoopCallback, (void *)he_RunNewOccurrenceMsg) != 0)
        return 1;
    if (he_SetEventHandlers(0xb, (void *)he_AllocTrade, (void *)he_RunTrade) != 0)
        return 1;
    if (he_SetEventHandlers(0xc, (void *)he_AllocResearch, (void *)he_RunResearch) != 0)
        return 1;
    if (he_SetEventHandlers(0xd, (void *)he_AllocGebaeudeBauen, (void *)he_RunConstruction) != 0)
        return 1;
    if (he_SetEventHandlers(0xe, (void *)d3io_NoopCallback, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_SetEventHandlers(0x4c, (void *)he_AllocMissionCheck, (void *)he_RunMissionCheck) != 0)
        return 1;
    if (he_SetEventHandlers(0x4d, (void *)he_FreeEventNow, (void *)d3io_NoopCallback) != 0)
        return 1;
    return 0;
}

void he_AllocRecipeResearch();
void he_RunRecipeResearch();
void he_AllocResearchRecipe();
void he_RunResearchRecipe();
void he_AllocTrainShare();
void he_RunTrainShare();
void he_AllocAlchemistDay();
void he_RunAlchemistDay();

/* 0x56d220, registers the research event types 0x95 .. 0x98 (called by 0x53d4b0) and the texts of 0x95 / 0x96; 0 = ok, 1 = a registration failed */
int he_RegisterEventTypes95(void)
{
    if (he_SetEventHandlers(0x95, (void *)he_AllocRecipeResearch, (void *)he_RunRecipeResearch) == 0) {
        if (he_SetEventHandlers(0x96, (void *)he_AllocResearchRecipe, (void *)he_RunResearchRecipe) == 0) {
            if (he_SetEventHandlers(0x97, (void *)he_AllocTrainShare, (void *)he_RunTrainShare) == 0) {
                if (he_SetEventHandlers(0x98, (void *)he_AllocAlchemistDay, (void *)he_RunAlchemistDay) == 0) {
                    sv_RegisterActivity(0x95, "erforschen", 1, 0x500, -1);
                    sv_RegisterActivity(0x96, "erforschen", 1, 0x500, -1);
                    return 0;
                }
            }
        }
    }
    return 1;
}

void snd_PlayFavourComment(void);

/* 0x56d2d0, the allocation handler of the research event 0x95 (he_RunRecipeResearch): the character (alchemist) and
   a random recipe it has no place for yet (ahm_RandomRecipe) set the recipe id and the research minutes (left /
   total), both clocks (worked and end) start at the game clock, the end is 5 minutes later, the stage is 0, and the
   character is registered in its building's group (gm_Query / sv_MoveToGroup); without a character or recipe the
   event frees itself */

void he_AllocRecipeResearch(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HERECIPESEARCH *r = (HERECIPESEARCH *)ev;
    AICHAR *c;
    HEAHMRECIPE *p;
    int v;

    c = (AICHAR *)gm_FindChar(r->alchemist);
    if (c != 0) {
        p = (HEAHMRECIPE *)ahm_RandomRecipe((char *)c);
        if (p != 0) {
            r->recipe = p->id;
            v = ahm_ResearchMinutes((char *)c, (union AHMGRID *)p);
            r->left = v;
            r->total = v;
            r->worked = ai_Time;
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 5, 0);
            e->stage = 0;
            sv_MoveToGroup((unsigned char *)c->bld, (short *)gm_Query((void *)c->bld->objList, 2, 6, 0, 0x24b), c->id,
                           (char *)ev);
            snd_PlayFavourComment();
            return;
        }
    }
    he_FreeEvent((int *)ev);
}

void ai_DomizilMessage(int param_1, int param_2, int param_3);

/* 0x56d3d0, the run handler of the research event 0x95 (alloc he_AllocRecipeResearch): without its character or the
   character's alchemist record the event ends (stage -2); stages -2 / -1 free it; outside the day hours of the season
   (gm_DayStartHours .. gm_DayEndHours) it waits an hour, the work clock restarted; stage 1 finishes: the Domizil
   message about the recipe to the event's character, the recipe added (ahm_SendAddPlace), the alchemist out of the
   laboratory group, the event freed; while the laboratory (room 0x24b of the character's building) is busy it waits
   15 minutes; stage 0 takes the minutes worked off the minutes left and goes on in 5 minutes, at none left stage 1 */
void he_RunRecipeResearch(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HERECIPESEARCH *r = (HERECIPESEARCH *)ev;
    AICHAR *c;
    AICHAR *to;
    char *o;
    int season;

    season = gm_ClockSeason(&ai_Time);
    c = (AICHAR *)gm_FindChar(r->alchemist);
    if (c == 0 || ahm_HasRecord((char *)c) == 0)
        e->stage = -2;
    if (e->stage == -2 || e->stage == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (!((float)ai_Time.hour >= gm_DayEndHours[season] || (float)ai_Time.hour < gm_DayStartHours[season])) {
        if (e->stage == 1) {
            o = gm_Query((void *)c->bld->objList, 2, 6, 0, 0x24b);
            to = (AICHAR *)gm_FindChar(e->netId);
            if (to != 0)
                ai_DomizilMessageK(to, c, o, r->recipe);
            ahm_SendAddPlace((char *)c, r->recipe);
            sv_MoveToGroup(0, 0, c->id, 0);
            he_FreeEvent((int *)ev);
            return;
        }
        if (he_RoomBusy(c->bld, 0x24b) != 0) {
            r->worked = ai_Time;
            gm_AddTime(&e->end, 0, 0xf, 0);
            return;
        }
        if (e->stage == 0) {
            r->left -= gm_MinutesBetween(&r->worked, &ai_Time);
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 5, 0);
            r->worked = ai_Time;
            if (r->left <= 0)
                e->stage = 1;
        }
        return;
    }
    r->worked = ai_Time;
    gm_AddTime(&e->end, 1, 0, 0);
}


/* 0x56d610, the alloc of the alchemist research event 0x96: the character (chr), the recipe (id) and the minutes the
   research takes (start) are looked up (gm_FindChar, ahm_FindRecipe, ahm_ResearchProgress into progress); when that
   progress is above ai_FloatZero the amount = the minutes it started with, both clocks (clock / end) = now, due 5,
   stage 0, and the character is registered in its building's group (gm_Query / sv_MoveToGroup); else the event
   frees itself */
void he_AllocResearchRecipe(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEAHMRESEARCH *r = (HEAHMRESEARCH *)ev;
    AICHAR *c;
    char *p;

    c = (AICHAR *)gm_FindChar(r->chr);
    if (c != 0) {
        p = (char *)ahm_FindRecipe(r->id);
        if (p != 0) {
            r->progress = ahm_ResearchProgress((char *)c, p, &r->start);
            if (!(r->progress <= 0.0f)) {
                r->amount = r->start;
                r->clock = ai_Time;
                e->end = ai_Time;
                gm_AddTime(&e->end, 0, 5, 0);
                e->stage = 0;
                sv_MoveToGroup((unsigned char *)c->bld, (short *)gm_Query((void *)c->bld->objList, 2, 6, 0, 0x24b), c->id,
                               (char *)ev);
                snd_PlayFavourComment();
                return;
            }
        }
    }
    he_FreeEvent((int *)ev);
}

void ai_DomizilMessageResearchKind(int param_1, int param_2, int param_3, int param_4);

/* 0x56d730, the run handler of the alchemist research event 0x96 (alloc he_AllocResearchRecipe): without its character,
   the character's alchemist record or the record's place of the recipe (ahm_SlotOf) the event ends (stage -2, freed);
   outside the day hours of the season it waits an hour, the clock restarted; while the laboratory (room 0x24b of the
   character's building) is busy it waits 15 minutes; stage 1 finishes: the Domizil message about the research to the
   event's character, the place's value raised by the progress (ahm_SendSetPlaceValue), the alchemist out of the
   laboratory group, the event freed; stage 0 takes the minutes worked off the amount and goes on in 5 minutes, at
   none left stage 1 */
void he_RunResearchRecipe(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEAHMRESEARCH *r = (HEAHMRESEARCH *)ev;
    AICHAR *c;
    AICHAR *to;
    AHMSLOT *p;
    char *o;
    int season;

    season = gm_ClockSeason(&ai_Time);
    p = 0;
    c = (AICHAR *)gm_FindChar(r->chr);
    if (c == 0 || ahm_HasRecord((char *)c) == 0 || (p = ahm_SlotOf((char *)c, r->id)) == 0)
        e->stage = -2;
    if (e->stage != -2) {
        if (!((float)ai_Time.hour >= gm_DayEndHours[season] || (float)ai_Time.hour < gm_DayStartHours[season])) {
            if (he_RoomBusy(c->bld, 0x24b) != 0) {
                r->clock = ai_Time;
                gm_AddTime(&e->end, 0, 0xf, 0);
                return;
            }
            if (e->stage == 1) {
                o = gm_Query((void *)c->bld->objList, 2, 6, 0, 0x24b);
                to = (AICHAR *)gm_FindChar(e->netId);
                if (to != 0)
                    ai_DomizilMessageResearchKind((int)to, (int)c, (int)o, r->id);
                ahm_SendSetPlaceValue((char *)c, r->id, r->progress + p->v);
                sv_MoveToGroup(0, 0, c->id, 0);
                he_FreeEvent((int *)ev);
                return;
            }
            if (e->stage == 0) {
                r->amount -= gm_MinutesBetween(&r->clock, &ai_Time);
                e->end = ai_Time;
                gm_AddTime(&e->end, 0, 5, 0);
                r->clock = ai_Time;
                if (r->amount <= 0)
                    e->stage = 1;
            }
            return;
        }
        r->clock = ai_Time;
        gm_AddTime(&e->end, 1, 0, 0);
        return;
    }
    he_FreeEvent((int *)ev);
}

void ai_DomizilMessageResearch(int param_1, int param_2, int param_3, int param_4);

/* 0x56d990, the alloc of the alchemist training event 0x97: the character (chr) and the share (id) are looked up
   (gm_FindChar, ahm_ShareProgress into progress, its minutes into start); when that progress is above ai_FloatZero
   the amount = the minutes it started with, both clocks (clock / end) = now, due 5, stage 0, and the character is
   registered in its building's group (gm_Query / sv_MoveToGroup); else the event frees itself */
void he_AllocTrainShare(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEAHMRESEARCH *r = (HEAHMRESEARCH *)ev;
    AICHAR *c;

    c = (AICHAR *)gm_FindChar(r->chr);
    if (c == 0)
        goto done;
    r->progress = ahm_ShareProgress((char *)c, r->id, &r->start);
    if (r->progress <= 0.0f)
        goto done;
    r->amount = r->start;
    r->clock = ai_Time;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 5, 0);
    e->stage = 0;
    sv_MoveToGroup((unsigned char *)c->bld, (short *)gm_Query((void *)c->bld->objList, 2, 6, 0, 0x24b), c->id,
                   (char *)ev);
    snd_PlayFavourComment();
    return;
done:
    he_FreeEvent((int *)ev);
}
/* 0x56daa0, the run handler of the alchemist training event 0x97 (alloc he_AllocTrainShare): without its character,
   the character's alchemist record or a share above ai_FloatZero (ahm_Share) the event ends (stage -2); stages -2 /
   -1 free it; outside the day hours of the season it waits an hour, the clock restarted; stage 1 finishes: the Domizil
   message about the training to the event's character, the share raised by the progress (ahm_SendSetShare), the
   alchemist out of the laboratory group, the event freed; while the laboratory (room 0x24b of the character's
   building) is busy it waits 15 minutes; stage 0 takes the minutes worked off the amount and goes on in 5 minutes, at
   none left stage 1 */
void he_RunTrainShare(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEAHMRESEARCH *r = (HEAHMRESEARCH *)ev;
    AICHAR *c;
    AICHAR *to;
    char *o;
    float share;
    int season;

    season = gm_ClockSeason(&ai_Time);
    share = 0;
    c = (AICHAR *)gm_FindChar(r->chr);
    if (c == 0 || ahm_HasRecord((char *)c) == 0 || (share = ahm_Share((char *)c, r->id)) <= 0.0f)
        e->stage = -2;
    if (e->stage == -2 || e->stage == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (!((float)ai_Time.hour >= gm_DayEndHours[season] || (float)ai_Time.hour < gm_DayStartHours[season])) {
        if (e->stage == 1) {
            o = gm_Query((void *)c->bld->objList, 2, 6, 0, 0x24b);
            to = (AICHAR *)gm_FindChar(e->netId);
            if (to != 0)
                ai_DomizilMessageResearch((int)to, (int)c, (int)o, r->id);
            ahm_SendSetShare((char *)c, r->id, share + r->progress);
            sv_MoveToGroup(0, 0, c->id, 0);
            he_FreeEvent((int *)ev);
            return;
        }
        if (he_RoomBusy(c->bld, 0x24b) != 0) {
            r->clock = ai_Time;
            gm_AddTime(&e->end, 0, 0xf, 0);
            return;
        }
        if (e->stage == 0) {
            r->amount -= gm_MinutesBetween(&r->clock, &ai_Time);
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 5, 0);
            r->clock = ai_Time;
            if (r->amount <= 0)
                e->stage = 1;
        }
        return;
    }
    r->clock = ai_Time;
    gm_AddTime(&e->end, 1, 0, 0);
}
/* 0x56dd20, the alloc of the research event 0x98 (not while paused, flags +0x78 & 4): the clock +0x52 = now, the hour set to a random 9 .. 16 (or 13 on the next day from 20 on), +0xac = 0, +0xb0 / +0xb4 = -1, and the handler scheduled (0x4c4c60) into +0x84 */

void he_AllocAlchemistDay(AIJOB *e)
{
    HEAHMDAY *d = (HEAHMDAY *)e;
    int h;

    if (e->opts & 4)
        return;
    e->end = ai_Time;
    if (e->end.hour > 0x13) {
        e->end.hour = 0xd;
        e->end.year += 1;
    } else if (e->end.hour < 9) {
        h = util_Random(8);
        e->end.hour = h + 9;
    }
    d->bonus = 0;
    d->payloadB0 = -1;
    d->payloadB4 = -1;
    e->exSlot = cm_SendExSetHEI(0, (char *)e);
}
/* 0x56ddc0, true when the room `kind` of the building b (0x24b the alchemist's laboratory, 0x256 the cannon
   room, 0x266 the cabinet) is locked (a task runs in it): its object (0x4ffe90 (b->objList, 2, 6, 0, kind))
   has a nonzero locked; the calls outside those three kinds answer 0 */

int he_RoomBusy(AIBLD *b, unsigned short kind)
{
    AIOBJ *o;

    if (kind == 0x24b || kind == 0x256 || kind == 0x266) {
        o = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, kind);
        if (o != 0 && o->locked > 0)
            return 1;
    }
    return 0;
}

/* 0x56de10, the run handler of the alchemist's daily laboratory event 0x98 (alloc he_AllocAlchemistDay; the character
   AIJOB.netId): stages -1 / -2 free it, a paused event (opts & 4) waits; without the character or its alchemist record
   the event is ended through cm_SendExSetHE (-1); it is due again next year at a random hour of the next season's
   day hours and a random quarter hour; a pending bonus adds 0.25 .. 0.5 to the record's value; else a roll of
   0.25 .. 0.5 above the record's value runs the laboratory scene he_LabIncident with the research state
   (ahm_ResearchState), otherwise the value changes by the state: 1 -0.1 .. -0.2, 2 -0.1 .. -0.25, 3 / 4 -0.1 .. -0.3,
   none +0.05 .. +0.2 (ahm_SendAddValue); then the event goes on (cm_SendExSetHE 0) */
void he_RunAlchemistDay(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEAHMDAY *d = (HEAHMDAY *)ev;
    AICHAR *c;
    AHMREC *rec;
    float done;
    int id;
    float delta;
    int season, state;

    season = (gm_ClockSeason(&ai_Time) + 1) % 4;
    c = (AICHAR *)gm_FindChar(e->netId);
    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->opts & 4)
        return;
    if (c == 0 || ahm_HasRecord((char *)c) == 0 || e->stage == -2 || e->stage == -1) {
        cm_SendExSetHEI(-1, ev);
        return;
    }
    rec = c->bld->ahm;
    e->end = ai_Time;
    e->end.year++;
    e->end.hour = util_RandomU((int)(gm_DayEndHours[season] - gm_DayStartHours[season])) + (int)gm_DayStartHours[season];
    e->end.min = util_RandomU(4) * 15;
    if (d->bonus != 0) {
        ahm_SendAddValue((char *)c, (util_RandomF() + 1.0f) * 0.25f);
        d->bonus = 0;
        cm_SendExSetHEI(0, ev);
        return;
    }
    state = ahm_ResearchState(c->bld, &id, &done, 1);
    if ((util_RandomF() + 1.0f) * 0.25f > rec->value) {
        he_LabIncident(c, ev, c->bld, state);
        cm_SendExSetHEI(0, ev);
        return;
    }
    switch (state) {
    case 1:
        delta = -((util_RandomF() + 1.0f) * 0.1f);
        break;
    case 2:
        delta = -(util_RandomF() * 0.15f + 0.1f);
        break;
    case 3:
    case 4:
        delta = -(util_RandomF() * 0.2f + 0.1f);
        break;
    default:
        delta = util_RandomF() * 0.15f + 0.05f;
        break;
    }
    ahm_SendAddValue((char *)c, delta);
    cm_SendExSetHEI(0, ev);
}

void ai_DomizilMessageTask(int param_1, int param_2, int param_3, unsigned short *param_4);
void ai_DomizilMessage04(int param_1, int param_2, int param_3);
void ai_DomizilMessageAge(int param_1, int param_2, int param_3);
void ai_DomizilMessagePlace(int param_1, int param_2, int param_3, int param_4);

/* 0x56e070, the laboratory incident of the alchemist's daily event 0x98 (he_RunAlchemistDay, only for a building
   type of kind 0x1e): of four mishaps it picks the first possible one from a random start (1 .. 4, in turn):
   1 a running task object of the building (0x24b / 0x256 / 0x266 with its state 0) is advanced (state +1,
   +0x20 by 3 .. 6), 2 the building's level drops by 4 .. 6 / 6 .. 8 / 8 .. 10 by the type's level 3 / 2 / other
   (only above level 10), 3 the alchemist ages by 1 .. 3 years (he_SetLifeByAge; always possible), 4 a place of
   the alchemist record above 0.2 loses 0.05 .. 0.15 (not in research state 2); the owner gets the message of the
   mishap; the mishap (0 none) goes to the event's +0xac (HEAHMDAY.bonus) */
void he_LabIncident(AICHAR *c, int ev, AIBLD *b, int state)
{
    HEAHMDAY *d = (HEAHMDAY *)ev;
    AIBTYPE *bt;
    AHMREC *rec;
    AIOBJ *task[3];
    int mishap = 0;
    int i, n, k, pick;
    char drop;
    float v;

    bt = &ai_BTypesT[b->type];
    if (bt->kind == 0x1e) {
        int possible[5];

        for (i = 0; i < 5; i++)
            possible[i] = 0;
        possible[3] = 1;
        if (b->level > 10)
            possible[2] = 1;
        rec = b->ahm;
        if (state != 2) {
            for (i = 0; i < 6; i++) {
                if (rec->slots[i].id != 0 && rec->slots[i].v > 0.2f) {
                    possible[4] = 1;
                    break;
                }
            }
        }
        task[0] = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x24b);
        if (task[0] != 0 && task[0]->lab.broken == 0)
            possible[1] = 1;
        task[1] = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x256);
        if (task[1] != 0 && task[1]->lab.broken == 0)
            possible[1] = 1;
        task[2] = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x266);
        if (task[2] != 0 && task[2]->lab.broken == 0)
            possible[1] = 1;
        pick = util_RandomU(4) + 1;
        for (n = 5; n != 0; n--) {
            if (pick != 0 && possible[pick] != 0) {
                mishap = pick;
                break;
            }
            pick = (pick + 1) % 5;
        }
        switch (mishap) {
        case 1:
            k = util_RandomU(3);
            for (n = 3; n != 0; n--) {
                if (task[k] != 0 && task[k]->lab.broken == 0) {
                    int one = 1;
                    int step = util_RandomU(4) + 3;

                    cm_BeginFieldBlock((char *)task[k], task[k]->objId);
                    cm_AddFieldDelta(4, 1, 0x1c, &one);
                    cm_AddFieldDelta(4, 1, 0x20, &step);
                    cm_SendFieldDeltas();
                    ai_DomizilMessageTask((int)&ai_CharsT[b->owner], (int)c, (int)b, (unsigned short *)task[k]);
                    break;
                }
                k = (k + 1) % 3;
            }
            break;
        case 2:
            if (bt->level == 3)
                drop = -4 - util_Random(3);
            else if (bt->level == 2)
                drop = -6 - util_Random(3);
            else
                drop = -8 - util_Random(3);
            cm_BeginFieldBlock((char *)b, b->id);
            cm_AddFieldValues(1, 1, 0x5c, &drop);
            cm_SendFieldDeltas();
            ai_DomizilMessage04((int)&ai_CharsT[b->owner], (int)c, (int)b);
            break;
        case 3:
            he_SetLifeByAge(c, util_Random(3) + 1);
            ai_DomizilMessageAge((int)&ai_CharsT[b->owner], (int)c, (int)b);
            break;
        case 4:
            k = util_RandomU(6);
            for (n = 6; n != 0; n--) {
                if (rec->slots[k].id != 0 && rec->slots[k].v > 0.2f) {
                    v = rec->slots[k].v - (util_RandomF() * 0.1f + 0.05f);
                    ahm_SendSetPlaceValue((char *)c, rec->slots[k].id, v);
                    ai_DomizilMessagePlace((int)&ai_CharsT[b->owner], (int)c, (int)b, rec->slots[k].id);
                    break;
                }
                k = (k + 1) % 6;
            }
            break;
        default:
            return;
        }
        d->bonus = mishap;
    }
}

void he_AllocSecretRoom();
void he_RunSecretRoom();
void he_AllocEvent9a();
void he_RunDomizilEvent();
void he_AllocDomizilEvent();

/* 0x56e400, registers the event types 0x99 .. 0x9b (called by 0x53d4b0): the pairs (0x56e460, 0x56e5b0), (0x56efc0, 0x56f040) and (0x56f050, 0x56f0b0); 0 = ok, 1 = a registration failed */
int he_RegisterEventTypes99(void)
{
    if (he_SetEventHandlers(0x99, (void *)he_AllocSecretRoom, (void *)he_RunSecretRoom) == 0) {
        if (he_SetEventHandlers(0x9a, (void *)he_AllocEvent9a, (void *)he_FreeEventNow) == 0) {
            if (he_SetEventHandlers(0x9b, (void *)he_AllocDomizilEvent, (void *)he_RunDomizilEvent) == 0)
                return 0;
        }
    }
    return 1;
}


/* 0x56e460, the alloc of the event 0x99 (not while paused, flags +0x78 & 4): when another event of the type 0x99 of the same player (+8) already runs, this one only announces the text 0x3ff7 (0x5176d0 / 0x53dda0), schedules with the handle -1 (0x4c4c60 ( -1, ev) into +0x84) and frees; else the start clock +0x44 and the due clock +0x52 = now, due 1, +0xcc = the notification 0x4c59f0 (0xd, ev's +0xb0, -1, 0x2d, 0, 0, the text 0x628dc8) and the handler scheduled (0x4c4c60 (0, ev)) into +0x84 */
void he_AllocSecretRoom(AIJOB *e)
{
    char text[0x400];
    AIJOB *r;

    if (e->opts & 4)
        return;
    for (r = (AIJOB *)he_FindEvent(1, 0, 0x99); r != 0; r = (AIJOB *)he_NextEvent()) {
        if (r != e && r->chr == e->chr)
            goto taken;
    }
    e->start = ai_Time;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 1, 0);
    ((HESECRETROOM *)e)->walkSlot = ((int (*)(int, int, int, int, int, int, const char *))cm_SendSpawnCharacter)(0xd, ((HESECRETROOM *)e)->home, -1, 0x2d, 0, 0, "ASSASSIN");
    e->exSlot = cm_SendExSetHEI(0, (char *)e);
    return;
taken:
    txt_Format(text, 0x3ff7, e->chr);
    he_ShowNews(ai_CharInts[e->chr][0], -1, text, 0, 0x5fa, 0);
    e->exSlot = cm_SendExSetHEI(-1, (char *)e);
    he_FreeEvent((int *)e);
}

int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out);   /* 0x4a63f0 (am.h); the 7-dword callers go through AI_4A63F0_7 */
void he_FreeEventNow(int *r);          /* 0x56f040 */

/* 0x56e5b0, the run handler of the secret room event 0x99 (alloc he_AllocSecretRoom): the stages -1 / -2 remove the
   agent (opts & 2) and, at -1, tell the event's character (text 0x3ff9, the news "_NACHRICHTEN_DOMIZIL_08") and free
   the event; a paused event or one whose cm slot is still busy waits. Stage 0 takes the spawned agent from its cm
   slot: the spy hunt (way 3) goes on with stage 6, else the agent walks to the target building (stage 1). Stage 1
   waits 4 minutes while the agent walks, then an hour at the target (stage 3); stage 3 does the deed by the way
   (1 he_SecretRoomAttack, 2 he_SecretRoomAccuse, 3 the spy hunt again, 4 he_SecretRoomDomizil) with its news, and
   stage 4 sends the agent home (stage 5); stage 5 waits until it is there, removes it and ends the event after the
   rest of the way's time (way 2 0x21c minutes, way 4 0xb40, else 0x5a0; at least 5). The spy hunt: stage 6 follows
   the nearest spy (he_FindNearestSpying: stage 7) or walks to a picked building (stage 6 again in 15 minutes),
   stage 7 tries to catch it (he_SecretRoomCatchSpy); after 0x5a0 minutes it goes home (stage 4). A lost agent ends
   the event. */
void he_RunSecretRoom(int ev)
{
    char text[0x100];
    AIJOB *e = (AIJOB *)ev;
    HESECRETROOM *s = (HESECRETROOM *)ev;
    AICHAR *per, *agent, *victim;
    AIBLD *bld;
    AIJOB *spy;
    int minutes, rest, caught;
    union { struct { unsigned int lo, hi; } s; __int64 q; } kinds;

    if (e->stage == -1 || e->stage == -2) {
        if (e->opts & 2) {
            if (gm_FindChar(s->agent) != 0)
                cm_SendKillCharacter(s->agent, 1);
            if (e->stage == -1)
                he_ShowNews(ai_CharsT[e->chr].netId, -1, txt_Ptrs[0x3ff9], 1, 0x5fa, "_NACHRICHTEN_DOMIZIL_08");
        }
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->opts & 4)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    per = &ai_CharsT[e->chr];
    switch (e->stage) {
    case 0:
        if (cm_GetSlotState(s->walkSlot) == 1 && (agent = (AICHAR *)cm_GetSlotValue(s->walkSlot)) != 0) {
            s->agent = agent->netId;
            if (s->way == 3) {
                e->end = ai_Time;
                e->exSlot = cm_SendExSetHEI(6, ev);
                return;
            }
            if ((bld = (AIBLD *)gm_BuildQuery(1, 1, s->target)) == 0) {
                e->end = ai_Time;
                e->exSlot = cm_SendExSetHEI(-1, ev);
                return;
            }
            s->walkSlot = cm_SendChrGotoBuilding(agent->netId, bld->id, -1, 0, 1, "ASSASSIN");
            cm_SendExSetHEI(1, ev);
            return;
        }
        e->exSlot = cm_SendExSetHEI(-1, ev);
        return;
    case 1:
        s->walkSlot = -1;
        agent = (AICHAR *)gm_FindChar(s->agent);
        if (agent == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        if (agent->scene->actions != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 4, 0);
            return;
        }
        if (gm_BuildQuery(1, 1, s->target) == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 1, 0, 0);
        e->exSlot = cm_SendExSetHEI(3, ev);
        return;
    case 3:
        agent = (AICHAR *)gm_FindChar(s->agent);
        if (agent == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        victim = (AICHAR *)gm_FindChar(s->victim);
        bld = (AIBLD *)gm_BuildQuery(1, 1, s->home);
        if (per != 0 && victim != 0 && bld != 0) {
            switch (s->way) {
            case 1:
                if (he_SecretRoomAttack(per, victim, s->strength) != 0) {
                    txt_Format(text, 0x4004, victim->id);
                    he_ShowNews(per->netId, -1, text, 1, 0x5fa, "_NACHRICHTEN_DOMIZIL_06");
                    txt_Format(text, 0x4006, s->strength + 0x4007);
                    he_ShowNews(victim->netId, -1, text, 1, 0x5fb, "_NACHRICHTEN_DOMIZIL_09");
                } else {
                    txt_Format(text, 0x4005, victim->id);
                    he_ShowNews(per->netId, -1, text, 1, 0x5fa, "_NACHRICHTEN_DOMIZIL_07");
                    txt_Format(text, 0x400a);
                    he_ShowNews(victim->netId, -1, text, 1, 0x5fb, "_NACHRICHTEN_DOMIZIL_09");
                }
                break;
            case 2:
                if (he_SecretRoomAccuse(per, victim, s->strength) != 0) {
                    txt_Format(text, 0x4019, 0x400c, victim->id, victim->id);
                    he_ShowNews(per->netId, -1, text, 1, 0x5fa, 0);
                } else {
                    txt_Format(text, 0x401a, 0x400c, victim->id, victim->id);
                    he_ShowNews(per->netId, -1, text, 1, 0x5fa, 0);
                }
                break;
            case 3:
                e->end = ai_Time;
                s->spyEvent = -1;
                s->hunting = 0;
                e->exSlot = cm_SendExSetHEI(6, ev);
                break;
            case 4:
                he_SecretRoomDomizil(bld->id, victim);
                break;
            }
        }
        e->exSlot = cm_SendExSetHEI(4, ev);
        break;
    case 4:
        agent = (AICHAR *)gm_FindChar(s->agent);
        e->end = ai_Time;
        if (agent == 0) {
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        s->walkSlot = cm_SendChrGotoBuilding(s->agent, s->home, -1, 0, 0, "ASSASSIN");
        e->exSlot = cm_SendExSetHEI(5, ev);
        return;
    case 5:
        agent = (AICHAR *)gm_FindChar(s->agent);
        if (agent == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        if (agent->scene->actions != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 4, 0);
            return;
        }
        minutes = gm_MinutesBetween(&e->start, &ai_Time);
        switch (s->way) {
        case 1:
            rest = 0x5a0;
            break;
        case 2:
            rest = 0x21c;
            break;
        case 3:
            rest = 0x5a0;
            break;
        case 4:
            rest = 0xb40;
            break;
        default:
            rest = 0x5a0;
            break;
        }
        rest -= minutes;
        minutes = rest > 5 ? rest : 5;
        e->end = ai_Time;
        gm_AddTime(&e->end, minutes / 60, minutes % 60, 0);
        cm_SendKillCharacter(s->agent, 1);
        e->exSlot = cm_SendExSetHEI(-1, ev);
        return;
    case 6:
        if (gm_MinutesBetween(&e->start, &ai_Time) > 0x5a0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(4, ev);
            return;
        }
        agent = (AICHAR *)gm_FindChar(s->agent);
        if (agent == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        if ((spy = (AIJOB *)he_FindNearestSpying(per, agent, s->spyEvent)) != 0) {
            s->target = ((HESPYING *)spy)->at;
            s->spyEvent = spy->id;
            s->hunting = 1;
            cm_SendChrStop(agent->netId);
            cm_SendChrGotoBuilding(agent->netId, s->target, -1, 0, 1, "Spy Killer");
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 5, 0);
            e->exSlot = cm_SendExSetHEI(7, ev);
        } else {
            kinds.s.lo = 0xa7f4c7a0;
            kinds.s.hi = 1;
            ai_PickActionBuilding((struct AIPER *)per, 0, kinds.q, 0.0f, 100.0f, &s->target);
            s->spyEvent = -1;
            s->hunting = 0;
            cm_SendChrStop(agent->netId);
            cm_SendChrGotoBuilding(agent->netId, s->target, -1, 0, 1, "Spy Killer");
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 0xf, 0);
            e->exSlot = cm_SendExSetHEI(6, ev);
        }
        return;
    case 7:
        minutes = gm_MinutesBetween(&e->start, &ai_Time);
        agent = (AICHAR *)gm_FindChar(s->agent);
        if (agent == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        if (minutes > 0x5a0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(4, ev);
            return;
        }
        caught = he_SecretRoomCatchSpy(agent, ev, (AIJOB *)he_FindEvent(2, 0, 0x18, 1, s->spyEvent));
        if (caught == 0) {
            gm_AddTime(&e->end, 0, 6, 0);
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(7, ev);
            return;
        }
        if (caught == 1) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(6, ev);
            return;
        }
        e->end = ai_Time;
        e->exSlot = cm_SendExSetHEI(6, ev);
        return;
    }
}
/* 0x56efc0, the alloc of the event 0x9a (the run 0x56f040 only frees it): the due clock +0x52 = now; when
   0x56f360 (no event 0x9a holds the building +0xac and the character of +0xb0 yet) answers 1, due in 0x18
   hours and the stage 0 posted; the stage -1 is posted in every case and its send slot kept at +0x84 */

void he_AllocEvent9a(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->end = ai_Time;
    if (he_NoEvent9aFor(e->bldId, (AICHAR *)gm_FindChar(e->id2)) != 0) {
        gm_AddTime(&e->end, 0x18, 0, 0);
        cm_SendExSetHEI(0, ev);
    }
    e->exSlot = cm_SendExSetHEI(-1, ev);
}
/* 0x56f040, frees the event record it is handed (he_FreeEvent): the alloc / run handler of the event types the
   game drops at once (5, 0x2d, 0x30, 0x4d, 0x7b) and the run handler of 0x29 / 0x9a */

void he_FreeEventNow(int *r)
{
    he_FreeEvent(r);
}
/* 0x56f050, the alloc of the event 0x9b (not while paused, flags +0x78 & 4): due = now +0x30 hours (0x4fe140), state 0 scheduled (0x4c4c60) */

void he_AllocDomizilEvent(AIJOB *e)
{
    if (e->opts & 4)
        return;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0x30, 0, 0);
    e->exSlot = cm_SendExSetHEI(0, (char *)e);
}
/* 0x56f0b0, the run of the domicile event 0x9b: any stage but 0 frees; stage 0 sends the text 0x4035 with the news
   0x5fb (its second text "_NACHRICHTEN_DOMIZIL_11") to the character partner1 when it exists, and the text 0x4036 with the
   news 0x5fa to the character partner2 when it exists (the first character's id as its argument), then frees */
void he_RunDomizilEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    char text[0x100];
    AICHAR *c, *c2;

    if (e->stage == 0) {
        c = (AICHAR *)gm_FindChar(e->partner1);
        c2 = (AICHAR *)gm_FindChar(e->partner2);
        if (c != 0) {
            txt_Format(text, 0x4035);
            he_ShowNews(e->partner1, -1, text, 1, 0x5fb, "_NACHRICHTEN_DOMIZIL_11");
        }
        if (c2 != 0) {
            txt_Format(text, 0x4036, c->id);
            he_ShowNews(e->partner2, -1, text, 1, 0x5fa, 0);
        }
    }
    he_FreeEvent((int *)ev);
}
/* 0x56f170, 1 when a running event of the type 0x99 has the owner +0xb0 == `owner` (the search 0x53e400 / 0x53e300) */

int he_EventOnBuilding(int owner)
{
    AIJOB *r;

    for (r = (AIJOB *)he_FindEvent(1, 0, 0x99); r != 0; r = (AIJOB *)he_NextEvent()) {
        if (((HESECRETROOM *)r)->home == owner)
            return 1;
    }
    return 0;
}
/* 0x56f1b0, a meeting message when the slot he_FindMeetingSlot (c + 3) is free and (0x504ec0 (a, 4) - 0x504ec0 (b, 4) * 0.5f) + 0x42ec20 () >= 1.0: 0x4f2e30 (slot, 1, b + 4, a + 4, the id +1 of the record at b + 0x170 / +0x16c, else -1); 0 when not sent */

int he_SecretRoomAccuse(AICHAR *a, AICHAR *b, int c)
{
    float fa, fb;
    int slot = he_FindMeetingSlot(c);
    int id;

    if (slot >= 0) {
        fa = gm_GetSkill((char *)a, 4);
        fb = gm_GetSkill((char *)b, 4);
        if (fa - fb * 0.5f + util_RandomF() >= 1.0f) {
            if (b->bld2 != 0)
                id = b->bld2->id;
            else if (b->bld != 0)
                id = b->bld->id;
            else
                id = -1;
            gv_Accuse(slot, 1, b->netId, a->netId, id);
            return 1;
        }
    }
    return 0;
}
/* 0x56f260, the first free meeting slot of 0x1d (two rounds from a random start): the record 0x4f2720 (slot) has +0x18 == 1, +1 == 2 and +0xc == the wanted id `c` + 3 (one more each round); -1 when none */

int he_FindMeetingSlot(int c)
{
    AIDEC dec;
    int want = c + 3;
    int round;
    int i;
    int slot;

    round = 2;
    do {
        slot = (unsigned short)util_Random(0x1d);
        for (i = 0x1d; i != 0; i--) {
            gv_GetTableRecord(slot, (int *)&dec);
            if (dec.cur == 1 && dec.valueText == 2 && dec.guilt == want)
                return slot;
            slot = (slot + 1) % 0x1d;
        }
        want++;
    } while (--round != 0);
    return -1;
}
/* 0x56f2e0, a message when the person a and the record b (its word at 0 is the target) can meet: b not 0 and (0x504ec0 (a, 2) - 0x504ec0 (b, 2) * 0.5f) + 0x42ec20 () >= 1.0; then 0x4fca40 (*b, (-1 - c) * 5) */

int he_SecretRoomAttack(AICHAR *a, AICHAR *b, int c)
{
    float fa, fb;

    if (b != 0) {
        fa = gm_GetSkill((char *)a, 2);
        fb = gm_GetSkill((char *)b, 2);
        if (fa - fb * 0.5f + util_RandomF() >= 1.0f) {
            ((float (*)(unsigned short, int))gm_AddHealth)(b->id, (-1 - c) * 5);
            return 1;
        }
    }
    return 0;
}
/* 0x56f360, 1 when no running event of the type 0x9a has both the building bldId and the netId of the character c; 0 when one does */

int he_NoEvent9aFor(int bldId, AICHAR *c)
{
    AIJOB *r;

    for (r = (AIJOB *)he_FindEvent(1, 0, 0x9a); r != 0; r = (AIJOB *)he_NextEvent()) {
        if (r->bldId == bldId && r->id2 == c->netId)
            return 0;
    }
    return 1;
}
/* 0x56f3b0, 1 when no running domicile event 0x9b runs against the character a (its partner1 is a's netId); 0 when
   one does */

int he_NoEvent9bFor(AICHAR *a)
{
    AIJOB *r;

    for (r = (AIJOB *)he_FindEvent(1, 0, 0x9b); r != 0; r = (AIJOB *)he_NextEvent()) {
        if (r->partner1 == (int)a->netId)
            return 0;
    }
    return 1;
}
/* 0x56f3f0, the domicile deed of the secret room event (way 4, he_RunSecretRoom): when no domicile event 0x9b runs
   against the victim yet (he_NoEvent9bFor) and the secret room's building bldId exists, its owner tries against the
   victim: skill 2 (owner - victim * 0.5 + random * 0.5 at least 0.8) and then skill 3 (at least 0.7); success tells
   both (texts 0x4031 / 0x4033) and posts the domicile event 0x9b (from the building, the victim and the owner) and
   returns 1; a failure tells both (texts 0x4032 / 0x4034); 0 when nothing was posted */
int he_SecretRoomDomizil(int bldId, AICHAR *victim)
{
    char text[0x400];
    AIMSG msg;
    AIBLD *bld;
    AICHAR *owner;
    float mine, theirs;

    if (he_NoEvent9bFor(victim) != 0) {
        bld = (AIBLD *)gm_BuildQuery(1, 1, bldId);
        if (bld != 0) {
            owner = &ai_CharsT[bld->owner];
            mine = gm_GetSkill((char *)owner, 2);
            theirs = gm_GetSkill((char *)victim, 2);
            if (mine - theirs * 0.5f + util_RandomF() * 0.5 < 0.8) {
                txt_Format(text, 0x4032, victim->id, victim->id);
                he_ShowNews(owner->netId, -1, text, 1, 0x5fa, "_NACHRICHTEN_DOMIZIL_07");
                txt_Format(text, 0x4034);
                he_ShowNews(victim->netId, -1, text, 1, 0x5fb, "_NACHRICHTEN_DOMIZIL_09");
            } else {
                mine = gm_GetSkill((char *)owner, 3);
                theirs = gm_GetSkill((char *)victim, 3);
                if (mine - theirs * 0.5f + util_RandomF() * 0.5 < 0.7) {
                    txt_Format(text, 0x4032, victim->id, victim->id);
                    he_ShowNews(owner->netId, -1, text, 1, 0x5fa, "_NACHRICHTEN_DOMIZIL_07");
                    txt_Format(text, 0x4034);
                    he_ShowNews(victim->netId, -1, text, 1, 0x5fb, "_NACHRICHTEN_DOMIZIL_09");
                } else {
                    txt_Format(text, 0x4031, victim->id);
                    he_ShowNews(owner->netId, -1, text, 1, 0x5fa, "_NACHRICHTEN_DOMIZIL_06");
                    txt_Format(text, 0x4033);
                    he_ShowNews(victim->netId, -1, text, 1, 0x5fb, "_NACHRICHTEN_DOMIZIL_09");
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x9b;
                    msg.to = owner->netId;
                    msg.fromId = bldId;
                    msg.arg[0] = victim->netId;     /* the event's partner1 */
                    msg.arg[1] = owner->netId;      /* its partner2 */
                    msg.flags = 2;
                    cm_SendLongMessage((char *)&msg);
                    return 1;
                }
            }
        }
    }
    return 0;
}
/* 0x56f600, the spy hunt's search (he_RunSecretRoom stage 6): of the running spying events 24 that spy on the
   character per (HESPYING.victim) and are not the event skip (an event id), the one whose spied building
   (HESPYING.at, gm_FindById) stands nearest to the agent's body; 0 when none (or the agent has no scene character) */
AIJOB *he_FindNearestSpying(AICHAR *per, AICHAR *agent, int skip)
{
    float best = 1.0e7f;
    D3VECTOR4 here, diff;
    float d;
    AIJOB *found, *ev;
    AIBLD *bld;
    D3OBJECT *body;

    found = 0;
    if (agent->scene == 0)
        return 0;
    body = agent->scene->obj;
    /* @original: the position copies go through the FPU (fld / fstp), here and into diff before the
       subtracts; VC6 copies a parenthesised float member that way, a plain one through an integer register */
    here.x = (body->pos.x);
    here.y = (body->pos.y);
    here.z = (body->pos.z);
    he_FindEvent(1, 0, 0x18);
    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x18); ev != 0; ev = (AIJOB *)he_NextEvent()) {
        if (ev->stage == -1 || ev->stage == -2 || skip == ev->id)
            continue;
        if (((HESPYING *)ev)->victim != per->netId)
            continue;
        gm_FindById((int *)&bld, 0, 0, ((HESPYING *)ev)->at);
        if (bld == 0)
            continue;
        body = (D3OBJECT *)bld->object;
        diff.x = (body->pos.x);
        diff.y = (body->pos.y);
        diff.z = (body->pos.z);
        diff.x -= here.x;
        diff.y -= here.y;
        diff.z -= here.z;
        d = sqrt(diff.z * diff.z + diff.y * diff.y + diff.x * diff.x);
        if (found == 0) {
            best = d;
            found = ev;
        } else if (best > d) {
            best = d;
            found = ev;
        }
    }
    return found;
}
/* 0x56f740, the spy hunt's catch (he_RunSecretRoom stage 7; agent = the secret room's agent, ev = its event, spy =
   the spying event 24 it follows): -1 without a spying event, after 0x14 tries (the event's spyEvent word, counted
   up here) or without the spy's scene character; while the spy is at least 1850 from the agent, the agent walks
   on to the spied building (when it changed, "Spy Killer") and 0; near, the spy is caught when the spy's
   employer's skill 2 + (days + 1) * 0.5 is below the victim's skill 2 + (strength + 1) * 0.5: the spying event
   ends, both are told (texts 0x4028 / 0x4029, the news "_NACHRICHTEN_DOMIZIL_10") and 1; else -1. match-deep 2026-11-10: parenthesized member copies (the FPU-copy lever) ins 16 -> 8. */
int he_SecretRoomCatchSpy(AICHAR *agent, int ev, AIJOB *spy)
{
    char text[0x400];
    HESECRETROOM *s = (HESECRETROOM *)ev;
    HESPYING *sp = (HESPYING *)spy;
    AICHAR *spyChr, *employer, *victim;
    D3VECTOR4 d, there;
    float dist, spyBonus, guard, skill;

    if (spy == 0 || spy->kind != 0x18)
        return -1;
    if ((unsigned int)s->spyEvent >= 0x14)
        return -1;
    s->spyEvent++;
    spyChr = (AICHAR *)gm_FindChar(sp->spy);
    if (spyChr == 0 || spyChr->scene == 0)
        return -1;
    d.x = (agent->scene->obj->pos.x);
    d.y = (agent->scene->obj->pos.y);
    d.z = (agent->scene->obj->pos.z);
    there.x = (spyChr->scene->obj->pos.x);
    there.y = (spyChr->scene->obj->pos.y);
    there.z = (spyChr->scene->obj->pos.z);
    d.x -= there.x;
    d.y -= there.y;
    d.z -= there.z;
    dist = sqrt(d.z * d.z + d.y * d.y + d.x * d.x);
    if (dist < 1850.0f) {
        employer = &ai_CharsT[spy->chr];
        victim = (AICHAR *)gm_FindChar(sp->victim);
        spyBonus = (sp->days + 1) * 0.5f;
        guard = (s->strength + 1) * 0.5f;
        skill = gm_GetSkill((char *)employer, 2);
        if (gm_GetSkill((char *)victim, 2) + guard > skill + spyBonus) {
            spy->exSlot = cm_SendExSetHEI(-1, spy);
            txt_Format(text, 0x4028, victim->id);
            he_ShowNews(employer->netId, -1, text, 1, 0x5fb, "_NACHRICHTEN_DOMIZIL_10");
            txt_Format(text, 0x4029, employer->id);
            he_ShowNews(victim->netId, -1, text, 1, 0x5fb, "_NACHRICHTEN_DOMIZIL_10");
            return 1;
        }
        return -1;
    }
    if (sp->at != s->target) {
        s->target = sp->at;
        cm_SendChrStop(agent->netId);
        cm_SendChrGotoBuilding(agent->netId, sp->at, -1, 0, 1, "Spy Killer");
    }
    return 0;
}

void he_AllocHaunting(int ev);          /* 0x56fc50 */
void he_RunHaunting(int ev);          /* 0x56fda0 */
void he_AllocDecay(int ev);          /* 0x570130 */
void he_RunDecay(int ev);          /* 0x5702e0 */
void he_AllocMasterWork(int ev);          /* 0x570ef0 */
void he_RunMasterWork(int ev);          /* 0x571000 */
void he_AllocGraveCare(int ev);          /* 0x571550 */
void he_RunGraveCare(int ev);          /* 0x571640 */
void he_AllocPlunder(int ev);          /* 0x5706e0 */
void he_RunPlunder(int ev);          /* 0x5707a0 */
void he_AllocPlunderOfferLease(int ev);          /* 0x572860 */
void he_RunPlunderOffer(int ev);          /* 0x570cc0 */
void he_AllocDeathDue(int ev);          /* 0x571990 */
void he_RunDeathDue(int ev);          /* 0x5719e0 */
void he_AllocGraveTraining(int ev);          /* 0x571ac0 */
void he_RunGraveTraining(int ev);          /* 0x571c30 */
void he_AllocMeeting(int ev);          /* 0x571f60 */
void he_RunMeeting(int ev);          /* 0x5725b0 */
void he_AllocMeetingScene(int ev);          /* 0x5726a0 */
void he_RunMeetingScene(int ev);          /* 0x5726b0 */
void he_AllocGraveyardWorkers(int ev);          /* 0x572b90 */
void he_RunGraveyardWorkers(int ev);          /* 0x572bf0 */
void he_RunGraveyardTheft(int ev);          /* 0x572fd0 */
void he_RunGraveLease(int ev);          /* 0x572a50 */

/* 0x56f980, registers the event types 0x88 .. 0x94 (called by 0x53d4b0): (0x88, 0x56fc50, 0x56fda0), (0x89, 0x570130, 0x5702e0), (0x8c, 0x570ef0, 0x571000), (0x8d, 0x571550, 0x571640), (0x8a, 0x5706e0, 0x5707a0), (0x8b, 0x572860, 0x570cc0), (0x8d again), (0x8e, 0x571990, 0x5719e0), (0x8f, 0x571ac0, 0x571c30), (0x90, 0x571f60, 0x5725b0), (0x91, 0x5726a0, 0x5726b0), (0x93, 0x572b90, 0x572bf0), (0x94, 0x55e3a0, 0x572fd0), (0x92, 0x572860, 0x572a50), then the texts of 0x52b350; 0 = ok, 1 = a registration failed */
int he_RegisterEventTypes88(void)
{
    if (he_SetEventHandlers(0x88, (void *)he_AllocHaunting, (void *)he_RunHaunting) == 0) {
        if (he_SetEventHandlers(0x89, (void *)he_AllocDecay, (void *)he_RunDecay) == 0) {
            if (he_SetEventHandlers(0x8c, (void *)he_AllocMasterWork, (void *)he_RunMasterWork) == 0) {
                if (he_SetEventHandlers(0x8d, (void *)he_AllocGraveCare, (void *)he_RunGraveCare) == 0) {
                    if (he_SetEventHandlers(0x8a, (void *)he_AllocPlunder, (void *)he_RunPlunder) == 0) {
                        if (he_SetEventHandlers(0x8b, (void *)he_AllocPlunderOfferLease, (void *)he_RunPlunderOffer) == 0) {
                            if (he_SetEventHandlers(0x8d, (void *)he_AllocGraveCare, (void *)he_RunGraveCare) == 0) {
                                if (he_SetEventHandlers(0x8e, (void *)he_AllocDeathDue, (void *)he_RunDeathDue) == 0) {
                                    if (he_SetEventHandlers(0x8f, (void *)he_AllocGraveTraining, (void *)he_RunGraveTraining) == 0) {
                                        if (he_SetEventHandlers(0x90, (void *)he_AllocMeeting, (void *)he_RunMeeting) == 0) {
                                            if (he_SetEventHandlers(0x91, (void *)he_AllocMeetingScene, (void *)he_RunMeetingScene) == 0) {
                                                if (he_SetEventHandlers(0x93, (void *)he_AllocGraveyardWorkers, (void *)he_RunGraveyardWorkers) == 0) {
                                                    if (he_SetEventHandlers(0x94, (void *)he_AllocDueIsStart, (void *)he_RunGraveyardTheft) == 0) {
                                                        if (he_SetEventHandlers(0x92, (void *)he_AllocPlunderOfferLease, (void *)he_RunGraveLease) == 0) {
                                                            sv_RegisterActivity(0x8d, "grabpflege", 1, 0x513, -1);
                                                            sv_RegisterActivity(0x8c, "Rohstoffsuchen", 1, 0x500, 0xac);
                                                            sv_RegisterActivity(0x8f, "Upgraden", 0, 0x512, -1);
                                                            sv_RegisterActivity(0x8a, 0, 0, 0x514, -1);
                                                            sv_RegisterActivity(0x88, 0, 0, 0x511, -1);
                                                            return 0;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}
/* 0x56fbc0, ages the character c's life value f01c by num years: fnum not above f020 -> sends the delta
   f018 - f01c; else (fnum - f020) steps of v = v - f014 + f014 / v from v = fnum - f020, then the delta
   v - f01c (cm_SendFloatDelta, field 0x1c)
   @deep size queue: padded size right (129 = original raw) with the parenthesised (c->f020) in the compare
   (rank lever: fld f020; fld st(1); fcompp, 10 ins), not the original's form. Left: the original keeps
   fnum in memory (num's dead slot: fst [esp+0xc]; fcomp f020; reloaded by fld for v = fnum - f020); ours
   holds it on the x87 stack (c2fpu: fnum prio 1 / sav 1 -> stack; the original needs savings <= 0). s3
   tried: the conversion as a CSE without fnum, (ushort)num compared directly, v as the fnum carrier
   (draft), if/else and if (c) {..} forms, > with swapped arms, c->f020 >= fnum, !(fnum > f020), double
   compare, f = c->f014 before the test, fnum initialised at its declaration, v assigned inside the ftol,
   while (n-- > 0): none puts fnum in memory. */

void he_SetLifeByAge(AICHAR *c, int num)
{
    float f, v, fnum;
    int n;

    if (c == 0)
        return;
    fnum = (float)(unsigned short)num;
    if (fnum <= (c->f020)) {
        ((int (*)(int, int, float))cm_SendFloatDelta)(c->netId, 0x1c, c->f018 - c->f01c);
        return;
    }
    f = c->f014;
    v = fnum - c->f020;
    for (n = (int)v; n > 0; n--)
        v = v - f + f / v;
    ((int (*)(int, int, float))cm_SendFloatDelta)(c->netId, 0x1c, v - c->f01c);
}
/* 0x56fc50, the alloc of the event 0x88 (a work at the building +0xb0 of the player +8): paused -> nothing; with a running event 0x88 of the same player and building the text 0x4096 and state -1; else the due clock now + 1 hour and state 0 (0x4c4c60), or -1 when +0xb4 is -1, and the message 0x50cc50 (pid, -(+0xb8)) */

void he_AllocHaunting(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEHAUNTING *h = (HEHAUNTING *)ev;
    AIJOB *r;
    char text[0x400];

    if (e->opts & 4)
        return;
    for (r = (AIJOB *)he_FindEvent(1, 0, 0x88); r != 0; r = (AIJOB *)he_NextEvent()) {
        if (r != e && r->chr == e->chr && ((HEHAUNTING *)r)->victim == h->victim)
            break;
    }
    if (r != 0) {
        e->exSlot = cm_SendExSetHEI(-1, ev);
        txt_Format(text, 0x4096, h->victim);
        he_ShowNews(ai_CharInts[e->chr][0], -1, text, 0, 0x5fa, 0);
        return;
    }
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 1, 0);
    if (h->haunter == -1)
        e->exSlot = cm_SendExSetHEI(-1, ev);
    else {
        e->exSlot = cm_SendExSetHEI(0, ev);
        gm_SendMoneyDelta(ai_CharInts[e->chr][0], -h->paid);
    }
}

void ai_HauntingText(unsigned short *param_1, int param_2, unsigned int param_3);                 /* 0x5a9910 */

/* 0x56fda0, the run handler of the graveyard owner's haunting event 0x88 (alloc he_AllocHaunting; am.c's
   ai_ExecGraveyardOwner0 / sv_t.c post it): without the haunter the event is freed; the stages -1 / -2 take the
   haunter out of the event's group (sv_MoveToGroup) and free it; a paused event or one whose cm slot is still busy
   waits. Stage 0 puts the haunter in the group and sends it to the victim's building ("HEIMSUCHUNG", stage 2), or
   ends the event; stage 2 waits 4 minutes while it walks, then a day at the building, telling the event's
   character and the victim (ai_HauntingText), stage 3; stage 3 sends it back to its own building (stage 4), stage
   4 waits until it is there, takes it out of the group and keeps the event (stage 5) for max (paid / 2, 1) + 1
   more years; stage 5 ends it. A haunter without a building ends the event. */
void he_RunHaunting(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEHAUNTING *h = (HEHAUNTING *)ev;
    AICHAR *haunter, *victim;
    AIBLD *bld;
    int years;

    haunter = (AICHAR *)gm_FindChar(h->haunter);
    if (haunter == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage == -1 || e->stage == -2) {
        sv_MoveToGroup(0, 0, haunter->id, (char *)ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->opts & 4)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    switch (e->stage) {
    case 0:
        if (cm_GetSlotState(e->exSlot) == 0)
            return;
        sv_MoveToGroup(0, 0, haunter->id, (char *)ev);
        bld = (AIBLD *)gm_BuildQuery(1, 1, h->home);
        if (bld == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        h->walkSlot = cm_SendChrGotoBuilding(haunter->netId, bld->id, -1, 0, 1, "HEIMSUCHUNG");
        cm_SendExSetHEI(2, ev);
        return;
    case 2:
        h->walkSlot = -1;
        if (haunter->scene->actions != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 4, 0);
            e->exSlot = cm_SendExSetHEI(2, ev);
            return;
        }
        if (gm_BuildQuery(1, 1, h->home) != 0) {
            e->end = ai_Time;
            victim = (AICHAR *)gm_FindChar(h->victim);
            if (victim != 0) {
                gm_AddTime(&e->end, 0x18, 0, 0);
                ai_HauntingText((unsigned short *)&ai_CharsT[e->chr], (int)victim, (unsigned int)&e->end);
            }
            e->exSlot = cm_SendExSetHEI(3, ev);
            return;
        }
        e->end = ai_Time;
        e->exSlot = cm_SendExSetHEI(-1, ev);
        return;
    case 3:
        if (haunter->bld == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        h->walkSlot = cm_SendChrGotoBuilding(haunter->netId, haunter->bld->id, -1, 0, 0, "HEIMSUCHUNG");
        e->exSlot = cm_SendExSetHEI(4, ev);
        return;
    case 4:
        if (haunter->bld == 0) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
        if (haunter->scene->actions != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 4, 0);
            return;
        }
        e->end = ai_Time;
        sv_MoveToGroup(0, 0, haunter->id, 0);
        e->exSlot = cm_SendExSetHEI(5, ev);
        years = h->paid >> 1;
        if (years < 1)
            years = 1;
        e->end.year += years + 1;
        return;
    case 5:
        e->exSlot = cm_SendExSetHEI(-1, ev);
        return;
    }
}
/* 0x570130, the alloc of the event 0x89 (a work at +0xb0 by the character +0xb4): paused -> nothing; with a running event 0x89 of the same player and building the text 0x40a2, state -1 and free; else the character +0xb4 missing -> free; the due clock now + 1 hour, 0x50cc50 (pid, -(+0xb8)), state 0 and 0x4f2e30 (0x1b, 1, pid, +4 of the character, the id +1 of +0x170 or -1) */

void he_AllocDecay(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEDECAY *d = (HEDECAY *)ev;
    AICHAR *per;
    AIJOB *r;
    char text[0x400];
    int id, netId;

    if (e->opts & 4)
        return;
    per = (AICHAR *)gm_FindChar(d->victim);
    for (r = (AIJOB *)he_FindEvent(1, 0, 0x89); r != 0; r = (AIJOB *)he_NextEvent()) {
        if (r != e && r->chr == e->chr && ((HEDECAY *)r)->victim == d->victim)
            break;
    }
    if (r != 0) {
        if (per != 0) {
            txt_Format(text, 0x40a2, per->id);
            he_ShowNews(ai_CharInts[e->chr][0], -1, text, 0, 0x5fa, 0);
        }
        e->exSlot = cm_SendExSetHEI(-1, ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_FindChar(d->agent) == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    d->aged = 0;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 1, 0);
    gm_SendMoneyDelta(ai_CharInts[e->chr][0], -d->paid);
    e->stage = 0;
    if (per != 0 && per->bld2 != 0)
        id = per->bld2->id;
    else
        id = -1;
    if (per != 0)
        netId = (int)per->netId;
    else
        netId = -1;
    gv_Accuse(0x1b, 1, ai_CharInts[e->chr][0], netId, id);
}

void ai_TellGraveWorkSuccess(int param_1);                 /* 0x5a9990 */
void ai_TellGraveWorkFailure(int param_1);                 /* 0x5a99e0 */
void ai_GraveyardMessage(int param_1, unsigned short *param_2, unsigned short *param_3, int param_4);                 /* 0x5a9a30 */
void ai_GraveyardMessagePlain(int param_1, unsigned short *param_2, unsigned short *param_3);                 /* 0x5a9af0 */

/* 0x5702e0, the run handler of the graveyard owner's "breath of decay" event 0x89 (alloc he_AllocDecay; am.c's
   ai_ExecGraveyardOwner2 / sv_t.c post it): the stages -1 / -2 take the agent out of the event's group and free
   the event, a lost agent frees it too. Stage 0 puts the agent in the group, stage 1 sends it to the victim's
   building ("HAUCH DES VERFALLS"), stage 2 waits 4 minutes while it walks, then (due in an hour) rolls
   gm_Mix (agent, victim, 2) + random above 1: the victim ages by the building's level / 50 + 0 .. 2 + its type's
   level * 2 years (he_SetLifeByAge, ai_TellGraveWorkSuccess; aged = 1), else ai_TellGraveWorkFailure (aged = 0);
   stage 3 sends the agent back to its building, stage 4 waits until it is there and reports to the event's
   character (aged: ai_GraveyardMessage, its last argument 0 when random + 1 - the agent's skill 1 is above 1;
   else ai_GraveyardMessagePlain); then the event ends (stage -1)
   @deep size queue s17 (raw 1044 with the table, padded 1056 vs 1024): the stage-1 tail is a
   post-allocation cross-jump into stage 3's call (push text; push 1; jmp to stage 3's mov eax,[eax+1]
   after its push 0), so stage 3's argument temps must have stage 1's registers: the original's scratch
   rotation is one step ahead of ours from the -1/-2 block on (that block xor edx,edx / mov dx,[eax] vs
   ours ecx; stage 3 FindChar push edx vs ecx; stage 3's loads eax/ecx vs edx/eax; stage 4 push edx vs
   ecx); the code before it (FindChar, the stage CSE in ecx, compares, table) is identical. Rounds: retgen
   single (12 sites, every return -> break alone / all but one) = all one code, no change in the tail; told
   as short / char / uchar / ushort (flip.py hit: the -1 block's register flips with told int->short)
   677..710 bytes, none closer. flip.py --want the stage-1 'push 1; jmp': no mutant of 487. Next: find the
   temp the original has one more of before the -1 block in code order (c2trace temps; the stage compare
   spelled as a switch / a local). prior: read the whole function against the original: typed as AIJOB +
   HEDECAY (event 0x89, am.c ai_ExecGraveyardOwner2 / sv_t.c), the walk text literal, the default stage
   leaves the event alone, the message's last argument as the original's told flag (!(random + 1 - skill >
   1.0) -> 1), the age sum level / 50 + (random + type level * 2) with he_SetLifeByAge taking an int (its
   callee narrows it: and eax, 0xffff at 0x56fbcd; the caller keeps the and at +0x1bf). Left: the original's
   stage 1 pushes the walk text and 1 and jumps into stage 3's cm_SendChrGotoBuilding call tail, ours keeps
   its own copy (15 instructions, the whole size difference), and stage 4's per address computed after the
   agent id push (+0x321); the M8 at +0x2d2 is the alignment of stage 4's ai_Time copy. */
void he_RunDecay(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEDECAY *d = (HEDECAY *)ev;
    AICHAR *agent, *victim, *per;
    AIBLD *bld;
    AIBTYPE *bt;
    float chance;
    int told;

    agent = (AICHAR *)gm_FindChar(d->agent);
    if (e->stage == -1 || e->stage == -2) {
        if (agent != 0)
            sv_MoveToGroup(0, 0, agent->id, 0);
        he_FreeEvent((int *)ev);
        return;
    }
    if (agent == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (e->stage) {
    case 0:
        sv_MoveToGroup(0, 0, agent->id, (char *)ev);
        e->exSlot = -1;
        e->stage++;
        return;
    case 1:
        bld = (AIBLD *)gm_BuildQuery(1, 1, d->home);
        if (bld == 0) {
            e->stage = -1;
            return;
        }
        cm_SendChrGotoBuilding(d->agent, bld->id, -1, 0, 1, "HAUCH DES VERFALLS");
        e->stage++;
        return;
    case 2:
        gm_BuildQuery(1, 1, d->home);
        agent = (AICHAR *)gm_FindChar(d->agent);
        if (agent == 0) {
            e->stage = -1;
            return;
        }
        if (agent->scene->actions != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 4, 0);
            return;
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 1, 0, 0);
        victim = (AICHAR *)gm_FindChar(d->victim);
        agent = (AICHAR *)gm_FindChar(d->agent);
        chance = gm_Mix((int)agent, (int)victim, 2);
        if (victim != 0) {
            if (util_RandomF() + chance > 1.0) {
                bld = agent->bld;
                bt = &ai_BTypesT[bld->type];
                d->aged = 1;
                he_SetLifeByAge(victim, (util_Random(3) & 0xffff) + bt->cap247 * 2 + bld->level / 50);
                ai_TellGraveWorkSuccess((int)victim);
                e->stage++;
                return;
            }
            ai_TellGraveWorkFailure((int)victim);
            d->aged = 0;
        }
        e->stage++;
        return;
    case 3:
        agent = (AICHAR *)gm_FindChar(d->agent);
        if (agent == 0 || (bld = agent->bld) == 0) {
            e->stage = -1;
            return;
        }
        cm_SendChrGotoBuilding(d->agent, bld->id, -1, 0, 0, "HAUCH DES VERFALLS");
        e->stage++;
        return;
    case 4:
        agent = (AICHAR *)gm_FindChar(d->agent);
        if (agent == 0 || agent->bld == 0) {
            e->stage = -1;
            return;
        }
        if (agent->scene->actions != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 4, 0);
            return;
        }
        gm_BuildQuery(1, 1, d->home);
        victim = (AICHAR *)gm_FindChar(d->victim);
        per = &ai_CharsT[e->chr];
        agent = (AICHAR *)gm_FindChar(d->agent);
        if (victim != 0 && per != 0 && agent != 0) {
            if (d->aged != 0) {
                chance = 1.0f - gm_GetSkill((char *)agent, 1);
                if (!(util_RandomF() + chance > 1.0))
                    told = 1;
                else
                    told = 0;
                ai_GraveyardMessage((int)per, (unsigned short *)agent, (unsigned short *)victim, told);
            } else
                ai_GraveyardMessagePlain((int)per, (unsigned short *)agent, (unsigned short *)victim);
        }
        e->stage = -1;
        return;
    }
}

int ai_PickActionBuildings(struct AIPER *p, int a, __int64 mask, float lo, float hi, unsigned char n, int *out); /* 0x4a6570 */

/* 0x5706e0, the alloc of the plunder event 0x8a: paused -> nothing; the buildings the character (chr) can plunder are
   picked (ai_PickActionBuildings, mask 4, at most 5, into bld) -> none -> free; the picked slot of the found index
   (when below 5) is cleared, at = 0, the due clock is now + 1 hour and the event is scheduled (cm_SendExSetHE)
   @deep only allocation / scheduling is left: ours 54 ins vs the original's 56, differing 42, hunks regs
   32 + one code hunk at 0x5706e5 (+0x5). Read whole against 0x5706e0..0x570795: the meaning is equal - the
   same 8 pushed dwords (p, 5, mask 4, lo 0, hi 0, n 5, out = &bld), the same he_FreeEvent, the same stores
   (bld[r] = 0 for r < 5, at = 0, end = ai_Time, exSlot = cm_SendExSetHE's result), the same constants and
   the same shared epilogue. What differs is the argument setup's schedule and registers: the original
   keeps the mask's 4 in eax (mov eax,4 before the opts test, test al) and the out pointer in edx, zeroes
   the three words with ecx and pushes the mask's low dword from eax; ours tests the immediate 4, computes
   the out pointer in eax and pushes the words as immediates, and the gm_AddTime/cm_SendExSetHE tail uses
   ecx/edx swapped (0x57074d). Tried: typed AIJOB/HEPLUNDER accesses (the current form, no code change),
   the out pointer as a local and as (int *)(ev + 0xb4), and the mask as a local (worse / unchanged,
   reverted). */
void he_AllocPlunder(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEPLUNDER *p = (HEPLUNDER *)ev;
    int r;

    if (e->opts & 4)
        return;
    r = ai_PickActionBuildings((struct AIPER *)((unsigned)&ai_Chars + e->chr * 0x21c), 5, 4, 0, 0, 5, p->bld);
    if (r == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (r < 5)
        p->bld[r] = 0;
    p->at = 0;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 1, 0);
    e->exSlot = cm_SendExSetHEI(0, ev);
}

int ai_PickPartnerRandomWalk(struct AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 */
void he_PlunderTakePrice(int *a, AICHAR *c);   /* 0x570bf0 */
void he_PlunderFailNews(int *a, AICHAR *c);   /* 0x570c70 */

/* 0x5707a0, the run handler of the plunder event 0x8a ("PAKT MIT DEM TOD"; alloc he_AllocPlunder): stage -1 / -2,
   a missing agent (the netId +0xac) or one without health take the agent out of the event's group
   (sv_MoveToGroup) and free the event. Stage 0 puts the agent in the group; stage 1 sends it to the building
   bld[at] (stage 2), none ends the event; stage 2 waits 10 minutes while the agent is busy, else picks a
   victim near the event's character (ai_PickPartnerRandomWalk, liking up to 100, not one of victims[]), notes
   it in victims[at] and, when the victim's worth (gm_GetWorth / gm_ConvertMoney) is above its protection price
   (gv_ProtectionPrice) and its flags1cc bit 0 is clear, asks a player victim (kind 6 / 7) with the message 0x8b,
   else takes the price (he_PlunderTakePrice) or tells of the failure (he_PlunderFailNews) by a coin throw; then the next
   building (stage 1 again) up to 5, else stage 3; stage 3 sends the agent home (stage 4), stage 4 waits 4
   minutes while it is busy there; a missing agent or home ends the event.
   @original case 1 ends with break where the other cases return: VC6 then merges case 2's closing stage++ into
   case 3's copy as the original does (with return it takes case 1's, and case 2's jumps become near jumps) */
void he_RunPlunder(int ev)
{
    unsigned short idx;
    int deal[4];
    AIMSG msg;
    AIJOB *e = (AIJOB *)ev;
    HEPLUNDER *p = (HEPLUNDER *)ev;
    AICHAR *agent;
    AIBLD *bld;
    int price, worth;

    agent = (AICHAR *)gm_FindChar(p->agent);
    if (e->stage == -1 || e->stage == -2 || agent == 0 || agent->health == 0) {
        if (agent != 0)
            sv_MoveToGroup(0, 0, agent->id, 0);
        he_FreeEvent((int *)ev);
        return;
    }
    switch (e->stage) {
    case 0:
        sv_MoveToGroup(0, 0, agent->id, (char *)ev);
        e->stage++;
        e->exSlot = -1;
        return;
    case 1:
        bld = (AIBLD *)gm_BuildQuery(1, 1, p->bld[p->at]);
        if (bld == 0) {
            e->stage = -1;
            return;
        }
        cm_SendChrGotoBuilding(p->agent, bld->id, -1, 0, 1, "PAKT MIT DEM TOD");
        e->stage++;
        break;
    case 2:
        agent = (AICHAR *)gm_FindChar(p->agent);
        if (agent == 0) {
            e->stage = -1;
            return;
        }
        if (agent->scene->actions != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 10, 0);
            return;
        }
        if (ai_PickPartnerRandomWalk((struct AIPER *)&ai_CharsT[e->chr], PLUNDER_PARTNER_FLAGS, 4, p->victims, 0, 100.0f, &idx) != 0) {
            price = gv_ProtectionPrice((unsigned short *)&ai_CharsT[idx]);
            worth = gm_ConvertMoney(gm_GetWorth(idx), ai_OwnPlayer);
            p->victims[p->at] = idx;
            if (worth > price && !(ai_CharsT[idx].flags1cc & 1)) {
                if (ai_CharsT[idx].kind == 6 || ai_CharsT[idx].kind == 7) {
                    msg.type = 0x8b;
                    msg.to = ai_CharsT[idx].netId;
                    msg.fromId = ai_CharsT[idx].netId;
                    msg.nIds = 0;
                    msg.reserved14 = 0;
                    msg.time = ai_Time;
                    msg.flags = 9;
                    msg.arg[0] = e->netId;
                    msg.arg[1] = p->agent;
                    msg.arg[2] = ai_CharsT[idx].netId;
                    msg.arg[3] = price;
                    cm_SendLongMessage((char *)&msg);
                } else {
                    deal[0] = e->netId;
                    deal[1] = p->agent;
                    deal[2] = ai_CharsT[idx].netId;
                    deal[3] = price;
                    if (util_RandomU(2) != 0)
                        he_PlunderTakePrice(deal, &ai_CharsT[idx]);
                    else
                        he_PlunderFailNews(deal, &ai_CharsT[idx]);
                }
            }
            p->at++;
            if (p->at < 5 && p->bld[p->at] != 0) {
                e->stage--;
                return;
            }
            e->stage++;
            return;
        }
        e->stage = -1;
        return;
    case 3:
        agent = (AICHAR *)gm_FindChar(p->agent);
        if (agent == 0 || agent->bld == 0) {
            e->stage = -1;
            return;
        }
        cm_SendChrGotoBuilding(p->agent, agent->bld->id, -1, 0, 0, "PAKT MIT DEM TOD");
        e->stage++;
        return;
    case 4:
        agent = (AICHAR *)gm_FindChar(p->agent);
        if (agent == 0 || agent->bld == 0 || agent->scene->actions == 0) {
            e->stage = -1;
            return;
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 4, 0);
        return;
    }
}
/* 0x570bf0, a plunder message: 0x4c4830 (a[0], b + 4, the byte 0x6f1bc5, a[3]), 0x4c4a60 (b + 4, 0x1cc, 4, 1, 0), the text 0x40fe to a[0] (flag 1, 0x5f4) */

void he_PlunderTakePrice(int *a, AICHAR *c)
{
    char text[0x400];

    ((int (*)(int, int, unsigned char, int))cm_SendMoveGoodsUnchecked)(a[0], c->netId, ai_OwnPlayer, a[3]);
    cm_SendSetField(c->netId, 0x1cc, 4, 1, 0);
    txt_Format(text, 0x40fe, c->id, a[3]);
    he_ShowNews(a[0], -1, text, 1, 0x5f4, 0);
}
/* 0x570c70, the text 0x40ff (the character's id) to a[0] (flag 1, 0x5f4) */

void he_PlunderFailNews(int *a, AICHAR *c)
{
    char text[0x400];

    txt_Format(text, 0x40ff, c->id);
    he_ShowNews(a[0], -1, text, 1, 0x5f4, 0);
}

int he_OpenEventSlot();                 /* 0x53d6f0 */

/* 0x570cc0, the run handler of the plunder offer 0x8b (he_RunPlunder sends it to a player victim; its words over
   AIJOB's union are the plunder deal HEPLUNDERDEAL): a missing client (the deal's netId) or a player whose
   flags1cc bit 0 is set (he_RunPlunder skips such a victim) end it (stage -2: the event bar slot is released, the event freed). Stage 0 opens the
   offer window ("Berufe\Messagebox_Meisterpruefung") with the text 0x40fd (the client, the price) and the voice
   "_NACHRICHTEN_FRIEDHOF_09", stage 1; a window that could not be opened frees the event. Stage 1: past the
   due time the offer counts as refused (he_PlunderFailNews); without the window back to stage 0; a click on its
   button 0 pays the price (he_PlunderTakePrice), on button 1 refuses; either ends the event. */
void he_RunPlunderOffer(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEPLUNDERDEAL *d = (HEPLUNDERDEAL *)ev;
    AICHAR *client;
    int id;

    client = (AICHAR *)gm_FindChar(d->deal[0]);
    if (client == 0 || (ai_CharsT[e->chr].flags1cc & 1))
        e->stage = -2;
    if (e->stage == -2) {
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    switch (e->stage) {
    case 0:
        if (e->slot == 0)
            he_OpenEventSlot(ev, 0, 0, "Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        if (e->slot == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(e->slot->win);
        d2_SetForm(e->slot->win, 0);
        io_PrintText("$C");
        io_PrintText((char *)0x40fd, client->id, d->deal[3]);
        snd_QueueVoice(-7, 0, "_NACHRICHTEN_FRIEDHOF_09", -1, 0);
        e->stage = 1;
        return;
    case 1:
        if (gm_CompareTime(&ai_Time, &e->end) > 0) {
            he_PlunderFailNews(d->deal, &ai_CharsT[ai_PlayerId]);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (e->slot == 0) {
            e->stage = 0;
            return;
        }
        if (d2_ClickedForm != e->slot->win)
            return;
        id = d2_ClickedId;
        if (id == -1)
            return;
        if (id == 0) {
            he_PlunderTakePrice(d->deal, &ai_CharsT[ai_PlayerId]);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (id == 1) {
            he_PlunderFailNews(d->deal, &ai_CharsT[ai_PlayerId]);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
        }
        return;
    }
}

int ai_PickPartnerRandomWalk(struct AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 */
void he_PlunderTakePrice(int *a, AICHAR *c);   /* 0x570bf0 */
void he_PlunderFailNews(int *a, AICHAR *c);   /* 0x570c70 */
int he_OpenEventSlot();                 /* 0x53d6f0 */
int sv_UsedGroupSlots();                 /* 0x5241b0 */
int gv_ListEntry(char *r, int i, unsigned char n);                 /* 0x4ef570 */
void ai_HauntingText(unsigned short *param_1, int param_2, unsigned int param_3);                 /* 0x5a9910 */

/* 0x570ea0, 1 when no running haunting event 0x88 has b's netId as its character (+8) and a's netId as its victim
   (+0xb0); 0 when one does */
int he_NoEvent88Pair(AICHAR *a, AICHAR *b)
{
    AIJOB *r;

    for (r = (AIJOB *)he_FindEvent(1, 0, 0x88); r != 0; r = (AIJOB *)he_NextEvent()) {
        if ((unsigned int)b->netId == r->chr && ((HEHAUNTING *)r)->victim == a->netId)
            return 0;
    }
    return 1;
}
/* 0x570ef0, the alloc of the master's production job 0x8c: the building (gm_BuildQuery 0x501190 on AIJOB.from), its
   object list (gm_Query 0x4ffe90, key 0xfd), and each of the four workers (AIJOB.ids[0..3], gm_FindChar) registered
   in the group (sv_MoveToGroup 0x524260); none -> the event is freed. Else the job starts: made 0, the counted-up
   clock since = now, 0x168 working minutes left, the due clock (AIJOB.end) = now + 5 minutes, stage 0 */
void he_AllocMasterWork(int ev)
{
    int i, n = 0;
    AIJOB *e = (AIJOB *)ev;
    HEMASTERWORK *p = (HEMASTERWORK *)ev;
    AIBLD *bld = (AIBLD *)gm_BuildQuery(1, 1, e->from);
    short *grp = (short *)gm_Query((void *)bld->objList, 2, 0, 0xfd, 6);
    AICHAR *w;

    for (i = 0; i < 4; i++) {
        w = (AICHAR *)gm_FindChar(e->ids[i]);
        if (w != 0) {
            n++;
            sv_MoveToGroup((unsigned char *)bld, grp, w->id, (char *)ev);
        }
    }
    if (n == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    p->made = 0.0f;
    p->since = ai_Time;
    p->left = 0x168;
    e->end = ai_Time;
    e->stage = 0;
    gm_AddTime(&e->end, 0, 5, 0);
}
/* 0x571000, the run handler of a master's production job 0x8c (alloc he_Unk570ef0; ai.c's master step posts it
   for a graveyard 0x1d or a gypsy camp 0x1f): outside the working hours of the season (gm_DayStartHours ..
   gm_DayEndHours) a running job only waits 5 minutes. A job without its character ends at once. The four
   workers (ids[]) are counted, the ones with the worker titles 0x1b / 0x1c at work (flags1b4 bit 0) count as
   idle; none, no building (from) or no store in it (gm_Query kind 0x2a) end the job (stage -1). Stage 0 adds
   per busy worker (skill + 1) * the minutes since `since` / 60 to `made` (the skill: gm_BuildingSkill 0x12 at
   a graveyard, else 0x1e, less 0.5 in winter without the heating bit 0x2000), takes the minutes off `left` (at
   0 the job ends) and waits 5 minutes. An ended job stores the whole units made (at least 1; none when it
   was cancelled with its group empty) in the store (gm_FreeCapacity, cm_SendExSellObject) and tells a player
   master who is not at work of it (he_MessageBoxQuickjump: the texts 0x4222 / 0x4223 / 0x4224 at a gypsy camp,
   0x412c / 0x412d / 0x412e else; a full store 0x4225 / 0x412f; the voices "_NACHRICHTEN_ZIGEUNER_03" /
   "_NACHRICHTEN_FRIEDHOF_01"); then the workers leave the group and the event is freed. The 1.0f is a literal
   (VC6's pooled 1.0f, 0x5eb780): spelled as an object VC6 loads it first and keeps the rate on
   the FPU stack (fld / fadd st, st(1)), which the original's fadd [0x5eb780] does not. */
void he_RunMasterWork(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEMASTERWORK *p = (HEMASTERWORK *)ev;
    char voice[0x98];
    char what[0x200];
    char text[0x400];
    int season, hour, i, n, cnt, busy, minutes;
    float rate, step;
    AICHAR *w, *master;
    AIBTYPE *bt;
    AIBLD *bld;
    AIOBJ *store;

    season = gm_ClockSeason(&ai_Time);
    hour = ai_Time.hour;
    store = 0;
    cnt = 0;
    busy = 0;
    bt = 0;
    if ((float)hour >= gm_DayEndHours[season] || (float)hour < gm_DayStartHours[season]) {
        if (e->stage != -1 && e->stage != -2) {
            gm_AddTime(&e->end, 0, 5, 0);
            return;
        }
    }
    if (e->chr == 0xffff || e->netId == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    for (i = 0; i < 4; i++) {
        w = (AICHAR *)gm_FindChar(e->ids[i]);
        if (w != 0) {
            cnt++;
            busy++;
            if ((w->title == 0x1b || w->title == 0x1c) && (w->flags1b4 & 1))
                busy--;
        }
    }
    if (cnt == 0)
        e->stage = -1;
    bld = (AIBLD *)gm_BuildQuery(1, 1, e->from);
    if (bld != 0) {
        bt = &ai_BTypesT[bld->type];
        store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
        if (store == 0)
            e->stage = -1;
    } else
        e->stage = -1;
    if (e->stage == 0) {
        minutes = gm_MinutesBetween(&p->since, &ai_Time);
        if (bt->kind == 0x1d) {
            rate = gm_BuildingSkill(bld, 0x12, 1) * 0.01f;
        } else {
            rate = gm_BuildingSkill(bld, 0x1e, 1) * 0.01f;
            if (season == 3 && !(bld->flags5aw & 0x2000))
                rate -= 0.5f;
        }
        step = (rate + 1.0f) * minutes * 0.01667f;
        for (i = busy; i > 0; i--)
            p->made += step;
        p->left -= minutes;
        p->since = ai_Time;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 5, 0);
        if (p->left < 0)
            e->stage = -1;
    }
    if (e->stage != -2 && e->stage != -1)
        return;
    n = (int)p->made;
    if (n <= 1)
        n = 1;
    if (e->stage == -1 && sv_UsedGroupSlots(ev) <= 0)
        n = -1;
    if (bld != 0 && store != 0 && n > 0) {
        master = (AICHAR *)gm_FindCharacterByMaster((int)bld);
        n = gm_FreeCapacity(store, p->good, n);
        if (n != 0) {
            cm_SendExSellObject(store->objId, -1, p->good, n, ai_OwnPlayer, 0);
            if (bt->kind == 0x1f) {
                txt_Format(text, 0x4222, 0x421e, 0x22c, bld);
                strcpy(voice, "_NACHRICHTEN_ZIGEUNER_03");
                if (n == 1)
                    txt_Format(what, 0x4223, p->good * 2 + 0x9a8);
                else
                    txt_Format(what, 0x4224, n, p->good * 2 + 0x9a8);
            } else {
                txt_Format(text, 0x412c, 0x4128, 0x228, bld);
                strcpy(voice, "_NACHRICHTEN_FRIEDHOF_01");
                if (n == 1)
                    txt_Format(what, 0x412d, p->good * 2 + 0x9a8);
                else
                    txt_Format(what, 0x412e, n, p->good * 2 + 0x9a8);
            }
            strcat(text, what);
        } else if (bt->kind == 0x1f) {
            txt_Format(text, 0x4225, 0x421e, 0x22c, bld);
            strcpy(voice, "_NACHRICHTEN_ZIGEUNER_03");
        } else {
            txt_Format(text, 0x412f, 0x4128, 0x228, bld);
            strcpy(voice, "_NACHRICHTEN_FRIEDHOF_01");
        }
        if (ai_CharsT[e->chr].kind == 6 || ai_CharsT[e->chr].kind == 7) {
            if (master == 0 || !(master->flags1b4 & 1))
                he_MessageBoxQuickjump(ai_CharsT[e->chr].netId, -1, text, 0, 0x5ec, bld->id, -1, 0, voice);
        }
    }
    for (i = 0; i < 4; i++) {
        w = (AICHAR *)gm_FindChar(e->ids[i]);
        if (w != 0)
            sv_MoveToGroup(0, 0, w->id, 0);
    }
    he_FreeEvent((int *)ev);
}
/* 0x571550, the alloc of the grave care job 0x8d: a missing worker (AIJOB.ids[0], gm_FindChar) or building (AIJOB.from,
   gm_FindBuildingById 0x502020) frees the event; else the worker is registered (sv_MoveToGroup 0x524260) in the group
   gm_Query 0x4ffe90 on the building's object list (key 0xfd), the care done is 0, 0x1fe working minutes are left, the
   counted-up clock since and the due clock (AIJOB.end) = now, the due clock + 5 minutes, stage 0 */
void he_AllocGraveCare(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGRAVECARE *p = (HEGRAVECARE *)ev;
    AICHAR *per = (AICHAR *)gm_FindChar(e->ids[0]);
    AIBLD *bld;
    short *grp;

    if (per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    bld = (AIBLD *)gm_FindBuildingById(e->from);
    if (bld == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    grp = (short *)gm_Query((void *)bld->objList, 1, 0, 0xfd);
    sv_MoveToGroup((unsigned char *)bld, grp, per->id, (char *)ev);
    p->care = 0.0f;
    p->left = 0x1fe;
    p->since = ai_Time;
    e->end = ai_Time;
    e->stage = 0;
    gm_AddTime(&e->end, 0, 5, 0);
}
/* 0x571640, the run handler of the grave care job 0x8d (alloc he_AllocGraveCare; ai.c's ai_GraveCare posts it with the
   grave's list size and entry): outside the working hours of the season (gm_DayStartHours .. gm_DayEndHours) a
   running job only restarts its clock and waits 5 minutes. A job without its character is freed at once. A
   missing worker (ids[0]; else it joins the event's group), building (from) or grave entry (gv_ListEntry of
   the building's graveyard) ends the job (stage -1). Stage 0 adds (4 - the entry's size) * the minutes since
   `since` * 0.1 to `care` (a quarter of it for a worker with the title 0x1b / 0x1c at work), waits 5 minutes,
   takes the minutes off `left` (at 0 the job ends) and ends it too when the entry's b0c + the care reaches
   0xff. An ended job hands the whole care to the grave (gv_OrderAdjustEntry) and tells a player master who is
   not at work of it (the text 0x4126 / 0x411e, the voice "_NACHRICHTEN_FRIEDHOF_02"); the worker leaves the
   group and the event is freed. */
void he_RunGraveCare(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGRAVECARE *g = (HEGRAVECARE *)ev;
    char text[0x400];
    int season, hour, minutes, n;
    float x;
    AICHAR *w, *master;
    AIBLD *bld;
    GVENTRY *entry;

    season = gm_ClockSeason(&ai_Time);
    hour = ai_Time.hour;
    if ((float)hour >= gm_DayEndHours[season] || (float)hour < gm_DayStartHours[season]) {
        if (e->stage != -1 && e->stage != -2) {
            g->since = ai_Time;
            gm_AddTime(&e->end, 0, 5, 0);
            return;
        }
    }
    if (e->chr == 0xffff || e->netId == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    w = (AICHAR *)gm_FindChar(e->ids[0]);
    if (w != 0)
        sv_MoveToGroup(0, 0, w->id, (char *)ev);
    else
        e->stage = -1;
    bld = (AIBLD *)gm_BuildQuery(1, 1, e->from);
    if (bld == 0)
        e->stage = -1;
    entry = (GVENTRY *)gv_ListEntry((char *)bld->yard, g->size, g->entry);
    if (entry == 0)
        e->stage = -1;
    if (e->stage == 0) {
        minutes = gm_MinutesBetween(&g->since, &ai_Time);
        x = (float)(4 - entry->size) * minutes * 0.1f;
        if ((w->title == 0x1b || w->title == 0x1c) && (w->flags1b4 & 1))
            x *= 0.25f;
        g->care += x;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 5, 0);
        g->left -= minutes;
        g->since = ai_Time;
        if (g->left <= 0)
            e->stage = -1;
        if (entry->b0c + (int)g->care >= 0xff)
            e->stage = -1;
    }
    if (e->stage < -2 || e->stage >= 0)
        return;
    n = (int)g->care;
    if (entry != 0 && n > 0) {
        gv_OrderAdjustEntry(bld, g->size, g->entry, n);
        if (w != 0) {
            master = (AICHAR *)gm_FindCharacterByMaster((int)bld);
            txt_Format(text, 0x4126, 0x411e, w->id, bld);
            if ((ai_CharsT[e->chr].kind == 6 || ai_CharsT[e->chr].kind == 7) && (master == 0 || !(master->flags1b4 & 1)))
                he_MessageBoxQuickjump(ai_CharsT[e->chr].netId, -1, text, 0, 0x5ec, bld->id, -1, 0, "_NACHRICHTEN_FRIEDHOF_02");
        }
    }
    if (w != 0)
        sv_MoveToGroup(0, 0, w->id, 0);
    he_FreeEvent((int *)ev);
}
/* 0x571990, the alloc of the death-due event 0x8e: the due clock (AIJOB.end) = now + 0x48 hours, stage 0 */
void he_AllocDeathDue(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->end = ai_Time;
    e->stage = 0;
    gm_AddTime(&e->end, 0x48, 0, 0);
}
/* 0x5719e0, the run handler of the event 0x8e (alloc he_AllocDeathDue: due 72 hours after its start; a reasoned
   guess from what it does: the due day of a pact with death): only at the stages -2 .. 0. The event's character
   (netId) pays the price (`price`, cm_SendMoveGoods to nobody) when its money (gm_CharCurrency) is above it;
   else every living relative of relatives[3..7] (the descendants ai_DecideUseDynastyObject walks) loses 1001
   health (gm_AddHealth), or the character itself when there is none. The event is then freed (twice on the
   unpaid path, as the original does: once in the branch, once at the end). */
void he_RunDeathDue(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEDEATHDUE *d = (HEDEATHDUE *)ev;
    AICHAR *ch, *r;
    int i, found, money;

    if (e->stage < -2 || e->stage > 0)
        return;
    ch = (AICHAR *)gm_FindChar(e->netId);
    if (ch != 0) {
        money = gm_CharCurrency((char *)ch, ai_OwnPlayer);
        if (money > d->price)
            cm_SendMoveGoods(-1, ch->netId, ai_OwnPlayer, d->price);
        else {
            found = 0;
            for (i = 0; i < 5; i++) {
                if (ch->relatives[3 + i] != -1) {
                    r = (AICHAR *)gm_FindChar(ch->relatives[3 + i]);
                    if (r != 0) {
                        gm_AddHealth(r->id, -1001);
                        found = 1;
                    }
                }
            }
            if (!found)
                gm_AddHealth(ch->id, -1001);
            he_FreeEvent((int *)ev);
        }
    }
    he_FreeEvent((int *)ev);
}
/* 0x571ac0, the alloc of the grave training job 0x8f: a missing trainee (HEGRAVETRAINING.trainee, gm_FindChar), a
   trainee already on a running job 0x8f (AICHAR.job's kind) or a missing building (AIJOB.from, gm_BuildQuery 0x501190)
   frees the event; with an object id (HEGRAVETRAINING.objId != -1) the group is gm_Query 0x4ffe90 on the building's
   object list (key 0xfd, the object 6, the id); the due clock (AIJOB.end) and the start clock (AIJOB.start) = now, the
   trainee is registered (sv_MoveToGroup 0x524260) in the group (or alone without one), the second clock (AIJOB.clock60)
   = now, the due clock + 0x1e minutes (gm_AddTime 0x4fe140), and until = the due clock, 3 hours later */
void he_AllocGraveTraining(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGRAVETRAINING *p = (HEGRAVETRAINING *)ev;
    short *grp = 0;
    AICHAR *per = (AICHAR *)gm_FindChar(p->trainee);
    AIBLD *bld;

    if (per != 0 && (per->job == 0 || (unsigned char)per->job->kind != 0x8f)
        && (bld = (AIBLD *)gm_BuildQuery(1, 1, e->from)) != 0) {
        if (p->objId != -1)
            grp = (short *)gm_Query((void *)bld->objList, 2, 6, 1, p->objId);
        e->end = ai_Time;
        e->start = ai_Time;
        if (grp != 0)
            sv_MoveToGroup((unsigned char *)bld, grp, per->id, (char *)ev);
        else
            sv_MoveToGroup((unsigned char *)bld, 0, per->id, (char *)ev);
        e->clock60 = ai_Time;
        gm_AddTime(&e->end, 0, 0x1e, 0);
        p->until = e->end;
        gm_AddTime(&p->until, 3, 0, 0);
    } else
        he_FreeEvent((int *)ev);
}
/* 0x571c30, the run handler of the training job 0x8f (ai_SendFreeSpies posts its message for the free characters
   of a graveyard building: the trainee's netId, the skill index 1 / 2 / 4, the object 0x211's id; alloc
   he_AllocGraveTraining): only at the stages -2 .. 0, -1 / -2 free the event. Stage 0, while the trainee's skill is
   below 252 and the end time `until` is not reached: every 30 minutes the skill gains the working minutes
   (gm_WorkMinutesBetween since clock60) / 30 * 8 * (1 + the building's skill 0xc / 100), divided by
   (1 + skill / 252) and capped at 252 (sent as a field value of the character, cm_AddFieldValues); a missing
   building frees the event. At the end (no trainee, the skill full or the time up) a player whose master is not
   at work is told (the text 0x1bc8 at the object, else at the building's main room; the voice
   "_NACHRICHTEN_HS_81"), the trainee leaves the group and the event is freed. The gain's spelling
   S * ((minutes / 30.0) * 8.0) gives the original's fmul order (the parentheses keep 1/30 and 8.0 apart and
   unfolded, the building factor first; he_RunStudy byte with it). */
void he_RunGraveTraining(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGRAVETRAINING *t = (HEGRAVETRAINING *)ev;
    char text[0x200];
    int minutes;
    float bskill;
    unsigned char *skill;
    AICHAR *ch, *master;
    AIBLD *bld;
    AIOBJ *obj;

    if (e->stage < -2)
        return;
    if (e->stage < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage != 0)
        return;
    ch = (AICHAR *)gm_FindChar(t->trainee);
    if (ch != 0 && (float)ch->skills[t->skill] < 252.0f && gm_CompareTime(&ai_Time, &t->until) < 0) {
        unsigned char gain;

        bld = (AIBLD *)gm_BuildQuery(1, 1, e->from);
        if (bld == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        bskill = (float)gm_BuildingSkill(bld, 0xc, 1);
        minutes = gm_WorkMinutesBetween(&e->clock60, &ai_Time, bld);
        gain = (unsigned char)((bskill * 0.01 + 1.0) * ((minutes / 30.0) * 8.0));
        skill = &ch->skills[t->skill];
        gain = (unsigned char)(gain / (*skill * (1.0f / 252) + 1.0));
        if ((float)(gain + *skill) > 252.0f)
            gain = (unsigned char)(252.0f - *skill);
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldValues(1, 1, (char *)ch->skills + (t->skill - (int)cm_FieldObject), &gain);
        cm_SendFieldDeltas();
        e->end = ai_Time;
        e->clock60 = ai_Time;
        gm_AddTime(&e->end, 0, 0x1e, 0);
        return;
    }
    ch = (AICHAR *)gm_FindChar(t->trainee);
    bld = (AIBLD *)gm_BuildQuery(1, 1, e->from);
    if (bld == 0 || ch == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    master = (AICHAR *)gm_FindCharacterByMaster((int)bld);
    if (master == 0 || !(master->flags1b4 & 1)) {
        obj = (AIOBJ *)gm_Query((void *)bld->objList, 1, 1, t->objId);
        if (obj == 0)
            obj = (AIOBJ *)gm_GetGebaeudeHauptraum((unsigned char *)bld);
        if (obj != 0) {
            txt_Format(text, 0x1bc8, ch->id, bld);
            he_MessageBoxQuickjump(ai_CharsT[e->chr].netId, e->from, text, 0, 0x5f2, bld->id, obj->objId, ch_EmptyName, "_NACHRICHTEN_HS_81");
        }
    }
    sv_MoveToGroup((int)bld, 0, ch->id, 0);
    he_FreeEvent((int *)ev);
}
/* 0x571f60, the alloc of the meeting event 0x90: stage 0, then paused -> nothing; the due clock = now; another
   running event 0x90 of the building (from) -> exSlot = cm_SendExSetHEI(-1, ev) and return; else the message 0x91
   (to the netId, from the building bldId, the clock, flags 0xa, arg 0 = the id, arg 1 = -1, arg 2 = unkb0) is
   sent, unkb4 / unkb0 are capped at 2, unkb8 = 0, the money -((unkb4 + 2) * (unkb0 + 2)) is taken from the
   character, due now + 5 hours, then the event is scheduled (cm_SendExSetHE)
   @deep deep s23: left the money product's two loads swapped (original kind [+0xb4] into ecx first, ours
   radius). Operand order / negation spellings (8 forms), climb round 1, r as AIJOB *, e->end moved, 1500
   sampled orders of the msg fill: = control or worse. flip.py --want 'mov ecx,[esi+180]': deleting
   msg.type / msg.flags / msg.arg[1] / msg.arg[2] or swapping memset with msg.type gives the original's
   order, so it is a value-numbering tie decided by the message fill above, not by the product's spelling.
   prior: near-byte sweep s22: m->rounds = 0 before the clamps (6 -> 4 ins). Left: the cost product's two
   field loads in the other order (original [+0xb4] into ecx first). = control: factors swapped, -(a)*b,
   a*b*-1, 0 - a*b. */
void he_AllocMeeting(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEMEETING *m = (HEMEETING *)ev;
    AIMSG msg;
    char *r;

    e->stage = 0;
    if (e->opts & 4)
        return;
    e->end = ai_Time;
    for (r = he_FindEvent(2, 0, 0x90, 3, e->from); r != 0; r = he_NextEvent()) {
        if ((int)r != ev) {
            e->exSlot = cm_SendExSetHEI(-1, ev);
            return;
        }
    }
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x91;
    msg.to = e->netId;
    msg.fromId = m->bld;
    msg.time = ai_Time;
    msg.flags = 0xa;
    msg.arg[0] = e->id;
    msg.arg[1] = -1;
    msg.arg[2] = m->radius;
    cm_SendLongMessage((char *)&msg);
    m->rounds = 0;
    if (m->kind > 2)
        m->kind = 2;
    if (m->radius > 2)
        m->radius = 2;
    gm_SendMoneyDelta(ai_CharInts[e->chr][0], -((m->kind + 2) * (m->radius + 2)));
    gm_AddTime(&e->end, 0, 5, 0);
    cm_SendExSetHEI(0, ev);
}


/* 0x5720f0, the meeting's disease lottery (he_SpreadMeetingDisease calls it per character): with the chance `chance`
   (util_RandomF) the character ch catches one of the diseases of the group `kind` that it does not have yet (its
   word v02c holds the diseases 2 .. 9 as bit fields): kind 2 the diseases 2 / 3, kind 1 the diseases 4 / 5, any
   other 6 and a coin throw of 7 / 8 (one of two free ones by a coin throw). The disease's record (gm_GetDisease)
   gives its severity util_Random(range); a severity above 0 is written into the disease's bit field (a field
   delta of v02c) and costs severity * the record's cost health (gm_AddHealth), the health left stored in *health
   when given. Returns the disease kind, 0 when none was caught.
   @deep s15: more forms of a = 0 before the call, all below ch again: a = 0 at the top with b = 0 inside
   the test (80 ins), a = b = 0 at the top with if (util_RandomF() < chance) (82), the early return form if
   (.. >= chance) return 0; (104), one final return a; with the b == 0 / chance-fail paths reaching it
   (93-95: VC6 folds them to xor eax,eax as the original, but a still loses ebx). climb from a = 0; b = 0;
   at the top (2 rounds): the only movers are meaning changes (gm_AddHealth only when health != 0: 52; b's
   coin inside the 0x1800000 test: 37). Kept: the s14 text (2 ins, right size). prior: size queue s14:
   right size and shape (78 -> 2 ins, raw 541 = 541): a = 0 inside the chance test (b = 0 stays at the
   top): a no longer lives across util_RandomF's block, its priority (c2trace: a#1 -12 against ch -1) rises
   above ch, so a takes ebx and ch is split / reloaded from its slot as the original (frame 0x10). Left:
   the original's xor ebx,ebx (a = 0) sits before the call with the xor esi,esi; any a = 0 before the call
   (a = 0; b = 0 / b = 0; a = 0 / a = b = 0 / b = a = 0 / declared inits / comma in the condition / a float
   temp for the roll) drops a below ch again (78-80 ins); both inside: 4 ins. prior: read whole against
   0x5720f0..0x57230c: every statement, constant, callee and field equals the original (the and/je chance
   test as an if around the body, the kind 2 / 1 / other bit tests of v02c, b = coin ? 7 : 8, the 0x1800000
   / 0x700000 checks, the pick, gm_GetDisease into the 12-byte... */
int he_CatchMeetingDisease(AICHAR *ch, float chance, int kind, float *health)
{
    unsigned int bits, delta;
    GMDISEASE d;
    int a, b, cost;     /* b: the second pick, then the severity (the original reuses it) */
    float res;

    b = 0;
    if (!(util_RandomF() >= chance)) {
        a = 0;
        if (kind == 2) {
            if ((ch->v02c & 0xf0) == 0)
                a = 2;
            if ((ch->v02c & 0xf00) == 0)
                b = 3;
        } else if (kind == 1) {
            if ((ch->v02c & 0x3000) == 0)
                a = 4;
            if ((ch->v02c & 0x1c000) == 0)
                b = 5;
        } else {
            if ((ch->v02c & 0xe0000) == 0)
                a = 6;
            b = util_RandomU(2) ? 7 : 8;
            if ((ch->v02c & 0x1800000) != 0 && b == 8)
                b = 0;
            if ((ch->v02c & (7 << 0x14)) != 0 && b == 7)
                b = 0;
        }
        if (a != 0) {
            if (b != 0 && util_RandomU(2) == 0)
                a = b;
        } else {
            if (b == 0)
                return 0;
            a = b;
        }
        bits = ch->v02c;
        if (gm_GetDisease(a, &d) == 0)
            return a;
        b = util_RandomU(d.range);
        cost = (int)(b * d.cost);
        if (b == 0)
            return a;
        if (a == 2)
            bits ^= ((b << 4) ^ bits) & 0xf0;
        else if (a == 3)
            bits ^= ((b << 8) ^ bits) & 0xf00;
        else if (a == 4)
            bits ^= ((b << 0xc) ^ bits) & 0x3000;
        else if (a == 5)
            bits ^= ((b << 0xe) ^ bits) & 0x1c000;
        else if (a == 6)
            bits ^= ((b << 0x11) ^ bits) & 0xe0000;
        else if (a == 7)
            bits ^= ((b << 0x14) ^ bits) & (7 << 0x14);
        else if (a == 8)
            bits ^= ((b << 0x17) ^ bits) & 0x1800000;
        else if (a == 9)
            bits ^= ((b << 0x19) ^ bits) & 0x1e000000;
        delta = bits;
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldDelta(4, 1, 0x2c, &delta);
        cm_SendFieldDeltas();
        res = gm_AddHealth(ch->id, -cost);
        if (health != 0)
            *health = res;
        return a;
    }
    return 0;
}
/* 0x572310, the disease spread of a meeting (the meeting event 0x90's run handler calls it): around the building
   b (it needs its 3D object) every other building of a kind in the meeting mask (the kinds 1, 2, 4, 8, 0xa, 0xe,
   0x12 .. 0x16, 0x1f and 0x20) with an owner who is not the player and an object within the radius (radius kind 2:
   1500, 1: 800, else 400) is visited; each character of the round's quarter (netId & 3 == group: the meeting
   passes its round HEMEETING.rounds) who works there (bld:
   mode 3), lives there (bld2: mode 2) or owns it (mode 1) gets the disease lottery he_CatchMeetingDisease of the
   kind group `kind` with the chance (1 - dist / radius) * 0.5 + 0.25. A disease caught by a worker is reported as
   the owner's character illness (he_CharIllness); one caught by a player owner (kind 6 / 7) is told to it (the
   text 0x163c with the disease's name 0x1632 + kind). The positions are three 16-byte vectors as the original's
   frame shows (0x844: the text, five words, three D3VECTOR4).
   @deep size queue s10: padded size now the original's (672; raw 672 vs 668): bt = &ai_BTypesT[rec->type]
   between lo = 0 and hi = 0 (climb.py swap, 17 -> 5 ins; the eax/edx/ebp rotation at the kind test fixed).
   Left (fpu 7): after fsqrt the original pops the two dying d values first ('fstp st(2); fstp st(0); fst
   [dist]; fcomp lim'), ours stores dist and reloads ('fstp [dist]; fstp st(0); fstp st(0); fld [dist]').
   c2fpu: d+4 / d+8 on the stack die in the sqrt statement, dist memory (sav 0 / -16). No lever s10 (= one
   code): no (float) cast, if ((dist = sqrt(..)) > lim), the sum in x/y/z order, owner after the sqrt;
   worse: lim < dist (11), !(dist <= lim) (9), >= (7). climb 3 more rounds from the kept text: nothing. The
   parenthesised copies (o->pos.x) are still a VC6 perturbation (flagged by the s9 session); without them
   65 ins. prior: read whole against 0x572310..0x5725ab and fixed the meaning (all 768 characters, not
   0x80; the building type through ai_BTypesT with the unsigned kind > 0x1f split; the 0x807c4116 / 1 kind
   masks; the player test against *ai_PlayerChar; bld 3 / bld2 2 / owner 1 modes; the chance (ai_FloatOne -
   dist / lim) * 0.5f + 0.25f; he_CharIllness / the 0x163c news); the frame now equals the original's 0x844
   with three D3VECTOR4 locals (pos, at, d). Left: instruction selection and allocation only - the original
   copies the object positions with the FPU ('+ fld dword ptr [eax + 0x4c]; fstp dword ptr [esp + 0x14]'
   and keeps at.x / at.y on the FPU stack, storing only at.z) where ours copies with integer moves ('- mov
   ecx, dword ptr [eax + 0x4c] ... mov dword ptr [esp + 0x24], ecx'), and the register choice that follows
   (regs 60). Tried: field-wise and struct copies, D3VECTOR / D3VECTOR4 / separate floats... */
void he_SpreadMeetingDisease(AIBLD *b, int radiusKind, int kind, int group)
{
    char text[0x800];
    float lim, dist, f;
    D3VECTOR4 pos, at, d;
    unsigned int lo, hi;
    int i, n, v, mode;
    AIBTYPE *bt;
    AIBLD *rec;
    AICHAR *owner, *ch;
    D3OBJECT *o;

    if (b->object == 0)
        return;
    if (radiusKind == 2)
        lim = 1500.0f;
    else if (radiusKind == 1)
        lim = 800.0f;
    else
        lim = 400.0f;
    o = (D3OBJECT *)b->object;
    pos.x = (o->pos.x);
    pos.y = (o->pos.y);
    pos.z = (o->pos.z);
    for (n = 0; n < 0x100; n++) {
        rec = &ai_BldsT[n];
        lo = 0;
        bt = &ai_BTypesT[rec->type];
        hi = 0;
        if ((unsigned char)bt->kind > 0x1f)
            hi = 1 << ((unsigned char)bt->kind - 0x20);
        else
            lo = 1 << (unsigned char)bt->kind;
        if (rec->owner == 0xffff || rec->owner == *ai_PlayerChar)
            continue;
        if (rec->object == 0)
            continue;
        if ((lo & MEETING_KINDS_LO) == 0 && (hi & MEETING_KINDS_HI) == 0)
            continue;
        o = (D3OBJECT *)rec->object;
        at.x = (o->pos.x);
        at.y = (o->pos.y);
        at.z = (o->pos.z);
        d.x = pos.x - at.x;
        d.y = pos.y - at.y;
        d.z = pos.z - at.z;
        owner = &ai_CharsT[rec->owner];
        dist = (float)sqrt(d.z * d.z + d.y * d.y + d.x * d.x);
        if (dist > lim)
            continue;
        for (i = 0; i < 0x300; i++) {
            ch = &ai_CharsT[i];
            if (ch->id == 0xffff || ch->kind >= 0xa || (int)(ch->netId & 3) != group)
                continue;
            if (ch->bld != 0 && ch->bld == rec)
                mode = 3;
            else if (ch->bld2 != 0 && ch->bld2 == rec)
                mode = 2;
            else if (owner == ch)
                mode = 1;
            else
                continue;
            v = he_CatchMeetingDisease(ch, (1.0f - dist / lim) * 0.5f + 0.25f, kind, &f);
            if (v != 0) {
                if (mode == 3)
                    he_CharIllness(0, ch, owner, rec, v, f);
                else if (mode == 1 && (ch->kind == 6 || ch->kind == 7)) {
                    txt_Format(text, 0x163c, v + 0x1632);
                    he_ShowNews(ch->netId, -1, text, 0, 0x5f7, ch_EmptyName);
                }
            }
        }
    }
}
/* 0x5725b0, the run handler of the meeting event 0x90: a stage -2 / -1 frees the event (and continues, as in the
   original); paused (AIJOB.opts bit 4) -> nothing; the due clock (AIJOB.end) = now; a missing building (HEMEETING.bld,
   gm_BuildQuery 0x501190) tells the client -1 (cm_SendExSetHE 0x4c4c60); a stage other than 0 does nothing; stage 0 with
   fewer than 4 rounds holds one (he_SpreadMeetingDisease 0x572310 on the radius / kind / rounds, the due clock + 0x1e
   minutes, rounds + 1, tells the client 0); else stage -1, the due clock + 0x16 hours and the client -1 */
void he_RunMeeting(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEMEETING *p = (HEMEETING *)ev;
    AIBLD *bld;

    if (e->stage == -2 || e->stage == -1)
        he_FreeEvent((int *)ev);
    if (e->opts & 4)
        return;
    e->end = ai_Time;
    bld = (AIBLD *)gm_BuildQuery(1, 1, p->bld);
    if (bld == 0) {
        cm_SendExSetHEI(-1, ev);
        return;
    }
    if (e->stage != 0)
        return;
    if (p->rounds < 4) {
        he_SpreadMeetingDisease(bld, p->radius, p->kind, p->rounds);
        gm_AddTime(&e->end, 0, 0x1e, 0);
        p->rounds = p->rounds + 1;
        cm_SendExSetHEI(0, ev);
        return;
    }
    e->stage = -1;
    gm_AddTime(&e->end, 0x16, 0, 0);
    cm_SendExSetHEI(-1, ev);
}
/* 0x5726a0, the alloc of the meeting scene event 0x91: stage 0 */
void he_AllocMeetingScene(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->stage = 0;
}
/* 0x5726b0, the run of the event 0x91 (the meeting scene object): while the partner event 0x90 (he_FindEvent) is
   active and the building (gm_BuildQuery, the event's from) has a scene object, it keeps the scene object the event
   holds (evt_FindById, its tag 0x66675073 and the building id) or loads the big / middle / small NEBEL_DES_GRAUENS
   script by the kind (evt_Load), marks it, selects the scene (d3if_SelectScene), raises the building's position 500
   (d3_PointToWorld) and runs the script with it (evt_RunScriptArgs), then keeps the script's id; without them it
   stops the old object (evt_Stop) and frees the event (he_FreeEvent) */
void he_RunMeetingScene(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEMEETSCENE *m = (HEMEETSCENE *)ev;
    AIJOB *other;
    AIBLD *bld;
    D3OBJECT *o;
    EVTSCRIPT *obj;
    D3VECTOR4 p;                        /* the engine's padded vector (the original's frame is 0x10) */
    int scene;

    other = (AIJOB *)he_FindEvent(1, 1, m->partner);
    bld = (AIBLD *)gm_BuildQuery(1, 1, e->from);
    if (bld != 0 && bld->object != 0 && other != 0 &&
        other->stage != -2 && other->stage != -1) {
        if (m->obj != -1) {
            obj = evt_FindById(m->obj);
            if (obj != 0) {
                if (((HEMEETNAME *)obj)->tag == 0x66675073 &&
                    ((HEMEETNAME *)obj)->bld == e->from)
                    return;
                m->obj = -1;
            }
        }
        if (m->kind == 2)
            obj = evt_Load("LOCATIONS\\NEKROMANTENTURM\\NEBEL_DES_GRAUENS_GROSS.esc");
        else if (m->kind == 1)
            obj = evt_Load("LOCATIONS\\NEKROMANTENTURM\\NEBEL_DES_GRAUENS_MITTEL.esc");
        else
            obj = evt_Load("LOCATIONS\\NEKROMANTENTURM\\NEBEL_DES_GRAUENS_KLEIN.esc");
        if (obj == 0)
            return;
        ((HEMEETNAME *)obj)->tag = 0x66675073;
        ((HEMEETNAME *)obj)->bld = e->from;
        scene = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        o = (D3OBJECT *)bld->object;
        d3_PointToWorld(o, &o->pos, (D3VECTOR *)&p);
        p.y += 500.0f;
        evt_RunScriptArgs(obj, 3, (int)p.x, (int)p.y, (int)p.z);
        obj->group = 0;
        d3if_SelectScene(scene, 1);
        m->obj = obj->id;
        return;
    }
    obj = evt_FindById(m->obj);
    if (obj != 0 && ((HEMEETNAME *)obj)->tag == 0x66675073 &&
        ((HEMEETNAME *)obj)->bld == e->from) {
        scene = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        evt_Stop(obj);
        d3if_SelectScene(scene, 1);
    }
    he_FreeEvent((int *)ev);
}
/* 0x572860, the alloc of the plunder offer / grave lease events 0x8b and 0x92: the due clock (AIJOB.end) = now; an
   hour (AIJOB.end.hour) at 0x17 (23:00) or later -> the due clock + 0x18 hours with the hour set to 0xc (noon), else
   the due clock + 6 hours; stage 0 */
void he_AllocPlunderOfferLease(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->end = ai_Time;
    if (e->end.hour >= 0x17) {
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = 0xc;
    } else
        gm_AddTime(&e->end, 6, 0, 0);
    e->stage = 0;
}

void ai_ShowGraveOrderExtended(int param_1, unsigned short *param_2, int param_3, int param_4, int param_5, int param_6);                 /* 0x5a97e0 */
void ai_ShowGraveOrderCancelled(int param_1, unsigned short *param_2, int param_3, int param_4);                 /* 0x5a9880 */
int gv_OrderExtendGrave(int *g, char *o, int v, int b, short s, int x);                 /* 0x4f0700 */
void he_AnswerGraveLease(int ev);          /* 0x5728d0 */
void he_ExamCancelled(int ev);          /* 0x5729c0 */

/* 0x5728d0, the answer of the grave lease offer event 0x92 (the run handler 0x572a50 calls it with the event): when
   the building (+0xac) is a graveyard (ai_BTypes kind 0x1d), it extends the lease of the character (+0xc) on it
   (gv_OrderExtendGrave: size +0xb0, entry +0xb4, years +0xbc, price +0xb8); when that character is not the building's
   owner and its kind is 6 / 7, the owner is told (ai_ShowGraveOrderExtended with the size's sale share of the price) */
void he_AnswerGraveLease(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGRAVELEASE *l = (HEGRAVELEASE *)ev;
    AIBLD *bld;
    AICHAR *per;
    int n;

    bld = (AIBLD *)gm_BuildQuery(1, 1, l->bld);
    if (bld == 0 || ((AIBTYPE *)ai_BTypes)[bld->type].kind != 0x1d)
        return;
    per = (AICHAR *)gm_FindChar(e->netId);
    if (per == 0)
        return;
    gv_OrderExtendGrave((int *)per, (char *)bld, l->size, l->entry, l->years, l->price);
    if (per == &ai_CharsT[bld->owner])
        return;
    if (ai_CharsT[bld->owner].kind != 6 && ai_CharsT[bld->owner].kind != 7)
        return;
    n = (int)((float)l->price * gv_SaleShares[l->size]);
    ai_ShowGraveOrderExtended((int)&ai_CharsT[bld->owner], (unsigned short *)per, (int)bld, l->size, l->price, n);
}
/* 0x5729c0, the master exam cancelled (the answer "no" of the exam window, its due time over): when the
   answer's building (0x501190 (1, 1, bldId)) is of the kind 0x1d (a graveyard), the grave order is freed
   (0x4f0900 (0, building, graveSize, graveEntry)) and the owner of the building is told of the character
   (0x5a9880 (the owner record, the character, the building, graveSize)) */

void he_ExamCancelled(int ev)
{
    AIJOB *e = (AIOBJ *)ev;
    AIBLD *bld = (AIBLD *)gm_BuildQuery(1, 1, e->bldId);
    AICHAR *per;

    if (bld == 0 || ai_BTypesT[bld->type].kind != 0x1d)
        return;
    gv_OrderSetGraveHolder(0, bld, e->graveSize, e->graveEntry);
    per = (AICHAR *)gm_FindChar(e->netId);
    if (per != 0)
        ai_ShowGraveOrderCancelled((int *)&ai_CharsT[bld->owner], (unsigned short *)per, (int)bld, e->graveSize);
}
/* 0x572a50, the run handler of the grave lease offer 0x92 (gv_GraveyardTurn posts it to the player whose grave lease
   runs out; its words over AIJOB's union are HEGRAVELEASE): stage -2 releases the event bar slot and frees the
   event. Stage 0 opens the offer window ("Berufe\Messagebox_Meisterpruefung") with the text 0x411c (the price) and
   the voice "_NACHRICHTEN_FRIEDHOF_07", stage 1; a window that could not be opened frees the event. Stage 1: past
   the due time the lease ends (he_ExamCancelled frees the grave); without the window back to stage 0; a click on
   its button 0 extends the lease (he_AnswerGraveLease), on button 1 refuses it; either ends the event. */
void he_RunGraveLease(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGRAVELEASE *l = (HEGRAVELEASE *)ev;
    int id;

    if (e->stage == -2) {
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    switch (e->stage) {
    case 0:
        if (e->slot == 0)
            he_OpenEventSlot(ev, 0, 0, "Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        if (e->slot == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(e->slot->win);
        d2_SetForm(e->slot->win, 0);
        io_PrintText("$C");
        io_PrintText((char *)0x411c, l->price);
        snd_QueueVoice(-7, 0, "_NACHRICHTEN_FRIEDHOF_07", -1, 0);
        e->stage = 1;
        return;
    case 1:
        if (gm_CompareTime(&ai_Time, &e->end) > 0) {
            he_ExamCancelled(ev);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (e->slot == 0) {
            e->stage = 0;
            return;
        }
        if (d2_ClickedForm != e->slot->win)
            return;
        id = d2_ClickedId;
        if (id == -1)
            return;
        if (id == 0) {
            he_AnswerGraveLease(ev);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (id == 1) {
            he_ExamCancelled(ev);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
        }
        return;
    }
}

/* 0x572b90, the alloc of the graveyard workers event 0x93: the due clock (AIJOB.end) = now, set to 6:00 (gm_SetTime
   0x4fe1e0); the second clock (AIJOB.clock60) = the due clock */
void he_AllocGraveyardWorkers(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->end = ai_Time;
    gm_SetTime(&e->end, 6, 0, 0);
    e->clock60 = e->end;
}

void ai_EndCharExtra(char *o, int f); /* 0x587c40 */
int ls_GetLanguage();                 /* 0x5190a0 */

/* 0x572bf0, the run handler of the graveyard workers' event 0x93 (alloc 0x572b90, first due at 6:00), a four-stage
   cycle over every graveyard building (gm_BuildQuery (1, 5, 0x1d)): -2 / -1 free the event. Stage 0, for each owned
   graveyard: its workers (the characters whose bld is it) are tested against 0.75 + 0.0025 * the building's skill
   0x15; one above it whose job is 4 / 0x8c / 0x8d and who is not player-led (flags1b4 bit 0) ends that job
   (ai_EndCharExtra (.., 1)); one below it and above 0.5, without a job, whose owner's opinion of him beats
   -random(0x4a), gets the message 0x94 (to the event's character, from the building, arg[0] his netId). Stage 1,
   for each graveyard: its player-led workers of the job 0 / 4 / 0x8c / 0x8d other than the master (gm_FindCharacterByMaster)
   end their job (ai_EndCharExtra (.., 0)) above 0.75, or below it when the master is player-led and the worker has a
   model. Both stages are due again in 20 + random(20) minutes, the stages 2 and 3 in 30 minutes (3 back to 0).
   While a graveyard has no owner (0xffff) stage 0 never takes the next one: the original's loop is written so. */
void he_RunGraveyardWorkers(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *chars[0x20];
    AIBLD *bld;
    AICHAR *ch, *master;
    float r, chance;
    int i, n, v;

    switch (e->stage) {
    case 0:
        bld = (AIBLD *)gm_BuildQuery(1, 5, 0x1d);
        while (bld != 0) {
            if (bld->owner != 0xffff) {
                chance = (float)gm_BuildingSkill(bld, 0x15, 1) * 0.0025f;
                n = 0;
                for (ch = ai_CharsT; (int)ch < (int)&ai_CharsT[768]; ch++) {
                    if (ch->id != 0xffff && ch->bld == bld)
                        chars[n++] = ch;
                }
                i = 0;
                if (n > 0) {
                    AIMSG msg;

                    chance += 0.75f;
                    while (i < n) {
                        r = util_RandomF();
                        if (r > chance) {
                            ch = chars[i];
                            if (!(ch->flags1b4 & 1) && (ch->jobKind == 4 || ch->jobKind == 0x8c || ch->jobKind == 0x8d))
                                ai_EndCharExtra((char *)ch, 1);
                        } else if (r > 0.5) {
                            ch = chars[i];
                            if (ch->job == 0) {
                                v = gm_Opinion(bld->owner, ch->id);
                                if (-(util_Random(0x4a) & 0xffff) < v) {
                                    memset(&msg, 0, sizeof(msg));
                                    msg.type = 0x94;
                                    msg.to = e->chr;
                                    msg.fromId = bld->id;
                                    msg.time = ai_Time;
                                    msg.flags = 1;
                                    msg.arg[0] = ch->netId;
                                    cm_SendLongMessage((char *)&msg);
                                }
                            }
                        }
                        i++;
                    }
                }
                bld = (AIBLD *)gm_BuildNext();
            }
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 0x14 + (util_Random(0x14) & 0xffff), 0);
        e->stage++;
        break;
    case 1:
        for (bld = (AIBLD *)gm_BuildQuery(1, 5, 0x1d); bld != 0; bld = (AIBLD *)gm_BuildNext()) {
            n = 0;
            master = (AICHAR *)gm_FindCharacterByMaster(bld);
            for (ch = ai_CharsT; (int)ch < (int)&ai_CharsT[768]; ch++) {
                if (ch->id != 0xffff && ch->bld == bld)
                    chars[n++] = ch;
            }
            gm_BuildingSkill(bld, 0x15, 1);
            for (i = 0; i < n; i++) {
                ch = chars[i];
                if (!(ch->flags1b4 & 1))
                    continue;
                if (ch->jobKind != 4 && ch->jobKind != 0x8c && ch->jobKind != 0x8d && ch->jobKind != 0)
                    continue;
                if (master != 0 && master == ch)
                    continue;
                if (util_RandomF() > 0.75f || (master != 0 && (master->flags1b4 & 1) && ch->model != 0))
                    ai_EndCharExtra((char *)ch, 0);
            }
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 0x14 + (util_Random(0x14) & 0xffff), 0);
        e->stage++;
        break;
    case 2:
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 0x1e, 0);
        e->stage++;
        break;
    case 3:
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 0x1e, 0);
        e->stage = 0;
        break;
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        break;
    }
}
/* 0x572fd0, the run handler of the graveyard worker's theft 0x94 (he_RunGraveyardWorkers posts its message for a
   worker without a job whose owner's opinion beat a random draw; arg[0], here HEGRAVETHEFT.worker, is his netId):
   -2 / -1 free the event. Stage 0: a missing worker or one without a building ends it, and so does an owner's
   opinion of him above -26. Else one object of the building's store (the object 0x2a, else 0x116 in its object
   list) is picked at random and sold away (cm_SendExSellObject (-1, the store, its good, 1, ai_OwnPlayer, 0)), the
   player of the event is told (the text 0x414b with 0x229, the building and the good's name 0x9a8 + 2 * good, their
   order by the language; the voice "_NACHRICHTEN_FRIEDHOF_06", the quick jump to the building's main object), and
   the event is freed. "Theft" is a reasoned guess from the sale to nobody and the news. */
void he_RunGraveyardTheft(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGRAVETHEFT *t = (HEGRAVETHEFT *)ev;
    AICHAR *per;
    AIBLD *bld;
    AIOBJ *store, *main, *obj;
    int n;
    AIOBJ *objs[0x20];
    char text[0x400];

    switch (e->stage) {
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        return;
    case 0:
        break;
    default:
        return;
    }
    per = (AICHAR *)gm_FindChar(t->worker);
    if (per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    bld = per->bld;
    if (bld == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_Opinion(bld->owner, per->id) > -0x1a) {
        he_FreeEvent((int *)ev);
        return;
    }
    store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
    if (store == 0)
        store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x116);
    main = (AIOBJ *)gm_BuildingMainObject((unsigned char *)bld);
    if (store == 0)
        goto done;
    n = 0;
    memset(objs, 0, sizeof objs);
    obj = (AIOBJ *)gm_Query((void *)store->list, 1, 5);
    while (obj != 0) {
        objs[n++] = obj;
        obj = (AIOBJ *)gm_QueryNext();
    }
    if (n == 0)
        goto done;
    obj = objs[util_Random(n) & 0xffff];
    if (obj == 0)
        goto done;
    gm_GoodsPrice(obj->id, ai_OwnPlayer);
    cm_SendExSellObject(-1, store->objId, obj->id, 1, ai_OwnPlayer, 0);
    switch (ls_GetLanguage()) {
    case 0:
    case 2:
        txt_Format(text, 0x414b, 0x229, per->bld, obj->id * 2 + 0x9a8);
        break;
    case 1:
        txt_Format(text, 0x414b, 0x229, obj->id * 2 + 0x9a8, per->bld);
        break;
    }
    he_MessageBoxQuickjump(ai_CharInts[e->chr][0], -1, text, 0, 0x5f2, bld->id, main->objId, 0, "_NACHRICHTEN_FRIEDHOF_06");
done:
    he_FreeEvent((int *)ev);
}
/* 0x573200, 1 when a graveyard work is pending on a grave: an event of the type 0x8d (the work ai_StartBuildingWork
   posts) of the building bld (its id, the event's search key) whose grave care words (HEGRAVECARE.size / entry)
   are the grave's size (list) and its index in that list */
int he_HasGraveWork(AIBLD *bld, int size, int index)
{
    HEGRAVECARE *g;
    char *ev;

    ev = he_FindEvent(2, 3, bld->id, 0, 0x8d);
    while (ev != 0) {
        g = (HEGRAVECARE *)ev;
        if (g->size == size && g->entry == index)
            return 1;
        ev = he_NextEvent();
    }
    return 0;
}

void he_AllocCouncil(int ev);          /* 0x573280 */
void he_RunCouncil(int ev);          /* 0x5732d0 */

/* 0x573260, registers the event type 0xa2 (called by 0x53d4b0): the pair (0x573280, 0x5732d0); 0 = ok, 1 = a registration failed */
int he_RegisterEventTypeA2(void)
{
    return he_SetEventHandlers(0xa2, (void *)he_AllocCouncil, (void *)he_RunCouncil) != 0;
}
/* 0x573280, the alloc of the event 0xa2 (the daily Hanseatic League politics): nothing while paused (AIJOB.opts & 4) or
   while the hour (ai_Time.hour) is 0xa or less; else the due clock AIJOB.end = now + 24 hours (0x4fe140), its hour 0xa,
   minute and second 0, stage 0 */

void he_AllocCouncil(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    if (e->opts & 4)
        return;
    if (ai_Time.hour <= 0xa)
        return;
    gm_AddTime(&e->end, 0x18, 0, 0);
    e->end.hour = 0xa;
    e->end.min = 0;
    e->end.sec = 0;
    e->stage = 0;
}

void hmt_Update();                 /* 0x4ace40, hmt_Update */
void hmt_Begin();                 /* 0x4ad0a0, hmt_Begin */
void hmt_Election();                 /* 0x4ad1c0, hmt_Election */
void hmt_ClearOffices(void); /* 0x4ad510 */
void hmt_DecideElection();                 /* 0x4ad580 */

/* 0x5732d0, the run of the event 0xa2 (the daily Hanseatic League politics): nothing while paused (AIJOB.opts & 4); the
   stage 0 .. 4 picks the step, each schedules the next state with 0x4c4c60: 0 hmt_Update and +2 hours, 1 hmt_Begin and
   +2 hours, 2 hmt_Election and the time of day 0x16 (0x4fe1e0), 3 hmt_ClearOffices and the time of day 6:05 with the day +1,
   4 hmt_DecideElection and the time of day 0xa */
void he_RunCouncil(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    if (e->opts & 4)
        return;
    switch (e->stage) {
    case 0:
        hmt_Update();
        gm_AddTime(&e->end, 2, 0, 0);
        cm_SendExSetHEI(1, ev);
        return;
    case 1:
        hmt_Begin();
        gm_AddTime(&e->end, 2, 0, 0);
        cm_SendExSetHEI(2, ev);
        return;
    case 2:
        hmt_Election();
        gm_SetTime(&e->end, 0x16, 0, 0);
        cm_SendExSetHEI(3, ev);
        return;
    case 3:
        hmt_ClearOffices();
        gm_SetTime(&e->end, 6, 5, 0);
        e->end.year = e->end.year + 1;
        cm_SendExSetHEI(4, ev);
        return;
    case 4:
        hmt_DecideElection();
        gm_SetTime(&e->end, 0xa, 0, 0);
        cm_SendExSetHEI(0, ev);
        return;
    }
}

void he_AllocGypsyApproach(int ev);          /* 0x5734b0 */
void he_RunGypsyApproach(int ev);          /* 0x573580 */
void he_AllocGypsyCurse(int ev);          /* 0x573f20 */
void he_RunGypsyCurse(int ev);          /* 0x573fb0 */
void he_AllocGypsySlowdown(int ev);          /* 0x574060 */
void he_RunGypsySlowdown(int ev);          /* 0x5740d0 */
void he_AllocTrialScene(int ev);          /* 0x574120 */
void he_RunTrialScene(int ev);          /* 0x5743c0 */
void he_AllocMeeting4(int ev);          /* 0x574550 */
void he_RunCircusShow(int ev);          /* 0x5748d0 */
void he_AllocPettyLoss(int ev);          /* 0x575150 */
void he_RunPettyLoss(int ev);          /* 0x5751e0 */
void he_AllocEventGood(int ev);          /* 0x575320 */
void he_RunEventGood(int ev);          /* 0x5753a0 */

/* 0x5733c0, registers the event types 0x9d .. 0xa1, 0xa3 and 0xa4 (called by 0x53d4b0): the pairs
   (he_AllocGypsyApproach, he_RunGypsyApproach), (he_AllocGypsyCurse, he_RunGypsyCurse),
   (he_AllocGypsySlowdown, he_RunGypsySlowdown), (he_AllocTrialScene, he_RunTrialScene),
   (he_AllocMeeting4, he_RunCircusShow), (he_AllocPettyLoss, he_RunPettyLoss) and (he_AllocEventGood, he_RunEventGood),
   then the type text 0x50f of 0xa1 (0x52b350); 0 = ok, 1 = a registration failed */
int he_RegisterEventTypes9d(void)
{
    if (he_SetEventHandlers(0x9d, (void *)he_AllocGypsyApproach, (void *)he_RunGypsyApproach) == 0) {
        if (he_SetEventHandlers(0x9e, (void *)he_AllocGypsyCurse, (void *)he_RunGypsyCurse) == 0) {
            if (he_SetEventHandlers(0x9f, (void *)he_AllocGypsySlowdown, (void *)he_RunGypsySlowdown) == 0) {
                if (he_SetEventHandlers(0xa0, (void *)he_AllocTrialScene, (void *)he_RunTrialScene) == 0) {
                    if (he_SetEventHandlers(0xa1, (void *)he_AllocMeeting4, (void *)he_RunCircusShow) == 0) {
                        if (he_SetEventHandlers(0xa3, (void *)he_AllocPettyLoss, (void *)he_RunPettyLoss) == 0) {
                            if (he_SetEventHandlers(0xa4, (void *)he_AllocEventGood, (void *)he_RunEventGood) == 0) {
                                sv_RegisterActivity(0xa1, 0, 0, 0x50f, -1);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}
/* 0x5734b0, the alloc of the gypsy event 0x9d: stage 0; nothing while paused (AIJOB.opts & 4); both clocks AIJOB.start /
   AIJOB.end = now; the gypsy figure HEGYPSY.figure (gm_FindChar): without one the event is scheduled with -1; else gv_Accuse
   (0x1c, 1, the player's netId AIJOB.netId, the figure's netId, the id of the figure's building AIBLD.bld2 ? its id : -1) and
   the handler scheduled with 0 */

void he_AllocGypsyApproach(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGYPSY *g = (HEGYPSY *)ev;
    AICHAR *per;
    AIBLD *b;
    int x;

    e->stage = 0;
    if (e->opts & 4)
        return;
    e->start = ai_Time;
    e->end = ai_Time;
    per = (AICHAR *)gm_FindChar(g->figure);
    if (per == 0) {
        e->exSlot = cm_SendExSetHEI(-1, ev);
        return;
    }
    b = per->bld2;
    x = b != 0 ? b->id : -1;
    gv_Accuse(0x1c, 1, e->netId, per->netId, x);
    e->exSlot = cm_SendExSetHEI(0, ev);
}

void he_GypsyApproach(char *pl, char *per, int a);  /* 0x5736d0 */
void he_GypsyFee(AICHAR *pl, int a);           /* 0x573e30 */

/* 0x573580, the run of the event 0x9d: -2 / -1 free; nothing while paused or while the pending handle AIJOB.exSlot is not done
   (0x4c3ae0); then exSlot = -1, the due clock AIJOB.end = now, the figure HEGYPSY.figure (0x505df0) and the player record
   ai_Chars[e->chr] (0x770b80) must exist (else scheduled with -1): stage 0 calls he_GypsyApproach (player, figure, the
   approach) and retries in 24 hours with the hour 6 and the minute 15 * random (4) + 5; stage 1 calls he_GypsyFee (player,
   the approach) and schedules -1 */
void he_RunGypsyApproach(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGYPSY *g = (HEGYPSY *)ev;
    AICHAR *per, *pl;

    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->opts & 4)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    e->end = ai_Time;
    per = (AICHAR *)gm_FindChar(g->figure);
    if (per == 0 || ai_CharsT[e->chr].id == 0xffff) {
        e->exSlot = cm_SendExSetHEI(-1, ev);
        return;
    }
    pl = &ai_CharsT[e->chr];
    switch (e->stage) {
    case 0:
        he_GypsyApproach((char *)pl, (char *)per, g->approach);
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = 6;
        e->end.min = (util_Random(4) & 0xffff) * 0xf + 5;
        e->exSlot = cm_SendExSetHEI(1, ev);
        return;
    case 1:
        he_GypsyFee(pl, g->approach);
        e->exSlot = cm_SendExSetHEI(-1, ev);
        return;
    }
}

void he_GypsyDisease(AICHAR *pl, AICHAR *per);     /* 0x573760 */
void he_GypsyRumour(AICHAR *pl, AICHAR *per);     /* 0x573820 */
void he_GypsyWorkerQuits(AICHAR *pl, AICHAR *per);     /* 0x573970 */
void he_GypsySwapSkills(AICHAR *pl, AICHAR *per);     /* 0x573ae0 */
void he_GypsyCurse(AICHAR *pl, AICHAR *per);     /* 0x573c20 */
void he_GypsySlowdown(AICHAR *pl, AICHAR *per);     /* 0x573d30 */

/* 0x5736d0, dispatches the approach +0xac `a` of the event 0x9d: 0 0x573760, 1 0x573820, 2 0x573970, 3 0x573ae0, 4 0x573c20, else 0x573d30 */
void he_GypsyApproach(char *pl, char *per, int a)
{
    if (a == 0)
        he_GypsyDisease((AICHAR *)pl, (AICHAR *)per);
    else if (a == 1)
        he_GypsyRumour((AICHAR *)pl, (AICHAR *)per);
    else if (a == 2)
        he_GypsyWorkerQuits((AICHAR *)pl, (AICHAR *)per);
    else if (a == 3)
        he_GypsySwapSkills((AICHAR *)pl, (AICHAR *)per);
    else if (a == 4)
        he_GypsyCurse((AICHAR *)pl, (AICHAR *)per);
    else
        he_GypsySlowdown((AICHAR *)pl, (AICHAR *)per);
}
/* 0x573760, the approach 0 of the event 0x9d (he_GypsyApproach): the character per catches a disease (gm_CatchDisease
   draws its kind and health; none caught: nothing), and per's player is told (the news 0x5fb, the text 0x41dd when
   per is the event's own character pl, else 0x41dc, around 0x41de with the disease's name 0x1632 + kind) with the
   voice "_NACHRICHTEN_ZIGEUNER_02" (pl) or "_NACHRICHTEN_ZIGEUNER_01" (another) */
void he_GypsyDisease(AICHAR *pl, AICHAR *per)
{
    int kind, health;
    char voice[100];
    char what[0x400];
    char text[0x400];
    int fmt;

    if (gm_CatchDisease(per, &kind, &health) == 0)
        return;
    if (per == pl) {
        fmt = 0x41dd;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_02");
    } else {
        fmt = 0x41dc;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_01");
    }
    txt_Format(what, 0x41de, kind + 0x1632);
    txt_Format(text, fmt, what);
    he_ShowNews(per->netId, -1, text, 1, 0x5fb, voice);
}
/* 0x573820, the approach 1 of the event 0x9d (he_GypsyApproach): the standing of the character per changes: when he
   holds an office (amt_GetKind of officeKind, its column col other than 0: the district), its opinion of him falls by
   20 + random(16) (amt_ChangeDistrictOpinion); else, when he is not of the kinds 5 / 6 / 7, the opinion of a random
   district 1 + random(3) rises by 20 + random(16); else every other figure of the kinds 5 .. 7 thinks worse of him
   by 20 + random(16) (cm_SendRelations). Then per's player is told (the news 0x5fb, the text 0x41dd when per is the
   event's own character pl, else 0x41dc, around 0x41df). "Rumour" is a reasoned guess from that effect. */
void he_GypsyRumour(AICHAR *pl, AICHAR *per)
{
    AMTKIND kind;
    char what[0x400];
    char text[0x400];
    AICHAR *ch;
    int i, delta, district, fmt;

    if (amt_GetKind(per->officeKind, &kind) != 0 && (district = kind.col) != 0) {
        amt_ChangeDistrictOpinion(per, district, -0x14 - (util_Random(0x10) & 0xffff));
    } else if (per->kind != 6 && per->kind != 7 && per->kind != 5) {
        amt_ChangeDistrictOpinion(per, 1 + (util_Random(3) & 0xffff), 0x14 + (util_Random(0x10) & 0xffff));
    } else {
        delta = -0x14 - (util_Random(0x10) & 0xffff);
        for (i = 0, ch = ai_CharsT; (int)&ch->kind < (int)&ai_CharsT[768].kind; ch++, i++) {
            if (i == per->id)
                continue;
            if (ch->kind == 6 || ch->kind == 7 || ch->kind == 5)
                cm_SendRelations(per->netId, ch->netId, delta, 1);
        }
    }
    if (per == pl)
        fmt = 0x41dd;
    else
        fmt = 0x41dc;
    txt_Format(what, 0x41df);
    txt_Format(text, fmt, what);
    he_ShowNews(per->netId, -1, text, 1, 0x5fb, 0);
}
/* 0x573970, the approach 2 of the event 0x9d (he_GypsyApproach): the first worker (kind 0, a building, no jobId) of a
   building per owns is made to quit: the message 0x33 (the worker who quits, its run handler 0x55e1d0) to per from
   that building, arg[0] the worker's netId. Then per's player is told (the news 0x5fb, the text 0x41dd when per is
   the event's own character pl, else 0x41dc, around 0x41e0 with the worker and the building) with the voice
   "_NACHRICHTEN_ZIGEUNER_02" (pl) or "_NACHRICHTEN_ZIGEUNER_01" (another). No such worker: nothing. */
void he_GypsyWorkerQuits(AICHAR *pl, AICHAR *per)
{
    AIMSG msg;
    char voice[100];
    char what[0x400];
    char text[0x400];
    AICHAR *ch;
    int i, fmt;

    ch = 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_CharsT[i].kind == 0 && ai_CharsT[i].bld != 0 && ai_CharsT[i].jobId == 0
            && ai_CharsT[i].bld->owner == per->id) {
            ch = &ai_CharsT[i];
            break;
        }
    }
    if (ch == 0)
        return;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x33;
    msg.to = per->netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_Time;
    msg.flags = 1;
    msg.arg[0] = ch->netId;
    cm_SendLongMessage((char *)&msg);
    if (per == pl) {
        fmt = 0x41dd;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_02");
    } else {
        fmt = 0x41dc;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_01");
    }
    txt_Format(what, 0x41e0, ch->id, ch->bld);
    txt_Format(text, fmt, what);
    he_ShowNews(per->netId, -1, text, 1, 0x5fb, voice);
}
/* 0x573ae0, the approach 3 of the event 0x9d (he_GypsyApproach): of per's five skills the highest and the lowest (the
   first of each) trade places: the lowest gains their difference, the highest loses it (cm_SendAddSkill); five
   equal skills: nothing. Then per's player is told (the news 0x5fb, the text 0x41dd when per is the event's own
   character pl, else 0x41dc, around 0x41e1 with both skills' names 0x16b7 + index) with the voice
   "_NACHRICHTEN_ZIGEUNER_02" (pl) or "_NACHRICHTEN_ZIGEUNER_01" (another). */
void he_GypsySwapSkills(AICHAR *pl, AICHAR *per)
{
    char voice[100];
    char what[0x400];
    char text[0x400];
    int hi, lo, hiIdx, loIdx, i, fmt;

    lo = 0xff;
    hi = -1;
    hiIdx = -1;
    loIdx = -1;
    for (i = 0; i < 5; i++) {
        if (per->skills[i] > hi) {
            hi = per->skills[i];
            hiIdx = i;
        }
        if (per->skills[i] < lo) {
            lo = per->skills[i];
            loIdx = i;
        }
    }
    if (loIdx == hiIdx || loIdx <= -1 || hiIdx <= -1)
        return;
    cm_SendAddSkill(per->netId, loIdx, hi - lo);
    cm_SendAddSkill(per->netId, hiIdx, lo - hi);
    if (per == pl) {
        fmt = 0x41dd;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_02");
    } else {
        fmt = 0x41dc;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_01");
    }
    txt_Format(what, 0x41e1, loIdx + 0x16b7, hiIdx + 0x16b7);
    txt_Format(text, fmt, what);
    he_ShowNews(per->netId, -1, text, 1, 0x5fb, voice);
}
/* 0x573c20, the approach 4 of the event 0x9d (he_GypsyApproach): posts the message 0x9e to per (from -1; arg[0] the
   netId of the event's own character pl, -1 when that is per; arg[1] per's netId), whose event (alloc 0x573f20, run
   0x573fb0) charges per once a day for two days. Then per's player is told (the news 0x5fb, the text 0x41dd when per
   is pl, else 0x41dc, around 0x41e2) with the voice "_NACHRICHTEN_ZIGEUNER_02" (pl) or "_NACHRICHTEN_ZIGEUNER_01"
   (another). "Curse" is a reasoned guess from that effect. */
void he_GypsyCurse(AICHAR *pl, AICHAR *per)
{
    AIMSG msg;
    char voice[100];
    char what[0x400];
    char text[0x400];
    int fmt;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x9e;
    msg.to = per->netId;
    msg.fromId = -1;
    msg.time = ai_Time;
    msg.flags = 1;
    msg.arg[0] = pl == per ? -1 : pl->netId;
    msg.arg[1] = per->netId;
    cm_SendLongMessage((char *)&msg);
    if (per == pl) {
        fmt = 0x41dd;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_02");
    } else {
        fmt = 0x41dc;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_01");
    }
    txt_Format(what, 0x41e2);
    txt_Format(text, fmt, what);
    he_ShowNews(per->netId, -1, text, 1, 0x5fb, voice);
}
/* 0x573d30, the default approach (5 and up) of the event 0x9d (he_GypsyApproach): posts the message 0x9f to per (from
   -1), whose event (alloc 0x574060, run 0x5740d0) lasts 24 hours; while it exists per's workshops work at 0.9 of
   their rate (he_HasEvent9f). Then per's player is told (the news 0x5fb, the text 0x41dd when per is the event's own
   character pl, else 0x41dc, around 0x41e3) with the voice "_NACHRICHTEN_ZIGEUNER_02" (pl) or
   "_NACHRICHTEN_ZIGEUNER_01" (another). "Slowdown" is a reasoned guess from that effect. */
void he_GypsySlowdown(AICHAR *pl, AICHAR *per)
{
    AIMSG msg;
    char voice[100];
    char what[0x400];
    char text[0x400];
    int fmt;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x9f;
    msg.to = per->netId;
    msg.fromId = -1;
    msg.time = ai_Time;
    msg.flags = 1;
    cm_SendLongMessage((char *)&msg);
    if (per == pl) {
        fmt = 0x41dd;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_02");
    } else {
        fmt = 0x41dc;
        strcpy(voice, "_NACHRICHTEN_ZIGEUNER_01");
    }
    txt_Format(what, 0x41e3);
    txt_Format(text, fmt, what);
    he_ShowNews(per->netId, -1, text, 1, 0x5fb, voice);
}


/* 0x573e30, the second step of the event 0x9d (state 1 of he_RunGypsyApproach): the event's own character pl pays the fee n
   for the approach a (a < 2: 2 + random(2); a == 2: 3 + random(2); a == 3: 3 + random(3); a == 4: 3 + random(4); else
   4 + random(4)): with the money he is charged n (gm_SendMoneyDelta) and told (the news 0x5fa, the text 0x41db with n)
   with the voice "_NACHRICHTEN_ZIGEUNER_00"; without it the approach a strikes himself (he_GypsyApproach (pl, pl, a)).
   "Fee" is a reasoned guess from that effect. */
void he_GypsyFee(AICHAR *pl, int a)
{
    char text[0x200];
    int n;

    if (a < 2)
        n = util_RandomU(2) + 2;
    else if (a == 2)
        n = util_RandomU(2) + 3;
    else if (a == 3)
        n = util_RandomU(3) + 3;
    else if (a == 4)
        n = util_RandomU(4) + 3;
    else
        n = util_RandomU(4) + 4;
    if (pl->money >= n) {
        gm_SendMoneyDelta(pl->netId, -n);
        txt_Format(text, 0x41db, n);
        he_ShowNews(pl->netId, -1, text, 1, 0x5fa, "_NACHRICHTEN_ZIGEUNER_00");
    } else {
        he_GypsyApproach((char *)pl, (char *)pl, a);
    }
}
/* 0x573f20, the alloc of the gypsy curse event 0x9e: stage 0, the start and the due clock AIJOB.start / AIJOB.end = now, the
   due clock + 24 hours (0x4fe140), the hour 6 and the minute 5 + random (0x10) */

void he_AllocGypsyCurse(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->stage = 0;
    e->start = ai_Time;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0x18, 0, 0);
    e->end.hour = 6;
    e->end.min = (util_Random(0x10) & 0xffff) + 5;
}
/* 0x573fb0, the run of the event 0x9e: -2 / -1 free; the stage + 1; the cursed character +0xb0 and the gypsy +0xac
   (gm_FindChar); the cursed one's money +0x194 (n) is taken -n and the gypsy gets n / 2 (at least 2) (gm_SendMoneyDelta);
   while the stage stays below 2 the due clock +0x52 moves 24 hours ahead (gm_AddTime), else the event is freed */

void he_RunGypsyCurse(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGYPSYCURSE *g = (HEGYPSYCURSE *)ev;
    AICHAR *a, *b;
    int n, x;

    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    e->stage = e->stage + 1;
    a = (AICHAR *)gm_FindChar(g->cursed);
    b = (AICHAR *)gm_FindChar(g->gypsy);
    if (a == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    n = a->money;
    if (n != 0) {
        gm_SendMoneyDelta(a->netId, -n);
        x = n / 2;
        if (x <= 2)
            x = 2;
        if (b != 0)
            gm_SendMoneyDelta(b->netId, x);
    }
    if (e->stage < 2) {
        gm_AddTime(&e->end, 0x18, 0, 0);
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x574060, the alloc of the event 0x9f: stage -1, start and end = now, the end clock + 24 hours (gm_AddTime) */

void he_AllocGypsySlowdown(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->stage = -1;
    e->start = ai_Time;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0x18, 0, 0);
}
/* 0x5740d0, the run of the event 0x9f: the stage -2 / -1 (a free record) frees the event */

void he_RunGypsySlowdown(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    if (e->stage == -1 || e->stage == -2)
        he_FreeEvent((int *)ev);
}
/* 0x5740f0, 1 when the character `a` has an event of the type 0x9f (he_FindEvent (2, 0, 0x9f, 2, a->id)) */

int he_HasEvent9f(AICHAR *a)
{
    return he_FindEvent(2, 0, 0x9f, 2, a->id) != 0;
}
/* 0x574120, the alloc of the event 0xa0: nothing while paused (opts & 4); start and end = now; when gm_AllowAll is set
   the end moves 1 hour ahead, else 24 hours with the hour 0x13 + random (2); the minute 0 and the handler scheduled with 0 */

void he_AllocTrialScene(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    if (e->opts & 4)
        return;
    e->start = ai_Time;
    e->end = ai_Time;
    if (gm_AllowAll != 0) {
        gm_AddTime(&e->end, 1, 0, 0);
    } else {
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = util_Random(2) + 0x13;
    }
    e->end.min = 0;
    e->exSlot = cm_SendExSetHEI(0, ev);
}


/* 0x5741d0, the curse event 0xa0 the building `a` sent (he_FindEvent (2, 0, 0xa0, 3, a->id): its from id), 0 when none */

char *ai_FindCurseEvent(char *a)
{
    return he_FindEvent(2, 0, 0xa0, 3, ((AIBLD *)a)->id);
}
/* 0x5741f0, the trial payment of the event 0xa0 (state 1 of he_RunTrialScene): the building from (gm_FindById) and the
   character netId (gm_FindChar) must exist. By the building's trial kind (gm_TrialBuildingKind) and the trial's level
   the bar (the liking a figure needs) and a bonus count are: kind 3: 40 + 20 / 10 / 0 and 10 / 20 / 25 for a level
   below 4 / below 7 / else; kind 2: 50 + 10 / 0 and 10 / 15 for a level below 4 / else; else 70 and 10. The fee per
   head is 32 * (15 + random(24)) / 32 * (12 + random(16)) / 32 * (10 + random(8)). Every other figure (kind below
   10) whose liking from the character (gm_Liking) is above the bar adds a head to the bonus; the character is told
   (the news 0x5fa, the text 0x41fa with the building, the heads and the sum) and gets heads * fee from the own player
   (cm_SendMoveGoodsUnchecked). */
void he_TrialPayment(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HETRIALCASE *t = (HETRIALCASE *)ev;
    char text[0x200];
    char *bld;
    AICHAR *per, *ch;
    int kind, add, bonus, cnt, fee, sum, i;
    float bar;

    gm_FindById(&bld, 0, 0, e->from);
    if (bld == 0)
        return;
    per = (AICHAR *)gm_FindChar(e->netId);
    if (per == 0)
        return;
    cnt = 0;
    kind = gm_TrialBuildingKind(bld);
    if (kind == 3) {
        add = 0;
        bonus = 0;
        if (t->level < 4) {
            add = 0x14;
            bonus = -0xf;
        } else if (t->level < 7) {
            add = 0xa;
            bonus = -5;
        }
        bar = add + 40.0f;
        bonus += 0x19;
        fee = util_RandomU(0x18) + 0xf;
    } else if (kind == 2) {
        add = 0;
        bonus = 0;
        if (t->level < 4) {
            add = 0xa;
            bonus = -5;
        }
        bar = add + 50.0f;
        bonus += 0xf;
        fee = util_RandomU(0x10) + 0xc;
    } else {
        bar = 70.0f;
        bonus = 0xa;
        fee = util_RandomU(8) + 0xa;
    }
    fee <<= 5;
    for (i = 0; i < 0x300; i++) {
        ch = &ai_CharsT[i];
        if (ch->id == 0xffff || ch == per || ch->kind >= 0xa)
            continue;
        if (gm_Liking(per->id, ch->id, 1) > bar)
            cnt++;
    }
    cnt += bonus;
    sum = cnt * fee;
    txt_Format(text, 0x41fa, bld, cnt, sum);
    he_ShowNews(per->netId, -1, text, 1, 0x5fa, 0);
    cm_SendMoveGoodsUnchecked(per->netId, -1, ai_OwnPlayer, sum);
}
/* 0x5743c0, the run of the trial event 0xa0: it waits on the pending handle (cm_GetSlotState) and frees on the states
   -1 / -2; state 0 finds the building (gm_FindBuildingById, its from) and its group object (gm_Query, the prototype
   0x27f), puts the due clock 2 hours ahead (gm_AddTime), and with a group shows the text 0x4205 (txt_Format,
   he_MessageBoxQuickjump with the owner's character and the group's id), selects the group's scene
   (init_ObjectSceneUniverse, d3if_SelectScene), loads the theatre script (evt_Load) as the owner's own scene and runs it
   with the trial's level, then restores the scene; it clears the due minute and goes to state 1; state 1 runs the trial
   payment (he_TrialPayment) unless paused (opts 4) and schedules the next run with cm_SendExSetHE */
void he_RunTrialScene(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HETRIALCASE *t = (HETRIALCASE *)ev;
    char text[0x400];
    AIBLD *bld;
    AIOBJ *grp;
    AICHAR *ch;
    EVTSCRIPT *o;
    int sc, sc2;

    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage == 0) {
        grp = 0;
        bld = (AIBLD *)gm_FindBuildingById(e->from);
        if (bld != 0)
            grp = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0x27f);
        gm_AddTime(&e->end, 2, 0, 0);
        if (grp != 0) {
            ch = &ai_CharsT[bld->owner];
            txt_Format(text, 0x4205, bld);
            he_MessageBoxQuickjump(ch->netId, ch->netId, text, 0, 0x5fc, bld->id, grp->objId, ch_EmptyName, ch_EmptyName);
            sc = d3_CurrentScene;
            sc2 = init_ObjectSceneUniverse((unsigned short *)grp);
            d3if_SelectScene(sc2, 1);
            o = evt_Load("locations/zirkuszelt/theaterauffuehrung.esc");
            if (o != 0) {
                o->group = grp->objId;
                o->ownScene = 1;
                evt_RunScriptArgs(o, 1, t->level);
            }
            d3if_SelectScene(sc, 1);
        }
        e->end.min = 0;
        e->stage = 1;
        return;
    }
    if (e->stage == 1 && !(e->opts & 4)) {
        he_TrialPayment(ev);
        e->exSlot = cm_SendExSetHEI(-1, ev);
    }
}
/* 0x574550, the alloc of the event 0xa1 (a meeting of up to four figures +0x8c ..): when one of them is in the building +0x10
   (0x505df0, +0x16c, +1) the start and the due clock +0x44 / +0x52 = now, two indices of the table 7 / 0x16 / 0xf / 0xa
   (0x505ca0 (4, out)) go to +0xc0 / +0xc2 and +0xc4 / +0xc6, the words +0xc8 .. +0xce = 0, +0xac = 0, +0xb0 / +0xb4 = 0xf0,
   +0xbc = 0, +0xd0 = -1 and state 0; else the event is freed */

void he_AllocMeeting4(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEMEET4 *m = (HEMEET4 *)ev;
    unsigned short w[4];
    int out[4];
    AICHAR *per;
    AIBLD *bld;
    int i;

    w[0] = 7;
    w[1] = 0x16;
    w[2] = 0xf;
    w[3] = 0xa;
    for (i = 0; i < 4; i++) {
        per = (AICHAR *)gm_FindChar(e->ids[i]);
        if (per == 0)
            continue;
        bld = per->bld;
        if (bld == 0)
            continue;
        if (bld->id == e->from)
            goto done;
    }
    he_FreeEvent((int *)ev);
    return;
done:                               /* no contact object: -1 */
    e->start = ai_Time;
    e->end = ai_Time;
    gm_Shuffled(4, out);
    m->objs[3] = 0;
    m->classes[0] = w[out[0]];
    m->classes[1] = w[out[1]];
    m->objs[2] = 0;
    m->classes[2] = w[out[2]];
    m->classes[3] = w[out[3]];
    m->objs[1] = 0;
    m->objs[0] = 0;
    m->income = 0;
    m->room = 0xf0;
    m->stop = 0xf0;
    m->meetStart = 0;
    m->bld = -1;
    e->stage = 0;
}
/* 0x5746b0, the payout at the end of the meeting event 0xa1 (its run 0x5748d0): the character netId (gm_FindChar) and
   the building from (gm_FindById) must exist. The meeting's objects objs[] (the non-zero ones) are looked up in his
   inventory (ai_FindWantedItems: per object the entry, st[0] the inventory's entries); an object he already holds 3 of,
   or one he lacks while his inventory has 6 entries, adds its price (gm_GoodsPrice) to the income; any other he gets
   (cm_SendExSellObject, one of it). He is paid the income (cm_SendMoveGoodsUnchecked) and told (the news 0x5ff, the
   text 0x41f0 with the building around 0x41f2 (objects given) or 0x41f1 (none), both with the income). The objects
   are read by the record's index from objs[] again (equal to the found ones while no zero lies between them). */
void he_CircusShowPayout(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEMEET4 *m = (HEMEET4 *)ev;
    char *bld;
    int st[3];                          /* ai_FindWantedItems: st[0] the inventory's entries */
    int given[4];
    int want[4][4];                     /* ai_FindWantedItems' records: [0] the object, [3] the inventory entry (AIOBJ) */
    char text[0x100];
    char text2[0x200];
    AICHAR *per;
    AIOBJ *entry;
    unsigned short *obj;
    int (*row)[4];
    int n, i, ngiven;

    per = (AICHAR *)gm_FindChar(e->netId);
    if (per == 0)
        return;
    gm_FindById(&bld, 0, 0, e->from);
    if (bld == 0)
        return;
    memset(want, 0, sizeof(want));
    n = 0;
    for (i = 0; i < 4; i++) {
        if (m->objs[i] != 0)
            want[n++][0] = m->objs[i];
    }
    if (n != 0) {
        ai_FindWantedItems(per, n, want, st);
        ngiven = 0;
        obj = m->objs;
        row = want;
        for (i = 0; i < n; i++, obj++, row++) {
            if (*obj == 0)
                continue;
            entry = (AIOBJ *)(*row)[3];
            if (entry != 0) {
                if (entry->count >= 3) {
                    m->income += (int)gm_GoodsPrice(*obj, ai_OwnPlayer);
                } else {
                    given[ngiven++] = *obj;
                    cm_SendExSellObject(per->netId, -1, *obj, 1, ai_OwnPlayer, 0);
                }
            } else if (st[0] >= 6) {
                m->income += (int)gm_GoodsPrice(*obj, ai_OwnPlayer);
            } else {
                st[0]++;
                given[ngiven++] = *obj;
                cm_SendExSellObject(per->netId, -1, *obj, 1, ai_OwnPlayer, 0);
            }
        }
    }
    if (n != 0 && ngiven != 0)
        txt_Format(text, 0x41f2, m->income);
    else
        txt_Format(text, 0x41f1, m->income);
    cm_SendMoveGoodsUnchecked(per->netId, -1, ai_OwnPlayer, m->income);
    txt_Format(text2, 0x41f0, bld, text);
    he_ShowNews(per->netId, -1, text2, 0, 0x5ff, 0);
}

void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g); /* 0x524260, the group register */
int sv_IsAtDoor();                 /* 0x524770 */
void he_CircusShowIncome(int ev, int n);   /* 0x575030 */

/* 0x5748d0, the run of the event 0xa1, a circus show of up to four figures (AIJOB.ids, the ones gm_FindChar finds):
   -1 / -2 or no figure: they leave the group (sv_MoveToGroup) and the event is freed. Stage 0 picks the building:
   with time left (room > 0) one of the show's building classes (he_FindMeetingRoom, walked to as a room), else the
   building from, at its main room (gm_GetGebaeudeHauptraum); without a building the event ends (stage -1, due now).
   Else bld keeps the building's id and every figure joins the group, stops and walks there ("Bettler"); stage 1 is
   due in 10 minutes. Stage 1 (the building found again by bld, else the end as above) waits
   5 minutes until every figure is at its door (sv_IsAtDoor); then with time left the show runs for 2 hours (stage 2):
   each figure with a model plays 8 of the ten circus animations at random (cm_SendChrAnim, 1 or 2 times each); with
   none left the figures leave the group, the payout runs (he_CircusShowPayout) and the event ends. Stage 2 stops the
   figures, takes the show's income (he_CircusShowIncome), takes 120 off the time and goes back to stage 0, due now. */
void he_RunCircusShow(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEMEET4 *m = (HEMEET4 *)ev;
    AIBLD *bld;
    AICHAR *pers[4];
    int viaRoom, place;
    AIOBJ *hall;
    AICHAR *p;
    int n, i, k;

    n = 0;
    memset(pers, 0, sizeof(pers));
    bld = 0;
    for (i = 0; i < 4; i++) {
        if (e->ids[i] != -1 && (p = (AICHAR *)gm_FindChar(e->ids[i])) != 0)
            pers[n++] = p;
    }
    if (e->stage == -1 || e->stage == -2 || n == 0) {
        for (i = 0; i < n; i++)
            sv_MoveToGroup(0, 0, pers[i]->id, 0);
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage == 0) {
        place = -1;
        if (m->room > 0) {
            bld = (AIBLD *)he_FindMeetingRoom(ev);
            viaRoom = 1;
            m->meetStart++;
        } else {
            gm_FindById(&bld, 0, 0, e->from);
            viaRoom = 0;
            if (bld != 0 && (hall = (AIOBJ *)gm_GetGebaeudeHauptraum((unsigned char *)bld)) != 0)
                place = hall->objId;
        }
        if (bld == 0) {
            e->stage = -1;
            e->end = ai_Time;
            return;
        }
        m->bld = bld->id;
        for (i = 0; i < n; i++) {
            p = pers[i];
            sv_MoveToGroup(0, 0, p->id, (char *)ev);
            cm_SendChrStop(p->netId);
            cm_SendChrGotoBuilding(p->netId, bld->id, place, 0, viaRoom, "Bettler");
        }
        e->stage = 1;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 0xa, 0);
        return;
    }
    gm_FindById(&bld, 0, 0, m->bld);
    if (bld == 0) {
        e->stage = -1;
        e->end = ai_Time;
        return;
    }
    if (e->stage == 1) {
        char anims[10][0x40] = {
            "zirkus/anfeuern", "zirkus/handstand", "zirkus/handstand2", "zirkus/lachen", "zirkus/lang_winken",
            "zirkus/posieren", "zirkus/ruecksalto", "zirkus/ruecksalto2", "zirkus/stehend_beschwoeren",
            "zirkus/tief_verbeugen"
        };
        for (i = 0; i < n; i++) {
            if (sv_IsAtDoor(pers[i], bld) == 0) {
                e->stage = 1;
                e->end = ai_Time;
                gm_AddTime(&e->end, 0, 5, 0);
                return;
            }
        }
        if (m->room <= 0) {
            for (i = 0; i < n; i++)
                sv_MoveToGroup(0, 0, pers[i]->id, 0);
            he_CircusShowPayout(ev);
            e->stage = -1;
            e->end = ai_Time;
            return;
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 2, 0, 0);
        e->stage = 2;
        for (i = 0; i < n; i++) {
            p = pers[i];
            if (p->model != 0) {
                for (k = 8; k != 0; k--)
                    cm_SendChrAnim(p->netId, anims[util_RandomU(0xa)], util_RandomU(2) + 1);
            }
        }
        return;
    }
    if (e->stage == 2) {
        for (i = 0; i < n; i++)
            cm_SendChrStop(pers[i]->netId);
        he_CircusShowIncome(ev, n);
        m->room -= 0x78;
        e->end = ai_Time;
        e->stage = 0;
    }
}
/* 0x574fe0, the meeting room of the event 0xa1: up to three of the event's words meetClasses[meetStart],
   [meetStart % 4 + 1], ... are looked up as buildings by their class (0x501190 (1, 5, word)); returns the
   first hit, 0 when none */

int he_FindMeetingRoom(int ev)
{
    AIJOB *e = (AIOBJ *)ev;
    int i, n, r;

    n = e->meetStart;
    r = 0;
    i = 3;
    while (1) {
        if (i == 0)
            break;
        r = gm_BuildQuery(1, 5, e->meetClasses[n]);
        i--;
        n = n % 4 + 1;
        if (r != 0)
            break;
    }
    return r;
}

unsigned short ai_RandomDomizilGoodNotIn(int param_1, unsigned short *param_2);                 /* 0x5a9e90 */

/* 0x575030, a round's takings of the circus show event 0xa1 (stage 2 of he_RunCircusShow) for its n figures: the
   character netId (gm_FindChar) and the building from (gm_FindById) must exist. The rate is 1 + the building's skill
   0x1d / 60 (gm_BuildingSkill), the character's skill 2 (gm_GetSkill) squared the base; n times the income grows by
   32 * (int)(((random + base) * 75 + 25) * rate). Then, when random + 1 is below the rate, the first free one of the
   show's four objects may (random above 0.4) become a Domizil good not among the ones before it (ai_RandomDomizilGoodNotIn). */
void he_CircusShowIncome(int ev, int n)
{
    AIJOB *e = (AIJOB *)ev;
    HEMEET4 *m = (HEMEET4 *)ev;
    AICHAR *per;
    AIBLD *bld;
    float rate, base, skill;
    int i;

    per = (AICHAR *)gm_FindChar(e->netId);
    if (per == 0)
        return;
    gm_FindById(&bld, 0, 0, e->from);
    if (bld == 0)
        return;
    rate = gm_BuildingSkill(bld, 0x1d, 1) * 0.0166666675f + 1.0f;
    skill = gm_GetSkill((char *)per, 2);
    base = skill * skill;
    for (i = n; i > 0; i--)
        m->income += (int)(((util_RandomF() + base) * 75.0 + 25.0) * rate) * 0x20;
    if (util_RandomF() + 1.0f < rate) {
        for (i = 0; i < 4; i++) {
            if (m->objs[i] == 0) {
                if (util_RandomF() > 0.4)
                    m->objs[i] = ai_RandomDomizilGoodNotIn(i, m->objs);
                return;
            }
        }
    }
}
/* 0x575150, the alloc of the event 0xa3: stage 0, start and end = now, the end plus random (2) hours and random (0x1e)
   minutes (gm_AddTime) */

void he_AllocPettyLoss(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->start = ai_Time;
    e->end = ai_Time;
    e->stage = 0;
    gm_AddTime(&e->end, util_Random(2) & 0xffff, util_Random(0x1e) & 0xffff, 0);
}
/* 0x5751e0, the run of the event 0xa3 (alloc he_AllocPettyLoss): -1 / -2, a stage above 3 or no character netId
   (gm_FindChar): freed. Else the stage advances and the next run is due in 3 + random(3) hours; a character with money
   loses 1 + random(2) of it when a first such draw is below his money, else all of it (gm_SendMoneyDelta); one of the
   kinds 6 / 7 is told (the news 0x5fb, the text 0x1008 with the amount); past the stage 3 the event is due now (and freed then).
   "Petty loss" is a reasoned guess from that effect.
   @deep read whole against the original and reconstructed (the amount is all the money unless a first draw
   1+random(2) is below it, then a second draw; the stage>3 due-now inside the money block; free on
   -1/-2/>3/no char). Only allocation is left: the original keeps per in esi and the amount in edi and
   saves edi lazily (push edi after the money>=1 test at +0x7a, pop edi before the stage test at +0xf0);
   ours puts per in edi, the amount in esi and saves all four in the prologue. Hunks: regs 46 (edi->esi,
   esi->edi), code 11 = the push/pop edi placement (+0x13, +0x7a/+0x7d mov edi,eax, +0xed..+0xf1, the two
   epilogues). Tried: money local vs re-read, x block-scoped, early return on money<1, declaration order.
   proof: only eax at the (void) return differs. */
void he_RunPettyLoss(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    char text[0x400];
    AICHAR *per;
    int x;

    per = (AICHAR *)gm_FindChar(e->netId);
    if (e->stage == -1 || e->stage == -2 || e->stage > 3 || per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    e->stage++;
    gm_AddTime(&e->end, (util_Random(3) & 0xffff) + 3, 0, 0);
    if (per->money < 1)
        return;
    x = per->money;
    if ((util_Random(2) & 0xffff) + 1 < x)
        x = util_RandomU(2) + 1;
    gm_SendMoneyDelta(per->netId, -x);
    if (per->kind == 6 || per->kind == 7) {
        txt_Format(text, 0x1008, x);
        he_ShowNews(per->netId, -1, text, 0, 0x5fb, 0);
    }
    if (e->stage > 3)
        e->end = ai_Time;
}
/* 0x575320, the alloc of the event 0xa4: stage 0, start and end = now, the end plus random (0x20) hours (gm_AddTime) */

void he_AllocEventGood(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->start = ai_Time;
    e->end = ai_Time;
    e->stage = 0;
    gm_AddTime(&e->end, util_Random(0x20) & 0xffff, 0, 0);
}

void he_GiveDomizilGood(AICHAR *per);     /* 0x575430 */
void he_EventGoodMoney(AICHAR *per);      /* 0x575540 */
void he_EventGoodRefund(AICHAR *per);     /* 0x575570 */

/* 0x5753a0, the run of the event 0xa4: -2 / -1 free, also without the character +0xc (gm_FindChar); a random draw picks
   the good outcome above 0.8 (he_GiveDomizilGood), the money one above 0.4 (he_EventGoodMoney), else the refund
   (he_EventGoodRefund); then free */
void he_RunEventGood(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *per;
    float r;
    int state;

    per = (AICHAR *)gm_FindChar(e->netId);
    state = e->stage;
    if (state == -1 || state == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    r = util_RandomF();
    if (r > (0.800000012f))
        he_GiveDomizilGood(per);
    else if (r > (0.400000006f))
        he_EventGoodMoney(per);
    else
        he_EventGoodRefund(per);
    he_FreeEvent((int *)ev);
}
/* 0x575430, the good outcome of the event 0xa4 (he_RunEventGood): a Domizil good of the ranks 3 .. 6
   (ai_DomizilGoodInRange) is looked up in the character's inventory (ai_FindWantedItems; st[0] its entries); he gets
   one of it (cm_SendExSellObject) unless he already holds 3 of it or lacks it with 6 entries in his inventory: then
   he is paid its price instead (gm_GoodsPrice, cm_SendMoveGoodsUnchecked). */
void he_GiveDomizilGood(AICHAR *per)
{
    int st[3];                          /* ai_FindWantedItems: st[0] the inventory's entries */
    int want[4];                        /* ai_FindWantedItems' record: [0] the good, [3] the inventory entry (AIOBJ) */
    AIOBJ *entry;

    memset(want, 0, sizeof(want));
    want[0] = ai_DomizilGoodInRange(3, 6) & 0xffff;
    ai_FindWantedItems(per, 1, want, st);
    entry = (AIOBJ *)want[3];
    if (entry != 0) {
        if (entry->count >= 3)
            cm_SendMoveGoodsUnchecked(per->netId, -1, ai_OwnPlayer,
                                      (int)gm_GoodsPrice((unsigned short)want[0], ai_OwnPlayer));
        else
            cm_SendExSellObject(per->netId, -1, want[0], 1, ai_OwnPlayer, 0);
    } else if (st[0] >= 6) {
        cm_SendMoveGoodsUnchecked(per->netId, -1, ai_OwnPlayer, (int)gm_GoodsPrice(want[0], ai_OwnPlayer));
    } else {
        cm_SendExSellObject(per->netId, -1, want[0], 1, ai_OwnPlayer, 0);
    }
}
/* 0x575540, the middle outcome of the event 0xa4: the character +4 is credited 3 + random (4) money (gm_SendMoneyDelta) */

void he_EventGoodMoney(AICHAR *per)
{
    gm_SendMoneyDelta(per->netId, (util_Random(4) & 0xffff) + 3);
}
/* 0x575570, the low outcome of the event 0xa4: the character +4 is credited the price of good 0x2b7 (gm_GoodsPrice)
   times (2 * random + 1) (cm_SendMoveGoodsUnchecked) */

void he_EventGoodRefund(AICHAR *per)
{
    float f;

    f = gm_GoodsPrice(0x2b7, ai_OwnPlayer);
    cm_SendMoveGoodsUnchecked(per->netId, -1, ai_OwnPlayer,
                              (int)((util_RandomF() * 2.0f + 1.0f) * f));
}
/* 0x5755c0, copies the INI path `s` to 0x6fa0e0 and loads "Game" / "message_filter_mask" from it (GetPrivateProfileIntA) into 0x6f263c */

void he_LoadMessageFilter(char *s)
{
    strcpy(he_OptionsPath, s);
    he_MessageFilterMask = GetPrivateProfileIntA("Game", "message_filter_mask", 0, he_OptionsPath);
}
/* 0x575600, writes 0x6f263c as "Game" / "message_filter_mask" into the INI file 0x6fa0e0 (WritePrivateProfileStringA) */

void he_SaveMessageFilter(void)
{
    char buf[0xc];

    sprintf(buf, "%d", he_MessageFilterMask);
    WritePrivateProfileStringA("Game", "message_filter_mask", buf, he_OptionsPath);
}
/* 0x575640, 1 when the message filter mask 0x6f263c has the bit of the message kind id (0x605 the bit 1, 0x606 2, 0x607 4, 0x608 8,
   0x609 0x10, 0x60a 0x20, 0x60b 0x40, 0x60c 0x80, 0x60d 0x100); else 0 */

int he_IsMessageKindFiltered(int id)
{
    switch (id) {
    case 0x605:
        if (he_MessageFilterMask & 1)
            return 1;
        break;
    case 0x606:
        if (he_MessageFilterMask & 2)
            return 1;
        break;
    case 0x607:
        if (he_MessageFilterMask & 4)
            return 1;
        break;
    case 0x608:
        if (he_MessageFilterMask & 8)
            return 1;
        break;
    case 0x609:
        if (he_MessageFilterMask & 0x10)
            return 1;
        break;
    case 0x60a:
        if (he_MessageFilterMask & 0x20)
            return 1;
        break;
    case 0x60b:
        if (he_MessageFilterMask & 0x40)
            return 1;
        return 0;
    case 0x60c:
        if (he_MessageFilterMask & 0x80)
            return 1;
        break;
    case 0x60d:
        if (he_MessageFilterMask & 0x100)
            return 1;
        break;
    }
    return 0;
}
/* 0x575710, sets / clears the bit of the message kind id in the filter mask 0x6f263c (0x605 the bit 1, 0x606 2,
   0x607 4, 0x608 8, 0x609 0x10, 0x60a 0x20, 0x60b 0x40, 0x60c 0x80, 0x60d 0x100): `on` 0 clears it, else sets it.
   The setter side of he_IsMessageKindFiltered; the jump table 0x575830. Ends 0x57582f, the table runs to 0x575854, fill to 0x575860 */

void he_SetMessageKindFiltered(int id, int on)
{
    switch (id) {
    case 0x605:
        if (on)
            he_MessageFilterMask |= 1;
        else
            he_MessageFilterMask &= ~1;
        break;
    case 0x606:
        if (on)
            he_MessageFilterMask |= 2;
        else
            he_MessageFilterMask &= ~2;
        break;
    case 0x607:
        if (on)
            he_MessageFilterMask |= 4;
        else
            he_MessageFilterMask &= ~4;
        break;
    case 0x608:
        if (on)
            he_MessageFilterMask |= 8;
        else
            he_MessageFilterMask &= ~8;
        break;
    case 0x609:
        if (on)
            he_MessageFilterMask |= 0x10;
        else
            he_MessageFilterMask &= ~0x10;
        break;
    case 0x60a:
        if (on)
            he_MessageFilterMask |= 0x20;
        else
            he_MessageFilterMask &= ~0x20;
        break;
    case 0x60b:
        if (on)
            he_MessageFilterMask |= 0x40;
        else
            he_MessageFilterMask &= ~0x40;
        break;
    case 0x60c:
        if (on)
            he_MessageFilterMask |= 0x80;
        else
            he_MessageFilterMask &= ~0x80;
        break;
    case 0x60d:
        if (on)
            he_MessageFilterMask |= 0x100;
        else
            he_MessageFilterMask &= ~0x100;
        break;
    }
}
