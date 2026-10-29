/* evtc.c -- script commands (EVTBUILTIN functions: every argument is passed as a pointer to its
   value), original file name unknown ("CallUserFunction(): Invalid userfunction"). Starts at 0x40b580 right
   after the character code (ch.c) and ends at 0x40ba08 (evtc_Register). Names are ours. */
#ifndef UNK_40B580_H
#define UNK_40B580_H

#include "evt.h"

extern volatile unsigned long d3_Time;  /* 0x6333cc, also in d3.h */
extern unsigned int tb_Delay;           /* 0xfe7a88 */

typedef void (*EVTUSERFUNC1)(int a);
typedef void (*EVTUSERFUNC2)(int a, int b);

int evtc_Random(int *n);                                                                /* 0x40b580 */
int evtc_GetTime(void);                                                                 /* 0x40b5a0 */
int evtc_RunScript(char **name);                                                        /* 0x40b5b0 */
int evtc_RunScript2(char **name, int *a, int *b);                                       /* 0x40b610 */
int evtc_RunScript1(char **name, int *a);                                               /* 0x40b680 */
int evtc_StopScript(int *sc);                                                           /* 0x40b6f0 */
int evtc_Wait(int *t);                                                                  /* 0x40b710, Unk40b710 in evt.h */
int evtc_FindScript(char **name);                                                       /* 0x40b7a0 */
int evtc_StopOthers(void);                                                              /* 0x40b7b0 */
int evtc_CallUserFunction(EVTUSERFUNC1 *f, int *a);                                     /* 0x40b820 */
int evtc_CallUserFunctionExt(EVTUSERFUNC2 *f, int *a, int *b);                          /* 0x40b860 */
void evtc_Register(void);                                                                /* 0x40b8a0 */

int ai_AlwaysTrue(void);                                                                /* 0x4a8c10, also in d3_engine.h */
int sv_ReturnZero(void);                                                                    /* 0x52a040, an empty command (ch.h) */

#endif
