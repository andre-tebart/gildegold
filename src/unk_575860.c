/* unk_575860.c -- the tutorial module ("FT*" record names, tutorial\* forms, TUTORIAL_* texts), 0x575860..0x589f10:
   the tutorial state machine (state record ai_Tutorial = the pointer 0x5eec60, states 1 .. 0xc, the jump table
   0x575fe8; ai_TutorialStart builds the run chain ai_TutorialRuns and initializes it, the tick 0x575860 runs it),
   the chapter run-record builders (ai_TutorialChapterInit) with their step tables (TUTSTEP) and position-marker
   paths (TUTMARK), the marker update, the chapter chooser, then the location / contact / tooltip windows (the
   alchemist's candidates and hire from 0x57bf00 on). Start: he.c's last function 0x575710, its jump table and
   nop fill end at 0x575860; the unit's one .data block starts at 0x62918c (ai_TutorialChapterOn, then the "FT"
   state names) and its literals climb in function order to 0x62aa58; the network lobby (0x589f10) is a unit of
   its own. The original file name is unknown (no leaked tag). Names are ours.
   Open: type debt - TUTSTATE / TUTRUN / TUTSTEP / TUTMARK are typed in unk_575860.h and the tutorial code up
   to ai_TutorialWindow uses them; the later functions still reach the state record through the S / U_I
   byte-offset macros; the message (AIMSG) and contact-list records are still accessed by byte offset; the cannon scene
   struct (CANNONSCENE, CANNONREC) is typed from ai_CannonScene to ai_CannonExplosion, ai_CannonAxes /
   ai_CannonPitchRatio / ai_CannonBuildPanel still address it by byte offset.
   Open: data: the unit's .rdata block 0x5eec60..0x5eed70 is typed up to ai_CannonZoomRange (0x5eec6c .. 0x5eecac:
   const data, the original loads it); 0x5eec64 / 0x5eec68 / 0x5eec8c / 0x5eec90 (0x54, the recoil time the code
   uses as an immediate) / the floats 0x5eec9c .. 0x5eeca8 and the tables from 0x5eecb0 are not declared yet.
   Open: sv_CheckGoodAmount's third parameter: the original's callers push the player byte unextended (a char
   parameter, kept as the cast macro sv_CheckGoodAmountB), sv.h / sv.c declare it unsigned and mask it.
   Open: ai_ClockMs is volatile in the original (ai_TutorialTick reloads it after every read, byte-matched through
   the unit's view ai_ClockMsVolatile); its owner and the other units declare it plain int.
   Open: the alchemist windows ai_AlchemistWindow / ai_AlchemistResearchWindow are @deep (allocation); their buttons call
   ai_AlchemistFundWindow / ai_AlchemistRescheduleWindow with (obj, record), the record parameter unread.
   Open: not included yet (dcp headers conflicts unk_575860 HEADER): d2.h gfx.h sv.h; held by own declarations that
   differ from the owner's: d2_CreateWinText; calls whose argument count differs from the definition (read the
   original's pushes): d2_GetObjValue, sv_DrawBalanceBar (d2_PlaceMainWindow goes through d2_PlaceMainWindowF).
   unit of rea 8cef7d5b (src/game/unk_575860.c); boundaries in decl/text.dcl */
struct AHMSTEP;
struct D3SKY;
#include "d2_types.h"
extern D2RADIO d2_Radios[];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unk_575860.h"
#include "txt.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#include "ai.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern const int sv_PhaseMinutes[24];
extern int sv_PanelFlag;
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
extern int sv_FrameDepth;


extern GAPDATA full_data_624ee0[];
#include "f3init_types.h"
extern GAPDATA full_data_6024f8[];
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_62a248[];



extern GAPDATA full_data_62a284[];
extern GAPDATA full_data_62a2a4[];
extern unsigned char ai_TimeHour;
extern int d2_PointerObj;
extern GAPDATA full_data_62a2c4[];
extern const int sv_PhaseMinutes[24];
extern int d2_DirtyRectLeft;
extern int d2_DirtyRectTop;
extern int d2_DirtyRectRight;
extern int d2_DirtyRectBottom;
extern char * sv_CameraObj;
extern struct D2OBJ *d2_Objs;
extern float d3s_QuadUV[6];
extern GAPDATA full_data_5fea38[];
extern GAPDATA full_data_62a2dc[];
extern GAPDATA full_data_62a2fc[];
extern GAPDATA full_data_62a310[];
extern GAPDATA full_data_62a328[];
extern GAPDATA full_data_62a340[];
extern GAPDATA full_data_62a350[];
extern GAPDATA full_data_6fa060[];
extern int sv_PanelFlag;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;
extern GAPDATA full_data_602568[];


extern GAPDATA full_data_602568[];
extern int ai_ClockMs;
extern int sv_PopRequest;
extern GAPDATA full_data_5f36e0[];
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern struct D2PIC *d2_Pics;
extern short d2_ScreenH;
extern short d2_ScreenW;
extern D2WIN d2_Wins[];










extern GAPDATA full_data_62a38c[];
extern GAPDATA full_data_62a3ac[];
extern GAPDATA full_data_62a3cc[];

extern unsigned char ai_TimeHour;
extern int d2_PointerObj;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;




extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern int sv_PopRequest;
extern unsigned char ai_TimeHour;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;





extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern int sv_PopRequest;
extern unsigned char ai_TimeHour;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;





extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern unsigned char ai_TimeHour;
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
extern int ai_BuildingPerms;

extern GAPDATA full_data_62a4bc[];
extern int sv_PopRequest;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;




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
extern int ai_ClockMs;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a578[];
extern GAPDATA full_data_62a580[];
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern struct D2PIC *d2_Pics;
extern GAPDATA full_data_62a53c[];
extern GAPDATA full_data_62a544[];
extern GAPDATA full_data_62a54c[];
extern GAPDATA full_data_62a564[];
extern GAPDATA full_data_62a56c[];
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
extern unsigned char ai_TimeHour;



extern GAPDATA full_data_62a5bc[];
extern int sv_PopRequest;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a5f8[];
extern GAPDATA full_data_62a62c[];
extern unsigned char ai_TimeHour;

extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

extern GAPDATA full_data_62a634[];
extern int sv_FrameMask;
extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern unsigned char ai_TimeHour;
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
extern int ai_ClockMs;
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
extern unsigned char ai_TimeHour;
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
extern char ai_Time[14];
extern unsigned char ai_TimeHour;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;



extern GAPDATA full_data_62a5bc[];
extern int sv_FrameMask;
extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern char ai_Time[14];
extern unsigned char ai_TimeHour;
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
extern char ai_Time[14];
extern unsigned char ai_TimeHour;
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
extern char ai_Time[14];
extern unsigned char ai_TimeHour;
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
extern int ai_ClockMs;
extern GAPDATA full_data_62a538[];
extern GAPDATA full_data_62a8a8[];
extern GAPDATA full_data_6f5f60[];
extern char ai_Chars[768][0x21c];
extern int d2_PointerObj;
extern int ai_DragWindow;
extern short d2_MouseX;
extern short d2_MouseY;
extern struct D2OBJ *d2_Objs;
extern int d2_Unkf8aac8;
extern D2VIEW d2_Views[];
extern int di_Absolute;
extern GAPDATA full_data_62a8d0[];
extern GAPDATA full_data_62a8d4[];
extern int sv_HoverItem;  /* 0x6f22f0 (sv.c) */
extern GAPDATA full_data_6f266c[];
extern GAPDATA full_data_6f2670[];
extern int gm_PlayerRecords[688];
extern unsigned short ai_PlayerId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a8d8[];
extern GAPDATA full_data_62a8f4[];
extern GAPDATA full_data_62a8f8[];
extern GAPDATA full_data_62a8fc[];
extern GAPDATA full_data_62a8fe[];
extern GAPDATA full_data_62a900[];
extern GAPDATA full_data_62a902[];
extern GAPDATA full_data_62a904[];
extern GAPDATA full_data_62a906[];
extern GAPDATA full_data_62a908[];
extern GAPDATA full_data_62a9a8[];
extern GAPDATA full_data_62a9b4[];
extern GAPDATA full_data_62a9cc[];
extern unsigned char ai_OwnPlayer;
extern const float ai_Unk5ee160[476];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a91c[];
extern GAPDATA full_data_62a940[];
extern GAPDATA full_data_62a960[];
extern GAPDATA full_data_62a96c[];
extern GAPDATA full_data_62a978[];
extern GAPDATA full_data_62a984[];
extern GAPDATA full_data_62a56c[];
extern GAPDATA full_data_62a9e4[];
extern GAPDATA full_data_62a9ec[];
extern GAPDATA full_data_62a9f8[];
extern int sv_PopRequest;
extern GAPDATA full_data_6f5ec0[];
extern GAPDATA full_data_6f5f38[];
extern GAPDATA full_data_6f5ec8[];
extern GAPDATA full_data_6f5f4c[];
extern GAPDATA full_data_6f5ec4[];
extern GAPDATA full_data_6f5f48[];
extern unsigned char kb_Last;
extern GAPDATA full_data_62aa14[];
extern GAPDATA full_data_62aa20[];
extern unsigned short * sv_PanelShown;

extern int d2_LastWindow;

extern GAPDATA full_data_62aa38[];
extern GAPDATA full_data_6f5ec0[];
extern GAPDATA full_data_6f5ec4[];
extern int sv_PopRequest;
extern short d2_ScreenH;
extern short d2_ScreenW;

extern GAPDATA full_data_62aa4c[];
extern GAPDATA full_data_62aa58[];
extern GAPDATA full_data_6f2674[];
extern int sv_FrameMask;
extern GAPDATA full_data_6f5ec0[];
extern GAPDATA full_data_6f5f44[];
extern unsigned char kb_Last;
extern int sv_PanelFlag;

extern unsigned char kb_Last;
extern int sv_FrameMask;
extern int sv_PanelFlag;
extern GAPDATA full_data_6f5ec4[];
extern GAPDATA full_data_6f5ec8[];
extern char ai_Chars[768][0x21c];

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_PickActionBuilding)(p, a, fl, b, c, f, o)

#define U_I(a) (*(int *)(a))

void sv_CameraFollow();

void ai_TutorialHideText2();
void ai_TutorialProgressSlider();
int ai_TutorialShowRunText();
int ai_TutorialIntroAnswered();
int ai_TutorialStepForm();
int ai_TutorialStepClicked();
void ai_TutorialShowText2();
void sv_CameraFollow();
void sv_SetStatusText(char *s);
void sv_SelectPanelObject(unsigned char *o, int b, char *c);
void sv_BookForward();
void sv_BookBack();
void ai_TutorialShowText();
void ai_TutorialHideText();
struct SBSAMPLE;
extern int sv_FrameBase; /* 0x6f22a4 */
extern char *txt_Ptrs[]; /* 0x143a1c0 */

/* the state record is at 0x6fa060 (the 0x40 byte of main.c's 0x6fa020 block); 0x5eec60 is a const pointer to it
   in the original. A load through that pointer (not the fixed address) is what the unit's functions compile to,
   and the full link relocates the initializer, so the record is ours and no neighbour is touched. */
TUTSTATE *const ai_Tutorial = (TUTSTATE *)full_data_6fa060;  /* 0x5eec60, the pointer to the tutorial state record */
#define S (*(char **)(unsigned)&ai_Tutorial)  /* the tutorial state record (at 0x6fa060): the pointer ai_Tutorial holds */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is */
#define ai_TimeK (*(AHMCLOCK *)&ai_Time)  /* 0x770ae0, ai_Time as the messages here copy it: @original 3 ints and a short (ahm.h AHMCLOCK) */
#define ai_ClockMsVolatile (*(volatile int *)&ai_ClockMs)  /* 0x6333cc, @original: the byte-matched ai_TutorialTick reloads the clock after every read (a timer-written int; its owner declares it plain int) */
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
#define gm_ConvertMoney3 ((int (*)(int, unsigned char, int))gm_ConvertMoney)  /* @original: the calls at 0x57df2c / 0x57df3d push a third word (the previous result) that f3gm.c:7921 never reads (it takes a, k); no byte-matched caller, the evidence is the pushes in the original (the same 3-push shape is noted at hst.c ai_SplitTokenFormat); header unchanged because every matched caller passes two words */
int ai_TutorialChapterOn[5] = { 1, 1, 1, 1, 1 };  /* 0x62918c, per chapter 1 = played (the start window's check boxes, ai_TutorialWindow) */
int (*ai_TutorialChapterInit[5])(void) = { ai_TutorialInitChapter1, ai_TutorialInitChapter2, ai_TutorialInitChapter3, ai_TutorialInitChapter4, ai_TutorialInitChapter5 };  /* 0x6291a0, the chapter chain builders the start appends for every chapter played */
int ai_TutorialBanks[6] = { 0 };  /* 0x6f2640, the voice banks of the main intro / outro and the chapters 1 .. 5 (snd_LoadVoiceBank), unloaded by the stop */
TUTRUN *ai_TutorialRuns = 0;  /* 0x6f2658, the chain of the tutorial's run records (built by its start 0x578180, freed by its stop 0x578370) */
int ai_TutorialOn = 0;  /* 0x6f265c, the tutorial runs (set by its start 0x578180, cleared by its end) */
int ai_TutorialRunning = 0;  /* 0x6f2660, the tutorial's state machine advances (unk_SetEndGameGate / unk_ClearEndGameGate) */
int ai_ShotDirty = 0;  /* 0x6f2664, set when ai_ShotTableStore stored a record, cleared by the scene (ai_CannonShotResults) */
SHOTRESULT ai_ShotTable[3] = { 0 };  /* 0x6f9f60, the cannon scene's shot results (the callback ai_ShotTableStore fills it, ai_ShotTableReset resets it) */

#include "time_view.h"
#include "ls.h"
#include "f3init.h"
#include "ahm.h"
#include "ai.h"
#include "am.h"
#include "amt.h"
#include "anm.h"
#include "bmp.h"
#include "f3gm.h"
#define gm_TitleJobI ((int (*)(int, int))gm_TitleJob)  /* 0x5044b0 (f3gm.h: unsigned char): @original the byte-matched callers here read the result as an int (no `and eax,0xff` after the call): ai_Unk587b70 0x587b70 (shape) */
#include "f3snd.h"
#include "gv.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "sblib.h"
#include "tb.h"
#include "tr.h"
#include "unk_4b39f0.h"
#include "unk_58bbd0.h"
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
#include "unk_458810.h"
#include "dd_vesa.h"
#include "unk_5d33b0.h"
#define d2_PlaceMainWindowF ((void (*)(int, int))d2_PlaceMainWindow)  /* 0x475850 (d2.c: int view): @original the callers pass the placement flags too, which the original's tail jump leaves on the stack for d2_PlaceWindowScaled */
#define sv_SelectPanelObjectK ((int (*)())sv_SelectPanelObject)  /* 0x52c4c0?: @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
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
        ai_Tutorial->textAt = ai_ClockMsVolatile;
        ai_Tutorial->stepAt = ai_ClockMsVolatile;
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
        if (ai_ClockMsVolatile > ai_Tutorial->textAt + ai_Tutorial->run->text1Time) {
            ai_Tutorial->textAt = ai_ClockMsVolatile;
            if (ai_Tutorial->text1Shown == 0 && ai_Tutorial->run->text1) {
                ai_Tutorial->text1Shown = 1;
                ai_TutorialShowText();
            }
        } else if (ai_ClockMsVolatile > ai_Tutorial->textAt + 0x238) {
            if (ai_Tutorial->text1Shown != 1)
                return 0;
            ai_Tutorial->textAt = ai_ClockMsVolatile;
            ai_Tutorial->text1Shown = 0;
            ai_TutorialHideText();
        }
        if (ai_Tutorial->text1Shown != 1)
            return 0;
        d2_RaiseViewWindows(ai_Tutorial->text1Win);
        return 0;
    case 5:                     /* drop the first text window, open the first step */
        ai_Tutorial->textAt = ai_ClockMsVolatile;
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
        tb_Paused = ai_ClockMsVolatile >= ai_Tutorial->run->steps[ai_Tutorial->step].delay + ai_Tutorial->stepAt;
        d2_SetObjEnabled(ai_Tutorial->stepControl, tb_Paused);
        if (ai_TutorialStepClicked() != 1)
            return 0;
        ai_Tutorial->step++;
        ai_Tutorial->textAt = ai_ClockMsVolatile;
        if (ai_Tutorial->step < ai_Tutorial->run->stepCount) {
            ai_Tutorial->stepAt = ai_ClockMsVolatile;
            ai_TutorialStepForm();
            return 0;
        }
        ai_Tutorial->state = 7;
        ai_Tutorial->next = 7;
        return 0;
    case 7:                     /* the chain is done: the second text window or the tail */
        ai_Tutorial->phase = 3;
        ai_Tutorial->stepAt = ai_ClockMsVolatile;
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
            ai_Tutorial->textAt = ai_ClockMsVolatile;
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
            ai_Tutorial->textAt = ai_ClockMsVolatile;
            ai_Tutorial->text1Shown = 0;
            ai_TutorialHideText();
        }
        if (ai_ClockMsVolatile > ai_Tutorial->run->tailTime + ai_Tutorial->stepAt) {
            sv_CameraFollow();
            sv_PopRequest = sv_FrameDepth - sv_FrameBase;
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = 0xb;
            ai_Tutorial->stepAt = ai_ClockMsVolatile;
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
            ai_Tutorial->textAt = ai_ClockMsVolatile;
            ai_Tutorial->text1Shown = 0;
            ai_TutorialHideText();
        }
        if (ai_ClockMsVolatile > ai_Tutorial->run->tail2Time + ai_Tutorial->stepAt) {
            sv_CameraFollow();
            sv_PopRequest = sv_FrameDepth - sv_FrameBase;
            ai_Tutorial->state = 0xc;
            ai_Tutorial->next = 0xc;
            ai_Tutorial->stepAt = ai_ClockMsVolatile;
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
            ai_Tutorial->textAt = ai_ClockMsVolatile;
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
   handled events in campaignSteps
   @deep read in full against the original (0x576040..0x57633a) and rewritten on the typed TUTSTATE /
   TUTRUN: the mode 2 / other split, the stepHook calls, the timed goal, the phase 1 / phase 3 transitions
   with the shared 'started' (stepAt = clock) / 'count' tails the original jumps to (0x57631d / 0x576330),
   the volatile clock reads; 238/238 instructions, the rest regs / order. Left: one allocation hunk in
   phase 1, c == 0: ours keeps the record pointer in edx and the clock in ecx and reloads the pointer into
   eax before 'test ebx, ebx' ('- mov edx,[ai_Tutorial]; mov ecx,[clock]; mov [edx+0xc],ecx; mov
   eax,[ai_Tutorial]'), the original keeps it in eax across the store ('+ mov eax,[ai_Tutorial]; mov
   edx,[clock]; test ebx,ebx; mov [eax+0xc],edx') and so its b == 0 jump lands on the reload before the
   shared tail (je +0x1ca vs ours +0x1cf) */

void ai_CampaignStep(int a, int b, int c, int d)
{
    int p[4];

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
        if (ai_Tutorial->run->stepHook) {
            ai_Tutorial->run->stepHook(p);
            ai_Tutorial->campaignSteps++;
            return;
        }
        if (ai_Tutorial->run->goal == 3) {
            if (ai_ClockMsVolatile > ai_Tutorial->run->tailTime + ai_Tutorial->stepAt) {
                ai_Tutorial->state = ai_Tutorial->run->goalState;
                ai_Tutorial->next = ai_Tutorial->run->goalState;
                ai_Tutorial->stepAt = ai_ClockMsVolatile;
                ai_Tutorial->statusShown = 1;
            }
            return;
        }
        if (ai_Tutorial->run->goal != a)
            goto count;
        if (c == 0) {
            ai_Tutorial->state = ai_Tutorial->run->goalState;
            ai_Tutorial->next = ai_Tutorial->state;
        } else {
            ai_Tutorial->state = 0xb;
            ai_Tutorial->next = ai_Tutorial->state;
        }
        goto started;
    }
    if (ai_Tutorial->next != 4 && ai_Tutorial->next != 9 && ai_Tutorial->next != 0xa)
        return;
    if (ai_Tutorial->run->stepHook) {
        ai_Tutorial->run->stepHook(p);
        return;
    }
    if (ai_Tutorial->phase == 1) {
        if (c == 0) {
            if (ai_Tutorial->run->goal == a) {
                ai_Tutorial->next = 5;
                ai_Tutorial->stepAt = ai_ClockMsVolatile;
                if (b)
                    ai_Tutorial->stepArg = b;
            }
        } else if (c == 1 && ai_Tutorial->run->goal == a) {
            ai_Tutorial->next = 4;
            ai_Tutorial->stepAt = ai_ClockMsVolatile;
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
                ai_Tutorial->stepAt = ai_ClockMsVolatile;
                ai_Tutorial->campaignSteps++;
                return;
            }
            if (ai_Tutorial->run->goal == a && c == 1) {
                ai_Tutorial->state = 4;
                ai_Tutorial->next = 4;
                ai_Tutorial->stepAt = ai_ClockMsVolatile;
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
                goto started;
            }
        } else if (ai_Tutorial->next == 4) {
            if (ai_Tutorial->run->goal == a && c == 0) {
                ai_Tutorial->state = 9;
                ai_Tutorial->next = 9;
                goto started;
            }
        }
    }
    goto count;
started:
    ai_Tutorial->stepAt = ai_ClockMsVolatile;
count:
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
/* 0x5763c0, 1 when the run record's word +0x68 has a bit of `mask` (the run mask), else 0; 1 while 0x6f265c is 0
   and when there is no run record, 0 in the mode 2 / state 5 and while a transition 0x6333e0 is pending */

int ai_RunAllows(int mask)
{
    char *st;

    if (!ai_TutorialOn)
        return 1;
    st = S;
    if (*(int *)(st + 0x3c) == 2 || U_I(st) == 5)
        return 0;
    if (!*(int *)(st + 0x18))
        return 1;
    if (U_I((unsigned)&tb_Paused))
        return 0;
    return (*(int *)(*(char **)(st + 0x18) + 0x68) & mask) != 0;
}
/* 0x576410, the same predicate as 0x5763c0 on the run record's word +0x6c */

int ai_Unk576410(int mask)
{
    char *st;

    if (!ai_TutorialOn)
        return 1;
    st = S;
    if (*(int *)(st + 0x3c) == 2 || U_I(st) == 5)
        return 0;
    if (!*(int *)(st + 0x18))
        return 1;
    if (U_I((unsigned)&tb_Paused))
        return 0;
    return (*(int *)(*(char **)(st + 0x18) + 0x6c) & mask) != 0;
}
/* 0x576460, 1 while the gate 0x6f265c is 0; else 0 in the mode 2 / state 5, 1 otherwise */

int ai_Unk576460(void)
{
    char *st;

    if (!ai_TutorialOn)
        return 1;
    st = S;
    if (*(int *)(st + 0x3c) == 2 || U_I(st) == 5)
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
            ai_Tutorial->stepAt = ai_ClockMsVolatile;
            return 1;
        }
        if (args[2] == 1) {
            if (ai_Tutorial->next == 9) {
                ai_Tutorial->state = 4;
                ai_Tutorial->next = 4;
                ai_Tutorial->stepAt = ai_ClockMsVolatile;
                return 1;
            }
            if (ai_Tutorial->next == 0xa) {
                ai_Tutorial->state = 0xb;
                ai_Tutorial->next = 0xb;
                ai_Tutorial->stepAt = ai_ClockMsVolatile;
                return 1;
            }
        }
    } else if (args[0] == 7 && args[1] && ai_Tutorial->next == 9 && ((unsigned char *)args[1])[2] == 5) {
        ai_Tutorial->state = 0xa;
        ai_Tutorial->next = 0xa;
        ai_Tutorial->stepAt = ai_ClockMsVolatile;
        return 1;
    }
    return 3;
}
/* 0x5767d0, the kind 0xd with a building whose table byte is 0xa moves the pending state to 0xb (1), else 3 */

int ai_StepChapter1Step10(int *p)
{
    int kind;

    if (p[0] == 0xd && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0xa) {
            U_I(S) = 0xb;
            U_I(S + 4) = 0xb;
            U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
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
        U_I(S) = 0xa;
        U_I(S + 4) = 0xa;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
    if (p[0] == 0x18) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
    return 3;
}
/* 0x5768a0, when `arg` is 0, finds a record (0x501190 (1, 5, 0xf)) and writes 0x10e into its word +0x29; no return
   value (0x501190 returns the record) */

void ai_HookChapter2Step2(int arg)
{
    char *r;

    if (arg)
        return;
    r = (char *)gm_BuildQuery(1, 5, 0xf);
    if (r)
        *(unsigned short *)(r + 0x29) = 0x10e;
}
/* 0x5768d0, the kind 0x19 with a record whose word is 0x10e moves the pending state to 0xb (1), else 3 */

int ai_StepChapter2Step2(int *p)
{
    if (p[0] == 0x19 && p[1] && *(unsigned short *)p[1] == 0x10e) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
    return 3;
}
/* 0x576920, the same as 0x5768d0 for the kind 0x19 and the record word 0x110 */

int ai_StepChapter2Step3(int *p)
{
    if (p[0] == 0x19 && p[1] && *(unsigned short *)p[1] == 0x110) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
    return 3;
}
/* 0x576970, the same for the kind 0x1a and the record word 0x110 */

int ai_StepChapter2Step4(int *p)
{
    if (p[0] == 0x1a && p[1] && *(unsigned short *)p[1] == 0x110) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
    return 3;
}
/* 0x5769c0, the kinds 0x1b / 0x1c with a building whose table byte is 0xd: the kind 0x1b only while the pending
   state is 4 (state 0xa), the kind 0x1c in any state (state 0xb); else 3. Ends 0x576a7f; 0x576a80 is a new start
   without nop padding */

int ai_StepChapter2Step5(int *p)
{
    int kind;

    if (p[0] == 0x1b) {
        if (!p[1])
            goto next;
        if (U_I(S + 4) != 4)
            goto next;
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xd)
            goto fail;
        U_I(S) = 0xa;
        U_I(S + 4) = 0xa;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
next:
    if (p[0] == 0x1c) {
        if (!p[1])
            goto fail;
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xd)
            goto fail;
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
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
        sv_SelectPanelObjectK(r, -1, (char *)(unsigned)&ch_EmptyName);
}
/* 0x576ab0, the kinds 0x19 / 0x1a with a record whose word is 0x13: the kind 0x19 only in the pending state 4
   (state 0xa), the kind 0x1a in any state (state 0xb); else 3 */

int ai_StepChapter2Step6(int *p)
{
    if (p[0] == 0x19 && p[1] && U_I(S + 4) == 4) {
        if (*(unsigned short *)p[1] != 0x13)
            return 3;
        U_I(S) = 0xa;
        U_I(S + 4) = 0xa;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
    if (p[0] == 0x1a && p[1] && *(unsigned short *)p[1] == 0x13) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
    return 3;
}
/* 0x576b50, the kind 0x19 with a record whose word is 0x74 moves the pending state to 0xb (1), else 3 */

int ai_StepChapter2Step7(int *p)
{
    if (p[0] == 0x19 && p[1] && *(unsigned short *)p[1] == 0x74) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
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
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0x12)
            return 3;
        if (*(unsigned short *)(p[1] + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId)
            return 3;
        if (U_I(S + 0x38))
            return 3;
        if (p[3] != 0xc && p[3] != 0x1df)
            return 3;
        U_I(S + 0x38) = 1;
        U_I(S + 4) = 0xa;
        U_I(S) = U_I(S + 4);
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 3;
    }
    if (p[0] != 0x10 || !p[1])
        return 3;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0x12)
        return 3;
    if (*(unsigned short *)(p[1] + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId)
        return 3;
    if (p[2] == 0 && !U_I(S + 0x38))
        U_I(S + 4) = 9;
    if (p[2] == 1) {
        if (U_I(S + 0x38) == 1)
            U_I(S + 4) = 0xb;
        else
            U_I(S + 4) = 4;
    }
    U_I(S) = U_I(S + 4);
    U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
    return 1;
}
/* 0x576cd0, a predicate: 1 for the kinds 0x1a / 0x19 / 0x1c, and for the kind 0x2b on the player's building (byte
   0x12, word +0x27 = 0x6f25b8) with the step word 0xc / 0x1df; else 0 */

int ai_QueryChapter4Step2(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0x12
                && *(unsigned short *)(p[1] + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId
                && (p[3] == 0xc || p[3] == 0x1df))
            return 1;
    }
    if (p[0] == 0x1a || p[0] == 0x19 || p[0] == 0x1c)
        return 1;
    return 0;
}
/* 0x576d40, the twin of 0x576ba0 for the kinds 0x2b (step word 0x78 / 0x79 / 0x7a) and 0x11 */

int ai_StepChapter4Step4(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0x12)
            return 3;
        if (*(unsigned short *)(p[1] + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId)
            return 3;
        if (U_I(S + 0x38))
            return 3;
        if (p[3] != 0x78 && p[3] != 0x79 && p[3] != 0x7a)
            return 3;
        U_I(S + 0x38) = 1;
        U_I(S + 4) = 0xa;
        U_I(S) = U_I(S + 4);
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 3;
    }
    if (p[0] != 0x11 || !p[1])
        return 3;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0x12)
        return 3;
    if (*(unsigned short *)(p[1] + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId)
        return 3;
    if (p[2] == 0 && !U_I(S + 0x38))
        U_I(S + 4) = 9;
    if (p[2] == 1) {
        if (U_I(S + 0x38) == 1)
            U_I(S + 4) = 0xb;
        else
            U_I(S + 4) = 4;
    }
    U_I(S) = U_I(S + 4);
    U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
    return 1;
}
/* 0x576e70, the predicate twin of 0x576cd0: 1 for the player's building (byte 0x12, word +0x27 = 0x6f25b8) with
   the step word 0x78 / 0x79 / 0x7a; else the answer of 0x5764b0 */

int ai_QueryChapter4Step4(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0x12
                && *(unsigned short *)(p[1] + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId
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
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 2)
            return 3;
        if (*(unsigned short *)(p[1] + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId)
            return 3;
        if (U_I(S + 0x38))
            return 3;
        if (p[3] != 5 && p[3] != 2 && p[3] != 0xd && p[3] != 8)
            return 3;
        U_I(S + 0x38) = 1;
        U_I(S + 4) = 0xa;
        U_I(S) = U_I(S + 4);
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 3;
    }
    if (p[0] != 0x10 || !p[1])
        return 3;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 2)
        return 3;
    if (*(unsigned short *)(p[1] + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId)
        return 3;
    if (p[2] == 0 && !U_I(S + 0x38))
        U_I(S + 4) = 9;
    if (p[2] == 1) {
        if (U_I(S + 0x38) == 1)
            U_I(S + 4) = 0xb;
        else
            U_I(S + 4) = 4;
    }
    U_I(S) = U_I(S + 4);
    U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
    return 1;
}
/* 0x577020, the predicate twin of 0x576ee0: 1 for a building of the table byte 0x2 with the step word
   0x5 / 0x2 / 0xd / 0x8 and for the kinds 0x1a / 0x19 / 0x1c; else 0 */

