/* he.c -- the event bar ("Panel\Event_Leiste", the tag "he_": "he_AllocNone(): Invalid HE-Type : %i, von Spieler %s" 0x626680): the "HE" (history event?) windows, 16 slots of 12 bytes at 0x6faa80
   (owner record, window id, panel handle), the module init table (0x53d4b0), the event messages with quick jump. Starts at 0x53d430, right after the program entry of main.c (0x53ccf0 .. 0x53d426); the globals
   0x625718 / 0x62571c head its .data block (layout 2026-10-23, session 5). ROUGH drafts.
   (no @bytes: the block after he_Unk555b60's ret at 0x556055 is its int 3 error path, reached by the je at 0x555dd7)
   unit of rea 8cef7d5b (src/game/he.c); boundaries in decl/text.dcl, notes in src/he/open.md */
struct AHMSLOT;
struct D3HMAP;
struct SBBANK;
struct SBSAMPLE;
union AHMGRID;
struct D3SM;
extern char d2_Wins[];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <d3d.h>
#include "he.h"
#include "d3.h"
#include "d3_quad.h"
#include "d3.h"
#include "d3_light.h"
#include "txt.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#define GAPDATA __declspec(align(1)) unsigned char
#define d3t_CreateEmitterK ((int (*)())d3t_CreateEmitter)  /* 0x415ad0 (d3_tools.c): @original he_Unk54fc00 / he_Unk54fc70 call it without a prototype, the floats as dword bit patterns (draft: 19 instructions this way, 29 through the prototype) */
#define ai_Unk5a9240K ((int (*)())ai_Unk5a9240)  /* 0x5a9240: @original the call passes more arguments than the draft definition names, compiled without a prototype (as before the definition's spelling reached this unit) */
#define ai_Unk5a9c30K ((int (*)())ai_Unk5a9c30)  /* 0x5a9c30: @original the call passes more arguments than the draft definition names, compiled without a prototype (as before the definition's spelling reached this unit) */

extern void main_GameLoop(int ctl);
extern int ai_TransportWindow();
/* full rebuild: the symbols whose original addresses the code above spells literally */
extern char * he_QuickList[10921];
extern void (*he_RunHandlers[5544])(unsigned char *);
extern unsigned char he_Events[339968];
extern unsigned char ai_TimeHour;
extern GAPDATA full_data_6faa80[];
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_625718[];
extern GAPDATA full_data_6faa60[];
extern GAPDATA full_data_6faa84[];
extern GAPDATA full_data_6f262c[];
extern GAPDATA full_data_6faa90[];
extern short d2_ScreenH;
extern short d2_ScreenW;
extern GAPDATA full_data_62571c[];
extern GAPDATA full_data_6faa88[];
extern int ai_GameFlags;
extern int tb_Paused;
extern int ai_Unk6f2618;
extern int ai_Unk6f2620;
extern int ai_Unk7585e4;
extern GAPDATA full_data_625708[];
extern GAPDATA full_data_62570c[];
extern GAPDATA full_data_625710[];
extern GAPDATA full_data_625714[];
extern GAPDATA full_data_6f2624[];
extern GAPDATA full_data_6f2628[];
extern const float ai_Unk5ebba4[];
extern unsigned char ai_GameSwitches;
extern unsigned short ai_PlayerId;
extern char ai_Chars[768][0x21c];
extern char ai_DataPath[608];
extern unsigned short * ai_PlayerChar;
extern char * ai_BTypes;
extern char * ai_Blds;
extern int ai_Difficulty;
extern unsigned char ai_OwnPlayer;
extern const int ai_Unk5eeab8[24];
extern char ch_EmptyName[1];
extern char * cm_FieldObject;
extern unsigned char ai_Unk5fb5b0;
extern const int ai_Unk5ed100[16];
extern signed char ai_Relations[768][768];
extern char ai_Unk625820[115][0x20];
extern int ai_GameState;
extern int d2_ClickedId;
extern struct D2WIN *d2_LastWindowPtr;
extern int d2_Unkf8aac4;
extern int snd_SystemBank;
extern char ai_PlayerSlots[4][0x2f4];
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_5ff710[];

