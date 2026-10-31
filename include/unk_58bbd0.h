/* unk_58bbd0.h -- the production / trade windows (unnamed): the table init and the "Handel\Handel_PRODUKTION"
   screen, see src/game/unk_58bbd0.c and notes/units/unk_58bbd0.md */
#ifndef UNK_58BBD0_H
#define UNK_58BBD0_H

struct AIBLD;
struct D3SCENE;

/* 0x62aae8: the 15 building groups, 0x50 bytes each: the name, then its 12 kind ids (ai_BuildingName) */
typedef struct AIBUILDNAME {
    char name[0x20];
    int ids[0xc];
} AIBUILDNAME;
extern AIBUILDNAME ai_BuildNames[0xf];  /* 0x62aae8 */

void ai_Unk58bbd0(void);                /* 0x58bbd0, the production window's table init */
void ai_MarketContact(struct AIBLD *b); /* 0x58f510, the shop-counter contact of a building */
int ai_BuildingName(int id, char *out); /* 0x58f640, the building name of a kind id */
/* 0x61168c: the "%1G" format of the map's city names (ai_FormatCityName). */
void ai_TradeContactChecks(void);       /* 0x5912c0, the pending trade / master checks every contact listener runs once per frame */
/* The windows the building contacts open (defined far below their callers in the .c). */
int ai_StorageWindow(int building, int list);           /* 0x5b26b0, the store window */
int ai_PersonnelBook(int building, char *text);         /* 0x5aab80, the personnel book */
void ai_MasterLetterWindow(unsigned char *building);    /* 0x5c3200, the master letter window */

/* the placement-candidate list of the build screen (ai_BauplatzFilter / ai_FindBauplaetze) */
extern int *ai_BauplatzList;            /* 0x6f5308 */
extern int ai_BauplatzCount;            /* 0x6f2688 */
/* 0x6f35c4: the vg_ objects from this index on are hidden ((unsigned char) index, (char) != 2) */
extern int ai_VgHideFrom;

/* 0x6f22e4: the object clicked in the scene (cm.h declares it for the units that include cm.h) */
extern int ai_ClickedObject;
/* 0x6f231c: the permission bits of the player in the building the scene focus is on (bit 0x200 / 0x400
   offer the building's contact plates) */
extern int ai_BuildingPerms;
/* The text slots of the building contacts (the text system fills them at run time). */














/* 0x6f25b8: the local player's character id (the owner field of the building records) */
extern unsigned short ai_PlayerId;

/* the plate text ids of the shop-counter contacts (read by the window code as ints) */






/* 0x5ba6b0, the views of ai_InventoryHas: the character's inventory (AICHAR +0x178, ai.h) and the object
   0x4ffe90 finds in it (AIOBJ: the type id at +0, the state flag at +0x21) */
#pragma pack(push, 1)
typedef struct G8INVCHAR {
    char pad00[0x178];
    int inventory;                      /* +0x178, the character's object list (AICHAR.inventory) */
} G8INVCHAR;

typedef struct G8INVOBJ {
    unsigned short id;                  /* +0x00, the object's type id (AIOBJ.id) */
    char pad02[0x1f];
    int unk21;                          /* +0x21, set when the object counts (AIOBJ's pad20 area) */
} G8INVOBJ;
#pragma pack(pop)

void ai_FreeSettings(void);                                        /* 0x5a8920 */
void ai_FreePicture(void);                                         /* 0x5bc0f0 */
#endif
