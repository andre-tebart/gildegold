/* unk_5d33b0.h -- the screen / front-end module (unnamed), see src/game/unk_5d33b0.c and
   notes/units/unk_5d33b0.md */
#ifndef UNK_5D33B0_H
#define UNK_5D33B0_H

#include "unk_5d33b0_types.h"        /* OPTIONBYTES */

/* The option values the profile keeps (the reader 0x5d5640 and the writer 0x5d4c70, the "Game" /
   "Gfx" / "Sound" sections of 0x75e480). The reader stores the camera / mouse speed as single bytes;
   the run time reads them as integers here. */
extern int ai_OptionSpeed;          /* 0x6f3608, the "speed" key: the game speed setting (0x2ee = 4.9375) */
extern int ai_OptionCameraSpeed;    /* 0x6f360c, the "camera_speed" key (the reader stores the low byte) */
extern int ai_OptionMouseSpeed;     /* 0x6f3610, the "mouse_speed" key (the input base is this - 0x40) */

/* 0x6f3600: the "Sound" options of the profile (the reader 0x5d5640), the master_vol / sfx_vol /
   msx_vol / speech_vol bytes and the msx_freq dword. The byte fields are bitfields on a char base
   type: the original's compiler loads the packed master + sfx dword as a whole and extracts the
   second byte with `mov dl, ah` - a plain unsigned char field would be a separate byte load. */
typedef struct SNDOPTIONBYTES {
    unsigned char masterVolume:8;   /* +0x0, "master_vol" */
    unsigned char sfxVolume:8;      /* +0x1, "sfx_vol" */
    unsigned char msxVolume:8;      /* +0x2, "msx_vol" */
    unsigned char voiceVolume:8;    /* +0x3, "speech_vol" */
    union {
        int msxFreq;                /* +0x4, "msx_freq" (the low byte is used) */
        unsigned char msxFreqByte[4]; /* +0x4, the same as the four bytes the reader stores */
    };
} SNDOPTIONBYTES;
extern SNDOPTIONBYTES ai_SoundOptions;  /* 0x6f3600 */

extern float snd_VoiceVolume;       /* 0x602b08, the voice volume fraction (f3snd.h) */
extern float snd_MusicVolume;       /* 0x602d68, the music volume 0..1 (f3snd.h) */
void sb_SetMasterVolume(int volume);/* 0x427370, sblib.c (that unit has no header yet) */
/* msx.h / f3snd.h redefine msx_Play with different types, so the unit cannot include both; the two
   setters are repeated from msx.h here. */
void msx_SetVolume(int vol);        /* 0x4239a0, the music master volume */
void msx_SetAmbVolume(int vol);     /* 0x423a50, the ambient master volume */

/* The run-time values ai_ApplyOptions writes; their own units declare them too (ai.h / cm.h / sv.c). */
extern int ai_GameFlags;  /* 0x6f23f8, the game mode flags: bit 2 = network game (ai.h, cm.h) */
extern unsigned int cm_SpeedLevel;  /* 0x60bd84, the shared speed level 0..4 of a network game (cm.h) */
extern float cm_SpeedFactor;        /* 0x5f1290, the game speed factor (cm.h) */
extern int ai_CameraStep;           /* 0x623814, the camera move step, 100 - camera speed; sv.c converts
                                       it with fild at the float uses (the original loads an int) */

