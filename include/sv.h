/* Declarations of sv.c, the game screen unit (the frame loop, the screen layout, the name plates, the
   selection, the clock): its functions and data as the definitions spell them. Names are ours. */
#ifndef SV_H
#define SV_H

#include "sv_types.h"

typedef struct SVSKYREC {               /* 0x4c bytes, a sky layer: the texture and its scale / v scale / v offset (d3sky_CreateLayer) */
    char name[0x40];
    float scale;                        /* +0x40, 3.0 in every record */
    float vscale;                       /* +0x44, 0.8 */
    float voffset;                      /* +0x48, -0.25 */
} SVSKYREC;
#pragma pack(push, 1)
typedef struct SVDAYPHASE {             /* 10 bytes, a phase of the day in sv_DayTables (sv_FillDayPhases) */
    char hour;                          /* +0 */
    char min;                           /* +1 */
    int sec;                            /* +2, the second of the day it starts at */
    char pad6[4];                       /* +6, 0 (the last phase's are the first sky key's colour) */
} SVDAYPHASE;
#pragma pack(pop)

void sv_UpdateFramesPerTick(void);                                      /* 0x520ec0 */
void sv_InitTick(void);                                                 /* 0x520ee0 */
void sv_UnregisterTick(void);                                           /* 0x520f10 */
void sv_ResetScreenClip(void);                                          /* 0x520f20 */
int sv_CheckMoney(char *o, int n);                                      /* 0x520f90 */
int sv_CheckCharacterFlag(int id);                                      /* 0x520fe0 */
int sv_CheckGoodAmount(char *o, int n, unsigned t);                     /* 0x521040 */
int sv_CheckStock(short *p, int stock, int n, unsigned t);              /* 0x5210a0 */
int sv_MessageBox(int text, unsigned flags);                            /* 0x521150 */
int sv_MessageBoxGreen(int a, int b, unsigned flags);                   /* 0x521410 */
int sv_MessageBoxYesNo(int text, unsigned flags);                       /* 0x5216a0 */
int sv_OpenMessageBox(int text, unsigned char flags);                   /* 0x521900 */
void sv_SelectPanelObject(unsigned char *o, int b, char *c);            /* 0x521960 */
void sv_DrawPanelPointer(void);                                         /* 0x521ba0 */
void sv_SceneLoadProgress(int kind, int cur, int total);                /* 0x521c10 */
void sv_BuildScreenLayout(int s);                                       /* 0x521e70 */
int sv_PanelImage(void);                                                /* 0x522040 */
void sv_SetPanelEnabled(int n);                                         /* 0x522100 */
void sv_Unk522280(void);                                                /* 0x522280 */
void sv_DestroyScreen(void);                                            /* 0x5223c0 */
void sv_LoadGroundplan(void);                                                /* 0x5224a0 */
void sv_UpdateGroundplan(unsigned flags);                                      /* 0x5229b0 */
void sv_SetResearchClass(unsigned char *o);                             /* 0x522ec0 */
void sv_UpdateCityPanel(unsigned char a, int b);                              /* 0x522f10 */
void sv_RedrawScreen(void);                                             /* 0x524010 */
void sv_AddGroupMember(int c, char *g);                                      /* 0x524100 */
void sv_RemoveGroupMember(int c, char *g);                              /* 0x524160 */
int sv_UsedGroupSlots(char *g);                                         /* 0x5241b0 */
void sv_CountGroupMembers(char *g);                                     /* 0x5241d0 */
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);       /* 0x524260 */
int sv_IsAtDoor(char *g, char *o);                                     /* 0x524770 */
int sv_IsCartAtDoor(char *g, unsigned char *o);                            /* 0x524810 */
int sv_CharStateIcons(char *c, int *out);                                    /* 0x524900 */
void sv_UpdateMemberList(unsigned char *g);                                    /* 0x524990 */
void sv_OpenPlayerBar(int a, char k);                                   /* 0x525220 */
void sv_ClosePlayerBar(void);                                           /* 0x525440 */
int sv_ClockSeason(void *t);                                            /* 0x525480 */
char sv_SetSkyLightColors(struct D3OBJECT *o, void *ctx);                                           /* 0x525490 */
struct GCLOCK; void sv_SetupWeather(struct GCLOCK *t, int reroll);/* 0x525510 */
void sv_UpdateSkyColors(void);                                          /* 0x525940 */
struct GCLOCK; void sv_FillDayPhases(struct GCLOCK *t);                                             /* 0x525a00 */
char sv_SetLampRamp(char *o, int f);                                      /* 0x525a80 */
struct AITIME; void sv_UpdateSkyPhase(struct AITIME *t);                    /* 0x525ad0 */
void sv_PlaceCamera(float f);                                           /* 0x525dd0 */
void sv_EaseCameraHeight(void);                                                /* 0x525ee0 */
void sv_BorderCameraView(void);                                                /* 0x525ff0 */
int sv_ScrollCamera(void);                                                 /* 0x5263a0 */
int sv_MouseCamera(void);                                                 /* 0x526cf0 */
int sv_UpdateCamera(void);                                                 /* 0x527640 */
void sv_FlyCameraToObject(int o);                                               /* 0x527840 */
void sv_CameraToObject(int o);                                               /* 0x527bb0 */
void sv_FlyCameraToModel(int m);                                               /* 0x527e90 */
void sv_Unk528140(void);                                                /* 0x528140 */
struct D3OBJECT; struct AIBLD; char sv_CreateBannerCB(struct D3OBJECT *part, struct AIBLD *b);                             /* 0x528180 */
char sv_ColourBannerCB(char *o, char *b);                                    /* 0x528290 */
void sv_Unk5282f0(char *b);                                             /* 0x5282f0 */
void sv_ColourBanners(int c);                                               /* 0x528390 */
struct AIBLD; void sv_UpdateChimneySmoke(struct AIBLD *b);                                /* 0x528490 */
char sv_Unk528660(char *o, int *p);                                     /* 0x528660 */
void sv_Unk5286a0(char *b);                                             /* 0x5286a0 */
void sv_Unk528770(void);                                                /* 0x528770 */
int sv_AddPanelButton(int x, int y, int text);                               /* 0x528800 */
struct AIBLD; void sv_OpenBuildingPanel(struct AIBLD *b);                                /* 0x528860 */
void sv_OpenItemPanel(unsigned short *p);                                   /* 0x528b80 */
struct AIOBJ; void sv_OpenCartPanel(struct AIOBJ *c);                                /* 0x528d90 */
void sv_OpenDefaultPanel(void);                                                /* 0x528ee0 */
struct AICHAR; void sv_OpenPersonPanel(struct AICHAR *c);                                 /* 0x5290f0 */
void sv_RefreshPanel(void);                                                /* 0x529380 */
void sv_ClosePanel(void);                                                /* 0x529fb0 */
int sv_ReturnZero(void);                                                /* 0x52a040 */
void sv_UpdatePanel(void);                                              /* 0x52a050 */
void sv_HoverPick(void);                                                /* 0x52a680 */
void sv_ResetSelection(void);                                                /* 0x52adb0 */
void sv_ClearSelection(void);                                           /* 0x52ae40 */
void sv_Unk52ae70(void);                                                /* 0x52ae70 */
void sv_CollectGroupCharacters(char *b);                                /* 0x52b2a0 */
int sv_Unk52b350(int a, char *name, int c, int d, int e);               /* 0x52b350 */
int sv_Unk52b3c0(char *c, int b, char *out);                            /* 0x52b3c0 */
int sv_Unk52b490(char *a, int b);                                       /* 0x52b490 */
void sv_Unk52b500(unsigned char *a);                                    /* 0x52b500 */
void sv_Unk52bb70(int id);                                              /* 0x52bb70 */
void sv_Unk52bbc0(void);                                                /* 0x52bbc0 */
void sv_Unk52bec0(void);                                                /* 0x52bec0 */
void sv_Unk52c420(void);                                                /* 0x52c420 */
void sv_ReleaseBubble(int o);                                           /* 0x52c4a0 */
void sv_Unk52c4e0(int o, int b, char *name);                            /* 0x52c4e0 */
void sv_Unk52c5c0(void);                                                /* 0x52c5c0 */
void sv_Unk52cef0(void);                                                /* 0x52cef0 */
void sv_Unk52d010(char *part);                                          /* 0x52d010 */
void sv_Unk52d1e0(void);                                                /* 0x52d1e0 */
void sv_Unk52d2a0(void);                                                /* 0x52d2a0 */
void sv_Unk52d480(void);                                                /* 0x52d480 */
void sv_CameraFollow(void);                                             /* 0x52d7a0 */
void sv_Unk52d940(unsigned fl);                                         /* 0x52d940 */
void sv_Unk52e050(void);                                                /* 0x52e050 */
void sv_Unk52e0c0(void);                                                /* 0x52e0c0 */
void sv_ClearNamePlates(void);                                          /* 0x52e170 */
void sv_ReleaseNamePlates(void);                                        /* 0x52e190 */
int sv_RegisterNamePlate(char *name, char *text, int b);                /* 0x52e1c0 */
void sv_SetStatusText(char *s);                                         /* 0x52e330 */
void sv_ButtonRow(int f, int y, int *out, int n, ...);                  /* 0x52e350 */
void sv_Unk52e4b0(int f, int y, int *out, int n, ...);                  /* 0x52e4b0 */
void sv_DisableVisibleWidgets(int a);                                   /* 0x52e600 */
void sv_Unk52e6d0(int f, int b, int c);                                 /* 0x52e6d0 */
int sv_CreateFormSlider(int x, int y, int f, int g, int p5, int *b, int *a, int *c); /* 0x52e860 */
void sv_FitForm(int f);                                                 /* 0x52ea30 */
void sv_ValueBar(int x, int y, int value, int k);                      /* 0x52eaa0, value / 21 half tiles of the picture k */
void sv_DrawBalanceBar(int x, int y, int a, int b, unsigned k);              /* 0x52eb50 */
void sv_Unk52ee50(void);                                                /* 0x52ee50 */
void sv_Unk52eed0(void);                                                /* 0x52eed0 */
void sv_Unk52eee0(void);                                                /* 0x52eee0 */
void sv_Unk52f0d0(void);                                                /* 0x52f0d0 */
void sv_Unk52f2c0(void);                                                /* 0x52f2c0 */
void sv_Unk52f340(int *book);                                           /* 0x52f340 */
int *sv_Unk52f360(int a, int b, char n, ...);                           /* 0x52f360 */
void sv_Unk52f530(int *bk);                                             /* 0x52f530 */
void sv_Unk52f600(int bk);                                              /* 0x52f600 */
int sv_Unk52f710(int *bk, char dir);                                    /* 0x52f710 */
void sv_Unk52f9f0(int *bk);                                             /* 0x52f9f0 */
void sv_Unk52fa00(int *bk);                                             /* 0x52fa00 */
int sv_CreateScrollForm(void);                                                 /* 0x52fa10 */
void sv_DestroyScrollForm(void);                                                /* 0x52fa90 */
void sv_Unk52fad0(void);                                                /* 0x52fad0 */
void sv_PopTo(int v);                                                   /* 0x52fb50 */
int sv_PumpMessages(void);                                              /* 0x52fb80 */
void sv_NetworkSyncWait(unsigned fl);                                   /* 0x52fc10 */
void sv_BroadcastText(unsigned fl, char *t);                            /* 0x52fcf0 */
void sv_Unk52fd80(char *t);                                             /* 0x52fd80 */
void sv_PumpNet(void);                                                  /* 0x52fe00 */
int sv_Unk52fe10(void);                                                 /* 0x52fe10 */
void sv_Unk530100(void);                                                /* 0x530100 */
void sv_Unk5301a0(void);                                                /* 0x5301a0 */
void sv_Unk530670(void);                                                /* 0x530670 */
void sv_Unk5307f0(void);                                                /* 0x5307f0 */
void sv_SetScreenCenter(unsigned fl);                                   /* 0x530900 */
int sv_RunFrame(int cb, unsigned mask);                                 /* 0x5309a0 */
void sv_UpdateClock(void);                                              /* 0x5315c0 */

