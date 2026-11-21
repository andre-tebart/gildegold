/* svcannon -- the cannon scene of the tutorial / the cannon room (0x57e230 .. 0x582730), split from sv_t; the name is
   ours. The shot table callback ai_ShotTableStore (defined before its creator, extern), ai_CannonScene, the aim,
   camera, fire, shot results, barrel switch and explosion, the control panel, then the cabinet's spy windows and
   the alchemist / cannon / cabinet room contacts (0x580fe0 .. 0x582600).
   Boundary: sv_t's literal 400.0f 0x5eec68 (its tutorial progress slider) is followed by this object's named
   constants 0x5eec6c .. 0x5eec9c (the pitch limits, ai_WimpelPos as a 16-byte D3VECTOR4, the unread recoil time
   0x54, the barrel switch time and distance), then its literals in function order: 0.0135f (ai_CannonScene), 0.0111f,
   0.00833f, 1.6f (ai_CannonAim, in reverse order of first use; byte only as literals), 1450.0f (ai_CannonCamera).
   Its .bss ai_ShotDirty 0x6f2664 follows sv_t's 0x6f2660. The first function: 0x57e230 stores into ai_ShotTable,
   whose first user it is (0x57e256); the one before, 0x57e0e0, is the alchemist dismissal window of sv_t's
   "Locations\Domizil\skill" family.
   Open: layout: the boundary rests on 0x5eec68 being a literal (sv_t's only own float; the one 400.0f in the exe, one
   reader, ai_TutorialProgressSlider); a named const would leave sv_t + svcannon one object. The codegen does not tell:
   `extern const float` and `400.0f` compile ai_TutorialProgressSlider to the same instructions; no code bound (bprobe).
   Open: the head declarations are sv_t.c's as the split copied them; trim them to what the unit uses.
   Open: data: the common rows ai_ShotTable 0x6f9f60 / ai_ShotCallbackArg 0x6fa020 stay defined in sv_t.c (their
   order before sv_t's ai_TutorialState 0x6fa060 in the common block). */
struct AHMSTEP;
struct D3SKY;
#include "d2_types.h"
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sv_t.h"
#include "svcannon.h"
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
extern TUTSTATE *const ai_Tutorial;
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
extern int ai_TutorialChapterOn[5];
extern int (*ai_TutorialChapterInit[5])(void);
extern int ai_TutorialBanks[6];
extern TUTRUN *ai_TutorialRuns;
extern int ai_TutorialOn;
extern int ai_TutorialRunning;
extern int ai_ShotDirty;
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
const float ai_CannonPitchMax = 0.7853982f;    /* 0x5eec6c, 45 degrees: the highest pitch of the cannon (ai_CannonAim) */
const float ai_CannonPitchMin = -0.2617994f;   /* 0x5eec70, -15 degrees: the lowest pitch */
const float ai_CameraPitchMin = -0.61086524f;  /* 0x5eec74, -35 degrees: the low end of the camera's pitch range (ai_CannonCamera) */
const float ai_CameraPitchMax = 0.7853982f;    /* 0x5eec78, 45 degrees: its high end */
const float ai_CameraPitchZero = 0.0f;         /* 0x5eec7c, the level camera: (zero - min) is the range the camera pitch moves in */
const D3VECTOR4 ai_WimpelPos = { -70.0f, -50.0f, 120.0f, 0.0f };  /* 0x5eec80, the wimpel's place on the cannon base (16 bytes: the next named constant is at 0x5eec90) */
const int ai_CannonRecoilMs = 0x54;  /* 0x5eec90, the recoil time (sc->recoilStart + 0x54 > sc->now; the code uses the immediate, no reader of this constant survives; name from that use) */
const float ai_CannonSwitchMs = 90.0f;     /* 0x5eec94, the length of each half of the barrel switch (ms, ai_CannonSwitchAnim) */
const float ai_CannonSwitchDist = 100.0f;  /* 0x5eec98, how far the cannon sinks back while the barrel switches */
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

/* dcp unit split: the definitions of sv_t the functions of svcannon use */
void ai_AlchemistResearchWindow(unsigned char *obj);
void ai_AlchemistWindow(unsigned char *obj);

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

extern char ai_ShotCallbackArg[0x40];
extern TUTSTATE ai_TutorialState;

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
   camera and releases the callback of the run record */