int ai_QueryChapter4Step5(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 2
                && *(unsigned short *)(p[1] + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId
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
    id = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    cm_SendMoveGoodsUnchecked(id, -1, 0, 0x2bc0);
}
/* 0x5770e0, a transition callback (0x30 B; a start the ret+nop scan missed: it follows 0x5770a0's nop fill and
   runs to 0x57710f): for the event kind 0x20 with the step p[3] 0x155 it answers 1 while the state record's +0x3c
   is 3, else 0; every other event goes on to 0x5764b0 */

int ai_QueryChapter3Step2(int *p)
{
    if (p[0] == 0x20) {
        if (p[3] == 0x155 && U_I(S + 0x3c) == 3)
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
            /* x / y are never written: the original pushes three arguments to the one-argument 0x55ea60, as he.c
               0x565940 does */
            if (!he_FigureContact((int)chr, &x, &y))    /* @original: 3 args to the 1-arg 0x55ea60 (see above) */
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

    if (U_I(S + 0x38) >= 0 && p[0] == 0x25 && p[3] == 0x155) {
        U_I(S + 0x38) = 1;
        return 1;
    }
    if (U_I(S + 0x38) >= 1 && p[1] && p[0] == 0x24) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0xa) {
            U_I(S + 0x38) = 2;
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
    char *r;
    int id;

    if (arg)
        return;
    r = (char *)gm_FindMarketObject(0x155);
    id = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    cm_SendMoveGoodsUnchecked(id, -1, 0, 0x2bc0);
    cm_SendExSellObject(*(int *)(r + 2), -1, 0x155, 3, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
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
/* 0x577320, the flag S+0x38 */

int ai_MarkerChapter3Step6(int mode)
{
    return U_I(S + 0x38);
}
/* 0x577330, a transition callback on the flag S+0x38: it is raised to 1 by the kind 0x28 (step word 0x155), to 2
   by the kind 0x27 (0x1d0) and to 3 by the kind 0x24 on the player's building (table byte 0x12, word +0x27 =
   0x6f25b8), each when the flag already reaches the previous level (1) and the step fits; the kinds 0x1a / 0x1c
   answer 1, everything else 0 */

int ai_QueryChapter3Step8(int *p)
{
    int kind;

    if (U_I(S + 0x38) >= 0 && p[0] == 0x28 && p[3] == 0x155) {
        U_I(S + 0x38) = 1;
        return 1;
    }
    if (U_I(S + 0x38) >= 1 && p[0] == 0x27 && p[3] == 0x1d0) {
        U_I(S + 0x38) = 2;
        return 1;
    }
    if (U_I(S + 0x38) >= 2 && p[1] && p[0] == 0x24) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0x12
                && *(unsigned short *)(p[1] + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId) {
            U_I(S + 0x38) = 3;
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
/* 0x577450, a step predicate: 1 for the kind 0x26 with the step word 0x1d0 while the flag S+0x38 is not negative
   (which sets the flag to 1), and for the kinds 0x1a / 0x1c; else 0 */

int ai_QueryChapter3Step10(int *p)
{
    if (U_I(S + 0x38) >= 0 && p[0] == 0x26 && p[3] == 0x1d0) {
        U_I(S + 0x38) = 1;
        return 1;
    }
    if (p[0] == 0x1a || p[0] == 0x1c)
        return 1;
    return 0;
}
/* 0x577490, releases the pending id S+0x50: with the kind S+0x40 2 through 0x52f9f0, with the kind 3 through
   0x52fa00; returns 0 */

int ai_StepShownChapter3Step4(void)
{
    char *st;

    st = S;
    if (*(int *)(st + 0x40) == 2 && *(int *)(st + 0x50))
        sv_BookForward(*(int *)(st + 0x50));
    else if (*(int *)(st + 0x40) == 3 && *(int *)(st + 0x50))
        sv_BookBack(*(int *)(st + 0x50));
    return 0;
}
/* 0x5774d0, when `arg` is 0 and the building 0x6f2308 is not already of the table byte 0x12: finds a record
   (0x501190 (1, 5, 0x12)), picks 0x4ffe90 (r+0x5d, 1, 0, 0x74) and passes it (0x521960 (r, q+2, 0x631090)) */

void ai_HookChapter3Intro(int arg)
{
    char *r;
    char *q;
    int kind;

    if (arg)
        return;
    if (U_I((unsigned)&ai_PlayerBuilding)) {
        kind = *(unsigned char *)U_I((unsigned)&ai_PlayerBuilding);
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0x12)
            return;
    }
    r = (char *)gm_BuildQuery(1, 5, 0x12);
    if (!r)
        return;
    q = (char *)gm_Query(*(int *)(r + 0x5d), 1, 0, 0x74);
    if (!q)
        return;
    sv_SelectPanelObjectK(r, *(int *)(q + 2), (char *)(unsigned)&ch_EmptyName);
}
/* 0x577540, when `arg` is 0 and the building 0x6f2308 is not already of the table byte 0x2: finds a record
   (0x501190 (2, 5, 2, 4, player) with the player word 0x6f25b8), picks 0x4ffe90 (r+0x5d, 1, 0, 0x13) and passes
   it (0x521960 (r, q+2, 0x631090)) */

void ai_HookChapter5Intro(int arg)
{
    char *r;
    char *q;
    int kind;

    if (arg)
        return;
    if (U_I((unsigned)&ai_PlayerBuilding)) {
        kind = *(unsigned char *)U_I((unsigned)&ai_PlayerBuilding);
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 2)
            return;
    }
    r = (char *)gm_BuildQuery(2, 5, 2, 4, *(unsigned short *)(unsigned)&ai_PlayerId);
    if (!r)
        return;
    q = (char *)gm_Query(*(int *)(r + 0x5d), 1, 0, 0x13);
    if (!q)
        return;
    sv_SelectPanelObjectK(r, *(int *)(q + 2), (char *)(unsigned)&ch_EmptyName);
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
    party.time = ai_TimeT;
    party.kind = 1;
    party.flags = 0xa0;
    party.thing = -1;
    cm_SendNewParty(&party);
}

#define R8 (*(int *)(*(char **)(*(char **)(S + 0x1c) + 0x74) + 8))

/* 0x577740, the start of the tutorial ("tutorial\main" 0x629304): -4 without a player / with a running run
   record, else the tutorial message 0x5ec (0x53d6f0), the controls of the run's window (0x475850, 0x475600,
   0x475f70 (id, 0) / (id, 3)), 0x52ea30 (0x6ed938), the text 0x624204 (0x515960, 0x7d) and the window state
   S+0x20 = 0x475b80 (id, 3, text); -1 without the run record, else 0. ROUGH: the chain 0x5eec60 -> +0x1c -> +0x74
   is reloaded before every call as in the original */
int ai_Unk577740(void)
{
    char *per;

    per = *(char **)(S + 0x1c);
    if (!per)
        return -4;
    if (*(int *)(per + 0x74))
        return -4;
    he_Unk53d6f0(per, 0, 0, (char *)(unsigned)&full_data_629304, 0x5ec);
    if (!*(int *)(*(char **)(S + 0x1c) + 0x74))
        return -1;
    d2_PlaceMainWindowF(R8, 3);
    d2_CentreViewWindows(R8);
    d2_SetForm(R8, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(R8, 3);
    U_I(S + 0x20) = d2_GetWinObj(R8, 3, io_PrintText((char *)(unsigned)"$Z%s", 0x7d));
    return 0;
}
/* 0x577820, -4 without a player / run record, else 0x53d800 (player) and 0 */

int ai_Unk577820(void)
{
    char *per;

    per = *(char **)(S + 0x1c);
    if (!per)
        return -4;
    if (!*(int *)(per + 0x74))
        return -4;
    he_Unk53d800(per);
    return 0;
}
/* 0x577850, -4 without a player / run record, else the run window's controls 1 and 2 (0x475f70) with the text
   0x602568 (0x515960) and 0 */

int ai_TutorialClearRunWindow(void)
{
    char *per;

    per = *(char **)(S + 0x1c);
    if (!per)
        return -4;
    if (!*(int *)(per + 0x74))
        return -4;
    d2_SetForm(*(int *)(*(char **)(per + 0x74) + 8), 1);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(R8, 2);
    io_PrintText((char *)(unsigned)"$C");
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
/* 0x577b60, clears the text window S+0x24: control 0 off (0x475f70), the text 0x602568 (0x515960), 0x475590
   (S+0x24, 0); no return value */

void ai_TutorialHideText(void)
{
    d2_SetForm(U_I(S + 0x24), 0);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetViewEnabled(U_I(S + 0x24), 0);
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
/* 0x577c60, clears the second text window S+0x34 (106 B): control 0 off (0x475f70), the text 0x602568
   (0x515960), 0x475590 (S+0x34, 0); then the window S+0x14 is released (0x426f20 / 0x427290) and cleared */

void ai_TutorialHideText2(void)
{
    d2_SetForm(U_I(S + 0x34), 0);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetViewEnabled(U_I(S + 0x34), 0);
    if (U_I(S + 0x14)) {
        if (sb_IsPlaying(U_I(S + 0x14)))
            sb_Stop(U_I(S + 0x14), 0);
        U_I(S + 0x14) = 0;
    }
}
/* 0x577cd0, creates the tutorial progress form ("tutorial\progress" 0x629314, 129 B): the form id
   0x4741f0 (0, 0, name) into S+0x68, its controls (0x475850 (id, 3), 0x475f70 (id, 0)) and the slider
   S+0x6c = 0x46a960 (x, 5, 400, 0, 400, 0x75, 0x42, d2_LastWindow), x = the short +0 of the window record
   0xf8f128 + i * 0x3c4 (i = d2_LastWindow) / 2 - 200 (centered on the 400 wide slider) */

void ai_TutorialOpenProgress(void)
{
    int i;

    U_I(S + 0x68) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629314);
    d2_PlaceMainWindowF(U_I(S + 0x68), 3);
    d2_SetForm(U_I(S + 0x68), 0);
    i = U_I((unsigned)&d2_LastWindow);
    U_I(S + 0x6c) = d2_WinOpenSlider(*(short *)((unsigned)&ai_Unkf8f128 + i * 0x3c4) / 2 - 0xc8, 5, 0x190, 0, 0x190,
                                 0x75, 0x42, i);
}
/* 0x577d60, releases the tutorial progress form S+0x68 (36 B, a Ghidra-missed start after 0x577cd0's nop fill):
   nothing while the id is -1; else 0x4758f0 (id) and the id reset to -1 */

void ai_TutorialCloseProgress(void)
{
    if (U_I(S + 0x68) != -1) {
        d2_CloseView(U_I(S + 0x68));
        U_I(S + 0x68) = -1;
    }
}


#define TUT_SINCE(at) (ai_ClockMsVolatile - (at) ? ai_ClockMsVolatile - (at) : 0)
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
/* 0x5780e0, predicate (49 B): -4 without the gate 0x6f265c; 0 while the input is idle (0xf8a668 == -1) or the
   current window 0x5fe710 is not the control S+0x4c; 1 when it is (the 0x577a00 shape on S+0x4c) */

int ai_TutorialStepClicked(void)
{
    if (!ai_TutorialOn)
        return -4;
    if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_PointerObj) == U_I(S + 0x4c))
        return 1;
    return 0;
}
/* 0x578120, sets the player S+0x1c = per and resets the window id S+0x20 = -1 (43 B): -4 without the gate
   0x6f265c, else 0 */

int ai_Unk578120(char *per)
{
    if (!ai_TutorialOn)
        return -4;
    *(char **)(S + 0x1c) = per;
    U_I(S + 0x20) = -1;
    return 0;
}
/* 0x578150, clears the player S+0x1c and resets the window id S+0x20 = -1 (26 B, no return value) */

void ai_Unk578150(void)
{
    U_I(S + 0x1c) = 0;
    U_I(S + 0x20) = -1;
}
/* 0x578170, the gate predicate (14 B): 1 while the gate 0x6f265c is clear, else 0 */

int ai_Unk578170(void)
{
    return !ai_TutorialOn;
}

int ai_TutorialInitMainIntro();
int ai_TutorialInitMainOutro();
void ai_TutorialStop();
int sb_UnloadSampleBank();
int snd_LoadVoiceBank();
void * mem_Alloc(int size, char *tag);
void ai_TutorialFreeRuns();

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
    ai_Tutorial->textAt = ai_ClockMsVolatile;
    ai_Tutorial->stepAt = ai_ClockMsVolatile;
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
/* 0x578370, the tutorial stop (196 B): the six handles 0x6f2640 .. 0x6f2658 unloaded when set (0x426ba0), the
   record chain 0x6f2658 freed by 0x578440 and cleared; the state record 0x5eec60 reset (+0, +8, +0xc, +0x10,
   +0x14, +0x18, +0x1c = 0, +0x20 = -1, the forms +0x24 / +0x34 released (0x4758f0) and -1, +0x14 = 0 again),
   the gates 0x6f265c / 0x6f2660 = 0; no return value */

void ai_TutorialStop(void)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (ai_TutorialBanks[i])
            sb_UnloadSampleBank(ai_TutorialBanks[i]);
    }
    ai_TutorialFreeRuns((char *)ai_TutorialRuns);
    ai_TutorialRuns = 0;
    U_I(S) = 0;
    U_I(S + 8) = 0;
    U_I(S + 0xc) = 0;
    U_I(S + 0x10) = 0;
    U_I(S + 0x14) = 0;
    U_I(S + 0x18) = 0;
    U_I(S + 0x1c) = 0;
    U_I(S + 0x20) = -1;
    d2_CloseView(U_I(S + 0x24));
    U_I(S + 0x24) = -1;
    d2_CloseView(U_I(S + 0x34));
    U_I(S + 0x34) = -1;
    U_I(S + 0x14) = 0;
    ai_TutorialOn = 0;
    ai_TutorialRunning = 0;
}
/* 0x578440, frees the tutorial record chain (82 B, recursive): nothing for a null record; else its +0x70 next
   record, its +0x58 and +0x64 tables (mem_Free, both cleared) and then the record itself */

void ai_TutorialFreeRuns(char *rec)
{
    if (!rec)
        return;
    if (*(char **)(rec + 0x70))
        ai_TutorialFreeRuns(*(char **)(rec + 0x70));
    if (*(int *)(rec + 0x58)) {
        mem_Free(*(int *)(rec + 0x58));
        *(int *)(rec + 0x58) = 0;
    }
    if (*(int *)(rec + 0x64)) {
        mem_Free(*(int *)(rec + 0x64));
        *(int *)(rec + 0x64) = 0;
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
    ai_TutorialFreeRuns((char *)head);
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
    ai_TutorialFreeRuns((char *)head);
    return 0;
}

/* 0x579570, builds the run chain of the tutorial's chapter 3 (ai_TutorialChapterInit[2]): ten run records (the
   intro 'ntr3', the steps 'c3s1' .. 'c3sA', the outro 'utr3'), each allocated under its "FTInitChapter3*" tag
   (the intro under the copy-pasted "FTInitChapter2Intro"), zeroed and filled with its texts / voices
   ("CHAPTER_3_*"), times, callbacks, goal and allow masks, and linked through next. The steps 1, 2, 4 and 6 get
   a step table ("FTInitScriptChapter3Step*"), the steps 2, 6, 8 and 10 the position marker's path
   ("FTInitAnimChapter3Step*"), whose points depend on the screen width (800, 1024, else). The voice bank
   "TUTORIAL_CHAPTER_3.sbf" is loaded into ai_TutorialBanks[3]. The first record, or 0 after a failed allocation
   (the records built so far freed)
   @deep read whole against the original: every record, step table and marker path stored at the original's
   offsets with its values (asmdiff --symbols: no ! line, 1213 vs 1213 instructions, size 5377 = 5377).
   Left: register choice for the repeated float constants of the marker paths, three hunks: 0x579df6 (step
   6 path 0 wait/travel/rest: original ecx=0x420c0000 edi=0x430e0000 edx=0x428e0000, ours edi/ecx/edx),
   0x579f27 (path 1 +0x4c/+0x50/+0x54 the same constants in ecx/edi/edx vs edi/ecx/edx) and 0x57a1b6 (step
   8 path 0 +4/+8: the reload of marks and mov eax,0x43910000 scheduled one store apart), plus regs hunks
   eax<->ecx at 0x57a110 / 0x57a168: allocation and scheduling only */

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
    ai_TutorialFreeRuns((char *)head);
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
    ai_TutorialFreeRuns((char *)head);
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
    ai_TutorialFreeRuns((char *)head);
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

void d2_RaiseObject();
void d2_EnableObject();
int d2_CreatePicObj();
struct G5CH; struct G9REC; struct VFSFILE; void d2_MoveObj(short x, short y, int obj);
void d2_RemoveObj();

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
            ai_Tutorial->markerAt = ai_ClockMs;
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
        ai_Tutorial->markerAt = ai_ClockMs;
        ai_Tutorial->markerPhase = 2;
        if (ai_Tutorial->markerObj == -1) {
            m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
            ai_Tutorial->markerObj = d2_CreatePicObj((int)m->from[0], (int)m->from[1], 0x1d);
        }
        m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
        d2_MoveObj((int)m->from[0], (int)m->from[1], ai_Tutorial->markerObj);
    } else if (mode == 1) {
        if (ai_Tutorial->markerPhase == 2) {
            if ((float)ai_Tutorial->markerAt + ai_Tutorial->run->marks[ai_Tutorial->markerStep].wait > (float)(unsigned)ai_ClockMs)
                return;
            ai_Tutorial->markerPhase = 3;
            ai_Tutorial->markerAt = ai_ClockMs;
        }
        if (ai_Tutorial->markerPhase == 3) {
            m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
            if ((float)ai_Tutorial->markerAt + m->travel > (float)(unsigned)ai_ClockMsVolatile) {
                t = (float)(ai_ClockMsVolatile - ai_Tutorial->markerAt) / m->travel;
                ai_TutorialBezier(m->from, m->ctrl1, m->ctrl2, m->to, t, pos);
                d2_MoveObj((int)pos[0], (int)pos[1], ai_Tutorial->markerObj);
                return;
            }
            ai_Tutorial->markerPhase = 4;
            ai_Tutorial->markerAt = ai_ClockMs;
            m = &ai_Tutorial->run->marks[ai_Tutorial->markerStep];
            d2_MoveObj((int)m->to[0], (int)m->to[1], ai_Tutorial->markerObj);
        }
        if (ai_Tutorial->markerPhase == 4) {
            if ((float)ai_Tutorial->markerAt + ai_Tutorial->run->marks[ai_Tutorial->markerStep].rest > (float)(unsigned)ai_ClockMs)
                return;
            ai_Tutorial->markerPhase = 2;
            ai_Tutorial->markerAt = ai_ClockMs;
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

int gm_HeadIconNumber();

/* 0x57bc30, the tutorial info block (168 B, ROUGH): the bytes 0x75e5a0 = 0x4b, 0x75e5a1 = 0x18, 0x75e5a2 =
   0x10, 0x75e5a3 = 0x33, the dword 0x75e5a4 = 0x59a, 0x75e5a8 / 0x75e5a9 = 0; 0x75e628 = 0x4fb770 (0x624ee0)
   + 0x639; the strings U_I(0x1449e48) / U_I(0x1449e4c) copied to 0x75e5aa / 0x75e5ca; 0x503ee0 (the local
   buffer, the byte 0x75e5a3) returns a record whose dword +0 goes to 0x75e5f0 and whose word +4 (low byte) to the
   local and 0x75e5f4 */
void ai_SetupTutorialPlayer(void)
{
    JOBTALENTS t;

    *(unsigned char *)(unsigned)&init_PlayerProfile = 0x4b;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x1) = 0x18;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x2) = 0x10;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x3) = 0x33;
    U_I(((unsigned)&init_PlayerProfile + 0x4)) = 0x59a;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x8) = 0;
    *(unsigned char *)((unsigned)&init_PlayerProfile + 0x9) = 0;
    init_PlayerProfile.modelId = gm_HeadIconNumber((char *)(unsigned)&full_data_624ee0) + 0x639;
    strcpy((char *)((unsigned)&init_PlayerProfile + 0xa), *(char **)(unsigned)&full_data_1449e48);
    strcpy((char *)((unsigned)&init_PlayerProfile + 0x2a), *(char **)(unsigned)&full_data_1449e4c);
    t = gm_GetJobTalents(*(unsigned char *)((unsigned)&init_PlayerProfile + 0x3));
    *(int *)init_PlayerProfile.talents = *(int *)t.skills;
    init_PlayerProfile.talents[4] = t.skills[4];
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

int ai_AlchemistSendRecipeResearch(); int ai_AlchemistSendResearchStart(); int ahm_ResearchState(); char * ahm_OwnerChar(); AHMGRID * ahm_RandomRecipe();
int d2_WinOpenSlider(); void d2_SetObjValue(); void d2_MenuNavigate(int group);

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
    lastClock = ai_ClockMs;
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
            d2_SetObjValue(slider, 0, 100, (int)(f * 100.0f));
        }
        d2_SetObjEnabled(start, d2_Radios[group].sel != -1 && n != 0 && state == 0 ? 1 : 0);
        d2_SetObjEnabled(other, state == 0);
        if (state != lastState || ai_ClockMs > lastClock + 0x32) {
            lastClock = ai_ClockMs;
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
   (ahm_ResearchState), else 1
   @deep read whole against the original and reconstructed (AIMSG zeroed, fromId = building id, type 0x96,
   flags 1, to = the owner character's netId, time copied as 3 ints and a short, arg[0] = rec id, arg[1] =
   slot id, arg[4] = (util_Random(0x5a0) & 0xffff) + 1000; 228 vs 227 instructions; asmdiff --symbols:
   every load, store, constant and call of the original is present). Left: scheduling only: the original
   pushes &msg (lea ecx,[esp+0x10]; push ecx) before the flags / to / time stores, interleaves the clock
   loads with them (ae8 first into [esp+0x40], then to, hour, year, the word at +0x34), ours loads the
   clock words first and pushes the pointer last; tried a struct copy, a field-wise copy (a,b,c,d) and the
   original's store order, all give the same schedule */
int ai_AlchemistSendRecipeResearch(unsigned char *obj, AHMREC *rec, int slot)
{
    AIMSG msg;
    AHMCLOCK *clk;
    int a;
    float f;

    if (ahm_ResearchState(obj, &a, &f, 0) != 0)
        return 0;
    memset(&msg, 0, sizeof(msg));
    clk = (AHMCLOCK *)&msg.time;
    msg.fromId = ((AIBLD *)obj)->id;
    clk->c = ai_TimeK.c;
    msg.type = 0x96;
    msg.flags = 1;
    msg.to = ai_CharsT[((AIBLD *)obj)->owner].netId;
    clk->b = ai_TimeK.b;
    clk->a = ai_TimeK.a;
    clk->d = ai_TimeK.d;
    msg.arg[0] = rec->id;
    msg.arg[1] = rec->slots[slot].id;
    msg.arg[4] = (util_Random(0x5a0) & 0xffff) + 1000;
    cm_SendLongMessage((char *)&msg);
    return 1;
}
/* 0x57ced0, starts the alchemist object obj's own research (record rec) with the message 0x95 (cm_SendLongMessage;
   the 0x57cde0 shape without a place: arg[0] and arg[3] the record's id, no random duration); 0 when a research
   already runs (ahm_ResearchState), else 1
   @deep read whole against the original and reconstructed (the 0x57cde0 shape: type 0x95, flags 1, arg[0]
   and arg[3] = rec id, no duration; 190 vs 189 instructions; asmdiff --symbols: every load, store,
   constant and call is present). Left: scheduling only: the original pushes &msg (lea ecx,[esp+0x10]; push
   ecx) between the type and flags stores and interleaves the clock loads (ae8 into [esp+0x40] first, year
   / hour / the word after the to load), ours loads the clock first and pushes the pointer last; a struct
   copy, a field-wise copy and the original's store order give the same schedule */
int ai_AlchemistSendResearchStart(unsigned char *obj, AHMREC *rec)
{
    AIMSG msg;
    AHMCLOCK *clk;
    int a;
    float f;

    if (ahm_ResearchState(obj, &a, &f, 0) != 0)
        return 0;
    memset(&msg, 0, sizeof(msg));
    clk = (AHMCLOCK *)&msg.time;
    msg.fromId = ((AIBLD *)obj)->id;
    clk->c = ai_TimeK.c;
    msg.type = 0x95;
    msg.flags = 1;
    msg.to = ai_CharsT[((AIBLD *)obj)->owner].netId;
    clk->b = ai_TimeK.b;
    clk->a = ai_TimeK.a;
    clk->d = ai_TimeK.d;
    msg.arg[0] = rec->id;
    msg.arg[3] = rec->id;
    cm_SendLongMessage((char *)&msg);
    return 1;
}
int ai_AlchemistTrainShareWindow(); int ai_AlchemistFundWindow(); int ai_AlchemistRescheduleWindow();
int d2_WinOpenSlider(); void d2_SetObjWidth(int obj, short w); void d2_SetViewEnabled(int view, int on); int d2_GetWinObj();

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
    lastClock = ai_ClockMs;
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
    d2_SetObjValue(slider, 0, 100, (int)(rec->value * 100.0f));
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
            d2_SetObjValue(slider, 0, 100, (int)(f * 100.0f));
        }
        if (state != lastState || ai_ClockMs > lastClock + 0x32) {
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
            d2_SetObjValue(slider, 0, 100, (int)(rec->value * 100.0f));
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
   Answers 1
   @deep read whole against the original and reconstructed typed (AIMSG, AHMREC, the form name as a
   literal); asmdiff --symbols: every call, argument, constant and branch in the original's order, 520 vs
   520 bytes (the same 94 leading instructions equal). Left: scheduling of the message block only (the
   0x57cde0 family): the original loads the owner's netId (lea/lea/lea, mov ecx,[ai_CharInts]) and the
   clock words (NOWX, ai_TimeHour, 0x770ae8, the word 0x770aec) interleaved with the fromId and arg stores,
   ours copies the clock first (ai_Time, ai_Time+0xc) and the owner after; struct copy, field-wise copy in
   several orders give the same schedule */
int ai_AlchemistTrainShareWindow(unsigned char *obj, AHMREC *rec, int idx)
{
    AIMSG msg;
    AHMCLOCK *clk;
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
                memset(&msg, 0, sizeof(msg));
                clk = (AHMCLOCK *)&msg.time;
                msg.type = 0x97;
                msg.flags = 1;
                msg.fromId = ((AIBLD *)obj)->id;
                msg.to = ai_CharsT[((AIBLD *)obj)->owner].netId;
                clk->a = ai_TimeK.a;
                clk->b = ai_TimeK.b;
                clk->c = ai_TimeK.c;
                clk->d = ai_TimeK.d;
                msg.arg[0] = rec->id;
                msg.arg[1] = idx;
                msg.arg[4] = (util_Random(0x5a0) & 0xffff) + 1000;
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
   answers 0 at once), the text 0x403a with an input field (d2_GetWinObj) preset with the tenth converted to the
   player's money (gm_ConvertMoney, called the original's way: the second and third call get the previous result
   as a third word) and the answers 0x403b. The first answer takes the field's amount (gm_PlayerFactor) from
   the player's character when sv_CheckGoodAmount agrees: the value is raised by amount / object value * share
   * 0.75 (ahm_SendAddValue, the float passed by value), the amount is moved from the player (cm_SendMoveGoods)
   and the window closes with the answer 1; the second answer and the right mouse button close it. The window
   closes with 0 when the owner is gone
   @deep read whole against the original and reconstructed typed (signature (obj, rec) as the caller
   pushes, gm_ConvertMoney's extra third word through a cast macro, the float passed by value to
   ahm_SendAddValue through a cast macro, the loop as a do-while with the owner test on top; 625 vs 631
   bytes, asmdiff --symbols: every call, argument, constant and branch in the original's order, 8
   instructions apart). Left: stack-pop scheduling only: the original pops the three gm_ConvertMoney calls
   separately (add esp,0x14 / 8 / 8 after the first, second and third call) and the whole prologue pile
   once later (add esp,0x34), ours defers them into one add esp,0x40 / 0x18; the original's field-object
   slot is [esp+0x3c] vs ours [esp+0x20] */
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
    v = gm_ConvertMoney(tenth, ai_OwnPlayer);
    v = gm_ConvertMoney3(points, ai_OwnPlayer, v);
    v = gm_ConvertMoney3(tenth, ai_OwnPlayer, v);
    d2_SetObjValue(field, v);
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
#define CAM ((D3OBJECT *)sv_CameraObj)  /* the camera object d3_Camera (0xfc2dec) */
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
char * gv_FindEffect(int id); int gv_RequestEffectLoad(char *o); int cm_GetSlotState(); void gv_SetEffectCallback();
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */
void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b); void sv_SetPanelEnabled(); void d3sky_Init(struct D3SKY *s, char build);
void d3t_GetObjectBounds(); 
void ai_CannonBuildPanel(); void ai_CannonPanel(); char d3if_ShowTree(struct D3OBJECT *obj, char show);
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);                 /* 0x447190, d3_interface.h */
char d3if_Show(struct D3OBJECT *obj, char show);        /* 0x4471b0, d3_interface.h */
void he_ShowObjectInfos(); void ai_CannonAxes(); void ai_CannonAim(CANNONSCENE *sc, float dt); void ai_CannonCamera();
void ai_CannonShotResults(CANNONSCENE *sc); void ai_CannonSwitchAnim(char *rec, CANNONSCENE *sc, GVEFFECT *run, int sel, int mask); int ai_CannonFireCheck(CANNONSCENE *sc); void ai_CannonExplosion(CANNONSCENE *sc, char *rec);
void sv_SetStatusText(char *s); void d2_RemoveObj(); void sv_RedrawScreen();
void d3if_Prerender(char map); 

