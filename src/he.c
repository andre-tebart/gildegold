/* he.c -- the event ("Handlung" / HE) system, tag "he_" (0x626680 ..), panel "Panel\Event_Leiste": the 16 slot
   event bar (0x6faa80), the event records (0x400 of 0x14c bytes at 0x6fab40), the alloc / run handler tables
   (0x74df40 / 0x758600, registered by he_RegisterEventTypes22 / he_RegisterEventTypes02) and the handlers of the
   single event types.
   0x53d430 .. 0x575860: starts right after main.c's program entry (its globals 0x625718 / 0x62571c follow
   main.c's INI strings); the tutorial / campaign block (.data 0x62918c) follows.
   (no @bytes: the block after he_Unk555b60's ret at 0x556055 is its int 3 error path, reached by the je at 0x555dd7)
   The event record is AIJOB (0x14c bytes, he_Jobs over he_Events), the message record AIMSG (both in ai_types.h,
   readable without ai.h); the buildings and characters are AIBLD / AICHAR (ai_bld.h / ai_char.h, he_PlayerCh), the
   carts AIOBJ (ai_types.h), the carried-goods records GMCARRYREC (f3gm_carry.h), the goods prototypes GMGOOD
   (f3gm_types.h), the AP events HEAPEVENT (he.h, he_ApEvents). The event bar, the event create / free / search, the
   gfx info, AP event and the event posting functions up to 0x53f370 read them typed.
   The handlers up to 0x547ba0 (the feast 0x37 / feast exam 0x36, tribute 0x35, courtship 0x41, patrol 0x43,
   raid 0x44, fight 0x45, duel challenge 0x46 and insult 0x47 events) read AIJOB typed, their kind words named in
   AIJOB's union (fest*, exam*, tribute*, pair*, patrol*, raid*; partner1 / partner2) and the fight parties they
   post as G28REC (cm_types.h, its raid lists).
   The handlers 0x547f90 .. 0x54a7b0 (events 0x4a, 0x53 .. 0x61) read AIJOB / AICHAR / AIBLD / G28REC typed too,
   and so do the send-home event 0x77 (AIJOB.homeUntil), the reaper event 0x54ff20 .. 0x550e9a (AIJOB's reaper*
   words, the D3OBJECT reaper) and the ranking event 0x550ee0 (its packets HERANKHEAD / HERANKLINE).
   Open: types: the other handlers past 0x547ba0 still read the event and message records through raw offsets
   (HM_I / U_I), except the kind-specific words over AIJOB's union the local views name: HEJOBTABLE (the job table
   0x12c at +0x28), HESTAMMTISCH (the meeting event 0x2a at +0xac .. +0xc0), HEOFFER (the offer event 0x2c and the
   master exam 0x2f: who[] at +0xac, +0xc4 / +0xc5 / +0xd4), HEDUEL (the duel challenge's +0xac and the duel's
   +0xb4 / +0xb6), HEGESELLE / HEEXAMPAY / HEPAYMENT / HEOFFSPRING / HETRIAL / HETRIALSTEP / HETRAINING / HEPLAGUE /
   HEGANG (the events 0x4a / 0x55 / 0x56 / 0x5d / 0x5e / 0x5f / 0x60 / 0x59 / 0x61), HEFEEMSG / HESKILLMSG /
   HEPLAGUEMSG (AIMSG's words of the AI messages 0x56 / 0x6a / 0x78), HEPROTECTION / HEAUDIT / HEGATHERING /
   HESIM / HEAUCTION / HETITLE (the events 0x62 / 0x64 / 0x65 / 0x66, 0x68, 0x70 / 0x67 / 0x69), HESKILLUP /
   HEBARD / HEPOPULATION / HELOVER / HEDARKCORNER (the events 0x6a / 0x6b / 0x6d / 0x6f / 0x74; the bard's script
   mark HEBARDSCRIPT, the job table's proofs / year HEJOBTABLE), HEINN (the inn
   simulation's list), HEFINVIEW (AIFIN's +0x18 / +0x64; ai.h is not
   included), and over AIOBJ HETAVERNOBJ / HEOBJ1B5VALUE (the tavern's object 0x1b5). AIJOB's fields between
   0x14 and 0xac are named only as the message's words; a member added to AIJOB / G28REC can flip other units'
   matches by declaration set (ai_Unk483a60, ord_CountGridObjects are debt for it).
   Open: data: he_QuickList (0x74db40, 10921 words) and he_RunHandlers (0x758600) are declared over several objects
   (the quick list, the alloc handlers 0x74df40, the cursor 0x7585e0; the gfx infos 0x7588a0): read through views.
   Open: data: 0x6faa80 he_EventSlots, 0x6faa60 he_BarView, 0x6f262c he_ActiveSlot, 0x6f2630 he_SlotConfirmed,
   0x6f2634 he_ReaperMusic, 0x6f2638 he_PatrolBuilding, 0x625718 he_LastSlot, 0x62571c he_BarShown, the search
   filter 0x625708 .. 0x625714 (he_FindKind .. he_FindFrom), 0x6f2624 he_FindFirst, 0x6f2628 he_FindUnused, owner
   he or main (typed in place in main.c's gap block; he.c and sv.c use them, they lie among main.c's data / bss).
   Open: not included yet (dcp headers conflicts he HEADER): ahm.h ai.h am.h amt.h anm.h cm.h d3_tools.h
   f3gm.h f3snd.h gv.h hmt.h hst.h rt.h sblib.h sv.h unk_4b39f0.h unk_4b6480.h unk_5d33b0.h; held by own
   declarations that differ from the owner's: ai_Time, cm_SendExSetHE, ahm_SendSetShare; calls whose argument
   count differs from the definition (read the original's pushes): gm_AddTime, gm_Liking, gm_TitleNumber,
   gm_TitleGroup, d2_SetObjValue, d3t_PlayAnim, ahm_SendSetShare, sb_Stop, sv_IsAtDoor.
   unit of rea 8cef7d5b (src/game/he.c); boundaries in decl/text.dcl */
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
#define d3t_CreateEmitterK ((int (*)())d3t_CreateEmitter)  /* 0x415ad0 (d3_tools.c): @original he_CreateSmokeEffect / he_CreatePointsEffect call it without a prototype, the floats as dword bit patterns (draft: 19 instructions this way, 29 through the prototype) */
#define ai_PlagueMessageK ((int (*)())ai_PlagueMessage)  /* 0x5a9240: @original the call passes more arguments than the draft definition names, compiled without a prototype (as before the definition's spelling reached this unit) */
#define ai_DomizilMessageK ((int (*)())ai_DomizilMessage)  /* 0x5a9c30: @original the call passes more arguments than the draft definition names, compiled without a prototype (as before the definition's spelling reached this unit) */


extern int ai_TransportWindow();
/* full rebuild: the symbols whose original addresses the code above spells literally */
extern void (*he_RunHandlers[5544])(unsigned char *);
extern const volatile unsigned __int64 gypsyFilter;  /* 0x5eed68, unk_5d33b0.c */
extern int ai_GameFlags;
#include "unk_5d33b0_types.h"
extern OPTIONBYTES ai_GameSwitches;
extern char ai_Chars[768][0x21c];
extern unsigned short * ai_PlayerChar;
extern char * ai_BTypes;
extern char * ai_Blds;
extern int ai_Difficulty;
extern unsigned char ai_OwnPlayer;
extern const int sv_PhaseMinutes[24];
extern char * cm_FieldObject;
extern const int ai_CharWalkSteps[16];
extern signed char ai_Relations[768][768];
extern char ai_Unk625820[115][0x20];
extern unsigned char ai_GameState;
extern char ai_PlayerSlots[4][0x2f4];
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_5ff710[];

extern char ai_Chars[768][0x21c];
extern int gm_AllowAll;
extern GAPDATA full_data_5f320c[];
extern char * ai_BTypes;
extern char * ai_Blds;
extern const int ai_RandomStrides[16];
extern int gm_PlayerRecords[688];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedForm;
extern unsigned short ai_PlayerId;
extern unsigned char ai_GameState;
extern char ai_PlayerSlots[4][0x2f4];
extern char * cm_FieldObject;
extern GAPDATA full_data_5fe7d0[];
extern signed char ai_Relations[768][768];
extern const int ai_CharWalkSteps[16];
extern char ai_Crimes[];
extern const int sv_PhaseMinutes[24];
extern unsigned short * ai_PlayerChar;
extern const float ai_FloatOne;
extern char ai_Unk625820[115][0x20];
extern GAPDATA full_data_60d0d4[];
extern struct G16TRACKHANDLES snd_TrackHandles;
extern int * d3t_Rain;
extern int d3t_Snow;
extern int ai_GameOver;
extern char * ai_Goods;
extern GAPDATA full_data_5f1890[];
extern int ai_PermsDirty;
extern char * ai_PlayerBuilding;
extern int ai_Unk75f280[576];
extern int sv_FrameDepth;
extern unsigned short * sv_PanelShown;
extern char * ai_FallbackBuilding;

extern char ai_Chars[768][0x21c];
extern char * ai_FallbackBuilding;
extern int ai_GameFlags;
extern unsigned char ai_GameState;
extern char * ai_PlayerBuilding;
extern unsigned short ai_PlayerId;
extern int snd_SystemBank;
extern char * ai_BTypes;
extern char * cm_FieldObject;
extern const int sv_PhaseMinutes[24];
extern GAPDATA full_rdata_5ebb44[];
extern double ai_Unk5eb990;
extern GAPDATA full_rdata_5ee020[];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern unsigned short * ai_PlayerChar;
#include "sv_types.h"
extern SVWEATHER sv_Weather;  /* 0x760488, sv.c (he.c does not include sv.h yet) */
extern GAPDATA full_data_627b0c[];
extern char gm_TagGebaeudePROT[24];
extern GAPDATA full_data_627b20[];
extern float gm_DayStartHours[];
extern OPTIONBYTES ai_GameSwitches;
extern float gm_DayEndHours[];
extern GAPDATA full_data_6279f0[];
extern GAPDATA full_data_627a10[];
extern GAPDATA full_data_627d74[];
extern GAPDATA full_data_6fa260[];
extern GAPDATA full_data_6fa264[];
extern GAPDATA full_data_6fa268[];
extern OPTIONBYTES ai_GamePanel;

extern GAPDATA full_data_6fa260[];
extern GAPDATA full_data_6fa264[];
extern GAPDATA full_data_6fa268[];
extern GAPDATA full_data_6fa26c[];
extern GAPDATA full_data_6fa1e0[];
extern OPTIONBYTES ai_GamePanel;
extern unsigned short ai_PlayerId;

extern GAPDATA full_data_6fa1e0[];
extern char ai_Chars[768][0x21c];
extern unsigned char ai_GameState;
extern struct AIBLD *ai_MarketBuilding;
extern int cm_TutorialActive;
extern char ch_EmptyName[1];
extern unsigned char ai_OwnPlayer;
extern char * cm_FieldObject;
extern char * ai_Goods;
extern int gm_PlayerRecords[688];
extern unsigned short * ai_PlayerChar;
extern GAPDATA full_data_628a28[];
extern char * ai_BTypes;
#include "cm_types.h"
extern G17STAT cm_SendStates[];
extern unsigned short gm_KindObjCount;
extern unsigned gm_KindObjs[732];
extern int d2_ClickedId;
extern int d2_ClickedForm;
extern GAPDATA full_data_627e98[];
extern GAPDATA full_data_627ea8[];
extern char ai_MissionMode;
extern int ai_MissionPartner;
#include "unk_4b39f0_types.h"
extern G13MIS ai_Missions[];
extern float gm_DayStartHours[];
extern float gm_DayEndHours[];
extern float ai_FloatZero;
extern const float ai_FloatOne;
extern GAPDATA full_data_628dc8[];

extern GAPDATA full_data_628dc8[];
extern GAPDATA full_data_628dd4[];
extern GAPDATA full_data_628dec[];
extern GAPDATA full_data_628df8[];
extern GAPDATA full_data_628e10[];
extern GAPDATA full_data_628e28[];
extern GAPDATA full_data_628e40[];
extern char ai_Chars[768][0x21c];
extern GAPDATA full_data_628e58[];
extern GAPDATA full_data_628e70[];
extern GAPDATA full_data_628e7c[];
extern GAPDATA full_data_628e8c[];
extern GAPDATA full_data_628e98[];
extern char * ai_BTypes;
extern GAPDATA full_data_628ea4[];
extern GAPDATA full_data_628eb8[];
extern unsigned char he_Events[339968];
extern unsigned char ai_OwnPlayer;
extern unsigned short ai_PlayerId;
extern char ai_Unk625820[115][0x20];
extern int d2_ClickedId;
extern int d2_ClickedForm;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_628ecc[];
extern const int sv_PhaseMinutes[24];
extern float gm_DayStartHours[];
extern float gm_DayEndHours[];
extern double ai_Unk5ece60;
extern const float gm_MoodRanges[476];
extern char ch_EmptyName[1];
extern char * cm_FieldObject;
extern GAPDATA full_rdata_5eb8a8[];
extern char * ai_Blds;
extern unsigned short * ai_PlayerChar;
extern GAPDATA full_data_628f3c[];
extern GAPDATA full_data_628f74[];
extern GAPDATA full_data_628fac[];
extern const float gv_SaleShares[];
extern GAPDATA full_data_628fe4[];
extern float ai_Unk5eb94c;
extern GAPDATA full_data_629000[];
extern GAPDATA full_rdata_5eb7e8[];
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_62901c[];
extern GAPDATA full_data_629038[];
extern GAPDATA full_data_629054[];
extern int gm_AllowAll;
extern GAPDATA full_data_629070[];
extern const float ai_FloatOne;
extern double ai_Unk5ebbe8;
extern GAPDATA full_rdata_5ee01c[];
extern GAPDATA full_data_6f263c[];
extern GAPDATA full_data_6fa0e0[];

void gm_Shuffled(unsigned char n, int *out);
void amt_ChangeDistrictOpinion();
void gm_Shuffled(unsigned char n, int *out);
struct AIPER; struct AMTREC; struct CHARACTER; struct CHACTION; struct EVTSCRIPT; struct G15PLANT; struct G11REC; struct AITIME;
void cm_SendProofExecuted(int a, int b);
void cm_SendChrStop(int a);
void cm_SendFieldDeltas(void);
int cm_SendRelations(int a, int b, int c, int d);

int gm_PlayerFactor(int a, unsigned char k);
void gm_SetTime(struct AITIME *c, unsigned char hour, unsigned min, unsigned sec);

#define U_I(a) (*(int *)(a))
typedef void (*HEALLOC)(AIJOB *ev);     /* an event type's alloc handler (registered with its run handler by he_SetEventHandlers) */
#define he_AllocHandlers (*(HEALLOC (*)[0xa7])((char *)&he_QuickList + 0x400))  /* 0x74df40 */

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_PickActionBuilding)(p, a, fl, b, c, f, o)

extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
#define AICHAR_FLAGS1C8 0x71  /* the ai_CharInts index of AICHAR.flags1c8 (ai.h, +0x1c8; ai_CharInts starts at +4) */
#define SLOT(i) ((int *)((unsigned)&he_EventSlots + (i) * 12))

/* one of the ten 16-byte slots of the walk event 0x5512c0 at AIJOB +0xac (he_Unk5512c0 clears them) */
typedef struct HEWALKSLOT {
    int chr;                            /* 0x0, the walking character's netId (gm_FindChar), -1 free */
    int target;                         /* 0x4, the building it walks to (gm_FindById's building) */
    unsigned char flags[4];             /* 0x8, state bytes (+0x9: 1 counted, 4 on its way: he_Unk551b70) */
    int pad;                            /* 0xc */
} HEWALKSLOT;

void d3io_NoopCallback(); int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v); void cm_BeginFieldBlock(char *o, int id); float gm_Mix(int a, int b, int c);
int he_RegisterEventTypes22(void);
int he_RegisterEventTypes02();
int he_RegisterEventTypes03();
int he_RegisterEventTypes88();
int he_RegisterEventTypes95();
int he_RegisterEventTypes99();
int he_RegisterEventTypes9d();
int he_RegisterEventTypeA2();

int gm_CompareTime();

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
#include "unk_458810.h"
#include "unk_575860.h"
#include "unk_58bbd0.h"
#include "ai_bld.h"
#include "f3gm_carry.h"
#include "ai_char.h"
#include "f3gm_types.h"              /* GMGOOD / GM_GOOD (the goods prototypes) */
#include "gv_types.h"
#include "ch.h"
#include "am_types.h"
#include "net.h"
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define cm_SendSpawnCharacterR ((int (*)(unsigned char, int, int, unsigned char, unsigned char, unsigned char, const char *))cm_SendSpawnCharacter)  /* 0x4c59f0: @original he_BeginPlague keeps what it returns (eax after the call: the cm slot of the new character), cm.c's definition is void */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original he_SpawnFounders reads the 16-bit result (mov cx, ax / and 0xffff) */
#define ai_PlayerSlotsT (*(GMPLAYERSLOT (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (cm.h: char[][0x2f4]) as the four player slots */
#define snd_CurrentTrack (*(char **)&snd_TrackHandles)  /* 0x6f0b6c, snd_TrackHandles.current (f3snd.h G16TRACKHANDLES, not included here): the music track playing */
#define he_PlayerCh (*(AICHAR **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) as the AICHAR it points to */
#define ai_TimeHourW (*(unsigned *)((char *)&ai_Time + 4))  /* 0x770ae4, ai_Time's hour with the low word of the minute int packed over it (0x565410 masks it to 0xffff) */
#define gm_NearestBuildingK ((int (*)())gm_NearestBuilding)  /* 0x50xxxx: @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
#define sv_SelectPanelObjectK ((int (*)())sv_SelectPanelObject)  /* 0x52c4c0?: @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
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
    char pad00[0x18];
    int gangTakings;                    /* 0x18, the money the pickpocket gangs brought its character (he_RunGang adds it) */
    char pad1c[0x48];
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
#pragma pack(pop)
/* 0x53d430, the allocation handler of an event of an unknown type (debug text only) */

void he_AllocNone(unsigned char *ev)
{
    AIJOB *e = (AIJOB *)ev;
    char buf[256];

    sprintf(buf, "he_AllocNone(): Invalid HE-Type : %i, von Spieler %s", *ev, ai_CharsT[e->chr].name);
}
/* 0x53d470, the run handler of an event of an unknown type (debug text only) */

void he_RunNone(unsigned char *ev)
{
    AIJOB *e = (AIJOB *)ev;
    char buf[256];

    sprintf(buf, "he_RunNone(): Invalid HE-Type : %i, von Spieler %s", *ev, ai_CharsT[e->chr].name);
}
/* 0x53d4b0, the init of the event module: registers the alloc / run handlers of the types 0 and 1, runs the init of the other event modules (0x553060 .. 0x573260; non 0 = failed) and clears the tables */

int he_InitEvents(void)
{
    if (he_SetEventHandlers(0, (void *)he_AllocNone, (void *)he_RunNone) != 0)
        return 1;
    if (he_SetEventHandlers(1, (void *)d3io_NoopCallback, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_RegisterEventTypes22() != 0)
        return 1;
    if (he_RegisterEventTypes02() != 0)
        return 1;
    if (he_RegisterEventTypes03() != 0)
        return 1;
    if (he_RegisterEventTypes88() != 0)
        return 1;
    if (he_RegisterEventTypes95() != 0)
        return 1;
    if (he_RegisterEventTypes99() != 0)
        return 1;
    if (he_RegisterEventTypes9d() != 0)
        return 1;
    if (he_RegisterEventTypeA2() != 0)
        return 1;
    memset((void *)((unsigned)&he_QuickList + 0x6a0), 0, 0x2900 * 4);
    memset((void *)((unsigned)&he_RunHandlers + 0x6a0), 0, 0x1400 * 4);
    memset((void *)((unsigned)&he_RunHandlers + 0x2a0), 0, 0x100 * 4);
    return 0;
}
/* 0x53d590, frees every used record (0x14c bytes each from 0x6fab40) up to 0x6f261c */

void he_FreeEvents(void)
{
    char *p;
    int i = 0;

    if (he_EventTop >= 0) {
        p = (char *)(unsigned)&he_Events;
        do {
            if ((int)p >= (int)((unsigned)&he_Events + 0x53000))
                return;
            if (*p != 0)
                he_FreeEvent(p);
            i++;
            p += 0x14c;
        } while (i <= he_EventTop);
    }
}
/* 0x53d5d0, frees the records, then asks 0x52a040 three times */

int he_ReleaseEvents(void)
{
    he_FreeEvents();
    if (sv_ReturnZero() != 0)
        return 1;
    if (sv_ReturnZero() != 0)
        return 1;
    return sv_ReturnZero() != 0;
}
/* 0x53d600, sets the alloc / run handlers of the event type `type` (0 .. 0xa6; 1 = type out of range) */

int he_SetEventHandlers(int type, void *alloc, void *run)
{
    if (type >= 0 && type < 0xa7) {
        he_AllocHandlers[type] = (HEALLOC)alloc;
        he_RunHandlers[type] = (void (*)(unsigned char *))run;
        return 0;
    }
    return 1;
}
/* 0x53d630, makes room in the 16 slots: finds the slot whose event started earliest, releases the slot and frees the event (he_ReleaseSlot / he_FreeEvent) and returns 0xf; -1 when no slot holds an event */

int he_FreeOldestSlot(void)
{
    AITIME oldest;
    AIJOB *best = 0;
    int bi = -1;
    int i;

    oldest = ai_Time;
    for (i = 0; i < 16; i++) {
        if (he_EventSlots[i].ev != 0 && he_EventSlots[i].obj != -1
            && gm_CompareTime(&he_EventSlots[i].ev->start, &oldest) < 0) {
            oldest = he_EventSlots[i].ev->start;
            best = he_EventSlots[i].ev;
            bi = i;
        }
    }
    if (best != 0) {
        he_ReleaseSlot((int)best);
        he_FreeEvent((int *)best);
        return 0xf;
    }
    if (bi != -1) {
        __asm int 3
    }
    return bi;
}
/* 0x53d6f0, gives the event `ev` a slot of the event bar (only with its option 8; 1: not shown in the bar, 2: no slot free
   nor freed by he_FreeOldestSlot): the slot's picture object (pic, at 32 * the slot index) and its event window, the view
   form opened at (x, y); the slot is kept in the event's slot field; 0 */

int he_OpenEventSlot(int ev, int x, int y, char *form, int pic)
{
    AIJOB *job = (AIJOB *)ev;
    int i;

    if ((job->opts & 8) == 0) {
        job->slot = 0;
        return 1;
    }
    for (i = 0; i < 16; i++)
        if (he_EventSlots[i].obj == -1)
            break;
    if (i >= 16)
        i = he_FreeOldestSlot();
    if (i == -1) {
        job->slot = 0;
        return 2;
    }
    if (i > he_LastSlot)
        he_LastSlot = i;
    d2_SetForm(he_BarView, 0);
    job->slot = &he_EventSlots[i];
    job->slot->ev = job;
    job->slot->obj = d2_WinOpenPic(i << 5, 0, pic, d2_LastWindow);
    d2_Objs[job->slot->obj].checkable = 1;
    job->slot->win = d2_OpenForm(x, y, form);
    d2_PlaceMainWindow(job->slot->win, 3);
    d2_SetViewEnabled(job->slot->win, 0);
    if (job->slot->obj == -1) {
        __asm int 3
    }
    if (job->slot->win == -1) {
        __asm int 3
    }
    return 0;
}
/* 0x53d800, releases the slot of the event `ev`: removes its picture object and closes its window, moves the events of
   the slots after the first free one down by one (their objects 32 to the left, d2_MoveObj) and sets he_LastSlot to the
   highest slot still used */

void he_ReleaseSlot(int ev)
{
    AIJOB *job = (AIJOB *)ev;
    int i, k;

    if (job->slot == 0)
        return;
    if (job->slot->obj != -1) {
        d2_SetForm(he_BarView, 0);
        d2_RemoveObj(job->slot->obj);
        job->slot->obj = -1;
    } else {
        __asm int 3
    }
    if (job->slot->win != -1) {
        d2_CloseView(job->slot->win);
        job->slot->win = -1;
    }
    if (he_ActiveSlot == job->slot)
        he_ActiveSlot = 0;
    job->slot = 0;
    for (i = 0; i < 16; i++)
        if (he_EventSlots[i].obj == -1)
            break;
    if (i < 16) {
        do {
            if (i < 15 && he_EventSlots[i + 1].obj != -1) {
                he_EventSlots[i].obj = he_EventSlots[i + 1].obj;
                he_EventSlots[i].ev = he_EventSlots[i + 1].ev;
                he_EventSlots[i].win = he_EventSlots[i + 1].win;
                he_EventSlots[i].ev->slot = &he_EventSlots[i];
                he_EventSlots[i + 1].obj = -1;
                he_EventSlots[i + 1].ev = 0;
                he_EventSlots[i + 1].win = -1;
                if (he_EventSlots[i].obj == -1 && he_EventSlots[i].ev != 0) {
                    __asm int 3
                }
            }
            k = he_EventSlots[i].obj;
            if (k != -1)
                d2_MoveObj(d2_Objs[k].x - 0x20, d2_Objs[k].y, k);
        } while (++i < 16);
    }
    for (i = 0; i < 16; i++)
        if (he_EventSlots[i].obj != -1)
            he_LastSlot = i;
}
/* 0x53d930, creates the panel "Panel\Event_Leiste" (bottom right: screen size - 0x7d / - 0x21) and frees all slots */

void he_CreateEventBar(void)
{
    int i;

    he_BarView = d2_OpenForm(0, 0, "Panel\\Event_Leiste");
    d2_SetForm(he_BarView, 0);
    d2_MoveObj(d2_ScreenW - 0x7d, d2_ScreenH - 0x21, d2_Wins[d2_LastWindow].parent);
    for (i = 0; i < 16; i++)
        he_EventSlots[i].obj = -1;
    d2_SetViewEnabled(he_BarView, 0);
}
/* 0x53d9b0, closes every used slot and the event bar panel */

void he_CloseEventSlots(void)
{
    HEEVSLOT *s;

    for (s = he_EventSlots; (int)s < (int)(he_EventSlots + 16); s++)
        if (s->obj != -1)
            he_ReleaseSlot((int)s->ev);
    d2_CloseView(he_BarView);
}
/* 0x53d9f0, shows / hides the event bar for the player slot `pl` (0xffff = hide): a bar already shown first hides its
   event windows and moves to the bottom right of the screen hidden; shown, he_LastSlot is the first free slot and
   he_BarShown the bar's view, hidden both are -1 */

void he_ShowEventBar(int pl)
{
    int i;

    if (he_BarShown != -1) {
        for (i = 0; i < 16; i++)
            if (he_EventSlots[i].obj != -1 && he_EventSlots[i].win != -1)
                d2_SetViewEnabled(he_EventSlots[i].win, 0);
        d2_SetForm(he_BarView, 0);
        d2_MoveObj(d2_ScreenW - 0x7d, d2_ScreenH - 0x21, d2_Wins[d2_LastWindow].parent);
        d2_SetViewEnabled(he_BarView, 0);
    }
    if (pl != 0xffff) {
        d2_SetViewEnabled(he_BarView, 1);
        for (i = 0; i < 16; i++)
            if (he_EventSlots[i].obj == -1)
                break;
        he_LastSlot = i;
        he_BarShown = he_BarView;
        return;
    }
    he_LastSlot = -1;
    he_BarShown = -1;
}
/* 0x53dad0, sets the state of the event bar panel */

void he_SetBarEnabled(int state)
{
    if (he_BarShown != -1)
        d2_SetViewEnabled(he_BarView, state);
}

/* the quick jump marker (0x6333e0): set when the active event is a type 0x11 record with bit 0x40 at +0xf0 or a type 0x87 record, outside the network games */
#define HE_PAUSES(e) ((e) != 0 && ((e)->kind == 0x11 && ((e)->modes & 0x40) != 0 || (e)->kind == (char)0x87) && (ai_GameFlags & 4) == 0)  /* the event pauses the game while its window is active (kinds 0x11 with mode 0x40, 0x87; not with game flag 4) */
#define HE_JUMP(r) HE_PAUSES((AIJOB *)(r))

/* 0x53daf0, picks the active event window (he_ActiveSlot): drops the active slot whose object is no longer checked (hides
   its window, unpauses), then activates each used slot whose object is checked: the previous one's window hidden and
   its object unchecked, the new one's window shown and raised; an event of HE_PAUSES pauses the game (tb_Paused) */
void he_PickActiveEvent(void)
{
    int i;

    if (he_BarShown != -1) {
        if (he_ActiveSlot != 0 && he_ActiveSlot->obj != -1 && d2_Objs[he_ActiveSlot->obj].checked == 0) {
            if (he_ActiveSlot->win != 0)
                d2_SetViewEnabled(he_ActiveSlot->win, 0);
            if (HE_PAUSES(he_ActiveSlot->ev))
                tb_Paused = 0;
            he_ActiveSlot = 0;
        }
        for (i = 0; i < 16; i++) {
            if (he_EventSlots[i].obj != -1 && he_ActiveSlot != &he_EventSlots[i] && d2_Objs[he_EventSlots[i].obj].checked != 0) {
                if (he_ActiveSlot != 0 && he_ActiveSlot->win != -1) {
                    if (he_ActiveSlot->win != 0)
                        d2_SetViewEnabled(he_ActiveSlot->win, 0);
                    if (he_ActiveSlot->obj != -1) {
                        d2_Objs[he_ActiveSlot->obj].checked = 0;
                        d2_Objs[he_ActiveSlot->obj].highlighted = 0;
                    }
                    if (HE_PAUSES(he_ActiveSlot->ev))
                        tb_Paused = 0;
                }
                he_ActiveSlot = &he_EventSlots[i];
                d2_SetViewEnabled(he_EventSlots[i].win, 1);
                d2_RaiseViewWindows(he_ActiveSlot->win);
                if (HE_PAUSES(he_ActiveSlot->ev))
                    tb_Paused = 1;
            }
        }
    }
}
/* 0x53dcb0, makes the slot `sl` the active event window (0x6f262c), closing the previous one */

void he_ActivateSlot(int *sl)
{
    HEEVSLOT *s = (HEEVSLOT *)sl;

    if (he_BarShown != -1) {
        if (he_ActiveSlot != 0) {
            if (he_ActiveSlot->obj != -1)
                d2_SetObjValue(he_ActiveSlot->obj, 0);
            d2_SetViewEnabled(he_ActiveSlot->win, 0);
            if (HE_PAUSES(he_ActiveSlot->ev))
                tb_Paused = 0;
            he_ActiveSlot = 0;
        }
        if (s != 0 && s->win != -1) {
            if (s->obj != -1)
                d2_SetObjValue(s->obj, 1);
            d2_SetViewEnabled(s->win, 1);
            d2_RaiseViewWindows(s->win);
            if (HE_PAUSES(s->ev))
                tb_Paused = 1;
        }
        he_ActiveSlot = s;
    }
}

#define HM_B(m, o) (*(unsigned char *)((m) + (o)))
#define HM_I(m, o) (*(int *)((m) + (o)))

short * gm_FindChar(int id);
int cm_SendLongMessageText();
void err_Message(char *text);

/* 0x53dda0, sends the news message 0x11 (message flags 9) to the character `pid`, which must be of kind 6 / 7 (else -1):
   from `from`, the news id `news` (arg[0]), the text flags `tflags` (| 0x10 when `text2` follows the text behind its NUL);
   cm_SendLongMessageText's result
   @deep read whole against the original and rewritten on AIMSG (moved to ai_types.h): the parameters are
   (pid, from -> fromId, text, tflags -> textFlags byte at 0x9c, news -> arg[0], text2); fixed the second
   textFlags store (0x9c with | 0x10, was 0xa0), the text-only send (passes text itself, not buf) and buf's
   size (0x1fa0, frame 0x2098). Left: only the placement of the callee-saved pushes: the original pushes
   esi alone before the kind 6 / 7 check and ebx / ebp / edi after it (its -1 path pops esi only), ours
   pushes all four at the entry (+0xa / +0x35 / +0x127 hunks) and schedules one lea edx, [esp+0x108] later;
   tried an early return, the body nested in the if and the locals in the inner block, all the same */
int he_ShowNews(int pid, int from, char *text, int tflags, int news, char *text2)
{
    AIMSG msg;
    char buf[0x1fa0];
    unsigned char *ch;

    ch = (unsigned char *)gm_FindChar(pid);
    if (ch != 0 && (ch[2] == 6 || ch[2] == 7)) {  /* AICHAR.kind */
        unsigned char fl = (unsigned char)tflags;
        int n;

        memset(&msg, 0, sizeof(msg));
        msg.to = pid;
        msg.fromId = from;
        msg.type = 0x11;
        msg.flags = 9;
        msg.arg[0] = news;
        msg.textFlags = fl;
        strcpy(buf, text);
        n = strlen(text) + 1;
        if (text2 == 0 || *text2 == 0)
            return cm_SendLongMessageText(&msg, n, text);
        memcpy(buf + n, text2, strlen(text2) + 1);
        fl |= 0x10;
        msg.textFlags = fl;
        return cm_SendLongMessageText(&msg, strlen(text2) + n + 1, buf);
    }
    return -1;
}
/* 0x53dee0, sends the news message 0x11 (message flags 9) with a quick jump to the player `pid`: from `from`, the news
   id `news` (arg[0], as he_ShowNews), the object `obj` and the second id `obj2` (arg[1] / arg[2]; callers pass a
   building id and an item object or -1), the contact name `jump` at +0x68 (at most 0x30 bytes with its NUL, else
   an error), the text flags `tflags` | 2 (| 0x10 when `text2` follows the text behind its NUL) */
void he_MessageBoxQuickjump(int pid, int from, char *text, char tflags, int news, int obj, int obj2, char *jump, char *text2)
{
    AIMSG msg;
    char buf[0x1fa0];
    int n;
    unsigned char fl;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x11;
    msg.to = pid;
    msg.fromId = from;
    msg.flags = 9;
    if (jump != 0) {
        if (strlen(jump) + 1 > 0x30)
            err_Message("he_MessageBoxQuickjump(): contactname is too long!");
        else
            strcpy((char *)&msg.arg[4], jump);  /* 0x68: the contact name, over arg[4..] (up to 0x98) */
    }
    msg.arg[0] = news;
    fl = (unsigned char)tflags | 2;
    msg.arg[1] = obj;
    msg.textFlags = fl;
    msg.arg[2] = obj2;
    strcpy(buf, text);
    n = strlen(text) + 1;
    if (text2 == 0 || *text2 == 0) {
        cm_SendLongMessageText(&msg, n, text);
        return;
    }
    memcpy(buf + n, text2, strlen(text2) + 1);
    fl |= 0x10;
    msg.textFlags = fl;
    cm_SendLongMessageText(&msg, strlen(text2) + n + 1, buf);
}

#include <stdarg.h>
#include <math.h>
void he_Unk55e3a0(int ev);
void he_FindFigurePlace(int p);
void he_SetLifeByAge(int rec, unsigned short num);

void d3t_SetColor(D3OBJECT *obj, unsigned char r, unsigned char g, unsigned char b);
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);  /* 0x447190, d3_interface.h */
char d3if_Show(struct D3OBJECT *obj, char show);
int d3t_GetHeightRange();
char *gm_FindBuildingById(int id);
char * ahm_OwnerChar();
unsigned char *gm_BuildQuery(int n, ...);
unsigned char * gm_BuildNext(void);
int cm_SendLongMessage(char *msg);
int cm_GetSlotState();
void sv_PumpNet();
int gm_GetGoodsValue();
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);
char *gm_Query(void *list, int n, ...);
void cm_BeginGroup(const char *s);
void cm_EndGroup(const char *s);
void cm_SendTableRequest(int *p);
void cm_SendScriptMessage(void *p);
int sprintf(char *buf, const char *fmt, ...);
void mem_Free(void *p);
void err_Message(char *text);

/* HE_SET_CLOCK: the game clock (0x770ae0 ..) into a message at +0x28 .. +0x34, flag byte 2 at +0x36 */
#define HE_CLK(m) do { HM_I(m, 0x28) = U_I((unsigned)&ai_Time); HM_I(m, 0x2c) = U_I((unsigned)&ai_Time.hour); HM_I(m, 0x30) = U_I(((unsigned)&ai_Time + 0x8)); *(short *)((m) + 0x34) = *(short *)((unsigned)&ai_Time + 0xc); HM_B(m, 0x36) = 2; } while (0)

#define he_Jobs (*(AIJOB (*)[0x400])&he_Events)  /* 0x6fab40 */

/* 0x53e040, creates the event record of the message `m`: its type needs both an alloc and a run handler (< 0xa7) and its
   receiver `to` (unless -1) must be a character; takes the first free of the 0x400 records he_Events (raising
   he_EventTop), copies the message into it (the character's index and netId, the new event id ai_NextEventId, the
   words, the time, the flags, the ids and the arguments 0x58.. to 0xac..), lists it in he_QuickList when it is
   shown in the event bar (flags 8, he_QuickTop), calls the alloc handler and counts it (he_EventCount); 0 when it
   cannot
   @deep read whole against the original and rewritten on AIMSG / AIJOB (AIJOB gained
   id/netId/from/words14/text/textSize/exSlot/ids and its 0x14c size): every statement, store and its order
   matches (the handler checks, the free record search, the he_EventTop / he_QuickTop raises, the
   character's id / netId or -1 / 0xffff, the quick list, the id, kind, from, the time struct copy, opts,
   stage/slot/text 0, exSlot -1, the 0xa0-byte argument copy, the alloc handler call, he_EventCount++).
   Left: the two word-copy loops (12 words +0x10 -> +0x14, 8 ids +0x38 -> +0x8c): the original walks two
   pointers with a down counter (lea ecx,[ebp+0x14]; lea eax,[ebx+0x10]; mov edx,0xc; mov edi,[eax]; add
   eax,4; mov [ecx],edi; add ecx,4; dec edx), ours folds them into one pointer plus the difference (mov
   ecx,ebx; sub ecx,eax; mov edi,[edx+eax]) and the register choice of the time copy / the rep movsd
   follows; tried pointer for loops, do-while with --k, the index form (gives [ebx+eax*4+0x10] with an up
   counter): an induction-variable choice of the optimizer */
unsigned char *he_CreateEvent(unsigned char *m)
{
    AIMSG *msg = (AIMSG *)m;
    short *ch = 0;                      /* the AICHAR (ai.h, not included here): id at 0, netId at 4 */
    AIJOB *ev;
    int i, j, k;
    int *d, *s;

    if (msg->type >= 0xa7 || he_AllocHandlers[msg->type] == 0 || he_RunHandlers[msg->type] == 0)
        return 0;
    if (msg->to != -1 && (ch = gm_FindChar(msg->to)) == 0)
        return 0;
    for (i = 0; i < 0x400; i++)
        if (he_Jobs[i].kind == 0)
            break;
    if (i == 0x400)
        return 0;
    ev = &he_Jobs[i];
    if (i > he_EventTop)
        he_EventTop = i;
    if (msg->to == -1) {
        ev->netId = -1;
        ev->chr = 0xffff;
    } else {
        ev->netId = ((int *)ch)[1];
        ev->chr = ch[0];
    }
    if (msg->flags & 8) {
        for (j = 0; j < 0x100; j++)
            if (he_QuickList[j] == 0)
                break;
        if (j == 0x100)
            return 0;
        he_QuickList[j] = (char *)ev;
        if (j > he_QuickTop)
            he_QuickTop = j;
    }
    ev->id = ai_NextEventId;
    ev->kind = msg->type;
    ai_NextEventId++;
    ev->from = msg->fromId;
    d = ev->words14;
    s = &msg->nIds;
    for (k = 0; k < 0xc; k++)
        *d++ = *s++;
    ev->start = msg->time;
    ev->opts = msg->flags;
    ev->stage = 0;
    ev->slot = 0;
    ev->text = 0;
    ev->exSlot = -1;
    d = ev->ids;
    s = msg->ids;
    for (k = 0; k < 8; k++)
        *d++ = *s++;
    memcpy(&ev->u.id, msg->arg, 0xa0);  /* the arguments 0x58..0xf8 */
    he_AllocHandlers[msg->type](ev);
    he_EventCount++;
    return (unsigned char *)ev;
}
/* 0x53e1f0, frees the event record `r`: its message box text (+0x7c), its entry in he_QuickList when it is shown in the
   event bar (flags 8: releases its slot first, shrinks he_QuickTop), counts it off (he_EventCount), clears it and,
   when it was the last used record, shrinks he_EventTop
   @deep read whole against the original and rewritten on AIJOB / HEEVSLOT: the text free, the quick list
   search and its q != ev return, the slot release through q, he_QuickTop's shrink and clamp,
   he_EventCount--, the 0x14c-byte clear, the he_EventTop shrink and clamp all match statement for
   statement. Left: the original computes &he_Jobs[he_EventTop] twice (once for the ev compare at +0xb9,
   again after the he_EventTop >= 0 test at +0xd1: lea edx,[eax+eax*4]; lea ecx,[eax+edx*8]; lea
   edx,[eax+ecx*2]; lea ecx,[edx*4+he_Events]), ours keeps the first (common subexpression); the rest is
   the register renaming that follows (edx / ecx for the clamp). Tried the do-while pointer walk in an if,
   the for over the index, a local copy of he_EventTop and a cast compare on r: all CSE the address */
void he_FreeEvent(int *r)
{
    AIJOB *ev = (AIJOB *)r;
    AIJOB *q;
    int i, t;

    if (ev == 0)
        return;
    if (ev->text != 0) {
        mem_Free(ev->text);
        ev->text = 0;
    }
    if (ev->opts & 8) {
        for (i = 0; i < 0x100; i++)
            if (he_QuickList[i] == (char *)ev)
                break;
        q = (AIJOB *)he_QuickList[i];
        if (q != ev)
            return;
        if (q->slot != 0 && q->slot->win != -1)
            he_ReleaseSlot((int)q);
        t = he_QuickTop;
        he_QuickList[i] = 0;
        if (i == t) {
            for (; t >= 0 && he_QuickList[t] == 0; t--)
                ;
            he_QuickTop = t < 0 ? 0 : t;
        }
    }
    he_EventCount--;
    memset(ev, 0, sizeof(AIJOB));
    if (ev == &he_Jobs[he_EventTop]) {
        for (; he_EventTop >= 0; he_EventTop--)
            if (he_Jobs[he_EventTop].kind != 0)
                break;
        he_EventTop = he_EventTop < 0 ? 0 : he_EventTop;
    }
}
#define he_Cursor (*(AIJOB **)((char *)&he_QuickList + 0xaaa0))  /* 0x7585e0, the record he_NextEvent stands on, 0: the search ended */

/* 0x53e300, the next event record after he_Cursor that passes he_FindEvent's filter (he_FindKind, he_FindId, he_FindChr,
   he_FindFrom; a record whose first filters matched stays matched through the free records after it), up to the last
   used record he_EventTop (an error when that is out of range); 0 at the end
   @deep read whole against the original and rewritten on AIJOB with the typed filter globals: the
   he_EventTop range check and its error, the cursor checks (0 / == last / > last), the he_FindFirst step,
   the filters hoisted (kind byte 0xff, id -1, chr word 0xffff, from -1), the ok flag kept over the free
   records, the step to last and the cursor clear all match instruction for instruction. Left is
   allocation: ours keeps the cursor in eax (returned directly) and saves edi late (push edi before the chr
   load), the original keeps it in esi, pushes esi at the entry and moves it to eax in the shared epilogue
   (+0x3ee mov eax, esi), so eax / esi / edi / edx rename through the function and the last local's slot
   moves [esp+8] -> [esp+0xc]; tried the cursor as a local with returns in place, the single exit, and
   he_Cursor itself throughout: the same two allocations */
char *he_NextEvent(void)
{
    char ok = 0;
    AIJOB *last = 0;

    if (he_EventTop < 0 || he_EventTop >= 0x400)
        err_Message("he_fn_Handlung(): invalid Handlungen_LAST");
    else
        last = &he_Jobs[he_EventTop];
    if (he_Cursor == 0 || he_Cursor == last || he_Cursor > last)
        return 0;
    if (he_FindFirst != 0)
        he_FindFirst = 0;
    else
        he_Cursor++;
    for (;;) {
        if (he_Cursor->kind != 0) {
            if (he_FindKind != 0xff) {
                if ((unsigned char)he_Cursor->kind != he_FindKind)
                    goto mismatch;
                ok = 1;
            }
            if (he_FindId != -1) {
                if (he_Cursor->id != he_FindId)
                    goto mismatch;
                ok = 1;
            }
            if (he_FindChr != 0xffff) {
                if (he_Cursor->chr != he_FindChr)
                    goto mismatch;
                ok = 1;
            }
            if (he_FindFrom != -1) {
                if (he_Cursor->from == he_FindFrom)
                    break;
                goto mismatch;
            }
        }
        if (ok)
            break;
        goto step;
mismatch:
        ok = 0;
step:
        if (he_Cursor >= last) {
            he_Cursor = 0;
            break;
        }
        he_Cursor++;
    }
    return (char *)he_Cursor;
}
/* 0x53e400, starts a search of the event records: n pairs (key, value) set the filter, key 0 the kind (he_FindKind),
   1 the event id (he_FindId), 2 the character index (he_FindChr), 3 the sending building (he_FindFrom), the others
   unset; puts he_Cursor on the first used record (0 when there is none) and returns he_NextEvent's first match */
char *he_FindEvent(int n, ...)
{
    va_list ap;
    int i;

    he_FindKind = 0xff;
    he_FindId = -1;
    he_FindChr = 0xffff;
    he_FindFrom = -1;
    he_FindFirst = 1;
    he_FindUnused = 0;
    va_start(ap, n);
    for (i = n; i > 0; i--) {
        switch ((unsigned char)va_arg(ap, int)) {
        case 0:
            he_FindKind = (unsigned char)va_arg(ap, int);
            break;
        case 1:
            he_FindId = va_arg(ap, int);
            break;
        case 2:
            he_FindChr = (unsigned short)va_arg(ap, int);
            break;
        case 3:
            he_FindFrom = va_arg(ap, int);
            break;
        }
    }
    va_end(ap);
    he_Cursor = he_Jobs;
    for (i = 0; he_Cursor->kind == 0; i++) {
        if (i >= 0x400)
            break;
        he_Cursor++;
    }
    if (i >= 0x400)
        he_Cursor = 0;
    return he_NextEvent();
}
#define he_GfxInfos (*(HEGFXINFO (*)[0x40])((char *)&he_RunHandlers + 0x2a0))  /* 0x7588a0 */

/* 0x53e4e0, spreads the gfx infos over the object `obj` (at most 0x20) on a circle around its position: radius 100 (0 for
   a single one), each at half its own height above obj's top */
void he_SpreadGfxInfos(D3OBJECT *obj)
{
    int cnt = 0;
    HEGFXINFO *list[32];
    D3VECTOR pos;
    float lo, hi, rad, ang;
    int i;
    D3VECTOR at;

    for (i = 0; i < 0x40; i++) {
        if (cnt >= 0x20)
            break;
        if (he_GfxInfos[i].parent == obj)
            list[cnt++] = &he_GfxInfos[i];
    }
    rad = 0.0f;
    if (cnt != 1)
        rad = 100.0f;
    d3_PointToWorld(obj, &obj->pos, &pos);
    d3t_GetHeightRange(obj, &lo, &hi);
    pos.y = hi;
    for (i = 0; i < cnt; i++) {
        d3t_GetHeightRange(list[i]->obj, &lo, &hi);
        ang = i * 6.2831855f / (float)cnt;
        at.x = (float)(sin(ang) * rad + pos.x);
        at.y = (float)((hi - lo) * 0.5 + pos.y);
        at.z = (float)(cos(ang) * rad + pos.z);
        d3_SetObjectPos(list[i]->obj, &at);
    }
}
/* 0x53e5f0, creates the model object `name` of the gfx info `e` at the top of `obj` (its world position, at its
   height's top), parent obj: shadow mask bits 0xc on, saved-with-scene and flag 4 off, kind 5, alpha 0x80, vertex
   colour (0xc0, 0xa0, 0x80), and a two-key path that turns it about y by 64 pi in 0x8c0; then re-spreads obj's
   gfx infos (he_SpreadGfxInfos). The object, 0 when it could not be created */
D3OBJECT *he_CreateGfxObject(HEGFXINFO *e, char *name, D3OBJECT *obj)
{
    D3TEXFLAGS alpha;
    D3ANIMFLAGS f;
    D3VECTOR4 pos;                      /* the engine's padded vector */
    float lo, hi;
    D3OBJECT *o;
    D3ANIMKEY keys[2];

    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    d3t_GetHeightRange(obj, &lo, &hi);
    pos.y = hi;
    o = d3_CreateObject(0, (D3VECTOR *)&pos, (D3VECTOR *)&d3_Origin, name);
    e->obj = o;
    e->parent = obj;
    if (o != 0) {
        o->objectFlags &= ~2;
        o->stateFlags |= 0xc;
        o->flags &= ~4;
        o->kind = 5;
        alpha.all = 0;
        alpha.b[2] = 1;
        alpha.b[0] = 0x80;
        d3_SetObjectAlpha(o, o->info, alpha);
        d3t_SetColor(o, 0xc0, 0xa0, 0x80);
        memset(&f, 0, sizeof(f));
        keys[0].time = 0x8c0;
        keys[1].rot.y = 201.06193f;             /* 64 pi */
        f.b[1] &= ~0x14;
        f.b[8] |= 2;
        f.b[1] &= ~8;
        f.b[0] = 0;
        f.b[1] &= ~1;
        keys[0].rot.x = 0.0f;
        keys[0].rot.y = 0.0f;
        keys[0].rot.z = 0.0f;
        keys[0].pos.x = 0.0f;
        keys[0].pos.y = 0.0f;
        keys[0].pos.z = 0.0f;
        keys[1].rot.x = 0.0f;
        keys[1].rot.z = 0.0f;
        keys[1].pos.x = 0.0f;
        keys[1].pos.y = 0.0f;
        keys[1].pos.z = 0.0f;
        d3anim_CreatePath(o, f, 2, keys);
        he_SpreadGfxInfos(obj);
    }
    return o;
}
/* 0x53e780, adds a gfx info for the event `ev`: the model `name` over the object `parent` in the first free of the 0x40
   he_GfxInfos (he_CreateGfxObject), with the 3D option (ai_GameSwitches byte 3) only; an unknown parent gives the
   message "he_CreateGfxInfo(): invalid parent :%s" (formatted, never shown; %s prints the parent's name at its
   offset 0). Selects scene 0 meanwhile
   @deep read whole against the original and rewritten on HEGFXINFO / AIJOB.gfx / D3OBJECT: the 3D option
   test (ai_GameSwitches byte 3), the free entry search over he_GfxInfos[].ev, the scene switch to 0 and
   back, d3_FindObject(0, 0x40, parent, 0), the dropped sprintf of the parent's name, the ev / parent 0 /
   eventId / ev->gfx stores and the he_CreateGfxObject call all match. Left is allocation: the original
   keeps the index in esi through the loop and turns it into the entry pointer in place (shl esi,4; add
   esi,he_GfxInfos), pushing esi / edi at the entry; ours runs the loop in eax and moves the entry to esi
   (mov esi,eax), pushing esi / edi after the loop (+0x7 / +0x37 hunks, the eax->esi renames); and the
   original folds the he_CreateGfxObject cleanup into the last add esp,0x14 where ours keeps add esp,0xc /
   add esp,8 (+0xa2). Tried early returns, nested ifs, a do-while search, the entry indexed in place of a
   pointer, and the parent itself as the %s argument */
void he_CreateGfxInfo(AIJOB *ev, char *name, D3OBJECT *parent)
{
    int scene = d3_CurrentScene;
    int i;
    HEGFXINFO *e;
    char buf[0x200];

    if (ai_GameSwitches.byte[3] != 0) {
        i = 0;
        do {
            if (he_GfxInfos[i].ev == 0)
                break;
            i++;
        } while (i < 0x40);
        if (i != 0x40) {
            e = &he_GfxInfos[i];
            d3if_SelectScene(0, 1);
            if (d3_FindObject(0, 0x40, (int)parent, 0) == 0) {
                sprintf(buf, "he_CreateGfxInfo(): invalid parent :%s", parent->name);
            } else {
                e->ev = ev;
                e->parent = 0;
                e->eventId = ev->id;
                ev->gfx = e;
                he_CreateGfxObject(e, name, parent);
            }
            d3if_SelectScene(scene, 1);
        }
    }
}
/* 0x53e840, removes the gfx info `e` (frees its object, re-spreads the others on the same parent) */

void he_FreeGfxInfo(int *e)
{
    HEGFXINFO *g = (HEGFXINFO *)e;
    int sv = d3_CurrentScene;
    D3OBJECT *par = g->parent;

    if (g->obj != 0) {
        g->parent = 0;
        d3if_SelectScene(0, 1);
        if (par != 0) {
            if (d3_FindObject(0, 0x1c0, (int)par, 0) != 0)
                he_SpreadGfxInfos(par);
        }
        d3if_Delete(g->obj);
        g->obj = 0;
        d3if_SelectScene(sv, 1);
    }
    g->ev = 0;
    g->eventId = -1;
}
/* 0x53e8b0, adds the gfx info of the event `ev` by its kind when it has none (3D option only): 0x1b a coin, 0x1c / 0x1e
   a gold (saw) hammer, 0x1f / 2 a plus hammer, 0x40 binoculars (the player's), 0x1a blows over the event's building;
   an exclamation mark over the notice board (0x35, the player's tribute collector, scene 0), the tribune (0x6b at stage
   2, scene 0, unless the player's flags1c8 has 0x20000) or the building of a kind 4 event of the player with argc8 */
void he_AddEventGfxInfo(AIJOB *ev)
{
    AIBLD *b;
    unsigned short *ch;
    D3OBJECT *o;

    if (ai_GameSwitches.byte[3] == 0)
        return;
    if (ev->gfx != 0)
        return;
    switch ((unsigned char)ev->kind) {
    case 0x1b:
        b = (AIBLD *)gm_FindBuildingById(ev->bldId);
        if (b != 0)
            he_CreateGfxInfo(ev, "he_muenze", (D3OBJECT *)b->object);
        break;
    case 0x1c:
        b = (AIBLD *)gm_FindBuildingById(ev->bldId);
        if (b != 0 && b->object != 0)
            he_CreateGfxInfo(ev, "he_hammer_gold", (D3OBJECT *)b->object);
        break;
    case 0x1e:
        b = (AIBLD *)gm_FindBuildingById(ev->bldId);
        if (b != 0 && b->object != 0)
            he_CreateGfxInfo(ev, "he_saege_hammer_gold", (D3OBJECT *)b->object);
        break;
    case 0x1f:
        b = (AIBLD *)gm_FindBuildingById(ev->bldId);
        if (b != 0 && b->object != 0)
            he_CreateGfxInfo(ev, "he_plus_hammer", (D3OBJECT *)b->object);
        break;
    case 2:
        b = (AIBLD *)gm_FindBuildingById(ev->from);
        if (b != 0 && b->object != 0)
            he_CreateGfxInfo(ev, "he_plus_hammer", (D3OBJECT *)b->object);
        break;
    case 0x40:
        b = (AIBLD *)gm_FindBuildingById(ev->bldId);
        if (b != 0 && b->object != 0 && ev->chr == ai_PlayerId)
            he_CreateGfxInfo(ev, "he_fernglas", (D3OBJECT *)b->object);
        break;
    case 0x35:
        ch = (unsigned short *)gm_FindChar(ev->idbc);
        if (ch != 0 && *ch == ai_PlayerId && d3_CurrentScene == 0) {
            o = d3_FindObject(0, 0x40, 0, "ob_SCHWARZES_BRETT");
            if (o != 0)
                he_CreateGfxInfo(ev, "he_ausrufezeichen", o);
        }
        break;
    case 0x6b:
        if (ev->stage == 2 && d3_CurrentScene == 0 && (ai_CharInts[ai_PlayerId][AICHAR_FLAGS1C8] & 0x20000) == 0) {
            o = d3_FindObject(0, 0x40, 0, "ob_TRIBUENE");
            if (o != 0)
                he_CreateGfxInfo(ev, "he_ausrufezeichen", o);
        }
        break;
    case 0x1a:
        b = (AIBLD *)gm_FindBuildingById(ev->bldd0);
        if (b != 0 && b->object != 0)
            he_CreateGfxInfo(ev, "he_schlaege", (D3OBJECT *)b->object);
        break;
    case 4:
        if (ev->chr == ai_PlayerId && ev->argc8 != 0) {
            b = (AIBLD *)gm_FindBuildingById(ev->idbc);
            if (b != 0 && b->object != 0)
                he_CreateGfxInfo(ev, "he_ausrufezeichen", (D3OBJECT *)b->object);
        }
        break;
    }
}
/* 0x53eba0, removes the gfx infos whose parent changed or when the 3D option is off */

void he_RemoveStaleGfxInfos(void)
{
    HEGFXINFO *p;

    for (p = he_GfxInfos; (int)p < (int)(he_GfxInfos + 0x40); p++)
        if (p->ev != 0 && (((AIJOB *)p->ev)->id != p->eventId || ai_GameSwitches.byte[3] == 0))
            he_FreeGfxInfo((int *)p);
}
/* 0x53ebe0, removes the gfx infos of the object `obj` */

void he_RemoveGfxInfos(int obj)
{
    int *p = (int *)((unsigned)&he_RunHandlers + 0x2a0);

    do {
        if (p[2] == obj)
            he_FreeGfxInfo(p);
        p += 4;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x6a0));
}
/* 0x53ec10, calls 0x4471b0 for the gfx infos of the object `obj` */

void he_ShowObjectInfos(int obj, int a)
{
    int *p = (int *)((unsigned)&he_RunHandlers + 0x2ac);

    do {
        if (p[-1] == obj)
            d3if_Show(*p, a);
        p += 4;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x6ac));
}
/* 0x53ec40, makes the event `r` due now (stage -2, end = the game clock) when its character is gone (another netId
   or kind 0xf) or dead (health 0) for the kinds of the list */

void he_RecheckEventPerson(unsigned char *r)
{
    AIJOB *ev = (AIJOB *)r;
    unsigned short us = ev->chr;

    if (us == 0xffff)
        return;
    if (ai_CharsT[us].netId != ev->netId || ai_CharsT[us].kind == 0xf)
        goto reset;
    if (ai_CharsT[us].health > 0)
        return;
    switch ((unsigned char)ev->kind) {
    case 0x16: case 0x18: case 0x19: case 0x1a: case 0x2d: case 0x2e: case 0x2f: case 0x35:
    case 0x36: case 0x37: case 0x38: case 0x41: case 0x45: case 0x46: case 0x47: case 0x4a:
    case 0x4b: case 0x5e: case 0x5f: case 0x60: case 0x69: case 0x6a: case 0x6f: case 0x7e:
        break;
    default:
        return;
    }
reset:
    ev->stage = -2;
    ev->end = ai_Time;
}
/* 0x53ed40, runs 0x53ec40 on every record of the player *pl (type 2 filter) */

void he_RecheckPersonEvents(unsigned short *pl)
{
    int iv = (int)he_FindEvent(1, 2, *pl);

    while (iv != 0) {
        he_RecheckEventPerson((unsigned char *)iv);
        iv = (int)he_NextEvent();
    }
}
/* 0x53ed70, the per-frame run of the event records: updates the gfx infos, then calls the run handler (0x758600) of every record (not 0x18 flagged) that is due */

void he_RunEvents(void)
{
    int iv, i;
    AIJOB *pb;

    he_RemoveStaleGfxInfos();
    i = 0;
    if (he_EventTop >= 0) {
        pb = he_Jobs;
        do {
            if ((int)pb >= (int)(he_Jobs + 0x400))
                return;
            if (pb->kind != 0 && (pb->opts & 0x18) == 0) {
                he_AddEventGfxInfo(pb);
                iv = gm_CompareTime(&ai_Time, &pb->end);
                if (iv > 0 && (unsigned char)pb->kind < 0xa7) {
                    he_RecheckEventPerson((unsigned char *)pb);
                    he_RunHandlers[(unsigned char)pb->kind](pb);
                }
            }
            i++;
            pb++;
        } while (i <= he_EventTop);
    }
}
/* 0x53edf0, runs the run handler of the quick list records (0x74db40) */

void he_RunQuickList(void)
{
    unsigned char *pb;
    int i = 0;

    if (he_QuickTop >= 0) {
        do {
            if (i >= 0x100)
                return;
            pb = (unsigned char *)he_QuickList[i];
            if (pb != 0 && *pb < 0xa7) {
                he_RecheckEventPerson(pb);
                he_RunHandlers[*pb](pb);
            }
            i++;
        } while (i <= he_QuickTop);
    }
}
/* 0x53ee40, runs the run handler of the records with the option 0x10 (runs every frame) */

void he_RunEveryFrameEvents(void)
{
    AIJOB *pb;
    int i = 0;

    if (he_EventTop >= 0) {
        pb = he_Jobs;
        do {
            if ((int)pb >= (int)(he_Jobs + 0x400))
                return;
            if (pb->kind != 0 && (pb->opts & 0x10) != 0 && (unsigned char)pb->kind < 0xa7) {
                he_RecheckEventPerson((unsigned char *)pb);
                he_RunHandlers[(unsigned char)pb->kind](pb);
            }
            i++;
            pb++;
        } while (i <= he_EventTop);
    }
}
/* 0x53eea0, counts the time to live of the 0x100 carried-goods records (gm_CarryRecs) down, freeing (ref 0) each one
   that reaches 0 */
void he_AgeCarryRecs(void)
{
    int i;

    for (i = 0; i < 0x100; i++) {
        if (--gm_CarryRecs[i].ttl == 0)
            gm_CarryRecs[i].ref = 0;
    }
}
/* 0x53eec0, registers the AP event `name` of the character `key` with `income` and `cost` in the first free (no
   income, cost or name) of the 0x100 he_ApEvents, for 2 rounds; 0 when all are used */
HEAPEVENT *he_AddApEvent(int key, char *name, int income, int cost)
{
    int i;
    HEAPEVENT *e;

    for (i = 0; i < 0x100; i++) {
        if (he_ApEvents[i].income == 0 && he_ApEvents[i].cost == 0 && he_ApEvents[i].name[0] == 0)
            break;
    }
    if (i >= 0x100) {
        err_Message("he_RegisterApEvent(): Too many ApEvents...");
        return 0;
    }
    e = &he_ApEvents[i];
    e->key = key;
    e->income = income;
    e->cost = cost;
    e->counter = 2;
    strncpy(e->name, name, 0x3f);
    return e;
}
/* 0x53ef50, counts the rounds of the he_ApEvents down; an expired one loses its income, cost and name (is free) */
void he_CountDownApEvents(void)
{
    int i;

    for (i = 0; i < 0x100; i++) {
        if (--he_ApEvents[i].counter <= 0) {
            he_ApEvents[i].income = 0;
            he_ApEvents[i].cost = 0;
            he_ApEvents[i].name[0] = 0;
        }
    }
}
/* 0x53ef80, the sum of the values (+8) of the AP events with key (+4) `key` */

int he_SumApEvents(int key)
{
    int sum = 0;
    int *p = (int *)((unsigned)&he_RunHandlers + 0x6a8);

    do {
        if (*p != 0 && p[-1] == key)
            sum += *p;
        p += 0x14;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x56a8));
    return sum;
}
/* 0x53efb0, the sum of the values (+0xc) of the AP events with key (+4) `key` */

int he_SumApEventCosts(int key)
{
    int sum = 0;
    int *p = (int *)((unsigned)&he_RunHandlers + 0x6ac);

    do {
        if (*p != 0 && p[-2] == key)
            sum += *p;
        p += 0x14;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x56ac));
    return sum;
}

/* the 0x6d / 0x6b / 0x7a events of the current player when none exists yet (one message of 0xf8 bytes) */
#define HE_ONCE(typ) \
    AIMSG m; \
    if (he_FindEvent(1, 0, typ) == 0) { \
        memset(&m, 0, sizeof(m)); \
        m.type = typ; \
        m.fromId = -1; \
        m.flags = 2; \
        m.to = ai_CharInts[ai_PlayerId][0]; \
        m.time = ai_Time; \
        cm_SendLongMessage((char *)&m); \
    }

/* 0x53efe0, posts the event 0x6d (once) */
void he_PostEvent6d(void)
{
    HE_ONCE(0x6d)
}
/* 0x53f070, posts the event 0x6b (once) */

void he_PostEvent6b(void)
{
    HE_ONCE(0x6b)
}
/* 0x53f100, posts the event 0x7a (once) */

void he_PostEvent7a(void)
{
    HE_ONCE(0x7a)
}
/* 0x53f190, posts the tavern event 0x2b for every building of kind 5 / 7 that has none yet (the message's fromId is the
   building); once one building has its event, the rest of the walk posts nothing */
void he_PostEvent2b(void)
{
    AIMSG m;
    int found = 0;
    AIJOB *ev;
    AIBLD *b;

    memset(&m, 0, sizeof(m));
    for (b = (AIBLD *)gm_BuildQuery(1, 5, 7); b != 0; b = (AIBLD *)gm_BuildNext()) {
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x2b); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->from == b->id) {
                found = 1;
                break;
            }
        }
        if (!found) {
            m.type = 0x2b;
            m.time = ai_Time;
            m.flags = 2;
            m.to = he_PlayerCh->netId;
            m.fromId = b->id;
            m.tavern.mode = 0;
            cm_SendLongMessage((char *)&m);
        }
    }
}
/* 0x53f270, posts the events 0x86 and 0xa2 to the player's character, each when none exists yet */
void he_PostPlayerEvents86a2(void)
{
    AIMSG m;

    if (he_FindEvent(1, 0, 0x86) == 0) {
        memset(&m, 0, sizeof(m));
        m.type = 0x86;
        m.to = he_PlayerCh->netId;
        m.fromId = -1;
        m.flags = 2;
        m.time = ai_Time;
        cm_SendLongMessage((char *)&m);
    }
    if (he_FindEvent(1, 0, 0xa2) == 0) {
        memset(&m, 0, sizeof(m));
        m.type = 0xa2;
        m.to = he_PlayerCh->netId;
        m.fromId = -1;
        m.flags = 2;
        m.time = ai_Time;
        cm_SendLongMessage((char *)&m);
    }
}
/* 0x53f370, posts the event 0x98 for every building of kind 0x1e (workshop) that has none (he_FindEvent key 3, the
   message's fromId: the building) and an owner (ahm_OwnerChar), to that owner */
void he_PostWorkshopEvents98(void)
{
    AIMSG m;
    AIBLD *b;
    AICHAR *o;

    for (b = (AIBLD *)gm_BuildQuery(1, 5, 0x1e); b != 0; b = (AIBLD *)gm_BuildNext()) {
        if (he_FindEvent(2, 0, 0x98, 3, b->id) == 0 && (o = (AICHAR *)ahm_OwnerChar(b)) != 0) {
            memset(&m, 0, sizeof(m));
            m.type = 0x98;
            m.to = o->netId;
            m.fromId = b->id;
            m.flags = 2;
            m.time = ai_Time;
            cm_SendLongMessage((char *)&m);
        }
    }
}
/* 0x53f430, for every inn building (type 0x16) sends the table request "leav" to the four seats of its table (the
   object 0x12d) whose figure is gone, dead (health 0) or a corpse (kind 0xf) ("stammtisch supervisor") */

void he_InnStaffLeave(void)
{
    AIBLD *f;
    AIOBJ *r;
    AICHAR *o;
    int k;
    int *p;
    int arg[3];

    f = (AIBLD *)gm_BuildQuery(1, 5, 0x16);
    while (f != 0) {
        r = (AIOBJ *)gm_Query((void *)f->objList, 1, 0, 0x120);
        if (r != 0 && (r = (AIOBJ *)gm_Query((void *)r->list, 1, 0, 0x12d)) != 0) {
            p = r->seats;
            for (k = 4; k != 0; k--) {
                if (*p != -1 && ((o = (AICHAR *)gm_FindChar(*p)) == 0 || o->health == 0 || o->kind == 0xf)) {
                    arg[2] = f->id;
                    arg[1] = *p;
                    arg[0] = 0x6c656176;
                    cm_BeginGroup("stammtisch supervisor");
                    cm_SendTableRequest(arg);
                    cm_EndGroup("stammtisch supervisor");
                }
                p++;
            }
        }
        f = (AIBLD *)gm_BuildNext();
    }
}
/* 0x53f500, for every inn building (type 0x16) whose job table (object 0x12c) was started more than one year ago sends
   the script message "cler" to clear its dark corner ("clear dunkle ecke") */

void he_ClearInnDarkCorner(void)
{
    AIBLD *f;
    AIOBJ *r;
    int arg[4];

    f = (AIBLD *)gm_BuildQuery(1, 5, 0x16);
    while (f != 0) {
        r = (AIOBJ *)gm_Query((void *)f->objList, 1, 0, 0x120);
        if (r != 0 && (r = (AIOBJ *)gm_Query((void *)r->list, 1, 0, 300)) != 0
                && ai_Time.year - ((HEJOBTABLE *)r)->year > 1) {
            arg[2] = f->id;
            arg[0] = 0x636c6572;
            arg[1] = -1;
            arg[3] = -1;
            cm_BeginGroup("clear dunkle ecke");
            cm_SendScriptMessage(arg);
            cm_EndGroup("clear dunkle ecke");
        }
        f = (AIBLD *)gm_BuildNext();
    }
}
/* 0x53f5b0, tops up the money of the characters of kind 5 and 4 (kind 2 when the year % 4 is its net id & 3) to the
   level 0x640 * (difficulty * 5 + 0x14), from the own player (no source); after 0x20 transfers it waits for the last
   one's send slot */
void he_TopUpCharMoney(void)
{
    int i, level, have, n, slot = -1;

    if (ai_Time.year % (ai_Time.year / 10 + 1) == 0) {
        level = (ai_Difficulty * 5 + 0x14) * 0x640;
        n = 0;
        for (i = 0; i < 768; i++) {
            if (ai_CharsT[i].kind == 5) {
                have = gm_GetGoodsValue((char *)&ai_CharsT[i]);
                if (have < level) {
                    slot = cm_SendMoveGoods(ai_CharsT[i].netId, -1, ai_OwnPlayer, level - have);
                    n++;
                }
            } else if (ai_CharsT[i].kind == 4 || (ai_CharsT[i].kind == 2 && ai_Time.year % 4 == (int)(ai_CharsT[i].netId & 3))) {
                have = gm_GetGoodsValue((char *)&ai_CharsT[i]);
                if (have < level) {
                    slot = cm_SendMoveGoods(ai_CharsT[i].netId, -1, ai_OwnPlayer, level - have);
                    n++;
                }
            }
            if (n >= 0x20 && slot != -1) {
                do {
                    if (cm_GetSlotState(slot) != 0)
                        break;
                    sv_PumpNet();
                } while (1);
                slot = -1;
                n = 0;
            }
        }
    }
}

int gm_TitleNumber();
int gm_EquipmentValue(unsigned char *b, int level);
int gm_BuildingKindValue();
int cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);
int cm_GetSlotValue(int h);
float util_RandomF();

/* the ai_Blds walk of he_SpawnFounders: i is the first building of the kind k that is the player's or nobody's
   (owner 0xffff), 0x100 when there is none (the original reuses the option loop's i for it) */
#define HE_FIND_OWN(k) \
    for (i = 0, b = ai_BldsT; i < 0x100; i++, b++) { \
        if (b->type != 0) { \
            bt = &ai_BTypesT[b->type]; \
            if (bt->kind == (k) && (b->owner == 0xffff || b->owner == *ai_PlayerChar)) \
                break; \
        } \
    }

/* that building b (of the type bt)'s offer: twice its equipment value, the title of the group at its type's level */
#define HE_OFFER_FOUND(group) \
    do { \
        amt = gm_EquipmentValue((unsigned char *)b, 100) * 2; \
        title = gm_TitleNumber(group, bt->cap247 * 2 - 1); \
    } while (0)

/* the offer without a building: twice the value of the building kind bk, the title (group, rank) */
#define HE_OFFER_KIND(bk, group, rank) \
    do { \
        amt = gm_BuildingKindValue(bk) * 2; \
        title = gm_TitleNumber(group, rank); \
    } while (0)

/* creates the founder (a kind 4 character with the title), waits for its send slot and, when it was created, gives it
   amt from the own player and sets its field 0xd to 2 (and what `extra` adds) */
#define HE_SEND_FOUNDER(rnd, extra) \
    do { \
        slot = cm_SendCreateCharacter(4, -1, -1, util_Random(4) + 0xf, -1, title, 0, rnd); \
        if (slot != -1) \
            do { \
                if (cm_GetSlotState(slot) != 0) \
                    break; \
                sv_PumpNet(); \
            } while (1); \
        if (cm_GetSlotState(slot) == 1) { \
            two = 2; \
            c = (AICHAR *)cm_GetSlotValue(slot); \
            cm_SendMoveGoods(c->netId, -1, ai_OwnPlayer, amt); \
            cm_BeginFieldBlock((char *)c, c->netId); \
            cm_AddFieldDelta(1, 1, 0xd, &two); \
            extra; \
            cm_SendFieldDeltas(); \
        } \
    } while (0)

const float he_FounderOdds[5] = { 4.0f, 3.0f, 2.5f, 1.5f, 1.0f };  /* 0x5eebc4, the odds of a founder by the difficulty: x / (x + odds) (he_SpawnFounders, its only reader; first in he.c's .rdata literals) */

/* 0x53f6b0, the founders of the round (main_BeginPlayerRound): tops up the money of the characters (he_TopUpCharMoney),
   counts the buildings per kind (all, and the player's or nobody's; of kind 7 the others' by their owner's b00c) and
   by the year % 5 looks at a group of building kinds (1: 4 / 0x13 / 0x10, 2: 0x12 / 0x15 / 0x14 / 0xe, 3: 8 / 0x16,
   0: 9 / 7 / 5, 4: 0x20 / 0x1f / 0x1d); the kinds the town has few of (fewer than 3, or the player's share at least
   a half) are eligible, the missing ones always, else one at random when util_RandomF() <= x / (x + odds[level]),
   x = year / 4 + 1; for each chosen kind it creates a founder character paid twice the value of the player's first
   building of the kind (or of the building kind) (reasoned: the character of kind 4 that he_TopUpCharMoney keeps in
   money, its title from the kind's group)
   @deep Rewritten in full from the original 0x53f6b0..0x540c8b (all five year%5 themes read instruction by
   instruction: the per-kind counting over ai_Blds with the kind-7 owner b00c split, the eligible / missing
   masks, the gate util_RandomF() > x/(x+he_FounderOdds[lvl]), the random pick loops, the per-option search
   that reuses the option loop's i, the shared founder send with the 0xd field and theme 0's 0xc field, the
   tail jmp to cm_SendFieldDeltas of theme 1). No call, store or reference differs (no ref:/differs: lines,
   calls equal); ours 1764 vs 1751 instructions. What is left is allocation / layout: (1) the loop exits of
   themes 2, 3, 0: ours 'inc ebx; cmp ebx,4; jl top' + a duplicated pop/ret epilogue, the original 'jge
   0x540c80; reload esi/edi; jmp top' (tried for(;;) + goto next + if (++i >= 4) break, and a switch (t):
   the same, the switch adds a jump table); (2) the lvl clamp's 'mov ecx,4' block placed out of line at
   0x53f7af in the original, inline in ours (tried if (lvl > 4) / if (lvl < 5) {..} else / if (lvl <= 4)
   {..} else); (3) the counted kind k spilled to [esp+0x20] in ours ('mov byte ptr [esp+0x20], dl'), kept
   in edx by the original; (4) theme 0 keeps n7a / n7b in esi / edx reloaded at each loop top, ours reads
   them from the frame and caches the constant 1 in edx (hunks 0x5404f6, 0x5405c8); (5) cnt[0x14] /
   cnt[0x16] / ai_PlayerChar cached in registers on one side, frame / reload on the other (hunks 0x53fe09,
   0x53fe5b, 0x53ff08, 0x5401a7, 0x540257); (6) a 'test esi, esi' after the dec of the theme 1 / 4 pick
   counters (tried for (n = 0; n < 3; n++): worse); (7) pick = 0 stored from the zeroed mask register in
   the original ('mov [esp+0x10], ebx'), as an immediate in ours. */
void he_SpawnFounders(void)
{
    int cnt[0x21];              /* the buildings per kind */
    int own[0x21];              /* the player's or nobody's per kind */
    int lvl, x, i, n, r, t;
    int n7a = 0, n7b;           /* the others' buildings of kind 7: owner's b00c 0 / not 0 */
    float gate, r0, r1, r2, r3;
    int d4, d13, d10, mask, zero, pick, elig, bit;
    int amt, title, slot, two, fl, rnd;
    unsigned char k;
    AIBLD *b;
    AIBTYPE *bt;
    AICHAR *c;

    n7b = 0;
    lvl = ai_Difficulty;
    if (lvl <= 4) {
        if (lvl <= 0)
            lvl = 0;
    } else
        lvl = 4;
    x = ai_Time.year / 4 + 1;
    gate = (float)x / ((float)x + he_FounderOdds[lvl]);
    he_TopUpCharMoney();
    memset(cnt, 0, sizeof(cnt));
    memset(own, 0, sizeof(own));
    for (i = 0, b = ai_BldsT; i < 0x100; i++, b++) {
        if (b->type != 0) {
            bt = &ai_BTypesT[b->type];
            k = bt->kind;
            if (k < 0x21) {
                cnt[k]++;
                if (b->owner == 0xffff || b->owner == *ai_PlayerChar)
                    own[k]++;
                else if (k == 7) {
                    if (ai_CharsT[b->owner].b00c == 0)
                        n7a++;
                    else
                        n7b++;
                }
            }
        }
    }
    t = ai_Time.year % 5;
    if (t == 1) {
        /* the others' buildings of the kinds 4 / 0x13 / 0x10 */
        d10 = cnt[0x10] - own[0x10];
        d4 = cnt[4] - own[4];
        d13 = cnt[0x13] - own[0x13];
        pick = 0;
        mask = 0;
        zero = 0;
        if (d4 < 3)
            mask |= 1;
        if (d4 == 0)
            zero |= 1;
        if (d13 < 3)
            mask |= 2;
        if (d13 == 0)
            zero |= 2;
        if (d10 < 3)
            mask |= 4;
        if (d10 == 0)
            zero |= 4;
        if (zero != 0) {
            r = util_RandomU(3);
            for (n = 3; n != 0; n--) {
                if (mask & (1 << r)) {
                    pick = mask = 1 << r;
                    break;
                }
                r = (r + 1) % 3;
            }
        } else {
            if (util_RandomF() > gate)
                return;
            if (d4 + d10 > d13 * 2 && (mask & 2))
                pick = 2;
            else if (d4 + d10 < d13 * 2) {
                if (d4 < d10) {
                    if (mask & 1)
                        pick = 1;
                } else if (mask & 4)
                    pick = 4;
            } else
                pick = 1 << util_Random(3);
        }
        if (!(mask & pick))
            return;
        if (pick == 1) {
            if (cnt[4] < 3) {
                amt = gm_BuildingKindValue(0xb) * 2;
                title = 6;
            } else {
                HE_FIND_OWN(4);
                if (i >= 0x100)
                    return;
                HE_OFFER_FOUND(0xb);
            }
        } else if (pick == 2) {
            if (cnt[0x13] < 3) {
                amt = gm_BuildingKindValue(0x2c) * 2;
                title = 0x39;
            } else {
                HE_FIND_OWN(0x13);
                if (i >= 0x100)
                    return;
                HE_OFFER_FOUND(10);
            }
        } else {
            title = 0x2d;
            amt = gm_BuildingKindValue(0x27) * 2;
        }
        HE_SEND_FOUNDER(0, (void)0);
    } else if (t == 2) {
        /* the kinds 0x12 / 0x15 / 0x14 / 0xe */
        r0 = cnt[0x12] > 0 ? (float)own[0x12] / cnt[0x12] : 0.0f;
        r1 = cnt[0x15] > 0 ? (float)own[0x15] / cnt[0x15] : 0.0f;
        r2 = cnt[0x14] > 0 ? (float)own[0x14] / cnt[0x14] : 0.0f;
        r3 = cnt[0xe] > 0 ? (float)own[0xe] / cnt[0xe] : 0.0f;
        elig = 0;
        zero = 0;
        if (cnt[0x12] < 3 || r0 >= 0.5f) {
            elig |= 1;
            if (cnt[0x12] == 0)
                zero |= 1;
        }
        if (cnt[0x15] < 3 || r1 >= 0.5f) {
            elig |= 2;
            if (cnt[0x15] == 0)
                zero |= 2;
        }
        if (cnt[0x14] < 3 || r2 >= 0.5f) {
            elig |= 4;
            if (cnt[0x14] == 0)
                zero |= 4;
        }
        if (cnt[0xe] < 3 || r3 >= 0.5f) {
            elig |= 8;
            if (cnt[0xe] == 0)
                zero |= 8;
        }
        if (zero != 0)
            elig = zero;
        else {
            if (util_RandomF() > gate)
                return;
            r = util_RandomU(4);
            for (n = 4; n != 0; n--, r = (r + 1) % 4) {
                if (elig & (1 << r)) {
                    elig = 1 << r;
                    break;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            if (elig == 0)
                return;
            bit = elig & (1 << i);
            if (bit == 1) {
                if (cnt[0x12] < 3)
                    HE_OFFER_KIND(0x29, 1, 1);
                else {
                    HE_FIND_OWN(0x12);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(1);
                }
            } else if (bit == 2) {
                if (cnt[0x15] < 3)
                    HE_OFFER_KIND(0x32, 3, 1);
                else {
                    HE_FIND_OWN(0x15);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(3);
                }
            } else if (bit == 4) {
                if (cnt[0x14] < 3)
                    HE_OFFER_KIND(0x2f, 2, 1);
                else {
                    HE_FIND_OWN(0x14);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(2);
                }
            } else if (bit == 8) {
                if (cnt[0xe] < 3)
                    HE_OFFER_KIND(0x21, 5, 1);
                else {
                    HE_FIND_OWN(0xe);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(5);
                }
            } else
                continue;
            HE_SEND_FOUNDER(util_Random(2), (void)0);
        }
    } else if (t == 3) {
        /* the kinds 8 / 0x16 */
        r0 = cnt[8] > 0 ? (float)own[8] / cnt[8] : 0.0f;
        r1 = cnt[0x16] > 0 ? (float)own[0x16] / cnt[0x16] : 0.0f;
        elig = 0;
        zero = 0;
        if (cnt[8] < 3 || r0 >= 0.5f) {
            elig |= 1;
            if (cnt[8] == 0)
                zero |= 1;
        }
        if (cnt[0x16] < 3 || r1 >= 0.5f) {
            elig |= 2;
            if (cnt[0x16] == 0)
                zero |= 2;
        }
        if (zero != 0)
            elig = zero;
        else {
            if (util_RandomF() > gate)
                return;
            r = util_RandomU(2);
            for (n = 2; n != 0; n--, r = (r + 1) % 2) {
                if (elig & (1 << r)) {
                    elig = 1 << r;
                    break;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            if (elig == 0)
                return;
            bit = elig & (1 << i);
            if (bit == 1) {
                if (cnt[8] < 3)
                    HE_OFFER_KIND(0x17, 6, 1);
                else {
                    HE_FIND_OWN(8);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(6);
                }
            } else if (bit == 2) {
                if (cnt[0x16] < 3)
                    HE_OFFER_KIND(0x35, 4, 1);
                else {
                    HE_FIND_OWN(0x16);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(4);
                }
            } else
                continue;
            HE_SEND_FOUNDER(util_Random(2), (void)0);
        }
    } else if (t == 0) {
        /* the kinds 9 / 7 / 5; kind 7 also when the others' kind 7 buildings are all of one b00c class */
        r0 = cnt[9] > 0 ? (float)own[9] / cnt[9] : 0.0f;
        r2 = cnt[5] > 0 ? (float)own[5] / cnt[5] : 0.0f;
        elig = 0;
        zero = 0;
        fl = util_RandomU(2);
        if (cnt[9] < 2 || r0 >= 0.5f) {
            elig |= 1;
            if (cnt[9] == 0)
                zero |= 1;
        }
        if (cnt[7] < 2 || (cnt[7] < 3 && (n7a == 0 || n7b == 0))) {
            elig |= 2;
            if (cnt[7] == 0)
                zero |= 2;
        }
        if (cnt[5] < 2 || r2 >= 0.5f) {
            elig |= 4;
            if (cnt[5] == 0)
                zero |= 4;
        }
        if (zero != 0)
            elig = zero;
        else {
            if (util_RandomF() > gate)
                return;
            r = util_RandomU(3);
            for (n = 3; n != 0; n--, r = (r + 1) % 3) {
                if (elig & (1 << r))
                    break;
            }
        }
        for (i = 0; i < 3; i++) {
            if (elig == 0)
                return;
            bit = elig & (1 << i);
            if (bit == 1) {
                if (cnt[9] < 2)
                    HE_OFFER_KIND(0x1a, 8, 1);
                else {
                    HE_FIND_OWN(9);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(8);
                }
            } else if (bit == 2) {
                /* the field 0xc: the b00c class of the fewer others' kind 7 buildings (random when even) */
                if (n7a < n7b)
                    fl = 0;
                else if (n7a > n7b)
                    fl = 1;
                for (i = 0, b = ai_BldsT; i < 0x100; i++, b++) {
                    if (b->type != 0) {
                        bt = &ai_BTypesT[b->type];
                        if (bt->kind == 7 && (b->owner == 0xffff || b->owner == *ai_PlayerChar)) {
                            HE_OFFER_FOUND(7);
                            break;
                        }
                    }
                }
                if (i >= 0x100) {
                    if (cnt[7] < 2)
                        HE_OFFER_KIND(0x14, 7, 1);
                    else
                        HE_OFFER_KIND(0x15, 7, 3);
                }
            } else if (bit == 4) {
                if (cnt[5] < 1)
                    HE_OFFER_KIND(0xe, 9, 1);
                else {
                    HE_FIND_OWN(5);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(9);
                }
            } else
                continue;
            HE_SEND_FOUNDER(util_Random(2), cm_AddFieldDelta(1, 1, 0xc, &fl));
        }
    } else if (t == 4) {
        /* the kinds 0x20 / 0x1f / 0x1d */
        r0 = cnt[0x20] > 0 ? (float)own[0x20] / cnt[0x20] : 0.0f;
        r1 = cnt[0x1f] > 0 ? (float)own[0x1f] / cnt[0x1f] : 0.0f;
        r2 = cnt[0x1d] > 0 ? (float)own[0x1d] / cnt[0x1d] : 0.0f;
        elig = 0;
        zero = 0;
        if (cnt[0x20] < 3 || r0 >= 0.5f) {
            elig |= 1;
            if (cnt[0x20] == 0)
                zero |= 1;
        }
        if (cnt[0x1f] < 3 || r1 >= 0.5f) {
            elig |= 2;
            if (cnt[0x1f] == 0)
                zero |= 2;
        }
        if (cnt[0x1d] < 3 || r2 >= 0.5f) {
            elig |= 4;
            if (cnt[0x1d] == 0)
                zero |= 4;
        }
        if (zero != 0)
            elig = zero;
        else {
            if (util_RandomF() > gate)
                return;
            r = util_RandomU(3);
            for (n = 3; n != 0; n--) {
                if (elig & (1 << r)) {
                    elig = 1 << r;
                    break;
                }
                r = (r + 1) % 3;
            }
        }
        for (i = 0; i < 3; i++) {
            if (elig == 0)
                return;
            rnd = 0;
            bit = elig & (1 << i);
            if (bit == 1) {
                rnd = util_RandomU(2);
                if (cnt[0x20] < 3)
                    HE_OFFER_KIND(0x51, 0xf, 1);
                else {
                    HE_FIND_OWN(0x20);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(0xf);
                }
            } else if (bit == 2) {
                if (cnt[0x1f] < 3)
                    HE_OFFER_KIND(0x4e, 0xe, 1);
                else {
                    HE_FIND_OWN(0x1f);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(0xe);
                }
            } else if (bit == 4) {
                if (cnt[0x1d] < 3)
                    HE_OFFER_KIND(0x48, 0xd, 1);
                else {
                    HE_FIND_OWN(0x1d);
                    if (i >= 0x100)
                        return;
                    HE_OFFER_FOUND(0xd);
                }
            } else
                continue;
            HE_SEND_FOUNDER(rnd, (void)0);
        }
    }
}

float util_RandomF();
int gv_GetTableRecord(int i, int *out);
int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v);
void cm_BeginFieldBlock(char *o, int id);
void cm_SendFieldDeltas(void);
void txt_Format(char *out, int id0, ...);
int cm_SendExSetHE(); /* @original: the callers here push the key as an int and read the return (byte-matched he_EndEventNow / he_AllocQuarrel / he_Unk548110 / he_Unk5725b0), against cm.h's void (unsigned char, char *) view */

/* 0x540c90, from the year 8 on, in 1 of 4 calls, when no event 0x67 runs: posts the message 0x67 about one of the
   player's buildings of the kinds 0x20 / 0x1e / 0x1f (the first of each kind, gm_BuildQuery; one of them at random):
   arg[0] its id, arg[1] its unk69 plus ((5 - the AI decision 7's cur) * 0.1 + 0.7) * 0.5 * (unk65 + unk69), or, when that
   is not positive, 1 .. 2 times its equipment value
   @deep byte (rewritten from the original 0x540c90..0x540e3f: no memset of the message, the AI decision
   7's cur, the amount sum * (x * 0.5), the equipment fallback through an int). Q-only handover, Q2:
   AIBLD.unk65 / unk69 only. Read: their users in every unit (ai.c steers unk65 towards AIPOS.unk18 and
   sends it as field 0x65; gv.c / f3gm.c read +0x65 as a record pointer; the inn code reads +0x65 / +0x69 /
   +0x6b as guest counters; main.c zeroes unk69 for kind 7 every round; unk_4b39f0 reads +0x69 as an
   AITIME) and the 0x67 run handler he_RunAuctionEvent (arg[1] is the ranking's minimum wealth): the 0x65 .. 0x6c
   area is a per-kind union, no source names the two ints for the kinds 0x1e / 0x1f / 0x20; a rename or a
   union in the shared ai_bld.h is a shared-header decision (ai.c notes a union there flipped a byte
   match). */
void he_PostEvent67(void)
{
    AIMSG m;
    AIDEC dec;
    AIBLD *cand[3];
    AIBLD *a, *b, *c, *f;
    int n, amt, pick;

    if (ai_Time.year >= 8 && util_RandomU(4) == 0 && he_FindEvent(1, 0, 0x67) == 0) {
        a = (AIBLD *)gm_BuildQuery(1, 0, 0x20);
        b = (AIBLD *)gm_BuildQuery(1, 0, 0x1e);
        c = (AIBLD *)gm_BuildQuery(1, 0, 0x1f);
        n = 0;
        if (a != 0 && a->owner == *ai_PlayerChar)
            cand[n++] = a;
        if (b != 0 && b->owner == *ai_PlayerChar)
            cand[n++] = b;
        if (c != 0 && c->owner == *ai_PlayerChar)
            cand[n++] = c;
        if (n != 0) {
            pick = util_RandomU(n);
            gv_GetTableRecord(7, (int *)&dec);
            f = cand[pick];
            amt = (int)((f->unk65 + f->unk69) * (((5 - dec.cur) * 0.1f + 0.7f) * 0.5f));
            m.type = 0x67;
            m.time = ai_Time;
            m.to = he_PlayerCh->netId;
            m.fromId = f->id;
            m.arg[0] = f->id;
            m.arg[1] = f->unk69 + amt;
            if (m.arg[1] <= 0) {
                amt = gm_EquipmentValue((unsigned char *)f, 100);
                m.arg[1] = (int)((util_RandomF() + 1.0f) * amt);
            }
            m.flags = 2;
            cm_SendLongMessage((char *)&m);
        }
    }
}
/* 0x540e40, the inn report of the round: for every inn (kind 5 / 0x16) with an owner and flag 1 of flags5a clear, when
   the owner is a character of kind 6 / 7 and the inn had guests, sends the owner the report 0x18a1 (fewer than half of
   the guests bought, with the jump "_NACHRICHTEN_HS_82") or 0x18a2 (the guests, the takings); then moves the takings to
   lastTakings and clears the round's counters (field deltas) */
void he_InnReport(void)
{
    AIBLD *b;
    unsigned short owner;
    int none, noneW, guests, served;
    char buf1[512], buf2[512];

    for (b = (AIBLD *)gm_BuildQuery(1, 5, 0x16); b != 0; b = (AIBLD *)gm_BuildNext()) {
        owner = b->owner;
        if (owner != 0xffff && (b->flags5a & 1) == 0) {
            none = 0;
            noneW = 0;
            if ((ai_CharsT[owner].kind == 6 || ai_CharsT[owner].kind == 7) && b->inn.guests > 0) {
                served = b->inn.served;
                guests = b->inn.guests;
                if ((double)served / guests < 0.5) {
                    txt_Format(buf1, 0x18a1, b, guests, b->inn.takings);
                    he_MessageBoxQuickjump(ai_CharsT[b->owner].netId, -1, buf1, 0, 0x5fc, b->id, -1, 0, "_NACHRICHTEN_HS_82");
                } else {
                    txt_Format(buf2, 0x18a2, b, guests, b->inn.takings);
                    he_MessageBoxQuickjump(ai_CharsT[b->owner].netId, -1, buf2, 0, 0x5fc, b->id, -1, 0, ch_EmptyName);
                }
            }
            cm_BeginFieldBlock((char *)b, b->id);
            cm_AddFieldDelta(4, 1, (char *)b - cm_FieldObject + 0x6d, &b->inn.takings);
            cm_AddFieldDelta(4, 1, (char *)b - cm_FieldObject + 0x65, &none);
            cm_AddFieldDelta(2, 1, (char *)b - cm_FieldObject + 0x6b, &noneW);
            cm_AddFieldDelta(2, 1, (char *)b - cm_FieldObject + 0x69, &noneW);
            cm_SendFieldDeltas();
        }
    }
}
/* 0x540fe0, the daily event checks */

void he_DailyEventChecks(void)
{
    he_PostEvent2b();
    he_PostPlayerEvents86a2();
    he_PostEvent67();
    he_ClearInnDarkCorner();
    he_InnStaffLeave();
    he_InnReport();
    he_PostWorkshopEvents98();
}
/* 0x541010, posts the message 0x3a (from -1, flag 1) to the player's character */

void he_PostEvent3a(void)
{
    AIMSG m;

    m.type = 0x3a;
    m.fromId = -1;
    m.flags = 1;
    m.to = ai_CharsT[ai_PlayerId].netId;
    cm_SendLongMessage((char *)&m);
}
/* 0x541060, posts the message 0x3b (now, from -1, flag 1) to the player's character, after freeing a running event 0x3b */

void he_PostEvent3b(void)
{
    AIMSG m;
    char *ev;

    ev = he_FindEvent(1, 0, 0x3b);
    if (ev != 0)
        he_FreeEvent((int *)ev);
    m.type = 0x3b;
    m.fromId = -1;
    m.flags = 1;
    m.to = ai_CharsT[ai_PlayerId].netId;
    m.time = ai_Time;
    cm_SendLongMessage((char *)&m);
}
/* 0x5410f0, posts the event 0x73 to the player character (its netId), from -1, flag 1, at the current ai_Time;
   replaces a running event 0x73 first */

void he_PostEvent73(void)
{
    AIMSG m;
    int r;

    r = (int)he_FindEvent(1, 0, 0x73);
    if (r != 0)
        he_FreeEvent((int *)r);
    m.type = 0x73;
    m.to = he_PlayerCh->netId;
    m.fromId = -1;
    m.time = ai_Time;
    m.flags = 1;
    cm_SendLongMessage((char *)&m);
}
/* 0x541170, posts the message 0xa6 (now, from -1, flag 1) to the player's character, after freeing a running event 0xa6 */

void he_PostEventA6(void)
{
    AIMSG m;
    char *ev;

    ev = he_FindEvent(1, 0, 0xa6);
    if (ev != 0)
        he_FreeEvent((int *)ev);
    m.type = 0xa6;
    m.fromId = -1;
    m.flags = 1;
    m.to = ai_CharsT[ai_PlayerId].netId;
    m.time = ai_Time;
    cm_SendLongMessage((char *)&m);
}
/* 0x541200, sets the end clock (AIJOB.end) of the event record to ai_Time and posts it again when its opts bit 2
   (post when due) is set */

void he_EndEventNow(int r)
{
    ((AIJOB *)r)->end = ai_Time;
    if ((((AIJOB *)r)->opts & 2) != 0)
        cm_SendExSetHE(-1, r);
}
/* 0x541250, sets the character slot (AIJOB.chr, +0x8) of the event record and its netId (+0xc), -1 for 0xffff */

void he_SetEventChar(int r, int pl)
{
    AIJOB *e = (AIJOB *)r;

    e->chr = (unsigned short)pl;
    if (pl != 0xffff) {
        e->netId = ai_CharInts[pl][0];
        return;
    }
    e->netId = -1;
}

float gm_CharacterBaseHealth();
int rd_PointsNear();
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);
struct CHACTION * ch_AddAction(struct CHARACTER *ch, unsigned int type, ...);
int ch_QueueAnim();
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);
void cm_SendSetField(int a, int b, int c, int d, int e);
int cm_SendChrGotoBuilding();

/* 0x541280, the idle behaviour of the event's characters (ids[], 8 of them) in the default scene: each one whose scene
   character has no queued action either walks to 1 .. 3 points near pos (rd_PointsNear, actions 0x2d tagged
   "he_CharacterBehavior"; 41 of 100) or plays one of three gestures twice, each 1 .. 3 times */
void he_CharacterBehavior(AIJOB *ev, float *pos)
{
    int *id;
    int k, n;
    AICHAR *c;
    struct D3SM *h;
    int px, pz;
    float py;
    float (*p)[4];
    CHACTION *act;

    if (d3_CurrentScene == 0) {
        for (id = ev->ids, k = 8; k != 0; k--, id++) {
            if (*id != -1 && (c = (AICHAR *)gm_FindChar(*id)) != 0 && c->scene->actions == 0) {
                if (util_RandomU(100) > 40) {
                    char gest[3][0x20] = { "gestik/durch_haar_fahr", "gestik/kopf_kratzen", "gestik/mit_faust_droh" };

                    ch_QueueAnim(c->scene, gest[util_RandomU(3)], util_RandomU(3) + 1);
                    ch_QueueAnim(c->scene, gest[util_RandomU(3)], util_RandomU(3) + 1);
                } else {
                    float pts[8][4];

                    h = d3_DefaultScene->hmap;
                    n = util_RandomU(3) + 1;
                    n = rd_PointsNear(pos, n, pts);
                    if (n > 0) {
                        for (p = pts; n != 0; n--, p++) {
                            if (d3_GetHeightAt(h, (struct D3VECTOR *)*p, &px, &pz, &py)) {
                                act = ch_AddAction(c->scene, 0x2d, px, pz, 0);
                                strcpy(act->why, "he_CharacterBehavior");
                            }
                        }
                    }
                }
            }
        }
    }
}
/* 0x5414a0, the accident of the character c at its workplace bld (the accident simulation he_RunAccidentSim rolls it):
   for value <= 0 it dies (its script stopped, the death message 0x71 posted, from the event ev's character or
   else the boss), and a playing boss (kind 6 / 7) gets the news 0x1bb0, or 0x414a for a kind 0x14 character,
   unless quiet; for a value below half its base health with a place to carry it to (he_FindFigurePlace) it is
   carried there ("Unfall") and the boss gets the news 0x1baf; else the boss gets 0x1bae, or 0x4149 for kind 0x14.
   value: the float the simulation rolls (reasoned guess: what is left of the character after the accident) */

void he_CharAccident(AIJOB *ev, AICHAR *c, AICHAR *boss, AIBLD *bld, float value, int quiet)
{
    AIMSG m;
    char buf[0x800];
    int place, obj;

    if (value <= 0.0f) {
        sv_MoveToGroup(0, 0, c->id, 0);
        if (c->model != 0 && c->model->scriptId != -1)
            evt_Stop(evt_FindById(c->model->scriptId));
        memset(&m, 0, sizeof(m));
        m.type = 0x71;
        if (ev != 0)
            m.to = ai_CharInts[ev->chr][0];
        else
            m.to = boss->netId;
        m.time = ai_Time;
        m.fromId = -1;
        m.flags = 2;
        m.arg[0] = c->netId;
        cm_SendLongMessage((char *)&m);
        if (boss->kind != 6 && boss->kind != 7)
            return;
        if (quiet != 0)
            return;
        if (c->kind == 0x14) {
            txt_Format(buf, 0x414a, c->id, bld);
            he_MessageBoxQuickjump(boss->netId, bld->id, buf, 0, 0x60a, bld->id, -1, 0, "_NACHRICHTEN_FRIEDHOF_05");
            return;
        }
        txt_Format(buf, 0x1bb0, c->id, bld);
        he_MessageBoxQuickjump(boss->netId, bld->id, buf, 0, 0x60a, bld->id, -1, 0, "_NACHRICHTEN_HS_62");
        return;
    }
    if (gm_CharacterBaseHealth(c) * 0.5f > value
        && ((int (*)(AICHAR *, int *, int *, int))he_FindFigurePlace)(c, &place, &obj, 0) != 0 && c->model != 0) {
        sv_MoveToGroup(0, 0, c->id, 0);
        if (c->model->scriptId != -1)
            evt_Stop(evt_FindById(c->model->scriptId));
        cm_SendSetField(c->netId, 0x1c8, 4, 0x800, 0);
        if (obj == -1)
            cm_SendChrGotoBuilding(c->netId, place, obj, 0, 1, "Unfall");
        else
            cm_SendChrGotoBuilding(c->netId, place, obj, 0, 0, "Unfall");
        if (boss->kind != 6 && boss->kind != 7)
            return;
        txt_Format(buf, 0x1baf, c->id, bld);
        he_MessageBoxQuickjump(boss->netId, bld->id, buf, 0, 0x60a, bld->id, -1, 0, "_NACHRICHTEN_HS_61");
        return;
    }
    if (boss->kind != 6 && boss->kind != 7)
        return;
    if (c->kind == 0x14) {
        txt_Format(buf, 0x4149, c->id, bld);
        he_MessageBoxQuickjump(boss->netId, bld->id, buf, 0, 0x60a, bld->id, -1, 0, "_NACHRICHTEN_FRIEDHOF_04");
        return;
    }
    txt_Format(buf, 0x1bae, c->id, bld);
    he_MessageBoxQuickjump(boss->netId, bld->id, buf, 0, 0x60a, bld->id, -1, 0, "_NACHRICHTEN_HS_61");
}
/* 0x5417d0, the illness of the character c at the workplace bld (the callers roll it: gm_CatchDisease's disease
   and value, the plague): for value <= 0 a playing boss (kind 6 / 7) gets the news 0x1632 and the death message
   0x71 is posted (to the event ev's character, else the boss); for a value below half its base health with a place
   to carry it to (he_FindFigurePlace) it is carried there ("Unfall") and the boss gets the news 0x1631, else
   0x1630; both name the disease (text 0x1626 + disease) */

void he_CharIllness(AIJOB *ev, AICHAR *c, AICHAR *boss, AIBLD *bld, int disease, float value)
{
    AIMSG m;
    char buf[0x800];
    int place, obj;

    if (value <= 0.0f) {
        if (boss->kind == 6 || boss->kind == 7) {
            txt_Format(buf, 0x1632, c->id, bld);
            he_MessageBoxQuickjump(boss->netId, bld->id, buf, 0, 0x5ed, bld->id, -1, 0, 0);
        }
        memset(&m, 0, sizeof(m));
        m.type = 0x71;
        if (ev != 0)
            m.to = ai_CharInts[ev->chr][0];
        else
            m.to = boss->netId;
        m.time = ai_Time;
        m.fromId = -1;
        m.flags = 2;
        m.arg[0] = c->netId;
        cm_SendLongMessage((char *)&m);
        return;
    }
    if (gm_CharacterBaseHealth(c) * 0.5f > value
        && ((int (*)(AICHAR *, int *, int *, int))he_FindFigurePlace)(c, &place, &obj, 0) != 0 && c->model != 0) {
        sv_MoveToGroup(0, 0, c->id, 0);
        if (c->model->scriptId != -1)
            evt_Stop(evt_FindById(c->model->scriptId));
        if (obj == -1)
            cm_SendChrGotoBuilding(c->netId, place, obj, 0, 1, "Unfall");
        else
            cm_SendChrGotoBuilding(c->netId, place, obj, 0, 0, "Unfall");
        if (boss->kind != 6 && boss->kind != 7)
            return;
        txt_Format(buf, 0x1631, c->id, bld, disease + 0x1626);
        he_MessageBoxQuickjump(boss->netId, bld->id, buf, 0, 0x5ed, bld->id, -1, 0, 0);
        return;
    }
    if (boss->kind != 6 && boss->kind != 7)
        return;
    txt_Format(buf, 0x1630, c->id, bld, disease + 0x1626);
    he_MessageBoxQuickjump(boss->netId, bld->id, buf, 0, 0x5ed, bld->id, -1, 0, 0);
}
/* 0x541a50, the run handler of the event types 0x29 / 0x2f / 0x25: sets the event's end clock (AIJOB.end) to
   ai_Time, so it is due at once */

void he_RunDueNow(int ev)
{
    ((AIJOB *)ev)->end = ai_Time;
}
/* 0x541a80, the alloc handler of the inn meeting event 0x2a (when opts bit 4 is clear): due now + 1, the meeting
   clock (+0xb0) now + 0x18 units with its minutes cleared and a random hour 0x13 .. 0x16, state 1, posts it and
   keeps the cm slot (+0x84) */

void he_AllocStammtisch(int r)
{
    AIJOB *e = (AIJOB *)r;
    HESTAMMTISCH *m = (HESTAMMTISCH *)r;
    short s;

    if ((e->opts & 4) == 0) {
        e->end = ai_Time;
        gm_AddTime(&e->end, 1, 0, 0);
        m->meeting = ai_Time;
        gm_AddTime(&m->meeting, 0x18, 0, 0);
        m->meeting.min = 0;
        s = util_Random(4);
        m->meeting.hour = s + 0x13;
        m->state = 1;
        e->exSlot = cm_SendExSetHE(0, r);
    }
}

float gm_GetSkill(char *c, int skill);
void gm_SendMoneyDelta(int a, int b);
int gv_Accuse();
void ai_ShowTributeDelayedNews(int param_1);
void ai_ShowTributeCollectNews(int param_1);
void ai_ShowTributeClaimNews(int param_1, int param_2);

/* 0x544070, a Ghidra miss (disassembled): the alloc handler of the tribute event 0x35, once it is started (opts bit 4
   clear). Without its collector (idbc) the event is re-posted; with more than four tribute events running it is
   re-posted and the event's character hears the delay (ai_ShowTributeDelayedNews). Else it runs from now to a day
   later (tributeEnd), the first round due in a minute; the character pays 1 (gm_SendMoneyDelta) and is accused
   (gv_Accuse 3, 1 with the collector and the character's home building), the round robin over all 0x300 characters
   starts at a random one with a random stride, and the character's skill 4 is kept. A collector holding the good
   0x15c re-posts it and tells of the claim (ai_ShowTributeClaimNews), else ai_ShowTributeCollectNews and the event is
   posted with key 0 */
void he_AllocTribute(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *coll;
    int n = 0;

    if (e->opts & 4)
        return;
    coll = (AICHAR *)gm_FindChar(e->idbc);
    if (coll == 0) {
        e->end = ai_Time;
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    if (he_FindEvent(1, 0, 0x35) != 0) {
        do
            n++;
        while (he_NextEvent() != 0);
        if (n > 4) {
            e->end = ai_Time;
            e->exSlot = cm_SendExSetHE(-1, ev);
            ai_ShowTributeDelayedNews((int)&ai_CharsT[e->chr]);
            return;
        }
    }
    e->end = ai_Time;
    e->start = ai_Time;
    e->tributeEnd = ai_Time;
    gm_AddTime(&e->end, 0, 1, 0);
    gm_AddTime(&e->tributeEnd, 0x18, 0, 0);
    gm_SendMoneyDelta(ai_CharsT[e->chr].netId, -1);
    gv_Accuse(3, 1, ai_CharsT[e->chr].netId, e->idbc, ai_CharsT[e->chr].bld2 != 0 ? ai_CharsT[e->chr].bld2->id : -1);
    e->tributeNext = util_RandomU(0x300);
    e->tributeStride = ai_CharWalkSteps[util_RandomU(0x10)];
    e->tributeSkill = gm_GetSkill((char *)&ai_CharsT[e->chr], 4);
    e->tributeLeft = 0x300;
    if (ai_InventoryHas(coll, 0x15c) != 0) {
        e->end = ai_Time;
        e->exSlot = cm_SendExSetHE(-1, ev);
        ai_ShowTributeClaimNews((int)coll, (int)&ai_CharsT[e->chr]);
        return;
    }
    ai_ShowTributeCollectNews((int)coll);
    e->exSlot = cm_SendExSetHE(0, ev);
}

/* ---- drafted out of address order (the big 0x53f6b0 / 0x541b40 / 0x5437c0 are still missing above this point; move the functions below into address order when those exist) ---- */

int ai_InventoryHas(void *ch, int id);
float gm_GetSkill(char *c, int skill);
float gm_Mix2(float u, float v);          /* 0x504e80, f3gm.h */
int gm_CharLevel(unsigned short id, int again);
int gm_GetWorth(unsigned short id);
void gm_Shuffle(unsigned n, int *out);
int ai_CourtshipRounds();

/* 0x544330, the run handler of the tribute event 0x35: ends it at stage -1 / -2; runs once started (opts bit 4
   clear) and once its last post is settled (exSlot -1 or cm_GetSlotState nonzero). Without its collector (idbc) it
   re-posts itself a minute later. Else one round visits up to 0x10 of the characters left (from tributeNext, stepping
   tributeStride mod 0x300): each living one (id not 0xffff, health above 0) of a kind below 10 other than the
   collector gets the relation change 1 - 10 * gm_Mix2(tributeSkill, its skill 4) from him, doubled while the
   event's character holds the good 0x2c5. While tributeEnd is later than now the next round is due in an hour,
   else the event is re-posted a minute later */
void he_RunTribute(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *coll, *owner;
    int mul, left, next, i;
    float f;

    coll = (AICHAR *)gm_FindChar(e->idbc);
    owner = (AICHAR *)gm_FindChar(e->netId);
    mul = 1;
    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((e->opts & 4) || (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0))
        return;
    e->exSlot = -1;
    if (coll == 0) {
        gm_AddTime(&e->end, 0, 1, 0);
        cm_SendExSetHE(-1, ev);
        return;
    }
    if (owner != 0)
        mul = (ai_InventoryHas(owner, 0x2c5) != 0) + 1;
    left = e->tributeLeft;
    next = e->tributeNext;
    for (i = 0; left != 0 && i < 0x10; i++, left = --e->tributeLeft, next = (e->tributeStride + next) % 0x300) {
        if (ai_CharsT[next].id != 0xffff && ai_CharsT[next].netId != e->idbc && ai_CharsT[next].kind < 10
            && ai_CharsT[next].health > 0) {
            f = -gm_Mix2(e->tributeSkill, gm_GetSkill((char *)&ai_CharsT[next], 4));
            cm_SendRelations(e->idbc, ai_CharsT[next].netId, (int)(f * 10.0f + 1.0f) * mul, 1);
        }
    }
    e->tributeNext = next;
    e->end = ai_Time;
    if (gm_CompareTime(&e->tributeEnd, &ai_Time) == 1) {
        gm_AddTime(&e->end, 1, 0, 0);
        return;
    }
    gm_AddTime(&e->end, 0, 1, 0);
    e->exSlot = cm_SendExSetHE(-1, ev);
}
/* 0x544550, draws the three picks of the pair event 0x41 for the partners a and b (its handlers 0x544630 /
   0x544840): twelve values from half a's level (gm_CharLevel) on, shuffled; the first three go to pairPick with the
   prices (pick + 1) * (a's worth + b's worth, each at least 32000) / 640 in pairPrice */
void he_DrawPairPicks(int ev, AICHAR *a, AICHAR *b)
{
    AIJOB *e = (AIJOB *)ev;
    int tab[12];
    int base, i, wa, wb;

    base = gm_CharLevel(a->id, 0) / 2;
    wa = gm_GetWorth(a->id) > 32000 ? gm_GetWorth(a->id) : 32000;
    wb = gm_GetWorth(b->id) > 32000 ? gm_GetWorth(b->id) : 32000;
    for (i = 0; i < 12; i++)
        tab[i] = i + base;
    gm_Shuffle(12, tab);
    for (i = 0; i < 3; i++) {
        e->pairPick[i] = (unsigned char)tab[i];
        e->pairPrice[i] = (int)((float)((unsigned char)tab[i] + 1) * (float)(wa + wb) * 0.0015625f);
    }
}
/* 0x544630, the alloc handler of the courtship event 0x41, run when posted (opts bit 2): the partners partner1 and
   partner2 must exist and be of different sex, else it is re-posted two minutes later. Their relatives[0] links are
   cleared (sent as field deltas); while another courtship names partner1 as its partner2 it is re-posted two minutes
   later as well. Else its first round is due tomorrow at 8:00, the rounds start at 0 of 0x5c6620(partner1,
   partner2), the picks are drawn (he_DrawPairPicks) and it is posted with key 0 */
void he_AllocCourtship(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *a, *b;
    AIJOB *r;
    int none;

    e->stage = 0;
    if ((e->opts & 2) == 0)
        return;
    a = (AICHAR *)gm_FindChar(e->partner1);
    b = (AICHAR *)gm_FindChar(e->partner2);
    if (a == 0 || b == 0 || a->sex == b->sex) {
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    if (a->relatives[0] != -1) {
        none = -1;
        cm_BeginFieldBlock((char *)a, a->netId);
        cm_AddFieldDelta(4, 1, 0x5c, &none);    /* a->relatives[0] = -1 */
        cm_SendFieldDeltas();
    }
    if (b->relatives[0] != -1) {
        none = -1;
        cm_BeginFieldBlock((char *)b, b->netId);
        cm_AddFieldDelta(4, 1, 0x5c, &none);    /* b->relatives[0] = -1 */
        cm_SendFieldDeltas();
    }
    for (r = (AIJOB *)he_FindEvent(1, 0, 0x41); r != 0; r = (AIJOB *)he_NextEvent()) {
        if (r->partner2 == a->netId) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
    }
    e->end = ai_Time;
    gm_AddTime(&e->end, 0x18, 0, 0);
    e->end.min = 0;
    e->stage = 0;
    e->pairRound = 0;
    e->end.hour = 8;
    e->pairRounds = (char)ai_CourtshipRounds(e->partner1, e->partner2);
    e->pairDrawn = 0;
    he_DrawPairPicks(ev, a, b);
    e->pairDone = 0;
    e->exSlot = -1;
    cm_SendExSetHE(0, ev);
}

unsigned int ai_MarriageTest(int param_1, int param_2);
int gv_FindReservedEntry();
int gv_EntryIndex(char *r, int *e);
int gv_OrderSetGraveHolder();
int cm_SendNewParty(void *p);
int cm_SendRelations(int a, int b, int c, int d);
void cm_SendFloatDelta(int id, int field, float delta);

/* 0x544840, a Ghidra miss (disassembled): the run handler of the courtship event 0x41 (see he_AllocCourtship) of
   partner1 (a) and partner2 (b), of different sex. Ending it (stage -1 / -2) after it was posted clears the flag
   0x40000 of the one partner left when the other is gone. While a partner is missing it is re-posted two minutes
   later; it waits for its last post to settle (exSlot). Stage 0 asks ai_MarriageTest: -0x405 / -0x406 tell a the
   refusal 0x42f6 / 0x42f7; -0x403 and 0 tell a player a (kind 6) how b's family stands (0x1ceb .., naming b's
   relatives[1] while alive and b no kind 5, else b); 1 recomputes the rounds (ai_CourtshipRounds) and counts one;
   once pairRound reaches pairRounds it is posted with key 1, else a player hears the rounds left (0x1cf3 / 0x1cf4),
   a kind 5 a counts 1 .. 2 rounds, others one by a chance of 1 in 4, and the next round is due tomorrow at 8:00 with
   new picks. Stage 1 links the partners (group "werbung": each other's relatives[0], b takes a's family w050, both
   get the flag 0x40000) and posts it with key 5. Stage 4 re-posts it. Stage 5: when they are each other's
   relatives[0], both relations go to 0x7f, a player or kind 5 a hears 0x1d31 and the partner of sex 1 gets f07c 1.1,
   b's reserved grave entry is ordered, and the wedding party (G28REC kind 6, flags 0xb2, two hours on, between 7 and
   22 o'clock) is sent with the event posted with key 4; else a player a hears the family text again
   @deep reconstructed and read in full against the original (stage -1/-2 flag clearing, the slot wait,
   stage 0's ai_MarriageTest ladder with the 0x42f6 / 0x42f7, 0x1ceb.. and 0x1cf3 / 0x1cf4 texts, the
   rounds, stage 1 'werbung' links, stage 4, stage 5's mutual test (bitwise, as the original), relations,
   0x1d31, f07c, the grave order and the G28REC party cleared by MEM_ZERO). Fixed meaning: the party's
   members are a's / b's netIds (was the event id), the stage 5 short text 0x1cf1 passes b's id, three
   buffers 0x200 / 0x200 / 0xec, the if/else nesting and the 'delay' join that put the free and the
   missing-partner blocks at the end. Calls, stores and references agree (no differs / ref lines). Left:
   frame layout (the text buffer at [ebp-0x100] and the party record at [ebp-0x214] where the original has
   them at [ebp-0x214] / [ebp-0x128], so every strlen append's lea [ebp+ecx-K] shows as code, +0xa81 ..),
   the slot of a ([ebp-8] vs the argument slot [ebp+8]) and of the MEM_ZERO temporaries, the scheduling of
   the party record's stores (+0xbb0 .. +0xc3c), and tail merging (+0xcb7: VC6 cross-jumps our
   missing-partner block and case 4 into the common tail where the original keeps them separate; switch
   entry 4 differs for that reason); tried declaration order, names, the buffer size and an inner scope for
   the record: no change of the layout (size: 1034 vs 1064 instructions, explained: 30 instructions: the
   missing-partner block (end = now, gm_AddTime, cm_SendExSetHE, epilogue: 25) and case 4's own pushes +
   jmp (5), which VC6 cross-jumps into the common tail in ours and keeps apart in the original (different
   register choice in its end copy); no statements missing) */
void he_RunCourtship(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *a, *b, *c1, *c2, *who;
    unsigned int r;
    AIBLD *yard;
    int *entry;
    char news[0x200];
    char gave[0x200];
    char line[0xec];
    G28REC party;

    a = (AICHAR *)gm_FindChar(e->partner1);
    b = (AICHAR *)gm_FindChar(e->partner2);
    if (e->stage != -1 && e->stage != -2) {
        if (e->opts & 4)
            return;
        if (a != 0 && b != 0 && a->sex != b->sex) {
            if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
                return;
            e->exSlot = -1;
            switch (e->stage) {
            case 0:
                r = ai_MarriageTest(e->partner1, e->partner2);
                if (r == -0x405) {
                    e->end = ai_Time;
                    gm_AddTime(&e->end, 0, 2, 0);
                    e->exSlot = cm_SendExSetHE(-1, ev);
                    txt_Format(gave, 0x42f6, b->id);
                    he_ShowNews(a->netId, a->netId, gave, 1, 0x5f4, 0);
                    return;
                }
                if (r == -0x406) {
                    e->end = ai_Time;
                    gm_AddTime(&e->end, 0, 2, 0);
                    e->exSlot = cm_SendExSetHE(-1, ev);
                    txt_Format(gave, 0x42f7, b->id);
                    he_ShowNews(a->netId, a->netId, gave, 1, 0x5f4, 0);
                    return;
                }
                if (r == -0x400 || r == -0x401 || r == -0x402)
                    break;
                if (r == -0x403) {
                    if (a->kind == 6) {
                        c1 = (AICHAR *)gm_FindChar(b->relatives[1]);
                        c2 = (AICHAR *)gm_FindChar(b->relatives[0]);
                        if (c1 != 0 && c1->health > 0 && b->kind != 5) {
                            txt_Format(line, 0x1ceb, c1->id, a->id);
                            txt_Format(line + strlen(line), 0x1cee);
                            txt_Format(line + strlen(line), 0x1cf2, b->id);
                            txt_Format(line + strlen(line), 0x1cef, c2->id);
                            txt_Format(line + strlen(line), 0x1cf0, c1->id);
                        } else {
                            txt_Format(line, 0x1ceb, b->id, a->id);
                            txt_Format(line + strlen(line), 0x1cee);
                            txt_Format(line + strlen(line), 0x1cf1);
                            txt_Format(line + strlen(line), 0x1cef, c2->id);
                            txt_Format(line + strlen(line), 0x1cf0, b->id);
                        }
                        he_ShowNews(a->netId, a->netId, line, 1, 0x5f4, 0);
                    }
                    break;
                }
                if (r == 0) {
                    if (a->kind == 6) {
                        c1 = (AICHAR *)gm_FindChar(b->relatives[1]);
                        if (c1 != 0 && c1->health > 0 && b->kind != 5) {
                            txt_Format(line, 0x1ceb, c1->id, a->id);
                            txt_Format(line + strlen(line), 0x1cec);
                            txt_Format(line + strlen(line), 0x1cf2, b->id);
                            txt_Format(line + strlen(line), 0x1ced);
                            txt_Format(line + strlen(line), 0x1cf0, c1->id);
                        } else {
                            txt_Format(line, 0x1ceb, b->id, a->id);
                            txt_Format(line + strlen(line), 0x1cec);
                            txt_Format(line + strlen(line), 0x1cf1);
                            txt_Format(line + strlen(line), 0x1ced);
                            txt_Format(line + strlen(line), 0x1cf0, b->id);
                        }
                        he_ShowNews(a->netId, a->netId, line, 1, 0x5f4, 0);
                    }
                    e->end = ai_Time;
                    goto delay;
                }
                if (r == 1) {
                    e->pairRounds = (char)ai_CourtshipRounds(e->partner1, e->partner2);
                    e->pairRound++;
                }
                if (e->pairRound >= e->pairRounds) {
                    e->end = ai_Time;
                    gm_AddTime(&e->end, 0, 2, 0);
                    e->exSlot = cm_SendExSetHE(1, ev);
                    return;
                }
                if (a->kind == 6) {
                    txt_Format(line, 0x1cf3);
                    txt_Format(line + strlen(line), 0x1cf4, b->id, 0x5d, e->pairRounds - e->pairRound, 0x5d);
                    he_ShowNews(a->netId, a->netId, line, 0, 0x5f4, 0);
                } else if (a->kind == 5) {
                    e->pairRound += (char)util_Random(2) + 1;
                } else if ((short)util_Random(4) == 0) {
                    e->pairRound++;
                }
                e->end = ai_Time;
                gm_AddTime(&e->end, 0x18, 0, 0);
                e->end.hour = 8;
                e->pairDrawn = 1;
                he_DrawPairPicks(ev, a, b);
                e->exSlot = -1;
                cm_SendExSetHE(0, ev);
                return;
            case 1:
                cm_BeginGroup("werbung");
                cm_BeginFieldBlock((char *)b, b->netId);
                cm_AddFieldDelta(4, 1, 0x5c, &a->netId);    /* b->relatives[0] = a->netId */
                cm_AddFieldDelta(2, 1, 0x50, &a->w050);     /* b->w050 = a->w050 */
                cm_SendFieldDeltas();
                cm_BeginFieldBlock((char *)a, a->netId);
                cm_AddFieldDelta(4, 1, 0x5c, &b->netId);    /* a->relatives[0] = b->netId */
                cm_SendFieldDeltas();
                cm_SendSetField(b->netId, 0x1c8, 4, 0x40000, 0);
                cm_SendSetField(a->netId, 0x1c8, 4, 0x40000, 0);
                cm_EndGroup("werbung");
                e->end = ai_Time;
                gm_AddTime(&e->end, 0, 1, 0);
                e->exSlot = cm_SendExSetHE(5, ev);
                return;
            case 4:
                gm_AddTime(&e->end, 0, 2, 0);
                e->exSlot = cm_SendExSetHE(-1, ev);
                return;
            case 5:
                /* each other's relatives[0]: the original tests both, bitwise (and edx, eax; test dl, 1) */
                if (!((a->relatives[0] == b->netId) & (b->relatives[0] == a->netId) & 1)) {
                    if (a->kind == 6) {
                        c1 = (AICHAR *)gm_FindChar(b->relatives[1]);
                        c2 = (AICHAR *)gm_FindChar(b->relatives[0]);
                        if (c1 != 0 && c1->health > 0 && b->kind != 5) {
                            txt_Format(line, 0x1ceb, c1->id, a->id);
                            txt_Format(line + strlen(line), 0x1cee);
                            txt_Format(line + strlen(line), 0x1cf2, b->id);
                            txt_Format(line + strlen(line), 0x1cef, c2->id);
                            txt_Format(line + strlen(line), 0x1cf0, c1->id);
                        } else {
                            txt_Format(line, 0x1ceb, b->id, a->id);
                            txt_Format(line + strlen(line), 0x1cee);
                            txt_Format(line + strlen(line), 0x1cf1, b->id);
                            txt_Format(line + strlen(line), 0x1cef, c2->id);
                            txt_Format(line + strlen(line), 0x1cf0, b->id);
                        }
                        he_ShowNews(a->netId, a->netId, line, 1, 0x5f4, 0);
                    }
                    break;
                }
                cm_SendRelations(a->netId, b->netId, 0x7f - ai_Relations[a->id][b->id], 1);
                cm_SendRelations(b->netId, a->netId, 0x7f - ai_Relations[b->id][a->id], 1);
                if (a->kind == 6 || a->kind == 5) {
                    who = a;
                    if (a->sex != 1)
                        who = b;
                    txt_Format(news, 0x1d31, b->id);
                    he_ShowNews(a->netId, a->netId, news, 0, 0x5f4, 0);
                    cm_SendFloatDelta(who->netId, 0x7c, 1.1f - who->f07c);
                }
                if (gv_FindReservedEntry(b, &yard, &entry) != 0)
                    gv_OrderSetGraveHolder(0, yard, *entry, gv_EntryIndex((char *)yard->yard, entry));
                MEM_ZERO(&party, sizeof(party));
                party.members[0] = a->netId;
                party.members[1] = b->netId;
                party.host = ai_CharsT[ai_PlayerId].netId;
                party.time = ai_Time;
                party.kind = 6;
                party.flags = 0xb2;
                party.thing = -1;
                party.used = 2;
                gm_AddTime(&party.time, 2, 0, 0);
                party.time.min = 0;
                if (party.time.hour > 0x16) {
                    party.time.year++;
                    party.time.hour = 0xb;
                } else if (party.time.hour < 7) {
                    party.time.hour = 0xb;
                }
                e->end = party.time;
                e->pairDone = 1;
                e->exSlot = -1;
                cm_SendExSetHE(4, ev);
                cm_SendNewParty(&party);
                return;
            default:
                return;
            }
            e->end = ai_Time;
        delay:
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
        } else {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
        }
    } else {
        if (e->opts & 2) {
            if (b != 0 && (b->flags1c8 & 0x40000) && a == 0)
                cm_SendSetField(b->netId, 0x1c8, 4, 0, 0x40000);
            else if (a != 0 && (a->flags1c8 & 0x40000) && b == 0)
                cm_SendSetField(a->netId, 0x1c8, 4, 0, 0x40000);
        }
        he_FreeEvent((int *)ev);
    }
}
/* 0x5471e0, the duel intro: lets both duellists (p2 / p3, 0x4c4a60 flag 0x400) hear the news 0x1d83 / 0x1d82 (messages 0x5fa / 0x5fb, "_DUELL_INTRO_EVENT_DUELLANTEN_HS_00" for the first) and posts the event (state 2) */

void he_DuelAccepted(int ev, unsigned short *p2, unsigned short *p3)
{
    char buf[1024];

    cm_SendSetField(*(int *)(p2 + 2), 0x1c8, 4, 0x400, 0);
    cm_SendSetField(*(int *)(p3 + 2), 0x1c8, 4, 0x400, 0);
    if (*(char *)(p2 + 1) == 6 || *(char *)(p2 + 1) == 7) {
        txt_Format(buf, 0x1d83, *p3, ev + 0x52, *p3);
        he_ShowNews(*(int *)(p2 + 2), *(int *)(p2 + 2), buf, 0, 0x5fa, "_DUELL_INTRO_EVENT_DUELLANTEN_HS_00");
    }
    if (*(char *)(p3 + 1) == 6 || *(char *)(p3 + 1) == 7) {
        txt_Format(buf, 0x1d82, *p2, ev + 0x52, *p2);
        he_ShowNews(*(int *)(p3 + 2), -1, buf, 0, 0x5fb, "_DUELL_INTRO_EVENT_DUELLANTEN_HS_00");
    }
    cm_SendExSetHE(2, ev);
}

void amt_ChangeDistrictOpinion();
int amt_FindOffice();
int ai_AmountRowStatus();
int ai_DrawAmountRow();
struct SBSAMPLE * sb_PlaySample();
int gm_FindById();
int cm_SendMessage3d(char *p, unsigned char b, int c, int d);
void gm_AddTime2();
int cm_SendLongMessage(char *msg);
int gm_CompareTime();
int d2_SetForm(int view, int n);
int io_PrintText(char *fmt, ...);

/* 0x5472d0, a Ghidra miss (disassembled): the challenge of the insult event ("Special\Beleidigung") is declined
   (the second choice, or no choice in time) by q, challenged by p (the characters partner1 / partner2 of the
   event ev it runs for): a player p hears the news 0x1d81 (0x1d68 + 1 for a q of sex 1) and both lose the flag
   0x400 (flags1c8); q's district (the district of its office, else a random 1 .. 3) loses
   9 + 8 * p's skill 4 + random(8) of its opinion of q (amt_ChangeDistrictOpinion, district 0 without an office),
   p's relation to q drops by 0x14, and ev ends now */
void he_DuelDeclined(int ev, AICHAR *p, AICHAR *q)
{
    char text[0x400];
    int office[6];
    int seat[6];
    int district = 0, amt;

    if (p->kind == 6 || p->kind == 7) {
        txt_Format(text, 0x1d81, q->id, q->id, (q->sex != 0) + 0x1d68);
        he_ShowNews(p->netId, p->netId, text, 0, 0x5fb, (char *)0);
        cm_SendSetField(p->netId, 0x1c8, 4, 0, 0x400);
        cm_SendSetField(q->netId, 0x1c8, 4, 0, 0x400);
    }
    if (q->officeKind != 0) {
        if (amt_FindOffice(q, office, seat) != 0)
            district = office[1];
        else
            district = util_RandomU(3) + 1;
    }
    amt = -9 - (int)(gm_GetSkill(p, 4) * 8.0f);
    amt -= (unsigned short)util_Random(8);
    amt_ChangeDistrictOpinion(q, district, amt);
    cm_SendRelations(p->netId, q->netId, -0x14, 1);
    ((AIJOB *)ev)->end = ai_Time;
    cm_SendExSetHE(-1, ev);
}
/* 0x547430, a Ghidra miss (disassembled): the run handler of the duel challenge event 0x46 (its alloc handler
   0x547170): duelId names the duel event, its partner1 the challenger p, partner2 the challenged q. With the
   stage -1 / -2 or the duel, p or q gone both events end (the slot released, the duel posted -1). Stage 0 opens
   the event slot "Special\Beleidigung" (no slot: the event is freed), plays "NeueMessage_s_01" (unless the game
   state is 2), prints the news 0x1d6c, p's special-amount row (ai_AmountRowStatus / ai_DrawAmountRow, its control
   clickable) and the news 0x1d6b; the answer is due in two hours. Stage 1: past that time, or with the second
   choice, q declines (he_DuelDeclined); the first choice accepts (he_DuelAccepted); then the slot is released and
   the event freed */
void he_RunDuelChallenge(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIJOB *duel;
    AICHAR *q = 0, *p = 0;
    int row[14];                        /* a special-amount row, cm.c's CMAMOUNTROW: [0] the character, [1] its control */

    duel = (AIJOB *)he_FindEvent(1, 1, ((HEDUEL *)e)->duelId);
    if (duel != 0) {
        q = (AICHAR *)gm_FindChar(duel->partner2);
        p = (AICHAR *)gm_FindChar(duel->partner1);
    }
    if (e->stage == -1 || e->stage == -2 || duel == 0 || q == 0 || p == 0) {
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        if (duel != 0) {
            duel->end = ai_Time;
            cm_SendExSetHE(-1, (int)duel);
        }
        he_FreeEvent((int *)ev);
        return;
    }
    switch (e->stage) {
    case 0:
        he_OpenEventSlot(ev, 0, 0, (int)"Special\\Beleidigung", 0x5ec);
        if (e->slot == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        if (ai_GameState != 2)
            sb_PlaySample(snd_SystemBank, "NeueMessage_s_01", 1, 0x3f, 0x7f, 0, 0);
        d2_SetForm(e->slot->win, 1);
        io_PrintText(0x1d6c);
        d2_SetForm(e->slot->win, 2);
        row[0] = (int)p;
        if (ai_AmountRowStatus(row) != 0) {
            ai_DrawAmountRow(d2_LastWindowPtr->w / 2 - 0x28, 0, d2_LastWindow, row);
            d2_Objs[row[1]].interactive = 1;
            d2_Objs[row[1]].style = 3;
            d2_Objs[row[1]].user = (void *)row[0];
        }
        d2_SetForm(e->slot->win, 3);
        io_PrintText(0x1d6b, ((HEDUEL *)duel)->newsArg1, p->id, ((HEDUEL *)duel)->newsArg2, p->id);
        e->start = ai_Time;
        e->end = ai_Time;
        gm_AddTime(&e->end, 2, 0, 0);
        e->stage++;
        return;
    case 1:
        if (gm_CompareTime(&ai_Time, &e->end) > 0) {
            he_DuelDeclined((int)duel, p, q);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (d2_ClickedForm != e->slot->win)
            return;
        if (d2_ClickedId == -1)
            return;
        if (d2_ClickedId == 0)
            he_DuelAccepted((int)duel, (unsigned short *)p, (unsigned short *)q);
        else if (d2_ClickedId == 1)
            he_DuelDeclined((int)duel, p, q);
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x547750, a Ghidra miss (disassembled): the alloc handler of the insult event 0x47 (its run handler 0x547ba0),
   partner1 (a) insults partner2 (b): it is due the next day at 6:30 (no cm slot yet). Once started (opts 2): when
   a or b is gone and one of them is already in a quarrel (flags1c8 0x400; @original read through the pointer
   that may be 0) it ends in two minutes. a is accused of the crime 4 against b (at a's home, else b's). A player b
   gets the long message 0x46 (to b, the event's id in arg[0], its time six hours past zero) that starts the duel
   challenge (he_RunDuelChallenge) -> 1, both in a quarrel. Else b answers by itself: with util_RandomF() +
   gm_Mix(b, a, 3) of 1 or more it challenges (both in a quarrel, a player a hears the duel intro news 0x1d83)
   -> 2; below, it declines as he_DuelDeclined does (a hears 0x1d81, b's district's opinion and b's relation to a
   drop) and the event ends in two minutes
   @deep Read whole against the original and fixed: typed over AIJOB / AICHAR / AIMSG (the message 0x46
   with msg.time + 6 hours through gm_AddTime2, the original's +0x28), the second office array 6 ints and
   the text / message buffers block-scoped (frame 0x134 equal), the chance as util_RandomF() + gm_Mix(b, a,
   3) >= ai_FloatOne (the original's and eax,0x100 / jne), the null-pointer flag test of the gone branch
   kept (@original). 338 vs 338 instructions; left: scheduling only in the 0x1d81 news arguments
   (+0x547951, 16 lines): the original loads b->sex into al, setne cl, add ecx,0x1d68, then xor eax / mov
   ax,[ebp]; ours loads sex into cl, the id first (xor eax / mov ax,[ebp]) and setne dl / add edx: the same
   instructions in another order and registers (0x1d68 + (sex != 0) spelled either way compiles alike;
   he_DuelDeclined's identical expression is proved equal); the prover stops at the escaped frame (+0x286) */
void he_AllocInsult(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *a, *b;
    int office[6];
    int seat[6];
    int place, district, amt;
    float liking;

    e->exSlot = -1;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0x18, 0, 0);
    e->end.hour = 6;
    e->end.min = 0x1e;
    if ((e->opts & 2) == 0)
        return;
    a = (AICHAR *)gm_FindChar(e->partner1);
    b = (AICHAR *)gm_FindChar(e->partner2);
    if (a == 0 || b == 0) {
        if ((a->flags1c8 & 0x400) != 0 || (b->flags1c8 & 0x400) != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
    }
    place = -1;
    if (a->bld2 != 0)
        place = a->bld2->id;
    else if (b->bld2 != 0)
        place = b->bld2->id;
    gv_Accuse(4, 1, a->netId, b->netId, place);
    if (b->kind == 6 || b->kind == 7) {
        AIMSG msg;

        memset(&msg, 0, sizeof(msg));
        msg.to = b->netId;
        msg.fromId = b->netId;
        msg.type = 0x46;
        msg.flags = 9;
        msg.arg[0] = e->id;
        gm_AddTime2(&msg.time, b->id, 6, 0, 0);     /* the second argument unused */
        cm_SendLongMessage((char *)&msg);
        e->end = ai_Time;
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = 6;
        e->end.min = 0x1e;
        e->exSlot = cm_SendExSetHE(1, ev);
        cm_SendSetField(a->netId, 0x1c8, 4, 0x400, 0);
        cm_SendSetField(b->netId, 0x1c8, 4, 0x400, 0);
        return;
    }
    liking = gm_Mix(b, a, 3);
    if (util_RandomF() + liking >= ai_FloatOne) {
        cm_SendSetField(a->netId, 0x1c8, 4, 0x400, 0);
        cm_SendSetField(b->netId, 0x1c8, 4, 0x400, 0);
        e->end = ai_Time;
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = 6;
        e->end.min = 0x1e;
        if (a->kind == 6 || a->kind == 7) {
            char text[0x100];

            txt_Format(text, 0x1d83, b->id, &e->end, b->id);
            he_ShowNews(a->netId, a->netId, text, 0, 0x5fa, "_DUELL_INTRO_EVENT_DUELLANTEN_HS_00");
        }
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    }
    district = 0;
    if (a->kind == 6 || a->kind == 7) {
        char text[0x100];

        txt_Format(text, 0x1d81, b->id, b->id, (b->sex != 0) + 0x1d68);
        he_ShowNews(a->netId, a->netId, text, 0, 0x5fa, (char *)0);
    }
    if (b->officeKind != 0) {
        if (amt_FindOffice(b, office, seat) != 0)
            district = office[1];
        else
            district = (unsigned short)util_Random(3) + 1;
    }
    amt = -9 - (int)(gm_GetSkill(a, 4) * 8.0f);
    amt -= (unsigned short)util_Random(8);
    amt_ChangeDistrictOpinion(b, district, amt);
    cm_SendRelations(b->netId, a->netId, -0x14, 1);
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 2, 0);
    e->exSlot = cm_SendExSetHE(-1, ev);
}
/* 0x547ba0, a Ghidra miss (disassembled): the run handler of the insult event 0x47 (he_AllocInsult), once its cm
   slot is answered: stage 2 (the insulted challenged) posts a fight party of kind 4 between partner1 (a) and
   partner2 (b) half an hour from now (G28REC, held by the player's character, flags 0xa2); stages 2 and 1 (a
   player's challenge sent, he_RunDuelChallenge goes on) take the quarrel flag 0x400 (flags1c8) from both and end
   the event in two minutes; stage -1 / -2 (left) does the same when it was started and frees the event; all only
   once started (opts 2)
   @deep Read whole against the original and fixed: typed over AIJOB / AICHAR / G28REC (the fight party of
   kind 4: members a / b, host the player's character, thing -1, used 2, flags 0xa2, variant 0, time now +
   0x1e minutes), exSlot cleared once before the switch, the cases 2 / 1 / -2,-1 with 0 the default (jump
   table equal, 5 entries); 798 vs 798 bytes, the rest of the function equal. Left: scheduling of the party
   block only (+0x547c3b..+0x547ca5, 46 lines): the original copies time.hour right after members[0],
   pushes the gm_AddTime arguments (0x1e, 0, &party.time) before the kind / flags / thing / used stores and
   copies year / min / sec last; ours (party.time = ai_Time placed after members[1]; placed after
   members[0] it is worse, 41 ins and an alignment M8) stores kind .. used first and pushes later: the same
   stores and loads in another order; the prover stops at the switch jump */
void he_RunInsult(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *a, *b;
    G28REC party;

    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    switch (e->stage) {
    case 2:
        if ((e->opts & 2) == 0)
            return;
        a = (AICHAR *)gm_FindChar(e->partner1);
        b = (AICHAR *)gm_FindChar(e->partner2);
        if (a != 0) {
            if (b != 0) {
                memset(&party, 0, sizeof(party));
                party.members[0] = a->netId;
                party.kind = 4;
                party.flags = 0xa2;
                party.thing = -1;
                party.used = 2;
                party.host = ai_CharInts[ai_PlayerId][0];
                party.members[1] = b->netId;
                party.time = ai_Time;
                party.variant = 0;
                gm_AddTime(&party.time, 0, 0x1e, 0);
                cm_SendNewParty(&party);
            }
            cm_SendSetField(a->netId, 0x1c8, 4, 0, 0x400);
        }
        if (b != 0)
            cm_SendSetField(b->netId, 0x1c8, 4, 0, 0x400);
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    case 1:
        if ((e->opts & 2) == 0)
            return;
        a = (AICHAR *)gm_FindChar(e->partner1);
        b = (AICHAR *)gm_FindChar(e->partner2);
        if (a != 0)
            cm_SendSetField(a->netId, 0x1c8, 4, 0, 0x400);
        if (b != 0)
            cm_SendSetField(b->netId, 0x1c8, 4, 0, 0x400);
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    case -2:
    case -1:
        if (e->opts & 2) {
            a = (AICHAR *)gm_FindChar(e->partner1);
            b = (AICHAR *)gm_FindChar(e->partner2);
            if (a != 0)
                cm_SendSetField(a->netId, 0x1c8, 4, 0, 0x400);
            if (b != 0)
                cm_SendSetField(b->netId, 0x1c8, 4, 0, 0x400);
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
        }
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x547ee0, a Ghidra miss (disassembled): the init of the "RndGeselle" event: the figure +0xb0 (0x4fea50) must exist (else the event is freed), +0xbc = 0x4c55d0 (figure, 0, the byte at +0x22f of its building kind record (0x24d bytes each from 0x7e3f30), 0), the clock is 6 units ahead (ROUGH) */

void he_Unk547ee0(int ev)
{
    int o = 0;
    int c;

    gm_FindById(&o, 0, 0, *(int *)(ev + 0xb0));
    if (o == 0)
        he_FreeEvent((int *)ev);
    c = *(unsigned char *)o;
    *(int *)(ev + 0xbc) = cm_SendMessage3d(o, 0, *(unsigned char *)(U_I((unsigned)&ai_BTypes) + c * 0x24d + 0x22f), 0);
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 6, 0);
}
/* 0x547f90, a Ghidra miss (disassembled): the run handler of the "RndGeselle" event 0x4a (a random journeyman):
   pushes the end four units ahead, frees the event at stage -2 or when its building is gone; at stage 0, once the
   cm slot is answered, sends the new character it holds to the building ("RndGeselle"), shows the news newsId
   (the character, its title text 0x20d / 0x230 by sex + title, the receiver) in a quick-jump message box and frees
   the event */

void he_RunGeselle(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGESELLE *g = (HEGESELLE *)ev;
    char text[0x200];
    int bld = 0, chr = 0;
    AICHAR *c;
    int title;

    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 4, 0);
    gm_FindById(&bld, 0, 0, g->bldId);
    gm_FindById(0, 0, &chr, g->chrId);
    if (e->stage == -2 || bld == 0)
        he_FreeEvent((int *)ev);
    if (e->stage == 0 && cm_GetSlotState(g->slot) != 0) {
        c = (AICHAR *)cm_GetSlotValue(g->slot);
        if (c != 0) {
            title = (c->sex ? 0x230 : 0x20d) + (unsigned char)c->title;
            cm_SendChrGotoBuilding(c->netId, g->bldId, g->bldArg, 0, 0, "RndGeselle");
            txt_Format(text, g->newsId, c->id, title, chr);
            he_MessageBoxQuickjump(g->chrId, g->chrId, text, 0, 0x5ec, g->bldId, g->bldArg, 0, "_NACHRICHTEN_HS_00");
        }
        he_FreeEvent((int *)ev);
    }
}
/* 0x548110, a Ghidra miss (disassembled): the start of the insult news event: the clock +0x18 units ahead, the news 0x1d64 (for b of kind 6 / 7, "_NACHRICHTEN_HS_20") and the event is posted (state 0); a gone figure ends it with the clock copied (ROUGH) */

void he_Unk548110(int ev)
{
    char text[0x400];
    int a, b;

    *(int *)(ev + 0x84) = -1;
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            b = gm_FindChar(*(int *)(ev + 0xb0));
            a = gm_FindChar(*(int *)(ev + 0xac));
            if (a == 0 || b == 0) {
                *(AITIME *)(ev + 0x52) = ai_Time;
                cm_SendExSetHE(-1, ev);
                return;
            }
            if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
                txt_Format(text, 0x1d64, *(unsigned short *)a, ev + 0x52, *(unsigned short *)a);
                he_ShowNews(*(int *)(b + 4), -1, text, 0, 0x5fb, "_NACHRICHTEN_HS_20");
            }
        }
        cm_SendExSetHE(0, ev);
    }
}

int gm_GetWorth(unsigned short id);
void gm_ChangeRelationToJobs(unsigned short a, int b);
void cm_SendAddSkill(int a, int b, int c);
void cm_SendSkillsChanged(int a);
void cm_SendSpawnCharacter(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s);
void ai_PlagueNews(void);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);
int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out);
void amt_ChangeDistrictOpinion();

int gm_BuildingClass();
unsigned char * gm_BuildNext(void);
int gm_ListAmount();
unsigned short *gm_QueryNext(void);
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);
void gm_ChangeBuildingState(unsigned char *b, int d);
float gm_GoodsPrice(unsigned id, unsigned char p);

int gm_TitleGroup();
int gm_JobSlots();
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);
void cm_SendSetJob(int id, int job);
int cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);
short *gm_PlayerRecord(char *c);
int ai_ApplyTrialStep();

/* 0x548230, the run handler of the insult news event (type 0x4b, whose start handler is 0x548110): a pending cm
   slot still in state 0 keeps it; a freed state (-2) ends it, a negative one frees it, and in state 0 with opts
   bit 2 it is posted again two minutes later (the end time = the clock now, cm_SendExSetHE) */
void he_RunInsultNews(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int st;

    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    st = e->stage;
    e->exSlot = -1;
    if (st < -2)
        return;
    if (st >= 0) {
        if (st != 0)
            return;
        if ((e->opts & 2) == 0)
            return;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x5482c0, a Ghidra miss: the start handler: the due clock +0x52 = now, posted 0x18 units ahead */

void he_Unk5482c0(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
}
/* 0x548300, a Ghidra miss: counts +0xac (a byte) down in the state 0, frees the event at 0 or when it is freed (-2) */

void he_Unk548300(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st != -2) {
        if (st != 0)
            return;
        if (*(unsigned char *)(ev + 0xac) != 0) {
            *(unsigned char *)(ev + 0xac) = *(unsigned char *)(ev + 0xac) - 1;
            return;
        }
    }
    he_FreeEvent((int *)ev);
}
/* 0x548330, a Ghidra miss: +0xb0 = +0xac + 1, the due clock now, posted 0x18 units ahead */

void he_Unk548330(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0xb0) = *(int *)(ev + 0xac) + 1;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
}
/* 0x548380, a Ghidra miss: frees the event in the state 0 or -2 */

void he_Unk548380(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st == -2 || st == 0)
        he_FreeEvent((int *)ev);
}
/* 0x5483a0, the init of a timed event: +0xb0 = -1, the clock +0x52 = the start (+0x44) + random(4) + 2 in the minute word (+0x56), state 0 */

void he_Unk5483a0(int ev)
{
    *(int *)(ev + 0xb0) = -1;
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(short *)(ev + 0x56) = *(short *)(ev + 0x56) + (short)util_Random(4) + 2;
    *(int *)(ev + 0x70) = 0;
}
/* 0x5483f0 (the Ghidra function 0x548410 starts after the free in the state -2), the run handler of event 0x53:
   formats the news 0x10c2 (the own player slot's name text at +0x20, one of the texts 0x10c4 .. 0x10c6); every
   living character (health >= 2) of kind 5 / 6 / 7 looks for a building of the kinds 7 / 22
   (ai_PickActionBuilding); kinds 6 / 7 get the news; with a building found it is paid 1% of its worth times
   2 .. 5 by the own player (cm_SendMoveGoodsUnchecked) and kinds 6 / 7 get the news 0x10c3 (the building, the
   amount); then frees the event */

void he_RunCharPayouts(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    char news[0x800];
    char line[0x200];
    AICHAR *c;
    unsigned kinds;
    int bld, i, at, amount;

    if (e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    txt_Format(news, 0x10c2, &ai_PlayerSlotsT[ai_OwnPlayer].name[0x20], (unsigned short)util_Random(3) + 0x10c4);
    kinds = (1 << 22) | (1 << 7);       /* ai_PassesBuildingFilter's kind mask: the building kinds 22 and 7 */
    bld = 0;
    for (i = 0; i < 0x300; i++) {
        c = &ai_CharsT[i];
        if (c->id != 0xffff && c->health >= 2 && (c->kind == 6 || c->kind == 7 || c->kind == 5)) {
            if (ai_PickActionBuilding((struct AIPER *)c, 4, kinds, 0.0f, 100.0f, &at) != 0)
                gm_FindById(&bld, 0, 0, at);
            if (c->kind == 6 || c->kind == 7)
                he_ShowNews(c->netId, -1, news, 0x20, 0x601, (char *)0);
            if (bld != 0) {
                amount = (int)(gm_GetWorth(c->id) * 0.01f) * ((unsigned short)util_Random(4) + 2);
                cm_SendMoveGoodsUnchecked(c->netId, -1, ai_OwnPlayer, amount);
                if (c->kind == 6 || c->kind == 7) {
                    txt_Format(line, 0x10c3, bld, amount);
                    he_ShowNews(c->netId, -1, line, 0, 0x5ff, (char *)0);
                }
            }
        }
        bld = 0;
    }
    he_FreeEvent((int *)ev);
}
/* 0x5485b0, the run handler of the master exam fee event 0x54: stage -1 / -2 releases its slot and frees it; once
   started, stage 0 opens the message box "Berufe\Messagebox_Meisterpruefung" with the news 0x10b1 (the character,
   the fee: 3% of its worth) and goes to stage 1; stage 1 waits for the answer: yes (0) moves the fee between the
   player's character and the event's character (cm_SendMoveGoods) and changes the character's relation to jobs by
   +8, no (1) by -6; both release the slot and free the event */

void he_RunMasterExamFee(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int fee;

    if (e->stage == -1 || e->stage == -2) {
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_CompareTime(&ai_Time, &e->start) == -1)
        return;
    switch (e->stage) {
    case 0:
        fee = (int)(gm_GetWorth(e->chr) * 0.03f);
        if (e->slot == 0)
            he_OpenEventSlot(ev, 0, 0, "Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        d2_SetForm(e->slot->win, 0);
        io_PrintText("$C");
        io_PrintText(0x10b1, e->chr, fee);
        e->stage = 1;
        return;
    case 1:
        if (e->slot == 0) {
            e->stage = 0;
            return;
        }
        if (d2_ClickedForm == e->slot->win && d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                cm_SendMoveGoods(he_PlayerCh->netId, ai_CharInts[e->chr][0], ai_OwnPlayer, (int)(gm_GetWorth(e->chr) * 0.03f));
                gm_ChangeRelationToJobs(e->chr, 8);
                he_ReleaseSlot(ev);
                he_FreeEvent((int *)ev);
                return;
            }
            if (d2_ClickedId == 1) {
                gm_ChangeRelationToJobs(e->chr, -6);
                he_ReleaseSlot(ev);
                he_FreeEvent((int *)ev);
                return;
            }
        }
    }
}
/* 0x548750, a Ghidra miss: the due clock = the start clock (+0x44), posted 3 units ahead */

void he_Unk548750(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 3, 0, 0);
}
/* 0x548790, a Ghidra miss: the run handler of the master exam payment event 0x55: stage -1 / -2 releases its slot
   and frees it; once started, stage 0 keeps the fee (5% of the character's worth), opens the message box
   "Berufe\Messagebox_Meisterpruefung" with the news 0x10b4 and goes to stage 1 (no box: frees the event); stage 1
   ends the event past its end time, else waits for the answer: yes (0) moves the fee between the player's character
   and the event's character (cm_SendMoveGoodsUnchecked) and posts the AI message 0x56 (flags 0x11) with it to the
   character, no (1) only ends it */

void he_RunMasterExamPay(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIMSG m;
    int fee;

    if (e->stage == -1 || e->stage == -2) {
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_CompareTime(&ai_Time, &e->start) == -1)
        return;
    switch (e->stage) {
    case 0:
        fee = (int)(gm_GetWorth(e->chr) * 0.05f);
        ((HEEXAMPAY *)e)->fee = fee;
        if (e->slot == 0)
            he_OpenEventSlot(ev, 0, 0, "Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        if (e->slot == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_SetForm(e->slot->win, 0);
        io_PrintText("$C");
        io_PrintText(0x10b4, e->chr, fee);
        e->stage = 1;
        return;
    case 1:
        if (gm_CompareTime(&ai_Time, &e->end) > 0) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (e->slot == 0) {
            e->stage = 0;
            return;
        }
        if (d2_ClickedForm == e->slot->win && d2_ClickedId != -1) {
            if (d2_ClickedId == 0) {
                memset(&m, 0, sizeof(m));
                fee = ((HEEXAMPAY *)e)->fee;
                cm_SendMoveGoodsUnchecked(he_PlayerCh->netId, ai_CharInts[e->chr][0], ai_OwnPlayer, fee);
                m.type = 0x56;
                m.fromId = -1;
                m.to = ai_CharInts[e->chr][0];
                m.flags = 0x11;
                ((HEFEEMSG *)&m)->what = 4;
                ((HEFEEMSG *)&m)->amount = fee;
                cm_SendLongMessage((char *)&m);
                he_ReleaseSlot(ev);
                he_FreeEvent((int *)ev);
                return;
            }
            if (d2_ClickedId == 1) {
                he_ReleaseSlot(ev);
                he_FreeEvent((int *)ev);
                return;
            }
        }
    }
}
/* 0x5489b0, a Ghidra miss: the run handler of the payment event 0x56: stage -2 frees it; at stage 0 it counts
   wait down, then rolls random(0x100) against the character's first skill / 2 + 64: below it the character gets
   twice the amount (the news 0x10b5), else half of it (the news 0x10b6), both by cm_SendMoveGoods from the own
   player; then frees the event */

void he_RunPayment(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEPAYMENT *p = (HEPAYMENT *)ev;
    char text[0x418];
    int r;

    if (e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage != 0)
        return;
    if (p->wait > 0) {
        p->wait--;
        return;
    }
    r = (unsigned short)util_Random(0x100);
    if (r < ai_CharsT[e->chr].skills[0] * 0.5 + 64.0) {
        txt_Format(text, 0x10b5, p->amount * 2);
        he_ShowNews(ai_CharInts[e->chr][0], ai_CharInts[e->chr][0], text, 0, 0x5ff, (char *)0);
        cm_SendMoveGoods(ai_CharInts[e->chr][0], -1, ai_OwnPlayer, p->amount * 2);
    } else {
        txt_Format(text, 0x10b6, (int)(p->amount * 0.5f));
        he_ShowNews(ai_CharInts[e->chr][0], ai_CharInts[e->chr][0], text, 0, 0x5ff, (char *)0);
        cm_SendMoveGoods(ai_CharInts[e->chr][0], -1, ai_OwnPlayer, (int)(p->amount * 0.5f));
    }
    he_FreeEvent((int *)ev);
}
/* 0x548b70, the init of a timed event: the clock +0x52 = the start (+0x44); a random half (0x42ec00 (2) != 0) frees it at once */

void he_Unk548b70(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    if ((short)util_Random(2) != 0)
        he_FreeEvent((int *)ev);
}
/* 0x548bb0, a Ghidra miss: the run handler of event 0x57: stage -2 frees it; at stage 0 it collects up to 16 of
   the character's buildings (owner query, gm_BuildQuery key 4) of class 1 or of type kind 9, picks one at random
   and sells a quarter (at least 1) of every good in its store object (0x2a, else 0x116) for the own player
   (cm_SendExSellObject, logged as "cm_RequestSellObjekt(..)" into a buffer nobody reads), adding up their prices;
   a character of kind 6 with a total gets the news 0x10b7 (the building, the total) in a quick-jump box; then frees
   the event */

void he_RunStoreSale(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    char text[0x418];
    char log[0x400];
    AIBLD *sel[16];
    int n = 0, total = 0, amount;
    AIBLD *b;
    AIBTYPE *t;
    AIOBJ *store;
    unsigned short *good;

    if (e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage != 0)
        return;
    b = (AIBLD *)gm_BuildQuery(1, 4, e->chr);
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    do {
        if (n >= 16)
            break;
        t = &ai_BTypesT[b->type];
        if (gm_BuildingClass(b->type) == 1 || t->kind == 9)
            sel[n++] = b;
    } while ((b = (AIBLD *)gm_BuildNext()) != 0);
    if (n == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = sel[(unsigned short)util_Random(n)];
    if (b != 0) {
        store = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x2a);
        if (store == 0)
            store = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x116);
        if (store != 0) {
            for (good = (unsigned short *)gm_Query((void *)store->list, 1, 5); good != 0; good = gm_QueryNext()) {
                amount = (int)(gm_ListAmount(store, good) * 0.25f);
                if (amount < 1)
                    amount = 1;
                sprintf(log, "cm_RequestSellObjekt(%i, %i, %i, %i, %i, %i)", -1, store->objId, *good, amount, ai_OwnPlayer, 0);
                cm_SendExSellObject(-1, store->objId, *good, amount, ai_OwnPlayer, 0);
                total = (int)(gm_GoodsPrice(*good, ai_OwnPlayer) * amount + total);
            }
        }
    }
    if (ai_CharsT[e->chr].kind == 6 && total != 0) {
        txt_Format(text, 0x10b7, b, total);
        he_MessageBoxQuickjump(ai_CharInts[e->chr][0], ai_CharInts[e->chr][0], text, 0, 0x5ec, b->id, -1, 0, (char *)0);
    }
    he_FreeEvent((int *)ev);
}
/* 0x548de0, a Ghidra miss: the run handler of event 0x58: stage -2 frees it; at stage 0 it collects up to 16 of
   the character's buildings (gm_BuildQuery key 4) of class 6, picks one at random and lowers its state by 50
   (gm_ChangeBuildingState); a character of kind 6 gets the news 0x10b8 (the building) in a quick-jump box; then
   frees the event */

void he_RunBuildingDecay(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    char text[0x418];
    AIBLD *sel[16];
    int n = 0;
    AIBLD *b;

    if (e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage != 0)
        return;
    b = (AIBLD *)gm_BuildQuery(1, 4, e->chr);
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    do {
        if (n >= 16)
            break;
        if (gm_BuildingClass(b->type) == 6)
            sel[n++] = b;
    } while ((b = (AIBLD *)gm_BuildNext()) != 0);
    if (n == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = sel[(unsigned short)util_Random(n)];
    if (b != 0)
        gm_ChangeBuildingState((unsigned char *)b, -50);
    if (ai_CharsT[e->chr].kind == 6) {
        txt_Format(text, 0x10b8, b);
        he_MessageBoxQuickjump(ai_CharInts[e->chr][0], ai_CharInts[e->chr][0], text, 0, 0x5ec, b->id, -1, 0, (char *)0);
    }
    he_FreeEvent((int *)ev);
}
/* 0x548ef0, a Ghidra miss: the start handler: the due clock +0x52 = now, posted 0x60 units ahead */

void he_Unk548ef0(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x60, 0, 0);
}
/* 0x548f30, the run handler of the HE event type 0x5a, a one-shot event without the pending-slot check of
   0x548230: a freed state (-2) ends it, a negative one frees it, and in state 0 with opts bit 2 it is posted
   again two minutes later (the end time = the clock now, cm_SendExSetHE) */

void he_RunRepostEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int st = e->stage;

    if (st < -2)
        return;
    if (st >= 0) {
        if (st != 0)
            return;
        if ((e->opts & 2) == 0)
            return;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x548fa0, a repeating payment: every 0x18 units the player of the event (word +8) gets +0xac (0x4c47e0, from the bank -1) for +0xb0 days */

void he_Unk548fa0(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st >= -2) {
        if (st >= 0) {
            if (st != 0)
                return;
            cm_SendMoveGoods(-1, U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, *(int *)(ev + 0xac));
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
            if (*(int *)(ev + 0xb0) != 0) {
                *(AITIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0x18, 0, 0);
                return;
            }
        }
        he_FreeEvent((int *)ev);
    }
}
/* 0x549040, a Ghidra miss: the due clock +0x52 = now, posted 5 units ahead */

void he_Unk549040(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
/* 0x549080, like 0x548fa0 but the amount -+0xac goes to the player through 0x53eec0 */

void he_Unk549080(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st >= -2) {
        if (st >= 0) {
            if (st != 0)
                return;
            he_AddApEvent(*(unsigned short *)(ev + 8), U_I((unsigned)&full_data_1442500), -*(int *)(ev + 0xac), 0);
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
            if (*(int *)(ev + 0xb0) != 0) {
                *(AITIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0x18, 0, 0);
                return;
            }
        }
        he_FreeEvent((int *)ev);
    }
}
/* 0x549110, a Ghidra miss: the init: state 0, +0xac = 0 */

void he_Unk549110(int ev)
{
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0xac) = 0;
}
/* 0x549120, a Ghidra miss (disassembled): the run handler of the offspring event 0x5d: frees it when the character or
   its partner (relatives[0]) is dead or at stage -2. Stage 0 counts the living children (relatives[3..7]); with all
   five it frees the event, else f = ((5 - n) * 0.2)^2 and a roll random + (1 - f) < f * chance moves it to stage 1
   (one year on), otherwise chance grows by f * 0.1. Stage 1 waits three runs (a year each); then a character of kind
   6 gets a party of kind 7 (flags 0x20) with its partner half an hour from now, and cm_SendCreateCharacter creates the
   child (the two parents in an order by the character's sex, the home building's id or -1); back to stage 0, 1 .. 8 years on */

void he_RunOffspring(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEOFFSPRING *s = (HEOFFSPRING *)ev;
    G28REC party;
    int home = -1;
    AICHAR *c, *partner;
    int *child;
    int i, n;
    float f, odds;

    c = &ai_CharsT[e->chr];
    partner = (AICHAR *)gm_FindChar(c->relatives[0]);
    if (partner == 0 || partner->health == 0 || c->health == 0 || e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage != 0) {
        if (e->stage == 1) {
            if (s->chance >= 3.0) {
                if (c->kind == 6) {
                    memset(&party, 0, sizeof(party));
                    party.host = c->netId;
                    party.members[0] = c->netId;
                    party.members[1] = partner->netId;
                    party.kind = 7;
                    party.flags = 0x20;
                    party.thing = -1;
                    party.used = 2;
                    party.time = ai_Time;
                    gm_AddTime(&party.time, 0, 0x1e, 0);
                    cm_SendNewParty(&party);
                }
                s->chance = 0;
                if (c->bld2 != 0)
                    home = c->bld2->id;
                if (c->sex == 0)
                    cm_SendCreateCharacter(8, c->netId, partner->netId, 0, home, 0, 0, 2);
                else
                    cm_SendCreateCharacter(8, partner->netId, c->netId, 0, home, 0, 0, 2);
                e->stage = 0;
                e->end.year += (unsigned short)util_Random(8) + 1;
            } else {
                s->chance += 1.0;
                e->end.year++;
            }
        }
    } else {
        n = 0;
        child = &c->relatives[3];
        for (i = 5; i != 0; i--) {
            if (*child != -1 && gm_FindChar(*child) != 0)
                n++;
            child++;
        }
        if (n == 5) {
            he_FreeEvent((int *)ev);
            return;
        }
        f = (5.0f - n) * 0.2f;
        f = f * f;
        odds = f * s->chance;
        if (util_RandomF() + (1.0f - f) < odds) {
            e->end.year++;
            e->stage = 1;
            s->chance = 0;
        } else
            s->chance += f * 0.1;
    }
}
/* 0x5493a0, the alloc handler of the trial event (type 0x5e, the run handler 0x549430): it looks for another
   running 0x5e event of its player (he_FindEvent (2, 0, 0x5e, 2, chr), skipping itself); none: the event starts
   in state 0 with unkb8 cleared and is posted, else it ends at the clock now (posted as failed) */

void he_StartTrialEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIJOB *found = (AIJOB *)he_FindEvent(2, 0, 0x5e, 2, e->chr);

    if (found != 0) {
        do {
            if (found == e) {
                found = (AIJOB *)he_NextEvent();
            } else {
                e->end = ai_Time;
                e->exSlot = cm_SendExSetHE(-1, ev);
                return;
            }
        } while (found != 0);
    }
    e->stage = 0;
    e->rounds = 0;
    e->exSlot = cm_SendExSetHE(0, ev);
}
/* 0x549430, a Ghidra miss (disassembled): the run handler of the trial event 0x5e (the character e->chr's trial
   with the teacher, a character found by its netId): stage -2 / -1 frees it; nothing while it is not started or
   its last post is not settled. Stage 0 holds a round: the news 0x1a9b, a day on, the round's cost to the
   teacher's AP events; the third round posts the event to stage 1. Stage 1 raises the two abilities of the job's
   title group (gm_JobSlots) by 16 .. 31 each, gives the character the job and tells the teacher (the news 0x1a9c
   with the job title, the female forms 0x5f on); without the teacher it ends now */

void he_RunTrialEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HETRIAL *t = (HETRIAL *)ev;
    char text[0x100];
    AICHAR *c, *teacher;
    int skillA, skillB;
    unsigned char raise;

    if (e->stage == -2 || e->stage == -1) {
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
        c = &ai_CharsT[e->chr];
        teacher = (AICHAR *)gm_FindChar(t->teacher);
        if (teacher == 0) {
            e->end = ai_Time;
            cm_SendExSetHE(-1, ev);
            return;
        }
        txt_Format(text, 0x1a9b, c->id);
        e->rounds++;
        e->end.hour += 0x18;
        cm_SendExSetHE(e->rounds >= 3, ev);
        he_AddApEvent(teacher->id, text, 0, -t->cost);
        return;
    case 1:
        c = &ai_CharsT[e->chr];
        gm_JobSlots(gm_TitleGroup(t->job), &skillA, &skillB);
        cm_BeginFieldBlock((char *)c, c->netId);
        raise = (unsigned char)(util_Random(0x10) + 0x10);
        cm_AddFieldValues(1, 1, skillA + 0x80, &raise);  /* 0x80: AICHAR.skills */
        raise = (unsigned char)(util_Random(0x10) + 0x10);
        cm_AddFieldValues(1, 1, skillB + 0x80, &raise);
        cm_SendFieldDeltas();
        cm_SendSetJob(c->netId, t->job);
        teacher = (AICHAR *)gm_FindChar(t->teacher);
        if (teacher == 0) {
            e->end = ai_Time;
            cm_SendExSetHE(-1, ev);
            return;
        }
        txt_Format(text, 0x1a9c, c->id, (c->sex != 0 ? 0x5f : 0) + 0x130 + t->job);
        he_ShowNews(teacher->netId, teacher->netId, text, 0, 0x5f4, (char *)0);
        e->end = ai_Time;
        cm_SendExSetHE(-1, ev);
        return;
    }
}
/* 0x5496f0, the alloc handler of the next trial step event (type 0x5f, the run handler 0x549770), like the
   trial's own alloc handler 0x5493a0 without the posting: it looks for another running 0x5f event of its player
   (he_FindEvent (2, 0, 0x5f, 2, chr), skipping itself); none: the event starts in state 0 with its step count
   cleared, else it ends at the clock now */

void he_StartNextTrialStep(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIJOB *found = (AIJOB *)he_FindEvent(2, 0, 0x5f, 2, e->chr);

    if (found != 0) {
        do {
            if (found == e) {
                found = (AIJOB *)he_NextEvent();
            } else {
                e->end = ai_Time;
                e->exSlot = cm_SendExSetHE(-1, ev);
                return;
            }
        } while (found != 0);
    }
    e->stage = 0;
    e->steps = 0;
}
/* 0x549770, a Ghidra miss (disassembled): the run handler of the next trial step event 0x5f (the character e->chr
   taught by the teacher, a character found by its netId): stage -2 / -1 frees it; nothing while it is not started
   or its last post is not settled. Stage 0 is a step: both skills (ai_ApplyTrialStep) raised, the news 0x1a9e, a
   day on, the step's cost to the teacher's AP events; the fifth step moves it to stage 1. Stage 1 tells the
   teacher (the news 0x1a9f), counts the course in the teacher's player record and ends the event now; without
   the teacher it ends now */

void he_RunTrialSteps(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HETRIALSTEP *t = (HETRIALSTEP *)ev;
    char text[0x100];
    AICHAR *c, *teacher;
    HEFINVIEW *fin;

    if (e->stage == -2 || e->stage == -1) {
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
        c = &ai_CharsT[e->chr];
        teacher = (AICHAR *)gm_FindChar(t->teacher);
        if (teacher == 0) {
            e->end = ai_Time;
            cm_SendExSetHE(-1, ev);
            return;
        }
        ai_ApplyTrialStep(c, t->skill[0]);
        ai_ApplyTrialStep(c, t->skill[1]);
        txt_Format(text, 0x1a9e, c->id);
        e->steps++;
        e->stage = e->steps >= 5;
        e->end.hour += 0x18;
        he_AddApEvent(teacher->id, text, 0, -t->cost);
        return;
    case 1:
        c = &ai_CharsT[e->chr];
        teacher = (AICHAR *)gm_FindChar(t->teacher);
        if (teacher != 0) {
            txt_Format(text, 0x1a9f, c->id);
            he_ShowNews(teacher->netId, teacher->netId, text, 0, 0x5f4, (char *)0);
            fin = (HEFINVIEW *)gm_PlayerRecord((char *)teacher);
            if (fin != 0)
                fin->taught++;
        }
        e->end = ai_Time;
        cm_SendExSetHE(-1, ev);
        return;
    }
}
/* 0x549970, a Ghidra miss (disassembled): the alloc handler of the training event 0x60: frees it when the trained
   skill of its character is at the cap 252 already, or when the character has another 0x60 event running
   (he_FindEvent (2, 2, chr, 0, 0x60), skipping itself); else it raises the skill by 0x15 less a random part of
   2 * (skill / 42 + 3), starts at stage 0, due 5 hours from now (a second with gm_AllowAll), started now */

void he_StartTraining(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HETRAINING *t = (HETRAINING *)ev;
    AIJOB *found;

    if ((float)ai_CharsT[e->chr].skills[t->skill] >= 252.0f) {
        he_FreeEvent((int *)ev);
        return;
    }
    found = (AIJOB *)he_FindEvent(2, 2, e->chr, 0, 0x60);
    if (found != 0) {
        do {
            if (found != e) {
                he_FreeEvent((int *)ev);
                return;
            }
            found = (AIJOB *)he_NextEvent();
        } while (found != 0);
    }
    t->range = (unsigned char)((int)(ai_CharsT[e->chr].skills[t->skill] * (1.0f / 42.0f) + ai_FloatOne) + 2) * 2;
    t->base = 0x15;
    e->stage = 0;
    e->end = ai_Time;
    if (gm_AllowAll != 0)
        gm_AddTime(&e->end, 0, 0, 1);
    else
        gm_AddTime(&e->end, 5, 0, 0);
    e->start = ai_Time;
}
/* 0x549aa0, the run handler of the training event 0x60: stage -2 frees it; at stage 0 the skill rises by base less
   a random part of range, at most to the cap 252 (cm_SendAddSkill). For a player character (kind 6) a new level
   (the value / 42) posts the AI message 0x6a (flags 9) with the skill and its level, else the news 0x16f2 with
   the skill's name 0x16f3 + skill and the sound "tal_0<skill>"; then the skills' message (cm_SendSkillsChanged)
   and the event is freed
   @deep read fully against the original: every statement, callee, constant (252.0f, 1/42), string
   ('tal_0%i', '%s%s'), the AIMSG 0x6a fields (type, to, fromId -1, time, flags 9, skill +0x58, level
   +0x5c) and the switch on stage (-2 frees, 0 runs) match; same size (575 vs 575, 165 ins each). Left: the
   first skill read's address form and one store's place: the original +0x61 'mov al, byte ptr [ecx + edx*4
   + ai_Chars+0x80]' (c->skills[skill] folded with ai_Chars) and stores c ([ebp-0xc]) after it, ours
   computes c first, 'mov [ebp-0xc], ebx; mov al, [ebx + ecx + 0x80]'; and +0x65 [eax + ebx + 0x80] vs [ebx
   + eax + 0x80] (operand order). Tried: the first read as ai_CharsT[e->chr].skills[skill] before / after c
   (VC6 then CSEs the second read into the absolute form too: M8, 22 ins worse), c assigned at the case's
   start (31 ins worse), if-chains instead of the switch (the free tail moves). A CSE / addressing choice:
   deep work */

void he_RunTraining(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HETRAINING *t = (HETRAINING *)ev;
    char text[0x800];
    char sound[0x20];
    AIMSG msg;
    AICHAR *c;
    int rise, skill, cur, level;

    switch (e->stage) {
    case -2:
        he_FreeEvent((int *)ev);
        return;
    case 0:
        rise = t->base - (unsigned short)util_Random(t->range);
        skill = t->skill;
        c = &ai_CharsT[e->chr];
        cur = c->skills[skill];
        if ((float)(cur + rise) > 252.0f)
            rise = (int)(252.0f - (float)cur);
        cm_SendAddSkill(c->netId, skill, rise);
        if (c->kind == 6) {
            skill = t->skill;
            cur = c->skills[skill];
            level = (int)((cur + rise) * (1.0f / 42.0f));
            if (level > (int)(cur * (1.0f / 42.0f))) {
                MEM_ZERO(&msg, sizeof(msg));
                msg.type = 0x6a;
                msg.to = c->netId;
                msg.fromId = -1;
                msg.time = ai_Time;
                msg.flags = 9;
                ((HESKILLMSG *)&msg)->skill = t->skill;
                ((HESKILLMSG *)&msg)->level = level;
                cm_SendLongMessage((char *)&msg);
            } else {
                sprintf(sound, "tal_0%i", skill);
                txt_Format(text, "%s%s", 0x16f2, t->skill + 0x16f3);
                he_ShowNews(ai_CharInts[e->chr][0], ai_CharInts[e->chr][0], text, 0, 0x5fd, sound);
            }
        }
        cm_SendSkillsChanged(c->netId);
        he_FreeEvent((int *)ev);
    }
}
/* 0x549ce0, begins the plague event 0x59 (its run handler 0x54a020 calls it at stage 5): from a random one of the
   0x100 buildings on, the first whose type is of kind 7 starts it: a random walk index and step, the AI message
   0x78 (flags 9) about it to every player character (kind 6 / 7), four carriers ("PEST", kind 0xe, job 0x12)
   created at it, it is their home; due a minute from now, posted at stage 0, and the plague news
   (ai_PlagueNews). Without such a building the event is posted as failed */

void he_BeginPlague(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEPLAGUE *p = (HEPLAGUE *)ev;
    AIMSG msg;
    HEPLAGUEMSG *m = (HEPLAGUEMSG *)&msg;
    AIBLD *b;
    int n = 0x100;
    int i;

    e->end = ai_Time;
    i = (unsigned short)util_Random(0x100);
    do {
        b = &ai_BldsT[i];
        if (ai_BTypesT[b->type].kind == 7) {
            p->walk = (unsigned short)util_Random(0x100);
            p->step = ai_RandomStrides[(unsigned short)util_Random(0x10)];
            D3_ZERO(&msg, sizeof(msg));
            m->bld = b->id;
            m->target = b->id;
            m->next = -1;
            m->walk = p->walk;
            m->step = p->step;
            m->left = p->left;
            msg.fromId = -1;
            msg.type = 0x78;
            msg.flags = 9;
            msg.time = ai_Time;
            for (i = 0; i < 768; i++) {
                if (ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7) {
                    msg.to = ai_CharsT[i].netId;
                    cm_SendLongMessage((char *)&msg);
                }
            }
            p->carriers[0] = cm_SendSpawnCharacterR(0xe, b->id, -1, 0x12, 0, 0, "PEST");
            p->carriers[1] = cm_SendSpawnCharacterR(0xe, b->id, -1, 0x12, 0, 0, "PEST");
            p->carriers[2] = cm_SendSpawnCharacterR(0xe, b->id, -1, 0x12, 0, 0, "PEST");
            p->carriers[3] = cm_SendSpawnCharacterR(0xe, b->id, -1, 0x12, 0, 0, "PEST");
            p->home = b->id;
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(0, ev);
            ai_PlagueNews();
            return;
        }
        n--;
        i = (i + 1) % 0x100;
    } while (n != 0);
    e->exSlot = cm_SendExSetHE(-1, ev);
}

int sv_IsAtDoor();
void cm_SendKillCharacter(int a, int b);
void ai_PlagueMessage(int param_1);
float gm_CatchPlague();
float util_RandomF2();
/* 0x549f70, a Ghidra miss: the start of the plague event ("Pest"): unless a running event of the type 0x59 exists the clock is copied from the start (+0x44), the state 5 is set and the event is posted (0x4c4c60 (5, ev)); else it is posted as failed */
void he_Unk549f70(int ev)
{
    int e;

    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    e = (int)he_FindEvent(1, 0, 0x59);
    while (e != 0) {
        if (e != ev) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        e = (int)he_NextEvent();
    }
    *(int *)(ev + 0x70) = 5;
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(int *)(ev + 0x84) = -1;
    cm_SendExSetHE(5, ev);
}
/* 0x54a020, a Ghidra miss (disassembled; 1720 B): the run handler of the plague event 0x59 ("Pest"): stage -1 / -2
   stops and kills the four carriers when the event is freed with the option 2, and frees it; nothing while it is
   not started or its last post is not settled. Stage 5 begins it (he_BeginPlague). Stage 0 turns the carriers'
   creation slots into their ids (-1: not created). Stage 1 walks the 0x100 buildings by the step from the walk
   index to the next owned building of a kind in the mask (kinds 4, 8, 14, 18 .. 22, 31, 32; the debug text
   "EvtPest: next target = %G") and sends the carriers there ("Pest"); none: it ends. Stage 2 waits until every
   carrier is at the target's door, then infects: of the target's owner's building up to 16 characters of kind 0 in
   it, a random share of them (at least one) and with 1 in 8 a character of kind 1 in it catch the plague
   (gm_CatchPlague), the owner with 1 in 256; any infection posts the plague message, and each value below 1000
   makes the character ill (he_CharIllness, illness 1). While left counts down it goes on to the next target,
   else the carriers go home. Stage 3 waits until every carrier is home, then the event ends
   @deep read in full against the original (576 instructions): the four-stage switch (jump table equal),
   the carriers loops, the building walk with the 64-bit kind mask 0x1807c4110 and the % 0x100 steps,
   txt_Format of the debug text, the owner / kind-0 / kind-1 infection block (memset ids -1, value 1000.0f,
   util_RandomF2 > 0.8f -> 1, * cnt, !(a > 1) -> 1, gm_CatchPlague, the 1/8 and 1/256 rolls,
   ai_PlagueMessage(owner, o), he_CharIllness(e, c, owner, o, 1, v) below 1000.0f), --left <= 0 sends the
   carriers home (1 / 10 minutes) else posts stage 1; every callee, constant and string checked. Fixed in
   this pass: sprintf -> txt_Format, the owner argument, the 0.8 / 1000.0 literals, the home wait 1 not 2
   minutes, stage 3's missing-carrier test, the start / target words (0xd0 / 0xcc). Left only tail / order
   hunks (diffclass: tail 80, order 20, no code hunk): VC6's choice of which identical return tails to
   cross-jump (see the size note), and the loop latch's scheduling at 0x54a172 (o = 0 / --n against the %
   fixup). Deep work (size: 20 of the instruction difference are shared / inline tails) (references: M8 at
   +0x381: ours refers to gm_AddTime (0x4fe140), the original's field to 0x4c4c60 (cm_SendExSetHE),
   explained: the M8 at +0x381 is the tail alignment: ours ends the stage-2 wait block with 'call
   gm_AddTime; jmp' into stage 1's identical 'push ebp; push 2; call cm_SendExSetHE' tail, the original
   keeps that tail inline, so the aligner pairs our gm_AddTime call with the original's cm_SendExSetHE
   call; both sides call gm_AddTime(&end, 0, 5, 0) then cm_SendExSetHE(2, ev) there) */

void he_RunPlague(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEPLAGUE *p = (HEPLAGUE *)ev;
    AIBLD *o;
    AIBTYPE *bt;
    AICHAR *c, *owner, *other;
    int *carrier;
    unsigned lo, hi;
    int i, n, k, cnt, m, any, r;
    float a, otherValue;
    float value[16];
    int ids[16];
    char text[0x200];

    if (e->stage == -1 || e->stage == -2) {
        if (e->opts & 2) {
            carrier = p->carriers;
            for (i = 4; i != 0; i--) {
                c = (AICHAR *)gm_FindChar(*carrier);
                if (c != 0) {
                    cm_SendChrStop(c->netId);
                    cm_SendKillCharacter(c->netId, 1);
                }
                carrier++;
            }
        }
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->opts & 4)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    if (e->stage == 5)
        he_BeginPlague(ev);
    switch (e->stage) {
    case 0:
        carrier = p->carriers;
        for (i = 4; i != 0; i--) {
            if (cm_GetSlotState(*carrier) == 1 && (r = cm_GetSlotValue(*carrier)) != 0)
                *carrier = ((AICHAR *)r)->netId;
            else
                *carrier = -1;
            carrier++;
        }
        gm_AddTime(&e->end, 0, 1, 0);
        e->exSlot = cm_SendExSetHE(1, ev);
        return;
    case 1:
        n = 0x100;
        k = p->walk;
        do {
            o = &ai_BldsT[k];
            hi = 0;
            bt = &ai_BTypesT[o->type];
            lo = 0;
            if ((unsigned char)bt->kind > 0x1f)
                hi = 1 << ((unsigned char)bt->kind - 0x20);
            else
                lo = 1 << (unsigned char)bt->kind;
            if (o->owner != 0xffff && ((lo & 0x807c4110) != 0 || (hi & 1) != 0))
                break;
            n--;
            o = 0;
            k = (k + p->step) % 0x100;
        } while (n != 0);
        p->walk = (p->step + k) % 0x100;
        if (o == 0) {
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        txt_Format(text, "EvtPest: next target = %G", o);
        n = 0;
        carrier = p->carriers;
        for (i = 4; i != 0; i--) {
            c = (AICHAR *)gm_FindChar(*carrier);
            if (c != 0) {
                cm_SendChrStop(c->netId);
                cm_SendChrGotoBuilding(c->netId, o->id, -1, 0, 1, "Pest");
                n++;
            }
            carrier++;
        }
        gm_AddTime(&e->end, 0, n > 0 ? 5 : 0x14, 0);
        p->target = o->id;
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 2:
        gm_FindById(&o, 0, 0, p->target);
        gm_AddTime(&e->end, 0, 5, 0);
        if (o == 0) {
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        for (i = 0; i < 4; i++) {
            c = (AICHAR *)gm_FindChar(p->carriers[i]);
            if (c != 0 && sv_IsAtDoor(c, o) == 0) {
                e->stage = 2;
                gm_AddTime(&e->end, 0, 5, 0);
                e->exSlot = cm_SendExSetHE(2, ev);
                return;
            }
        }
        if (o->owner != 0xffff) {
            any = 0;
            other = 0;
            otherValue = 1000.0f;
            owner = &ai_CharsT[o->owner];
            memset(ids, -1, sizeof(ids));
            cnt = 0;
            for (k = 0; k < 768; k++) {
                if (cnt >= 16)
                    break;
                if (ai_CharsT[k].kind == 0 && ai_CharsT[k].bld == o) {
                    value[cnt] = 1000.0f;
                    ids[cnt] = k;
                    cnt++;
                } else if (ai_CharsT[k].kind == 1 && ai_CharsT[k].bld == o)
                    other = &ai_CharsT[k];
            }
            if (cnt != 0) {
                a = util_RandomF2();
                if (a > 0.8f)
                    a = ai_FloatOne;
                a = a * cnt;
                if (!(a > ai_FloatOne))
                    a = ai_FloatOne;
                m = (int)a;
                for (i = 0; i < m; i++) {
                    any = 1;
                    value[i] = gm_CatchPlague(&ai_CharsT[ids[i]]);
                }
                if (other != 0 && (unsigned short)util_Random(8) == 0) {
                    otherValue = gm_CatchPlague(other);
                    any = 1;
                }
                if ((unsigned short)util_Random(0x100) == 0)
                    gm_CatchPlague(owner);
                if (any)
                    ai_PlagueMessageK(owner, o);
                for (i = 0; i < m; i++) {
                    if (value[i] < 1000.0f)
                        he_CharIllness(e, &ai_CharsT[ids[i]], owner, o, 1, value[i]);
                }
                if (otherValue < 1000.0f)
                    he_CharIllness(e, other, owner, o, 1, otherValue);
            }
        }
        if (--p->left <= 0) {
            gm_FindById(&o, 0, 0, p->home);
            if (o == 0) {
                e->exSlot = cm_SendExSetHE(-1, ev);
                return;
            }
            n = 0;
            carrier = p->carriers;
            for (i = 4; i != 0; i--) {
                c = (AICHAR *)gm_FindChar(*carrier);
                if (c != 0) {
                    cm_SendChrStop(c->netId);
                    cm_SendChrGotoBuilding(c->netId, o->id, -1, 0, 1, "Pest");
                    n++;
                }
                carrier++;
            }
            gm_AddTime(&e->end, 0, n > 0 ? 1 : 10, 0);
            e->exSlot = cm_SendExSetHE(3, ev);
        } else
            e->exSlot = cm_SendExSetHE(1, ev);
        return;
    case 3:
        gm_AddTime(&e->end, 0, 5, 0);
        gm_FindById(&o, 0, 0, p->home);
        if (o == 0) {
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        for (i = 0; i < 4; i++) {
            c = (AICHAR *)gm_FindChar(p->carriers[i]);
            if (c != 0 && sv_IsAtDoor(c, o) == 0) {
                e->stage = 3;
                gm_AddTime(&e->end, 0, 5, 0);
                e->exSlot = cm_SendExSetHE(3, ev);
                return;
            }
        }
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
}
/* 0x54a6f0, a Ghidra miss: the alloc handler of the pickpocket gang event 0x61: counts the members (the ids[]
   not -1); without any it frees the event, else no targets yet, the first member next, started now and due two
   minutes from now */

void he_StartGang(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGANG *g = (HEGANG *)ev;
    int *id;
    int i;

    g->members = 0;
    id = e->ids;
    for (i = 6; i != 0; i--) {
        if (*id++ != -1)
            g->members++;
    }
    if (g->members == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    memset(g->targets, -1, sizeof(g->targets));
    g->next = 0;
    g->start = ai_Time;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 2, 0);
}

int gm_GetGoodsValue();
void *gm_GetGebaeudeHauptraum(unsigned char *b);

/* 0x54a7b0, a Ghidra miss (disassembled; 1646 B): the run handler of the pickpocket gang event 0x61 ("Taschendieb";
   the gang is ids[0..5], its home building e->from, whose owner is the leader when it has one). Freed (stage -2),
   without a leader or 0x1e0 minutes after the start it goes to stage 1, else it takes the next member found (none:
   back to the first, wait) and is due in six minutes. Stage 1 calls the gang home: each member's take (3 .. 6
   rolls of its skill 2's level) adds up, the leader gets it (gm_PlayerFactor, + 1% per skill point of the home
   building), the building's round sum and the leader's player record count it, and a leader of kind 6 without a
   master with the flag 1 gets the news 0x19f4; then the event is freed. Stage 0 lets an idle member steal at its
   target: of up to 32 random characters (alive, kind below 10, not of title 1 / 2, not the leader) the first
   whose goods are worth the take; it notices the thief when its skill 2 beats the thief's by more than a roll of
   0x7e (the news 0x19f3), else it loses the take (the news 0x19f2); then the member walks to a random building of
   a random other kind (10 / 5 / 7 / 0x16 / 0xf, kind 10 when there is none) */

void he_RunGang(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGANG *g = (HEGANG *)ev;
    AICHAR *member, *leader, *c, *victim;
    AIBLD *s, *b, *o;
    AIBTYPE *bt;
    AIOBJ *room;
    HEFINVIEW *fin;
    int *id;
    int dt, idx, i, k, n, r, lvl, loot, sum, v, amt, tries, kind, diff;
    int pick[5];                        /* the building kinds to pick from, then the buildings of the kind */
    char text[0x400];

    dt = gm_MinutesBetween(&g->start, &ai_Time);
    member = 0;
    idx = 0;
    leader = (AICHAR *)gm_FindChar(g->leader);
    s = (AIBLD *)gm_BuildQuery(1, 1, e->from);
    if (s != 0 && s->owner != 0xffff)
        leader = &ai_CharsT[s->owner];
    if (e->stage == -2 || leader == 0) {
        e->stage = 1;
    } else {
        member = 0;
        for (;;) {
            if (g->next < 6) {
                member = (AICHAR *)gm_FindChar(e->ids[g->next]);
                idx = g->next;
                g->next++;
                if (member == 0)
                    continue;
            } else if (member == 0) {
                g->next = 0;
                return;
            }
            break;
        }
        e->stage = dt > 0x1e0;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 6, 0);
        g->next = g->next % 6;
    }
    switch (e->stage) {
    case 0:
        if (member == 0 || member->scene->actions != 0)
            return;
        b = (AIBLD *)gm_BuildQuery(1, 1, g->targets[idx]);
        n = 0;
        pick[0] = 10;
        pick[1] = 5;
        pick[2] = 7;
        pick[3] = 0x16;
        pick[4] = 0xf;
        kind = pick[(unsigned short)util_Random(5)];
        if (b != 0) {
            lvl = (int)(member->skills[2] * (1.0f / 42.0f) + ai_FloatOne);
            loot = ((unsigned short)util_Random((lvl + 1) * lvl) + lvl * 2) * 2;
            v = gm_PlayerFactor(loot, ai_OwnPlayer);
            if (g->members != 0) {
                victim = 0;
                tries = 0x20;
                do {
                    r = (unsigned short)util_Random(0x300);
                    tries--;
                    if (ai_CharsT[r].id != 0xffff && ai_CharsT[r].health > 0 && ai_CharsT[r].kind < 10 && ai_CharsT[r].title != 2
                        && ai_CharsT[r].title != 1 && ai_CharsT[r].netId != leader->netId) {
                        victim = &ai_CharsT[r];
                        if (gm_GetGoodsValue(victim) < v)
                            victim = 0;
                    }
                } while (victim == 0 && tries != 0);
                g->members--;
                if (victim != 0) {
                    diff = victim->skills[2] - member->skills[2];
                    if (diff > (unsigned short)util_Random(0x7e)) {
                        if (victim->kind == 6 || victim->kind == 7) {
                            txt_Format(text, 0x19f3);
                            he_ShowNews(victim->netId, victim->netId, text, 0, 0x5f0, (char *)0);
                        }
                    } else {
                        cm_SendMoveGoodsUnchecked(-1, victim->netId, ai_OwnPlayer, v);
                        if (victim->kind == 6 || victim->kind == 7) {
                            txt_Format(text, 0x19f2, v);
                            he_ShowNews(victim->netId, victim->netId, text, 0, 0x5f0, "_NACHRICHTEN_HS_19");
                        }
                    }
                }
            }
            bt = &ai_BTypesT[b->type];
            do
                kind = pick[(unsigned short)util_Random(5)];
            while ((unsigned char)bt->kind == kind);
        }
        o = (AIBLD *)gm_BuildQuery(1, 5, kind);
        if (o == 0)
            o = (AIBLD *)gm_BuildQuery(1, 5, 10);
        else {
            do {
                if (n >= 4)
                    break;
                pick[n++] = (int)o;
            } while ((o = (AIBLD *)gm_BuildNext()) != 0);
            o = (AIBLD *)pick[(unsigned short)util_Random(n)];
        }
        sv_MoveToGroup((unsigned char *)o, 0, member->id, (char *)ev);
        cm_SendChrStop(member->netId);
        cm_SendChrGotoBuilding(member->netId, o->id, -1, 0, 1, "Taschendieb");
        g->targets[idx] = o->id;
        return;
    case 1:
        s = (AIBLD *)gm_BuildQuery(1, 1, e->from);
        sum = 0;
        if (s != 0 && leader != 0) {
            room = (AIOBJ *)gm_GetGebaeudeHauptraum((unsigned char *)s);
            id = e->ids;
            for (i = 6; i != 0; i--) {
                c = (AICHAR *)gm_FindChar(*id);
                if (c != 0) {
                    lvl = (int)(c->skills[2] * (1.0f / 42.0f) + ai_FloatOne);
                    for (k = (unsigned short)util_Random(4) + 3; k > 0; k--)
                        sum += ((unsigned short)util_Random((lvl + 1) * lvl) + lvl * 2) * 2;
                    cm_SendChrStop(c->netId);
                    cm_SendChrGotoBuilding(c->netId, s->id, room->objId, 0, 0, "Taschendieb");
                    sv_MoveToGroup(0, 0, c->id, 0);
                }
                id++;
            }
            v = gm_PlayerFactor(sum, ai_OwnPlayer);
            amt = (int)((gm_BuildingSkill(s, 1, 1) * 0.01f + ai_FloatOne) * v);
            cm_SendMoveGoodsUnchecked(leader->netId, -1, ai_OwnPlayer, amt);
            s->roundSums[0] += amt;
            if (leader->kind == 6) {
                fin = (HEFINVIEW *)gm_PlayerRecord((char *)leader);
                if (fin != 0)
                    fin->gangTakings += amt;
                c = (AICHAR *)gm_FindCharacterByMaster(s);
                if (c == 0 || (c->flags1b4 & 1) == 0) {
                    txt_Format(text, 0x19f4, amt);
                    he_ShowNews(leader->netId, leader->netId, text, 0, 0x5f0, ch_EmptyName);
                }
            }
        }
        he_FreeEvent((int *)ev);
        return;
    }
}

int ls_GetLanguage();
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay);

/* 0x54ae20, a Ghidra miss: the start handler: the due clock +0x52 = now, posted 2 units ahead */
void he_Unk54ae20(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 2, 0);
}
/* 0x54ae60, a Ghidra miss: the run handler of the event 0x63, the protection money question to a player victim
   ("Berufe\Messagebox_SCHUTZGELD"): it answers the protection money event 0x62 whose id it holds (HEPROTECTION).
   A gone event or victim building frees it. Stage 0 opens the window with the news 0x1a8c (the collector, the
   amount and the building, their order by the language) and gives the answer three hours; stage 1 frees it when
   that is over, else waits for a button: 0 pays (the event 0x62 goes on in stage 3 with the answer 1, the
   building gets the flag 0x100 at +0x5a, its owner pays the collector, the collector's home building counts the
   money), 1 refuses (the answer 0); the answer's time is now (offline: 6:00 of today) */

void he_RunProtectionAsk(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIJOB *p;
    HEPROTECTION *q;
    AIBLD *b = 0, *home;
    AITIME now;

    p = (AIJOB *)he_FindEvent(1, 1, e->u.id);
    q = (HEPROTECTION *)p;
    if (p != 0)
        b = (AIBLD *)gm_BuildQuery(1, 1, q->victim);
    if (e->stage == -2 || e->stage == -1 || p == 0 || b == 0) {
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
    }
    switch (e->stage) {
    case 0:
        he_OpenEventSlot(ev, 0, 0, (int)"Berufe\\Messagebox_SCHUTZGELD", 0x5f3);
        if (e->slot == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_SetForm(e->slot->win, 0);
        switch (ls_GetLanguage()) {
        case 0:
            io_PrintText(0x1a8c, p->chr, q->amount, b);
            break;
        case 1:
            io_PrintText(0x1a8c, p->chr, b, q->amount);
            break;
        }
        e->start = ai_Time;
        e->end = ai_Time;
        gm_AddTime(&e->end, 3, 0, 0);
        snd_QueueVoice(-7, 0, "_NACHRICHTEN_HS_34", 0, 0);
        e->stage++;
        return;
    case 1:
        if (gm_CompareTime(&ai_Time, &e->end) > 0) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (d2_ClickedForm != e->slot->win || d2_ClickedId == -1)
            return;
        now = ai_Time;
        if (net_Conn.s == -1) {
            now.hour = 6;
            now.min = 0;
        }
        if (d2_ClickedId == 0) {
            e->stage = 3;
            q->answer = 1;
            p->end = now;
            cm_SendExSetHE(3, p);
            cm_SendSetField(b->id, 0x5a, 2, 0x100, 0);
            cm_SendMoveGoodsUnchecked(ai_CharsT[p->chr].netId, ai_CharsT[b->owner].netId, 0, q->amount);
            if (q->home != -1) {
                home = (AIBLD *)gm_BuildQuery(1, 1, q->home);
                home->roundSums[0] += q->amount;
            }
        } else if (d2_ClickedId == 1) {
            e->stage = 3;
            q->answer = 0;
            p->end = now;
            cm_SendExSetHE(3, p);
        }
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
    }
}

int gm_EquipmentValue(unsigned char *b, int level);
int gm_BuildingLevel();

/* 0x54b170, a Ghidra miss: the init of the protection money event: +0xb8 = -1, and (bit 4 of +0x78 clear) due now, state 1, posted 2 units ahead */
void he_StartProtection(int ev)
{
    *(int *)(ev + 0x84) = -1;
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(int *)(ev + 0xb8) = -1;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x70) = 1;
        *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
    }
}
/* 0x54b1e0, a Ghidra miss: the run handler of the protection money event 0x62 ("Schutzgeld", HEPROTECTION: the
   collector +0xac, the victim building +0xb0, the amount, the answer, the collector's home building and room).
   Freed (stage -1 / -2) it clears the building's flag 0x100 at +0x5a (a posted event only) and frees the event;
   it waits while not started or while its cm slot is busy; without the collector, the building or its owner it
   ends in two minutes. Stage 1 sends the collector to the building, stage 2 (once the collector is idle) sizes
   the amount (6.5% of the building's equipment value) and asks: a player owner gets the question event 0x63
   (he_RunProtectionAsk) and three minutes, else the answer is 2 for a building already paying (the flag 0x100),
   1 (pays) for an owner whose sel is 0xb or when a roll stays below 0.001 * its mood[11][2], else a coin toss.
   Stage 3 reports: paid, the building gets the flag 0x100, the owner pays the collector, the home building
   counts it, the news 0x1a8d, stage 4 in 48 hours; refused (0x1a8e) or else (0x1a8f), stage 4 in five minutes.
   Stage 4 shows the news 0x1a90 to both when paid and sends the collector home
   @deep Read all of it against the original and rewrote it typed (AIJOB / HEPROTECTION / AIMSG / AIBLD /
   AICHAR): fixed the meaning errors - stage 3 goes to stage 4 with cm_SendExSetHE(4) (the original's ebx =
   4, not 1), the AIMSG fields (to +8, fromId +0xc, time, flags +0x36, arg[0]; no store at +0x34), the
   owner's sel / mood[11][2] at stride 0x21c (was 0x14), the roll's direction (!(mood * 0.001f >
   util_RandomF()): test ah,5 / jnp), the refused / other branches as two full copies, text[0x200] (frame
   0x2fc equal). 580 = 580 instructions, the frame and switch table equal. Left, scheduling and allocation
   only: the owner record addressed as a byte offset kept in edx (lea edx,[eax+eax*8]; shl edx,2; mov
   al,[edx+ai_Chars+2], reused for [edx+ai_CharInts] after the memset) where ours forms the pointer (lea
   edx,[eax*4+ai_Chars]); the AIMSG stores and the ai_Time copies in another order / with ecx<->edx swapped
   (tried 8 field orders, the best kept); the cm_SendSetField / amount load order in the paid branch and
   the 0x1a8d txt_Format argument loads. The M8 at +0x20a is the aligner pairing ours' mov edx,[ai_Time+8]
   with the original's mov edx,[ai_TimeHourW] inside the same 14-byte ai_Time copy, both copying in the
   same order (registers swapped) (references: M8 at +0x20a: ours refers to ai_Time+0x8 (0x770ae8), the
   original's field to 0x770ae4 (ai_TimeHourW), explained: the ref: line is the ai_Time struct copy into
   e->end, the same four loads in the same order on both sides with ecx/edx/eax renamed; the aligner pairs
   different loads, no other object is referenced) */

void he_RunProtection(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEPROTECTION *q = (HEPROTECTION *)ev;
    AIBLD *b, *home;
    AICHAR *c, *owner;
    AIMSG msg;
    char text[0x200];

    b = (AIBLD *)gm_BuildQuery(1, 1, q->victim);
    c = (AICHAR *)gm_FindChar(q->collector);
    if (e->stage == -1 || e->stage == -2) {
        if (b != 0 && (e->opts & 2) != 0)
            cm_SendSetField(b->id, 0x5a, 2, 0, 0x100);
        he_FreeEvent((int *)ev);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    if (c == 0 || b == 0 || b->owner == 0xffff) {
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->stage = -1;
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    switch (e->stage) {
    case 1:
        sv_MoveToGroup((unsigned char *)b, 0, c->id, (char *)ev);
        cm_SendChrStop(c->netId);
        cm_SendChrGotoBuilding(c->netId, b->id, -1, 0, 1, "Schutzgeld");
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 10, 0);
        e->stage = 2;
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 2:
        if (c->scene->actions != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 4, 0);
            e->stage = 2;
            e->exSlot = cm_SendExSetHE(2, ev);
            return;
        }
        q->amount = (int)((float)gm_EquipmentValue((unsigned char *)b, 100) * 0.065f);
        owner = &ai_CharsT[b->owner];
        if (owner->kind == 6 || owner->kind == 7) {
            memset(&msg, 0, sizeof(msg));
            msg.fromId = b->id;
            msg.to = owner->netId;
            msg.time = ai_Time;
            msg.type = 0x63;
            msg.flags = 9;
            msg.arg[0] = e->id;
            cm_SendLongMessage((char *)&msg);
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 3, 0);
            e->stage = 3;
            q->answer = 2;
            e->exSlot = cm_SendExSetHE(3, ev);
            return;
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, (unsigned short)util_Random(0x10) + 2, 0);
        e->stage = 3;
        if ((b->flags5aw & 0x100) != 0)
            q->answer = 2;
        else if (ai_CharsT[b->owner].sel != 0xb && !(ai_CharsT[b->owner].mood[11][2] * 0.001f > util_RandomF()))
            q->answer = (unsigned short)util_Random(2);
        else
            q->answer = 1;
        e->exSlot = cm_SendExSetHE(3, ev);
        return;
    case 3:
        if (q->answer == 1) {
            cm_SendSetField(b->id, 0x5a, 2, 0x100, 0);
            cm_SendMoveGoodsUnchecked(ai_CharsT[e->chr].netId, ai_CharsT[b->owner].netId, 0, q->amount);
            if (q->home != -1) {
                home = (AIBLD *)gm_BuildQuery(1, 1, q->home);
                home->roundSums[0] += q->amount;
            }
            e->end = ai_Time;
            e->start = ai_Time;
            gm_AddTime(&e->end, 0x30, 0, 0);
            e->stage = 4;
            e->exSlot = cm_SendExSetHE(4, ev);
            txt_Format(text, 0x1a8d, b->owner, b, q->amount);
            he_ShowNews(ai_CharsT[e->chr].netId, ai_CharsT[e->chr].netId, text, 0, 0x5f3, "_NACHRICHTEN_HS_84");
            return;
        }
        if (q->answer == 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 5, 0);
            e->stage = 4;
            e->exSlot = cm_SendExSetHE(4, ev);
            txt_Format(text, 0x1a8e, b->owner, b);
            he_ShowNews(ai_CharsT[e->chr].netId, ai_CharsT[e->chr].netId, text, 0, 0x5f3, "_NACHRICHTEN_HS_85");
            return;
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 5, 0);
        e->stage = 4;
        e->exSlot = cm_SendExSetHE(4, ev);
        txt_Format(text, 0x1a8f, b->owner, b);
        he_ShowNews(ai_CharsT[e->chr].netId, ai_CharsT[e->chr].netId, text, 0, 0x5f3, "_NACHRICHTEN_HS_85");
        return;
    case 4:
        if (q->answer == 1) {
            txt_Format(text, 0x1a90, b);
            he_ShowNews(ai_CharsT[e->chr].netId, ai_CharsT[e->chr].netId, text, 0, 0x5f3, (char *)0);
            he_ShowNews(ai_CharsT[b->owner].netId, ai_CharsT[b->owner].netId, text, 0, 0x5f3, (char *)0);
        }
        sv_MoveToGroup((unsigned char *)b, 0, c->id, 0);
        cm_SendChrStop(c->netId);
        cm_SendChrGotoBuilding(c->netId, q->home, q->homeRoom, 0, 0, "Schutzgeld");
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->stage = -1;
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
}
/* 0x54b9b0, a Ghidra miss: the end handler of the audit event: the due clock now, posted 2 units ahead, and the victim figure (+0xb0) gets the flag 0x800 (0x4c4a60 0x5a) */

void he_StartAudit(int ev)
{
    int s = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));

    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 2, 0);
    if (s != 0)
        cm_SendSetField(*(int *)(s + 1), 0x5a, 2, 0x800, 0);
}
/* 0x54ba20, a Ghidra miss: the run handler of the audit event 0x64 ("BetriebsPr\xfc" "fung", HEAUDIT: the
   inspector +0xac checks the building +0xb0 for the character AIJOB.chr). A freed event, a gone inspector or
   building or a building without an owner frees it. Stage 0 sends the inspector to the building (stage 1 in ten
   minutes); stage 1 waits (four minutes) while the inspector is busy, then judges: the shortfall is
   (100 - the building's level) * 0.01; when it is at most 0.2 and no cart (gm_Query kind 0x1d) of the building
   is filled below 80%, nobody is fined (the news 0x1a76 to the character, 0x1a78 to the owner); else the owner
   pays the character equipment value * shortfall * 0.07 (the news 0x1a75 / 0x1a77 to players). Stage 2 sends the
   inspector home and frees the event */
void he_RunAudit(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEAUDIT *a = (HEAUDIT *)ev;
    AIBLD *b;
    AICHAR *c;
    AIOBJ *o;
    int found, fine;
    float shortfall;
    char text[0x400];

    b = (AIBLD *)gm_BuildQuery(1, 1, a->bld);
    c = (AICHAR *)gm_FindChar(a->inspector);
    if (e->stage == -2 || c == 0 || b == 0 || b->owner == 0xffff) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (e->stage) {
    case 0:
        sv_MoveToGroup((unsigned char *)b, 0, c->id, (char *)ev);
        cm_SendChrStop(c->netId);
        cm_SendChrGotoBuilding(c->netId, b->id, -1, 0, 1, "BetriebsPr\xfc" "fung");
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 10, 0);
        e->stage = 1;
        return;
    case 1:
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 4, 0);
        if (c->scene->actions != 0) {
            e->stage = 1;
            return;
        }
        shortfall = (100.0f - gm_BuildingLevel((char *)b)) * 0.01f;
        found = 0;
        for (o = (AIOBJ *)gm_Query(0, 2, 7, 4, 0x1d); o != 0; o = (AIOBJ *)gm_QueryNext()) {
            if (o->bldId == b->id && o->fill < 0x50) {
                found = 1;
                break;
            }
        }
        if (!(shortfall > 0.2) && found == 0) {
            txt_Format(text, 0x1a76, b->owner, b);
            he_ShowNews(ai_CharsT[e->chr].netId, ai_CharsT[e->chr].netId, text, found, 0x5f0, "_NACHRICHTEN_HS_88");
            txt_Format(text, 0x1a78, e->chr, b);
            he_MessageBoxQuickjump(ai_CharsT[b->owner].netId, ai_CharsT[b->owner].netId, text, found, 0x5f0, b->id, -1, found, "_NACHRICHTEN_HS_86");
            e->stage = 2;
            return;
        }
        fine = (int)(gm_EquipmentValue((unsigned char *)b, 100) * shortfall * 0.07f);
        cm_SendMoveGoodsUnchecked(ai_CharsT[e->chr].netId, ai_CharsT[b->owner].netId, 0, fine);
        if (ai_CharsT[e->chr].kind == 6 || ai_CharsT[e->chr].kind == 7) {
            txt_Format(text, 0x1a75, fine, b->owner, b);
            he_ShowNews(ai_CharsT[e->chr].netId, ai_CharsT[e->chr].netId, text, 0, 0x5f0, "_NACHRICHTEN_HS_87");
        }
        if (ai_CharsT[b->owner].kind == 6 || ai_CharsT[b->owner].kind == 7) {
            txt_Format(text, 0x1a77, e->chr, b, fine);
            he_MessageBoxQuickjump(ai_CharsT[b->owner].netId, ai_CharsT[b->owner].netId, text, 0, 0x5f0, b->id, -1, 0, "_NACHRICHTEN_HS_86");
        }
        e->stage = 2;
        return;
    case 2:
        sv_MoveToGroup((unsigned char *)b, 0, c->id, 0);
        cm_SendChrStop(c->netId);
        cm_SendChrGotoBuilding(c->netId, a->home, a->homeRoom, 0, 0, "BetriebsPr\xfc" "fung");
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x54be20, a Ghidra miss: the init of the patrol-meeting event: +0xd4 = -1, +0xdc = 0, the start (+0x44) and the due clock (+0x52) = now, posted at once (0x4fe140 (.., 0, 1, 0), state 0) */


void he_StartGathering(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(int *)(ev + 0xd4) = -1;
        *(unsigned char *)(ev + 0xdc) = 0;
        *(AITIME *)(ev + 0x44) = ai_Time;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
/* 0x54bec0, a Ghidra miss: the run handler of the patrol gathering event 0x65 ("Patrol", HEGATHERING: the figures
   ids[0..5] gather at the scene object named place). Freed (stage -2) it frees a not yet started event, else it
   sends the figures home once (stage 5) and then frees it; stage -1 frees it. Each step is due from now. Stage 0
   sends the figures there (stage 1 in five minutes); stage 1 waits (four minutes) while one is busy, then starts
   the gathering (stage 2 in ten minutes); stage 2 gives up a day after the start (stage 5), else looks for an
   event 0x48 at the same place of another character: on a coin toss the two meet in a fight party (G28REC kind 3,
   in 30 minutes: the opponent's figures defend, ours raid, stage 3), else the figures idle about the place
   (he_CharacterBehavior) for ten minutes more. Stage 3 waits for the party's slot (stage 4 with its id, stage 5
   without), stage 4 waits until the party is over, stage 5 sends the figures home and ends the event
   @deep Read all of it against the original and rewrote it typed (AIJOB / HEGATHERING / G28REC kind 3 /
   AICHAR / D3OBJECT): fixed the party record (members[0] is e's character, [1] the opponent's, thing -1,
   variant 4, the raid lists and raidHome), the opponent search (an event 0x48 at the same place of another
   character), the stage 4 test order (4 when cm_PartyById finds the party), the frame (giveUp and pos at
   function scope: 0x138 = 0x138), the switch table equal. 478 vs 484 instructions. Left: the register
   allocation of stage 2's opponent: the original keeps the found event p in a stack slot ([esp+0x10],
   shared with the free flag) besides ebp, loads p->chr into dx and e->chr into bp for the compare and
   reuses both for the party members (and ebp,0xffff / and edx,0xffff), reloading p from the slot for the
   defenders loop; ours keeps p in ebp and reloads p->chr. With p spilled, VC6 lays the party block right
   after the loop and the not-found block (ten minutes more, he_CharacterBehavior) behind it (the loop exit
   jumps forward: test eax,eax; mov [esp+0x10],eax; je), ours lays the not-found block after the loop: the
   43-line hunk at +0x298 and the shifted 33 lines at +0x487 are those blocks, the same code. Tried: a
   while loop with the condition, the free flag and p as one variable, the compare operands swapped, 3
   orders of the party's time / raid stores (references: M8 at +0x3f7: ours refers to ai_Time (0x770ae0),
   the original's field to 0x770ae4 (ai_TimeHourW) (and 1 more), explained: the two ref: lines are the
   ai_Time copy into party.time, the same four loads on both sides with eax/ecx/edx renamed and interleaved
   with raidHome / raid100 stores; no other object is referenced) */
void he_RunGathering(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEGATHERING *g = (HEGATHERING *)ev;
    AIJOB *p;
    AICHAR *c;
    D3OBJECT *o;
    int *id;
    int i, k, n, freeIt;
    int *r;
    G28REC party;
    AITIME giveUp;
    D3VECTOR4 pos;

    freeIt = 0;
    if (e->stage == -2) {
        if ((e->opts & 4) != 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        e->stage = 5;
        freeIt = 1;
    }
    if (e->stage == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    e->end = ai_Time;
    switch (e->stage) {
    case 0:
        for (id = e->ids, k = 6; k != 0; k--, id++) {
            c = (AICHAR *)gm_FindChar(*id);
            if (c != 0 && c->scene != 0) {
                sv_MoveToGroup(0, 0, c->id, (char *)ev);
                cm_SendChrStop(c->netId);
                cm_SendChrGotoBuilding(c->netId, -1, -1, g->place, 1, "Patrol");
            }
        }
        gm_AddTime(&e->end, 0, 5, 0);
        e->exSlot = cm_SendExSetHE(1, ev);
        return;
    case 1:
        for (i = 0; i < 6; i++) {
            c = (AICHAR *)gm_FindChar(e->ids[i]);
            if (c != 0 && c->scene != 0 && c->scene->actions != 0) {
                gm_AddTime(&e->end, 0, 4, 0);
                e->exSlot = cm_SendExSetHE(1, ev);
                return;
            }
        }
        e->start = ai_Time;
        gm_AddTime(&e->end, 0, 10, 0);
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 2:
        giveUp = e->start;
        gm_AddTime(&giveUp, 0x18, 0, 0);
        if (gm_CompareTime(&ai_Time, &giveUp) > 0) {
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(5, ev);
            return;
        }
        p = (AIJOB *)he_FindEvent(1, 0, 0x48);
        while (p != 0 && (strcmp(g->place, ((HEGATHERING *)p)->place) != 0 || e->chr == p->chr))
            p = (AIJOB *)he_NextEvent();
        if (p != 0 && (short)util_Random(2) != 0 && e->chr != p->chr) {
            memset(&party, 0, sizeof(party));
            party.kind = 3;
            party.flags = 0xa2;
            party.thing = -1;
            party.used = 2;
            party.variant = 4;
            party.host = ai_CharInts[ai_PlayerId][0];
            party.members[0] = ai_CharInts[e->chr][0];
            party.members[1] = ai_CharInts[p->chr][0];
            for (i = 0; i < 16; i++) {
                party.raid.raiders[i] = -1;
                party.raid.defenders[i] = -1;
            }
            for (n = 0, i = 0; i < 8 && n < 6; i++) {
                if (p->ids[i] != -1)
                    party.raid.defenders[n++] = p->ids[i];
            }
            for (n = 0, i = 0; i < 6 && n < 6; i++) {
                if (e->ids[i] != -1)
                    party.raid.raiders[n++] = e->ids[i];
            }
            party.raid.raid100[0] = -1;
            party.raid.raid100[1] = -1;
            party.raid.raid100[2] = -1;
            party.raid.raidHome = g->home;
            party.time = ai_Time;
            gm_AddTime(&party.time, 0, 0x1e, 0);
            g->partySlot = cm_SendNewParty(&party);
            e->stage = 3;
            gm_AddTime(&e->end, 0, 10, 0);
            e->exSlot = cm_SendExSetHE(3, ev);
            return;
        }
        gm_AddTime(&e->end, 0, 10, 0);
        if (d3_CurrentScene == 0 && (o = d3_FindObject(0, 0x140, 0, g->place)) != 0) {
            d3_PointToWorld(o, &o->pos, (D3VECTOR *)&pos);
            he_CharacterBehavior(e, (float *)&pos);
        }
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 3:
        gm_AddTime(&e->end, 0, 1, 0);
        if (cm_GetSlotState(g->partySlot) == 0) {
            e->exSlot = cm_SendExSetHE(3, ev);
            return;
        }
        r = (int *)cm_GetSlotValue(g->partySlot);
        if (r == 0) {
            e->exSlot = cm_SendExSetHE(5, ev);
            return;
        }
        g->party = *r;
        e->exSlot = cm_SendExSetHE(4, ev);
        return;
    case 4:
        gm_AddTime(&e->end, 0, 5, 0);
        if (cm_PartyById(g->party) != 0) {
            e->exSlot = cm_SendExSetHE(4, ev);
            return;
        }
        e->exSlot = cm_SendExSetHE(5, ev);
        return;
    case 5:
        for (id = e->ids, k = 6; k != 0; k--, id++) {
            c = (AICHAR *)gm_FindChar(*id);
            if (c != 0 && c->scene != 0) {
                sv_MoveToGroup(0, 0, c->id, 0);
                cm_SendChrStop(c->netId);
                cm_SendChrGotoBuilding(c->netId, g->home, g->homeRoom, 0, 0, "Patrol");
            }
        }
        gm_AddTime(&e->end, 0, 2, 0);
        if (freeIt) {
            he_FreeEvent((int *)ev);
            return;
        }
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
}
/* 0x54c4c0, a Ghidra miss: the init of the event: the due clock now with +0x56 = 0xa, +0x58 / +0x5c / +0xac / +0xb0 / +0xb4 = 0 */

void he_Unk54c4c0(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(short *)(ev + 0x56) = 0xa;
    *(int *)(ev + 0x58) = 0;
    *(int *)(ev + 0x5c) = 0;
    *(int *)(ev + 0xac) = 0;
    *(int *)(ev + 0xb0) = 0;
    *(int *)(ev + 0xb4) = 0;
}

int ai_IsLocalWorker();
float gm_AddHealth(unsigned short id, int d);

/* 0x54c510, a Ghidra miss: the test of a figure record for the accident simulation: alive (word 0 != 0xffff, +8 != 0), kind 0, with a boss (+0x16c) whose id (+1) is the one of its job record (+0x184 + 0x2c): then 0x47e840 (rec), else 0 */
int he_IsAtWork(int rec)
{
    int ok = 1;

    if (*(unsigned short *)rec == 0xffff)
        ok = 0;
    if (*(unsigned char *)(rec + 8) == 0)
        ok = 0;
    if (*(unsigned char *)(rec + 2) == 0 && ok && *(int *)(rec + 0x16c) != 0 && *(int *)(rec + 0x184) != 0
        && *(int *)(*(int *)(rec + 0x184) + 0x2c) == *(int *)(*(int *)(rec + 0x16c) + 1))
        return ai_IsLocalWorker(rec);
    return 0;
}
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* 0x54c570, a Ghidra miss: the run handler of the work accident simulation, event 0x66 (HESIM; the text
   "RunSimUnfall: calculating at <Day ..>" is formatted and never shown). Freed (stage -2) it frees the event.
   A round starts with a random stride of 16 numbers prime to 0x300 and a random first character; each run (stage
   0) visits the next 12 characters of the round: a worker at its building (he_IsAtWork) whose building has an
   owner has an accident (he_CharAccident) when a roll of 0 .. 100 stays below 25 - 20 * the building's skill 2
   * 0.0125: it loses 50 + random(1 + 255 * (2 - skill * 0.0125), clamped to 0 .. 2) health, and the accident
   gets the health gm_AddHealth leaves. After all 0x300 the round is over: due the next day at 10:00, else due in
   30 minutes, the next day at 10:00 once that is 18:00 or later */
void he_RunAccidentSim(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HESIM *s = (HESIM *)ev;
    AIBLD *b;
    int n, cur, skill;
    float roll, lim, health;
    int primes[16];
    char text[0x200];

    if (e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    txt_Format(text, (int)"-------> RunSimUnfall: calculating at <Day = %i Time = %i.2:%i.2>", ai_Time.year, ai_Time.hour, ai_Time.min);
    if (s->step == 0) {
        primes[0] = 1;
        primes[1] = 3;
        primes[2] = 5;
        primes[3] = 7;
        primes[4] = 0xb;
        primes[5] = 0xd;
        primes[6] = 0x11;
        primes[7] = 0x13;
        primes[8] = 0x2ed;
        primes[9] = 0x2ef;
        primes[10] = 0x2f3;
        primes[11] = 0x2f5;
        primes[12] = 0x2f9;
        primes[13] = 0x2fb;
        primes[14] = 0x2fd;
        primes[15] = 0x2ff;
        s->step = primes[(unsigned short)util_Random(0x10)];
        s->cursor = (unsigned short)util_Random(0x300);
    }
    if (e->stage != 0)
        return;
    cur = s->cursor;
    for (n = 12; n != 0; n--) {
        if (he_IsAtWork((int)&ai_CharsT[cur]) != 0 && ai_CharsT[cur].bld->owner != 0xffff) {
            roll = util_RandomF() * 100.0f;
            skill = gm_BuildingSkill((unsigned char *)ai_CharsT[cur].bld, 2, 1);
            if (25.0f - 20.0f * (skill * 0.0125f) > roll) {
                lim = MIN(MAX(2.0f - skill * 0.0125f, 0.0f), 2.0f);
                health = gm_AddHealth(cur, -50 - (unsigned short)util_Random((int)(lim * 255.0f + ai_FloatOne)));
                b = ai_CharsT[cur].bld;
                he_CharAccident(e, &ai_CharsT[cur], &ai_CharsT[b->owner], b, health, 0);
            }
        }
        cur = (s->step + cur) % 0x300;
        s->done++;
    }
    s->cursor = cur;
    if (s->done >= 0x300) {
        s->cursor = 0;
        s->step = 0;
        s->done = 0;
        e->end.year++;
        e->end.min = 0;
        e->end.hour = 0xa;
        return;
    }
    gm_AddTime(&e->end, 0, 0x1e, 0);
    if (e->end.hour >= 0x12) {
        e->end.min = 0;
        e->end.hour = 0xa;
        e->end.year++;
    }
}
/* 0x54c870, a Ghidra miss: the init of the ranking event: the due clock now, +0x56 = 9, +0x58 = 0, posted 0x18 units ahead (state 0) */

void he_StartAuctionEvent(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 9;
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
/* 0x54c8e0, a Ghidra miss: the run handler of the auction event 0x67 (HEAUCTION: the building +0xac, the start
   price +0xb0). Stage -1 frees it; a not yet started event waits. Stage 0 announces the auction (the news 0x11be
   with the building and the price, 0x601, to every living player character; no building: the event ends) and
   is due the next day at 13:30 (stage 1). Stage 1 invites as bidders up to 8 living player characters (kind 6 /
   7, not held, worth at least the price), then the 8 worthiest such characters of the kinds 3 .. 5, and posts the
   auction party (G28REC kind 10, cm_RunAuction) for 30 minutes from now when there is one; the event ends
   @deep Read all of it against the original and rewrote it typed (AIJOB / HEAUCTION / AICHAR / G28REC): it
   is the auction event 0x67, not a ranking: fixed the party (kind 10, flags 0x22, thing -1, used n, the
   bidders in members[] directly, auction.building / price, the 0 at +0x84 as auction.zero84), the index
   loops over ai_CharsT (signed jl), the insertion with the original's jumps (an empty slot goes straight
   to the insert, a better value shifts first, an exhausted search continues: goto insert / goto shift),
   stage 0's next day 13:30. 246 vs 248 instructions, the control flow the same. Left, allocation and
   scheduling: the original keeps the bidder count n in ebx with a stack home ([esp+0x10], frame 0x358 vs
   ours 0x354) and uses ebx in the shift (mov ebx,[esp+0x10] after it), ours uses ebp and reloads ev from
   the argument (mov ebp,[esp+0x368]); and the party stores at +0x1a3..+0x210 in another order (the
   original: price, time.hour, kind, flags, thing, used, host, building, time.year, zero84, time+8,
   time+0xc; tried 5 source orders, the best kept) (references: M8 at +0x1e6: ours refers to ai_Time
   (0x770ae0), the original's field to 0xac (and 1 more), explained: the ref: lines are the ai_Time copy
   into party.time and the e+0xac load, paired across the reordered party stores: the same loads and stores
   on both sides in another order, no other object) */
void he_RunAuctionEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEAUCTION *a = (HEAUCTION *)ev;
    AIBLD *b;
    int n, i, j, k, v;
    int best[8];
    int val[8];
    G28REC party;
    char text[0x200];

    if (e->stage == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    switch (e->stage) {
    case 0:
        b = (AIBLD *)gm_BuildQuery(1, 1, a->bld);
        if (b == 0) {
            cm_SendExSetHE(-1, ev);
            return;
        }
        txt_Format(text, 0x11be, b, a->price);
        for (k = 0; k < 0x300; k++) {
            if ((ai_CharsT[k].kind == 6 || ai_CharsT[k].kind == 7) && ai_CharsT[k].health > 0)
                he_ShowNews(ai_CharsT[k].netId, ai_CharsT[k].netId, text, 0, 0x601, (char *)0);
        }
        e->end.year++;
        e->end.hour = 0xd;
        e->end.min = 0x1e;
        e->exSlot = cm_SendExSetHE(1, ev);
        return;
    case 1:
        memset(best, -1, sizeof(best));
        memset(val, 0, sizeof(val));
        n = 0;
        memset(&party, 0, sizeof(party));
        for (k = 0; k < 0x300 && n < 8; k++) {
            if ((ai_CharsT[k].kind == 6 || ai_CharsT[k].kind == 7) && ai_CharsT[k].health > 0 && ai_CharsT[k].heldRounds == 0
                && gm_GetGoodsValue(&ai_CharsT[k]) >= a->price)
                party.members[n++] = ai_CharsT[k].netId;
        }
        for (k = 0; k < 0x300; k++) {
            if (ai_CharsT[k].health > 0 && ai_CharsT[k].heldRounds == 0 && (ai_CharsT[k].kind == 5 || ai_CharsT[k].kind == 3 || ai_CharsT[k].kind == 4)) {
                v = gm_GetGoodsValue(&ai_CharsT[k]);
                if (v < a->price)
                    continue;
                for (i = 0; i < 8; i++) {
                    if (best[i] == -1)
                        goto insert;
                    if (v > val[i])
                        goto shift;
                }
                continue;
            shift:
                for (j = 7; j > i; j--) {
                    best[j] = best[j - 1];
                    val[j] = val[j - 1];
                }
            insert:
                val[i] = v;
                best[i] = ai_CharsT[k].netId;
            }
        }
        for (i = 0; i < 8 && n < 8; i++) {
            if (best[i] != -1)
                party.members[n++] = best[i];
        }
        if (n != 0) {
            party.auction.price = a->price;
            party.kind = 0xa;
            party.flags = 0x22;
            party.thing = -1;
            party.used = n;
            party.host = ai_CharInts[ai_PlayerId][0];
            party.auction.building = a->bld;
            party.auction.zero84 = 0;
            party.time = ai_Time;
            gm_AddTime(&party.time, 0, 0x1e, 0);
            cm_SendNewParty(&party);
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
}
/* 0x54cc20, a Ghidra miss: the init: the due clock = the start clock (+0x44), state 0, +0xb4 = 0 */

void he_StartTitleCeremony(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0xb4) = 0;
}
/* 0x54cc50, a Ghidra miss: the run handler of the title ceremony event 0x69 (HETITLE: the character +0xb0 gets the
   title +0xac), run only on the game of its player (AIJOB.chr is ai_PlayerId) in a running game once it is due.
   Freed, ended, without its player or its character it frees the event. Stage 0 opens the window
   ("SPECIAL\TITEL" when the character is the player's own, "SPECIAL\TITEL_ANDERE" for another one): for the own
   character it sends the title once (field 0xd) and shows the news 0x42a3 / 0x42a1 / 0x429f (titles 9 / 8 / 7),
   0x1618 (the citizenship, 2) or 0x1617 (with the player's name, the end clock, the character and the title's
   text by sex) with the fanfare by the title's height and the herald's voice, then the click sound; for another
   character the news 0x42a4 / 0x42a2 / 0x42a0, 0x161b or 0x161a with the rival's voice; then the news 0x161c below
   (stage 1). Stage 1 frees the event when the window is answered (button 0, or the event bar's confirmation of
   its slot); without a window it starts over
   @deep Read all of it against the original and rewrote it typed (AIJOB / HETITLE / AICHAR /
   GMPLAYERSLOT): reconstructed the missing statements (the voices: the fanfare by the title's height Kurz
   / Mittel / Lang, _VERLEIHUNG_TITEL / _BUERGERRECHT, _TITEL_MAENNLICH / WEIBLICH with the title, the
   _ERHALTEN_HS_02..04 heralds, the click sound; the rival's _KONKURENT / _FB_HS_02..04 voices), fixed the
   news arguments (&e->end, not ev; the player's name &ai_PlayerSlotsT[ai_OwnPlayer].name[0x20]), the
   per-sex title text as two sums (sex == 0 ? title + 0x114 : title + 0x11e). 448 = 448 instructions, the
   same blocks in the same order. Left, allocation and scheduling only: the saved registers pushed ebx,
   ebp, esi, edi in the original (ours ebx, ebp, edi, esi; ev in edi, the character in esi on both sides),
   ecx / edx / eax swapped through the news and voice argument loads, and in the 0x1618 news (+0x237) the
   original computes the sex term in eax and loads ai_OwnPlayer before the character's id (ours the id
   first, the term in edx); tried the locals' declaration order */
void he_RunTitleCeremony(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HETITLE *t = (HETITLE *)ev;
    AICHAR *own, *c;

    own = &ai_CharsT[e->chr];
    if (ai_PlayerId != e->chr || ai_GameState == 0)
        return;
    if (gm_CompareTime(&e->end, &ai_Time) == 1)
        return;
    c = (AICHAR *)gm_FindChar(t->ch);
    if (e->stage == -2 || e->stage == -1 || own->id == 0xffff || c == 0) {
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    switch (e->stage) {
    case 0:
        if (e->slot == 0) {
            if (c == own)
                he_OpenEventSlot(ev, 0, 0, (int)"SPECIAL\\TITEL", 0x5fd);
            else
                he_OpenEventSlot(ev, 0, 0, (int)"SPECIAL\\TITEL_ANDERE", 0x5fe);
        }
        if (e->slot == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(e->slot->win);
        d2_SetForm(e->slot->win, 1);
        if (c == own) {
            if (t->sent == 0) {
                cm_BeginFieldBlock((char *)c, c->netId);
                cm_AddFieldDelta(1, 1, 0xd, &t->title);
                cm_SendFieldDeltas();
                t->sent = 1;
            }
            if (t->title == 9)
                io_PrintText(0x42a3, &e->end);
            else if (t->title == 8)
                io_PrintText(0x42a1, &e->end);
            else if (t->title == 7)
                io_PrintText(0x429f, &e->end);
            else if (t->title != 2)
                io_PrintText(0x1617, &ai_PlayerSlotsT[ai_OwnPlayer].name[0x20], &e->end, c->id, (c->sex == 0 ? t->title + 0x114 : t->title + 0x11e));
            else
                io_PrintText(0x1618, &ai_PlayerSlotsT[ai_OwnPlayer].name[0x20], &e->end, c->id, (c->sex != 0 ? 10 : 0) + 0x116);
            if (t->title == 2) {
                snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftKurz", 0, 100);
                snd_QueueVoice(-8, 0, "_VERLEIHUNG_BUERGERRECHT", 0, 0xfa);
                snd_QueueVoice(-8, 0, c->sex == 0 ? "_TITEL_MAENNLICH" : "_TITEL_WEIBLICH", t->title, 100);
                snd_QueueVoice(-7, 0, "MausclickLinks_s_01", -1, 0);
            } else {
                if (t->title <= 3)
                    snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftKurz", 0, 100);
                else if (t->title < 6)
                    snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftMittel", 0, 100);
                else
                    snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftLang", 0, 100);
                if (t->title < 7) {
                    snd_QueueVoice(-8, 0, "_VERLEIHUNG_TITEL", 0, 0xfa);
                    snd_QueueVoice(-8, 0, c->sex == 0 ? "_TITEL_MAENNLICH" : "_TITEL_WEIBLICH", t->title, 100);
                } else if (t->title == 7)
                    snd_QueueVoice(-7, 0, "_VERLEIHUNG_TITEL_ERHALTEN_HS_02", -1, 100);
                else if (t->title == 8)
                    snd_QueueVoice(-7, 0, "_VERLEIHUNG_TITEL_ERHALTEN_HS_03", -1, 100);
                else if (t->title == 9)
                    snd_QueueVoice(-7, 0, "_VERLEIHUNG_TITEL_ERHALTEN_HS_04", -1, 100);
                snd_QueueVoice(-7, 0, "MausclickLinks_s_01", -1, 0);
            }
        } else {
            if (t->title == 9)
                io_PrintText(0x42a4, c->id, (c->sex != 0 ? 10 : 0) + 0x11d, c->id);
            else if (t->title == 8)
                io_PrintText(0x42a2, c->id, (c->sex != 0 ? 10 : 0) + 0x11c, c->id);
            else if (t->title == 7)
                io_PrintText(0x42a0, c->id, (c->sex != 0 ? 10 : 0) + 0x11b, c->id);
            else if (t->title != 2)
                io_PrintText(0x161a, c->id, c->id, (c->sex == 0 ? t->title + 0x114 : t->title + 0x11e), c->id);
            else
                io_PrintText(0x161b, c->id);
            if (t->title == 2)
                snd_QueueVoice(-8, 0, "_VERLEIHUNG_BUERGERRECHT_KONKURENT", 0, 100);
            else if (t->title < 7) {
                snd_QueueVoice(-8, 0, "_VERLEIHUNG_TITEL_KONKURENT", 0, 100);
                snd_QueueVoice(-8, 0, c->sex == 0 ? "_TITEL_MAENNLICH" : "_TITEL_WEIBLICH", t->title, 100);
            } else if (t->title == 7)
                snd_QueueVoice(-7, 0, "_VERLEIHUNG_TITEL_FB_HS_02", -1, 100);
            else if (t->title == 8)
                snd_QueueVoice(-7, 0, "_VERLEIHUNG_TITEL_FB_HS_03", -1, 100);
            else if (t->title == 9)
                snd_QueueVoice(-7, 0, "_VERLEIHUNG_TITEL_FB_HS_04", -1, 100);
        }
        d2_SetForm(e->slot->win, 2);
        io_PrintText(0x161c);
        e->stage = 1;
        return;
    case 1:
        if (e->slot == 0) {
            e->stage = 0;
            return;
        }
        if (d2_ClickedForm != e->slot->win && he_SlotConfirmed == 0)
            return;
        if (d2_ClickedId == 0) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (he_SlotConfirmed == 0 || he_ActiveSlot != e->slot)
            return;
        he_SlotConfirmed = 0;
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x54d190, a Ghidra miss: the init of the title event: the due clock now, +0x56 = 0x16, +0x58 = 0x1e, +0x5c / +0xac / +0xb0 = 0 */

void he_StartInnSim(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(short *)(ev + 0x56) = 0x16;
    *(int *)(ev + 0x58) = 0x1e;
    *(int *)(ev + 0x5c) = 0;
    *(int *)(ev + 0xac) = 0;
    *(int *)(ev + 0xb0) = 0;
}

int ai_CanAct();
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);

/* 0x54d1e0, a Ghidra miss: looks the figure of a record up in a table of 16 entries of 12 bytes (first word: a building of 0x501190): finds the entry whose building id (+1) is the figure's job id (+0x184 + 0x2c) and that has an owner (word +0x27 != 0xffff); stores its index in *out and returns 1, else 0 */
int he_FindGuestInn(int rec, int *tab, int *out)
{
    int i;
    int *e;

    if (*(unsigned short *)rec == 0xffff || *(unsigned char *)(rec + 8) == 0 || *(unsigned char *)(rec + 2) != 0)
        return 0;
    if (*(int *)(rec + 0x184) == 0 || ai_CanAct(rec) == 0)
        return 0;
    e = tab;
    for (i = 0; i < 16; i++) {
        if (*e == 0)
            return 0;
        if (*(int *)(*e + 1) == *(int *)(*(int *)(rec + 0x184) + 0x2c) && *(unsigned short *)(*e + 0x27) != 0xffff) {
            *out = i;
            return 1;
        }
        e += 3;
    }
    return 0;
}
typedef struct HEINN {                  /* he_RunInnSim's list of the inns (he_FindGuestInn walks it, 12 bytes each) */
    AIBLD *inn;                         /* 0x0, the inn (a building of kind 0x16), 0: the end of the list */
    AIOBJ *drink1;                      /* 0x4, its bar's good 0x179, 0: none */
    AIOBJ *drink2;                      /* 0x8, its bar's good 0x17a, 0: none */
} HEINN;

/* he_RunInnSim's guest count: the inn's field +0x69 counts `guests` more */
#define HE_INN_GUEST(r) {                                                                       \
    cm_BeginFieldBlock((char *)(r).inn, (r).inn->id);                                           \
    cm_AddFieldValues(2, 1, (char *)(r).inn - cm_FieldObject + 0x69, &guests);                  \
    cm_SendFieldDeltas();                                                                       \
}
/* he_RunInnSim's sale of a drink to the guest ai_CharsT[i] (cm_SendExSellObject: one of the good): 0.65 of the
   price from the guest, the price to the inn's owner, the takings (+0x65) and the guests (+0x6b) of the inn, then the guest count */
#define HE_INN_SALE(r, drink, good, price) {                                                    \
    cm_BeginGroup("SIM_WIRTSHAUS");                                                             \
    cm_SendExSellObject(-1, (r).drink->ownerId, good, 1, ai_OwnPlayer, 0);                     \
    cm_SendMoveGoodsUnchecked(-1, ai_CharsT[i].netId, ai_OwnPlayer, (int)((float)price * 0.65f)); \
    cm_SendMoveGoodsUnchecked(ai_CharsT[(r).inn->owner].netId, -1, ai_OwnPlayer, price);        \
    cm_BeginFieldBlock((char *)(r).inn, (r).inn->id);                                           \
    cm_AddFieldValues(4, 1, (char *)(r).inn - cm_FieldObject + 0x65, &price);                   \
    cm_AddFieldValues(2, 1, (char *)(r).inn - cm_FieldObject + 0x6b, &guests);                  \
    cm_SendFieldDeltas();                                                                       \
    cm_EndGroup("SIM_WIRTSHAUS");                                                               \
    HE_INN_GUEST(r);                                                                            \
}
/* 0x54d260, a Ghidra miss: the run handler of the inn simulation, event 0x68 ("SIM_WIRTSHAUS"; HESIM's cursor).
   Freed (stage -2) it frees the event. It lists up to 16 inns (buildings of kind 0x16) with their bar object (kind
   0x2a, 6) and its two drinks (the goods 0x179 / 0x17a, 0: none); each run (stage 0) visits the next 0x180
   characters: one sitting in an inn (he_FindGuestInn) whose wealth (gm_GetGoodsValue) is above 0.65 * the dearer
   drink (the prices 1.5 * gm_GoodsPrice) buys either drink at random when the inn has both, else the first one it
   can afford, else the second: the inn sells one (cm_SendExSellObject), the guest pays 0.65 of the price, the
   inn's owner gets the price, and the inn's fields +0x65 (the takings) / +0x6b (3 .. 6 guests) count it; a guest
   counts at +0x69 too, so does one who could afford the first drink of an inn without the second. After the last
   character the round is over (due the next day at 22:30), else due in 15 minutes */
void he_RunInnSim(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HESIM *s = (HESIM *)ev;
    HEINN inns[16];
    int i, n, end, idx, wealth, price1, price2, lim, guests;

    if (e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    D3_ZERO(inns, sizeof(inns));
    n = 0;
    inns[0].inn = (AIBLD *)gm_BuildQuery(1, 5, 0x16);
    while (inns[n].inn != 0 && n < 16) {
        AIOBJ *bar = (AIOBJ *)gm_Query((void *)inns[n].inn->objList, 2, 0, 0x2a, 6);

        if (bar != 0) {
            inns[n].drink1 = (AIOBJ *)gm_Query((void *)bar->list, 1, 0, 0x179);
            inns[n].drink2 = (AIOBJ *)gm_Query((void *)bar->list, 1, 0, 0x17a);
        }
        n++;
        inns[n].inn = (AIBLD *)gm_BuildNext();
    }
    if (e->stage != 0)
        return;
    end = s->cursor + 0x180;
    for (i = s->cursor; i < end; i++) {
        if (he_FindGuestInn((int)&ai_CharsT[i], (int *)inns, &idx) == 0)
            continue;
        price1 = (int)(gm_GoodsPrice(0x179, ai_OwnPlayer) * 1.5);
        price2 = (int)(gm_GoodsPrice(0x17a, ai_OwnPlayer) * 1.5);
        guests = util_Random(4) + 3;
        wealth = gm_GetGoodsValue(&ai_CharsT[i]);
        lim = price1 > price2 ? price1 : price2;
        if ((float)wealth > (float)lim * 0.65f && inns[idx].drink1 != 0 && inns[idx].drink2 != 0) {
            if (util_RandomU(2) != 0) {
                HE_INN_SALE(inns[idx], drink1, 0x179, price1);
            } else {
                HE_INN_SALE(inns[idx], drink2, 0x17a, price2);
            }
        } else if (wealth > price1 && inns[idx].drink1 != 0) {
            HE_INN_SALE(inns[idx], drink1, 0x179, price1);
        } else if (wealth > price2) {
            if (inns[idx].drink2 != 0) {
                HE_INN_SALE(inns[idx], drink2, 0x17a, price2);
            } else if (wealth > price1) {
                HE_INN_GUEST(inns[idx]);
            }
        }
    }
    s->cursor = end;
    if (end >= 0x300) {
        s->cursor = 0;
        s->step = 0;
        e->end.year++;
        e->end.hour = 0x16;
        e->end.min = 0x1e;
        return;
    }
    gm_AddTime(&e->end, 0, 0xf, 0);
}

unsigned short hmt_RecordCount(void);
unsigned short hmt_RandomRecord(short k);
unsigned short hmt_RandomTextOffset(unsigned short k);
struct G11REC *hmt_GetRecord(unsigned short k);
void d2_CentreViewWindows(int view);

/* 0x54d790, a Ghidra miss: the init of the level-up event: the due clock = the start clock (+0x44), +0xb4 = random(6) */
void he_StartSkillUp(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(int *)(ev + 0xb4) = (unsigned short)util_Random(6);
}
/* 0x54d7d0, the run handler of the skill level-up event 0x6a of the own player ("SPECIAL\TALENT_STUFE"): shows the
   news 0x16c8, one of six texts of the skill (0x16c9 + skill * 6 + variant) with the fanfare and its voice
   ("_TALENT_MAIN"), the level's news and voice ("_TALENT_STUFE") or below level 6 the skill's master news
   ("_TALENT_MAX"), then 0x16f1; stage 1 waits for the click and frees the event */

void he_RunSkillUp(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HESKILLUP *s = (HESKILLUP *)ev;
    AICHAR *own;

    own = &ai_CharsT[e->chr];
    if (ai_PlayerId != e->chr || ai_GameState == 0)
        return;
    if (gm_CompareTime(&e->end, &ai_Time) == 1)
        return;
    if (e->stage == -2 || e->stage == -1 || own->id == 0xffff) {
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    switch (e->stage) {
    case 0:
        if (e->slot == 0)
            he_OpenEventSlot(ev, 0, 0, (int)"SPECIAL\\TALENT_STUFE", 0x5ec);
        if (e->slot == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(e->slot->win);
        d2_SetForm(e->slot->win, 1);
        io_PrintText(0x16c8);
        io_PrintText(s->variant + s->skill * 6 + 0x16c9);
        snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftKurz", 0, 100);
        snd_QueueVoice(-8, 0, "_TALENT_MAIN", s->variant + s->skill * 6, 0xfa);
        if (s->level < 6) {
            io_PrintText(s->level + 0x16e6);
            snd_QueueVoice(-8, 0, "_TALENT_STUFE", s->level - 1, 100);
        } else {
            io_PrintText(s->skill + 0x16ec);
            snd_QueueVoice(-8, 0, "_TALENT_MAX", s->skill, 100);
        }
        snd_QueueVoice(-7, 0, "MausclickLinks_s_01", -1, 0);
        d2_SetForm(e->slot->win, 2);
        io_PrintText(0x16f1);
        e->stage = 1;
        return;
    case 1:
        if (e->slot == 0) {
            e->stage = 0;
            return;
        }
        if (d2_ClickedForm != e->slot->win && he_SlotConfirmed == 0 && d2_ClickedId == -1)
            return;
        if (d2_ClickedId == 0) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (he_SlotConfirmed == 0 || he_ActiveSlot != e->slot)
            return;
        he_SlotConfirmed = 0;
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x54da20, a Ghidra miss: the init of the talent event: +0xac = the number of the talent (0x4ac140), +0xb4 = -1, the due clock now, posted (state 0) */

void he_StartBard(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(unsigned short *)(ev + 0xac) = (unsigned short)hmt_RecordCount();
        *(int *)(ev + 0xb4) = -1;
        *(AITIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
/* 0x54da80, the run handler of the bard event: stage 0 draws the song's subject (an hmt record and its text) and
   sets the due clock 3 + random(4) ahead at 10:00; stage 1 shows the news 0x1e29 to every player figure (kinds 6 /
   7); stage 2 creates the bard (SpecialEvents\barde\barde_create.esc, marked 'DrBh', run with the record's text
   set) and keeps it until 20:00, then stops it; it frees the gfx info once the own player has the flag 0x20000;
   stage 3 clears the flag 0x20000 of everybody and ends the event a day later at 6:00 */

void he_RunBard(int ev)
{
    char text[0x200];
    AIJOB *e = (AIJOB *)ev;
    HEBARD *b = (HEBARD *)ev;
    int i;
    EVTSCRIPT *sc;
    struct G11REC *rec;
    int scene;

    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    switch (e->stage) {
    case 0:
        if ((e->opts & 4) != 0)
            return;
        b->rec = hmt_RandomRecord(b->rec);
        b->textOff = hmt_RandomTextOffset(b->rec);
        e->end = ai_Time;
        e->end.year += (unsigned short)util_Random(4) + 3;
        e->end.hour = 0xa;
        e->end.min = 0;
        cm_SendExSetHE(1, ev);
        e->exSlot = -1;
        return;
    case 1:
        if ((e->opts & 4) != 0)
            return;
        txt_Format(text, 0x1e29);
        for (i = 0; i < 768; i++) {
            if (ai_CharsT[i].id != 0xffff && (ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7))
                he_ShowNews(ai_CharsT[i].netId, e->id, text, 0, 0x601, ch_EmptyName);
        }
        gm_AddTime(&e->end, 0, 5, 0);
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 2:
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 5, 0);
        sc = evt_FindById(b->script);
        if (e->end.hour < 0x14) {
            if (sc == 0 || ((HEBARDSCRIPT *)sc)->mark != HE_BARD_MARK) {
                b->script = -1;
                rec = hmt_GetRecord(b->rec);
                sprintf(text, "%i", rec->textSet);
                scene = d3_CurrentScene;
                d3if_SelectScene(0, 1);
                sc = evt_Load("SpecialEvents\\barde\\barde_create.esc");
                if (sc != 0) {
                    ((HEBARDSCRIPT *)sc)->mark = HE_BARD_MARK;
                    b->script = sc->id;
                    evt_RunScriptArgs(sc, 1, rec->textSet);
                    sc->group = 0;
                }
                d3if_SelectScene(scene, 1);
            }
            e->stage = 2;
            if (e->gfx != 0 && (ai_CharsT[ai_PlayerId].flags1c8 & 0x20000) != 0) {
                he_FreeGfxInfo((int *)e->gfx);
                e->gfx = 0;
            }
            return;
        }
        if (sc != 0 && ((HEBARDSCRIPT *)sc)->mark == HE_BARD_MARK)
            evt_Stop(sc);
        gm_AddTime(&e->end, 0, 5, 0);
        e->stage = 3;
        return;
    case 3:
        if ((e->opts & 2) != 0) {
            for (i = 0; i < 768; i++) {
                if (ai_CharsT[i].id != 0xffff && (ai_CharsT[i].flags1c8 & 0x20000) != 0)
                    cm_SendSetField(ai_CharsT[i].netId, 0x1c8, 4, 0, 0x20000);
            }
        }
        e->end = ai_Time;
        e->end.year++;
        e->end.hour = 6;
        e->end.min = 0;
        if (e->gfx != 0) {
            he_FreeGfxInfo((int *)e->gfx);
            e->gfx = 0;
        }
        if ((e->opts & 2) != 0)
            e->exSlot = cm_SendExSetHE(0, ev);
        e->exSlot = -1;
        return;
    }
}

float ord_GetStatistics(int *o);
int ord_GetKindRecord(int i, int *out);
void cm_SendBoss(int a, int b);
float gm_AddHealth(unsigned short id, int d);

/* 0x54de60, a Ghidra miss: the init of the brawl event: the due clock now, +0xac = +0xb0 = 0, posted (state 0) */
void he_StartPopulation(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0xac) = 0;
        *(int *)(ev + 0xb0) = 0;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
/* 0x54dec0, the run handler of the population event: stage 1 rates the town (ord_GetStatistics: the free room
   (sumA - chars) / sumA, the share without a home col[3]): few homeless (< 0.1) -> stage 2, newcomers; many
   (> 0.3) -> stage 6, deaths; else stage 7 from 15:00. Stage 2 draws against chance every hour until 20:00: a
   newcomer is created (cm_SendCreateCharacter, stage 3: it gets a free home, (51 .. 100) * 32 money and its fields
   2 / 0xc set); stage 6 likewise kills a homeless figure of kind 0 without a workplace (-2000 health); stage 7 gives
   a homeless figure of kind 0 / 1 a free home every ten minutes until 22:00, then the event rests until 6:00
   @deep Reconstructed whole against the original (all 8 cases: the switch table equal; every call, push
   order and gm_AddTime argument read from the pushes: case 0 (0,0,5), 1 and the 2/3/6 waits (1,0,0),
   creation (0,0,1), the kill (0,0x1e,0), case 7 (0,0xa,0); ORDSTATS out[4]/out[8] as ints, col[3] against
   0.1 / 0.3, the 2.0f / 1.0f literals; the newcomer's cm_SendMoveGoodsUnchecked(netId, -1, own,
   (51..100)<<5), its field deltas 2 / 0xc; the free-home loop with the unsigned short compare). Left:
   allocation and VC6 cross-jumping. The original keeps -1 in ebp (case 3's c==0 path stores an immediate
   0xffffffff and jumps into the slot==2 block after its store), case 3's found flag in [esp+0x10] with b
   in ebx, case 7's found in ebp with b in [esp+0x10] (ours the reverse: found ebp / b ebx in case 3, found
   [esp+0x10] in case 7); the original merges the cm_SendExSetHE call tails of case 1 (<=0.3), case 2 / 6
   (hour >= 20), case 3's end and all of case 7 into the shared block 0x54e08d..0x54e0ac (the kill path
   even into case 0's gm_AddTime call at 0x54df23) where ours keeps inline copies (tail hunks at +0x19a,
   +0x6c, +0x819; the M8 lines at +0x3a4 / +0x7f4 / +0x835 / +0x83f are those misaligned tails and ai_Time
   copies in another register order). Tried: the while/do forms of both loops (found && n vs n && found:
   the found flag moved between ebp and the slot), case 7's chain as if/else with one return (worse, 692
   ins), chance *= 0.0f vs ai_FloatZero (the literal matches). (references: M8 at +0x3a4: ours refers to
   cm_SendExSetHE (0x4c4c60), the original's field to 0x4fe140 (gm_AddTime) (and 3 more), explained: the M8
   refs are the misaligned tails (our cm_SendExSetHE call aligned against the original's gm_AddTime in a
   merged block) and the ai_Time struct copies whose dwords load in another register order; every call and
   store is present on both sides) (size: 677 vs 657 instructions, explained: ours 677 vs 657: the 20 extra
   are the inline copies of the call / epilogue tails the original cross-jumps into 0x54e097 (case 7's two
   cm_SendExSetHE(0) tails, 2 x 9, and case 6's kill path reusing case 0's gm_AddTime call)) */

void he_RunPopulation(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEPOPULATION *p = (HEPOPULATION *)ev;
    AIBLD *b;
    AICHAR *c;
    int found, zero, id, n, i, j;
    int kind[2];
    ORDSTATS st;
    float x;

    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    switch (e->stage) {
    case 0:
        p->chance *= 0.0f;
        p->step = 0.0f;
        p->count = 0;
        gm_AddTime(&e->end, 0, 0, 5);
        e->exSlot = cm_SendExSetHE(1, ev);
        return;
    case 1:
        ord_GetStatistics((int *)&st);
        x = (float)(st.sumA - st.chars) / st.sumA;
        p->chance = 1.0f - x;
        p->step = x / (p->count + 2);
        gm_AddTime(&e->end, 1, 0, 0);
        if (st.col[3] < 0.1) {
            p->chance = (2.0f - st.score) * p->chance;
            e->exSlot = cm_SendExSetHE(2, ev);
            return;
        }
        if (st.col[3] > 0.3) {
            p->chance = (float)fabs((st.score + 1.0f) * p->chance);
            p->step = (float)fabs(p->step);
            e->exSlot = cm_SendExSetHE(6, ev);
            return;
        }
        e->end = ai_Time;
        e->end.hour = 0xf;
        e->exSlot = cm_SendExSetHE(7, ev);
        return;
    case 2:
        if (ai_Time.hour >= 0x14) {
            e->end = ai_Time;
            e->end.min++;
            e->exSlot = cm_SendExSetHE(7, ev);
            return;
        }
        if (util_RandomF() > p->chance) {
            p->slot = -1;
            p->slot = cm_SendCreateCharacter(0xff, -1, -1, util_Random(0xc) + 0xf, -1, 0, 0, 2);
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 0, 1);
            e->exSlot = cm_SendExSetHE(3, ev);
            return;
        }
        p->chance -= p->step;
        e->end = ai_Time;
        gm_AddTime(&e->end, 1, 0, 0);
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 3:
        if (cm_GetSlotState(p->slot) == 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 0, 1);
            e->exSlot = cm_SendExSetHE(3, ev);
            return;
        }
        if (cm_GetSlotState(p->slot) == 2) {
            gm_AddTime(&e->end, 1, 0, 0);
            p->slot = -1;
            e->exSlot = cm_SendExSetHE(6, ev);
            return;
        }
        c = (AICHAR *)cm_GetSlotValue(p->slot);
        id = (unsigned short)util_Random(2);
        zero = 0;
        if (c == 0) {
            gm_AddTime(&e->end, 1, 0, 0);
            p->slot = -1;
            e->exSlot = cm_SendExSetHE(6, ev);
            return;
        }
        found = 0;
        n = 0x100;
        i = (unsigned short)util_Random(0x100);
        while (found == 0 && n != 0) {
            b = &ai_BldsT[i];
            if (ai_BTypesT[b->type].kind == 1 && ord_GetKindRecord(b->type, kind) != 0
                && (unsigned short)b->residents < (unsigned short)kind[0])
                found = 1;
            n--;
            i = (i + 1) % 0x100;
        }
        if (found != 0)
            cm_SendBoss(c->netId, b->id);
        cm_SendMoveGoodsUnchecked(c->netId, -1, ai_OwnPlayer, ((unsigned short)util_Random(0x32) + 0x33) << 5);
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldDelta(1, 1, 2, &zero);
        cm_AddFieldDelta(1, 1, 0xc, &id);
        cm_SendFieldDeltas();
        e->end = ai_Time;
        gm_AddTime(&e->end, 1, 0, 0);
        p->count++;
        e->exSlot = cm_SendExSetHE(1, ev);
        return;
    case 6:
        if (ai_Time.hour >= 0x14) {
            e->end = ai_Time;
            e->end.min++;
            e->exSlot = cm_SendExSetHE(7, ev);
            return;
        }
        if (util_RandomF() > p->chance) {
            i = (unsigned short)util_Random(0x300);
            n = 0x300;
            do {
                if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].health != 0 && ai_CharsT[i].kind == 0 && ai_CharsT[i].bld == 0) {
                    gm_AddHealth(i, -2000);
                    p->count++;
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            } while (n != 0);
            p->slot = -1;
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 0x1e, 0);
            e->exSlot = cm_SendExSetHE(1, ev);
            return;
        }
        p->chance -= p->step;
        e->end = ai_Time;
        gm_AddTime(&e->end, 1, 0, 0);
        e->exSlot = cm_SendExSetHE(6, ev);
        return;
    case 7:
        found = 0;
        i = (unsigned short)util_Random(0x300);
        n = 0x300;
        do {
            if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].health != 0
                && ((ai_CharsT[i].kind == 0 && ai_CharsT[i].bld2 == 0) || (ai_CharsT[i].kind == 1 && ai_CharsT[i].bld2 == 0))) {
                found = 1;
                break;
            }
            i = (i + 1) % 0x300;
            n--;
        } while (n != 0);
        if (found != 0) {
            found = 0;
            n = 0x100;
            j = (unsigned short)util_Random(0x100);
            while (found == 0 && n != 0) {
                b = &ai_BldsT[j];
                if (ai_BTypesT[b->type].kind == 1 && ord_GetKindRecord(b->type, kind) != 0
                    && (unsigned short)b->residents < (unsigned short)kind[0])
                    found = 1;
                n--;
                j = (j + 1) % 0x100;
            }
            if (found != 0)
                cm_SendBoss(ai_CharsT[i].netId, b->id);
        }
        gm_AddTime(&e->end, 0, 0xa, 0);
        if (e->end.hour >= 0x16) {
            e->end = ai_Time;
            e->end.year++;
            e->end.min = 0;
            e->end.hour = 6;
            e->exSlot = cm_SendExSetHE(0, ev);
            return;
        }
        if (e->end.hour < 0xf) {
            e->end = ai_Time;
            e->end.min++;
            e->exSlot = cm_SendExSetHE(0, ev);
            return;
        }
        e->exSlot = cm_SendExSetHE(7, ev);
        return;
    }
}

int ai_InventoryHas(void *ch, int id);
int gm_CatchDisease();
float gm_CharacterHealth();

/* 0x54e770, a Ghidra miss (disassembled): the allocation of the "Geliebte" (lover) event: the aggressor +0xac (ebx), the victim +0xb0 (edi): the clock now, posted at once; a gone victim or one with the flag 0x800000 (+0x1c8) ends it, else the flag is set (0x4c4a60), the relation of the aggressor falls by 0x32 (0x4c4b00) and the event goes to the state 2 (+0xb4 = 0, +0xb8 = 2, +0xbc = -1) (ROUGH) */
void he_StartLover(int ev)
{
    int edi = gm_FindChar(*(int *)(ev + 0xb0));
    int ebx = gm_FindChar(*(int *)(ev + 0xac));

    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    if (edi == 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if ((*(unsigned *)(edi + 0x1c8) & HE_LOVER_FLAG) != 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    cm_BeginGroup("Alloc_Geliebte");
    cm_SendSetField(*(int *)(edi + 4), 0x1c8, 4, HE_LOVER_FLAG, 0);
    cm_SendRelations(*(int *)(ebx + 4), *(int *)(edi + 4), 0x32, 1);
    cm_EndGroup("Alloc_Geliebte AI");
    *(int *)(ev + 0xb4) = 0;
    *(int *)(ev + 0xb8) = 2;
    *(int *)(ev + 0xbc) = -1;
    *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
}
/* 0x54e890, the run handler of the lover event ("Geliebte"): stage 2 waits for the posting (cm_GetSlotState: 1 goes
   on, 2 ends it); while the keeper lives, every day the keeper pays the lover (an AP event of -pay with the news
   0xb4 / 0xb5 by the lover's sex) and the rounds count up; stage 1 or a dead keeper ends the affair (the lover's
   flag 0x800000 cleared, at stage 1 the relation falls by (2 + rounds) * 5); stage -2 frees it with the relation
   -(8 + rounds) * 5 */

void he_RunLover(int ev)
{
    char text[0x200];
    AIJOB *e = (AIJOB *)ev;
    HELOVER *l = (HELOVER *)ev;
    AICHAR *lover, *keeper;
    int h;

    lover = (AICHAR *)gm_FindChar(l->lover);
    keeper = (AICHAR *)gm_FindChar(l->keeper);
    if (e->stage == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage == -2) {
        if ((e->opts & 4) != 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        if (lover != 0) {
            cm_SendSetField(lover->netId, 0x1c8, 4, 0, HE_LOVER_FLAG);
            if (keeper != 0)
                cm_SendRelations(keeper->netId, lover->netId, (-8 - l->rounds) * 5, 1);
        }
        he_FreeEvent((int *)ev);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    if (lover == 0) {
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    if (e->stage == 2) {
        h = cm_GetSlotState(e->exSlot);
        if (h == 0)
            return;
        if (h == 1) {
            gm_AddTime(&e->end, 0, 2, 0);
            cm_SendExSetHE(0, ev);
            return;
        }
        if (h == 2) {
            gm_AddTime(&e->end, 0, 2, 0);
            cm_SendExSetHE(-1, ev);
        }
        return;
    }
    if (keeper != 0 && keeper->health != 0) {
        if (e->stage == 1) {
            cm_SendSetField(lover->netId, 0x1c8, 4, 0, HE_LOVER_FLAG);
            cm_SendRelations(keeper->netId, lover->netId, (-2 - l->rounds) * 5, 1);
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        if (e->stage == 3) {
            e->end = ai_Time;
            cm_SendExSetHE(0, ev);
            return;
        }
        if (lover->sex == 1)
            txt_Format(text, 0xb4, lover->id);
        else
            txt_Format(text, 0xb5, lover->id);
        he_AddApEvent(e->chr, text, -l->pay, 0);
        l->rounds++;
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->exSlot = cm_SendExSetHE(0, ev);
        return;
    }
    cm_SendSetField(lover->netId, 0x1c8, 4, 0, HE_LOVER_FLAG);
    gm_AddTime(&e->end, 0, 2, 0);
    e->exSlot = cm_SendExSetHE(-1, ev);
}
/* 0x54eb80, a Ghidra miss: the figure record test of the street simulation: alive, kind below 10, and (0x47e840 or 0x47e7f0 non zero) */

int he_IsSimCandidate(int rec)
{
    if (*(unsigned short *)rec == 0xffff)
        return 0;
    if (*(unsigned char *)(rec + 8) == 0)
        return 0;
    if (*(unsigned char *)(rec + 2) >= 10)
        return 0;
    if (ai_IsLocalWorker(rec) != 0)
        return 1;
    return ai_CanAct(rec) != 0;
}
/* 0x54ebd0, the run handler of the disease simulation, event 0x70 (HESIM, the twin of he_RunAccidentSim; the text
   "RunSimKrankheiten: calculating at <Day ..>" is formatted and never shown). Freed (stage -2) it frees the event.
   A round starts with a random stride of 16 numbers prime to 0x300 and a random first character; each run (stage
   0) visits the next 12 characters of the round: a candidate (he_IsSimCandidate) falls ill when a roll of 0 .. 100
   stays below its risk: without a home 25 - 10 * random (three quarters of it with the item 0x2b3), less the home's
   skill 8 * 0.0125 * 20 (0.02 for a home of kind other than 2). gm_CatchDisease draws the disease and its health;
   a worker whose building has an owner is reported (he_CharIllness), else a player figure (kind 6 / 7) healthier
   than that hears the news 0x163c. After all 0x300 the round is over: due the next day at 10:00, else due in 30
   minutes, the next day at 10:00 once that is 18:00 or later */
void he_RunDiseaseSim(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HESIM *s = (HESIM *)ev;
    AICHAR *c;
    AIBLD *b;
    int n, cur, kind;
    float roll, immunity, risk, health;
    int primes[16];
    char text[0x200];

    if (e->stage == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    txt_Format(text, (int)"-------> RunSimKrankheiten: calculating at <Day = %i Time = %i.2:%i.2>", ai_Time.year, ai_Time.hour, ai_Time.min);
    if (s->step == 0) {
        primes[0] = 1;
        primes[1] = 3;
        primes[2] = 5;
        primes[3] = 7;
        primes[4] = 0xb;
        primes[5] = 0xd;
        primes[6] = 0x11;
        primes[7] = 0x13;
        primes[8] = 0x2ed;
        primes[9] = 0x2ef;
        primes[10] = 0x2f3;
        primes[11] = 0x2f5;
        primes[12] = 0x2f9;
        primes[13] = 0x2fb;
        primes[14] = 0x2fd;
        primes[15] = 0x2ff;
        s->step = primes[(unsigned short)util_Random(0x10)];
        s->cursor = (unsigned short)util_Random(0x300);
    }
    if (e->stage != 0)
        return;
    cur = s->cursor;
    for (n = 12; n != 0; n--) {
        c = &ai_CharsT[cur];
        if (he_IsSimCandidate((int)c) != 0) {
            roll = util_RandomF() * 100.0f;
            risk = 0.0f;
            immunity = 0.0f;
            if (ai_CharsT[cur].bld2 != 0) {
                if (ai_BTypesT[ai_CharsT[cur].bld2->type].kind == 2)
                    immunity = 20.0f * (gm_BuildingSkill((unsigned char *)ai_CharsT[cur].bld2, 8, 1) * 0.0125f);
                else
                    immunity = 20.0f * (gm_BuildingSkill((unsigned char *)ai_CharsT[cur].bld2, 8, 1) * 0.02f);
            } else
                risk = (float)(25.0 - util_RandomF() * 20.0f * 0.5);
            if (ai_InventoryHas(c, 0x2b3) != 0)
                risk *= 0.75f;
            if (roll < risk - immunity && gm_CatchDisease(c, &kind, &health) != 0) {
                b = ai_CharsT[cur].bld;
                if (b != 0 && b->owner != 0xffff && &ai_CharsT[b->owner] != 0)
                    he_CharIllness(e, c, &ai_CharsT[b->owner], b, kind, health);
                else if (gm_CharacterHealth(c) > health && (ai_CharsT[cur].kind == 6 || ai_CharsT[cur].kind == 7)) {
                    txt_Format(text, 0x163c, kind + 0x1632);
                    he_ShowNews(ai_CharsT[cur].netId, -1, text, 0, 0x5f7, ch_EmptyName);
                }
            }
        }
        cur = (s->step + cur) % 0x300;
        s->done++;
    }
    s->cursor = cur;
    if (s->done >= 0x300) {
        e->end.hour = 0xa;
        s->cursor = 0;
        s->step = 0;
        s->done = 0;
        e->end.year++;
        return;
    }
    gm_AddTime(&e->end, 0, 0x1e, 0);
    if (e->end.hour >= 0x12) {
        e->end.min = 0;
        e->end.hour = 0xa;
        e->end.year++;
    }
}
/* 0x54ef70, the init of the "Dunkle Ecke" (dark corner) robbery event 0x74: no carrier, a random walk over the
   buildings, 5 + random(4) steps, due a minute from now; it ends at once when another player's robbery runs at the
   same victim, or the victim building or its job table (object 0x12c) is gone; else the job table gets the year,
   and the carrier figure (cm_SendSpawnCharacter 0xe "DunkleEcke") is spawned at the victim */

void he_StartDarkCorner(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEDARKCORNER *d = (HEDARKCORNER *)ev;
    AIJOB *o;
    AIBLD *bld;
    AIOBJ *jt;
    int year;

    if ((e->opts & 4) != 0)
        return;
    d->carrier = -1;
    d->cursor = (unsigned short)util_Random(0x100);
    d->step = ai_RandomStrides[(unsigned short)util_Random(0x10)];
    d->loot = 0;
    d->target = -1;
    d->steps = (unsigned short)util_Random(4) + 5;
    e->end = ai_Time;
    e->start = ai_Time;
    gm_AddTime(&e->end, 0, 1, 0);
    for (o = (AIJOB *)he_FindEvent(1, 0, 0x74); o != 0; o = (AIJOB *)he_NextEvent()) {
        if (o->id != e->id && ((HEDARKCORNER *)o)->victim == d->victim) {
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
    }
    gm_FindById(&bld, 0, 0, d->victim);
    if (bld == 0 || (jt = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x12c)) == 0) {
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    year = ai_Time.year;
    cm_BeginFieldBlock((char *)jt, jt->objId);
    cm_AddFieldDelta(4, 1, (char *)jt - cm_FieldObject + 0x28, &year);  /* HEJOBTABLE.year */
    cm_SendFieldDeltas();
    cm_BeginGroup("Dunkle Ecke");
    d->carrier = cm_SendSpawnCharacterR(0xe, bld->id, -1, 0, 0, 0, "DunkleEcke");
    e->exSlot = cm_SendExSetHE(0, ev);
    cm_EndGroup("Dunkle Ecke");
}
/* 0x54f160, the run handler of the "Dunkle Ecke" robbery event 0x74 (HEDARKCORNER): stage 0 waits for the carrier
   figure (its id once spawned, else a day later stage 4); stage 1 walks the buildings to the next target of a kind
   in the mask 0x1a7fcc3a6 other than the victim ("Dunkle Ecke: next target = %G") and sends the carrier there;
   stage 2 waits until it is at the door, counts the steps down and raises the loot level by the chance of the
   target owner's skill 2, then sends the carrier on (stage 1) or, its steps done, to the victim (stage 3); stage 3
   at the victim (stage 4 at once) takes up to loot (0 .. 3; stage 4 draws it against the victim owner's skill 2)
   open crimes of others from ai_Crimes as proofs (cm_SendProofExecuted), writes them into the victim's job table
   and tells its owner (the news 0x18a9 / 0x18aa); freed with the flag 2 it removes the carrier
   @deep Reconstructed whole against the original (the five cases of the jump table equal; every call, push
   order and gm_AddTime argument read from the pushes; case 1's 64-bit kind mask 0xa7fcc3a6 / bit 32 with
   the target != the victim (the draft had ==), case 2's loot chance util_RandomF() * 0.5f < skill, case
   4's three draws at 0.66f, the MAX(MIN(loot, 3), 0) clamp, the proof walk over ai_Crimes (CRIME: state 1,
   another culprit, not after the event's start) with loot[0..2] = -1, the job table's proofs[] (HEJOBTABLE
   +0x1c) and year, case 3's two full message calls against case 4's if / else of txt_Format). Left:
   allocation and VC6 cross-jumping. Case 1's walk: the original keeps hi in ebx and computes the AIBTYPE
   pointer (lea eax, [ebx + eax*4]) before the kind byte compare, n in [esp+0x14]; ours keeps hi in ebx but
   lo / n in other homes ([esp+0x18]); the original shares the Send(-1) tails of case 1 / 2 (0x54f311,
   0x54f319) and the AddTime of case 2's not-at-door path, ours keeps inline copies (tail hunks at +0x2ee,
   +0x49d) and shares case 3/4's end at +0x9a5 instead; the message calls schedule add esp later in the
   original; the loot index from util_Random is moved as mov bx, ax in the original (ours mov ebx, eax: the
   same zero extension). Tried: kind as an unsigned char local (the byte compare jbe now matches), the
   crimes read by index instead of a CRIME pointer (matches the original's [edi + ai_Crimes + off]
   addressing), the three -1 stores in order, case 4's ternary (neg / sbb: wrong, reverted to if / else).
   (size: 6 of the instruction difference are shared / inline tails) (references: M8 at +0x989: ours refers
   to ai_Time+0x4 (0x770ae4), the original's field to 0x770ae0 (ai_Time), explained: the one M8 is the
   ai_Time copy of case 4 aligned against a shifted tail (ours loads ai_Time+4 where the alignment pairs
   the original's ai_Time); every call and store is present on both sides) */

void he_RunDarkCorner(int ev)
{
    char text[0x400];
    AIJOB *e = (AIJOB *)ev;
    HEDARKCORNER *d = (HEDARKCORNER *)ev;
    AIBLD *bld;
    AICHAR *c;
    AIOBJ *jt;
    unsigned lo, hi;
    unsigned char kind;
    int n, idx, i, k, w;
    int loot[3];
    float skill;

    if (e->stage == -1 || e->stage == -2) {
        if ((e->opts & 2) != 0) {
            c = (AICHAR *)gm_FindChar(d->carrier);
            if (c != 0) {
                cm_SendChrStop(c->netId);
                cm_SendKillCharacter(c->netId, 1);
            }
        }
        he_FreeEvent((int *)ev);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    switch (e->stage) {
    case 0:
        if (cm_GetSlotState(d->carrier) == 1) {
            d->carrier = ((AICHAR *)cm_GetSlotValue(d->carrier))->netId;
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(1, ev);
            return;
        }
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->exSlot = cm_SendExSetHE(4, ev);
        return;
    case 1:
        idx = d->cursor;
        n = 0x100;
        do {
            bld = &ai_BldsT[idx];
            lo = 0;
            hi = 0;
            kind = ai_BTypesT[bld->type].kind;
            if (kind > 0x1f)
                hi = 1 << (kind - 0x20);
            else
                lo = 1 << kind;
            if (((lo & 0xa7fcc3a6) != 0 || (hi & 1) != 0) && bld->id != d->victim)
                break;
            idx = (idx + d->step) % 0x100;
            bld = 0;
            n--;
        } while (n != 0);
        d->cursor = (d->step + idx) % 0x100;
        if (bld == 0) {
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        txt_Format(text, (int)"Dunkle Ecke: next target = %G", bld);
        c = (AICHAR *)gm_FindChar(d->carrier);
        if (c == 0) {
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        cm_SendChrStop(c->netId);
        cm_SendChrGotoBuilding(c->netId, bld->id, -1, 0, 1, "Dunkle Ecke");
        gm_AddTime(&e->end, 0, 5, 0);
        d->target = bld->id;
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 2:
        gm_FindById(&bld, 0, 0, d->target);
        gm_AddTime(&e->end, 0, 5, 0);
        if (bld == 0) {
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        c = (AICHAR *)gm_FindChar(d->carrier);
        if (c == 0) {
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        if (sv_IsAtDoor(c, bld) == 0) {
            e->stage = 2;
            gm_AddTime(&e->end, 0, 5, 0);
            e->exSlot = cm_SendExSetHE(2, ev);
            return;
        }
        d->steps--;
        skill = gm_GetSkill((char *)&ai_CharsT[bld->owner], 2);
        if (d->loot < 1)
            d->loot++;
        else
            d->loot += util_RandomF() * 0.5f < skill ? 1 : 0;
        if (d->loot > 2)
            d->steps = 0;
        gm_FindById(&bld, 0, 0, d->victim);
        if (bld == 0 || bld->owner == 0xffff)
            break;
        if (d->steps <= 0) {
            cm_SendChrStop(c->netId);
            cm_SendChrGotoBuilding(c->netId, bld->id, d->amount, 0, 0, "Dunkle Ecke");
            e->exSlot = cm_SendExSetHE(3, ev);
            return;
        }
        e->exSlot = cm_SendExSetHE(1, ev);
        return;
    case 3:
        gm_AddTime(&e->end, 0, 5, 0);
        gm_FindById(&bld, 0, 0, d->victim);
        if (bld == 0 || bld->owner == 0xffff)
            break;
        c = (AICHAR *)gm_FindChar(d->carrier);
        if (c != 0 && sv_IsAtDoor(c, bld) == 0) {
            e->stage = 3;
            gm_AddTime(&e->end, 0, 5, 0);
            e->exSlot = cm_SendExSetHE(3, ev);
            return;
        }
        d->loot = MAX(MIN(d->loot, 3), 0);
        if (d->loot != 0) {
            loot[0] = -1;
            loot[1] = -1;
            loot[2] = -1;
            idx = (unsigned short)util_Random(0x200);
            w = ai_CharWalkSteps[(unsigned short)util_Random(8)];
            n = 0;
            for (i = 0x200; i != 0 && n < d->loot; i--) {
                if (ai_CrimesT[idx].id != -1 && ai_CrimesT[idx].state == 1 && ai_CrimesT[idx].culprit != ai_CharsT[bld->owner].netId
                    && gm_CompareTime(&e->start, &ai_CrimesT[idx].time) != 1)
                    loot[n++] = ai_CrimesT[idx].id;
                idx = (idx + w) % 0x200;
            }
            d->loot = n;
            for (k = 0; k < n; k++)
                cm_SendProofExecuted(loot[k], ai_CharsT[bld->owner].netId);
            jt = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x12c);
            if (jt != 0) {
                cm_BeginFieldBlock((char *)jt, jt->objId);
                for (k = 0; k < 3; k++)
                    cm_AddFieldDelta(4, 1, k * 4 + 0x1c - (int)cm_FieldObject + (int)jt, &loot[k]);  /* HEJOBTABLE.proofs[k] */
                cm_SendFieldDeltas();
            }
        }
        if (d->loot != 0) {
            txt_Format(text, 0x18a9);
            he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, -1, text, 0, 0x5ec, bld->id, d->amount, 0, ch_EmptyName);
        } else {
            txt_Format(text, 0x18aa);
            he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, -1, text, 0, 0x5ec, bld->id, d->amount, 0, ch_EmptyName);
        }
        break;
    case 4:
        gm_FindById(&bld, 0, 0, d->victim);
        if (bld == 0 || bld->owner == 0xffff)
            break;
        d->loot = 0;
        for (k = 3; k != 0; k--) {
            skill = gm_GetSkill((char *)&ai_CharsT[bld->owner], 2);
            d->loot += util_RandomF() * 0.66f < skill ? 1 : 0;
        }
        if (d->loot != 0) {
            loot[0] = -1;
            loot[1] = -1;
            loot[2] = -1;
            idx = (unsigned short)util_Random(0x200);
            w = ai_CharWalkSteps[(unsigned short)util_Random(8)];
            n = 0;
            for (i = 0x200; i != 0 && n < d->loot; i--) {
                if (ai_CrimesT[idx].id != -1 && ai_CrimesT[idx].state == 1 && ai_CrimesT[idx].culprit != ai_CharsT[bld->owner].netId
                    && gm_CompareTime(&e->start, &ai_CrimesT[idx].time) != 1)
                    loot[n++] = ai_CrimesT[idx].id;
                idx = (idx + w) % 0x200;
            }
            d->loot = n;
            for (k = 0; k < n; k++)
                cm_SendProofExecuted(loot[k], ai_CharsT[bld->owner].netId);
            jt = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x12c);
            if (jt != 0) {
                cm_BeginFieldBlock((char *)jt, jt->objId);
                for (k = 0; k < 3; k++)
                    cm_AddFieldDelta(4, 1, k * 4 + 0x1c - (int)cm_FieldObject + (int)jt, &loot[k]);  /* HEJOBTABLE.proofs[k] */
                cm_SendFieldDeltas();
            }
        }
        if (d->loot != 0)
            txt_Format(text, 0x18a9);
        else
            txt_Format(text, 0x18aa);
        he_MessageBoxQuickjump(ai_CharsT[bld->owner].netId, -1, text, 0, 0x5ec, bld->id, d->amount, 0, ch_EmptyName);
        e->end = ai_Time;
        break;
    default:
        return;
    }
    e->exSlot = cm_SendExSetHE(-1, ev);
}

int ai_FindUsableGood(int param_1);
D3PARTSYS *d3t_CreateEmitter(D3OBJECT *obj, char *tex, char flag, unsigned long color, D3VECTOR *pos, int count, int unused, float size, float a, float b, float c, float d);
int mem_IsValid();
void par_Delete();
char *gm_FindBuildingById(int id);
char *gm_HomeObject(unsigned char *b);
void cm_SendCharClear(int a, int b, int c);
void cm_SendCharHalt(int a);

/* 0x54fb50, a Ghidra miss: the init of the effect event: the object +0xac (0x4fea50) must have a scene record (+0x61) and the effect word +0xc2 a table record (0x5ba620) with a duration (+4): +0xb4 = now + that duration (0x4fe140), +0xb0 = 0, state 0; else the event is freed */
void he_StartEffectEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *bld;
    G9DOMIZIL *d;

    gm_FindById((int *)&bld, 0, 0, e->bldId);
    if (bld != 0 && bld->object != 0 && (d = (G9DOMIZIL *)ai_FindUsableGood(e->effectGood)) != 0) {
        if (d->minutes != 0) {
            e->end6c = ai_Time;
            gm_AddTime(&e->end6c, 0, d->minutes, 0);
            e->effectObj = 0;
            e->stage = 0;
            return;
        }
    }
    he_FreeEvent((int *)ev);
}
/* 0x54fc00, the smoke emitter of the effect event: creates the effect 0x177 ("Rauch", the colour 0x64ffa0, a
   30 / 2 / 7.5 / 0.4 / 1 emitter) on the scene record (AIBLD.object) of the building o and returns its handle;
   the five floats go as dword bit patterns through d3t_CreateEmitterK */

int he_CreateSmokeEffect(int o)
{
    AIBLD *bld = (AIBLD *)o;
    unsigned char col[4];
    int *color = (int *)col;
    float v[4];

    col[2] = 0xa0;
    col[1] = 0xff;
    col[0] = 0x64;
    v[0] = 0.0f;
    v[1] = 200.0f;
    v[2] = 0.0f;
    return d3t_CreateEmitterK(bld->object, "Rauch", 1, *color, v, 0x12c, 1, 0x41f00000, 0x40000000, 0x40f00000, 0x3ecccccd, 0x3f800000);
}
/* 0x54fc70, the points emitter of the effect event (like he_CreateSmokeEffect, the other colour 0x321e50 and
   size): creates the effect 0x178 ("punkte_nm_2", a 50 / 1 / 2 / 0.5 / 0.5 emitter) on the scene record
   (AIBLD.object) of the building o and returns its handle; the five floats go as dword bit patterns */

int he_CreatePointsEffect(int o)
{
    AIBLD *bld = (AIBLD *)o;
    unsigned char col[4];
    int *color = (int *)col;
    float v[4];

    col[2] = 0x50;
    col[1] = 0x1e;
    col[0] = 0x32;
    v[0] = 0.0f;
    v[1] = 50.0f;
    v[2] = 0.0f;
    return d3t_CreateEmitterK(bld->object, "punkte_nm_2", 1, *color, v, 0x12c, 1, 0x42480000, 0x3f800000, 0x40000000, 0x3f000000, 0x3f000000);
}
/* 0x54fce0, the run handler of the effect event 0x76: stage 0 creates the emitter on the building's scene object
   (he_CreateSmokeEffect for the Domizil good 0x177, he_CreatePointsEffect for 0x178, in scene 0) and goes on to 1
   (no emitter: -1); stage 1 waits for the end time end6c, then stops the emitter's spawning and goes to 2; stage
   2 waits until the emitter has run out (flag 2) and ends; freed (-1 / -2) it deletes a valid emitter */

void he_RunEffectEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *bld;
    int scene;

    switch (e->stage) {
    case 0:
        gm_FindById(&bld, 0, 0, e->bldId);
        if (bld == 0 || bld->object == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        scene = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        if (e->effectGood == 0x177)
            e->effectObj = he_CreateSmokeEffect((int)bld);
        else if (e->effectGood == 0x178)
            e->effectObj = he_CreatePointsEffect((int)bld);
        d3if_SelectScene(scene, 1);
        e->stage = (e->effectObj != 0) * 2 - 1;
        return;
    case 1:
        if (gm_CompareTime(&ai_Time, &e->end6c) == 1) {
            if (e->effectObj != 0 && mem_IsValid(e->effectObj) != 0)
                ((D3PARTSYS *)e->effectObj)->spawnCount = 0;
            e->stage = 2;
            return;
        }
        if (e->effectObj == 0)
            e->stage = 0;
        return;
    case 2:
        if (e->effectObj == 0 || (((D3PARTSYS *)e->effectObj)->flags & 2) != 0)
            e->stage = -1;
        return;
    case -2:
    case -1:
        gm_FindById(&bld, 0, 0, e->bldId);
        if (e->effectObj != 0 && mem_IsValid(e->effectObj) != 0) {
            scene = d3_CurrentScene;
            d3if_SelectScene(0, 1);
            par_Delete(e->effectObj);
            d3if_SelectScene(scene, 1);
        }
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x54fe80, a Ghidra miss: the init of the effect event: the due clock now, +0xb0 = now + the duration of the effect record 0x178 (0x5ba620, +4); no record: freed */

void he_Unk54fe80(int ev)
{
    int r;

    *(AITIME *)(ev + 0x52) = ai_Time;
    r = ai_FindUsableGood(0x178);
    if (r != 0 && *(int *)(r + 4) != 0) {
        *(AITIME *)(ev + 0xb0) = ai_Time;
        gm_AddTime(ev + 0xb0, 0, *(int *)(r + 4), 0);
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x54ff20, the run handler of the send-home event 0x77 (its init 0x54fe80): every 30 minutes until homeUntil, each
   living character standing in the building bldId and sent there (model->bldId) who has a home goes home with a
   chance of one half: its action is cleared (messages 0x47 / 0x4d) and it walks to its home object ("Unkendunk");
   a missing building or a freed event (-1 / -2) frees it
   @deep Read whole against the original and rewritten typed (AIJOB stage/bldId/end, the new
   AIJOB.homeUntil at 0xb0, AICHAR health/model/bld/bld2/netId, the rand > 0.5 test as and 0x4100/jne, the
   768 loop with jl). Every statement, call and argument now equal. Left: VC6's choice of the loop's
   induction pointer inside ai_Chars: ours esi = ai_Chars+0x184 (model) with fields at [esi-0x17c],
   [esi-0x18], [esi-0x14], [esi-0x180], end cmp esi,0x7d6104; the original esi = ai_Chars+4 (netId,
   ai_CharInts) with [esi+4], [esi+0x168], [esi+0x16c], [esi], end 0x7d5f84; each access names the same
   absolute field. Its side effect: ours cmp dword ptr [esi],0 where the original has mov eax,[esi+0x180];
   test eax,eax (+0x4e, 99 vs 100 instructions). Tried: c = &ai_CharsT[i] pointer, direct ai_CharsT[i],
   ((AICHAR *)ai_Chars)[i]: all pick +0x184. Lead: the original may address the record through a sub-object
   at +4. (references: M8 at +0x3b: ours refers to ai_Chars+0x184 (0x770d04), the original's field to
   0x770b84 (ai_CharInts) (and 1 more), explained: the two ref: lines (M8 +0x3b ai_Chars+0x184 vs
   ai_Chars+4, +0xdd the end 0x7d6104 vs 0x7d5f84) are the same induction pointer over ai_Chars at another
   base offset, 0x180 apart on both lines; every load through it reads the same absolute field) */

void he_RunSendHomeEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *bld;
    AIOBJ *home;
    AICHAR *c;
    int i;

    if (e->stage < -2)
        return;
    if (e->stage < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->stage != 0)
        return;
    bld = (AIBLD *)gm_FindBuildingById(e->bldId);
    if (bld == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    for (i = 0; i < 768; i++) {
        c = &ai_CharsT[i];
        if (c->health > 0 && c->model != 0 && c->bld == bld && c->bld2 != 0
            && util_RandomF() > 0.5 && c->model->bldId == e->bldId) {
            home = (AIOBJ *)gm_HomeObject((unsigned char *)c->bld2);
            cm_SendCharClear(c->netId, 0, 0);
            cm_SendCharHalt(c->netId);
            if (home != 0)
                cm_SendChrGotoBuilding(c->netId, c->bld2->id, home->objId, 0, 0, "Unkendunk");
        }
    }
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 0x1e, 0);
    if (gm_CompareTime(&ai_Time, &e->homeUntil) == 1)
        he_FreeEvent((int *)ev);
}

int d3t_PlayAnim();
float d3_VecAngle2();

/* 0x550070, creates the reaper object ("Sensenmann", in scene 0) of the reaper event: both buildings reaperFrom and
   reaperTo must have a 3D object with a mesh driver; the first time it stands above reaperFrom's object (its height
   + 80) turned (y angle) towards the same point above reaperTo's, later at the cached reaperPos / reaperRot; the
   object's position and angles are cached (reaperPlaced = 1), its flags set and "*sensenmann.baf" played, the clock
   reaperClock started. 1 = created, 0 = a building or the object missing
   @deep Read whole against the original and rewritten typed: AIJOB reaper fields
   (reaperFrom/To/Placed/reaper/Pos/Rot at 0xb0..0xe8, reaperClock over stormDue at 0xec), the D3VECTOR4
   locals (the 16-byte slots pos 0x14, target 0x24, rot 0x34, dir 0x44), the one found-building slot reused
   for both lookups with from / to taken after their test chains, rot zeroed before its y angle
   (d3_VecAngle2(d3_ZAxis, dir)), the member-wise caches, d3t_PlayAnim(obj, name, 1). Instruction count 201
   = 201, every other line equal. Left (diffclass: the code hunks are moved instructions): the original
   hoists the push 1 shared by d3t_PlayAnim and the failure tail's d3if_SelectScene above the je at +0x22b
   (the original's je goes to push ebp at +0x2eb), and pushes the '*sensenmann.baf' string before mov byte
   ptr [eax+0x211], cl; ours pushes 1 after the and cl, 0xfd and the string after mov eax,[esi+0xc8]:
   scheduling. The prover stops on the d3t_PlayAnim string literal address only. */
int he_CreateReaper(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int scene = d3_CurrentScene;
    AIBLD *bld, *from, *to;
    D3VECTOR4 pos, target, rot, dir;      /* padded to 16 bytes, as the engine keeps them */
    D3OBJECT *obj;

    d3if_SelectScene(0, 1);
    gm_FindById((int *)&bld, 0, 0, e->reaperFrom);
    if (bld != 0 && bld->object != 0 && ((D3OBJECT *)bld->object)->info != 0 && ((D3OBJECT *)bld->object)->info->mesh != 0) {
        from = bld;
        gm_FindById((int *)&bld, 0, 0, e->reaperTo);
        if (bld != 0 && bld->object != 0 && ((D3OBJECT *)bld->object)->info != 0 && ((D3OBJECT *)bld->object)->info->mesh != 0) {
            to = bld;
            if (e->reaperPlaced == 0) {
                d3_PointToWorld(((D3OBJECT *)from->object), &((D3OBJECT *)from->object)->pos, (D3VECTOR *)&pos);
                pos.y = pos.y + ((D3OBJECT *)from->object)->info->mesh->radius + 80.0f;
                d3_PointToWorld(((D3OBJECT *)to->object), &((D3OBJECT *)to->object)->pos, (D3VECTOR *)&target);
                target.y = target.y + ((D3OBJECT *)to->object)->info->mesh->radius + 80.0f;
                rot.x = 0.0f;
                rot.y = 0.0f;
                rot.z = 0.0f;
                dir.x = target.x - pos.x;
                dir.y = target.y - pos.y;
                dir.z = target.z - pos.z;
                rot.y = d3_VecAngle2(&d3_ZAxis, (D3VECTOR *)&dir);
            } else {
                rot.x = e->reaperRot.x;
                rot.y = e->reaperRot.y;
                rot.z = e->reaperRot.z;
                pos.x = e->reaperPos.x;
                pos.y = e->reaperPos.y;
                pos.z = e->reaperPos.z;
                d3_PointToWorld(((D3OBJECT *)to->object), &((D3OBJECT *)to->object)->pos, (D3VECTOR *)&target);
                target.y = target.y + ((D3OBJECT *)to->object)->info->mesh->radius + 80.0f;
                dir.x = target.x - pos.x;
                dir.y = target.y - pos.y;
                dir.z = target.z - pos.z;
            }
            obj = d3_CreateObject(0, (D3VECTOR *)&pos, (D3VECTOR *)&rot, "Sensenmann");
            e->reaper = obj;
            if (obj != 0) {
                e->reaperPlaced = 1;
                e->reaperRot.x = obj->rotation.x;
                e->reaperRot.y = obj->rotation.y;
                e->reaperRot.z = obj->rotation.z;
                e->reaperPos.x = obj->pos.x;
                e->reaperPos.y = obj->pos.y;
                e->reaperPos.z = obj->pos.z;
                obj->objectFlags &= ~2;
                e->reaper->stateFlags |= 0xc;
                e->reaper->flags &= ~4;
                d3t_PlayAnim(e->reaper, "*sensenmann.baf", 1);
                e->reaperClock = d3_Time;
                d3if_SelectScene(scene, 1);
                return 1;
            }
        }
    }
    d3if_SelectScene(scene, 1);
    return 0;
}
float gm_GoodsPrice(unsigned id, unsigned char p);

/* 0x541b40, the inn meeting ("Stammtisch", see 0x53f430) hears of a table request (cm_ExTableRequest) or of its
   own run (he_RunStammtisch): `cmd` is a four letter code, inn the inn building, table its table object (kind 0x12d,
   the four seats), figId the character who joins / leaves. It tells the seated players and the inn's owner (boss)
   the news 0x1893 .. 0x189d: 'join' (with a full table now), 'leav' (the last one gone; a table that was full tells
   every other player too), 'cler' (cleared), 'new ' (a new meeting, to every player but the owner), and 'exec'
   (the meeting is held: every member pays the owner the price of the goods 0x179 + 0x17a, and all the members and
   the owner rise in each other's relations) */
void he_StammtischCommand(unsigned cmd, AIBLD *inn, AIOBJ *table, int figId)
{
    AICHAR *boss;
    int n;
    int total;
    AICHAR *mem[4];
    char big[0x400];
    char s2[0x100];
    char s1[0x100];
    AICHAR *fig, *pm, *c;
    int *seat;
    int i, j, k, amt;

    n = 0;
    boss = &ai_CharsT[inn->owner];
    seat = table->seats;
    for (i = 4; i != 0; i--, seat++) {
        pm = (AICHAR *)gm_FindChar(*seat);
        if (pm != 0)
            mem[n++] = pm;
    }
    switch (cmd) {
    case 'exec':
        amt = (int)(gm_GoodsPrice(0x17a, ai_OwnPlayer) + gm_GoodsPrice(0x179, ai_OwnPlayer));
        total = 0;
        txt_Format(big, 0x189a, inn, amt);
        for (j = 0; j < n; j++) {
            pm = mem[j];
            cm_SendMoveGoodsUnchecked(boss->netId, pm->netId, ai_OwnPlayer, amt);
            total += amt;
            if (pm->kind == 6 || pm->kind == 7)
                he_MessageBoxQuickjump(pm->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
            cm_SendRelations(boss->netId, pm->netId, 3, 1);
            cm_SendRelations(pm->netId, boss->netId, 3, 1);
        }
        if (boss->kind == 6 || boss->kind == 7) {
            txt_Format(big, 0x1893, inn, total);
            he_MessageBoxQuickjump(boss->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
        }
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (i != j) {
                    cm_SendRelations(mem[i]->netId, mem[j]->netId, 3, 1);
                    cm_SendRelations(mem[j]->netId, mem[i]->netId, 3, 1);
                }
            }
        }
        break;
    case 'cler':
        txt_Format(big, 0x189d, inn);
        for (j = 0; j < n; j++) {
            if (mem[j]->kind == 6 || mem[j]->kind == 7)
                he_MessageBoxQuickjump(mem[j]->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
        }
        break;
    case 'join':
        fig = (AICHAR *)gm_FindChar(figId);
        if (fig == 0)
            break;
        txt_Format(big, 0x189b, inn, fig->id);
        for (j = 0; j < n; j++) {
            if (mem[j]->kind == 6 || mem[j]->kind == 7)
                he_MessageBoxQuickjump(mem[j]->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
        }
        if (boss->kind == 6 || boss->kind == 7) {
            if (++n == 4) {
                txt_Format(s1, 0x1894, inn, fig->id);
                txt_Format(s2, 0x1896);
                txt_Format(big, "%s%s", s1, s2);
            } else {
                txt_Format(big, 0x1894, inn, fig->id);
            }
            he_MessageBoxQuickjump(boss->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
        }
        break;
    case 'leav':
        fig = (AICHAR *)gm_FindChar(figId);
        if (fig == 0)
            break;
        txt_Format(big, 0x189c, inn, fig->id);
        for (j = 0; j < n; j++) {
            if ((mem[j]->kind == 6 || mem[j]->kind == 7) && mem[j]->netId != fig->netId)
                he_MessageBoxQuickjump(mem[j]->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
        }
        if (boss->kind == 6 || boss->kind == 7) {
            if (n - 1 == 0) {
                txt_Format(s1, 0x1895, inn, fig->id);
                txt_Format(s2, 0x1897);
                txt_Format(big, "%s%s", s1, s2);
            } else {
                txt_Format(big, 0x1895, inn, fig->id);
            }
            he_MessageBoxQuickjump(boss->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
        }
        if (n == 4) {
            txt_Format(big, 0x1899, inn);
            for (i = 0; i < 768; i++) {
                c = &ai_CharsT[i];
                if ((c->kind == 6 || c->kind == 7) && c->netId != boss->netId) {
                    for (k = 0; k < 4; k++) {
                        if (c->netId == mem[k]->netId)
                            break;
                    }
                    if (k < 4)
                        he_MessageBoxQuickjump(c->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
                }
            }
        }
        break;
    case 'new ':
        txt_Format(big, 0x1898, inn);
        for (i = 0; i < 768; i++) {
            c = &ai_CharsT[i];
            if ((c->kind == 6 || c->kind == 7) && c->netId != boss->netId)
                he_MessageBoxQuickjump(c->netId, -1, big, 0, 0x5ec, inn->id, -1, 0, ch_EmptyName);
        }
        break;
    }
}

int gm_MinutesBetween();
void cm_BeginGroup(const char *s);

/* 0x542100, the run handler of the inn meeting event 0x2a ("Stammtisch", he_AllocStammtisch; its scripts
   he_StammtischCommand): the inn (innId) must have its table object 0x12d, else it waits 5 minutes. With an inn owner
   who is alive, not the player and not of kind 0xf: in state 1 the meeting is announced ('new ', the state counted
   down), else when the meeting time has come it is held ('exec') (before that it waits an hour); either way the next
   meeting is planned a day ahead at a random hour 0x13 .. 0x16. Without such an owner the table is cleared once
   ('cler' request, the state raised) and the meeting planned again */
void he_RunStammtisch(int r)
{
    AIJOB *e = (AIJOB *)r;
    HESTAMMTISCH *m = (HESTAMMTISCH *)r;
    AIBLD *bld;
    AIOBJ *table;
    AICHAR *boss;
    int cmd[3];

    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)r);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    gm_FindById(&bld, 0, 0, m->innId);
    if (bld == 0) {
        gm_AddTime(&e->end, 0, 5, 0);
        cm_SendExSetHE(-1, r);
        return;
    }
    table = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x12d);
    if (table == 0) {
        gm_AddTime(&e->end, 0, 5, 0);
        cm_SendExSetHE(-1, r);
        return;
    }
    if (bld->owner != 0xffff && (boss = &ai_CharsT[bld->owner]) != 0 && boss != he_PlayerCh && boss->health != 0
        && boss->kind != 0xf) {
        if (m->state == 1) {
            he_StammtischCommand('new ', bld, table, -1);
            m->state--;
            e->end = ai_Time;
            gm_AddTime(&e->end, 1, 0, 0);
            m->meeting = ai_Time;
            gm_AddTime(&m->meeting, 0x18, 0, 0);
        } else {
            if (gm_MinutesBetween(&ai_Time, &m->meeting) > 0) {
                gm_AddTime(&e->end, 1, 0, 0);
                cm_SendExSetHE(0, r);
                return;
            }
            he_StammtischCommand('exec', bld, table, -1);
            e->end = ai_Time;
            gm_AddTime(&e->end, 1, 0, 0);
            m->meeting = ai_Time;
            gm_AddTime(&m->meeting, 0x18, 0, 0);
        }
        m->meeting.min = 0;
        m->meeting.hour = util_Random(4) + 0x13;
        cm_SendExSetHE(0, r);
        return;
    }
    if (m->state == 0) {
        cmd[0] = 'cler';
        cmd[1] = -1;
        cmd[2] = bld->id;
        cm_BeginGroup("stammtisch clear HE");
        cm_SendTableRequest(cmd);
        cm_EndGroup("stammtisch clear HE");
        m->state++;
    }
    e->end = ai_Time;
    gm_AddTime(&e->end, 1, 0, 0);
    m->meeting = ai_Time;
    gm_AddTime(&m->meeting, 0x18, 0, 0);
    m->meeting.min = 0;
    m->meeting.hour = util_Random(4) + 0x13;
    cm_SendExSetHE(0, r);
}

int cm_SendObject1b5(int owner, G17B7 value);

/* 0x542490, a Ghidra miss (disassembled): the event of the building `from`: sets its end clock to a day ahead at 7:00;
   with the building gone it is posted again (key -1), else a building without an object of kind 0x1b5 gets one
   (cm_SendObject1b5, its value all -1) and the event is posted (key 0) */
void he_AllocTavern(int r)
{
    AIJOB *e = (AIJOB *)r;
    AIBLD *bld = 0;
    HEOBJ1B5VALUE v;

    if ((e->opts & 4) != 0)
        return;
    e->end = ai_Time;
    gm_AddTime(&e->end, 0x18, 0, 0);
    e->end.hour = 7;
    e->end.min = 0;
    gm_FindById(&bld, 0, 0, e->from);
    if (bld == 0) {
        cm_SendExSetHE(-1, r);
        return;
    }
    if (gm_Query((void *)bld->objList, 1, 0, 0x1b5) == 0) {
        v.f.c1c = -1;
        v.f.c1d = -1;
        v.f.v1e = -1;
        cm_SendObject1b5(bld->id, v.block);
    }
    e->exSlot = -1;
    cm_SendExSetHE(0, r);
}

int ai_ChooseObjectTarget();
int gm_HandOutMoney();
void ai_ShowTavernHelperNews(char *param_1, char *param_2, int param_3);

/* 0x542590, a Ghidra miss (disassembled): the run handler of the tavern event 0x2b of the building `from` (no owner:
   again in a day at 7:00). Stage 0: an AI owner may hire a helper (ai_ChooseObjectTarget on the tavern's object
   0x1b5), a player's helper is hired when the object says so (hired): then stage 1 in an hour, else again in a day.
   Stage 1 waits 1 .. 2 hours. Stage 2: the hired helper is done: its news (ai_ShowTavernHelperNews), the object is
   reset (no helper), and when the tavern pays out (gm_HandOutMoney) its owner gets the takings (the news 0x19a7 for a
   player) and the building's field 0x69 the amount; again in a day at 7:00
   @deep Read whole against the original and rebuilt typed (AIJOB, AIBLD, the tavern object view
   HETAVERNOBJ, int out[3] whose out[2] gm_HandOutMoney reads as the partner = the helper). Every call,
   store and branch matches; what is left is VC6's tail merging and scheduling: (1) code hunk at +0x2b8,
   stage 0's AI branch: ours 'je X; push 0; push 0; lea ecx,[esi+0x52]; push 1; push ecx; jmp' (the hour
   block inline, the 0x18 block shared), the original 'push 0; push 0; jne X; lea edx,[esi+0x52]; push
   0x18; push edx; jmp' (the 0x18 block inline, the hour block shared with the player branch); tried the AI
   / player branch order both ways and the inverted AI condition: those merge more tails (dist 46 / 52).
   (2) tail at +0x32d: the original jumps from the player's 0x18 block into the shared Send(0) call. (3)
   order at +0x102: the original loads t->helperId before the three field-value stores (no, none, noId) and
   out[2] = 0; the prover stops there (a call argument read after other local stores). (size: 6 of the
   instruction difference are shared / inline tails) */
void he_RunTavern(int r)
{
    AIJOB *e = (AIJOB *)r;
    AIBLD *bld = 0;
    HETAVERNOBJ *t;
    AICHAR *helper;
    int out[3];
    unsigned char no, none;
    int noId;
    char buf[0x100];

    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)r);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    gm_FindById(&bld, 0, 0, e->from);
    if (bld == 0) {
        cm_SendExSetHE(-1, r);
        return;
    }
    if (bld->owner == 0xffff) {
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = 7;
        cm_SendExSetHE(0, r);
        return;
    }
    t = (HETAVERNOBJ *)gm_Query((void *)bld->objList, 1, 0, 0x1b5);
    switch (e->stage) {
    case 0:
        if (ai_CharsT[bld->owner].kind == 6 || ai_CharsT[bld->owner].kind == 7) {
            if (t != 0 && t->hired != 0) {
                gm_AddTime(&e->end, 1, 0, 0);
                e->exSlot = cm_SendExSetHE(1, r);
                return;
            }
            gm_AddTime(&e->end, 0x18, 0, 0);
            e->end.hour = 7;
            e->exSlot = cm_SendExSetHE(0, r);
            return;
        }
        if (t != 0 && ai_ChooseObjectTarget(&ai_CharsT[bld->owner], t) != 0) {
            gm_AddTime(&e->end, 1, 0, 0);
            e->exSlot = cm_SendExSetHE(1, r);
            return;
        }
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = 7;
        e->exSlot = cm_SendExSetHE(0, r);
        return;
    case 1:
        gm_AddTime(&e->end, util_RandomU(2) + 1, 0, 0);
        e->exSlot = cm_SendExSetHE(2, r);
        return;
    case 2:
        if (t != 0 && t->hired != 0) {
            no = 0;
            none = 0xff;
            noId = -1;
            out[2] = 0;
            helper = (AICHAR *)gm_FindChar(t->helperId);
            if (helper != 0) {
                out[2] = (int)helper;
                ai_ShowTavernHelperNews((char *)&ai_CharsT[bld->owner], (char *)helper, (int)bld);
            }
            cm_BeginFieldBlock((char *)t, t->objId);
            cm_AddFieldDelta(1, 1, 0x22, &no);
            cm_AddFieldDelta(1, 1, 0x1c, &none);
            cm_AddFieldDelta(1, 1, 0x1d, &none);
            cm_AddFieldDelta(4, 1, 0x1e, &noId);
            cm_SendFieldDeltas();
            if (gm_HandOutMoney(bld, out) != 0) {
                if (ai_CharsT[bld->owner].kind == 6 || ai_CharsT[bld->owner].kind == 7) {
                    txt_Format(buf, 0x19a7, out[0], out[1]);
                    he_ShowNews(ai_CharsT[bld->owner].netId, -1, buf, 0, 0x5ff, ch_EmptyName);
                }
                cm_SendMoveGoods(ai_CharsT[bld->owner].netId, -1, ai_OwnPlayer, out[1]);
                cm_BeginFieldBlock((char *)bld, bld->id);
                cm_AddFieldDelta(4, 1, 0x69, &out[1]);
                cm_SendFieldDeltas();
            }
        }
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = 7;
        e->exSlot = cm_SendExSetHE(0, r);
    }
}
/* 0x543020, the alloc handler of the quarrel event 0x2e between the figures partner1 (+0xac) and partner2 (+0xb0):
   re-posts the event in 0x30 units, takes 0x14 relation points of partner2 towards partner1, tells partner1 (news
   0x175a) and reports the crime (gv_Accuse 0x13, 1, partner2, partner1) with the home building (AICHAR.bld2) id of
   partner2, else of partner1 */

void he_AllocQuarrel(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *a, *b;
    AIBLD *t;
    int id;
    char buf[0x200];
    unsigned char fl;

    a = (AICHAR *)gm_FindChar(e->partner1);
    b = (AICHAR *)gm_FindChar(e->partner2);
    fl = e->opts;
    e->exSlot = -1;
    if ((fl & 2) != 0) {
        if (a == 0 || b == 0) {
            e->end = ai_Time;
            cm_SendExSetHE(-1, ev);
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0x30, 0, 0);
        cm_SendExSetHE(0, ev);
        cm_SendRelations(b->netId, a->netId, -20, 1);
        if (a->kind == 6 || a->kind == 7) {
            txt_Format(buf, 0x175a, b->id, b->id);
            he_ShowNews(a->netId, -1, buf, 0, 0x5fb, (char *)(unsigned)&ch_EmptyName);
        }
        t = b->bld2;
        id = -1;
        if (t != 0) {
            id = t->id;
        } else {
            t = a->bld2;
            if (t != 0)
                id = t->id;
        }
        gv_Accuse(0x13, 1, e->partner2, e->partner1, id);
    }
}
/* 0x543180, a Ghidra miss (disassembled): the follow-up of the quarrel: tells b (news 0x175d, message 0x5fa) when flag 2 is set, then re-posts in 1 unit (state 0 only) */

void he_Unk543180(int ev)
{
    int a, b;
    char buf[0x100];

    if (*(int *)(ev + 0x70) == -1 || *(int *)(ev + 0x70) == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    a = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_FindChar(*(int *)(ev + 0xb0));
    if (b == 0 || a == 0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if (*(int *)(ev + 0x70) == 0) {
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            txt_Format(buf, 0x175d, *(unsigned short *)a);
            he_ShowNews(*(int *)(b + 4), -1, buf, 0, 0x5fa, (char *)0);
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
    }
}

int io_PrintText(char *fmt, ...);
int snd_PlayVoice(int id, int bank, char *text, int var);
float gm_Mix(int a, int b, int c);

/* 0x5432f0, a Ghidra miss (disassembled): the run handler of the master exam event ("Meisterpruefung") of the three
   characters who[0] (the master), who[1] (the one told) and who[2] (the candidate); gone ones end it, and a candidate
   who is no player skips to stage 2. Stage 0 opens the window "Berufe\Messagebox_Meisterpruefung" (news 0x5ec) with
   the text 0x1763 and the voice "_NACHRICHTEN_HS_20", due in a day. Stage 1 waits for the answer in that window
   (d2_ClickedId 0: passed, 1: failed) or for the due time (then stage 2). Stage 2 decides by chance
   (gm_Mix(who[1], who[0], 4) + a random 0 .. 1 below 1: failed). Passed: who[1] hears 0x1764 and the master's
   relation to the candidate drops by 20; failed: 0x1765 and who[1] and the candidate drop by 15 each way */
void he_RunMasterExam(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEOFFER *o = (HEOFFER *)ev;
    AICHAR *p, *q, *r;
    float f;
    char buf[0x100];

    p = (AICHAR *)gm_FindChar(o->who[0]);
    q = (AICHAR *)gm_FindChar(o->who[1]);
    r = (AICHAR *)gm_FindChar(o->who[2]);
    if (e->stage == -2 || p == 0 || q == 0 || r == 0) {
        he_FreeEvent((int *)ev);
        if (e->slot != 0)
            he_ReleaseSlot(ev);
        return;
    }
    if (e->stage == -1 || (r->kind != 6 && r->kind != 7))
        e->stage = 2;
    switch (e->stage) {
    case 0:
        he_OpenEventSlot(ev, 0, 0, "Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        if (e->slot == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_SetForm(e->slot->win, 0);
        io_PrintText(0x1763, q->id, p->id);
        e->start = ai_Time;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0x18, 0, 0);
        snd_PlayVoice(-7, 0, "_NACHRICHTEN_HS_20", 0);
        e->stage++;
        return;
    case 1:
        if (gm_CompareTime(&ai_Time, &e->end) > 0) {
            he_ReleaseSlot(ev);
            e->stage = 2;
            return;
        }
        if (d2_ClickedForm != e->slot->win || d2_ClickedId == -1)
            return;
        if (d2_ClickedId == 0) {
            txt_Format(buf, 0x1764, r->id, p->id);
            he_ShowNews(q->netId, q->netId, buf, 0, 0x5fa, 0);
            cm_SendRelations(p->netId, r->netId, -20, 1);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (d2_ClickedId == 1) {
            txt_Format(buf, 0x1765, r->id, p->id);
            he_ShowNews(q->netId, q->netId, buf, 0, 0x5fa, 0);
            cm_SendRelations(q->netId, r->netId, -15, 1);
            cm_SendRelations(r->netId, q->netId, -15, 1);
        }
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    case 2:
        f = gm_Mix((int)q, (int)p, 4);
        if (util_RandomF() + f >= 1.0f) {
            txt_Format(buf, 0x1764, r->id, p->id);
            he_ShowNews(q->netId, q->netId, buf, 0, 0x5fa, 0);
            cm_SendRelations(p->netId, r->netId, -20, 1);
        } else {
            txt_Format(buf, 0x1765, r->id, p->id);
            he_ShowNews(q->netId, q->netId, buf, 0, 0x5fa, 0);
            cm_SendRelations(q->netId, r->netId, -15, 1);
            cm_SendRelations(r->netId, q->netId, -15, 1);
        }
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x5436a0, a Ghidra miss (disassembled): starts the group event: state 0, clock +0x52 in 0x18 units (state word 0x14); with +0xc0 == 1 every member figure (+0xac .. +0xb8) gets the message 0x36 carrying the event clock and id (flag 9 for the players, else 1), then posts the event (ROUGH) */

void he_Unk5436a0(int ev)
{
    char msg[0xf8];
    int *pp;
    int k, m;
    unsigned char fl;

    fl = *(unsigned char *)(ev + 0x78);
    *(int *)(ev + 0x70) = 0;
    if ((fl & 4) == 0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 0x14;
        *(int *)(ev + 0x58) = 0;
        if (*(int *)(ev + 0xc0) == 1) {
            pp = (int *)(ev + 0xac);
            for (k = 4; k != 0; k--, pp++) {
                m = gm_FindChar(*pp);
                if (m != 0) {
                    HM_B(msg, 4) = 0x36;
                    HM_I(msg, 8) = *(int *)(m + 4);
                    HM_I(msg, 0xc) = -1;
                    HM_I(msg, 0x68) = 1;
                    *(AITIME *)(msg + 0x58) = *(AITIME *)(ev + 0x52);
                    HM_I(msg, 0x6c) = *(int *)(ev + 4);
                    if (*(char *)(m + 2) == 6 || *(char *)(m + 2) == 7)
                        HM_B(msg, 0x36) = 9;
                    else
                        HM_B(msg, 0x36) = 1;
                    cm_SendLongMessage(msg);
                }
            }
        }
        cm_SendExSetHE(0, ev);
    }
}

void cm_SendHandOverCrimes(int a, int b, int c, int d);

/* 0x542960, the alloc handler of the offer event 0x2c between the figures partner1 (+0xac) and partner2 (+0xb0):
   sets the event's end clock to ai_Time and the offer clock (+0xc5) to now + 0x18 units, hour 0xa, minute 0; when
   opts bit 2 is set it walks the other pending 0x2c offers of the same kind: an offer of the same figure only
   shows the news 0x1bbe and re-posts this event at once, an older one of another figure moves our clock to its
   time; at the end the offer is created (cm_SendHandOverCrimes, +0xd4) and the event posted */
void he_AllocOffer(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    HEOFFER *o = (HEOFFER *)ev;
    AIJOB *o2;
    HEOFFER *f2;
    AICHAR *q, *p;
    int r;
    unsigned char fl;
    char buf[0x100];

    q = (AICHAR *)gm_FindChar(e->partner2);
    p = (AICHAR *)gm_FindChar(e->partner1);
    o->clock = ai_Time;
    gm_AddTime(&o->clock, 0x18, 0, 0);
    o->clock.hour = 0xa;
    o->clock.min = 0;
    e->end = ai_Time;
    fl = e->opts;
    if ((fl & 2) != 0) {
        r = (int)he_FindEvent(1, 0, 0x2c);
        while (r != 0) {
            o2 = (AIJOB *)r;
            f2 = (HEOFFER *)r;
            if (o2->id != e->id && f2->kind == o->kind) {
                if (o2->partner1 == e->partner1) {
                    txt_Format(buf, 0x1bbe, p->id, q->id);
                    he_ShowNews(p->netId, p->netId, buf, 0, 0x5f5, (char *)0);
                    e->end = ai_Time;
                    e->exSlot = cm_SendExSetHE(-1, ev);
                    return;
                }
                if (o->clock.year <= f2->clock.year) {
                    o->clock = f2->clock;
                    gm_AddTime(&o->clock, 0x18, 0, 0);
                    o->clock.hour = 0xa;
                    o->clock.min = 0;
                }
            }
            r = (int)he_NextEvent();
        }
        o->offer = ((int (*)(int, int, int, int))cm_SendHandOverCrimes)(e->partner1, e->partner2, e->id, 1);
        e->exSlot = cm_SendExSetHE(0, ev);
    }
}

int cm_SendNewParty(void *p);

/* 0x542b60, a Ghidra miss (disassembled): the run handler of the wedding offer event (see 0x542960; HEOFFER). Stage 0
   waits for the answer to the offer (cm_GetSlotState of the offer handle: 1 accepted, 2 refused). Accepted: the end
   clock becomes the offer's clock, the partner and the suitor get the news 0x1bc0 / 0x1bbf ("_NACHRICHTEN_HS_16" /
   "_NACHRICHTEN_HS_15"), the guests who[3] / who[4] the news 0x1bc1 (HS_11) and who[2] / who[5] the news 0x1bc2
   (HS_12) when they are players, and the event goes on to stage 1. Refused: the suitor hears 0x1bbe and the event is
   posted again two minutes on. Stage 1 sends the wedding party (G28REC, kind 2, flags 0xa2, its 5 or 6 members, 45
   minutes from now, hosted by the player) and posts the event again */
void he_RunWeddingOffer(int r)
{
    AIJOB *e = (AIJOB *)r;
    HEOFFER *o = (HEOFFER *)r;
    AICHAR *partner, *suitor, *g2, *g3, *g4, *g5;
    char ok;
    G28REC m;
    char buf[0x200];

    if (e->stage == -1 || e->stage == -2) {
        he_FreeEvent((int *)r);
        return;
    }
    if ((e->opts & 4) != 0)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    switch (e->stage) {
    case 0:
        ok = cm_GetSlotState(o->offer);
        if (ok == 0)
            return;
        partner = (AICHAR *)gm_FindChar(o->who[1]);
        suitor = (AICHAR *)gm_FindChar(o->who[0]);
        g2 = (AICHAR *)gm_FindChar(o->who[2]);
        g3 = (AICHAR *)gm_FindChar(o->who[3]);
        g4 = (AICHAR *)gm_FindChar(o->who[4]);
        g5 = (AICHAR *)gm_FindChar(o->who[5]);
        if (ok == 1) {
            e->end = o->clock;
            if (partner->kind == 6 || partner->kind == 7) {
                txt_Format(buf, 0x1bc0, suitor->id, &e->end, e->end.hour, g2->id, g3->id, g4->id);
                he_ShowNews(partner->netId, partner->netId, buf, 0, 0x5f5, "_NACHRICHTEN_HS_16");
            }
            if (suitor->kind == 6 || partner->kind == 7) {
                txt_Format(buf, 0x1bbf, partner->id, &e->end, e->end.hour, g2->id, g3->id, g4->id);
                he_ShowNews(suitor->netId, suitor->netId, buf, 1, 0x5f5, "_NACHRICHTEN_HS_15");
            }
            txt_Format(buf, 0x1bc1, suitor->id, partner->id, &e->end, e->end.hour);
            if (g3->kind == 6 || g3->kind == 7)
                he_ShowNews(g3->netId, g3->netId, buf, 0, 0x5f5, "_NACHRICHTEN_HS_11");
            if (g4->kind == 6 || g4->kind == 7)
                he_ShowNews(g4->netId, g4->netId, buf, 0, 0x5f5, "_NACHRICHTEN_HS_11");
            txt_Format(buf, 0x1bc2, suitor->id, partner->id, &e->end, e->end.hour);
            if (g2->kind == 6 || g2->kind == 7)
                he_ShowNews(g2->netId, g2->netId, buf, 0, 0x5f5, "_NACHRICHTEN_HS_12");
            if (g5 != 0 && (g5->kind == 6 || g5->kind == 7))
                he_ShowNews(g5->netId, g5->netId, buf, 0, 0x5f5, "_NACHRICHTEN_HS_12");
            cm_SendExSetHE(1, r);
            return;
        }
        if (ok == 2) {
            if (suitor->kind == 6 || partner->kind == 7) {
                txt_Format(buf, 0x1bbe, partner->id);
                he_ShowNews(suitor->netId, suitor->netId, buf, 0, 0x5f5, 0);
            }
            e->end = ai_Time;
            e->end.min += 2;
            cm_SendExSetHE(-1, r);
        }
        return;
    case 1:
        memset(&m, 0, sizeof(m));
        m.offerKind = o->kind;
        m.eventId = e->id;
        m.members[0] = o->who[0];
        m.members[1] = o->who[1];
        m.members[2] = o->who[2];
        m.members[3] = o->who[3];
        m.members[4] = o->who[4];
        m.members[5] = o->who[5];
        m.used = (o->who[5] != -1) + 5;
        m.kind = 2;
        m.flags = 0xa2;
        m.thing = -1;
        m.host = ai_CharsT[ai_PlayerId].netId;
        m.time = ai_Time;
        gm_AddTime(&m.time, 0, 0x2d, 0);
        cm_SendNewParty(&m);
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, r);
        return;
    }
}

int gm_FindById();
int ai_EvalFest();

/* 0x5437c0, the run handler of the feast event: the host is the event's character, the guests and the menu /
   musician picks come from the feast table object (gm_Query kind 0x17) of the building e->from, or from the event's
   own words when it names no building; a building without a feast table only re-posts the event. Stage 0 has the
   feast evaluated (ai_EvalFest, sending its effects) and, when that returns 0 for a player host (kind 6 / 7), tells
   the host the news 0x5fd: the text 0x1787 with the cost, then one line 0x1788 per guest (0x178f + 3 * the guest's
   verdict + a random 0 .. 2); it re-posts the event, as stage 1 does. The original clears only the group's first
   4 bytes, and without a building it tests the guests of a null table (*(int *)0x1c ..) before it looks up the
   event's own guests: both kept
   @deep reconstructed and read in full against the original (feast event: AIGRP filled from the feast
   table object kind 0x17 or the event's festGuests, MEM_ZERO inline clear of 4 bytes, switch on stage);
   same instruction count 261/261, 14 differing. Left: +0x100 the table branch stores essen ([ebp-0x1c])
   before loading festMusik (mov cl,[esi+0x2d]) where the original loads both first (order); +0x1df the
   txt_Format(line, 0x1788, id, ...) argument setup: ours mov ecx,[ebp+ebx*8-0x3c]; xor edx,edx; mov
   dx,[ecx]; lea eax,line vs the original xor ecx,ecx; mov eax,[ebp+ebx*8-0x3c]; lea edx,line; mov cx,[eax]
   (registers eax/ecx/edx permuted, same pushes); tried the arguments' term order (no change) */
void he_RunFeast(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *bld = 0;
    AIOBJ *table = 0;
    AICHAR *c;
    int k, n;
    AIGRP g;
    char text[0x800];
    char line[0xec];
    G28REC party;

    if (e->stage == -2 || e->stage == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->opts & 4)
        return;
    gm_FindById(&bld, 0, 0, e->from);
    if (bld != 0 && (table = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0x17)) == 0) {
        e->end = ai_Time;
        cm_SendExSetHE(-1, ev);
        return;
    }
    switch (e->stage) {
    case 0:
        MEM_ZERO(&g, 4);
        n = 0;
        g.leader = (struct AIC4 *)&ai_CharsT[e->chr];
        if (table != 0) {
            g.essen = table->festEssen;
            g.musik = table->festMusik;
            for (k = 0; k < 4; k++) {
                if (table->festGuests[k] != -1 && (c = (AICHAR *)gm_FindChar(table->festGuests[k])) != 0)
                    g.mem[n++].ch = (struct AIC4 *)c;
            }
        } else {
            g.essen = e->festEssen;
            g.musik = e->festMusik;
            for (k = 0; k < 4; k++) {
                if (table->festGuests[k] != -1 && (c = (AICHAR *)gm_FindChar(e->festGuests[k])) != 0)
                    g.mem[n++].ch = (struct AIC4 *)c;
            }
        }
        g.n = (unsigned char)n;
        if (ai_EvalFest(&g, 1) == 0
            && (((AICHAR *)g.leader)->kind == 6 || ((AICHAR *)g.leader)->kind == 7)) {
            txt_Format(text, 0x1787, g.cost);
            for (k = 0; k < g.n; k++) {
                txt_Format(line, 0x1788, ((AICHAR *)g.mem[k].ch)->id, util_RandomU(3) + 0x178f + g.mem[k].d * 3);
                strcat(text, line);
            }
            he_ShowNews(((AICHAR *)g.leader)->netId, -1, text, 0x20, 0x5fd, ch_EmptyName);
        }
        e->end = ai_Time;
        cm_SendExSetHE(-1, ev);
        return;
    case 1:
        e->end = ai_Time;
        cm_SendExSetHE(-1, ev);
        return;
    }
}

int ai_MasterExamPassed();
/* 0x543ac0, a Ghidra miss (disassembled): the run handler of the master exam at a feast (kind 0x36). The examiner is
   the owner of the building e->from, the guests read from its feast table object (gm_Query kind 0x17), or (examAt 1)
   the host of the feast event examFeast (kind 0x37) with that event's guests; the candidate is the event's character,
   and one who is no player (kind 6 / 7) skips to stage 2. Stage 0 opens the window
   "Berufe\Messagebox_Meisterpruefung" (news 0x5ec) with the text 0x1770 / 0x1771 and the voice "_NACHRICHTEN_HS_28",
   due in a day. Stage 1 waits for the answer in that window (d2_ClickedId 0: passed, 1: failed) or for the due time
   (then stage 2); stage 2 decides by ai_MasterExamPassed. Passed: the examiner hears 0x1785; failed: the candidate
   leaves the feast's guests (the table's field, or the feast event re-posted) and the examiner hears 0x1786; either
   only while the candidate is a guest. The text 0x1771 reads the owner of a null building: kept as the original
   @deep reconstructed and read in full against the original (all 4 search loops, the field delta / feast
   re-post on failure, the 0x1785 / 0x1786 news, the stage 0 window and texts, the 0x200 buffer). The
   calls, stores and branches match per path; what differs is tail merging: the original reloads ev from
   its argument slot (mov edx/esi, [esp+0x220]) before the closing he_FreeEvent / he_ReleaseSlot of the
   stage 1 / 2 paths, so their tails are identical and cross-jumped (stage 1 not-found exits jmp +0x479,
   the news calls share +0x471 / +0x295), where ours keeps ev in esi on the passed paths and duplicates
   each return block (returns 13/9, txt_Format x4 vs x3, he_ShowNews x4 vs x2). Hunks: code at +0xf8 (mov
   ecx,eax: master kept in a register), +0x172/+0x19b/+0x386/+0x3b5 (duplicated return blocks), +0x1ba (the
   0x1785 news not cross-jumped into the 0x1786 one), +0x404 (the stage 1 news tail), +0x53e/+0x578 (the
   ai_Time copies into start / end: regs + order) (size: 539 vs 485 instructions, explained: 54
   instructions: four duplicated return blocks (push ev; call he_FreeEvent [; he_ReleaseSlot]; four pops;
   add esp; ret, ~8-10 each) and two news sequences (txt_Format + he_ShowNews, ~16 each) that the original
   cross-jumps because it reloads ev from the argument slot on those paths: allocation, not statements) */
void he_RunFeastExam(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *bld = 0;
    AIOBJ *table = 0;
    AIJOB *feast = 0;
    AICHAR *master, *cand;
    int i, none;
    char buf[0x200];

    if (e->stage == -1 || e->stage == -2)
        goto fail;
    if (e->examAt == 0) {
        gm_FindById(&bld, 0, 0, e->from);
        if (bld == 0)
            goto fail;
        table = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0x17);
        if (table == 0 || bld->owner == 0xffff)
            goto fail;
        master = &ai_CharsT[bld->owner];
        if (master == 0)
            goto fail;
    } else if (e->examAt == 1) {
        feast = (AIJOB *)he_FindEvent(2, 1, e->examFeast, 0, 0x37);
        if (feast == 0)
            goto fail;
        master = &ai_CharsT[feast->chr];
        if (master == 0)
            goto fail;
    } else {
        goto done;
    }
    cand = &ai_CharsT[e->chr];
    if (cand->kind != 6 && cand->kind != 7)
        e->stage = 2;
    switch (e->stage) {
    case 0:
        he_OpenEventSlot(ev, 0, 0, "Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        if (e->slot == 0)
            goto done;
        d2_SetForm(e->slot->win, 0);
        if (e->examAt == 0)
            io_PrintText(0x1770, bld->owner, cand->id, e->examText, bld, master->id, cand->id);
        else
            io_PrintText(0x1771, bld->owner, cand->id, e->examText, master->id, cand->id);
        e->start = ai_Time;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0x18, 0, 0);
        snd_PlayVoice(-7, 0, "_NACHRICHTEN_HS_28", 0);
        e->stage++;
        return;
    case 1:
        if (gm_CompareTime(&ai_Time, &e->end) > 0) {
            he_ReleaseSlot(ev);
            e->stage++;
            return;
        }
        if (d2_ClickedForm != e->slot->win)
            return;
        none = -1;
        if (d2_ClickedId == -1)
            return;
        if (d2_ClickedId == 0) {
            if (e->examAt == 0) {
                for (i = 0; i < 4; i++)
                    if (table->festGuests[i] == cand->netId)
                        break;
            } else {
                for (i = 0; i < 4; i++)
                    if (feast->festGuests[i] == cand->netId)
                        break;
            }
            if (i < 4) {
                txt_Format(buf, 0x1785, cand->id);
                he_ShowNews(master->netId, -1, buf, 0, 0x5ec, 0);
            }
        } else if (d2_ClickedId == 1) {
            if (e->examAt == 0) {
                for (i = 0; i < 4; i++)
                    if (table->festGuests[i] == cand->netId) {
                        cm_BeginFieldBlock((char *)table, table->objId);
                        cm_AddFieldDelta(4, 1, 0x1c + i * 4, &none);    /* table->festGuests[i] = -1 */
                        cm_SendFieldDeltas();
                        break;
                    }
            } else {
                for (i = 0; i < 4; i++)
                    if (feast->festGuests[i] == cand->netId) {
                        feast->festGuests[i] = -1;
                        cm_SendExSetHE(0, ev);
                        break;
                    }
            }
            if (i < 4) {
                txt_Format(buf, 0x1786, cand->id);
                he_ShowNews(master->netId, -1, buf, 0, 0x5ec, 0);
            }
        }
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    case 2:
        if (ai_MasterExamPassed(master, cand) == 1) {
            if (e->examAt == 0) {
                for (i = 0; i < 4; i++)
                    if (table->festGuests[i] == cand->netId)
                        break;
            } else {
                for (i = 0; i < 4; i++)
                    if (feast->festGuests[i] == cand->netId)
                        break;
            }
            if (i < 4) {
                txt_Format(buf, 0x1785, cand->id);
                he_ShowNews(master->netId, -1, buf, 0, 0x5ec, 0);
            }
        } else {
            none = -1;
            if (e->examAt == 0) {
                for (i = 0; i < 4; i++)
                    if (table->festGuests[i] == cand->netId) {
                        cm_BeginFieldBlock((char *)table, table->objId);
                        cm_AddFieldDelta(4, 1, 0x1c + i * 4, &none);    /* table->festGuests[i] = -1 */
                        cm_SendFieldDeltas();
                        break;
                    }
            } else {
                for (i = 0; i < 4; i++)
                    if (feast->festGuests[i] == cand->netId) {
                        feast->festGuests[i] = -1;
                        cm_SendExSetHE(0, ev);
                        break;
                    }
            }
            if (i < 4) {
                txt_Format(buf, 0x1786, cand->id);
                he_ShowNews(master->netId, -1, buf, 0, 0x5ec, 0);
            }
        }
        he_FreeEvent((int *)ev);
        return;
    }
    return;
fail:
    if (e->slot != 0)
        he_ReleaseSlot(ev);
done:
    he_FreeEvent((int *)ev);
}
/* 0x5455e0, a Ghidra miss (disassembled): sets the due clock (+0x52) to now with a random hour (+0x56 = 10 + random(10)) and minute (+0x58 = random(0x3b)), +0x5c = 0 */

void he_Unk5455e0(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(short *)(ev + 0x56) = (short)(util_Random(10) + 10);
    *(int *)(ev + 0x58) = (unsigned short)util_Random(0x3b);
    *(int *)(ev + 0x5c) = 0;
}

int ai_RunMessage();

/* 0x545640, runs the message kept at +0xac (unless the state is -2) and frees the event record */
void he_Unk545640(int ev)
{
    if (*(int *)(ev + 0x70) != -2)
        ai_RunMessage(ev + 0xac);
    he_FreeEvent((int *)ev);
}

void gm_Shuffled(unsigned char n, int *out);

/* 0x545670, a Ghidra miss (disassembled): the alloc handler of the patrol event 0x43, once it is started (opts bit
   4 clear): the start clock is now; of the buildings of kind 6 (gm_BuildQuery 1, 6) up to 8 with a scene object,
   other than the target and the start and not of type 0x47, within 950 of the target's object are collected, and
   up to four of them in a random order (gm_Shuffled) become its way (patrolWay, else -1); prints the
   "STREIFE_INIT" line (not shown), its first step is due in a second, and it is posted with key 0 */
void he_AllocPatrol(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *target, *b;
    D3OBJECT *p, *q;
    int cand[8];
    int pick[8];
    char buf[0x100];
    int n = 0;
    int k;
    float dx, dy, dz, dist;

    if (e->opts & 4)
        return;
    e->start = ai_Time;
    memset(cand, -1, sizeof(cand));
    target = (AIBLD *)gm_BuildQuery(1, 1, e->patrolTarget);
    for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
        if (n >= 8)
            break;
        p = (D3OBJECT *)b->object;
        if (p != 0 && b->id != target->id && b->id != e->patrolStart && b->type != 0x47) {
            q = (D3OBJECT *)target->object;
            dx = p->pos.x - q->pos.x;
            dy = p->pos.y - q->pos.y;
            dz = p->pos.z - q->pos.z;
            dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
            if (dist <= 950.0f)
                cand[n++] = b->id;
        }
    }
    if (n != 0) {
        memset(e->patrolWay, -1, sizeof(e->patrolWay));
        gm_Shuffled((unsigned char)n, pick);
        for (k = 0; k < n && k < 4; k++)
            e->patrolWay[k] = cand[pick[k]];
    }
    sprintf(buf, "STREIFE_INIT: Target: %i, Way: %i, %i, %i, %i, Start: %i", e->patrolTarget, e->patrolWay[0],
            e->patrolWay[1], e->patrolWay[2], e->patrolWay[3], e->patrolStart);
    e->end = ai_Time;
    gm_AddTime(&e->end, 0, 0, 1);
    e->patrolStep = 0;
    e->patrolRound = 0;
    e->patrolWait = -1;
    e->exSlot = cm_SendExSetHE(0, ev);
}

int gm_MinutesBetween();
int gm_PlayerFactor(int a, unsigned char k);
void cm_SendChrStop(int a);
void cm_SendChrAnim(int a, const char *s, unsigned char c);
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);
unsigned char *gm_BuildQuery(int n, ...);
char *gm_Query(void *list, int n, ...);

/* 0x545840, a Ghidra miss (disassembled): the run handler of the patrol event 0x43 (he_AllocPatrol); each stage
   posts the next one with cm_SendExSetHE, due after gm_AddTime (hours, minutes, seconds). While none of the four
   patrolGuards is found the event is put off a minute and ends (-1). Stage -2 runs stage 1 once and frees the event;
   a set patrolWait enters stage 0x400. Stage 0 sends the guards with a scene character to the target building
   ("Streife"; "GOTO_TARGET: %i", not shown; the others are dropped) -> 2; stage 2 / 0x400 wait while a guard's
   scene character has actions queued (0x400 looks every 4 minutes and clears patrolWait) -> 3; stage 3 sends them
   to the next way point (patrolStep) with a random gesture ("gestik/..." once or twice; "GOTO_NEXT: %i") -> 2, past
   the last one counts patrolRound and starts at the target again -> 0, and 8 hours (0x1e0 minutes) after the start
   turns home -> 1; stage 1 sends them to the start building's object 0x146 and pays the event's character for
   each guard 3 .. 6 rolls of random(v * (v + 1)) + 2v, v = sk82 / 42 + 1 (gm_PlayerFactor of the sum: the news
   0x1a6a and cm_SendMoveGoods; "GOTO_START: %i") -> -1, the end; a building not found ends it as well */
void he_RunPatrol(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *b;
    AIOBJ *o;
    AICHAR *c;
    char line[0x40];
    char text[0x200];
    int i, k, v, pay;
    int gone = 0, leaving = 0, paid, sum;

    if (e->stage == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->opts & 4) {
        if (e->stage == -2)
            he_FreeEvent((int *)ev);
        return;
    }
    for (i = 0; i < 4; i++) {
        if (gm_FindChar(e->patrolGuards[i]) != 0)
            break;
        gone++;
    }
    if (gone == 4) {
        gm_AddTime(&e->end, 0, 1, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    if (e->stage == -2) {
        leaving = 1;
        e->stage = 1;
    }
    if (e->patrolWait != -1)
        e->stage = 0x400;
    switch (e->stage) {
    case 0:
        b = (AIBLD *)gm_BuildQuery(1, 1, e->patrolTarget);
        if (b == 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        for (i = 0; i < 4; i++) {
            if (e->patrolGuards[i] == -1)
                continue;
            c = (AICHAR *)gm_FindChar(e->patrolGuards[i]);
            if (c != 0 && c->scene != 0) {
                sv_MoveToGroup(0, 0, c->id, ev);
                cm_SendChrStop(e->patrolGuards[i]);
                cm_SendChrGotoBuilding(e->patrolGuards[i], b->id, -1, 0, 1, "Streife");
            } else {
                e->patrolGuards[i] = -1;
            }
        }
        sprintf(line, "GOTO_TARGET: %i", b->id);
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 1:
        b = (AIBLD *)gm_BuildQuery(1, 1, e->patrolStart);
        if (b == 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        o = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, 0x146);
        paid = 0;
        sum = 0;
        for (i = 0; i < 4; i++) {
            if (e->patrolGuards[i] == -1)
                continue;
            c = (AICHAR *)gm_FindChar(e->patrolGuards[i]);
            if (c != 0 && c->scene != 0) {
                v = (int)((float)c->sk82 * 0.023809524f + ai_FloatOne);
                for (k = (unsigned short)util_Random(4) + 3; k > 0; k--)
                    sum += (unsigned short)util_Random((v + 1) * v) + v * 2;
                paid++;
                cm_SendChrStop(e->patrolGuards[i]);
                cm_SendChrGotoBuilding(e->patrolGuards[i], b->id, o->objId, 0, 0, "Streife");
                sv_MoveToGroup(0, 0, c->id, ev);
            } else {
                e->patrolGuards[i] = -1;
            }
        }
        if (leaving) {
            he_FreeEvent((int *)ev);
            return;
        }
        if (paid != 0) {
            pay = gm_PlayerFactor(sum, ai_OwnPlayer);
            txt_Format(text, 0x1a6a, pay);
            he_MessageBoxQuickjump(ai_CharInts[e->chr][0], -1, text, 0, 0x5ec, b->id, -1, 0, (char *)0);
            cm_SendMoveGoods(ai_CharInts[e->chr][0], -1, ai_OwnPlayer, pay);
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        sprintf(line, "GOTO_START: %i", b->id);
        return;
    case 2:
        for (i = 0; i < 4; i++) {
            if (e->patrolGuards[i] != -1 && (c = (AICHAR *)gm_FindChar(e->patrolGuards[i])) != 0 && c->scene != 0
                && c->scene->actions != 0)
                return;
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(3, ev);
        return;
    case 0x400:
        for (i = 0; i < 4; i++) {
            if (e->patrolGuards[i] != -1 && (c = (AICHAR *)gm_FindChar(e->patrolGuards[i])) != 0 && c->scene != 0
                && c->scene->actions != 0) {
                gm_AddTime(&e->end, 0, 4, 0);
                e->exSlot = cm_SendExSetHE(0x400, ev);
                return;
            }
        }
        e->end = ai_Time;
        e->patrolWait = -1;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(3, ev);
        return;
    case 3:
        if (e->patrolStep <= 3 && e->patrolWay[e->patrolStep] != -1) {
            if (gm_MinutesBetween(&e->start, &ai_Time) > 0x1e0) {
                e->end = ai_Time;
                gm_AddTime(&e->end, 0, 2, 0);
                e->exSlot = cm_SendExSetHE(1, ev);
                return;
            }
            b = (AIBLD *)gm_BuildQuery(1, 1, e->patrolWay[e->patrolStep]);
            e->patrolStep++;
            if (b == 0) {
                e->end = ai_Time;
                gm_AddTime(&e->end, 0, 2, 0);
                e->exSlot = cm_SendExSetHE(1, ev);
                return;
            }
            for (i = 0; i < 4; i++) {
                if (e->patrolGuards[i] == -1)
                    continue;
                c = (AICHAR *)gm_FindChar(e->patrolGuards[i]);
                if (c != 0 && c->scene != 0) {
                    char gestures[3][0x20] = { "gestik/kopf_kratzen", "gestik/durch_haar_fahr", "gestik/kinn_kratzen" };

                    sv_MoveToGroup(0, 0, c->id, ev);
                    cm_SendChrStop(e->patrolGuards[i]);
                    cm_SendChrGotoBuilding(e->patrolGuards[i], b->id, -1, 0, 1, "Streife");
                    k = (unsigned short)util_Random(2) + 1;
                    cm_SendChrAnim(e->patrolGuards[i], gestures[(unsigned short)util_Random(3)], k);
                } else {
                    e->patrolGuards[i] = -1;
                }
            }
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(2, ev);
            sprintf(line, "GOTO_NEXT: %i", b->id);
            return;
        }
        if (gm_MinutesBetween(&e->start, &ai_Time) > 0x1e0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 2, 0);
            e->exSlot = cm_SendExSetHE(1, ev);
            return;
        }
        e->patrolStep = 0;
        e->patrolRound++;
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(0, ev);
        return;
    }
}

int gv_Accuse();
int gv_HeldCrimeIdsOf(int culprit, int *out, unsigned short t);
void gm_Shuffled(unsigned char n, int *out);
void cm_SendDropEvidence(int a, int b, int c);
int ai_CountMissionEvent();
void cm_SendMoneyDelta(int a, int b);

/* 0x546100, a Ghidra miss (disassembled): the start handler of the event of a figure pair (+0xac): when the target (0x501190 kind 1) is gone the event ends, else it is posted again at once (ROUGH) */
void he_Unk546100(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
        if (gm_BuildQuery(1, 1, *(int *)(ev + 0xac)) == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 0, 1);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}

int cm_LootStock();
int cm_FightRole();
int cm_PartyById();
int cm_GetSlotValue(int h);
void *gm_GetGebaeudeHauptraum(unsigned char *b);
unsigned short *gm_QueryNext(void);
int gm_BuildingSkill();

#define HE_REC(id) ((unsigned)&ai_Chars + (id) * 0x21c)

/* 0x5461c0, a Ghidra miss (disassembled): the run handler of the raid event 0x44 ("Razzia"; the raiders are ids[],
   the buildings raidTarget / raidStart). It waits for the event's cm send slot (exSlot); stage -2 (left) runs
   stage 1 once and frees the event. Stage 0 sends the raiders to the target ("Razzia"; the others are dropped) -> 2,
   while the target has an owner, else the event ends (-1, as when a building is not found); stage 2 waits until no
   raider's scene character has actions queued -> 3; stage 3 rolls random(0x4b) + 100 * the start building's spy
   report on the target (its object 0xca: (100 + reportKnow) / 100, 5 with gm_AllowAll) against 125 * (1 + 0.01 *
   gm_BuildingSkill(target, 5, 1)): above it a fight party of kind 3 (G28REC.raid: the target's living
   people against the raiders, half an hour on) is posted -> 4, but with no defender or no raider the target
   is looted (cm_LootStock, the news 0x1c54 / 0x1c55 to the players among owner and raider) -> 1; not above it
   the raid fails (the news 0x114b with the start building's fight role, 0x1c56 to a player owner) -> -1;
   stage 4 waits for the party's slot and keeps its id -> 5, stage 5 until the party is over -> 1; stage 1
   (only with opts 2) sends the living raiders back to the start building's object 0x146 -> -1; stage -1
   sends them to its main room and frees the event. The opts tests of the stages 0 and 2 are
   @original `(opts == 0) & 2`, never true.
   @deep Read whole against the original and fixed: stage -1 last in the switch (table equal), text[0x800]
   (frame 0x924 equal), the party lists initialised -1 (G28REC.raid), the defenders scan as an index loop
   over ai_Chars (signed jl, health && bld == the target), the start building queried into its own temp
   before gm_Query 0xca, the spy-report search as for/break + if, the failure branch's kind tests re-read
   ai_Chars[b->owner]; 826 vs 825 instructions. Left: scheduling and allocation only: the entry (ours mov
   edi,1 before the 1.0f store, the original stores 1.0f first and keeps 1 in ebp: edi->ebp throughout),
   the -1 of raid100 in ebp (ours) vs eax (+0x546704), the party.time copy loads in another register order
   (+0x546775), the cm_LootStock / he_MessageBoxQuickjump / he_ShowNews argument address computations
   interleaved with the pushes in another order (+0x546813..+0x54691c: the same lea chains,
   ai_Chars[b->owner] pushed at once in the original); the prover stops at the switch jump (references: M8
   at +0x5af: ours refers to ai_Time+0x4 (0x770ae4), the original's field to 0x770ae0 (ai_Time) (and 1
   more), explained: the two ref: M8 lines (+0x5b5 / +0x5ba) are the party.time copy of ai_Time loaded in
   another register order (ours eax=[+4], edx=[+0], ecx=[+8]; the original ecx=[+4], eax=[+0], edx=[+8]):
   the alignment pairs different loads, the same four words are copied to party.time) */
void he_RunRaid(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *b, *home;
    AICHAR *c, *raider;
    AIOBJ *o;
    G28REC party;
    char text[0x800];
    int i, n, k, loot, role;
    int leaving = 0, idle = 1;
    float chance = 1.0f, defence;

    if (e->stage == -2) {
        if ((e->opts & 2) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        leaving = 1;
        e->stage = 1;
    }
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    e->exSlot = -1;
    switch (e->stage) {
    case 0:
        if ((e->opts == 0) & 2)
            return;
        b = (AIBLD *)gm_BuildQuery(1, 1, e->raidTarget);
        if (b == 0 || b->owner == 0xffff) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        for (i = 0; i < 8; i++) {
            if (e->ids[i] == -1)
                continue;
            c = (AICHAR *)gm_FindChar(e->ids[i]);
            if (c != 0) {
                sv_MoveToGroup((unsigned char *)b, 0, c->id, (char *)ev);
                cm_SendChrStop(e->ids[i]);
                cm_SendChrGotoBuilding(e->ids[i], b->id, -1, 0, 0, "Razzia");
            } else {
                e->ids[i] = -1;
            }
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 1:
        if ((e->opts & 2) == 0)
            return;
        home = (AIBLD *)gm_BuildQuery(1, 1, e->raidStart);
        if (home == 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 1, 0);
            e->exSlot = cm_SendExSetHE(-1, ev);
            return;
        }
        o = (AIOBJ *)gm_Query((void *)home->objList, 1, 0, 0x146);
        for (i = 0; i < 8; i++) {
            if (e->ids[i] == -1)
                continue;
            c = (AICHAR *)gm_FindChar(e->ids[i]);
            if (c != 0 && c->health > 0) {
                cm_SendChrStop(e->ids[i]);
                cm_SendChrGotoBuilding(e->ids[i], home->id, o->objId, 0, 0, "Razzia");
                sv_MoveToGroup((unsigned char *)home, 0, c->id, (char *)ev);
            } else {
                e->ids[i] = -1;
            }
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        if (leaving) {
            he_FreeEvent((int *)ev);
            return;
        }
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    case 2:
        if ((e->opts == 0) & 2)
            return;
        for (i = 0; i < 8; i++) {
            if (e->ids[i] != -1 && (c = (AICHAR *)gm_FindChar(e->ids[i])) != 0 && c->scene->actions != 0) {
                idle = 0;
                break;
            }
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        if (idle)
            e->exSlot = cm_SendExSetHE(3, ev);
        else
            e->exSlot = cm_SendExSetHE(2, ev);
        return;
    case 3:
        if ((e->opts & 2) == 0)
            return;
        b = (AIBLD *)gm_BuildQuery(1, 1, e->raidTarget);
        c = &ai_CharsT[b->owner];
        raider = &ai_CharsT[e->chr];
        defence = (float)(gm_BuildingSkill(b, 5, 1) * 0.01 + 1.0);
        home = (AIBLD *)gm_BuildQuery(1, 1, e->raidStart);
        o = (AIOBJ *)gm_Query((void *)home->objList, 1, 0, 0xca);
        for (; o != 0; o = (AIOBJ *)gm_QueryNext())
            if (o->reportBld == b->id)
                break;
        if (o != 0)
            chance = (float)((o->reportKnow + 100.0) * 0.01);
        if (gm_AllowAll != 0)
            chance = 5.0f;
        if ((float)(unsigned short)util_Random(0x4b) + chance * 100.0f > defence * 125.0f) {
            memset(&party, 0, sizeof(party));
            party.kind = 3;
            party.flags = 0xa2;
            party.thing = b->id;
            party.host = ai_CharInts[ai_PlayerId][0];
            party.used = 2;
            party.variant = 2;
            party.members[0] = raider->netId;
            party.members[1] = c->netId;
            for (i = 0; i < 16; i++) {
                party.raid.raiders[i] = -1;
                party.raid.defenders[i] = -1;
            }
            n = 0;
            for (i = 0; i < 768 && n < 16 && n < 6; i++) {
                if (ai_CharsT[i].health != 0 && ai_CharsT[i].bld == b) {
                    party.raid.defenders[n] = ai_CharsT[i].netId;
                    n++;
                }
            }
            party.raid.raid100[0] = -1;
            party.raid.raid100[1] = -1;
            party.raid.raid100[2] = -1;
            party.raid.raidHome = ((AIBLD *)gm_BuildQuery(1, 1, e->raidStart))->id;
            k = 0;
            for (i = 0; i < 8 && k < 6; i++) {
                if (e->ids[i] != -1) {
                    party.raid.raiders[k] = e->ids[i];
                    k++;
                }
            }
            if (k != 0 && n != 0) {
                party.time = ai_Time;
                gm_AddTime(&party.time, 0, 0x1e, 0);
                e->raidSlot = cm_SendNewParty(&party);
                e->end = ai_Time;
                gm_AddTime(&e->end, 0, 0, 1);
                e->stage = 4;
                return;
            }
        } else {
            role = cm_FightRole(gm_BuildQuery(1, 1, e->raidStart)) + 0x1125;
            txt_Format(text, 0x114b, role, b->owner, b, role);
            he_ShowNews(ai_CharInts[e->chr][0], -1, text, 0, 0x5f0, (char *)0);
            if (ai_CharsT[b->owner].kind == 6 || ai_CharsT[b->owner].kind == 7) {
                txt_Format(text, 0x1c56, b);
                he_MessageBoxQuickjump(ai_CharInts[b->owner][0], -1, text, 0, 0x5f0, b->id, -1, 0, (char *)0);
            }
            e->stage = -1;
            return;
        }
        home = (AIBLD *)gm_BuildQuery(1, 1, e->raidStart);
        b = (AIBLD *)gm_BuildQuery(1, 1, e->raidTarget);
        loot = cm_LootStock(b, home, &ai_CharsT[e->chr], &ai_CharsT[b->owner]);
        if (ai_CharsT[b->owner].kind == 6 || ai_CharsT[b->owner].kind == 7) {
            txt_Format(text, 0x1c54, b, loot);
            he_MessageBoxQuickjump(ai_CharInts[b->owner][0], -1, text, 0, 0x5f0, b->id, -1, 0, (char *)0);
        }
        if (ai_CharsT[e->chr].kind == 6 || ai_CharsT[e->chr].kind == 7) {
            txt_Format(text, 0x1c55, b->owner, b, loot);
            he_ShowNews(ai_CharInts[e->chr][0], -1, text, 0, 0x5f0, (char *)0);
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 0, 1);
        e->stage = 1;
        return;
    case 4:
        if (cm_GetSlotState(e->raidSlot) == 0)
            return;
        e->raidParty = ((G28REC *)cm_GetSlotValue(e->raidSlot))->id;    /* the slot's value: the party record made */
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 1, 0);
        e->stage = 5;
        return;
    case 5:
        if (cm_PartyById(e->raidParty) != 0) {
            e->end = ai_Time;
            gm_AddTime(&e->end, 0, 5, 0);
            return;
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 0, 1);
        e->stage = 1;
        return;
    case -1:
        if (e->opts & 2) {
            home = (AIBLD *)gm_BuildQuery(1, 1, e->raidStart);
            o = (AIOBJ *)gm_GetGebaeudeHauptraum((unsigned char *)home);
            for (i = 0; i < 8; i++) {
                if (e->ids[i] != -1 && (c = (AICHAR *)gm_FindChar(e->ids[i])) != 0) {
                    cm_SendChrStop(e->ids[i]);
                    cm_SendChrGotoBuilding(e->ids[i], home->id, o->objId, 0, 0, "Razzia");
                    sv_MoveToGroup((unsigned char *)home, 0, c->id, 0);
                }
            }
        }
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x546c40, a Ghidra miss (disassembled): the alloc handler of the fight event 0x45 between partner1 (a) and
   partner2 (b), its run handler 0x546ef0 the end: the event is due from now, stage 2. Unless it is started (opts 2)
   nothing more; with a or b gone, a already fighting (flags1c8 0x100) or b already fought (0x200) it ends in two
   minutes. With +0xb4 set (id3) it runs a day and a player b hears the news 0x1e18 (code -4); else it runs four
   days, a player b hears 0x1d5b, a is accused of the crime 0x16 against b (at a's home, else b's: gv_Accuse), a's
   relation to b gets 0x34, up to three crimes of b whose evidence a holds are dropped (gv_HeldCrimeIdsOf in a
   random order, cm_SendDropEvidence), a player a counts the mission event 0xb, code -2. Both get their flag and
   cm_SendMoneyDelta(a, code) */
void he_AllocFight(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *a, *b;
    char text[0x100];
    int pick[16];
    int crimes[32];
    int code, place, n, i;
    AIBLD *home;

    e->end = ai_Time;
    e->stage = 2;
    if ((e->opts & 2) == 0)
        return;
    a = (AICHAR *)gm_FindChar(e->partner1);
    b = (AICHAR *)gm_FindChar(e->partner2);
    if (a == 0 || (a->flags1c8 & 0x100) != 0 || b == 0 || (b->flags1c8 & 0x200) != 0) {
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    }
    if (e->id3 != 0) {
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.min = 0;
        if (b->kind == 6 || b->kind == 7) {
            txt_Format(text, 0x1e18, a->id);
            he_ShowNews(b->netId, -1, text, 0, 0x5fb, "_NACHRICHTEN_NEU_04");
        }
        code = -4;
    } else {
        gm_AddTime(&e->end, 0x60, 0, 0);
        e->end.min = 0;
        code = -2;
        if (b->kind == 6 || b->kind == 7) {
            txt_Format(text, 0x1d5b, a->id, a->id);
            he_ShowNews(b->netId, -1, text, 0, 0x5fb, "_NACHRICHTEN_HS_21");
        }
        place = -1;
        home = a->bld2;
        if (home == 0)
            home = b->bld2;
        if (home != 0)
            place = home->id;
        gv_Accuse(0x16, 1, a->netId, b->netId, place);
        cm_SendRelations(a->netId, b->netId, 0x34, 1);
        n = gv_HeldCrimeIdsOf(b->netId, crimes, a->id);
        gm_Shuffled(n < 0x10 ? n : 0x10, pick);
        for (i = 0; i < 3 && n != 0; i++, n--)
            cm_SendDropEvidence(crimes[pick[i]], a->netId, 0);
        if (a->kind == 6)
            ai_CountMissionEvent(a, 0xb);
    }
    cm_SendSetField(a->netId, 0x1c8, 4, 0x100, 0);
    cm_SendSetField(b->netId, 0x1c8, 4, 0x200, 0);
    cm_SendMoneyDelta(a->netId, code);
}
/* 0x546ef0, a Ghidra miss (disassembled): the run handler of the fight event 0x45 (he_AllocFight): at stage 2
   (started, opts 2) both fighters lose their flag (flags1c8 0x100 / 0x200); with both found the outcome id3 tells:
   1 / 2 a player partner2 (b) hears the news 0x1e19 / 0x1e20, else a's relation to b drops by 0x68 and b hears
   0x1d5d, a player a's news 0x1d5c goes to b as well; the event ends in two minutes. Stage -1 / -2 (left) clears
   the flags (when started) and frees the event */
void he_RunFight(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AICHAR *a, *b;
    char text[0x100];

    switch (e->stage) {
    case 2:
        if ((e->opts & 2) == 0)
            return;
        a = (AICHAR *)gm_FindChar(e->partner1);
        if (a != 0)
            cm_SendSetField(a->netId, 0x1c8, 4, 0, 0x100);
        b = (AICHAR *)gm_FindChar(e->partner2);
        if (b != 0)
            cm_SendSetField(b->netId, 0x1c8, 4, 0, 0x200);
        if (a != 0 && b != 0) {
            if (e->id3 == 1) {
                if (b->kind == 6 || b->kind == 7) {
                    txt_Format(text, 0x1e19, a->id);
                    he_ShowNews(b->netId, -1, text, 0, 0x5fb, (char *)0);
                }
            } else if (e->id3 == 2) {
                if (b->kind == 6 || b->kind == 7) {
                    txt_Format(text, 0x1e20, a->id);
                    he_ShowNews(b->netId, -1, text, 0, 0x5fb, (char *)0);
                }
            } else {
                cm_SendRelations(a->netId, b->netId, -0x68, 1);
                if (b->kind == 6 || b->kind == 7) {
                    txt_Format(text, 0x1d5d, a->id);
                    he_ShowNews(b->netId, b->netId, text, 0, 0x5fb, (char *)0);
                }
                if (a->kind == 6 || a->kind == 7) {
                    txt_Format(text, 0x1d5c, b->id);
                    he_ShowNews(b->netId, b->netId, text, 0, 0x5fb, (char *)0);
                }
            }
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 2, 0);
        e->exSlot = cm_SendExSetHE(-1, ev);
        return;
    case -1:
    case -2:
        if (e->opts & 2) {
            a = (AICHAR *)gm_FindChar(e->partner1);
            if (a != 0)
                cm_SendSetField(a->netId, 0x1c8, 4, 0, 0x100);
            b = (AICHAR *)gm_FindChar(e->partner2);
            if (b != 0)
                cm_SendSetField(b->netId, 0x1c8, 4, 0, 0x200);
        }
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x547170, sets the start (+0x44) and the due clock (+0x52) of the event record to now and posts it 2 units later (4fe140) */

void he_Unk547170(int ev)
{
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 2, 0, 0);
}

unsigned int cm_GetSpeedLevel(void);

/* 0x550370, one step of the reaper's flight to the building reaperTo (in scene 0): while the object follows a path
   animation only the clock reaperClock is refreshed; else the target is the point 80 above the ground under
   reaperTo's object: closer than 45 (horizontally) the flight ends (reaperPlaced = 0, returns 2); otherwise the
   object moves along the direction by the time since reaperClock times a speed of 0.5 + 0.25 per cm_SpeedLevel,
   rising (0.978 per time unit, with a tenth of the step) while it is less than 80 above the ground there, and its
   position and angles are cached; 1 = flying, 0 = the object or the building missing
   @deep Read whole against the original and rewritten typed: AIJOB reaper fields, reaperClock unsigned
   (the original zero-extends it to a qword before the fisub), D3OBJECT.path at 0x1d0 (the path animation,
   d3_anim.h D3PATH) for the early clock refresh, d3_Scenes[0].hmap for both d3_GetHeightAt calls, the
   D3VECTOR4 target/at/dir with target reused for the new position, dist < 45.0f (test ah,5/jp), ground +
   80 > target.y (and 0x4100/jne), step = (float)d3_Time - reaperClock then * (cm_GetSpeedLevel()
   (unsigned: fild qword) * 0.25f + 0.5f), the 0.978 double and the 0.1f rise, ret 2 / 1 through ebx. 234
   vs 236 instructions; diffclass: slots 80, frame 78, regs 56, order 44, fpu 26 and code 28 that are all
   allocation: the original zeroes ebp and compares / pushes it (ours test reg,reg / push 0), keeps the
   scene in edi and also spills it to [esp+0x14] (reloaded mov edi,[esp+0x20] before the last
   d3if_SelectScene), loads the height map into edi before the second d3_GetHeightAt (ours mov eax,[X] at
   the push), returns ebx where ours returns edi, and its frame is 0x50 vs ours 0x48 (stack offsets shifted
   by 8). The prover stops on the escaped frame layout (ours frame+0x8 = nothing, the original's = the
   spilled d3_CurrentScene). */
int he_MoveReaper(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int scene = d3_CurrentScene;
    AIBLD *bld;
    D3VECTOR4 target, at, dir;
    float dist, step, ground;
    int x, z, ret;

    d3if_SelectScene(0, 1);
    if (e->reaper != 0 && e->reaper->info != 0 && e->reaper->info->mesh != 0) {
        gm_FindById((int *)&bld, 0, 0, e->reaperTo);
        if (bld != 0 && bld->object != 0 && ((D3OBJECT *)bld->object)->info != 0
            && ((D3OBJECT *)bld->object)->info->mesh != 0) {
            if (e->reaper->path != 0) {
                e->reaperClock = d3_Time;
                d3if_SelectScene(scene, 1);
                return 1;
            }
            d3_PointToWorld((D3OBJECT *)bld->object, &((D3OBJECT *)bld->object)->pos, (D3VECTOR *)&target);
            d3_PointToWorld(e->reaper, &e->reaper->pos, (D3VECTOR *)&at);
            dir.x = target.x - at.x;
            dir.y = 0.0f;
            dir.z = target.z - at.z;
            dist = (float)sqrt(dir.z * dir.z + dir.x * dir.x);
            d3_GetHeightAt((D3SM *)d3_Scenes[0].hmap, (D3VECTOR *)&target, &x, &z, &ground);
            target.y = ground + 80.0f;
            dir.x = target.x - at.x;
            dir.y = target.y - at.y;
            dir.z = target.z - at.z;
            if (dist < 45.0f) {
                e->reaperPlaced = 0;
                ret = 2;
            } else {
                ret = 1;
                step = (float)d3_Time - e->reaperClock;
                step = ((float)cm_GetSpeedLevel() * 0.25f + 0.5f) * step;
                e->reaperClock = d3_Time;
                d3_Normalize(&dir);
                dir.x = step * dir.x;
                dir.y = step * dir.y;
                dir.z = step * dir.z;
                target.x = dir.x + at.x;
                target.y = dir.y + at.y;
                target.z = dir.z + at.z;
                d3_GetHeightAt((D3SM *)d3_Scenes[0].hmap, (D3VECTOR *)&target, &z, &x, &ground);
                if (ground + 80.0f > target.y) {
                    target.y = step * 0.978 + target.y;
                    target.x = step * dir.x * 0.1f + at.x;
                    target.z = step * dir.z * 0.1f + at.z;
                }
                e->reaperRot.x = e->reaper->rotation.x;
                e->reaperRot.y = e->reaper->rotation.y;
                e->reaperRot.z = e->reaper->rotation.z;
                e->reaperPos.x = target.x;
                e->reaperPos.y = target.y;
                e->reaperPos.z = target.z;
                d3_SetObjectPos(e->reaper, (D3VECTOR *)&target);
            }
            d3if_SelectScene(scene, 1);
            return ret;
        }
    }
    d3if_SelectScene(scene, 1);
    return 0;
}
/* 0x5506c0, turns the reaper towards its next target: both the reaper object and the building reaperTo's object
   need a mesh driver; the target is the point above reaperTo's object (its height + 80), the object's angles are
   set from the direction to it (d3_VecAngle2 writes them), cached in reaperRot with the target in reaperPos, and
   reaperPlaced = 1. 1 = done, 0 = an object missing */
int he_AimReaper(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int scene = d3_CurrentScene;
    AIBLD *bld;
    D3VECTOR4 target, at, dir;

    d3if_SelectScene(0, 1);
    if (e->reaper != 0 && e->reaper->info != 0 && e->reaper->info->mesh != 0) {
        gm_FindById((int *)&bld, 0, 0, e->reaperTo);
        if (bld != 0 && bld->object != 0 && ((D3OBJECT *)bld->object)->info != 0
            && ((D3OBJECT *)bld->object)->info->mesh != 0) {
            d3_PointToWorld((D3OBJECT *)bld->object, &((D3OBJECT *)bld->object)->pos, (D3VECTOR *)&target);
            target.y = target.y + ((D3OBJECT *)bld->object)->info->mesh->radius + 80.0f;
            d3_PointToWorld(e->reaper, &e->reaper->pos, (D3VECTOR *)&at);
            dir.x = target.x - at.x;
            dir.y = target.y - at.y;
            dir.z = target.z - at.z;
            d3_VecAngle2((D3VECTOR *)&dir, &e->reaper->rotation);
            e->reaperRot.x = e->reaper->rotation.x;
            e->reaperRot.y = e->reaper->rotation.y;
            e->reaperRot.z = e->reaper->rotation.z;
            e->reaperPos.x = target.x;
            e->reaperPos.y = target.y;
            e->reaperPos.z = target.z;
            e->reaperPlaced = 1;
            d3if_SelectScene(scene, 1);
            return 1;
        }
    }
    d3if_SelectScene(scene, 1);
    return 0;
}
void d3_DirToWorld();
int d3t_PlaceObjectV(struct D3OBJECT *camera, struct D3VECTOR *pos, struct D3VECTOR *dir, int unk, char flags);

/* 0x550850, turns the reaper around towards the building reaperTo once it arrived: both objects need a mesh driver;
   the angle between the object's forward axis (its z axis in the world) and the direction to the point above
   reaperTo's object (its height + 80) is added to its y angle, and d3t_PlaceObjectV moves it to its own position
   with those angles over 0x78 (flags 8). 1 = done, 0 = an object missing */
int he_TurnReaper(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int scene = d3_CurrentScene;
    AIBLD *bld;
    D3VECTOR4 rot, target, at, dir;
    float angle;

    d3if_SelectScene(0, 1);
    if (e->reaper != 0 && e->reaper->info != 0 && e->reaper->info->mesh != 0) {
        gm_FindById((int *)&bld, 0, 0, e->reaperTo);
        if (bld != 0 && bld->object != 0 && ((D3OBJECT *)bld->object)->info != 0
            && ((D3OBJECT *)bld->object)->info->mesh != 0) {
            d3_PointToWorld((D3OBJECT *)bld->object, &((D3OBJECT *)bld->object)->pos, (D3VECTOR *)&target);
            target.y = target.y + ((D3OBJECT *)bld->object)->info->mesh->radius + 80.0f;
            d3_PointToWorld(e->reaper, &e->reaper->pos, (D3VECTOR *)&at);
            dir.x = target.x - at.x;
            dir.y = target.y - at.y;
            dir.z = target.z - at.z;
            d3_DirToWorld(e->reaper, &d3_ZAxis, &rot);
            angle = d3_VecAngle2((D3VECTOR *)&rot, (D3VECTOR *)&dir);
            if (e->reaper == 0) {
                d3if_SelectScene(scene, 1);
                return 0;
            }
            rot.x = e->reaper->rotation.x;
            rot.y = e->reaper->rotation.y;
            rot.z = e->reaper->rotation.z;
            rot.y = rot.y + angle;
            d3t_PlaceObjectV(e->reaper, (D3VECTOR *)&at, (D3VECTOR *)&rot, 0x78, 8);
            d3if_SelectScene(scene, 1);
            return 1;
        }
    }
    d3if_SelectScene(scene, 1);
    return 0;
}
int cm_PlaySpecialTrack();
int cm_EndSpecialTrack();
void ai_PlagueMessageAll(void);

/* 0x5509f0, the allocation handler of the reaper event ("Sensenmann"): due now; from reaperNext it walks the 0x100
   buildings by reaperStep (mod 0x100) for the next one in use whose kind is in the 64-bit mask 0x1807c4110 (as the
   plague event, he_RunPlague), notes where the next walk starts, and flies the reaper to it (reaperTo, read even when
   none was found) by creating its object; nothing found or no object: the record is freed. Posted again 15 minutes
   later */
void he_StartReaper(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    char text[0x200];
    AIBLD *o;
    AIBTYPE *bt;
    unsigned int lo, hi;
    int n, k;

    e->end = ai_Time;
    n = 0x100;
    k = e->reaperNext;
    do {
        o = &ai_BldsT[k];
        hi = 0;
        bt = &ai_BTypesT[o->type];
        lo = 0;
        if ((unsigned char)bt->kind > 0x1f)
            hi = 1 << ((unsigned char)bt->kind - 0x20);
        else
            lo = 1 << (unsigned char)bt->kind;
        if (o->owner != 0xffff && ((lo & 0x807c4110) != 0 || (hi & 1) != 0))
            break;
        o = 0;
        n--;
        k = (k + e->reaperStep) % 0x100;
    } while (n != 0);
    e->reaperNext = (e->reaperStep + k) % 0x100;
    if (o == 0)
        he_FreeEvent((int *)ev);
    else
        txt_Format(text, "EvtSensenmann: next target = %G", o);
    e->reaperTo = o->id;
    if (he_CreateReaper(ev) == 0)
        he_FreeEvent((int *)ev);
    gm_AddTime(&e->end, 0, 0xf, 0);
}
/* 0x550b60, the run handler of the reaper event ("Sensenmann"): freed (-1 / -2) it deletes the reaper object, stops
   its track and frees the record; it (re)creates the object when there is none (failing: freed) and starts the track
   "cd2\DerSeuchenzug.mp3" once (he_ReaperMusic). Stage 0 flies (he_MoveReaper, again in 2 minutes); arrived, the
   target becomes reaperFrom and while reaperLeft > 0 the next building of the kind mask is the new target (none:
   -1), at 0 the reaper turns back to reaperHome; turned (he_TurnReaper) it goes to stage 2 in 15 minutes, else -1.
   Stage 2 aims it (he_AimReaper) and flies on (0), or ends with the plague message to all (-1) */
void he_RunReaper(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    char text[0x200];
    AIBLD *o;
    AIBTYPE *bt;
    unsigned int lo, hi;
    int n, k, r;

    if (e->stage == -2 || e->stage == -1) {
        if (e->reaper != 0)
            d3if_Delete(e->reaper);
        if (he_ReaperMusic != 0)
            cm_EndSpecialTrack();
        he_ReaperMusic = 0;
        he_FreeEvent((int *)ev);
        return;
    }
    if (e->reaper == 0 && he_CreateReaper(ev) == 0) {
        he_FreeEvent((int *)ev);
        if (he_ReaperMusic != 0)
            cm_EndSpecialTrack();
        he_ReaperMusic = 0;
        return;
    }
    if (he_ReaperMusic == 0) {
        cm_PlaySpecialTrack("cd2\\DerSeuchenzug.mp3");
        if (snd_CurrentTrack != 0)
            he_ReaperMusic = 1;
    }
    e->end = ai_Time;
    switch (e->stage) {
    case 0:
        r = he_MoveReaper(ev);
        if (r == 1) {
            e->stage = 0;
            gm_AddTime(&e->end, 0, 2, 0);
            return;
        }
        if (r == 2) {
            e->reaperFrom = e->reaperTo;
            if (e->reaperLeft <= 0) {
                if (e->reaperLeft == 0) {
                    e->reaperTo = e->reaperHome;
                    if (he_TurnReaper(ev) != 0) {
                        gm_AddTime(&e->end, 0, 0xf, 0);
                        e->reaperLeft--;
                        e->stage = 2;
                        return;
                    }
                    gm_AddTime(&e->end, 0, 2, 0);
                }
                e->stage = -1;
                return;
            }
            n = 0x100;
            k = e->reaperNext;
            do {
                o = &ai_BldsT[k];
                lo = 0;
                bt = &ai_BTypesT[o->type];
                hi = 0;
                if ((unsigned char)bt->kind > 0x1f)
                    hi = 1 << ((unsigned char)bt->kind - 0x20);
                else
                    lo = 1 << (unsigned char)bt->kind;
                if (o->owner != 0xffff && ((lo & 0x807c4110) != 0 || (hi & 1) != 0))
                    break;
                o = 0;
                n--;
                k = (k + e->reaperStep) % 0x100;
            } while (n != 0);
            e->reaperNext = (e->reaperStep + k) % 0x100;
            if (o == 0) {
                e->stage = -1;
                return;
            }
            txt_Format(text, "EvtSensenmann: next target = %G", o);
            e->reaperTo = o->id;
            e->reaperLeft--;
            if (he_TurnReaper(ev) != 0) {
                gm_AddTime(&e->end, 0, 0xf, 0);
                e->stage = 2;
            } else {
                gm_AddTime(&e->end, 0, 2, 0);
                e->stage = -1;
            }
            return;
        }
        gm_AddTime(&e->end, 0, 2, 0);
        e->stage = -1;
        return;
    case 2:
        gm_AddTime(&e->end, 0, 2, 0);
        if (he_AimReaper(ev) != 0) {
            e->stage = 0;
        } else {
            ai_PlagueMessageAll();
            e->stage = -1;
        }
    }
}
/* 0x550ea0, sets state 0 and the due clock of an event record to now with the minute word 7 (a clock field, ROUGH) */

void he_Unk550ea0(int ev)
{
    *(int *)(ev + 0x70) = 0;
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(unsigned short *)(ev + 0x56) = 7;
    *(int *)(ev + 0x58) = 0;
}

void gm_RankPlayers();
int gm_ConvertMoney(int a, unsigned char k);
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

/* 0x550ee0, the run handler of the ranking event: in the stage 0, every hour of the day, it ranks the players
   (gm_RankPlayers) and sends the ranking over the network (udp_Send): a head packet with the column titles and the own
   player's name, then a line per ranked player; at 21 o'clock it goes to stage 1, which waits a day (0x18 hours) and
   starts again at 7:00 */
void he_RunRanking(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int cnt = 8;
    GMRANK recs[8];
    HERANKHEAD head;
    HERANKLINE line;
    char tmp[0x100];
    AICHAR *c;
    int i;

    switch (e->stage) {
    case 1:
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.hour = 7;
        e->end.min = 0;
        e->stage = 0;
        return;
    case 0:
        gm_RankPlayers(&cnt, recs);
        head.one = 1;
        head.kind = 3;
        head.size = 0xcf;
        for (i = 0; i < 5; i++) {
            txt_Format(tmp, "%s", 0x16b7 + i);
            strncpy(head.titles[i], tmp, 0x18);
            head.titles[i][0x17] = 0;
        }
        strcpy(head.player, &ai_PlayerSlotsT[ai_OwnPlayer].name[0x20]);
        strcpy(head.worthTitle, "Verm\xf6gen");
        strcpy(head.pointsTitle, "Gewinnpunkte");
        head.count = (unsigned char)cnt;
        udp_Send((char *)&head, head.size);
        for (i = 0; i < cnt; i++) {
            line.one = 1;
            line.kind = 2;
            line.size = 0xb9;
            c = &ai_CharsT[recs[i].id];
            line.netId = c->netId;
            line.worth = gm_ConvertMoney(recs[i].worth, ai_OwnPlayer);
            line.points = recs[i].points;
            line.count = (unsigned char)cnt;
            if (c->kind == 6 || c->kind == 7)
                line.player = 1;
            else
                line.player = 2;
            memcpy(line.skills, c->skills, 5);
            strncpy(line.name, c->name, 0x18);
            line.name[0x17] = 0;
            txt_Format(tmp, "%s", (c->sex != 0 ? 0x18f : 0x130) + c->jobId);
            strncpy(line.job, tmp, 0x20);
            line.job[0x1f] = 0;
            txt_Format(tmp, "%s", (c->sex != 0 ? 0x11e : 0x114) + c->b00d);
            strncpy(line.rank, tmp, 0x20);
            line.job[0x1f] = 0;         /* sic: the original ends job again, not rank */
            txt_Format(tmp, "%s", (c->sex != 0 ? 0x27b : 0x253) + c->officeKind);
            strncpy(line.office, tmp, 0x24);
            line.office[0x23] = 0;
            txt_Format(tmp, "%s", (c->sex != 0 ? 0x27b : 0x253) + c->secondOffice);
            strncpy(line.office2, tmp, 0x24);
            line.office2[0x23] = 0;
            udp_Send((char *)&line, line.size);
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 1, 0, 0);
        if (e->end.hour >= 0x15)
            e->stage++;
    }
}
/* 0x5512c0, the allocation handler of the ranking-like event (ROUGH): state 0; unless flag 4 of +0x78 is set it clears the ten 16-byte slots from +0xac (two ids = -1, four bytes 0), sets the due clock to now with the minute word 0xb and posts the event (0x4c4c60 (0, ev), handle in +0x84) */

void he_Unk5512c0(int ev)
{
    int i;
    unsigned char *s;

    *(int *)(ev + 0x70) = 0;
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        s = (unsigned char *)(ev + 0xac);
        for (i = 0; i < 10; i++) {
            *(int *)s = -1;
            *(int *)(s + 4) = -1;
            s[8] = 0;
            s[9] = 0;
            s[10] = 0;
            s[11] = 0;
            s += 0x10;
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        *(unsigned short *)(ev + 0x56) = 0xb;
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}

void cm_SendChrDelete(int a);
void cm_SendChrStop(int a);
int sv_IsAtDoor();
void cm_SendSetField(int a, int b, int c, int d, int e);

/* 0x551340, steps one slot of the walk event 0x5512c0: a free slot gives 0; the character (gm_FindChar) walks to
   its building (gm_FindById): while it is not at the door yet (sv_IsAtDoor) and the hour is before 22 the result is
   2 (still on its way); else, or when the character or the building is gone, the slot is freed, a character with a
   scene model is stopped (cm_SendChrStop, message 0x57) and its field 0x1c8 loses the bit 0x40000000, result 0 */
int he_StepWalkSlot(HEWALKSLOT *s)
{
    AICHAR *c;
    int bld;

    if (s->chr == -1)
        return 0;
    c = (AICHAR *)gm_FindChar(s->chr);
    if (c != 0) {
        gm_FindById(&bld, 0, 0, s->target);
        if (bld == 0) {
            if (c->model != 0) {
                cm_SendChrDelete(s->chr);
                cm_SendSetField(c->netId, 0x1c8, 4, 0, 0x40000000);
            }
        } else {
            if (sv_IsAtDoor((char *)c, (char *)bld) == 0 && ai_Time.hour < 0x16)
                return 2;
            if (c->model != 0) {
                cm_SendChrStop(c->netId);
                cm_SendChrDelete(s->chr);
            }
            s->chr = -1;
            s->target = -1;
            s->flags[0] = 0;
            s->flags[1] = 0;
            s->flags[2] = 0;
            s->flags[3] = 0;
            cm_SendSetField(c->netId, 0x1c8, 4, 0, 0x40000000);
            return 0;
        }
    }
    s->chr = -1;
    s->target = -1;
    s->flags[0] = 0;
    s->flags[1] = 0;
    s->flags[2] = 0;
    s->flags[3] = 0;
    return 0;
}

/* 0x551440, fills one free 16-byte slot of the 0x5512c0 event with a person and a target (ROUGH, from the disassembly): -1 when the slots are used up (n + the two flags 0x76061c / 0x760618 (+4) >= 10), the slot is in use (byte +9) or it is before the hour 0xb; the chance (n - m) / (n + 1) against 0x5eceec picks the order of the two try values 0x20a0 / 0x402c20 for the person's search 0x4a5b50 (player 0x6f25b8, up to 100.0), a target is the person's own target (+0x170) or comes from 0x4a63f0; a random roll 0x42ec20 against 0.1 may end it (-1); else the target's building number (0x4fea50, 0xa9 bytes each) goes in +0xb, the person is sent there (message 0x1c8) and the slot gets person, target, 1 / 9 (0x29 for a character with +0x1f0), a random byte of 0x5ed140 and 0xff (0x4c52a0 (person, target, -1) starts it) */
int he_Unk551440(unsigned char *slot, int n, int m)
{
    int pair[2];
    int out = -1;
    float x;
    int ch, tgt, o = 0, bld, r;
    int lim = n + (U_I((unsigned)&d3t_Snow) != 0) + (U_I((unsigned)&d3t_Rain) != 0 ? 4 : 0);

    pair[0] = 0x20a0;
    pair[1] = 0x402c20;
    if (lim >= 10 || slot[9] != 0 || *(unsigned short *)(unsigned)&ai_Time.hour < 0xb)
        return -1;
    x = (float)(n - m) / (float)(n + 1);
    if (x >= (0.660000026f)) {
        pair[0] = 0x402c20;
        pair[1] = 0x20a0;
    }
    ch = (unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c;
    if (ai_FindPartnersRandomWalk(ch, 1, pair, 0, 100.0f, &out) == 0 && ai_FindPartnersRandomWalk(ch, 1, pair + 1, 0, 100.0f, &out) == 0)
        return -1;
    ch = (unsigned)&ai_Chars + (unsigned short)out * 0x21c;
    r = *(int *)(ch + 0x170);
    if (r != 0) {
        tgt = *(int *)(r + 1);
    } else {
        if (AI_4A63F0_7(ch, 0, 0xa7f4c3a0, 1, 0, 100.0f, &tgt) == 0)
            return -1;
    }
    if ((float)util_RandomF() < 0.1f * 0.0f)
        return -1;
    gm_FindById(&o, 0, 0, tgt);
    if (o == 0)
        return -1;
    bld = (o - U_I((unsigned)&ai_Blds)) / 0xa9;
    cm_SendSetField(*(int *)(ch + 4), 0x1c8, 4, 0, 0x40000000);
    *(int *)slot = *(int *)(ch + 4);
    *(int *)(slot + 4) = o;
    slot[8] = 1;
    slot[9] = 9;
    if (*(int *)(ch + 0x1f0) != 0)
        slot[9] = 0x29;
    slot[10] = *(unsigned char *)((unsigned)&ai_RandomStrides + (unsigned short)util_Random(0x10) * 4);
    slot[11] = (unsigned char)bld;
    slot[12] = 0xff;
    ai_Unk4c52a0(*(int *)(ch + 4), tgt, -1);
    return 0;
}

void cm_SendChrAnim(int a, const char *s, unsigned char c);

/* 0x5516a0, queues a random animation on a person (+4 the figure; random 0x42ec00(4) & 0xffff, jump table 0x551764): 0 = 0x626fd0, 1 = 0x626fbc then 0x626fd0, 2 = 0x626a4c then 0x626fd0, 3 = 0x6268c8, 0x626fbc, 0x626fd0 (all with 0x4c5410 (id, name, 1)) */
void he_Unk5516a0(int p)
{
    switch ((unsigned short)util_Random(4)) {
    case 0:
        cm_SendChrAnim(*(int *)(p + 4), "gestik/haende_reiben", 1);
        break;
    case 1:
        cm_SendChrAnim(*(int *)(p + 4), "gestik/herranwinken", 1);
        cm_SendChrAnim(*(int *)(p + 4), "gestik/haende_reiben", 1);
        break;
    case 2:
        cm_SendChrAnim(*(int *)(p + 4), "gestik/kinn_kratzen", 1);
        cm_SendChrAnim(*(int *)(p + 4), "gestik/haende_reiben", 1);
        break;
    case 3:
        cm_SendChrAnim(*(int *)(p + 4), "gestik/durch_haar_fahr", 1);
        cm_SendChrAnim(*(int *)(p + 4), "gestik/herranwinken", 1);
        cm_SendChrAnim(*(int *)(p + 4), "gestik/haende_reiben", 1);
        break;
    }
}

unsigned char *gm_BuildQuery(int n, ...);
unsigned char * gm_BuildNext(void);
char *gm_FindBuildingById(int id);
int he_Unk55ee40();
void cm_SendCharHalt(int a);
int cm_SendChrGotoBuilding();

/* 0x551780, steps one 16-byte slot of the 0x5512c0 event (a person on a walk through town; bytes: +8 the stage counter, +9 flags 0x40 / 0x10 / 0x04 (done), +0xa the step, +0xb the building index, +0xc the hour it started): the person (0x505df0) is gone -> the slot is cleared; with a target (0x4fea50) the walk is checked (0x524770) and a person with a work object (+0x184) is held / released by the hour (message 0x1c8 / 0x1000, 0x4c5b90); every 1 + budget / 2 stages (budget = 2 + random 0x42ec00(6) + the two flags) the person plays an animation (0x5516a0); else a new target building is looked for, with a chance above 0.5 from the 0x20 buildings of the list 0x501190 (1, 5, 0x16) through 0x55ee40, else by walking the 0x100 buildings with step +0xa for one of the kinds in the mask 0xa7f4c3a0; the found building number goes to +0xb, its id to +4, and the person is sent there (0x4c52e0 "...", the flag differs for the kinds 0x16 / 7 / 0xf) (ROUGH) */
void he_Unk551780(unsigned char *s)
{
    int v10, o = 0, v18 = 0, a;
    int arr[0x20];
    int n, bld, kind, step, ebx, left;
    unsigned lo, hi;
    char c;
    int obj;

    a = gm_FindChar(*(int *)s);
    if (a == 0) {
        *(int *)s = -1;
        *(int *)(s + 4) = -1;
        s[8] = 0;
        s[9] = 0;
        s[10] = 0;
        s[11] = 0;
        s[12] = 0xff;
        return;
    }
    gm_FindById(&o, 0, 0, *(int *)(s + 4));
    v18 = 0;
    if (o == 0)
        goto cleanup;
    v10 = (U_I((unsigned)&d3t_Snow) != 0) + (U_I((unsigned)&d3t_Rain) != 0 ? 2 : 0) + (unsigned short)util_Random(6) + 2;
    if (sv_IsAtDoor(a, v18) == 0)
        return;
    obj = *(int *)(a + 0x184);
    if (obj != 0 && *(int *)(obj + 0x2c) != -1 && *(int *)(obj + 0x30) != -1) {
        c = (char)s[0xc];
        s[9] |= 0x40;
        v18 = 1;
        if (c == 0xff) {
            s[0xc] = (unsigned char)*(unsigned short *)(unsigned)&ai_Time.hour;
            cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x1000);
            return;
        }
        if ((*(unsigned short *)(unsigned)&ai_Time.hour & 0xffff) <= c + 1)
            return;
        s[9] &= 0xbf;
        cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x1000);
        cm_SendCharHalt(*(int *)(a + 4));
        s[0xc] = 0xff;
    }
    n = s[8];
    if (10 - n < v10) {
        s[9] |= 4;
        return;
    }
    if ((s[9] & 0x10) == 0) {
        step = v10 / 2 + 1;
        if ((n + 2) % step == 0 && v18 == 0) {
            s[8] = (unsigned char)(n + 1);
            he_Unk5516a0(a);
            s[9] |= 0x10;
            return;
        }
    }
cleanup:
    s[9] &= 0xef;
    cm_SendChrStop(*(int *)(a + 4));
    if ((float)util_RandomF() > 0.5f) {
        int e = gm_BuildQuery(1, 5, 0x16);
        int i = 0;

        if (e != 0) {
            while (i < 0x20) {
                if (*(unsigned short *)(e + 0x27) != 0xffff && *(int *)(e + 0x61) != 0)
                    arr[i++] = e;
                e = gm_BuildNext();
                if (e == 0)
                    break;
            }
            if (i != 0 && he_Unk55ee40(a, i, arr, &v10, &v18) != 0)
                gm_FindBuildingById(v10);
        }
        bld = o;
    } else {
        step = s[10];
        ebx = (s[11] + step) & 0xff;
        left = 0x100;
        bld = 0;
        do {
            bld = U_I((unsigned)&ai_Blds) + ebx * 0xa9;
            o = bld;
            if (*(unsigned char *)bld != 0) {
                int k = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)bld * 0x24d);

                lo = 0;
                hi = 0;
                if (k > 0x1f)
                    hi = 1u << (k - 0x20);
                else
                    lo = 1u << k;
                if (*(int *)(bld + 0x61) != 0 && ((lo & 0xa7f4c3a0) != 0 || (hi & 1) != 0))
                    break;
                bld = 0;
                o = 0;
            }
            ebx = (ebx + step) & 0xff;
            left--;
        } while (left != 0);
    }
    if (bld == 0) {
        s[9] |= 4;
        return;
    }
    s[11] = (unsigned char)ebx;
    *(int *)(s + 4) = *(int *)(bld + 1);
    s[8]++;
    kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)bld * 0x24d);
    if (kind == 0x16 || kind == 7 || kind == 0xf)
        cm_SendChrGotoBuilding(*(int *)s, *(int *)(s + 4), -1, 0, 0, "Sim Politicians");
    else
        cm_SendChrGotoBuilding(*(int *)s, *(int *)(s + 4), -1, 0, 1, "Sim Politicians");
}

int cm_GetSlotState();
void gm_Shuffled(unsigned char n, int *out);

/* 0x551b70, the run handler of the walk event 0x5512c0 (ROUGH): states -1 / -2 free the record; a record with flag 4 or a pending handle (+0x84, 0x4c3ae0 checks it) does nothing; else the due clock is set to now and posted 5 later, and after the minute word 0x11 the ten slots (+0xac, 16 bytes) are stepped by 0x551340 (counting the ones still walking = 2; when none walks, the minute word is reset and the day counter +0x52 incremented), before it the slots are stepped by 0x551340 (done: flag 4) or 0x551780 and the ones with flag 1 counted, then up to ten tries to fill a slot (0x551440) post the event again 1 later */
void he_Unk551b70(int ev)
{
    int st = *(int *)(ev + 0x70);
    int n = 0, i, r;
    unsigned char *s;

    if (st == -1 || st == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    *(int *)(ev + 0x84) = -1;
    if (*(unsigned short *)(ev + 0x56) >= 0x11) {
        s = (unsigned char *)(ev + 0xac);
        for (i = 10; i != 0; i--) {
            if (he_StepWalkSlot((HEWALKSLOT *)s) == 2)
                n++;
            s += 0x10;
        }
        if (n == 0) {
            *(unsigned short *)(ev + 0x56) = (unsigned short)n;
            *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
        }
        cm_SendExSetHE(0, ev);
        return;
    }
    s = (unsigned char *)(ev + 0xac);
    for (i = 10; i != 0; i--) {
        if ((s[9] & 4) != 0)
            he_StepWalkSlot((HEWALKSLOT *)s);
        else
            he_Unk551780(s);
        if ((s[9] & 1) != 0)
            n++;
        s += 0x10;
    }
    s = (unsigned char *)(ev + 0xac);
    for (i = 0; i < 10; i++) {
        r = he_Unk551440(s, n, 0);
        *(int *)(ev + 0x84) = r;
        if (r != -1) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 1, 0);
            cm_SendExSetHE(0, ev);
            return;
        }
        s += 0x10;
    }
    cm_SendExSetHE(0, ev);
}
/* 0x551d00, the alloc handler of the six-group event (type 0x7d, the run handler 0x551d70 walks its six figure
   groups at +0xb0): gm_Shuffled fills the six ids and each is incremented (0 -> the first figure), the due time
   is the clock now with hour 9 / min 0, and the state becomes 0xc */

void he_InitSixGroupEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    int i;
    int *q;

    gm_Shuffled(6, e->groups);
    for (q = e->groups, i = 0; i < 6; i++)
        *q++ += 1;
    e->end = ai_Time;
    e->stage = 0xc;
    e->end.hour = 9;
    e->end.min = 0;
}

int amt_GetColumnOffices(int col, int max, struct AMTREC *out);
int amt_GetKind(int k, struct AMTKIND *out);
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);
int cm_SendRelations(int a, int b, int c, int d);

/* 0x551d70 (the Ghidra start; ROUGH), the run handler of the event 0x551d00 (six figure groups at +0xb0, state 0xc = the announcement): state 0xc tells every player character (kind 6 / 7: 0x53dda0 (id, -1, text 0x10bc / 0x10be by +0xac, 0x20, 0x600, 0x631090)) and goes to state 0; states 0 .. 5 take the group +0xb0 + state * 4: for every member (0x4e6fc0, 0x18-byte records) that has a person (0x505df0) and a job (0x4e6f80) a random roll may raise its mood (0x4e6680), unless the person has kind 3 every other character in the same building (+0x168 / +0x27) gets a random opinion change (0x4c4b00), and a player character gets the message 0x10bd / 0x10bf (0x5fd); the state is then incremented and the event posted 1 later; other states free the record */
void he_Unk551d70(int ev)
{
    char text[0x200];
    int st = *(int *)(ev + 0x70);
    unsigned char arr[0x18 * 6];
    int n, i, k, c, p, hit;
    int job[8];

    if (st == 0xc) {
        txt_Format(text, *(int *)(ev + 0xac) == 0 ? 0x10bc : 0x10be, (char *)((unsigned)&ai_PlayerSlots + 0x20) + *(unsigned char *)(unsigned)&ai_OwnPlayer * 0x2f4);
        for (c = (unsigned)&ai_Chars; c < (unsigned)((unsigned)ai_Chars + 0x65400); c += 0x21c) {
            if (*(unsigned char *)(c + 2) == 6 || *(unsigned char *)(c + 2) == 7)
                he_ShowNews(*(int *)(c + 4), -1, text, 0x20, 0x600, (unsigned)&ch_EmptyName);
        }
        *(int *)(ev + 0x70) = 0;
        gm_AddTime(ev + 0x52, 1, 0, 0);
        return;
    }
    if (st < 0 || st >= 6) {
        he_FreeEvent((int *)ev);
        return;
    }
    n = ((int (*)(int, int, struct AMTREC *, int))amt_GetColumnOffices)(*(int *)(ev + 0xb0 + st * 4), arr, 6, 0);
    if (n > 0) {
        for (i = 0; i < n; i++) {
            unsigned char *m = arr + i * 0x18;

            p = gm_FindChar(*(int *)(m + 0));
            if (p == 0)
                continue;
            if (amt_GetKind(m[4], job) == 0)
                continue;
            if (*(int *)(ev + 0xac) == 1 && ((unsigned char)job[2] == 9 || (unsigned char)job[2] == 6)) {
                if ((unsigned short)util_Random(3) != 0)
                    amt_SendOfficeMessage(m[0], 0, 0, 4, 0xff);
            }
            if (*(unsigned char *)(p + 2) == 3)
                continue;
            hit = 0;
            for (c = (unsigned)&ai_CharInts; c < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2); c += 0x21c) {
                if (*(int *)(c + 0x168) != 0 && *(int *)c != *(int *)(p + 4) && *(unsigned short *)(*(int *)(c + 0x168) + 0x27) == *(unsigned short *)p) {
                    hit++;
                    k = (unsigned short)util_Random(1);
                    ((void (*)(int, int, int))cm_SendRelations)(*(int *)(p + 4), *(int *)c, -k - job[2] * 3);
                }
            }
            if (hit != 0 && (*(unsigned char *)(p + 2) == 6 || *(unsigned char *)(p + 2) == 7)) {
                txt_Format(text, *(int *)(ev + 0xac) == 0 ? 0x10bd : 0x10bf, *(unsigned short *)p);
                he_ShowNews(*(int *)(p + 4), -1, text, 1, 0x5fd, (unsigned)&ch_EmptyName);
            }
        }
    }
    *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
    gm_AddTime(ev + 0x52, 1, 0, 0);
}
/* 0x552040, re-posts an event 2 later and resets its state to 0 */

void he_Unk552040(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 2, 0, 0);
    *(int *)(ev + 0x70) = 0;
}
/* 0x552090, the run handler of the master exam style event "Berufe\Messagebox_Meisterpruefung" (ROUGH, same shape as 0x547430): states -1 / -2 free the record (and its window), state 0 opens the window, shows the page 0x602568 and the news 0x10b9 (player name, figure +8), state 1 waits for the due time (then it closes) or the answer (0xf8a668: 0 = the figure's record 0x53e400 (2, 2, id, 1, +0xb0) gets +0xb0 = 1, 1 = nothing) and closes */

void he_Unk552090(int ev)
{
    int st = *(int *)(ev + 0x70);
    int h;
    char *r;

    if (st == -1 || st == -2) {
        if (*(int *)(ev + 0x74) != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (st == 0) {
        if (*(int *)(ev + 0x74) == 0)
            he_OpenEventSlot(ev, 0, 0, (int)"Berufe\Messagebox_Meisterpruefung", 0x5ec);
        if (*(int *)(ev + 0x74) != 0) {
            d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
            d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
            io_PrintText("$C");
            io_PrintText(0x10b9, (char *)((unsigned)&ai_PlayerSlots + 0x20) + *(unsigned char *)(unsigned)&ai_OwnPlayer * 0x2f4, *(unsigned short *)(ev + 8));
            *(int *)(ev + 0x70) = 1;
        } else {
            he_FreeEvent((int *)ev);
        }
        return;
    }
    if (st == 1) {
        if (gm_CompareTime((unsigned)&ai_Time, ev + 0x52) > 0) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (*(int *)(ev + 0x74) == 0) {
            *(int *)(ev + 0x70) = 0;
            return;
        }
        if (U_I((unsigned)&d2_ClickedForm) != *(int *)(*(int *)(ev + 0x74) + 8))
            return;
        h = U_I((unsigned)&d2_ClickedId);
        if (h == -1)
            return;
        if (h == 0) {
            r = he_FindEvent(2, 2, *(unsigned short *)(ev + 8), 1, *(int *)(ev + 0xb0));
            *(int *)(r + 0xb0) = 1;
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
        } else if (h == 1) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
        }
    }
}

int cm_SendLongMessage(char *msg);
float gm_GetSkill(char *c, int skill);
int gm_GetWorth(unsigned short id);
float gm_CharacterHealth();
float gm_AddHealth(unsigned short id, int d);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);

/* 0x552200, the allocation handler of the 0x552270 event: state 0, due now, minute word 9 + random 0x42ec00(3), +0xb0 = 0 (no answer yet), +0x58 = random 0x42ec00(4) * 15 */
void he_Unk552200(int ev)
{
    int r;

    *(int *)(ev + 0x70) = 0;
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(unsigned short *)(ev + 0x56) = (unsigned short)(util_Random(3) + 9);
    r = (unsigned short)util_Random(4);
    *(int *)(ev + 0xb0) = 0;
    *(int *)(ev + 0x58) = r * 15;
}
/* 0x552270, the run handler of the two-stage event of 0x552200 (ROUGH): state 0 posts a message record of 0xf8 bytes (type 0x7e, the figure +0xac, the clocks) through 0x4fe140 and 0x4c4b70 and the event 8 later (state 1); state 1: with +0xb0 set the figure +8 pays (0x504ec0 (.., 3) clamped to 0.1 scales the amount, 0x50a640 / 0x4fc820 give the base, 0x4fca40 (id, -amount) and 0x4c4830 (+0xac, -1, player, amount)) and the player characters read the news 0x10ba, without it the news 0x10bb; then the record is freed */

void he_Unk552270(int ev)
{
    int st = *(int *)(ev + 0x70);
    char text[0x100];
    unsigned char m[0xf8];
    int ch, x, v, pay;
    float f, t;

    if (st < 0 || st > 1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (st == 0) {
        memset(m, 0, sizeof(m));
        *(int *)(m + 0x10) = *(int *)(ev + 0xac);
        *(int *)(m + 0x60) = *(int *)(ev + 0xac);
        m[0x18] = 0x7e;
        *(int *)(m + 0x20) = -1;
        *(int *)(m + 0x30) = *(int *)(unsigned)&ai_Time;
        *(int *)(m + 0x68) = *(int *)(unsigned)&ai_Time;
        *(int *)(m + 0x40) = *(int *)(unsigned)&ai_Time.hour;
        *(int *)(m + 0x44) = *(int *)((unsigned)&ai_Time + 0x8);
        *(unsigned short *)(m + 0x48) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
        m[0x4a] = 9;
        *(int *)(m + 0x70) = *(int *)(ev + 4);
        *(int *)(m + 0x78) = *(int *)(unsigned)&ai_Time.hour;
        *(int *)(m + 0x7c) = *(int *)((unsigned)&ai_Time + 0x8);
        *(unsigned short *)(m + 0x80) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
        gm_AddTime(m + 0x6c, 2, 0, 0);
        cm_SendLongMessage(m + 0x14);
        *(int *)(ev + 0x70) = 1;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 8, 0, 0);
        return;
    }
    if (*(int *)(ev + 0xb0) != 0) {
        ch = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
        if (gm_GetSkill(ch, 3) > 0.1f)
            f = gm_GetSkill(ch, 3);
        else
            f = 0.1f;
        x = gm_GetWorth(*(unsigned short *)ch);
        v = (int)gm_CharacterHealth(ch);
        t = (float)v * (*(float *)(unsigned)&ai_FloatOne - f) * 0.5f;
        gm_AddHealth(*(unsigned short *)ch, -(int)t);
        pay = (int)((float)x * f * (0.0599999987f));
        cm_SendMoveGoodsUnchecked(*(int *)(ev + 0xac), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, pay);
        txt_Format(text, 0x10ba, pay);
        he_ShowNews(*(int *)(ev + 0xac), -1, text, 0x20, 0x5fd, (unsigned)&ch_EmptyName);
    } else {
        txt_Format(text, 0x10bb);
        he_ShowNews(*(int *)(ev + 0xac), -1, text, 0x20, 0x5fd, (unsigned)&ch_EmptyName);
    }
    he_FreeEvent((int *)ev);
}
/* 0x5524e0, the allocation handler of an event without the flag 4 of +0x78: the due clock (+0x52) is a copy of the start clock (+0x44) and the event is posted (0x4c4c60 (0, ev), handle in +0x84) */

void he_Unk5524e0(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}

void cm_BeginFieldBlock(char *o, int id);
int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v);
void cm_SendFieldDeltas(void);

/* 0x552520, the run handler of a counting event: a negative state or a counter +0xac <= 0 frees the record; flag 4 does nothing; else the counter is decremented and the event posted 0x18 later (handle in +0x84) */
void he_Unk552520(int ev)
{
    if (*(int *)(ev + 0x70) < 0 || *(int *)(ev + 0xac) <= 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    *(int *)(ev + 0xac) = *(int *)(ev + 0xac) - 1;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
/* 0x5525a0, the run handler of the migration event (ROUGH): +0xac people (at most sqrt(+0xac + +0xb0)) of the 0x300 characters (the ring from a random start; id != 0xffff, kind <= 4, byte +0xc = +0xb4) are sent away (0x4c3ba0, 0x4c3c00 (1, 1, 0xc, ..), 0x4c49a0); +0xb0 / +0xac are reduced and the event posted 0x18 later, or 5 later with the handle -1 when not enough were found */

void he_Unk5525a0(int ev)
{
    int cnt = 0, n, a, b, ring, left, ch;
    double s;
    int x[4];

    if (*(int *)(ev + 0x70) < 0 || *(int *)(ev + 0xb0) <= 0 || *(int *)(ev + 0xac) <= 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    a = *(int *)(ev + 0xac);
    b = *(int *)(ev + 0xb0);
    s = sqrt((double)(a + b));
    if (s > (double)a)
        n = (int)(double)*(int *)(ev + 0xac);
    else
        n = (int)sqrt((double)(*(int *)(ev + 0xb0) + *(int *)(ev + 0xac)));
    ring = (unsigned short)util_Random(0x300);
    for (left = 0x300; left != 0; left--) {
        if (cnt >= n)
            break;
        ch = (unsigned)&ai_Chars + ring * 0x21c;
        if (*(unsigned short *)ch != 0xffff && *(unsigned char *)(ch + 2) <= 4 && *(unsigned char *)(ch + 0xc) == *(int *)(ev + 0xb4)) {
            cm_BeginFieldBlock(ch, *(int *)(ch + 4));
            cm_AddFieldDelta(1, 1, 0xc, x);
            cm_SendFieldDeltas();
            cnt++;
        }
        ring = (ring + 1) % 0x300;
    }
    if (cnt < n) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
    *(int *)(ev + 0xac) = *(int *)(ev + 0xac) - n;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
/* 0x5527c0, the allocation handler: unless flag 4 of +0x78 is set the event is posted +0xb8 units later (due clock now, 0x4c4c60 (0, ev), handle in +0x84) */

void he_Unk5527c0(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, *(int *)(ev + 0xb8), 0, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
/* 0x552820, the run handler of a one-shot event: states -1 / -2 free the record; else unless flag 4 the event is posted with the handle -1 (0x4c4c60 (-1, ev), due clock now, handle in +0x84) */

void he_Unk552820(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st == -1 || st == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
/* 0x552880, the allocation handler of an event with a day timer (ROUGH): unless flag 4 the due clock is now (posted 0x18 later when the minute word was already >= 0x11), the minute word is set to 0x11, +0xac = 0, +0xb0 = 0x1b, +0xb4 = +0xb8 = -1 and the event posted (0x4c4c60 (0, ev), handle in +0x84) */

void he_Unk552880(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        if (*(unsigned short *)(ev + 0x56) >= 0x11)
            gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = 0x11;
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0xac) = 0;
        *(int *)(ev + 0xb0) = 0x1b;
        *(int *)(ev + 0xb4) = -1;
        *(int *)(ev + 0xb8) = -1;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}

int cm_GetSlotValue(int h);
int amt_InitApplicant();
int amt_GetOpenOffice();
int amt_CreateApplicant();

/* 0x552920, the run handler of the two-person event (ROUGH): states -1 / -2 free the record, flag 4 or a pending handle (+0x84) does nothing. State 0: with +0xb0 set 0x4e78a0 picks a person +0xac (none: the record is reset like 0x552880 with the minute word 0x11 and 0x4c4c60 (0, ev)), 0x4e7aa0 picks the two partners +0xb4 and (one time in 4 less) +0xb8 and the event is posted 1 later; state 1 checks both (0x4c3ae0: 1 = arrived: 0x4e7b60 (the person of the handle 0x4c3b10, +0xac), 2 = gone) and posts 5 later when both are done */
void he_Unk552920(int ev)
{
    int st = *(int *)(ev + 0x70);
    int n, r;

    if (st == -1 || st == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    if (st == 0) {
        if (*(int *)(ev + 0xb0) == 0) {
            goto reset;
        }
        *(int *)(ev + 0xac) = amt_GetOpenOffice(*(int *)(ev + 0xb0));
        if (*(int *)(ev + 0xac) == 0) {
reset:
            gm_AddTime(ev + 0x52, 0x18, 0, 0);
            *(unsigned short *)(ev + 0x56) = 0x11;
            *(int *)(ev + 0x58) = 0;
            *(int *)(ev + 0xac) = 0;
            *(int *)(ev + 0xb0) = 0x1b;
            *(int *)(ev + 0xb4) = -1;
            *(int *)(ev + 0xb8) = -1;
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
            return;
        }
        *(int *)(ev + 0xb4) = amt_CreateApplicant(*(int *)(ev + 0xac));
        if ((unsigned short)util_Random(3) != 0)
            *(int *)(ev + 0xb8) = amt_CreateApplicant(*(int *)(ev + 0xac));
        else
            *(int *)(ev + 0xb8) = -1;
    } else if (st == 1) {
        n = 0;
        if (*(int *)(ev + 0xb4) == -1) {
            n = 1;
        } else {
            r = cm_GetSlotState(*(int *)(ev + 0xb4));
            if (r == 1) {
                n = 1;
                amt_InitApplicant(cm_GetSlotValue(*(int *)(ev + 0xb4)), *(int *)(ev + 0xac));
                *(int *)(ev + 0xb4) = -1;
            } else if (r == 2) {
                *(int *)(ev + 0xb4) = -1;
                n = 1;
            }
        }
        if (*(int *)(ev + 0xb8) == -1) {
            n++;
        } else {
            r = cm_GetSlotState(*(int *)(ev + 0xb8));
            if (r == 1) {
                n++;
                amt_InitApplicant(cm_GetSlotValue(*(int *)(ev + 0xb8)), *(int *)(ev + 0xac));
                *(int *)(ev + 0xb8) = -1;
            } else if (r == 2) {
                *(int *)(ev + 0xb8) = -1;
                n++;
            }
        }
        if (n == 2) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
            return;
        }
    } else {
        return;
    }
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
}

int ai_TutorialSetHost();
int ai_TutorialOpenIntro();
int ai_TutorialTick();
int ai_TutorialCloseIntro();
void ai_TutorialClearHost(void);
void sv_PopTo(int v);
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay);

/* 0x552b90, the run handler of an event driven by 0x578120: when it returns nonzero the record is freed, else the state is 0 */
void he_Unk552b90(int ev)
{
    if (ai_TutorialSetHost(ev) != 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    *(int *)(ev + 0x70) = 0;
}
/* 0x552bc0, the run handler of the end-of-game style event (only with 0x6f25ac set; ROUGH): state 0 -> 1 or 2 by 0x577740, state 1 -> 2 when 0x575860 says so, state 2 shuts down (0x577820, 0x578150), frees the record, sets the flag 0x6f259c and continues in 0x52fb50 (0x53bb20) */

void he_Unk552bc0(int ev)
{
    if (*(unsigned char *)(unsigned)&ai_GameState == 0)
        return;
    switch (*(int *)(ev + 0x70)) {
    case 0:
        *(int *)(ev + 0x70) = (ai_TutorialOpenIntro() != 0) + 1;
        break;
    case 1:
        if (ai_TutorialTick() != 0)
            *(int *)(ev + 0x70) = 2;
        break;
    case 2:
        ai_TutorialCloseIntro();
        ai_TutorialClearHost();
        he_FreeEvent((int *)ev);
        U_I((unsigned)&ai_GameOver) = 1;
        sv_PopTo((void *)(unsigned)&main_GameLoop);
        break;
    }
}
/* 0x552c30, the allocation handler of an event: state 0, due clock (+0x52) and the clock +0xac set to now */

void he_Unk552c30(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0xac) = ai_Time;
    *(int *)(ev + 0x70) = 0;
}
/* 0x552ca0, the run handler of the award ceremony of an imperial office ("_VERLEIHUNG_REICHSAMT_.." texts; the figure +8 is the player's, +0xbc the awarded person, +0xc0 the office 0x23 .. 0x25, +0xac the clock of the award; ROUGH): nothing unless the figure is the player's (0x6f25b8) and the game runs; state 0 opens the window "SPECIAL\TITEL" (the player is awarded) or "SPECIAL\TITEL_ANDERE", plays the fanfare "FanfarePositivHeldenhaftLang" and the spoken address / office texts (0x4c27d0 (-7, 0, name, 0, 100) with the text names by sex (person +9) and rank (person +0xd: 9 = _02, 8 = _01, else _00)), prints the news (0x515960) and the click sound; state 1 waits for the answer (0xf8a668 == 0 or the confirm 0x6f2630) and closes; a gone figure / person frees the record */

void he_Unk552ca0(int ev)
{
    int id = *(unsigned short *)(ev + 8);
    int ch = (unsigned)&ai_Chars + id * 0x21c;
    int p, st, own, base, tid, w, off;
    char *s1;
    char *s2;
    char cl;

    if (*(unsigned short *)(unsigned)&ai_PlayerId != id || *(unsigned char *)(unsigned)&ai_GameState == 0)
        return;
    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) == 1)
        return;
    p = gm_FindChar(*(int *)(ev + 0xbc));
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1 || *(unsigned short *)ch == 0xffff || p == 0) {
        if (*(int *)(ev + 0x74) != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (st == 0) {
        if (*(int *)(ev + 0x74) == 0) {
            if (p == ch)
                he_OpenEventSlot(ev, 0, 0, (int)"SPECIAL\TITEL", 0x5fd);
            else
                he_OpenEventSlot(ev, 0, 0, (int)"SPECIAL\TITEL_ANDERE", 0x5fe);
        }
        if (*(int *)(ev + 0x74) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        w = *(unsigned char *)(p + 9) == 1;
        off = *(unsigned char *)(p + 9) == 0 ? 0x253 : *(int *)(ev + 0xc0) + 0x27b;
        if (p == ch) {
            int rank = *(unsigned char *)(p + 0xd);

            if (rank == 9) {
                base = 0x42a9;
                s1 = w ? "_VERLEIHUNG_REICHSAMT_ANREDE_HS_W_02" : "_VERLEIHUNG_REICHSAMT_ANREDE_HS_M_02";
            } else if (rank == 8) {
                base = 0x42a7;
                s1 = w ? "_VERLEIHUNG_REICHSAMT_ANREDE_HS_W_01" : "_VERLEIHUNG_REICHSAMT_ANREDE_HS_M_01";
            } else {
                base = 0x42a5;
                s1 = w ? "_VERLEIHUNG_REICHSAMT_ANREDE_HS_W_00" : "_VERLEIHUNG_REICHSAMT_ANREDE_HS_M_00";
            }
            tid = 0;
            if (w)
                base++;
            snd_QueueVoice(-7, 0, "FanfarePositivHeldenhaftLang", 0, 0x64);
            snd_QueueVoice(-7, 0, s1, 0, 0x64);
            s2 = 0;
            if (*(int *)(ev + 0xc0) == 0x25) {
                tid = 0x42ab;
                s2 = w ? "_VERLEIHUNG_REICHSAMT_WEIBLICH_HS_00" : "_VERLEIHUNG_REICHSAMT_MAENNLICH_HS_00";
            } else if (*(int *)(ev + 0xc0) == 0x24) {
                tid = 0x42af;
                s2 = w ? "_VERLEIHUNG_REICHSAMT_WEIBLICH_HS_02" : "_VERLEIHUNG_REICHSAMT_MAENNLICH_HS_02";
            } else if (*(int *)(ev + 0xc0) == 0x23) {
                tid = 0x42ad;
                s2 = w ? "_VERLEIHUNG_REICHSAMT_WEIBLICH_HS_01" : "_VERLEIHUNG_REICHSAMT_MAENNLICH_HS_01";
            }
            snd_QueueVoice(-7, 0, s2, 0, 0x64);
            snd_QueueVoice(-7, 0, "MausclickLinks_s_01", -1, 0);
            io_PrintText(tid, base, ev + 0xac, off);
        } else {
            tid = 0;
            if (*(int *)(ev + 0xc0) == 0x25)
                tid = 0x42ac;
            else if (*(int *)(ev + 0xc0) == 0x24)
                tid = 0x42b0;
            else if (*(int *)(ev + 0xc0) == 0x23)
                tid = 0x42ae;
            io_PrintText(tid, *(unsigned short *)p, off);
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 2);
        io_PrintText(0x161c);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (st == 1) {
        if (*(int *)(ev + 0x74) == 0) {
            *(int *)(ev + 0x70) = 0;
            return;
        }
        cl = *(char *)(unsigned)&he_SlotConfirmed;
        if (U_I((unsigned)&d2_ClickedForm) != *(int *)(*(int *)(ev + 0x74) + 8) && cl == 0)
            return;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (cl == 0 || U_I((unsigned)&he_ActiveSlot) != *(int *)(ev + 0x74))
            return;
        *(char *)(unsigned)&he_SlotConfirmed = 0;
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
    }
}

void ai_Unk56f040();
int sv_RegisterActivity();

/* 0x553060, registers the alloc / run handler pairs of the event types 0x22 .. 0xa5 (he_SetEventHandlers; the pair of a type is (alloc, run); 1 = a registration failed; 0x52b350 (type, 0, 0, 0x507 / 0x50e, -1) registers the message type of a few of them). The handlers above 0x553060 are the ones of this file; 0x56f040 is a forwarder to the free (0x53e1f0); the rest lives in the stretch 0x553880 .. 0x575700 (drafts to come) */
int he_RegisterEventTypes22(void)
{
    if (he_SetEventHandlers(0x22, (void *)d3io_NoopCallback, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_SetEventHandlers(0x29, (void *)he_RunDueNow, (void *)ai_Unk56f040) != 0)
        return 1;
    if (he_SetEventHandlers(0x2a, (void *)he_AllocStammtisch, (void *)he_RunStammtisch) != 0)
        return 1;
    if (he_SetEventHandlers(0x2b, (void *)he_AllocTavern, (void *)he_RunTavern) != 0)
        return 1;
    if (he_SetEventHandlers(0x2c, (void *)he_AllocOffer, (void *)he_RunWeddingOffer) != 0)
        return 1;
    if (he_SetEventHandlers(0x2d, (void *)ai_Unk56f040, (void *)ai_Unk56f040) != 0)
        return 1;
    if (he_SetEventHandlers(0x2e, (void *)he_AllocQuarrel, (void *)he_Unk543180) != 0)
        return 1;
    if (he_SetEventHandlers(0x2f, (void *)he_RunDueNow, (void *)he_RunMasterExam) != 0)
        return 1;
    if (he_SetEventHandlers(0x30, (void *)ai_Unk56f040, (void *)ai_Unk56f040) != 0)
        return 1;
    if (he_SetEventHandlers(0x37, (void *)he_Unk5436a0, (void *)he_RunFeast) != 0)
        return 1;
    if (he_SetEventHandlers(0x36, (void *)he_Unk552040, (void *)he_RunFeastExam) != 0)
        return 1;
    if (he_SetEventHandlers(0x35, (void *)he_AllocTribute, (void *)he_RunTribute) != 0)
        return 1;
    if (he_SetEventHandlers(0x41, (void *)he_AllocCourtship, (void *)he_RunCourtship) != 0)
        return 1;
    if (he_SetEventHandlers(0x42, (void *)he_Unk5455e0, (void *)he_Unk545640) != 0)
        return 1;
    if (he_SetEventHandlers(0x43, (void *)he_AllocPatrol, (void *)he_RunPatrol) != 0)
        return 1;
    sv_RegisterActivity(0x43, 0x0, 0x0, 0x507, -1);
    if (he_SetEventHandlers(0x44, (void *)he_Unk546100, (void *)he_RunRaid) != 0)
        return 1;
    if (he_SetEventHandlers(0x45, (void *)he_AllocFight, (void *)he_RunFight) != 0)
        return 1;
    if (he_SetEventHandlers(0x46, (void *)he_Unk547170, (void *)he_RunDuelChallenge) != 0)
        return 1;
    if (he_SetEventHandlers(0x47, (void *)he_AllocInsult, (void *)he_RunInsult) != 0)
        return 1;
    if (he_SetEventHandlers(0x4a, (void *)he_Unk547ee0, (void *)he_RunGeselle) != 0)
        return 1;
    if (he_SetEventHandlers(0x4b, (void *)he_Unk548110, (void *)he_RunInsultNews) != 0)
        return 1;
    if (he_SetEventHandlers(0x51, (void *)he_Unk5482c0, (void *)he_Unk548300) != 0)
        return 1;
    if (he_SetEventHandlers(0x52, (void *)he_Unk548330, (void *)he_Unk548380) != 0)
        return 1;
    if (he_SetEventHandlers(0x53, (void *)he_Unk5483a0, (void *)he_RunCharPayouts) != 0)
        return 1;
    if (he_SetEventHandlers(0x54, (void *)he_Unk55e3a0, (void *)he_RunMasterExamFee) != 0)
        return 1;
    if (he_SetEventHandlers(0x55, (void *)he_Unk548750, (void *)he_RunMasterExamPay) != 0)
        return 1;
    if (he_SetEventHandlers(0x56, (void *)he_Unk5482c0, (void *)he_RunPayment) != 0)
        return 1;
    if (he_SetEventHandlers(0x57, (void *)he_Unk548b70, (void *)he_RunStoreSale) != 0)
        return 1;
    if (he_SetEventHandlers(0x58, (void *)he_Unk548b70, (void *)he_RunBuildingDecay) != 0)
        return 1;
    if (he_SetEventHandlers(0x5a, (void *)he_Unk548ef0, (void *)he_RunRepostEvent) != 0)
        return 1;
    if (he_SetEventHandlers(0x5b, (void *)he_Unk549040, (void *)he_Unk548fa0) != 0)
        return 1;
    if (he_SetEventHandlers(0x5c, (void *)he_Unk549040, (void *)he_Unk549080) != 0)
        return 1;
    if (he_SetEventHandlers(0x5d, (void *)he_Unk549110, (void *)he_RunOffspring) != 0)
        return 1;
    if (he_SetEventHandlers(0x5e, (void *)he_StartTrialEvent, (void *)he_RunTrialEvent) != 0)
        return 1;
    if (he_SetEventHandlers(0x5f, (void *)he_StartNextTrialStep, (void *)he_RunTrialSteps) != 0)
        return 1;
    if (he_SetEventHandlers(0x60, (void *)he_StartTraining, (void *)he_RunTraining) != 0)
        return 1;
    if (he_SetEventHandlers(0x59, (void *)he_Unk549f70, (void *)he_RunPlague) != 0)
        return 1;
    if (he_SetEventHandlers(0x61, (void *)he_StartGang, (void *)he_RunGang) != 0)
        return 1;
    sv_RegisterActivity(0x61, 0x0, 0x0, 0x507, -1);
    if (he_SetEventHandlers(0x63, (void *)he_Unk54ae20, (void *)he_RunProtectionAsk) != 0)
        return 1;
    if (he_SetEventHandlers(0x62, (void *)he_StartProtection, (void *)he_RunProtection) != 0)
        return 1;
    sv_RegisterActivity(0x62, 0x0, 0x0, 0x50e, -1);
    if (he_SetEventHandlers(0x64, (void *)he_StartAudit, (void *)he_RunAudit) != 0)
        return 1;
    sv_RegisterActivity(0x64, 0x0, 0x0, 0x507, -1);
    if (he_SetEventHandlers(0x65, (void *)he_StartGathering, (void *)he_RunGathering) != 0)
        return 1;
    sv_RegisterActivity(0x65, 0x0, 0x0, 0x507, -1);
    if (he_SetEventHandlers(0x66, (void *)he_Unk54c4c0, (void *)he_RunAccidentSim) != 0)
        return 1;
    if (he_SetEventHandlers(0x67, (void *)he_StartAuctionEvent, (void *)he_RunAuctionEvent) != 0)
        return 1;
    if (he_SetEventHandlers(0x68, (void *)he_StartInnSim, (void *)he_RunInnSim) != 0)
        return 1;
    if (he_SetEventHandlers(0x69, (void *)he_StartTitleCeremony, (void *)he_RunTitleCeremony) != 0)
        return 1;
    if (he_SetEventHandlers(0x6a, (void *)he_StartSkillUp, (void *)he_RunSkillUp) != 0)
        return 1;
    if (he_SetEventHandlers(0x6b, (void *)he_StartBard, (void *)he_RunBard) != 0)
        return 1;
    if (he_SetEventHandlers(0x6d, (void *)he_StartPopulation, (void *)he_RunPopulation) != 0)
        return 1;
    if (he_SetEventHandlers(0x6f, (void *)he_StartLover, (void *)he_RunLover) != 0)
        return 1;
    if (he_SetEventHandlers(0x70, (void *)he_Unk54c4c0, (void *)he_RunDiseaseSim) != 0)
        return 1;
    if (he_SetEventHandlers(0x74, (void *)he_StartDarkCorner, (void *)he_RunDarkCorner) != 0)
        return 1;
    if (he_SetEventHandlers(0x76, (void *)he_StartEffectEvent, (void *)he_RunEffectEvent) != 0)
        return 1;
    if (he_SetEventHandlers(0x77, (void *)he_Unk54fe80, (void *)he_RunSendHomeEvent) != 0)
        return 1;
    if (he_SetEventHandlers(0x78, (void *)he_StartReaper, (void *)he_RunReaper) != 0)
        return 1;
    if (he_SetEventHandlers(0x79, (void *)he_Unk550ea0, (void *)he_RunRanking) != 0)
        return 1;
    if (he_SetEventHandlers(0x7a, (void *)he_Unk5512c0, (void *)he_Unk551b70) != 0)
        return 1;
    if (he_SetEventHandlers(0x7b, (void *)ai_Unk56f040, (void *)ai_Unk56f040) != 0)
        return 1;
    if (he_SetEventHandlers(0x7d, (void *)he_InitSixGroupEvent, (void *)he_Unk551d70) != 0)
        return 1;
    if (he_SetEventHandlers(0x7e, (void *)he_Unk552040, (void *)he_Unk552090) != 0)
        return 1;
    if (he_SetEventHandlers(0x7f, (void *)he_Unk552200, (void *)he_Unk552270) != 0)
        return 1;
    if (he_SetEventHandlers(0x80, (void *)he_Unk5524e0, (void *)he_Unk552520) != 0)
        return 1;
    if (he_SetEventHandlers(0x81, (void *)he_Unk5524e0, (void *)he_Unk5525a0) != 0)
        return 1;
    if (he_SetEventHandlers(0x82, (void *)he_Unk5527c0, (void *)he_Unk552820) != 0)
        return 1;
    if (he_SetEventHandlers(0x86, (void *)he_Unk552880, (void *)he_Unk552920) != 0)
        return 1;
    if (he_SetEventHandlers(0x87, (void *)he_Unk552b90, (void *)he_Unk552bc0) != 0)
        return 1;
    return he_SetEventHandlers(0xa5, (void *)he_Unk552c30, (void *)he_Unk552ca0) != 0;
}

void cm_SendHandOverCrimes(int a, int b, int c, int d);

/* 0x5538b0, ends every running event record that involves the person `p` (+4 the id; nothing for a free slot): the records of the types 0x41, 0x6f, 0x45 (the two partners +0xac / +0xb0, the other one is looked up (0x505df0) before the record is ended by 0x541200), 0x2c (the partners +0xac / +0xb4 / +0xc0 get the message 0x4c5580 (+0xac, +0xb0, +4, 0)), 0x47 (+0xac or +0xb0), 0x5e and 0x5f (+0xac); only records with a bit of 3 in +0x78 are touched (ROUGH) */
void he_EndPersonEvents(unsigned short *p)
{
    char *e;
    int id, a, b;

    if (p == 0 || *p == 0xffff)
        return;
    id = *(int *)((char *)p + 4);
    for (e = he_FindEvent(1, 0, 0x41); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        a = *(int *)(e + 0xac);
        if (a == id)
            gm_FindChar(*(int *)(e + 0xb0));
        else if (*(int *)(e + 0xb0) == id)
            gm_FindChar(a);
        else
            continue;
        he_EndEventNow((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x6f); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        a = *(int *)(e + 0xac);
        if (a == id)
            b = *(int *)(e + 0xb0);
        else if (*(int *)(e + 0xb0) == id)
            b = a;
        else
            continue;
        gm_FindChar(b);
        he_EndEventNow((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x45); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        a = *(int *)(e + 0xac);
        b = *(int *)(e + 0xb0);
        if (a == id)
            gm_FindChar(b);
        else if (b == id)
            gm_FindChar(a);
        else
            continue;
        he_EndEventNow((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x2c); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        a = *(int *)(e + 0xac);
        if (a == id || *(int *)(e + 0xb4) == id || *(int *)(e + 0xc0) == id) {
            cm_SendHandOverCrimes(a, *(int *)(e + 0xb0), *(int *)(e + 4), 0);
            he_EndEventNow((int)e);
        } else if (*(int *)(e + 0xb0) == id) {
            he_EndEventNow((int)e);
        }
    }
    for (e = he_FindEvent(1, 0, 0x47); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) == 0)
            continue;
        if (*(int *)(e + 0xac) == id || *(int *)(e + 0xb0) == id)
            he_EndEventNow((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x5e); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) != 0 && *(int *)(e + 0xac) == id)
            he_EndEventNow((int)e);
    }
    for (e = he_FindEvent(1, 0, 0x5f); e != 0; e = he_NextEvent()) {
        if ((*(unsigned char *)(e + 0x78) & 3) != 0 && *(int *)(e + 0xac) == id)
            he_EndEventNow((int)e);
    }
}

void cm_SendChrAnim(int a, const char *s, unsigned char c);
int cm_SendChrGotoBuilding();
void cm_SendChrStop(int a);

/* 0x553ae0 (ROUGH): `q` (+0 an object with a scene record at +0x61, +4 the range, +8 the other person) looks for a running record of type 0x43 (no +0xd4 target yet, another +0xc, with one of its four persons +0xc4 .. closer than the range, 3D distance of the scene positions); stores it in q+0xc, sends the four persons (message 0x4c5230, "gestik/herranwinken" for the person itself, 0x4c52e0 "Detected", "gestik/abfaellig" 1 .. 2), sets the target +0xd4 (the other person's id), the due clock now + 8 and the record's message 0x400; 1 = found */
int he_Unk553ae0(int *q)
{
    char *e;
    int best = 0, found = 0, i, h, pp;
    float *a, *b;
    float dx, dy, dz, dist;

    if (q[0] == 0 || *(int *)(q[0] + 0x61) == 0)
        return 0;
    e = he_FindEvent(1, 0, 0x43);
    for (; e != 0; e = he_NextEvent()) {
        if (best == 0) {
            if (*(int *)(e + 0xd4) == -1 && *(int *)(e + 0xc) != *(int *)(q[2] + 0xc)) {
                for (i = 0; i < 4; i++) {
                    h = *(int *)(e + 0xc4 + i * 4);
                    if (h == -1)
                        continue;
                    pp = gm_FindChar(h);
                    found = pp;
                    if (pp == 0)
                        continue;
                    a = (float *)*(int *)(q[0] + 0x61);
                    b = (float *)*(int *)(*(int *)(*(int *)(pp + 0x184) + 0x34));
                    dx = a[0x4c / 4] - b[0x4c / 4];
                    dy = a[0x50 / 4] - b[0x50 / 4];
                    dz = a[0x54 / 4] - b[0x54 / 4];
                    dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
                    if (dist < *(float *)&q[1]) {
                        best = (int)e;
                        break;
                    }
                }
            }
        }
    }
    if (best == 0)
        return 0;
    q[3] = best;
    for (i = 0; i < 4; i++) {
        h = *(int *)(best + 0xc4 + i * 4);
        if (h == -1)
            continue;
        pp = gm_FindChar(h);
        if (pp == 0)
            continue;
        cm_SendChrStop(h);
        if (*(int *)(pp + 4) == h)
            cm_SendChrAnim(h, "gestik/herranwinken", 2);
        cm_SendChrGotoBuilding(h, *(int *)(q[0] + 1), -1, 0, 1, "Detected");
        cm_SendChrAnim(h, "gestik/abfaellig", (unsigned short)util_Random(2) + 1);
    }
    *(int *)(best + 0xd4) = *(int *)(q[2] + 4);
    *(AITIME *)(best + 0x52) = ai_Time;
    gm_AddTime(best + 0x52, 0, 8, 0);
    cm_SendExSetHE(0x400, best);
    return 1;
}
/* 0x553cc0, 1 unless a record of type 0x62 with +0xb0 = the id (+1 of `p`) and +0xb8 == 1 is running (then 0); a null p gives 1 */

int he_Unk553cc0(int p)
{
    char *e;

    if (p == 0)
        return 1;
    for (e = he_FindEvent(1, 0, 0x62); e != 0; e = he_NextEvent()) {
        if (*(int *)(e + 0xb0) == *(int *)(p + 1) && *(int *)(e + 0xb8) == 1)
            return 0;
    }
    return 1;
}
/* 0x553d10, 1 unless a record of type 0x2e with +0xac = the id (+4) of b and +0xb0 = the id of a is running (then 0) */

int he_NoEvent2ePair(int a, int b)
{
    char *e;

    for (e = he_FindEvent(1, 0, 0x2e); e != 0; e = he_NextEvent()) {
        if (*(int *)(b + 4) == *(int *)(e + 0xac) && *(int *)(e + 0xb0) == *(int *)(a + 4))
            return 0;
    }
    return 1;
}
/* 0x553d60, 1 unless a record of type 0x4b with +0xb0 = the id (+4) of b and +0xac = the id of a is running (then 0) */

int he_NoEvent4bPair(int a, int b)
{
    char *e;

    for (e = he_FindEvent(1, 0, 0x4b); e != 0; e = he_NextEvent()) {
        if (*(int *)(b + 4) == *(int *)(e + 0xb0) && *(int *)(a + 4) == *(int *)(e + 0xac))
            return 0;
    }
    return 1;
}
/* 0x553db0, 1 unless a record of type 0x35 of the player *a (kind 2, 2) has +0xbc = the id (+4) of b (then 0); *count (when given) receives the number of records visited */

int he_NoEvent35Pair(unsigned short *a, int b, int *count)
{
    char *e;
    int n = 0;

    for (e = he_FindEvent(2, 2, *a, 0, 0x35); e != 0; e = he_NextEvent()) {
        if (*(int *)(b + 4) == *(int *)(e + 0xbc))
            return 0;
        n++;
    }
    if (count != 0)
        *count = n;
    return 1;
}
/* 0x553e10, sets the due clock of the event record to now and posts it: with the flag 0x6f2448 one unit later, else after the delay (+0x22) of its table record (0x91aaf0, 0x41 bytes each, index +0xac) */

void he_Unk553e10(int ev)
{
    char *rec = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41;

    *(AITIME *)(ev + 0x52) = ai_Time;
    if (U_I((unsigned)&gm_AllowAll) != 0)
        gm_AddTime(ev + 0x52, 0, 0, 1);
    else
        gm_AddTime(ev + 0x52, 0, *(int *)(rec + 0x22), 0);
}

int cm_GetSlotState();
int cm_GetSlotValue(int h);
unsigned char *gm_BuildQuery(int n, ...);
char *gm_Query(void *list, int n, ...);
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);
int gv_OrderOpenSize();
int cm_SendLongMessage(char *msg);
int cm_SendExSysMessage(unsigned char id, void *msg);
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2);

/* 0x553e90, the run handler of a building event (a Ghidra function with a frame; ROUGH): state 0 -> 1; state 1 waits for the due time, takes the building of the kind +0x10 (0x501190 (1, 1, +0x10); none: the record is freed), asks its owner record (0x4ffe90) and starts the order 0x4c4880 (+0xb0, -1, +0xac, 1, player, 0), whose handle goes to +0xb4, state 2; state 2 waits for the order (0x4c3ae0 / 0x4c3b10 give the person), then by the type of the person (0x1f9 / 0x1fd / 0x201: 0x4f0a90 (the owner character, building, 1 / 2 / 3), 0x12d: a message record type 0x2a through 0x4c4b70) tells the owner (a player character gets the news 0x606 / 0x60b with "ob_%s" of the table name 0x91aaf0, texts 0x1c2b / 0x1c2d and "_NACHRICHTEN_HS_53" / "_54") and the town (0x4c4db0 (4, ..)), marks the person (+0x13 |= 0xa0) and frees the record */
void he_Unk553e90(int ev)
{
    int st = *(int *)(ev + 0x70);
    int h, p, b, t, q, own;
    unsigned short ty;
    unsigned char msg[0xf8];
    char text[0x400];
    char name[0x80];
    char buf2[0x80];
    int c[4];
    char *rec;
    int tid;

    if (st == 0) {
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (st == 1) {
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        gm_Query(*(int *)(b + 0x5d), 1, 1, *(int *)(ev + 0xb0));
        *(int *)(ev + 0xb4) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(ev + 0xb0), -1, *(unsigned short *)(ev + 0xac), 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    }
    if (st != 2)
        return;
    if (cm_GetSlotState(*(int *)(ev + 0xb4)) == 0)
        return;
    p = cm_GetSlotValue(*(int *)(ev + 0xb4));
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0 || p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    ty = *(unsigned short *)p;
    if (ty == 0x1f9)
        gv_OrderOpenSize((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c, b, 1);
    else if (ty == 0x1fd)
        gv_OrderOpenSize((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c, b, 2);
    else if (ty == 0x201)
        gv_OrderOpenSize((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c, b, 3);
    else if (ty == 0x12d) {
        memset(msg, 0, sizeof(msg));
        msg[4] = 0x2a;
        *(int *)(msg + 0xc) = *(int *)(b + 1);
        *(int *)(msg + 0x58) = *(int *)(b + 1);
        *(int *)(msg + 8) = *(int *)(U_I((unsigned)&ai_PlayerChar) + 4);
        msg[0x36] = 2;
        cm_SendLongMessage(msg);
    }
    own = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
    if (*(unsigned char *)(own + 2) == 6) {
        q = gm_Query(*(int *)(b + 0x5d), 1, 1, *(int *)(ev + 0xb0));
        rec = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41;
        tid = 0x606;
        if (*(unsigned char *)(*(unsigned char *)b * 0x24d + U_I((unsigned)&ai_BTypes)) != 6) {
            int o = gm_FindCharacterByMaster(b);

            if (o != 0 && *(unsigned char *)(o + 2) == 1 && (*(unsigned char *)(o + 0x1b4) & 1) != 0 && *(int *)(ev + 0xb8) != 0)
                tid = 0x60b;
        }
        if (q != 0 && *(unsigned short *)q != 0xfd) {
            strcpy(name, rec + 1);
            _strupr(name);
            sprintf(buf2, "ob_%s", name);
            txt_Format(text, 0x1c2b, b, *(unsigned short *)(ev + 0xac) + 0x9a8);
            ((void (*)(int, int, char *, char, int, int, int, char *, char *, int))he_MessageBoxQuickjump)(*(int *)(own + 4), *(int *)(ev + 0x10), text, 0, tid, *(int *)(b + 1), *(int *)(ev + 0xb0), buf2, "_NACHRICHTEN_HS_53", 0);
        } else {
            txt_Format(text, 0x1c2d, b, *(unsigned short *)(ev + 0xac) + 0x9a8);
            he_MessageBoxQuickjump(*(int *)(own + 4), *(int *)(ev + 0x10), text, 0, tid, *(int *)(b + 1), -1, 0, "_NACHRICHTEN_HS_54");
        }
        *(unsigned char *)(p + 0x13) |= 0xa0;
    }
    q = gm_Query(*(int *)(b + 0x5d), 1, 1, *(int *)(ev + 0xb0));
    *(AITIME *)c = ai_Time;
    gm_AddTime(c, 0, 0, 1);
    c[0] = *(int *)(b + 1);
    c[1] = *(int *)(q + 2);
    cm_SendExSysMessage(4, c);
    he_FreeEvent((int *)ev);
}

float gm_JobCost();
int gm_GetGoodsValue();
int cm_SendMessage3d(char *p, unsigned char b, int c, int d);
int gm_BuildingMainObject(unsigned char *b);
void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s);
short *gm_PlayerRecord(char *c);

/* 0x554320, the run handler of the sales-tour event (a Ghidra miss; ROUGH): +0xb0 .. +0xdc twelve counters of wares to deliver, +0xac the kind, +0xe4 the order handle, +0xe8 the person's id, +0xe0 the target. State 0 -> 1; state 1 waits for the due time and takes the building of the kind +0x10 (none: free), the price check 0x50cd40 / 0x509e60 (a roll above the building's budget: the news 0x1a40, 0x5f2, free), picks the first counter > 0 (none: free) and orders (0x4c55d0, handle +0xe4), state 2; state 2 waits for the order and the person (0x4c3ae0 / 0x4c3b10; gone: the message 0x62720c and free), takes its id, fixes the target (0x501b50) and sends it (0x4c51b0 .. 0x5f1890), state 3; state 3 hands one ware over (counter - 1, a player gets the news 0x1bab, 0x60c / 0x5f2 with "0x626b04", 0x4c4830 pays, 0x506430 raises a field +0x4c) and goes on to the next counter after a random 0x1e + 0x42ec00(10) units (state 1), or frees the record when all are done; states -2 / -1 free */
void he_Unk554320(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, i, p, r, o, q, ch;
    char text[0x400];

    if (st < -2 || st > 3)
        return;
    switch (st) {
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        return;
    case 0:
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        ch = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
        gm_JobCost(ch, *(unsigned char *)b, -1, 1);
        r = (int)gm_GetGoodsValue(ch);
        if (r < 0) {
            txt_Format(text, 0x1a40, b);
            he_MessageBoxQuickjump(*(int *)(ch + 4), *(int *)(ev + 0x10), text, 0, 0x5f2, *(int *)(b + 1), *(int *)(ev + 0xe0), 0, 0);
            he_FreeEvent((int *)ev);
            return;
        }
        for (i = 0; i < 12; i++) {
            if (*(int *)(ev + 0xb0 + i * 4) > 0)
                break;
        }
        if (i >= 12) {
            he_FreeEvent((int *)ev);
            return;
        }
        *(int *)(ev + 0xe4) = cm_SendMessage3d(b, *(unsigned char *)(ev + 0xac), 0, 0);
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 2:
        if (*(int *)(ev + 0xe4) == -1 || cm_GetSlotState(*(int *)(ev + 0xe4)) == 0)
            return;
        p = cm_GetSlotValue(*(int *)(ev + 0xe4));
        if (p == 0) {
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, "$2AEinwohnerlimit erreicht!", 0, 0x5f2, 0);
            he_FreeEvent((int *)ev);
            return;
        }
        *(int *)(ev + 0xe8) = *(int *)(p + 4);
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        if (*(int *)(ev + 0xe0) == -1) {
            q = gm_BuildingMainObject(b);
            if (q != 0)
                *(int *)(ev + 0xe0) = *(int *)(q + 2);
        }
        cm_RequestChrMoveToUniverse(*(int *)(ev + 0xe8), *(int *)(ev + 0x10), *(int *)(ev + 0xe0), (char *)(unsigned)"dummy_EINGANG");
        *(int *)(ev + 0x70) = 3;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        return;
    case 3:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        p = gm_FindChar(*(int *)(ev + 0xe8));
        for (i = 0; i < 12; i++) {
            if (p != 0 && *(int *)(ev + 0xb0 + i * 4) > 0)
                break;
        }
        if (i < 12) {
            ch = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
            gm_JobCost(ch, *(unsigned char *)b, -1, 1);
            r = (int)gm_GetGoodsValue(ch);
            *(int *)(ev + 0xb0 + i * 4) = *(int *)(ev + 0xb0 + i * 4) - 1;
            o = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
            if (o == 6 || o == 7) {
                q = gm_FindCharacterByMaster(b);
                txt_Format(text, 0x1bab, r, *(unsigned short *)p, b);
                if (q != 0 && (*(unsigned char *)(q + 0x1b4) & 1) != 0)
                    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, 0x60c, *(int *)(b + 1), *(int *)(ev + 0xe0), "_NACHRICHTEN_HS_00", 0);
                else
                    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, 0x5f2, *(int *)(b + 1), *(int *)(ev + 0xe0), "_NACHRICHTEN_HS_00", 0);
            }
            cm_SendMoveGoodsUnchecked(*(int *)(p + 4), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, r);
            q = gm_PlayerRecord((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c);
            if (q != 0)
                *(int *)(q + 0x4c) = *(int *)(q + 0x4c) + r;
        }
        for (i = 0; i < 12; i++) {
            if (*(int *)(ev + 0xb0 + i * 4) > 0)
                break;
        }
        if (i >= 12) {
            he_FreeEvent((int *)ev);
            return;
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(10) + 0x1e, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    }
}
/* 0x554820, the allocation handler of the 0x554860 event: the due clock is the start clock (+0x44) and the event posted 0xf later */

void he_Unk554820(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 0xf, 0);
}
/* 0x554860, the run handler of the single-ware variant of 0x554320 (ROUGH): one counter +0xb0, +0xac the ware, +0xb4 the target building (0 = the first of the kind +0x29, -1 = kind 0xfd), +0xb8 the order handle, +0xbc the person's id. State 0 -> 1; state 1 orders the delivery (0x4c55d0 (building, 1, 0, the ware's price byte +0x223 of the kind record)), state 2 sends the person (0x4c51b0, news 0x1bb1 / "0x627228" to a player, payment 0x4c4830) and posts the next round 10 + random 10 later (state 1); states -2 / -1 free */

void he_Unk554860(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, p, r, q, ch, kind, tgt, n;
    char text[0x400];

    if (st < -2 || st > 2)
        return;
    switch (st) {
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        return;
    case 0:
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        ch = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
        gm_JobCost(ch, *(unsigned char *)b, *(unsigned char *)(ev + 0xac), -1);
        r = (int)gm_GetGoodsValue(ch);
        kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
        if (r < 0) {
            txt_Format(text, 0x1a41, b);
            he_MessageBoxQuickjump(*(int *)(ch + 4), *(int *)(ev + 0x10), text, 0, 0x5f2, *(int *)(b + 1), *(int *)(ev + 0xb4), 0, 0);
            he_FreeEvent((int *)ev);
            return;
        }
        if (*(int *)(ev + 0xb0) <= 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        *(int *)(ev + 0xb8) = cm_SendMessage3d(b, 1, 0, *(unsigned char *)(kind + *(unsigned char *)(ev + 0xac) + 0x223));
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 2:
        if (cm_GetSlotState(*(int *)(ev + 0xb8)) == 0)
            return;
        p = cm_GetSlotValue(*(int *)(ev + 0xb8));
        if (p == 0) {
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, "Einwohnerlimit erreicht!", 0, 0x5f8, 0);
            he_FreeEvent((int *)ev);
            return;
        }
        U_I((unsigned)&ai_PermsDirty) = 1;
        *(int *)(ev + 0xbc) = *(int *)(p + 4);
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
        p = cm_GetSlotValue(*(int *)(ev + 0xb8));
        *(unsigned char *)(p + 0x164) = *(unsigned char *)(kind + *(unsigned char *)(ev + 0xac) + 0x223);
        *(unsigned char *)(p + 0x165) = 0;
        if (*(int *)(ev + 0xb0) > 0) {
            ch = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
            gm_JobCost(ch, *(unsigned char *)b, *(unsigned char *)(ev + 0xac), -1);
            n = (int)gm_GetGoodsValue(ch);
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
            q = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
            if (q == 6 || q == 7) {
                txt_Format(text, 0x1bb1, n, *(unsigned short *)p, b);
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, 0x5f2, *(int *)(b + 1), *(int *)(ev + 0xb4), "_NACHRICHTEN_HS_03", 0);
            }
            cm_SendMoveGoodsUnchecked(*(int *)(p + 4), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, n);
        }
        tgt = *(int *)(ev + 0xb4);
        if (tgt == 0) {
            q = gm_Query(*(int *)(b + 0x5d), 1, 0, *(unsigned short *)(b + 0x29));
            tgt = q != 0 ? *(int *)(q + 2) : -1;
        } else if (tgt == -1) {
            q = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
            tgt = q != 0 ? *(int *)(q + 2) : -1;
        }
        cm_RequestChrMoveToUniverse(*(int *)(ev + 0xbc), *(int *)(ev + 0x10), tgt, (char *)(unsigned)"dummy_EINGANG");
        if (*(int *)(ev + 0xb0) <= 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(10) + 10, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    }
}

int gm_IsHomeType();
char *gm_FindBuildingById(int id);
int gm_BuildingSkill();
int gm_StallGoodsList(char *c, unsigned *out);
int gm_LevelPayout(int v, int kind);

/* 0x554cb0, the allocation handler of the two-building event (a Ghidra miss; ROUGH, the tail is only sketched): the clocks +0x52 / +0x60 are the start clock, +0xe8 = -1, the buildings +0xb0 / +0xb4 (0x501190) and the person +0xac (0x4fea50) are looked up (any missing: free the record); when the second building is of the kind 0xa and the person has flag 1 (+0x20) the nearest building of the kind 5 / 0xa list (0x501190 (1, 5, 0xa), 0x458a20 positions, start range 1e8) replaces it in all records of type +0xf4 (+0xb8 .. / +0xb0) and +0xb4; the two buildings' 0x502520 flags choose the mode +0xc8 (1 / 2 / 3, 4 when both: equal buildings = error text 0x62728c and free), +0xb8 = the sum of both +0x30, +0xc4 = the first building's id, the event is posted 1 later and the person's data (0x502020, 0x5092e0, the +0x1a0 factor from the person's byte +0x12 with the type 0x135 / 0x136 correction) is set up; flag 2 of +0x78 sends the person the message 0x13 / 0x40, the person gets +0x24 = ev and +0x10 = its id; with flag 1 the 0x50aa00 / 0x627258 text decides a count +0xcc (<= the person's limit) and its tag +0x28 (0x50a970), the handle +0x84 / +0xec = -1 */
void he_Unk554cb0(int ev)
{
    int b1, b2, o = 0, best, e, f1 = 0, f2 = 0, mode;
    float mind, dist;
    float p1[3], p2[3];
    char tmp[0x100];

    *(int *)(ev + 0xe8) = -1;
    *(int *)(ev + 0xd4) = 0;
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(unsigned char *)(ev + 0xd0) = 0;
    *(AITIME *)(ev + 0x60) = *(AITIME *)(ev + 0x44);
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    if (b1 == 0)
        goto fail;
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o == 0)
        goto fail;
    b2 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    if (b2 == 0)
        goto fail;
    if (*(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b2 * 0x24d) == 0xa && (*(unsigned char *)(o + 0x20) & 1) != 0) {
        best = b2;
        mind = 1e8f;
        d3_PointToWorld(*(int *)(b1 + 0x61), *(int *)(b1 + 0x61) + 0x4c, p1);
        for (e = gm_BuildQuery(1, 5, 0xa); e != 0; e = gm_BuildNext()) {
            float dx, dy, dz;

            d3_PointToWorld(*(int *)(e + 0x61), *(int *)(e + 0x61) + 0x4c, p2);
            dx = p2[0] - p1[0];
            dy = p2[1] - p1[1];
            dz = p2[2] - p1[2];
            dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
            if (!(dist > mind)) {
                mind = dist;
                best = e;
            }
        }
        if (best != b2) {
            if (*(int *)(ev + 0xf4) != 0 && *(int *)(ev + 0xf4) != -1) {
                char *r = he_FindEvent(1, 1, *(int *)(ev + 0xf4));
                int k;

                if (r != 0) {
                    for (k = 0; k < 3; k++) {
                        if (*(int *)(r + 0xc4 + k * 4) == *(int *)(b2 + 1))
                            *(int *)(r + 0xc4 + k * 4) = *(int *)(best + 1);
                    }
                    if (*(int *)(r + 0xb0) == *(int *)(b2 + 1))
                        *(int *)(r + 0xb0) = *(int *)(best + 1);
                }
            }
            *(int *)(ev + 0xb4) = *(int *)(best + 1);
            b2 = best;
        }
    }
    if (gm_IsHomeType(b1) != 0)
        f1 = 1;
    if (gm_IsHomeType(b2) != 0)
        f2 = 1;
    mode = f1 ? (f2 ? 4 : 2) : (f2 ? 1 : 3);
    *(int *)(ev + 0xc8) = mode;
    if (mode == 4 && b1 == b2) {
        err_Message("he_AllocTransport(): Start and goal are the same!");
        he_FreeEvent((int *)ev);
        return;
    }
    *(int *)(ev + 0xb8) = *(int *)(b2 + 0x30) + *(int *)(b1 + 0x30);
    *(int *)(ev + 0xc4) = *(int *)(b1 + 1);
    *(unsigned char *)(ev + 0xd0) = 0;
    gm_AddTime(ev + 0x52, 0, 0, 1);
    gm_FindBuildingById(*(int *)(o + 0x1c));
    *(unsigned char *)(o + 0x13) |= 0x40;
    if ((*(unsigned char *)(ev + 0x78) & 2) != 0)
        cm_SendSetField(*(int *)(o + 2), 0x13, 1, 0x40, 0);
    *(int *)(o + 0x24) = ev;
    *(int *)(ev + 0x10) = *(int *)(o + 2);
    if ((*(unsigned char *)(o + 0x20) & 1) != 0) {
        gm_StallGoodsList(o, tmp);
        sprintf(tmp, "%s", "");
        *(unsigned char *)(o + 0x28) = *(unsigned char *)(ev + 0xcc);
        gm_LevelPayoutK(*(int *)(ev + 0xcc), *(char *)(o + 0x28));
    }
    *(int *)(ev + 0x84) = -1;
    *(int *)(ev + 0xec) = -1;
    return;
fail:
    he_FreeEvent((int *)ev);
}
/* 0x5551c0, an object callback of 0x438d40: objects whose name contains "dummy_TRANSPORTER" are collected into the context (32 pointers, +0x80 the count); returns 1 while there is room */


char he_CollectTransporter(char *name, int *ctx)
{
    if (strstr(name, "dummy_TRANSPORTER") != 0) {
        ctx[ctx[0x20]] = (int)name;
        ctx[0x20] = ctx[0x20] + 1;
    }
    return ctx[0x20] < 0x20;
}
/* 0x555210 (ROUGH), collects the transporter dummies of the object `obj` (0x438d40 (scene 0x5fb5b0, obj, 0x100, 0x5551c0, list), the object's +0x1f0 is cleared during the walk unless flag 1 of +0x210 is set) and returns a random one of them (0x42ec00 (n) with the list's count n), 0 when n = 0; one parameter (every caller pushes one) */

int he_RandomTransporter(char *obj)
{
    int list[0x21];
    int save;

    memset(list, 0, sizeof(list));
    list[0x20] = 0;
    if (obj != 0 && (*(unsigned char *)(obj + 0x210) & 1) == 0) {
        save = *(int *)(obj + 0x1f0);
        *(int *)(obj + 0x1f0) = 0;
        d3_ForEachObject(d3_DefaultScene, obj, 0x100, (void *)he_CollectTransporter, list);
        *(int *)(obj + 0x1f0) = save;
    } else {
        d3_ForEachObject(d3_DefaultScene, obj, 0x100, (void *)he_CollectTransporter, list);
    }
    if (list[0x20] == 0)
        return 0;
    return list[(unsigned short)util_Random(list[0x20])];
}
/* 0x5552c0, sets the speed factor (+0x1a0 of the record at +0x3b of `p`) from the event's value +0xc0: * the first constant when 0x76061c points to a value above 100, else * the second when 0x760618 points above 200, else unchanged; a second stage (+0xf1 == 2) multiplies it once more */

void he_Unk5552c0(int ev, int p)
{
    int rec = *(int *)(p + 0x3b);

    if (U_I((unsigned)&d3t_Snow) != 0 && *(int *)U_I((unsigned)&d3t_Snow) > 0x64)
        *(float *)(rec + 0x1a0) = (float)(*(float *)(ev + 0xc0) * 0.7);
    else if (U_I((unsigned)&d3t_Rain) != 0 && *(int *)U_I((unsigned)&d3t_Rain) > 0xc8)
        *(float *)(rec + 0x1a0) = (float)(*(float *)(ev + 0xc0) * 0.8);
    else
        *(int *)(rec + 0x1a0) = *(int *)(ev + 0xc0);
    if (*(unsigned char *)(ev + 0xf1) == 2)
        *(float *)(rec + 0x1a0) = (float)(*(float *)(rec + 0x1a0) * 0.8);
}

int gm_FindObjectInst(int id);
void ai_Unk4c4920(int a, int b, int c);
void ai_Unk4c5260(int a, int b);
int gm_IsType10(unsigned char *b);
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);

/* 0x555350, the run handler of a transport event (a Ghidra miss; ROUGH): +0xb0 the source building, +0xb4 the target building, +0xac the transport id (0x4fe980 gives the transporter object in the scene 0x633444), +0xd0 started flag, +0xd4 a tick counter, +0xd8 .. the last position, +0xec the guide handle, +0xf2 silent flag. First round: the transporter is sent (0x4c5230, 0x4c52e0 "Trans", the guide 0x4c4920, +0xd0 = 1); later rounds check whether it arrived (the position change 0x453e80 against 100.0, or more than 6 rounds): then the guide is dropped (0x4c5260), the transporter gets the message 0x13 / 0x40, the news (0x1c43 / 0x1c44 / 0x1c39 / 0x1c41, by the building kind 0x1d / 0x1f and the 0x5024f0 flag) is formatted and shown to a player (0x53dee0 .., the text 0x608) unless the world map is in a mode (0x6f229c, 0x6f2310 ranges) and the event is posted again with the handle 3 (0x4c4c60 (3, ev), +0x84) */
void he_Unk555350(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int b0, b1, tr, kind, e, f;
    int a44, a45;
    int o, q;
    char text[0x400];
    int kd;

    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    kd = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b0 * 0x24d;
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    tr = gm_FindObjectInst(*(int *)(ev + 0xac));
    (void)a44;
    (void)a45;
    if (tr != 0) {
        he_Unk5552c0(ev, tr);
        if (*(unsigned char *)(ev + 0xd0) == 0) {
            cm_SendChrStop(*(int *)(tr + 2));
            cm_SendChrGotoBuilding(*(int *)(tr + 2), *(int *)(b1 + 1), -1, 0, 1, "Trans");
            *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b0 + 1), *(int *)(tr + 2));
            *(unsigned char *)(ev + 0xd0) = 1;
        } else if (*(int *)(tr + 0x3b) != 0 && *(int *)(*(int *)(tr + 0x3b) + 0x128) == 0) {
            o = *(int *)(tr + 0x3b);
            if ((*(int *)(o + 0x88) == (unsigned)&d3_Scenes && d3_VecEqual(*(int *)(o + 0x34) + 0x4c, ev + 0xd8, 100.0f) != 0) || *(int *)(ev + 0xd4) > 6) {
                if (gm_IsType10(b1) == 0) {
                    *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(b1 + 1), *(int *)(a45 + 1), *(int *)(tr + 2));
                    ai_Unk4c5260(*(int *)(tr + 2), 0);
                }
                cm_SendSetField(*(int *)(tr + 2), 0x13, 1, 0, 0x40);
                kind = *(unsigned char *)kd;
                if (gm_IsType10(b1) != 0)
                    txt_Format(text, 0x1c43);
                else if (kind != 0x1d && kind != 0x1f)
                    txt_Format(text, 0x1c41, 0x1c39, b0, *(unsigned short *)(b1 + 0x27), b1);
                else if (gm_IsType10(b1) != 0)
                    txt_Format(text, 0x1c43);
                else
                    txt_Format(text, 0x1c44, b1);
                e = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
                if (e == 6 || e == 7) {
                    f = 0;
                    if (U_I(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (unsigned)&ai_TransportWindow) {
                        if (U_I((unsigned)&ai_PlayerBuilding) == b1)
                            f = 1;
                        else if (gm_IsType10(b1) != 0 && U_I((unsigned)&sv_PanelShown) != 0) {
                            int w = *(unsigned short *)U_I((unsigned)&sv_PanelShown);

                            if ((w >= 0x92 && w <= 0x97) || w == 0x2db)
                                f = 1;
                        }
                    }
                    if (f == 0 && (*(unsigned char *)(tr + 0x20) & 1) == 0 && *(unsigned char *)(ev + 0xf2) == 0) {
                        q = gm_BuildingMainObject(b1);
                        if (q != 0)
                            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(tr + 2), text, 0, 0x608, *(int *)(b1 + 1), *(int *)(q + 2), 0, 0);
                        else
                            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(tr + 2), text, 0, 0x608, *(int *)(b1 + 1), -1, 0, 0);
                    }
                }
                *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
            }
        }
    }
    d3if_SelectScene(sc, 1);
}

int sv_IsCartAtDoor();

/* 0x555690, the run handler of the return leg of the transport event (a Ghidra miss; ROUGH, same record as 0x555350): the first round stops the transporter (0x4c5230), puts it on the road (0x4c5260 (id, 1) unless the target is a 0x5024f0 building), sends it home ("Trans", 0x4c52e0 (id, b1 id, -1, 0, 0, ..)) and starts the guide 0x4c4920; later rounds check the arrival (0x524810 (transporter, b1)), restart the guide and format the arrival news (0x1c39 + 0x1c3b / 0x1c3d / 0x1c3f / 0x1c43 / 0x1c44 by the kinds 0x47 / 0x1d .. 0x20 of the two buildings and the player's name 0x7e3f40) which a player character gets through 0x53dee0 (text 0x609, the strings 0x627318 / 0x627304) unless the world map shows the building (0x6f229c / 0x6f230c); the event is then posted again with the handle 3 */
void he_Unk555690(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int a44, a45, b0, b1, tr, e;
    int skip;
    char text[0x400];

    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    tr = gm_FindObjectInst(*(int *)(ev + 0xac));
    (void)a45;
    if (tr != 0) {
        he_Unk5552c0(ev, tr);
        if (*(unsigned char *)(ev + 0xd0) == 0) {
            cm_SendChrStop(*(int *)(tr + 2));
            if (gm_IsType10(b0) == 0)
                ai_Unk4c5260(*(int *)(tr + 2), 1);
            cm_SendChrGotoBuilding(*(int *)(tr + 2), *(int *)(b1 + 1), -1, 0, 0, "Trans");
            if (gm_IsType10(b0) == 0)
                *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b0 + 1), *(int *)(tr + 2));
            *(unsigned char *)(ev + 0xd0) = 1;
        } else if (sv_IsCartAtDoor(tr, b1) != 0) {
            *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b1 + 1), *(int *)(tr + 2));
            if (*(unsigned char *)b1 == 0x47) {
                txt_Format(text, 0x1c3b, 1, 0x1c39, (char *)(unsigned)&ai_PlayerSlots + *(int *)(b1 + 0x65) * 0x2f4);
                skip = U_I(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (unsigned)&ai_TransportWindow && U_I((unsigned)&ai_FallbackBuilding) == b1;
                if (!skip)
                    he_MessageBoxQuickjump(0, -1, text, 0, 0x609, 0, 0, "_NACHRICHTEN_HS_59", 0);
            } else if (*(unsigned char *)b0 == 0x47) {
                txt_Format(text, 0x1c3d, 1, 0x1c39);
                skip = U_I(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (unsigned)&ai_TransportWindow && U_I((unsigned)&ai_FallbackBuilding) == b1;
                if (!skip)
                    he_MessageBoxQuickjump(0, -1, text, 0, 0x609, 0, 0, "_NACHRICHTEN_HS_60", 0);
            } else {
                e = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b1 * 0x24d);
                if (e == 0x1d || e == 0x1f) {
                    if (gm_IsType10(b1) != 0)
                        txt_Format(text, 0x1c43);
                    else
                        txt_Format(text, 0x1c44, b1);
                } else {
                    txt_Format(text, 0x1c3f, 1, 0x1c39);
                }
                skip = U_I(((unsigned)&ai_Unk75f280 + 0x500) + U_I((unsigned)&sv_FrameDepth) * 8) == (unsigned)&ai_TransportWindow && U_I((unsigned)&ai_FallbackBuilding) == b1;
                if (!skip)
                    he_MessageBoxQuickjump(0, -1, text, 0, 0x609, 0, 0, 0, 0);
            }
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        }
    }
    d3if_SelectScene(sc, 1);
}

struct D3HMAP * ch_GetHMap();
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);
void cm_SendChrWalkTo(int a, int b, int c, int d);

/* 0x555b60, the run handler "he_TransportStadtStadt()" of the city-to-city transport (the error text "character-action not attached but target should be reached : %s" is in the 0x627330 literal; ROUGH, outlined from the disassembly): +0xb0 / +0xb4 the buildings (0x501190 (1, 1, id)), +0xc4 the id of the building the cart is at, +0xd0 started, +0xd4 rounds, +0xac the cart's id. Before the start (+0xc4 = the first building's id, +0xd0 = 0): a start position is taken (the building's transporter dummy 0x555210 or its +0x61 object, 0x458a20, the ground 0x45f170), the cart object is looked up (0x4ffe90 (building object, 1, 1, +0xac), else 0x4fe980) and put on the road (0x4c5260, 0x4c5100, the guide 0x4c4920) and +0xd0 = 1; with +0xc4 = the second building's id the cart's arrival (0x453e80 against the target, or more than 6 rounds) ends the trip: the guide is dropped, the cart gets the message 0x13 / 0x40, the arrival news is formatted (0x1c39 .. 0x1c44 by kind) and shown to a player character (0x53dee0, unless the world map shows the building: 0x6f229c / 0x6f2308 / 0x6f2310 with the ranges 0x92 .. 0x97 and 0x2db), the event is posted with the handle 3; the cart not found while it should have arrived gives the debug error 0x4222b0 */
void he_Unk555b60(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int a44, a45, b0, b1, cart, o, e;
    float pos[3];
    char text[0x400];
    char err[0x200];

    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    if (*(int *)(ev + 0xc4) == *(int *)(b0 + 1) && *(unsigned char *)(ev + 0xd0) == 0) {
        d3if_SelectScene(0, 1);
        o = *(int *)(b1 + 0x61);
        d3_PointToWorld(o, o + 0x4c, pos);
        if (gm_IsType10(b0) != 0)
            cart = gm_Query(*(int *)(a44 + 0x5d), 1, 1, *(int *)(ev + 0xac));
        else
            cart = gm_Query(*(int *)(b0 + 0x5d), 1, 1, *(int *)(ev + 0xac));
        if (cart == 0)
            cart = gm_FindObjectInst(*(int *)(ev + 0xac));
        if (cart != 0) {
            he_Unk5552c0(ev, cart);
            ch_GetHMap(*(int *)(cart + 0x3b));
            d3_GetHeightAt(U_I((unsigned)&ai_Unkfd7c50), pos, 0, 0, 0);
            if (gm_IsType10(b0) == 0) {
                ai_Unk4c5260(*(int *)(cart + 2), 0);
                ((void (*)(int, int))cm_SendChrWalkTo)(*(int *)(cart + 2), pos);
                if (gm_IsType10(b0) == 0)
                    *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b1 + 1), *(int *)(cart + 2));
            }
            *(unsigned char *)(ev + 0xd0) = 1;
        }
        d3if_SelectScene(sc, 1);
        return;
    }
    if (*(int *)(ev + 0xc4) != *(int *)(b1 + 1))
        return;
    cart = gm_Query(*(int *)(b1 + 0x5d), 1, 1, *(int *)(ev + 0xac));                 /* 0x555daf */
    if (cart == 0)
        cart = gm_Query(cart, 2, 7, 1, *(int *)(ev + 0xac));                        /* 0x555dcb: (0, 2, 7, 1, +0xac) */
    if (cart == 0) {
        __asm int 3                                                                     /* 0x556055: the original traps here, repeats the query and returns */
        gm_Query(0, 2, 7, 1, *(int *)(ev + 0xac));
        return;
    }
    he_Unk5552c0(ev, cart);
    if (*(unsigned char *)(cart + 9) == 0x2d || (d3_VecEqual(*(int *)(*(int *)(cart + 0x3b) + 0x34) + 0x4c, ev + 0xd8, 100.0f) != 0) || *(int *)(ev + 0xd4) > 6) {
        cm_SendSetField(*(int *)(cart + 2), 0x13, 1, 0, 0x40);
        if (gm_IsType10(b1) == 0)
            *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(b1 + 1), *(int *)(a45 + 1), *(int *)(cart + 2));
        if (gm_IsType10(b1) != 0)
            txt_Format(text, 0x1c43);
        else
            txt_Format(text, 0x1c44, b1);
        e = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
        if ((e == 6 || e == 7) && U_I((unsigned)&ai_PlayerBuilding) != b1)
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(cart + 2), text, 0, 0x608, *(int *)(b1 + 1), -1, 0, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
    } else {
        /* 0x556022..0x556054 (ROUGH: the name index is a guess): the target should have been reached */
        sprintf(err, "he_TransportStadtStadt(): character-action not attached but target should be reached : %s",
                *(char **)((unsigned)&full_data_143b3f8 + *(unsigned char *)b1 * 7 * 8));
    }
}

int init_BuildingSceneUniverse();
void ai_Unk4c5a70(int a);
void cm_BeginFieldBlock(char *o, int id);

/* 0x556080, the fourth leg of the transport event (mode 4 of +0xc8, called by the run handler 0x556600; ROUGH, a skeleton from the call sequence): like 0x555350 / 0x555690 / 0x555b60 the buildings +0xb0 / +0xb4 and the cart +0xac are looked up (0x501190 x4, 0x4fe980); the first round sends the cart (0x4c5230, "Trans" 0x4c52e0, the guide 0x4c4920), later rounds wait for the arrival (0x524810) and send it on (0x4c51b0, scene select 0x511e60), put it at the entry dummy "dummy_TRANSPORTER_ENTRY" (0x555210 random dummy, ground 0x45f170, 0x4c5100, message 0x13 / 0x40) and tell the player (news 0x1c39 .. by 0x1c3b / 0x1c3d, "_NACHRICHTEN_HS_59" / "_NACHRICHTEN_HS_60", 0x53dee0); the event is posted with the handle 3 and the scene reselected (0x4478e0) */
void he_Unk556080(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int a44, a45, b0, b1, cart, o, e;
    float pos[3];
    char text[0x400];

    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    cart = gm_FindObjectInst(*(int *)(ev + 0xac));
    (void)a45;
    if (cart == 0) {
        d3if_SelectScene(sc, 1);
        return;
    }
    he_Unk5552c0(ev, cart);
    if (*(unsigned char *)(ev + 0xd0) == 0) {
        cm_SendChrStop(*(int *)(cart + 2));
        cm_SendChrGotoBuilding(*(int *)(cart + 2), *(int *)(b1 + 1), -1, 0, 0, "Trans");
        *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b0 + 1), *(int *)(cart + 2));
        *(unsigned char *)(ev + 0xd0) = 1;
        d3if_SelectScene(sc, 1);
        return;
    }
    if (sv_IsCartAtDoor(cart, b0) != 0) {
        *(int *)(ev + 0xec) = ((int (*)(int, int, int))ai_Unk4c4920)(*(int *)(a44 + 1), *(int *)(b1 + 1), *(int *)(cart + 2));
        cm_RequestChrMoveToUniverse(*(int *)(cart + 2), *(int *)(b1 + 1), -1, (char *)0);
        init_BuildingSceneUniverse(b1);
        d3if_SelectScene(0, 1);
        o = he_RandomTransporter(0);  /* the original passes 0 (push 0 at 0x5561a2), not b1 */
        if (o != 0) {
            d3_PointToWorld(o, o + 0x4c, pos);
            d3_GetHeightAt(U_I((unsigned)&ai_Unkfd7c50), pos, 0, 0, 0);
            ((void (*)(int, int))cm_SendChrWalkTo)(*(int *)(cart + 2), pos);
        }
        cm_SendSetField(*(int *)(cart + 2), 0x13, 1, 0, 0x40);
        e = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c);
        if (e == 6 || e == 7) {
            txt_Format(text, 0x1c3b, 1, 0x1c39);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(cart + 2), text, 0, 0x609, 0, 0, (char *)"_NACHRICHTEN_HS_59", 0);
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        cm_SendChrGotoBuilding(*(int *)(cart + 2), *(int *)(b1 + 1), -1, 0, 0, "Trans");
    }
    d3if_SelectScene(sc, 1);
}
/* 0x556600, the run handler "he_RunTransport" of the transport events (ROUGH, a skeleton: the jump table 0x556d94 has 8 states): a flagged (2) record with a pending handle +0x84 waits for it (0x4c3ae0); the buildings +0xb0 / +0xb4 (0x501190 x4), the cart +0xac (0x4fe980) are looked up (missing cart: the event is freed, buildings: nothing). State 0 posts the record 3 later with the handle 4, state 1 waits for the due time and the two handles, then runs the leg by the mode +0xc8 (1: 0x555690, 2: 0x555350, 3: 0x555b60, 4: 0x556080; else the debug text "he_RunTransport: invalid type!" 0x4222b0), a random chance (0x42ec00) lets the cart play the cart noise "Karrenrumpeln_r" (0x4c5a70 / 0x53dda0 with the news 0x1c2f / .., 0x4c5a70), the later states hand over goods (0x4c4db0, sb_PlaySample 0x426fa0 "Karrenrumpeln_r", 0x509 .. 0x4bfa40 / 0x4bfc60 / 0x4bda80 / 0x59e170 / 0x541250 / 0x50ca50 / 0x472ce0) and end the transport (state -1 / 2: the cart's flag 0x40 / 0x20 cleared, +0x24 = 0, free). The jump-table cases are only outlined by the helpers 0x556b50, 0x556be0 .. above them */

void he_Unk556600(int ev)
{
    int b0, b1, a44, a45, cart, mode, st;
    int h;

    if ((*(unsigned char *)(ev + 0x78) & 2) != 0 && *(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    a44 = gm_BuildQuery(1, 0, 0x44);
    a45 = gm_BuildQuery(1, 0, 0x45);
    b0 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
    b1 = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    cart = gm_FindObjectInst(*(int *)(ev + 0xac));
    if (a44 == 0 || a45 == 0 || b0 == 0 || b1 == 0)
        return;
    if (cart == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    st = *(int *)(ev + 0x70);
    if (st > 7)
        return;
    switch (st) {
    case 0:
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 3, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) != 0)
            return;
        h = *(int *)(ev + 0xec);
        if (h != -1 && cm_GetSlotState(h) == 0)
            return;
        h = *(int *)(ev + 0xe8);
        if (h != -1 && cm_GetSlotState(h) == 2) {
            he_FreeEvent((int *)ev);
            return;
        }
        mode = *(int *)(ev + 0xc8);
        if (mode == 2)
            he_Unk555350(ev);
        else if (mode == 1)
            he_Unk555690(ev);
        else if (mode == 3)
            he_Unk555b60(ev);
        else if (mode == 4)
            he_Unk556080(ev);
        else
            err_Message("he_RunTransport: invalid type!");
        return;
    default:
        he_FreeEvent((int *)ev);
        return;
    }
}

int gm_MinutesBetween();

/* 0x556dc0, the allocation handler of the delivery event (ROUGH): the clock +0xc0 = the start clock + (+0xb8 * +0xb0) units (the duration), the due clock (+0x52) is the start clock + 1, the person +0xac (0x4fea50) is freed from an older job (flag 0x20 of +0x13 set: the record is freed) or gets the message 0x13 / 0x20, +0x60 = now, +0xb4 = min(+0xb0 + the person's byte +0x12, 100) */
void he_Unk556dc0(int ev)
{
    int o = 0, n;

    *(AITIME *)(ev + 0xc0) = *(AITIME *)(ev + 0x44);
    gm_AddTime(ev + 0xc0, 0, *(int *)(ev + 0xb8) * *(int *)(ev + 0xb0), 0);
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 0, 1);
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o != 0) {
        if ((*(unsigned char *)(o + 0x13) & 0x20) != 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        cm_SendSetField(*(int *)(o + 2), 0x13, 1, 0x20, 0);
    }
    *(AITIME *)(ev + 0x60) = ai_Time;
    n = *(unsigned char *)(o + 0x12) + *(int *)(ev + 0xb0);
    if (n >= 0x64)
        n = 0x64;
    *(int *)(ev + 0xb4) = n;
}
/* 0x556ed0, the run handler of the delivery event (a Ghidra function with a jump table 0x557150 for 4 states; ROUGH): state 0 waits for the duration clock (0x4fe280 against +0xc0), asks the buildings of the person (0x4ffe90, none: free), state 1 / 2 hands the goods over and tells the owner (0x50ca50, the news 0x1c49 and 0x6273bc text, 0x53dee0 with 0x5ec, the person gets 0x4c4a60 / 0x4c3ba0 / 0x4c3c00 / 0x4c49a0 and the record is freed), the last state frees the person's flag 0x13 / 0x20 and the record */

void he_Unk556ed0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int o = 0, b, q;
    char text[0x100];

    if (st < 0 || st > 3)
        return;
    switch (st) {
    case 0:
        if (gm_MinutesBetween((unsigned)&ai_Time, ev + 0xc0) < 0)
            return;
        b = gm_Query(0, 1, 0, *(int *)(ev + 0xac));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
    case 2:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
        if (o == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        txt_Format(text, 0x1c49);
        gm_FindBuildingById(*(int *)(o + 0x1c));
        q = gm_BuildingMainObject(o);
        if (q != 0)
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(o + 1), *(int *)(q + 2), "_NACHRICHTEN_HS_55", 0);
        else
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(o + 1), -1, 0, 0);
        cm_SendSetField(*(int *)(o + 2), 0x13, 1, 0, 0x20);
        he_FreeEvent((int *)ev);
        return;
    case 3:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
        if (o != 0)
            cm_SendSetField(*(int *)(o + 2), 0x13, 1, 0, 0x20);
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x557160, the allocation handler of a player event: state 0, +0xec = 0; a figure that is not a player character (kind 6) frees the record at once */

void he_Unk557160(int ev)
{
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0xec) = 0;
    if (*(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) != 6)
        he_FreeEvent((int *)ev);
}

int cm_PartyById();
int ai_Unk4e5210();
int cm_SendLeaveParty(int a, int b);
void sb_Stop();
void sv_SelectPanelObject(unsigned char *o, int b, char *c);
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay);

/* 0x557190, the run handler of the event message box (only for the player's figure while a game runs: 0x6f25b8 / 0x6f25ac; ROUGH skeleton, the flags of +0xf0: 1 = quick jump, 2 = big, 4 = cutscene, 8 = scene window, 0x10 = speech (the text +0x7c is spoken 0x4c27d0 with the shape +0xec), 0x20 = history, 0x40 = help window, else the plain box): state 0 opens the window of the flavour "misc\EventMessagebox_QJ_BIG" / "_QJ" / "_BIG" / "EventMsgbox_History" / "EventMessagebox_Cutscenes" / "EventMessagebox_Help" / "EventMessagebox" (0x53d6f0 (ev, 0, 0, name, +0xac)), shows the news 0x1ba1 / 0x1ba2 / 0x1ba3 / 0x7d and the text +0x7c (0x515960 "%s"), plays the sound "NeueMessage_s_01" (0x426fa0) unless 0x6f25ac == 2, and sets both clocks (+0x44 / +0x52) to now (the due clock +0x14 hours for a quick jump, the minute word 6); state 1 waits for the due time (0x4fe210) or the click and then carries out the quick jump (0x4e50a0 / 0x4e5210 / 0x4c4fd0 / 0x521960 on the contact +0xb0 / +0xb4) and closes the window (0x53d800, 0x410620 / 0x427290 for the help windows); states -2 / -1 free the record */
void he_Unk557190(int ev)
{
    int st = *(int *)(ev + 0x70);
    int pl = 0, fl, b, w;
    char speak[0x7d0];

    if (*(unsigned short *)(unsigned)&ai_PlayerId != *(unsigned short *)(ev + 8) || *(unsigned char *)(unsigned)&ai_GameState == 0)
        return;
    fl = *(unsigned char *)(ev + 0xf0);
    switch (st) {
    case -2:
    case -1:
        if ((fl & 2) != 0) {
            if (U_I((unsigned)&ai_PlayerBuilding) != 0)
                pl = U_I((unsigned)&ai_PlayerBuilding);
            else if (U_I((unsigned)&ai_FallbackBuilding) != 0)
                pl = U_I((unsigned)&ai_FallbackBuilding);
            he_OpenEventSlot(ev, 0, 0, (int)((fl & 1) ? "misc\EventMessagebox_QJ_BIG" : "misc\EventMessagebox_QJ"), *(int *)(ev + 0xac));
            if (*(int *)(ev + 0x74) == 0) {
                he_FreeEvent((int *)ev);
                return;
            }
            d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
            if (pl != 0 && *(int *)(pl + 1) == *(int *)(ev + 0xb0) && *(unsigned char *)(ev + 0xbc) == 0)
                io_PrintText(0x1ba2);
            else
                io_PrintText(0x1ba1);
        } else if ((fl & 1) != 0) {
            he_OpenEventSlot(ev, 0, 0, (int)"misc\EventMessagebox_BIG", *(int *)(ev + 0xac));
            if (*(int *)(ev + 0x74) != 0) {
                d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
                io_PrintText(0x1ba3);
            }
        } else if ((fl & 0x20) != 0) {
            he_OpenEventSlot(ev, 0, 0, (int)"misc\EventMsgbox_History", *(int *)(ev + 0xac));
            if (*(int *)(ev + 0x74) != 0) {
                d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
                io_PrintText(0x1ba3);
            }
        } else if ((fl & 4) != 0) {
            he_OpenEventSlot(ev, 0, 0, (int)"misc\EventMessagebox_Cutscenes", *(int *)(ev + 0xac));
        } else if ((fl & 0x40) != 0) {
            he_OpenEventSlot(ev, 0, 0, (int)"misc\EventMessagebox_Help", *(int *)(ev + 0xac));
        } else {
            he_OpenEventSlot(ev, 0, 0, (int)"misc\EventMessagebox", *(int *)(ev + 0xac));
            if (*(int *)(ev + 0x74) != 0) {
                d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
                io_PrintText(0x7d);
            }
        }
        if (*(int *)(ev + 0x74) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
        io_PrintText("%s", *(int *)(ev + 0x7c));
        if (*(unsigned char *)(unsigned)&ai_GameState != 2)
            sb_PlaySample(U_I((unsigned)&snd_SystemBank), "NeueMessage_s_01", 1, 0x3f, 0x7f, 0, 0);
        if ((fl & 8) != 0)
            he_ActivateSlot(*(int *)(ev + 0x74));
        if ((fl & 0x10) != 0) {
            strcpy(speak, (char *)(*(int *)(ev + 0x7c) + strlen((char *)*(int *)(ev + 0x7c)) + 1));
            *(int *)(ev + 0xec) = snd_QueueVoice(-7, 0, speak, 0, 0x7d0);
        }
        *(AITIME *)(ev + 0x44) = ai_Time;
        *(AITIME *)(ev + 0x52) = ai_Time;
        if ((fl & 2) != 0 && *(unsigned short *)(ev + 0x56) < 6)
            gm_AddTime(ev + 0x52, 0, 0, 1);
        else
            gm_AddTime(ev + 0x52, 0, 0, 1);
        return;
    case 0:
    case 1:
        b = gm_CompareTime((unsigned)&ai_Time, ev + 0x52);
        if (b > 0 || ((fl & 2) != 0 && *(int *)(ev + 0xb0) != -1 && cm_PartyById() != 0)) {
            if ((fl & 2) != 0 && *(int *)(ev + 0xb0) != -1) {
                w = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
                if (w != 0)
                    sv_SelectPanelObjectK(w);
                ai_Unk4e5210();
                ((void (*)())cm_SendLeaveParty)();
            }
            if ((fl & 0x40) != 0 && (U_I((unsigned)&ai_GameFlags) & 4) != 0)
                di_ClearButtons();
            sb_Stop(*(int *)(ev + 0xec));
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
        }
        return;
    }
}

void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);

/* 0x5578a0, the run handler of the training event (ROUGH skeleton): state 0 -> 1 (inc), state 1 waits for the due time, takes the person +0xac (0x4fea50) and its building (0x501190) and the two skill limits from the kind record (+0x240 / +0x241, +0x242 for the person type 0x116), counts +0xb0 (first) then +0xb4 (second) down while the person's skill byte (+0x1c / +0x1d) is below the limit, raising it by the message 0x4c3ba0 / 0x4c3d00 / 0x4c49a0; when both counters are 0 the person's owner gets the news 0x1bba (0x5f6 / 0x60b, with the 0x501b50 contact) and the record is freed; states -2 / -1 free */
void he_Unk5578a0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int o = 0, b, kind, lim0, lim1, c, q, own, tid;
    char text[0x400];

    if (st < -2 || st > 1)
        return;
    switch (st) {
    case 0:
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
        if (o == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
        if (*(unsigned short *)o == 0x116) {
            lim0 = *(unsigned char *)(kind + 0x242);
            lim1 = lim0;
        } else {
            lim0 = *(unsigned char *)(kind + 0x240);
            lim1 = *(unsigned char *)(kind + 0x241);
        }
        if (*(int *)(ev + 0xb0) > 0) {
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
            if (*(unsigned char *)(o + 0x1c) < lim0) {
                cm_BeginFieldBlock(o, *(int *)(o + 2));
                cm_AddFieldValues(1, 1, o - U_I((unsigned)&cm_FieldObject) + 0x1c, text);
                cm_SendFieldDeltas();
            }
        } else if (*(int *)(ev + 0xb4) > 0) {
            *(int *)(ev + 0xb4) = *(int *)(ev + 0xb4) - 1;
            if (*(unsigned char *)(o + 0x1d) < lim1) {
                cm_BeginFieldBlock(o, *(int *)(o + 2));
                cm_AddFieldValues(1, 1, o - U_I((unsigned)&cm_FieldObject) + 0x1d, text);
                cm_SendFieldDeltas();
            }
        }
        if (*(int *)(ev + 0xb4) == 0 && *(int *)(ev + 0xb0) == 0) {
            tid = 0x5f6;
            b = gm_BuildQuery(1, 0, *(int *)(ev + 0x10));
            if (b == 0) {
                he_FreeEvent((int *)ev);
                return;
            }
            own = gm_FindCharacterByMaster(b);
            if (own != 0 && (*(unsigned char *)(own + 0x1b4) & 1) != 0)
                tid = 0x60b;
            q = gm_BuildingMainObject(b);
            txt_Format(text, 0x1bba, b);
            if (q != 0)
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, tid, *(int *)(b + 1), *(int *)(q + 2), 0, 0);
            else
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, tid, -1, 0, 0, 0);
            he_FreeEvent((int *)ev);
            return;
        }
        gm_AddTime(ev + 0x52, 0, 0, 1);
        return;
    default:
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x557b30, the allocation handler of the 0x557b70 event: due clock = start clock, posted 2 units later */

void he_Unk557b30(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 2, 0, 0);
}
/* 0x557b70, the run handler "he_RunLagerErweitern" (the warehouse extension, ROUGH skeleton): state 0 .. 3 (jump table 0x557d58): waits for the due time (0x4fe210), moves the person +0xac (0x4fea50; 0x4c3ba0 / 0x4c3d00 / 0x4c49a0 raise a field), tells the owner (the news 0x1bbb, 0x53dee0 with 0x501b50's contact and the 0x50a810 owner flag) and frees; the default is the debug text "he_RunLagerErweitern():Wrong LagerID..." (0x4222b0) and the free */

void he_Unk557b70(int ev)
{
    int st = *(int *)(ev + 0x70);
    int o = 0, b, q, own;
    char text[0x400];

    if (st < 0 || st > 3) {
        err_Message("he_RunLagerErweitern():Wrong LagerID...");
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
        return;
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    cm_BeginFieldBlock(o, *(int *)(o + 2));
    cm_AddFieldValues(1, 1, st, text);
    cm_SendFieldDeltas();
    b = gm_BuildQuery(1, 0, *(int *)(ev + 0x10));
    if (b != 0) {
        own = gm_FindCharacterByMaster(b);
        q = gm_BuildingMainObject(b);
        txt_Format(text, 0x1bbb, b);
        if (q != 0)
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, (own != 0 && (*(unsigned char *)(own + 0x1b4) & 1) != 0) ? 0x60b : 0x5f6, *(int *)(b + 1), *(int *)(q + 2), 0, 0);
        else
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, 0x5f6, -1, 0, 0, 0);
    }
    he_FreeEvent((int *)ev);
}
/* 0x557d70, the allocation handler of the group event: the +0xac persons (ids from +0x8c) that exist are sent (0x524260 (0, 0, type, ev)); with none the record is freed, else state 1 and the event is posted 5 later */

void he_Unk557d70(int ev)
{
    int i, p, any = 0;

    for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
        p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
        if (p != 0) {
            any |= 1;
            sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        }
    }
    if (any == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x70) = 1;
    gm_AddTime(ev + 0x52, 0, 5, 0);
}

int gm_ClockSeason();
int gm_FreeCapacity();
int sv_IsAtDoor();

/* 0x557e20, the run handler of the herb gathering trip ("Kr\344uter" animation; ROUGH skeleton, 7 states by the jump table 0x5584a8; a season check 0x4fe350 against the float tables 0x617830 / 0x617840): the +0xac group persons (ids from +0x8c) are sent to the building +0x10 / the field positions (0x501190, 0x458a20, the 3D distance test against the range), gather ("Kr\344uter" 0x4c52e0, 0x4c5230), arrive (0x524770) and are given the harvest (the amount from 0x5092e0 and two random rolls 0x42ec00, 0x50ab80 / 0x50a810 for the owner), paid out by 0x4c4880 (.., -1) and the news 0x17fd / 0x1802 .. 0x1804 is shown to the owner (0x53dee0); the end frees the record and the persons (0x524260) */
void he_Unk557e20(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, i, p, n = 0, o;
    char text[0x400];
    float pos[3], pos2[3];

    if (gm_ClockSeason((unsigned)&ai_Time) < 0 || st < -2 || st > 4) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (st) {
    case -2:
    case -1:
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0)
                sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        }
        he_FreeEvent((int *)ev);
        return;
    case 0:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0)
            return;
        o = *(int *)(b + 0x61);
        d3_PointToWorld(o, o + 0x4c, pos);
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0) {
                cm_SendChrStop(*(int *)(p + 4));
                cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "Kr\344uter");
                n++;
            }
        }
        *(int *)(ev + 0x70) = 1;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        return;
    default:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0 && sv_IsAtDoor(p, 0) != 0) {
                n++;
                txt_Format(text, 0x17fd, p);
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, -1, 0, 0, 0);
            }
        }
        (void)pos2;
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x5584d0, the allocation handler of the group events: the +0xac persons (ids from +0x8c) that exist are sent (0x524260 (0, 0, type, ev)), the due clock is the start clock (+0x44) and the event posted 1 later (state kept) */

void he_Unk5584d0(int ev)
{
    int i, p;

    for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
        p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
        if (p != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
    }
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
/* 0x558550, the run handler of the animal hunt ("Getier", the second string 0x6274c0; ROUGH skeleton, 4 states by the jump table 0x5588a8): state 0 sends the hunters (0x505df0, 0x4c5230, 0x4c52e0 (.., "Getier" 0x6274c0)) and waits a random time (0x42ec00), state 1 waits for the due time (0x4fe210), looks for the building (0x501190, 0x4ffe90, the contact 0x501b50 / 0x50a810 owner) and pays out the catch (0x5092e0 and a random roll, 0x4c4880 (.., -1), the news 0x1805 / 0x1809 / 0x180a for the owner and the players of kind 6 / 7 (0x53dee0)); the end frees the persons (0x524260) and the record */

void he_Unk558550(int ev)
{
    int st = *(int *)(ev + 0x70);
    int i, p, b, q;
    char text[0x200];

    if (st < -2 || st > 1) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (st) {
    case -2:
    case -1:
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0)
                sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        }
        he_FreeEvent((int *)ev);
        return;
    case 0:
        for (i = 0; i < *(unsigned char *)(ev + 0xac); i++) {
            p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
            if (p != 0) {
                cm_SendChrStop(*(int *)(p + 4));
                cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(ev + 0x10), -1, 0, 0, "Getier");
            }
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(10), 0);
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b != 0) {
            q = gm_BuildingMainObject(b);
            txt_Format(text, 0x1805, b);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), q != 0 ? *(int *)(q + 2) : -1, 0, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    }
}

void cm_SendSpawnCharacter(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s);
int gm_WorkMinutesBetween();
void *gm_GetGebaeudeHauptraum(unsigned char *b);

/* 0x5588c0, the allocation handler of the sabotage event (ROUGH): the saboteur +0xb0 (0x505df0; gone or with a job of the kind 0x16 at +0x17c: free) and the building +0x10 are looked up (+0xb4 optionally the target figure 0x4ffe90 (.., 2, 6, 1, id)); the start / due clocks are now, the saboteur is stored for the group (0x524260 (building, target, id, ev)), +0x60 = now, posted 0x1e later, +0xb8 = the due clock + 6 units, and the word +0xbc is set to 0x1d + (+0xbc - the minute word) when it is below 6 */
void he_Unk5588c0(int ev)
{
    int p, b, tgt = 0;

    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0 || (*(int *)(p + 0x17c) != 0 && *(unsigned char *)*(int *)(p + 0x17c) == 0x16)) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(int *)(ev + 0xb4) != -1)
        tgt = gm_Query(*(int *)(b + 0x5d), 2, 6, 1, *(int *)(ev + 0xb4));
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0x44) = ai_Time;
    sv_MoveToGroup(b, tgt, *(unsigned short *)p, ev);
    *(AITIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    *(AITIME *)(ev + 0xb8) = *(AITIME *)(ev + 0x52);
    gm_AddTime(ev + 0xb8, 6, 0, 0);
    if (*(unsigned short *)(ev + 0xbc) < 6)
        *(unsigned short *)(ev + 0xbc) = (unsigned short)(*(unsigned short *)(ev + 0xbc) - *(unsigned short *)(ev + 0x56) + 0x1d);
}
/* 0x558a40, the run handler of the sabotage event (ROUGH skeleton): waits for the due time, rolls the success from the saboteur's skill (0x509400 x2, 0x5092e0 and the _ftol rolls) against the building, then marks the building / person (0x4c3ba0 / 0x4c3d00 / 0x4c49a0) and tells the owner (0x501ce0, the news "_NACHRICHTEN_HS_81" 0x53dee0); the group is released (0x524260) and the record freed */

void he_Unk558a40(int ev)
{
    int p, b, q;
    char text[0x200];

    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
        return;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b != 0) {
        q = gm_FindCharacterByMaster(b);
        txt_Format(text, 0x1bba, b);
        gm_GetGebaeudeHauptraum(b);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), q != 0 ? *(int *)(q + 2) : -1, "_NACHRICHTEN_HS_81", 0);
    }
    sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
    he_FreeEvent((int *)ev);
}
/* 0x558de0, the allocation handler: due clock = the clock at +0xb4 */

void he_Unk558de0(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0xb4);
}
/* 0x558e10, the run handler of the event type 0x17: state 3 sets its due clock to now + 1 minute, state 9 frees the record */

void he_RunEvent17(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    switch (e->stage) {
    case 3:
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 1, 0);
        break;
    case 9:
        he_FreeEvent((int *)ev);
        break;
    }
}
/* 0x558e70, the allocation handler of the "SABOTAGE" message event: due now + 1; with no building (+0x10 = -1) the first of the kind 0x16 (else 0xf) is taken; +0xc4 = -1 and +0xc8 = 0x4c59f0 (0, +0x10, -1, 0, 1, 0x174, "SABOTAGE") */

void he_Unk558e70(int ev)
{
    int b;

    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    if (*(int *)(ev + 0x10) == -1) {
        b = gm_BuildQuery(1, 5, 0x16);
        if (b == 0)
            b = gm_BuildQuery(1, 5, 0xf);
        if (b != 0)
            *(int *)(ev + 0x10) = *(int *)(b + 1);
    }
    *(int *)(ev + 0xc4) = -1;
    *(int *)(ev + 0xc8) = ((int (*)(int, int, int, int, int, int, const char *))cm_SendSpawnCharacter)(0, *(int *)(ev + 0x10), -1, 0, 1, 0x174, "SABOTAGE");
}

int gv_Accuse();
void cm_SendProofExecuted(int a, int b);
void cm_BeginGroup(const char *s);
void cm_EndGroup(const char *s);
int ord_DispatchOrder(int id, char *c, int *m);
int ai_InventoryHas(void *ch, int id);
void gm_SendMoneyDelta(int a, int b);

/* 0x558f10, the run handler of the sabotage event "Sabotage" (3600 bytes, 11 states by the jump table 0x559ce8; ROUGH skeleton, the details of every state are not drafted): the saboteur +0xb0 (the handle +0xb4 / +0xb8 of its job 0x4c3ae0 / 0x4c3b10, 0x4c52e0 "Sabotage" sends it), the building +0x10 and the target person +0xb4 are followed through the states: walk to the building, take the tool ("spezial/einhaendig_vom_boden_nehmen" 0x4c5410), the 0x553ae0 detection check and the guards (0x53e400 over the type 0x1d / 0x2c records), the success roll (0x5ba6b0 skill, 0x50cc50, a random float util_RandomF / gm_Mix), the damage (0x4c4b00 relation, 0x4c4ed0 / 0x4c4830 payment, 0x4f2e30 the relation check) and the news to the owner / the saboteur ("_NACHRICHTEN_HS_17" / "_18", 0x53dee0 / 0x53dda0); the end releases the group (0x524260) and frees the record */
void he_Unk558f10(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, b, h, o = 0;
    char text[0x200];

    if (st < -2 || st > 8) {
        he_FreeEvent((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0 || st < 0) {
        if (p != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        cm_SendMoveGoodsUnchecked(*(int *)(p + 4), 0, 0, 0);
        cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "Sabotage");
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    default:
        h = *(int *)(ev + 0xc4);
        if (h != -1 && cm_GetSlotState(h) == 0)
            return;
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xb4));
        txt_Format(text, 0x1bba, b);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_18", 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        return;
    }
}
/* 0x559d20, the allocation handler of the espionage event "Spionage" (ROUGH): the spy +0xac / +0xb0 is checked (0x505df0, flag 4 of +0x78), the target building is rolled (random 0x100, 0x42ec00 twice), the records of the type 0x35-like list (0x53e400 / 0x53e300) are searched for another spy on the same target (+0xd8, +8), the target person is looked up (0x4fea50, 0x4a63f0 (.., 100.0)) and the event posted (0x4c4c60); a spy of kind 6 / 7 gets the message record (0x4c4540 "Spionage", the message 0x4c59f0 "SPIONAGE" (0x62751c) and 0x4c4580), the handle goes to +0x84 */

void he_Unk559d20(int ev)
{
    int p, o = 0, e, n;

    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    n = (unsigned short)util_Random(0x100);
    (void)n;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
    for (e = (int)he_FindEvent(1, 0, 0x4e); e != 0; e = (int)he_NextEvent()) {
        if (*(unsigned short *)(e + 8) == *(unsigned short *)(ev + 8) && *(int *)(e + 0xd8) == *(int *)(ev + 0xd8))
            break;
    }
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o == 0) {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if (ord_DispatchOrder(0x26, (char *)ev, &o) == 2 || ((int (*)(void *))cm_BeginGroup)("Spionage") == 0) {
        cm_SendSpawnCharacter(0, *(int *)(ev + 0x10), -1, 0, 1, 0x174, "SPIONAGE");
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    } else {
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
    }
    cm_EndGroup("Spionage");
}
/* 0x55a030, the run handler of the espionage event ("Spionage: next target = %G" debug text, 5 states by the jump table 0x55a5d0; ROUGH skeleton): the spy's job handles (0x4c3ae0 / 0x4c3b10), the player record, the next target (0x524770 walk checks, 0x524260 group release, 0x4c52e0 / 0x4c5230 "Spionage" send, 0x4c4c60 posts, 0x4c4e00 stops) and the end frees the record */

void he_Unk55a030(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, h;

    if (st < -2 || st > 2) {
        he_FreeEvent((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0 || st < 0) {
        if (p != 0) {
            cm_SendChrStop(*(int *)(p + 4));
            ((void (*)(int))cm_SendKillCharacter)(*(int *)(p + 4));
        }
        he_FreeEvent((int *)ev);
        return;
    }
    h = *(int *)(ev + 0xc4);
    if (h != -1 && cm_GetSlotState(h) == 0)
        return;
    cm_SendChrStop(*(int *)(p + 4));
    cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(ev + 0x10), -1, 0, 0, "Spionage");
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
}

int ai_BrawlWin();
int gm_EquipmentValue(unsigned char *b, int level);
int gm_SumListAmounts(void *list, unsigned char p);
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);

/* 0x55a5f0, the allocation handler of the brawl event "PRUEGEL" (ROUGH): due now; with no building (+0x10 = -1) the first of the kind 0x16 (else 0xf) is taken, the message 0x4c59f0 (0x11, +0x10, -1, 0, 2, 0x154, "PRUEGEL") is created (+0xc8 = -1, handle +0xcc), and the target building +0xd0 is the opponent's own (+0x170) or its first of the type (0x501190 (1, 4, id)), else a free spot near it (0x4a63f0 (player, .., 100.0)) */
void he_Unk55a5f0(int ev)
{
    int p, b, own, spot;
    float v;

    p = gm_FindChar(*(int *)(ev + 0xb4));
    *(AITIME *)(ev + 0x52) = ai_Time;
    if (*(int *)(ev + 0x10) == -1) {
        b = gm_BuildQuery(1, 5, 0x16);
        if (b == 0)
            b = gm_BuildQuery(1, 5, 0xf);
        if (b != 0)
            *(int *)(ev + 0x10) = *(int *)(b + 1);
    }
    *(int *)(ev + 0xc8) = -1;
    *(int *)(ev + 0xcc) = ((int (*)(int, int, int, int, int, int, const char *))cm_SendSpawnCharacter)(0x11, *(int *)(ev + 0x10), -1, 0, 2, 0x154, "PRUEGEL");
    p = gm_FindChar(*(int *)(ev + 0xb4));
    own = *(int *)(p + 0x170);
    if (own == 0)
        own = gm_BuildQuery(1, 4, *(unsigned short *)p);
    if (own != 0) {
        *(int *)(ev + 0xd0) = *(int *)(own + 1);
        return;
    }
    spot = 0;
    if (AI_4A63F0_7((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, 6, *(int *)((unsigned)&gypsyFilter), *(int *)((unsigned)&gypsyFilter + 4), spot, 100.0f, &v) != 0)
        *(int *)(ev + 0xd0) = *(int *)(gm_BuildQuery(1, 1, (int)v) + 1);
    else
        *(int *)(ev + 0xd0) = *(int *)(own + 1);
}
/* 0x55a730, the run handler of the brawl event "PRUEGEL" (2112 bytes; ROUGH skeleton like 0x558f10): the two brawlers (+0xac / +0xb0 and the job handles 0x4c3ae0 / 0x4c3b10) walk to the building (0x4c52e0, the detection 0x553ae0, 0x4f2e30 relation, the guards 0x53e400, 0x4c4ed0), the fight is decided by the skills (0x494d90, 0x5ba6b0, 0x50cc50 twice) and "_NACHRICHTEN_HS_17" tells the winner / loser (0x53dda0 / 0x53dee0), the end releases the group (0x524260) and frees the record */

void he_Unk55a730(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, b, h;
    char text[0x200];

    if (st < -2 || st > 6) {
        he_FreeEvent((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0 || st < 0) {
        if (p != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    h = *(int *)(ev + 0xcc);
    if (h != -1 && cm_GetSlotState(h) == 0)
        return;
    cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "PRUEGEL");
    txt_Format(text, 0x1bba, b);
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, 0);
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0x70) = st + 1;
}
/* 0x55af70, the allocation handler of a news event (ROUGH): due now + random 0x42ec00(3) units, the first building (0x501190 (1, 1, ..)) of the kinds 0xb / 0xc / 0xd gives the message 0x4c4a60 (.., 0x5a, 2, 2) to its figure, the owner gets the news "_NACHRICHTEN_HS_43" (0x53dee0) and 0x509e30 / 0x4c47e0 set a flag; the record is freed */

void he_Unk55af70(int ev)
{
    int b, kind, p;
    char text[0x100];

    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(3), 0);
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d);
    if (kind == 0xd || kind == 0xc || kind == 0xb) {
        p = gm_FindChar(*(int *)(b + 1));
        if (p != 0)
            cm_SendSetField(*(int *)(p + 4), 0x5a, 2, 2, 0);
    }
    txt_Format(text, 0x1bba, b);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_43", 0);
    if (((int (*)(void *))gm_SumListAmounts)(b) != 0)
        ((void (*)(int))cm_SendMoveGoods)(b);
    he_FreeEvent((int *)ev);
}
/* 0x55b140, the run handler of the random theft message event (ROUGH): a flagged (2) record with a pending handle +0x84 waits (0x4c3ae0); states -2 .. -1 and above 0 free the record; state 0 with +0xb8 >= 2 looks at the building +0xac (not of the kinds 0xb / 0xc / 0xd; the type 0x27 needs a player owner of kind 6 / 7 at +0x27), rolls a chance from the building's value (0x509b10 (building, +0x5c), the factor 0x5ebb44 and +0xb0) against 0x42ec00(100) and sends a message record (type 0x6e, +0x4a = 1) through 0x4c4b70, then posts the event (0x4c4c60 (0, ev)) with the chance for a second round (0x4c4a60) */

void he_Unk55b140(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, kind, o, r;
    unsigned char m[0xf8];
    float chance;

    if ((*(unsigned char *)(ev + 0x78) & 2) != 0 && *(int *)(ev + 0x84) != -1) {
        if (cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
            return;
        *(int *)(ev + 0x84) = -1;
    }
    if (st < -2 || st > 0 || st != 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(int *)(ev + 0xb8) >= 2) {
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d);
        if (kind == 0xd || kind == 0xc || kind == 0xb) {
            he_FreeEvent((int *)ev);
            return;
        }
        o = gm_EquipmentValue(b, *(char *)(b + 0x5c));
        chance = (float)*(int *)(ev + 0xb0) / ((float)o + *(float *)(unsigned)&full_rdata_5ebb44);
        r = (unsigned short)util_Random(100);
        if ((float)r <= chance * (50.0f)) {
            memset(m, 0, sizeof(m));
            m[4] = 0x6e;
            *(int *)(m + 0xc) = -1;
            m[0x36] = 1;
            *(int *)(m + 8) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c);
            cm_SendLongMessage(m);
        }
    }
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}

void gm_ChangeBuildingState(unsigned char *b, int d);
void ai_Unk4c54e0(int a);
void cm_SendBuildingDestroyed(int a);

/* 0x55b3e0, the allocation handler of a building event (ROUGH): the building +0xac (none: free) is the event place (+0x10), its figure gets the message 0x5a / 2 / 4, both clocks are now, the due clock + 0xa units, and the end clock +0xb8 = now + (the owner rank factor 0x5eb7b8 / 0x5ee020 / 0x5eb828 / 1.0 by the rank byte +0xd (>= 9 / 8 / 7) * +0xb0 * 0x5eb990) units */
void he_Unk55b3e0(int ev)
{
    int b, rank;
    float f;

    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    *(int *)(ev + 0x10) = *(int *)(b + 1);
    cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 4, 0);
    *(AITIME *)(ev + 0x60) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0xa, 0);
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0xb8) = *(AITIME *)(ev + 0x52);
    rank = *(unsigned char *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c + 0xd);
    if (rank >= 9)
        f = (0.400000006f);
    else if (rank == 8)
        f = *(float *)(unsigned)&full_rdata_5ee020;
    else if (rank == 7)
        f = (0.800000012f);
    else
        f = 1.0f;
    gm_AddTime(ev + 0xb8, 0, (int)(f * *(int *)(ev + 0xb0) * *(double *)(unsigned)&ai_Unk5eb990), 0);
}
/* 0x55b530, the run handler of a building event (ROUGH): states -2 / -1 free; the building +0xac is looked up (none: free), the minutes since the clock +0x60 (0x4fe280) scale a value (+0xb0 .. 0x50cad0), the news 0x17cb ("_NACHRICHTEN_HS_68") goes to the owner (0x53dee0) and the event is posted 0xa later */

void he_Unk55b530(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, m;
    char text[0x100];

    if (st < 0 || (b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac))) == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    m = gm_MinutesBetween((unsigned)&ai_Time, ev + 0x60);
    *(int *)(ev + 0xb0) = (int)(m * 0.5 + 0.25) + *(int *)(ev + 0xb0);
    gm_ChangeBuildingState(b, *(int *)(ev + 0xb0));
    if (*(int *)(ev + 0xb0) != 0) {
        txt_Format(text, 0x17cb, b);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_68", 0);
    }
    *(AITIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0xa, 0);
}
/* 0x55b720, the allocation handler of a building event: both clocks now, the building +0xac (none: free), the due delay is the table value +0x243 of its kind (else 0x1e units), +0x10 = its id; a building with the flag 0x40 in +0x5a frees the record, else the flag is set (message 0x5a / 2 / 0x40) */

void he_Unk55b720(int ev)
{
    int b, kind, d;

    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0x44) = ai_Time;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
    d = *(int *)(kind + 0x243);
    gm_AddTime(ev + 0x52, 0, d != 0 ? d : 0x1e, 0);
    *(int *)(ev + 0x10) = *(int *)(b + 1);
    if ((*(unsigned short *)(b + 0x5a) & 0x40) != 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    *(unsigned short *)(b + 0x5a) = *(unsigned short *)(b + 0x5a) | 0x40;
    cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x40, 0);
}
/* 0x55b830, the run handler of a building event (ROUGH): states -2 / -1 free; the building +0xac is looked up, the player gets the news 0x17e7 and 0x1c32 for the owner (0x50a810), the flag 0x40 of the building is cleared (0x4c4a60 0x5a / 2 / 0x40, 0x4c54e0) and the record freed */

void he_Unk55b830(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, own, ch;
    char text[0x200];

    if (st < 0 || (b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac))) == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    own = gm_FindCharacterByMaster(b);
    ch = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    txt_Format(text, 0x17e7);
    he_MessageBoxQuickjump(*(int *)(ch + 4), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, 0, 0);
    if (own != 0) {
        txt_Format(text, 0x1c32, *(unsigned short *)(ev + 8) * 2 + 0x49c);
        he_MessageBoxQuickjump(*(int *)(ch + 4), -1, text, 0, 0x5ec, *(int *)(b + 1), *(int *)(own + 2), 0, 0);
    }
    ai_Unk4c54e0(b);
    cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x40, 0);
    he_FreeEvent((int *)ev);
}
/* 0x55ba00, the allocation handler of the event type 0x1f: both clocks now, then the due clock 1 second later (5 hours with
   gm_AllowAll); the building +0xac (none: the record is freed) is copied to +0x10 and gets the field 0x5a / 2 / 0x80
   while the 0x80 bit of its byte +0x5a is clear */

void he_AllocEvent1f(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *b;

    e->end = ai_Time;
    e->start = ai_Time;
    if (gm_AllowAll != 0)
        gm_AddTime(&e->end, 0, 0, 1);
    else
        gm_AddTime(&e->end, 5, 0, 0);
    b = (AIBLD *)gm_BuildQuery(1, 1, e->bldId);
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    e->from = b->id;
    if ((b->flags5a & 0x80) == 0)
        cm_SendSetField(b->id, 0x5a, 2, 0x80, 0);
}
/* 0x55bad0, the run handler of a building event (4 states by the jump table 0x55bd10; ROUGH): state 0 orders (0x4c4880 (+0xac building, -1, +0xb0, 1, 0, 0)) and marks the building (0x5a / 2 / 0x80), state 1 takes the owner figure (0x4ffe90), tells the player the news 0x1c2b (text 0x9a8 + +0xb0 * 2, "_NACHRICHTEN_HS_53", 0x606 / 0x60b by the 0x50a810 flag), the last state sends the goods (0x4c4db0 (4, ..)) and frees */

void he_Unk55bad0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, o, tid = 0x606;
    char text[0x400];

    if (st < 0 || st > 3) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        cm_SendExSellObject(*(int *)(b + 1), -1, *(unsigned short *)(ev + 0xb0), 1, 0, 0);
        cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x80, 0);
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 1:
        o = gm_Query(*(int *)(b + 0x5d), 2, 6, 1, *(unsigned short *)(ev + 0xb0));
        if (o != 0) {
            int w = gm_FindCharacterByMaster(b);

            txt_Format(text, 0x1c2b, b, *(unsigned short *)(ev + 0xb0) + 0x9a8);
            if (w != 0 && *(unsigned char *)(w + 2) == 1 && (*(unsigned char *)(w + 0x1b4) & 1) != 0)
                tid = 0x60b;
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(o + 2), text, 0, tid, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_53", 0);
        }
        *(int *)(ev + 0x70) = st + 1;
        return;
    default:
        cm_SendExSysMessage(4, ev + 0xb0);
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x55bd20, the allocation handler of a building event: the building +0xac (when found) gets the message 0x5a / 2 / 0x400, the due clock is now */

void he_Unk55bd20(int ev)
{
    int b;

    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b != 0)
        cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x400, 0);
    *(AITIME *)(ev + 0x52) = ai_Time;
}
/* 0x55bd80, the run handler "GebAbreissen" (the demolition of a building, 4 states by the jump table 0x55bf40; ROUGH): state 0 sends the figures of the building away (0x4c5b90, 0x4c52e0 (.., "GebAbreissen")) and waits, the later state demolishes (0x4c5510) after the news for the kind 0xf (0x53dda0) and frees the record */

void he_Unk55bd80(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, c, k;
    char text[0x200];

    if (st < 0 || st > 3) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        for (c = (unsigned)&ai_CharInts; c < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2); c += 0x21c) {
            if (*(int *)(c + 0x168) == b && *(int *)(c + 0x180) != 0) {
                cm_SendCharHalt(*(int *)c);
                cm_SendChrGotoBuilding(*(int *)c, -1, -1, 0, 0, "GebAbreissen");
            }
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = st + 1;
        return;
    default:
        k = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d);
        if (k == 0xf) {
            txt_Format(text, 0x1bbb, b);
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, 0);
        }
        cm_SendBuildingDestroyed(b);
        he_FreeEvent((int *)ev);
        return;
    }
}

float gm_Liking();
void cm_SendOwnership(int a, int b, int c);
void cm_SendSetJob(int id, int job);
int ai_LiquidateAssets();

/* 0x55bf50, a Ghidra miss: the allocation handler of the event 0x21 (ROUGH): the object 0x502020 (+0xac); the first run is random(10) + 10 units ahead; 0x4c4a60 (object + 1, 0x5a, 2, 0x200, 0) */
void he_Unk55bf50(int ev)
{
    int o;

    o = gm_FindBuildingById(*(int *)(ev + 0xac));
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, (unsigned short)((int (*)(int, int))util_Random)(10, 0) + 10);
    cm_SendSetField(*(int *)(o + 1), 0x5a, 2, 0x200, 0);
}
/* 0x55bfd0, the run handler of the building purchase event (a Ghidra function with the jump table 0x55c52c, 4 states and the case code 0x55c370 .. 0x55c52b; ROUGH skeleton): the buyer +0xb4 (0x505df0) and the building +0xac (0x502020) are looked up; state 0 opens the offer for the player (0x17e9 / 0x17ea news, 0x53dee0, the building flag 0x4c4a60 0x5a / 2 / 0x200), a computer buyer decides at once by a price factor (0x41700000 / 0x40a00000 / 0x3f99999a / 0x41000000 / 0x3fcccccd from the relation, the owner kind 5 / 6 / 7 and 0x50c550) against the value (0x509b10 (building, +0x5c)); the answer (0xf8a668: 0 = buy, 1 = decline) pays (0x4c4830), hands the building over (0x4c54a0) and closes the window (0x53d800); the end frees the record */

void he_Unk55bfd0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int buyer, b, price;
    float f;
    char text[0x200];

    if (st < 0 || st > 3) {
        he_FreeEvent((int *)ev);
        return;
    }
    buyer = gm_FindChar(*(int *)(ev + 0xb4));
    b = gm_FindBuildingById(*(int *)(ev + 0xac));
    if (buyer == 0 || b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        f = 1.0f;
        price = gm_EquipmentValue(b, *(char *)(b + 0x5c));
        (void)price;
        gm_Liking(*(unsigned short *)(b + 0x25));
        txt_Format(text, 0x17e9, *(unsigned short *)(b + 0x27));
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(int *)(ev + 0xb0) * 0x21c), -1, text, 0, 0x5ec, -1, 0, 0, 0);
        cm_SendSetField(*(int *)(buyer + 1), 0x5a, 2, 0x200, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (U_I((unsigned)&d2_ClickedId) == -1)
            return;
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            cm_SendMoveGoodsUnchecked(*(int *)(buyer + 1), *(int *)((unsigned)&ai_CharInts + *(int *)(ev + 0xb0) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            ((void (*)(int))cm_SendOwnership)(*(int *)(buyer + 1));
        } else {
            txt_Format(text, 0x17ea, *(unsigned short *)(b + 0x27));
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(int *)(ev + 0xb0) * 0x21c), -1, text, 0, 0x5ec, -1, 0, 0, 0);
        }
        cm_SendSetField(*(int *)(buyer + 1), 0x5a, 2, 0x200, 0);
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    default:
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x55c540, the allocation handler of the purchase event (ROUGH): the due clock is now, the end clock +0xc4 = now + the delay +0xb8, +0xc8 = 8, +0xca / +0xce = 0, +0xbc = +0xb8; the building +0xac is looked up (none: free), the buyer +0xb0 (0x505df0); for a player buyer (kind 6 / 7) the offer window is prepared (0x4ffe90, the news 0x18d7 with 0x5d, 0x53dee0 (.., 0x5ec)) */

void he_Unk55c540(int ev)
{
    int b, buyer, o;
    char text[0x200];

    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0xc4) = ai_Time;
    gm_AddTime(ev + 0xc4, 0, *(int *)(ev + 0xb8), 0);
    *(unsigned short *)(ev + 0xc8) = 8;
    *(int *)(ev + 0xca) = 0;
    *(int *)(ev + 0xce) = 0;
    *(int *)(ev + 0xbc) = *(int *)(ev + 0xb8);
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
        return;
    buyer = gm_FindChar(*(int *)(ev + 0xb0));
    if (buyer != 0 && (*(unsigned char *)(buyer + 2) == 6 || *(unsigned char *)(buyer + 2) == 7)) {
        o = gm_Query(*(int *)(b + 0x5d), 2, 6, 1, *(unsigned short *)(ev + 0xb0));
        if (o != 0) {
            txt_Format(text, 0x18d7, 0x5d);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, -1, 0, 0, 0);
        }
    }
}
/* 0x55c700, the run handler "he_RunKredit" (the loan event, 4 states; the debug text "he_RunKredit(): Could not find GebaeudeId '%i'" when the building +0xac is missing; ROUGH skeleton): the player figure gets the offer news (0x5ec / "_NACHRICHTEN_HS_69", the amount 0x5b92e0, the interest from 0x509e60 through _ftol / 0x5de652 (sprintf)) and 0x4c4830 books the payment; the end frees the record */

void he_Unk55c700(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, amt;
    char text[0x200];

    if (st < 0 || st > 3 || (st != 0 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0 && *(int *)(ev + 0x84) != -1)) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        sprintf(text, "he_RunKredit(): Could not find GebaeudeId '%i'", *(int *)(ev + 0xac));
        he_FreeEvent((int *)ev);
        return;
    }
    amt = (int)gm_GetGoodsValue(*(unsigned short *)(b + 0x27));
    txt_Format(text, 0x17f0, b, amt);
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, 0);
    cm_SendMoveGoodsUnchecked(*(int *)(ev + 0xb0), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, amt);
    he_FreeEvent((int *)ev);
}
/* 0x55cb00, the run handler of a message event (ROUGH): states -2 / -1 and the missing figure +0xb0 free the record; a figure of kind 6 / 7 gets a message record (0x4c4b70) and the news 0x4c59b0; then the record is freed */

void he_Unk55cb00(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p;
    unsigned char m[0xf8];

    if (st <= -2 || (p = gm_FindChar(*(int *)(ev + 0xb0))) == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(unsigned char *)(p + 2) == 6 || *(unsigned char *)(p + 2) == 7) {
        memset(m, 0, sizeof(m));
        cm_SendLongMessage(m);
        ((void (*)(int))cm_SendSetJob)(*(int *)(ev + 0x10));
    }
    he_FreeEvent((int *)ev);
}

int gm_TitlesLeft();
int gm_TitleGroup();
struct SBBANK * sb_FindBank();

/* 0x55cc10, the run handler of the master exam announcement ("Berufe\Messagebox_Meisterpruefung", 4 states by the jump table 0x55cdec; ROUGH): returns while the due time is not reached (0x4fe210); state 0 opens the window (0x53d6f0 (ev, 0, 0, name, 0x5fd), none: free), state 1 looks up the person +0xac (0x505df0), its title group (0x504ce0, 0x504ad0 = titles left; the sound names "BERUF_TRAINING_SPIELER_MAENNLICH" / "_WEIBLICH" with the ADDON variants by the sex and by 0xc titles, 0x426290 lookups) and speaks it (0x4c27d0 (-8, .., name, ..), the news 0x159a through 0x515960), state 2 / 3 close the window (0x4ffe90 contact, 0x521960, 0x53d800) and free the record */
void he_Unk55cc10(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, n;
    char *snd;

    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) == 0)
        return;
    if (st < 0 || st > 3) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        he_OpenEventSlot(ev, 0, 0, (int)"Berufe\\Messagebox_Meisterpruefung", 0x5fd);
        if (*(int *)(ev + 0x74) == 0)
            he_FreeEvent((int *)ev);
        return;
    case 1:
        p = gm_FindChar(*(int *)(ev + 0xac));
        if (p == 0)
            return;
        n = gm_TitlesLeft(p);
        if (gm_TitleGroup(p, n) != 0) {
            if (n == 0xc)
                snd = "BERUF_TRAINING_SPIELER_MAENNLICH";
            else
                snd = "ADDON_BERUF_TRAINING_SPIELER_MAENNLICH";
        } else {
            snd = *(unsigned char *)(p + 9) == 1 ? "BERUF_TRAINING_SPIELER_WEIBLICH" : "ADDON_BERUF_TRAINING_SPIELER_WEIBLICH";
        }
        sb_FindBank(snd);
        snd_QueueVoice(-8, 0, snd, 0, 0);
        io_PrintText(0x159a);
        return;
    default:
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x55ce00, the allocation handler of the master exam passed event: the record of type 0x25 of the same player (0x53e400 (2, 2, id, 0, 0x25)) is ended (state -1), the due clock is now */

void he_Unk55ce00(int ev)
{
    char *e;

    gm_FindChar(*(int *)(ev + 0xac));
    e = he_FindEvent(2, 2, *(unsigned short *)(ev + 8), 0, 0x25);
    if (e != 0)
        *(int *)(e + 0x70) = -1;
    *(AITIME *)(ev + 0x52) = ai_Time;
}
/* 0x55ce60, the run handler of the "Berufe\Meisterpruefung_BESTANDEN" ceremony (the exam passed; for the player's figure only: 0x6f25b8; ROUGH skeleton, 1199 bytes): opens the window, shows the person's new title (gm_TitleGroup / gm_TitlesLeft, the texts "BERUF_BESTANDEN", "ADDON_BERUF_LISTE_MAENNLICH" / "_WEIBLICH", "ADDON_GEBAEUDE_LISTE", the sound banks "brk_%02i" 0x4c27d0 x8), waits for the click and closes (0x53d800) and frees the record */

void he_Unk55ce60(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p;
    char name[0x40];

    if (*(unsigned short *)(unsigned)&ai_PlayerId != *(unsigned short *)(ev + 8))
        return;
    if (st < 0 || st > 2) {
        if (*(int *)(ev + 0x74) != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (st == 0) {
        he_OpenEventSlot(ev, 0, 0, (int)"Berufe\\Meisterpruefung_BESTANDEN", 0x5fd);
        if (*(int *)(ev + 0x74) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
        p = gm_FindChar(*(int *)(ev + 0xac));
        sprintf(name, "brk_%02i", p != 0 ? *(unsigned char *)(p + 0xd) : 0);
        snd_QueueVoice(-8, 0, name, 0, 0);
        io_PrintText("BERUF_BESTANDEN");
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (U_I((unsigned)&d2_ClickedId) == -1)
        return;
    he_ReleaseSlot(ev);
    he_FreeEvent((int *)ev);
}

struct G15PLANT * veg_FindPlantId(struct G15PLANT *p, int id);
void cm_SetPlant(int a, unsigned char b, unsigned char c, short d, unsigned char e, int f);
void cm_SendCharClear(int a, int b, int c);
void cm_SendFieldValues(void);
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);
int gm_Opinion(int a, int b);
void cm_SendMessage5b(int a, int b, int c);
int gm_PlayerFactor(int a, unsigned char k);

/* 0x55d310, the allocation handler of the event type 0x27: the building +0xac (none: the record is freed) is asked for its
   skill 0x10 / 1, the due clock is now + 0x18 hours with its minute field cleared */
void he_AllocEvent27(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *b;

    b = (AIBLD *)gm_BuildQuery(1, 1, e->bldId);
    if (b != 0) {
        gm_BuildingSkill((unsigned char *)b, 0x10, 1);
        e->end = ai_Time;
        gm_AddTime(&e->end, 0x18, 0, 0);
        e->end.min = 0;
    }
    else {
        he_FreeEvent((int *)ev);
    }
}
/* 0x55d390, the run handler of a payment event (ROUGH): states -2 / -1 free; the building +0xac (none: free), the news 0x190b (+0xb4, +0xb0) to the player (0x53dda0 .., 0x5ff) and the payment 0x4c4880 (+0xb4, .., +0xb0, ..); then the record is freed */

void he_Unk55d390(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b;
    char text[0x200];

    if (st < 0 || (b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac))) == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    txt_Format(text, 0x190b, *(int *)(ev + 0xb4), *(unsigned char *)(ev + 0xb0));
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ff, 0);
    cm_SendExSellObject(*(int *)(ev + 0xb4), -1, *(unsigned char *)(ev + 0xb0), 0, 0, 0);
    he_FreeEvent((int *)ev);
}
/* 0x55d490, the allocation handler of the harvest event (ROUGH): the due clock is now (+1), state 0; the building +0xac (0x501190) and the field object +0xb8 (0x4bc670) and the worker +0xb0 (0x505df0; a free worker without job / not of the job 0x28: the group 0x524260 (building, 0xfd contact via 0x4ffe90, ..)) are set up, the job handle +0xc0 = 0x4c5050 (building, 1), +0xb4 = -1 and the word +0xbc = the minute word; a missing worker frees the record */

void he_Unk55d490(int ev)
{
    int b, p, o;

    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
    *(int *)(ev + 0x70) = 0;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b != 0)
        veg_FindPlantId(b, *(int *)(ev + 0xb8));
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        sv_MoveToGroup(0, 0, 0, ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (b != 0 && *(int *)(b + 0x10) != -1 && *(unsigned char *)(p + 0x180) != 0x28) {
        o = gm_Query(0, 1, 0, 0xfd);
        sv_MoveToGroup(b, o, *(unsigned short *)p, ev);
    }
    *(int *)(ev + 0xc0) = ((int (*)(int, int))cm_SetPlant)(*(int *)(ev + 0xac), 1);
    *(int *)(ev + 0xb4) = -1;
    *(unsigned short *)(ev + 0xbc) = *(unsigned short *)(ev + 0xbc);
}
/* 0x55d5c0, the run handler of the harvest event ("Ernten_start_%s" / "Ernten_end_%s" cutscene scripts "x:\engine\gfx\scripts\" "%slocations/%s/%s_%s.esc", "sp_%i" spots; ROUGH skeleton, 1440 bytes): the field object (0x4bc670 / 0x4bc5a0), the worker handle (0x4c3ae0), the scene (0x511e60) and the script (0x42f8f0 path, evt_LoadScript, evt_RunScriptArgs) are started and stopped (evt_FindById, evt_Stop) around the work; the end orders (0x4c4880), sends a message record (0x4c4b70), releases the group (0x524260) and frees the record */

void he_Unk55d5c0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, p;
    char path[0x100];

    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0 || st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        sv_MoveToGroup(0, 0, 0, ev);
        he_FreeEvent((int *)ev);
        return;
    }
    init_BuildingSceneUniverse(b);
    sprintf(path, "%slocations/%s/%s_%s.esc", evt_Path, "", "Ernten_start_%s", "");
    ((void (*)(int, int))cm_SetPlant)(*(int *)(ev + 0xac), 0);
    sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
    he_FreeEvent((int *)ev);
}
/* 0x55db60, a run handler: ends the job of the building +0xac (0x4c5050 (.., 0)) and frees the record; a missing figure releases the group (0x524260) first */

void he_Unk55db60(int ev)
{
    int p;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p != 0)
        ((void (*)(int, int))cm_SetPlant)(*(int *)(ev + 0xac), 0);
    else
        sv_MoveToGroup(0, 0, 0, ev);
    he_FreeEvent((int *)ev);
}
/* 0x55dbe0, the run handler "Entlassen" (the dismissal of a worker, jump table 0x55dbc0; ROUGH): states -2 / -1 free; the figure +0xac (0x505df0), its job record +0x16c (0x4c3ba0 / 0x4c3d90 / 0x4c49e0 mark it, 0x50c520 the wage check against -0x1e), the relation 0x4c4b00, the job end 0x4c5970 / 0x4c5b90, the contact 0x4fdcb0 and the walk away (0x4c52e0 (.., "Entlassen" 0x627860)); the record is freed, state -1 on a failed check */

void he_Unk55dbe0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p;

    if (st == -2 || st == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    if (*(int *)(p + 0x16c) != 0) {
        cm_BeginFieldBlock(p, *(int *)(p + 4));
        cm_AddFieldValue(1, 1, 4, 0);
        cm_SendFieldValues();
        ((void (*)(int))gm_Opinion)(*(int *)(p + 0x16c));
        cm_SendRelations(*(int *)(p + 4), *(int *)(p + 0x16c), -0x1e, 1);
    }
    ((void (*)(int, int))cm_SendCharClear)(0, 0);
    cm_SendCharHalt(*(int *)(p + 4));
    cm_SendChrGotoBuilding(*(int *)(p + 4), -1, -1, 0, 0, "Entlassen");
    he_FreeEvent((int *)ev);
}
#define HE_PROMOTION_BIT 0x1000000  /* bit 24 of AICHAR.flags1cc: the promotion alloc handler 0x55dd70 sets it, the run handler 0x55de30 clears it (cm_SendSetField(id, 0x1cc, 4, bit, 0)); the meaning is a reasoned guess */

/* 0x55dd70, the allocation handler of the event type 0x32 (the promotion, its run handler 0x55de30): when another record of
   the type 0x32 is already running for the same figure (+0xac) this one is freed; else +0xb0 = -1, the due clock is now + 1
   second and a figure that does not exist frees the record; a record with the option bit 2 sends the field 0x1cc / 4 /
   HE_PROMOTION_BIT to the figure */

void he_AllocEvent32(int ev)
{
    AIJOB *j = (AIJOB *)ev;
    AIJOB *e;
    AICHAR *p;

    for (e = (AIJOB *)he_FindEvent(1, 0, 0x32); e != 0; e = (AIJOB *)he_NextEvent()) {
        if (e != j && e->ownerCh == j->ownerCh) {
            he_FreeEvent((int *)ev);
            return;
        }
    }
    j->dropSlot = -1;
    j->end = ai_Time;
    gm_AddTime(&j->end, 0, 0, 1);
    p = (AICHAR *)gm_FindChar(j->ownerCh);
    if (p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if ((j->opts & 2) != 0)
        cm_SendSetField(p->netId, 0x1cc, 4, HE_PROMOTION_BIT, 0);
}
/* 0x55de30, the run handler of the promotion event ("Effekte/befoerdern.esc" cutscene; ROUGH skeleton, 928 bytes): the figure +0xac and its building (gm_FindBuildingById) are looked up, the old rank is dropped (gm_DropRank), the effect script is loaded and run in the scene (evt_Load, evt_RunScriptArgs, d3if_SelectScene, ch_Delete of the old hand object, ch_SetHandObject), the job handle is cleaned (0x4c3ae0, 0x4c4c60, 0x4c4a60) and the record freed */

void he_Unk55de30(int ev)
{
    int p;

    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    cm_SendExSetHE(-1, ev);
    cm_SendSetField(*(int *)(p + 4), 0x1cc, 4, 0, ((unsigned)&txt_Keys + 0x181c0));
    he_FreeEvent((int *)ev);
}
/* 0x55e180, a Ghidra miss: the allocation handler of the event 0x33: the player 0x505df0 (+0xac) must exist and his kind byte (+2) must not be 0x14 (else free), the run clock is the start clock */

void he_Unk55e180(int ev)
{
    int p;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p != 0 && *(char *)(p + 2) != 0x14) {
        *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x55e1d0, the run handler "Entlassen" of the dismissal with news (ROUGH skeleton, 464 bytes): the figure +0xac (0x505df0), 0x50c520, the job end 0x4c5970 / 0x4c5b90, the marks 0x4c3ba0 / 0x4c3d90 / 0x4c49e0, the walk away (0x4fdcb0 contact, 0x4c52e0 "Entlassen"), the relation 0x4c4b00 and the news "_NACHRICHTEN_HS_01" (0x53dee0, with the building main object) to the owner; the record is freed */

void he_Unk55e1d0(int ev)
{
    int p;
    char text[0x100];

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    ((void (*)(int, int))cm_SendCharClear)(0, 0);
    cm_SendCharHalt(*(int *)(p + 4));
    cm_SendChrGotoBuilding(*(int *)(p + 4), -1, -1, 0, 0, "Entlassen");
    txt_Format(text, 0x1bba, p);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(p + 4), -1, "_NACHRICHTEN_HS_01", 0);
    he_FreeEvent((int *)ev);
}
/* 0x55e3a0, the allocation handler: the due clock is the start clock (+0x44) */

void he_Unk55e3a0(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
}
/* 0x55e3d0, the run handler of a news event (ROUGH skeleton, 496 bytes): a figure (0x505df0) is looked up, a random building of the query list (gm_Query / gm_QueryNext, 0x42ec00) is picked, the amount 0x5089c0 / _ftol is booked (0x4c4880) and the news "_NACHRICHTEN_HS_09" shown to the owner (0x53dee0); the record is freed */

void he_Unk55e3d0(int ev)
{
    int p;
    char text[0x100];

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p != 0) {
        cm_SendExSellObject(*(int *)(ev + 0xac), -1, 0, 0, 0, 0);
        txt_Format(text, 0x1bba, p);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(p + 4), -1, "_NACHRICHTEN_HS_09", 0);
    }
    he_FreeEvent((int *)ev);
}
/* 0x55e5c0, the allocation handler: due clock now + 4 hours (0x4fe140 (clock, 0, 0, 4)) */

void he_Unk55e5c0(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 4, 0, 0);
}
/* 0x55e600, the run handler of a relation event (ROUGH): states -2 / -1 free; the building +0x10 (none: free) and the figure +0xac; every character of the town in the same building gets the relation change 0x4c4b00 (id, +0xac, 0x3e8-scaled by 0x5085f0 (1000)) and the player figures (kind 6 / 7) the news 0x19ab (0x53dee0, 0x5ec); then the record is freed */

void he_Unk55e600(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, c;
    char text[0x100];

    if (st < 0 || (b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10))) == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    ((void (*)(int))gm_PlayerFactor)(0x3e8);
    for (c = (unsigned)&ai_CharInts; c < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x16a); c += 0x21c) {
        if (*(unsigned short *)(c - 0x4) != 0xffff && *(int *)(c + 0x168) == b) {
            cm_SendRelations(*(int *)c, *(int *)(ev + 0xac), 0, 0);
            if (*(unsigned char *)(c - 2) == 6 || *(unsigned char *)(c - 2) == 7) {
                txt_Format(text, 0x19ab, b);
                he_MessageBoxQuickjump(*(int *)c, -1, text, 0, 0x5ec, -1, 0, 0, 0);
            }
        }
    }
    he_FreeEvent((int *)ev);
}
/* 0x55e730, the run handler of the exam repeat event ("Berufe\Messagebox_Meisterpruefung"; ROUGH skeleton, 815 bytes): opens the window for the player (0x53d6f0, d2_SetForm), shows the offer (0x515960, the news 0x53dda0 via 0x5176d0), the answer pays / refuses (0x4c5fa0, 0x50b290, 0x4c4b00 relation) and closes (0x53d800, free) */

void he_Unk55e730(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st < 0 || st > 1) {
        if (*(int *)(ev + 0x74) != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (st == 0) {
        he_OpenEventSlot(ev, 0, 0, (int)"Berufe\\Messagebox_Meisterpruefung", 0x5fd);
        if (*(int *)(ev + 0x74) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (U_I((unsigned)&d2_ClickedId) == -1)
        return;
    ((void (*)(int))cm_SendMessage5b)(*(int *)(ev + 0xac));
    he_ReleaseSlot(ev);
    he_FreeEvent((int *)ev);
}

int init_ObjectSceneUniverse();

/* 0x55e9d0, a Ghidra miss: the allocation handler of the daily routine event 0x3a: +0xb0 = 3, +0xac = 0 (the routine step), the clocks +0x52 / +0x44 are now, the minute word is set to 6:15 (0x4fe1e0 (+0x52, 6, 0xf, 0)) */
void he_Unk55e9d0(int ev)
{
    gm_ClockSeason((unsigned)&ai_Time);
    *(int *)(ev + 0xb0) = 3;
    *(int *)(ev + 0xac) = 0;
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0x44) = ai_Time;
    gm_SetTime(ev + 0x52, 6, 0xf, 0);
}
/* 0x55ea60, the id of the contact (the person to talk to at the place) of the figure `p`, or -1 (ROUGH): -1 for no figure, the kinds 0xd / 0xe, no current building (+0x16c) or object (+0x184) and for a busy job code (+0x180: 0x3f, 0x40, 0x48, 0x3c, 0x49, 0x44, 0x62, 0x61, 0x65, 0x64, 0x14, 0x75, 0x1a, 0x18, 0xa1); a building that 0x502520 knows takes the owner contact (0x4ffe90 (building object +0x5d, 1, 0, 0xfd) or the type 0x37 for the job codes 3 / 8 / 9) and gives its +2 id when the target of the figure (+0x184 +0x2c / +0x30) is not it (0x501b50 as the fallback) */

int he_FigureContact(int p)
{
    int code, b, o;

    if (p == 0 || *(unsigned char *)(p + 2) == 0xd || *(unsigned char *)(p + 2) == 0xe)
        return -1;
    b = *(int *)(p + 0x16c);
    if (b == 0 || *(int *)(p + 0x184) == 0)
        return -1;
    code = *(unsigned char *)(p + 0x180);
    if (code == 0x3f || code == 0x40 || code == 0x48 || code == 0x3c || code == 0x49 || code == 0x44 || code == 0x62 || code == 0x61 || code == 0x65 || code == 0x64 || code == 0x14 || code == 0x75 || code == 0x1a || code == 0x18 || code == 0xa1)
        return -1;
    if (*(unsigned char *)(p + 0x1b1) != 0)
        return -1;
    if (gm_IsHomeType(b) != 0) {
        if (code == 3 || code == 8 || code == 9)
            o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
        else
            o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0x37);
        if (o != 0 && *(unsigned short *)o != 0xfd && *(int *)(*(int *)(p + 0x184) + 0x2c) != *(int *)(*(int *)(p + 0x184) + 0x30))
            return *(int *)(o + 2);
        return -1;
    }
    o = gm_BuildingMainObject(b);
    return o != 0 ? *(int *)(o + 2) : -1;
}
/* 0x55eba0, selects the scene of the figure `p` when it should be seen (ROUGH): nothing for no figure, the kinds 0xd / 0xe or a busy job code (as 0x55ea60: 4, 0x10, 0x13, 0x1d, 0x1e and the same list), the owner contact of its building (+0x16c; 0x4ffe90 types 0x2c / 0x109) or its target (+0x170) is looked up (0x4fdcb0, 0x4fe980) and the scene shown (0x511e60, or 0x511dc0 for the free-standing case) */

void he_FindFigurePlace(int p)
{
    int b, code, o;

    if (p == 0 || *(unsigned char *)(p + 2) == 0xd || *(unsigned char *)(p + 2) == 0xe)
        return;
    b = *(int *)(p + 0x16c);
    code = *(unsigned char *)(p + 0x180);
    if (code == 4 || code == 0x10 || code == 0x13 || code == 0x1d || code == 0x1e)
        return;
    if (code == 0x3f || code == 0x40 || code == 0x48 || code == 0x3c || code == 0x49 || code == 0x44 || code == 0x62 || code == 0x61 || code == 0x65 || code == 0x64 || code == 0x14 || code == 0x75 || code == 0x1a || code == 0x18 || code == 0xa1)
        return;
    if (*(unsigned char *)(p + 0x1b1) != 0)
        return;
    if (b != 0 && gm_IsHomeType(b) != 0) {
        o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0x2c);
        if (o == 0)
            o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0x109);
        if (o != 0)
            init_ObjectSceneUniverse(o);
        return;
    }
    if (*(int *)(p + 0x170) != 0) {
        init_BuildingSceneUniverse(*(int *)(p + 0x170));
        return;
    }
    o = gm_FindObjectInst(*(int *)(p + 4));
    if (o != 0)
        init_BuildingSceneUniverse(*(int *)(o + 0x5d));
}

void gm_SetTime(struct AITIME *c, unsigned char hour, unsigned min, unsigned sec);
void cm_RequestChrMove(int a, int b, int c, const char *s);
void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s);
void cm_SendGoodsDelta(struct GMSTOCK *p, unsigned char k);
int gm_ListAmount();
int gm_CharCurrency(char *c, char p);
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);
void *gm_GetGebaeudeHauptraum(unsigned char *b);

/* 0x55ee40, a Ghidra start: the chance of a figure to be seen at its place (ROUGH): for a figure with the flag 1 of +0x5a and objects (+0x61 / the target's +0x170 +0x61) the 3D distance of the two objects scales a factor against 0x5eb780 (1.0), 0x5edbb8 and the building value (0x5092e0 (.., 9, 1)); a nearby contact (0x4ffe90 (.., 1, 0, 0x120)) with an object returns 1, else 0 */
int he_Unk55ee40(int p)
{
    int o1, o2, c;
    float dx, dy, dz, d;

    if (p == 0 || (*(unsigned char *)(p + 0x5a) & 1) == 0)
        return 0;
    o1 = *(int *)(p + 0x61);
    if (o1 == 0 || *(int *)(p + 0x170) == 0)
        return 0;
    o2 = *(int *)(*(int *)(p + 0x170) + 0x61);
    if (o2 == 0)
        return 0;
    dx = *(float *)(o1 + 0x4c) - *(float *)(o2 + 0x4c);
    dy = *(float *)(o1 + 0x50) - *(float *)(o2 + 0x50);
    dz = *(float *)(o1 + 0x54) - *(float *)(o2 + 0x54);
    d = (float)sqrt(dx * dx + dy * dy + dz * dz);
    gm_BuildingSkill(p, 9, 1);
    if (d > 1.0f)
        return 0;
    c = gm_Query(*(int *)(p + 0x5d), 1, 0, 0x120);
    return c != 0 && *(int *)(c + 0x61) != 0;
}
/* 0x55efc0, the run handler of the daily routine of a figure ("Go to work" / "Go to wirtshaus" / "Go home" debug texts, "dummy_EINGANG" entrance dummies; ROUGH skeleton, 2656 bytes): by the season / time of day (gm_ClockSeason) the figure +0xac either walks to its workplace (0x55ea60 contact, 0x4c5b90 / 0x4c52e0 / 0x4c51b0 / 0x4c5150, the entrance 0x458a20 positions), to the tavern or home (gm_IsHomeType), and the routine event is posted again (0x4fe1e0 clocks); 0x55eba0 selects the scene of a watched figure */

void he_Unk55efc0(int ev)
{
    int p, c;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    c = he_FigureContact(p);
    cm_SendCharHalt(*(int *)(p + 4));
    cm_SendChrGotoBuilding(*(int *)(p + 4), c, -1, 0, 0, "Go to work");
    cm_RequestChrMoveToUniverse(*(int *)(p + 4), -1, c, "dummy_EINGANG");
    he_FindFigurePlace(p);
    he_Unk55ee40(p);
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
}
/* 0x55fa20, the allocation handler of the market event: the due clock is now (+0x52, 0x4fe1e0 sets the minute word 5) and the event posted (0x4fe140 (.., 0, 0, 1)) */

void he_Unk55fa20(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_SetTime(ev + 0x52, 5, 0, 0);
    gm_AddTime(ev + 0x52, 1, 0, 0);
}
/* 0x55fa90, the run handler "he_RunMarktSupervisor" of the market (3472 bytes, ROUGH skeleton; the debug text "he_RunMarktSupervisor(): Could not find markt-lager:%s"): the market object (gm_FindMarketObject) and the stores (gm_Query, 0x50aaf0 / 0x4ffa10) of the town are looked at; from random rolls (util_Random / util_RandomF) the supervisor buys and sells goods (0x4c4880 payments, 0x5de518 _ftol amounts), a figure goes shopping (0x4c5710) and the clock event is posted again; a missing store prints the debug text and frees the record */

void he_Unk55fa90(int ev)
{
    int st = *(int *)(ev + 0x70);
    char text[0x100];

    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_CharCurrency(*(char **)(unsigned)&ai_PlayerChar, *(unsigned char *)(unsigned)&ai_OwnPlayer) == 0) {
        sprintf(text, "he_RunMarktSupervisor(): Could not find markt-lager:%s", "");
        he_FreeEvent((int *)ev);
        return;
    }
    ((void (*)(int))cm_SendMoveGoods)(ev);
    cm_SendExSellObject(*(int *)(ev + 0xac), -1, 0, 0, 0, 0);
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
/* 0x560820, ends a duplicate: of the records of type 0x3c (0x53e400 / 0x53e300) the one with the same +0xac and +0xb4 as `ev` (not ev itself) sets the state -1 of `ev`; a building (+0xb4, 0x501190) with +0x65 = -1 or a figure +0xac (0x505df0) with the flag +0x1b1 does the same; the due clock is now and the event posted (0x4fe140 (.., 0, 0, 1)) */

void he_Unk560820(int ev)
{
    char *e;
    int b, p;

    for (e = he_FindEvent(1, 0, 0x3c); e != 0; e = he_NextEvent()) {
        if ((int)e != ev && *(int *)(e + 0xb4) == *(int *)(ev + 0xb4) && *(int *)(e + 0xac) == *(int *)(ev + 0xac))
            *(int *)(ev + 0x70) = -1;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xb4));
    if (b != 0 && *(int *)(b + 0x65) == -1)
        *(int *)(ev + 0x70) = -1;
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p != 0 && *(unsigned char *)(p + 0x1b1) != 0)
        *(int *)(ev + 0x70) = -1;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
}
/* 0x560900, the run handler of the prison visit / arrest event (dummy "dummy_EINGANG_ZELLE", the news "_NACHRICHTEN_HS_71" / "_26" / "_72" / "_73"; ROUGH skeleton, 2096 bytes): the guard and the prisoner (+0xac, group 0x524260, 0x4c5230 / 0x4c52e0 send them, 0x524770 arrival), the cell entrance and the sentence by a roll (util_RandomF, gm_BuildingSkill, 0x5ba6b0 skill), the news to the player (0x53dee0 / 0x53dda0) and the release; the end frees the record */

void he_Unk560900(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, b;
    char text[0x200];

    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (p == 0 || b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    gm_GetGebaeudeHauptraum(b);
    cm_SendChrStop(*(int *)(p + 4));
    cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "dummy_EINGANG_ZELLE");
    txt_Format(text, 0x1bba, b);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_71", 0);
    sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
/* 0x561130, the allocation handler: the due clock and the clock +0xb4 are now (copy of the game clock twice) */

void he_Unk561130(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0xb4) = ai_Time;
}

unsigned char *gm_NearestBuilding(unsigned char *b, unsigned kind);
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);

/* 0x561190, the run handler of the ransom event (a thief gang, "L\366segeld" 0x627978; ROUGH skeleton, 736 bytes): states -2 / -1 free; the victim figure +0xac (0x505df0) and the building (0x501190, +0x65 the owner link) are looked up, the news 0x19da (0x5fb) / 0x19c2 (0x5f0) tells the victim and the owner (0x53dda0, 0x53eec0 (-2)), the victim walks to the hideout (0x4c52e0 "L\366segeld", 0x55eba0 scene, 0x4c3ba0 / 0x4c3c00 / 0x4c49a0 mark it, the job byte +0x1b1 1 .. 4 selects the text) and the record is freed */
void he_Unk561190(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, b;
    char text[0x200];

    if (st == -2 || st == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (p == 0 || b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    txt_Format(text, 0x19da, b);
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fb, 0);
    he_FindFigurePlace(*(int *)(p + 0x184));
    cm_SendChrGotoBuilding(*(int *)(p + 4), -1, -1, 0, 0, "L\366segeld");
    cm_BeginFieldBlock(p, *(int *)(p + 4));
    cm_AddFieldDelta(1, 1, 0, text);
    cm_SendFieldDeltas();
    he_FreeEvent((int *)ev);
}
/* 0x561470, the allocation handler the events 0x39 and 0x3e share: the due clock is now + 1 hour + a random
   0 .. 0x1d minutes less 0x14 */

void he_AllocEvent39_3e(int ev)
{
    AIJOB *e = (AIJOB *)ev;

    e->end = ai_Time;
    gm_AddTime(&e->end, 1, (unsigned short)util_Random(0x1e) - 0x14, 0);
}
/* 0x5614c0, the run handler of the ransom demand ("locations\diebesgilde\Diebesgilde_Loesegeld_Msg" window; ROUGH skeleton, 1376 bytes): opens the thieves guild message (0x53d6f0, d2_SetForm), shows the demand (0x515960, the news 0x53dda0), the answer pays (0x4c4830 / 0x4c5fa0, 0x4c3ba0 / 0x4c3c00 / 0x4c49a0, the player record) or refuses (a random reaction 0x53dee0), the thief figure leaves (0x55eba0, 0x4c52e0); the window is closed and the record freed */

void he_Unk5614c0(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st < 0 || st > 1) {
        if (*(int *)(ev + 0x74) != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (st == 0) {
        he_OpenEventSlot(ev, 0, 0, (int)"locations\\diebesgilde\\Diebesgilde_Loesegeld_Msg", 0x5fd);
        if (*(int *)(ev + 0x74) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (U_I((unsigned)&d2_ClickedId) == -1)
        return;
    if (U_I((unsigned)&d2_ClickedId) == 0)
        cm_SendMoveGoodsUnchecked(*(int *)(ev + 0xac), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, *(int *)(ev + 0xb0));
    ((void (*)(int))cm_SendMessage5b)(*(int *)(ev + 0xac));
    he_ReleaseSlot(ev);
    he_FreeEvent((int *)ev);
}
/* 0x5619a0, a Ghidra miss: the allocation handler of the event 0x3f: an older event of the type with the same +0xac makes this one -1 (free on its first run); the first run is now + 1 */

void he_Unk5619a0(int ev)
{
    int o;

    for (o = (int)he_FindEvent(1, 0, 0x3f); o != 0; o = (int)he_NextEvent()) {
        if (o != ev && *(int *)(o + 0xac) == *(int *)(ev + 0xac)) {
            *(int *)(ev + 0x70) = -1;
            break;
        }
    }
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
}
/* 0x561a20, the run handler "Einbruch" (the burglary walk; 8 states by the jump table 0x5628bc; ROUGH): state 0 sends the burglars (the group 0x524260 (building, .., id, ev), 0x4c5230, 0x4c52e0 "Einbruch" 0x627a24) towards the building +0xac (none: state -1), state 1 waits for their arrival (0x524770, a figure count of 8) and goes on with the entry in 0x561c20; the clock is posted by 0x4fe140 */

void he_Unk561a20(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, i, p;

    if (st < 0 || st > 7)
        return;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    for (i = 0; i < 8; i++) {
        p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
        if (p == 0)
            continue;
        if (st == 0) {
            sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
            cm_SendChrStop(*(int *)(p + 4));
            cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "Einbruch");
        } else {
            sv_IsAtDoor(p, 0);
        }
    }
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0x70) = st + 1;
}
/* 0x561c20, the run handler of the burglary ("Einbruch" / "Entdeckt", the news "_NACHRICHTEN_HS_76" / "_77", the formats "$Z%T$A" and "%i %s$A"; ROUGH skeleton, 3264 bytes): the detection check 0x553ae0 / 0x4f2e30, the guards (0x53e400 over the type records, 0x4c4ed0), the loot by a random roll (util_Random, 0x5085f0) taken from the owner (0x4c4830), the news to the owner and the thieves (0x53dda0) and the end releases the group and frees the record */

void he_Unk561c20(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b;
    char text[0x200];

    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    txt_Format(text, 0x1bba, b);
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fb, 0);
    ((void (*)(int))gm_PlayerFactor)((unsigned short)util_Random(100));
    cm_SendMoveGoodsUnchecked(*(int *)(ev + 0xac), -1, 0, 0);
    he_FreeEvent((int *)ev);
}
/* 0x5628e0, the run handler "GebSpionage" (spying on a building; ROUGH skeleton, 2064 bytes): like the espionage walk 0x55a030 the spies (group 0x524260 (building, .., id, ev), 0x4c5230, 0x4c52e0 "GebSpionage") walk to the building (0x524770), look around (gm_SkillPoints, 0x5090b0, _ftol rolls), the owner is told (the news "_NACHRICHTEN_HS_78", 0x501ce0 shows the building) and the group released; the record is freed at the end */

void he_Unk5628e0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, p;
    char text[0x200];

    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (b == 0 || p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    cm_SendChrStop(*(int *)(p + 4));
    cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "GebSpionage");
    sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
    gm_GetGebaeudeHauptraum(b);
    txt_Format(text, 0x1bba, b);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_78", 0);
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
/* 0x563070, a Ghidra miss: the allocation handler of the event 0x48: the clocks +0x52 and +0xcc are now, the run at +0xcc 0x30 units ahead and at +0x52 one unit ahead */

void he_Unk563070(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0xcc) = ai_Time;
    gm_AddTime(ev + 0xcc, 0, 0x30, 0);
    gm_AddTime(ev + 0x52, 0, 0, 1);
}
/* 0x5630f0, the run handler of the town gate event ("STADT"; ROUGH skeleton, 2192 bytes): the job handle (0x4c3ae0), the group (0x524260 (.., id, ev), 0x4c52e0), the scene (d3if_SelectScene, d3_FindObject / d3_Unk458a20 positions, d3_VecEqual against the gate position, 0x541280 / 0x50aa00 / 0x4e5050 helpers) and the random walk (util_Random) are driven by the state; the clock is posted by 0x4c4c60 and 0x4fe140 */

void he_Unk5630f0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p;

    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
    cm_SendChrGotoBuilding(*(int *)(p + 4), -1, -1, 0, 0, "STADT");
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
}
/* 0x563980, the allocation handler: due clock now + 1 */

void he_Unk563980(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
}
/* 0x5639c0, the run handler "Angriff" (the attack of a gang on a building; ROUGH skeleton, 2336 bytes): the attackers (group 0x524260, 0x4c5230, 0x4c52e0 "Angriff") walk to the building (0x524770), the victims of the query list (gm_Query / gm_QueryNext, util_Random) are picked, the fight is started (0x4c4fa0, 0x4d3d10) and the news 0x5176d0 / 0x53dee0 shown; the record is freed at the end */

void he_Unk5639c0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, p;

    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (b == 0 || p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    cm_SendChrStop(*(int *)(p + 4));
    cm_SendChrGotoBuilding(*(int *)(p + 4), *(int *)(b + 1), -1, 0, 0, "Angriff");
    sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
/* 0x5642e0, picks the target of a fire (ROUGH): the first building of the list (0x501190 (1, 1, ..), checks 0x502520 / 0x5024f0, kind 3 buildings are skipped) near the scene object of the reference building (0x458a20 positions, 0x453e80 against the range), at random (0x42ec00) with up to 0x10 tries; returns the building or 0 */

int he_Unk5642e0(int ref)
{
    int b, n;

    for (n = 0; n < 0x10; n++) {
        b = gm_BuildQuery(1, 1, (unsigned short)util_Random(0x100));
        if (b == 0)
            continue;
        if (gm_IsHomeType(b) != 0 || gm_IsType10(b) != 0)
            continue;
        if (*(unsigned char *)b == 3)
            continue;
        if (b != ref)
            return b;
    }
    return 0;
}
/* 0x5644e0, the allocation handler of the fire event (sounds "Brand_Beginn" / "Brand_Ende" 0x4c27d0; ROUGH): the burning building and a second one (0x458a20 positions), a message record (0x4c4b70), the other fires of the type list (0x53e400 / 0x53e300) are checked and the due clock is now + 0x14 or 7 units by the minute word (0x42ec00 rolls) */

void he_Unk5644e0(int ev)
{
    char *e;
    int b;
    unsigned char m[0xf8];

    snd_QueueVoice(-7, 0, "Brand_Beginn", 0, 0);
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    (void)b;
    memset(m, 0, sizeof(m));
    cm_SendLongMessage(m);
    for (e = he_FindEvent(1, 0, 0x3d); e != 0; e = he_NextEvent()) {
        if ((int)e != ev)
            break;
    }
    snd_QueueVoice(-7, 0, "Brand_Ende", 0, 0);
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, *(unsigned short *)(ev + 0x56) >= 0x14 ? (unsigned short)util_Random(5) : 7, 0);
}
/* 0x564860, the allocation handler of the fire event (ROUGH): the clocks +0x52 / +0x60 are now, +0xb4 = 0, the building +0xac (none: free), the nearest building (0x501e40, 0x458a20 positions) gives the distance scaled by +0xb0 (_ftol) as the burning time +0xb0 and +0x84 = -1 */

void he_Unk564860(int ev)
{
    int b;

    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0x60) = ai_Time;
    *(unsigned char *)(ev + 0xb4) = 0;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    gm_NearestBuildingK(b);
    *(int *)(ev + 0x84) = -1;
    *(int *)(ev + 0xb0) = (int)((float)*(int *)(ev + 0xb0) * 1.0f);
}
/* 0x5649b0, the run handler of the fire ("Brand_l" / "Stichflamme_s" sounds, "Brand_Betroffen", "dummy_FAHNE", the script "specialEvents\hausbrand.esc", "brand_he_%i_%i"; ROUGH skeleton, 2655 bytes): the fire objects are placed at the dummies of the burning building (d3_FindObject, d3_Unk458a20, util_Random), the sounds attached and played (sb_GetSample, d3snd_Attach / d3snd_Play / d3snd_PlayAt), the script loaded and run (evt_Load, evt_RunScriptArgs), the news to the owner (0x53dee0) and the burning handled by the state until it goes out */

void he_Unk5649b0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b;
    char name[0x40];

    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    sprintf(name, "brand_he_%i_%i", *(int *)(ev + 0xac), st);
    snd_QueueVoice(-7, 0, "Brand_l", 0, 0);
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
/* 0x5652b0, a Ghidra miss: the allocation handler of the event 0x50 (ROUGH): the characters of kind 6 (0x501190 (1, 6) / 0x501030) without 0x502520 / 0x5024f0 and with a +0x61 object are collected (at most 16 are kept as targets); +0xb0 .. +0xeb = -1; the chosen
   character +0xac (random pick when -1) gets the nearby ones within +0xf0 (0x453e80 on their +0x61 positions + 0x4c) as followers at +0xac + n * 4; +0x100 / +0x104 = 0; the run clock is now */

void he_Unk5652b0(int ev)
{
    int list[0x100];
    int n, i, o, c, k;

    n = 0;
    list[0] = 1;
    for (o = gm_BuildQuery(1, 6); o != 0; o = gm_BuildNext()) {
        if (gm_IsHomeType(o) == 0 && gm_IsType10(o) == 0 && *(int *)(o + 0x61) != 0)
            list[n++ & 0xffff] = o;
    }
    for (i = 0; i < 0xf; i++)
        *(int *)(ev + 0xb0 + i * 4) = -1;
    if (*(int *)(ev + 0xac) == -1) {
        c = list[(unsigned short)util_Random(n)];
        *(int *)(ev + 0xac) = *(int *)(c + 1);
    } else {
        c = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    }
    k = 0;
    n &= 0xffff;
    for (i = 0; i < n && (unsigned short)k < 0x10; i++) {
        o = list[i];
        if (o != c && d3_VecEqual(*(int *)(o + 0x61) + 0x4c, *(int *)(c + 0x61) + 0x4c, (float)*(int *)(ev + 0xf0)) != 0)
            *(int *)(ev + 0xac + (unsigned short)k++ * 4) = *(int *)(o + 1);
    }
    *(int *)(ev + 0x100) = 0;
    *(int *)(ev + 0x104) = 0;
    *(AITIME *)(ev + 0x52) = ai_Time;
}
/* 0x565410, the run handler of the storm event (type 0x50, its alloc handler 0x5652b0): state 0 writes the storm forecast into
   sv_Weather for the three hours after now (clouds 1000, storm 3) and runs again two hours ahead; state 1 damages the stock of
   the 16 buildings the alloc handler collected (gm_ChangeBuildingState, -(random(5) + 5)) once, then frees the record */

void he_RunStormEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIBLD *b;
    unsigned base;
    int i, last;

    switch (e->stage) {
    case 0:
        base = ai_TimeHourW;
        i = (base & 0xffff) + 1;
        last = (base & 0xffff) + 4;
        if (i < last) {
            do {
                sv_Weather.clouds[i] = 1000;
                sv_Weather.storm[i] = 3;
                i++;
            } while (i < last);
        }
        e->end.hour = base + 2;
        e->end.min = 0;
        e->stage = e->stage + 1;
        return;
    case 1: {
        int n, dmg;
        AIBLD *b;
        int *bldIds;

        if ((e->opts & 2) != 0 && e->stormDue != 0) {
            bldIds = e->stormBlds;
            n = 0x10;
            do {
                if (*bldIds != -1 && (b = (AIBLD *)gm_BuildQuery(1, 1, *bldIds)) != 0) {
                    dmg = (unsigned short)util_Random(5) + 5;
                    gm_ChangeBuildingState((unsigned char *)b, -dmg);
                }
                bldIds++;
                n--;
            } while (n != 0);
            e->stormDue = 0;
        }
        he_FreeEvent((int *)ev);
        return;
    }
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        return;
    default:
        return;
    }
}
/* 0x565500, the allocation handler of the "player lost" event (ROUGH): the player record 0x505df0 (+0xac), the object 0x502020 (+0x10), the follow-up 0x4fe980 (+0xc4), 0x524260 (obj, follow, player id, ev); the value +0xb0 = 200, the clocks +0xb4 / +0x44 / +0x52 / +0x60 = now, the next run 400 units ahead */

void he_Unk565500(int ev)
{
    unsigned short *p;
    int o, f;

    f = 0;
    p = (unsigned short *)gm_FindChar(*(int *)(ev + 0xac));
    o = gm_FindBuildingById(*(int *)(ev + 0x10));
    if (*(int *)(ev + 0xc4) != -1)
        f = gm_FindObjectInst(*(int *)(ev + 0xc4));
    sv_MoveToGroup(o, f, *p, ev);
    *(int *)(ev + 0xb0) = 200;
    *(AITIME *)(ev + 0xb4) = ai_Time;
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0xb4, 0, 400, 0);
}

/* 0x565620, a Ghidra miss: the run handler of the healing event ("Heilen", type 0x6c): state 0 heals the player 0x505df0 (+0xac) step by step: the elapsed time since +0x60 (0x4fe280) * 0.1 * 5 is the step, the stat 0x4fc8e0 times 100.0
   plus the step must stay below 100.0 (0x4fca40 (id, ftol (step)), the rest +0xb0 -= step, the next run 10 units ahead), else the next state; +0x60 = now; state 1 releases the lock 0x524260 (obj +0x10, 0, id) and sends the
   news 0x1a18 ("_NACHRICHTEN_HS_80") then frees; -1 / -2 release the lock and free (ROUGH) */
float gm_HealthRatio(short *c);

/* 0x565620 */
void he_Unk565620(int ev)
{
    int p, o, d;
    float step;
    float v;
    char text[0x200];

    switch (*(int *)(ev + 0x70)) {
    case 0:
        p = gm_FindChar(*(int *)(ev + 0xac));
        if (p == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d = gm_MinutesBetween(ev + 0x60, (unsigned)&ai_Time);
        step = 5.0f * ((float)d * 0.1f);
        v = gm_HealthRatio(p) * 100.0;
        if (*(unsigned *)(ev + 0xb0) > 0 && step + v < 100.0f) {
            gm_AddHealth(*(unsigned short *)p, (int)step);
            *(int *)(ev + 0xb0) = (int)((double)*(unsigned *)(ev + 0xb0) - step);
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 10, 0);
        } else {
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        }
        *(AITIME *)(ev + 0x60) = ai_Time;
        return;
    case 1:
        p = gm_FindChar(*(int *)(ev + 0xac));
        o = gm_FindBuildingById(*(int *)(ev + 0x10));
        if (p != 0 && o != 0) {
            sv_MoveToGroup(o, *(unsigned short *)p, 0, 0);
            txt_Format(text, 0x1a18, o, *(unsigned short *)p);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5f2, *(int *)(ev + 0x10), -1, 0, (unsigned)&full_data_627b0c);
        }
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        p = gm_FindChar(*(int *)(ev + 0xac));
        if (p != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)p, 0);
        he_FreeEvent((int *)ev);
        return;
    default:
        return;
    }
}
/* 0x565850, the run handler that gives a building (+0xb4: the object 0x502020) a new worker (ROUGH): kind 2 buildings get a random hire 0x4c4650 (2, -1, -1, random(5, ..) + 0x16), the others the work 0x4c55d0 with the last non-empty slot of the record (+0x223 .. +0x228) and +0x230; the result is stored at +0xac */

void he_Unk565850(int ev)
{
    unsigned char *p;
    char *rec;
    int r, i, n, v, u;
    char c;

    c = 0;
    p = (unsigned char *)gm_FindBuildingById(*(int *)(ev + 0xb4));
    if (p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    rec = (char *)(U_I((unsigned)&ai_BTypes) + *p * 0x24d);
    if (*rec == 2) {
        u = (unsigned short)((int (*)(int, int))util_Random)(0xc, 1);
        v = gm_TitleNumber(u + 1);
        n = ((int (*)(int, int, int, int, int))util_Random)(5, -1, v, 0, 2);
        r = ((int (*)(int, int, int, int))cm_SendCreateCharacter)(2, -1, -1, n + 0x16);
    } else {
        i = 5;
        do {
            if (rec[i + 0x223] != 0) {
                c = rec[i + 0x223];
                break;
            }
            i--;
        } while (-1 < i);
        r = cm_SendMessage3d(p, 1, rec[0x230], c);
    }
    *(int *)(ev + 0xac) = r;
    *(AITIME *)(ev + 0x52) = ai_Time;
}
/* 0x565940, a Ghidra miss: the run handler of the event 0x6e (ROUGH): when the sound handle +0xac is done (0x4c3ae0) the result 0x4c3b10 and the object +0xb4 (0x502020) decide: 0x4c54a0 (obj + 1, the owner (+0x25) player record 0x770b84 or the result), the reward
   0x4c47e0 (result +4, -1, 0x6f1bc5, 0x5085f0 (0xbb8, 0x6f1bc5)); with a value +0xbc the owner (kinds 6 / 7: the news 0x17e6) gets the share 0x4c47e0 and 0x55ea60 yields a target for 0x4c52e0 (id, target, 0, 0, 0x627b20); always freed; -1 / -2 free */

void he_Unk565940(int ev)
{
    int st, r, o, own;
    int v[2];
    char text[0x400];

    st = *(int *)(ev + 0x70);
    if (st < -2)
        return;
    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (st != 0)
        return;
    if (cm_GetSlotState(*(int *)(ev + 0xac)) == 0)
        return;
    r = cm_GetSlotValue(*(int *)(ev + 0xac));
    o = gm_FindBuildingById(*(int *)(ev + 0xb4));
    if (r != 0 && o != 0) {
        if (*(unsigned short *)(o + 0x25) != 0xffff)
            cm_SendOwnership(*(int *)(o + 1), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(o + 0x25) * 0x21c), *(int *)(r + 4));
        else
            cm_SendOwnership(*(int *)(o + 1), *(int *)(r + 4), *(int *)(r + 4));
        cm_SendMoveGoods(*(int *)(r + 4), -1, U_I((unsigned)&ai_OwnPlayer), gm_PlayerFactor(0xbb8, *(unsigned char *)(unsigned)&ai_OwnPlayer));
        if (*(int *)(ev + 0xbc) != 0) {
            own = (unsigned)&ai_Chars + *(unsigned short *)(o + 0x25) * 0x21c;
            if (own != 0) {
                if (*(char *)(own + 2) == 6 || *(char *)(own + 2) == 7) {
                    txt_Format(text, 0x17e6, o, *(unsigned short *)r);
                    he_MessageBoxQuickjump(*(int *)(own + 4), -1, text, 0, 0x5ec, *(int *)(o + 1), -1, 0, (unsigned)"_NACHRICHTEN_HS_30");
                }
                cm_SendMoveGoods(*(int *)(own + 4), -1, U_I((unsigned)&ai_OwnPlayer), *(int *)(ev + 0xbc));
            }
            cm_SendOwnership(*(int *)(o + 1), *(int *)(r + 4), *(int *)(r + 4));
            /* the original caller pushes 3 arguments (0x565ab5 push ecx, 0x565ab6 push edx, 0x565ab7 push edi) to the
               1-argument 0x55ea60, which reads only [esp+8] (0x55ea61); the extra pushes are the original bytes */
            if (he_FigureContact(r, &v[0], &v[1]) != 0)    /* @original: 3 args to the 1-arg 0x55ea60 (see above) */
                cm_SendChrGotoBuilding(*(int *)(r + 4), v[0], v[1], 0, 0, (unsigned)&full_data_627b20);
        }
    }
    he_FreeEvent((int *)ev);
}
/* 0x565b00, the allocation handler "he_AllocDiePlayer": the player 0x505df0 (+0xac) must exist (else the error message and free); his flag byte +8 is cleared, the next run is now */

void he_Unk565b00(int ev)
{
    int p;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        err_Message("he_AllocDiePlayer: player not found!");
        he_FreeEvent((int *)ev);
        return;
    }
    *(char *)(p + 8) = 0;
    *(AITIME *)(ev + 0x52) = ai_Time;
}
/* 0x565b60, the run handler of the death of a player (ROUGH): state 0 removes the follow object of his figure (0x4284a0 / 0x429e90) and clears his lock 0x524260; state 1 waits until the figure object (+0x184, its +0x28) is gone; state 2 clears the flag +8, the lock, the figure +0x16c and lays the figure down (0x405610, 0x40b2f0 (obj, 0x36, 1): the animation name "bewegung/zu_boden_sinken" at +0xf0), next run at once; state 3 informs the owner 0x4c4e00 (+4, 1) when the flag +0x78 & 2 is set; then freed */

void he_Unk565b60(int ev)
{
    unsigned short *p;
    int q, o, a;
    char *s, *d;

    switch (*(int *)(ev + 0x70)) {
    case 0:
        p = (unsigned short *)gm_FindChar(*(int *)(ev + 0xac));
        if (p != 0) {
            if (*(int *)(p + 0xc2) != 0 && (o = *(int *)(*(int *)(p + 0xc2) + 0x28)) != -1)
                evt_Stop(evt_FindById(o));
            sv_MoveToGroup(0, 0, *p, 0);
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        break;
    case 1:
        q = gm_FindChar(*(int *)(ev + 0xac));
        if (q != 0) {
            if (*(int *)(q + 0x184) != 0 && *(int *)(*(int *)(q + 0x184) + 0x28) != -1)
                return;
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        break;
    case 2:
        p = (unsigned short *)gm_FindChar(*(int *)(ev + 0xac));
        if (p != 0) {
            *(char *)(p + 4) = 0;
            sv_MoveToGroup(0, 0, *p, 0);
            *(int *)(p + 0xb6) = 0;
            if (*(int *)(p + 0xc2) != 0) {
                ch_StopActions(*(int *)(p + 0xc2));
                a = ch_AddAction(*(int *)(p + 0xc2), 0x36, 1);
                strcpy((char *)(a + 0xf0), "bewegung/zu_boden_sinken");
            }
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        break;
    case 3:
        q = gm_FindChar(*(int *)(ev + 0xac));
        if (q != 0 && (*(unsigned char *)(ev + 0x78) & 2) != 0)
            cm_SendKillCharacter(*(int *)(q + 4), 1);
        break;
    case -2:
    case -1:
        break;
    default:
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x565d10, the allocation handler "he_AllocKillPlayer" (ROUGH): the player 0x505df0 (+0xac) must exist (else the error message and free); when another event of the type 0x72 already kills him this one is freed, else
   +0xb4 / +0xb0 / +0x84 = -1 and the run clock is the start clock (+0x44) */

void he_Unk565d10(int ev)
{
    int p, o;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        err_Message("he_AllocKillPlayer: player not found!");
        he_FreeEvent((int *)ev);
        return;
    }
    for (o = (int)he_FindEvent(1, 0, 0x72); o != 0; o = (int)he_NextEvent()) {
        if (o != ev && *(int *)(o + 0xac) == *(int *)(ev + 0xac)) {
            he_FreeEvent((int *)ev);
            return;
        }
    }
    *(int *)(ev + 0xb4) = -1;
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(int *)(ev + 0xb0) = -1;
    *(int *)(ev + 0x84) = -1;
}
/* 0x565db0, the run handler of the kill-player event (ROUGH, 1472 bytes with the jump table 0x5662a8; the Ghidra function 0x565d10 stops before it: a Ghidra miss; the code at 0x566030 is a case of it, not a
   function): -1 / -2 free; while the player (0x505df0 (+0xac)) has +0x20c set (and the state is not 6) the next run is 5 units ahead; state 0: removes the figure object (+0x184: 0x429e90 (0x4284a0 (+0x28))), the lock
   0x524260 (0, 0, id, 0), with the flag +0x78 & 2 the news 0x42f4 for the opponent figure (+0x16c: 0x53dee0, 9 parameters), +0x16c = 0; state 1 waits until the figure is gone; state 2 lays the figure down (0x405610,
   0x40b2f0 (fig, 0x36, 1), the animation "bewegung/zu_boden_sinken") and waits 0x1e units (frees at once with +0x78 & 4); state 3: figure kinds 6 / 7 build the message 0x114 bytes (type 8, the clock, +0x36 = 0xa0,
   the player +4) for 0x4c4fa0 (+0xb4) and go to state 6, the others wait 2 and go to 4; state 4: 0x493ea0 finds the successor h (none: state 5), 0x506890 (p, h), kinds 5 / 6 / 7 whose +0xd level exceeds h's raise
   h (0x4c3ba0 / 0x4c3c00 (1, 1, 0xd, &level) / 0x4c49a0), 0x4c4e00 (+4, 0x5075c0 (p)), free; state 5: 0x506890 (p, 0x6f1bf8), 0x4c4e00, free; state 6 polls 0x4c3ae0 (+0xb4) (0: wait, 2: back to state 4, else +0xb0
   = *0x4c3b10 and state 7); state 7 waits for 0x4e50a0 (+0xb0) and the dead flag +8, frees or goes to state 4 */

void he_Unk565db0(int ev)
{
    int p, f, o, h, q, a, st, idx;
    unsigned char m[0x114];
    char text[0x200];
    char kind;

    st = *(int *)(ev + 0x70);
    if (st == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(int *)(p + 0x20c) != -1 && st != 6) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        return;
    }
    switch (st) {
    case 0:
        f = *(int *)(p + 0x184);
        if (f == 0) {
            if ((*(unsigned char *)(ev + 0x78) & 2) == 0) {
                he_FreeEvent((int *)ev);
                return;
            }
            *(int *)(ev + 0x70) = 3;
            return;
        }
        o = *(int *)(f + 0x28);
        if (o != -1)
            evt_Stop(evt_FindById(o));
        sv_MoveToGroup(0, 0, *(unsigned short *)p, 0);
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0 && (q = *(int *)(p + 0x16c)) != 0) {
            idx = *(unsigned short *)(q + 0x27);
            kind = *(char *)(((unsigned)&ai_Chars + 0x2) + idx * 0x21c);
            if (kind == 6 || kind == 7) {
                txt_Format(text, 0x42f4, *(unsigned short *)p, q);
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + idx * 0x21c), *(int *)(q + 1), text, 0, 0x60a, *(int *)(q + 1), -1, (unsigned)&ch_EmptyName, (unsigned)&ch_EmptyName);
            }
        }
        *(int *)(p + 0x16c) = 0;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        f = *(int *)(p + 0x184);
        if (f != 0 && *(int *)(f + 0x28) != -1)
            return;
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 2:
        f = *(int *)(p + 0x184);
        if (f != 0) {
            ch_StopActions(f);
            a = ch_AddAction(*(int *)(p + 0x184), 0x36, 1);
            if (a != 0)
                strcpy((char *)(a + 0xf0), "bewegung/zu_boden_sinken");
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
            he_FreeEvent((int *)ev);
        return;
    case 3:
        if (*(char *)(p + 2) != 6 && *(char *)(p + 2) != 7) {
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x70) = 4;
            return;
        }
        memset(m, 0, sizeof(m));
        m[0x18] = 8;
        m[0x36] = 0xa0;
        *(AITIME *)(m + 0x1c) = ai_Time;
        gm_AddTime(m + 0x1c, 0, 0x1e, 0);
        h = *(int *)(p + 4);
        *(int *)(m + 0x2c) = h;
        *(int *)(m + 0x28) = -1;
        m[0x44] = 1;
        *(int *)(m + 0x48) = h;
        *(int *)(ev + 0xb4) = ((int (*)(void *))cm_SendNewParty)(m + 0x10);
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = 6;
        return;
    case 4:
        h = ai_FindSuccessor(p);
        if (h == 0) {
            *(int *)(ev + 0x70) = 5;
            return;
        }
        gm_InheritEstate(p, h);
        kind = *(char *)(p + 2);
        if ((kind == 6 || kind == 7 || kind == 5) && *(unsigned char *)(p + 0xd) > *(unsigned char *)(h + 0xd)) {
            cm_BeginFieldBlock(h, *(int *)(h + 4));
            cm_AddFieldDelta(1, 1, 0xd, p + 0xd);
            cm_SendFieldDeltas();
        }
        cm_SendKillCharacter(*(int *)(p + 4), gm_MayRemoveChar(p));
        he_FreeEvent((int *)ev);
        return;
    case 5:
        gm_InheritEstate(p, U_I((unsigned)&ai_PlayerChar));
        cm_SendKillCharacter(*(int *)(p + 4), gm_MayRemoveChar(p));
        he_FreeEvent((int *)ev);
        return;
    case 6:
        a = cm_GetSlotState(*(int *)(ev + 0xb4));
        if (a == 0)
            return;
        if (a == 2) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x70) = 4;
            return;
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0xb0) = *(int *)cm_GetSlotValue(*(int *)(ev + 0xb4));
        gm_AddTime(ev + 0x52, 2, 0, 0);
        *(int *)(ev + 0x70) = 7;
        return;
    case 7:
        if (*(unsigned char *)(p + 8) != 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        if (cm_PartyById(*(int *)(ev + 0xb0)) == 0) {
            *(int *)(ev + 0xb4) = -1;
            *(int *)(ev + 0xb0) = -1;
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x70) = 4;
            return;
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 2, 0, 0);
        return;
    case -2:
        he_FreeEvent((int *)ev);
        return;
    default:
        return;
    }
}
/* 0x5662d0, the allocation handler of the night watchman (type 0x73; a Ghidra miss): the clocks +0x52 / +0x44 are now, the building / player +0xac = random(5) - 5 (as the last argument of the run: a signed pick), the first run at the morning hour 0x617830 [season] (+0x56 = ftol, +0x58 = 0) */

void he_Unk5662d0(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(int *)(ev + 0xac) = (unsigned short)util_Random(5) - 5;
    *(short *)(ev + 0x56) = (short)(int)((float *)(unsigned)&gm_DayStartHours)[gm_ClockSeason((unsigned)&ai_Time)];
    *(int *)(ev + 0x58) = 0;
}
/* 0x566370, the run handler of the night watchman (a Ghidra miss, 1216 bytes with the jump table 0x566814): freed when the day changed (+0x44 != the day 0x770ae0); the season 0x4fe350 indexes the float tables 0x617830 (morning
   hour 8 / 7 / 8 / 9) and 0x617840 (evening hour 20 / 21 / 20 / 19); state 0 waits for the morning hour (hour in [h, h + 1)), then plays the bell "_NACHTWAECHTER_GLOCKEN_SFX_SHORT" (0x6f25ac: sound on) and with 0x6f3619
   the announcements (random(10) > 7: "_NACHTWAECHTER_ANKUENDIGUNGEN", the morning time text, "_NACHTWAECHTER_MORGENS_SPRUECHE", 0x4c27d0 (player, 0, text, -1, volume)) and sets the next run to the evening hour; state 1
   the same for the evening hour ("ABENDS_ZEIT_7 / _8 / _8_HERBST / _9", "ABENDS_SPRUECHE"), next run 22:30; state 2 at hour 22 and minute 30 .. 49 plays the bell and "NACHTS_ENDE", then frees (ROUGH) */

void he_Unk566370(int ev)
{
    int season;
    float h, m;
    int id;

    if (U_I((unsigned)&ai_Time) != *(int *)(ev + 0x44)) {
        he_FreeEvent((int *)ev);
        return;
    }
    season = gm_ClockSeason((unsigned)&ai_Time);
    id = *(int *)(ev + 0xac);
    switch (*(int *)(ev + 0x70)) {
    case 0:
        h = (float)(int)(U_I((unsigned)&ai_Time.hour) & 0xffff);
        m = ((float *)(unsigned)&gm_DayStartHours)[season];
        if (h < m || !(h < m + 1.0f)) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            goto lagain;
        }
        if (*(char *)(unsigned)&ai_GameState != 0)
            snd_QueueVoice(-7, 0, "_NACHTWAECHTER_GLOCKEN_SFX_SHORT", -1, 0xfa);
        if (*(char *)((unsigned)&ai_GameSwitches + 0x1) != 0 && *(char *)(unsigned)&ai_GameState != 0 && (unsigned short)util_Random(10) > 7 && *(char *)(unsigned)&ai_GameState != 0) {
            snd_QueueVoice(id, 0, "_NACHTWAECHTER_ANKUENDIGUNGEN", -1, 0xfa);
            if (m == 7.0f) {
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_ZEIT_7", -1, 0xfa);
            } else if (m == 8.0f) {
                if (season == 2)
                    snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_ZEIT_HERBST", -1, 0xfa);
                else
                    snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_ZEIT_8", -1, 0xfa);
            } else if (m == 9.0f) {
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_ZEIT_9", -1, 0x3e8);
            }
            if ((unsigned short)util_Random(10) > 7)
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_MORGENS_SPRUECHE", -1, 0x3e8);
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        *(short *)(ev + 0x56) = (short)(int)((float *)(unsigned)&gm_DayEndHours)[season];
        *(int *)(ev + 0x58) = 0;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        h = (float)(int)(U_I((unsigned)&ai_Time.hour) & 0xffff);
        m = ((float *)(unsigned)&gm_DayEndHours)[season];
        if (h < m || !(h < m + 1.0f)) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            goto lagain;
        }
        if (*(char *)(unsigned)&ai_GameState != 0)
            snd_QueueVoice(-7, 0, "_NACHTWAECHTER_GLOCKEN_SFX_SHORT", -1, 0xfa);
        if (*(char *)((unsigned)&ai_GameSwitches + 0x1) != 0 && *(char *)(unsigned)&ai_GameState != 0 && (unsigned short)util_Random(10) > 7 && *(char *)(unsigned)&ai_GameState != 0) {
            snd_QueueVoice(id, 0, "_NACHTWAECHTER_ANKUENDIGUNGEN", -1, 0xfa);
            if (m == 19.0f) {
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_ZEIT_7", -1, 0xfa);
            } else if (m == 20.0f) {
                if (season == 2)
                    snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_ZEIT_8_HERBST", -1, 0xfa);
                else
                    snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_ZEIT_8", -1, 0xfa);
            } else if (m == 21.0f) {
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_ZEIT_9", -1, 0xfa);
            }
            if ((unsigned short)util_Random(10) > 7)
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_ABENDS_SPRUECHE", -1, 0x3e8);
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        *(short *)(ev + 0x56) = 0x16;
        *(int *)(ev + 0x58) = 0x1e;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 2:
        if (*(unsigned short *)(unsigned)&ai_Time.hour < 0x16 || (int)U_I(((unsigned)&ai_Time + 0x6)) < 0x1e || (int)U_I(((unsigned)&ai_Time + 0x6)) >= 0x32) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            goto lagain;
        }
        if (*(char *)(unsigned)&ai_GameState != 0) {
            snd_QueueVoice(-7, 0, "_NACHTWAECHTER_GLOCKEN_SFX_SHORT", -1, 0xfa);
            if (*(char *)((unsigned)&ai_GameSwitches + 0x1) != 0 && (unsigned short)util_Random(10) > 7)
                snd_QueueVoice(id, 0, "_NACHTWAECHTER_NACHTS_ENDE", -1, 0xfa);
        }
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        return;
    default:
        return;
    }
lagain:
    gm_AddTime((unsigned)&ai_Time, 0, 0, 1);
}
/* 0x566830, the run handler of the attack event with up to eight attackers (the player ids at +0x8c .. +0xab, -1 = free; the target building +0xac, the attacker building +0xb0; a Ghidra miss, 3568 bytes with the jump
   table 0x5675f4 (states -2 .. 5); ROUGH skeleton: states 0 .. 3 follow the disassembly, 4 / 5 (the fight result: loot 0x4c47e0 / 0x4c4880, the news 0x19e7 / 0x19e9 / 0x1a85 / 0x1a86 through 0x53dee0, the
   cases 0x4c3ba0 / 0x4c3d00 (4, 1, 0x65 / 0x69) and 0x4c49a0) are summarised): the end handler (-1 / -2) releases the attackers (0x4c5230, 0x4c52e0 (id, target, leader, 0, 0, 0x627d74), 0x524260 (0, 0, id, 0)) and
   frees; state 0 sends every attacker to the building (0x524260 (b, id, 0, ev), 0x4c5230, 0x4c52e0) and waits 4 units; state 1 waits until every attacker with a figure has arrived (0x524770 (p, b)); state 2 starts the
   fight scene (0x553ae0 on a record {b, 5000.0f, ev, 0}, 0x4f2e30 (0x17, 1, owner, +4, b + 1); +0xb4 / +0xb8 / +0xbc = the scene, the opponent, its value) or, without a scene, ends the attack with a random
   sound text (0x4c5410 from the three names of the table 0x62689c / 0x627d8c) and state 5; state 3 waits for the scene (0x4c3ae0) and the opponent event (0x53e400 (1, 1, +0xb8), day +0xd4 == +4) and then
   hands the scene result (0x4c3b10, 0x4c4ed0) over; state 4 (waits 6 units while the opponent event runs) and state 5 settle the result (ROUGH) */

void he_Unk566830(int ev)
{
    int st, b, p, i, id, lead, e, r, idx, owner;
    int *slot;
    int rec[4];
    char names[3][0x40];
    char text[0x200];

    memset(names, 0, sizeof(names));
    lead = 0;
    st = *(int *)(ev + 0x70);
    switch (st) {
    case -2:
    case -1:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
        if (b != 0)
            lead = gm_GetGebaeudeHauptraum(b);
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p == 0)
                continue;
            cm_SendChrStop(*slot);
            if (b != 0 && lead != 0)
                cm_SendChrGotoBuilding(*slot, *(int *)(b + 1), *(int *)(lead + 2), 0, 0, (unsigned)&full_data_627d74);
            else
                cm_SendChrGotoBuilding(*slot, -1, -1, 0, 0, (unsigned)&full_data_627d74);
            sv_MoveToGroup(0, 0, *(unsigned short *)p, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    case 0:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p == 0)
                continue;
            sv_MoveToGroup(b, *(unsigned short *)p, 0, ev);
            cm_SendChrStop(*slot);
            cm_SendChrGotoBuilding(*slot, *(int *)(b + 1), -1, 0, 1, (unsigned)&full_data_627d74);
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 4, 0);
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p != 0 && *(int *)(p + 0x184) != 0 && sv_IsAtDoor(p, b) == 0) {
                *(AITIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0, 4, 0);
                return;
            }
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0x70) = 2;
        return;
    case 2:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (b != 0) {
            idx = *(unsigned short *)(b + 0x27);
            rec[0] = b;
            rec[1] = 0x451c4000;
            rec[2] = ev;
            rec[3] = 0;
            if (he_Unk553ae0(rec) != 0) {
                *(int *)(ev + 0xb4) = gv_Accuse(0x17, 1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)((unsigned)&ai_Chars + idx * 0x21c + 4), *(int *)(b + 1));
                *(int *)(ev + 0xb8) = *(int *)(rec[3] + 4);
                *(int *)(ev + 0xbc) = *(int *)(rec[3] + 0xb0);
                *(int *)(ev + 0x70) = 3;
                *(AITIME *)(ev + 0x52) = ai_Time;
                return;
            }
        }
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p == 0)
                continue;
            sv_MoveToGroup(b, *(unsigned short *)p, 0, ev);
            cm_SendChrStop(*slot);
            cm_SendChrGotoBuilding(*slot, *(int *)(b + 1), -1, 0, 0, (unsigned)&full_data_627d74);
            ((void (*)(int, const char *))cm_SendChrAnim)(*slot, names[(unsigned short)((int (*)(int, int))util_Random)(3, 1)]);
        }
        *(int *)(ev + 0x70) = 5;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0xf, 0);
        return;
    case 3:
        if (*(int *)(ev + 0xb4) != -1 && cm_GetSlotState(*(int *)(ev + 0xb4)) == 0)
            return;
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        e = (int)he_FindEvent(1, 1, *(int *)(ev + 0xb8));
        if (b != 0 && e != 0 && *(int *)(e + 0xd4) == *(int *)(ev + 4) && *(int *)(ev + 0xb4) != -1) {
            p = gm_FindChar(*(int *)(ev + 0xb8));
            r = cm_GetSlotValue(*(int *)(ev + 0xb4));
            if (p != 0 && r != 0)
                cm_SendProofExecuted(*(int *)r, *(int *)(p + 4));
            *(int *)(ev + 0x70) = 4;
            gm_AddTime(ev + 0x52, 0, 10, 0);
            return;
        }
        *(int *)(ev + 0x70) = -1;
        return;
    case 4:
        e = (int)he_FindEvent(1, 1, *(int *)(ev + 0xb8));
        if (e == 0) {
            *(int *)(ev + 0x70) = -1;
            return;
        }
        if (*(int *)(e + 0xd4) == *(int *)(ev + 4)) {
            gm_AddTime(ev + 0x52, 0, 6, 0);
            return;
        }
        /* the fight result: loot from the opponent building to the attackers, the news of the owner / attacker, the cases
           of the attackers released (ROUGH, see the header) */
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        owner = gm_FindChar(*(unsigned short *)(ev + 8) * 0 + *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c));
        if (b != 0 && owner != 0) {
            ((void (*)(int))gm_PlayerFactor)(0x226 + (unsigned short)((int (*)(int, int))util_Random)(0xc8, U_I((unsigned)&ai_OwnPlayer)));
            txt_Format(text, 0x19ea, b, 0, 0);
            he_ShowNews(*(int *)(owner + 4), *(int *)(owner + 4), text, 1, 0x5f0, (unsigned)&full_data_627a10);
        }
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot == -1)
                continue;
            p = gm_FindChar(*slot);
            if (p != 0)
                gm_AddHealth(*(unsigned short *)p, -100 - (unsigned short)util_Random(100));
        }
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
        if (b == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        lead = gm_GetGebaeudeHauptraum(b);
        for (i = 0, slot = (int *)(ev + 0x8c); i < 8; i++, slot++) {
            if (*slot != -1) {
                cm_SendChrStop(*slot);
                cm_SendChrGotoBuilding(*slot, *(int *)(b + 1), *(int *)(lead + 2), 0, 0, (unsigned)&full_data_6279f0);
            }
        }
        he_FreeEvent((int *)ev);
        return;
    case 5:
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        owner = gm_FindChar(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c));
        if (b != 0 && owner != 0) {
            txt_Format(text, 0x19e7, *(unsigned short *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), 0x19c1, b);
            he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(owner + 4), text, 1, 0x5f0, *(int *)(b + 1), -1, 0, (unsigned)&ch_EmptyName);
        }
        *(int *)(ev + 0x70) = -2;
        return;
    default:
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x567620, a Ghidra miss: the allocation handler of a periodic check, the next run is 20 units ahead */

void he_Unk567620(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x14, 0);
}

float gm_TitlePeriodServed(int n, int *out);

/* 0x567660, a Ghidra miss: the run handler that warns a player once (ROUGH): state 0 with the flag word of the player (0x770d48 + id * 0x21c) negative (already warned) waits 4 units; else when his value 0x504510 (id, &out)
   is below 1.0 waits 20 units, else sends the message ('%', the player, +0x36 = 9, +0x60 = 2) 0x4c4b70, sets the sign bit of the flag word and waits 4 units; -1 / -2 free */
void he_Unk567660(int ev)
{
    int st, id;
    unsigned char m[0xf8];
    double v;
    int out[2];

    st = *(int *)(ev + 0x70);
    if (st < -2)
        return;
    if (st < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (st != 0)
        return;
    id = *(unsigned short *)(ev + 8);
    if (*(int *)(((unsigned)&ai_Selected + 0x40) + id * 0x21c) < 0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 4, 0, 0);
        return;
    }
    v = gm_TitlePeriodServed(id, out);
    if (v < 1.0) {
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x14, 0);
        return;
    }
    memset(m, 0, sizeof(m));
    m[4] = 0x25;
    *(int *)(m + 0xc) = -1;
    m[0x36] = 9;
    *(int *)(m + 0x60) = 2;
    *(int *)(m + 8) = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
    *(int *)(m + 0x5c) = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
    cm_SendLongMessage(m);
    *(unsigned *)(((unsigned)&ai_Selected + 0x40) + id * 0x21c) |= 0x80000000;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 4, 0, 0);
}
/* 0x567810, "he_OpenHelpEvents": reads the tutorial hint script "<name>\project\game\tutorial\<param>.ini" (0x627e08 with 0x624690: "%s\tutorial\%s.ini") into the table 0x6fa260 (128 entries of four ints: due day, due
   time, due clock tail, text id): per line (lines containing "//" are comments) the ':' separated fields are the text label (0x42e770; an unknown label: the message "he_OpenHelpEvents(): invalid textlabel:%s"), and the three
   numbers; an unopenable file gives the message box "he_OpenHelpEvents(): Could not open help-definition!" (ROUGH) */

void he_Unk567810(char *param)
{
    char sep[2] = ":";
    char path[260];
    char line[0x100], label[0x100], msg[0x400];
    int *e;
    FILE *f;
    char *t;
    int k, i;
    char c;

    sprintf(path, "%s\\tutorial\\%s.ini", "\\project\\game\\", param);
    for (e = (int *)(unsigned)&full_data_6fa260, i = 0; i < 0x200; i++)
        e[i] = 0;
    f = fopen(path, "rt");
    if (f == 0) {
        MessageBoxA(0, "he_OpenHelpEvents(): Could not open help-definition!", "Error", 0x30);
        return;
    }
    e = (int *)(unsigned)&full_data_6fa260;
    for (;;) {
        memset(line, 0, sizeof(line));
        fgets(line, 0x100, f);
        if (strstr(line, "//") == 0) {
            k = 0;
            for (t = strtok(line, sep); t != 0; t = strtok(0, sep)) {
                if (k == 0) {
                    i = 0;
                    do {
                        c = t[i];
                        label[i] = c;
                        i++;
                    } while (c != 0);
                    e[3] = txt_FindKey(label);
                    if (e[3] == -1)
                        sprintf(msg, "he_OpenHelpEvents(): invalid textlabel:%s", label);
                } else if (k == 1) {
                    e[0] = atoi(t);
                } else if (k == 2) {
                    e[1] = atoi(t);
                } else if (k == 3) {
                    e[2] = atoi(t);
                }
                k++;
            }
            e += 4;
            if ((int)e > ((unsigned)&he_BarView))  /* the end of the help table: the object after it */
                return;
        }
        if (feof(f)) {
            fclose(f);
            return;
        }
    }
}
/* 0x5679b0, a Ghidra miss: the allocation handler of the tutorial event 0x83: the hint script by the profession (0x504ad0 of the kind byte 0x770ce4 of the local player: 1 / 2 / 3 "help_events_hw", 5 / 6 "help_events_alpa", 7 "help_events_kirche", 4 "help_events_wirt", 0xb "help_events_dieb") is read
   (0x567810), the first run is the first table entry 0x6fa260 (ROUGH) */

void he_Unk5679b0(int ev)
{
    int k;

    k = gm_TitleGroup(*(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
    if (k == 1 || k == 2 || k == 3)
        he_Unk567810("help_events_hw");
    else if (k == 5 || k == 6)
        he_Unk567810("help_events_alpa");
    else if (k == 7)
        he_Unk567810("help_events_kirche");
    else if (k == 4)
        he_Unk567810("help_events_wirt");
    else if (k == 0xb)
        he_Unk567810("help_events_dieb");
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x52) = U_I((unsigned)&full_data_6fa260);
    *(short *)(ev + 0x56) = *(short *)(unsigned)&full_data_6fa264;
    *(int *)(ev + 0x58) = U_I((unsigned)&full_data_6fa268);
}
/* 0x567a80, the run handler of the tutorial hints (ROUGH): state 0 shows the hint of the entry +0xac of the table 0x6fa260 (the text 0x143a1c0 [id] through 0x53dda0 to the local player, when hints 0x6f3669 are on), steps
   to the next entry and sets the next run to its due clock; an empty entry ends (state 1); -1 / -2 free */

void he_Unk567a80(int ev)
{
    int st, v, i;

    st = *(int *)(ev + 0x70);
    if (-3 < st) {
        if (st < 0) {
            he_FreeEvent((int *)ev);
        } else if (st == 0) {
            v = *(int *)((unsigned)&full_data_6fa26c + *(int *)(ev + 0xac) * 16);
            if (v != 0 && v != -1 && *(char *)((unsigned)&ai_GamePanel + 0x1) != 0)
                he_ShowNews(*(int *)((unsigned)&ai_CharInts + U_I((unsigned)&ai_PlayerId) * 0x21c), -1, *(int *)((unsigned)&d2_Strings + v * 4), 0x40, 0x5f9, 0);
            i = *(int *)(ev + 0xac) + 1;
            *(int *)(ev + 0xac) = i;
            if (*(int *)((unsigned)&full_data_6fa26c + i * 16) == 0) {
                *(int *)(ev + 0x70) = 1;
                return;
            }
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x52) = *(int *)((unsigned)&full_data_6fa260 + i * 16);
            *(short *)(ev + 0x56) = *(short *)((unsigned)&full_data_6fa264 + i * 16);
            *(int *)(ev + 0x58) = *(int *)((unsigned)&full_data_6fa268 + i * 16);
        }
    }
}
/* 0x567b80, the allocation handler of the tips (type 0x84; a Ghidra miss): the first run is today, 4 days later when a tutorial event 0x83 exists; +0x56 = 8, +0x58 = 0; 0x505ca0 (0x1a, 0x6fa1e0) fills the tip order table */

void he_Unk567b80(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    if (he_FindEvent(1, 0, 0x83) != 0)
        *(int *)(ev + 0x52) = U_I((unsigned)&ai_Time) + 4;
    else
        *(int *)(ev + 0x52) = U_I((unsigned)&ai_Time);
    *(short *)(ev + 0x56) = 8;
    *(int *)(ev + 0x58) = 0;
    gm_Shuffled(0x1a, (unsigned)&full_data_6fa1e0);
}
/* 0x567c00, the run handler of the tips (ROUGH): state 0 shows the next of the 27 tip texts (0x1442430 [0x6fa1e0 [+0xac]]) while the tips 0x6f366b are on, the next run in random(4) + 8 units, else in 1 unit */

void he_Unk567c00(int ev)
{
    int st, i;
    unsigned r;

    st = *(int *)(ev + 0x70);
    if (-3 < st) {
        if (st < 0) {
            he_FreeEvent((int *)ev);
        } else if (st == 0) {
            if (*(char *)((unsigned)&ai_GamePanel + 0x3) != 0) {
                he_ShowNews(*(int *)((unsigned)&ai_CharInts + U_I((unsigned)&ai_PlayerId) * 0x21c), -1, *(int *)((unsigned)&full_data_1442430 + *(int *)((unsigned)&full_data_6fa1e0 + *(int *)(ev + 0xac) * 4) * 4), 0, 0x5f9, 0);
                i = *(int *)(ev + 0xac) + 1;
                *(int *)(ev + 0xac) = i;
                if (0x1a < i)
                    *(int *)(ev + 0x70) = 1;
                *(AITIME *)(ev + 0x52) = ai_Time;
                r = ((int (*)(int, int, int))util_Random)(4, 0, 0);
                gm_AddTime(ev + 0x52, (r & 0xffff) + 8);
                return;
            }
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
        }
    }
}
/* 0x567d20, a Ghidra miss: the allocation handler of the tutorial event 0x85: the first run is now + 7 units (+0x56 = 7, +0x58 = 0) */

void he_Unk567d20(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(short *)(ev + 0x56) = 7;
    *(int *)(ev + 0x58) = 0;
}
/* 0x567d60, a Ghidra miss: the run handler of the tutorial event 0x85 (night walk of the player's characters; only with the tutorial flag 0x6f2438, else -1): state 0 (sound on 0x6f25ac) sets the kind byte of
   the player record (+2) to 5; state 1 before 22:00 collects the characters of the player (0x501190 (1, 4, id) / 0x501030, at most 0x40, plus 0x6f1bc8) and shows the speech bubble 0x521960 (one of them, with 1 of 4
   the name 0x501b50 / 0x501ce0 + 2), next run +random(2) hours, random(0x3c) minutes; from 22:00 the next state; state 2 sets the kind byte to 6 and frees (ROUGH) */

void he_Unk567d60(int ev)
{
    int n, k, o, r, pick;
    int list[0x41];

    n = 0;
    if (U_I((unsigned)&cm_TutorialActive) == 0)
        *(int *)(ev + 0x70) = -1;
    switch (*(int *)(ev + 0x70)) {
    case 0:
        if (*(char *)(unsigned)&ai_GameState == 0)
            return;
        *(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) = 5;
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        if (*(unsigned short *)(unsigned)&ai_Time.hour >= 0x16) {
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        o = gm_BuildQuery(1, 4, *(unsigned short *)(ev + 8));
        while (o != 0 && n < 0x40) {
            list[n++] = o;
            o = gm_BuildNext();
        }
        list[n++] = U_I((unsigned)&ai_MarketBuilding);
        pick = list[(unsigned short)util_Random(n)];
        if ((unsigned short)util_Random(4) != 0 && pick != U_I((unsigned)&ai_MarketBuilding)) {
            r = gm_BuildingMainObject(pick);
            if (r == 0)
                r = gm_GetGebaeudeHauptraum(pick);
            if (r != 0)
                sv_SelectPanelObjectK(pick, *(int *)(r + 2), (unsigned)&ch_EmptyName);
            else
                sv_SelectPanelObjectK(pick, -1, (unsigned)&ch_EmptyName);
        } else {
            sv_SelectPanelObjectK(pick, -1, (unsigned)&ch_EmptyName);
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        *(unsigned short *)(ev + 0x56) += (unsigned short)util_Random(2);
        *(int *)(ev + 0x58) = (unsigned short)util_Random(0x3c);
        return;
    case 2:
        *(char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) = 6;
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        return;
    default:
        return;
    }
}
/* 0x567f00, a Ghidra miss: the allocation handler of the event 0xa6: the first run is now + 6 units, +0x84 = -1 */

void he_Unk567f00(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = -1;
    *(short *)(ev + 0x56) = 6;
    *(int *)(ev + 0x58) = 0;
}
/* 0x567f50, the per-character step of a patrol / escort event (ROUGH; ev, the character record c, its +0x24 the current wait counter): when the character is idle (+0x24 == 0) with probability by the stat +0x12 the
   news 0x10 is sent (the message type 0x10 with the clock, 0x4fe140 (&clock, 0, 0, 10), the stat 0x5087f0 / 0x4c47e0), else the next waypoint (0x4bda80 (+0x29, +0x2d), the object 0x502020) is walked to: the pause
   table {12, 8, 4, 2, 0} by +0x31 counts +0x32 up (0x524810, 0x5024f0, 0x4bdaa0 wraps the index at the route length), 0x4c3ba0 / 0x4c3d90 set the route index (+0x2d) and the result 0x4c49e0 goes to +0x84;
   a blocked step with the neighbour 0x50a0f0 sends the message type 0xf */

void he_PatrolStep(int ev, unsigned short *c)
{
    int o, w, iv, n, k, t;
    int pause[7];
    unsigned char m[0xf8];

    if (*(int *)(c + 0x12) != 0)
        return;
    o = gm_ObjectBuildingOwner(c);
    if (*(unsigned char *)(c + 9) < (unsigned)((unsigned short)util_Random(0x20) + 0x28)) {
        memset(m, 0, sizeof(m));
        *(int *)(m + 8) = *(int *)(o + 4);
        *(int *)(m + 0x2c) = U_I((unsigned)&ai_Time.hour);
        *(int *)(m + 0x28) = U_I((unsigned)&ai_Time);
        m[4] = 0x10;
        m[0x36] = 1;
        *(int *)(m + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
        *(short *)(m + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
        *(int *)(m + 0xc) = *(int *)(m + 8);
        gm_AddTime(m + 0x28, 0, 0, 10);
        *(int *)(m + 0x58) = *(int *)(c + 1);
        *(int *)(m + 0x5c) = 100 - *(unsigned char *)(c + 9);
        *(int *)(m + 0x64) = 5;
        *(int *)(m + 0x84) = 0;
        gm_ItemPrice(*c, 100);
        t = 100 - *(unsigned char *)(c + 9);
        cm_SendMoveGoods(-1, *(int *)(o + 4), U_I((unsigned)&ai_OwnPlayer), t);
        cm_SendLongMessage(m);
        return;
    }
    w = rt_PointAt(*(int *)((char *)c + 0x29), *(int *)((char *)c + 0x2d));
    if (w != 0 && (o = gm_FindBuildingById(*(int *)(w + 4))) != 0) {
        if (*(int *)((char *)c + 0x2d) == 0) {
            pause[1] = 0xc;
            pause[2] = 8;
            pause[3] = 4;
            pause[4] = 2;
            pause[5] = 0;
            k = pause[*(unsigned char *)((char *)c + 0x31) + 1];
            *(unsigned char *)(c + 0x19) = *(unsigned char *)(c + 0x19) + 1;
            if (*(unsigned char *)(c + 0x19) < k)
                return;
            *(unsigned char *)(c + 0x19) = 0;
        }
        if (sv_IsCartAtDoor(c, w) != 0 && (gm_IsType10(w) == 0 || *(int *)(c + 3) == he_PatrolBuilding->id)) {
            iv = rt_PointCount(*(int *)((char *)c + 0x29));
            n = *(int *)((char *)c + 0x2d) + 1;
            if (iv <= n)
                n = 0;
            cm_BeginFieldBlock(c, *(int *)(c + 1));
            cm_AddFieldValue(4, 1, (int)c + (0x2d - U_I((unsigned)&cm_FieldObject)), &n);
            *(int *)(ev + 0x84) = ((int (*)())cm_SendFieldValues)();
            return;
        }
        k = gm_ObjectBuilding(c);
        if (k != 0) {
            memset(m, 0, sizeof(m));
            *(int *)(m + 8) = *(int *)(o + 4);
            *(int *)(m + 0x28) = U_I((unsigned)&ai_Time);
            *(int *)(m + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
            *(int *)(m + 0x58) = *(int *)(c + 1);
            *(int *)(m + 0x2c) = U_I((unsigned)&ai_Time.hour);
            m[4] = 0xf;
            *(int *)(m + 0xc) = -1;
            m[0x36] = 2;
            *(short *)(m + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
            *(int *)(m + 0x5c) = *(int *)(k + 1);
            *(int *)(m + 0x60) = *(int *)(w + 1);
            m[0x9e] = 1;
            cm_SendLongMessage(m);
        }
    }
}
/* 0x568220, the run handler of the guard patrol event 0xa6 (only while the game state byte ai_GameState is set; waits for
   the sound handle exSlot): state 0 looks up the patrol building (kind 0x44) and calls he_PatrolStep for every guard group
   cart (gm_Query (0, 2, 7, 4, 0x1d)) that has a route, is not flagged and has a pause counter; it runs again 15 minutes
   later; the negative states free the record */

void he_RunPatrolEvent(int ev)
{
    AIJOB *e = (AIJOB *)ev;
    AIOBJ *g;
    int st;

    if (ai_GameState == 0)
        return;
    if (e->exSlot != -1 && cm_GetSlotState(e->exSlot) == 0)
        return;
    st = e->stage;
    e->exSlot = -1;
    if (st < -2)
        return;
    if (st >= 0) {
        if (st != 0)
            return;
        he_PatrolBuilding = (AIBLD *)gm_BuildQuery(1, 0, 0x44);
        for (g = (AIOBJ *)gm_Query(0, 2, 7, 4, 0x1d); g != 0; g = (AIOBJ *)gm_QueryNext()) {
            if (g->route != 0 && (g->flags20 & 1) == 0 && (g->pad13 & 0x20) == 0 && g->pad2e[5] != 0)
                he_PatrolStep(ev, (unsigned short *)g);
        }
        e->end = ai_Time;
        gm_AddTime(&e->end, 0, 0xf, 0);
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x568310, registers the event types 0x02 .. 0xa6 (second part, after 0x553060): he_SetEventHandlers (type, alloc, run) for each (a non-zero result = out of table space: return 1), the type texts 0x52b350 (type, name, 0, menu id, icon) for the types with an entry in the event list, and the tip table 0x505ca0 (0x1a, 0x6fa1e0); returns 0 */
int he_RegisterEventTypes02(void)
{
    if (he_SetEventHandlers(2, (void *)he_Unk553e10, (void *)he_Unk553e90) != 0)
        return 1;
    if (he_SetEventHandlers(6, (void *)he_Unk554820, (void *)he_Unk554320) != 0)
        return 1;
    if (he_SetEventHandlers(7, (void *)he_Unk554820, (void *)he_Unk554860) != 0)
        return 1;
    if (he_SetEventHandlers(15, (void *)he_Unk554cb0, (void *)he_Unk556600) != 0)
        return 1;
    if (he_SetEventHandlers(16, (void *)he_Unk556dc0, (void *)he_Unk556ed0) != 0)
        return 1;
    if (he_SetEventHandlers(17, (void *)he_Unk557160, (void *)he_Unk557190) != 0)
        return 1;
    if (he_SetEventHandlers(18, (void *)he_Unk557b30, (void *)he_Unk5578a0) != 0)
        return 1;
    if (he_SetEventHandlers(19, (void *)he_Unk557b30, (void *)he_Unk557b70) != 0)
        return 1;
    if (he_SetEventHandlers(20, (void *)he_Unk557d70, (void *)he_Unk557e20) != 0)
        return 1;
    sv_RegisterActivity(20, 0, 1, 1280, 174);
    if (he_SetEventHandlers(21, (void *)he_Unk5584d0, (void *)he_Unk558550) != 0)
        return 1;
    sv_RegisterActivity(21, "Getier", 1, 1280, 174);
    if (he_SetEventHandlers(22, (void *)he_Unk5588c0, (void *)he_Unk558a40) != 0)
        return 1;
    sv_RegisterActivity(22, "Trainieren", 0, 1289, -1);
    if (he_SetEventHandlers(23, (void *)he_Unk558de0, (void *)he_RunEvent17) != 0)
        return 1;
    if (he_SetEventHandlers(25, (void *)he_Unk558e70, (void *)he_Unk558f10) != 0)
        return 1;
    if (he_SetEventHandlers(24, (void *)he_Unk559d20, (void *)he_Unk55a030) != 0)
        return 1;
    if (he_SetEventHandlers(26, (void *)he_Unk55a5f0, (void *)he_Unk55a730) != 0)
        return 1;
    if (he_SetEventHandlers(27, (void *)he_Unk55af70, (void *)he_Unk55b140) != 0)
        return 1;
    if (he_SetEventHandlers(28, (void *)he_Unk55b3e0, (void *)he_Unk55b530) != 0)
        return 1;
    if (he_SetEventHandlers(30, (void *)he_Unk55b720, (void *)he_Unk55b830) != 0)
        return 1;
    if (he_SetEventHandlers(32, (void *)he_Unk55bd20, (void *)he_Unk55bd80) != 0)
        return 1;
    if (he_SetEventHandlers(33, (void *)he_Unk55bf50, (void *)he_Unk55bfd0) != 0)
        return 1;
    if (he_SetEventHandlers(31, (void *)he_AllocEvent1f, (void *)he_Unk55bad0) != 0)
        return 1;
    if (he_SetEventHandlers(35, (void *)he_Unk55c540, (void *)he_Unk55c700) != 0)
        return 1;
    if (he_SetEventHandlers(36, (void *)he_Unk55e3a0, (void *)he_Unk55cb00) != 0)
        return 1;
    if (he_SetEventHandlers(37, (void *)he_RunDueNow, (void *)he_Unk55cc10) != 0)
        return 1;
    if (he_SetEventHandlers(38, (void *)he_Unk55ce00, (void *)he_Unk55ce60) != 0)
        return 1;
    if (he_SetEventHandlers(39, (void *)he_AllocEvent27, (void *)he_Unk55d390) != 0)
        return 1;
    if (he_SetEventHandlers(40, (void *)he_Unk55d490, (void *)he_Unk55d5c0) != 0)
        return 1;
    sv_RegisterActivity(40, 0, 1, 1279, 188);
    if (he_SetEventHandlers(49, (void *)he_Unk563980, (void *)he_Unk55dbe0) != 0)
        return 1;
    if (he_SetEventHandlers(50, (void *)he_AllocEvent32, (void *)he_Unk55de30) != 0)
        return 1;
    if (he_SetEventHandlers(51, (void *)he_Unk55e180, (void *)he_Unk55e1d0) != 0)
        return 1;
    if (he_SetEventHandlers(52, (void *)he_Unk55e3a0, (void *)he_Unk55e3d0) != 0)
        return 1;
    if (he_SetEventHandlers(56, (void *)he_Unk55e5c0, (void *)he_Unk55e600) != 0)
        return 1;
    if (he_SetEventHandlers(57, (void *)he_AllocEvent39_3e, (void *)he_Unk55e730) != 0)
        return 1;
    if (he_SetEventHandlers(58, (void *)he_Unk55e9d0, (void *)he_Unk55efc0) != 0)
        return 1;
    if (he_SetEventHandlers(59, (void *)he_Unk55fa20, (void *)he_Unk55fa90) != 0)
        return 1;
    if (he_SetEventHandlers(60, (void *)he_Unk560820, (void *)he_Unk560900) != 0)
        return 1;
    sv_RegisterActivity(60, 0, 0, 1284, -1);
    if (he_SetEventHandlers(61, (void *)he_Unk561130, (void *)he_Unk561190) != 0)
        return 1;
    if (he_SetEventHandlers(62, (void *)he_AllocEvent39_3e, (void *)he_Unk5614c0) != 0)
        return 1;
    if (he_SetEventHandlers(63, (void *)he_Unk5619a0, (void *)he_Unk561a20) != 0)
        return 1;
    sv_RegisterActivity(63, 0, 0, 1290, -1);
    if (he_SetEventHandlers(64, (void *)he_Unk563980, (void *)he_Unk5628e0) != 0)
        return 1;
    sv_RegisterActivity(64, 0, 0, 1283, -1);
    if (he_SetEventHandlers(72, (void *)he_Unk563070, (void *)he_Unk5630f0) != 0)
        return 1;
    sv_RegisterActivity(72, 0, 0, 1282, -1);
    if (he_SetEventHandlers(73, (void *)he_Unk563980, (void *)he_Unk5639c0) != 0)
        return 1;
    sv_RegisterActivity(73, 0, 0, 1290, -1);
    if (he_SetEventHandlers(78, (void *)he_Unk5642e0, (void *)he_Unk5644e0) != 0)
        return 1;
    if (he_SetEventHandlers(79, (void *)he_Unk564860, (void *)he_Unk5649b0) != 0)
        return 1;
    if (he_SetEventHandlers(80, (void *)he_Unk5652b0, (void *)he_RunStormEvent) != 0)
        return 1;
    if (he_SetEventHandlers(108, (void *)he_Unk565500, (void *)he_Unk565620) != 0)
        return 1;
    sv_RegisterActivity(108, "Heilen", 0, 1285, -1);
    if (he_SetEventHandlers(110, (void *)he_Unk565850, (void *)he_Unk565940) != 0)
        return 1;
    if (he_SetEventHandlers(113, (void *)he_Unk565b00, (void *)he_Unk565b60) != 0)
        return 1;
    if (he_SetEventHandlers(114, (void *)he_Unk565d10, (void *)he_Unk565db0) != 0)
        return 1;
    if (he_SetEventHandlers(115, (void *)he_Unk5662d0, (void *)he_Unk566370) != 0)
        return 1;
    if (he_SetEventHandlers(117, (void *)he_Unk563980, (void *)he_Unk566830) != 0)
        return 1;
    sv_RegisterActivity(117, 0, 0, 1282, -1);
    if (he_SetEventHandlers(124, (void *)he_Unk567620, (void *)he_Unk567660) != 0)
        return 1;
    if (he_SetEventHandlers(131, (void *)he_Unk5679b0, (void *)he_Unk567a80) != 0)
        return 1;
    if (he_SetEventHandlers(132, (void *)he_Unk567b80, (void *)he_Unk567c00) != 0)
        return 1;
    if (he_SetEventHandlers(133, (void *)he_Unk567d20, (void *)he_Unk567d60) != 0)
        return 1;
    if (he_SetEventHandlers(166, (void *)he_Unk567f00, (void *)he_RunPatrolEvent) != 0)
        return 1;
    gm_Shuffled(0x1a, (unsigned)&full_data_6fa1e0);
    return 0;
}
/* 0x568aa0, calls the handler 0x541200 for every event of the type 0x23 with the flags +0x78 & 3 whose +0xb0 equals the day +4 of this event */

void he_EndKind23Events(int ev)
{
    int o;

    for (o = (int)he_FindEvent(1, 0, 0x23); o != 0; o = (int)he_NextEvent()) {
        if ((*(unsigned char *)(o + 0x78) & 3) != 0 && *(int *)(o + 0xb0) == *(int *)(ev + 4))
            he_EndEventNow(o);
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
/* 0x568be0, the run handler of a building event with a running income (a Ghidra miss; ROUGH skeleton, 1248 bytes, jump table 0x5690a4): the building 0x4fea50 (+0xb8); the owner record ebp (+0x27, not the local player); every player record
   whose +0x17c is this event is released (0x524260 (0, 0, id, 0)) when it ends: -1 / -2 / no building free; state 0 goes to 1; state 1 (while the counters +0x14 / +0x18 are non-zero) each minute step (0x509400 (+0x60, now, 0)) adds to the
   byte +0xc8, at 60 it re-rates the eight slots (+0x8c: 0x4c4b00 (owner +4, id, 2 - level, 0), the rate +0xc4 = (0x50c980 (id, 8, +0x8c) - 0.5) * 0.25) and while the due clock (+0x52, 0x4fe210) is past pays the amount from the stock
   +0xb0 to the party (0x4c4880 (+0xc0, +0xbc, id, min (+0x36 of the record, +0xe of the group), money 0x6f1bc5, 0), 0x4ffe90 (+0x14 of the building, 0, 1, id) group); an exhausted stock ends with the news 0x1bb7 (0x628a28) and
   frees; state 2 releases the players and frees when both counters are zero */

void he_Unk568be0(int ev)
{
    int buf[0x20], b2[0x20];
    int obj, ebp, rec, i, v, g, n, bld;
    float f1, f2;
    char text[0x100];

    ebp = 0;
    gm_FindById(buf, 0, 0, *(int *)(ev + 0xb8));
    obj = buf[4];
    if (obj == 0) {
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(0, 0, *(unsigned short *)rec, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(unsigned short *)(obj + 0x27) != 0xffff) {
        rec = (unsigned)&ai_Chars + *(unsigned short *)(obj + 0x27) * 0x21c;
        if (rec != U_I((unsigned)&ai_PlayerChar))
            ebp = rec;
    }
    switch (*(int *)(ev + 0x70)) {
    case 0:
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (*(int *)(ev + 0x14) == 0 && *(int *)(ev + 0x18) == 0) {
            *(int *)(ev + 0x70) = 2;
            return;
        }
        gm_FindById(b2, 0, 0, *(int *)(ev + 0xb8));
        v = gm_BuildingSkill(b2[4], 1, 1);
        f1 = (float)((double)v * 0.0 + 1.0);
        if (*(unsigned char *)(ev + 0xc8) >= 0x3c) {
            if (ebp != 0) {
                for (i = 0; i < 8; i++) {
                    if (*(int *)(ev + 0x8c + i * 4) != -1 && *(int *)(b2[4] + 0x41) != 2)
                        cm_SendRelations(*(int *)(ebp + 4), *(int *)(ev + 0x8c + i * 4), 2 - *(int *)(b2[4] + 0x41), 0);
                }
                *(float *)(ev + 0xc4) = (float)((gm_MeanLiking(*(unsigned short *)ebp, 8, ev + 0x8c) - 0.5) * 0.25);
            }
            *(char *)(ev + 0xc8) = 0;
        }
        n = gm_WorkMinutesBetween(ev + 0x60, (unsigned)&ai_Time, 0);
        if (n > 0)
            *(unsigned char *)(ev + 0xc8) += n;
        f2 = f1 + *(float *)(ev + 0xc4);
        while (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) < 0) {
            n = gm_WorkMinutesBetween(ev + 0x60, ev + 0x52, 0);
            v = (int)((float)n * 0.0f * f2);
            *(int *)(ev + 0xb0) = (int)((float)*(int *)(ev + 0xb0) - (float)v);
            if (*(int *)(ev + 0xb0) <= 0) {
                gm_FindById(b2, 0, 0, *(int *)(ev + 0xbc));
                g = gm_Query(*(int *)(b2[4] + 0x14), 0, 1, *(unsigned short *)(ev + 0xac));
                gm_FindById(b2, 0, 0, *(int *)(ev + 0xc0));
                if (g != 0) {
                    n = *(unsigned short *)(U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41 + 0x36);
                    if (n >= *(int *)(g + 0xe))
                        n = *(int *)(g + 0xe);
                    cm_SendExSellObject(*(int *)(ev + 0xc0), *(int *)(ev + 0xbc), n, *(unsigned short *)(ev + 0xac), U_I((unsigned)&ai_OwnPlayer), 0);
                    *(int *)(ev + 0xb0) += *(int *)(U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41 + 0x22);
                    *(float *)(ev + 0xb4) = (float)*(int *)(ev + 0xb0);
                }
                if (g == 0) {
                    txt_Format(text, 0x1bb7, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b2[4]);
                    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5f1, *(int *)(b2[4] + 1), -1, 0, (unsigned)&full_data_628a28);
                    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
                        if (*(int *)(rec + 0x17c) == ev)
                            sv_MoveToGroup(b2[4], *(unsigned short *)rec, 0, 0);
                    }
                    he_FreeEvent((int *)ev);
                    return;
                }
            }
            *(AITIME *)(ev + 0x60) = *(AITIME *)(ebp != 0 ? ebp : ev + 0x52);
            gm_AddTime(ev + 0x52, 0, 0, 5);
        }
        return;
    case 2:
        if (*(int *)(ev + 0x14) > 0 || *(int *)(ev + 0x18) > 0) {
            *(int *)(ev + 0x70) = 1;
            return;
        }
        if (obj == 0) {
            *(int *)(ev + 0x70) = -1;
            return;
        }
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(obj, *(unsigned short *)rec, 0, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    case -2:
    case -1:
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(0, 0, *(unsigned short *)rec, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    default:
        return;
    }
}
/* 0x5690c0, "he_AllocProduktion()": prepares the production event record r (the building +0xbc, the good +0xac): both clocks (+0x52 / +0x60) = the start clock (+0x44), the amount (+0xb4 / +0xb8) = the 0x22 field of the good's record in 0x91aaf0 (0x41 bytes each), due 1 unit later, state +0xc4 = -1, +0xc8 = 0, every character record that holds this event (+0x17c) is handed to 0x524260 with the building, +0xd4 = 1.0 and +0xcc = (the 0x50c980 value of the owner (+8) - 0.5) * 0.25. A building that is not found with the owner unset (+8 = 0xffff) prints the debug text. ROUGH */

void he_Unk5690c0(int r)
{
    char buf[0x200];
    int b, o, rec, id;
    char *t;
    float v;

    if (*(unsigned short *)(r + 8) == 0xffff) {
        o = gm_FindBuildingById(*(int *)(r + 0xbc));
        if (o != 0)
            txt_Format(buf, "he_AllocProduktion(): geb:%G", o);
    }
    b = gm_FindBuildingById(*(int *)(r + 0xbc));
    t = (char *)U_I((unsigned)&ai_Goods);
    id = *(unsigned short *)(r + 0xac);
    v = (float)*(unsigned int *)(t + (id << 6) + id + 0x22);
    *(AITIME *)(r + 0x52) = *(AITIME *)(r + 0x44);
    *(AITIME *)(r + 0x60) = *(AITIME *)(r + 0x44);
    *(float *)(r + 0xb4) = v;
    *(float *)(r + 0xb8) = v;
    gm_AddTime(r + 0x52, 0, 0, 1);
    *(int *)(r + 0xc4) = -1;
    *(int *)(r + 0xc8) = 0;
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == r)
            sv_MoveToGroup(b, *(unsigned short *)rec, 0, r);
    }
    *(float *)(r + 0xcc) = (gm_MeanLiking(*(unsigned short *)(r + 8), 8, r + 0x8c) - 0.5) * 0.25;
    *(int *)(r + 0xd4) = 0x3f800000;
}

float ahm_PlaceFactor();
void ai_Unk4c5840(int a);
void ai_Unk4c48e0(int a, short b, int c);
void sv_CountGroupMembers(char *g);
int ai_Unk5740f0();
int ai_Unk5ba700();
int cm_SendRelations(int a, int b, int c, int d);
int gm_FreeCapacity();
int gm_ListAmount();
int gm_BuildingMainObject(unsigned char *b);
int gm_WorkMinutesBetween();
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);
int ai_InventoryHas(void *ch, int id);

/* unlinked, the table of the raw material shortage news (0x628a5c .. 0x628bdc) by the type id of the building (0x569d17 / 0x5694d8) */
static __inline char *he_ProdShortage(int type)
{
    switch (type) {
    case 7: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_01";
    case 8: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_03";
    case 0xb: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_05";
    case 0xc: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_07";
    case 0xd: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_09";
    case 0xe: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_11";
    case 0x12: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_13";
    case 0x14: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_15";
    case 0x15: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_17";
    case 0x16: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_19";
    case 0x20:
    case 0x1f:
    case 0x1d: return "_NACHRICHTEN_ROHSTOFFMANGEL_HS_20";
    }
    return 0;
}

/* 0x569210, "he_RunProduktion()", the run handler of the production event record ev (+0xbc the building, +0xc0 the
   building of the goods, +0xac the good, +0x14 / +0x18 the pending orders, +0x70 the state -2 .. 2 by the jump table
   0x56a0e4, +0xb4 / +0xb8 the amount left, +0xcc / +0xd4 the factors, +0xd0 the rounds, +0x52 / +0x60 the clocks, +0xc4 the
   handle of the transport, +0xc8 the missing-goods flag): state 0 -> 1; state 1 with orders: the goods are fetched from the
   goods building (0x4ffe90 / 0x50ab80 / 0x50aaf0), the amount is used up by the rounds (the rate is the 0x5092e0 value, the
   level of the owner (+0x81), the workers +0x8c (8 ids, their 0x4fc8e0 skill) ..); a missing good gives the news
   0x1bb2 .. 0x1bb4 (_NACHRICHTEN_ROHSTOFFMANGEL_HS_xx by the type) to the owner (0x53dee0); state 2 without orders frees the
   record (hands the characters on with 0x524260). ROUGH, from the disassembly: the control flow is kept by labels */
void he_Unk569210(int ev)
{
    char text[0x400];
    char name[0x24];
    char crlf[2];
    int b, b2, g, t, w, o, chk, ck, tp, found, rec, news, i, n, h, owner;
    float f, rate, fv, sum, v;
    int bo[2];

    name[0] = 0;
    if (*(unsigned short *)(ev + 8) == 0xffff) {
        b = gm_FindBuildingById(*(int *)(ev + 0xbc));
        if (b == 0)
            goto done_free;
        txt_Format(text, "he_RunProduktion(): geb:%G", b);
        he_FreeEvent((int *)ev);
        return;
    }
    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        goto state_free;
    case 0:
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (*(int *)(ev + 0x14) > 0 || *(int *)(ev + 0x18) > 0)
            goto work;
        *(int *)(ev + 0x70) = 2;
        return;
    case 2:
        goto state2;
    default:
        return;
    }
work:
    w = *(int *)(ev + 0xc4);
    if (w == -1)
        goto fetch;
    if (cm_GetSlotState(w) == 0)
        return;
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xbc));
    b = bo[0];
    if (b == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    tp = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
    if (cm_GetSlotState(*(int *)(ev + 0xc4)) != 2)
        goto fetch;
    chk = gm_FindCharacterByMaster(b);
    found = 0;
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == ev)
            found = rec;
    }
    if (found == 0)
        goto arrived;
    if (chk != 0 && (*(unsigned char *)(chk + 0x1b4) & 1))
        goto arrived;
    if (*(int *)(ev + 0xc8) != 0)
        goto arrived;
    gm_FindById(0, bo, 0, *(int *)(ev + 0xc0));
    b2 = bo[0];
    t = gm_BuildingMainObject(b2);
    g = gm_Query(*(int *)(b2 + 0x14), 1, 0, *(unsigned short *)(ev + 0xac));
    if (g != 0 && gm_FreeCapacity(b2, *(unsigned short *)(ev + 0xac), 1) == 0) {
        txt_Format(text, 0x1bb3, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
        strcpy(name, "_NACHRICHTEN_HS_04");
        goto news;
    }
    if (gm_FreeCapacity(b2, *(unsigned short *)(ev + 0xac), 1) == 0) {
        txt_Format(text, 0x1bb4, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
        strcpy(name, "_NACHRICHTEN_HS_05");
        goto news;
    }
    if (*(char *)(((unsigned)&cm_SendStates + 0x1) + *(int *)(ev + 0xc4) * 10) != 8)
        goto arrived;
    txt_Format(text, 0x1bb2, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
    if (*(char *)tp == 0x1e)
        strcpy(name, "_NACHRICHTEN_DOMIZIL_02");
    else if (he_ProdShortage(*(unsigned char *)tp) != 0)
        strcpy(name, he_ProdShortage(*(unsigned char *)tp));
news:
    owner = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
    if (*(char *)(owner + 2) == 6 || *(char *)(owner + 2) == 7) {
        news = 0x5f1;
        chk = gm_FindCharacterByMaster(b);
        if (chk != 0 && (*(unsigned char *)(chk + 0x1b4) & 1))
            news = 0x60d;
        if (name[0] != 0)
            he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(ev + 4), text, 0, news, *(int *)(b + 1), *(int *)(t + 2), 0, name);
        else
            he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(ev + 4), text, 0, news, *(int *)(b + 1), *(int *)(t + 2), 0, 0);
    }
    if (b != 0 && *(int *)(b + 0x95) != -1)
        ai_Unk4c5840(*(int *)(b + 1));
    *(int *)(ev + 0xc8) = 1;
arrived:
    *(int *)(ev + 0xc4) = -1;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    return;
fetch:
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xbc));
    b = bo[0];
    if (b == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    if (*(int *)(b + 0x95) == -1)
        ai_Unk4c5840(*(int *)(b + 1));
    tp = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
    n = gm_BuildingSkill(b, 1, 1);
    f = (float)n * 0.01 + 1.0;
    chk = gm_FindCharacterByMaster(b);
    owner = chk != 0 ? chk : (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
    if (*(char *)(owner + 2) == 6 || *(char *)(owner + 2) == 7)
        f = (float)*(unsigned char *)(owner + 0x81) * 0.000992063492063492 + f;
    else
        f = 0.0f + f;
    if ((*(unsigned char *)(b + 0x5a) & 0x20) && ai_Unk5ba700(b, 0x178))
        f = f * 0.5;
    if (*(unsigned char *)(ev + 0xd0) >= 0x3c) {
        n = 0;
        sum = 0.0f;
        for (i = 0; i < 8; i++) {
            w = *(int *)(ev + 0x8c + i * 4);
            if (w == -1)
                continue;
            if (*(int *)(b + 0x41) != 2)
                cm_SendRelations(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(b + 0x27) * 0x21c), w, 2 - *(int *)(b + 0x41), 0);
            o = gm_FindChar(w);
            if (o == 0)
                continue;
            fv = gm_HealthRatio(o);
            if (*(unsigned char *)(o + 0x165) == 0x1b || *(unsigned char *)(o + 0x165) == 0x1c) {
                fv = (gm_GetSkill(o, 1) + 0.5) * fv;
                if (*(unsigned char *)(o + 0x1b4) & 1)
                    fv = 0.01f;
            }
            if (*(unsigned char *)(o + 0x1cc) & 4)
                fv = ahm_PlaceFactor(o, *(unsigned short *)(ev + 0xac)) * fv;
            if (ai_InventoryHas((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c, 0x2ba))
                fv = fv * 1.25f;
            sum = fv + sum;
            n++;
        }
        if (n != 0) {
            *(float *)(ev + 0xd4) = sum / n;
            if (ai_Unk5740f0((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c))
                *(float *)(ev + 0xd4) = *(float *)(ev + 0xd4) * 0.9f;
        }
        sv_CountGroupMembers(ev);
        *(float *)(ev + 0xcc) = (gm_MeanLiking(*(unsigned short *)(b + 0x27), 8, ev + 0x8c) - 0.5) * 0.25;
        *(unsigned char *)(ev + 0xd0) = 0;
    }
    ck = ev + 0x60;
    n = gm_WorkMinutesBetween(ck, (unsigned)&ai_Time, 0);
    if (n > 0)
        *(unsigned char *)(ev + 0xd0) += n;
    v = f;
    if (*(unsigned char *)(b + 0x5b) & 0x10)
        v = v + 0.1f;
    rate = (v + (float)(*(int *)(b + 0x41) - 2) * 0.1 + *(float *)(ev + 0xcc)) * *(float *)(ev + 0xd4);
    gm_FindById(0, bo, 0, *(int *)(ev + 0xc0));
    b2 = bo[0];
    if (b2 == 0)
        goto done_free;
    if (*(unsigned short *)b2 != 0x2a && *(unsigned short *)b2 != 0x116)
        goto done_free;
    g = gm_Query(*(int *)(b2 + 0x14), 1, 0, *(unsigned short *)(ev + 0xac));
    if (g != 0 && gm_FreeCapacity(b2, *(unsigned short *)(ev + 0xac), 1) == 0) {
        txt_Format(text, 0x1bb3, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
        strcpy(name, "_NACHRICHTEN_HS_04");
        t = 0;
        goto lacking;
    }
    if (gm_FreeCapacity(b2, *(unsigned short *)(ev + 0xac), 1) == 0) {
        txt_Format(text, 0x1bb4, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
        strcpy(name, "_NACHRICHTEN_HS_05");
        t = 0;
        goto lacking;
    }
    {
        char *rec91 = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41;
        unsigned short *gp = (unsigned short *)(rec91 + 0x2e);
        t = 0;
        for (i = 0; i < 4; i++, gp++) {
            if (*gp == 0)
                continue;
            o = gm_Query(*(int *)(b2 + 0x14), 1, 0, *gp);
            if (o == 0 || gm_ListAmount(b2, o) < *(unsigned short *)((char *)gp - 8)) {
                txt_Format(text, 0x1bb2, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b);
                tp = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
                if (he_ProdShortage(*(unsigned char *)tp) != 0)
                    strcpy(name, he_ProdShortage(*(unsigned char *)tp));
                goto lacking;
            }
        }
        if (*(int *)(ev + 0xc8) != 0) {
            h = (int)he_FindEvent(2, 0, 0x11, 3, *(int *)(ev + 4));
            if (h != 0)
                he_FreeEvent((int *)h);
            h = *(int *)(ev + 0x88);
            *(int *)(ev + 0xc8) = 0;
            if (h != 0) {
                he_FreeGfxInfo((int *)h);
                *(int *)(ev + 0x88) = 0;
            }
            *(AITIME *)ck = *(AITIME *)(ev + 0x52);
        }
        for (;;) {
            if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
                return;
            n = gm_WorkMinutesBetween(ck, ev + 0x52, 0);
            *(float *)(ev + 0xb4) = *(float *)(ev + 0xb4) - (float)n * ((float)*(int *)(ev + 0x14) * 1.3f + *(int *)(ev + 0x18)) * rate;
            i = 0;
            while (!(*(float *)(ev + 0xb4) > 0.0f)) {
                *(float *)(ev + 0xb4) = (float)*(unsigned int *)(rec91 + 0x22) + *(float *)(ev + 0xb4);
                *(float *)(ev + 0xb8) = *(float *)(ev + 0xb4);
                i++;
            }
            if (i != 0) {
                *(int *)(ev + 0xc4) = ((int (*)(int, short, int))ai_Unk4c48e0)(*(int *)(ev + 0xc0), *(unsigned short *)(ev + 0xac), i);
            } else if (*(int *)(ev + 0xc4) == -1) {
                if (gm_Query(*(int *)(b2 + 0x14), 1, 0, *(unsigned short *)(ev + 0xac)) == 0)
                    *(int *)(ev + 0xc4) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(b2 + 2), -1, *(unsigned short *)(ev + 0xac), 0, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            }
            *(AITIME *)ck = *(AITIME *)(ev + 0x52);
            gm_AddTime(ev + 0x52, 0, 1, 0);
            if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
                return;
        }
    }
lacking:
    t = gm_BuildingMainObject(b);
    if (*(int *)(ev + 0xc8) == 0) {
        owner = (unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c;
        if (*(char *)(owner + 2) == 6 || *(char *)(owner + 2) == 7) {
            news = 0x5f1;
            chk = gm_FindCharacterByMaster(b);
            if (chk != 0 && (*(unsigned char *)(chk + 0x1b4) & 1))
                news = 0x60d;
            if (t != 0)
                he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(ev + 4), text, 0, news, *(int *)(b + 1), *(int *)(t + 2), 0, name);
            else
                he_MessageBoxQuickjump(*(int *)(owner + 4), *(int *)(ev + 4), text, 0, news, *(int *)(b + 1), -1, 0, name);
        }
    }
    if (b != 0 && *(int *)(ev + 0xc8) == 0 && *(int *)(b + 0x95) != -1)
        ai_Unk4c5840(*(int *)(b + 1));
    *(int *)(ev + 0xc8) = 1;
    *(int *)(ev + 0xc4) = -1;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    *(AITIME *)(ev + 0x60) = *(AITIME *)(ev + 0x52);
    return;
state2:
    if (*(int *)(ev + 0x14) > 0 || *(int *)(ev + 0x18) > 0) {
        *(int *)(ev + 0x70) = 1;
        return;
    }
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xbc));
    b = bo[0];
    if (b == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == ev)
            sv_MoveToGroup(b, *(unsigned short *)rec, 0, 0);
    }
    if (b == 0)
        goto done_free;
    ai_Unk4c5840(*(int *)(b + 1));
    he_FreeEvent((int *)ev);
    return;
state_free:
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xbc));
    if (bo[0] != 0)
        ai_Unk4c5840(*(int *)(bo[0] + 1));
    for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
        if (*(int *)(rec + 0x17c) == ev)
            sv_MoveToGroup(0, *(unsigned short *)rec, 0, 0);
    }
done_free:
    he_FreeEvent((int *)ev);
}
/* 0x56a100, the alloc of an event record: both clocks (+0x52 / +0x60) = the start clock (+0x44), due a random 0 .. 59 units of the kind 4 later */

void he_Unk56a100(int r)
{
    *(AITIME *)(r + 0x52) = *(AITIME *)(r + 0x44);
    *(AITIME *)(r + 0x60) = *(AITIME *)(r + 0x44);
    gm_AddTime(r + 0x52, 4, (unsigned short)util_Random(0x3c), 0);
}
/* 0x56a170, a run handler (state 0 -> 1; state 1 with orders (+0x14 / +0x18): when due, the goods building +0xb0 gets an order of the good
   +0xac (the 0x4c4880 order, the amount = (a random number below 0x28 times the 0x36 field of the good + 0x28 times it) * the orders), the owner
   is told (news 0x1bb8, "_NACHRICHTEN_HS_08") and the record is freed; states -2 / -1 hand the characters on and free it) (ROUGH) */

void he_Unk56a170(int ev)
{
    char text[0x200];
    int b, rec, amount, bo[2];
    char *r91;
    unsigned short x;

    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(0, *(unsigned short *)rec, 0, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    case 0:
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (*(int *)(ev + 0x14) == 0 && *(int *)(ev + 0x18) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        gm_FindById(bo, 0, 0, *(int *)(ev + 0xb0));
        b = bo[0];
        if (b == 0) {
            *(int *)(ev + 0x70) = -1;
            return;
        }
        r91 = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)(ev + 0xac) * 0x41;
        x = *(unsigned short *)(r91 + 0x36) * 0x28;
        amount = ((unsigned short)util_Random(x) + *(unsigned short *)(r91 + 0x36) * 0x28) * (*(int *)(ev + 0x14) + *(int *)(ev + 0x18));
        cm_SendExSellObject(*(int *)(ev + 0xb4), -1, *(unsigned short *)(ev + 0xac), amount, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        txt_Format(text, 0x1bb8, *(unsigned short *)(ev + 0xac) * 2 + 0x9a8, b, amount);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(b + 0x27) * 0x21c), -1, text, 0, 0x5f1, *(int *)(b + 1), -1, 0, "_NACHRICHTEN_HS_08");
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(b, *(unsigned short *)rec, 0, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x56a360, the alloc of the same event: both clocks (+0x52 / +0x60) = the start clock (+0x44), due 7 units of the kind 7 and a random 30 .. 59 later */

void he_Unk56a360(int r)
{
    *(AITIME *)(r + 0x52) = *(AITIME *)(r + 0x44);
    *(AITIME *)(r + 0x60) = *(AITIME *)(r + 0x44);
    gm_AddTime(r + 0x52, 7, (unsigned short)util_Random(0x1e) + 0x1e, 0);
}

unsigned short *gm_QueryNext(void);
int gm_BuildKindObjects(char *b);

/* 0x56a3d0, the run handler of the sibling event of 0x56a170 (state 0 -> 1; state 1 with orders: when due, the first good object of the kind 0x17 with +0xe == 1 is looked up
   in the goods building +0xb0 (0x4ffe90 / 0x4ffcc0), ordered (0x4c4880), and every good of the table 0x889400 (0x76fdc4 entries) whose 4 ingredients (+0x2e) are all in stock
   (+0xe >= 2) is ordered 3 times; then the order of the amount as in 0x56a170, the news 0x1bb8 / "_NACHRICHTEN_HS_08"; the record is freed) (ROUGH) */
void he_Unk56a3d0(int ev)
{
    char text[0x200];
    int b, g, rec, amount, i, j, ok, id, o, bo[2], bo2[2];
    char *r91;
    unsigned short x;

    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(0, *(unsigned short *)rec, 0, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    case 0:
        *(int *)(ev + 0x70) = 1;
        return;
    case 1:
        if (*(int *)(ev + 0x14) == 0 && *(int *)(ev + 0x18) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        gm_FindById(bo, 0, 0, *(int *)(ev + 0xac));
        b = bo[0];
        if (b == 0) {
            *(int *)(ev + 0x70) = -1;
            return;
        }
        gm_FindById(0, bo2, 0, *(int *)(ev + 0xb0));
        g = gm_Query(*(int *)(bo2[0] + 0x14), 1, 4, 0x17);
        while (g != 0 && *(int *)(g + 0xe) != 1)
            g = gm_QueryNext();
        if (g != 0) {
            cm_SendExSellObject(*(int *)(b + 2), -1, *(unsigned short *)g, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            gm_BuildKindObjects(bo2[0]);
            for (i = 0; i < *(unsigned short *)(unsigned)&gm_KindObjCount; i++) {
                id = *(int *)((unsigned)&gm_KindObjs + i * 4);
                r91 = (char *)U_I((unsigned)&ai_Goods) + id * 0x41;
                if (*r91 == 0x17)
                    continue;
                ok = 1;
                for (j = 0; j < 4 && ok; j++) {
                    if (*(unsigned short *)(r91 + 0x2e + j * 2) != 0 && *(unsigned short *)(r91 + 0x2e + j * 2) != *(unsigned short *)g) {
                        o = gm_Query(*(int *)(bo2[0] + 0x14), 1, 0, *(unsigned short *)(r91 + 0x2e + j * 2));
                        if (o == 0 || *(int *)(o + 0xe) < 2)
                            ok = 0;
                    }
                }
                if (ok)
                    cm_SendExSellObject(*(int *)(b + 2), -1, id, 3, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            }
            r91 = (char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)g * 0x41;
            x = *(unsigned short *)(r91 + 0x36) * 0x1e;
            amount = ((unsigned short)util_Random(x) + *(unsigned short *)(r91 + 0x36) * 0x1e) * (*(int *)(ev + 0x14) + *(int *)(ev + 0x18));
            cm_SendExSellObject(*(int *)(ev + 0xb4), -1, *(unsigned short *)g, amount, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            txt_Format(text, 0x1bb8, *(unsigned short *)g * 2 + 0x9a8, b, amount);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(b + 0x27) * 0x21c), -1, text, 0, 0x5f1, *(int *)(b + 1), -1, 0, "_NACHRICHTEN_HS_08");
        }
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_Chars + 0x65400); rec += 0x21c) {
            if (*(int *)(rec + 0x17c) == ev)
                sv_MoveToGroup(bo[0], *(unsigned short *)rec, 0, 0);
        }
        he_FreeEvent((int *)ev);
        return;
    }
}
/* 0x56a710, the run handler of the debug event "HE_NEUE_VORKOMMEN_SUCHEN_MSG" (a bug text with the name of the good +0xac): state 0 opens the message panel
   "Misc\EventMessagebox" (0x53d6f0, slot at +0x74), shows the text and goes to state 1; state 1 waits until the panel (the window of the slot +8) is closed */

void he_Unk56a710(int ev)
{
    int id;

    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    case 0:
        id = *(int *)(ev + 0xac);
        he_OpenEventSlot(ev, 0, 0, (int)"Misc\\EventMessagebox", 0x5f2);
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
        io_PrintText("HE_NEUE_VORKOMMEN_SUCHEN_MSG: Ich bin ein Bug !!! %s gefunden!!", id * 2 + 0x9a8);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        if (U_I((unsigned)&d2_ClickedForm) == *(int *)(*(int *)(ev + 0x74) + 8) && U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_ClickedId) == 0) {
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
        }
        return;
    }
}

int gm_FindObjectInst(int id);

/* 0x56a7b0, the alloc of an event of the type 0xb: when another record of the type (+0x10) already works for the same good (+0xac) of another player (+4) the new one is freed,
   else both clocks (+0x52 / +0x60) = the start clock, due now + 1, the handles +0xe8 / +0xec = -1, and a message record (0xf8 bytes: kind 0xf, the player's 0x770b84 entry, the clock, the good,
   the buyer +0x10, the goods building +0xb0) is posted (0x4c4b70) unless +0xf1 is set (then due 1 + 0x1e later); +0xf0 = 4 when the character +0xb4 exists (ROUGH) */
void he_Unk56a7b0(int ev)
{
    char msg[0xf8];
    int e2;

    for (e2 = (int)he_FindEvent(2, 0, 0xb, 3, *(int *)(ev + 0x10)); e2 != 0; e2 = he_NextEvent()) {
        if (*(int *)(e2 + 0xac) == *(int *)(ev + 0xac) && *(int *)(e2 + 4) != *(int *)(ev + 4)) {
            he_FreeEvent((int *)ev);
            return;
        }
    }
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(AITIME *)(ev + 0x60) = *(AITIME *)(ev + 0x44);
    *(int *)(ev + 0xe8) = -1;
    *(int *)(ev + 0xec) = -1;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    memset(msg, 0, sizeof(msg));
    *(AITIME *)(msg + 0x28) = *(AITIME *)(ev + 0x44);
    msg[4] = 0xf;
    *(int *)(msg + 0xc) = -1;
    *(int *)(msg + 8) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c);
    msg[0x36] = 2;
    *(int *)(msg + 0x60) = *(int *)(ev + 0xb0);
    *(int *)(msg + 0x5c) = *(int *)(ev + 0x10);
    *(int *)(msg + 0x58) = *(int *)(ev + 0xac);
    *(int *)(msg + 0xa0) = *(int *)(ev + 4);
    if (gm_FindObjectInst(*(int *)(ev + 0xac)) != 0) {
        if (*(char *)(ev + 0xf1) != 0)
            gm_AddTime(ev + 0x52, 1, 0x1e, 0);
        else
            *(int *)(ev + 0xec) = cm_SendLongMessage(msg);
    } else
        he_FreeEvent((int *)ev);
    *(unsigned char *)(ev + 0xf0) = 0;
    if (gm_FindChar(*(int *)(ev + 0xb4)) != 0)
        *(unsigned char *)(ev + 0xf0) = 4;
}

int ai_Unk5ae8e0();
int gm_FindMarketObject(int id);
float gm_GoodsPrice(unsigned id, unsigned char p);
int gm_ListStock();
int gm_CharCurrency(char *c, char p);

/* 0x56a960, the run handler of the trade event (ROUGH, a skeleton from the disassembly; the two slot loops are simplified): +0xb0 / +0xb4 / +0xac the buildings and the good looked up (0x4fea50 A / C / B),
   +0xd0 (3 good ids, short) and +0xd6 (3 short) with the owners +0xb8 / +0xc4 are the sell / buy slots, +0xe8 / +0xec the transport / message handles, +0xf0 / +0xf1 flags; state -2 resets the
   state and runs 0x541250 for the first player; while a handle is still running the record is due again in 1 unit; a record of the same good that is already running (0x53e400 (2, 0, 0xf, 2, owner)) frees this
   one; the amounts are ordered by 0x4c4880 from the goods buildings (0x4ffe90 ...) and the rest is posted as a message (0x4c4b70) */
void he_Unk56a960(int ev)
{
    int A, B, C, bo[2], g, g2, s, i, k, n, m, flags, amount, left, hit, e2, per;
    unsigned short *sl;
    char msg[0xf8];
    char buf[0x100];

    A = B = C = 0;
    flags = 0;
    left = 0;
    if (*(int *)(ev + 0x70) == -2) {
        *(int *)(ev + 0x70) = 0;
        he_SetEventChar(ev, *(unsigned short *)U_I((unsigned)&ai_PlayerChar));
    }
    if (*(int *)(ev + 0xe8) != -1) {
        if (cm_GetSlotState(*(int *)(ev + 0xe8)) == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            return;
        }
        *(int *)(ev + 0xe8) = -1;
    }
    if (*(int *)(ev + 0xec) != -1) {
        if (cm_GetSlotState(*(int *)(ev + 0xec)) == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            return;
        }
        *(int *)(ev + 0xec) = -1;
    }
    gm_FindById(0, bo, 0, *(int *)(ev + 0xac));
    B = bo[0];
    gm_FindById(bo, 0, 0, *(int *)(ev + 0xb0));
    A = bo[0];
    gm_FindById(0, 0, bo, *(int *)(ev + 0xb4));
    C = bo[0];
    if (B == 0 || A == 0)
        goto done_free;
    for (e2 = (int)he_FindEvent(2, 0, 0xf, 2, *(unsigned short *)(ev + 8)); e2 != 0; e2 = he_NextEvent()) {
        if (*(int *)(e2 + 0xac) == *(int *)(ev + 0xac)) {
            if (*(int *)(e2 + 0xb4) == *(int *)(ev + 0xb0))
                goto rearm;
            goto done_free;
        }
    }
    if (*(char *)(ev + 0xf1) == 0 && ai_Unk5ae8e0(A, B) == 0) {
        ai_Unk5ae8e0(A, B);
        he_FreeEvent((int *)ev);
        return;
    }
    g = gm_Query(*(int *)(B + 0x14), 1, 0, 0x1dd);
    s = *(unsigned char *)(g + 0x1c) < 3 ? *(unsigned char *)(g + 0x1c) : 3;
    hit = 0;
    for (i = 0; i < s && !hit; i++)
        if (*(unsigned short *)(ev + 0xd0 + i * 2) != 0 || *(unsigned short *)(ev + 0xd6 + i * 2) != 0)
            hit = 1;
    if (!hit) {
        /* nothing to trade: the transport is ordered from the first store of the kind 0x2a / 0x116 (0x4c4880) */
        k = gm_Query(*(int *)(A + 0x5d), 2, 6, 0x2a);
        if (k == 0)
            k = gm_Query(*(int *)(A + 0x5d), 2, 6, 0x116);
        for (g2 = k != 0 ? gm_Query(*(int *)(g + 0x14), 1, 5, 0) : 0; g2 != 0; g2 = gm_QueryNext())
            *(int *)(ev + 0xe8) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(k + 2), *(int *)(g + 2), *(unsigned short *)g2, *(int *)(g2 + 0xe), *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        if (*(int *)(ev + 0xe8) == -1)
            goto done_free;
        goto rearm;
    }
    for (sl = (unsigned short *)(ev + 0xd0), i = 0; i < s; i++, sl++) {
        if (*sl != 0 && *(int *)(ev + 0xb8 + i * 4) == *(int *)(A + 1)) {
            g2 = gm_Query(*(int *)(g + 0x14), 1, 0, *sl);
            if (g2 != 0) {
                k = *(char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)A * 0x24d) == 0xa ? gm_FindMarketObject(*sl) : gm_Query(*(int *)(A + 0x5d), 2, 6, 0x2a);
                if (k == 0)
                    k = gm_Query(*(int *)(A + 0x5d), 2, 6, 0x116);
                if (k != 0) {
                    n = gm_ListAmount(B, g2);
                    m = gm_FreeCapacity(k, *(unsigned short *)g2, n);
                    amount = (int)(gm_GoodsPrice(*(unsigned short *)g2, *(unsigned char *)(unsigned)&ai_OwnPlayer) * 0.01);
                    if (n < m)
                        m = n;
                    cm_SendExSellObject(*(int *)(k + 2), *(unsigned short *)g2, m, amount, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                    if (C != 0)
                        *(int *)(C + 0x1b8) += amount * m;
                    if (n - m != 0) {
                        amount = (int)(gm_GoodsPrice(*(unsigned short *)g2, *(unsigned char *)(unsigned)&ai_OwnPlayer) * 0.01);
                        *(int *)(ev + 0xe8) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(B + 2), -1, *(unsigned short *)g2, n - m, *(unsigned char *)(unsigned)&ai_OwnPlayer, amount);
                    }
                }
            }
            *sl = 0;
            *(int *)(ev + 0xb8 + i * 4) = -1;
        }
    }
    if (C != 0 && *(int *)(C + 0x16c) != 0)
        left = gm_CharCurrency((unsigned)&ai_Chars + *(unsigned short *)(*(int *)(C + 0x16c) + 0x27) * 0x21c, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    if (left > *(int *)(C + 0x1b8))
        left = *(int *)(C + 0x1b8);
    for (sl = (unsigned short *)(ev + 0xd6), i = 0; i < s; i++, sl++) {
        if (*sl != 0 && *(int *)(ev + 0xc4 + i * 4) == *(int *)(A + 1)) {
            k = *(char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)A * 0x24d) == 0xa ? gm_FindMarketObject(*sl) : gm_Query(*(int *)(A + 0x5d), 2, 6, 0x2a);
            if (k == 0)
                k = gm_Query(*(int *)(A + 0x5d), 2, 6, 0x116);
            n = 0;
            if (k != 0) {
                g2 = gm_Query(*(int *)(k + 0x14), 0, 1, *sl);
                if (g2 != 0) {
                    if (*(char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)A * 0x24d) == 0xa)
                        amount = (int)(gm_GoodsPrice(*(unsigned short *)g2, *(unsigned char *)(unsigned)&ai_OwnPlayer) * 0.01);
                    else
                        amount = (int)(gm_GoodsPrice(*(unsigned short *)g2, *(unsigned char *)(unsigned)&ai_OwnPlayer) * *(float *)(A + 0x49));
                    per = amount;
                    n = gm_ListStock(k, *(unsigned short *)g2) * *(int *)(g2 + 0x1c) >> 2;
                    n = gm_ListAmount(k, g2) - n;
                    if (left / per < n)
                        n = left / per;
                    if (n < 0)
                        n = 0;
                    if (n != 0)
                        *(int *)(ev + 0xe8) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(B + 2), *(unsigned short *)g2, n, per, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                    if (C != 0 && *(int *)(C + 0x16c) != 0)
                        sprintf(buf, "%s %s %li %li %s", (char *)(C + 0x30), (char *)(*(int *)(C + 0x16c) + 5), n * per, n, (char *)((char *)U_I((unsigned)&ai_Goods) + *(unsigned short *)g2 * 0x41 + 1));
                    left -= n * per;
                    if (C != 0)
                        *(int *)(C + 0x1b8) -= n * per;
                }
            }
            if (n == 0 && *(unsigned char *)(ev + 0xf0) != 0)
                flags |= 1;
            else {
                *sl = 0;
                *(int *)(ev + 0xc4 + i * 4) = -1;
            }
        }
    }
    if (*(unsigned char *)(ev + 0xf0) != 0 && flags != 0) {
        if (*(unsigned short *)(unsigned)&ai_Time.hour < 0x14) {
            gm_AddTime(ev + 0x52, 0, 0x1e, 0);
            *(unsigned char *)(ev + 0xf0) -= 1;
        } else {
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(unsigned char *)(ev + 0xf0) = 0;
        }
        return;
    }
    *(int *)(ev + 0xb0) = -1;
    for (i = 0; i < s && *(int *)(ev + 0xb0) == -1; i++)
        if (*(unsigned short *)(ev + 0xd0 + i * 2) != 0)
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb8 + i * 4);
    for (i = 0; i < s && *(int *)(ev + 0xb0) == -1; i++)
        if (*(unsigned short *)(ev + 0xd6 + i * 2) != 0)
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xc4 + i * 4);
    if (*(int *)(ev + 0xb0) == -1) {
        gm_FindById(bo, 0, 0, *(int *)(ev + 0x10));
        *(int *)(ev + 0xb0) = *(int *)(bo[0] + 1);
    }
    if (*(char *)(ev + 0xf1) != 0) {
        gm_AddTime(ev + 0x52, 1, 0x1e, 0);
        return;
    }
    memset(msg, 0, sizeof(msg));
    msg[4] = 0xf;
    *(int *)(msg + 0xc) = -1;
    *(int *)(msg + 8) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c);
    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_Time.hour);
    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
    msg[0x36] = 2;
    *(int *)(msg + 0x58) = *(int *)(B + 2);
    *(int *)(msg + 0x5c) = *(int *)(A + 1);
    *(int *)(msg + 0x60) = *(int *)(ev + 0xb0);
    *(int *)(msg + 0xa0) = *(int *)(ev + 4);
    if (*(int *)(A + 1) == *(int *)(ev + 0xb0)) {
        gm_AddTime(ev + 0x52, 0, 5, 0);
        return;
    }
    *(int *)(ev + 0xec) = cm_SendLongMessage(msg);
    gm_AddTime(ev + 0x52, 0, 0xa, 0);
    return;
rearm:
    gm_AddTime(ev + 0x52, 0, 5, 0);
    return;
done_free:
    he_FreeEvent((int *)ev);
}
/* 0x56b380, the alloc of an event record: both clocks (+0x52 / +0x60) = the start clock (+0x44), due now + 1, +0xbc / +0xc0 / +0xc4 = 0 */

void he_Unk56b380(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(AITIME *)(ev + 0x60) = *(AITIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0xbc) = 0;
    *(int *)(ev + 0xc0) = 0;
    *(int *)(ev + 0xc4) = 0;
}

void init_HookObject(char *o, unsigned short *p, int kind);

/* 0x56b3f0, a run handler (state 0 when due: the order 0x4c4880 of the good +0xac to the building +0xb4, its handle at +0xc8; state 1 waits for the handle (0x4c3ae0), and when it ended without error
   (not 2) takes the building (+0xb8 = its +2) and starts a countdown of 0x20 (+0xc0 / +0xc4); state 2 each round looks the building up (0x4fea50) and at the end of the countdown runs 0x50d380 for it and frees the record) */
void he_Unk56b3f0(int ev)
{
    int bo[1], g;

    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        goto done;
    case 0:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) > 0)
            return;
        *(int *)(ev + 0xc8) = ((int (*)(int, int, short, int, unsigned char, int))cm_SendExSellObject)(*(int *)(ev + 0xac), -1, *(int *)(ev + 0xb4), 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        if (cm_GetSlotState(*(int *)(ev + 0xc8)) == 0)
            return;
        if (cm_GetSlotState(*(int *)(ev + 0xc8)) == 2)
            goto done;
        g = cm_GetSlotValue(*(int *)(ev + 0xc8));
        *(int *)(ev + 0xb8) = *(int *)(g + 2);
        *(int *)(ev + 0xc0) = 0x20;
        *(int *)(ev + 0xc4) = 0;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 2:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        do {
            gm_FindById(0, bo, 0, *(int *)(ev + 0xb8));
            gm_AddTime(ev + 0x52, 0, 1, 0);
            if (*(int *)(ev + 0xc4) > *(int *)(ev + 0xc0)) {
                init_HookObject(*(int *)(bo[0] + 0x18), bo[0], 1);
                goto done;
            }
        } while (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) < 0);
        return;
    }
    return;
done:
    he_FreeEvent((int *)ev);
}

int ai_BuildingName();
int ai_FindBauplaetze();
int ai_FindNearestObject(float *ref, int *list);  /* 0x58fe10 (unk_58bbd0.c) */

/* 0x56b570, "he_AllocGebaeudeBauen", the alloc of an event of the type 0xd (when another one of the type is still below the state 5 the record is freed): clocks (+0x52 / +0x60) = the start clock, due now + 1, +0xc4 / +0xc8 = 0;
   the data of the object +0xb0 is looked up (0x58f640 / 0x58f7a0 in the scene 0, 0x4478e0 selects it) and every entry is checked (0x58fe10 against +0xd4); without entries or with a miss the record is freed,
   else the error text is formatted and +0x84 = -1 (ROUGH) */
void he_Unk56b570(int ev)
{
    char buf[0x10];
    char tab[0x400];
    int e2, n, i, scene;
    int a[2];

    for (e2 = (int)he_FindEvent(1, 0, 0xd); e2 != 0; e2 = he_NextEvent()) {
        if (e2 != ev && *(int *)(e2 + 0x70) < 5)
            goto done;
    }
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(AITIME *)(ev + 0x60) = *(AITIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0xc4) = 0;
    *(int *)(ev + 0xc8) = 0;
    if (ai_BuildingName(*(int *)(ev + 0xb0), a) == 0)
        goto done;
    scene = U_I((unsigned)&d3_CurrentScene);
    memset(tab, 0, sizeof(tab));
    d3if_SelectScene(0, 1);
    n = ai_FindBauplaetze(a, tab + 8);
    if (n == 0) {
        d3if_SelectScene(scene, 1);
        goto done;
    }
    for (i = 0; i < n; i++) {
        if (ai_FindNearestObject(ev + 0xd4, tab) == 0) {
            he_FreeEvent((int *)ev);
            d3if_SelectScene(scene, 1);
            return;
        }
    }
    txt_Format(buf, "he_AllocGebaeudeBauen: %1N1", *(unsigned short *)(ev + 8));
    d3if_SelectScene(scene, 1);
    *(int *)(ev + 0x84) = -1;
    return;
done:
    he_FreeEvent((int *)ev);
}
/* 0x56b700, an object callback of the event (o: +0x4c the position, +0x84 .. +0x8c a target): places the object at the target lifted by 1.47 (0x411860 (o, pos, target, 0x46, 0x20)) and counts +0xd0 down */

void he_Unk56b700(int o)
{
    float t[3];

    t[0] = *(float *)(o + 0x84) + 1.47;
    t[1] = *(float *)(o + 0x88);
    t[2] = *(float *)(o + 0x8c);
    d3t_PlaceObjectV(o, o + 0x4c, t, 0x46, 0x20);
    *(unsigned char *)(o + 0xd0) -= 1;
}
/* 0x56b750, scene callback: appends the object o to the list (0x40 entries, the count at +0x104); true while there is room */

char he_Unk56b750(int o, int *list)
{
    list[list[0x41] + 1] = o;
    list[0x41]++;
    return list[0x41] < 0x40;
}

char d3if_ShowTree(struct D3OBJECT *obj, char show);

/* 0x56b780, shows the construction state of the building scene object "he_%i" (+4 the id) for the building event ev: with +0xc8 = -1 every child (listed by 0x438d40 with the callback 0x56b750) becomes the type 5 and
   is switched off (0x447300), below the count +0xc4 each child is switched on / off by the numbers in its name ("..._n1_n2": one number = the stage from which it shows, two = the range), above it the
   building (+0xb8, its object +0x61) is shown and the scene object is deleted (0x447190); the scene is restored at the end (ROUGH) */
void he_ShowConstruction(int ev)
{
    char name[0x100];
    char nm[0x100];
    char dig[4];
    int list[0x42];
    int scene, obj, w, i, k, n, o;
    int nums[2];
    char *p;

    scene = U_I((unsigned)&d3_CurrentScene);
    d3if_SelectScene(0, 1);
    sprintf(name, "he_%i", *(int *)(ev + 4));
    obj = d3_FindObject(0, 0x1c0, 0, name);
    w = *(int *)(ev + 0xc8);
    if (w == -1 && obj != 0) {
        list[0x41] = 0;
        list[0] = obj;
        d3_ForEachObject(d3_DefaultScene, obj, 0x2c0, (unsigned)&he_Unk56b750, list);
        for (i = 0; i < list[0x41]; i++) {
            o = list[i + 1];
            if (*(int *)(o + 0x220) != 5 || (*(unsigned char *)(o + 0x213) & 4)) {
                *(int *)(o + 0x220) = 5;
                o = list[i + 1];
                if ((*(unsigned char *)(o + 0x213) & 4) && U_I((unsigned)&d3_QuadTree) != 0)
                    d3q_RemoveObject((D3OBJECT *)o, (D3QNODE *)U_I((unsigned)&d3_QuadTree));
                *(unsigned char *)(list[i + 1] + 0x213) &= ~4;
            }
        }
        d3if_ShowTree(obj, 0);
    } else if (w < *(int *)(ev + 0xc4)) {
        if (obj != 0) {
            list[0x41] = 0;
            list[0] = obj;
            d3_ForEachObject(d3_DefaultScene, obj, 0x2c0, (unsigned)&he_Unk56b750, list);
            for (i = 0; i < list[0x41]; i++) {
                o = list[i + 1];
                if (*(int *)(o + 0x220) != 5 || (*(unsigned char *)(o + 0x213) & 4)) {
                    if ((*(unsigned char *)(o + 0x213) & 4) && U_I((unsigned)&d3_QuadTree) != 0)
                        d3q_RemoveObject((D3OBJECT *)o, (D3QNODE *)U_I((unsigned)&d3_QuadTree));
                    *(int *)(list[i + 1] + 0x220) = 5;
                    *(unsigned char *)(list[i + 1] + 0x213) &= ~4;
                }
                *(int *)(list[i + 1] + 0x224) = 0;
                strcpy(nm, (char *)list[i + 1]);
                n = 0;
                for (p = strchr(nm, '_'); p != 0 && n < 2; p = strchr(nm, '_')) {
                    strncpy(dig, p + 1, 2);
                    dig[2] = 0;
                    if (isdigit(dig[0])) {
                        nums[n] = atoi(dig);
                        n++;
                    }
                    *p = 0;
                }
                if (n == 1)
                    d3if_ShowTree(list[i + 1], w >= nums[0] ? 1 : 0);
                else if (n == 2) {
                    if (w <= nums[0] && w >= nums[1])
                        d3if_ShowTree(list[i + 1], 1);
                    else
                        d3if_ShowTree(list[i + 1], 0);
                }
            }
        }
    } else if (w > *(int *)(ev + 0xc4)) {
        o = gm_FindBuildingById(*(int *)(ev + 0xb8));
        if (o != 0 && *(int *)(o + 0x61) != 0)
            d3if_ShowTree(*(int *)(o + 0x61), 1);
        if (obj != 0) {
            if (U_I((unsigned)&d3_QuadTree) != 0)
                d3q_RemoveObject((D3OBJECT *)obj, (D3QNODE *)U_I((unsigned)&d3_QuadTree));
            d3if_Delete((struct D3OBJECT *)obj);
        }
    }
    d3if_SelectScene(scene, 1);
}
/* 0x56bac0, the highest number in the "_nn" suffixes of the children of the building scene object "he_%i" (id +4), i.e. the construction stage reached: the children are listed by 0x438d40 with the callback 0x56b750, their names parsed from the right ("..._n1_n2" = the stage range) */

int he_Unk56bac0(int ev)
{
    char name[0x100];
    char nm[0x100];
    char dig[4];
    int list[0x42];
    int scene, obj, idx, cnt, o, best;
    int nums[2];
    char *p;

    scene = U_I((unsigned)&d3_CurrentScene);
    best = 0;
    d3if_SelectScene(0, 1);
    sprintf(name, "he_%i", *(int *)(ev + 4));
    obj = d3_FindObject(0, 0x1c0, 0, name);
    list[0] = obj;
    list[0x41] = 0;
    d3_ForEachObject(d3_DefaultScene, obj, 0x2c0, (unsigned)&he_Unk56b750, list);
    if (list[0x41] > 1) {
        for (idx = 1; idx < list[0x41]; idx++) {
            o = list[idx + 1];
            *(int *)(o + 0x224) = 0;
            strcpy(nm, (char *)o);
            cnt = 0;
            for (p = strrchr(nm, '_'); p != 0 && cnt < 2; p = strrchr(nm, '_')) {
                strncpy(dig, p + 1, 2);
                dig[2] = 0;
                if (isdigit(dig[0])) {
                    nums[cnt] = atoi(dig);
                    cnt++;
                }
                *p = 0;
            }
            if (cnt != 0 && best < nums[0])
                best = nums[0];
        }
    }
    d3if_SelectScene(scene, 1);
    return best;
}

struct D3OBJECT * d3_LoadGroup();
int d3t_DropObject();

/* 0x56bc50, creates the two scene objects of the building event ev (arg5): the model of the building type *type (a table of 0x50-byte records at 0x627e98: four codes, then the model name at +0x10, the last record ends it), named "he_<id>" (+4), and the group "model" (arg4, the scaffolding); both are placed at pos (arg2) and rot (arg1), the second is dropped on the ground (0x412a70) and hooked to the building (0x50d380 (o, type, 2)); returns the second object (0 = a load failed) */
int he_Unk56bc50(void *rot, float *pos, unsigned char *type, char *model, int ev)
{
    char name[0x100];
    int scene, obj, obj2, n, k, found;
    int *rec;

    scene = U_I((unsigned)&d3_CurrentScene);
    found = 0;
    n = 0;
    rec = (int *)(unsigned)&full_data_627e98;
    if (rec[0] != 0) {
        while (!found) {
            for (k = 0; k < 4; k++) {
                if (*type == rec[k]) {
                    strcpy(name, (char *)rec + 0x10);
                    found = 1;
                    break;
                }
            }
            rec = (int *)((char *)rec + 0x50);
            n++;
            if (rec[0] == 0)
                break;
        }
    }
    if (!found)
        strcpy(name, (char *)((unsigned)&full_data_627ea8 + n * 0x50));
    d3if_SelectScene(0, 1);
    obj = d3_LoadGroup(name, 0, 0);
    if (obj == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3_SetObjectPos(obj, pos);
    d3_SetObjectRot(obj, rot);
    *(unsigned char *)(obj + 0x211) = *(unsigned char *)(obj + 0x211) & 0xfb | 2;
    sprintf((char *)obj, "he_%i", *(int *)(ev + 4));
    obj2 = d3_LoadGroup(model, 0, 0);
    if (obj2 == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3_SetObjectPos(obj2, pos);
    d3_SetObjectRot(obj2, rot);
    init_StripTorches(obj2);
    d3t_DropObject(obj2, 1, 0);
    if (*(float *)(obj2 + 0x50) < pos[1]) {
        *(float *)(obj2 + 0x4c) = pos[0];
        *(float *)(obj2 + 0x50) = pos[1];
        *(float *)(obj2 + 0x54) = pos[2];
    }
    d3if_ShowTree(obj2, 0);
    d3_RelightObjects(obj2);
    *(int *)(ev + 0xc8) = -1;
    *(int *)(ev + 0xbc) = -1;
    he_ShowConstruction(ev);
    init_HookObject(obj2, type, 2);
    d3if_SelectScene(scene, 1);
    return obj2;
}

int gm_CompareTime();
short * gm_FindChar(int id);
int cm_SendMoveGoods(int a, int b, unsigned char c, int d);
short *gm_PlayerRecord(char *c);
void cm_SendBuildingDestroyed(int a);
int cm_SendExSysMessage(unsigned char id, void *msg);

/* 0x56be30, the run handler of the building construction event (the type 0xd; ROUGH SKELETON: the states 0x70 = -2 .. 7 from the jump table 0x56cda8, the calls in order from the disassembly; the conditions and most call arguments are not recovered yet; the states -1 / -2 hand the building back, rebuild its scene object and free the record) */
void he_Unk56be30(int ev)
{
    int o;

    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xb8));
        d3if_SelectScene(0, 1);
        if (*(int *)(ev + 0xbc) != -1) {
            o = (int)evt_FindById(*(int *)(ev + 0xbc));
            if (o != 0)
                evt_Stop((EVTSCRIPT *)o);
            *(int *)(ev + 0xbc) = -1;
        }
        *(int *)(ev + 0xc8) = *(int *)(ev + 0xc4) + 1;
        he_ShowConstruction(ev);
        if (*(unsigned char *)(ev + 0x78) & 2)
            ((void (*)(int))cm_SendExSysMessage)(0);
        d3if_SelectScene(U_I((unsigned)&d3_CurrentScene), 1);
        he_FreeEvent((int *)ev);
        break;
    case 0:
        /* 0x4fe210 (ev + 0x52, &ai_Time); 0x505df0 (ev + 0xac); 0x4c47e0 (4 args) -> +0xcc; 0x506430; 0x50c4e0 (ev + 0xb4) */
        break;
    case 1:
        if (*(unsigned char *)(ev + 0x78) & 4)
            break;
        if (cm_GetSlotState(*(int *)(ev + 0xcc)) == 0)
            break;
        if (cm_GetSlotState(*(int *)(ev + 0xcc)) == 2)
            cm_SendExSetHE(-1, ev);
        break;
    case 2:
        /* 0x4c45c0 (4 args); 0x4c4c60 */
        break;
    case 3:
        /* 0x4c3b10; 0x4c4c60 */
        break;
    case 4:
        /* 0x4fea50 (out, 0, 0, id); 0x56bc50 (rot, pos, type, model, ev); 0x4c4c60; 0x4478e0 */
        break;
    case 5:
        /* 0x50d380; 0x56bac0 x2; 0x4fe140; 0x4478e0; 0x4fea50; 0x58fc40; 0x458a20; 0x449690; 0x4c4c60; 0x438d40; 0x4287b0; 0x429ab0; 0x42ada0 */
        break;
    case 6:
        /* 0x4fe140; 0x4fea50; 0x58fc40; 0x4c4c60; 0x438d40; 0x458780; 0x447190 */
        break;
    default:
        /* state 7: see the disassembly 0x56c5a9 .. 0x56ccf2 (the long final part) */
        break;
    }
}
/* 0x56cdd0, the alloc of an event of the type 0x4c: both clocks (+0x52 / +0x60) = the start clock (+0x44), due 10 units later (0x4fe140), state 0, +0xac = -1 */

void he_Unk56cdd0(int ev)
{
    *(AITIME *)(ev + 0x52) = *(AITIME *)(ev + 0x44);
    *(AITIME *)(ev + 0x60) = *(AITIME *)(ev + 0x44);
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0xac) = -1;
    gm_AddTime(ev + 0x52, 0, 0xa, 0);
}

int ai_IsMissionFulfilled();
int cm_SendExSysMessage(unsigned char id, void *msg);

/* 0x56ce40, the run handler of the type 0x4c (a character action of the player below 22 h): the player byte 0x624580 (negative = no player: free the record), the record of the player *(unsigned short *)(ev + 8) in the 0x80-entry table 0xf82320 (0x24 bytes: +4 the character's +4, +0x20 the handled flag), the states (-2 / -1 free) 0 = wait for the flag, 1 = start the action 0x4c4db0 (8, &the record's +4) and wait for its handle +0xac, 2 = the handle result (1 = done, state 0 and wait 24; else state 1 and wait 30) */
void he_Unk56ce40(int ev)
{
    int i, h, arg;

    if (*(signed char *)(unsigned)&ai_MissionMode < 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type != 0 && ai_Missions[i].owner == *(int *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c + 4))
            break;
    }
    if (i >= 0x80) {
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        return;
    }
    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        break;
    case 0:
        if (ai_Missions[i].fulfilled == 0 && *(unsigned short *)(unsigned)&ai_Time.hour < 0x16) {
            if (ai_IsMissionFulfilled(&ai_Missions[i]) != 0) {
                ai_Missions[i].fulfilled = 1;
                gm_AddTime(ev + 0x52, 0, 1, 0);
                *(int *)(ev + 0x70) = 1;
                return;
            }
        }
        gm_AddTime(ev + 0x52, 0, 0xa, 0);
        break;
    case 1:
        gm_AddTime(ev + 0x52, 0, 1, 0);
        if (ai_Missions[i].fulfilled == 1 && *(unsigned short *)(unsigned)&ai_Time.hour < 0x16 &&
            U_I((unsigned)&ai_MissionPartner) == -1) {
            arg = ai_Missions[i].owner;
            *(int *)(ev + 0xac) = ((int (*)(unsigned char, void *))cm_SendExSysMessage)(8, &arg);
            *(int *)(ev + 0x70) = 2;
            return;
        }
        gm_AddTime(ev + 0x52, 1, 0, 0);
        *(int *)(ev + 0x70) = 1;
        break;
    case 2:
        h = cm_GetSlotState(*(int *)(ev + 0xac));
        gm_AddTime(ev + 0x52, 0, 1, 0);
        if (h == 0)
            return;
        if (h == 1) {
            *(int *)(ev + 0x70) = 0;
            gm_AddTime(ev + 0x52, 0x18, 0, 0);
            return;
        }
        *(int *)(ev + 0x70) = 1;
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        break;
    }
}
/* 0x56d050, registers the event types of the construction / production group (called by 0x53d4b0): the alloc / run pairs of 3, 4, 5, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0x4c, 0x4d and the type texts of 3, 4, 8, 9; 0 = ok, 1 = a registration failed */

int he_RegisterEventTypes03(void)
{
    if (he_SetEventHandlers(3, (void *)he_AllocIncomeEvent, (void *)he_Unk568be0) != 0)
        return 1;
    sv_RegisterActivity(3, "Abbau", 1, 0x501, 0xac);
    if (he_SetEventHandlers(4, (void *)he_Unk5690c0, (void *)he_Unk569210) != 0)
        return 1;
    sv_RegisterActivity(4, "Produktion", 1, 0x4ff, 0xac);
    if (he_SetEventHandlers(5, (void *)ai_Unk56f040, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_SetEventHandlers(8, (void *)he_Unk56a100, (void *)he_Unk56a170) != 0)
        return 1;
    sv_RegisterActivity(8, "Suchen", 1, 0x500, 0xac);
    if (he_SetEventHandlers(9, (void *)he_Unk56a360, (void *)he_Unk56a3d0) != 0)
        return 1;
    sv_RegisterActivity(9, "Suchen", 1, 0x500, -1);
    if (he_SetEventHandlers(10, (void *)d3io_NoopCallback, (void *)he_Unk56a710) != 0)
        return 1;
    if (he_SetEventHandlers(0xb, (void *)he_Unk56a7b0, (void *)he_Unk56a960) != 0)
        return 1;
    if (he_SetEventHandlers(0xc, (void *)he_Unk56b380, (void *)he_Unk56b3f0) != 0)
        return 1;
    if (he_SetEventHandlers(0xd, (void *)he_Unk56b570, (void *)he_Unk56be30) != 0)
        return 1;
    if (he_SetEventHandlers(0xe, (void *)d3io_NoopCallback, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_SetEventHandlers(0x4c, (void *)he_Unk56cdd0, (void *)he_Unk56ce40) != 0)
        return 1;
    if (he_SetEventHandlers(0x4d, (void *)ai_Unk56f040, (void *)d3io_NoopCallback) != 0)
        return 1;
    return 0;
}

void he_Unk56d2d0();
void he_Unk56d3d0();
void he_Unk56d610();
void he_Unk56d730();
void he_Unk56d990();
void he_Unk56daa0();
void he_Unk56dd20();
void he_Unk56de10();

/* 0x56d220, registers the research event types 0x95 .. 0x98 (called by 0x53d4b0) and the texts of 0x95 / 0x96; 0 = ok, 1 = a registration failed */
int he_RegisterEventTypes95(void)
{
    if (he_SetEventHandlers(0x95, (void *)he_Unk56d2d0, (void *)he_Unk56d3d0) == 0) {
        if (he_SetEventHandlers(0x96, (void *)he_Unk56d610, (void *)he_Unk56d730) == 0) {
            if (he_SetEventHandlers(0x97, (void *)he_Unk56d990, (void *)he_Unk56daa0) == 0) {
                if (he_SetEventHandlers(0x98, (void *)he_Unk56dd20, (void *)he_Unk56de10) == 0) {
                    sv_RegisterActivity(0x95, "erforschen", 1, 0x500, -1);
                    sv_RegisterActivity(0x96, "erforschen", 1, 0x500, -1);
                    return 0;
                }
            }
        }
    }
    return 1;
}

union AHMGRID * ahm_RandomRecipe();
int ahm_ResearchMinutes();
char *gm_Query(void *list, int n, ...);
void snd_PlayFavourComment(void);

/* 0x56d2d0, the alloc of the research event 0x95: the character +0xac and its research record (0x4eaea0), the amount of its +4 (0x4eafa0) into +0xb4 / +0xb8, the clocks +0xbc / +0x52 = now, due 5 (0x4fe140), state 0, the building +0x16c of the character registered with 0x524260 (building, 0x4ffe90 (building's +0x5d, 2, 6, 0, 0x24b, character id, ev)); without a record the event is freed */
void he_Unk56d2d0(int ev)
{
    char *c, *p;
    int v;

    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c != 0) {
        p = (char *)ahm_RandomRecipe(c);
        if (p != 0) {
            *(int *)(ev + 0xb0) = *(int *)p;
            v = ahm_ResearchMinutes(c, p);
            *(int *)(ev + 0xb4) = v;
            *(int *)(ev + 0xb8) = v;
            *(AITIME *)(ev + 0xbc) = ai_Time;
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0x70) = 0;
            v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0, 0x24b,
                             *(unsigned short *)c, ev);
            ((void (*)(int, int))sv_MoveToGroup)(*(int *)(c + 0x16c), v);
            snd_PlayFavourComment();
            return;
        }
    }
    he_FreeEvent((int *)ev);
}

int gm_ClockSeason();
int ahm_HasRecord();
void ai_DomizilMessage(int param_1, int param_2, int param_3);
int ahm_SendAddPlace();
int gm_MinutesBetween();

/* 0x56d3d0, the run handler of the research event 0x95: states -2 / -1 free; in the night of the day (the float ranges 0x617830 / 0x617840) the clock +0xbc is reset and the event retried after 1; state 1 hands the result over (0x5a9c30, 0x4ecaa0), releases the building group and frees; state 0 waits for the research (0x56ddc0), takes the elapsed time from +0xbc (0x4fe280) off the amount +0xb4 and retries after 5 until the amount is used up (state 1) */
void he_Unk56d3d0(int ev)
{
    char *c;
    int d, st, h, v;

    d = gm_ClockSeason((void *)(unsigned)&ai_Time);
    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0 || ahm_HasRecord(c) == 0)
        *(int *)(ev + 0x70) = -2;
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(float *)((unsigned)&gm_DayEndHours + d * 4) <= (float)*(unsigned short *)(unsigned)&ai_Time.hour ||
        (float)*(unsigned short *)(unsigned)&ai_Time.hour < *(float *)((unsigned)&gm_DayStartHours + d * 4)) {
        *(AITIME *)(ev + 0xbc) = ai_Time;
        gm_AddTime(ev + 0x52, 1, 0, 0);
        return;
    }
    if (st == 1) {
        v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0);
        h = gm_FindChar(*(int *)(ev + 0xc));
        if (h != 0)
            ai_DomizilMessageK(h, c, v, *(int *)(ev + 0xb0));
        ahm_SendAddPlace(c, *(int *)(ev + 0xb0));
        sv_MoveToGroup(0, 0, *(unsigned short *)c, 0);
        he_FreeEvent((int *)ev);
        return;
    }
    if (he_RoomBusy(*(int *)(c + 0x16c), 0x24b) != 0) {
        *(AITIME *)(ev + 0xbc) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0xf, 0);
        return;
    }
    if (st == 0) {
        v = gm_MinutesBetween((void *)(ev + 0xbc), (void *)(unsigned)&ai_Time);
        *(int *)(ev + 0xb4) -= v;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(AITIME *)(ev + 0xbc) = ai_Time;
        if (*(int *)(ev + 0xb4) < 1)
            *(int *)(ev + 0x70) = 1;
    }
}

union AHMGRID * ahm_FindRecipe();
float ahm_ResearchProgress();

/* 0x56d610, the alloc of the research event 0x96: the character +0xac, its research record 0x4eae60 (the +0xb0 kind) and the amount 0x4eafd0 (character, record, &clock +0xbc) into +0xb4; above the constant 0x5eb788: +0xb8 = the clock +0xbc, the clocks +0x52 / +0xc0 = now, due 5, state 0, the building registered (0x4ffe90 / 0x524260); else the event frees itself */
void he_Unk56d610(int ev)
{
    char *c, *p;
    int v;

    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0)
        goto done;
    p = (char *)ahm_FindRecipe(*(int *)(ev + 0xb0));
    if (p == 0)
        goto done;
    *(float *)(ev + 0xb4) = ahm_ResearchProgress(c, p, (void *)(ev + 0xbc));
    if (*(float *)(ev + 0xb4) > *(float *)(unsigned)&ai_FloatZero)
        goto done;
    *(int *)(ev + 0xb8) = *(int *)(ev + 0xbc);
    *(AITIME *)(ev + 0xc0) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    *(int *)(ev + 0x70) = 0;
    v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0, 0x24b, *(unsigned short *)c, ev);
    ((void (*)(int, int))sv_MoveToGroup)(*(int *)(c + 0x16c), v);
    snd_PlayFavourComment();
    return;
done:
    he_FreeEvent((int *)ev);
}

struct AHMSLOT * ahm_FindSlot();
void ai_DomizilMessageResearchKind(int param_1, int param_2, int param_3, int param_4);
int ahm_SendSetPlaceValue(char *c, int id, float v);

/* 0x56d730, the run handler of the research event 0x96: like 0x56d3d0 but the record is 0x4eabe0 (the +0xb0 kind) and the clock is +0xc0; state 1 hands the result over (0x5a9bc0, 0x4ecc30 with +0xb4 + the record's +4), releases the building group and frees; state 0 takes the elapsed time off +0xb8 and retries after 5 until it is used up */
void he_Unk56d730(int ev)
{
    char *c, *p;
    int d, st, h, v;

    d = gm_ClockSeason((void *)(unsigned)&ai_Time);
    p = 0;
    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0 || ahm_HasRecord(c) == 0 || (p = (char *)ahm_FindSlot(c, *(int *)(ev + 0xb0))) == 0)
        *(int *)(ev + 0x70) = -2;
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(float *)((unsigned)&gm_DayEndHours + d * 4) <= (float)*(unsigned short *)(unsigned)&ai_Time.hour ||
        (float)*(unsigned short *)(unsigned)&ai_Time.hour < *(float *)((unsigned)&gm_DayStartHours + d * 4)) {
        *(AITIME *)(ev + 0xc0) = ai_Time;
        gm_AddTime(ev + 0x52, 1, 0, 0);
        return;
    }
    if (he_RoomBusy(*(int *)(c + 0x16c), 0x24b) != 0) {
        *(AITIME *)(ev + 0xc0) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0xf, 0);
        return;
    }
    if (st == 1) {
        v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0, 0x24b);
        h = gm_FindChar(*(int *)(ev + 0xc));
        if (h != 0)
            ai_DomizilMessageResearchKind(h, c, v, *(int *)(ev + 0xb0));
        ahm_SendSetPlaceValue(c, *(int *)(ev + 0xb0), *(float *)(ev + 0xb4) + *(float *)(p + 4));
        sv_MoveToGroup(0, 0, *(unsigned short *)c, 0);
        he_FreeEvent((int *)ev);
        return;
    }
    if (st == 0) {
        v = gm_MinutesBetween((void *)(ev + 0xc0), (void *)(unsigned)&ai_Time);
        *(int *)(ev + 0xb8) -= v;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(AITIME *)(ev + 0xc0) = ai_Time;
        if (*(int *)(ev + 0xb8) < 1)
            *(int *)(ev + 0x70) = 1;
    }
}

float ahm_ShareProgress();
float ahm_Share();
void ai_DomizilMessageResearch(int param_1, int param_2, int param_3, int param_4);
int ahm_SendSetShare();  /* @original: he.c's call at 14930 passes 2 arguments to ahm.h's 3-parameter prototype, so this TU was compiled against the non-prototype declaration */

/* 0x56d990, the alloc of the research event 0x97: like 0x56d610 but the amount is 0x4eb0b0 (character, the +0xb0 kind, &clock +0xbc), no separate record; the clock +0xc0 and the registration as there */
void he_Unk56d990(int ev)
{
    char *c;
    int v;

    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0)
        goto done;
    *(float *)(ev + 0xb4) = ahm_ShareProgress(c, *(int *)(ev + 0xb0), (void *)(ev + 0xbc));
    if (*(float *)(ev + 0xb4) > *(float *)(unsigned)&ai_FloatZero)
        goto done;
    *(int *)(ev + 0xb8) = *(int *)(ev + 0xbc);
    *(AITIME *)(ev + 0xc0) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    *(int *)(ev + 0x70) = 0;
    v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0, 0x24b, *(unsigned short *)c, ev);
    ((void (*)(int, int))sv_MoveToGroup)(*(int *)(c + 0x16c), v);
    snd_PlayFavourComment();
    return;
done:
    he_FreeEvent((int *)ev);
}
/* 0x56daa0, the run handler of the research event 0x97: the record is 0x4eab20 (the +0xb0 kind); state 1 hands the result over (0x5a9b50, 0x4ec940), releases the building group and frees; state 0 takes the elapsed time off +0xb8 and retries after 5 until it is used up; in the night +0x52 is retried after 1 / 0xf */

void he_Unk56daa0(int ev)
{
    char *c;
    int d, st, h, v;

    d = gm_ClockSeason((void *)(unsigned)&ai_Time);
    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0 || ahm_HasRecord(c) == 0 || ahm_Share(c, *(int *)(ev + 0xb0)) == 0)
        *(int *)(ev + 0x70) = -2;
    st = *(int *)(ev + 0x70);
    if (st == -2 || st == -1) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(float *)((unsigned)&gm_DayEndHours + d * 4) <= (float)*(unsigned short *)(unsigned)&ai_Time.hour ||
        (float)*(unsigned short *)(unsigned)&ai_Time.hour < *(float *)((unsigned)&gm_DayStartHours + d * 4)) {
        gm_AddTime(ev + 0x52, 1, 0, 0);
        return;
    }
    if (he_RoomBusy(*(int *)(c + 0x16c), 0x24b) != 0) {
        gm_AddTime(ev + 0x52, 0, 0xf, 0);
        return;
    }
    if (st == 1) {
        v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0, 0x24b);
        h = gm_FindChar(*(int *)(ev + 0xc));
        if (h != 0)
            ai_DomizilMessageResearch(h, c, v, *(int *)(ev + 0xb0));
        ahm_SendSetShare(c, *(int *)(ev + 0xb0));
        sv_MoveToGroup(0, 0, *(unsigned short *)c, 0);
        he_FreeEvent((int *)ev);
        return;
    }
    if (st == 0) {
        v = gm_MinutesBetween((void *)(ev + 0xc0), (void *)(unsigned)&ai_Time);
        *(int *)(ev + 0xb8) -= v;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(AITIME *)(ev + 0xc0) = ai_Time;
        if (*(int *)(ev + 0xb8) < 1)
            *(int *)(ev + 0x70) = 1;
    }
}
/* 0x56dd20, the alloc of the research event 0x98 (not while paused, flags +0x78 & 4): the clock +0x52 = now, the hour set to a random 9 .. 16 (or 13 on the next day from 20 on), +0xac = 0, +0xb0 / +0xb4 = -1, and the handler scheduled (0x4c4c60) into +0x84 */

void he_Unk56dd20(int ev)
{
    int h;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    *(AITIME *)(ev + 0x52) = ai_Time;
    if (*(unsigned short *)(ev + 0x56) < 0x14) {
        if (*(unsigned short *)(ev + 0x56) < 9) {
            h = util_Random(8);
            *(unsigned short *)(ev + 0x56) = h + 9;
        }
    } else {
        *(unsigned short *)(ev + 0x56) = 0xd;
        *(int *)(ev + 0x52) += 1;
    }
    *(int *)(ev + 0xac) = 0;
    *(int *)(ev + 0xb0) = -1;
    *(int *)(ev + 0xb4) = -1;
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
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

int ahm_ResearchState();
int ahm_SendAddValue(char *c, float v);
float util_RandomF();
void he_Unk56e070();

/* 0x56de10, the run handler of the research event 0x98 (the random event of a character +0xc; ROUGH): -1 / -2 free, paused (+0x78 & 4) returns, a missing character / research record reschedules with 0x4c4c60 (-1, ev); the clock +0x52 = now + 1 day, the hour = the day's window (0x617830 / 0x617840 by (day + 1) % 4) plus a random part, the minute = random(4) * 15; with a pending 0x4ecd50 value (+0xac) it is applied and the handler rescheduled; else the task 0x4eb180 (building's +0x16c, 1) decides: <= the building's +0x65 +4 gives a random offset by the task kind (1 .. 4, negated; else positive), above it the scene action 0x56e070 (character, ev, building, task) runs; then 0x4ecd50 (character, offset) and 0x4c4c60 (0, ev) */
void he_Unk56de10(int ev)
{
    char *c;
    int day, st, h, v;
    float f;
    int a, b;

    day = gm_ClockSeason((void *)(unsigned)&ai_Time);
    h = (day + 1) % 4;
    c = (char *)gm_FindChar(*(int *)(ev + 0xc));
    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    if (c == 0 || ahm_HasRecord(c) == 0 || *(int *)(ev + 0x70) == -2 || *(int *)(ev + 0x70) == -1) {
        cm_SendExSetHE(-1, ev);
        return;
    }
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x52) += 1;
    *(unsigned short *)(ev + 0x56) = util_Random((int)(*(float *)((unsigned)&gm_DayEndHours + h * 4) - *(float *)((unsigned)&gm_DayStartHours + h * 4))) +
                                     (int)*(float *)((unsigned)&gm_DayStartHours + h * 4);
    *(int *)(ev + 0x58) = util_Random(4) * 15;
    if (*(int *)(ev + 0xac) != 0) {
        f = (util_RandomF() + *(float *)(unsigned)&ai_FloatOne) * 0.25f;
        ahm_SendAddValue(c, f);
        *(int *)(ev + 0xac) = 0;
        cm_SendExSetHE(0, ev);
        return;
    }
    v = ahm_ResearchState(*(int *)(c + 0x16c), &b, &a, 1);
    f = (util_RandomF() + *(float *)(unsigned)&ai_FloatOne) * 0.25f;
    if (f <= *(float *)(*(int *)(*(int *)(c + 0x16c) + 0x65) + 4)) {
        switch (v) {
        case 1:
            f = -(util_RandomF() + *(float *)(unsigned)&ai_FloatOne) * (0.100000001f);
            break;
        case 2:
            f = -(util_RandomF() * (0.150000006f) + (0.100000001f));
            break;
        case 3:
        case 4:
            f = -(util_RandomF() * 0.2f + (0.100000001f));
            break;
        default:
            f = util_RandomF() * (0.150000006f) + (0.0500000007f);
            break;
        }
        ahm_SendAddValue(c, f);
    } else {
        he_Unk56e070(c, ev, *(void **)(c + 0x16c), v);
    }
    cm_SendExSetHE(0, ev);
}

void cm_BeginFieldBlock(char *o, int id);
int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v);
void cm_SendFieldDeltas(void);
void ai_DomizilMessageTask(int param_1, int param_2, int param_3, unsigned short *param_4);
void ai_DomizilMessage04(int param_1, int param_2, int param_3);
void ai_DomizilMessageAge(int param_1, int param_2, int param_3);
void ai_DomizilMessagePlace(int param_1, int param_2, int param_3, int param_4);
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);

/* 0x56e070, the scene action of the research event 0x98 (ROUGH, the selection array of the original is not recovered): only for a building of the kind 0x1e (the kind record 0x7e3f30 + kind * 0x24d); bools: a task running 0x24b / 0x256 / 0x266 with its +0x1c = 0, the building's +0x5c above 10, a running effect in the +0x65 record (six 8-byte entries: int at +0xc, float at +0x10 above 0.2); a random pick of five candidates (start random(4) + 1) drives the action: 1 a random job 0x4c3ba0 / 0x4c3c00 (1, 0x1c / 0x20) / 0x4c49a0 and 0x5a9d60; 2 a task 0x4c3d00 (1, 0x5c, random) and 0x5a9ca0; 3 0x56fbc0 (c, random(3) + 1) and 0x5a9d00; 4 an effect from the +0x65 record reduced by a random 0.05 .. 0.15 (0x4ecc30 / 0x5a9dd0); the pick goes to +0xac of ev */
void he_Unk56e070(char *c, int ev, unsigned char *b, int task)
{
    int *rec;
    int h[3];
    int f1, f2, i, n, pick, sel;
    float fv;

    if (*(unsigned char *)((unsigned)&ai_BTypes + *b * 0x24d) != 0x1e)
        return;
    sel = 0;
    f1 = 0;
    f2 = 10 < (char)b[0x5c];
    rec = *(int **)(b + 0x65);
    if (task != 1) {
        for (i = 0; i < 6; i++) {
            if (*(int *)((char *)rec + 0xc + i * 8) != 0 && 0.2 < *(float *)((char *)rec + 0x10 + i * 8)) {
                f1 = 1;
                break;
            }
        }
    }
    h[0] = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x24b);
    if (h[0] != 0 && *(int *)(h[0] + 0x1c) == 0)
        f2 = 1;
    h[1] = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x256);
    if (h[1] != 0 && *(int *)(h[1] + 0x1c) == 0)
        f2 = 1;
    h[2] = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x266);
    if (h[2] != 0 && *(int *)(h[2] + 0x1c) == 0)
        f2 = 1;
    pick = (util_Random(4) & 0xffff) + 1;
    for (i = 0; i < 5; i++) {
        n = i + 1;
        if (pick == 1 && f2)
            sel = n;
        else if (pick == 2 && f1)
            sel = n;
        else if (pick == 3 && 1)
            sel = n;
        else if (pick == 4 && f1)
            sel = n;
        else if (pick == 5)
            sel = n;
        if (sel)
            break;
        pick = pick % 5 + 1;
    }
    switch (sel) {
    case 1:
        {
            int t = (util_Random(3) & 0xffff);
            int v = (util_Random(4) & 0xffff) + 3;
            cm_BeginFieldBlock(h[0], *(int *)(h[0] + 2));
            cm_AddFieldDelta(4, 1, 0x1c, &t);
            cm_AddFieldDelta(4, 1, 0x20, &v);
            cm_SendFieldDeltas();
            ai_DomizilMessageTask((void *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), c, b, h[0]);
        }
        break;
    case 2:
        {
            int t = (util_Random(3) & 0xffff);
            int v = -4 - t;
            cm_BeginFieldBlock(b, *(int *)(b + 1));
            cm_AddFieldValues(1, 1, 0x5c, &v);
            cm_SendFieldDeltas();
            ai_DomizilMessage04((void *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), c, b);
        }
        break;
    case 3:
        he_SetLifeByAge(c, (util_Random(3) & 0xffff) + 1);
        ai_DomizilMessageAge((void *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), c, b);
        break;
    case 4:
        {
            int k = util_Random(6) & 0xffff;
            for (i = 0; i < 6; i++) {
                if (*(int *)((char *)rec + 0xc + k * 8) != 0 && 0.2 < *(float *)((char *)rec + 0x10 + k * 8)) {
                    fv = *(float *)((char *)rec + 0x10 + k * 8) - (util_RandomF() * 0.1f + 0.05f);
                    ahm_SendSetPlaceValue(c, *(int *)((char *)rec + 0xc + k * 8), fv);
                    ai_DomizilMessagePlace((void *)((unsigned)&ai_Chars + *(unsigned short *)(b + 0x27) * 0x21c), c, b,
                                 *(int *)((char *)rec + 0xc + k * 8));
                    break;
                }
                k = (k + 1) % 6;
            }
        }
        break;
    }
    *(int *)(ev + 0xac) = sel;
}

void he_Unk56e460();
void he_Unk56e5b0();
void he_AllocEvent9a();
void he_Unk56f0b0();
void he_Unk56f050();

/* 0x56e400, registers the event types 0x99 .. 0x9b (called by 0x53d4b0): the pairs (0x56e460, 0x56e5b0), (0x56efc0, 0x56f040) and (0x56f050, 0x56f0b0); 0 = ok, 1 = a registration failed */
int he_RegisterEventTypes99(void)
{
    if (he_SetEventHandlers(0x99, (void *)he_Unk56e460, (void *)he_Unk56e5b0) == 0) {
        if (he_SetEventHandlers(0x9a, (void *)he_AllocEvent9a, (void *)ai_Unk56f040) == 0) {
            if (he_SetEventHandlers(0x9b, (void *)he_Unk56f050, (void *)he_Unk56f0b0) == 0)
                return 0;
        }
    }
    return 1;
}


void cm_SendSpawnCharacter(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s);

/* 0x56e460, the alloc of the event 0x99 (not while paused, flags +0x78 & 4): when another event of the type 0x99 of the same player (+8) already runs, this one only announces the text 0x3ff7 (0x5176d0 / 0x53dda0), schedules with the handle -1 (0x4c4c60 ( -1, ev) into +0x84) and frees; else the start clock +0x44 and the due clock +0x52 = now, due 1, +0xcc = the notification 0x4c59f0 (0xd, ev's +0xb0, -1, 0x2d, 0, 0, the text 0x628dc8) and the handler scheduled (0x4c4c60 (0, ev)) into +0x84 */
void he_Unk56e460(int ev)
{
    char text[0x400];
    int r;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    for (r = he_FindEvent(1, 0, 0x99); r != 0; r = he_NextEvent()) {
        if (r != ev && *(unsigned short *)(r + 8) == *(unsigned short *)(ev + 8))
            goto taken;
    }
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0xcc) = ((int (*)(int, int, int, int, int, int, const char *))cm_SendSpawnCharacter)(0xd, *(int *)(ev + 0xb0), -1, 0x2d, 0, 0, (char *)(unsigned)&full_data_628dc8);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    return;
taken:
    txt_Format(text, 0x3ff7, *(unsigned short *)(ev + 8));
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fa, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
    he_FreeEvent((int *)ev);
}

int gm_MinutesBetween();                 /* 0x4fe280 */
void cm_SendKillCharacter(int a, int b); /* 0x4c4e00 */
void cm_SendChrStop(int a); /* 0x4c5230 */
int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out);   /* 0x4a63f0 (am.h); the 7-dword callers go through AI_4A63F0_7 */
int he_Unk56f1b0();                 /* 0x56f1b0 */
int he_Unk56f260();                 /* 0x56f260 */
int gv_GetTableRecord(int i, int *out);                 /* 0x4f2720 (the record of a meeting slot) */
int he_Unk56f2e0();                 /* 0x56f2e0 */
int he_NoEvent9bFor();                 /* 0x56f3b0 */
int he_Unk56f3f0();                 /* 0x56f3f0 */
int he_Unk56f3f0();                 /* 0x56f3f0 */
int he_Unk56f600();                 /* 0x56f600 */
int he_Unk56f740();                 /* 0x56f740 */
int gm_FindById();                 /* 0x4fea50 */
void cm_SendChrStop(int a); /* 0x4c5230 */
void ai_Unk56f040(int *r);          /* 0x56f040 */

/* 0x56e5b0, the run of the event 0x99 (the construction run): the states -1 / -2 free the event (with the text 0x628dd4 to the player when the state is -1); paused or with the handle +0x84 still active it does nothing. State 0 waits for the notification +0xcc (0x4c3ae0 / 0x4c3b10: the building kind into +0xb8); kind 3 schedules state 6, else the text 0x628dc8 (0x4c52e0) and state 1. States 1 / 3 look up the building of +0xb8 (0x505df0, done = [0x184]+0x128); while it is not done they wait (0 / 4 minutes); when it is, the checks 0x56f2e0 / 0x56f1b0 / 0x56f3f0 pick the texts 0x4004 .. 0x401a (0x5176d0 / 0x53dda0) and state 4. State 4 the text 0x628dc8 and state 5. State 5 waits until the building is done, then waits the rest of the run time (0x21c / 0xb40 / 0x5a0 seconds by the kind +0xbc, at least 5) and frees. State 6 the domicile search 0x56f600 (state 7; else the action 0x4a63f0 and state 6), state 7 the search 0x56f740 (state 6 / 7) */
void he_Unk56e5b0(int ev)
{
    char text[0x100];
    char *per;
    int r, e, k;

    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2) {
        if (*(unsigned char *)(ev + 0x78) & 2) {
            if (gm_FindChar(HM_I(ev, 0xb8)) != 0)
                cm_SendKillCharacter(HM_I(ev, 0xb8), 1);
            if (HM_I(ev, 0x70) == -1)
                he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, (char *)U_I((unsigned)&full_data_144a1a4), 1, 0x5fa,
                             (char *)(unsigned)&full_data_628dd4);
        }
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    if (HM_I(ev, 0x84) != -1 && cm_GetSlotState(HM_I(ev, 0x84)) == 0)
        return;
    HM_I(ev, 0x84) = -1;
    per = (char *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c);
    switch (HM_I(ev, 0x70)) {
    case 0:
        if (cm_GetSlotState(HM_I(ev, 0xcc)) == 1 && (r = cm_GetSlotValue(HM_I(ev, 0xcc))) != 0) {
            HM_I(ev, 0xb8) = HM_I(r, 4);
            if (HM_I(ev, 0xbc) == 3) {
                *(AITIME *)(ev + 0x52) = ai_Time;
                *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
            } else if ((e = gm_BuildQuery(1, 1, HM_I(ev, 0xac))) != 0) {
                *(int *)(ev + 0xcc) = cm_SendChrGotoBuilding(HM_I(r, 4), HM_I(e, 1), -1, 0, 1, (char *)(unsigned)&full_data_628dc8);
                *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            } else {
                *(AITIME *)(ev + 0x52) = ai_Time;
                *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            }
        } else
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 1:
        HM_I(ev, 0xcc) = -1;
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r != 0 && HM_I(HM_I(r, 0x184), 0x128) != 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            return;
        }
        if (r == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (gm_BuildQuery(1, 1, HM_I(ev, 0xac)) != 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
            return;
        }
        /* falls through to case 3 */
    case 3:
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        k = gm_FindChar(HM_I(ev, 0xb4));
        e = gm_BuildQuery(1, 1, HM_I(ev, 0xb0));
        if (per != 0 && k != 0 && e != 0) {
            switch (HM_I(ev, 0xbc) - 1) {
            case 0:
                if (he_Unk56f2e0(per, k, HM_I(ev, 0xc0)) != 0) {
                    txt_Format(text, 0x4004, *(unsigned short *)k);
                    he_ShowNews(HM_I(per, 4), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_628e28);
                    txt_Format(text, 0x4006, HM_I(ev, 0xc0) + 0x4007);
                    he_ShowNews(HM_I(k, 4), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e10);
                } else {
                    txt_Format(text, 0x4005, *(unsigned short *)k);
                    he_ShowNews(HM_I(per, 4), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_628df8);
                    txt_Format(text, 0x400a);
                    he_ShowNews(HM_I(k, 4), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e10);
                }
                break;
            case 1:
                if (he_Unk56f1b0(per, k, HM_I(ev, 0xc0)) != 0)
                    txt_Format(text, 0x4019, 0x400c, *(unsigned short *)k, *(unsigned short *)k);
                else
                    txt_Format(text, 0x401a, 0x400c, *(unsigned short *)k, *(unsigned short *)k);
                he_ShowNews(HM_I(per, 4), -1, text, 1, 0x5fa, 0);
                break;
            case 2:
                *(AITIME *)(ev + 0x52) = ai_Time;
                HM_I(ev, 0xc4) = -1;
                HM_I(ev, 0xc8) = 0;
                *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
                break;
            case 3:
                he_Unk56f3f0(HM_I(e, 1), k);
                break;
            }
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
        return;
    case 4:
        r = gm_FindChar(HM_I(ev, 0xb8));
        *(AITIME *)(ev + 0x52) = ai_Time;
        if (r == 0)
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        else {
            *(int *)(ev + 0xcc) = cm_SendChrGotoBuilding(HM_I(ev, 0xb0), HM_I(ev, 0xb8), -1, 0, 0, (char *)(unsigned)&full_data_628dc8);
            *(int *)(ev + 0x84) = cm_SendExSetHE(5, ev);
        }
        return;
    case 5:
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (HM_I(HM_I(r, 0x184), 0x128) != 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            return;
        }
        e = gm_MinutesBetween(ev + 0x44, &ai_Time);
        switch (HM_I(ev, 0xbc) - 1) {
        case 1:
            k = 0x21c;
            break;
        case 2:
            k = 0xb40;
            break;
        default:
            k = 0x5a0;
            break;
        }
        k -= e;
        if (k <= 5)
            k = 5;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, k / 60, k % 60, 0);
        cm_SendKillCharacter(HM_I(ev, 0xb8), 1);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 6:
        e = gm_MinutesBetween(ev + 0x44, &ai_Time);
        if (e > 0x5a0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
            return;
        }
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if ((k = he_Unk56f600(per, r, HM_I(ev, 0xc4))) != 0) {
            HM_I(ev, 0xac) = HM_I(k, 0xb4);
            HM_I(ev, 0xc4) = HM_I(k, 4);
            HM_I(ev, 0xc8) = 1;
            cm_SendChrStop(HM_I(r, 4));
            cm_SendChrGotoBuilding(HM_I(r, 4), HM_I(ev, 0xac), -1, 0, 1, (char *)(unsigned)&full_data_628dec);
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(7, ev);
        } else {
            AI_4A63F0_7(per, 0, 0xa7f4c7a0, 1, 0, 100.0f, (int *)(ev + 0xac));
            HM_I(ev, 0xc4) = -1;
            HM_I(ev, 0xc8) = 0;
            cm_SendChrStop(HM_I(r, 4));
            cm_SendChrGotoBuilding(HM_I(r, 4), HM_I(ev, 0xac), -1, 0, 1, (char *)(unsigned)&full_data_628dec);
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 0xf, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
        }
        return;
    case 7:
        e = gm_MinutesBetween(ev + 0x44, &ai_Time);
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (e > 0x5a0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
            return;
        }
        if (he_Unk56f740(r, ev, he_FindEvent(2, 0, 0x18, 1, HM_I(ev, 0xc4))) != 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
        } else {
            gm_AddTime(ev + 0x52, 0, 0, 0);
            *(AITIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(7, ev);
        }
        return;
    case 2:
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
    if (he_NoEvent9aFor(e->bldId, gm_FindChar(e->id2)) != 0) {
        gm_AddTime(&e->end, 0x18, 0, 0);
        cm_SendExSetHE(0, ev);
    }
    e->exSlot = cm_SendExSetHE(-1, ev);
}
/* 0x56f040, the free forwarder of the event 0x9a (jmp 0x53e1f0) */

void ai_Unk56f040(int *r)
{
    he_FreeEvent(r);
}
/* 0x56f050, the alloc of the event 0x9b (not while paused, flags +0x78 & 4): due = now +0x30 hours (0x4fe140), state 0 scheduled (0x4c4c60) */

void he_Unk56f050(int ev)
{
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x30, 0, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
/* 0x56f0b0, the run of the event 0x9b: any state but 0 frees; the state 0 sends the text 0x4035 (0x628e40) when the record of +0xac exists, the text 0x4036 when the record of +0xb0 exists, then frees */

void he_Unk56f0b0(int ev)
{
    char text[0x100];
    int r;

    if (HM_I(ev, 0x70) != 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    r = gm_FindChar(HM_I(ev, 0xac));
    if (r != 0) {
        txt_Format(text, 0x4035);
        he_ShowNews(HM_I(ev, 0xac), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e40);
    }
    if (gm_FindChar(HM_I(ev, 0xb0)) != 0) {
        txt_Format(text, 0x4036, *(unsigned short *)r);
        he_ShowNews(HM_I(ev, 0xb0), -1, text, 1, 0x5fa, 0);
    }
    he_FreeEvent((int *)ev);
}
/* 0x56f170, 1 when a running event of the type 0x99 has the owner +0xb0 == `owner` (the search 0x53e400 / 0x53e300) */

int he_EventOnBuilding(int owner)
{
    int r;

    for (r = (int)he_FindEvent(1, 0, 0x99); r != 0; r = (int)he_NextEvent()) {
        if (HM_I(r, 0xb0) == owner)
            return 1;
    }
    return 0;
}
/* 0x56f1b0, a meeting message when the slot he_Unk56f260 (c + 3) is free and (0x504ec0 (a, 4) - 0x504ec0 (b, 4) * 0.5f) + 0x42ec20 () >= 1.0: 0x4f2e30 (slot, 1, b + 4, a + 4, the id +1 of the record at b + 0x170 / +0x16c, else -1); 0 when not sent */

int he_Unk56f1b0(int a, int b, int c)
{
    float fa, fb;
    int slot = he_Unk56f260(c);
    int id;

    if (slot >= 0) {
        fa = gm_GetSkill(a, 4);
        fb = gm_GetSkill(b, 4);
        if (fa - fb * 0.5f + util_RandomF() >= 1.0f) {
            if (*(int *)(b + 0x170) != 0)
                id = *(int *)(*(int *)(b + 0x170) + 1);
            else if (*(int *)(b + 0x16c) != 0)
                id = *(int *)(*(int *)(b + 0x16c) + 1);
            else
                id = -1;
            gv_Accuse(slot, 1, *(int *)(b + 4), *(int *)(a + 4), id);
            return 1;
        }
    }
    return 0;
}
/* 0x56f260, the first free meeting slot of 0x1d (two rounds from a random start): the record 0x4f2720 (slot) has +0x18 == 1, +1 == 2 and +0xc == the wanted id `c` + 3 (one more each round); -1 when none */

int he_Unk56f260(int c)
{
    char rec[0x24];
    int want = c + 3;
    int round;
    int i;
    int slot;

    round = 2;
    do {
        slot = (unsigned short)util_Random(0x1d);
        for (i = 0x1d; i != 0; i--) {
            gv_GetTableRecord(slot, rec);
            if (*(int *)(rec + 0x18) == 1 && rec[1] == 2 && *(int *)(rec + 0xc) == want)
                return slot;
            slot = (slot + 1) % 0x1d;
        }
        want++;
    } while (--round != 0);
    return -1;
}
/* 0x56f2e0, a message when the person a and the record b (its word at 0 is the target) can meet: b not 0 and (0x504ec0 (a, 2) - 0x504ec0 (b, 2) * 0.5f) + 0x42ec20 () >= 1.0; then 0x4fca40 (*b, (-1 - c) * 5) */

int he_Unk56f2e0(int a, unsigned short *b, int c)
{
    float fa, fb;

    if (b != 0) {
        fa = gm_GetSkill(a, 2);
        fb = gm_GetSkill(b, 2);
        if (fa - fb * 0.5f + util_RandomF() >= 1.0f) {
            ((float (*)(unsigned short, int))gm_AddHealth)(*b, (-1 - c) * 5);
            return 1;
        }
    }
    return 0;
}
/* 0x56f360, 1 when no running event of the type 0x9a has both +0xac == a and +0xb0 == the dword at b + 4; 0 when one does */

int he_NoEvent9aFor(int a, int b)
{
    int r;

    for (r = (int)he_FindEvent(1, 0, 0x9a); r != 0; r = (int)he_NextEvent()) {
        if (HM_I(r, 0xac) == a && HM_I(r, 0xb0) == *(int *)(b + 4))
            return 0;
    }
    return 1;
}
/* 0x56f3b0, 1 when no running event of the type 0x9b has +0xac == the dword at a + 4; 0 when one does */

int he_NoEvent9bFor(int a)
{
    int r;

    for (r = (int)he_FindEvent(1, 0, 0x9b); r != 0; r = (int)he_NextEvent()) {
        if (HM_I(r, 0xac) == *(int *)(a + 4))
            return 0;
    }
    return 1;
}
/* 0x56f3f0, the domicile message of the construction run (a = the building id, b = the record: the word at 0 is one person, the dword at +4 the other): when 0x56f3b0 (b) is free and 0x501190 (1, 1, a) finds, the player word at building +0x27 selects the character record; the checks (0x504ec0 (rec, 2) - 0x504ec0 (b, 2) * 0.5f) + 0x42ec20 () * 0.5f >= 0.8 and the same over the field 3 >= 0.7: both -> the texts 0x4031 / 0x4033 and a message 0x9b (0x4c4b70), else the texts 0x4032 / 0x4034 */

int he_Unk56f3f0(int a, unsigned short *b)
{
    char text[0x400];
    char msg[0xf8];
    int bld, p;
    float f1, f2, f3;

    if (he_NoEvent9bFor((int)b) != 0) {
        bld = gm_BuildQuery(1, 1, a);
        if (bld != 0) {
            p = *(unsigned short *)(bld + 0x27);
            f1 = gm_GetSkill((unsigned)&ai_Chars + p * 0x21c, 2);
            f2 = gm_GetSkill(b, 2);
            f3 = util_RandomF();
            if (f1 - f2 * 0.5f + f3 * 0.5 >= 0.8) {
                f1 = gm_GetSkill((unsigned)&ai_Chars + p * 0x21c, 3);
                f2 = gm_GetSkill(b, 3);
                f3 = util_RandomF();
                if (f1 - f2 * 0.5f + f3 * 0.5 >= 0.7) {
                    txt_Format(text, 0x4031, *b);
                    he_ShowNews(*(int *)((unsigned)&ai_CharInts + p * 0x21c), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_628e28);
                    txt_Format(text, 0x4033);
                    he_ShowNews(*(int *)(b + 2), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e10);
                    memset(msg, 0, 0xf8);
                    msg[4] = 0x9b;
                    HM_I(msg, 0xc) = a;
                    msg[0x36] = 2;
                    HM_I(msg, 8) = *(int *)((unsigned)&ai_CharInts + p * 0x21c);
                    HM_I(msg, 0x58) = *(int *)(b + 2);
                    HM_I(msg, 0x5c) = *(int *)((unsigned)&ai_CharInts + p * 0x21c);
                    cm_SendLongMessage(msg);
                    return 1;
                }
            }
            txt_Format(text, 0x4032, *b, *b);
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + p * 0x21c), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_628df8);
            txt_Format(text, 0x4034);
            he_ShowNews(*(int *)(b + 2), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e10);
        }
    }
    return 0;
}
/* 0x56f600, the running event 0x18 (of the person a: its +4) not owned by c (its +4) whose building (0x4fea50 of +0xb4) is nearest to the place +0x4c .. of the workplace (+0x184 -> +0x34) of b; 0 when none */

int he_Unk56f600(int a, int b, int c)
{
    float best = 1.0e7f;
    float x, y, z, dx, dy, dz, d;
    int job, found;
    char *ev;

    if (HM_I(b, 0x184) == 0)
        return 0;
    job = HM_I(HM_I(b, 0x184), 0x34);
    x = *(float *)(job + 0x4c);
    y = *(float *)(job + 0x50);
    z = *(float *)(job + 0x54);
    he_FindEvent(1, 0, 0x18);
    found = 0;
    for (ev = he_FindEvent(1, 0, 0x18); ev != 0; ev = he_NextEvent()) {
        if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2 || c == HM_I(ev, 4))
            continue;
        if (HM_I(ev, 0xc4) != HM_I(a, 4))
            continue;
        gm_FindById(&b, 0, 0, HM_I(ev, 0xb4));
        if (b == 0)
            continue;
        job = HM_I(b, 0x61);
        dx = *(float *)(job + 0x4c) - x;
        dy = *(float *)(job + 0x50) - y;
        dz = *(float *)(job + 0x54) - z;
        d = sqrt(dx * dx + dy * dy + dz * dz);
        if (found == 0 || d < best) {
            found = (int)ev;
            best = d;
        }
    }
    return found;
}
/* 0x56f740, the domicile action of the construction run (a = the character, b = the event, c = the event 0x18 found for b + 0xc4): only the type 0x18 and fewer than 0x14 tries; b + 0xc4 + 1; when the workplace of 0x505df0 (c + 0xd8) is at least 1850 from the one of a, b + 0xac = c + 0xb4 (0x4c5230 / 0x4c52e0) and 0; else the sums 0x504ec0 (the record of the player c + 8, 2) + (c + 0xe8 + 1) * 0.5f against 0x504ec0 (0x505df0 (c + 0xc4), 2) + (b + 0xc0 + 1) * 0.5f: the first smaller -> the texts 0x4028 / 0x4029, the state -1 (0x4c4c60) and 1; -1 when nothing */

int he_Unk56f740(int a, int b, char *c)
{
    char text[0x400];
    int per, bld1, bld2, other;
    unsigned short pl;
    float dx, dy, dz;

    if (c == 0 || *c != 0x18)
        return -1;
    if (*(unsigned int *)(b + 0xc4) >= 0x14)
        return -1;
    (*(unsigned int *)(b + 0xc4))++;
    per = gm_FindChar(HM_I(c, 0xd8));
    if (per == 0 || HM_I(per, 0x184) == 0)
        return -1;
    bld1 = HM_I(HM_I(a, 0x184), 0x34);
    bld2 = HM_I(HM_I(per, 0x184), 0x34);
    dx = *(float *)(bld1 + 0x4c) - *(float *)(bld2 + 0x4c);
    dy = *(float *)(bld1 + 0x50) - *(float *)(bld2 + 0x50);
    dz = *(float *)(bld1 + 0x54) - *(float *)(bld2 + 0x54);
    if (sqrt(dx * dx + dy * dy + dz * dz) >= 1850.0f) {
        if (HM_I(c, 0xb4) != HM_I(b, 0xac)) {
            HM_I(b, 0xac) = HM_I(c, 0xb4);
            cm_SendChrStop(HM_I(a, 4));
            cm_SendChrGotoBuilding(HM_I(a, 4), HM_I(c, 0xb4), -1, 0, 1, (char *)(unsigned)&full_data_628dec);
        }
        return 0;
    }
    pl = *(unsigned short *)(c + 8);
    other = gm_FindChar(HM_I(c, 0xc4));
    if (gm_GetSkill((unsigned)&ai_Chars + pl * 0x21c, 2) + (HM_I(c, 0xe8) + 1) * 0.5f <
        gm_GetSkill(other, 2) + (HM_I(b, 0xc0) + 1) * 0.5f) {
        HM_I(c, 0x84) = cm_SendExSetHE(-1, c);
        txt_Format(text, 0x4028, *(unsigned short *)other);
        he_ShowNews(*(int *)((unsigned)&ai_CharInts + pl * 0x21c), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e58);
        txt_Format(text, 0x4029, *(unsigned short *)((unsigned)&ai_Chars + pl * 0x21c));
        he_ShowNews(HM_I(other, 4), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e58);
        return 1;
    }
    return -1;
}

void he_Unk56fc50(int ev);          /* 0x56fc50 */
void he_Unk56fda0(int ev);          /* 0x56fda0 */
void he_Unk570130(int ev);          /* 0x570130 */
void he_Unk5702e0(int ev);          /* 0x5702e0 */
void he_Unk570ef0(int ev);          /* 0x570ef0 */
void he_Unk571000(int ev);          /* 0x571000 */
void he_Unk571550(int ev);          /* 0x571550 */
void he_Unk571640(int ev);          /* 0x571640 */
void he_Unk5706e0(int ev);          /* 0x5706e0 */
void he_Unk5707a0(int ev);          /* 0x5707a0 */
void he_Unk572860(int ev);          /* 0x572860 */
void he_Unk570cc0(int ev);          /* 0x570cc0 */
void he_Unk571990(int ev);          /* 0x571990 */
void he_Unk5719e0(int ev);          /* 0x5719e0 */
void he_Unk571ac0(int ev);          /* 0x571ac0 */
void he_Unk571c30(int ev);          /* 0x571c30 */
void he_Unk571f60(int ev);          /* 0x571f60 */
void he_Unk5725b0(int ev);          /* 0x5725b0 */
void he_Unk5726a0(int ev);          /* 0x5726a0 */
void he_Unk5726b0(int ev);          /* 0x5726b0 */
void he_Unk572b90(int ev);          /* 0x572b90 */
void he_Unk572bf0(int ev);          /* 0x572bf0 */
void he_Unk572fd0(int ev);          /* 0x572fd0 */
void he_Unk572a50(int ev);          /* 0x572a50 */

/* 0x56f980, registers the event types 0x88 .. 0x94 (called by 0x53d4b0): (0x88, 0x56fc50, 0x56fda0), (0x89, 0x570130, 0x5702e0), (0x8c, 0x570ef0, 0x571000), (0x8d, 0x571550, 0x571640), (0x8a, 0x5706e0, 0x5707a0), (0x8b, 0x572860, 0x570cc0), (0x8d again), (0x8e, 0x571990, 0x5719e0), (0x8f, 0x571ac0, 0x571c30), (0x90, 0x571f60, 0x5725b0), (0x91, 0x5726a0, 0x5726b0), (0x93, 0x572b90, 0x572bf0), (0x94, 0x55e3a0, 0x572fd0), (0x92, 0x572860, 0x572a50), then the texts of 0x52b350; 0 = ok, 1 = a registration failed */
int he_RegisterEventTypes88(void)
{
    if (he_SetEventHandlers(0x88, (void *)he_Unk56fc50, (void *)he_Unk56fda0) == 0) {
        if (he_SetEventHandlers(0x89, (void *)he_Unk570130, (void *)he_Unk5702e0) == 0) {
            if (he_SetEventHandlers(0x8c, (void *)he_Unk570ef0, (void *)he_Unk571000) == 0) {
                if (he_SetEventHandlers(0x8d, (void *)he_Unk571550, (void *)he_Unk571640) == 0) {
                    if (he_SetEventHandlers(0x8a, (void *)he_Unk5706e0, (void *)he_Unk5707a0) == 0) {
                        if (he_SetEventHandlers(0x8b, (void *)he_Unk572860, (void *)he_Unk570cc0) == 0) {
                            if (he_SetEventHandlers(0x8d, (void *)he_Unk571550, (void *)he_Unk571640) == 0) {
                                if (he_SetEventHandlers(0x8e, (void *)he_Unk571990, (void *)he_Unk5719e0) == 0) {
                                    if (he_SetEventHandlers(0x8f, (void *)he_Unk571ac0, (void *)he_Unk571c30) == 0) {
                                        if (he_SetEventHandlers(0x90, (void *)he_Unk571f60, (void *)he_Unk5725b0) == 0) {
                                            if (he_SetEventHandlers(0x91, (void *)he_Unk5726a0, (void *)he_Unk5726b0) == 0) {
                                                if (he_SetEventHandlers(0x93, (void *)he_Unk572b90, (void *)he_Unk572bf0) == 0) {
                                                    if (he_SetEventHandlers(0x94, (void *)he_Unk55e3a0, (void *)he_Unk572fd0) == 0) {
                                                        if (he_SetEventHandlers(0x92, (void *)he_Unk572860, (void *)he_Unk572a50) == 0) {
                                                            sv_RegisterActivity(0x8d, (char *)(unsigned)&full_data_628e8c, 1, 0x513, -1);
                                                            sv_RegisterActivity(0x8c, (char *)(unsigned)&full_data_628e7c, 1, 0x500, 0xac);
                                                            sv_RegisterActivity(0x8f, (char *)(unsigned)&full_data_628e70, 0, 0x512, -1);
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
/* 0x56fbc0, the progress value of a building record (rec + 0x18 .. the value +0x1c, the steps +0x14, the start +0x20): num not above +0x20 -> 0x4c4ac0 (rec + 4, 0x1c, rec + 0x18 - rec + 0x1c); else (num - +0x20) steps of v = v - step + step / v from v = num - +0x20, then 0x4c4ac0 (rec + 4, 0x1c, v - rec + 0x1c) */

void he_SetLifeByAge(int rec, unsigned short num)
{
    float f, v, fnum;
    int n;

    if (rec == 0)
        return;
    fnum = (float)num;
    if (fnum <= *(float *)(rec + 0x20)) {
        ((int (*)(int, int, float))cm_SendFloatDelta)(*(int *)(rec + 4), 0x1c, *(float *)(rec + 0x18) - *(float *)(rec + 0x1c));
        return;
    }
    f = *(float *)(rec + 0x14);
    v = fnum - *(float *)(rec + 0x20);
    for (n = (int)v; n > 0; n--)
        v = v - f + f / v;
    ((int (*)(int, int, float))cm_SendFloatDelta)(*(int *)(rec + 4), 0x1c, v - *(float *)(rec + 0x1c));
}
/* 0x56fc50, the alloc of the event 0x88 (a work at the building +0xb0 of the player +8): paused -> nothing; with a running event 0x88 of the same player and building the text 0x4096 and state -1; else the due clock now + 1 hour and state 0 (0x4c4c60), or -1 when +0xb4 is -1, and the message 0x50cc50 (pid, -(+0xb8)) */

void he_Unk56fc50(int ev)
{
    char text[0x400];
    int r;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    for (r = (int)he_FindEvent(1, 0, 0x88); r != 0; r = (int)he_NextEvent()) {
        if (r != ev && *(unsigned short *)(r + 8) == *(unsigned short *)(ev + 8) && HM_I(r, 0xb0) == HM_I(ev, 0xb0))
            break;
    }
    if (r != 0) {
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        txt_Format(text, 0x4096, HM_I(ev, 0xb0));
        he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fa, 0);
        return;
    }
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    if (HM_I(ev, 0xb4) == -1)
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
    else {
        HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
        gm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -HM_I(ev, 0xb8));
    }
}

void ai_Unk5a9910(unsigned short *param_1, int param_2, unsigned int param_3);                 /* 0x5a9910 */

/* 0x56fda0, the run of the event 0x88 (a work at the building +0xb0 for the character +0xb4): -1 / -2 release the worker (0x524260) and free; paused or with the handle +0x84 still active nothing. State 0: the owner -1 (0x4c3ae0), the worker registered, the building of +0xac (0x501190) -> its +1 to 0x4c52e0 (state 2), else the state -1. State 2: +0xbc = -1; the job +0x128 of the character or the building missing -> wait 4 hours and state 2; else 0x4fe140 (+0x18) and 0x5a9910 (the record of the player +8, the worker, the due clock), state 3. State 3: the boss +0x16c -> 0x4c52e0 (state 4), else state -1. State 4: no boss -> state -1; work in progress -> wait 4 hours; else 0x524260, state 5 and the due clock += (max (+0xb8 / 2, 1) + 1). State 5: state -1 (0x4c4c60). ROUGH */
void he_Unk56fda0(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0xb4));
    int state, r;

    if (per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    state = HM_I(ev, 0x70);
    if (state == -1 || state == -2) {
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    if (HM_I(ev, 0x84) != -1 && cm_GetSlotState(HM_I(ev, 0x84)) == 0)
        return;
    HM_I(ev, 0x84) = -1;
    switch (state) {
    case 0:
        if (cm_GetSlotState(-1) == 0)
            return;
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        r = gm_BuildQuery(1, 1, HM_I(ev, 0xac));
        if (r == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        HM_I(ev, 0xbc) = cm_SendChrGotoBuilding(HM_I(per, 4), *(int *)(r + 1), -1, 0, 1, (char *)(unsigned)&full_data_628e98);
        HM_I(ev, 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 1:
        return;
    case 2:
        HM_I(ev, 0xbc) = -1;
        if (HM_I(HM_I(per, 0x184), 0x128) != 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            HM_I(ev, 0x84) = cm_SendExSetHE(2, ev);
            return;
        }
        if (gm_BuildQuery(1, 1, HM_I(ev, 0xac)) != 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            r = gm_FindChar(HM_I(ev, 0xb0));
            if (r != 0) {
                gm_AddTime(ev + 0x52, 0x18, 0, 0);
                ai_Unk5a9910((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, r, ev + 0x52);
            }
            HM_I(ev, 0x84) = cm_SendExSetHE(3, ev);
            return;
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 3:
        if (HM_I(per, 0x16c) == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        HM_I(ev, 0xbc) = cm_SendChrGotoBuilding(HM_I(per, 4), *(int *)(HM_I(per, 0x16c) + 1), -1, 0, 0, (char *)(unsigned)&full_data_628e98);
        HM_I(ev, 0x84) = cm_SendExSetHE(4, ev);
        return;
    case 4:
        if (HM_I(per, 0x16c) == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (HM_I(HM_I(per, 0x184), 0x128) != 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            return;
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        HM_I(ev, 0x84) = cm_SendExSetHE(5, ev);
        r = HM_I(ev, 0xb8) >> 1;
        if (r < 1)
            r = 1;
        HM_I(ev, 0x52) += r + 1;
        return;
    case 5:
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
}
/* 0x570130, the alloc of the event 0x89 (a work at +0xb0 by the character +0xb4): paused -> nothing; with a running event 0x89 of the same player and building the text 0x40a2, state -1 and free; else the character +0xb4 missing -> free; the due clock now + 1 hour, 0x50cc50 (pid, -(+0xb8)), state 0 and 0x4f2e30 (0x1b, 1, pid, +4 of the character, the id +1 of +0x170 or -1) */

void he_Unk570130(int ev)
{
    char text[0x400];
    char *per;
    int r, id;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xb0));
    for (r = (int)he_FindEvent(1, 0, 0x89); r != 0; r = (int)he_NextEvent()) {
        if (r != ev && *(unsigned short *)(r + 8) == *(unsigned short *)(ev + 8) && HM_I(r, 0xb0) == HM_I(ev, 0xb0))
            break;
    }
    if (r != 0) {
        if (per != 0) {
            txt_Format(text, 0x40a2, *(unsigned short *)per);
            he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fa, 0);
        }
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_FindChar(HM_I(ev, 0xb4)) == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    HM_I(ev, 0xbc) = 0;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    gm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -HM_I(ev, 0xb8));
    HM_I(ev, 0x70) = 0;
    if (per != 0 && HM_I(per, 0x170) != 0)
        id = *(int *)(HM_I(per, 0x170) + 1);
    else
        id = -1;
    if (per != 0)
        r = HM_I(per, 4);
    else
        r = -1;
    gv_Accuse(0x1b, 1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), r, id);
}

void ai_TellGraveWorkSuccess(int param_1);                 /* 0x5a9990 */
void ai_TellGraveWorkFailure(int param_1);                 /* 0x5a99e0 */
void ai_GraveyardMessage(int param_1, unsigned short *param_2, unsigned short *param_3, int param_4);                 /* 0x5a9a30 */
void ai_GraveyardMessagePlain(int param_1, unsigned short *param_2, unsigned short *param_3);                 /* 0x5a9af0 */

/* 0x5702e0, the run of the event 0x89 (the state machine of the work at the building +0xac by the character +0xb4, the second character +0xb0): -1 / -2 release the character and free; states 0 .. 4 as in the code (0x524260, 0x4c52e0 with the text 0x628ea4, the progress 0x4fe140, 0x504e20 / 0x5a9990 / 0x5a99e0, 0x5a9a30 / 0x5a9af0); ROUGH */
void he_Unk5702e0(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0xb4));
    int state = HM_I(ev, 0x70);
    int r, b2;
    float f;
    unsigned char *job;

    if (state == -1 || state == -2) {
        if (per != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)per, 0);
        he_FreeEvent((int *)ev);
        return;
    }
    if (per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (state) {
    case 0:
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        HM_I(ev, 0x84) = -1;
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 1:
        r = gm_BuildQuery(1, 1, HM_I(ev, 0xac));
        if (r != 0) {
            cm_SendChrGotoBuilding(HM_I(ev, 0xb4), *(int *)(r + 1), -1, 0, 1, (char *)(unsigned)&full_data_628ea4);
            HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
            return;
        }
        HM_I(ev, 0x70) = -1;
        return;
    case 2:
        gm_BuildQuery(1, 1, HM_I(ev, 0xac));
        r = gm_FindChar(HM_I(ev, 0xb4));
        if (r == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        if (HM_I(HM_I(r, 0x184), 0x128) == 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
            b2 = gm_FindChar(HM_I(ev, 0xb0));
            r = gm_FindChar(HM_I(ev, 0xb4));
            f = gm_Mix(r, b2, 2);
            if (b2 != 0) {
                if (1.0 < util_RandomF() + f) {
                    job = *(unsigned char **)(r + 0x16c);
                    HM_I(ev, 0xbc) = 1;
                    he_SetLifeByAge(b2, (char)job[0x5c] / 0x32 + (util_Random(3) & 0xffff) + *(unsigned char *)((unsigned)&ai_BTypes + *job * 0x24d + 0x247) * 2);
                    ai_TellGraveWorkSuccess(b2);
                    HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
                    return;
                }
                ai_TellGraveWorkFailure(b2);
                HM_I(ev, 0xbc) = 0;
            }
            HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
            return;
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 4, 0);
        return;
    case 3:
        r = gm_FindChar(HM_I(ev, 0xb4));
        if (r != 0 && (r = HM_I(r, 0x16c)) != 0) {
            cm_SendChrGotoBuilding(HM_I(ev, 0xb4), *(int *)(r + 1), -1, 0, 0, (char *)(unsigned)&full_data_628ea4);
            HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
            return;
        }
        HM_I(ev, 0x70) = -1;
        return;
    case 4:
        r = gm_FindChar(HM_I(ev, 0xb4));
        if (r != 0 && HM_I(r, 0x16c) != 0) {
            if (HM_I(HM_I(r, 0x184), 0x128) != 0) {
                *(AITIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0, 4, 0);
                return;
            }
            gm_BuildQuery(1, 1, HM_I(ev, 0xac));
            b2 = gm_FindChar(HM_I(ev, 0xb0));
            r = gm_FindChar(HM_I(ev, 0xb4));
            if (b2 != 0 && r != 0) {
                if (HM_I(ev, 0xbc) != 0) {
                    f = gm_GetSkill(r, 1);
                    if (1.0 < util_RandomF() + (1.0 - f))
                        ai_GraveyardMessage((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, r, b2, 0);
                    else
                        ai_GraveyardMessage((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, r, b2, 1);
                } else
                    ai_GraveyardMessagePlain((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, r, b2);
            }
            HM_I(ev, 0x70) = -1;
            return;
        }
        HM_I(ev, 0x70) = -1;
        return;
    default:
        HM_I(ev, 0x70) = -1;
        return;
    }
}

int ai_PickActionBuildings(struct AIPER *p, int a, __int64 mask, float lo, float hi, unsigned char n, int *out); /* 0x4a6570 */

/* 0x5706e0, the alloc of the event 0x8a: paused -> nothing; 0x4a6570 (the record of the player +8, 5, 4, 0, 0, 0, 5, ev + 0xb4) -> 0 -> free; the slot +0xb4 of the found index (when < 5) cleared, +0xb0 = 0, due clock now + 1 hour, state 0 */
void he_Unk5706e0(int ev)
{
    int r;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    r = ai_PickActionBuildings((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, 5, 4, 0, 0, 5, ev + 0xb4);
    if (r == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (r < 5)
        HM_I(ev, 0xb4 + r * 4) = 0;
    HM_I(ev, 0xb0) = 0;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
}

int ai_PickPartnerRandomWalk(struct AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 */
int gv_ProtectionPrice();                 /* 0x4f24a0 */
int gm_GetWorth(unsigned short id);                 /* 0x50a640 */
int gm_ConvertMoney(int a, unsigned char k); /* 0x5085b0 */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); /* 0x4c4830 */
void cm_SendSetField(int a, int b, int c, int d, int e); /* 0x4c4a60 */
void he_Unk570bf0(int *a, int b);   /* 0x570bf0 */
void he_Unk570c70(int *a, int b);   /* 0x570c70 */

/* 0x5707a0, the run of the event 0x8a (the plunder of the buildings +0xb4 .. by the character +0xac): -1 / -2 or a character without the byte +8 -> 0x524260 and free; states 0 .. 4: 0 registers the character (0x524260) and state -1; 1 starts the work 0x4c52e0 (text 0x628eb8) on the building +0xb4 (+0xb0 index); 2 looks the character up, waits 0xa hours while its job +0x128 runs, else finds the nearest target 0x4a5d30 (100.0f), compares 0x4f24a0 / 0x50a640 / 0x5085b0, sends the message 0x8b (0x4c4b70) or calls 0x570bf0 / 0x570c70 by 0x42ec00 (2), stores the word into +0xc8 [+0xb0], steps +0xb0 and state; 3 the boss of the character -> 0x4c52e0; 4 waits 4 hours while its job runs; ROUGH */
void he_Unk5707a0(int ev)
{
    char msg[0x70];
    char *per = (char *)gm_FindChar(HM_I(ev, 0xac));
    int state = HM_I(ev, 0x70);
    int r, id, n, best;
    unsigned short idx, v;
    int rec;

    if (state == -1 || state == -2) {
        if (per != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)per, 0);
        he_FreeEvent((int *)ev);
        return;
    }
    if (per == 0 || per[8] == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    switch (state) {
    case 0:
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        HM_I(ev, 0x84) = -1;
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 1:
        r = gm_BuildQuery(1, 1, HM_I(ev, 0xb4 + HM_I(ev, 0xb0) * 4));
        if (r == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        cm_SendChrGotoBuilding(HM_I(ev, 0xac), *(int *)(r + 1), -1, 0, 1, (char *)(unsigned)&full_data_628eb8);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 2:
        r = gm_FindChar(HM_I(ev, 0xac));
        if (r == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        if (HM_I(HM_I(r, 0x184), 0x128) != 0) {
            *(AITIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 0xa, 0);
            return;
        }
        if (ai_PickPartnerRandomWalk((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, ((unsigned)&he_Events + 0x58c8), 4, ev + 0xc8, 0, 100.0f, (int *)&idx) == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        id = idx;
        best = gv_ProtectionPrice((unsigned)&ai_Chars + id * 0x21c);
        n = ((int (*)(int))gm_ConvertMoney)(gm_GetWorth(id));
        *(unsigned short *)(ev + 0xc8 + HM_I(ev, 0xb0) * 2) = idx;
        if (n > best) {
            if (HM_B((unsigned)&ai_Chars + id * 0x21c, 2) == 6 || HM_B((unsigned)&ai_Chars + id * 0x21c, 2) == 7) {
                HM_B(msg, 4) = 0x8b;
                HM_I(msg, 8) = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
                HM_I(msg, 0xc) = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
                *(AITIME *)(msg + 0x28) = ai_Time;
                HM_B(msg, 0x3a) = 9;
                HM_I(msg, 0x5c) = HM_I(ev, 0xc);
                HM_I(msg, 0x60) = HM_I(ev, 0xac);
                HM_I(msg, 0x68) = best;
                cm_SendLongMessage(msg);
            } else {
                rec = HM_I(ev, 0xc);
                msg[0] = HM_I(ev, 0xc);
                msg[4] = HM_I(ev, 0xac);
                msg[8] = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
                msg[0xc] = best;
                if (util_Random(2) != 0)
                    he_Unk570bf0((int *)msg, (unsigned)&ai_Chars + id * 0x21c);
                else
                    he_Unk570c70((int *)msg, (unsigned)&ai_Chars + id * 0x21c);
            }
        }
        HM_I(ev, 0xb0) = HM_I(ev, 0xb0) + 1;
        if (HM_I(ev, 0xb0) < 5 && HM_I(ev, 0xb4 + HM_I(ev, 0xb0) * 4) != 0) {
            HM_I(ev, 0x70) = HM_I(ev, 0x70) - 1;
            return;
        }
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 3:
        r = gm_FindChar(HM_I(ev, 0xac));
        if (r == 0 || HM_I(r, 0x16c) == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        cm_SendChrGotoBuilding(HM_I(ev, 0xac), *(int *)(HM_I(r, 0x16c) + 1), -1, 0, 0, (char *)(unsigned)&full_data_628eb8);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 4:
        r = gm_FindChar(HM_I(ev, 0xac));
        if (r == 0 || HM_I(r, 0x16c) == 0 || HM_I(HM_I(r, 0x184), 0x128) == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 4, 0);
        return;
    }
}
/* 0x570bf0, a plunder message: 0x4c4830 (a[0], b + 4, the byte 0x6f1bc5, a[3]), 0x4c4a60 (b + 4, 0x1cc, 4, 1, 0), the text 0x40fe to a[0] (flag 1, 0x5f4) */

void he_Unk570bf0(int *a, int b)
{
    char text[0x400];

    ((int (*)(int, int, unsigned char, int))cm_SendMoveGoodsUnchecked)(a[0], *(int *)(b + 4), *(unsigned char *)(unsigned)&ai_OwnPlayer, a[3]);
    cm_SendSetField(*(int *)(b + 4), 0x1cc, 4, 1, 0);
    txt_Format(text, 0x40fe, *(unsigned short *)b, a[3]);
    he_ShowNews(a[0], -1, text, 1, 0x5f4, 0);
}
/* 0x570c70, the text 0x40ff (the word of b) to a[0] (flag 1, 0x5f4) */

void he_Unk570c70(int *a, int b)
{
    char text[0x400];

    txt_Format(text, 0x40ff, *(unsigned short *)b);
    he_ShowNews(a[0], -1, text, 1, 0x5f4, 0);
}

int he_OpenEventSlot();                 /* 0x53d6f0 */

/* 0x570cc0, the run of the event 0x8b (a fight / attack of the character +0xac on the player +8): a missing character or a player without the bit 1 of +0x770d4c -> state -2; -2 releases the window +0x74 (0x53d800) and frees; state 0 opens the window 0x53d6f0 (0x6269c0, 0x5ec), 0x475600 / 0x475f70, the texts 0x602568 and 0x40fd and 0x4c27d0 (-7, .., 0x628ecc, -1, 0), state 1; state 1: due (0x4fe210) -> text 0x40ff (0x570c70), 0x53d800, free; the window missing -> state 0; the actor 0xf8aac4 != window +8 or 0xf8a668 -1 -> nothing; 0xf8a668 0 -> 0x570bf0; 1 -> 0x570c70, then 0x53d800 and free. ROUGH */
void he_Unk570cc0(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0xac));
    int state;

    if (per == 0 || (HM_B(((unsigned)&ai_Selected + 0x44) + *(unsigned short *)(ev + 8) * 0x21c, 0) & 1) == 0)
        HM_I(ev, 0x70) = -2;
    state = HM_I(ev, 0x70);
    if (state == -2) {
        if (HM_I(ev, 0x74) != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (state == 0) {
        if (HM_I(ev, 0x74) == 0)
            he_OpenEventSlot(ev, 0, 0, "Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        if (HM_I(ev, 0x74) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(HM_I(HM_I(ev, 0x74), 8));
        d2_SetForm(HM_I(HM_I(ev, 0x74), 8), 0);
        io_PrintText((unsigned)"$C");
        io_PrintText(0x40fd, *(unsigned short *)per, HM_I(ev, 0xb8));
        snd_QueueVoice(-7, 0, (char *)(unsigned)&full_data_628ecc, -1, 0);
        HM_I(ev, 0x70) = 1;
        return;
    }
    if (state != 1)
        return;
    if (gm_CompareTime(&ai_Time, ev + 0x52) > 0) {
        he_Unk570c70((int *)(ev + 0xac), (unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (HM_I(ev, 0x74) == 0) {
        HM_I(ev, 0x70) = 0;
        return;
    }
    if (*(int *)(unsigned)&d2_ClickedForm != HM_I(HM_I(ev, 0x74), 8))
        return;
    state = *(int *)(unsigned)&d2_ClickedId;
    if (state == -1)
        return;
    if (state == 0) {
        he_Unk570bf0((int *)(ev + 0xac), (unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
        he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (state != 1)
        return;
    he_Unk570c70((int *)(ev + 0xac), (unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    he_ReleaseSlot(ev);
    he_FreeEvent((int *)ev);
}

int ai_PickPartnerRandomWalk(struct AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 */
int gv_ProtectionPrice();                 /* 0x4f24a0 */
int gm_GetWorth(unsigned short id);                 /* 0x50a640 */
int gm_ConvertMoney(int a, unsigned char k); /* 0x5085b0 */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); /* 0x4c4830 */
void cm_SendSetField(int a, int b, int c, int d, int e); /* 0x4c4a60 */
void he_Unk570bf0(int *a, int b);   /* 0x570bf0 */
void he_Unk570c70(int *a, int b);   /* 0x570c70 */
int he_OpenEventSlot();                 /* 0x53d6f0 */
int gm_ClockSeason();                 /* 0x4fe350 */
int gm_BuildingSkill();                 /* 0x5092e0 */
int sv_UsedGroupSlots();                 /* 0x5241b0 */
int gm_FreeCapacity();                 /* 0x50ab80 */
char *gm_FindBuildingById(int id); /* 0x502020 */
int gv_ListEntry(char *r, int i, unsigned char n);                 /* 0x4ef570 */
int gv_OrderAdjustEntry();                 /* 0x4f0c20 */
int gm_CharCurrency(char *c, char p);                 /* 0x509ee0 */
void ai_Unk5a9910(unsigned short *param_1, int param_2, unsigned int param_3);                 /* 0x5a9910 */

/* 0x570ea0, 1 when no running event 0x88 has the player +8 == b + 4 and the building +0xb0 == a + 4; 0 when one does */
int he_NoEvent88Pair(int a, int b)
{
    int r;

    for (r = (int)he_FindEvent(1, 0, 0x88); r != 0; r = (int)he_NextEvent()) {
        if (*(unsigned int *)(b + 4) == *(unsigned short *)(r + 8) && HM_I(r, 0xb0) == HM_I(a, 4))
            return 0;
    }
    return 1;
}
/* 0x570ef0, the alloc of the event 0x8c: the building 0x501190 (1, 1, ev + 0x10), the group 0x4ffe90 (building +0x5d, 2, 0, 0xfd, 6); each of the four characters +0x8c .. registered (0x524260); none -> free; else +0xb0 = 0, the start clock +0xb8 = now, +0xb4 = 0x168, the due clock now + 5 hours, state 0 */

void he_Unk570ef0(int ev)
{
    int bld = gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    int grp = gm_Query(HM_I(bld, 0x5d), 2, 0, 0xfd, 6);
    int i, n = 0, r;

    for (i = 0; i < 4; i++) {
        r = gm_FindChar(HM_I(ev, 0x8c + i * 4));
        if (r != 0) {
            sv_MoveToGroup(bld, grp, *(unsigned short *)r, ev);
            n++;
        }
    }
    if (n == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    HM_I(ev, 0xb0) = 0;
    *(AITIME *)(ev + 0xb8) = ai_Time;
    HM_I(ev, 0xb4) = 0x168;
    *(AITIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
/* 0x571000, the run of the event 0x8c (a work on the building +0x10 by up to four characters +0x8c ..): outside the night window (the hour 0x4fe350 against the minute curves 0x617840 / 0x617830) a state -1 / -2 works; the player +8 0xffff or +0xc -1 releases all and frees. The four characters counted (cnt; alive without the kinds 0x1b / 0x1c with +0x1b4 bit 0), none -> state -1; the building 0x501190 (1, 1, +0x10) and the work group 0x4ffe90 (+0x5d, 2, 0, 0x2a, 6), missing -> state -1. State 0: the elapsed 0x4fe280 (+0xb8) and the value 0x5092e0 (group, 0x12 / 0x1e, 1) * 0x5eb7f0 (minus 0.5 for kind 3 without the +0x5b bit 0x20), (that + 1.0f) * elapsed * 0x5eec50 added alive times to +0xb0; +0xb4 -= alive; the start clock +0xb8 = now; due now + 5 hours; +0xb4 < 0 -> state -1. With a state -1 / -2: n = (int)+0xb0 (at least 1), -1 when 0x5241b0 (ev) <= 0 (the -1 state only); with a group: 0x50a810, 0x50ab80, 0x4c4880 and the texts 0x421e / 0x4222 / 0x4128 / 0x412c, 0x4223 / 0x4224 / 0x412d / 0x412e and 0x628f04 / 0x628ee8, the message 0x53dee0. Finally the four released (0x524260) and freed. ROUGH: the text building around the 0x53dee0 calls is a skeleton */

void he_Unk571000(int ev)
{
    char text[0x200];
    char name[0x200];
    int hour, minute, i, n, cnt = 0, alive = 0, e;
    float x;
    char *per, *bld, *grp, *r;

    hour = gm_ClockSeason(&ai_Time);
    minute = *(unsigned short *)((char *)&ai_Time + 4);
    if ((float)minute < *(float *)((unsigned)&gm_DayEndHours + hour * 4) || (float)minute < *(float *)((unsigned)&gm_DayStartHours + hour * 4)) {
        if (HM_I(ev, 0x70) != -1 && HM_I(ev, 0x70) != -2) {
            gm_AddTime(ev + 0x52, 0, 5, 0);
            return;
        }
    }
    if (*(unsigned short *)(ev + 8) == 0xffff || HM_I(ev, 0xc) == -1)
        goto free_all;
    for (i = 0; i < 4; i++) {
        r = (char *)gm_FindChar(HM_I(ev, 0x8c + i * 4));
        if (r != 0) {
            cnt++;
            alive++;
            if ((HM_B(r, 0x165) == 0x1b || HM_B(r, 0x165) == 0x1c) && (HM_B(r, 0x1b4) & 1))
                alive--;
        }
    }
    if (cnt == 0)
        HM_I(ev, 0x70) = -1;
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    grp = 0;
    if (bld != 0) {
        grp = (char *)gm_Query(HM_I(bld, 0x5d), 2, 0, 0x2a, 6);
        if (grp == 0)
            HM_I(ev, 0x70) = -1;
    } else
        HM_I(ev, 0x70) = -1;
    if (HM_I(ev, 0x70) == 0) {
        e = gm_MinutesBetween(ev + 0xb8, &ai_Time);
        if (HM_B(bld, 0) == 0x1d)
            x = (float)gm_BuildingSkill(grp, 0x12, 1) * (0.00999999978f);
        else {
            x = (float)gm_BuildingSkill(grp, 0x1e, 1) * (0.00999999978f);
            if (e == 3 && !(HM_B(grp, 0x5b) & 0x20))
                x -= 0.5f;
        }
        x = (x + 1.0f) * e * 0.01667f;
        for (i = alive; i > 0; i--)
            *(float *)(ev + 0xb0) += x;
        HM_I(ev, 0xb4) -= alive;
        *(AITIME *)(ev + 0xb8) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        if (HM_I(ev, 0xb4) < 0)
            HM_I(ev, 0x70) = -1;
    }
    n = HM_I(ev, 0x70);
    if (n != -1 && n != -2)
        return;
    n = (int)*(float *)(ev + 0xb0);
    if (n <= 1)
        n = 1;
    if (HM_I(ev, 0x70) == -1 && sv_UsedGroupSlots(ev) <= 0)
        n = -1;
    if (cnt != 0 && grp != 0 && n > 0) {
        per = (char *)gm_FindCharacterByMaster(grp);
        r = (char *)gm_FreeCapacity(grp, *(unsigned short *)(ev + 0xac), n);
        if (r != 0) {
            cm_SendExSellObject(*(int *)(grp + 2), -1, *(unsigned short *)(ev + 0xac), r, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            if (per[0] == 0x1f) {
                txt_Format(text, 0x421e, 0x4222, 0x22c);
                txt_Format(name, 0x4223, 0x4224, r, HM_I(ev, 0xac) * 2 + 0x9a8);
            } else {
                txt_Format(text, 0x4128, 0x412c, 0x228);
                txt_Format(name, 0x412d, 0x412e, r, HM_I(ev, 0xac) * 2 + 0x9a8);
            }
            if (HM_B(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c, 0) == 6 || HM_B(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c, 0) == 7) {
                if (per == 0 || !(HM_B(per, 0x1b4) & 1))
                    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, -1, HM_I(ev, 0xac), 0x5ec, name, 0);
            }
        }
    }
free_all:
    for (i = 0; i < 4; i++) {
        r = (char *)gm_FindChar(HM_I(ev, 0x8c + i * 4));
        if (r != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)r, 0);
    }
    he_FreeEvent((int *)ev);
}
/* 0x571550, the alloc of the event 0x8d: the character +0x8c or the building 0x502020 (+0x10) missing -> free; the group 0x4ffe90 (building +0x5d, 1, 0, 0xfd), the character registered (0x524260), +0xac = 0, +0xb8 = 0x1fe, the start clock +0xbc and the due clock +0x52 = now + 5 hours, state 0 */

void he_Unk571550(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0x8c));
    char *bld;
    int grp;

    if (per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    bld = (char *)gm_FindBuildingById(HM_I(ev, 0x10));
    if (bld == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    grp = gm_Query(HM_I(bld, 0x5d), 1, 0, 0xfd);
    sv_MoveToGroup(bld, grp, *(unsigned short *)per, ev);
    HM_I(ev, 0xac) = 0;
    HM_I(ev, 0xb8) = 0x1fe;
    *(AITIME *)(ev + 0xbc) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, 0, 5, 0);
}
/* 0x571640, the run of the event 0x8d (a work on the building +0x10 by the character +0x8c): outside the night window (the hour 0x4fe350 against the minute curves 0x617840 / 0x617830) a state -1 / -2 works; the player +8 0xffff or +0xc -1 frees. The character 0x505df0 (+0x8c) missing -> state -1, else registered (0x524260); the building 0x501190 (1, 1, +0x10) missing -> state -1; the work 0x4ef570 (building +0x65, +0xb0, +0xb4) missing -> state -1. State 0: the elapsed 0x4fe280 (+0xbc), the value (4 - x) * y * 0x5eb91c (times 0x5eb79c for the kinds 0x1b / 0x1c with +0x1b4 bit 0) added to +0xac, due now + 5 hours, +0xb8 -= elapsed, the start clock +0xbc = now, +0xb8 <= 0 -> state -1; n = (int)+0xac + HM_B (rec, 0xc), n >= 0xff -> state -1. With a state -1 / -2: n = (int)+0xac, 0x4f0c20 (the character, +0xb0, +0xb4, n), the text 0x411e / 0x4126 and the message 0x53dee0 (when the player is kind 6 / 7 and the character no +0x1b4 bit 0), the release 0x524260, the free. ROUGH */

void he_Unk571640(int ev)
{
    char text[0x200];
    char name[0x200];
    int hour, minute, n, e;
    float x;
    char *per, *bld, *r;

    hour = gm_ClockSeason(&ai_Time);
    minute = *(unsigned short *)((char *)&ai_Time + 4);
    if ((float)minute < *(float *)((unsigned)&gm_DayEndHours + hour * 4) || (float)minute < *(float *)((unsigned)&gm_DayStartHours + hour * 4)) {
        if (HM_I(ev, 0x70) != -1 && HM_I(ev, 0x70) != -2) {
            *(AITIME *)(ev + 0xbc) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            return;
        }
    }
    if (*(unsigned short *)(ev + 8) == 0xffff || HM_I(ev, 0xc) == -1)
        goto free_one;
    per = (char *)gm_FindChar(HM_I(ev, 0x8c));
    if (per == 0)
        HM_I(ev, 0x70) = -1;
    else
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld == 0)
        HM_I(ev, 0x70) = -1;
    r = (char *)gv_ListEntry(HM_I(bld, 0x65), HM_I(ev, 0xb0), HM_B(ev, 0xb4));
    if (r == 0)
        HM_I(ev, 0x70) = -1;
    if (HM_I(ev, 0x70) == 0) {
        e = gm_MinutesBetween(ev + 0xbc, &ai_Time);
        x = (float)(4 - HM_I(ev, 0xb0)) * (float)HM_I(ev, 0xb4) * (0.100000001f);
        if ((HM_B(per, 0x165) == 0x1b || HM_B(per, 0x165) == 0x1c) && (HM_B(per, 0x1b4) & 1))
            x *= 0.25f;
        *(float *)(ev + 0xac) += x;
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        HM_I(ev, 0xb8) -= e;
        *(AITIME *)(ev + 0xbc) = ai_Time;
        if (HM_I(ev, 0xb8) <= 0)
            HM_I(ev, 0x70) = -1;
        n = (int)*(float *)(ev + 0xac) + HM_B(r, 0xc);
        if (n >= 0xff)
            HM_I(ev, 0x70) = -1;
    }
    n = HM_I(ev, 0x70);
    if (n < -2 || n >= 0)
        return;
    n = (int)*(float *)(ev + 0xac);
    if (per != 0 && n > 0)
        gv_OrderAdjustEntry(per, HM_I(ev, 0xb0), HM_B(ev, 0xb4), n);
    if (per != 0) {
        r = (char *)gm_FindCharacterByMaster(per);
        txt_Format(name, 0x411e, 0x4126, *(unsigned short *)per);
        if ((HM_B(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c, 0) == 6 || HM_B(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c, 0) == 7) &&
            (r == 0 || !(HM_B(r, 0x1b4) & 1)))
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, name, 0, -1, *(int *)(per + 1), 0x5ec, text, 0);
    }
    if (per != 0)
        sv_MoveToGroup(0, 0, *(unsigned short *)per, 0);
free_one:
    he_FreeEvent((int *)ev);
}
/* 0x571990, the alloc of the event 0x8e: the due clock +0x52 = now + 0x48 hours, state 0 */

void he_Unk571990(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, 0x48, 0, 0);
}
/* 0x5719e0, the run of the event 0x8e: only the states -2 .. 0; the character +0xc (0x505df0) missing -> free; the cost 0x509ee0 (character, the byte 0x6f1bc5) above +0xac -> 0x4c47e0 (-1, +4 of the character, the byte, +0xac) and free; else each of the five job records +0x68 .. present loses 0x4fca40 (id, -1001), or the character itself when none; free */

void he_Unk5719e0(int ev)
{
    char *per;
    char *r;
    int n, i, found = 0;

    if (HM_I(ev, 0x70) < -2 || HM_I(ev, 0x70) > 0)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per != 0) {
        n = ((int (*)(char *, char))gm_CharCurrency)(per, *(char *)(unsigned)&ai_OwnPlayer);
        if (HM_I(ev, 0xac) < n) {
            cm_SendMoveGoods(-1, *(int *)(per + 4), *(unsigned char *)(unsigned)&ai_OwnPlayer, HM_I(ev, 0xac));
            he_FreeEvent((int *)ev);
            return;
        }
        for (i = 0; i < 5; i++) {
            if (HM_I(per, 0x68 + i * 4) != -1) {
                r = (char *)gm_FindChar(HM_I(per, 0x68 + i * 4));
                if (r != 0) {
                    ((float (*)(unsigned short, int))gm_AddHealth)(*(unsigned short *)r, -1001);
                    found = 1;
                }
            }
        }
        if (!found)
            ((float (*)(unsigned short, int))gm_AddHealth)(*(unsigned short *)per, -1001);
    }
    he_FreeEvent((int *)ev);
}
/* 0x571ac0, the alloc of the event 0x8f: the character +0xac missing, already with an event 0x8f (+0x17c type 0x8f) or the building 0x501190 (1, 1, +0x10) missing -> free; +0xb4 != -1 -> the group 0x4ffe90 (building +0x5d, 2, 6, 1, +0xb4); the due clock +0x52 and the start clock +0x44 = now; the character registered (0x524260) in the group (or alone without one); the second clock +0x60 = now; due now + 0x1e hours (0x4fe140); +0xb8 = the due clock and 0x4fe140 (+0xb8, 3, 0, 0) */

void he_Unk571ac0(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0xac));
    char *bld;
    int grp = 0;

    if (per == 0)
        goto free_ev;
    if (HM_I(per, 0x17c) != 0 && HM_B(HM_I(per, 0x17c), 0) == 0x8f)
        goto free_ev;
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld == 0)
        goto free_ev;
    if (HM_I(ev, 0xb4) != -1)
        grp = gm_Query(HM_I(bld, 0x5d), 2, 6, 1, HM_I(ev, 0xb4));
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0x44) = ai_Time;
    if (grp != 0)
        sv_MoveToGroup(bld, grp, *(unsigned short *)per, ev);
    else
        sv_MoveToGroup(bld, 0, *(unsigned short *)per, ev);
    *(AITIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    *(AITIME *)(ev + 0xb8) = *(AITIME *)(ev + 0x52);
    gm_AddTime(ev + 0xb8, 3, 0, 0);
    return;
free_ev:
    he_FreeEvent((int *)ev);
}
/* 0x571c30, the run of the event 0x8f: only the states -2 .. 0 (below -> nothing, -1 / -2 -> free). State 0: the character +0xac missing, the byte +0x80 [+0xb0] >= 1000.0f (0x5eceb4) or +0xb8 not due (0x4fe210) -> the release part; the building 0x501190 (1, 1, +0x10) missing -> free; the value 0x5092e0 (+0xc) and 0x509400 (+0x60, now, building) -> the byte +0x80 [+0xb0] = a share of it (the constants 0x5eec10, 0x5ee928, 0x5ece60, 0x5eb8a8); 0x4c3ba0 (character, +4), 0x4c3d00 (1, 1, the byte record, &out), 0x4c49a0, the due clock +0x52 and +0x60 = now, due now + 0x1e hours. The release part: the character 0x505df0 (+0xac) and the building 0x501190 (1, 1, +0x10) -> free when missing; the leader 0x50a810 (building) with +0x1b4 bit 0 -> release; the group 0x4ffe90 (+0x5d, 1, 1, +0xb4) or 0x501ce0 (building) -> the text 0x1bc8 (and the strings 0x631090 / 0x6274c8) to the player, the message 0x53dee0; the character released (0x524260) and the event freed. ROUGH */

void he_Unk571c30(int ev)
{
    char text[0x200];
    float f;
    int state = HM_I(ev, 0x70);
    int v, n;
    char *per, *bld, *r;
    int grp;

    if (state < -2)
        return;
    if (state < 0)
        goto free_ev;
    if (state != 0)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xac));
    if (per == 0)
        goto release;
    v = HM_B(per + HM_I(ev, 0xb0), 0x80);
    if ((float)v >= (252.0f))
        goto release;
    if (gm_CompareTime(&ai_Time, ev + 0xb8) >= 0)
        goto release;
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld == 0)
        goto free_ev;
    n = gm_BuildingSkill(bld, 0xc, 1);
    f = (float)n;
    v = gm_WorkMinutesBetween(ev + 0x60, &ai_Time, bld);
    HM_B(per + HM_I(ev, 0xb0), 0x80) = (unsigned char)(int)((double)v * 0.03333333333333333 * 8.0 * ((double)f * *(double *)(unsigned)&ai_Unk5ece60 + *(double *)(unsigned)&full_rdata_5eb8a8));
    cm_BeginFieldBlock(per, *(int *)(per + 4));
    cm_AddFieldValues(1, 1, per + (*(unsigned short *)(ev + 0xb0) - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x80, &n);
    cm_SendFieldDeltas();
    *(AITIME *)(ev + 0x52) = ai_Time;
    *(AITIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    return;
release:
    per = (char *)gm_FindChar(HM_I(ev, 0xac));
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld == 0 || per == 0)
        goto free_ev;
    r = (char *)gm_FindCharacterByMaster(bld);
    if (r != 0 && (HM_B(r, 0x1b4) & 1))
        goto released;
    grp = gm_Query(HM_I(bld, 0x5d), 1, 1, HM_I(ev, 0xb4));
    if (grp == 0) {
        grp = gm_GetGebaeudeHauptraum(bld);
        if (grp == 0)
            goto released;
    }
    txt_Format(text, 0x1bc8, *(unsigned short *)per, bld);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), HM_I(ev, 0x10), text, 0, 0x5f2, *(int *)(bld + 1), *(int *)(grp + 2), (char *)(unsigned)&ch_EmptyName, "_NACHRICHTEN_HS_81");
released:
    sv_MoveToGroup(bld, 0, *(unsigned short *)per, 0);
    he_FreeEvent((int *)ev);
    return;
free_ev:
    he_FreeEvent((int *)ev);
}
/* 0x571f60, the alloc of the event 0x90: paused -> nothing; state 0; the due clock +0x52 = now; 0x53e400 (2, 0, 0x90, 3, +0x10) finds another running event 0x90 of the building -> state -1 (0x4c4c60); else the message 0x91 (+0x28 the clock, +0x36 0xa, +0x58 +4, +0x5c -1, +0x60 +0xb0) through 0x4c4b70, +0xb4 / +0xb0 capped at 2, +0xb8 = 0, 0x50cc50 (pid, -(+0xb4 + 2) * (+0xb0 + 2)), due now + 5 hours, state 0 */

void he_Unk571f60(int ev)
{
    char msg[0xf8];
    char *r;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    HM_I(ev, 0x70) = 0;
    *(AITIME *)(ev + 0x52) = ai_Time;
    for (r = he_FindEvent(2, 0, 0x90, 3, HM_I(ev, 0x10)); r != 0; r = he_NextEvent()) {
        if ((int)r != ev) {
            HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
    }
    memset(msg, 0, 0xf8);
    HM_B(msg, 4) = 0x91;
    HM_I(msg, 8) = HM_I(ev, 0xc);
    HM_I(msg, 0xc) = HM_I(ev, 0xac);
    *(AITIME *)(msg + 0x28) = ai_Time;
    HM_B(msg, 0x36) = 0xa;
    HM_I(msg, 0x58) = HM_I(ev, 4);
    HM_I(msg, 0x5c) = -1;
    HM_I(msg, 0x60) = HM_I(ev, 0xb0);
    cm_SendLongMessage(msg);
    if (HM_I(ev, 0xb4) > 2)
        HM_I(ev, 0xb4) = 2;
    if (HM_I(ev, 0xb0) > 2)
        HM_I(ev, 0xb0) = 2;
    HM_I(ev, 0xb8) = 0;
    gm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -((HM_I(ev, 0xb4) + 2) * (HM_I(ev, 0xb0) + 2)));
    gm_AddTime(ev + 0x52, 0, 5, 0);
    HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
}

int gm_GetDisease();                 /* 0x5051d0 */
int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v);                 /* 0x4c3c00 */

/* 0x5720f0, the skill lottery of the record rec (the field at +0x2c): the chance f must beat 0x42ec20 (), then the kind 2 / 1 / other picks the free bit range a or b (2 / 3, 4 / 5, 6 and 7 / 8 / 9 by 0x42ec00 (2) and the taken bits 0x1800000 / 0x700000); a when b is 0, else b by 0x42ec00 (2); 0 when none. 0x5051d0 (a, buf) gives the max (buf + 8) of 0x42ec00; n = that * f; the field gets (v << shift ^ field) & mask for a; 0x4c3ba0 (rec, +4), 0x4c3c00 (4, 1, 0x2c, &field), 0x4c49a0 (); 0x4fca40 (*rec, -n) into out; a */
int he_Unk5720f0(unsigned short *rec, float f, int kind, float *out)
{
    char buf[12];
    unsigned int field, v, x;
    int a = 0, b = 0, n;
    float res;

    if (f <= util_RandomF())
        return 0;
    if (kind == 2) {
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0xf0) == 0)
            a = 2;
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0xf00) == 0)
            b = 3;
    } else if (kind == 1) {
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0x3000) == 0)
            a = 4;
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0x1c000) == 0)
            b = 5;
    } else {
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0xe0000) == 0)
            a = 6;
        b = 8 - (util_Random(2) != 0);
        if ((*(unsigned int *)((char *)rec + 0x2c) & 0x1800000) != 0 && b == 8)
            b = 0;
        if ((*(unsigned int *)((char *)rec + 0x2c) & ((unsigned)&he_Events + 0x54c0)) != 0 && b == 7)
            b = 0;
    }
    if (a == 0) {
        if (b == 0)
            return 0;
        a = b;
    } else if (b != 0 && util_Random(2) == 0)
        a = b;
    field = *(unsigned int *)((char *)rec + 0x2c);
    if (gm_GetDisease(a, buf) == 0)
        return a;
    v = util_Random(*(int *)(buf + 8)) & 0xffff;
    n = (int)((float)v * f);
    if (v == 0)
        return a;
    switch (a) {
    case 2: v = (v << 4 ^ field) & 0xf0; break;
    case 3: v = (v << 8 ^ field) & 0xf00; break;
    case 4: v = (v << 0xc ^ field) & 0x3000; break;
    case 5: v = (v << 0xe ^ field) & 0x1c000; break;
    case 6: v = (v << 0x11 ^ field) & 0xe0000; break;
    case 7: v = (v << 0x14 ^ field) & ((unsigned)&he_Events + 0x54c0); break;
    case 8: v = (v << 0x17 ^ field) & 0x1800000; break;
    case 9: v = (v << 0x19 ^ field) & 0x1e000000; break;
    default: goto done;
    }
    field ^= v;
done:
    cm_BeginFieldBlock(rec, *(int *)((char *)rec + 4));
    cm_AddFieldDelta(4, 1, 0x2c, &field);
    cm_SendFieldDeltas();
    res = ((float (*)(unsigned short, int))gm_AddHealth)(*rec, -n);
    if (out != 0)
        *out = res;
    return a;
}
/* 0x572310, the meeting search of the event 0x90 (a = the building, b = the radius kind 2 / 1 / other, c = the kind of the meeting, d = the low 2 bits of the character's +4): the building pointer +0x61 must exist; each of the 0x100 record slots of 0x889f74 (0xa9 bytes: the kind of 0x7e3f30 and the player word +0x27) whose kind bit (0x807c4116 / bit 0) is allowed, with a building and a character +0x61 within the radius (1500 / 800 / 400) of the building: each character record 0x770b80 (0x21c, id word != -1, kind +2 < 10, +4 & 3 == d) that this record owns (+0x16c = 3, +0x170 = 2, own building = 1) gets the lottery 0x5720f0 (chance (1 - dist / radius) * 0.5 + 0.25, c): mode 3 -> 0x5417d0 (0, ch, building, the record, v, f), mode 1 of a kind 6 / 7 -> the text 0x163c and the message 0x53dda0 (+0x5f7, 0x631090) */


void he_Unk572310(int a, int b, int c, unsigned int d)
{
    char text[0x800];
    char *per, *rec, *other, *bldrec, *ch;
    float x, y, z, dist, lim, f;
    unsigned int lo, hi;
    int i, n, v, mode;
    unsigned short bld;

    per = *(char **)(a + 0x61);
    if (per == 0)
        return;
    if (b == 2)
        lim = 1500.0f;
    else if (b == 1)
        lim = 800.0f;
    else
        lim = 400.0f;
    x = *(float *)(per + 0x4c);
    y = *(float *)(per + 0x50);
    z = *(float *)(per + 0x54);
    for (n = 0; n < 0xa900; n += 0xa9) {
        rec = (char *)(*(int *)(unsigned)&ai_Blds + n);
        lo = 0;
        hi = 0;
        if (*(unsigned char *)((unsigned)&ai_BTypes + rec[0] * 0x24d) < 0x20)
            lo = 1 << *(unsigned char *)((unsigned)&ai_BTypes + rec[0] * 0x24d);
        else
            hi = 1 << (*(unsigned char *)((unsigned)&ai_BTypes + rec[0] * 0x24d) - 0x20);
        bld = *(unsigned short *)(rec + 0x27);
        if (bld == 0xffff || bld == *(unsigned short *)(unsigned)&ai_PlayerChar)
            continue;
        other = *(char **)(rec + 0x61);
        if (other == 0)
            continue;
        if ((lo & 0x807c4116) == 0 && (hi & 1) == 0)
            continue;
        dist = sqrt((x - *(float *)(other + 0x4c)) * (x - *(float *)(other + 0x4c)) +
                    (y - *(float *)(other + 0x50)) * (y - *(float *)(other + 0x50)) +
                    (z - *(float *)(other + 0x54)) * (z - *(float *)(other + 0x54)));
        if (dist > lim)
            continue;
        bldrec = (char *)((unsigned)&ai_Chars + bld * 0x21c);
        for (i = 0; i < 0x80; i++) {
            ch = (char *)((unsigned)&ai_Chars + i * 0x21c);
            if (*(short *)ch == -1 || HM_B(ch, 2) >= 0xa || (HM_I(ch, 4) & 3) != (int)d)
                continue;
            if (HM_I(ch, 0x16c) != 0 && HM_I(ch, 0x16c) == (int)rec)
                mode = 3;
            else if (HM_I(ch, 0x170) != 0 && HM_I(ch, 0x170) == (int)rec)
                mode = 2;
            else if (bldrec != ch)
                continue;
            else
                mode = 1;
            v = he_Unk5720f0((unsigned short *)ch, (1.0f - dist / lim) * 0.5f + 0.25f, c, &f);
            if (v != 0) {
                if (mode == 3)
                    he_CharIllness(0, (AICHAR *)ch, (AICHAR *)bldrec, (AIBLD *)rec, v, f);
                else if (mode == 1 && (HM_B(ch, 2) == 6 || HM_B(ch, 2) == 7)) {
                    txt_Format(text, 0x163c, v + 0x1632);
                    he_ShowNews(HM_I(ch, 4), -1, text, 0, 0x5f7, (char *)(unsigned)&ch_EmptyName);
                }
            }
        }
    }
}
/* 0x5725b0, the run of the event 0x90 (the meeting): the states -2 / -1 free (and continue, as in the original); paused -> nothing; the due clock +0x52 = now; the building 0x501190 (1, 1, +0xac) missing -> 0x4c4c60 (-1); state 0 with +0xb8 < 4: 0x572310 (building, +0xb0, +0xb4, +0xb8), due now + 0x1e hours, +0xb8 + 1, 0x4c4c60 (0); else state -1, 0x4fe140 (+0x52, 0x16, 0, 0) and 0x4c4c60 (-1) */

void he_Unk5725b0(int ev)
{
    char *bld;

    if (HM_I(ev, 0x70) == -2 || HM_I(ev, 0x70) == -1)
        he_FreeEvent((int *)ev);
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    *(AITIME *)(ev + 0x52) = ai_Time;
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0xac));
    if (bld == 0) {
        cm_SendExSetHE(-1, ev);
        return;
    }
    if (HM_I(ev, 0x70) != 0)
        return;
    if (HM_I(ev, 0xb8) < 4) {
        he_Unk572310((int)bld, HM_I(ev, 0xb0), HM_I(ev, 0xb4), HM_I(ev, 0xb8));
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        HM_I(ev, 0xb8) = HM_I(ev, 0xb8) + 1;
        cm_SendExSetHE(0, ev);
        return;
    }
    HM_I(ev, 0x70) = -1;
    gm_AddTime(ev + 0x52, 0x16, 0, 0);
    cm_SendExSetHE(-1, ev);
}
/* 0x5726a0, the alloc of the event 0x91: state 0 */

void he_Unk5726a0(int ev)
{
    HM_I(ev, 0x70) = 0;
}
/* 0x5726b0, the run of the event 0x91 (the meeting scene object): the partner event 0x53e400 (1, 1, +0xac) must be active and the building 0x501190 (1, 1, +0x10) must have a scene object +0x61; the old object +0xb0 (0x4284a0, tag 0x66675073 and +4 == +0x10) stays; else the text 0x628fac / 0x628f74 / 0x628f3c by +0xb4 (2 / 1 / other), a new object 0x4287b0 (tag 0x66675073, +4 = +0x10), 0x4478e0, 0x458a20 (the position, +0x5ed098 on the z), 0x429ab0 (3, the position), 0x4478e0, +0x104 = 0, +0xb0 = the handle +0x100. Without the partner or the building: the old object 0x429e90 and the event freed. ROUGH */


void he_Unk5726b0(int ev)
{
    char *bld, *other, *obj;
    float p[3];

    other = he_FindEvent(1, 1, HM_I(ev, 0xac));
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld != 0 && HM_I(bld, 0x61) != 0 && other != 0 &&
        HM_I(other, 0x70) != -2 && HM_I(other, 0x70) != -1) {
        if (HM_I(ev, 0xb0) != -1) {
            obj = (char *)evt_FindById(HM_I(ev, 0xb0));
            if (obj != 0 && *(int *)obj == 0x66675073 && HM_I(obj, 4) == HM_I(ev, 0x10))
                return;
            HM_I(ev, 0xb0) = -1;
        }
        if (HM_I(ev, 0xb4) == 2)
            obj = (char *)evt_Load((char *)(unsigned)&full_data_628fac);
        else if (HM_I(ev, 0xb4) == 1)
            obj = (char *)evt_Load((char *)(unsigned)&full_data_628f74);
        else
            obj = (char *)evt_Load((char *)(unsigned)&full_data_628f3c);
        if (obj == 0)
            return;
        *(int *)obj = 0x66675073;
        HM_I(obj, 4) = HM_I(ev, 0x10);
        d3if_SelectScene(0, 1);
        d3_PointToWorld(HM_I(bld, 0x61), HM_I(bld, 0x61) + 0x4c, p);
        p[2] += 500.0f;
        evt_RunScriptArgs((EVTSCRIPT *)obj, 3, (int)p[0], (int)p[1], (int)p[2]);
        d3if_SelectScene(*(int *)(unsigned)&d3_CurrentScene, 1);
        HM_I(obj, 0x104) = 0;
        HM_I(ev, 0xb0) = HM_I(obj, 0x100);
        return;
    }
    obj = (char *)evt_FindById(HM_I(ev, 0xb0));
    if (obj != 0 && *(int *)obj == 0x66675073 && HM_I(obj, 4) == HM_I(ev, 0x10)) {
        d3if_SelectScene(0, 1);
        evt_Stop((EVTSCRIPT *)obj);
        d3if_SelectScene(*(int *)(unsigned)&d3_CurrentScene, 1);
    }
    he_FreeEvent((int *)ev);
}
/* 0x572860, the alloc of the events 0x8b and 0x92: the due clock +0x52 = now; the minute word +0x56 >= 0x17 -> due + 0x18 hours and the minute word = 0xc, else due + 6 hours; state 0 */

void he_Unk572860(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    if (*(unsigned short *)(ev + 0x56) >= 0x17) {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = 0xc;
    } else
        gm_AddTime(ev + 0x52, 6, 0, 0);
    HM_I(ev, 0x70) = 0;
}

void ai_Unk5a97e0(int param_1, unsigned short *param_2, int param_3, int param_4, int param_5, int param_6);                 /* 0x5a97e0 */
void ai_ShowGraveOrderCancelled(int param_1, unsigned short *param_2, int param_3, int param_4);                 /* 0x5a9880 */
int gv_OrderExtendGrave(int *g, char *o, int v, int b, short s, int x);                 /* 0x4f0700 */
int gv_OrderSetGraveHolder();                 /* 0x4f0900 */
void he_Unk5728d0(int ev);          /* 0x5728d0 */
void he_ExamCancelled(int ev);          /* 0x5729c0 */

/* 0x5728d0, the exam result: the building 0x501190 (1, 1, +0xac) of the kind 0x1d (0x7e3f30) and the character +0xc: 0x4f0700 (character, building, +0xb0, +0xb4, +0xbc, +0xb8); when the character is not the building's owner record and its kind is 6 / 7: 0x5a97e0 (the owner record, character, building, +0xb0, +0xb8, (int)(+0xb8 * 0x5ee034[+0xb0])) */
void he_Unk5728d0(int ev)
{
    char *bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0xac));
    char *per, *own;
    int n;

    if (bld == 0 || *(char *)((unsigned)&ai_BTypes + *(unsigned char *)bld * 0x24d) != 0x1d)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per == 0)
        return;
    gv_OrderExtendGrave(per, bld, HM_I(ev, 0xb0), HM_B(ev, 0xb4), *(unsigned short *)(ev + 0xbc), HM_I(ev, 0xb8));
    own = (char *)((unsigned)&ai_Chars + *(unsigned short *)(bld + 0x27) * 0x21c);
    if (per == own)
        return;
    if (HM_B(own, 2) != 6 && HM_B(own, 2) != 7)
        return;
    n = (int)((float)HM_I(ev, 0xb8) * *(float *)((unsigned)&gv_SaleShares + HM_I(ev, 0xb0) * 4));
    ai_Unk5a97e0(own, per, bld, HM_I(ev, 0xb0), HM_I(ev, 0xb8), n);
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
/* 0x572a50, the run of the event 0x92: state -2 releases the window +0x74 and frees; state 0 opens the window 0x53d6f0 (0x6269c0, 0x5ec), 0x475600 / 0x475f70, the texts 0x602568 and 0x411c, 0x4c27d0 (-7, 0, 0x628fe4, -1, 0) and state 1; state 1: due (0x4fe210) -> 0x5729c0, 0x53d800 and free; the window missing -> state 0; the actor 0xf8aac4 == window +8 and 0xf8a668: 0 -> 0x5728d0, 1 -> 0x5729c0, then 0x53d800 and free */

void he_Unk572a50(int ev)
{
    int state = HM_I(ev, 0x70);

    if (state == -2) {
        if (HM_I(ev, 0x74) != 0)
            he_ReleaseSlot(ev);
        he_FreeEvent((int *)ev);
        return;
    }
    if (state == 0) {
        if (HM_I(ev, 0x74) == 0)
            he_OpenEventSlot(ev, 0, 0, "Berufe\\Messagebox_Meisterpruefung", 0x5ec);
        if (HM_I(ev, 0x74) == 0) {
            he_FreeEvent((int *)ev);
            return;
        }
        d2_CentreViewWindows(HM_I(HM_I(ev, 0x74), 8));
        d2_SetForm(HM_I(HM_I(ev, 0x74), 8), 0);
        io_PrintText((unsigned)"$C");
        io_PrintText(0x411c, HM_I(ev, 0xb8));
        snd_QueueVoice(-7, 0, (char *)(unsigned)&full_data_628fe4, -1, 0);
        HM_I(ev, 0x70) = 1;
    } else if (state == 1) {
        if (gm_CompareTime(&ai_Time, ev + 0x52) > 0) {
            he_ExamCancelled(ev);
            he_ReleaseSlot(ev);
            he_FreeEvent((int *)ev);
            return;
        }
        if (HM_I(ev, 0x74) == 0) {
            HM_I(ev, 0x70) = 0;
            return;
        }
        if (*(int *)(unsigned)&d2_ClickedForm == HM_I(HM_I(ev, 0x74), 8) && *(int *)(unsigned)&d2_ClickedId != -1) {
            if (*(int *)(unsigned)&d2_ClickedId == 0) {
                he_Unk5728d0(ev);
                he_ReleaseSlot(ev);
                he_FreeEvent((int *)ev);
                return;
            }
            if (*(int *)(unsigned)&d2_ClickedId == 1) {
                he_ExamCancelled(ev);
                he_ReleaseSlot(ev);
                he_FreeEvent((int *)ev);
                return;
            }
        }
    }
}

void gm_SetTime(struct AITIME *c, unsigned char hour, unsigned min, unsigned sec); /* 0x4fe1e0 */

/* 0x572b90, the alloc of the event 0x93: the due clock +0x52 = now + 6 hours (0x4fe1e0), the second clock +0x60 = the due clock */
void he_Unk572b90(int ev)
{
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_SetTime(ev + 0x52, 6, 0, 0);
    *(AITIME *)(ev + 0x60) = *(AITIME *)(ev + 0x52);
}

int gm_Opinion(int a, int b); /* 0x50c520 */
unsigned char * gm_BuildNext(void); /* 0x501030 */
int gm_BuildingMainObject(unsigned char *b); /* 0x501b50 */
unsigned short *gm_QueryNext(void); /* 0x4ffcc0 */
void ai_EndCharExtra(char *o, int f); /* 0x587c40 */
float gm_GoodsPrice(unsigned id, unsigned char p); /* 0x5089c0 */
int ls_GetLanguage();                 /* 0x5190a0 */

/* 0x572bf0, the run of the event 0x93 (the working of a building 0x1d): -2 / -1 free. State 0: the building 0x501190 (1, 5, 0x1d) with a player word; the value 0x5092e0 (+0x15) * 0x5eb950 + 0x5eb94c; the characters +0x770b80 (0x21c) whose +0x16c is the building are collected; each one: 0x42ec20 over the value -> when over: 0x587c40 (char, 1) for the kinds 4 / 0x8c / 0x8d without +0x1b4 bit 0; when under and 0x42ec20 <= 0x5eb7e8: 0x50c520 (building +0x27, char) against -(0x42ec00 (0x4a)), then the message 0x94 (+4 = 0x94, +8 = the player word of the event, +0xc = building +4, +0x28 clock, +0x36 = 1, +0x58 = event +4) via 0x4c4b70. Tail: due now + 0x14 + 0x42ec00 (0x14), state + 1. State 1: the same collection with 0x50a810 / 0x501b50, a random pick 0x42ec00 (count), 0x5089c0, 0x4c4880, 0x5190a0 and the texts 0x414b / 0x229, the message 0x53dee0 (0x5f2, 0x629000), 0x501030 / 0x4ffcc0; then the tail. State 2: due now + 0x1e, state + 1. State 3: due now + 0x1e, state 0. ROUGH */
void he_Unk572bf0(int ev)
{
    char *chars[0x20];
    char msg[0xf8];
    char text[0x100];
    char *per, *bld, *ch;
    float f;
    int i, n, cnt, v, k;

    switch (HM_I(ev, 0x70)) {
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        return;
    case 0:
        bld = (char *)gm_BuildQuery(1, 5, 0x1d);
        if (bld == 0)
            goto tail;
        if (*(unsigned short *)(bld + 0x27) != 0xffff) {
            n = gm_BuildingSkill(bld, 0x15, 1);
            f = (float)n * (0.00249999994f);
            cnt = 0;
            for (ch = (char *)(unsigned)&ai_Chars; (int)ch < (unsigned)((unsigned)ai_Chars + 0x65400); ch += 0x21c) {
                if (*(unsigned short *)ch != 0xffff && HM_I(ch, 0x16c) == (int)bld)
                    chars[cnt++] = ch;
            }
            if (cnt > 0) {
                f += *(float *)(unsigned)&ai_Unk5eb94c;
                for (i = 0; i < cnt; i++) {
                    if (util_RandomF() <= f) {
                        if (util_RandomF() <= *(double *)(unsigned)&full_rdata_5eb7e8) {
                            ch = chars[i];
                            if (HM_I(ch, 0x17c) == 0) {
                                v = gm_Opinion(*(unsigned short *)(bld + 0x27), *(unsigned short *)ch);
                                k = -(util_Random(0x4a) & 0xffff);
                                if (k < v) {
                                    memset(msg, 0, 0xf8);
                                    HM_B(msg, 4) = 0x94;
                                    HM_I(msg, 8) = *(unsigned short *)(ev + 8);
                                    HM_I(msg, 0xc) = HM_I(bld, 4);
                                    *(AITIME *)(msg + 0x28) = ai_Time;
                                    HM_B(msg, 0x36) = 1;
                                    HM_I(msg, 0x58) = HM_I(ev, 4);
                                    cm_SendLongMessage(msg);
                                }
                            }
                        }
                    } else {
                        ch = chars[i];
                        if (!(HM_B(ch, 0x1b4) & 1) &&
                            (HM_B(ch, 0x180) == 4 || HM_B(ch, 0x180) == 0x8c || HM_B(ch, 0x180) == 0x8d))
                            ai_EndCharExtra(ch, 1);
                    }
                }
            }
        }
tail:
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x14 + (util_Random(0x14) & 0xffff), 0);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 1:
        bld = (char *)gm_BuildQuery(1, 5, 0x1d);
        if (bld == 0)
            goto tail;
        per = (char *)gm_FindCharacterByMaster(bld);
        cnt = 0;
        for (ch = (char *)(unsigned)&ai_Chars; (int)ch < (unsigned)((unsigned)ai_Chars + 0x65400); ch += 0x21c) {
            if (*(unsigned short *)ch != 0xffff && HM_I(ch, 0x16c) == (int)bld)
                chars[cnt++] = ch;
        }
        gm_BuildingSkill(bld, 0x15, 1);
        for (i = 0; i < cnt; i++) {
            ch = chars[i];
            if (!(HM_B(ch, 0x1b4) & 1))
                continue;
            if (!(HM_B(ch, 0x180) == 4 || HM_B(ch, 0x180) == 0x8c || HM_B(ch, 0x180) == 0x8d || HM_B(ch, 0x180) == 0))
                continue;
            if (per != 0 && per == ch)
                continue;
            if (util_RandomF() <= *(float *)(unsigned)&ai_Unk5eb94c) {
                if (per != 0 && (HM_B(per, 0x1b4) & 1) && HM_I(ch, 0x184) != 0) {
                    ai_EndCharExtra(ch, 0);
                    continue;
                }
            } else if (per == 0 || !(HM_B(per, 0x1b4) & 1) || HM_I(ch, 0x184) == 0) {
                ai_EndCharExtra(ch, 0);
                continue;
            }
            ai_EndCharExtra(ch, 0);
        }
        gm_BuildNext();
        per = (char *)gm_BuildingMainObject(bld);
        if (cnt > 0) {
            n = util_Random(cnt) & 0xffff;
            ch = chars[n];
            if (ch != 0) {
                gm_GoodsPrice(*(unsigned short *)ch, *(unsigned char *)(unsigned)&ai_OwnPlayer);
                cm_SendExSellObject(-1, HM_I(bld, 4), *(unsigned short *)ch, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                if (ls_GetLanguage() == 0 || ls_GetLanguage() == 1) {
                    txt_Format(text, 0x414b, 0x229, HM_I(ch, 0x16c), *(unsigned short *)ch * 2 + 0x9a8);
                }
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5f2, 0, *(int *)(per + 2), (char *)(unsigned)&full_data_629000, 0);
            }
        }
        he_FreeEvent((int *)ev);
        return;
    case 2:
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 3:
        *(AITIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        HM_I(ev, 0x70) = 0;
        return;
    }
}
/* 0x572fd0, the run of the event 0x94 (an employee at work): -2 / -1 free. State 0: the character +0xac (0x505df0) and its
   building +0x16c; when the building word +0x27 and the character word compare as -0x1a or less (0x50c520): the group of
   the building +0x5d (0x4ffe90 (.., 2, 6, 0, 0x2a), else .. 0x116) and its members (0x4ffe90 (group +0x14, 1, 5), then
   0x4ffcc0), one picked at random (0x42ec00); then 0x5089c0 (word, 0x6f1bc5), 0x4c4880 (-1, group +2, word, 1, 0x6f1bc5, 0),
   the text 0x414b / 0x229 (0x5176d0) for the results 0 .. 2 of 0x5190a0 and the message 0x53dee0 (0x5f2, 0x629000, the
   building +1, the owner +2) to the player word of the event (+8); then free */

void he_Unk572fd0(int ev)
{
    char *per, *owner, *ch, *grp, *e;
    int bld, cnt;
    char *lst[0x20];
    char text[0x400];

    switch (HM_I(ev, 0x70)) {
    case -2:
    case -1:
        he_FreeEvent((int *)ev);
        return;
    case 0:
        break;
    default:
        return;
    }
    per = (char *)gm_FindChar(HM_I(ev, 0xac));
    if (per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    bld = HM_I(per, 0x16c);
    if (bld == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (gm_Opinion(*(unsigned short *)(bld + 0x27), *(unsigned short *)per) > -0x1a) {
        he_FreeEvent((int *)ev);
        return;
    }
    grp = (char *)gm_Query(HM_I(bld, 0x5d), 2, 6, 0, 0x2a);
    if (grp == 0)
        grp = (char *)gm_Query(HM_I(bld, 0x5d), 2, 6, 0, 0x116);
    owner = (char *)gm_BuildingMainObject(bld);
    if (grp == 0)
        goto free;
    cnt = 0;
    memset(lst, 0, sizeof lst);
    e = (char *)gm_Query(HM_I(grp, 0x14), 1, 5);
    while (e != 0) {
        lst[cnt++] = e;
        e = (char *)gm_QueryNext();
    }
    if (cnt == 0)
        goto free;
    ch = lst[util_Random(cnt) & 0xffff];
    if (ch == 0)
        goto free;
    ((float (*)(unsigned short, unsigned char))gm_GoodsPrice)(*(unsigned short *)ch, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    ((int (*)(int, int, unsigned short, int, unsigned char, int))cm_SendExSellObject)(-1, HM_I(grp, 2), *(unsigned short *)ch, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
    switch (ls_GetLanguage()) {
    case 0:
    case 2:
        txt_Format(text, 0x414b, 0x229, HM_I(per, 0x16c), *(unsigned short *)ch * 2 + 0x9a8);
        break;
    case 1:
        txt_Format(text, 0x414b, 0x229, HM_I(per, 0x16c), *(unsigned short *)ch * 2 + 0x9a8);
        break;
    }
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5f2, HM_I(bld, 1), HM_I(owner, 2), 0, (char *)(unsigned)&full_data_629000);
free:
    he_FreeEvent((int *)ev);
}
/* 0x573200, 1 when a graveyard work is pending on a grave: an event of the type 0x8d (the work ai_StartBuildingWork
   posts) of the building bld (its id, the word at +1, the event's search key) whose +0xb0 is the grave's size (list)
   and whose byte +0xb4 is its index in that list */
int he_HasGraveWork(char *bld, int size, int index)
{
    char *ev;

    ev = he_FindEvent(2, 3, *(int *)(bld + 1), 0, 0x8d);
    while (ev != 0) {
        if (HM_I(ev, 0xb0) == size && HM_B(ev, 0xb4) == index)
            return 1;
        ev = he_NextEvent();
    }
    return 0;
}

void he_Unk573280(int ev);          /* 0x573280 */
void he_Unk5732d0(int ev);          /* 0x5732d0 */

/* 0x573260, registers the event type 0xa2 (called by 0x53d4b0): the pair (0x573280, 0x5732d0); 0 = ok, 1 = a registration failed */
int he_RegisterEventTypeA2(void)
{
    return he_SetEventHandlers(0xa2, (void *)he_Unk573280, (void *)he_Unk5732d0) != 0;
}
/* 0x573280, the alloc of the event 0xa2 (the daily Hanseatic League politics): nothing while paused (+0x78 & 4) or while the
   hour (ai_Time.b) is 0xa or less; else the due clock +0x52 = now + 24 hours (0x4fe140), its hour +0x56 = 0xa, the minute
   +0x58 and the second +0x5c = 0, state 0 */

void he_Unk573280(int ev)
{
    if (HM_B(ev, 0x78) & 4)
        return;
    if (ai_Time.hour <= 0xa)
        return;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(unsigned short *)(ev + 0x56) = 0xa;
    HM_I(ev, 0x58) = 0;
    HM_I(ev, 0x5c) = 0;
    HM_I(ev, 0x70) = 0;
}

void hmt_Update();                 /* 0x4ace40, hmt_Update */
void hmt_Begin();                 /* 0x4ad0a0, hmt_Begin */
void hmt_Election();                 /* 0x4ad1c0, hmt_Election */
void hmt_ClearOffices(void); /* 0x4ad510 */
void hmt_Unk4ad580();                 /* 0x4ad580 */

/* 0x5732d0, the run of the event 0xa2 (the daily Hanseatic League politics): nothing while paused (+0x78 & 4); the state
   +0x70 0 .. 4 picks the step, each schedules the next state with 0x4c4c60: 0 hmt_Update and +2 hours, 1 hmt_Begin and
   +2 hours, 2 hmt_Election and the time of day 0x16 (0x4fe1e0), 3 hmt_ClearOffices and the time of day 6:05 with the day +1,
   4 hmt_Unk4ad580 and the time of day 0xa */
void he_Unk5732d0(int ev)
{
    int *c;

    if (HM_B(ev, 0x78) & 4)
        return;
    switch (HM_I(ev, 0x70)) {
    case 0:
        hmt_Update();
        gm_AddTime(ev + 0x52, 2, 0, 0);
        cm_SendExSetHE(1, ev);
        return;
    case 1:
        hmt_Begin();
        gm_AddTime(ev + 0x52, 2, 0, 0);
        cm_SendExSetHE(2, ev);
        return;
    case 2:
        hmt_Election();
        gm_SetTime(ev + 0x52, 0x16, 0, 0);
        cm_SendExSetHE(3, ev);
        return;
    case 3:
        hmt_ClearOffices();
        gm_SetTime(ev + 0x52, 6, 5, 0);
        c = (int *)(ev + 0x52);
        *c = *c + 1;
        cm_SendExSetHE(4, ev);
        return;
    case 4:
        hmt_Unk4ad580();
        gm_SetTime(ev + 0x52, 0xa, 0, 0);
        cm_SendExSetHE(0, ev);
        return;
    }
}

void he_Unk5734b0(int ev);          /* 0x5734b0 */
void he_Unk573580(int ev);          /* 0x573580 */
void he_Unk573f20(int ev);          /* 0x573f20 */
void he_Unk573fb0(int ev);          /* 0x573fb0 */
void he_Unk574060(int ev);          /* 0x574060 */
void he_Unk5740d0(int ev);          /* 0x5740d0 */
void he_Unk574120(int ev);          /* 0x574120 */
void he_Unk5743c0(int ev);          /* 0x5743c0 */
void he_Unk574550(int ev);          /* 0x574550 */
void he_Unk5748d0(int ev);          /* 0x5748d0 */
void he_Unk575150(int ev);          /* 0x575150 */
void he_Unk5751e0(int ev);          /* 0x5751e0 */
void he_Unk575320(int ev);          /* 0x575320 */
void he_Unk5753a0(int ev);          /* 0x5753a0 */

/* 0x5733c0, registers the event types 0x9d .. 0xa1, 0xa3 and 0xa4 (called by 0x53d4b0): the pairs (0x5734b0, 0x573580), (0x573f20, 0x573fb0), (0x574060, 0x5740d0), (0x574120, 0x5743c0), (0x574550, 0x5748d0), (0x575150, 0x5751e0) and (0x575320, 0x5753a0), then the type text 0x50f of 0xa1 (0x52b350); 0 = ok, 1 = a registration failed */
int he_RegisterEventTypes9d(void)
{
    if (he_SetEventHandlers(0x9d, (void *)he_Unk5734b0, (void *)he_Unk573580) == 0) {
        if (he_SetEventHandlers(0x9e, (void *)he_Unk573f20, (void *)he_Unk573fb0) == 0) {
            if (he_SetEventHandlers(0x9f, (void *)he_Unk574060, (void *)he_Unk5740d0) == 0) {
                if (he_SetEventHandlers(0xa0, (void *)he_Unk574120, (void *)he_Unk5743c0) == 0) {
                    if (he_SetEventHandlers(0xa1, (void *)he_Unk574550, (void *)he_Unk5748d0) == 0) {
                        if (he_SetEventHandlers(0xa3, (void *)he_Unk575150, (void *)he_Unk5751e0) == 0) {
                            if (he_SetEventHandlers(0xa4, (void *)he_Unk575320, (void *)he_Unk5753a0) == 0) {
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
/* 0x5734b0, the alloc of the event 0x9d: state 0; nothing while paused (+0x78 & 4); both clocks +0x44 / +0x52 = now; the
   figure +0xb0 (0x505df0): without one the event is scheduled with -1; else the message 0x4f2e30 (0x1c, 1, the player id
   +0xc, figure +4, *(figure + 0x170) ? the word after it : -1) and the handler scheduled with 0 */

void he_Unk5734b0(int ev)
{
    char *per;
    int x;

    HM_I(ev, 0x70) = 0;
    if (HM_B(ev, 0x78) & 4)
        return;
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    per = (char *)gm_FindChar(HM_I(ev, 0xb0));
    if (per == 0) {
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    x = HM_I(per, 0x170);
    if (x != 0)
        x = HM_I(x, 1);
    else
        x = -1;
    gv_Accuse(0x1c, 1, HM_I(ev, 0xc), HM_I(per, 4), x);
    HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
}

void he_Unk5736d0(char *pl, char *per, int a);  /* 0x5736d0 */
void he_Unk573e30(char *pl, int a);             /* 0x573e30 */

/* 0x573580, the run of the event 0x9d: -2 / -1 free; nothing while paused or while the pending handle +0x84 is not done
   (0x4c3ae0); then +0x84 = -1, the due clock +0x52 = now, the figure +0xb0 (0x505df0) and the player record (0x770b80) must
   exist (else scheduled with -1): state 0 calls 0x5736d0 (player, figure, ev +0xac) and retries in 24 hours with the hour 6
   and the minute 15 * random (4) + 5; state 1 calls 0x573e30 (player, ev +0xac) and schedules -1 */
void he_Unk573580(int ev)
{
    char *per, *pl;

    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (HM_B(ev, 0x78) & 4)
        return;
    if (HM_I(ev, 0x84) != -1 && cm_GetSlotState(HM_I(ev, 0x84)) == 0)
        return;
    HM_I(ev, 0x84) = -1;
    *(AITIME *)(ev + 0x52) = ai_Time;
    per = (char *)gm_FindChar(HM_I(ev, 0xb0));
    if (per == 0 || *(unsigned short *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c) == 0xffff) {
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    pl = (char *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c);
    switch (HM_I(ev, 0x70)) {
    case 0:
        he_Unk5736d0(pl, per, HM_I(ev, 0xac));
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = 6;
        HM_I(ev, 0x58) = (util_Random(4) & 0xffff) * 0xf + 5;
        HM_I(ev, 0x84) = cm_SendExSetHE(1, ev);
        return;
    case 1:
        he_Unk573e30(pl, HM_I(ev, 0xac));
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
}

void he_Unk573760(char *pl, char *per);     /* 0x573760 */
void he_Unk573820(char *pl, char *per);     /* 0x573820 */
void he_Unk573970(char *pl, char *per);     /* 0x573970 */
void he_Unk573ae0(char *pl, char *per);     /* 0x573ae0 */
void he_Unk573c20(char *pl, char *per);     /* 0x573c20 */
void he_Unk573d30(char *pl, char *per);     /* 0x573d30 */

/* 0x5736d0, dispatches the approach +0xac `a` of the event 0x9d: 0 0x573760, 1 0x573820, 2 0x573970, 3 0x573ae0, 4 0x573c20, else 0x573d30 */
void he_Unk5736d0(char *pl, char *per, int a)
{
    if (a == 0)
        he_Unk573760(pl, per);
    else if (a == 1)
        he_Unk573820(pl, per);
    else if (a == 2)
        he_Unk573970(pl, per);
    else if (a == 3)
        he_Unk573ae0(pl, per);
    else if (a == 4)
        he_Unk573c20(pl, per);
    else
        he_Unk573d30(pl, per);
}
/* 0x573760, the approach 0 of the event 0x9d: with the two words of 0x505230 (per, ..) a name (copied from 0x629038 when
   the figure is the event's own, else 0x62901c, 0x19 bytes) and the texts 0x41de / 0x41dd (else 0x41dc) are built and
   sent as the message 0x5fb (0x53dda0) to per +4 */

void he_Unk573760(char *pl, char *per)
{
    char name[0x19];
    char t1[0x400];
    char t2[0x400];
    int a, b;

    if (gm_CatchDisease(per, &a, &b) == 0)
        return;
    if (per == pl) {
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
        txt_Format(t1, 0x41de, a + 0x1632);
        txt_Format(t2, 0x41dd, t1);
    } else {
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
        txt_Format(t1, 0x41de, a + 0x1632);
        txt_Format(t2, 0x41dc, t1);
    }
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, name);
}
/* 0x573820, the approach 1 of the event 0x9d: the relation of per changes (0x4ea550): to the figure of the byte +0x166
   (0x4e6f80), when there is one: by -0x14 - random (0x10); else, for a figure not of the kind 5 / 6 / 7: by 1 + random (3) and
   0x14 + random (0x10); else to every other figure of the kinds 5 .. 7 by -0x14 - random (0x10); then the texts 0x41df / 0x41dd
   (0x41dc when per is not pl) and the message 0x5fb (0x53dda0) to per +4 */

void he_Unk573820(char *pl, char *per)
{
    char t1[0x400];
    char t2[0x400];
    char *ch;
    int a, x, i;

    if (amt_GetKind(HM_B(per, 0x166), &a) != 0 && a != 0) {
        amt_ChangeDistrictOpinion(per, a, -0x14 - (util_Random(0x10) & 0xffff));
    } else if (HM_B(per, 2) != 6 && HM_B(per, 2) != 7 && HM_B(per, 2) != 5) {
        amt_ChangeDistrictOpinion(per, 1 + (util_Random(3) & 0xffff), 0x14 + (util_Random(0x10) & 0xffff));
    } else {
        x = -0x14 - (util_Random(0x10) & 0xffff);
        for (i = 0; i < 0x300; i++) {
            if (i == *(unsigned short *)per)
                continue;
            ch = (char *)((unsigned)&ai_Chars + i * 0x21c);
            if (HM_B(ch, 2) == 6 || HM_B(ch, 2) == 7 || HM_B(ch, 2) == 5)
                cm_SendRelations(HM_I(per, 4), HM_I(ch, 4), x, 1);
        }
    }
    txt_Format(t1, 0x41df);
    txt_Format(t2, per == pl ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, 0);
}
/* 0x573970, the approach 2 of the event 0x9d: searches the characters (0x770b80, 0x300) of the kind 0 without the byte +0x164
   whose building (+0x16c) word +0x27 is pl's word; when one is found a message 0x33 (0x4c4b70, the clock, the building +1 at
   +0xc, the character +4 at +0x58) is posted for pl +4, the name (0x629038 when pl is per, else 0x62901c, 0x19 bytes) and the
   texts 0x41e0 / 0x41dd (else 0x41dc) are built and sent as the message 0x5fb (0x53dda0) to pl +4 */

void he_Unk573970(char *pl, char *per)
{
    char msg[0xf8];
    char name[0x19];
    char t1[0x400];
    char t2[0x400];
    char *ch;
    int i;

    ch = 0;
    for (i = 0; i < 0x300; i++) {
        char *c = (char *)((unsigned)&ai_Chars + i * 0x21c);
        if (HM_B(c, 2) == 0 && HM_I(c, 0x16c) != 0 && HM_B(c, 0x164) == 0
            && *(unsigned short *)(HM_I(c, 0x16c) + 0x27) == *(unsigned short *)pl) {
            ch = c;
            break;
        }
    }
    if (ch == 0)
        return;
    memset(msg, 0, 0xf8);
    HM_B(msg, 4) = 0x33;
    HM_I(msg, 8) = HM_I(pl, 4);
    HM_I(msg, 0xc) = HM_I(HM_I(ch, 0x16c), 1);
    *(AITIME *)(msg + 0x28) = ai_Time;
    HM_B(msg, 0x36) = 1;
    HM_I(msg, 0x58) = HM_I(ch, 4);
    cm_SendLongMessage(msg);
    if (pl == per)
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
    else
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
    txt_Format(t1, 0x41e0, *(unsigned short *)ch, HM_I(ch, 0x16c));
    txt_Format(t2, pl == per ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(pl, 4), -1, t2, 1, 0x5fb, name);
}
/* 0x573ae0, the approach 3 of the event 0x9d: over the five values per +0x80 .. +0x84 the highest and the lowest are sought;
   with two different ones their differences are sent (0x4c6020 (per +4, index, difference), highest - lowest and lowest -
   highest), the name and the texts 0x41e1 (the indices + 0x16b7) / 0x41dd (else 0x41dc) are built and the message 0x5fb
   (0x53dda0) sent to per +4 */

void he_Unk573ae0(char *pl, char *per)
{
    char name[0x19];
    char t1[0x400];
    char t2[0x400];
    int hi = -1, lo = 0xff, hi_i = -1, lo_i = -1, i, v;

    for (i = 0; i < 5; i++) {
        v = HM_B(per, 0x80 + i);
        if (v > hi) {
            hi = v;
            hi_i = i;
        }
        if (v < lo) {
            lo = v;
            lo_i = i;
        }
    }
    if (lo_i == hi_i || lo_i < 0 || hi_i < 0)
        return;
    cm_SendAddSkill(HM_I(per, 4), lo_i, hi - lo);
    cm_SendAddSkill(HM_I(per, 4), hi_i, lo - hi);
    if (pl == per)
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
    else
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
    txt_Format(t1, 0x41e1, lo_i + 0x16b7, hi_i + 0x16b7);
    txt_Format(t2, pl == per ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, name);
}
/* 0x573c20, the approach 4 of the event 0x9d: posts a message 0x9e (0x4c4b70) from per +4 with the clock, -1 at +0xc, +0x58 =
   pl +4 (-1 when pl is per) and +0x60 = per +4; then the name (0x629038 / 0x62901c) and the texts 0x41e2 / 0x41dd (else
   0x41dc) and the message 0x5fb (0x53dda0) to per +4 */

void he_Unk573c20(char *pl, char *per)
{
    char msg[0xf8];
    char name[0x19];
    char t1[0x400];
    char t2[0x400];

    memset(msg, 0, 0xf8);
    HM_B(msg, 4) = 0x9e;
    HM_I(msg, 8) = HM_I(per, 4);
    HM_I(msg, 0xc) = -1;
    *(AITIME *)(msg + 0x28) = ai_Time;
    HM_B(msg, 0x36) = 1;
    HM_I(msg, 0x58) = pl == per ? -1 : HM_I(pl, 4);
    HM_I(msg, 0x60) = HM_I(per, 4);
    cm_SendLongMessage(msg);
    if (pl == per)
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
    else
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
    txt_Format(t1, 0x41e2);
    txt_Format(t2, pl == per ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, name);
}
/* 0x573d30, the default approach of the event 0x9d: posts a message 0x9f (0x4c4b70) from per +4 with the clock, -1 at +0xc;
   then the name (0x629038 / 0x62901c) and the texts 0x41e3 / 0x41dd (else 0x41dc) and the message 0x5fb (0x53dda0) to per +4 */

void he_Unk573d30(char *pl, char *per)
{
    char msg[0xf8];
    char name[0x19];
    char t1[0x400];
    char t2[0x400];

    memset(msg, 0, 0xf8);
    HM_B(msg, 4) = 0x9f;
    HM_I(msg, 8) = HM_I(per, 4);
    HM_I(msg, 0xc) = -1;
    HM_I(msg, 0x28) = U_I((unsigned)&ai_Time);
    HM_I(msg, 0x2c) = U_I((unsigned)&ai_Time.hour);
    HM_I(msg, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
    HM_B(msg, 0x36) = 1;
    cm_SendLongMessage(msg);
    if (pl == per)
        memcpy(name, (void *)(unsigned)&full_data_629038, 0x19);
    else
        memcpy(name, (void *)(unsigned)&full_data_62901c, 0x19);
    txt_Format(t1, 0x41e3);
    txt_Format(t2, pl == per ? 0x41dd : 0x41dc, t1);
    he_ShowNews(HM_I(per, 4), -1, t2, 1, 0x5fb, name);
}

void gm_SendMoneyDelta(int a, int b);                 /* 0x50cc50 */

/* 0x573e30, the second step of the event 0x9d for the figure pl and its argument a (the approach): a random count n by the
   approach (a < 2: 2 + random (2); a == 2: 3 + random (2); a == 3: 3 + random (3); else 4 + random (4), 3 when a == 4); while
   pl's +0x194 is below n the approach 0x5736d0 (pl, pl, a) runs; else pl +4 is charged -n (0x50cc50) and the text 0x41db /
   the message 0x5fa (0x53dda0) with 0x629054 are sent to pl +4 */
void he_Unk573e30(char *pl, int a)
{
    char text[0x200];
    int n;

    if (a < 2)
        n = (util_Random(2) & 0xffff) + 2;
    else if (a == 2)
        n = (util_Random(2) & 0xffff) + 3;
    else if (a == 3)
        n = (util_Random(3) & 0xffff) + 3;
    else
        n = (util_Random(4) & 0xffff) + (a == 4 ? 3 : 4);
    if (HM_I(pl, 0x194) < n) {
        he_Unk5736d0(pl, pl, a);
        return;
    }
    gm_SendMoneyDelta(HM_I(pl, 4), -n);
    txt_Format(text, 0x41db, n);
    he_ShowNews(HM_I(pl, 4), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_629054);
}
/* 0x573f20, the alloc of the event 0x9e: state 0, the start and the due clock +0x44 / +0x52 = now, the due clock + 24 hours
   (0x4fe140), the hour +0x56 = 6 and the minute +0x58 = 5 + random (0x10) */

void he_Unk573f20(int ev)
{
    HM_I(ev, 0x70) = 0;
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(unsigned short *)(ev + 0x56) = 6;
    HM_I(ev, 0x58) = (util_Random(0x10) & 0xffff) + 5;
}
/* 0x573fb0, the run of the event 0x9e: -2 / -1 free; the state + 1; the figures +0xb0 (a) and +0xac (b) (0x505df0); from a's
   +0x194 (n) a is charged -n and b n / 2 (at least 2) (0x50cc50); while the state stays below 2 the due clock +0x52 = now +
   24 hours (0x4fe140), else the event is freed */

void he_Unk573fb0(int ev)
{
    char *a, *b;
    int n, x;

    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
    a = (char *)gm_FindChar(HM_I(ev, 0xb0));
    b = (char *)gm_FindChar(HM_I(ev, 0xac));
    if (a == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    n = HM_I(a, 0x194);
    if (n != 0) {
        gm_SendMoneyDelta(HM_I(a, 4), -n);
        x = n / 2;
        if (x <= 2)
            x = 2;
        if (b != 0)
            gm_SendMoneyDelta(HM_I(b, 4), x);
    }
    if (HM_I(ev, 0x70) < 2) {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        return;
    }
    he_FreeEvent((int *)ev);
}
/* 0x574060, the alloc of the event 0x9f: state -1, the start and the due clock +0x44 / +0x52 = now, the due clock + 24 hours
   (0x4fe140) */

void he_Unk574060(int ev)
{
    HM_I(ev, 0x70) = -1;
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
}
/* 0x5740d0, the run of the event 0x9f: -2 / -1 free */

void he_Unk5740d0(int ev)
{
    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2)
        he_FreeEvent((int *)ev);
}
/* 0x5740f0, 1 when an event of the type 0x9f of the player word of `a` exists (0x53e400 (2, 0, 0x9f, 2, word)) */

int ai_Unk5740f0(char *a)
{
    return he_FindEvent(2, 0, 0x9f, 2, *(unsigned short *)a) != 0;
}
/* 0x5741d0, the event of the type 0xa0 whose +0x10 argument is the word after `a` (0x53e400 (2, 0, 0xa0, 3, ..)), 0 when none */

char *ai_FindCurseEvent(char *a)
{
    return he_FindEvent(2, 0, 0xa0, 3, *(int *)(a + 1));
}
/* 0x574120, the alloc of the event 0xa0: nothing while paused (+0x78 & 4); the start and the due clock +0x44 / +0x52 = now; when
   0x6f2448 is set the due clock + 1 (0x4fe140 (.., 1, 0, 0)), else + 24 hours with the hour +0x56 = 0x13 + random (2); the
   minute +0x58 = 0 and the handler scheduled with 0 */

void he_Unk574120(int ev)
{
    if (HM_B(ev, 0x78) & 4)
        return;
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    if (U_I((unsigned)&gm_AllowAll) != 0) {
        gm_AddTime(ev + 0x52, 1, 0, 0);
    } else {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(unsigned short *)(ev + 0x56) = util_Random(2) + 0x13;
    }
    HM_I(ev, 0x58) = 0;
    HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
}

int gm_TrialBuildingKind();                 /* 0x50d060 */

/* 0x5741f0, the trial of the event 0xa0 (ROUGH): the building +0x10 (0x4fea50) and the figure +0xc (0x505df0) must exist; by the
   building kind (0x50d060) 3 / 2 / else the approach +0xac gives a base value (0x14 / 0xa / 0 for a below 4 / below 7) plus the
   float constants 0x5ebad4 / 0x5eb7f4 and a bonus count (0xa / 0x14 / 0x19); a random amount (0x42ec00 (0x18 / 0x10 / 8) + 0xf /
   0xc / 0xa, times 0x20) and the other characters of the kind below 0xa whose 0x50c550 (word, per's word, 1) reaches the base
   are counted; text 0x41fa (0x5176d0), the message 0x5fa (0x53dda0) and the payment -amount from per +4 (0x4c4830) */
void he_Unk5741f0(int ev)
{
    char text[0x200];
    char *bld, *per, *ch;
    int a, kind, cnt, n, i, x;
    float base;

    gm_FindById(&bld, 0, 0, HM_I(ev, 0x10));
    if (bld == 0)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per == 0)
        return;
    a = HM_I(ev, 0xac);
    kind = gm_TrialBuildingKind(bld);
    cnt = 0;
    x = 0;
    if (kind == 3) {
        if (a < 4) {
            base = 0x14 + (40.0f);
            x = 0xa;
        } else if (a < 7) {
            base = 0xa + (40.0f);
            x = 0x14;
        } else {
            base = (40.0f);
            x = 0x19;
        }
        n = (util_Random(0x18) & 0xffff) + 0xf;
    } else if (kind == 2) {
        if (a < 4) {
            base = 0xa + (50.0f);
            x = 0xf;
        } else {
            base = (50.0f);
            x = 0xf;
        }
        n = (util_Random(0x10) & 0xffff) + 0xc;
    } else {
        base = 70.0f;
        n = (util_Random(8) & 0xffff) + 0xa;
    }
    n = n * 0x20;
    for (i = 0; i < 0x300; i++) {
        ch = (char *)((unsigned)&ai_Chars + i * 0x21c);
        if (*(unsigned short *)ch == 0xffff || ch == per || HM_B(ch, 2) >= 0xa)
            continue;
        if (((float (*)(unsigned short, unsigned short, int))gm_Liking)(*(unsigned short *)ch, *(unsigned short *)per, 1) >= base)
            cnt++;
    }
    cnt += x;
    txt_Format(text, 0x41fa, 0, cnt, n * cnt);
    he_ShowNews(HM_I(per, 4), -1, text, 1, 0x5fa, 0);
    cm_SendMoveGoodsUnchecked(HM_I(per, 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, n * cnt);
}
/* 0x5743c0, the run of the event 0xa0 (the trial): a pending handle +0x84 is awaited (0x4c3ae0); -2 / -1 free; state 0: the
   building +0x10 (0x502020) and its group (0x4ffe90 ([building +0x5d], 1, 0, 0x27f)) are taken, the due clock + 2 hours
   (0x4fe140); with a group the texts 0x4205 / 0x5fc (0x5176d0 / 0x53dee0) and the scene (0x511dc0, 0x4287b0, 0x429ab0) run;
   then +0x58 = 0 and state 1; state 1: nothing while paused (+0x78 & 4), else the trial 0x5741f0 and scheduled with -1 */

void he_Unk5743c0(int ev)
{
    char text[0x400];
    char *bld, *grp, *ch;
    int sc;

    if (HM_I(ev, 0x84) != -1 && cm_GetSlotState(HM_I(ev, 0x84)) == 0)
        return;
    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (HM_I(ev, 0x70) == 0) {
        bld = (char *)gm_FindBuildingById(HM_I(ev, 0x10));
        grp = 0;
        if (bld != 0)
            grp = (char *)gm_Query(HM_I(bld, 0x5d), 1, 0, 0x27f);
        gm_AddTime(ev + 0x52, 2, 0, 0);
        if (grp != 0) {
            ch = (char *)((unsigned)&ai_Chars + *(unsigned short *)(bld + 0x27) * 0x21c);
            txt_Format(text, 0x4205, bld);
            he_MessageBoxQuickjump(HM_I(ch, 4), HM_I(ch, 4), text, 0, 0x5fc, HM_I(bld, 1), HM_I(grp, 2), (char *)(unsigned)&ch_EmptyName, (char *)(unsigned)&ch_EmptyName);
            sc = U_I((unsigned)&d3_CurrentScene);
            d3if_SelectScene(init_ObjectSceneUniverse(grp), 1);
            {
                int o = (int)evt_Load((unsigned)&full_data_629070);
                if (o != 0) {
                    HM_I(o, 0xc8c) = 1;
                    HM_I(o, 0x104) = HM_I(grp, 2);
                    evt_RunScriptArgs((EVTSCRIPT *)o, 1, HM_I(ev, 0xac));
                }
            }
            d3if_SelectScene(sc, 1);
        }
        HM_I(ev, 0x58) = 0;
        HM_I(ev, 0x70) = 1;
        return;
    }
    if (HM_I(ev, 0x70) == 1 && !(HM_B(ev, 0x78) & 4)) {
        he_Unk5741f0(ev);
        HM_I(ev, 0x84) = cm_SendExSetHE(-1, ev);
    }
}
/* 0x574550, the alloc of the event 0xa1 (a meeting of up to four figures +0x8c ..): when one of them is in the building +0x10
   (0x505df0, +0x16c, +1) the start and the due clock +0x44 / +0x52 = now, two indices of the table 7 / 0x16 / 0xf / 0xa
   (0x505ca0 (4, out)) go to +0xc0 / +0xc2 and +0xc4 / +0xc6, the words +0xc8 .. +0xce = 0, +0xac = 0, +0xb0 / +0xb4 = 0xf0,
   +0xbc = 0, +0xd0 = -1 and state 0; else the event is freed */

void he_Unk574550(int ev)
{
    unsigned short w[4];
    int out[2];
    char *per;
    int i, bld;

    w[0] = 7;
    w[1] = 0x16;
    w[2] = 0xf;
    w[3] = 0xa;
    bld = 0;
    for (i = 0; i < 4; i++) {
        per = (char *)gm_FindChar(HM_I(ev, 0x8c + i * 4));
        if (per == 0)
            continue;
        bld = HM_I(per, 0x16c);
        if (bld == 0)
            continue;
        if (HM_I(bld, 1) == HM_I(ev, 0x10))
            break;
    }
    if (i == 4) {
        he_FreeEvent((int *)ev);
        return;
    }
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    gm_Shuffled(4, out);
    *(unsigned short *)(ev + 0xce) = 0;
    *(unsigned short *)(ev + 0xc0) = w[out[0]];
    *(unsigned short *)(ev + 0xc2) = w[out[1]];
    *(unsigned short *)(ev + 0xcc) = 0;
    *(unsigned short *)(ev + 0xc4) = w[out[0]];
    *(unsigned short *)(ev + 0xc6) = w[out[1]];
    *(unsigned short *)(ev + 0xca) = 0;
    *(unsigned short *)(ev + 0xc8) = 0;
    HM_I(ev, 0xac) = 0;
    HM_I(ev, 0xb0) = 0xf0;
    HM_I(ev, 0xb4) = 0xf0;
    HM_I(ev, 0xbc) = 0;
    HM_I(ev, 0xd0) = -1;
    HM_I(ev, 0x70) = 0;
}
/* 0x5746b0, the scene of the event 0xa1 (the meeting, ROUGH): the figure +0xc (0x505df0) and the building +0x10 (0x4fea50); the
   non-zero words +0xc8 .. +0xce are looked up (0x4a5760 (per, n, words, out)); each found record pays the figure through
   0x4c4880 (per +4, -1, 1, 0x6f1bc5, 0, word) or, from the level +0xe 3 and the sixth entry on, raises +0xac by the value
   (0x5089c0 / 0x5de518); text 0x41f2 (0x41f1 when none paid, 0x41f0 with the count, 0x5176d0), the payment (0x4c4830) and the
   message 0x5ff (0x53dda0) to per +4 */

void he_Unk5746b0(int ev)
{
    char text[0x200];
    char text2[0x100];
    char *per, *bld;
    int tab[4][4];
    int words[4], pay[4];
    int n, i, k, np;

    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per == 0)
        return;
    gm_FindById(&bld, 0, 0, HM_I(ev, 0x10));
    if (bld == 0)
        return;
    memset(tab, 0, sizeof tab);
    n = 0;
    for (k = 0; k < 4; k++) {
        unsigned short v = *(unsigned short *)(ev + 0xc8 + k * 2);
        if (v != 0)
            tab[n++][0] = v;
    }
    np = 0;
    if (n != 0) {
        ai_FindWantedItems(per, n, tab, &words[0]);
        for (i = 0; i < n; i++) {
            char *rec = (char *)tab[i][1];
            if (rec != 0) {
                if (HM_I(rec, 0xe) >= 3) {
                    gm_GoodsPrice(tab[i][0], *(unsigned char *)(unsigned)&ai_OwnPlayer);
                    HM_I(ev, 0xac) = HM_I(ev, 0xac) + _ftol();
                } else {
                    cm_SendExSellObject(HM_I(per, 4), -1, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0, tab[i][0]);
                    pay[np++] = tab[i][0];
                }
            } else if (words[0] >= 6) {
                gm_GoodsPrice(tab[i][0], *(unsigned char *)(unsigned)&ai_OwnPlayer);
                HM_I(ev, 0xac) = HM_I(ev, 0xac) + _ftol();
            } else {
                cm_SendExSellObject(HM_I(per, 4), -1, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0, tab[i][0]);
                pay[np++] = tab[i][0];
            }
        }
        if (np != 0)
            txt_Format(text, 0x41f2, HM_I(ev, 0xac));
        else
            txt_Format(text, 0x41f1, HM_I(ev, 0xac));
    } else {
        txt_Format(text, 0x41f1, HM_I(ev, 0xac));
    }
    cm_SendMoveGoodsUnchecked(HM_I(per, 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, HM_I(ev, 0xac));
    txt_Format(text2, 0x41f0, np, text);
    he_ShowNews(HM_I(per, 4), -1, text2, 0, 0x5ff, 0);
}

void *gm_GetGebaeudeHauptraum(unsigned char *b); /* 0x501ce0 */
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g); /* 0x524260, the group register */
void cm_SendChrStop(int a); /* 0x4c5230 */
int cm_SendChrGotoBuilding();                 /* 0x4c52e0 */
void cm_SendChrAnim(int a, const char *s, unsigned char c); /* 0x4c5410 */
int sv_IsAtDoor();                 /* 0x524770 */
void ai_Unk575030();                /* 0x575030 */

/* 0x5748d0, the run of the event 0xa1 (the meeting, ROUGH SKELETON): the four figures +0x8c .. (0x505df0) are collected; -2 / -1
   free; without a figure also; state 0 looks for the meeting room (+0xb0 > 0: 0x574fe0 else the building +0x10 (0x4fea50,
   0x501ce0)), state 1 groups them (0x524260 / 0x4c5230 / 0x4c52e0), later states the scene 0x5746b0 (0x524770), the payments
   (0x4fe140, 0x42ec00, 0x4c5410, 0x575030) and the free (0x53e1f0) */
void he_Unk5748d0(int ev)
{
    char *pers[4];
    char *bld;
    int n, i, state, x;

    n = 0;
    for (i = 0; i < 4; i++) {
        int id = HM_I(ev, 0x8c + i * 4);
        if (id != -1) {
            char *p = (char *)gm_FindChar(id);
            if (p != 0)
                pers[n++] = p;
        }
    }
    state = HM_I(ev, 0x70);
    if (state == -1 || state == -2 || n == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (state == 0) {
        x = -1;
        if (HM_I(ev, 0xb0) > 0) {
            x = he_FindMeetingRoom(ev);
            HM_I(ev, 0xbc) = HM_I(ev, 0xbc) + 1;
        } else {
            bld = 0;
            gm_FindById(&bld, 0, 0, HM_I(ev, 0x10));
            if (bld != 0 && gm_GetGebaeudeHauptraum(bld) != 0) {
                sv_MoveToGroup(pers[0], bld, 0x1f, ev);
                ((void (*)(int, int))cm_SendChrStop)(HM_I(pers[0], 4), HM_I(ev, 0xb4));
                cm_SendChrGotoBuilding(HM_I(pers[0], 4), pers[1], 0, 0, 0, (char *)(unsigned)&full_data_629070);
            }
        }
        gm_AddTime(ev + 0x52, 0, 2, 0);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    }
    /* the later states: 1 the scene, 2 the payments, 3 the close (all ROUGH) */
    if (state == 1) {
        sv_IsAtDoor(ev);
        sv_MoveToGroup(0, 0, 0x1f, ev);
        he_Unk5746b0(ev);
        return;
    }
    if (state == 2) {
        gm_AddTime(ev + 0x52, 0, 2, 0);
        return;
    }
    ((void (*)(int, int))cm_SendChrStop)(HM_I(pers[0], 4), HM_I(ev, 0xb4));
    ai_Unk575030(ev);
    he_FreeEvent((int *)ev);
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

unsigned short ai_Unk5a9e90(int param_1, unsigned short *param_2);                 /* 0x5a9e90 */

/* 0x575030, the income of the meeting (the event 0xa1, ROUGH): the figure +0xc (0x505df0) and the building +0x10 (0x4fea50); the
   amount 0x5092e0 (building, 0x1d, 1) * 0x5ee01c + 0x5eb780, times 0x504ec0 (per, 2) and a random 0x42ec20 curve (0x5eec58 /
   0x5ebbe8), raises per +0xac for each of the count; then, with the random check 0x5eb780 / 0x5eb9f0, the first free word
   +0xc8 .. of the event gets the id 0x5a9e90 (index, ev +0xc8) */
void ai_Unk575030(int ev)
{
    char *per, *bld;
    int v, n, i;
    float f;

    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per == 0)
        return;
    bld = 0;
    gm_FindById(&bld, 0, 0, HM_I(ev, 0x10));
    if (bld == 0)
        return;
    v = gm_BuildingSkill(bld, 0x1d, 1);
    f = (float)v * *(float *)(unsigned)&full_rdata_5ee01c + *(float *)(unsigned)&ai_FloatOne;
    n = gm_GetSkill(per, 2);
    for (i = n; i > 0; i--)
        HM_I(ev, 0xac) = HM_I(ev, 0xac) + (int)((util_RandomF() + f) * 75.0 + *(double *)(unsigned)&ai_Unk5ebbe8) * f;
    if (util_RandomF() + *(float *)(unsigned)&ai_FloatOne >= (-1.58818684e-23f))
        return;
    for (i = 0; i < 4; i++) {
        if (*(unsigned short *)(ev + 0xc8 + i * 2) == 0) {
            *(unsigned short *)(ev + 0xc8 + i * 2) = ai_Unk5a9e90(i, ev + 0xc8);
            break;
        }
    }
}
/* 0x575150, the alloc of the event 0xa3: state 0, the start and the due clock +0x44 / +0x52 = now, the due clock plus random (2)
   hours and random (0x1e) minutes (0x4fe140) */

void he_Unk575150(int ev)
{
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, util_Random(2) & 0xffff, util_Random(0x1e) & 0xffff, 0);
}
/* 0x5751e0, the run of the event 0xa3: -2 / -1 free, also above the state 3 and without the figure +0xc (0x505df0); state + 1, the
   due clock + 3 + random (3) hours (0x4fe140); from the figure's +0x194 a payment -x (x = random (2) + 1, drawn again when below
   +0x194, 0x50cc50) and for the kinds 6 / 7 the text 0x1008 and the message 0x5fb (0x53dda0); when the state passes 3 the due
   clock +0x52 = now */

void he_Unk5751e0(int ev)
{
    char text[0x400];
    char *per;
    int n, x, state;

    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    state = HM_I(ev, 0x70);
    if (state == -1 || state == -2) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (state > 3) {
        he_FreeEvent((int *)ev);
        return;
    }
    if (per == 0) {
        he_FreeEvent((int *)ev);
        return;
    }
    HM_I(ev, 0x70) = state + 1;
    gm_AddTime(ev + 0x52, (util_Random(3) & 0xffff) + 3, 0, 0);
    n = HM_I(per, 0x194);
    if (n >= 1) {
        x = (util_Random(2) & 0xffff) + 1;
        if (x < n)
            x = (util_Random(2) & 0xffff) + 1;
        gm_SendMoneyDelta(HM_I(per, 4), -x);
        if (HM_B(per, 2) == 6 || HM_B(per, 2) == 7) {
            txt_Format(text, 0x1008, x);
            he_ShowNews(HM_I(per, 4), -1, text, 0, 0x5fb, 0);
        }
    }
    if (HM_I(ev, 0x70) > 3)
        *(AITIME *)(ev + 0x52) = ai_Time;
}
/* 0x575320, the alloc of the event 0xa4: state 0, the clocks +0x44 / +0x52 = now, the due clock + random (0x20) hours (0x4fe140) */

void he_Unk575320(int ev)
{
    *(AITIME *)(ev + 0x44) = ai_Time;
    *(AITIME *)(ev + 0x52) = ai_Time;
    HM_I(ev, 0x70) = 0;
    gm_AddTime(ev + 0x52, util_Random(0x20) & 0xffff, 0, 0);
}

void he_Unk575430(char *per);       /* 0x575430 */
void he_Unk575540(char *per);       /* 0x575540 */
void he_Unk575570(char *per);       /* 0x575570 */

/* 0x5753a0, the run of the event 0xa4: -2 / -1 free, also without the figure +0xc (0x505df0); a random 0x42ec20 picks: above
   0x5eb828 0x575430 (per), above 0x5eb7b8 0x575540 (per), else 0x575570 (per); then free */
void he_Unk5753a0(int ev)
{
    char *per;
    float r;
    int state;

    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    state = HM_I(ev, 0x70);
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
        he_Unk575430(per);
    else if (r > (0.400000006f))
        he_Unk575540(per);
    else
        he_Unk575570(per);
    he_FreeEvent((int *)ev);
}
/* 0x575430, the good outcome of the event 0xa4 (ROUGH): a record of the kind 0x5a9f10 (3, 6) is looked up (0x4a5760 (per, 1,
   ..)); with a record of the level +0xe 3 or more the figure's +0xac grows (0x5089c0 / 0x5de518) and the payment 0x4c4830
   (per +4, -1, 0x6f1bc5, the amount) runs; else 0x4c4880 (per +4, -1, 1, 0x6f1bc5, 0, the word) */

void he_Unk575430(char *per)
{
    int w[4];
    int out[4];
    char *rec;

    memset(w, 0, sizeof w);
    w[0] = ai_DomizilGoodInRange(3, 6) & 0xffff;
    memset(out, 0, sizeof out);
    ai_FindWantedItems(per, 1, w, out);
    rec = (char *)out[0];
    if (rec == 0) {
        cm_SendExSellObject(HM_I(per, 4), -1, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0, w[0]);
        return;
    }
    if (HM_I(rec, 0xe) >= 3) {
        gm_GoodsPrice(w[0], *(unsigned char *)(unsigned)&ai_OwnPlayer);
        HM_I(per, 0xac) = HM_I(per, 0xac) + _ftol();
        cm_SendMoveGoodsUnchecked(HM_I(per, 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, HM_I(per, 0xac));
        return;
    }
    cm_SendExSellObject(HM_I(per, 4), -1, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0, w[0]);
}
/* 0x575540, the middle outcome of the event 0xa4: per +4 is charged 3 + random (4) (0x50cc50) */

void he_Unk575540(char *per)
{
    gm_SendMoneyDelta(HM_I(per, 4), (util_Random(4) & 0xffff) + 3);
}
/* 0x575570, the low outcome of the event 0xa4: per +4 pays (0x4c4830) the amount 0x5089c0 (0x2b7, 0x6f1bc5) times
   (2 * random (0x42ec20) + 0x5eb780) */

void he_Unk575570(char *per)
{
    float f;

    f = ((float (*)(int, unsigned char))gm_GoodsPrice)(0x2b7, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    ((int (*)(int, int, unsigned char, int))cm_SendMoveGoodsUnchecked)(HM_I(per, 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, (int)((util_RandomF() * 2.0f + *(float *)(unsigned)&ai_FloatOne) * f));
}
/* 0x5755c0, copies the INI path `s` to 0x6fa0e0 and loads "Game" / "message_filter_mask" from it (GetPrivateProfileIntA) into 0x6f263c */

void he_Unk5755c0(char *s)
{
    strcpy((char *)(unsigned)&full_data_6fa0e0, s);
    U_I((unsigned)&full_data_6f263c) = GetPrivateProfileIntA("Game", "message_filter_mask", 0, (char *)(unsigned)&full_data_6fa0e0);
}
/* 0x575600, writes 0x6f263c as "Game" / "message_filter_mask" into the INI file 0x6fa0e0 (WritePrivateProfileStringA) */

void he_SaveMessageFilter(void)
{
    char buf[0xc];

    sprintf(buf, "%d", U_I((unsigned)&full_data_6f263c));
    WritePrivateProfileStringA("Game", "message_filter_mask", buf, (char *)(unsigned)&full_data_6fa0e0);
}
/* 0x575640, 1 when the message filter mask 0x6f263c has the bit of the message kind id (0x605 the bit 1, 0x606 2, 0x607 4, 0x608 8,
   0x609 0x10, 0x60a 0x20, 0x60b 0x40, 0x60c 0x80, 0x60d 0x100); else 0 */

int he_IsMessageKindFiltered(int id)
{
    switch (id) {
    case 0x605:
        return (U_I((unsigned)&full_data_6f263c) & 1) != 0;
    case 0x606:
        return (U_I((unsigned)&full_data_6f263c) & 2) != 0;
    case 0x607:
        return (U_I((unsigned)&full_data_6f263c) & 4) != 0;
    case 0x608:
        return (U_I((unsigned)&full_data_6f263c) & 8) != 0;
    case 0x609:
        return (U_I((unsigned)&full_data_6f263c) & 0x10) != 0;
    case 0x60a:
        return (U_I((unsigned)&full_data_6f263c) & 0x20) != 0;
    case 0x60b:
        return (U_I((unsigned)&full_data_6f263c) & 0x40) != 0;
    case 0x60c:
        return (U_I((unsigned)&full_data_6f263c) & 0x80) != 0;
    case 0x60d:
        return (U_I((unsigned)&full_data_6f263c) & 0x100) != 0;
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
            U_I((unsigned)&full_data_6f263c) |= 1;
        else
            U_I((unsigned)&full_data_6f263c) &= ~1;
        break;
    case 0x606:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 2;
        else
            U_I((unsigned)&full_data_6f263c) &= ~2;
        break;
    case 0x607:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 4;
        else
            U_I((unsigned)&full_data_6f263c) &= ~4;
        break;
    case 0x608:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 8;
        else
            U_I((unsigned)&full_data_6f263c) &= ~8;
        break;
    case 0x609:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x10;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x10;
        break;
    case 0x60a:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x20;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x20;
        break;
    case 0x60b:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x40;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x40;
        break;
    case 0x60c:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x80;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x80;
        break;
    case 0x60d:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x100;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x100;
        break;
    }
}
