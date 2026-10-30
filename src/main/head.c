/* main.c -- the program level code (0x531670 ..): the tag "main_" ("main_OpenEngine(): could not open ..." 0x624a8c): the window class "Die Gilde" (0x62493c), the engine start / stop in order, the single instance
   mutex, the version string "Europa 1400 - Gold Edition - 2.06", the window creation and the first init (screenshot directory "<path>gamedata\Screenshots"). Split from sv.c (layout 2026-10-23 session 5): the strings
   0x62493c .. 0x6249a8 sit after the last sv string ("MASTER(): .." 0x624534) and the code is the process level start / stop, not the in-game screen. ROUGH drafts.
   unit of rea 8cef7d5b (src/game/main.c); boundaries in decl/text.dcl, notes in src/main/open.md */
struct UDPSERVER;
extern char gm_PlayerRecords[];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "he.h"
#include "sfx.h"
#include <objbase.h>
#include <strmif.h>
#include <control.h>
#include "f3gm.h"
#include "mov.h"
#include "txt.h"
#include "di_input.h"
#include "evt.h"
#include <d3d.h>
#include "d3.h"
#include "d3_light.h"
#include "d3_engine.h"
#include "data_views.h"
#include "d2.h"
#include "unk_58bbd0.h"
#include "ecmd.h"
#include "ts_texture.h"
#include "unk_5d33b0.h"
#include "ai.h"
#include "gfx.h"
#include "dd_vesa.h"
#include "d3_interface.h"
#include "d3_int_io.h"
#include "err.h"
#include "tr.h"
#include "vfs.h"
#include "tb.h"
#include "msx.h"
#include "evtc.h"
#include "sndc.h"
#include "net.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int main_Inactive;       /* 0x6f2584, 1 while the app is deactivated */
extern char ai_DataPath[];
extern unsigned char ai_GfxResolution;
extern int ai_ScreenModeH;
extern int ai_ScreenModeW;
extern struct SCREENMODE { int w, h; } ai_ScreenModes[];
extern char d3_AnimDir[];
extern int di_MaxX;
extern char main_GamePath[];
extern int main_TextureDivider;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;
extern unsigned char ai_Unk9367e0;
extern GAPDATA full_data_5f320c[];
extern int main_TextReopen;
extern int main_HmtMode;
extern int main_HmtBuild;
extern char main_AppDir[];
extern unsigned char init_PlayerProfile[];
extern GAPDATA full_data_60da10[];
extern struct LSSAVEVIEW ls_SavePath;
extern int sv_PopRequest;
extern int ai_ModeMask;
extern GAPDATA full_data_624b48[];
extern GAPDATA full_data_624b4c[];
extern GAPDATA full_data_624b50[];
extern GAPDATA main_NetGameRecord[];
extern int ai_ModeMask;
extern unsigned char ai_Unk9364e0;
extern unsigned char d3_Unk6333fc;
extern GAPDATA full_data_5f1084[];
extern GAPDATA full_data_624b48[];
extern GAPDATA full_data_624b4c[];
extern GAPDATA full_data_624b50[];
extern GAPDATA main_NetGameRecord[];
extern struct LSSAVEVIEW ls_SavePath;
extern unsigned char ai_OptionsFile;
extern int sv_PopRequest;
extern char ai_DataPath[];

extern unsigned char ai_OptionsFile;
extern unsigned char ai_Unk6333cc;
extern unsigned char ai_Unk9364e0;
extern unsigned char d3_Unk6333fc;



extern GAPDATA full_data_5f1084[];
extern GAPDATA full_data_936460[];
extern struct LSSAVEVIEW ls_SavePath;
extern int sv_PopRequest;
extern unsigned char ai_MissionMode;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern char ai_DataPath[];




extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;





extern GAPDATA full_data_5f320c[];
extern GAPDATA full_data_6f2418[];
extern GAPDATA full_data_6f245c[];
extern GAPDATA full_data_6f25b4[];
extern unsigned char init_PlayerProfile[];
extern unsigned char sv_FrameBase;
extern unsigned char sv_FrameDepth;
extern unsigned char sv_FrameMask;
extern int sv_PopRequest;

