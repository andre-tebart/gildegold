/* unk_575860.h -- the unit after he.c (unnamed): the tutorial module and its state machine,
   see the lead comment of src/unk_575860.c */
#ifndef UNK_575860_H
#define UNK_575860_H

struct HEEVSLOT;
struct SBSAMPLE;

typedef struct TUTSTEP {                /* 0x10 bytes, a step of a run record's chain (TUTRUN.steps) */
    unsigned int delay;                 /* 0x00, ms after the step started until its control is enabled */
    int kind;                           /* 0x04, the chapter kind 0 .. 3 (ai_TutorialStepForm) */
    int text;                           /* 0x08 */
    int unk0c;                          /* 0x0c */
} TUTSTEP;

typedef struct TUTRUN {                 /* 0x74 bytes, a run (chapter) record of the tutorial, chained by next */
    char pad00[8];
    int mode;                           /* 0x08, 1 / 2 start with the timed first text window (state 4), else state 0xb */
    char pad0c[8];
    int text1;                          /* 0x14, the first text window's text (ai_TutorialShowText) */
    char pad18[4];
    int text2;                          /* 0x1c, the second text window's text (ai_TutorialShowText2) */
    char pad20[4];
    int statusText;                     /* 0x24, the d2_Strings line shown in the status bar by the states 0xa / 0xb */
    char *voice;                        /* 0x28, the voice played with it (snd_PlayVoice) */
    unsigned int text1Time;             /* 0x2c, ms the first text window waits before it shows */
    unsigned int tail2Time;             /* 0x30, the length of the state 0xb */
    void (*hook)(int end);              /* 0x34, called with 0 when the run starts, 1 when it ends */
    int (*stepHook)(int *args);         /* 0x38, ai_CampaignStep hands its four arguments to it; set: the start takes a campaign step */
    int (*query)(int *args);            /* 0x3c, ai_ScriptQuery's callback */
    int goal;                           /* 0x40, the event index a that completes the run's goal (ai_CampaignStep); 3: timed */
    unsigned int tailTime;              /* 0x44, the length of the state 0xa and of a timed goal */
    int goalState;                      /* 0x48, the state a completed goal switches to */
    int text2Goal;                      /* 0x4c, the event index that ends the second text window; set: the chain ends in the state 9, else 0xa */
    unsigned int stepCount;             /* 0x50 */
    char pad54[4];
    TUTSTEP *steps;                     /* 0x58 */
    char pad5c[8];
    int marker;                         /* 0x64, set: the position marker runs (ai_TutorialMarkerUpdate) */
    int allowMask;                      /* 0x68, ai_RunAllows */
    int allowMask2;                     /* 0x6c */
    struct TUTRUN *next;                /* 0x70, the next run record */
} TUTRUN;

typedef struct TUTHOST {                /* the record at TUTSTATE.host; only its event slot is known */
    char pad00[0x74];
    struct HEEVSLOT *slot;              /* 0x74, the event bar slot of the tutorial's window */
} TUTHOST;

typedef struct TUTSTATE {               /* the tutorial state record (0x6fa060, ai_Tutorial points to it) */
    int state;                          /* 0x00, 1 .. 0xc (ai_TutorialTick) */
    int next;                           /* 0x04, the state to return to */
    unsigned int textAt;                /* 0x08, ai_ClockMs when the first text window last changed */
    unsigned int stepAt;                /* 0x0c, ai_ClockMs when the step / tail started */
    int campaignSteps;                  /* 0x10, counts ai_CampaignStep's handled steps, 0 when a run starts */
    struct SBSAMPLE *voice;             /* 0x14, the playing voice */
    TUTRUN *run;                        /* 0x18, the current run record */
    TUTHOST *host;                      /* 0x1c */
    int unk20;                          /* 0x20 */
    int text1Win;                       /* 0x24, the first text window */
    int text1Shown;                     /* 0x28 */
    int statusShown;                    /* 0x2c, the status text of the states 0xa / 0xb is shown */
    int text2Shown;                     /* 0x30 */
    int text2Win;                       /* 0x34, the second text window */
    int goalDone;                       /* 0x38, set by the run's step callbacks when their goal is met, 0 when a run starts */
    int phase;                          /* 0x3c, 0 .. 3 */
    unsigned int step;                  /* 0x40, the step of the run's chain */
    int unk44;                          /* 0x44 */
    int stepForm;                       /* 0x48, ai_TutorialStepForm's form */
    int stepControl;                    /* 0x4c, its control, -1 = none */
    int stepArg;                        /* 0x50, ai_CampaignStep's argument b of the step that ended the chain */
    int markerStep;                     /* 0x54 */
    int markerPhase;                    /* 0x58 */
    int markerAt;                       /* 0x5c */
    int markerObj;                      /* 0x60, -1 = none */
} TUTSTATE;