extern const int sv_PhaseMinutes[24];                          /* 0x5eeab8 */
extern int ai_Unk5fcc78;                                    /* 0x5fcc78 */
extern int ai_Unk5fcc7c;                                    /* 0x5fcc7c */
extern int ai_Unk5fcc80;                                    /* 0x5fcc80 */
extern int d2_PointerObj;                                    /* 0x5fe710 */
extern float sv_FramesPerTick;                              /* 0x62378c */
extern int sv_TickFrames;                                   /* 0x623790 */
extern char sv_GroundplanColours[];                                 /* 0x623794 */
extern int sv_StatusTime;                                   /* 0x6237c4 */
extern int sv_PanelRebuild;                                    /* 0x6237cc */
extern float sv_CameraLimits[12];                           /* 0x6237d0 */
extern float sv_CameraHeightNear;                           /* 0x623800 */
extern float sv_CameraAngleNear;                            /* 0x623804 */
extern float sv_CameraHeightFar;                            /* 0x623808 */
extern float sv_CameraAngleFar;                             /* 0x62380c */
extern float sv_ScrollSpeed;                                  /* 0x623818 */
extern char sv_CameraEaseOn;                                   /* 0x62381c */
extern int ai_Unk623820;                                    /* 0x623820 */
extern int ai_Unk623824;                                    /* 0x623824 */
extern int ai_Unk623828;                                    /* 0x623828 */
extern int ai_Unk62382c;                                    /* 0x62382c */
extern int ai_Unk623830;                                    /* 0x623830 */
extern int ai_Unk623834;                                    /* 0x623834 */
extern int ai_Unk623838;                                    /* 0x623838 */
extern int ai_Unk62383c;                                    /* 0x62383c */
extern int ai_Unk623840;                                    /* 0x623840 */
extern int ai_Unk623844;                                    /* 0x623844 */
extern int ai_Unk623848;                                    /* 0x623848 */
extern int ai_Unk62384c;                                    /* 0x62384c */
extern int ai_Unk623850;                                    /* 0x623850 */
extern int sv_PanelPic;                                     /* 0x623854 */
extern int sv_PlayerBarForm;                                /* 0x62385c */
extern int sv_PanelForm;                                    /* 0x623860 */
extern int sv_PanelBldPic;                                    /* 0x623864 */
extern int sv_PanelHealthBar;                                    /* 0x623868 */
extern int sv_PanelWidgets[4];                                 /* 0x62386c, the info panel's 4 widget ids */
extern int sv_PanelCartWin;                                    /* 0x62387c */
extern int sv_PanelPortrait;                                    /* 0x623880 */
extern int sv_PanelBuildBtn;                                    /* 0x623884 */
extern int sv_PanelItemBtn;                                    /* 0x623888 */
extern int sv_PanelTransportBtn;                                    /* 0x62388c */
extern int sv_PanelCartBtn;                                    /* 0x623890 */
extern int sv_PanelActionBtn;                                    /* 0x623894 */
extern int sv_PanelKind1eBtn;                                    /* 0x623898 */
extern int ai_Unk6238a4;                                    /* 0x6238a4 */
extern SVSKYREC sv_SkyFair[4];                              /* 0x6238a8 */
extern SVSKYREC sv_SkyMedium[3];                            /* 0x6239d8 */
extern char sv_DayTables[278];                              /* 0x623ba8 */
extern int ai_Unk623cc0[];                                  /* 0x623cc0 */
extern int ai_Unk623e10[];                                  /* 0x623e10 */
extern int sv_LampPhase;                                    /* 0x623e80 */
extern int ai_Unk623e88;                                    /* 0x623e88 */
extern int sv_GroundplanScene;                                    /* 0x623e8c */
extern float sv_DayPhaseFrac;                                  /* 0x623e94 */
extern int ai_Unk623ea0;                                    /* 0x623ea0 */
extern int ai_Unk623ea4;                                    /* 0x623ea4 */
extern int ai_Unk623ea8;                                    /* 0x623ea8 */
extern int ai_Unk623ec4;                                    /* 0x623ec4 */
extern int ai_Unk623ec8;                                    /* 0x623ec8 */
extern int sv_ViewRectLeft;                                 /* 0x6248a4 */
extern int sv_ViewRectTop;                                  /* 0x6248a8 */
extern unsigned ai_Unk623ecc;                               /* 0x623ecc */
extern int sv_ViewRectRight;                                /* 0x6248ac */
extern int sv_ViewRectBottom;                               /* 0x6248b0 */
extern int ai_DragWindow;                                    /* 0x62a8cc */
extern int sv_PanelCartChar;                                    /* 0x631078 */
extern short d2_PointerLook;                                /* 0x6ed9d0 */
extern int sv_SavedMaxY;                                    /* 0x6f1c40 */
extern float sv_ScrollVelYPrev;                                  /* 0x6f1c44 */
extern int sv_DragLastY;                                    /* 0x6f1c48 */
extern int sv_SavedMaxX;                                    /* 0x6f1c50 */
extern float sv_DragLastPos[3];                               /* 0x6f1c64 */
extern float sv_ScrollLastMove[3];                               /* 0x6f1c74 */
extern int sv_SavedMinX;                                    /* 0x6f1c84 */
extern int sv_DragLastX;                                    /* 0x6f1c88 */
extern int sv_ZoomStartY;                                    /* 0x6f1c8c */
extern int sv_ZoomStartX;                                    /* 0x6f2290 */
extern int sv_SavedMinY;                                    /* 0x6f2298 */
extern int sv_FrameDepth;                                   /* 0x6f229c */
extern int sv_PopRequest;                                   /* 0x6f22a0 */
extern int sv_FrameBase;                                    /* 0x6f22a4 */
extern int sv_CameraScrolled;                                    /* 0x6f22b4 */
extern int ai_Unk6f22c0;                                    /* 0x6f22c0 */
extern struct D2SURF *sv_GroundplanSurf;                                   /* 0x6f22c4 */
extern struct D2SURF *sv_GroundplanView;                                   /* 0x6f22c8 */
extern int sv_BorderViewX;                                    /* 0x6f22cc */
extern int sv_BorderViewY;                                    /* 0x6f22d0 */
extern float sv_CameraHeightFactor;                         /* 0x6f22d4 */
extern float sv_CameraAngleFactor;                          /* 0x6f22d8 */
extern int sv_SelectedModel;                                    /* 0x6f2304 */
extern char * ai_PlayerBuilding;                            /* 0x6f2308 */
extern char * ai_FallbackBuilding;                          /* 0x6f230c */
extern unsigned short * sv_PanelShown;                      /* 0x6f2310 */
extern int ai_SelectedCount;                                    /* 0x6f2334 */
extern int sv_ResearchClass;                                /* 0x6f233c */
extern char sv_LampSwitches;                                   /* 0x6f2340 */
extern char sv_MessageBoxOpen;                              /* 0x6f2350 */
extern char sv_MessageBoxGreenOpen;                         /* 0x6f2351 */
extern char sv_MessageBoxYesNoOpen;                         /* 0x6f2352 */
extern int sv_ScreenDirty;                                  /* 0x6f2358 */
extern int sv_GroundplanBuilding;                                    /* 0x6f235c */
extern int sv_GroundplanRoom;                                    /* 0x6f2360 */
extern char sv_PlayerBarKind;                                   /* 0x6f2368 */
extern int sv_DayPhase;                                    /* 0x6f236c */
extern int sv_ThunderFog;                                    /* 0x6f2370 */
extern float sv_CameraHeightStep;                                  /* 0x6f2374 */
extern int sv_BorderMoved;                                    /* 0x6f2378 */
extern float sv_ScrollVelX;                                  /* 0x6f2388 */
extern float sv_ScrollVelXPrev;                                  /* 0x6f238c */
extern float sv_ScrollVelY;                                  /* 0x6f2390 */
extern int sv_ScrollTime;                                    /* 0x6f2394 */
extern int sv_DragStartX;                                    /* 0x6f2398 */
extern int sv_DragStartY;                                    /* 0x6f239c */
extern int sv_ZoomTurnOn;                                    /* 0x6f23a0 */
extern int sv_MouseCamActive;                                    /* 0x6f23a4 */
extern float sv_ZoomStartHeight;                                  /* 0x6f23a8 */
extern float sv_ZoomStartAngle;                                  /* 0x6f23ac */
extern float sv_ZoomStartTurn;                                  /* 0x6f23b0 */
extern float sv_ZoomBaseY;                                  /* 0x6f23b4 */
extern int sv_MouseDragOn;                                    /* 0x6f23b8 */
extern int sv_ShownModel;  /* 0x6f2294 */
extern int sv_PanelPerms;  /* 0x6f2320 */
extern int sv_PickX;  /* 0x623e98 */
extern int sv_PickY;  /* 0x623e9c */
extern int sv_HoverLast;  /* 0x6f1c4c */
extern int sv_HoverItem;  /* 0x6f22f0 */
extern int sv_HoverBuilding;  /* 0x6f22f4 */
extern int sv_HoverContact;  /* 0x6f2300 */
extern int sv_HoverBldPerms;  /* 0x6f2324 */
extern int sv_HoverItemPerms;  /* 0x6f2328 */
extern int sv_HoverCharPerms;  /* 0x6f232c */
extern int sv_PointerSpriteFor;  /* 0x6f23dc */
extern unsigned char sv_OtherSelected;  /* 0x6f2338 */
extern int sv_ShownBuilding;  /* 0x6f23bc */
extern int sv_ShownFallback;  /* 0x6f23c0 */
extern int sv_ShownObject;  /* 0x6f23c4 */
extern int sv_ShownRoom;  /* 0x6f23c8 */
extern int sv_ShownItem;  /* 0x6f23cc */
extern int sv_ShownChar;  /* 0x6f23d0 */
extern int sv_ShownOther;  /* 0x6f23d4 */
extern int sv_ShownCount;  /* 0x6f23d8 */
extern unsigned char ai_GameState;                          /* 0x6f25ac */
extern int ai_OptionScrollSpeed;                            /* 0x6f3614 */
extern int sv_PanelFlag;                                    /* 0x75e8c0 */
extern char * sv_PanelObject;                               /* 0x75e8c4 */
extern unsigned short * sv_PanelRecord;                     /* 0x75e8c8 */
extern char sv_PanelText[76];                               /* 0x75e8d4 */
extern int sv_SelectionFlags[0xc0];                         /* 0x75e920 */
extern int sv_SkyScene;                                    /* 0x75ec20 */
extern unsigned short * sv_SelectedItem;                       /* 0x75ec5c */
extern int ai_Unk75f280[576];                               /* 0x75f280 */
extern float sv_ViewPos[4];                                 /* 0x75fb80, the stored camera position (X/Y/Z views: data_views.h) */
extern float sv_ViewRot[4];                                 /* 0x75fb90, its rotation */
extern float sv_SkyScale;                                  /* 0x760420 */
extern int sv_BandActive;                                    /* 0x760440 */
extern int sv_SkyPhase;                                    /* 0x760464 */
extern int sv_CloudChance;                                    /* 0x760480 */
extern int sv_SeasonClimate;                                    /* 0x760484 */
extern SVWEATHER sv_Weather;                                /* 0x760488 */
extern int sv_WinterWeather;                                    /* 0x760610 */
extern int sv_WeatherHour;                                    /* 0x760614 */
extern int d3t_Snow;                                    /* 0x76061c */
extern int d3_SkyLayers[6];                                 /* 0x760620, the sky layer handles (sv creates 0 .. 2) */
extern char sv_StatusText[128];                             /* 0x761f40 */
extern int sv_Bubbles[0x40][0x43];                          /* 0x761fc0, the 64 speech bubble records (0x10c bytes) */
#ifndef DATA_VIEWS_H
extern int sv_BubbleObjects[4286];                          /* 0x761fc8, the object field (+8) of each bubble record */
#endif
extern int sv_SelectedChar;                                    /* 0x7662c0 */
extern int sv_GroundplanButtons[15];                                /* 0x7662e0 */
extern int sv_SelectedExtra;                                       /* 0x76631c */
extern int cm_Bombs[0x40];                                  /* 0x935c00 */
extern int d2_HitWindow;                                    /* 0xf8aad0 */
extern int ai_Unkfb9060;                                    /* 0xfb9060 */
extern int ai_Unkfb9064;                                    /* 0xfb9064 */
extern int ai_Unkfb9068;                                    /* 0xfb9068 */
extern int ai_Unkfb906c;                                    /* 0xfb906c */
extern int ai_Unkfba9c0[168];                               /* 0xfba9c0 */
extern char * sv_CameraObj;                                 /* 0xfc2dec */

#endif
