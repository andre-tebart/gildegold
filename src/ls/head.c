/* ls.c -- the save game (0x519010 ..): the tag "ls_" ("ls:tmp" 0x623654, "ls_LoadCharacter(): invalid next or prev-action pointer ...", "ls_SendGame"), the language number of a name (0x519010: the table "GERMAN" .. 0x5ee978), the save game writer 0x5190b0 (header 0x5194c0: "SCENARIO", the player, the party records and the screen shot of 0xe100 bytes, then one writer per subsystem), the pointer <-> index conversions before / after a save (0x519a00 / 0x519aa0 / 0x519b50) and the writers of the building list (0x519bf0) ... The unit starts at its first global 0x62364c (the language number, -1). Drafts of layout 2026-10-23; the unit end is open (the strings run to "ls_SendGame" 0x62376c; 0x51c950 ..?).
   unit of rea 8cef7d5b (src/game/ls.c); boundaries in decl/text.dcl, notes in src/ls/open.md */
struct D3SM;
struct D3TERRAIN;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "f3gm.h"
#include "gv.h"
#include "vfs.h"
#include "he.h"
#include "txt.h"
#include "data_views.h"
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
extern char d3_Scenes[];
extern int cm_SaveAcks[8][2];
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int ai_Unk7d6a40;
extern char ai_Unk7d6a60[];  /* 0x7d6a60, the five 0x2a90-byte stock blocks (f3gm.c's definition) */
#define ai_Unk7d6a64 (*(int (*)[3])((char *)ai_Unk7d6a60 + 4))  /* 0x7d6a64, block 0's header tail (a view) */
extern unsigned char ai_Grid;
extern unsigned ai_Unk91ac40[136];
extern unsigned char ord_Statistics;
extern unsigned char ord_StatisticsClock;
extern char ai_DataPath[608];
extern char ai_Unk925520[];
extern int ai_Unk92bca0;
extern GAPDATA full_data_92bcb3[];
extern int ai_Unk60bd88;
extern int ai_Unk60bd8c;
extern int ai_Unk60bd90;
extern unsigned char ai_Unk9367e0;
extern unsigned char ai_Unka68800[1024];
extern unsigned int ai_Unka68c04;
extern unsigned char ai_Unka69140;
extern char ai_Unkf31160[];
extern unsigned int ai_Unkf8121c;
extern int ai_Unk760548;
extern int ai_Unk760610;
extern GAPDATA full_data_7604e8[];
extern void main_Unk537b60(int kind, int cur, int total);
extern char ls_ClearObjectFlag();
extern char ls_HookBuildingCB();
extern int ai_Unk7d6a40;
extern unsigned char ch_Motions;
extern int ai_Unk7e3f38;
extern int gm_Offers[528];
extern struct G10GAME *ai_Unkf83654;

