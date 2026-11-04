/* unk_575860.h -- the unit after he.c (unnamed): the tutorial module and its state machine,
   see the lead comment of src/unk_575860.c */
#ifndef UNK_575860_H
#define UNK_575860_H

struct HEEVSLOT;
struct SBSAMPLE;
struct AHMSTEP;
struct AIBLD;

typedef struct TUTSTEP {                /* 0x10 bytes, a step of a run record's chain (TUTRUN.steps) */
    unsigned int delay;                 /* 0x00, ms after the step started until its control is enabled */
    int kind;                           /* 0x04, the chapter kind 0 .. 3 (ai_TutorialStepForm) */
    int text;                           /* 0x08, the step's text (control 1 of the step form) */
    char *voice;                        /* 0x0c, the voice played with it, 0 = none */
} TUTSTEP;

typedef struct TUTMARK {                /* 0x2c bytes, a path of the position marker (TUTRUN.marks, ai_TutorialMarkerUpdate) */
    float from[2];                      /* 0x00, the start point x, y: the marker is placed there */
    float ctrl1[2];                     /* 0x08, the first Bezier control point */
    float ctrl2[2];                     /* 0x10, the second Bezier control point */
    float to[2];                        /* 0x18, the end point */
    float wait;                         /* 0x20, ms at the start before the move */
    float travel;                       /* 0x24, ms the move takes (the Bezier's t = elapsed / travel) */
    float rest;                         /* 0x28, ms at the end before it starts over */
} TUTMARK;

typedef struct TUTRUN {                 /* 0x74 bytes, a run (chapter) record of the tutorial, chained by next */
    int id;                             /* 0x00, the run's number (the main intro 0, the outro 7) */
    int tag;                            /* 0x04, a four-character tag ('INTR' main intro, 'OUTR' outro) */
    int mode;                           /* 0x08, 1 / 2 start with the timed first text window (state 4), else state 0xb */
    int mainText;                       /* 0x0c, the run window's text: control 1 shows it, control 2 mainText + 1 (ai_TutorialShowRunText) */
    char *mainVoice;                    /* 0x10, the voice played with it ("CHAPTER_5_C_MAIN") */
    int text1;                          /* 0x14, the first text window's text (ai_TutorialShowText) */
    char *text1Voice;                   /* 0x18, the voice played with the first text window (ai_TutorialShowText) */
    int text2;                          /* 0x1c, the second text window's text (ai_TutorialShowText2) */
    char *text2Voice;                   /* 0x20, the voice played with the second text window (ai_TutorialShowText2) */
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
    void (*stepShown)(void);            /* 0x54, called after every step form is shown and when the chain is done (ai_TutorialStepForm) */
    TUTSTEP *steps;                     /* 0x58 */
    int markCount;                      /* 0x5c, the entries of marks */
    int (*markPick)(int mode);          /* 0x60, called with the marker update's argument: the entry of marks to run, -1 and below hides the marker */
    TUTMARK *marks;                     /* 0x64, set: the position marker runs (ai_TutorialMarkerUpdate) */
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
    unsigned int textAt;                /* 0x08, d3_Time when the first text window last changed */
    unsigned int stepAt;                /* 0x0c, d3_Time when the step / tail started */
    int campaignSteps;                  /* 0x10, counts ai_CampaignStep's handled steps, 0 when a run starts */
    struct SBSAMPLE *voice;             /* 0x14, the playing voice */
    TUTRUN *run;                        /* 0x18, the current run record */
    TUTHOST *host;                      /* 0x1c */
    int continueObj;                    /* 0x20, the intro window's control 3 text object (0x577740; text 0x7d): a click on it answers the intro (ai_TutorialIntroAnswered), -1 = none */
    int text1Win;                       /* 0x24, the first text window */
    int text1Shown;                     /* 0x28 */
    int statusShown;                    /* 0x2c, the status text of the states 0xa / 0xb is shown */
    int text2Shown;                     /* 0x30 */
    int text2Win;                       /* 0x34, the second text window */
    int goalDone;                       /* 0x38, set by the run's step callbacks when their goal is met, 0 when a run starts */
    int phase;                          /* 0x3c, 0 .. 3 */
    unsigned int step;                  /* 0x40, the step of the run's chain */
    int stepKind;                       /* 0x44, the kind 0 .. 3 of the open step form, 4 = none */
    int stepForm;                       /* 0x48, ai_TutorialStepForm's form */
    int stepControl;                    /* 0x4c, its control, -1 = none */
    int stepArg;                        /* 0x50, ai_CampaignStep's argument b of the step that ended the chain */
    int markerStep;                     /* 0x54, the marker's path (TUTRUN.marks) */
    int markerPhase;                    /* 0x58, 1 hidden, 2 waiting at the start, 3 moving, 4 resting at the end */
    unsigned int markerAt;              /* 0x5c, d3_Time when the marker's phase started */
    int markerObj;                      /* 0x60, the marker's picture object, -1 = none */
    int markerDepth;                    /* 0x64, sv_FrameDepth when the marker was placed: it is enabled only while that is current */
    int progressForm;                   /* 0x68, the progress form of the state 0xa (ai_TutorialOpenProgress), -1 = none */
    int progressSlider;                 /* 0x6c, its slider */
} TUTSTATE;

