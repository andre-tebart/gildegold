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
   The unit's .rdata: ai_Tutorial 0x5eec60 and ai_TutorialConst5, then its literal 400.0f 0x5eec68; the cannon
   object svcannon (0x57e230) and the graveyard object svgrave (0x582730) follow; the cannon scene, the cabinet and
   the graveyard parts of the description above are theirs.
   Open: GVGRAVE.v2c / v30 / nameId: gv.c stores small numbers there, ai_GraveDeadName reads v2c as the office
   (1 .. 9, gv.c's prevOffice / random) and ai_GraveDeadDescription v30 / nameId as text ids; which texts the
   bases 0x114 / 0x130 / 0x11e / 0x18f (and the office ones 0x253 / 0x27b) name is unknown.
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
TUTSTATE *const ai_Tutorial = &ai_TutorialState;  /* 0x5eec60, the pointer to the tutorial state record */
const int ai_TutorialConst5 = 5;  /* 0x5eec64, the tutorial object's second named constant (after ai_Tutorial, before its literal 400.0f); no reader in the surviving code (name from the value: its meaning is open) */
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
int ai_TutorialChapterOn[5] = { 1, 1, 1, 1, 1 };  /* 0x62918c, per chapter 1 = played (the start window's check boxes, ai_TutorialWindow) */
int (*ai_TutorialChapterInit[5])(void) = { ai_TutorialInitChapter1, ai_TutorialInitChapter2, ai_TutorialInitChapter3, ai_TutorialInitChapter4, ai_TutorialInitChapter5 };  /* 0x6291a0, the chapter chain builders the start appends for every chapter played */
char *ai_TutorialStateNames[18] = {  /* 0x6291b4, the names of the tutorial's 18 states (the FT* strings 0x6291fc.. in reverse order); no reader in the image (its users were removed); the name is ours */
    "FTDisabled", "FTStart", "FTViewing", "FTReviewing", "FTIdle", "FTBeginScript", "FTIdleScript", "FTEndScript",
    "FTRemind", "FTPartialSuccsess", "FTBeginSuccsess", "FTSuccsess", "FTEndSuccsess", "FTNoBezierAnim",
    "FTBezierAnimOff", "FTBezierStartDelay", "FTBezierAnimating", "FTBezierEndDelay"
};
int ai_TutorialBanks[6] = { 0 };  /* 0x6f2640, the voice banks of the main intro / outro and the chapters 1 .. 5 (snd_LoadVoiceBank), unloaded by the stop */
TUTRUN *ai_TutorialRuns = 0;  /* 0x6f2658, the chain of the tutorial's run records (built by its start 0x578180, freed by its stop 0x578370) */
int ai_TutorialOn = 0;  /* 0x6f265c, the tutorial runs (set by its start 0x578180, cleared by its end) */
int ai_TutorialRunning = 0;  /* 0x6f2660, the tutorial's state machine advances (unk_SetEndGameGate / unk_ClearEndGameGate) */
SHOTRESULT ai_ShotTable[3];  /* 0x6f9f60, the cannon scene's shot results (the callback ai_ShotTableStore fills it, ai_ShotTableReset resets it) */

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
#include "d3_pose.h"
#include "dd_vesa.h"
#include "menu.h"
#include "d2.h"
#include "sv.h"
#include "gfx.h"

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
    party.kind = 1;
    party.flags = 0xa0;
    party.thing = -1;
    party.time = ai_Time;
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
    d2_PlaceMainWindow(ai_Tutorial->host->slot->win, 3);
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
   the voice still playing is stopped and 1 answers (the answer code is written in each arm, as the
   original: VC6 cross-jumps the two copies) */

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
           automation gets past it. Answer straight away like the keyboard arm does - forging a click in
           d2_ClickedId / d2_PointerObj would leak the phantom click into the rest of the frame. Without the flag
           the message waits for the real click / Return. */
        {
            char *rea = getenv("REA_START");
            if (rea != 0 && strcmp(rea, "tutorial") == 0) {
                if (ai_Tutorial->voice) {
                    if (sb_IsPlaying(ai_Tutorial->voice))
                        sb_Stop(ai_Tutorial->voice, 0);
                    ai_Tutorial->voice = 0;
                }
                return 1;
            }
        }
#endif
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == ai_Tutorial->continueObj) {
                if (ai_Tutorial->voice) {
                    if (sb_IsPlaying(ai_Tutorial->voice))
                        sb_Stop(ai_Tutorial->voice, 0);
                    ai_Tutorial->voice = 0;
                }
                return 1;
            }
        } else if (kb_Last == 0x1c) {
            if (ai_Tutorial->voice) {
                if (sb_IsPlaying(ai_Tutorial->voice))
                    sb_Stop(ai_Tutorial->voice, 0);
                ai_Tutorial->voice = 0;
            }
            return 1;
        }
    }
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
    d2_PlaceMainWindow(ai_Tutorial->progressForm, 3);
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
   part of the run's tailTime still left since stepAt, scaled to 0 .. 400 */