int ai_Unk61a658 = 1;  /* 0x61a658 */
int ls_Language = 65535;  /* 0x62364c, the language number ( */
int ls_Version = 65626;  /* 0x623650 */
const char ls_LanguageNames[][0x40] = { "GERMAN", "ENGLISH", "FRENCH", "ITALIAN", "SPANISH" };  /* 0x5ee978, the language names (5 of */
extern int gm_PlayerRecords[688];          /* 0x7d5f80, the 16 player records (0xac bytes each; gm_PlayerRecord, f3gm.c) */
/* gm_PlayerRecordBytes: view of gm_PlayerRecords (tools/data_views.py) */
extern char *ls_ActionQueue;            /* 0x631050, the action queue (ch.h: CHACTION *ch_Queue) */
extern char ch_EmptyName[];             /* 0x631090 */
extern char ls_SavePath[640];
extern int d3_CurrentScene;                /* 0x633444 */
extern int ai_JudgeCatholic, ai_JudgeProtestant;  /* 0x6f1bfc / 0x6f1c00 */
extern char * ai_KindBChars[8];             /* 0x6f1c04, the eight slots of the kind 0xb characters (cm.h: char *) */
int * ai_Unk633738 = 0;  /* 0x633738, the scene (its first word the kind: */
extern char * ai_PlayerBuilding;                /* 0x6f2308 */
extern char * ai_FallbackBuilding;                /* 0x6f230c */
extern int sv_PanelShown;                /* 0x6f2310 */
extern unsigned short ai_PlayerId;              /* 0x6f25b8 */
int ai_Unk6f1b88 = 0;  /* 0x6f1b88 .. */
int ai_Unk6f1bcc = 0;  /* 0x6f1bcc */
unsigned char ls_SaveSlot = 0;  /* 0x6f1c38, the slot the save header records */
extern int ai_Difficulty;                /* 0x6f23fc */
extern unsigned short ai_ScreenImage[19200];
/* ai_Unk76fa24: view of cm_SaveAcks (tools/data_views.py) */
extern char ai_PlayerSlots[4][0x2f4];      /* 0x7e3f40, the player slots */
extern short *ai_Objects;             /* 0x7e3f34 */
extern char ai_Time[];             /* 0x770ae0, the clock */
extern int ai_UpdateStart;             /* 0xf89580 */
extern int ai_NextObjectId;                /* 0x61a654 */
int ai_Unk6f2618 = 0;  /* 0x6f2618 */
extern int ai_Unk6f1bb8, ai_Unk6f1bbc, ai_CharCount, ai_Unk6f1bcc, ai_Unk6f1b88;   /* 0x6f1bb8, 0x6f1bbc, 0x6f1bc0, 0x6f1bcc, 0x6f1b88 .. */
/* ls_RefTargets: view of ai_Unkf31160 (tools/data_views.py) */
int vfs_fclose(struct VFSFILE *f);                    /* 0x430aa0 */
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                     /* 0x430470 */
void mem_Free(void *p);                    /* 0x422620 */
void dd_SplitRGB();                    /* 0x41e6d0 */
void d3_SaveScene(char *name, struct VFSFILE *f);                     /* 0x44aa10 */
int gm_FindObjectInst(int id);                     /* 0x4fe980 */
char * gm_FindBuildingById(int id);                     /* 0x502020 */
int cm_PartyById(int id);               /* 0x4e50a0, cm.h's prototype (that header redefines gv.h's globals here) */
int gm_GetWorth(unsigned short id);                     /* 0x50a640 */
int gm_ConvertMoney(int a, unsigned char k);                     /* 0x5085b0 */
int ls_WriteSaveHeader();                     /* 0x5194c0 */
void unk_Unk519a00();                    /* 0x519a00 */
void unk_Unk519aa0();                    /* 0x519aa0 */
int unk_Unk519b50();                     /* 0x519b50 */
int ls_WriteObjects();                     /* 0x519bf0 */
int unk_Unk519d20();                     /* 0x519d20 */
int unk_Unk51a1a0();                     /* 0x51a1a0 */
int unk_Unk51a4b0();                     /* 0x51a4b0 */
int unk_Unk51b340();                     /* 0x51b340 */
int amt_SaveAemter();                     /* 0x4ea690 */
int unk_Unk4f28d0();                     /* 0x4f28d0 */
int ls_WriteRelations();                     /* 0x51b890 */
int unk_Unk51b910();                     /* 0x51b910 */
int unk_Unk51bdc0();                     /* 0x51bdc0 */
int unk_Unk51c2f0();                     /* 0x51c2f0 */
int ls_WriteHistory();                     /* 0x51c450 */
int unk_Unk51c560();                     /* 0x51c560 */
int unk_Unk51c870();                     /* 0x51c870 */
int unk_Unk51aff0();                     /* 0x51aff0 */
int ai_Unk4b62e0();                     /* 0x4b62e0 */
int ai_Unk589aa0();                     /* 0x589aa0 */
int ai_Unk5a8940();                     /* 0x5a8940 */
int ahm_SaveWalk();                     /* 0x4ebfb0 */
int hmt_Save();                     /* 0x4adb20 */
int rt_SaveRoutes();                     /* 0x4bff40 */

