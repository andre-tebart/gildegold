/* d3_snd.c -- original file name unknown; the allocation tags are "d3snd_Init" and "d3sndw_Init".
   Sounds attached to 3D objects: volume falls off with the distance to the camera, pan follows the
   direction. Extent in GildeGold.exe (inferred, not proven): 0x417650..0x418ce0; 0x417ec0.. is the
   water ambience ("d3sndw") and may be a file of its own.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_snd.c); boundaries in decl/text.dcl, notes in src/d3_snd/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"

/* Sound library (0x426030..0x427390), file not identified yet. */
typedef struct SNDENTRY {
    int unk00;
    char name[0x32];                /* 0x04 */
} SNDENTRY;

typedef struct SNDHANDLE {
    int unk00;
    SNDENTRY *entry;                /* 0x04 */
    char pad08[0x4];
    int loops;                      /* 0x0c, copy of D3SOUND.loops */
    char pad10[0x4];
    unsigned int flags;             /* 0x14, bit 3 set when the sound is let go */
    int delay;                      /* copy of D3SOUND.delay */
    char pad1c[0x8];
    int volume;                     /* 0x24, SBSAMPLE.volume: set by sb_SetVolume, 0 = inaudible */
} SNDHANDLE;

struct SBBANK;
struct SBSAMPLE;
struct SBSAMPLE * sb_GetSample(struct SBBANK *bank, char *name);                       /* 0x426030, sblib.c (sndc.h's view) */
void sb_SetRestart(char *smp, int on);                                                  /* 0x426170, sblib.c: SBSAMPLE *; 1 after open, 0 to let go (f3snd.h's view) */
int sb_IsPlaying(struct SBSAMPLE *s);                                                      /* 0x426f20 */
void sb_Resume(struct SBSAMPLE *s);                                                     /* 0x427010, sblib.c */
void sb_Play(SNDHANDLE *snd, int a, int b);                                            /* 0x427110 */  /* @original: d3_snd.c sees a SNDHANDLE pointer and void, sblib.c's definition an SBSAMPLE pointer and int; both are pointers in the original */
void sb_Stop(struct SBSAMPLE *s, int fade);                                                 /* 0x427290 */
void sb_SetPan(SNDHANDLE *snd, int pan);                                               /* 0x4272f0, 0..127 */
void sb_SetVolume(struct SBSAMPLE *s, int volume);                                            /* 0x427330 */  /* @original: d3_snd.c passes the SBSAMPLE*, f3snd.h the int handle view of the same original call */
void sb_SetLoops(struct SBSAMPLE *s, int loops);                                        /* 0x427390, sblib.c (sndc.h's view) */

void d3_MatTranspose(D3MATRIX *src, D3MATRIX *dst);                                        /* 0x454040 */
float d3_VecAngle(D3VECTOR *a, D3VECTOR *b);                                            /* 0x4539f0 */
extern D3VECTOR d3_ZAxis;                   /* 0x5ebb74, (0, 0, 1) */
float d3t_GetMeanHeight(struct D3VECTOR *pos);                                                 /* 0x412550, d3_tools.c */

#define D3SND_INCH (1.0 / 39.37)            /* the engine works in inches */

typedef struct D3SOUND {            /* 0x54 bytes, d3snd_Max of them at d3snd_List */
    char name[0x34];                /* 0x00, sample name */
    SNDHANDLE *snd;                 /* 0x34 */
    D3OBJECT *obj;                  /* 0x38, NULL = free slot */
    int idle;                       /* 0x3c, updates since the sound stopped */
    int volume;                     /* 0x40, at distance 0 */
    int loops;                      /* 0x44, loop count: copied to SBSAMPLE.loops (0x0c) and passed to sb_SetLoops */
    float range;                    /* 0x48, in meters; silent beyond */
    int delay;
    int hold;                       /* 0x50 */
} D3SOUND;

void d3snd_UpdatePan(D3SOUND *s);                                       /* 0x4179f0 */
void d3snd_UpdateVolume(D3SOUND *s);                                    /* 0x417b10 */
void d3snd_Play(struct D3SOUND *s, int a, int loops, int delay);               /* 0x417cf0 */

static D3SOUND *d3snd_List = 0;          /* 0x631130 */
int d3snd_Max = 10;  /* 0x5f38a0, initialized to 10; TODO define here once .data is matched */