typedef struct EFFECTREC {              /* 0x10 bytes at 0x62de60: the default effect (the wind): a vector and its scale */
    float pos[3];                       /* +0x00, 0, 0, 1.0 at the start; ai_InitWind sets (sin, 0, cos) of the wind direction 6 */
    union {
        float scale;                    /* +0x0c, 25.0 at the start */
        int scaleBits;                  /* the same bits as an int: 0x5dbc20 passes them through an int K&R view */
    };
} EFFECTREC;
extern EFFECTREC ai_DefaultEffect;  /* 0x62de60 */
extern float ai_ShotDrag;          /* 0x62de58, .data: the drag coefficient of a cannon ball, 1.5 */
extern float ai_ShotHitHeight;     /* 0x62de5c, .data: 98.4 = 2.5 m in inches, the height above the ground that counts as a hit */
typedef struct EFFECTSTATE {            /* 0x1f0 bytes at 0x6f3300: the wind state ai_InitWind builds (reasoned: the caller 0x525510 passes the wind rose tables) */
    int year;                           /* +0x00, the clock of the last update (a GAMECLOCK spread to ints): ai_InitWind's second argument (the clock's year) */
    int hour;                           /* +0x04, 6 after ai_InitWind; ai_UpdateWind stores the clock's hour */
    int min;                            /* +0x08, 0 after ai_InitWind; ai_UpdateWind stores the clock's minute */
    float sinTab[24];                   /* +0x0c, a copy of the caller's first table (sin of the 24 directions) */
    float cosTab[24];                   /* +0x6c, a copy of the second table (cos) */
    float base[24];                     /* +0xcc, random per direction: 60 * random * k + 1 (k by the day phase) */
    float gust[24];                     /* +0x12c, random: 60 * random * k + c, or 60 * random * k */
    float phase[24];                    /* +0x18c, random: (random + n) * pi (or 2 pi) */
    float angle;                        /* +0x1ec, a random angle 0 .. 2 pi */
} EFFECTSTATE;
extern EFFECTSTATE ai_EffectState;  /* 0x6f3300 */

/* The five-float record the sphere volume 0x5dc1a0 fills: radius, size, the two metrics derived from
   the size and the reach volume * radius. am.c passes the same record to 0x5dc210 as int *in. */