void ai_GypsyHelperWindow(char *b);     /* 0x584d70, the helpers window of a building (HANDEL_SUCHEN form), opened by the gypsy production contact */
extern int ai_TutorialOn;               /* 0x6f265c, the tutorial runs */
extern int ai_TutorialRunning;          /* 0x6f2660, its state machine advances */
int ai_TutorialTick(void);                 /* 0x575860, the tutorial's tick: runs its state machine */
void ai_TutorialMarkerUpdate(int arg);  /* 0x57b8a0, moves the tutorial's position marker */
void unk_ClearEndGameGate(void);                /* 0x576020, clears the gate 0x6f2660 */
void unk_SetEndGameGate(void);                /* 0x576030, sets the gate 0x6f2660 */
void ai_CampaignStep(int a, int b, int c, int d);  /* 0x576040, one step of the campaign state machine */
int ai_ScriptQuery(int a, int b, int c, int d);   /* 0x576340, calls the run record's callback +0x3c */
int ai_RunAllows(int mask);             /* 0x5763c0, run mask +0x68 */
int ai_Unk576410(int mask);             /* 0x576410, run mask +0x6c */
int ai_Unk576460(void);                 /* 0x576460, mode 2 / state 5 query */
int ai_Unk576490(int *p);               /* 0x576490, step callback predicate */
int ai_QueryDefault(int *p);               /* 0x5764b0, step callback predicate */
int ai_QueryBuilding0f(int *p);               /* 0x576500, step callback predicate */
int ai_QueryChapter2Step3(int *p);               /* 0x576590, step callback predicate */
int ai_Unk5765e0(int *p);               /* 0x5765e0, step callback predicate */
int ai_QueryChapter2Step5(int *p);               /* 0x576600, step callback predicate */
int ai_QueryChapter2Step6(int *p);               /* 0x576640, step callback predicate */
int ai_QueryChapter2Step7(int *p);               /* 0x576680, step callback predicate */
int ai_StepChapter1Step5(int *p);               /* 0x5766d0, transition callback */
int ai_Unk5767d0(int *p);               /* 0x5767d0, transition callback */
int ai_Unk576830(int *p);               /* 0x576830, transition callback */
void ai_Unk5768a0(int arg);             /* 0x5768a0, writes a record word */
int ai_Unk5768d0(int *p);               /* 0x5768d0, transition callback */
int ai_Unk576920(int *p);               /* 0x576920, transition callback */
int ai_Unk576970(int *p);               /* 0x576970, transition callback */
int ai_Unk5769c0(int *p);               /* 0x5769c0, transition callback */
void ai_Unk576a80(int arg);             /* 0x576a80, record lookup + 0x521960 */
int ai_Unk576ab0(int *p);               /* 0x576ab0, transition callback */
int ai_Unk576b50(int *p);               /* 0x576b50, transition callback */
int ai_Unk576ba0(int *p);               /* 0x576ba0, transition callback */
int ai_Unk576cd0(int *p);               /* 0x576cd0, step callback predicate */
int ai_Unk576d40(int *p);               /* 0x576d40, transition callback */
int ai_Unk576e70(int *p);               /* 0x576e70, step callback predicate */
int ai_Unk576ee0(int *p);               /* 0x576ee0, transition callback */
int ai_Unk577020(int *p);               /* 0x577020, step callback predicate */
void ai_Unk5770a0(int arg);             /* 0x5770a0, posts the message 0x2bc0 */
void ai_HookChapter3Step2(int arg);             /* 0x577110, posts to the player's character record */
int ai_Unk5771b0(int *p);               /* 0x5771b0, transition callback */
void ai_Unk577230(int arg);             /* 0x577230, posts messages */
int ai_QueryChapter5Step2(int *p);               /* 0x577290, step predicate */
int ai_MarkerChapter5Step2(void);                 /* 0x5772d0, the record fight kind */
int ai_Unk577320(void);                 /* 0x577320, the flag S+0x38 */
int ai_Unk577330(int *p);               /* 0x577330, transition callback */
int ai_MarkerChapter3Step8(void);                 /* 0x5773f0, fight kind + flag */
int ai_Unk577450(int *p);               /* 0x577450, step predicate */
int ai_Unk577490(void);                 /* 0x577490, releases the pending id S+0x50 */
void ai_Unk5774d0(int arg);             /* 0x5774d0, posts a record message */
void ai_Unk577540(int arg);             /* 0x577540, posts a record message */
void ai_HookChapter5Step4(int arg);             /* 0x5775c0, posts two character messages */
void ai_HookChapter5Step5(int arg);             /* 0x577610, character record lookup + 0x4e6180 */
void ai_HookChapter5Step6(int arg);             /* 0x577690, builds a save record (ROUGH) */
int ai_Unk577740(void);                 /* 0x577740, the tutorial start ("tutorial\main") */
int ai_Unk577820(void);                 /* 0x577820, 0x53d800 (player) */
int ai_Unk577850(void);                 /* 0x577850, run window controls 1 / 2 */
int ai_TutorialShowRunText(void);                 /* 0x5778b0, the run window's text tick */
int ai_TutorialIntroAnswered(void);                 /* 0x577a00, the tutorial's escape check */
void ai_TutorialShowText(void);                /* 0x577a90, rewrites the text window */
void ai_TutorialHideText(void);                /* 0x577b60, clears the text window */
void ai_TutorialShowText2(void);                /* 0x577b90, rewrites the second text window */
void ai_TutorialHideText2(void);                /* 0x577c60, clears the second text window */
void ai_Unk577cd0(void);                /* 0x577cd0, creates the progress form "tutorial\progress" */
void ai_Unk577d60(void);                /* 0x577d60, releases the progress form */
void ai_TutorialProgressSlider(void);                /* 0x577d90, the progress slider (state 0xa) */
int ai_TutorialStepForm(void);                 /* 0x577e50, the tutorial chapter form */
int ai_TutorialStepClicked(void);                 /* 0x5780e0, the control predicate on S+0x4c */
int ai_Unk578120(char *per);            /* 0x578120, sets the player S+0x1c */
void ai_Unk578150(void);                /* 0x578150, clears the player S+0x1c */
int ai_Unk578170(void);                 /* 0x578170, the gate predicate */
int ai_Unk578180(void);                 /* 0x578180, the tutorial start / record reset */
void ai_Unk578370(void);                /* 0x578370, the tutorial stop (handle + chain reset) */
void ai_Unk578440(char *rec);           /* 0x578440, frees the tutorial record chain */
int ai_Unk5784a0(void);                 /* 0x5784a0, allocates the main-intro record */
int ai_Unk578570(void);                 /* 0x578570, the chapter 1 chain (ROUGH) */
int ai_Unk578e40(void);                 /* 0x578e40, the chapter 2 chain (ROUGH) */
int ai_Unk579570(void);                 /* 0x579570, the chapter 3 chain (ROUGH) */
int ai_Unk57aa80(void);                 /* 0x57aa80, the chapter 4 chain (ROUGH) */
int ai_Unk57aeb0(void);                 /* 0x57aeb0, the chapter 5 chain (ROUGH) */
int ai_Unk57b750(void);                 /* 0x57b750, the main-outro record */
void ai_Unk57b810(float *a, float *b, float *c, float *d, float t, float *out);  /* 0x57b810, the Bezier */

/* declared in other units' headers before (moved to the owner) */
void ai_EndCharExtra(char *ch, int f);  /* 0x587c40 */
void ai_SetupTutorialPlayer(void);  /* 0x57bc30, sets up the tutorial player's profile */
int ai_TutorialWindow(void);  /* 0x57bce0, the tutorial chooser window: 1 starts the tutorial, 0 is the back */

int ai_SaveSlotTable(int fh);  /* 0x589aa0, writes the slot table 0x6f5ec0 */
#endif
