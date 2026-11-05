/* he.h -- the event bar (0x53d430 ..), see src/game/he.c */
#ifndef HE_H
#define HE_H

struct D3OBJECT;
struct AIJOB;
struct AICHAR;
struct AIBLD;
struct AIOBJ;

typedef struct HEGFXINFO {              /* 16 bytes, a gfx info: a 3D model shown over an object for an event (he_CreateGfxInfo) */
    int eventId;                        /* 0x0, the event's AIJOB.id */
    struct AIJOB *ev;                   /* 0x4, the event */
    struct D3OBJECT *parent;            /* 0x8, the object it is shown over (he_CreateGfxObject sets it), 0: a free entry */
    struct D3OBJECT *obj;               /* 0xc, its model object (he_CreateGfxObject) */
} HEGFXINFO;

void he_AllocNone(unsigned char *ev);
void he_RunNone(unsigned char *ev);
int he_InitEvents(void);
void he_FreeEvents(void);
int he_ReleaseEvents(void);
int he_FigureContact(int p);                 /* 0x55ea60, the contact id of the figure p, or -1 */
int he_SetEventHandlers(int type, void *alloc, void *run);
int he_FreeOldestSlot(void);
int he_OpenEventSlot(int ev, int x, int y, char *form, int pic);
void he_ReleaseSlot(int ev);
void he_CreateEventBar(void);
void he_CloseEventSlots(void);
void he_ShowEventBar(int pl);
void he_SetBarEnabled(int state);
void he_PickActiveEvent(void);
void he_ActivateSlot(int *sl);

typedef struct HEEVSLOT {               /* 12 bytes, a slot of the event bar (he_EventSlots) */
    struct AIJOB *ev;                   /* 0x00, the event record the slot shows (ai.h AIJOB: its kind at +0) */
    int obj;                            /* 0x04, the slot's d2 object, -1 = the slot is free (0x53d6f0) */
    int win;                            /* 0x08, the event window (0x53dcb0 enables / raises it) */
} HEEVSLOT;
extern HEEVSLOT he_EventSlots[16];      /* 0x6faa80 */
extern HEEVSLOT *he_ActiveSlot;         /* 0x6f262c, the slot of the active event window (0x53dcb0, 0x53daf0), 0 = none */
extern unsigned char he_SlotConfirmed;  /* 0x6f2630, set by a right click / Enter on the active window of the
                                           ceremony kinds 0x11 / 0x26 / 0x69 (0x52d940); their run handlers
                                           proceed on it and clear it */
extern struct AIBLD *he_PatrolBuilding;      /* 0x6f2638, the building kind 0x44 the guard patrol event 0xa6 runs
                                                from (0x568220 looks it up; 0x567f50 compares a cart's building with it) */
int he_ShowNews(int pid, int from, char *text, int tflags, int news, char *text2);
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2);

unsigned char * he_CreateEvent(unsigned char *m);
void he_FreeEvent(int *r);
char *he_NextEvent(void);
char *he_FindEvent(int n, ...);
int he_NoEvent9aFor(int a, int b);              /* 0x56f360, 1 when no running type 0x9a event has both +0xac == a and +0xb0 == b + 4 */
int he_NoEvent9bFor(int a);                     /* 0x56f3b0, 1 when no running type 0x9b event has +0xac == a + 4 */
int he_RoomBusy(struct AIBLD *b, unsigned short kind);     /* 0x56ddc0, 1 when the room `kind` of the building b is locked */
int he_FindMeetingRoom(int ev);                 /* 0x574fe0, the first building of the meeting event's room classes, 0 when none */
void he_ShowConstruction(int ev);               /* 0x56b780, the construction state of the building event ev */
void he_SpreadGfxInfos(struct D3OBJECT *obj);
struct D3OBJECT *he_CreateGfxObject(HEGFXINFO *e, char *name, struct D3OBJECT *obj);
void he_CreateGfxInfo(struct AIJOB *ev, char *name, struct D3OBJECT *parent);
void he_FreeGfxInfo(int *e);
void he_AddEventGfxInfo(struct AIJOB *ev);
void he_RemoveStaleGfxInfos(void);
void he_RemoveGfxInfos(int obj);
void he_ShowObjectInfos(int obj, int a);
void he_RecheckEventPerson(unsigned char *r);
void he_RecheckPersonEvents(unsigned short *pl);
void he_RunEvents(void);
void he_RunQuickList(void);
void he_RunEveryFrameEvents(void);
void he_AgeCarryRecs(void);