char ai_ShotCallbackArg[0x40] = { 0 };  /* 0x6fa020, the argument gv_SetEffectCallback stores with the shot callback ai_ShotTableStore (ai_CannonScene); never read by the code, its type is unknown; it runs to the tutorial state record at 0x6fa060 */

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
    sc.last = ai_ClockMs;
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
    cam3.x = CAM->pos.x;
    cam3.y = CAM->pos.y;
    cam3.z = CAM->pos.z;
    save3.x = CAM->rotation.x;
    save3.y = CAM->rotation.y;
    save3.z = CAM->rotation.z;
    rect[2] = d2_DirtyRectRight;
    rect[0] = d2_DirtyRectLeft;
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
    d2_DirtyRectLeft = d2_ScreenW;
    d2_DirtyRectTop = 0x4b;
    d2_DirtyRectBottom = d2_ScreenH - 0x4b;
    sv_SetPanelEnabled(0);
    cam3.x = CAM->pos.x;
    cam3.y = CAM->pos.y;
    cam3.z = CAM->pos.z;
    save3.x = CAM->rotation.x;
    save3.y = CAM->rotation.y;
    save3.z = CAM->rotation.z;
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
        sc.now = ai_ClockMs;
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
    d2_DirtyRectLeft = rect[0];
    d2_DirtyRectTop = rect[1];
    d2_DirtyRectBottom = rect[3];
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    d3if_SelectScene(scene, 0);
    if (d3_Sky)
        d3sky_Init(d3_Sky, 1);
    d3_SetObjectPos(CAM, &cam3);
    d3_SetObjectRot(CAM, (D3VECTOR *)&save3);
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
   The pitch is kept within -15 .. 45 degrees (the pitch speed is cleared at a stop), the yaw within 0 .. 2 pi
   @deep read whole against the original (323 vs 325 ins). Fixed meaning: the speed steps are if/else
   statements (sc->x += dt * 0.8f / 0.25f, the original tail-merges the fadd/fstp, so the clamp reloads),
   the speed clamps are x < 1 ? x : 1 / x > -1 ? x : -1, the pitch clamp is MIN_(MAX_(x, lo), hi) with <
   ternaries. Remaining: +0xb1 tail / +0xf4 code 3: the out-of-range 'dragState = 0; goto clamp' block, the
   original has it inline (entered at +0xb5 with ebx still 0, +0xb3 xor ebx after the movsx ebx compare),
   ours places the same 3 instructions after the dragState==0 block (block placement; tried else-if form,
   same); +0x12b code 14: the drag dx/dy, the same instructions, the dragY load scheduled before the movsx
   and ecx/edx/eax rotated; +0x438 code 4: the outer pitch compare, ours jump-threads the x < lo path past
   the hi compare (jnp to the second inner copy), the original evaluates fld lo; jmp and then compares it
   with hi (tried >= form: worse, 52 differing). The rest is regs (ecx->edx, eax->ecx edx->eax) (size: 3 of
   the instruction difference are shared / inline tails) */
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
    if (dm_L.ldown != 0) {
        if (y < 0x50 || y > d2_ScreenH - 0x50) {
            sc->dragState = 0;
            goto clamp;
        } else if (sc->dragState == 0) {
            sc->dragX = dm_L.x;
            sc->dragY = dm_L.y;
            sc->dragState = 1;
            sc->pitchDelta = 0;
            sc->yawDelta = 0;
            sc->pitchSpeed = 0;
            sc->yawSpeed = 0;
            goto clamp;
        }
    } else if (sc->dragState == 1) {
        sc->dragState = 0;
        goto clamp;
    }
    if (sc->dragState == 1) {
        dx = dm_L.x - sc->dragX;
        sc->dragX = dm_L.x;
        dy = y - sc->dragY;
        sc->dragY = dm_L.y;
        sc->rot.x = sc->rot.x - dy * 0.008333334f;
        sc->rot.y = sc->rot.y - dx * 0.011111111f;
        goto clamp;
    }
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
clamp:
    if (sc->rot.x < ai_CannonPitchMin || sc->rot.x > ai_CannonPitchMax) {
        sc->rot.x = (sc->rot.x < ai_CannonPitchMin ? ai_CannonPitchMin : sc->rot.x) < ai_CannonPitchMax
                        ? (sc->rot.x < ai_CannonPitchMin ? ai_CannonPitchMin : sc->rot.x) : ai_CannonPitchMax;
        sc->pitchSpeed = 0;
    }
    sc->rot.y = (float)fmod(sc->rot.y, 6.2831855f);
}

void d3_MatTranspose(); float d3_VecAngle2(); int ai_GetDefaultEffect();

#define CAM_AXIS (*(D3VECTOR4 *)&ai_AxisPair)   /* 0x5ebb74, the (0, 0, 1) axis (the (0, 1, 0) one follows it) */

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
    pos.y = (float)(d3_VecAngle2((char *)&CAM_AXIS, def) - 1.5707964f) - sc->rot.y;
    pos.x = 0;
    pos.z = 0;
    d3_SetObjectRot(sc->obj9c, (char *)&pos);
    if (sc->fired == 0)
        ai_CannonAxes((char *)sc);
    v.x = CAM_AXIS.x * sc->obj98->matrix._11 + CAM_AXIS.y * sc->obj98->matrix._21 + CAM_AXIS.z * sc->obj98->matrix._31;
    v.y = CAM_AXIS.x * sc->obj98->matrix._12 + CAM_AXIS.y * sc->obj98->matrix._22 + CAM_AXIS.z * sc->obj98->matrix._32;
    v.z = CAM_AXIS.x * sc->obj98->matrix._13 + CAM_AXIS.y * sc->obj98->matrix._23 + CAM_AXIS.z * sc->obj98->matrix._33;
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
        ai_CannonAxes((char *)sc);
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
            pos.x = CAM_AXIS.x;
            pos.y = CAM_AXIS.y;
            pos.z = CAM_AXIS.z;
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

/* 0x57f5d0, copies three vectors of the cannon object (55 B): 0x458810 (sc + 0x8c, 0x5ebb74, sc + 0x24),
   (0x5ebb94, sc + 0x44) and (0x5ebb84, sc + 0x34) */
void ai_CannonAxes(char *sc)
{
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)(unsigned)&ai_AxisPair, sc + 0x24);
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)(unsigned)&d3_YAxis, sc + 0x44);
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)((unsigned)&ai_AxisPair + 0x10), sc + 0x34);
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
    if (sc->resultClock + 0x15e <= (unsigned)ai_ClockMsVolatile) {
        sc->resultClock = ai_ClockMsVolatile;
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
int _ftol(); float ai_CannonPitchRatio(); int d2_CreateWinText(); void d2_SetObjFont();  /* @original: the call at 4110 passes 1 argument to d2.h's 4-parameter prototype */
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
    d2_SetObjValue(sc->kindSlider, 0, n, a);
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
/* 0x57fdb0, the cannon's pitch as 0 .. 1 (28 B): (sc + 0x10 + 0.2617994) / (0.7853982 + 0.2617994) */

float ai_CannonPitchRatio(char *sc)
{
    return (*(float *)(sc + 0x10) - -0.2617994f) / (0.7853982f - -0.2617994f);
}
/* 0x57fdd0, the barrel switch: rec = the building record, sc = the scene, run = its effect slot, sel = the barrel
   (0 .. 2), mask = the pump mask; nothing for the selected barrel or one above sc->maxSel. Two halves of
   ai_CannonSwitchMs each, every frame pumping sv_RunFrame (this function, mask) and updating the panel
   (ai_CannonPanel); past the half's time sv_PopRequest ends it. The cannon object sinks back along the level aim
   direction by ai_CannonSwitchDist * f and the wimpel moves from ai_WimpelPos along CAM_AXIS by as much, f going
   0 -> 1 in the first half and 1 -> 0 in the second; between them the barrels are switched (only sel shown) and
   sc->selected = sel; at the end sc->now = the clock
   @deep read whole against the original (241 vs 247 ins). Fixed meaning: the two halves were written with
   the wrong offsets; now the scene's fields (dirX / dirZ, pos, obj98 / obj9c, obj8c, selected, maxSel,
   now), the half's time and the sink distance are the const data ai_CannonSwitchMs / ai_CannonSwitchDist
   (0x5eec94 / 98: the original divides by it, a literal would be a multiply), the clock is read through
   ai_ClockMsVolatile (the original reloads it for the compare and for f), past the time sv_PopRequest, v =
   dist * f * v then pos - v, the wimpel ai_WimpelPos + (dist * f) * CAM_AXIS. Remaining, each allocation:
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

    start = ai_ClockMs;
    if (sel == sc->selected || sel > sc->maxSel)
        return;
    while (sv_RunFrame((int)ai_CannonSwitchAnim, mask) != 0) {
        ai_CannonPanel(rec, sc, run);
        if ((float)(ai_ClockMsVolatile - start) > ai_CannonSwitchMs) {
            sv_PopRequest = 1;
        } else {
            v.x = sc->dirX;
            v.y = 0;
            v.z = sc->dirZ;
            f = (float)(ai_ClockMsVolatile - start) / ai_CannonSwitchMs;
            d3_Normalize((char *)&v);
            v.x = ai_CannonSwitchDist * f * v.x;
            v.y = ai_CannonSwitchDist * f * v.y;
            v.z = ai_CannonSwitchDist * f * v.z;
            v.x = sc->pos.x - v.x;
            v.y = sc->pos.y - v.y;
            v.z = sc->pos.z - v.z;
            d3_SetObjectPos(sc->obj98, (char *)&v);
            w.x = CAM_AXIS.x;
            w.y = CAM_AXIS.y;
            w.z = CAM_AXIS.z;
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
    start = ai_ClockMs;
    while (sv_RunFrame((int)ai_CannonSwitchAnim, mask) != 0) {
        ai_CannonPanel(rec, sc, run);
        if ((float)(ai_ClockMsVolatile - start) > ai_CannonSwitchMs) {
            sv_PopRequest = 1;
        } else {
            v.x = sc->dirX;
            v.y = 0;
            v.z = sc->dirZ;
            f = 1.0f - (float)(ai_ClockMsVolatile - start) / ai_CannonSwitchMs;
            d3_Normalize((char *)&v);
            v.x = ai_CannonSwitchDist * f * v.x;
            v.y = ai_CannonSwitchDist * f * v.y;
            v.z = ai_CannonSwitchDist * f * v.z;
            v.x = sc->pos.x - v.x;
            v.y = sc->pos.y - v.y;
            v.z = sc->pos.z - v.z;
            d3_SetObjectPos(sc->obj98, (char *)&v);
            w.x = CAM_AXIS.x;
            w.y = CAM_AXIS.y;
            w.z = CAM_AXIS.z;
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
    sc->now = ai_ClockMs;
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
    D3VECTOR t;
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

int d2_CreatePicObj(); void d2_MoveObj(short x, short y, int obj); int d2_WinOpenPic(short x, short y, int pic, int win);
int d2_WinOpenSlider(); void d2_SetLabel(); void d2_SetList(int id, int n, ...);
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
    sc->resultClock = ai_ClockMs;
}

int he_Unk56f170(); int sv_MessageBox();

/* 0x580fe0, a scene check (49 B): 0x56f170 (the record + 1); when it answers non-zero, 0x521150 (0x144a19c,
   0); 1 when 0x56f170 answered 0, else 0 */
int ai_CanSpyOn(char *rec)
{
    int r;

    r = he_Unk56f170(U_I(rec + 1));
    if (r)
        sv_MessageBox(U_I((unsigned)&full_data_144a19c), 0);
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
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], amount, ai_OwnPlayer) != 0 && ai_CanSpyOn((char *)rec) != 0) {
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
                    msg.time = ai_TimeT;
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
    AIMSG msg;
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
                    sv_CheckMoney(ai_Chars[ai_PlayerId], amt[i]) == 1 && ai_CanSpyOn((char *)rec) != 0) {
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
                    msg.time = ai_TimeT;
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
    AIMSG msg;
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
        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText("$C");
        io_PrintText((char *)0x3ffd, 0x3ffb, p->id, n);
        io_PrintText((char *)0x3ffe, amt[i]);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                if (sv_CheckMoney(ai_Chars[ai_PlayerId], amt[i]) == 1 && ai_CanSpyOn((char *)rec) != 0) {
                    gm_SendMoneyDelta(((AICHAR *)ai_Chars[ai_PlayerId])->netId, -amt[i]);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x99;
                    msg.flags = 2;
                    msg.arg[3] = -1;
                    msg.arg[4] = 1;
                    msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                    msg.fromId = rec->id;
                    msg.time = ai_TimeT;
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

int he_NoEvent9bFor(); void txt_Format(char *out, int id0, ...); int sv_MessageBox();

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
   Sixteen orders of the ten stores tried (the best kept: arg[5] last, 22 ins). */
void ai_CabinetIntrigueWindow(AIBLD *rec)
{
    AIMSG msg;
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
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == btnPay) {
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) != 0 &&
                    sv_CheckMoney(ai_Chars[ai_PlayerId], 0x10) == 1 && ai_CanSpyOn((char *)rec) != 0) {
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
                    msg.time = ai_TimeT;
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

void sv_ReleaseNamePlates(void); int sv_RegisterNamePlate(); int ai_OpenTransport(); int ai_StorageWindow();
int ai_ProductionWindow(); void ai_Unk5823b0(); void ai_AlchemistResearchWindow(); void ai_AlchemistWindow();

/* 0x5821d0, the contact dispatcher (467 B): arg 1 = the record, arg 2 = the window's entry list. 0x52e190 once,
   then the pump loop 0x5309a0 (this dispatcher, 0x67fff): the escape 0x14673a0 -> 0x6f22a0 = 1; the contact
   flags 0x6f231c bit 0x200 -> the ids 0x52e1c0 ("contact_REZEPTBUCH" 0x62a4a8 / "contact_PRODUKTION_ALCHIMIST"
   0x62a488, the texts 0x144a2f8 / 0x1441f98, 0x16 / 0x13), bit 0x400 -> the ids "contact_CONTRACT" 0x62a474,
   "contact_LAGER" 0x62a464, "contact_TRANSPORT" 0x62a450, "contact_ALCHIMISTENVERTRAG" 0x62a434 (the texts
   0x144a2a0 / 0x1441f90 / 0x1441fa0 / 0x144a29c, 0xe / 0xe / 0x15 / 0x16); the clicked contact 0x6f22e4:
   TRANSPORT -> 0x5af1a0 (record); LAGER -> 0x5b26b0 (record, the record + 0x5d); the entry list +0x1c -> 0x5823b0
   (record, the list); PRODUCTION -> 0x4eb180 (record, &local, &local2, 0): 0 -> 0x58bd60 (record), else
   0x521150 (0x144a870 + the answer * 4, 0x100); REZEPTBUCH -> 0x57c690 (record); ALCHIMISTENVERTRAG ->
   0x57cf90 (record); CONTRACT -> 0x57bf00 (record) */
void ai_Unk5821d0(char *rec, char *list)
{
    int c1;
    int c2;
    int c3;
    int c4;
    int c5;
    int c6;
    int i;
    char a[4];
    char b[4];

    c1 = 0;
    c2 = 0;
    c3 = 0;
    c4 = 0;
    c5 = 0;
    c6 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk5821d0, 0x67fff) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            c6 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a4a8, (char *)U_I((unsigned)&full_data_144a2f8), 0x16);
            c5 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a488, (char *)U_I((unsigned)&full_data_1441f98), 0x13);
        }
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            c1 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a474, (char *)U_I((unsigned)&full_data_144a2a0), 0xe);
            c2 = sv_RegisterNamePlate((char *)(unsigned)"contact_LAGER", (char *)U_I((unsigned)&ai_ContactTextStorage), 0xe);
            c3 = sv_RegisterNamePlate((char *)(unsigned)"contact_TRANSPORT", (char *)U_I((unsigned)&ai_ContactTextTransport), 0x15);
            c4 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a434, (char *)U_I((unsigned)&full_data_144a29c), 0x16);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == c3) {
                ai_OpenTransport(rec);
            } else if (U_I((unsigned)&ai_ClickedObject) == c2) {
                ai_StorageWindow(rec, U_I(rec + 0x5d));
            } else if (U_I(list + 0x1c) == 0) {
                if (U_I((unsigned)&ai_ClickedObject) == c5) {
                    i = ahm_ResearchState(rec, a, b, 0);
                    if (i == 0)
                        ai_ProductionWindow(rec);
                    else
                        sv_MessageBox(U_I(((unsigned)&ai_Unk144a80c + 0x64) + i * 4), 0x100);
                } else if (U_I((unsigned)&ai_ClickedObject) == c6) {
                    ai_AlchemistResearchWindow(rec);
                } else if (U_I((unsigned)&ai_ClickedObject) == c4) {
                    ai_AlchemistWindow(rec);
                } else if (U_I((unsigned)&ai_ClickedObject) == c1) {
                    ai_AlchemistCandidates(rec);
                }
            } else {
                ai_Unk5823b0(rec, list);
            }
        }
    }
}

void cm_BeginFieldBlock(char *o, int id); int cm_AddFieldDelta(int size, int n, short field, void *v); void cm_SendFieldDeltas(void);

/* 0x5823b0, the unit purchase window (429 B): arg 1 = the record (unused), arg 2 = the entry list; its +0x20 is
   the price; the window "Locations_Domizil_kabinett" (0x62a418) with 0x476f80 / 0x475600, the sound 0x52ea30,
   the texts 0x602568 / 0x407d / 0x4171 (the price) and the entries 0x407e / +1 in 0x475b80 (window, 2, text); the
   pump loop 0x5309a0 (this handler, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry:
   0x520f90 (the character 0x770b80 + 0x6f25b8 * 0x21c, the price) == 1 -> 0x4c5f60 (the character, -the price),
   0x4c3ba0 (the list, the list + 2), 0x4c3c00 (4, 1, 0x1c, &0) / (4, 1, 0x20, &0), 0x4c49a0 () and the wait on
   0x4c3ae0 (pump 0x20007), 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window),
   0x52d7a0 () */
void ai_Unk5823b0(char *rec, char *list)
{
    int win;
    int b1;
    int b2;
    int price;
    int i;
    int v;

    price = U_I(list + 0x20);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a418);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(0x407d, 0x4171, price);
    d2_SetForm(win, 2);
    i = io_PrintText(0x407e);
    b1 = d2_GetWinObj(win, 2, i);
    b2 = d2_GetWinObj(win, 2, i + 1);
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, price) == 1) {
                    v = 0;
                    cm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -price);
                    cm_BeginFieldBlock(list, U_I(list + 2));
                    cm_AddFieldDelta(4, 1, 0x1c, &v);
                    cm_AddFieldDelta(4, 1, 0x20, &v);
                    i = ((int (*)())cm_SendFieldDeltas)();
                    if (i != -1) {
                        while (cm_GetSlotState(i) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk5823b0, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
/* 0x582560, the cannon contact dispatcher (159 B): arg 1 = the record, arg 2 = the entry list. 0x52e190 once,
   then the pump loop 0x5309a0 (this dispatcher, 0x67fff): the contact flags 0x6f231c bit 0x400 -> the id
   0x52e1c0 ("contact_KANONE" 0x62a4bc, the text 0x144a334, 0x16); the escape 0x14673a0 -> 0x6f22a0 = 1; the
   clicked contact 0x6f22e4: with the list +0x1c clear and the id KANONE -> 0x57e270 (the cannon scene); with the
   list set -> 0x5823b0 (the record, the list) */

void ai_Unk582560(char *rec, char *list)
{
    int k;
    int id;

    id = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk582560, 0x67fff) != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400)
            id = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a4bc, (char *)U_I((unsigned)&full_data_144a334), 0x16);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I(list + 0x1c) == 0) {
                if (U_I((unsigned)&ai_ClickedObject) == id)
                    ai_CannonScene(rec);
            } else {
                ai_Unk5823b0(rec, list);
            }
        }
    }
}
/* 0x582600, the spy contact dispatcher (289 B): arg 1 = the record, arg 2 = the entry list. 0x52e190 once, then
   the pump loop 0x5309a0 (this dispatcher, 0x67fff): the contact flags 0x6f231c bit 0x400 -> the ids 0x52e1c0
   ("contact_VERGIFTEN" 0x62a524 / "contact_SPIONE_AUSSCHALTEN" 0x62a508 / "contact_SCHLEICHENDE_LAEHMUNG"
   0x62a4e8 / "contact_BEWEISE_FAELSCHEN" 0x62a4cc, the texts 0x144a1a8 / 0x144a22c / 0x144a268 / 0x144a1ec,
   0x16); the escape 0x14673a0 -> 0x6f22a0 = 1; the clicked contact 0x6f22e4, with the list +0x1c clear:
   0x581950 / 0x581020 / 0x581db0 / 0x581400 (the record); with the list set -> 0x5823b0 (the record, the list) */

void ai_Unk582600(char *rec, char *list)
{
    int c1;
    int c2;
    int c3;
    int c4;

    c1 = 0;
    c2 = 0;
    c3 = 0;
    c4 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk582600, 0x67fff) != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x400) {
            c1 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a524, (char *)U_I((unsigned)&full_data_144a1a8), 0x16);
            c2 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a508, (char *)U_I((unsigned)&full_data_144a22c), 0x16);
            c3 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a4e8, (char *)U_I((unsigned)&full_data_144a268), 0x16);
            c4 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a4cc, (char *)U_I((unsigned)&full_data_144a1ec), 0x16);
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I(list + 0x1c) == 0) {
                if (U_I((unsigned)&ai_ClickedObject) == c1)
                    ai_CabinetSpyByMoney(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c2)
                    ai_CabinetSpyWindow(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c3)
                    ai_CabinetIntrigueWindow(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c4)
                    ai_CabinetSpyOnCharacter(rec);
            } else {
                ai_Unk5823b0(rec, list);
            }
        }
    }
}
/* 0x582730, the four contact buttons (399 B): arg 1 = the record (its +0x65 the entries' state record), arg 2 =
   the window list (0 the window, 4 the parent, 8 .. 0xb the buttons, 0xc the selected button). 0x475f70 (the
   window, 6) and the text 0x602568; the four buttons 0x4731d0 (i * 0x6c + 6, 0, the text 0x5eecb0[i], the parent)
   into the list 8 .. 0xb with the window table 0xf8f080 +0x4c = 1, +0x1c0 = 3 and the icon 0x91aaf0 + the table
   0x5eece0[i] * 0x41 at +0x2e8; the buttons enabled by the state bytes +0x19 / 0x1a / 0x1b; the selected 0xc:
   3 / 2 / 1 with its byte 1 -> that button marked at +0x2c, else the first button marked and 0xc = 0; the
   selected one marked at +0x28 */

void ai_Unk582730(char *rec, int *w)
{
    char *r;
    int i;
    int id;
    int sel;

    r = (char *)U_I(rec + 0x65);
    d2_SetForm(w[0], 6);
    io_PrintText((char *)(unsigned)"$C");
    for (i = 0; i < 4; i++) {
        id = d2_WinOpenPic(i * 0x6c + 6, 0, (char *)U_I(((unsigned)&sv_PhaseMinutes + 0x1f8) + i * 4), w[4]);
        w[8 + i] = id;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + id * 0x2ec) = 3;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x2e8 + id * 0x2ec) = U_I((unsigned)&ai_Goods) + U_I(((unsigned)&sv_PhaseMinutes + 0x228) + i * 4) * 0x41;
    }
    d2_SetObjEnabled(w[9], *(unsigned char *)(r + 0x19));
    d2_SetObjEnabled(w[10], *(unsigned char *)(r + 0x1a));
    d2_SetObjEnabled(w[0xb], *(unsigned char *)(r + 0x1b));
    sel = w[0xc];
    if (sel == 3 && *(char *)(r + 0x1b) == 1) {
        id = w[0xb];
    } else if (sel == 2 && *(char *)(r + 0x1a) == 1) {
        id = w[10];
    } else if (sel == 1 && *(char *)(r + 0x19) == 1) {
        id = w[9];
    } else {
        *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + w[8] * 0x2ec) = 1;
        w[0xc] = 0;
        goto done;
    }
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + id * 0x2ec) = 1;
done:
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + w[w[0xc] + 8] * 0x2ec) = 1;
}

int sv_CreateScrollButtons(); unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l); int gv_EntryIndex(char *r, int *e); int he_Unk573200();
void d2_ScrollWindow();

/* 0x5828c0, the contact list builder (635 B): rec = the record, w = the window list (0 the window, 2 the screen
   part, 4 the parent, 5 the font, 0xc the selected button, 0xd the entries of 4 dwords each, 0x4d the selected
   entry), sel = the newly clicked button (the current selection's entry 0xf8f368 + w[2] * 0x3c4 saved); 0x475f70
   (the window, 2) and (4) with the text 0x602568; the scroll area 0x52e860 (0, 0, w[5], w[2], 0x35, two locals,
   &sel); the entries 0x4ef7c0 (0, rec, the previous, 0, w[0xc]) each: +4 the entry, +8 its id, +0xc the byte
   0x4ef6b0 (the record + 0x65, the entry), the button 0x4731d0 (1, n * 0x35 + 2, the text 0x5eecb0 / 0x5eecc0 by
   the button, w[2]); the marker 0x4731d0 (0xf, n * 0x35 + 9, 0x7de) when 0x573200 (rec, the entry, the byte); the
   window table 0xf8f080 +0x4c = 1 and +0x1c0 = 3; the selected entry 0x4d clamped to 0 and marked at +0x2c / +0x28;
   the rest of the 0x10 entries filled (+0xd = -1, +0xe = 0, +0xf = 0xff, +0x10 = 0xff); the screen 0xf8f368 /
   0xf8f384 (w[2] * 0x3c4) restored and 0x46f840 (w[2]) */
void ai_Unk5828c0(char *rec, int *w, int sel)
{
    char a[4];
    char b[4];
    int *e;
    int *p;
    int saved;
    int n;
    int i;
    int id;

    saved = 0;
    if (sel == w[0xc])
        saved = U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[2] * 0x3c4);
    d2_SetForm(w[0], 2);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 4);
    io_PrintText((char *)(unsigned)"$C");
    sv_CreateScrollButtons(0, 0, w[5], w[2], 0x35, a, b, &sel);
    e = (int *)gv_FindEntry(0, rec, 0, 0, w[0xc]);
    n = 0;
    if (e) {
        p = w + 0xd;
        do {
            p[1] = (int)e;
            p[2] = *e;
            *(char *)(p + 3) = gv_EntryIndex(U_I(rec + 0x65), e);
            if (e[1] == -1)
                id = d2_WinOpenPic(1, n * 0x35 + 2, (char *)U_I(((unsigned)&sv_PhaseMinutes + 0x1f8) + w[0xc] * 4), w[2]);
            else
                id = d2_WinOpenPic(1, n * 0x35 + 2, (char *)U_I(((unsigned)&sv_PhaseMinutes + 0x208) + w[0xc] * 4), w[2]);
            *p = id;
            if (he_Unk573200(rec, *e, (char)p[3]) != 0)
                d2_WinOpenPic(0xf, n * 0x35 + 9, 0x7de, w[2]);
            *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + *p * 0x2ec) = 1;
            *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *p * 0x2ec) = 3;
            e = (int *)gv_FindEntry(0, rec, e, 0, w[0xc]);
            n++;
            p += 4;
        } while (e);
    }
    if (n <= w[0x4d])
        w[0x4d] = 0;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + w[0x4d] * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + w[w[0x4d] * 4 + 0xd] * 0x2ec) = 1;
    if (n < 0x10) {
        p = w + n * 4 + 0xf;
        for (i = 0; i < 0x10 - n; i++) {
            p[-2] = -1;
            p[-1] = 0;
            p[0] = 0xff;
            *(unsigned char *)(p + 1) = 0xff;
            p += 4;
        }
    }
    U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[2] * 0x3c4) = saved;
    U_I(((unsigned)&ai_Unkf8f13c + 0x248) + w[2] * 0x3c4) = 0x35;
    d2_ScrollWindow(w[2]);
}

void ai_Unk5830f0(); void ai_Unk583150();

/* 0x582b40, the contact detail panel (1449 B, ROUGH SKELETON: the entry collection, the item widgets and the
   header texts are written, the detail texts of the lower half are comments): rec = the record, w = the window
   list (0 the window, 3 the screen part, 4 / 5 / 6 the fonts, 0x4d the selected entry, 0x4e the detail window,
   0x50 the item records of 3 dwords each, 0x51 their widgets), clicked = the newly clicked entry, flag = 0 / 1.
   The selected entry w[w[0x4d] * 4 + 0xe] and its record; the sub-entries (+4, 0x44 bytes each, whose +0 != -1
   are collected); the list zeroed to -1 beyond; w[0x4e] = -1; the scroll position 0xf8f368 + w[3] * 0x3c4
   saved; the entries w[0] 3 / 5 / 7 with the text 0x602568 and the scroll area 0x52e860 (0, 0, w[6], w[3], the
   screen height 0xf8f12a + w[3] * 0x3c4); per item: the button 0x4731d0 (the centre, the row, 0x6c7) and its
   +0x3c child window 0x4731d0, the window table 0xf8f080 +0x4c = (flag == 0), +0x1c0 = 3, the two texts
   0x5830f0 / 0x583150 and the five value labels 0x473650 (0x28, 0x6002 / 0x16b7 + i ... 0x62a56c, width 300,
   0x4760b0 (0x54)) and 0x52eaa0 (0xa0, the row + i * 0xd, the item byte +0x24 + i, 0x4df); the position
   0xf8f368 restored, the detail 7 with the text 0x602568; the detail text: the plain name 0x5eecd0[*e] through
   0x5176d0 (0x4100) into 0x624204, or the character 0x505df0 (e[1]) and the three strings 0x62a54c (0x4101 the
   name 0x62a564, 0x4102 "AD: %i" 0x62a544, 0x4103 "%i ..." 0x62a53c with a __ftol value); with flag 1 the
   detail window 0x46bf10 (the centre, 0xbf, 0x144ad84) with 0x46c260 (0xb4); the position restored and
   0x46f840 (w[3]) */
void ai_Unk582b40(char *rec, int *w, int clicked, int flag)
{
    char buf1[0x100];
    char buf2[0x100];
    int *e;
    int n;
    int i;
    int k;
    int saved;

    e = (int *)w[w[0x4d] * 4 + 0xe];
    saved = 0;
    n = 0;
    if (*(int *)((unsigned)&gv_EntryCounts + *e * 4) > 0) {
        int *p = w + 0x51;
        for (i = 0; i < *(int *)((unsigned)&gv_EntryCounts + *e * 4); i++) {
            if (*(int *)(e[4] + i * 0x44) != -1) {
                p[-1] = e[4] + i * 0x44;
                p[-2] = -1;
                p[0] = *(int *)(e[4] + i * 0x44);
                n++;
                p += 3;
            }
        }
    }
    if (n < 8) {
        int *p = w + n * 3 + 0x51;
        for (i = 0; i < 8 - n; i++) {
            p[-2] = -1;
            p[-1] = 0;
            p[0] = -1;
            p += 3;
        }
    }
    w[0x4e] = -1;
    if (clicked == w[0x4d])
        saved = U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[3] * 0x3c4);
    d2_SetForm(w[0], 3);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 5);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 7);
    io_PrintText((char *)(unsigned)"$C");
    sv_CreateScrollButtons(0, 0, w[6], w[3], *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[3] * 0x3c4), buf2, buf1, &clicked);
    d2_SetForm(w[0], 3);
    /* ROUGH: the per-item widgets (0x4731d0 / 0x473650 / 0x5830f0 / 0x583150 / 0x52eaa0, see above) and the
       detail texts are not written yet */
    U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[3] * 0x3c4) = saved;
    d2_ScrollWindow(w[3]);
}
/* 0x5830f0, a contact entry's name line (90 B): the record's +0x2c text (the German +0x253 / the English +0x27b
   by the flag +0x38) with the two name parts +4 / +0x14 through 0x5176d0 ("%s %s %s %s" 0x623594), or just the
   two parts ("%s %s" 0x60c7b8) */