void ai_TutorialProgressSlider(void)
{
    unsigned int t;
    int left;

    if (!ai_Tutorial->run || ai_Tutorial->state != 0xa) {
        ai_TutorialCloseProgress();
        return;
    }
    if (ai_Tutorial->progressForm == -1)
        ai_TutorialOpenProgress();
    t = MINU(TUT_SINCE(ai_Tutorial->stepAt), ai_Tutorial->run->tailTime);
    left = ai_Tutorial->run->tailTime - t;
    d2_SetObjValue(ai_Tutorial->progressSlider, 0, 400, (int)((float)left / ai_Tutorial->run->tailTime * 400.0f), 1);
}
/* 0x577e50, shows the form of the run's current step: -4 without a run record; nothing (0) for a run without
   steps. Past the last step the run's stepShown callback runs and an open step form is closed (stepForm,
   stepControl -1, stepKind 4: none); when none was open, the original goes on to show the step past the end
   (it reads the step record behind the chain). Otherwise the step form of the step's kind (0 left, 1 right, 2 top, 3 bottom; -4 for another
   kind) is opened unless it is already open, when only its controls 1 / 2 are cleared (an open form of another
   kind is closed first, with stepControl -1 and stepKind 4; with no form open those two keep their values);
   control 1 gets the step's text, control 2 the continue text 0x7d whose object becomes stepControl; the
   step's voice replaces the playing one, and the run's stepShown callback runs */

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
            ai_Tutorial->stepControl = -1;
            ai_Tutorial->stepKind = 4;
        }
        if (ai_Tutorial->run->steps[ai_Tutorial->step].kind >= 4)
            return -4;
        switch (ai_Tutorial->run->steps[ai_Tutorial->step].kind) {
        case 0: ai_Tutorial->stepForm = d2_OpenForm(0, 0, "tutorial\\left_form"); break;
        case 1: ai_Tutorial->stepForm = d2_OpenForm(0, 0, "tutorial\\right_form"); break;
        case 2: ai_Tutorial->stepForm = d2_OpenForm(0, 0, "tutorial\\top_form"); break;
        case 3: ai_Tutorial->stepForm = d2_OpenForm(0, 0, "tutorial\\bottom_form"); break;
        }
        d2_PlaceMainWindow(ai_Tutorial->stepForm, 3);
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
    d2_PlaceMainWindow(ai_Tutorial->text1Win, 3);
    ai_Tutorial->text1Shown = 0;
    ai_Tutorial->statusShown = 0;
    ai_Tutorial->text2Shown = 0;
    ai_Tutorial->text2Win = d2_OpenForm(0, 0, "tutorial\\aux_form");
    d2_SetViewEnabled(ai_Tutorial->text2Win, 0);
    d2_PlaceMainWindow(ai_Tutorial->text2Win, 3);
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
    float u3;
    float t3;

    u = 1.0f - t;
    u3 = u * u * u;
    t3 = t * t * t;
    out[0] = u3 * a[0] + (u * b[0] + t * c[0]) * u * t * 3.0f + t3 * d[0];
    out[1] = u3 * a[1] + (u * b[1] + t * c[1]) * u * t * 3.0f + t3 * d[1];
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
            return;
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
    init_PlayerProfile.job = 0x33;
    init_PlayerProfile.status = 0x10;
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
   ai_TutorialChapterOn. 1 when start was clicked with a chapter checked (the tutorial starts), else 0 */
