struct SBSAMPLE;
struct SBBANK;
struct D3OBJECT;
struct D3VECTOR;
/* Declarations of f3snd.c (0x4c01b0..): sound queue and ambient sounds (tag "f3snd: snd_AddToQueue") (the gusts "Boehe_Leicht_s" / "_Mittel_s" /
   "_Stark_s" in its .data). Names are ours. */
#ifndef F3SND_H
#define F3SND_H

typedef struct G16SMPENTRY {            /* SBENTRY in sblib.c: the file entry of a sample */
    int offset;                         /* 0x00, file offset of the data */
    char name[0x32];                    /* 0x04, "Fanfare", "MausclickLinks", ... */
} G16SMPENTRY;

typedef struct G16SMP {                 /* SBSAMPLE in sblib.c; the fields the sound queue reads */
    int handle;                         /* 0x00 */
    G16SMPENTRY *info;                  /* 0x04, NULL = unused */
} G16SMP;

typedef struct G16SND {                 /* 0x10 bytes ("f3snd: snd_AddToQueue"), a queued sound */
    union {
        char *smp;                      /* 0x00, SBSAMPLE *, as the sb_* calls take it */
        G16SMP *sample;                 /* 0x00, the same record's fields (the queue's view) */
    };
    unsigned int delay;                 /* 0x04, time units (d3_Time * 13) after the previous one */
    int playing;                        /* 0x08 */
    struct G16SND *next;                /* 0x0c */
} G16SND;

typedef struct G16SEASON {              /* 0xc bytes, snd_Gusts[season]: the gusts */
    float chance;                       /* 0x00 */
    float vol;                          /* 0x04, 0..1, the gust volume (scaled by 127) */
    unsigned int interval;              /* 0x08, time units */
} G16SEASON;

typedef struct G16AMB {                 /* 0x14 bytes, an animal sound of a season */
    float chance;                       /* 0x00 */
    float vol;                          /* 0x04 */
    int count;                          /* 0x08, variants */
    unsigned int interval;              /* 0x0c */
    unsigned int last;                  /* 0x10 */
} G16AMB;

typedef struct G16LOC {                 /* 0x114 bytes, snd_LocationTracks[]: a location track */
    int kinds[4];                       /* 0x00, building kinds (0 ends; 0x2694: outdoors); kinds[0] 0 ends the table */
    char track[0x100];                  /* 0x10 */
    short prio;                         /* 0x110 */
    char active;                        /* 0x112 */
    char pad113;
} G16LOC;

typedef struct G16TRACKS {              /* 0x24c bytes, ai_Unk602d70[]: tracks by kind / room / season */
    unsigned int kind;                  /* 0x00, 0 ends */
    unsigned int room;                  /* 0x04, 0: any */
    int season;                         /* 0x08, -1: any */
    char names[6][0x60];                /* 0x0c */
} G16TRACKS;

#pragma pack(push, 1)
typedef struct G16BLDVIEW {             /* the market building as f3snd reads it (AIBLD in ai.h, which f3snd
                                           cannot include: its AITIME ai_Time view differs) */
    char pad61[0x61];
    char *object;                       /* 0x61, its 3D object in the scene */
} G16BLDVIEW;
#pragma pack(pop)

typedef struct G16TRACKHANDLES {        /* 0x8 bytes, snd_TrackHandles: the music track and a copy of it,
                                           always written together (0x4c1bd0, 0x4c18a0) */
    char *current;                      /* 0x00, the music track (MSXTRACK *) */
    char *copy;                         /* 0x04, a copy of current, never read */
} G16TRACKHANDLES;

#include "f3snd_types.h"             /* G16AMBIENTON */

typedef struct G16CHARCLICK {           /* the click-comment view of the start of a character record (0x770b80) */
    unsigned short id;                  /* 0x00, the character id */
    unsigned char kind;                 /* 0x02, the building kind (0x14: none) */
} G16CHARCLICK;

typedef struct G16CHAR {                /* 0x21c bytes, ai_CharInts[0x300] (AICHAR, ai.h) */
    unsigned int unk00;                 /* 0x00 */
    char pad04;
    char sex;                           /* 0x05, 1: woman */
    char pad06[0x216];
} G16CHAR;

