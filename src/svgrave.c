/* svgrave -- the graveyard windows (0x582730 .. 0x587e00), split from sv_t; the
   name is ours. The grave size buttons, list and detail (ai_GraveWindow), the rent-a-grave, move and release
   confirmations, the GRAEBER_PFLEGEN / GEHILFEN_ERSCHAFFEN callbacks, the helpers window, the fog / seance / haunting /
   insurance contacts, the graveyard chatter tick.
   Boundary: sv_t's .data strings end with "contact_VERGIFTEN" 0x62a524 (user 0x58263e, the cabinet contacts 0x582600);
   this object's plain .data block is ai_GraveChatterAt 0x62a538 (-1); its .rdata is the named grave tables
   ai_GravePics .. ai_GraveGoods 0x5eecb0 .. 0x5eecf0 after the cannon literals (1450.0f 0x5eecac), first read by
   ai_GraveSizeButtons 0x582730 (0x58276b).
   End: the tooltip / bookmark code 0x587e00.. is its own object (svtip.c, data layout s11): its plain .data block
   ai_DragWindow 0x62a8cc follows this object's last string (the chatter's "locations\friedhof\gespraeche.esc"
   0x62a8a8). ai_ClearTurnTable 0x587de0 uses no .data / .bss (only the common ai_TurnTable): it stays here, the
   bound between the last reader of this object's data (0x587c90) and the first of svtip's (0x587e00); bprobe at
   0x587c90 .. 0x588160 changes nothing (ai_BookmarkWindow 16 -> 15 at each).
   Open: the head declarations are sv_t.c's as the split copied them; trim them to what the unit uses. */
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
#define d2_DirtyRectTop (*(int *)((char *)&d2_DirtyRectLeft + 0x4))       /* 0xfb9064 */
#define d2_DirtyRectRight (*(int *)((char *)&d2_DirtyRectLeft + 0x8))     /* 0xfb9068 */
#define d2_DirtyRectBottom (*(int *)((char *)&d2_DirtyRectLeft + 0xc))    /* 0xfb906c */
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
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;


extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
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
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;









extern GAPDATA full_data_602568[];
extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;


extern int sv_PopRequest;
extern unsigned short ai_PlayerId;
extern volatile unsigned long d3_Time;
extern GAPDATA full_data_60c7b8[];
extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern int d2_PointerObj;
extern const int sv_PhaseMinutes[24];


extern unsigned short ai_PlayerId;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;









extern GAPDATA full_data_62a450[];
extern unsigned char kb_Last;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;



extern int sv_FrameMask;
extern int sv_PopRequest;
extern char ai_Chars[768][0x21c];
extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern int d2_PointerObj;
extern int d2_ClickedId;
extern int d2_LastWindow;


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

extern int sv_PopRequest;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;





extern GAPDATA full_data_62a450[];
extern GAPDATA full_data_62a464[];
extern int ai_ClickedObject;

extern unsigned short ai_PlayerId;
extern AITIME ai_Time;                   /* 0x770ae0, the game clock */
extern char ai_Chars[768][0x21c];
extern int d2_PointerObj;
extern int gm_AllowAll;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern int sv_PopRequest;
extern int ai_BuildingPerms;
extern int ai_ClickedObject;


