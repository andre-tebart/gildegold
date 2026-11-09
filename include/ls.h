/* Declarations of ls.c, the save game unit from 0x519010 (load / save, the network game transfer):
   its functions and data as the definitions spell them. Names are ours except the ls_* ones the
   strings leak. */
#ifndef LS_H
#define LS_H

#include "ls_types.h"                     /* NETHDR, LSSAVE */

struct CHACTION;

int ls_LanguageOf(char *s);                                             /* 0x519010 */
int ls_GetLanguage(void);                                               /* 0x5190a0 */
int ls_SaveGame(int name, char *path, unsigned flags);                  /* 0x5190b0 */
int ls_WriteSaveHeader(int f, unsigned flags);                          /* 0x5194c0 */
void ls_RefsToIds(void);                                               /* 0x519a00 */
void ls_IdsToRefs(void);                                               /* 0x519aa0 */
int ls_RelinkScene(int f);                                               /* 0x519b50 */
int ls_WriteObjects(int f);                                             /* 0x519bf0 */
int ls_WriteBuildings(int f);                                               /* 0x519d20 */
int ls_WritePlayerRecords(int f);                                               /* 0x51a1a0 */
int ls_WriteCharacters(int f);                                               /* 0x51a4b0 */
unsigned ls_ActionIndex(struct CHACTION *p);                            /* 0x51ad30 */
struct CHACTION *ls_ActionAt(int n);                                    /* 0x51ad60 */
int ls_WriteSceneCharacter(struct CHARACTER *c, int owner, int f);  /* 0x51ad80, writes one scene character record */
int ls_WriteSceneRecords(int f);                                               /* 0x51aff0 */
int ls_WritePlayerSlots(int f);                                               /* 0x51b340 */
int ls_WriteRelations(int f);                                           /* 0x51b890 */
int ls_WriteStatistics(int f);                                               /* 0x51b910 */
int ls_WriteAvatars(void *f);                                           /* 0x51bdb0 */
int ls_WriteEventTables(int f);                                               /* 0x51bdc0 */
int ls_WriteParties(int f);                                               /* 0x51c2f0 */
int ls_WriteHistory(int f);                                             /* 0x51c450 */
int ls_WriteMessageQueues(int f);                                               /* 0x51c560 */
int ls_WriteEnvironment(int f);                                               /* 0x51c870 */
int ls_Read(void *buf, int size, int count, int f);                     /* 0x51c920 */
int ls_LoadGame(int name, int kind);                                  /* 0x51c950, kind is not read (main passes 1 / 2, f3init 5) */
typedef struct LSHEADER {               /* 0x100 bytes, the header of a save game (ls_ReadSaveHeader; ai_Unk76f920) */
    int version;                        /* 0x00, kept in ls_Version */
    unsigned char flags;                /* 0x04, bit 1: a map-only save */
    char name[0x20];                    /* 0x05 */
    char pad25[3];
    char slotPair[8];                   /* 0x28, the two ints at +0x40 / +0x44 of the player's slot (ai_PlayerSlots) */
    unsigned char slotByte;             /* 0x30, the byte +0x48 of the player's slot */
    unsigned char numPlayers;           /* 0x31, ai_NumPlayers (from 0x10033) */
    char clock[0xe];                    /* 0x32, ai_Time (from 0x10028) */
    char charText30[0x10];              /* 0x40, the player's character +0x30 (16 bytes, a name part?) */
    char charText40[0x10];              /* 0x50, its +0x40 (from 0x10054) */
    unsigned char charByte164;          /* 0x60, its +0x164 */
    unsigned char charByte166;          /* 0x61, its +0x166 */
    unsigned char charByte0d;           /* 0x62, its +0xd */
    unsigned char charByte0c;           /* 0x63, its +0xc (from 0x10054) */
    int charRef54;                      /* 0x64, its +0x54 reference (from 0x10054) */
    int worth;                          /* 0x68, its worth (gm_GetWorth, converted) */
    unsigned char charByte09;           /* 0x6c, its +9 */
    unsigned char saveSlot;             /* 0x6d, ls_SaveSlot */
    char pad6e[2];
    int startBuilding;                  /* 0x70, the building the view starts at (main.c), -1 below 0x1002b */
    int startObject;                    /* 0x74, the object instance with it (main.c), -1 below 0x1002b */
    int party[8];                       /* 0x78, the party table (from 0x10034) */
    int difficulty;                     /* 0x98, ai_Difficulty of the save, 2 below 0x10038 */
    char title[0x60];                   /* 0x9c, "Savegame" below 0x10039 */
} LSHEADER;
int ls_ReadSaveHeader(int f, int *h);                                   /* 0x51cee0 */
int ls_LoadScene(int f);                                               /* 0x51d2d0 */
int ls_ReadObjects(int f);                                              /* 0x51d3d0 */
char ls_HookBuildingCB(char *o);                                        /* 0x51d4f0 */
char ls_ClearObjectFlag(char *o);                                       /* 0x51d530 */
int ls_ReadBuildings(int f);                                            /* 0x51d550 */
int ls_ReadPlayerRecords(int f);                                        /* 0x51db00 */
int ls_ReadCharacters(int f);                                           /* 0x51de70 */
struct CHARACTER *ls_ReadCharacter(int f);                                         /* 0x51e750 */
int ls_ReadSceneRecords(int f);                                               /* 0x51e900 */
int ls_ReadPlayerSlots(int f);                                               /* 0x51f080 */
int ls_ReadRelations(int f);                                            /* 0x51f650 */
int ls_ReadStatistics(int f);                                               /* 0x51f720 */
int ls_ReadAvatars(void *f);                                            /* 0x51fb80 */
int ls_ReadEventTables(int f);                                               /* 0x51fb90 */
int ls_ReadParties(int f);                                               /* 0x520000 */
int unk_Unk520200(int f);                                               /* 0x520200 */
int unk_Unk520310(int f);                                               /* 0x520310 */
int ls_ReadEnvironment(int f);                                               /* 0x5205f0 */
int ls_FixupAfterLoad(void);                                            /* 0x5206b0 */
int ls_SendGame(char *name);                                            /* 0x520a30 */
int ls_ReceiveGame(void);                                               /* 0x520b80 */

