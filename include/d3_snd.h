/* Declarations of d3_snd.c (0x417650..0x418ce0): sounds attached to 3D objects ("d3snd_Init") and the water
   ambience ("d3sndw_Init"). D3SOUND (0x54 bytes) and the SNDHANDLE view are d3_snd.c's types. Names are ours. */
#ifndef D3_SND_H
#define D3_SND_H

struct D3SOUND;
struct SNDHANDLE;
struct D3OBJECT;

int d3snd_Init(int max);                                                         /* 0x417650 */
void d3snd_Exit(void);                                                           /* 0x4176f0, frees the sound slot array d3snd_Init allocated and clears it */
struct D3SOUND *d3snd_Attach(struct SNDHANDLE *snd, struct D3OBJECT *obj, int volume, float range);   /* 0x417710, attaches an opened sound to obj */
void d3snd_Detach(struct D3SOUND *s);                                                   /* 0x4177b0, stops the slot's sound, marks it stopped and unlinks it from the slot */
struct D3SOUND *d3snd_Alloc(void);                                                      /* 0x417800, the first free slot of d3snd_List (obj == NULL), NULL if none */
void d3snd_Update(void);                                                         /* 0x417840, once per frame */
void d3snd_DetachAll(void);                                                      /* 0x417990, detaches every used sound slot */
void d3snd_Free(struct D3SOUND *s);                                                     /* 0x4179d0 */
void d3snd_UpdatePan(struct D3SOUND *s);                                                /* 0x4179f0, pan from the angle between the camera's z axis and the direction to */
void d3snd_UpdateVolume(struct D3SOUND *s);                                             /* 0x417b10, volume from the distance to the camera */
void d3snd_Play(struct D3SOUND *s, int a, int loops, int delay);                        /* 0x417cf0, plays the slot's sound with loops/delay (sb_Resume when the slot is */
void d3snd_Stop(struct D3SOUND *s, int unk);                                            /* 0x417db0, stops the slot's sound with the given fade */
void d3snd_SetHold(struct D3SOUND *s, int hold);                                        /* 0x417dd0, sets the hold flag: a held slot restarts instead of being unlinked */
void d3snd_SetVolume(struct D3SOUND *s, int volume);                                    /* 0x417e00, sets the slot's volume at distance 0 */
int d3snd_IsPlaying(struct D3SOUND *s);                                                 /* 0x417e10, 1 while the slot's sound plays */
int d3snd_PlayAt(struct SNDHANDLE *snd, struct D3OBJECT *obj, int volume, float range);        /* 0x417e30, plays snd once at obj's position without attaching it */
int d3sndw_Init(unsigned short n);                                               /* 0x417ec0 */
void d3sndw_Exit(void);                                                          /* 0x418030, frees the water-ambience ray arrays allocated by d3sndw_Init */
struct D3SOUND *d3sndw_GetSound(int type, int *slot);                                   /* 0x418080, the sound slot for water type */
void d3sndw_Update(void);                                                        /* 0x4183d0, once per frame: the water ambience (up to two looping sounds */

extern int d3snd_Max;                                        /* 0x5f38a0 */
extern float d3sndw_Range[];                                 /* 0x5f38a4 */
extern unsigned int d3sndw_Vol[];                            /* 0x5f38bc */
extern char d3sndw_Names[6][25];                             /* 0x5f38d8 */
#endif
