/* sndc.c -- sound script commands (LoadSampleBank, PlaySample, ...) and their registration, original
   file name unknown. EVTBUILTIN functions: every argument is passed as a pointer to its value. Starts at
   0x40ffc0 (right after the object-command registration 0x40f960) and ends with the registration 0x410220.
   Names are ours (sndc_ + the script command name). */
#ifndef UNK_40FFC0_H
#define UNK_40FFC0_H

struct D3OBJECT;
struct SNDHANDLE;
#include "evt.h"

struct SBBANK;
struct SBSAMPLE;

/* sblib.c (no header there) */
struct SBSAMPLE * sb_GetSample(struct SBBANK *bank, char *name);                         /* 0x426030 */
struct SBBANK *sb_FindBank(char *name);                                                 /* 0x426290 */
struct SBBANK * sb_LoadSampleBank(char *name, int preload);                              /* 0x426950 */

/* script globals SND_W2 ... SND_VAR registered by sndc_Register */
extern int snd_W2;                      /* 0x5f3108 (see .c) */
extern int snd_W1;                      /* 0x5f310c */
extern int snd_M3;                      /* 0x5f3110 */
extern int snd_M2;                      /* 0x5f3114 */
extern int snd_M1;                      /* 0x5f3118 */
extern int snd_HS;                      /* 0x5f311c */
extern int snd_NONE;                    /* 0x5f3120 */
extern int snd_VAR;                     /* 0x5f3124 */

struct SBBANK *sndc_LoadSampleBank(char **name);                                        /* 0x40ffc0 */
int sndc_KillSampleBank(struct SBBANK **bank);                                          /* 0x40ffe0 */
struct SBSAMPLE *sndc_PlaySampleHandle(struct SBBANK **bank, char **name);              /* 0x410000 */
struct SBSAMPLE *sndc_PlaySample(char **name, int *loops);                              /* 0x410040 */
struct SBSAMPLE *sndc_PlaySample3D(struct D3OBJECT **obj, int *range, char **name, int *loops);   /* 0x4100b0 */
int sndc_StopSample(struct SBSAMPLE **s);                                               /* 0x410130 */
struct SBBANK *sndc_GetSampleBankHandle(char **name);                                   /* 0x410150 */
struct SBSAMPLE *sndc_GetSampleHandle(struct SBBANK **bank, char **name);               /* 0x410160 */
int sndc_SpeechQueued(int *id, int *bank, char **name, int *var, int *delay);           /* 0x410180 */
void sndc_WaitForSample(struct SBSAMPLE **s);                                           /* 0x4101d0 */
void sndc_Register(void);                                                               /* 0x410220 */

#endif
