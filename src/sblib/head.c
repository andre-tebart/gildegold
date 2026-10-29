/* sblib.c -- original file name unknown ("sb_Init", "sblib successfully initialzied").
   Sample/sound library on top of the sound driver wrappers: sample banks (files with a 0x144-byte
   header and 0x40-byte entries) loaded on demand into a memory budget, and a fixed set of playing
   sample slots. Extent in GildeGold.exe (inferred): 0x425bc0..0x4273d0 (0x4273d0 starts the script
   interpreter). Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/sblib.c); boundaries in decl/text.dcl, notes in src/sblib/open.md */
struct SBSAMPLE;
struct SBBANK;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "sfx.h"

typedef struct SBDATA {             /* a loaded sample: 12-byte header, then the file data */
    char fmt;                       /* 1 wav, 2 mp3 */
    int len;                        /* 0x04 */
    char *data;                     /* 0x08 */
} SBDATA;

typedef struct SBVAR {              /* a loaded variation: header, count SBDATAs, then their data */
    int count;
    SBDATA *items;                  /* 0x04 */
    int len;                        /* 0x08 */
} SBVAR;

typedef struct SBENTRY {            /* 0x40 bytes */
    int offset;                     /* file offset of the data */
    char name[50];                  /* 0x04 */
    char type;                      /* 0x36, 1 sample (SBDATA), 2 variation (SBVAR) */
    void *data;                     /* 0x38, NULL = not loaded */
    int used;                       /* 0x3c, time of last use */
} SBENTRY;

typedef struct SBBANK {             /* 0x144 bytes, also the file header */
    char name[50];
    char path[256];                 /* 0x32 */
    int count;                      /* 0x134 */
    SBENTRY *entries;               /* 0x138 */
    struct SBBANK *next;            /* 0x13c */
    int refs;                       /* 0x140 */
} SBBANK;

typedef struct SBSAMPLE {           /* 0x30 bytes, sb_Count of them at sb_Samples */
    int handle;
    SBENTRY *info;                  /* 0x04, NULL = unused */
    int start;                      /* 0x08 */
    int loops;                      /* 0x0c */
    unsigned int fadestart;         /* 0x10 */
    union {
        unsigned int flags;         /* 0x14, 1 fade in, 2 fade out, 4 fixed variation, 8 stopped, 0x10 restart */
        volatile unsigned int volatileFlags;    /* the same dword through a volatile store, where VC6 must keep
                                                   two flag stores (sb_Update: stopped + fade bits) */
    };
    int delay;                      /* 0x18, repeat delay */
    unsigned int last;              /* 0x1c */
    int var;                        /* 0x20, variation index */
    unsigned int volume;            /* 0x24 */
    int pan;                        /* 0x28 */
    void (*done)(struct SBSAMPLE *s);   /* 0x2c */
} SBSAMPLE;


struct SBSAMPLE * sb_GetSample(struct SBBANK *bank, char *name);                                       /* 0x426030 */
SBENTRY *sb_FindEntry(SBBANK *bank, char *name);                                        /* 0x4260c0 */
SBSAMPLE *sb_FindSampleOf(SBENTRY *e, int *idx);                                        /* 0x4262f0 */
SBENTRY *sb_FindOldestEntry(void);                                                      /* 0x426350 */
SBBANK *sb_GetBankOf(SBENTRY *e);                                                       /* 0x426450 */
int sb_FreeMemory(int size);                                                            /* 0x4264a0 */
SBSAMPLE *sb_AllocSample(void);                                                         /* 0x426540 */
SBBANK *sb_LastBank(void);                                                              /* 0x426630 */
int sb_LoadEntry(SBBANK *bank, SBENTRY *e);                                             /* 0x426660 */
void sb_UnloadAll(void);                                                                /* 0x426db0 */
int sb_Play(struct SBSAMPLE *s, int fade, int delay);                                          /* 0x427110 */
void sb_Stop(struct SBSAMPLE *s, int fade);                                                    /* 0x427290 */

int sb_Mem = 0;  /* 0x633190 */
int sb_Used = 0;  /* 0x633194 */
SBBANK * sb_Banks = 0;  /* 0x633198 */
int sb_Driver = 0;  /* 0x63319c */
char sb_Path[256] = {0};  /* 0x6331a8 */
extern int sb_Count;
SBSAMPLE * sb_Samples = 0;  /* 0x6332a8 */
int sb_Count = 0;  /* 0x6332ac */
int sb_Stopping = 0;  /* 0x6332b0 */
unsigned int sb_FadeTime = 3000;  /* 0x5f8140 */
unsigned int sb_StopTime = 3000;  /* 0x5f8144 */
SBBANK sb_DummyBank = {0};  /* 0x14607c0 */
char sb_Msg[256] = {0};  /* 0x1460920 */
SBSAMPLE sb_Dummy = {0};  /* 0x1460a20 */
SBENTRY sb_DummyEntry = {0};  /* 0x1460a60 */

/* d3_Time * 13, spelled so that the volatile is read three times */
#define SB_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

