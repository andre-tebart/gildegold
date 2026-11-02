/* unk_575860.c -- the tutorial module ("FT*" record names, tutorial\* forms, TUTORIAL_* texts), 0x575860..0x589f10:
   the tutorial state machine (state record S = the pointer 0x5eec60, states 1 .. 0xc, the jump table 0x575fe8;
   0x578180 initializes it, the tick 0x575860 runs it), the chapter record chains and their step / animation
   tables, and the tutorial's location / contact / tooltip windows. Start: he.c's last function 0x575710, its
   jump table and nop fill end at 0x575860; the unit's one .data block starts at 0x62918c (the "FT" state names)
   and its literals climb in function order to 0x62aa58; the network lobby (0x589f10) is a unit of its own.
   The original file name is unknown (no leaked tag). Names are ours.
   Open: type debt - the state record S, the 0x74-byte chapter records and the message (AIMSG) / contact-list
   records are accessed by byte offset; ai.h's AIMSG and a record type would clear most of the drafts' offsets.
   Open: not included yet (dcp headers conflicts unk_575860 HEADER): d2.h gfx.h sv.h unk_5d33b0.h; held by own
   declarations that differ from the owner's: d2_CreateWinText, SHOTREC; calls whose argument count differs from
   the definition (read the original's pushes): d2_GetObjValue, d2_PlaceMainWindow, sv_DrawBalanceBar.
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
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern const int sv_PhaseMinutes[24];
extern GAPDATA full_data_6f265c[];
extern GAPDATA full_data_6f2660[];
extern int sv_PanelFlag;
extern GAPDATA full_data_5f1890[];
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_629304[];
extern GAPDATA full_data_602568[];
extern int d2_PointerObj;
extern unsigned char kb_Last;
extern GAPDATA full_data_629314[];
extern GAPDATA full_data_629328[];
extern GAPDATA full_data_629340[];
extern GAPDATA full_data_629354[];
extern GAPDATA full_data_629368[];
extern GAPDATA full_data_62918c[];
extern int full_data_6291a0[5];
extern GAPDATA full_data_62937c[];
extern GAPDATA full_data_629390[];
extern GAPDATA full_data_6f2658[];
extern GAPDATA full_data_6f2640[];
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
extern GAPDATA full_data_6f2644[];
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
extern GAPDATA full_data_6f2648[];
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
extern GAPDATA full_data_6f264c[];
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
extern GAPDATA full_data_6f2650[];
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
extern GAPDATA full_data_6f2654[];
extern GAPDATA full_data_62a224[];
extern GAPDATA full_data_62a238[];
extern int sv_FrameDepth;


extern GAPDATA full_data_624ee0[];
#include "f3init_types.h"
extern GAPDATA full_data_6024f8[];
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_62918c[];
extern GAPDATA full_data_62a248[];



extern GAPDATA full_data_62a284[];
extern GAPDATA full_data_62a2a4[];
extern unsigned char ai_TimeHour;
extern int d2_PointerObj;
extern int d2_LastWindow;
extern GAPDATA full_data_62a2c4[];
extern GAPDATA full_data_6f2664[];
extern GAPDATA full_data_6f9f60[];
extern const int sv_PhaseMinutes[24];
extern int ai_Unkfb9060;
extern int ai_Unkfb9064;
extern int ai_Unkfb9068;
extern int ai_Unkfb906c;
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
extern GAPDATA full_data_6f9f98[];
extern GAPDATA full_data_6f9fd0[];
extern GAPDATA full_data_6fa020[];
extern int sv_PanelFlag;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_6f9f68[];
extern GAPDATA full_data_6f9f70[];
extern GAPDATA full_data_6f9f74[];


extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a364[];
extern int ai_ClockMs;
extern int sv_PopRequest;
extern GAPDATA full_data_5f36e0[];
extern GAPDATA full_data_62a36c[];
extern GAPDATA full_data_6f9f60[];
extern GAPDATA full_data_6f9f68[];
extern GAPDATA full_data_6f9f6c[];
extern GAPDATA full_data_6f9f70[];
extern GAPDATA full_data_6f9f74[];
extern char ai_Unk925114[4];
extern char ai_Unk925118[4];
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
extern char ai_Chars[768][0x21c];
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
extern unsigned char ai_OwnPlayer;
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
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a5bc[];
extern GAPDATA full_data_62a6f8[];
extern GAPDATA full_data_62a70c[];
extern char ai_Time[14];
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
extern int ai_Unk6f2448;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern GAPDATA full_data_62a84c[];
extern int sv_PopRequest;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;


extern GAPDATA full_data_62a870[];
extern GAPDATA full_data_62a88c[];
extern int ai_Unk6f2448;
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
extern int ai_OptionSpeed;

extern unsigned char kb_Last;
extern int sv_FrameMask;
extern int sv_PanelFlag;
extern GAPDATA full_data_6f5ec4[];
extern GAPDATA full_data_6f5ec8[];
extern char ai_Chars[768][0x21c];

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_PickActionBuilding)(p, a, fl, b, c, f, o)

#define U_I(a) (*(int *)(a))

void sv_CameraFollow();

void ai_Unk577c60();
void ai_Unk57b8a0();
void ai_Unk577d90();
int ai_Unk5778b0();
int ai_Unk577a00();
int ai_Unk577e50();
void d2_SetObjEnabled();
int ai_Unk5780e0();
void ai_Unk577b90();
void sv_CameraFollow();
void sv_SetStatusText(char *s);
void sv_SelectPanelObject(unsigned char *o, int b, char *c);
int he_Unk55ea60();
void sv_Unk52f9f0();
void sv_Unk52fa00();
void sv_FitForm(int f);
void ai_Unk577a90();
void ai_Unk577b60();
struct SBSAMPLE;
extern int sv_FrameBase; /* 0x6f22a4 */
extern char *txt_Ptrs[]; /* 0x143a1c0 */

/* the state record is at 0x6fa060 (the 0x40 byte of main.c's 0x6fa020 block); 0x5eec60 is a const pointer to it
   in the original. A load through that pointer (not the fixed address) is what the unit's functions compile to,
   and the full link relocates the initializer, so the record is ours and no neighbour is touched. */
char *const ai_Unk5eec60 = (char *)full_data_6fa020 + 0x40;  /* 0x5eec60, the pointer to the tutorial state record */
#define S (*(char **)(unsigned)&ai_Unk5eec60)  /* the tutorial state record (at 0x6fa060): the pointer ai_Unk5eec60 holds */

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
#include "unk_458810.h"
#include "dd_vesa.h"
#define sv_SelectPanelObjectK ((int (*)())sv_SelectPanelObject)  /* 0x52c4c0?: @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
/* 0x575860, the end-of-game tick (1984 B, ROUGH SKELETON): validates the end-of-game gates and the state record,
   then runs the state machine on *(int *)0x5eec60 - 1 (12 states, jump table 0x575fe8, the states 1 .. 0xc). The
   case bodies are not written yet: their helper calls, in address order, are 0x476f80, 0x577c60, 0x57b8a0,
   0x577d90, 0x5778b0, 0x576040 (the state transition), the state record writes, 0x53dcb0 (the end-of-game
   message), 0x577a00, 0x577a90, 0x577b60 (the clock writers 0x6333cc / 0x6333e0), 0x4741a0, 0x577e50,
   0x475e40, 0x5780e0, 0x577b90, 0x52d7a0, 0x52e330, 0x426f20, 0x427290, 0x4c2760; the exit is -4 (0xfffffffc)
   on a failed gate. Ends at 0x575fe2 (a shared return), the jump table 0x575fe8 .. 0x576017, nop fill to 0x576020. */

int ai_Unk575860(void)
{
    char *st;
    char *per;

    if (!U_I((unsigned)&full_data_6f265c))
        return -4;
    st = S;
    if (!st)
        return -4;
    if (!*(int *)(st + 0x18))
        return -4;
    per = *(char **)(st + 0x1c);
    if (!*(int *)(per + 0x74))
        return -4;
    /* 0x57589d .. 0x575915: the first-run / pause handling: the state 3 and the gate pairs 0x6310e4 / 0x75e8c0
       (0x476f80), the state record [st] = [st+4] when the progress word changed, [st+0x30] == 1 and [st] != 9
       resets the flag and calls 0x577c60 / 0x57b8a0(2) */
    if (U_I((unsigned)&he_ActiveSlot) == *(int *)(per + 0x74)) {
        if (U_I(st) != 2 && !U_I((unsigned)&d3t_CamMoving) && !U_I((unsigned)&sv_PanelFlag)) {
            d2_SetPointerSprite(0, 0);
            *(int *)(S) = 3;
        }
    }
    st = S;
    if (U_I((unsigned)&he_ActiveSlot) != *(int *)(*(char **)(st + 0x1c) + 0x74)) {
        *(int *)st = *(int *)(st + 4);
        st = S;
    }
    if (*(int *)(st + 0x30) == 1 && U_I(st) != 9) {
        *(int *)(st + 0x30) = 0;
        ai_Unk577c60();
        ai_Unk57b8a0(2);
    }
    if (!U_I((unsigned)&full_data_6f2660))
        goto end;
    ai_Unk577d90();
    if (U_I((unsigned)&d3t_CamMoving) || U_I((unsigned)&sv_PanelFlag))
        goto end;
    st = S;
    switch (U_I(st) - 1) {
    case 0:                     /* state 1, 0x57595a: show the main-intro text and start the step chain */
        if (d3t_CamMove != 0)
            break;
        if (ai_Unk5778b0() != 0)
            return -4;
        U_I(S + 0x38) = 0;
        U_I(S + 0x3c) = 0;
        U_I(S) = 2;
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 8) == 1 || *(int *)(per + 8) == 2) {
            U_I(S + 4) = 4;
            U_I(S + 0x2c) = 0;
        } else {
            U_I(S + 4) = 0xb;
            U_I(S + 0x2c) = 1;
        }
        if (*(int *)(per + 8) == 2)
            U_I(S + 0x3c) = 1;
        if (*(int *)(per + 0x38))
            ai_CampaignStep(2, 0, 0, 0);
        U_I(S + 0x50) = 0;
        U_I(S + 0x58) = *(int *)(per + 0x64) != 0;
        U_I(S + 0x54) = 0;
        U_I(S + 0x5c) = 0;
        U_I(S + 0x60) = -1;
        if (*(int *)(per + 0x34))
            ((void (*)(int))*(int *)(per + 0x34))(0);
        he_Unk53dcb0(*(int **)(*(char **)(S + 0x1c) + 0x74));
        d2_SetPointerSprite(0, 0);
        break;
    case 1:                     /* state 2, 0x575a69: the intro is answered, close the window and continue */
        if (ai_Unk577a00() != 1)
            break;
        U_I(S) = U_I(S + 4);
        U_I(S + 8) = U_I((unsigned)&ai_ClockMs);
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        U_I(S + 0x10) = 0;
        he_Unk53dcb0(0);
        break;
    case 2:                     /* state 3, 0x575ab5: the same without the clocks */
        if (ai_Unk577a00() != 1)
            break;
        U_I(S) = U_I(S + 4);
        he_Unk53dcb0(0);
        break;
    case 3:                     /* state 4, 0x575adc: the timed first text window */
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x40) == 3) {
            ai_CampaignStep(0, 0, 0, 0);
            break;
        }
        if (U_I((unsigned)&ai_ClockMs) > U_I(st + 8) + *(int *)(per + 0x2c)) {
            U_I(st + 8) = U_I((unsigned)&ai_ClockMs);
            if (U_I(st + 0x28) == 0 && *(int *)(per + 0x14) != 0) {
                U_I(st + 0x28) = 1;
                ai_Unk577a90();
            }
        } else if (U_I((unsigned)&ai_ClockMs) > U_I(st + 8) + 0x238) {
            if (U_I(st + 0x28) != 1)
                break;
            U_I(st + 8) = U_I((unsigned)&ai_ClockMs);
            U_I(st + 0x28) = 0;
            ai_Unk577b60();
        }
        if (U_I(st + 0x28) == 1)
            d2_RaiseViewWindows(U_I(st + 0x24));
        break;
    case 4:                     /* state 5, 0x575b82: drop the intro window and open the first step */
        U_I(S + 8) = U_I((unsigned)&ai_ClockMs);
        U_I(S + 0x28) = 0;
        ai_Unk577b60();
        U_I(S + 0x3c) = 2;
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x50) == 0) {
            U_I(S) = 7;
            U_I(S + 4) = 7;
            break;
        }
        U_I(S + 0x40) = 0;
        U_I(S) = 6;
        U_I(S + 4) = 6;
        ai_Unk577e50();
        break;
    case 5:                     /* state 6, 0x575bdc: one step of the chapter chain */
        st = S;
        per = *(char **)(st + 0x18);
        U_I((unsigned)&tb_Paused) = U_I((unsigned)&ai_ClockMs) >= *(int *)(*(int *)(per + 0x58) + U_I(st + 0x40) * 0x10) + U_I(st + 0xc);
        d2_SetObjEnabled(U_I(st + 0x4c), U_I((unsigned)&tb_Paused));
        if (ai_Unk5780e0() != 1)
            break;
        U_I(S + 0x40)++;
        U_I(S + 8) = U_I((unsigned)&ai_ClockMs);
        st = S;
        per = *(char **)(st + 0x18);
        if (U_I(st + 0x40) < *(int *)(per + 0x50)) {
            U_I(st + 0xc) = U_I((unsigned)&ai_ClockMs);
            ai_Unk577e50();
            break;
        }
        U_I(S) = 7;
        U_I(S + 4) = 7;
        break;
    case 6:                     /* state 7, 0x575c74: the chapter is done, pick the next state */
        U_I(S + 0x3c) = 3;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        U_I(S + 0x50) = 0;
        ai_Unk577e50();
        U_I((unsigned)&tb_Paused) = 0;
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x4c) == 0) {
            U_I(S) = 9;
            U_I(S + 4) = 9;
        } else {
            U_I(S) = 0xa;
            U_I(S + 4) = 0xa;
            U_I(S + 0x2c) = *(int *)(per + 0x24) == 0;
        }
        break;
    case 7:                     /* state 8, 0x575fe2: nothing */
        break;
    case 8:                     /* state 9, 0x575cec: the second text window and the run update */
        if (U_I(S + 0x28) == 1) {
            U_I(S + 8) = U_I((unsigned)&ai_ClockMs);
            U_I(S + 0x28) = 0;
            ai_Unk577b60();
        }
        st = S;
        per = *(char **)(st + 0x18);
        if (U_I(st + 0x30) == 0 && *(int *)(per + 0x1c) != 0) {
            U_I(st + 0x30) = 1;
            ai_Unk577b90();
            ai_Unk57b8a0(0);
        }
        if (U_I(S + 0x30) == 1)
            d2_RaiseViewWindows(U_I(S + 0x34));
        ai_Unk57b8a0(1);
        break;
    case 9:                     /* state 10, 0x575d57: the chapter's timed tail */
        if (U_I(S + 0x28) == 1) {
            U_I(S + 8) = U_I((unsigned)&ai_ClockMs);
            U_I(S + 0x28) = 0;
            ai_Unk577b60();
        }
        st = S;
        per = *(char **)(st + 0x18);
        if (U_I((unsigned)&ai_ClockMs) > U_I(st + 0xc) + *(int *)(per + 0x44)) {
            sv_CameraFollow();
            U_I((unsigned)&sv_PopRequest) = U_I((unsigned)&sv_FrameDepth) - U_I((unsigned)&sv_FrameBase);
            U_I(S) = 1;
            U_I(S + 4) = 1;
            U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        }
        st = S;
        if (U_I(st + 0x2c) != 0)
            break;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x24) == 0)
            break;
        sv_SetStatusText(txt_Ptrs[*(int *)(per + 0x24)]);
        if (*(int *)(per + 0x28) != 0) {
            if (U_I(S + 0x14)) {
                if (sb_IsPlaying((struct SBSAMPLE *)U_I(S + 0x14)))
                    sb_Stop((struct SBSAMPLE *)U_I(S + 0x14), 0);
                U_I(S + 0x14) = 0;
            }
            U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, (char *)U_I(per + 0x28), -1);
        }
        U_I(S + 0x2c) = 1;
        ai_Unk5778b0();
        break;
    case 10:                    /* state 11, 0x575e68: the same with the second window's timing */
        if (U_I(S + 0x28) == 1) {
            U_I(S + 8) = U_I((unsigned)&ai_ClockMs);
            U_I(S + 0x28) = 0;
            ai_Unk577b60();
        }
        st = S;
        per = *(char **)(st + 0x18);
        if (U_I((unsigned)&ai_ClockMs) > U_I(st + 0xc) + *(int *)(per + 0x30)) {
            sv_CameraFollow();
            U_I((unsigned)&sv_PopRequest) = U_I((unsigned)&sv_FrameDepth) - U_I((unsigned)&sv_FrameBase);
            U_I(S) = 0xc;
            U_I(S + 4) = 0xc;
            U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        }
        st = S;
        if (U_I(st + 0x2c) != 0)
            break;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x24) == 0)
            break;
        sv_SetStatusText(txt_Ptrs[*(int *)(per + 0x24)]);
        if (*(int *)(per + 0x28) != 0) {
            if (U_I(S + 0x14)) {
                if (sb_IsPlaying((struct SBSAMPLE *)U_I(S + 0x14)))
                    sb_Stop((struct SBSAMPLE *)U_I(S + 0x14), 0);
                U_I(S + 0x14) = 0;
            }
            U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, (char *)U_I(per + 0x28), -1);
        }
        U_I(S + 0x2c) = 1;
        ai_Unk5778b0();
        break;
    case 11:                    /* state 12, 0x575f80: next chapter record */
        if (U_I(S + 0x28) == 1) {
            U_I(S + 8) = U_I((unsigned)&ai_ClockMs);
            U_I(S + 0x28) = 0;
            ai_Unk577b60();
        }
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x34))
            ((void (*)(int))*(int *)(per + 0x34))(1);
        st = S;
        per = *(char **)(st + 0x18);
        *(char **)(st + 0x18) = *(char **)(per + 0x70);
        U_I(S) = 1;
        U_I(S + 4) = 1;
        sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
        break;
    }
end:
    return 0;
}
/* 0x576020, clears the end-of-game gate 0x6f2660 (checked by the tick 0x575860 at 0x575915) */

void unk_ClearEndGameGate(void)
{
    U_I((unsigned)&full_data_6f2660) = 0;
}
/* 0x576030, sets the end-of-game gate 0x6f2660 (the setter counterpart of 0x576020) */

void unk_SetEndGameGate(void)
{
    U_I((unsigned)&full_data_6f2660) = 1;
}
/* 0x576040, one step of the campaign state machine (763 B): `a` is the expected index, `b` and `c` the event
   kind / result, `d` is only handed to the callback. The record 0x5eec60: +0 the state, +4 the pending state,
   +0xc the time of the step (0x6333cc), +0x10 the step counter, +0x18 the run record; the run record holds the
   callback +0x38 (called with the four arguments as an int[4]), +0x3c the mode (1 / 3), +0x40 the expected index,
   +0x44 / +0x48 the transition times, +0x4c an index, +0x50 a stored value. Every path ends with the step
   counter +0x10 incremented; the failed gates at the top return without it. */

void ai_CampaignStep(int a, int b, int c, int d)
{
    int p[4];
    int (*cb)(int *);
    char *r;

    if (!U_I((unsigned)&full_data_6f265c))
        return;
    if (!*(int *)(S + 0x18))
        return;
    p[0] = a;
    p[1] = b;
    p[2] = c;
    p[3] = d;
    r = *(char **)(S + 0x18);
    if (*(int *)(r + 8) == 2)
        goto st2;
    if (U_I(S + 4) != 4 && U_I(S + 4) != 0xa)
        return;
    /* 0x5760a4 */
    cb = *(int (**)(int *))(r + 0x38);
    if (cb) {
        cb(p);
        U_I(S + 0x10)++;
        return;
    }
    if (*(int *)(r + 0x40) == 3) {
        if (U_I((unsigned)&ai_ClockMs) > *(int *)(r + 0x44) + U_I(S + 0xc)) {
            U_I(S) = *(int *)(r + 0x48);
            r = *(char **)(S + 0x18);
            U_I(S + 4) = *(int *)(r + 0x48);
            U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
            U_I(S + 0x2c) = 1;
        }
        return;
    }
    if (*(int *)(r + 0x40) != a)
        goto inc;
    if (c == 0) {
        U_I(S) = *(int *)(r + 0x48);
        U_I(S + 4) = U_I(S);
    } else {
        U_I(S) = 0xb;
        U_I(S + 4) = U_I(S);
    }
    goto clocked;

st2:
    if (U_I(S + 4) != 4 && U_I(S + 4) != 9 && U_I(S + 4) != 0xa)
        return;
    cb = *(int (**)(int *))(r + 0x38);
    if (cb) {
        cb(p);
        return;
    }
    if (U_I(S + 0x3c) == 1) {
        if (c != 0) {
            if (c != 1 || *(int *)(r + 0x40) != a)
                goto apply;
            U_I(S + 4) = 4;
            U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
            U_I(S + 0x30) = 0;
            ai_Unk577c60();
            ai_Unk57b8a0(2);
            goto apply;
        }
        if (*(int *)(r + 0x40) != a)
            goto apply;
        U_I(S + 4) = 5;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        if (b)
            U_I(S + 0x50) = b;
        goto apply;
    }
    if (U_I(S + 0x3c) != 3)
        goto inc;
    if (U_I(S + 4) == 9) {
        if (*(int *)(r + 0x4c) && *(int *)(r + 0x4c) == a) {
            U_I(S) = 0xa;
            U_I(S + 4) = 0xa;
            U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
            U_I(S + 0x10)++;
            return;
        }
        if (*(int *)(r + 0x40) != a || c != 1)
            goto inc;
        U_I(S) = 4;
        U_I(S + 4) = 4;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        U_I(S + 0x30) = 0;
        ai_Unk577c60();
        ai_Unk57b8a0(2);
        U_I(S + 0x10)++;
        return;
    }
    if (U_I(S + 4) == 0xa) {
        if (*(int *)(r + 0x40) != a || c != 1)
            goto inc;
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        goto clocked;
    }
    if (U_I(S + 4) == 4) {
        if (*(int *)(r + 0x40) != a || c != 0)
            goto inc;
        U_I(S) = 9;
        U_I(S + 4) = 9;
        goto clocked;
    }
    goto inc;

apply:
    U_I(S) = U_I(S + 4);
    U_I(S + 0x10)++;
    return;
clocked:
    U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
    U_I(S + 0x10)++;
    return;
inc:
    U_I(S + 0x10)++;
}
/* 0x576340, calls the run record's callback +0x3c with the four arguments as an int[4] and returns its result;
   1 when a gate fails (0x6f265c, the run record, [r+0x3c]), 0 while 0x6333e0 is set (a transition in progress) */

int ai_ScriptQuery(int a, int b, int c, int d)
{
    int p[4];
    char *r;

    if (!U_I((unsigned)&full_data_6f265c))
        return 1;
    if (!*(int *)(S + 0x18))
        return 1;
    if (U_I((unsigned)&tb_Paused))
        return 0;
    r = *(char **)(S + 0x18);
    if (!*(int *)(r + 0x3c))
        return 1;
    p[0] = a;
    p[1] = b;
    p[2] = c;
    p[3] = d;
    return ((int (*)(int *))*(int *)(r + 0x3c))(p);
}
/* 0x5763c0, 1 when the run record's word +0x68 has a bit of `mask` (the run mask), else 0; 1 while 0x6f265c is 0
   and when there is no run record, 0 in the mode 2 / state 5 and while a transition 0x6333e0 is pending */

int ai_RunAllows(int mask)
{
    char *st;

    if (!U_I((unsigned)&full_data_6f265c))
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

    if (!U_I((unsigned)&full_data_6f265c))
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

    if (!U_I((unsigned)&full_data_6f265c))
        return 1;
    st = S;
    if (*(int *)(st + 0x3c) == 2 || U_I(st) == 5)
        return 0;
    return 1;
}
/* 0x576490, a callback predicate on the first of the four step arguments (a pointer to the int[4]): 0 for the
   kinds 0x19 / 0x1b / 0x29, else 1 (Ghidra missed this start; it ends the 80 byte record of 0x576460 at 0x5764ae) */

int ai_Unk576490(int *p)
{
    if (*p == 0x19 || *p == 0x1b || *p == 0x29)
        return 0;
    return 1;
}
/* 0x5764b0, a callback predicate: 0 when the step kind is 0x1a, the record p[1] names a building kind whose table
   byte (0x7e3f30, 0x24d bytes per kind) is 0x12 and the word 0x6f2310 is 0x74; else 1. 65 B, the next start
   0x576500 is not Ghidra's (its 1776 B record begins here) */

int ai_Unk5764b0(int *p)
{
    int kind;

    if (p[0] != 0x1a || !p[1])
        return 1;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0x12)
        return 1;
    if (*(unsigned short *)(unsigned)&sv_PanelShown != 0x74)
        return 1;
    return 0;
}
/* 0x576500, a callback predicate: the kinds 0x19 / 0x1a with a building whose table byte is 0xf; the kind 0x19
   matches when the step word p[3] is 0x10e, the kind 0x1a when the building word 0x6f2310 is 0x10e; a missing
   pointer or another kind answers p[0] != 0x1b. 0x576583 nop, the next start at 0x576590 */

int ai_Unk576500(int *p)
{
    int kind;

    if (p[0] == 0x19 && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xf)
            return 0;
        return p[3] == 0x10e;
    }
    if (p[0] == 0x1a && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xf)
            return 1;
        return *(unsigned short *)(unsigned)&sv_PanelShown != 0x10e;
    }
    return p[0] != 0x1b;
}
/* 0x576590, the kind 0x19 with a building whose table byte is 0xf and the step word p[3] 0x110 */

int ai_Unk576590(int *p)
{
    int kind;

    if (p[0] != 0x19 || !p[1])
        return 0;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xf)
        return 0;
    if (p[3] != 0x110)
        return 0;
    return 1;
}
/* 0x5765e0, 1 for the kind 0x1a with a record */

int ai_Unk5765e0(int *p)
{
    if (p[0] == 0x1a && p[1])
        return 1;
    return 0;
}
/* 0x576600, 1 unless the kind 0x19 has a building whose table byte is not 0xf */

int ai_Unk576600(int *p)
{
    int kind;

    if (p[0] != 0x19 || !p[1])
        return 1;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xf)
        return 0;
    return 1;
}
/* 0x576640, 1 for the kind 0x19 with a table byte 0xf or 2 (a missing record / kind answers 1) */

