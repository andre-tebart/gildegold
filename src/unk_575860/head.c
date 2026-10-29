/* unk_575860.c -- the unit after he.c (0x575860 ..): the end-of-game tick and the campaign state machine. Its
   state record is the pointer 0x5eec60: +0 the state (1 .. 0xc, the jump table 0x575fe8), +0x18 the running
   record, +0x1c the player, +0x74 the player's progress word, +0x2c / +0x30 / +0x38 / +0x3c flags; the clock is
   0x6333cc (and 0x6333e0), the end-of-game gates 0x6f265c / 0x6f262c, the pause 0x6310e4 / 0x75e8c0.
   The tail of the unit (layout 2026-10-23 session 18, 0x57dc50 .. 0x583700) is the tutorial's location and
   contact windows: the "Locations_Domizil_skill" / "_kabinett" / "_kabinett_spy" forms and the cannon / spy /
   contract / graveyard contact dispatchers (0x5821d0 .. 0x583240, all one window-list type), their literals
   climbing 0x62a2a4 .. 0x62a580 in function order (one .data block, no new boundary).
   Session 19 (0x584410 .. 0x586ab0) added the graveyard family: the grave action / confirmation / rent windows,
   the "Gehilfen erschaffen" and "Fog" windows, the seance selection / ritual / outcomes and the contact
   dispatcher 0x586840; literals climb 0x62a5bc .. 0x62a718 in function order (one .data block, no new boundary).
   Unit start (layout 2026-10-23 session 16): he.c's last function 0x575710 ends at 0x57582f, its jump table
   0x575830 .. 0x575853 and nop fill to 0x575860; the code here reaches neither he's literal block (last "Game"
   0x62916c) nor the next .data block at 0x62918c (the "FT" state names), whose first literals come with 0x577740.
   The name is unknown (open boundary; the "FT" tutorial tag may cover this stretch too).
   0x5770e0..0x577110 is not nop fill: it is the 0x30-byte function ai_Unk5770e0 (a ret + nop start the scan
   missed)
   unit of rea 8cef7d5b (src/game/unk_575860.c); boundaries in decl/text.dcl, notes in src/unk_575860/open.md */
struct AHMSTEP;
struct D3SKY;
extern char d2_Radios[];
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