#define LS_W(p, sz) if (vfs_fwrite((p), (sz), 1, f) <= 0) return 0;
extern int ai_NBauplaetze;                /* 0xf812e0 */
extern struct G15BK *ai_Bauplaetze;              /* 0xf812e4 */
int unk_Unk4f17f0();                     /* 0x4f17f0 */
int ahm_WriteRecord();                     /* 0x4ecf10 */

#define LS_R(p, sz) if (ls_Read((int)(p), (sz), 1, f) == 0) return 0;
extern int ai_Unk76f920[1];
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                     /* 0x430470 */
DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b);          /* 0x41e670, an RGB triple to a 16 bit pixel */

extern unsigned char init_Detail;      /* 0x61adf8, the detail level */
extern int f3gm_ObjectCount;                /* 0x6f1bdc */
extern char *ai_Unk5fb5b0;              /* 0x5fb5b0, the current scene */
struct D3SM * d3sm_Create(struct D3TERRAIN *t, int size, int mask);  /* 0x45eeb0, d3_sm.h */
char d3_LoadScene(char *name, void *f, void (*progress)(int a, int i, int n));                     /* 0x44be60 */
char d3if_ClearScene(int index);                    /* 0x447e60 */
void d3fl_SetDetail(unsigned int n);                    /* 0x43f8a0 */
int ahm_ReadRecord(void *f, unsigned char *p);  /* 0x4ed040, ahm.h's prototype (that header's `const char *tag` for mem_Alloc conflicts with gv.h) */

struct CHARACTER *ch_AllocAt(int id);               /* 0x402a30, ch.h's prototype (that header needs d3.h, which conflicts with gv.h here) */
extern char *txt_Ptrs[];           /* 0x143a1c0, the text table */
extern int hst_TextCurrent, hst_TextFirst, hst_KeyNext, hst_KeyFirst;   /* 0x601060, 0x601058, 0x60106c, 0x601064 .. 0x60106c */
extern unsigned char hst_TextsOn;               /* 0x6f0724 */
extern char hst_Key[];                 /* 0xf83520, the history key ("G12", 0x20 bytes; the save keeps 0x10) */
extern int hst_Lists[];                /* 0xf83540, the four history lists (G12ROW[4] in hst.h), int view */
void init_HookObject(char *o, unsigned short *p, int kind);  /* 0x50d380, names the scene object o and hooks it to its owner p */
int ai_LoadWind();                     /* 0x5dc0c0 */
int unk_Unk4ed910();                     /* 0x4ed910 */
int gm_FindById();                     /* 0x4fea50 */

extern struct AIBLD *ai_MarketBuilding;                /* 0x6f1bc8 */
extern int ai_GameState;      /* 0x6f25ac */
void sv_CountGroupMembers(char *g);     /* 0x5241d0, sv.c */
/* ai_Unk6f261c: view of ai_Unk6f2618 (tools/data_views.py) */
int ai_Unk6f2620 = 0;  /* 0x6f2620 / */
unsigned char he_Events[339968] = {0};  /* 0x6fab40, the */
char * he_QuickList[10921] = {0};  /* 0x74db40, pointers to the */
int ai_Unk7585e4 = 0;  /* 0x7585e4 */
void (*he_RunHandlers[5544])(unsigned char *) = {0};  /* 0x758600, the event run handlers by type (he.c */
int ai_Unk75e134 = 0;  /* 0x75e134 */
int ai_Unk75e138 = 0;  /* 0x75e138 .. the clock copy */