typedef struct SPHEREVOLUME {
    float radius;       /* +0x00 */
    float size;         /* +0x04 */
    float surface;      /* +0x08, 2 pi size^2 */
    float volume;       /* +0x0c, 4/3 pi size^3 */
    float reach;        /* +0x10, volume * radius */
} SPHEREVOLUME;
typedef struct SHOTVEL {                /* 0x10 bytes: a velocity as direction * speed and the speed */
    D3VECTOR v;
    float speed;
} SHOTVEL;
typedef struct SHOTREC {                /* 0x68 bytes: a cannon ball in flight (am.c 0x4a6ae0 builds one per ball with ai_InitShot
                                           from the barrel direction, the charge, the ball size and 7874 = the density of iron;
                                           gv.c 0x4edf80 moves it with ai_StepShot); the units are meters and seconds, the
                                           positions are in the world's units (inches: 39.37 per meter) */
    SHOTVEL velStart;                   /* +0x00, the start velocity: barrel direction * speed, speed = charge / mass * 0.1 */
    SHOTVEL vel;                        /* +0x10, the current velocity (m/s), the start velocity at first */
    D3VECTOR posStart;                  /* +0x20, the start position */
    float pad2c;
    D3VECTOR pos;                       /* +0x30, the current position, the start position at first */
    float pad3c;
    EFFECTREC wind;                     /* +0x40, the wind (ai_DefaultEffect) with its scale clamped to 0 .. 60 and the vector scaled by it */
    float area;                         /* +0x50, the ball's SPHEREVOLUME surface (2 pi size^2), the drag area */
    float mass;                         /* +0x54, volume * density */
    float energy;                       /* +0x58, set by ai_StepShot when the ball lands: 0.5 * mass * v^2 (0 when it left the map) */
    int state;                          /* +0x5c, 1 in flight, 2 landed, 3 left the height map */
    int time;                           /* +0x60, d3_Time (ms) of the last flight step (one step per ms) */
    unsigned steps;                     /* +0x64, the flight steps made; ai_ScatterShot works only before the first */
} SHOTREC;
typedef struct POKERPANEL {             /* 0x1c bytes: one panel (form view) of the poker window */
    int win;                            /* +0x00, the window the form view made (d2_LastWindow after d2_SetForm) */
    int slot[5];                        /* +0x04, the panel's controls / seats, -1 = none yet */
    int shown;                          /* +0x18, 1 for the form 6 panel, 0 for the forms 2 and 3 */
} POKERPANEL;
typedef struct POKERUI {                /* 0x70 bytes at 0x62dda0: the poker window state */
    int window;                         /* +0x00, the view of the open poker window, -1 when closed */
    int buttons[5];                     /* +0x04, the ids of the button row (the third is disabled at the start) */
    int betSlider;                      /* +0x18, the bet slider (made by ai_PokerBetControl), -1 until then */
    POKERPANEL panel[3];                /* +0x1c, the panels of the forms 3, 1 and 2, in memory order */
} POKERUI;
typedef struct POKERWHO {               /* a seat's character record, the two names the game prints (G10NPC / AICHAR: +0x30 / +0x40) */
    char pad00[9];
    unsigned char female;               /* +0x09, G10NPC.female: picks the title text row (+ 10) */
    char pad0a[3];
    unsigned char title;                /* +0x0d, G10NPC.unk0d (1 .. 4): the title text (txt_Ptrs[0x114 + 10 * female + title]) */
    char pad0e[0x22];
    char vname[16];                     /* +0x30, the first name */
    char nname[16];                     /* +0x40, the family name */
} POKERWHO;
typedef struct POKERSEAT {              /* 0x34 bytes, unk_4a7140.h's G10SEAT; seat 0 is the player, 1 and 2 the two opponents */
    POKERWHO *who;                      /* +0x00 */
    int model;                          /* +0x04 */
    int card[5];                        /* +0x08, indices into the deck, -1 = none (what ai_PokerSeatPanel shows) */
    int unk1c, unk20;                   /* +0x1c */
    int dropped;                        /* +0x24, the deck index of the card the seat put away in the exchange (ai_Unk4a79d0 sets it), -1 = none */
    int foldPhase;                      /* +0x28, the phase the seat folded in (ai_Unk4a7c20), -1 = still in */
    int unk2c, unk30;                   /* +0x2c */
} POKERSEAT;
typedef struct POKERGAME {              /* 0xc0 bytes at 0x6f3500, unk_4a7140.h's G10GAME (ai_PokerStart fills it, ai_PokerStep steps it) */
    int phase;                          /* +0x00, 1 .. 11: the step ai_PokerGame shows */
    POKERSEAT seat[3];                  /* +0x04 */
    int pot;                            /* +0xa0, G10GAME.unka0: the number printed with the text 0x4252 in every phase (reasoned: the pot) */
    int minBet;                         /* +0xa4, G10GAME.stake: the opening stake (money / (random 3 + 7)); printed with the text 0x4251 */
    int maxBet;                         /* +0xa8, G10GAME.money: the upper limit of the bet slider */
    int bet;                            /* +0xac, G10GAME.stake2: the bet chosen with the slider (starts as minBet) */
    int actor;                          /* +0xb0, G10GAME.unkb0: 0 = the player acts, 1 / 2 an opponent (reasoned) */
    unsigned char left;                 /* +0xb4, cards left in the deck */
    unsigned char unkb5;                /* +0xb5 */
    short pad;
    short winner;                       /* +0xb8, G10GAME.unkb8: the seat that won (0 = the player; 4 at the start) (reasoned) */
    short padba;
    int winRank;                        /* +0xbc, G10GAME.unkbc: the hand rank of the win, the text 0x4266 + rank names it (reasoned) */
} POKERGAME;
int gm_ConvertMoney(int a, unsigned char k);   /* 0x5085b0, f3gm.c: a / the divisor of the player slot k, rounded */
extern POKERGAME ai_PokerState;     /* 0x6f3500 */
void ai_PokerSeatPanel(int seat, int mode, int *cards);  /* 0x5dadf0, builds the seat panel (the three-string message) */
void ai_PokerWindow2(void);            /* 0x5db110, the second poker window "Locations\zigeuner\poker2" */
struct AIPER;
struct G10GAME;
int ai_PokerStart(struct AIPER *p, struct G10GAME *g);  /* 0x4a7140, unk_4a7140.c: starts a poker game of the player's character into g (ai_PokerState), 0 when it cannot */
int ai_PokerStep(void);             /* 0x4a74e0, unk_4a7140.c: one step of the game, returns the button mask to show (0x10 / 0x21 / 9 / 0x29 / 0x1c) or 0 */
void ai_PokerExchange(int k);           /* 0x4a8630, unk_4a7140.c: the player exchanges card k */
int gm_PlayerFactor(int a, unsigned char k);   /* 0x5085f0, f3gm.c: a * the factor of the player slot k */
int gm_GetWorth(unsigned short id);            /* 0x50a640, f3gm.c: the worth of the character id */
short *gm_PlayerRecord(char *c);               /* 0x506430, f3gm.c: the player record of a character (kinds 5 / 6 / 7), 0 else */
int util_Random(unsigned short n);             /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, ai_Unk4d88c0 ..) call util_RandomU, a cast in their head */
float util_RandomF(void);                      /* 0x42ec20, tr.c */
extern POKERUI ai_PokerUi;          /* 0x62dda0 */
int ai_OpenPokerWindow(void);       /* 0x5d9de0, opens the poker window (1 = opened, 0 = already open / refused) */
/* Two entries of the run-time text table (the .data slots hold char * into the loaded text file; the
   contents are not in the exe). ai_Quicksave shows them when the network quicksave is refused. */


