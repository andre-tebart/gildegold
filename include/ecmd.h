/* ecmd.c -- object script commands ("ecmd_KillObject: Could not find object to kill"), original file
   name unknown. Starts at 0x40d680 right after the character commands (chc.c) and ends with their
   registration at 0x40f960. EVTBUILTIN functions: every argument is passed as a pointer to its value.
   Names are ours (ocmd_ + the script name). */
#ifndef UNK_40D680_H
#define UNK_40D680_H

struct XOBJ;
struct D3VECTOR4;
struct D3OBJECT;
#include "evt.h"

/* context of the GetRnd*ObjectHandle object walks */
typedef struct OCMDFIND {
    D3OBJECT *objs[32];
    int n;                          /* 0x80 */
    char pattern[0x40];             /* 0x84 */
} OCMDFIND;

/* context of AttachAnimLoopedToAll / DetachAnimFromAll */
typedef struct OCMDANIM {
    char pattern[0x40];
    char file[0x80];                /* 0x40 */
    int n;                          /* 0xc0 */
    int rnd;                        /* 0xc4, start at a random frame */
    int unkc8;
} OCMDANIM;

/* ambient colour, d3_interface.c data */
extern float d3if_AmbR;                 /* 0x5fcc78 */
extern float d3if_AmbG;                 /* 0x5fcc7c */
extern float d3if_AmbB;                 /* 0x5fcc80 */

int mem_IsValid(void *p);                                                               /* 0x4225e0 */
int d3t_PlaceObject(struct D3OBJECT *camera, float px, float py, float pz, float dx, float dy, float dz, int unk, char flags);                /* 0x411680 */
char d3if_Delete(D3OBJECT *obj);                                                        /* 0x447190 */
char d3if_Reload(D3OBJECT *obj, char *name);                                            /* 0x447340 */
char d3_LoadScene(char *name, void *f, void (*progress)(int a, int i, int n));                                      /* 0x44be60 */
struct D3OBJECT * d3_LoadGroup(char *name, void *f, struct D3OBJECT *parent);                                 /* 0x44c3f0 */
void d3_WorldAngles(struct XOBJ *obj, struct D3VECTOR4 *angles);                                        /* 0x458910, world angles? */

int ocmd_GetObjectHandle(char **name);                                                  /* 0x40d680 */
char ocmd_FindCB(D3OBJECT *obj, OCMDFIND *f);                                           /* 0x40d6e0 */
int ocmd_GetRndObjectHandle(char **pattern);                                            /* 0x40d730 */
int ocmd_GetRndSubObjectHandle(D3OBJECT **parent, char **pattern);                      /* 0x40d7b0 */
int ocmd_GetSubObjectHandle(D3OBJECT **parent, char **name);                            /* 0x40d860 */
int ocmd_CreateObject(float *x, float *y, float *z, char **name);                       /* 0x40d8d0 */
int ocmd_CreateObjectAtDummy(char **name, D3OBJECT **dummy);                            /* 0x40d940 */
int ocmd_CreateObjectGroupAtDummy(char **name, D3OBJECT **dummy);                       /* 0x40d9a0 */
int ocmd_KillObject(D3OBJECT **obj);                                                    /* 0x40da10 */
int ocmd_SetPos(D3OBJECT **obj, float *x, float *y, float *z);                          /* 0x40da50 */
int ocmd_SetAngle(D3OBJECT **obj, float *x, float *y, float *z);                        /* 0x40dab0 */
int ocmd_MoveObject(D3OBJECT **obj, int *x, int *y, int *z, int *time);                 /* 0x40db30 */
int ocmd_RotateObject(D3OBJECT **obj, int *x, int *y, int *z, int *time);               /* 0x40dbb0 */
int ocmd_MoveObjectRelative(D3OBJECT **obj, int *x, int *y, int *z, int *time);         /* 0x40dc30 */
int ocmd_RotateObjectRelative(D3OBJECT **obj, int *x, int *y, int *z, int *time);       /* 0x40dcb0 */
int ocmd_SetAmbiente(int *r, int *g, int *b);                                           /* 0x40dd50 */
int ocmd_LightCalc(int *on);                                                            /* 0x40dd90 */
int ocmd_ReplaceObject(D3OBJECT **obj, char **name);                                    /* 0x40ddb0 */
int ocmd_LoadScene(char **name);                                                        /* 0x40ddf0 */
int ocmd_AttachAnim(D3OBJECT **obj, char **file, int *once);                            /* 0x40de30 */
int ocmd_PreloadAnim(D3OBJECT **obj, char **file);                                      /* 0x40dfc0 */
int ocmd_AttachAnimLooped(D3OBJECT **obj, char **file, int *n);                         /* 0x40e0e0 */
char ocmd_AttachAnimCB(D3OBJECT *obj, OCMDANIM *a);                                     /* 0x40e2b0 */
int ocmd_AttachAnimLoopedToAll(char **pattern, char **file, int *n, int *rnd);          /* 0x40e430 */
char ocmd_DetachAnimCB(D3OBJECT *obj, OCMDANIM *a);                                     /* 0x40e4c0 */
int ocmd_DetachAnimFromAll(char **pattern);                                             /* 0x40e4f0 */

