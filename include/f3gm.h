/* Declarations of f3gm.c (0x4f9e30 .. 0x50d0b0): the game data part (clock, building / object tables and
   queries, character helpers, prices). The unit still declares most of its own functions in the .c; these are
   the ones other units call. Names are ours. */
#ifndef F3GM_H
#define F3GM_H

unsigned char *gm_BuildNext(void);              /* 0x501030, the next building of the query */
unsigned char *gm_BuildQuery(int n, ...);       /* 0x501190, n (kind, value) filters; 6 takes no value */
int gm_BuildingMainObject(unsigned char *bld);  /* 0x501b50, the main object of the building by its class (an AIOBJ, ai.c +0x2 objId) */
short *gm_FindChar(int id);                     /* 0x505df0, the character with the id, 0 when none */
int gm_FindById(int *bld, int *obj, int *chr, int id);  /* 0x4fea50, what has the id: 1 building, 2 object, 3 character */
int gm_FindObjectProto(char *name);             /* 0x4fe900, the number of the object prototype with the given name (case-insensitive), 0 when none */
int gm_FindObjectInst(int id);                  /* 0x4fe980, the object instance with the id (int +2) */
char *gm_FindBuildingById(int id);              /* 0x502020, the building with the id (+1) */
void gm_ResetStock(void);                       /* 0x50cc60, the stock of the type 0x1e buildings' stalls to 2 */
int gm_IsType10(unsigned char *b);              /* 0x5024f0, 1 for the town market */
int gm_IsHomeType(unsigned char *b);            /* 0x502520, 1 for a home type */
void gm_ShowSlotObjects(int unused, int scene, int c);  /* 0x502a60, the scene objects of the character's five slots on */
void gm_AssignBuilding(unsigned char *b, unsigned short place, unsigned own);  /* 0x502b60 */

void gm_ClearBuildings(void);                   /* 0x4fe7f0, clears the 0x100 records of ai_Blds */
float gm_Mix2(float u, float v);                /* 0x504e80 */
void gm_ClearObjects(void);                     /* 0x4fe830, returns the instances of live owners and clears the 0x5008-record table */

int gm_LoadTables(char *root);                                       /* 0x4fe570 */
void unk_LoadIconNames(void);                                         /* 0x4fde90 */
void gm_ResetCharacters(void);                                        /* 0x503e80 */
void gm_FreeIconNames(void);                                          /* 0x4fdfb0 */
void gm_Free(void);                                                   /* 0x4fe9c0 */
int gm_TitlesLeft(int n);                                             /* 0x504ce0 */
int gm_CollectCarried(int id, int *out);                                   /* 0x50af00 */
int gm_IsBankrupt(unsigned short *p, int n);                                  /* 0x50a6c0, p: an AICHAR */
#endif