typedef struct AIBOOKMARK {             /* 0xc bytes, a building bookmark on a function key (ai_Bookmarks) */
    unsigned char key;                  /* 0x0, the key code kb_Last answers (0x3b + i: F1 .. F10, 0x57: F11) */
    int building;                       /* 0x4, the building id (its +1), -1 = none */
    int object;                         /* 0x8, the id of the building's main object (its +2), -1 = none */
} AIBOOKMARK;

void ai_GypsyHelperWindow(struct AIBLD *rec); /* 0x584d70, the helpers window of a building (HANDEL_SUCHEN form), opened by the gypsy production contact */
extern int ai_TutorialChapterOn[5];     /* 0x62918c, per chapter 1 = played */
extern int (*ai_TutorialChapterInit[5])(void);  /* 0x6291a0, the chapter chain builders */
extern int ai_DragObj;  /* 0x62a8d0, the right-held d2 object whose tooltip ai_DragWindow shows */
extern int ai_DragViewWin;  /* 0x62a8d4, the first window of the tooltip view */
extern char *ai_DragPicked;  /* 0x6f266c, the picked object the tooltip was last opened for */
extern int ai_DragCutscene;  /* 0x6f2670, 1 while the tooltip view holds d2_CutsceneActive set */
extern int ai_ScreenshotCount;  /* 0x6f2674, the number of the next screenshot */
extern AIBOOKMARK ai_Bookmarks[11];  /* 0x6f5ec0, the function-key building bookmarks */
extern unsigned int ai_GraveChatterAt;  /* 0x62a538, d3_Time at the last graveyard chatter tick */
extern int ai_TutorialBanks[6];         /* 0x6f2640, the tutorial's voice banks */
extern TUTRUN *ai_TutorialRuns;         /* 0x6f2658, the chain of the tutorial's run records */
extern int ai_TutorialOn;               /* 0x6f265c, the tutorial runs */
extern int ai_TutorialRunning;          /* 0x6f2660, its state machine advances */
int ai_TutorialTick(void);                 /* 0x575860, the tutorial's tick: runs its state machine */
void ai_TutorialMarkerUpdate(int mode);  /* 0x57b8a0, moves the tutorial's position marker */
void unk_ClearEndGameGate(void);                /* 0x576020, clears the gate 0x6f2660 */
void unk_SetEndGameGate(void);                /* 0x576030, sets the gate 0x6f2660 */
void ai_CampaignStep(int a, int b, int c, int d);  /* 0x576040, one step of the campaign state machine */
int ai_ScriptQuery(int a, int b, int c, int d);   /* 0x576340, calls the run record's callback +0x3c */
int ai_RunAllows(int mask);             /* 0x5763c0, run mask +0x68 */
int ai_RunAllowsContact(int mask);             /* 0x576410, run mask +0x6c */
int ai_RunAllowsWindow(void);                 /* 0x576460, 1 while the tutorial lets a building window open */
int ai_TutorialQueryDefault(int *p);               /* 0x576490, step callback predicate */
int ai_QueryDefault(int *p);               /* 0x5764b0, step callback predicate */
int ai_QueryBuilding0f(int *p);               /* 0x576500, step callback predicate */
int ai_QueryChapter2Step3(int *p);               /* 0x576590, step callback predicate */
int ai_QueryChapter2Step4(int *p);               /* 0x5765e0, step callback predicate */
int ai_QueryChapter2Step5(int *p);               /* 0x576600, step callback predicate */
int ai_QueryChapter2Step6(int *p);               /* 0x576640, step callback predicate */
int ai_QueryChapter2Step7(int *p);               /* 0x576680, step callback predicate */
int ai_StepChapter1Step5(int *p);               /* 0x5766d0, transition callback */
int ai_StepChapter1Step10(int *p);               /* 0x5767d0, transition callback */
int ai_StepChapter2Step1(int *p);               /* 0x576830, transition callback */
void ai_HookChapter2Step2(int arg);             /* 0x5768a0, writes a record word */
int ai_StepChapter2Step2(int *p);               /* 0x5768d0, transition callback */
int ai_StepChapter2Step3(int *p);               /* 0x576920, transition callback */
int ai_StepChapter2Step4(int *p);               /* 0x576970, transition callback */
int ai_StepChapter2Step5(int *p);               /* 0x5769c0, transition callback */
void ai_HookChapter2Step5(int arg);             /* 0x576a80, record lookup + 0x521960 */
int ai_StepChapter2Step6(int *p);               /* 0x576ab0, transition callback */
int ai_StepChapter2Step7(int *p);               /* 0x576b50, transition callback */
int ai_StepChapter4Step2(int *p);               /* 0x576ba0, transition callback */
int ai_QueryChapter4Step2(int *p);               /* 0x576cd0, step callback predicate */
int ai_StepChapter4Step4(int *p);               /* 0x576d40, transition callback */
int ai_QueryChapter4Step4(int *p);               /* 0x576e70, step callback predicate */
int ai_StepChapter4Step5(int *p);               /* 0x576ee0, transition callback */
int ai_QueryChapter4Step5(int *p);               /* 0x577020, step callback predicate */
void ai_HookChapter4Step(int arg);             /* 0x5770a0, posts the message 0x2bc0 */
void ai_HookChapter3Step2(int arg);             /* 0x577110, posts to the player's character record */
int ai_QueryChapter3Step6(int *p);               /* 0x5771b0, transition callback */
void ai_HookChapter5Step2(int arg);             /* 0x577230, posts messages */
int ai_QueryChapter5Step2(int *p);               /* 0x577290, step predicate */
int ai_MarkerChapter5Step2(int mode);                /* 0x5772d0, the record fight kind */
int ai_MarkerChapter3Step6(int mode);                /* 0x577320, the flag S+0x38 */
int ai_QueryChapter3Step8(int *p);               /* 0x577330, transition callback */
int ai_MarkerChapter3Step8(int mode);                /* 0x5773f0, fight kind + flag */
int ai_QueryChapter3Step10(int *p);               /* 0x577450, step predicate */
int ai_StepShownChapter3Step4(void);                 /* 0x577490, releases the pending id S+0x50 */
void ai_HookChapter3Intro(int arg);             /* 0x5774d0, posts a record message */
void ai_HookChapter5Intro(int arg);             /* 0x577540, posts a record message */
void ai_HookChapter5Step4(int arg);             /* 0x5775c0, posts two character messages */
void ai_HookChapter5Step5(int arg);             /* 0x577610, character record lookup + 0x4e6180 */
void ai_HookChapter5Step6(int arg);             /* 0x577690, builds a save record (ROUGH) */
int ai_Unk577740(void);                 /* 0x577740, the tutorial start ("tutorial\main") */
int ai_Unk577820(void);                 /* 0x577820, 0x53d800 (player) */
int ai_TutorialClearRunWindow(void);                 /* 0x577850, run window controls 1 / 2 */
int ai_TutorialShowRunText(void);                 /* 0x5778b0, the run window's text tick */
int ai_TutorialIntroAnswered(void);                 /* 0x577a00, the tutorial's escape check */
void ai_TutorialShowText(void);                /* 0x577a90, rewrites the text window */
void ai_TutorialHideText(void);                /* 0x577b60, clears the text window */
void ai_TutorialShowText2(void);                /* 0x577b90, rewrites the second text window */
void ai_TutorialHideText2(void);                /* 0x577c60, clears the second text window */
void ai_TutorialOpenProgress(void);                /* 0x577cd0, creates the progress form "tutorial\progress" */
void ai_TutorialCloseProgress(void);                /* 0x577d60, releases the progress form */
void ai_TutorialProgressSlider(void);                /* 0x577d90, the progress slider (state 0xa) */
int ai_TutorialStepForm(void);                 /* 0x577e50, the tutorial chapter form */
int ai_TutorialStepClicked(void);                 /* 0x5780e0, the control predicate on S+0x4c */
int ai_Unk578120(char *per);            /* 0x578120, sets the player S+0x1c */
void ai_Unk578150(void);                /* 0x578150, clears the player S+0x1c */
int ai_Unk578170(void);                 /* 0x578170, the gate predicate */
int ai_TutorialStart(void);                 /* 0x578180, the tutorial start / record reset */
void ai_TutorialStop(void);                /* 0x578370, the tutorial stop (handle + chain reset) */
void ai_TutorialFreeRuns(char *rec);           /* 0x578440, frees the tutorial record chain */
int ai_TutorialInitMainIntro(void);                 /* 0x5784a0, allocates the main-intro record */
int ai_TutorialInitChapter1(void);      /* 0x578570, the chapter 1 chain */
int ai_TutorialInitChapter2(void);      /* 0x578e40, the chapter 2 chain */
int ai_TutorialInitChapter3(void);                 /* 0x579570, the chapter 3 chain */
int ai_TutorialInitChapter4(void);                 /* 0x57aa80, the chapter 4 chain */
int ai_TutorialInitChapter5(void);                 /* 0x57aeb0, the chapter 5 chain */
int ai_TutorialInitMainOutro(void);                 /* 0x57b750, the main-outro record */
void ai_TutorialBezier(float *a, float *b, float *c, float *d, float t, float *out);  /* 0x57b810, the Bezier */
void ai_AlchemistCandidates(unsigned char *obj);  /* 0x57bf00, the alchemist's candidates window: hires one of the walk steps */
int ai_AlchemistHire(unsigned char *obj, struct AHMSTEP *step, int value);  /* 0x57c560, hires the candidate step (ROUGH) */

