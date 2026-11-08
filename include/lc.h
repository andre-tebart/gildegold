/* lc.h -- the production / trade windows (unnamed): the table init and the "Handel\Handel_PRODUKTION"
   screen, see src/game/unk_58bbd0.c */
#ifndef LC_H
#define LC_H

#include "am_types.h"                 /* G9DOMIZIL, the Domizil table's entry (ai_DomizilTable) */
#include "ai_types.h"                 /* AITIME, the goods-reserve clock (LCSETTINGS.reserveTime) */

struct AIBLD;
struct D2SURF;
struct AIOBJ;
struct D3SCENE;
struct D3OBJECT;
struct D3VECTOR;

/* 0x62aae8: the 15 building groups, 0x50 bytes each: the name, then its 12 kind ids (ai_BuildingName) */
typedef struct AIBUILDNAME {
    char name[0x20];
    int ids[0xc];
} AIBUILDNAME;
extern AIBUILDNAME ai_BuildNames[0xf];  /* 0x62aae8 */
extern int ai_BuildGroup;               /* 0x62af98, the building group the construction window shows (1 .. 9) */
/* 0x62afa0: the opening hours of a building class (AIBTYPE.buildingClass), 8 bytes a row, ended by class 0 */
typedef struct LCOPENHOURS {
    int buildingClass;             /* 0x0, the class the row is for */
    unsigned char open;            /* 0x4, the hour it opens */
    unsigned char close;           /* 0x5, the hour it closes (open while open <= hour < close) */
} LCOPENHOURS;
extern LCOPENHOURS ai_OpeningHours[19]; /* 0x62afa0, ai_IsBuildingOpen's table */
extern float ai_MapCellScale;          /* 0x62c270, the town map pixels per scene cell (7.5 / 5.33) */
extern char ai_NewsKeyHS46[20];         /* 0x62c2ac, the second text of ai_NotifyPlayerOfChar's news (event 0x5fa): the key "_NACHRICHTEN_HS_46" */
extern char ai_NewsKeyFriedhof12[28];   /* 0x62c364, the sound of the graveyard work news ai_TellGraveWorkSuccess (event 0x5fb): the key "_NACHRICHTEN_FRIEDHOF_12" */
extern char ai_NewsKeyFriedhof13[28];   /* 0x62c380, the sound of the graveyard work news (event 0x5fb): the key "_NACHRICHTEN_FRIEDHOF_13" */
extern char ai_NewsKeyFriedhof00[28];   /* 0x62c39c, the sound of the graveyard news ai_GraveyardMessage (event 0x5fa): the key "_NACHRICHTEN_FRIEDHOF_00" */

void ai_InitProductionTables(void);                /* 0x58bbd0, the production window's table init */
int ai_ProductionWindow(struct AIBLD *bld);        /* 0x58bd60, the production window "Handel\Handel_PRODUKTION" of the building */
void ai_MarketContact(struct AIBLD *b); /* 0x58f510, the shop-counter contact of a building */
void ai_RunObjectRoom(struct AIOBJ *room); /* 0x59f410, runs the room object of the building the player is in (its kind's handler) */
int ai_BuildingName(int id, char *out); /* 0x58f640, the building name of a kind id */