int ai_Unk576640(int *p)
{
    int kind;
    unsigned char c;

    if (p[0] != 0x19 || !p[1])
        return 1;
    kind = *(unsigned char *)p[1];
    c = *(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d);
    if (c == 0xf || c == 2)
        return 1;
    return 0;
}
/* 0x576680, the same as 0x576640 for the table bytes 0xf / 2 / 0x12 */

int ai_Unk576680(int *p)
{
    int kind;
    unsigned char c;

    if (p[0] != 0x19 || !p[1])
        return 1;
    kind = *(unsigned char *)p[1];
    c = *(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d);
    if (c == 0xf || c == 2 || c == 0x12)
        return 1;
    return 0;
}
/* 0x5766d0, a transition callback on the step arguments: 1 when the step applied a state change (the kind 0xa
   step p[2] 0 / 1 drives the states 4 <-> 9 / 0xa -> 0xb), 3 otherwise; the kind 7 with a record whose byte +2
   is 5 moves the pending state 9 to 0xa */

int ai_Unk5766d0(int *p)
{
    if (p[0] == 0xa) {
        if (p[2] == 0 && U_I(S + 4) == 4) {
            U_I(S) = 9;
            U_I(S + 4) = 9;
            U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
            return 1;
        }
        if (p[2] == 1) {
            if (U_I(S + 4) == 9) {
                U_I(S) = 4;
                U_I(S + 4) = 4;
                U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
                return 1;
            }
            if (U_I(S + 4) == 0xa) {
                U_I(S) = 0xb;
                U_I(S + 4) = 0xb;
                U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
                return 1;
            }
        }
        return 3;
    }
    if (p[0] == 7) {
        if (!p[1])
            return 3;
        if (U_I(S + 4) != 9)
            return 3;
        if (*(unsigned char *)(p[1] + 2) != 5)
            return 3;
        U_I(S) = 0xa;
        U_I(S + 4) = 0xa;
        U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
        return 1;
    }
    return 3;
}
/* 0x5767d0, the kind 0xd with a building whose table byte is 0xa moves the pending state to 0xb (1), else 3 */

int ai_Unk5767d0(int *p)
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

int ai_Unk576830(int *p)
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

void ai_Unk5768a0(int arg)
{
    char *r;

    if (arg)
        return;
    r = (char *)gm_BuildQuery(1, 5, 0xf);
    if (r)
        *(unsigned short *)(r + 0x29) = 0x10e;
}
/* 0x5768d0, the kind 0x19 with a record whose word is 0x10e moves the pending state to 0xb (1), else 3 */

int ai_Unk5768d0(int *p)
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

int ai_Unk576920(int *p)
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

int ai_Unk576970(int *p)
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

int ai_Unk5769c0(int *p)
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

void ai_Unk576a80(int arg)
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

int ai_Unk576ab0(int *p)
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

int ai_Unk576b50(int *p)
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

int ai_Unk576ba0(int *p)
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

int ai_Unk576cd0(int *p)
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

int ai_Unk576d40(int *p)
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

int ai_Unk576e70(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0x12
                && *(unsigned short *)(p[1] + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId
                && (p[3] == 0x79 || p[3] == 0x7a || p[3] == 0x78))
            return 1;
    }
    return ai_Unk5764b0(p);
}
/* 0x576ee0, the twin of 0x576ba0 for a building of the table byte 0x2: the kind 0x2b with the step word
   0x5 / 0x2 / 0xd / 0x8 and the flag +0x38 clear sets the flag and the state 0xa (3); the kind 0x10 sets the
   pending state 9 / 0xb / 4, applies it and answers 1 */

int ai_Unk576ee0(int *p)
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

int ai_Unk577020(int *p)
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

void ai_Unk5770a0(int arg)
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

int ai_Unk5770e0(int *p)
{
    if (p[0] == 0x20) {
        if (p[3] == 0x155 && U_I(S + 0x3c) == 3)
            return 1;
        return 0;
    }
    return ai_Unk5764b0(p);
}
/* 0x577110, when `arg` is 0: finds the character record 0x770b80 + i * 0x21c whose owner (the pointer at
   0x770cec + i * 0x21c, +0x21c per record) has the player word 0x6f25b8 at +0x27, asks 0x55ea60 for two values and
   posts both (0x4c51b0 with 0x5f1890, 0x4c5150 with 0x631090) on the record's id +4; no return value */

void ai_Unk577110(int arg)
{
    char *rec;
    int a, b;
    int p, i;

    if (arg)
        return;
    for (p = (unsigned)&ai_Unk770cec, i = 0; p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x16a); p += 0x21c, i++) {
        rec = *(char **)p;
        if (rec && *(unsigned short *)(rec + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId)
            break;
    }
    if (p >= ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x16a))
        return;
    rec = (char *)((unsigned)&ai_Chars + i * 0x21c);
    if (!he_Unk55ea60(rec, &a, &b))
        return;
    cm_RequestChrMoveToUniverse(*(int *)(rec + 4), a, b, (char *)(unsigned)"dummy_EINGANG");
    ai_Unk4c5150(*(int *)(rec + 4), a, b, (char *)(unsigned)&ch_EmptyName);
}
/* 0x5771b0, a transition callback on the flag S+0x38: it is raised to 1 by the kind 0x25 with the step word
   0x155, and to 2 by the kind 0x24 on a building with the table byte 0xa (1); else 0, and the answer of 0x5764b0
   when the flag is below the needed value or no record is given */

int ai_Unk5771b0(int *p)
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
    return ai_Unk5764b0(p);
}
/* 0x577230, when `arg` is 0: posts the message 0x2bc0 for the player's character id (0x770b80 + id * 0x21c, +4)
   and a fight message (0x4c4880) on the record of 0x500ec0 (0x155): (id +2, -1, 0x155, 3, 0x6f1bc5, 0) */

void ai_Unk577230(int arg)
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
/* 0x577290, 1 while the flag S+0x38 is not positive, and for the kind 0x29 with the step word 0x155 (which sets
   the flag to 1); else 0 */

int ai_Unk577290(int *p)
{
    if (U_I(S + 0x38) <= 0)
        return 1;
    if (p[0] == 0x29 && p[3] == 0x155) {
        U_I(S + 0x38) = 1;
        return 1;
    }
    return 0;
}
/* 0x5772d0, the fight kind of the record 0x6f2310: -1 without a record, for the kinds outside 0x92 .. 0x97 /
   0x2db, and while the flag S+0x38 is set; else 0 for the kind 0x92, 1 for the other kinds */

int ai_Unk5772d0(void)
{
    unsigned short w;
    int v;

    if (!U_I((unsigned)&sv_PanelShown))
        return -1;
    w = *(unsigned short *)U_I((unsigned)&sv_PanelShown);
    v = w;
    if (v < 0x92)
        return -1;
    if (v > 0x97 && v != 0x2db)
        return -1;
    if (U_I(S + 0x38))
        return -1;
    return w != 0x92;
}
/* 0x577320, the flag S+0x38 */

int ai_Unk577320(void)
{
    return U_I(S + 0x38);
}
/* 0x577330, a transition callback on the flag S+0x38: it is raised to 1 by the kind 0x28 (step word 0x155), to 2
   by the kind 0x27 (0x1d0) and to 3 by the kind 0x24 on the player's building (table byte 0x12, word +0x27 =
   0x6f25b8), each when the flag already reaches the previous level (1) and the step fits; the kinds 0x1a / 0x1c
   answer 1, everything else 0 */

int ai_Unk577330(int *p)
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
/* 0x5773f0, the fight kind of the record 0x6f2310 (0x92 .. 0x97) merged with the flag S+0x38: -1 without a
   record / outside the kinds; flag 0: 0 for 0x92, 3 else; flag 1: 1 for 0x95, 4 else; flag 2: 2 */

int ai_Unk5773f0(void)
{
    unsigned short w;
    int v;

    if (!U_I((unsigned)&sv_PanelShown))
        return -1;
    w = *(unsigned short *)U_I((unsigned)&sv_PanelShown);
    v = w;
    if (v < 0x92 || v > 0x97)
        return -1;
    if (U_I(S + 0x38) == 0)
        return w == 0x92 ? 0 : 3;
    if (U_I(S + 0x38) == 1)
        return w == 0x95 ? 1 : 4;
    if (U_I(S + 0x38) == 2)
        return 2;
    return -1;
}
/* 0x577450, a step predicate: 1 for the kind 0x26 with the step word 0x1d0 while the flag S+0x38 is not negative
   (which sets the flag to 1), and for the kinds 0x1a / 0x1c; else 0 */

int ai_Unk577450(int *p)
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

int ai_Unk577490(void)
{
    char *st;

    st = S;
    if (*(int *)(st + 0x40) == 2 && *(int *)(st + 0x50))
        sv_Unk52f9f0(*(int *)(st + 0x50));
    else if (*(int *)(st + 0x40) == 3 && *(int *)(st + 0x50))
        sv_Unk52fa00(*(int *)(st + 0x50));
    return 0;
}
/* 0x5774d0, when `arg` is 0 and the building 0x6f2308 is not already of the table byte 0x12: finds a record
   (0x501190 (1, 5, 0x12)), picks 0x4ffe90 (r+0x5d, 1, 0, 0x74) and passes it (0x521960 (r, q+2, 0x631090)) */

void ai_Unk5774d0(int arg)
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

void ai_Unk577540(int arg)
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
/* 0x5775c0, when `arg` is 0: two messages on the player's character id (0x770b80 + id * 0x21c, +4):
   0x4c4a60 (id, 0x1c8, 4, 0x4000, 0) and 0x4c4830 (id, -1, 0, 0x3e80) */

void ai_Unk5775c0(int arg)
{
    char *rec;
    int id;

    id = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    if (arg)
        return;
    cm_SendSetField(id, 0x1c8, 4, 0x4000, 0);
    cm_SendMoveGoodsUnchecked(id, -1, 0, 0x3e80);
}
/* 0x577610, when `arg` is 0: 0x4eaa00 (1); when it is 1: finds the character record 0x770b80 + i * 0x21c whose
   byte +2 is 5, asks 0x4e7150 (rec, 1, &v) and, when it answers > 0, calls 0x4e6180 (rec, v) */

void ai_Unk577610(int arg)
{
    char *rec;
    int v;
    int p, i;

    if (arg == 0) {
        amt_Unk4eaa00(1);
        return;
    }
    if (arg != 1)
        return;
    for (p = ((unsigned)&ai_Chars + 0x2), i = 0; p < (unsigned)((unsigned)ai_Chars + 0x65402); p += 0x21c, i++)
        if (*(unsigned char *)p == 5)
            break;
    if (p >= (unsigned)((unsigned)ai_Chars + 0x65402))
        return;
    rec = (char *)((unsigned)&ai_Chars + i * 0x21c);
    if (amt_GetOfficeCandidates(rec, 1, &v) > 0)
        amt_ApplyForOffice(rec, v);
}
/* 0x577690, when `arg` is 0 and 0x4e8160() answers non-zero: builds a 0x114-byte record (zeroed) out of the
   character records 0x770b80 + i * 0x21c (the kind byte +2: 6 -> its id to +0xc, 7 -> the ids +0x34, count +0x30,
   byte), copies the game time 0x770ae0 .. 0x770aec, sets the bytes +0x10 = 1 / +0x2e = 0xa0 / the dword +0x1c = -1
   and calls 0x4c4fa0 (record). ROUGH: the record's field offsets are the disassembly's; its type is unknown.
   Ends 0x57773f; the next start 0x577740 has no nop padding */

void ai_Unk577690(int arg)
{
    char rec[0x114];
    char *p;
    int i, n;

    if (arg)
        return;
    if (!amt_CheckOfficeColumns())
        return;
    memset(rec, 0, sizeof rec);
    n = 0;
    for (p = (char *)(unsigned)&ai_CharInts; p < (char *)((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2); p += 0x21c) {
        if (*(unsigned char *)(p - 2) == 6)
            *(int *)(rec + 0xc) = *(int *)p;
        else if (*(unsigned char *)(p - 2) == 7) {
            *(int *)(rec + 0x34 + n * 4) = *(int *)p;
            n++;
        }
    }
    rec[0x30] = (char)n;
    *(int *)(rec + 0x18) = U_I((unsigned)&ai_Time);
    *(int *)(rec + 0x1c) = -1;
    *(int *)(rec + 0x28) = U_I(((unsigned)&ai_Time + 0x8));
    *(unsigned short *)(rec + 0x2c) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
    rec[0x10] = 1;
    rec[0x2e] = (char)0xa0;
    ai_Unk4c4fa0(rec);
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
    d2_PlaceMainWindow(R8, 3);
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

int ai_Unk577850(void)
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
/* 0x5778b0, the run window's text tick: -4 without the run record / when 0x577850 fails, else the controls 1 / 2
   with the texts of the record +0xc / +0xc + 1 (or 0x3fd3 / 0x3fd4 in the states 0xa / 0xb of the modes 1 / 2)
   and, when the record word +0x10 is set, the window S+0x14 recreated by 0x4c2760 (0xfffffff9, 0, +0x10, -1)
   after releasing the previous one (0x426f20 / 0x427290); 0 */

int ai_Unk5778b0(void)
{
    char *st;
    char *r;

    if (!*(int *)(S + 0x18))
        return -4;
    if (ai_Unk577850() == -4)
        return -4;
    st = S;
    r = *(char **)(st + 0x18);
    if ((*(int *)(r + 8) == 1 || *(int *)(r + 8) == 2)
            && (*(int *)(st + 4) == 0xa || *(int *)(st + 4) == 0xb)) {
        d2_SetForm(R8, 1);
        io_PrintText(0x3fd3);
        d2_SetForm(R8, 2);
        io_PrintText(0x3fd4);
        return 0;
    }
    d2_SetForm(R8, 1);
    io_PrintText(*(int *)(*(char **)(S + 0x18) + 0xc));
    d2_SetForm(R8, 2);
    io_PrintText(*(int *)(*(char **)(S + 0x18) + 0xc) + 1);
    r = *(char **)(S + 0x18);
    if (*(int *)(r + 0x10)) {
        if (U_I(S + 0x14)) {
            if (sb_IsPlaying(U_I(S + 0x14)))
                sb_Stop(U_I(S + 0x14), 0);
            U_I(S + 0x14) = 0;
        }
        U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, *(int *)(r + 0x10), -1);
    }
    return 0;
}
/* 0x577a00, the tutorial's escape check: -4 without a player / run record; 0 while the scene 0x6f262c is not a
   record of the type byte 0x87, while the window / object is not the one in S+0x20 (or, with 0xf8a668 == -1, while
   the last key is not 0x1c); else the window S+0x14 is released (0x426f20 / 0x427290) and 1 answers */

int ai_Unk577a00(void)
{
    char *st;

    st = S;
    if (!*(int *)(st + 0x1c))
        return -4;
    if (!*(int *)(*(char **)(st + 0x1c) + 0x74))
        return -4;
    if (!U_I((unsigned)&he_ActiveSlot))
        return 0;
    if (!*(char **)U_I((unsigned)&he_ActiveSlot) || *(unsigned char *)*(char **)U_I((unsigned)&he_ActiveSlot) != 0x87)
        return 0;
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
    if (U_I((unsigned)&d2_ClickedId) != -1) {
        if (U_I((unsigned)&d2_PointerObj) != *(int *)(st + 0x20))
            return 0;
    } else {
        if (*(unsigned char *)(unsigned)&kb_Last != 0x1c)
            return 0;
    }
#ifdef REA_START
answered:
#endif
    if (U_I(st + 0x14)) {
        if (sb_IsPlaying(U_I(st + 0x14)))
            sb_Stop(U_I(st + 0x14), 0);
        U_I(st + 0x14) = 0;
    }
    return 1;
}
/* 0x577a90, no return value: nothing without the run record or its text +0x14; else the text window S+0x24 is
   rewritten (0x52e330, control 0 with the text 0x602568 and the record text +0x14, control 1 with 0x475590) and,
   when the record word +0x18 is set, the window S+0x14 recreated by 0x4c2760 (0xfffffff9, 0, +0x18, -1) after
   releasing the previous one (0x426f20 / 0x427290) */

void ai_Unk577a90(void)
{
    char *st;
    char *r;

    st = S;
    r = *(char **)(st + 0x18);
    if (!r)
        return;
    if (!*(int *)(r + 0x14))
        return;
    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    d2_SetForm(U_I(S + 0x24), 0);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(*(int *)(*(char **)(S + 0x18) + 0x14));
    d2_SetViewEnabled(U_I(S + 0x24), 1);
    st = S;
    if (!*(int *)(*(char **)(st + 0x18) + 0x18))
        return;
    if (U_I(st + 0x14)) {
        if (sb_IsPlaying(U_I(st + 0x14)))
            sb_Stop(U_I(st + 0x14), 0);
        U_I(st + 0x14) = 0;
    }
    r = *(char **)(S + 0x18);
    U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, *(int *)(r + 0x18), -1);
}
/* 0x577b60, clears the text window S+0x24: control 0 off (0x475f70), the text 0x602568 (0x515960), 0x475590
   (S+0x24, 0); no return value */

void ai_Unk577b60(void)
{
    d2_SetForm(U_I(S + 0x24), 0);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetViewEnabled(U_I(S + 0x24), 0);
}
/* 0x577b90, the sibling of 0x577a90 for the second text window S+0x34 (203 B): nothing without the run record
   or its second text +0x1c; else the window S+0x34 is rewritten (0x52e330, control 0 with the text 0x602568 and
   the record text +0x1c, 0x475590 control 1) and, when the record word +0x20 is set, the window S+0x14 recreated
   by 0x4c2760 (0xfffffff9, 0, +0x20, -1) after releasing the previous one (0x426f20 / 0x427290) */

void ai_Unk577b90(void)
{
    char *r;

    if (!*(int *)(S + 0x18))
        return;
    if (!*(int *)(*(char **)(S + 0x18) + 0x1c))
        return;
    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    d2_SetForm(U_I(S + 0x34), 0);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(*(int *)(*(char **)(S + 0x18) + 0x1c));
    d2_SetViewEnabled(U_I(S + 0x34), 1);
    if (!*(int *)(*(char **)(S + 0x18) + 0x20))
        return;
    if (U_I(S + 0x14)) {
        if (sb_IsPlaying(U_I(S + 0x14)))
            sb_Stop(U_I(S + 0x14), 0);
        U_I(S + 0x14) = 0;
    }
    r = *(char **)(S + 0x18);
    U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, *(int *)(r + 0x20), -1);
}
/* 0x577c60, clears the second text window S+0x34 (106 B): control 0 off (0x475f70), the text 0x602568
   (0x515960), 0x475590 (S+0x34, 0); then the window S+0x14 is released (0x426f20 / 0x427290) and cleared */

void ai_Unk577c60(void)
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

void ai_Unk577cd0(void)
{
    int i;

    U_I(S + 0x68) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629314);
    d2_PlaceMainWindow(U_I(S + 0x68), 3);
    d2_SetForm(U_I(S + 0x68), 0);
    i = U_I((unsigned)&d2_LastWindow);
    U_I(S + 0x6c) = d2_WinOpenSlider(*(short *)((unsigned)&ai_Unkf8f128 + i * 0x3c4) / 2 - 0xc8, 5, 0x190, 0, 0x190,
                                 0x75, 0x42, i);
}
/* 0x577d60, releases the tutorial progress form S+0x68 (36 B, a Ghidra-missed start after 0x577cd0's nop fill):
   nothing while the id is -1; else 0x4758f0 (id) and the id reset to -1 */

void ai_Unk577d60(void)
{
    if (U_I(S + 0x68) != -1) {
        d2_CloseView(U_I(S + 0x68));
        U_I(S + 0x68) = -1;
    }
}

#define TDIFF (U_I((unsigned)&ai_ClockMs) - *(int *)(st + 0xc))
#define MINU(a, b) ((unsigned)(a) < (unsigned)(b) ? (a) : (b))

/* 0x577d90, the tutorial progress slider (179 B, the state 0xa only): the state record, else 0x577d60 (the form
   released); the progress form created when S+0x68 is -1 (0x577cd0); the slider S+0x6c set by 0x475cb0 (id, 0,
   400, value) with value = (total - min(total, the clock 0x6333cc - S+0xc)) / total * 400.0f */
void ai_Unk577d90(void)
{
    char *st;
    char *r;
    int total;
    int t;

    st = S;
    r = *(char **)(st + 0x18);
    if (!r || U_I(st) != 0xa) {
        ai_Unk577d60();
        return;
    }
    if (U_I(st + 0x68) == -1) {
        ai_Unk577cd0();
        st = S;
    }
    total = *(int *)(*(char **)(st + 0x18) + 0x44);
    t = MINU(TDIFF ? TDIFF : 0, total);
    d2_SetObjValue(U_I(st + 0x6c), 0, 0x190, (int)((float)(total - t) / (float)total * 400.0f));
}
/* 0x577e50, the tutorial chapter form (632 B): r = S+0x18; -4 without it; the step S+0x40 against the step
   count r+0x50 (unsigned): done -> the callback r+0x54, the form S+0x48 released (0x4758f0), S+0x4c = -1,
   S+0x44 = 4, 0. Else the step record (r+0x58, 0x10 B each: +4 the chapter kind 0 .. 3, +8 the text, +0xc a
   window value): when the chapter S+0x44 is unchanged the form's controls 1 / 2 are cleared (0x475f70 +
   the empty text 0x602568); else the old form is released, the kind re-read (-4 when >= 4) and the new form
   created as 0x4741f0 (0, 0, kind 0 "tutorial\left_form" 0x629368, 1 "tutorial\right_form" 0x629354,
   2 "tutorial\top_form" 0x629340, 3 "tutorial\bottom_form" 0x629328), 0x475850 (id, 3), the chapter S+0x44
   set. Then control 1 = the step text, control 2 = 0x515960 (0x624204 "$Z%s", 0x7d) into 0x475b80 (id, 2,
   text) into S+0x4c, the window S+0x14 = 0x4c2760 (0xfffffff9, 0, the step's +0xc, -1) when that is set, the
   callback r+0x54, 0 */

int ai_Unk577e50(void)
{
    char *st;
    char *r;
    int step;
    int kind;
    int cb;
    int v;

    st = S;
    r = *(char **)(st + 0x18);
    if (!r)
        return -4;
    step = *(int *)(st + 0x40);
    if (step >= (unsigned int)*(int *)(r + 0x50)) {
        cb = *(int *)(r + 0x54);
        if (cb)
            ((void (*)())cb)();
        if (U_I(S + 0x48) != -1) {
            d2_CloseView(U_I(S + 0x48));
            U_I(S + 0x48) = -1;
        }
        U_I(S + 0x4c) = -1;
        U_I(S + 0x44) = 4;
        return 0;
    }
    kind = *(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 4);
    if (U_I(S + 0x44) == kind) {
        d2_SetForm(U_I(S + 0x48), 1);
        io_PrintText((char *)(unsigned)"$C");
        d2_SetForm(U_I(S + 0x48), 2);
        io_PrintText((char *)(unsigned)"$C");
    } else {
        if (U_I(S + 0x48) != -1) {
            d2_CloseView(U_I(S + 0x48));
            U_I(S + 0x48) = -1;
        }
        U_I(S + 0x4c) = -1;
        U_I(S + 0x44) = 4;
        kind = *(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 4);
        if (kind >= 4)
            return -4;
        switch (kind) {
        case 0: U_I(S + 0x48) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629368); break;
        case 1: U_I(S + 0x48) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629354); break;
        case 2: U_I(S + 0x48) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629340); break;
        case 3: U_I(S + 0x48) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629328); break;
        }
        d2_PlaceMainWindow(U_I(S + 0x48), 3);
        U_I(S + 0x44) = *(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 4);
    }
    d2_SetForm(U_I(S + 0x48), 1);
    io_PrintText(*(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 8));
    d2_SetForm(U_I(S + 0x48), 2);
    U_I(S + 0x4c) = d2_GetWinObj(U_I(S + 0x48), 2, io_PrintText((char *)(unsigned)"$Z%s", 0x7d));
    v = *(int *)(*(int *)(*(char **)(S + 0x18) + 0x58) + U_I(S + 0x40) * 0x10 + 0xc);
    if (v) {
        if (U_I(S + 0x14)) {
            if (sb_IsPlaying(U_I(S + 0x14)))
                sb_Stop(U_I(S + 0x14), 0);
            U_I(S + 0x14) = 0;
        }
        U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, v, -1);
    }
    cb = *(int *)(*(char **)(S + 0x18) + 0x54);
    if (cb)
        ((void (*)())cb)();
    return 0;
}
/* 0x5780e0, predicate (49 B): -4 without the gate 0x6f265c; 0 while the input is idle (0xf8a668 == -1) or the
   current window 0x5fe710 is not the control S+0x4c; 1 when it is (the 0x577a00 shape on S+0x4c) */

int ai_Unk5780e0(void)
{
    if (!U_I((unsigned)&full_data_6f265c))
        return -4;
    if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_PointerObj) == U_I(S + 0x4c))
        return 1;
    return 0;
}
/* 0x578120, sets the player S+0x1c = per and resets the window id S+0x20 = -1 (43 B): -4 without the gate
   0x6f265c, else 0 */

int ai_Unk578120(char *per)
{
    if (!U_I((unsigned)&full_data_6f265c))
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
    return !U_I((unsigned)&full_data_6f265c);
}

int ai_Unk5784a0();
int ai_Unk57b750();
void ai_Unk578370();
int sb_UnloadSampleBank();
int snd_LoadVoiceBank();
void * mem_Alloc(int size, char *tag);
void ai_Unk578440();

/* 0x578180, the tutorial start (493 B, called from 0x53bb20): -3 while the gate 0x6f265c is set; the table
   0x62918c (5 ints, counted to 0x6291a0) must hold at least one 1 (else -2); the record chain built:
   0x5784a0 () into 0x6f2658, its +0x70 tail; for every i whose 0x62918c[i] is 1 the callback 0x6291a0[i] ()
   appended at the tail (its own tail walked), then 0x57b750 (); failures go to 0x578370 () and -1. Then the
   state record 0x5eec60 is reset: +0 / +4 = 1, +8 / +0xc = the clock 0x6333cc, +0x10 / +0x14 = 0,
   +0x18 = 0x6f2658, +0x1c = 0, +0x20 = -1, the form +0x24 = 0x4741f0 (0, 0, "tutorial\inline_form"
   0x629390) (0x475590 (id, 0), 0x475850 (id, 3)), +0x28 / +0x2c / +0x30 = 0, the form +0x34 =
   0x4741f0 (0, 0, "tutorial\aux_form" 0x62937c) (0x475590 (id, 0), 0x475850 (id, 3)), +0x3c / +0x40 /
   +0x54 / +0x58 / +0x5c = 0, +0x60 / +0x68 / +0x6c = -1, the gates 0x6f265c / 0x6f2660 = 1, 0 */