extern GAPDATA full_data_624ce0[];
extern GAPDATA full_data_624ce4[];
extern GAPDATA full_data_624ce8[];
extern GAPDATA full_data_624cec[];
extern GAPDATA full_data_624cf8[];
extern GAPDATA full_data_75e180[];
extern GAPDATA full_data_75e198[];
extern GAPDATA full_data_75e5f0[];
extern unsigned char init_PlayerProfile[];
extern int sv_PopRequest;
extern GAPDATA full_data_75e5f5[];
extern GAPDATA full_data_75e628[];
extern unsigned char ai_Unk5f8438;
extern unsigned char ai_Unk5fb5b0;
extern unsigned char ai_Unk633738;
extern unsigned char ai_Unkfc2dec;
extern unsigned char d3t_CamMove;
extern unsigned char d3t_CamMoving;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_624938[];
extern char ai_DataPath[];
extern unsigned char ai_OptionsFile;




extern unsigned char init_PlayerProfile[];
extern int sv_PopRequest;
extern unsigned char ai_OptionHistorie;

extern GAPDATA full_data_624938[];
extern GAPDATA full_data_75e5ec[];
extern GAPDATA full_data_75e5f0[];
extern GAPDATA full_data_75e5f4[];
extern GAPDATA full_data_75e628[];
extern unsigned char hst_TextsOn;
extern unsigned char init_PlayerProfile[];
extern unsigned char sv_FrameMask;
extern int sv_PopRequest;
extern unsigned char ai_MissionMode;
extern unsigned char ai_GamePanel;
extern unsigned char ai_Opponents;






extern int sv_PopRequest;
extern char ai_DataPath[];
extern unsigned char ai_Unk60d360;
extern unsigned char ai_Unk6f22e8;
extern unsigned char ai_Unkfc2dec;

extern unsigned char d3_Unk6333fc;


extern GAPDATA full_data_5f1084[];
extern GAPDATA full_data_602568[];
extern unsigned char sv_FrameMask;
extern int sv_PopRequest;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern unsigned char init_StatusForm;
extern unsigned char cm_RequestedSaveName;
extern unsigned char ai_Unk1460ae0;
extern unsigned char ai_MissionSeed;
extern GAPDATA full_data_75df20[];
extern unsigned char ai_Unk75ec60;
extern unsigned char ch_SndLib;
extern unsigned char cm_RequestedSaveName;
extern GAPDATA full_data_6f2408[];
extern GAPDATA full_data_6f240c[];
extern GAPDATA full_data_6f2410[];
extern GAPDATA full_data_6f2414[];
extern GAPDATA full_data_6f2598[];
extern GAPDATA full_data_6f25c0[];
extern GAPDATA full_data_75e630[];
extern unsigned char snd_Bank;
extern unsigned char sv_PanelShown;
extern unsigned char ai_Unk1468ae0;
extern unsigned char ai_Unk6f0d8c;
extern unsigned char ai_Unk760618;
extern unsigned char ai_Unk76061c;
extern unsigned char ai_Unk760620;
extern unsigned char ch_Motions;
extern unsigned char d3t_CamMove;
extern unsigned char d3t_SunRays;
extern GAPDATA full_data_6f25c4[];
extern unsigned char sv_FrameMask;
extern unsigned char ai_Unk1460ae0;
extern unsigned char ai_GamePanel;
extern unsigned char ai_MissionMode;
extern unsigned char ai_Unk6f2438;
extern unsigned char ai_Unkf83650;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern GAPDATA full_data_624560[];
extern GAPDATA full_data_6253b0[];
extern GAPDATA full_data_6253b4[];
extern GAPDATA full_data_6253b8[];
extern GAPDATA full_data_760454[];
extern unsigned char he_RunHandlers;
extern unsigned char sv_PanelForm;
extern int sv_PopRequest;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;


extern unsigned char ai_Unk6f1bb8;
extern char ai_Unk925520[];
extern GAPDATA full_data_92bca8[];
extern unsigned char he_Events;
extern unsigned char he_QuickList;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern unsigned char ai_Unkfc2dec;
extern GAPDATA full_data_6024e4[];
extern GAPDATA full_data_624560[];
extern GAPDATA full_data_6f25b0[];
extern GAPDATA full_data_6f25c0[];
extern unsigned char sv_FrameMask;
extern unsigned char sv_PanelFlag;
extern int sv_PopRequest;
extern unsigned char ai_MissionMode;