void ai_Unk5830f0(char *e, char *out)
{
    char *p;

    p = (char *)U_I(e + 0x2c);
    if (p) {
        if (U_I(e + 0x38) == 0)
            txt_Format(out, (char *)(unsigned)"%s %s %s", p + 0x253, e + 4, e + 0x14);
        else
            txt_Format(out, (char *)(unsigned)"%s %s %s", p + 0x27b, e + 4, e + 0x14);
        return;
    }
    txt_Format(out, (char *)(unsigned)"%s %s", e + 4, e + 0x14);
}
/* 0x583150, a contact entry's second line (108 B): the texts +0x30 / +0x34 (the German +0x114 / the English
   +0x11e and +0x130 / +399 by the flag +0x38): both ("%s - %s" 0x62a570), one ("%s" 0x5f8318), or empty */

void ai_Unk583150(char *e, char *out)
{
    char *a;
    char *b;

    a = (char *)U_I(e + 0x30);
    if (a) {
        b = (char *)U_I(e + 0x34);
        if (b) {
            if (U_I(e + 0x38) == 0)
                txt_Format(out, (char *)(unsigned)&full_data_62a570, a + 0x114, b + 0x130);
            else
                txt_Format(out, (char *)(unsigned)&full_data_62a570, a + 0x11e, b + 399);
            return;
        }
        if (U_I(e + 0x38) == 0) {
            txt_Format(out, "%s", a + 0x114);
            return;
        }
        txt_Format(out, "%s", a + 0x11e);
        return;
    }
    b = (char *)U_I(e + 0x34);
    if (!b) {
        *out = 0;
        return;
    }
    if (U_I(e + 0x38) == 0) {
        txt_Format(out, "%s", b + 0x130);
        return;
    }
    txt_Format(out, "%s", b + 399);
}

void d2_SetViewEnabled(int view, int on);

/* 0x583240, the graveyard contact window (1205 B, ROUGH SKELETON): out = the picked entry (0 / the entry, its
   id, the byte 0x4ef6b0, the second entry, its id), rec = the record, title = the window's title text, flag =
   0 / 1, cb = the callback. The window "Locations_Friedhof_Graves" (0x62a580) with 0x476f80 (0, 0) / 0x475600,
   the sound 0x52ea30, the texts 0x62a578 ("%C Z %s", title) and the sections 2 / 3 / 6 / 4 / 5 / 7 (0x6ed938 is
   kept per section); then the buttons 0x582730 (rec, w), the list 0x5828c0 (rec, w, -1) and the detail
   0x582b40 (rec, w, -1, flag); 0xf8a668 = -1 and the pump loop 0x5309a0 (this window, 0x677c7): the clicks on
   the four buttons (w + 0x??, the selection 0x170), the entries (0x6c) and the detail (param_4 = 1, w + 0x68)
   are dispatched here: a button sets the selection and redraws (0x82730 / 0x828c0 / 0x82b40 again, the second
   from the entry index), an entry fills out (the entry / its id / 0x4ef6b0 / the second entry) and with no
   callback sets 0x6f22a0 = 1 or calls cb (rec, &the entry) between 0x475590 (the window, 0) / (1); the timer
   0x6333cc + 0x96 redraws the three parts each tick; the escape 0x14673a0 -> 0x6f22a0 = 1 */
void ai_Unk583240(int *out, char *rec, char *title, int flag, void (*cb)())
{
    int w[0x50];
    int win;
    int sel;
    int entry;
    int redraw;
    int tick;
    int i;

    if (out) {
        out[0] = 0;
        out[3] = 0;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a580);
    memset(w, 0, sizeof(w));
    w[0] = win;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText((char *)(unsigned)&full_data_62a578, title);
    d2_SetForm(win, 2);
    d2_SetForm(win, 3);
    d2_SetForm(win, 6);
    d2_SetForm(win, 4);
    d2_SetForm(win, 5);
    d2_SetForm(win, 7);
    sel = 0;
    entry = 0;
    ai_Unk582730(rec, w);
    ai_Unk5828c0(rec, w, -1);
    ai_Unk582b40(rec, w, -1, flag);
    U_I((unsigned)&d2_ClickedId) = -1;
    /* ROUGH: the click dispatch (the four buttons, the 16 entries and the detail) and the redraw flags are not
       written yet; the window list w matches local_1a0, its selection w[0x170] / entry w[0x6c] the locals */
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk583240, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
        tick = U_I((unsigned)&ai_ClockMs);
        (void)tick;
        (void)sel;
        (void)entry;
        (void)cb;
        (void)i;
        (void)redraw;
    }
}
/* 0x583700, the graveyard detail panel (1123 B, ROUGH: the collection, the item widgets and the three detail
   texts are written, the layout numbers are from the disassembly): w = the window list (0 the window, 2 the
   screen part, 4 the font, 7 the selected entry record, 0xa the item buttons, 0xb the item records, 0xc the
   entries of 3 dwords each). The selected entry's sub-entries (0x44 bytes each, whose +0 != -1) collected into
   w + 0xc and the rest zeroed to -1; the scroll position 0xf8f368 + w[2] * 0x3c4 saved; the sections 2 / 3 / 5
   with the text 0x602568 and the scroll area 0x52e860 (0, 0, w[4], w[2], the height); per item: the button
   0x4731d0 (the centre, the row, 0x6c7) and its child window 0x4731d0 (the entry + 0x3c), the window table
   0xf8f080 +0x4c = 1 / +0x1c0 = 3, the name / second line 0x5830f0 / 0x583150 as two windows 0x473650 and the
   five value labels 0x473650 (0x28) / 0x52eaa0 (0xa0, 0x4df); the position restored; the detail 5 with
   0x602568 and the character's name 0x770b80 + 0x6f25b8 * 0x21c through 0x5176d0 (0x62a564) and 0x515960
   (0x62a54c, 0x4101), "AD: %i" 0x62a544 (0x4102, the word at the entry + 0xa + 0x578) and "%i ..." 0x62a53c
   (0x4103, a 0x5de518 conversion) */

void ai_Unk583700(int *w)
{
    char buf1[0x100];
    char buf2[0x100];
    char a[4];
    char b[4];
    char c[4];
    int *e;
    int *p;
    int n;
    int k;
    int i;
    int id;
    int x;
    int saved;

    n = 0;
    e = (int *)w[7];
    saved = U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[2] * 0x3c4);
    if (*(int *)((unsigned)&gv_EntryCounts + *e * 4) > 0) {
        p = w + 0xc;
        for (i = 0; i < *(int *)((unsigned)&gv_EntryCounts + *e * 4); i++) {
            if (*(int *)(e[4] + i * 0x44) != -1) {
                p[-1] = e[4] + i * 0x44;
                n++;
                p[-2] = -1;
                p[0] = *(int *)(e[4] + i * 0x44);
                p += 3;
            }
        }
    }
    if (n < 8) {
        p = w + n * 3 + 0xc;
        for (i = 0; i < 8 - n; i++) {
            p[-2] = -1;
            p[-1] = 0;
            p[0] = -1;
            p += 3;
        }
    }
    d2_SetForm(w[0], 2);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 3);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(w[0], 5);
    io_PrintText((char *)(unsigned)"$C");
    sv_CreateScrollButtons(0, 0, w[4], w[2], *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4), b, a, c);
    d2_SetForm(w[0], 2);
    for (k = 0; k < n; k++) {
        x = *(short *)((unsigned)&ai_Unkf8f128 + w[2] * 0x3c4) - *(short *)((unsigned)&d2_Pics + 0x2399c);
        id = d2_WinOpenPic(x / 2, (*(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4) * k) + 2, 0x6c7, w[2]);
        w[0xb + k * 3 - 1] = id;
        id = d2_WinOpenPic(x / 2 + 2, (*(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4) * k) + 4, *(int *)(w[0xb + k * 3] + 0x3c), w[2]);
        w[0xb + k * 3 - 1] = id;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + w[0xb + k * 3 - 1] * 0x2ec) = 3;
        ai_Unk5830f0((char *)w[0xb + k * 3], buf1);
        ai_Unk583150((char *)w[0xb + k * 3], buf2);
        x = *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4) * k;
        id = d2_CreateWinText(0, x + 0x38, buf1, w[2]);
        *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + id * 0x2ec) = 1;
        *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + w[2] * 0x3c4);
        d2_SetObjFont(id, 0x54);
        id = d2_CreateWinText(0, x + 0x45, buf2, w[2]);
        *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + id * 0x2ec) = 1;
        *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + w[2] * 0x3c4);
        d2_SetObjFont(id, 0x54);
        x += 0x57;
        for (i = 0; i < 5; i++) {
            txt_Format(buf1, (char *)(unsigned)&full_data_62a56c, i + 0x16b7);
            id = d2_CreateWinText(0x28, x, buf1, w[2]);
            *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = 300;
            d2_SetObjFont(id, 0x54);
            sv_ValueBar(0xa0, x, *(unsigned char *)(w[0xb + k * 3] + 0x24 + i), 0x4df);
            x += 0xd;
        }
    }
    U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + w[2] * 0x3c4) = saved;
    d2_SetForm(w[0], 5);
    io_PrintText((char *)(unsigned)"$C");
    txt_Format(buf1, (char *)(unsigned)&full_data_62a564, *(int *)((unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
    io_PrintText((char *)(unsigned)&full_data_62a54c, 0x4101, buf1);
    txt_Format(buf1, (char *)(unsigned)&full_data_62a544, *(unsigned short *)((char *)e + 10) + 0x578);
    io_PrintText((char *)(unsigned)&full_data_62a54c, 0x4102, buf1);
    txt_Format(buf1, (char *)(unsigned)&full_data_62a53c, (int)_ftol());
    io_PrintText((char *)(unsigned)&full_data_62a54c, 0x4103, buf1);
}
/* 0x583b70, opens the graveyard detail window (209 B): w = the window list (7 the selected entry record), title
   = its title; 0 without a selected entry, else "Locations_Friedhof_RentAGrave2" (0x62a59c) with 0x476f80
   (0, 0) / 0x475600, the sound 0x52ea30, the texts 0x62a578 ("%C Z %s", title) and 0x602568, the sections
   1 / 2 / 3 / 5 / 4 (0x6ed938 kept in w[1] / w[2] / w[4] / w[5] / w[6]), then 0x583700 (w); 1 */

int ai_Unk583b70(int *w, char *title)
{
    int win;

    if (w[7] == 0)
        return 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a59c);
    w[0] = win;
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(w[0]);
    d2_SetForm(w[0], 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(w[0], 1);
    w[1] = U_I((unsigned)&d2_LastWindow);
    io_PrintText((char *)(unsigned)&full_data_62a578, title);
    d2_SetForm(w[0], 2);
    w[2] = U_I((unsigned)&d2_LastWindow);
    d2_SetForm(w[0], 3);
    w[4] = U_I((unsigned)&d2_LastWindow);
    d2_SetForm(w[0], 5);
    w[5] = U_I((unsigned)&d2_LastWindow);
    d2_SetForm(w[0], 4);
    w[6] = U_I((unsigned)&d2_LastWindow);
    io_PrintText((char *)(unsigned)"$C");
    ai_Unk583700(w);
    return 1;
}
/* 0x583c50, the four graveyard buttons (396 B, the 0x582730 shape): rec = the record (its +0x65 the state
   record), win / section = the window and its section, btns = the four button ids out, selp = the selected one.
   0x475f70 (win, section) and the text 0x602568; the four buttons 0x4731d0 (i * 0x6c + 6, 0, the text
   0x5eecb0[i], 0x6ed938) with the window table 0xf8f080 +0x4c = 1, +0x1c0 = 3 and the icon 0x91aaf0 + the table
   0x5eece0[i] * 0x41 at +0x2e8; the buttons 1 .. 3 enabled by the state bytes +0x19 / 0x1a / 0x1b; the selected
   *selp 3 / 2 / 1 with its byte 1 -> that button marked at +0x2c, else the first marked and *selp = 0; the
   selected one marked at +0x28 */

void ai_Unk583c50(char *rec, int win, int section, int *btns, int *selp)
{
    char *r;
    int i;
    int id;
    int sel;

    r = (char *)U_I(rec + 0x65);
    d2_SetForm(win, section);
    io_PrintText((char *)(unsigned)"$C");
    for (i = 0; i < 4; i++) {
        id = d2_WinOpenPic(i * 0x6c + 6, 0, (char *)U_I(((unsigned)&sv_PhaseMinutes + 0x1f8) + i * 4), U_I((unsigned)&d2_LastWindow));
        btns[i] = id;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + btns[i] * 0x2ec) = 3;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x2e8 + btns[i] * 0x2ec) = U_I((unsigned)&ai_Goods) + U_I(((unsigned)&sv_PhaseMinutes + 0x228) + i * 4) * 0x41;
    }
    d2_SetObjEnabled(btns[1], *(unsigned char *)(r + 0x19));
    d2_SetObjEnabled(btns[2], *(unsigned char *)(r + 0x1a));
    d2_SetObjEnabled(btns[3], *(unsigned char *)(r + 0x1b));
    sel = *selp;
    if (sel == 3 && *(char *)(r + 0x1b) == 1) {
        id = btns[3];
    } else if (sel == 2 && *(char *)(r + 0x1a) == 1) {
        id = btns[2];
    } else if (sel == 1 && *(char *)(r + 0x19) == 1) {
        id = btns[1];
    } else {
        *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + btns[0] * 0x2ec) = 1;
        *selp = 0;
        goto done;
    }
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + id * 0x2ec) = 1;
done:
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + btns[*selp] * 0x2ec) = 1;
}

int gv_OrderSetGraveHolder(); int gv_SizeOpen();

/* 0x583de0, a graveyard confirmation window (272 B): rec / the two values passed on to 0x4f0900; the window
   "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x410f /
   0x410e and the entries 0x4111 / +1 in 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0
   (this window, 0x677c7): the click 0x14673a0 -> 0x6f22a0 = 1; the first entry -> 0x4f0900 (0, rec, the two
   values), 1; the second -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the result */
int ai_Unk583de0(char *rec, int a, unsigned char b)
{
    int win;
    int b1;
    int b2;
    int v;
    int done;

    done = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x410f, 0x410e);
    d2_SetForm(win, 2);
    v = io_PrintText(0x4111);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                gv_OrderSetGraveHolder(0, rec, a, b);
                done = 1;
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk583de0, 0x677c7) == 0) {
            d2_CloseView(win);
            return done;
        }
    }
}
/* 0x583ef0, enables the graveyard buttons by the state record (277 B): rec = the record (its +0x65 the state
   record), from = the last drawn index, btns = the four button ids, sel = the selected one. The buttons 0 ..
   from disabled (0x475e40 (the button, 0) and the window table +0x28 = 0); the buttons from .. 3: 0x4ef4f0 (the
   state record, i) -> 0 enabled / 0 (0x475e40 (the button, 0) and +0x28 = 0), else enabled and the first one
   kept; with the selected button marked in the table (+0x3c) and some button enabled, the first enabled
   becomes the selection and is marked at +0x28; 1 when any button is enabled, else 0 */

int ai_Unk583ef0(char *rec, int from, int unused, int *btns, int *sel)
{
    int i;
    int any;
    int r;

    from++;
    any = -1;
    for (i = 0; i < from; i++) {
        d2_SetObjEnabled(btns[i], 0);
        *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + btns[i] * 0x2ec) = 0;
    }
    r = 0;
    for (i = from; i < 4; i++) {
        if (gv_SizeOpen(U_I(rec + 0x65), i) == 0) {
            d2_SetObjEnabled(btns[i], 0);
            *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + btns[i] * 0x2ec) = 0;
        } else {
            r = 1;
            d2_SetObjEnabled(btns[i], 1);
            if (any == -1)
                any = i;
        }
    }
    if (*(int *)(U_I((unsigned)&d2_Objs) + 0x3c + btns[*sel] * 0x2ec) != 0 && any != -1) {
        *sel = any;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + btns[any] * 0x2ec) = 1;
    }
    return r;
}

void gv_OrderMoveGrave(int *c, int to, char *o, int from, int entry, int price); int gv_OrderReserveGrave(int *g, char *o, int v, short s, int x);

/* 0x584010, the graveyard rent confirmation (587 B): rec = the record (its +0x27 the grave's owner word), a / b
   the values passed on, idx = the grave index; the cost 16000 when the owner is not the player (0x6f25b8), else
   0; the window "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the text
   0x4115 / 0x4114 (0x4112, the name 0x5eecd0[idx], the cost), the entries 0x4116 / +1 in 0x475b80 (window, 2,
   text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the click 0x14673a0 -> 0x6f22a0 = 1; the
   first entry: with a cost, 0x521040 (the character 0x770b80 + 0x6f25b8 * 0x21c, the cost, 0x6f1bc5) 0 -> the
   pump; 0x4f0e80 (the character, idx, rec, a, b, the cost) and its answer waited on 0x4c3ae0 (pump 0x20007),
   0x760638 = 0x677c7; the answer 2 -> the text 0x411b (0x4112, the name) through 0x5176d0 and 0x521150
   (0x100), else 0x6f22a0 = 1 and 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the
   result */
int ai_Unk584010(char *rec, int a, unsigned char b, int idx)
{
    char buf[0x200];
    int win;
    int b1;
    int b2;
    int v;
    int r;
    int h;
    unsigned int cost;

    r = 0;
    cost = -(unsigned int)(*(unsigned short *)(rec + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId) & 16000;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    if (*(unsigned short *)(rec + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId) {
        io_PrintText(0x4115, 0x4112, (int)rec, U_I(((unsigned)&sv_PhaseMinutes + 0x218) + idx * 4));
    } else {
        io_PrintText(0x4114, 0x4112, U_I(((unsigned)&sv_PhaseMinutes + 0x218) + idx * 4), cost);
    }
    d2_SetForm(win, 2);
    v = io_PrintText(0x4116);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                if (cost != 0) {
                    if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, cost, *(unsigned char *)(unsigned)&ai_OwnPlayer) == 0)
                        goto pump;
                }
                h = ((int (*)(void *, int, int, int, int, int))gv_OrderMoveGrave)((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, idx, rec, a, b, cost);
                if (h != -1) {
                    while (cm_GetSlotState(h) == 0)
                        sv_RunFrame(0, 0x20007);
                }
                U_I((unsigned)&sv_FrameMask) = 0x677c7;
                if (cm_GetSlotState(h) == 2) {
                    txt_Format(buf, (char *)0x411b, 0x4112, U_I(((unsigned)&sv_PhaseMinutes + 0x218) + idx * 4));
                    sv_MessageBox(buf, 0x100);
                } else {
                    U_I((unsigned)&sv_PopRequest) = 1;
                    r = 1;
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
pump:
        if (sv_RunFrame(ai_Unk584010, 0x677c7) == 0) {
            d2_CloseView(win);
            return r;
        }
    }
}
/* 0x584260, the rent-a-grave window (427 B): rec = the record, a / b the values passed on; the window
   "Locations_Friedhof_RentAGrave" (0x62a5d8) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x4113
   (0x4112) and 0x625150 (0x4119) in the entry 0x475b80 (window, 3), the four buttons 0x583c50 (rec, window, 2,
   the buttons, the selection), the confirm enabled by 0x583ef0 (rec, a, b, the buttons, the selection),
   0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the
   confirm -> 0x475590 (window, 0), 0x584010 (rec, a, b, the selection) == 1 -> 1 and 0x6f22a0 = 1,
   0x475590 (window, 1); a clicked button != the selection -> the selection, 0x583c50 again and 0x583ef0 again;
   the pump 0 -> 0x4758f0 (window), the result */

int ai_Unk584260(char *rec, int a, int b)
{
    int win;
    int btn;
    int sel;
    int r;
    int i;
    int btns[4];

    sel = 0;
    r = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5d8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x4113, 0x4112);
    ai_Unk583c50(rec, win, 2, btns, &sel);
    d2_SetForm(win, 3);
    i = io_PrintText((char *)(unsigned)&full_data_625150, 0x4119);
    btn = d2_GetWinObj(win, 3, i);
    d2_SetObjEnabled(btn, ai_Unk583ef0(rec, a, b, btns, &sel));
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == btn) {
                d2_SetViewEnabled(win, 0);
                if (ai_Unk584010(rec, a, b, sel) == 1) {
                    r = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
                d2_SetViewEnabled(win, 1);
            } else {
                for (i = 0; i < 4; i++) {
                    if (U_I((unsigned)&d2_PointerObj) == btns[i] && i != sel) {
                        sel = i;
                        ai_Unk583c50(rec, win, 2, btns, &sel);
                        d2_SetObjEnabled(btn, ai_Unk583ef0(rec, a, b, btns, &sel));
                        break;
                    }
                }
            }
        }
        if (sv_RunFrame(ai_Unk584260, 0x677c7) == 0) {
            d2_CloseView(win);
            return r;
        }
    }
}
/* 0x584410, the graveyard confirmation with two buttons (264 B): rec / w the window record (its +0 the window
   handle, +0x18 the form, +0x1c the state object, +0x20 the value passed on, +0x24 the byte passed on). Two
   buttons via 0x52e350 in the form w[6] at height 0, their labels U_I (0x144a620) / U_I (0x144a624); when the
   state object's [0] == 3 the second button is disabled (0x475e40 (the button, 0)); 0xf8a668 = -1; the pump loop
   0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first button -> 0x475590 (the
   window, 0), 0x583de0 (rec, w[8], w+0x24) == 1 -> 0x6f22a0 = 1, 0x475590 (the window, 1); the second button ->
   the same through 0x584260 (the rent-a-grave window); the window is released by the caller, the pump 0
   returns. Caller: the graveyard handler 0x5871e0 after 0x583b70 (which fills the record w). */

void ai_Unk584410(char *rec, int *w)
{
    int btns[2];

    sv_ButtonRow(w[6], 0, btns, 2, U_I((unsigned)&full_data_144a620), U_I((unsigned)&full_data_144a624));
    if (*(int *)w[7] == 3)
        d2_SetObjEnabled(btns[1], 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == btns[0]) {
                d2_SetViewEnabled(w[0], 0);
                if (ai_Unk583de0(rec, w[8], *(unsigned char *)((char *)w + 0x24)) == 1)
                    U_I((unsigned)&sv_PopRequest) = 1;
                d2_SetViewEnabled(w[0], 1);
            } else if (U_I((unsigned)&d2_PointerObj) == btns[1]) {
                d2_SetViewEnabled(w[0], 0);
                if (ai_Unk584260(rec, w[8], *(unsigned char *)((char *)w + 0x24)) == 1)
                    U_I((unsigned)&sv_PopRequest) = 1;
                d2_SetViewEnabled(w[0], 1);
            }
        }
        if (sv_RunFrame(ai_Unk584410, 0x677c7) == 0)
            return;
    }
}
/* 0x584520, the graveyard grave action (632 B): rec / idx the grave index. The cost when the grave's owner
   (+0x27) is not the player (0x6f25b8): 0x4f2330 (the character, 0xff, idx, 1, &out), else 0; the window
   "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the text 0x410c /
   0x4109 (rec, the name 0x5eecd0[idx]) for the player's own grave or 0x410b / 0x4108 (the cost, the name), the
   entries 0x410d / +1 in 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window,
   0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: with a cost, 0x521040 (the character, the
   cost, 0x6f1bc5) 0 -> the pump; 0x4f0430 (the character, rec, idx, 10, the cost) and its answer waited on
   0x4c3ae0 (pump 0x20007), 0x760638 = 0x677c7; the answer 2 -> 0x5176d0 (buf, 0x411b, (the owner is the player)
   + 0x4108, the name) and 0x521150 (buf, 0x100), else 0x6f22a0 = 1 and 1; the second entry -> 0x6f22a0 = 1;
   the pump 0 -> 0x4758f0 (window), the result */

int ai_Unk584520(char *rec, int idx)
{
    char buf[0x200];
    int out;
    int win;
    int b1;
    int b2;
    int v;
    int r;
    int h;
    int cost;

    r = 0;
    if (*(unsigned short *)(rec + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId)
        cost = 0;
    else
        cost = gv_LeasePrice((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0xff, idx, 1, &out);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    if (*(unsigned short *)(rec + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId) {
        io_PrintText(0x410c, 0x4109, (int)rec, U_I(((unsigned)&sv_PhaseMinutes + 0x218) + idx * 4));
    } else {
        io_PrintText(0x410b, 0x4108, U_I(((unsigned)&sv_PhaseMinutes + 0x218) + idx * 4), cost);
    }
    d2_SetForm(win, 2);
    v = io_PrintText(0x410d);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                if (cost != 0) {
                    if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, cost, *(unsigned char *)(unsigned)&ai_OwnPlayer) == 0)
                        goto pump;
                }
                h = gv_OrderReserveGrave((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, rec, idx, 10, cost);
                if (h != -1) {
                    while (cm_GetSlotState(h) == 0)
                        sv_RunFrame(0, 0x20007);
                }
                U_I((unsigned)&sv_FrameMask) = 0x677c7;
                if (cm_GetSlotState(h) == 2) {
                    txt_Format(buf, (char *)0x411b, (*(unsigned short *)(rec + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId) + 0x4108, U_I(((unsigned)&sv_PhaseMinutes + 0x218) + idx * 4));
                    sv_MessageBox(buf, 0x100);
                } else {
                    U_I((unsigned)&sv_PopRequest) = 1;
                    r = 1;
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
pump:
        if (sv_RunFrame(ai_Unk584520, 0x677c7) == 0) {
            d2_CloseView(win);
            return r;
        }
    }
}
/* 0x5847a0, the rent-a-grave selection window (340 B): rec; the window "Locations_Friedhof_RentAGrave"
   (0x62a5d8) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x410a / 0x4108 and 0x625150 (0x4117) in
   the entry 0x475b80 (window, 3), the four buttons 0x583c50 (rec, window, 2, the buttons, the selection),
   0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the
   confirm -> 0x475590 (window, 0), 0x584520 (rec, the selection) == 1 -> 0x6f22a0 = 1, 0x475590 (window, 1); a
   clicked button != the selection -> the selection, 0x583c50 again; the pump 0 -> 0x4758f0 (window) */

void ai_Unk5847a0(char *rec)
{
    int win;
    int btn;
    int sel;
    int i;
    int btns[4];

    sel = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5d8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x410a, 0x4108);
    ai_Unk583c50(rec, win, 2, btns, &sel);
    d2_SetForm(win, 3);
    i = io_PrintText((char *)(unsigned)&full_data_625150, 0x4117);
    btn = d2_GetWinObj(win, 3, i);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == btn) {
                d2_SetViewEnabled(win, 0);
                if (ai_Unk584520(rec, sel) == 1)
                    U_I((unsigned)&sv_PopRequest) = 1;
                d2_SetViewEnabled(win, 1);
            } else {
                for (i = 0; i < 4; i++) {
                    if (U_I((unsigned)&d2_PointerObj) == btns[i] && i != sel) {
                        sel = i;
                        ai_Unk583c50(rec, win, 2, btns, &sel);
                        break;
                    }
                }
            }
        }
        if (sv_RunFrame(ai_Unk5847a0, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
    }
}

int sv_MessageBoxGreen(); void sv_OpenPlayerBar(int a, char k); void sv_ClosePlayerBar(void);

/* 0x584900, the graveyard grave action with a character (615 B, a start Ghidra missed: the callback is reached
   only through a table; the ledger found the dword 0x584b31): rec / ev the event record with +4 (a dword) and
   +8 (a byte). When 0x573200 (rec, ev+4, ev+8) finds a running event of the type 0x8d, the green message box
   0x521410 (U_I (0x144a638), U_I (0x144a63c), 0) and 0; else the player bar 0x525220 (rec, 0), the window
   "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x4120
   (0x411e, 0x228) and 0x4121 / +1 in 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0 (this
   window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: the first selected character (the
   byte +0x188 of the records 0x770b80 + i * 0x21c, i = 0 .. 0x2ff), none -> 0x521150 (U_I (0x144a40c), 0),
   else the message (type 0x8d at +4, 1 at +0x36, the character 0x770b84 + rec's owner word +0x27 * 0x21c at
   +8, rec + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at +0x28, the character record
   +4 at +0x38, ev +4 at +0x5c, ev +8 at +0x60) through 0x4c4b70, its answer waited on 0x4c3ae0 (pump 0x20007),
   and 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x525440 (), 0x52ae40 (), the
   result */
int ai_Unk584900(char *rec, char *ev)
{
    char msg[0xf8];
    char *p;
    int win;
    int b1;
    int b2;
    int v;
    int i;
    int r;
    int h;

    r = 0;
    if (he_Unk573200(rec, *(int *)(ev + 4), *(unsigned char *)(ev + 8)) != 0) {
        sv_MessageBoxGreen(U_I((unsigned)&full_data_144a638), U_I((unsigned)&full_data_144a63c), 0);
        return 0;
    }
    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x4120, 0x411e, 0x228);
    d2_SetForm(win, 2);
    v = io_PrintText(0x4121);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                p = 0;
                for (i = 0; i < 0x300; i++) {
                    if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                        p = (char *)((unsigned)&ai_Chars + i * 0x21c);
                        break;
                    }
                }
                if (p == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
                } else {
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x8d;
                    *(unsigned char *)(msg + 0x36) = 1;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x38) = *(int *)(p + 4);
                    *(int *)(msg + 0x5c) = *(int *)(ev + 4);
                    *(unsigned char *)(msg + 0x60) = *(unsigned char *)(ev + 8);
                    h = cm_SendLongMessage(msg);
                    if (h != -1) {
                        while (cm_GetSlotState(h) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    r = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk584900, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_ClearSelection();
            return r;
        }
    }
}

int gv_RoomForRaised(unsigned char *o, int *n); int gv_OrderRaiseDead(); int cm_GetSlotValue(int h); int sprintf(char *buf, const char *fmt, ...);