extern G16SEASON snd_Gusts[];           /* 0x602ba8, by season */
extern float snd_VoiceVolume;           /* 0x602b08, voice volume (1.0, scaled by 127) */
extern float snd_MusicVolume;           /* 0x602d68, music volume 0..1: 0 = no music (0x4c23f0, 0x4c2000) */
extern unsigned int ai_Unk60b0d8;       /* 0x60b0d8, start of the music pause */
extern unsigned char ai_GameState;      /* 0x6f25ac, the game state (as cm.h / unk_5d33b0.h) */
extern int snd_MusicEnabled;            /* 0x6f2578, the "msx" setting of game.INI [Sound]: music enabled */
extern int snd_SfxEnabled;              /* 0x6f257c, the "sfx" setting of game.INI [Sound]: effects and voices on */
extern int ai_Unk6f0cb4;                /* 0x6f0cb4, the season of the music */
extern unsigned int ai_Unk6f0cb0;       /* 0x6f0cb0, the pause length */
extern unsigned int ai_Unk6f0cb8;       /* 0x6f0cb8, time of the last track decision */
extern unsigned char *ai_Unk6f0ca8;     /* 0x6f0ca8, the building of the track */
extern int ai_Unk6f0cac;                /* 0x6f0cac, the room of the track */
extern char *snd_SpecialTrack;          /* 0x6f0b74, the special track of 0x4c23f0 (MSXTRACK *) */
extern int snd_AmbientSaved;            /* 0x6f0b78, 1 while the ambient flag is off and saved (0x4c23f0) */
extern unsigned int snd_AmbientOnSaved; /* 0x6f0b64, the saved snd_AmbientOn.on */
extern char ai_Unk6f0860[];             /* 0x6f0860, a debug text */
extern char ch_EmptyName[];             /* 0x631090, "" */
extern char ai_Chars[768][0x21c];        /* 0x770b80, the characters (ahm.c); f3snd.c reads them flat (ai_CharsC) */
extern int ai_Unk7603a0[32];            /* 0x7603a0, the selected workers (their id is the first word; as cm.h) */
extern int snd_VoiceBankOrders;         /* 0x6f0cbc, voice bank: orders (snd_LoadBuildingVoices) */
extern int snd_VoiceBankClick;          /* 0x6f0cc0, voice bank: click comments */
extern int snd_VoiceBankNoise;          /* 0x6f0cc4, voice bank: click noises */
extern int snd_VoiceBankGreeting;       /* 0x6f0ccc, voice bank: greeting */
extern G16LOC snd_LocationTracks[];           /* 0x6094d0 */
extern G16TRACKS ai_Unk602d70[];        /* 0x602d70 */
extern char *ai_FallbackBuilding;        /* 0x6f230c, the second current building, used when 0x6f2308 is 0 (sv.c defines char *) */
extern unsigned short ai_PlayerId;      /* 0x6f25b8, the player (as am.h / cm.h / gv.h declare it) */
extern G16TRACKHANDLES snd_TrackHandles; /* 0x6f0b6c, the music track and its copy */
extern char ai_Unk6f0a60[];             /* 0x6f0a60, a debug text */
extern char snd_TrackText[];             /* 0x6f0760, a debug text */
extern char snd_LastTrack[];             /* 0x6f0ba8, the outdoor track played last */
extern G16AMB ai_Unk602bd8[][5];        /* 0x602bd8, per season: birds, crickets, chickens, horses, dogs */
extern char ai_Unk6f0960[];             /* 0x6f0960, a debug text */
extern int ai_Unk6f0b94;                /* 0x6f0b94, hpLocustsPerm */
extern int ai_Unk6f0b98;                /* 0x6f0b98, hpLocusts */
extern int ai_Unk6f0b9c;                /* 0x6f0b9c, the season */
extern int snd_AmbientTrack;                /* 0x6f0b80, the d3snd handle of the ambient track (bird song, market ambience) */
extern struct AIBLD *ai_MarketBuilding;        /* 0x6f1bc8, the market building (ai.c; f3snd.c reads it through ai_MarketBuildingV) */
extern char *ai_PlayerBuilding;          /* 0x6f2308, the building the player is in: an AIBLD (sv.c defines char *; ai_PlayerBuildingT views) */
extern unsigned short * sv_PanelShown;             /* 0x6f2310, its room */
extern void *ai_Unk6f0ba0;              /* 0x6f0ba0, the atmosphere mp3 (MSXTRACK *) */
extern char *ai_BTypes;                 /* 0x7e3f30, ai.h: AIBTYPE * (0x24d bytes, the kind at 0) */
extern int d3_CurrentScene;             /* 0x633444 */        /* 0x602ba8 */
extern char snd_GustNames[3][0x32];     /* 0x602b10, "Boehe_Leicht_s", "Boehe_Mittel_s", "Boehe_Stark_s" */
extern void *d3_Terrain;                /* 0x633738 */
extern G16AMBIENTON snd_AmbientOn;      /* 0x6f0b60, ambient sounds on */
extern int snd_Season;                  /* 0x6f0b8c, the season */
extern unsigned int snd_LastGust;       /* 0x6f0b90, time of the last gust */
extern char ai_Time[];                /* 0x770ae0, AITIME (ai.h) */
extern char snd_TimeCopy[14];           /* 0xf81240, a copy of ai_Time */
extern char *ai_Unkfc2dec;              /* 0xfc2dec, the camera object */
extern int *d3t_Rain;               /* 0x760618, the rain level */
extern int snd_Bank;                    /* 0x6f2404, the sample bank */
extern int snd_RainDrops;               /* 0xf81254, the "Tropfen_s" loop */
extern int snd_RainLightNear;           /* 0xf8125c, "LeichtNah_l_01" */
extern int snd_RainLightFar;            /* 0xf81274, "LeichtFern_l_01" */
extern int snd_RainMediumNear;          /* 0xf81264, "MittelNah_l_01" */
extern int snd_RainMediumFar;           /* 0xf8127c, "MittelFern_l_01" */
extern int snd_RainHeavyNear;           /* 0xf8126c, "StarkNah_l_01" */
extern int snd_RainHeavyFar;            /* 0xf81284, "StarkFern_l_01" */
extern int snd_FanfareFade;             /* 0x6f0b7c, 1 while a "Fanfare" has faded the music out */
extern G16SND *snd_Queue;               /* 0x6f0b84, the queue (the first queued sound) */
extern int snd_QueuePaused;             /* 0x6f0b68, nonzero: the queue does not advance */
extern int snd_QueueTime;               /* 0x6f0b88, time of the last queue change */
extern volatile unsigned long d3_Time;  /* 0x6333cc */

