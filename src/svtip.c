/* svtip -- the right-button tooltips (ai_HandleItemDrag 0x587e00 ..), the character info window, the function-key
   bookmarks, the screenshot and the slot / speed keys (0x587e00 .. 0x589f10), split from svgrave; the name is ours.
   Start: this object's plain .data block ai_DragWindow / ai_DragObj / ai_DragViewWin 0x62a8cc (all -1) follows
   svgrave's last string ("locations\friedhof\gespraeche.esc" 0x62a8a8, ai_GraveyardChatter); its first reader is
   ai_HandleItemDrag. Its .bss ai_DragPicked 0x6f266c .. 0x6f267c, its common group ai_Bookmarks 0x6f5ec0.
   unit 0x00587e00..0x00589f10; boundaries in decl/text.dcl */
struct AHMSTEP;
struct D3SKY;
#include "d2_types.h"
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sv_t.h"
#include "svgrave.h"
#include "svtip.h"
#include "txt.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#include "ai.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_5f1890[];
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_602568[];
extern unsigned char kb_Last;


#include "f3init_types.h"
extern GAPDATA full_data_6024f8[];
extern GAPDATA full_data_624204[];



extern const int sv_PhaseMinutes[24];
extern float d3s_QuadUV[6];
extern GAPDATA full_data_5fea38[];

extern GAPDATA full_data_602568[];


extern GAPDATA full_data_602568[];
extern GAPDATA full_data_5f36e0[];











extern struct D2OBJ *d2_Objs;




extern GAPDATA full_data_602568[];
extern int sv_PopRequest;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;





extern GAPDATA full_data_602568[];
extern int sv_PopRequest;
extern int d2_PointerObj;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;





extern GAPDATA full_data_602568[];
extern int sv_PopRequest;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern GAPDATA full_data_602568[];
extern int sv_PopRequest;







extern GAPDATA full_data_62a450[];
extern GAPDATA full_data_62a464[];
extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_602568[];

extern int sv_PopRequest;
extern int ai_BuildingPerms;




extern int sv_PopRequest;
extern const int sv_PhaseMinutes[24];
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_60c7b8[];
extern GAPDATA full_data_623594[];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;


extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern const int sv_PhaseMinutes[24];
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern int sv_FrameMask;



extern int sv_PopRequest;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

extern GAPDATA full_data_62a634[];
extern int sv_FrameMask;
extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;









extern GAPDATA full_data_602568[];
extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;


extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern volatile unsigned long d3_Time;
extern GAPDATA full_data_60c7b8[];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern int d2_PointerObj;
extern const int sv_PhaseMinutes[24];


extern unsigned short ai_PlayerId;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;









extern GAPDATA full_data_62a450[];
extern unsigned char kb_Last;
extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;



extern int sv_FrameMask;
extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;


extern int sv_FrameMask;
extern int sv_PopRequest;

extern unsigned short ai_PlayerId;

extern const int sv_PhaseMinutes[24];


extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern int sv_PopRequest;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;





extern GAPDATA full_data_62a450[];
extern GAPDATA full_data_62a464[];
extern int ai_ClickedObject;

extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int gm_AllowAll;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern int sv_PopRequest;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;


extern int gm_AllowAll;
extern volatile unsigned long d3_Time;
extern int d2_PointerObj;
extern struct D2OBJ *d2_Objs;
extern int di_Absolute;
extern unsigned short ai_PlayerId;
extern int d2_LastWindow;
extern unsigned char ai_OwnPlayer;
extern const float gm_MoodRanges[476];
extern GAPDATA full_data_602568[];
extern int sv_PopRequest;
extern unsigned char kb_Last;

extern int d2_LastWindow;

extern int sv_PopRequest;

extern int sv_FrameMask;
extern unsigned char kb_Last;

