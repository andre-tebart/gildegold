/* main.c -- the program level code (0x531670 ..): the tag "main_" ("main_OpenEngine(): could not open ..." 0x624a8c): the window class "Die Gilde" (0x62493c), the engine start / stop in order, the single instance
   mutex, the version string "Europa 1400 - Gold Edition - 2.06", the window creation and the first init (screenshot directory "<path>gamedata\Screenshots"). Split from sv.c (layout 2026-10-23 session 5): the strings
   0x62493c .. 0x6249a8 sit after the last sv string ("MASTER(): .." 0x624534) and the code is the process level start / stop, not the in-game screen. ROUGH drafts.
   unit of rea 8cef7d5b (src/game/main.c); boundaries in decl/text.dcl, notes in src/main/open.md */
struct UDPSERVER;
extern char gm_PlayerRecords[];
struct D2VIEWV { int id; int wins[2]; char pad[0x2ac - 12]; };  /* the used fields of d2.h's D2VIEW (0x2ac bytes) */
struct D2WINV { int id; short x, y, w, h; char pad0c[0x26c - 0xc]; int parent; char pad270[0x3c4 - 0x270]; };  /* the used fields of d2.h's D2WIN (0x3c4 bytes) */
extern struct D2WINV d2_Wins[];
extern char d2_Radios[];
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
#include "data_views.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
#pragma pack(push, 2)
struct MAINCLOCK { int year; unsigned short hour; int min; };  /* the used head of ai.h's AITIME (14 bytes: year, hour, minute, ..) */
#pragma pack(pop)
extern void *d2_BackSurf;     /* 0x6ed92c, the back surface (d2.c) */
extern unsigned char d3_Hardware;
extern int main_Inactive;       /* 0x6f2584, 1 while the app is deactivated */
extern int tb_Paused;           /* 0x6333e0 (tb.h) */
extern void *d2_ScreenSurf;  /* 0x6ed924, the screen surface (d2.c) */  /* 0x5fe710, 0x6ed924, 0x6ed938 */
extern char ai_DataPath[];
extern unsigned char ai_GfxResolution;
extern int ai_ScreenModeH;
extern int ai_ScreenModeW;
extern struct SCREENMODE { int w, h; } ai_ScreenModes[];
extern char d2_FormsDir[];
extern int d2_MainWindow;
extern short d2_ScreenH;
extern short d2_ScreenW;
extern char d3_AnimDir[];
extern unsigned char d3_DefaultD3DConfig[];
extern int d3io_Lod;
extern unsigned char dd_Has1024x768;
extern unsigned char dd_Has1152x864;
extern unsigned char dd_Has800x600;
extern int di_MaxX;
extern int di_MaxY;
extern char main_GamePath[];
extern int main_TextureDivider;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;
extern unsigned char ai_Unk9367e0;
extern struct D2VIEWV d2_Views[];
extern void *(*d3e_Resolve)(void);  /* 0x5fb920 */
extern GAPDATA full_data_5f320c[];
extern int snd_SystemBank;
extern int main_TextReopen;
extern int main_HmtMode;
extern int main_HmtBuild;
extern char main_AppDir[];
extern unsigned char init_PlayerProfile[];
extern GAPDATA full_data_60da10[];
extern char ls_SavePath[];
extern struct MAINCLOCK ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_NumPlayers;
extern unsigned char d2_ClickedId;
extern unsigned char sv_PopRequest;
extern unsigned char ai_GameFlags;
extern unsigned char ai_Unk9364dc;
extern unsigned char d3_ScaleX;
extern unsigned char d3_ZFar;
extern unsigned char d3_ZNear;
extern GAPDATA full_data_624b48[];
extern GAPDATA full_data_624b4c[];
extern GAPDATA full_data_624b50[];
extern GAPDATA full_data_75e1c0[];
extern GAPDATA full_data_75e1c4[];
extern GAPDATA full_data_75e1c8[];
extern GAPDATA full_data_75e360[];
extern GAPDATA full_data_936470[];
extern void *d2_ScreenSurf;  /* 0x6ed924, the screen surface (d2.c) */  /* 0x5fe710, 0x6ed924, 0x6ed938 */
extern unsigned char ai_GameFlags;
extern unsigned char ai_NumPlayers;
extern unsigned char ai_Unk9364dc;
extern unsigned char ai_Unk9364e0;
extern unsigned char d3_Unk6333fc;
extern GAPDATA full_data_5f1084[];
extern GAPDATA full_data_624b48[];
extern GAPDATA full_data_624b4c[];
extern GAPDATA full_data_624b50[];
extern GAPDATA full_data_75e1c0[];
extern GAPDATA full_data_75e1c4[];
extern GAPDATA full_data_75e1c8[];
extern GAPDATA full_data_75e360[];
extern GAPDATA full_data_936470[];
extern char ls_SavePath[];
extern unsigned char ai_OptionsFile;
extern unsigned char d2_ClickedId;
extern unsigned char kb_Last;
extern unsigned char sv_PopRequest;
extern char ai_DataPath[];