/* 0x584b70, the graveyard "Gehilfen erschaffen" window (501 B, a start Ghidra missed: the callback is reached
   only through a table; the ledger found the dword 0x584d3b): o / ev the event record (its +0 a pointer to a
   dword, +8 a byte, +0xc a pointer to a dword, +0x10 a dword). 0x4f1d50 (o, &n) counts the graveyard's living
   characters: 1 (below the limits) -> the window "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 /
   0x475600, the sound 0x52ea30, the texts 0x4132 (0x4131, 0x229) and 0x4133 / +1 in 0x475b80 (window, 2,
   text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1;
   the first entry: when *(int *)*(ev + 0xc) == -1 -> 0x6f22a0 = 1; else 0x4f1170 (o, *(int *)*(int *)ev,
   ev[8], *(int *)(ev + 0x10), 0x1c) raises the dead, its answer waited on 0x4c3ae0 (pump 0x20007) and
   0x4c3b10 (h) gives the helper, its value +4 the line "sp_%u" (0x62a62c), then the script
   "LOCATIONS\\FRIEDHOF\\Gehilfen_erschaffen_FRIEDHOF.esc" (0x62a5f8) run with the one argument (0x4287b0 /
   0x429ab0) and 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the result; 0 (the limit
   reached) -> the text 0x4135 (n, 0x229, o) through 0x5176d0 and 0x521150 (buf, 0), 0; -1 -> 0 */
int ai_Unk584b70(char *o, int *ev)
{
    char buf[0x200];
    char line[0x40];
    int win;
    int b1;
    int b2;
    int n;
    int r;
    int v;
    int h;
    char *a;

    r = 0;
    v = gv_RoomForRaised(o, &n);
    if (v == 1) {
        win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        sv_FitForm(U_I((unsigned)&d2_LastWindow));
        d2_SetForm(win, 1);
        io_PrintText(0x4132, 0x4131, 0x229);
        d2_SetForm(win, 2);
        h = io_PrintText(0x4133);
        b1 = d2_GetWinObj(win, 2, h);
        b2 = d2_GetWinObj(win, 2, h + 1);
        U_I((unsigned)&d2_ClickedId) = -1;
        for (;;) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&d2_PointerObj) == b1) {
                    if (*(int *)*(int *)((char *)ev + 0xc) == -1) {
                        U_I((unsigned)&sv_PopRequest) = 1;
                    } else {
                        h = gv_OrderRaiseDead(o, *(int *)*(int *)ev, *(unsigned char *)((char *)ev + 8), *(int *)((char *)ev + 0x10), 0x1c);
                        if (h != -1) {
                            while (cm_GetSlotState(h) == 0)
                                sv_RunFrame(0, 0x20007);
                        }
                        a = (char *)cm_GetSlotValue(h);
                        if (a != 0) {
                            sprintf(line, (char *)(unsigned)&full_data_62a62c, *(int *)(a + 4));
                            evt_RunScriptArgs(evt_Load((char *)(unsigned)&full_data_62a5f8), 1, line);
                        }
                        r = 1;
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
            if (sv_RunFrame(ai_Unk584b70, 0x677c7) == 0) {
                d2_CloseView(win);
                return r;
            }
        }
    }
    if (v == 0) {
        txt_Format(buf, (char *)0x4135, n, 0x229, o);
        sv_MessageBox(buf, 0);
    }
    return 0;
}

int sv_CheckCharacterFlag(); char * he_FindEvent(int n, ...); char * he_NextEvent(void); void sv_OpenWindowFrame(int f, int b, int c); void d2_SetCaption();
void ai_Unk58bbd0(void); int ai_Unk58d720(); void ai_Unk58dd00(int param_1, int param_2, int param_3, int param_4);

/* 0x584d70, the graveyard "create helpers" window (1549 B, ROUGH: the calls and the flow are complete): b = the
   building record (byte +0 the building kind, +1 the id, +0x27 the owner word, +0x5d the owner record).
   0x4ffe90 (b+0x5d, 2, 6, 0, 0x2a) kept; 0x520fe0 (b) ends the handler when it fails. The building table
   0x7e3f30 + kind * 0x24d: the kind 0x1f and the limit byte +0x247 - two grave kinds 0x225 / 0x226 (0x293 /
   0x294 for the kind 0x1f) and a third 0x227 / 0x295 with the limit >= 2 (n = 2 / 3). The running 0x8c events
   0x53e400 (2, 0, 0x8c, 3, b + 1) whose +0xac word is a kind count their non-(-1) +0x8c[4] entries. The window
   "HANDEL\\HANDEL_SUCHEN" (0x62a634) with 0x475600, 0x475f70 (window, 0 / 3), 0x52e6d0 (0x6ed938, 0x70f,
   0x741) and the text 0x421e / 0x4128 (kind 0x1f), 0x525220 (b, 0), 0x58bbd0 (), 0x475600, then the buttons
   0x4731d0 (x, i * 0x50 + 0x19, kinds[i] + 0xdf, 0x6ed938) with x = *(short *)(0xf8f128 + 0x6ed938 * 0x3c4)
   / 2 - 0x80, the table 0xf8f080 +0x4c = 1 / +0x1c0 = 3 / +0x1cc = 100 / +0x1d0 = 0x10 / word +0x74 = 0x54,
   the icon U_I (0x143c860 + kinds[i] * 8) via 0x473540 and 0x475e40 (id, 1). Each frame 0x58d720 (b, the
   0x4ffe90 record) and 0x58dd00 (window, 1, 0, b), the counts recounted, 0x475f70 (window, 2), the button texts
   by the counts (0 / 1 / >= 2: 0x421f / 0x4220 / "%s" 0x5f8318 with kinds[i] * 2 + 0x9a8 for the kind 0x1f,
   0x4129 / 0x412a / "%s" for the others) into the line and 0x473540 (id, line). The pump loop 0x5309a0 (this
   window, 0x677c7): the click 0x5fe710 on a button -> the message (type 0x8c at +4, 1 at +0x36, the character
   0x770b84 + b[0x27] * 0x21c at +8, b + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at
   +0x28, the kind word at +0x58, the first two selected characters (byte +0x188) at +0x5c / +0x64, the free
   slots -1) through 0x4c4b70 and 0x4c3ae0 (pump 0x20007), 0x760638 = 0x677c7, 0x52ae40 (); no character ->
   0x521150 (U_I (0x144a40c), 4) and 0x52ae40 (); the escape 0x14673a0 -> 0x6f22a0 = 1; the pump 0 ->
   0x52ae40 (), 0x525440 (), 0x4758f0 (window), 0x52d7a0 () */
void ai_GypsyHelperWindow(char *b)
{
    char line[0x100];
    char msg[0xf8];
    int btns[3];
    char *bldg;
    char *e;
    int res;
    int win;
    int counts[3];
    int kinds[3];
    int n;
    int i;
    int j;
    int k;
    int id;
    int h;
    int x;

    res = gm_Query(b + 0x5d, 2, 6, 0, 0x2a);
    bldg = (char *)(unsigned)&ai_BTypes + *(unsigned char *)b * 0x24d;
    n = 2;
    if (*(unsigned char *)(bldg + 0x247) >= 2)
        n = 3;
    if (*(unsigned char *)bldg == 0x1f) {
        kinds[0] = 0x293;
        kinds[1] = 0x294;
        kinds[2] = 0x295;
    } else {
        kinds[0] = 0x225;
        kinds[1] = 0x226;
        kinds[2] = 0x227;
    }
    if (sv_CheckCharacterFlag(b) != 0)
        return;
    counts[0] = 0;
    counts[1] = 0;
    counts[2] = 0;
    e = (char *)he_FindEvent(2, 0, 0x8c, 3, b + 1);
    while (e != 0) {
        for (i = 0; i < 3; i++) {
            if (*(unsigned short *)(e + 0xac) == kinds[i]) {
                for (j = 0; j < 4; j++)
                    if (*(int *)(e + 0x8c + j * 4) != -1)
                        counts[i]++;
            }
        }
        e = (char *)he_NextEvent();
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)"HANDEL\\HANDEL_SUCHEN");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(U_I((unsigned)&d2_LastWindow), 0x70f, 0x741);
    d2_SetForm(win, 3);
    if (*(unsigned char *)bldg == 0x1f)
        io_PrintText(0x421e);
    else
        io_PrintText(0x4128);
    sv_OpenPlayerBar(b, 0);
    ai_Unk58bbd0();
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    x = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2 - 0x80;
    for (i = 0; i < n; i++) {
        id = d2_WinOpenPic(x, i * 0x50 + 0x19, (char *)(kinds[i] + 0xdf), U_I((unsigned)&d2_LastWindow));
        *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + id * 0x2ec) = 3;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1cc + id * 0x2ec) = 100;
        *(int *)(U_I((unsigned)&d2_Objs) + 0x1d0 + id * 0x2ec) = 0x10;
        *(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x74 + id * 0x2ec) = 0x54;
        d2_SetCaption(id, (char *)U_I((unsigned)&txt_ObjectNames + kinds[i] * 8));
        btns[i] = id;
        d2_SetObjEnabled(id, 1);
    }
    d2_SetPointerSprite(0, 0);
    ai_Unk58d720(b, res);
    ai_Unk58dd00(win, 1, 0, b);
    for (;;) {
        if (sv_RunFrame(ai_GypsyHelperWindow, 0x677c7) == 0)
            break;
        ai_Unk58d720(b, res);
        ai_Unk58dd00(win, 1, 0, b);
        counts[0] = 0;
        counts[1] = 0;
        counts[2] = 0;
        e = (char *)he_FindEvent(2, 0, 0x8c, 3, b + 1);
        while (e != 0) {
            for (i = 0; i < 3; i++) {
                if (*(unsigned short *)(e + 0xac) == kinds[i]) {
                    for (j = 0; j < 4; j++)
                        if (*(int *)(e + 0x8c + j * 4) != -1)
                            counts[i]++;
                }
            }
            e = (char *)he_NextEvent();
        }
        d2_SetForm(win, 2);
        for (i = 0; i < n; i++) {
            id = btns[i];
            d2_SetObjEnabled(id, 1);
            if (*(unsigned char *)bldg == 0x1f) {
                if (counts[i] == 1)
                    txt_Format(line, (char *)0x421f, 0x22d, kinds[i] * 2 + 0x9a8);
                else if (counts[i] <= 1)
                    txt_Format(line, "%s", kinds[i] * 2 + 0x9a8);
                else
                    txt_Format(line, (char *)0x4220, counts[i], 0x22d, kinds[i] * 2 + 0x9a8);
            } else {
                if (counts[i] == 1)
                    txt_Format(line, (char *)0x4129, 0x228, kinds[i] * 2 + 0x9a8);
                else if (counts[i] <= 1)
                    txt_Format(line, "%s", kinds[i] * 2 + 0x9a8);
                else
                    txt_Format(line, (char *)0x412a, counts[i], 0x228, kinds[i] * 2 + 0x9a8);
            }
            d2_SetCaption(id, line);
        }
        if (U_I((unsigned)&d2_ClickedId) == -1) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0)
                U_I((unsigned)&sv_PopRequest) = 1;
        } else {
            for (i = 0; i < n; i++)
                if (U_I((unsigned)&d2_PointerObj) == btns[i])
                    break;
            if (i != n) {
                memset(msg, 0, 0xf8);
                *(unsigned char *)(msg + 0x04) = 0x8c;
                *(unsigned char *)(msg + 0x36) = 1;
                *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(b + 0x27) * 0x21c);
                *(int *)(msg + 0x0c) = U_I(b + 1);
                *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                *(unsigned short *)(msg + 0x58) = kinds[i];
                k = 0;
                for (j = 0; j < 0x300 && k < 4; j++) {
                    if (*(char *)((unsigned)&ai_CharInts + j * 0x21c + 0x184)) {
                        *(int *)(msg + 0x5c + k * 4) = *(int *)((unsigned)&ai_CharInts + j * 0x21c);
                        k++;
                    }
                }
                for (j = k; j < 4; j += 2)
                    *(int *)(msg + 0x5c + j * 4) = -1;
                if (k == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 4);
                    sv_ClearSelection();
                } else {
                    h = cm_SendLongMessage(msg);
                    if (h != -1) {
                        while (cm_GetSlotState(h) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    U_I((unsigned)&sv_FrameMask) = 0x677c7;
                    sv_ClearSelection();
                }
            }
        }
    }
    sv_ClearSelection();
    sv_ClosePlayerBar();
    d2_CloseView(win);
    sv_CameraFollow();
}

void d2_SetSliderFlags(); int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay); int ai_Unk5b2180();

/* 0x585380, the graveyard "Fog" window (975 B): rec = the building record (+1 the id). The text 0x40c8 (0x40b6)
   into a 0x400 buffer, then a running 0x90 event with the id rec+1 (0x53e400 (2, 0, 0x90, 3, rec + 1)) -> 4c27d0
   (-1, 0, "_NACHRICHTEN_FRIEDHOF_14" 0x62a664, 0, 100), 0x521150 (buf, 0) and out. Else a 0x50-byte query record
   (1, -1, 0xa07c43b4, 1, 6 at +0x10, 0x727 at +0x4c) through 0x5b2180 (1, &q, U_I (0x144a498), 0, 0); 0 ->
   0x52d7a0 (). Else the window "Locations_Friedhof_Fog" (0x62a64c) with 0x476f80 / 0x475600, the sound
   0x52ea30, the text 0x40b7 (0x40b6, its owner word +0x27, res), two sliders 0x46a960 (100, 0xa0 / 0xbe, 200,
   0, 2, 0x7b, 0x82, sound) with 0x475cb0 (0, 2, 0), 0x46bdd0 (0x82), 0x46bc70 (U_I (0x144a4bc) / 0x144a4cc) and
   0x46bca0 (3, U_I (0x144a4c0..c8) / 0x144a4d0..d8), the entries 0x40be / +1 in 0x475b80 (window, 2, text),
   0xf8a668 = -1; each frame 0x475f70 (window, 3), 0x515960 (0x602568), the count (slider values + 2 multiplied)
   as 0x40b9 / 0x40ba, the pump 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first
   entry: 0x520f90 (the character, the count) == 1 -> the message (type 0x90 at +4, 2 at +0x36, the character
   0x770b84 at +8, rec + 1 at +0xc, the clock 0x770ae0 .. 0x770aec at +0x28, res + 1 at +0x58, the two slider
   values at +0x5c / +0x60) through 0x4c4b70 and 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 ->
   0x4758f0 (window), 0x52d7a0 () */
void ai_Unk585380(char *rec)
{
    char buf[0x400];
    char msg[0xf8];
    char q[0x50];
    char *res;
    int win;
    int slider1;
    int slider2;
    int b1;
    int b2;
    int v;
    int n;

    txt_Format(buf, (char *)0x40c8, 0x40b6);
    if (he_FindEvent(2, 0, 0x90, 3, rec + 1) != 0) {
        snd_QueueVoice(-1, 0, (char *)(unsigned)&full_data_62a664, 0, 100);
        sv_MessageBox(buf, 0);
        return;
    }
    memset(q, 0, 0x50);
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = -1;
    *(int *)(q + 0x08) = 0xa07c43b4;
    *(int *)(q + 0x0c) = 1;
    *(int *)(q + 0x10) = 6;
    *(int *)(q + 0x4c) = 0x727;
    res = (char *)ai_Unk5b2180(1, q, U_I((unsigned)&full_data_144a498), 0, 0);
    if (res != 0) {
        win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a64c);
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        sv_FitForm(U_I((unsigned)&d2_LastWindow));
        d2_SetForm(win, 1);
        io_PrintText(0x40b7, 0x40b6, *(unsigned short *)(res + 0x27), res);
        slider1 = d2_WinOpenSlider(100, 0xa0, 0xc8, 0, 2, 0x7b, 0x82, U_I((unsigned)&d2_LastWindow));
        d2_SetObjValue(slider1, 0, 2, 0);
        d2_SetSliderFlags(slider1, 0x82);
        d2_SetLabel(slider1, U_I((unsigned)&full_data_144a4bc));
        d2_SetList(slider1, 3, U_I((unsigned)&full_data_144a4c0), U_I((unsigned)&full_data_144a4c4), U_I((unsigned)&full_data_144a4c8));
        slider2 = d2_WinOpenSlider(100, 0xbe, 0xc8, 0, 2, 0x7b, 0x82, U_I((unsigned)&d2_LastWindow));
        d2_SetObjValue(slider2, 0, 2, 0);
        d2_SetSliderFlags(slider2, 0x82);
        d2_SetLabel(slider2, U_I((unsigned)&full_data_144a4cc));
        d2_SetList(slider2, 3, U_I((unsigned)&full_data_144a4d0), U_I((unsigned)&full_data_144a4d4), U_I((unsigned)&full_data_144a4d8));
        d2_SetForm(win, 2);
        v = io_PrintText(0x40be);
        b1 = d2_GetWinObj(win, 2, v);
        b2 = d2_GetWinObj(win, 2, v + 1);
        U_I((unsigned)&d2_ClickedId) = -1;
        for (;;) {
            d2_SetForm(win, 3);
            io_PrintText((char *)(unsigned)"$C");
            n = (d2_GetObjValue(slider1) + 2) * (d2_GetObjValue(slider2) + 2);
            io_PrintText(0x40b9);
            io_PrintText(0x40ba, n);
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&d2_PointerObj) == b1) {
                    if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, n) == 1) {
                        memset(msg, 0, 0xf8);
                        *(unsigned char *)(msg + 0x04) = 0x90;
                        *(unsigned char *)(msg + 0x36) = 2;
                        *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
                        *(int *)(msg + 0x0c) = U_I(rec + 1);
                        *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                        *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                        *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                        *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                        *(int *)(msg + 0x58) = U_I(res + 1);
                        *(int *)(msg + 0x5c) = d2_GetObjValue(slider1);
                        *(int *)(msg + 0x60) = d2_GetObjValue(slider2);
                        cm_SendLongMessage(msg);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
            if (sv_RunFrame(ai_Unk585380, 0x677c7) == 0) {
                d2_CloseView(win);
                break;
            }
        }
    }
    sv_CameraFollow();
}
/* 0x585750, the graveyard "Seance" window (613 B): the first argument is unused, ev the event record, out the
   result record. out[0] / out[1] = 0, out+8 = ev+0x20, out+0xc = ev+0x24, out+0x10 = **(int **)(ev + 0x2c +
   *(int *)(ev + 0x88) * 0xc) and its 0x11 dwords copied to out+0x14; when the player's character byte +0x1cc
   has bit 8 -> the message box 0x521410 (U_I (0x144a4f0), U_I (0x144a4f4), 0) and 0. Else the window
   "Locations_Friedhof_Seance" (0x62a680) with 0x476f80 / 0x475600, the sound 0x52ea30, the text 0x40ce
   (0x40cc), the four entries 0x40cf (4) / 0x40d0 (6) / 0x40d1 (8) / 0x40d2 (cancel) in 0x475b80 (window, 2,
   text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1;
   the cancel -> 0x6f22a0 = 1; the first entry -> 0x520f90 (the character, 4) * 1, the second -> * 2, the third
   -> * 3; a non-zero result -> out[0] = the index, out[1] = 4 / 6 / 8, 0x6f22a0 = 1 and 1; the pump 0 ->
   0x4758f0 (window), the result */

int ai_Unk585750(char *unused, char *ev, int *out)
{
    int amt[4];
    int *p;
    int *q;
    int win;
    int b1;
    int b2;
    int b3;
    int b4;
    int v;
    int i;
    int r;

    r = 0;
    out[1] = 0;
    out[0] = 0;
    *(int *)((char *)out + 0x08) = *(int *)(ev + 0x20);
    *(unsigned char *)((char *)out + 0x0c) = *(unsigned char *)(ev + 0x24);
    amt[0] = 0;
    amt[1] = 4;
    amt[2] = 6;
    amt[3] = 8;
    p = *(int **)(ev + 0x2c + *(int *)(ev + 0x88) * 0xc);
    *(int *)((char *)out + 0x10) = *p;
    q = (int *)((char *)out + 0x14);
    for (i = 0; i < 0x11; i++)
        q[i] = p[i];
    if (*(unsigned char *)((unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c + 0x1cc) & 8) {
        sv_MessageBoxGreen(U_I((unsigned)&full_data_144a4f0), U_I((unsigned)&full_data_144a4f4), 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a680);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x40ce, 0x40cc);
    d2_SetForm(win, 2);
    b1 = d2_GetWinObj(win, 2, io_PrintText(0x40cf, 4));
    b2 = d2_GetWinObj(win, 2, io_PrintText(0x40d0, 6));
    b3 = d2_GetWinObj(win, 2, io_PrintText(0x40d1, 8));
    b4 = d2_GetWinObj(win, 2, io_PrintText(0x40d2));
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b4) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else {
                if (U_I((unsigned)&d2_PointerObj) == b1) {
                    v = sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 4);
                } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                    v = sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 6) * 2;
                } else if (U_I((unsigned)&d2_PointerObj) == b3) {
                    v = sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 8) * 3;
                } else {
                    goto pump;
                }
                if (v != 0) {
                    out[0] = v;
                    out[1] = amt[v];
                    r = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
        }
pump:
        if (sv_RunFrame(ai_Unk585750, 0x677c7) == 0) {
            d2_CloseView(win);
            return r;
        }
    }
}

void ai_Unk585c40(); int sb_IsInit(); void d2_PlaceViewWindows(int view, unsigned char flags);

/* 0x5859c0, the graveyard "Seance2" ritual (638 B): the first argument is unused, ev the event record (+0x48 /
   +0x4c two name ids). 0x585c40 (8, the formula ids, the buffer, 0x40d7) fills the eight formulas; the window
   "Locations_Friedhof_Seance2" (0x62a6dc) with 0x476f80 (0,0), 0x475870 (window, 1) and 0x475f70 (window, 0).
   The script "LOCATIONS_KRYPTA_SEANCE_KRYPTA_e" (0x62a6b8) is loaded (0x4287b0) and started (0x429c40) with
   the two names "%s %s" (0x60c7b8, the name +4): the player's bytes 0x770b89 / 0x770ce4 (0x4fb330, the second
   alone 0 when missing) and ev +0x4c / +0x48 (the same fallback). 0x4273c0 () == 0: the delays 0x47 and 0xf8 x8,
   each one due (the clock 0x6333cc) shows its formula text 0x515960 (the id) in section 0; the ninth entry
   closes the window (0x6f22a0 = 1); the pump 0x5309a0 (this window, 0x743c7). Else: each of the eight formulas
   becomes the news 0x4c27d0 (-7, 0, "%SEANCE_FORMEL_PART_%i_0_%i" (0x62a69c) of (id - 0x40d7) / 6 + 1 and
   (id - 0x40d7) % 6, -1, 100), then each is waited on 0x426f20 (pump 0x743c7) before and after its text
   0x515960 (the id). The window is released (0x4758f0) and the script handle returned */
int ai_Unk5859c0(char *unused, char *ev)
{
    char buf[0x400];
    char text[0x100];
    char b2[0x20];
    int ids[8];
    int a[9];
    char *name1;
    char *name2;
    int script;
    int win;
    int i;
    int h;
    unsigned int t;

    ai_Unk585c40(8, ids, b2, 0x40d7);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a6dc);
    d2_SetPointerSprite(0, 0);
    d2_PlaceViewWindows(win, 1);
    d2_SetForm(win, 0);
    script = (int)evt_Load((char *)(unsigned)&full_data_62a6b8);
    if (script != 0) {
        name1 = (char *)gm_NameRecord(*(unsigned char *)((unsigned)&ai_Unk770b89 + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
        if (name1 == 0)
            name1 = (char *)gm_NameRecord(*(unsigned char *)((unsigned)&ai_Unk770b89 + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), 0);
        name2 = (char *)gm_NameRecord(U_I(ev + 0x4c), U_I(ev + 0x48));
        if (name2 == 0)
            name2 = (char *)gm_NameRecord(U_I(ev + 0x4c), 0);
        sprintf(buf, (char *)(unsigned)"%s %s", name1 + 4, name2 + 4);
        evt_RunCommand((EVTSCRIPT *)script, buf);
    }
    if (sb_IsInit() == 0) {
        a[0] = 0x47;
        for (i = 1; i < 9; i++)
            a[i] = 0xf8;
        t = U_I((unsigned)&ai_ClockMs);
        i = 0;
        for (;;) {
            if (t + a[i] < (unsigned int)U_I((unsigned)&ai_ClockMs)) {
                t = U_I((unsigned)&ai_ClockMs);
                d2_SetForm(win, 0);
                if (i < 8) {
                    d2_SetForm(win, 0);
                    io_PrintText(ids[i]);
                    i++;
                } else {
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
            if (sv_RunFrame(ai_Unk5859c0, 0x743c7) == 0)
                break;
        }
    } else {
        for (i = 0; i < 8; i++) {
            sprintf(text, (char *)(unsigned)&full_data_62a69c, (ids[i] - 0x40d7) / 6 + 1, (ids[i] - 0x40d7) % 6);
            a[i] = snd_QueueVoice(-7, 0, text, -1, 100);
        }
        for (i = 0; i < 8; i++) {
            h = a[i];
            while (sb_IsPlaying(h) == 0)
                sv_RunFrame(0, 0x743c7);
            d2_SetForm(win, 0);
            io_PrintText(ids[i]);
            while (sb_IsPlaying(h) != 0)
                sv_RunFrame(0, 0x743c7);
        }
    }
    d2_CloseView(win);
    return script;
}
/* 0x585c40, fills the eight seance formulas (101 B): n / a / b the two arrays of ints, base = 0x40d7; each pair
   i, i+1 gets the random 0x42ec00 (3) & 0xffff r: a[i] = base + r, a[i+1] = base + r + 3, b[i] = b[i+1] = r;
   base += 6 per pair */

void ai_Unk585c40(int n, int *a, int *b, int base)
{
    int i;
    int r;

    for (i = 0; i < n; i += 2) {
        r = util_Random(3) & 0xffff;
        b[i] = r;
        b[i + 1] = r;
        a[i] = base + r;
        a[i + 1] = base + r + 3;
        base += 6;
    }
}

int snd_VoiceVariant(); char *gv_FindAccused(char *c, int max, int *ids, int *cnt); float unk_Unk4f2660(); float util_RandomF();
void cm_SendProofExecuted(int a, int b); 

/* 0x585cb0, the graveyard seance outcome (633 B): ch = the character, ev = the event record (+0x14 / +0x4c the
   voice pair). 0x4c24e0 (ev + 0x14, ev + 0x4c) the voice variant, 0x5859c0 (ch, ev) the ritual (the script handle
   kept), 0x4f41b0 (ch, 4, ids, &cnt) the ghost candidates; mode = (its result == 0), overridden to 2 when
   0x42ec20 () + 0x4f2660 (ch) - 0.1f < 1.0f. The window "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 /
   0x475600, the sound 0x52ea30, 0xf8a668 = -1; 0x4c24e0 again (the voice handle). Mode 0 (a ghost found): the
   records 0x4c4ed0 (the id, the character + 4), 0x56fbc0 (ch, 1), the text 0x40ef (0x40cc, the ghost's id word)
   and the news 0x4c27d0 (the voice, 0, "GEIST_KOMMT" 0x62a70c, 0x42ec00 (8) & 0xffff, 0xfa). Mode 1:
   0x42ec00 (0x200) & 0xffff + 0x200 times 0x20 as the amount, 0x4c47e0 (the character + 4, -1, 0x6f1bc5, the
   amount), the text 0x40f0 (0x40cc, the amount) and the same news. Mode 2: the text 0x40d3 (0x40cc) and the
   news "GEIST_KOMMT_NICHT" (0x62a6f8). The pump loop 0x5309a0 (this window, 0x777c7): the escape 0x14673a0 ->
   0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the script freed (0x429e90) */
void ai_Unk585cb0(char *ch, char *ev)
{
    int ids[4];
    int cnt;
    char *p;
    float f;
    int win;
    int voice;
    int mode;
    int script;
    int i;
    int n;

    snd_VoiceVariant(U_I(ev + 0x14), U_I(ev + 0x4c));
    script = ai_Unk5859c0(ch, ev);
    p = (char *)gv_FindAccused((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 4, ids, &cnt);
    mode = p == 0;
    f = unk_Unk4f2660(ch);
    if (util_RandomF() + f - (0.100000001f) < *(float *)(unsigned)&ai_Unk5eb780)
        mode = 2;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_ClickedId) = -1;
    d2_SetForm(win, 1);
    voice = snd_VoiceVariant(U_I(ev + 0x14), U_I(ev + 0x4c));
    if (mode == 0) {
        for (i = 0; i < cnt; i++)
            cm_SendProofExecuted(ids[i], *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
        he_Unk56fbc0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 1);
        io_PrintText(0x40ef, 0x40cc, *(unsigned short *)p);
        i = util_Random(8) & 0xffff;
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, i, 0xfa);
    } else if (mode == 1) {
        n = ((util_Random(0x200) & 0xffff) + 0x200) << 5;
        cm_SendMoveGoods(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, n);
        io_PrintText(0x40f0, 0x40cc, n);
        i = util_Random(8) & 0xffff;
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, i, 0xfa);
    } else {
        io_PrintText(0x40d3, 0x40cc);
        i = util_Random(8) & 0xffff;
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a6f8, i, 0xfa);
    }
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk585cb0, 0x777c7) == 0)
            break;
    }
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}
/* 0x585f30, the graveyard seance with a sacrifice (717 B): ch / ev as 0x585cb0. 0x4c24e0 (ev + 0x14, ev + 0x4c)
   the voice, 0x5859c0 (ch, ev) the ritual (the script handle); the chance 0x42ec20 () + 0x4f2660 (ch) - 0.4f <
   1.0f -> no ghost (mode 2). Else the sacrifice: 0x50a640 (the player) * 0.25f clamped to 160000 .. 800000
   (0x27100 / 0xc3500); 0x4c47e0 (the character + 4, -1, 0x6f1bc5, the amount); the message (type 0x8e at +4,
   the character 0x770b84 at +8, ch + 1 at +0xc, the clock 0x770ae0 .. 0x770aec at +0x28, 1 at +0x36, the
   amount at +0x58) through 0x4c4b70; 0x56fbc0 (the character, 3). The window "Locations_Friedhof_Friedhof"
   (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, 0xf8a668 = -1; the news 0x4c27d0 (the voice, 0,
   "GEIST_KOMMT" 0x62a70c / "GEIST_KOMMT_NICHT" 0x62a6f8, 0x42ec00 (8) & 0xffff, 0xfa) and the text 0x40f1
   (0x40cc, the amount) / 0x40d3 (0x40cc); the pump loop 0x5309a0 (this window, 0x777c7): the escape -> 0x6f22a0
   = 1; the pump 0 -> 0x4758f0 (window), the script freed (0x429e90) */