extern unsigned char kb_Last;
extern int sv_FrameMask;


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
extern TUTSTATE *const ai_Tutorial;
#define S (*(char **)(unsigned)&ai_Tutorial)  /* the tutorial state record (at 0x6fa060): the pointer ai_Tutorial holds */
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
extern int ai_TutorialChapterOn[5];
extern int (*ai_TutorialChapterInit[5])(void);
extern unsigned int ai_GraveChatterAt;
extern int ai_DragObj;
extern int ai_DragViewWin;
extern int ai_TutorialBanks[6];
extern TUTRUN *ai_TutorialRuns;
extern int ai_TutorialOn;
extern int ai_TutorialRunning;
extern int ai_ShotDirty;
extern char *ai_DragPicked;
extern int ai_DragCutscene;
extern int ai_ScreenshotCount;
extern AIBOOKMARK ai_Bookmarks[11];
extern unsigned char ai_TurnTable[0x4000];
extern SHOTRESULT ai_ShotTable[3];

#include "ls.h"
#include "f3init.h"
#include "ahm.h"
#include "ai.h"
#include "am.h"
#include "amt.h"
#include "anm.h"
#include "bmp.h"
#include "f3gm.h"
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
extern const float ai_CannonPitchMax;
extern const float ai_CannonPitchMin;
extern const float ai_CameraPitchMin;
extern const float ai_CameraPitchMax;
extern const float ai_CameraPitchZero;
extern const struct D3VECTOR4 ai_WimpelPos;
extern const float ai_CannonSwitchMs;
extern const float ai_CannonSwitchDist;
#include "d3_pose.h"
#include "dd_vesa.h"
#include "menu.h"
#include "d2.h"
#include "sv.h"
#include "gfx.h"

/* dcp unit split: the declarations of sv_t before the moved functions */
int ai_TutorialInitMainIntro();
int ai_TutorialInitMainOutro();
void ai_TutorialStop();
int sb_UnloadSampleBank();
int snd_LoadVoiceBank();
void * mem_Alloc(int size, char *tag);
void ai_TutorialFreeRuns(TUTRUN *rec);
struct G5CH; struct G9REC; struct VFSFILE;
int sv_RunFrame(int cb, unsigned mask);
void sv_ButtonRow(int f, int y, int *out, int n, ...); int sv_MessageBoxGreen(); void sv_ValueBar();
char * ahm_OwnerChar();
int sv_CheckGoodAmount(char *o, int n, unsigned char t);
int sv_MessageBox();
void txt_Format(char *out, int id0, ...);
int ahm_SendCreateAlchemist();
int ahm_ResearchState(); char * ahm_OwnerChar(); AHMGRID * ahm_RandomRecipe();
#pragma pack(push, 1)
typedef struct CANNONREC {   /* the tutorial's cannon record (ai_CannonScene's argument), read at its byte offsets */
    char pad0;
    int id;                  /* +0x01, the effect id gv_FindEffect takes (GVCANNON.bld) */
    char pad5[0x5c];
    D3OBJECT *obj;           /* +0x61, the cannon object the scene is built around */
} CANNONREC;
#pragma pack(pop)
typedef struct CANNONSCENE { /* 0x104 bytes (0xe4 cleared), the scene struct of ai_CannonScene (the other cannon functions still address it by byte offset) */
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
    float pad100;            /* +0x100, bmin's fourth float: the original's frame keeps 4 bytes behind it */
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
double _CIfmod();
#include <math.h>
void d3_MatTranspose(); float d3_VecAngle2(); int ai_GetDefaultEffect();
typedef struct CANNONROT {  /* ai_CannonCamera's rotation / position vector: three floats and a word cleared as an int */
    float x, y, z;
    int w;
} CANNONROT;
void d3_DirToWorld();
int gm_FindById();
float gv_SalvoCharge(); void gv_BuildingCannon(char *o, unsigned *a, unsigned *b); int gv_CannonCost(); float ai_GetDefaultEffectScale();
int _ftol(); float ai_CannonPitchRatio(CANNONSCENE *sc);
void txt_Format(char *out, int id0, ...); int gm_CharCurrency(char *c, char p);
void gv_CannonScatter(char *o, float *a, float *b); float gv_CannonItemFactor(); void ai_ShotTableReset(); int ai_InitShot(struct SHOTREC *shot, struct D3VECTOR *pos, struct D3VECTOR *dir, float charge, float size, float density);
int ai_ScatterShot(struct SHOTREC *shot, float sideSpread, struct D3VECTOR *side, float otherSpread, struct D3VECTOR *other, float speedSpread); int gv_FireShots(struct AIBLD *bld, struct AICHAR *ch, struct SHOTREC *a, struct SHOTREC *b, struct SHOTREC *d, float v, int cost, int x); struct SBBANK; struct SBSAMPLE *sb_PlayVariation(struct SBBANK *bank, char *name, int idx, int loops, int pan, int volume, int fade); void ai_CannonBuildPanel();
int he_EventOnBuilding(); int sv_MessageBox();
int gm_GetWorth(unsigned short id); int d2_WinOpenSlider(); int sv_CheckGoodAmount(char *o, int n, unsigned char t); short * gm_PlayerRecord(char *c);
int cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out); int snd_PlayVoice(int id, int bank, char *text, int var); void sv_CameraFollow();
int ai_AmountMenu(); int amt_GetKindLevel(int k); int sv_CheckMoney(); void cm_SendMoneyDelta(int a, int b);
void txt_Format(char *out, int id0, ...); int sv_MessageBox();
void sv_ReleaseNamePlates(void); int sv_RegisterNamePlate(); void ai_StorageWindow();
void ai_AlchemistResearchWindow(); void ai_AlchemistWindow();
void cm_BeginFieldBlock(char *o, int id); int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldDeltas(void);

