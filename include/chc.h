/* chc.c -- character script commands ("CreateCharacter(): Could not load character"), original file
   name unknown. Starts at 0x40ba10 right after the script builtins (evtc.c) and ends with their
   registration at 0x40d270 (not reconstructed yet). EVTBUILTIN functions: every argument is passed as a
   pointer to its value. Names are ours (chc_ + the script name). */
#ifndef UNK_40BA10_H
#define UNK_40BA10_H

#include "evt.h"

/* CHACTION fields used by the animation-script callbacks */
#define CHC_SCRIPT(a)   ((char *)(a) + 0x90)                        /* script to run, 0x5f chars */
#define CHC_GROUP(a)    (*(int *)((char *)(a) + 0x17c))              /* EVTSCRIPT.group for it */
#define CHC_ARG(a)      (*(int *)((char *)(a) + 0x180))              /* its int argument */
#define CHC_UNK18C(a)   (*(int *)((char *)(a) + 0x18c))

/* D3TEXFLAGS as built for d3_SetObjectAlpha by SetCharacterTransperancy */
typedef struct CHCALPHA {
    unsigned char alpha;
    unsigned char b1;
    unsigned char f0 : 1, fr : 7;
    unsigned char b3;
} CHCALPHA;
#define d3_SetObjectAlphaC ((void (*)(D3OBJECT *, struct D3OBJINFO *, CHCALPHA))d3_SetObjectAlpha)

extern char ch_StepSample[];        /* 0x5f1230, "normal_s" (ch.c data) */
int mem_IsValid(void *p);                                                               /* 0x4225e0 */

int chc_CreateCharacter(char **name, float *x, float *y, float *z);                     /* 0x40ba10 */
int chc_CreateCharacterAtDummy(char **mesh, D3OBJECT **dummy, char **name);                        /* 0x40ba90 */
int chc_KillCharacter(CHARACTER **ch);                                                  /* 0x40bbb0 */
int chc_WalkToDummy(CHARACTER **ch, D3OBJECT **dummy);                                  /* 0x40bbc0 */
int chc_WalkToDummyRotate(CHARACTER **ch, D3OBJECT **dummy);                            /* 0x40bc80 */
int chc_WalkToDummyRotateVerified(CHARACTER **ch, D3OBJECT **dummy, int *dist);         /* 0x40bd40 */
int chc_WalkToDummyVerified(CHARACTER **ch, D3OBJECT **dummy, int *dist);               /* 0x40be00 */
int chc_PlayAnimation(CHARACTER **ch, char **name, int *n);                             /* 0x40bec0 */
void chc_AniSoundCB(CHACTION *a);                                                       /* 0x40bf60 */
int chc_PlayAnimationSound(CHARACTER **ch, char **name, int *n, int *frame, char **sound); /* 0x40bfc0 */
void chc_AniScriptCB(CHACTION *a);                                                      /* 0x40c090 */
int chc_PlayAnimationScript(CHARACTER **ch, char **name, int *n, int *frame, char **script); /* 0x40c0d0 */
void chc_AniScriptIntCB(CHACTION *a);                                                   /* 0x40c1d0 */
int chc_PlayAnimationScriptInt(CHARACTER **ch, char **name, int *n, int *frame, char **script, int *arg); /* 0x40c210 */
int chc_GetCharacterHandle(char **name);                                                /* 0x40c330 */
int chc_TakeObject(CHARACTER **ch, char **name, int *frame, D3OBJECT **obj);            /* 0x40c390 */
int chc_TakeObjectLeft(CHARACTER **ch, char **name, int *frame, D3OBJECT **obj);        /* 0x40c440 */
int chc_TakeObjectScript(CHARACTER **ch, char **name, int *n, D3OBJECT **obj, int *frame, char **script); /* 0x40c4f0 */
int chc_GiveObject(CHARACTER **ch, char **name);                                        /* 0x40c630 */
int chc_DropObject(CHARACTER **ch, char **name, int *frame, char **obj);                /* 0x40c660 */
int chc_DropObjectLeft(CHARACTER **ch, char **name, int *frame, char **obj);            /* 0x40c720 */
int chc_SitDown(CHARACTER **ch, char **name);                                           /* 0x40c7e0 */
int chc_SitDownAtOnce(CHARACTER **ch, char **name);                                     /* 0x40c870 */
int chc_GetUp(CHARACTER **ch, char **name);                                             /* 0x40c910 */
int chc_SetCharacterCamera(CHARACTER **ch, char **view);                                /* 0x40c9a0 */
int chc_MoveCharacterCamera(CHARACTER **ch, char **view, int *time);                    /* 0x40ca60 */
int chc_PreloadAnimation(CHARACTER **ch, char **a, char **b, char **c, char **d);       /* 0x40cb40 */
int chc_CharacterIdle(CHARACTER **ch);                                                  /* 0x40cba0 */
int chc_StopCharacter(CHARACTER **ch);                                                  /* 0x40cbe0 */
int chc_KillCharacterAnimations(CHARACTER **ch);                                        /* 0x40cc20 */
int chc_DummyBlocked(D3OBJECT **dummy, int *dist);                                      /* 0x40cc80 */
int chc_CharacterCount(void);                                                           /* 0x40cd70 */
int chc_LookAtCharacter(CHARACTER **ch, CHARACTER **target);                            /* 0x40cda0 */
int chc_LookAtObject(CHARACTER **ch, D3OBJECT **obj);                                   /* 0x40ce90 */
int chc_SetCharacterToDummy(CHARACTER **ch, D3OBJECT **dummy);                          /* 0x40cf10 */
int chc_CharacterSitting(CHARACTER **ch);                                               /* 0x40cfd0 */
int chc_GetCharacterSubObjectHandle(CHARACTER **ch, char **name);                       /* 0x40d010 */
int chc_SetCharacterStepSample(char **name);                                            /* 0x40d060 */
int chc_SetLowPoly(CHARACTER **ch, int *on);                                            /* 0x40d080 */
int chc_AttachObject(CHARACTER **ch, char **where, char **obj);                         /* 0x40d0d0 */
int chc_SetCharacterTransperancy(CHARACTER **ch, int *alpha);                           /* 0x40d150 */
int chc_CharacterSitsDown(CHARACTER **ch);                                              /* 0x40d1e0 */
int chc_FindInCharacterName(CHARACTER **ch, char **s);                                  /* 0x40d220 */
void chc_Register(void);                                                                 /* 0x40d270 */

#endif