extern unsigned char ai_GameFlags;
extern unsigned char ai_OptionsFile;
extern unsigned char ai_Unk6333cc;
extern unsigned char ai_Unk9364e0;
extern unsigned char d2_ClickedId;
extern unsigned char d2_Objs;
extern unsigned char d3_Unk6333fc;



extern GAPDATA full_data_5f1084[];
extern GAPDATA full_data_75e1c0[];
extern GAPDATA full_data_75e360[];
extern GAPDATA full_data_936460[];
extern unsigned char kb_Last;
extern char ls_SavePath[];
extern unsigned char sv_PopRequest;
extern unsigned char d2_ClickedId;
extern unsigned char ai_GameFlags;
extern unsigned char ai_MissionMode;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern unsigned char d2_Objs;
extern char ai_DataPath[];
extern unsigned char ai_GameOver;
extern unsigned char ai_NumPlayers;




extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern short d2_ScreenH;
extern short d2_ScreenW;





extern GAPDATA full_data_5f320c[];
extern GAPDATA full_data_6f2418[];
extern GAPDATA full_data_6f245c[];
extern int main_ScreenRestart;
extern GAPDATA full_data_6f25b4[];
extern unsigned char init_PlayerProfile[];
extern unsigned char kb_Last;
extern unsigned char sv_FrameBase;
extern unsigned char sv_FrameDepth;
extern unsigned char sv_FrameMask;
extern unsigned char sv_PopRequest;
extern const unsigned char ai_AxisPair;

extern unsigned char ai_OwnPlayer;
extern unsigned char d2_Objs;
extern GAPDATA full_data_624ce0[];
extern GAPDATA full_data_624ce4[];
extern GAPDATA full_data_624ce8[];
extern GAPDATA full_data_624cec[];
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindowPtr;
extern GAPDATA full_data_624cf8[];
extern GAPDATA full_data_75e180[];
extern GAPDATA full_data_75e198[];
extern GAPDATA full_data_75e5f0[];
extern unsigned char init_PlayerProfile[];
extern unsigned char kb_Last;
extern unsigned char sv_PopRequest;
extern GAPDATA full_data_75e5f5[];
extern GAPDATA full_data_75e628[];
extern unsigned char ai_ClickedObject;
extern unsigned char ai_Unk5f8438;
extern unsigned char ai_Unk5fb5b0;
extern unsigned char ai_Unk633738;
extern unsigned char ai_Unkfc2dec;
extern short d2_ScreenH;
extern short d2_ScreenW;
extern unsigned char d3t_CamMove;
extern unsigned char d3t_CamMoving;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_624938[];
extern char ai_DataPath[];
extern unsigned char ai_OptionsFile;
extern unsigned char d2_Unkf8a670;




extern unsigned char init_PlayerProfile[];
extern unsigned char kb_Last;
extern unsigned char sv_PopRequest;
extern unsigned char ai_GameFlags;
extern unsigned char ai_OptionHistorie;
extern unsigned char d2_ClickedId;

