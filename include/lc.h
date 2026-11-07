/* lc.h -- the production / trade windows (unnamed): the table init and the "Handel\Handel_PRODUKTION"
   screen, see src/game/unk_58bbd0.c */
#ifndef LC_H
#define LC_H

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
int ai_AmountMenu(int cfg, int title, int text, int check);  /* 0x5c1130, the special-amount menu (cfg: an AMOUNTCFG, menu.h) */
void ai_PersonnelBook(struct AIBLD *bld, char *model);  /* 0x5aab80, the personnel book of a building (ob_PERSONALBUCH) */
void ai_MercenaryBook(struct AIBLD *bld, char *model);  /* 0x5ac110, the mercenary book of a building (ob_SOELDNERBUCH) */
void ai_HelperBook(struct AIBLD *bld, char *model);     /* 0x5ad150, the helpers' book of a building (ob_GEHILFENBUCH) */
void ai_MasterLetterWindow(unsigned char *building);    /* 0x5c3200, the master letter window */
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
extern char ai_EspionageOverviewForm[20];  /* 0x62d64c, the form of the espionage overview window "privillegien\wunder" (ai_EspionageOverviewHandler) */
extern char ai_NewsText[1024];          /* 0x6f26c8, the shared text buffer of the unit's news messages (txt_Format into it, he_ShowNews reads it) */
extern char ai_NewsSecond[1176];        /* 0x6f2ac8, the second text buffer of the news messages that append a part */
extern struct AICHARREC *ai_InvChar;     /* 0x6f2f60, the character whose inventory the inventory child window lists */
extern int ai_BldCartCount;             /* 0x6f40a0, the number of carts in ai_BldCarts (at most 3) */
extern struct AIOBJ *ai_BldCarts[3];    /* 0x6f40a4, the carts of the building the cart list shows (gm_BuildingCarts) */
extern int ai_BldCartButtons[3];        /* 0x6f40b0, per cart the button to its destination building, -1: none */
extern int ai_BldCartTexts[3];          /* 0x6f40bc, per cart its text control, -1: none */
extern unsigned char ai_GfxDetail;  /* 0x6f35c4, the "details" key */
void ai_OpenInventoryWindow(int win, int x, int y, unsigned short charId, int text);  /* 0x5ba1c0, opens the inventory child window of the character charId */
int ai_UpdateInventoryWindow(void);                    /* 0x5ba290, the inventory child window's frame (1 while goods are picked from it) */
void ai_CloseChildWindow(void);            /* 0x5ba600, closes the inventory child window */
extern int ai_ChildWindowId;            /* 0x62c418, the child window ai_OpenInventoryWindow creates and ai_CloseChildWindow removes, -1: none */

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
int ai_DismissalHandler(unsigned short *param_1, int param_2);  /* 0x5ce950 */
char ai_EmbezzlementContract(int param_1, int param_2);  /* 0x5cacd0 */
int ai_EmbezzlementPrivilege(unsigned short *param_1, int *param_2);  /* 0x5ca560 */
int ai_EspionageAmountWindow(int param_1, int *param_2);  /* 0x5cca40 */
int ai_EspionageOrderHandler(int param_1, int *param_2);  /* 0x5cbe00 */
int ai_EspionageOverviewHandler(void *ch, int *m);  /* 0x5cd2e0 */
int ai_EspionagePayment(int param_1, int param_2);  /* 0x5cba90 */
int ai_EspionagePayment2(int param_1, int *m);  /* 0x5cc700 */
int ai_EspionageRelation(int param_1, int *param_2);  /* 0x5cc2b0 */
int ai_EspionageResult(int param_1, int *m);  /* 0x5cd400 */
int ai_EspionageWindow(int param_1, int param_2);  /* 0x5cb4f0 */
int ai_InterrogationWindow(int param_1, int param_2);  /* 0x5ce610 */
int ai_InterrogationWindow1(int param_1, int param_2);  /* 0x5cede0 */
int ai_InterrogationWindow2(int param_1, int param_2);  /* 0x5cf020 */
int ai_LawPrivilegeWindow(int param_1, int *param_2);  /* 0x5ca150 */
int ai_MisappropriationWindow(unsigned short *param_1, int *param_2);  /* 0x5ca860 */
int ai_OfficeApplication(short *param_1, int param_2);  /* 0x5c6d00 */
int ai_SpyTargetFlagHandler(int param_1, int *param_2);  /* 0x5ce1a0 */
int ai_SpyTargetHandler(int param_1, int *param_2);  /* 0x5cde90 */
void ai_ShowCharacterNews(unsigned short *cRec, char *text);  /* 0x5a9110 */
void ai_ShowEvidenceVoidNews(unsigned short *toldRec, unsigned short *culpritRec);  /* 0x5a9430 */
void ai_TellGraveNeglect(unsigned short *plRec, int bld);  /* 0x5a9690 */
void ai_NotifySale(int owner, unsigned short *buyer, int bld, int size, int price, int share);  /* 0x5a96e0, the grave sale news to the graveyard's owner */
void ai_TellGravesPaid(int plRec, int bld, int paid, int sum);  /* 0x5a9780 */
void ai_DomizilMessage12(char *ch, char *b, int n);  /* 0x5a9e40, posts the Domizil message 12 (text 0x4071 + n) about the building b to the character ch */
int ai_MedicusHandler(unsigned short *param_1, void *unused);  /* 0x5c8bc0, the caller (ord 0x4f50ae) pushes a second argument the handler does not read */
int ai_Unk5c8ef0(unsigned short *param_1, void *unused);  /* 0x5c8ef0, the caller (ord 0x4f58a4) pushes a second argument the handler does not read */
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
int ai_BlackmailBuildingListener(unsigned short *param_1, int param_2);  /* 0x5c9550 */
int ai_BlackmailContact(int param_1, int param_2);  /* 0x5c9c50 */
unsigned char ai_BlackmailContactVariant(int param_1, int param_2);  /* 0x5c9ff0 */
int ai_CounterEspionage(int param_1, int *m);  /* 0x5cae00 */
int ai_CounterEspionageWindow(int *param_1, int param_2);  /* 0x5cb250 */
int ai_AmountLimit(int param_1, int param_2, int param_3, int *param_4);  /* 0x5bd060 */
void ai_FamilyTreeWindow(int netId, unsigned char flags, int caption, int callId);  /* 0x5c4150, the family tree window */
struct AICHARREC;
void ai_CharInfoPanel(struct AICHARREC *chr, int other);  /* 0x5c5310, the character info panel */
void ai_ShowOfficeNews(unsigned short *param_1, char *param_2);  /* 0x5a8cb0 */
struct LCAMOUNTROW;
int ai_AmountRow(int x, int y, int win, int font, struct LCAMOUNTROW *row, int titled);  /* 0x5be3b0, one row of a special-amount list (lc.c LCAMOUNTROW) */
int ai_AmountRowStatus(struct LCAMOUNTROW *row);  /* 0x5be1d0, the corner icons of a special-amount row */
int ai_DrawAmountRow(int x, int y, int win, int *row);  /* 0x5be720, draws the amount row row at (x, y) of the window win */
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
int ai_PassesPartnerFilter(unsigned short param_1, unsigned short param_2, unsigned int *param_3, int param_4);  /* 0x5c33c0 */
int ai_PassesBuildingFilter(short *param_1, char *param_2, unsigned char *param_3);  /* 0x5c37a0 */
int ai_UseGood(int param_1, int *param_2);  /* 0x5cf8a0 */
void ai_SendSwapText(unsigned short *param_1, unsigned short *param_2, unsigned short *param_3);  /* 0x5a9330 */

int ai_SaveSettings(void *param_1);  /* 0x5a8940, saves the settings */
#endif