/* The 0x0f network message the quicksave sends: cm_SendExSysMessage copies 0x1f dwords (0x7c B) of it
   into the message data. +0 = the save slot (1 = quicksave, the value the local path passes to
   ls_SaveGame as its third argument), +4 = the save name. */
typedef struct CMQUICKSAVE {
    int slot;                           /* +0x00 */
    char name[0x78];                    /* +0x04, "QUICKSAVE" */
} CMQUICKSAVE;
extern int ai_GameState; /* 0x6f25ac, the game state: 0 loading / paused, 1 running, 2 ... (as cm.h / f3snd.h) */
extern unsigned char ls_SaveSlot;   /* 0x6f1c38, the save slot the next save records: 0 autosave, 1 quicksave,
                                       the network save slot (as ls.c, which writes it into the save header) */
extern char ch_EmptyName[];         /* 0x631090, the empty status / character name (f3init.c, f3gm.c) */
extern unsigned short ai_ScreenImage[]; /* 0x766320, the 0x78 x 0xa0 16-bit screen image (as ls.c) */

void ai_ApplyOptions(void);         /* 0x5d5e90, applies the option values to the run time */
void ai_SaveOptions(void);          /* 0x5d4c70, writes the options to the INI */
int ai_ApplyGfxOptions(void);     /* 0x5d5c40, applies the graphics options (the shadow setup) */
void ai_LoadOptions(void);          /* 0x5d5640, reads the options from the INI */
extern int sv_ScreenDirty;          /* 0x6f2358, sv.c: nonzero = rebuild the screen (sv_Unk5229b0 clears it) */
void sv_DestroyScreen(void);        /* 0x5223c0, sv.c: destroys the screen's widgets and views */
int sv_PanelImage(void);            /* 0x522040, sv.c: the panel image number of the player's group */
void sv_BuildScreenLayout(int s);   /* 0x521e70, sv.c: builds the main screen layout for the panel image s */
/* The gypsy windows' callees (the special-amount list of unk_58bbd0.c, the money / relation messages). */
void cm_SendRelations(int a, int b, int c, int d);   /* 0x4c4b00, cm.c: message 0x1b, the relation of a to b changes by c */
float gm_Mix(int a, int b, int c);                   /* 0x504e20, f3gm.c: the mix of two relation values */
int sv_MessageBoxGreen(int a, int b, unsigned flags); /* 0x521410, sv.c: the green message box with the title text a and the text b */
struct AIBLD;
struct AIPER;
int gm_BathObjectCount(char *b);             /* 0x50cf60, f3gm.c: 1 + the kitchen / bath objects of the building (as unk_4a7140.h) */
void ai_GypsyDeliveryText(unsigned short *param_1, int param_2, int *param_3); /* 0x5a9470, unk_58bbd0.c: the delivery text for n chosen records (as unk_4a7140.h) */
int ai_AmountRowStatus(int *row);                    /* 0x5be1d0, unk_58bbd0.c: the status fields of a list row */
int ai_AmountRow(int x, int y, int win, int pic, int *row, int mode); /* 0x5be3b0, unk_58bbd0.c: draws a list row at (x, y) */
void gm_SendMoneyDelta(int a, int b);                /* 0x50cc50, f3gm.c: forwards to cm_SendMoneyDelta */
int amt_GetKindLevel(int k);                         /* 0x4e7c40, amt.c: the level of the office kind k */
extern int snd_SystemBank;        /* 0x6f2400, main.c: the handle of the system sample bank (system.sbf) */
extern int main_ScreenRestart;     /* 0x6f25a4, main.c: nonzero = re-initialise the screen (the resolution changed) */
extern char dd_Has1024x768;         /* 0x631226, dd_vesa.h */
extern char dd_Has1152x864;         /* 0x631227, dd_vesa.h */
void d2_SetLabel(int id, char *s);                         /* 0x46bc70, d2.h: sets the widget's label */
void d2_SetList(int id, int n, ...);                       /* 0x46bca0, d2.h: sets the widget's "a|b|c|" item list */
void d2_SetSliderFlags(int id, int flags);                 /* 0x46bdd0, d2.h: sets the slider's flags */
void d2_SetObjValue();                                     /* 0x475cb0, d2.h (K&R: d2.c takes a 5th parameter, the call sites push 2, 4 or 5) */
#define d2_SetObjValue5 ((void (*)(int, int, int, int, int))d2_SetObjValue) /* the 5-argument call of the gypsy windows */
typedef struct AMOUNTCFG {              /* 0x50 bytes, the config of the special-amount menu ai_AmountMenu (unk_58bbd0.c) */
    int flags;                          /* +0x00, 1 / 0x20400 / 0x2040a .. (the gypsy windows: 1) */
    int mode;                           /* +0x04, 0x400 */
    unsigned __int64 filter;            /* +0x08, 0x1a7fdfbb6 (the gypsy windows), 0x1a07c43a0 (the sabotage) */
    int kind;                           /* +0x10, 6 */
    int option14;                       /* +0x14, 2 in the blood revenge window (meaning not derived) */
    int unk18;                          /* +0x18 */
    unsigned short ids[16];             /* +0x1c, the character ids the menu leaves out / lists, 0xffff = none */
    char pad3c[0x14];
} AMOUNTCFG;
#pragma pack(push, 2)
typedef struct GAMECLOCK {              /* 14 bytes, ai.h's AITIME (ai_Time itself is char[] here, f3snd.h) */
    int year;                           /* +0x00 */
    unsigned short hour;                /* +0x04 */
    int min;                            /* +0x06 */
    int xy;                             /* +0x0a */
} GAMECLOCK;
#pragma pack(pop)