void ai_Unk585f30(char *ch, char *ev)
{
    char msg[0xf8];
    int voice;
    int script;
    int win;
    int amount;
    int mode;
    int i;

    voice = snd_VoiceVariant(U_I(ev + 0x14), U_I(ev + 0x4c));
    script = ai_Unk5859c0(ch, ev);
    amount = 0;
    mode = 0;
    if (util_RandomF() + unk_Unk4f2660(ch) - (0.400000006f) < *(float *)(unsigned)&ai_Unk5eb780) {
        mode = 2;
    } else {
        amount = (int)(gm_GetWorth(*(unsigned short *)(unsigned)&ai_PlayerId) * 0.25f);
        if (amount > 0x27100) {
            if (amount >= 0xc3500)
                amount = 0xc3500;
        } else {
            amount = 0x27100;
        }
        cm_SendMoveGoods(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, amount);
        memset(msg, 0, 0xf8);
        *(unsigned char *)(msg + 0x04) = 0x8e;
        *(unsigned char *)(msg + 0x36) = 1;
        *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
        *(int *)(msg + 0x0c) = U_I(ch + 1);
        *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
        *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
        *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
        *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
        *(int *)(msg + 0x58) = amount;
        cm_SendLongMessage(msg);
        he_Unk56fbc0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 3);
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_ClickedId) = -1;
    d2_SetForm(win, 1);
    i = util_Random(8) & 0xffff;
    if (mode == 0) {
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, i, 0xfa);
        io_PrintText(0x40f1, 0x40cc, amount);
    } else {
        snd_QueueVoice(voice, 0, (char *)(unsigned)&full_data_62a6f8, i, 0xfa);
        io_PrintText(0x40d3, 0x40cc);
    }
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk585f30, 0x777c7) == 0)
            break;
    }
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}

void ai_FindWantedItems(struct G5CH *p, int n, struct G9REC *rec, int *st);

/* 0x586200, the graveyard seance with a ghost lottery (860 B): ch / ev as 0x585cb0. 0x4c24e0 (ev + 0x14, ev +
   0x4c) the voice, 0x5859c0 (ch, ev) the ritual (the script handle). The fifteen wanted items 0x2b8, 0x2bb,
   0x2bc, 0x2bf, 0x2c2, 0x2c3, 0x2bd, 0x2c1, 0x2c4, 0x2c5, 0x2c6, 0x2ba, 0x2c0, 0x2be, 0x2b9 are looked up
   in the character's inventory (0x4a5760 (ch, 0xf, rec, st)); (st[0] == 6 and st[2] == 0) or st[1] == 0xf ->
   mode 1; else a random free record (0x42ec00 (0xf)) gives mode 0 and its item. The chance 0x42ec20 () +
   0x4f2660 (ch) - 0.2f < 1.0f -> mode 2 (no ghost). The window "Locations_Friedhof_Friedhof" (0x62a5bc) with
   0x476f80 / 0x475600, the sound 0x52ea30, 0xf8a668 = -1; mode 0: 0x4c4880 (the character + 4, -1, the item,
   1, 0x6f1bc5, 0), 0x56fbc0 (the character, 2), the text 0x40f2 (0x40cc, the item * 2 + 0x9a8) and the news
   0x4c2760 (the voice, 0, "GEIST_KOMMT" 0x62a70c, 0x42ec00 (8) & 0xffff); mode 1: 0x42ec00 (0x400) & 0xffff +
   1000 times 0x20, 0x4c47e0 (the character + 4, -1, 0x6f1bc5, the amount), the text 0x40f3 and the same news;
   mode 2: the text 0x40d3 (0x40cc) and "GEIST_KOMMT_NICHT" (0x62a6f8). The pump loop 0x5309a0 (this window,
   0x777c7): the escape -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), the script freed (0x429e90) */
void ai_Unk586200(char *ch, char *ev)
{
    int rec[15][4];
    int st[3];
    int win;
    int voice;
    int script;
    int item;
    int mode;
    int i;
    int n;

    voice = snd_VoiceVariant(U_I(ev + 0x14), U_I(ev + 0x4c));
    script = ai_Unk5859c0(ch, ev);
    memset(rec, 0, sizeof rec);
    rec[0][0] = 0x2b8;
    rec[1][0] = 0x2bb;
    rec[2][0] = 0x2bc;
    rec[3][0] = 0x2bf;
    rec[4][0] = 0x2c2;
    rec[5][0] = 0x2c3;
    rec[6][0] = 0x2bd;
    rec[7][0] = 0x2c1;
    rec[8][0] = 0x2c4;
    rec[9][0] = 0x2c5;
    rec[10][0] = 0x2c6;
    rec[11][0] = 0x2ba;
    rec[12][0] = 0x2c0;
    rec[13][0] = 0x2be;
    rec[14][0] = 0x2b9;
    ai_FindWantedItems((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0xf, (int *)rec, st);
    item = 0;
    if (st[0] == 6 && st[2] == 0) {
        mode = 1;
    } else if (st[1] == 0xf) {
        mode = 1;
    } else {
        i = util_Random(0xf) & 0xffff;
        while (rec[i][1] != 0)
            i = (i + 1) % 0xf;
        mode = 0;
        item = rec[i][0];
    }
    if (util_RandomF() + unk_Unk4f2660(ch) - 0.2f < *(float *)(unsigned)&ai_Unk5eb780)
        mode = 2;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    U_I((unsigned)&d2_ClickedId) = -1;
    d2_SetForm(win, 1);
    if (mode == 0) {
        cm_SendExSellObject(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -1, item, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        he_Unk56fbc0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 2);
        io_PrintText(0x40f2, 0x40cc, item * 2 + 0x9a8);
        snd_PlayVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, util_Random(8) & 0xffff);
    } else if (mode == 1) {
        n = ((util_Random(0x400) & 0xffff) + 0x3e8) << 5;
        cm_SendMoveGoods(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, n);
        io_PrintText(0x40f3, 0x40cc, n);
        snd_PlayVoice(voice, 0, (char *)(unsigned)&full_data_62a70c, util_Random(8) & 0xffff);
    } else {
        io_PrintText(0x40d3, 0x40cc);
        snd_PlayVoice(voice, 0, (char *)(unsigned)&full_data_62a6f8, util_Random(8) & 0xffff);
    }
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (sv_RunFrame(ai_Unk586200, 0x777c7) == 0)
            break;
    }
    d2_CloseView(win);
    if (script != 0)
        evt_Stop((EVTSCRIPT *)script);
}
/* 0x586560, the graveyard seance selection window (171 B): a / w the window record (its +0 the window, +0x88 the
   selected entry, +0x28 the eight entries of 3 dwords), out the result; 0xf8a668 = -1; the pump loop 0x5309a0
   (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; a clicked entry i -> w[0x22] = i, 0x475590 (the
   window, 0), 0x585750 (a, w, out) and 0x475590 (the window, 1); a non-zero result -> 1 and 0x6f22a0 = 1; the
   pump 0 -> the result */


int ai_Unk586560(char *a, int *w, int *out)
{
    int r;
    int i;

    r = 0;
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) == 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                for (i = 0; i < 8; i++) {
                    if (U_I((unsigned)&d2_PointerObj) == w[10 + i * 3]) {
                        w[0x22] = i;
                        d2_SetViewEnabled(w[0], 0);
                        r = ai_Unk585750(a, (char *)w, out);
                        d2_SetViewEnabled(w[0], 1);
                        break;
                    }
                }
                if (r != 0) {
                    r = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        if (sv_RunFrame(ai_Unk586560, 0x677c7) == 0)
            return r;
    }
}

void gm_SendMoneyDelta(int a, int b);

/* 0x586610, the graveyard seance (419 B): rec = the record (+0x65 the state record). 0x4ef7c0 (the character,
   rec, 0, 0x4ef740, 0xff) the grave entry -> none: 0x521150 (U_I (0x144a514), 0) and out. The window record w:
   its +0x1c the entry, +0x20 its id, +0x24 the 0x4ef6b0 flag, the text 0x4107 (the name 0x5eecd0[id]);
   0x583b70 (w, U_I (0x144a4f0)) opens the detail window -> 0: 0x52d7a0 (). 0x586560 (rec, w, out) the selection;
   0x4758f0 (w[0]). When a choice: 0x4c4a60 (the character + 4, 0x1cc, 4, 0x800000, 0); out[1] -> 0x50cc50 (the
   character, -out[1]); out[0] 1 -> 0x585cb0 (rec, out), 2 -> 0x586200 (rec, out), 3 -> 0x585f30 (rec, out);
   0x52d7a0 () */
void ai_Unk586610(char *rec)
{
    int w[0x23];
    int out[2];
    char line[0x100];
    int *p;

    p = (int *)gv_FindEntry((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, rec, 0, (char *)(unsigned)&gv_EntrySameId, 0xff);
    if (p == 0) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_144a514), 0);
        return;
    }
    w[7] = (int)p;
    w[8] = *p;
    *(unsigned char *)((char *)w + 0x24) = gv_EntryIndex(U_I(rec + 0x65), p);
    txt_Format(line, (char *)0x4107, U_I(((unsigned)&sv_PhaseMinutes + 0x218) + *p * 4));
    if (ai_Unk583b70(w, (char *)U_I((unsigned)&full_data_144a4f0)) == 0) {
        sv_CameraFollow();
        return;
    }
    p = (int *)ai_Unk586560(rec, w, out);
    d2_CloseView(w[0]);
    if (p != 0) {
        cm_SendSetField(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), 0x1cc, 4, ((unsigned)&ai_Relations + 0x1aca0), 0);
        if (out[1] != 0)
            gm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -out[1]);
        if (out[0] == 1)
            ai_Unk585cb0(rec, (char *)out);
        else if (out[0] == 2) {
            ai_Unk586200(rec, (char *)out);
            sv_CameraFollow();
            return;
        } else if (out[0] == 3) {
            ai_Unk585f30(rec, (char *)out);
            sv_CameraFollow();
            return;
        }
    }
    sv_CameraFollow();
}

void ai_EndCharExtra(char *ch, int f); int cm_SendRelations(int a, int b, int c, int d); int sv_Unk52b490();

/* 0x5867c0, the graveyard "Schlag" action (114 B): when the grave object 0x6f22f8 exists with the flag +0x1b4
   bit 0 and U_I (0x146738c) is set: 0x587c40 (the object, 0), 0x4c4b00 (the character + 4, the object's +4, -5,
   0), then 0x52b490 (the object, "locations/friedhof/schlag_FRIEDHOF.esc" 0x62a718) +0xc8c = 1 */
void ai_Unk5867c0(char *unused)
{
    char *p;

    p = *(char **)(unsigned)&ai_PickedChar;
    if (p != 0 && (*(unsigned char *)(p + 0x1b4) & 1) && U_I(((unsigned)&dm_L + 0xc)) != 0) {
        ai_EndCharExtra(p, 0);
        p = *(char **)(unsigned)&ai_PickedChar;
        cm_SendRelations(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(int *)(p + 4), -5, 0);
        p = (char *)sv_Unk52b490((int)*(char **)(unsigned)&ai_PickedChar, (char *)(unsigned)&full_data_62a718);
        if (p != 0)
            *(int *)(p + 0xc8c) = 1;
    }
}

void sv_ReleaseNamePlates(void); void ai_Unk587c90(); int sv_RegisterNamePlate(); void ai_TradeContactChecks(void);
void ai_Unk586e40(); void ai_Unk587370(); void ai_Unk586ab0(); void ai_Unk587300();
void ai_Unk5871e0(); void ai_Unk587340(); void ai_Unk5871c0();

/* 0x586840, the graveyard contact dispatcher (611 B): the contact list 0x52e190 (), the pump loop 0x5309a0 (this
   dispatcher, 0x67fff): each frame 0x587c90 (rec); with the filter 0x6f231c bit 0x200 the contacts
   "contact_ALTERN_LASSEN" (0x62a7e0, 0x2f), "contact_HEIMSUCHUNG" (0x62a7cc, 0x29), "contact_ZUTATEN_SUCHEN"
   (0x62a7b4, 0x13), "contact_GRAEBER_PFLEGEN" (0x62a79c, 0x2a), "contact_GEHILFEN_ERSCHAFFEN" (0x62a780,
   0x28), "contact_VERSICHERUNG" (0x62a768, 0x2b), "contact_LIEGEPLAN" (0x62a754, 0x13) and "contact_TRANSPORT"
   (0x62a450, 0x15) via 0x52e1c0 (name, the text U_I (0x144a440 / 410 / 65c / 634 / 680 / 590 / 5d0 / 1441fa0),
   the kind); always "contact_GRAB_MIETEN" (0x62a740, U_I (0x144a5d4), 0x16). No clicked contact 0x6f22e4: the
   gate 0x6f2448 and the key 0x1468410 -> 0x1e 0x586e40, 0x2f 0x587370, 0x22 0x585380, 0x23 0x586ab0, 0x19
   0x587300, 0x16 0x5871e0, 0x11 0x587340. Else: with the bit 0x200, 0x5867c0 (rec) first, then the matching
   contact: ALTERN -> 0x586e40, HEIMSUCHUNG -> 0x586ab0, ZUTATEN -> 0x584d70, GRAEBER -> 0x587300, GEHILFEN ->
   0x587340, VERSICHERUNG -> 0x587370, LIEGEPLAN -> 0x5871c0, TRANSPORT -> 0x5af1a0; with the byte 0x6f231c
   negative and GRAB_MIETEN clicked -> 0x5871e0 (rec); 0x5912c0 () each frame */
void ai_Unk586840(char *rec)
{
    int c_altern, c_heim, c_zut, c_graeber, c_gehilfen, c_vers, c_liege, c_trans, c_mieten;

    c_altern = 0;
    c_heim = 0;
    c_zut = 0;
    c_graeber = 0;
    c_gehilfen = 0;
    c_vers = 0;
    c_liege = 0;
    c_trans = 0;
    c_mieten = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk586840, 0x67fff) != 0) {
        ai_Unk587c90(rec);
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            c_altern = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a7e0, U_I((unsigned)&full_data_144a440), 0x2f);
            c_heim = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a7cc, U_I((unsigned)&full_data_144a410), 0x29);
            c_zut = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a7b4, U_I((unsigned)&full_data_144a65c), 0x13);
            c_graeber = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a79c, U_I((unsigned)&full_data_144a634), 0x2a);
            c_gehilfen = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a780, U_I((unsigned)&full_data_144a680), 0x28);
            c_vers = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a768, U_I((unsigned)&full_data_144a590), 0x2b);
            c_liege = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a754, U_I((unsigned)&full_data_144a5d0), 0x13);
            c_trans = sv_RegisterNamePlate((char *)(unsigned)"contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
        }
        c_mieten = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a740, U_I((unsigned)&full_data_144a5d4), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) == 0) {
            if (U_I((unsigned)&gm_AllowAll) != 0) {
                switch (U_I((unsigned)&kb_Last)) {
                case 0x1e: ai_Unk586e40(rec); break;
                case 0x2f: ai_Unk587370(rec); break;
                case 0x22: ai_Unk585380(rec); break;
                case 0x23: ai_Unk586ab0(rec); break;
                case 0x19: ai_Unk587300(rec); break;
                case 0x16: ai_Unk5871e0(rec); break;
                case 0x11: ai_Unk587340(rec); break;
                }
            }
        } else {
            if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
                ai_Unk5867c0(rec);
                if (U_I((unsigned)&ai_ClickedObject) == c_altern)
                    ai_Unk586e40(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_heim)
                    ai_Unk586ab0(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_zut)
                    ai_GypsyHelperWindow(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_graeber)
                    ai_Unk587300(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_gehilfen)
                    ai_Unk587340(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_vers)
                    ai_Unk587370(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_liege)
                    ai_Unk5871c0(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c_trans)
                    ai_OpenTransport(rec);
            }
            if ((char)U_I((unsigned)&ai_BuildingPerms) < 0 && U_I((unsigned)&ai_ClickedObject) == c_mieten)
                ai_Unk5871e0(rec);
        }
        ai_TradeContactChecks();
    }
}

int ai_AmountMenu(); int sv_CheckMoney();

/* 0x586ab0, the graveyard "HEIMSUCHUNG" contact (902 B): rec = the contact record (+0x27 the owner word, +1 a
   dword). The target picked by 0x5c1130 (the 0x50-byte query: 1, 0x400, 6 at +0x10, -1 at +0x1c .. +0x38,
   U_I (0x144a414)); none -> 0x52d7a0 (). 0x525220 (rec, 0), the window "Locations_Friedhof_Friedhof"
   (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the text 0x4098 / 0x4097 (the target's word twice)
   in 0x475b80 (window, 1, text) = the slider, 0x475cb0 (slider, 4, 0xc, 4), the texts 0x4099 / 0x4099 + 1 in
   0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape
   0x14673a0 -> 0x6f22a0 = 1; the first entry: n = 0x475980 (slider), 0x520f90 (the player's character, n) == 0
   -> skip, the first selected character (the byte +0x188 of 0x770b80 + i * 0x21c, i = 0 .. 0x2ff) or none ->
   0x521150 (U_I (0x144a40c), 0); else the message (type 0x88 at +4, 2 at +0x36, n at +0x64, the player's
   character word at +8, rec + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at +0x28, the
   target +4 at +0x5c, the found character +4 at +0x38 / +0x60, the level at +0x58: the target's +0x170 / +0x16c
   record + 1 or 0x4a63f0 (the player's character, 0, 0x7b4c4a0, 0, 0, 100.0f, &lv) with lv = -1) through
   0x4c4b70, its answer waited on 0x4c3ae0 (pump 0x20007), 0x760638 = 0x677c7, 0x6f22a0 = 1, 0x521150
   (U_I (0x144a428), 0); the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x525440 (),
   0x52ae40 () */
void ai_Unk586ab0(char *rec)
{
    char msg[0xf8];
    char q[0x50];
    char *p;
    char *f;
    int win;
    int slider;
    int b1;
    int b2;
    int n;
    int v;
    int i;
    int h;
    int lv;

    memset(q, 0, 0x50);
    for (i = 0; i < 8; i++)
        *(int *)(q + 0x1c + i * 4) = -1;
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = 0x400;
    *(int *)(q + 0x10) = 6;
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a414), 0, 0);
    if (p == 0) {
        sv_CameraFollow();
        return;
    }
    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    n = *(unsigned short *)p;
    slider = d2_GetWinObj(win, 1, io_PrintText(0x4098, 0x4097, n, n));
    d2_SetForm(win, 2);
    n = io_PrintText(0x4099);
    b1 = d2_GetWinObj(win, 2, n);
    b2 = d2_GetWinObj(win, 2, n + 1);
    d2_SetObjValue(slider, 4, 0xc, 4);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                v = 0;
                n = d2_GetObjValue(slider);
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, n) == 0)
                    v = 1;
                f = 0;
                for (i = 0; i < 0x300; i++) {
                    if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                        f = (char *)((unsigned)&ai_Chars + i * 0x21c);
                        break;
                    }
                }
                if (f == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
                } else if (v == 0) {
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x88;
                    *(unsigned char *)(msg + 0x36) = 2;
                    *(int *)(msg + 0x64) = n;
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(int *)(msg + 0x5c) = *(int *)(p + 4);
                    *(int *)(msg + 0x60) = *(int *)(f + 4);
                    *(int *)(msg + 0x38) = *(int *)(f + 4);
                    lv = -1;
                    if (*(int *)(p + 0x170) == 0) {
                        if (*(int *)(p + 0x16c) == 0)
                            AI_4A63F0_7((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0, (char *)0x7b4c4a0, 0, 0, 100.0f, &lv);
                        else
                            lv = *(int *)(*(int *)(p + 0x16c) + 1);
                    } else {
                        lv = *(int *)(*(int *)(p + 0x170) + 1);
                    }
                    *(int *)(msg + 0x58) = lv;
                    h = cm_SendLongMessage(msg);
                    if (h != -1) {
                        while (cm_GetSlotState(h) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    U_I((unsigned)&sv_FrameMask) = 0x677c7;
                    U_I((unsigned)&sv_PopRequest) = 1;
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a428), 0);
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk586ab0, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_ClearSelection();
            return;
        }
    }
}
/* 0x586e40, the graveyard "ALTERN_LASSEN" contact (842 B): the sibling of 0x586ab0; rec = the contact record
   (+0x27 the owner word, +1 a dword). The target picked by 0x5c1130 (the 0x50-byte query: 1, 0x400, 6 at
   +0x10, -1 at +0x1c .. +0x38, U_I (0x144a444)); none -> 0x52d7a0 (). 0x525220 (rec, 0), the window
   "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the sound 0x52ea30, the text 0x40a4 /
   0x40a3 (the target's word, 4) and 0x40a5 / 0x40a5 + 1 in 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump
   loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: 0x520f90 (the
   player's character, 4) == 0 -> skip, the first selected character (the byte +0x188 of 0x770b80 + i * 0x21c,
   i = 0 .. 0x2ff) or none -> 0x521150 (U_I (0x144a40c), 0); else the message (type 0x89 at +4, 1 at +0x36, 4
   at +0x64, the player's character word at +8, rec + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 /
   word 0x770aec at +0x28, the target +4 at +0x5c, the found character +4 at +0x38 / +0x60, the level at +0x58:
   the target's +0x170 / +0x16c record + 1 or 0x4a63f0 (the player's character, 0, 0x7b4c4a0, 0, 0, 100.0f,
   &lv) with lv = -1) through 0x4c4b70, its answer waited on 0x4c3ae0 (pump 0x20007), 0x760638 = 0x677c7, the
   line 0x40a7 (the found character's word) in 0x5176d0 and 0x521150 (the line, 0), 0x6f22a0 = 1; the second
   entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x525440 (), 0x52ae40 () */

void ai_Unk586e40(char *rec)
{
    char msg[0xf8];
    char buf[0x200];
    char q[0x50];
    char *p;
    char *f;
    int win;
    int b1;
    int b2;
    int v;
    int i;
    int h;
    int lv;

    memset(q, 0, 0x50);
    for (i = 0; i < 8; i++)
        *(int *)(q + 0x1c + i * 4) = -1;
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = 0x400;
    *(int *)(q + 0x10) = 6;
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a444), 0, 0);
    if (p == 0) {
        sv_CameraFollow();
        return;
    }
    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    i = io_PrintText(0x40a4, 0x40a3, *(unsigned short *)p, 4);
    d2_SetForm(win, 2);
    i = io_PrintText(0x40a5);
    b1 = d2_GetWinObj(win, 2, i);
    b2 = d2_GetWinObj(win, 2, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                v = 0;
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 4) == 0)
                    v = 1;
                f = 0;
                for (i = 0; i < 0x300; i++) {
                    if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                        f = (char *)((unsigned)&ai_Chars + i * 0x21c);
                        break;
                    }
                }
                if (v == 0) {
                    if (f == 0) {
                        sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
                    } else {
                        memset(msg, 0, 0xf8);
                        *(unsigned char *)(msg + 0x04) = 0x89;
                        *(unsigned char *)(msg + 0x36) = 1;
                        *(int *)(msg + 0x64) = 4;
                        *(int *)(msg + 0x0c) = U_I(rec + 1);
                        *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                        *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                        *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                        *(int *)(msg + 0x60) = *(int *)(f + 4);
                        *(int *)(msg + 0x38) = *(int *)(f + 4);
                        *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                        *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                        *(int *)(msg + 0x5c) = *(int *)(p + 4);
                        lv = -1;
                        if (*(int *)(p + 0x170) == 0) {
                            if (*(int *)(p + 0x16c) == 0)
                                AI_4A63F0_7((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0, (char *)0x7b4c4a0, 0, 0, 100.0f, &lv);
                            else
                                lv = *(int *)(*(int *)(p + 0x16c) + 1);
                        } else {
                            lv = *(int *)(*(int *)(p + 0x170) + 1);
                        }
                        *(int *)(msg + 0x58) = lv;
                        h = cm_SendLongMessage(msg);
                        if (h != -1) {
                            while (cm_GetSlotState(h) == 0)
                                sv_RunFrame(0, 0x20007);
                        }
                        U_I((unsigned)&sv_FrameMask) = 0x677c7;
                        txt_Format(buf, (char *)0x40a7, *(unsigned short *)f);
                        sv_MessageBox(buf, 0);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk586e40, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_ClearSelection();
            return;
        }
    }
}
/* 0x5871c0, the graveyard "GRAB_MIETEN" contact (30 B): the contact window 0x583240 (0, rec,
   U_I (0x144a5d0), 0, 0), then 0x52d7a0 () */

void ai_Unk5871c0(char *rec)
{
    ai_Unk583240(0, rec, (char *)U_I((unsigned)&full_data_144a5d0), 0, 0);
    sv_CameraFollow();
}

int gv_FindReservedEntry();

/* 0x5871e0, the graveyard rent-a-grave handler (278 B): rec = the contact record (+0x65 the state record). The
   entry of the player's character (0x770b80 + 0x6f25b8 * 0x21c, rec, 0, 0x4ef740, 0xff) through 0x4ef7c0;
   without an entry: 0x4f25d0 (the character, &bld, &p) == 0 -> 0x5847a0 (rec) and 0x52d7a0 (), else 0x521150
   (U_I (0x144ada0), 0). With the entry p: the window record w (+0x1c p, +0x20 *p, +0x24 the 0x4ef6b0 flag), the
   line 0x4107 (the name 0x5eecd0[*p]); 0x583b70 (w, the line) -> 0: return; 0x584410 (rec, w), 0x4758f0 (w[0]),
   0x52d7a0 () */
void ai_Unk5871e0(char *rec)
{
    int w[0x23];
    char line[0x100];
    char *bld;
    int *p;

    p = (int *)gv_FindEntry((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, rec, 0, (char *)(unsigned)&gv_EntrySameId, 0xff);
    if (p == 0) {
        if (gv_FindReservedEntry((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, &bld, &p) == 0) {
            ai_Unk5847a0(rec);
            sv_CameraFollow();
            return;
        }
        sv_MessageBox((char *)U_I(((unsigned)&ai_Unk144ad78 + 0x28)), 0);
        return;
    }
    w[7] = (int)p;
    w[8] = *p;
    *(unsigned char *)((char *)w + 0x24) = gv_EntryIndex(U_I(rec + 0x65), p);
    txt_Format(line, (char *)0x4107, U_I(((unsigned)&sv_PhaseMinutes + 0x218) + *p * 4));
    if (ai_Unk583b70(w, line) == 0)
        return;
    ai_Unk584410(rec, w);
    d2_CloseView(w[0]);
    sv_CameraFollow();
}
/* 0x587300, the graveyard "GRAEBER_PFLEGEN" contact (50 B): 0x520fe0 (rec) == 0 -> the contact window 0x583240
   (0, rec, U_I (0x144a638), 1, 0x584900) and 0x52d7a0 () */

void ai_Unk587300(char *rec)
{
    if (sv_CheckCharacterFlag(rec) != 0)
        return;
    ai_Unk583240(0, rec, (char *)U_I((unsigned)&full_data_144a638), 1, ai_Unk584900);
    sv_CameraFollow();
}
/* 0x587340, the graveyard "GEHILFEN_ERSCHAFFEN" contact (47 B): 0x520fe0 (rec) == 0 -> the contact window
   0x583240 (0, rec, U_I (0x144a684), 0, 0x584b70) and 0x52d7a0 () */

void ai_Unk587340(char *rec)
{
    if (sv_CheckCharacterFlag(rec) == 0)
        ai_Unk583240(0, rec, (char *)U_I((unsigned)&full_data_144a684), 0, ai_Unk584b70);
    sv_CameraFollow();
}
/* 0x587370, the graveyard "VERSICHERUNG" contact (485 B): rec = the contact record (+0x27 the owner word, +1 a
   dword). 0x525220 (rec, 0), the window "Locations_Friedhof_Friedhof" (0x62a5bc) with 0x476f80 / 0x475600, the
   sound 0x52ea30, the texts 0x40f6 / 0x40f5 (0x228) and 0x40f7 / 0x40f7 + 1 in 0x475b80 (window, 2, text),
   0xf8a668 = -1; the pump loop 0x5309a0 (this window, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first
   entry: the first selected character (the byte +0x188 of 0x770b80 + i * 0x21c, i = 0 .. 0x2ff) or none ->
   0x521150 (U_I (0x144a40c), 0); else the message (type 0x8a at +4, 1 at +0x36, the player's character word at
   +8, rec + 1 at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at +0x28, the found character +4
   at +0x38 / +0x58) through 0x4c4b70 and 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 ->
   0x4758f0 (window), 0x525440 (), 0x52ae40 () */

void ai_Unk587370(char *rec)
{
    char msg[0xf8];
    char *f;
    int win;
    int b1;
    int b2;
    int i;

    sv_OpenPlayerBar(rec, 0);
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a5bc);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x40f6, 0x40f5, 0x228);
    d2_SetForm(win, 2);
    i = io_PrintText(0x40f7);
    b1 = d2_GetWinObj(win, 2, i);
    b2 = d2_GetWinObj(win, 2, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                f = 0;
                for (i = 0; i < 0x300; i++) {
                    if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                        f = (char *)((unsigned)&ai_Chars + i * 0x21c);
                        break;
                    }
                }
                if (f == 0) {
                    sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
                } else {
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x8a;
                    *(unsigned char *)(msg + 0x36) = 1;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x38) = *(int *)(f + 4);
                    *(int *)(msg + 0x58) = *(int *)(f + 4);
                    cm_SendLongMessage(msg);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk587370, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_ClosePlayerBar();
            sv_ClearSelection();
            return;
        }
    }
}

int ai_ProductionWindow(); int ai_OpenTransport(); void ai_MasterLetterWindow(); int ai_Unk5ad150(); int ai_StorageWindow();

/* 0x587560, the graveyard trade-contact dispatcher (317 B): rec = the record (+0x5d the owner record). The
   contact list 0x52e190 (); the pump loop 0x5309a0 (this dispatcher, 0x67fff): each frame 0x587c90 (rec); with
   the filter 0x6f231c bit 0x200 the contacts 0x62a818 (U_I (0x144a714), 0x13), "contact_TRANSPORT" 0x62a450
   (U_I (0x1441fa0), 0x15), 0x62a808 (U_I (0x143cbc8), 0xc), 0x62a7f8 (U_I (0x143c9f0), 0xc) and 0x62a464
   (U_I (0x1441f90), 0xe) via 0x52e1c0 (name, text, kind), then 0x5867c0 (rec); the clicked contact 0x6f22e4:
   the first -> 0x58bd60 (rec), the second -> 0x5af1a0 (rec), the third -> 0x5c3200 (rec), the fourth ->
   0x5ad150 (rec, "contact_TRANSPORT") and the fifth -> 0x5b26b0 (rec, *(int *)(rec + 0x5d)) */