extern int ai_Unk6f2620, ai_Unk7585e4;   /* 0x6f2620, 0x7585e4 */
extern struct G17MSG *ai_Unka68d24;  /* 0xa68d24 */
extern struct G17MSG *ai_Unka68c00;  /* 0xa68c00 */
extern struct G17MSG *cm_SendQueue;  /* 0xf81160 */
extern int ai_Unkf81168;  /* 0xf81168 */
void he_Unk53d590();                    /* 0x53d590 */
extern int d3_CurrentScene;             /* 0x633444, the current scene index (d3.h) */
char d3if_SelectScene(int index, char keep);  /* 0x4478e0, d3_interface.c */

/* ai_Unk1469700: view of ch_Motions (tools/data_views.py) */
char *strstr();                   /* 0x5de6b0, strstr-like (the "hozfaeller" misspelling check) */

extern unsigned ai_Unkf81220, ai_Unka68d28;  /* 0xf81220 / 0xa68d28, the received bytes and the expected length (compared unsigned, 0x520ba1 jae) */
int ai_Unk430fa0(struct VFSFILE *f, long offset, int origin);                    /* 0x430fa0, fseek */
int ai_Unk431170();                     /* 0x431170, file length */
void cm_SendLoadSize(int size);         /* 0x4c44d0, cm.h's prototype (that header redefines gv.h's globals in this TU) */
int cm_SendLoadBlock(void *p);          /* 0x4c4500, the 0x80-byte block of the load buffer; -1: send ring full */
int sv_PumpMessages(void);              /* 0x52fb80, sv.c: pumps the window messages */
void sv_PumpNet(void);                  /* 0x52fe00, sv.c: sends the queued messages, receives and runs the waiting ones */
#define ai_BauplaetzeV (*(char * (*))&ai_Bauplaetze)  /* 0xf812e4, this unit reads it as `char *ai_Bauplaetze` */
#define ai_FallbackBuildingI (*(int (*))&ai_FallbackBuilding)  /* 0x6f230c, this unit reads it as `int ai_FallbackBuilding` */
#define ai_GameStateB (*(unsigned char (*))&ai_GameState)  /* 0x6f25ac, this unit reads it as `unsigned char ai_GameState` */
#define ai_KindBCharsV (*(int * (*)[])&ai_KindBChars)  /* 0x6f1c04, this unit reads it as `int *ai_KindBChars[]` */
#define ai_MarketBuildingI (*(int (*))&ai_MarketBuilding)  /* 0x6f1bc8, this unit reads it as `int ai_MarketBuilding` */
#define ai_PlayerBuildingI (*(int (*))&ai_PlayerBuilding)  /* 0x6f2308, this unit reads it as `int ai_PlayerBuilding` */
#define ai_PlayerSlotsV (*(char (*)[][0x2f4])&ai_PlayerSlots)  /* 0x7e3f40, this unit reads it as `char ai_PlayerSlots[][0x2f4]` */
#define ai_Unk1468ae0V (*(int (*)[])&ai_Unk1468ae0)  /* 0x1468ae0, this unit reads it as `int ai_Unk1468ae0[]` */
#define ai_Unk75e130I (*(int (*))&ai_Unk75e130)  /* 0x75e130, this unit reads it as `int ai_Unk75e130` */
#define ai_Unka68c00I (*(int (*))&ai_Unka68c00)  /* 0xa68c00, this unit reads it as `int ai_Unka68c00` */
#define ai_Unka68d24I (*(int (*))&ai_Unka68d24)  /* 0xa68d24, this unit reads it as `int ai_Unka68d24` */
#define ai_Unkf31140I (*(int (*))&ai_Unkf31140)  /* 0xf31140, this unit reads it as `int ai_Unkf31140` */
#define ai_UpdateStartV (*(char (*)[])&ai_UpdateStart)  /* 0xf89580, this unit reads it as `char ai_UpdateStart[]` */
#define cm_SendQueueI (*(int (*))&cm_SendQueue)  /* 0xf81160, this unit reads it as `int cm_SendQueue` */
#define gm_PlayerRecordsV (*(int (*)[])&gm_PlayerRecords)  /* 0x7d5f80, this unit reads it as `int gm_PlayerRecords[]` */