extern int gm_AllowAll;
extern volatile unsigned long d3_Time;
extern char ai_Chars[768][0x21c];
extern int d2_PointerObj;
extern struct D2OBJ *d2_Objs;
extern int di_Absolute;
extern int gm_PlayerRecords[688];
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
extern char ai_Chars[768][0x21c];


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
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
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
extern int ai_TutorialBanks[6];
extern TUTRUN *ai_TutorialRuns;
extern int ai_TutorialOn;
extern int ai_TutorialRunning;
extern int ai_ShotDirty;
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
unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: tr.c defines it returning int; this unit's callers read the 16-bit result (mov si,ax / movzx): the original's units saw a 16-bit declaration */
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
        d2_Objs[w[8 + i]].user = &ai_Goods[ai_GraveGoods[i]];
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
   @deep s36: the trait loop as for (k = 0; k < 5; k++, y += 0xd) (the original increments k before y: inc
   ebx / add esi,0xd): 22 -> 20 ins, kept. Left: the constant 0 web only (c2trace: const#0#28 lines 717-795
   takes ebp in pass 7 at savings 0; the original's 0 piece ends at the fill loop, push 0 / test after it)
   and the n=0 slot store before the first loop. No lever (= control): items local dropped for ((GVWIN
   *)w)->items (24), the fill through items[] (24, other code), loop 1 through w->items, NULL, !flag, c !=
   0, item = items + i, separate obj locals for the text objects / trait labels, saved = 0 moved after the
   fill (22, other bytes). climb round 1: nothing. prior: draft 30 -> shape 22 ins. Kept: the labels "%s:"
   / "%1N3" (literals of this unit's run since s11); the item fill as the plain index loop ((GVWIN
   *)w)->items[i].grave / id / btn (gives the original's lea [n*3+0x51] form; the int-pointer walk is
   gone); the collect loop as items[n].f with n++ (the item++ walk: 24, a count-free bound in the for: 34).
   Left: the constant 0 web: the original zeroes ebp at entry, re-zeroes it only on the collect loop's path
   (xor ebp,ebp before the join) and later pushes immediates / test; ours keeps const#0 in ebp after the
   main loop (push ebp x2 for sv_CreateScrollButtons, cmp esi,ebp / mov [esp+0x10],ebp, cmp eax,ebp, an
   extra xor ebp,ebp after the main loop, the n spill before the collect loop moving the jle target).
   Tried: 6 fill store orders (all worse), saved = 0 in an else (25), n = 0 in the for-init / while fill /
   init order (= one code), flip.py --want two push 0 (25 mutants, only meaning-changing ones closer: the
   fill's grave = 0 constant is the reference that decides). Open: M10 100.0f vs ai_FloatHundred (no view
   of full_rdata_5ebb40 in sv_t). Next: c2trace.py on the const#0 pieces (ids 1/2/3 of 6 webs) to see which
   web takes ebp. (tiebreak: not measured (dcp headers sample ai_GraveDetail)) */
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
        for (k = 0; k < 5; k++, y += 0xd) {
            txt_Format(buf, "%s:", 0x16b7 + k);
            x = d2_CreateWinText(0x28, y, buf, w[3]);
            d2_Objs[x].w = 300;
            d2_SetObjFont(x, 0x54);
            sv_ValueBar(0xa0, y, item->grave->traits[k], 0x4df);
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
            txt_Format(buf, "%1N3", (unsigned short)*c);
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
   @deep match-deep 2026-11-14 (256 ins kept; headers sample: insensitive, 32 samples one codegen). Read
   again: the original puts the right-button sv_PopRequest store out of line (cmp [rup],ebp / jne, the
   store cross-jumped with the flag-1 one): if (dm_L.rup == 0) { if (d2_ClickedId != -1) {..} } else
   sv_PopRequest = 1; gives that layout (and the && form the same code) but 312 ins: the allocation is the
   real difference. c2trace on that form: rec is queued at prio 124 and gets ebx, out at 94 is split (its
   loop piece goes to the stack); in the original rec lives on the stack ([esp+0x1f4] at every use), out in
   edi, w[0x4d] in ebx, ebp the cached zero that becomes the walking pointer of the three click loops (lea
   ebp,[esp+0x6c] / +0x80 / +0x18c) and is re-zeroed at the loop latch (xor ebp,ebp behind the entry jmp).
   Next: find the reference that the original's out had more (or rec fewer) in the loop - the out fill /
   pick fill blocks and the three redraw calls are the candidates; the redraw-bit / handled flags are as
   the original (for (..; !handled && i < N; ..) matches the bottom test). (tiebreak: insensitive) prior:
   reconstructed in full from the original (every instruction read: the setup with the seven section
   windows, the frame loop with the size-button, row, dead-person and slider clicks, the out / callback
   picks, the three redraw bits); the statements and the branch structure match (each hunk's jumps go to
   the same statements). Left is allocation: the original keeps out in edi, w[0x4d] in ebx and ebp as the
   zero / loop pointer, rec on the stack ([esp+0x1f4] at every use); ours keeps rec in ebx, w[0x4d] in ebp
   and out on the stack, so every out store, gv_EntryIndex(rec->yard) push and redraw call differs by
   register (the regs / code hunks 0x583247 .. 0x583376, 0x5834ad .. 0x5835f1, 0x583644 .. 0x5836a7); with
   ebp free the original strength-reduces the size-button loop to a walking pointer (lea ebp,[esp+0x6c];
   add ebp,4) where ours indexes [esp+eax*4+0x6c], keeps the redraw bits in memory across ... */
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
   @deep deep s39: nothing kept (31). What the original has: n in esi and e in ebx (ours e esi / n ebx), no
   home store of n = 0 at entry and ONE store behind the collect loop's join (cmp esi,8 / mov
   [esp+0x18],esi; the count <= 0 jle lands on the cmp). c2trace: pass 2 serves e#2 (prio 76) before n#2
   (74) and the first served takes esi; --prio: block 0 (P 7) gives e +28 / n +14, the test block (P 4, w
   2) e +16 / n -8, the fill guard and preheader n +4 / +30, the pre-show block (P 13) n +26 / e -13: n
   needs +3. flip.py --want 'cmp esi, 8': 30 hits, the count-keeping ones all change a const-0 reference
   (items[i].grave = 1, sv_CreateScrollButtons(1, 0 ..), show loop i = 1, n++ doubled,
   sv_CreateScrollButtons doubled): one more or one fewer zero reference flips the pair, as in
   ai_GraveDetail. = control (no lever): for (i = n = 0 ..), for (i = n ..), n = 0 in the for-init, e
   before n / count before n, dead inits (saved / item / x / y / k / i / count = 0), a second variable for
   the count behind the collect loop (used = n, count = n, the fill loop on n itself); a GVGRAVE *g block
   pointer in the collect loop: 62. climb round 1: nothing (184 mutants). prior: draft 70 -> 31 ins. Kept:
   the collect and fill loops as plain index loops on ((GVDETAILWIN *)w)->items[n] / [i] (the item++ walk
   and the int-pointer fill walk gone: 70 -> 33, the e / n stores and the IV base on the id field now as
   the original), the labels "%s:" / "%1N3" (literals since s11), the trait loop's y += 0xd in the for
   increment (the original's inc before add esp: order 16 -> 12). Left: e and n swap registers (ours e esi
   / n ebx, the original n esi / e ebx: c2trace has the e piece at prio 76 vs n 74, so e is served first),
   plus the n = 0 home store at entry (mov [esp+0x18],esi before the loop; the original stores n once after
   it). flip.py --want 'mov ebx, [edi+28]' found 27 hits, all removing a const 0 reference
   (sv_CreateScrollButtons 0,0 -> 1: 7 ins; the fill's grave = 0 -> 1: 7; the show loop's i = 0: 11) or an
   e reference (e->grave read through w->entry in the loop: 8; count from w->size: 7, a meaning change) -
   the same const#0 web as the sibling ai_GraveDetail. No lever: init order n / e / count (3 orders,
   for-init n = 0, entry field spelling, declaration order: = control), continue / n++ in the index / while
   fill (= control), a GVGRAVE *g walk (50), id through items[n].grave (55), btn before id (66), the item
   pointer for-walk in the show loop (45). Open: M10 100.0f vs ai_FloatHundred (no view in sv_t) (tiebreak:
   not measured (dcp headers sample ai_RentGraveDetail)) */