void ai_Unk587560(char *rec)
{
    int c0, c1, c2, c3, c4;

    c0 = 0;
    c1 = 0;
    c2 = 0;
    c3 = 0;
    c4 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk587560, 0x67fff) != 0) {
        ai_Unk587c90(rec);
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            c0 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a818, U_I((unsigned)&full_data_144a714), 0x13);
            c1 = sv_RegisterNamePlate((char *)(unsigned)"contact_TRANSPORT", U_I((unsigned)&ai_ContactTextTransport), 0x15);
            c2 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a808, U_I((unsigned)&ai_ContactTextMasterLetter), 0xc);
            c3 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a7f8, U_I((unsigned)&ai_ContactTextPersonnelBook), 0xc);
            c4 = sv_RegisterNamePlate((char *)(unsigned)"contact_LAGER", U_I((unsigned)&ai_ContactTextStorage), 0xe);
            ai_Unk5867c0(rec);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == c0)
                ai_ProductionWindow(rec);
            else if (U_I((unsigned)&ai_ClickedObject) == c1)
                ai_OpenTransport(rec);
            else if (U_I((unsigned)&ai_ClickedObject) == c2)
                ai_MasterLetterWindow(rec);
            else if (U_I((unsigned)&ai_ClickedObject) == c3)
                ai_Unk5ad150(rec, (char *)(unsigned)&full_data_62a7f8);
            else if (U_I((unsigned)&ai_ClickedObject) == c4)
                ai_StorageWindow(rec, *(__int32 *)(rec + 0x5d));
        }
    }
}
/* 0x5876a0, the graveyard seance contact dispatcher (95 B): rec = the record. The pump loop 0x5309a0 (this
   dispatcher, 0x67fff): the contact 0x62a83c (U_I (0x144a764), 0x16) via 0x52e1c0 (name, text, kind); the
   clicked contact 0x6f22e4 (not 0) on it -> 0x586610 (rec) */

void ai_Unk5876a0(char *rec)
{
    int c;

    while (sv_RunFrame(ai_Unk5876a0, 0x67fff) != 0) {
        c = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a83c, U_I((unsigned)&full_data_144a764), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) != 0 && U_I((unsigned)&ai_ClickedObject) == c)
            ai_Unk586610(rec);
    }
}
/* 0x587700, the graveyard grave-detail message (269 B): rec / ch the character / sel the picked line (1 / 2 / 4).
   The line 0x413a (the grave's word) through 0x5176d0 and 0x521150 (the line, 1) == 0 -> 0; 0x4ffe90
   (rec + 0x5d, 1, 0, 0x211); the message (type 0x8f at +4, 1 at +0x36, the player's character word at +8, rec + 1
   at +0xc, the clock 0x770ae0 / 0x770ae4 / 0x770ae8 / word 0x770aec at +0x28, the 0x4ffe90 answer's +2 (or -1)
   at +0x60, the character +4 at +0x58, sel at +0x5c) through 0x4c4b70; 1 */

int ai_Unk587700(char *rec, char *ch, int sel)
{
    char msg[0xf8];
    char buf[0x400];
    int r;

    txt_Format(buf, (char *)0x413a, *(unsigned short *)rec);
    if (sv_MessageBox(buf, 1) == 0)
        return 0;
    r = gm_Query(rec + 0x5d, 1, 0, 0x211);
    memset(msg, 0, 0xf8);
    *(unsigned char *)(msg + 0x04) = 0x8f;
    *(unsigned char *)(msg + 0x36) = 1;
    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    *(int *)(msg + 0x0c) = U_I(rec + 1);
    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
    if (r != 0)
        *(int *)(msg + 0x60) = *(int *)(r + 2);
    else
        *(int *)(msg + 0x60) = -1;
    *(int *)(msg + 0x58) = *(int *)(ch + 4);
    *(int *)(msg + 0x5c) = sel;
    cm_SendLongMessage(msg);
    return 1;
}

float gm_GetSkill(char *c, int skill); int gm_ClockSeason(); char * he_FindEvent(int n, ...); void d2_SetObjEnabled();

/* 0x587810, the graveyard grave-detail window (692 B): rec = the record (+1 a dword). n = 0x4fe350 (0x770ae0);
   0x520fe0 (rec) != 0 -> return. U_I (0x6f2448) == 0 and n != 2 -> the line 0x413b, 0x5176d0 / 0x521150 (the
   line, 4), 0x52d7a0 (); else when a running 0x8f event exists (0x53e400 (2, 0, 0x8f, 3, rec + 1)) -> the line
   0x413c, 0x5176d0 / 0x521150 (the line, 4), 0x52d7a0 (). Otherwise the window 0x62a84c with 0x476f80 / 0x475600,
   the sound 0x52ea30, the sections 0 / 1 / 3, the text 0x4138, 0x4139 (0x228) and 0x4139 + 1 / + 2 in 0x475b80
   (window, 3, text) = the three buttons, 0x525220 (rec, 0); the pump loop 0x5309a0 (this window, 0x677c7): the
   escape 0x14673a0 -> 0x6f22a0 = 1; the first selected character (the byte +0x188, i = 0 .. 0x2ff): the three
   buttons by 0x504ec0 (the character, 1 / 2 / 4) < 1.0 in 0x475e40 (the button, the flag); a click on one ->
   sel 1 / 2 / 4: without a character 0x521150 (U_I (0x144a40c), 0), else 0x587700 (rec, the character, sel) != 0
   -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x525440 (), 0x52ae40 (), 0x52d7a0 () */
void ai_Unk587810(char *rec)
{
    char buf[0x400];
    char *f;
    int win;
    int b1;
    int b2;
    int b3;
    int n;
    int i;
    int sel;

    n = gm_ClockSeason((char *)(unsigned)&ai_Time);
    if (sv_CheckCharacterFlag(rec) != 0)
        return;
    if (U_I((unsigned)&gm_AllowAll) != 0 || n == 2) {
        if (he_FindEvent(2, 0, 0x8f, 3, rec + 1) != 0) {
            txt_Format(buf, (char *)0x413c);
            sv_MessageBox(buf, 4);
            sv_CameraFollow();
            return;
        }
    } else {
        txt_Format(buf, (char *)0x413b);
        sv_MessageBox(buf, 4);
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a84c);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x4138);
    d2_SetForm(win, 3);
    n = io_PrintText(0x4139, 0x228);
    b1 = d2_GetWinObj(win, 3, n);
    b2 = d2_GetWinObj(win, 3, n + 1);
    b3 = d2_GetWinObj(win, 3, n + 2);
    sv_OpenPlayerBar(rec, 0);
    while (sv_RunFrame(ai_Unk587810, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        f = 0;
        for (i = 0; i < 0x300; i++) {
            if (*(char *)((unsigned)&ai_Chars + i * 0x21c + 0x188)) {
                f = (char *)((unsigned)&ai_Chars + i * 0x21c);
                break;
            }
        }
        if (f != 0) {
            d2_SetObjEnabled(b1, gm_GetSkill(f, 1) < 1.0);
            d2_SetObjEnabled(b2, gm_GetSkill(f, 2) < 1.0);
            d2_SetObjEnabled(b3, gm_GetSkill(f, 4) < 1.0);
        }
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1)
                sel = 1;
            else if (U_I((unsigned)&d2_PointerObj) == b2)
                sel = 2;
            else if (U_I((unsigned)&d2_PointerObj) == b3)
                sel = 4;
            else
                continue;
            if (f == 0) {
                sv_MessageBox((char *)U_I((unsigned)&full_data_144a40c), 0);
            } else if (ai_Unk587700(rec, f, sel) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
    }
    d2_CloseView(win);
    sv_ClosePlayerBar();
    sv_ClearSelection();
    sv_CameraFollow();
}
/* 0x587ad0, the graveyard fog / vermin contact dispatcher (152 B): rec = the record. The contact list 0x52e190
   (); the pump loop 0x5309a0 (this dispatcher, 0x67fff): with the filter 0x6f231c bit 0x200 the contacts
   0x62a88c (U_I (0x144a69c), 0x28) and 0x62a870 (U_I (0x144a490), 0x13) via 0x52e1c0 (name, text, kind); the
   clicked contact 0x6f22e4 (not 0): the first -> 0x587810 (rec), the second -> 0x585380 (rec) */

void ai_Unk587ad0(char *rec)
{
    int c0, c1;

    c0 = 0;
    c1 = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk587ad0, 0x67fff) != 0) {
        if (U_I((unsigned)&ai_BuildingPerms) & 0x200) {
            c0 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a88c, U_I((unsigned)&full_data_144a69c), 0x28);
            c1 = sv_RegisterNamePlate((char *)(unsigned)&full_data_62a870, U_I((unsigned)&full_data_144a490), 0x13);
        }
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == c0)
                ai_Unk587810(rec);
            else if (U_I((unsigned)&ai_ClickedObject) == c1)
                ai_Unk585380(rec);
        }
    }
}

int gm_BuildingJob(); 

/* 0x587b70, the building flag mask of a character (201 B), called from f3gm.c 0x50359b: ch = the character
   (+0 the word), b = the building record (+0 the kind, +0x25 / +0x27 the owner words), x = a record (+0 a word)
   or 0. bldg = 0x7e3f30 + the kind * 0x24d; v = 0x5044b0 (the character's word, 0x5048e0 (the building kind));
   the mask 0x81, 0x10bd when the building's +0x25 is the character's word; x with the word 0x211 / 0x205 ->
   bit 0 cleared. When the building's +0x27 is the character's word: 0x4461 set and, when one of the six bytes at
   bldg + 0x223 is v (a 0 byte ends the scan), 0x2200; x with the word 0x21b -> 0x5000; U_I (0x6f2448) != 0 ->
   -1 */
int ai_Unk587b70(char *ch, char *b, char *x)
{
    char *bldg;
    int kind;
    int v;
    int f;
    int i;

    kind = *(unsigned char *)b;
    bldg = (char *)U_I((unsigned)&ai_BTypes) + kind * 0x24d;
    v = gm_TitleJobI(*(unsigned short *)ch, gm_BuildingJob(*(unsigned char *)bldg));
    f = 0x81;
    if (*(unsigned short *)(b + 0x25) == *(unsigned short *)ch)
        f = 0x10bd;
    if (x != 0 && (*(unsigned short *)x == 0x211 || *(unsigned short *)x == 0x205))
        f &= ~1;
    if (*(unsigned short *)(b + 0x27) == *(unsigned short *)ch) {
        f |= 0x4461;
        for (i = 0; i < 6; i++) {
            if (*(unsigned char *)(bldg + 0x223 + i) == 0)
                break;
            if (*(unsigned char *)(bldg + 0x223 + i) == v) {
                f |= 0x2200;
                break;
            }
        }
    }
    if (x != 0 && *(unsigned short *)x == 0x21b)
        f |= 0x5000;
    if (U_I((unsigned)&gm_AllowAll) != 0)
        return -1;
    return f;
}
/* 0x587c40, ends the character ch's extra -- the "sp_<id>" role pose script that sv.c 0x52b490 / 0x52b500
   links back at the model +0x28 (the model at +0x184) / +0xc94 -- when one runs (0x4284a0 evt_FindById,
   0x429e90 evt_Stop), then toggles the at-work flag bit 0 of +0x1b4 by f's bit 0 (f = 0: ends the pose only,
   the form the AI (ai_EndWorkerPose) and the graveyard "Schlag" action (0x5867c0, then 0x52b490 with the new
   pose) use) */


void ai_EndCharExtra(char *ch, int f)
{
    int h;

    if (*(int *)(*(int *)(ch + 0x184) + 0x28) != -1) {
        h = (int)evt_FindById(*(int *)(*(int *)(ch + 0x184) + 0x28));
        if (h != 0)
            evt_Stop((EVTSCRIPT *)h);
    }
    *(int *)(ch + 0x1b4) ^= f & 1;
}
/* 0x587c90, the graveyard idle-chatter tick (329 B, the argument is not used): only every 0x1194 units of the
   clock 0x6333cc (0x62a538 the last run); with U_I (0x6f2448) == 0 and 0x42ec20 () < 0.9f no chatter. The scene's
   0x20 object slots 0x7603a0 .. 0x760420: the objects of the kind 0x14 (byte +2) whose building [obj + 0x184]
   is 0x5fb5b0 (+0x88) are collected; none -> return; one (x) and, with more than one, a second different one (y)
   are picked at random: a = (x + 4) % 3, or 3 + ((x + 4) & 1) when x + 9 == 1; b = -1, or the same for y; the
   script "locations\friedhof\gespraeche.esc" (0x62a8a8) -> 0x4287b0 and 0x429ab0 (the script, 3,
   x's building + 0x184, a, b) */

void ai_Unk587c90(char *unused)
{
    int list[0x20];
    int cnt;
    int n;
    int a;
    int b;
    char *x;
    char *y;
    char *p;
    int i;
    int s;

    if (U_I((unsigned)&ai_ClockMs) < U_I((unsigned)&full_data_62a538) + 0x1194)
        return;
    U_I((unsigned)&full_data_62a538) = U_I((unsigned)&ai_ClockMs);
    if (U_I((unsigned)&gm_AllowAll) == 0 && util_RandomF() < 0.9f)
        return;
    cnt = 0;
    for (i = 0; i < 0x20; i++) {
        p = *(char **)((unsigned)&ai_SceneChars + i * 4);
        if (p != 0 && *(unsigned char *)(p + 2) == 0x14 && *(int *)(p + 0x184) != 0 &&
            *(int *)(*(int *)(p + 0x184) + 0x88) == U_I((unsigned)&ai_Unk5fb5b0))
            list[cnt++] = (int)p;
    }
    if (cnt == 0)
        return;
    n = util_Random(cnt) & 0xffff;
    x = (char *)list[n];
    y = 0;
    if (cnt > 1) {
        n = util_Random(cnt) & 0xffff;
        y = (char *)list[n];
        if (y == x) {
            n++;
            y = (char *)list[n % cnt];
        }
    }
    a = 0;
    if (x != 0) {
        if (*(unsigned char *)(x + 9) == 1)
            a = (*(int *)(x + 4) & 1) + 3;
        else
            a = *(int *)(x + 4) % 3;
    }
    b = -1;
    if (y != 0) {
        if (*(unsigned char *)(y + 9) == 1)
            b = (*(int *)(y + 4) & 1) + 3;
        else
            b = *(int *)(y + 4) % 3;
    }
    s = (int)evt_Load((char *)(unsigned)&full_data_62a8a8);
    if (s != 0)
        evt_RunScriptArgs((EVTSCRIPT *)s, 3, *(int *)(x + 0x184), a, b);
}
/* 0x587de0, clears the 0x4000-byte table 0x6f5f60 (17 B), called from main.c 0x532160 (the per-turn reset) */

void ai_ClearTurnTable(void)
{
    memset((char *)(unsigned)&full_data_6f5f60, 0, 0x4000);
}

int ai_Unk588310(); int ai_Unk588950(); int ai_Unk588160(); int ai_OpenHelpTooltip();
int ai_Unk588c60(); void d2_RaiseViewWindows(int view);

/* 0x587e00, the item drag-and-drop handler (863 B, ROUGH: the classification and every call are written, the
   branch details around them come from the disassembly): src = 0xf8aac8 (the dragged object), win = 0x62a8cc
   (the drag window). The release: (src == -1 and 0x62a8d0 != -1) or (0x6f266c != 0, 0x14673a8 == 0, win != -1)
   -> 0x4758f0 (win), 0x6f266c = 0, the three 0x62a8cc / 0x62a8d4 / 0x62a8d0 = -1, 0x5f3240 = 1 and
   0x4113f0 (word 0xf8a674, word 0xf8a676). The target 0x6f22e8 (not 0, not 0x6f266c, and 0x14673a8 != 0):
   0x6f22f0's word, 0 -> the "other" record 0x6f22e8 kept; 0x6f266c = 0x6f22e8 and 0x62a8d0 = -1. The widget
   under the pointer (0xf8f080 + U_I (0x5fe710) * 0x2ec + 0x2e8) classifies src: a character (0x770b80 ..
   0x7d5f80, the word 0xffff = none), the goods table (0x889f74 + 0xa900: the byte 0 < 0x58), the buildings
   (0x7e3f30 + 0xca78 = 0x58 records of 0x24d), the 0x7e3f34 table (+0x14f218: the word < 0x2dc), the item table
   0x91aaf0 (+0xb9dc: / 0x41 < 0x2dc) or, without a user record, src itself in the ranges 0xdf .. 0x427 and
   0x427 .. 0x47f. With nothing classified: the tail. Else 0x4758f0 (win) when win != -1 and the drag window
   reset; the item 0x588310 (the kinds 0x17 / 0x20 / 0x25 / 0x27) or 0x588950, the goods 0x588160, the other
   record 0x588bb0, the character 0x588c60 -> win = the new drag window 0x62a8cc = the result; 0x62a8d0 = src,
   0x62a8d4 = 0xfb1024 + win * 0x2ac, 0x5f3240 = 0 when 0x14673a8 != 0. win != -1: 0x6ed9d4 = 1, 0x6f2670 = 1,
   0x4741a0 (win); else the drag reset 0x62a8d0 = -1, 0x6f266c = 0 and 0x6ed9d4 / 0x6f2670 cleared */
void ai_HandleItemDrag(void)
{
    char *src;
    char *win;
    char *p;
    char *sel;
    char *ch;
    int item;

    src = *(char **)(unsigned)&d2_Unkf8aac8;
    win = *(char **)(unsigned)&ai_DragWindow;
    item = 0;
    sel = 0;
    ch = 0;
    if ((src == (char *)-1 && U_I((unsigned)&full_data_62a8d0) != -1) ||
        (U_I((unsigned)&full_data_6f266c) != 0 && U_I(((unsigned)&dm_L + 0x28)) == 0 && win != (char *)-1)) {
        d2_CloseView(win);
        U_I((unsigned)&full_data_6f266c) = 0;
        U_I((unsigned)&ai_DragWindow) = -1;
        U_I((unsigned)&full_data_62a8d4) = -1;
        U_I((unsigned)&full_data_62a8d0) = -1;
        ch = 0;
        U_I((unsigned)&di_Absolute) = 1;
        di_SetMousePos(*(short *)(unsigned)&d2_MouseX, *(short *)(unsigned)&d2_MouseY);
        src = *(char **)(unsigned)&d2_Unkf8aac8;
        win = *(char **)(unsigned)&ai_DragWindow;
    }
    if (U_I((unsigned)&ai_PickedObject) != 0 && U_I((unsigned)&full_data_6f266c) != U_I((unsigned)&ai_PickedObject) && U_I(((unsigned)&dm_L + 0x28)) != 0) {
        if (sv_HoverItem == 0 || *(unsigned short *)sv_HoverItem == 0)
            sel = (char *)U_I((unsigned)&ai_PickedObject);
        U_I((unsigned)&full_data_6f266c) = U_I((unsigned)&ai_PickedObject);
        U_I((unsigned)&full_data_62a8d0) = -1;
    }
    if (src != (char *)-1 && (char *)U_I((unsigned)&full_data_62a8d0) != src) {
        p = *(char **)(U_I((unsigned)&d2_Objs) + U_I((unsigned)&d2_PointerObj) * 0x2ec + 0x2e8);
        if (p == 0) {
            if (src >= (char *)0xdf && src < (char *)0x427)
                item = (int)src - 0xdf;
            else if (src >= (char *)0x427 && src < (char *)0x47f)
                item = (int)src - 0x427;
        } else if (p >= (char *)(unsigned)&ai_Chars && p < (char *)(unsigned)ai_Chars[768]) {  /* ai_Chars[768]: one past the table (0x7d5f80) */
            ch = (*(unsigned short *)p != 0xffff) ? p : 0;
        } else if (p >= (char *)U_I((unsigned)&ai_Blds) && p < (char *)U_I((unsigned)&ai_Blds) + 0xa900) {
            item = *(unsigned char *)p;
            if (item >= 0x58)
                item = 0;
        } else if (p >= (char *)U_I((unsigned)&ai_BTypes) && p < (char *)U_I((unsigned)&ai_BTypes) + 0xca78) {
            item = (int)(p - (char *)U_I((unsigned)&ai_BTypes)) / 0x24d;
        } else if (p >= (char *)U_I((unsigned)&ai_Objects) && p < (char *)U_I((unsigned)&ai_Objects) + 0x14f218) {
            item = *(unsigned short *)p;
            if (item >= 0x2dc)
                item = 0;
        } else if (p >= (char *)U_I((unsigned)&ai_Goods) && p < (char *)U_I((unsigned)&ai_Goods) + 0xb9dc) {
            item = (int)(p - (char *)U_I((unsigned)&ai_Goods)) / 0x41;
        }
        U_I((unsigned)&full_data_6f266c) = 0;
    }
    if (item == 0 && sel == 0 && ch == 0)
        goto tail;
    if (win != (char *)-1) {
        d2_CloseView(win);
        src = *(char **)(unsigned)&d2_Unkf8aac8;
        win = 0;
        U_I((unsigned)&ai_DragWindow) = -1;
        U_I((unsigned)&full_data_62a8d4) = -1;
    }
    if (item != 0) {
        if (*(unsigned char *)(U_I((unsigned)&ai_Goods) + item * 0x41) == 0x20 ||
            *(unsigned char *)(U_I((unsigned)&ai_Goods) + item * 0x41) == 0x17 ||
            *(unsigned char *)(U_I((unsigned)&ai_Goods) + item * 0x41) == 0x25 ||
            *(unsigned char *)(U_I((unsigned)&ai_Goods) + item * 0x41) == 0x27)
            win = (char *)ai_Unk588310(item);
        else
            win = (char *)ai_Unk588950(item);
    } else if (sel != 0) {
        win = (char *)ai_OpenHelpTooltip(sel);
    } else if (ch != 0) {
        win = (char *)ai_Unk588c60(ch);
    }
    src = *(char **)(unsigned)&d2_Unkf8aac8;
    U_I((unsigned)&ai_DragWindow) = (int)win;
    if (win != (char *)-1) {
        U_I((unsigned)&full_data_62a8d0) = (int)src;
        U_I((unsigned)&full_data_62a8d4) = *(int *)(((unsigned)&d2_Views + 0x4) + (int)win * 0x2ac);
        if (U_I(((unsigned)&dm_L + 0x28)) != 0)
            U_I((unsigned)&di_Absolute) = 0;
        U_I((unsigned)&d2_CutsceneActive) = 1;
        U_I((unsigned)&full_data_6f2670) = 1;
        d2_RaiseViewWindows(win);
        return;
    }
    U_I((unsigned)&full_data_62a8d0) = -1;
    U_I((unsigned)&full_data_6f266c) = 0;
tail:
    if (U_I((unsigned)&full_data_6f2670) != 0) {
        U_I((unsigned)&d2_CutsceneActive) = 0;
        U_I((unsigned)&full_data_6f2670) = 0;
    }
}

int gm_BuildingKindCost(int unused, int kind); char *_strupr(char *); int sprintf(char *buf, const char *fmt, ...);

/* 0x588160, the drag window of a goods item (419 B, ROUGH: the calls and the text ids are written, the 0x1c-byte
   record at the frame start is assembled field by field from the globals 0x62a8f4 .. 0x62a908 (the words), the
   bytes 0x62a902 / 0x62a906 / 0x62a8fe and 0x631090, restored by the disassembly): idx = the building index
   (0x7e3f30 + idx * 0x24d). The window 0x62a8d8 with 0x475600, the section 1; the building kind (byte 0) not
   0xa: the text 0x27 (idx * 0xe + 0x48e, the record word at +0x10 + [bldg + 0x247] * 4), else 0x515960
   (idx * 0xe + 0x48e); the section 2, 0x4731d0 (0, 0, idx + 0x427, U_I (0x6ed938)) and the text
   idx * 0xe + 0x48f; not the kind 0xa: the section 3, the texts 0x2a (the record at +0x18 + [bldg + 0x247] *
   4), 0x28 (0x509dc0 (the player's character, idx)) and 0x29 (bldg + 0x243); the window */
int ai_Unk588160(int idx)
{
    char d[0x1c];
    char *bldg;
    int win;

    *(unsigned char *)(d + 0x00) = *(unsigned char *)(unsigned)&ch_EmptyName;
    *(unsigned short *)(d + 0x01) = 0;
    *(unsigned char *)(d + 0x03) = 0;
    *(unsigned short *)(d + 0x04) = *(unsigned short *)(unsigned)&full_data_62a908;
    *(unsigned short *)(d + 0x06) = 0;
    *(unsigned short *)(d + 0x08) = *(unsigned short *)(unsigned)&full_data_62a904;
    *(unsigned char *)(d + 0x0a) = *(unsigned char *)(unsigned)&full_data_62a906;
    *(unsigned char *)(d + 0x0b) = 0;
    *(int *)(d + 0x0c) = U_I((unsigned)&full_data_62a8f8);
    *(unsigned short *)(d + 0x10) = *(unsigned short *)(unsigned)&full_data_62a900;
    *(unsigned char *)(d + 0x12) = *(unsigned char *)(unsigned)&full_data_62a902;
    *(unsigned char *)(d + 0x13) = 0;
    *(unsigned short *)(d + 0x14) = *(unsigned short *)(unsigned)&full_data_62a8f4;
    *(unsigned short *)(d + 0x16) = 0;
    *(unsigned short *)(d + 0x18) = *(unsigned short *)(unsigned)&full_data_62a8fc;
    *(unsigned char *)(d + 0x1a) = *(unsigned char *)(unsigned)&full_data_62a8fe;
    *(unsigned char *)(d + 0x1b) = 0;
    bldg = (char *)U_I((unsigned)&ai_BTypes) + idx * 0x24d;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a8d8);
    d2_CentreViewWindows(win);
    d2_SetForm(0, 1);
    if (*(unsigned char *)bldg != 0xa)
        io_PrintText(0x27, idx * 0xe + 0x48e, d + 0x10 + *(unsigned char *)(bldg + 0x247) * 4);
    else
        io_PrintText(idx * 0xe + 0x48e);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, (char *)(idx + 0x427), U_I((unsigned)&d2_LastWindow));
    io_PrintText(idx * 0xe + 0x48f);
    if (*(unsigned char *)bldg != 0xa) {
        d2_SetForm(0, 3);
        io_PrintText(0x2a, d + 0x18 + *(unsigned char *)(bldg + 0x247) * 4);
        io_PrintText(0x28, gm_BuildingKindCost((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, idx));
        io_PrintText(0x29, *(int *)(bldg + 0x243));
    }
    return win;
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

/* 0x588310, the drag window of an item (1596 B, ROUGH SKELETON: the call order, the text ids and the case values
   are complete; the widget layout arithmetic and the case split around the texts are summarised as comments):
   idx = the item index, item = 0x91aaf0 + idx * 0x41, o = 0x4cd9d0 (idx). The window "ToolTip\ToolTip_Objekte_
   Waffen" (0x62a940) when o and *o, else "ToolTip\ToolTip_Objekte_Handelsgut" (0x62a91c), 0x475600; the position
   from the window record 0xfb1024 + win * 0x2ac (0x26c / +6) and the screen width 0xfb9072 in 0x46cda0; the
   sections 2 / 1 / 3, 0x4731d0 (0, 0, idx + 0xdf) and the texts idx * 2 + 0x9a9 / idx * 2 + 0x9a8, 0x21 (the byte
   o + 0x40) or 0x1f (0x5089c0 (idx, 0x6f1bc5, (int)0x5087f0 (idx, 0x64)) as float -> int); the section 4 and the
   text 0x20 (o + 0x22) or, with 0x6f2308 / 0x6f230c / 0x75ec60 (the first non-0, 0 -> the text 0x20), the
   characters 0x770b80 + i * 0x21c (+0x16c == that, 0x300 of them) collected in 0x50cdc0 (that, the count, the
   list) -> float; the text 0x22 ((o + 0x22 + the character's byte +0x81 * 1 / 1008) * the float -> int); the
   text 0x25 with the section 4 / 3; the lists 0x61a348 (15 entries of 0x15, the word 0x61a349 == the record's
   word and != 0x1cd) 0x473350 (6, idx * 0xa + 0x28, the byte + 0x427, -2, U_I (0x6ed938)) and the text
   "%s$N" (0x62a90c, the text (the byte * 7 + 0x48e)); the case values 0x225 / 0x226 / 0x227 / 0x293 / 0x294 /
   0x295 -> the texts 0x48 / 0x49 / 0x4a or 0x4e / 0x4f / 0x50, 0x1c1 .. 0x1c3 -> 0x18 / 0x19, 0x1c4 .. 0x1c7 /
   0x2af / 0x2b0 -> 0x1e, 0x1bd .. 0x1c0 / 0x1d0 .. 0x1d2 -> 0x20, 0x1b7 .. 0x1b9 / 0x1ca .. 0x1cc -> 0x1e,
   0x1ba / 0x1bb / 0x1bc / 0x1cd .. 0x1cf -> 0x1f, each 0x473350 (6, the slot * 0xa + 0x28, the value + 0x427,
   -2) and "%s$N"; the 0x1c1 .. 0x1c7 / 0x2af / 0x2b0 kinds with the item kinds 0x17 / 0x25: the extra slot 0x444
   / 0x624; nothing at all -> the text 0x602568; the window */
int ai_Unk588310(int idx)
{
    char *item;
    char *o;
    int win;
    int n;
    int i;
    int j;

    item = (char *)U_I((unsigned)&ai_Goods) + idx * 0x41;
    o = (char *)ai_Unk4cd9d0(idx);
    win = d2_OpenForm(0, 0, (char *)((o != 0 && *(int *)o != 0) ? (unsigned)&full_data_62a940 : (unsigned)&full_data_62a91c));
    d2_CentreViewWindows(win);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, (char *)(idx + 0xdf), U_I((unsigned)&d2_LastWindow));
    io_PrintText(idx * 2 + 0x9a9);
    d2_SetForm(0, 1);
    io_PrintText(idx * 2 + 0x9a8);
    d2_SetForm(0, 3);
    if (*(unsigned char *)(o + 0x40) != 0) {
        io_PrintText(0x21, *(unsigned char *)(o + 0x40));
    } else {
        io_PrintText(0x1f, (int)((float (*)(int, int, int))gm_GoodsPrice)(idx, *(unsigned char *)(unsigned)&ai_OwnPlayer, (int)gm_ItemPrice(idx, 0x64)));
    }
    n = U_I((unsigned)&ai_PlayerBuilding);
    if (n == 0)
        n = U_I((unsigned)&ai_FallbackBuilding);
    if (n == 0)
        n = U_I((unsigned)&sv_SelectedObject);
    d2_SetForm(0, 4);
    if (n == 0) {
        io_PrintText(0x20, *(int *)(o + 0x22));
    } else {
        /* ROUGH: the characters of n (the +0x16c field) collected and 0x50cdc0 (n, the count, the list) */
        gm_BuildingSkill(n, 1, 1);
        gm_FindCharacterByMaster(n);
        gm_Unk50cdc0(n, 0, 0);
        gm_FindCharacterByMaster(n);
        io_PrintText(0x22, (int)(*(float *)&U_I(((unsigned)&ai_Unk5ee160 + 0x7e8)) * (float)*(unsigned char *)(o + 0x81) * 0.5 +
                                 (float)(int)*(float *)&U_I(((unsigned)&ai_Unk5ee160 + 0x7e8))));
        io_PrintText(0x20, *(int *)(o + 0x22));
    }
    /* ROUGH: the 0x61a348 / 0x61a34a lists (the word == the record's word, != 0x1cd) with 0x473350 (6, ...) and
       the text "%s$N" (0x62a90c); then the kind cases 0x225 .. 0x295 / 0x1b7 .. 0x2b0 with 0x473350 (6, ...) and
       "%s$N"; the extra slot 0x444 / 0x624 for the kinds 0x1c1 .. 0x1c7 / 0x2af / 0x2b0 with the item kinds
       0x17 / 0x25 */
    i = 0;
    j = 0;
    (void)item;
    (void)i;
    (void)j;
    if (win == 0)
        io_PrintText((char *)(unsigned)"$C");
    return win;
}
/* 0x588950, the drag window of a goods item (598 B, ROUGH: a start Ghidra missed - 0x588970 is the body, the
   guard falls through): idx = the item index, item = 0x91aaf0 + idx * 0x41; the kind 0x1d -> -1. The window
   "ToolTip\ToolTip_Objekte_Upgrade" (0x62a984) with 0x475600, the position from the window record and the screen
   width 0xfb9072 in 0x46cda0; the sections 2 / 1 / 3, 0x4731d0 (0, 0, idx + 0xdf), the texts idx * 2 + 0x9a9 /
   idx * 2 + 0x9a8, 0x21 (the byte +0x40) or 0x23 (0x5087f0 (idx, 0x64) * (0x6f23fc * 0.25 + 0.5) -> int); the
   text 0x20 (the dword +0x22). With 0x6f2308 / 0x6f230c / 0x75ec60 (the first non-0) and the item kind not 2 / 6:
   the building record of that object (0x7e3f30 + kind * 0x24d), its 0x40 words at +0x23 (& 0x7fff) against the
   item's word, the bytes +0x1a3 / +0x1e3, the text 0x24 and 0xf9e / 0xfe0 / the word + 0xf98 in 0x515960
   (0x62a978 / 0x62a96c / 0x62a960); the window */