extern int gv_EntrySameId(int *a, int *b);
/* full rebuild: the symbols whose original addresses the code above spells literally */
extern const unsigned char ai_Unk5eeab8;
extern unsigned char d3t_CamMoving;
extern GAPDATA full_data_6f262c[];
extern GAPDATA full_data_6f265c[];
extern GAPDATA full_data_6f2660[];
extern unsigned char sv_PanelFlag;
extern unsigned char ai_Unk6333cc;
extern unsigned char tb_Paused;
extern unsigned char ai_BTypes;
extern unsigned char sv_PanelShown;
extern unsigned char ch_EmptyName;
extern unsigned char ai_PlayerId;
extern char ai_Chars[];
extern GAPDATA full_data_5f1890[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerBuilding;
extern unsigned char ai_Time;
extern unsigned char d2_LastWindow;
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_629304[];
extern GAPDATA full_data_602568[];
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char kb_Last;
extern GAPDATA full_data_629314[];
extern GAPDATA full_data_629328[];
extern GAPDATA full_data_629340[];
extern GAPDATA full_data_629354[];
extern GAPDATA full_data_629368[];
extern GAPDATA full_data_62918c[];
extern GAPDATA full_data_6291a0[];
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
extern unsigned char sv_FrameDepth;


extern GAPDATA full_data_624ee0[];
extern GAPDATA full_data_75e5f0[];
extern GAPDATA full_data_75e5f4[];
extern GAPDATA full_data_75e628[];
extern unsigned char init_PlayerProfile;
extern unsigned char d2_Objs;
extern GAPDATA full_data_6024f8[];
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_62918c[];
extern GAPDATA full_data_62a248[];
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerId;



extern unsigned char ai_Unk6333cc;
extern unsigned char d2_ClickedId;
extern GAPDATA full_data_62a284[];
extern GAPDATA full_data_62a2a4[];
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_LastWindow;
extern GAPDATA full_data_62a2c4[];
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerId;
extern GAPDATA full_data_6f2664[];
extern GAPDATA full_data_6f9f60[];
extern unsigned char ai_Unk5ebba4;
extern const unsigned char ai_Unk5ebbb4;
extern const unsigned char ai_Unk5eeab8;
extern unsigned char ai_Unk633738;
extern unsigned char ai_Unk925100;
extern unsigned char ai_Unk925108;
extern unsigned char ai_Unk925114;
extern unsigned char ai_Unk925118;
extern unsigned char ai_Unk92511c;
extern unsigned char ai_Unkf8aae0;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern unsigned char ai_Unkfc2dec;
extern unsigned char ch_EmptyName;
extern unsigned char d2_Objs;
extern unsigned char d2_ScreenH;
extern unsigned char d2_ScreenSurf;
extern unsigned char d2_ScreenW;
extern unsigned char d3_ScaleX;
extern unsigned char d3_Sky;
extern unsigned char d3_ZFar;
extern unsigned char d3_ZNear;
extern unsigned char d3s_QuadUV;
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
extern unsigned char sv_PanelFlag;
extern unsigned char sv_ViewRectBottom;
extern unsigned char sv_ViewRectLeft;
extern unsigned char sv_ViewRectRight;
extern unsigned char sv_ViewRectTop;
extern const unsigned char ai_Unk5ebb74;
extern unsigned char d3_YAxis;
extern unsigned char ai_PlayerChar;
extern unsigned char ai_Unk60c6c0;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_6f9f68[];
extern GAPDATA full_data_6f9f70[];
extern GAPDATA full_data_6f9f74[];


extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a364[];
extern unsigned char ai_Unk6333cc;
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern GAPDATA full_data_5f36e0[];
extern GAPDATA full_data_62a36c[];
extern GAPDATA full_data_6f9f60[];
extern GAPDATA full_data_6f9f68[];
extern GAPDATA full_data_6f9f6c[];
extern GAPDATA full_data_6f9f70[];
extern GAPDATA full_data_6f9f74[];
extern unsigned char ai_Unk925114;
extern unsigned char ai_Unk925118;
extern unsigned char d2_LastWindow;
extern unsigned char d2_Objs;
extern unsigned char d2_Pics;
extern unsigned char d2_ScreenH;
extern unsigned char d2_ScreenW;
extern char d2_Wins[];










extern GAPDATA full_data_62a38c[];
extern GAPDATA full_data_62a3ac[];
extern GAPDATA full_data_62a3cc[];

extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerId;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern unsigned char d2_Objs;




extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerId;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern unsigned char d2_Objs;





extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern unsigned char d2_Objs;





extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a3f8[];
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerId;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;

extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a3ec[];
extern GAPDATA full_data_62a418[];
extern unsigned char sv_PopRequest;
extern unsigned char ai_BuildingPerms;
extern unsigned char ai_ClickedObject;







extern GAPDATA full_data_62a434[];
extern GAPDATA full_data_62a450[];
extern GAPDATA full_data_62a464[];
extern GAPDATA full_data_62a474[];
extern GAPDATA full_data_62a488[];
extern GAPDATA full_data_62a4a8[];
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a418[];
extern unsigned char ai_BuildingPerms;
extern unsigned char ai_ClickedObject;

extern GAPDATA full_data_62a4bc[];
extern unsigned char sv_PopRequest;
extern unsigned char ai_BuildingPerms;
extern unsigned char ai_ClickedObject;




extern GAPDATA full_data_62a4cc[];
extern GAPDATA full_data_62a4e8[];
extern GAPDATA full_data_62a508[];
extern GAPDATA full_data_62a524[];
extern unsigned char sv_PopRequest;
extern unsigned char ai_Goods;
extern const unsigned char ai_Unk5eeab8;
extern unsigned char d2_Objs;
extern GAPDATA full_data_602568[];
extern const unsigned char gv_EntryCounts;
extern GAPDATA full_data_60c7b8[];
extern GAPDATA full_data_623594[];
extern GAPDATA full_data_62a570[];
extern unsigned char ai_Unk6333cc;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern GAPDATA full_data_62a578[];
extern GAPDATA full_data_62a580[];
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern unsigned char d2_Pics;
extern GAPDATA full_data_62a53c[];
extern GAPDATA full_data_62a544[];
extern GAPDATA full_data_62a54c[];
extern GAPDATA full_data_62a564[];
extern GAPDATA full_data_62a56c[];
extern GAPDATA full_data_62a59c[];
extern unsigned char ai_Unk5fe710;
extern GAPDATA full_data_62a5bc[];
extern unsigned char ai_OwnPlayer;
extern unsigned char sv_FrameMask;
extern GAPDATA full_data_625150[];
extern GAPDATA full_data_62a5d8[];


extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerId;
extern const unsigned char ai_Unk5eeab8;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern GAPDATA full_data_62a5bc[];
extern unsigned char sv_FrameMask;
extern GAPDATA full_data_625150[];
extern GAPDATA full_data_62a5d8[];
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;



extern GAPDATA full_data_62a5bc[];
extern unsigned char sv_PopRequest;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern GAPDATA full_data_62a5f8[];
extern GAPDATA full_data_62a62c[];
extern unsigned char ai_BTypes;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;

extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern unsigned char d2_Objs;

extern GAPDATA full_data_62a634[];
extern unsigned char sv_FrameMask;
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;









extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a64c[];
extern GAPDATA full_data_62a664[];
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;


extern GAPDATA full_data_62a680[];
extern unsigned char sv_PopRequest;
extern unsigned char ai_PlayerId;
extern unsigned char ai_Unk6333cc;
extern GAPDATA full_data_60c7b8[];
extern GAPDATA full_data_62a69c[];
extern GAPDATA full_data_62a6b8[];
extern GAPDATA full_data_62a6dc[];
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_Unk5eb780;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;
extern GAPDATA full_data_62a5bc[];
extern GAPDATA full_data_62a6f8[];
extern GAPDATA full_data_62a70c[];
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char ai_Relations;
extern const unsigned char ai_Unk5eeab8;


extern unsigned char ai_PlayerId;
extern unsigned char ai_Unk6f22f8;
extern GAPDATA full_data_62a718[];
extern unsigned char ai_BuildingPerms;
extern unsigned char ai_ClickedObject;

extern unsigned char ai_Unk6f2448;








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
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;



extern GAPDATA full_data_62a5bc[];
extern unsigned char sv_FrameMask;
extern unsigned char sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;


extern GAPDATA full_data_62a5bc[];
extern unsigned char sv_FrameMask;
extern unsigned char sv_PopRequest;

extern char ai_Chars[];
extern unsigned char ai_PlayerId;

extern const unsigned char ai_Unk5eeab8;


extern char ai_Chars[];
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk5fe710;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;

extern GAPDATA full_data_62a5bc[];
extern unsigned char sv_PopRequest;
extern unsigned char ai_BuildingPerms;
extern unsigned char ai_ClickedObject;





extern GAPDATA full_data_62a450[];
extern GAPDATA full_data_62a464[];
extern GAPDATA full_data_62a7f8[];
extern GAPDATA full_data_62a808[];
extern GAPDATA full_data_62a818[];
extern unsigned char ai_ClickedObject;

extern GAPDATA full_data_62a83c[];
extern unsigned char ai_PlayerId;
extern unsigned char ai_Time;
extern unsigned char ai_TimeHour;
extern char ai_Chars[];
extern unsigned char ai_Unk5fe710;
extern unsigned char ai_Unk6f2448;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindow;

extern GAPDATA full_data_62a84c[];
extern unsigned char sv_PopRequest;
extern unsigned char ai_BuildingPerms;
extern unsigned char ai_ClickedObject;


extern GAPDATA full_data_62a870[];
extern GAPDATA full_data_62a88c[];
extern unsigned char ai_BTypes;
extern unsigned char ai_Unk6f2448;
extern unsigned char ai_Unk5fb5b0;
extern unsigned char ai_Unk6333cc;
extern unsigned char ai_Unk7603a0;
extern GAPDATA full_data_62a538[];
extern GAPDATA full_data_62a8a8[];
extern GAPDATA full_data_6f5f60[];
extern unsigned char ai_Blds;
extern char ai_Chars[];
extern unsigned char ai_Goods;
extern unsigned char ai_Objects;
extern unsigned char ai_Unk5fe710;
extern unsigned char ai_Unk62a8cc;
extern unsigned char ai_Unk6f22e8;
extern unsigned char d2_CutsceneActive;
extern unsigned char d2_MouseX;
extern unsigned char d2_MouseY;
extern unsigned char d2_Objs;
extern unsigned char d2_Unkf8aac8;
extern char d2_Views[];
extern unsigned char di_Absolute;
extern GAPDATA full_data_62a8d0[];
extern GAPDATA full_data_62a8d4[];
extern GAPDATA full_data_6f22f0[];
extern GAPDATA full_data_6f266c[];
extern GAPDATA full_data_6f2670[];
extern char gm_PlayerRecords[];
extern unsigned char ai_PlayerId;
extern unsigned char ch_EmptyName;
extern unsigned char d2_LastWindow;
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
extern unsigned char ai_FallbackBuilding;
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerBuilding;
extern const unsigned char ai_Unk5ee160;
extern unsigned char ai_Unk75ec60;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62a91c[];
extern GAPDATA full_data_62a940[];
extern unsigned char ai_Difficulty;
extern GAPDATA full_data_62a960[];
extern GAPDATA full_data_62a96c[];
extern GAPDATA full_data_62a978[];
extern GAPDATA full_data_62a984[];
extern GAPDATA full_data_62a56c[];
extern GAPDATA full_data_62a9e4[];
extern GAPDATA full_data_62a9ec[];
extern GAPDATA full_data_62a9f8[];
extern unsigned char sv_PopRequest;
extern GAPDATA full_data_6f5ec0[];
extern GAPDATA full_data_6f5f38[];
extern GAPDATA full_data_6f5ec8[];
extern GAPDATA full_data_6f5f4c[];
extern GAPDATA full_data_6f5ec4[];
extern GAPDATA full_data_6f5f48[];
extern unsigned char kb_Last;
extern unsigned char ai_MarketBuilding;
extern GAPDATA full_data_62aa14[];
extern GAPDATA full_data_62aa20[];
extern unsigned char sv_PanelShown;

extern unsigned char d2_LastWindow;

extern GAPDATA full_data_62aa38[];
extern GAPDATA full_data_6f5ec0[];
extern GAPDATA full_data_6f5ec4[];
extern unsigned char sv_PopRequest;
extern unsigned char ch_EmptyName;
extern unsigned char d2_ScreenH;
extern unsigned char d2_ScreenW;
extern unsigned char dd_Surface;

extern GAPDATA full_data_62aa4c[];
extern GAPDATA full_data_62aa58[];
extern GAPDATA full_data_6f2674[];
extern unsigned char sv_FrameMask;
extern GAPDATA full_data_6f5ec0[];
extern GAPDATA full_data_6f5f44[];
extern unsigned char kb_Last;
extern unsigned char sv_PanelFlag;
extern unsigned char ai_GameFlags;
extern unsigned char ai_OptionSpeed;
extern unsigned char d2_FocusedInput;

extern unsigned char kb_Last;
extern unsigned char sv_FrameMask;
extern unsigned char sv_PanelFlag;
extern GAPDATA full_data_6f5ec4[];
extern GAPDATA full_data_6f5ec8[];
extern unsigned char ai_BTypes;
extern char ai_Chars[];
extern unsigned char ai_MarketBuilding;

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_Unk4a63f0)(p, a, fl, b, c, f, o)