int ai_TutorialWindow(void)
{
    int start;
    int result;
    int back;
    int ids[5];
    int i;
    /* @original n's initializer emits no code; with it VC6 keeps the original's two counters in the
       chapter scan (edx for i, esi for n, cmp edx,5) instead of merging them into one */
    int n = 0;
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
        for (i = 0, n = 0; i < 5; i++) {
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
   @deep match-deep 2026-11-14: 260 -> 221 ins with the row position computed from i at the loop top (y =
   i * 0x64 + 0x31; the running y += 0x64 and y in the for increment = control, every use written as i *
   0x64 + k = 263). Statement orders of the five entry stores (120) and an else around the rest: no lever.
   Frame lead: the original stores form / win2 / win3 / group to slots spaced like elements of one int
   array (form 0x34, win2 0x3c, win3 0x44, group 0x50 by the push depths, win2 written with mov
   [esp+0x78],edi and reloaded from it in the loop); int w[8] with form w[0], win2 w[2], win3 w[4], group
   w[7] (or w[3] / w[6]) gives the original's frame 0x55c and 215 ins, but win2 stays in a register in ours
   and the slot order of n / rec / steps differs. Left beside that: the loop keeps i in memory and its *4
   offset in ebp ([esp+ebp+0x64]), ours i in a register (scaled index), and &d2_Wins[win2].w hoisted into
   ebp in ours. (tiebreak: not measured (dcp headers sample ai_AlchemistCandidates)) prior: read whole
   against the original and reconstructed (483 vs 489 instructions; asmdiff --symbols: every call,
   argument, constant and text id in the original's order, no ! line but the reordered x / y name loads).
   Left: allocation. The original keeps form in esi, the radio group in ebp then ebx = group*0x23, ok in
   edi; ours form in ebp and -1 in esi (cmp [d2_Radios+8], esi vs cmp ..., -1). In the candidate loop the
   original keeps i and a byte offset i*4 in slots (mov ebp,[esp+0x18] at the loop top, [esp+ebp+0x64] /
   [esp+ebp+0x50], add edx,4 at the end) and computes &d2_Wins[win2].w after the name text, ours keeps i in
   ebx (scaled [esp+ebx*4+..]) and hoists &d2_Wins[win2].w out of the loop; the original saves edi only
   after the no-candidate return (push edi at 0x57bfaf); frame 0x55c vs 0x548 */
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
    for (i = 0; i < n; i++) {
        y = i * 0x64 + 0x31;
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
int sv_CheckGoodAmount(char *o, int n, unsigned char t);
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
    if (sv_CheckGoodAmount(ai_Chars[ai_PlayerId], value, ai_OwnPlayer)) {
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
   @deep frame sweep s21: frame 0x264 vs 0x25c. dcp frame pairs msg with line in one of the original's
   slots, but one buffer for both gives frame 0x164 (235 ins): the two are separate 0x100 arrays; line per
   slot loop / msg in the state block = control (199). The 8 bytes are among the scalars (the original
   packs pid / t / slot / group / pic / slider into three slots). Lead: c2frame --names on the scalars.
   prior: match-deep 2026-11-14 (199 ins kept). Tried: the slot loops as index loops (rec->slots[k],
   pics[n]): 343..359 ins (the original's [edx] store through a reloaded pic pointer is a hand pointer,
   *pic = pid; pic++); *pic++ = pid: 237..247; slot index + pic pointer: 197 (frame hunk unchanged). row[2]
   declared in a block around sv_ButtonRow: frame 0x25c = the original's (sel then shares row's slot as
   there) and the entry registers come out as the original's (owner edi, rec ebp, form esi), but 223 ins:
   start / other get registers (the original copies row[0] / row[1] to the slots 0x44 / 0x48) and win2
   (c2trace prio 1187, 11 refs) keeps ebx where the original stores it to 0x40 right after the
   SetForm(form, 2) and reloads it in the loops. Next: from the row-block form, make win2 / start / other
   lose priority (fewer references or a split) - c2trace.py with the arw2 variant sep_ptr_row1 in the
   session notes' generator. (tiebreak: insensitive under the four probes only, best draft) */
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
            d2_SetObjValue(slider, 0, 100, (int)(f * 100.0f));
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
   @deep s25: the original's in-loop copy of the character panel has the same [index+base] order as ours
   ([ecx+edx+0x5c] at the name text), only its FIRST panel has [base+index] at the name text's centred
   store and both 0x4040 stores: the order is not in those statements' text. climb.py round 1 (528 mutants,
   404 compiled, 110 codes): nothing. flip.py --want '[edx+ecx+92], 1|[ecx+eax+92], 1' (958 mutants): one
   flip, swapping d2_Objs[t].centred = 1 with d2_SetObjFont(t, 0x54) in the research-state block (line
   ~4659, 1014 ins): a later block's statement order decides the first panel's SIB order, so it is a
   function-wide rank / numbering effect (the operand order of d2_Objs + t*0x5c), not the panel's spelling.
   Next: read what that swap changes (the d2_Objs / t temps' numbering or counts) and look for the ordinary
   form with that effect in the state block or the loop. prior: draft 88 -> shape 6 ins (frame 0x240 = the
   original's). Kept: the nine -1 ids as ONE record (form, win2, slider, back, extra, bars[4]; the
   original's rep stosd covers exactly those slots, base +0x18..+0x38, and spills win2 / slider / extra
   into them as they are set: 84 -> 12), cleared with memset (the loop over the record = one code);
   d2_SetForm(form, 2) written in both arms of the owner test (the first io_PrintText cleanup before it: 88
   -> 84); the research slider as its own block local prog (the original's extra slot, frame 0x23c ->
   0x240: 12 -> 8); d2_ClickedId = -1 before the clear (or eax,-1 before mov ecx,9: 8 -> 6). Left: sib 8 +
   regs 2 in the FIRST character panel only - the original's d2_Objs[t].centred store of the name text and
   both stores of the 0x4040 text are [base+index] ([edx+ecx+0x5c], [ecx+eax+0x5c], [edx+eax+0x14]) where
   ours has [index+base]; the in-loop copy of the same text is identical in both. No lever: t renamed to k
   / j per text block (127 subsets: = control but one worse), (d2_Objs + t)->, (t + d2_Objs)->, t[d2_Objs]
   (= control), centred before w in the name block (200), index click loop (117), flip.py --want
   '[ecx+eax+92], 1' (one hit, 76 ins); headers sample insensitive. Open: M10 100.0f vs ai_FloatHundred
   (tiebreak: not measured (dcp headers sample ai_AlchemistWindow)) */
void ai_AlchemistWindow(unsigned char *obj)
{
    char msg[0x100];
    char line[0x100];
    struct {                    /* the window's nine object ids, all -1 at the start (one record in the original's frame) */
        int form;
        int win2;               /* section 2's window */
        int slider;             /* the panel's value slider */
        int back;               /* the button with the text 0x4049 */
        int extra;              /* the fund button */
        int bars[4];            /* the four share bars */
    } w;
    AHMREC *rec;
    AICHAR *owner;
    int state;
    int lastState;
    unsigned lastClock;
    int a;
    float f;
    int t;
    int j;

    int *pb;

    owner = (AICHAR *)ahm_OwnerChar(obj);
    rec = ((AHMOBJ *)obj)->rec;
    lastState = ahm_ResearchState(obj, &a, &f, 0);
    lastClock = d3_Time;
    d2_ClickedId = -1;
    memset(&w, 0xff, sizeof(w));
    w.form = d2_OpenForm(0, 0, "Locations\\Domizil\\Alchemistry3");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(w.form);
    d2_SetForm(w.form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(w.form, 2);
    w.win2 = d2_LastWindow;
    d2_SetForm(w.form, 1);
    io_PrintText(0x4039);
    if (owner == 0) {
        d2_SetForm(w.form, 2);
        io_PrintText(0x4051, 0x182);
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_AlchemistResearchWindow, 0x677c7));
        d2_CloseView(w.form);
        return;
    }
    d2_SetForm(w.form, 2);
    d2_WinOpenPic(d2_LastWindowPtr->w / 2 - 0x18, 0x1e, 0x6c7, w.win2);
    d2_WinOpenPic(d2_LastWindowPtr->w / 2 - 0x16, 0x20, owner->picture, w.win2);
    w.slider = d2_WinOpenSlider((d2_LastWindowPtr->w - 0x32) / 2, 0x55, 0x32, 0, 100, 0x75, 0x42, d2_LastWindow);
    d2_SetObjValue(w.slider, 0, 100, (int)(rec->value * 100.0f));
    txt_Format(line, (int)"%s %s", owner->first, owner->family);
    t = d2_CreateWinText(0, 0, line, w.win2);
    d2_Objs[t].w = d2_Wins[w.win2].w;
    d2_Objs[t].centred = 1;
    d2_SetObjFont(t, 0x53);
    txt_Format(line, 0x403f, owner->w00a);
    t = d2_CreateWinText(0x12c, 0x64, line, w.win2);
    d2_SetObjFont(t, 0x53);
    d2_Objs[t].centred = 1;
    d2_Objs[t].w = 0xc8;
    txt_Format(line, 0x4040, ahm_ObjectValue(obj));
    t = d2_CreateWinText(0x12c, 0x82, line, w.win2);
    d2_SetObjFont(t, 0x53);
    d2_Objs[t].centred = 1;
    d2_Objs[t].w = 0xc8;
    txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
    w.extra = d2_WinCreateSlider(0x14a, 0xbe, 0xc5, txt_Ptrs[0x42f9], w.win2);
    d2_SetObjWidth(w.extra, d2_Wins[w.win2].w / 3 - 0x14);
    d2_SetObjFont(w.extra, 0x54);
    txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
    w.bars[0] = d2_WinCreateSlider(10, 0x64, 0xc5, line, w.win2);
    d2_SetObjWidth(w.bars[0], d2_Wins[w.win2].w / 3 - 0x14);
    d2_SetObjFont(w.bars[0], 0x54);
    sv_ValueBar(d2_Wins[w.win2].w / 3 + 10, 0x68, rec->share[0], 0x4df);
    txt_Format(line, (int)"%s", txt_Ptrs[0x4173]);
    w.bars[1] = d2_WinCreateSlider(10, 0x82, 0xc5, line, w.win2);
    d2_SetObjWidth(w.bars[1], d2_Wins[w.win2].w / 3 - 0x14);
    d2_SetObjFont(w.bars[1], 0x54);
    sv_ValueBar(d2_Wins[w.win2].w / 3 + 10, 0x86, rec->share[1], 0x4df);
    txt_Format(line, (int)"%s", txt_Ptrs[0x4174]);
    w.bars[2] = d2_WinCreateSlider(10, 0xa0, 0xc5, line, w.win2);
    d2_SetObjWidth(w.bars[2], d2_Wins[w.win2].w / 3 - 0x14);
    d2_SetObjFont(w.bars[2], 0x54);
    sv_ValueBar(d2_Wins[w.win2].w / 3 + 10, 0xa4, rec->share[2], 0x4df);
    txt_Format(line, (int)"%s", txt_Ptrs[0x4175]);
    w.bars[3] = d2_WinCreateSlider(10, 0xbe, 0xc5, line, w.win2);
    d2_SetObjWidth(w.bars[3], d2_Wins[w.win2].w / 3 - 0x14);
    d2_SetObjFont(w.bars[3], 0x54);
    sv_ValueBar(d2_Wins[w.win2].w / 3 + 10, 0xc2, rec->share[3], 0x4df);
    d2_SetForm(w.form, 3);
    w.back = d2_GetWinObj(w.form, 3, io_PrintText(0x4049));
    do {
        state = ahm_ResearchState(obj, &a, &f, 0);
        d2_SetForm(w.form, 4);
        io_PrintText("$C");
        if (state != 0) {
            int prog;

            prog = d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2, 0x12, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
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
            d2_SetObjValue(prog, 0, 100, (int)(f * 100.0f));
        }
        if (state != lastState || d3_Time > lastClock + 0x32) {
            owner = (AICHAR *)ahm_OwnerChar(obj);
            if (owner == 0) {
                sv_PopRequest = 1;
                sv_RunFrame((int)ai_AlchemistWindow, 0x20007);
                d2_CloseView(w.form);
                return;
            }
            d2_SetForm(w.form, 2);
            io_PrintText("$C");
            d2_WinOpenPic(d2_LastWindowPtr->w / 2 - 0x18, 0x1e, 0x6c7, w.win2);
            d2_WinOpenPic(d2_LastWindowPtr->w / 2 - 0x16, 0x20, owner->picture, w.win2);
            w.slider = d2_WinOpenSlider((d2_LastWindowPtr->w - 0x32) / 2, 0x55, 0x32, 0, 100, 0x75, 0x42, d2_LastWindow);
            d2_SetObjValue(w.slider, 0, 100, (int)(rec->value * 100.0f));
            txt_Format(line, (int)"%s %s", owner->first, owner->family);
            t = d2_CreateWinText(0, 0, line, w.win2);
            d2_Objs[t].w = d2_Wins[w.win2].w;
            d2_Objs[t].centred = 1;
            d2_SetObjFont(t, 0x53);
            txt_Format(line, 0x403f, owner->w00a);
            t = d2_CreateWinText(0x12c, 0x64, line, w.win2);
            d2_SetObjFont(t, 0x53);
            d2_Objs[t].centred = 1;
            d2_Objs[t].w = 0xc8;
            txt_Format(line, 0x4040, ahm_ObjectValue(obj));
            t = d2_CreateWinText(0x12c, 0x82, line, w.win2);
            d2_SetObjFont(t, 0x53);
            d2_Objs[t].centred = 1;
            d2_Objs[t].w = 0xc8;
            txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
            w.extra = d2_WinCreateSlider(0x14a, 0xbe, 0xc5, txt_Ptrs[0x42f9], w.win2);
            d2_SetObjWidth(w.extra, d2_Wins[w.win2].w / 3 - 0x14);
            d2_SetObjFont(w.extra, 0x54);
            txt_Format(line, (int)"%s", txt_Ptrs[0x4172]);
            w.bars[0] = d2_WinCreateSlider(10, 0x64, 0xc5, line, w.win2);
            d2_SetObjWidth(w.bars[0], d2_Wins[w.win2].w / 3 - 0x14);
            d2_SetObjFont(w.bars[0], 0x54);
            sv_ValueBar(d2_Wins[w.win2].w / 3 + 10, 0x68, rec->share[0], 0x4df);
            txt_Format(line, (int)"%s", txt_Ptrs[0x4173]);
            w.bars[1] = d2_WinCreateSlider(10, 0x82, 0xc5, line, w.win2);
            d2_SetObjWidth(w.bars[1], d2_Wins[w.win2].w / 3 - 0x14);
            d2_SetObjFont(w.bars[1], 0x54);
            sv_ValueBar(d2_Wins[w.win2].w / 3 + 10, 0x86, rec->share[1], 0x4df);
            txt_Format(line, (int)"%s", txt_Ptrs[0x4174]);
            w.bars[2] = d2_WinCreateSlider(10, 0xa0, 0xc5, line, w.win2);
            d2_SetObjWidth(w.bars[2], d2_Wins[w.win2].w / 3 - 0x14);
            d2_SetObjFont(w.bars[2], 0x54);
            sv_ValueBar(d2_Wins[w.win2].w / 3 + 10, 0xa4, rec->share[2], 0x4df);
            txt_Format(line, (int)"%s", txt_Ptrs[0x4175]);
            w.bars[3] = d2_WinCreateSlider(10, 0xbe, 0xc5, line, w.win2);
            d2_SetObjWidth(w.bars[3], d2_Wins[w.win2].w / 3 - 0x14);
            d2_SetObjFont(w.bars[3], 0x54);
            sv_ValueBar(d2_Wins[w.win2].w / 3 + 10, 0xc2, rec->share[3], 0x4df);
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            for (j = 0, pb = w.bars; j < 4; j++, pb++) {
                if (*pb == d2_PointerObj) {
                    d2_SetViewEnabled(w.form, 0);
                    ai_AlchemistTrainShareWindow(obj, rec, j);
                    d2_SetViewEnabled(w.form, 1);
                    d2_PointerObj = -1;
                }
            }
            if (d2_PointerObj == w.extra) {
                d2_SetViewEnabled(w.form, 0);
                ai_AlchemistFundWindow(obj, rec);
                d2_SetViewEnabled(w.form, 1);
            } else if (d2_PointerObj == w.back) {
                d2_SetViewEnabled(w.form, 0);
                if (ai_AlchemistRescheduleWindow(obj, rec))
                    sv_PopRequest = 1;
                d2_SetViewEnabled(w.form, 1);
            }
        }
    } while (sv_RunFrame((int)ai_AlchemistWindow, 0x677c7));
    d2_CloseView(w.form);
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
    d2_SetObjValue(field, gm_ConvertMoney(tenth, ai_OwnPlayer), gm_ConvertMoney(points, ai_OwnPlayer),
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
                if (sv_CheckGoodAmount(ai_Chars[ai_PlayerId], v, ai_OwnPlayer)) {
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

/* dcp unit split: the declarations that moved to svcannon (copies: drop what sv_t does not use) */
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
/* dcp unit split: the functions of svcannon the rest of sv_t uses */
void ai_CannonAim(CANNONSCENE *sc, float dt);
void ai_CannonAxes(CANNONSCENE *sc);
int ai_CannonFireCheck(CANNONSCENE *sc);
void ai_CannonShotResults(CANNONSCENE *sc);
float ai_CannonPitchRatio(CANNONSCENE *sc);
void ai_CannonSwitchAnim(char *rec, CANNONSCENE *sc, GVEFFECT *run, int sel, int mask);
void ai_CannonExplosion(CANNONSCENE *sc, char *rec);
void ai_ShotTableReset(void);
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
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out); int snd_PlayVoice(int id, int bank, char *text, int var); void sv_CameraFollow();
int ai_AmountMenu(); int amt_GetKindLevel(int k); int sv_CheckMoney(); void cm_SendMoneyDelta(int a, int b);
void txt_Format(char *out, int id0, ...); int sv_MessageBox();
void sv_ReleaseNamePlates(void); int sv_RegisterNamePlate(); void ai_StorageWindow();
void ai_AlchemistResearchWindow(); void ai_AlchemistWindow();
void cm_BeginFieldBlock(char *o, int id); int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldDeltas(void);
char ai_ShotCallbackArg[0x40];  /* 0x6fa020, the argument gv_SetEffectCallback stores with the shot callback ai_ShotTableStore (ai_CannonScene); never read by the code, its type is unknown; it runs to the tutorial state record at 0x6fa060 */
TUTSTATE ai_TutorialState;  /* 0x6fa060, the tutorial state record ai_Tutorial points to (the tutorial code reads it through S); its last read field is progressSlider at +0x6c, the 0x10 bytes to he_OptionsPath at 0x6fa0e0 stay unowned */

/* dcp unit split: the functions of svgrave the rest of sv_t uses */
void ai_GraveDeadName(GVGRAVE *e, char *out);
/* dcp unit split: the declarations that moved to svgrave (copies: drop what sv_t does not use) */
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