/* dcp unit split: the definitions of sv_t the functions of svgrave use */
void ai_AlchemistResearchWindow(unsigned char *obj);
void ai_AlchemistWindow(unsigned char *obj);
void ai_CannonAim(CANNONSCENE *sc, float dt);
void ai_CannonAxes(CANNONSCENE *sc);
int ai_CannonFireCheck(CANNONSCENE *sc);
void ai_CannonShotResults(CANNONSCENE *sc);
float ai_CannonPitchRatio(CANNONSCENE *sc);
void ai_CannonSwitchAnim(char *rec, CANNONSCENE *sc, GVEFFECT *run, int sel, int mask);
void ai_CannonExplosion(CANNONSCENE *sc, char *rec);
void ai_ShotTableReset(void);
extern const int ai_GravePics[4];
extern const int ai_GravePicsAlt[4];
extern const int ai_GraveGoods[4];

/* dcp unit split: the declarations of svgrave before the moved functions */
int sv_CreateScrollButtons(); unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l); int gv_EntryIndex(char *r, int *e);
typedef struct GVROW {
    int btn;                    /* 0x00, its button object, -1: an unused row */
    GVENTRY *entry;             /* 0x04, the grave entry, 0: unused */
    int size;                   /* 0x08, the entry's size (list), 0xff: unused */
    unsigned char index;        /* 0x0c, the entry's index in its list (gv_EntryIndex), 0xff: unused */
} GVROW;
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
void ai_GraveDeadName(GVGRAVE *e, char *out);
typedef struct GVPICK {
    GVENTRY *entry;             /* 0x00, the selected grave entry */
    int size;                   /* 0x04, its size (list) */
    unsigned char index;        /* 0x08, its index in that list (gv_EntryIndex) */
    GVGRAVE *grave;             /* 0x0c, the dead person picked in it, 0: the grave itself (the slider) */
    int id;                     /* 0x10, that dead person's id, -1: none */
} GVPICK;
int gv_OrderSetGraveHolder(); int gv_SizeOpen();
int sv_MessageBoxGreen(); void sv_OpenPlayerBar(int a, char k); void sv_ClosePlayerBar(void);
int gv_RoomForRaised(unsigned char *o, int *n); int gv_OrderRaiseDead(); int cm_GetSlotValue(int h); int sprintf(char *buf, const char *fmt, ...);
int sv_CheckCharacterFlag(int id); char * he_FindEvent(int n, ...); char * he_NextEvent(void); void sv_OpenWindowFrame(int f, int b, int c);
void ai_InitProductionTables(void); int ai_UpdateProdGoodsList(); void ai_LayoutProdGoodsList(int param_1, int param_2, int param_3, int param_4);
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay); int ai_MapPick();
typedef struct GVSEANCE {
    int kind;                   /* 0x00, the seance chosen: 1 .. 3, 0 none */
    int cost;                   /* 0x04, its price: 4 / 6 / 8 */
    int size;                   /* 0x08, the grave's size (list) */
    unsigned char index;        /* 0x0c, the grave's index in that list */
    char pad0d[3];
    int graveId;                /* 0x10, the dead person's GVGRAVE.id */
    GVGRAVE grave;              /* 0x14, a copy of the dead person */
} GVSEANCE;
int sb_IsInit(); void d2_PlaceViewWindows(int view, unsigned char flags);
int snd_VoiceVariant(); char *gv_FindAccused(char *c, int max, int *ids, int *cnt); float gv_SeanceChance(); float util_RandomF();
void cm_SendProofExecuted(int a, int b); 
void ai_FindWantedItems(struct G5CH *p, int n, struct G9REC *rec, int *st);
void gm_SendMoneyDelta(int a, int b);
void ai_EndCharExtra(char *ch, int f); int cm_SendRelations(int a, int b, int c, int d); int sv_StartPoseScript(char *a, int b);
void sv_ReleaseNamePlates(void); void ai_TradeContactChecks(void);
int ai_AmountMenu(); int sv_CheckMoney();
float gm_GetSkill(char *c, int skill); int gm_ClockSeason(); char * he_FindEvent(int n, ...);
int gm_BuildingJob(); 

