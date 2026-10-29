/* unk_5d33b0.h -- the screen / front-end module (unnamed), see src/game/unk_5d33b0.c and
   notes/units/unk_5d33b0.md */
#ifndef UNK_5D33B0_H
#define UNK_5D33B0_H

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
extern unsigned char ai_GameFlags;  /* 0x6f23f8, the game mode flags: bit 2 = network game (ai.h, cm.h) */
extern unsigned int cm_SpeedLevel;  /* 0x60bd84, the shared speed level 0..4 of a network game (cm.h) */
extern float cm_SpeedFactor;        /* 0x5f1290, the game speed factor (cm.h) */
extern int ai_CameraStep;           /* 0x623814, the camera move step, 100 - camera speed; sv.c converts
                                       it with fild at the float uses (the original loads an int) */

extern char ai_DefaultEffect[];     /* 0x62de60, the default effect record: an empty name, the scale
                                       1.0, the lifetime 25.0, then the entry's ids / callbacks */
extern int ai_DefaultEffectScaleBits; /* 0x62de6c, the scale of ai_DefaultEffect (a float 1.0) read as
                                       its bit pattern: 0x5dbc20 passes it through an int K&R view */

/* The five-float record the sphere volume 0x5dc1a0 fills: radius, size, the two metrics derived from
   the size and the reach volume * radius. am.c passes the same record to 0x5dc210 as int *in. */
typedef struct SPHEREVOLUME {
    float radius;       /* +0x00 */
    float size;         /* +0x04 */
    float surface;      /* +0x08, 2 pi size^2 */
    float volume;       /* +0x0c, 4/3 pi size^3 */
    float reach;        /* +0x10, volume * radius */
} SPHEREVOLUME;
extern int ai_PokerWindow;          /* 0x62dda0, the view of the open poker window, -1 when closed */
/* Two entries of the run-time text table (the .data slots hold char * into the loaded text file; the
   contents are not in the exe). ai_Quicksave shows them when the network quicksave is refused. */


/* The 0x0f network message the quicksave sends: cm_SendExSysMessage copies 0x1f dwords (0x7c B) of it
   into the message data. +0 = the save slot (1 = quicksave, the value the local path passes to
   ls_SaveGame as its third argument), +4 = the save name. */
typedef struct CMQUICKSAVE {
    int slot;                           /* +0x00 */
    char name[0x78];                    /* +0x04, "QUICKSAVE" */
} CMQUICKSAVE;
extern unsigned char ai_GameState; /* 0x6f25ac, the game state: 0 loading / paused, 1 running, 2 ... (as cm.h / f3snd.h) */
extern unsigned char ls_SaveSlot;   /* 0x6f1c38, the save slot the next save records: 0 autosave, 1 quicksave,
                                       the network save slot (as ls.c, which writes it into the save header) */
extern char ch_EmptyName[];         /* 0x631090, the empty status / character name (f3init.c, f3gm.c) */
extern unsigned short ai_ScreenImage[]; /* 0x766320, the 0x78 x 0xa0 16-bit screen image (as ls.c) */

void ai_ApplyOptions(void);         /* 0x5d5e90, applies the option values to the run time */
void ai_SaveOptions(void);          /* 0x5d4c70, writes the options to the INI */
int ai_ApplyGfxOptions(void);     /* 0x5d5c40, applies the graphics options (the shadow setup) */
void ai_LoadOptions(void);          /* 0x5d5640, reads the options from the INI */
extern int snd_SystemBank;          /* 0x6f2400, main.c: the handle of the system sample bank (system.sbf) */
extern int main_ScreenRestart;     /* 0x6f25a4, main.c: nonzero = re-initialise the screen (the resolution changed) */
extern char dd_Has1024x768;         /* 0x631226, dd_vesa.h */
extern char dd_Has1152x864;         /* 0x631227, dd_vesa.h */
void d2_SetLabel(int id, char *s);                         /* 0x46bc70, d2.h: sets the widget's label */
void d2_SetList(int id, int n, ...);                       /* 0x46bca0, d2.h: sets the widget's "a|b|c|" item list */
void d2_SetSliderFlags(int id, int flags);                 /* 0x46bdd0, d2.h: sets the slider's flags */
void d2_SetObjValue();                                     /* 0x475cb0, d2.h (K&R: d2.c takes a 5th parameter, the call sites push 2, 4 or 5) */
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue) /* the options screens' 4-argument call (rt.h does the same for its sv_ row) */
void sv_ButtonRow(int f, int y, int *out, int n, ...); /* 0x52e350, rt.h: a row of n buttons, ids in out[] */
void ai_ApplyVolumes(void);         /* 0x5d5f10, applies the sound option volumes to the mixer */
void ai_LoadScreenImage(char *path); /* 0x5d7320, the loading screen reader */

/* Callees whose units have no header yet (type debt: the prototype should live there). */
int ls_SaveGame(char *path, char *name, unsigned flags); /* 0x5190b0, ls.c: the save writer; the
                                       definition names the two char * arguments name / path (swapped) */
int cm_GetSlotState(int h);         /* 0x4c3ae0, cm.c (as include/cm.h): the state of message slot h */
void sv_ReleaseNamePlates(void);    /* 0x52e190, sv.c: drops the contact plates (the objects lose their
                                       +0x224 hook, the plate table is cleared) */
int sv_RegisterNamePlate(char *name, char *text, int b); /* 0x52e1c0, sv.c: finds or creates the contact
                                       plate of the object called name (text shown, b the icon), returns
                                       the object or 0 */

/* The scene / contact values the listeners read. */
extern int ai_BuildingPerms;        /* 0x6f231c, the permission bits of the player in the building the
                                       scene focus is on (f3gm.c 0x503530): bit 0x200 / 0x400 offer
                                       the building's contact plates */
extern int ai_ClickedObject;        /* 0x6f22e4, the object clicked in the scene this frame (cm.h) */
/* Text-pointer slots of the contact plates (the text system fills them at run time). */





/* The plates of the production buildings (the listener 0x5d8420). */








/* The windows and game screens the contact listeners open. They are defined below the listeners (see
   the order note in the .c): their bodies must not be visible to the callers, or VC6 drops the calls. */
int ai_BloodRevengeWindow(int building); /* 0x5d85e0, the gypsy blood revenge window */
int ai_HomageWindow(int building);       /* 0x5d95a0, the "Ehrerbietung" homage window */
int ai_CurseWindow(int building);        /* 0x5d8c90, the curse window ("_WAHRSAGERIN_FLUCH_0%d") */
int ai_CurseTargetWindow(int building);  /* 0x5d90e0, the curse victim / share picker */
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
void d2_PlaceMainWindow(int view, int fl); /* 0x475850, d2.c: places the view's first window */
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
