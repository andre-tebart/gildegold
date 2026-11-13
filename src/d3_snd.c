/* d3_snd.c -- original file name unknown; the allocation tags are "d3snd_Init" and "d3sndw_Init".
   Sounds attached to 3D objects: volume falls off with the distance to the camera, pan follows the
   direction. Extent in GildeGold.exe (inferred, not proven): 0x417650..0x418ce0; 0x417ec0.. is the
   water ambience ("d3sndw") and may be a file of its own.
   Functions are kept in their original order. Names are ours.
   Open: not included yet (dcp headers conflicts d3_snd HEADER): d3_fl.h; held by own declarations that differ
   from the owner's: map_Region.
   unit of rea 8cef7d5b (src/game/d3_snd.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_snd.h"
#define sb_SetRestartC ((void (*)(char *, int))sb_SetRestart)  /* 0x426170 (sblib.c: SBSAMPLE *): this unit's char * view of the sample */

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

#include "sblib.h"
#include "d3_engine.h"
#include "d3_tools.h"
#include "d3_vec.h"
#include "mem.h"
#include "d3_pose.h"
#define sb_PlayH ((void (*)(SNDHANDLE *, int, int))sb_Play)  /* 0x427110 (sblib.c: int (SBSAMPLE *, int, int)): @original d3_snd.c sees a SNDHANDLE pointer and void */
#define sb_SetPanH ((void (*)(SNDHANDLE *, int))sb_SetPan)  /* 0x4272f0 (sblib.c: SBSAMPLE *): this unit's SNDHANDLE view of the sample, 0..127 */
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */


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


static D3SOUND *d3snd_List = 0;          /* 0x631130 */
int d3snd_Max = 10;  /* 0x5f38a0, initialized to 10; TODO define here once .data is matched */

/* 0x417650, allocates the sound slot array (d3snd_Max D3SOUNDs, 10 by default) and clears it; `max`
   overrides the count when nonzero. 0 ok, -1 when the array exists already or the allocation fails */