typedef struct CURSEMSG {               /* 0xf8 bytes, ai.h's AIMSG (the long message cm_SendLongMessage sends) */
    int unk00;
    unsigned char type;                 /* +0x04 */
    char pad05[3];
    int to;                             /* +0x08, the net id of the sender (the message goes to the player's group) */
    int fromId;                         /* +0x0c */
    char pad10[0x18];
    GAMECLOCK time;                     /* +0x28 */
    unsigned char flags;                /* +0x36 */
    char pad37;
    int ids[8];                         /* +0x38 */
    int arg[11];                        /* +0x58 */
    char pad84[0x74];
} CURSEMSG;

#pragma pack(push, 1)
typedef struct CURSEBLD {               /* the building record: the id and the owner */
    unsigned char type;                 /* +0x00 */
    int id;                             /* +0x01 */
    char pad05[0x22];
    unsigned short owner;               /* +0x27 */
} CURSEBLD;
#pragma pack(pop)

#pragma pack(push, 2)
typedef struct CURSEEVENT {             /* the event 0xa0 ai_FindCurseEvent finds: only the fields the curse target window reads */
    char pad00[0x44];
    GAMECLOCK from;                     /* +0x44, the clock the event started */
    GAMECLOCK due;                      /* +0x52, the clock it is due */
    char pad60[0x10];
    int done;                           /* +0x70 */
    char pad74[0x38];
    int victim;                         /* +0xac */
} CURSEEVENT;
#pragma pack(pop)
struct GCLOCK;
struct AITIME;
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);   /* 0x4fe140, f3gm.c: adds hours / minutes / seconds to the clock */
int gm_MinutesBetween(struct AITIME *a, struct AITIME *b);          /* 0x4fe280, f3gm.c: the minutes from clock a to clock b */
int ls_Read(void *buf, int size, int count, int f);                 /* 0x51c920, ls.c's read (true when the fread returned a positive count); ls.c has no header yet */
char *ai_FindCurseEvent(char *a);                              /* 0x5741d0, he.c: the pending curse event of the building (as unk_4a7140.h) */
int gm_CurseObjectCount(char *b);                            /* 0x50cfd0, f3gm.c: 2 + the objects 0x274 / 0x27e / 0x28e / 0x291 of the building (as unk_4a7140.h) */
int gm_CurseTargetCount(char *b);                                   /* 0x50d0b0, f3gm.c: counts from the objects 0x284 / 0x289 of the building */
/* the filter of the gypsy windows: a const object, the original loads it from .rdata 0x5eed68 (mov eax,[..]; mov ecx,[..+4]) */
static const volatile int ai_CardBack = 0x18;  /* 0x5ed1bc, .rdata: the picture offset of the card back (the original loads it from memory) */
static const volatile unsigned __int64 gypsyFilter = 0x1a7fdfbb6;
static const float ai_InchPerMeter = 39.37008f;  /* 0x5eedf4, .rdata: world units (inches) per meter */
static const float ai_Gravity = 9.81f;            /* 0x5eedfc, .rdata: m/s^2 */
extern const char ai_AxisPair[];    /* 0x5ebb74, cm.h: the (0, 0, 1) / (0, 1, 0) axis pair, 32 bytes */
static const float ai_ShotStep = 0.013513514f;    /* 0x5eee00, .rdata: 1/74, the time (s) of one flight step of ai_StepShot */
static const float ai_ShotHalfStep = 0.0067567569f;  /* 0x5eee04, .rdata: 1/148, the step of ai_StepShotSlow */
static const float ai_AirDensity = 1.2f;          /* 0x5eee0c, .rdata: kg/m^3 */
static const float ai_WindGain = 60.0f;  /* 0x5eee14, .rdata: the factor of ai_Unk5dbc30's per-direction random values (the original loads it from memory, it is not folded) */
int ai_ShotHitsGround(struct D3SM *map, D3VECTOR *from, D3VECTOR *to, int px, int pz, int *npx, int *npz);  /* 0x5dcf00, 1 when the move from -> to of a flight step hits the ground (to is moved to the hit) */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win); /* 0x46a960, d2.c: a slider in a window */
int sv_CheckGoodAmount(char *o, int n, unsigned t);   /* 0x521040, sv.c: 1 when the character's stock covers n of the good t (callers that push the good as a byte: sv_CheckGoodAmountB in their head) */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); /* 0x4c4830, cm.c */
int cm_SendLongMessage(char *msg);                         /* 0x4c4b70, cm.c: sends a long (0xf8 bytes) message */
extern unsigned char ai_OwnPlayer;                         /* 0x6f1bc5, the player's own goods slot */
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue) /* the options screens' 4-argument call (rt.h does the same for its sv_ row) */
void sv_ButtonRow(int f, int y, int *out, int n, ...); /* 0x52e350, rt.h: a row of n buttons, ids in out[] */
void ai_ApplyVolumes(void);         /* 0x5d5f10, applies the sound option volumes to the mixer */
void ai_LoadScreenImage(char *path); /* 0x5d7320, the loading screen reader */

