/* msx.c (D:\develop\units\Msx\msx.c): streamed music and ambient tracks on top of the sound library.
   Extent in GildeGold.exe: 0x423770..0x424510 (0x423770 is the update thread, not a Ghidra function).
   Names are ours. */
#ifndef MSX_H
#define MSX_H

typedef struct MSXTRACK {           /* 0x128 bytes, MSX_TRACKS of them at msx_Tracks */
    char name[0x100];               /* file name relative to msx_Path; "" = free slot */
    int stream;                     /* 0x100, sound library stream handle */
    char playing;                   /* 0x104 */
    char queued;                    /* 0x105, starts when the playing track has faded out */
    char bAmbient;                  /* 0x106, played on the ambient driver */
    char pad107;
    int length;                     /* 0x108, ms */
    int pos;                        /* 0x10c, ms */
    int fadepos;                    /* 0x110, pos when the fade started */
    int fademode;                   /* 0x114, 1 = fade in, 2 = fade out */
    unsigned int volume;            /* 0x118, 0..127 */
    unsigned int curvol;            /* 0x11c */
    int fadevol;                    /* 0x120, curvol when the fade started */
    struct MSXTRACK *next;          /* 0x124, track queued after this one */
} MSXTRACK;

#define MSX_TRACKS 10

/* sound library (0x42b780..0x42c3a0), file not identified yet */
int __stdcall AIL_get_timer_highest_delay(void);                                        /* 0x42c3a0, mss32 thunk: the import library's symbol is _AIL_get_timer_highest_delay@0 */

extern MSXTRACK msx_Tracks[MSX_TRACKS];     /* 0x6325d0 */
extern MSXTRACK msx_Dummy;                  /* 0x14612a0, returned when there is no driver */
extern CRITICAL_SECTION msx_Lock;           /* 0x14613e0 */
extern int msx_Driver;                      /* 0x6325c4, music */
extern int msx_AmbDriver;                   /* 0x6325c8, ambient */
extern int msx_Quit;                        /* 0x6325c0, ends the update thread */
extern LARGE_INTEGER msx_Freq;              /* 0x6323a8 */
extern int msx_DelayPeak;                   /* 0x6325bc */
extern char msx_Msg[0x100];                 /* 0x6323b0, debug message (not printed) */
extern char msx_VolMsg[0x100];              /* 0x6324b0 */
extern char msx_Path[0x100];                /* 0x14611a0 */
extern float msx_Fade;                      /* 0x5f74a8, master volume factor 0..1 */
extern float msx_FadeFrom;                  /* 0x5f74ac */
extern float msx_FadeTo;                    /* 0x5f74b0, < 0 = no master fade running */
extern unsigned int msx_FadeDuration;       /* 0x5f74b4 */
extern int msx_Volume;                      /* 0x5f74b8 */
extern int msx_AmbVolume;                   /* 0x5f74bc */
extern int msx_FadeTime;                    /* 0x5f74c0, track fade time */
extern unsigned int msx_FadeStart;          /* 0x6325b0 */

int msx_Init(int a, int b, int c);                                                      /* 0x423830 */
void msx_Exit(void);                                                                    /* 0x4238e0 */
void msx_SetTrackVolume(MSXTRACK *t, int vol);                                          /* 0x423930 */
void msx_SetVolume(int vol);                                                            /* 0x4239a0 */
void msx_SetAmbVolume(int vol);                                                         /* 0x423a50 */
MSXTRACK *msx_Play(char *name, int once);                                               /* 0x423a90 */
int msx_Start(MSXTRACK *t, int once);                                                   /* 0x423b30 */
void msx_Stop(MSXTRACK *t, int fade);                                                   /* 0x423df0 */
void msx_SetFadeTime(int ms);                                                           /* 0x423e70 */
void msx_Update(void);                                                                  /* 0x423e80 */
void msx_FadeMaster(float vol, int ms);                                                 /* 0x4241a0 */
MSXTRACK *msx_Find(char *name);                                                         /* 0x424250 */
MSXTRACK *msx_GetPlaying(void);                                                         /* 0x4242c0 */
MSXTRACK *msx_Alloc(void);                                                              /* 0x424300 */
void msx_SetFade(MSXTRACK *t, int mode);                                                /* 0x424370 */
void msx_SetPath(char *path);                                                           /* 0x4243d0 */
MSXTRACK *msx_PlayAmbient(char *name, int vol);                                         /* 0x4243f0 */
void msx_StopAmbient(MSXTRACK *t);                                                      /* 0x4244b0 */

#endif