void ai_CannonScene(char *rec)
{
    GVEFFECT *run;
    CANNONSCENE sc;
    unsigned short list[0x300];
    int n;
    int m;
    struct {
        int rect[4];
        int scene;
    } saved;                    /* the clip rectangle and the scene to restore: one 20-byte local in the original's frame */
    unsigned mask;
    float dt;
    unsigned char *pb;
    D3VECTOR4 cam3;
    D3VECTOR4 save3;

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
    saved.rect[2] = d2_DirtyRectRight;
    saved.rect[0] = d2_DirtyRectLeft[0];
    saved.rect[1] = d2_DirtyRectTop;
    saved.rect[3] = d2_DirtyRectBottom;
    saved.scene = d3_CurrentScene;
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
        int v;
        D2VIEW *vw;

        for (v = 0; v < 0x30; v++) {
            vw = &d2_Views[v];
            if (vw->active != 0 && vw->enabled != 0) {
                for (i = 0; i < 0x300; i++) {
                    if (d2_Objs[i].type != 0 && (D2VIEW *)d2_Objs[i].buf == vw && d2_Objs[i].disabled == 0) {
                        list[n++] = i;
                        d2_Objs[i].disabled = 1;
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
        memcpy(gv_Cannon.camTarget, &save3, sizeof(gv_Cannon.camTarget));
    }
    d3_PointToWorld(CR->obj, &CR->obj->pos, &sc.pos);
    memcpy(&sc.rot, gv_Cannon.camTarget, sizeof(sc.rot));
    sc.pos.y = (sc.bmax.y - sc.bmin.y) * 0.8f + sc.pos.y;
    {
        D3VECTOR rotv;

        rotv.y = sc.rot.y;
        rotv.z = sc.rot.z;
        rotv.x = 0.0f;
        sc.obj98 = d3_CreateObject(0, &d3_Origin, &rotv, "KANONE_UNTERBAU_Aa");
    }
    sc.obj8c[0] = d3_CreateObject(sc.obj98, &d3_Origin, &d3_ZeroRot, "KANONE_ROHR_Aa");
    sc.obj8c[1] = d3_CreateObject(sc.obj98, &d3_Origin, &d3_ZeroRot, "KANONE_DOPPEL_ROHR_Aa");
    sc.obj8c[2] = d3_CreateObject(sc.obj98, &d3_Origin, &d3_ZeroRot, "KANONE_DREIFACH_ROHR_Aa");
    sc.selected = gv_Cannon.selected;
    sc.zoom = 0.0f;
    d3_SetObjectPos(sc.obj98, &sc.pos);
    d3_SetObjectRot(sc.obj98, (D3VECTOR *)&sc.rot);
    sc.obj9c = d3_CreateObject(sc.obj98, (D3VECTOR *)&ai_WimpelPos, &d3_ZeroRot, "AO_KANONE_WIMPEL_Aa");
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
    d2_DirtyRectRight = saved.rect[2];
    d2_DirtyRectLeft[0] = saved.rect[0];
    d2_DirtyRectTop = saved.rect[1];
    d2_DirtyRectBottom = saved.rect[3];
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    d3if_SelectScene(saved.scene, 0);
    if (d3_Sky)
        d3sky_Init(d3_Sky, 1);
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&cam3);
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

typedef struct CANNONROT {  /* ai_CannonCamera's rotation / position vector: three floats and a word cleared as an int */
    float x, y, z;
    int w;
} CANNONROT;


/* 0x57f0e0, the cannon camera update (1254 B): sc = the scene struct. The cannon object (sc->obj98) and the
   three barrels (sc->obj8c) get their rotation reset, the wimpel (sc->obj9c) is turned from the aim axis
   (d3_VecAngle2 - 1.5707964 - sc->rot.y) and the ai_CannonAxes directions (recomputed while no shot runs) turn
   the aim axis of the cannon object's matrix (sc->obj98->matrix) into the direction v, normalized; the camera
   starts 30 units back from the scene position (y + 20) and is placed / rotated through d3_Camera, the mouse
   value (sc->speedSlider) * 0.01 as the speed, the pitch's place in the camera's range (ai_CameraPitchMin ..
   ai_CameraPitchMax) and in the cannon's (ai_CannonPitchMin .. ai_CannonPitchMax) the angle and the height, the zoom
   the distance (up to 1450). The recoil (sc->recoilStart + 0x54 > sc->now): the fade from the elapsed clock (0.25 per
   ms during the first 4, then (1 - (elapsed - 4) * 0.0125)^2) times 50 pushes the cannon object back along the
   level aim direction and the wimpel out from ai_WimpelPos; after it the wimpel is set back and the cannon object placed at
   the scene position
   @deep match-deep 2026-11-17 (s8): 1450 is a LITERAL (kept): its only reader is this function (grep_orig
   0x5eecac), it sits behind ai_CannonAim's literals 0x5eec9c..0x5eeca8 (literals in function order, each
   function's in reverse first use) and before svgrave's tables 0x5eecb0; a named const there would have to
   come first in its object. The 2 ins the literal cost: VC6 moves a literal factor of a product to the end
   (a*1450*r compiled as a*r*1450); the original multiplies by 1450 first. Lever: the zoom distance as its
   own statement (fade = (speed*ratio2*0.5+1)*1450; fade = (fade*ratio2 + 50)*zoom): 52 -> 50, = the
   named-const code. Same code: ratio2*((..)*1450), ((..)*1450)*ratio2 (parens: a trick, not kept);
   1450*(..)*ratio2 / ratio2*(..)*1450 / (..)*1450*ratio2 as one fade = control; 1450.0 double 54; a k
   local 57. Left as in prior: frame 0x74 vs 0x78 (v as padded 16-byte slot before pos), the dirZ copy
   placement. prior: match-deep 2026-11-14: draft 142 -> 50 ins. Levers that moved: pos.w stored as an int
   0 (CANNONROT, 99 -> the zero web in ebx from the zero block on, as the original); d as a 16-byte
   D3VECTOR4 (its slot packs with ratio and the qword temps at the original's 0x14); the d copies after the
   v.x statement (xy9: fpu 55 -> 23). No lever: ai_FloatOne / ai_FloatZero objects (= same distance),
   recoil x,z,y order, wimpel add order. Left: (1) frame 0x74 vs 0x78: the original has v at 0x24 as a
   16-byte slot (hole at 0x30) before pos at 0x34; v as D3VECTOR4 gives 0x78 but pos (count 36) then opens
   its slot before v (27), every shift 162+ ins; splitting pos into rot / w locals (function or block
   scope) does not share slots. (2) the dirZ copy: the original copies sc->dirZ to its slot (0x1c) after
   the d2_GetObjValue call and forwards dirX / dirY; ours forwards all three in the 50-ins form, the copy
   only ... */
void ai_CannonCamera(CANNONSCENE *sc)
{
    CANNONROT pos;    /* the rotations, then the camera position */
    D3VECTOR v;       /* the aim direction, then the camera rotation */
    D3VECTOR4 d;      /* the camera's direction (sc->dirX ..) */
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
    pos.y = ((0.5f - 0.25f * speed) * ratio2 * sc->zoom + ratio) * 30.0f + pos.y;
    v.x = (speed * sc->zoom * 0.5f + ((1.0f - sc->zoom) * 0.7f + (float)sqrt(1.0f - sc->zoom) * 0.3f))
          * (ai_CameraPitchZero - ai_CameraPitchMin) * ratio2 + ai_CameraPitchMin;
    d.z = sc->dirZ;
    d.x = sc->dirX;
    d.y = sc->dirY;
    fade = (speed * ratio2 * 0.5f + 1.0f) * 1450.0f;
    fade = (fade * ratio2 + 50.0f) * sc->zoom;
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
   text, "$A" separates the entries. A record with entries, or with none while ai_ShotDirty is still set
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
                        io_PrintText("$A");
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
   sc->selected = sel; at the end sc->now = the clock. The half's time and the sink distance are the const data
   ai_CannonSwitchMs / ai_CannonSwitchDist (the original divides by the time: a literal would be a multiply);
   v and w are padded D3VECTOR4s (the original's frame) */
void ai_CannonSwitchAnim(char *rec, CANNONSCENE *sc, GVEFFECT *run, int sel, int mask)
{
    D3VECTOR4 v;
    D3VECTOR4 w;
    unsigned start;
    float f;
    float k;
    int i;

    start = d3_Time;
    if (sel == sc->selected || sel > sc->maxSel)
        return;
    while (sv_RunFrame((int)ai_CannonSwitchAnim, mask) != 0) {
        ai_CannonPanel(rec, sc, run);
        if ((float)(d3_Time - start) > ai_CannonSwitchMs) {
            sv_PopRequest = 1;
        } else {
            f = (float)(d3_Time - start) / ai_CannonSwitchMs;
            v.x = sc->dirX;
            v.z = sc->dirZ;
            v.y = 0;
            d3_Normalize((char *)&v);
            v.x = (ai_CannonSwitchDist * f) * v.x;
            v.y = (ai_CannonSwitchDist * f) * v.y;
            v.z = (ai_CannonSwitchDist * f) * v.z;
            v.x = sc->pos.x - v.x;
            v.y = sc->pos.y - v.y;
            v.z = sc->pos.z - v.z;
            d3_SetObjectPos(sc->obj98, (char *)&v);
            w.x = d3_ZAxis.x;
            w.y = d3_ZAxis.y;
            w.z = d3_ZAxis.z;
            k = ai_CannonSwitchDist * f;
            w.x = k * w.x;
            w.y = k * w.y;
            w.z = k * w.z;
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
            f = (float)(d3_Time - start) / ai_CannonSwitchMs;
            v.x = sc->dirX;
            v.z = sc->dirZ;
            v.y = 0;
            f = 1.0f - f;
            d3_Normalize((char *)&v);
            v.x = (ai_CannonSwitchDist * f) * v.x;
            v.y = (ai_CannonSwitchDist * f) * v.y;
            v.z = (ai_CannonSwitchDist * f) * v.z;
            v.x = sc->pos.x - v.x;
            v.y = sc->pos.y - v.y;
            v.z = sc->pos.z - v.z;
            d3_SetObjectPos(sc->obj98, (char *)&v);
            w.x = d3_ZAxis.x;
            w.y = d3_ZAxis.y;
            w.z = d3_ZAxis.z;
            k = ai_CannonSwitchDist * f;
            w.x = k * w.x;
            w.y = k * w.y;
            w.z = k * w.z;
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
   @deep 476 -> 460 ins: the six vectors fwd / side / up / p1 / p2 / p3 as D3VECTOR4 (the original's frame
   0x1cc with a 4-byte hole behind each; the frame now equal), copy order fwd x,z,y / up z,x,y (2591 copy
   orders: best 460). Left (code 276, fpu 166): the x87 stack allocation of the t / p chains (the original
   keeps a base component with fld st(0) / fst and int-copies one per case), and the ball index a * 12 the
   original computes after gv_CannonItemFactor (ours before the call into edi). Tried: ball pointer local
   (500 / 488), size first, 8 spellings of the charge expression (one code), any D3VECTOR4 subset (64; all
   six best for the frame) (tiebreak: not measured (dcp headers sample ai_CannonExplosion)) prior: x87
   stack allocation and the a * 12 ball index (the original computes it after gv_CannonItemFactor) are left */
void ai_CannonExplosion(CANNONSCENE *sc, char *rec)
{
    SHOTREC shotA;
    SHOTREC shotB;
    SHOTREC shotC;
    D3VECTOR4 fwd;
    D3VECTOR4 side;
    D3VECTOR4 up;
    D3VECTOR4 p1;
    D3VECTOR4 p2;
    D3VECTOR4 p3;
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
    fwd.z = sc->dir.z;
    fwd.y = sc->dir.y;
    side.x = sc->side.x;
    side.y = sc->side.y;
    side.z = sc->side.z;
    up.z = sc->up.z;
    up.x = sc->up.x;
    up.y = sc->up.y;
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


/* 0x580a90, builds the cannon control panel of the scene sc (run = its effect slot): the two edge pictures (by the
   screen width 800 / 0x400 / other: 0x7cb / 0x7cc / 0x7cd at the top left, 0x7ce / 0x7cf / 0x7d0 at the bottom
   left) into sc->picL / picR; the form "Locations\domizil\cannon_bottom" (sc->win_a4), its window moved 0x19 left
   of the screen centre line, with the fire / quit buttons (texts 0x7d5 / 0x7d4), the kind slider (0 .. 4,
   gv_Cannon.sliderA, labelled txt 0x4061 with the five kinds 0x4063 .. 0x4067) and the speed slider (0 .. 100,
   gv_Cannon.sliderB, txt 0x4062), then the three barrel controls (0x7d3 / 0x7d1 / 0x7d2) into sc->selBtn; the form
   "Locations\domizil\cannon_top" (sc->win_c0) 0x4b left of the line with the three bars (0 .. 100): the default
   effect scale, the pitch bar (also from the effect scale) and the run's charge of the selected barrel (txt 0x405e
   .. 0x4060); the form "Locations\domizil\cannon_res" (sc->win74) placed with 3 and disabled; sc->resultClock =
   the clock. The buttons are interactive with the style 3 */
void ai_CannonBuildPanel(CANNONSCENE *sc, GVEFFECT *run)
{
    int h;
    int mid;
    int pic;
    int base;
    int id;
    D2WIN *w;

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
    sc->picR = d2_CreatePicObj(0, d2_ScreenH - d2_Pics[base].h, base);
    sc->win_a4 = d2_OpenForm(0, 0, "Locations\\domizil\\cannon_bottom");
    d2_PlaceMainWindow(sc->win_a4, 1);
    d2_SetForm(sc->win_a4, 0);
    w = &d2_Wins[d2_LastWindow];
    d2_MoveObj(w->x, h - 0x19, w->parent);
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
    d2_PlaceMainWindow(sc->win_c0, 1);
    d2_SetForm(sc->win_c0, 0);
    w = &d2_Wins[d2_LastWindow];
    d2_MoveObj(w->x, mid - 0x4b, w->parent);
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
    d2_PlaceMainWindow(sc->win74, 3);
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

int gm_GetWorth(unsigned short id); int d2_WinOpenSlider(); int sv_CheckGoodAmount(char *o, int n, unsigned char t); short * gm_PlayerRecord(char *c);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out); int snd_PlayVoice(int id, int bank, char *text, int var); void sv_CameraFollow();

/* 0x581020, the cabinet's spy window for the building rec: the form "Locations\Domizil\kabinett_spy" (centred,
   fitted, the default pointer) with the title txt 0x4020 centred over the window's width in font 0x54, a slider of
   three steps (txt 0x4021 .. 0x4023) choosing the payment: 1%, 1.5% or 2% of the player's worth (gm_GetWorth), and
   the two buttons from the text 0x4024 on (pay / leave). Each frame prints the texts 0x401e / 0x401c and 0x401f
   with the chosen amount; the right mouse button or the second button leaves (sv_PopRequest). Pay: when the player
   has the money (sv_CheckGoodAmount) and the building may be spied on (ai_CanSpyOn), the amount is added to the
   player record's spyPaid and moved away from the player (cm_SendMoveGoodsUnchecked), the message 0x99 goes to the
   building's owner (flags 2, the game time, the building id, the step and ai_PickActionBuilding's pick of the
   player's character), its slot is waited for, the voice "KOMMENTARE" plays and the window leaves. At the end the
   view is closed and the camera follows again */
void ai_CabinetSpyWindow(AIBLD *rec)
{
    int v[3];
    int worth;
    int win;
    int slider;
    int i;
    int id;
    int btnPay;
    int btnLeave;

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
        int amount;

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
                if (sv_CheckGoodAmount(ai_Chars[ai_PlayerId], amount, ai_OwnPlayer) != 0 && ai_CanSpyOn(rec) != 0) {
                    AIMSG msg;
                    int pick;
                    LARGE_INTEGER mask;
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
   and the cost). Pay: when the player has both (sv_CheckGoodAmount, sv_CheckMoney) and the building may be spied on
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
                if (sv_CheckGoodAmount(ai_Chars[ai_PlayerId], cost[i], ai_OwnPlayer) != 0 &&
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
   (sv_CheckGoodAmount, sv_CheckMoney 0x10) and the building may be spied on (ai_CanSpyOn), the price is added to
   the player record's spyPaid and moved away (cm_SendMoveGoodsUnchecked), 0x10 taken off (gm_SendMoneyDelta), the
   message 0x99 goes to the building's owner (flags 2, the game time, the target's netId, 4, the target's own
   building / building, else ai_PickActionBuilding's pick), its slot is waited for, the voice "KOMMENTARE" plays
   (variation -1) and the window leaves; the right button or the second button leaves too */
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
                if (sv_CheckGoodAmount(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) != 0 &&
                    sv_CheckMoney(ai_Chars[ai_PlayerId], 0x10) == 1 && ai_CanSpyOn(rec) != 0) {
                    AIFIN *fin = (AIFIN *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);

                    if (fin != 0)
                        fin->spyPaid += price;
                    cm_SendMoveGoodsUnchecked(-1, ((AICHAR *)ai_Chars[ai_PlayerId])->netId, ai_OwnPlayer, price);
                    gm_SendMoneyDelta(((AICHAR *)ai_Chars[ai_PlayerId])->netId, -0x10);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x99;
                    msg.flags = 2;
                    msg.to = ((AICHAR *)ai_Chars[rec->owner])->netId;
                    msg.fromId = rec->id;
                    msg.time = ai_Time;
                    msg.arg[1] = rec->id;
                    msg.arg[2] = p->netId;
                    msg.arg[3] = -1;
                    msg.arg[4] = 4;
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

/* the unit's .bss (after sv_t.c's 0x6f263c .. 0x6f2660) */
int ai_ShotDirty = 0;  /* 0x6f2664, set when ai_ShotTableStore stored a record, cleared by the scene (ai_CannonShotResults) */
int svc_UnreadBss = 0;  /* 0x6f2668, no surviving instruction or pointer refers to it: data of functions /OPT:REF removed; size from the neighbours' addresses */