/* Callees whose units have no header yet (type debt: the prototype should live there). */
int ls_SaveGame(int name, char *path, unsigned flags); /* 0x5190b0, ls.c: the save writer; the
                                       definition names the two char * arguments name / path (swapped) */
int cm_GetSlotState(int h);         /* 0x4c3ae0, cm.c (as include/cm.h): the state of message slot h */
void sv_ReleaseNamePlates(void);    /* 0x52e190, sv.c: drops the contact plates (the objects lose their
                                       +0x224 hook, the plate table is cleared) */
int sv_RegisterNamePlate(char *name, char *text, int b); /* 0x52e1c0, sv.c: finds or creates the contact
                                       plate of the object called name (text shown, b the icon), returns
                                       the object or 0 */

void sv_OpenPlayerBar(int building, char k);  /* 0x525220, sv.c: opens the player bar form (Misc\PlayerBar) for the building */
void sv_ClosePlayerBar(void);       /* 0x525440, sv.c: closes the player bar again */
void sv_ClearSelection(void);       /* 0x52ae40, sv.c: clears the group selection (the focus and the selected flag of every character) */

/* The scene / contact values the listeners read. */
extern int ai_BuildingPerms;        /* 0x6f231c, the permission bits of the player in the building the
                                       scene focus is on (f3gm.c 0x503530): bit 0x200 / 0x400 offer
                                       the building's contact plates */