void he_CountDownApEvents(void);
#pragma pack(push, 1)
typedef struct HEAPEVENT {              /* 0x50 bytes, an AP event (0x100 of them at 0x758ca0, he_AddApEvent) */
    char counter;                       /* 0x00, rounds left (he_CountDownApEvents counts it down, 2 at the start) */
    char pad01[3];
    int key;                            /* 0x04, the character it belongs to */
    int income;                         /* 0x08, summed by he_SumApEvents into the round's money (main.c 0x537de0) */
    int cost;                           /* 0x0c, summed by he_SumApEventCosts and taken from the player at the round change */
    char name[0x40];                    /* 0x10, 0x3f characters */
} HEAPEVENT;
#pragma pack(pop)
#define he_ApEvents (*(HEAPEVENT (*)[0x100])((char *)&he_RunHandlers + 0x6a0))  /* 0x758ca0, the AP events (ls.c's he_RunHandlers block holds them) */
int he_SumApEvents(int key);
HEAPEVENT *he_AddApEvent(int key, char *name, int income, int cost);  /* 0x53eec0, registers an AP event, returns it (0 when full) */
int he_SumApEventCosts(int key);
void he_PostEvent6d(void);
void he_PostEvent6b(void);
void he_PostEvent7a(void);
void he_PostEvent2b(void);
void he_PostPlayerEvents86a2(void);
void he_PostWorkshopEvents98(void);
void he_InnStaffLeave(void);
void he_ClearInnDarkCorner(void);
void he_TopUpCharMoney(void);

void he_PostEvent67(void);
void he_InnReport(void);
void he_DailyEventChecks(void);
void he_PostEvent3a(void);
void he_PostEvent3b(void);
void he_PostEvent73(void);
void he_PostEventA6(void);
void he_EndEventNow(int r);
void he_SetEventChar(int r, int pl);



void he_CharacterBehavior(struct AIJOB *ev, float *pos);
void he_CharAccident(struct AIJOB *ev, struct AICHAR *c, struct AICHAR *boss, struct AIBLD *bld, float value, int quiet); /* 0x5414a0, a character's accident: carried away ("Unfall") or dead, with the boss's news */
void he_CharIllness(struct AIJOB *ev, struct AICHAR *c, struct AICHAR *boss, struct AIBLD *bld, int disease, float value); /* 0x5417d0, a character's illness: carried away ("Unfall") or dead, with the boss's news */
void he_AllocStammtisch(int r);

void he_Unk544330(int ev);
void he_Unk544550(int ev, unsigned short *p2, unsigned short *p3);
void he_Unk544630(int ev);
void he_Unk5471e0(int ev, unsigned short *p2, unsigned short *p3);

void he_Unk5437c0(int ev);

void he_Unk545640(int ev);

void he_Unk545670(int ev);

void he_Unk544070(int ev);

void he_SpawnFounders(void);
extern const float he_FounderOdds[5];  /* 0x5eebc4 */

void he_StammtischCommand(unsigned cmd, struct AIBLD *inn, struct AIOBJ *table, int figId); /* 0x541b40, the inn meeting hears of a table request: join / leav / cler / exec / new */

void he_AllocTavern(int ev);

void he_RunTavern(int ev);

void he_AllocQuarrel(int ev);
void he_Unk543180(int ev);

void he_RunMasterExam(int ev);

void he_Unk5436a0(int ev);

void he_AllocOffer(int ev);

void he_RunWeddingOffer(int ev);

void he_RunStammtisch(int ev);

void he_SaveMessageFilter(void);                                       /* 0x575600 */

/* declared in other units' headers before (moved to the owner) */
int he_Unk553cc0(int p);  /* 0x553cc0 */
int he_NoEvent2ePair(int a, int b);  /* 0x553d10 */
int he_NoEvent4bPair(int a, int b);  /* 0x553d60 */
int he_EventOnBuilding(int owner);  /* 0x56f170 */
int he_NoEvent88Pair(int a, int b);  /* 0x570ea0 */
int he_IsMessageKindFiltered(int id);  /* 0x575640 */
int he_RandomTransporter(char *obj);  /* 0x555210 */
char *ai_FindCurseEvent(char *a);  /* 0x5741d0 */
int he_HasGraveWork(char *bld, int size, int index);  /* 0x573200, 1 when a work event 0x8d of bld is pending on the grave (size, index) */
int he_NoEvent35Pair(unsigned short *a, int b, int *count);  /* 0x553db0 */

extern unsigned char he_FindKind;       /* 0x625708, he_FindEvent's filter on AIJOB.kind (key 0), 0xff = any */
extern int he_FindId;                   /* 0x62570c, its filter on AIJOB.id (key 1), -1 = any */
extern unsigned short he_FindChr;       /* 0x625710, its filter on AIJOB.chr (key 2), 0xffff = any */
extern int he_FindFrom;                 /* 0x625714, its filter on AIJOB.from (key 3: the sending building), -1 = any */
extern int he_FindUnused;               /* 0x6f2628, set 0 by he_FindEvent; nothing reads it (the name says only that) */
extern char he_FindFirst;               /* 0x6f2624, 1 while he_Cursor stands on the first record he_FindEvent found:
                                           he_NextEvent tests it before stepping on */
extern int he_LastSlot;                 /* 0x625718, the highest slot index given an event (he_OpenEventSlot), -1 = none; sv_UpdateCityPanel places the bar's end by it */
extern int he_BarShown;                 /* 0x62571c, he_BarView while the event bar is shown (he_ShowEventBar), -1 = hidden */
extern int he_BarView;                  /* 0x6faa60, the view "Panel\Event_Leiste" (he_CreateEventBar opens it) */

#endif