int ai_Unk588950(int idx)
{
    char *item;
    char *o;
    int win;
    int i;
    int v;

    item = (char *)U_I((unsigned)&ai_Goods) + idx * 0x41;
    if (*(unsigned char *)item == 0x1d)
        return -1;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a984);
    d2_CentreViewWindows(win);
    d2_SetForm(0, 2);
    d2_WinOpenPic(0, 0, (char *)(idx + 0xdf), U_I((unsigned)&d2_LastWindow));
    io_PrintText(idx * 2 + 0x9a9);
    d2_SetForm(0, 1);
    io_PrintText(idx * 2 + 0x9a8);
    d2_SetForm(0, 3);
    if (*(unsigned char *)(item + 0x40) != 0)
        io_PrintText(0x21, *(unsigned char *)(item + 0x40));
    else
        io_PrintText(0x23, (int)(gm_ItemPrice(idx, 0x64) * (float)(U_I((unsigned)&ai_Difficulty) * 0.25 + 0.5)));
    io_PrintText(0x20, *(int *)(item + 0x22));
    v = U_I((unsigned)&ai_PlayerBuilding);
    if (v == 0)
        v = U_I((unsigned)&ai_FallbackBuilding);
    if (v == 0)
        v = U_I((unsigned)&sv_SelectedObject);
    if (v == 0)
        return win;
    if (*(unsigned char *)item == 2 || *(unsigned char *)item == 6)
        return win;
    o = (char *)U_I((unsigned)&ai_BTypes) + *(unsigned char *)v * 0x24d;
    for (i = 0; i < 0x40; i++)
        if ((*(unsigned short *)(o + 0x23 + i * 2) & 0x7fff) == *(unsigned short *)item)
            break;
    if (i == 0x40)
        return win;
    io_PrintText(0x24);
    v = *(unsigned char *)(o + 0x1a3 + i);
    i = *(unsigned char *)(o + 0x1e3 + i);
    if (v == 0 || v == -1 || v == 0xff)
        return win;
    if (v == 3)
        io_PrintText((char *)(unsigned)&full_data_62a978, i, 0xf9e);
    else if (v == 0x24)
        io_PrintText((char *)(unsigned)&full_data_62a96c, i, 0xfe0);
    else
        io_PrintText((char *)(unsigned)&full_data_62a960, i, v * 2 + 0xf98);
    return win;
}

float gm_CharAge(); int ai_AmountRowStatus(); int ai_Unk5be720(); short * gm_FindChar(int id); int gm_TitlesLeft();

/* 0x588c60, the drag window of a character (1111 B, ROUGH SKELETON: the window, every call and the text ids are
   on the disassembly; the branch details around them are summarised): ch = the character record (+0 the word,
   +2 the kind, +0xd a byte, +0x164 / +0x166 / +0x169 bytes). The window 0x62a9f8 with 0x475600, the section 1,
   the text 0x62a9ec (the word), the section 4, the record {ch} in 0x5be1d0 and, when it answers, 0x5be720 (0xa6,
   5, U_I (0x6ed938), the line); the section 2, the kind != 0x14 -> 0x505e30 (the word) -> int in 0x515960
   (0x2b). The flags: the word 0x3000 (ebx), the bytes +0x164 / +0x166 / +0x169, the texts 0x2d / 0x2c (the mask
   0x5f + 0x130 + the byte), 0x2f / 0x30 / 0x31 / 0x32 / 0x33 / 0x34 / 0x35 (0x62a9e4) / 0x41, the kind lists
   0x53e400 / 0x53e300, 0x505df0 (ch) and 0x505df0 (ch + 0x68), 0x62a9d8, 0x62a9e4; the line 0x62a56c
   (0x5176d0), 0x473650, 0x52eb50 (0x64, 0x4df) and 0x576040; the sections via 0x475f70; the window */
int ai_Unk588c60(char *ch)
{
    char line[0x100];
    char *w;
    int win;
    int v;

    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a9f8);
    w = (char *)win;
    d2_CentreViewWindows(win);
    d2_SetForm(0, 1);
    io_PrintText((unsigned)&full_data_62a9ec, *(unsigned short *)ch);
    d2_SetForm(0, 4);
    *(char **)line = ch;
    if (ai_AmountRowStatus(line) != 0)
        ai_Unk5be720(0xa6, 5, U_I((unsigned)&d2_LastWindow), line);
    d2_SetForm(0, 2);
    if (*(unsigned char *)(ch + 2) != 0x14)
        io_PrintText(0x2b, (int)*(float *)&U_I(((unsigned)&ai_Unk5ee160 + 0x7e8)));
    /* ROUGH: the flags ebx = 0x3000, the bytes +0x164 / +0x166 / +0x169 and the texts 0x2d (the mask 0x5f + 0x130
       + the byte +0x164), 0x2c (both branches), 0x2f / 0x30 / 0x31 (0x505df0 (ch) on the byte), 0x32 / 0x41
       (0x53e400 / 0x53e300), 0x33 / 0x34 / 0x35 (0x62a9e4) / 0x62a9d8 (0x505df0 (ch + 0x68)) */
    io_PrintText(0x2d);
    io_PrintText(0x2c, 0);
    if (gm_FindChar(ch) != 0)
        io_PrintText(0x2f);
    else
        io_PrintText(0x32);
    io_PrintText(0x33);
    io_PrintText(0x34);
    io_PrintText(0x35, (char *)(unsigned)&full_data_62a9e4);
    if (he_FindEvent(2, 0, 0x41) != 0) {
        while (he_NextEvent() != 0)
            ;
    }
    txt_Format(line, (char *)(unsigned)&full_data_62a56c, 0);
    d2_CreateWinText(0, 0, line, U_I((unsigned)&d2_LastWindow));
    sv_DrawBalanceBar(0x64, 0x4df);
    ai_CampaignStep(7, ch, 0, 0);
    d2_SetForm(0, 3);
    (void)w;
    (void)v;
    return win;
}
/* 0x5890c0, the drag window of a character of a lower kind (823 B, ROUGH SKELETON: the window, every call and
   the text ids are on the disassembly, the branch details around them are summarised): ch = the character
   record (+0 the word, +2 the kind). The kind 0xa and up -> return 0. The window 0x62a9f8 with 0x475600, the
   section 1, the text 0x62a9ec (the word), the section 4, the 0x38-byte record {ch} in 0x5be1d0 and, when it
   answers, 0x5be720 (0xa6, 5, U_I (0x6ed938), the line); the section 2; the kind != 0x14 -> 0x505e30 (the word)
   -> int in 0x515960 (0x2b); the flags and the texts 0x2c .. 0x41, the kind lists 0x53e400 / 0x53e300 and
   0x505df0 (ch) as in 0x588c60; the entries 0 .. 0x4d step 0xf: the line 0x62a56c (0x5176d0, i + 0x16b7),
   0x473650 (0, i * 0xf, the line, U_I (0x6ed938)) and 0x52eb50 (0x64, i * 0xf, ch, i, 0x4df); then the pump
   loop 0x5309a0 (this handler, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; 0x520f20 () and
   0x4758f0 (the window); 0 */


int ai_Unk5890c0(char *ch)
{
    char line[0x100];
    char rec[0x38];
    int win;
    int i;
    int k;

    if (*(unsigned char *)(ch + 2) >= 0xa)
        return 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a9f8);
    d2_CentreViewWindows(win);
    d2_SetForm(0, 1);
    io_PrintText((unsigned)&full_data_62a9ec, *(unsigned short *)ch);
    d2_SetForm(0, 4);
    memset(rec, 0, 0x38);
    *(char **)rec = ch;
    if (ai_AmountRowStatus(rec) != 0)
        ai_Unk5be720(0xa6, 5, U_I((unsigned)&d2_LastWindow), line);
    d2_SetForm(0, 2);
    if (*(unsigned char *)(ch + 2) != 0x14)
        io_PrintText(0x2b, (int)*(float *)&U_I(((unsigned)&ai_Unk5ee160 + 0x7e8)));
    io_PrintText(0x2d);
    io_PrintText(0x2c, 0);
    if (gm_FindChar(ch) != 0)
        io_PrintText(0x2f);
    io_PrintText(0x31);
    io_PrintText(0x33);
    io_PrintText(0x34);
    io_PrintText(0x35, (char *)(unsigned)&full_data_62a9e4);
    if (he_FindEvent(2, 0, 0x41) != 0) {
        while (he_NextEvent() != 0)
            ;
    }
    for (i = 0, k = 2; k < 0x4d; i++, k += 0xf) {
        txt_Format(line, (char *)(unsigned)&full_data_62a56c, (char *)(i + 0x16b7));
        d2_CreateWinText(0, i * 0xf, line, U_I((unsigned)&d2_LastWindow));
        sv_DrawBalanceBar(0x64, i * 0xf, ch, i, 0x4df);
    }
    while (sv_RunFrame(ai_Unk5890c0, 0x677c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    sv_ResetScreenClip();
    d2_CloseView(win);
    return 0;
}
/* 0x589400, clears one 12-byte slot of the table 0x6f5ec0 (14 B): its +4 / +8 = -1 */

void ai_Unk589400(int *p)
{
    p[1] = -1;
    p[2] = -1;
}
/* 0x589410, the init of the 12-byte slot table 0x6f5ec0 (63 B, 0xb slots up to 0x6f5f43): cleared, then each
   slot's byte 0 = i + 0x3b (0x57 for the last one, 0x6f5f38) and 0x589400 (the slot) */

void ai_Unk589410(void)
{
    char *p;
    int i;

    memset((char *)(unsigned)&full_data_6f5ec0, 0, 0x84);
    for (i = 0; i < 0xb; i++) {
        p = (char *)((unsigned)&full_data_6f5ec0 + i * 0xc);
        *(unsigned char *)p = (unsigned char)(i + 0x3b);
        if (i == 0xa)
            *(unsigned char *)(unsigned)&full_data_6f5f38 = 0x57;
        ai_Unk589400((int *)p);
    }
}

char * gm_FindBuildingById(int id); int gm_FindObjectInst(int id); unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c);

/* 0x589450, validates the 12-byte slots 0x6f5ec4 .. 0x6f5f4c (113 B): a slot's +0 (not -1) through
   0x502020 (id): 0 -> 0x589400 (the slot); else the +4 value through 0x4fe980: 0 while +4 is not -1 ->
   0x589400, else 0x503530 (the player word, id, that, 0) without bit 0 -> 0x589400 */
void ai_Unk589450(void)
{
    char *p;
    int id;
    int e;

    for (p = (char *)(unsigned)&full_data_6f5ec8; p < (char *)(unsigned)((unsigned)full_data_6f5ec8 + 0x84); p += 0xc) {
        if (*(int *)(p - 4) == -1)
            continue;
        id = gm_FindBuildingById(*(int *)(p - 4));
        if (id == 0) {
            ai_Unk589400((int *)(p - 8));
            continue;
        }
        e = gm_FindObjectInst(*(int *)p);
        if (e == 0 && *(int *)p != -1) {
            ai_Unk589400((int *)(p - 8));
            continue;
        }
        if ((gm_PermissionBits(*(unsigned short *)(unsigned)&ai_PlayerId, id, e, 0) & 1) == 0)
            ai_Unk589400((int *)(p - 8));
    }
}

void sv_SelectPanelObject(unsigned char *o, int b, char *c);

/* 0x5894d0, the key-triggered slot selection (151 B): with U_B (0x1468410) set and U_B (0x14681ea) clear, the
   slots 0x6f5ec4 .. 0x6f5f48 whose byte +0 is that key (0x589450 first): the first valid one (+0 not -1):
   0x502020 (its +0) and 0x4fe980 (its +4) -> 0x521960 (the first, the second's +2 or -1, 0x631090) */
void ai_Unk5894d0(void)
{
    char *p;
    int id;
    char *e;

    if (*(unsigned char *)(unsigned)&kb_Last == 0)
        return;
    if (*(unsigned char *)((unsigned)&kb_Down + 0x2a) != 0)
        return;
    for (p = (char *)(unsigned)&full_data_6f5ec4; p < (char *)(unsigned)((unsigned)full_data_6f5ec4 + 0x84); p += 0xc) {
        if (*(unsigned char *)(p - 4) != *(unsigned char *)(unsigned)&kb_Last)
            continue;
        if (*(unsigned char *)((unsigned)&kb_Down + 0x2a) != 0)
            continue;
        ai_Unk589450();
        if (*(int *)p != -1)
            break;
    }
    if (p >= (char *)(unsigned)((unsigned)full_data_6f5ec4 + 0x84))
        return;
    id = gm_FindBuildingById(*(int *)p);
    e = (char *)gm_FindObjectInst(*(int *)(p + 4));
    if (e != 0)
        sv_SelectPanelObjectK(id, *(int *)(e + 2), (char *)(unsigned)&ch_EmptyName);
    else
        sv_SelectPanelObjectK(id, -1, (char *)(unsigned)&ch_EmptyName);
}
/* 0x589570, the tooltip line of the slot idx (256 B): a = U_I (0x6f2308) (else U_I (0x6f230c) -> its
   0x4ffe90 (a + 0x5d, 2, 6, 0, 0xfd) when U_I (0x6f2310) is 0, else U_I (0x75ec60) or U_I (0x6f1bc8)),
   b = U_I (0x6f2310). The slot 0x6f5ec4 + idx * 0xc = a + 1, 0x6f5ec8 = b + 2, or -1 without b or with b's word
   0xfd; the line "%s = %G, %s" (0x62aa14: idx + 0x205e, a, b's word * 2 + 0x9a8) or "%s = %G" (0x62aa20) in
   0x5176d0, then 0x52e330 (the line) */

void ai_Unk589570(int idx)
{
    char buf[0x100];
    char *a;
    char *b;

    a = *(char **)(unsigned)&ai_PlayerBuilding;
    b = 0;
    if (a != 0) {
        b = *(char **)(unsigned)&sv_PanelShown;
    } else {
        a = *(char **)(unsigned)&ai_FallbackBuilding;
        if (a != 0) {
            b = *(char **)(unsigned)&sv_PanelShown;
            if (b == 0)
                gm_Query(a + 0x5d, 2, 6, 0, 0xfd);
        } else {
            a = *(char **)(unsigned)&sv_SelectedObject;
            if (a == 0)
                a = *(char **)(unsigned)&ai_MarketBuilding;
        }
    }
    *(int *)((unsigned)&full_data_6f5ec4 + idx * 0xc) = *(int *)(a + 1);
    if (b == 0 || *(unsigned short *)b == 0xfd) {
        *(int *)((unsigned)&full_data_6f5ec8 + idx * 0xc) = -1;
        txt_Format(buf, (char *)(unsigned)&full_data_62aa20, (char *)(idx + 0x205e), a);
    } else {
        *(int *)((unsigned)&full_data_6f5ec8 + idx * 0xc) = *(int *)(b + 2);
        txt_Format(buf, (char *)(unsigned)&full_data_62aa14, (char *)(idx + 0x205e), a, *(unsigned short *)b * 2 + 0x9a8);
    }
    sv_SetStatusText(buf);
}

int d2_GetViewWin(); int sv_CreateScrollButtons(); void d2_RadioClear(int g); int d2_RemoveWindowObjects(); void d2_MenuNavigate(int g);

/* 0x589670, the item-slot window (1066 B, ROUGH SKELETON: the window, every call and the widget ids are on the
   disassembly; the slot grid arithmetic is summarised): 0x589450 () first; the window 0x62aa38 with 0x475600,
   0x475f70 (win, 0), the sound 0x52ea30, 0x476f80 (0, 0), the section 2, the text 0x205b, the section 0; the
   rectangles 0x475ff0 / 0x52e860 (0x214, 0x168, 0x38, ...) and the buttons 0x52e350 (0, 2, the list); 0x476b70 ();
   the drag window 0xfa5aa8 in 0x46c600, 0x472d20 (U_I (0x6ed938), 1). The slots: 0x4731d0 (0x10, the slot + 4),
   0x502020 (the id) / 0x4fe980, 0x4731d0 (0x60, i + 0x427), 0x473650 (0x9c, ...), 0x473650 (0x60, ...,
   0x1442334), 0x4731d0 (0x128, ...), 0x473650 (0x164, ...), 0x589400 (the slot 0x6f5ec0 + i * 0xc). The pump
   loop 0x5309a0 (this window, 0x657c7): 0x589570 (the selected slot) when 0xfa5aa8 is not -1, 0x46c6c0,
   0x475e40 (the buttons), 0x5894d0 (); the exit 0x46c600 and 0x4758f0 (the window) */
void ai_Unk589670(void)
{
    char line[0x100];
    int win;
    int i;
    int id;
    int e;
    int p;

    ai_Unk589450();
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62aa38);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    io_PrintText(0x205b);
    d2_SetForm(win, 0);
    d2_ClearInput();
    for (i = 0; i < 0xb; i++) {
        p = d2_WinOpenPic(0x10, 0, (char *)(i + 4), U_I((unsigned)&d2_LastWindow));
        id = *(int *)((unsigned)&full_data_6f5ec4 + i * 0xc);
        if (id != -1)
            e = gm_FindBuildingById(id);
        else
            e = 0;
        d2_WinOpenPic(0x60, *(unsigned char *)((unsigned)&full_data_6f5ec0 + i * 0xc) + 0x427, (char *)win, U_I((unsigned)&d2_LastWindow));
        d2_CreateWinText(0x9c, i, line, U_I((unsigned)&txt_ObjectNames));
        d2_CreateWinText(0x60, i, line, U_I((unsigned)&full_data_1442334));
        d2_WinOpenPic(0x128, 0, (char *)(i + 0xdf), U_I((unsigned)&d2_LastWindow));
        d2_CreateWinText(0x164, i, line, U_I((unsigned)&full_data_1442334));
        (void)e;
        (void)p;
    }
    while (sv_RunFrame(ai_Unk589670, 0x657c7) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (*(int *)((unsigned)&d2_RadioSel) != -1)
            ai_Unk589570(*(int *)((unsigned)&d2_RadioSel));
        d2_MenuNavigate(0);
        d2_SetObjEnabled(0, 0);
        ai_Unk5894d0();
    }
    d2_RadioClear(0);
    d2_CloseView(win);
}

unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);

/* 0x589aa0, loads the 12-byte slot table 0x6f5ec0 from a file (142 B): n = 0xb read through 0x430e30 (&n, 4, 1,
   fh), then each slot (the byte +0, the dword +4, the dword +8); 0 on a short read, 1 */
int ai_SaveSlotTable(int fh)
{
    int n;
    int i;
    char *p;

    n = 0xb;
    if (vfs_fwrite(&n, 4, 1, fh) == 0)
        return 0;
    i = 0;
    p = (char *)(unsigned)&full_data_6f5ec4;
    while (i < n) {
        if (vfs_fwrite(p - 4, 1, 1, fh) == 0)
            return 0;
        if (vfs_fwrite(p, 4, 1, fh) == 0)
            return 0;
        if (vfs_fwrite(p + 4, 4, 1, fh) == 0)
            return 0;
        i++;
        p += 0xc;
    }
    return 1;
}

int ls_Read();

/* 0x589b30, the 0x589aa0 counterpart over 0x51c920 (136 B): n = 0 read first, then each slot's byte +0, dword
   +4 and dword +8 from the table 0x6f5ec0; 0 on a failed read, 1 */
int ai_Unk589b30(int fh)
{
    int n;
    int i;
    char *p;

    n = 0;
    if (ls_Read(&n, 4, 1, fh) == 0)
        return 0;
    i = 0;
    p = (char *)(unsigned)&full_data_6f5ec4;
    while (i < n) {
        if (ls_Read(p - 4, 1, 1, fh) == 0)
            return 0;
        if (ls_Read(p, 4, 1, fh) == 0)
            return 0;
        if (ls_Read(p + 4, 4, 1, fh) == 0)
            return 0;
        i++;
        p += 0xc;
    }
    return 1;
}

void * mem_Alloc(int size, char *tag); char dd_Lock(void); void d2_Convert16ToRgb(); void dd_Unlock(); unsigned char bmp_Save24(char *path, int w, int h, unsigned char *data); void mem_Free(void *p);

/* 0x589bc0, a screenshot to disk (198 B): 0x52e330 (0x631090) and the pump 0x5309a0 (0, U_I (0x760638)); the
   name 0x62aa58 with the counter 0x6f2674 (incremented) in 0x5de652; the image 0x4223c0 (the screen 0xfb9072 *
   0xfb9070 * 3, 0x62aa4c), 0x41dec0 (), 0x478c50 (0, 0, 0xfb9072, 0xfb9070, U_I (0x1463590), 0, 0, the image),
   0x41e050 (), 0x401b90 (the name, 0xfb9072, 0xfb9070, the image), 0x422620 (the image) and 0x52e330
   (U_I (0x144adb4)) */
void ai_Unk589bc0(void)
{
    char name[0x40];
    char *img;
    int n;

    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    sv_RunFrame(0, U_I((unsigned)&sv_FrameMask));
    n = U_I((unsigned)&full_data_6f2674);
    U_I((unsigned)&full_data_6f2674) = n + 1;
    sprintf(name, (char *)(unsigned)&full_data_62aa58, n);
    img = (char *)mem_Alloc(*(short *)(unsigned)&d2_ScreenW * *(short *)(unsigned)&d2_ScreenH * 3, (char *)(unsigned)&full_data_62aa4c);
    dd_Lock();
    d2_Convert16ToRgb(0, 0, *(short *)(unsigned)&d2_ScreenW, *(short *)(unsigned)&d2_ScreenH, U_I((unsigned)&dd_Surface), 0, 0, img);
    dd_Unlock();
    bmp_Save24(name, *(short *)(unsigned)&d2_ScreenW, *(short *)(unsigned)&d2_ScreenH, img);
    mem_Free(img);
    sv_SetStatusText((char *)U_I((unsigned)&full_data_144adb4));
}
/* 0x589c90, the slot keys each frame (86 B): U_B (0x1468410) set and U_I (0x75e8c0) not 1; the key 0x58 ->
   0x589670 (); 0x5894d0 (); each slot 0x6f5ec0 + i * 0xc up to 0x6f5f44 whose byte 0 is the key, with
   U_B (0x14681ea) set -> 0x589570 (i) */

void ai_SlotKeys(void)
{
    char *p;
    int i;

    if (*(unsigned char *)(unsigned)&kb_Last == 0)
        return;
    if (U_I((unsigned)&sv_PanelFlag) == 1)
        return;
    if (*(unsigned char *)(unsigned)&kb_Last == 0x58)
        ai_Unk589670();
    ai_Unk5894d0();
    i = 0;
    for (p = (char *)(unsigned)&full_data_6f5ec0; p < (char *)(unsigned)((unsigned)full_data_6f5ec0 + 0x84); p += 0xc) {
        if (*(unsigned char *)p == *(unsigned char *)(unsigned)&kb_Last && *(unsigned char *)((unsigned)&kb_Down + 0x2a) != 0)
            ai_Unk589570(i);
        i++;
    }
}

void ai_Quicksave(void); void ai_ApplyOptions(); void ai_Unk4c3f20(void); void ai_Unk4c3f50(void);

/* 0x589cf0, the speed keys each frame (264 B): the game speed 0x6f3608 in steps of 40, v = (int)(speed / 40 +
   0.5f); key 0x1468410 0 = none and the pause 0x75e8c0 == 1 return at once; the key 0x1f takes a screenshot
   (0x589bc0), the key 0x10 runs 0x5d7430 while 0x6ed9dc is 0 and the mask 0x760638 has no 0x10000; then the mode
   byte 0x6f23f8: bit 2 (the network game) - with bit 0x10 the keys 0x4e / 0x4a only raise / lower 0x4c3f20 /
   0x4c3f50, without it they show the text U_I (0x143a3d8) via 0x52e330; without bit 2 they raise / lower and step
   the speed between 0 and 4 * 40 by 40, then 0x5d5e90 applies it (the keys are read once more after each call) */
void ai_SpeedKeys(void)
{
    unsigned char key;
    unsigned char fl;
    int v;

    v = (int)((float)(U_I((unsigned)&ai_OptionSpeed) / 40) + 0.5f);
    key = *(unsigned char *)(unsigned)&kb_Last;
    if (key == 0)
        return;
    if (U_I((unsigned)&sv_PanelFlag) == 1)
        return;
    if (key == 0x1f) {
        ai_Unk589bc0();
        key = *(unsigned char *)(unsigned)&kb_Last;
    }
    if (key == 0x10 && U_I((unsigned)&d2_FocusedInput) == 0 && (U_I((unsigned)&sv_FrameMask) & 0x10000) == 0) {
        ai_Quicksave();
        key = *(unsigned char *)(unsigned)&kb_Last;
    }
    fl = *(unsigned char *)(unsigned)&ai_GameFlags;
    if ((fl & 4) == 0) {
        if (key == 0x4e) {
            ai_Unk4c3f20();
            if (v < 4) {
                v++;
                U_I((unsigned)&ai_OptionSpeed) = v * 40;
                ai_ApplyOptions();
            }
            key = *(unsigned char *)(unsigned)&kb_Last;
        }
        if (key == 0x4a) {
            ai_Unk4c3f50();
            if (0 < v) {
                U_I((unsigned)&ai_OptionSpeed) = v * 40 - 40;
                ai_ApplyOptions();
                return;
            }
        }
    } else if ((fl & 0x10) != 0) {
        if (key == 0x4e) {
            ai_Unk4c3f20();
            return;
        }
        if (key == 0x4a) {
            ai_Unk4c3f50();
            return;
        }
    } else {
        if (key == 0x4e || key == 0x4a)
            sv_SetStatusText(U_I((unsigned)&full_data_143a3d8));
    }
}

int gm_BuildingMainObject(unsigned char *b); void gm_GetGebaeudeHauptraum(unsigned char *b);

/* 0x589e00, fills slot i of the table 0x6f5ec0 from the building b (79 B): the building's byte +0 is its kind, its
   id at +1 -> the slot's +4; the main object of the building (0x501b50, else 0x501ce0; none for the kind 0x1d)
   gives its id at +2 -> the slot's +8, -1 without an object */
void ai_Unk589e00(int i, char *b)
{
    char *o;
    int off;

    o = 0;
    if (*(unsigned char *)b != 0x1d) {
        o = (char *)gm_BuildingMainObject(b);
        if (o == 0)
            o = (char *)((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(b);
    }
    off = i * 0xc;
    *(int *)((unsigned)&full_data_6f5ec4 + off) = *(int *)(b + 1);
    if (o != 0)
        *(int *)((unsigned)&full_data_6f5ec8 + off) = *(int *)(o + 2);
    else
        *(int *)((unsigned)&full_data_6f5ec8 + off) = -1;
}

unsigned char * gm_BuildNext(void); unsigned char * gm_BuildQuery(int n, ...);

/* 0x589e50, fills the building slots of the player (184 B): the table 0x6f5ec0 (0x589e00) gets slot 0 the market
   building 0x6f1bc8, slot 1 the town hall (the query 0x501190 (1, 5, 0xf) = the building type 0xf), the next slot
   the building at +0x170 of the first character of the kind 6 (the records 0x770b80 + i * 0x21c, at most 0x300),
   then from slot 3 the player's buildings (0x501190 (1, 4, i) / 0x501030, the word +0x27 = the character index)
   whose building kind record (0x7e3f30 + kind * 0x24d) byte 0 is not 2 */
void ai_Unk589e50(void)
{
    unsigned char *b;
    int i;
    int n;

    i = 0;
    b = (unsigned char *)((unsigned)&ai_Chars + 0x2);
    do {
        if (*b == 6)
            break;
        b += 0x21c;
        i++;
    } while ((int)b < (unsigned)((unsigned)ai_Chars + 0x65402));
    n = 1;
    ai_Unk589e00(0, (char *)U_I((unsigned)&ai_MarketBuilding));
    b = (unsigned char *)gm_BuildQuery(1, 5, 0xf);
    if (b != 0)
        ai_Unk589e00(n++, (char *)b);
    b = (unsigned char *)U_I(((unsigned)&ai_Unk770cec + 0x4) + i * 0x21c);
    if (b != 0)
        ai_Unk589e00(n, (char *)b);
    n = 3;
    b = (unsigned char *)gm_BuildQuery(1, 4, i);
    while (b != 0) {
        if (*(unsigned char *)(U_I((unsigned)&ai_BTypes) + *b * 0x24d) != 2)
            ai_Unk589e00(n++, (char *)b);
        b = (unsigned char *)gm_BuildNext();
    }
}