/* dcp unit split: the definitions of svgrave the functions of svtip use */
void ai_GraveDeadName(GVGRAVE *e, char *out);
void ai_EndCharExtra(char *ch, int f);

int ai_DragWindow = -1;  /* 0x62a8cc, the tooltip view the right-button tooltip handler 0x587e00 opens (-1 = none; sv.c reads it): the first of the .data block before ai_DragObj */
int ai_DragObj = -1;  /* 0x62a8d0, the right-held d2 object (d2_RightHeldObj) whose tooltip ai_DragWindow shows, -1 = none */
int ai_DragViewWin = -1;  /* 0x62a8d4, the first window of the tooltip view ai_DragWindow (d2_Views[].wins[0]), -1 = none */
char *ai_DragPicked = 0;  /* 0x6f266c, the picked object (ai_PickedObject) the tooltip was last opened for, 0 = none */
int ai_DragCutscene = 0;  /* 0x6f2670, 1 while the tooltip view holds d2_CutsceneActive set */
int ai_ScreenshotCount = 0;  /* 0x6f2674, the number of the next screenshot (gamedata/screenshots/gilde%04i.bmp, ai_SaveScreenshot) */
int svg_UnreadBss = 0;  /* 0x6f2678, no surviving instruction or pointer refers to it: data of functions /OPT:REF removed; size from the neighbours' addresses */
AIBOOKMARK ai_Bookmarks[11];  /* 0x6f5ec0, the function-key building bookmarks (ai_BookmarksInit sets the keys F1 .. F11) */

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
            if ((char *)user >= (char *)&ai_Chars[0] && (char *)user < (char *)&ai_Chars[768]) {  /* ai_Chars[768]: one past the table */
                ch = (AICHAR *)user;
                if (ch->id == 0xffff)
                    ch = 0;
            } else if ((char *)user >= (char *)ai_Blds && (char *)user < (char *)&ai_Blds[0x100]) {  /* ai_Blds[0x100]: one past the table */
                bkind = ((AIBLD *)user)->type;
                if (bkind >= 0x58)
                    bkind = 0;
            } else if ((char *)user >= (char *)ai_BTypes && (char *)user < (char *)&ai_BTypes[0x58]) {  /* ai_BTypes[0x58]: one past the table */
                bkind = (AIBTYPE *)user - ai_BTypes;
            } else if ((char *)user >= (char *)ai_Objects && (char *)user < (char *)&ai_Objects[0x5008]) {  /* ai_Objects[0x5008]: one past the table */
                item = ((AIOBJ *)user)->id;
                if (item >= 0x2dc)
                    item = 0;
            } else if ((char *)user >= (char *)ai_Goods && (char *)user < (char *)&ai_Goods[0x2dc]) {  /* ai_Goods[0x2dc]: one past the table */
                item = (AIGOOD *)user - ai_Goods;
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
            g = &ai_Goods[item];
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

int gm_BuildingKindCost(int c, int kind); char *strupr(char *); int sprintf(char *buf, const char *fmt, ...);

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

    bt = ai_BTypes + idx;
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
        io_PrintText(0x28, gm_BuildingKindCost((int)&ai_Chars[ai_PlayerId], idx));
        io_PrintText(0x29, bt->v243);
    }
    return view;
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
   @deep size queue s19: the size gap (raw 1620 vs 1596, padded 1632 vs 1600) is all in the type 0x17/0x25
   chain (ours 0x58869f..0x58885b = 444 bytes, the original 420). The original's jump threading is weaker
   than ours: outer 0x225 / 0x226 -> A, but 0x227 / 0x293 / 0x294 / 0x295 -> a re-test 'cmp 0x226; je A;
   cmp 0x227; jne B-chain' (the inner 0x225 test gone), and its four n = 1 arms share ONE n store (0x1e
   arm, 0x20 arm, second 0x1e arm jmp to the 0x1f arm's 'mov [n],1'), the two 0x1e arms NOT merged with
   each other; ours threads 0x227 straight to A and 0x293.. into the inner test, merges the two 0x1e arms
   whole and keeps own n = 1 stores in the first two arms. So the original's arms were not identical blocks
   at merge time. No lever (rows): list[n++] = K in the n = 1 arms or everywhere (= control); n = 1 / n = 3
   / n = 2 before the list stores (55 / 53 / 55 ins, raw 1620 each); all 719 orders of the outer
   six-compare (best o9 53 ins / 574 bytes, nothing moves the layout). Next: a form where the arms differ
   before cross-jumping (a value per arm through a local, the list written by a helper), or the 0x225 chain
   as a macro shared with the 0x1c1 test at the end. prior: 273 -> 53 ins: d2_WinOpenPicStyled called with
   a short y (its true short x, short y prototype; the row y is recomputed per call, loop counters in
   memory: -101), the staff loop as an index loop over ai_Chars (countdown edx 0x300: -117),
   (float)(c->sk81 * (1.0 / 1008)) + factor (the fxch pair). Left: only the type 0x17/0x25 chain layout
   (all 49 code ins, 0x58869f..0x588843): the original puts each arm inline after an inverted last compare,
   A/B/1c1 with their own n store and the n = 1 arms cross-jumped into one tail; ours places A, B, 1c1
   after the chain (switch-like) and threads 0x293.. into the inner test at 0x227. Tried: inner order (6
   perms, all worse), flat chain without the outer test (162), else-B, B first, sequential ifs, idx <=
   0x227 range, n = 0 inside, outer test negated with swapped arms, 1c1 condition spellings: none moved the
   layout (tiebreak: not measured (dcp headers s... */
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
    int list[32];

    w = ai_FindWeaponRecord(idx);
    item = ai_Goods + idx;
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
        n = 0;
        gm_BuildingSkill((unsigned char *)bld, 1, 1);
        master = (AICHAR *)gm_FindCharacterByMaster((int)bld);
        for (i = 0; i < 0x300; i++) {
            if ((char *)ai_Chars[i].bld == bld && &ai_Chars[i] != master)
                list[n++] = ai_Chars[i].netId;
        }
        factor = gm_StaffFactor((unsigned char *)bld, n, list);
        c = (AICHAR *)gm_FindCharacterByMaster((int)bld);
        if (c == 0)
            c = (AICHAR *)&ai_Chars[((AIBLD *)bld)->owner];
        io_PrintText(0x22, (int)(item->workTime / ((float)(c->sk81 * (1.0 / 1008)) + factor)), item->workTime);
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
   class * 2 + 0xf98), with the object's value; returns the view
   @deep deep s28: only slots differ: the original keeps view in the local slot [esp+8] and puts f into
   idx's dead argument slot; ours packs view into the argument slot and f into the local. c2frame --names:
   view count 6 opens slot 0, f (2) opens 1, idx (param, count 1, only its first load) joins slot 0 (it
   passed slot 1 for a reason the tool does not show). Tried: f a block local of the price else, f folded
   into the call argument (draft 18: the call moves before the fild), f = .. * f / f *= .. / f assigned
   inside the argument (= control), view declared after item, view = -1 before the type guard (folded, =
   control), the loop's returns as break / one guarded print / the type test as an if around the loop (=
   control); climb 3 rounds (47 / 284 / 92 mutants): nothing. Left: slots 16 (view / f / the returns' view
   reloads). */
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

    if (ai_Goods[idx].type == 0x1d)
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
    item = ai_Goods + idx;
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
    bt = ai_BTypes + ((AIBLD *)bld)->type;
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