extern GAPDATA full_data_75e401[];
extern GAPDATA full_data_75e5ec[];
extern GAPDATA full_data_75e5f0[];
extern GAPDATA full_data_75e5f5[];
extern GAPDATA full_data_75e628[];
extern unsigned char init_PlayerProfile[];
extern unsigned char ai_Unk1460ae0;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern unsigned char cm_NetFlags;
extern GAPDATA full_data_624560[];
extern unsigned char init_BuildingUniverse;
extern unsigned char snd_AmbientOn;
extern unsigned char sv_FrameMask;
extern int sv_PopRequest;
extern char ai_DataPath[];

extern unsigned char ai_FrameWait;
extern unsigned char ai_GamePanel;
extern unsigned char ai_Unk1460ae0;
extern unsigned char ai_Unk60bd94;
extern unsigned char ai_Unk60c1ec;
extern unsigned char ai_Unk6f2440;
extern unsigned char ai_Unkf31144;
extern unsigned char cm_NetFlags;


extern GAPDATA full_data_62456c[];
extern GAPDATA full_data_6f2428[];
extern GAPDATA full_data_6f2598[];
extern GAPDATA full_data_6f25a8[];
extern GAPDATA full_data_6f25c4[];
extern GAPDATA full_data_760454[];
extern GAPDATA full_data_76f990[];
extern GAPDATA full_data_76f994[];
extern unsigned char init_StatusForm;
extern struct LSSAVEVIEW ls_SavePath;
extern unsigned char main_MoneyCheat;
extern unsigned char sv_FrameMask;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;
extern GAPDATA full_data_624798[];
extern GAPDATA full_data_6f241c[];
extern unsigned char di_Disabled;
extern GAPDATA full_data_6f2574[];
extern unsigned char snd_AmbientOn;
extern char ai_DataPath[];
extern unsigned char ai_GfxResolution;
extern unsigned char ai_OptionsFile;
extern int ai_ScreenModeH;
extern int ai_ScreenModeW;
extern struct SCREENMODE ai_ScreenModes[];





extern GAPDATA full_data_5f320c[];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_624584[];
extern unsigned char main_GfxPath[];
extern char main_GamePath[];
extern GAPDATA full_data_624798[];
extern GAPDATA full_data_624cf8[];
extern GAPDATA full_data_6f2408[];
extern GAPDATA full_data_6f240c[];
extern GAPDATA full_data_6f2410[];
extern int main_TextureDivider;
extern GAPDATA full_data_6f2434[];
extern GAPDATA full_data_6f2470[];
extern GAPDATA full_data_6f2574[];
extern GAPDATA full_data_6f2580[];
extern int main_Inactive;       /* 0x6f2584, 1 while the app is deactivated */
extern GAPDATA full_data_6f2598[];
extern GAPDATA full_data_6f25a8[];
extern GAPDATA full_data_6f25b4[];
extern char main_AppDir[];
extern GAPDATA full_data_75e5f0[];
extern GAPDATA full_data_75e5f4[];
extern GAPDATA full_data_75e628[];
extern unsigned char init_PlayerProfile[];
extern struct LSSAVEVIEW ls_SavePath;
extern unsigned char main_MoneyCheat;
extern unsigned char sv_FrameDepth;
extern int sv_PopRequest;

#define U_I(a) (*(int *)(a))

extern HWND main_Window;
extern int snd_SfxEnabled;              /* 0x6f257c, f3snd.h */
extern int snd_MusicEnabled;            /* 0x6f2578, f3snd.h */

void main_Unk537f40(void);
void main_Unk537be0(void);
void main_Unk538140(void);
void main_Unk5384e0(void);
void main_Unk539870(void);

void main_Unk53a520(unsigned short pl);
void main_Unk53cb40(void);

