/* lc.h -- the production / trade windows (unnamed): the table init and the "Handel\Handel_PRODUKTION"
   screen, see src/game/unk_58bbd0.c */
#ifndef LC_H
#define LC_H

struct AIBLD;
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
extern char ai_NewsKeyHS46[20];         /* 0x62c2ac, the second text of ai_NotifyPlayerOfChar's news (event 0x5fa): the key "_NACHRICHTEN_HS_46" */
extern char ai_NewsKeyFriedhof12[28];   /* 0x62c364, the sound of the graveyard work news ai_TellGraveWorkSuccess (event 0x5fb): the key "_NACHRICHTEN_FRIEDHOF_12" */
extern char ai_NewsKeyFriedhof13[28];   /* 0x62c380, the sound of the graveyard work news (event 0x5fb): the key "_NACHRICHTEN_FRIEDHOF_13" */
extern char ai_NewsKeyFriedhof00[28];   /* 0x62c39c, the sound of the graveyard news ai_GraveyardMessage (event 0x5fa): the key "_NACHRICHTEN_FRIEDHOF_00" */

void ai_InitProductionTables(void);                /* 0x58bbd0, the production window's table init */
void ai_MarketContact(struct AIBLD *b); /* 0x58f510, the shop-counter contact of a building */
void ai_RunObjectRoom(unsigned short *room); /* 0x59f410, runs the room of an object record (its kind's handler) */
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
int ai_TransportWindow(int building, int mode);          /* 0x5af1d0, the transport window of a building (sv_UpdatePanel: mode 1) */
int ai_StorageWindow(int building, int list);           /* 0x5b26b0, the store window */
int ai_ProductionGoodsWindow(int building);             /* 0x593150, the goods / production window a kontor contact opens */
void ai_SpyRingWindow(int building);                    /* 0x5a45c0, the "locations\diebesgilde\diebesgilde_spionagering" window */
int ai_GelageWindow(int building);                      /* 0x5b9f30, the "special\gelage" window of the kontor contacts */
int ai_TrainingWindow(int building);                    /* 0x5bba00, the "special\training" window (regeneration / target / straw-dummy plates) */
int ai_EquipWindow(int building);                       /* 0x5bb350, the equip window of the AUSRUESTEN / weapon-stand plates */
int ai_BuildingWindow(int building);                     /* 0x5b84f0, the window the building panel's action button opens (reasoned) */
int ai_ManagedBuildingWindow(int building);              /* 0x5b8fc0, its variant for the classes 3 / 5 with the permission 0x10 (reasoned) */
int ai_AmountMenu(int cfg, int title, int text, int check);  /* 0x5c1130, the special-amount menu (cfg: an AMOUNTCFG, menu.h) */
int ai_PersonnelBook(int building, char *text);         /* 0x5aab80, the personnel book */
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
extern int *ai_PlacementSites;          /* 0x6f26b0, the candidate site list of the running building placement (the map window marks each entry) */
extern char ai_NewsText[1024];          /* 0x6f26c8, the shared text buffer of the unit's news messages (txt_Format into it, he_ShowNews reads it) */
extern char ai_NewsSecond[1180];        /* 0x6f2ac8, the second text buffer of the news messages that append a part */
extern unsigned char ai_GfxDetail;  /* 0x6f35c4, the "details" key */
extern int ai_ChildWindowId;            /* 0x62c418, the child window ai_Unk5ba1c0 creates and ai_CloseChildWindow removes, -1: none */

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
    int active;                         /* +0x21, nonzero when the object counts as held (reasoned guess: the
                                           reader ai_InventoryHas tests it; no writer of the field is drafted) */
} G8INVOBJ;
#pragma pack(pop)

void ai_FreeSettings(void);                                        /* 0x5a8920 */
void ai_FreePicture(void);                                         /* 0x5bc0f0 */

/* declared in other units' headers before (moved to the owner) */
int ai_DismissalHandler(unsigned short *param_1, int param_2);  /* 0x5ce950 */
char ai_EmbezzlementContract(int param_1, int param_2);  /* 0x5cacd0 */
int ai_EmbezzlementPrivilege(unsigned short *param_1, int *param_2);  /* 0x5ca560 */
int ai_EspionageAmountWindow(int param_1, int *param_2);  /* 0x5cca40 */
int ai_EspionageOrderHandler(int param_1, int *param_2);  /* 0x5cbe00 */
int ai_EspionageOverviewHandler(int param_1, int *m);  /* 0x5cd2e0 */
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
void ai_NotifySale(int param_1, unsigned short *param_2, int param_3, int param_4, int param_5, int param_6);  /* 0x5a96e0 */
void ai_TellGravesPaid(int plRec, int bld, int paid, int sum);  /* 0x5a9780 */
void ai_DomizilMessage12(char *ch, char *b, int n);  /* 0x5a9e40, posts the Domizil message 12 (text 0x4071 + n) about the building b to the character ch */
int ai_MedicusHandler(unsigned short *param_1, void *unused);  /* 0x5c8bc0, the caller (ord 0x4f50ae) pushes a second argument the handler does not read */
int ai_Unk5c8ef0(unsigned short *param_1, void *unused);  /* 0x5c8ef0, the caller (ord 0x4f58a4) pushes a second argument the handler does not read */
int ai_WorkerDismissalWindow(int param_1, int *m);  /* 0x5ce7f0 */
void ai_Unk5a6b70(int param_1, int param_2, int param_3);  /* 0x5a6b70 */
void ai_Unk5a6cb0(int *m_);  /* 0x5a6cb0 */
void ai_Unk5a78a0(int *m_);  /* 0x5a78a0 */
void ai_Unk5a7cd0(int *m_);  /* 0x5a7cd0 */
void ai_CloseMapWindow(int *m_);  /* 0x5a82f0 */
void ai_GypsyDeliveryText(unsigned short *param_1, int param_2, int *param_3);  /* 0x5a9470 */
int ai_BlackmailBuildingListener(unsigned short *param_1, int param_2);  /* 0x5c9550 */
int ai_BlackmailContact(int param_1, int param_2);  /* 0x5c9c50 */
unsigned char ai_BlackmailContactVariant(int param_1, int param_2);  /* 0x5c9ff0 */
int ai_CounterEspionage(int param_1, int *m);  /* 0x5cae00 */
int ai_CounterEspionageWindow(int *param_1, int param_2);  /* 0x5cb250 */
int ai_AmountLimit(int param_1, int param_2, int param_3, int *param_4);  /* 0x5bd060 */
void ai_FamilyTreeWindow(int param_1, unsigned char param_2, int param_3, int param_4);  /* 0x5c4150 */
void ai_ShowOfficeNews(unsigned short *param_1, char *param_2);  /* 0x5a8cb0 */
int ai_AmountRow(int param_1, int param_2, int param_3, int param_4, int *param_5, int param_6);  /* 0x5be3b0 */
int ai_AmountRowStatus(int *param_1);  /* 0x5be1d0 */
int ai_DrawAmountRow(int x, int y, int win, int *row);  /* 0x5be720, draws the amount row row at (x, y) of the window win */
void ai_GameFrame(void);  /* 0x591520 */
void ai_DropObjectAt(struct D3OBJECT *obj, struct D3VECTOR *target);  /* 0x58feb0 */
void ai_ShowPurchaseNews(unsigned short *param_1, char *param_2, int param_3, int param_4);  /* 0x5a8c50 */
void ai_NotifyPlayerOfChar(unsigned short *cRec, unsigned short *pRec);  /* 0x5a9170 */
int ai_Unk5ba750(void);  /* 0x5ba750 */
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