void sb_SetRestart(struct SBSAMPLE *s, int on);                                  /* 0x426170, sblib.c (this header's units: sb_SetRestartC) */
int sb_IsPlaying(struct SBSAMPLE *s);                                                     /* 0x426f20 */
int sb_Play(struct SBSAMPLE *s, int fade, int delay);                                     /* 0x427110 */
int gm_ClockSeason(void *t);                                                       /* 0x4fe350, the season */
float d3t_GetMeanHeight(struct D3VECTOR *pos);                                              /* 0x412550, d3_tools.c */
struct SBSAMPLE *sb_PlaySample(struct SBBANK *bank, char *name, int loops, int pan, int volume, int fade, int delay); /* 0x426fa0, sblib.c (this header's units: sb_PlaySampleI) */
void sb_SetVolume(struct SBSAMPLE *s, int volume);                               /* 0x427330, sblib.c (f3snd.c: sb_SetVolumeI) */
void sb_Stop(struct SBSAMPLE *s, int fade);                                                      /* 0x427290 */
void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out);                               /* 0x458a20 */
int ch_CountAt(int scene, int all, struct D3VECTOR *pos, float eps);                               /* 0x402720 */
int ch_Count(int scene, int a);                                                  /* 0x4026c0 */
void d3snd_SetVolume(struct D3SOUND *s, int volume);                                    /* 0x417e00, d3_snd.c (f3snd.c: d3snd_SetVolumeI) */
void msx_SetTrackVolume(struct MSXTRACK *t, int vol);                                       /* 0x423930, msx.h: MSXTRACK * */
struct MSXTRACK * msx_PlayAmbient(char *name, int vol);                                      /* 0x4243f0 */
void msx_StopAmbient(struct MSXTRACK *t);                                                   /* 0x4244b0 */
unsigned char * gm_BuildQuery(int n, ...);                                           /* 0x501190 */
int util_Random(unsigned short n);                                               /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, ai_Unk4d88c0 ..) call util_RandomU, a cast in their head */
struct MSXTRACK * msx_Play(char *name, int once);                                            /* 0x423a90, msx.h: MSXTRACK * */
float util_RandomF(void);                                                        /* 0x42ec20 */
void msx_Stop(struct MSXTRACK *t, int fade);                                                /* 0x423df0 */
void msx_FadeMaster(float vol, int ms);                                          /* 0x4241a0 */
int sb_IsVariation(struct SBBANK *bank, char *name);                            /* 0x426f60, sblib.c (f3snd.c: sb_IsVariationI) */
struct SBSAMPLE *sb_GetVariation(struct SBBANK *bank, char *name, int idx);       /* 0x426190, sblib.c (f3snd.c: sb_GetVariationI) */
struct SBSAMPLE * sb_GetSample(struct SBBANK *bank, char *name);                                          /* 0x426030 */
struct SBBANK * sb_LoadSampleBank(char *name, int preload);                                        /* 0x426950 */
int sb_UnloadSampleBank(struct SBBANK *bank);                                                 /* 0x426ba0 */
float gm_Liking(unsigned a, unsigned b, int c);                                       /* 0x50c550, a favour */
int gm_BuildingGoodsList(char *b, unsigned *out, int kind);                            /* 0x5098a0 */
int gm_Unk5099e0(char *b, unsigned *out, int kind);                            /* 0x5099e0 */
struct D3SOUND *d3snd_Attach(struct SNDHANDLE *snd, struct D3OBJECT *obj, int volume, float range);   /* 0x417710, d3_snd.c (f3snd.c: d3snd_AttachI) */
void d3snd_SetHold(struct D3SOUND *s, int hold);                                        /* 0x417dd0, d3_snd.c (f3snd.c: d3snd_SetHoldI) */
void d3snd_Play(struct D3SOUND *s, int a, int loops, int delay);                                     /* 0x417cf0 */
void d3snd_Stop(struct D3SOUND *s, int unk);                                            /* 0x417db0, d3_snd.c (f3snd.c: d3snd_StopI) */
void d3snd_Detach(struct D3SOUND *s);                                                   /* 0x4177b0, d3_snd.c (f3snd.c: d3snd_DetachI) */
void * mem_Alloc(int size, char *tag);                                            /* 0x4223c0, mem.c */
void mem_Free(void *p);                                                          /* 0x422620, mem.c */

void snd_ClearQueue(void);
G16SND *snd_QueueTail(void);
int snd_AddToQueue(char *smp, unsigned int delay);
void snd_UpdateQueue(void);
void snd_UpdateRain(void);
void ai_Unk4c0e60(void);
void snd_EnterBuilding(void);
void snd_PlayOutdoorTrack(void);
int snd_FindActiveTrack(void);
int snd_FindTrackKind(int kind);
void ai_Unk4c2000(void);
void snd_PlaySpecialTrack(char *name);
void snd_EndSpecialTrack(void);
int snd_VoiceVariant(unsigned int voice, int woman);
int snd_FindVoiceSample(int id, int bank, char *text, int var);
int snd_PlayVoice(int id, int bank, char *text, int var);
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay);
int snd_LoadVoiceBank(char *name, int a);
void snd_LoadBuildingVoices(unsigned char *b);
void snd_UnloadBuildingVoices(void);
void snd_PlayWorkerClick(G16CHARCLICK *c);
void snd_PlaySelectedClick(void);
void snd_PlayFavourComment(void);
void snd_StartMarketAmbience(void);
void snd_StopMarketAmbience(void);
void snd_FadeMusic(float v, int ms);

#endif