int ai_Unk578180(void)
{
    char *p;
    char *rec;
    char *tail;
    char *r;
    int cnt;
    int i;

    if (U_I((unsigned)&full_data_6f265c) != 0)
        return -3;
    cnt = 0;
    for (p = (char *)(unsigned)&full_data_62918c; p < (char *)(unsigned)((unsigned)full_data_62918c + 0x14); p += 4) {
        if (U_I(p) == 1)
            cnt++;
    }
    if (!cnt)
        return -2;
    rec = (char *)ai_Unk5784a0();
    *(char **)(unsigned)&full_data_6f2658 = rec;
    if (!rec)
        goto fail;
    tail = rec;
    while (*(char **)(tail + 0x70))
        tail = *(char **)(tail + 0x70);
    for (i = 0; i < 0x14; i += 4) {
        if (U_I((unsigned)&full_data_62918c + i) == 1) {
            r = (char *)((int (*)())U_I((unsigned)&full_data_6291a0 + i))();
            *(char **)(tail + 0x70) = r;
            if (!r)
                goto fail;
            tail = r;
            while (*(char **)(tail + 0x70))
                tail = *(char **)(tail + 0x70);
        }
    }
    r = (char *)ai_Unk57b750();
    *(char **)(tail + 0x70) = r;
    if (!r)
        goto fail;
    U_I(S) = 1;
    U_I(S + 4) = 1;
    U_I(S + 8) = U_I((unsigned)&ai_ClockMs);
    U_I(S + 0xc) = U_I((unsigned)&ai_ClockMs);
    U_I(S + 0x10) = 0;
    U_I(S + 0x14) = 0;
    *(char **)(S + 0x18) = *(char **)(unsigned)&full_data_6f2658;
    U_I(S + 0x1c) = 0;
    U_I(S + 0x20) = -1;
    U_I(S + 0x24) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629390);
    d2_SetViewEnabled(U_I(S + 0x24), 0);
    d2_PlaceMainWindow(U_I(S + 0x24), 3);
    U_I(S + 0x28) = 0;
    U_I(S + 0x2c) = 0;
    U_I(S + 0x30) = 0;
    U_I(S + 0x34) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62937c);
    d2_SetViewEnabled(U_I(S + 0x34), 0);
    d2_PlaceMainWindow(U_I(S + 0x34), 3);
    U_I(S + 0x3c) = 0;
    U_I(S + 0x40) = 0;
    U_I(S + 0x54) = 0;
    U_I(S + 0x58) = 0;
    U_I(S + 0x5c) = 0;
    U_I(S + 0x60) = -1;
    U_I(S + 0x68) = -1;
    U_I(S + 0x6c) = -1;
    U_I((unsigned)&full_data_6f265c) = 1;
    U_I((unsigned)&full_data_6f2660) = 1;
    return 0;
fail:
    ai_Unk578370();
    return -1;
}
/* 0x578370, the tutorial stop (196 B): the six handles 0x6f2640 .. 0x6f2658 unloaded when set (0x426ba0), the
   record chain 0x6f2658 freed by 0x578440 and cleared; the state record 0x5eec60 reset (+0, +8, +0xc, +0x10,
   +0x14, +0x18, +0x1c = 0, +0x20 = -1, the forms +0x24 / +0x34 released (0x4758f0) and -1, +0x14 = 0 again),
   the gates 0x6f265c / 0x6f2660 = 0; no return value */

void ai_Unk578370(void)
{
    int i;

    for (i = (unsigned)&full_data_6f2640; i < (int)((unsigned)&full_data_6f2640 + 0x18); i += 4) {
        if (U_I(i))
            sb_UnloadSampleBank(U_I(i));
    }
    ai_Unk578440(U_I((unsigned)&full_data_6f2658));
    U_I((unsigned)&full_data_6f2658) = 0;
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
    U_I((unsigned)&full_data_6f265c) = 0;
    U_I((unsigned)&full_data_6f2660) = 0;
}
/* 0x578440, frees the tutorial record chain (82 B, recursive): nothing for a null record; else its +0x70 next
   record, its +0x58 and +0x64 tables (mem_Free, both cleared) and then the record itself */

void ai_Unk578440(char *rec)
{
    if (!rec)
        return;
    if (*(char **)(rec + 0x70))
        ai_Unk578440(*(char **)(rec + 0x70));
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
/* 0x5784a0, allocates the tutorial's main-intro record (204 B): mem_Alloc (0x74, "FTInitMainIntro" 0x6293d8),
   0 while it fails (0x578440 (0)); the record zeroed (memset 0x74) and its fields set: +0 = 0, +4 = the magic
   "RTNI" 0x494e5452, +8 = 4, +0xc = 0x3f24, +0x10 = the text key "TUTORIAL_MAIN_INTRO" 0x6293a8, +0x14 ..
   +0x20 = 0, +0x2c = -1, +0x30 = 0x8e, +0x34 / +0x38 = 0, +0x40 / +0x44 = 0, +0x48 = 0xb, +0x68 / +0x6c /
   +0x70 = 0; the sample bank handle 0x6f2640 = 0x4c2830 ("TUTORIAL_INTRO_OUTRO.sbf" 0x6293bc, 0); the record */

int ai_Unk5784a0(void)
{
    char *rec;

    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6293d8);
    if (!rec) {
        ai_Unk578440(0);
        return 0;
    }
    memset(rec, 0, 0x74);
    U_I(rec) = 0;
    U_I(rec + 4) = 0x494e5452;
    U_I(rec + 8) = 4;
    U_I(rec + 0xc) = 0x3f24;
    U_I(rec + 0x10) = (unsigned)&full_data_6293a8;
    U_I(rec + 0x14) = 0;
    U_I(rec + 0x18) = 0;
    U_I(rec + 0x1c) = 0;
    U_I(rec + 0x20) = 0;
    U_I(rec + 0x2c) = -1;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x34) = 0;
    U_I(rec + 0x38) = 0;
    U_I(rec + 0x40) = 0;
    U_I(rec + 0x44) = 0;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0;
    U_I(rec + 0x6c) = 0;
    U_I(rec + 0x70) = 0;
    U_I((unsigned)&full_data_6f2640) = snd_LoadVoiceBank((char *)(unsigned)&full_data_6293bc, 0);
    return (int)rec;
}
/* 0x578570, the tutorial chapter 1 chain (2254 B, ROUGH from the disassembly): eleven 0x74-byte step records,
   each mem_Alloc (0x74, name) + memset 0x74 with the fields +0 = 1, +4 the step magic ("1rtn" 0x6e747231,
   then 0x63317341 / 0x63317343 .. 0x6331734a, 0x75747231), +0xc the text id (0x3f2a .. 0x3f51), +0x10 the text
   key (CHAPTER_1_INTRO, CHAPTER_1_A_MAIN .. CHAPTER_1_J_MAIN, CHAPTER_1_OUTRO), +0x2c = -1, +0x30 (0x8e / 0x163
   / 0), +0x3c = 0x576490 (the step predicate), +0x48 (0xb / 0xa / 9 / 0), +0x68 (0, 1, 3, 7, 0x17 .. 0x1837), the
   records linked through +0x70 (the last +8 = 3); then the sample bank "TUTORIAL_CHAPTER_1.sbf" 0x6293f8 into
   0x6f2644 (0x4c2830 (name, 0)); the first record, or 0x578440 (the chain) and 0 after a failed alloc */

int ai_Unk578570(void)
{
    char *rec;
    char *prev;
    char *r;

    /* FTInitChapter1Intro */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629710);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x1;
    U_I(rec + 0x4) = 0x6e747231;
    U_I(rec + 0xc) = 0x3f2a;
    U_I(rec + 0x10) = (unsigned)&full_data_6296ec;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(rec + 0x48) = 0xb;
    prev = rec;

    /* FTInitChapter1Step1 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6296fc);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317341;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f2c;
    U_I(r + 0x10) = (unsigned)&full_data_6296c4;
    U_I(r + 0x14) = 0x3f2e;
    U_I(r + 0x18) = (unsigned)&full_data_6296b0;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x16;
    U_I(r + 0x48) = 0xb;
    prev = r;

    /* FTInitChapter1Step3 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6296d8);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317343;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f30;
    U_I(r + 0x10) = (unsigned)&full_data_629674;
    U_I(r + 0x14) = 0x3f33;
    U_I(r + 0x18) = (unsigned)&full_data_629660;
    U_I(r + 0x1c) = 0x3f32;
    U_I(r + 0x20) = (unsigned)&full_data_629650;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x6;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x68) = 0x1;
    prev = r;

    /* FTInitChapter1Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629688);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317344;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f35;
    U_I(r + 0x10) = (unsigned)&full_data_629614;
    U_I(r + 0x14) = 0x3f38;
    U_I(r + 0x18) = (unsigned)&full_data_629600;
    U_I(r + 0x1c) = 0x3f37;
    U_I(r + 0x20) = (unsigned)&full_data_6295f0;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x8;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x68) = 0x3;
    prev = r;

    /* FTInitChapter1Step5 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629628);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317345;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f3a;
    U_I(r + 0x10) = (unsigned)&full_data_6295b4;
    U_I(r + 0x14) = 0x3f3d;
    U_I(r + 0x18) = (unsigned)&full_data_6295a0;
    U_I(r + 0x1c) = 0x3f3c;
    U_I(r + 0x20) = (unsigned)&full_data_629590;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk5766d0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x1;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x68) = 0x7;
    prev = r;

    /* FTInitChapter1Step6 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6295c8);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317346;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f3f;
    U_I(r + 0x10) = (unsigned)&full_data_629554;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x3;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x7;
    prev = r;

    /* FTInitChapter1Step7 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629568);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317347;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f41;
    U_I(r + 0x10) = (unsigned)&full_data_62952c;
    U_I(r + 0x14) = 0x3f43;
    U_I(r + 0x18) = (unsigned)&full_data_629518;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0xc;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x17;
    prev = r;

    /* FTInitChapter1Step8 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629540);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317348;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f45;
    U_I(r + 0x10) = (unsigned)&full_data_6294dc;
    U_I(r + 0x14) = 0x3f47;
    U_I(r + 0x18) = (unsigned)&full_data_6294c8;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x13;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x817;
    prev = r;

    /* FTInitChapter1Step9 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6294f0);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317349;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f49;
    U_I(r + 0x10) = (unsigned)&full_data_629488;
    U_I(r + 0x14) = 0x3f4b;
    U_I(r + 0x18) = (unsigned)&full_data_629474;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x14;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x1817;
    prev = r;

    /* FTInitChapter1Step10 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62949c);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x6331734a;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f4d;
    U_I(r + 0x10) = (unsigned)&full_data_629438;
    U_I(r + 0x14) = 0x3f4f;
    U_I(r + 0x18) = (unsigned)&full_data_629424;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x38) = (unsigned)&ai_Unk5767d0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0xd;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x1837;
    prev = r;

    /* FTInitChapter1Outro */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62944c);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x75747231;
    U_I(r + 0x8) = 0x3;
    U_I(r + 0xc) = 0x3f51;
    U_I(r + 0x10) = (unsigned)&full_data_6293e8;
    U_I(r + 0x2c) = 0xffffffff;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x1837;
    prev = r;
    U_I((unsigned)&full_data_6f2644) = snd_LoadVoiceBank((char *)(unsigned)&full_data_6293f8, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}
/* 0x578e40, the tutorial chapter 2 chain (1839 B, ROUGH from the disassembly, the same shape as 0x578570): nine
   0x74-byte step records (mem_Alloc (0x74, name) + memset 0x74): +0 = 2, +4 the magic ("2rtn" 0x6e747232, then
   0x63327341 .. 0x63327347, the outro 0x75747232), +0xc the text id (0x3f53 .. 0x3f70), +0x10 the text key
   (CHAPTER_2_INTRO, CHAPTER_2_A_MAIN .. CHAPTER_2_F_MAIN, CHAPTER_2_OUTRO), +0x2c = -1, +0x30 (0x8e / 0x163),
   +0x3c = 0x576490 (the step predicate; absent on the outro), +0x48 (0xb / 9 / 0xa), +0x68 (0x1837 / 0x5837),
   the last +0x6c = 0x1e00, the records linked through +0x70; the sample bank "TUTORIAL_CHAPTER_2.sbf" 0x629734
   into 0x6f2648 (0x4c2830 (name, 0)); the first record, or 0x578440 (the chain) and 0 after a failed alloc */

int ai_Unk578e40(void)
{
    char *rec;
    char *prev;
    char *r;


    /* FTInitChapter2Intro */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62998c);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x2;
    U_I(rec + 0x4) = 0x6e747232;
    U_I(rec + 0xc) = 0x3f53;
    U_I(rec + 0x10) = (unsigned)&full_data_629968;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0x1837;
    prev = rec;

    /* FTInitChapter2Step1 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629978);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327341;
    U_I(r + 0xc) = 0x3f55;
    U_I(r + 0x10) = (unsigned)&full_data_629940;
    U_I(r + 0x14) = 0x3f57;
    U_I(r + 0x18) = (unsigned)&full_data_62992c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk576830;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x1837;
    prev = r;

    /* FTInitChapter2Step2 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629954);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327342;
    U_I(r + 0xc) = 0x3f59;
    U_I(r + 0x10) = (unsigned)&full_data_6298f0;
    U_I(r + 0x14) = 0x3f5b;
    U_I(r + 0x18) = (unsigned)&full_data_6298dc;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x34) = (unsigned)&ai_Unk5768a0;
    U_I(r + 0x38) = (unsigned)&ai_Unk5768d0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576500;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    prev = r;

    /* FTInitChapter2Step3 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629904);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327343;
    U_I(r + 0xc) = 0x3f5d;
    U_I(r + 0x10) = (unsigned)&full_data_6298a0;
    U_I(r + 0x14) = 0x3f5f;
    U_I(r + 0x18) = (unsigned)&full_data_62988c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk576920;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576590;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0xe00;
    prev = r;

    /* FTInitChapter2Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6298b4);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327344;
    U_I(r + 0xc) = 0x3f61;
    U_I(r + 0x10) = (unsigned)&full_data_629850;
    U_I(r + 0x14) = 0x3f63;
    U_I(r + 0x18) = (unsigned)&full_data_62983c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk576970;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5765e0;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;

    /* FTInitChapter2Step5 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629864);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327345;
    U_I(r + 0xc) = 0x3f65;
    U_I(r + 0x10) = (unsigned)&full_data_629800;
    U_I(r + 0x14) = 0x3f67;
    U_I(r + 0x18) = (unsigned)&full_data_6297ec;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk576a80;
    U_I(r + 0x38) = (unsigned)&ai_Unk5769c0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576600;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;

    /* FTInitChapter2Step6 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629814);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327346;
    U_I(r + 0xc) = 0x3f69;
    U_I(r + 0x10) = (unsigned)&full_data_6297b0;
    U_I(r + 0x14) = 0x3f6b;
    U_I(r + 0x18) = (unsigned)&full_data_62979c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk576a80;
    U_I(r + 0x38) = (unsigned)&ai_Unk576ab0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576640;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;

    /* FTInitChapter2Step7 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6297c4);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327347;
    U_I(r + 0xc) = 0x3f6d;
    U_I(r + 0x10) = (unsigned)&full_data_629760;
    U_I(r + 0x14) = 0x3f6f;
    U_I(r + 0x18) = (unsigned)&full_data_62974c;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk576b50;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576680;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;

    /* FTInitChapter2Outro */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629774);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x75747232;
    U_I(r + 0x8) = 0x3;
    U_I(r + 0xc) = 0x3f70;
    U_I(r + 0x10) = (unsigned)&full_data_629724;
    U_I(r + 0x2c) = 0xffffffff;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;
    U_I((unsigned)&full_data_6f2648) = snd_LoadVoiceBank((char *)(unsigned)&full_data_629734, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}

/* the sub tables of a chapter step record: the step table at +0x58 (0x10 B per step) and the animation table
   at +0x64 (0x2c B per entry); both are reloaded through the record for every store, as the original does */
#define CHSTEP(o) U_I((char *)U_I(r + 0x58) + (o))
#define CHANIM(o) U_I((char *)U_I(r + 0x64) + (o))

/* 0x579570, the tutorial chapter 3 chain (5377 B, ROUGH from the disassembly, the same record shape as 0x578570
   / 0x578e40 plus the two sub tables of a step record: +0x50 the step count and +0x58 the step table (0x10 B
   per step: +0 the voice argument 0x2c6, +4 the chapter form kind, +8 the text id, +0xc a window / voice
   value), +0x5c the animation entry count, +0x60 the animation step callback and +0x64 the animation table
   (0x2c B per entry: +0 / +4 the position, +8 / +0xc / +0x10 / +0x14 / +0x18 / +0x1c the Bezier points and
   +0x20 / +0x24 / +0x28 the timings, read by 0x57b8a0). Ten 0x74-byte step records (mem_Alloc (0x74, name) +
   memset 0x74): +0 = 3, +4 the magic "3rtn" 0x6e747233 / 0x633373xx / 0x75747233, +0xc the text id, +0x10 the
   text key (CHAPTER_3_*), +0x2c = -1, +0x30, +0x3c, +0x48, +0x68, linked through +0x70. Every sub table has
   its own mem_Alloc and a failed alloc tears the whole chain down too. The screen width d2_ScreenW picks the
   animation floats (0x320 / 0x400 / else). The sample bank "TUTORIAL_CHAPTER_3.sbf" 0x6299b0 into 0x6f264c
   (0x4c2830 (name, 0)); the first record, or 0x578440 (the chain) and 0 after a failed alloc */
