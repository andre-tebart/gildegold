/* Declarations of unk_4b39f0.c (0x4b39f0..): missions ("Auftraege"). Names are ours. */
#ifndef UNK_4B39F0_H
#define UNK_4B39F0_H

#include "am.h"

#include "unk_4b39f0_types.h"        /* G13MIS */
typedef struct G13TYPE {                /* 0x1c bytes, ai_MissionTypes[48], the mission types */
    int type;                           /* 0x00 */
    int kind;                           /* 0x04, 0..5 */
    int text;                           /* 0x08, text id */
    int unk0c;                          /* 0x0c */
    int speech;                         /* 0x10, the speech number nn (_AUFTRAEGE_ERFOLG_HS_nn) */
    int goal;                           /* 0x14, the goal amount a condition compares against (count, sum, percent or minutes) */
    int unk18;                          /* 0x18 */
} G13TYPE;

extern G13MIS ai_Missions[0x80];       /* 0xf82320 */
extern G13TYPE ai_MissionTypes[48];        /* 0x601f88 */

int ai_ClearMissions(void);                                                          /* 0x4b39f0 */
unsigned int ai_MissionRandom(unsigned int n);                                       /* 0x4b3a10 */
G13MIS *ai_FindOwnerMission(G13MIS *m);                                                 /* 0x4b3a50 */
void ai_RestartMission(G13MIS *m);                                                    /* 0x4b3a90 */
G13TYPE *ai_GetMissionType(int type);                                                 /* 0x4b3ae0 */
int ai_HasMission(int owner);                                                      /* 0x4b3b10 */
int ai_CountMissionTypes(int kind);                                                      /* 0x4b3b40 */
G13TYPE *ai_FindMissionType(int kind, G13TYPE *after);                                 /* 0x4b3b70 */
void ai_Unk4b3bd0(int owner, int type);                                          /* 0x4b3bd0 */
G13MIS *ai_NewRandomMission(int owner, int kind, int x);                                /* 0x4b3c10 */
int ai_RandomMissionType(int kind);                                                      /* 0x4b3c40 */
G13MIS *ai_NewMission(int owner, int type, int x);                                /* 0x4b3ca0 */

#endif