/* the "vg_" object list ai_VgFilter collects (ai_ShowVgObjects, cm's building creation) */
typedef struct AIVGLIST {
    int objs[0x20];                /* 0x00 */
    int count;                     /* 0x80 */
} AIVGLIST;
/* the construction-site record lc_GetGebaeudeBauplatz fills (0x54 bytes: cm's building creation clears 0x15 dwords) */
typedef struct AIBUILDSITE {
    union {
        char name[0x40];           /* 0x00, the building name (ai_BuildingName), then the "bk_" site object's name */
        int nameHead;              /* 0x00, cleared before the name is written */
    };
    float pos[4];                  /* 0x40, the owner's world position (d3_PointToWorld) */
    int object;                    /* 0x50, the "bk_" site object ai_FindBauplatz found */
} AIBUILDSITE;
char ai_VgFilter(int obj, AIVGLIST *ctx);                       /* 0x58f920, d3_ForEachObject callback: collects "vg_" objects */
int lc_GetGebaeudeBauplatz(AIBUILDSITE *out, struct AIBLD *b);  /* 0x58fc40, the construction site of a building */
void ai_CheckBuildSites(void);          /* 0x58fb60 */
/* 0x61168c: the "%1G" format of the map's city names (ai_FormatCityName). */
void ai_TradeContactChecks(void);       /* 0x5912c0, the pending trade / master checks every contact listener runs once per frame */
/* The windows the building contacts open (defined far below their callers in the .c). */
int ai_TransportWindow(struct AIBLD *bld, unsigned int mode);          /* 0x5af1d0, the transport window of a building (sv_UpdatePanel: mode 1) */
void ai_StorageWindow(struct AIBLD *bld, int list);           /* 0x5b26b0, the store window */
int ai_ProductionGoodsWindow(struct AIBLD *bld);        /* 0x593150, the goods / production window a kontor contact opens */
void ai_SpyReportsWindow(struct AIBLD *bld);           /* 0x5a45c0, the spy reports window "locations\diebesgilde\diebesgilde_informationen" of a guild building */
void ai_GelageWindow(struct AIBLD *bld);                 /* 0x5b9f30, the "special\gelage" window of the kontor contacts */
void ai_TrainingWindow(struct AIBLD *bld);              /* 0x5bba00, the "special\training" window (regeneration / target / straw-dummy plates) */
void ai_EquipWindow(struct AIBLD *bld);                 /* 0x5bb350, the equip window of the AUSRUESTEN / weapon-stand plates */
void ai_BuildingWindow(struct AIBLD *bld);              /* 0x5b84f0, the options window of a building (sell, renovate, upgrade, ...) */
void ai_ManagedBuildingWindow(struct AIBLD *bld);       /* 0x5b8fc0, its variant for the classes 3 / 5 with the permission 0x10 */
struct AMOUNTCFG; struct AICHARREC;
void ai_AmountInfoWindow(int kind, struct AICHARREC *ch);  /* 0x5c1a80, the info box of an amount kind (ch is not read) */
void ai_AmountKindMenu(void (*pick)(int kind), char *caption, int withKind8);  /* 0x5c1b40, the seven-button amount kind menu: pick gets the clicked kind */
void ai_AgendaOfficeList(int kind);  /* 0x5c1e10, the agenda of one office column (ai_AmountKindMenu's callback) */
void ai_OpenLawBook(int book);  /* 0x5c2160, the law book 0x111 / 0x112 / 0x113 */
void ai_LawBookInfoWindow(int textId, int lawText);  /* 0x5c2410, the info box of one law */
int ai_AmountMenu(struct AMOUNTCFG *cfg, char *caption, int prompt, int (*pick)(struct AICHARREC *, int));  /* 0x5c1130, the special-amount menu: returns the chosen character record (0: none) */
void ai_PersonnelBook(struct AIBLD *bld, char *model);  /* 0x5aab80, the personnel book of a building (ob_PERSONALBUCH) */
void ai_MercenaryBook(struct AIBLD *bld, char *model);  /* 0x5ac110, the mercenary book of a building (ob_SOELDNERBUCH) */
void ai_HelperBook(struct AIBLD *bld, char *model);     /* 0x5ad150, the helpers' book of a building (ob_GEHILFENBUCH) */
void ai_MasterLetterWindow(struct AIBLD *bld);         /* 0x5c3200, opens the master letter of the player's building */
int ai_FreeProdSetting(int id);                     /* 0x5a86c0, unlinks and frees the production setting of the building id (ai_ProdSetting), -1 when it has none */

/* the placement-candidate list of the build screen (ai_BauplatzFilter / ai_FindBauplaetze) */
extern int *ai_BauplatzList;            /* 0x6f5308 */

/* the goods grids of the production window (ai_InitProductionTables sets them up, ai_UpdateProdGoodsList fills
   them from the store, ai_LayoutProdGoodsList lays them out): a row of four slots, and a slot */
typedef struct LCPRODROW {              /* 0x14 bytes */
    int x;                              /* 0x00, the row's place in the window: centred by its open slots' picture */
    int y;                              /* 0x04, (the picture 0x743's height + 0x16) * row (the second grid + 0xb6) */
    int shown;                          /* 0x08, the slots the title picture shows (-1 at the init) */
    int slots;                          /* 0x0c, the slots of the row the building's level opens (0 .. 4) */
    int title;                          /* 0x10, the row's title picture 0x743, -1 = not made yet */
} LCPRODROW;
typedef struct LCPRODSLOT {             /* 0x38 bytes */
    int good;                           /* 0x00, the goods in the slot (an ai_Goods index), 0 = empty */
    int obj04;                          /* 0x04, -1 at the init (no reader read yet) */
    int x;                              /* 0x08, its place in the grid: (i % 4) * 0x40 + 0x10 */
    int y;                              /* 0x0c, (i / 4) * 0x40 + 0x10 (the second grid + 0xd0) */
    int check;                          /* 0x10, the goods' icon: an input icon with the amount to take (type 0x41) where the
                                           slots take input, else a picture; -1 = none */
    int label;                          /* 0x14, the side's label picture (0x747 buy / 0x746 sell), -1 = none */
    int priceText;                      /* 0x18, the sell price text, -1 = none */
    int buyPriceText;                   /* 0x1c, the buy price text, -1 = none */
    int slider;                         /* 0x20, the stock bar (amount / reserved of the stock); a click on it cycles the
                                           goods' reserve (AIOBJ.reserve 0 .. 4), -1 = none */
    int win;                            /* 0x24, a child window (its parent object moves with the slot), -1 = none */
    int obj28;                          /* 0x28, -1 at the init (no reader read yet) */
    int reserved;                       /* 0x2c, the stock kept from buyers: the store's stock * AIOBJ.reserve / 4 */
    int amount;                         /* 0x30, the amount in the store (gm_ListAmount) */
    int stock;                          /* 0x34, the store's stock of the goods (gm_ListStock) */
} LCPRODSLOT;
extern LCPRODROW ai_ProdRows[4];        /* 0x6f4f00, the rows of the first grid */
extern LCPRODSLOT ai_ProdGoods[16];     /* 0x6f4f60, the first grid: the goods of the types 0x17 / 0x25 */
extern LCPRODROW ai_ProdRows2[2];       /* 0x6f52e0, the rows of the second grid */
extern LCPRODSLOT ai_ProdGoods2[8];     /* 0x6f58a0, the second grid: the other goods */
extern LCPRODSLOT ai_ProdColumnSlots[16]; /* 0x6f5a80, a slot per production column (ai_InitProductionTables lays them out) */