int ai_Unk579570(void)
{
    char *rec;
    char *prev;
    char *r;

    /* FTInitChapter2Intro (the original's copy-paste name for the chapter 3 intro) */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62998c);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x3;
    U_I(rec + 0x4) = 0x6e747233;
    U_I(rec + 0xc) = 0x3f72;
    U_I(rec + 0x10) = (unsigned)&full_data_629e1c;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x34) = (unsigned)&ai_Unk5774d0;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0x5837;
    U_I(rec + 0x6c) = 0x1e00;
    prev = rec;

    /* FTInitChapter3Step1 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629e2c);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337331;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f74;
    U_I(r + 0x10) = (unsigned)&full_data_629dec;
    U_I(r + 0x14) = 0x3f7a;
    U_I(r + 0x18) = (unsigned)&full_data_629dd8;
    U_I(r + 0x24) = 0x3f79;
    U_I(r + 0x28) = (unsigned)(full_data_629db0 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5764b0;
    U_I(r + 0x40) = 0x1d;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x50) = 0x3;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e10;
    /* FTInitScriptChapter3Step1: three steps */
    U_I(r + 0x58) = (unsigned)mem_Alloc(0x30, (char *)(unsigned)&full_data_629e00);
    if (!U_I(r + 0x58))
        goto fail;
    CHSTEP(0x0) = 0x2c6;
    CHSTEP(0x4) = 0x1;
    CHSTEP(0x8) = 0x3f76;
    CHSTEP(0xc) = (unsigned)&full_data_629d98;
    CHSTEP(0x10) = 0x2c6;
    CHSTEP(0x14) = 0x0;
    CHSTEP(0x18) = 0x3f77;
    CHSTEP(0x1c) = (unsigned)&full_data_629d80;
    CHSTEP(0x20) = 0x2c6;
    CHSTEP(0x24) = 0x2;
    CHSTEP(0x28) = 0x3f78;
    CHSTEP(0x2c) = (unsigned)&full_data_629d68;
    prev = r;

    /* FTInitChapter3Step2 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629db0);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337332;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f7b;
    U_I(r + 0x10) = (unsigned)&full_data_629d38;
    U_I(r + 0x14) = 0x3f80;
    U_I(r + 0x18) = (unsigned)&full_data_629d24;
    U_I(r + 0x1c) = 0x3f7f;
    U_I(r + 0x20) = (unsigned)&full_data_629d0c;
    U_I(r + 0x24) = 0x3f81;
    U_I(r + 0x28) = (unsigned)(full_data_629ce0 + 0x18);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x34) = (unsigned)&ai_Unk577110;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5770e0;
    U_I(r + 0x40) = 0x1f;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x20;
    U_I(r + 0x50) = 0x2;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x1;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e30;
    /* FTInitScriptChapter3Step2: two steps */
    U_I(r + 0x58) = (unsigned)mem_Alloc(0x20, (char *)(unsigned)&full_data_629d4c);
    if (!U_I(r + 0x58))
        goto fail;
    CHSTEP(0x0) = 0x2c6;
    CHSTEP(0x4) = 0x1;
    CHSTEP(0x8) = 0x3f7d;
    CHSTEP(0xc) = (unsigned)&full_data_629cc8;
    CHSTEP(0x10) = 0x2c6;
    CHSTEP(0x14) = 0x0;
    CHSTEP(0x18) = 0x3f7e;
    CHSTEP(0x1c) = (unsigned)&full_data_629cb0;
    /* FTInitAnimChapter3Step2: one animation entry */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0x2c, (char *)(unsigned)&full_data_629ce0);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x0) = 0x442dc000;
        CHANIM(0x4) = 0x43960000;
        CHANIM(0x8) = 0x441c8000;
        CHANIM(0xc) = 0x43c80000;
        CHANIM(0x10) = 0x43b70000;
        CHANIM(0x14) = 0x43be0000;
        CHANIM(0x18) = 0x43bc0000;
        CHANIM(0x1c) = 0x43908000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x0) = 0x44660000;
        CHANIM(0x4) = 0x43960000;
        CHANIM(0x8) = 0x44538000;
        CHANIM(0xc) = 0x43dc0000;
        CHANIM(0x10) = 0x44010000;
        CHANIM(0x14) = 0x43e10000;
        CHANIM(0x18) = 0x43f00000;
        CHANIM(0x1c) = 0x43bb8000;
    } else {
        CHANIM(0x0) = 0x44804000;
        CHANIM(0x4) = 0x43a00000;
        CHANIM(0x8) = 0x44768000;
        CHANIM(0xc) = 0x43dc0000;
        CHANIM(0x10) = 0x44240000;
        CHANIM(0x14) = 0x44160000;
        CHANIM(0x18) = 0x44038000;
        CHANIM(0x1c) = 0x43dc0000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x430e0000;
    CHANIM(0x28) = 0x428e0000;
    prev = r;

    /* FTInitChapter3Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629c9c);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337334;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f82;
    U_I(r + 0x10) = (unsigned)&full_data_629c6c;
    U_I(r + 0x14) = 0x3f88;
    U_I(r + 0x18) = (unsigned)&full_data_629c58;
    U_I(r + 0x1c) = 0x3f87;
    U_I(r + 0x20) = (unsigned)&full_data_629c48;
    U_I(r + 0x24) = 0x3f89;
    U_I(r + 0x28) = (unsigned)(full_data_629c20 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5764b0;
    U_I(r + 0x40) = 0x21;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x4c) = 0x22;
    U_I(r + 0x50) = 0x3;
    U_I(r + 0x54) = (unsigned)&ai_Unk577490;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e70;
    /* FTInitScriptChapter3Step4: three steps */
    U_I(r + 0x58) = (unsigned)mem_Alloc(0x30, (char *)(unsigned)&full_data_629c80);
    if (!U_I(r + 0x58))
        goto fail;
    CHSTEP(0x0) = 0x2c6;
    CHSTEP(0x4) = 0x1;
    CHSTEP(0x8) = 0x3f84;
    CHSTEP(0xc) = (unsigned)&full_data_629c08;
    CHSTEP(0x10) = 0x2c6;
    CHSTEP(0x14) = 0x0;
    CHSTEP(0x18) = 0x3f85;
    CHSTEP(0x1c) = (unsigned)&full_data_629bf0;
    CHSTEP(0x20) = 0x2c6;
    CHSTEP(0x24) = 0x0;
    CHSTEP(0x28) = 0x3f86;
    CHSTEP(0x2c) = (unsigned)&full_data_629bd4;
    prev = r;

    /* FTInitChapter3Step6 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629c20);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337336;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f8a;
    U_I(r + 0x10) = (unsigned)&full_data_629ba4;
    U_I(r + 0x14) = 0x3f90;
    U_I(r + 0x18) = (unsigned)&full_data_629b90;
    U_I(r + 0x1c) = 0x3f8f;
    U_I(r + 0x20) = (unsigned)&full_data_629b78;
    U_I(r + 0x24) = 0x3f91;
    U_I(r + 0x28) = (unsigned)(full_data_629b4c + 0x18);
    U_I(r + 0x2c) = 0x58c;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5771b0;
    U_I(r + 0x40) = 0x23;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x24;
    U_I(r + 0x50) = 0x3;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x2;
    U_I(r + 0x60) = (unsigned)&ai_Unk577320;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    /* FTInitScriptChapter3Step6: three steps */
    U_I(r + 0x58) = (unsigned)mem_Alloc(0x30, (char *)(unsigned)&full_data_629bb8);
    if (!U_I(r + 0x58))
        goto fail;
    CHSTEP(0x0) = 0x2c6;
    CHSTEP(0x4) = 0x1;
    CHSTEP(0x8) = 0x3f8c;
    CHSTEP(0xc) = (unsigned)&full_data_629b34;
    CHSTEP(0x10) = 0x2c6;
    CHSTEP(0x14) = 0x0;
    CHSTEP(0x18) = 0x3f8d;
    CHSTEP(0x1c) = (unsigned)&full_data_629b1c;
    CHSTEP(0x20) = 0x2c6;
    CHSTEP(0x24) = 0x0;
    CHSTEP(0x28) = 0x3f8e;
    CHSTEP(0x2c) = (unsigned)&full_data_629b04;
    /* FTInitAnimChapter3Step6: two animation entries */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0x58, (char *)(unsigned)&full_data_629b4c);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x0) = 0x43080000;
        CHANIM(0x4) = 0x43d70000;
        CHANIM(0x8) = 0x43a80000;
        CHANIM(0xc) = 0x43e10000;
        CHANIM(0x10) = 0x43e90000;
        CHANIM(0x14) = 0x43870000;
        CHANIM(0x18) = 0x43e88000;
        CHANIM(0x1c) = 0x43430000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x0) = 0x437a0000;
        CHANIM(0x4) = 0x4400c000;
        CHANIM(0x8) = 0x43e90000;
        CHANIM(0xc) = 0x44070000;
        CHANIM(0x10) = 0x440d8000;
        CHANIM(0x14) = 0x43be0000;
        CHANIM(0x18) = 0x440fc000;
        CHANIM(0x1c) = 0x43898000;
    } else {
        CHANIM(0x0) = 0x43990000;
        CHANIM(0x4) = 0x44110000;
        CHANIM(0x8) = 0x43e40000;
        CHANIM(0xc) = 0x44160000;
        CHANIM(0x10) = 0x441a0000;
        CHANIM(0x14) = 0x44020000;
        CHANIM(0x18) = 0x441c8000;
        CHANIM(0x1c) = 0x43af0000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x430e0000;
    CHANIM(0x28) = 0x428e0000;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x2c) = 0x43e88000;
        CHANIM(0x30) = 0x43430000;
        CHANIM(0x34) = 0x438f0000;
        CHANIM(0x38) = 0x43910000;
        CHANIM(0x3c) = 0x43cb0000;
        CHANIM(0x40) = 0x44048000;
        CHANIM(0x44) = 0x42e60000;
        CHANIM(0x48) = 0x43ff0000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x2c) = 0x440fc000;
        CHANIM(0x30) = 0x43898000;
        CHANIM(0x34) = 0x43da0000;
        CHANIM(0x38) = 0x43aa0000;
        CHANIM(0x3c) = 0x44060000;
        CHANIM(0x40) = 0x441b0000;
        CHANIM(0x44) = 0x43620000;
        CHANIM(0x48) = 0x44148000;
    } else {
        CHANIM(0x2c) = 0x441c8000;
        CHANIM(0x30) = 0x43af0000;
        CHANIM(0x34) = 0x43ee0000;
        CHANIM(0x38) = 0x43c30000;
        CHANIM(0x3c) = 0x44150000;
        CHANIM(0x40) = 0x44250000;
        CHANIM(0x44) = 0x438f0000;
        CHANIM(0x48) = 0x44228000;
    }
    CHANIM(0x4c) = 0x420c0000;
    CHANIM(0x50) = 0x430e0000;
    CHANIM(0x54) = 0x428e0000;
    prev = r;

    /* FTInitChapter3Step7 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629af0);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337337;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f92;
    U_I(r + 0x10) = (unsigned)&full_data_629ac8;
    U_I(r + 0x2c) = 0x58c;
    U_I(r + 0x30) = 0x47;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5764b0;
    U_I(r + 0x40) = 0x3;
    U_I(r + 0x44) = 0x1aa;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    prev = r;

    /* FTInitChapter3Step8 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629adc);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337338;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f94;
    U_I(r + 0x10) = (unsigned)&full_data_629a9c;
    U_I(r + 0x14) = 0x3f97;
    U_I(r + 0x18) = (unsigned)&full_data_629a88;
    U_I(r + 0x1c) = 0x3f96;
    U_I(r + 0x20) = (unsigned)&full_data_629a70;
    U_I(r + 0x24) = 0x3f98;
    U_I(r + 0x28) = (unsigned)(full_data_629a48 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x3c) = (unsigned)&ai_Unk577330;
    U_I(r + 0x40) = 0x17;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x24;
    U_I(r + 0x50) = 0x0;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x5;
    U_I(r + 0x60) = (unsigned)&ai_Unk5773f0;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    /* FTInitAnimChapter3Step8: five animation entries */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0xdc, (char *)(unsigned)&full_data_629ab0);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x0) = 0x43e88000;
        CHANIM(0x4) = 0x43430000;
        CHANIM(0x8) = 0x44060000;
        CHANIM(0xc) = 0x43dc0000;
        CHANIM(0x10) = 0x43b20000;
        CHANIM(0x14) = 0x43d20000;
        CHANIM(0x18) = 0x430c0000;
        CHANIM(0x1c) = 0x432a0000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x0) = 0x440fc000;
        CHANIM(0x4) = 0x43898000;
        CHANIM(0x8) = 0x43f30000;
        CHANIM(0xc) = 0x43c30000;
        CHANIM(0x10) = 0x43f80000;
        CHANIM(0x14) = 0x440c0000;
        CHANIM(0x18) = 0x43750000;
        CHANIM(0x1c) = 0x437f0000;
    } else {
        CHANIM(0x0) = 0x441c8000;
        CHANIM(0x4) = 0x43af0000;
        CHANIM(0x8) = 0x43cb0000;
        CHANIM(0xc) = 0x43910000;
        CHANIM(0x10) = 0x43300000;
        CHANIM(0x14) = 0x43910000;
        CHANIM(0x18) = 0x42fc0000;
        CHANIM(0x1c) = 0x43340000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x430e0000;
    CHANIM(0x28) = 0x428e0000;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x2c) = 0x42960000;
        CHANIM(0x30) = 0x432a0000;
        CHANIM(0x34) = 0x434e0000;
        CHANIM(0x38) = 0x43960000;
        CHANIM(0x3c) = 0x43d50000;
        CHANIM(0x40) = 0x43820000;
        CHANIM(0x44) = 0x43e88000;
        CHANIM(0x48) = 0x43430000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x2c) = 0x43340000;
        CHANIM(0x30) = 0x437f0000;
        CHANIM(0x34) = 0x43620000;
        CHANIM(0x38) = 0x43c30000;
        CHANIM(0x3c) = 0x440d8000;
        CHANIM(0x40) = 0x43be0000;
        CHANIM(0x44) = 0x440fc000;
        CHANIM(0x48) = 0x43898000;
    } else {
        CHANIM(0x2c) = 0x43620000;
        CHANIM(0x30) = 0x43a00000;
        CHANIM(0x34) = 0x438a0000;
        CHANIM(0x38) = 0x43cd0000;
        CHANIM(0x3c) = 0x441c8000;
        CHANIM(0x40) = 0x43e10000;
        CHANIM(0x44) = 0x441c8000;
        CHANIM(0x48) = 0x43aa0000;
    }
    CHANIM(0x4c) = 0x420c0000;
    CHANIM(0x50) = 0x430e0000;
    CHANIM(0x54) = 0x428e0000;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x58) = 0x43e88000;
        CHANIM(0x5c) = 0x43430000;
        CHANIM(0x60) = 0x438f0000;
        CHANIM(0x64) = 0x43910000;
        CHANIM(0x68) = 0x43cb0000;
        CHANIM(0x6c) = 0x44048000;
        CHANIM(0x70) = 0x42e60000;
        CHANIM(0x74) = 0x43ff0000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x58) = 0x440fc000;
        CHANIM(0x5c) = 0x43898000;
        CHANIM(0x60) = 0x43da0000;
        CHANIM(0x64) = 0x43aa0000;
        CHANIM(0x68) = 0x44060000;
        CHANIM(0x6c) = 0x441b0000;
        CHANIM(0x70) = 0x43620000;
        CHANIM(0x74) = 0x44148000;
    } else {
        CHANIM(0x58) = 0x441c8000;
        CHANIM(0x5c) = 0x43af0000;
        CHANIM(0x60) = 0x43ee0000;
        CHANIM(0x64) = 0x43c30000;
        CHANIM(0x68) = 0x44150000;
        CHANIM(0x6c) = 0x44250000;
        CHANIM(0x70) = 0x438f0000;
        CHANIM(0x74) = 0x44228000;
    }
    CHANIM(0x78) = 0x420c0000;
    CHANIM(0x7c) = 0x430e0000;
    CHANIM(0x80) = 0x428e0000;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x84) = 0x442f0000;
        CHANIM(0x88) = 0x439b0000;
        CHANIM(0x8c) = 0x442f0000;
        CHANIM(0x90) = 0x439b0000;
        CHANIM(0x94) = 0x442f0000;
        CHANIM(0x98) = 0x439b0000;
        CHANIM(0x9c) = 0x442f0000;
        CHANIM(0xa0) = 0x439b0000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x84) = 0x44670000;
        CHANIM(0x88) = 0x439b0000;
        CHANIM(0x8c) = 0x44670000;
        CHANIM(0x90) = 0x439b0000;
        CHANIM(0x94) = 0x44670000;
        CHANIM(0x98) = 0x439b0000;
        CHANIM(0x9c) = 0x44670000;
        CHANIM(0xa0) = 0x439b0000;
    } else {
        CHANIM(0x84) = 0x440c0000;
        CHANIM(0x88) = 0x43d20000;
        CHANIM(0x8c) = 0x44318000;
        CHANIM(0x90) = 0x43b90000;
        CHANIM(0x94) = 0x44570000;
        CHANIM(0x98) = 0x43a00000;
        CHANIM(0x9c) = 0x447f0000;
        CHANIM(0xa0) = 0x439b0000;
    }
    CHANIM(0xa4) = 0x0;
    CHANIM(0xa8) = 0x40e00000;
    CHANIM(0xac) = 0x41a80000;
    if (d2_ScreenW == 0x320) {
        CHANIM(0xb0) = 0x442f0000;
        CHANIM(0xb4) = 0x438c0000;
        CHANIM(0xb8) = 0x442f0000;
        CHANIM(0xbc) = 0x438c0000;
        CHANIM(0xc0) = 0x442f0000;
        CHANIM(0xc4) = 0x438c0000;
        CHANIM(0xc8) = 0x442f0000;
        CHANIM(0xcc) = 0x438c0000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0xb0) = 0x44670000;
        CHANIM(0xb4) = 0x438c0000;
        CHANIM(0xb8) = 0x44670000;
        CHANIM(0xbc) = 0x438c0000;
        CHANIM(0xc0) = 0x44670000;
        CHANIM(0xc4) = 0x438c0000;
        CHANIM(0xc8) = 0x44670000;
        CHANIM(0xcc) = 0x438c0000;
    } else {
        CHANIM(0xb0) = 0x440c0000;
        CHANIM(0xb4) = 0x43d20000;
        CHANIM(0xb8) = 0x44318000;
        CHANIM(0xbc) = 0x43b90000;
        CHANIM(0xc0) = 0x44570000;
        CHANIM(0xc4) = 0x43a00000;
        CHANIM(0xc8) = 0x447f0000;
        CHANIM(0xcc) = 0x438c0000;
    }
    CHANIM(0xd0) = 0x0;
    CHANIM(0xd4) = 0x40e00000;
    CHANIM(0xd8) = 0x41a80000;
    prev = r;

    /* FTInitChapter3Step9 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629a48);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337339;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f99;
    U_I(r + 0x10) = (unsigned)&full_data_629a1c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x47;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x3;
    U_I(r + 0x44) = 0x163;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    prev = r;

    /* FTInitChapter3Step10 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629a30);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337341;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f9b;
    U_I(r + 0x10) = (unsigned)&full_data_629a08;
    U_I(r + 0x1c) = 0x3f9d;
    U_I(r + 0x20) = (unsigned)&full_data_6299f0;
    U_I(r + 0x24) = 0x3f9f;
    U_I(r + 0x28) = (unsigned)(full_data_6299c8 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x3c) = (unsigned)&ai_Unk577450;
    U_I(r + 0x40) = 0x12;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x26;
    U_I(r + 0x50) = 0x0;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x1;
    U_I(r + 0x68) = 0x5c37;
    U_I(r + 0x6c) = 0x1e70;
    /* FTInitAnimChapter3Step8 (the copy-paste name again): one animation entry */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0x2c, (char *)(unsigned)&full_data_629ab0);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x0) = 0x43e88000;
        CHANIM(0x4) = 0x43430000;
        CHANIM(0x8) = 0x43e40000;
        CHANIM(0xc) = 0x43960000;
        CHANIM(0x10) = 0x43580000;
        CHANIM(0x14) = 0x438c0000;
        CHANIM(0x18) = 0x43260000;
        CHANIM(0x1c) = 0x432a0000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x0) = 0x440fc000;
        CHANIM(0x4) = 0x43898000;
        CHANIM(0x8) = 0x440d8000;
        CHANIM(0xc) = 0x43b90000;
        CHANIM(0x10) = 0x43ad0000;
        CHANIM(0x14) = 0x43cd0000;
        CHANIM(0x18) = 0x438c0000;
        CHANIM(0x1c) = 0x437f0000;
    } else {
        CHANIM(0x0) = 0x441c8000;
        CHANIM(0x4) = 0x43af0000;
        CHANIM(0x8) = 0x441f0000;
        CHANIM(0xc) = 0x43d70000;
        CHANIM(0x10) = 0x43bc0000;
        CHANIM(0x14) = 0x43dc0000;
        CHANIM(0x18) = 0x43a80000;
        CHANIM(0x1c) = 0x43a00000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x432a0000;
    CHANIM(0x28) = 0x42d40000;
    prev = r;

    /* FTInitChapter3Outro */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6299c8);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x75747233;
    U_I(r + 0x8) = 0x3;
    U_I(r + 0xc) = 0x3fa0;
    U_I(r + 0x10) = (unsigned)&full_data_6299a0;
    U_I(r + 0x2c) = 0xffffffff;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    prev = r;
    U_I((unsigned)&full_data_6f264c) = snd_LoadVoiceBank((char *)(unsigned)&full_data_6299b0, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}
/* 0x57aa80, the tutorial chapter 4 chain (1072 B, ROUGH from the disassembly, the same shape as the chapters 1 ..
   3): five 0x74-byte step records (mem_Alloc (0x74, name) + memset 0x74): +0 = 4, +4 the magic "4rtn" 0x6e747234
   / 0x633473xx / 0x75747234, +0xc the text id, +0x10 the text key (CHAPTER_4_INTRO, CHAPTER_4_B_MAIN,
   CHAPTER_4_D_MAIN, CHAPTER_4_E_MAIN, CHAPTER_4_OUTRO), +0x2c = -1, +0x30, +0x3c = 0x576490, +0x48, +0x68, the
   last +0x6c = 0x1f70; linked through +0x70; the sample bank "TUTORIAL_CHAPTER_4.sbf" 0x629e50 into 0x6f2650
   (0x4c2830 (name, 0)); the first record, or 0x578440 (the chain) and 0 after a failed alloc */

int ai_Unk57aa80(void)
{
    char *rec;
    char *prev;
    char *r;


    /* FTInitChapter4Intro */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629fac);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x3;
    U_I(rec + 0x4) = 0x6e747234;
    U_I(rec + 0xc) = 0x3fa2;
    U_I(rec + 0x10) = (unsigned)&full_data_629f88;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x34) = (unsigned)&ai_Unk5774d0;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0x5837;
    U_I(rec + 0x6c) = 0x1f70;
    prev = rec;

    /* FTInitChapter4Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629f98);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63347334;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fa4;
    U_I(r + 0x10) = (unsigned)&full_data_629f60;
    U_I(r + 0x14) = 0x3fa7;
    U_I(r + 0x18) = (unsigned)&full_data_629f4c;
    U_I(r + 0x1c) = 0x3fa6;
    U_I(r + 0x20) = (unsigned)&full_data_629f3c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk5770a0;
    U_I(r + 0x38) = (unsigned)&ai_Unk576d40;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576e70;
    U_I(r + 0x40) = 0x1;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5a37;
    U_I(r + 0x6c) = 0x1f70;
    U_I(r + 0x70) = 0x0;
    prev = r;

    /* FTInitChapter4Step2 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629f74);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63347332;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fa9;
    U_I(r + 0x10) = (unsigned)&full_data_629f00;
    U_I(r + 0x14) = 0x3fac;
    U_I(r + 0x18) = (unsigned)&full_data_629eec;
    U_I(r + 0x1c) = 0x3fab;
    U_I(r + 0x20) = (unsigned)&full_data_629edc;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk5770a0;
    U_I(r + 0x38) = (unsigned)&ai_Unk576ba0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576cd0;
    U_I(r + 0x40) = 0x1;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5b77;
    U_I(r + 0x6c) = 0x1f70;
    U_I(r + 0x70) = 0x0;
    prev = r;

    /* FTInitChapter4Step5 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629f14);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63347335;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fae;
    U_I(r + 0x10) = (unsigned)&full_data_629ea0;
    U_I(r + 0x14) = 0x3fb1;
    U_I(r + 0x18) = (unsigned)&full_data_629e8c;
    U_I(r + 0x1c) = 0x3fb0;
    U_I(r + 0x20) = (unsigned)&full_data_629e7c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk5770a0;
    U_I(r + 0x38) = (unsigned)&ai_Unk576ee0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk577020;
    U_I(r + 0x40) = 0x1;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5f77;
    U_I(r + 0x6c) = 0x1f70;
    U_I(r + 0x70) = 0x0;
    prev = r;

    /* FTInitChapter4Outro */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629eb4);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x4;
    U_I(r + 0x4) = 0x75747234;
    U_I(r + 0x8) = 0x3;
    U_I(r + 0xc) = 0x3fb3;
    U_I(r + 0x10) = (unsigned)&full_data_629e40;
    U_I(r + 0x2c) = 0xffffffff;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x1837;
    U_I(r + 0x6c) = 0x1f70;
    prev = r;
    U_I((unsigned)&full_data_6f2650) = snd_LoadVoiceBank((char *)(unsigned)&full_data_629e50, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}
/* 0x57aeb0, the tutorial chapter 5 chain (2207 B, ROUGH from the disassembly, the same record shape as the
   chapters 1 .. 4 plus one step record with an animation table: eight 0x74-byte records (mem_Alloc (0x74, name)
   + memset 0x74), +0 = 5 (the last one 6), +4 the magic "5rtn" 0x6e747235 / 0x633573xx, +0xc the text id, +0x10
   the text key (CHAPTER_5_*), +0x2c = -1, +0x30, +0x3c, +0x48, +0x68, linked through +0x70. FTInitChapter5Step2
   carries the animation table at +0x64 (0x58 B = two 0x2c entries, see ai_Unk577e50 / 0x57b8a0), its entry count
   +0x5c = 2 and the step callback +0x60 = 0x5772d0; the screen width d2_ScreenW picks the floats. The sample
   bank "TUTORIAL_CHAPTER_5.sbf" 0x629fd4 into 0x6f2654 (0x4c2830 (name, 0)); the first record, or 0x578440
   (the chain) and 0 after a failed alloc */

int ai_Unk57aeb0(void)
{
    char *rec;
    char *prev;
    char *r;

    /* FTInitChapter5Intro */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a210);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x5;
    U_I(rec + 0x4) = 0x6e747235;
    U_I(rec + 0xc) = 0x3fb5;
    U_I(rec + 0x10) = (unsigned)&full_data_62a1ec;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x34) = (unsigned)&ai_Unk577540;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0x5f77;
    U_I(rec + 0x6c) = 0x1f70;
    prev = rec;

    /* FTInitChapter5Step1 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a1fc);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357331;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fb7;
    U_I(r + 0x10) = (unsigned)&full_data_62a1c4;
    U_I(r + 0x14) = 0x3fb9;
    U_I(r + 0x18) = (unsigned)&full_data_62a1b0;
    U_I(r + 0x24) = 0x3fba;
    U_I(r + 0x28) = (unsigned)(full_data_62a188 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x40) = 0x2c;
    U_I(r + 0x44) = 0x1aa;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x1877;
    U_I(r + 0x6c) = 0x1f78;
    prev = r;

    /* FTInitChapter5Step3 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a1d8);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357333;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fc5;
    U_I(r + 0x10) = (unsigned)&full_data_62a174;
    U_I(r + 0x14) = 0x3fc7;
    U_I(r + 0x18) = (unsigned)&full_data_62a160;
    U_I(r + 0x24) = 0x3fc8;
    U_I(r + 0x28) = (unsigned)(full_data_62a134 + 0x18);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x40) = 0xf;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x18f7;
    U_I(r + 0x6c) = 0x1f78;
    prev = r;

    /* FTInitChapter5Step2 (with its animation table) */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a188);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357332;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fbb;
    U_I(r + 0x10) = (unsigned)&full_data_62a120;
    U_I(r + 0x14) = 0x3fbe;
    U_I(r + 0x18) = (unsigned)&full_data_62a10c;
    U_I(r + 0x1c) = 0x3fbd;
    U_I(r + 0x20) = (unsigned)&full_data_62a0f4;
    U_I(r + 0x24) = 0x3fbf;
    U_I(r + 0x28) = (unsigned)(full_data_62a0cc + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x34) = (unsigned)&ai_Unk577230;
    U_I(r + 0x3c) = (unsigned)&ai_Unk577290;
    U_I(r + 0x40) = 0x17;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x29;
    U_I(r + 0x50) = 0x0;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x2;
    U_I(r + 0x60) = (unsigned)&ai_Unk5772d0;
    U_I(r + 0x68) = 0x1877;
    U_I(r + 0x6c) = 0x1f78;
    /* FTInitAnimChapter5Step2: two animation entries */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0x58, (char *)(unsigned)&full_data_62a134);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x0) = 0x430c0000;
        CHANIM(0x4) = 0x432a0000;
        CHANIM(0x8) = 0x437a0000;
        CHANIM(0xc) = 0x43910000;
        CHANIM(0x10) = 0x43e10000;
        CHANIM(0x14) = 0x43910000;
        CHANIM(0x18) = 0x44160000;
        CHANIM(0x1c) = 0x43988000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x0) = 0x437a0000;
        CHANIM(0x4) = 0x437f0000;
        CHANIM(0x8) = 0x43c80000;
        CHANIM(0xc) = 0x43c80000;
        CHANIM(0x10) = 0x44160000;
        CHANIM(0x14) = 0x43e10000;
        CHANIM(0x18) = 0x44318000;
        CHANIM(0x1c) = 0x43c30000;
    } else {
        CHANIM(0x0) = 0x441c8000;
        CHANIM(0x4) = 0x43af0000;
        CHANIM(0x8) = 0x441f0000;
        CHANIM(0xc) = 0x43d70000;
        CHANIM(0x10) = 0x43bc0000;
        CHANIM(0x14) = 0x43dc0000;
        CHANIM(0x18) = 0x43a80000;
        CHANIM(0x1c) = 0x43a00000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x43310000;
    CHANIM(0x28) = 0x42d40000;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x2c) = 0x442f0000;
        CHANIM(0x30) = 0x439b0000;
        CHANIM(0x34) = 0x442f0000;
        CHANIM(0x38) = 0x439b0000;
        CHANIM(0x3c) = 0x442f0000;
        CHANIM(0x40) = 0x439b0000;
        CHANIM(0x44) = 0x442f0000;
        CHANIM(0x48) = 0x439b0000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x2c) = 0x44670000;
        CHANIM(0x30) = 0x439b0000;
        CHANIM(0x34) = 0x44670000;
        CHANIM(0x38) = 0x439b0000;
        CHANIM(0x3c) = 0x44670000;
        CHANIM(0x40) = 0x439b0000;
        CHANIM(0x44) = 0x44670000;
        CHANIM(0x48) = 0x439b0000;
    } else {
        CHANIM(0x2c) = 0x440c0000;
        CHANIM(0x30) = 0x43d20000;
        CHANIM(0x34) = 0x44318000;
        CHANIM(0x38) = 0x43b90000;
        CHANIM(0x3c) = 0x44570000;
        CHANIM(0x40) = 0x43a00000;
        CHANIM(0x44) = 0x447f0000;
        CHANIM(0x48) = 0x439b0000;
    }
    CHANIM(0x4c) = 0x0;
    CHANIM(0x50) = 0x40e00000;
    CHANIM(0x54) = 0x41a80000;
    prev = r;

    /* FTInitChapter5Step2 (the second record with that name in the original) */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a188);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357332;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fc0;
    U_I(r + 0x10) = (unsigned)&full_data_62a0b8;
    U_I(r + 0x14) = 0x3fc3;
    U_I(r + 0x18) = (unsigned)&full_data_62a0a4;
    U_I(r + 0x1c) = 0x3fc2;
    U_I(r + 0x20) = (unsigned)&full_data_62a08c;
    U_I(r + 0x24) = 0x3fc4;
    U_I(r + 0x28) = (unsigned)(full_data_62a064 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x40) = 0x14;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x2d;
    U_I(r + 0x50) = 0x0;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x68) = 0x1877;
    prev = r;

    /* FTInitChapter5Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a0cc);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357334;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fc9;
    U_I(r + 0x10) = (unsigned)&full_data_62a050;
    U_I(r + 0x14) = 0x3fcb;
    U_I(r + 0x18) = (unsigned)&full_data_62a03c;
    U_I(r + 0x24) = 0x3fcc;
    U_I(r + 0x28) = (unsigned)(full_data_62a014 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x34) = (unsigned)&ai_Unk5775c0;
    U_I(r + 0x40) = 0x2e;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x18f7;
    U_I(r + 0x6c) = 0x3f78;
    prev = r;

    /* FTInitChapter5Step5 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a064);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357335;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fcd;
    U_I(r + 0x10) = (unsigned)&full_data_62a014;
    U_I(r + 0x14) = 0x3fcf;
    U_I(r + 0x18) = (unsigned)&full_data_62a000;
    U_I(r + 0x24) = 0x3fd0;
    U_I(r + 0x28) = (unsigned)(full_data_629fd4 + 0x18);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x34) = (unsigned)&ai_Unk577610;
    U_I(r + 0x40) = 0x2f;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x18f7;
    U_I(r + 0x6c) = 0xbf78;
    prev = r;

    /* FTInitChapter5Step6 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a064);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x6;
    U_I(r + 0x4) = 0x63357336;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fd1;
    U_I(r + 0x10) = (unsigned)&full_data_629fc0;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk577690;
    U_I(r + 0x40) = 0x30;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x18f7;
    U_I(r + 0x6c) = 0xbf78;
    prev = r;
    U_I((unsigned)&full_data_6f2654) = snd_LoadVoiceBank((char *)(unsigned)&full_data_629fd4, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}

#undef CHSTEP
#undef CHANIM

/* 0x57b750, the tutorial's main-outro record (186 B): mem_Alloc (0x74, "FTInitMainOutro" 0x62a238), 0 when it
   fails (0x578440 (0)); memset 0x74; +0x14 .. +0x20 / +0x30 / +0x34 / +0x38 / +0x40 / +0x44 / +0x70 = 0, then
   +0 = 7, +4 = the magic "RTUO" 0x4f555452, +8 = 5, +0xc = 0x3f27, +0x10 = the text "TUTORIAL_MAIN_OUTRO"
   0x62a224, +0x2c = -1, +0x48 = 0xb, +0x68 = -9; the record */
int ai_Unk57b750(void)
{
    char *rec;

    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a238);
    if (!rec) {
        ai_Unk578440(0);
        return 0;
    }
    memset(rec, 0, 0x74);
    U_I(rec + 0x14) = 0;
    U_I(rec + 0x18) = 0;
    U_I(rec + 0x1c) = 0;
    U_I(rec + 0x20) = 0;
    U_I(rec + 0x30) = 0;
    U_I(rec + 0x34) = 0;
    U_I(rec + 0x38) = 0;
    U_I(rec + 0x40) = 0;
    U_I(rec + 0x44) = 0;
    U_I(rec + 0x70) = 0;
    U_I(rec) = 7;
    U_I(rec + 4) = 0x4f555452;
    U_I(rec + 8) = 5;
    U_I(rec + 0xc) = 0x3f27;
    U_I(rec + 0x10) = (unsigned)&full_data_62a224;
    U_I(rec + 0x2c) = -1;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = -9;
    return (int)rec;
}
/* 0x57b810, a cubic Bezier of two floats (134 B): u = 1.0f (0x5eb780) - t, out[0] / out[1] =
   u^3 * a + 3 * u * t * (u * b + t * c) + t^3 * d for the components +0 / +4 of the float vectors a, b, c, d;
   `t` is the fifth argument (a float on the stack), `out` the sixth. The term nesting is the original's FP stack
   (u * b + t * c scaled by 3 * u * t) */