/* ---- 0x40e560.. (worker ocmd2) ---- */

/* what the SelectAllTextureSets callback gets as context (0xcc bytes, on the callers stack) */
typedef struct OCMDTEXSEL {
    char name[0x40];                /* substring of the object name */
    char file[0x80];                /* 0x40, emptied before the walk (as OCMDANIM's file) */
    int n;                          /* 0xc0, 0, as OCMDANIM's n */
    int rnd;                        /* 0xc4, 0, as OCMDANIM's rnd */
    unsigned char set;              /* 0xc8, texture set to select */
} OCMDTEXSEL;

/* the bits of D3OBJECT.stateFlags the commands set */
typedef struct OCMDOBJBITS {
    unsigned char b0 : 1, b1 : 1, transient : 2, blocked : 1, b5 : 3;
} OCMDOBJBITS;

/* a particle system created by par_CreateEmitter (0x457b30); the commands fill in its parameters */
typedef struct D3EMITTER {
    char pad00[0x40];
    float amp[3];                   /* 0x40 */
    float amp3;                     /* 0x4c */
    float phase[3];                 /* 0x50 */
    float phase3;                   /* 0x5c */
    float size[3];                  /* 0x60 */
    float dir[3];                   /* 0x6c */
    float vel[3];                   /* 0x78 */
    float rnd[3];                   /* 0x84 */
    float acc[3];                   /* 0x90 */
    float plane[3];                 /* 0x9c */
    float plane3;                   /* 0xa8 */
    float maxtrigger;               /* 0xac, also set by SetEmitterFlags */
    float amp4;                     /* 0xb0 */
    float phase4;                   /* 0xb4 */
    float time;                     /* 0xb8 */
    int alpha[3];                   /* 0xbc */
    unsigned char b, g, r, a;       /* 0xc8 */
    unsigned char texmode : 5, initfill : 1, rebirthfill : 1, istrigger : 1;   /* 0xcc */
    unsigned char triggeronce : 1, trigger : 1, bcd : 6;                        /* 0xcd */
} D3EMITTER;

/* the template par_CreateEmitter reads (global at 0x1468620, 0xa4 bytes) */
typedef struct D3EMITTERCFG {
    int type;                       /* 0x00, emitter type: 0/1/2 -> par_EmitterCB0/1/2 */
    char pad04[0x30];
    float f34, f38, f3c;
    D3VECTOR dir;                   /* 0x40, d3_YAxis */
    char pad4c[0x34];
    float f80;
    char pad84[0x8];
    float f8c;
    int i90, i94, i98;
    unsigned char c9c, c9d, c9e, c9f;
    unsigned char flags;            /* 0xa0 */
    char pada1[3];
} D3EMITTERCFG;

typedef struct OCMDTEXFLAGS {       /* D3TEXFLAGS with the bits par_CreateEmitter's callers set */
    union {
        D3TEXFLAGS tex;             /* the value par_Create takes */
        struct {
            unsigned char b0, b1;
            unsigned char f0 : 1, f1 : 1, fr : 6;
            unsigned char b3;
        };
    };
} OCMDTEXFLAGS;

extern D3EMITTERCFG d3_EmitterCfg;      /* 0x1468620 */
extern const D3VECTOR d3_YAxis;         /* 0x5ebb94, (0, 1, 0) */

D3PARTSYS *par_CreateEmitter(int count, D3VECTOR *pos, int parent, char *tex, OCMDTEXFLAGS flags, float size,
                             D3EMITTERCFG *cfg);                                     /* 0x457b30 */
void par_Delete(D3PARTSYS *ps);                                                         /* 0x455760, d3_par.c */
struct D3OBJECT * d3t_FindObject(char *name, char flags);                                    /* 0x411900 */
int d3t_PlaceAtObject(D3OBJECT *obj, int unk);                                       /* 0x4118a0 */
int d3t_MoveCameraPath(int time, int n, ...);                                        /* 0x4136d0 */
D3ANIM *d3t_MoveObjectPath(D3OBJECT *obj, int time, int n, D3OBJECT **path);         /* 0x413960 */
struct D3TRAIN;
struct D3TRAIN *d3t_CreateRain(int count);                                           /* 0x413b30 */
void d3t_DeleteRain(struct D3TRAIN *rain);                                           /* 0x413d00 */