/* the goods columns of the production window "Handel\Handel_PRODUKTION" (ai_ProductionWindow; ai_InitProductionTables sets
   them up, ai_UpdateResourceColumns builds their windows) */
typedef struct LCPRODCOL {              /* 0x58 bytes */
    int good;                           /* 0x00, the goods the column makes (an ai_Goods index) */
    int in[4];                          /* 0x04, its input goods (AIGOOD.in), 0 = none */
    int need[4];                        /* 0x14, the amount of each input one run takes (AIGOOD.need) */
    int x;                              /* 0x24, the column window's x (0x10) */
    int y;                              /* 0x28, its y (8 + column * 0x78) */
    int selected;                       /* 0x2c, the selected entry of the column, -1 = none (ai_UpdateProdColumns sets it from
                                           0x6f5a60 and compares it; reasoned guess from those uses) */
    int win;                            /* 0x30, the column's child window, -1 = not built yet */
    int pic;                            /* 0x34, the goods picture: a click on it (d2_PointerObj) picks the column */
    int bar;                            /* 0x38, the progress picture 0x4fd of the running job, -1 = none */
    int inPics[4];                      /* 0x3c, the pictures of the four inputs */
    int amountWin;                      /* 0x4c, the child window of the amount text */
    int maxAmount;                      /* 0x50, what the inputs in store make: the fewest runs over the inputs
                                           times AIGOOD.batch */
    int stock;                          /* 0x54, the amount of the goods in store */
} LCPRODCOL;
extern LCPRODCOL ai_ProdColumns[16];    /* 0x6f5320 */
extern int ai_ProdSelectedInput;        /* 0x6f5a60, the input goods picked by a click on a column's input picture, 0 = none
                                           (ai_ProductionGoodsWindow; LCPRODCOL.selected marks it) */
extern int ai_BauplatzCount;            /* 0x6f2688 */
extern int ai_DragSavedMaxY;            /* 0x6f268c, the mouse bounds kept while the town map is dragged */
extern int ai_DragSavedMinY;            /* 0x6f2690 */
extern int ai_MapScrollY;               /* 0x6f2694, the town map's scroll the key / edge scroll last computed */
extern int ai_DragSavedMaxX;            /* 0x6f2698 */
extern int ai_DragSavedMinX;            /* 0x6f269c */
extern int ai_MapScrollX;               /* 0x6f26a0 */
extern int ai_DragScrollX;              /* 0x6f26a4, the map window's scroll when the drag began */
extern int ai_DragScrollY;              /* 0x6f26a8 */
extern int ai_DragStartX;               /* 0x6f26b4, the mouse position the drag began at */
extern int ai_DragStartY;               /* 0x6f26b8 */
extern int ai_Dragging;                 /* 0x6f26bc, 1 while the town map is dragged */
extern int *ai_PlacementSites;          /* 0x6f26b0, the candidate site list of the running building placement (the map window marks each entry) */
extern char ai_CourtshipForm[24];       /* 0x62d40c, the form of the courtship (advertising) confirm window "privillegien\werbung2" (ai_CourtshipConfirm) */
extern char ai_WorkerDismissalForm[32];  /* 0x62d5dc, the form of the worker dismissal window "privillegien\arbeiter_ausweisen" (ai_WorkerDismissalWindow) */
extern char ai_MiracleForm[20];  /* 0x62d64c, the form of the miracle order's window "privillegien\wunder" (ai_MiracleWindow) */
/* The production settings chain (0x6f26c0, the head ai_Settings): one 0x4c-byte record per building, the seed at +4,
   the rotation byte at +8, seven {good, weight, lock} slots at +0xc (stride 8) and the factor at +0x44 */