void ai_Unk57b810(float *a, float *b, float *c, float *d, float t, float *out)
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

/* 0x57b8a0, the tutorial per-frame update (910 B, ROUGH from the disassembly): - nothing without the gate
   0x6f265c, the step table r+0x64 (r = S+0x18) or its count r+0x5c < 1. The step callback r+0x60 called with
   `arg` answers v (edi): -1 and below -> 2, else a step index 0 .. r+0x5c-1 that differs from S+0x54 switches to
   the phase S+0x58 = 2, S+0x5c = the clock 0x6333cc, S+0x54 = v. Then S+0x60 (the position object, -1 = none) is
   0x472f50 (S+0x60) and 0x475aa0 (S+0x60, 1 when S+0x64 == 0x6f229c, else 0). By v: 0 -> S+0x64 = 0x6f229c,
   S+0x5c = clock, S+0x58 = 2, the object created by 0x46cd80 ((int) e[0], (int) e[1], 0x1d) when S+0x60 is -1
   (e = the step entry, 0x2c B) and 0x46cda0 ((int) e[0], (int) e[1], S+0x60); 1 -> the phases S+0x58:
   2 (wait: the clock past S+0x5c + e+0x20 -> 3, S+0x5c = clock), 3 (the Bezier 0x57b810 (e+0, e+8, e+0x10,
   e+0x18, t = (clock - S+0x5c) / e+0x24, the local vector) into 0x46cda0 (x, y, S+0x60) and return), 4 (the clock
   past S+0x5c + e+0x28 -> 2, then 0x46cda0 ((int) e[0], (int) e[1], S+0x60)); 2 -> 0x46e710 (S+0x60), S+0x60 = -1,
   S+0x58 = 1 */
void ai_Unk57b8a0(int arg)
{
    char *st;
    char *r;
    char *e;
    float v[2];
    int x;
    int y;

    if (!U_I((unsigned)&full_data_6f265c))
        return;
    st = S;
    r = *(char **)(st + 0x18);
    if (!*(int *)(r + 0x64))
        return;
    if (*(int *)(r + 0x5c) < 1)
        return;
    x = 0;
    if (*(int *)(r + 0x60)) {
        x = ((int (*)()) * (int *)(r + 0x60))(arg);
        st = S;
        if (x <= -1) {
            x = 2;
        } else if (x >= 0 && x < *(int *)(*(char **)(st + 0x18) + 0x5c) && x != U_I(st + 0x54)) {
            U_I(st + 0x58) = 2;
            U_I(st + 0x5c) = U_I((unsigned)&ai_ClockMs);
            U_I(st + 0x54) = x;
            st = S;
        }
    }
    if (U_I(st + 0x60) != -1) {
        d2_RaiseObject(U_I(st + 0x60));
        if (U_I(S + 0x64) == U_I((unsigned)&sv_FrameDepth))
            d2_EnableObject(U_I(S + 0x60), 1);
        else
            d2_EnableObject(U_I(S + 0x60), 0);
        st = S;
    }
    e = (char *)(*(int *)(*(char **)(st + 0x18) + 0x64) + U_I(st + 0x54) * 0x2c);
    if (x == 0) {
        U_I(st + 0x64) = U_I((unsigned)&sv_FrameDepth);
        U_I(st + 0x5c) = U_I((unsigned)&ai_ClockMs);
        U_I(st + 0x58) = 2;
        if (U_I(S + 0x60) == -1)
            U_I(S + 0x60) = d2_CreatePicObj((int)*(float *)e, (int)*(float *)(e + 4), 0x1d);
        d2_MoveObj((int)*(float *)e, (int)*(float *)(e + 4), U_I(S + 0x60));
        return;
    }
    if (x == 1) {
        if (U_I(st + 0x58) == 2) {
            if (!((float)U_I((unsigned)&ai_ClockMs) > (float)U_I(st + 0x5c) + *(float *)(e + 0x20)))
                return;
            U_I(st + 0x58) = 3;
            U_I(st + 0x5c) = U_I((unsigned)&ai_ClockMs);
            st = S;
        }
        if (U_I(st + 0x58) == 3) {
            ai_Unk57b810((float *)e, (float *)(e + 8), (float *)(e + 0x10), (float *)(e + 0x18),
                         ((float)U_I((unsigned)&ai_ClockMs) - (float)U_I(st + 0x5c)) / *(float *)(e + 0x24), v);
            d2_MoveObj((int)v[0], (int)v[1], U_I(S + 0x60));
            return;
        }
        if (U_I(st + 0x58) == 4) {
            if (!((float)U_I((unsigned)&ai_ClockMs) > (float)U_I(st + 0x5c) + *(float *)(e + 0x28)))
                return;
            U_I(st + 0x58) = 2;
            U_I(st + 0x5c) = U_I((unsigned)&ai_ClockMs);
            d2_MoveObj((int)*(float *)e, (int)*(float *)(e + 4), U_I(S + 0x60));
        }
        return;
    }
    if (x == 2) {
        if (U_I(S + 0x60) != -1)
            d2_RemoveObj(U_I(S + 0x60));
        U_I(S + 0x60) = -1;
        U_I(S + 0x58) = 1;
    }
}

int gm_HeadIconNumber();
unsigned *gm_Unk503ee0(unsigned *out, int n);

/* 0x57bc30, the tutorial info block (168 B, ROUGH): the bytes 0x75e5a0 = 0x4b, 0x75e5a1 = 0x18, 0x75e5a2 =
   0x10, 0x75e5a3 = 0x33, the dword 0x75e5a4 = 0x59a, 0x75e5a8 / 0x75e5a9 = 0; 0x75e628 = 0x4fb770 (0x624ee0)
   + 0x639; the strings U_I(0x1449e48) / U_I(0x1449e4c) copied to 0x75e5aa / 0x75e5ca; 0x503ee0 (the local
   buffer, the byte 0x75e5a3) returns a record whose dword +0 goes to 0x75e5f0 and whose word +4 (low byte) to the
   local and 0x75e5f4 */
void ai_SetupTutorialPlayer(void)
{
    char local[0x10];
    char *r;

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
    gm_Unk503ee0((unsigned *)local, *(unsigned char *)((unsigned)&init_PlayerProfile + 0x3));
    r = local;
    *(int *)init_PlayerProfile.talents = *(int *)r;
    *(unsigned short *)(local + 0xc) = *(unsigned short *)(r + 4);
    init_PlayerProfile.talents[4] = *(unsigned char *)(r + 4);
}

int sv_RunFrame(int cb, unsigned mask);

/* 0x57bce0, the tutorial chapter window (541 B): the window 0x4741f0 (0, 0, "tutorial\CHOOSE_tutorial" 0x62a248)
   with 0x475600, 0x475f70 (id, 1), the text 0x3f1a through 0x515960 (0x6024f8) and 0x475f70 (id, 2); the five
   chapter entries (i = 0 .. 4): the text 0x3f1b + i through 0x515960 (0x6024f8) into 0x475b80 (id, 2, text), the
   id stored in the local list, and in the window table entry 0xf8f080 + id * 0x2ec: +0x48 (checkable) = 1,
   +0x28 (checked) = the chapter table 0x62918c[i], +0x1c0 (style) &= -3; the two extra texts 0x3f20 (the start)
   / 0x3f21 (the back) through 0x475b80; control 2 via 0x475f70. Then the frame pump 0x5309a0 (this function,
   0x86), every returned frame: the five entries scanned for a checked one (their +0x28 != 0) and 0x475e40 (the
   start, 1), or 0x475e40 (the start, 0) when none is set; the escape (the right button 0x14673a0, the key
   0x1468410 == 1, the enter 0xf8a668 == 1) sets 0x6f22a0 = 1; a click (0xf8a668 != -1) on the back sets
   0x6f22a0 = 1, on the start sets 0x6f22a0 = 1, writes the checked state of the five entries back to 0x62918c
   and makes the result 1 when at least one is checked. 0x4758f0 closes the window; 1 starts the tutorial, 0 is
   the back. (The menu that is skipped when this function pops at once is exactly this pump loop.) */
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
    id = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a248);
    d2_CentreViewWindows(id);
    d2_SetForm(id, 1);
    io_PrintText((char *)(unsigned)"$Z%s", 0x3f1a);
    d2_SetForm(id, 2);
    for (i = 0; i < 5; i++) {
        ids[i] = d2_GetWinObj(id, 2, io_PrintText((char *)(unsigned)"%ia[%s]$N", 0x3f1b + i));
        U_I(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec + 0x48) = 1;
        U_I(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec + 0x28) = U_I((unsigned)&full_data_62918c + i * 4);
        U_I(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec + 0x1c0) &= -3;
    }
    start = d2_GetWinObj(id, 2, io_PrintText((char *)(unsigned)"%ia[%s]$N", 0x3f20));
    back = d2_GetWinObj(id, 2, io_PrintText((char *)(unsigned)"%ia[%s]$N", 0x3f21));
    d2_SetForm(id, 2);
    while (sv_RunFrame((unsigned)&ai_TutorialWindow, 0x86) != 0) {
#ifdef REA_START
        /* REA_START=tutorial (`uv run play|dbg --start tutorial`): the chapter menu is the screen between the
           main-menu entry and the loading screen. Accept its default selection (0x62918c starts all 1) the way
           the "Tutorial starten" click does, so a run without GUI automation still reaches the tutorial;
           without the flag the menu waits for a real click. The frame above has drawn and registered it. */
        {
            char *rea = getenv("REA_START");
            if (rea != 0 && strcmp(rea, "tutorial") == 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
                result = 1;
            }
        }
#endif
        n = 0;
        for (i = 0; i < 5; i++) {
            if (U_I((char *)(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec) + 0x28) != 0) {
                d2_SetObjEnabled(start, 1);
                break;
            }
            n++;
        }
        if (n == 5)
            d2_SetObjEnabled(start, 0);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0 || *(unsigned char *)(unsigned)&kb_Last == 1 || U_I((unsigned)&d2_ClickedId) == 1)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == back) {
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_PointerObj) == start) {
                U_I((unsigned)&sv_PopRequest) = 1;
                n = 0;
                for (i = 0; i < 5; i++) {
                    if (U_I((char *)(U_I((unsigned)&d2_Objs) + ids[i] * 0x2ec) + 0x28) != 0) {
                        n++;
                        U_I((unsigned)&full_data_62918c + i * 4) = 1;
                    } else {
                        U_I((unsigned)&full_data_62918c + i * 4) = 0;
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

struct AHMSTEP * ahm_WalkStep();
int ahm_StepValue();
int ai_Unk57c560();

/* 0x57bf00, the tutorial results window (1629 B, ROUGH SKELETON): a 0x55c-byte local frame; 0x4fb330 (0, 0x52);
   the three player records collected (0x4ec2d0 (i) when non-null, then 0x4ec320 (p) - the names -, i = 0 .. 2);
   when no player was found the byte 0x6f1bc5 seeds a fallback list (the [esp+0x168] buffer). Then the window
   0x4741f0 and its controls / texts (0x475f70, 0x473650 x8, 0x4760b0 x5, 0x5176d0 x9, 0x52eaa0 x4, 0x521410,
   0x52e350, 0x52ea30), 0x57c560 (the results list), the release 0x4758f0, 0x4731d0 x2, 0x476f80, 0x475e40,
   0x475600 and the pump 0x5309a0 (0x86, ...); the rest of the body is not written yet (see the disassembly
   build/dis57bf00.txt) */
int ai_Unk57bf00(void *rec)
{
    char *per[3];
    char *nm[3];
    char *v;
    int n;
    int i;

    v = (char *)unk_Unk4fb330(0, 0x52);
    n = 0;
    for (i = 0; i < 3; i++) {
        per[n] = (char *)ahm_WalkStep(i);
        if (*(int *)per[n]) {
            nm[n] = (char *)ahm_StepValue(per[n]);
            n++;
        }
    }
    /* ROUGH: the n == 0 fallback (the byte 0x6f1bc5, the local buffer at +0x168), the window and its controls,
       the results built by 0x57c560, the pump 0x5309a0 (0x86) and the tail are not written yet */
    return (int)v;
}

char * ahm_OwnerChar();
int sv_CheckGoodAmount();
int sv_MessageBox();
void txt_Format(char *out, int id0, ...);
int ahm_SendCreateAlchemist();

/* 0x57c560, the tutorial result string writer (301 B, ROUGH SKELETON): p = 0x4eaa50 (arg 1) (the record looked
   up); 0x521040 (the player user record 0x770b80 + U16(0x6f25b8) * 0x21c, arg 3, the byte 0x6f1bc5): when true
   and p is set, the global text U_I(0x144a2e0) is written (0x521150 (text, 0x100)) and 0 answers; else the
   string built by 0x5176d0 (the 0x200 buffer, 0x4044, the tables 0x144a7a8[arg2->0x10] / 0x144a80c[arg2->0x14])
   and written by 0x521150 (buffer, 1), then 0x4ec490 (arg 1, arg 2) and the rest (the tail is not written yet,
   see build/dis57c560.txt) */
int ai_Unk57c560(int a, char *rec, int c)
{
    char buf[0x200];
    char *p;

    p = (char *)ahm_OwnerChar(a);
    if (sv_CheckGoodAmount((char *)((unsigned)&ai_Chars + (unsigned short)U_I((unsigned)&ai_PlayerId) * 0x21c), c, *(unsigned char *)(unsigned)&ai_OwnPlayer)) {
        if (p)
            sv_MessageBox(U_I((unsigned)&full_data_144a2e0), 0x100);
        return 0;
    }
    txt_Format(buf, 0x4044, U_I((unsigned)&ai_Unk144a7a8 + *(int *)(rec + 0x10) * 4),
                 U_I((unsigned)&ai_Unk144a80c + *(int *)(rec + 0x14) * 4));
    if (!sv_MessageBox(buf, 1))
        return 0;
    /* ROUGH: 0x4ec490 (arg 1, arg 2) and the tail are not written yet */
    return 0;
}

int ahm_ResearchState();

/* 0x57c690, the tutorial selection handler (1871 B, ROUGH SKELETON): arg 1 = the record; the lookup 0x4eaa50
   (record), the dword record +0x65, 0x4eb180 (record, &local, &local2, 0), the clock 0x6333cc into a local,
   0xf8a668 = -1, the window 0x4741f0 (0, 0, 0x62a284), 0x476f80 (0, 0) and 0x475600 (the window). The rest of
   the body (the selection entries, the pump and the tail) is not written yet, see build/dis57c690.txt */
int ai_Unk57c690(char *rec)
{
    char *r;
    int a;
    int b;
    int l1;
    int l2;

    r = (char *)ahm_OwnerChar(rec);
    a = U_I(rec + 0x65);
    b = ahm_ResearchState(rec, &l1, &l2, 0);
    l1 = U_I((unsigned)&ai_ClockMs);
    U_I((unsigned)&d2_ClickedId) = -1;
    r = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a284);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(r);
    /* ROUGH: the rest is not written yet */
    return 0;
}
/* 0x57cde0, a tutorial message builder (227 B, ROUGH SKELETON): a 0x100-byte frame; 0x4eb180 (arg 1, &local,
   &local2, 0) - non-zero answers 0 at once; the 0xf8-byte struct zeroed (rep stos 0x3e dwords) and filled from
   the record (+1 dword, the word +0x27), the clock globals 0x770ae0 / 0x770ae4 / 0x770ae8 / 0x770aec, the
   character record 0x770b84 + the word * 0x21c, the random 0x42ec00 (0x5a0); then the message from the locals
   and the record (the rest is not written yet, see the disassembly) */

int ai_Unk57cde0(char *rec)
{
    char buf[0x100];
    int a;
    int b;

    if (ahm_ResearchState(rec, &a, &b, 0) != 0)
        return 0;
    memset(buf, 0, 0xf8);
    /* ROUGH: the fields and the message are not written yet */
    return 0;
}
/* 0x57ced0, a second tutorial message builder (189 B, ROUGH SKELETON, the 0x57cde0 shape): a 0x100-byte frame;
   0x4eb180 (arg 1, &local, &local2, 0) - non-zero answers 0; the 0xf8-byte struct zeroed and filled from the
   record (+1 dword, the word +0x27), the clock globals 0x770ae0 / 0x770ae4 / 0x770ae8 / 0x770aec, the character
   record 0x770b84 + the word * 0x21c, the byte 0x95 at +4 and 1 at +0x3a, 0x4c4b70 (the local); 1 */

int ai_Unk57ced0(char *rec)
{
    char buf[0x100];
    int a;
    int b;

    if (ahm_ResearchState(rec, &a, &b, 0) != 0)
        return 0;
    memset(buf, 0, 0xf8);
    /* ROUGH: the fields and the message are not written yet */
    cm_SendLongMessage(buf + 0x10);
    return 1;
}
/* 0x57cf90, the tutorial window handler (3264 B, ROUGH SKELETON - the 0x57c690 shape): arg 1 = the record; the
   lookup 0x4eaa50 (record), the dword record +0x65, 0x4eb180 (record, &local, &local2, 0), the clock 0x6333cc,
   the nine dwords at esp+0x2c set to -1, 0xf8a668 = -1, the window 0x4741f0 (0, 0, 0x62a2a4), 0x476f80 (0, 0),
   0x475600 (window), 0x475f70 (window, 0). The rest of the body (the entries, the pump and the tail) is not
   written yet, see the disassembly */

int ai_Unk57cf90(char *rec)
{
    char *r;
    int a;
    int b;
    int l1;
    int l2;

    r = (char *)ahm_OwnerChar(rec);
    a = U_I(rec + 0x65);
    b = ahm_ResearchState(rec, &l1, &l2, 0);
    l1 = U_I((unsigned)&ai_ClockMs);
    U_I((unsigned)&d2_ClickedId) = -1;
    r = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a2a4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(r);
    d2_SetForm(r, 0);
    /* ROUGH: the rest is not written yet */
    return 0;
}
/* 0x57dc50, a tutorial skill window (520 B): arg 1 = the record, arg 2 = the value put into the message, arg 3 =
   the skill index. The lookup 0x4eb180 (record, &a, &b, 0) != 0 or index 3 clears the answer flag; the window
   "Locations_Domizil_skill" (0x62a2c4) with 0x476f80 (0, 0) / 0x475600, the sound 0x52ea30 (0x6ed938), the
   texts 0x4045 (index + 0x4172), index + 0x4176 and 0x4046 (0x182, only with the answer), the two entries
   0x475b80 (window, 2, 0x515960 (0x4047) / +1) and 0x475e40 (the first entry, the answer), 0xf8a668 = -1.
   Then the pump loop 0x5309a0 (this handler, 0x677c7): the click 0x14673a0 -> 0x6f22a0 = 1; the window
   0x5fe710 == the first entry -> the message (0xf8 bytes: type 0x97 at +4, 1 at +0x36, the record +1 dword at
   +0xc, the character 0x770b84 [word record +0x27] at +8, the game time 0x770ae0 at +0x28, arg[0] = *arg 2 at
   +0x58, arg[1] = the index at +0x5c, arg[4] = random 0x42ec00 (0x5a0) & 0xffff + 1000 at +0x68) and
   0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 1 */



int ai_Unk57dc50(char *rec, int *arg, int idx)
{
    char msg[0xf8];
    char *win;
    int a;
    int b;
    int t1;
    int t2;
    int ok;

    ok = 1;
    if (ahm_ResearchState(rec, &a, &b, 0) != 0)
        ok = 0;
    if (idx == 3)
        ok = 0;
    win = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a2c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
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
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == t1) {
                memset(msg, 0, 0xf8);
                *(unsigned char *)(msg + 0x04) = 0x97;
                *(unsigned char *)(msg + 0x36) = 1;
                *(int *)(msg + 0x0c) = *(int *)(rec + 1);
                *(int *)(msg + 0x08) = ((int *)(unsigned)&ai_CharInts)[*(unsigned short *)(rec + 0x27) * 0x87];
                *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                *(int *)(msg + 0x58) = *arg;
                *(int *)(msg + 0x5c) = idx;
                *(int *)(msg + 0x68) = (util_Random(0x5a0) & 0xffff) + 1000;
                cm_SendLongMessage(msg);
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_PointerObj) == t2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk57dc50, 0x677c7) == 0) {
            d2_CloseView(win);
            return 1;
        }
    }
}

int ahm_ObjectValue();
char * ahm_OwnerChar();
float ahm_Share();
int d2_GetObjValue();
int gm_ConvertMoney(int a, unsigned char k);
int gm_PlayerFactor(int a, unsigned char k);
int sv_CheckGoodAmount();
int ahm_SendAddValue();
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);

/* 0x57de60, the tutorial training window (631 B, the 0x57dc50 sibling): the skill points 0x4ec3d0 (record) and
   their tenth 0x4ec3d0 / 10; the run record 0x4eaa50 (record), 0 for none -> 0; the factor 0x4eab20 (record, 3)
   (a float); the window "Locations_Domizil_skill" (0x62a2c4) with 0x476f80 (0, 0) / 0x475600, the sound
   0x52ea30 (0x6ed938), the text 0x403a (0x182, the byte 0x6f1bc5) into the entry 0x475b80 (window, 1, text),
   the number 0x5085b0 (tenth, 0x6f1bc5) / (points) / (tenth) into 0x475cb0 (entry), the texts 0x403b / +1 into
   the entries 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0 (this handler, 0x677c7):
   0x4eaa50 (record) 0 -> 0x6f22a0 = 1, the pump 0x20007, 0x4758f0 (window), 0; the click 0x14673a0 ->
   0x6f22a0 = 1; the window 0x5fe710 == the first 0x403b entry -> the field text 0x475980 (the 0x403a entry,
   0x6f1bc5) parsed 0x5085f0 and checked 0x521040 (the character 0x770b80 + player 0x6f25b8 * 0x21c, value,
   0x6f1bc5): non-zero -> 0x4ecd50 (record, value / points * the factor * 0.75), the result 1, 0x6f22a0 = 1 and
   0x4c47e0 (record +4, the character 0x770b84 + player * 0x21c, 0x6f1bc5, value); the second entry -> 0x6f22a0
   = 1; the pump 0 -> 0x4758f0 (window), the result */
int ai_Unk57de60(char *rec)
{
    char *rr;
    char *win;
    int sp;
    int q;
    float f;
    int fld;
    int b1;
    int b2;
    int v;
    int done;

    done = 0;
    sp = ahm_ObjectValue(rec);
    q = sp / 10;
    rr = (char *)ahm_OwnerChar(rec);
    if (!rr)
        return 0;
    f = ahm_Share(rr, 3);
    win = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a2c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    v = io_PrintText(0x403a, 0x182, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    fld = d2_GetWinObj(win, 1, v);
    v = gm_ConvertMoney(q, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    v = ((int (*)(int, int, int))gm_ConvertMoney)(sp, *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
    v = ((int (*)(int, int, int))gm_ConvertMoney)(q, *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
    d2_SetObjValue(fld, v);
    d2_SetForm(win, 2);
    v = io_PrintText(0x403b);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        rr = (char *)ahm_OwnerChar(rec);
        if (!rr) {
            U_I((unsigned)&sv_PopRequest) = 1;
            sv_RunFrame(ai_Unk57de60, 0x20007);
            d2_CloseView(win);
            return 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                v = ((int (*)(int))gm_PlayerFactor)(d2_GetObjValue(fld, *(unsigned char *)(unsigned)&ai_OwnPlayer));
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, v, *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0) {
                    ahm_SendAddValue(rr, ((float)v / (float)sp) * f * 0.75f);
                    done = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                    cm_SendMoveGoods(*(int *)(rr + 4), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk57de60, 0x677c7) == 0) {
            d2_CloseView(win);
            return done;
        }
    }
}

void ahm_RescheduleResearch();
int ahm_SendDelete();

/* 0x57e0e0, the tutorial skill "use" window (336 B, the 0x57de60 sibling): the run record 0x4eaa50 (record),
   0 for none -> 0; the window "Locations_Domizil_skill" (0x62a2c4) with 0x476f80 (0, 0) / 0x475600, the sound
   0x52ea30 (0x6ed938), the text 0x403d (0x182) and the entries 0x403e / +1 via 0x475b80 (window, 2, text),
   0xf8a668 = -1; the pump loop 0x5309a0 (this handler, 0x677c7): 0x4eaa50 (record) 0 -> 0x6f22a0 = 1, the pump
   0x20007, 0x4758f0 (window), 0; the click 0x14673a0 -> 0x6f22a0 = 1; the first entry -> the result 1,
   0x4eb410 (record) and 0x4ece10 (the run record), 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 ->
   0x4758f0 (window), the result */
int ai_Unk57e0e0(char *rec)
{
    char *rr;
    char *win;
    int b1;
    int b2;
    int v;
    int done;

    done = 0;
    rr = (char *)ahm_OwnerChar(rec);
    if (!rr)
        return 0;
    win = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a2c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText(0x403d, 0x182);
    d2_SetForm(win, 2);
    v = io_PrintText(0x403e);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        rr = (char *)ahm_OwnerChar(rec);
        if (!rr) {
            U_I((unsigned)&sv_PopRequest) = 1;
            sv_RunFrame(ai_Unk57e0e0, 0x20007);
            d2_CloseView(win);
            return 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                done = 1;
                ahm_RescheduleResearch(rec);
                ahm_SendDelete(rr);
                U_I((unsigned)&sv_PopRequest) = 1;
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk57e0e0, 0x677c7) == 0) {
            d2_CloseView(win);
            return done;
        }
    }
}
/* 0x57e230, stores one 0x38-byte record of a 3-entry table (61 B): src (0 .. 2 in src[0]) copied 0xe dwords
   into 0x6f9f60 + src[0] * 0xe and 0x6f2664 = 1; 1, else 0 */

int ai_Unk57e230(int *src)
{
    int i;

    if (!src || src[0] < 0 || src[0] >= 3)
        return 0;
    for (i = 0; i < 0xe; i++)
        ((int *)(unsigned)&full_data_6f9f60)[src[0] * 0xe + i] = src[i];
    U_I((unsigned)&full_data_6f2664) = 1;
    return 1;
}

char * gv_FindEffect(int id); void unk_Unk4eea70(char *o); int cm_GetSlotState(); void unk_Unk4eeee0();
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */
void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b); void sv_SetPanelEnabled(); void d3sky_Init(struct D3SKY *s, char build);
void d3t_GetObjectBounds(); 
void ai_Unk580a90(); void ai_Unk57f890(); char d3if_ShowTree(struct D3OBJECT *obj, char show);
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);                 /* 0x447190, d3_interface.h */
char d3if_Show(struct D3OBJECT *obj, char show);        /* 0x4471b0, d3_interface.h */
void he_Unk53ec10(); void ai_Unk57f5d0(); void ai_Unk57ec40(char *sc, float dt); void ai_Unk57f0e0();
void ai_Unk57f690(); void ai_Unk57fdd0(char *rec, float *sc, char *run, float sel, int mask); int ai_Unk57f620(); void ai_Unk580160();
void sv_SetStatusText(char *s); void d2_RemoveObj(); void sv_RedrawScreen();
void d3if_Prerender(char map); 