extern GAPDATA full_data_624938[];
extern GAPDATA full_data_75e5ec[];
extern GAPDATA full_data_75e5f0[];
extern GAPDATA full_data_75e5f4[];
extern GAPDATA full_data_75e628[];
extern unsigned char hst_TextsOn;
extern unsigned char init_PlayerProfile[];
extern unsigned char kb_Last;
extern unsigned char sv_FrameMask;
extern unsigned char sv_PopRequest;
extern unsigned char ai_MissionMode;
extern unsigned char d2_ClickedId;
extern unsigned char ai_Difficulty;
extern unsigned char ai_GamePanel;
extern unsigned char ai_Opponents;
extern unsigned char d2_LastWindowPtr;






extern unsigned char kb_Last;
extern unsigned char sv_PopRequest;
extern unsigned char ai_ClickedObject;
extern char ai_DataPath[];
extern unsigned char ai_NumPlayers;
extern unsigned char ai_Unk60d360;
extern unsigned char ai_Unk6f22e8;
extern unsigned char ai_Unkfc2dec;
extern unsigned char d2_ClickedId;
extern short d2_ScreenH;
extern short d2_ScreenW;

extern unsigned char d3_ScaleX;
extern unsigned char d3_Unk6333fc;
extern unsigned char d3_ZFar;
extern unsigned char d3_ZNear;


extern GAPDATA full_data_5f1084[];
extern GAPDATA full_data_602568[];
extern unsigned char kb_Last;
extern unsigned char sv_FrameMask;
extern unsigned char sv_PopRequest;
extern unsigned char ai_GameFlags;
extern unsigned char ai_NumPlayers;
extern void *d2_ScreenSurf;  /* 0x6ed924, the screen surface (d2.c) */  /* 0x5fe710, 0x6ed924, 0x6ed938 */
extern unsigned char ai_GameFlags;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern short d2_ScreenH;
extern short d2_ScreenW;
extern struct D2VIEWV d2_Views[];
extern unsigned char d3_ScaleX;
extern unsigned char d3_ZFar;
extern unsigned char d3_ZNear;
extern unsigned char init_StatusForm;
extern unsigned char cm_RequestedSaveName;
extern unsigned char ai_Unk1460ae0;
extern void *d2_ScreenSurf;  /* 0x6ed924, the screen surface (d2.c) */  /* 0x5fe710, 0x6ed924, 0x6ed938 */
extern unsigned char ai_MissionSeed;
extern GAPDATA full_data_75df20[];
extern unsigned char ai_FallbackBuilding;
extern unsigned char ai_GameOver;
extern unsigned char ai_GameState;
extern unsigned char ai_LeaveGame;
extern unsigned char ai_PlayerBuilding;
extern struct MAINCLOCK ai_Time;
extern int sv_ScreenDirty;
extern unsigned char ai_Unk75ec60;
extern unsigned char ch_SndLib;
extern unsigned char cm_RequestedSaveName;
extern unsigned char cm_SpeedLevel;
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
extern unsigned char d3_FogByDistance;
extern unsigned char ai_Unk6f0d8c;
extern unsigned char ai_Unk760618;
extern unsigned char ai_Unk76061c;
extern unsigned char ai_Unk760620;
extern unsigned char ch_EmptyName;
extern unsigned char ch_Motions;
extern unsigned char d3_Sky;
extern unsigned char d3t_CamMove;
extern unsigned char d3t_SunRays;
extern GAPDATA full_data_6f25c4[];
extern unsigned char sv_FrameMask;
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk1460ae0;
extern unsigned char ai_BTypes;
extern unsigned char ai_GameFlags;
extern unsigned char ai_GamePanel;
extern unsigned char ai_MissionMode;
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_Unk6f2438;
extern unsigned char ai_Unkf83650;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern unsigned char d2_ClickedId;
extern short d2_ScreenH;
extern short d2_ScreenW;
extern GAPDATA full_data_624560[];
extern GAPDATA full_data_6253b0[];
extern GAPDATA full_data_6253b4[];
extern GAPDATA full_data_6253b8[];
extern GAPDATA full_data_760454[];
extern unsigned char he_RunHandlers;
extern unsigned char kb_Last;
extern unsigned char sv_PanelForm;
extern unsigned char sv_PopRequest;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;