char ocmd_SelectTextureSetCB(D3OBJECT *obj, OCMDTEXSEL *ctx);                         /* 0x40e560, ForEach callback */
int ocmd_SelectAllTextureSets(char **name, int *set);                                /* 0x40e5a0 */
int ocmd_SetNoTextures(int *on);                                                     /* 0x40e620 */
int ocmd_SetLightSet(int *set);                                                      /* 0x40e640 */
int ocmd_SetFogSet(int *on, int *a, int *b);                                         /* 0x40e670 */
int ocmd_CameraFlight(int *time, char **a, char **b);                                /* 0x40e6f0 */
int ocmd_CameraFlightEnhanced(int *time, char **a, char **b, char **c, char **d, char **e, char **f); /* 0x40e750 */
int ocmd_NewCameraFlight(int *time, char **a, char **b, char **c, char **d, char **e, char **f);      /* 0x40e810 */
int ocmd_ObjectFlight(D3OBJECT **obj, int *time, char **a, char **b, char **c, char **d, char **e);    /* 0x40e8b0 */
int ocmd_ZoomOnObject(int *time, char **name);                                       /* 0x40ea00 */
int ocmd_CreateRain(int *count);                                                     /* 0x40ea90 */
int ocmd_DeleteRain(void **rain);                                                    /* 0x40eaa0 */
int ocmd_CreateParticle(void **parent);                                              /* 0x40ec70 */
int ocmd_KillParticle(D3PARTSYS **ps);                                               /* 0x40edc0 */
int ocmd_SetCameraToDummy(char **name);                                              /* 0x40ee10 */
int ocmd_StopCameraFlight(void);                                                     /* 0x40eea0 */
int ocmd_RenameObject(D3OBJECT **obj, char **name);                                  /* 0x40eec0 */
int ocmd_SetBlocked(D3OBJECT **obj, int *on);                                        /* 0x40eef0 */
int ocmd_SetTransient(D3OBJECT **obj, int *mode);                                    /* 0x40ef40 */
int ocmd_CreateEmitter(int *a, int *count, int *parent, int *size, char **tex, int *f0, int *f1); /* 0x40ef90 */
int ocmd_KillEmitter(D3EMITTER **e);                                                 /* 0x40f0c0 */
int ocmd_SetEmitterAmplitude(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f);  /* 0x40f110 */
int ocmd_SetEmitterPhasespeed(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f); /* 0x40f1b0 */
int ocmd_SetEmitterDirection(D3EMITTER **e, int *x, int *y, int *z);                 /* 0x40f250 */
int ocmd_SetEmitterSize(D3EMITTER **e, int *x, int *y, int *z);                      /* 0x40f2c0 */
int ocmd_SetEmitterVelocity(D3EMITTER **e, int *x, int *y, int *z);                  /* 0x40f320 */
int ocmd_SetEmitterAcceleration(D3EMITTER **e, int *x, int *y, int *z);              /* 0x40f390 */
int ocmd_SetEmitterRndVelocity(D3EMITTER **e, int *x, int *y, int *z);               /* 0x40f400 */
int ocmd_SetEmitterPlane(D3EMITTER **e, int *x, int *y, int *z, int *d);             /* 0x40f470 */
int ocmd_SetEmitterTimeAndAlpha(D3EMITTER **e, int *a, int *b, int *c, int *t);      /* 0x40f4f0 */
int ocmd_SetEmitterColor(D3EMITTER **e, int *r, int *g, int *b, int *a);             /* 0x40f560 */
int ocmd_SetEmitterFlags(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f, int *g); /* 0x40f5d0 */
int ocmd_SetEmitterInitFill(D3EMITTER **e, int *on);                                 /* 0x40f690 */
int ocmd_SetEmitterRebirthFill(D3EMITTER **e, int *on);                              /* 0x40f6f0 */
int ocmd_SetEmitterTextureMode(D3EMITTER **e, int *mode);                            /* 0x40f750 */
int ocmd_SetEmitterIsTrigger(D3EMITTER **e, int *on);                                /* 0x40f7a0 */
int ocmd_SetEmitterTriggerOnce(D3EMITTER **e, int *on);                              /* 0x40f800 */
int ocmd_SetEmitterMaxTrigger(D3EMITTER **e, int *n);                                /* 0x40f850 */
int ocmd_TriggerEmitter(D3EMITTER **e);                                              /* 0x40f8a0 */
int ocmd_SetParticlePos(D3PARTSYS **ps, int *x, int *y, int *z);                     /* 0x40f8f0 */

#endif