extern int ai_ClickedObject;        /* 0x6f22e4, the object clicked in the scene this frame (cm.h) */
/* Text-pointer slots of the contact plates (the text system fills them at run time). */





/* The plates of the production buildings (the listener 0x5d8420). */








/* The windows and game screens the contact listeners open. They are defined below the listeners (see
   the order note in the .c): their bodies must not be visible to the callers, or VC6 drops the calls. */
void ai_BloodRevengeWindow(int building); /* 0x5d85e0, the gypsy blood revenge window (no result: every exit ends in a call) */
void ai_HomageWindow(int building);      /* 0x5d95a0, the "Ehrerbietung" homage window (no result: every exit ends in a call) */
void ai_CurseWindow(int building);       /* 0x5d8c90, the curse window ("_WAHRSAGERIN_FLUCH_0%d"; no result: every exit ends in a call) */
void ai_CurseTargetWindow(int building); /* 0x5d90e0, the curse victim / share picker (no result: every exit ends in a call) */
int ai_PokerGame(void);                  /* 0x5da030, the poker game window */

/* The save / load windows and the three option screens of the options menu 0x5d7770. Their bodies are
   also defined below their caller (see the order note in the .c): VC6 drops a call whose result is
   unused to a side-effect-free body it has already seen, so the caller must not see them. */
int ai_SaveOptionWindow(int image);             /* 0x5d45d0, the save window (the image to restore) */
int ai_SaveNameWindow(int image);             /* 0x5d4920, the load window (the image to restore) */
void ai_OptionsGfxScreen(void);          /* 0x5d5fd0, "menu\options_gfx" */
void ai_OptionsSfxScreen(void);          /* 0x5d6580, "menu\options_sfx" */
void ai_OptionsGameScreen(void);         /* 0x5d6950, "menu\options_game" */

