/* Declarations of f3init.c, the game setup unit (the town / building scenes, the player records, the
   new game): its functions and data as the definitions spell them (f3init's own record views by tag).
   Names are ours except the init_* ones its error texts leak. */
#ifndef F3INIT_H
#define F3INIT_H

#include "f3init_types.h"                 /* INIT_PLAYER */
#include "rd_types.h"                     /* G15BK */
struct INIT_BLD;
struct D3OBJECT;

int init_SceneNameToPrototype(char *o);                                 /* 0x50d100 */
void init_SceneObjectDestroyCB(int o);                                  /* 0x50d350 */
void init_HookObject(char *o, unsigned short *p, int kind);             /* 0x50d380 */
char init_ResetSceneUniverses(void);                                    /* 0x50d460 */
void init_FreeScenes(void);                                             /* 0x50d490 */
void init_AllocSceneBuilding(int o);                                    /* 0x50d550 */
void init_InheritOwner(char *o);                                        /* 0x50d6a0 */
char init_LoadTownScene(char *n);                                       /* 0x50d6d0 */
void init_AdjustSceneLights(int o);                                     /* 0x50d730 */
void init_LoadObjectScene(int n);                                       /* 0x50d7f0 */
int init_BuildingKindName(int k, char *out);                            /* 0x50dba0 */
void init_LoadBuildingScene(int k);                                     /* 0x50dc30 */
void init_StartNewGame(void);                                           /* 0x50e0d0 */
void init_CreateOpponents(int n, int m, int *list);                     /* 0x50e330 */
unsigned short init_PlaceOffices(int n, int *out, int *ids);            /* 0x50e5a0 */
int init_CreateStartCharacters(void);                                   /* 0x50e690 */
void init_CreateOpeningObjects(void);                                   /* 0x50f0b0 */
void init_StockMarketStalls(void);                                      /* 0x50f340 */
void init_Betrieb(struct INIT_BLD *b);                                  /* 0x50f570 */
void init_OpenBuilding(struct INIT_BLD *b);                             /* 0x50f750 */
void init_OpenTown(void);                                               /* 0x50f910 */
char init_CollectSiteObject(struct D3OBJECT *o, void *ctx);             /* 0x510730 */
void init_LoadSiteScene(char *name);                                    /* 0x510770 */
int init_InitTown(char *city);                                          /* 0x510ad0 */
void init_ShowStatus(int t);                                            /* 0x510e20 */
int init_StartNetworkGame(char *name);                                  /* 0x510e50 */
int init_JoinNetworkGame(int saveName);                                               /* 0x5112d0 */
int init_SetupNewGame(int n);                                           /* 0x511400 */
char init_CollectTorchObject(struct D3OBJECT *o, void *ctx);           /* 0x5115b0 */
void init_StripTorches(struct D3OBJECT *s);                                              /* 0x511600 */
void unk_Unk5116b0(void);                                               /* 0x5116b0 */
char init_TownBuildingHook(char *o);                                    /* 0x511700 */
void unk_Unk511790(void);                                               /* 0x511790 */
int ai_Unk511810(char *o);                                              /* 0x511810 */
void unk_Unk511840(void);                                               /* 0x511840 */
int init_NameBuildings(void);                                           /* 0x511890 */
void init_LocalData(void);                                              /* 0x511ad0 */
void init_FillMarkets(int p);                                           /* 0x511bc0 */
void unk_Unk511c70(void);                                               /* 0x511c70 */
int init_ObjectSceneUniverse(unsigned short *p);                        /* 0x511dc0 */
int init_BuildingSceneUniverse(unsigned char *b);                       /* 0x511e60 */
void init_SyncSceneObjects(char *o);                                    /* 0x511f20 */
void unk_Unk5124d0(void);                                               /* 0x5124d0 */
void init_LoadShopScript(unsigned char *b, unsigned short *o);          /* 0x5124f0 */
void init_SoundObjectHook(char *o);                                     /* 0x5126a0 */
void init_GateObjectHook(int o);                                        /* 0x512780 */
void init_SoundObjectFreeHook(char *o);                                 /* 0x5127c0 */
void init_PlayTownSound(char *name, int volume, int delay);             /* 0x5127f0 */
void init_PlayGraveyardSound(char *b);                                  /* 0x512840 */
void unk_Unk512890(void);                                               /* 0x512890 */
void init_SetSeasonTextures(void);                                      /* 0x5128d0 */
void init_PlantTextureSetHook(char *o);                                 /* 0x512de0 */
char init_TextureSetHook(int o);                                        /* 0x512e40 */
char init_BuildingTextureSetHook(struct D3OBJECT *o, void *ctx);  /* 0x512e80 */
void init_SetCharacterAnimations(unsigned char *b);                     /* 0x512f30 */
void init_ShowBuildingScene(unsigned char *b, int slot);                /* 0x513050 */
void init_FinishBuildingScene(unsigned char *b, unsigned short *o);     /* 0x513400 */
struct D3FLMAP;
void init_SetDetailLevel(struct D3FLMAP *s, char f);                   /* 0x513590 */
void unk_Unk5136b0(void);                                               /* 0x5136b0 */
void init_AfterTownLoad(void);                                               /* 0x513740 */
int init_ParseNumberList(char *s, int *out, int n);                     /* 0x513970 */
int init_LoadCityINI(char *city, int slot);                             /* 0x513a20 */

extern unsigned int d3_LightKeyBudgetCulled;                /* 0x5fcc6c */
extern unsigned int d3_LightKeyBudgetVisible;               /* 0x5fcc70 */
extern unsigned char init_Detail;                           /* 0x61adf8 */
extern float init_SoundVolume;                              /* 0x61adfc */
extern int init_ShopScript;                                 /* 0x61ae00 */
extern int init_AiTown;                                     /* 0x624564 */
extern unsigned int d3fl_DetailScale;                       /* 0x633748 */
extern unsigned char d3if_Unk6339db;                        /* 0x6339db */
extern int d2_CutsceneActive;                               /* 0x6ed9d4 */
extern char * ai_KindBChars[8];                             /* 0x6f1c04 */
extern unsigned char init_TextureSet;                       /* 0x6f1c24 */
extern int d3_QuadTree;                                     /* 0x6f1c28 */
extern int init_TownSoundCount;                             /* 0x6f1c34 */
extern int ai_FrameWait;                                    /* 0x6f22bc */
extern int main_StartHouseCheat;                            /* 0x6f2430 */
extern int ai_Unk6f2440;                                    /* 0x6f2440 */
extern int main_MoneyCheat;                                 /* 0x6f2444 */
extern int ai_MapSize;                                      /* 0x6f4ee8 */
extern int init_StatusForm;                                 /* 0x75df00 */
extern INIT_PLAYER init_PlayerProfile;                      /* 0x75e5a0 */
extern int ai_SceneChars[32];                                /* 0x7603a0 */
extern signed char init_BuildingUniverse[0x22];             /* 0x76fa60 */
extern unsigned short * ai_SceneObject;                     /* 0x76fa84 */
extern signed char init_ObjectUniverse[0x2dc];              /* 0x76faa0 */
extern int init_TownSounds[16];                             /* 0x76fd80 */
extern unsigned char * ai_SceneBuilding;                    /* 0x76fdc0 */
extern int ai_NBauplaetze;                                  /* 0xf812e0 */
extern G15BK * ai_Bauplaetze;                               /* 0xf812e4 */
extern int evt_NoGroup;                                     /* 0x5f8430 */
extern float d3o_FogScale;                                  /* 0x5fbd04 */

#endif