/* 0x588bb0, opens the help tooltip of the record named s: the key "_HILFE_<S>+0" (the name upper-cased) is
   looked up in the texts; without it -1 (no window). Else the form "ToolTip\ToolTip_Contact", centred, its
   section 1 the text's title line and section 2 the text after it; returns the window */

int ai_OpenHelpTooltip(char *s)
{
    char buf[0x180];
    int v;
    int win;

    strcpy(buf, s);
    strupr(buf);
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
            io_PrintText("$A");
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
   the bookmark keys work while it is open (ai_SelectBookmark)
   @deep deep s38: left i in ebx / o in ebp where the original has i in ebp (it shares ebp with rebuild)
   and o in ebx, plus the view slot [esp+0x28] against the group*20 temp's [esp+0x24] (two count-2 slots of
   a 0x128 frame: the unstable density sort). c2trace: o#1 prio 148 (its block-16 use o->id * 2 + 0x9a8, P
   13) outranks i#1 132 (i is only referenced at the loop top and latch; the bookmark walk is an IV in
   [esp+0x10]), so o is served first and takes ebp. climb round 1 (96 mutants): nothing; o = 0 / b = 0
   placed elsewhere or as else arms: far worse (M8). flip --want 'inc ebp' flips on kb_Last == 1 -> 0 / 2
   (the const 1 web: rebuild = 1 in ebp is also d2_GetViewWin's 1 at the top) and on b = 0 / o = 0 -> 1:
   the constant 0 / 1 webs decide it, not i's references. */
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
    fl = ai_GameFlags;
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
        if (ai_Chars[i].kind == 6)
            break;
    }
    n = 1;
    ai_BookmarkSetBuilding(0, ai_MarketBuilding);
    b = (AIBLD *)gm_BuildQuery(1, 5, 0xf);
    if (b != 0)
        ai_BookmarkSetBuilding(n++, b);
    b = ai_Chars[i].bld2;
    if (b != 0)
        ai_BookmarkSetBuilding(n, b);
    n = 3;
    b = (AIBLD *)gm_BuildQuery(1, 4, i);
    while (b != 0) {
        if (ai_BTypes[b->type].kind != 2)
            ai_BookmarkSetBuilding(n++, b);
        b = (AIBLD *)gm_BuildNext();
    }
}