/* The two text slots of the tailor contact prompt 0x5d8360 (the text system fills them at run time;
   they sit behind contact_PLAUSCH's own slot 0x144aa58). */



/* ---- the options menu 0x5d7770 (ai_OptionsMenu) ---- */
extern int d2_LastWindow;           /* 0x6ed938, d2.c: the window created last (the current form the
                                       text / control calls work in; d2.c sets it, every screen reads it) */
extern unsigned char snd_AmbientOnByte; /* 0x6f0b60, the `on` byte of snd_AmbientOn
                                       (f3snd.h: G16AMBIENTON; that unit has no header here) */
extern int ai_GameOver;             /* 0x6f259c, cm.h: nonzero once the game is over / left */
extern int ai_LeaveGame;            /* 0x6f25a0, set with ai_GameOver when the menu leaves the game (the
                                       load window / the game loop end); main.c clears it when a game
                                       starts (0x537de0) and when the game loop ends */
#ifndef DATA_VIEWS_H
extern unsigned char sv_MenuOpen;   /* 0x6f25ae, 1 while a menu screen runs (0x5d7770 sets it around its
                                       pump); sv.c's frame loop 0x5309a0 caller skips the hotkeys while it
                                       is set */
#endif
extern int d2_PointerObj;           /* 0x5fe710, rt.h: the object the pointer clicked (-1: none) */
extern int d2_ClickedId;            /* 0xf8a668, d2.h / rt.h: the id the pointer click selected
                                       (0 / 1 = the button answers, -1: no click yet) */

/* The text-pointer slots of the menu buttons (the text system fills them at run time; main.c passes the
   same slots to its own menu buttons). */








/* The callees of the options menu whose units have no header (type debt, the prototype belongs there). */
int d2_OpenForm(int x, int y, char *name);  /* 0x4741f0, d2.h: loads the view, returns its number */
void d2_SetObjEnabled(int obj, int on);     /* 0x475e40, d2.h: enables / disables one control */
void d2_SetViewEnabled(int view, int on);   /* 0x475590, d2.c: enables / disables the objects of a view */
void d2_PlaceViewWindows(int view, unsigned char flags); /* 0x475870, d2.c: places the view's windows
                                       by the flags of 0x4756b0 (1 = centre x, 2 = centre y) */
void d2_PlaceMainWindow(int view); /* 0x475850, d2.c: places the view's first window (callers that pass the flags too: d2_PlaceMainWindowF in their head) */
int d2_CreateRadioGroup(int n, ...);        /* 0x46c490, d2.h: the radio group of n control ids */
void d2_RadioClear(int g);                  /* 0x46c600, d2.h: frees a radio group */
void d2_MenuNavigate(int g);                /* 0x46c6c0, d2.h: keyboard / pointer navigation in a group */
void sv_SetScreenCenter(unsigned fl);       /* 0x530900, sv.c: sets the screen centre from the pump flags */
void sv_FitForm(int f);                     /* 0x52ea30, sv.c: fits the form to its background picture */
void sv_ResetScreenClip(void);              /* 0x520f20, sv.c: resets the screen clip and the scene state */
void sv_PopTo(int v);                       /* 0x52fb50, sv.c: pops the screen stack to the record whose
                                       first word is v (sets sv_PopRequest to its distance) */
int cm_SendPlayerId(void);                  /* 0x4c3f90, cm.c: sends message 0x13 with the local player's
                                       id, returns the message slot handle (-1: none) */
void unk_ClearEndGameGate(void);            /* 0x576020, unk_575860.c: clears the end-of-game gate */
void unk_SetEndGameGate(void);              /* 0x576030, unk_575860.c: sets the end-of-game gate */
void main_GameLoop(int ctl);                /* 0x53bb20, main.c: the main game loop / dialog callback */
void txt_Format(char *out, int id0, ...);   /* 0x5176d0, io_text.c: formats a text id into out */

#endif