#ifndef DATA_VIEWS_H
extern short ai_Unk75e13c;                                  /* 0x75e13c */
#endif
extern LSSAVE ls_SavePath;                                  /* 0x75e640 */
extern unsigned short ai_ScreenImage[19200];                /* 0x766320 */
extern int ai_Unk76f920[0x40];                              /* 0x76f920, the 0x100-byte header of the loaded save game */
#ifndef DATA_VIEWS_H
extern int ai_Unk76f924;                                    /* 0x76f924 */
extern int ai_Unk76f9b8;                                    /* 0x76f9b8 */
extern char ai_Unk76f9bc[100];                              /* 0x76f9bc */
#endif
extern int ai_NextPartyId;                                    /* 0x61a658 */
extern int ls_Language;                                     /* 0x62364c */
extern int ls_Version;                                      /* 0x623650 */
extern const char ls_LanguageNames[][0x40];                 /* 0x5ee978 */
extern void *d3_Terrain;                                    /* 0x633738, d3_fl.c: the current terrain map */
extern int ai_NextCrimeId;                                    /* 0x6f1b88 */
extern int ls_SpareInt;                                    /* 0x6f1bcc */
extern unsigned char ls_SaveSlot;                           /* 0x6f1c38 */
extern int ai_NextEventId;                                    /* 0x6f2618 */
extern int he_EventTop;                                      /* 0x6f261c, the highest used index of he_Events */
extern int he_QuickTop;                                    /* 0x6f2620 */
extern unsigned char he_Events[339968];                     /* 0x6fab40 */
#define he_Jobs (*(struct AIJOB (*)[0x400])&he_Events)  /* 0x6fab40, he_Events as the 0x400 event records (AIJOB, ai_types.h) */
extern char * he_QuickList[10921];                          /* 0x74db40 */
extern int he_EventCount;                                    /* 0x7585e4 */
#ifndef DATA_VIEWS_H
extern int ai_Unk75e134;                                    /* 0x75e134 */
extern int ai_Unk75e138;                                    /* 0x75e138 */
#endif
extern unsigned cm_LoadSize;                               /* 0xa68d28 */
extern int cm_LastSeq;                                    /* 0xf81168 */
extern unsigned cm_LoadReceived;                               /* 0xf81220 */

#endif