extern char ai_Chars[768][0x21c];
extern int ai_Unk6f2448;
extern GAPDATA full_data_5f320c[];
extern char * ai_BTypes;
extern char * ai_Blds;
extern const int ai_Unk5ed140[16];
extern int gm_PlayerRecords[688];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_Unkf8aac4;
extern unsigned short ai_PlayerId;
extern unsigned char ai_TimeHour;
extern int ai_GameState;
extern char ai_PlayerSlots[4][0x2f4];
extern GAPDATA full_data_6f262c[];
extern GAPDATA full_data_6f2630[];
extern char * cm_FieldObject;
extern GAPDATA full_data_5fe7d0[];
extern signed char ai_Relations[768][768];
extern const int ai_Unk5ed100[16];
extern char ai_Unk91f700[];
extern const int ai_Unk5eeab8[24];
extern int ai_Unk6333cc;
extern char ch_EmptyName[1];
extern unsigned short * ai_PlayerChar;
extern float ai_Unk5eb780;
extern char ai_Unk625820[115][0x20];
extern const char ai_AxisPair[];
extern GAPDATA full_data_60d0d4[];
extern GAPDATA full_data_6f2634[];
extern unsigned char snd_TrackHandles;
extern int * ai_Unk760618;
extern int ai_Unk76061c;
extern int ai_GameOver;
extern char * ai_Goods;
extern GAPDATA full_data_5f1890[];
extern int ai_Unk6f2318;
extern unsigned char ai_Unk5fb5b0;
extern char * ai_PlayerBuilding;
extern int ai_Unk75f280[576];
extern int sv_FrameDepth;
extern unsigned short * sv_PanelShown;
extern char * ai_FallbackBuilding;

extern char ai_Chars[768][0x21c];
extern char * ai_FallbackBuilding;
extern int ai_GameFlags;
extern int ai_GameState;
extern char * ai_PlayerBuilding;
extern unsigned short ai_PlayerId;
extern int snd_SystemBank;
extern char * ai_BTypes;
extern char * cm_FieldObject;
extern const int ai_Unk5eeab8[24];
extern GAPDATA full_rdata_5ebb44[];
extern double ai_Unk5eb990;
extern GAPDATA full_rdata_5ee020[];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern unsigned short * ai_PlayerChar;
extern unsigned char ai_TimeHour;
extern unsigned ai_Unk760488[0x18];
extern GAPDATA full_data_7604e8[];
extern GAPDATA full_data_627b0c[];
extern char ai_Unk61aa38[24];
extern GAPDATA full_data_627b20[];
extern char ch_EmptyName[1];
extern float ai_Unk617830[];
extern unsigned char ai_GameSwitches;
extern float ai_Unk617840[];
extern GAPDATA full_data_6279f0[];
extern GAPDATA full_data_627a10[];
extern GAPDATA full_data_627d74[];
extern GAPDATA full_data_6fa260[];
extern GAPDATA full_data_6faa60[];
extern GAPDATA full_data_6fa264[];
extern GAPDATA full_data_6fa268[];
extern unsigned char ai_GamePanel;

extern GAPDATA full_data_6fa260[];
extern GAPDATA full_data_6fa264[];
extern GAPDATA full_data_6fa268[];
extern GAPDATA full_data_6fa26c[];
extern GAPDATA full_data_6fa1e0[];
extern unsigned char ai_GamePanel;
extern unsigned short ai_PlayerId;