/* 0x57e270, the tutorial cannon scene (2305 B, ROUGH: the whole call sequence and the main loop are written, the
   3D setup details are comments): arg 1 = the record; the scene struct sc (local_704, 0x39 floats: the camera /
   target, its control ids at 0xbc / 0xc8 / 0xcc / 0xd0, the selected index at 0xa0, the objects at 0x8c) zeroed;
   the clock 0x6333cc saved; returns at once without rec + 0x61; the run record 0x4eded0 (rec + 1), else the loader
   0x4eea70 (rec) waited out on 0x4c3ae0 (pump 0x20007), then 0x4eded0 again (0 -> return); the table callback
   0x4eeee0 (rec + 1, 0x57e230, 0x6fa020). The camera globals of 0xfc2dec (+0x4c .. +0x54, +0x84 .. +0x8c), the
   screen 0xfb9060 / 0x64 / 0x68 / 0x6c and 0x633444 saved; the window cleanup 0x476f80 (0, 0) / 0x52ae40 /
   0x476c70 / 0x520f20, 0x75e8c0 = 0, 0xf8aae0 = 0; the BLOCK "BLACK" (0x5fea38) drawn by 0x4767d0 (0, 0,
   0xfb9072, 0xfb9070, "BLACK", 0x5a, 1) and waited on (*pb & 4, pump mask 0x10180); the window table 0xf8f080
   (stride 0x2ec, 0x300 entries) scanned against the list 0xfb11c0 (stride 0xab, to 0xfb9200): an entry with
   +0x18 set, +0x40 = the list record - 0x68 and +0x38 clear gets +0x38 = 1 and its index into the list; the
   screen 0x43b950 ("BLACK", the rect 0x6248a4 .. 0x6248b0, 0xfe73b4, 0xfc2564, 0xfc5348), 0x522100 (0), the
   camera restored, 0x4478e0 (0, 0), 0x45d690 (0x633dd4, 1); 0x413150 (two 4-byte locals, rec + 0x61); the
   0x925100 block reset (the record + 1 changed) or reused (0x925108 / 0x92510c / 0x925110 = the camera);
   0x458a20 (rec + 0x61, rec + 0x61 + 0x4c, sc); sc + 0x10 = 0x925108 / 0c / 10, sc + 4 += (a - b) * 0.8; the four
   scene objects 0x446e10 ("KANONE_UNTERBAU_Aa" 0x62a350, "KANONE_ROHR_Aa" 0x62a340, "KANONE_DOPPEL_ROHR_Aa"
   0x62a328, "KANONE_DREIFACH_ROHR_Aa" 0x62a310) placed (0x43b790 / 0x43b860) and the wimpel object "AO_KANONE_
   WIMPEL_Aa" 0x62a2fc with 0x411960 ("ADDON_AO_KANONE_WIMPEL_Aa" 0x62a2dc, 1) and its bytes +0x212 = (b & 0xf7)
   | 4, then | 0x40, +0x211 &= 0xfd; the mouse 0x4796c0 (0x6ed924), the pump 0x86 (twice); the BLOCK redrawn
   (0x476860, 0x4767d0 ... 0x5a, 10); 0x580a90 (sc, the run) and 0x57f890 (rec, sc, the run); the selected index
   = MIN (sc + 0xdc, ...); 0x471b0 (obj[i], 0) for 0..2 and (obj[selected], 1) (the control 0x447300 / 0x53ec10,
   rec + 0x61, 0); 0xf8a668 = -1; 0x57f5d0 (sc); the 3-entry table 0x6f9f60 / 0x98 / 0xd0 = -1; the main loop
   while 0x5309a0 (this handler, 0x2656c7): 0x522100 (0), dt = (the clock - last) * 0.013513514; the record + 4
   set and the escape / click -> 0x6f22a0 = 1, else 0x57ec40 (sc, dt) / 0x57f0e0 (sc) / 0x57f890 / 0x57f690 (sc), the
   clicks on the ids sc + 0xc8 / 0xcc / 0xd0 -> 0x57fdd0 (rec, sc, the run, 0 / 1 / 2, 0x2656c7), and
   0x57f620 (sc) -> 0x580160 (sc, rec) and the table reset. The teardown: 0x52e330 (0x631090), the BLOCK, the
   windows freed (0x4758f0 x3, 0x46e710 x2, 0x47190 x5), 0x447300 / 0x53ec10 (rec + 0x61, 1), the scanned table
   entries +0x38 = 0, the screen 0x43b950, the camera / 0xfb9060 .. restored, 0x522100 (1), 0x524010,
   0x4478e0 (saved, 0), 0x45d690, 0x43b790 / 0x43b860 (0xfc2dec, saved), 0x513590 (0x633738, 0), 0x447860 (1),
   the BLOCK closed, 0x4eeee0 (rec + 1, 0, 0) */
void ai_Unk57e270(char *rec)
{
    char sc[0xe4];
    int list[0x300];
    int obj[4];
    int sel;
    int n;
    int item;
    char *run;
    char *pb;
    int m;
    int last;
    int now;
    float dt;
    char *cam;
    int cam3[3];
    int save3[3];
    int bg[4];
    int bg5;
    char a4[4];
    char b4[4];
    int *pp;

    *(unsigned char *)((unsigned)&kb_Down + 0x2f) = 0;
    memset(sc, 0, 0xe4);
    last = U_I((unsigned)&ai_ClockMs);
    if (!U_I(rec + 0x61))
        return;
    run = (char *)gv_FindEffect(U_I(rec + 1));
    if (!run) {
        m = ((int (*)(int))unk_Unk4eea70)(rec);
        if (m != -1)
            while (cm_GetSlotState(m) == 0)
                sv_RunFrame(0, 0x20007);
        run = (char *)gv_FindEffect(U_I(rec + 1));
        if (!run)
            return;
    }
    unk_Unk4eeee0(U_I(rec + 1), ai_Unk57e230, (char *)(unsigned)&full_data_6fa020);
    cam = (char *)U_I((unsigned)&sv_CameraObj);
    cam3[0] = U_I(cam + 0x4c);
    cam3[1] = U_I(cam + 0x50);
    cam3[2] = U_I(cam + 0x54);
    save3[0] = U_I(cam + 0x84);
    save3[1] = U_I(cam + 0x88);
    save3[2] = U_I(cam + 0x8c);
    bg[0] = U_I((unsigned)&ai_Unkfb9068);
    bg[1] = U_I((unsigned)&ai_Unkfb9060);
    bg[2] = U_I((unsigned)&ai_Unkfb9064);
    bg[3] = U_I((unsigned)&ai_Unkfb906c);
    bg5 = U_I((unsigned)&d3_CurrentScene);
    d2_SetPointerSprite(0, 0);
    sv_ClearSelection();
    d2_ClearPointerItems();
    sv_ResetScreenClip();
    U_I((unsigned)&sv_PanelFlag) = 0;
    U_I((unsigned)&d2_CaptionCopy) = 0;
    pb = (char *)d2_StartFade(0, 0, U_I((unsigned)&d2_ScreenW), U_I((unsigned)&d2_ScreenH), (char *)(unsigned)"BLACK", 0x5a, 1);
    while (!(*(unsigned char *)pb & 4))
        sv_RunFrame(0, 0x10180);
    n = 0;
    {
        int *lp = (int *)((unsigned)&ai_Unkfb1030 + 0x190);
        for (; (int)lp < (unsigned)&d3s_QuadUV; lp += 0xab / 4) {
            if (lp[-4] != 0 && lp[0] != 0) {
                int i = 0;
                char *e = (char *)U_I((unsigned)&d2_Objs);
                while (e < (char *)0x8c400 + U_I((unsigned)&d2_Objs)) {
                    if (*(int *)(e + 0x18) != 0 && *(int *)(e + 0x40) == (int)(lp - 0x68) && *(int *)(e + 0x38) == 0) {
                        *(int *)(e + 0x38) = 1;
                        list[n++] = i;
                    }
                    i++;
                    e += 0x2ec;
                }
            }
        }
    }
    d3_SetViewport((float)U_I((unsigned)&d2_ScreenW), (float)U_I((unsigned)&d2_ScreenH) - 150.0f, 0.0f, 75.0f, *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (float)U_I((unsigned)&d2_ScreenW), (float)U_I((unsigned)&d2_ScreenH));
    U_I((unsigned)&ai_Unkfb9060) = U_I((unsigned)&d2_ScreenW);
    U_I((unsigned)&ai_Unkfb906c) = U_I((unsigned)&d2_ScreenH) - 0x4b;
    U_I((unsigned)&ai_Unkfb9068) = 0;
    U_I((unsigned)&ai_Unkfb9064) = 0x4b;
    sv_SetPanelEnabled(0);
    cam3[0] = U_I(cam + 0x4c);
    cam3[1] = U_I(cam + 0x50);
    cam3[2] = U_I(cam + 0x54);
    save3[0] = U_I(cam + 0x84);
    save3[1] = U_I(cam + 0x88);
    save3[2] = U_I(cam + 0x8c);
    d3if_SelectScene(0, 0);
    if (U_I((unsigned)&d3_Sky))
        d3sky_Init(U_I((unsigned)&d3_Sky), 1);
    d3t_GetObjectBounds(a4, b4, U_I(rec + 0x61));
    if (U_I((unsigned)&ai_Unk925100) != U_I(rec + 1)) {
        U_I((unsigned)&ai_Unk925114) = 0;
        U_I((unsigned)&ai_Unk925118) = 0;
        U_I((unsigned)&ai_Unk92511c) = 0;
        U_I((unsigned)&ai_Unk925108) = save3[0];
        U_I(((unsigned)&ai_Unk925108 + 0x4)) = save3[1];
        U_I(((unsigned)&ai_Unk925108 + 0x8)) = save3[2];
        U_I((unsigned)&ai_Unk925100) = U_I(rec + 1);
    }
    d3_PointToWorld(U_I(rec + 0x61), U_I(rec + 0x61) + 0x4c, (float *)sc);
    *(int *)(sc + 0x10) = U_I((unsigned)&ai_Unk925108);
    *(int *)(sc + 0x14) = U_I(((unsigned)&ai_Unk925108 + 0x4));
    *(int *)(sc + 0x18) = U_I(((unsigned)&ai_Unk925108 + 0x8));
    *(float *)(sc + 4) = (*(float *)(sc + 0xe8) - *(float *)(sc + 0xf8)) * 0.8f + *(float *)(sc + 4);
    pp = 0;
    obj[3] = d3_CreateObject(0, (char *)(unsigned)&ai_Unk5ebba4, (char *)&pp, (char *)(unsigned)&full_data_62a350);
    obj[0] = d3_CreateObject(obj[3], (char *)(unsigned)&ai_Unk5ebba4, (char *)(unsigned)&ai_Unk5ebbb4, (char *)(unsigned)&full_data_62a340);
    obj[1] = d3_CreateObject(obj[3], (char *)(unsigned)&ai_Unk5ebba4, (char *)(unsigned)&ai_Unk5ebbb4, (char *)(unsigned)&full_data_62a328);
    obj[2] = d3_CreateObject(obj[3], (char *)(unsigned)&ai_Unk5ebba4, (char *)(unsigned)&ai_Unk5ebbb4, (char *)(unsigned)&full_data_62a310);
    sel = U_I((unsigned)&ai_Unk92511c);
    *(int *)(sc + 0x20) = 0;
    d3_SetObjectPos(obj[3], (float *)sc);
    d3_SetObjectRot(obj[3], sc + 0x10);
    item = d3_CreateObject(obj[3], (char *)((unsigned)&sv_PhaseMinutes + 0x1c8), (char *)(unsigned)&ai_Unk5ebbb4, (char *)(unsigned)&full_data_62a2fc);
    d3t_PlayAnim(item, (char *)(unsigned)&full_data_62a2dc, 1);
    *(unsigned char *)(item + 0x212) = (*(unsigned char *)(item + 0x212) & 0xf7) | 4;
    *(unsigned char *)(item + 0x212) |= 0x40;
    *(unsigned char *)(item + 0x211) &= 0xfd;
    d2_ClearSurface(U_I((unsigned)&d2_ScreenSurf));
    sv_RunFrame(0, 0x86);
    d2_ClearSurface(U_I((unsigned)&d2_ScreenSurf));
    d2_ReleaseFade(pb);
    d2_StartFade(0, 0, U_I((unsigned)&d2_ScreenW), U_I((unsigned)&d2_ScreenH), (char *)(unsigned)"BLACK", 0x5a, 10);
    ai_Unk580a90((float *)sc, run);
    ai_Unk57f890(rec, (float *)sc, run);
    if (*(int *)(sc + 0xdc) < sel)
        sel = *(int *)(sc + 0xdc);
    for (m = 0; m < 3; m++)
        d3if_Show((struct D3OBJECT *)obj[m], 0);
    d3if_Show((struct D3OBJECT *)obj[sel], 1);
    d3if_ShowTree(U_I(rec + 0x61), 0);
    he_Unk53ec10(U_I(rec + 0x61), 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    ai_Unk57f5d0((float *)sc);
    U_I((unsigned)&full_data_6f9fd0) = -1;
    U_I((unsigned)&full_data_6f9f98) = -1;
    U_I((unsigned)&full_data_6f9f60) = -1;
    while (sv_RunFrame(ai_Unk57e270, 0x2656c7) != 0) {
        sv_SetPanelEnabled(0);
        now = U_I((unsigned)&ai_ClockMs);
        dt = (float)(now - last) * 0.013513514f;
        if (U_I(run + 4) == 0 && (U_I(((unsigned)&dm_L + 0x20)) != 0 || (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_PointerObj) == *(int *)(sc + 0xbc)))) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else {
            ai_Unk57ec40((float *)sc, dt);
            ai_Unk57f0e0((float *)sc);
            ai_Unk57f890(rec, (float *)sc, run);
            ai_Unk57f690((float *)sc);
            if (U_I(run + 4) == 0) {
                if (U_I((unsigned)&d2_ClickedId) != -1) {
                    if (U_I((unsigned)&d2_PointerObj) == *(int *)(sc + 0xc8))
                        ai_Unk57fdd0(rec, (float *)sc, run, 0, 0x2656c7);
                    else if (U_I((unsigned)&d2_PointerObj) == *(int *)(sc + 0xcc))
                        ai_Unk57fdd0(rec, (float *)sc, run, 1, 0x2656c7);
                    else if (U_I((unsigned)&d2_PointerObj) == *(int *)(sc + 0xd0))
                        ai_Unk57fdd0(rec, (float *)sc, run, 2, 0x2656c7);
                }
                if (U_I(run + 4) == 0 && ai_Unk57f620((float *)sc) != 0) {
                    ai_Unk580160((float *)sc, rec);
                    U_I((unsigned)&full_data_6f9fd0) = -1;
                    U_I((unsigned)&full_data_6f9f98) = -1;
                    U_I((unsigned)&full_data_6f9f60) = -1;
                }
            }
        }
        last = now;
    }
    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    pb = (char *)d2_StartFade(0, 0, U_I((unsigned)&d2_ScreenW), U_I((unsigned)&d2_ScreenH), (char *)(unsigned)"BLACK", 0x5a, 1);
    while (!(*(unsigned char *)pb & 4))
        sv_RunFrame(0, 0x10080);
    d2_CloseView(*(int *)(sc + 0xa4));
    d2_CloseView(*(int *)(sc + 0xc0));
    d2_CloseView(*(int *)(sc + 0x74));
    d2_RemoveObj(*(int *)(sc + 0xb4));
    d2_RemoveObj(*(int *)(sc + 0xb8));
    for (m = 0; m < 4; m++)
        if (obj[m])
            d3if_Delete((struct D3OBJECT *)obj[m]);
    if (item)
        d3if_Delete((struct D3OBJECT *)item);
    d3if_ShowTree(U_I(rec + 0x61), 1);
    he_Unk53ec10(U_I(rec + 0x61), 1);
    for (m = 0; m < n; m++)
        *(int *)(U_I((unsigned)&d2_Objs) + 0x38 + list[m] * 0x2ec) = 0;
    d3_SetViewport((float)(U_I((unsigned)&sv_ViewRectRight) - U_I((unsigned)&sv_ViewRectLeft)), (float)(U_I((unsigned)&sv_ViewRectBottom) - U_I((unsigned)&sv_ViewRectTop)), (float)U_I((unsigned)&sv_ViewRectLeft), (float)U_I((unsigned)&sv_ViewRectTop), *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (float)U_I((unsigned)&d2_ScreenW), (float)U_I((unsigned)&d2_ScreenH));
    U_I((unsigned)&ai_Unkfb9068) = bg[0];
    U_I((unsigned)&ai_Unkfb9060) = bg[1];
    U_I((unsigned)&ai_Unkfb9064) = bg[2];
    U_I((unsigned)&ai_Unkfb906c) = bg[3];
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    d3if_SelectScene(bg5, 0);
    if (U_I((unsigned)&d3_Sky))
        d3sky_Init(U_I((unsigned)&d3_Sky), 1);
    d3_SetObjectPos(U_I((unsigned)&sv_CameraObj), (float *)cam3);
    d3_SetObjectRot(U_I((unsigned)&sv_CameraObj), (char *)save3);
    init_SetDetailLevel(U_I((unsigned)&ai_Unk633738), 0);
    d3if_Prerender(1);
    d2_ReleaseFade(pb);
    d2_StartFade(0, 0, U_I((unsigned)&d2_ScreenW), U_I((unsigned)&d2_ScreenH), (char *)(unsigned)"BLACK", 0x5a, 10);
    unk_Unk4eeee0(U_I(rec + 1), 0, 0);
}

double _CIfmod();

/* 0x57ec40, the cannon aiming update (1169 B): sc = the scene struct, dt = the frame time. Returns at once with
   sc + 0x7c set; the zoom sc + 0x20 moves by -dt * 1.6 (or +dt * 0.8 with the byte 0x14681d8) and is clamped to
   0 .. 1; the yaw sc + 0x14 and the pitch sc + 0x10 steer with the mouse: not dragging (0x1467390 = 0) -> the
   keys (0x1468288 / 0x1468290 the yaw, 0x146828b / 0x146828d the pitch, the screen 0xfb9070 / 0xfb9072 as the
   edges) drive the speeds sc + 0x54 / 0x58 (toward 0, or +/-1 at speed 0.25 / 0.8) into the deltas sc + 0x5c
   (* 1.5) / sc + 0x60 (* 0.8); dragging (0x1467390): inside the middle of the screen (0x50 .. 0xfb9070 - 0x50)
   the drag state sc + 0x88: 0 -> the start sc + 0x80 / sc + 0x84, the speeds zeroed; 1 -> the pitch -= (the
   old mouse y - sc + 0x84) * 0.008333334, the yaw -= (the mouse x - sc + 0x80) * 0.011111111; outside, or with
   the button released (the state 1 -> 0), the speeds fall toward 0. The pitch is clamped to -0.2617994 ..
   0.7853982 (the speed sc + 0x58 zeroed at the stop) and the yaw set from 0x5de73a (float args lost by Ghidra;
   the CRT atan-like helper) */
void ai_Unk57ec40(char *sc, float dt)
{
    float f;
    int a;
    int b;
    int m;
    short y;

    if (*(int *)(sc + 0x7c))
        return;
    if (*(char *)((unsigned)&kb_Down + 0x18) == 0)
        f = *(float *)(sc + 0x20) - dt * 1.6f;
    else
        f = dt * 0.8f + *(float *)(sc + 0x20);
    *(float *)(sc + 0x20) = f;
    if (1.0f < *(float *)(sc + 0x20) || 0.0f < *(float *)(sc + 0x20)) {
        if (*(float *)(sc + 0x20) <= 1.0f)
            f = *(float *)(sc + 0x20);
        else
            f = 1.0f;
    } else {
        f = 0.0f;
    }
    *(float *)(sc + 0x20) = f;
    y = *(short *)((unsigned)&dm_L + 0x2);
    if (*(int *)((unsigned)&dm_L + 0x10) == 0) {
        if (*(int *)(sc + 0x88) == 1) {
            *(int *)(sc + 0x88) = 0;
            goto clamp;
        }
    } else {
        if (y < 0x50 || (unsigned)&d2_ScreenH - 0x50 < y) {
            *(int *)(sc + 0x88) = 0;
            goto clamp;
        }
        if (*(int *)(sc + 0x88) == 0) {
            *(int *)(sc + 0x80) = *(short *)(unsigned)&dm_L;
            *(int *)(sc + 0x84) = y;
            *(int *)(sc + 0x88) = 1;
            *(float *)(sc + 0x60) = 0;
            *(float *)(sc + 0x5c) = 0;
            *(float *)(sc + 0x58) = 0;
            *(float *)(sc + 0x54) = 0;
            goto clamp;
        }
        if (*(int *)(sc + 0x88) != 1) {
            a = 0;
            b = 0;
            goto steer;
        }
        a = *(short *)(unsigned)&dm_L;
        b = *(int *)(sc + 0x80);
        *(int *)(sc + 0x80) = a;
        m = *(int *)(sc + 0x84);
        y = *(short *)((unsigned)&dm_L + 0x2);
        *(int *)(sc + 0x84) = y;
        *(float *)(sc + 0x10) = *(float *)(sc + 0x10) - (float)(y - m) * 0.008333334f;
        *(float *)(sc + 0x14) = *(float *)(sc + 0x14) - (float)(a - b) * 0.011111111f;
        goto done;
    }
steer:
    a = 0;
    b = 0;
    if (*(char *)((unsigned)&kb_Down + 0xc8) != 0 || y < 6)
        a = 1;
    if (*(char *)((unsigned)&kb_Down + 0xd0) != 0 || (int)y >= (unsigned)&d2_ScreenH - 5)
        a--;
    if (*(char *)((unsigned)&kb_Down + 0xcb) != 0 || *(short *)(unsigned)&dm_L < 6)
        b = 1;
    if (*(char *)((unsigned)&kb_Down + 0xcd) != 0 || (int)*(short *)(unsigned)&dm_L >= (unsigned)&d2_ScreenW - 10)
        b--;
    if (*(float *)(sc + 0x54) == 0.0f) {
        if (b != 0)
            goto pitch_speed;
    } else if (b == 0) {
        if (*(float *)(sc + 0x54) >= 0.0f) {
            *(float *)(sc + 0x54) -= dt * 0.8f;
            if (*(float *)(sc + 0x54) <= 0.0f)
                *(float *)(sc + 0x54) = 0.0f;
        } else {
            *(float *)(sc + 0x54) += dt * 0.8f;
            if (*(float *)(sc + 0x54) >= 0.0f)
                *(float *)(sc + 0x54) = 0.0f;
        }
    } else {
pitch_speed:
        if (b < 1) {
            f = *(float *)(sc + 0x54) <= 0.0f ? 0.25f : 0.8f;
            *(float *)(sc + 0x54) -= dt * f;
            if (*(float *)(sc + 0x54) <= -1.0f)
                *(float *)(sc + 0x54) = -1.0f;
        } else {
            f = *(float *)(sc + 0x54) >= 0.0f ? 0.25f : 0.8f;
            *(float *)(sc + 0x54) += dt * f;
            if (*(float *)(sc + 0x54) >= 1.0f)
                *(float *)(sc + 0x54) = 1.0f;
        }
    }
    *(float *)(sc + 0x5c) = dt * *(float *)(sc + 0x54) * 1.5f;
    if (*(float *)(sc + 0x58) == 0.0f) {
        if (a != 0)
            goto yaw_speed;
    } else if (a == 0) {
        if (*(float *)(sc + 0x58) >= 0.0f) {
            *(float *)(sc + 0x58) -= dt * 0.8f;
            if (*(float *)(sc + 0x58) <= 0.0f)
                *(float *)(sc + 0x58) = 0.0f;
        } else {
            *(float *)(sc + 0x58) += dt * 0.8f;
            if (*(float *)(sc + 0x58) >= 0.0f)
                *(float *)(sc + 0x58) = 0.0f;
        }
    } else {
yaw_speed:
        if (a < 1) {
            f = *(float *)(sc + 0x58) <= 0.0f ? 0.25f : 0.8f;
            *(float *)(sc + 0x58) -= dt * f;
            if (*(float *)(sc + 0x58) <= -1.0f)
                *(float *)(sc + 0x58) = -1.0f;
        } else {
            f = *(float *)(sc + 0x58) >= 0.0f ? 0.25f : 0.8f;
            *(float *)(sc + 0x58) += dt * f;
            if (*(float *)(sc + 0x58) >= 1.0f)
                *(float *)(sc + 0x58) = 1.0f;
        }
    }
    *(float *)(sc + 0x10) += *(float *)(sc + 0x60);
    *(float *)(sc + 0x14) += *(float *)(sc + 0x5c);
done:
clamp:
    if (*(float *)(sc + 0x10) < -0.2617994f || 0.7853982f < *(float *)(sc + 0x10)) {
        if (*(float *)(sc + 0x10) < -0.2617994f)
            f = -0.2617994f;
        else
            f = *(float *)(sc + 0x10);
        if (f >= 0.7853982f)
            f = 0.7853982f;
        *(float *)(sc + 0x10) = f;
        *(float *)(sc + 0x58) = 0;
    }
    *(float *)(sc + 0x14) = (float)_CIfmod();
}

void d3_MatTranspose(); float d3_VecAngle2(); int ai_GetDefaultEffect();

/* 0x57f0e0, the cannon camera update (947 B, ROUGH: the placement calls and the aiming / recoil maths are
   written, the matrix details are comments): sc = the scene struct; the camera position sc + 0x90 / the object
   sc + 0x98 / the wimpel sc + 0x9c handles; the screen 0xfb9070 / 0xfb9072; the view 0xfc2dec. The three object
   handles sc + 0x8c / 0x90 / 0x94 get their positions through 0x43b860 (0x54560 the rotation); the mouse
   0x475980 (sc + 0xb4) * 0.01 as the speed; the target position from the object matrix (the 0x18c / 0x194 /
   entries * 30), 0x54560; the camera placement 0x43b790 (0xfc2dec, the position) / 0x43b860 (0xfc2dec, the
   angle); the recoil sc + 0x78 / sc + 0x7c timer with the fade 0x54560 and the wimpel at -70 / -50 / 120;
   0x43b790 (sc + 0x98, sc) as the default */