typedef struct G8SLOT {                 /* one slot of a setting (stride 8) */
    int good;                           /* +0x00, the good id of the slot */
    unsigned char weight;               /* +0x04, the slot's weight in percent (ai_FillSettingWeights sums the seven to 100) */
    unsigned char lock;                 /* +0x05, 1 when the row is locked (ai_MasterLetterSettings toggles it) */
    char pad06[2];
} G8SLOT;
typedef struct G8SETTING {              /* 0x4c bytes, a production setting record (the next at +0x48) */
    int bldId;                          /* +0x00, the id of the building it is for (G8BLD.id; the chain's key, ai_FindSetting / ai_FindSettingSlots) */
    int seed;                           /* +0x04, the LCG state of the record's random */
    unsigned char rotate;               /* +0x08, the rotation index ai_SetSettingWeight advances */
    char pad09[3];
    G8SLOT slots[7];                    /* +0x0c, the seven good slots (ai_FindSettingSlots returns this table) */
    float factor;                       /* +0x44, the weight factor (1.0f at creation, ai_SettingFactor) */
    struct G8SETTING *next;             /* +0x48, the next setting of the chain (ai_FreeSettingChain frees it first) */
} G8SETTING;
typedef struct G8SETTAIL {              /* the tail a setting's slot table points at: the seven slots, then the factor */
    G8SLOT slots[7];
    float factor;                       /* +0x38 of the table (= +0x44 of the record), ai_SettingFactor */
} G8SETTAIL;
extern G8SETTING *ai_Settings;          /* 0x6f26c0, the head of the production settings chain, 0: none */
extern char ai_NewsText[1024];          /* 0x6f26c8, the shared text buffer of the unit's news messages (txt_Format into it, he_ShowNews reads it) */
extern char ai_NewsSecond[1176];        /* 0x6f2ac8, the second text buffer of the news messages that append a part */
extern struct AICHARREC *ai_InvChar;     /* 0x6f2f60, the character whose inventory the inventory child window lists */
/* 0x6f2f64: the transport and goods-reserve statics of the unit (one 0x1c-byte record; the fields were the
   ai_TransportEscort / lc_ReserveTime / lc_ReserveCursor / ai_EquipRowCount views of the gap block) */
typedef struct LCSETTINGS {
    char escort;                        /* +0x00, the escort kind of the last transport order (ai_TransportWindow clears it, the order sets it to the cart's AIOBJ.escort) */
    char pad01[3];
    AITIME reserveTime;                 /* +0x04, when lc_UpdateGoodsReserves ran last (set to ai_Time at its end) */
    char pad12[2];
    int reserveCursor;                  /* +0x14, the first character lc_UpdateGoodsReserves checks next (32 per call) */
    int equipRowCount;                  /* +0x18, the rows of the equip window in use (ai_UpdateEquipRows counts them each frame) */
} LCSETTINGS;
extern LCSETTINGS lc_Settings;          /* 0x6f2f64 */
extern unsigned short ai_StatsChartFlags; /* 0x6f2f88, the charts the statistics panel shows (bit i: the legend row i) */
extern unsigned int ai_AmountTab;        /* 0x6f2f8c, the open tab of the special-amount window (0 .. 7; 8: no tab has rows) */
extern int ai_MasterLetterPage;          /* 0x6f2f90, the open page of the master letter window: 0 the work settings, 1 the goods weights */
/* 0x6f3880: the equip window's row table, 32 rows of 0x40 (ai_EquipRowsInit frees every row, ai_UpdateEquipRows keeps
   the rows of the scene characters of the job groups 10 .. 12, ai_EquipWindow lays them out) */
typedef struct AIEQUIPSLOT {            /* 8 bytes, one equipment slot of a row: a good as an input icon */
    int pic;                            /* 0x00, the slot's picture object, -1 none */
    int obj;                            /* 0x04, the slot's object, 0 none */
} AIEQUIPSLOT;
typedef struct AIEQUIPROW {             /* 0x40 bytes, one row */
    struct AICHARREC *ch;               /* 0x00, the scene character shown, 0: a free row */
    int portrait;                       /* 0x04, the character's portrait picture object (AICHARREC.picture), -1 none */
    int frame;                          /* 0x08, the row's frame picture object (picture 0x74c), -1 none */
    int window;                         /* 0x0c, the row's child window (name, health and skill bars), -1 none */
    AIEQUIPSLOT slot[6];                /* 0x10, the six equipment slots: the character's goods as input icons */
} AIEQUIPROW;
extern AIEQUIPROW ai_EquipRows[0x20];   /* 0x6f3880, 32 rows; written until 0x6f4080 */
extern int ai_BldCartCount;             /* 0x6f40a0, the number of carts in ai_BldCarts (at most 3) */
extern struct AIOBJ *ai_BldCarts[3];    /* 0x6f40a4, the carts of the building the cart list shows (gm_BuildingCarts) */
extern int ai_BldCartButtons[3];        /* 0x6f40b0, per cart the button to its destination building, -1: none */
extern int ai_BldCartTexts[3];          /* 0x6f40bc, per cart its text control, -1: none */
typedef struct LCMASTERROW {            /* 0x18 bytes, a row of the stats panel's master list page (128 rows) */
    int pic;                            /* 0x00, the building type's picture, -1 none */
    int input;                          /* 0x04, the budget input box, -1 none (ai_PlayerStatsPanel compares its value
                                           with budget and sends a change to the building and its master) */
    int budgetText;                     /* 0x08, the master's budget text, -1 none */
    int nameText;                       /* 0x0c, the building's type and name text, -1 none */
    int budget;                         /* 0x10, the input box value last sent */
    struct AIBLD *bld;                  /* 0x14, the building */
} LCMASTERROW;
extern LCMASTERROW ai_MasterRows[0x80];  /* 0x6f40e0, the master list page's rows (ai_PlayerStatsPanel fills them) */
extern int ai_StatsBldRows[0x80];       /* 0x6f4ce0, the control of each row of the stats panel's building page (ai_ShowBuildingRows fills them) */
extern int ai_MasterBar;                /* 0x6f4ee0, the master list page's scroll bar (sv_CreateScrollButtons) */
extern unsigned char ai_GfxDetail;  /* 0x6f35c4, the "details" key */
void ai_OpenInventoryWindow(int win, int x, int y, unsigned short charId, int text);  /* 0x5ba1c0, opens the inventory child window of the character charId */
int ai_UpdateInventoryWindow(void);                    /* 0x5ba290, the inventory child window's frame (1 while goods are picked from it) */
void ai_CloseChildWindow(void);            /* 0x5ba600, closes the inventory child window */
extern int ai_ChildWindowId;            /* 0x62c418, the child window ai_OpenInventoryWindow creates and ai_CloseChildWindow removes, -1: none */
extern G9DOMIZIL ai_DomizilTable[0x42]; /* 0x62c420, the Domizil goods: 66 entries of 0x1c (id +0, minutes +4, law +8, weight +0xc, mask +0x10, use +0x14, rank +0x18) */