#define U_I(a) (*(int *)(a))

char * ahm_OwnerChar();
void d2_RemoveObj();
void d2_SetObjEnabled();
void sv_CameraFollow();
int cm_SendLongMessage(char *msg);

void d2_SetPointerSprite(int a, int pic);
void ai_Unk577c60();
void ai_Unk57b8a0();
void ai_Unk577d90();
int ai_Unk5778b0();
int ai_Unk577a00();
void d2_RaiseViewWindows(int view);
int ai_Unk577e50();
void d2_SetObjEnabled();
int ai_Unk5780e0();
void ai_Unk577b90();
void sv_CameraFollow();
void sv_SetStatusText(char *s);
int sb_IsPlaying();
void sb_Stop();
int snd_PlayVoice(int id, int bank, char *text, int var);
unsigned char * gm_BuildQuery(int n, ...);
int sv_SelectPanelObject();
int he_Unk55ea60();
void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s);
void ai_Unk4c5150(int a, int b, int c, const char *s);
int gm_FindMarketObject(int id);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);
void sv_Unk52f9f0();
void sv_Unk52fa00();
char * gm_Query(void *list, int n, ...);
void cm_SendSetField(int a, int b, int c, int d, int e);
void amt_Unk4eaa00();
int amt_GetOfficeCandidates();
int amt_ApplyForOffice();
int amt_CheckOfficeColumns();
void ai_Unk4c4fa0(void *p);
int he_Unk53d6f0();
void d2_PlaceMainWindow();
void d2_SetViewEnabled(int view, int on);
int d2_OpenForm();
int d2_WinOpenSlider();
void d2_CloseView(int view);
void d2_SetObjValue();
void d2_CentreViewWindows(int view);
int d2_SetForm(int view, int n);
void sv_FitForm(int f);
int d2_GetWinObj();
int io_PrintText(char *fmt, ...);
void he_Unk53dcb0(int *sl);
int sb_IsPlaying();
void sb_Stop();
int snd_PlayVoice(int id, int bank, char *text, int var);
void ai_Unk577a90();
void ai_Unk577b60();
struct SBSAMPLE;
extern int d3t_CamMove;  /* 0x6310e8 */
extern int sv_FrameBase; /* 0x6f22a4 */
extern char *txt_Ptrs[]; /* 0x143a1c0 */

/* the state record is at 0x6fa060 (the 0x40 byte of main.c's 0x6fa020 block); 0x5eec60 is a const pointer to it
   in the original. A load through that pointer (not the fixed address) is what the unit's functions compile to,
   and the full link relocates the initializer, so the record is ours and no neighbour is touched. */
char *const ai_Unk5eec60 = (char *)full_data_6fa020 + 0x40;  /* 0x5eec60, the pointer to the tutorial state record */
#define S (*(char **)(unsigned)&ai_Unk5eec60)  /* 0x6fa060, the tutorial state record */

#include "time_view.h"