void ai_Unk57f0e0(char *sc)
{
    float pos[3];
    float ang[3];
    float t[16];   /* t is a D3MATRIX: d3_MatTranspose writes 0x40 bytes */
    float f1;
    float f2;
    int m;
    int i;
    int h;

    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    d3_SetObjectRot(*(int *)(sc + 0x98), (char *)pos);
    pos[0] = *(float *)(sc + 0x10);
    pos[1] = 0;
    pos[2] = 0;
    for (i = 0; i < 3; i++)
        d3_SetObjectRot(*(int *)(sc + 0x8c + i * 4), (char *)pos);
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    t[3] = 0;
    h = ai_GetDefaultEffect();
    d3_MatTranspose(*(int *)(sc + 0x98) + 0x18c, (char *)t);
    ang[0] = 0;
    ang[1] = 0;
    ang[2] = (float)(d3_VecAngle2((char *)(unsigned)&ai_AxisPair, h) - 1.5707964f) - *(float *)(sc + 0x14);
    d3_SetObjectRot(*(int *)(sc + 0x9c), (char *)ang);
    if (*(float *)(sc + 0x7c) == 0.0f)
        ai_Unk57f5d0(sc);
    /* the object's forward vector (the matrix +0x18c / +0x19c / +0x1ac, +0x194 / +0x1a0 / +0x1b0, +0x194 /
       +0x1a4 / +0x1b4 at * 30) -> 0x54560 -> the camera offset from sc + 0x10 / 0x14 / 0x18, the mouse x *
       0.01 as the speed and the pitch sc + 0x10 scaled */
    f1 = *(float *)(sc + 0x10);
    f2 = (f1 - -0.2617994f) / 1.0471976f;
    m = d2_GetObjValue(*(int *)(sc + 0xb4));
    d3_SetObjectPos(U_I((unsigned)&sv_CameraObj), (char *)pos);
    d3_SetObjectRot(U_I((unsigned)&sv_CameraObj), (char *)ang);
    /* the recoil: sc + 0x78 (timer) / sc + 0x7c (active): the fade 0x54560, the camera pushed back by the
       object direction * 50 * the fade, the wimpel to -70 / -50 / 120, else reset and 0x43b790 (sc + 0x9c,
       0x5eec80) / 0x43b790 (sc + 0x98, sc) */
    if (*(float *)(sc + 0x7c) != 0.0f) {
        if (*(float *)(sc + 0x78) < *(float *)(sc + 0x6c) + 0x54) {
            *(float *)(sc + 0x7c) = 0;
        } else {
            *(float *)(sc + 0x7c) = 0;
            *(float *)(sc + 0x78) = 0;
            d3_SetObjectPos(*(int *)(sc + 0x9c), (char *)((unsigned)&sv_PhaseMinutes + 0x1c8));
        }
    }
    d3_SetObjectPos(*(int *)(sc + 0x98), sc);
}

void d3_DirToWorld();
int gm_FindById();

/* 0x57f5d0, copies three vectors of the cannon object (55 B): 0x458810 (sc + 0x8c, 0x5ebb74, sc + 0x24),
   (0x5ebb94, sc + 0x44) and (0x5ebb84, sc + 0x34) */
void ai_Unk57f5d0(char *sc)
{
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)(unsigned)&ai_AxisPair, sc + 0x24);
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)(unsigned)&d3_YAxis, sc + 0x44);
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)((unsigned)&ai_AxisPair + 0x10), sc + 0x34);
}
/* 0x57f620, the fire check (74 B): sc + 0x7c clear and the zoom sc + 0x20 <= 0.01: the fire button sc + 0xb8
   clicked (0xf8a668 / 0x5fe710) or the shot pending 0x14681f2 -> 0x14681f2 = 0, the timer sc + 0x78 = sc + 0x6c,
   the recoil sc + 0x7c = 1; 1, else 0 */

int ai_Unk57f620(char *sc)
{
    if (*(int *)(sc + 0x7c) == 0 && *(float *)(sc + 0x20) <= 0.01f) {
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&d2_PointerObj) == *(int *)(sc + 0xb8)) {
            *(char *)((unsigned)&kb_Down + 0x32) = 0;
            *(int *)(sc + 0x78) = *(int *)(sc + 0x6c);
            *(int *)(sc + 0x7c) = 1;
            return 1;
        }
        if (*(char *)((unsigned)&kb_Down + 0x32) != 0) {
            *(char *)((unsigned)&kb_Down + 0x32) = 0;
            *(int *)(sc + 0x78) = *(int *)(sc + 0x6c);
            *(int *)(sc + 0x7c) = 1;
            return 1;
        }
    }
    return 0;
}
/* 0x57f690, the shot result table (260 B): every 0x15e ticks (0x6333cc vs sc + 0x70) the three 0x38-byte records
   of 0x6f9f60 (rec 0 .. 2, the record index in the first dword; three 0x10-byte entries each: the object 0x4fea50
   (0, 0, the id), the flag +8, the value +0xc) are turned into the texts 0x406c / 0x406d (the character word +0x27
   of the object, 0xffff / 0x6f1bf8 = the playerless case) with 0x4071 / 0x4072 (<= 25) / 0x4073 and the name block
   0x602568 / 0x60c6c0; the first record with any entry: 0x6f2664 = 0, its index = -1, 0x475590 (sc + 0x74, 1);
   with 0x6f2664 set but no entry: the texts 0x602568 / 0x406e, the same; none at all: 0x475590 (sc + 0x74, 0) and
   0x6f2664 = 0 */

void ai_Unk57f690(char *sc)
{
    int n;
    int rec;
    int i;
    int h;
    int v;
    unsigned short w;

    n = 0;
    if ((unsigned int)(*(int *)(sc + 0x70) + 0x15e) <= (unsigned int)U_I((unsigned)&ai_ClockMs)) {
        *(int *)(sc + 0x70) = U_I((unsigned)&ai_ClockMs);
        d2_SetForm(*(int *)(sc + 0x74), 0);
        for (rec = 0; rec < 3; rec++) {
            i = 0;
            if (*(int *)((unsigned)&full_data_6f9f60 + rec * 0x38) == rec) {
                U_I((unsigned)&full_data_6f2664) = 1;
                for (i = 0; i < 3; i++) {
                    h = 0;
                    gm_FindById(&h, 0, 0, *(int *)((unsigned)&full_data_6f9f68 + rec * 0x38 + i * 0x10));
                    if (h == 0)
                        break;
                    v = 0x4071;
                    if (*(int *)((unsigned)&full_data_6f9f70 + rec * 0x38 + i * 0x10) == 1) {
                        v = 0x4073;
                        if (*(float *)((unsigned)&full_data_6f9f74 + rec * 0x38 + i * 0x10) <= 25.0f)
                            v = 0x4072;
                    }
                    io_PrintText(i == 0 ? (char *)(unsigned)"$C" : (char *)(unsigned)&ai_Unk60c6c0);
                    w = *(unsigned short *)(h + 0x27);
                    if (w == 0xffff || (char *)(unsigned)&ai_Chars + w * 0x21c == (char *)U_I((unsigned)&ai_PlayerChar))
                        io_PrintText(0x406c, h, v);
                    else
                        io_PrintText(0x406d, (int)w, h, v);
                    n++;
                }
                if (n != 0) {
                    U_I((unsigned)&full_data_6f2664) = 0;
                    *(int *)((unsigned)&full_data_6f9f60 + rec * 0x38) = -1;
                    d2_SetViewEnabled(*(int *)(sc + 0x74), 1);
                    return;
                }
                if (U_I((unsigned)&full_data_6f2664) != 0) {
                    U_I((unsigned)&full_data_6f2664) = 0;
                    *(int *)((unsigned)&full_data_6f9f60 + rec * 0x38) = -1;
                    io_PrintText((char *)(unsigned)"$C");
                    io_PrintText(0x406e);
                    d2_SetViewEnabled(*(int *)(sc + 0x74), 1);
                    return;
                }
            }
        }
        d2_SetViewEnabled(*(int *)(sc + 0x74), 0);
        U_I((unsigned)&full_data_6f2664) = 0;
    }
}

float unk_Unk4eef50(); void unk_Unk4ef170(char *o, unsigned *a, unsigned *b); int unk_Unk4ef090(); float ai_GetDefaultEffectScale();
int _ftol(); float ai_Unk57fdb0(); int d2_CreateWinText(); void d2_SetObjFont();  /* @original: the call at 4110 passes 1 argument to d2.h's 4-parameter prototype */
void txt_Format(char *out, int id0, ...); int gm_Unk509ee0(char *c, char p);

/* 0x57f890, the cannon control panel (383 B, ROUGH: the calls and the window-table stores are written; the three
   float -> int conversions (0x5de518 = __ftol) lost their arguments in the pseudocode): rec, sc = the scene
   struct, run = the tutorial step. The two fields sc + 0xb0 / 0xb4 read by 0x475980; the max 0x4eef50 (a, b, 1);
   the selection 0x4ef170 (rec, &sc + 0xdc, &sc + 0xe0), sc + 0xb0 set to (sc + 0xe0, MIN (the value, a)); the
   controls sc + 0xcc / 0xd0 enabled by the selection > 0 / > 1; the selected object (sc + 0xa0) checked in the
   window table 0xf8f080 (+0x2c and +0x28 = 1 for the selected id sc + 0xc8 / 0xcc / 0xd0, 0 for the others);
   0x4ef090 (the character 0x770b80 + 0x6f25b8 * 0x21c, the two values, sc + 0xa0 + 1, &l1, &l2); the fire control
   sc + 0xb8 enabled by the zoom sc + 0x20 = 0.01, 0x4ef090 and the run's cost sc + 8 + sc + 0xa0 * 4 against
   0x4eef50 and the run's flag +4 & 2; sc + 0xbc enabled by the run's flag +4 bit 1 inverted; the three bars
   sc + 0xa8 / 0xac / 0xc4 (0x475cb0, 0 .. 100, 0x5dbc20 / 0x57fdb0 / 1 through 0x5de518) and sc + 0xc0 (2);
   0x515960 (0x602568); the four windows 0x473650 (0x4144a360 / 0x4144a364 texts, the "__T__a" 0x62a364 format
   through 0x5176d0 and 0x509ee0, the screen width 0xf8f128 + 0x6ed938 * 0x3c4 / 2) placed with +0x5c = 1 and
   +0x14 = the width / 2, 0x4760b0 (0x54); the globals 0x925114 / 18 / 1c / 08 / 0c / 10 updated */
void ai_Unk57f890(char *rec, char *sc, char *run)
{
    char buf[0x40];
    int a;
    int b;
    int c;
    int m;
    int n;
    int l1;
    int l2;
    int d;
    int w;

    a = d2_GetObjValue(*(int *)(sc + 0xb0));
    b = d2_GetObjValue(*(int *)(sc + 0xb4));
    c = (int)unk_Unk4eef50(a, b, 1);
    unk_Unk4ef170(rec, (int *)(sc + 0xdc), (int *)(sc + 0xe0));
    n = *(int *)(sc + 0xe0);
    if (n < a)
        a = n;
    d2_SetObjValue(*(int *)(sc + 0xb0), 0, n, a);
    d2_SetObjEnabled(*(int *)(sc + 0xcc), 0 < *(int *)(sc + 0xdc));
    d2_SetObjEnabled(*(int *)(sc + 0xd0), 1 < *(int *)(sc + 0xdc));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + *(int *)(sc + 0xc8) * 0x2ec) = *(int *)(sc + 0xa0) == 0;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + *(int *)(sc + 0xcc) * 0x2ec) = *(int *)(sc + 0xa0) == 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + *(int *)(sc + 0xd0) * 0x2ec) = *(int *)(sc + 0xa0) == 2;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + *(int *)(sc + 0xc8) * 0x2ec) = *(int *)(sc + 0xa0) == 0;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + *(int *)(sc + 0xcc) * 0x2ec) = *(int *)(sc + 0xa0) == 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + *(int *)(sc + 0xd0) * 0x2ec) = *(int *)(sc + 0xa0) == 2;
    m = unk_Unk4ef090((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, a, b, *(int *)(sc + 0xa0) + 1, &l1, &l2);
    if (*(float *)(sc + 0x20) > 0.01f || m == 0 || *(float *)(run + 8 + *(int *)(sc + 0xa0) * 4) < (float)c || (*(unsigned char *)(run + 4) & 2) != 0)
        w = 0;
    else
        w = 1;
    d2_SetObjEnabled(*(int *)(sc + 0xb8), w);
    d2_SetObjEnabled(*(int *)(sc + 0xbc), (~*(unsigned int *)(run + 4) >> 1) & 1);
    d2_SetObjValue(*(int *)(sc + 0xa8), 0, 100, (int)ai_GetDefaultEffectScale(1));
    ai_Unk57fdb0(sc, 1);
    d2_SetObjValue(*(int *)(sc + 0xac), 0, 100, (int)_ftol());
    d2_SetObjValue(*(int *)(sc + 0xc4), 0, 100, (int)_ftol());
    d2_SetForm(*(int *)(sc + 0xc0), 2);
    io_PrintText((char *)(unsigned)"$C");
    m = d2_CreateWinText(0, 0xf, (char *)U_I((unsigned)&full_data_144a360), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + m * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + m * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2;
    d2_SetObjFont(m, 0x54);
    txt_Format(buf, (char *)(unsigned)&full_data_62a364, l2, l1);
    m = d2_CreateWinText(0, 0x23, buf, U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + m * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + m * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2;
    d2_SetObjFont(m, 0x54);
    m = d2_CreateWinText(*(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2, 0xf, (char *)U_I((unsigned)&full_data_144a364), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + m * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + m * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2;
    d2_SetObjFont(m, 0x54);
    d = gm_Unk509ee0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    txt_Format(buf, (char *)(unsigned)&full_data_62a364, d, *(int *)(((unsigned)&ai_Selected + 0xc) + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
    m = d2_CreateWinText(*(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2, 0x23, buf, U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + m * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + m * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2;
    d2_SetObjFont(m, 0x54);
    U_I((unsigned)&ai_Unk925114) = a;
    U_I((unsigned)&ai_Unk925118) = b;
    U_I((unsigned)&ai_Unk92511c) = *(int *)(sc + 0xa0);
    U_I((unsigned)&ai_Unk925108) = *(int *)(sc + 0x10);
    U_I(((unsigned)&ai_Unk925108 + 0x4)) = *(int *)(sc + 0x14);
    U_I(((unsigned)&ai_Unk925108 + 0x8)) = *(int *)(sc + 0x18);
}
/* 0x57fdb0, the cannon's pitch as 0 .. 1 (28 B): (sc + 0x10 + 0.2617994) / (0.7853982 + 0.2617994) */

float ai_Unk57fdb0(char *sc)
{
    return (*(float *)(sc + 0x10) - -0.2617994f) / (0.7853982f - -0.2617994f);
}
/* 0x57fdd0, the shot animation (587 B): rec, sc = the scene struct, run = the tutorial step, sel = the object
   (0 .. 2), mask = the pump mask; an unchanged selection (sc + 0x28) or out of range answers at once; two
   90-tick phases, each pumping 0x5309a0 (this function, mask) and calling 0x57f890 (rec, sc, run): the camera
   sc + 0x26 pushed back by the object's direction sc + 0x24 (0x454560) * 100 * the phase (the clock - the
   start) / 90 and the wimpel sc + 0x27 to -70 / -50 / 100 * the phase + 120 (0x43b790), the second phase with
   1 - the time, the clock 0x6333cc reset after each; then the three objects sc + 0x8c / 0x90 / 0x94 switched
   off (0x471b0) and the selected one on, sc + 0x28 = the selection, sc + 0x1b = the clock */


void ai_Unk57fdd0(char *rec, float *sc, char *run, float sel, int mask)
{
    float v[3];
    float w[3];
    float f;
    float start;
    int i;
    int h;

    start = (float)U_I((unsigned)&ai_ClockMs);
    if (sel != sc[0x28] && (int)sel <= (int)sc[0x37]) {
        while ((h = sv_RunFrame(ai_Unk57fdd0, mask)) != 0) {
            ai_Unk57f890(rec, (char *)sc, run);
            if ((float)(unsigned int)(U_I((unsigned)&ai_ClockMs) - (int)start) <= 90.0f) {
                v[0] = sc[9];
                v[2] = sc[0xb];
                f = (float)(unsigned int)(U_I((unsigned)&ai_ClockMs) - (int)start) / 90.0f;
                v[1] = 0;
                d3_Normalize((char *)v);
                w[0] = -70.0f;
                w[1] = -50.0f;
                w[2] = f * 100.0f + 120.0f;
                v[0] = *sc - v[0] * (f * 100.0f);
                v[1] = sc[1] - (f * 100.0f) * v[1];
                v[2] = sc[2] - (f * 100.0f) * v[2];
                d3_SetObjectPos(*(int *)&sc[0x26], (char *)v);
                d3_SetObjectPos(*(int *)&sc[0x27], (char *)w);
            } else {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        for (i = 0; i < 3; i++)
            d3if_Show(*(struct D3OBJECT **)&sc[0x23 + i], 0);
        d3if_Show(*(struct D3OBJECT **)&sc[0x23 + (int)sel], 1);
        sc[0x28] = sel;
        start = (float)U_I((unsigned)&ai_ClockMs);
        while ((h = sv_RunFrame(ai_Unk57fdd0, mask)) != 0) {
            ai_Unk57f890(rec, (char *)sc, run);
            if ((float)(unsigned int)(U_I((unsigned)&ai_ClockMs) - (int)start) <= 90.0f) {
                v[0] = sc[9];
                v[2] = sc[0xb];
                v[1] = 0;
                f = 1.0f - (float)(unsigned int)(U_I((unsigned)&ai_ClockMs) - (int)start) / 90.0f;
                d3_Normalize((char *)v);
                w[0] = -70.0f;
                w[1] = -50.0f;
                w[2] = f * 100.0f + 120.0f;
                v[0] = *sc - v[0] * (f * 100.0f);
                v[1] = sc[1] - (f * 100.0f) * v[1];
                v[2] = sc[2] - (f * 100.0f) * v[2];
                d3_SetObjectPos(*(int *)&sc[0x26], (char *)v);
                d3_SetObjectPos(*(int *)&sc[0x27], (char *)w);
            } else {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        *(int *)((char *)sc + 0x6c) = U_I((unsigned)&ai_ClockMs);
    }
}

void unk_Unk4ef260(char *o, float *a, float *b); float unk_Unk4ef330(); void ai_Unk580a50(); int ai_InitShot(struct SHOTREC *shot, struct D3VECTOR *pos, struct D3VECTOR *dir, float charge, float size, float density);
int ai_ScatterShot(struct SHOTREC *shot, float sideSpread, struct D3VECTOR *side, float otherSpread, struct D3VECTOR *other, float speedSpread); int unk_Unk4eeb70(char *o, int *c, int *a, int *b, int *d, float v, int n, int x); struct SBBANK; struct SBSAMPLE *sb_PlayVariation(struct SBBANK *bank, char *name, int idx, int loops, int pan, int volume, int fade); void ai_Unk580a90();

/* 0x580160, the cannon's explosion spawn (1173 B, ROUGH: the branch structure and the effect calls are written,
   the matrix arithmetic is a comment): sc = the scene struct, rec = the record. The two fields sc + 0xb0 / 0xb4
   through 0x475980 and the selected object (sc + 0xa0) + 1 checked by 0x4ef090 (the character 0x770b80 +
   0x6f25b8 * 0x21c); 0x4ef260 (rec) / 0x4ef330 (rec) and the per-object table 0x5edfe0 / 0x5edfe4 / 0x5edfe8
   + the field * 0xc give the arc; three explosion records (by the selection 0, 1, 2): one (0x5dc240 / 0x5dc3a0
   matrices, 0x4eeb70 the objects, 1 record), two (two records) or three (local_68 / local_d0 / local_138, three
   records), 0x4eef50 the data; the run waited out on 0x4c3ae0 (pump 0x20007); then 0x4287b0 ("LOCATIONS_
   DOMIZIL_KANONE1_esc" 0x62a36c) and 0x429ab0 (4, the counts, the offsets, the wimpel hand sc + 0x23 / 0x24 /
   0x25) for each record and the effect 0x427050 (0, "Explosion" 0x5f36e0, the record count, 1, 0x3f, 0x7f, 0) */
void ai_Unk580160(char *sc, char *rec)
{
    int v1;
    int v2;
    int sel;
    int i;
    int h;
    int a;
    int b;
    int c;

    v1 = d2_GetObjValue(*(int *)(sc + 0xb0));
    v2 = d2_GetObjValue(*(int *)(sc + 0xb4));
    sel = (int)*(float *)(sc + 0xa0);
    h = unk_Unk4ef090((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, v1, v2, sel + 1, &a, &b);
    if (!h)
        return;
    unk_Unk4ef260(rec, &a, &b);
    c = (int)unk_Unk4ef330(rec);
    ai_Unk580a50();
    /* the per-selection explosion records: 1 (selection 0), 2 (selection 1: local_d0 + local_138) or 3
       (selection 2: local_68 + local_d0 + local_138) of 0x68-byte matrices from 0x5dc240 (the corner positions
       of the selected gun, the arc fVar4 from the table 0x5edfe0 + v1 * 0xc) and 0x5dc3a0 (the placement); the
       object list 0x770b80 + 0x6f25b8 * 0x21c through 0x4eeb70 with the 0x4eef50 value; the run waited out on
       0x4c3ae0 (pump 0x20007). The three spawns: 0x4287b0 ("LOCATIONS_DOMIZIL_KANONE1_esc"), 0x429ab0 (4, the
       offsets 0 / 0x28 / 0x28, the wimpel hand sc + 0x23 + sel) and 0x427050 (0, "Explosion", the record
       count, 1, 0x3f, 0x7f, 0) */
    /* ROUGH: the three branch bodies (the 1 / 2 / 3 explosion records through 0x5dc240 / 0x5dc3a0 / 0x4eeb70
       and the run wait 0x4c3ae0 / 0x20007) are not written yet; only the last spawn (selection 0) is below */
    {
        int u10 = (int)evt_Load((char *)(unsigned)&full_data_62a36c);
        evt_RunScriptArgs((EVTSCRIPT *)u10, 4, 0, 0, 0x28, *(int *)(sc + 0x8c + (sel + 1) * 4));
    }
    sb_PlayVariation(0, (char *)(unsigned)"Explosion", sel + 1, 1, 0x3f, 0x7f, 0);
}

typedef struct SHOTENTRY {   /* 0x10 bytes */
    int obj;                 /* +0x0, -1 when empty */
    float dist;              /* +0x4 */
    int a;                   /* +0x8 */
    int b;                   /* +0xc */
} SHOTENTRY;

typedef struct SHOTREC {     /* 0x38 bytes, three of them at 0x6f9f60 (full_data_6f9f60) */
    int id;                  /* +0x0, -1 when empty */
    int unk4;                /* +0x4 */
    SHOTENTRY e[3];          /* +0x8 */
} SHOTREC;

/* 0x580a50, resets the three 0x38-byte shot records of 0x6f9f60 (57 B): each record: +0 = -1 and its three
   0x10-byte entries: +8 = -1, +0xc = 100000.0, +0x10 = 0, +0x14 = 0 */
void ai_Unk580a50(void)
{
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        ((SHOTREC *)&full_data_6f9f60)[i].id = -1;
        for (j = 0; j < 3; j++) {
            ((SHOTREC *)&full_data_6f9f60)[i].e[j].obj = -1;
            ((SHOTREC *)&full_data_6f9f60)[i].e[j].dist = 100000.0f;
            ((SHOTREC *)&full_data_6f9f60)[i].e[j].a = 0;
            ((SHOTREC *)&full_data_6f9f60)[i].e[j].b = 0;
        }
    }
}

int d2_CreatePicObj(); void d2_MoveObj(short x, short y, int obj); void d2_PlaceMainWindow(); int d2_WinOpenPic(short x, short y, int pic, int win);
int d2_WinOpenSlider(); void d2_SetLabel(); void d2_SetList(int id, int n, ...);

/* 0x580a90, builds the cannon control panel (1042 B, ROUGH: all the calls are written; the screen layout value
   of 0x46cda0 (the CONCAT22 in the pseudocode) is a guess from the short 0xf8f124 + 0x6ed938 * 0x3c4): sc = the
   scene struct. The bottom / top windows "Locations_domizil_cannon_bottom" (0x62a3cc) and "_top" (0x62a3ac)
   with 0x475850 (1) / 0x475f70, the "res" window (0x62a38c) with 0x475850 (3) / 0x475590 (0); the two window
   bases 0x46cd80 (the ids 0x7cb / 0x7cc / 0x7cd by the width 800 / 0x400 / other and the height, 0x6ed918 +
   0x52 + the id * 0x54) into sc + 0xd4 / 0xd8; the four buttons 0x4731d0 (0x59 / 0x9b / 0x14 / 0x56 / 0x98, the
   texts 0x7d5 / 0x7d4 / 0x7d3 / 0x7d1 / 0x7d2) into sc + 0xb8 / 0xbc / 0xc8 / 0xcc / 0xd0 with the window table
   0xf8f080 +0x4c = 1 and +0x1c0 = 3; the two sliders 0x46a960 (4 / 100, the values 0x925114 / 0x925118) into
   sc + 0xb0 / 0xb4 with 0x46bc70 / 0x46bca0; the three bars 0x46a960 (0x7d / 0x4c2) into sc + 0xa8 / 0xac /
   0xc4 (0x5dbc20 and two __ftol 0x5de518 values) with 0x46bc70; sc + 0x70 = the clock 0x6333cc */
void ai_Unk580a90(char *sc)
{
    int x;
    int mid;
    int id;
    int v;
    int w;

    x = U_I((unsigned)&d2_ScreenH);
    mid = (x + ((int)U_I((unsigned)&d2_ScreenW) >> 4) * -9) >> 1;
    if (U_I((unsigned)&d2_ScreenW) == 800) {
        v = 0x7cb;
        w = 0x7ce;
    } else if (U_I((unsigned)&d2_ScreenW) == 0x400) {
        v = 0x7cc;
        w = 1999;
    } else {
        v = 0x7cd;
        w = 2000;
    }
    v = d2_CreatePicObj(0, 0, v);
    *(int *)(sc + 0xd4) = v;
    v = d2_CreatePicObj(0, U_I((unsigned)&d2_ScreenH) - *(short *)((unsigned)&d2_Pics + 0x52 + w * 0x54), w);
    *(int *)(sc + 0xd8) = v;
    v = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3cc);
    *(int *)(sc + 0xa4) = v;
    d2_PlaceMainWindow(v, 1);
    d2_SetForm(*(int *)(sc + 0xa4), 0);
    d2_MoveObj(*(unsigned short *)(((unsigned)&d2_Wins + 0x4) + U_I((unsigned)&d2_LastWindow) * 0x3c4), (x - mid) - 0x19, *(int *)(((unsigned)&ai_Unkf8f13c + 0x250) + U_I((unsigned)&d2_LastWindow) * 0x3c4));
    d2_SetForm(*(int *)(sc + 0xa4), 2);
    id = d2_WinOpenPic(0x59, 0xf, 0x7d5, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xb8) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xb8) * 0x2ec) = 3;
    id = d2_WinOpenPic(0x9b, 0xf, 0x7d4, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xbc) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xbc) * 0x2ec) = 3;
    d2_SetForm(*(int *)(sc + 0xa4), 1);
    v = d2_WinOpenSlider(0x140, 0x14, 100, 0, 100, 0x7b, 0x82, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xb0) = v;
    d2_SetObjValue(v, 0, 4, U_I((unsigned)&ai_Unk925114), 1);
    d2_SetLabel(*(int *)(sc + 0xb0), U_I((unsigned)&full_data_144a344));
    d2_SetList(*(int *)(sc + 0xb0), 5, U_I((unsigned)&full_data_144a34c), U_I((unsigned)&full_data_144a350), U_I((unsigned)&full_data_144a354), U_I((unsigned)&full_data_144a358), U_I((unsigned)&full_data_144a35c));
    v = d2_WinOpenSlider(0x140, 0x32, 100, 0, 100, 0x7b, 0x82, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xb4) = v;
    d2_SetObjValue(v, 0, 100, U_I((unsigned)&ai_Unk925118), 1);
    d2_SetLabel(*(int *)(sc + 0xb4), U_I((unsigned)&full_data_144a348));
    id = d2_WinOpenPic(0x14, 0x17, 0x7d3, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xc8) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xc8) * 0x2ec) = 3;
    id = d2_WinOpenPic(0x56, 0x17, 0x7d1, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xcc) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xcc) * 0x2ec) = 3;
    id = d2_WinOpenPic(0x98, 0x17, 0x7d2, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xd0) = id;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x4c + id * 0x2ec) = 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x1c0 + *(int *)(sc + 0xd0) * 0x2ec) = 3;
    v = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3ac);
    *(int *)(sc + 0xc0) = v;
    d2_PlaceMainWindow(v, 1);
    d2_SetForm(*(int *)(sc + 0xc0), 0);
    d2_MoveObj(*(unsigned short *)(((unsigned)&d2_Wins + 0x4) + U_I((unsigned)&d2_LastWindow) * 0x3c4), mid - 0x4b, *(int *)(((unsigned)&ai_Unkf8f13c + 0x250) + U_I((unsigned)&d2_LastWindow) * 0x3c4));
    d2_SetForm(*(int *)(sc + 0xc0), 1);
    v = d2_WinOpenSlider(0x96, 0xb, 200, 0, 100, 0x75, 0x4c2, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xa8) = v;
    d2_SetObjValue(v, 0, 100, (int)ai_GetDefaultEffectScale(1));
    d2_SetLabel(*(int *)(sc + 0xa8), U_I((unsigned)&full_data_144a338));
    v = d2_WinOpenSlider(0x96, 0x1f, 200, 0, 100, 0x75, 0x4c2, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xac) = v;
    d2_SetObjValue(v, 0, 100, (int)ai_GetDefaultEffectScale(1));
    d2_SetLabel(*(int *)(sc + 0xac), U_I((unsigned)&full_data_144a33c));
    v = d2_WinOpenSlider(0x96, 0x33, 200, 0, 100, 0x75, 0x4c2, U_I((unsigned)&d2_LastWindow));
    *(int *)(sc + 0xc4) = v;
    d2_SetObjValue(v, 0, 100, (int)_ftol());
    d2_SetLabel(*(int *)(sc + 0xc4), U_I((unsigned)&full_data_144a340));
    v = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a38c);
    *(int *)(sc + 0x74) = v;
    d2_PlaceMainWindow(v, 3);
    d2_SetViewEnabled(*(int *)(sc + 0x74), 0);
    *(int *)(sc + 0x70) = U_I((unsigned)&ai_ClockMs);
}