/* declared in other units' headers before (moved to the owner) */
int ai_PermissionBits1d(char *ch, char *b, char *x);  /* 0x587b70, the permission bits of the character ch in the building b of the type class 0x1d (gm_PermissionBits) */
void ai_EndCharExtra(char *ch, int f);  /* 0x587c40 */
void ai_SetupTutorialPlayer(void);  /* 0x57bc30, sets up the tutorial player's profile */
int ai_TutorialWindow(void);  /* 0x57bce0, the tutorial chooser window: 1 starts the tutorial, 0 is the back */

void ai_GraveyardContact(char *rec);       /* 0x586840, the graveyard's contact dispatcher */
void ai_GraveHauntContact(struct AIBLD *rec);    /* 0x586ab0, the graveyard contact HEIMSUCHUNG */
void ai_GraveAgeContact(struct AIBLD *rec);      /* 0x586e40, the graveyard contact ALTERN_LASSEN */
void ai_GravePlanContact(char *rec);       /* 0x5871c0, the graveyard contact LIEGEPLAN */
void ai_GraveRentContact(struct AIBLD *rec);     /* 0x5871e0, the graveyard contact GRAB_MIETEN */
void ai_GraveCareContact(char *rec);       /* 0x587300, the graveyard contact GRAEBER_PFLEGEN */
void ai_GraveHelperContact(char *rec);     /* 0x587340, the graveyard contact GEHILFEN_ERSCHAFFEN */
void ai_GraveInsuranceContact(struct AIBLD *rec);/* 0x587370, the graveyard contact VERSICHERUNG */
void ai_GraveyardChatter(char *unused);    /* 0x587c90, the graveyard's idle-chatter tick */
void ai_HandleItemDrag(void);  /* 0x587e00, the right-button tooltip handler */
int ai_OpenBuildingTooltip(int idx);  /* 0x588160, opens the tooltip view of the building type idx */
int ai_OpenItemTooltip(int idx);  /* 0x588310, opens the item tooltip view of the goods item idx */
int ai_OpenUpgradeTooltip(int idx);  /* 0x588950, opens the upgrade tooltip view of the goods item idx */
int ai_OpenHelpTooltip(char *s);  /* 0x588bb0, opens the help tooltip of the record named s */
int ai_OpenCharTooltip(char *ch);  /* 0x588c60, opens the tooltip view of the character ch */

void ai_InitBookmarks(void);  /* 0x589410, clears the 11 bookmark slots and sets their function keys */
int ai_SaveBookmarks(int fh);  /* 0x589aa0, writes the slot table 0x6f5ec0 */

extern const float ai_CannonPitchMax;   /* 0x5eec6c, 45 degrees: the highest pitch of the cannon */
extern const float ai_CannonPitchMin;   /* 0x5eec70, -15 degrees: the lowest pitch */
extern const float ai_CameraPitchMin;   /* 0x5eec74, -35 degrees: the low end of the cannon camera's pitch range */
extern const float ai_CameraPitchMax;   /* 0x5eec78, 45 degrees: its high end */
extern const float ai_CameraPitchZero;  /* 0x5eec7c, the level camera */
extern const struct D3VECTOR ai_WimpelPos;  /* 0x5eec80, the wimpel's place on the cannon base */
extern const float ai_CannonSwitchMs;    /* 0x5eec94, the length of each half of the barrel switch (ms) */
extern const float ai_CannonSwitchDist;  /* 0x5eec98, how far the cannon sinks back while the barrel switches */
extern const float ai_CannonZoomRange;  /* 0x5eecac, the distance the zoom moves the camera out */
#endif