extern unsigned char ai_Unk6f1bb8;
extern char ai_Unk925520[];
extern GAPDATA full_data_92bca8[];
extern unsigned char he_Events;
extern unsigned char he_QuickList;
extern char ai_Chars[];
extern unsigned char ai_PlayerId;
extern struct MAINCLOCK ai_Time;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern unsigned char ai_Unkfc2dec;
extern unsigned char d2_ClickedId;
extern unsigned char d2_LastWindowPtr;
extern short d2_ScreenH;
extern short d2_ScreenW;
extern GAPDATA full_data_6024e4[];
extern GAPDATA full_data_624560[];
extern GAPDATA full_data_6f25b0[];
extern GAPDATA full_data_6f25c0[];
extern unsigned char kb_Last;
extern unsigned char sv_FrameMask;
extern unsigned char sv_PanelFlag;
extern unsigned char sv_PopRequest;
extern void *d2_ScreenSurf;  /* 0x6ed924, the screen surface (d2.c) */  /* 0x5fe710, 0x6ed924, 0x6ed938 */
extern char ai_Chars[];
extern unsigned char cm_FieldObject;
extern unsigned char ai_MissionMode;
extern unsigned char ai_OwnPlayer;


extern GAPDATA full_data_75e401[];
extern GAPDATA full_data_75e5ec[];
extern GAPDATA full_data_75e5f0[];
extern GAPDATA full_data_75e5f5[];
extern GAPDATA full_data_75e628[];
extern unsigned char init_PlayerProfile[];
extern unsigned char ai_BTypes;
extern unsigned char ai_Difficulty;
extern unsigned char ai_GameFlags;
extern unsigned char ai_GameOver;
extern unsigned char ai_GameState;
extern unsigned char ai_PlayerId;
extern struct MAINCLOCK ai_Time;
extern unsigned char ai_Unk1460ae0;
extern unsigned char ai_Unkfb9060;
extern unsigned char ai_Unkfb9064;
extern unsigned char ai_Unkfb9068;
extern unsigned char ai_Unkfb906c;
extern unsigned char cm_NetFlags;
extern unsigned char d2_ClickedId;
extern short d2_ScreenH;
extern short d2_ScreenW;
extern GAPDATA full_data_624560[];
extern unsigned char init_BuildingUniverse;
extern unsigned char snd_AmbientOn;
extern unsigned char sv_FrameMask;
extern unsigned char sv_PopRequest;
extern void *d2_ScreenSurf;  /* 0x6ed924, the screen surface (d2.c) */  /* 0x5fe710, 0x6ed924, 0x6ed938 */
extern unsigned char ai_BTypes;
extern unsigned char ai_Blds;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerChar;
extern struct MAINCLOCK ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char cm_FieldObject;
extern unsigned char ai_CurrentPlayer;
extern char ai_DataPath[];
extern unsigned char ai_Difficulty;

extern unsigned char ai_FrameWait;
extern unsigned char ai_GameFlags;
extern unsigned char ai_GameOver;
extern unsigned char ai_GamePanel;
extern unsigned char ai_LeaveGame;
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_PlayerId;
extern struct MAINCLOCK ai_Time;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk1460ae0;
extern unsigned char ai_Unk60bd94;
extern unsigned char ai_Unk60c1ec;
extern unsigned char ai_Unk6f2440;
extern unsigned char ai_Unkf31144;
extern unsigned char ch_EmptyName;
extern unsigned char cm_NetFlags;
extern short d2_ScreenH;
extern short d2_ScreenW;

extern unsigned char d3_ScaleX;
extern unsigned char d3_ZFar;
extern unsigned char d3_ZNear;