int he_Unk56f170(); int sv_MessageBox();

/* 0x580fe0, a scene check (49 B): 0x56f170 (the record + 1); when it answers non-zero, 0x521150 (0x144a19c,
   0); 1 when 0x56f170 answered 0, else 0 */
int ai_Unk580fe0(char *rec)
{
    int r;

    r = he_Unk56f170(U_I(rec + 1));
    if (r)
        sv_MessageBox(U_I((unsigned)&full_data_144a19c), 0);
    return r == 0;
}

int gm_GetWorth(unsigned short id); int d2_WinOpenSlider(); int sv_CheckGoodAmount(); short * gm_PlayerRecord(char *c);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, int c, float f, int *out); int snd_PlayVoice(int id, int bank, char *text, int var); void sv_CameraFollow();

/* 0x581020, the spy / comment window (981 B, ROUGH: the three amounts v[0..2] are three float -> int
   conversions (0x5de518 = __ftol) whose FPU arguments Ghidra lost, they are 0 here): arg 1 = the record; the
   window "Locations_Domizil_kabinett_spy" (0x62a3f8) with 0x476f80 (0, 0) / 0x475600, the sound 0x52ea30
   (0x6ed938), a slider 0x46a960 (0x57, 0x14, 200, 0 .. 100, 0x7b, 2) with 0x46bca0 and 3 steps, the two entries
   0x475b80 (window, 3, text 0x4024 / +1), 0xf8a668 = -1; the pump loop 0x5309a0 (this handler, 0x677c7): the
   slider's step 0x475980, the texts 0x602568 / 0x401e / 0x401c / 0x401f, the click 0x14673a0 -> 0x6f22a0 = 1;
   the first entry: 0x521040 (the character 0x770b80 + 0x6f25b8 * 0x21c, the amount, 0x6f1bc5), 0x580fe0 (rec),
   0x506430 (the character) +0x28 += the amount, 0x4c4830 (-1, the character, 0x6f1bc5, the amount) and the
   message 0xf8 bytes (type 0x99 at +4, 2 at +0x36, the character 0x770b84 at +8, the record + 1 at +0xc, the game
   time 0x770ae0 at +0x28, -1 at +0x60 / +0x64, 3 at +0x68, the record + 1 at +0x5c, the amount at +0x6c, 0x4a63f0
   (the character, 0, 0x7b4c4a0, 0, 0, 100.0, &the value at +0x58)) and 0x4c2760 (-7, 0, "KOMMENTARE" 0x62a3ec, 0)
   then 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x52d7a0 () */
void ai_Unk581020(char *rec)
{
    char msg[0xf8];
    int v[3];
    int win;
    int slider;
    int id;
    int i;
    int r;
    int b1;
    int b2;
    int lv;

    gm_GetWorth(U_I((unsigned)&ai_PlayerId));
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3f8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 2);
    id = d2_CreateWinText(0, 3, (char *)U_I((unsigned)&full_data_144a240), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + id * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    d2_SetObjFont(id, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 200, 0, 100, 0x7b, 2, U_I((unsigned)&d2_LastWindow));
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, U_I((unsigned)&full_data_144a244), U_I((unsigned)&full_data_144a248), U_I((unsigned)&full_data_144a24c));
    d2_SetForm(win, 3);
    i = io_PrintText(0x4024);
    b1 = d2_GetWinObj(win, 3, i);
    b2 = d2_GetWinObj(win, 3, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText(0x401e, 0x401c);
        id = v[i];
        io_PrintText(0x401f, id);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, id, *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0) {
                    if (ai_Unk580fe0(rec) != 0) {
                        r = gm_PlayerRecord((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
                        if (r != 0)
                            *(int *)(r + 0x28) += id;
                        cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, id);
                        memset(msg, 0, 0xf8);
                        *(unsigned char *)(msg + 0x04) = 0x99;
                        *(unsigned char *)(msg + 0x36) = 2;
                        *(int *)(msg + 0x60) = -1;
                        *(int *)(msg + 0x64) = -1;
                        *(int *)(msg + 0x68) = 3;
                        *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                        *(int *)(msg + 0x0c) = U_I(rec + 1);
                        *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                        *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                        *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                        *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                        *(int *)(msg + 0x5c) = U_I(rec + 1);
                        *(int *)(msg + 0x6c) = i;
                        lv = -1;
                        AI_4A63F0_7((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0, (char *)0x7b4c4a0, 0, 0, 100.0f, &lv);
                        *(int *)(msg + 0x58) = lv;
                        id = cm_SendLongMessage(msg);
                        if (id != -1)
                            while (cm_GetSlotState(id) == 0)
                                sv_RunFrame(0, 0x20007);
                        snd_PlayVoice(-7, 0, (char *)(unsigned)&full_data_62a3ec, 0);
                        U_I((unsigned)&sv_PopRequest) = 1;
                    }
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk581020, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}

int ai_AmountMenu(); int amt_GetKindLevel(int k); int sv_CheckMoney(); void cm_SendMoneyDelta(int a, int b);

/* 0x581400, the spy window on a selected character (1357 B, ROUGH: the six float -> int amounts amt[0..2] /
   cost[0..2] are 0x5de518 conversions whose FPU arguments Ghidra lost, and the 0x34-byte block zeroed to -1 is
   not written): arg 1 = the record; the target picked by 0x5c1130 (a 0x50-byte query: 1, 0x400, 6, 0x144a1f4),
   0 for none -> return; the skill points 0x50a640 (0x6f25b8) + 0x50a640 (the target's word), the level
   0x4e7c40 (the byte +0x166) / 3 + 2; the window "Locations_Domizil_kabinett_spy" (0x62a3f8) with 0x476f80 /
   0x475600, the sound 0x52ea30, the slider 0x46a960 (0x57, 0x14, 200, 0 .. 100, 0x7b, 2) with 0x46bca0 and the
   texts 0x4014 / +1 in 0x475b80 (window, 3, text); the pump loop 0x5309a0 (this handler, 0x677c7): the slider
   step, the texts 0x602568 / 0x400e / 0x400c (the target's word) / 0x400f (amt[i], cost[i]); the escape
   0x14673a0 -> 0x6f22a0 = 1; the first entry: 0x521040 (the character 0x770b80 + 0x6f25b8 * 0x21c, cost[i],
   0x6f1bc5), 0x520f90 (the character, amt[i]) == 1 and 0x580fe0 (rec) -> 0x506430 (the character) +0x28 +=
   cost[i], 0x4c4830 (-1, the character, 0x6f1bc5, cost[i]), 0x4c5f60 (the character, -amt[i]), the message (type
   0x99, 2 at +0x36, the time, the target's word at +0x60, -1 at +0x64, 2 at +0x68, the record + 1 at +0xc /
   +0x5c, the character at +8, the step at +0x6c, the value at +0x58 from 0x4a63f0 or the target's +0x16c /
   +0x170 record) and 0x4c2760 (-7, 0, "KOMMENTARE", 0) then 0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1;
   the pump 0 -> 0x4758f0 (window), 0x52d7a0 () */
void ai_Unk581400(char *rec)
{
    char msg[0xf8];
    int amt[3];
    int cost[3];
    char q[0x50];
    char *p;
    int win;
    int slider;
    int b1;
    int b2;
    int total;
    int n;
    int i;
    int k;
    int r;
    int id;
    int lv;

    total = gm_GetWorth(U_I((unsigned)&ai_PlayerId));
    memset(q, 0, 0x50);
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = 0x400;
    *(int *)(q + 0x10) = 6;
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a1f4), 0, 0);
    if (!p)
        return;
    total += gm_GetWorth(*(unsigned short *)p);
    n = amt_GetKindLevel(*(unsigned char *)(p + 0x166)) / 3 + 2;
    amt[0] = 0;
    amt[1] = 0;
    amt[2] = 0;
    cost[0] = 0;
    cost[1] = 0;
    cost[2] = 0;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3f8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 2);
    k = d2_CreateWinText(0, 3, (char *)U_I((unsigned)&full_data_144a200), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + k * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + k * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    d2_SetObjFont(k, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 200, 0, 100, 0x7b, 2, U_I((unsigned)&d2_LastWindow));
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, U_I((unsigned)&full_data_144a204), U_I((unsigned)&full_data_144a208), U_I((unsigned)&full_data_144a20c));
    d2_SetForm(win, 3);
    k = io_PrintText(0x4014);
    b1 = d2_GetWinObj(win, 3, k);
    b2 = d2_GetWinObj(win, 3, k + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText(0x400e, 0x400c, *(unsigned short *)p);
        io_PrintText(0x400f, amt[i], cost[i]);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, cost[i], *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0 &&
                    sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, amt[i]) == 1 &&
                    ai_Unk580fe0(rec) != 0) {
                    r = gm_PlayerRecord((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
                    if (r != 0)
                        *(int *)(r + 0x28) += cost[i];
                    cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, cost[i]);
                    cm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -amt[i]);
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x99;
                    *(unsigned char *)(msg + 0x36) = 2;
                    *(int *)(msg + 0x64) = -1;
                    *(int *)(msg + 0x68) = 2;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x60) = *(int *)(p + 4);
                    *(int *)(msg + 0x5c) = U_I(rec + 1);
                    *(int *)(msg + 0x6c) = i;
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
                    id = cm_SendLongMessage(msg);
                    if (id != -1) {
                        while (cm_GetSlotState(id) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    snd_PlayVoice(-7, 0, (char *)(unsigned)&full_data_62a3ec, 0);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk581400, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}
/* 0x581950, the spy window's pay step (1117 B, ROUGH): arg 1 = the record; the target picked by 0x5c1130 (the
   same 0x50-byte query, 0x144a1b0), 0 for none -> 0x52d7a0 () and return; the level 0x4e7c40 (the byte +0x166) / 3,
   the amounts level + 5 / + 7 / + 10 and the level + 2; the window "Locations_Domizil_kabinett_spy" (0x62a3f8)
   with 0x476f80 / 0x475600, the sound 0x52ea30, the slider 0x46a960 (0x57, 0x14, 200, 0 .. 100, 0x7b, 2) with
   0x46bca0 and the texts 0x4003 / +1 in 0x475b80 (window, 3, text); the pump loop 0x5309a0 (this handler,
   0x677c7): the step, the texts 0x602568 / 0x3ffd / 0x3ffb (the target's word, the level + 2) / 0x3ffe (the
   amount); the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: 0x520f90 (the character 0x770b80 + 0x6f25b8 *
   0x21c, the amount) == 1 and 0x580fe0 (rec) -> 0x4c5f60 (the character, -the amount), the message (type 0x99,
   2 at +0x36, -1 at +0x64, 1 at +0x68, the target's word at +0x60, the character at +8, the record + 1 at +0xc
   / +0x5c, the time, the step at +0x6c, the value at +0x58) and 0x4c2760 (-7, 0, "KOMMENTARE", 0) then
   0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x52d7a0 () */

void ai_Unk581950(char *rec)
{
    char msg[0xf8];
    char q[0x50];
    char *p;
    int amt[3];
    int win;
    int slider;
    int b1;
    int b2;
    int lvl;
    int n;
    int i;
    int k;
    int r;
    int id;
    int lv;

    memset(q, 0, 0x50);
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = 0x400;
    *(int *)(q + 0x10) = 6;
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a1b0), 0, 0);
    if (!p) {
        sv_CameraFollow();
        return;
    }
    lvl = amt_GetKindLevel(*(unsigned char *)(p + 0x166)) / 3;
    n = lvl + 2;
    amt[0] = lvl + 5;
    amt[1] = lvl + 7;
    amt[2] = lvl + 10;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a3f8);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 2);
    k = d2_CreateWinText(0, 3, (char *)U_I((unsigned)&full_data_144a1bc), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + k * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + k * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    d2_SetObjFont(k, 0x54);
    slider = d2_WinOpenSlider(0x57, 0x14, 200, 0, 100, 0x7b, 2, U_I((unsigned)&d2_LastWindow));
    d2_SetObjValue(slider, 0, 2, 0, 1);
    d2_SetList(slider, 3, U_I((unsigned)&full_data_144a1c0), U_I((unsigned)&full_data_144a1c4), U_I((unsigned)&full_data_144a1c8));
    d2_SetForm(win, 3);
    k = io_PrintText(0x4003);
    b1 = d2_GetWinObj(win, 3, k);
    b2 = d2_GetWinObj(win, 3, k + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        i = d2_GetObjValue(slider);
        d2_SetForm(win, 1);
        io_PrintText((char *)(unsigned)"$C");
        io_PrintText(0x3ffd, 0x3ffb, *(unsigned short *)p, n);
        io_PrintText(0x3ffe, amt[i]);
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                if (sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, amt[i]) == 1 &&
                    ai_Unk580fe0(rec) != 0) {
                    cm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -amt[i]);
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x99;
                    *(unsigned char *)(msg + 0x36) = 2;
                    *(int *)(msg + 0x64) = -1;
                    *(int *)(msg + 0x68) = 1;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x60) = *(int *)(p + 4);
                    *(int *)(msg + 0x5c) = U_I(rec + 1);
                    *(int *)(msg + 0x6c) = i;
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
                    id = cm_SendLongMessage(msg);
                    if (id != -1) {
                        while (cm_GetSlotState(id) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    snd_PlayVoice(-7, 0, (char *)(unsigned)&full_data_62a3ec, 0);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk581950, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}

int he_Unk56f3b0(); void txt_Format(char *out, int id0, ...); int sv_MessageBox();

/* 0x581db0, the cabinet spy window (1047 B, ROUGH: the amount is one float -> int conversion (0x5de518) whose
   argument Ghidra lost): arg 1 = the record; the target picked by 0x5c1130 (the 0x50-byte query, 0x144a270);
   0x56f3b0 (target) 0 -> the text 0x4030 through 0x5176d0 (the target's word) and 0x521150 (text, 0); else the
   skill points 0x50a640 (0x6f25b8) + 0x50a640 (the target's word) and the amount; the window
   "Locations_Domizil_kabinett" (0x62a418) with 0x476f80 / 0x475600, the sound 0x52ea30, the texts 0x602568 /
   0x402e / 0x402b (the amount, the target's word) and the entries 0x402f / +1 in 0x475b80 (window, 2, text); the
   pump loop 0x5309a0 (this handler, 0x677c7): the escape 0x14673a0 -> 0x6f22a0 = 1; the first entry: 0x521040
   (the character 0x770b80 + 0x6f25b8 * 0x21c, the amount, 0x6f1bc5), 0x520f90 (the character, 0x10) == 1,
   0x580fe0 (rec) -> 0x506430 (the character) +0x28 += the amount, 0x4c4830 (-1, the character, 0x6f1bc5, the
   amount), 0x4c5f60 (the character, -0x10), the message (type 0x99, 2 at +0x36, -1 at +0x64, 4 at +0x68, 0 at
   +0x6c, the target's word at +0x60, the time, the value at +0x58) and 0x4c2760 (-7, 0, "KOMMENTARE", -1) then
   0x6f22a0 = 1; the second entry -> 0x6f22a0 = 1; the pump 0 -> 0x4758f0 (window), 0x52d7a0 () */
void ai_Unk581db0(char *rec)
{
    char msg[0xf8];
    char q[0x50];
    char *p;
    int win;
    int b1;
    int b2;
    int total;
    int amount;
    int i;
    int r;
    int lv;

    total = gm_GetWorth(U_I((unsigned)&ai_PlayerId));
    memset(q, 0, 0x50);
    *(int *)(q + 0x00) = 1;
    *(int *)(q + 0x04) = 0x400;
    *(int *)(q + 0x10) = 6;
    p = (char *)ai_AmountMenu(q, (char *)U_I((unsigned)&full_data_144a270), 0, 0);
    if (!he_Unk56f3b0(p)) {
        txt_Format(msg, (char *)0x4030, *(unsigned short *)p);
        sv_MessageBox(msg, 0);
        return;
    }
    if (!p)
        return;
    total += gm_GetWorth(*(unsigned short *)p);
    amount = 0;   /* ROUGH: the 0x5de518 conversion's argument is lost */
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a418);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    io_PrintText((char *)(unsigned)"$C");
    io_PrintText(0x402e, 0x402b, amount, *(unsigned short *)p);
    d2_SetForm(win, 2);
    i = io_PrintText(0x402f);
    b1 = d2_GetWinObj(win, 2, i);
    b2 = d2_GetWinObj(win, 2, i + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&d2_PointerObj) == b1) {
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, amount, *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0 &&
                    sv_CheckMoney((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0x10) == 1 &&
                    ai_Unk580fe0(rec) != 0) {
                    r = gm_PlayerRecord((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
                    if (r != 0)
                        *(int *)(r + 0x28) += amount;
                    cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, amount);
                    cm_SendMoneyDelta(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -0x10);
                    memset(msg, 0, 0xf8);
                    *(unsigned char *)(msg + 0x04) = 0x99;
                    *(unsigned char *)(msg + 0x36) = 2;
                    *(int *)(msg + 0x64) = -1;
                    *(int *)(msg + 0x68) = 4;
                    *(int *)(msg + 0x6c) = 0;
                    *(int *)(msg + 0x08) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(rec + 0x27) * 0x21c);
                    *(int *)(msg + 0x0c) = U_I(rec + 1);
                    *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                    *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                    *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                    *(int *)(msg + 0x60) = *(int *)(p + 4);
                    *(int *)(msg + 0x5c) = U_I(rec + 1);
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
                    i = cm_SendLongMessage(msg);
                    if (i != -1) {
                        while (cm_GetSlotState(i) == 0)
                            sv_RunFrame(0, 0x20007);
                    }
                    snd_PlayVoice(-7, 0, (char *)(unsigned)&full_data_62a3ec, -1);
                    U_I((unsigned)&sv_PopRequest) = 1;
                }
            } else if (U_I((unsigned)&d2_PointerObj) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk581db0, 0x677c7) == 0) {
            d2_CloseView(win);
            sv_CameraFollow();
            return;
        }
    }
}

void sv_ReleaseNamePlates(void); int sv_RegisterNamePlate(); int ai_OpenTransport(); int ai_StorageWindow();
int ai_ProductionWindow(); void ai_Unk5823b0(); int ai_Unk57c690(); int ai_Unk57cf90();
int ai_Unk57bf00(void *rec);

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
                    ai_Unk57c690(rec);
                } else if (U_I((unsigned)&ai_ClickedObject) == c4) {
                    ai_Unk57cf90(rec);
                } else if (U_I((unsigned)&ai_ClickedObject) == c1) {
                    ai_Unk57bf00(rec);
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
                    ai_Unk57e270(rec);
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
                    ai_Unk581950(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c2)
                    ai_Unk581020(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c3)
                    ai_Unk581db0(rec);
                else if (U_I((unsigned)&ai_ClickedObject) == c4)
                    ai_Unk581400(rec);
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

int sv_CreateFormSlider(); unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l); int gv_EntryIndex(char *r, int *e); int he_Unk573200();
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
    sv_CreateFormSlider(0, 0, w[5], w[2], 0x35, a, b, &sel);
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
    sv_CreateFormSlider(0, 0, w[6], w[3], *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[3] * 0x3c4), buf2, buf1, &clicked);
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
    sv_CreateFormSlider(0, 0, w[4], w[2], *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + w[2] * 0x3c4), b, a, c);
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

int unk_Unk4f0900(); int unk_Unk4ef4f0(); void sv_ButtonRow(int f, int y, int *out, int n, ...);

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
                unk_Unk4f0900(0, rec, a, b);
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
        if (unk_Unk4ef4f0(U_I(rec + 0x65), i) == 0) {
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

void unk_Unk4f0e80(int *g, int x, char *o, int v, char b, int y); int unk_Unk4f2330(); int unk_Unk4f0430(int *g, char *o, int v, short s, int x);

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
                h = ((int (*)(void *, int, int, int, int, int))unk_Unk4f0e80)((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, idx, rec, a, b, cost);
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
        cost = unk_Unk4f2330((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0xff, idx, 1, &out);
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
                h = unk_Unk4f0430((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, rec, idx, 10, cost);
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

int unk_Unk4f1d50(unsigned char *o, int *n); int unk_Unk4f1170(); int cm_GetSlotValue(int h); int sprintf(char *buf, const char *fmt, ...);

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
    v = unk_Unk4f1d50(o, &n);
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
                        h = unk_Unk4f1170(o, *(int *)*(int *)ev, *(unsigned char *)((char *)ev + 8), *(int *)((char *)ev + 0x10), 0x1c);
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

int sv_CheckCharacterFlag(); char * he_FindEvent(int n, ...); char * he_NextEvent(void); void sv_Unk52e6d0(int f, int b, int c); void d2_SetCaption();
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
    sv_Unk52e6d0(U_I((unsigned)&d2_LastWindow), 0x70f, 0x741);
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
        name1 = (char *)unk_Unk4fb330(*(unsigned char *)((unsigned)&ai_Unk770b89 + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(((unsigned)&ai_CharInts + 0x160) + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
        if (name1 == 0)
            name1 = (char *)unk_Unk4fb330(*(unsigned char *)((unsigned)&ai_Unk770b89 + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), 0);
        name2 = (char *)unk_Unk4fb330(U_I(ev + 0x4c), U_I(ev + 0x48));
        if (name2 == 0)
            name2 = (char *)unk_Unk4fb330(U_I(ev + 0x4c), 0);
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

void ai_Unk4a5760(struct G5CH *p, int n, struct G9REC *rec, int *st);

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
    ai_Unk4a5760((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, 0xf, (int *)rec, st);
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

void ai_EndCharExtra(char *ch, int f); void cm_SendRelations(int a, int b, int c, int d); int sv_Unk52b490();

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
            if (U_I((unsigned)&ai_Unk6f2448) != 0) {
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

int unk_Unk4f25d0();

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
        if (unk_Unk4f25d0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, &bld, &p) == 0) {
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
    if (U_I((unsigned)&ai_Unk6f2448) != 0 || n == 2) {
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
    if (U_I((unsigned)&ai_Unk6f2448) != 0)
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
    if (U_I((unsigned)&ai_Unk6f2448) == 0 && util_RandomF() < 0.9f)
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
void ai_Unk587e00(void)
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

int gm_Unk509dc0(int unused, int kind); char *_strupr(char *); int sprintf(char *buf, const char *fmt, ...);

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
        io_PrintText(0x28, gm_Unk509dc0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, idx));
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

int d2_GetViewWin(); int sv_CreateFormSlider(); void d2_RadioClear(int g); int d2_RemoveWindowObjects(); void d2_MenuNavigate(int g);

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

void ai_Unk589c90(void)
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
void ai_Unk589cf0(void)
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