extern unsigned int d3anim_CacheMax;   /* 0x5f927c, d3_anim.h */
void hst_ResetLists(void);              /* 0x4b1f30, hst.c */
int io_PrintText(char *fmt, ...);       /* 0x515960 */
void ai_ClearTurnTable(void);           /* 0x587de0, unk_575860.c: not in unk_575860.h (adding it there moves unk_575860's drafts ai_Unk579570 / ai_Unk57aeb0, see open.md) */
void ai_InitStates(void);               /* 0x4c3480 */
void unk_ResetEffectSlots(void);        /* 0x4eddf0 */
int sb_Init(int mem, int count, int a, int b, int c);  /* 0x425bc0 */
int d3snd_Init(int max);                /* 0x417650 */
int d3sndw_Init(unsigned short n);      /* 0x417ec0 */
void sb_SetPath(char *path);            /* 0x426240 */
int main_PreloadSound(char *f);         /* 0x537c70 */
void sv_InitTick(void);                 /* 0x520ee0 */
char init_ResetSceneUniverses(void);    /* 0x50d460 */
int ch_Open(void);                      /* 0x40afb0 */
void d3t_CreateSnowMipmaps(void);       /* 0x416980 */
int ahm_LoadPrototypes(int mode);       /* 0x4eb480 */
int hmt_CreateBinaryFile(void);         /* 0x4ac440 */
void hmt_Init(void);                    /* 0x4aca90 */
void ai_BuildFloatTable(void);          /* 0x4d27a0 */
void chc_Register(void);                /* 0x40d270 */
int init_InitTown(char *city);          /* 0x510ad0, f3init */
void init_FreeScenes(void);             /* 0x50d490, f3init */
#define d2_MoveObjS ((void (*)(short, short, int))d2_MoveObj)  /* 0x46cda0 as its definition reads it (d2.c f_46cda0.c: short x, short y): this unit's callers push the short sums untruncated (0x532212 `sub dx, [..]; sub edx, 0x30; push edx`); d2.h keeps int for d2_CreatePicObj (@original) */
#define util_RandomInt ((int (*)(unsigned short))util_Random)  /* 0x42ec00 as tr.c defines it: @original this unit reads the full int result (0x53b8c9 util_Random(4) + 0x20 pushed without the 16-bit mask ai.h's unsigned short prototype adds) */
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue)  /* the 4-argument call (as unk_5d33b0.h) */
#pragma pack(push, 2)
struct NETHDR { short a, b, size; char name[0x20]; unsigned char players; char pad27; short x; char rest[0x40]; };  /* the 106-byte network game record at ls_SavePath + 0xa0 (reasoned from 0x5328f0's stores: +4 the size 0x6a, +6 the name, +0x26 the player count) */
#pragma pack(pop)
int main_ChooseCity(int unused, int host);  /* 0x537230, main.c */
struct LSSAVEVIEW { char pad0[0x84]; short cur; char pad86[0xa0 - 0x86]; struct NETHDR net; char pad10a[0x120 - 0x10a]; char version[0x40]; char rest[0x280 - 0x160]; };  /* the used parts of ls.c's char ls_SavePath[640] (+0x84 a short, +0xa0 the network game record, +0x120 the version text) */
struct D3OBJECT;
void d3t_FreeSnowMipmaps(void);         /* 0x4175c0, d3_tools.h */
void main_FreeSoundBanks(void);         /* 0x537db0 */
void sv_UnregisterTick(void);           /* 0x520f10, sv.c */
void sb_Exit(void);                     /* 0x425ce0, sblib.c */
void d3snd_Exit(void);                  /* 0x4176f0, d3_snd.c */
void d3sndw_Exit(void);                 /* 0x418030, d3_snd.c */
void ahm_FreePrototypes(void);          /* 0x4ebf60, ahm.h */
int hmt_Exit(void);                     /* 0x4acde0, hmt.h */
void gv_FreeEffectObjects(void);        /* 0x4ede70, gv.h */
void ch_Close(void);                    /* 0x40b1a0, ch.h */
void ai_ResetMessages(void);                /* 0x4c3560 */
void mem_Exit(void);                    /* 0x422910, mem.h */
void mem_Init(int max);                 /* 0x4226a0, mem.h */
char d3d_Restore();                    /* 0x4370d0 */
void amt_CheckOffices(void);            /* 0x4e9260, amt.h */

#include "time_view.h"
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c): @original this unit pushes the third argument as an int (byte-matched ai_MeisterBewaffnen / ai_Unk497c80 / ai_Unk49c880) */