extern GAPDATA full_data_62456c[];
extern GAPDATA full_data_6f2428[];
extern GAPDATA full_data_6f2598[];
extern GAPDATA full_data_6f25a8[];
extern GAPDATA full_data_6f25c4[];
extern GAPDATA full_data_75e1c0[];
extern GAPDATA full_data_75e360[];
extern GAPDATA full_data_760454[];
extern GAPDATA full_data_76f990[];
extern GAPDATA full_data_76f994[];
extern unsigned char init_StatusForm;
extern char ls_SavePath[];
extern unsigned char ls_SaveSlot;
extern unsigned char main_MoneyCheat;
extern unsigned char sv_FrameMask;
extern int sv_ViewRectBottom;
extern int sv_ViewRectLeft;
extern int sv_ViewRectRight;
extern int sv_ViewRectTop;
extern GAPDATA full_data_624798[];
extern GAPDATA full_data_6f241c[];
extern unsigned char ai_SoundOptions;
extern short d2_ScreenH;
extern short d2_ScreenW;
extern unsigned char di_Disabled;
extern GAPDATA full_data_6f2574[];
extern unsigned char kb_Last;
extern unsigned char msx_FadeTo;
extern unsigned char snd_AmbientOn;
extern char ai_DataPath[];
extern unsigned char ai_GameFlags;
extern unsigned char ai_GfxResolution;
extern unsigned char ai_LeaveGame;
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
extern int snd_SystemBank;
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
extern int main_ScreenRestart;
extern GAPDATA full_data_6f25a8[];
extern GAPDATA full_data_6f25b4[];
extern GAPDATA full_data_75e1c0[];
extern char main_AppDir[];
extern GAPDATA full_data_75e5f0[];
extern GAPDATA full_data_75e5f4[];
extern GAPDATA full_data_75e628[];
extern unsigned char init_PlayerProfile[];
extern char ls_SavePath[];
extern unsigned char main_MoneyCheat;
extern unsigned char sv_FrameDepth;
extern unsigned char sv_PopRequest;

#define U_I(a) (*(int *)(a))

extern char main_ScreenOpen;
extern int main_PlatformNT;
extern int main_ScreenSaverOff;
extern HWND main_Window;
extern int d2_CutsceneActive;           /* 0x6ed9d4, gfx.h */
extern int snd_SfxEnabled;              /* 0x6f257c, f3snd.h */
extern int snd_MusicEnabled;            /* 0x6f2578, f3snd.h */

void main_GameLoop(int ctl);
void main_Unk537f40(void);
void main_Unk537be0(void);
void main_Unk538140(void);
void main_Unk5384e0(void);
void main_Unk539870(void);

void main_Unk53a520(unsigned short pl);
void main_Unk53cb40(void);