int d3snd_Init(int max)
{
    int ret = 0;

    if (!d3snd_List) {
        if (max)
            d3snd_Max = max;
        d3snd_List = mem_Alloc(d3snd_Max * sizeof(D3SOUND), "d3snd_Init");
        if (d3snd_List) {
            D3_ZERO_AT(d3snd_List, d3snd_Max * sizeof(D3SOUND));
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x4176f0, frees the sound slot array d3snd_Init allocated and clears it */

void d3snd_Exit(void)
{
    if (d3snd_List)
        mem_Free(d3snd_List);
    d3snd_List = NULL;
}

D3SOUND *d3snd_Alloc(void);                                             /* 0x417800 */

/* 0x417710, attaches an opened sound to obj */
D3SOUND *d3snd_Attach(SNDHANDLE *snd, D3OBJECT *obj, int volume, float range)
{
    D3SOUND *s = NULL;

    if (snd && obj && !obj->sound) {
        s = d3snd_Alloc();
        if (s && snd->entry) {
            sb_SetRestartC((char *)snd, 1);
            strncpy(s->name, snd->entry->name, 0x32);
            s->obj = obj;
            obj->sound = s;
            s->snd = snd;
            s->volume = volume;
            s->range = range * D3SND_INCH;
            s->hold = 1;
            d3snd_UpdateVolume(s);
            d3snd_UpdatePan(s);
            s->hold = 0;
            s->idle = 0;
        }
    }
    return s;
}
/* 0x4177b0, stops the slot's sound, marks it stopped and unlinks it from the slot and its object */

void d3snd_Detach(D3SOUND *s)
{
    if (s->snd) {
        sb_Stop(s->snd, 0);
        s->snd->flags |= 8;
        sb_SetRestartC((char *)s->snd, 0);
        s->snd = NULL;
    }
    if (s->obj) {
        s->obj->sound = NULL;
        s->obj = NULL;
    }
}
/* 0x417800, the first free slot of d3snd_List (obj == NULL), NULL if none */

D3SOUND *d3snd_Alloc(void)
{
    int i = 0;
    D3SOUND *s = NULL;

    if (d3snd_List) {
        for (; i < d3snd_Max; i++)
            if (!d3snd_List[i].obj) {
                s = &d3snd_List[i];
                break;
            }
    }
    return s;
}
/* 0x417840, once per frame */

void d3snd_Update(void)
{
    int i;

    if (!d3snd_List)
        return;
    for (i = 0; i < d3snd_Max; i++) {
        if (!d3snd_List[i].obj)
            continue;
        if (d3snd_List[i].obj->scene != d3_DefaultScene) {
            if (d3snd_List[i].snd) {
                sb_Stop(d3snd_List[i].snd, 0);
                d3snd_List[i].snd->flags |= 8;
                if (!d3snd_List[i].hold) {
                    sb_SetRestartC((char *)d3snd_List[i].snd, 0);
                    d3snd_List[i].snd = NULL;
                }
            }
        } else {
            d3snd_UpdateVolume(&d3snd_List[i]);
            d3snd_UpdatePan(&d3snd_List[i]);
            if (d3snd_List[i].snd && !sb_IsPlaying(d3snd_List[i].snd)) {
                d3snd_List[i].idle++;
                if (d3snd_List[i].idle > 500 && !d3snd_List[i].hold) {
                    if (d3snd_List[i].loops)
                        d3snd_Detach(&d3snd_List[i]);
                    else {
                        sb_SetRestartC((char *)d3snd_List[i].snd, 0);
                        d3snd_List[i].snd->flags |= 8;
                        d3snd_List[i].snd = NULL;
                    }
                }
            }
        }
    }
}
/* 0x417990, detaches every used sound slot */

void d3snd_DetachAll(void)
{
    int i;

    for (i = 0; i < d3snd_Max; i++)
        if (d3snd_List[i].obj)
            d3snd_Detach(&d3snd_List[i]);
}
/* 0x4179d0, detaches a sound slot from its sample and object (NULL is ignored) */

void d3snd_Free(D3SOUND *s)
{
    if (s)
        d3snd_Detach(s);
}
/* 0x4179f0, pan from the angle between the camera's z axis and the direction to the object */

void d3snd_UpdatePan(D3SOUND *s)
{
    D3VECTOR4 pos;
    D3VECTOR4 dir;
    D3VECTOR4 rel;
    D3MATRIX m;

    if (s && s->obj && s->snd) {
        d3_MatTranspose(&d3_Camera->matrix, &m);
        dir.x = d3_ZAxis.x * m._11 + d3_ZAxis.y * m._21 + d3_ZAxis.z * m._31;
        dir.y = d3_ZAxis.x * m._12 + d3_ZAxis.y * m._22 + d3_ZAxis.z * m._32;
        dir.z = d3_ZAxis.x * m._13 + d3_ZAxis.y * m._23 + d3_ZAxis.z * m._33;
        d3_PointToWorld(s->obj, &s->obj->pos, (D3VECTOR *)&pos);
        rel.x = pos.x - d3_Camera->pos.x;
        rel.y = pos.y - d3_Camera->pos.y;
        rel.z = pos.z - d3_Camera->pos.z;
        sb_SetPanH(s->snd, (int)(sin(d3_VecAngle((D3VECTOR *)&rel, (D3VECTOR *)&dir)) * 63.5 + 63.0));
    }
}
/* 0x417b10, volume from the distance to the camera; opens the sound when it gets audible */

void d3snd_UpdateVolume(D3SOUND *s)
{
    D3VECTOR4 pos;
    float dx, dy, dz, dist, t;
    int lin, quad, vol;

    if (!s || !s->obj)
        return;
    d3_PointToWorld(s->obj, &s->obj->pos, (D3VECTOR *)&pos);
    dx = pos.x - d3_Camera->pos.x;
    dy = pos.y - d3_Camera->pos.y;
    dz = pos.z - d3_Camera->pos.z;
    dist = sqrt(dx * dx + dy * dy + dz * dz);
    dist = dist * D3SND_INCH;
    t = s->range - dist;
    if (t > 0.0f) {
        lin = (int)(t / s->range * s->volume);
        quad = (int)(s->volume - s->volume * dist * dist / (s->range * s->range));
        vol = (int)((quad + (lin > 0 ? lin : 0)) * 0.5f);
    } else
        vol = 0;
    if (vol > 5) {
        if (s->snd) {
            sb_SetVolume(s->snd, vol);
            if (sb_IsPlaying(s->snd) || s->loops)
                return;
            s->idle = 0;
            if (s->delay)
                s->snd->flags &= ~8;
            else
                d3snd_Play(s, 0, 0, 0);
        } else {
            s->snd = (SNDHANDLE *)sb_GetSample(0, s->name);
            if (!s->snd)
                return;
            sb_SetRestartC((char *)s->snd, 1);
            s->snd->loops = s->loops;
            s->snd->delay = s->delay;
            sb_SetVolume(s->snd, vol);
            sb_SetLoops((struct SBSAMPLE *)s->snd,s->loops);
            d3snd_Play(s, 0, s->loops, s->delay);
        }
    } else if (s->snd) {
        sb_SetVolume(s->snd, vol);
        sb_Stop(s->snd, 0);
        if (!s->hold) {
            sb_SetRestartC((char *)s->snd, 0);
            s->snd = NULL;
        }
    }
}
/* 0x417cf0, plays the slot's sound with loops/delay (sb_Resume when the slot is held); resets idle */

void d3snd_Play(D3SOUND *s, int a, int loops, int delay)
{
    char buf[256];

    if (!s || !s->snd)
        return;
    if (!s->snd->entry) {
        sprintf(buf, "Entry-Handle missing for %s", s->name);
        sprintf(buf, "3dSound-Handle is attached to %s", s->obj->name);
        return;
    }
    s->snd->loops = loops;
    s->loops = loops;
    sb_SetLoops((struct SBSAMPLE *)s->snd,loops);
    s->delay = delay;
    if (!s->hold)
        sb_PlayH(s->snd, a, delay);
    else
        sb_Resume((struct SBSAMPLE *)s->snd);
    s->idle = 0;
}
/* 0x417db0, stops the slot's sound with the given fade */

void d3snd_Stop(D3SOUND *s, int unk)
{
    if (s && s->snd)
        sb_Stop(s->snd, unk);
}
/* 0x417dd0, sets the hold flag: a held slot restarts instead of being unlinked when its sound stops; a new
   hold first starts the sound */

void d3snd_SetHold(D3SOUND *s, int hold)
{
    if (hold)
        d3snd_Play(s, 0, 0, 0);
    s->hold = hold;
}
/* 0x417e00, sets the slot's volume at distance 0 */

void d3snd_SetVolume(D3SOUND *s, int volume)
{
    s->volume = volume;
}
/* 0x417e10, 1 while the slot's sound plays */

int d3snd_IsPlaying(D3SOUND *s)
{
    if (s && s->snd)
        return sb_IsPlaying(s->snd);
    return 0;
}
/* 0x417e30, plays snd once at obj's position without attaching it */

int d3snd_PlayAt(SNDHANDLE *snd, D3OBJECT *obj, int volume, float range)
{
    D3SOUND s;

    if (snd && obj) {
        s.range = range * D3SND_INCH;
        s.obj = obj;
        s.loops = 1;
        s.snd = snd;
        snd->loops = 1;
        snd->flags = 0;
        s.volume = volume;
        d3snd_UpdatePan(&s);
        d3snd_UpdateVolume(&s);
        if (snd->volume) {
            sb_PlayH(snd, 0, 0);
            return 1;
        }
    }
    return 0;
}

/* Water ambience ("d3sndw"): rays are cast around the camera to find the nearest water of each type;
   up to two water sounds play at once in d3sndw_Snd. */
static float *d3sndw_Sin = 0;            /* 0x631134, sin of each ray angle */
static int *d3sndw_Type = 0;             /* 0x631138, water type hit by each ray */
static int *d3sndw_State = 0;            /* 0x63113c, per ray: 0 done, 1 searching, 2 hit, 3 ... */
static unsigned short d3sndw_N = 0;      /* 0x631140, number of rays */
static D3SOUND d3sndw_Snd[2] = {0};        /* 0x631148 */

/* 0x417ec0, allocates the water-ambience sine, type and state arrays and fills the sine table */
int d3sndw_Init(unsigned short n)
{
    unsigned short i;
    float a, step;

    if (n > 4) {
        d3sndw_N = n;
        d3sndw_Sin = mem_Alloc(n * sizeof(float), "d3sndw_Init");
        if (d3sndw_Sin) {
            d3sndw_Type = mem_Alloc(n * sizeof(int), "d3sndw_Init");
            if (d3sndw_Type) {
                D3_ZERO_AT(d3sndw_Type, n * sizeof(int));
                d3sndw_State = mem_Alloc(n * sizeof(int), "d3sndw_Init");
                if (d3sndw_State) {
                    a = 0.0f;
                    step = 6.2831855f / n;
                    D3_ZERO_AT(d3sndw_State, n * sizeof(int));
                    for (i = 0; i < n; i++) {
                        d3sndw_Sin[i] = sin(a);
                        a += step;
                    }
                    return 1;
                }
                mem_Free(d3sndw_Type);
                d3sndw_Type = NULL;
                mem_Free(d3sndw_Sin);
                d3sndw_Type = NULL;
                d3sndw_Sin = NULL;
                return 0;
            }
            mem_Free(d3sndw_Sin);
            d3sndw_Sin = NULL;
        }
    }
    return 0;
}
/* 0x418030, frees the water-ambience ray arrays allocated by d3sndw_Init */

void d3sndw_Exit(void)
{
    if (d3sndw_Sin) {
        mem_Free(d3sndw_Sin);
        d3sndw_Sin = NULL;
        mem_Free(d3sndw_Type);
        d3sndw_Type = NULL;
        mem_Free(d3sndw_State);
        d3sndw_State = NULL;
        d3sndw_Type = NULL;
        d3sndw_Sin = NULL;
    }
}

extern char d3sndw_Names[6][25];  /* 0x5f38d8, the definition is below (declaration order = .data order) */
/* 0x418080, the sound slot for water type; *slot: -1 any, 0 only slot 1, 1 only slot 0 (the slot not
   taken yet); returns the slot's sound and sets *slot to it, or NULL */
D3SOUND *d3sndw_GetSound(int type, int *slot)
{
    D3SOUND *s = NULL;

    switch (*slot) {
    case -1:
        if (!d3snd_IsPlaying(&d3sndw_Snd[0])) {
            s = &d3sndw_Snd[0];
            if (d3sndw_Snd[0].snd)
                sb_SetRestartC((char *)d3sndw_Snd[0].snd, 0);
            d3sndw_Snd[0].snd = (SNDHANDLE *)sb_GetSample(0, d3sndw_Names[type]);
            if (d3sndw_Snd[0].snd && d3sndw_Snd[0].snd->entry) {
                sb_SetRestartC((char *)d3sndw_Snd[0].snd, 1);
                strncpy(d3sndw_Snd[0].name, d3sndw_Snd[0].snd->entry->name, 0x32);
                *slot = 0;
            } else
                s = NULL;
        } else if (!strcmp(d3sndw_Snd[0].name, d3sndw_Names[type])) {
            s = &d3sndw_Snd[0];
            *slot = 0;
        } else if (!d3snd_IsPlaying(&d3sndw_Snd[1])) {
            s = &d3sndw_Snd[1];
            if (d3sndw_Snd[1].snd)
                sb_SetRestartC((char *)d3sndw_Snd[1].snd, 0);
            d3sndw_Snd[1].snd = (SNDHANDLE *)sb_GetSample(0, d3sndw_Names[type]);
            if (d3sndw_Snd[1].snd && d3sndw_Snd[1].snd->entry) {
                sb_SetRestartC((char *)d3sndw_Snd[1].snd, 1);
                strncpy(d3sndw_Snd[1].name, d3sndw_Snd[1].snd->entry->name, 0x32);
                *slot = 1;
            } else
                s = NULL;
        } else if (!strcmp(d3sndw_Snd[1].name, d3sndw_Names[type])) {
            s = &d3sndw_Snd[1];
            *slot = 1;
        }
        break;
    case 0:
        if (!d3snd_IsPlaying(&d3sndw_Snd[1])) {
            s = &d3sndw_Snd[1];
            if (d3sndw_Snd[1].snd)
                sb_SetRestartC((char *)d3sndw_Snd[1].snd, 0);
            d3sndw_Snd[1].snd = (SNDHANDLE *)sb_GetSample(0, d3sndw_Names[type]);
            if (d3sndw_Snd[1].snd && d3sndw_Snd[1].snd->entry) {
                sb_SetRestartC((char *)d3sndw_Snd[1].snd, 1);
                strncpy(d3sndw_Snd[1].name, d3sndw_Snd[1].snd->entry->name, 0x32);
                *slot = 1;
            } else
                s = NULL;
        } else if (!strcmp(d3sndw_Snd[1].name, d3sndw_Names[type])) {
            s = &d3sndw_Snd[1];
            *slot = 1;
        }
        break;
    case 1:
        if (!d3snd_IsPlaying(&d3sndw_Snd[0])) {
            s = &d3sndw_Snd[0];
            if (d3sndw_Snd[0].snd)
                sb_SetRestartC((char *)d3sndw_Snd[0].snd, 0);
            d3sndw_Snd[0].snd = (SNDHANDLE *)sb_GetSample(0, d3sndw_Names[type]);
            if (!d3sndw_Snd[0].snd || !d3sndw_Snd[0].snd->entry) {
                s = NULL;
                *slot = 0;
            } else {
                sb_SetRestartC((char *)d3sndw_Snd[0].snd, 1);
                strncpy(d3sndw_Snd[0].name, d3sndw_Snd[0].snd->entry->name, 0x32);
                *slot = 0;
            }
        } else if (!strcmp(d3sndw_Snd[0].name, d3sndw_Names[type]))
            s = &d3sndw_Snd[0];
        break;
    }
    return s;
}

/* Terrain map (d3_Terrain), as far as d3sndw_Update uses it */
typedef struct D3MAPREGION {        /* 0x158 bytes */
    char pad000[0x154];
    int water;                      /* 0x154, water type, 0 = none */
} D3MAPREGION;

typedef struct D3MAP {
    char pad0000[0x19e0];
    D3MAPREGION *regions;           /* 0x19e0 */
} D3MAP;

char map_Pick(struct D3FLMAP *map, struct D3VECTOR *pos, char lod, int *x, int *y, float *h);          /* 0x4463b0 */
int map_Region(D3MAP *map, int x, int y);                                              /* 0x446910, index into regions or < 0 */

extern unsigned char snd_AmbientOn;     /* 0x6f0b60, 1: ambient sounds on */
extern float ai_FloatZero;              /* 0x5eb788, the shared 0.0f (cm.c): the k * k of the camera's own region */
float d3sndw_Range[] = { 0.0f, 30.0f, 25.0f, 20.0f, 15.0f, 10.0f };  /* 0x5f38a4, per water type, in meters */
unsigned int d3sndw_Vol[] = { 0, 70, 70, 70, 70, 60, 0 };  /* 0x5f38bc, per water type */
extern char d3sndw_Names[6][25];  /* 0x5f38d8, defined below (declaration order = section order) */
char d3sndw_Names[6][25] = { "", "FliesstSchnell_l", "FliesstMittel_l", "FliesstLangsam_l", "Platsch_s", "Platsch_s" };  /* 0x5f38d8, sample per water type; TODO define here once .data is matched */
static unsigned long d3sndw_LastSplash = 0;     /* 0x6311f0 */
static D3SCENE *d3sndw_Scene = 0;               /* 0x6311f4 */
/* d3_Time * 13, spelled so that the volatile is read three times */
#define D3SNDW_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

typedef struct D3SNDWHIT {
    int type;
    int vol;
    int pan;
} D3SNDWHIT;

/* 0x4183d0, once per frame: the water ambience (up to two looping sounds, d3sndw_Snd[]). Takes the map region
   under the camera (region water kind 1..3) and probes ahead along the view direction for more water regions
   to pick volume and pan of the sounds; both sounds are stopped when the scene changed or the camera / map /
   sound system is not ready. The stop code is written out in both exits, as the original had it: VC6 merges
   the two copies (cross-jumping), which is where the first block's store before its add esp comes from */
void d3sndw_Update(void)
{
    float dist;
    int slot;
    int nfound;
    int k;
    int ground;
    D3VECTOR4 campos;
    D3SNDWHIT found[2];
    D3VECTOR4 probe;
    int gy, gy2;
    char unk[4];
    int gx2, gx;
    D3VECTOR4 dir;
    D3VECTOR4 rot, rel;
    D3MATRIX m;
    float a, sum;
    int i, j, n, idx, type, cnt;
    unsigned long t;
    D3SOUND *s;

    if (!d3_Terrain || !d3sndw_Sin || !d3_Camera || !snd_AmbientOn) {
        if (d3sndw_Snd[0].snd) {
            sb_Stop(d3sndw_Snd[0].snd, 0);
            sb_SetRestartC((char *)d3sndw_Snd[0].snd, 0);
            d3sndw_Snd[0].snd = NULL;
        }
        if (d3sndw_Snd[1].snd) {
            sb_Stop(d3sndw_Snd[1].snd, 0);
            sb_SetRestartC((char *)d3sndw_Snd[1].snd, 0);
            d3sndw_Snd[1].snd = NULL;
        }
        return;
    }
    if (!d3sndw_Scene)
        d3sndw_Scene = d3_DefaultScene;
    else if (d3sndw_Scene != d3_DefaultScene) {
        d3sndw_Scene = d3_DefaultScene;
        if (d3sndw_Snd[0].snd) {
            sb_Stop(d3sndw_Snd[0].snd, 0);
            sb_SetRestartC((char *)d3sndw_Snd[0].snd, 0);
            d3sndw_Snd[0].snd = NULL;
        }
        if (d3sndw_Snd[1].snd) {
            sb_Stop(d3sndw_Snd[1].snd, 0);
            sb_SetRestartC((char *)d3sndw_Snd[1].snd, 0);
            d3sndw_Snd[1].snd = NULL;
        }
        return;
    }
    campos.x = d3_Camera->pos.x;
    campos.y = d3_Camera->pos.y;
    campos.z = d3_Camera->pos.z;
    ground = (int)(d3_Camera->pos.y - d3t_GetMeanHeight(&d3_Camera->pos));
    if (ground > 2000)
        ground = 2000;
    if (!map_Pick(d3_Terrain, (D3VECTOR *)&campos, 1, &gx, &gy, unk))
        return;
    nfound = 0;
    dist = 39.37f;
    found[0].type = 0;
    /* @original: found[0].type is cleared by itself and the rest by an inline memset (its own zero
       register, the one dir.y = 0 reuses) */
    memset(&found[0].vol, 0, sizeof(found) - sizeof(found[0].type));
    d3_MatTranspose(&d3_Camera->matrix, &m);
    dir.x = d3_ZAxis.x * m._11 + d3_ZAxis.y * m._21 + d3_ZAxis.z * m._31;
    dir.z = d3_ZAxis.x * m._13 + d3_ZAxis.y * m._23 + d3_ZAxis.z * m._33;
    dir.y = 0.0f;
    d3_Normalize(&dir);
    idx = map_Region(d3_Terrain, gx, gy);
    if (idx < 0 || (type = ((D3MAP *)d3_Terrain)->regions[idx].water) > 3 || type == 0) {
        for (i = 0; i < d3sndw_N; i++)
            d3sndw_State[i] = 1;
    } else {
        found[0].type = type;
        found[0].vol = (int)((d3sndw_Vol[type] - (d3sndw_Vol[type] * ai_FloatZero) / (d3sndw_Range[type] * d3sndw_Range[type]))
                             * (2000 - ground) * 0.0005f);
        found[0].pan = 63;
        nfound = 1;
        for (i = 0; i < d3sndw_N; i++)
            d3sndw_State[i] = 3;
    }
    for (k = 1; k < d3sndw_Range[1] && nfound < 2; k++) {
        probe.x = dir.x * dist + campos.x;
        probe.y = dir.y * dist + campos.y;
        probe.z = dir.z * dist + campos.z;
        for (i = 0; i < d3sndw_N; i++) {
            if (d3sndw_State[i]) {
                if (map_Pick(d3_Terrain, (D3VECTOR *)&probe, 0, &gx2, &gy2, unk)) {
                    idx = map_Region(d3_Terrain, gx2, gy2);
                    if (idx >= 0)
                        d3sndw_Type[i] = ((D3MAP *)d3_Terrain)->regions[idx].water;
                    else
                        d3sndw_Type[i] = 0;
                    if (d3sndw_Type[i]) {
                        if (d3sndw_State[i] == 1)
                            d3sndw_State[i] = 2;
                        else if (d3sndw_State[i] == 2)
                            d3sndw_State[i] = 3;
                    } else
                        d3sndw_State[i] = 1;
                } else
                    d3sndw_State[i] = 0;
            }
            /* turn the probe around the camera to the next ray */
            rel.x = probe.x - campos.x;
            rel.y = probe.y - campos.y;
            rel.z = probe.z - campos.z;
            a = 6.2831855f / d3sndw_N;
            rot.x = rel.x * cos(a) + rel.z * sin(a);
            rot.y = rel.y;
            rot.z = rel.z * cos(a) - rel.x * sin(a);
            probe.x = rot.x + campos.x;
            probe.y = rot.y + campos.y;
            probe.z = rot.z + campos.z;
        }
        for (j = 0; j < d3sndw_N && nfound < 2; j++) {
            if (d3sndw_State[j] == 2 && k <= d3sndw_Range[d3sndw_Type[j]]) {
                /* merge all rays that hit the same water type */
                cnt = 1;
                sum = d3sndw_Sin[j];
                for (n = 0; n < d3sndw_N; n++) {
                    if (n != j && d3sndw_Type[j] == d3sndw_Type[n]) {
                        d3sndw_Type[n] = 0;
                        cnt++;
                        sum += d3sndw_Sin[n];
                    }
                }
                type = d3sndw_Type[j];
                if (type < 4) {
                    found[nfound].type = type;
                    found[nfound].vol = (int)((d3sndw_Vol[type] - d3sndw_Vol[type] * k * k
                                               / (d3sndw_Range[type] * d3sndw_Range[type]))
                                              * (2000 - ground) * 0.0005f);
                    found[nfound].pan = (int)((sum / cnt + 1.0f) * 63.0f);
                    nfound++;
                } else {
                    /* splashes: at most one per 60..120 s */
                    t = D3SNDW_TIME;
                    t -= d3sndw_LastSplash;
                    if (t > 60000 && t < 120000) {
                        sb_PlaySampleI(0, d3sndw_Names[type], 1,
                                      (int)((sum / cnt + 1.0f) * 63.0f),
                                      (int)((d3sndw_Vol[type] - d3sndw_Vol[type] * k * k
                                             / (d3sndw_Range[type] * d3sndw_Range[type]))
                                            * (2000 - ground) * 0.0005f), 0, 0);
                        d3sndw_LastSplash = D3SNDW_TIME;
                    } else if (t > 120000)
                        d3sndw_LastSplash = D3SNDW_TIME;
                }
            }
        }
        dist += 39.37;
    }
    if (nfound == 2 && found[0].type == found[1].type && abs(found[0].pan - found[1].pan) < 31)
        nfound = 1;
    slot = -1;
    switch (nfound) {
    case 0:
        if (d3sndw_Snd[0].snd)
            sb_Stop(d3sndw_Snd[0].snd, 0);
        if (d3sndw_Snd[1].snd)
            sb_Stop(d3sndw_Snd[1].snd, 0);
        return;
    case 1:
        s = d3sndw_GetSound(found[0].type, &slot);
        if (s) {
            if (s->snd) {
                sb_SetVolume(s->snd, found[0].vol);
                sb_SetPanH(s->snd, found[0].pan);
            }
            if (!d3snd_IsPlaying(s))
                d3snd_Play(s, 0, 0, 0);
        }
        if (d3sndw_Snd[slot == 0].snd)
            sb_Stop(d3sndw_Snd[slot == 0].snd, 0);
    case 2:
        slot = -1;
        for (i = 0; i < nfound; i++) {
            s = d3sndw_GetSound(found[i].type, &slot);
            if (s) {
                if (s->snd) {
                    sb_SetVolume(s->snd, found[i].vol);
                    sb_SetPanH(s->snd, found[i].pan);
                }
                if (!d3snd_IsPlaying(s))
                    d3snd_Play(s, 0, 0, 0);
            }
        }
    }
    return;
}