extern GAPDATA full_data_6fa1e0[];
extern char ai_Chars[768][0x21c];
extern int ai_GameState;
extern struct AIBLD *ai_MarketBuilding;
extern unsigned char ai_TimeHour;
extern int ai_Unk6f2438;
extern char ch_EmptyName[1];
extern unsigned char ai_OwnPlayer;
extern char * cm_FieldObject;
extern GAPDATA full_data_6f2638[];
extern char * ai_Goods;
extern int gm_PlayerRecords[688];
extern unsigned short * ai_PlayerChar;
extern GAPDATA full_data_628a28[];
extern char * ai_BTypes;
extern char ai_Unkf31160[];
extern unsigned short ai_Unk76fdc4;
extern unsigned ai_Unk889400[732];
extern int d2_ClickedId;
extern int d2_Unkf8aac4;
extern unsigned char ai_Unk5fb5b0;
extern int d3_QuadTree;
extern GAPDATA full_data_627e98[];
extern GAPDATA full_data_627ea8[];
extern char ai_MissionMode;
extern int ai_MissionPartner;
extern unsigned char ai_Missions;
extern float ai_Unk617830[];
extern float ai_Unk617840[];
extern float ai_Unk5eb788;
extern float ai_Unk5eb780;
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
extern int d2_Unkf8aac4;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_628ecc[];
extern const int ai_Unk5eeab8[24];
extern float ai_Unk617830[];
extern float ai_Unk617840[];
extern double ai_Unk5ece60;
extern const float ai_Unk5ee160[476];
extern char ch_EmptyName[1];
extern char * cm_FieldObject;
extern GAPDATA full_rdata_5eb8a8[];
extern char * ai_Blds;
extern unsigned short * ai_PlayerChar;
extern GAPDATA full_data_628f3c[];
extern GAPDATA full_data_628f74[];
extern GAPDATA full_data_628fac[];
extern const float unk_Tab5ee034[];
extern GAPDATA full_data_628fe4[];
extern float ai_Unk5eb94c;
extern GAPDATA full_data_629000[];
extern GAPDATA full_rdata_5eb7e8[];
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_62901c[];
extern GAPDATA full_data_629038[];
extern unsigned char ai_TimeHour;
extern GAPDATA full_data_629054[];
extern int ai_Unk6f2448;
extern GAPDATA full_data_629070[];
extern float ai_Unk5eb780;
extern double ai_Unk5ebbe8;
extern GAPDATA full_rdata_5ee01c[];
extern GAPDATA full_data_6f263c[];
extern GAPDATA full_data_6fa0e0[];

char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);
void gm_Shuffled(unsigned char n, int *out);
void amt_ChangeDistrictOpinion();
void gm_Shuffled(unsigned char n, int *out);
struct AIPER; struct AMTREC; struct CHARACTER; struct CHACTION; struct EVTSCRIPT; struct G15PLANT; struct G11REC; struct GCLOCK;
void cm_SendProofExecuted(int a, int b);
void ai_Unk4c5230(int a);
void cm_SendFieldDeltas(void);
void cm_SendRelations(int a, int b, int c, int d);
int util_Random(unsigned short n);      /* 0x42ec00, tr.c's definition: this TU's callers were compiled against the 16-bit parameter and result */

int gm_PlayerFactor(int a, unsigned char k);
void gm_SetTime(struct GCLOCK *c, unsigned char hour, unsigned min, unsigned sec);

#define U_I(a) (*(int *)(a))

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_Unk4a63f0)(p, a, fl, b, c, f, o)

#pragma pack(push, 2)
typedef struct HETIME {            /* the 14 byte game time (day, hour, minute, ..: ai.h AITIME) */
    int a;
    unsigned short b;
    int c;
    int d;
} HETIME;
#pragma pack(pop)
extern HETIME ai_Time;                  /* 0x770ae0 (ai.h) */
#define SLOT(i) ((int *)((unsigned)&full_data_6faa80 + (i) * 12))

void d3io_NoopCallback(); int cm_AddFieldDelta(int size, int n, short field, void *v); void cm_BeginFieldBlock(char *o, int id); float gm_Mix(int a, int b, int c);
int he_Unk553060(void);
int he_Unk568310();
int ai_Unk56d050();
int he_Unk56f980();
int ai_Unk56d220();
int ai_Unk56e400();
int ai_Unk5733c0();
int ai_Unk573260();

int sv_ReturnZero(void);
int gm_CompareTime();
int d2_SetForm(int view, int n);
struct AMTKIND; struct D3VECTOR; int d2_WinOpenPic(short x, short y, int pic, int win);
int d2_OpenForm();
void d2_PlaceMainWindow();
void d2_SetViewEnabled(int view, int on);
void d2_RemoveObj();
void d2_CloseView(int view);
void d2_CentreViewWindows(int view);
void d2_MoveObj(short x, short y, int obj);
void d2_SetObjValue();
void d2_RaiseViewWindows(int view);

#include "time_view.h"
#define ai_CharsV (*(char (*)[])&ai_Chars)  /* 0x770b80, this unit reads it as `char ai_Chars[]` */