void d2_FreeItemListSurfaces(void);                /* 0x477420 */ void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b);
int ai_SaveOptions(void);               /* 0x5d4c70, unk_5d33b0.c */
void d2_Exit(void);                     /* 0x468a90, d2.h */
typedef struct VIDCFG {          /* the video record d3_Init reads: d3_engine.h's D3INITCFG (that header cannot be included here: its externs conflict with this unit's data views) */
    unsigned int w, h;
    int bpp, mode, d3d;
    int triple;                  /* d3_engine.h has an unsigned char + pad here; the original stores a dword */
    int maxpolys, texcount, maxhicol, texParam, zbpp, texbpp;   /* texParam: tex_Init's 3rd argument */
    int flFlag, flShift, flArg, texShift, shadowSize;           /* d3fl_Init (0x400, 0x40 >> flShift, flArg, flFlag), d3d_TexShift, d3_InitShadows' first argument (names from their use in d3_Init) */
    float viewW, viewH, viewX, viewY, zn, zf, scale;
} VIDCFG;
char d3_Init(VIDCFG *cfg, HWND hwnd, void *hinst);   /* 0x43bc20, d3_engine.h */
int ai_LoadOptions(void);               /* 0x5d5640, unk_5d33b0.h */
void ai_ApplyOptions(void);             /* 0x5d5e90, cm.h */
void ai_ApplyGfxOptions(void);          /* 0x5d5c40, unk_5d33b0.h */
int d2_Open(int unk, int w, int h, char *dir, char *file);  /* 0x473c10, d2.h */
void d2_CreateItemListSurfaces(void);   /* 0x477320, gfx.h */
char dd_CheckModes(int mode, int bpp, HWND parent, int c);  /* 0x41d230, dd_vesa.h */
void d3if_CreateCameras(void);          /* 0x448910, d3_interface.h */
void d3io_SetStockDir(char *path);      /* 0x44c510, d3_int_io.h */
void tex_SetPalette(char *dir);         /* 0x45fb30 */
struct D2FADE { unsigned int flags; };  /* the head of d2.h's D2FADE (flags: 4 = done) */
extern unsigned int d3anim_CacheMax;   /* 0x5f927c, d3_anim.h */
int txt_LoadIncludes(char *file);       /* 0x42c9c0 */
int txt_OpenText(char *file, int mode);  /* 0x42cbc0 */
void err_Message(char *text);           /* 0x4222b0 */
void hst_ResetLists(void);              /* 0x4b1f30, hst.c */
void d2_ClearScreen(void);              /* 0x46e990, d2.h */
int d2_OpenForm(int a, int b, char *name);  /* 0x4741f0, d2.h */
void d2_PlaceWindow(int win, unsigned char flags);  /* 0x4756b0, d2.h */
void d2_MoveObj(short x, short y, int obj);  /* 0x46cda0, d2.h */
int io_PrintText(char *fmt, ...);       /* 0x515960 */
int net_Connect(char *host, unsigned short port);  /* 0x424990 */
void ai_InitStates(void);               /* 0x4c3480 */
void ai_ClearTurnTable(void);           /* 0x587de0, unk_575860.h */
int gm_LoadTables(char *root);          /* 0x4fe570, f3gm.h */
void unk_ResetEffectSlots(void);        /* 0x4eddf0 */
void unk_LoadIconNames(void);           /* 0x4fde90 */
float gm_ItemPrice(int item, unsigned char n);  /* 0x5087f0 */
int sfx_Startup(void);                  /* 0x42b720 */
int sb_Init(int mem, int count, int a, int b, int c);  /* 0x425bc0 */
int d3snd_Init(int max);                /* 0x417650 */
int d3sndw_Init(unsigned short n);      /* 0x417ec0 */
void sb_SetPath(char *path);            /* 0x426240 */
int sb_LoadSampleBank(char *name, int flags);  /* 0x426950 */
int main_PreloadSound(char *f);         /* 0x537c70 */
int msx_Init(int a, int b, int c);      /* 0x423830 */
void msx_SetPath(char *path);           /* 0x4243d0 */
void ai_ApplyVolumes(void);             /* 0x5d5f10, unk_5d33b0.h */
void sv_InitTick(void);                 /* 0x520ee0 */
char init_ResetSceneUniverses(void);    /* 0x50d460 */
int ch_Open(void);                      /* 0x40afb0 */
void d3t_CreateSnowMipmaps(void);       /* 0x416980 */
int ai_Init(void);                      /* 0x47d9e0 */
int ahm_LoadPrototypes(int mode);       /* 0x4eb480 */
int hmt_CreateBinaryFile(void);         /* 0x4ac440 */
void hmt_Init(void);                    /* 0x4aca90 */
void ai_BuildFloatTable(void);          /* 0x4d27a0 */
void d2_CloseView(int view);            /* 0x4758f0, d2.h */
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags);  /* 0x4767d0, d2.h */
void d2_ReleaseFade(struct D2FADE *f);  /* 0x476860, d2.h */
void evtc_Register(void);               /* 0x40b8a0 */
void ocmd_Register(void);               /* 0x40f960 */
void chc_Register(void);                /* 0x40d270 */
void sndc_Register(void);               /* 0x410220 */
void d3if_Reset(void);                  /* 0x4473f0, cm.h */
int ls_SaveGame(char *path, char *name, unsigned flags);  /* 0x5190b0, ls.c (as unk_5d33b0.h) */
int ls_GetLanguage(void);               /* 0x5190a0, ai.h */
int init_InitTown(char *city);          /* 0x510ad0, f3init */
void gm_ResetCharacters(void);          /* 0x503e80, f3gm */
void init_FreeScenes(void);             /* 0x50d490, f3init */
char d3_LoadScene(char *name, void *f, void (*progress)(int a, int i, int n));  /* 0x44be60, cm.h */
struct D2SURF { int makeFlags; int w; int h; };  /* the head of d2.h's D2SURF (d2.h cannot be included here: its externs conflict with this unit's data views) */
void d2_BlitPart(int sx, int sy, int w, int h, struct D2SURF *src, int x, int y, struct D2SURF *dst);  /* 0x479480, d2.h */
int d2_LockSurface(struct D2SURF *s);   /* 0x479020, d2.h */
int d2_UnlockSurface(struct D2SURF *s);  /* 0x4790e0, d2.h */
char d3if_SelectScene(int index, char keep);  /* 0x4478e0, cm.h */
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);  /* 0x447190, d3_interface.h */
void d3t_FreeSnowMipmaps(void);         /* 0x4175c0, d3_tools.h */
void d3_Shutdown(void);                 /* 0x43c240, d3_engine.h */
void gm_FreeIconNames(void);            /* 0x4fdfb0, f3gm.c */
void main_FreeSoundBanks(void);         /* 0x537db0 */
void sv_UnregisterTick(void);           /* 0x520f10, sv.c */
void sb_Exit(void);                     /* 0x425ce0, sblib.c */
void d3snd_Exit(void);                  /* 0x4176f0, d3_snd.c */
void msx_Exit(void);                    /* 0x4238e0, msx.h */
void d3sndw_Exit(void);                 /* 0x418030, d3_snd.c */
void he_SaveMessageFilter(void);        /* 0x575600, he.c */
void gm_Free(void);                     /* 0x4fe9c0, f3gm.c */
void ai_FreeSettings(void);             /* 0x5a8920, unk_58bbd0.c */
void ahm_FreePrototypes(void);          /* 0x4ebf60, ahm.h */
int hmt_Exit(void);                     /* 0x4acde0, hmt.h */
void gv_FreeEffectObjects(void);        /* 0x4ede70, gv.h */
void ch_Close(void);                    /* 0x40b1a0, ch.h */
void ai_ResetMessages(void);                /* 0x4c3560 */
void txt_Format(char *out, int id0, ...); /* 0x5176d0 */
void tb_Exit(void);                     /* 0x42e940, tb.h */
void vfs_Exit(char warn);               /* 0x431510, vfs.h */
void tr_shutdown(void);                 /* 0x42ebc0, tr.h */
void mem_Exit(void);                    /* 0x422910, mem.h */
void err_Exit(void);                    /* 0x422250, err.h */
void err_Init(int started, void (*crashCB)(HANDLE), HWND hwnd, unsigned int flags);  /* 0x4220b0, err.h */
void d3io_NoopCallback();               /* 0x44ee40, d3_io_a.h: the empty default callback, called with no argument (K&R on purpose) */
void mem_Init(int max);                 /* 0x4226a0, mem.h */
int tr_startup(unsigned short n);       /* 0x42ea90, tr.h */
int sprintf(char *buf, const char *fmt, ...);                     /* 0x5de652 */
char vfs_Init(char *root, char unk);    /* 0x431490, vfs.h */
int tb_Init(UINT delay, int oneshot);   /* 0x42e8c0, tb.h */
char dd_Restore();                    /* 0x41bdd0 */
char d3d_Restore();                    /* 0x4370d0 */
void d2_FlushDirty();                     /* 0x468e30 */
void d2_DrawObjects();                     /* 0x46d3d0 */
void dd_Flip();                     /* 0x41e390 */
void d2_BlitPart();                     /* 0x479480 */
void amt_CheckOffices(void);            /* 0x4e9260, amt.h */

#include "time_view.h"