/* 0x6f22e4: the object clicked in the scene (cm.h declares it for the units that include cm.h) */
extern int ai_ClickedObject;
/* 0x6f231c: the permission bits of the player in the building the scene focus is on (bit 0x200 / 0x400
   offer the building's contact plates) */
extern int ai_BuildingPerms;
/* The text slots of the building contacts (the text system fills them at run time). */














/* 0x6f25b8: the local player's character id (the owner field of the building records) */
extern unsigned short ai_PlayerId;

/* the plate text ids of the shop-counter contacts (read by the window code as ints) */






/* 0x5ba6b0, the view of ai_InventoryHas: the character's inventory (AICHAR +0x178, ai.h); the object found in
   it is lc.c's G8INVOBJ */
#pragma pack(push, 1)
typedef struct G8INVCHAR {
    char pad00[0x178];
    int inventory;                      /* +0x178, the character's object list (AICHAR.inventory) */
} G8INVCHAR;

#pragma pack(pop)

void ai_FreeSettings(void);                                        /* 0x5a8920 */
int ai_LoadSettings(int f);  /* 0x5a8a20, reads the production settings chain back from the save f (ls_LoadGame); 0 on an error */
void ai_FreePicture(void);                                         /* 0x5bc0f0 */

/* declared in other units' headers before (moved to the owner) */
struct AICHARREC; struct LCORDMSG; int ai_DismissalHandler(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5ce950, the dismissal order's ask */
struct AICHARREC; struct LCORDMSG; int ai_BewitchOrder(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cacd0, the bewitch order's ask */
struct AICHARREC; struct LCORDMSG; int ai_DismissOfficeAsk(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5ca560 */
int ai_PenanceWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cca40, the penance order's ask */
struct AICHARREC; struct LCORDMSG; int ai_ExpelWorkerWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cbe00, the expel-worker order's ask */
int ai_MiracleWindow(void *ch, int *m);  /* 0x5cd2e0 */
struct AICHARREC; struct LCORDMSG; int ai_HearingWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cba90, the hearing order's ask */
int ai_ConvertWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cc700, the conversion order's ask */
struct AICHARREC; struct LCORDMSG; int ai_PeaceWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cc2b0, the peace order's ask */
int ai_ChangeProfessionWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cd400, the profession change */
struct AICHARREC; struct LCORDMSG; int ai_HatredWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cb4f0, the hatred order's ask */
void ai_Interrogate(struct AICHARREC *me, struct AICHARREC *ch);  /* 0x5ce4b0, carries out the interrogation */
int ai_InterrogationWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5ce610, the interrogation order's ask */
int ai_InterrogationWindow1(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cede0, the interrogation order's ask, variant 1 */
int ai_InterrogationWindow2(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cf020, the interrogation order's ask, variant 2 */
int ai_ChooseProfessionWindow(struct AICHARREC *parent, struct AICHARREC *child);  /* 0x5cf260, the profession chooser: parent apprentices child */
int ai_UniversityWindow(struct AICHARREC *parent, struct AICHARREC *child);  /* 0x5cf4f0, the university window: the player sends child to study */
struct AICHARREC; struct LCORDMSG; int ai_LawPrivilegeWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5ca150 */
struct AICHARREC; struct LCORDMSG; int ai_MisappropriationWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5ca860 */
struct LCORDMSG; int ai_CourtshipWindow(struct AICHARREC *ch, struct LCORDMSG *m);  /* 0x5c6d00, the courtship order's ask (lc.c LCORDMSG: gv.h ORDMSG) */
int ai_FaithAccusationWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5ce1a0, the accusation by faith (ai_AccusationWindow's twin) */
int ai_AccusationWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cde90, the accusation order's ask */
void ai_ShowCharacterNews(unsigned short *cRec, char *text);  /* 0x5a9110 */
void ai_ShowEvidenceVoidNews(unsigned short *toldRec, unsigned short *culpritRec);  /* 0x5a9430 */
void ai_TellGraveNeglect(unsigned short *plRec, int bld);  /* 0x5a9690 */
void ai_NotifySale(int owner, unsigned short *buyer, int bld, int size, int price, int share);  /* 0x5a96e0, the grave sale news to the graveyard's owner */
void ai_TellGravesPaid(int plRec, int bld, int paid, int sum);  /* 0x5a9780 */
void ai_DomizilMessage12(char *ch, char *b, int n);  /* 0x5a9e40, posts the Domizil message 12 (text 0x4071 + n) about the building b to the character ch */
struct AICHARREC; struct LCORDMSG; int ai_MedicusHandler(struct AICHARREC *ch, struct LCORDMSG *m);  /* 0x5c8bc0, the medicus order's ask (ord_MedicusOrder); m is not read */
int ai_DivorceDialog(struct AICHARREC *ch, struct LCORDMSG *m);  /* 0x5c8ef0, the divorce order's ask (ord.c 0x4f5800); m is not read */
int ai_WorkerDismissalWindow(void *ch, int *m);  /* 0x5ce7f0 */
typedef struct LCMAPMARK {         /* 0x18 bytes, a mark of the town map (LCMAPWIN.marks, rt's building marks) */
    int ctrl;                      /* 0x00, its picture control, -1: not placed yet */
    struct AIBLD *bld;             /* 0x04, the building it marks */
    int cellX;                     /* 0x08, its scene cell (d3_GetHeightAt); once placed its pixel */
    int cellZ;                     /* 0x0c */
    float x;                       /* 0x10, its town map pixel (ai_MapCellToScreen) */
    float y;                       /* 0x14 */
} LCMAPMARK;
typedef struct LCMAPWIN {          /* 0x27ac bytes, the town map window's record (ai_MapPick's local, rt's route editor) */
    int view;                      /* 0x000 */
    unsigned int flags;            /* 0x004, 4: show the ambush spots, 8: the building placement sites */
    int screen;                    /* 0x008, the map window */
    int req[0x14];                 /* 0x00c, the request record: its filter greys the buildings out
                                      (ai_PassesMapPickFilter) */
    char name[0x68];               /* 0x05c, the title */
    char pad0c4[0x18];
    LCMAPMARK marks[0x100];        /* 0x0dc, the buildings, sorted by their scene row */
    int controls[0x300];           /* 0x18dc, the control objects, -1 free (ai_BuildTownMap clears them,
                                      ai_CloseMapWindow removes them) */
    int siteCtrls[0x80];           /* 0x24dc, the controls of ai_PlacementSites' entries (flag 8; the original writes up to 0x100) */
    int spotCtrls[0x10];           /* 0x26dc, the controls of the ambush spots found (flag 4), packed */
    char pad271c[0x80];
    int scrolled;                  /* 0x279c, nonzero: the last frame dragged or key-scrolled the map (ai_UpdateTownMap) */
    unsigned long scrollTime;      /* 0x27a0, d3_Time of the last scroll; the mark info waits 75 ms after it */
    int nMarks;                    /* 0x27a4 */
    struct D2SURF *tile;           /* 0x27a8, the background tile the open saved (d2_BackgroundTile) */
} LCMAPWIN;
void ai_MapCellToScreen(LCMAPMARK *m, int originX, int originY);  /* 0x5a6b70, the town map pixel of a mark's scene cell */
void ai_BuildTownMap(LCMAPWIN *m);  /* 0x5a6cb0, places the marks of the town map window */
int ai_OpenTownMap(LCMAPWIN *m);  /* 0x5a78a0, opens the town map window */
void ai_UpdateTownMap(LCMAPWIN *m);  /* 0x5a7cd0, the town map window's per-frame update */
void ai_CloseMapWindow(int *m_);  /* 0x5a82f0 */
unsigned char *ai_FindItemGroup(char *good);  /* 0x5a84d0, the item group (am_types.h G7GRP) of a building's kind byte, 0 when none */
void ai_GypsyDeliveryText(void *from, int n, void **to);  /* 0x5a9470, the gypsys' delivery news of the sender from (a character) about its n targets to */
struct AICHARREC; struct LCORDMSG; int ai_BlackmailBuildingListener(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5c9550 */
struct AICHARREC; struct LCORDMSG; int ai_BlackmailContact(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5c9c50 */
struct AICHARREC; struct LCORDMSG; int ai_IntimidateContact(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5c9ff0 */
int ai_CounterEspionage(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cae00, the counter-espionage order's ask */
int ai_SeatSwapWindow(struct AICHARREC *me, struct LCORDMSG *m);  /* 0x5cb250, the seat swap order's ask */
int ai_AmountLimit(int param_1, int param_2, int param_3, int *param_4);  /* 0x5bd060 */
void ai_FamilyTreeWindow(int netId, unsigned char flags, int caption, int callId);  /* 0x5c4150, the family tree window */
struct AICHARREC;
void ai_CharInfoPanel(struct AICHARREC *chr, int other);  /* 0x5c5310, the character info panel */
void ai_ShowOfficeNews(unsigned short *param_1, char *param_2);  /* 0x5a8cb0 */
struct LCAMOUNTROW;
int ai_AmountRow(int x, int y, int win, int font, struct LCAMOUNTROW *row, int titled);  /* 0x5be3b0, one row of a special-amount list (lc.c LCAMOUNTROW) */
int ai_AmountRowStatus(struct LCAMOUNTROW *row);  /* 0x5be1d0, the corner icons of a special-amount row */
int ai_DrawAmountRow(int x, int y, int win, struct LCAMOUNTROW *row);  /* 0x5be720, a compact special-amount row at (x, y) (lc.c LCAMOUNTROW) */
void ai_GameFrame(void);  /* 0x591520 */
void ai_DropObjectAt(struct D3OBJECT *obj, struct D3VECTOR *target);  /* 0x58feb0 */
void ai_ShowPurchaseNews(unsigned short *param_1, char *param_2, int param_3, int param_4);  /* 0x5a8c50 */
void ai_NotifyPlayerOfChar(unsigned short *cRec, unsigned short *pRec);  /* 0x5a9170 */
void lc_UpdateGoodsReserves(void);  /* 0x5ba750 */
int ai_DrawSettingGood(int param_1);  /* 0x5a8870 */
float ai_SettingFactor(int param_1);  /* 0x5a88e0 */
void ai_ShowPartingNews(unsigned short *param_1, unsigned short *param_2);  /* 0x5a8ee0 */
void ai_ShowFeudNews(unsigned short *param_1, unsigned short *param_2);  /* 0x5a8f90 */
void ai_ShowPartnerNews(unsigned short *param_1, unsigned short *param_2);  /* 0x5a9050 */
int ai_GoodFitsBuilding(void *ch, int good);  /* 0x5bb2d0 */
int ai_FindBauplaetze(char *prefix, int *list);  /* 0x58f7a0 */
int ai_InventoryHas(void *ch, int id);  /* 0x5ba6b0 */
void ai_ObjectAngle(void *out_, int plot);  /* 0x58fd90 */
void ai_ObjectPosition(void *out, int plot);  /* 0x58fd40 */
void ai_ShowIndulgenceNews(unsigned short *param_1, char *param_2, int param_3, int param_4, int param_5);  /* 0x5a8b80 */
void ai_ShowPaymentNews(unsigned short *param_1, char *param_2, int param_3);  /* 0x5a8c00 */
int ai_FindUsableGood(int param_1);  /* 0x5ba620 */
int ai_PassesPartnerFilter(unsigned short me, unsigned short id, struct AMOUNTCFG *cfg, int unused);  /* 0x5c33c0, whether id passes the amount menu's partner filter cfg for me */
int ai_PassesBuildingFilter(struct AICHARREC *me, struct AMOUNTCFG *cfg, struct AIBLD *bld);  /* 0x5c37a0, whether bld passes the amount menu's building filter cfg for me */
struct G8USEVIEW; int ai_UseGood(struct AICHARREC *ch, struct G8USEVIEW *use);  /* 0x5cf8a0, uses a Domizil good: ch the character, use the request (am.h G8USE) */
void ai_SendSwapText(unsigned short *param_1, unsigned short *param_2, unsigned short *param_3);  /* 0x5a9330 */

int ai_SaveSettings(void *param_1);  /* 0x5a8940, saves the settings */

int ai_CourtshipRounds(int idA, int idB);  /* 0x5c6620, the courtship rounds the suitor idA needs for idB (2 .. 15) */
int ai_ApplyTrialStep(struct AICHARREC *ch, int skill);  /* 0x5d08c0, raises a skill of ch by a trial step (skill 0 .. 4) */
int ai_IsBuildingOpen(struct AIBLD *b, unsigned int *open, unsigned int *close);  /* 0x59e170, whether b is open now, its hours to *open / *close */
int ai_LiquidateAssets(int to, struct AICHARREC *ch, int amount, char *note);  /* 0x5b92e0, sells ch's assets for the amount, the receipt note */
int ai_BuildingHasItem(struct AIBLD *bld, int id);  /* 0x5ba700, whether bld holds an item of the type id */
int ai_CartAtBuilding(struct AIBLD *bld, struct AIOBJ *cart);  /* 0x5ae8e0, whether the cart is at its building */
unsigned short ai_DomizilGoodInRange(int from, int to);  /* 0x5a9f10, a Domizil entry whose rank lies in from .. to */

/* the unit's own contact listeners and window openers (defined in lc.c after their callers) */
void ai_UpdateProdColumns(int win, int part, int ncols, int store, int mode);  /* 0x58cc10 */
int ai_UpdateProdGoodsList(struct AIBLD *bld, struct AIOBJ *store);  /* 0x58d720 */
void ai_MarketStandListener(struct AIBLD *market);  /* 0x59a800 */
void ai_EnterBuilding(struct AIBLD *b, int mainRoom);  /* 0x59e420 */
void ai_EnterOwnBuilding(struct AIBLD *b);  /* 0x59ee40 */
void ai_TownCashContact(struct AIBLD *bld, int room);  /* 0x59fca0 */
void ai_TownHallContact(int building, int room);  /* 0x59fd30 */
void ai_LawBookContact(struct AIBLD *bld, int room);  /* 0x5a07d0 */
void ai_GuildLevel2Listener(int building, int room);  /* 0x5a18d0 */
void ai_GuildLevel3Listener(int building, int room);  /* 0x5a1960 */
void ai_GuildLevel1Listener(int building, int room);  /* 0x5a1a60 */
void ai_ConfessionListener(int building, int room);  /* 0x5a2de0 */
void ai_ProductionWriteContact(int building, int room);  /* 0x5a36c0 */
void ai_ChurchListener(int building, int room);  /* 0x5a3810 */
void ai_ChurchContact(int building, int arg);  /* 0x5a39b0 */
void ai_DungeonContact(int building, int room);  /* 0x5a3b20 */
void ai_DungeonBribeListener(struct AIBLD *bld, int room);  /* 0x5a3df0 */
void ai_DungeonEmptyListener(int building, int room);  /* 0x5a3ef0 */
void ai_ThievesGuildListener(int building, int room);  /* 0x5a4d40 */
void ai_ThievesGuildEmptyListener(int building, int room);  /* 0x5a4f50 */
void ai_StorageContact(int building, int room);  /* 0x5a4f70 */
void ai_TrainingContact(int building, int room);  /* 0x5a5080 */
void ai_KidnapContact(int building, int room);  /* 0x5a5bc0 */
void ai_CityWatchListener(struct AIBLD *bld, int room);  /* 0x5a67a0 */
void ai_GuildHallListener(struct AIBLD *bld, int room);  /* 0x5a6920 */
void ai_TrainingRoomListener(struct AIBLD *bld, int room);  /* 0x5a6a90 */
int ai_OpenTransportSell(int building);  /* 0x5af1b0 */
int ai_OpenTransport(int building);  /* 0x5af1a0, the transport list of the building (ai_TransportWindow mode 1) */
int ai_CreateCityPoint(char *city);  /* 0x59a2c0, creates the city point object of a town, 0 when its dummy is missing */
void ai_LoadTownMap(void);  /* 0x5b2090, loads the town map picture of the own player */
int ai_OpenTransportBuy(int building);  /* 0x5af1c0 */
int ai_MapPick(unsigned int flags, int *req, char *name, void (*cb)(), int unused);  /* 0x5b2180 */
void ai_OpenRobberBribe(int bld);  /* 0x5b4bf0 */
int ai_DrohbriefWindow(int bld);  /* 0x5b4fb0 */
void ai_LeftAlignedText(char *text, int x, int y, int w, int font);  /* 0x5bd1f0 */
int ai_SpecialAmountListWindow(int kind, unsigned int *filter, char *caption, char *caption2,
                               int (*pick)(struct AICHARREC *, int));  /* 0x5bea50 */
int ai_SpecialAmountWindow(unsigned int *filter, char *caption, int tab, int (*pick)(struct AICHARREC *, int));  /* 0x5c0970 */
int ai_PassesMapPickFilter(struct AICHARREC *me, struct AMOUNTCFG *cfg, struct AIBLD *bld);  /* 0x5c39a0 */
int ai_LawSelectionEntry(struct AIBLD *bld, struct AMOUNTCFG *filter, char *text, void (*cb)(struct AICHARREC *, struct AIBLD *));  /* 0x5c4100 */
int ai_GiftWindow(struct AICHARREC *giver, struct AICHARREC *chr);  /* 0x5c6050 */
void ai_HandleMapKey(void);  /* 0x591170, a key of the map window */
void ai_InventoryPanel(void);  /* 0x5bac10, the inventory panel */
void ai_PlayerStatsPanel(void);  /* 0x5bcc70, the player's stats panel */
void ai_CityStatsPanel(void);  /* 0x5c7f00, the city's stats panel */
void ai_PlayerComparePanel(void);  /* 0x5c82f0, the two players' comparison panel */
void ai_PrivilegesPanel(void);  /* 0x5c8640, the privileges panel */
#endif