void ai_RentGraveDetail(int *w)
{
    GVENTRY *e;
    GVITEM *item;
    int count;
    int n;
    int i;

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
    count = gv_EntryCounts[e->size];
    for (i = 0; i < count; i++) {
        if (e->grave[i].id != -1) {
            ((GVDETAILWIN *)w)->items[n].grave = &e->grave[i];
            ((GVDETAILWIN *)w)->items[n].id = e->grave[i].id;
            ((GVDETAILWIN *)w)->items[n].btn = -1;
            n++;
        }
    }
    for (i = n; i < 8; i++) {
        ((GVDETAILWIN *)w)->items[i].grave = 0;
        ((GVDETAILWIN *)w)->items[i].id = -1;
        ((GVDETAILWIN *)w)->items[i].btn = -1;
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
        for (k = 0; k < 5; k++, y += 0xd) {
            txt_Format(buf, "%s:", 0x16b7 + k);
            x = d2_CreateWinText(0x28, y, buf, w[2]);
            d2_Objs[x].w = 300;
            d2_SetObjFont(x, 0x54);
            sv_ValueBar(0xa0, y, item->grave->traits[k], 0x4df);
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
        d2_Objs[btns[i]].user = &ai_Goods[ai_GraveGoods[i]];
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
                if (cost == 0 || sv_CheckGoodAmount(ai_Chars[ai_PlayerId], cost, ai_OwnPlayer)) {
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
                if (cost == 0 || sv_CheckGoodAmount(ai_Chars[ai_PlayerId], cost, ai_OwnPlayer)) {
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
   ends the window */
void ai_GypsyHelperWindow(AIBLD *rec)
{
    AIBTYPE *type;
    AIJOB *job;
    char *res;
    int win;
    int counts[3];
    int btns[3];
    int kinds[3];
    int n;
    int i;
    int j;
    int k;
    int sel;
    int id;
    int h;
    int x;

    kinds[2] = 0x227;
    res = gm_Query((void *)rec->objList, 2, 6, 0, 0x2a);
    type = &ai_BTypes[rec->type];
    if (type->kind == 0x1f) {
        kinds[0] = 0x293;
        kinds[1] = 0x294;
        n = 2;
        if (type->cap247 >= 2) {
            kinds[2] = 0x295;
            n = 3;
        }
    } else {
        kinds[0] = 0x225;
        kinds[1] = 0x226;
        n = 2;
        if (type->cap247 >= 2) {
            kinds[2] = 0x227;
            n = 3;
        }
    }
    if (sv_CheckCharacterFlag((char *)rec) != 0)
        return;
    memset(counts, 0, sizeof(counts));
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
        ai_UpdateProdGoodsList(rec, res);
        ai_LayoutProdGoodsList(win, 1, 0, rec);
        memset(counts, 0, sizeof(counts));
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
            if (type->kind == 0x1f) {
                if (counts[i] == 1)
                    txt_Format(line, 0x421f, 0x22d, kinds[i] * 2 + 0x9a8);
                else if (counts[i] > 1)
                    txt_Format(line, 0x4220, counts[i], 0x22d, kinds[i] * 2 + 0x9a8);
                else
                    txt_Format(line, "%s", kinds[i] * 2 + 0x9a8);
            } else {
                if (counts[i] == 1)
                    txt_Format(line, 0x4129, 0x228, kinds[i] * 2 + 0x9a8);
                else if (counts[i] > 1)
                    txt_Format(line, 0x412a, counts[i], 0x228, kinds[i] * 2 + 0x9a8);
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
   form closes; a right click or leave closes it too. The camera follows again at the end */
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
    if (target == 0) {
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, "Locations\\Friedhof\\Fog");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x40b7, 0x40b6, target->owner, target);
    factor1 = d2_WinOpenSlider(100, 0xa0, 0xc8, 0, 2, 0x7b, 0x82, d2_LastWindow);
    d2_SetObjValue(factor1, 0, 2, 0);
    d2_SetSliderFlags(factor1, 0x82);
    d2_SetLabel(factor1, txt_Ptrs[0x40bf]);
    d2_SetList(factor1, 3, txt_Ptrs[0x40c0], txt_Ptrs[0x40c1], txt_Ptrs[0x40c2]);
    factor2 = d2_WinOpenSlider(100, 0xbe, 0xc8, 0, 2, 0x7b, 0x82, d2_LastWindow);
    d2_SetObjValue(factor2, 0, 2, 0);
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
   Closes the form; returns the script */
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
        GVGRAVE *g = &ev->grave;
        name1 = (char *)gm_NameRecord(ai_CharsT[ai_PlayerId].sex, ai_CharsT[ai_PlayerId].jobId);
        if (name1 == 0)
            gm_NameRecord(ai_CharsT[ai_PlayerId].sex, 0);   /* @original drops the result: name1 stays 0 */
        name2 = (char *)gm_NameRecord(g->sex, g->nameId);
        if (name2 == 0)
            name2 = (char *)gm_NameRecord(g->sex, 0);
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
        r = util_Random(3);
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
        snd_QueueVoice(voice, 0, "GEIST_KOMMT", util_Random(8), 0xfa);
    } else if (mode == 1) {
        r = util_Random(0x200);
        n = (r + 0x200) << 5;
        cm_SendMoveGoods(ai_CharsT[ai_PlayerId].netId, -1, ai_OwnPlayer, n);
        io_PrintText(0x40f0, 0x40cc, n);
        snd_QueueVoice(voice, 0, "GEIST_KOMMT", util_Random(8), 0xfa);
    } else {
        io_PrintText(0x40d3, 0x40cc);
        snd_QueueVoice(voice, 0, "GEIST_KOMMT_NICHT", util_Random(8), 0xfa);
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
        snd_QueueVoice(voice, 0, "GEIST_KOMMT", util_Random(8), 0xfa);
        io_PrintText(0x40f1, 0x40cc, amount);
    } else {
        snd_QueueVoice(voice, 0, "GEIST_KOMMT_NICHT", util_Random(8), 0xfa);
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
        i = util_Random(0xf);
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
        snd_PlayVoice(voice, 0, "GEIST_KOMMT", util_Random(8));
    } else if (mode == 1) {
        r = util_Random(0x400);
        n = (r + 1000) << 5;
        cm_SendMoveGoods(ai_CharsT[ai_PlayerId].netId, -1, ai_OwnPlayer, n);
        io_PrintText(0x40f3, 0x40cc, n);
        snd_PlayVoice(voice, 0, "GEIST_KOMMT", util_Random(8));
    } else {
        io_PrintText(0x40d3, 0x40cc);
        snd_PlayVoice(voice, 0, "GEIST_KOMMT_NICHT", util_Random(8));
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
    d2_SetObjValue(slider, 4, 0xc, 4);
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

    bldg = &ai_BTypes[((AIBLD *)b)->type];
    v = gm_TitleJob(((AICHAR *)ch)->id, gm_BuildingJob((unsigned char)bldg->kind));
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
            if (v == (&bldg->jobBytes)[i]) {
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
    n = util_Random(cnt);
    x = list[n];
    if (cnt > 1) {
        n = util_Random(cnt);
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

/* dcp unit split: the declarations that moved to svtip (copies: drop what svgrave does not use) */
int gm_BuildingKindCost(int c, int kind); char *strupr(char *); int sprintf(char *buf, const char *fmt, ...);
float gm_GoodsPrice(unsigned id, unsigned char p);
int ai_AmountRowStatus(); int ai_DrawAmountRow(); short * gm_FindChar(int id); int gm_TitlesLeft();
char * gm_FindBuildingById(int id); int gm_FindObjectInst(int id); unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c);
int sv_CreateScrollButtons(); void d2_RadioClear(int g);
unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);
int ls_Read();
void * mem_Alloc(int size, char *tag); char dd_Lock(void); void dd_Unlock(); unsigned char bmp_Save24(char *path, int w, int h, unsigned char *data); void mem_Free(void *p);
void ai_Quicksave(void); void ai_ApplyOptions(); void cm_RaiseSpeedLevel(void); void cm_LowerSpeedLevel(void);
int gm_BuildingMainObject(unsigned char *b); void *gm_GetGebaeudeHauptraum(unsigned char *b);
unsigned char * gm_BuildNext(void); unsigned char * gm_BuildQuery(int n, ...);

/* the unit's data in the original's order: .rdata the grave tables 0x5eecb0 .. 0x5eecf0 (named constants after the cannon
   literals, before lc.c's); .data ai_GraveChatterAt 0x62a538 (the plain block after sv_t.c's strings); the
   common row only this unit reads */
/* the four grave sizes of the graveyard windows (ai_GraveSizeButtons, ai_GraveSizeButtonsAt; ai_GraveMoveConfirm names the grave by
   its size; the sizes 1 .. 3 are open while GVYARD.open[i] is set); only this unit reads them, they stay out of
   sv_t.h: declared above ai_TutorialInitChapter2 they flip its register tie-break (byte -> shape) */
const int ai_GravePics[4] = { 0x2d5, 0x2d8, 0x2dc, 0x2e0 };      /* 0x5eecb0, the buttons' pictures */
const int ai_GravePicsAlt[4] = { 0x7da, 0x7db, 0x7dc, 0x7dd };   /* 0x5eecc0, the pictures ai_GraveList uses by the button */
const int ai_GraveNames[4] = { 0xd94, 0xd9a, 0xda2, 0xdaa };     /* 0x5eecd0, the texts of the sizes' names */
const int ai_GraveGoods[4] = { 0x1f6, 0x1f9, 0x1fd, 0x201 };     /* 0x5eece0, the ai_Goods index whose record the button keeps */
unsigned int ai_GraveChatterAt = 0xffffffff;  /* 0x62a538, d3_Time at the last graveyard chatter tick (ai_GraveyardChatter runs every 0x1194 ms) */
unsigned char ai_TurnTable[0x4000];  /* 0x6f5f60, the 0x4000-byte table ai_ClearTurnTable clears once at engine open; no other code refers to it (its role is taken from that function's name), it ends where ai_ShotTable starts */
