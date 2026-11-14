/* sv_t.c -- the tutorial module ("FT*" record names, tutorial\* forms, TUTORIAL_* texts), 0x575860..0x589f10:
   the tutorial state machine (state record ai_Tutorial = the pointer 0x5eec60, states 1 .. 0xc, the jump table
   0x575fe8; ai_TutorialStart builds the run chain ai_TutorialRuns and initializes it, the tick 0x575860 runs it),
   the chapter run-record builders (ai_TutorialChapterInit) with their step tables (TUTSTEP) and position-marker
   paths (TUTMARK), the marker update, the chapter chooser, then the location / contact / tooltip windows (the
   alchemist's candidates and hire from 0x57bf00 on), the cannon scene, the cabinet's spy windows (kabinett_spy /
   kabinett, message 0x99), the room contacts (ai_AlchemistLabContact, ai_CannonContact, ai_CabinetContact) and the
   graveyard windows (ai_GraveWindow with its size buttons, list and detail; the rent-a-grave detail; the move /
   rent / free confirmation forms and the GRAEBER_PFLEGEN / GEHILFEN_ERSCHAFFEN callbacks ai_GraveCare /
   ai_GraveRaiseHelper), the helpers window ai_GypsyHelperWindow, the graveyard chatter tick, the right-button
   tooltips (ai_HandleItemDrag and its ai_Open*Tooltip views), the character info window ai_CharInfoWindow, the
   function-key building bookmarks (ai_Bookmarks: the keys, ai_BookmarkWindow, the save game steps), the
   screenshot and the speed keys. Start: he.c's last function 0x575710, its jump table and
   nop fill end at 0x575860; the unit's one .data block starts at 0x62918c (ai_TutorialChapterOn, then the "FT"
   state names) and its literals climb in function order to 0x62aa58; the network lobby (0x589f10) is a unit of
   its own. The original file name is unknown (no leaked tag). Names are ours.
   Open: type debt - TUTSTATE / TUTRUN / TUTSTEP / TUTMARK are typed in sv_t.h and the tutorial code up
   to ai_TutorialWindow and the chapter transition callbacks use them; most later functions still reach the
   state record through the S / U_I byte-offset macros; the message (AIMSG, ai.h) is typed from
   ai_CabinetSpyWindow on, earlier senders and the contact-list records still use byte offsets; the cannon
   scene struct (CANNONSCENE, CANNONREC) is typed from ai_CannonScene to ai_CannonBuildPanel.
   Open: d2_CreatePicObj: d2.h declares int parameters, the original's callers here (and d2.c's) pass a short y
   (ai_CannonBuildPanel through the cast macro d2_CreatePicObjS).
   Open: data: the unit's .rdata block 0x5eec60..0x5eed70 is typed up to ai_CannonZoomRange (0x5eec6c .. 0x5eecac:
   const data, the original loads it); 0x5eec64 / 0x5eec68 / 0x5eec8c / 0x5eec90 (0x54, the recoil time the code
   uses as an immediate) / the floats 0x5eec9c .. 0x5eeca8 and the tables from 0x5eecf0 are not declared yet (the
   grave size tables 0x5eecb0 .. 0x5eecef are).
   Open: data: ai_DragWindow (0x62a8cc) lies in this unit's .data block between its literals and next to
   ai_DragObj / ai_DragViewWin, but sv.c defines it (sv.h declares it; sv.c and lc.c use it): owner
   sv_t or sv.
   Open: GVGRAVE.v2c / v30 / nameId: gv.c stores small numbers there, ai_GraveDeadName reads v2c as the office
   (1 .. 9, gv.c's prevOffice / random) and ai_GraveDeadDescription v30 / nameId as text ids; which texts the
   bases 0x114 / 0x130 / 0x11e / 0x18f (and the office ones 0x253 / 0x27b) name is unknown.
   Open: sv_CheckGoodAmount's third parameter: the original's callers push the player byte unextended (a char
   parameter, kept as the cast macro sv_CheckGoodAmountB), sv.h / sv.c declare it unsigned and mask it.
   Open: d3_Time is volatile in the original (ai_TutorialTick reloads it after every read; the step callbacks
   and the marker update load it in the original's order only through the unit's view d3_Time); its
   owner and the other units declare it plain int.
   Open: head: cm.h's declaration set still moves the chapter builders by the declaration set alone (a register
   tie-break, recorded in ledger/tiebreak.tsv: ai_TutorialInitChapter2 / ai_TutorialInitChapter3 shape); cm.h's
   declaration set is not the original's yet.
   Open: the alchemist windows ai_AlchemistWindow / ai_AlchemistResearchWindow are @deep (allocation); their buttons call
   ai_AlchemistFundWindow / ai_AlchemistRescheduleWindow with (obj, record), the record parameter unread.
   Open: type debt - the graveyard detail window record GVDETAILWIN is typed in the seance and contact functions
   (ai_SeanceWindow .. ai_UpgradeWorkerWindow); ai_GraveDetailOpen / ai_RentGraveDetail / ai_GraveDetailButtons
   still take int *w (the callers cast), the graveyard contacts before ai_GraveHauntContact take char *rec.
   Open: data: ai_TurnTable (0x6f5f60, 0x4000 bytes) is cleared once at engine open and no other code refers to
   it: what it holds, and so its true name and element type, is unknown (the name is ai_ClearTurnTable's).
   unit of rea 8cef7d5b (src/game/unk_575860.c); boundaries in decl/text.dcl */
struct AHMSTEP;
struct D3SKY;
#include "d2_types.h"
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sv_t.h"
#include "txt.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#include "ai.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_5f1890[];
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_629304[];
extern GAPDATA full_data_602568[];
extern unsigned char kb_Last;
extern GAPDATA full_data_629314[];
extern GAPDATA full_data_629328[];
extern GAPDATA full_data_629340[];
extern GAPDATA full_data_629354[];
extern GAPDATA full_data_629368[];
extern GAPDATA full_data_62937c[];
extern GAPDATA full_data_629390[];
extern GAPDATA full_data_6293a8[];
extern GAPDATA full_data_6293bc[];
extern GAPDATA full_data_6293d8[];
extern GAPDATA full_data_6293e8[];
extern GAPDATA full_data_6293f8[];
extern GAPDATA full_data_629424[];
extern GAPDATA full_data_629438[];
extern GAPDATA full_data_62944c[];
extern GAPDATA full_data_629474[];
extern GAPDATA full_data_629488[];
extern GAPDATA full_data_62949c[];
extern GAPDATA full_data_6294c8[];
extern GAPDATA full_data_6294dc[];
extern GAPDATA full_data_6294f0[];
extern GAPDATA full_data_629518[];
extern GAPDATA full_data_62952c[];
extern GAPDATA full_data_629540[];
extern GAPDATA full_data_629554[];
extern GAPDATA full_data_629568[];
extern GAPDATA full_data_629590[];
extern GAPDATA full_data_6295a0[];
extern GAPDATA full_data_6295b4[];
extern GAPDATA full_data_6295c8[];
extern GAPDATA full_data_6295f0[];
extern GAPDATA full_data_629600[];
extern GAPDATA full_data_629614[];
extern GAPDATA full_data_629628[];
extern GAPDATA full_data_629650[];
extern GAPDATA full_data_629660[];
extern GAPDATA full_data_629674[];
extern GAPDATA full_data_629688[];
extern GAPDATA full_data_6296b0[];
extern GAPDATA full_data_6296c4[];
extern GAPDATA full_data_6296d8[];
extern GAPDATA full_data_6296ec[];
extern GAPDATA full_data_6296fc[];
extern GAPDATA full_data_629710[];
extern GAPDATA full_data_629724[];
extern GAPDATA full_data_629734[];
extern GAPDATA full_data_62974c[];
extern GAPDATA full_data_629760[];
extern GAPDATA full_data_629774[];
extern GAPDATA full_data_62979c[];
extern GAPDATA full_data_6297b0[];
extern GAPDATA full_data_6297c4[];
extern GAPDATA full_data_6297ec[];
extern GAPDATA full_data_629800[];
extern GAPDATA full_data_629814[];
extern GAPDATA full_data_62983c[];
extern GAPDATA full_data_629850[];
extern GAPDATA full_data_629864[];
extern GAPDATA full_data_62988c[];
extern GAPDATA full_data_6298a0[];
extern GAPDATA full_data_6298b4[];
extern GAPDATA full_data_6298dc[];
extern GAPDATA full_data_6298f0[];
extern GAPDATA full_data_629904[];
extern GAPDATA full_data_62992c[];
extern GAPDATA full_data_629940[];
extern GAPDATA full_data_629954[];
extern GAPDATA full_data_629968[];
extern GAPDATA full_data_629978[];
extern GAPDATA full_data_62998c[];
extern GAPDATA full_data_6299a0[];
extern GAPDATA full_data_6299b0[];
extern GAPDATA full_data_6299c8[];
extern GAPDATA full_data_6299f0[];
extern GAPDATA full_data_629a08[];
extern GAPDATA full_data_629a1c[];
extern GAPDATA full_data_629a30[];
extern GAPDATA full_data_629a48[];
extern GAPDATA full_data_629a70[];
extern GAPDATA full_data_629a88[];
extern GAPDATA full_data_629a9c[];
extern GAPDATA full_data_629ab0[];
extern GAPDATA full_data_629ac8[];
extern GAPDATA full_data_629adc[];
extern GAPDATA full_data_629af0[];
extern GAPDATA full_data_629b04[];
extern GAPDATA full_data_629b1c[];
extern GAPDATA full_data_629b34[];
extern GAPDATA full_data_629b4c[];
extern GAPDATA full_data_629b78[];
extern GAPDATA full_data_629b90[];
extern GAPDATA full_data_629ba4[];
extern GAPDATA full_data_629bb8[];
extern GAPDATA full_data_629bd4[];
extern GAPDATA full_data_629bf0[];
extern GAPDATA full_data_629c08[];
extern GAPDATA full_data_629c20[];
extern GAPDATA full_data_629c48[];
extern GAPDATA full_data_629c58[];
extern GAPDATA full_data_629c6c[];
extern GAPDATA full_data_629c80[];
extern GAPDATA full_data_629c9c[];
extern GAPDATA full_data_629cb0[];
extern GAPDATA full_data_629cc8[];
extern GAPDATA full_data_629ce0[];
extern GAPDATA full_data_629d0c[];
extern GAPDATA full_data_629d24[];
extern GAPDATA full_data_629d38[];
extern GAPDATA full_data_629d4c[];
extern GAPDATA full_data_629d68[];
extern GAPDATA full_data_629d80[];
extern GAPDATA full_data_629d98[];
extern GAPDATA full_data_629db0[];
extern GAPDATA full_data_629dd8[];
extern GAPDATA full_data_629dec[];
extern GAPDATA full_data_629e00[];
extern GAPDATA full_data_629e1c[];
extern GAPDATA full_data_629e2c[];
extern GAPDATA full_data_629e40[];
extern GAPDATA full_data_629e50[];
extern GAPDATA full_data_629e7c[];
extern GAPDATA full_data_629e8c[];
extern GAPDATA full_data_629ea0[];
extern GAPDATA full_data_629eb4[];
extern GAPDATA full_data_629edc[];
extern GAPDATA full_data_629eec[];
extern GAPDATA full_data_629f00[];
extern GAPDATA full_data_629f14[];
extern GAPDATA full_data_629f3c[];
extern GAPDATA full_data_629f4c[];
extern GAPDATA full_data_629f60[];
extern GAPDATA full_data_629f74[];
extern GAPDATA full_data_629f88[];
extern GAPDATA full_data_629f98[];
extern GAPDATA full_data_629fac[];
extern GAPDATA full_data_629fc0[];
extern GAPDATA full_data_629fd4[];
extern GAPDATA full_data_62a000[];
extern GAPDATA full_data_62a014[];
extern GAPDATA full_data_62a03c[];
extern GAPDATA full_data_62a050[];
extern GAPDATA full_data_62a064[];
extern GAPDATA full_data_62a08c[];
extern GAPDATA full_data_62a0a4[];
extern GAPDATA full_data_62a0b8[];
extern GAPDATA full_data_62a0cc[];
extern GAPDATA full_data_62a0f4[];
extern GAPDATA full_data_62a10c[];
extern GAPDATA full_data_62a120[];
extern GAPDATA full_data_62a134[];
extern GAPDATA full_data_62a160[];
extern GAPDATA full_data_62a174[];
extern GAPDATA full_data_62a188[];
extern GAPDATA full_data_62a1b0[];
extern GAPDATA full_data_62a1c4[];
extern GAPDATA full_data_62a1d8[];
extern GAPDATA full_data_62a1ec[];
extern GAPDATA full_data_62a1fc[];
extern GAPDATA full_data_62a210[];
extern GAPDATA full_data_62a224[];
extern GAPDATA full_data_62a238[];


extern GAPDATA full_data_624ee0[];
#include "f3init_types.h"
extern GAPDATA full_data_6024f8[];
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_62a248[];



extern GAPDATA full_data_62a284[];
extern GAPDATA full_data_62a2a4[];
extern GAPDATA full_data_62a2c4[];
extern const int sv_PhaseMinutes[24];
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
extern float d3s_QuadUV[6];
extern GAPDATA full_data_5fea38[];
extern GAPDATA full_data_62a2dc[];
extern GAPDATA full_data_62a2fc[];
extern GAPDATA full_data_62a310[];
extern GAPDATA full_data_62a328[];
extern GAPDATA full_data_62a340[];
extern GAPDATA full_data_62a350[];

extern int sv_PanelFlag;
extern GAPDATA full_data_602568[];


extern GAPDATA full_data_602568[];
extern GAPDATA full_data_5f36e0[];










extern GAPDATA full_data_62a38c[];
extern GAPDATA full_data_62a3ac[];
extern GAPDATA full_data_62a3cc[];

extern struct D2OBJ *d2_Objs;




extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern int sv_PopRequest;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;





extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern int sv_PopRequest;
extern int d2_PointerObj;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;





extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern int sv_PopRequest;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a418[];
extern int sv_PopRequest;







extern GAPDATA full_data_62a434[];
extern GAPDATA full_data_62a450[];
extern GAPDATA full_data_62a464[];
extern GAPDATA full_data_62a474[];
extern GAPDATA full_data_62a488[];
extern GAPDATA full_data_62a4a8[];
extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a418[];

extern GAPDATA full_data_62a4bc[];
extern int sv_PopRequest;
extern int ai_BuildingPerms;




extern GAPDATA full_data_62a4cc[];
extern GAPDATA full_data_62a4e8[];
extern GAPDATA full_data_62a508[];
extern GAPDATA full_data_62a524[];
extern int sv_PopRequest;
extern const int sv_PhaseMinutes[24];
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_60c7b8[];
extern GAPDATA full_data_623594[];
extern GAPDATA full_data_62a570[];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a578[];
extern GAPDATA full_data_62a580[];
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern GAPDATA full_data_62a53c[];
extern GAPDATA full_data_62a544[];
extern GAPDATA full_data_62a54c[];
extern char ai_CharNameFmt[];
extern GAPDATA full_data_62a59c[];
extern int d2_PointerObj;
extern GAPDATA full_data_62a5bc[];
extern GAPDATA full_data_625150[];
extern GAPDATA full_data_62a5d8[];


extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern const int sv_PhaseMinutes[24];
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a5bc[];
extern int sv_FrameMask;
extern GAPDATA full_data_625150[];
extern GAPDATA full_data_62a5d8[];



extern GAPDATA full_data_62a5bc[];
extern int sv_PopRequest;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a5f8[];
extern GAPDATA full_data_62a62c[];

extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

extern GAPDATA full_data_62a634[];
extern int sv_FrameMask;
extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;









extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a64c[];
extern GAPDATA full_data_62a664[];
extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;


extern GAPDATA full_data_62a680[];
extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern volatile unsigned long d3_Time;
extern GAPDATA full_data_60c7b8[];
extern GAPDATA full_data_62a69c[];
extern GAPDATA full_data_62a6b8[];
extern GAPDATA full_data_62a6dc[];
extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a5bc[];
extern GAPDATA full_data_62a6f8[];
extern GAPDATA full_data_62a70c[];
extern int d2_PointerObj;
extern const int sv_PhaseMinutes[24];


extern unsigned short ai_PlayerId;
extern GAPDATA full_data_62a718[];
extern int ai_BuildingPerms;
extern int ai_ClickedObject;









extern GAPDATA full_data_62a450[];
extern GAPDATA full_data_62a740[];
extern GAPDATA full_data_62a754[];
extern GAPDATA full_data_62a768[];
extern GAPDATA full_data_62a780[];
extern GAPDATA full_data_62a79c[];
extern GAPDATA full_data_62a7b4[];
extern GAPDATA full_data_62a7cc[];
extern GAPDATA full_data_62a7e0[];
extern unsigned char kb_Last;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;



extern GAPDATA full_data_62a5bc[];
extern int sv_FrameMask;
extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;


extern GAPDATA full_data_62a5bc[];
extern int sv_FrameMask;
extern int sv_PopRequest;

extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;

extern const int sv_PhaseMinutes[24];


extern char ai_Chars[768][0x21c];
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern GAPDATA full_data_62a5bc[];
extern int sv_PopRequest;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;





extern GAPDATA full_data_62a450[];
extern GAPDATA full_data_62a464[];
extern GAPDATA full_data_62a7f8[];
extern GAPDATA full_data_62a808[];
extern GAPDATA full_data_62a818[];
extern int ai_ClickedObject;

extern GAPDATA full_data_62a83c[];
extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern char ai_Chars[768][0x21c];
extern int d2_PointerObj;
extern int gm_AllowAll;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern GAPDATA full_data_62a84c[];
extern int sv_PopRequest;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;


extern GAPDATA full_data_62a870[];
extern GAPDATA full_data_62a88c[];
extern int gm_AllowAll;
extern volatile unsigned long d3_Time;
extern char ai_Chars[768][0x21c];
extern int d2_PointerObj;
extern struct D2OBJ *d2_Objs;
extern int di_Absolute;
extern int gm_PlayerRecords[688];
extern unsigned short ai_PlayerId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a9a8[];
extern GAPDATA full_data_62a9b4[];
extern GAPDATA full_data_62a9cc[];
extern unsigned char ai_OwnPlayer;
extern const float gm_MoodRanges[476];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a91c[];
extern GAPDATA full_data_62a940[];
extern GAPDATA full_data_62a960[];
extern GAPDATA full_data_62a96c[];
extern GAPDATA full_data_62a978[];
extern GAPDATA full_data_62a984[];
extern GAPDATA full_data_62a9e4[];
extern GAPDATA full_data_62a9ec[];
extern GAPDATA full_data_62a9f8[];
extern int sv_PopRequest;
extern GAPDATA full_data_6f5f4c[];
extern GAPDATA full_data_6f5f48[];
extern unsigned char kb_Last;
extern GAPDATA full_data_62aa14[];
extern GAPDATA full_data_62aa20[];

extern int d2_LastWindow;

extern GAPDATA full_data_62aa38[];
extern int sv_PopRequest;

extern GAPDATA full_data_62aa4c[];
extern GAPDATA full_data_62aa58[];
extern int sv_FrameMask;
extern GAPDATA full_data_6f5f44[];
extern unsigned char kb_Last;
extern int sv_PanelFlag;

extern unsigned char kb_Last;
extern int sv_FrameMask;
extern int sv_PanelFlag;
extern char ai_Chars[768][0x21c];

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_PickActionBuilding)(p, a, fl, b, c, f, o)

#define U_I(a) (*(int *)(a))


void ai_TutorialHideText2();
void ai_TutorialProgressSlider();
int ai_TutorialShowRunText();
int ai_TutorialIntroAnswered();
int ai_TutorialStepForm();
int ai_TutorialStepClicked();
void ai_TutorialShowText2();
void sv_CameraFollow();
void ai_TutorialShowText();
void ai_TutorialHideText();
struct SBSAMPLE;

/* the state record is at 0x6fa060 (the 0x40 byte of main.c's 0x6fa020 block); 0x5eec60 is a const pointer to it
   in the original. A load through that pointer (not the fixed address) is what the unit's functions compile to,
   and the full link relocates the initializer, so the record is ours and no neighbour is touched. */
TUTSTATE *const ai_Tutorial = &ai_TutorialState;  /* 0x5eec60, the pointer to the tutorial state record */
#define S (*(char **)(unsigned)&ai_Tutorial)  /* the tutorial state record (at 0x6fa060): the pointer ai_Tutorial holds */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original ai_SeanceGhost reads the 16-bit result (mov si, ax) */
typedef struct SHOTENTRY {   /* 0x10 bytes */
    int obj;                 /* +0x0, -1 when empty */
    float dist;              /* +0x4 */
    int a;                   /* +0x8 */
    union {
        int b;               /* +0xc */
        float bf;            /* +0xc, read as a float by ai_CannonShotResults (<= 25.0) */
    };
} SHOTENTRY;

typedef struct SHOTRESULT {     /* 0x38 bytes, three of them in ai_ShotTable: the cannon scene's shot results */
    int id;                  /* +0x0, the record index, -1 when empty */
    int unk4;                /* +0x4 */
    SHOTENTRY e[3];          /* +0x8 */
} SHOTRESULT;
#define sv_CheckGoodAmountB ((int (*)(char *, int, unsigned char))sv_CheckGoodAmount)  /* @original: ai_AlchemistHire pushes the player byte unextended (mov al, ai_OwnPlayer / push eax), a char third parameter; sv_CheckGoodAmount itself masks it (t & 0xff) */
int ai_TutorialChapterOn[5] = { 1, 1, 1, 1, 1 };  /* 0x62918c, per chapter 1 = played (the start window's check boxes, ai_TutorialWindow) */
int (*ai_TutorialChapterInit[5])(void) = { ai_TutorialInitChapter1, ai_TutorialInitChapter2, ai_TutorialInitChapter3, ai_TutorialInitChapter4, ai_TutorialInitChapter5 };  /* 0x6291a0, the chapter chain builders the start appends for every chapter played */
unsigned int ai_GraveChatterAt = 0xffffffff;  /* 0x62a538, d3_Time at the last graveyard chatter tick (ai_GraveyardChatter runs every 0x1194 ms) */
int ai_DragObj = -1;  /* 0x62a8d0, the right-held d2 object (d2_RightHeldObj) whose tooltip ai_DragWindow shows, -1 = none */
int ai_DragViewWin = -1;  /* 0x62a8d4, the first window of the tooltip view ai_DragWindow (d2_Views[].wins[0]), -1 = none */
int ai_TutorialBanks[6] = { 0 };  /* 0x6f2640, the voice banks of the main intro / outro and the chapters 1 .. 5 (snd_LoadVoiceBank), unloaded by the stop */
TUTRUN *ai_TutorialRuns = 0;  /* 0x6f2658, the chain of the tutorial's run records (built by its start 0x578180, freed by its stop 0x578370) */
int ai_TutorialOn = 0;  /* 0x6f265c, the tutorial runs (set by its start 0x578180, cleared by its end) */
int ai_TutorialRunning = 0;  /* 0x6f2660, the tutorial's state machine advances (unk_SetEndGameGate / unk_ClearEndGameGate) */
int ai_ShotDirty = 0;  /* 0x6f2664, set when ai_ShotTableStore stored a record, cleared by the scene (ai_CannonShotResults) */
char *ai_DragPicked = 0;  /* 0x6f266c, the picked object (ai_PickedObject) the tooltip was last opened for, 0 = none */
int ai_DragCutscene = 0;  /* 0x6f2670, 1 while the tooltip view holds d2_CutsceneActive set */
int ai_ScreenshotCount = 0;  /* 0x6f2674, the number of the next screenshot (gamedata/screenshots/gilde%04i.bmp, ai_SaveScreenshot) */
AIBOOKMARK ai_Bookmarks[11] = { 0 };  /* 0x6f5ec0, the function-key building bookmarks (ai_BookmarksInit sets the keys F1 .. F11) */
unsigned char ai_TurnTable[0x4000] = { 0 };  /* 0x6f5f60, the 0x4000-byte table ai_ClearTurnTable clears once at engine open; no other code refers to it (its role is taken from that function's name), it ends where ai_ShotTable starts */
SHOTRESULT ai_ShotTable[3] = { 0 };  /* 0x6f9f60, the cannon scene's shot results (the callback ai_ShotTableStore fills it, ai_ShotTableReset resets it) */

#include "ls.h"
#include "f3init.h"
#include "ahm.h"
#include "ai.h"
#include "am.h"
#include "amt.h"
#include "anm.h"
#include "bmp.h"
#include "f3gm.h"
#define gm_TitleJobI ((int (*)(int, int))gm_TitleJob)  /* 0x5044b0 (f3gm.h: unsigned char): @original the byte-matched callers here read the result as an int (no `and eax,0xff` after the call): ai_PermissionBits1d 0x587b70 (shape) */
#include "f3snd.h"
#include "gv.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "sblib.h"
#include "tb.h"
#include "tr.h"
#include "mission.h"
#include "lc.h"
#include "vfs.h"
#include "ch.h"
#include "cm.h"
#include "d3_engine.h"
#include "d3_interface.h"
#include "d3_sky.h"
#include "d3_tools.h"
#include "d3_vec.h"
#include "d3s.h"
/* the cannon scene's limits (const data, not literals: the original loads them, computes (x - min) / (max - min)
   at run time and keeps two copies of 45 degrees) */
const float ai_CannonPitchMax = 0.7853982f;    /* 0x5eec6c, 45 degrees: the highest pitch of the cannon (ai_CannonAim) */
const float ai_CannonPitchMin = -0.2617994f;   /* 0x5eec70, -15 degrees: the lowest pitch */
const float ai_CameraPitchMin = -0.61086524f;  /* 0x5eec74, -35 degrees: the low end of the camera's pitch range (ai_CannonCamera) */
const float ai_CameraPitchMax = 0.7853982f;    /* 0x5eec78, 45 degrees: its high end */
const float ai_CameraPitchZero = 0.0f;         /* 0x5eec7c, the level camera: (zero - min) is the range the camera pitch moves in */
const struct D3VECTOR ai_WimpelPos = { -70.0f, -50.0f, 120.0f };  /* 0x5eec80, the wimpel's place on the cannon base */
const float ai_CannonSwitchMs = 90.0f;     /* 0x5eec94, the length of each half of the barrel switch (ms, ai_CannonSwitchAnim) */
const float ai_CannonSwitchDist = 100.0f;  /* 0x5eec98, how far the cannon sinks back while the barrel switches */
const float ai_CannonZoomRange = 1450.0f;  /* 0x5eecac, the distance the zoom moves the camera out (scaled by the pitch and the speed) */
#include "d3_pose.h"
#include "dd_vesa.h"
#include "menu.h"
#include "d2.h"
#include "sv.h"
#include "gfx.h"
#define d2_PlaceMainWindowF ((void (*)(int, int))d2_PlaceMainWindow)  /* 0x475850 (d2.h: int view): @original the callers push the placement flags too, which the original's tail jump leaves on the stack for d2_PlaceWindowScaled */

/* 0x575860, the tutorial's tick: nothing (-4) without the tutorial, its run record or its event slot; puts the
   tutorial's own event window back to the state 3 (or its state to next when another window is active), then, while
   ai_TutorialRunning and no camera move / panel is busy, runs the state machine on ai_Tutorial->state: 1 starts a run
   record, 2 / 3 wait for the intro window, 4 the timed first text window, 5 / 6 / 7 the chapter's step chain, 9 the
   second text window, 0xa / 0xb the timed tails with their status text and voice, 0xc the next run record */

int ai_TutorialTick(void)
{
    if (!ai_TutorialOn)
        return -4;
    if (!ai_Tutorial->run)
        return -4;
    if (!ai_Tutorial->host->slot)
        return -4;
    if (he_ActiveSlot == ai_Tutorial->host->slot && ai_Tutorial->state != 2) {
        if (d3t_CamMoving || sv_PanelFlag) {
            he_ActivateSlot(0);
            return 0;
        }
        d2_SetPointerSprite(0, 0);
        ai_Tutorial->state = 3;
    }
    if (he_ActiveSlot != ai_Tutorial->host->slot)
        ai_Tutorial->state = ai_Tutorial->next;
    if (ai_Tutorial->text2Shown == 1 && ai_Tutorial->state != 9) {
        ai_Tutorial->text2Shown = 0;
        ai_TutorialHideText2();
        ai_TutorialMarkerUpdate(2);
    }
    if (!ai_TutorialRunning)
        return 0;
    ai_TutorialProgressSlider();
    if (d3t_CamMoving || sv_PanelFlag)
        return 0;
    switch (ai_Tutorial->state) {
    case 1:                     /* start the run record */
        if (d3t_CamMove)
            return 0;
        if (ai_TutorialShowRunText())
            return -4;
        ai_Tutorial->goalDone = 0;
        ai_Tutorial->phase = 0;
        ai_Tutorial->state = 2;
        if (ai_Tutorial->run->mode != 1 && ai_Tutorial->run->mode != 2) {
            ai_Tutorial->next = 0xb;
            ai_Tutorial->statusShown = 1;
        } else {
            ai_Tutorial->next = 4;
            ai_Tutorial->statusShown = 0;
        }
        if (ai_Tutorial->run->mode == 2)
            ai_Tutorial->phase = 1;
        if (ai_Tutorial->run->stepHook)
            ai_CampaignStep(2, 0, 0, 0);
        ai_Tutorial->stepArg = 0;
        if (!ai_Tutorial->run->marks)
            ai_Tutorial->markerPhase = 0;
        else
            ai_Tutorial->markerPhase = 1;
        ai_Tutorial->markerStep = 0;
        ai_Tutorial->markerAt = 0;
        ai_Tutorial->markerObj = -1;
        if (ai_Tutorial->run->hook)
            ai_Tutorial->run->hook(0);
        he_ActivateSlot((int *)ai_Tutorial->host->slot);
        d2_SetPointerSprite(0, 0);
        return 0;
    case 2:                     /* the intro window is answered: continue with the clocks reset */
        if (ai_TutorialIntroAnswered() != 1)
            return 0;
        ai_Tutorial->state = ai_Tutorial->next;
        ai_Tutorial->textAt = d3_Time;
        ai_Tutorial->stepAt = d3_Time;
        ai_Tutorial->campaignSteps = 0;
        he_ActivateSlot(0);
        return 0;
    case 3:                     /* the same without the clocks */
        if (ai_TutorialIntroAnswered() != 1)
            return 0;
        ai_Tutorial->state = ai_Tutorial->next;
        he_ActivateSlot(0);
        return 0;
    case 4:                     /* the timed first text window */
        if (ai_Tutorial->run->goal == 3) {
            ai_CampaignStep(0, 0, 0, 0);
            return 0;
        }
        if (d3_Time > ai_Tutorial->textAt + ai_Tutorial->run->text1Time) {
            ai_Tutorial->textAt = d3_Time;
            if (ai_Tutorial->text1Shown == 0 && ai_Tutorial->run->text1) {
                ai_Tutorial->text1Shown = 1;
                ai_TutorialShowText();
            }
        } else if (d3_Time > ai_Tutorial->textAt + 0x238) {
            if (ai_Tutorial->text1Shown != 1)
                return 0;
            ai_Tutorial->textAt = d3_Time;
            ai_Tutorial->text1Shown = 0;
            ai_TutorialHideText();
        }
        if (ai_Tutorial->text1Shown != 1)
            return 0;
        d2_RaiseViewWindows(ai_Tutorial->text1Win);
        return 0;
    case 5:                     /* drop the first text window, open the first step */
        ai_Tutorial->textAt = d3_Time;
        ai_Tutorial->text1Shown = 0;
        ai_TutorialHideText();
        ai_Tutorial->phase = 2;
        if (ai_Tutorial->run->stepCount > 0) {
            ai_Tutorial->step = 0;
            ai_Tutorial->state = 6;
            ai_Tutorial->next = 6;
            ai_TutorialStepForm();
            return 0;
        }
        ai_Tutorial->state = 7;
        ai_Tutorial->next = 7;
        return 0;
    case 6:                     /* one step of the chain: its control is enabled after the step's delay */
        tb_Paused = d3_Time >= ai_Tutorial->run->steps[ai_Tutorial->step].delay + ai_Tutorial->stepAt;
        d2_SetObjEnabled(ai_Tutorial->stepControl, tb_Paused);
        if (ai_TutorialStepClicked() != 1)
            return 0;
        ai_Tutorial->step++;
        ai_Tutorial->textAt = d3_Time;
        if (ai_Tutorial->step < ai_Tutorial->run->stepCount) {
            ai_Tutorial->stepAt = d3_Time;
            ai_TutorialStepForm();
            return 0;
        }
        ai_Tutorial->state = 7;
        ai_Tutorial->next = 7;
        return 0;
    case 7:                     /* the chain is done: the second text window or the tail */
        ai_Tutorial->phase = 3;
        ai_Tutorial->stepAt = d3_Time;
        ai_Tutorial->stepArg = 0;
        ai_TutorialStepForm();
        tb_Paused = 0;
        if (ai_Tutorial->run->text2Goal) {
            ai_Tutorial->state = 9;
            ai_Tutorial->next = 9;
            return 0;
        }
        ai_Tutorial->state = 0xa;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->statusShown = ai_Tutorial->run->statusText == 0;
        return 0;
    case 9:                     /* the second text window and the marker */
        if (ai_Tutorial->text1Shown == 1) {
            ai_Tutorial->textAt = d3_Time;
            ai_Tutorial->text1Shown = 0;
            ai_TutorialHideText();
        }
        if (ai_Tutorial->text2Shown == 0 && ai_Tutorial->run->text2) {
            ai_Tutorial->text2Shown = 1;
            ai_TutorialShowText2();
            ai_TutorialMarkerUpdate(0);
        }
        if (ai_Tutorial->text2Shown == 1)
            d2_RaiseViewWindows(ai_Tutorial->text2Win);
        ai_TutorialMarkerUpdate(1);
        return 0;
    case 0xa:                   /* the tail: after tailTime the camera returns and the state 0xb follows */
        if (ai_Tutorial->text1Shown == 1) {
            ai_Tutorial->textAt = d3_Time;
            ai_Tutorial->text1Shown = 0;
            ai_TutorialHideText();
        }
        if (d3_Time > ai_Tutorial->run->tailTime + ai_Tutorial->stepAt) {
            sv_CameraFollow();
            sv_PopRequest = sv_FrameDepth - sv_FrameBase;
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = 0xb;
            ai_Tutorial->stepAt = d3_Time;
        }
        if (ai_Tutorial->statusShown)
            return 0;
        if (!ai_Tutorial->run->statusText)
            return 0;
        sv_SetStatusText(txt_Ptrs[ai_Tutorial->run->statusText]);
        if (ai_Tutorial->run->voice) {
            if (ai_Tutorial->voice) {
                if (sb_IsPlaying(ai_Tutorial->voice))
                    sb_Stop(ai_Tutorial->voice, 0);
                ai_Tutorial->voice = 0;
            }
            ai_Tutorial->voice = (struct SBSAMPLE *)snd_PlayVoice(-7, 0, ai_Tutorial->run->voice, -1);
        }
        ai_Tutorial->statusShown = 1;
        ai_TutorialShowRunText();
        return 0;
    case 0xb:                   /* the second tail: after tail2Time the state 0xc */
        if (ai_Tutorial->text1Shown == 1) {
            ai_Tutorial->textAt = d3_Time;
            ai_Tutorial->text1Shown = 0;
            ai_TutorialHideText();
        }
        if (d3_Time > ai_Tutorial->run->tail2Time + ai_Tutorial->stepAt) {
            sv_CameraFollow();
            sv_PopRequest = sv_FrameDepth - sv_FrameBase;
            ai_Tutorial->state = 0xc;
            ai_Tutorial->next = 0xc;
            ai_Tutorial->stepAt = d3_Time;
        }
        if (ai_Tutorial->statusShown)
            return 0;
        if (!ai_Tutorial->run->statusText)
            return 0;
        sv_SetStatusText(txt_Ptrs[ai_Tutorial->run->statusText]);
        if (ai_Tutorial->run->voice) {
            if (ai_Tutorial->voice) {
                if (sb_IsPlaying(ai_Tutorial->voice))
                    sb_Stop(ai_Tutorial->voice, 0);
                ai_Tutorial->voice = 0;
            }
            ai_Tutorial->voice = (struct SBSAMPLE *)snd_PlayVoice(-7, 0, ai_Tutorial->run->voice, -1);
        }
        ai_Tutorial->statusShown = 1;
        ai_TutorialShowRunText();
        return 0;
    case 0xc:                   /* the run is over: the next run record */
        if (ai_Tutorial->text1Shown == 1) {
            ai_Tutorial->textAt = d3_Time;
            ai_Tutorial->text1Shown = 0;
            ai_TutorialHideText();
        }
        if (ai_Tutorial->run->hook)
            ai_Tutorial->run->hook(1);
        ai_Tutorial->run = ai_Tutorial->run->next;
        ai_Tutorial->state = 1;
        ai_Tutorial->next = 1;
        sv_SetStatusText(ch_EmptyName);
        return 0;
    }
    return 0;
}
/* 0x576020, clears the end-of-game gate 0x6f2660 (checked by the tick 0x575860 at 0x575915) */

void unk_ClearEndGameGate(void)
{
    ai_TutorialRunning = 0;
}
/* 0x576030, sets the end-of-game gate 0x6f2660 (the setter counterpart of 0x576020) */

void unk_SetEndGameGate(void)
{
    ai_TutorialRunning = 1;
}
/* 0x576040, one event of the tutorial run (a the event index, b / c its kind / result, d only for the hook): the
   run's stepHook gets all four as an int[4] when it has one; else, for a run of mode 2, the event that meets the
   run's goal (or the second text window's text2Goal) moves the states 4 / 9 / 0xa on (phase 1: next 5 or back to 4;
   phase 3: 4 -> 9, 9 -> 0xa or back to 4, 0xa -> 0xb); for the other modes the goal event in the states 4 / 0xa
   switches to goalState (c == 0) or 0xb, and a timed goal (3) to goalState once tailTime has passed. Counts the
   handled events in campaignSteps */

void ai_CampaignStep(int a, int b, int c, int d)
{
    int p[4];
    int (*hook)(int *);

    if (!ai_TutorialOn)
        return;
    if (!ai_Tutorial->run)
        return;
    p[0] = a;
    p[1] = b;
    p[2] = c;
    p[3] = d;
    if (ai_Tutorial->run->mode != 2) {
        if (ai_Tutorial->next != 4 && ai_Tutorial->next != 0xa)
            return;
        hook = ai_Tutorial->run->stepHook;
        if (hook) {
            hook(p);
            ai_Tutorial->campaignSteps++;
            return;
        }
        if (ai_Tutorial->run->goal == 3) {
            if (d3_Time > ai_Tutorial->run->tailTime + ai_Tutorial->stepAt) {
                ai_Tutorial->state = ai_Tutorial->run->goalState;
                ai_Tutorial->next = ai_Tutorial->run->goalState;
                ai_Tutorial->stepAt = d3_Time;
                ai_Tutorial->statusShown = 1;
            }
            return;
        }
        if (ai_Tutorial->run->goal == a) {
            if (c == 0)
                ai_Tutorial->state = ai_Tutorial->run->goalState;
            else
                ai_Tutorial->state = 0xb;
            ai_Tutorial->next = ai_Tutorial->state;
            ai_Tutorial->stepAt = d3_Time;
        }
        ai_Tutorial->campaignSteps++;
        return;
    }
    if (ai_Tutorial->next != 4 && ai_Tutorial->next != 9 && ai_Tutorial->next != 0xa)
        return;
    hook = ai_Tutorial->run->stepHook;
    if (hook) {
        hook(p);
        return;
    }
    if (ai_Tutorial->phase == 1) {
        if (c == 0) {
            if (ai_Tutorial->run->goal == a) {
                ai_Tutorial->next = 5;
                ai_Tutorial->stepAt = d3_Time;
                if (b)
                    ai_Tutorial->stepArg = b;
            }
        } else if (c == 1 && ai_Tutorial->run->goal == a) {
            ai_Tutorial->next = 4;
            ai_Tutorial->stepAt = d3_Time;
            ai_Tutorial->text2Shown = 0;
            ai_TutorialHideText2();
            ai_TutorialMarkerUpdate(2);
        }
        ai_Tutorial->state = ai_Tutorial->next;
        ai_Tutorial->campaignSteps++;
        return;
    }
    if (ai_Tutorial->phase == 3) {
        if (ai_Tutorial->next == 9) {
            if (ai_Tutorial->run->text2Goal && ai_Tutorial->run->text2Goal == a) {
                ai_Tutorial->state = 0xa;
                ai_Tutorial->next = 0xa;
                ai_Tutorial->stepAt = d3_Time;
                ai_Tutorial->campaignSteps++;
                return;
            }
            if (ai_Tutorial->run->goal == a && c == 1) {
                ai_Tutorial->state = 4;
                ai_Tutorial->next = 4;
                ai_Tutorial->stepAt = d3_Time;
                ai_Tutorial->text2Shown = 0;
                ai_TutorialHideText2();
                ai_TutorialMarkerUpdate(2);
                ai_Tutorial->campaignSteps++;
                return;
            }
        } else if (ai_Tutorial->next == 0xa) {
            if (ai_Tutorial->run->goal == a && c == 1) {
                ai_Tutorial->state = 0xb;
                ai_Tutorial->next = 0xb;
                ai_Tutorial->stepAt = d3_Time;
            }
        } else if (ai_Tutorial->next == 4) {
            if (ai_Tutorial->run->goal == a && c == 0) {
                ai_Tutorial->state = 9;
                ai_Tutorial->next = 9;
                ai_Tutorial->stepAt = d3_Time;
            }
        }
    }
    ai_Tutorial->campaignSteps++;
}
/* 0x576340, asks the current run record's query callback: hands it the four arguments as an int[4] and returns
   its answer; 1 (no objection) while the tutorial is off, no run is current or the run has no callback, 0 while
   tb_Paused is set */

int ai_ScriptQuery(int a, int b, int c, int d)
{
    int args[4];

    if (!ai_TutorialOn)
        return 1;
    if (!ai_Tutorial->run)
        return 1;
    if (tb_Paused)
        return 0;
    if (!ai_Tutorial->run->query)
        return 1;
    args[0] = a;
    args[1] = b;
    args[2] = c;
    args[3] = d;
    return ai_Tutorial->run->query(args);
}
/* 0x5763c0, 1 when the current run record's allow mask (TUTRUN.allowMask) has a bit of `mask`, else 0; 1 while
   the tutorial is off (ai_TutorialOn) and when the state record holds no run, 0 in the phase 2 or the state 5 and
   while the time base is paused (tb_Paused) */

int ai_RunAllows(int mask)
{
    if (!ai_TutorialOn)
        return 1;
    if (ai_Tutorial->phase == 2 || ai_Tutorial->state == 5)
        return 0;
    if (!ai_Tutorial->run)
        return 1;
    if (tb_Paused)
        return 0;
    return (ai_Tutorial->run->allowMask & mask) != 0;
}
/* 0x576410, the same predicate as ai_RunAllows on the run record's second allow mask (TUTRUN.allowMask2): the
   contact menu entries the tutorial allows */

int ai_RunAllowsContact(int mask)
{
    if (!ai_TutorialOn)
        return 1;
    if (ai_Tutorial->phase == 2 || ai_Tutorial->state == 5)
        return 0;
    if (!ai_Tutorial->run)
        return 1;
    if (tb_Paused)
        return 0;
    return (ai_Tutorial->run->allowMask2 & mask) != 0;
}
/* 0x576460, 1 while the tutorial lets a building window open: 1 while it is off (ai_TutorialOn), else 0 in the
   phase 2 or the state 5, 1 otherwise */

int ai_RunAllowsWindow(void)
{
    if (!ai_TutorialOn)
        return 1;
    if (ai_Tutorial->phase == 2 || ai_Tutorial->state == 5)
        return 0;
    return 1;
}
/* 0x576490, a callback predicate on the first of the four step arguments (a pointer to the int[4]): 0 for the
   kinds 0x19 / 0x1b / 0x29, else 1 (Ghidra missed this start; it ends the 80 byte record of 0x576460 at 0x5764ae) */

int ai_TutorialQueryDefault(int *p)
{
    if (*p == 0x19 || *p == 0x1b || *p == 0x29)
        return 0;
    return 1;
}
/* 0x5764b0, the default query callback of the run records (TUTRUN.query): 0 (refuse) for the event 0x1a on a
   building of the type kind 0x12 while the panel shows the room 0x74, else 1 (allow); the other callbacks fall
   back to it */

int ai_QueryDefault(int *args)
{
    AIBLD *bld;

    if (args[0] != 0x1a || !args[1])
        return 1;
    bld = (AIBLD *)args[1];
    if (((AIBTYPE *)ai_BTypes)[bld->type].kind == 0x12 && *sv_PanelShown == 0x74)
        return 0;
    return 1;
}
/* 0x576500, a run record's query callback for the building type kind 0xf: the event 0x19 on such a building is
   allowed only with the argument 0x10e, the event 0x1a on it is refused while the panel shows the room 0x10e;
   the event 0x1b is refused, everything else allowed */

int ai_QueryBuilding0f(int *args)
{
    if (args[0] == 0x19 && args[1]) {
        if (((AIBTYPE *)ai_BTypes)[((AIBLD *)args[1])->type].kind == 0xf && args[3] == 0x10e)
            return 1;
        return 0;
    }
    if (args[0] == 0x1a && args[1]) {
        if (((AIBTYPE *)ai_BTypes)[((AIBLD *)args[1])->type].kind == 0xf && *sv_PanelShown == 0x10e)
            return 0;
        return 1;
    }
    return args[0] != 0x1b;
}
/* 0x576590, the query callback of the run record FTInitChapter2Step3: allows only the event 0x19 on a building
   of the type kind 0xf with the argument 0x110, refuses everything else */

int ai_QueryChapter2Step3(int *args)
{
    if (args[0] == 0x19 && args[1]
            && ((AIBTYPE *)ai_BTypes)[((AIBLD *)args[1])->type].kind == 0xf && args[3] == 0x110)
        return 1;
    return 0;
}
/* 0x5765e0, 1 for the kind 0x1a with a record */

int ai_QueryChapter2Step4(int *p)
{
    if (p[0] == 0x1a && p[1])
        return 1;
    return 0;
}
/* 0x576600, the query callback of the run record FTInitChapter2Step5: refuses the event 0x19 on a building of
   another type kind than 0xf, allows everything else */

int ai_QueryChapter2Step5(int *args)
{
    if (args[0] == 0x19 && args[1]
            && ((AIBTYPE *)ai_BTypes)[((AIBLD *)args[1])->type].kind != 0xf)
        return 0;
    return 1;
}
/* 0x576640, the query callback of the run record FTInitChapter2Step6: refuses the event 0x19 on a building of
   another type kind than 0xf or 2, allows everything else */

int ai_QueryChapter2Step6(int *args)
{
    AIBTYPE *type;

    if (args[0] == 0x19 && args[1]) {
        type = &((AIBTYPE *)ai_BTypes)[((AIBLD *)args[1])->type];
        if (type->kind != 0xf && type->kind != 2)
            return 0;
    }
    return 1;
}
/* 0x576680, the query callback of the run record FTInitChapter2Step7: refuses the event 0x19 on a building of
   another type kind than 0xf, 2 or 0x12, allows everything else */

int ai_QueryChapter2Step7(int *args)
{
    AIBTYPE *type;

    if (args[0] == 0x19 && args[1]) {
        type = &((AIBTYPE *)ai_BTypes)[((AIBLD *)args[1])->type];
        if (type->kind != 0xf && type->kind != 2 && type->kind != 0x12)
            return 0;
    }
    return 1;
}
/* 0x5766d0, the step callback of the run record FTInitChapter1Step5: the event 0xa with the argument 0 switches
   the state 4 to 9, with 1 the state 9 back to 4 and 0xa to 0xb; the event 7 on a record whose byte +2 is 5
   switches the state 9 to 0xa; each switch restarts the step clock and answers 1, anything else answers 3 */

int ai_StepChapter1Step5(int *args)
{
    if (args[0] == 0xa) {
        if (args[2] == 0 && ai_Tutorial->next == 4) {
            ai_Tutorial->state = 9;
            ai_Tutorial->next = 9;
            ai_Tutorial->stepAt = d3_Time;
            return 1;
        }
        if (args[2] == 1) {
            if (ai_Tutorial->next == 9) {
                ai_Tutorial->state = 4;
                ai_Tutorial->next = 4;
                ai_Tutorial->stepAt = d3_Time;
                return 1;
            }
            if (ai_Tutorial->next == 0xa) {
                ai_Tutorial->state = 0xb;
                ai_Tutorial->next = 0xb;
                ai_Tutorial->stepAt = d3_Time;
                return 1;
            }
        }
    } else if (args[0] == 7 && args[1] && ai_Tutorial->next == 9 && ((unsigned char *)args[1])[2] == 5) {
        ai_Tutorial->state = 0xa;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->stepAt = d3_Time;
        return 1;
    }
    return 3;
}
/* 0x5767d0, the kind 0xd with a building whose table byte is 0xa moves the pending state to 0xb (1), else 3 */

int ai_StepChapter1Step10(int *p)
{
    AIBTYPE *type;

    if (p[0] == 0xd && p[1]) {
        type = &((AIBTYPE *)ai_BTypes)[((AIBLD *)p[1])->type];
        if (type->kind == 0xa) {
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = 0xb;
            ai_Tutorial->stepAt = d3_Time;
            return 1;
        }
    }
    return 3;
}
/* 0x576830, the kinds 0x17 / 0x18 set the pending state 0xa / 0xb (1), else 3. Ends at 0x57689f; 0x5768a0 is a
   new start without nop padding */

int ai_StepChapter2Step1(int *p)
{
    if (p[0] == 0x17) {
        ai_Tutorial->state = 0xa;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->stepAt = d3_Time;
        return 1;
    }
    if (p[0] == 0x18) {
        ai_Tutorial->state = 0xb;
        ai_Tutorial->next = 0xb;
        ai_Tutorial->stepAt = d3_Time;
        return 1;
    }
    return 3;
}
/* 0x5768a0, the step hook of the run record FTInitChapter2Step2: with `arg` 0 it opens the town hall (the
   building type 0xf, gm_BuildQuery (1, 5, 0xf)) at the room 0x10e; no return value */

void ai_HookChapter2Step2(int arg)
{
    AIBLD *b;

    if (arg)
        return;
    b = (AIBLD *)gm_BuildQuery(1, 5, 0xf);
    if (b)
        b->mainRoom = 0x10e;
}
/* 0x5768d0, the kind 0x19 with a record whose word is 0x10e moves the pending state to 0xb (1), else 3 */

int ai_StepChapter2Step2(int *p)
{
    if (p[0] == 0x19 && p[1]) {
        unsigned short *room = (unsigned short *)(unsigned)p[1];

        if (*room == 0x10e) {
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = 0xb;
            ai_Tutorial->stepAt = d3_Time;
            return 1;
        }
    }
    return 3;
}
/* 0x576920, the same as 0x5768d0 for the kind 0x19 and the record word 0x110 */

int ai_StepChapter2Step3(int *p)
{
    if (p[0] == 0x19 && p[1]) {
        unsigned short *room = (unsigned short *)(unsigned)p[1];

        if (*room == 0x110) {
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = 0xb;
            ai_Tutorial->stepAt = d3_Time;
            return 1;
        }
    }
    return 3;
}
/* 0x576970, the same for the kind 0x1a and the record word 0x110 */

int ai_StepChapter2Step4(int *p)
{
    if (p[0] == 0x1a && p[1]) {
        unsigned short *room = (unsigned short *)(unsigned)p[1];

        if (*room == 0x110) {
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = 0xb;
            ai_Tutorial->stepAt = d3_Time;
            return 1;
        }
    }
    return 3;
}
/* 0x5769c0, the kinds 0x1b / 0x1c with a building whose table byte is 0xd: the kind 0x1b only while the pending
   state is 4 (state 0xa), the kind 0x1c in any state (state 0xb); else 3. Ends 0x576a7f; 0x576a80 is a new start
   without nop padding */

int ai_StepChapter2Step5(int *p)
{
    AIBLD *b;

    if (p[0] == 0x1b) {
        if (!p[1])
            goto next;
        if (ai_Tutorial->next != 4)
            goto next;
        b = (AIBLD *)p[1];
        if (((AIBTYPE *)ai_BTypes)[b->type].kind != 0xd)
            goto fail;
        ai_Tutorial->state = 0xa;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->stepAt = d3_Time;
        return 1;
    }
next:
    if (p[0] == 0x1c) {
        if (!p[1])
            goto fail;
        b = (AIBLD *)p[1];
        if (((AIBTYPE *)ai_BTypes)[b->type].kind != 0xd)
            goto fail;
        ai_Tutorial->state = 0xb;
        ai_Tutorial->next = 0xb;
        ai_Tutorial->stepAt = d3_Time;
        return 1;
    }
fail:
    return 3;
}
/* 0x576a80, when `arg` is 1, finds a record (0x501190 (1, 5, 0xa)) and passes it to 0x521960 (r, -1, 0x631090);
   no return value */

void ai_HookChapter2Step5(int arg)
{
    char *r;

    if (arg != 1)
        return;
    r = (char *)gm_BuildQuery(1, 5, 0xa);
    if (r)
        sv_SelectPanelObject((unsigned char *)r, -1, (char *)(unsigned)&ch_EmptyName);
}
/* 0x576ab0, the kinds 0x19 / 0x1a with a record whose word is 0x13: the kind 0x19 only in the pending state 4
   (state 0xa), the kind 0x1a in any state (state 0xb); else 3 */

int ai_StepChapter2Step6(int *p)
{
    if (p[0] == 0x19 && p[1] && ai_Tutorial->next == 4) {
        unsigned short *room = (unsigned short *)(unsigned)p[1];

        if (*room != 0x13)
            return 3;
        ai_Tutorial->state = 0xa;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->stepAt = d3_Time;
        return 1;
    }
    if (p[0] == 0x1a && p[1]) {
        unsigned short *room = (unsigned short *)(unsigned)p[1];

        if (*room == 0x13) {
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = 0xb;
            ai_Tutorial->stepAt = d3_Time;
            return 1;
        }
    }
    return 3;
}
/* 0x576b50, the kind 0x19 with a record whose word is 0x74 moves the pending state to 0xb (1), else 3 */

int ai_StepChapter2Step7(int *p)
{
    if (p[0] == 0x19 && p[1]) {
        unsigned short *room = (unsigned short *)(unsigned)p[1];

        if (*room == 0x74) {
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = 0xb;
            ai_Tutorial->stepAt = d3_Time;
            return 1;
        }
    }
    return 3;
}
/* 0x576ba0, a transition callback on the kinds 0x2b / 0x10 for a building (table 0x7e3f30, byte 0x12) whose word
   +0x27 is the player's 0x6f25b8: the kind 0x2b with the step word p[3] 0xc / 0x1df and the flag +0x38 clear sets
   the flag and the state 0xa, and answers 3; the kind 0x10 sets the pending state 9 (p[2] 0), 0xb / 4 (p[2] 1 by
   the flag +0x38), applies it and answers 1 */

int ai_StepChapter4Step2(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = ((AIBLD *)p[1])->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind != 0x12)
            return 3;
        if (((AIBLD *)p[1])->owner != ai_PlayerId)
            return 3;
        if (ai_Tutorial->goalDone)
            return 3;
        if (p[3] != 0xc && p[3] != 0x1df)
            return 3;
        ai_Tutorial->goalDone = 1;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->state = ai_Tutorial->next;
        ai_Tutorial->stepAt = d3_Time;
        return 3;
    }
    if (p[0] != 0x10 || !p[1])
        return 3;
    kind = ((AIBLD *)p[1])->type;
    if (((AIBTYPE *)ai_BTypes)[kind].kind != 0x12)
        return 3;
    if (((AIBLD *)p[1])->owner != ai_PlayerId)
        return 3;
    if (p[2] == 0 && !ai_Tutorial->goalDone)
        ai_Tutorial->next = 9;
    if (p[2] == 1) {
        if (ai_Tutorial->goalDone == 1)
            ai_Tutorial->next = 0xb;
        else
            ai_Tutorial->next = 4;
    }
    ai_Tutorial->state = ai_Tutorial->next;
    ai_Tutorial->stepAt = d3_Time;
    return 1;
}
/* 0x576cd0, a predicate: 1 for the kinds 0x1a / 0x19 / 0x1c, and for the kind 0x2b on the player's building (byte
   0x12, word +0x27 = 0x6f25b8) with the step word 0xc / 0x1df; else 0 */

int ai_QueryChapter4Step2(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = ((AIBLD *)p[1])->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind == 0x12
                && ((AIBLD *)p[1])->owner == ai_PlayerId
                && (p[3] == 0xc || p[3] == 0x1df))
            return 1;
    }
    if (p[0] == 0x1a || p[0] == 0x19 || p[0] == 0x1c)
        return 1;
    return 0;
}
/* 0x576d40, the twin of ai_StepChapter4Step2 (0x576ba0) for the kinds 0x2b / 0x11: on a building of the type kind
   0x12 that the player owns, the kind 0x2b with the step word p[3] 0x78 / 0x79 / 0x7a and the goal not met yet
   meets the goal and switches to the state 0xa (answers 3); the kind 0x11 sets the pending state 9 (p[2] 0, goal
   open), 0xb / 4 (p[2] 1, by the goal), applies it and answers 1 */

int ai_StepChapter4Step4(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = ((AIBLD *)p[1])->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind != 0x12)
            return 3;
        if (((AIBLD *)p[1])->owner != ai_PlayerId)
            return 3;
        if (ai_Tutorial->goalDone)
            return 3;
        if (p[3] != 0x79 && p[3] != 0x7a && p[3] != 0x78)
            return 3;
        ai_Tutorial->goalDone = 1;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->state = ai_Tutorial->next;
        ai_Tutorial->stepAt = d3_Time;
        return 3;
    }
    if (p[0] != 0x11 || !p[1])
        return 3;
    kind = ((AIBLD *)p[1])->type;
    if (((AIBTYPE *)ai_BTypes)[kind].kind != 0x12)
        return 3;
    if (((AIBLD *)p[1])->owner != ai_PlayerId)
        return 3;
    if (p[2] == 0 && !ai_Tutorial->goalDone)
        ai_Tutorial->next = 9;
    if (p[2] == 1) {
        if (ai_Tutorial->goalDone == 1)
            ai_Tutorial->next = 0xb;
        else
            ai_Tutorial->next = 4;
    }
    ai_Tutorial->state = ai_Tutorial->next;
    ai_Tutorial->stepAt = d3_Time;
    return 1;
}
/* 0x576e70, the predicate twin of 0x576cd0: 1 for the player's building (byte 0x12, word +0x27 = 0x6f25b8) with
   the step word 0x78 / 0x79 / 0x7a; else the answer of 0x5764b0 */

int ai_QueryChapter4Step4(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = ((AIBLD *)p[1])->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind == 0x12
                && ((AIBLD *)p[1])->owner == ai_PlayerId
                && (p[3] == 0x79 || p[3] == 0x7a || p[3] == 0x78))
            return 1;
    }
    return ai_QueryDefault(p);
}
/* 0x576ee0, the twin of 0x576ba0 for a building of the table byte 0x2: the kind 0x2b with the step word
   0x5 / 0x2 / 0xd / 0x8 and the flag +0x38 clear sets the flag and the state 0xa (3); the kind 0x10 sets the
   pending state 9 / 0xb / 4, applies it and answers 1 */

int ai_StepChapter4Step5(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = ((AIBLD *)p[1])->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind != 2)
            return 3;
        if (((AIBLD *)p[1])->owner != ai_PlayerId)
            return 3;
        if (ai_Tutorial->goalDone)
            return 3;
        if (p[3] != 5 && p[3] != 2 && p[3] != 0xd && p[3] != 8)
            return 3;
        ai_Tutorial->goalDone = 1;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->state = ai_Tutorial->next;
        ai_Tutorial->stepAt = d3_Time;
        return 3;
    }
    if (p[0] != 0x10 || !p[1])
        return 3;
    kind = ((AIBLD *)p[1])->type;
    if (((AIBTYPE *)ai_BTypes)[kind].kind != 2)
        return 3;
    if (((AIBLD *)p[1])->owner != ai_PlayerId)
        return 3;
    if (p[2] == 0 && !ai_Tutorial->goalDone)
        ai_Tutorial->next = 9;
    if (p[2] == 1) {
        if (ai_Tutorial->goalDone == 1)
            ai_Tutorial->next = 0xb;
        else
            ai_Tutorial->next = 4;
    }
    ai_Tutorial->state = ai_Tutorial->next;
    ai_Tutorial->stepAt = d3_Time;
    return 1;
}
/* 0x577020, the predicate twin of 0x576ee0: 1 for a building of the table byte 0x2 with the step word
   0x5 / 0x2 / 0xd / 0x8 and for the kinds 0x1a / 0x19 / 0x1c; else 0 */

int ai_QueryChapter4Step5(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = ((AIBLD *)p[1])->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind == 2
                && ((AIBLD *)p[1])->owner == ai_PlayerId
                && (p[3] == 5 || p[3] == 2 || p[3] == 0xd || p[3] == 8))
            return 1;
    }
    if (p[0] == 0x1a || p[0] == 0x19 || p[0] == 0x1c)
        return 1;
    return 0;
}
/* 0x5770a0, when `arg` is 0: posts the message 0x2bc0 for the player's character id (0x770b80 + id * 0x21c, +4) */

void ai_HookChapter4Step(int arg)
{
    int id;

    if (arg)
        return;
    id = ai_CharInts[ai_PlayerId][0];
    cm_SendMoveGoodsUnchecked(id, -1, 0, 0x2bc0);
}
/* 0x5770e0, a transition callback (0x30 B; a start the ret+nop scan missed: it follows 0x5770a0's nop fill and
   runs to 0x57710f): for the event kind 0x20 with the step p[3] 0x155 it answers 1 while the state record's +0x3c
   is 3, else 0; every other event goes on to 0x5764b0 */

int ai_QueryChapter3Step2(int *p)
{
    if (p[0] == 0x20) {
        if (p[3] == 0x155 && ai_Tutorial->phase == 3)
            return 1;
        return 0;
    }
    return ai_QueryDefault(p);
}
/* 0x577110, the run hook of the run record FTInitChapter3Step2: at the run's start (end 0) finds the first
   character whose current building belongs to the player and, when 0x55ea60 answers for it, sends it to the
   entrance dummy ("dummy_EINGANG", message 0x30) and posts the message 0x2f with the same values */

void ai_HookChapter3Step2(int end)
{
    AICHAR *chr;
    int x, y;
    int i;

    if (end)
        return;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].bld && ai_CharsT[i].bld->owner == ai_PlayerId) {
            chr = &ai_CharsT[i];
            if (!chr)
                return;
            if (!he_FigureContact(chr, &x, &y))
                return;
            cm_RequestChrMoveToUniverse(chr->netId, x, y, "dummy_EINGANG");
            cm_RequestChrMove(chr->netId, x, y, (char *)(unsigned)&ch_EmptyName);
            return;
        }
    }
}
/* 0x5771b0, a transition callback on the flag S+0x38: it is raised to 1 by the kind 0x25 with the step word
   0x155, and to 2 by the kind 0x24 on a building with the table byte 0xa (1); else 0, and the answer of 0x5764b0
   when the flag is below the needed value or no record is given */

int ai_QueryChapter3Step6(int *p)
{
    int kind;

    if (ai_Tutorial->goalDone >= 0 && p[0] == 0x25 && p[3] == 0x155) {
        ai_Tutorial->goalDone = 1;
        return 1;
    }
    if (ai_Tutorial->goalDone >= 1 && p[1] && p[0] == 0x24) {
        kind = ((AIBLD *)p[1])->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind == 0xa) {
            ai_Tutorial->goalDone = 2;
            return 1;
        }
        return 0;
    }
    return ai_QueryDefault(p);
}
/* 0x577230, when `arg` is 0: posts the message 0x2bc0 for the player's character id (0x770b80 + id * 0x21c, +4)
   and a fight message (0x4c4880) on the record of 0x500ec0 (0x155): (id +2, -1, 0x155, 3, 0x6f1bc5, 0) */

void ai_HookChapter5Step2(int arg)
{
    ORDOBJ *r;
    int id;

    if (arg)
        return;
    r = (ORDOBJ *)gm_FindMarketObject(0x155);
    id = ai_CharInts[ai_PlayerId][0];
    cm_SendMoveGoodsUnchecked(id, -1, 0, 0x2bc0);
    cm_SendExSellObject(r->objId, -1, 0x155, 3, ai_OwnPlayer, 0);
}
/* 0x577290, the query callback of the run record FTInitChapter5Step2: once its goal is done (goalDone > 0) it
   allows only the event 0x29 on the market object 0x155 (and sets goalDone back to 1), refusing everything else;
   before that it allows everything */

int ai_QueryChapter5Step2(int *args)
{
    if (ai_Tutorial->goalDone > 0) {
        if (args[0] == 0x29 && args[3] == 0x155) {
            ai_Tutorial->goalDone = 1;
            return 1;
        }
        return 0;
    }
    return 1;
}
/* 0x5772d0, the marker step callback of the run record FTInitChapter5Step2 (ai_TutorialMarkerUpdate): picks the
   marker entry for the shown panel room - 0 for the room 0x92, 1 for 0x93 .. 0x97 and 0x2db - while the goal is
   not done; -1 (no marker) without a shown panel, for other rooms or once goalDone is set */

int ai_MarkerChapter5Step2(int mode)
{
    unsigned short room;
    int r;

    if (sv_PanelShown) {
        room = *sv_PanelShown;
        r = room;
        if (r >= 0x92 && (r <= 0x97 || r == 0x2db) && !ai_Tutorial->goalDone)
            return room != 0x92;
    }
    return -1;
}
/* 0x577320, the chapter 3 step 6 marker: the tutorial's goalDone flag (0 nothing, 1 the event kind 0x25 with the
   step word 0x155, 2 the building of the table byte 0xa visited) */

int ai_MarkerChapter3Step6(int mode)
{
    return ai_Tutorial->goalDone;
}
/* 0x577330, a transition callback on the goalDone flag: it is raised to 1 by the kind 0x28 (step word 0x155), to 2
   by the kind 0x27 (0x1d0) and to 3 by the kind 0x24 on the player's building (table byte 0x12, owner =
   ai_PlayerId), each when the flag already reaches the previous level (1) and the step fits; the kinds 0x1a /
   0x1c answer 1, everything else 0 */

int ai_QueryChapter3Step8(int *p)
{
    int kind;

    if (ai_Tutorial->goalDone >= 0 && p[0] == 0x28 && p[3] == 0x155) {
        ai_Tutorial->goalDone = 1;
        return 1;
    }
    if (ai_Tutorial->goalDone >= 1 && p[0] == 0x27 && p[3] == 0x1d0) {
        ai_Tutorial->goalDone = 2;
        return 1;
    }
    if (ai_Tutorial->goalDone >= 2 && p[1] && p[0] == 0x24) {
        kind = ((AIBLD *)p[1])->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind == 0x12
                && ((AIBLD *)p[1])->owner == ai_PlayerId) {
            ai_Tutorial->goalDone = 3;
            return 1;
        }
    }
    if (p[0] == 0x1a || p[0] == 0x1c)
        return 1;
    return 0;
}
/* 0x5773f0, the marker step callback of the run record FTInitChapter3Step8 (ai_TutorialMarkerUpdate): picks one
   of its five marker entries by the shown panel room (0x92 .. 0x97) and the goal progress goalDone - 0: 0 for
   the room 0x92, 3 for the others; 1: 1 for the room 0x95, 4 for the others; 2: 2; -1 (no marker) without a
   shown panel, for other rooms or another goalDone */

int ai_MarkerChapter3Step8(int mode)
{
    unsigned short room;
    int r;
    int done;

    if (sv_PanelShown) {
        room = *sv_PanelShown;
        r = room;
        if (r >= 0x92 && r <= 0x97) {
            done = ai_Tutorial->goalDone;
            if (done == 0)
                return room == 0x92 ? 0 : 3;
            if (done == 1)
                return room == 0x95 ? 1 : 4;
            if (done == 2)
                return done;
        }
    }
    return -1;
}
/* 0x577450, a step predicate: 1 for the kind 0x26 with the step word 0x1d0 while the goalDone flag is not negative
   (which sets the flag to 1), and for the kinds 0x1a / 0x1c; else 0 */

int ai_QueryChapter3Step10(int *p)
{
    if (ai_Tutorial->goalDone >= 0 && p[0] == 0x26 && p[3] == 0x1d0) {
        ai_Tutorial->goalDone = 1;
        return 1;
    }
    if (p[0] == 0x1a || p[0] == 0x1c)
        return 1;
    return 0;
}
/* 0x577490, releases the pending step argument (stepArg): with the step 2 through sv_BookForward, with the step 3
   through sv_BookBack; returns 0 */

int ai_StepShownChapter3Step4(void)
{
    if (ai_Tutorial->step == 2 && ai_Tutorial->stepArg)
        sv_BookForward(ai_Tutorial->stepArg);
    else if (ai_Tutorial->step == 3 && ai_Tutorial->stepArg)
        sv_BookBack(ai_Tutorial->stepArg);
    return 0;
}
/* 0x5774d0, when `arg` is 0 and the building 0x6f2308 is not already of the table byte 0x12: finds a record
   (0x501190 (1, 5, 0x12)), picks 0x4ffe90 (r+0x5d, 1, 0, 0x74) and passes it (0x521960 (r, q+2, 0x631090)) */

void ai_HookChapter3Intro(int arg)
{
    AIBLD *r;
    AIOBJ *q;
    int kind;

    if (arg)
        return;
    if (ai_PlayerBuilding) {
        kind = ((AIBLD *)ai_PlayerBuilding)->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind == 0x12)
            return;
    }
    r = (AIBLD *)gm_BuildQuery(1, 5, 0x12);
    if (!r)
        return;
    q = (AIOBJ *)gm_Query((void *)r->objList, 1, 0, 0x74);
    if (!q)
        return;
    sv_SelectPanelObject((unsigned char *)r, q->objId, ch_EmptyName);
}
/* 0x577540, when `arg` is 0 and the building 0x6f2308 is not already of the table byte 0x2: finds a record
   (0x501190 (2, 5, 2, 4, player) with the player word 0x6f25b8), picks 0x4ffe90 (r+0x5d, 1, 0, 0x13) and passes
   it (0x521960 (r, q+2, 0x631090)) */

void ai_HookChapter5Intro(int arg)
{
    AIBLD *r;
    AIOBJ *q;
    int kind;

    if (arg)
        return;
    if (ai_PlayerBuilding) {
        kind = ((AIBLD *)ai_PlayerBuilding)->type;
        if (((AIBTYPE *)ai_BTypes)[kind].kind == 2)
            return;
    }
    r = (AIBLD *)gm_BuildQuery(2, 5, 2, 4, ai_PlayerId);
    if (!r)
        return;
    q = (AIOBJ *)gm_Query((void *)r->objList, 1, 0, 0x13);
    if (!q)
        return;
    sv_SelectPanelObject((unsigned char *)r, q->objId, ch_EmptyName);
}
/* 0x5775c0, the run hook of the run record FTInitChapter5Step4: at the run's start (end 0) sends two messages for
   the player's character - cm_SendSetField (netId, 0x1c8, 4, 0x4000, 0) and cm_SendMoveGoodsUnchecked (netId, -1,
   0, 0x3e80) */

void ai_HookChapter5Step4(int end)
{
    AICHAR *chr;

    chr = &ai_CharsT[ai_PlayerId];
    if (end)
        return;
    cm_SendSetField(chr->netId, 0x1c8, 4, 0x4000, 0);
    cm_SendMoveGoodsUnchecked(chr->netId, -1, 0, 0x3e80);
}
/* 0x577610, the run hook of the run record FTInitChapter5Step5: at the run's start (end 0) announces the office of the
   kind 1 as free (amt_AnnounceOfficeFree); at its end (1) finds the first character of the kind 5 and, when amt_GetOfficeCandidates offers it an office,
   lets it apply for the first candidate's office (amt_ApplyForOffice) */

void ai_HookChapter5Step5(int end)
{
    AICHAR *chr;
    AMTCAND cand;
    int i;

    if (end == 0) {
        amt_AnnounceOfficeFree(1);
        return;
    }
    if (end != 1)
        return;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind == 5) {
            chr = &ai_CharsT[i];
            if (amt_GetOfficeCandidates((char *)chr, 1, &cand) > 0)
                amt_ApplyForOffice((char *)chr, cand.kind);
            return;
        }
    }
}
/* 0x577690, the run hook of the run record FTInitChapter5Step6: at the run's start (end 0), when
   amt_CheckOfficeColumns answers, sends a new scene party (cm_SendNewParty, a G28REC: kind 1, thing -1, the flags
   0xa0, now): its members are the characters of the kinds 6 and 7, and the (last) one of the kind 6 is its host */

void ai_HookChapter5Step6(int end)
{
    G28REC party;
    int i, n;

    if (end)
        return;
    if (!amt_CheckOfficeColumns())
        return;
    memset(&party, 0, sizeof party);
    n = 0;
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind == 6) {
            party.host = ai_CharsT[i].netId;
            party.members[n++] = ai_CharsT[i].netId;
        } else if (ai_CharsT[i].kind == 7)
            party.members[n++] = ai_CharsT[i].netId;
    }
    party.used = n;
    party.time = ai_Time;
    party.kind = 1;
    party.flags = 0xa0;
    party.thing = -1;
    cm_SendNewParty(&party);
}

/* 0x577740, the start of the tutorial's main event ("tutorial\main"): -4 without a host / with a running run
   record, else the event message 0x5ec (he_OpenEventSlot) with the form and picture 0x5ec, the run window's
   controls (d2_PlaceMainWindow, d2_CentreViewWindows, d2_SetForm (win, 0) / (win, 3)), sv_FitForm (d2_LastWindow),
   the intro text 0x7d (io_PrintText) and its object in continueObj (d2_GetWinObj (win, 3, text)); -1 without the
   run record, else 0. The host -> slot -> window chain is reloaded before every call as in the original */
int ai_TutorialOpenIntro(void)
{
    TUTHOST *host;

    host = ai_Tutorial->host;
    if (!host)
        return -4;
    if (host->slot)
        return -4;
    he_OpenEventSlot((int)host, 0, 0, "tutorial\\main", 0x5ec);
    if (!ai_Tutorial->host->slot)
        return -1;
    d2_PlaceMainWindowF(ai_Tutorial->host->slot->win, 3);
    d2_CentreViewWindows(ai_Tutorial->host->slot->win);
    d2_SetForm(ai_Tutorial->host->slot->win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(ai_Tutorial->host->slot->win, 3);
    ai_Tutorial->continueObj = d2_GetWinObj(ai_Tutorial->host->slot->win, 3,
            io_PrintText("$Z%s", 0x7d));
    return 0;
}
/* 0x577820, closes the tutorial's main event: -4 without a host / with no open event slot, else releases the slot
   (he_ReleaseSlot) and answers 0 */

int ai_TutorialCloseIntro(void)
{
    TUTHOST *host;

    host = ai_Tutorial->host;
    if (!host)
        return -4;
    if (!host->slot)
        return -4;
    he_ReleaseSlot((int)host);
    return 0;
}
/* 0x577850, clears the run window's two text controls: -4 without a host / with no open event slot, else sets the
   slot window's form 1 and 2, each followed by the "$C" text clear (io_PrintText); 0 */

int ai_TutorialClearRunWindow(void)
{
    TUTHOST *host;

    host = ai_Tutorial->host;
    if (!host)
        return -4;
    if (!host->slot)
        return -4;
    d2_SetForm(host->slot->win, 1);
    io_PrintText("$C");
    d2_SetForm(ai_Tutorial->host->slot->win, 2);
    io_PrintText("$C");
    return 0;
}
/* 0x5778b0, shows the current run's texts in the run window (the tutorial host's event slot window): clears it
   (ai_TutorialClearRunWindow), then control 1 / 2 get the run's mainText / mainText + 1 and its mainVoice is played
   (stopping the voice still playing); in the tails (states 0xa / 0xb) of the runs of the modes 1 / 2 the texts
   0x3fd3 / 0x3fd4 instead, without a voice. -4 without a run or a window, else 0 */

int ai_TutorialShowRunText(void)
{
    if (!ai_Tutorial->run)
        return -4;
    if (ai_TutorialClearRunWindow() == -4)
        return -4;
    if ((ai_Tutorial->run->mode == 1 || ai_Tutorial->run->mode == 2)
            && (ai_Tutorial->next == 0xa || ai_Tutorial->next == 0xb)) {
        d2_SetForm(ai_Tutorial->host->slot->win, 1);
        io_PrintText(0x3fd3);
        d2_SetForm(ai_Tutorial->host->slot->win, 2);
        io_PrintText(0x3fd4);
        return 0;
    }
    d2_SetForm(ai_Tutorial->host->slot->win, 1);
    io_PrintText(ai_Tutorial->run->mainText);
    d2_SetForm(ai_Tutorial->host->slot->win, 2);
    io_PrintText(ai_Tutorial->run->mainText + 1);
    if (ai_Tutorial->run->mainVoice) {
        if (ai_Tutorial->voice) {
            if (sb_IsPlaying(ai_Tutorial->voice))
                sb_Stop(ai_Tutorial->voice, 0);
            ai_Tutorial->voice = 0;
        }
        ai_Tutorial->voice = (struct SBSAMPLE *)snd_PlayVoice(-7, 0, ai_Tutorial->run->mainVoice, -1);
    }
    return 0;
}
/* 0x577a00, asks whether the player answered the tutorial's intro message: -4 without the host's event slot; 0
   while the active event window does not show an event of the kind 0x87 (the intro) or it is not answered yet;
   answered by a click on its continue object (continueObj) or, without a click, by the key 0x1c (Return) - then
   the voice still playing is stopped and 1 answers
   @deep read against the original in full and rewritten on TUTSTATE / HEEVSLOT / AIJOB (no null check of
   he_ActiveSlot itself, as the original; the shared return 0 through a label gives the original's jmp into
   the common jne). Only allocation is left: at 0x577a4f the voice pointer is loaded into eax (ours: mov
   eax, [ecx+0x14]; test eax, eax; push eax) where the original reuses ecx (mov ecx, [ecx+0x14]; test ecx,
   ecx; push ecx); diffclass calls the first two lines code only because the destination register is also
   the base register on the original's side, 45 vs 45 instructions, 3 bytes differ */

int ai_TutorialIntroAnswered(void)
{
    if (!ai_Tutorial->host)
        return -4;
    if (!ai_Tutorial->host->slot)
        return -4;
    if (he_ActiveSlot->ev && he_ActiveSlot->ev->kind == (char)0x87) {
#ifdef REA_START
        /* REA_START=tutorial (`uv run play|dbg --start tutorial`): accept the tutorial's intro message (the 0x87
           event, whose "Weiter" text object 0x577740 stores in S+0x20) without a mouse click, so a run without GUI
           automation gets past it. Jump straight to the answer like the keyboard arm does - forging a click in
           d2_ClickedId / d2_PointerObj would leak the phantom click into the rest of the frame. Without the flag
           the message waits for the real click / Return. */
        {
            char *rea = getenv("REA_START");
            if (rea != 0 && strcmp(rea, "tutorial") == 0)
                goto answered;
        }
#endif
        if (d2_ClickedId != -1) {
            if (d2_PointerObj != ai_Tutorial->continueObj)
                goto unanswered;
        } else if (kb_Last != 0x1c)
            goto unanswered;
#ifdef REA_START
answered:
#endif
        if (ai_Tutorial->voice) {
            if (sb_IsPlaying(ai_Tutorial->voice))
                sb_Stop(ai_Tutorial->voice, 0);
            ai_Tutorial->voice = 0;
        }
        return 1;
    }
unanswered:
    return 0;
}
/* 0x577a90, shows the run's first text window: nothing without a run record or its text1; else the status text
   is cleared and the window text1Win rewritten with the run's text1 and enabled, and when the run has a
   text1Voice, the playing voice is stopped and text1Voice played in its place */

void ai_TutorialShowText(void)
{
    if (!ai_Tutorial->run)
        return;
    if (!ai_Tutorial->run->text1)
        return;
    sv_SetStatusText(ch_EmptyName);
    d2_SetForm(ai_Tutorial->text1Win, 0);
    io_PrintText("$C");
    io_PrintText((char *)ai_Tutorial->run->text1);
    d2_SetViewEnabled(ai_Tutorial->text1Win, 1);
    if (!ai_Tutorial->run->text1Voice)
        return;
    if (ai_Tutorial->voice) {
        if (sb_IsPlaying(ai_Tutorial->voice))
            sb_Stop(ai_Tutorial->voice, 0);
        ai_Tutorial->voice = 0;
    }
    ai_Tutorial->voice = (struct SBSAMPLE *)snd_PlayVoice(-7, 0, ai_Tutorial->run->text1Voice, -1);
}
/* 0x577b60, clears the first text window text1Win: control 0 turned off (0x475f70), the text cleared to "$C"
   (0x602568, 0x515960) and the window disabled (0x475590); no return value */

void ai_TutorialHideText(void)
{
    d2_SetForm(ai_Tutorial->text1Win, 0);
    io_PrintText("$C");
    d2_SetViewEnabled(ai_Tutorial->text1Win, 0);
}
/* 0x577b90, shows the run's second text window, the sibling of ai_TutorialShowText: nothing without a run record
   or its text2; else the status text is cleared and the window text2Win rewritten with the run's text2 and
   enabled, and when the run has a text2Voice, the playing voice is stopped and text2Voice played in its place */

void ai_TutorialShowText2(void)
{
    if (!ai_Tutorial->run)
        return;
    if (!ai_Tutorial->run->text2)
        return;
    sv_SetStatusText(ch_EmptyName);
    d2_SetForm(ai_Tutorial->text2Win, 0);
    io_PrintText("$C");
    io_PrintText((char *)ai_Tutorial->run->text2);
    d2_SetViewEnabled(ai_Tutorial->text2Win, 1);
    if (!ai_Tutorial->run->text2Voice)
        return;
    if (ai_Tutorial->voice) {
        if (sb_IsPlaying(ai_Tutorial->voice))
            sb_Stop(ai_Tutorial->voice, 0);
        ai_Tutorial->voice = 0;
    }
    ai_Tutorial->voice = (struct SBSAMPLE *)snd_PlayVoice(-7, 0, ai_Tutorial->run->text2Voice, -1);
}
/* 0x577c60, clears the second text window text2Win: control 0 turned off (0x475f70), the text cleared to "$C"
   (0x602568, 0x515960) and the window disabled (0x475590); then the playing voice is stopped (0x426f20 /
   0x427290) and cleared; no return value */

void ai_TutorialHideText2(void)
{
    d2_SetForm(ai_Tutorial->text2Win, 0);
    io_PrintText("$C");
    d2_SetViewEnabled(ai_Tutorial->text2Win, 0);
    if (ai_Tutorial->voice) {
        if (sb_IsPlaying(ai_Tutorial->voice))
            sb_Stop(ai_Tutorial->voice, 0);
        ai_Tutorial->voice = 0;
    }
}
/* 0x577cd0, creates the tutorial progress form ("tutorial\progress"): the form id (0x4741f0) into progressForm,
   3 across (0x475850), its control 0 off (0x475f70), and the slider into progressSlider (0x46a960, x, 5, 400, 0, 400,
   0x75, 0x42, d2_LastWindow) at x = the last window's width / 2 - 200 (centered on the 400 wide slider) */

void ai_TutorialOpenProgress(void)
{
    int i;

    ai_Tutorial->progressForm = d2_OpenForm(0, 0, "tutorial\\progress");
    d2_PlaceMainWindowF(ai_Tutorial->progressForm, 3);
    d2_SetForm(ai_Tutorial->progressForm, 0);
    i = d2_LastWindow;
    ai_Tutorial->progressSlider = d2_WinOpenSlider(d2_Wins[i].w / 2 - 0xc8, 5, 0x190, 0, 0x190,
                                 0x75, 0x42, i);
}
/* 0x577d60, releases the tutorial progress form (36 B, a Ghidra-missed start after 0x577cd0's nop fill): nothing
   while progressForm is -1; else the view is closed (0x4758f0) and progressForm reset to -1 */

void ai_TutorialCloseProgress(void)
{
    if (ai_Tutorial->progressForm != -1) {
        d2_CloseView(ai_Tutorial->progressForm);
        ai_Tutorial->progressForm = -1;
    }
}


#define TUT_SINCE(at) (d3_Time - (at) ? d3_Time - (at) : 0)
#define MINU(a, b) ((a) < (b) ? (a) : (b))
/* 0x577d90, updates the tutorial progress slider of the state 0xa: without a run record or in another state the
   progress form is released (ai_TutorialCloseProgress); else the form is opened when it is not, and its slider set to the
   part of the run's tailTime still left since stepAt, scaled to 0 .. 400
   @deep read whole function against 0x577d90..0x577e43: calls, constants (400, 1, 0/0x190), the
   double-evaluated TUT_SINCE/MINU macros with the volatile clock and the signed fild/fidiv all match.
   Left: eax/ecx swapped (--regs eax=ecx,ecx=eax clears it) and the slot use at the float conversion: the
   original stores a dead 0 to [esp+8] (577dfa mov dword ptr [esp+8],0), then reuses one slot for left and
   total (577e06 mov [esp+8],edx; fild; 577e0e mov [esp+8],ecx; fidiv [esp+8]), ours keeps two slots (mov
   [esp+8],ecx / mov [esp+4],ecx; fidiv [esp+0xc]). Tried: (float)(int)(__int64), __int64 local, unsigned
   total, (float)total, multiply-first, inline expression: the dead store never appears (qword forms grow
   the frame to 0xc) */

void ai_TutorialProgressSlider(void)
{
    unsigned int t;
    int total;
    int left;

    if (!ai_Tutorial->run || ai_Tutorial->state != 0xa) {
        ai_TutorialCloseProgress();
        return;
    }
    if (ai_Tutorial->progressForm == -1)
        ai_TutorialOpenProgress();
    t = MINU(TUT_SINCE(ai_Tutorial->stepAt), ai_Tutorial->run->tailTime);
    total = ai_Tutorial->run->tailTime;
    left = total - t;
    d2_SetObjValue(ai_Tutorial->progressSlider, 0, 400, (int)((float)left / total * 400.0f), 1);
}
/* 0x577e50, shows the form of the run's current step: -4 without a run record; nothing (0) for a run without
   steps. Past the last step the run's stepShown callback runs and an open step form is closed (stepForm,
   stepControl -1, stepKind 4: none); when none was open, the original goes on to show the step past the end
   (it reads the step record behind the chain). Otherwise the step form of the step's kind (0 left, 1 right, 2 top, 3 bottom; -4 for another
   kind) is opened unless it is already open, when only its controls 1 / 2 are cleared; control 1 gets the step's
   text, control 2 the continue text 0x7d whose object becomes stepControl; the step's voice replaces the playing
   one, and the run's stepShown callback runs
   @deep read against 0x577e50..0x5780c8 statement by statement: the stepCount==0 early return, the done
   path that falls into the display when no form is open (577e8e je 0x577ebc), the four form literals
   (switch tables equal), the $Z%s/0x7d control, the voice re-read and the stepShown calls all match; 199
   vs 199 instructions, every value reloaded through ai_Tutorial as in the original. Left is register
   choice and scheduling only: 577f2d (ours mov eax,[ai_Tutorial]; mov [eax+0x44],4; mov eax,[ai_Tutorial];
   mov ecx,[eax+0x18] vs mov ecx,[ai_Tutorial]; mov [ecx+0x44],4; mov eax,...; mov edx,[eax+0x18]), 577fa5
   (the add esp,0xc / add esp,8 placed one instruction later in ours around d2_PlaceMainWindow, eax/ecx/edx
   rotated), and the regs hunks 577f5b/578007/57801c/578025 */

int ai_TutorialStepForm(void)
{
    if (!ai_Tutorial->run)
        return -4;
    if (!ai_Tutorial->run->stepCount)
        return 0;
    if (ai_Tutorial->step >= ai_Tutorial->run->stepCount) {
        if (ai_Tutorial->run->stepShown)
            ai_Tutorial->run->stepShown();
        if (ai_Tutorial->stepForm != -1) {
            d2_CloseView(ai_Tutorial->stepForm);
            ai_Tutorial->stepForm = -1;
            ai_Tutorial->stepControl = -1;
            ai_Tutorial->stepKind = 4;
            return 0;
        }
    }
    if (ai_Tutorial->stepKind == ai_Tutorial->run->steps[ai_Tutorial->step].kind) {
        d2_SetForm(ai_Tutorial->stepForm, 1);
        io_PrintText("$C");
        d2_SetForm(ai_Tutorial->stepForm, 2);
        io_PrintText("$C");
    } else {
        if (ai_Tutorial->stepForm != -1) {
            d2_CloseView(ai_Tutorial->stepForm);
            ai_Tutorial->stepForm = -1;
        }
        ai_Tutorial->stepControl = -1;
        ai_Tutorial->stepKind = 4;
        if (ai_Tutorial->run->steps[ai_Tutorial->step].kind >= 4)
            return -4;
        switch (ai_Tutorial->run->steps[ai_Tutorial->step].kind) {
        case 0: ai_Tutorial->stepForm = d2_OpenForm(0, 0, "tutorial\\left_form"); break;
        case 1: ai_Tutorial->stepForm = d2_OpenForm(0, 0, "tutorial\\right_form"); break;
        case 2: ai_Tutorial->stepForm = d2_OpenForm(0, 0, "tutorial\\top_form"); break;
        case 3: ai_Tutorial->stepForm = d2_OpenForm(0, 0, "tutorial\\bottom_form"); break;
        }
        d2_PlaceMainWindowF(ai_Tutorial->stepForm, 3);
        ai_Tutorial->stepKind = ai_Tutorial->run->steps[ai_Tutorial->step].kind;
    }
    d2_SetForm(ai_Tutorial->stepForm, 1);
    io_PrintText((char *)ai_Tutorial->run->steps[ai_Tutorial->step].text);
    d2_SetForm(ai_Tutorial->stepForm, 2);
    ai_Tutorial->stepControl = d2_GetWinObj(ai_Tutorial->stepForm, 2, io_PrintText("$Z%s", 0x7d));
    if (ai_Tutorial->run->steps[ai_Tutorial->step].voice) {
        if (ai_Tutorial->voice) {
            if (sb_IsPlaying(ai_Tutorial->voice))
                sb_Stop(ai_Tutorial->voice, 0);
            ai_Tutorial->voice = 0;
        }
        ai_Tutorial->voice = (struct SBSAMPLE *)snd_PlayVoice(-7, 0, ai_Tutorial->run->steps[ai_Tutorial->step].voice, -1);
    }
    if (ai_Tutorial->run->stepShown)
        ai_Tutorial->run->stepShown();
    return 0;
}
/* 0x5780e0, predicate (49 B): -4 without the gate 0x6f265c; 0 while the input is idle (d2_ClickedId == -1) or
   the clicked object (d2_PointerObj) is not the open step form's control (stepControl); 1 when it is */

int ai_TutorialStepClicked(void)
{
    if (!ai_TutorialOn)
        return -4;
    if (d2_ClickedId != -1 && d2_PointerObj == ai_Tutorial->stepControl)
        return 1;
    return 0;
}
/* 0x578120, attaches the tutorial's event record (host, the tutorial intro event 0x87's record) and resets the
   intro window's control object (continueObj) to -1 (43 B): -4 without the gate 0x6f265c, else 0 */

int ai_TutorialSetHost(char *per)
{
    if (!ai_TutorialOn)
        return -4;
    ai_Tutorial->host = (TUTHOST *)per;
    ai_Tutorial->continueObj = -1;
    return 0;
}
/* 0x578150, detaches the tutorial's event record (host) and resets the intro window's control object (continueObj)
   to -1 (26 B, no return value) */

void ai_TutorialClearHost(void)
{
    ai_Tutorial->host = 0;
    ai_Tutorial->continueObj = -1;
}
/* 0x578170, the tutorial-off predicate (14 B): 1 while the tutorial does not run (ai_TutorialOn clear), else 0 */

int ai_TutorialOff(void)
{
    return !ai_TutorialOn;
}

int ai_TutorialInitMainIntro();
int ai_TutorialInitMainOutro();
void ai_TutorialStop();
int sb_UnloadSampleBank();
int snd_LoadVoiceBank();
void * mem_Alloc(int size, char *tag);
void ai_TutorialFreeRuns(TUTRUN *rec);

/* 0x578180, starts the tutorial (called from 0x53bb20): -3 while it runs, -2 when no chapter is played; the run
   chain ai_TutorialRuns is built from the main intro, the chain of every chapter played (ai_TutorialChapterOn /
   ai_TutorialChapterInit, each appended at the tail of the chain) and the main outro; a failed builder stops the
   tutorial again (ai_TutorialStop) and gives -1. Then the state record is reset to the state 1 with the first run,
   both text windows opened hidden ("tutorial\inline_form", "tutorial\aux_form"), the progress form closed, and
   the tutorial set running; 0 */

int ai_TutorialStart(void)
{
    TUTRUN *tail;
    TUTRUN *r;
    int cnt;
    int i;

    if (ai_TutorialOn != 0)
        return -3;
    cnt = 0;
    for (i = 0; i < 5; i++) {
        if (ai_TutorialChapterOn[i] == 1)
            cnt++;
    }
    if (!cnt)
        return -2;
    ai_TutorialRuns = (TUTRUN *)ai_TutorialInitMainIntro();
    if (!ai_TutorialRuns)
        goto fail;
    for (tail = ai_TutorialRuns; tail->next; tail = tail->next)
        ;
    for (i = 0; i < 5; i++) {
        if (ai_TutorialChapterOn[i] == 1) {
            r = (TUTRUN *)ai_TutorialChapterInit[i]();
            tail->next = r;
            if (!r)
                goto fail;
            while (tail->next)
                tail = tail->next;
        }
    }
    r = (TUTRUN *)ai_TutorialInitMainOutro();
    tail->next = r;
    if (!r)
        goto fail;
    ai_Tutorial->state = 1;
    ai_Tutorial->next = 1;
    ai_Tutorial->textAt = d3_Time;
    ai_Tutorial->stepAt = d3_Time;
    ai_Tutorial->campaignSteps = 0;
    ai_Tutorial->voice = 0;
    ai_Tutorial->run = ai_TutorialRuns;
    ai_Tutorial->voice = 0;
    ai_Tutorial->host = 0;
    ai_Tutorial->continueObj = -1;
    ai_Tutorial->text1Win = d2_OpenForm(0, 0, "tutorial\\inline_form");
    d2_SetViewEnabled(ai_Tutorial->text1Win, 0);
    d2_PlaceMainWindowF(ai_Tutorial->text1Win, 3);
    ai_Tutorial->text1Shown = 0;
    ai_Tutorial->statusShown = 0;
    ai_Tutorial->text2Shown = 0;
    ai_Tutorial->text2Win = d2_OpenForm(0, 0, "tutorial\\aux_form");
    d2_SetViewEnabled(ai_Tutorial->text2Win, 0);
    d2_PlaceMainWindowF(ai_Tutorial->text2Win, 3);
    ai_Tutorial->phase = 0;
    ai_Tutorial->step = 0;
    ai_Tutorial->markerPhase = 0;
    ai_Tutorial->markerStep = 0;
    ai_Tutorial->markerAt = 0;
    ai_Tutorial->markerObj = -1;
    ai_Tutorial->progressForm = -1;
    ai_Tutorial->progressSlider = -1;
    ai_TutorialOn = 1;
    ai_TutorialRunning = 1;
    return 0;
fail:
    ai_TutorialStop();
    return -1;
}
/* 0x578370, the tutorial stop (196 B): the six voice banks (0x6f2640) unloaded when set (0x426ba0), the run
   record chain (ai_TutorialRuns, 0x6f2658) freed by 0x578440 and cleared; the state record 0x5eec60 reset (state,
   textAt, stepAt, campaignSteps, voice, run, host = 0, continueObj = -1, the windows text1Win / text2Win released
   (0x4758f0) and set to -1, voice cleared again), the gates ai_TutorialOn / ai_TutorialRunning = 0; no return
   value */

void ai_TutorialStop(void)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (ai_TutorialBanks[i])
            sb_UnloadSampleBank(ai_TutorialBanks[i]);
    }
    ai_TutorialFreeRuns(ai_TutorialRuns);
    ai_TutorialRuns = 0;
    ai_Tutorial->state = 0;
    ai_Tutorial->textAt = 0;
    ai_Tutorial->stepAt = 0;
    ai_Tutorial->campaignSteps = 0;
    ai_Tutorial->voice = 0;
    ai_Tutorial->run = 0;
    ai_Tutorial->host = 0;
    ai_Tutorial->continueObj = -1;
    d2_CloseView(ai_Tutorial->text1Win);
    ai_Tutorial->text1Win = -1;
    d2_CloseView(ai_Tutorial->text2Win);
    ai_Tutorial->text2Win = -1;
    ai_Tutorial->voice = 0;
    ai_TutorialOn = 0;
    ai_TutorialRunning = 0;
}
/* 0x578440, frees the tutorial record chain (82 B, recursive): nothing for a null record; else its next record,
   then its step table (steps) and marker table (marks) (mem_Free, both cleared) and then the record itself */

void ai_TutorialFreeRuns(TUTRUN *rec)
{
    if (!rec)
        return;
    if (rec->next)
        ai_TutorialFreeRuns(rec->next);
    if (rec->steps) {
        mem_Free(rec->steps);
        rec->steps = 0;
    }
    if (rec->marks) {
        mem_Free(rec->marks);
        rec->marks = 0;
    }
    mem_Free(rec);
}
/* 0x5784a0, allocates the run record of the tutorial's main intro ("FTInitMainIntro"): 0 when that fails (the
   chain freed: nothing); else the zeroed record as the run 0 'INTR' of the mode 4 with the text 0x3f24 and the
   voice "TUTORIAL_MAIN_INTRO", no text windows, the tail of 0x8e ms and the goal state 0xb; its voice bank
   "TUTORIAL_INTRO_OUTRO.sbf" is loaded into ai_TutorialBanks[0]; the record */

int ai_TutorialInitMainIntro(void)
{
    TUTRUN *rec;

    rec = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitMainIntro");
    if (!rec) {
        ai_TutorialFreeRuns(0);
        return 0;
    }
    AI_ZERO_AT(rec, sizeof(TUTRUN));
    rec->id = 0;
    rec->tag = 'INTR';
    rec->mode = 4;
    rec->mainText = 0x3f24;
    rec->mainVoice = "TUTORIAL_MAIN_INTRO";
    rec->text1 = 0;
    rec->text1Voice = 0;
    rec->text2 = 0;
    rec->text2Voice = 0;
    rec->text1Time = -1;
    rec->tail2Time = 0x8e;
    rec->hook = 0;
    rec->stepHook = 0;
    rec->goal = 0;
    rec->tailTime = 0;
    rec->goalState = 0xb;
    rec->allowMask = 0;
    rec->allowMask2 = 0;
    rec->next = 0;
    ai_TutorialBanks[0] = snd_LoadVoiceBank("TUTORIAL_INTRO_OUTRO.sbf", 0);
    return (int)rec;
}
/* 0x578570, builds the run chain of the tutorial's chapter 1 (ai_TutorialChapterInit[0]): eleven run records
   (the intro 'ntr1', the steps 'c1sA' .. 'c1sJ', the outro 'utr1'), each allocated under its "FTInitChapter1*"
   tag, zeroed and filled with its texts / voices ("CHAPTER_1_*"), times, callbacks, goal and allow masks, and
   linked through next; the voice bank "TUTORIAL_CHAPTER_1.sbf" is loaded into ai_TutorialBanks[1]. The first
   record, or 0 after a failed allocation (the records built so far freed) */

int ai_TutorialInitChapter1(void)
{
    TUTRUN *head;
    TUTRUN *r;

    head = 0;
    r = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Intro");
    if (!r)
        goto fail;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'ntr1';
    r->mode = 0;
    r->mainText = 0x3f2a;
    r->mainVoice = "CHAPTER_1_INTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0;
    r->allowMask2 = 0;
    r->next = 0;
    head = r;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step1");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sA';
    r->mode = 1;
    r->mainText = 0x3f2c;
    r->mainVoice = "CHAPTER_1_A_MAIN";
    r->text1 = 0x3f2e;
    r->text1Voice = "CHAPTER_1_A_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f2f;
    r->voice = "CHAPTER_1_A_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x163;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0x16;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step3");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sC';
    r->mode = 2;
    r->mainText = 0x3f30;
    r->mainVoice = "CHAPTER_1_C_MAIN";
    r->text1 = 0x3f33;
    r->text1Voice = "CHAPTER_1_C_REMIND";
    r->text2 = 0x3f32;
    r->text2Voice = "Chapter_1_C_Aux";
    r->statusText = 0x3f34;
    r->voice = "CHAPTER_1_C_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 6;
    r->tailTime = 0x2148;
    r->goalState = 9;
    r->allowMask = 1;
    r->allowMask2 = 0;
    r->text2Goal = 7;
    r->stepCount = 0;
    r->stepShown = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step4");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sD';
    r->mode = 2;
    r->mainText = 0x3f35;
    r->mainVoice = "CHAPTER_1_D_MAIN";
    r->text1 = 0x3f38;
    r->text1Voice = "CHAPTER_1_D_REMIND";
    r->text2 = 0x3f37;
    r->text2Voice = "Chapter_1_D_Aux";
    r->statusText = 0x3f39;
    r->voice = "CHAPTER_1_D_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 8;
    r->tailTime = 0x2148;
    r->goalState = 9;
    r->allowMask = 3;
    r->allowMask2 = 0;
    r->text2Goal = 9;
    r->stepCount = 0;
    r->stepShown = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step5");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sE';
    r->mode = 2;
    r->mainText = 0x3f3a;
    r->mainVoice = "CHAPTER_1_E_MAIN";
    r->text1 = 0x3f3d;
    r->text1Voice = "CHAPTER_1_E_REMIND";
    r->text2 = 0x3f3c;
    r->text2Voice = "Chapter_1_E_Aux";
    r->statusText = 0x3f3e;
    r->voice = "CHAPTER_1_E_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = ai_StepChapter1Step5;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 9;
    r->allowMask = 7;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step6");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sF';
    r->mode = 1;
    r->mainText = 0x3f3f;
    r->mainVoice = "CHAPTER_1_F_MAIN";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = 0x429;
    r->tail2Time = 0x47;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 3;
    r->tailTime = 0x47;
    r->goalState = 0xb;
    r->allowMask = 7;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step7");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sG';
    r->mode = 1;
    r->mainText = 0x3f41;
    r->mainVoice = "CHAPTER_1_G_MAIN";
    r->text1 = 0x3f43;
    r->text1Voice = "CHAPTER_1_G_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f44;
    r->voice = "CHAPTER_1_G_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0xc;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x17;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step8");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sH';
    r->mode = 1;
    r->mainText = 0x3f45;
    r->mainVoice = "CHAPTER_1_H_MAIN";
    r->text1 = 0x3f47;
    r->text1Voice = "CHAPTER_1_H_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f48;
    r->voice = "CHAPTER_1_H_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0x13;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x817;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step9");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sI';
    r->mode = 1;
    r->mainText = 0x3f49;
    r->mainVoice = "CHAPTER_1_I_MAIN";
    r->text1 = 0x3f4b;
    r->text1Voice = "CHAPTER_1_I_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f4c;
    r->voice = "CHAPTER_1_I_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0x14;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x1817;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Step10");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c1sJ';
    r->mode = 1;
    r->mainText = 0x3f4d;
    r->mainVoice = "CHAPTER_1_J_MAIN";
    r->text1 = 0x3f4f;
    r->text1Voice = "CHAPTER_1_J_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f50;
    r->voice = "CHAPTER_1_J_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x163;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = ai_StepChapter1Step10;
    r->goal = 0xd;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x1837;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter1Outro");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'utr1';
    r->mode = 3;
    r->mainText = 0x3f51;
    r->mainVoice = "CHAPTER_1_OUTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x163;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x1837;
    r->allowMask2 = 0;
    r->next = 0;
    ai_TutorialBanks[1] = snd_LoadVoiceBank("TUTORIAL_CHAPTER_1.sbf", 0);
    return (int)head;
fail:
    ai_TutorialFreeRuns(head);
    return 0;
}
/* 0x578e40, builds the run chain of the tutorial's chapter 2 (ai_TutorialChapterInit[1]): nine run records (the
   intro 'ntr2', the steps 'c2sA' .. 'c2sG' with their hooks and step callbacks, the outro 'utr2'), each
   allocated under its "FTInitChapter2*" tag, zeroed and filled with its texts / voices ("CHAPTER_2_*"), times,
   callbacks, goal and allow masks, and linked through next; the voice bank "TUTORIAL_CHAPTER_2.sbf" is loaded
   into ai_TutorialBanks[2]. The first record, or 0 after a failed allocation (the records built so far freed) */

int ai_TutorialInitChapter2(void)
{
    TUTRUN *head;
    TUTRUN *r;

    head = 0;
    r = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Intro");
    if (!r)
        goto fail;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'ntr2';
    r->mode = 0;
    r->mainText = 0x3f53;
    r->mainVoice = "CHAPTER_2_INTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x1837;
    r->allowMask2 = 0;
    r->next = 0;
    head = r;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Step1");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'c2sA';
    r->mode = 1;
    r->mainText = 0x3f55;
    r->mainVoice = "CHAPTER_2_A_MAIN";
    r->text1 = 0x3f57;
    r->text1Voice = "CHAPTER_2_A_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f58;
    r->voice = "CHAPTER_2_A_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_TutorialQueryDefault;
    r->hook = 0;
    r->stepHook = ai_StepChapter2Step1;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x1837;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Step2");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'c2sB';
    r->mode = 1;
    r->mainText = 0x3f59;
    r->mainVoice = "CHAPTER_2_B_MAIN";
    r->text1 = 0x3f5b;
    r->text1Voice = "CHAPTER_2_B_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f5c;
    r->voice = "CHAPTER_2_B_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0xd5;
    r->query = ai_QueryBuilding0f;
    r->hook = ai_HookChapter2Step2;
    r->stepHook = ai_StepChapter2Step2;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xb;
    r->allowMask = 0x5837;
    r->allowMask2 = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Step3");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'c2sC';
    r->mode = 1;
    r->mainText = 0x3f5d;
    r->mainVoice = "CHAPTER_2_C_MAIN";
    r->text1 = 0x3f5f;
    r->text1Voice = "CHAPTER_2_C_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f60;
    r->voice = "CHAPTER_2_C_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_QueryChapter2Step3;
    r->hook = 0;
    r->stepHook = ai_StepChapter2Step3;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x5837;
    r->allowMask2 = 0xe00;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Step4");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'c2sD';
    r->mode = 1;
    r->mainText = 0x3f61;
    r->mainVoice = "CHAPTER_2_C1_MAIN";
    r->text1 = 0x3f63;
    r->text1Voice = "CHAPTER_2_C1_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f64;
    r->voice = "CHAPTER_2_C1_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->hook = 0;
    r->query = ai_QueryChapter2Step4;
    r->stepHook = ai_StepChapter2Step4;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e00;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Step5");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'c2sE';
    r->mode = 1;
    r->mainText = 0x3f65;
    r->mainVoice = "CHAPTER_2_D_MAIN";
    r->text1 = 0x3f67;
    r->text1Voice = "CHAPTER_2_D_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f68;
    r->voice = "CHAPTER_2_D_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_QueryChapter2Step5;
    r->hook = ai_HookChapter2Step5;
    r->stepHook = ai_StepChapter2Step5;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e00;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Step6");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'c2sF';
    r->mode = 1;
    r->mainText = 0x3f69;
    r->mainVoice = "CHAPTER_2_E_MAIN";
    r->text1 = 0x3f6b;
    r->text1Voice = "CHAPTER_2_E_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3f6c;
    r->voice = "CHAPTER_2_E_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_QueryChapter2Step6;
    r->hook = ai_HookChapter2Step5;
    r->stepHook = ai_StepChapter2Step6;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e00;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Step7");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'c2sG';
    r->mode = 1;
    r->mainText = 0x3f6d;
    r->mainVoice = "CHAPTER_2_F_MAIN";
    r->text1 = 0x3f6f;
    r->text1Voice = "CHAPTER_2_F_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_QueryChapter2Step7;
    r->hook = 0;
    r->stepHook = ai_StepChapter2Step7;
    r->goal = 1;
    r->tailTime = 0x429;
    r->goalState = 0xa;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e00;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Outro");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 2;
    r->tag = 'utr2';
    r->mode = 3;
    r->mainText = 0x3f70;
    r->mainVoice = "CHAPTER_2_OUTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x163;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e00;
    r->next = 0;
    ai_TutorialBanks[2] = snd_LoadVoiceBank("TUTORIAL_CHAPTER_2.sbf", 0);
    return (int)head;
fail:
    ai_TutorialFreeRuns(head);
    return 0;
}

/* 0x579570, builds the run chain of the tutorial's chapter 3 (ai_TutorialChapterInit[2]): ten run records (the
   intro 'ntr3', the steps 'c3s1' .. 'c3sA', the outro 'utr3'), each allocated under its "FTInitChapter3*" tag
   (the intro under the copy-pasted "FTInitChapter2Intro"), zeroed and filled with its texts / voices
   ("CHAPTER_3_*"), times, callbacks, goal and allow masks, and linked through next. The steps 1, 2, 4 and 6 get
   a step table ("FTInitScriptChapter3Step*"), the steps 2, 6, 8 and 10 the position marker's path
   ("FTInitAnimChapter3Step*"), whose points depend on the screen width (800, 1024, else). The voice bank
   "TUTORIAL_CHAPTER_3.sbf" is loaded into ai_TutorialBanks[3]. The first record, or 0 after a failed allocation
   (the records built so far freed) */

int ai_TutorialInitChapter3(void)
{
    TUTRUN *head;
    TUTRUN *r;

    head = 0;
    r = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter2Intro");
    if (!r)
        goto fail;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'ntr3';
    r->mode = 0;
    r->mainText = 0x3f72;
    r->mainVoice = "CHAPTER_3_INTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x8e;
    r->hook = ai_HookChapter3Intro;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e00;
    r->next = 0;
    head = r;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Step1");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'c3s1';
    r->mode = 2;
    r->mainText = 0x3f74;
    r->mainVoice = "CHAPTER_3_A_MAIN";
    r->text1 = 0x3f7a;
    r->text1Voice = "CHAPTER_3_A_REMIND";
    r->statusText = 0x3f79;
    r->voice = "CHAPTER_3_A_DONE";
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = 0x429;
    r->tail2Time = 0xd5;
    r->query = ai_QueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0x1d;
    r->tailTime = 0x58c;
    r->goalState = 0xb;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e10;
    r->text2Goal = 0;
    r->stepCount = 3;
    r->stepShown = 0;
    r->steps = (TUTSTEP *)mem_Alloc(3 * sizeof(TUTSTEP), "FTInitScriptChapter3Step1");
    if (!r->steps)
        goto fail;
    r->steps[0].delay = 0x2c6;
    r->steps[0].kind = 1;
    r->steps[0].text = 0x3f76;
    r->steps[0].voice = "CHAPTER_3_A_STEP_RIGHT";
    r->steps[1].delay = 0x2c6;
    r->steps[1].kind = 0;
    r->steps[1].text = 0x3f77;
    r->steps[1].voice = "CHAPTER_3_A_STEP_LEFT";
    r->steps[2].delay = 0x2c6;
    r->steps[2].kind = 2;
    r->steps[2].text = 0x3f78;
    r->steps[2].voice = "CHAPTER_3_A_STEP_TOP";
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Step2");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'c3s2';
    r->mode = 2;
    r->mainText = 0x3f7b;
    r->mainVoice = "CHAPTER_3_B_MAIN";
    r->text1 = 0x3f80;
    r->text1Voice = "CHAPTER_3_B_REMIND";
    r->text2 = 0x3f7f;
    r->text2Voice = "Chapter_3_B_Step_Aux";
    r->statusText = 0x3f81;
    r->voice = "CHAPTER_3_B_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0xd5;
    r->hook = ai_HookChapter3Step2;
    r->stepHook = 0;
    r->query = ai_QueryChapter3Step2;
    r->goal = 0x1f;
    r->tailTime = 0x58c;
    r->goalState = 9;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e30;
    r->text2Goal = 0x20;
    r->stepCount = 2;
    r->stepShown = 0;
    r->steps = (TUTSTEP *)mem_Alloc(2 * sizeof(TUTSTEP), "FTInitScriptChapter3Step2");
    if (!r->steps)
        goto fail;
    r->steps[0].delay = 0x2c6;
    r->steps[0].kind = 1;
    r->steps[0].text = 0x3f7d;
    r->steps[0].voice = "CHAPTER_3_B_STEP_RIGHT";
    r->steps[1].delay = 0x2c6;
    r->steps[1].kind = 0;
    r->steps[1].text = 0x3f7e;
    r->steps[1].voice = "CHAPTER_3_B_STEP_LEFT";
    r->markCount = 1;
    r->marks = (TUTMARK *)mem_Alloc(1 * sizeof(TUTMARK), "FTInitAnimChapter3Step2");
    if (!r->marks)
        goto fail;
    if (d2_ScreenW == 0x320) {
        r->marks[0].from[0] = 695.0f;
        r->marks[0].from[1] = 300.0f;
        r->marks[0].ctrl1[0] = 626.0f;
        r->marks[0].ctrl1[1] = 400.0f;
        r->marks[0].ctrl2[0] = 366.0f;
        r->marks[0].ctrl2[1] = 380.0f;
        r->marks[0].to[0] = 376.0f;
        r->marks[0].to[1] = 289.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[0].from[0] = 920.0f;
        r->marks[0].from[1] = 300.0f;
        r->marks[0].ctrl1[0] = 846.0f;
        r->marks[0].ctrl1[1] = 440.0f;
        r->marks[0].ctrl2[0] = 516.0f;
        r->marks[0].ctrl2[1] = 450.0f;
        r->marks[0].to[0] = 480.0f;
        r->marks[0].to[1] = 375.0f;
    } else {
        r->marks[0].from[0] = 1026.0f;
        r->marks[0].from[1] = 320.0f;
        r->marks[0].ctrl1[0] = 986.0f;
        r->marks[0].ctrl1[1] = 440.0f;
        r->marks[0].ctrl2[0] = 656.0f;
        r->marks[0].ctrl2[1] = 600.0f;
        r->marks[0].to[0] = 526.0f;
        r->marks[0].to[1] = 440.0f;
    }
    r->marks[0].wait = 35.0f;
    r->marks[0].travel = 142.0f;
    r->marks[0].rest = 71.0f;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Step4");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'c3s4';
    r->mode = 2;
    r->mainText = 0x3f82;
    r->mainVoice = "CHAPTER_3_D_MAIN";
    r->text1 = 0x3f88;
    r->text1Voice = "CHAPTER_3_D_REMIND";
    r->text2 = 0x3f87;
    r->text2Voice = "Chapter_3_D_Aux";
    r->statusText = 0x3f89;
    r->voice = "CHAPTER_3_D_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->query = ai_QueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0x21;
    r->tailTime = 0x58c;
    r->goalState = 0xa;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1e70;
    r->text2Goal = 0x22;
    r->stepCount = 3;
    r->stepShown = ai_StepShownChapter3Step4;
    r->steps = (TUTSTEP *)mem_Alloc(3 * sizeof(TUTSTEP), "FTInitScriptChapter3Step4");
    if (!r->steps)
        goto fail;
    r->steps[0].delay = 0x2c6;
    r->steps[0].kind = 1;
    r->steps[0].text = 0x3f84;
    r->steps[0].voice = "CHAPTER_3_D_STEP_RIGHT";
    r->steps[1].delay = 0x2c6;
    r->steps[1].kind = 0;
    r->steps[1].text = 0x3f85;
    r->steps[1].voice = "CHAPTER_3_D_STEP_LEFT";
    r->steps[2].delay = 0x2c6;
    r->steps[2].kind = 0;
    r->steps[2].text = 0x3f86;
    r->steps[2].voice = "CHAPTER_3_D_STEP_NEXT_LEFT";
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Step6");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'c3s6';
    r->mode = 2;
    r->mainText = 0x3f8a;
    r->mainVoice = "CHAPTER_3_E_MAIN";
    r->text1 = 0x3f90;
    r->text1Voice = "CHAPTER_3_E_REMIND";
    r->text2 = 0x3f8f;
    r->text2Voice = "Chapter_3_E_Step_Aux";
    r->statusText = 0x3f91;
    r->voice = "CHAPTER_3_E_DONE";
    r->text1Time = 0x58c;
    r->tail2Time = 0xd5;
    r->hook = 0;
    r->stepHook = 0;
    r->query = ai_QueryChapter3Step6;
    r->goal = 0x23;
    r->tailTime = 0x58c;
    r->goalState = 9;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1f70;
    r->text2Goal = 0x24;
    r->stepCount = 3;
    r->stepShown = 0;
    r->steps = (TUTSTEP *)mem_Alloc(3 * sizeof(TUTSTEP), "FTInitScriptChapter3Step6");
    if (!r->steps)
        goto fail;
    r->steps[0].delay = 0x2c6;
    r->steps[0].kind = 1;
    r->steps[0].text = 0x3f8c;
    r->steps[0].voice = "CHAPTER_3_E_STEP_RIGHT";
    r->steps[1].delay = 0x2c6;
    r->steps[1].kind = 0;
    r->steps[1].text = 0x3f8d;
    r->steps[1].voice = "CHAPTER_3_E_STEP_LEFT";
    r->steps[2].delay = 0x2c6;
    r->steps[2].kind = 0;
    r->steps[2].text = 0x3f8e;
    r->steps[2].voice = "CHAPTER_3_E_STEP_TOP";
    r->markCount = 2;
    r->markPick = ai_MarkerChapter3Step6;
    r->marks = (TUTMARK *)mem_Alloc(2 * sizeof(TUTMARK), "FTInitAnimChapter3Step6");
    if (!r->marks)
        goto fail;
    if (d2_ScreenW == 0x320) {
        r->marks[0].from[0] = 136.0f;
        r->marks[0].from[1] = 430.0f;
        r->marks[0].ctrl1[0] = 336.0f;
        r->marks[0].ctrl1[1] = 450.0f;
        r->marks[0].ctrl2[0] = 466.0f;
        r->marks[0].ctrl2[1] = 270.0f;
        r->marks[0].to[0] = 465.0f;
        r->marks[0].to[1] = 195.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[0].from[0] = 250.0f;
        r->marks[0].from[1] = 515.0f;
        r->marks[0].ctrl1[0] = 466.0f;
        r->marks[0].ctrl1[1] = 540.0f;
        r->marks[0].ctrl2[0] = 566.0f;
        r->marks[0].ctrl2[1] = 380.0f;
        r->marks[0].to[0] = 575.0f;
        r->marks[0].to[1] = 275.0f;
    } else {
        r->marks[0].from[0] = 306.0f;
        r->marks[0].from[1] = 580.0f;
        r->marks[0].ctrl1[0] = 456.0f;
        r->marks[0].ctrl1[1] = 600.0f;
        r->marks[0].ctrl2[0] = 616.0f;
        r->marks[0].ctrl2[1] = 520.0f;
        r->marks[0].to[0] = 626.0f;
        r->marks[0].to[1] = 350.0f;
    }
    r->marks[0].wait = 35.0f;
    r->marks[0].travel = 142.0f;
    r->marks[0].rest = 71.0f;
    if (d2_ScreenW == 0x320) {
        r->marks[1].from[0] = 465.0f;
        r->marks[1].from[1] = 195.0f;
        r->marks[1].ctrl1[0] = 286.0f;
        r->marks[1].ctrl1[1] = 290.0f;
        r->marks[1].ctrl2[0] = 406.0f;
        r->marks[1].ctrl2[1] = 530.0f;
        r->marks[1].to[0] = 115.0f;
        r->marks[1].to[1] = 510.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[1].from[0] = 575.0f;
        r->marks[1].from[1] = 275.0f;
        r->marks[1].ctrl1[0] = 436.0f;
        r->marks[1].ctrl1[1] = 340.0f;
        r->marks[1].ctrl2[0] = 536.0f;
        r->marks[1].ctrl2[1] = 620.0f;
        r->marks[1].to[0] = 226.0f;
        r->marks[1].to[1] = 594.0f;
    } else {
        r->marks[1].from[0] = 626.0f;
        r->marks[1].from[1] = 350.0f;
        r->marks[1].ctrl1[0] = 476.0f;
        r->marks[1].ctrl1[1] = 390.0f;
        r->marks[1].ctrl2[0] = 596.0f;
        r->marks[1].ctrl2[1] = 660.0f;
        r->marks[1].to[0] = 286.0f;
        r->marks[1].to[1] = 650.0f;
    }
    r->marks[1].wait = 35.0f;
    r->marks[1].travel = 142.0f;
    r->marks[1].rest = 71.0f;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Step7");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'c3s7';
    r->mode = 1;
    r->mainText = 0x3f92;
    r->mainVoice = "CHAPTER_3_F_MAIN";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = 0x58c;
    r->tail2Time = 0x47;
    r->query = ai_QueryDefault;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 3;
    r->tailTime = 0x1aa;
    r->goalState = 0xb;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Step8");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'c3s8';
    r->mode = 2;
    r->mainText = 0x3f94;
    r->mainVoice = "CHAPTER_3_G_MAIN";
    r->text1 = 0x3f97;
    r->text1Voice = "CHAPTER_3_G_REMIND";
    r->text2 = 0x3f96;
    r->text2Voice = "Chapter_3_G_Step_Aux";
    r->statusText = 0x3f98;
    r->voice = "CHAPTER_3_G_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0xd5;
    r->hook = 0;
    r->stepHook = 0;
    r->query = ai_QueryChapter3Step8;
    r->goal = 0x17;
    r->tailTime = 0x58c;
    r->goalState = 9;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1f70;
    r->text2Goal = 0x24;
    r->stepCount = 0;
    r->stepShown = 0;
    r->markCount = 5;
    r->markPick = ai_MarkerChapter3Step8;
    r->marks = (TUTMARK *)mem_Alloc(5 * sizeof(TUTMARK), "FTInitAnimChapter3Step8");
    if (!r->marks)
        goto fail;
    if (d2_ScreenW == 0x320) {
        r->marks[0].from[0] = 465.0f;
        r->marks[0].from[1] = 195.0f;
        r->marks[0].ctrl1[0] = 536.0f;
        r->marks[0].ctrl1[1] = 440.0f;
        r->marks[0].ctrl2[0] = 356.0f;
        r->marks[0].ctrl2[1] = 420.0f;
        r->marks[0].to[0] = 140.0f;
        r->marks[0].to[1] = 170.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[0].from[0] = 575.0f;
        r->marks[0].from[1] = 275.0f;
        r->marks[0].ctrl1[0] = 486.0f;
        r->marks[0].ctrl1[1] = 390.0f;
        r->marks[0].ctrl2[0] = 496.0f;
        r->marks[0].ctrl2[1] = 560.0f;
        r->marks[0].to[0] = 245.0f;
        r->marks[0].to[1] = 255.0f;
    } else {
        r->marks[0].from[0] = 626.0f;
        r->marks[0].from[1] = 350.0f;
        r->marks[0].ctrl1[0] = 406.0f;
        r->marks[0].ctrl1[1] = 290.0f;
        r->marks[0].ctrl2[0] = 176.0f;
        r->marks[0].ctrl2[1] = 290.0f;
        r->marks[0].to[0] = 126.0f;
        r->marks[0].to[1] = 180.0f;
    }
    r->marks[0].wait = 35.0f;
    r->marks[0].travel = 142.0f;
    r->marks[0].rest = 71.0f;
    if (d2_ScreenW == 0x320) {
        r->marks[1].from[0] = 75.0f;
        r->marks[1].from[1] = 170.0f;
        r->marks[1].ctrl1[0] = 206.0f;
        r->marks[1].ctrl1[1] = 300.0f;
        r->marks[1].ctrl2[0] = 426.0f;
        r->marks[1].ctrl2[1] = 260.0f;
        r->marks[1].to[0] = 465.0f;
        r->marks[1].to[1] = 195.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[1].from[0] = 180.0f;
        r->marks[1].from[1] = 255.0f;
        r->marks[1].ctrl1[0] = 226.0f;
        r->marks[1].ctrl1[1] = 390.0f;
        r->marks[1].ctrl2[0] = 566.0f;
        r->marks[1].ctrl2[1] = 380.0f;
        r->marks[1].to[0] = 575.0f;
        r->marks[1].to[1] = 275.0f;
    } else {
        r->marks[1].from[0] = 226.0f;
        r->marks[1].from[1] = 320.0f;
        r->marks[1].ctrl1[0] = 276.0f;
        r->marks[1].ctrl1[1] = 410.0f;
        r->marks[1].ctrl2[0] = 626.0f;
        r->marks[1].ctrl2[1] = 450.0f;
        r->marks[1].to[0] = 626.0f;
        r->marks[1].to[1] = 340.0f;
    }
    r->marks[1].wait = 35.0f;
    r->marks[1].travel = 142.0f;
    r->marks[1].rest = 71.0f;
    if (d2_ScreenW == 0x320) {
        r->marks[2].from[0] = 465.0f;
        r->marks[2].from[1] = 195.0f;
        r->marks[2].ctrl1[0] = 286.0f;
        r->marks[2].ctrl1[1] = 290.0f;
        r->marks[2].ctrl2[0] = 406.0f;
        r->marks[2].ctrl2[1] = 530.0f;
        r->marks[2].to[0] = 115.0f;
        r->marks[2].to[1] = 510.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[2].from[0] = 575.0f;
        r->marks[2].from[1] = 275.0f;
        r->marks[2].ctrl1[0] = 436.0f;
        r->marks[2].ctrl1[1] = 340.0f;
        r->marks[2].ctrl2[0] = 536.0f;
        r->marks[2].ctrl2[1] = 620.0f;
        r->marks[2].to[0] = 226.0f;
        r->marks[2].to[1] = 594.0f;
    } else {
        r->marks[2].from[0] = 626.0f;
        r->marks[2].from[1] = 350.0f;
        r->marks[2].ctrl1[0] = 476.0f;
        r->marks[2].ctrl1[1] = 390.0f;
        r->marks[2].ctrl2[0] = 596.0f;
        r->marks[2].ctrl2[1] = 660.0f;
        r->marks[2].to[0] = 286.0f;
        r->marks[2].to[1] = 650.0f;
    }
    r->marks[2].wait = 35.0f;
    r->marks[2].travel = 142.0f;
    r->marks[2].rest = 71.0f;
    if (d2_ScreenW == 0x320) {
        r->marks[3].from[0] = 700.0f;
        r->marks[3].from[1] = 310.0f;
        r->marks[3].ctrl1[0] = 700.0f;
        r->marks[3].ctrl1[1] = 310.0f;
        r->marks[3].ctrl2[0] = 700.0f;
        r->marks[3].ctrl2[1] = 310.0f;
        r->marks[3].to[0] = 700.0f;
        r->marks[3].to[1] = 310.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[3].from[0] = 924.0f;
        r->marks[3].from[1] = 310.0f;
        r->marks[3].ctrl1[0] = 924.0f;
        r->marks[3].ctrl1[1] = 310.0f;
        r->marks[3].ctrl2[0] = 924.0f;
        r->marks[3].ctrl2[1] = 310.0f;
        r->marks[3].to[0] = 924.0f;
        r->marks[3].to[1] = 310.0f;
    } else {
        r->marks[3].from[0] = 560.0f;
        r->marks[3].from[1] = 420.0f;
        r->marks[3].ctrl1[0] = 710.0f;
        r->marks[3].ctrl1[1] = 370.0f;
        r->marks[3].ctrl2[0] = 860.0f;
        r->marks[3].ctrl2[1] = 320.0f;
        r->marks[3].to[0] = 1020.0f;
        r->marks[3].to[1] = 310.0f;
    }
    r->marks[3].wait = 0.0f;
    r->marks[3].travel = 7.0f;
    r->marks[3].rest = 21.0f;
    if (d2_ScreenW == 0x320) {
        r->marks[4].from[0] = 700.0f;
        r->marks[4].from[1] = 280.0f;
        r->marks[4].ctrl1[0] = 700.0f;
        r->marks[4].ctrl1[1] = 280.0f;
        r->marks[4].ctrl2[0] = 700.0f;
        r->marks[4].ctrl2[1] = 280.0f;
        r->marks[4].to[0] = 700.0f;
        r->marks[4].to[1] = 280.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[4].from[0] = 924.0f;
        r->marks[4].from[1] = 280.0f;
        r->marks[4].ctrl1[0] = 924.0f;
        r->marks[4].ctrl1[1] = 280.0f;
        r->marks[4].ctrl2[0] = 924.0f;
        r->marks[4].ctrl2[1] = 280.0f;
        r->marks[4].to[0] = 924.0f;
        r->marks[4].to[1] = 280.0f;
    } else {
        r->marks[4].from[0] = 560.0f;
        r->marks[4].from[1] = 420.0f;
        r->marks[4].ctrl1[0] = 710.0f;
        r->marks[4].ctrl1[1] = 370.0f;
        r->marks[4].ctrl2[0] = 860.0f;
        r->marks[4].ctrl2[1] = 320.0f;
        r->marks[4].to[0] = 1020.0f;
        r->marks[4].to[1] = 280.0f;
    }
    r->marks[4].wait = 0.0f;
    r->marks[4].travel = 7.0f;
    r->marks[4].rest = 21.0f;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Step9");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'c3s9';
    r->mode = 1;
    r->mainText = 0x3f99;
    r->mainVoice = "CHAPTER_3_H_MAIN";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->query = ai_TutorialQueryDefault;
    r->text1Time = 0x429;
    r->tail2Time = 0x47;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 3;
    r->tailTime = 0x163;
    r->goalState = 0xb;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Step10");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'c3sA';
    r->mode = 2;
    r->mainText = 0x3f9b;
    r->mainVoice = "CHAPTER_3_I_MAIN";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0x3f9d;
    r->text2Voice = "Chapter_3_I_Step_Aux";
    r->statusText = 0x3f9f;
    r->voice = "CHAPTER_3_I_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0xd5;
    r->hook = 0;
    r->stepHook = 0;
    r->query = ai_QueryChapter3Step10;
    r->goal = 0x12;
    r->tailTime = 0x58c;
    r->goalState = 9;
    r->allowMask = 0x5c37;
    r->allowMask2 = 0x1e70;
    r->text2Goal = 0x26;
    r->stepCount = 0;
    r->stepShown = 0;
    r->markCount = 1;
    r->marks = (TUTMARK *)mem_Alloc(1 * sizeof(TUTMARK), "FTInitAnimChapter3Step8");
    if (!r->marks)
        goto fail;
    if (d2_ScreenW == 0x320) {
        r->marks[0].from[0] = 465.0f;
        r->marks[0].from[1] = 195.0f;
        r->marks[0].ctrl1[0] = 456.0f;
        r->marks[0].ctrl1[1] = 300.0f;
        r->marks[0].ctrl2[0] = 216.0f;
        r->marks[0].ctrl2[1] = 280.0f;
        r->marks[0].to[0] = 166.0f;
        r->marks[0].to[1] = 170.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[0].from[0] = 575.0f;
        r->marks[0].from[1] = 275.0f;
        r->marks[0].ctrl1[0] = 566.0f;
        r->marks[0].ctrl1[1] = 370.0f;
        r->marks[0].ctrl2[0] = 346.0f;
        r->marks[0].ctrl2[1] = 410.0f;
        r->marks[0].to[0] = 280.0f;
        r->marks[0].to[1] = 255.0f;
    } else {
        r->marks[0].from[0] = 626.0f;
        r->marks[0].from[1] = 350.0f;
        r->marks[0].ctrl1[0] = 636.0f;
        r->marks[0].ctrl1[1] = 430.0f;
        r->marks[0].ctrl2[0] = 376.0f;
        r->marks[0].ctrl2[1] = 440.0f;
        r->marks[0].to[0] = 336.0f;
        r->marks[0].to[1] = 320.0f;
    }
    r->marks[0].wait = 35.0f;
    r->marks[0].travel = 170.0f;
    r->marks[0].rest = 106.0f;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter3Outro");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'utr3';
    r->mode = 3;
    r->mainText = 0x3fa0;
    r->mainVoice = "CHAPTER_3_OUTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x163;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    ai_TutorialBanks[3] = snd_LoadVoiceBank("TUTORIAL_CHAPTER_3.sbf", 0);
    return (int)head;
fail:
    ai_TutorialFreeRuns(head);
    return 0;
}
/* 0x57aa80, builds the run chain of the tutorial's chapter 4 (ai_TutorialChapterInit[3]): five run records (the
   intro 'ntr4', the steps 'c4s4', 'c4s2', 'c4s5' with the hook ai_HookChapter4Step and their step callbacks,
   the outro 'utr4'), each allocated under its "FTInitChapter4*" tag (shifted by one: the intro's successor is
   allocated as "FTInitChapter4Step4" and so on), zeroed and filled with its texts / voices ("CHAPTER_4_*"),
   times, callbacks, goal and allow masks, and linked through next. The ids are copy-pasted (the intro 3, the
   steps 1, the outro 4). The voice bank "TUTORIAL_CHAPTER_4.sbf" is loaded into ai_TutorialBanks[4]. The first
   record, or 0 after a failed allocation (the records built so far freed) */

int ai_TutorialInitChapter4(void)
{
    TUTRUN *head;
    TUTRUN *r;

    head = 0;
    r = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter4Intro");
    if (!r)
        goto fail;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 3;
    r->tag = 'ntr4';
    r->mode = 0;
    r->mainText = 0x3fa2;
    r->mainVoice = "CHAPTER_4_INTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x8e;
    r->hook = ai_HookChapter3Intro;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x5837;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    head = r;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter4Step4");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c4s4';
    r->mode = 2;
    r->mainText = 0x3fa4;
    r->mainVoice = "CHAPTER_4_D_MAIN";
    r->text1 = 0x3fa7;
    r->text1Voice = "CHAPTER_4_D_REMIND";
    r->text2 = 0x3fa6;
    r->text2Voice = "Chapter_4_D_Aux";
    r->statusText = 0x3fa8;
    r->voice = "CHAPTER_4_D_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->hook = ai_HookChapter4Step;
    r->query = ai_QueryChapter4Step4;
    r->stepHook = ai_StepChapter4Step4;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x5a37;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter4Step2");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c4s2';
    r->mode = 2;
    r->mainText = 0x3fa9;
    r->mainVoice = "CHAPTER_4_B_MAIN";
    r->text1 = 0x3fac;
    r->text1Voice = "CHAPTER_4_B_REMIND";
    r->text2 = 0x3fab;
    r->text2Voice = "Chapter_4_B_Aux";
    r->statusText = 0x3fad;
    r->voice = "CHAPTER_4_B_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->hook = ai_HookChapter4Step;
    r->stepHook = ai_StepChapter4Step2;
    r->query = ai_QueryChapter4Step2;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x5b77;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter4Step5");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 1;
    r->tag = 'c4s5';
    r->mode = 2;
    r->mainText = 0x3fae;
    r->mainVoice = "CHAPTER_4_E_MAIN";
    r->text1 = 0x3fb1;
    r->text1Voice = "CHAPTER_4_E_REMIND";
    r->text2 = 0x3fb0;
    r->text2Voice = "Chapter_4_E_Aux";
    r->statusText = 0x3fb2;
    r->voice = "CHAPTER_4_E_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->hook = ai_HookChapter4Step;
    r->query = ai_QueryChapter4Step5;
    r->stepHook = ai_StepChapter4Step5;
    r->goal = 1;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x5f77;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter4Outro");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 4;
    r->tag = 'utr4';
    r->mode = 3;
    r->mainText = 0x3fb3;
    r->mainVoice = "CHAPTER_4_OUTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x163;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x1837;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    ai_TutorialBanks[4] = snd_LoadVoiceBank("TUTORIAL_CHAPTER_4.sbf", 0);
    return (int)head;
fail:
    ai_TutorialFreeRuns(head);
    return 0;
}
/* 0x57aeb0, builds the run chain of the tutorial's chapter 5 (ai_TutorialChapterInit[4]): eight run records (the
   intro 'ntr5', the steps 'c5s1', 'c5s3', 'c5s2' twice (the second with the C1 texts), 'c5s4', 'c5s5' and the
   closing 'c5s6' with the id 6), each allocated under its "FTInitChapter5*" tag (shifted by one: a record's
   successor is allocated under the name of the next step, some names twice), zeroed and filled with its texts /
   voices ("CHAPTER_5_*"), times, callbacks, goal and allow masks, and linked through next. The first 'c5s2'
   gets the position marker's two paths ("FTInitAnimChapter5Step2"), whose points depend on the screen width
   (800, 1024, else). The voice bank "TUTORIAL_CHAPTER_5.sbf" is loaded into ai_TutorialBanks[5]. The first
   record, or 0 after a failed allocation (the records built so far freed) */

int ai_TutorialInitChapter5(void)
{
    TUTRUN *head;
    TUTRUN *r;

    head = 0;
    r = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter5Intro");
    if (!r)
        goto fail;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 5;
    r->tag = 'ntr5';
    r->mode = 0;
    r->mainText = 0x3fb5;
    r->mainVoice = "CHAPTER_5_INTRO";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = -1;
    r->tail2Time = 0x8e;
    r->hook = ai_HookChapter5Intro;
    r->stepHook = 0;
    r->goal = 0;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x5f77;
    r->allowMask2 = 0x1f70;
    r->next = 0;
    head = r;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter5Step1");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 5;
    r->tag = 'c5s1';
    r->mode = 1;
    r->mainText = 0x3fb7;
    r->mainVoice = "CHAPTER_5_B_MAIN";
    r->text1 = 0x3fb9;
    r->text1Voice = "CHAPTER_5_B_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3fba;
    r->voice = "CHAPTER_5_B_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x163;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0x2c;
    r->tailTime = 0x1aa;
    r->goalState = 0xb;
    r->allowMask = 0x1877;
    r->allowMask2 = 0x1f78;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter5Step3");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 5;
    r->tag = 'c5s3';
    r->mode = 1;
    r->mainText = 0x3fc5;
    r->mainVoice = "CHAPTER_5_D_MAIN";
    r->text1 = 0x3fc7;
    r->text1Voice = "CHAPTER_5_D_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3fc8;
    r->voice = "CHAPTER_5_D_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->hook = 0;
    r->stepHook = 0;
    r->goal = 0xf;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x18f7;
    r->allowMask2 = 0x1f78;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter5Step2");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 5;
    r->tag = 'c5s2';
    r->mode = 2;
    r->mainText = 0x3fbb;
    r->mainVoice = "CHAPTER_5_C_MAIN";
    r->text1 = 0x3fbe;
    r->text1Voice = "CHAPTER_5_C_REMIND";
    r->text2 = 0x3fbd;
    r->text2Voice = "Chapter_5_C_Step_Aux";
    r->statusText = 0x3fbf;
    r->voice = "CHAPTER_5_C_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0xd5;
    r->stepHook = 0;
    r->hook = ai_HookChapter5Step2;
    r->query = ai_QueryChapter5Step2;
    r->goal = 0x17;
    r->tailTime = 0x58c;
    r->goalState = 9;
    r->allowMask = 0x1877;
    r->text2Goal = 0x29;
    r->stepCount = 0;
    r->stepShown = 0;
    r->markCount = 2;
    r->markPick = ai_MarkerChapter5Step2;
    r->marks = (TUTMARK *)mem_Alloc(2 * sizeof(TUTMARK), "FTInitAnimChapter5Step2");
    if (!r->marks)
        goto fail;
    if (d2_ScreenW == 0x320) {
        r->marks[0].from[0] = 140.0f;
        r->marks[0].from[1] = 170.0f;
        r->marks[0].ctrl1[0] = 250.0f;
        r->marks[0].ctrl1[1] = 290.0f;
        r->marks[0].ctrl2[0] = 450.0f;
        r->marks[0].ctrl2[1] = 290.0f;
        r->marks[0].to[0] = 600.0f;
        r->marks[0].to[1] = 305.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[0].from[0] = 250.0f;
        r->marks[0].from[1] = 255.0f;
        r->marks[0].ctrl1[0] = 400.0f;
        r->marks[0].ctrl1[1] = 400.0f;
        r->marks[0].ctrl2[0] = 600.0f;
        r->marks[0].ctrl2[1] = 450.0f;
        r->marks[0].to[0] = 710.0f;
        r->marks[0].to[1] = 390.0f;
    } else {
        r->marks[0].from[0] = 626.0f;
        r->marks[0].from[1] = 350.0f;
        r->marks[0].ctrl1[0] = 636.0f;
        r->marks[0].ctrl1[1] = 430.0f;
        r->marks[0].ctrl2[0] = 376.0f;
        r->marks[0].ctrl2[1] = 440.0f;
        r->marks[0].to[0] = 336.0f;
        r->marks[0].to[1] = 320.0f;
    }
    r->marks[0].wait = 35.0f;
    r->marks[0].travel = 177.0f;
    r->marks[0].rest = 106.0f;
    if (d2_ScreenW == 0x320) {
        r->marks[1].from[0] = 700.0f;
        r->marks[1].from[1] = 310.0f;
        r->marks[1].ctrl1[0] = 700.0f;
        r->marks[1].ctrl1[1] = 310.0f;
        r->marks[1].ctrl2[0] = 700.0f;
        r->marks[1].ctrl2[1] = 310.0f;
        r->marks[1].to[0] = 700.0f;
        r->marks[1].to[1] = 310.0f;
    } else if (d2_ScreenW == 0x400) {
        r->marks[1].from[0] = 924.0f;
        r->marks[1].from[1] = 310.0f;
        r->marks[1].ctrl1[0] = 924.0f;
        r->marks[1].ctrl1[1] = 310.0f;
        r->marks[1].ctrl2[0] = 924.0f;
        r->marks[1].ctrl2[1] = 310.0f;
        r->marks[1].to[0] = 924.0f;
        r->marks[1].to[1] = 310.0f;
    } else {
        r->marks[1].from[0] = 560.0f;
        r->marks[1].from[1] = 420.0f;
        r->marks[1].ctrl1[0] = 710.0f;
        r->marks[1].ctrl1[1] = 370.0f;
        r->marks[1].ctrl2[0] = 860.0f;
        r->marks[1].ctrl2[1] = 320.0f;
        r->marks[1].to[0] = 1020.0f;
        r->marks[1].to[1] = 310.0f;
    }
    r->marks[1].wait = 0.0f;
    r->marks[1].travel = 7.0f;
    r->marks[1].rest = 21.0f;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter5Step2");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 5;
    r->tag = 'c5s2';
    r->mode = 2;
    r->mainText = 0x3fc0;
    r->mainVoice = "CHAPTER_5_C1_MAIN";
    r->text1 = 0x3fc3;
    r->text1Voice = "CHAPTER_5_C1_REMIND";
    r->text2 = 0x3fc2;
    r->text2Voice = "Chapter_5_C1_Step_Aux";
    r->statusText = 0x3fc4;
    r->voice = "CHAPTER_5_C1_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0xd5;
    r->stepHook = 0;
    r->goal = 0x14;
    r->tailTime = 0x2148;
    r->goalState = 9;
    r->allowMask = 0x1877;
    r->text2Goal = 0x2d;
    r->stepCount = 0;
    r->stepShown = 0;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter5Step4");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 5;
    r->tag = 'c5s4';
    r->mode = 1;
    r->mainText = 0x3fc9;
    r->mainVoice = "CHAPTER_5_E_MAIN";
    r->text1 = 0x3fcb;
    r->text1Voice = "CHAPTER_5_E_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3fcc;
    r->voice = "CHAPTER_5_E_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0x163;
    r->hook = ai_HookChapter5Step4;
    r->stepHook = 0;
    r->goal = 0x2e;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x18f7;
    r->allowMask2 = 0x3f78;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter5Step5");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 5;
    r->tag = 'c5s5';
    r->mode = 1;
    r->mainText = 0x3fcd;
    r->mainVoice = "CHAPTER_5_F_MAIN";
    r->text1 = 0x3fcf;
    r->text1Voice = "CHAPTER_5_F_REMIND";
    r->text2 = 0;
    r->text2Voice = 0;
    r->statusText = 0x3fd0;
    r->voice = "CHAPTER_5_F_DONE";
    r->text1Time = 0x429;
    r->tail2Time = 0xd5;
    r->hook = ai_HookChapter5Step5;
    r->stepHook = 0;
    r->goal = 0x2f;
    r->tailTime = 0x2148;
    r->goalState = 0xa;
    r->allowMask = 0x18f7;
    r->allowMask2 = 0xbf78;
    r->next = 0;
    r->next = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitChapter5Step5");
    if (!r->next)
        goto fail;
    r = r->next;
    AI_ZERO_AT(r, sizeof(TUTRUN));
    r->id = 6;
    r->tag = 'c5s6';
    r->mode = 1;
    r->mainText = 0x3fd1;
    r->mainVoice = "CHAPTER_5_G_MAIN";
    r->text1 = 0;
    r->text1Voice = 0;
    r->text2 = 0;
    r->text2Voice = 0;
    r->text1Time = 0x429;
    r->tail2Time = 0x8e;
    r->hook = ai_HookChapter5Step6;
    r->stepHook = 0;
    r->goal = 0x30;
    r->tailTime = 0;
    r->goalState = 0xb;
    r->allowMask = 0x18f7;
    r->allowMask2 = 0xbf78;
    r->next = 0;
    ai_TutorialBanks[5] = snd_LoadVoiceBank("TUTORIAL_CHAPTER_5.sbf", 0);
    return (int)head;
fail:
    ai_TutorialFreeRuns(head);
    return 0;
}
/* 0x57b750, allocates the run record of the tutorial's main outro ("FTInitMainOutro"): 0 when that fails (the
   chain freed: nothing); else the zeroed record as the run 7 'OUTR' of the mode 5 with the text 0x3f27 and the
   voice "TUTORIAL_MAIN_OUTRO", no text windows, no tail, the goal state 0xb and every action but the bit 3
   allowed; the record (its voices are in the main intro's bank) */

int ai_TutorialInitMainOutro(void)
{
    TUTRUN *rec;

    rec = (TUTRUN *)mem_Alloc(sizeof(TUTRUN), "FTInitMainOutro");
    if (!rec) {
        ai_TutorialFreeRuns(0);
        return 0;
    }
    AI_ZERO_AT(rec, sizeof(TUTRUN));
    rec->id = 7;
    rec->tag = 'OUTR';
    rec->mode = 5;
    rec->mainText = 0x3f27;
    rec->mainVoice = "TUTORIAL_MAIN_OUTRO";
    rec->text1 = 0;
    rec->text1Voice = 0;
    rec->text2 = 0;
    rec->text2Voice = 0;
    rec->text1Time = -1;
    rec->tail2Time = 0;
    rec->hook = 0;
    rec->stepHook = 0;
    rec->goal = 0;
    rec->tailTime = 0;
    rec->goalState = 0xb;
    rec->allowMask = ~8;
    rec->next = 0;
    return (int)rec;
}
/* 0x57b810, a cubic Bezier of two floats (134 B): u = 1.0f (0x5eb780) - t, out[0] / out[1] =
   u^3 * a + 3 * u * t * (u * b + t * c) + t^3 * d for the components +0 / +4 of the float vectors a, b, c, d;
   `t` is the fifth argument (a float on the stack), `out` the sixth. The term nesting is the original's FP stack
   (u * b + t * c scaled by 3 * u * t) */

void ai_TutorialBezier(float *a, float *b, float *c, float *d, float t, float *out)
{
    float u;

    u = 1.0f - t;
    out[0] = u * u * u * a[0] + 3.0f * u * t * (u * b[0] + t * c[0]) + t * t * t * d[0];
    out[1] = u * u * u * a[1] + 3.0f * u * t * (u * b[1] + t * c[1]) + t * t * t * d[1];
}

struct G5CH; struct G9REC; struct VFSFILE;

/* 0x57b8a0, moves the tutorial's position marker along the run's paths (TUTRUN.marks), once per frame: nothing
   while the tutorial is off or the run has no paths. `mode` is the caller's; the run's markPick (when set)
   answers the path for it: -1 and below hides the marker (mode 2), another valid path than the current one
   restarts the marker on it (phase 2). The shown marker is raised and enabled while its frame depth is the
   current one. Mode 0 places it at the path's start (created as the picture 0x1d when there is none) and waits
   (phase 2); mode 1 runs the phases: 2 waits `wait` ms at the start, 3 moves along the Bezier for `travel` ms,
   4 rests `rest` ms at the end and starts over; mode 2 removes it (phase 1) */

void ai_TutorialMarkerUpdate(int mode)
{
    int pick;
    TUTMARK *m;
    float t;
    float pos[2];

    if (!ai_TutorialOn)
        return;
    if (!ai_Tutorial->run->marks)
        return;
    if (ai_Tutorial->run->markCount < 1)
        return;
    if (ai_Tutorial->run->markPick) {
        pick = ai_Tutorial->run->markPick(mode);
        if (pick <= -1) {
            mode = 2;
        } else if (pick >= 0 && pick < ai_Tutorial->run->markCount && pick != ai_Tutorial->markerStep) {
            ai_Tutorial->markerPhase = 2;
            ai_Tutorial->markerAt = d3_Time;
            ai_Tutorial->markerStep = pick;
        }
    }
    if (ai_Tutorial->markerObj != -1) {
        d2_RaiseObject(ai_Tutorial->markerObj);
        if (ai_Tutorial->markerDepth != sv_FrameDepth)
            d2_EnableObject(ai_Tutorial->markerObj, 0);
        else
            d2_EnableObject(ai_Tutorial->markerObj, 1);
    }
    if (mode == 0) {
        ai_Tutorial->markerDepth = sv_FrameDepth;
        ai_Tutorial->markerAt = d3_Time;
        ai_Tutorial->markerPhase = 2;
        if (ai_Tutorial->markerObj == -1) {
            m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
            ai_Tutorial->markerObj = d2_CreatePicObj((int)m->from[0], (int)m->from[1], 0x1d);
        }
        m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
        d2_MoveObj((int)m->from[0], (int)m->from[1], ai_Tutorial->markerObj);
    } else if (mode == 1) {
        if (ai_Tutorial->markerPhase == 2) {
            if ((float)ai_Tutorial->markerAt + ai_Tutorial->run->marks[ai_Tutorial->markerStep].wait > (float)(unsigned)d3_Time)
                return;
            ai_Tutorial->markerPhase = 3;
            ai_Tutorial->markerAt = d3_Time;
        }
        if (ai_Tutorial->markerPhase == 3) {
            m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
            if ((float)ai_Tutorial->markerAt + m->travel > (float)(unsigned)d3_Time) {
                t = (float)(d3_Time - ai_Tutorial->markerAt) / m->travel;
                ai_TutorialBezier(m->from, m->ctrl1, m->ctrl2, m->to, t, pos);
                d2_MoveObj((int)pos[0], (int)pos[1], ai_Tutorial->markerObj);
                return;
            }
            ai_Tutorial->markerPhase = 4;
            ai_Tutorial->markerAt = d3_Time;
            m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
            d2_MoveObj((int)m->to[0], (int)m->to[1], ai_Tutorial->markerObj);
        }
        if (ai_Tutorial->markerPhase == 4) {
            if ((float)ai_Tutorial->markerAt + ai_Tutorial->run->marks[ai_Tutorial->markerStep].rest > (float)(unsigned)d3_Time)
                return;
            ai_Tutorial->markerPhase = 2;
            ai_Tutorial->markerAt = d3_Time;
            m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
            d2_MoveObj((int)m->from[0], (int)m->from[1], ai_Tutorial->markerObj);
        }
    } else if (mode == 2) {
        if (ai_Tutorial->markerObj != -1)
            d2_RemoveObj(ai_Tutorial->markerObj);
        ai_Tutorial->markerObj = -1;
        ai_Tutorial->markerPhase = 1;
    }
}

/* 0x57bc30, the tutorial character: sets init_PlayerProfile to the tutorial player - the tools 0x4b / 0x18, the
   status 0x10, the job 0x33, the coat 0x59a, sex 0 / faith 0, the model id of "patrizier_MANN2" (gm_HeadIconNumber
   + 0x639), the default first / family name (texts 0x3f22 / 0x3f23) and the talents of the job */
void ai_SetupTutorialPlayer(void)
{
    JOBTALENTS t;

    init_PlayerProfile.tool1 = 0x4b;
    init_PlayerProfile.tool2 = 0x18;
    init_PlayerProfile.status = 0x10;
    init_PlayerProfile.job = 0x33;
    init_PlayerProfile.coat = 0x59a;
    init_PlayerProfile.sex = 0;
    init_PlayerProfile.faith = 0;
    init_PlayerProfile.modelId = gm_HeadIconNumber("patrizier_MANN2") + 0x639;
    strcpy(init_PlayerProfile.firstName, txt_Ptrs[0x3f22]);
    strcpy(init_PlayerProfile.familyName, txt_Ptrs[0x3f23]);
    t = gm_GetJobTalents(init_PlayerProfile.job);
    memcpy(init_PlayerProfile.talents, t.skills, 5);
}

int sv_RunFrame(int cb, unsigned mask);

/* 0x57bce0, the tutorial's chapter chooser: opens the form "tutorial\CHOOSE_tutorial" centred, writes its title
   (text 0x3f1a) into control 1 and into control 2 five checkboxes for the chapters (texts 0x3f1b ..), each
   checked as ai_TutorialChapterOn says, then the buttons start (0x3f20) and back (0x3f21). The frame pump runs
   it (mask 0x86): start is enabled only while a chapter is checked; the right mouse button, the escape key or
   the control 1 close the window; back closes it, start closes it and stores the checkboxes into
   ai_TutorialChapterOn. 1 when start was clicked with a chapter checked (the tutorial starts), else 0
   @deep read whole against the original (167 instructions, ours 165, every call, field and constant
   equal). Left: the scan loop of the frame pump keeps two counters in the original (edx the index i, esi
   the count n: xor edx,edx / inc esi / inc edx / cmp edx,5, then cmp esi,5), ours merges them into esi
   (cmp esi,5 as the loop test). Tried: for (i = 0, n = 0; ...; i++, n++), n unsigned, else { n++; }, n
   declared before i: all merge the same; a VC6 induction-variable decision, not a statement difference */
int ai_TutorialWindow(void)
{
    int start;
    int result;
    int back;
    int ids[5];
    int i;
    int n;
    int id;

    result = 0;
    id = d2_OpenForm(0, 0, "tutorial\\CHOOSE_tutorial");
    d2_CentreViewWindows(id);
    d2_SetForm(id, 1);
    io_PrintText("$Z%s", 0x3f1a);
    d2_SetForm(id, 2);
    for (i = 0; i < 5; i++) {
        ids[i] = d2_GetWinObj(id, 2, io_PrintText("%ia[%s]$N", 0x3f1b + i));
        d2_Objs[ids[i]].checkable = 1;
        d2_Objs[ids[i]].checked = ai_TutorialChapterOn[i];
        d2_Objs[ids[i]].style &= ~2;
    }
    start = d2_GetWinObj(id, 2, io_PrintText("%ia[%s]$N", 0x3f20));
    back = d2_GetWinObj(id, 2, io_PrintText("%ia[%s]$N", 0x3f21));
    d2_SetForm(id, 2);
    while (sv_RunFrame((int)ai_TutorialWindow, 0x86) != 0) {
#ifdef REA_START
        /* REA_START=tutorial (`uv run play|dbg --start tutorial`): the chapter menu is the screen between the
           main-menu entry and the loading screen. Accept its default selection (0x62918c starts all 1) the way
           the "Tutorial starten" click does, so a run without GUI automation still reaches the tutorial;
           without the flag the menu waits for a real click. The frame above has drawn and registered it. */
        {
            char *rea = getenv("REA_START");
            if (rea != 0 && strcmp(rea, "tutorial") == 0) {
                sv_PopRequest = 1;
                result = 1;
            }
        }
#endif
        n = 0;
        for (i = 0; i < 5; i++) {
            if (d2_Objs[ids[i]].checked != 0) {
                d2_SetObjEnabled(start, 1);
                break;
            }
            n++;
        }
        if (n == 5)
            d2_SetObjEnabled(start, 0);
        if (dm_L.rup != 0 || kb_Last == 1 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == back) {
                sv_PopRequest = 1;
            } else if (d2_PointerObj == start) {
                sv_PopRequest = 1;
                n = 0;
                for (i = 0; i < 5; i++) {
                    if (d2_Objs[ids[i]].checked != 0) {
                        n++;
                        ai_TutorialChapterOn[i] = 1;
                    } else {
                        ai_TutorialChapterOn[i] = 0;
                    }
                }
                if (n != 0)
                    result = 1;
            }
        }
    }
    d2_CloseView(id);
    return result;
}

void sv_ButtonRow(int f, int y, int *out, int n, ...); int sv_MessageBoxGreen(); void sv_ValueBar();

/* 0x57bf00, the alchemist's candidates window of the alchemist object obj ("Locations\Domizil\Alchemistry1"):
   the walk steps in use (ahm_WalkStep 0 .. 2) are the candidates; none: the green message box (title text
   0x4039) with the text 0x404a about the own player. Else per candidate a selectable portrait (a radio group;
   the portrait picture 0x639 + one of the two picture words +0x34 / +0x38 of the name record 0x52, alternating)
   with its name (the first and last name texts 0x417a + x / 0x4193 + y), its four attribute bars (labels 0x4172
   .. 0x4175), the texts 0x403f (attr[4]), 0x4040 (its ahm_StepValue) and 0x4041 (the object name of grid id),
   100 pixels apart; the buttons 0x4042 (hire: enabled while a candidate is selected) and 0x4043 (back). Hire
   hands the selected candidate to ai_AlchemistHire and closes the window when that succeeds; back or the right
   mouse button close it. The camera follows again at the end
   @deep read whole against the original and reconstructed (483 vs 489 instructions; asmdiff --symbols:
   every call, argument, constant and text id in the original's order, no ! line but the reordered x / y
   name loads). Left: allocation. The original keeps form in esi, the radio group in ebp then ebx =
   group*0x23, ok in edi; ours form in ebp and -1 in esi (cmp [d2_Radios+8], esi vs cmp ..., -1). In the
   candidate loop the original keeps i and a byte offset i*4 in slots (mov ebp,[esp+0x18] at the loop top,
   [esp+ebp+0x64] / [esp+ebp+0x50], add edx,4 at the end) and computes &d2_Wins[win2].w after the name
   text, ours keeps i in ebx (scaled [esp+ebx*4+..]) and hoists &d2_Wins[win2].w out of the loop; the
   original saves edi only after the no-candidate return (push edi at 0x57bfaf); frame 0x55c vs 0x548 */
void ai_AlchemistCandidates(unsigned char *obj)
{
    int t;
    int i;
    int n;
    unsigned *rec;
    int buttons[2];
    AHMSTEP *step;
    AHMSTEP *steps[3];
    int form;
    int win2;
    int win3;
    int group;
    int portraits[3];
    int values[3];
    char line[0x100];
    char msg[0x400];
    int y;
    int j;

    n = 0;
    steps[0] = 0;
    steps[1] = 0;
    steps[2] = 0;
    rec = gm_NameRecord(0, 0x52);
    for (i = 0; i < 3; i++) {
        step = ahm_WalkStep(i);
        if (step->on != 0) {
            steps[n] = step;
            values[n] = ahm_StepValue((char *)step);
            n++;
        }
    }
    if (n == 0) {
        txt_Format(msg, 0x404a, ai_PlayerSlots[ai_OwnPlayer]);
        sv_MessageBoxGreen((int)txt_Ptrs[0x4039], (int)msg, 0);
        sv_CameraFollow();
        return;
    }
    form = d2_OpenForm(0, 0, "Locations\\Domizil\\Alchemistry1");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 1);
    d2_SetForm(form, 2);
    win2 = d2_LastWindow;
    d2_SetForm(form, 3);
    win3 = d2_LastWindow;
    d2_SetForm(form, 1);
    io_PrintText(0x4039);
    d2_SetForm(form, 2);
    group = d2_CreateRadioGroup(0);
    y = 0x31;
    for (i = 0; i < n; i++) {
        portraits[i] = d2_WinOpenPic(5, y - 0x19, 0x6c7, win2);
        portraits[i] = d2_WinOpenPic(7, y - 0x17, rec[0xd + i % 2] + 0x639, win2);
        d2_Objs[portraits[i]].interactive = 1;
        d2_RadioAdd(group, portraits[i]);
        step = steps[i];
        txt_Format(line, (int)"%s %s", txt_Ptrs[0x417a + step->x], txt_Ptrs[0x4193 + step->y]);
        t = d2_CreateWinText(0, y - 0x30, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w;
        d2_Objs[t].centred = 1;
        d2_SetObjFont(t, 0x54);
        txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
        t = d2_CreateWinText(0x50, y - 0x1a, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w - 0x50;
        d2_SetObjFont(t, 0x54);
        sv_ValueBar(0x8c, y - 0x1a, step->grid.plr.attr[0], 0x4df);
        txt_Format(line, (int)"%s", txt_Ptrs[0x4173]);
        t = d2_CreateWinText(0x50, y - 0xd, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w - 0x50;
        d2_SetObjFont(t, 0x54);
        sv_ValueBar(0x8c, y - 0xd, step->grid.plr.attr[1], 0x4df);
        txt_Format(line, (int)"%s", txt_Ptrs[0x4174]);
        t = d2_CreateWinText(0x50, y, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w - 0x50;
        d2_SetObjFont(t, 0x54);
        sv_ValueBar(0x8c, y, step->grid.plr.attr[2], 0x4df);
        txt_Format(line, (int)"%s", txt_Ptrs[0x4175]);
        t = d2_CreateWinText(0x50, y + 0xd, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w - 0x50;
        d2_SetObjFont(t, 0x54);
        sv_ValueBar(0x8c, y + 0xd, step->grid.plr.attr[3], 0x4df);
        txt_Format(line, 0x403f, step->grid.plr.attr[4]);
        t = d2_CreateWinText(0xfa, y - 0x1b, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w - 0xfa;
        txt_Format(line, 0x4040, ahm_StepValue((char *)step));
        t = d2_CreateWinText(0xfa, y - 0xe, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w - 0xfa;
        txt_Format(line, 0x4041, step->grid.plr.id * 2 + 0x9a8);
        t = d2_CreateWinText(0xfa, y - 1, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w - 0xfa;
        y += 0x64;
    }
    d2_SetForm(form, 3);
    sv_ButtonRow(win3, 0, buttons, 2, txt_Ptrs[0x4042], txt_Ptrs[0x4043]);
    d2_ClickedId = -1;
    do {
        d2_SetObjEnabled(buttons[0], d2_Radios[group].sel != -1 && i != 0 ? 1 : 0);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == buttons[0]) {
                for (j = 0; j < 3; j++)
                    if (d2_Radios[group].sel == j && ai_AlchemistHire(obj, steps[j], values[j]))
                        sv_PopRequest = 1;
            } else if (d2_PointerObj == buttons[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_AlchemistCandidates, 0x677c7));
    d2_RadioClear(group);
    d2_CloseView(form);
    sv_CameraFollow();
}

char * ahm_OwnerChar();
int sv_CheckGoodAmount();
int sv_MessageBox();
void txt_Format(char *out, int id0, ...);
int ahm_SendCreateAlchemist();

/* 0x57c560, hires the alchemist candidate step for the alchemist object obj at the price value: 0 when the
   player cannot pay it (sv_CheckGoodAmount), when the object already has an owner (the message text 0x4048)
   or when the confirmation (text 0x4044 with the candidate's first and last name) is declined. Else the
   creation is posted (ahm_SendCreateAlchemist) and its slot awaited (the frame pump while cm_GetSlotState
   is 0); a slot that did not end in state 1 answers 0. On success the step is cleared, the price moved from
   the player (cm_SendMoveGoods) and 1 answered */
int ai_AlchemistHire(unsigned char *obj, AHMSTEP *step, int value)
{
    char text[0x200];
    char *owner;
    int slot;

    owner = ahm_OwnerChar(obj);
    if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], value, ai_OwnPlayer)) {
        if (owner) {
            sv_MessageBox((int)txt_Ptrs[0x4048], 0x100);
        } else {
            txt_Format(text, 0x4044, txt_Ptrs[0x417a + step->x], txt_Ptrs[0x4193 + step->y]);
            if (sv_MessageBox((int)text, 1)) {
                slot = ahm_SendCreateAlchemist(obj, step);
                while (slot != -1 && !cm_GetSlotState(slot))
                    sv_RunFrame(0, 0x20007);
                if (cm_GetSlotState(slot) == 1) {
                    ahm_ClearStep(step);
                    cm_SendMoveGoods(-1, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, value);
                    return 1;
                }
            }
        }
    }
    return 0;
}

int ahm_ResearchState(); char * ahm_OwnerChar(); AHMGRID * ahm_RandomRecipe();

/* 0x57c690, the alchemist's research window of the alchemist object obj ("Locations\Domizil\Alchemistry2"):
   without an owning character only the text 0x4051 (0x182); else the places of the record at +0x65 as
   selectable pictures (ai_ResearchSlots; rebuilt, keeping the selection, when the research state changed or
   0x32 ms passed), a progress slider and a text for the running research (ahm_ResearchState 1: text 0x404b,
   2: 0x404c with the recipe's name, 3: 0x404d with the share's name), and two buttons (texts 0x4053, 0x4054).
   The first is enabled while a place is selected and no research runs; it hands the selected place to
   ai_AlchemistSendRecipeResearch when ahm_RandomRecipe gives the owner a recipe, else the message 0x41b0. The second is enabled
   while no research runs and calls ai_AlchemistSendResearchStart. The right mouse button closes the window; the camera follows
   again at the end
   @deep read whole against the original, reconstructed (559 vs 563 instructions, asmdiff --symbols has no
   symbol/argument/constant difference; no branch-target class remains after inlining the slot loop twice
   with function-level n/pic/slot). Left (dcp diffclass: code 140, slots 122, frame 34, order 20, regs 8),
   all allocation: (1) the original keeps owner in edi and the record in ebp, form in esi, ours form in
   ebp/record in esi (regs esi<->ebp rotate: asmdiff --regs esi=ebp,ebp=esi turns the push/pop lines
   equal); (2) win2 is spilled by the original to [esp+0x40] right after d2_SetForm(form,2) ('+ mov dword
   ptr [esp + 0x40], ebx' at 0x57c725) and reloaded as mov ebx,[esp+0x64] after each text, ours keeps it in
   ebx; (3) in the slot loop the original holds x in ebp, y in esi, i in edi and the slot/picture pointers
   in [esp+0x14]/[esp+0x10] with the counter in [esp+0x1c], ours x in esi, y in ebp, counter [esp+0x24] and
   a different lea schedule (lea eax,[eax+eax*2]; lea esi,[eax+eax*4] vs lea ebp,[eax+eax*2]; ...; lea
   ebp,[ebp+ebp*4]); tried: the helper as __inline function, the loop written out twice, a picture-id
   local, declaration order; frame 0x264 vs 0x25c */
void ai_AlchemistResearchWindow(unsigned char *obj)
{
    char msg[0x100];
    char line[0x100];
    int pics[6];
    AHMREC *rec;
    char *owner;
    int *pic;
    AHMSLOT *slot;
    int n;
    int x;
    int y;
    int k;
    int pid;
    int t;
    int row[2];
    int start;
    int other;
    int state;
    int lastState;
    unsigned lastClock;
    int sel;
    int a;
    float f;
    int form;
    int win2;
    int win3;
    int group;
    int slider;
    int j;

    owner = ahm_OwnerChar(obj);
    rec = ((AHMOBJ *)obj)->rec;
    lastState = ahm_ResearchState(obj, &a, &f, 0);
    lastClock = d3_Time;
    d2_ClickedId = -1;
    form = d2_OpenForm(0, 0, "Locations\\Domizil\\Alchemistry2");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    win2 = d2_LastWindow;
    d2_SetForm(form, 1);
    io_PrintText(0x404f);
    if (owner == 0) {
        d2_SetForm(form, 2);
        io_PrintText(0x4051, 0x182);
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_AlchemistResearchWindow, 0x677c7));
        d2_CloseView(form);
        return;
    }
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, row, 2, txt_Ptrs[0x4053], txt_Ptrs[0x4054]);
    start = row[0];
    other = row[1];
    d2_SetForm(form, 2);
    io_PrintText("$C");
    group = d2_CreateRadioGroup(0);
    n = 0;
    pic = pics;
    slot = rec->slots;
    for (k = 0; k < 6; k++, slot++) {
        if (slot->id == 0)
            continue;
        x = (n & 1) * 240 + 10;
        y = n / 2 * 80 + 10;
        pid = d2_WinOpenPic(x, y, slot->id + 0xdf, win2);
        *pic = pid;
        d2_Objs[pid].interactive = 1;
        d2_Objs[pid].user = ai_Goods + slot->id * 0x41;
        d2_RadioAdd(group, pid);
        txt_Format(line, (int)"%s", slot->id * 2 + 0x9a8);
        t = d2_CreateWinText(x + 0x3c, y + 5, line, win2);
        d2_Objs[t].w = d2_Wins[win2].w / 2 - 10;
        d2_SetObjFont(t, 0x54);
        sv_ValueBar(x + 0x3c, y + 0x19, (int)(slot->v * 252.0f), 0x4df);
        n++;
        pic++;
    }
    do {
        state = ahm_ResearchState(obj, &a, &f, 0);
        d2_SetForm(form, 4);
        io_PrintText("$C");
        if (state != 0) {
            slider = d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2, 0x12, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
            if (state == 1)
                txt_Format(msg, 0x404b);
            else if (state == 2)
                txt_Format(msg, 0x404c, a * 2 + 0x9a8);
            else if (state == 3)
                txt_Format(msg, 0x404d, a + 0x4172);
            t = d2_CreateWinText(0, 0, msg, d2_LastWindow);
            d2_Objs[t].w = d2_LastWindowPtr->w;
            d2_Objs[t].centred = 1;
            d2_SetObjFont(t, 0x54);
            d2_SetObjValue4(slider, 0, 100, (int)(f * 100.0f));
        }
        d2_SetObjEnabled(start, d2_Radios[group].sel != -1 && n != 0 && state == 0 ? 1 : 0);
        d2_SetObjEnabled(other, state == 0);
        if (state != lastState || d3_Time > lastClock + 0x32) {
            lastClock = d3_Time;
            lastState = state;
            sel = d2_Radios[group].sel;
            d2_RadioClear(group);
            d2_SetForm(form, 2);
            io_PrintText("$C");
            group = d2_CreateRadioGroup(0);
        n = 0;
        pic = pics;
        slot = rec->slots;
        for (k = 0; k < 6; k++, slot++) {
            if (slot->id == 0)
                continue;
            x = (n & 1) * 240 + 10;
            y = n / 2 * 80 + 10;
            pid = d2_WinOpenPic(x, y, slot->id + 0xdf, win2);
            *pic = pid;
            d2_Objs[pid].interactive = 1;
            d2_Objs[pid].user = ai_Goods + slot->id * 0x41;
            d2_RadioAdd(group, pid);
            txt_Format(line, (int)"%s", slot->id * 2 + 0x9a8);
            t = d2_CreateWinText(x + 0x3c, y + 5, line, win2);
            d2_Objs[t].w = d2_Wins[win2].w / 2 - 10;
            d2_SetObjFont(t, 0x54);
            sv_ValueBar(x + 0x3c, y + 0x19, (int)(slot->v * 252.0f), 0x4df);
            n++;
            pic++;
        }
            d2_Radios[group].sel = sel;
            d2_MenuNavigate(group);
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == start) {
                for (j = 0; j < 6; j++) {
                    if (d2_Radios[group].sel == j) {
                        if (ahm_RandomRecipe(owner))
                            ai_AlchemistSendRecipeResearch(obj, rec, j);
                        else
                            sv_MessageBox(txt_Ptrs[0x41b0], 0);
                    }
                }
            } else if (d2_PointerObj == other) {
                ai_AlchemistSendResearchStart(obj, rec);
            }
        }
    } while (sv_RunFrame((int)ai_AlchemistResearchWindow, 0x677c7));
    d2_RadioClear(group);
    d2_CloseView(form);
    sv_CameraFollow();
}
/* 0x57cde0, starts the research of the place slot of the alchemist object obj (record rec) with the message 0x96
   (cm_SendLongMessage; from the building, to the owner character's netId, the game clock, flags 1, arg[0] the
   record's id, arg[1] the place's id, arg[4] a random duration 1000 .. 66535); 0 when a research already runs
   (ahm_ResearchState), else 1 */
int ai_AlchemistSendRecipeResearch(unsigned char *obj, AHMREC *rec, int slot)
{
    AIMSG msg;
    int a;
    float f;

    if (ahm_ResearchState(obj, &a, &f, 0) != 0)
        return 0;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x96;
    msg.flags = 1;
    msg.to = ai_CharsT[((AIBLD *)obj)->owner].netId;
    msg.fromId = ((AIBLD *)obj)->id;
    msg.time = ai_Time;
    msg.arg[1] = rec->slots[slot].id;
    msg.arg[0] = rec->id;
    msg.arg[4] = (util_Random(0x5a0) & 0xffff) + 1000;
    cm_SendLongMessage((char *)&msg);
    return 1;
}
/* 0x57ced0, starts the alchemist object obj's own research (record rec) with the message 0x95 (cm_SendLongMessage;
   the 0x57cde0 shape without a place: arg[0] and arg[3] the record's id, no random duration); 0 when a research
   already runs (ahm_ResearchState), else 1 */
int ai_AlchemistSendResearchStart(unsigned char *obj, AHMREC *rec)
{
    AIMSG msg;
    int a;
    float f;

    if (ahm_ResearchState(obj, &a, &f, 0) != 0)
        return 0;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x95;
    msg.flags = 1;
    msg.to = ai_CharsT[((AIBLD *)obj)->owner].netId;
    msg.fromId = ((AIBLD *)obj)->id;
    msg.time = ai_Time;
    msg.arg[0] = rec->id;
    msg.arg[3] = rec->id;
    cm_SendLongMessage((char *)&msg);
    return 1;
}

/* 0x57cf90, the alchemist's window of the alchemist object obj ("Locations\Domizil\Alchemistry3"): without an
   owning character only the text 0x4051 (0x182); else the character panel (ai_AlchemistPanel; rebuilt when the
   research state changed or 0x32 ms passed, the character still existing: gone, the window closes), the
   research slider and text as in ai_AlchemistResearchWindow, and the buttons: a click on one of the four share
   bars opens the skill window ai_AlchemistTrainShareWindow (obj, record, the bar), the extra button ai_AlchemistFundWindow and the button
   with the text 0x4049 ai_AlchemistRescheduleWindow (whose answer closes the window); the form is disabled meanwhile. The right
   mouse button closes the window; the camera follows again at the end
   @deep reconstructed whole (990 vs 990-ish: 3272 vs 3264 bytes; asmdiff --symbols: every call, argument,
   constant, text id and branch in the original's order; the character panel is written out twice, as the
   original expands it, the -1 fill of the nine ids is a loop (rep stosd), the click loop walks a pointer
   over the bar ids). Left: allocation and scheduling only (hunks: order 46, slots 58, frame 124, code 22 =
   spill stores): the original spills win2 to [esp+0x1c] and the extra button id to [esp+0x48] right after
   they are set (ours keeps them in registers), puts the add esp,0xc of the first io_PrintText before the
   d2_SetForm(form, 2) pair, keeps the owner in ebp and the object in esi (ours swapped), compares cmp
   [ecx], eax in the click loop; frame 0x248 vs 0x240 */
void ai_AlchemistWindow(unsigned char *obj)
{
    char msg[0x100];
    char line[0x100];
    int bars[9];
    AHMREC *rec;
    AICHAR *owner;
    int state;
    int lastState;
    unsigned lastClock;
    int a;
    float f;
    int form;
    int win2;
    int extra;
    int back;
    int slider;
    int t;
    int j;
    int k;
    int *pb;

    owner = (AICHAR *)ahm_OwnerChar(obj);
    rec = ((AHMOBJ *)obj)->rec;
    lastState = ahm_ResearchState(obj, &a, &f, 0);
    lastClock = d3_Time;
    for (k = 0; k < 9; k++)
        bars[k] = -1;
    d2_ClickedId = -1;
    form = d2_OpenForm(0, 0, "Locations\\Domizil\\Alchemistry3");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    win2 = d2_LastWindow;
    d2_SetForm(form, 1);
    io_PrintText(0x4039);
    d2_SetForm(form, 2);
    if (owner == 0) {
        io_PrintText(0x4051, 0x182);
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_AlchemistResearchWindow, 0x677c7));
        d2_CloseView(form);
        return;
    }
    d2_WinOpenPic(d2_LastWindowPtr->w / 2 - 0x18, 0x1e, 0x6c7, win2);
    d2_WinOpenPic(d2_LastWindowPtr->w / 2 - 0x16, 0x20, owner->picture, win2);
    slider = d2_WinOpenSlider((d2_LastWindowPtr->w - 0x32) / 2, 0x55, 0x32, 0, 100, 0x75, 0x42, d2_LastWindow);
    d2_SetObjValue4(slider, 0, 100, (int)(rec->value * 100.0f));
    txt_Format(line, (int)"%s %s", owner->first, owner->family);
    t = d2_CreateWinText(0, 0, line, win2);
    d2_Objs[t].w = d2_Wins[win2].w;
    d2_Objs[t].centred = 1;
    d2_SetObjFont(t, 0x53);
    txt_Format(line, 0x403f, owner->w00a);
    t = d2_CreateWinText(0x12c, 0x64, line, win2);
    d2_SetObjFont(t, 0x53);
    d2_Objs[t].centred = 1;
    d2_Objs[t].w = 0xc8;
    txt_Format(line, 0x4040, ahm_ObjectValue(obj));
    t = d2_CreateWinText(0x12c, 0x82, line, win2);
    d2_SetObjFont(t, 0x53);
    d2_Objs[t].centred = 1;
    d2_Objs[t].w = 0xc8;
    txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
    extra = d2_WinCreateSlider(0x14a, 0xbe, 0xc5, txt_Ptrs[0x42f9], win2);
    d2_SetObjWidth(extra, d2_Wins[win2].w / 3 - 0x14);
    d2_SetObjFont(extra, 0x54);
    txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
    bars[0] = d2_WinCreateSlider(10, 0x64, 0xc5, line, win2);
    d2_SetObjWidth(bars[0], d2_Wins[win2].w / 3 - 0x14);
    d2_SetObjFont(bars[0], 0x54);
    sv_ValueBar(d2_Wins[win2].w / 3 + 10, 0x68, rec->share[0], 0x4df);
    txt_Format(line, (int)"%s", txt_Ptrs[0x4173]);
    bars[1] = d2_WinCreateSlider(10, 0x82, 0xc5, line, win2);
    d2_SetObjWidth(bars[1], d2_Wins[win2].w / 3 - 0x14);
    d2_SetObjFont(bars[1], 0x54);
    sv_ValueBar(d2_Wins[win2].w / 3 + 10, 0x86, rec->share[1], 0x4df);
    txt_Format(line, (int)"%s", txt_Ptrs[0x4174]);
    bars[2] = d2_WinCreateSlider(10, 0xa0, 0xc5, line, win2);
    d2_SetObjWidth(bars[2], d2_Wins[win2].w / 3 - 0x14);
    d2_SetObjFont(bars[2], 0x54);
    sv_ValueBar(d2_Wins[win2].w / 3 + 10, 0xa4, rec->share[2], 0x4df);
    txt_Format(line, (int)"%s", txt_Ptrs[0x4175]);
    bars[3] = d2_WinCreateSlider(10, 0xbe, 0xc5, line, win2);
    d2_SetObjWidth(bars[3], d2_Wins[win2].w / 3 - 0x14);
    d2_SetObjFont(bars[3], 0x54);
    sv_ValueBar(d2_Wins[win2].w / 3 + 10, 0xc2, rec->share[3], 0x4df);
    d2_SetForm(form, 3);
    back = d2_GetWinObj(form, 3, io_PrintText(0x4049));
    do {
        state = ahm_ResearchState(obj, &a, &f, 0);
        d2_SetForm(form, 4);
        io_PrintText("$C");
        if (state != 0) {
            slider = d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2, 0x12, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
            if (state == 1)
                txt_Format(msg, 0x404b);
            else if (state == 2)
                txt_Format(msg, 0x404c, a * 2 + 0x9a8);
            else if (state == 3)
                txt_Format(msg, 0x404d, a + 0x4172);
            t = d2_CreateWinText(0, 0, msg, d2_LastWindow);
            d2_Objs[t].w = d2_LastWindowPtr->w;
            d2_Objs[t].centred = 1;
            d2_SetObjFont(t, 0x54);
            d2_SetObjValue4(slider, 0, 100, (int)(f * 100.0f));
        }
        if (state != lastState || d3_Time > lastClock + 0x32) {
            owner = (AICHAR *)ahm_OwnerChar(obj);
            if (owner == 0) {
                sv_PopRequest = 1;
                sv_RunFrame((int)ai_AlchemistWindow, 0x20007);
                d2_CloseView(form);
                return;
            }
            d2_SetForm(form, 2);
            io_PrintText("$C");
            d2_WinOpenPic(d2_LastWindowPtr->w / 2 - 0x18, 0x1e, 0x6c7, win2);
            d2_WinOpenPic(d2_LastWindowPtr->w / 2 - 0x16, 0x20, owner->picture, win2);
            slider = d2_WinOpenSlider((d2_LastWindowPtr->w - 0x32) / 2, 0x55, 0x32, 0, 100, 0x75, 0x42, d2_LastWindow);
            d2_SetObjValue4(slider, 0, 100, (int)(rec->value * 100.0f));
            txt_Format(line, (int)"%s %s", owner->first, owner->family);
            t = d2_CreateWinText(0, 0, line, win2);
            d2_Objs[t].w = d2_Wins[win2].w;
            d2_Objs[t].centred = 1;
            d2_SetObjFont(t, 0x53);
            txt_Format(line, 0x403f, owner->w00a);
            t = d2_CreateWinText(0x12c, 0x64, line, win2);
            d2_SetObjFont(t, 0x53);
            d2_Objs[t].centred = 1;
            d2_Objs[t].w = 0xc8;
            txt_Format(line, 0x4040, ahm_ObjectValue(obj));
            t = d2_CreateWinText(0x12c, 0x82, line, win2);
            d2_SetObjFont(t, 0x53);
            d2_Objs[t].centred = 1;
            d2_Objs[t].w = 0xc8;
            txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
            extra = d2_WinCreateSlider(0x14a, 0xbe, 0xc5, txt_Ptrs[0x42f9], win2);
            d2_SetObjWidth(extra, d2_Wins[win2].w / 3 - 0x14);
            d2_SetObjFont(extra, 0x54);
            txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
            bars[0] = d2_WinCreateSlider(10, 0x64, 0xc5, line, win2);
            d2_SetObjWidth(bars[0], d2_Wins[win2].w / 3 - 0x14);
            d2_SetObjFont(bars[0], 0x54);
            sv_ValueBar(d2_Wins[win2].w / 3 + 10, 0x68, rec->share[0], 0x4df);
            txt_Format(line, (int)"%s", txt_Ptrs[0x4173]);
            bars[1] = d2_WinCreateSlider(10, 0x82, 0xc5, line, win2);
            d2_SetObjWidth(bars[1], d2_Wins[win2].w / 3 - 0x14);
            d2_SetObjFont(bars[1], 0x54);
            sv_ValueBar(d2_Wins[win2].w / 3 + 10, 0x86, rec->share[1], 0x4df);
            txt_Format(line, (int)"%s", txt_Ptrs[0x4174]);
            bars[2] = d2_WinCreateSlider(10, 0xa0, 0xc5, line, win2);
            d2_SetObjWidth(bars[2], d2_Wins[win2].w / 3 - 0x14);
            d2_SetObjFont(bars[2], 0x54);
            sv_ValueBar(d2_Wins[win2].w / 3 + 10, 0xa4, rec->share[2], 0x4df);
            txt_Format(line, (int)"%s", txt_Ptrs[0x4175]);
            bars[3] = d2_WinCreateSlider(10, 0xbe, 0xc5, line, win2);
            d2_SetObjWidth(bars[3], d2_Wins[win2].w / 3 - 0x14);
            d2_SetObjFont(bars[3], 0x54);
            sv_ValueBar(d2_Wins[win2].w / 3 + 10, 0xc2, rec->share[3], 0x4df);
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            for (j = 0, pb = bars; j < 4; j++, pb++) {
                if (*pb == d2_PointerObj) {
                    d2_SetViewEnabled(form, 0);
                    ai_AlchemistTrainShareWindow(obj, rec, j);
                    d2_SetViewEnabled(form, 1);
                    d2_PointerObj = -1;
                }
            }
            if (d2_PointerObj == extra) {
                d2_SetViewEnabled(form, 0);
                ai_AlchemistFundWindow(obj, rec);
                d2_SetViewEnabled(form, 1);
            } else if (d2_PointerObj == back) {
                d2_SetViewEnabled(form, 0);
                if (ai_AlchemistRescheduleWindow(obj, rec))
                    sv_PopRequest = 1;
                d2_SetViewEnabled(form, 1);
            }
        }
    } while (sv_RunFrame((int)ai_AlchemistWindow, 0x677c7));
    d2_CloseView(form);
    sv_CameraFollow();
}
/* 0x57dc50, the share research window of the alchemist object obj (record rec) for the share idx ("Locations\Domizil\skill"):
   the share's name (text idx + 0x4172) and description (idx + 0x4176) and, while no research runs (ahm_ResearchState)
   and the share is not the fourth, the question 0x4046; the first answer posts the message 0x97 (cm_SendLongMessage:
   from the building, to the owner character, the game clock, arg[0] the record's id, arg[1] the share, arg[4] a
   random duration 1000 .. 66535) and closes the window; the second answer and the right mouse button close it.
   Answers 1 */
int ai_AlchemistTrainShareWindow(unsigned char *obj, AHMREC *rec, int idx)
{

    int win;
    int a;
    float b;
    int t1;
    int t2;
    int ok;

    ok = 1;
    if (ahm_ResearchState(obj, &a, &b, 0) != 0)
        ok = 0;
    if (idx == 3)
        ok = 0;
    win = d2_OpenForm(0, 0, "Locations\\Domizil\\skill");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x4045, idx + 0x4172);
    io_PrintText(idx + 0x4176);
    if (ok)
        io_PrintText(0x4046, 0x182);
    d2_SetForm(win, 2);
    a = io_PrintText(0x4047);
    t1 = d2_GetWinObj(win, 2, a);
    t2 = d2_GetWinObj(win, 2, a + 1);
    d2_SetObjEnabled(t1, ok);
    d2_ClickedId = -1;
    for (;;) {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == t1) {
                AIMSG msg;

                memset(&msg, 0, sizeof(msg));
                msg.type = 0x97;
                msg.flags = 1;
                msg.to = ai_CharsT[((AIBLD *)obj)->owner].netId;
                msg.fromId = ((AIBLD *)obj)->id;
                msg.time = ai_Time;
                msg.arg[0] = rec->id;
                msg.arg[4] = (util_Random(0x5a0) & 0xffff) + 1000;
                msg.arg[1] = idx;
                cm_SendLongMessage((char *)&msg);
                sv_PopRequest = 1;
            } else if (d2_PointerObj == t2) {
                sv_PopRequest = 1;
            }
        }
        if (sv_RunFrame((int)ai_AlchemistTrainShareWindow, 0x677c7) == 0) {
            d2_CloseView(win);
            return 1;
        }
    }
}
/* 0x57de60, the money window of the alchemist object obj ("Locations\\Domizil\\skill"; rec is not read): the
   object's value (ahm_ObjectValue) and its tenth, the share 3 of the owning character (ahm_Share; no owner
   answers 0 at once), the text 0x403a with an input field (d2_GetWinObj) set to the range tenth .. value with
   the tenth preset, each converted to the player's money (gm_ConvertMoney), and the answers 0x403b. The first answer takes the field's amount (gm_PlayerFactor) from
   the player's character when sv_CheckGoodAmount agrees: the value is raised by amount / object value * share
   * 0.75 (ahm_SendAddValue, the float passed by value), the amount is moved from the player (cm_SendMoveGoods)
   and the window closes with the answer 1; the second answer and the right mouse button close it. The window
   closes with 0 when the owner is gone */
int ai_AlchemistFundWindow(unsigned char *obj, AHMREC *rec)
{
    char *owner;
    int win;
    int points;
    int tenth;
    float share;
    int field;
    int ok;
    int cancel;
    int v;
    int done;

    done = 0;
    points = ahm_ObjectValue(obj);
    tenth = points / 10;
    owner = ahm_OwnerChar(obj);
    if (!owner)
        return 0;
    share = ahm_Share(owner, 3);
    win = d2_OpenForm(0, 0, "Locations\\Domizil\\skill");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    v = io_PrintText(0x403a, 0x182, ai_OwnPlayer);
    field = d2_GetWinObj(win, 1, v);
    d2_SetObjValue4(field, gm_ConvertMoney(tenth, ai_OwnPlayer), gm_ConvertMoney(points, ai_OwnPlayer),
                    gm_ConvertMoney(tenth, ai_OwnPlayer));
    d2_SetForm(win, 2);
    v = io_PrintText(0x403b);
    ok = d2_GetWinObj(win, 2, v);
    cancel = d2_GetWinObj(win, 2, v + 1);
    d2_ClickedId = -1;
    do {
        owner = ahm_OwnerChar(obj);
        if (!owner) {
            sv_PopRequest = 1;
            sv_RunFrame((int)ai_AlchemistFundWindow, 0x20007);
            d2_CloseView(win);
            return 0;
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                v = gm_PlayerFactor(d2_GetObjValue(field), ai_OwnPlayer);
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], v, ai_OwnPlayer)) {
                    ahm_SendAddValue(owner, (float)v / (float)points * share * 0.75f);
                    done = 1;
                    sv_PopRequest = 1;
                    cm_SendMoveGoods(((AICHAR *)owner)->netId, ai_CharInts[ai_PlayerId][0], ai_OwnPlayer, v);
                }
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_AlchemistFundWindow, 0x677c7));
    d2_CloseView(win);
    return done;
}
/* 0x57e0e0, the dismissal window of the alchemist object obj ("Locations\\Domizil\\skill"; rec is not read): no
   owning character answers 0 at once; else the question 0x403d and the answers 0x403e. The first answer reschedules
   the object's research (ahm_RescheduleResearch) and deletes the alchemist (ahm_SendDelete) and closes the window
   with the answer 1; the second answer and the right mouse button close it. The window closes with 0 when the
   owner is gone */
int ai_AlchemistRescheduleWindow(unsigned char *obj, AHMREC *rec)
{
    char *owner;
    int win;
    int ok;
    int cancel;
    int v;
    int done;

    done = 0;
    owner = ahm_OwnerChar(obj);
    if (!owner)
        return 0;
    win = d2_OpenForm(0, 0, "Locations\\Domizil\\skill");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x403d, 0x182);
    d2_SetForm(win, 2);
    v = io_PrintText(0x403e);
    ok = d2_GetWinObj(win, 2, v);
    cancel = d2_GetWinObj(win, 2, v + 1);
    d2_ClickedId = -1;
    do {
        owner = ahm_OwnerChar(obj);
        if (!owner) {
            sv_PopRequest = 1;
            sv_RunFrame((int)ai_AlchemistRescheduleWindow, 0x20007);
            d2_CloseView(win);
            return 0;
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ok) {
                done = 1;
                ahm_RescheduleResearch(obj);
                ahm_SendDelete(owner);
                sv_PopRequest = 1;
            } else if (d2_PointerObj == cancel) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_AlchemistRescheduleWindow, 0x677c7));
    d2_CloseView(win);
    return done;
}
/* 0x57e230, the table callback of the cannon scene: stores the shot record src (its index 0 .. 2 in src->id)
   in the table ai_ShotTable at that index and marks the table as changed (ai_ShotDirty); answers 1, else 0 for
   no record or an index out of range */
int ai_ShotTableStore(SHOTRESULT *src)
{
    if (!src || src->id < 0 || src->id >= 3)
        return 0;
    ai_ShotTable[src->id] = *src;
    ai_ShotDirty = 1;
    return 1;
}

#pragma pack(push, 1)
typedef struct CANNONREC {   /* the tutorial's cannon record (ai_CannonScene's argument), read at its byte offsets */
    char pad0;
    int id;                  /* +0x01, the effect id gv_FindEffect takes (GVCANNON.bld) */
    char pad5[0x5c];
    D3OBJECT *obj;           /* +0x61, the cannon object the scene is built around */
} CANNONREC;
#pragma pack(pop)
#define CR ((CANNONREC *)rec)
typedef struct CANNONSCENE { /* 0xe4 bytes, the scene struct of ai_CannonScene (the other cannon functions still address it by byte offset) */
    D3VECTOR pos;            /* +0x00, d3_PointToWorld of the cannon object; pos.y gets the height of its bounds * 0.8 */
    float pad0c;
    D3VECTOR rot;            /* +0x10, rot.x the pitch, rot.y the yaw (the angles are GVCANNON.camTarget at the start) */
    float pad1c;
    float zoom;              /* +0x20, cleared here, moved by ai_CannonAim */
    union {
        struct {
            float dirX;      /* +0x24, the aim direction (ai_CannonCamera): the barrel's z axis in the world (ai_CannonAxes) */
            float dirY;      /* +0x28 */
            float dirZ;      /* +0x2c */
        };
        D3VECTOR dir;        /* +0x24, the three as one vector */
    };
    float pad30;
    D3VECTOR side;           /* +0x34, the barrel's x axis in the world (ai_CannonAxes): the shots of a salvo are set apart along it */
    float pad40;
    D3VECTOR up;             /* +0x44, the barrel's y axis in the world (ai_CannonAxes) */
    float pad50;
    float yawSpeed;          /* +0x54, the horizontal turn speed ai_CannonAim steers (-1 .. 1) */
    float pitchSpeed;        /* +0x58, the vertical one */
    float yawDelta;          /* +0x5c, the yaw step of this frame (speed * dt * 1.5) */
    float pitchDelta;        /* +0x60, the pitch step (speed * dt * 0.8) */
    char pad64[4];
    unsigned last;           /* +0x68, the clock of the last frame (ai_CannonScene's dt) */
    unsigned now;            /* +0x6c, the clock of this frame (ai_CannonCamera's recoil timer) */
    unsigned resultClock;    /* +0x70, the clock of the last shot result update (ai_CannonShotResults, every 0x15e ms) */
    int win74;               /* +0x74, closed at the end */
    int recoilStart;         /* +0x78, the clock of the shot (ai_CannonCamera's recoil timer) */
    int fired;               /* +0x7c, set while the shot runs: ai_CannonAim then does nothing */
    int dragX;               /* +0x80, the mouse position at the last drag step */
    int dragY;               /* +0x84 */
    int dragState;           /* +0x88, 0 = no drag, 1 = dragging */
    D3OBJECT *obj8c[3];      /* +0x8c, 0x90, 0x94: the three barrels ai_CannonCamera turns */
    D3OBJECT *obj98;         /* +0x98, the cannon object whose matrix holds the aim */
    D3OBJECT *obj9c;         /* +0x9c, the wimpel object */
    int selected;            /* +0xa0, the selected object */
    int win_a4;              /* +0xa4, closed at the end */
    int scaleBar;            /* +0xa8, the bar of ai_GetDefaultEffectScale (0 .. 100, ai_CannonPanel) */
    int pitchBar;            /* +0xac, the bar of the pitch (ai_CannonPitchRatio, 0 .. 100) */
    int kindSlider;          /* +0xb0, the first slider (0 .. kind): GVCANNON.sliderA */
    int speedSlider;         /* +0xb4, the slider whose value * 0.01 is the shot speed (ai_CannonBuildPanel) */
    int fireBtn;             /* +0xb8, ai_CannonFireCheck reads it as the fire button */
    int quitBtn;             /* +0xbc, a click on it leaves the scene */
    int win_c0;              /* +0xc0, closed at the end */
    int chargeBar;           /* +0xc4, the bar of the run's charge of the selected barrel (0 .. 100) */
    int selBtn[3];           /* +0xc8, the controls of the three cannon objects (ai_CannonSwitchAnim 0 .. 2) */
    int picL;                /* +0xd4, the two picture objects of ai_CannonBuildPanel (d2_CreatePicObj), removed at the end */
    int picR;                /* +0xd8 */
    int maxSel;              /* +0xdc, the highest selectable object: gv_BuildingCannon's balls (0 .. 2) */
    int kind;                /* +0xe0, the cannon kind (gv_BuildingCannon, 0 .. 4): the top of kindSlider */
    D3VECTOR bmax;           /* +0xe4, not part of the cleared 0xe4 bytes: the bounds of the cannon object (d3t_GetObjectBounds) */
    float padf0;
    D3VECTOR bmin;           /* +0xf4 */
} CANNONSCENE;
char * gv_FindEffect(int id); int gv_RequestEffectLoad(struct AIBLD *bld); int cm_GetSlotState(); void gv_SetEffectCallback();
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */
void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b); void sv_SetPanelEnabled(); void d3sky_Init(struct D3SKY *s, char build);
void d3t_GetObjectBounds(); 
char d3if_ShowTree(struct D3OBJECT *obj, char show);
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);                 /* 0x447190, d3_interface.h */
char d3if_Show(struct D3OBJECT *obj, char show);        /* 0x4471b0, d3_interface.h */
void he_ShowObjectInfos(); void ai_CannonAxes(CANNONSCENE *sc); void ai_CannonAim(CANNONSCENE *sc, float dt);
void ai_CannonShotResults(CANNONSCENE *sc); void ai_CannonSwitchAnim(char *rec, CANNONSCENE *sc, GVEFFECT *run, int sel, int mask); int ai_CannonFireCheck(CANNONSCENE *sc); void ai_CannonExplosion(CANNONSCENE *sc, char *rec);
void sv_SetStatusText(char *s); void sv_RedrawScreen();
void d3if_Prerender(char map); 

char ai_ShotCallbackArg[0x40] = { 0 };  /* 0x6fa020, the argument gv_SetEffectCallback stores with the shot callback ai_ShotTableStore (ai_CannonScene); never read by the code, its type is unknown; it runs to the tutorial state record at 0x6fa060 */
TUTSTATE ai_TutorialState = { 0 };  /* 0x6fa060, the tutorial state record ai_Tutorial points to (the tutorial code reads it through S); its last read field is progressSlider at +0x6c, the 0x10 bytes to he_OptionsPath at 0x6fa0e0 stay unowned */

/* 0x57e270, the tutorial's cannon scene (the record rec = the cannon): the run record gv_FindEffect (rec->id), else
   loaded by gv_RequestEffectLoad (rec) and waited out (slot state, pump 0x20007), or returns; the clip rectangle,
   camera and scene saved, the BLACK fade out (pump mask = sv_FrameMask | 0x80 without 0x2108, | 0x10000, | 0x100),
   the objects of the open views marked disabled (their d2_Objs indices listed) while the scene runs, the viewport
   and the clip rectangle set to the 3D area, the scene placed from d3_PointToWorld of the cannon object, the cannon
   base "KANONE_UNTERBAU_Aa" with its three barrel objects (ROHR / DOPPEL_ROHR / DREIFACH_ROHR) and the flag
   "AO_KANONE_WIMPEL_Aa" (animation _ADDON\AO_KANONE_WIMPEL_Aa.baf), the panel built (ai_CannonBuildPanel) and the
   selection restored from GVCANNON; the loop pumps 0x2656c7 and, unless the run record is released (flags), steers
   with ai_CannonAim / ai_CannonCamera / ai_CannonPanel / ai_CannonShotResults, a click on a control starts
   ai_CannonSwitchAnim (0 .. 2) and a fire (ai_CannonFireCheck) ai_CannonExplosion; a right button release or a click
   on the quit control sets sv_PopRequest. The teardown fades out again, closes the three windows, removes the two
   objects, deletes the 3D objects, re-enables the listed objects, restores viewport, clip rectangle, scene, sky and
   camera and releases the callback of the run record
   @deep read whole against the original (667 = 667 ins). Fixed meaning: the 3D objects and the selection
   are CANNONSCENE fields (obj8c[3]/obj98/obj9c/selected at +0x8c..+0xa0, the original reloads them from
   sc+0x98 etc. and does the wimpel stateFlags as 3 RMWs), last/now are sc+0x68/+0x6c (loop end sc.last =
   sc.now), the teardown removes the pic objects +0xd4/+0xd8 (not +0xb4/+0xb8), the rect save reads Right,
   Left, Top, Bottom, the view walk is an int-typed address (signed jl). Remaining code hunks, each
   allocation: +0x336/+0x38c the mask copy in edi (or edi,0x100 hoisted) vs ours in esi; +0x3e3 (113) the
   view/object loop: the original keeps pb in esi and the zero in edi, so pv and n live in
   [esp+0x14]/[esp+0x20] and pv-0x1a0 is rebuilt in ecx per object (base &enabled), ours holds pv in esi
   and n in a slot; +0x4be / +0x77b / +0x7de / +0x800 / +0x817 the zero register (edi vs esi), pb in esi vs
   our slot [esp+0x64] (push esi vs mov edx,[esp+0x64]; push edx), m starting from the edi that already
   holds 0 (no xor); +0xa0b / +0xa3f aligner artifacts of the same 3 d2_CloseView + 2 d2_RemoveObj with
   shifted slots. The 12 differing branch targets are all inside these hunks (+0x17f..+0x217 the view loop,
   +0x565/+0x585 the m loop, +0x785 the fade wait) and differ by 1..9 bytes from the reg vs slot encodings,
   the statement structure is the same. Frame 0x734 vs 0x750: the original's extra 0x1c are temp slots
   (int->float conversions, the qword at 0x14 shared with pv) */
void ai_CannonScene(char *rec)
{
    GVEFFECT *run;
    CANNONSCENE sc;
    D3VECTOR rotv;
    unsigned short list[0x2f2];
    int n;
    int m;
    int rect[4];
    int scene;
    unsigned mask;
    float dt;
    unsigned char *pb;
    D3VECTOR cam3;
    D3VECTOR save3;

    kb_Down[0x2f] = 0;
    memset(&sc, 0, 0xe4);
    sc.last = d3_Time;
    mask = sv_FrameMask | 0x80;
    if (!CR->obj)
        return;
    run = (GVEFFECT *)gv_FindEffect(CR->id);
    if (!run) {
        m = gv_RequestEffectLoad(rec);
        while (m != -1 && cm_GetSlotState(m) == 0)
            sv_RunFrame(0, 0x20007);
        run = (GVEFFECT *)gv_FindEffect(CR->id);
        if (!run)
            return;
    }
    gv_SetEffectCallback(CR->id, ai_ShotTableStore, ai_ShotCallbackArg);
    mask = (mask & 0xffffdef7) | 0x10000;
    cam3.x = d3_Camera->pos.x;
    cam3.y = d3_Camera->pos.y;
    cam3.z = d3_Camera->pos.z;
    save3.x = d3_Camera->rotation.x;
    save3.y = d3_Camera->rotation.y;
    save3.z = d3_Camera->rotation.z;
    rect[2] = d2_DirtyRectRight;
    rect[0] = d2_DirtyRectLeft[0];
    rect[1] = d2_DirtyRectTop;
    rect[3] = d2_DirtyRectBottom;
    scene = d3_CurrentScene;
    d2_SetPointerSprite(0, 0);
    sv_ClearSelection();
    d2_ClearPointerItems();
    sv_ResetScreenClip();
    sv_PanelFlag = 0;
    d2_CaptionCopy[0] = 0;
    pb = (unsigned char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
    while (!(*pb & 4))
        sv_RunFrame(0, mask | 0x100);
    n = 0;
    {
        int i;
        int v;  /* the view's address, walked as an int (the original's compare is signed) */

        for (v = (int)d2_Views; v < (int)&d2_Views[0x30]; v += sizeof(D2VIEW)) {
            if (((D2VIEW *)v)->active != 0 && ((D2VIEW *)v)->enabled != 0) {
                for (i = 0; i < 0x300; i++) {
                    if (d2_Objs[i].type != 0 && (int)d2_Objs[i].buf == v && d2_Objs[i].disabled == 0) {
                        d2_Objs[i].disabled = 1;
                        list[n++] = i;
                    }
                }
            }
        }
    }
    d3_SetViewport((float)d2_ScreenW, (float)d2_ScreenH - 150.0f, 0.0f, 75.0f, d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW, d2_ScreenH);
    d2_DirtyRectRight = 0;
    d2_DirtyRectLeft[0] = d2_ScreenW;
    d2_DirtyRectTop = 0x4b;
    d2_DirtyRectBottom = d2_ScreenH - 0x4b;
    sv_SetPanelEnabled(0);
    cam3.x = d3_Camera->pos.x;
    cam3.y = d3_Camera->pos.y;
    cam3.z = d3_Camera->pos.z;
    save3.x = d3_Camera->rotation.x;
    save3.y = d3_Camera->rotation.y;
    save3.z = d3_Camera->rotation.z;
    d3if_SelectScene(0, 0);
    if (d3_Sky)
        d3sky_Init(d3_Sky, 1);
    d3t_GetObjectBounds(&sc.bmin, &sc.bmax, CR->obj);
    if (gv_Cannon.bld != CR->id) {
        gv_Cannon.bld = CR->id;
        gv_Cannon.sliderA = 0;
        gv_Cannon.sliderB = 0;
        gv_Cannon.selected = 0;
        memcpy(gv_Cannon.camTarget, &save3, sizeof(save3));
    }
    d3_PointToWorld(CR->obj, &CR->obj->pos, &sc.pos);
    memcpy(&sc.rot, gv_Cannon.camTarget, sizeof(sc.rot));
    sc.pos.y = (sc.bmax.y - sc.bmin.y) * 0.8f + sc.pos.y;
    rotv.x = 0.0f;
    rotv.y = sc.rot.y;
    rotv.z = sc.rot.z;
    sc.obj98 = d3_CreateObject(0, &d3_Origin, &rotv, "KANONE_UNTERBAU_Aa");
    sc.obj8c[0] = d3_CreateObject(sc.obj98, &d3_Origin, (D3VECTOR *)ai_ZeroRot, "KANONE_ROHR_Aa");
    sc.obj8c[1] = d3_CreateObject(sc.obj98, &d3_Origin, (D3VECTOR *)ai_ZeroRot, "KANONE_DOPPEL_ROHR_Aa");
    sc.obj8c[2] = d3_CreateObject(sc.obj98, &d3_Origin, (D3VECTOR *)ai_ZeroRot, "KANONE_DREIFACH_ROHR_Aa");
    sc.selected = gv_Cannon.selected;
    sc.zoom = 0.0f;
    d3_SetObjectPos(sc.obj98, &sc.pos);
    d3_SetObjectRot(sc.obj98, (D3VECTOR *)&sc.rot);
    sc.obj9c = d3_CreateObject(sc.obj98, (D3VECTOR *)&ai_WimpelPos, (D3VECTOR *)ai_ZeroRot, "AO_KANONE_WIMPEL_Aa");
    d3t_PlayAnim(sc.obj9c, "_ADDON\\AO_KANONE_WIMPEL_Aa.baf", 1);
    sc.obj9c->stateFlags = (sc.obj9c->stateFlags & 0xf7) | 4;
    sc.obj9c->stateFlags |= 0x40;
    sc.obj9c->objectFlags &= 0xfd;
    d2_ClearSurface(d2_ScreenSurf);
    sv_RunFrame(0, 0x86);
    d2_ClearSurface(d2_ScreenSurf);
    d2_ReleaseFade(pb);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 10);
    ai_CannonBuildPanel(&sc, run);
    ai_CannonPanel(rec, &sc, run);
    if (sc.selected > sc.maxSel)
        sc.selected = sc.maxSel;
    for (m = 0; m < 3; m++)
        d3if_Show(sc.obj8c[m], 0);
    d3if_Show(sc.obj8c[sc.selected], 1);
    d3if_ShowTree(CR->obj, 0);
    he_ShowObjectInfos((int)CR->obj, 0);
    d2_ClickedId = -1;
    ai_CannonAxes(&sc);
    ai_ShotTable[2].id = -1;
    ai_ShotTable[1].id = -1;
    ai_ShotTable[0].id = -1;
    while (sv_RunFrame((int)ai_CannonScene, 0x2656c7) != 0) {
        sv_SetPanelEnabled(0);
        sc.now = d3_Time;
        dt = (float)(sc.now - sc.last) * 0.013513514f;
        if (run->flags == 0 && (dm_L.rup != 0 || (d2_ClickedId != -1 && d2_PointerObj == sc.quitBtn))) {
            sv_PopRequest = 1;
        } else {
            ai_CannonAim(&sc, dt);
            ai_CannonCamera(&sc);
            ai_CannonPanel(rec, &sc, run);
            ai_CannonShotResults(&sc);
            if (run->flags == 0) {
                if (d2_ClickedId != -1) {
                    if (d2_PointerObj == sc.selBtn[0])
                        ai_CannonSwitchAnim(rec, &sc, run, 0, 0x2656c7);
                    else if (d2_PointerObj == sc.selBtn[1])
                        ai_CannonSwitchAnim(rec, &sc, run, 1, 0x2656c7);
                    else if (d2_PointerObj == sc.selBtn[2])
                        ai_CannonSwitchAnim(rec, &sc, run, 2, 0x2656c7);
                }
                if (run->flags == 0 && ai_CannonFireCheck(&sc) != 0) {
                    ai_CannonExplosion(&sc, rec);
                    ai_ShotTable[2].id = -1;
                    ai_ShotTable[1].id = -1;
                    ai_ShotTable[0].id = -1;
                }
            }
        }
        sc.last = sc.now;
    }
    sv_SetStatusText(ch_EmptyName);
    pb = (unsigned char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
    while (!(*pb & 4))
        sv_RunFrame(0, mask);
    d2_CloseView(sc.win_a4);
    d2_CloseView(sc.win_c0);
    d2_CloseView(sc.win74);
    d2_RemoveObj(sc.picL);
    d2_RemoveObj(sc.picR);
    if (sc.obj8c[0])
        d3if_Delete(sc.obj8c[0]);
    if (sc.obj8c[1])
        d3if_Delete(sc.obj8c[1]);
    if (sc.obj8c[2])
        d3if_Delete(sc.obj8c[2]);
    if (sc.obj9c)
        d3if_Delete(sc.obj9c);
    if (sc.obj98)
        d3if_Delete(sc.obj98);
    d3if_ShowTree(CR->obj, 1);
    he_ShowObjectInfos((int)CR->obj, 1);
    for (m = 0; m < n; m++)
        d2_Objs[list[m]].disabled = 0;
    d3_SetViewport((float)(sv_ViewRectRight - sv_ViewRectLeft), (float)(sv_ViewRectBottom - sv_ViewRectTop), (float)sv_ViewRectLeft, (float)sv_ViewRectTop, d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW, d2_ScreenH);
    d2_DirtyRectRight = rect[2];
    d2_DirtyRectLeft[0] = rect[0];
    d2_DirtyRectTop = rect[1];
    d2_DirtyRectBottom = rect[3];
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    d3if_SelectScene(scene, 0);
    if (d3_Sky)
        d3sky_Init(d3_Sky, 1);
    d3_SetObjectPos(d3_Camera, &cam3);
    d3_SetObjectRot(d3_Camera, (D3VECTOR *)&save3);
    init_SetDetailLevel(d3_Terrain, 0);
    d3if_Prerender(1);
    d2_ReleaseFade(pb);
    d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 10);
    gv_SetEffectCallback(CR->id, 0, 0);
}

double _CIfmod();

#include <math.h>
#define FMIN(a, b) ((a) > (b) ? (b) : (a))
#define FMAX(a, b) ((a) > (b) ? (a) : (b))
/* 0x57ec40, the cannon aiming update (1169 B): sc = the scene, dt = the frame time. Does nothing while the shot
   runs (sc->fired). The zoom moves by +dt * 0.8 (with the key kb_Down[0x18]) or -dt * 1.6 and is clamped to 0 .. 1.
   The mouse pitches and yaws the cannon: with the left button held (dm_L.ldown) inside the middle of the screen
   (0x50 from the top and bottom) the first step only remembers the mouse (dragState 0 -> 1, speeds cleared) and
   the next ones turn the pitch by 0.008333334 and the yaw by 0.011111111 per mouse pixel; outside that zone the
   drag ends. Without a drag (or after the button is let go) the keys kb_Down[0xc8 / 0xd0 / 0xcb / 0xcd] or the
   mouse at the screen edges (5 pixels) give the directions a (vertical) and b (horizontal) in -1 .. 1; each
   speed (pitchSpeed, yawSpeed) falls toward 0 without a direction and otherwise grows by dt * 0.8 (or * 0.25
   when it runs against the direction) up to +-1, the frame deltas are speed * dt * 0.8 (pitch) and * 1.5 (yaw).
   The pitch is kept within -15 .. 45 degrees (the pitch speed is cleared at a stop), the yaw within 0 .. 2 pi */
void ai_CannonAim(CANNONSCENE *sc, float dt)
{
    int a = 0;
    int b = 0;
    int dx;
    int dy;
    short y;

    if (sc->fired)
        return;
    if (kb_Down[0x18] != 0)
        sc->zoom = dt * 0.8f + sc->zoom;
    else
        sc->zoom = sc->zoom - dt * 1.6f;
    sc->zoom = FMAX(FMIN(sc->zoom, 1.0f), 0.0f);
    y = dm_L.y;
    if (dm_L.ldown != 0 && (y < 0x50 || y > d2_ScreenH - 0x50)) {
        sc->dragState = 0;
    } else if (dm_L.ldown != 0 && sc->dragState == 0) {
        sc->dragX = dm_L.x;
        sc->dragY = dm_L.y;
        sc->dragState = 1;
        sc->pitchDelta = 0;
        sc->yawDelta = 0;
        sc->pitchSpeed = 0;
        sc->yawSpeed = 0;
    } else if (dm_L.ldown == 0 && sc->dragState == 1) {
        sc->dragState = 0;
    } else if (sc->dragState == 1) {
        dx = dm_L.x - sc->dragX;
        dy = y - sc->dragY;
        sc->dragX = dm_L.x;
        sc->dragY = dm_L.y;
        sc->rot.x = sc->rot.x - dy * 0.008333334f;
        sc->rot.y = sc->rot.y - dx * 0.011111111f;
    } else {
        if (kb_Down[0xc8] != 0 || y <= 5)
            a = 1;
        if (kb_Down[0xd0] != 0 || y >= d2_ScreenH - 5)
            a--;
        if (kb_Down[0xcb] != 0 || dm_L.x <= 5)
            b = 1;
        if (kb_Down[0xcd] != 0 || dm_L.x >= d2_ScreenW - 10)
            b--;
        if (sc->yawSpeed != 0.0f && b == 0) {
            if (sc->yawSpeed < 0.0f) {
                sc->yawSpeed = dt * 0.8f + sc->yawSpeed;
                sc->yawSpeed = sc->yawSpeed < 0.0f ? sc->yawSpeed : 0.0f;
            } else {
                sc->yawSpeed = sc->yawSpeed - dt * 0.8f;
                sc->yawSpeed = sc->yawSpeed > 0.0f ? sc->yawSpeed : 0.0f;
            }
            sc->yawDelta = dt * sc->yawSpeed * 1.5f;
        } else if (b != 0) {
            if (b > 0) {
                if (sc->yawSpeed < 0.0f)
                    sc->yawSpeed += dt * 0.8f;
                else
                    sc->yawSpeed += dt * 0.25f;
                sc->yawSpeed = sc->yawSpeed < 1.0f ? sc->yawSpeed : 1.0f;
            } else {
                if (sc->yawSpeed > 0.0f)
                    sc->yawSpeed -= dt * 0.8f;
                else
                    sc->yawSpeed -= dt * 0.25f;
                sc->yawSpeed = sc->yawSpeed > -1.0f ? sc->yawSpeed : -1.0f;
            }
            sc->yawDelta = dt * sc->yawSpeed * 1.5f;
        }
        if (sc->pitchSpeed != 0.0f && a == 0) {
            if (sc->pitchSpeed < 0.0f) {
                sc->pitchSpeed = dt * 0.8f + sc->pitchSpeed;
                sc->pitchSpeed = sc->pitchSpeed < 0.0f ? sc->pitchSpeed : 0.0f;
            } else {
                sc->pitchSpeed = sc->pitchSpeed - dt * 0.8f;
                sc->pitchSpeed = sc->pitchSpeed > 0.0f ? sc->pitchSpeed : 0.0f;
            }
            sc->pitchDelta = dt * sc->pitchSpeed * 0.8f;
        } else if (a != 0) {
            if (a > 0) {
                if (sc->pitchSpeed < 0.0f)
                    sc->pitchSpeed += dt * 0.8f;
                else
                    sc->pitchSpeed += dt * 0.25f;
                sc->pitchSpeed = sc->pitchSpeed < 1.0f ? sc->pitchSpeed : 1.0f;
            } else {
                if (sc->pitchSpeed > 0.0f)
                    sc->pitchSpeed -= dt * 0.8f;
                else
                    sc->pitchSpeed -= dt * 0.25f;
                sc->pitchSpeed = sc->pitchSpeed > -1.0f ? sc->pitchSpeed : -1.0f;
            }
            sc->pitchDelta = dt * sc->pitchSpeed * 0.8f;
        }
        sc->rot.x = sc->pitchDelta + sc->rot.x;
        sc->rot.y = sc->yawDelta + sc->rot.y;
    }
    if (sc->rot.x < ai_CannonPitchMin || sc->rot.x > ai_CannonPitchMax) {
        sc->rot.x = (sc->rot.x < ai_CannonPitchMin ? ai_CannonPitchMin : sc->rot.x) < ai_CannonPitchMax
                        ? (sc->rot.x < ai_CannonPitchMin ? ai_CannonPitchMin : sc->rot.x) : ai_CannonPitchMax;
        sc->pitchSpeed = 0;
    }
    sc->rot.y = (float)fmod(sc->rot.y, 6.2831855f);
}

void d3_MatTranspose(); float d3_VecAngle2(); int ai_GetDefaultEffect();


/* 0x57f0e0, the cannon camera update (1254 B): sc = the scene struct. The cannon object (sc->obj98) and the
   three barrels (sc->obj8c) get their rotation reset, the wimpel (sc->obj9c) is turned from the aim axis
   (d3_VecAngle2 - 1.5707964 - sc->rot.y) and the ai_CannonAxes directions (recomputed while no shot runs) turn
   the aim axis of the cannon object's matrix (sc->obj98->matrix) into the direction v, normalized; the camera
   starts 30 units back from the scene position (y + 20) and is placed / rotated through d3_Camera, the mouse
   value (sc->speedSlider) * 0.01 as the speed, the pitch's place in the camera's range (ai_CameraPitchMin ..
   ai_CameraPitchMax) and in the cannon's (ai_CannonPitchMin .. ai_CannonPitchMax) the angle and the height, the zoom
   the distance (up to ai_CannonZoomRange). The recoil (sc->recoilStart + 0x54 > sc->now): the fade from the elapsed clock (0.25 per
   ms during the first 4, then (1 - (elapsed - 4) * 0.0125)^2) times 50 pushes the cannon object back along the
   level aim direction and the wimpel out from ai_WimpelPos; after it the wimpel is set back and the cannon object placed at
   the scene position
   @deep read whole against the original (334 = 334 ins, frame 0x78 = 0x78). Fixed meaning: the wimpel turn
   goes to pos.y (not z), the pitch ratios are (x - min) / (max - min) over the const data ai_CameraPitch*
   / ai_CannonPitch* (loaded, divided at run time), 1450 is ai_CannonZoomRange, the camera rotation reuses
   v (= sc->rot, the camera direction is a copied d scaled by fade), the 30-unit back step and the recoil
   step scale first and subtract after (v = fade * v; v = sc->pos - v, SetObjectPos(obj98, &v)), the wimpel
   offset is fade * axis + ai_WimpelPos, the fired / recoil branches as the original lays them out (reset
   inline, recoil returns). Remaining, each allocation: +0x70 / +0xce code 9 / 11: the original holds 0 in
   ebx from the zero block on (pos.w, pos.x, pos.z stored from ebx), ours stores immediates and zeroes edi
   after the call (edi->ebx everywhere else); +0x235 code 29 and the fpu hunks at +0x27e..+0x2f1: the
   camera block, the original keeps d.x / d.y in FPU registers (fld sc->dirX / dirY forwarded, only d.z
   copied to its slot) and spills fade * dirX to the dead ratio slot 0x14, ours keeps d.y / d.z copies in
   slots and d.x forwarded (tried 8 placements of the copy and the uncopied form: 142 is the best, the
   uncopied form is 196 and a smaller frame); +0x431 / +0x4bc fpu: the same store order with v at 0x1c vs
   0x24 (our slots 8 lower, ratio2 at 0x84 needs disp32: the 15 bytes size difference); the slots / order
   hunks are the stack layout */
void ai_CannonCamera(CANNONSCENE *sc)
{
    D3VECTOR4 pos;    /* the rotations, then the camera position */
    D3VECTOR v;       /* the aim direction, then the camera rotation */
    D3VECTOR d;       /* the camera's direction (sc->dirX ..) */
    D3MATRIX t;       /* the transpose of sc->obj98->matrix */
    float ratio;      /* the pitch within the camera's range */
    float ratio2;     /* the pitch within the cannon's range */
    float speed;
    float fade;
    float k;
    int def;
    int i;

    pos.x = 0;
    pos.y = sc->rot.y;
    pos.z = 0;
    d3_SetObjectRot(sc->obj98, (char *)&pos);
    pos.x = sc->rot.x;
    pos.y = 0;
    pos.z = 0;
    for (i = 0; i < 3; i++)
        d3_SetObjectRot(sc->obj8c[i], (char *)&pos);
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    pos.w = 0;
    def = ai_GetDefaultEffect();
    d3_MatTranspose(&sc->obj98->matrix, (char *)&t);
    pos.y = (float)(d3_VecAngle2((char *)&d3_ZAxis, def) - 1.5707964f) - sc->rot.y;
    pos.x = 0;
    pos.z = 0;
    d3_SetObjectRot(sc->obj9c, (char *)&pos);
    if (sc->fired == 0)
        ai_CannonAxes(sc);
    v.x = d3_ZAxis.x * sc->obj98->matrix._11 + d3_ZAxis.y * sc->obj98->matrix._21 + d3_ZAxis.z * sc->obj98->matrix._31;
    v.y = d3_ZAxis.x * sc->obj98->matrix._12 + d3_ZAxis.y * sc->obj98->matrix._22 + d3_ZAxis.z * sc->obj98->matrix._32;
    v.z = d3_ZAxis.x * sc->obj98->matrix._13 + d3_ZAxis.y * sc->obj98->matrix._23 + d3_ZAxis.z * sc->obj98->matrix._33;
    d3_Normalize((char *)&v);
    pos.x = v.x * 30.0f;
    pos.y = v.y * 30.0f;
    pos.z = v.z * 30.0f;
    pos.x = sc->pos.x - pos.x;
    pos.y = sc->pos.y - pos.y;
    pos.z = sc->pos.z - pos.z;
    pos.y += 20.0f;
    v.x = sc->rot.x;
    v.y = sc->rot.y;
    v.z = sc->rot.z;
    ratio = (v.x - ai_CameraPitchMin) / (ai_CameraPitchMax - ai_CameraPitchMin);
    ratio2 = (v.x - ai_CannonPitchMin) / (ai_CannonPitchMax - ai_CannonPitchMin);
    speed = (float)d2_GetObjValue(sc->speedSlider) * 0.01f;
    d.x = sc->dirX;
    d.y = sc->dirY;
    d.z = sc->dirZ;
    pos.y = ((0.5f - 0.25f * speed) * ratio2 * sc->zoom + ratio) * 30.0f + pos.y;
    v.x = (speed * sc->zoom * 0.5f + ((1.0f - sc->zoom) * 0.7f + (float)sqrt(1.0f - sc->zoom) * 0.3f))
          * (ai_CameraPitchZero - ai_CameraPitchMin) * ratio2 + ai_CameraPitchMin;
    fade = ((speed * ratio2 * 0.5f + 1.0f) * ai_CannonZoomRange * ratio2 + 50.0f) * sc->zoom;
    d.x = fade * d.x;
    d.y = fade * d.y;
    d.z = fade * d.z;
    pos.x += d.x;
    pos.y += d.y;
    pos.z += d.z;
    d3_SetObjectPos(d3_Camera, (char *)&pos);
    d3_SetObjectRot(d3_Camera, (char *)&v);
    if (sc->fired == 0)
        ai_CannonAxes(sc);
    if (sc->fired != 0) {
        if (sc->recoilStart + 0x54 <= sc->now) {
            sc->fired = 0;
            sc->recoilStart = 0;
            d3_SetObjectPos(sc->obj9c, (char *)&ai_WimpelPos);
        } else {
            if (sc->recoilStart + 4 > sc->now)
                fade = (float)(sc->now - sc->recoilStart) * 0.25f;
            else {
                fade = 1.0f - (float)(sc->now - sc->recoilStart - 4) * 0.0125f;
                fade = fade * fade;
            }
            v.x = sc->dirX;
            v.y = 0;
            v.z = sc->dirZ;
            d3_Normalize((char *)&v);
            k = 1.0f;
            if (sc->rot.x > 0.0f)
                k = 1.0f - sc->rot.x / ai_CameraPitchMax;
            fade = k * fade * 50.0f;
            v.x = fade * v.x;
            v.y = fade * v.y;
            v.z = fade * v.z;
            v.x = sc->pos.x - v.x;
            v.y = sc->pos.y - v.y;
            v.z = sc->pos.z - v.z;
            d3_SetObjectPos(sc->obj98, (char *)&v);
            pos.x = d3_ZAxis.x;
            pos.y = d3_ZAxis.y;
            pos.z = d3_ZAxis.z;
            pos.x = fade * pos.x;
            pos.y = fade * pos.y;
            pos.z = fade * pos.z;
            pos.x += ai_WimpelPos.x;
            pos.y += ai_WimpelPos.y;
            pos.z += ai_WimpelPos.z;
            d3_SetObjectPos(sc->obj9c, (char *)&pos);
            return;
        }
    }
    d3_SetObjectPos(sc->obj98, (char *)sc);
}

void d3_DirToWorld();
int gm_FindById();

/* 0x57f5d0, the cannon's three axes in world space: d3_DirToWorld of the first barrel (sc->obj8c[0]) turns
   d3_ZAxis into the aim direction sc->dir, d3_YAxis into the up vector sc->up and d3_XAxis into the side
   vector sc->side */
void ai_CannonAxes(CANNONSCENE *sc)
{
    d3_DirToWorld(sc->obj8c[0], (char *)&d3_ZAxis, (char *)&sc->dir);
    d3_DirToWorld(sc->obj8c[0], (char *)&d3_YAxis, (char *)&sc->up);
    d3_DirToWorld(sc->obj8c[0], (char *)&d3_XAxis, (char *)&sc->side);
}
/* 0x57f620, the fire check: nothing (0) while a shot runs (sc->fired) or the zoom is above 0.01; a click on the
   fire button (sc->fireBtn) or the key kb_Down[0x32] (DIK_M) fires: the key is cleared, the recoil clock starts
   (sc->recoilStart = sc->now) and sc->fired is set; 1 */

int ai_CannonFireCheck(CANNONSCENE *sc)
{
    if (sc->fired != 0 || sc->zoom > 0.01f)
        return 0;
    if (d2_ClickedId != -1 && d2_PointerObj == sc->fireBtn) {
        kb_Down[0x32] = 0;
        sc->recoilStart = sc->now;
        sc->fired = 1;
        return 1;
    }
    if (kb_Down[0x32] != 0) {
        kb_Down[0x32] = 0;
        sc->recoilStart = sc->now;
        sc->fired = 1;
        return 1;
    }
    return 0;
}
/* 0x57f690, the shot results: every 0x15e ms (sc->resultClock) the result view sc->win74 is cleared and the first
   of the three ai_ShotTable records whose id is its index is printed (ai_ShotDirty set): each entry's building
   (gm_FindById of e[i].obj, the list ends at a missing one) as the text 0x406c (no owner or the player's) or 0x406d
   (with its owner), the hit 0x4071 / 0x4073 (e[i].a == 1, more than 25 away) / 0x4072 (within 25); "$C" starts the
   text, cm_TextNewline separates the entries. A record with entries, or with none while ai_ShotDirty is still set
   (then the text 0x406e), is consumed (id = -1, ai_ShotDirty = 0) and shows the view; without any the view is
   hidden and ai_ShotDirty cleared */

void ai_CannonShotResults(CANNONSCENE *sc)
{
    int n;
    int rec;
    int i;
    AIBLD *bld;
    int v;
    unsigned short w;

    n = 0;
    if (sc->resultClock + 0x15e <= (unsigned)d3_Time) {
        sc->resultClock = d3_Time;
        d2_SetForm(sc->win74, 0);
        for (rec = 0; rec < 3; rec++) {
            i = 0;
            if (ai_ShotTable[rec].id == rec) {
                ai_ShotDirty = 1;
                for (i = 0; i < 3; i++) {
                    gm_FindById((int *)&bld, 0, 0, ai_ShotTable[rec].e[i].obj);
                    if (bld == 0)
                        break;
                    v = 0x4071;
                    if (ai_ShotTable[rec].e[i].a == 1)
                        v = ai_ShotTable[rec].e[i].bf > 25.0f ? 0x4073 : 0x4072;
                    if (i != 0)
                        io_PrintText(cm_TextNewline);
                    else
                        io_PrintText("$C");
                    w = bld->owner;
                    if (w == 0xffff || (void *)&ai_CharsT[w] == (void *)ai_PlayerChar)
                        io_PrintText(0x406c, bld, v);
                    else
                        io_PrintText(0x406d, (int)w, bld, v);
                    n++;
                }
                if (n != 0) {
                    ai_ShotDirty = 0;
                    ai_ShotTable[rec].id = -1;
                    d2_SetViewEnabled(sc->win74, 1);
                    return;
                }
                if (ai_ShotDirty != 0) {
                    ai_ShotDirty = 0;
                    ai_ShotTable[rec].id = -1;
                    io_PrintText("$C");
                    io_PrintText(0x406e);
                    d2_SetViewEnabled(sc->win74, 1);
                    return;
                }
            }
        }
        d2_SetViewEnabled(sc->win74, 0);
        ai_ShotDirty = 0;
    }
}

float gv_SalvoCharge(); void gv_BuildingCannon(char *o, unsigned *a, unsigned *b); int gv_CannonCost(); float ai_GetDefaultEffectScale();
int _ftol(); float ai_CannonPitchRatio(CANNONSCENE *sc);
void txt_Format(char *out, int id0, ...); int gm_CharCurrency(char *c, char p);

/* 0x57f890, the cannon control panel: rec = the building record, sc = the scene, run = its effect slot. The two
   sliders (sc->kindSlider, clamped to the cannon kind of gv_BuildingCannon, and sc->speedSlider) give the salvo's
   charge (gv_SalvoCharge); the barrel controls 1 / 2 are enabled by the balls (sc->maxSel), the selected one
   (sc->selected) is highlighted and checked; gv_CannonCost of the player's character gives the count and price; the
   fire button is enabled at the zoom 0 (<= 0.01) with a cost, the run's charge of the barrel at least the salvo's
   and the run not busy (flags & 2), the quit button while not busy; the bars show ai_GetDefaultEffectScale, the
   pitch (ai_CannonPitchRatio) and the run's charge (* 100); the window sc->win_c0 gets the texts 0x4068 / 0x4069 with
   the count and price and the character's currency and money ("%T %a"), each centred at half the window's width
   in font 0x54; the slider values, the barrel and the aim go back to gv_Cannon */
void ai_CannonPanel(char *rec, CANNONSCENE *sc, GVEFFECT *run)
{
    char buf[0x40];
    int a;
    int b;
    float c;
    int m;
    int n;
    int l1;
    int l2;
    int w;

    a = d2_GetObjValue(sc->kindSlider);
    b = d2_GetObjValue(sc->speedSlider);
    c = gv_SalvoCharge(a, b, 1);
    gv_BuildingCannon(rec, (unsigned *)&sc->maxSel, (unsigned *)&sc->kind);
    n = sc->kind;
    if (n < a)
        a = n;
    d2_SetObjValue4(sc->kindSlider, 0, n, a);
    d2_SetObjEnabled(sc->selBtn[1], sc->maxSel >= 1);
    d2_SetObjEnabled(sc->selBtn[2], sc->maxSel >= 2);
    d2_Objs[sc->selBtn[0]].highlighted = sc->selected == 0;
    d2_Objs[sc->selBtn[1]].highlighted = sc->selected == 1;
    d2_Objs[sc->selBtn[2]].highlighted = sc->selected == 2;
    d2_Objs[sc->selBtn[0]].checked = sc->selected == 0;
    d2_Objs[sc->selBtn[1]].checked = sc->selected == 1;
    d2_Objs[sc->selBtn[2]].checked = sc->selected == 2;
    m = gv_CannonCost(ai_Chars[ai_PlayerId], a, b, sc->selected + 1, &l1, &l2);
    if (sc->zoom <= 0.01f && m != 0 && run->charge[sc->selected] >= c && (run->flags & 2) == 0)
        w = 1;
    else
        w = 0;
    d2_SetObjEnabled(sc->fireBtn, w);
    d2_SetObjEnabled(sc->quitBtn, (~(unsigned)run->flags >> 1) & 1);
    d2_SetObjValue(sc->scaleBar, 0, 100, (int)(ai_GetDefaultEffectScale() * 100.0f), 1);
    d2_SetObjValue(sc->pitchBar, 0, 100, (int)(ai_CannonPitchRatio(sc) * 100.0f), 1);
    d2_SetObjValue(sc->chargeBar, 0, 100, (int)(run->charge[sc->selected] * 100.0f), 1);
    d2_SetForm(sc->win_c0, 2);
    io_PrintText("$C");
    m = d2_CreateWinText(0, 0xf, txt_Ptrs[0x4068], d2_LastWindow);
    d2_Objs[m].centred = 1;
    d2_Objs[m].w = d2_Wins[d2_LastWindow].w / 2;
    d2_SetObjFont(m, 0x54);
    txt_Format(buf, (int)"%T %a", l2, l1);
    m = d2_CreateWinText(0, 0x23, buf, d2_LastWindow);
    d2_Objs[m].centred = 1;
    d2_Objs[m].w = d2_Wins[d2_LastWindow].w / 2;
    d2_SetObjFont(m, 0x54);
    m = d2_CreateWinText(d2_Wins[d2_LastWindow].w / 2, 0xf, txt_Ptrs[0x4069], d2_LastWindow);
    d2_Objs[m].centred = 1;
    d2_Objs[m].w = d2_Wins[d2_LastWindow].w / 2;
    d2_SetObjFont(m, 0x54);
    txt_Format(buf, (int)"%T %a", gm_CharCurrency(ai_Chars[ai_PlayerId], ai_OwnPlayer), ((AICHAR *)ai_Chars[ai_PlayerId])->money);
    m = d2_CreateWinText(d2_Wins[d2_LastWindow].w / 2, 0x23, buf, d2_LastWindow);
    d2_Objs[m].centred = 1;
    d2_Objs[m].w = d2_Wins[d2_LastWindow].w / 2;
    d2_SetObjFont(m, 0x54);
    gv_Cannon.sliderA = a;
    gv_Cannon.sliderB = b;
    gv_Cannon.selected = sc->selected;
    gv_Cannon.camTarget[0] = sc->rot.x;
    gv_Cannon.camTarget[1] = sc->rot.y;
    gv_Cannon.camTarget[2] = sc->rot.z;
}
/* 0x57fdb0, the cannon's pitch as 0 .. 1 (28 B): (sc->rot.x - ai_CannonPitchMin) / (ai_CannonPitchMax -
   ai_CannonPitchMin) */

float ai_CannonPitchRatio(CANNONSCENE *sc)
{
    return (sc->rot.x - ai_CannonPitchMin) / (ai_CannonPitchMax - ai_CannonPitchMin);
}
/* 0x57fdd0, the barrel switch: rec = the building record, sc = the scene, run = its effect slot, sel = the barrel
   (0 .. 2), mask = the pump mask; nothing for the selected barrel or one above sc->maxSel. Two halves of
   ai_CannonSwitchMs each, every frame pumping sv_RunFrame (this function, mask) and updating the panel
   (ai_CannonPanel); past the half's time sv_PopRequest ends it. The cannon object sinks back along the level aim
   direction by ai_CannonSwitchDist * f and the wimpel moves from ai_WimpelPos along d3_ZAxis by as much, f going
   0 -> 1 in the first half and 1 -> 0 in the second; between them the barrels are switched (only sel shown) and
   sc->selected = sel; at the end sc->now = the clock
   @deep read whole against the original (241 vs 247 ins). Fixed meaning: the two halves were written with
   the wrong offsets; now the scene's fields (dirX / dirZ, pos, obj98 / obj9c, obj8c, selected, maxSel,
   now), the half's time and the sink distance are the const data ai_CannonSwitchMs / ai_CannonSwitchDist
   (0x5eec94 / 98: the original divides by it, a literal would be a multiply), the clock is read through
   d3_Time (the original reloads it for the compare and for f), past the time sv_PopRequest, v =
   dist * f * v then pos - v, the wimpel ai_WimpelPos + (dist * f) * d3_ZAxis. Remaining, each allocation:
   +0x98 / +0x253 code 22 / 25: the same sub / mov / fild of the elapsed time with eax/ecx/edx rotated and
   the dirX / dirZ copies scheduled around the fild; the frame 0x28 vs 0x30: the original keeps the
   compare's and f's qword temps apart (0x10 and 0x18), ours shares one; +0x14c / +0x30e fpu 12: the wimpel
   products, the original keeps dist * f on the FPU stack after fst f (fmul st, st(2) and a reload) where
   ours pops it and multiplies from memory (the 3 instructions per half: the count difference); tried the
   folded w = W + f * axis form: worse (106) */
void ai_CannonSwitchAnim(char *rec, CANNONSCENE *sc, GVEFFECT *run, int sel, int mask)
{
    D3VECTOR v;
    D3VECTOR w;
    unsigned start;
    float f;
    int i;

    start = d3_Time;
    if (sel == sc->selected || sel > sc->maxSel)
        return;
    while (sv_RunFrame((int)ai_CannonSwitchAnim, mask) != 0) {
        ai_CannonPanel(rec, sc, run);
        if ((float)(d3_Time - start) > ai_CannonSwitchMs) {
            sv_PopRequest = 1;
        } else {
            v.x = sc->dirX;
            v.y = 0;
            v.z = sc->dirZ;
            f = (float)(d3_Time - start) / ai_CannonSwitchMs;
            d3_Normalize((char *)&v);
            v.x = ai_CannonSwitchDist * f * v.x;
            v.y = ai_CannonSwitchDist * f * v.y;
            v.z = ai_CannonSwitchDist * f * v.z;
            v.x = sc->pos.x - v.x;
            v.y = sc->pos.y - v.y;
            v.z = sc->pos.z - v.z;
            d3_SetObjectPos(sc->obj98, (char *)&v);
            w.x = d3_ZAxis.x;
            w.y = d3_ZAxis.y;
            w.z = d3_ZAxis.z;
            f = ai_CannonSwitchDist * f;
            w.x = f * w.x;
            w.y = f * w.y;
            w.z = f * w.z;
            w.x = ai_WimpelPos.x + w.x;
            w.y = ai_WimpelPos.y + w.y;
            w.z = ai_WimpelPos.z + w.z;
            d3_SetObjectPos(sc->obj9c, (char *)&w);
        }
    }
    for (i = 0; i < 3; i++)
        d3if_Show(sc->obj8c[i], 0);
    d3if_Show(sc->obj8c[sel], 1);
    sc->selected = sel;
    start = d3_Time;
    while (sv_RunFrame((int)ai_CannonSwitchAnim, mask) != 0) {
        ai_CannonPanel(rec, sc, run);
        if ((float)(d3_Time - start) > ai_CannonSwitchMs) {
            sv_PopRequest = 1;
        } else {
            v.x = sc->dirX;
            v.y = 0;
            v.z = sc->dirZ;
            f = 1.0f - (float)(d3_Time - start) / ai_CannonSwitchMs;
            d3_Normalize((char *)&v);
            v.x = ai_CannonSwitchDist * f * v.x;
            v.y = ai_CannonSwitchDist * f * v.y;
            v.z = ai_CannonSwitchDist * f * v.z;
            v.x = sc->pos.x - v.x;
            v.y = sc->pos.y - v.y;
            v.z = sc->pos.z - v.z;
            d3_SetObjectPos(sc->obj98, (char *)&v);
            w.x = d3_ZAxis.x;
            w.y = d3_ZAxis.y;
            w.z = d3_ZAxis.z;
            f = ai_CannonSwitchDist * f;
            w.x = f * w.x;
            w.y = f * w.y;
            w.z = f * w.z;
            w.x = ai_WimpelPos.x + w.x;
            w.y = ai_WimpelPos.y + w.y;
            w.z = ai_WimpelPos.z + w.z;
            d3_SetObjectPos(sc->obj9c, (char *)&w);
        }
    }
    sc->now = d3_Time;
}

void gv_CannonScatter(char *o, float *a, float *b); float gv_CannonItemFactor(); void ai_ShotTableReset(); int ai_InitShot(struct SHOTREC *shot, struct D3VECTOR *pos, struct D3VECTOR *dir, float charge, float size, float density);
int ai_ScatterShot(struct SHOTREC *shot, float sideSpread, struct D3VECTOR *side, float otherSpread, struct D3VECTOR *other, float speedSpread); int gv_FireShots(struct AIBLD *bld, struct AICHAR *ch, struct SHOTREC *a, struct SHOTREC *b, struct SHOTREC *d, float v, int cost, int x); struct SBBANK; struct SBSAMPLE *sb_PlayVariation(struct SBBANK *bank, char *name, int idx, int loops, int pan, int volume, int fade); void ai_CannonBuildPanel();

/* 0x580160, fires the cannon: sc = the scene, rec = the building record. The two sliders give the cannon kind a
   and the speed b; gv_CannonCost of the player's character for sel + 1 balls (sc->selected) gives the cost, none ->
   nothing. The charge of a ball is b% of the way from ai_CannonBalls[a].a to ai_CannonItemFactor * .b, its size
   ai_CannonBalls[a].c, its density ai_IronDensity; the shot table is reset (ai_ShotTableReset). Barrel 0 fires one ball
   from 50 ahead along sc->dir, barrel 1 two from 40 ahead 5 to either side (sc->side), barrel 2 three: two of them
   3 below (sc->up) and 5 to either side, one 5 above; each ball is built (ai_InitShot) and scattered by the
   building's spreads (gv_CannonScatter, ai_ScatterShot along side / up), the salvo fired (gv_FireShots with
   gv_SalvoCharge) and its slot awaited (pump 0x20007). Then one LOCATIONS\DOMIZIL\KANONE1.esc script per ball
   runs at the barrel's object (sc->obj8c[sel]) with its offset (x, y, z) and the "Explosion" sound plays (the
   variation 3 for one ball, 2 for two, 1 for three)
   @deep Read against the whole original 0x580160..0x580a4d and reconciled: the wait loop corrected to
   while (h != -1 && cm_GetSlotState(h) == 0) (one call, top test; the calls now equal); every call,
   argument, constant and the statement order of the three barrel cases match (t = v*k, p += t per
   component, the same order up*5/up*-3/side*5/side*-5, the same shots to InitShot/ScatterShot/FireShots,
   the same tail). What is left is x87 stack allocation and frame packing: the original spills one base
   component per case to a dead int slot (x2 0x2c / y3 0x60) and int-copies it (mov edx,[esp+0x60]; mov
   [esp+0x38],edx), keeps base.x duplicated on the FPU stack (fld st(0)/fld st(3)) where ours re-reads
   memory, so its frame is 0x18 larger (0x1cc vs 0x1b4, all slots shifted), the a*12 index lands in ecx
   after the call instead of edi before it (0x5801fd hunk), and the push/load orders around
   InitShot/ScatterShot/SalvoCharge differ (0x5802a9, 0x58045a, 0x5804ba hunks). Tried: a base vector with
   struct copies p1 = p2 = base (dist 514 -> 475 but 656 vs 645 instructions, all three components
   int-copied where the original copies one), p2 = p1 (487), a ball pointer for ai_CannonBalls[a] (no
   change); kept the chain assignments. ours 635 vs original 645. */
void ai_CannonExplosion(CANNONSCENE *sc, char *rec)
{
    SHOTREC shotA;
    SHOTREC shotB;
    SHOTREC shotC;
    D3VECTOR fwd;
    D3VECTOR side;
    D3VECTOR up;
    D3VECTOR p1;
    D3VECTOR p2;
    D3VECTOR p3;
    int a;
    int b;
    int cost;
    int x;
    int h;
    int n;
    float spread;
    float speedSpread;
    float charge;
    float size;
    EVTSCRIPT *e1;
    EVTSCRIPT *e2;
    EVTSCRIPT *e3;
    int x1, y1, z1;
    int x2 = 0, y2 = 0, z2 = 0;
    int y3 = 0, z3 = 0;

    a = d2_GetObjValue(sc->kindSlider);
    b = d2_GetObjValue(sc->speedSlider);
    if (gv_CannonCost(ai_Chars[ai_PlayerId], a, b, sc->selected + 1, &cost, &x) == 0)
        return;
    gv_CannonScatter(rec, &spread, &speedSpread);
    charge = (gv_CannonItemFactor(rec) * ai_CannonBalls[a].b - ai_CannonBalls[a].a) * ((float)b * 0.01f) + ai_CannonBalls[a].a;
    size = ai_CannonBalls[a].c;
    ai_ShotTableReset();
    fwd.x = sc->dir.x;
    fwd.y = sc->dir.y;
    fwd.z = sc->dir.z;
    side.x = sc->side.x;
    side.y = sc->side.y;
    side.z = sc->side.z;
    up.x = sc->up.x;
    up.y = sc->up.y;
    up.z = sc->up.z;
    if (sc->selected == 0) {
        D3VECTOR t;

        t.x = fwd.x * 50.0f;
        t.y = fwd.y * 50.0f;
        t.z = fwd.z * 50.0f;
        p1.x = t.x + sc->pos.x;
        p1.y = t.y + sc->pos.y;
        p1.z = t.z + sc->pos.z;
        ai_InitShot(&shotA, &p1, &sc->dir, charge, size, ai_IronDensity);
        ai_ScatterShot(&shotA, spread, &sc->side, spread, &sc->up, speedSpread);
        h = gv_FireShots((AIBLD *)rec, (AICHAR *)ai_Chars[ai_PlayerId], &shotA, 0, 0, gv_SalvoCharge(a, b, 1), cost, x);
        x1 = 0;
        y1 = 0;
        z1 = 50;
        n = 1;
    } else if (sc->selected == 1) {
        D3VECTOR t;

        t.x = fwd.x * 40.0f;
        t.y = fwd.y * 40.0f;
        t.z = fwd.z * 40.0f;
        p1.x = p2.x = t.x + sc->pos.x;
        p1.y = p2.y = t.y + sc->pos.y;
        p1.z = p2.z = t.z + sc->pos.z;
        t.x = side.x * 5.0f;
        t.y = side.y * 5.0f;
        t.z = side.z * 5.0f;
        p1.x = t.x + p1.x;
        p1.y = t.y + p1.y;
        p1.z = t.z + p1.z;
        t.x = side.x * -5.0f;
        t.y = side.y * -5.0f;
        t.z = side.z * -5.0f;
        p2.x = t.x + p2.x;
        p2.y = t.y + p2.y;
        p2.z = t.z + p2.z;
        ai_InitShot(&shotB, &p1, &sc->dir, charge, size, ai_IronDensity);
        ai_InitShot(&shotA, &p2, &sc->dir, charge, size, ai_IronDensity);
        ai_ScatterShot(&shotB, spread, &sc->side, spread, &sc->up, speedSpread);
        ai_ScatterShot(&shotA, spread, &sc->side, spread, &sc->up, speedSpread);
        h = gv_FireShots((AIBLD *)rec, (AICHAR *)ai_Chars[ai_PlayerId], &shotB, &shotA, 0, gv_SalvoCharge(a, b, 1), cost, x);
        x1 = 5;
        y1 = 0;
        z1 = 40;
        x2 = -5;
        y2 = 0;
        z2 = 40;
        n = 2;
    } else if (sc->selected == 2) {
        D3VECTOR t;

        t.x = fwd.x * 40.0f;
        t.y = fwd.y * 40.0f;
        t.z = fwd.z * 40.0f;
        p1.x = p2.x = p3.x = t.x + sc->pos.x;
        p1.y = p2.y = p3.y = t.y + sc->pos.y;
        p1.z = p2.z = p3.z = t.z + sc->pos.z;
        t.x = up.x * 5.0f;
        t.y = up.y * 5.0f;
        t.z = up.z * 5.0f;
        p3.x = t.x + p3.x;
        p3.y = t.y + p3.y;
        p3.z = t.z + p3.z;
        t.x = up.x * -3.0f;
        t.y = up.y * -3.0f;
        t.z = up.z * -3.0f;
        p1.x = t.x + p1.x;
        p1.y = t.y + p1.y;
        p1.z = t.z + p1.z;
        p2.x = t.x + p2.x;
        p2.y = t.y + p2.y;
        p2.z = t.z + p2.z;
        t.x = side.x * 5.0f;
        t.y = side.y * 5.0f;
        t.z = side.z * 5.0f;
        p1.x = t.x + p1.x;
        p1.y = t.y + p1.y;
        p1.z = t.z + p1.z;
        t.x = side.x * -5.0f;
        t.y = side.y * -5.0f;
        t.z = side.z * -5.0f;
        p2.x = t.x + p2.x;
        p2.y = t.y + p2.y;
        p2.z = t.z + p2.z;
        ai_InitShot(&shotC, &p1, &sc->dir, charge, size, ai_IronDensity);
        ai_InitShot(&shotA, &p2, &sc->dir, charge, size, ai_IronDensity);
        ai_InitShot(&shotB, &p3, &sc->dir, charge, size, ai_IronDensity);
        ai_ScatterShot(&shotC, spread, &sc->side, spread, &sc->up, speedSpread);
        ai_ScatterShot(&shotA, spread, &sc->side, spread, &sc->up, speedSpread);
        ai_ScatterShot(&shotB, spread, &sc->side, spread, &sc->up, speedSpread);
        h = gv_FireShots((AIBLD *)rec, (AICHAR *)ai_Chars[ai_PlayerId], &shotC, &shotA, &shotB, gv_SalvoCharge(a, b, 1), cost, x);
        x1 = 5;
        y1 = -3;
        z1 = 40;
        x2 = -5;
        y2 = -3;
        z2 = 40;
        y3 = 5;
        z3 = 40;
        n = 3;
    } else
        return;
    while (h != -1 && cm_GetSlotState(h) == 0)
        sv_RunFrame(0, 0x20007);
    if (n == 0)
        return;
    if (n == 3) {
        e1 = evt_Load("LOCATIONS\\DOMIZIL\\KANONE1.esc");
        e2 = evt_Load("LOCATIONS\\DOMIZIL\\KANONE1.esc");
        e3 = evt_Load("LOCATIONS\\DOMIZIL\\KANONE1.esc");
        evt_RunScriptArgs(e1, 4, x1, y1, z1, sc->obj8c[2]);
        evt_RunScriptArgs(e2, 4, x2, y2, z2, sc->obj8c[2]);
        evt_RunScriptArgs(e3, 4, 0, y3, z3, sc->obj8c[2]);
        sb_PlayVariation(0, "Explosion", 1, 1, 0x3f, 0x7f, 0);
        return;
    }
    if (n == 2) {
        e1 = evt_Load("LOCATIONS\\DOMIZIL\\KANONE1.esc");
        e2 = evt_Load("LOCATIONS\\DOMIZIL\\KANONE1.esc");
        evt_RunScriptArgs(e1, 4, x1, y1, z1, sc->obj8c[1]);
        evt_RunScriptArgs(e2, 4, x2, y2, z2, sc->obj8c[1]);
        sb_PlayVariation(0, "Explosion", 2, 1, 0x3f, 0x7f, 0);
        return;
    }
    e1 = evt_Load("LOCATIONS\\DOMIZIL\\KANONE1.esc");
    evt_RunScriptArgs(e1, 4, x1, y1, z1, sc->obj8c[0]);
    sb_PlayVariation(0, "Explosion", 3, 1, 0x3f, 0x7f, 0);
}


/* 0x580a50, resets the three 0x38-byte shot records of 0x6f9f60 (57 B): each record: +0 = -1 and its three
   0x10-byte entries: +8 = -1, +0xc = 100000.0, +0x10 = 0, +0x14 = 0 */
void ai_ShotTableReset(void)
{
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        ai_ShotTable[i].id = -1;
        for (j = 0; j < 3; j++) {
            ai_ShotTable[i].e[j].obj = -1;
            ai_ShotTable[i].e[j].dist = 100000.0f;
            ai_ShotTable[i].e[j].a = 0;
            ai_ShotTable[i].e[j].b = 0;
        }
    }
}

#define d2_CreatePicObjS ((int (*)(short, short, int))d2_CreatePicObj)  /* @original ai_CannonBuildPanel subtracts the picture height in 16 bits and pushes it unextended (0x580b18 sub cx / push ecx): a short y, as d2.c's callers (d2_WinOpenPic) */

/* 0x580a90, builds the cannon control panel of the scene sc (run = its effect slot): the two edge pictures (by the
   screen width 800 / 0x400 / other: 0x7cb / 0x7cc / 0x7cd at the top left, 0x7ce / 0x7cf / 0x7d0 at the bottom
   left) into sc->picL / picR; the form "Locations\domizil\cannon_bottom" (sc->win_a4), its window moved 0x19 left
   of the screen centre line, with the fire / quit buttons (texts 0x7d5 / 0x7d4), the kind slider (0 .. 4,
   gv_Cannon.sliderA, labelled txt 0x4061 with the five kinds 0x4063 .. 0x4067) and the speed slider (0 .. 100,
   gv_Cannon.sliderB, txt 0x4062), then the three barrel controls (0x7d3 / 0x7d1 / 0x7d2) into sc->selBtn; the form
   "Locations\domizil\cannon_top" (sc->win_c0) 0x4b left of the line with the three bars (0 .. 100): the default
   effect scale, the pitch bar (also from the effect scale) and the run's charge of the selected barrel (txt 0x405e
   .. 0x4060); the form "Locations\domizil\cannon_res" (sc->win74) placed with 3 and disabled; sc->resultClock =
   the clock. The buttons are interactive with the style 3
   @deep Rewritten from the whole original 0x580a90..0x580fdd (typed: CANNONSCENE / GVEFFECT run, d2_Wins /
   d2_Objs / d2_Pics fields, txt_Ptrs 0x405e..0x4067, the third bar run->charge[sc->selected] * 100, the
   5-argument SetObjValue, the short y of d2_CreatePicObj); 388 vs 388 instructions, every call, argument
   and constant equal. Left: the address selection of d2_MoveObj(d2_Wins[d2_LastWindow].x, ..,
   d2_Wins[d2_LastWindow].parent): ours shl eax,2 / mov ecx,[eax+X] / mov dx,[eax+X], the original mov
   edx,[ecx*4+0xf8f38c] / lea eax,[ecx*4+d2_Wins] / mov ax,[eax+4] (0x580b62, 0x580e44); the same C gives
   the same difference in sv_UpdateCityPanel (0x523eb8, 0x523f26), (&d2_Wins[i])->x and a sized
   d2_Wins[0x60] tried without change; the hunks after it are the register rotation it starts (ecx->eax,
   eax->ecx, ecx->edx) and the push/load order around the bar SetObjValue calls (0x580ebe: push eax before
   / after mov reg,[esi+0xa8], swapped between bar 1 and 2), and the push ebp placement of the second
   button (0x580c45). */
void ai_CannonBuildPanel(CANNONSCENE *sc, GVEFFECT *run)
{
    int h;
    int mid;
    int pic;
    int base;
    int id;

    h = d2_ScreenH;
    mid = (h - (d2_ScreenW >> 4) * 9) >> 1;
    h -= mid;
    if (d2_ScreenW == 800) {
        pic = 0x7cb;
        base = 0x7ce;
    } else if (d2_ScreenW == 0x400) {
        pic = 0x7cc;
        base = 0x7cf;
    } else {
        pic = 0x7cd;
        base = 0x7d0;
    }
    sc->picL = d2_CreatePicObj(0, 0, pic);
    sc->picR = d2_CreatePicObjS(0, d2_ScreenH - d2_Pics[base].h, base);
    sc->win_a4 = d2_OpenForm(0, 0, "Locations\\domizil\\cannon_bottom");
    d2_PlaceMainWindowF(sc->win_a4, 1);
    d2_SetForm(sc->win_a4, 0);
    d2_MoveObj(d2_Wins[d2_LastWindow].x, h - 0x19, d2_Wins[d2_LastWindow].parent);
    d2_SetForm(sc->win_a4, 2);
    id = d2_WinOpenPic(0x59, 0xf, 0x7d5, d2_LastWindow);
    sc->fireBtn = id;
    d2_Objs[id].interactive = 1;
    d2_Objs[sc->fireBtn].style = 3;
    id = d2_WinOpenPic(0x9b, 0xf, 0x7d4, d2_LastWindow);
    sc->quitBtn = id;
    d2_Objs[id].interactive = 1;
    d2_Objs[sc->quitBtn].style = 3;
    d2_SetForm(sc->win_a4, 1);
    sc->kindSlider = d2_WinOpenSlider(0x140, 0x14, 100, 0, 100, 0x7b, 0x82, d2_LastWindow);
    d2_SetObjValue(sc->kindSlider, 0, 4, gv_Cannon.sliderA, 1);
    d2_SetLabel(sc->kindSlider, txt_Ptrs[0x4061]);
    d2_SetList(sc->kindSlider, 5, txt_Ptrs[0x4063], txt_Ptrs[0x4064], txt_Ptrs[0x4065], txt_Ptrs[0x4066], txt_Ptrs[0x4067]);
    sc->speedSlider = d2_WinOpenSlider(0x140, 0x32, 100, 0, 100, 0x7b, 0x82, d2_LastWindow);
    d2_SetObjValue(sc->speedSlider, 0, 100, gv_Cannon.sliderB, 1);
    d2_SetLabel(sc->speedSlider, txt_Ptrs[0x4062]);
    id = d2_WinOpenPic(0x14, 0x17, 0x7d3, d2_LastWindow);
    sc->selBtn[0] = id;
    d2_Objs[id].interactive = 1;
    d2_Objs[sc->selBtn[0]].style = 3;
    id = d2_WinOpenPic(0x56, 0x17, 0x7d1, d2_LastWindow);
    sc->selBtn[1] = id;
    d2_Objs[id].interactive = 1;
    d2_Objs[sc->selBtn[1]].style = 3;
    id = d2_WinOpenPic(0x98, 0x17, 0x7d2, d2_LastWindow);
    sc->selBtn[2] = id;
    d2_Objs[id].interactive = 1;
    d2_Objs[sc->selBtn[2]].style = 3;
    sc->win_c0 = d2_OpenForm(0, 0, "Locations\\domizil\\cannon_top");
    d2_PlaceMainWindowF(sc->win_c0, 1);
    d2_SetForm(sc->win_c0, 0);
    d2_MoveObj(d2_Wins[d2_LastWindow].x, mid - 0x4b, d2_Wins[d2_LastWindow].parent);
    d2_SetForm(sc->win_c0, 1);
    sc->scaleBar = d2_WinOpenSlider(0x96, 0xb, 200, 0, 100, 0x75, 0x4c2, d2_LastWindow);
    d2_SetObjValue(sc->scaleBar, 0, 100, (int)(ai_GetDefaultEffectScale() * 100.0f), 1);
    d2_SetLabel(sc->scaleBar, txt_Ptrs[0x405e]);
    sc->pitchBar = d2_WinOpenSlider(0x96, 0x1f, 200, 0, 100, 0x75, 0x4c2, d2_LastWindow);
    d2_SetObjValue(sc->pitchBar, 0, 100, (int)(ai_GetDefaultEffectScale() * 100.0f), 1);
    d2_SetLabel(sc->pitchBar, txt_Ptrs[0x405f]);
    sc->chargeBar = d2_WinOpenSlider(0x96, 0x33, 200, 0, 100, 0x75, 0x4c2, d2_LastWindow);
    d2_SetObjValue(sc->chargeBar, 0, 100, (int)(run->charge[sc->selected] * 100.0f), 1);
    d2_SetLabel(sc->chargeBar, txt_Ptrs[0x4060]);
    sc->win74 = d2_OpenForm(0, 0, "Locations\\domizil\\cannon_res");
    d2_PlaceMainWindowF(sc->win74, 3);
    d2_SetViewEnabled(sc->win74, 0);
    sc->resultClock = d3_Time;
}

int he_EventOnBuilding(); int sv_MessageBox();

/* 0x580fe0, whether the building rec may be spied on: 0 when an event already runs on it (he_EventOnBuilding of
   rec->id), the message text 0x3ff7 shown; 1 when it is free */
int ai_CanSpyOn(AIBLD *rec)
{
    int r;

    r = he_EventOnBuilding(rec->id);
    if (r)
        sv_MessageBox((int)txt_Ptrs[0x3ff7], 0);
    return r == 0;
}

int gm_GetWorth(unsigned short id); int d2_WinOpenSlider(); int sv_CheckGoodAmount(); short * gm_PlayerRecord(char *c);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out); int snd_PlayVoice(int id, int bank, char *text, int var); void sv_CameraFollow();

/* 0x581020, the cabinet's spy window for the building rec: the form "Locations\Domizil\kabinett_spy" (centred,
   fitted, the default pointer) with the title txt 0x4020 centred over the window's width in font 0x54, a slider of
   three steps (txt 0x4021 .. 0x4023) choosing the payment: 1%, 1.5% or 2% of the player's worth (gm_GetWorth), and
   the two buttons from the text 0x4024 on (pay / leave). Each frame prints the texts 0x401e / 0x401c and 0x401f
   with the chosen amount; the right mouse button or the second button leaves (sv_PopRequest). Pay: when the player
   has the money (sv_CheckGoodAmountB) and the building may be spied on (ai_CanSpyOn), the amount is added to the
   player record's spyPaid and moved away from the player (cm_SendMoveGoodsUnchecked), the message 0x99 goes to the
   building's owner (flags 2, the game time, the building id, the step and ai_PickActionBuilding's pick of the
   player's character), its slot is waited for, the voice "KOMMENTARE" plays and the window leaves. At the end the
   view is closed and the camera follows again */
void ai_CabinetSpyWindow(AIBLD *rec)
{
    AIMSG msg;
    int v[3];
    int worth;
    int win;
    int slider;
    int i;
    int amount;
    int id;
    int btnPay;
    int btnLeave;
    int pick;
    LARGE_INTEGER mask;

    worth = gm_GetWorth(ai_PlayerId);
    v[0] = (int)(worth * 0.01f);
    v[1] = (int)(worth * 0.015f);
    v[2] = (int)(worth * 0.02f);
    win = d2_OpenForm(0, 0, "Locations\\Domizil\\kabinett_spy");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 2);
    id = d2_CreateWinText(0, 3, txt_Ptrs[0x4020], d2_LastWindow);
    d2_Objs[id].centred = 1;
    d2_Objs[id].w = d2_Wins[d2_LastWindow].w;
    d2_SetObjFont(id, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, txt_Ptrs[0x4021], txt_Ptrs[0x4022], txt_Ptrs[0x4023]);
    d2_SetForm(win, 3);
    i = io_PrintText((char *)0x4024);
    btnPay = d2_GetWinObj(win, 3, i);
    btnLeave = d2_GetWinObj(win, 3, i + 1);
    d2_ClickedId = -1;
    do {
        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText("$C");
        io_PrintText((char *)0x401e, 0x401c);
        amount = v[i];
        io_PrintText((char *)0x401f, amount);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], amount, ai_OwnPlayer) != 0 && ai_CanSpyOn(rec) != 0) {
                    AIFIN *fin = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);

                    if (fin != 0)
                        fin->spyPaid += amount;
                    cm_SendMoveGoodsUnchecked(-1, ((AICHAR *)ai_Chars[ai_PlayerId])->netId, ai_OwnPlayer, amount);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x99;
                    msg.flags = 2;
                    msg.arg[2] = -1;
                    msg.arg[3] = -1;
                    msg.arg[4] = 3;
                    msg.arg[5] = i;
                    pick = -1;
                    msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                    msg.fromId = rec->id;
                    msg.time = ai_Time;
                    msg.arg[1] = rec->id;
                    mask.LowPart = 0x7b4c4a0;
                    mask.HighPart = 0;
                    ai_PickActionBuilding((struct AIPER *)ai_Chars[ai_PlayerId], 0, mask.QuadPart, 0, 100.0f, &pick);
                    msg.arg[0] = pick;
                    id = cm_SendLongMessage((char *)&msg);
                    while (id != -1 && cm_GetSlotState(id) == 0)
                        sv_RunFrame(0, 0x20007);
                    snd_PlayVoice(-7, 0, "KOMMENTARE", 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == btnLeave) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_CabinetSpyWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}

int ai_AmountMenu(); int amt_GetKindLevel(int k); int sv_CheckMoney(); void cm_SendMoneyDelta(int a, int b);

/* 0x581400, the cabinet's spy window on a character for the building rec: the target is picked in the amount menu
   (AMOUNTCFG flags 1, mode 0x400, kind 6, no ids left out; title txt 0x400d), none -> return. The three steps of
   the slider (txt 0x4011 .. 0x4013) cost 1%, 1.5% or 2% of the worth of the player and the target together
   (gm_GetWorth) and take 1, 1.5 or 2 times the office level of the target (amt_GetKindLevel (officeKind) / 3 + 2)
   in money (sv_CheckMoney); the form "Locations\Domizil\kabinett_spy" with the title txt 0x4010, the two buttons
   from the text 0x4014 on (pay / leave), each frame the texts 0x400e / 0x400c (the target) and 0x400f (the amount
   and the cost). Pay: when the player has both (sv_CheckGoodAmountB, sv_CheckMoney) and the building may be spied on
   (ai_CanSpyOn), the cost is added to the player record's spyPaid and moved away (cm_SendMoveGoodsUnchecked), the
   amount taken off (gm_SendMoneyDelta), the message 0x99 goes to the building's owner (flags 2, the game time,
   the target's netId, 2, the step, the target's own building / building, else ai_PickActionBuilding's pick), its
   slot is waited for, the voice "KOMMENTARE" plays and the window leaves; the right button or the second button
   leaves too */
void ai_CabinetSpyOnCharacter(AIBLD *rec)
{
    AMOUNTCFG q;
    int amt[3];
    int win;
    int slider;
    int btnPay;
    int btnLeave;
    int cost[3];
    float level;
    int worth;
    AICHAR *p;
    int i;
    int k;
    int id;
    LARGE_INTEGER mask;

    worth = gm_GetWorth(ai_PlayerId);
    memset(&q, 0, sizeof(q));
    memset(q.ids, 0xff, sizeof(q.ids));
    q.flags = 1;
    q.mode = 0x400;
    q.kind = 6;
    p = (AICHAR *)ai_AmountMenu(&q, txt_Ptrs[0x400d], 0, 0);
    if (!p)
        return;
    worth += gm_GetWorth(p->id);
    level = (float)(amt_GetKindLevel(p->officeKind) / 3 + 2);
    amt[0] = (int)level;
    amt[1] = (int)(level * 1.5f);
    amt[2] = (int)(level * 2.0f);
    cost[0] = (int)(worth * 0.01f);
    cost[1] = (int)(worth * 0.015f);
    cost[2] = (int)(worth * 0.02f);
    win = d2_OpenForm(0, 0, "Locations\\Domizil\\kabinett_spy");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 2);
    k = d2_CreateWinText(0, 3, txt_Ptrs[0x4010], d2_LastWindow);
    d2_Objs[k].centred = 1;
    d2_Objs[k].w = d2_Wins[d2_LastWindow].w;
    d2_SetObjFont(k, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, txt_Ptrs[0x4011], txt_Ptrs[0x4012], txt_Ptrs[0x4013]);
    d2_SetForm(win, 3);
    k = io_PrintText((char *)0x4014);
    btnPay = d2_GetWinObj(win, 3, k);
    btnLeave = d2_GetWinObj(win, 3, k + 1);
    d2_ClickedId = -1;
    do {
        AIMSG msg;

        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText("$C");
        io_PrintText((char *)0x400e, 0x400c, p->id);
        io_PrintText((char *)0x400f, amt[i], cost[i]);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], cost[i], ai_OwnPlayer) != 0 &&
                    sv_CheckMoney(ai_Chars[ai_PlayerId], amt[i]) == 1 && ai_CanSpyOn(rec) != 0) {
                    AIFIN *fin = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);

                    if (fin != 0)
                        fin->spyPaid += cost[i];
                    cm_SendMoveGoodsUnchecked(-1, ((AICHAR *)ai_Chars[ai_PlayerId])->netId, ai_OwnPlayer, cost[i]);
                    gm_SendMoneyDelta(((AICHAR *)ai_Chars[ai_PlayerId])->netId, -amt[i]);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x99;
                    msg.flags = 2;
                    msg.arg[3] = -1;
                    msg.arg[4] = 2;
                    msg.arg[5] = i;
                    msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                    msg.fromId = rec->id;
                    msg.time = ai_Time;
                    msg.arg[2] = p->netId;
                    msg.arg[1] = rec->id;
                    if (p->bld2 != 0)
                        msg.arg[0] = p->bld2->id;
                    else if (p->bld != 0)
                        msg.arg[0] = p->bld->id;
                    else {
                        int pick;

                        mask.LowPart = 0x7b4c4a0;
                        mask.HighPart = 0;
                        pick = -1;
                        ai_PickActionBuilding((struct AIPER *)ai_Chars[ai_PlayerId], 0, mask.QuadPart, 0, 100.0f, &pick);
                        msg.arg[0] = pick;
                    }
                    id = cm_SendLongMessage((char *)&msg);
                    while (id != -1 && cm_GetSlotState(id) == 0)
                        sv_RunFrame(0, 0x20007);
                    snd_PlayVoice(-7, 0, "KOMMENTARE", 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == btnLeave) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_CabinetSpyOnCharacter, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x581950, the cabinet's spy window paid in money only, on a character for the building rec (the name is a reasoned
   guess: the third of the kabinett_spy windows, message kind 1 where ai_CabinetSpyOnCharacter sends 2 and
   ai_CabinetSpyWindow 3): the target is picked in the amount menu (AMOUNTCFG flags 1, mode 0x400, kind 6, no ids
   left out; title txt 0x3ffc), none -> the camera follows again and return. The level n = the office level of the
   target (amt_GetKindLevel (officeKind) / 3 + 2); the three steps of the slider (txt 0x4000 .. 0x4002) cost
   n + 3, n + 5 or n + 8 in money; the form "Locations\Domizil\kabinett_spy" with the title txt 0x3fff, the two
   buttons from the text 0x4003 on (pay / leave), each frame the texts 0x3ffd / 0x3ffb (the target, n) and 0x3ffe
   (the amount). Pay: when the player has the money (sv_CheckMoney) and the building may be spied on (ai_CanSpyOn),
   the amount is taken off (gm_SendMoneyDelta), the message 0x99 goes to the building's owner (flags 2, the game
   time, the target's netId, 1, the step, the target's own building / building, else ai_PickActionBuilding's pick),
   its slot is waited for, the voice "KOMMENTARE" plays and the window leaves; the right button or the second
   button leaves too */
void ai_CabinetSpyByMoney(AIBLD *rec)
{
    AMOUNTCFG q;
    int amt[3];
    AICHAR *p;
    int n;
    int win;
    int slider;
    int btnPay;
    int btnLeave;
    int i;
    int k;
    int id;
    LARGE_INTEGER mask;

    memset(&q, 0, sizeof(q));
    memset(q.ids, 0xff, sizeof(q.ids));
    q.flags = 1;
    q.mode = 0x400;
    q.kind = 6;
    p = (AICHAR *)ai_AmountMenu(&q, txt_Ptrs[0x3ffc], 0, 0);
    if (!p) {
        sv_CameraFollow();
        return;
    }
    n = amt_GetKindLevel(p->officeKind) / 3 + 2;
    amt[0] = n + 3;
    amt[1] = n + 5;
    amt[2] = n + 8;
    win = d2_OpenForm(0, 0, "Locations\\Domizil\\kabinett_spy");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 2);
    k = d2_CreateWinText(0, 3, txt_Ptrs[0x3fff], d2_LastWindow);
    d2_Objs[k].centred = 1;
    d2_Objs[k].w = d2_Wins[d2_LastWindow].w;
    d2_SetObjFont(k, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, txt_Ptrs[0x4000], txt_Ptrs[0x4001], txt_Ptrs[0x4002]);
    d2_SetForm(win, 3);
    k = io_PrintText((char *)0x4003);
    btnPay = d2_GetWinObj(win, 3, k);
    btnLeave = d2_GetWinObj(win, 3, k + 1);
    d2_ClickedId = -1;
    do {
        AIMSG msg;

        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText("$C");
        io_PrintText((char *)0x3ffd, 0x3ffb, p->id, n);
        io_PrintText((char *)0x3ffe, amt[i]);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                if (sv_CheckMoney(ai_Chars[ai_PlayerId], amt[i]) == 1 && ai_CanSpyOn(rec) != 0) {
                    gm_SendMoneyDelta(((AICHAR *)ai_Chars[ai_PlayerId])->netId, -amt[i]);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x99;
                    msg.flags = 2;
                    msg.arg[3] = -1;
                    msg.arg[4] = 1;
                    msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                    msg.fromId = rec->id;
                    msg.time = ai_Time;
                    msg.arg[2] = p->netId;
                    msg.arg[1] = rec->id;
                    msg.arg[5] = i;
                    if (p->bld2 != 0)
                        msg.arg[0] = p->bld2->id;
                    else if (p->bld != 0)
                        msg.arg[0] = p->bld->id;
                    else {
                        int pick;

                        mask.LowPart = 0x7b4c4a0;
                        mask.HighPart = 0;
                        pick = -1;
                        ai_PickActionBuilding((struct AIPER *)ai_Chars[ai_PlayerId], 0, mask.QuadPart, 0, 100.0f, &pick);
                        msg.arg[0] = pick;
                    }
                    id = cm_SendLongMessage((char *)&msg);
                    while (id != -1 && cm_GetSlotState(id) == 0)
                        sv_RunFrame(0, 0x20007);
                    snd_PlayVoice(-7, 0, "KOMMENTARE", 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == btnLeave) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_CabinetSpyByMoney, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}

void txt_Format(char *out, int id0, ...); int sv_MessageBox();

/* 0x581db0, the cabinet's window on a character for the building rec, message kind 4 (the name is a reasoned guess:
   the form "Locations\Domizil\kabinett" without the slider of the kabinett_spy windows): the target is picked in
   the amount menu (AMOUNTCFG flags 1, mode 0x400, kind 6, no ids left out; title txt 0x402c); a target he_NoEvent9bFor
   refuses gets the message box of the text 0x4030 (the target) and return, none -> return. The price is 2% of
   the worth of the player and the target together (gm_GetWorth) plus 0x10 in money; the texts 0x402e / 0x402b
   (the price, the target) and the two buttons from the text 0x402f on (pay / leave). Pay: when the player has both
   (sv_CheckGoodAmountB, sv_CheckMoney 0x10) and the building may be spied on (ai_CanSpyOn), the price is added to
   the player record's spyPaid and moved away (cm_SendMoveGoodsUnchecked), 0x10 taken off (gm_SendMoneyDelta), the
   message 0x99 goes to the building's owner (flags 2, the game time, the target's netId, 4, the target's own
   building / building, else ai_PickActionBuilding's pick), its slot is waited for, the voice "KOMMENTARE" plays
   (variation -1) and the window leaves; the right button or the second button leaves too
   @deep Rewritten from the whole original 0x581db0..0x5821c7 (AMOUNTCFG, AICHAR target, the
   he_NoEvent9bFor refusal with txt_Format 0x4030 into a 0x400 text buffer, the price (worth + target's
   worth) * 0.02 plus 0x10 money, AIMSG kind 4, LARGE_INTEGER mask, the block-scoped pick that shares its
   slot with the spilled slot handle as in the original's frame 0x558); 290 vs 290 instructions, frame
   equal, no branch-target difference. Left: the scheduling of the message stores after the memset: the
   original hoists p->netId (mov edx,[esi+4]) and the time words before the type / flags stores and places
   add esp,0x18 earlier (0x582049, 22 ins), and loads bld2 before zeroing ecx for arg[5] (0x5820af).
   Sixteen orders of the ten stores tried (the best kept: arg[5] last, 22 ins). Match-deep 2026-11-10: msg block-scoped in the do loop took it to shape, 4 ins: only the p->netId store goes after the constant arg stores where the original stores it before them (moving the arg[2] line to any earlier place: draft). */
void ai_CabinetIntrigueWindow(AIBLD *rec)
{
    AMOUNTCFG q;
    char text[0x400];
    AICHAR *p;
    int worth;
    int price;
    int win;
    int btnPay;
    int btnLeave;
    int k;
    int id;
    LARGE_INTEGER mask;

    worth = gm_GetWorth(ai_PlayerId);
    memset(&q, 0, sizeof(q));
    memset(q.ids, 0xff, sizeof(q.ids));
    q.flags = 1;
    q.mode = 0x400;
    q.kind = 6;
    p = (AICHAR *)ai_AmountMenu(&q, txt_Ptrs[0x402c], 0, 0);
    if (!he_NoEvent9bFor(p)) {
        txt_Format(text, 0x4030, p->id);
        sv_MessageBox(text, 0);
        return;
    }
    if (!p)
        return;
    price = (int)((worth + gm_GetWorth(p->id)) * 0.02f);
    win = d2_OpenForm(0, 0, "Locations\\Domizil\\kabinett");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText((char *)0x402e, 0x402b, price, p->id);
    d2_SetForm(win, 2);
    k = io_PrintText((char *)0x402f);
    btnPay = d2_GetWinObj(win, 2, k);
    btnLeave = d2_GetWinObj(win, 2, k + 1);
    d2_ClickedId = -1;
    do {
        AIMSG msg;

        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) != 0 &&
                    sv_CheckMoney(ai_Chars[ai_PlayerId], 0x10) == 1 && ai_CanSpyOn(rec) != 0) {
                    AIFIN *fin = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);

                    if (fin != 0)
                        fin->spyPaid += price;
                    cm_SendMoveGoodsUnchecked(-1, ((AICHAR *)ai_Chars[ai_PlayerId])->netId, ai_OwnPlayer, price);
                    gm_SendMoneyDelta(((AICHAR *)ai_Chars[ai_PlayerId])->netId, -0x10);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x99;
                    msg.flags = 2;
                    msg.arg[3] = -1;
                    msg.arg[4] = 4;
                    msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                    msg.fromId = rec->id;
                    msg.time = ai_Time;
                    msg.arg[2] = p->netId;
                    msg.arg[1] = rec->id;
                    msg.arg[5] = 0;
                    if (p->bld2 != 0)
                        msg.arg[0] = p->bld2->id;
                    else if (p->bld != 0)
                        msg.arg[0] = p->bld->id;
                    else {
                        int pick;

                        mask.LowPart = 0x7b4c4a0;
                        mask.HighPart = 0;
                        pick = -1;
                        ai_PickActionBuilding((struct AIPER *)ai_Chars[ai_PlayerId], 0, mask.QuadPart, 0, 100.0f, &pick);
                        msg.arg[0] = pick;
                    }
                    id = cm_SendLongMessage((char *)&msg);
                    while (id != -1 && cm_GetSlotState(id) == 0)
                        sv_RunFrame(0, 0x20007);
                    snd_PlayVoice(-7, 0, "KOMMENTARE", -1);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == btnLeave) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_CabinetIntrigueWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}

void sv_ReleaseNamePlates(void); int sv_RegisterNamePlate(); void ai_StorageWindow();
void ai_AlchemistResearchWindow(); void ai_AlchemistWindow();

/* 0x5821d0, the contacts of the alchemist's laboratory (the room kind 0x24b of the enter-building driver
   ai_EnterBuilding): rec = the building, room = the room object. The name plates are released once, then each frame
   (pump 0x67fff, the right button leaves): with the building permission 0x200 the plates "contact_REZEPTBUCH"
   (txt 0x404e, 0x16) and "contact_PRODUKTION_ALCHIMIST" (txt 0x1f76, 0x13), with 0x400 "contact_CONTRACT"
   (txt 0x4038, 0xe), "contact_LAGER" (0xe), "contact_TRANSPORT" (0x15) and "contact_ALCHIMISTENVERTRAG"
   (txt 0x4037, 0x16) are registered; a click on the transport opens ai_OpenTransport, on the store
   ai_StorageWindow (the building's object list); any other click in a locked room opens ai_UnlockRoomWindow, else
   the production (when ahm_ResearchState answers 0, else its message txt 0x41ac + the answer), the recipe book
   (ai_AlchemistResearchWindow), the alchemist contract (ai_AlchemistWindow) or the contract (ai_AlchemistCandidates) */
void ai_AlchemistLabContact(AIBLD *rec, AIOBJ *room)
{
    int contract;
    int storage;
    int transport;
    int alchemistContract;
    int production;
    int recipeBook;
    int r;
    int a;
    int b;

    contract = 0;
    storage = 0;
    transport = 0;
    production = 0;
    alchemistContract = 0;
    recipeBook = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_AlchemistLabContact, 0x67fff) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (ai_BuildingPerms & 0x200) {
            recipeBook = sv_RegisterNamePlate("contact_REZEPTBUCH", txt_Ptrs[0x404e], 0x16);
            production = sv_RegisterNamePlate("contact_PRODUKTION_ALCHIMIST", txt_Ptrs[0x1f76], 0x13);
        }
        if (ai_BuildingPerms & 0x400) {
            contract = sv_RegisterNamePlate("contact_CONTRACT", txt_Ptrs[0x4038], 0xe);
            storage = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            alchemistContract = sv_RegisterNamePlate("contact_ALCHIMISTENVERTRAG", txt_Ptrs[0x4037], 0x16);
        }
        if (ai_ClickedObject == 0)
            continue;
        if (ai_ClickedObject == transport)
            ai_OpenTransport(rec);
        else if (ai_ClickedObject == storage)
            ai_StorageWindow(rec, rec->objList);
        else if (room->locked != 0)
            ai_UnlockRoomWindow(rec, room);
        else if (ai_ClickedObject == production) {
            r = ahm_ResearchState(rec, &a, &b, 0);
            if (r != 0)
                sv_MessageBox(txt_Ptrs[0x41ac + r], 0x100);
            else
                ai_ProductionWindow(rec);
        } else if (ai_ClickedObject == recipeBook)
            ai_AlchemistResearchWindow(rec);
        else if (ai_ClickedObject == alchemistContract)
            ai_AlchemistWindow(rec);
        else if (ai_ClickedObject == contract)
            ai_AlchemistCandidates(rec);
    }
}

void cm_BeginFieldBlock(char *o, int id); int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldDeltas(void);

/* 0x5823b0, the window that unlocks a room of the building rec (rec unused): the form "Locations\Domizil\kabinett"
   with the texts 0x407d / 0x4171 (the room's unlockPrice) and the two buttons from the text 0x407e on (pay /
   leave). Pay: when the player has the money (sv_CheckMoney), it is taken off (gm_SendMoneyDelta), the room's
   locked and unlockPrice are set to 0 through a field block (cm_BeginFieldBlock / cm_AddFieldDelta, sent by
   cm_SendFieldDeltas), its slot is waited for and the window leaves; the right button or the second button leaves
   too. At the end the view is closed and the camera follows again */
void ai_UnlockRoomWindow(AIBLD *rec, AIOBJ *room)
{
    int win;
    int btnPay;
    int btnLeave;
    int price;
    int k;
    int zero;

    price = room->unlockPrice;
    win = d2_OpenForm(0, 0, "Locations\\Domizil\\kabinett");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText((char *)0x407d, 0x4171, price);
    d2_SetForm(win, 2);
    k = io_PrintText((char *)0x407e);
    btnPay = d2_GetWinObj(win, 2, k);
    btnLeave = d2_GetWinObj(win, 2, k + 1);
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                if (sv_CheckMoney(ai_Chars[ai_PlayerId], price) == 1) {
                    zero = 0;
                    gm_SendMoneyDelta(((AICHAR *)ai_Chars[ai_PlayerId])->netId, -price);
                    cm_BeginFieldBlock((char *)room, room->objId);
                    cm_AddFieldDelta(4, 1, 0x1c, &zero);
                    cm_AddFieldDelta(4, 1, 0x20, &zero);
                    k = ((int (*)(void))cm_SendFieldDeltas)();  /* cm.c defines it void; the original's callers use eax: the slot of the block */
                    while (k != -1 && cm_GetSlotState(k) == 0)
                        sv_RunFrame(0, 0x20007);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == btnLeave) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_UnlockRoomWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x582560, the contacts of the cannon room (the room kind 0x256 of the enter-building driver ai_EnterBuilding):
   rec = the building, room = the room object. The name plates are released once, then each frame (pump 0x67fff,
   the right button leaves): with the building permission 0x400 the plate "contact_KANONE" (txt 0x405d, 0x16) is
   registered; any click in a locked room opens ai_UnlockRoomWindow, a click on the cannon the cannon scene */
void ai_CannonContact(AIBLD *rec, AIOBJ *room)
{
    int cannon;

    cannon = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_CannonContact, 0x67fff) != 0) {
        if (ai_BuildingPerms & 0x400)
            cannon = sv_RegisterNamePlate("contact_KANONE", txt_Ptrs[0x405d], 0x16);
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (ai_ClickedObject == 0)
            continue;
        if (room->locked != 0)
            ai_UnlockRoomWindow(rec, room);
        else if (ai_ClickedObject == cannon)
            ai_CannonScene((char *)rec);
    }
}
/* 0x582600, the contacts of the cabinet (kabinett, the room kind 0x266 of the enter-building driver ai_EnterBuilding):
   rec = the building, room = the room object. The name plates are released once, then each frame (pump 0x67fff,
   the right button leaves): with the building permission 0x400 the plates "contact_VERGIFTEN" (txt 0x3ffa),
   "contact_SPIONE_AUSSCHALTEN" (txt 0x401b), "contact_SCHLEICHENDE_LAEHMUNG" (txt 0x402a) and
   "contact_BEWEISE_FAELSCHEN" (txt 0x400b, all 0x16) are registered; any click in a locked room opens
   ai_UnlockRoomWindow, else the clicked plate opens ai_CabinetSpyByMoney / ai_CabinetSpyWindow /
   ai_CabinetIntrigueWindow / ai_CabinetSpyOnCharacter */
void ai_CabinetContact(AIBLD *rec, AIOBJ *room)
{
    int poison;
    int removeSpies;
    int paralysis;
    int forgeEvidence;

    poison = 0;
    removeSpies = 0;
    paralysis = 0;
    forgeEvidence = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_CabinetContact, 0x67fff) != 0) {
        if (ai_BuildingPerms & 0x400) {
            poison = sv_RegisterNamePlate("contact_VERGIFTEN", txt_Ptrs[0x3ffa], 0x16);
            removeSpies = sv_RegisterNamePlate("contact_SPIONE_AUSSCHALTEN", txt_Ptrs[0x401b], 0x16);
            paralysis = sv_RegisterNamePlate("contact_SCHLEICHENDE_LAEHMUNG", txt_Ptrs[0x402a], 0x16);
            forgeEvidence = sv_RegisterNamePlate("contact_BEWEISE_FAELSCHEN", txt_Ptrs[0x400b], 0x16);
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (ai_ClickedObject == 0)
            continue;
        if (room->locked != 0)
            ai_UnlockRoomWindow(rec, room);
        else if (ai_ClickedObject == poison)
            ai_CabinetSpyByMoney(rec);
        else if (ai_ClickedObject == removeSpies)
            ai_CabinetSpyWindow(rec);
        else if (ai_ClickedObject == paralysis)
            ai_CabinetIntrigueWindow(rec);
        else if (ai_ClickedObject == forgeEvidence)
            ai_CabinetSpyOnCharacter(rec);
    }
}
/* the four grave sizes of the graveyard windows (ai_GraveSizeButtons, ai_GraveSizeButtonsAt; ai_GraveMoveConfirm names the grave by
   its size; the sizes 1 .. 3 are open while GVYARD.open[i] is set); only this unit reads them, they stay out of
   sv_t.h: declared above ai_TutorialInitChapter2 they flip its register tie-break (byte -> shape) */
const int ai_GravePics[4] = { 0x2d5, 0x2d8, 0x2dc, 0x2e0 };      /* 0x5eecb0, the buttons' pictures */
const int ai_GravePicsAlt[4] = { 0x7da, 0x7db, 0x7dc, 0x7dd };   /* 0x5eecc0, the pictures ai_GraveList uses by the button */
const int ai_GraveNames[4] = { 0xd94, 0xd9a, 0xda2, 0xdaa };     /* 0x5eecd0, the texts of the sizes' names */
const int ai_GraveGoods[4] = { 0x1f6, 0x1f9, 0x1fd, 0x201 };     /* 0x5eece0, the ai_Goods index whose record the button keeps */

/* 0x582730, the four grave size buttons of the graveyard contact window: rec = the graveyard building (its yard),
   w = the window list (0 the window, 4 the parent, 8 .. 0xb the buttons, 0xc the selected size). Section 6 gets
   "$C" and the four picture buttons (ai_GravePics, 0x6c apart), interactive, style 3, each keeping its goods
   record (ai_GraveGoods); the sizes 1 .. 3 are enabled while the yard has them open. The selected size stays
   highlighted when it is open, else the first button is highlighted and the selection falls back to 0; the
   selected button is checked */
void ai_GraveSizeButtons(AIBLD *rec, int *w)
{
    GVYARD *yard;
    int i;

    yard = rec->yard;
    d2_SetForm(w[0], 6);
    io_PrintText("$C");
    for (i = 0; i < 4; i++) {
        w[8 + i] = d2_WinOpenPic(i * 0x6c + 6, 0, ai_GravePics[i], w[4]);
        d2_Objs[w[8 + i]].interactive = 1;
        d2_Objs[w[8 + i]].style = 3;
        d2_Objs[w[8 + i]].user = ai_Goods + ai_GraveGoods[i] * 0x41;
    }
    d2_SetObjEnabled(w[9], yard->open[1]);
    d2_SetObjEnabled(w[10], yard->open[2]);
    d2_SetObjEnabled(w[11], yard->open[3]);
    if (w[12] == 3 && yard->open[3] == 1)
        d2_Objs[w[11]].highlighted = 1;
    else if (w[12] == 2 && yard->open[2] == 1)
        d2_Objs[w[10]].highlighted = 1;
    else if (w[12] == 1 && yard->open[1] == 1)
        d2_Objs[w[9]].highlighted = 1;
    else {
        d2_Objs[w[8]].highlighted = 1;
        w[12] = 0;
    }
    d2_Objs[w[w[12] + 8]].checked = 1;
}

int sv_CreateScrollButtons(); unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l); int gv_EntryIndex(char *r, int *e);

/* a row of the graveyard contact window's grave list (w + 0xd, 0x10 rows of 4 dwords; ai_GraveList fills them) */
typedef struct GVROW {
    int btn;                    /* 0x00, its button object, -1: an unused row */
    GVENTRY *entry;             /* 0x04, the grave entry, 0: unused */
    int size;                   /* 0x08, the entry's size (list), 0xff: unused */
    unsigned char index;        /* 0x0c, the entry's index in its list (gv_EntryIndex), 0xff: unused */
} GVROW;

/* a dead person shown by the graveyard contact window's detail (w + 0x4f, 8 items of 3 dwords; ai_GraveDetail) */
typedef struct GVITEM {
    int btn;                    /* 0x00, its picture object, -1: none */
    GVGRAVE *grave;             /* 0x04, the dead person, 0: an unused item */
    int id;                     /* 0x08, the grave's id, -1: unused */
} GVITEM;

typedef struct GVWIN {                  /* 0x1a0 bytes, the graveyard contact window's list (ai_GraveWindow's w[0x68]) */
    int head[0xc];                      /* 0x00, its windows: 0 the window, 2 the list's window, 3 the detail list's window,
                                           5 / 6 their fonts, 8 .. 11 the size buttons (ai_GraveSizeButtons) */
    int size;                           /* 0x30 (w[0xc]), the selected grave size */
    GVROW rows[0x10];                   /* 0x34 (w[0xd]), the grave list's rows (ai_GraveList) */
    int sel;                            /* 0x134 (w[0x4d]), the selected row */
    int slider;                         /* 0x138 (w[0x4e]), the detail's slider */
    GVITEM items[8];                    /* 0x13c (w[0x4f]), the detail's dead persons (ai_GraveDetail) */
    int item;                           /* 0x19c (w[0x67]), the item last clicked */
} GVWIN;

/* the window record of the graveyard detail windows (ai_GraveDetailOpen, ai_RentGraveDetail, ai_SeancePick: int[0x23]) */
typedef struct GVDETAILWIN {
    int win;                    /* 0x00, the form */
    int sections[6];            /* 0x04, the d2_LastWindow of its sections */
    GVENTRY *entry;             /* 0x1c, the grave entry shown */
    int size;                   /* 0x20, its size (list) */
    unsigned char index;        /* 0x24, its index in that list (gv_EntryIndex) */
    char pad25[3];
    GVITEM items[8];            /* 0x28, the dead persons of the grave */
    int selected;               /* 0x88, the item clicked (ai_SeancePick) */
} GVDETAILWIN;

/* 0x5828c0, the grave list of the graveyard contact window: rec = the graveyard building, w = the window list (0 the
   window, 2 the list's window, 5 its font, 0xc the selected grave size, 0xd the rows (GVROW), 0x4d the selected
   row), sel = the size just clicked (-1: none; the list keeps its scroll position only when it is the selected
   size). Sections 2 and 4 get "$C"; the scroll buttons (sv_CreateScrollButtons, step 0x35); each entry of the size
   (gv_FindEntry) gets a row: the entry, its size and index, a button 0x35 apart with the size's picture
   (ai_GravePics when the grave is free, else ai_GravePicsAlt), interactive, style 3, and the marker 0x7de when an
   event of type 0x8d is pending for it (he_HasGraveWork). A selection past the rows falls back to row 0; it is
   highlighted and checked; the unused rows are cleared; the scroll position is restored, the step set and the
   window scrolled */
void ai_GraveList(AIBLD *rec, int *w, int sel)
{
    char a[4];
    char b[4];
    GVENTRY *e;
    GVROW *row;
    int saved;
    int n;
    int i;

    saved = 0;
    if (sel == w[0xc])
        saved = d2_Wins[w[2]].scrollY;
    d2_SetForm(w[0], 2);
    io_PrintText("$C");
    d2_SetForm(w[0], 4);
    io_PrintText("$C");
    sv_CreateScrollButtons(0, 0, w[5], w[2], 0x35, a, b, &sel);
    e = (GVENTRY *)gv_FindEntry(0, (unsigned char *)rec, 0, 0, w[0xc]);
    n = 0;
    if (e) {
        row = ((GVWIN *)w)->rows;
        do {
            row->entry = e;
            row->size = e->size;
            row->index = gv_EntryIndex((char *)rec->yard, (int *)e);
            if (e->id == -1)
                row->btn = d2_WinOpenPic(1, n * 0x35 + 2, ai_GravePics[w[0xc]], w[2]);
            else
                row->btn = d2_WinOpenPic(1, n * 0x35 + 2, ai_GravePicsAlt[w[0xc]], w[2]);
            if (he_HasGraveWork(rec, e->size, row->index) != 0)
                d2_WinOpenPic(0xf, n * 0x35 + 9, 0x7de, w[2]);
            d2_Objs[row->btn].interactive = 1;
            d2_Objs[row->btn].style = 3;
            e = (GVENTRY *)gv_FindEntry(0, (unsigned char *)rec, (unsigned)e, 0, w[0xc]);
            n++;
            row++;
        } while (e);
    }
    if (w[0x4d] >= n)
        w[0x4d] = 0;
    d2_Objs[w[0x4d]].highlighted = 1;
    d2_Objs[((GVWIN *)w)->rows[w[0x4d]].btn].checked = 1;
    if (n < 0x10) {
        row = ((GVWIN *)w)->rows;
        for (i = n; i < 0x10; i++) {
            row[i].entry = 0;
            row[i].size = 0xff;
            row[i].index = 0xff;
            row[i].btn = -1;
        }
    }
    d2_Wins[w[2]].scrollY = saved;
    d2_Wins[w[2]].visible = 0x35;
    d2_ScrollWindow(w[2]);
}

void ai_GraveDeadName(GVGRAVE *e, char *out);


/* 0x582b40, the detail of the selected grave in the graveyard contact window: rec = the graveyard building, w = the
   window list (0 the window, 3 the detail list's window, 6 its font, 0x4d the selected row (GVROW at w + 0xd),
   0x4e the slider, 0x4f the items (GVITEM)), clicked = the row just clicked (the list keeps its scroll position
   only when it is the selected row), flag = 1: the window has the slider. The used graves of the entry
   (gv_EntryCounts[size] of them) become the items, the rest is cleared; sections 3 / 5 / 7 get "$C" and the
   scroll buttons (one window height per item). Per item, one window height apart: the frame picture 0x6c7
   centred with the dead person's icon on it (interactive unless flag), the name and second line (ai_GraveDeadName /
   ai_GraveDeadDescription) centred in font 0x54, and the five traits as "%s:" labels (txt 0x16b7 ..) with a value bar each.
   Section 7: a free grave shows its size's name (txt 0x4100), else the holder (the character's name, or the
   item of that id), the year "AD %i" (until + 1400) and the state in percent (txt 0x4101 .. 0x4103); with flag 1
   a slider (0xbf, txt 0x42f1) 0xb4 wide is added at the bottom. The scroll position is restored
   @stuck draft 30 -> shape 22 ins. Kept: the M9 labels as the objects ai_FmtLabel / ai_CharNameFmt; the
   item fill as the plain index loop ((GVWIN *)w)->items[i].grave / id / btn (gives the original's lea
   [n*3+0x51] form; the int-pointer walk is gone); the collect loop as items[n].f with n++ (the item++
   walk: 24, a count-free bound in the for: 34). Left: the constant 0 web: the original zeroes ebp at
   entry, re-zeroes it only on the collect loop's path (xor ebp,ebp before the join) and later pushes
   immediates / test; ours keeps const#0 in ebp after the main loop (push ebp x2 for
   sv_CreateScrollButtons, cmp esi,ebp / mov [esp+0x10],ebp, cmp eax,ebp, an extra xor ebp,ebp after the
   main loop, the n spill before the collect loop moving the jle target). Tried: 6 fill store orders (all
   worse), saved = 0 in an else (25), n = 0 in the for-init / while fill / init order (= one code), flip.py
   --want two push 0 (25 mutants, only meaning-changing ones closer: the fill's grave = 0 constant is the
   reference that decides). Open: M10 100.0f vs ai_FloatHundred (no view of full_rdata_5ebb40 in sv_t).
   Next: c2trace.py on the const#0 pieces (ids 1/2/3 of 6 webs) to see which web takes ebp. (tiebreak: not
   measured (dcp headers sample ai_GraveDetail)) prior: reconstructed in full from the original (all 434
   instructions read: the item collection, the per-item pictures / texts / trait bars, the detail texts,
   the slider); what is left is allocation: ours keeps the constant 0 in ebp after the first loop (push ebp
   / push ebp for sv_CreateScrollButtons' 0, 0 at 0x582c76, cmp esi,ebp / mov [esp+0x10],ebp / cmp eax,ebp
   at 0x582c8d, the re-zero xor ebp,ebp at 0x582efa, cmp eax,ebp at 0x582f72, cmp esi,ebp at 0x582f97)
   where the original uses immediates and test; and ours spills n to [esp+0x1c] before the first loop
   (0x582b5f) where the original stores it once after, which moves the count<=0 jle target by that one
   store (same statement, both jump to the n < 8 test). Tried: the count hoisted into a local (kept, it
   gives the original's dec-counter loop), the fill loop through an int pointer (kept, the original's
   [n*3+0x51] index form), the items pointer... */
void ai_GraveDetail(AIBLD *rec, int *w, int clicked, int flag)
{
    int i;
    GVENTRY *e;
    int saved;
    int n;
    int count;
    int up;
    int down;
    int bar;
    GVITEM *items;
    GVITEM *item;
    short *c;
    int x;
    int y;
    int k;

    saved = 0;
    n = 0;
    e = ((GVWIN *)w)->rows[w[0x4d]].entry;
    items = ((GVWIN *)w)->items;
    count = gv_EntryCounts[e->size];
    for (i = 0; i < count; i++) {
        if (e->grave[i].id != -1) {
            items[n].grave = &e->grave[i];
            items[n].id = e->grave[i].id;
            items[n].btn = -1;
            n++;
        }
    }
    for (i = n; i < 8; i++) {
        ((GVWIN *)w)->items[i].grave = 0;
        ((GVWIN *)w)->items[i].id = -1;
        ((GVWIN *)w)->items[i].btn = -1;
    }
    w[0x4e] = -1;
    if (clicked == w[0x4d])
        saved = d2_Wins[w[3]].scrollY;
    d2_SetForm(w[0], 3);
    io_PrintText("$C");
    d2_SetForm(w[0], 5);
    io_PrintText("$C");
    d2_SetForm(w[0], 7);
    io_PrintText("$C");
    sv_CreateScrollButtons(0, 0, w[6], w[3], d2_Wins[w[3]].h, &up, &down, &bar);
    d2_SetForm(w[0], 3);
    for (i = 0; i < n; i++) {
        char buf[0x100];
        char line2[0x100];

        item = &items[i];
        x = (d2_Wins[w[3]].w - d2_Pics[0x6c7].w) / 2;
        item->btn = d2_WinOpenPic(x, d2_Wins[w[3]].h * i + 2, 0x6c7, w[3]);
        item->btn = d2_WinOpenPic(x + 2, d2_Wins[w[3]].h * i + 4, item->grave->icon, w[3]);
        d2_Objs[item->btn].interactive = flag == 0;
        d2_Objs[item->btn].style = 3;
        ai_GraveDeadName(item->grave, buf);
        ai_GraveDeadDescription((char *)item->grave, line2);
        y = d2_Wins[w[3]].h * i + 0x38;
        k = d2_CreateWinText(0, y, buf, w[3]);
        d2_Objs[k].centred = 1;
        d2_Objs[k].w = d2_Wins[w[3]].w;
        d2_SetObjFont(k, 0x54);
        y += 0xd;
        k = d2_CreateWinText(0, y, line2, w[3]);
        d2_Objs[k].centred = 1;
        d2_Objs[k].w = d2_Wins[w[3]].w;
        d2_SetObjFont(k, 0x54);
        y += 0x12;
        for (k = 0; k < 5; k++) {
            txt_Format(buf, ai_FmtLabel, 0x16b7 + k);
            x = d2_CreateWinText(0x28, y, buf, w[3]);
            d2_Objs[x].w = 300;
            d2_SetObjFont(x, 0x54);
            sv_ValueBar(0xa0, y, item->grave->traits[k], 0x4df);
            y += 0xd;
        }
    }
    d2_Wins[w[3]].scrollY = saved;
    d2_SetForm(w[0], 7);
    io_PrintText("$C");
    if (e->id == -1) {
        char buf[0x100];

        buf[0] = 0;
        txt_Format(buf, 0x4100, ai_GraveNames[e->size]);
        io_PrintText("$Z%s", buf);
    } else {
        char buf[0x100];

        c = gm_FindChar(e->id);
        buf[0] = 0;
        if (c) {
            txt_Format(buf, ai_CharNameFmt, (unsigned short)*c);
        } else {
            for (i = 0; i < n; i++) {
                if (items[i].grave->id == e->id) {
                    ai_GraveDeadName(items[i].grave, buf);
                    break;
                }
            }
        }
        io_PrintText("$7<$R%s$0<$4>$L%s$A$0>", 0x4101, buf);
        txt_Format(buf, "AD %i", e->until + 0x578);
        io_PrintText("$7<$R%s$0<$4>$L%s$A$0>", 0x4102, buf);
        txt_Format(buf, "%i %%", (int)(100.0f * (e->b0c / 255.0f)));
        io_PrintText("$7<$R%s$0<$4>$L%s$A$0>", 0x4103, buf);
        if (flag == 1) {
            w[0x4e] = d2_WinCreateSlider((d2_Wins[d2_LastWindow].w - 0xb4) / 2, d2_Wins[d2_LastWindow].h - 0x21, 0xbf,
                                         txt_Ptrs[0x42f1], d2_LastWindow);
            d2_SetObjWidth(w[0x4e], 0xb4);
        }
    }
    d2_Wins[w[3]].scrollY = saved;
    d2_ScrollWindow(w[3]);
}
/* 0x5830f0, the name line of a dead person in a grave (ai_GraveDetail, ai_RentGraveDetail): his office (v2c,
   1 .. 9) as a text id on top of the male 0x253 / female 0x27b base of the office titles by his sex, then his
   first and family name ("%s %s %s" 0x623594), or just the two names ("%s %s" 0x60c7b8) */
void ai_GraveDeadName(GVGRAVE *e, char *out)
{
    if (e->v2c) {
        if (e->sex == 0)
            txt_Format(out, (char *)(unsigned)"%s %s %s", e->v2c + 0x253, e->first, e->family);
        else
            txt_Format(out, (char *)(unsigned)"%s %s %s", e->v2c + 0x27b, e->first, e->family);
        return;
    }
    txt_Format(out, (char *)(unsigned)"%s %s", e->first, e->family);
}
/* 0x583150, the second line of a dead person in a grave (ai_GraveDetail): the two texts the record points to at
   +0x30 / +0x34 as text ids (their male v30 + 0x114 / nameId + 0x130 or female + 0x11e / + 0x18f forms by g->sex;
   txt_Format's "%s" takes a text id), both as "%s, %s", one alone, or an empty line */
void ai_GraveDeadDescription(GVGRAVE *g, char *out)
{
    if (g->v30 && g->nameId) {
        if (g->sex == 0)
            txt_Format(out, "%s, %s", g->v30 + 0x114, g->nameId + 0x130);
        else
            txt_Format(out, "%s, %s", g->v30 + 0x11e, g->nameId + 0x18f);
    } else if (g->v30) {
        if (g->sex == 0)
            txt_Format(out, "%s", g->v30 + 0x114);
        else
            txt_Format(out, "%s", g->v30 + 0x11e);
    } else if (g->nameId) {
        if (g->sex == 0)
            txt_Format(out, "%s", g->nameId + 0x130);
        else
            txt_Format(out, "%s", g->nameId + 0x18f);
    } else {
        *out = 0;
    }
}

/* the grave the graveyard window hands out (ai_GraveWindow's out and its callback's record) */
typedef struct GVPICK {
    GVENTRY *entry;             /* 0x00, the selected grave entry */
    int size;                   /* 0x04, its size (list) */
    unsigned char index;        /* 0x08, its index in that list (gv_EntryIndex) */
    GVGRAVE *grave;             /* 0x0c, the dead person picked in it, 0: the grave itself (the slider) */
    int id;                     /* 0x10, that dead person's id, -1: none */
} GVPICK;

/* 0x583240, the graveyard window "Locations\Friedhof\Graves": out = the picked grave (0: none wanted), rec = the
   graveyard building, title = the window's title, flag = 0: the dead persons in the grave can be picked, 1: the
   grave itself through the detail's slider, cb = called (rec, the pick) with the window disabled, 0: a pick ends
   the window. The window list w keeps the window and its sections 1 .. 7 (each one's d2_LastWindow); it starts at
   size 0, row 0 and draws the size buttons, the grave list and the detail. Each frame (pump 0x677c7): the right
   button ends it; a click on another size button selects it (row 0, all redrawn), on another row of the list
   selects it (list and detail redrawn), on a dead person (flag 0) or the slider (flag 1) fills out and calls cb
   (or ends the window when out is wanted and there is no cb); 150 ms after the start everything is redrawn every
   frame
   @deep reconstructed in full from the original (every instruction read: the setup with the seven section
   windows, the frame loop with the size-button, row, dead-person and slider clicks, the out / callback
   picks, the three redraw bits); the statements and the branch structure match (each hunk's jumps go to
   the same statements). Left is allocation: the original keeps out in edi, w[0x4d] in ebx and ebp as the
   zero / loop pointer, rec on the stack ([esp+0x1f4] at every use); ours keeps rec in ebx, w[0x4d] in ebp
   and out on the stack, so every out store, gv_EntryIndex(rec->yard) push and redraw call differs by
   register (the regs / code hunks 0x583247 .. 0x583376, 0x5834ad .. 0x5835f1, 0x583644 .. 0x5836a7); with
   ebp free the original strength-reduces the size-button loop to a walking pointer (lea ebp,[esp+0x6c];
   add ebp,4) where ours indexes [esp+eax*4+0x6c], keeps the redraw bits in memory across the row loop
   (ours in ecx), reloads d2_PointerObj after each call (ours at the loop top) and tail-merges the
   right-button sv_PopRequest store with the flag 1 one (ours: a reload of out sits at that join). Tried:
   the entry through a temporary vs re-read out->entry (identical) */
void ai_GraveWindow(GVPICK *out, AIBLD *rec, char *title, int flag, void (*cb)(AIBLD *rec, GVPICK *pick))
{
    int redraw;
    int j;
    int detailRow;
    unsigned next;
    int prevSize;
    GVPICK pick;
    GVPICK whole;
    int w[0x68];
    int handled;
    int i;
    int win;

    next = d3_Time;
    if (out) {
        out->entry = 0;
        out->grave = 0;
    }
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Graves");
    w[0] = win;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    w[1] = d2_LastWindow;
    io_PrintText("$C$Z%s", title);
    d2_SetForm(win, 2);
    w[2] = d2_LastWindow;
    d2_SetForm(win, 3);
    w[3] = d2_LastWindow;
    d2_SetForm(win, 6);
    w[4] = d2_LastWindow;
    d2_SetForm(win, 4);
    w[5] = d2_LastWindow;
    d2_SetForm(win, 5);
    w[6] = d2_LastWindow;
    d2_SetForm(win, 7);
    w[7] = d2_LastWindow;
    w[0xc] = 0;
    w[0x4d] = 0;
    w[0x67] = 0;
    ai_GraveSizeButtons(rec, w);
    ai_GraveList(rec, w, -1);
    ai_GraveDetail(rec, w, -1, flag);
    next += 0x96;
    d2_ClickedId = -1;
    do {
        redraw = 0;
        handled = 0;
        prevSize = w[0xc];
        detailRow = w[0x4d];
        if (d3_Time > next)
            redraw = 7;
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            for (i = 0; !handled && i < 4; i++) {
                if (d2_PointerObj == w[8 + i] && i != w[0xc]) {
                    redraw |= 7;
                    w[0xc] = i;
                    w[0x4d] = 0;
                    detailRow = -1;
                    handled = 1;
                }
            }
            for (i = 0; !handled && i < 0x10; i++) {
                if (d2_PointerObj == ((GVWIN *)w)->rows[i].btn && i != w[0x4d]) {
                    w[0x4d] = i;
                    redraw |= 3;
                    handled = 1;
                }
            }
            if (flag == 0) {
                for (j = 0; !handled && j < 8; j++) {
                    if (d2_PointerObj != ((GVWIN *)w)->items[j].btn)
                        continue;
                    w[0x67] = j;
                    handled = 1;
                    if (out) {
                        out->entry = ((GVWIN *)w)->rows[w[0x4d]].entry;
                        out->size = out->entry->size;
                        out->index = gv_EntryIndex((char *)rec->yard, (int *)out->entry);
                        out->grave = ((GVWIN *)w)->items[j].grave;
                        out->id = out->grave->id;
                    }
                    if (cb) {
                        pick.entry = ((GVWIN *)w)->rows[w[0x4d]].entry;
                        pick.size = pick.entry->size;
                        pick.index = gv_EntryIndex((char *)rec->yard, (int *)pick.entry);
                        pick.grave = ((GVWIN *)w)->items[j].grave;
                        pick.id = pick.grave->id;
                        d2_SetViewEnabled(w[0], 0);
                        cb(rec, &pick);
                        d2_SetViewEnabled(w[0], 1);
                    } else if (out) {
                        sv_PopRequest = 1;
                    }
                }
            } else if (flag == 1 && d2_PointerObj == w[0x4e]) {
                if (out) {
                    out->entry = ((GVWIN *)w)->rows[w[0x4d]].entry;
                    out->size = out->entry->size;
                    out->index = gv_EntryIndex((char *)rec->yard, (int *)out->entry);
                    out->grave = 0;
                    out->id = -1;
                }
                if (cb) {
                    whole.entry = ((GVWIN *)w)->rows[w[0x4d]].entry;
                    whole.size = whole.entry->size;
                    whole.index = gv_EntryIndex((char *)rec->yard, (int *)whole.entry);
                    whole.grave = 0;
                    whole.id = -1;
                    d2_SetViewEnabled(w[0], 0);
                    cb(rec, &whole);
                    d2_SetViewEnabled(w[0], 1);
                } else if (out) {
                    sv_PopRequest = 1;
                }
            }
        }
        if (redraw & 4)
            ai_GraveSizeButtons(rec, w);
        if (redraw & 2)
            ai_GraveList(rec, w, prevSize);
        if (redraw & 1)
            ai_GraveDetail(rec, w, detailRow, flag);
    } while (sv_RunFrame(ai_GraveWindow, 0x677c7) != 0);
    d2_CloseView(w[0]);
}
/* 0x583700, the detail of the grave in the rent-a-grave window (ai_GraveDetailOpen): w = its window list (0 the window,
   2 the detail list's window, 4 its font, 7 .. 9 the grave (entry, size, index as in GVPICK), 0xa the items
   (GVITEM)). As ai_GraveDetail without the selection: the used graves of the entry become the items, the rest
   is cleared; sections 2 / 3 / 5 get "$C" and the scroll buttons; per item, one window height apart, the frame
   picture 0x6c7 with the dead person's icon (interactive), the name and second line centred in font 0x54 and the
   five traits with their value bars. Section 5: the player's character as the holder, the year "AD %i" (until +
   1400) and the care in percent (txt 0x4101 .. 0x4103); the scroll position is kept
   @deep reconstructed in full from the original (all 343 instructions read; the skeleton's layout numbers
   replaced by the d2_Wins / d2_Pics / D2OBJ fields, the item loop as in ai_GraveDetail, the holder
   ai_CharsT[ai_PlayerId].id); from 0x583780 on everything is equal but order / regs hunks (0x583a4e order,
   esi->ebx at 0x583a6f / 0x583ae5 / 0x583b10). Left is the constant register in the first two loops: the
   original keeps -1 in ebp (cmp [edx],ebp / btn = ebp / id = ebp) and so spills the grave count to
   [esp+0x10] (dec in memory) and walks the item pointer from &w[0xc]; ours keeps 0 in ebp (cmp edx,ebp for
   count > 0, push ebp / push ebp for sv_CreateScrollButtons' 0, 0 at 0x58380a / 0x583821) and the count in
   ebp, n in ebx, the pointer from &w[0xa] (hunk 0x583711, 74 lines: the same statements in the same order,
   the three branch-target shifts are those instructions). Tried: the loop bound re-read each pass instead
   of the count local (worse, the count local stays) */
void ai_RentGraveDetail(int *w)
{
    GVENTRY *e;
    GVITEM *item;
    int count;
    int n;
    int i;
    int *p;
    int saved;
    int up;
    int down;
    int bar;
    char buf[0x100];
    char line2[0x100];
    int x;
    int y;
    int k;

    n = 0;
    e = (GVENTRY *)w[7];
    item = ((GVDETAILWIN *)w)->items;
    count = gv_EntryCounts[e->size];
    for (i = 0; i < count; i++) {
        if (e->grave[i].id != -1) {
            item->grave = &e->grave[i];
            item->id = e->grave[i].id;
            item->btn = -1;
            n++;
            item++;
        }
    }
    /* the unused items, walked as ints (the original's index form): p = &items[i].id */
    p = &w[n * 3 + 0xc];
    for (i = n; i < 8; i++) {
        p[-1] = 0;              /* grave */
        p[0] = -1;              /* id */
        p[-2] = -1;             /* btn */
        p += 3;
    }
    saved = d2_Wins[w[2]].scrollY;
    d2_SetForm(w[0], 2);
    io_PrintText("$C");
    d2_SetForm(w[0], 3);
    io_PrintText("$C");
    d2_SetForm(w[0], 5);
    io_PrintText("$C");
    sv_CreateScrollButtons(0, 0, w[4], w[2], d2_Wins[w[2]].h, &up, &down, &bar);
    d2_SetForm(w[0], 2);
    for (i = 0; i < n; i++) {
        item = &((GVDETAILWIN *)w)->items[i];
        x = (d2_Wins[w[2]].w - d2_Pics[0x6c7].w) / 2;
        item->btn = d2_WinOpenPic(x, d2_Wins[w[2]].h * i + 2, 0x6c7, w[2]);
        item->btn = d2_WinOpenPic(x + 2, d2_Wins[w[2]].h * i + 4, item->grave->icon, w[2]);
        d2_Objs[item->btn].interactive = 1;
        d2_Objs[item->btn].style = 3;
        ai_GraveDeadName(item->grave, buf);
        ai_GraveDeadDescription(item->grave, line2);
        y = d2_Wins[w[2]].h * i + 0x38;
        k = d2_CreateWinText(0, y, buf, w[2]);
        d2_Objs[k].centred = 1;
        d2_Objs[k].w = d2_Wins[w[2]].w;
        d2_SetObjFont(k, 0x54);
        y += 0xd;
        k = d2_CreateWinText(0, y, line2, w[2]);
        d2_Objs[k].centred = 1;
        d2_Objs[k].w = d2_Wins[w[2]].w;
        d2_SetObjFont(k, 0x54);
        y += 0x12;
        for (k = 0; k < 5; k++) {
            txt_Format(buf, "%s:", 0x16b7 + k);
            x = d2_CreateWinText(0x28, y, buf, w[2]);
            d2_Objs[x].w = 300;
            d2_SetObjFont(x, 0x54);
            sv_ValueBar(0xa0, y, item->grave->traits[k], 0x4df);
            y += 0xd;
        }
    }
    d2_Wins[w[2]].scrollY = saved;
    d2_SetForm(w[0], 5);
    io_PrintText("$C");
    txt_Format(buf, "%1N3", ai_CharsT[ai_PlayerId].id);
    io_PrintText("$7<$R%s$0<$4>$L%s$A$0>", 0x4101, buf);
    txt_Format(buf, "AD %i", e->until + 0x578);
    io_PrintText("$7<$R%s$0<$4>$L%s$A$0>", 0x4102, buf);
    txt_Format(buf, "%i %%", (int)(100.0f * (e->b0c / 255.0f)));
    io_PrintText("$7<$R%s$0<$4>$L%s$A$0>", 0x4103, buf);
}
/* 0x583b70, opens the graveyard detail window (209 B): w = the window list (7 the selected entry record), title
   = its title; 0 without a selected entry, else the form "Locations\Friedhof\RentAGrave2" (d2_OpenForm (0, 0),
   the pointer sprite off, centred, fitted), the title line "$C$Z%s" of the title, the five sections
   1 / 2 / 3 / 5 / 4 (d2_LastWindow kept in w[1] / w[2] / w[4] / w[5] / w[6]), then the detail fill
   ai_RentGraveDetail (w); 1 */

int ai_GraveDetailOpen(int *w, char *title)
{
    int win;

    if (w[7] == 0)
        return 0;
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\RentAGrave2");
    w[0] = win;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(w[0]);
    d2_SetForm(w[0], 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(w[0], 1);
    w[1] = d2_LastWindow;
    io_PrintText("$C$Z%s", title);
    d2_SetForm(w[0], 2);
    w[2] = d2_LastWindow;
    d2_SetForm(w[0], 3);
    w[4] = d2_LastWindow;
    d2_SetForm(w[0], 5);
    w[5] = d2_LastWindow;
    d2_SetForm(w[0], 4);
    w[6] = d2_LastWindow;
    io_PrintText("$C");
    ai_RentGraveDetail(w);
    return 1;
}
/* 0x583c50, the four grave size buttons into a given window section (the ai_GraveSizeButtons body with the
   window, section, button ids and selection passed in): rec = the graveyard building (its yard), btns = the four
   button ids out, selp = the selected size. The open sizes 1 .. 3 are enabled; the selected size stays
   highlighted when it is open, else the first button is highlighted and the selection falls back to 0; the
   selected button is checked */
void ai_GraveSizeButtonsAt(AIBLD *rec, int win, int section, int *btns, int *selp)
{
    GVYARD *yard;
    int i;

    yard = rec->yard;
    d2_SetForm(win, section);
    io_PrintText("$C");
    for (i = 0; i < 4; i++) {
        btns[i] = d2_WinOpenPic(i * 0x6c + 6, 0, ai_GravePics[i], d2_LastWindow);
        d2_Objs[btns[i]].interactive = 1;
        d2_Objs[btns[i]].style = 3;
        d2_Objs[btns[i]].user = ai_Goods + ai_GraveGoods[i] * 0x41;
    }
    d2_SetObjEnabled(btns[1], yard->open[1]);
    d2_SetObjEnabled(btns[2], yard->open[2]);
    d2_SetObjEnabled(btns[3], yard->open[3]);
    if (*selp == 3 && yard->open[3] == 1)
        d2_Objs[btns[3]].highlighted = 1;
    else if (*selp == 2 && yard->open[2] == 1)
        d2_Objs[btns[2]].highlighted = 1;
    else if (*selp == 1 && yard->open[1] == 1)
        d2_Objs[btns[1]].highlighted = 1;
    else {
        d2_Objs[btns[0]].highlighted = 1;
        *selp = 0;
    }
    d2_Objs[btns[*selp]].checked = 1;
}

int gv_OrderSetGraveHolder(); int gv_SizeOpen();

/* 0x583de0, the confirmation window of a grave release (272 B): rec / a / b are passed on to
   gv_OrderSetGraveHolder; the form "Locations\Friedhof\Friedhof" (d2_OpenForm (0, 0), the pointer sprite off,
   centred, fitted), the texts 0x410f / 0x410e and the two entries 0x4111 / +1 (d2_GetWinObj), d2_ClickedId
   cleared; the pump loop (this window, 0x677c7): a right button release (dm_L.rup) or the second entry sets
   sv_PopRequest, the first sets the grave holder (0, rec, a, b) and returns 1; the close of the window, the result */
int ai_GraveFreeConfirm(char *rec, int a, unsigned char b)
{
    int win;
    int b1;
    int b2;
    int v;
    int done;

    done = 0;
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x410f, 0x410e);
    d2_SetForm(win, 2);
    v = io_PrintText(0x4111);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    d2_ClickedId = -1;
    for (;;) {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == b1) {
                gv_OrderSetGraveHolder(0, rec, a, b);
                done = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == b2) {
                sv_PopRequest = 1;
            }
        }
        if (sv_RunFrame(ai_GraveFreeConfirm, 0x677c7) == 0) {
            d2_CloseView(win);
            return done;
        }
    }
}
/* 0x583ef0, enables the grave size buttons the yard has open from a minimum size on: rec = the graveyard
   building (its yard), from = the largest size that is too small, entry (the grave's entry, not read), btns = the four button ids, selp = the selected
   size. The sizes 0 .. from are disabled and unchecked, so are the sizes after it the yard has not open; the open
   ones are enabled. When the selected button is not clickable any more, the first open size becomes the selection
   and is checked; 1 when any size is open, else 0 */
int ai_GraveSizeEnable(AIBLD *rec, int from, unsigned char entry, int *btns, int *selp)
{
    int i;
    int first;
    int any;

    any = 0;
    first = -1;
    for (i = 0; i < from + 1; i++) {
        d2_SetObjEnabled(btns[i], 0);
        d2_Objs[btns[i]].checked = 0;
    }
    for (i = from + 1; i < 4; i++) {
        if (gv_SizeOpen((char *)rec->yard, i)) {
            any = 1;
            d2_SetObjEnabled(btns[i], 1);
            if (first == -1)
                first = i;
        } else {
            d2_SetObjEnabled(btns[i], 0);
            d2_Objs[btns[i]].checked = 0;
        }
    }
    if (d2_Objs[btns[*selp]].clickDisabled && first != -1) {
        *selp = first;
        d2_Objs[btns[first]].checked = 1;
    }
    return any;
}


/* 0x584010, confirms moving a grave to another size: rec = the graveyard building, from / entry = the grave's
   size and entry, to = the new size. The move costs 16000 unless the player owns the graveyard. The form
   "Locations\Friedhof\Friedhof" shows the question (txt 0x4115 with the building for the owner, else 0x4114 with
   the price) and the yes / no entries (txt 0x4116 and the next one); a right click or no closes it. Yes, when the
   player has the money: gv_OrderMoveGrave and its answer waited for; refused (slot state 2) -> the message 0x411b
   and the form stays, else the form closes with 1. Returns 1 when the move was made, else 0 */
int ai_GraveMoveConfirm(AIBLD *rec, int from, unsigned char entry, int to)
{
    char buf[0x200];
    int win;
    int yes;
    int no;
    int v;
    int r;
    int h;
    int cost;

    r = 0;
    cost = rec->owner != ai_PlayerId ? 16000 : 0;
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    if (rec->owner == ai_PlayerId)
        io_PrintText(0x4115, 0x4112, rec, ai_GraveNames[to]);
    else
        io_PrintText(0x4114, 0x4112, ai_GraveNames[to], cost);
    d2_SetForm(win, 2);
    v = io_PrintText(0x4116);
    yes = d2_GetWinObj(win, 2, v);
    no = d2_GetWinObj(win, 2, v + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes) {
                if (cost == 0 || sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], cost, ai_OwnPlayer)) {
                    h = gv_OrderMoveGrave((int *)ai_Chars[ai_PlayerId], to, (char *)rec, from, entry, cost);
                    while (h != -1 && cm_GetSlotState(h) == 0)
                        sv_RunFrame(0, 0x20007);
                    sv_FrameMask = 0x677c7;
                    if (cm_GetSlotState(h) == 2) {
                        txt_Format(buf, 0x411b, 0x4112, ai_GraveNames[to]);
                        sv_MessageBox((int)buf, 0x100);
                    } else {
                        sv_PopRequest = 1;
                        r = 1;
                    }
                }
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_GraveMoveConfirm, 0x677c7));
    d2_CloseView(win);
    return r;
}
/* 0x584260, the rent-a-grave window (427 B): rec = the record, a / b the values passed on; the form
   "Locations\Friedhof\RentAGrave" (d2_OpenForm (0, 0), the pointer sprite off, centred, fitted), the texts 0x4113
   (0x4112) and "%ia[%s]" (0x4119) in the third section, the four size buttons ai_GraveSizeButtonsAt (rec, window,
   2, the buttons, the selection), the confirm enabled by ai_GraveSizeEnable (rec, a, b, the buttons, the
   selection), d2_ClickedId cleared; the pump loop (this window, 0x677c7): a right button release (dm_L.rup) sets
   sv_PopRequest; the confirm disables the window, ai_GraveMoveConfirm (rec, a, b, the selection) == 1 -> 1 and
   sv_PopRequest, the window enabled again; a clicked button != the selection -> the selection, the buttons and the
   confirm redone; the close of the window, the result */

int ai_GraveMoveWindow(char *rec, int a, unsigned char b)
{
    int win;
    int btn;
    int sel;
    int r;
    int i;
    int btns[4];

    sel = 0;
    r = 0;
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\RentAGrave");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x4113, 0x4112);
    ai_GraveSizeButtonsAt((AIBLD *)rec, win, 2, btns, &sel);
    d2_SetForm(win, 3);
    i = io_PrintText("%ia[%s]", 0x4119);
    btn = d2_GetWinObj(win, 3, i);
    d2_SetObjEnabled(btn, ai_GraveSizeEnable((AIBLD *)rec, a, b, btns, &sel));
    d2_ClickedId = -1;
    for (;;) {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btn) {
                d2_SetViewEnabled(win, 0);
                if (ai_GraveMoveConfirm((AIBLD *)rec, a, b, sel) == 1) {
                    r = 1;
                    sv_PopRequest = 1;
                }
                d2_SetViewEnabled(win, 1);
            } else {
                for (i = 0; i < 4; i++) {
                    if (d2_PointerObj == btns[i] && i != sel) {
                        sel = i;
                        ai_GraveSizeButtonsAt((AIBLD *)rec, win, 2, btns, &sel);
                        d2_SetObjEnabled(btn, ai_GraveSizeEnable((AIBLD *)rec, a, b, btns, &sel));
                        break;
                    }
                }
            }
        }
        if (sv_RunFrame(ai_GraveMoveWindow, 0x677c7) == 0) {
            d2_CloseView(win);
            return r;
        }
    }
}
/* 0x584410, the two buttons of the grave detail window (ai_GraveDetailOpen fills w: 0 the window, 6 the button
   section, 7 the selected entry record, 8 / 9 the grave's size and entry) and its pump: the buttons txt 0x4118
   (ai_GraveFreeConfirm on the grave) and 0x4119 (ai_GraveMoveWindow, the move to another size; disabled when the entry is
   already of size 3), each run with the window disabled and closing it when it answers 1; a right click
   closes it */
void ai_GraveDetailButtons(char *rec, int *w)
{
    int btns[2];
    int first;
    int move;

    sv_ButtonRow(w[6], 0, btns, 2, txt_Ptrs[0x4118], txt_Ptrs[0x4119]);
    first = btns[0];
    move = btns[1];
    if (((GVENTRY *)w[7])->size == 3)
        d2_SetObjEnabled(move, 0);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == first) {
                d2_SetViewEnabled(w[0], 0);
                if (ai_GraveFreeConfirm(rec, w[8], w[9]) == 1)
                    sv_PopRequest = 1;
                d2_SetViewEnabled(w[0], 1);
            } else if (d2_PointerObj == move) {
                d2_SetViewEnabled(w[0], 0);
                if (ai_GraveMoveWindow(rec, w[8], w[9]) == 1)
                    sv_PopRequest = 1;
                d2_SetViewEnabled(w[0], 1);
            }
        }
    } while (sv_RunFrame((int)ai_GraveDetailButtons, 0x677c7));
}
/* 0x584520, confirms renting a grave: rec = the graveyard building, size = the grave size. The lease costs
   gv_LeasePrice of the player's character (full care) unless the player owns the graveyard. The form
   "Locations\Friedhof\Friedhof" shows the question (txt 0x410c with the building for the owner, else 0x410b with
   the price) and the yes / no entries (txt 0x410d and the next one); a right click or no closes it. Yes, when the
   player has the money: gv_OrderReserveGrave for 10 years and its answer waited for; refused (slot state 2) ->
   the message 0x411b and the form stays, else the form closes with 1. Returns 1 when the grave was rented */
int ai_GraveRentConfirm(AIBLD *rec, int size)
{
    char buf[0x200];
    int share;
    int win;
    int yes;
    int no;
    int v;
    int r;
    int h;
    int cost;

    r = 0;
    if (rec->owner == ai_PlayerId)
        cost = 0;
    else
        cost = gv_LeasePrice((unsigned short *)ai_Chars[ai_PlayerId], 0xff, size, 1, &share);
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    if (rec->owner == ai_PlayerId)
        io_PrintText(0x410c, 0x4109, rec, ai_GraveNames[size]);
    else
        io_PrintText(0x410b, 0x4108, ai_GraveNames[size], cost);
    d2_SetForm(win, 2);
    v = io_PrintText(0x410d);
    yes = d2_GetWinObj(win, 2, v);
    no = d2_GetWinObj(win, 2, v + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes) {
                if (cost == 0 || sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], cost, ai_OwnPlayer)) {
                    h = gv_OrderReserveGrave((int *)ai_Chars[ai_PlayerId], (char *)rec, size, 10, cost);
                    while (h != -1 && cm_GetSlotState(h) == 0)
                        sv_RunFrame(0, 0x20007);
                    sv_FrameMask = 0x677c7;
                    if (cm_GetSlotState(h) == 2) {
                        txt_Format(buf, 0x411b, (rec->owner == ai_PlayerId) + 0x4108, ai_GraveNames[size]);
                        sv_MessageBox((int)buf, 0x100);
                    } else {
                        sv_PopRequest = 1;
                        r = 1;
                    }
                }
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_GraveRentConfirm, 0x677c7));
    d2_CloseView(win);
    return r;
}
/* 0x5847a0, the rent-a-grave selection window (340 B): rec = the record; the form
   "Locations\Friedhof\RentAGrave" (d2_OpenForm (0, 0), the pointer sprite off, centred, fitted), the texts 0x410a
   / 0x4108 and "%ia[%s]" (0x4117) in the third section, the four size buttons ai_GraveSizeButtonsAt (rec, window,
   2, the buttons, the selection), d2_ClickedId cleared; the pump loop (this window, 0x677c7): a right button
   release (dm_L.rup) sets sv_PopRequest; the confirm disables the window, ai_GraveRentConfirm (rec, the selection)
   == 1 -> sv_PopRequest, the window enabled again; a clicked button != the selection -> the selection and the
   buttons redone; the close of the window */

void ai_RentGraveSelect(char *rec)
{
    int win;
    int btn;
    int sel;
    int i;
    int btns[4];

    sel = 0;
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\RentAGrave");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x410a, 0x4108);
    ai_GraveSizeButtonsAt((AIBLD *)rec, win, 2, btns, &sel);
    d2_SetForm(win, 3);
    i = io_PrintText("%ia[%s]", 0x4117);
    btn = d2_GetWinObj(win, 3, i);
    d2_ClickedId = -1;
    for (;;) {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btn) {
                d2_SetViewEnabled(win, 0);
                if (ai_GraveRentConfirm((AIBLD *)rec, sel) == 1)
                    sv_PopRequest = 1;
                d2_SetViewEnabled(win, 1);
            } else {
                for (i = 0; i < 4; i++) {
                    if (d2_PointerObj == btns[i] && i != sel) {
                        sel = i;
                        ai_GraveSizeButtonsAt((AIBLD *)rec, win, 2, btns, &sel);
                        break;
                    }
                }
            }
        }
        if (sv_RunFrame(ai_RentGraveSelect, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
    }
}

int sv_MessageBoxGreen(); void sv_OpenPlayerBar(int a, char k); void sv_ClosePlayerBar(void);

/* 0x584900, the "GRAEBER_PFLEGEN" callback of the graveyard window (ai_GraveWindow, a function pointer only):
   sends a selected character to tend the picked grave. When a work is already pending on the grave
   (he_HasGraveWork) the green message txt 0x411e / 0x411f and 0. Else, with the player bar open, the form
   "Locations\Friedhof\Friedhof" asks (txt 0x4120 with 0x411e, 0x228) with the yes / no entries (txt 0x4121 and
   the next one); a right click or no closes it. Yes: without a selected character the message txt 0x4093, else
   the work message 0x8d (flags 1, to the owner, the game time, the character, the grave) is sent, its slot
   waited for and the form closes with 1. The bar and the selection are cleared at the end */
int ai_GraveCare(AIBLD *rec, GVPICK *pick)
{
    AIMSG msg;
    AICHAR *c;
    int win;
    int yes;
    int no;
    int v;
    int i;
    int r;
    int h;

    r = 0;
    if (he_HasGraveWork(rec, pick->size, pick->index) != 0) {
        sv_MessageBoxGreen((int)txt_Ptrs[0x411e], (int)txt_Ptrs[0x411f], 0);
        return 0;
    }
    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x4120, 0x411e, 0x228);
    d2_SetForm(win, 2);
    v = io_PrintText(0x4121);
    yes = d2_GetWinObj(win, 2, v);
    no = d2_GetWinObj(win, 2, v + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes) {
                c = 0;
                for (i = 0; i < 0x300; i++) {
                    if (ai_CharsT[i].selected) {
                        c = &ai_CharsT[i];
                        break;
                    }
                }
                if (c == 0) {
                    sv_MessageBox(txt_Ptrs[0x4093], 0);
                } else {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x8d;
                    msg.flags = 1;
                    msg.to = ai_CharsT[rec->owner].netId;
                    msg.fromId = rec->id;
                    msg.time = ai_Time;
                    msg.graveWork.size = pick->size;
                    msg.graveWork.index = pick->index;
                    msg.ids[0] = c->netId;
                    h = cm_SendLongMessage((char *)&msg);
                    while (h != -1 && cm_GetSlotState(h) == 0)
                        sv_RunFrame(0, 0x20007);
                    r = 1;
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_GraveCare, 0x677c7));
    d2_CloseView(win);
    sv_ClosePlayerBar();
    sv_ClearSelection();
    return r;
}

int gv_RoomForRaised(unsigned char *o, int *n); int gv_OrderRaiseDead(); int cm_GetSlotValue(int h); int sprintf(char *buf, const char *fmt, ...);

/* 0x584b70, the "GEHILFEN_ERSCHAFFEN" callback of the graveyard window (ai_GraveWindow, a function pointer
   only): raises the picked dead person as a helper. gv_RoomForRaised counts the yard's raised: -1 -> 0; 0 (the
   limit reached) -> the message txt 0x4135 (the count, 0x229, the building) and 0; else the form
   "Locations\Friedhof\Friedhof" asks (txt 0x4132 with 0x4131, 0x229) with the yes / no entries (txt 0x4133 and
   the next one); a right click or no closes it. Yes without a dead person picked closes it; else
   gv_OrderRaiseDead (kind 0x1c), its slot waited for, and with the slot's value (the new character, a guess
   from its +4 the netId) the script "LOCATIONS\FRIEDHOF\Gehilfen_erschaffen_FRIEDHOF.esc" runs with "sp_<its
   netId>"; the form closes with 1 */
int ai_GraveRaiseHelper(AIBLD *rec, GVPICK *pick)
{
    char buf[0x200];
    char line[0x40];
    int win;
    int yes;
    int no;
    int n;
    int r;
    int v;
    int h;
    AICHAR *c;
    EVTSCRIPT *sc;

    r = 0;
    v = gv_RoomForRaised((unsigned char *)rec, &n);
    if (v != 1) {
        if (v == 0) {
            txt_Format(buf, 0x4135, n, 0x229, rec);
            sv_MessageBox((int)buf, 0);
        }
        return 0;
    }
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x4132, 0x4131, 0x229);
    d2_SetForm(win, 2);
    v = io_PrintText(0x4133);
    yes = d2_GetWinObj(win, 2, v);
    no = d2_GetWinObj(win, 2, v + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == yes) {
                if (pick->grave->id == -1) {
                    sv_PopRequest = 1;
                } else {
                    h = gv_OrderRaiseDead((char *)rec, pick->entry->size, pick->index, pick->id, 0x1c);
                    while (h != -1 && cm_GetSlotState(h) == 0)
                        sv_RunFrame(0, 0x20007);
                    c = (AICHAR *)cm_GetSlotValue(h);
                    if (c != 0) {
                        sprintf(line, "sp_%u", c->netId);
                        sc = evt_Load("LOCATIONS\\FRIEDHOF\\Gehilfen_erschaffen_FRIEDHOF.esc");
                        evt_RunScriptArgs(sc, 1, line);
                    }
                    r = 1;
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == no) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_GraveRaiseHelper, 0x677c7));
    d2_CloseView(win);
    return r;
}

int sv_CheckCharacterFlag(int id); char * he_FindEvent(int n, ...); char * he_NextEvent(void); void sv_OpenWindowFrame(int f, int b, int c);
void ai_InitProductionTables(void); int ai_UpdateProdGoodsList(); void ai_LayoutProdGoodsList(int param_1, int param_2, int param_3, int param_4);

/* 0x584d70, the helpers window of a building (the form "HANDEL\HANDEL_SUCHEN"): rec = the building. Its type
   offers two helper kinds (0x225 / 0x226, of the type kind 0x1f 0x293 / 0x294) and a third (0x227 / 0x295) from
   the type's level 2 on; a character flag ends it at once. The window frame 0x70f / 0x741, the title txt 0x421e
   (kind 0x1f) / 0x4128, the player bar and the production table (ai_InitProductionTables), and one picture button per helper
   kind (its picture kind + 0xdf, its name the caption) 0x50 apart. Each frame the production lines are redrawn
   (ai_UpdateProdGoodsList with the building's gm_Query record, ai_Unk58dd00), the helpers already on the building's helper
   jobs 0x8c are counted per kind and each button reads the kind's name with the count (txt 0x421f / 0x4220 for
   kind 0x1f, 0x4129 / 0x412a for the others, the name alone without one). A click on a button sends the helper
   job message 0x8c (flags 1, to the owner, the game time, the kind, the first four selected characters) and
   waits for it; with no character selected the message txt 0x4093; the selection is cleared. A right click
   ends the window
   @deep reconstructed in full from the original (all 449 instructions read: the kinds / limit setup, both
   helper-job counts over AIJOB.helpers, the window and button setup, the per-frame caption texts, the
   click -> message 0x8c with the selected characters, the closing); 448 vs 449 instructions, the
   statements and branches match (the one branch-target shift, je at +0xd8, is the 2-byte xor ebx,ebx
   before the d2_OpenForm it jumps to). Left is the constant register: the original caches 2 in ebx (push
   ebx for gm_Query / he_FindEvent's 2, cmp al,bl for the level test, n stored from it) and 0x227 in esi,
   and so saves ebx in the prologue; ours caches 0 in ebx (push ebx for every 0 argument and the counts
   zeroing, xor ebx,ebx again before d2_OpenForm) and saves it after the sv_CheckCharacterFlag return,
   which shifts every stack slot by 4 (the slots hunks, inc [esp+edx+0x24] vs 0x20) and turns the second
   count zeroing into esi vs edx (hunks 0x584d81, 0x584e24, 0x584e86, 0x58503c, 0x58505a, 0x5850a6). Also
   the button loop's pushes are the same instructions in another register order (0x584f4d, regs eax<->edx
   at 0x584f0b / 0x584f72), and in the caption loop the kind test is cmp byte [ecx],0x1f (ours, counts[i]
   already in eax) vs mov al,[ecx] / cmp al / mov eax,counts[i] (0x5850cf). Tried: the statement order of n
   / kinds[2] / gm_Query (this one best), chained counts zeroing (identical), the count local inside each
   branch (worse: the name argument is hoisted again), the count tests as > 1 first / a switch (worse) */
void ai_GypsyHelperWindow(AIBLD *rec)
{
    AIBTYPE *type;
    AIJOB *job;
    char *res;
    int win;
    int counts[3];
    int kinds[3];
    int n;
    int i;
    int j;
    int k;
    int sel;
    int c;
    int id;
    int h;
    int x;

    n = 2;
    kinds[2] = 0x227;
    res = gm_Query((void *)rec->objList, 2, 6, 0, 0x2a);
    type = &((AIBTYPE *)ai_BTypes)[rec->type];
    if (type->kind == 0x1f) {
        kinds[0] = 0x293;
        kinds[1] = 0x294;
        if (type->cap247 >= 2) {
            kinds[2] = 0x295;
            n = 3;
        }
    } else {
        kinds[0] = 0x225;
        kinds[1] = 0x226;
        if (type->cap247 >= 2) {
            kinds[2] = 0x227;
            n = 3;
        }
    }
    if (sv_CheckCharacterFlag((char *)rec) != 0)
        return;
    counts[0] = 0;
    counts[1] = 0;
    counts[2] = 0;
    for (job = (AIJOB *)he_FindEvent(2, 0, 0x8c, 3, rec->id); job != 0; job = (AIJOB *)he_NextEvent()) {
        for (i = 0; i < 3; i++) {
            if (job->u.w.lo == kinds[i]) {
                for (j = 0; j < 4; j++)
                    if (job->helpers[j] != -1)
                        counts[i]++;
            }
        }
    }
    win = d2_OpenForm(0, 0, "HANDEL\\HANDEL_SUCHEN");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x70f, 0x741);
    d2_SetForm(win, 3);
    if (type->kind == 0x1f)
        io_PrintText(0x421e);
    else
        io_PrintText(0x4128);
    sv_OpenPlayerBar((int)rec, 0);
    ai_InitProductionTables();
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    x = d2_Wins[d2_LastWindow].w / 2 - 0xa8;
    for (i = 0; i < n; i++) {
        int btns[3];

        id = d2_WinOpenPic(x + 0x28, i * 0x50 + 0x19, kinds[i] + 0xdf, d2_LastWindow);
        d2_Objs[id].interactive = 1;
        d2_Objs[id].style = 3;
        d2_Objs[id].captionX = 100;
        d2_Objs[id].captionY = 0x10;
        d2_Objs[id].font = 0x54;
        d2_SetCaption(id, (char *)txt_ObjectNames[kinds[i]][0]);
        btns[i] = id;
        d2_SetObjEnabled(id, 1);
    }
    d2_SetPointerSprite(0, 0);
    ai_UpdateProdGoodsList(rec, res);
    ai_LayoutProdGoodsList(win, 1, 0, rec);
    while (sv_RunFrame((int)ai_GypsyHelperWindow, 0x677c7)) {
        int btns[3];

        ai_UpdateProdGoodsList(rec, res);
        ai_LayoutProdGoodsList(win, 1, 0, rec);
        counts[0] = 0;
        counts[1] = 0;
        counts[2] = 0;
        for (job = (AIJOB *)he_FindEvent(2, 0, 0x8c, 3, rec->id); job != 0; job = (AIJOB *)he_NextEvent()) {
            for (i = 0; i < 3; i++) {
                if (job->u.w.lo == kinds[i]) {
                    for (j = 0; j < 4; j++)
                        if (job->helpers[j] != -1)
                            counts[i]++;
                }
            }
        }
        d2_SetForm(win, 2);
        for (i = 0; i < n; i++) {
            char line[0x100];

            id = btns[i];
            d2_SetObjEnabled(id, 1);
            c = counts[i];
            if (type->kind == 0x1f) {
                if (c == 1)
                    txt_Format(line, 0x421f, 0x22d, kinds[i] * 2 + 0x9a8);
                else if (c > 1)
                    txt_Format(line, 0x4220, c, 0x22d, kinds[i] * 2 + 0x9a8);
                else
                    txt_Format(line, "%s", kinds[i] * 2 + 0x9a8);
            } else {
                if (c == 1)
                    txt_Format(line, 0x4129, 0x228, kinds[i] * 2 + 0x9a8);
                else if (c > 1)
                    txt_Format(line, 0x412a, c, 0x228, kinds[i] * 2 + 0x9a8);
                else
                    txt_Format(line, "%s", kinds[i] * 2 + 0x9a8);
            }
            d2_SetCaption(id, line);
        }
        if (d2_ClickedId != -1) {
            sel = -1;
            for (i = 0; i < n; i++) {
                if (btns[i] == d2_PointerObj) {
                    sel = i;
                    break;
                }
            }
            if (sel != -1) {
                AIMSG msg;

                memset(&msg, 0, sizeof(msg));
                msg.type = 0x8c;
                msg.flags = 1;
                msg.to = ai_CharsT[rec->owner].netId;
                msg.fromId = rec->id;
                msg.time = ai_Time;
                msg.itemId = kinds[sel];
                k = 0;
                for (j = 0; j < 0x300 && k < 4; j++) {
                    if (ai_CharsT[j].selected)
                        msg.ids[k++] = ai_CharsT[j].netId;
                }
                for (j = k; j < 4; j += 2)
                    msg.ids[j] = -1;
                if (k != 0) {
                    h = cm_SendLongMessage((char *)&msg);
                    while (h != -1 && cm_GetSlotState(h) == 0)
                        sv_RunFrame(0, 0x20007);
                    sv_FrameMask = 0x677c7;
                    sv_ClearSelection();
                } else {
                    sv_MessageBox(txt_Ptrs[0x4093], 4);
                    sv_ClearSelection();
                }
            }
        } else if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        }
    }
    sv_ClearSelection();
    sv_ClosePlayerBar();
    d2_CloseView(win);
    sv_CameraFollow();
}

int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay); int ai_MapPick();

/* 0x585380, the graveyard's fog window (the Graveyard Owner's action 0x90, am.c ai_DecideGraveyardOwner1): rec =
   the graveyard. A fog of this graveyard still pending (he_FindEvent 0x90) -> the voice "_NACHRICHTEN_FRIEDHOF_14"
   and the message txt 0x40c8 (0x40b6), out. Else the player picks the target building on the map (ai_MapPick,
   the building filter 0x1a07c43b4 of the AI's pick, title txt 0x40b6); none -> out. The form
   "Locations\Friedhof\Fog" shows the target (txt 0x40b7), two three-step sliders (labels txt 0x40bf / 0x40c3,
   steps 0x40c0 .. 0x40c2 / 0x40c4 .. 0x40c6) and the order / leave entries from txt 0x40be on; each frame the
   cost (factor1 + 2) * (factor2 + 2) (txt 0x40b9 / 0x40ba). Order, when the player has the money: the message
   0x90 (flags 2) from the graveyard to the player's character with the target and the two factors, and the
   form closes; a right click or leave closes it too. The camera follows again at the end
   @deep Read whole against the original 0x585380..0x58574f and reconstructed: the pending-fog check
   he_FindEvent(2,0,0x90,3,rec->id) with voice + message box, the AI_ZERO inline-asm clear of the AMOUNTCFG
   map filter (flags 1, mode -1, filter 0x1a07c43b4, kind 6, option4c 0x727), ai_MapPick, the Fog form with
   its two sliders and txt_Ptrs labels, the cost (f2+2)*(f1+2), the 0x90 message (to, fromId, time, arg
   0..2) and the do/while pump; 280 of 289 instructions equal. Left is only the callee-saved push
   placement: the original pushes ebx/esi/edi in the prologue (+0x9..0xb) and pops them before the early
   return (pop edi/pop esi/pop ebx before mov esp,ebp), ours shrink-wraps them past the early return (push
   ebx / push esi / push edi at +0x64..); the branch-target difference (je at +0x3b: +0x64 vs +0x6a) is
   only that 3-byte shift. Tried: the early exit as if/else with one exit (same code), the inverted if
   (found == 0) {..} else {..} (135 differing, reverted). VC6 shrink-wrapping, no source lead left */
void ai_GraveFogWindow(AIBLD *rec)
{
    char buf[0x400];
    AIMSG msg;
    AMOUNTCFG cfg;
    AIBLD *target;
    int win;
    int factor1;
    int factor2;
    int btnOrder;
    int btnLeave;
    int k;
    int cost;

    txt_Format(buf, (char *)0x40c8, 0x40b6);
    if (he_FindEvent(2, 0, 0x90, 3, rec->id) != 0) {
        snd_QueueVoice(-1, 0, "_NACHRICHTEN_FRIEDHOF_14", 0, 100);
        sv_MessageBox(buf, 0);
        return;
    }
    AI_ZERO(&cfg, sizeof(cfg));
    cfg.flags = 1;
    cfg.mode = -1;
    cfg.kind = 6;
    cfg.filter = 0x1a07c43b4;
    cfg.option4c = 0x727;
    target = (AIBLD *)ai_MapPick(1, &cfg, txt_Ptrs[0x40b6], 0, 0);
    if (target != 0) {
        win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Fog");
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        sv_FitForm(d2_LastWindow);
        d2_SetForm(win, 1);
        io_PrintText(0x40b7, 0x40b6, target->owner, target);
        factor1 = d2_WinOpenSlider(100, 0xa0, 0xc8, 0, 2, 0x7b, 0x82, d2_LastWindow);
        d2_SetObjValue4(factor1, 0, 2, 0);
        d2_SetSliderFlags(factor1, 0x82);
        d2_SetLabel(factor1, txt_Ptrs[0x40bf]);
        d2_SetList(factor1, 3, txt_Ptrs[0x40c0], txt_Ptrs[0x40c1], txt_Ptrs[0x40c2]);
        factor2 = d2_WinOpenSlider(100, 0xbe, 0xc8, 0, 2, 0x7b, 0x82, d2_LastWindow);
        d2_SetObjValue4(factor2, 0, 2, 0);
        d2_SetSliderFlags(factor2, 0x82);
        d2_SetLabel(factor2, txt_Ptrs[0x40c3]);
        d2_SetList(factor2, 3, txt_Ptrs[0x40c4], txt_Ptrs[0x40c5], txt_Ptrs[0x40c6]);
        d2_SetForm(win, 2);
        k = io_PrintText(0x40be);
        btnOrder = d2_GetWinObj(win, 2, k);
        btnLeave = d2_GetWinObj(win, 2, k + 1);
        d2_ClickedId = -1;
        do {
            d2_SetForm(win, 3);
            io_PrintText("$C");
            cost = (d2_GetObjValue(factor2) + 2) * (d2_GetObjValue(factor1) + 2);
            io_PrintText(0x40b9);
            io_PrintText(0x40ba, cost);
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
            } else if (d2_ClickedId != -1) {
                if (d2_PointerObj == btnOrder) {
                    if (sv_CheckMoney(ai_Chars[ai_PlayerId], cost) == 1) {
                        memset(&msg, 0, sizeof(msg));
                        msg.type = 0x90;
                        msg.flags = 2;
                        msg.to = ((AICHAR *)ai_Chars[ai_PlayerId])->netId;
                        msg.fromId = rec->id;
                        msg.time = ai_Time;
                        msg.arg[1] = d2_GetObjValue(factor1);
                        msg.arg[2] = d2_GetObjValue(factor2);
                        msg.arg[0] = target->id;
                        cm_SendLongMessage((char *)&msg);
                        sv_PopRequest = 1;
                    }
                } else if (d2_PointerObj == btnLeave) {
                    sv_PopRequest = 1;
                }
            }
        } while (sv_RunFrame(ai_GraveFogWindow, 0x677c7) != 0);
        d2_CloseView(win);
    }
    sv_CameraFollow();
}
/* the result of the seance window (ai_SeanceWindow), read by the seance kinds 0x585cb0 / 0x586200 / 0x585f30 */
typedef struct GVSEANCE {
    int kind;                   /* 0x00, the seance chosen: 1 .. 3, 0 none */
    int cost;                   /* 0x04, its price: 4 / 6 / 8 */
    int size;                   /* 0x08, the grave's size (list) */
    unsigned char index;        /* 0x0c, the grave's index in that list */
    char pad0d[3];
    int graveId;                /* 0x10, the dead person's GVGRAVE.id */
    GVGRAVE grave;              /* 0x14, a copy of the dead person */
} GVSEANCE;

/* 0x585750, the graveyard's seance window "Locations\Friedhof\Seance" for the dead person w->items[w->selected] of
   the detail window w (the first argument is unused): out gets the grave and a copy of the dead person, kind and
   cost 0. When the player's character has the flag 8 of flags1cc (a seance already held, a reasoned guess), the
   green message box txt 0x40cc / 0x40cd and 0. Else the title txt 0x40ce (0x40cc), the entries 0x40cf .. 0x40d1
   (the prices 4 / 6 / 8) and the cancel entry 0x40d2; a right click or cancel closes it; an entry the player can
   pay (sv_CheckMoney) sets out->kind (1 .. 3) and out->cost and closes it with 1 */
int ai_SeanceWindow(char *unused, GVDETAILWIN *w, GVSEANCE *out)
{
    int cost[3];
    int win;
    int btn1;
    int btn2;
    int btn3;
    int btnCancel;
    int kind;
    int r;

    r = 0;
    out->cost = 0;
    out->kind = 0;
    out->size = w->size;
    out->index = w->index;
    cost[0] = 4;
    cost[1] = 6;
    cost[2] = 8;
    out->graveId = w->items[w->selected].grave->id;
    out->grave = *w->items[w->selected].grave;
    if (ai_CharsT[ai_PlayerId].flags1cc & 8) {
        sv_MessageBoxGreen(txt_Ptrs[0x40cc], txt_Ptrs[0x40cd], 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Seance");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x40ce, 0x40cc);
    d2_SetForm(win, 2);
    btn1 = d2_GetWinObj(win, 2, io_PrintText(0x40cf, 4));
    btn2 = d2_GetWinObj(win, 2, io_PrintText(0x40d0, 6));
    btn3 = d2_GetWinObj(win, 2, io_PrintText(0x40d1, 8));
    btnCancel = d2_GetWinObj(win, 2, io_PrintText(0x40d2));
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnCancel) {
                sv_PopRequest = 1;
            } else {
                if (d2_PointerObj == btn1)
                    kind = sv_CheckMoney(ai_Chars[ai_PlayerId], 4);
                else if (d2_PointerObj == btn2)
                    kind = sv_CheckMoney(ai_Chars[ai_PlayerId], 6) * 2;
                else if (d2_PointerObj == btn3)
                    kind = sv_CheckMoney(ai_Chars[ai_PlayerId], 8) * 3;
                else
                    continue;
                if (kind != 0) {
                    out->kind = kind;
                    out->cost = cost[kind - 1];
                    r = 1;
                    sv_PopRequest = 1;
                }
            }
        }
    } while (sv_RunFrame(ai_SeanceWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return r;
}

int sb_IsInit(); void d2_PlaceViewWindows(int view, unsigned char flags);

/* 0x5859c0, the seance ritual of the seance kinds (ch unused, ev the seance window's result): rolls the eight
   formula texts (ai_SeanceRollFormulas from txt 0x40d7), opens "Locations\Friedhof\Seance2" and starts the script
   "LOCATIONS\KRYPTA\SEANCE_KRYPTA.esc" with the names of the player's character and of the dead person ("%s %s",
   each name record's own id, else the first of its sex). With the sound system: each formula is voiced
   ("_SEANCE_FORMEL_PART_%i_0%i", its pair and variant), and its text shown while it plays. Without: the texts
   appear one by one, the first after 0x47 ms, the rest 0xf8 ms apart, and the window closes after the last.
   Closes the form; returns the script
   @deep Read whole against the original 0x5859c0..0x585c3e: the formula roll, the Seance2 form, the
   SEANCE_KRYPTA script with the two names (the original drops the player's fallback name record: kept as a
   bare call), the sb_IsInit voice path (formula k = id - 0x40d7 as its own local: sub vs add-negative
   fixed) and the clock path (d3_Time, the nine waits written out as the original stores them);
   196 of 200 instructions equal. Left is one moved instruction: the original loads ev (mov edi,
   [esp+0x57c]) at the head of the script block (0x585a23) before the first gm_NameRecord, ours just before
   the ev->grave loads (0x585a77); the branch-target difference (jne at +0x93) is that move. Scheduling, no
   source lead (no other use of ev in the block) */
int ai_SeanceRitual(char *ch, GVSEANCE *ev)
{
    char buf[0x400];
    char text[0x100];
    int variants[8];
    int ids[8];
    int wait[9];                /* the clock path's delays, the voice path's voice handles */
    char *name1;
    char *name2;
    int script;
    int win;
    int i;
    int h;
    int k;                      /* the formula's offset from txt 0x40d7: its pair (/ 6) and variant (% 6) */
    unsigned int t;

    ai_SeanceRollFormulas(8, ids, variants, 0x40d7);
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Seance2");
    d2_SetPointerSprite(0, 0);
    d2_PlaceViewWindows(win, 1);
    d2_SetForm(win, 0);
    script = (int)evt_Load("LOCATIONS\\KRYPTA\\SEANCE_KRYPTA.esc");
    if (script != 0) {
        name1 = (char *)gm_NameRecord(ai_CharsT[ai_PlayerId].sex, ai_CharsT[ai_PlayerId].jobId);
        if (name1 == 0)
            gm_NameRecord(ai_CharsT[ai_PlayerId].sex, 0);   /* @original drops the result: name1 stays 0 */
        name2 = (char *)gm_NameRecord(ev->grave.sex, ev->grave.nameId);
        if (name2 == 0)
            name2 = (char *)gm_NameRecord(ev->grave.sex, 0);
        sprintf(buf, "%s %s", name1 + 4, name2 + 4);
        evt_RunCommand((EVTSCRIPT *)script, buf);
    }
    i = 0;
    if (sb_IsInit() != 0) {
        for (i = 0; i < 8; i++) {
            k = ids[i] - 0x40d7;
            sprintf(text, "_SEANCE_FORMEL_PART_%i_0%i", k / 6 + 1, k % 6);
            wait[i] = snd_QueueVoice(-7, 0, text, -1, 100);
        }
        for (i = 0; i < 8; i++) {
            h = wait[i];
            while (sb_IsPlaying(h) == 0)
                sv_RunFrame(0, 0x743c7);
            d2_SetForm(win, 0);
            io_PrintText(ids[i]);
            while (sb_IsPlaying(h) != 0)
                sv_RunFrame(0, 0x743c7);
        }
    } else {
        t = d3_Time;
        wait[0] = 0x47;
        wait[1] = 0xf8;
        wait[2] = 0xf8;
        wait[3] = 0xf8;
        wait[4] = 0xf8;
        wait[5] = 0xf8;
        wait[6] = 0xf8;
        wait[7] = 0xf8;
        wait[8] = 0xf8;
        do {
            if (t + wait[i] < (unsigned int)d3_Time) {
                t = d3_Time;
                d2_SetForm(win, 0);
                if (i >= 8) {
                    sv_PopRequest = 1;
                } else {
                    d2_SetForm(win, 0);
                    io_PrintText(ids[i]);
                    i++;
                }
            }
        } while (sv_RunFrame(ai_SeanceRitual, 0x743c7) != 0);
    }
    d2_CloseView(win);
    return script;
}
/* 0x585c40, rolls the seance formulas: n formula text ids into ids, in pairs from the text base on (6 texts per
   pair); each pair gets a random variant r 0 .. 2 (util_Random), its first formula base + r, its second base + r
   + 3, and variants[] gets r for both */
void ai_SeanceRollFormulas(int n, int *ids, int *variants, int base)
{
    int i;
    int r;

    for (i = 0; i < n; i += 2) {
        r = util_Random(3) & 0xffff;
        variants[i] = r;
        variants[i + 1] = r;
        ids[i] = base + r;
        ids[i + 1] = variants[i] + base + 3;
        base += 6;
    }
}

int snd_VoiceVariant(); char *gv_FindAccused(char *c, int max, int *ids, int *cnt); float gv_SeanceChance(); float util_RandomF();
void cm_SendProofExecuted(int a, int b); 

/* 0x585cb0, the seance kind 1, the ghost of the dead person (rec = the graveyard, ev = the seance window's
   result): the dead person's voice (snd_VoiceVariant), the ritual (ai_SeanceRitual), then the outcome: the ghost
   names the player's accusers (gv_FindAccused, up to 4: their proofs are dropped, cm_SendProofExecuted, the
   player's life value is set back to the age 1, txt 0x40ef with the first accuser), or, with none, it gives
   money ((0x200 .. 0x3ff) * 0x20, txt 0x40f0); when util_RandomF() + gv_SeanceChance (rec) - 0.1 < 1 it does not
   come at all (txt 0x40d3). The form "Locations\Friedhof\Friedhof" shows the outcome with the voice
   "GEIST_KOMMT" / "GEIST_KOMMT_NICHT"; it stays until a right click and pump end, then the script is stopped */
void ai_SeanceGhost(AIBLD *rec, GVSEANCE *ev)
{
    int ids[4];
    int cnt;
    AICHAR *accuser;
    float f;
    int win;
    int voice;
    int mode;
    int script;
    int i;
    unsigned short r;
    int n;

    snd_VoiceVariant(ev->grave.id, ev->grave.sex);
    script = ai_SeanceRitual((char *)rec, ev);
    accuser = (AICHAR *)gv_FindAccused(ai_Chars[ai_PlayerId], 4, ids, &cnt);
    mode = accuser == 0;
    f = gv_SeanceChance((char *)rec);
    if (util_RandomF() + f - 0.1f < 1.0f)
        mode = 2;
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_ClickedId = -1;
    d2_SetForm(win, 1);
    voice = snd_VoiceVariant(ev->grave.id, ev->grave.sex);
    if (mode == 0) {
        for (i = 0; i < cnt; i++)
            cm_SendProofExecuted(ids[i], ai_CharsT[ai_PlayerId].netId);
        he_SetLifeByAge(&ai_CharsT[ai_PlayerId], 1);
        io_PrintText(0x40ef, 0x40cc, accuser->id);
        snd_QueueVoice(voice, 0, "GEIST_KOMMT", util_Random(8) & 0xffff, 0xfa);
    } else if (mode == 1) {
        r = util_RandomU(0x200);
        n = (r + 0x200) << 5;
        cm_SendMoveGoods(ai_CharsT[ai_PlayerId].netId, -1, ai_OwnPlayer, n);
        io_PrintText(0x40f0, 0x40cc, n);
        snd_QueueVoice(voice, 0, "GEIST_KOMMT", util_Random(8) & 0xffff, 0xfa);
    } else {
        io_PrintText(0x40d3, 0x40cc);
        snd_QueueVoice(voice, 0, "GEIST_KOMMT_NICHT", util_Random(8) & 0xffff, 0xfa);
    }
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_SeanceGhost, 0x777c7) != 0);
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}
/* 0x585f30, the seance kind 3, the sacrifice (rec = the graveyard, ev = the seance window's result): the dead
   person's voice and the ritual as in ai_SeanceGhost; when util_RandomF() + gv_SeanceChance (rec) - 0.4 < 1 the
   ghost does not come (txt 0x40d3, "GEIST_KOMMT_NICHT"). Else the player pays a quarter of the worth
   (gm_GetWorth), at least 160000 and at most 800000, the message 0x8e (flags 1) from the graveyard to the player
   with the amount is sent, the player's life value is set back to the age 3, and the form shows txt 0x40f1 with
   the amount ("GEIST_KOMMT"). The form "Locations\Friedhof\Friedhof" stays until a right click and pump end, then
   the script is stopped */
void ai_SeanceSacrifice(AIBLD *rec, GVSEANCE *ev)
{
    AIMSG msg;
    int voice;
    int script;
    float f;
    int win;
    int amount;
    int mode;

    amount = 0;
    mode = 0;
    voice = snd_VoiceVariant(ev->grave.id, ev->grave.sex);
    script = ai_SeanceRitual((char *)rec, ev);
    f = gv_SeanceChance((char *)rec);
    if (util_RandomF() + f - 0.4f < 1.0f) {
        mode = 2;
    } else {
        amount = min(max((int)(gm_GetWorth(ai_PlayerId) * 0.25f), 0x27100), 0xc3500);
        cm_SendMoveGoods(ai_CharsT[ai_PlayerId].netId, -1, ai_OwnPlayer, amount);
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x8e;
        msg.to = ai_CharsT[ai_PlayerId].netId;
        msg.fromId = rec->id;
        msg.time = ai_Time;
        msg.flags = 1;
        msg.arg[0] = amount;
        cm_SendLongMessage((char *)&msg);
        he_SetLifeByAge(&ai_CharsT[ai_PlayerId], 3);
    }
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_ClickedId = -1;
    d2_SetForm(win, 1);
    if (mode == 0) {
        snd_QueueVoice(voice, 0, "GEIST_KOMMT", util_Random(8) & 0xffff, 0xfa);
        io_PrintText(0x40f1, 0x40cc, amount);
    } else {
        snd_QueueVoice(voice, 0, "GEIST_KOMMT_NICHT", util_Random(8) & 0xffff, 0xfa);
        io_PrintText(0x40d3, 0x40cc);
    }
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_SeanceSacrifice, 0x777c7) != 0);
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}

void ai_FindWantedItems(struct G5CH *p, int n, struct G9REC *rec, int *st);

/* 0x586200, the seance kind 2, the ghost's gift (rec = the graveyard, ev = the seance window's result): the dead
   person's voice and the ritual as in ai_SeanceGhost. The fifteen gifts 0x2b8 .. 0x2c6 are looked up in the
   player's inventory (ai_FindWantedItems); when six are found and none usable, or all fifteen are unusable, the
   ghost gives money ((1000 .. 2023) * 0x20, txt 0x40f3), else a random one the player lacks (from a random start
   the next with value 0: cm_SendExSellObject, the player's life value is set back to the age 2, txt 0x40f2 with
   the item's name text 0x9a8 + 2 * item). When util_RandomF() + gv_SeanceChance (rec) - 0.2 < 1 the ghost does
   not come (txt 0x40d3). The form "Locations\Friedhof\Friedhof" shows it with the voice "GEIST_KOMMT" /
   "GEIST_KOMMT_NICHT" and stays until a right click and pump end, then the script is stopped */
void ai_SeanceGift(AIBLD *rec, GVSEANCE *ev)
{
    G9REC gifts[15];
    int st[3];
    float f;
    int win;
    int voice;
    int script;
    int item;
    int mode;
    int i;
    unsigned short r;
    int n;

    item = 0;
    voice = snd_VoiceVariant(ev->grave.id, ev->grave.sex);
    script = ai_SeanceRitual((char *)rec, ev);
    memset(gifts, 0, sizeof(gifts));
    gifts[0].item = 0x2b8;
    gifts[1].item = 0x2bb;
    gifts[2].item = 0x2bc;
    gifts[3].item = 0x2bf;
    gifts[4].item = 0x2c2;
    gifts[5].item = 0x2c3;
    gifts[6].item = 0x2bd;
    gifts[7].item = 0x2c1;
    gifts[8].item = 0x2c4;
    gifts[9].item = 0x2c5;
    gifts[10].item = 0x2c6;
    gifts[11].item = 0x2ba;
    gifts[12].item = 0x2c0;
    gifts[13].item = 0x2be;
    gifts[14].item = 0x2b9;
    ai_FindWantedItems((struct G5CH *)ai_Chars[ai_PlayerId], 0xf, gifts, st);
    if ((st[0] == 6 && st[2] == 0) || st[1] == 0xf) {
        mode = 1;
    } else {
        i = util_Random(0xf) & 0xffff;
        while (gifts[i].value != 0)
            i = (i + 1) % 0xf;
        item = gifts[i].item;
        mode = 0;
    }
    f = gv_SeanceChance((char *)rec);
    if (util_RandomF() + f - 0.2f < 1.0f)
        mode = 2;
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_ClickedId = -1;
    d2_SetForm(win, 1);
    if (mode == 0) {
        cm_SendExSellObject(ai_CharsT[ai_PlayerId].netId, -1, item, 1, ai_OwnPlayer, 0);
        he_SetLifeByAge(&ai_CharsT[ai_PlayerId], 2);
        io_PrintText(0x40f2, 0x40cc, item * 2 + 0x9a8);
        snd_PlayVoice(voice, 0, "GEIST_KOMMT", util_Random(8) & 0xffff);
    } else if (mode == 1) {
        r = util_RandomU(0x400);
        n = (r + 1000) << 5;
        cm_SendMoveGoods(ai_CharsT[ai_PlayerId].netId, -1, ai_OwnPlayer, n);
        io_PrintText(0x40f3, 0x40cc, n);
        snd_PlayVoice(voice, 0, "GEIST_KOMMT", util_Random(8) & 0xffff);
    } else {
        io_PrintText(0x40d3, 0x40cc);
        snd_PlayVoice(voice, 0, "GEIST_KOMMT_NICHT", util_Random(8) & 0xffff);
    }
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_SeanceGift, 0x777c7) != 0);
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}
/* 0x586560, the pick of the dead person for a seance in the graveyard detail window w (rec = the graveyard): each
   frame (pump 0x677c7) a right click closes it; a click on one of the eight items selects it (w->selected) and
   opens the seance window for it (ai_SeanceWindow, w disabled meanwhile); a seance chosen there closes it with
   1, out holding the choice */
int ai_SeancePick(char *rec, GVDETAILWIN *w, GVSEANCE *out)
{
    int r;
    int i;

    r = 0;
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            for (i = 0; i < 8; i++) {
                if (d2_PointerObj == w->items[i].btn) {
                    w->selected = i;
                    d2_SetViewEnabled(w->win, 0);
                    r = ai_SeanceWindow(rec, w, out);
                    d2_SetViewEnabled(w->win, 1);
                    break;
                }
            }
            if (r != 0) {
                r = 1;
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_SeancePick, 0x677c7) != 0);
    return r;
}

void gm_SendMoneyDelta(int a, int b);

#define SEANCE_HELD 0x800000  /* the bit of AICHAR flags1cc a seance sets (cm_SendSetField(id, 0x1cc, 4, bit, 0)); the name is a reasoned guess */
/* 0x586610, the graveyard seance (the contact ai_GraveyardSeanceContact): rec = the graveyard. The player's character's grave
   entry (gv_FindEntry with gv_EntrySameId); none: the message box txt 0x40d5 and out. Else the detail window
   record gets the entry, its size and its index in that list, the line txt 0x4107 (the size's name; formatted,
   not shown) and the detail window opens with the title txt 0x40cc; the pick of the dead person
   (ai_SeancePick) fills the seance choice. A seance chosen: the player's character gets the flag 0x800000 of
   flags1cc (cm_SendSetField), its price is taken off (gm_SendMoneyDelta), then the kind runs: 1 the ghost, 2 the
   gift, 3 the sacrifice. The camera follows again */
void ai_GraveSeance(AIBLD *rec)
{
    GVSEANCE out;
    GVDETAILWIN w;
    char line[0x100];
    GVENTRY *e;

    e = (GVENTRY *)gv_FindEntry(ai_Chars[ai_PlayerId], (unsigned char *)rec, 0, gv_EntrySameId, 0xff);
    if (e != 0) {
        w.entry = e;
        w.size = e->size;
        w.index = gv_EntryIndex((char *)rec->yard, (int *)e);
        txt_Format(line, 0x4107, ai_GraveNames[e->size]);
        if (ai_GraveDetailOpen((int *)&w, txt_Ptrs[0x40cc]) == 0) {
            sv_CameraFollow();
            return;
        }
        e = (GVENTRY *)ai_SeancePick((char *)rec, &w, &out);
        d2_CloseView(w.win);
        if (e != 0) {
            cm_SendSetField(((AICHAR *)ai_Chars[ai_PlayerId])->netId, 0x1cc, 4, SEANCE_HELD, 0);
            if (out.cost != 0)
                gm_SendMoneyDelta(((AICHAR *)ai_Chars[ai_PlayerId])->netId, -out.cost);
            switch (out.kind) {
            case 1:
                ai_SeanceGhost(rec, &out);
                break;
            case 2:
                ai_SeanceGift(rec, &out);
                break;
            case 3:
                ai_SeanceSacrifice(rec, &out);
                break;
            }
        }
        sv_CameraFollow();
    } else {
        sv_MessageBox(txt_Ptrs[0x40d5], 0);
    }
}

void ai_EndCharExtra(char *ch, int f); int cm_SendRelations(int a, int b, int c, int d); int sv_StartPoseScript(char *a, int b);

/* 0x5867c0, the graveyard "Schlag" action (114 B): when the character under the pointer (ai_PickedChar) exists
   with the at-work flag bit 0 of +0x1b4 and the left button (dm_L.lup) is latched: ends its extra (ai_EndCharExtra,
   0), sends the player character's relation change towards it (-5, 0), then starts its role pose
   "locations/friedhof/schlag_FRIEDHOF.esc" (sv_StartPoseScript) and marks that script to run in its own scene */
void ai_GraveyardSchlag(char *unused)
{
    AICHAR *c;
    EVTSCRIPT *script;

    c = (AICHAR *)ai_PickedChar;
    if (c != 0 && (c->flags1b4 & 1) != 0 && dm_L.lup != 0) {
        ai_EndCharExtra((char *)c, 0);
        c = (AICHAR *)ai_PickedChar;
        cm_SendRelations(ai_CharInts[ai_PlayerId][0], c->netId, -5, 0);
        script = (EVTSCRIPT *)sv_StartPoseScript((char *)ai_PickedChar, "locations/friedhof/schlag_FRIEDHOF.esc");
        if (script != 0)
            script->ownScene = 1;
    }
}

void sv_ReleaseNamePlates(void); void ai_TradeContactChecks(void);

/* 0x586840, the graveyard contact dispatcher: rec = the graveyard. Each frame (pump 0x67fff) the idle chatter
   (ai_GraveyardChatter); with the building permission 0x200 the contacts ALTERN_LASSEN, HEIMSUCHUNG,
   ZUTATEN_SUCHEN, GRAEBER_PFLEGEN, GEHILFEN_ERSCHAFFEN, VERSICHERUNG, LIEGEPLAN and TRANSPORT, always GRAB_MIETEN.
   A clicked contact: with 0x200 the "Schlag" script (ai_GraveyardSchlag) first, then that contact's handler;
   GRAB_MIETEN needs the permission bit 0x80. No click: with gm_AllowAll the keys 0x1e / 0x2f / 0x22 / 0x23 /
   0x19 / 0x16 / 0x11 open the handlers directly (a cheat path, a reasoned guess) */
void ai_GraveyardContact(char *rec)
{
    int c_altern, c_heim, c_zut, c_graeber, c_gehilfen, c_vers, c_liege, c_trans, c_mieten;

    c_altern = 0;
    c_heim = 0;
    c_liege = 0;
    c_zut = 0;
    c_gehilfen = 0;
    c_graeber = 0;
    c_vers = 0;
    c_trans = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_GraveyardContact, 0x67fff) != 0) {
        ai_GraveyardChatter(rec);
        if (ai_BuildingPerms & 0x200) {
            c_altern = sv_RegisterNamePlate("contact_ALTERN_LASSEN", txt_Ptrs[0x40a0], 0x2f);
            c_heim = sv_RegisterNamePlate("contact_HEIMSUCHUNG", txt_Ptrs[0x4094], 0x29);
            c_zut = sv_RegisterNamePlate("contact_ZUTATEN_SUCHEN", txt_Ptrs[0x4127], 0x13);
            c_graeber = sv_RegisterNamePlate("contact_GRAEBER_PFLEGEN", txt_Ptrs[0x411d], 0x2a);
            c_gehilfen = sv_RegisterNamePlate("contact_GEHILFEN_ERSCHAFFEN", txt_Ptrs[0x4130], 0x28);
            c_vers = sv_RegisterNamePlate("contact_VERSICHERUNG", txt_Ptrs[0x40f4], 0x2b);
            c_liege = sv_RegisterNamePlate("contact_LIEGEPLAN", txt_Ptrs[0x4104], 0x13);
            c_trans = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
        }
        c_mieten = sv_RegisterNamePlate("contact_GRAB_MIETEN", txt_Ptrs[0x4105], 0x16);
        if (ai_ClickedObject != 0) {
            if (ai_BuildingPerms & 0x200) {
                ai_GraveyardSchlag(rec);
                if (ai_ClickedObject == c_altern)
                    ai_GraveAgeContact((AIBLD *)rec);
                else if (ai_ClickedObject == c_heim)
                    ai_GraveHauntContact((AIBLD *)rec);
                else if (ai_ClickedObject == c_zut)
                    ai_GypsyHelperWindow((AIBLD *)rec);
                else if (ai_ClickedObject == c_graeber)
                    ai_GraveCareContact(rec);
                else if (ai_ClickedObject == c_gehilfen)
                    ai_GraveHelperContact(rec);
                else if (ai_ClickedObject == c_vers)
                    ai_GraveInsuranceContact((AIBLD *)rec);
                else if (ai_ClickedObject == c_liege)
                    ai_GravePlanContact(rec);
                else if (ai_ClickedObject == c_trans)
                    ai_OpenTransport(rec);
            }
            if ((ai_BuildingPerms & 0x80) && ai_ClickedObject == c_mieten)
                ai_GraveRentContact((AIBLD *)rec);
        } else if (gm_AllowAll != 0) {
            if (kb_Last == 0x1e)
                ai_GraveAgeContact((AIBLD *)rec);
            else if (kb_Last == 0x2f)
                ai_GraveInsuranceContact((AIBLD *)rec);
            else if (kb_Last == 0x22)
                ai_GraveFogWindow((AIBLD *)rec);
            else if (kb_Last == 0x23)
                ai_GraveHauntContact((AIBLD *)rec);
            else if (kb_Last == 0x19)
                ai_GraveCareContact(rec);
            else if (kb_Last == 0x16)
                ai_GraveRentContact((AIBLD *)rec);
            else if (kb_Last == 0x11)
                ai_GraveHelperContact(rec);
        }
        ai_TradeContactChecks();
    }
}

int ai_AmountMenu(); int sv_CheckMoney();

/* 0x586ab0, the graveyard contact HEIMSUCHUNG (the haunting) for the graveyard rec: the target is picked in the
   amount menu (AMOUNTCFG flags 1, mode 0x400, kind 6, no ids left out; title txt 0x4095), none -> the camera
   follows again. The player bar, the form "Locations\Friedhof\Friedhof" with the text 0x4098 / 0x4097 (the
   target's id twice) and its slider (4 .. 0xc, step 4), the two buttons from the text 0x4099 on (pay / leave).
   Pay: the slider's amount in money (sv_CheckMoney) and the first selected character (the one who haunts) are
   needed: none selected -> the message box txt 0x4093; else the message 0x88 goes to the graveyard's owner
   (flags 2, the game time, the haunter's netId, the target's, the amount, the target's own building /
   building, else ai_PickActionBuilding's pick), its slot is waited for and the window leaves with the message
   box txt 0x409a. The right button or the second button leaves; then the player bar and the selection close
   and the camera follows again */
void ai_GraveHauntContact(AIBLD *rec)
{
    AIMSG msg;
    AMOUNTCFG q;
    AICHAR *p;
    AICHAR *f;
    int win;
    int slider;
    int btnPay;
    int btnLeave;
    int k;
    int n;
    int poor;
    int i;
    int id;
    int b;
    LARGE_INTEGER mask;

    memset(&q, 0, sizeof(q));
    memset(q.ids, 0xff, sizeof(q.ids));
    q.flags = 1;
    q.mode = 0x400;
    q.kind = 6;
    p = (AICHAR *)ai_AmountMenu(&q, txt_Ptrs[0x4095], 0, 0);
    if (p == 0) {
        sv_CameraFollow();
        return;
    }
    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    slider = d2_GetWinObj(win, 1, io_PrintText((char *)0x4098, 0x4097, p->id, p->id));
    d2_SetForm(win, 2);
    k = io_PrintText((char *)0x4099);
    btnPay = d2_GetWinObj(win, 2, k);
    btnLeave = d2_GetWinObj(win, 2, k + 1);
    d2_SetObjValue4(slider, 4, 0xc, 4);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                poor = 0;
                n = d2_GetObjValue(slider);
                if (sv_CheckMoney(ai_Chars[ai_PlayerId], n) == 0)
                    poor = 1;
                f = 0;
                for (i = 0; i < 0x300; i++) {
                    if (ai_CharsT[i].selected) {
                        f = &ai_CharsT[i];
                        break;
                    }
                }
                if (f == 0) {
                    sv_MessageBox(txt_Ptrs[0x4093], 0);
                } else if (poor == 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x88;
                    msg.flags = 2;
                    msg.arg[3] = n;
                    msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                    msg.fromId = rec->id;
                    msg.time = ai_Time;
                    msg.arg[1] = p->netId;
                    msg.arg[2] = f->netId;
                    msg.ids[0] = f->netId;
                    if (p->bld2 != 0)
                        msg.arg[0] = p->bld2->id;
                    else {
                        if (p->bld != 0)
                            b = p->bld->id;
                        else {
                            int pick;

                            mask.LowPart = 0x7b4c4a0;
                            mask.HighPart = 0;
                            pick = -1;
                            ai_PickActionBuilding((struct AIPER *)ai_Chars[ai_PlayerId], 0, mask.QuadPart, 0, 100.0f, &pick);
                            b = pick;
                        }
                        msg.arg[0] = b;
                    }
                    id = cm_SendLongMessage((char *)&msg);
                    while (id != -1 && cm_GetSlotState(id) == 0)
                        sv_RunFrame(0, 0x20007);
                    sv_FrameMask = 0x677c7;
                    sv_PopRequest = 1;
                    sv_MessageBox(txt_Ptrs[0x409a], 0);
                }
            } else if (d2_PointerObj == btnLeave) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_GraveHauntContact, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClosePlayerBar();
    sv_ClearSelection();
    sv_CameraFollow();
}
/* 0x586e40, the graveyard contact ALTERN_LASSEN (let age), the sibling of ai_GraveHauntContact, for the graveyard
   rec: the target is picked in the amount menu (AMOUNTCFG flags 1, mode 0x400, kind 6, no ids left out; title txt
   0x40a1), none -> the camera follows again. The player bar, the form "Locations\Friedhof\Friedhof" with the text
   0x40a4 / 0x40a3 (the target's id, the price 4), the two buttons from the text 0x40a5 on (pay / leave). Pay: 4
   in money (sv_CheckMoney) and the first selected character are needed: none selected -> the message box txt
   0x4093; else the message 0x89 goes to the graveyard's owner (flags 1, the game time, the selected character's
   netId, the target's, 4, the target's own building / building, else ai_PickActionBuilding's pick), its slot is
   waited for, the message box txt 0x40a7 (the selected character's id) and the window leaves. The right button or
   the second button leaves; then the player bar and the selection close and the camera follows again */
void ai_GraveAgeContact(AIBLD *rec)
{
    AMOUNTCFG q;
    char line[0x200];
    AICHAR *p;
    AICHAR *f;
    int win;
    int btnPay;
    int btnLeave;
    int k;
    int poor;
    int i;
    int id;
    LARGE_INTEGER mask;

    memset(&q, 0, sizeof(q));
    memset(q.ids, 0xff, sizeof(q.ids));
    q.flags = 1;
    q.mode = 0x400;
    q.kind = 6;
    p = (AICHAR *)ai_AmountMenu(&q, txt_Ptrs[0x40a1], 0, 0);
    if (p == 0) {
        sv_CameraFollow();
        return;
    }
    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText((char *)0x40a4, 0x40a3, p->id, 4);
    d2_SetForm(win, 2);
    k = io_PrintText((char *)0x40a5);
    btnPay = d2_GetWinObj(win, 2, k);
    btnLeave = d2_GetWinObj(win, 2, k + 1);
    d2_ClickedId = -1;
    do {
        AIMSG msg;

        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                poor = 0;
                f = 0;
                if (sv_CheckMoney(ai_Chars[ai_PlayerId], 4) == 0)
                    poor = 1;
                for (i = 0; i < 0x300; i++) {
                    if (ai_CharsT[i].selected) {
                        f = &ai_CharsT[i];
                        break;
                    }
                }
                if (poor == 0) {
                    if (f == 0) {
                        sv_MessageBox(txt_Ptrs[0x4093], 0);
                    } else {
                        memset(&msg, 0, sizeof(msg));
                        msg.type = 0x89;
                        msg.flags = 1;
                        msg.arg[3] = 4;
                        msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                        msg.fromId = rec->id;
                        msg.time = ai_Time;
                        msg.arg[2] = f->netId;
                        msg.ids[0] = f->netId;
                        msg.arg[1] = p->netId;
                        if (p->bld2 != 0)
                            msg.arg[0] = p->bld2->id;
                        else if (p->bld != 0)
                            msg.arg[0] = p->bld->id;
                        else {
                            int pick;

                            mask.LowPart = 0x7b4c4a0;
                            mask.HighPart = 0;
                            pick = -1;
                            ai_PickActionBuilding((struct AIPER *)ai_Chars[ai_PlayerId], 0, mask.QuadPart, 0, 100.0f, &pick);
                            msg.arg[0] = pick;
                        }
                        id = cm_SendLongMessage((char *)&msg);
                        while (id != -1 && cm_GetSlotState(id) == 0)
                            sv_RunFrame(0, 0x20007);
                        sv_FrameMask = 0x677c7;
                        txt_Format(line, 0x40a7, f->id);
                        sv_MessageBox(line, 0);
                        sv_PopRequest = 1;
                    }
                }
            } else if (d2_PointerObj == btnLeave) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_GraveAgeContact, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClosePlayerBar();
    sv_ClearSelection();
    sv_CameraFollow();
}
/* 0x5871c0, the graveyard "GRAB_MIETEN" contact (30 B): opens the grave window (ai_GraveWindow: no output, rec,
   the title text 0x4104, flag 0, no callback), then follows the camera again (sv_CameraFollow) */

void ai_GravePlanContact(char *rec)
{
    ai_GraveWindow(0, rec, txt_Ptrs[0x4104], 0, 0);
    sv_CameraFollow();
}


/* 0x5871e0, the graveyard contact GRAB_MIETEN (rent a grave) for the graveyard rec: the grave entry of the player's
   character (gv_FindEntry with gv_EntrySameId). None: without a reserved entry elsewhere (gv_FindReservedEntry)
   the rent-a-grave window (ai_RentGraveSelect) and the camera follows again, else the message box txt 0x42f8.
   With the entry: the detail window record gets the entry, its size and its index in that list, the line txt
   0x4107 (the size's name) is the detail window's title; opened, its two buttons run (ai_GraveDetailButtons),
   it closes and the camera follows again */
void ai_GraveRentContact(AIBLD *rec)
{
    GVENTRY *e;
    unsigned char *bld;
    GVDETAILWIN w;
    char line[0x100];

    e = (GVENTRY *)gv_FindEntry(ai_Chars[ai_PlayerId], (unsigned char *)rec, 0, gv_EntrySameId, 0xff);
    if (e != 0) {
        w.entry = e;
        w.size = e->size;
        w.index = gv_EntryIndex((char *)rec->yard, (int *)e);
        txt_Format(line, 0x4107, ai_GraveNames[e->size]);
        if (ai_GraveDetailOpen((int *)&w, line) == 0)
            return;
        ai_GraveDetailButtons((char *)rec, (int *)&w);
        d2_CloseView(w.win);
        sv_CameraFollow();
    } else if (gv_FindReservedEntry(ai_Chars[ai_PlayerId], &bld, (int *)&e) == 0) {
        ai_RentGraveSelect((char *)rec);
        sv_CameraFollow();
    } else {
        sv_MessageBox(txt_Ptrs[0x42f8], 0);
    }
}
/* 0x587300, the graveyard "GRAEBER_PFLEGEN" contact (50 B): when no character of the graveyard's staff shows the
   at-work flag (sv_CheckCharacterFlag, 0), opens the grave window (ai_GraveWindow: no output, rec, the title text
   0x411e, flag 1 = the detail's slider picks, the callback ai_GraveCare) and follows the camera again
   (sv_CameraFollow) */

void ai_GraveCareContact(char *rec)
{
    if (sv_CheckCharacterFlag(rec) != 0)
        return;
    ai_GraveWindow(0, rec, txt_Ptrs[0x411e], 1, (void (*)(AIBLD *, GVPICK *))ai_GraveCare);
    sv_CameraFollow();
}
/* 0x587340, the graveyard "GEHILFEN_ERSCHAFFEN" contact (47 B): when no character of the graveyard's staff shows
   the at-work flag (sv_CheckCharacterFlag, 0), opens the grave window (ai_GraveWindow: no output, rec, the title
   text 0x4131, flag 0 = a dead person is picked, the callback ai_GraveRaiseHelper) and follows the camera again
   (sv_CameraFollow) */

void ai_GraveHelperContact(char *rec)
{
    if (sv_CheckCharacterFlag(rec) == 0)
        ai_GraveWindow(0, rec, txt_Ptrs[0x4131], 0, (void (*)(AIBLD *, GVPICK *))ai_GraveRaiseHelper);
    sv_CameraFollow();
}
/* 0x587370, the graveyard contact VERSICHERUNG (the insurance) for the graveyard rec: the player bar, the form
   "Locations\Friedhof\Friedhof" with the text 0x40f6 / 0x40f5 (the price 0x228), the two buttons from the text
   0x40f7 on (take / leave). Take: the first selected character is needed: none -> the message box txt 0x4093;
   else the message 0x8a goes to the graveyard's owner (flags 1, the game time, the character's netId) and the
   window leaves. The right button or the second button leaves; then the player bar and the selection close and
   the camera follows again */
void ai_GraveInsuranceContact(AIBLD *rec)
{
    AIMSG msg;
    AICHAR *f;
    int win;
    int btnTake;
    int btnLeave;
    int i;

    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Friedhof");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText((char *)0x40f6, 0x40f5, 0x228);
    d2_SetForm(win, 2);
    i = io_PrintText((char *)0x40f7);
    btnTake = d2_GetWinObj(win, 2, i);
    btnLeave = d2_GetWinObj(win, 2, i + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnTake) {
                f = 0;
                for (i = 0; i < 0x300; i++) {
                    if (ai_CharsT[i].selected) {
                        f = &ai_CharsT[i];
                        break;
                    }
                }
                if (f == 0) {
                    sv_MessageBox(txt_Ptrs[0x4093], 0);
                } else {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x8a;
                    msg.flags = 1;
                    msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                    msg.fromId = rec->id;
                    msg.time = ai_Time;
                    msg.arg[0] = f->netId;
                    msg.ids[0] = f->netId;
                    cm_SendLongMessage((char *)&msg);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == btnLeave) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_GraveInsuranceContact, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClosePlayerBar();
    sv_ClearSelection();
    sv_CameraFollow();
}

/* 0x587560, the graveyard trade-contact dispatcher (317 B): rec = the building record. Releases the name plates
   (sv_ReleaseNamePlates); each frame (sv_RunFrame, pump 0x67fff) the graveyard chatter (ai_GraveyardChatter); with
   the building permission 0x200 the five contact plates are registered (sv_RegisterNamePlate): the graveyard
   chapel's production ("contact_PRODUKTION_FRIEDHOFSKAPELLE", text 0x4155, look 0x13), the transport
   ("contact_TRANSPORT", ai_ContactTextTransport, 0x15), the master letter ("ob_MEISTERBRIEF",
   ai_ContactTextMasterLetter, 0xc), the personnel book ("ob_GEHILFENBUCH", ai_ContactTextPersonnelBook, 0xc) and
   the storage ("contact_LAGER", ai_ContactTextStorage, 0xe); then the "Schlag" action (ai_GraveyardSchlag). A
   clicked plate (ai_ClickedObject) opens its window: ai_ProductionWindow, ai_OpenTransport, ai_MasterLetterWindow,
   ai_HelperBook (with the plate's name) or ai_StorageWindow (with the building's object list) */
void ai_GraveyardTradeContact(char *rec)
{
    int c0, c1, c2, c3, c4;

    c0 = 0;
    c1 = 0;
    c2 = 0;
    c3 = 0;
    c4 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_GraveyardTradeContact, 0x67fff) != 0) {
        ai_GraveyardChatter(rec);
        if (ai_BuildingPerms & 0x200) {
            c0 = sv_RegisterNamePlate("contact_PRODUKTION_FRIEDHOFSKAPELLE", txt_Ptrs[0x4155], 0x13);
            c1 = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            c2 = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0xc);
            c3 = sv_RegisterNamePlate("ob_GEHILFENBUCH", ai_ContactTextPersonnelBook, 0xc);
            c4 = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            ai_GraveyardSchlag(rec);
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == c0)
                ai_ProductionWindow((struct AIBLD *)rec);
            else if (ai_ClickedObject == c1)
                ai_OpenTransport((int)rec);
            else if (ai_ClickedObject == c2)
                ai_MasterLetterWindow((struct AIBLD *)rec);
            else if (ai_ClickedObject == c3)
                ai_HelperBook((struct AIBLD *)rec, "ob_GEHILFENBUCH");
            else if (ai_ClickedObject == c4)
                ai_StorageWindow((int)rec, ((AIBLD *)rec)->objList);
        }
    }
}
/* 0x5876a0, the graveyard seance contact dispatcher (95 B): rec = the building record. Each frame (sv_RunFrame,
   pump 0x67fff) the "contact_SEANCE" name plate is registered (sv_RegisterNamePlate, text 0x4169, look 0x16); a
   click on it (ai_ClickedObject) opens the seance (ai_GraveSeance) */

void ai_GraveyardSeanceContact(char *rec)
{
    int c;

    while (sv_RunFrame(ai_GraveyardSeanceContact, 0x67fff) != 0) {
        c = sv_RegisterNamePlate("contact_SEANCE", txt_Ptrs[0x4169], 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == c)
            ai_GraveSeance((AIBLD *)rec);
    }
}
/* 0x587700, the confirm of the graveyard's worker upgrade (ai_UpgradeWorkerWindow) for the graveyard rec, the
   selected character ch and the skill sel (1 / 2 / 4): the question txt 0x413a (ch's id) in a yes / no message box;
   no -> 0. Yes: the message 0x8f from the player (flags 1, the game time, ch's netId, sel, the objId of the
   object gm_Query (rec->objList, 1, 0, 0x211) finds, a reasoned guess: the kind 0x211; -1 without one); 1 */
int ai_UpgradeWorkerSend(AIBLD *rec, AICHAR *ch, int sel)
{
    AIMSG msg;
    char line[0x400];
    AIOBJ *obj;

    txt_Format(line, 0x413a, ch->id);
    if (sv_MessageBox(line, 1) != 0) {
        obj = (AIOBJ *)gm_Query((void *)rec->objList, 1, 0, 0x211);
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x8f;
        msg.flags = 1;
        msg.to = ((AICHAR *)ai_Chars[ai_PlayerId])->netId;
        msg.fromId = rec->id;
        msg.time = ai_Time;
        if (obj != 0)
            msg.arg[2] = obj->objId;
        else
            msg.arg[2] = -1;
        msg.arg[0] = ch->netId;
        msg.arg[1] = sel;
        cm_SendLongMessage((char *)&msg);
        return 1;
    }
    return 0;
}

float gm_GetSkill(char *c, int skill); int gm_ClockSeason(); char * he_FindEvent(int n, ...);

/* 0x587810, the graveyard's worker upgrade window for the graveyard rec (the contact of ai_GraveyardFogContact): nothing
   while sv_CheckCharacterFlag (rec) holds. Outside the season 2 of the game clock (gm_ClockSeason) and without
   gm_AllowAll the message box txt 0x413b; with an upgrade already running (an event 0x8f of the building,
   he_FindEvent) the message box txt 0x413c; both: the camera follows again. Else the form
   "Locations\Friedhof\UpgradeWorker" with the title txt 0x4138, the three buttons from the text 0x4139 (0x228)
   on, and the player bar; each frame the first selected character (kept from the frame before when none is
   selected) enables the buttons of its skills 1 / 2 / 4 below 1.0 (gm_GetSkill); a click on one: without a
   character the message box txt 0x4093, else the confirm (ai_UpgradeWorkerSend) leaves the window. Then the
   player bar and the selection close and the camera follows again */
void ai_UpgradeWorkerWindow(AIBLD *rec)
{
    char line[0x400];
    AICHAR *f;
    int win;
    int btn1;
    int btn2;
    int btn4;
    int n;
    int i;
    int sel;

    f = 0;
    n = gm_ClockSeason(&ai_Time);
    if (sv_CheckCharacterFlag(rec) != 0)
        return;
    if (gm_AllowAll == 0 && n != 2) {
        txt_Format(line, 0x413b);
        sv_MessageBox(line, 4);
        sv_CameraFollow();
        return;
    }
    if (he_FindEvent(2, 0, 0x8f, 3, rec->id) != 0) {
        txt_Format(line, 0x413c);
        sv_MessageBox(line, 4);
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\UpgradeWorker");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText((char *)0x4138);
    d2_SetForm(win, 3);
    n = io_PrintText((char *)0x4139, 0x228);
    btn1 = d2_GetWinObj(win, 3, n);
    btn2 = d2_GetWinObj(win, 3, n + 1);
    btn4 = d2_GetWinObj(win, 3, n + 2);
    sv_OpenPlayerBar(rec, 0);
    while (sv_RunFrame(ai_UpgradeWorkerWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        for (i = 0; i < 0x300; i++) {
            if (ai_CharsT[i].selected) {
                f = &ai_CharsT[i];
                break;
            }
        }
        if (f != 0) {
            if (gm_GetSkill((char *)f, 1) >= 1.0)
                d2_SetObjEnabled(btn1, 0);
            else
                d2_SetObjEnabled(btn1, 1);
            if (gm_GetSkill((char *)f, 2) >= 1.0)
                d2_SetObjEnabled(btn2, 0);
            else
                d2_SetObjEnabled(btn2, 1);
            if (gm_GetSkill((char *)f, 4) >= 1.0)
                d2_SetObjEnabled(btn4, 0);
            else
                d2_SetObjEnabled(btn4, 1);
        }
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == btn1)
                sel = 1;
            else if (d2_PointerObj == btn2)
                sel = 2;
            else if (d2_PointerObj == btn4)
                sel = 4;
            else
                continue;
            if (f == 0)
                sv_MessageBox(txt_Ptrs[0x4093], 0);
            else if (ai_UpgradeWorkerSend(rec, f, sel) != 0)
                sv_PopRequest = 1;
        }
    }
    d2_CloseView(win);
    sv_ClosePlayerBar();
    sv_ClearSelection();
    sv_CameraFollow();
}
/* 0x587ad0, the graveyard fog room's contact dispatcher (152 B): rec = the building record. Releases the name
   plates (sv_ReleaseNamePlates); each frame (sv_RunFrame, pump 0x67fff) with the building permission 0x200 two
   plates are registered (sv_RegisterNamePlate): the helper upgrade ("contact_GEHILFEN_VERBESSERN", text 0x4137,
   look 0x28) and the fog of horror ("contact_NEBEL_DES_GRAUENS", text 0x40b4, look 0x13). A clicked plate
   (ai_ClickedObject) opens ai_UpgradeWorkerWindow or ai_GraveFogWindow (rec) */

void ai_GraveyardFogContact(char *rec)
{
    int c0, c1;

    c0 = 0;
    c1 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_GraveyardFogContact, 0x67fff) != 0) {
        if (ai_BuildingPerms & 0x200) {
            c0 = sv_RegisterNamePlate("contact_GEHILFEN_VERBESSERN", txt_Ptrs[0x4137], 0x28);
            c1 = sv_RegisterNamePlate("contact_NEBEL_DES_GRAUENS", txt_Ptrs[0x40b4], 0x13);
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == c0)
                ai_UpgradeWorkerWindow((AIBLD *)rec);
            else if (ai_ClickedObject == c1)
                ai_GraveFogWindow(rec);
        }
    }
}

int gm_BuildingJob(); 

/* 0x587b70, the permission bits of the class 0x1d buildings, the graveyard (201 B), called from f3gm.c 0x50359b
   (gm_PermissionBits): ch = the character, b = the building record, x = the picked object or 0. bldg = the
   building kind's record (ai_BTypes + kind * 0x24d); v = the job of the character in the kind
   (gm_TitleJob, gm_BuildingJob). The mask 0x81, 0x10bd when the building's masterId is the character; x of the
   prototypes 0x211 / 0x205 clears bit 0. When the building's owner is the character: 0x4461 set and, when one of
   the six job bytes at bldg + 0x223 is v (a 0 byte ends the scan), 0x2200; x of the prototype 0x21b -> 0x5000.
   gm_AllowAll set: -1 */
int ai_PermissionBits1d(char *ch, char *b, char *x)
{
    AIBTYPE *bldg;
    int v;
    int f;
    int i;

    bldg = &((AIBTYPE *)ai_BTypes)[((AIBLD *)b)->type];
    v = gm_TitleJobI(((AICHAR *)ch)->id, gm_BuildingJob((unsigned char)bldg->kind));
    f = 0x81;
    if (((AIBLD *)b)->masterId == ((AICHAR *)ch)->id)
        f = 0x10bd;
    if (x != 0 && (((ORDOBJ *)x)->type == 0x211 || ((ORDOBJ *)x)->type == 0x205))
        f &= ~1;
    if (((AIBLD *)b)->owner == ((AICHAR *)ch)->id) {
        f |= 0x4461;
        for (i = 0; i < 6; i++) {
            if ((&bldg->jobBytes)[i] == 0)
                break;
            if ((&bldg->jobBytes)[i] == v) {
                f |= 0x2200;
                break;
            }
        }
    }
    if (x != 0 && ((ORDOBJ *)x)->type == 0x21b)
        f |= 0x5000;
    if (gm_AllowAll != 0)
        return -1;
    return f;
}
/* 0x587c40, ends the character ch's extra -- the "sp_<id>" role pose script that sv.c 0x52b490 / 0x52b500
   links back at the model +0x28 (the model at +0x184) / +0xc94 -- when one runs (0x4284a0 evt_FindById,
   0x429e90 evt_Stop), then sets the at-work flag bit 0 of +0x1b4 to f's bit 0 (f = 0: ends the pose only,
   the form the AI (ai_EndWorkerPose) and the graveyard "Schlag" action (0x5867c0, then 0x52b490 with the new
   pose) use) */


void ai_EndCharExtra(char *ch, int f)
{
    AICHAR *c = (AICHAR *)ch;
    EVTSCRIPT *script;

    if (c->model->scriptId != -1) {
        script = evt_FindById(c->model->scriptId);
        if (script != 0)
            evt_Stop(script);
    }
    c->flags1b4 ^= (c->flags1b4 ^ f) & 1;    /* bit 0 = f's bit 0 */
}
/* 0x587c90, the graveyard idle-chatter tick (the argument is not used): at most every 0x1194 ms of d3_Time
   (ai_GraveChatterAt the last run), and unless gm_AllowAll only with a 10% chance; collects the scene characters
   (ai_SceneChars) of the kind 0x14 whose model stands in d3_DefaultScene, picks one (x) and, with more than one,
   a second different one (y) at random, and runs the script "locations\friedhof\gespraeche.esc" with
   (x's model, a, b): a / b the speaker's line set, netId % 3, or 3 + (netId & 1) for sex 1; b = -1 without y */

void ai_GraveyardChatter(char *unused)
{
    AICHAR *list[0x20];
    int cnt;
    int n;
    int a;
    int b;
    AICHAR *x;
    AICHAR *y;
    AICHAR *c;
    int *slot;
    EVTSCRIPT *script;

    y = 0;
    cnt = 0;
    a = 0;
    b = -1;
    if ((unsigned)d3_Time < ai_GraveChatterAt + 0x1194)
        return;
    ai_GraveChatterAt = d3_Time;
    if (gm_AllowAll == 0 && util_RandomF() < 0.9f)
        return;
    for (slot = ai_SceneChars; (int)slot < (int)(ai_SceneChars + 0x20); slot++) {
        c = (AICHAR *)*slot;
        if (c != 0 && c->kind == 0x14 && c->model != 0 && (void *)c->model->scene == (void *)d3_DefaultScene)
            list[cnt++] = c;
    }
    if (cnt == 0)
        return;
    n = util_Random(cnt) & 0xffff;
    x = list[n];
    if (cnt > 1) {
        n = util_Random(cnt) & 0xffff;
        y = list[n];
        if (y == x) {
            n++;
            y = list[n % cnt];
        }
    }
    if (x != 0) {
        if (x->sex == 1)
            a = (x->netId & 1) + 3;
        else
            a = x->netId % 3;
    }
    if (y != 0) {
        if (y->sex == 1)
            b = (y->netId & 1) + 3;
        else
            b = y->netId % 3;
    }
    script = evt_Load("locations\\friedhof\\gespraeche.esc");
    if (script != 0)
        evt_RunScriptArgs(script, 3, (int)x->model, a, b);
}
/* 0x587de0, clears the 0x4000-byte table ai_TurnTable (17 B), called from main.c main_OpenEngine 0x532160 right after
   ai_InitStates; the table has no other reader or writer in the binary, so its role is the function's name */

void ai_ClearTurnTable(void)
{
    memset(ai_TurnTable, 0, sizeof ai_TurnTable);
}

/* 0x587e00, the right-button tooltip handler: while the right button holds a d2 object (d2_RightHeldObj) it shows
   the tooltip view ai_DragWindow for it. Releasing the button (or a picked object losing the right button) closes
   the view and puts the pointer back in place. A newly picked world object (ai_PickedObject, with the right
   button held) shows the hovered item (sv_HoverItem) or, without one, the object's help tooltip. A new held
   object is classified by its user record (D2OBJ.user): a character (ai_Chars), a building (ai_Blds: its type),
   a building type (ai_BTypes), an object (ai_Objects: its goods item) or a goods item (ai_Goods); without a user
   record the object id itself, 0xdf .. 0x426 a goods item, 0x427 .. 0x47e a building type. The old view is
   closed and the tooltip of the kind opened (items of the types 0x20 / 0x17 / 0x25 / 0x27 the item tooltip,
   others the upgrade tooltip); while a view is up it is raised and d2_CutsceneActive is kept set */
void ai_HandleItemDrag(void)
{
    int bkind;
    int item;
    char *sel;
    AICHAR *ch;
    void *user;
    AIGOOD *g;

    item = 0;
    bkind = 0;
    ch = 0;
    sel = 0;
    if ((d2_RightHeldObj == -1 && ai_DragObj != -1) ||
        (ai_DragPicked != 0 && dm_L.rhold == 0 && ai_DragWindow != -1)) {
        d2_CloseView(ai_DragWindow);
        ai_DragPicked = 0;
        ai_DragWindow = -1;
        ai_DragViewWin = -1;
        ai_DragObj = -1;
        sel = 0;  /* @original: a dead reset, stored to sel's frame slot (still 0 here) */
        di_Absolute = 1;
        di_SetMousePos(d2_MouseX, d2_MouseY);
    }
    if (ai_PickedObject != 0 && ai_DragPicked != ai_PickedObject && dm_L.rhold != 0) {
        item = sv_HoverItem != 0 ? ((AIOBJ *)sv_HoverItem)->id : 0;  /* reasoned: the hovered object record, its goods item first */
        if (item == 0)
            sel = ai_PickedObject;
        ai_DragPicked = ai_PickedObject;
        ai_DragObj = -1;
    }
    if (d2_RightHeldObj != -1 && d2_RightHeldObj != ai_DragObj) {
        user = d2_Objs[d2_PointerObj].user;
        if (user != 0) {
            if ((char *)user >= ai_Chars[0] && (char *)user < ai_Chars[768]) {  /* ai_Chars[768]: one past the table */
                ch = (AICHAR *)user;
                if (ch->id == 0xffff)
                    ch = 0;
            } else if ((char *)user >= ai_Blds && (char *)user < ai_Blds + 0xa900) {
                bkind = ((AIBLD *)user)->type;
                if (bkind >= 0x58)
                    bkind = 0;
            } else if ((char *)user >= ai_BTypes && (char *)user < ai_BTypes + 0x58 * 0x24d) {
                bkind = ((char *)user - ai_BTypes) / 0x24d;
            } else if ((char *)user >= (char *)ai_Objects && (char *)user < (char *)&ai_Objects[0x5008]) {  /* ai_Objects[0x5008]: one past the table */
                item = ((AIOBJ *)user)->id;
                if (item >= 0x2dc)
                    item = 0;
            } else if ((char *)user >= ai_Goods && (char *)user < ai_Goods + 0x2dc * 0x41) {
                item = ((char *)user - ai_Goods) / 0x41;
            }
        } else {
            if (d2_RightHeldObj >= 0xdf && d2_RightHeldObj < 0x427)
                item = d2_RightHeldObj - 0xdf;
            else if (d2_RightHeldObj >= 0x427 && d2_RightHeldObj < 0x47f)
                bkind = d2_RightHeldObj - 0x427;
        }
        ai_DragPicked = 0;
    }
    if (item != 0 || bkind != 0 || sel != 0 || ch != 0) {
        if (ai_DragWindow != -1) {
            d2_CloseView(ai_DragWindow);
            ai_DragWindow = -1;
            ai_DragViewWin = -1;
        }
        if (item != 0) {
            g = &((AIGOOD *)ai_Goods)[item];
            if (g->type == 0x20 || g->type == 0x17 || g->type == 0x25 || g->type == 0x27)
                ai_DragWindow = ai_OpenItemTooltip(item);
            else
                ai_DragWindow = ai_OpenUpgradeTooltip(item);
        } else if (bkind != 0) {
            ai_DragWindow = ai_OpenBuildingTooltip(bkind);
        } else if (sel != 0) {
            ai_DragWindow = ai_OpenHelpTooltip(sel);
        } else if (ch != 0) {
            ai_DragWindow = ai_OpenCharTooltip((char *)ch);
        }
        if (ai_DragWindow != -1) {
            ai_DragObj = d2_RightHeldObj;
            ai_DragViewWin = d2_Views[ai_DragWindow].wins[0];
            if (dm_L.rhold != 0)
                di_Absolute = 0;
        } else {
            ai_DragObj = -1;
            ai_DragPicked = 0;
            goto tail;
        }
    }
    if (ai_DragWindow != -1) {
        d2_CutsceneActive = 1;
        ai_DragCutscene = 1;
        d2_RaiseViewWindows(ai_DragWindow);
        return;
    }
tail:
    if (ai_DragCutscene != 0)
    {
        d2_CutsceneActive = 0;
        ai_DragCutscene = 0;
    }
}

int gm_BuildingKindCost(int c, int kind); char *_strupr(char *); int sprintf(char *buf, const char *fmt, ...);

/* 0x588160, opens the tooltip view of the building type idx (ai_BTypes): the form "ToolTip\ToolTip_Gebaeude",
   centred; section 1 the type's name (text idx * 0xe + 0x48e), for a kind other than 0xa through the text 0x27
   with the type's level as a roman numeral; section 2 the type's picture (object id idx + 0x427) and the text
   idx * 0xe + 0x48f; for a kind other than 0xa section 3 the level (text 0x2a), the cost for the player's
   character (gm_BuildingKindCost, text 0x28) and the value +0x243 (text 0x29); returns the view */
int ai_OpenBuildingTooltip(int idx)
{
    char roman[7][4] = { "", "I", "II", "III", "IV", "V", "VI" };
    AIBTYPE *bt;
    int view;

    bt = (AIBTYPE *)ai_BTypes + idx;
    view = d2_OpenForm(0, 0, "ToolTip\\ToolTip_Gebaeude");
    d2_CentreViewWindows(view);
    d2_SetForm(0, 1);
    if (bt->kind != 0xa)
        io_PrintText(0x27, idx * 0xe + 0x48e, roman[bt->cap247]);
    else
        io_PrintText(idx * 0xe + 0x48e);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, idx + 0x427, d2_LastWindow);
    io_PrintText(idx * 0xe + 0x48f);
    if (bt->kind != 0xa) {
        d2_SetForm(0, 3);
        io_PrintText(0x2a, roman[bt->cap247]);
        io_PrintText(0x28, gm_BuildingKindCost((int)ai_Chars[ai_PlayerId], idx));
        io_PrintText(0x29, bt->v243);
    }
    return view;
}
/* 0x588bb0, opens the help tooltip of the record named s: the key "_HILFE_<S>+0" (the name upper-cased) is
   looked up in the texts; without it -1 (no window). Else the form "ToolTip\ToolTip_Contact", centred, its
   section 1 the text's title line and section 2 the text after it; returns the window */

int ai_OpenHelpTooltip(char *s)
{
    char buf[0x180];
    int v;
    int win;

    strcpy(buf, s);
    _strupr(buf);
    sprintf(buf + 0x80, "_HILFE_%s+0", buf);   /* the key from the whole name, built at buf + 0x80 and looked up there (0x588bd8 .. 0x588bf6) */
    v = txt_FindKey(buf + 0x80);
    if (v == -1)
        return v;   /* -1: no window (the original returns txt_FindKey's -1, 0x588c06; 0 would be window 0) */
    win = d2_OpenForm(0, 0, "ToolTip\\ToolTip_Contact");
    d2_CentreViewWindows(win);
    d2_SetForm(0, 1);
    io_PrintText("$Z$[%s$]", v);
    d2_SetForm(0, 2);
    io_PrintText(v + 1);
    return win;
}

float gm_GoodsPrice(unsigned id, unsigned char p);

/* 0x588310, opens the item tooltip view of the goods item idx: the form "ToolTip\ToolTip_Objekte_Waffen" when
   the item has a weapon record (ai_FindWeaponRecord) with a nonzero id, else "ToolTip\ToolTip_Objekte_Handelsgut",
   centred and kept left of the screen's right edge; section 2 the item's picture (object id idx + 0xdf) and
   its description (text idx * 2 + 0x9a9), section 1 its name (idx * 2 + 0x9a8), section 3 the byte +0x40
   (text 0x21) or the player's price (text 0x1f); section 4 of the view the base price (text 0x20) or, in a
   building (ai_PlayerBuilding, ai_FallbackBuilding, sv_SelectedObject), the price over the building's staff
   factor (gm_StaffFactor of its workers other than the master, plus the master's skill byte / 1008, text 0x22);
   then the list: the goods it is made of with their amounts (section 4, text 0x25), the guild-house buildings
   that make it (gm_BuildingItemGroups, section 3, text 0x26), for the item types 0x17 / 0x25 the buildings that use the
   item idx and the extra slot 0x444 (text 0x624); "$C" when the list stayed empty; returns the view
   @deep reconstructed in full from 0x588310..0x58894b (453 vs 449 instructions): form choice, window
   clamp, sections 1..4, the price texts 0x21 / 0x1f (io_PrintText's third word is (int)gm_ItemPrice,
   gm_GoodsPrice takes two), the staff loop (ai_Chars bld == building, not the master, netIds) with
   gm_StaffFactor and the work time / (master skill / 1008 + factor), the ingredient loop (AIGOOD in/need),
   the gm_BuildingItemGroups rows, the building lists by item type 0x17/0x25 and the extra slot 0x444, every call
   and constant compared. What remains needs a variant round: (1) the 0x225..0x295 / 0x1c1.. if-else chain:
   the original lays the blocks inline after each test (0x5886df 0x48..0x4a, 0x588720 0x4e..0x50, 0x58875d
   0x18/0x19) and re-tests 0x226/0x227 at 0x5886cf, our VC6 threads every test to blocks placed after the
   chain (0x58881f, 0x5887e9) - tried no outer test, two ifs, ranges, rotated inner order, plain else:
   worse or equal; this placement moves all later branch targets (the 12 differing targets are this shift
   and the loop heads below); (2) slot * 0x16 + 0x28 is recomputed per call in the original (lea chains on
   edi / esi), ours strength-reduces it into ebx / edi, which moves the loop counters into registers (the
   original keeps them at [esp+0x10]); (3) fxch fxch at 0x58854a in the work-time division (tried operand
   orders and casts: unchanged); (4) the prologue pushes ebx/ebp at entry vs after the form call (follows
   from (2)) */
int ai_OpenItemTooltip(int idx)
{
    int n;
    int slot;
    int view;
    AIGOOD *item;
    float factor;
    char *w;
    char *bld;
    AICHAR *c;
    AICHAR *master;
    D2WIN *win;
    int i;
    int k;
    int id;
    int amount;

    w = ai_FindWeaponRecord(idx);
    item = (AIGOOD *)ai_Goods + idx;
    if (w != 0 && ((AIWEAPON *)w)->id != 0)
        view = d2_OpenForm(0, 0, "ToolTip\\ToolTip_Objekte_Waffen");
    else
        view = d2_OpenForm(0, 0, "ToolTip\\ToolTip_Objekte_Handelsgut");
    d2_CentreViewWindows(view);
    win = &d2_Wins[d2_Views[view].wins[0]];
    if (win->x + win->w > d2_ScreenW - 0x10)
        d2_MoveObj(d2_ScreenW - win->w - 0x10, win->y, win->parent);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, idx + 0xdf, d2_LastWindow);
    io_PrintText(idx * 2 + 0x9a9);
    d2_SetForm(0, 1);
    io_PrintText(idx * 2 + 0x9a8);
    d2_SetForm(0, 3);
    if (item->b40 != 0)
        io_PrintText(0x21, item->b40);
    else
        io_PrintText(0x1f, (int)gm_GoodsPrice(idx, ai_OwnPlayer), (int)gm_ItemPrice(idx, 0x64));
    bld = ai_PlayerBuilding;
    if (bld == 0) {
        bld = ai_FallbackBuilding;
        if (bld == 0)
            bld = sv_SelectedObject;
    }
    d2_SetForm(view, 4);
    if (bld == 0) {
        io_PrintText(0x20, item->workTime);
    } else {
        int list[32];

        n = 0;
        gm_BuildingSkill((unsigned char *)bld, 1, 1);
        master = (AICHAR *)gm_FindCharacterByMaster((int)bld);
        for (c = (AICHAR *)ai_Chars; c < (AICHAR *)ai_Chars[0x300]; c++) {
            if ((char *)c->bld == bld && c != master)
                list[n++] = c->netId;
        }
        factor = gm_StaffFactor((unsigned char *)bld, n, list);
        c = (AICHAR *)gm_FindCharacterByMaster((int)bld);
        if (c == 0)
            c = (AICHAR *)ai_Chars[((AIBLD *)bld)->owner];
        io_PrintText(0x22, (int)(item->workTime / (c->sk81 * (1.0 / 1008) + factor)), item->workTime);
    }
    d2_SetForm(0, 4);
    io_PrintText(0x25);
    slot = 0;
    for (k = 0; k < 4; k++) {
        id = item->in[k];
        amount = item->need[k];
        if (id != 0) {
            d2_WinOpenPicStyled(0, slot * 0x16 + 0x28, id + 0xdf, d2_LastWindow, -2);
            io_PrintText("%i %s$N", amount, id * 2 + 0x9a8);
            slot++;
        }
    }
    d2_SetForm(0, 3);
    io_PrintText(0x26);
    slot = 0;
    for (i = 0; i < 37; i++) {
        for (k = 0; k < 10; k++) {
            if (gm_BuildingItemGroups[i].items[k] == idx && idx != 0x1cd) {
                d2_WinOpenPicStyled(6, slot * 0x16 + 0x28, gm_BuildingItemGroups[i].key + 0x427, d2_LastWindow, -2);
                io_PrintText("%s$N", gm_BuildingItemGroups[i].key * 0xe + 0x48e);
                slot++;
            }
        }
    }
    n = 0;
    if (item->type == 0x17 || item->type == 0x25) {
        int list[32];

        if (idx == 0x225 || idx == 0x226 || idx == 0x227 || idx == 0x293 || idx == 0x294 || idx == 0x295) {
            if (idx == 0x225 || idx == 0x226 || idx == 0x227) {
                list[0] = 0x48;
                list[1] = 0x49;
                list[2] = 0x4a;
                n = 3;
            } else if (idx == 0x293 || idx == 0x294 || idx == 0x295) {
                list[0] = 0x4e;
                list[1] = 0x4f;
                list[2] = 0x50;
                n = 3;
            }
        } else if (idx == 0x1c1 || idx == 0x1c2 || idx == 0x1c3) {
            list[0] = 0x18;
            list[1] = 0x19;
            n = 2;
        } else if (idx != 0x1c7 && (idx == 0x1c4 || idx == 0x1c5 || idx == 0x1c6 || idx == 0x2af || idx == 0x2b0)) {
            list[0] = 0x1e;
            n = 1;
        } else if ((idx >= 0x1d0 && idx <= 0x1d2) || (idx >= 0x1bd && idx <= 0x1c0)) {
            list[0] = 0x20;
            n = 1;
        } else if ((idx >= 0x1b7 && idx <= 0x1b9) || (idx >= 0x1ca && idx <= 0x1cc)) {
            list[0] = 0x1e;
            n = 1;
        } else if ((idx >= 0x1cd && idx <= 0x1cf) || idx == 0x1ba || (idx >= 0x1bb && idx <= 0x1bc)) {
            list[0] = 0x1f;
            n = 1;
        }
    }
    for (k = 0; k < n; k++) {
        int list[32];

        id = list[k];
        d2_WinOpenPicStyled(6, slot * 0x16 + 0x28, id + 0x427, d2_LastWindow, -2);
        io_PrintText("%s$N", id * 0xe + 0x48e);
        slot++;
    }
    if (!(idx == 0x1c1 || idx == 0x1c2 || idx == 0x1c3 ||
          (idx != 0x1c7 && (idx == 0x1c4 || idx == 0x1c5 || idx == 0x1c6 || idx == 0x2af || idx == 0x2b0))) ||
        idx == 0x1c7) {
        if (item->type == 0x17 || item->type == 0x25) {
            d2_WinOpenPicStyled(6, slot * 0x16 + 0x28, 0x444, d2_LastWindow, -2);
            io_PrintText("%s$N", 0x624);
            slot++;
        }
    }
    if (slot == 0)
        io_PrintText("$C");
    return view;
}
/* 0x588950, opens the upgrade tooltip view of the goods item idx (-1 for an item of the type 0x1d): the form
   "ToolTip\ToolTip_Objekte_Upgrades", centred and kept left of the screen's right edge; section 2 the item's
   picture (object id idx + 0xdf) and description (text idx * 2 + 0x9a9), section 1 its name (idx * 2 + 0x9a8),
   section 3 the byte +0x40 (text 0x21) or the price scaled by the difficulty (0.5 + ai_Difficulty / 4, text
   0x23) and the work time (text 0x20). In a building (ai_PlayerBuilding, ai_FallbackBuilding, sv_SelectedObject)
   and for an item type other than 2 / 6: when the building's type lists the item among its objects, the
   object's effect (text 0x24) by its class: 3 "+%a" (text 0xf9e), 0x24 "+%i" (text 0xfe0), else "%i%%" (text
   class * 2 + 0xf98), with the object's value; returns the view */
int ai_OpenUpgradeTooltip(int idx)
{
    int view;
    AIGOOD *item;
    D2WIN *win;
    char *bld;
    AIBTYPE *bt;
    int i;
    int cls;
    int value;
    float f;

    if (((AIGOOD *)ai_Goods)[idx].type == 0x1d)
        return -1;
    view = d2_OpenForm(0, 0, "ToolTip\\ToolTip_Objekte_Upgrades");
    d2_CentreViewWindows(view);
    win = &d2_Wins[d2_Views[view].wins[0]];
    if (win->x + win->w > d2_ScreenW - 0x10)
        d2_MoveObj(d2_ScreenW - win->w - 0x10, win->y, win->parent);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, idx + 0xdf, d2_LastWindow);
    io_PrintText(idx * 2 + 0x9a9);
    d2_SetForm(0, 1);
    io_PrintText(idx * 2 + 0x9a8);
    item = (AIGOOD *)ai_Goods + idx;
    d2_SetForm(0, 3);
    if (item->b40 != 0)
        io_PrintText(0x21, item->b40);
    else {
        f = ai_Difficulty * 0.25 + 0.5;
        io_PrintText(0x23, (int)(gm_ItemPrice(idx, 0x64) * f));
    }
    io_PrintText(0x20, item->workTime);
    bld = ai_PlayerBuilding;
    if (bld == 0) {
        bld = ai_FallbackBuilding;
        if (bld == 0) {
            bld = sv_SelectedObject;
            if (bld == 0)
                return view;
        }
    }
    if (item->type == 2 || item->type == 6)
        return view;
    bt = (AIBTYPE *)ai_BTypes + ((AIBLD *)bld)->type;
    for (i = 0; i < 0x40; i++) {
        if ((bt->objs[i] & 0x7fff) == idx) {
            cls = bt->objClass[i];
            value = bt->objValue[i];
            if (cls == 0 || cls == -1 || cls == 0xff)
                return view;
            io_PrintText(0x24);
            if (cls == 3)
                io_PrintText("+%a %s$N", value, 0xf9e);
            else if (cls == 0x24)
                io_PrintText("+%i %s$N", value, 0xfe0);
            else
                io_PrintText("%i%% %s$N", value, cls * 2 + 0xf98);
            return view;
        }
    }
    return view;
}

int ai_AmountRowStatus(); int ai_DrawAmountRow(); short * gm_FindChar(int id); int gm_TitlesLeft();

/* 0x588c60, opens the tooltip view of the character ch: the form "ToolTip\ToolTip_Personen", centred; section 1
   the name, section 4 the character's amount row (ai_AmountRowStatus, drawn by ai_DrawAmountRow when it
   answers), section 2 the age (not for the kind 0x14), the title (text 0x2c with gm_TitlesLeft) and the rank /
   offices (text 0x2d: the rank of the status bits 0x200 .. 0x2000 of +0x1cc, the offices officeKind /
   secondOffice), the bits 0x3000 clear: the line 0x37 (+0x00d) and the partner (text 0x2f: the spouse
   relatives[0] alive 0x30, a pair event 0x41 for it 0x31, else 0x32); the worth (0x2e), the byte +0x00c
   (text 0x33), the bits 0x3000 clear: the children relatives[3 .. 7] (text 0x34, none 0x35); section 3 the
   five skill bars (sv_DrawBalanceBar, texts 0x16b7 ..); then ai_CampaignStep (7, ch); returns the view */
int ai_OpenCharTooltip(char *ch)
{
    int view;
    int row[14];                        /* an amount row (menu.c GYPSYROW): row[0] the character */
    char line[0x100];
    AICHAR *c;
    AICHAR *p;
    AIJOB *ev;
    int rank;
    int any;
    int i;

    c = (AICHAR *)ch;
    view = d2_OpenForm(0, 0, "ToolTip\\ToolTip_Personen");
    d2_CentreViewWindows(view);
    d2_SetForm(0, 1);
    io_PrintText("$Z$[%1N3$]", c->id);
    d2_SetForm(0, 4);
    row[0] = (int)c;
    if (ai_AmountRowStatus(row) != 0)
        ai_DrawAmountRow(0xa6, 5, d2_LastWindow, row);
    d2_SetForm(0, 2);
    if (c->kind != 0x14)
        io_PrintText(0x2b, (int)gm_CharAge(c->id));
    if (c->officeKind == 0 && c->secondOffice == 0 && (c->flags1cc & 0xe00) == 0) {
        if ((c->flags1cc & 0x3000) != 0) {
            if (c->flags1cc & 0x1000)
                rank = 0x26;
            else if (c->flags1cc & 0x2000)
                rank = 0x27;
            else if (c->flags1cc & 0x800)
                rank = 0x25;
            else if (c->flags1cc & 0x400)
                rank = 0x24;
            else
                rank = (c->flags1cc & 0x200) ? 0x23 : 0;
            io_PrintText(0x2d);
            io_PrintText("$3>%s$A", (c->sex ? 0x27b : 0x253) + rank);
            io_PrintText(cm_TextNewline);
        } else {
            io_PrintText(0x2c, (c->sex ? 0x18f : 0x130) + c->jobId, gm_TitlesLeft(c->jobId));
        }
    } else {
        if (c->flags1cc & 0x1000)
            rank = 0x26;
        else if (c->flags1cc & 0x2000)
            rank = 0x27;
        else if (c->flags1cc & 0x800)
            rank = 0x25;
        else if (c->flags1cc & 0x400)
            rank = 0x24;
        else
            rank = (c->flags1cc & 0x200) ? 0x23 : 0;
        io_PrintText(0x2c, (c->sex ? 0x18f : 0x130) + c->jobId, gm_TitlesLeft(c->jobId));
        io_PrintText(0x2d);
        if (c->flags1cc & 0xe00)
            io_PrintText("$3>%s$A", (c->sex ? 0x27b : 0x253) + rank);
        if (c->officeKind != 0)
            io_PrintText("$3>%s$A", (c->sex ? 0x27b : 0x253) + c->officeKind);
        if (c->secondOffice != 0)
            io_PrintText("$3>%s$A", (c->sex ? 0x27b : 0x253) + c->secondOffice);
    }
    if ((c->flags1cc & 0x3000) == 0)
        io_PrintText(0x37, (c->sex ? 0x11e : 0x114) + c->b00d);
    io_PrintText(0x2e, gm_GetWorth(c->id));
    if ((c->flags1cc & 0x3000) == 0) {
        io_PrintText(0x2f);
        p = (AICHAR *)gm_FindChar(c->relatives[0]);
        if (p != 0 && p->health > 0) {
            io_PrintText(0x30, p->id);
        } else {
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x41); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                if (ev->partner1 == c->netId)
                    break;
            }
            if (ev != 0 && (p = (AICHAR *)gm_FindChar(ev->partner2)) != 0)
                io_PrintText(0x31, p->id);
            else
                io_PrintText(0x32);
        }
    }
    io_PrintText(0x33, c->b00c + 0x486);
    if ((c->flags1cc & 0x3000) == 0) {
        any = 0;
        io_PrintText(0x34);
        for (i = 0; i < 5; i++) {
            p = (AICHAR *)gm_FindChar(c->relatives[3 + i]);
            if (p != 0) {
                io_PrintText("$3>%1N1$A", p->id);
                any = 1;
            }
        }
        if (any == 0)
            io_PrintText("$3>%s$A", 0x35);
    }
    d2_SetForm(0, 3);
    for (i = 0; i < 5; i++) {
        txt_Format(line, (int)"%s:", i + 0x16b7);
        d2_CreateWinText(0, i * 15, line, d2_LastWindow);
        sv_DrawBalanceBar(0x64, i * 15 + 2, ch, i, 0x4df);
    }
    ai_CampaignStep(7, (int)ch, 0, 0);
    return view;
}
/* 0x5890c0, the modal info window of the character ch (kinds below 0xa only): the form
   "ToolTip\ToolTip_Personen" laid out as ai_OpenCharTooltip does without its rank lines - section 1 the name,
   section 4 the amount row, section 2 the age (not for the kind 0x14), the title, the offices (text 0x2d: none
   0x253, else officeKind / secondOffice), the line 0x37, the worth, the partner (0x2f .. 0x32), the byte +0x00c
   (0x33) and the children (0x34, none 0x35); section 3 the five skill bars; then the frame pump (the right
   button closes it) and the view closed */
void ai_CharInfoWindow(char *ch)
{
    int view;
    int row[14];                        /* an amount row (menu.c GYPSYROW): row[0] the character */
    char line[0x100];
    AICHAR *c;
    AICHAR *p;
    AIJOB *ev;
    int any;
    int i;

    c = (AICHAR *)ch;
    if (c->kind >= 0xa)
        return;
    view = d2_OpenForm(0, 0, "ToolTip\\ToolTip_Personen");
    d2_CentreViewWindows(view);
    d2_SetForm(0, 1);
    io_PrintText("$Z$[%1N3$]", c->id);
    d2_SetForm(0, 4);
    memset(row, 0, sizeof(row));
    row[0] = (int)c;
    if (ai_AmountRowStatus(row) != 0)
        ai_DrawAmountRow(0xa6, 5, d2_LastWindow, row);
    d2_SetForm(0, 2);
    if (c->kind != 0x14)
        io_PrintText(0x2b, (int)gm_CharAge(c->id));
    io_PrintText(0x2c, (c->sex ? 0x18f : 0x130) + c->jobId, gm_TitlesLeft(c->jobId));
    io_PrintText(0x2d);
    if (c->officeKind == 0 && c->secondOffice == 0) {
        io_PrintText("$3>%s$A", 0x253);
    } else {
        if (c->officeKind != 0)
            io_PrintText("$3>%s$A", (c->sex ? 0x27b : 0x253) + c->officeKind);
        if (c->secondOffice != 0)
            io_PrintText("$3>%s$A", (c->sex ? 0x27b : 0x253) + c->secondOffice);
    }
    io_PrintText(0x37, (c->sex ? 0x11e : 0x114) + c->b00d);
    io_PrintText(0x2e, gm_GetWorth(c->id));
    io_PrintText(0x2f);
    p = (AICHAR *)gm_FindChar(c->relatives[0]);
    if (p != 0 && p->health > 0) {
        io_PrintText(0x30, p->id);
    } else {
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x41); ev != 0; ev = (AIJOB *)he_NextEvent()) {
            if (ev->partner1 == c->netId)
                break;
        }
        if (ev != 0 && (p = (AICHAR *)gm_FindChar(ev->partner2)) != 0)
            io_PrintText(0x31, p->id);
        else
            io_PrintText(0x32);
    }
    io_PrintText(0x33, c->b00c + 0x486);
    any = 0;
    io_PrintText(0x34);
    for (i = 0; i < 5; i++) {
        p = (AICHAR *)gm_FindChar(c->relatives[3 + i]);
        if (p != 0) {
            io_PrintText("$3>%1N1$A", p->id);
            any = 1;
        }
    }
    if (any == 0)
        io_PrintText("$3>%s$A", 0x35);
    d2_SetForm(0, 3);
    for (i = 0; i < 5; i++) {
        txt_Format(line, (int)"%s:", i + 0x16b7);
        d2_CreateWinText(0, i * 15, line, d2_LastWindow);
        sv_DrawBalanceBar(0x64, i * 15 + 2, ch, i, 0x4df);
    }
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_CharInfoWindow, 0x677c7));
    sv_ResetScreenClip();
    d2_CloseView(view);
}
/* 0x589400, clears the bookmark b (14 B): its building and object = -1 */

void ai_ClearBookmark(AIBOOKMARK *b)
{
    b->building = -1;
    b->object = -1;
}
/* 0x589410, the init of the bookmark table ai_Bookmarks (0xb slots of 12 bytes, up to 0x6f5f44): cleared, then
   each slot's key = i + 0x3b (0x3b .. 0x44: F1 .. F10) or 0x57 (F11) and the slot cleared by 0x589400 */

void ai_InitBookmarks(void)
{
    int i;

    memset(ai_Bookmarks, 0, sizeof(ai_Bookmarks));
    for (i = 0; i < 0xb; i++) {
        ai_Bookmarks[i].key = (unsigned char)(i + 0x3b);
        if (i == 0xa)
            ai_Bookmarks[10].key = 0x57;
        ai_ClearBookmark(&ai_Bookmarks[i]);
    }
}

char * gm_FindBuildingById(int id); int gm_FindObjectInst(int id); unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c);

/* 0x589450, drops the stale bookmarks (113 B): every set one (building not -1) whose building is gone
   (gm_FindBuildingById), whose object is set but gone (gm_FindObjectInst) or whose permission for the own player
   lacks bit 1 (gm_PermissionBits) is cleared (ai_ClearBookmark) */
void ai_ValidateBookmarks(void)
{
    char *b;
    int e;
    int i;

    for (i = 0; i < 11; i++) {
        if (ai_Bookmarks[i].building == -1)
            continue;
        b = gm_FindBuildingById(ai_Bookmarks[i].building);
        if (b == 0) {
            ai_ClearBookmark(&ai_Bookmarks[i]);
        } else {
            e = gm_FindObjectInst(ai_Bookmarks[i].object);
            if (e == 0 && ai_Bookmarks[i].object != -1)
                ai_ClearBookmark(&ai_Bookmarks[i]);
            else if ((gm_PermissionBits(ai_PlayerId, (unsigned char *)b, (unsigned short *)e, 0) & 1) == 0)
                ai_ClearBookmark(&ai_Bookmarks[i]);
        }
    }
}

/* 0x5894d0, the bookmark keys (151 B): on a key (kb_Last) without the left shift held (kb_Down[0x2a]), the
   first bookmark of that key still set after ai_ValidateBookmarks selects its building and object (or -1) in
   the panel (sv_SelectPanelObject) */
void ai_SelectBookmark(void)
{
    char *b;
    AIOBJ *o;
    int i;

    if (kb_Last == 0 || kb_Down[0x2a] != 0)
        return;
    for (i = 0; i < 11; i++) {
        if (ai_Bookmarks[i].key == kb_Last && kb_Down[0x2a] == 0) {
            ai_ValidateBookmarks();
            if (ai_Bookmarks[i].building != -1) {
                b = gm_FindBuildingById(ai_Bookmarks[i].building);
                o = (AIOBJ *)gm_FindObjectInst(ai_Bookmarks[i].object);
                if (o != 0)
                    sv_SelectPanelObject((unsigned char *)b, o->objId, ch_EmptyName);
                else
                    sv_SelectPanelObject((unsigned char *)b, -1, ch_EmptyName);
                return;
            }
        }
    }
}
/* 0x589570, sets the bookmark idx (256 B): the building the player is in (ai_PlayerBuilding, else
   ai_FallbackBuilding with its shown room or the first room gm_Query finds, else the selected object or the
   market) and the room object shown (sv_PanelShown); the status bar line "%s = %G, %s" (the key's text idx +
   0x205e, the building, the room's text) or, without a room or for the room 0xfd, "%s = %G" */
void ai_SetBookmark(int idx)
{
    char buf[0x100];
    AIBLD *a;
    AIOBJ *o;

    a = (AIBLD *)ai_PlayerBuilding;
    if (a != 0) {
        if (sv_PanelShown != 0)
            o = (AIOBJ *)sv_PanelShown;
        else
            o = 0;
    } else {
        a = (AIBLD *)ai_FallbackBuilding;
        if (a != 0) {
            if (sv_PanelShown != 0)
                o = (AIOBJ *)sv_PanelShown;
            else
                o = (AIOBJ *)gm_Query((void *)a->objList, 2, 6, 0, 0xfd);
        } else {
            a = (AIBLD *)sv_SelectedObject;
            if (a == 0)
                a = ai_MarketBuilding;
            o = 0;
        }
    }
    if (o != 0) {
        ai_Bookmarks[idx].building = a->id;
        ai_Bookmarks[idx].object = o->objId;
    } else {
        ai_Bookmarks[idx].building = a->id;
        ai_Bookmarks[idx].object = -1;
    }
    if (o != 0 && o->id != 0xfd)
        txt_Format(buf, (int)"%s = %G, %s", idx + 0x205e, a, o->id * 2 + 0x9a8);
    else
        txt_Format(buf, (int)"%s = %G", idx + 0x205e, a);
    sv_SetStatusText(buf);
}

int sv_CreateScrollButtons(); void d2_RadioClear(int g);

/* 0x589670, the bookmark window (1066 B): the form "special\geb_hotkeys" (ai_ValidateBookmarks first), the title
   text 0x205b, the scroll buttons of section 1 and the button row of section 3 (texts 0x206b / 0x206a: set,
   clear); section 1 is rebuilt whenever a bookmark changed: a radio group of the 11 key pictures (0x769 + i),
   each bookmark's building (its picture 0x427 + type, "%2N8~ %1G" with its owner) or the text 0x205d, and its
   room (picture 0xdf + id, its name) unless none or 0xfd. The right button, F12 (0x58) or escape close it; the
   set button stores the current building in the selected bookmark (ai_SetBookmark), the clear button clears it;
   the bookmark keys work while it is open (ai_SelectBookmark) */
void ai_BookmarkWindow(void)
{
    char line[0x100];
    int buttons[2];
    int scroll[3];
    int view;
    int group;
    int sel;
    int rebuild;
    int i;
    int t;
    AIBLD *b;
    AIOBJ *o;

    group = -1;
    sel = -1;
    rebuild = 1;
    ai_ValidateBookmarks();
    view = d2_OpenForm(0, 0, "special\\geb_hotkeys");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(view, 2);
    io_PrintText(0x205b);
    d2_SetForm(view, 0);
    sv_CreateScrollButtons(0x214, 0x168, d2_LastWindow, d2_GetViewWin(view, 1), 0x38, &scroll[0], &scroll[1], &scroll[2]);
    d2_SetForm(view, 3);
    sv_ButtonRow(d2_LastWindow, 0, buttons, 2, txt_Ptrs[0x206b], txt_Ptrs[0x206a]);
    d2_ClearInput();
    do {
        if (rebuild) {
            d2_SetForm(view, 1);
            if (group != -1) {
                sel = d2_Radios[group].sel;
                d2_RadioClear(group);
            }
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            group = d2_CreateRadioGroup(0);
            d2_Radios[group].flags = 1;
            for (i = 0; i < 11; i++) {
                b = 0;
                o = 0;
                d2_RadioAdd(group, d2_WinOpenPic(0x10, i * 0x38 + 4, i + 0x769, d2_LastWindow));
                if (ai_Bookmarks[i].building != -1)
                    b = (AIBLD *)gm_FindBuildingById(ai_Bookmarks[i].building);
                if (ai_Bookmarks[i].object != -1)
                    o = (AIOBJ *)gm_FindObjectInst(ai_Bookmarks[i].object);
                if (b != 0) {
                    d2_WinOpenPic(0x60, i * 0x38, b->type + 0x427, d2_LastWindow);
                    txt_Format(line, (int)"%2N8~ %1G", b->owner, b);
                    t = d2_CreateWinText(0x9c, i * 0x38 + 9, line, d2_LastWindow);
                    d2_Objs[t].w = 0x80;
                    d2_Objs[t].font = 0x54;
                } else {
                    t = d2_CreateWinText(0x60, i * 0x38 + 9, txt_Ptrs[0x205d], d2_LastWindow);
                    d2_Objs[t].font = 0x53;
                }
                if (o != 0 && o->id != 0xfd) {
                    d2_WinOpenPic(0x128, i * 0x38, o->id + 0xdf, d2_LastWindow);
                    txt_Format(line, (int)"%1s", o->id * 2 + 0x9a8);
                    t = d2_CreateWinText(0x164, i * 0x38 + 0x12, line, d2_LastWindow);
                    d2_Objs[t].w = 0x80;
                    d2_Objs[t].font = 0x54;
                }
            }
            d2_RadioSelect(group, sel);
            rebuild = 0;
        }
        if (dm_L.rup != 0 || kb_Last == 0x58 || kb_Last == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == buttons[1] && d2_Radios[group].sel != -1) {
                ai_ClearBookmark(&ai_Bookmarks[d2_Radios[group].sel]);
                rebuild = 1;
            }
            if (d2_PointerObj == buttons[0] && d2_Radios[group].sel != -1) {
                ai_SetBookmark(d2_Radios[group].sel);
                rebuild = 1;
            }
        }
        d2_MenuNavigate(group);
        if (d2_Radios[group].sel != -1 && ai_Bookmarks[d2_Radios[group].sel].building != -1)
            d2_SetObjEnabled(buttons[1], 1);
        else
            d2_SetObjEnabled(buttons[1], 0);
        if (d2_Radios[group].sel == -1)
            d2_SetObjEnabled(buttons[0], 0);
        else
            d2_SetObjEnabled(buttons[0], 1);
        ai_SelectBookmark();
    } while (sv_RunFrame((int)ai_BookmarkWindow, 0x657c7));
    d2_RadioClear(group);
    d2_CloseView(view);
}

unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);

/* 0x589aa0, the save game's bookmarks step (142 B): writes the count 0xb, then each bookmark's key (1 byte),
   building and object (4 bytes each); 0 on a failed write, 1 */
int ai_SaveBookmarks(int fh)
{
    int n;
    int i;

    n = 0xb;
    if (vfs_fwrite(&n, 4, 1, fh) <= 0)
        return 0;
    for (i = 0; i < n; i++) {
        if (vfs_fwrite(&ai_Bookmarks[i].key, 1, 1, fh) <= 0)
            return 0;
        if (vfs_fwrite(&ai_Bookmarks[i].building, 4, 1, fh) <= 0)
            return 0;
        if (vfs_fwrite(&ai_Bookmarks[i].object, 4, 1, fh) <= 0)
            return 0;
    }
    return 1;
}

int ls_Read();

/* 0x589b30, the save game's bookmarks reader, ai_SaveBookmarks's counterpart (136 B): reads the count, then
   that many bookmarks' key (1 byte), building and object (4 bytes each) through ls_Read; 0 on a failed read, 1 */
int ai_LoadBookmarks(int fh)
{
    int n;
    int i;

    n = 0;
    if (ls_Read(&n, 4, 1, fh) == 0)
        return 0;
    for (i = 0; i < n; i++) {
        if (ls_Read(&ai_Bookmarks[i].key, 1, 1, fh) == 0)
            return 0;
        if (ls_Read(&ai_Bookmarks[i].building, 4, 1, fh) == 0)
            return 0;
        if (ls_Read(&ai_Bookmarks[i].object, 4, 1, fh) == 0)
            return 0;
    }
    return 1;
}

void * mem_Alloc(int size, char *tag); char dd_Lock(void); void dd_Unlock(); unsigned char bmp_Save24(char *path, int w, int h, unsigned char *data); void mem_Free(void *p);

/* 0x589bc0, takes a screenshot (198 B): clears the status line and draws a frame (sv_RunFrame with no handler),
   then copies the screen surface (dd_Lock, d2_Convert16ToRgb) into a 24-bit image and saves it as
   gamedata/screenshots/gilde%04i.bmp with the next number of ai_ScreenshotCount; the status line text 0x42fd */
void ai_SaveScreenshot(void)
{
    char name[0x80];
    unsigned char *img;
    int n;

    sv_SetStatusText(ch_EmptyName);
    sv_RunFrame(0, sv_FrameMask);
    n = ai_ScreenshotCount++;
    sprintf(name, "gamedata/screenshots/gilde%04i.bmp", n);
    img = (unsigned char *)mem_Alloc(d2_ScreenW * d2_ScreenH * 3, "f3_sv_t:d");
    dd_Lock();
    d2_Convert16ToRgb(0, 0, d2_ScreenW, d2_ScreenH, dd_Surface, 0, 0, img);
    dd_Unlock();
    bmp_Save24(name, d2_ScreenW, d2_ScreenH, img);
    mem_Free(img);
    sv_SetStatusText(txt_Ptrs[0x42fd]);
}
/* 0x589c90, the bookmark keys each frame: nothing on no key or while the panel flag is 1; F12 (0x58) opens the bookmark
   window, a bare function key selects its bookmark (ai_SelectBookmark), shift + a function key stores the current
   view in that bookmark (ai_SetBookmark) */

void ai_SlotKeys(void)
{
    int i;

    if (kb_Last == 0)
        return;
    if (sv_PanelFlag == 1)
        return;
    if (kb_Last == 0x58)
        ai_BookmarkWindow();
    ai_SelectBookmark();
    for (i = 0; i < 11; i++) {
        if (ai_Bookmarks[i].key == kb_Last && kb_Down[0x2a] != 0)
            ai_SetBookmark(i);
    }
}

void ai_Quicksave(void); void ai_ApplyOptions(); void cm_RaiseSpeedLevel(void); void cm_LowerSpeedLevel(void);

/* 0x589cf0, the speed keys each frame (264 B): the game speed ai_OptionSpeed in steps of 40, v = (int)(speed / 40 +
   0.5f); kb_Last 0 = none and the panel flag sv_PanelFlag == 1 return at once; the key 0x1f takes a screenshot
   (0x589bc0), the key 0x10 runs ai_Quicksave while d2_FocusedInput is 0 and sv_FrameMask has no 0x10000; then the
   ai_GameFlags byte: bit 2 (the network game) - with bit 0x10 (the host) the keys 0x4e / 0x4a only raise / lower
   cm_SpeedLevel (cm_RaiseSpeedLevel / cm_LowerSpeedLevel), without it they show the status text txt_SpeedHostOnly;
   without bit 2 they raise / lower and step the speed between 0 and 4 * 40 by 40, then ai_ApplyOptions applies it
   (kb_Last is read once more after each call) */
void ai_SpeedKeys(void)
{
    unsigned char key;
    unsigned char fl;
    int v;

    v = (int)((float)(ai_OptionSpeed / 40) + 0.5f);
    key = kb_Last;
    if (key == 0)
        return;
    if (sv_PanelFlag == 1)
        return;
    if (key == 0x1f) {
        ai_SaveScreenshot();
        key = kb_Last;
    }
    if (key == 0x10 && d2_FocusedInput == 0 && (sv_FrameMask & 0x10000) == 0) {
        ai_Quicksave();
        key = kb_Last;
    }
    fl = ai_GameFlagsB;
    if ((fl & 4) == 0) {
        if (key == 0x4e) {
            cm_RaiseSpeedLevel();
            if (v < 4) {
                v++;
                ai_OptionSpeed = v * 40;
                ai_ApplyOptions();
            }
            key = kb_Last;
        }
        if (key == 0x4a) {
            cm_LowerSpeedLevel();
            if (0 < v) {
                ai_OptionSpeed = v * 40 - 40;
                ai_ApplyOptions();
                return;
            }
        }
    } else if ((fl & 0x10) != 0) {
        if (key == 0x4e) {
            cm_RaiseSpeedLevel();
            return;
        }
        if (key == 0x4a) {
            cm_LowerSpeedLevel();
            return;
        }
    } else {
        if (key == 0x4e || key == 0x4a)
            sv_SetStatusText(txt_SpeedHostOnly);
    }
}

int gm_BuildingMainObject(unsigned char *b); void *gm_GetGebaeudeHauptraum(unsigned char *b);

/* 0x589e00, fills bookmark slot i from the building b (79 B): the slot's building = b->id, and its object = the id
   (AIOBJ.objId) of the building's main object (gm_BuildingMainObject, else gm_GetGebaeudeHauptraum; none for the
   kind 0x1d), -1 without an object */
void ai_BookmarkSetBuilding(int i, AIBLD *b)
{
    AIOBJ *o;

    o = 0;
    if (b->type != 0x1d) {
        o = (AIOBJ *)gm_BuildingMainObject((unsigned char *)b);
        if (o == 0)
            o = (AIOBJ *)gm_GetGebaeudeHauptraum((unsigned char *)b);
    }
    ai_Bookmarks[i].building = b->id;
    if (o != 0)
        ai_Bookmarks[i].object = o->objId;
    else
        ai_Bookmarks[i].object = -1;
}

unsigned char * gm_BuildNext(void); unsigned char * gm_BuildQuery(int n, ...);

/* 0x589e50, presets the function-key bookmarks: F1 the market, F2 the town hall (gm_BuildQuery (1, 5, 0xf), the
   building type 0xf) when there is one, the next key the home (AICHAR.bld2) of the first character of the kind 6,
   then from F4 on every building the player's character owns (key 4) whose type's kind is not 2 */
void ai_BookmarksPreset(void)
{
    AIBLD *b;
    int i;
    int n;

    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].kind == 6)
            break;
    }
    n = 1;
    ai_BookmarkSetBuilding(0, ai_MarketBuilding);
    b = (AIBLD *)gm_BuildQuery(1, 5, 0xf);
    if (b != 0)
        ai_BookmarkSetBuilding(n++, b);
    b = ai_CharsT[i].bld2;
    if (b != 0)
        ai_BookmarkSetBuilding(n, b);
    n = 3;
    b = (AIBLD *)gm_BuildQuery(1, 4, i);
    while (b != 0) {
        if (((AIBTYPE *)ai_BTypes)[b->type].kind != 2)
            ai_BookmarkSetBuilding(n++, b);
        b = (AIBLD *)gm_BuildNext();
    }
}
