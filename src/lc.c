/* lc -- the production / trade windows and the location windows of the game screen (0x58bbd0 .. 0x5a6b50, about
   190 functions); name unknown. Its .rdata literals end with 1600000.0 0x5eed10, its .data strings with
   "contact_RAZZIA" 0x62c25c, its .bss with ai_BauplatzCount 0x6f2688: the town map object lcmap (0x5a6b50), the
   Domizil object lcdom (0x5a9e90) and the special-amount object lcamt (0x5bd060) follow, each starting with named
   data; the descriptions below still cover their parts too. The
   contact listeners and the map picks of the kontor / thieves' guild / church / guild-hall scenes read their
   plate texts through the txt_views.h slots (ai_ContactText*) or their txt_Ptrs text id and the building record
   as AIBLD (id, owner, mainRoom, objList, object); the character record view AICHARREC names id, kind, netId,
   f01c, ailments, faith, relatives, officeKind, appliedOffice, secondOffice, donated and powerFlags. The
   guild-hall windows are the guild master's powers (amt_CanUseMayorPower2 / 3, amt_MayorBonusStep): the price
   message (level 1), the gift message through a map pick (level 2), and by the second office the character request
   (0x1e) or the amount-kind menus (0x1f / 0x21, 0x20) that send the district request {4, column} (level 3). The money
   lender windows read a bank's offers as AIBLD.credits (gv_types.h ORDCREDIT), its loans as HELOAN (ai_types.h) and
   its fees as AIBLD.messengerFee / exchangeFee; the exchange windows keep their currency rows as LCEXCHROW (16 per
   list), the money messenger sends AIMSG.payment and finds the running payment event by AIJOB.bldId / payer.
   The town map window (ai_OpenTownMap, ai_BuildTownMap, its per-frame ai_UpdateTownMap with the drag / key scroll, and
   its close) keeps its record as LCMAPWIN (lc.h, the marks LCMAPMARK; ai_MapPick and rt hand it in); the family tree window draws its boxes as LCFAMILYBOX
   and opens the character info panel ai_CharInfoPanel. The staff books (ai_PersonnelBook, ai_MercenaryBook,
   ai_HelperBook) keep their rows as G9DOMIZILROW and post the hire requests 6 / 7 as AIMSG.hireTitle / hireRows /
   hireMaster (the running event's AIJOB.hireTitle); the building's cart list (ai_ShowBldCarts / ai_UpdateBldCarts)
   keeps its carts in ai_BldCarts, and the transport window buys and sells carts (ai_BuyCartWindow, ai_SellCarts).
   The building options windows (ai_BuildingWindow, ai_ManagedBuildingWindow) open the sale, renovation, room upgrade
   (AIMSG.roomUpgrade), upgrade (message 0x1e), buy offer (0x21), fire fighting and technique effect windows.
   The thieves' guild windows: the equip window (ai_EquipWindow, its rows AIEQUIPROW in ai_EquipRows kept by
   ai_UpdateEquipRows) and the training window (ai_TrainingWindow, rows LCTRAINROW, the thief message 0x16 and
   AIJOB.thiefClass / thief); the player stats panel (ai_PlayerStatsPanel) and its pages (ai_OpenMoneyInfoPage,
   ai_OpenBuildingListPage with ai_ShowBuildingRows, ai_OpenMasterListPage with its rows LCMASTERROW, the building
   detail ai_BuildingDetailWindow) and the coat of arms choice; lc_CharRecs reads ai_Chars as AICHARREC. The
   special-amount lists build their rows (LCAMOUNTROW) with ai_ShowAmountRows / ai_ShowOfficeRows / ai_AmountRow /
   ai_DrawAmountRow and ai_AmountRowStatus (lc.h declares them on struct LCAMOUNTROW); the list window
   (ai_SpecialAmountListWindow) writes the corner icons in line, and the special-amount window's tab table
   ai_AmountTabs (AIAMOUNTTAB, typed fill / show handlers) fills its pages with ai_Fill*Tab (me, max (0: count
   only), rows, the partner filter) and draws them with ai_Show*Tab; the tabbed window ai_SpecialAmountWindow keeps
   the open tab in ai_AmountTab. The menus ai_AmountMenu (AMOUNTCFG, lc.h) and ai_AmountKindMenu (a kind callback:
   ai_AmountListWindow, ai_AgendaOfficeList) lead to them; the law book ai_OpenLawBook reads AIDEC records.
   The master letter (ai_MasterLetterWindow) opens ai_MasterHireWindow or ai_MasterLetterSettings, whose pages
   (ai_MasterLetterPage) edit the work settings LCWORKSET (AIBLD / AICHARREC.workSet, ai_ShowWorkSettingsPage) and the
   production setting's goods (ai_ShowSettingGoods). The partner, building and map-pick filters
   (ai_PassesPartnerFilter / ai_PassesBuildingFilter / ai_PassesMapPickFilter) read AMOUNTCFG: mode (the partner
   bits), classMask (the building classes), kind (the owner test), option14, ids and the liking range.
   The courtship order's ask (ai_CourtshipWindow, from ord.c with the order message as LCORDMSG, gv.h ORDMSG) reads
   the courtship event 0x41 as AIJOB.partner1 / pair*, lists the candidates (ai_CourtshipListWindow) and rates them
   (ai_CourtshipRounds); the statistics panel (ai_CityStatsPanel, its flags ai_StatsChartFlags) draws its five charts
   (ai_DrawStatisticsCharts: ai_PlotRatingBaseChart .. ai_PlotNoSecondChart) from the current ORDSTATS by value and
   ord_StatisticsHistory. The order asks (the medicus, the divorce, the blackmail and its contact letter, the
   intimidation, the law change, the dismissal, the embezzlement: ai_MedicusHandler .. ai_MisappropriationWindow)
   take the character and the order message (LCORDMSG: law, value, target / answerFlag, partner) as ord.c passes
   them, their AI path sending what the player's window sends (AIMSG.letter of 0x47, arg[] of 0x45 / 0x4b); the
   privileges panel lists the orders. The privilege asks of the "privillegien" forms (the hatred, the peace, the
   conversion, the penance, the miracle with its six outcomes ai_Miracle, the profession change) and the accusation
   (ai_AccusationWindow and its twin by faith ai_FaithAccusationWindow, the evidence rows ai_AccuseWindow) end in
   the trial message 0x2c (ai_OpenTrial). The interrogation asks (ai_InterrogationWindow -> ai_Interrogate, the
   variants ai_InterrogationWindow1 / 2) and the dismissal (ai_DismissalHandler) pick their holder through
   AMOUNTCFG and mark me in AICHARREC.flags1cc / powerFlags; the character info panel's apprenticeship and
   university windows send AIMSG.apprenticeship (0x5e) and AIMSG.study (0x5f).
   Open: types: the amount rows and tabs (ai_ShowAmountRows, ai_Fill*Tab, ai_SpecialAmountListWindow) and am.c's
   partner chain still pass that filter as unsigned int *.
   Open: the AIMSG message records are built by raw offsets in most functions (the use callbacks of the Domizil
   table use AIMSG: ai_UseEventGood, the money messenger AIMSG.payment).
   Open: types: the event / message words LCAMBUSHEVENT, LCSEARCHEVENT, LCSEARCHWORDS, LCKIDNAPEVENT (he.c's
   HEEV3C), LCRANSOMEVENT (he.c's HEEV3D) and LCOFFEREVENT (he.c's HEOFFER; no header holds these) are local views over AIJOB / AIMSG.arg (the gang leader, the group members and
   the watch plan words are written as AIMSG.arg[n]); a member added to ai_types.h's AIJOB or AIMSG unions moves the declaration-set matches of
   ord_CountGridObjects and ai.c's ai_Unk483a60 (both debt) and of sv_t's tutorial builders (tie-break).
   Open: types: the amount-menu config (AMOUNTCFG), the frame stack (SVFRAME, 0x75f780), the remaining character /
   object record views (G8CHARREC / G8OBJ of the news messages), the use callback's request (G8USEVIEW), the
   Domizil entry (G9DOMIZILRANGE), the scene character (G8SCENECHAR, ch.h CHARACTER), the equip window's rows
   (AIEQUIPROW / AIEQUIPSLOT), the window row pages and
   the gv record (AIWINDOWROW, AIPOSVIEW), the dice duel (LCDUEL / LCDUELSIDE, am.h AGDUEL, full layout), the
   player record (LCFIN, ai.h AIFIN) and the special-amount row (LCAMOUNTROW, cm.c CMAMOUNTROW) are local views here (menu.h / sv.h / ai_char.h / ai_types.h /
   am_types.h / am.h / ai.h hold the owners' types); they settle when those headers can be included. The "hp_" production
   settings are typed the same way: G8SETTING / G8SLOT / G8SETTAIL and G8BLD (a building's good id, gv.h's ORDBLD).
   The production window's tables are this unit's typed data (lc.h): the goods columns ai_ProdColumns (LCPRODCOL),
   the picked input ai_ProdSelectedInput and the goods grids ai_ProdRows / ai_ProdGoods / ai_ProdRows2 /
   ai_ProdGoods2 and the column slots ai_ProdColumnSlots (LCPRODROW / LCPRODSLOT); the member list rows are sv's SVMEMBERROW (sv_types.h, sv_MemberRows).
   Open: the other users of those tables (ai_InitProductionTables, the transport / storage / equip windows) still
   read them through byte offsets.
   Open: data: the shared float 0x5ebb40 (ai_FloatHundred, cm.c and he.c read it too) is the compiler's pooled
   100.0f: one copy in the image's constant pool, shared by 30 functions; a named array reproduces the loads but
   would add a second object, so its bytes stay in main.c's full_rdata gap block.
   Open: data: 0x6f2450 ai_DebugHotkeys (a view of main.c's full_data_6f2450), owner cm or main (the bss
   there interleaves f3init, cm, main and am: cm's 0x6f244c before, main's main_DevMenu 0x6f245c after; only
   this unit reads it).
   Open: data: 0x6f2314 sv_LastRoom, owner sv or f3gm (between sv's sv_PanelShown and f3gm's ai_PermsDirty; only
   ai_EnterBuilding writes it), named in main.c's gap block.
   The Domizil goods' clock at 0x6f2f64 is LCSETTINGS lc_Settings; the transport window's cart rows are LCCARTROW,
   the inventory window's character ai_InvChar (0x6f2f60, after ai_NewsSecond), its six rows ai_InvSlots
   (LCINVSLOT) and their objects G8INVOBJ (a view of AIOBJ: its union does not reach the
   use stamp at +0x2d).
   The TU boundary the use callbacks pointed at (our ai_FindUsableGood 0x5ba620 visible to ai_UseGood 0x5cf8a0) is
   lcamt's start 0x5bd060.
   Open: head: held by own declarations that differ from the owner's: ai_Time, d3sm_GetVertex, d2_CreateWinText, AIBLD, AIBTYPE, cm_SendExSetHE,
   d3sm_Pick; calls whose argument count differs from the definition (read the original's pushes): vfs_GetPath,
   d2_SetViewEnabled, d2_GetWinObj, d2_CreateWinText, d2_GetObjValue, d3_LoadGroup, d2_GetViewWin, sv_MessageBox,
   d2_CreateScrollButtons, d2_PlotLine, d2_PlaceMainWindow, d3t_PlayAnim, d3t_SetTreeColor. gv.h's cm_SendSetField
   (void) results the unit reads stay a cast; hmt.h's four prototypes (their unsigned short pushes) and am.h's
   ai_EvalFest stay copied in the bard / feast windows.
   unit of rea 8cef7d5b (src/game/unk_58bbd0.c); boundaries in decl/text.dcl */
#define GAPDATA __declspec(align(1)) unsigned char

#define sv_ReturnZeroV ((int (*)(int))sv_ReturnZero)  /* @original 0x5c4100 passes the research class as an argument (sv.h declares the function void) */
#define ai_MapPickCb(cb) ((void (*)(void *, void *))(cb))  /* @original: ai_MapPick's cb is void (*)() (lc.h, its callbacks take differing pointer types); both indirect calls push two dwords and read no result (0x5b22f6/0x5b22f7 push ebp twice; 0x5b248f/0x5b2490 push the picked building and picked) */
#define util_RandomS ((unsigned short (*)(unsigned short))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original the plague messages (ai_PlagueNews, ai_PlagueMessage, ai_PlagueMessageAll) read the 16-bit result (mov si,ax / and esi,0xffff), as f3gm.c's gm_Shuffled */
#define d2_WinOpenPicStyledS ((int (*)(short, short, int, int, int))d2_WinOpenPicStyled)  /* @original the production columns' input pictures pass x = k * 0x31 + 0x11 as a short (no movsx, computed afresh per call: no strength reduction), as sv.c / svtip.c do */
/* lc.c --the production / trade windows and the tavern / location / money lender / Rathaus windows
   (0x58bbd0 .. 0x5d33b0): the
   "Handel\Handel_PRODUKTION" screen of a building (0x58bd60) and its helpers (the table init 0x58bbd0, the
   goods columns 0x58c680 ..), then the building purchase / construction screens (0x58f510 .. 0x593960: the
   "contact_EINKAUF" / "BAUEN_GEB_BAUEN" strings), the trade search windows (0x595480 / 0x595a00), the contact
   listeners of the building rooms and locations (0x5950e0 .. 0x599cb0: the long-distance trade, smithy, carpenter,
   mixing, stonecutter and brewery rooms, the residence's social room with its feast / brawl / denounce plates and
   the Stammtisch of the tavern), the
   Wirtshaus / locations module (0x593e80 .. 0x599af0: "locations\diebesgilde", "locations\wohnsitz",
   "locations\Wirtshaus", the dice game and the dark corner), the money lender family
   ("locations\geldleihe\*", 0x59ad60 .. 0x59d5e0) and the Rathaus / guild rooms (0x59df10 ..: the enter check
   0x59e210 and the building driver 0x59e420 with its room dispatch switch).
   Unit start: 0x58bbd0 has no literals and is called from 0x58bd60 (and from the
   "HANDEL\HANDEL_SUCHEN" helper 0x584d70 of the previous unit); it initializes the tables 0x6f4f08 .. 0x6f5a64
   that 0x58bd60 / 0x58c680 use. The first literal of the unit is "Handel\Handel_PRODUKTION" 0x62b038 (0x58bd60),
   a +0x568 jump past the previous unit's last literal 0x62aad0 (data order D8); no literal belongs to 0x58bbd0
   itself, so the boundary rests on the subject and the call coupling.
   The rest of the unit, in address order: the church rooms, the dungeon and the thieves' guild (burglary,
   espionage, pickpocket, kidnapping), the city watch, the town map window and its production settings
   ("hp_" list 0x6f26c0), the plague / gypsy / graveyard / Domizil messages (their sound keys are the unit's
   the news key strings 0x62c2ac .. 0x62c400), the Domizil table 0x62c420 and its
   windows, the personnel / transport / storage windows, the player stats and special-amount panels, the law
   book and master letter, the statistics and privilege panels, the privilege / espionage / interrogation
   handlers and the trial / action callbacks reached only through the 0x62c420 records (+0x14). The literals
   climb in function order from 0x62b038 to 0x62d6bc as one .data block (D8), so no split was found inside;
   the next unit starts at 0x5d33b0.
   @named_by -
   @boundary plausible */
struct SBSAMPLE;
struct D3SM;
#include "txt_views.h"
#include <windows.h>
#include <d3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lc.h"
#include "di_input.h"
#include "evt.h"
#include "d3.h"
#include "d3_light.h"
#include "data_views.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
#define gypsyFilterWords (*(int (*)[2])&gypsyFilter)  /* 0x5eed68, the filter's two 32-bit words */
extern GAPDATA full_data_62b038[];
extern float gm_DayStartHours[];
extern float gm_DayEndHours[];
#include "d2_types.h"


extern struct D2OBJ *d2_Objs;
extern D2VIEW d2_Views[];
extern D2WIN d2_Wins[];
extern char * cm_FieldObject;
extern struct D2PIC *d2_Pics;
extern unsigned char kb_Last;

extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern unsigned char kb_Last;
extern char * ai_BTypes;
extern float gm_DayStartHours[];
extern float gm_DayEndHours[];
extern char * sv_SelectedObject;
extern int di_Absolute;
extern int di_MaxX;
extern int di_MaxY;
extern int di_MinX;
extern int di_MinY;
extern int d3t_BoxCornerIdx;  /* 0x5f3418 (d3_tools.c) */
extern int init_TerrainDivide;  /* 0x6f1c2c (f3init.h) */
extern int gm_TreeNodeCount;



extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

















extern int d2_ClickedId;




extern int d2_ClickedId;














extern char * ai_BTypes;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;


extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;






extern int ai_SabotageCallback();
extern int ai_BeatingCallback();



extern unsigned char kb_Last;



extern int d2_ClickedId;
extern int d2_LastWindow;


extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_602568[];
extern int ai_EspionageAmountCB();







extern GAPDATA full_data_6f2450[];
extern unsigned char kb_Last;
extern int d2_LastWindow;
extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern GAPDATA full_data_602568[];
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;



extern GAPDATA full_data_6f2450[];
#define ai_DebugHotkeys (*(int *)&full_data_6f2450)  /* 0x6f2450, a debug switch (no writer in the exe): set, the '-' key opens the mistress window in the study (ai_StudyContact) and the enter key posts the tavern message 0x74 (ai_TavernContact) */
extern unsigned char kb_Last;

extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern D2VIEW d2_Views[];
extern D2WIN d2_Wins[];
extern char * ai_PickedObject;

extern struct AIBLD *ai_MarketBuilding;
extern int ai_PermsDirty;
extern char * sv_SelectedObject;

extern char ai_PlayerSlots[4][0x2f4];









extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;

extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;


extern int d2_LastWindow;
extern struct D2PIC *d2_Pics;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;

extern unsigned char ai_OwnPlayer;

extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;

extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;
extern char ai_PlayerSlots[4][0x2f4];

extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;



extern int d2_ClickedId;
extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[4][0x2f4];
extern char ch_EmptyName[1];
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;


#include "ai_types.h"
extern int d2_ClickedId;
extern int d2_LastWindow;



extern char * ai_BTypes;

extern int d2_CutsceneActive;
extern int snd_RoomBank;
extern char * ai_BTypes;
#include "menu_types.h"
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern int d2_LastWindow;




extern unsigned char kb_Last;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[4][0x2f4];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;



extern int gm_PlayerRecords[688];
extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;









extern char * ai_BTypes;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;


extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;




extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;




extern char ai_Chars[768][0x21c];

extern char ai_Chars[768][0x21c];


extern char ai_Chars[768][0x21c];

extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;
extern char ai_Chars[768][0x21c];


extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;
extern GAPDATA full_data_602568[];





extern unsigned char kb_Last;
extern char ai_Chars[768][0x21c];
extern int gm_PlayerRecords[688];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;


extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;


extern char * ai_BTypes;
extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;

extern char * ai_BTypes;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_602568[];
extern int ai_SceneChars[32];


extern int ai_SceneChars[32];
extern int d2_ClickedId;










extern char ai_Chars[768][0x21c];
extern int d2_ClickedId;


extern char * cm_FieldObject;
extern int d2_ClickedId;


extern char ai_Chars[768][0x21c];
extern char * cm_FieldObject;
extern GAPDATA full_data_627978[];




extern int ai_CityWatchWindow();

extern int ai_CityWatchCrewWindow();

extern int ai_CityWatchWindow();
extern int ai_WatchPlanWindow();
extern int d2_ClickedId;

extern int ai_CityWatchWindow();
extern int ai_CityWatchCrewWindow();
extern int d2_ClickedId;

extern int ai_CityWatchCrewWindow();
extern int ai_WatchPlanWindow();
extern int ai_SceneChars[32];
extern int d2_ClickedId;

extern int ai_WatchPlanWindow();
extern int ai_SceneChars[32];
extern int d2_ClickedId;














extern char ai_Chars[768][0x21c];
extern int ai_MapSize;
extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[4][0x2f4];
extern char cm_EventNames[115][0x20];
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;
extern D2WIN d2_Wins[];
extern struct D2SURF *rt_MapSurface;
extern short d2_MouseX;
extern short d2_MouseY;
extern int di_Absolute;
extern int di_MaxX;
extern int di_MaxY;
extern int di_MinX;
extern int di_MinY;
extern int ai_MapScrollEnabled;
extern D2VIEW d2_Views[];
#include "am_types.h"
#include "lcdom.h"
extern char ch_EmptyName[1];



extern char ch_EmptyName[1];
extern int ai_DomizilCount;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;

extern GAPDATA full_data_60ea68[];
extern unsigned char kb_Last;

extern char ch_EmptyName[1];
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern char * ai_BTypes;
extern GAPDATA full_data_60d568[];

extern char ch_EmptyName[1];
extern struct D2OBJ *d2_Objs;

extern struct AIBLD *ai_MarketBuilding;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

extern GAPDATA full_data_602568[];
extern struct AIBLD *ai_MarketBuilding;
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;

extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;

extern GAPDATA full_data_60ea68[];
extern unsigned char kb_Last;
extern int sv_FrameMask;
extern char ai_Chars[768][0x21c];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;
#pragma pack(push, 1)
typedef struct G8INVOBJ {               /* an object of a character's inventory (AIOBJ, ai_types.h, read as a held good:
                                           ai_InventoryHas, ai_BuildingHasItem, the inventory window, the Domizil
                                           goods' clock lc_UpdateGoodsReserves); AIOBJ's union does not reach +0x2d */
    unsigned short id;                  /* +0x00, the object's type id (AIOBJ.id) */
    int objId;                          /* +0x02, AIOBJ.objId */
    int ownerId;                        /* +0x06, the id of the character holding it (AIOBJ.ownerId; the Domizil clock
                                           sells the used-up good from it) */
    char pad0a[4];
    int count;                          /* +0x0e, how many (AIOBJ.count; the inventory window's amount control) */
    char pad12[0xa];
    int minutesLeft;                    /* +0x1c, of a Domizil good in use: the minutes its effect still runs
                                           (lc_UpdateGoodsReserves counts it down from G9DOMIZIL.minutes) */
    char inUse;                         /* +0x20, of a Domizil good: set to 1 by ai_UseGood with minutesLeft (the name is
                                           a reasoned guess) */
    int active;                         /* +0x21, nonzero when the object counts as held, of a Domizil good while its
                                           effect runs (ai_InventoryHas tests it; lc_UpdateGoodsReserves clears it
                                           when minutesLeft runs out; the name is a reasoned guess) */
    int bldId;                          /* +0x25, the id of the building the object stands in (reasoned guess:
                                           ai_BuildingHasItem compares it with AIBLD.id) */
    char pad29[4];
    AITIME since;                       /* +0x2d, of a Domizil good in use: when minutesLeft was last counted down */
} G8INVOBJ;
#pragma pack(pop)
#define d2_OpenFormS ((int (*)(short, short, char *))d2_OpenForm)  /* @original: the transport window computes x = d2_ScreenW - 0x68 in 16 bits (0x5af336 mov ax, [d2_ScreenW]; sub ax, 0x68; push eax), as sv_OpenPlayerBar */
extern char * ai_BTypes;
extern GAPDATA full_data_60c28c[];
extern int ai_MapSize;
extern char ai_PlayerSlots[4][0x2f4];
extern struct D2SURF *rt_MapSurface;
extern char ch_EmptyName[1];
extern int ai_SabotageCallback();
extern int ai_EspionageAmountCB();
extern int ai_BeatingCallback();
extern char * ai_PickedObject;
extern char * cm_FieldObject;
extern struct D2OBJ *d2_Objs;



extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern char ai_Chars[768][0x21c];
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2PIC *d2_Pics;
extern GAPDATA full_data_60d224[];
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_602568[];
extern int d2_ClickedId;

extern int d2_ClickedId;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2WIN *d2_LastWindowPtr;
extern struct D2OBJ *d2_Objs;


extern int d2_ClickedId;
extern GAPDATA full_data_625380[];
extern GAPDATA full_data_62bdc8[];
extern GAPDATA full_data_62bde4[];
extern GAPDATA full_data_62be00[];
extern char * ai_BTypes;
extern unsigned char ai_OwnPlayer;
extern int d2_LastWindow;
extern GAPDATA full_data_602568[];
extern struct D2OBJ *d2_Objs;
extern char * cm_FieldObject;


extern char * ai_BTypes;
extern char ai_Chars[768][0x21c];
extern signed char ai_Relations[768][768];
#include "cm_types.h"
extern G17MSG cm_SendRing[];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern int sv_FrameMask;
extern unsigned char ai_OwnPlayer;
extern unsigned gm_OwnNameHeads[136];
#include "gv_types.h"
#include "ai_bld.h"                /* AIBLD, AIBTYPE */
extern GAPDATA full_data_5fe7d0[];
extern char ai_PlayerSlots[4][0x2f4];

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_602568[];
extern unsigned char ai_OwnPlayer;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern char ai_Chars[768][0x21c];
extern GAPDATA full_data_624204[];
extern int gm_PlayerRecords[688];
extern char cm_EventNames[115][0x20];
extern unsigned char ai_OwnPlayer;
extern char * ai_BTypes;
extern char ai_Crimes[];

extern int d2_ClickedId;

extern int d2_ClickedId;

extern char ch_EmptyName[1];
extern int d2_ClickedId;

extern char ch_EmptyName[1];
extern int d2_ClickedId;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern struct D2PIC *d2_Pics;
extern int sv_FrameMask;
extern char ai_Chars[768][0x21c];
extern GAPDATA full_data_60d224[];
extern unsigned char ai_OwnPlayer;
extern char * cm_FieldObject;
extern signed char ai_Relations[768][768];

int sprintf(char *buf, const char *fmt, ...); /* 0x5de652, the CRT's sprintf */

struct AMTREC;  /* file-scope tag for the prototypes below */

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_PickActionBuilding)(p, a, fl, b, c, f, o)

struct AITIME;  /* f3gm.h typedef; file-scope tag for the gm_AddTime prototype */


#define U_I(a) (*(int *)(a))
#define U_W(a) (*(unsigned short *)(a))

#pragma pack(push, 1)
typedef struct AITASKVIEW {       /* the character's model record fields this unit reads (ai_char.h AITASK) */
    char pad000[0x2c];
    int bldId;                     /* 0x2c, the building the character was sent to (ai_char.h AITASK.bldId) */
    int scriptGroup;               /* 0x30, the group of the script that created / moved it (the room object's id) */
} AITASKVIEW;

typedef struct LCWORKSET {       /* the work settings a building keeps at AIBLD +0x99 (ai_bld.h AIBLD.workSet) and a
                                      hired worker takes over into AICHAR.flags1b4 .. target (ai_char.h; cm_ExHire
                                      copies them as 16 bytes); the master letter window passes them by value */
    unsigned int flags;            /* +0x0, the order flags (bit 0 .. 3: the page's four checkboxes) */
    int budget;                    /* +0x4, AICHAR.budget */
    int prod;                      /* +0x8, AICHAR.prod (the item the workshop makes) */
    int target;                    /* +0xc, AICHAR.target (a building id or -1) */
} LCWORKSET;
typedef struct AICHARREC {         /* the character record fields this unit reads (ai_char.h AICHAR, same layout;
                                      G8CHARREC below is the view of the news messages) */
    unsigned short id;             /* 0x000, the character's id (AIBLD.owner refers to it) */
    unsigned char kind;            /* 0x002, the character kind (ai_char.h AICHAR.kind; 6 / 7 need no cell model) */
    char pad003;
    unsigned int netId;            /* 0x004, the character's network id (ai_char.h AICHAR.netId) */
    unsigned char health;          /* 0x008 */
    unsigned char sex;             /* 0x009, 0 / non-0 (ai_char.h AICHAR.sex; it picks the male / female name table) */
    unsigned short w00a;           /* 0x00a, the age in years (ai_char.h AICHAR.w00a, gm_CharAge; the family tree
                                      window lets a child above 10 be picked) */
    unsigned char faith;           /* 0x00c, the faith, 0 / 1 (ai_char.h AICHAR.b00c): the baptism window sets it to
                                      1 - it (or to the church owner's) and names it by the text 0x197a + it; the
                                      church donation reaches the characters of the owner's faith */
    unsigned char b00d;            /* 0x00d, 1 .. 2 (ai_char.h AICHAR.b00d, gv.c copies it to the grave's v30) */
    char pad00e[0xe];
    float f01c;                    /* 0x01c, the life value (ai_char.h AICHAR.f01c: 1000.0 at creation, aged
                                      by gm_AgeCharacter, the character dies when it is used up) */
    float f020;                    /* 0x020, ai_char.h AICHAR.f020 (the character info panel offers a jobless child
                                      the profession choice when it is at most the age + 4) */
    int v024;                      /* 0x024, ai_char.h AICHAR.v024: below 0 the character is hurt (ord.c's medicus
                                      order tests it below -20); the medicus heals half of it (gm_AddHealth) */
    char pad028[4];
    unsigned int ailments;         /* 0x02c, the ailment counters (am.h G5CH.ailments; ai_char.h AICHAR.v02c):
                                      the fields at bits 0, 4, 8, 12, 14, 17, 20, 23, 25 (4, 4, 4, 2, 3, 3, 3, 2, 4
                                      bits), the daily step 0x4fbe90 halves each, the diseases 2 .. 9 sit in the
                                      field pairs of gm_CatchMinorDisease / gm_CatchUpperDisease */
    char name[0x20];               /* 0x030, the character's name (ai_char.h AICHAR.name) */
    unsigned short w050;           /* 0x050, the family's grave word (ai_char.h AICHAR.w050); the family tab takes the
                                      characters with the same w050 and b058 */
    char pad052[2];
    int ref54;                     /* 0x054, a reference word the related characters share (ai_char.h AICHAR.ref54;
                                      the game frame compares it with the player's); a picture id, 0x59a + the
                                      dynasty's colour 0 .. 7 (ai_PlayerComparePanel shows it and colours the
                                      slider by it) */
    unsigned char b058;            /* 0x058, compared with w050 by the family tab (ai_FillFamilyTab) */
    char pad059[3];
    unsigned int relatives[8];     /* 0x05c, the netIds of the character's relatives (ai_char.h AICHAR.relatives;
                                      the baptism window converts [3..7] with the player) */
    char pad07c[4];
    unsigned char skills[5];       /* 0x080, the five skill values (ai_char.h AICHAR.skills; the personnel book shows
                                      [3] and [2] as bars) */
    char pad085[0xdf];
    unsigned char jobId;           /* 0x164, the character's job (ai_char.h AICHAR.jobId; the construction window's group 7
                                      needs 0x34 .. 0x39) */
    unsigned char title;           /* 0x165, the title rank (ai_char.h AICHAR.title; text 0x20d / 0x230 + it) */
    unsigned char officeKind;      /* 0x166, the office the character holds, 0 = none (ai_char.h AICHAR.officeKind) */
    char pad167;
    unsigned char appliedOffice;   /* 0x168, the office kind applied for, 0 = none (ai_char.h AICHAR.appliedOffice) */
    unsigned char secondOffice;    /* 0x169, the second office kind, the deputy one (ai_char.h AICHAR.secondOffice) */
    char pad16a[2];
    struct AIBLD *bld;             /* 0x16c, the building the character is in (ai_char.h AICHAR.bld) */
    int bld2;                      /* 0x170, the character's own building, 0 when none (ai_char.h AICHAR.bld2) */
    int v174;                      /* 0x174, 0 when hired (ai_char.h AICHAR.v174); the personnel book shows it with
                                      the texts 0x1a39 / 0x1a3a */
    int inventory;                 /* 0x178, the character's goods inventory, an object list (ai_char.h AICHAR.inventory) */
    struct AIJOB *job;             /* 0x17c, the job the character works in, 0 = none (ai_char.h AICHAR.job) */
    char pad180[4];
    struct AITASKVIEW *model;      /* 0x184, the character's model record, 0 when none (ai_char.h AICHAR.model) */
    unsigned char selected;        /* 0x188, nonzero: in the player's selection (ai_char.h AICHAR.selected) */
    char pad189[3];
    int picture;                   /* 0x18c, the portrait picture (ai_char.h AICHAR.picture) */
    char pad190[4];
    int money;                     /* 0x194, the character's money (ai_char.h AICHAR.money) */
    char pad198[0x14];
    int donated;                   /* 0x1ac, the money the character gave the church: ai_IndulgenceWindow adds the price
                                      (cm_AddFieldValues 0x1ac) and shows it with the text 0x1964 */
    char pad1b0;
    unsigned char heldRounds;      /* 0x1b1, the rounds the character is held (ai_char.h AICHAR.heldRounds) */
    char pad1b2[2];
    union {
        LCWORKSET workSet;         /* 0x1b4, the work settings the character took over from its building (ai_char.h
                                      AICHAR.flags1b4 .. target; the master letter window edits them) */
        struct {
            unsigned int flags1b4; /* 0x1b4, the order flags (ai_char.h AICHAR.flags1b4) */
            int budget;            /* 0x1b8, the money the character may spend (ai_char.h AICHAR.budget; main_EndRound
                                      copies the building's budget to its jobs; the master list page shows it) */
        };
    };
    char pad1c4;
    unsigned char b1c5;            /* 0x1c5, ai_char.h AICHAR.b1c5 (a worker value am.c averages per building; the
                                      Domizil good 0x161 halves it, rounded up, for everyone in the building) */
    char pad1c6[2];
    unsigned int powerFlags;       /* 0x1c8, the office-power bits already used (ai_char.h AICHAR.powerFlags) */
    unsigned int flags1cc;         /* 0x1cc, ai_char.h AICHAR.flags1cc (0x800: a bribe raises the relation by 6 instead of 4) */
    char pad1d0[0x1c];
    unsigned int goodsMask;        /* 0x1ec, the classes of the Domizil goods in use (G9DOMIZIL.mask; ai_UseGood sets them) */
    int w1f0;                      /* 0x1f0, ai_char.h AICHAR.v1f0 (0 at creation; the partner filter bit 0x80 wants it set) */
    char pad1f4[0x1c];
    int partner;                   /* 0x210, the netId of the partner (ai_char.h AICHAR.partner; the amount rows mark
                                      a kind 6 viewer's partner) */
    char pad214[8];
} AICHARREC;                       /* 0x21c bytes, ai_Chars[768] */
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct LCFIREEVENT {       /* the fire event 0x3d's word over AIJOB's union (its alloc 0x564860; the jail check
                                      0x591ad0 reads the burning building here) */
    char pad00[0xac];
    int bld;                       /* 0xac, the building that burns, -1 / 0: none */
} LCFIREEVENT;
typedef struct LCAMBUSHEVENT {     /* the ambush event 0x48's word over AIJOB's union (he.c HEAMBUSH; ai_BreakInCallback
                                      looks for one at a place) */
    char pad00[0xac];
    char place[0x20];              /* 0xac, the name of the scene object the gang lies in wait at */
} LCAMBUSHEVENT;
typedef struct LCSEARCHEVENT {     /* the search event 0x14's words over AIJOB's union (ai_TradeSearchWindow counts them) */
    char pad00[0xac];
    unsigned char count;           /* 0xac, how many characters search */
    char padad;
    unsigned short good;           /* 0xae, the good searched for */
} LCSEARCHEVENT;
typedef struct LCSEARCHWORDS {     /* the search message 0x14's words at AIMSG +0x58 (msg.arg) */
    unsigned char count;           /* 0x58, how many of ids[] are set */
    char pad59;
    unsigned short good;           /* 0x5a, the good searched for */
    int room;                      /* 0x5c, the objId of the room the search starts from */
} LCSEARCHWORDS;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct AIROOM {            /* the room object sv_PanelShown / the driver's room parameter points to (ai_types.h AIOBJ) */
    unsigned short id;             /* 0x00, the room (scene record) id */
    int objId;                     /* 0x02, the object's own id (AIOBJ.objId) */
    char pad006[0xe];
    int list;                      /* 0x14, its object list (ai_types.h AIOBJ.list; ai_StorageWindow / gm_SumListAmounts) */
} AIROOM;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct G8USEVIEW {              /* a local view of am.h's G8USE (am.h cannot be included here yet): the
                                           record ai_UseGood hands to a Domizil record's use callback (+0x14) */
    G8INVOBJ *obj;                      /* 0x00, the inventory object used (G8USE.obj; its +0 id) */
    int target;                         /* 0x04, the target reference (G8USE.target) */
    int arg;                            /* 0x08 */
    int effect;                         /* 0x0c, 1 the use took effect, 0 not, -1 backfired (G8USE.effect) */
    int handled;                        /* 0x10, set by a callback that reported the use itself (G8USE.handled) */
} G8USEVIEW;
#pragma pack(pop)


#include "ls.h"
#include "f3init.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_interface.h"
#include "d3_sky.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "d3s.h"
#include "err.h"
#include "f3gm.h"
#include "he.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "sblib.h"
#include "tb.h"
#include "tr.h"
#include "d3_pose.h"
#include "sv_t.h"
#include "sv_types.h"
#include "amt_types.h"
#include "ch.h"
#include "hmt_types.h"
#include "msx.h"
#include "rd.h"
#include "am.h"
#include "amt.h"
#include "gv.h"
#include "hst.h"
#include "hmt.h"
#include "mission.h"
#include "ai.h"
#include "anm.h"
#include "chc.h"
#include "d3_int_io.h"
#include "d3_tools.h"
#include "f3snd.h"
#include "vfs.h"
#include "d2.h"
#include "rt.h"
#include "gfx.h"
#include "cm.h"
#include "ahm.h"
#include "menu.h"
#include "sv.h"
#include "sv_quickjump.h"
#define d2_CreatePicObjS ((int (*)(short, short, int))d2_CreatePicObj)  /* 0x46cd80 (d2.h: int x, y): @original the town map update adds the window position to the mark pixel in 16 bits (0x5a8052 mov dx, word ptr [eax + 4] / add dx, di / push edx), as d2.c's callers */
#define d2_SetObjValue2 ((void (*)(int, int))d2_SetObjValue)  /* 0x475cb0 (d2.h: five parameters): @original the checkbox / route-box calls push only the object and the value (0x5c81ab ai_CityStatsPanel: push esi; push edi; 0x5b0a1b ai_TransportWindow: push 0; push esi) */
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue)  /* 0x475cb0 (d2.h: five parameters): @original the range calls push four (0x5c2610 ai_ShowWorkSettingsPage: push *budget; push 0x2328; push 0x3e8; push eax; 0x5c2675: push eax; push 0x96; push 0x32; push esi) */
/* 0x58bbd0, sets up the production window's tables: the grid rows ai_ProdRows / ai_ProdRows2 (no title picture),
   the 16 goods columns ai_ProdColumns (no inputs, x 0x10, y 8 + i * 0x78, no windows or pictures), the column
   slots ai_ProdColumnSlots (at 0x1a / 0x10 + i * 0x45) and the goods grids ai_ProdGoods / ai_ProdGoods2 (four
   slots a row, 0x40 apart from 0x10 / 0x10 resp. 0xd0), every picture and window -1 */

void ai_InitProductionTables(void)
{
    int i;
    int j;

    for (i = 0; i < 4; i++) {
        ai_ProdRows[i].shown = -1;
        ai_ProdRows[i].title = -1;
    }
    for (i = 0; i < 2; i++) {
        ai_ProdRows2[i].shown = 0;
        ai_ProdRows2[i].title = -1;
    }
    for (i = 0; i < 16; i++) {
        ai_ProdColumns[i].good = 0;
        for (j = 0; j < 4; j++) {
            ai_ProdColumns[i].in[j] = 0;
            ai_ProdColumns[i].need[j] = -1;
        }
        ai_ProdColumns[i].x = 0x10;
        ai_ProdColumns[i].y = i * 0x78 + 8;
        ai_ProdColumns[i].selected = -1;
        ai_ProdColumns[i].win = -1;
        ai_ProdColumns[i].pic = -1;
        ai_ProdColumns[i].bar = -1;
        for (j = 0; j < 4; j++)
            ai_ProdColumns[i].inPics[j] = -1;
        ai_ProdColumns[i].amountWin = -1;
    }
    ai_ProdSelectedInput = 0;
    for (i = 0; i < 16; i++) {
        ai_ProdColumnSlots[i].good = 0;
        ai_ProdColumnSlots[i].obj04 = -1;
        ai_ProdColumnSlots[i].x = 0x1a;
        ai_ProdColumnSlots[i].y = i * 0x45 + 0x10;
        ai_ProdColumnSlots[i].check = -1;
        ai_ProdColumnSlots[i].label = -1;
        ai_ProdColumnSlots[i].priceText = -1;
        ai_ProdColumnSlots[i].buyPriceText = -1;
        ai_ProdColumnSlots[i].obj28 = -1;
        ai_ProdColumnSlots[i].slider = -1;
        ai_ProdColumnSlots[i].win = -1;
        ai_ProdColumnSlots[i].stock = -1;
    }
    for (i = 0; i < 16; i++) {
        ai_ProdGoods[i].good = 0;
        ai_ProdGoods[i].obj04 = -1;
        ai_ProdGoods[i].x = (i % 4) * 0x40 + 0x10;
        ai_ProdGoods[i].check = -1;
        ai_ProdGoods[i].label = -1;
        ai_ProdGoods[i].priceText = -1;
        ai_ProdGoods[i].buyPriceText = -1;
        ai_ProdGoods[i].y = (i / 4) * 0x40 + 0x10;
        ai_ProdGoods[i].obj28 = -1;
        ai_ProdGoods[i].slider = -1;
        ai_ProdGoods[i].win = -1;
        ai_ProdGoods[i].stock = -1;
    }
    for (i = 0; i < 8; i++) {
        ai_ProdGoods2[i].good = 0;
        ai_ProdGoods2[i].obj04 = -1;
        ai_ProdGoods2[i].x = (i % 4) * 0x40 + 0x10;
        ai_ProdGoods2[i].check = -1;
        ai_ProdGoods2[i].label = -1;
        ai_ProdGoods2[i].priceText = -1;
        ai_ProdGoods2[i].buyPriceText = -1;
        ai_ProdGoods2[i].y = (i / 4) * 0x40 + 0xd0;
        ai_ProdGoods2[i].obj28 = -1;
        ai_ProdGoods2[i].slider = -1;
        ai_ProdGoods2[i].win = -1;
        ai_ProdGoods2[i].stock = -1;
    }
}

struct D3VECTOR; int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps); int d2_OpenForm(); void d2_CentreViewWindows(int view);
float gm_GoodsPrice(unsigned id, unsigned char p);   /* 0x5089c0, the price (ai.h / cm.h / rt.h: float) */
void gm_FormatAmount(int v, unsigned char k, char *out);  /* 0x508a80, above its first call (line 1119) */
int d2_SetForm(int view, int n); int io_PrintText(char *fmt, ...);
int d2_GetViewWin(); void d2_SetPointerSprite(int a, int pic); char * gm_Query(void *list, int n, ...);
int gm_FreeCapacity(); char * he_FindEvent(int n, ...); char * he_NextEvent(void);
int ai_ScriptQuery(); int cm_SendLongMessage(char *msg); int cm_GetSlotState(); int cm_GetSlotValue(int h);
void snd_PlayFavourComment(void); void d2_CloseView(int view);
int gm_ListAmount(); char * gm_Query(void *list, int n, ...);
void ai_LayoutProdGoodsList(int param_1, int param_2, int param_3, int param_4); char *itoa(int, char *, int); void d2_SetObjText();

#define sv_MemberRows (*(SVMEMBERROW (*)[0x20])&ai_Unk75f280)  /* 0x75f280, sv.c's member list rows (32 of SVMEMBERROW) */

/* 0x58bd60, the production window "Handel\Handel_PRODUKTION" of the building bld (2335 B): returns 0 at once when
   sv_CheckCharacterFlag refuses; opens the form (tutorial step 0x1f), the player bar (2 for the building class
   0x1e), the frame by the building class (texts 0x72f / 0x70f / 0x713 / 0x717, else 0x70b) and the scroll buttons.
   Every frame it finds the building's two rooms (gm_Query 0xff, the room of the goods, and 0x2a, the store; it
   ends when one is gone) and handles a click on a column's goods picture: with members of the player's list
   (sv_MemberRows) marked it starts the production job 4 of that goods (the messages 0xbe / 0xbf when the store
   has no room for a batch, 0xc0 when an input is short; ai_ScriptQuery 0x20 confirms, the order message type 4
   goes out with cm_SendLongMessage) and moves the marked members into it (sv_MoveToGroup); with none marked it
   selects the workers of the building's job of that goods. A right click clears the marks (or pops the window).
   Then it collects the goods of the first room into ai_ProdColumns (new kinds only, at most 16), computes each
   column's inputs, the amount the stock of inputs makes and the stock of the goods, shows the count and lays
   out the columns and the goods list; on the way out it saves the scroll position in bld->prodScroll
   @deep s49: 151 -> 127 ins. Lever: fresh = 1 inside the count > 1 test (the byte sibling
   ai_ExtractResourceWindow's form; 151 -> 127). The original's class test is an AIBTYPE pointer with
   type->kind read at each test (lea eax,[edi+eax*4]; mov al,[eax]: typeptr-nocls fixes that hunk, 126 ins)
   but in our current allocation it raises the size 2335 -> 2337, so not kept. Left (allocation): the
   original keeps bld in ebp from the prologue (mov ebp,[esp+0x134] before push esi), win in esi across the
   frame loop (cmp [d2_ClickedForm],esi), the click loop's column offset c and its countdown in two slots
   ([esp+0x1c] / [esp+0x20], frame 0x128 vs ours 0x124: ours has the offset in ebp, c2trace k#519 prio -426
   still takes ebp); the member loop's character index premultiplied (shl eax,2; mov
   ecx,[eax+ai_Chars.job]) where ours forms ch. No lever (s49): ch written out at each use (the sibling's
   form: 211-239), the second column loop as an index or p = &ai_ProdColumns[c] per pass (= control), g
   written out at each use (frame right 0x128 but 296+: the original has g as a pointer, esi, and the k
   loop walks &g->in[k]), int cls (272), climb round 1 (305 mutants, nothing comparable: frame size).
   prior: reconstructed from the original (2335 B, was a 615 B skeleton): the click on a column picture
   (the member-row any-loop, the job-worker selection, the capacity / input checks with txt 0xbe/0xbf/0xc0,
   the job search, the order message AIMSG type 4 with its cm_GetSlotState wait, sv_MoveToGroup), the
   right-click branch, the column collection (gm_Query 1,5 / gm_QueryNext, ncols < 16), the per-column
   inputs with min(...)/need and batch, the stock, the count text, ai_UpdateProdColumns /
   ai_UpdateProdGoodsList / ai_LayoutProdGoodsList in the original order, the scroll save. Same size (2335
   vs 2335), same calls, no ref differences. All code hunks read: allocation only - the original keeps bld
   in ebp for the whole function and uses ebx as a zero register in the order branch (ours reloads bld from
   the argument slot, 0x58c0ed: '- mov ebx, [esp+0x138]' / '+ mov eax, [ebp+1]; xor ebx, ebx'), keeps win
   in esi across the frame loop (0x58bf39 '- mov edx,[esp+0x28]; cmp eax,edx' / '+ cmp [X], esi') and the
   column loop as an offset + countdown in two slots (0x58bf87, frame 0x128 vs ours 0x124); the character
   lookups index ai_Chars by eax*4 + field where ours forms a pointer (0x58bfc4, writing it without the
   pointer cost 50 more instructions); 0x58be23 the class byte through lea+mov al. */
int ai_ProductionWindow(AIBLD *bld)
{
    int win;
    int ncols;
    int any;
    int c;
    int k;
    int ok;
    int fresh;
    int first;
    int text;
    unsigned char cls;
    int slot;
    int down;
    int up;
    int bar;
    char count[4];
    AIOBJ *room;
    AIOBJ *store;
    AIOBJ *item;
    AIGOOD *g;
    AIJOB *job;
    int i;
    LCPRODCOL *col;
    AICHARREC *ch;
    AIMSG msg;

    first = 1;
    if (sv_CheckCharacterFlag(bld) != 0)
        return 0;
    ai_CampaignStep(0x1f, 0, 0, 0);
    win = d2_OpenForm(0, 0, "Handel\\Handel_PRODUKTION");
    d2_CentreViewWindows(win);
    if (((AIBTYPE *)ai_BTypes)[bld->type].kind == 0x1e)
        sv_OpenPlayerBar((int)bld, 2);
    else
        sv_OpenPlayerBar((int)bld, 0);
    d2_SetForm(win, 3);
    io_PrintText(0xb7);
    ai_InitProductionTables();
    ncols = 0;
    d2_SetForm(win, 0);
    text = 0x70b;
    cls = ((AIBTYPE *)ai_BTypes)[bld->type].kind;
    if (cls == 0x13 || cls == 4 || cls == 0x10)
        text = 0x72f;
    else if (cls == 8 || cls == 0xe)
        text = 0x70f;
    else if (cls == 6)
        text = 0x713;
    else if (cls == 0x16)
        text = 0x717;
    else if (cls == 0xd || cls == 0xc || cls == 0xb)
        text = 0x717;
    sv_OpenWindowFrame(d2_LastWindow, text, 0x742);
    sv_CreateScrollButtons(0x270, 0x15e, d2_LastWindow, d2_GetViewWin(win, 2), 0x78, &down, &up, &bar);
    d2_Objs[bar].font = 0x54;
    d2_SetPointerSprite(0, 0);
    do {
        d2_SetForm(win, 2);
        room = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0xff);
        if (room == 0)
            return 0;
        store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
        if (store == 0)
            return 0;
        if (d2_ClickedId != -1 && d2_ClickedForm == win) {
            any = 0;
            for (i = 0; !any && i < 0x20 && sv_MemberRows[i].chr != 0xffff; i++)
                if (sv_MemberRows[i].selected != 0)
                    any = 1;
            for (c = 0; c < ncols; c++) {
                if (d2_PointerObj != ai_ProdColumns[c].pic)
                    continue;
                if (!any) {
                    /* nobody marked: select the workers of the building's job of this goods */
                    for (i = 0; i < 0x20; i++) {
                        if (sv_MemberRows[i].chr == 0xffff)
                            break;
                        ch = &((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr];
                        job = ch->job;
                        if (job != 0 && job->kind == 4 && job->from == bld->id && (ch->powerFlags & 0x800) == 0 &&
                            job->u.w.lo == ai_ProdColumns[c].good)
                            ch->selected = 1;
                    }
                    continue;
                }
                ok = 1;
                g = &((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good];
                if (gm_FreeCapacity(store, ai_ProdColumns[c].good, g->batch) == 0) {
                    if (g->type == 0x25 || g->type == 0x17)
                        sv_MessageBox(txt_Ptrs[0xbe], 0);
                    else
                        sv_MessageBox(txt_Ptrs[0xbf], 0);
                    continue;
                }
                for (k = 0; k < 4; k++) {
                    if (g->in[k] == 0)
                        break;
                    if (g->in[k] != 0xffff) {
                        item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, g->in[k]);
                        if (item == 0 || item->count < g->need[k])
                            ok = 0;
                    }
                }
                if (!ok) {
                    sv_MessageBox(txt_Ptrs[0xc0], ok);
                    continue;
                }
                for (job = (AIJOB *)he_FindEvent(2, 0, 4, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                    if (job->u.w.lo == ai_ProdColumns[c].good)
                        break;
                if (job == 0 && ai_ScriptQuery(0x20, bld, 0, ai_ProdColumns[c].good) == 1) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 4;
                    if (bld->owner != 0xffff)
                        msg.to = ai_CharInts[bld->owner][0];
                    else
                        msg.to = ai_CharInts[ai_PlayerId][0];
                    msg.fromId = bld->id;
                    msg.time = ai_Time;
                    msg.itemId = (unsigned short)ai_ProdColumns[c].good;
                    msg.nIds = 0;
                    msg.reserved14 = 0;
                    msg.flags = 1;
                    msg.arg[1] = -1;
                    msg.arg[4] = bld->id;
                    msg.arg[5] = store->objId;
                    slot = cm_SendLongMessage((char *)&msg);
                    while (slot != -1 && cm_GetSlotState(slot) == 0)
                        sv_RunFrame(0, 0x20007);
                    job = (AIJOB *)cm_GetSlotValue(slot);
                    ai_CampaignStep(0x20, 0, 0, 0);
                }
                snd_PlayFavourComment();
                for (i = 0; i < 0x20; i++) {
                    if (sv_MemberRows[i].chr == 0xffff)
                        break;
                    if (sv_MemberRows[i].selected != 0) {
                        sv_MoveToGroup((unsigned char *)bld, (short *)sv_PanelShown, sv_MemberRows[i].chr, (char *)job);
                        ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                    }
                }
                if (job != 0 && job->stage == 0)
                    job->stage = 1;
            }
        }
        if (dm_L.rup != 0) {
            any = 0;
            for (i = 0; !any && i < 0x20 && sv_MemberRows[i].chr != 0xffff; i++)
                if (sv_MemberRows[i].selected != 0)
                    any = 1;
            if (any) {
                for (i = 0; i < 0x20; i++) {
                    if (sv_MemberRows[i].chr == 0xffff)
                        break;
                    ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                }
            } else if (ai_RunAllowsWindow() == 1) {
                sv_PopRequest = 1;
                sv_CameraFollow();
            }
        }
        /* the columns: each kind of goods in the room once */
        item = (AIOBJ *)gm_Query((void *)room->list, 1, 5);
        if (item != 0) {
            do {
                if (item->count > 1) {
                    fresh = 1;
                    for (k = 0; fresh && k < ncols; k++)
                        if (ai_ProdColumns[k].good == item->id)
                            fresh = 0;
                    if (fresh && ncols < 16)
                        ai_ProdColumns[ncols++].good = item->id;
                }
                item = (AIOBJ *)gm_QueryNext();
            } while (item != 0);
        }
        for (c = 0, col = ai_ProdColumns; c < ncols; c++, col++) {
            col->maxAmount = 0x7fffffff;
            for (k = 0; k < 4; k++) {
                col->in[k] = ((AIGOOD *)ai_Goods)[col->good].in[k];
                col->need[k] = ((AIGOOD *)ai_Goods)[col->good].need[k];
                if (col->in[k] == 0)
                    continue;
                item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, col->in[k]);
                if (item == 0)
                    col->maxAmount = 0;
                else
                    col->maxAmount = min(col->maxAmount, gm_ListAmount(store, item) / ((AIGOOD *)ai_Goods)[col->good].need[k]);
            }
            col->maxAmount = ((AIGOOD *)ai_Goods)[col->good].batch * col->maxAmount;
            item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, col->good);
            if (item != 0)
                col->stock = gm_ListAmount(store, item);
            else
                col->stock = 0;
        }
        itoa(ncols, count, 10);
        d2_SetObjText(bar, count);
        d2_Objs[bar].centred = 1;
        d2_Objs[bar].w = 0x30;
        ai_UpdateProdColumns(win, 2, ncols, store, 0);
        if (first) {
            d2_SetForm(win, 2);
            first = 0;
            d2_Wins[d2_LastWindow].scrollY = d2_Wins[d2_LastWindow].visible * bld->prodScroll;
        }
        ai_UpdateProdGoodsList(bld, store);
        ai_LayoutProdGoodsList(win, 1, 0, (int)bld);
    } while (sv_RunFrame((int)ai_ProductionWindow, 0x677c7) != 0);
    d2_SetForm(win, 2);
    bld->prodScroll = (char)((d2_Wins[d2_LastWindow].scrollY + d2_Wins[d2_LastWindow].scrollRemY) / d2_Wins[d2_LastWindow].visible);
    d2_CloseView(win);
    sv_ClosePlayerBar();
    ai_CampaignStep(0x1f, 0, 1, 0);
    return 0;
}

int gm_BuildingGoodsList(char *b, unsigned *out, int kind); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win); struct AIPER; struct D3VECTOR; int d2_WinOpenPic(short x, short y, int pic, int win); int d2_WinOpenPicStyled();
void d2_RemoveObj(); int gm_MinutesBetween(); char * he_FindEvent(int n, ...); char * he_NextEvent(void);
int d2_SetForm(int view, int n); int io_PrintText(char *fmt, ...);

/* 0x58c680, shows the ncols goods columns (ai_ProdColumns) of the resource window in the form win / its part form:
   builds a column on first use (its child window 0xcd x 0x57 at (x, y), the frame 0x749, the goods picture 0x74a
   for column 0 else the goods' picture 0xdf + good, the input pictures, the amount window); marks each input by
   the goods list of the player's building (ai_PlayerBuilding, else ai_FallbackBuilding): 0x74d enough, 0x74e short,
   0x74f none, the column picture's icon 0x750 + the worst; shows the progress of the running job (kind 9 for
   column 0, else the kind 8 job of the column's goods: the progress picture 0x4fd and the fill fraction of the
   elapsed time) and the amount text (200 / 0xc9); then removes the columns from ncols up to 16. The fourth
   argument is not read
   @deep size queue s16: the input pictures' x goes through a short-x view (d2_WinOpenPicStyledS, as sv.c /
   svtip.c): the original pushes k * 0x31 + 0x11 without movsx and computes it per call (110 -> 91 ins, raw
   1429 -> 1427, padded still 1440 vs 1424: 3 bytes over). Left: the original holds 0x54 in ebp for the two
   font stores and loads -1 into ecx per iconx / icony pair; ours makes -1 the ebp candidate (4 refs). No
   lever: iconx = icony = -1 / icony = iconx = -1 chains (= control), icony copied from iconx (worse),
   climb round 1 (its best was a float local, a meaning change). prior: rewritten from the original (was a
   condensed skeleton, 1097 B; now 1429 vs 1421, 408 vs 406 instructions, same calls): the column build
   (child window, font 0x54, frame 0x749, goods picture, input pictures with (short)(k*0x31+0x11), amount
   window), the input marks from the AIRECIPE goods list (0x74f none / 0x74e short / 0x74d enough, icon
   0x750 + worst), the job progress (kind 9 for column 0, else the kind 8 job of the goods; progress
   picture 0x4fd, fillFraction = minutes(start, now) / minutes(start, end)), the amount texts 200 / 0xc9,
   the removal of the columns up to 16 (the inPics loop the original unrolls through a pointer). All hunks
   read: allocation - the original holds 0x54 in ebp for the two font stores and -1 in ecx per store (ours:
   -1 in ebp, 0x54 immediate: 0x58c734 '+ mov ebp, 0x54' / 0x58c7a1 '- or ebp, -1'), keeps k in ebx with
   the x recomputed (lea/shl) where ours strength-r... */
void ai_UpdateResourceColumns(int win, int part, int ncols, int unused)
{
    AIBLD *bld;
    AIJOB *job;
    int c;
    int k;
    int j;
    int worst;
    int amount;
    float total;
    AIRECIPE list;

    bld = 0;
    if (ai_PlayerBuilding != 0)
        bld = (AIBLD *)ai_PlayerBuilding;
    else if (ai_FallbackBuilding != 0)
        bld = (AIBLD *)ai_FallbackBuilding;
    d2_SetForm(win, 0);
    d2_SetForm(win, part);
    gm_BuildingGoodsList((char *)bld, (unsigned *)&list, 1);
    for (c = 0; c < ncols; c++) {
        if (ai_ProdColumns[c].win < 0) {
            ai_ProdColumns[c].win = d2_CreateChildWindow(ai_ProdColumns[c].x, ai_ProdColumns[c].y, 0xcd, 0x57, 0x10, d2_LastWindow);
            d2_Wins[ai_ProdColumns[c].win].font = 0x54;
            d2_WinOpenPic(0, 0, 0x749, ai_ProdColumns[c].win);
            if (c == 0)
                ai_ProdColumns[0].pic = d2_WinOpenPic(0xd, 5, 0x74a, ai_ProdColumns[0].win);
            else
                ai_ProdColumns[c].pic = d2_WinOpenPic(0xd, 5, ai_ProdColumns[c].good + 0xdf, ai_ProdColumns[c].win);
            d2_Objs[ai_ProdColumns[c].pic].interactive = 1;
            d2_Objs[ai_ProdColumns[c].pic].iconx = -1;
            d2_Objs[ai_ProdColumns[c].pic].icony = -1;
            for (k = 0; k < 4; k++) {
                if (ai_ProdColumns[c].in[k] != 0) {
                    ai_ProdColumns[c].inPics[k] = d2_WinOpenPicStyledS(k * 0x31 + 0x11, 0x3a, ai_ProdColumns[c].in[k] + 0xdf, ai_ProdColumns[c].win, -2);
                    d2_Objs[ai_ProdColumns[c].inPics[k]].interactive = 1;
                    d2_Objs[ai_ProdColumns[c].inPics[k]].iconx = -1;
                    d2_Objs[ai_ProdColumns[c].inPics[k]].icony = -1;
                }
            }
            ai_ProdColumns[c].amountWin = d2_CreateChildWindow(0x43, 4, 0x88, 0x32, 0x10, ai_ProdColumns[c].win);
            d2_Wins[ai_ProdColumns[c].amountWin].font = 0x54;
        }
        worst = 0;
        for (k = 0; k < 4; k++) {
            if (ai_ProdColumns[c].in[k] == 0)
                continue;
            for (j = 0; j < list.count; j++)
                if (list.item[j] == ai_ProdColumns[c].in[k])
                    break;
            if (j >= list.count) {
                d2_Objs[ai_ProdColumns[c].inPics[k]].icon = 0x74f;
                worst = 2;
            } else if (list.amount[j] < ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].need[k]) {
                d2_Objs[ai_ProdColumns[c].inPics[k]].icon = 0x74e;
                if (worst <= 1)
                    worst = 1;
            } else {
                d2_Objs[ai_ProdColumns[c].inPics[k]].icon = 0x74d;
            }
        }
        d2_Objs[ai_ProdColumns[c].pic].icon = worst + 0x750;
        amount = 0;
        if (c == 0) {
            job = (AIJOB *)he_FindEvent(2, 0, 9, 3, bld->id);
        } else {
            for (job = (AIJOB *)he_FindEvent(2, 0, 8, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                if (job->u.w.lo == ai_ProdColumns[c].good)
                    break;
        }
        if (job != 0 && (job->words14[0] != 0 || job->words14[1] != 0)) {
            if (ai_ProdColumns[c].bar < 0)
                ai_ProdColumns[c].bar = d2_WinOpenPic(0xd, 5, 0x4fd, ai_ProdColumns[c].win);
            amount = job->words14[1] + job->words14[0];
            total = (float)gm_MinutesBetween(&job->start, &job->end);
            d2_Objs[ai_ProdColumns[c].pic].fillFraction = (float)gm_MinutesBetween(&job->start, &ai_Time) / total;
        } else {
            if (ai_ProdColumns[c].bar != -1) {
                d2_RemoveObj(ai_ProdColumns[c].bar);
                ai_ProdColumns[c].bar = -1;
            }
            if (ai_ProdColumns[c].pic != -1)
                d2_Objs[ai_ProdColumns[c].pic].fillFraction = 0;
        }
        if (c == 0) {
            d2_LastWindow = ai_ProdColumns[0].amountWin;
            io_PrintText(200, amount, 0x51e);
        } else {
            d2_LastWindow = ai_ProdColumns[c].amountWin;
            io_PrintText(0xc9, amount, 0x51e, ai_ProdColumns[c].good * 2 + 0x9a8);
        }
        d2_SetForm(win, part);
    }
    for (; c < 16; c++) {
        if (ai_ProdColumns[c].win >= 0) {
            ai_ProdColumns[c].selected = -1;
            d2_RemoveObj(ai_ProdColumns[c].win);
            ai_ProdColumns[c].win = -1;
            ai_ProdColumns[c].pic = -1;
            ai_ProdColumns[c].bar = -1;
            for (k = 0; k < 4; k++)
                ai_ProdColumns[c].inPics[k] = -1;
            ai_ProdColumns[c].amountWin = -1;
        }
    }
}

int gm_BuildingClass(); int d2_WinOpenPicStyled(); void d2_SetObjFont(); void d2_SetCaption();
int gm_ClockSeason(); int ls_GetLanguage(); int d2_CountPointerItems(); void cm_BeginFieldBlock(char *o, int id);
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldValues(void); void ai_ShowProdSlot(int view, int part, LCPRODSLOT *slot, int input, unsigned char side, AIBLD *bld); void d2_MoveObj(short x, short y, int obj);
void d2_SetPointerItem(int id, int amount); void d2_ClearPointerItems(void);

/* 0x58cc10, shows the ncols goods columns (ai_ProdColumns) of the production window in the form win / its part form,
   for the player's building (ai_PlayerBuilding, else ai_FallbackBuilding): marks in each column the input picked
   (ai_ProdSelectedInput, LCPRODCOL.selected); builds a column on first use (the frame 0x74c, the goods picture,
   the input pictures with the amount each run takes as their caption, the amount window); marks each input by the
   building's goods list (0x74d enough, 0x74e short, 0x74f none; the column picture's icon 0x750 + the worst) and
   keeps how much of the picked input is missing; shows the running job of the goods (production job 4, else the
   income job 3: the picture 0x4fc by day while job 4 waits on nothing, else 0x50b / 0x50c, its progress as the
   fill fraction and its amount); then the amount text: without a picked input the texts 0xb8 / 0xba / 0xbb by
   mode, with one it highlights it and prints the batch (0xc1 / 0xc2 in the language's word order), the amount of
   the input a run takes (0xc3 / 0xc4) and what is missing (0xc5 / 0xc6). Removes the columns from ncols up to 16
   @deep size queue s16: the input pictures' x through the short-x view d2_WinOpenPicStyledS (no movsx, no
   strength reduction, as the original): 186 -> 161 ins; raw 2802 vs 2827 (frame 0xd8 vs 0xf0 left: 0x18
   bytes of locals missing). prior: rewritten from the original (was a condensed draft, 1812 B; now 794 vs
   794 instructions, 2804 vs 2827 B, same calls): the picked-input marks (ai_ProdSelectedInput ->
   LCPRODCOL.selected), the column build with the input captions (need, font 0x54, caption -8 / 6), the
   input marks 0x74d..0x74f with the missing amount, the job search (production job 4, then income job 3),
   the day test (hour >= start && hour < end: the original's test ah,5), the pictures 0x4fc / 0x50b /
   0x50c, the fill 1.0 - prodLeft / prodWork (AIJOB's new production words) or 1.0 - incomeAmount /
   incomeAmountF, the amount texts (the three format literals that sat in main.c's gap block), the batch /
   need / missing texts 0xc1..0xc6 in both language orders, the column removal. All code hunks read: block
   layout and allocation - the original lays the found-job block out of line after the mode 0 text
   (0x58d110 '- xor ebp, ebp; cm... */
void ai_UpdateProdColumns(int win, int part, int ncols, int store, int mode)
{
    AIBLD *bld;
    AIJOB *job;
    int c;
    int k;
    int j;
    int worst;
    int missing;
    int amount;
    int pic;
    int season;
    int hour;
    int cls;
    char buf[0xc];
    AIRECIPE list;

    bld = 0;
    if (ai_PlayerBuilding != 0)
        bld = (AIBLD *)ai_PlayerBuilding;
    else if (ai_FallbackBuilding != 0)
        bld = (AIBLD *)ai_FallbackBuilding;
    cls = gm_BuildingClass(bld->type);
    d2_SetForm(win, 0);
    d2_SetForm(win, part);
    gm_BuildingGoodsList((char *)bld, (unsigned *)&list, 1);
    for (c = 0; c < ncols; c++) {
        ai_ProdColumns[c].selected = -1;
        if (ai_ProdSelectedInput != 0)
            for (k = 0; k < 4; k++)
                if (ai_ProdColumns[c].in[k] == ai_ProdSelectedInput)
                    ai_ProdColumns[c].selected = k;
    }
    for (c = 0; c < ncols; c++) {
        missing = 0;
        if (ai_ProdColumns[c].win < 0) {
            ai_ProdColumns[c].win = d2_CreateChildWindow(ai_ProdColumns[c].x, ai_ProdColumns[c].y, 0xcd, 0x57, 0x10, d2_LastWindow);
            d2_Wins[ai_ProdColumns[c].win].font = 0x54;
            d2_WinOpenPic(0, 0, 0x74c, ai_ProdColumns[c].win);
            ai_ProdColumns[c].pic = d2_WinOpenPic(0xd, 5, ai_ProdColumns[c].good + 0xdf, ai_ProdColumns[c].win);
            d2_Objs[ai_ProdColumns[c].pic].interactive = 1;
            d2_Objs[ai_ProdColumns[c].pic].iconx = -1;
            d2_Objs[ai_ProdColumns[c].pic].icony = -1;
            for (k = 0; k < 4; k++) {
                if (ai_ProdColumns[c].in[k] != 0) {
                    ai_ProdColumns[c].inPics[k] = d2_WinOpenPicStyledS(k * 0x31 + 0x11, 0x3a, ai_ProdColumns[c].in[k] + 0xdf,
                                                                      ai_ProdColumns[c].win, -2);
                    d2_Objs[ai_ProdColumns[c].inPics[k]].interactive = 1;
                    d2_SetObjFont(ai_ProdColumns[c].inPics[k], 0x54);
                    itoa(((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].need[k], buf, 10);
                    d2_SetCaption(ai_ProdColumns[c].inPics[k], buf);
                    d2_Objs[ai_ProdColumns[c].inPics[k]].captionX = -8;
                    d2_Objs[ai_ProdColumns[c].inPics[k]].captionY = 6;
                    d2_Objs[ai_ProdColumns[c].inPics[k]].iconx = -1;
                    d2_Objs[ai_ProdColumns[c].inPics[k]].icony = -1;
                }
            }
            ai_ProdColumns[c].amountWin = d2_CreateChildWindow(0x43, 4, 0x88, 0x32, 0x10, ai_ProdColumns[c].win);
            d2_Wins[ai_ProdColumns[c].amountWin].font = 0x54;
        }
        worst = 0;
        for (k = 0; k < 4; k++) {
            if (ai_ProdColumns[c].in[k] == 0)
                continue;
            for (j = 0; j < list.count; j++)
                if (list.item[j] == ai_ProdColumns[c].in[k])
                    break;
            if (j >= list.count) {
                d2_Objs[ai_ProdColumns[c].inPics[k]].icon = 0x74f;
                worst = 2;
                if (ai_ProdColumns[c].selected == k)
                    missing = ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].need[k];
            } else if (list.amount[j] < ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].need[k]) {
                d2_Objs[ai_ProdColumns[c].inPics[k]].icon = 0x74e;
                if (worst <= 1)
                    worst = 1;
                missing = ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].need[k] - list.amount[j];
            } else {
                d2_Objs[ai_ProdColumns[c].inPics[k]].icon = 0x74d;
            }
        }
        d2_Objs[ai_ProdColumns[c].pic].icon = worst + 0x750;
        for (job = (AIJOB *)he_FindEvent(2, 0, 4, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
            if (job->u.w.lo == ai_ProdColumns[c].good)
                break;
        if (job == 0)
            for (job = (AIJOB *)he_FindEvent(2, 0, 3, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                if (job->u.w.lo == ai_ProdColumns[c].good)
                    break;
        amount = 0;
        if (job == 0 || (job->words14[0] == 0 && job->words14[1] == 0)) {
            if (ai_ProdColumns[c].bar != -1) {
                d2_RemoveObj(ai_ProdColumns[c].bar);
                ai_ProdColumns[c].bar = -1;
            }
            if (ai_ProdColumns[c].pic != -1)
                d2_Objs[ai_ProdColumns[c].pic].fillFraction = 0;
        } else {
            pic = 0;
            season = gm_ClockSeason(&ai_Time);
            hour = ai_Time.hour;
            if ((float)hour >= gm_DayStartHours[season] && (float)hour < gm_DayEndHours[season] && job->kind == 4 &&
                job->argc8 == 0)
                pic = 0x4fc;
            else if (job->kind == 4)
                pic = 0x50b;
            else if (job->kind == 3)
                pic = 0x50c;
            if (ai_ProdColumns[c].bar >= 0 && d2_Objs[ai_ProdColumns[c].bar].pic != pic) {
                d2_RemoveObj(ai_ProdColumns[c].bar);
                ai_ProdColumns[c].bar = -1;
            }
            if (ai_ProdColumns[c].bar < 0)
                ai_ProdColumns[c].bar = d2_WinOpenPic(0xd, 5, pic, ai_ProdColumns[c].win);
            amount = job->words14[1] + job->words14[0];
            if (job->kind == 4)
                d2_Objs[ai_ProdColumns[c].pic].fillFraction = (float)(1.0 - job->prodLeft / job->prodWork);
            else if (job->kind == 3)
                d2_Objs[ai_ProdColumns[c].pic].fillFraction = (float)(1.0 - job->incomeAmount / job->incomeAmountF);
        }
        for (k = 0; k < 4; k++)
            if (ai_ProdColumns[c].in[k] != 0)
                d2_Objs[ai_ProdColumns[c].inPics[k]].highlighted = 0;
        if (ai_ProdColumns[c].selected < 0) {
            d2_LastWindow = ai_ProdColumns[c].amountWin;
            if (mode == 0)
                io_PrintText("$C %i %ii %1s$A %1S", amount, 0x51e, 0xb8, ai_ProdColumns[c].good * 2 + 0x9a8);
            else if (mode == 1)
                io_PrintText("$C %i %ii %1s$A %1S ab", amount, 0x51e, 0xba, ai_ProdColumns[c].good * 2 + 0x9a8);
            else
                io_PrintText("$C %i %ii %s zu$A %1S", amount, 0x51e, 0xbb, ai_ProdColumns[c].good * 2 + 0x9a8);
        } else if (missing != 0) {
            d2_Objs[ai_ProdColumns[c].inPics[ai_ProdColumns[c].selected]].highlighted = 1;
            d2_LastWindow = ai_ProdColumns[c].amountWin;
            if (((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch == 1) {
                switch (ls_GetLanguage()) {
                case 0:
                    io_PrintText(0xc1, ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch, ai_ProdColumns[c].good * 2 + 0x9a8);
                    break;
                case 1:
                    io_PrintText(0xc1, ai_ProdColumns[c].good * 2 + 0x9a8, ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch);
                    break;
                }
            } else {
                switch (ls_GetLanguage()) {
                case 0:
                    io_PrintText(0xc2, ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch, ai_ProdColumns[c].good * 2 + 0x9a8);
                    break;
                case 1:
                    io_PrintText(0xc2, ai_ProdColumns[c].good * 2 + 0x9a8, ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch);
                    break;
                }
            }
            if (ai_ProdColumns[c].need[ai_ProdColumns[c].selected] == 1)
                io_PrintText(0xc3, 1, ai_ProdColumns[c].in[ai_ProdColumns[c].selected] * 2 + 0x9a8);
            else
                io_PrintText(0xc4, ai_ProdColumns[c].need[ai_ProdColumns[c].selected],
                             ai_ProdColumns[c].in[ai_ProdColumns[c].selected] * 2 + 0x9a8);
            if (missing == 1)
                io_PrintText(0xc5);
            else
                io_PrintText(0xc6, missing);
        } else {
            d2_Objs[ai_ProdColumns[c].inPics[ai_ProdColumns[c].selected]].highlighted = 1;
            d2_LastWindow = ai_ProdColumns[c].amountWin;
            if (((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch == 1) {
                switch (ls_GetLanguage()) {
                case 0:
                    io_PrintText(0xc1, ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch, ai_ProdColumns[c].good * 2 + 0x9a8);
                    break;
                case 1:
                    io_PrintText(0xc1, ai_ProdColumns[c].good * 2 + 0x9a8, ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch);
                    break;
                }
            } else {
                switch (ls_GetLanguage()) {
                case 0:
                    io_PrintText(0xc2, ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch, ai_ProdColumns[c].good * 2 + 0x9a8);
                    break;
                case 1:
                    io_PrintText(0xc2, ai_ProdColumns[c].good * 2 + 0x9a8, ((AIGOOD *)ai_Goods)[ai_ProdColumns[c].good].batch);
                    break;
                }
            }
            if (ai_ProdColumns[c].need[ai_ProdColumns[c].selected] == 1)
                io_PrintText(0xc3, 1, ai_ProdColumns[c].in[ai_ProdColumns[c].selected] * 2 + 0x9a8);
            else
                io_PrintText(0xc4, ai_ProdColumns[c].need[ai_ProdColumns[c].selected],
                             ai_ProdColumns[c].in[ai_ProdColumns[c].selected] * 2 + 0x9a8);
        }
        d2_SetForm(win, part);
    }
    for (; c < 16; c++) {
        if (ai_ProdColumns[c].win >= 0) {
            ai_ProdColumns[c].selected = -1;
            d2_RemoveObj(ai_ProdColumns[c].win);
            ai_ProdColumns[c].win = -1;
            ai_ProdColumns[c].pic = -1;
            ai_ProdColumns[c].bar = -1;
            for (k = 0; k < 4; k++)
                ai_ProdColumns[c].inPics[k] = -1;
            ai_ProdColumns[c].amountWin = -1;
        }
    }
}
/* 0x58d720, fills the goods grids of the production window from the store of the building bld: the rows open by
   the store's levels (AIOBJ.lvl1c: the four rows of ai_ProdRows, lvl1d: the two of ai_ProdRows2); each goods of the
   store (gm_Query 1, 5) gets its slot or the first free one, in ai_ProdGoods (the types 0x17 / 0x25; a building of
   class 2 only 0x17) or ai_ProdGoods2, with its reserved part, amount and stock; slots whose goods left the store
   are cleared. A left click (with the permission 0x200, nothing on the pointer) on a slot's reserve button cycles
   the goods' reserve 0 .. 4 and sends it (cm field message). Returns 1 when a slot's check box is set */
int ai_UpdateProdGoodsList(AIBLD *bld, AIOBJ *store)
{
    int rows[9][4] = { { 2, 0 }, { 2, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 3, 2 }, { 4, 2 }, { 4, 3 }, { 4, 4 } };
    int rows2[9][2] = { { 0, 2 }, { 0, 2 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 2, 3 }, { 2, 4 }, { 3, 4 }, { 4, 4 } };
    AIOBJ *item;
    int any;
    int i;
    unsigned int value;     /* the building class, later the new reserve sent (one frame slot in the original) */

    value = gm_BuildingClass(bld->type);
    for (i = 0; i < 4; i++)
        ai_ProdRows[i].slots = rows[store->lvl1c][i];
    for (i = 0; i < 2; i++)
        ai_ProdRows2[i].slots = rows2[store->lvl1d][i];
    for (item = (AIOBJ *)gm_Query((void *)store->list, 1, 5); item != 0; item = (AIOBJ *)gm_QueryNext()) {
        if (value == 2 && ((AIGOOD *)ai_Goods)[item->id].type == 0x17 ||
            value != 2 && (((AIGOOD *)ai_Goods)[item->id].type == 0x17 || ((AIGOOD *)ai_Goods)[item->id].type == 0x25)) {
            for (i = 0; i < 16; i++)
                if (ai_ProdGoods[i].good == item->id)
                    break;
            if (i >= 16) {
                for (i = 0; i < 16; i++)
                    if (ai_ProdGoods[i].good == 0)
                        break;
                if (i < 16) {
                    ai_ProdGoods[i].good = item->id;
                    ai_ProdGoods[i].reserved = (unsigned)(gm_ListStock(store, item->id) * item->reserve) >> 2;
                    ai_ProdGoods[i].amount = gm_ListAmount(store, item);
                    ai_ProdGoods[i].stock = gm_ListStock(store, item->id);
                }
            } else {
                ai_ProdGoods[i].reserved = (unsigned)(gm_ListStock(store, item->id) * item->reserve) >> 2;
                ai_ProdGoods[i].amount = gm_ListAmount(store, item);
                ai_ProdGoods[i].stock = gm_ListStock(store, item->id);
            }
        } else {
            for (i = 0; i < 8; i++)
                if (ai_ProdGoods2[i].good == item->id)
                    break;
            if (i >= 8) {
                for (i = 0; i < 8; i++)
                    if (ai_ProdGoods2[i].good == 0)
                        break;
                if (i < 8) {
                    ai_ProdGoods2[i].good = item->id;
                    ai_ProdGoods2[i].reserved = (unsigned)(gm_ListStock(store, item->id) * item->reserve) >> 2;
                    ai_ProdGoods2[i].amount = gm_ListAmount(store, item);
                    ai_ProdGoods2[i].stock = gm_ListStock(store, item->id);
                }
            } else {
                ai_ProdGoods2[i].reserved = (unsigned)(gm_ListStock(store, item->id) * item->reserve) >> 2;
                ai_ProdGoods2[i].amount = gm_ListAmount(store, item);
                ai_ProdGoods2[i].stock = gm_ListStock(store, item->id);
            }
        }
    }
    for (i = 0; i < 16; i++)
        if (gm_Query((void *)store->list, 1, 0, ai_ProdGoods[i].good) == 0)
            ai_ProdGoods[i].good = 0;
    for (i = 0; i < 8; i++)
        if (gm_Query((void *)store->list, 1, 0, ai_ProdGoods2[i].good) == 0)
            ai_ProdGoods2[i].good = 0;
    any = 0;
    for (i = 0; !any && i < 16; i++)
        if (ai_ProdGoods[i].check != -1 && d2_Objs[ai_ProdGoods[i].check].type == 0x41 && d2_GetObjValue(ai_ProdGoods[i].check) != 0)
            any = 1;
    for (i = 0; !any && i < 8; i++)
        if (ai_ProdGoods2[i].check != -1 && d2_Objs[ai_ProdGoods2[i].check].type == 0x41 &&
            d2_GetObjValue(ai_ProdGoods2[i].check) != 0)
            any = 1;
    if ((ai_BuildingPerms & 0x200) != 0 && dm_L.lclick != 0 && d2_CountPointerItems() == 0) {
        for (i = 0; i < 16; i++) {
            if (ai_ProdGoods[i].slider == -1 || ai_ProdGoods[i].slider != d2_PointerObj)
                continue;
            item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, ai_ProdGoods[i].good);
            if (item == 0)
                break;
            value = item->reserve >= 4 ? 0 : item->reserve + 1;
            cm_BeginFieldBlock((char *)item, item->objId);
            cm_AddFieldValue(4, 1, (unsigned short)((char *)&item->reserve - cm_FieldObject), &value);
            cm_SendFieldValues();
            item->reserve = value;
        }
        for (i = 0; i < 8; i++) {
            if (ai_ProdGoods2[i].slider == -1 || ai_ProdGoods2[i].slider != d2_PointerObj)
                continue;
            item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, ai_ProdGoods2[i].good);
            if (item == 0)
                break;
            value = item->reserve >= 4 ? 0 : item->reserve + 1;
            cm_BeginFieldBlock((char *)item, item->objId);
            cm_AddFieldValue(4, 1, (unsigned short)((char *)&item->reserve - cm_FieldObject), &value);
            cm_SendFieldValues();
            item->reserve = value;
        }
    }
    return any;
}
/* 0x58dd00, lays out the goods grids of the production window in the part form of the view: makes the rows' title
   pictures (0x743), gives each row the picture of its open slots (0x741 + slots) when they changed, centres the row
   in the window (rows with fewer than 2 slots are hidden; the second grid's rows 0xb6 lower), places the slots of
   ai_ProdGoods / ai_ProdGoods2 row by row (0x44 apart) and shows each (ai_ShowProdSlot). With pointer set it puts
   the checked goods on the mouse pointer (d2_SetPointerItem) */
void ai_LayoutProdGoodsList(int view, int part, int pointer, int bld)
{
    D2WIN *win;
    int width;
    int i;
    int k;
    int row;
    int n;

    win = &d2_Wins[d2_Views[view].wins[part]];
    width = win->w;
    d2_SetForm(view, part);
    for (i = 0; i < 4; i++)
        if (ai_ProdRows[i].title == -1)
            ai_ProdRows[i].title = d2_WinOpenPic(0, 0, 0x743, d2_LastWindow);
    d2_SetForm(view, part);
    for (i = 0; i < 2; i++)
        if (ai_ProdRows2[i].title == -1)
            ai_ProdRows2[i].title = d2_WinOpenPic(0, 0, 0x743, d2_LastWindow);
    for (i = 0; i < 4; i++) {
        if (ai_ProdRows[i].shown != ai_ProdRows[i].slots)
            d2_SetObjPic(ai_ProdRows[i].title, ai_ProdRows[i].slots + 0x741);
        ai_ProdRows[i].shown = ai_ProdRows[i].slots;
        if (ai_ProdRows[i].slots >= 2) {
            ai_ProdRows[i].x = (width - d2_Pics[ai_ProdRows[i].slots + 0x741].w) >> 1;
            ai_ProdRows[i].y = (d2_Pics[0x743].h + 0x16) * i;
        }
    }
    for (i = 0; i < 2; i++) {
        if (ai_ProdRows2[i].shown != ai_ProdRows2[i].slots)
            d2_SetObjPic(ai_ProdRows2[i].title, ai_ProdRows2[i].slots + 0x741);
        ai_ProdRows2[i].shown = ai_ProdRows2[i].slots;
        if (ai_ProdRows2[i].slots >= 2) {
            ai_ProdRows2[i].x = (width - d2_Pics[ai_ProdRows2[i].slots + 0x741].w) >> 1;
            ai_ProdRows2[i].y = (d2_Pics[0x743].h + 0x16) * i + 0xb6;
        }
    }
    d2_SetForm(view, part);
    for (i = 0; i < 4; i++) {
        if (ai_ProdRows[i].shown >= 2) {
            d2_Objs[ai_ProdRows[i].title].disabled = 0;
            d2_MoveObj(ai_ProdRows[i].x + win->x, ai_ProdRows[i].y + win->y, ai_ProdRows[i].title);
        } else {
            d2_Objs[ai_ProdRows[i].title].disabled = 1;
        }
    }
    n = 0;
    row = 0;
    for (k = 0; k < 16; k++) {
        if (++n > ai_ProdRows[row].shown) {
            n = 1;
            do
                row++;
            while (ai_ProdRows[row].shown < 1);
        }
        ai_ProdGoods[k].x = ai_ProdRows[row].x + n * 0x44 - 0x44;
        ai_ProdGoods[k].y = ai_ProdRows[row].y;
        ai_ShowProdSlot(view, part, &ai_ProdGoods[k], pointer, 2, (AIBLD *)bld);
    }
    d2_SetForm(view, part);
    for (i = 0; i < 2; i++) {
        if (ai_ProdRows2[i].shown != ai_ProdRows2[i].slots)
            d2_SetObjPic(ai_ProdRows2[i].title, ai_ProdRows2[i].slots + 0x741);
        ai_ProdRows2[i].shown = ai_ProdRows2[i].slots;
        if (ai_ProdRows2[i].shown >= 2) {
            d2_Objs[ai_ProdRows2[i].title].disabled = 0;
            d2_MoveObj(ai_ProdRows2[i].x + win->x, ai_ProdRows2[i].y + win->y, ai_ProdRows2[i].title);
        } else {
            d2_Objs[ai_ProdRows2[i].title].disabled = 1;
        }
    }
    n = 0;
    row = 0;
    for (k = 0; k < 8; k++) {
        if (++n > ai_ProdRows2[row].shown) {
            n = 1;
            do
                row++;
            while (ai_ProdRows2[row].shown < 1);
        }
        ai_ProdGoods2[k].x = ai_ProdRows2[row].x + n * 0x44 - 0x44;
        ai_ProdGoods2[k].y = ai_ProdRows2[row].y;
        ai_ShowProdSlot(view, part, &ai_ProdGoods2[k], pointer, 2, (AIBLD *)bld);
    }
    if (pointer) {
        d2_ClearPointerItems();
        for (k = 0; k < 16; k++)
            if (ai_ProdGoods[k].check != -1)
                d2_SetPointerItem(ai_ProdGoods[k].good + 0xdf, d2_GetObjValue(ai_ProdGoods[k].check));
        for (k = 0; k < 8; k++)
            if (ai_ProdGoods2[k].check != -1)
                d2_SetPointerItem(ai_ProdGoods2[k].good + 0xdf, d2_GetObjValue(ai_ProdGoods2[k].check));
    }
}


/* 0x58e130, fills the goods grid of the transport (market) window from the market object market for the stall:
   the stall's level (AIOBJ.lvl1c) opens the slots of the four rows of ai_ProdRows; each goods of the market
   (gm_Query 1, 5) keeps its slot in ai_ProdGoods (its amount cleared) or gets the first free one (the goods, its
   reserved part, amount and stock 0); then every slot takes the amount and stock of its goods in the stall.
   Returns 1 when a slot's check box (an object of type 0x41) is set, as ai_UpdateProdGoodsList does */
int ai_UpdateMarketGoodsList(AIOBJ *stall, AIOBJ *market)
{
    int rows[17][4] = {
        { 2, 0, 0, 0 }, { 2, 0, 0, 0 }, { 2, 0, 0, 0 }, { 3, 0, 0, 0 }, { 4, 0, 0, 0 }, { 3, 2, 0, 0 },
        { 4, 2, 0, 0 }, { 4, 3, 0, 0 }, { 4, 4, 0, 0 }, { 4, 3, 2, 0 }, { 4, 3, 3, 0 }, { 4, 4, 3, 0 },
        { 4, 4, 4, 0 }, { 4, 4, 3, 2 }, { 4, 4, 3, 3 }, { 4, 4, 4, 3 }, { 4, 4, 4, 4 }
    };
    AIOBJ *item;
    int any;
    int i;

    for (i = 0; i < 4; i++)
        ai_ProdRows[i].slots = rows[stall->lvl1c][i];
    for (item = (AIOBJ *)gm_Query((void *)market->list, 1, 5); item != 0; item = (AIOBJ *)gm_QueryNext()) {
        for (i = 0; i < 16; i++)
            if (ai_ProdGoods[i].good == item->id)
                break;
        if (i >= 16) {
            for (i = 0; i < 16; i++)
                if (ai_ProdGoods[i].good == 0)
                    break;
            if (i < 16) {
                ai_ProdGoods[i].good = item->id;
                ai_ProdGoods[i].reserved = 0;
                ai_ProdGoods[i].amount = 0;
                ai_ProdGoods[i].stock = 0;
            }
        } else
            ai_ProdGoods[i].amount = 0;
    }
    for (i = 0; i < 16; i++) {
        item = (AIOBJ *)gm_Query((void *)stall->list, 1, 0, ai_ProdGoods[i].good);
        if (item != 0) {
            ai_ProdGoods[i].amount = gm_ListAmount(stall, item);
            ai_ProdGoods[i].stock = gm_ListStock(stall, item->id);
        }
    }
    any = 0;
    for (i = 0; !any && i < 16; i++)
        if (ai_ProdGoods[i].check != -1 && d2_Objs[ai_ProdGoods[i].check].type == 0x41 && d2_GetObjValue(ai_ProdGoods[i].check) != 0)
            any = 1;
    return any;
}
/* 0x58e440, lays out the goods grid of the transport (market) window in the part form of the view, as
   ai_LayoutProdGoodsList does for the first grid alone: makes the rows' title pictures (0x743), gives each row the
   picture of its open slots (0x741 + slots) when they changed, centres the row in the window (0x1c below the
   title's height apart; rows with fewer than 2 slots are hidden), places the slots of ai_ProdGoods row by row
   (0x44 apart) and shows each (ai_ShowProdSlot, kind 3). With pointer set it puts the checked goods of both grids
   on the mouse pointer (d2_SetPointerItem) */
void ai_LayoutMarketGoodsList(int view, int part, int pointer, int bld)
{
    D2WIN *win;
    int width;
    int i;
    int k;
    int row;
    int n;

    win = &d2_Wins[d2_Views[view].wins[part]];
    width = win->w;
    d2_SetForm(view, part);
    for (i = 0; i < 4; i++)
        if (ai_ProdRows[i].title == -1)
            ai_ProdRows[i].title = d2_WinOpenPic(0, 0, 0x743, d2_LastWindow);
    for (i = 0; i < 4; i++) {
        if (ai_ProdRows[i].shown != ai_ProdRows[i].slots)
            d2_SetObjPic(ai_ProdRows[i].title, ai_ProdRows[i].slots + 0x741);
        ai_ProdRows[i].shown = ai_ProdRows[i].slots;
        if (ai_ProdRows[i].slots >= 2) {
            ai_ProdRows[i].x = (width - d2_Pics[ai_ProdRows[i].slots + 0x741].w) >> 1;
            ai_ProdRows[i].y = (d2_Pics[0x743].h + 0x1c) * i;
        }
    }
    d2_SetForm(view, part);
    for (i = 0; i < 4; i++) {
        if (ai_ProdRows[i].shown >= 2) {
            d2_Objs[ai_ProdRows[i].title].disabled = 0;
            d2_MoveObj(ai_ProdRows[i].x + win->x, ai_ProdRows[i].y + win->y, ai_ProdRows[i].title);
        } else {
            d2_Objs[ai_ProdRows[i].title].disabled = 1;
        }
    }
    n = 0;
    row = 0;
    for (k = 0; k < 16; k++) {
        if (++n > ai_ProdRows[row].shown) {
            n = 1;
            do
                row++;
            while (ai_ProdRows[row].shown < 1);
        }
        ai_ProdGoods[k].x = ai_ProdRows[row].x + n * 0x44 - 0x44;
        ai_ProdGoods[k].y = ai_ProdRows[row].y;
        ai_ShowProdSlot(view, part, &ai_ProdGoods[k], pointer, 3, (AIBLD *)bld);
    }
    if (pointer) {
        d2_ClearPointerItems();
        for (k = 0; k < 16; k++)
            if (ai_ProdGoods[k].check != -1)
                d2_SetPointerItem(ai_ProdGoods[k].good + 0xdf, d2_GetObjValue(ai_ProdGoods[k].check));
        for (k = 0; k < 8; k++)
            if (ai_ProdGoods2[k].check != -1)
                d2_SetPointerItem(ai_ProdGoods2[k].good + 0xdf, d2_GetObjValue(ai_ProdGoods2[k].check));
    }
}
#define d2_CreateWinTextS ((int (*)(short, short, char *, int))d2_CreateWinText)  /* @original ai_ShowProdSlot passes 16-bit x / y (mov ax, word ptr / add ax, 0x40; push eax) */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win);  /* 0x46a960 (d2.h) */
int d2_RemoveWindow(int win);                                   /* 0x472ce0 (d2.h) */

/* 0x58e6a0, shows one goods slot of the production window's grids in the part form of the view: an empty slot
   (good 0) loses its objects; else it gets its stock bar, its icon (an input icon taking the amount when input is
   set, its style 0xca with the permission 0x200, else 0x8a; else a plain picture), the side's label (side bit 1:
   0x747 and the buy price at 0.9 of gm_GoodsPrice, bit 2: 0x746 and the sell price: the market (class 0xa) 1.1
   times it, else the building's priceFactor; a building of kind 0x47 prices by its player slot), the progress
   of the building's running job of the goods (job 4 / 3) as the icon's fill, the icon's and the bar's values
   (the input clamped to the amount, without the permission less the reserved part), the tooltip "%s~ %li %s"
   (name, amount, unit) and the positions of all its objects in the window */
void ai_ShowProdSlot(int view, int part, LCPRODSLOT *slot, int input, unsigned char side, AIBLD *bld)
{
    D2WIN *win;
    AIJOB *job;
    AIBTYPE *bt;
    int style;
    int value;
    int max;
    char text[0x20];

    d2_SetForm(view, part);
    win = &d2_Wins[d2_Views[view].wins[part]];
    if (slot->good != 0) {
        if (slot->slider == -1)
            slot->slider = d2_WinOpenSlider(slot->x + 6, slot->y, 0x30, 0, 0x64, 0x6a, 0x3d, d2_LastWindow);
        if (slot->check == -1) {
            if (input) {
                style = 0x8a;
                if ((ai_BuildingPerms & 0x200) != 0)
                    style = 0xca;
                slot->check = d2_WinOpenInputIcon(slot->x + 0xf, slot->y + 6, slot->good + 0xdf, style, d2_LastWindow);
                d2_SetObjFont(slot->check, 0x54);
            } else {
                slot->check = d2_WinOpenPic(slot->x + 0xf, slot->y + 6, slot->good + 0xdf, d2_LastWindow);
                d2_Objs[slot->check].interactive = 1;
            }
        }
        if (slot->label == -1) {
            if (side & 1)
                slot->label = d2_WinOpenPic(slot->x + 0xf, slot->y + 0x3a, 0x747, d2_LastWindow);
            else if (side & 2)
                slot->label = d2_WinOpenPic(slot->x + 0xf, slot->y + 0x3a, 0x746, d2_LastWindow);
        }
        if (side & 2) {
            bt = &((AIBTYPE *)ai_BTypes)[bld->type];
            gm_FormatAmount((int)(bld->type == 0x47 ? gm_GoodsPrice(slot->good, (unsigned char)bld->playerSlot)
                                  : bt->kind == 0xa
                                      ? gm_GoodsPrice(slot->good, ai_OwnPlayer) * 1.1
                                      : gm_GoodsPrice(slot->good, ai_OwnPlayer) * bld->priceFactor),
                            ai_OwnPlayer, text);
            if (slot->priceText == -1)
                slot->priceText = d2_CreateWinTextS(slot->x + 0x10, slot->y + 0x40, text, d2_LastWindow);
            else
                d2_SetObjText(slot->priceText, text);
            d2_Objs[slot->priceText].w = 0x2d;
            d2_Objs[slot->priceText].font = 0x54;
            d2_Objs[slot->priceText].rightAligned = 1;
        }
        if (side & 1) {
            gm_FormatAmount((int)(bld->type == 0x47 ? gm_GoodsPrice(slot->good, (unsigned char)bld->playerSlot)
                                                    : gm_GoodsPrice(slot->good, ai_OwnPlayer) * 0.9),
                            ai_OwnPlayer, text);
            if (slot->buyPriceText == -1)
                slot->buyPriceText = d2_CreateWinTextS(slot->x + 0x10, slot->y + 0x49, text, d2_LastWindow);
            else
                d2_SetObjText(slot->buyPriceText, text);
            d2_Objs[slot->buyPriceText].w = 0x2d;
            d2_Objs[slot->buyPriceText].font = 0x54;
            d2_Objs[slot->buyPriceText].rightAligned = 1;
        }
        for (job = (AIJOB *)he_FindEvent(2, 0, 4, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
            if (job->u.w.lo == slot->good)
                break;
        if (job == 0)
            for (job = (AIJOB *)he_FindEvent(2, 0, 3, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                if (job->u.w.lo == slot->good)
                    break;
        if (job == 0 || (job->words14[0] == 0 && job->words14[1] == 0)) {
            if (slot->check != -1)
                d2_Objs[slot->check].fillFraction = 0;
        } else if (job->kind == 4) {
            d2_Objs[slot->check].fillFraction = 1.0 - (job->prodLeft / job->prodWork);
        } else if (job->kind == 3) {
            d2_Objs[slot->check].fillFraction = 1.0 - (job->incomeAmount / job->incomeAmountF);
        }
        if (input) {
            value = d2_GetObjValue(slot->check);
            if (value > slot->amount)
                value = slot->amount;
            max = slot->amount;
            if ((ai_BuildingPerms & 0x200) == 0)
                max -= slot->reserved;
            d2_SetObjValue4(slot->check, 0, max < 0 ? 0 : max, value);
            if (slot->amount != 0)
                d2_Objs[slot->check].greyed = 0;
            else
                d2_Objs[slot->check].greyed = 1;
            max = slot->stock;
            value = d2_GetObjValue(slot->check);
            d2_SetObjValue(slot->slider, 0, max, slot->amount - (value < 0 ? 0 : value), slot->reserved);
        } else {
            if (slot->amount != 0)
                d2_Objs[slot->check].greyed = 0;
            else
                d2_Objs[slot->check].greyed = 1;
            d2_SetObjValue(slot->slider, 0, slot->stock, slot->amount, slot->reserved);
        }
        d2_Objs[slot->check].style = 0x22;
        d2_Objs[slot->check].captionX = 4;
        d2_Objs[slot->check].captionY = 0;
        sprintf(d2_Objs[slot->check].caption, "%s~ %li %s", txt_ObjectNames[slot->good][0], slot->amount, txt_Ptrs[0x3fe8]);
        if (slot->check >= 0)
            d2_MoveObj(win->x + slot->x + 0xf, win->y + slot->y + 6, slot->check);
        if (slot->label >= 0)
            d2_MoveObj(win->x + slot->x + 0xf, win->y + slot->y + 0x3a, slot->label);
        if (slot->priceText >= 0)
            d2_MoveObj(win->x + slot->x + 0x12, win->y + slot->y + 0x3f, slot->priceText);
        if (slot->buyPriceText >= 0)
            d2_MoveObj(win->x + slot->x + 0x12, win->y + slot->y + 0x49, slot->buyPriceText);
        if (slot->slider >= 0)
            d2_MoveObj(win->x + slot->x + 6, win->y + slot->y, slot->slider);
        if (slot->win >= 0)
            d2_MoveObj(win->x + slot->x + 0xf, win->y + slot->y + 6, d2_Wins[slot->win].parent);
    } else {
        if (slot->check != -1) {
            d2_RemoveObj(slot->check);
            slot->check = -1;
        }
        if (slot->label != -1) {
            d2_RemoveObj(slot->label);
            slot->label = -1;
        }
        if (slot->priceText != -1) {
            d2_RemoveObj(slot->priceText);
            slot->priceText = -1;
        }
        if (slot->buyPriceText != -1) {
            d2_RemoveObj(slot->buyPriceText);
            slot->buyPriceText = -1;
        }
        if (slot->slider != -1) {
            d2_RemoveObj(slot->slider);
            slot->slider = -1;
        }
        if (slot->win != -1) {
            d2_RemoveWindow(slot->win);
            slot->win = -1;
        }
    }
}

char * gm_FindGoods(int p, unsigned id);

/* 0x58ee30, fills the goods grid of the store window from the store store of the building bld: the store's level
   (AIOBJ.lvl1c) opens the slots of the four rows of ai_ProdRows; each goods of the store (gm_Query 1, 5) keeps its
   slot in ai_ProdGoods (its amount refreshed) or gets the first free one; a market stall (0x1db / 0x1dc) takes
   only goods the player of bld (AIBLD.playerSlot) holds (gm_FindGoods). Then every slot takes its reserved part,
   amount and stock (a stall: amount and stock of the goods the player holds), or is cleared when its goods left
   the store. A left click (with the permission 0x200, nothing on the pointer) on a slot's reserve button cycles
   the goods' reserve 0 .. 4 and sends it (cm field message). Returns 1 when a slot's check box is set */
int ai_UpdateStoreGoods(AIBLD *bld, AIOBJ *store)
{
    unsigned int value;     /* the new reserve sent */
    int rows[17][4] = {
        { 2, 0, 0, 0 }, { 2, 0, 0, 0 }, { 2, 0, 0, 0 }, { 3, 0, 0, 0 }, { 4, 0, 0, 0 }, { 3, 2, 0, 0 },
        { 4, 2, 0, 0 }, { 4, 3, 0, 0 }, { 4, 4, 0, 0 }, { 4, 3, 2, 0 }, { 4, 3, 3, 0 }, { 4, 4, 3, 0 },
        { 4, 4, 4, 0 }, { 4, 4, 3, 2 }, { 4, 4, 3, 3 }, { 4, 4, 4, 3 }, { 4, 4, 4, 4 }
    };
    AIOBJ *item;
    GMSTOCK *stock;
    int any;
    int i;

    for (i = 0; i < 4; i++)
        ai_ProdRows[i].slots = rows[store->lvl1c][i];
    for (item = (AIOBJ *)gm_Query((void *)store->list, 1, 5); item != 0; item = (AIOBJ *)gm_QueryNext()) {
        for (i = 0; i < 16; i++)
            if (ai_ProdGoods[i].good == item->id)
                break;
        if (i >= 16) {
            for (i = 0; i < 16; i++)
                if (ai_ProdGoods[i].good == 0)
                    break;
            if (i < 16 && (store->id != 0x1db && store->id != 0x1dc
                           || (stock = (GMSTOCK *)gm_FindGoods(bld->playerSlot, item->id)) == 0 || stock->amount != 0)) {
                ai_ProdGoods[i].good = item->id;
                ai_ProdGoods[i].reserved = 0;
                ai_ProdGoods[i].amount = 0;
                ai_ProdGoods[i].stock = 0;
            }
        } else
            ai_ProdGoods[i].amount = gm_ListAmount(store, item);
    }
    for (i = 0; i < 16; i++) {
        item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, ai_ProdGoods[i].good);
        if (item != 0) {
            if (store->id != 0x1db && store->id != 0x1dc) {
                ai_ProdGoods[i].reserved = (unsigned)(gm_ListStock(store, item->id) * item->reserve) >> 2;
                ai_ProdGoods[i].amount = gm_ListAmount(store, item);
                ai_ProdGoods[i].stock = gm_ListStock(store, item->id);
            } else {
                stock = (GMSTOCK *)gm_FindGoods(bld->playerSlot, item->id);
                if (stock != 0 && stock->amount != 0) {
                    ai_ProdGoods[i].amount = gm_ListAmount(store, item);
                    ai_ProdGoods[i].stock = gm_ListStock(store, item->id);
                }
            }
        } else
            ai_ProdGoods[i].good = 0;
    }
    if ((ai_BuildingPerms & 0x200) != 0 && dm_L.lclick != 0 && d2_CountPointerItems() == 0) {
        for (i = 0; i < 16; i++) {
            if (ai_ProdGoods[i].slider == -1 || ai_ProdGoods[i].slider != d2_PointerObj)
                continue;
            item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, ai_ProdGoods[i].good);
            if (item == 0)
                break;
            value = item->reserve >= 4 ? 0 : item->reserve + 1;
            cm_BeginFieldBlock((char *)item, item->objId);
            cm_AddFieldValue(4, 1, (char *)&item->reserve - cm_FieldObject, &value);
            cm_SendFieldValues();
            item->reserve = value;
        }
    }
    any = 0;
    for (i = 0; !any && i < 16; i++)
        if (ai_ProdGoods[i].check != -1 && d2_Objs[ai_ProdGoods[i].check].type == 0x41 && d2_GetObjValue(ai_ProdGoods[i].check) != 0)
            any = 1;
    return any;
}
/* 0x58f290, lays out the first goods grid (ai_ProdRows / ai_ProdGoods) of the transport / storage / equip windows
   in the part form of the view, as ai_LayoutProdGoodsList does for the production window but with the rows
   (the picture 0x743's height + 0x23) apart and the slots' side by the object obj: 0x1db (1), 0x1dc (2), else 3;
   with pointer set it puts the checked goods of both grids on the mouse pointer */
void ai_LayoutStoreGoods(int view, int part, int pointer, AIBLD *bld, unsigned short *obj)
{
    D2WIN *win;
    int width;
    int i;
    int k;
    int row;
    int n;

    win = &d2_Wins[d2_Views[view].wins[part]];
    width = win->w;
    d2_SetForm(view, part);
    for (i = 0; i < 4; i++)
        if (ai_ProdRows[i].title == -1)
            ai_ProdRows[i].title = d2_WinOpenPic(0, 0, 0x743, d2_LastWindow);
    for (i = 0; i < 4; i++) {
        if (ai_ProdRows[i].shown != ai_ProdRows[i].slots)
            d2_SetObjPic(ai_ProdRows[i].title, ai_ProdRows[i].slots + 0x741);
        ai_ProdRows[i].shown = ai_ProdRows[i].slots;
        if (ai_ProdRows[i].slots >= 2) {
            ai_ProdRows[i].x = (width - d2_Pics[ai_ProdRows[i].slots + 0x741].w) >> 1;
            ai_ProdRows[i].y = (d2_Pics[0x743].h + 0x23) * i;
        }
    }
    d2_SetForm(view, part);
    for (i = 0; i < 4; i++) {
        if (ai_ProdRows[i].shown >= 2) {
            d2_Objs[ai_ProdRows[i].title].disabled = 0;
            d2_MoveObj(ai_ProdRows[i].x + win->x, ai_ProdRows[i].y + win->y, ai_ProdRows[i].title);
        } else {
            d2_Objs[ai_ProdRows[i].title].disabled = 1;
        }
    }
    n = 0;
    row = 0;
    for (k = 0; k < 16; k++) {
        if (++n > ai_ProdRows[row].shown) {
            n = 1;
            do
                row++;
            while (ai_ProdRows[row].shown < 1);
        }
        ai_ProdGoods[k].x = ai_ProdRows[row].x + n * 0x44 - 0x44;
        ai_ProdGoods[k].y = ai_ProdRows[row].y;
        if (*obj == 0x1db)
            ai_ShowProdSlot(view, part, &ai_ProdGoods[k], pointer, 1, bld);
        else if (*obj == 0x1dc)
            ai_ShowProdSlot(view, part, &ai_ProdGoods[k], pointer, 2, bld);
        else
            ai_ShowProdSlot(view, part, &ai_ProdGoods[k], pointer, 3, bld);
    }
    if (pointer) {
        d2_ClearPointerItems();
        for (k = 0; k < 16; k++)
            if (ai_ProdGoods[k].check != -1)
                d2_SetPointerItem(ai_ProdGoods[k].good + 0xdf, d2_GetObjValue(ai_ProdGoods[k].check));
        for (k = 0; k < 8; k++)
            if (ai_ProdGoods2[k].check != -1)
                d2_SetPointerItem(ai_ProdGoods2[k].good + 0xdf, d2_GetObjValue(ai_ProdGoods2[k].check));
    }
}


struct D3OBJECT;
struct D3OBJECT * d3_FindObject(int a, int flags, int c, char *name); /* 0x449690 (d3_interface.c, d3.h) */
int sv_RegisterNamePlate(char *name, char *text, int b);         /* 0x52e1c0 (sv.c) */
int ai_OpenTransport(int rec);                                   /* 0x5af1a0, below */

/* 0x58f510, the shop-counter contact of a building (264 B): for a building of another player (the owner
   +0x27 differs from ai_PlayerId), the first existing counter dummy found by d3_FindObject (mask 0x60) among
   contact_EINKAUF, _METALL, _SKRIPTE, _HOLZ, _STEIN gets its name plate registered by sv_RegisterNamePlate
   (the counter's name, its plate text, 0x10); when that plate is the clicked object, the transport window
   ai_OpenTransport opens on the building record */
void ai_MarketContact(AIBLD *b)
{
    int plate;

    plate = 0;
    if (ai_PlayerId != b->owner) {
        if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF"))
            plate = sv_RegisterNamePlate("contact_EINKAUF", ai_TextEinkauf, 0x10);
        else if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF_METALL"))
            plate = sv_RegisterNamePlate("contact_EINKAUF_METALL", ai_TextEinkaufMetall, 0x10);
        else if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF_SKRIPTE"))
            plate = sv_RegisterNamePlate("contact_EINKAUF_SKRIPTE", ai_TextEinkaufSkripte, 0x10);
        else if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF_HOLZ"))
            plate = sv_RegisterNamePlate("contact_EINKAUF_HOLZ", ai_TextEinkaufHolz, 0x10);
        else if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF_STEIN"))
            plate = sv_RegisterNamePlate("contact_EINKAUF_STEIN", ai_TextEinkaufStein, 0x10);
    }
    if (ai_PlayerId != b->owner && ai_ClickedObject != 0 && ai_ClickedObject == plate)
        ai_OpenTransport((int)b);
}


/* 0x58f620, the frame pump of the build screen (23 B): runs sv_RunFrame (0x5309a0, this callback, mask 0x67fff)
   until the screen ends; the caller's context is not used */
void ai_PumpFrames(int unused)
{
    int r;

    do {
        r = sv_RunFrame(ai_PumpFrames, 0x67fff);
    } while (r != 0);
}

char *strstr(); unsigned char * gm_BuildQuery(int n, ...); unsigned char * gm_BuildNext(void);

/* 0x58f640, the building name of a kind id (83 B): ai_BuildNames holds 15 groups of 0x50 bytes (the 12 kind ids
   after the 0x20-byte name); the name of the matching id is copied to out (inlined strcpy); 1 when found, 0 else */
int ai_BuildingName(int id, char *out)
{
    int i;
    int g;

    for (g = 0; g < 0xf; g++) {
        for (i = 0; i < 0xc; i++) {
            if (ai_BuildNames[g].ids[i] == id) {
                strcpy(out, ai_BuildNames[g].name);
                return 1;
            }
        }
    }
    return 0;
}

struct D3OBJECT;
struct D3VECTOR;
void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out); /* 0x458a20 (d3_pose.c) */
unsigned char *gm_BuildQuery(int n, ...);                  /* 0x501190 (f3gm.c) */
unsigned char *gm_BuildNext(void);                         /* 0x501030 (f3gm.c) */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps); /* 0x453e80 (d3_vec.c) */

/* 0x58f6a0, the placement-candidate filter (250 B, a d3_ForEachObject callback found through the pointer in
   0x58f7a0): obj passes when its name (or its name + 1) starts with the prefix (case-insensitive; "bk_" when the
   prefix is 0); it is dropped when a building of the build query (gm_BuildQuery (1, 6) .. gm_BuildNext) stands
   within 100.0 of it; otherwise it is appended to ai_BauplatzList (count ai_BauplatzCount); always returns 1 */
char ai_BauplatzFilter(int obj, char *prefix)
{
    AIBLD *b;
    int c;
    float pos[4];
    float q[4];

    if (prefix == 0) {
        if (strnicmp((char *)obj, "bk_", 3) != 0 && strnicmp((char *)(obj + 1), "bk_", 3) != 0)
            return 1;
    } else {
        if (strnicmp((char *)obj, prefix, strlen(prefix)) != 0 &&
            strnicmp((char *)(obj + 1), prefix, strlen(prefix)) != 0)
            return 1;
    }
    d3_PointToWorld((struct D3OBJECT *)obj, &((D3OBJECT *)obj)->pos, (struct D3VECTOR *)pos);
    for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
        c = (int)b->object;
        if (c != 0) {
            d3_PointToWorld((struct D3OBJECT *)c, &((D3OBJECT *)c)->pos, (struct D3VECTOR *)q);
            if (d3_VecEqual((struct D3VECTOR *)q, (struct D3VECTOR *)pos, 100.0f) != 0)
                return 1;
        }
    }
    ai_BauplatzList[ai_BauplatzCount] = obj;
    ai_BauplatzCount++;
    return 1;
}

int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);  /* 0x453e80 */

/* 0x58f7a0, collects the build sites of a building kind: d3_ForEachObject (the default scene, mask 0x180) with the
   filter ai_BauplatzFilter puts the objects named with prefix into list (ai_BauplatzList, count ai_BauplatzCount);
   a site is dropped again when the object of a building (gm_BuildQuery 1, 6) stands within 100.0 of its parent
   object or of one of its "bk_" named children; returns the count */
int ai_FindBauplaetze(char *prefix, int *list)
{
    int i;
    D3OBJECT *obj;
    D3OBJECT *o;
    AIBLD *b;
    float at[4];            /* d3_PointToWorld writes the angles after the position */
    float pos[4];

    ai_BauplatzCount = 0;
    ai_BauplatzList = list;
    d3_ForEachObject(d3_DefaultScene, 0, 0x180, ai_BauplatzFilter, prefix);
    for (i = 0; i < ai_BauplatzCount; i++) {
        if ((obj = (D3OBJECT *)ai_BauplatzList[i]) == 0)
            goto next;
        if (obj->parent != 0) {
            d3_PointToWorld(obj->parent, &obj->parent->pos, (D3VECTOR *)at);
            for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
                o = (D3OBJECT *)b->object;
                if (o != 0) {
                    d3_PointToWorld(o, &o->pos, (D3VECTOR *)pos);
                    if (d3_VecEqual((D3VECTOR *)pos, (D3VECTOR *)at, 100.0f) != 0) {
                        ai_BauplatzList[i] = 0;
                        break;
                    }
                }
            }
        }
        for (obj = obj->child; obj != 0; obj = obj->next) {
            if (strncmp(obj->name, "bk_", 3) == 0) {
                d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)at);
                for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
                    o = (D3OBJECT *)b->object;
                    if (o != 0) {
                        d3_PointToWorld(o, &o->pos, (D3VECTOR *)pos);
                        if (d3_VecEqual((D3VECTOR *)pos, (D3VECTOR *)at, 100.0f) != 0) {
                            ai_BauplatzList[i] = 0;
                            goto next;
                        }
                    }
                }
            }
        }
    next:
        ;
    }
    return ai_BauplatzCount;
}

#pragma pack(push, 1)
typedef struct AIOBJVIEW {         /* the object fields the vg_ / build-site callbacks read (d3.h D3OBJECT, same layout) */
    char pad0[0x1f0];
    struct AIOBJVIEW *next;        /* 0x1f0, the sibling list */
    struct AIOBJVIEW *prev;        /* 0x1f4 */
    struct AIOBJVIEW *parent;      /* 0x1f8, the parent object (the owner of a building site) */
    struct AIOBJVIEW *child;       /* 0x1fc, the first child (the "bk_" site markers live here) */
    char pad200[0x20];
    int kind;                      /* 0x220, 1 = per-vertex lit */
} AIOBJVIEW;
#pragma pack(pop)

/* 0x58f920, the "vg_" predicate of ai_ShowVgObjects (86 B, a d3_ForEachObject callback): objects whose name contains
   "vg_" and whose parent is no per-vertex-lit object (parent == 0 or its kind != 1) are appended to the ctx list
   (count at +0x80); returns whether the list still has room (0x20 entries) */
char ai_VgFilter(int obj, AIVGLIST *ctx)
{
    AIOBJVIEW *o;

    o = (AIOBJVIEW *)obj;
    if (strstr((char *)obj, "vg_") != 0 && (o->parent == 0 || o->parent->kind != 1)) {
        ctx->objs[ctx->count] = obj;
        ctx->count = ctx->count + 1;
    }
    return ctx->count < 0x20;
}

char d3if_ShowTree(struct D3OBJECT *obj, char show);

char d3_ForEachObject(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, char (*cb)(struct D3OBJECT *, void *), void *ctx);  /* 0x438d40 (d3.h) */
char d3if_ShowTree(struct D3OBJECT *obj, char show);                /* 0x447300 (d3_interface.c) */

/* 0x58f980, shows or hides the player's "vg_" objects (146 B): the 0x84-byte local list (count at +0x80) is
   filled by d3_ForEachObject (mask 0x2c0) with ai_VgFilter, then every entry is shown by d3if_ShowTree (obj, 0) or
   hidden by d3if_ShowTree (obj, 1); showAll == 0 shows all, else the entries from the index ai_GfxDetail (when
   that detail is not 2) are hidden */
void ai_ShowVgObjects(int start, int showAll)
{
    int list[0x21];
    int i;
    int v;

    memset(list, 0, 0x84);
    d3_ForEachObject(d3_DefaultScene, (struct D3OBJECT *)start, 0x2c0,
                     (char (*)(struct D3OBJECT *, void *))ai_VgFilter, (AIVGLIST *)list);
    for (i = 0; i < list[0x20]; i++) {
        v = list[i];
        if (showAll == 0 ||
            (i >= ai_GfxDetail && ai_GfxDetail != 2))
            d3if_ShowTree((struct D3OBJECT *)v, 0);
        else
            d3if_ShowTree((struct D3OBJECT *)v, 1);
    }
}
/* 0x58fa20, whether a building site may be built on, shown for its object (310 B): a site is blocked (0) when a
   character of the build query (sm_BuildQuery (1, 6) .. gm_BuildNext) stands within 100.0 of the site object's
   owner (+0x1f8) or of one of its "bk_" / "!bk_" named children (+0x1fc list); 1 when free. The result is passed
   to ai_ShowVgObjects (obj, result), which shows / hides the "vg_" markers of the object */

void ai_CheckBuildable(int obj)
{
    AIOBJVIEW *view;
    AIOBJVIEW *owner;
    AIOBJVIEW *site;
    AIBLD *bld;
    float here[4];
    float there[4];
    int buildable;

    buildable = 1;
    owner = ((AIOBJVIEW *)obj)->parent;
    if (owner != 0) {
        d3_PointToWorld((struct D3OBJECT *)owner, &((D3OBJECT *)owner)->pos,
                        (struct D3VECTOR *)here);
        for (bld = (AIBLD *)gm_BuildQuery(1, 6); bld != 0; bld = (AIBLD *)gm_BuildNext()) {
            site = (AIOBJVIEW *)bld->object;
            if (site != 0) {
                d3_PointToWorld((struct D3OBJECT *)site, &((D3OBJECT *)site)->pos,
                                (struct D3VECTOR *)there);
                if (d3_VecEqual((struct D3VECTOR *)there, (struct D3VECTOR *)here, 100.0f) != 0) {
                    buildable = 0;
                    break;
                }
            }
        }
    }
    for (view = ((AIOBJVIEW *)obj)->child; view != 0; view = view->next) {
        if (strncmp((char *)view, "bk_", 3) == 0 || strncmp((char *)view, "!bk_", 4) == 0) {
            d3_PointToWorld((struct D3OBJECT *)view, &((D3OBJECT *)view)->pos,
                            (struct D3VECTOR *)here);
            for (bld = (AIBLD *)gm_BuildQuery(1, 6); bld != 0; bld = (AIBLD *)gm_BuildNext()) {
                site = (AIOBJVIEW *)bld->object;
                if (site != 0) {
                    d3_PointToWorld((struct D3OBJECT *)site, &((D3OBJECT *)site)->pos,
                                    (struct D3VECTOR *)there);
                    if (d3_VecEqual((struct D3VECTOR *)there, (struct D3VECTOR *)here, 100.0f) != 0) {
                        buildable = 0;
                        goto done;
                    }
                }
            }
        }
    }
done:
    ai_ShowVgObjects(obj, buildable);
}
/* 0x58fb60, checks every placement candidate (62 B): ai_FindBauplaetze (prefix 0) fills the 0x100-entry
   candidate list, then ai_CheckBuildable runs on each non-null entry */

void ai_CheckBuildSites(void)
{
    int list[0x100];
    int n;
    int i;

    n = ai_FindBauplaetze(0, list);
    for (i = 0; i < n; i++) {
        if (list[i] != 0)
            ai_CheckBuildable(list[i]);
    }
}
/* 0x58fba0, the "bk_" site finder of 0x58fc40 (148 B, a d3_ForEachObject callback found through the pointer):
   objects whose name (or name + 1) starts with "bk_" and that stand within 100.0 of ctx->pos are stored to
   ctx->object; the part of the name from the "bk_" prefix on is copied to ctx->name; returns 0 (stops the
   traversal), 1 when the object does not match */

char ai_FindBauplatz(int obj, AIBUILDSITE *ctx)
{
    float pos[4];
    char *s;

    if (strnicmp((char *)obj, "bk_", 3) != 0 && strnicmp((char *)(obj + 1), "bk_", 3) != 0)
        return 1;
    d3_PointToWorld((struct D3OBJECT *)obj, &((D3OBJECT *)obj)->pos, (struct D3VECTOR *)pos);
    if (d3_VecEqual((struct D3VECTOR *)ctx->pos, (struct D3VECTOR *)pos, 100.0f) != 0) {
        ctx->object = obj;
        s = strstr((char *)obj, "bk_");
        if (s != 0)
            strcpy(ctx->name, s);
        return 0;
    }
    return 1;
}

char d3if_SelectScene(int index, char keep);  /* 0x4478e0 (d3_interface.c) */

/* 0x58fc40, the construction site of a building record (247 B): the record's object (+0x61) must exist;
   d3if_SelectScene (0, 1) brackets the work; the record's kind byte names the building (ai_BuildingName) into
   out->name, the owner position goes to out->pos and is formatted (sprintf,
   "lc_GetGebaeudeBauplatz(): FindPos->Pos:%f  %f  %f") into the unused 0x100-byte local; the "bk_" finder
   ai_FindBauplatz (d3_ForEachObject, mask 0x180) over d3_DefaultScene fills out->object; 1 when a site was
   found, 0 else. out is the first parameter, the building record the second (every caller, cm.c / he.c / 0x5b7690, pushes (out, b)) */
int lc_GetGebaeudeBauplatz(AIBUILDSITE *out, AIBLD *b)
{
    char text[0x100];
    int scene;

    if (b->object == 0)
        return 0;
    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    out->nameHead = 0;
    if (ai_BuildingName(b->type, out->name) == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3_PointToWorld((struct D3OBJECT *)b->object, &((D3OBJECT *)b->object)->pos,
                    (struct D3VECTOR *)out->pos);
    sprintf(text, "lc_GetGebaeudeBauplatz(): FindPos->Pos:%f  %f  %f",
            (double)out->pos[0], (double)out->pos[1], (double)out->pos[2]);
    out->object = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x180,
                     (char (*)(struct D3OBJECT *, void *))ai_FindBauplatz, out);
    if (out->object == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3if_SelectScene(scene, 1);
    return 1;
}

int d3t_DropObject(); 

struct XOBJ;
void d3_LocalToWorld(struct XOBJ *obj, float *in, float *out);  /* 0x458c10 (d3_pose.c) */
void d3_DirToWorld(struct XOBJ *obj, float *in, float *out);    /* 0x458810 (d3_pose.c) */
float d3_VecAngle(struct D3VECTOR *a, struct D3VECTOR *b);      /* 0x4539f0 (d3_vec.c) */
extern const struct D3VECTOR4 d3_ZAxis;                          /* 0x5ebb74, (0, 0, 1) */

/* 0x58fd40, the world position of a plot record's origin, written to out (79 B): d3if_SelectScene (0, 1) brackets
   the work; the zeroed 0x10-byte vector goes through d3_LocalToWorld (the plot, the vector, out), which applies
   the record's matrix and its parent chain; d3_CurrentScene is selected again afterwards */
void ai_ObjectPosition(void *out, int plot)
{
    float v[4];
    int scene;

    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    d3_LocalToWorld((struct XOBJ *)plot, v, (float *)out);
    d3if_SelectScene(scene, 1);
}
/* 0x58fd90, the rotation angle of a plot record (123 B): d3if_SelectScene (0, 1) brackets the work; out[0..2] is
   cleared, the zeroed vector takes the record's world direction (d3_DirToWorld on d3_ZAxis) and its angle to
   d3_ZAxis (d3_VecAngle) is stored to out[1]; d3_CurrentScene is selected again afterwards */

void ai_ObjectAngle(void *out_, int plot)
{
    float v[3];
    float *out = (float *)out_;
    int scene;

    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    d3_DirToWorld((struct XOBJ *)plot, (float *)&d3_ZAxis, v);
    out[1] = d3_VecAngle((struct D3VECTOR *)&d3_ZAxis, (struct D3VECTOR *)v);
    d3if_SelectScene(scene, 1);
}
/* 0x58fe10, the nearest object of a 0x100-entry list within 100.0 (157 B): the horizontal distance of the
   position ref (x, z) to each object's position (+0x4c) picks the nearest below 100.0; 0 when none */

int ai_FindNearestObject(float *ref, int *list)
{
    int i;
    int obj;
    float best;
    float p[4];
    float pos[4];
    float d, dx, dz;

    obj = 0;
    best = 1e+08f;
    pos[0] = ref[0];
    pos[2] = ref[2];
    for (i = 0; i < 0x100; i++) {
        if (list[i] != 0) {
            d3_PointToWorld((struct D3OBJECT *)list[i], (struct D3VECTOR *)(list[i] + 0x4c),
                            (struct D3VECTOR *)p);
            p[1] = 0.0f;
            dx = p[0] - pos[0];
            dz = p[2] - pos[2];
            d = (float)sqrt(dx * dx + dz * dz);
            if (d < best && d < 100.0) {
                best = d;
                obj = list[i];
            }
        }
    }
    return obj;
}
/* 0x58feb0, drops the object obj onto the target position (78 B): the position is set (d3_SetObjectPos) and the
   object dropped (d3t_DropObject); when its world y (d3_PointToWorld of the local position obj->pos, written into
   the engine's padded vector) is below target->y, the position is set again */
void ai_DropObjectAt(struct D3OBJECT *obj, struct D3VECTOR *target)
{
    D3VECTOR4 p;

    d3_SetObjectPos(obj, target);
    d3t_DropObject(obj, 1, 0);
    d3_PointToWorld(obj, &obj->pos, (struct D3VECTOR *)&p);
    if (p.y < target->y)
        d3_SetObjectPos(obj, target);
}

unsigned char * gm_BuildQuery(int n, ...); unsigned char * gm_BuildNext(void); int gm_MayTakeJob(int k, int c);
char *strupr(char *); int sprintf(char *buf, const char *fmt, ...); char * vfs_GetPath();
struct D3OBJECT * d3_LoadGroup(); void d3t_InflateObjectTree(D3OBJECT *obj); int d3t_DropObject();
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);  /* 0x447190, d3_interface.h */
int gm_BuildingKindCost(int c, int kind); void txt_Format(char *out, int id0, ...);
char d3sm_GetVertex();  /* 0x45f100: this caller tests the char result (al) */
char d3sm_Pick();  /* 0x45f2b0, d3_sm.h: the cell under a screen point, 0 = none */
void sv_SetStatusText(char *s);  /* 0x52e330, sv.h */
extern char ai_DataPath[];  /* 0x75dca0, the data path (am.h) */

/* 0x58ff00, lets the player place a building of the kind kind (2709 B; the name of its own error strings): a
   kind of class 2 the player already owns (build query 2, 4, the player, 5, 2) asks first (text 0x17f2 with the
   kind's name text 14 * kind + 0x48e); the models "<data>gebaeude/*GB_<NAME>.ogr" and "..._A .. _Z..." that the vfs
   holds go to names[]; the "bk_" sites of the building name (ai_FindBauplaetze) become ai_PlacementSites and get a
   banner each (sp_BAU_WIMPEL, coloured 0xc0a080, animated); the banner nearest to the camera starts the loop:
   a random model of names[] follows the mouse over the height map (d3sm_Pick) or the map window's pick
   (kb_Last '2', the building icon or the compass), snaps to the nearest site (ai_FindNearestObject, turned by
   ai_ObjectAngle; green 0x40c040 on a site, red 0xc04040 off it), and a left release on a site asks the
   confirmation (text 0x17ef: the cost, the name, AIBTYPE.v243); returns the chosen site object and copies the
   picked model name names[idx] to out (out = "" and 0 when none was chosen)
   @deep Reconstructed in full from the original 0x58ff00..0x590995 (the name loop, the banner loop, the
   interactive placement loop, the confirm and the cleanup), every call, store and string read against it;
   refs_differ=0, ours 786 vs 787 instructions, frame equal. Left: 14 code lines of allocation/scheduling
   only: +0x311 the original copies the banner to eax before storing sel and cur (mov eax, esi; mov [sel],
   eax; mov [cur], eax) where ours stores esi twice (tried sel = cur = obj, cur = sel, D3OBJECT * sel, sel
   = flags[i]: no change); +0x7ba fld v.y; fadd cm_Pi in the original vs fld cm_Pi; fadd v.y in ours (v.y
   += cm_Pi, v.y = v.y + cm_Pi and cm_Pi + v.y all give ours); +0x97f the original pushes the known-zero
   esi for sv_MessageBox(txt_Ptrs[0x17f0], 0) where ours pushes 0 (the original keeps esi as the zero
   register through the banner loop). The rest is slots / order / regs. A variant round. */
int lc_GebaeudeAusrichten(int kind, char *out)
{
    char names[128][0x60];          /* the model files of the kind found in the vfs */
    char text[0x400];               /* the confirmation text */
    char found[0x100];              /* vfs_GetPath's output */
    int flags[0x100];               /* the banner per site */
    int sites[0x100];               /* the "bk_" site objects (ai_FindBauplaetze) */
    char flagPath[0x100];
    char path[0x100];
    D3MATRIX mat;
    char bname[0x40];
    char buf[0x100];                /* "gb_<NAME>", then the error message */
    D3VECTOR4 bmin;                 /* the model's bounds (unused) */
    D3VECTOR4 bmax;
    D3VECTOR ang;
    D3VECTOR v;
    D3VECTOR rot;
    D3VECTOR4 pos;                  /* the engine's padded vector */
    D3TEXFLAGS tf;
    struct D3HMAP *hmap;
    D3OBJECT *model;
    D3OBJECT *obj;
    AIBTYPE *type;
    unsigned char *b;
    float letter;
    float shown;
    float best;
    float d;
    float dx;
    float dy;
    float dz;
    int result;
    int color;
    int n;
    int reload;
    int sel;                        /* the banner nearest to the camera */
    int cur;
    int hit;
    int firstPick;
    int idx;
    int count;
    int picked;
    int ok;
    int px;
    int pz;
    int i;

    hmap = d3_DefaultScene->hmap;
    model = 0;
    result = 0;
    color = 0;
    n = 0;
    reload = 1;
    sel = 0;
    cur = 0;
    best = 1e+09f;
    firstPick = 1;
    if (((AIBTYPE *)ai_BTypes)[kind].kind == 2) {
        for (b = gm_BuildQuery(2, 4, ai_PlayerId, 5, 2); b != 0; b = gm_BuildNext()) {
            if (*b == kind) {
                char ask[0x400];    /* the question whether to build another one */

                txt_Format(ask, 0x17f2, kind * 14 + 0x48e);
                if (sv_MessageBox(ask, 1) == 0)
                    return 0;
                break;
            }
        }
    }
    memset(sites, 0, sizeof(sites));
    memset(flags, 0, sizeof(flags));
    strcpy(buf, "gb_");
    strcat(buf, ((AIBTYPE *)ai_BTypes)[kind].name);
    strupr(buf + 2);
    for (letter = 0.0f; letter < 128.0f; letter += 1.0f) {
        if (letter != 0.0f)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, buf, (char)(letter + 'A' - 1.0f));
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, buf);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, found) != 0) {
            strcpy(names[n++], path);
        } else if (letter != 0.0f)
            break;
    }
    if (n == 0) {
        sprintf(buf, "lc_GebaeudeAusrichten(): Could not find any 3D-Object-Group fitting this building...", path);
        err_Message(buf);
        return 0;
    }
    idx = util_Random(n) & 0xffff;
    if (((AIBTYPE *)ai_BTypes)[kind].kind == '!')
        idx = ((AICHARREC *)ai_Chars)[ai_PlayerId].sex != 1;
    if (ai_BuildingName(kind, bname) == 0)
        return 0;
    count = ai_FindBauplaetze(bname, sites);
    rot.x = 0;
    rot.y = 0;
    rot.z = 0;
    ai_PlacementSites = sites;
    if (count > 0) {
        i = 0;
        do {
            if (sites[i] != 0) {
                d3_PointToWorld((D3OBJECT *)sites[i], &((D3OBJECT *)sites[i])->pos, (D3VECTOR *)&pos);
                sprintf(flagPath, "%s*sp_BAU_WIMPEL.ogr", ai_DataPath);
                obj = d3_LoadGroup(flagPath, 0, 0);
                flags[i] = (int)obj;
                d3t_InflateObjectTree(obj);
                d3_SetObjectPos(obj, (D3VECTOR *)&pos);
                obj->objectFlags &= ~2;
                tf.all = 0;
                tf.b[2] = 1;
                tf.b[0] = 0x80;
                d3_SetObjectAlpha(obj, obj->info, tf);
                d3t_SetTreeColor(obj, 0xc0, 0xa0, 0x80);
                dx = pos.x - d3_Camera->pos.x;
                dy = pos.y - d3_Camera->pos.y;
                dz = pos.z - d3_Camera->pos.z;
                d = (float)sqrt(dz * dz + dy * dy + dx * dx);
                if (d < best) {
                    best = d;
                    sel = cur = (int)obj;
                }
                d3t_PlayAnim(obj, "sonstiges\\sp_WIMPEL.baf", 1);
            }
            i++;
        } while (i < count);
        if (sel != 0) {
            d3_LightInvalidate(1);
            di_ClearEvents(2);
            kb_Last = '2';
            do {
                if (reload) {
                    /* a new model of names[] at the old one's place */
                    shown = 0.0f;
                    if (model != 0) {
                        pos.x = model->pos.x;
                        pos.y = model->pos.y;
                        pos.z = model->pos.z;
                        rot.x = model->rotation.x;
                        rot.y = model->rotation.y;
                        rot.z = model->rotation.z;
                        d3if_Delete(model);
                    }
                    strcpy(path, names[util_Random(n) & 0xffff]);
                    model = d3_LoadGroup(path, 0, 0);
                    if (model == 0) {
                        sprintf(buf, "lc_GebaeudeAusrichten(): Could not load 3D-Object-Group '%s'...", path);
                        err_Message(buf);
                        i = 0;
                        do {
                            if (flags[i] != 0)
                                d3if_Delete((D3OBJECT *)flags[i]);
                            i++;
                        } while (i < count);
                        return 0;
                    }
                    model->objectFlags &= ~2;
                    d3_SetObjectPos(model, (D3VECTOR *)&pos);
                    d3t_DropObject(model, 1, 0);
                    cur = ai_FindNearestObject((float *)&pos, sites);
                    if (cur != 0) {
                        ai_ObjectAngle(&rot, cur);
                        d3_SetObjectRot(model, &rot);
                        d3t_DropObject(model, 1, 0);
                        sv_SetStatusText(" ");
                        if (color != 1) {
                            d3t_SetTreeColor(model, 0x40, 0xc0, 0x40);
                            color = 1;
                        }
                    } else {
                        sv_SetStatusText(txt_Ptrs[0x17f1]);
                        if (color != 2) {
                            d3t_SetTreeColor(model, 0xc0, 0x40, 0x40);
                            color = 2;
                        }
                    }
                    model->rotation.x = 0;
                    model->rotation.y = 0;
                    model->rotation.z = 0;
                    model->pos.x = 0;
                    model->pos.y = 0;
                    model->pos.z = 0;
                    d3t_GetObjectBounds(&bmin, &bmax, model);
                    model->pos.x = pos.x;
                    model->pos.y = pos.y;
                    model->pos.z = pos.z;
                    model->rotation = rot;
                    reload = 0;
                }
                if (kb_Last == '2' ||
                    (dm_L.lclick != 0 && (d2_PointerObj == sv_PanelBuildingIcon || d2_PointerObj == sv_PanelCompass))) {
                    /* the map window picks the place */
                    picked = ai_MapPick(8, 0, txt_Ptrs[0x17ec], 0, 0);
                    if (picked != 0) {
                        sv_FlyCameraToModel(picked);
                        d3_PointToWorld((D3OBJECT *)picked, &((D3OBJECT *)picked)->pos, (D3VECTOR *)&pos);
                        ai_DropObjectAt(model, (D3VECTOR *)&pos);
                    } else if (firstPick) {
                        sv_PopRequest = 1;
                    }
                    firstPick = 0;
                }
                hit = ai_FindNearestObject((float *)&model->pos, sites);
                if (hit != 0) {
                    if (hit != cur) {
                        cur = hit;
                        d3_PointToWorld((D3OBJECT *)hit, &((D3OBJECT *)hit)->pos, (D3VECTOR *)&pos);
                        d3_SetObjectPos(model, (D3VECTOR *)&pos);
                        ai_ObjectAngle(&ang, hit);
                        d3_SetObjectRot(model, &ang);
                    }
                    sv_SetStatusText(" ");
                    ok = 1;
                    if (color != 1) {
                        d3t_SetTreeColor(model, 0x40, 0xc0, 0x40);
                        color = 1;
                    }
                } else {
                    cur = 0;
                    sv_SetStatusText(txt_Ptrs[0x17f1]);
                    ok = 0;
                    if (color != 2) {
                        d3t_SetTreeColor(model, 0xc0, 0x40, 0x40);
                        color = 2;
                    }
                }
                if (dm_L.rup != 0)
                    sv_PopRequest = 1;
                if (model != 0 && shown < 90.0f) {
                    /* the model half transparent, once per model */
                    model->stateFlags &= ~2;
                    tf.all = 0;
                    tf.b[2] = 1;
                    tf.b[0] = 0xdd;
                    d3_SetObjectAlpha(model, model->info, tf);
                    model->stateFlags |= 2;
                    if (ok) {
                        if (color != 1) {
                            d3t_SetTreeColor(model, 0x40, 0xc0, 0x40);
                            color = 1;
                        }
                    } else {
                        if (color != 2) {
                            d3t_SetTreeColor(model, 0xc0, 0x40, 0x40);
                            color = 2;
                        }
                    }
                    shown = 90.0f;
                }
                if (sv_UpdateCamera() == 0) {
                    /* the model follows the mouse over the height map */
                    if (d3sm_Pick(hmap, dm_L.x, dm_L.y, &px, &pz) && d3sm_GetVertex(hmap, px, pz, (D3VECTOR *)&pos)) {
                        d3_SetObjectPos(model, (D3VECTOR *)&pos);
                        d3t_DropObject(model, 1, 0);
                    }
                    d3_RelightObject(model);
                    v.x = model->rotation.x;
                    v.y = model->rotation.y;
                    v.z = model->rotation.z;
                    v.y += 3.14159274f;
                    d3_MatFromAngles((D3VECTOR4 *)&v, &mat);
                }
                if (dm_L.lup != 0 && ok) {
                    type = (AIBTYPE *)ai_BTypes + kind;
                    i = gm_BuildingKindCost((int)ai_Chars[ai_PlayerId], kind);
                    txt_Format(text, 0x17ef, i, kind * 14 + 0x48e, type->v243);
                    if (sv_MessageBox(text, 1) != 0) {
                        sv_PopRequest = 1;
                        result = cur;
                    } else {
                        sv_PopRequest = 1;
                        result = 0;
                    }
                }
            } while (sv_RunFrame((int)lc_GebaeudeAusrichten, 0x657c7) != 0);
            if (result == 0) {
                d3if_Delete(model);
                out[0] = ch_EmptyName[0];
                model = 0;
            } else {
                strcpy(out, names[idx]);
            }
            i = 0;
            do {
                if (flags[i] != 0)
                    d3if_Delete((D3OBJECT *)flags[i]);
                i++;
            } while (i < count);
            if (model != 0)
                d3if_Delete(model);
            return result;
        }
    }
    sv_MessageBox(txt_Ptrs[0x17f0], 0);
    return 0;
}

int ai_RunAllows(); int d2_OpenForm();
int d2_SetForm(int view, int n); int d2_GetViewWin(); int io_PrintText(char *fmt, ...);
int d2_WinOpenPic(short x, short y, int pic, int win); void d2_SetObjFont(); void d2_SetObjText();  /* d2_CreateWinText: d2.h's prototype; ai_ShowProdSlot's 16-bit calls go through d2_CreateWinTextS */
void d2_SetViewEnabled(); void d2_CloseView(int view); int cm_SendLongMessage(char *msg);
void txt_Format(char *out, int id0, ...);
int gm_CharCurrency(char *c, char p); int gm_BuildingClass(); int gm_MayTakeJob(int k, int c);

#define sv_CheckStockB ((int (*)(AIROOM *, int, int, unsigned char))sv_CheckStock)  /* @original ai_ConstructionWindow pushes the money kind as a byte (only cl loaded) */
#define sv_CheckGoodAmountB ((int (*)(char *, int, unsigned char))sv_CheckGoodAmount)  /* @original as sv_CheckStockB */

/* 0x5909a0, the construction window "BAUEN\GEB_BAUEN" (1988 B): run by ai_RunAllows (0x40) between the campaign
   steps 0xe (0, 0, 0) / (0, 1, 0); the window's padding 0x18 and its scroll buttons; the form 1 title (text 0x17eb)
   and the group buttons 1 .. 9 of form 2 (pictures 0x6d5 ..; 2 and 5 are left out; group 3 needs the office 0xf or
   0xa, group 7 a job 0x34 .. 0x39, else greyed; the caption is the group's text 0x95e + group); the checked button
   is ai_BuildGroup. A click on a group button selects it; a new group rebuilds form 3 with the building kinds of
   the group the player may build (gm_MayTakeJob 1: the picture 0x427 + kind with the kind's name and its cost,
   3 per row; 2: an empty picture 0x4fb; the classes 0xf and 6 are left out; group 3 also sets sv_ResearchClass and
   pays from the player's seat object). A click on a kind checks the money (the seat's stock or the character's),
   lc_GebaeudeAusrichten places it and the build message 0xd (the kind, its cost, the site's position and angles,
   the model) is sent; the right button closes the window
   @deep Reconstructed in full from the original 0x5909a0..0x591164: the group buttons (2 and 5 skipped, 3
   / 7 gated by officeKind / jobId), the kind list per group (gm_BuildingClass, gm_MayTakeJob 1 / 2, the
   kind name and cost text), the click handling (sv_CheckStock on the seat for group 3, else
   sv_CheckGoodAmount; the byte-pushed money kind is the @original cast), lc_GebaeudeAusrichten and the
   build message 0xd (AIMSG.buildPos / buildRot / buildPath). ours=547 original=550. Left: register
   allocation and scheduling only: the original keeps ebx as the zero register from the prologue on (push
   ebx before the ai_RunAllows test; ours saves esi there), the group index in esi, the kind loop's name
   pointer txt_Ptrs + 0x48e*4 walked in [esp+0x10] and the strcpy of the kind's name spilled through
   [esp+0x1c] / [esp+0x17] (frame 0x2cc vs ours 0x2b4: these spills); ours holds them in ebx / edi; e.g.
   +0xd5e: ours 'mov ecx, [ebx]; mov bl, [ecx]' vs the original 'mov ecx, [edx]; mov [esp+0x1c], ecx; ...
   mov dl, [edx]; mov [esp+0x17], dl'. A variant round. (references: M8 at +0x68a: ours refers to
   ai_Time+0x8 (0x770ae8), the original's field to 0x88 (and 1 more), explained: the two M8 lines are the
   alignment pairing two loads of the build message's field copies that the schedulers order differently
   (ours loads ai_Time+8 and ai_BuildGroup at other places in the same block; both sides copy ai_Time's 14
   bytes into msg.time and test ai_BuildGroup == 3 once)) */
void ai_ConstructionWindow(void)
{
    AIMSG msg;
    char amount[0x70];              /* the cost text of a kind */
    char model[0x100];              /* the model lc_GebaeudeAusrichten picked */
    int groups[10];                 /* the group buttons */
    AIROOM *seat;                   /* group 3: the player's seat object that pays */
    AIBTYPE *type;
    int prevGroup;
    int pointed;                    /* the kind button clicked last, -1 = none */
    int view;
    int down;
    int up;
    int bar;
    int off;
    int ctl;
    int text;
    int site;
    int kind;
    int cost;
    int ok;
    int x;                          /* a kind button's place: 3 per row, 70 x 0x5a apart */
    int y;
    int n;
    int i;

    pointed = -1;
    prevGroup = -1;
    seat = 0;
    if (ai_RunAllows(0x40) == 0)
        return;
    ai_CampaignStep(0xe, 0, 0, 0);
    view = d2_OpenForm(0, 0, "BAUEN\\GEB_BAUEN");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 0);
    d2_Wins[d2_LastWindow].padl = 0x18;
    d2_Wins[d2_LastWindow].padt = 0x18;
    d2_Wins[d2_LastWindow].padr = 0x18;
    d2_Wins[d2_LastWindow].padb = 0x18;
    sv_CreateScrollButtons(0x20c, 0x168, d2_LastWindow, d2_GetViewWin(view, 3), 0x5a, &down, &up, &bar);
    d2_SetForm(view, 1);
    io_PrintText(0x17eb);
    d2_SetForm(view, 2);
    n = 0;
    off = 0;
    for (i = 1; i < 10; i++) {
        if (i == 2 || i == 5)
            continue;
        if (i == 3) {
            if (((AICHARREC *)ai_Chars)[ai_PlayerId].officeKind != 0xf &&
                ((AICHARREC *)ai_Chars)[ai_PlayerId].officeKind != 0xa)
                off = 1;
        } else if (i == 7) {
            if (((AICHARREC *)ai_Chars)[ai_PlayerId].jobId < 0x34 || ((AICHARREC *)ai_Chars)[ai_PlayerId].jobId > 0x39)
                off = 1;
        }
        ctl = d2_WinOpenPic((n % 2) * 70 + 0x30, (n / 2) * 0x5a + 0xf, i + 0x6d4, d2_LastWindow);
        groups[i] = ctl;
        if (off == 0) {
            d2_Objs[ctl].interactive = 1;
            strcpy(d2_Objs[ctl].caption, txt_Ptrs[0x95e + i]);
            d2_Objs[ctl].style = 3;
            d2_Objs[ctl].capfont = 0x54;
            d2_Objs[ctl].captionY = -1;
        } else {
            d2_Objs[ctl].clickDisabled = 1;
            d2_Objs[ctl].greyed = 1;
        }
        n++;
        off = 0;
    }
    d2_Objs[groups[ai_BuildGroup]].checked = 1;
    do {
        if (d2_ClickedId >= 0x6d4 && d2_ClickedId < 0x6de) {
            /* a group button */
            ai_BuildGroup = d2_ClickedId - 0x6d4;
            d2_Objs[groups[ai_BuildGroup]].checked = 1;
            if (prevGroup != -1)
                d2_Objs[groups[prevGroup]].checked = 0;
        }
        if (ai_BuildGroup != prevGroup) {
            /* the kinds of the group */
            d2_SetForm(view, 3);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            seat = 0;
            if (ai_BuildGroup == 3) {
                sv_ResearchClass = 1;
                seat = (AIROOM *)gm_PlayerSeat(1);
            } else {
                sv_ResearchClass = 0;
            }
            n = 0;
            for (kind = 0; kind < 88; kind++) {
                if (gm_BuildingClass(kind) != ai_BuildGroup)
                    continue;
                type = (AIBTYPE *)ai_BTypes + kind;
                ok = gm_MayTakeJob(kind, ai_PlayerId);
                if (ok == 1) {
                    if (type->kind == 0xf || type->kind == 6)
                        continue;
                    x = (n % 3) * 70;
                    y = (n / 3) * 0x5a;
                    ctl = d2_WinOpenPic(x + 0x30, y + 0xf, kind + 0x427, d2_LastWindow);
                    d2_Objs[ctl].checkable = 1;
                    strcpy(d2_Objs[ctl].caption, txt_Ptrs[0x48e + kind * 14]);
                    d2_Objs[ctl].style = 3;
                    d2_Objs[ctl].capfont = 0x54;
                    gm_FormatAmount(gm_BuildingKindCost((int)ai_Chars[ai_PlayerId], kind), ai_OwnPlayer, amount);
                    text = d2_CreateWinText(x + 2, y + 0x3f, ch_EmptyName, d2_LastWindow);
                    d2_SetObjFont(text, 0x54);
                    d2_SetObjText(text, amount);
                    d2_Objs[text].w = 0x8c;
                    d2_Objs[text].centred = 1;
                } else if (ok == 2) {
                    ctl = d2_WinOpenPic((n % 3) * 70 + 0x30, (n / 3) * 0x5a + 0xf, 0x4fb, d2_LastWindow);
                    d2_Objs[ctl].caption[0] = ch_EmptyName[0];
                    d2_Objs[ctl].style = 3;
                    d2_Objs[ctl].capfont = 0x54;
                } else {
                    continue;
                }
                n++;
            }
            prevGroup = ai_BuildGroup;
        }
        if (d2_ClickedId >= 0x427 && d2_ClickedId <= 0x47f) {
            /* a kind button: pay, place and send the build message */
            kind = d2_ClickedId - 0x427;
            if (pointed != -1) {
                d2_Objs[pointed].checked = 0;
                d2_Objs[pointed].highlighted = 0;
            }
            pointed = d2_PointerObj;
            if ((seat != 0 &&
                 sv_CheckStockB(seat, seat->list, gm_BuildingKindCost((int)ai_Chars[ai_PlayerId], kind), ai_OwnPlayer)) ||
                (seat == 0 && sv_CheckGoodAmountB(ai_Chars[ai_PlayerId],
                                                  gm_BuildingKindCost((int)ai_Chars[ai_PlayerId], kind), ai_OwnPlayer))) {
                d2_SetViewEnabled(view, 0);
                site = lc_GebaeudeAusrichten(kind, model);
                if (site != 0) {
                    msg.time = ai_Time;
                    msg.type = 0xd;
                    msg.flags = 2;
                    msg.to = msg.fromId = ai_CharInts[ai_PlayerId][0];
                    if (ai_BuildGroup == 3)
                        msg.arg[0] = ((AICHARREC *)ai_PlayerChar)->netId;
                    else
                        msg.arg[0] = msg.to;
                    msg.arg[1] = kind;
                    msg.arg[2] = gm_BuildingKindCost((int)ai_Chars[ai_PlayerId], kind);
                    d3_PointToWorld((D3OBJECT *)site, &((D3OBJECT *)site)->pos, (D3VECTOR *)msg.buildPos);
                    ai_ObjectAngle(msg.buildRot, site);
                    strncpy(msg.buildPath, model, 0x3f);
                    cm_SendLongMessage((char *)&msg);
                    sv_PopRequest = 1;
                } else {
                    d2_SetViewEnabled(view, 1);
                }
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_ConstructionWindow, 0x657c7) != 0);
    if (view != -1)
        d2_CloseView(view);
    sv_ResearchClass = 0;
    ai_CampaignStep(0xe, 0, 1, 0);
}

int gm_IsHomeType();
unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c); void init_ShowBuildingScene(unsigned char *b, int slot); void init_FinishBuildingScene(unsigned char *b, unsigned short *o);
int d3t_PlaceObjectV(struct D3OBJECT *camera, struct D3VECTOR *pos, struct D3VECTOR *dir, int unk, char flags); int gm_ClockSeason(); void init_AfterTownLoad(void); void d3fl_MakeDivide();
void ai_ChooseCoatOfArms(void); void snd_StartMarketAmbience(void); int tb_SetFunctionOff(); void snd_StopMarketAmbience(void);
void d2_CopyCaption(char *caption);
struct AIBLD; int ai_ScriptQuery();
int gm_IsType10(unsigned char *b);
int d2_GetWinObj(); void ai_TechEffectsWindow(AIBLD *bld);
float gm_ItemPrice(int item, unsigned char n);
void cm_BeginGroup(const char *s); void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int cm_SendMoveGoods(int a, int b, unsigned char c, int d); short * gm_PlayerRecord(char *c);
int cm_SendLongMessage(char *msg); int cm_EndGroup(const char *s); int cm_GetSlotState(); void cm_SendMoneyDelta(int a, int b);
char *strupr(char *); void gm_CloseTechTree(void);
void ai_MasterLetterWindow();

void ai_GameFrame(void);
void lc_PlayerInJail(void);

/* 0x591170, the jail / building-question helper (323 B): in the '2' state or when the two window ids match
   (0x5fe710 against 0x623838 / 0x62383c), the record 0x5b2180 (1, the 0x50-byte message local zeroed with
   +0x13 = 0x71f, 0, 0, 0) is resolved: the pending contact 0x6f230c (when set) gets 0x521960 (text 0xfd / -1);
   else 0x502520 and the 'G' kind get 0x521960 (0xfd), the other kinds 0x527840 (the plain open) or 0x521960
   (the record's +0x29 text) */
void ai_HandleMapKey(void)
{
    int msg[0x14];
    AIBLD *p;
    int r;
    int u;

    if (kb_Last == '2' ||
        (dm_L.lclick != 0 &&
         (d2_PointerObj == sv_PanelBuildingIcon || d2_PointerObj == sv_PanelCompass))) {
        memset(msg, 0, 0x50);
        msg[4] = 0;
        msg[2] = 0;
        msg[0x13] = 0x71f;
        p = (AIBLD *)ai_MapPick(1, msg, 0, 0, 0);
        if (p != 0 && sv_OtherSelected == 0) {
            if (ai_FallbackBuilding != 0) {
                r = gm_IsHomeType((unsigned char *)p);
                if (r != 0 && p->type == 'G') {
                    sv_SelectPanelObject(p, 0xfd, (void *)(unsigned)&ch_EmptyName);
                    return;
                }
                sv_SelectPanelObject(p, -1, (void *)(unsigned)&ch_EmptyName);
                return;
            }
            r = gm_IsHomeType((unsigned char *)p);
            if (r == 0 || p->type != 'G') {
                if (ai_FallbackBuilding == 0 && ai_PlayerBuilding == 0) {
                    sv_FlyCameraToObject((int)p);
                    return;
                }
                sv_SelectPanelObject(p, p->mainRoom, (void *)(unsigned)&ch_EmptyName);
            } else {
                u = gm_PermissionBits(ai_PlayerId, (unsigned char *)p, 0, 0);
                if (u & 1) {
                    sv_SelectPanelObject(p, 0xfd, (void *)(unsigned)&ch_EmptyName);
                    return;
                }
            }
        }
    }
}
/* 0x5912c0, the meister / building checks of the trade window (445 B): when the selected goods record
   0x76045c is a book (kind 6 / 2 at 0x91aaf0 + goods * 0x41) and 0x503530 says the player owns it, the pending
   contact 0x6f230c is finished (0x4ffe90 (0x5d, 2, 6, 0, 0xfd), 0x59f410, 0x513050, 0x513400, 0x6f22a4 =
   0x6f229c); the pending request 0x75e8c0 / 0x75e8c8 / 0x75e8c4 resolves the same way; 0x6f22fc does so too;
   for the building kinds 0x1e / 0x1f / 0x20 (or the records' 0x1d / 0x1f) 0x58f510 runs */

void ai_TradeContactChecks(void)
{
    unsigned short *item;           /* the clicked object record: its first word is its goods id */
    unsigned short *room;
    AIBLD *b;
    AIBTYPE *type;
    AIGOOD *g;

    item = (unsigned short *)sv_ClickedItem;
    if (item != 0) {
        g = (AIGOOD *)ai_Goods + *item;
        if ((g->type == 6 || g->type == 2) &&
            (gm_PermissionBits(ai_PlayerId, (unsigned char *)ai_FallbackBuilding, item, 0) &
             0x4000) != 0) {
            room = (unsigned short *)gm_Query((void *)((AIBLD *)ai_FallbackBuilding)->objList, 2, 6, 0, 0xfd);
            ai_RunObjectRoom((AIOBJ *)sv_ClickedItem);
            init_ShowBuildingScene((unsigned char *)ai_FallbackBuilding, 0xfd);
            init_FinishBuildingScene((unsigned char *)ai_FallbackBuilding, room);
            sv_FrameBase = sv_FrameDepth;
        }
    }
    if (sv_PanelFlag != 0 && sv_QuickJump.record != 0 && sv_QuickJump.object == ai_FallbackBuilding) {
        room = (unsigned short *)gm_Query((void *)((AIBLD *)ai_FallbackBuilding)->objList, 2, 6, 0, 0xfd);
        if (sv_QuickJump.record != room) {
            ai_RunObjectRoom((AIOBJ *)sv_QuickJump.record);
            init_ShowBuildingScene((unsigned char *)ai_FallbackBuilding, 0xfd);
            init_FinishBuildingScene((unsigned char *)ai_FallbackBuilding, room);
        } else {
            sv_PanelFlag = 0;
            sv_QuickJump.record = 0;
            sv_QuickJump.object = 0;
        }
    }
    if (sv_ClickedContact != 0 && ai_SelectedCount == 0) {
        room = (unsigned short *)gm_Query((void *)((AIBLD *)ai_FallbackBuilding)->objList, 2, 6, 0, 0xfd);
        ai_RunObjectRoom((AIOBJ *)sv_ClickedContact);
        init_ShowBuildingScene((unsigned char *)ai_FallbackBuilding, 0xfd);
        init_FinishBuildingScene((unsigned char *)ai_FallbackBuilding, room);
    }
    b = (AIBLD *)ai_FallbackBuilding;
    type = (AIBTYPE *)ai_BTypes + b->type;
    if (b->type == 0x20 || b->type == 0x1f || b->type == 0x1e || type->kind == 0x1f ||
        type->kind == 0x1d)
        ai_MarketContact(b);
}
typedef struct SVFRAME {                /* 8 bytes, an entry of the frame stack 0x75f780 (sv.c sv_RunFrame: "MASTER()") */
    int cb;                             /* 0x00, the frame callback registered at this depth */
    char pad04[4];
} SVFRAME;
#define sv_Frames (*(SVFRAME (*)[0x80])((char *)&ai_Unk75f280 + 0x500))  /* 0x75f780, the screen's frame stack, indexed by sv_FrameDepth */

/* 0x591480, the '1' key of the game frame (called every frame by ai_GameFrame 0x591520; 150 B): when no
   other screen runs on top (the frame stack's entry is ai_GameFrame) and the camera exists, the tutorial
   event 0x16 runs and the camera is levelled: its rotation (x, 0, z) is compared with 0.1 and, when it still
   pitches, 0x411860 places the camera there. @original the parenthesized rotation copies give VC6's fld/fstp
   copy (a plain member copy moves through a GPR). Read against the original: kb_Last is a byte, the frame stack
   is read through the typed SVFRAME view (Q2/Q4), d3_VecEqual takes v first and the camera rotation second. */

void ai_CampaignKey1(void)
{
    if (kb_Last == '1' && d3_Camera != 0 &&
        sv_Frames[sv_FrameDepth].cb == (int)ai_GameFrame) {
        D3VECTOR v;

        ai_CampaignStep(0x16, 0, 0, 0);
        v.x = (d3_Camera->rotation.x);
        v.y = 0;
        v.z = (d3_Camera->rotation.z);
        if (d3_VecEqual(&v, &d3_Camera->rotation, 0.1f) == 0)
            d3t_PlaceObjectV(d3_Camera, &d3_Camera->pos, &v, 0x1e, 0x28);
    }
}
/* 0x591520, the main game frame loop (1445 B): the season of the clock, the terrain divided once
   (init_TerrainDivide), the mouse limits saved and restored around the loop, the ambient sounds on; in a network
   game (ai_GameFlags & 4) a relative of the player (the same AICHAR.ref54, kind 5 .. 7) with a higher netId makes
   the player choose a coat of arms; in the first round of a single game (& 1) the camera goes to the player's first
   building that is not the home. Per frame (sv_RunFrame (this, 0x67fff)): the jail check of a held player, the
   camera, the chimney smoke of the buildings by the day hours of the season (off before gm_DayStartHours / from
   gm_DayEndHours, on in between; mode 0 .. 3), the campaign key, the camera flight to the hovered building, the
   construction window from the panel's build button, entering the selected building (the right click or a
   left release with the pointer look 0xb, permission bit 1: the own / home building by the script query 0x1b,
   another one by 0x19), the market stands and the building the info panel picked (its room word, the camera,
   entered or only flown to); the exit clears the texts and the selection and restores the mouse */
void ai_GameFrame(void)
{
    AICHARREC *c;
    AIBLD *b;
    unsigned char *it;
    EVTSCRIPT *e;
    unsigned int perm;
    int season;
    int mode;
    int maxX;
    int maxY;
    int minX;
    int minY;
    int i;

    season = gm_ClockSeason(&ai_Time);
    d3t_BoxCornerIdx = -1;
    init_AfterTownLoad();
    if (init_TerrainDivide != 0) {
        d3fl_MakeDivide(d3_Terrain);
        init_TerrainDivide = 0;
    }
    maxX = di_MaxX;
    maxY = di_MaxY;
    minX = di_MinX;
    minY = di_MinY;
    snd_AmbientOn = 1;
    ai_GameState = 1;
    if (ai_GameFlags & 4) {
        for (i = 0; i < 768; i++) {
            c = &((AICHARREC *)ai_Chars)[i];
            if (c->id != 0xffff && c->ref54 == ((AICHARREC *)ai_Chars)[ai_PlayerId].ref54 && c->id != ai_PlayerId &&
                (c->kind == 6 || c->kind == 7 || c->kind == 5) && c->netId > ((AICHARREC *)ai_Chars)[ai_PlayerId].netId) {
                ai_ChooseCoatOfArms();
                break;
            }
        }
    }
    if ((ai_GameFlags & 1) && main_Round == 1) {
        for (it = gm_BuildQuery(1, 4, ai_PlayerId); it != 0; it = gm_BuildNext()) {
            if ((int)it != ((AICHARREC *)ai_Chars)[ai_PlayerId].bld2) {
                sv_CameraToObject(it);
                break;
            }
        }
    }
    snd_StartMarketAmbience();
    mode = 0;
    tb_SetFunctionOff(sv_UpdateClock, 0);
    while (sv_RunFrame((int)ai_GameFrame, 0x67fff) != 0) {
        sv_FrameBase = sv_FrameDepth;
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].heldRounds > 0)
            lc_PlayerInJail();
        sv_UpdateCamera();
        if (mode == 0) {
            if ((float)ai_Time.hour < gm_DayStartHours[season] || (float)ai_Time.hour >= gm_DayEndHours[season]) {
                for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
                    if (b->smokeEffect != -1) {
                        e = evt_FindById(b->smokeEffect);
                        if (e != 0)
                            evt_Stop(e);
                        b->smokeEffect = -1;
                    }
                }
                mode = 1;
            }
        } else if (mode == 1) {
            if ((float)ai_Time.hour >= gm_DayStartHours[season] && (float)ai_Time.hour < gm_DayEndHours[season]) {
                for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
                    if (b->smokeEffect == -1)
                        sv_UpdateChimneySmoke((struct AIBLD *)b);
                }
                mode = 2;
            }
        } else if (mode == 2) {
            if ((float)ai_Time.hour < gm_DayStartHours[season] || (float)ai_Time.hour >= gm_DayEndHours[season]) {
                for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
                    if (b->smokeEffect != -1) {
                        e = evt_FindById(b->smokeEffect);
                        if (e != 0)
                            evt_Stop(e);
                        b->smokeEffect = -1;
                    }
                }
                mode = 3;
            }
        }
        ai_CampaignKey1();
        if (dm_L.lhold != 0 && dm_L.rdown == 0 && sv_HoverBuilding != 0 && d3t_CamMove == 0)
            sv_FlyCameraToObject(sv_HoverBuilding);
        if (d2_ClickedId != -1 && d2_PointerObj == sv_PanelBuildBtn)
            ai_ConstructionWindow();
        if (sv_SelectedObject != 0) {
            perm = gm_PermissionBits(ai_PlayerId, (unsigned char *)sv_SelectedObject, 0, 0);
            b = (AIBLD *)sv_SelectedObject;
            if (b != 0 && sv_HoverBuilding == (int)b &&
                (dm_L.lextra != 0 || (dm_L.lup != 0 && d2_PointerLook == 0xb)) && (perm & 1) != 0 &&
                (b->flags5a & 1) == 0) {
                if (gm_IsHomeType(b) != 0 && ai_ScriptQuery(0x1b, b, 0, 0) == 1)
                    ai_EnterOwnBuilding(b);
                else if (gm_IsType10((unsigned char *)b) == 0 && ai_ScriptQuery(0x19, b, 0, b->mainRoom) == 1)
                    ai_EnterBuilding(b, b->mainRoom);
            }
        }
        if (ai_MarketBuilding != 0)
            ai_MarketStandListener(ai_MarketBuilding);
        if (sv_PanelFlag != 0) {
            /* the building the info panel picked */
            b = (AIBLD *)sv_QuickJump.object;
            if (sv_QuickJump.record != 0) {
                b->mainRoom = *sv_QuickJump.record;
                if ((b->flags5a & 1) != 0) {
                    sv_PanelFlag = 0;
                } else {
                    if (b->object != 0 && gm_IsType10((unsigned char *)b) == 0)
                        sv_CameraToObject(b);
                    if (gm_IsHomeType(b) != 0) {
                        ai_EnterOwnBuilding(b);
                    } else if (gm_IsType10((unsigned char *)b) == 0) {
                        ai_EnterBuilding(b, b->mainRoom);
                    } else if (gm_IsType10((unsigned char *)b) != 0) {
                        sv_FlyCameraToObject((int)b);
                        sv_PanelFlag = 0;
                    }
                }
            } else {
                sv_FlyCameraToObject((int)b);
                sv_PanelFlag = 0;
            }
        }
    }
    snd_AmbientOn = 0;
    ai_GameState = 0;
    snd_StopMarketAmbience();
    d2_CopyCaption(ch_EmptyName);
    sv_SetStatusText(ch_EmptyName);
    sv_ResetSelection();
    d2_PointerLook = 0;
    d2_CutsceneActive = 0;
    di_Absolute = 1;
    di_SetMousePos(d2_MouseX, d2_MouseY);
    di_MaxX = maxX;
    di_MaxY = maxY;
    di_MinX = minX;
    di_MinY = minY;
    sv_RunFrame(0, 0x67fff);
}
/* 0x591ad0, finds the player's building in the jail (278 B): 0x53e400 (1, 0, 0x3d) walks the buildings; the one
   whose +0xac is the player's building word is taken, else the character query 0x501190 (1, 5, 6) / 0x501030
   (the one with +0x65); 0x4ffe90 (the record +0x5d, 1, 0, 0xba / 0x65) gives the jail record and 0x521960
   (the record, +2, the text 0x631090) reports it; the errors 0x62b1d8 / 0x62b198 go to 0x4222b0 */

void lc_PlayerInJail(void)
{
    AIJOB *b;
    AIBLD *it;
    AIOBJ *r;

    b = (AIJOB *)he_FindEvent(1, 0, 0x3d);
    while (b != 0) {
        if (((LCFIREEVENT *)b)->bld == ai_CharInts[ai_PlayerId][0])
            break;
        b = (AIJOB *)he_NextEvent();
    }
    if (b != 0) {
        it = (AIBLD *)gm_BuildQuery(1, 5, 4);
        while (it != 0) {
            if (it->victimId == ai_CharInts[ai_PlayerId][0])
                break;
            it = (AIBLD *)gm_BuildNext();
        }
        if (it == 0) {
            err_Message("lc_PlayerInJail(): Could not find ob_DIEBESGILDE_GEFAENGNIS...");
            return;
        }
        r = (AIOBJ *)gm_Query((void *)it->objList, 1, 0, 0x65);
        if (r == 0 || sv_PanelShown == (unsigned short *)r)
            return;
        sv_SelectPanelObject(it, r->objId, (void *)(unsigned)&ch_EmptyName);
        return;
    }
    it = (AIBLD *)gm_BuildQuery(1, 5, 6);
    if (it == 0) {
        err_Message("lc_PlayerInJail(): Could not find a jail...");
        return;
    }
    r = (AIOBJ *)gm_Query((void *)it->objList, 1, 0, 0xba);
    if (r == 0 || sv_PanelShown == (unsigned short *)r)
        return;
    sv_SelectPanelObject(it, r->objId, (void *)(unsigned)&ch_EmptyName);
}
typedef struct LCTREENODE {        /* a node of f3gm's research tree layout gm_TreeNodes (f3gm.c RTNODE, 0x34 bytes) */
    char pad00[0x24];
    int obj;                       /* 0x24, the node's button object (RTNODE.l[2]), -1 = none */
    char pad28[0xc];
} LCTREENODE;
extern char gm_TreeNodes[3328];    /* 0x76fde0, LCTREENODE[64] (f3gm.c) */
extern int ai_Difficulty;          /* 0x6f23fc, the game difficulty 0 .. 4 (ai.h) */

/* 0x591bf0, the tech tree window of the building bld (1593 B): for the player's building it is run by
   ai_RunAllows (0x200) between the campaign steps 0x11 (bld, 0 / 1, room); the research object of the room
   (gm_Query (objList, 1, 0, room), else (1, 4, 2)) opens "techtree\TechTree" (gm_TechTreeWindow; a failure shows
   the text d2_Strings[0x1e]) with its effects button (text 0x17e2: ai_TechEffectsWindow). A click on a tree node
   (ids 0xdf .. 0x3bb, a node of gm_TreeNodes) researches the goods id - 0xdf when the script query 0x2b allows it:
   not yet held (the room's list, or the building's rooms (2, 6, 0)) it costs either AIGOOD.b40 of the character's
   money (text 0x1b / 0x74, cm_SendMoneyDelta) or the price gm_ItemPrice * (difficulty / 4 + 0.5) from the master's
   goods or the treasury (text 0x1a / 0x1c, cm_SendMoveGoods); the research message (type 0xc for a building
   upgrade, goods kind 6, else 2) goes out in the group "upgrade" and its slot is waited for; an object already
   held is selected on the panel ("ob_<NAME>"); gm_UpdateTechTree redraws the tree every frame */
void ai_TechTreeWindow(int bld, int room)
{
    AIMSG msg;
    char text[0x100];
    char name[0x80];
    char objName[0x80];
    AIBLD *b;
    AIROOM *rec;                    /* the research object */
    AIROOM *treasury;
    AIGOOD *good;
    AICHARREC *me;
    int *prec;
    int win;
    int button;
    int found;
    int cost;
    int slot;
    int ok;
    int id;
    int k;
    int i;
    float factor;

    b = (AIBLD *)bld;
    if (bld == (int)ai_PlayerBuilding) {
        if (ai_RunAllows(0x200) == 0)
            return;
        ai_CampaignStep(0x11, bld, 0, room);
    }
    me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    rec = (AIROOM *)gm_Query((void *)b->objList, 1, 0, room);
    if (rec == 0) {
        rec = (AIROOM *)gm_Query((void *)b->objList, 1, 4, 2);
        if (rec == 0)
            return;
    }
    win = gm_TechTreeWindow("techtree\\TechTree", bld, rec);
    if (win < 0) {
        sv_MessageBox(d2_Strings[0x1e], 0);
        return;
    }
    d2_CentreViewWindows(win);
    d2_SetForm(win, 4);
    button = d2_GetWinObj(win, 4, io_PrintText(0x17e2));
    sv_SetResearchClass((unsigned char *)bld);
    if (sv_RunFrame((int)ai_TechTreeWindow, 0x657c7) != 0) {
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            id = d2_ClickedId;
            if (id != -1 && d2_PointerObj == button) {
                ai_TechEffectsWindow(bld);
                id = d2_ClickedId;
            }
            if (id >= 0xdf && id <= 0x3bb) {
                for (k = 0; k < gm_TreeNodeCount; k++) {
                    if (((LCTREENODE *)gm_TreeNodes)[k].obj != -1 && d2_PointerObj == ((LCTREENODE *)gm_TreeNodes)[k].obj)
                        break;
                }
                if (k < gm_TreeNodeCount) {
                    memset(&msg, 0, sizeof(msg));
                    i = id - 0xdf;
                    if (ai_ScriptQuery(0x2b, bld, 0, i) == 0)
                        continue;
                    good = &((AIGOOD *)ai_Goods)[i];
                    if (rec != 0)
                        found = (int)gm_Query((void *)rec->list, 1, 0, i);
                    else
                        found = (int)gm_Query((void *)b->objList, 2, 6, 0, i);
                    if (found == 0) {
                        /* research it */
                        if (((AIGOOD *)ai_Goods)[i].type == 6) {
                            msg.flags = 1;
                            msg.type = 0xc;
                            msg.fromId = b->id;
                            msg.to = ai_CharInts[b->masterId][0];
                            msg.time = ai_Time;
                            msg.arg[0] = b->id;
                            msg.arg[2] = i;
                            msg.arg[1] = msg.to;
                        } else {
                            msg.flags = 1;
                            msg.type = 2;
                            msg.to = ai_CharInts[b->masterId][0];
                            msg.fromId = b->id;
                            msg.time = ai_Time;
                            msg.arg[1] = rec->objId;
                            msg.itemId = (unsigned short)i;
                        }
                        cost = good->b40;
                        if (cost == 0) {
                            /* paid in goods: the price by the difficulty */
                            ok = 0;
                            factor = (float)(ai_Difficulty * 0.25 + 0.5);
                            cost = (int)((int)gm_ItemPrice(i, 100) * factor);
                            if (b->owner != *ai_PlayerChar) {
                                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], cost, ai_OwnPlayer) != 0)
                                    ok = 1;
                                txt_Format(text, 0x1a, cost, bld, i * 2 + 0x9a8);
                            } else {
                                treasury = (AIROOM *)gm_ClassTreasury(b);
                                if (sv_CheckStockB(treasury, treasury->list, cost, ai_OwnPlayer) != 0)
                                    ok = 1;
                                txt_Format(text, 0x1c, cost, bld, i * 2 + 0x9a8);
                            }
                            if (ok == 0 || sv_MessageBox(text, 1) == 0)
                                goto redraw;
                            ai_CampaignStep(0x2b, bld, 0, i);
                            cm_BeginGroup("upgrade");
                            if (b->owner != *ai_PlayerChar) {
                                cm_SendMoveGoods(-1, ai_CharInts[b->owner][0], ai_OwnPlayer, cost);
                                prec = (int *)gm_PlayerRecord(ai_Chars[ai_PlayerId]);
                                if (prec != 0)
                                    prec[0x10] += cost;
                            } else {
                                treasury = (AIROOM *)gm_ClassTreasury(b);
                                if (treasury != 0)
                                    cm_SendMoveGoodsUnchecked(-1, treasury->objId, ai_OwnPlayer, cost);
                            }
                            slot = cm_SendLongMessage((char *)&msg);
                            cm_EndGroup("upgrade");
                        } else {
                            /* paid in money */
                            txt_Format(text, 0x1b, cost, bld, i * 2 + 0x9a8);
                            if (sv_MessageBox(text, 1) == 0)
                                goto redraw;
                            if (cost > me->money) {
                                txt_Format(text, 0x74, cost);
                                sv_MessageBox(text, 0);
                                goto redraw;
                            }
                            cm_BeginGroup("upgrade");
                            cm_SendMoneyDelta(me->netId, -cost);
                            slot = cm_SendLongMessage((char *)&msg);
                            cm_EndGroup("upgrade");
                        }
                        while (slot != -1 && cm_GetSlotState(slot) == 0)
                            sv_RunFrame(0, 0x657c7);
                    } else if (rec != 0 && (!rec->id == 0xfd || ai_FallbackBuilding != 0)) {
                        /* held already: select it on the panel */
                        strcpy(name, good->name);
                        strupr(name);
                        sprintf(objName, "ob_%s", name);
                        sv_SelectPanelObject((unsigned char *)bld, rec->objId, objName);
                    }
                }
            }
        redraw:
            gm_UpdateTechTree(bld, rec);
        } while (sv_RunFrame((int)ai_TechTreeWindow, 0x657c7) != 0);
    }
    gm_CloseTechTree();
    sv_ResearchClass = 0;
    if (bld == (int)ai_PlayerBuilding)
        ai_CampaignStep(0x11, bld, 1, room);
}
/* 0x592230, the upgrade listener (89 B): the pump 0x5309a0 (this, 0x67fff) until 0; when the pending contact
   0x6f22e4 is "UPGRADE" the tech tree 0x591bf0 (building, 0x2c) opens */

void ai_UpgradeContact(int building)
{
    int r;

    r = sv_RunFrame(ai_UpgradeContact, 0x67fff);
    while (r != 0) {
        if (ai_ClickedObject != 0 && strcmpi("UPGRADE", (char *)ai_ClickedObject) == 0)
            ai_TechTreeWindow(building, 0x2c);
        r = sv_RunFrame(ai_UpgradeContact, 0x67fff);
    }
}
/* 0x592290, the book contact of the building bld (143 B): releases the name plates, then per frame of the pump
   sv_RunFrame registers the two book objects (ob_MEISTERBRIEF with ai_ContactTextMasterLetter, ob_PERSONALBUCH
   with ai_ContactTextPersonnelBook) and opens ai_MasterLetterWindow / ai_PersonnelBook when one is the clicked
   object (ai_ClickedObject) */

void ai_BookContact(char *bld)
{
    int r;
    int a;
    int b;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_BookContact, 0x67fff);
    while (r != 0) {
        a = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0xc);
        b = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == a)
                ai_MasterLetterWindow(bld);
            else if (ai_ClickedObject == b)
                ai_PersonnelBook(bld, "ob_PERSONALBUCH");
        }
        r = sv_RunFrame(ai_BookContact, 0x67fff);
    }
}
/* 0x592320, the mercenary-book contact of the building bld (160 B): releases the name plates, then per frame of
   the pump sv_RunFrame, while ai_BuildingPerms has 0x200, registers the two plates ob_SOELDNERBUCH (the hover
   text ai_ContactTextMercenaryBook) and ub_STAENDER_WAFFEN (ai_ContactTextEquip) and opens ai_MercenaryBook /
   ai_EquipWindow when one is the clicked object (ai_ClickedObject) */

void ai_MercenaryBookContact(char *bld)
{
    int r;
    int a;
    int b;

    a = 0;
    b = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_MercenaryBookContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            a = sv_RegisterNamePlate("ob_SOELDNERBUCH", ai_ContactTextMercenaryBook, 0xc);
            b = sv_RegisterNamePlate("ub_STAENDER_WAFFEN", ai_ContactTextEquip, 0xd);
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == a)
                ai_MercenaryBook((AIBLD *)bld, "ob_SOELDNERBUCH");
            else if (ai_ClickedObject == b)
                ai_EquipWindow(bld);
        }
        r = sv_RunFrame(ai_MercenaryBookContact, 0x67fff);
    }
}


/* 0x5923c0, the resource-search window of the building bld (the SUCHEN_ERZ / SUCHEN_STEIN plate opens it, 1998 B):
   the production screen "Handel\Handel_PRODUKTION" with the title text 0xc7 and the frame 0x70b; column 0 is the
   search itself, the further columns the goods of the room 0xff (gm_Query (1, 4, 0x17), new kinds only, at most
   16). Every frame it finds the rooms 0xff (the goods), 0x2a (the store) and 0xfe (the resource room; it ends
   when one is gone) and handles a click on a column picture: with members of the player's list marked it moves
   them into the building's search job (kind 9, column 0) or the job 8 of the column's goods, sending the job's
   message (type 9 / 8) first when there is none (and waiting for its slot); with none marked it selects the
   workers of that job (column 0 clears them). A right click clears the marks or pops the window. Each column
   gets its inputs, the amount the store's inputs make and its stock; the count text shows the goods columns;
   the frame callback is ai_ProductionWindow's
   @deep size queue s17 (raw 2015 vs 1998, padded 2016 vs 2000; 141 ins): snd_PlayFavourComment stands in
   each arm of if (c == 0) {..} else {..} as in the original (it calls it at 0x592654 and 0x59273e): now
   the column-0 arm's slot wait jumps into the other arm's job = cm_GetSlotValue(slot) tail as the original
   (144 -> 141). Left: bld in ebp where the original has ebx (and ai_ProdColumns index base the other way),
   the frame's equal-size pairs swapped (ncols / c, store / room, win / res, down / up: every declaration
   order of the four pairs = control, so reference counts / the unstable sort decide), and the goods-column
   loop keeps ncols in memory where the original holds a split piece in ebx (mov ebx,[ncols] after
   gm_Query, mov edi,ebx; imul edi,0x58, cmp edx,ebx, inc ebx). climb round 1 (364 mutants): nothing
   closer. prior: Reconstructed in full from the original 0x5923c0..0x592b8d (was a skeleton) on the model
   of ai_ProductionWindow: the three rooms (0xff / 0x2a / 0xfe), the member-row any-loop, the column click
   (no marks: select / clear the workers of job 9 / 8; marks: the job 9 / 8 found or created by the message
   type 9 / 8 with its slot wait, sv_MoveToGroup, the stage), the right click, the goods columns from 1 on
   (gm_Query 1, 4, 0x17, new kinds, < 16), the inputs / amount / stock per column, the count text and the
   three update calls with ai_ProductionWindow as the frame callback. ours=609 original=603. Left:
   allocation only: the original keeps bld in ebx (reloaded from edi at the loop end, 0x592b6c 'mov ebx,
   edi') and ncols in ebx inside the column collection where ours keeps bld in ebp and ncols in [esp+0x10]
   (0x592992: ours 'mov ecx, [esp+0x10]; inc ecx; ... mov [esp+0x10], ecx' vs the original 'inc ... */
int ai_SearchResourceWindow(AIBLD *bld)
{
    AIMSG msg;
    char count[4];
    AIOBJ *room;
    AIOBJ *store;
    AIOBJ *res;
    AIOBJ *item;
    AIJOB *job;
    LCPRODCOL *col;
    int win;
    int ncols;
    int any;
    int fresh;
    int slot;
    int down;
    int up;
    int bar;
    int c;
    int k;
    int i;

    if (sv_CheckCharacterFlag(bld) != 0)
        return 0;
    win = d2_OpenForm(0, 0, "Handel\\Handel_PRODUKTION");
    d2_CentreViewWindows(win);
    sv_OpenPlayerBar((int)bld, 0);
    d2_SetForm(win, 3);
    io_PrintText(0xc7);
    ai_InitProductionTables();
    ai_ProdColumns[0].good = 0;
    ncols = 1;
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0xe;
    d2_Wins[d2_LastWindow].padt = 0xe;
    d2_Wins[d2_LastWindow].padr = 0xe;
    d2_Wins[d2_LastWindow].padb = 0xe;
    sv_OpenWindowFrame(d2_LastWindow, 0x70b, 0x742);
    sv_CreateScrollButtons(0x270, 0x15e, d2_LastWindow, d2_GetViewWin(win, 2), 0x78, &down, &up, &bar);
    d2_Objs[bar].font = 0x54;
    d2_SetPointerSprite(0, 0);
    do {
        d2_SetForm(win, 2);
        room = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0xff);
        if (room == 0)
            return 0;
        store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
        if (store == 0)
            return 0;
        res = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0xfe);
        if (res == 0)
            return 0;
        if (d2_ClickedId != -1 && d2_ClickedForm == win) {
            any = 0;
            for (i = 0; !any && i < 0x20 && sv_MemberRows[i].chr != 0xffff; i++)
                if (sv_MemberRows[i].selected != 0)
                    any = 1;
            for (c = 0; c < ncols; c++) {
                if (d2_PointerObj != ai_ProdColumns[c].pic)
                    continue;
                if (!any) {
                    /* nobody marked: select the workers of the column's job (column 0: clear the searchers) */
                    for (i = 0; i < 0x20; i++) {
                        if (sv_MemberRows[i].chr == 0xffff)
                            break;
                        job = ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].job;
                        if (c == 0) {
                            if (job != 0 && job->kind == 9 && job->from == bld->id)
                                ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                        } else if (job != 0 && job->kind == 8 && job->from == bld->id &&
                                   job->u.w.lo == ai_ProdColumns[c].good) {
                            ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 1;
                        }
                    }
                    continue;
                }
                if (c == 0) {
                    snd_PlayFavourComment();
                    job = (AIJOB *)he_FindEvent(2, 0, 9, 3, bld->id);
                    if (job == 0) {
                        memset(&msg, 0, sizeof(msg));
                        msg.type = 9;
                        msg.to = ai_CharInts[ai_PlayerId][0];
                        msg.fromId = bld->id;
                        msg.nIds = 0;
                        msg.reserved14 = 0;
                        msg.time = ai_Time;
                        msg.flags = 1;
                        msg.arg[0] = bld->id;
                        msg.arg[1] = room->objId;
                        msg.arg[2] = res->objId;
                        slot = cm_SendLongMessage((char *)&msg);
                        while (cm_GetSlotState(slot) == 0)
                            sv_RunFrame(0, 0x20007);
                        job = (AIJOB *)cm_GetSlotValue(slot);
                        if (job == 0)
                            continue;
                    }
                } else {
                    snd_PlayFavourComment();
                    for (job = (AIJOB *)he_FindEvent(2, 0, 8, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                        if (job->u.w.lo == ai_ProdColumns[c].good)
                            break;
                    if (job == 0) {
                        memset(&msg, 0, sizeof(msg));
                        msg.type = 8;
                        msg.to = ai_CharInts[ai_PlayerId][0];
                        msg.fromId = bld->id;
                        msg.nIds = 0;
                        msg.reserved14 = 0;
                        msg.time = ai_Time;
                        msg.flags = 1;
                        msg.itemId = (unsigned short)ai_ProdColumns[c].good;
                        msg.arg[1] = bld->id;
                        msg.arg[2] = res->objId;
                        slot = cm_SendLongMessage((char *)&msg);
                        while (cm_GetSlotState(slot) == 0)
                            sv_RunFrame(0, 0x20007);
                        job = (AIJOB *)cm_GetSlotValue(slot);
                        if (job == 0)
                            continue;
                    }
                }
                for (i = 0; i < 0x20; i++) {
                    if (sv_MemberRows[i].chr == 0xffff)
                        break;
                    if (sv_MemberRows[i].selected != 0) {
                        sv_MoveToGroup((unsigned char *)bld, (short *)sv_PanelShown, sv_MemberRows[i].chr, (char *)job);
                        ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                    }
                }
                if (job->stage == 0)
                    job->stage = 1;
            }
        }
        if (dm_L.rup != 0) {
            any = 0;
            for (i = 0; !any && i < 0x20 && sv_MemberRows[i].chr != 0xffff; i++)
                if (sv_MemberRows[i].selected != 0)
                    any = 1;
            if (any) {
                for (i = 0; i < 0x20; i++) {
                    if (sv_MemberRows[i].chr == 0xffff)
                        break;
                    ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                }
            } else {
                sv_PopRequest = 1;
                sv_CameraFollow();
            }
        }
        /* the goods columns after the search column: each kind of goods in the room once */
        item = (AIOBJ *)gm_Query((void *)room->list, 1, 4, 0x17);
        if (item != 0) {
            do {
                if (item->count > 1) {
                    fresh = 1;
                    for (k = 1; fresh && k < ncols; k++)
                        if (ai_ProdColumns[k].good == item->id)
                            fresh = 0;
                    if (fresh && ncols < 16)
                        ai_ProdColumns[ncols++].good = item->id;
                }
                item = (AIOBJ *)gm_QueryNext();
            } while (item != 0);
        }
        for (c = 1, col = &ai_ProdColumns[1]; c < ncols; c++, col++) {
            col->maxAmount = 0x7fffffff;
            for (k = 0; k < 4; k++) {
                col->in[k] = ((AIGOOD *)ai_Goods)[col->good].in[k];
                col->need[k] = ((AIGOOD *)ai_Goods)[col->good].need[k];
                if (col->in[k] == 0)
                    continue;
                item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, col->in[k]);
                if (item == 0)
                    col->maxAmount = 0;
                else
                    col->maxAmount = min(col->maxAmount, gm_ListAmount(store, item) / ((AIGOOD *)ai_Goods)[col->good].need[k]);
            }
            col->maxAmount = ((AIGOOD *)ai_Goods)[col->good].batch * col->maxAmount;
            item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, col->good);
            if (item != 0)
                col->stock = gm_ListAmount(store, item);
            else
                col->stock = 0;
        }
        itoa(ncols - 1, count, 10);
        d2_SetObjText(bar, count);
        ai_UpdateResourceColumns(win, 2, ncols, (int)res);
        ai_UpdateProdGoodsList(bld, store);
        ai_LayoutProdGoodsList(win, 1, 0, (int)bld);
    } while (sv_RunFrame((int)ai_ProductionWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClosePlayerBar();
    return 0;
}
/* 0x592b90, the resource-extraction window of the building bld (the ABBAUEN / ABBAU plate opens it, 1457 B): the
   production screen "Handel\Handel_PRODUKTION" with the title text 0xca and the frame 0x70b; the columns are the
   goods of the room 0xff (gm_Query (1, 4, 0x17), new kinds only, at most 16). Every frame it finds the rooms 0xff
   (the goods), 0xfe (the resource room) and 0x2a (the store; it ends when one is gone) and handles a click on a
   column picture: with members of the player's list marked it moves them into the building's extraction job 3 of
   the column's goods, sending the job's message (type 3, the room and the store) first when there is none (and
   waiting for its slot); with none marked it selects the workers of that job. A right click clears the marks or
   pops the window. Each column gets the amount the resource room holds and its stock, no inputs */
int ai_ExtractResourceWindow(AIBLD *bld)
{
    AIMSG msg;
    char count[4];
    AIOBJ *room;
    AIOBJ *store;
    AIOBJ *res;
    AIOBJ *item;
    AIJOB *job;
    int win;
    int ncols;
    int any;
    int fresh;
    int slot;
    int down;
    int up;
    int bar;
    int c;
    int k;
    int i;

    if (sv_CheckCharacterFlag(bld) != 0)
        return 0;
    win = d2_OpenForm(0, 0, "Handel\\Handel_PRODUKTION");
    d2_CentreViewWindows(win);
    sv_OpenPlayerBar((int)bld, 0);
    d2_SetForm(win, 3);
    io_PrintText(0xca);
    ai_InitProductionTables();
    ncols = 0;
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0xe;
    d2_Wins[d2_LastWindow].padt = 0xe;
    d2_Wins[d2_LastWindow].padr = 0xe;
    d2_Wins[d2_LastWindow].padb = 0xe;
    sv_OpenWindowFrame(d2_LastWindow, 0x70b, 0x742);
    sv_CreateScrollButtons(0x270, 0x15e, d2_LastWindow, d2_GetViewWin(win, 2), 0x78, &down, &up, &bar);
    d2_Objs[bar].font = 0x54;
    do {
        d2_SetForm(win, 2);
        room = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0xff);
        if (room == 0)
            return 0;
        res = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0xfe);
        if (res == 0)
            return 0;
        store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
        if (store == 0)
            return 0;
        if (d2_ClickedId != -1 && d2_ClickedForm == win) {
            any = 0;
            for (i = 0; !any && i < 0x20 && sv_MemberRows[i].chr != 0xffff; i++)
                if (sv_MemberRows[i].selected != 0)
                    any = 1;
            for (c = 0; c < ncols; c++) {
                if (d2_PointerObj != ai_ProdColumns[c].pic)
                    continue;
                if (!any) {
                    /* nobody marked: select the workers of the column's job */
                    for (i = 0; i < 0x20; i++) {
                        if (sv_MemberRows[i].chr == 0xffff)
                            break;
                        job = ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].job;
                        if (job != 0 && job->kind == 3 && job->from == bld->id && job->u.w.lo == ai_ProdColumns[c].good)
                            ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 1;
                    }
                    continue;
                }
                snd_PlayFavourComment();
                for (job = (AIJOB *)he_FindEvent(2, 0, 3, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                    if (job->u.w.lo == ai_ProdColumns[c].good)
                        break;
                if (job == 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 3;
                    msg.nIds = 0;
                    msg.reserved14 = 0;
                    msg.flags = 1;
                    msg.to = ai_CharInts[bld->owner][0];
                    msg.fromId = bld->id;
                    msg.time = ai_Time;
                    msg.itemId = (unsigned short)ai_ProdColumns[c].good;
                    msg.arg[3] = bld->id;
                    msg.arg[5] = store->objId;
                    msg.arg[4] = res->objId;
                    slot = cm_SendLongMessage((char *)&msg);
                    while (cm_GetSlotState(slot) == 0)
                        sv_RunFrame(0, 0x20007);
                    job = (AIJOB *)cm_GetSlotValue(slot);
                }
                for (i = 0; i < 0x20; i++) {
                    if (sv_MemberRows[i].chr == 0xffff)
                        break;
                    if (sv_MemberRows[i].selected != 0) {
                        sv_MoveToGroup((unsigned char *)bld, (short *)sv_PanelShown, sv_MemberRows[i].chr, (char *)job);
                        ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                    }
                }
                if (job->stage == 0)
                    job->stage = 1;
            }
        }
        if (dm_L.rup != 0) {
            any = 0;
            for (i = 0; !any && i < 0x20 && sv_MemberRows[i].chr != 0xffff; i++)
                if (sv_MemberRows[i].selected != 0)
                    any = 1;
            if (any) {
                for (i = 0; i < 0x20; i++) {
                    if (sv_MemberRows[i].chr == 0xffff)
                        break;
                    ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                }
            } else {
                sv_PopRequest = 1;
                sv_CameraFollow();
            }
        }
        /* the columns: each kind of goods in the room once */
        item = (AIOBJ *)gm_Query((void *)room->list, 1, 4, 0x17);
        if (item != 0) {
            do {
                if (item->count > 1) {
                    fresh = 1;
                    for (k = 0; fresh && k < ncols; k++)
                        if (ai_ProdColumns[k].good == item->id)
                            fresh = 0;
                    if (fresh && ncols < 16)
                        ai_ProdColumns[ncols++].good = item->id;
                }
                item = (AIOBJ *)gm_QueryNext();
            } while (item != 0);
        }
        for (c = 0; c < ncols; c++) {
            item = (AIOBJ *)gm_Query((void *)res->list, 1, 0, ai_ProdColumns[c].good);
            if (item != 0)
                ai_ProdColumns[c].maxAmount = item->count;
            else
                ai_ProdColumns[c].maxAmount = 0;
            item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, ai_ProdColumns[c].good);
            if (item != 0)
                ai_ProdColumns[c].stock = gm_ListAmount(store, item);
            else
                ai_ProdColumns[c].stock = 0;
            memset(ai_ProdColumns[c].in, 0, sizeof(ai_ProdColumns[c].in));
        }
        itoa(ncols, count, 10);
        d2_SetObjText(bar, count);
        ai_UpdateProdColumns(win, 2, ncols, store, 1);
        ai_UpdateProdGoodsList(bld, store);
        ai_LayoutProdGoodsList(win, 1, 0, (int)bld);
    } while (sv_RunFrame((int)ai_ExtractResourceWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClosePlayerBar();
    return 0;
}

int gm_BuildKindObjects(char *b);
char *itoa(int, char *, int);

/* 0x593150, the production window of a workshop bld (1894 B): the production screen "Handel\Handel_PRODUKTION" with
   the title text 0xcb and the frame 0x70b; the columns are the kinds of objects the building can make
   (gm_BuildKindObjects: gm_KindObjs, not of the type 0x17) whose four inputs the room 0xff holds (a count of 2, or
   any count in a building of the kind 0x1e), new ones only, at most 16. Every frame it finds the rooms 0xff and
   0x2a (the store; it ends when one is gone) and handles a click: on a column picture with members of the player's
   list marked it moves them (into the shown room, else the room 0xfd) into the building's production job 4 of the
   column (ai_ProdColumnSlots' goods), sending the order message type 4 first when there is none (and waiting for
   its slot); with none marked it selects the workers of the job 4 of the column's goods; on an input picture it
   picks that input (ai_ProdSelectedInput, again: none). A right click clears the marks or pops the window. Each
   column gets its inputs, the amount the store's inputs make (by the column's need) and its stock
   @deep deep s35: the column loop has two strength-reduced pointers: &ai_ProdColumns[ncols] (the
   original's ebp, its end compare cmp ebp,X for ncols < 16, the store add ebp,0x58; mov [ebp-0x58],ecx
   after inc esi = ncols) and &gm_KindObjs[i] (the original's [esp+0x10], reloaded into ebx in the fresh
   loop); ours gives ebp to the gm_KindObjs pointer. ai_ProdColumns[ncols].good = ..; ncols++ = control;
   ncols++ then [ncols - 1] (or [++ncols - 1]) builds the column pointer as imul ebp,0x58 / add ebp like
   the original but keeps the gm_KindObjs pointer in ebp and the column pointer in [esp+0x14] (77 ins); the
   fresh loop without the fresh && test: 113. climb round 1: only msg.nIds = 0 before msg.time (21 ins /
   429 bytes, not kept). Left as the prior note says plus this pointer priority. prior: Reconstructed in
   full from the original 0x593150..0x5938b5 (was a skeleton) on the model of ai_ProductionWindow: the two
   rooms, the member any-loop, the column click (no marks: select the workers of job 4; marks: the job 4 by
   ai_ProdColumnSlots' goods or the order message type 4 with its slot wait, the move into the shown room
   or the room 0xfd, the stage), the input-picture pick (ai_ProdSelectedInput), the right click, the
   columns from gm_BuildKindObjects (type 0x17 skipped, the four inputs in the room: count 2 or a kind 0x1e
   building; new ones, < 16), the inputs / amount (by col->need) / stock per column and the update calls.
   ours=575 original=574, no ref difference. Left: allocation and scheduling: in the fresh-goods check the
   original keeps ncols in esi and reloads the gm_KindObjs pointer from [esp+0x10] into ebx each pass
   (ours: the pointer in esi, ncols in edi, 0x5936a6), and it increments ncols before storing the new
   column ('inc esi; mov [esp+0x18], esi; add ebp, 0x58; mov ecx, [eax]; mov [ebp-0x58], ecx' vs ours 'mov
   ecx, [eax]; mov [ebp], ecx; mov ecx, [esp+0x18]; inc ecx'); one store of the message (msg.time's word)
   is scheduled later. A variant round. */
int ai_ProductionGoodsWindow(AIBLD *bld)
{
    AIMSG msg;
    char count[4];
    AIOBJ *room;
    AIOBJ *store;
    AIOBJ *item;
    AIJOB *job;
    AIGOOD *g;
    LCPRODCOL *col;
    short *target;
    int win;
    int ncols;
    int any;
    int ok;
    int fresh;
    int slot;
    int down;
    int up;
    int bar;
    int c;
    int k;
    int i;

    if (sv_CheckCharacterFlag(bld) != 0)
        return 0;
    win = d2_OpenForm(0, 0, "Handel\\Handel_PRODUKTION");
    d2_CentreViewWindows(win);
    sv_OpenPlayerBar((int)bld, 0);
    d2_SetForm(win, 3);
    io_PrintText(0xcb);
    ai_InitProductionTables();
    ncols = 0;
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0xe;
    d2_Wins[d2_LastWindow].padt = 0xe;
    d2_Wins[d2_LastWindow].padr = 0xe;
    d2_Wins[d2_LastWindow].padb = 0xe;
    sv_OpenWindowFrame(d2_LastWindow, 0x70b, 0x742);
    sv_CreateScrollButtons(0x270, 0x15e, d2_LastWindow, d2_GetViewWin(win, 2), 0x78, &down, &up, &bar);
    d2_Objs[bar].font = 0x54;
    do {
        d2_SetForm(win, 2);
        room = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0xff);
        if (room == 0)
            return 0;
        store = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
        if (store == 0)
            return 0;
        if (d2_ClickedId != -1 && d2_ClickedForm == win) {
            any = 0;
            for (i = 0; !any && i < 0x20 && sv_MemberRows[i].chr != 0xffff; i++)
                if (sv_MemberRows[i].selected != 0)
                    any = 1;
            for (c = 0; c < ncols; c++) {
                if (d2_PointerObj == ai_ProdColumns[c].pic) {
                    if (!any) {
                        /* nobody marked: select the workers of the column's job */
                        for (i = 0; i < 0x20; i++) {
                            if (sv_MemberRows[i].chr == 0xffff)
                                break;
                            job = ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].job;
                            if (job != 0 && job->kind == 4 && job->from == bld->id &&
                                job->u.w.lo == ai_ProdColumns[c].good)
                                ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 1;
                        }
                    } else {
                        for (job = (AIJOB *)he_FindEvent(2, 0, 4, 3, bld->id); job != 0; job = (AIJOB *)he_NextEvent())
                            if (job->u.w.lo == ai_ProdColumnSlots[c].good)
                                break;
                        if (job == 0) {
                            snd_PlayFavourComment();
                            memset(&msg, 0, sizeof(msg));
                            msg.type = 4;
                            if (bld->owner != 0xffff)
                                msg.to = ai_CharInts[bld->owner][0];
                            else
                                msg.to = ai_CharInts[ai_PlayerId][0];
                            msg.fromId = bld->id;
                            msg.time = ai_Time;
                            msg.itemId = (unsigned short)ai_ProdColumns[c].good;
                            msg.nIds = 0;
                            msg.reserved14 = 0;
                            msg.flags = 1;
                            msg.arg[1] = -1;
                            msg.arg[4] = bld->id;
                            msg.arg[5] = store->objId;
                            slot = cm_SendLongMessage((char *)&msg);
                            while (cm_GetSlotState(slot) == 0)
                                sv_RunFrame(0, 0x20007);
                            job = (AIJOB *)cm_GetSlotValue(slot);
                        }
                        for (i = 0; i < 0x20; i++) {
                            if (sv_MemberRows[i].chr == 0xffff)
                                break;
                            if (sv_MemberRows[i].selected != 0) {
                                target = (short *)sv_PanelShown;
                                if (target == 0)
                                    target = (short *)gm_Query((void *)bld->objList, 1, 0, 0xfd);
                                sv_MoveToGroup((unsigned char *)bld, target, sv_MemberRows[i].chr, (char *)job);
                                ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                            }
                        }
                        if (job->stage == 0)
                            job->stage = 1;
                    }
                }
                /* an input picture of the column picks that input */
                for (k = 0; k < 4; k++) {
                    if (d2_PointerObj == ai_ProdColumns[c].inPics[k]) {
                        if (ai_ProdColumns[c].selected == k)
                            ai_ProdSelectedInput = 0;
                        else
                            ai_ProdSelectedInput = ai_ProdColumns[c].in[k];
                    }
                }
            }
        }
        if (dm_L.rup != 0) {
            any = 0;
            for (i = 0; !any && i < 0x20 && sv_MemberRows[i].chr != 0xffff; i++)
                if (sv_MemberRows[i].selected != 0)
                    any = 1;
            if (any) {
                for (i = 0; i < 0x20; i++) {
                    if (sv_MemberRows[i].chr == 0xffff)
                        break;
                    ((AICHARREC *)ai_Chars)[sv_MemberRows[i].chr].selected = 0;
                }
            } else {
                sv_PopRequest = 1;
                sv_CameraFollow();
            }
        }
        /* the columns: the objects the building can make whose inputs the room holds */
        gm_BuildKindObjects((char *)bld);
        for (i = 0; ncols < 16 && i < gm_KindObjCount; i++) {
            g = &((AIGOOD *)ai_Goods)[gm_KindObjs[i]];
            if (g->type == 0x17)
                continue;
            ok = 1;
            for (k = 0; ok && k < 4; k++) {
                if (g->in[k] != 0) {
                    item = (AIOBJ *)gm_Query((void *)room->list, 1, 0, g->in[k]);
                    if (item == 0 || (item->count != 2 && bld->type != 0x1e))
                        ok = 0;
                }
            }
            if (ok) {
                fresh = 1;
                for (k = 0; fresh && k < ncols; k++)
                    if (ai_ProdColumns[k].good == gm_KindObjs[i])
                        fresh = 0;
                if (fresh && ncols < 16)
                    ai_ProdColumns[ncols++].good = gm_KindObjs[i];
            }
        }
        for (c = 0, col = ai_ProdColumns; c < ncols; c++, col++) {
            col->maxAmount = 0x7fffffff;
            for (k = 0; k < 4; k++) {
                col->in[k] = ((AIGOOD *)ai_Goods)[col->good].in[k];
                col->need[k] = ((AIGOOD *)ai_Goods)[col->good].need[k];
                if (col->in[k] != 0) {
                    item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, col->in[k]);
                    if (item == 0)
                        col->maxAmount = 0;
                    else
                        col->maxAmount = min(col->maxAmount, gm_ListAmount(store, item) / col->need[k]);
                }
            }
            col->maxAmount = ((AIGOOD *)ai_Goods)[col->good].batch * col->maxAmount;
            item = (AIOBJ *)gm_Query((void *)store->list, 1, 0, col->good);
            if (item != 0)
                col->stock = gm_ListAmount(store, item);
            else
                col->stock = 0;
        }
        itoa(ncols, count, 10);
        d2_SetObjText(bar, count);
        ai_UpdateProdColumns(win, 2, ncols, store, 2);
        ai_UpdateProdGoodsList(bld, store);
        ai_LayoutProdGoodsList(win, 1, 0, (int)bld);
    } while (sv_RunFrame((int)ai_ProductionGoodsWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClosePlayerBar();
    return 0;
}

int ai_SearchResourceWindow(AIBLD *bld);
int ai_ExtractResourceWindow(AIBLD *bld);

/* 0x5938c0, the mining contact of the building bld (152 B): releases the name plates, then per frame of the
   pump sv_RunFrame, while ai_BuildingPerms has 0x200, registers the two plates contact_SUCHEN_ERZ (the hover
   text ai_ContactTextSearchOre) and contact_ABBAUEN (ai_ContactTextExtract) and opens ai_SearchResourceWindow /
   ai_ExtractResourceWindow when one is the clicked object (ai_ClickedObject) */
void ai_MiningContact(char *bld)
{
    int r;
    int a;
    int b;

    a = 0;
    b = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_MiningContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            a = sv_RegisterNamePlate("contact_SUCHEN_ERZ", ai_ContactTextSearchOre, 0x16);
            b = sv_RegisterNamePlate("contact_ABBAUEN", ai_ContactTextExtract, 10);
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == a)
                ai_SearchResourceWindow(bld);
            else if (ai_ClickedObject == b)
                ai_ExtractResourceWindow(bld);
        }
        r = sv_RunFrame(ai_MiningContact, 0x67fff);
    }
}
/* 0x593960, the kontor contact listener (407 B): the campaign step 0x1b in and 0x1c out (0x576040); the plates
   released (0x52e190) and, while the building permission 0x200 (0x6f231c) is set, registered again through
   0x52e1c0: PRODUKTION_METALL (0x1441f98, 0x13) -> the goods window 0x593150, TRANSPORT (0x1441fa0, 0x15) ->
   0x5af1a0, LAGER (0x1441f90, 0xe) -> 0x5b26b0 (bld, 0), GELAGE (0x1441f88, 0x16) -> 0x5b9f30 and, with the
   records 0x4ffe90 (0x35 / 0x34) of the building's objList, STROHPUPPE (0x1440ad0) / ZIELSCHEIBE (0x1440acc) ->
   0x5bba00; 0x5912c0 runs every frame; a clicked plate (0x6f22e4) opens its window; the frame pump 0x5309a0
   (this, 0x67fff) */

void ai_KontorContact(AIBLD *bld)
{
    int r;
    int metall;
    int transport;
    int lager;
    int gelage;
    int stroh;
    int ziel;

    metall = 0;
    lager = 0;
    transport = 0;
    gelage = 0;
    stroh = 0;
    ziel = 0;
    ai_CampaignStep(0x1b, (int)bld, 0, 0);
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_KontorContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            metall = sv_RegisterNamePlate("contact_PRODUKTION_METALL", ai_ContactTextProduction, 0x13);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            gelage = sv_RegisterNamePlate("contact_GELAGE", ai_ContactTextGelage, 0x16);
            if (gm_Query(bld->objList, 2, 6, 0, 0x35) != 0)
                stroh = sv_RegisterNamePlate("ob_STROHPUPPE", ai_ContactTextTraining, 0x17);
            if (gm_Query(bld->objList, 2, 6, 0, 0x34) != 0)
                ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", ai_ContactTextTarget, 0x17);
        }
        ai_TradeContactChecks();
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
            else if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, 0);
            else if (ai_ClickedObject == metall)
                ai_ProductionGoodsWindow((int)bld);
            else if (ai_ClickedObject == gelage)
                ai_GelageWindow(bld);
            else if (ai_ClickedObject == ziel || ai_ClickedObject == stroh)
                ai_TrainingWindow(bld);
        }
        r = sv_RunFrame(ai_KontorContact, 0x67fff);
    }
    ai_CampaignStep(0x1c, (int)bld, 0, 0);
}
void ob_HandleBepflanzung(AIBLD *bld);
void ai_HandlePickedPlant(AIBLD *bld, int plants);
/* 0x593b00, the wood-kontor contact listener (416 B): 0x52e190; the context *(param_1 + 0x71); the pump
   0x5309a0 (this, 0x67fff); while the flag 0x6f231c has 0x200: LAGER (0x1441f90, 0xe), PRODUKTION_HOLZ
   (0x1442128, 0x13), GELAGE (0x1441f88, 0x16), TRANSPORT (0x1441fa0, 0x15), ZIELSCHEIBE (0x1440acc, 0xc),
   STROHPUPPE (0x1440ad0, 0xc); 0x5912c0 and 0x5b9db0 (param_1, the context) every frame; the dispatcher:
   TRANSPORT -> 0x5af1a0, LAGER -> 0x5b26b0 (param_1, param_1 + 0x5d), HOLZ -> 0x593150, GELAGE -> 0x5b9f30,
   ZIEL / STROH -> 0x5bba00; the window 0x623898 -> 0x5b9730 */


void ai_WoodKontorContact(AIBLD *bld)
{
    int r;
    int lager;
    int holz;
    int gelage;
    int transport;
    int ziel;
    int stroh;
    int plants;

    transport = 0;
    lager = 0;
    holz = 0;
    gelage = 0;
    stroh = 0;
    ziel = 0;
    sv_ReleaseNamePlates();
    plants = bld->plants;
    r = sv_RunFrame(ai_WoodKontorContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            holz = sv_RegisterNamePlate("contact_PRODUKTION_HOLZ", ai_ContactTextProductionWood, 0x13);
            gelage = sv_RegisterNamePlate("contact_GELAGE", ai_ContactTextGelage, 0x16);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", ai_ContactTextTarget, 0xc);
            stroh = sv_RegisterNamePlate("ob_STROHPUPPE", ai_ContactTextTraining, 0xc);
        }
        ai_TradeContactChecks();
        ai_HandlePickedPlant(bld, plants);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == transport)
                ai_OpenTransport(bld);
            else if (ai_ClickedObject == lager)
                ai_StorageWindow(bld, bld->objList);
            else if (ai_ClickedObject == holz)
                ai_ProductionGoodsWindow(bld);
            else if (ai_ClickedObject == gelage)
                ai_GelageWindow(bld);
            else if (ai_ClickedObject == ziel)
                ai_TrainingWindow(bld);
            else if (ai_ClickedObject == stroh)
                ai_TrainingWindow(bld);
        }
        if (d2_ClickedId != -1 && d2_PointerObj == sv_PanelPlantBtn)
            ob_HandleBepflanzung(bld);
        r = sv_RunFrame(ai_WoodKontorContact, 0x67fff);
    }
}
/* 0x593ca0, the stone-kontor contact listener (468 B): the plates released (0x52e190) and, while the building
   permission 0x200 (0x6f231c) is set, registered again (0x52e1c0): LAGER (0x1441f90, 0xe), ABBAU (0x14420a8, 10),
   SUCHEN_STEIN (0x14420d8, 0x16), PRODUKTION (0x1441f98, 0x13), TRANSPORT (0x1441fa0, 0x15), GELAGE (0x1441f88,
   0x16), and with the records 0x4ffe90 (0x35 / 0x34) of the building's objList STROHPUPPE / ZIELSCHEIBE (0x1440ad0
   / 0x1440acc, 0x17); 0x5912c0 runs every frame; a clicked plate (0x6f22e4) opens its window: 0x5b26b0 (bld,
   bld->objList), 0x5b9f30, 0x592b90, 0x5923c0, 0x5af1a0, 0x593150, 0x5bba00; the frame pump 0x5309a0
   (this, 0x67fff) */

void ai_StoneKontorContact(AIBLD *bld)
{
    int r;
    int lager;
    int abbau;
    int stein;
    int produktion;
    int transport;
    int gelage;
    int stroh;
    int ziel;

    lager = 0;
    produktion = 0;
    stein = 0;
    transport = 0;
    abbau = 0;
    gelage = 0;
    stroh = 0;
    ziel = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_StoneKontorContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            abbau = sv_RegisterNamePlate("contact_ABBAU", ai_ContactTextExtract, 10);
            stein = sv_RegisterNamePlate("contact_SUCHEN_STEIN", ai_ContactTextSearchStone, 0x16);
            produktion = sv_RegisterNamePlate("contact_PRODUKTION", ai_ContactTextProduction, 0x13);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            gelage = sv_RegisterNamePlate("contact_GELAGE", ai_ContactTextGelage, 0x16);
            if (gm_Query(bld->objList, 2, 6, 0, 0x35) != 0)
                stroh = sv_RegisterNamePlate("ob_STROHPUPPE", ai_ContactTextTraining, 0x17);
            if (gm_Query(bld->objList, 2, 6, 0, 0x34) != 0)
                ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", ai_ContactTextTarget, 0x17);
        }
        ai_TradeContactChecks();
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, bld->objList);
            else if (ai_ClickedObject == gelage)
                ai_GelageWindow(bld);
            else if (ai_ClickedObject == abbau)
                ai_ExtractResourceWindow(bld);
            else if (ai_ClickedObject == stein)
                ai_SearchResourceWindow(bld);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
            else if (ai_ClickedObject == produktion)
                ai_ProductionGoodsWindow((int)bld);
            else if (ai_ClickedObject == ziel || ai_ClickedObject == stroh)
                ai_TrainingWindow(bld);
        }
        r = sv_RunFrame(ai_StoneKontorContact, 0x67fff);
    }
}



/* 0x593e80, the information listener (186 B): the plates released (0x52e190) and registered again every frame
   (0x52e1c0): INFORMATION (0x143cb38, 0xc) -> the spy reports window 0x5a45c0, MEISTERBRIEF (0x143cbc8, 0xc) ->
   0x5c3200, PERSONALBUCH (0x143c9f0, 0xc) -> 0x5aab80 (the building, the name); the clicked plate (0x6f22e4)
   opens its window; the frame pump 0x5309a0 (this, 0x67fff) */
void ai_InformationContact(int building)
{
    int r;
    int inf;
    int meister;
    int personal;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_InformationContact, 0x67fff);
    while (r != 0) {
        inf = sv_RegisterNamePlate("contact_INFORMATION", ai_ContactTextInformation, 0xc);
        meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0xc);
        personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)building);
            else if (ai_ClickedObject == personal)
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == inf)
                ai_SpyReportsWindow((AIBLD *)building);
        }
        r = sv_RunFrame(ai_InformationContact, 0x67fff);
    }
}
/* 0x593f40, the thieves' guild ambush window ("locations\diebesgilde\diebesgilde_einbruch"; the callback 0x594180
   hands to 0x5b2180 with the picked scene object's name, 0: returns 0). Its first button (text 0x1a95) lays the
   selected figures in wait at that place: unless the guild building's owner already runs an ambush event 0x48
   there, it posts the ambush message 0x48 (the place, the selected scene characters as ids, flags 2) and clears
   the selection; the second button (0x1a96) and a right click close it. Returns 1 once the ambush was ordered,
   else 0 */

int ai_BreakInCallback(char *place)
{
    AIMSG msg;
    int win;
    int order;
    int cancel;
    int *ch;
    int *dst;
    int base;
    LCAMBUSHEVENT *ev;
    AIBLD *bld;

    ev = 0;
    if (place == 0)
        return 0;
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_einbruch");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    base = io_PrintText(0x1a95);
    order = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == order) {
            bld = (AIBLD *)ai_FallbackBuilding;
            for (ev = (LCAMBUSHEVENT *)he_FindEvent(3, 0, 0x48, 2, bld->owner, 3, bld->id); ev != 0;
                 ev = (LCAMBUSHEVENT *)he_NextEvent())
                if (strcmp(ev->place, place) == 0)
                    break;
            if (ev == 0) {
                bld = (AIBLD *)ai_FallbackBuilding;
                msg.type = 0x48;
                msg.to = ai_CharInts[bld->owner][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                msg.flags = 2;
                strcpy((char *)msg.arg, place);  /* 0x58: the place's name (the event's place) */
                memset(msg.ids, 0xff, sizeof(msg.ids));
                snd_PlayFavourComment();
                dst = msg.ids;
                for (ch = ai_SceneChars; (int)ch < (int)&ai_SceneChars[32]; ch++)  /* a signed compare (jl) */
                    if (*ch != 0 && ((AICHARREC *)*ch)->selected != 0)
                        *dst++ = ((AICHARREC *)*ch)->netId;
                cm_SendLongMessage((char *)&msg);
            }
            sv_ClearSelection();
            ev = (LCAMBUSHEVENT *)1;  /* the result: the ambush was ordered */
            sv_PopRequest = 1;
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BreakInCallback, 0x677c7) != 0);
    d2_CloseView(win);
    return (int)ev;
}
/* 0x594180, opens the burglar-guild break-in window (63 B): when the character of the building is free
   (sv_CheckCharacterFlag 0x520fe0 is 0) the player bar opens (0x525220), the map pick runs (0x5b2180: 4, 0, the
   prompt 0x1440c10, the break-in callback 0x593f40, 0), then the player bar closes (0x525440) and the selection is
   cleared (0x52ae40) */

void ai_BreakInWindow(int building)
{
    if (sv_CheckCharacterFlag(building) == 0) {
        sv_OpenPlayerBar(building, 0);
        ai_MapPick(4, 0, ai_BreakInPickText, (int)ai_BreakInCallback, 0);
        sv_ClosePlayerBar();
        sv_ClearSelection();
    }
}
/* 0x5941c0, the thieves' guild pickpocket window ("locations\diebesgilde\taschendiebstahl"; the map pick callback
   of ai_OpenPickpocket, with the picked building): the text 0x19ee names the building and its owner; the first
   button sends the selected figures there: when the guild building's owner already runs a pickpocket event 0x49
   on that building, they join its group (sv_MoveToGroup), else the pickpocket message 0x49 (the target building,
   the guild building, up to six selected scene characters as ids, flags 1) is posted, with the text 0x1a49 when
   nobody is selected and 0x19ef when more than six are; the second button and a right click close it. Returns 1
   once the figures were sent, else 0 */

int ai_PickpocketWindow(char *place, AIBLD *target)
{
    AIMSG msg;
    char text[0x400];
    int result;
    int win;
    int send;
    int cancel;
    int base;
    int n;
    int *ch;
    AIJOB *ev;
    AIBLD *bld;

    result = 0;
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\taschendiebstahl");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    base = io_PrintText(0x19ee, target->owner, target);
    send = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == send) {
            bld = (AIBLD *)ai_FallbackBuilding;
            for (ev = (AIJOB *)he_FindEvent(3, 0, 0x49, 2, bld->owner, 3, bld->id); ev != 0;
                 ev = (AIJOB *)he_NextEvent())
                if (ev->bldId == target->id)
                    break;
            if (ev == 0) {
                bld = (AIBLD *)ai_FallbackBuilding;
                msg.type = 0x49;
                msg.to = ai_CharInts[bld->owner][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.arg[0] = target->id;
                msg.arg[1] = bld->id;
                memset(msg.ids, 0xff, sizeof(msg.ids));
                n = 0;
                for (ch = ai_SceneChars; (int)ch < (int)&ai_SceneChars[32] && n < 6; ch++)  /* signed compares (jl) */
                    if (*ch != 0 && ((AICHARREC *)*ch)->selected != 0)
                        msg.ids[n++] = ((AICHARREC *)*ch)->netId;
                if (n == 0) {
                    sv_MessageBox(txt_Ptrs[0x1a49], 0);
                    continue;
                }
                result = 1;
                cm_SendLongMessage((char *)&msg);
                snd_PlayFavourComment();
                n = 0;
                for (ch = ai_SceneChars; (int)ch < (int)&ai_SceneChars[32]; ch++)
                    if (*ch != 0 && ((AICHARREC *)*ch)->selected != 0)
                        n++;
                if (n > 6) {
                    txt_Format(text, 0x19ef, 6);
                    sv_MessageBox(text, 0);
                }
                sv_PopRequest = 2;
            } else {
                result = 1;
                sv_PopRequest = 2;
                for (ch = ai_SceneChars; (int)ch < (int)&ai_SceneChars[32]; ch++)
                    if (*ch != 0 && ((AICHARREC *)*ch)->selected != 0)
                        sv_MoveToGroup(0, 0, ((AICHARREC *)*ch)->id, (char *)ev);
            }
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_PickpocketWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return result;
}

int he_NoProtectionEvent();

/* 0x594470, opens the pickpocket map pick (99 B): the 0x50-byte config local zeroed, its flags +4 = 0x400, +8 =
   0x93810 and +0x10 = 7; the player bar opens (0x525220), the map pick runs (0x5b2180: 1, the config, the
   prompt 0x1440974, the pickpocket window 0x5941c0 as callback, 0), then the player bar closes (0x525440) and the
   selection is cleared (0x52ae40) */
void ai_OpenPickpocket(int building)
{
    int cfg[0x14];

    memset(cfg, 0, 0x50);
    cfg[3] = 0;
    cfg[1] = 0x400;
    cfg[4] = 7;
    cfg[2] = 0x93810;
    sv_OpenPlayerBar(building, 0);
    ai_MapPick(1, cfg, ai_MapPickPrompt, (int)ai_PickpocketWindow, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
/* 0x5944e0, the bribe window of the robbers' camp ("LOCATIONS\RAEUBERLAGER\STANDARD"; ai_OpenBribe's map pick callback, with the picked
   building, 0: returns 0). A building already under a bribe event 0x62 gives the text 0x1a91, no selected character the
   text 0x1a79, a building without a protection event (he_NoProtectionEvent 0) the text 0x1a89; else the window
   (text 0x1a8b) asks: its first button posts the bribe message 0x62 from the player (the building, the player's
   building, the shown room object or -1, the first selected scene character, flags 2) and clears the selection,
   the second button and a right click close it. Returns 0 */

int ai_BribeCallback(char *place, AIBLD *target)
{
    AIMSG msg;
    char text[0x200];
    int win;
    int ok;
    int cancel;
    int base;
    int i;
    AIJOB *ev;

    if (target == 0)
        return 0;
    for (ev = (AIJOB *)he_FindEvent(1, 0, 0x62); ev != 0; ev = (AIJOB *)he_NextEvent())
        if (ev->id2 == target->id)
            break;
    if (ev != 0) {
        txt_Format(text, 0x1a91, target->owner, target);
        sv_MessageBox(text, 0);
        return 0;
    }
    for (i = 0; i < 0x300; i++)
        if (((AICHARREC *)ai_Chars)[i].selected != 0)
            break;
    if (i >= 0x300) {
        sv_MessageBox(txt_Ptrs[0x1a79], 0);
        return 0;
    }
    if (he_NoProtectionEvent(target) == 0) {
        txt_Format(text, 0x1a89, target->owner, target);
        sv_MessageBox(text, 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\\RAEUBERLAGER\\STANDARD");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    base = io_PrintText(0x1a8b, target);
    ok = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            snd_PlayFavourComment();
            msg.type = 0x62;
            msg.to = ai_CharInts[ai_PlayerId][0];
            msg.fromId = ai_PlayerBuilding != 0 ? ((AIBLD *)ai_PlayerBuilding)->id : ((AIBLD *)ai_FallbackBuilding)->id;
            msg.time = ai_Time;
            msg.flags = 2;
            msg.arg[1] = target->id;
            msg.arg[4] = ai_PlayerBuilding != 0 ? ((AIBLD *)ai_PlayerBuilding)->id : ((AIBLD *)ai_FallbackBuilding)->id;
            msg.arg[5] = sv_PanelShown != 0 ? ((AIROOM *)sv_PanelShown)->objId : -1;
            for (i = 0; i < 0x20; i++)
                if (ai_SceneChars[i] != 0 && ((AICHARREC *)ai_SceneChars[i])->selected != 0) {
                    msg.arg[0] = ((AICHARREC *)ai_SceneChars[i])->netId;
                    break;
                }
            cm_SendLongMessage((char *)&msg);
            sv_PopRequest = 1;
            sv_ClearSelection();
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BribeCallback, 0x677c7) != 0);
    d2_CloseView(win);
    return 0;
}
/* 0x594790, opens the bribe map pick (128 B): when the character of the building is free (sv_CheckCharacterFlag
   0x520fe0 is 0) and the building is not 0, the 0x50-byte config is zeroed (flags +0 = 1, +4 = -1, +0x10 = 6,
   weight +8 = 0xa07c43a0, +0xc = 1); the player bar opens (0x525220), the map pick runs (0x5b2180: 1, the config,
   the prompt 0x1440be8, the bribe callback 0x5944e0, 0), then the bar closes (0x525440) and the selection is
   cleared (0x52ae40) */

void ai_OpenBribe(int building)
{
    int cfg[0x14];

    if (sv_CheckCharacterFlag(building) == 0 && building != 0) {
        memset(cfg, 0, 0x50);
        cfg[0] = 1;
        cfg[1] = -1;
        cfg[4] = 6;
        cfg[2] = 0xa07c43a0;
        cfg[3] = 1;
        sv_OpenPlayerBar(building, 0);
        ai_MapPick(1, cfg, ai_BribePickText, (int)ai_BribeCallback, 0);
        sv_ClosePlayerBar();
        sv_ClearSelection();
    }
}

unsigned short * gm_QueryNext(void); void txt_Format(char *out, int id0, ...);

/* 0x594810, the bribery window of the spying order ("misc\Bestechung"; ai_OpenSpying's map pick callback, with the
   picked building; nothing for none or an ownerless one). A building without a protection event
   (he_NoProtectionEvent 0) gives the text 0x1a92. The spy reports (objects 0xca) the player's building holds on
   other buildings and the player's running spy events 0x40 on buildings no such report covers count against twice
   the target type's level: at the limit the text 0x19fa. Else the window (text 0x19f7 with the owner) asks: its
   first button posts the spy message 0x40 (the target, the guild building, up to eight selected characters as
   ids, flags 1; the text 0x19e3 when nobody is selected), the second button and a right click close it */

void ai_BriberyWindow(char *place, AIBLD *target)
{
    AIMSG msg;
    AIOBJ *reports[0x10];
    char limitText[0x100];
    char text[0x400];
    int win;
    int ok;
    int cancel;
    int base;
    int n;
    int uncovered;
    int i;
    AIBTYPE *type;
    AIBLD *bld;
    AIOBJ *o;
    AIJOB *ev;
    AICHARREC *ch;

    uncovered = 0;
    if (target == 0 || target->owner == 0xffff)
        return;
    if (he_NoProtectionEvent(target) == 0) {
        txt_Format(text, 0x1a92, target->owner, target);
        sv_MessageBox(text, uncovered);
        return;
    }
    type = &((AIBTYPE *)ai_BTypes)[target->type];
    n = 0;
    bld = (AIBLD *)ai_PlayerBuilding;
    if (bld == 0)
        bld = (AIBLD *)ai_FallbackBuilding;
    if (bld != 0)
        for (o = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0xca); o != 0; o = (AIOBJ *)gm_QueryNext())
            if (o->reportBld != target->id)
                reports[n++] = o;
    for (ev = (AIJOB *)he_FindEvent(2, 2, ai_PlayerId, 0, 0x40); ev != 0; ev = (AIJOB *)he_NextEvent()) {
        for (i = 0; i < n; i++)
            if (ev->bldId == reports[i]->reportBld)
                break;
        if (i >= n)
            uncovered++;
    }
    n += uncovered;
    if (n >= type->level * 2) {
        txt_Format(limitText, 0x19fa, n, ai_FallbackBuilding);
        sv_MessageBox(limitText, 0);
        return;
    }
    ch = &((AICHARREC *)ai_Chars)[target->owner];
    win = d2_OpenForm(0, 0, "misc\\Bestechung");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    base = io_PrintText(0x19f7, ch->id, target);
    ok = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            memset(msg.ids, 0xff, sizeof(msg.ids));
            msg.type = 0x40;
            msg.to = ai_CharInts[ai_PlayerId][0];
            msg.flags = 1;
            msg.arg[0] = target->id;
            msg.arg[1] = ((AIBLD *)ai_FallbackBuilding)->id;
            n = 0;
            for (i = 0; i < 0x300 && n < 8; i++)
                if (((AICHARREC *)ai_Chars)[i].selected != 0)
                    msg.ids[n++] = ai_CharInts[i][0];
            if (n != 0) {
                cm_SendLongMessage((char *)&msg);
                snd_PlayFavourComment();
                sv_ClearSelection();
            } else
                sv_MessageBox(txt_Ptrs[0x19e3], 0);
            sv_PopRequest = 1;
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_BriberyWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x594b10, opens the spying map pick (150 B): when the character of the building is free (sv_CheckCharacterFlag
   0x520fe0 is 0) and the building is not 0, the 0x50-byte config is zeroed (flags +4 = 0x400, +0x4c = 0x727,
   +0x10 = 6, +8 = 0) and the prompt 0x19f6 is formatted (txt_Format 0x5176d0) into the 0x400 buffer; the player
   bar opens (0x525220), the map pick runs (0x5b2180: 1, the config, the text, the bribery window 0x594810, 0),
   then the bar closes (0x525440) and the selection is cleared (0x52ae40) */

void ai_OpenSpying(int building)
{
    int cfg[0x14];
    char text[0x400];

    if (sv_CheckCharacterFlag(building) == 0) {
        memset(cfg, 0, 0x50);
        cfg[1] = 0x400;
        if (building != 0) {
            sv_OpenPlayerBar(building, 0);
            cfg[0x13] = 0x727;
            cfg[4] = 6;
            cfg[2] = 0;
            txt_Format(text, 0x19f6);
            ai_MapPick(1, cfg, text, (int)ai_BriberyWindow, 0);
            sv_ClosePlayerBar();
            sv_ClearSelection();
        }
    }
}

void gm_SendMoneyDelta(int a, int b);

/* 0x594bb0, the raid window of the robbers' camp ("locations\raeuberlager\raubzug"; the map pick callback of
   0x594db0, with the picked building; nothing for none or an ownerless one). A building without a protection
   event (he_NoProtectionEvent 0) gives the text 0x1a92; else the window (text 0x1a84) asks: its first button posts
   the raid message 0x75 from the player (the target, the guild building, every selected character as an id,
   flags 1) and takes 1 off the player's money (gm_SendMoneyDelta), or shows the text 0x1a79 when nobody is
   selected; the second button and a right click close it. Clears the selection at the end */

void ai_RaidWindow(char *place, AIBLD *target)
{
    AIMSG msg;
    char text[0x400];
    int win;
    int ok;
    int cancel;
    int base;
    int n;
    int i;

    if (target == 0 || target->owner == 0xffff)
        return;
    if (he_NoProtectionEvent(target) == 0) {
        txt_Format(text, 0x1a92, target->owner, target);
        sv_MessageBox(text, 0);
        return;
    }
    win = d2_OpenForm(0, 0, "locations\\raeuberlager\\raubzug");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    base = io_PrintText(0x1a84, target);
    ok = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            n = 0;
            msg.type = 0x75;
            msg.flags = 1;
            msg.to = ai_CharInts[ai_PlayerId][0];
            msg.arg[0] = target->id;
            msg.arg[1] = ((AIBLD *)ai_FallbackBuilding)->id;
            memset(msg.ids, 0xff, sizeof(msg.ids));
            for (i = 0; i < 0x300; i++)
                if (((AICHARREC *)ai_Chars)[i].selected != 0)
                    msg.ids[n++] = ai_CharInts[i][0];
            if (n != 0) {
                cm_SendLongMessage((char *)&msg);
                gm_SendMoneyDelta(ai_CharInts[ai_PlayerId][0], -1);
                snd_PlayFavourComment();
            } else
                sv_MessageBox(txt_Ptrs[0x1a79], 0);
            sv_PopRequest = 1;
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_RaidWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClearSelection();
}
/* 0x594db0, opens the raid map pick (158 B): as ai_OpenSpying (the character flag 0x520fe0, the 0x50-byte config
   zeroed with +4 = 0x400, +0x4c = 0x727, +0x10 = 6) but +8 = 0xa07443a0, +0xc = 1, the prompt 0x1a83
   (txt_Format 0x5176d0) and the raid window 0x594bb0 as map-pick callback; the bar closes (0x525440) and the
   selection is cleared (0x52ae40) */

void ai_OpenRaid(int building)
{
    int cfg[0x14];
    char text[0x400];

    if (sv_CheckCharacterFlag(building) == 0) {
        memset(cfg, 0, 0x50);
        cfg[1] = 0x400;
        if (building != 0) {
            sv_OpenPlayerBar(building, 0);
            cfg[0x13] = 0x727;
            cfg[4] = 6;
            cfg[2] = 0xa07443a0;
            cfg[3] = 1;
            txt_Format(text, 0x1a83);
            ai_MapPick(1, cfg, text, (int)ai_RaidWindow, 0);
            sv_ClosePlayerBar();
            sv_ClearSelection();
        }
    }
}


/* 0x594e50, the criminal contact listener (643 B): the plates released (0x52e190) and, while the building
   permission 0x200 (0x6f231c) is set, registered again (0x52e1c0): AUSRUESTEN (0x1441fb8, 0xd), AUF_LAUER_LEGEN
   (0x1441fb0, 0x15), ANGRIFF (0x1441fa8, 0x17), and with the records 0x4ffe90 (0x35 / 0x34) of the building's
   objList STROHPUPPE / ZIELSCHEIBE (0x1440ad0 / 0x1440acc, 0x17), then SCHUTZGELD_ERPRESSEN (0x1442010),
   GEBAEUDE_AUSSPIONIEREN (0x1441fe0), RAUBUEBERFALL (0x1440bc8) and REGENERATION (0x1442008), all 0x17; with the
   permission 0x400: LAGER (0x1441f90, 0xe), GELAGE (0x1441f88, 0xe) and TRANSPORT (0x1441fa0, 0x15); 0x5912c0
   runs every frame; a clicked plate (0x6f22e4) opens its window: 0x5bb350, 0x594180, 0x594470, 0x5bba00,
   0x594790, 0x594b10, 0x594db0, 0x5b26b0 (bld, bld->objList), 0x5b9f30, 0x5af1a0; the frame pump 0x5309a0
   (this, 0x67fff) */
void ai_CriminalContact(AIBLD *bld)
{
    int r;
    int ausruesten;
    int lauer;
    int angriff;
    int schutzgeld;
    int spionieren;
    int raub;
    int regen;
    int stroh;
    int ziel;
    int lager;
    int gelage;
    int transport;

    transport = 0;
    lager = 0;
    gelage = 0;
    ausruesten = 0;
    ziel = 0;
    stroh = 0;
    schutzgeld = 0;
    lauer = 0;
    angriff = 0;
    regen = 0;
    spionieren = 0;
    raub = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_CriminalContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            ausruesten = sv_RegisterNamePlate("contact_AUSRUESTEN", ai_ContactTextEquip, 0xd);
            lauer = sv_RegisterNamePlate("contact_AUF_LAUER_LEGEN", ai_ContactTextAmbush, 0x15);
            angriff = sv_RegisterNamePlate("contact_ANGRIFF", ai_ContactTextAttack, 0x17);
            if (gm_Query(bld->objList, 2, 6, 0, 0x35) != 0)
                stroh = sv_RegisterNamePlate("ob_STROHPUPPE", ai_ContactTextTraining, 0x17);
            if (gm_Query(bld->objList, 2, 6, 0, 0x34) != 0)
                ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", ai_ContactTextTarget, 0x17);
            schutzgeld = sv_RegisterNamePlate("contact_SCHUTZGELD_ERPRESSEN", ai_ContactTextExtort, 0x17);
            spionieren = sv_RegisterNamePlate("contact_GEBAEUDE_AUSSPIONIEREN", ai_ContactTextSpy, 0x17);
            raub = sv_RegisterNamePlate("contact_RAUBUEBERFALL", ai_ContactTextRaid, 0x17);
            regen = sv_RegisterNamePlate("contact_REGENERATION", ai_ContactTextRegeneration, 0x17);
        }
        if (ai_BuildingPerms & 0x400) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            gelage = sv_RegisterNamePlate("contact_GELAGE", ai_ContactTextGelage, 0xe);
            transport = sv_RegisterNamePlate("TRANSPORT", ai_ContactTextTransport, 0x15);
        }
        ai_TradeContactChecks();
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, bld->objList);
            else if (ai_ClickedObject == gelage)
                ai_GelageWindow(bld);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
            else if (ai_ClickedObject == ausruesten)
                ai_EquipWindow(bld);
            else if (ai_ClickedObject == ziel || ai_ClickedObject == stroh ||
                     ai_ClickedObject == regen)
                ai_TrainingWindow(bld);
            else if (ai_ClickedObject == lauer)
                ai_BreakInWindow((int)bld);
            else if (ai_ClickedObject == angriff)
                ai_OpenPickpocket((int)bld);
            else if (ai_ClickedObject == schutzgeld)
                ai_OpenBribe((int)bld);
            else if (ai_ClickedObject == spionieren)
                ai_OpenSpying((int)bld);
            else if (ai_ClickedObject == raub)
                ai_OpenRaid((int)bld);
        }
        r = sv_RunFrame(ai_CriminalContact, 0x67fff);
    }
}
/* 0x5950e0, the contact listener of the long-distance trade building (kind 0x47): it releases the name plates and
   pumps the frame (sv_RunFrame, this, mask 0x67fff) while its screen stays on top; every frame it registers the
   FERNEINKAUF plate (text ai_ContactTextFarBuy) and the VERKAUF plate (ai_ContactTextSell), runs the trade checks
   and opens the transport window of the building in buy (mode 4) / sell (mode 2) mode when that plate was clicked */

void ai_FarTradeContact(int building)
{
    int r;
    int buy;
    int sell;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_FarTradeContact, 0x67fff);
    while (r != 0) {
        buy = sv_RegisterNamePlate("FERNEINKAUF", ai_ContactTextFarBuy, 0x10);
        sell = sv_RegisterNamePlate("VERKAUF", ai_ContactTextSell, 0x10);
        ai_TradeContactChecks();
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == buy)
                ai_OpenTransportBuy(building);
            else if (ai_ClickedObject == sell)
                ai_OpenTransportSell(building);
        }
        r = sv_RunFrame(ai_FarTradeContact, 0x67fff);
    }
}

int ai_RunAllowsContact();

/* 0x595170, the smithy contact listener: it releases the name plates, pumps the frame (sv_RunFrame, this, mask
   0x67fff) while the screen stays on top and, every frame, registers the contact plates ai_BuildingPerms allows:
   with 0x200 the production plate PRODUKTION_SCHMIEDEN (text ai_ContactTextProductionSmithy, when
   ai_RunAllowsContact(0x20)) and ob_MEISTERBRIEF (0x80); with 0x400 LAGER (0x10), TRANSPORT (0x100) and
   PERSONALBUCH (0x40). A clicked plate opens its window (the store window ai_StorageWindow on the building's object
   list, ai_ProductionWindow, ai_OpenTransport, ai_PersonnelBook, ai_MasterLetterWindow) and ai_MarketContact runs
   every frame; the plates are released again on the exit */
void ai_SmithyContact(AIBLD *bld, int room)
{
    int r;
    int schmieden;
    int meister;
    int lager;
    int transport;
    int personal;

    lager = 0;
    schmieden = 0;
    transport = 0;
    personal = 0;
    meister = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_SmithyContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            if (ai_RunAllowsContact(0x20) == 1)
                schmieden = sv_RegisterNamePlate("contact_PRODUKTION_SCHMIEDEN", ai_ContactTextProductionSmithy, 0x13);
            else
                schmieden = 0;
            if (ai_RunAllowsContact(0x80) == 1)
                meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            else
                meister = 0;
        }
        if (ai_BuildingPerms & 0x400) {
            if (ai_RunAllowsContact(0x10) == 1)
                lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            else
                lager = 0;
            if (ai_RunAllowsContact(0x100) == 1)
                transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            else
                transport = 0;
            if (ai_RunAllowsContact(0x40) == 1)
                personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            else
                personal = 0;
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, bld->objList);
            else if (ai_ClickedObject == schmieden)
                ai_ProductionWindow((AIBLD *)bld);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
            else if (ai_ClickedObject == personal)
                ai_PersonnelBook(bld, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)bld);
        }
        ai_MarketContact(bld);
        r = sv_RunFrame(ai_SmithyContact, 0x67fff);
    }
    sv_ReleaseNamePlates();
}
/* 0x595330, the carpenter contact listener: it releases the name plates, pumps the frame (sv_RunFrame, this, mask
   0x67fff) while the screen stays on top and, every frame, registers the contact plates ai_BuildingPerms allows:
   with 0x200 the production plate PRODUKTION_TISCHLER (text ai_ContactTextProductionCarpenter); with 0x400 LAGER,
   TRANSPORT, ob_MEISTERBRIEF and ob_PERSONALBUCH. ai_MarketContact runs every frame; a clicked plate opens its
   window (ai_ProductionWindow, ai_OpenTransport, ai_PersonnelBook, the store window ai_StorageWindow on the
   building's object list, ai_MasterLetterWindow) */

void ai_CarpenterContact(AIBLD *bld, int room)
{
    int r;
    int tischler;
    int lager;
    int transport;
    int meister;
    int personal;

    meister = 0;
    lager = 0;
    tischler = 0;
    transport = 0;
    personal = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_CarpenterContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200)
            tischler = sv_RegisterNamePlate("contact_PRODUKTION_TISCHLER", ai_ContactTextProductionCarpenter, 0x13);
        if (ai_BuildingPerms & 0x400) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0xc);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
        }
        ai_MarketContact(bld);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == tischler)
                ai_ProductionWindow((AIBLD *)bld);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
            else if (ai_ClickedObject == personal)
                ai_PersonnelBook(bld, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, bld->objList);
            else if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)bld);
        }
        r = sv_RunFrame(ai_CarpenterContact, 0x67fff);
    }
}

void d2_SetObjEnabled(); void d2_SetCaption(); int d2_WinOpenPic(short x, short y, int pic, int win);

/* 0x595480, the search window of a building ("HANDEL\HANDEL_SUCHEN", frame texts 0x70f / 0x741, title 0x17fe):
   the goods the building's people can search for (0x1c4 .. 0x1c6; type class 0x20: 0x2af, with level 2 also
   0x2b0; class 0x1f: 0x293 .. 0x295; class 8 or level 1: the first two) as picture buttons, each captioned
   with the searches running (the search events 0x14 of the building, LCSEARCHEVENT: 1 the text 0x17ff, more
   0x1800, none the good's name), next to the store's goods list. A click on a good posts the search message
   0x14 (the good, the room object, up to six selected characters) and waits for its slot; no selected
   character gives the text 0x1801. Nothing when the building's character is busy (sv_CheckCharacterFlag) */

void ai_TradeSearchWindow(AIBLD *bld, AIROOM *room)
{
    int n;
    int goods[3];
    int searches[3];
    int store;
    int win;
    int buttons[3];
    AIMSG msg;
    char text[0x100];
    int i;
    int sel;
    int x;
    int good;
    int slot;
    int id;
    int picked;
    AIBTYPE *type;
    LCSEARCHEVENT *ev;

    goods[0] = 0x1c4;
    goods[1] = 0x1c5;
    goods[2] = 0x1c6;
    store = gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    if (type->kind == 0x20) {
        goods[0] = 0x2af;
        n = 1;
        if (type->level >= 2) {
            goods[1] = 0x2b0;
            n = 2;
        }
    } else if (type->kind == 0x1f) {
        goods[0] = 0x293;
        goods[1] = 0x294;
        goods[2] = 0x295;
        n = 3;
    } else if (type->kind == 8) {
        n = 2;
    } else {
        n = 3;
        if (type->level < 2)
            n = 2;
    }
    if (sv_CheckCharacterFlag(bld) != 0)
        return;
    for (i = 0; i < 3; i++)
        searches[i] = 0;
    for (ev = (LCSEARCHEVENT *)he_FindEvent(2, 0, 0x14, 3, bld->id); ev != 0; ev = (LCSEARCHEVENT *)he_NextEvent())
        for (i = 0; i < 3; i++)
            if (ev->good == goods[i]) {
                searches[i] += ev->count;
                break;
            }
    win = d2_OpenForm(0, 0, "HANDEL\\HANDEL_SUCHEN");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x70f, 0x741);
    d2_SetForm(win, 3);
    io_PrintText(0x17fe);
    sv_OpenPlayerBar(bld, 0);
    ai_InitProductionTables();
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    x = d2_Wins[d2_LastWindow].w / 2 - 0xa8;
    for (i = 0; i < n; i++) {
        good = goods[i];
        id = d2_WinOpenPic(x + 0x28, i * 0x50 + 0x19, good + 0xdf, d2_LastWindow);
        d2_Objs[id].interactive = 1;
        d2_Objs[id].style = 3;
        d2_Objs[id].captionX = 100;
        d2_Objs[id].captionY = 0x10;
        d2_Objs[id].font = 0x54;
        d2_SetCaption(id, txt_ObjectNames[good][0]);
        buttons[i] = id;
        d2_SetObjEnabled(id, 1);
    }
    d2_SetPointerSprite(0, 0);
    ai_UpdateProdGoodsList(bld, store);
    ai_LayoutProdGoodsList(win, 1, 0, bld);
    while (sv_RunFrame((int)ai_TradeSearchWindow, 0x677c7) != 0) {
        ai_UpdateProdGoodsList(bld, store);
        ai_LayoutProdGoodsList(win, 1, 0, bld);
        for (i = 0; i < 3; i++)
            searches[i] = 0;
        for (ev = (LCSEARCHEVENT *)he_FindEvent(2, 0, 0x14, 3, bld->id); ev != 0; ev = (LCSEARCHEVENT *)he_NextEvent())
            for (i = 0; i < 3; i++)
                if (ev->good == goods[i]) {
                    searches[i] += ev->count;
                    break;
                }
        d2_SetForm(win, 2);
        for (i = 0; i < n; i++) {
            d2_SetObjEnabled(buttons[i], 1);
            if (searches[i] == 1)
                txt_Format(text, 0x17ff, searches[i], goods[i] * 2 + 0x9a8);
            else if (searches[i] > 1)
                txt_Format(text, 0x1800, searches[i], goods[i] * 2 + 0x9a8);
            else
                txt_Format(text, "%s", goods[i] * 2 + 0x9a8);
            d2_SetCaption(buttons[i], text);
        }
        if (d2_ClickedId != -1) {
            sel = -1;
            for (i = 0; i < n; i++)
                if (buttons[i] == d2_PointerObj) {
                    sel = i;
                    break;
                }
            if (sel != -1) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x14;
                msg.flags = 1;
                msg.to = ai_CharInts[bld->owner][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                ((LCSEARCHWORDS *)msg.arg)->good = (unsigned short)goods[sel];
                ((LCSEARCHWORDS *)msg.arg)->room = room->objId;
                picked = 0;
                for (i = 0; i < 0x300 && picked < 6; i++)
                    if (((AICHARREC *)ai_Chars)[i].selected != 0)
                        msg.ids[picked++] = ai_CharInts[i][0];
                for (i = picked; i < 6; i++)
                    msg.ids[i] = -1;
                ((LCSEARCHWORDS *)msg.arg)->count = (unsigned char)picked;
                if (picked != 0) {
                    snd_PlayFavourComment();
                    sv_ClearSelection();
                    slot = cm_SendLongMessage((char *)&msg);
                    while (cm_GetSlotState(slot) == 0)
                        sv_RunFrame(0, 0x20007);
                    cm_GetSlotValue(slot);
                } else
                    sv_MessageBox(txt_Ptrs[0x1801], 4);
            }
        } else if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    sv_ClearSelection();
    sv_ClosePlayerBar();
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x595a00, the second search window of a building (as ai_TradeSearchWindow: "HANDEL\HANDEL_SUCHEN", frame
   texts 0x70f / 0x741, title 0x1806): always the three goods 0x1c1 .. 0x1c3 as picture buttons, captioned with
   the searches running (the search events 0x15 of the building, LCSEARCHEVENT: 1 the text 0x1807, more 0x1808,
   none the good's name). A click on a good posts the search message 0x15 (LCSEARCHWORDS' count and good, the
   room object in arg[2] where 0x14 has it in arg[1], up to six selected characters) and waits for its slot; no selected character gives the text 0x1801.
   Nothing when the building's character is busy (sv_CheckCharacterFlag) */

void ai_AquariumSearchWindow(AIBLD *bld, AIROOM *room)
{
    int goods[3];
    int searches[3];
    int store;
    int win;
    int buttons[3];
    AIMSG msg;
    char text[0x100];
    int i;
    int sel;
    int x;
    int good;
    int slot;
    int id;
    int picked;
    LCSEARCHEVENT *ev;

    goods[0] = 0x1c1;
    goods[1] = 0x1c2;
    goods[2] = 0x1c3;
    store = gm_Query((void *)bld->objList, 2, 6, 0, 0x2a);
    if (sv_CheckCharacterFlag(bld) != 0)
        return;
    for (i = 0; i < 3; i++)
        searches[i] = 0;
    for (ev = (LCSEARCHEVENT *)he_FindEvent(2, 0, 0x15, 3, bld->id); ev != 0; ev = (LCSEARCHEVENT *)he_NextEvent())
        for (i = 0; i < 3; i++)
            if (ev->good == goods[i]) {
                searches[i] += ev->count;
                break;
            }
    win = d2_OpenForm(0, 0, "HANDEL\\HANDEL_SUCHEN");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x70f, 0x741);
    d2_SetForm(win, 3);
    io_PrintText(0x1806);
    sv_OpenPlayerBar(bld, 0);
    ai_InitProductionTables();
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    x = d2_Wins[d2_LastWindow].w / 2 - 0xa8;
    for (i = 0; i < 3; i++) {
        good = goods[i];
        id = d2_WinOpenPic(x + 0x28, i * 0x50 + 0x19, good + 0xdf, d2_LastWindow);
        d2_Objs[id].interactive = 1;
        d2_Objs[id].style = 3;
        d2_Objs[id].captionX = 100;
        d2_Objs[id].captionY = 0x10;
        d2_Objs[id].font = 0x54;
        d2_SetCaption(id, txt_ObjectNames[good][0]);
        buttons[i] = id;
        d2_SetObjEnabled(id, 1);
    }
    d2_SetPointerSprite(0, 0);
    ai_UpdateProdGoodsList(bld, store);
    ai_LayoutProdGoodsList(win, 1, 0, bld);
    while (sv_RunFrame((int)ai_AquariumSearchWindow, 0x677c7) != 0) {
        ai_UpdateProdGoodsList(bld, store);
        ai_LayoutProdGoodsList(win, 1, 0, bld);
        for (i = 0; i < 3; i++)
            searches[i] = 0;
        for (ev = (LCSEARCHEVENT *)he_FindEvent(2, 0, 0x15, 3, bld->id); ev != 0; ev = (LCSEARCHEVENT *)he_NextEvent())
            for (i = 0; i < 3; i++)
                if (ev->good == goods[i]) {
                    searches[i] += ev->count;
                    break;
                }
        d2_SetForm(win, 2);
        for (i = 0; i < 3; i++) {
            d2_SetObjEnabled(buttons[i], 1);
            if (searches[i] == 1)
                txt_Format(text, 0x1807, searches[i], goods[i] * 2 + 0x9a8);
            else if (searches[i] > 1)
                txt_Format(text, 0x1808, searches[i], goods[i] * 2 + 0x9a8);
            else
                txt_Format(text, "%s", goods[i] * 2 + 0x9a8);
            d2_SetCaption(buttons[i], text);
        }
        if (d2_ClickedId != -1) {
            sel = -1;
            for (i = 0; i < 3; i++)
                if (buttons[i] == d2_PointerObj) {
                    sel = i;
                    break;
                }
            if (sel != -1) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x15;
                msg.flags = 1;
                msg.to = ai_CharInts[bld->owner][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                ((LCSEARCHWORDS *)msg.arg)->good = (unsigned short)goods[sel];
                msg.arg[2] = room->objId; /* the message 0x15 carries the room in arg[2], not at LCSEARCHWORDS.room */
                picked = 0;
                for (i = 0; i < 0x300 && picked < 6; i++)
                    if (((AICHARREC *)ai_Chars)[i].selected != 0)
                        msg.ids[picked++] = ai_CharInts[i][0];
                for (i = picked; i < 6; i++)
                    msg.ids[i] = -1;
                ((LCSEARCHWORDS *)msg.arg)->count = (unsigned char)picked;
                if (picked != 0) {
                    snd_PlayFavourComment();
                    sv_ClearSelection();
                    slot = cm_SendLongMessage((char *)&msg);
                    while (cm_GetSlotState(slot) == 0)
                        sv_RunFrame(0, 0x20007);
                    cm_GetSlotValue(slot);
                } else
                    sv_MessageBox(txt_Ptrs[0x1801], 4);
            }
        } else if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    sv_ClearSelection();
    sv_ClosePlayerBar();
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x595ed0, the perfumery contact listener (388 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with 0x200:
   SAMMELN (0x14401b4, 0x16) -> 0x595480 (param_1, param_2) and PRODUKTION_PARFUMERIE (0x1442098, 0x13) ->
   0x58bd60; with 0x400: LAGER (0x1441f90) -> 0x5b26b0 (param_1, param_1 + 0x5d), TRANSPORT (0x1441fa0) ->
   0x5af1a0, MEISTERBRIEF (0x143cbc8, 0x16) -> 0x5c3200, PERSONALBUCH (0x143c9f0, 0xc) -> 0x5aab80; 0x58f510
   every frame */

void ai_PerfumeryContact(int building, int room)
{
    int r;
    int sammeln;
    int parfum;
    int lager;
    int transport;
    int meister;
    int personal;

    lager = 0;
    transport = 0;
    parfum = 0;
    meister = 0;
    sammeln = 0;
    personal = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_PerfumeryContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            sammeln = sv_RegisterNamePlate("contact_SAMMELN", ai_ContactTextRawMaterialSearch, 0x16);
            parfum = sv_RegisterNamePlate("contact_PRODUKTION_PARFUMERIE", ai_ContactTextProductionPerfumery, 0x13);
        }
        if (ai_BuildingPerms & 0x400) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
        }
        ai_MarketContact((AIBLD *)building);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == parfum)
                ai_ProductionWindow((AIBLD *)building);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport(building);
            else if (ai_ClickedObject == personal)
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == lager)
                ai_StorageWindow(building, ((AIBLD *)building)->objList);
            else if (ai_ClickedObject == sammeln)
                ai_TradeSearchWindow((unsigned char *)building, room);
            else if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)building);
        }
        r = sv_RunFrame(ai_PerfumeryContact, 0x67fff);
    }
}
/* 0x596060, the idle contact listener (28 B): 0x52e190, then the pump 0x5309a0 (this, 0x67fff) until 0 */

void ai_IdleContact(int building, int room)
{
    int r;

    sv_ReleaseNamePlates();
    do {
        r = sv_RunFrame(ai_IdleContact, 0x67fff);
    } while (r != 0);
}
/* 0x596080, the aquarium contact listener (151 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with 0x200 the
   contacts 0x52e1c0 FLIEGENGLAS (0x143cf58, 0x16), TERRARIUM (0x143cf78, 0x16) and TEICH (0x143cf50, 0x16);
   when TEICH is the pending 0x6f22e4, the trade search 0x595a00 (param_1, param_2) opens */

void ai_AquariumContact(int building, int room)
{
    int r;
    int teich;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_AquariumContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            sv_RegisterNamePlate("ob_FLIEGENGLAS", ai_ContactTextFlyGlass, 0x16);
            sv_RegisterNamePlate("ob_TERRARIUM", ai_ContactTextTerrarium, 0x16);
            teich = sv_RegisterNamePlate("ob_TEICH", ai_ContactTextPond, 0x16);
            if (ai_ClickedObject == teich)
                ai_AquariumSearchWindow(building, room);
        }
        r = sv_RunFrame(ai_AquariumContact, 0x67fff);
    }
}
/* 0x596120, the contact listener of the mixing workshop (room kind 0xcf, the profession itself unidentified): it
   releases the name plates, pumps the frame (sv_RunFrame, this, mask 0x67fff) while the screen stays on top and,
   every frame, registers the contact plates ai_BuildingPerms allows: with 0x200 SAMMELN (which opens the trade
   search window for the room) and PRODUKTION_MISCHEN (text ai_ContactTextProductionMixing); with 0x400
   ob_MEISTERBRIEF, ob_PERSONALBUCH, LAGER and TRANSPORT. ai_MarketContact runs every frame; a clicked plate opens
   its window (ai_ProductionWindow, ai_OpenTransport, ai_PersonnelBook, the store window ai_StorageWindow on the
   building's object list, ai_TradeSearchWindow, ai_MasterLetterWindow) */

void ai_MixingContact(AIBLD *bld, int room)
{
    int r;
    int sammeln;
    int mischen;
    int meister;
    int personal;
    int lager;
    int transport;

    meister = 0;
    lager = 0;
    transport = 0;
    mischen = 0;
    sammeln = 0;
    personal = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_MixingContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            sammeln = sv_RegisterNamePlate("contact_SAMMELN", ai_ContactTextRawMaterialSearch, 0x16);
            mischen = sv_RegisterNamePlate("contact_PRODUKTION_MISCHEN", ai_ContactTextProductionMixing, 0x13);
        }
        if (ai_BuildingPerms & 0x400) {
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
        }
        ai_MarketContact(bld);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == mischen)
                ai_ProductionWindow((AIBLD *)bld);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
            else if (ai_ClickedObject == personal)
                ai_PersonnelBook(bld, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, bld->objList);
            else if (ai_ClickedObject == sammeln)
                ai_TradeSearchWindow((unsigned char *)bld, room);
            else if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)bld);
        }
        r = sv_RunFrame(ai_MixingContact, 0x67fff);
    }
}

int ai_AmountMenu(); void ai_TalentWindow(struct AIBLD *bld, int room, int talent);
void ai_PamphletWindow(int bld);

/* 0x5962b0, the stonecutter contact listener: it releases the name plates, pumps the frame (sv_RunFrame, this,
   mask 0x67fff) while the screen stays on top and, every frame, registers the contact plates ai_BuildingPerms
   allows: with 0x200 the production plate PRODUKTION_STEINMETZ (text ai_ContactTextProductionStonecutter); with
   0x400 ob_MEISTERBRIEF, LAGER, TRANSPORT and ob_PERSONALBUCH. ai_MarketContact runs every frame; a clicked plate
   opens its window (ai_ProductionWindow, ai_OpenTransport, ai_PersonnelBook, the store window ai_StorageWindow on
   the building's object list, ai_MasterLetterWindow) */
void ai_StonecutterContact(AIBLD *bld, int room)
{
    int r;
    int steinmetz;
    int meister;
    int lager;
    int transport;
    int personal;

    lager = 0;
    transport = 0;
    steinmetz = 0;
    meister = 0;
    personal = 0;
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_StonecutterContact, 0x67fff);
    while (r != 0) {
        ai_MarketContact(bld);
        if (ai_BuildingPerms & 0x200)
            steinmetz = sv_RegisterNamePlate("contact_PRODUKTION_STEINMETZ", ai_ContactTextProductionStonecutter, 0x13);
        if (ai_BuildingPerms & 0x400) {
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == steinmetz)
                ai_ProductionWindow((AIBLD *)bld);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
            else if (ai_ClickedObject == personal)
                ai_PersonnelBook(bld, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, bld->objList);
            else if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)bld);
        }
        r = sv_RunFrame(ai_StonecutterContact, 0x67fff);
    }
}

#define PF_KIND67   0x100000             /* the partner filter's one-of bits (AMOUNTCFG.mode, ai_PassesPartnerFilter): kind 6 / 7 */
#define PF_KIND5    0x200000             /* kind 5 */
#define PF_SERVANT  0x400000             /* not kind 5, working for a master (ai_WorksForMaster) */
#define PF_KIND0    0x800000             /* kind 0 */
#define PF_ONE_OF   0xf00000             /* one of these must hold */
/* 0x596400, the contact listener of the robbers' room (room kind 0x14, ai_EnterBuilding): every frame the plates
   SABOTAGE (text 0x16fe), VERPRUEGELN (0x1706), BEI_NACHT_UND_NEBEL (0x1fe8) and BESTECHUNG (0x174b); a clicked
   plate runs its order: the sabotage map pick (ai_MapPick with the config flags 1, mode -1, filter 0x3e07c43b4,
   kind 6, and the prompt 0x16ff; callback ai_SabotageCallback), the beating amount menu (ai_AmountMenu with mode 0x400,
   kind 6, the gypsy filter, the prompt 0x1707; callback ai_BeatingCallback), the night-and-fog window
   (ai_TalentWindow, talent 2) or the bribe (ai_OpenRobberBribe) */

void ai_RobberContact(int bld, int room)
{
    int sabotage;
    int beating;
    int nightFog;
    int bribe;
    AMOUNTCFG pick;
    AMOUNTCFG beat;
    char beatText[0x40];
    char pickText[0x80];

    sv_ReleaseNamePlates();
    while (sv_RunFrame((int)ai_RobberContact, 0x67fff) != 0) {
        sabotage = sv_RegisterNamePlate("contact_SABOTAGE", txt_Ptrs[0x16fe], 0x17);
        beating = sv_RegisterNamePlate("contact_VERPRUEGELN", txt_Ptrs[0x1706], 0xd);
        nightFog = sv_RegisterNamePlate("contact_BEI_NACHT_UND_NEBEL", txt_Ptrs[0x1fe8], 0xc);
        bribe = sv_RegisterNamePlate("contact_BESTECHUNG", txt_Ptrs[0x174b], 0x10);
        if (ai_ClickedObject == 0)
            continue;
        if (ai_ClickedObject == sabotage) {
            D3_ZERO(&pick, sizeof(pick));
            pick.flags = 1;
            pick.mode = -1;
            pick.kind = 6;
            pick.filter = 0x3e07c43b4;
            pick.option4c = 0x727;
            txt_Format(pickText, 0x16ff);
            ai_MapPick(1, &pick, pickText, ai_SabotageCallback, 0);
        } else if (ai_ClickedObject == beating) {
            memset(&beat, 0, sizeof(beat));
            beat.mode = 0x400;
            beat.kind = 6;
            beat.filter = gypsyFilter;
            beat.option4c = 0x727;
            txt_Format(beatText, 0x1707);
            ai_AmountMenu(&beat, beatText, 0, ai_BeatingCallback);
            sv_CameraFollow();
        } else if (ai_ClickedObject == nightFog)
            ai_TalentWindow(bld, room, 2);
        else if (ai_ClickedObject == bribe)
            ai_OpenRobberBribe(bld);
    }
}
/* 0x5965f0, the rhetoric contact listener (300 B): releases the name plates, then pumps the frame (0x5309a0,
   this, 0x67fff) while the pump returns 1: the three contact plates (contact_DROHBRIEF / contact_PAMPHLET /
   contact_RHETORIK, the texts 0x1757 / 0x190c / 0x16ae of txt_Ptrs, look 0xc) are registered; with gm_AllowAll
   the P key opens the pamphlet window and the space key the drohbrief window; a clicked plate opens the
   drohbrief window (then the camera follows), the pamphlet window, or the five-text window in mode 4 */

void ai_RhetoricContact(int bld, int room)
{
    int r;
    int drohbrief;
    int pamphlet;
    int rhetorik;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_RhetoricContact, 0x67fff);
    while (r != 0) {
        drohbrief = sv_RegisterNamePlate("contact_DROHBRIEF", txt_Ptrs[0x1757], 0xc);
        pamphlet = sv_RegisterNamePlate("contact_PAMPHLET", txt_Ptrs[0x190c], 0xc);
        rhetorik = sv_RegisterNamePlate("contact_RHETORIK", txt_Ptrs[0x16ae], 0xc);
        if (gm_AllowAll != 0) {
            if (kb_Last == 0x19)
                ai_PamphletWindow(bld);
            if (kb_Last == ' ')
                ai_DrohbriefWindow(bld);
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == drohbrief) {
                ai_DrohbriefWindow(bld);
                sv_CameraFollow();
            } else if (ai_ClickedObject == pamphlet) {
                ai_PamphletWindow(bld);
            } else if (ai_ClickedObject == rhetorik) {
                ai_TalentWindow(bld, room, 4);
            }
        }
        r = sv_RunFrame(ai_RhetoricContact, 0x67fff);
    }
}

void ai_FeastWindow(AIBLD *bld, AIOBJ *table); void ai_DenounceWindow(AIBLD *bld);
short * gm_FindChar(int id);
#define cm_SendExSetHEI ((int (*)(int, char *))cm_SendExSetHE)  /* @original: the callers here push the key as an int and read the return (byte-matched ai_TransportOrderDue), against cm.h's (unsigned char, char *) prototype */
int cm_SendRelations(int a, int b, int c, int d); void cm_SendMoneyDelta(int a, int b); void d2_SetObjEnabled();

/* 0x5966f0, the contact listener of the residence's social room (room kind 0x17 in ai_EnterBuilding; the feast window
   it opens is "locations\wohnsitz\fest"): it releases the name plates and pumps the frame (sv_RunFrame, this, mask
   0x67fff) while the screen stays on top; every frame it registers the three plates contact_FESTGEBEN (text
   txt_Ptrs[0x1766], throw a feast), contact_KAMPF (text 0x16ae, the brawl) and contact_ANSCHWAERZEN (text 0x175e,
   denounce). A clicked plate opens its window: the feast window ai_FeastWindow, the talent window
   ai_TalentWindow for the talent 3, or the denounce window ai_DenounceWindow (then the camera follows) */
void ai_FeastContact(AIBLD *bld, int room)
{
    int r;
    int fest;
    int kampf;
    int ansch;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_FeastContact, 0x67fff);
    while (r != 0) {
        fest = sv_RegisterNamePlate("contact_FESTGEBEN", txt_Ptrs[0x1766], 0x16);
        kampf = sv_RegisterNamePlate("contact_KAMPF", txt_Ptrs[0x16ae], 0xc);
        ansch = sv_RegisterNamePlate("contact_ANSCHWAERZEN", txt_Ptrs[0x175e], 0xc);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == fest) {
                ai_FeastWindow(bld, (AIOBJ *)room);
            } else if (ai_ClickedObject == kampf) {
                ai_TalentWindow(bld, room, 3);
            } else if (ai_ClickedObject == ansch) {
                ai_DenounceWindow(bld);
                sv_CameraFollow();
            }
        }
        r = sv_RunFrame(ai_FeastContact, 0x67fff);
    }
}
typedef struct LCAMOUNTROW {            /* 0x38 bytes, one row of the special-amount list (ai_AmountRowStatus /
                                           ai_AmountRow; cm.c CMAMOUNTROW) */
    AICHARREC *rec;                     /* 0x00, the character the row shows */
    int ctl[2];                         /* 0x04, the row's controls (ai_AmountRow): the frame picture, the liking slider */
    int office;                         /* 0x0c, of a vacant seat: its office, the text 0x253 + it (ai_ShowOfficeRows) */
    int x;                              /* 0x10 */
    int y;                              /* 0x14 */
    int portrait;                       /* 0x18, the portrait picture (ai_ShowAmountRows) */
    int icons[4];                       /* 0x1c, the icons in the frame's corners (ai_ShowAmountRows) */
    int x2c;                            /* 0x2c, a value the row's text shows (the evidence book: the evidence count) */
    int pickable;                       /* 0x30, nonzero: the row's frame takes clicks (ai_AmountRow) */
    int x34;                            /* 0x34, a second value (the evidence book: the open evidence) */
} LCAMOUNTROW;
/* 0x5967c0, the window of one of the player's mistresses "locations\wohnsitz\geliebte2" (from ai_MistressWindow,
   with her character record and her pair event 0x6f): her name and her amount row, two buttons (the texts
   txt_Ptrs[0x1a5c] / [0x1a5d]). The window closes (sv_PopRequest) once she or her event is gone or on a right click.
   The first button asks (0x1a57, or 0x1a58 for a female player) and then sets the event's end time to now, sends it
   (cm_SendExSetHE 1) and waits for its slot. The second, enabled once a year (the event's idbc against the year of
   ai_Time), picks a character in the amount menu (mode 0x20400, the gypsy filter, kind 6; title 0x1a5a), asks
   (0x1a5b) and, the player paying 2, stamps the year, sends the event (cm_SendExSetHE 3), takes the 2 off the
   player's money and lowers the picked character's relation to her by 0x21 (player: the player's record) */

void ai_MistressAffairWindow(AICHARREC *player, AICHARREC *ch, AIJOB *ev)
{
    int win;
    int visit;
    int affair;
    int netId;
    int evId;
    int alive;
    int slot;
    int btn[2];
    AMOUNTCFG cfg;
    AICHARREC *pick;
    char prompt[0x40];
    char text[0x200];
    char ask[0x200];

    evId = ev->id;
    netId = ch->netId;
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\geliebte2");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText("$C$Z$[%1N3$]", ch->id);
    d2_SetForm(win, 3);
    sv_ButtonRow(d2_LastWindow, 0, btn, 2, txt_Ptrs[0x1a5c], txt_Ptrs[0x1a5d]);
    visit = btn[0];
    affair = btn[1];
    d2_SetForm(win, 2);
    {
        LCAMOUNTROW row;                /* her row, centred (its stack place is shared with the texts below) */

        row.x = (d2_LastWindowPtr->w - d2_Pics[0x5da].w) / 2;
        row.y = 0x19;
        row.rec = ch;
        row.pickable = 0;
        ai_AmountRowStatus(&row);
        ai_AmountRow(row.x, row.y, d2_LastWindow, 0x54, &row, 1);
    }
    d2_ClickedId = -1;
    do {
        if (gm_FindChar(netId) != 0 && he_FindEvent(1, 1, evId) != 0)
            alive = 1;
        else
            alive = 0;
        if (alive && ev->idbc < ai_Time.year)       /* once a year */
            d2_SetObjEnabled(affair, 1);
        else
            d2_SetObjEnabled(affair, 0);
        if (dm_L.rup == 0 && alive) {
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == visit) {
                    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
                        txt_Format(text, 0x1a57, ch->id);
                    else
                        txt_Format(text, 0x1a58, ch->id);
                    if (sv_MessageBox(text, 1) != 0) {
                        ev->end = ai_Time;
                        ev->idbc = ai_Time.year;
                        slot = cm_SendExSetHEI(1, ev);
                        if (slot != -1)
                            do {
                                if (cm_GetSlotState(slot) != 0)
                                    break;
                                sv_RunFrame(0, 0x20007);
                            } while (1);
                        sv_PopRequest = 1;
                    }
                } else if (d2_PointerObj == affair) {
                    char title[0x40];

                    memset(&cfg, 0, sizeof(cfg));
                    cfg.mode = 0x20400;
                    cfg.kind = 6;
                    cfg.filter = gypsyFilter;
                    txt_Format(title, 0x1a53);
                    txt_Format(prompt, 0x1a54);
                    pick = (AICHARREC *)ai_AmountMenu(&cfg, txt_Ptrs[0x1a5a], 0, 0);
                    if (pick != 0) {
                        txt_Format(ask, 0x1a5b, 2, ch->id, pick->id);
                        if (sv_MessageBox(ask, 1) != 0 && sv_CheckMoney(ai_Chars[ai_PlayerId], 2) != 0) {
                            ev->idbc = ai_Time.year;
                            cm_SendExSetHEI(3, ev);
                            gm_SendMoneyDelta(player->netId, -2);
                            cm_SendRelations(pick->netId, ch->netId, -0x21, 1);
                            sv_PopRequest = 1;
                        }
                    }
                }
            }
        } else {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame(ai_MistressAffairWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x596ba0, the mistress window "locations\wohnsitz\geliebte" of the residence bld (the GELIEBTE plate of
   ai_MistressContact): the button to take a mistress (text 0x1a50, 0x1a51 for a female player) and, every frame,
   the player's up to four mistresses: the pair events 0x6f whose first partner is the player (the second partner
   is the mistress, the event's stage 0 enables her button, text 0x1a52 with her name). The take button (enabled
   below four) picks a character in the amount menu (mode 0x2040a, 0x20409 for a female player; the gypsy
   filter, kind 6; the texts 0x1a53 / 0x1a54), asks (0x1a55 / 0x1a56) and posts the message 0x6f (flags 2, from
   the building, the player and the picked character); a mistress' button opens her window
   ai_MistressAffairWindow with this one disabled. A right click closes it; then the camera follows
   @deep size queue s16: cfg, msg, prompt / title, text and pick declared in the blocks that use them (all
   31 subsets): two codes, the control's and one 2 bytes off (msg block-local); no lever for the -1 /
   store-order hunks. prior: Reconstructed whole from the original (all statements, calls, the 0x6f message
   record, the amount-menu config, the texts 0x1a50..0x1a56 and the mistress buttons read and compared; the
   -1 inits are inline memsets, which gives the original's ebx=0). Left is allocation/scheduling only: the
   original keeps the -1 of memset(ctl) in esi up to d2_ClickedId = esi and skips the loop-top 'or esi,-1'
   on the first pass (jmp +0xa2), ours materializes -1 again ('mov [d2_ClickedId], 0xffffffff', 'or eax,-1;
   or ecx,-1'); the player-record index (ai_PlayerId*0x21c) is computed before/after the ev+0xac load, the
   cfg/msg field stores are scheduled in another order (mode/filter/kind, to/fromId/arg[0]/arg[1]) and the
   0x1a55/0x1a56 arms swap eax/ecx. Tried: explicit -1 stores in the original's order (ebx stayed -1),
   if/else for the sex and stage tests, field reorderings */

void ai_MistressWindow(AIBLD *bld)
{
    int win;
    int take;
    int n;
    int i;
    int charIds[4];                     /* the mistresses' netIds (the events' partner2) */
    int ctl[4];                         /* their buttons */
    int evIds[4];                       /* their pair events */
    int enabled[4];                     /* the event's stage is 0 */
    AMOUNTCFG cfg;
    AIMSG msg;
    AIJOB *ev;
    AICHARREC *pick;
    char prompt[0x40];
    char title[0x40];
    char text[0x200];

    memset(ctl, 0xff, sizeof(ctl));
    sv_ReleaseNamePlates();
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\geliebte");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
        take = d2_GetWinObj(win, 2, io_PrintText(0x1a50));
    else
        take = d2_GetWinObj(win, 2, io_PrintText(0x1a51));
    d2_ClickedId = -1;
    do {
        memset(evIds, 0xff, sizeof(evIds));
        memset(charIds, 0xff, sizeof(charIds));
        n = 0;
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x6f); ev != 0 && n < 4; ev = (AIJOB *)he_NextEvent()) {
            if (ev->partner1 == ai_CharInts[ai_PlayerId][0]) {
                evIds[n] = ev->id;
                if (ev->stage == 0)
                    enabled[n] = 1;
                else
                    enabled[n] = 0;
                charIds[n] = ev->partner2;
                n++;
            }
        }
        d2_SetForm(win, 1);
        io_PrintText("$C");
        io_PrintText("$Z$[%s$]$N", 0x1a4e);
        io_PrintText(0x1a4f);
        for (i = 0; i < 4; i++) {
            AICHARREC *ch = (AICHARREC *)gm_FindChar(charIds[i]);

            if (ch == 0)
                break;
            ctl[i] = d2_GetWinObj(win, 1, io_PrintText(0x1a52, ch->id));
            d2_SetObjEnabled(ctl[i], enabled[i]);
        }
        if (n >= 4)
            d2_SetObjEnabled(take, 0);
        else
            d2_SetObjEnabled(take, 1);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == take) {
                memset(&cfg, 0, sizeof(cfg));
                cfg.kind = 6;
                cfg.filter = gypsyFilter;
                cfg.mode = ((AICHARREC *)ai_Chars)[ai_PlayerId].sex != 0 ? 0x20409 : 0x2040a;
                txt_Format(title, 0x1a53);
                txt_Format(prompt, 0x1a54);
                pick = (AICHARREC *)ai_AmountMenu(&cfg, title, prompt, 0);
                if (pick != 0) {
                    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
                        txt_Format(text, 0x1a55, pick->id);
                    else
                        txt_Format(text, 0x1a56, pick->id);
                    if (sv_MessageBox(text, 1) != 0) {
                        memset(&msg, 0, sizeof(msg));
                        msg.type = 0x6f;
                        msg.flags = 2;
                        msg.to = ai_CharInts[ai_PlayerId][0];
                        msg.fromId = bld->id;
                        msg.arg[0] = ai_CharInts[ai_PlayerId][0];
                        msg.arg[1] = pick->netId;
                        cm_SendLongMessage((char *)&msg);
                    }
                }
            } else {
                for (i = 0; i < 4; i++) {
                    if (d2_PointerObj == ctl[i]) {
                        AICHARREC *ch = (AICHARREC *)gm_FindChar(charIds[i]);

                        ev = (AIJOB *)he_FindEvent(1, 1, evIds[i]);
                        d2_SetViewEnabled(win, 0);
                        ai_MistressAffairWindow((AICHARREC *)ai_Chars[ai_PlayerId], ch, ev);
                        d2_SetViewEnabled(win, 1);
                        break;
                    }
                }
            }
        }
    } while (sv_RunFrame(ai_MistressWindow, 0x67fff) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x596f90, the mistress listener (95 B): the pump 0x5309a0 (this, 0x67fff); the contact GELIEBTE
   (0x1440af8, 0x16) opens the mistress window 0x596ba0 (param_1) when it is the pending 0x6f22e4 */

void ai_MistressContact(int building, int room)
{
    int r;
    int geliebte;

    r = sv_RunFrame(ai_MistressContact, 0x67fff);
    while (r != 0) {
        geliebte = sv_RegisterNamePlate("contact_GELIEBTE", ai_ContactTextMistress, 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == geliebte)
            ai_MistressWindow((AIBLD *)building);
        r = sv_RunFrame(ai_MistressContact, 0x67fff);
    }
}

int gm_TitlesLeft(); float gm_TitlePeriodServed(int n, int *out); int d2_WinOpenSlider();
int gm_TitleGroup(); void gm_AddTime(struct AITIME *c, int hours, int mins, int secs);

/* 0x596ff0, the master-exam window "locations\wohnsitz\meisterpruefung" (the MEISTERPRUEFUNG plate of the
   residence): the player's next title (gm_TitlesLeft of the job + 1) and the share of the current title's period
   served (gm_TitlePeriodServed, with the time still to serve). A pending exam event 0x26 shows the text 0x15bd
   (the job's title name); a period not yet served shows 0x15bc with the time left and a 0..100 slider of the
   share, or 0x15be past the sixth title; otherwise the exam offer 0x15bf (fee 4, the title name), 0x15c0 for an
   odd title and 0x15c1. Its button 0 takes the exam: under 4 in money the text 0x74, else the player pays 4 and
   posts the exam message 0x26 (flags 9, from -1, due in two hours) and the window closes; button 1 or a right
   click close it; then the camera follows */
void ai_MasterExamWindow(AIBLD *bld)
{
    int win;
    int next;
    int slider;
    int title;
    AICHARREC *ch;
    AIJOB *exam;                        /* a pending exam event 0x26 of the player */
    float served;
    AITIME left;
    char text[0x100];

    ch = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    next = gm_TitlesLeft(ch->jobId) + 1;
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\meisterpruefung");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    sv_ResetScreenClip();
    served = gm_TitlePeriodServed(ai_PlayerId, (int *)&left);
    exam = (AIJOB *)he_FindEvent(2, 2, ai_PlayerId, 0, 0x26);
    if (exam == 0 && served >= 1.0 && next <= 6) {
        title = ch->sex != 0 ? 0x18f : 0x130;   /* the job titles' texts: 0x130.., female 0x18f.. */

        gm_TitleGroup(ch->jobId);
        d2_SetForm(win, 1);
        io_PrintText(0x15bb);
        d2_SetForm(win, 2);
        io_PrintText(0x15bf, ch->id, 4, ch->jobId - 1 + title);
        if (next % 2 != 0)
            io_PrintText(0x15c0);
        d2_SetForm(win, 3);
        io_PrintText(0x15c1);
    } else if (exam != 0) {
        d2_SetForm(win, 1);
        io_PrintText(0x15bb);
        d2_SetForm(win, 2);
        if (ch->sex != 0)
            title = 0x18f;
        else
            title = 0x130;
        io_PrintText(0x15bd, ch->id, ch->jobId - 1 + title);
    } else if (next >= 7) {
        d2_SetForm(win, 1);
        io_PrintText(0x15bb);
        d2_SetForm(win, 2);
        io_PrintText(0x15be, ch->id);
    } else {
        d2_SetForm(win, 1);
        io_PrintText(0x15bb);
        d2_SetForm(win, 2);
        io_PrintText(0x15bc, ch->id, &left);
        slider = d2_WinOpenSlider(0x1c, d2_LastWindowPtr->h - 0x3c, d2_LastWindowPtr->w - 0x40, 0, 100, 0x83, 0x42,
                                  d2_LastWindow);
        d2_SetObjValue4(slider, 0, 100, (int)(served * 100.0f));
    }
    while (sv_RunFrame(ai_MasterExamWindow, 0x677c7) != 0) {
        if (d2_ClickedId != -1 && served >= 1.0) {
            if (d2_ClickedId == 0) {
                if (ch->money < 4) {
                    txt_Format(text, 0x74, 4);
                    sv_MessageBox(text, 4);
                } else {
                    AIMSG msg;

                    gm_SendMoneyDelta(ch->netId, -4);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x26;
                    msg.to = ch->netId;
                    msg.fromId = -1;
                    msg.flags = 9;
                    msg.arg[0] = ch->netId;
                    msg.time = ai_Time;
                    gm_AddTime(&msg.time, 2, 0, 0);
                    cm_SendLongMessage((char *)&msg);
                    sv_PopRequest = 1;
                }
            } else if (d2_ClickedId == 1) {
                sv_PopRequest = 1;
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(win);
    sv_CameraFollow();
}

/* 0x597380, the espionage contact's amount menu (107 B): the config (mode 0x400, kind 6, the filter
   gypsyFilter 0x5eed68, the rest cleared) and the text 0x16f9 go to 0x5c1130 (the config, the text, 0, the
   callback 0x5b3ad0), then the camera follows */

void ai_EspionageAmountMenu(AIBLD *bld)
{
    AMOUNTCFG cfg;
    char text[0x80];

    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    txt_Format(text, 0x16f9);
    ai_AmountMenu(&cfg, text, 0, (void *)ai_EspionageAmountCB);
    sv_CameraFollow();
}

int gm_HasAdultRelative(); float gm_AddHealth(unsigned short id, int d); int ai_TutorialOff(); void ai_EvidenceBookWindow(struct AIBLD *bld);

/* 0x5973f0, the suicide window of the residence (the SELBSTMORD plate; the form "locations\wohnsitz\weinkeller",
   texts 0x42cf / 0x42d1): button 0 asks gm_HasAdultRelative of the player's record; with an adult relative (an
   heir) the player's character loses 2000 health (gm_AddHealth -0x7d0) and the window stays, else the text
   txt_Ptrs[0x42d2] (no relative) or [0x42d3] (none adult) and it closes; button 1, a right click or the escape
   key close it */
void ai_SuicideWindow(AIBLD *bld)
{
    int win;
    int relatives;
    AICHARREC *ch;

    ch = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\weinkeller");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x42cf);
    d2_SetForm(win, 2);
    io_PrintText(0x42d1);
    while (sv_RunFrame(ai_SuicideWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0 || kb_Down[1] != 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId == 0) {
            relatives = d2_ClickedId;
            if (gm_HasAdultRelative((char *)ch, &relatives) != 0)
                gm_AddHealth(ch->id, -0x7d0);
            else {
                sv_MessageBox(txt_Ptrs[relatives != 0 ? 0x42d3 : 0x42d2], 0);
                sv_PopRequest = 1;
            }
        } else if (d2_ClickedId == 1)
            sv_PopRequest = 1;
    }
    d2_CloseView(win);
}
/* 0x597500, the contact listener of the residence's study (room kind 0x13 in ai_EnterBuilding): it releases the
   name plates and pumps the frame (sv_RunFrame, this, 0x67fff); every frame the plates contact_VERHANDELN
   (txt_Ptrs[0x1ffe]) and contact_HANDWERKSKUNST ([0x1ff0]) when ai_RunAllowsContact(8), and contact_BEWEISBUCH
   ([0x173e]), contact_SPIONAGE ([0x16f8]), contact_MEISTERPRUEFUNG ([0x1ff4]) and contact_SELBSTMORD ([0x42d4])
   past the tutorial. A clicked plate opens the five-text window (mode 0 / 1), the evidence book, the master exam, the
   espionage amount menu or the suicide window; with no plate clicked, ai_DebugHotkeys and the '-' key open the
   mistress window */

void ai_StudyContact(AIBLD *bld, int room)
{
    int r;
    int verhandeln;
    int handwerk;
    int beweis;
    int spionage;
    int meister;
    int selbstmord;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_StudyContact, 0x67fff);
    while (r != 0) {
        if (ai_RunAllowsContact(8) == 1) {
            verhandeln = sv_RegisterNamePlate("contact_VERHANDELN", txt_Ptrs[0x1ffe], 0xc);
            handwerk = sv_RegisterNamePlate("contact_HANDWERKSKUNST", txt_Ptrs[0x1ff0], 0xc);
        } else {
            verhandeln = 0;
            handwerk = 0;
        }
        if (ai_TutorialOff() == 1) {
            beweis = sv_RegisterNamePlate("contact_BEWEISBUCH", txt_Ptrs[0x173e], 0xc);
            spionage = sv_RegisterNamePlate("contact_SPIONAGE", txt_Ptrs[0x16f8], 0x17);
            meister = sv_RegisterNamePlate("contact_MEISTERPRUEFUNG", txt_Ptrs[0x1ff4], 0xc);
            selbstmord = sv_RegisterNamePlate("contact_SELBSTMORD", txt_Ptrs[0x42d4], 0x24);
        } else {
            beweis = 0;
            spionage = 0;
            meister = 0;            /* selbstmord stays unset (the original's else path leaves its slot) */
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == verhandeln)
                ai_TalentWindow(bld, room, 0);
            else if (ai_ClickedObject == handwerk)
                ai_TalentWindow(bld, room, 1);
            else if (ai_ClickedObject == beweis)
                ai_EvidenceBookWindow(bld);
            else if (ai_ClickedObject == meister)
                ai_MasterExamWindow(bld);
            else if (ai_ClickedObject == spionage)
                ai_EspionageAmountMenu(bld);
            else if (ai_ClickedObject == selbstmord)
                ai_SuicideWindow(bld);
        } else if (ai_DebugHotkeys != 0 && kb_Last == '-') {
            ai_MistressWindow(bld);
        }
        r = sv_RunFrame(ai_StudyContact, 0x67fff);
    }
}

void d2_CreateScrollButtons(); int d2_RemoveWindowObjects();

/* 0x5976a0, the tavern rules window "locations\Wirtshaus\WS_regeln" (210 B): the form is opened and centred,
   fitted to the screen (sv_FitForm), the pointer hidden, then the frames 2 / 3 / 1 / 2 with the scroll buttons
   of 0x767 and the texts 0x1889 / 0x188a printed; the pump runs until the right button leaves the window */

void ai_TavernRulesWindow(void)
{
    int win, obj;

    win = d2_OpenForm(0, 0, "locations\\Wirtshaus\\WS_regeln");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    obj = d2_LastWindow;
    d2_SetForm(win, 3);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, obj, 0x767);
    d2_SetForm(win, 1);
    io_PrintText(0x1889);
    d2_SetForm(win, 2);
    io_PrintText(0x188a);
    while (sv_RunFrame(ai_TavernRulesWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(win);
}
/* 0x597780, draws three dice (134 B): the window's objects are removed (d2_RemoveWindowObjects), the caption is
   printed (io_PrintText "$C$Z%s"), then the three dice cells at x = 0x1c + i * 0x3a (0 every 8) and y = 0x1d
   (0x46 after 8), each a d2_WinOpenPic with the random picture 0x75b + (util_Random(6) & 0xffff) */

void ai_DrawDice(char *caption)
{
    int i;
    int x;
    int y;

    d2_RemoveWindowObjects(d2_LastWindow, 1);
    io_PrintText("$C$Z%s", caption);
    i = 0;
    x = 0x1c;
    y = 0x1d;
    do {
        d2_WinOpenPic(x, y, (util_Random(6) & 0xffff) + 0x75b, d2_LastWindow);
        i++;
        if (i % 8 == 0)
            x = 0;
        else
            x += 0x3a;
        y = i > 7 ? 0x46 : 0x1d;
    } while (i < 3);
}
typedef struct LCDUELSIDE {             /* one side of a dice duel: am.h AGDUEL's side1 (+0x0c) or side2 (+0x30) block
                                           (am.h cannot be included here yet) */
    AICHARREC *ch;                      /* 0x00, the character throwing (AGDUEL.side1) */
    unsigned char n;                    /* 0x04, the dice thrown (AGDUEL.n1) */
    char pad05[3];
    unsigned int stats;                 /* 0x08 (AGDUEL.stats1) */
    unsigned char t[0x10];              /* 0x0c, the thrown values 1 .. 6 (AGDUEL.t1) */
} LCDUELSIDE;
/* 0x597810, draws one side of a dice duel into the last window: the window's objects are removed, the caption
   printed ("$C$Z%s"), then the side's dice in rows of eight from (0x1c, 0x1d), 0x3a apart (the second row at
   y 0x46): the thrown values (the pictures 0x75b .. 0x760) for the player's character (kind 6), the hidden die
   0x761 for an opponent; then, in each branch, one more random die cell 0x75b + util_Random(6) */

void ai_DrawDuelDice(char *caption, LCDUELSIDE *side)
{
    int i;
    int x;
    int y;

    d2_RemoveWindowObjects(d2_LastWindow, 1);
    io_PrintText("$C$Z%s", caption);
    i = 0;
    if (side->ch->kind == 6) {
        x = 0x1c;
        y = 0x1d;
        while (i < side->n) {
            d2_WinOpenPic(x, y, side->t[i] + 0x75a, d2_LastWindow);
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = i > 7 ? 0x46 : 0x1d;
        }
        d2_WinOpenPic(x, y, (util_Random(6) & 0xffff) + 0x75b, d2_LastWindow);
    } else {
        x = 0x1c;
        y = 0x1d;
        while (i < side->n) {
            d2_WinOpenPic(x, y, 0x761, d2_LastWindow);
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = i > 7 ? 0x46 : 0x1d;
        }
        d2_WinOpenPic(x, y, (util_Random(6) & 0xffff) + 0x75b, d2_LastWindow);
    }
}
/* 0x597920, draws one side of a dice duel as thrown (as ai_DrawDuelDice without the random cell): the
   window's objects removed, the caption, then the thrown values (0x75b .. 0x760) for the player's character
   (kind 6) or the hidden die 0x761, in rows of eight */

void ai_DrawDuelThrow(char *caption, LCDUELSIDE *side)
{
    int i;
    int x;
    int y;

    d2_RemoveWindowObjects(d2_LastWindow, 1);
    io_PrintText("$C$Z%s", caption);
    i = 0;
    x = 0x1c;
    if (side->ch->kind == 6) {
        y = 0x1d;
        while (i < side->n) {
            d2_WinOpenPic(x, y, side->t[i] + 0x75a, d2_LastWindow);
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = i > 7 ? 0x46 : 0x1d;
        }
    } else {
        y = 0x1d;
        while (i < side->n) {
            d2_WinOpenPic(x, y, 0x761, d2_LastWindow);
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = i > 7 ? 0x46 : 0x1d;
        }
    }
}
typedef struct LCDUEL {                 /* a dice duel (am.h AGDUEL, same layout; am.h cannot be included here yet) */
    int stake;                          /* 0x00, the first throw's stake (AGDUEL.stake) */
    int raiseAmount;                    /* 0x04, a tenth of the stake, paid on the throws of kind 2 / 3 (AGDUEL.raiseAmount) */
    int phase;                          /* 0x08, 5 / 6 at the end: side1 / side2 won (AGDUEL.phase) */
    LCDUELSIDE side1;                   /* 0x0c (AGDUEL.side1 .. t1) */
    int kind1;                          /* 0x28, the action side1 chose, 4 answer (AGDUEL.kind1) */
    int reserved2c;                     /* 0x2c */
    LCDUELSIDE side2;                   /* 0x30 (AGDUEL.side2 .. t2) */
    int kind2;                          /* 0x4c (AGDUEL.kind2) */
    int reserved50;                     /* 0x50 */
    int total;                          /* 0x54, the stakes paid so far (AGDUEL.total) */
} LCDUEL;
/* 0x597a10, shows a duel side's throw in the frame `frame` of the window win: the caption (by mode: 0x1883 the
   name, 0x1884 / 0x1885 / 0x1886 the name and the raise amount). Without `fast`, modes 1 and 2 roll for `wait`
   ms, redrawing the rolling dice (ai_DrawDice, ai_DrawDuelDice) at growing steps (2 ms, then half again each
   time), and modes 3 and 4 draw the throw (ai_DrawDuelThrow) and wait `wait` ms; with `fast` every mode draws
   the throw and waits. The frames are pumped with sv_RunFrame(0, 0x777c7); it returns as soon as the player's
   character is dead (health 0)
   @deep size queue s17: the 7 bytes over the original (1092 vs 1085) are exactly the second mode load: the
   original loads mode once in the first block (test fast; mov eax,[mode]; .. dec eax in both switches),
   ours loads it in each arm (mov eax/edx,[esp+0x120], 7 bytes each). c2trace lists no mode candidate (one
   reference per arm); a local m = mode before or after step = 2 = control; the arms swapped (if (fast !=
   0)) 270. climb round 1: best 187 ins (end = d3_Time moved behind ai_DrawDuelThrow in the fast tail: the
   dead read's position; not kept, it is the draft's dead read). Next: what gives mode a reference before
   the if (a read the original keeps, e.g. a guard) or a mode candidate that spans both switches. prior:
   Reconstructed whole from the original: both switches on mode (the slow one: modes 1/2 roll with
   ai_DrawDice / ai_DrawDuelDice at growing steps step += step/2, modes 3/4 draw ai_DrawDuelThrow and wait;
   the fast one formats 0x1883..0x1886 and shares the throw-and-wait tail), the player's health check
   (return), the texts with the name +0x30 and the duel's raiseAmount, the second (dead) volatile d3_Time
   read of the throw paths. Left: allocation/scheduling: the original hoists the mode load above the fast
   test ('test eax,eax; mov eax,[esp+0x110]' and 'dec eax' in the second switch), ours loads it in each arm
   ('mov edx,[esp+0x120]; lea eax,[edx-1]'), which shifts the second switch's case bodies (tables differ
   only by those offsets); the health byte goes through al/edx in the original, cl/eax in ours; 'end =
   d3_Time + wait' loads the clock before/after the add. Tried: the fast arm as an else (n... */

void ai_DuelDiceAnimation(LCDUEL *duel, LCDUELSIDE *side, int mode, int win, int frame, int wait, int fast)
{
    char text[0x100];
    int step;
    unsigned int t0;
    unsigned int end;

    step = 2;
    if (fast == 0) {
        switch (mode) {
        case 1:
            txt_Format(text, 0x1883, side->ch->name);
            end = d3_Time + wait;
            t0 = d3_Time;
            while (end > d3_Time) {
                sv_RunFrame(0, 0x777c7);
                if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
                    return;
                if (t0 + step < d3_Time) {
                    d2_SetForm(win, frame);
                    ai_DrawDice(text);
                    t0 = d3_Time;
                    step += step / 2;
                }
            }
            break;
        case 2:
            txt_Format(text, 0x1884, side->ch->name, duel->raiseAmount);
            end = d3_Time + wait;
            t0 = d3_Time;
            while (end > d3_Time) {
                sv_RunFrame(0, 0x777c7);
                if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
                    return;
                if (t0 + step < d3_Time) {
                    d2_SetForm(win, frame);
                    ai_DrawDuelDice(text, side);
                    t0 = d3_Time;
                    step += step / 2;
                }
            }
            break;
        case 3:
            txt_Format(text, 0x1885, side->ch->name, duel->raiseAmount);
            t0 = d3_Time;
            end = d3_Time;
            d2_SetForm(win, frame);
            ai_DrawDuelThrow(text, side);
            end = t0 + wait;
            while (end > d3_Time) {
                if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
                    return;
                sv_RunFrame(0, 0x777c7);
            }
            break;
        case 4:
            txt_Format(text, 0x1886, side->ch->name, duel->raiseAmount);
            t0 = d3_Time;
            end = d3_Time;
            d2_SetForm(win, frame);
            ai_DrawDuelThrow(text, side);
            end = t0 + wait;
            while (end > d3_Time) {
                if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
                    return;
                sv_RunFrame(0, 0x777c7);
            }
            break;
        }
    } else {
        switch (mode) {
        case 1:
            txt_Format(text, 0x1883, side->ch->name);
            break;
        case 2:
            txt_Format(text, 0x1884, side->ch->name, duel->raiseAmount);
            break;
        case 3:
            txt_Format(text, 0x1885, side->ch->name, duel->raiseAmount);
            break;
        case 4:
            txt_Format(text, 0x1886, side->ch->name, duel->raiseAmount);
            break;
        }
        t0 = d3_Time;
        end = d3_Time;
        d2_SetForm(win, frame);
        ai_DrawDuelThrow(text, side);
        end = t0 + wait;
        while (end > d3_Time) {
            if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
                return;
            sv_RunFrame(0, 0x777c7);
        }
    }
}
/* 0x597e70, draws the end of a dice duel into the window win: the frames 2 and 3 cleared, then each side's
   caption ("$C$Z%s", the character's name) and dice in rows of eight from (0x1c, 0x1d), 0x3a apart, in frame
   3 for the player's character (kind 6: the thrown values 0x75b .. 0x760) or frame 2 for the opponent (its
   values, or the hidden die 0x761 while side1's action is 4; the original reads kind1 for both sides); then
   frame 4 cleared and the result: 0x1887 with the total when the player's side won (phase 5 side1, 6 side2),
   else 0x1888 with half the total */

void ai_DrawDuelEnd(LCDUEL *duel, int win)
{
    int i;
    int x;
    int y;

    d2_SetForm(win, 2);
    d2_RemoveWindowObjects(d2_LastWindow, 1);
    d2_SetForm(win, 3);
    d2_RemoveWindowObjects(d2_LastWindow, 1);
    if (duel->side1.ch->kind == 6) {
        d2_SetForm(win, 3);
        io_PrintText("$C$Z%s", duel->side1.ch->name);
        x = 0x1c;
        y = 0x1d;
        i = 0;
        while (i < duel->side1.n) {
            d2_WinOpenPic(x, y, duel->side1.t[i] + 0x75a, d2_LastWindow);
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = i > 7 ? 0x46 : 0x1d;
        }
    } else {
        d2_SetForm(win, 2);
        io_PrintText("$C$Z%s", duel->side1.ch->name);
        x = 0x1c;
        y = 0x1d;
        i = 0;
        while (i < duel->side1.n) {
            if (duel->kind1 == 4)
                d2_WinOpenPic(x, y, 0x761, d2_LastWindow);
            else
                d2_WinOpenPic(x, y, duel->side1.t[i] + 0x75a, d2_LastWindow);
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = i > 7 ? 0x46 : 0x1d;
        }
    }
    if (duel->side2.ch->kind == 6) {
        d2_SetForm(win, 3);
        io_PrintText("$C$Z%s", duel->side2.ch->name);
        x = 0x1c;
        y = 0x1d;
        i = 0;
        while (i < duel->side2.n) {
            d2_WinOpenPic(x, y, duel->side2.t[i] + 0x75a, d2_LastWindow);
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = i > 7 ? 0x46 : 0x1d;
        }
    } else {
        d2_SetForm(win, 2);
        io_PrintText("$C$Z%s", duel->side2.ch->name);
        x = 0x1c;
        y = 0x1d;
        i = 0;
        while (i < duel->side2.n) {
            if (duel->kind1 == 4)
                d2_WinOpenPic(x, y, 0x761, d2_LastWindow);
            else
                d2_WinOpenPic(x, y, duel->side2.t[i] + 0x75a, d2_LastWindow);
            i++;
            if (i % 8 == 0)
                x = 0;
            else
                x += 0x3a;
            y = i > 7 ? 0x46 : 0x1d;
        }
    }
    d2_SetForm(win, 4);
    d2_RemoveWindowObjects(d2_LastWindow, 1);
    if ((duel->phase == 5 && duel->side1.ch->kind == 6) || (duel->phase == 6 && duel->side2.ch->kind == 6))
        io_PrintText((char *)0x1887, duel->total);
    else
        io_PrintText((char *)0x1888, duel->total / 2);
}

int gm_GetWorth(unsigned short id); int gm_ConvertMoney(int a, unsigned char k); short * gm_PlayerRecord(char *c); int ai_DuelStart();
int ai_DuelAct(); void ai_DuelNextPhase(); int ai_DuelOpponentAction(); int ai_DuelAcceptAction();

typedef struct LCFIN {                  /* what gm_PlayerRecord finds for a character (ai.h AIFIN; ai.h cannot be included here yet) */
    char pad00[0x28];
    int spyPaid;                        /* 0x28, AIFIN.spyPaid: the money paid to spies and bribed characters (ai_BribeAmountCB) */
    char pad2c[0x18];
    int renovationSpent;                /* 0x44, AIFIN.renovationSpent: the money paid for renovations (ai_RenovationWindow) */
    int loansRepaid;                    /* 0x48, AIFIN.loansRepaid: the loan sums paid back at a money lender (ai_RepayLoan) */
    char pad4c[4];
    int cartSpent;                      /* 0x50, AIFIN.cartSpent: the money paid for new carts (ai_BuyCartWindow) */
    char pad54[4];
    float duelBias;                     /* 0x58, AIFIN.roundFactors[0]: the bias for who starts a dice duel (am.h AGFIN.firstBias) */
    float tavernChance;                 /* 0x5c, AIFIN.roundFactors[1]: the chance of being invited to the dice game; each visit
                                           takes it to * 0.66 - 0.1 (invited) or * 0.66 - 0.02 (not invited, after 11 o'clock) */
} LCFIN;
typedef struct LCDUELBTN {              /* an action button of the dice game window */
    int obj;                            /* 0x000, its window object, -1 while the action is not allowed (ai_DuelAcceptAction) */
    int kind;                           /* 0x004, the action: 2 another die, 3 stop, 4 answer (AGDUEL.kind1) */
    int unused;                         /* 0x008, set to 0, never read */
    char text[0x100];                   /* 0x00c, its caption (0x1880 / 0x1881 with the raise amount, 0x1882) */
} LCDUELBTN;
/* 0x598170, the tavern dice game window: the intro window "locations\wirtshaus\WSIntro" invites the player when
   util_RandomF() is below the character's LCFIN.tavernChance, after 11 o'clock and with a worth of at least 10000
   (0x1876, the buttons rules 0x1877 / play 0x1878 / leave 0x1879; else 0x1875), lowering the chance each visit;
   playing picks the stake (0x187a / 0x187b / 0x187c: 1, 2, 4, ai_DuelStart) and shows it (0x187e when the player
   throws first, else 0x187d), then the game window "locations\wirtshaus\WS": both sides throw (ai_DuelAct,
   ai_DuelNextPhase, shown by ai_DuelDiceAnimation, the player's side in frame 3, the opponent's in frame 2), then
   the player picks an action button (frame 4, the ones ai_DuelAcceptAction allows) and the opponent answers
   (ai_DuelOpponentAction) until the duel ends (phase 5 / 6); ai_DrawDuelEnd shows the end and the player gets
   the total when he won (cm_SendMoveGoodsUnchecked). It leaves as soon as the player's character is dead.
   bld, the tavern building the room listener 0x599af0 passes, is not read */

void ai_TavernDiceWindow(AIBLD *bld)
{
    AICHARREC *pc = (AICHARREC *)ai_Chars[ai_PlayerId];
    LCFIN *fin;
    LCDUELSIDE *mine;
    LCDUELSIDE *other;
    int money;
    int win;
    int leave;
    int ctlRules;
    int ctlPlay;
    int ctlLeave;
    int first;
    int done;
    int refresh;
    int k;
    int i;
    LCDUEL duel = {0};

    leave = 1;
    ctlPlay = -1;
    ctlLeave = -1;
    ctlRules = -1;
    money = gm_ConvertMoney(gm_GetWorth(ai_PlayerId), ai_OwnPlayer);
    fin = (LCFIN *)gm_PlayerRecord((char *)pc);
    win = d2_OpenForm(0, 0, "locations\\wirtshaus\\WSIntro");
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    d2_SetPointerSprite(0, 0);
    io_PrintText((char *)0x1874);
    if (util_RandomF() < fin->tavernChance && ai_Time.hour > 0xb && money >= 10000) {
        d2_SetForm(win, 2);
        io_PrintText((char *)0x1876);
        d2_SetForm(win, 3);
        ctlRules = d2_GetWinObj(win, 3, io_PrintText((char *)0x1877));
        ctlPlay = d2_GetWinObj(win, 3, io_PrintText((char *)0x1878));
        ctlLeave = d2_GetWinObj(win, 3, io_PrintText((char *)0x1879));
        fin->tavernChance = fin->tavernChance * 0.66f - 0.1f;
    } else {
        d2_SetForm(win, 2);
        io_PrintText((char *)0x1875);
        leave = 1;
        if (ai_Time.hour > 0xb)
            fin->tavernChance = fin->tavernChance * 0.66f - 0.02f;
    }
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
            leave = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ctlLeave) {
                sv_PopRequest = 1;
                leave = 1;
            } else if (d2_PointerObj == ctlPlay) {
                sv_PopRequest = 1;
                leave = 0;
            } else if (d2_PointerObj == ctlRules) {
                d2_SetViewEnabled(win, 0);
                ai_TavernRulesWindow();
                d2_SetViewEnabled(win, 1);
            }
        }
    } while (sv_RunFrame((int)ai_TavernDiceWindow, 0x677c7) != 0);
    if (leave != 0 || ((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
        goto close;

    /* the stake */
    d2_SetForm(win, 3);
    io_PrintText("$C");
    ctlRules = d2_GetWinObj(win, 3, io_PrintText((char *)0x187a));
    ctlPlay = d2_GetWinObj(win, 3, io_PrintText((char *)0x187b));
    ctlLeave = d2_GetWinObj(win, 3, io_PrintText((char *)0x187c));
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId != -1) {
            k = 1;
            if (d2_PointerObj == ctlRules)
                k = 1;
            else if (d2_PointerObj == ctlPlay)
                k = 2;
            else if (d2_PointerObj == ctlLeave)
                k = 4;
            ai_DuelStart(pc, &duel, k);
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_TavernDiceWindow, 0x777c7) != 0);
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
        goto close;
    d2_SetForm(win, 2);
    io_PrintText("$C");
    if (duel.side1.ch == pc)
        io_PrintText((char *)0x187e, duel.stake);
    else
        io_PrintText((char *)0x187d, duel.stake);
    d2_SetForm(win, 3);
    io_PrintText("$C");
    io_PrintText((char *)0x187f);
    d2_ClickedId = -1;
    do {
        if (d2_ClickedId == 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_TavernDiceWindow, 0x777c7) != 0);
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
        goto close;
    d2_CloseView(win);

    /* the game */
    {
        LCDUELBTN btn[3] = {{-1, 2, 0, {0}}, {-1, 3, 0, {0}}, {-1, 4, 0, {0}}};

        refresh = 1;
        txt_Format(btn[0].text, 0x1880, duel.raiseAmount);
        txt_Format(btn[1].text, 0x1881, duel.raiseAmount);
        txt_Format(btn[2].text, 0x1882);
        if (pc == duel.side1.ch) {
            mine = &duel.side1;
            other = &duel.side2;
            first = 1;
        } else {
            other = &duel.side1;
            mine = &duel.side2;
            first = 0;
        }
        win = d2_OpenForm(0, 0, "locations\\wirtshaus\\WS");
        d2_SetForm(win, 0);
        sv_FitForm(d2_LastWindow);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 1);
        d2_SetPointerSprite(0, 0);
        io_PrintText((char *)0x1874);
        duel.kind1 = 0;
        duel.kind2 = 0;

        /* side1's first throw */
        if (first)
            ai_DuelDiceAnimation(&duel, mine, 1, win, 3, 0x96, 0);
        else
            ai_DuelDiceAnimation(&duel, other, 1, win, 2, 0x96, 0);
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
            goto close;
        ai_DuelAct(&duel, duel.side1.ch, 1);
        ai_DuelNextPhase(&duel);
        if (first)
            ai_DuelDiceAnimation(&duel, mine, 1, win, 3, 0x14, 1);
        else
            ai_DuelDiceAnimation(&duel, other, 1, win, 2, 0x14, 1);
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
            goto close;

        /* side2's first throw */
        if (first)
            ai_DuelDiceAnimation(&duel, other, 1, win, 2, 0x96, 0);
        else
            ai_DuelDiceAnimation(&duel, mine, 1, win, 3, 0x96, 0);
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
            goto close;
        ai_DuelAct(&duel, duel.side2.ch, 1);
        ai_DuelNextPhase(&duel);
        if (first)
            ai_DuelDiceAnimation(&duel, other, 1, win, 2, 0x14, 1);
        else
            ai_DuelDiceAnimation(&duel, mine, 1, win, 3, 0x14, 1);
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
            goto close;

        if (duel.phase != 5 && duel.phase != 6) {
            if (!first) {
                /* the opponent acts first */
                k = ai_DuelOpponentAction(&duel, other->ch);
                ai_DuelDiceAnimation(&duel, other, k, win, 2, 0x96, 0);
                if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
                    goto close;
                ai_DuelAct(&duel, other->ch, k);
                ai_DuelNextPhase(&duel);
                ai_DuelDiceAnimation(&duel, other, k, win, 2, 0x14, 1);
                if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
                    goto close;
            }
            done = 0;
            d2_ClickedId = -1;
            do {
                if (done) {
                    sv_PopRequest = 1;
                } else if (refresh) {
                    refresh = 0;
                    d2_SetForm(win, 4);
                    d2_RemoveWindowObjects(d2_LastWindow, 1);
                    io_PrintText("$C");
                    for (i = 0; i < 3; i++) {
                        if (ai_DuelAcceptAction(&duel, pc, btn[i].kind))
                            btn[i].obj = d2_GetWinObj(win, 4, io_PrintText(btn[i].text));
                        else
                            btn[i].obj = -1;
                    }
                } else if (d2_ClickedId != -1) {
                    for (i = 0; i < 3; i++) {
                        if (d2_PointerObj == btn[i].obj) {
                            /* the player's action, then the opponent's answer */
                            refresh = 1;
                            d2_SetForm(win, 4);
                            io_PrintText("$C");
                            d2_RemoveWindowObjects(d2_LastWindow, 1);
                            ai_DuelDiceAnimation(&duel, mine, btn[i].kind, win, 3, 0x96, 0);
                            ai_DuelAct(&duel, pc, btn[i].kind);
                            ai_DuelNextPhase(&duel);
                            ai_DuelDiceAnimation(&duel, mine, btn[i].kind, win, 3, 0x14, 1);
                            if (duel.phase != 5 && duel.phase != 6) {
                                k = ai_DuelOpponentAction(&duel, other->ch);
                                ai_DuelDiceAnimation(&duel, other, k, win, 2, 0x96, 0);
                                ai_DuelAct(&duel, other->ch, k);
                                ai_DuelNextPhase(&duel);
                                ai_DuelDiceAnimation(&duel, other, k, win, 2, 0x14, 1);
                                if (duel.phase == 5 || duel.phase == 6)
                                    done = 1;
                            } else {
                                done = 1;
                            }
                            break;
                        }
                    }
                }
            } while (sv_RunFrame((int)ai_TavernDiceWindow, 0x777c7) != 0);
        }
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].health == 0)
            goto close;
        ai_DrawDuelEnd(&duel, win);
        if ((duel.phase == 5 && pc == duel.side1.ch) || (duel.phase == 6 && pc == duel.side2.ch))
            cm_SendMoveGoodsUnchecked(pc->netId, -1, 0, duel.total);
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
        } while (sv_RunFrame((int)ai_TavernDiceWindow, 0x777c7) != 0);
    }
close:
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x598b80, the regulars' table window "locations\Wirtshaus\Stammtisch" of the player's own tavern: per frame the
   characters at the table's four seats (gm_FindChar) are packed into the amount rows; on a change frame 1 is
   redrawn (0x188c with the building, 0x188e while the table is empty) with the rows in two columns
   (ai_AmountRowStatus / ai_AmountRow, the row picture 0x5da), and frame 2 shows 0x1892 when all four sit there;
   the right button leaves */

void ai_StammtischWindow(AIBLD *bld, AIOBJ *table)
{
    LCAMOUNTROW rows[4];
    int win;
    int n;
    int i;
    int changed;
    int gap;
    int off;
    int *seat;
    AICHARREC *c;

    memset(rows, 0, sizeof(rows));
    win = d2_OpenForm(0, 0, "locations\\Wirtshaus\\Stammtisch");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    do {
        n = 0;
        changed = 0;
        seat = table->seats;
        for (i = 0; i < 4; i++, seat++) {
            c = (AICHARREC *)gm_FindChar(*seat);
            if (c != rows[n].rec)
                changed = 1;
            if (c != 0) {
                rows[n].rec = c;
                n++;
            }
        }
        for (i = n; i < 4; i++)
            rows[i].rec = 0;
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            io_PrintText((char *)0x188c, bld);
            if (n == 0) {
                io_PrintText((char *)0x188e);
            } else {
                gap = (d2_LastWindowPtr->w - d2_Pics[0x5da].w * 2) / 3;
                off = (d2_LastWindowPtr->w - d2_Pics[0x5da].w * 2) % 3 / 2;
                for (i = 0; i < n; i++) {
                    rows[i].x = (i % 2 + 1) * gap - 10 + off + (d2_Pics[0x5da].w + 10) * (i % 2);
                    rows[i].pickable = 1;
                    rows[i].y = i / 2 * 0x82 + 100;
                    ai_AmountRowStatus(&rows[i]);
                    ai_AmountRow(rows[i].x, rows[i].y, d2_LastWindow, 0x54, &rows[i], 1);
                }
            }
            d2_SetForm(win, 2);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            if (n == 4)
                io_PrintText((char *)0x1892);
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame((int)ai_StammtischWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
int cm_SendTableRequest(G17TABLEMSG *p); void cm_BeginGroup(const char *s); int cm_EndGroup(const char *s); unsigned char * gm_BuildQuery(int n, ...);
unsigned char * gm_BuildNext(void);


/* 0x598dc0, the regulars' table window "locations\Wirtshaus\Stammtisch" of another player's tavern, with the join
   button: the seats are packed into the amount rows as in ai_StammtischWindow; the mode: 1 the player sits at
   this table (0x1890 and the button), 2 the table is full (0x1892), 3 a seat is free (0x188f and the join
   button), 4 the player sits at a table of another tavern (kind 0x37, 0x1891). A click on the join button sends
   the 'join' request (cm_SendTableRequest in the group "stammtisch join H-Plr"), waits for its slot and shows
   0x189e (taken) or 0x189f with the building. The frames are pumped with ai_StammtischWindow's callback */

void ai_StammtischJoinWindow(AIBLD *bld, AIOBJ *table)
{
    int mode;
    int ctl;
    int win;
    int n;
    int changed;
    int *seat;
    G17TABLEMSG msg;
    LCAMOUNTROW rows[4];
    int i;
    int gap;
    int off;
    int h;
    AICHARREC *c;
    AIBLD *b;
    AIOBJ *t;

    memset(rows, 0, sizeof(rows));
    ctl = -1;
    win = d2_OpenForm(0, 0, "locations\\Wirtshaus\\Stammtisch");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    do {
        mode = 0;
        n = 0;
        changed = 0;
        seat = table->seats;
        for (i = 0; i < 4; i++, seat++) {
            c = (AICHARREC *)gm_FindChar(*seat);
            if (c != rows[n].rec)
                changed = 1;
            if (c != 0) {
                rows[n].rec = c;
                n++;
                if (c->netId == ((AICHARREC *)ai_Chars)[ai_PlayerId].netId)
                    mode = 1;
            }
        }
        if (n == 4) {
            if (mode == 0)
                mode = 2;
        } else if (n < 4) {
            if (mode == 0) {
                mode = 3;
                for (b = (AIBLD *)gm_BuildQuery(1, 0, 0x37); b != 0; b = (AIBLD *)gm_BuildNext()) {
                    t = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12d);
                    if (t != 0) {
                        for (i = 0; i < 4; i++) {
                            if (t->seats[i] == ((AICHARREC *)ai_Chars)[ai_PlayerId].netId) {
                                mode = 4;
                                break;
                            }
                        }
                    }
                }
            }
            for (i = n; i < 4; i++)
                rows[i].rec = 0;
        }
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            io_PrintText((char *)0x188c, bld);
            if (n == 0) {
                io_PrintText((char *)0x188e);
            } else {
                gap = (d2_LastWindowPtr->w - d2_Pics[0x5da].w * 2) / 3;
                off = (d2_LastWindowPtr->w - d2_Pics[0x5da].w * 2) % 3 / 2;
                for (i = 0; i < n; i++) {
                    rows[i].x = (i % 2 + 1) * gap - 10 + off + (d2_Pics[0x5da].w + 10) * (i % 2);
                    rows[i].pickable = 1;
                    rows[i].y = i / 2 * 0x82 + 100;
                    ai_AmountRowStatus(&rows[i]);
                    ai_AmountRow(rows[i].x, rows[i].y, d2_LastWindow, 0x54, &rows[i], 1);
                }
            }
            d2_SetForm(win, 2);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            io_PrintText("$C");
            ctl = -1;
            if (mode == 2)
                io_PrintText((char *)0x1892);
            else if (mode == 4)
                io_PrintText((char *)0x1891);
            else if (mode == 3)
                ctl = d2_GetWinObj(win, 2, io_PrintText((char *)0x188f));
            else if (mode == 1)
                ctl = d2_GetWinObj(win, 2, io_PrintText((char *)0x1890));
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1 && d2_PointerObj == ctl && mode == 3) {
            char text[0x200] = "";

            if (d2_ClickedId == 0) {
                msg.op = 0x6a6f696e;    /* 'join' */
                msg.charId = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                msg.bldId = bld->id;
                cm_BeginGroup("stammtisch join H-Plr");
                h = cm_SendTableRequest(&msg);
                cm_EndGroup("stammtisch join H-Plr");
                i = cm_GetSlotState(h);
                while (i == 0) {
                    sv_RunFrame(0, 0x20007);
                    i = cm_GetSlotState(h);
                }
                if (i == 1)
                    txt_Format(text, 0x189e, bld);
                else
                    txt_Format(text, 0x189f, bld);
            }
            if (text[0] != 0)
                sv_MessageBox(text, 0);
        }
    } while (sv_RunFrame((int)ai_StammtischWindow, 0x677c7) != 0);
    d2_CloseView(win);
}

/* 0x599240, opens the regulars' table (Stammtisch) of a tavern room: it finds the table object (kind 0x12d) in the
   room's object list and the table event (kind 0x2a) of the building; when the building belongs to the player (the
   player's AICHAR.id equals its owner) the plain table window opens, else the joinable one; the camera follows in
   every case */

void ai_OpenStammtisch(AIBLD *bld, AIOBJ *room)
{
    int rec;
    int b;

    rec = gm_Query(room->list, 1, 0, 0x12d);
    if (rec != 0) {
        b = he_FindEvent(2, 0, 0x2a, 3, bld->id);
        if (b != 0) {
            if (((AICHARREC *)ai_Chars)[ai_PlayerId].id == bld->owner) {
                ai_StammtischWindow(bld, (AIOBJ *)rec);
                sv_CameraFollow();
                return;
            }
            ai_StammtischJoinWindow(bld, (AIOBJ *)rec);
        }
    }
    sv_CameraFollow();
}

int gv_EvidencePrice(); void cm_SendScriptMessage(void *p); void cm_SendProofExecuted(int a, int b);

/* 0x5992c0, the dark-corner menu of the player's own tavern, "locations\Wirtshaus\DUNKLEECKE": per frame the
   corner's three evidence offers (corner->recs, gv_EvidencePrice with the building owner as the seller) fill the
   amount rows with the culprits; on a change frame 1 shows 0x18a3 and per offer its row and its text (0x18a5 with
   the law's name 0x13fd + law * 5 and the price, 0x18a6 for none), and frame 2 shows 0x18a8 while an order 0x74
   of this building runs (he_FindEvent), else the order button 0x18a7 once corner->cornerYear is past. A click on
   it sends the order message 0x74 (the player, the building, the clock, the room); the right button leaves
   @deep frame sweep s21: 73 -> 47 ins: price / who (gv_EvidencePrice's out-parameters) locals of the
   evidence loop's body; msg / text as block locals = control. Left: code 55, regs 36. prior: Reconstructed
   whole from the original (the three gv_EvidencePrice offers with cur/vals/law, the rows LCAMOUNTROW, the
   y-controlled draw loop to 0x181, the order-0x74 event check via AIJOB.id2, AIOBJ.cornerYear vs
   ai_Time.year, the AIMSG 0x74 with D3_ZERO, time copy, arg[1]/arg[2]). Left: frame layout and allocation
   only: the original lays the arrays cur -0x18 / vals -0x28 / law -0x40 and price -0x2c / who -0x30 /
   D3_ZERO's _zn -0x34 _zp -0x1c, ours cur -0x20 / vals -0x34 / price -0x10 / who -0x14 / _zn -0x28 _zp
   -0x24 (the indexed [ebp+edi-0x18] vs [ebp+edi-0x20] lines are the 'code' hunks); the original keeps 0 in
   ebx through the head (push ebx, vals=ebx), ours in edi after rep stosd; ours loads cur[i] before the
   gv_EvidencePrice test ('mov ecx,[ebp+edi-0x20]'), the original in each branch; 'mov [ebp-4],0' vs ours
   'mov [ebp-4],esi'. Tried: five declaration orders of the locals (no change: VC6 does not lay them by
   declaration), ctl before cur, the compare's operand order */

void ai_DarkCornerMenu(AIBLD *bld, AIOBJ *room, AIOBJ *corner)
{
    int changed;
    int ctl;
    int win;
    int cur[3];
    int vals[3];
    unsigned int law[3];
    AIMSG msg;
    LCAMOUNTROW rows[3];
    char text[0x200];
    int i;
    int y;
    int half;
    int x;
    int found;
    AIJOB *ev;

    ctl = -1;
    cur[0] = -1;
    cur[1] = -1;
    cur[2] = -1;
    memset(rows, 0, sizeof(rows));
    vals[0] = 0;
    vals[1] = 0;
    vals[2] = 0;
    win = d2_OpenForm(0, 0, "locations\\Wirtshaus\\DUNKLEECKE");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    memset(rows, 0, sizeof(rows));
    changed = 1;
    do {
        for (i = 0; i < 3; i++) {
            int price;
            unsigned short who;

            if (gv_EvidencePrice(corner->recs[i], ai_Chars[bld->owner], &price, &who, &law[i]) != 0) {
                if (corner->recs[i] != cur[i])
                    changed = 1;
                cur[i] = corner->recs[i];
                rows[i].rec = (AICHARREC *)ai_Chars[who];
                vals[i] = price;
            } else if (cur[i] != -1) {
                changed = 1;
                rows[i].rec = 0;
                cur[i] = -1;
                vals[i] = 0;
            }
        }
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            io_PrintText((char *)0x18a3);
            half = d2_LastWindowPtr->w / 2;
            x = half - d2_Pics[0x5da].w - 0x28;
            for (i = 0, y = 0x46; y < 0x46 + 3 * 0x69; i++, y += 0x69) {
                rows[i].x = x;
                rows[i].y = y - 0x1e;
                rows[i].pickable = rows[i].rec != 0;
                ai_AmountRowStatus(&rows[i]);
                ai_AmountRow(rows[i].x, rows[i].y, d2_LastWindow, 0x54, &rows[i], 1);
                if (cur[i] == -1)
                    txt_Format(text, 0x18a6);
                else
                    txt_Format(text, 0x18a5, law[i] * 5 + 0x13fd, vals[i]);
                ai_LeftAlignedText(text, half, y, 0xf0, 0x54);
            }
            found = 0;
            changed = 0;
            ctl = -1;
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x74); ev != 0; ev = (AIJOB *)he_NextEvent()) {
                if (ev->id2 == bld->id) {
                    found = 1;
                    break;
                }
            }
            d2_SetForm(win, 2);
            io_PrintText("$C");
            if (found)
                io_PrintText((char *)0x18a8);
            else if (corner->cornerYear < ai_Time.year)
                ctl = d2_GetWinObj(win, 2, io_PrintText((char *)0x18a7));
        }
        if (d2_ClickedId != -1 && d2_PointerObj == ctl) {
            D3_ZERO(&msg, sizeof(msg));
            msg.type = 0x74;
            msg.flags = 2;
            msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
            msg.fromId = bld->id;
            msg.time = ai_Time;
            msg.arg[1] = bld->id;
            msg.arg[2] = room->objId;
            cm_SendLongMessage((char *)&msg);
            sv_PopRequest = 1;
        } else if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_DarkCornerMenu, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x5996b0, the dark-corner buying screen of another player's tavern, "locations\Wirtshaus\DUNKLEECKE": the
   corner's three evidence offers drawn as in ai_DarkCornerMenu (the amount rows, 0x18a5 / 0x18a6); a click on an
   offer's row buys it while it is still among the corner's offers and the player can pay (sv_CheckGoodAmount):
   in the group "buy dunkle ecke" the script message 0x53 {'buy ', the player, the building, the offer, the
   price} (cm_SendScriptMessage), the proof message (cm_SendProofExecuted) and the price paid to the owner
   (cm_SendMoveGoodsUnchecked). The frames are pumped with ai_DarkCornerMenu's callback
   @deep size queue s16: the offer row stores as rows[i].rec, cur[i], vals[i] (all 6 orders): 119 -> 94
   ins, raw 1016 -> 1020 (padded 1024 vs 1008). climb.py round 2 further: rows[i].x after .pickable and
   cur[sel] read at each use instead of id (85 ins), not kept (an id local reads better and the size
   stays). prior: Reconstructed whole from the original (as ai_DarkCornerMenu: the three gv_EvidencePrice
   offers, the rows, the y-controlled draw loop; the click finds the row by its ctl[0], checks the offer is
   still in corner->recs and sv_CheckGoodAmount, then the group 'buy dunkle ecke' with the script message
   {'buy ', player, building, offer, price}, cm_SendProofExecuted and the payment to the owner; the 'nicht
   mehr zum verkauf' box behind the original's sel == 3 test). Left: allocation and frame layout only: the
   original keeps 0 in ebx through the head (push ebx, vals = ebx), ours in edi after rep stosd; ours
   hoists the cur[i] load above the gv_EvidencePrice test ('mov ecx,[esp+esi+0x28]'), the original loads it
   in each branch; the original keeps x in ebp and half in [esp+0x10], ours the reverse; sel in ebp
   ('[esp+ebp*4+0x14]') vs ours edi with 'lea edx,[eax*4]'; ai_OwnPlayer via cl vs al; the slots o... */

void ai_DarkCornerBuy(AIBLD *bld, AIOBJ *corner)
{
    int changed;
    int cur[3];
    int win;
    int vals[3];
    int price;
    unsigned short who;
    int req[5];
    unsigned int law[3];
    LCAMOUNTROW rows[3];
    char text[0x200];
    int i;
    int y;
    int half;
    int x;
    int sel;
    int id;
    int cost;

    cur[0] = -1;
    cur[1] = -1;
    cur[2] = -1;
    memset(rows, 0, sizeof(rows));
    vals[0] = 0;
    vals[1] = 0;
    vals[2] = 0;
    win = d2_OpenForm(0, 0, "locations\\Wirtshaus\\DUNKLEECKE");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    memset(rows, 0, sizeof(rows));
    changed = 1;
    do {
        for (i = 0; i < 3; i++) {
            if (gv_EvidencePrice(corner->recs[i], ai_Chars[bld->owner], &price, &who, &law[i]) != 0) {
                if (cur[i] != corner->recs[i])
                    changed = 1;
                rows[i].rec = (AICHARREC *)ai_Chars[who];
                cur[i] = corner->recs[i];
                vals[i] = price;
            } else if (cur[i] != -1) {
                changed = 1;
                rows[i].rec = 0;
                cur[i] = -1;
                vals[i] = 0;
            }
        }
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            io_PrintText((char *)0x18a3);
            half = d2_LastWindowPtr->w / 2;
            x = half - d2_Pics[0x5da].w - 0x28;
            for (i = 0, y = 0x46; y < 0x46 + 3 * 0x69; i++, y += 0x69) {
                rows[i].x = x;
                rows[i].y = y - 0x1e;
                rows[i].pickable = rows[i].rec != 0;
                ai_AmountRowStatus(&rows[i]);
                ai_AmountRow(rows[i].x, rows[i].y, d2_LastWindow, 0x54, &rows[i], 1);
                if (cur[i] == -1)
                    txt_Format(text, 0x18a6);
                else
                    txt_Format(text, 0x18a5, law[i] * 5 + 0x13fd, vals[i]);
                ai_LeftAlignedText(text, half, y, 0xf0, 0x54);
            }
            changed = 0;
        }
        if (d2_ClickedId != -1) {
            sel = -1;
            for (i = 0; i < 3; i++) {
                if (d2_PointerObj == rows[i].ctl[0]) {
                    sel = i;
                    break;
                }
            }
            if (sel != -1) {
                id = cur[sel];
                for (i = 0; i < 3; i++) {
                    if (corner->recs[i] == id) {
                        cost = vals[sel];
                        if (sv_CheckGoodAmount(ai_Chars[ai_PlayerId], cost, ai_OwnPlayer) != 0) {
                            req[0] = 0x62757920;    /* 'buy ' */
                            req[1] = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                            req[2] = bld->id;
                            req[3] = id;
                            req[4] = cost;
                            cm_BeginGroup("buy dunkle ecke");
                            cm_SendScriptMessage(req);
                            cm_SendProofExecuted(id, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId);
                            cm_SendMoveGoodsUnchecked(((AICHARREC *)ai_Chars)[bld->owner].netId,
                                                      ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, 0, cost);
                            cm_EndGroup("buy dunkle ecke");
                        }
                        break;
                    }
                }
                if (sel == 3)   /* never true (sel < 3 here); the original tests sel, not i */
                    sv_MessageBox("$Z$NDiese Information steht nicht mehr zum verkauf!", 0);
            }
        } else if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)ai_DarkCornerMenu, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x599aa0, opens the dark corner of a table (79 B): the table's record (0x4ffe90 (room->list, 1, 0, 300));
   when the table belongs to the player, the dark-corner menu opens with the record as the offer source
   (0x5992c0 (building, room, record)), else the buying screen (0x5996b0 (building, record)); then the camera
   follows */

void ai_DarkCornerTable(AIBLD *bld, AIOBJ *room)
{
    AIOBJ *rec;

    rec = (AIOBJ *)gm_Query((void *)room->list, 1, 0, 300);
    if (rec != 0) {
        if (bld->owner == ai_PlayerId) {
            ai_DarkCornerMenu(bld, room, rec);
            sv_CameraFollow();
            return;
        }
        ai_DarkCornerBuy(bld, rec);
    }
    sv_CameraFollow();
}

int d3t_PlaceAtObject(); void d2_SetBackgroundTexture(); 
struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask); int gm_GetGoodsValue(); int gm_SumListAmounts(void *list, unsigned char p);
int gm_PlayerFactor(int a, unsigned char k); int gm_GetCreditOffers(); void d2_EnableViewObjects();
#define gm_SumListAmountsL ((int (*)(void *))gm_SumListAmounts)  /* 0x509e30: @original ai_WineCellarWindow passes the list alone (one push, no player slot) */
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2); void he_FreeEvent(); void d2_RadioClear(int g); int d2_CreateRadioGroup(int n, ...);
void ai_StorageWindow(); void ai_MasterLetterWindow(); int ai_OpenTransport();
void ai_BardWindow(void); void ai_NoticeBoardWindow(void);
void cm_BeginGroup(const char *s); void cm_SendScriptMessage(void *p); void cm_SendProofExecuted(int a, int b);

/* 0x599af0, the tavern room's contact listener: it releases the name plates, then per frame (sv_RunFrame, this,
   mask 0x67fff) runs ai_MarketContact and registers the plates contact_WUERFELSPIEL, contact_STAMMTISCH and
   ob_DUNKLE_ECKE; a clicked plate opens the dice game (ai_TavernDiceWindow), the regulars' table
   (ai_OpenStammtisch) or the dark corner (ai_DarkCornerTable). With ai_DebugHotkeys set, the key 0x12 sends the
   dark-corner order 0x74 at once (as ai_DarkCornerMenu's, from no building: fromId -1) and the key 0x17 opens the
   dark corner */

void ai_TavernContact(AIBLD *bld, AIOBJ *room)
{
    int dice;
    int table;
    int corner;
    AIMSG msg;

    sv_ReleaseNamePlates();
    while (sv_RunFrame((int)ai_TavernContact, 0x67fff) != 0) {
        ai_MarketContact(bld);
        dice = sv_RegisterNamePlate("contact_WUERFELSPIEL", ai_ContactTextDiceGame, 0x16);
        table = sv_RegisterNamePlate("contact_STAMMTISCH", ai_ContactTextStammtisch, 0x16);
        corner = sv_RegisterNamePlate("ob_DUNKLE_ECKE", ai_ContactTextDarkCorner, 0x16);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == dice)
                ai_TavernDiceWindow(bld);
            else if (ai_ClickedObject == table)
                ai_OpenStammtisch(bld, room);
            else if (ai_ClickedObject == corner)
                ai_DarkCornerTable(bld, room);
        }
        if (ai_DebugHotkeys != 0) {
            if (kb_Last == 0x12) {
                D3_ZERO(&msg, sizeof(msg));
                msg.type = 0x74;
                msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                msg.fromId = -1;
                msg.flags = 2;
                msg.time = ai_Time;
                msg.arg[1] = bld->id;
                msg.arg[2] = room->objId;
                cm_SendLongMessage((char *)&msg);
            } else if (kb_Last == 0x17) {
                ai_DarkCornerTable(bld, room);
            }
        }
    }
}
/* 0x599cb0, the contact listener of the brewery: it releases the name plates and pumps the frame (sv_RunFrame,
   this, mask 0x67fff) while the screen stays on top and, every frame, registers the contact plates ai_BuildingPerms
   allows: with 0x200 the production plate PRODUKTION_BRAUEN (text ai_ContactTextProductionBrewing, look 0x2d); with
   0x400 ob_MEISTERBRIEF, ob_PERSONALBUCH, contact_TRANSPORT and contact_LAGER. ai_MarketContact runs every frame; a
   clicked plate opens its window (ai_PersonnelBook, the store window ai_StorageWindow on the building's object list,
   ai_MasterLetterWindow, ai_ProductionWindow, ai_OpenTransport) */

void ai_BreweryContact(AIBLD *bld, int room)
{
    int brauen;
    int personal;
    int lager;
    int meister;
    int transport;

    brauen = 0;
    lager = 0;
    transport = 0;
    meister = 0;
    personal = 0;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_BreweryContact, 0x67fff) != 0) {
        if ((ai_BuildingPerms & 0x200) != 0)
            brauen = sv_RegisterNamePlate("contact_PRODUKTION_BRAUEN", ai_ContactTextProductionBrewing, 0x2d);
        if ((ai_BuildingPerms & 0x400) != 0) {
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
        }
        ai_MarketContact(bld);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == personal)
                ai_PersonnelBook(bld, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, bld->objList);
            else if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)bld);
            else if (ai_ClickedObject == brauen)
                ai_ProductionWindow((AIBLD *)bld);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
        }
    }
}
/* 0x599e00, draws one goods cell at (x, y) of the window win: the cell frame 0x525, the goods' picture 0xdf + goods
   (made interactive), the price plate 0x746 and on it the goods' price for the player slot (gm_GoodsPrice,
   gm_FormatAmount) as a right-aligned text of width 0x24 in the font 0x54 (in the last window); returns the
   goods picture's object */

int ai_DrawGoodsCell(int x, int y, int win, int goods, int slot)
{
    char text[0x40];
    int pic;
    int obj;
    float price;

    d2_WinOpenPic(x, y, 0x525, win);
    pic = d2_WinOpenPic(x + 2, y + 2, goods + 0xdf, win);
    d2_WinOpenPic(x + 3, y + 0x35, 0x746, win);
    d2_Objs[pic].interactive = 1;
    price = gm_GoodsPrice(goods, slot);
    gm_FormatAmount((int)price, slot, text);
    obj = d2_CreateWinText(x + 7, y + 0x3a, text, d2_LastWindow);
    d2_Objs[obj].w = 0x24;
    d2_Objs[obj].font = 0x54;
    d2_Objs[obj].rightAligned = 1;
    return pic;
}
/* 0x599ef0, the "Handel\Handel_Stadtinfo" city window (976 B): for a city record (kind 'G', 0x47: its player
   slot at +0x65) it places the camera at "dummy_Kartentisch_RECHTS_UNTEN" and, while the frame pump 0x5309a0
   (this, 0x677c7) runs, shows the window once: the paddings 0x18 / 0x18 / 0x1c / 0xb, the texts 0x1a11 and
   0x1a14 (the city's name and its stock block's demand index), the coat of arms "Wappen128_ns_Stadt_%s" as the
   background, then the goods of the city's two production groups (0x1db, 0x1dc) the city holds, each a goods
   cell (ai_DrawGoodsCell) in a 4-column grid under the texts 0x1a12 / 0x1a13; the background fades in over
   650 ms up to the alpha 0.35; a right click closes it, and the camera returns to "ob_KARTENTISCH" */

void ai_CityInfoWindow(AIBLD *city, int unused)
{
    char text[0x104];
    int table;
    int corner;
    int win;
    int shown;
    unsigned long start = 0;
    D2WIN *bg = 0;
    AIOBJ *group1;
    AIOBJ *group2;
    AIOBJ *item;
    GMSTOCK *stock;
    int n;
    double alpha;

    if (city->type != 'G')
        return;
    corner = d3_FindObject(0, 0x100, 0, "dummy_Kartentisch_RECHTS_UNTEN");
    if (corner != 0)
        d3t_PlaceAtObject(corner, ai_CameraStep);
    table = d3_FindObject(0, 0x40, 0, "ob_KARTENTISCH");
    win = -1;
    shown = -1;
    while (sv_RunFrame(ai_CityInfoWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (shown != 1) {
            if (win != -1)
                d2_CloseView(win);
            win = d2_OpenForm(0, 0, "Handel\\Handel_Stadtinfo");
            d2_CentreViewWindows(win);
            d2_SetForm(win, 0);
            d2_Wins[d2_LastWindow].padl = 0x18;
            d2_Wins[d2_LastWindow].padt = 0x18;
            d2_Wins[d2_LastWindow].padr = 0x1c;
            d2_Wins[d2_LastWindow].padb = 0xb;
            d2_SetForm(win, 1);
            io_PrintText(0x1a11);
            d2_SetForm(win, 5);
            io_PrintText(0x1a14, ai_PlayerSlots[city->playerSlot], gm_StockBlocks[city->playerSlot].demandIndex);
            bg = &d2_Wins[d2_Views[win].wins[2]];
            sprintf(text, "Wappen128_ns_Stadt_%s", ai_PlayerSlots[city->playerSlot]);
            d2_SetBackgroundTexture(text, bg->id);
            bg->alpha = 0;
            start = d3_Time;
            group1 = (AIOBJ *)gm_Query((void *)city->objList, 2, 0, 0x1db, 6);
            if (group1 == 0)
                return;
            group2 = (AIOBJ *)gm_Query((void *)city->objList, 2, 0, 0x1dc, 6);
            if (group2 == 0)
                return;
            d2_SetForm(win, 3);
            io_PrintText(0x1a12);
            n = 0;
            for (item = (AIOBJ *)gm_Query((void *)group1->list, 1, 5); item != 0; item = (AIOBJ *)gm_QueryNext()) {
                stock = (GMSTOCK *)gm_FindGoods(city->playerSlot, item->id);
                if (stock != 0 && stock->amount != 0) {
                    ai_DrawGoodsCell(n % 4 * 0x3c + 0xe, n / 4 * 0x50 + 0x1e, d2_LastWindow, item->id, city->playerSlot);
                    n++;
                }
            }
            n = 0;
            d2_SetForm(win, 4);
            io_PrintText(0x1a13);
            for (item = (AIOBJ *)gm_Query((void *)group2->list, 1, 5); item != 0; item = (AIOBJ *)gm_QueryNext()) {
                stock = (GMSTOCK *)gm_FindGoods(city->playerSlot, item->id);
                if (stock != 0 && stock->amount != 0) {
                    ai_DrawGoodsCell(n % 4 * 0x3c + 0xe, n / 4 * 0x50 + 0x1e, d2_LastWindow, item->id, city->playerSlot);
                    n++;
                }
            }
            shown = 1;
        }
        alpha = (d3_Time - start) * 0.0015384615384615385;
        bg->alpha = (float)(alpha < 0.35 ? alpha : 0.35);
    }
    if (win != -1)
        d2_CloseView(win);
    if (table != 0)
        d3t_PlaceAtObject(table, ai_CameraStep);
}

int d3t_PlayAnim(); void d2_SetBackgroundTexture();
char d3if_ShowTree(struct D3OBJECT *obj, char show); int ls_GetLanguage(); struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask);
unsigned char * gm_BuildNext(void); void ai_StorageWindow(); void ai_MasterLetterWindow(); int ai_OpenTransport();
int ai_TransportWindow(); 

/* 0x59a2c0, creates the city point object of a town (256 B): the name "dummy_<CITY>" (the uppercase city via
   _strupr, sprintf 0x5de652) looked up by 0x449690 (0, 0x140, 0); its position (obj->pos) copied by 0x458a20
   into the vector v and the object "sp_STADTPUNKT_K" created there (0x446e10, the origin 0x5ebba4); kind 4,
   mask 1, objectFlags |= 4, stateFlags = (b & 0xf7) | 4; 0x453280 and the model
   "sonstiges\STADTPUNKT_ANM_K.baf" (0x411960, flag 1). Returns the object (0 when the dummy is missing) */
D3OBJECT *ai_CreateCityPoint(char *city)
{
    char upper[0x100];
    char name[0x100];
    D3VECTOR4 v = {0, 0, 0, 0};
    D3OBJECT *obj;

    strcpy(upper, city);
    strupr(upper);
    sprintf(name, "dummy_%s", upper);
    obj = d3_FindObject(0, 0x140, 0, name);
    if (obj == 0)
        return 0;
    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&v);
    obj = d3_CreateObject(0, (D3VECTOR *)&v, (D3VECTOR *)&d3_Origin, "sp_STADTPUNKT_K");
    obj->kind = 4;
    obj->mask = 1;
    obj->objectFlags |= 4;
    obj->stateFlags = (obj->stateFlags & 0xf7) | 4;
    d3_RelightObject(obj);
    d3t_PlayAnim(obj, "sonstiges\\STADTPUNKT_ANM_K.baf", 1);
    return obj;
}
/* 0x59a3c0, the city map screen of the market hall's map table (416 B): creates the city point objects of the
   other players (the player slots 1 .. 3, each slot's name at +0, 0x59a2c0) and names them "stadt_%s" over
   the object name (registered by 0x52e1c0); the frame pump 0x5309a0 (0x59a3c0, 0x677d7) refreshes the plates,
   asks 0x448630 (dm_L.x, dm_L.y, 0x60, 1) and opens the info window 0x599ef0 (the building, 1) of the city in
   the clicked slot (0x501190 (1, 0, 0x47), its AIBLD.playerSlot); on exit the three objects are freed
   (0x447190) between 0x52e190 / 0x52e170, then 0x52d7a0 */

void ai_MapTableScreen(void)
{
    D3OBJECT *objs[4];
    int i;
    AIBLD *rec;
    D3OBJECT *r;

    objs[0] = 0;
    objs[1] = 0;
    objs[2] = 0;
    objs[3] = 0;
    for (i = 0; i < 3; i++) {
        objs[i] = ai_CreateCityPoint(ai_PlayerSlots[i + 1]);
        sprintf(objs[i]->name, "stadt_%s", ai_PlayerSlots[i + 1]);
        sv_RegisterNamePlate(objs[i]->name, ai_PlayerSlots[i + 1], 0);
    }
    d2_SetPointerSprite(0, 0);
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_MapTableScreen, 0x677d7) != 0) {
        ai_ClickedObject = 0;
        for (i = 0; i < 3; i++)
            sv_RegisterNamePlate(objs[i]->name, ai_PlayerSlots[i + 1], 0);
        r = d3if_Pick((float)dm_L.x, (float)dm_L.y, 0x60, 1);
        ai_PickedObject = (char *)r;
        if (dm_L.lup == 0 || r == 0) {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
        } else {
            ai_ClickedObject = (int)r;
            for (i = 0; i < 3; i++) {
                if (ai_ClickedObject == (int)objs[i] && (rec = (AIBLD *)gm_BuildQuery(1, 0, 0x47)) != 0) {
                    do {
                        if (rec->playerSlot == i + 1) {
                            ai_CityInfoWindow(rec, 1);
                            break;
                        }
                        rec = (AIBLD *)gm_BuildNext();
                    } while (rec != 0);
                }
            }
        }
    }
    sv_ReleaseNamePlates();
    for (i = 0; i < 3; i++)
        if (objs[i] != 0)
            d3if_Delete(objs[i]);
    sv_ClearNamePlates();
    sv_CameraFollow();
}
/* 0x59a560, the fair / market hall contact listener (432 B): the two table objects "ob_KARTENTISCH_germ" and
   "ob_KARTENTISCH" (0x449690, 0xc0) are hidden (0x447300, 0), then the German table is shown when the language
   is 0 (0x5190a0) and it exists, else the plain one; the pump 0x5309a0 (0x59a560, 0x67fff), the contact list
   0x58f510 (bld); with ai_BuildingPerms & 0x200 the four plates (transport 0x15, storage 0xe, the master
   letter and the map table, class 0x16); the clicked contact (ai_ClickedObject) opens the transport 0x5af1a0
   (bld), the storage 0x5b26b0 (bld, bld->objList), the master letter 0x5c3200 (bld) or the city map screen
   0x59a3c0 (the table object's mask off / on) */

void ai_MapTableContact(AIBLD *bld, int room)
{
    D3OBJECT *obj;
    D3OBJECT *lager = 0;
    D3OBJECT *transport = 0;
    D3OBJECT *meister = 0;
    D3OBJECT *kartentisch = 0;

    obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH_germ");
    if (obj != 0)
        d3if_ShowTree(obj, 0);
    obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH");
    if (obj != 0)
        d3if_ShowTree(obj, 0);
    if (ls_GetLanguage() == 0) {
        obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH_germ");
        if (obj == 0)
            obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH");
    } else
        obj = d3_FindObject(0, 0xc0, 0, "ob_KARTENTISCH");
    if (obj != 0)
        d3if_ShowTree(obj, 1);
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_MapTableContact, 0x67fff) != 0) {
        ai_MarketContact(bld);
        if ((ai_BuildingPerms & 0x200) != 0) {
            transport = (D3OBJECT *)sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            lager = (D3OBJECT *)sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            meister = (D3OBJECT *)sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            kartentisch = (D3OBJECT *)sv_RegisterNamePlate("ob_KARTENTISCH", ai_ContactTextMapTable, 0x16);
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == (int)transport)
                ai_OpenTransport((int)bld);
            if (ai_ClickedObject == (int)lager)
                ai_StorageWindow((int)bld, bld->objList);
            else if (ai_ClickedObject == (int)meister)
                ai_MasterLetterWindow((int)bld);
            else if (ai_ClickedObject == (int)kartentisch) {
                kartentisch->mask = 0;
                ai_MapTableScreen();
                kartentisch->mask = 1;
            }
        }
    }
}
/* 0x59a710, the window of the market stand `kind` of the building bld (the market, ai_MarketBuilding): the
   selection (sv_SelectedObject / sv_SelectedModel) cleared, the info panel closed (sv_ClosePanel /
   sv_ClearSelection, ai_PermsDirty = 1, sv_PanelForm hidden); the stand record (gm_Query, 1, 0, kind) of
   the building's object list shown (sv_PanelShown); with a record the tutorial step 0x17 posts and the
   transport window (mode 9) runs on the record until it is gone (ai_PermsDirty = 1 each round), then the
   steps 0x18 and 0x17 (result 1), the record cleared and the panel form shown again */

void ai_MarketStandWindow(AIBLD *bld, int kind)
{
    sv_SelectedObject = 0;
    sv_SelectedModel = 0;
    sv_ClosePanel();
    sv_ClearSelection();
    ai_PermsDirty = 1;
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 0);
    sv_PanelShown = (unsigned short *)gm_Query((void *)ai_MarketBuilding->objList, 1, 0, kind);
    if (sv_PanelShown != 0) {
        ai_CampaignStep(0x17, (int)sv_PanelShown, 0, kind);
        while (sv_PanelShown != 0) {
            ai_PermsDirty = 1;
            ai_TransportWindow((int)bld, 9);
        }
    }
    ai_CampaignStep(0x18, (int)sv_PanelShown, 0, kind);
    ai_CampaignStep(0x17, (int)sv_PanelShown, 1, kind);
    sv_PanelShown = 0;
    if (sv_PanelForm != -1)
        d2_SetViewEnabled(sv_PanelForm, 1);
}
#define MARKET_FLECK_NAME (0x57 * 0xe - 2)  /* the name of building kind 0x57 (ai_BuildingNames[kind * 0xe - 2] as
                                               f3init.c reads a kind's name); the kind is computed from the slot 0x4c0, a guess */
/* 0x59a800, the market stand listener (736 B): registers the name plates of the market's stands (the object kind
   names 0x92 .. 0x97 and 0x2db of txt_ObjectNames), the notice board (kind 0x98) and the stand ob_TRIBUENE and, in
   Nuernberg (the own player slot's city name), the three "contact_MARKTFLECKEN_0x" plates; a clicked stand opens
   ai_MarketStandWindow(market, its kind), the notice board 0x5b5690, the stand 0x5b5a20, a market fleck plate the
   window of stand 0x95 for the building its object's parent stands for */

void ai_MarketStandListener(AIBLD *market)
{
    char *fleck1 = 0;
    char *fleck2 = 0;
    char *fleck3 = 0;
    char *c;
    AIBLD *bld;

    sv_RegisterNamePlate("ob_MARKTSTAND_TISCHLER_SCHMIEDE", (char *)txt_ObjectNames[0x92][0], 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_STEINMETZ_WIRT", (char *)txt_ObjectNames[0x93][0], 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_PARFUM_KRAEUTER", (char *)txt_ObjectNames[0x94][0], 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_ROHSTOFFE", (char *)txt_ObjectNames[0x95][0], 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_IMPORTEUR", (char *)txt_ObjectNames[0x96][0], 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_KIRCHE", (char *)txt_ObjectNames[0x97][0], 0x10);
    sv_RegisterNamePlate("ob_MARKTSTAND_OKKULT", (char *)txt_ObjectNames[0x2db][0], 0x10);
    sv_RegisterNamePlate("ob_SCHWARZES_BRETT", (char *)txt_ObjectNames[0x98][0], 0x10);
    sv_RegisterNamePlate("ob_TRIBUENE", txt_Tribune, 0x10);
    if (strcmp(ai_PlayerSlots[ai_OwnPlayer], "Nuernberg") == 0) {
        fleck1 = (char *)sv_RegisterNamePlate("contact_MARKTFLECKEN_01", (char *)ai_BuildingNames[MARKET_FLECK_NAME], 0x10);
        fleck2 = (char *)sv_RegisterNamePlate("contact_MARKTFLECKEN_02", (char *)ai_BuildingNames[MARKET_FLECK_NAME], 0x10);
        fleck3 = (char *)sv_RegisterNamePlate("contact_MARKTFLECKEN_03", (char *)ai_BuildingNames[MARKET_FLECK_NAME], 0x10);
    }
    c = (char *)ai_ClickedObject;
    if (c == 0)
        return;
    if (strcmp(c, "ob_MARKTSTAND_TISCHLER_SCHMIEDE") == 0) {
        ai_MarketStandWindow(market, 0x92);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_STEINMETZ_WIRT") == 0) {
        ai_MarketStandWindow(market, 0x93);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_PARFUM_KRAEUTER") == 0) {
        ai_MarketStandWindow(market, 0x94);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_ROHSTOFFE") == 0) {
        ai_MarketStandWindow(market, 0x95);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_IMPORTEUR") == 0) {
        ai_MarketStandWindow(market, 0x96);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_KIRCHE") == 0) {
        ai_MarketStandWindow(market, 0x97);
        return;
    }
    if (strcmp(c, "ob_MARKTSTAND_OKKULT") == 0) {
        ai_MarketStandWindow(market, 0x2db);
        return;
    }
    if (strcmp(c, "contact_ob_SCHWARZES_BRETT") == 0 || strcmp(c, "ob_SCHWARZES_BRETT") == 0) {
        ai_NoticeBoardWindow();
        return;
    }
    if (strcmp(c, "ob_TRIBUENE") == 0) {
        ai_BardWindow();
        return;
    }
    if (c != fleck1 && c != fleck2 && c != fleck3)
        return;
    bld = (AIBLD *)((D3OBJECT *)c)->parent->owner;
    if (bld == 0)
        return;
    ai_MarketStandWindow(bld, 0x95);
}
/* 0x59aae0, the wine cellar of the player's house (535 B): the form "locations\wohnsitz\weinkeller" with the texts
   0x1a4c and 0x1a4d (the own player slot) and a slider over the money the cellar room (sv_PanelShown) holds,
   up to the player's own money plus the cellar's (both converted to the own player slot's money, the windows.h
   max, which is why the conversion calls repeat); while the frame pump runs, a right click or the button 1
   closes it and the button 0 moves the difference between the slider and the cellar's money from the cellar to
   the player or back (cm_SendMoveGoods) and closes it; then the camera follows the player again */

void ai_WineCellarWindow(void)
{
    int win;
    int own;
    int cellar;
    int slider;
    int ctl;

    win = d2_OpenForm(0, 0, "locations\\wohnsitz\\weinkeller");
    own = gm_GetGoodsValue(ai_Chars[ai_PlayerId]);
    cellar = gm_SumListAmountsL((void *)((AIROOM *)sv_PanelShown)->list);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x1a4c);
    d2_SetForm(win, 2);
    ctl = d2_GetWinObj(win, 2, io_PrintText(0x1a4d, ai_OwnPlayer));
    d2_SetObjValue4(ctl, 0, max(gm_ConvertMoney(cellar, ai_OwnPlayer) + gm_ConvertMoney(own, ai_OwnPlayer),
                               gm_ConvertMoney(cellar, ai_OwnPlayer)),
                   gm_ConvertMoney(cellar, ai_OwnPlayer));
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame(ai_WineCellarWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            slider = gm_PlayerFactor(d2_GetObjValue(ctl), ai_OwnPlayer);
            if (cellar < slider)
                cm_SendMoveGoods(((AIROOM *)sv_PanelShown)->objId, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId,
                                 ai_OwnPlayer, slider - cellar);
            else
                cm_SendMoveGoods(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ((AIROOM *)sv_PanelShown)->objId,
                                 ai_OwnPlayer, cellar - slider);
            sv_PopRequest = 1;
        }
    }
    if (win != 0)
        d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x59ad00, the wine cellar contact listener (96 B): the pump 0x5309a0 (0x59ad00, 0x67fff) looks up
   "contact_WEINSCHRANK" (0x1440aec, class 0x16) and opens the cellar window 0x59aae0 when it is clicked */

void ai_WineCellarContact(int building, int room)
{
    int c;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_WineCellarContact, 0x67fff) != 0) {
        c = sv_RegisterNamePlate("contact_WEINSCHRANK", ai_ContactTextWineCellar, 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == c)
            ai_WineCellarWindow();
    }
}
typedef struct CREDITROW {          /* a row of the new credit window (ai_NewCreditWindow): an offer and its object */
    ORDCREDIT offer;                /* 0x00, the bank's offer (gm_GetCreditOffers) */
    int obj;                        /* 0x0c, the row's window object (d2_GetWinObj) */
    int index;                      /* 0x10, the offer's slot in the bank's credits[] */
} CREDITROW;
/* 0x59ad60, the new credit window "locations\geldleihe\geldleihe_neuerkredit" of the bank bank (719 B): the credit
   offers gm_GetCreditOffers makes the player (up to 3) with a positive amount become the window's rows (the text
   0x18d2 of amount, interest and term, an object of the form 2 each) under the texts 0x18d0 / 0x18d1 (the bank's
   master and the bank); a click on a row takes that credit: the AI message 0x23 to the bank's owner (the amount,
   term and interest at arg[2] / arg[3] / arg[5]), the sum paid to the player (cm_SendMoveGoodsUnchecked), the
   window waits for the message to go out, shows the text 0x18d4 and marks the bank's offer slot taken (-1);
   a right click or the button 1 closes it */

void ai_NewCreditWindow(AIBLD *bank)
{
    CREDITROW rows[3];
    AIMSG msg;
    ORDCREDIT offers[3];
    int win;
    int i;
    int n;
    int handle;

    memset(rows, 0, sizeof(rows));
    gm_GetCreditOffers((int)bank, (unsigned short *)ai_Chars[ai_PlayerId], 3, (int *)offers);
    n = 0;
    for (i = 0; i < 3; i++) {
        if (offers[i].amount > 0) {
            rows[n].offer.amount = offers[i].amount;
            rows[n].offer.term = offers[i].term;
            rows[n].offer.interest = offers[i].interest;
            rows[n].index = i;
            n++;
        }
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_neuerkredit");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x18d0);
    d2_SetForm(win, 2);
    io_PrintText(0x18d1, bank->masterId, bank);
    for (i = 0; i < 3; i++) {
        if (rows[i].offer.amount > 0)
            rows[i].obj = d2_GetWinObj(win, 2, io_PrintText(0x18d2, rows[i].offer.amount, rows[i].offer.interest,
                                                            rows[i].offer.term, 0x5d));
    }
    io_PrintText(0x18d3);
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame(ai_NewCreditWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            for (i = 0; i < 3; i++) {
                if (rows[i].offer.amount != 0 && d2_PointerObj == rows[i].obj) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x23;
                    msg.to = ((AICHARREC *)ai_Chars)[bank->owner].netId;
                    msg.fromId = bank->id;
                    msg.time = ai_Time;
                    msg.flags = 0x12;
                    msg.arg[0] = bank->id;
                    msg.arg[1] = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                    msg.arg[2] = rows[i].offer.amount;
                    msg.arg[3] = rows[i].offer.term;
                    msg.arg[5] = rows[i].offer.interest;
                    cm_SendMoveGoodsUnchecked(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, bank->id, ai_OwnPlayer,
                                              rows[i].offer.amount);
                    handle = cm_SendLongMessage((char *)&msg);
                    while (cm_GetSlotState(handle) == 0)
                        sv_RunFrame(0, 0x677c7);
                    d2_EnableViewObjects(win, 0);
                    sv_MessageBox(txt_Ptrs[0x18d4], 4);
                    d2_EnableViewObjects(win, 1);
                    bank->credits[rows[i].index].amount = -1;
                    sv_PopRequest = 1;
                    break;
                }
            }
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
    }
    d2_CloseView(win);
}

short * gm_FindChar(int id); void txt_Format(char *out, int id0, ...);
short * gm_PlayerRecord(char *c); void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2); void he_FreeEvent();
void d2_RadioClear(int g); int d2_CreateRadioGroup(int n, ...); void d2_SetObjText(); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
int d2_RemoveWindow(); void d2_RemoveObj(); int gm_SumSlotAmounts(void *list, unsigned char i);
void gm_SortCharsByName(int n, int *ids, int flag); void d2_AddPointerItem(int id, int amount); void d2_ClearPointerItems(void); int d2_CountPointerItems();
void d2_EndInputEdit(void); void d2_EnableObject(); void d2_ScrollWindowBy(); void d2_MoveObj(short x, short y, int obj);
int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win); void d2_SetObjFont(); void d2_CopyCaption(char *caption);
void d2_SetObjPic(); int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldDeltas(void); void cm_BeginFieldBlock(char *o, int id);
char *itoa(int, char *, int); void cm_BeginGroup(const char *s); int cm_EndGroup(const char *s);
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f); int cm_SendMoveGoods(int a, int b, unsigned char c, int d); int gm_GetGoodsValue(); int gm_ConvertMoney(int a, unsigned char k);

/* 0x59b030, pays back the loan loan of the bank bank (300 B): the borrower (gm_FindChar of the loan's partner) is
   asked with the text 0x18d5 (the amount); when the bank is given, the borrower has the amount in the own player
   slot's money and the player confirms, the amount moves between the bank and the borrower (cm_SendMoveGoods),
   the borrower's record counts it as repaid (LCFIN.loansRepaid), the bank's owner gets the news 0x18d8 unless it
   is of kind 5, and the loan event is freed */
void ai_RepayLoan(AIBLD *bank, HELOAN *loan)
{
    char news[0x200];
    char text[0x400];
    unsigned short *borrower;
    AICHARREC *owner;
    LCFIN *fin;

    borrower = (unsigned short *)gm_FindChar(loan->partner);
    txt_Format(text, 0x18d5, loan->amount);
    if (bank == 0)
        return;
    if (sv_CheckGoodAmountB((char *)borrower, loan->amount, ai_OwnPlayer) == 0)
        return;
    if (sv_MessageBox(text, 1) == 0)
        return;
    owner = &((AICHARREC *)ai_Chars)[bank->owner];
    cm_SendMoveGoods(bank->id, loan->partner, ai_OwnPlayer, loan->amount);
    fin = (LCFIN *)gm_PlayerRecord((char *)borrower);
    if (fin != 0)
        fin->loansRepaid += loan->amount;
    if (owner->kind != 5) {
        txt_Format(news, 0x18d8, *borrower, bank, loan->amount);
        he_MessageBoxQuickjump(owner->netId, -1, news, 0, 0x5ec, bank->id, -1, 0, 0);
    }
    he_FreeEvent(loan);
}
typedef struct CREDITLISTROW {      /* a row of the credit list window (ai_CreditListWindow) */
    int child;                      /* 0x0, the row's child window (d2_CreateChildWindow), -1: none */
    int pic;                        /* 0x4, its frame picture 0x75a (d2_WinOpenPic), -1: none; the radio group selects it */
    HELOAN *loan;                   /* 0x8, the loan it shows (gm_CreditEvents) */
} CREDITLISTROW;
/* 0x59b160, the money lender's credit list "locations\geldleihe\geldleihe_Kreditaufnehmen" of the bank bank
   (1224 B): the form with the paddings 0x18, the text 0x18c9 (the object name of the own player slot's money good);
   while the frame pump runs, on a rebuild the player's loans (gm_CreditEvents: all of them, then this bank's)
   become rows (a frame picture 0x75a and a child window with the text 0x18d6: amount, interest, due date) of a
   radio group, under the text 0x18cb (9 loans or more), 0x18cc (3 or more), 0x18cd (none) or 0x18ca (the owner,
   the bank, the count and the count of all loans), and the buttons txt 0x18ce (a new credit, below 3 loans) and
   0x18cf (pay back, enabled while a row is selected); a new credit opens ai_NewCreditWindow, a payback
   ai_RepayLoan of the selected row's loan, either rebuilds the list */

void ai_CreditListWindow(AIBLD *bank)
{
    int radio;
    int n;
    int newCtl;
    int total;
    int rowH;
    int win;
    int repayCtl;
    int out[4];
    CREDITLISTROW rows[9];
    HELOAN *loans[9];
    int rebuild;
    int full;
    int rowW;
    int i;

    newCtl = -1;
    repayCtl = -1;
    n = 0;
    rebuild = 1;
    radio = -1;
    for (i = 0; i < 9; i++) {
        rows[i].child = -1;
        rows[i].pic = -1;
        rows[i].loan = 0;
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_kreditaufnehmen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0x18;
    d2_Wins[d2_LastWindow].padt = 0x18;
    d2_Wins[d2_LastWindow].padr = 0x18;
    d2_Wins[d2_LastWindow].padb = 0x18;
    d2_SetForm(win, 1);
    io_PrintText(0x18c9, ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].moneyGood * 2 + 0x9a8);
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame(ai_CreditListWindow, 0x677c7) != 0) {
        if (rebuild) {
            for (i = 0; i < 9; i++) {
                if (rows[i].child != -1) {
                    d2_RemoveWindow(rows[i].child);
                    d2_RemoveObj(rows[i].pic);
                    rows[i].child = -1;
                    rows[i].pic = -1;
                    rows[i].loan = 0;
                }
            }
            total = gm_CreditEvents(ai_PlayerId, 0, loans);
            n = gm_CreditEvents(ai_PlayerId, bank, loans);
            d2_SetForm(win, 4);
            io_PrintText("$C");
            if (n >= 9) {
                io_PrintText(0x18cb);
                full = 1;
            } else if (n >= 3) {
                io_PrintText(0x18cc);
                full = 1;
            } else {
                if (n == 0)
                    io_PrintText(0x18cd, bank->owner, bank);
                else
                    io_PrintText(0x18ca, bank->owner, bank, n, total);
                full = 0;
            }
            d2_SetForm(win, 3);
            io_PrintText("$C");
            if (!full) {
                sv_ButtonRow(d2_LastWindow, full, out, 2, txt_Ptrs[0x18ce], txt_Ptrs[0x18cf]);
                newCtl = out[0];
                repayCtl = out[1];
            } else {
                sv_ButtonRow(d2_LastWindow, 0, out, 1, txt_Ptrs[0x18cf]);
                newCtl = -1;
                repayCtl = out[0];
            }
            rowW = d2_Pics[0x75a].w;
            rowH = d2_Pics[0x75a].h;
            for (i = 0; i < n; i++) {
                d2_SetForm(win, 2);
                rows[i].pic = d2_WinOpenPic((d2_LastWindowPtr->w - rowW) >> 1, (rowH + 8) * i, 0x75a, d2_LastWindow);
                rows[i].child = d2_CreateChildWindow((d2_LastWindowPtr->w - rowW) >> 1, (rowH + 8) * i, rowW, rowH, 0x10,
                                                     d2_LastWindow);
                rows[i].loan = loans[i];
                d2_Wins[rows[i].child].font = 0x56;
                d2_Wins[rows[i].child].ml = 0x10;
                io_PrintText(0x18d6, loans[i]->amount, loans[i]->interest, &loans[i]->due);
            }
            if (radio != -1) {
                d2_RadioClear(radio);
                radio = -1;
            }
            switch (n) {
            case 1:
                radio = d2_CreateRadioGroup(1, rows[0].pic);
                break;
            case 2:
                radio = d2_CreateRadioGroup(2, rows[0].pic, rows[1].pic);
                break;
            case 3:
                radio = d2_CreateRadioGroup(3, rows[0].pic, rows[1].pic, rows[2].pic);
                break;
            }
            rebuild = 0;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (repayCtl != -1) {
            if (n > 0 && d2_Radios[radio].sel != -1)
                d2_SetObjEnabled(repayCtl, 1);
            else
                d2_SetObjEnabled(repayCtl, 0);
        }
        if (d2_ClickedId != -1 && d2_ClickedId == 0) {
            if (d2_PointerObj == newCtl && newCtl != -1) {
                d2_SetViewEnabled(win, 0);
                ai_NewCreditWindow(bank);
                d2_SetViewEnabled(win, 1);
                rebuild = 1;
            } else if (d2_PointerObj == repayCtl && repayCtl != -1) {
                d2_SetViewEnabled(win, 0);
                ai_RepayLoan(bank, rows[d2_Radios[radio].sel].loan);
                rebuild = 1;
                d2_SetViewEnabled(win, 1);
            }
        }
    }
    d2_CloseView(win);
    if (radio != -1)
        d2_RadioClear(radio);
    sv_CameraFollow();
}
/* 0x59b630, the loans of one borrower at the bank bank (581 B), for the bank's owner (ai_CreditGiverWindow): the form
   "locations\geldleihe\geldleihe_kreditaufnehmen" with the paddings 0xe / 0x18 / 0x18 / 0xe and the text 0x18d9;
   while the frame pump runs, the first frame lists the borrower's loans at the bank (gm_CreditEvents), each a row
   of the credit list (a frame picture 0x75a and a child window with the texts 0x18e0 amount, 0x18e1 due date,
   0x18e2 interest); a right click closes it */

void ai_BorrowerLoansWindow(AIBLD *bank, int borrower)
{
    int rowH;
    int win;
    int rowW;
    HELOAN *loans[9];
    CREDITLISTROW rows[9];
    int rebuild;
    int n;
    int i;

    rebuild = 1;
    for (i = 0; i < 9; i++) {
        rows[i].child = -1;
        rows[i].pic = -1;
        rows[i].loan = 0;
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_kreditaufnehmen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0xe;
    d2_Wins[d2_LastWindow].padt = 0x18;
    d2_Wins[d2_LastWindow].padr = 0x18;
    d2_Wins[d2_LastWindow].padb = 0xe;
    d2_SetForm(win, 1);
    io_PrintText(0x18d9);
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame(ai_BorrowerLoansWindow, 0x677c7) != 0) {
        if (rebuild) {
            for (i = 0; i < 9; i++) {
                if (rows[i].child != -1) {
                    d2_RemoveWindow(rows[i].child);
                    d2_RemoveObj(rows[i].pic);
                    rows[i].child = -1;
                    rows[i].pic = -1;
                    rows[i].loan = 0;
                }
            }
            n = gm_CreditEvents(borrower, bank, loans);
            rowW = d2_Pics[0x75a].w;
            rowH = d2_Pics[0x75a].h;
            for (i = 0; i < n; i++) {
                d2_SetForm(win, 2);
                rows[i].pic = d2_WinOpenPic(0, (rowH + 4) * i, 0x75a, d2_LastWindow);
                rows[i].child = d2_CreateChildWindow(0, (rowH + 4) * i, rowW, rowH, 0x110, d2_LastWindow);
                rows[i].loan = loans[i];
                d2_Wins[rows[i].child].font = 0x56;
                d2_Wins[rows[i].child].ml = 0x10;
                io_PrintText(0x18e0, rows[i].loan->amount);
                io_PrintText(0x18e1, &rows[i].loan->due);
                io_PrintText(0x18e2, rows[i].loan->interest);
            }
            rebuild = 0;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x59b880, the money lender's credit release window "locations\geldleihe\geldleihe_KreditFreigabe" of the bank
   bank (587 B), for its owner: the texts 0x18e5 and 0x18e6 (the object name of the own player slot's money good),
   whose two objects are the interest slider (5 .. 0x28, at the bank's interest) and the money slider (up to the
   owner's money, at the money the bank's stock holds, both in the own player slot's money); while the frame pump
   runs, sv_PendingMoney shows the stock minus the chosen sum; the button 0 moves the difference between the
   owner and the bank (cm_SendMoveGoods), sends the new interest as the bank's field 0x65 and closes it; a right
   click or the button 1 closes it */

void ai_CreditReleaseWindow(AIBLD *bank)
{
    int value;
    int moneyCtl;
    int win;
    int stock;
    int own;
    int interestCtl;
    int text;

    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_KreditFreigabe");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x18e5);
    stock = gm_SumSlotAmounts((void *)bank->objList, ai_OwnPlayer);
    d2_SetForm(win, 2);
    text = io_PrintText(0x18e6, ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].moneyGood * 2 + 0x9a8);
    interestCtl = d2_GetWinObj(win, 2, text);
    moneyCtl = d2_GetWinObj(win, 2, text + 1);
    d2_SetObjValue4(interestCtl, 5, 0x28, bank->interest);
    own = gm_GetGoodsValue(ai_Chars[bank->owner]);
    d2_SetObjValue4(moneyCtl, 0, gm_ConvertMoney(own, ai_OwnPlayer), gm_ConvertMoney(stock, ai_OwnPlayer));
    d2_SetPointerSprite(0, 0);
    while (sv_RunFrame(ai_CreditReleaseWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        sv_PendingMoney = stock - gm_PlayerFactor(d2_GetObjValue(moneyCtl), ai_OwnPlayer);
        if (d2_ClickedId == 0) {
            value = gm_PlayerFactor(d2_GetObjValue(moneyCtl), ai_OwnPlayer);
            if (stock < value)
                cm_SendMoveGoods(bank->id, ((AICHARREC *)ai_Chars)[bank->owner].netId, ai_OwnPlayer, value - stock);
            else
                cm_SendMoveGoods(((AICHARREC *)ai_Chars)[bank->owner].netId, bank->id, ai_OwnPlayer, stock - value);
            value = d2_GetObjValue(interestCtl);
            cm_BeginFieldBlock((char *)bank, bank->id);
            cm_AddFieldDelta(4, 1, 0x65, &value);
            cm_SendFieldDeltas();
            sv_PopRequest = 1;
        }
    }
    d2_CloseView(win);
    sv_PendingMoney = 0;
}

int gm_SumSlotAmounts(void *list, unsigned char i); int gm_GetGoodsValue(); int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
void d2_SetObjEnabled(); void d2_EnableViewObjects(); void d2_EnableObject();
void gm_SortCharsByName(int n, int *ids, int flag); int d2_RemoveWindow(); void d2_RemoveObj();
void d2_SetObjText(); void d2_MoveObj(short x, short y, int obj); int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win); void d2_SetObjFont();

/* 0x59bad0, the money lender's credit giver window "locations\geldleihe\geldleihe_KreditGeber" of the bank bank
   (951 B), for its owner: the scroll list (0x206 / 0x167) with the paddings 0xe / 0x18 / 0xe / 0xe, the text
   0x18d9 and the release button (txt_Ptrs[0x18dd]); on a rebuild it frees the 16 rows, collects the bank's
   loans (gm_BuildingEvents) and their borrowers (by netId, gm_FindChar) into up to 16 ids, prints 0x18de (the
   own player slot's money good) or 0x48d (no borrower), sorts the borrowers by name and lists each as a text
   0x18df (the borrower, his loans, their summed amount); the release button opens ai_CreditReleaseWindow, a
   borrower's row ai_BorrowerLoansWindow (then the list is rebuilt); a right click closes it */
void ai_CreditGiverWindow(AIBLD *bank)
{
    CREDITLISTROW rows[16];
    HELOAN *loans[16];
    int buttons[8];
    int borrowers[16];
    int bar;
    int down;
    int up;
    int release;
    int n;
    int win;
    int rebuild;
    int count;
    int i;
    int j;
    int sum;
    AICHARREC *ch;

    count = 0;
    rebuild = 1;
    for (i = 0; i < 16; i++) {
        rows[i].child = -1;
        rows[i].pic = -1;
        rows[i].loan = 0;
    }
    d2_SetPointerSprite(0, 0);
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_KreditGeber");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x73b, 0x73f);
    sv_CreateScrollButtons(0x206, 0x167, d2_LastWindow, d2_GetViewWin(win, 2), 0x78, &down, &up, &bar);
    d2_Wins[d2_LastWindow].padl = 0xe;
    d2_Wins[d2_LastWindow].padt = 0x18;
    d2_Wins[d2_LastWindow].padr = 0xe;
    d2_Wins[d2_LastWindow].padb = 0xe;
    d2_SetForm(win, 1);
    io_PrintText(0x18d9);
    d2_SetForm(win, 3);
    sv_ButtonRow(d2_LastWindow, 0, buttons, 1, txt_Ptrs[0x18dd]);
    release = buttons[0];
    while (sv_RunFrame(ai_CreditGiverWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (rebuild) {
            d2_SetForm(win, 2);
            for (i = 0; i <= 15; i++) {
                if (rows[i].child != -1) {
                    d2_RemoveWindow(rows[i].child);
                    d2_RemoveObj(rows[i].pic);
                    rows[i].child = -1;
                    rows[i].pic = -1;
                    rows[i].loan = 0;
                }
            }
            io_PrintText("$C");
            n = gm_BuildingEvents(bank->owner, bank, 0xffff, loans);
            for (i = 0; i < 16; i++)
                borrowers[i] = -1;
            count = 0;
            for (i = 0; i < n; i++) {
                for (j = 0; j < count + 1; j++) {
                    if (((AICHARREC *)ai_Chars)[borrowers[j]].netId == loans[i]->partner)
                        break;
                    if (borrowers[j] == -1) {
                        ch = (AICHARREC *)gm_FindChar(loans[i]->partner);
                        if (ch != 0) {
                            borrowers[j] = ch->id;
                            count++;
                        }
                        break;
                    }
                }
            }
            d2_SetForm(win, 2);
            io_PrintText(0x18de, ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].moneyGood * 2 + 0x9a8);
            if (count == 0)
                io_PrintText(0x48d);
            gm_SortCharsByName(count, borrowers, 0);
            for (i = 0; i < count; i++) {
                n = gm_BuildingEvents(bank->owner, bank, borrowers[i], loans);
                sum = 0;
                for (j = 0; j < n; j++)
                    sum += loans[j]->amount;
                rows[i].pic = d2_GetWinObj(win, 2, io_PrintText(0x18df, borrowers[i], n, sum));
            }
            rebuild = 0;
        }
        if (d2_ClickedId == 0) {
            if (d2_PointerObj == release) {
                ai_CreditReleaseWindow(bank);
            } else {
                for (i = 0; i < count; i++) {
                    if (d2_PointerObj == rows[i].pic) {
                        ai_BorrowerLoansWindow(bank, borrowers[i]);
                        rebuild = 1;
                    }
                }
            }
        }
    }
    d2_CloseView(win);
    sv_CameraFollow();
}

/* 0x59be90, sorts the rows of the money lender's exchange window by the name of their currency good and places
   them (653 B): the rows are counted up to the last used one (a good and an icon), sorted by an exchange
   sort on the name (the own player slot's money good as "AAAAAAAAA" first, none as "ZZZZZZZZZ" last), then each
   used row's icon and text move to the row's place in the current window (0x4c apart, the text 0x30 below) */
void ai_SortExchangeRows(LCEXCHROW *rows)
{
    char nameB[0x80];
    char nameA[0x80];
    LCEXCHROW tmp;
    int n;
    int i;
    int j;
    int row;
    int own;

    n = 0;
    for (i = 15; i >= 0; i--) {
        if (rows[i].good != 0 && rows[i].icon != -1) {
            n = i + 1;
            break;
        }
    }
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            own = ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].moneyGood;
            if (rows[i].good == own)
                strcpy(nameA, "AAAAAAAAA");
            else if (rows[i].good == 0)
                strcpy(nameA, "ZZZZZZZZZ");
            else
                strcpy(nameA, (char *)txt_ObjectNames[rows[i].good][0]);
            if (rows[j].good == own)
                strcpy(nameB, "AAAAAAAAA");
            else if (rows[j].good == 0)
                strcpy(nameB, "ZZZZZZZZZ");
            else
                strcpy(nameB, (char *)txt_ObjectNames[rows[j].good][0]);
            if (strcmp(nameA, nameB) == 1) {
                tmp = rows[i];
                rows[i] = rows[j];
                rows[j] = tmp;
            }
        }
    }
    row = 0;
    for (i = 0; i < n; i++) {
        if (rows[i].icon != -1) {
            d2_MoveObj(d2_Objs[rows[i].icon].x, d2_LastWindowPtr->y + row * 0x4c, rows[i].icon);
            d2_MoveObj(d2_Objs[rows[i].text].x, d2_LastWindowPtr->y + row * 0x4c + 0x30, rows[i].text);
            row++;
        }
    }
}
/* 0x59c120, fills one side of the money lender's exchange window from the object list list (814 B): each currency
   object of the list (gm_Query(list, 1, 4, 9)) whose good no row holds yet takes the first free row, with an input
   icon (the good's icon, 0x82) and a centred text below it in the form view / form; then every row reads its
   icon's value and the amount the list holds of its good (a row whose good is gone is removed), shows the amount
   and the good's name ("%I %s"); with input, a picked amount goes to the pointer (d2_AddPointerItem) and turns
   the other rows off; when a row came or went the rows are sorted (ai_SortExchangeRows); returns the row whose
   amount was picked, else -1 */
int ai_FillExchangeRows(int list, LCEXCHROW *rows, int view, int form, char input)
{
    char text[0x20];
    int sel;
    int changed;
    unsigned short *good;
    AIOBJ *obj;
    int i;
    int j;
    int value;
    int amount;

    sel = -1;
    changed = 0;
    for (good = (unsigned short *)gm_Query((void *)list, 1, 4, 9); good != 0; good = gm_QueryNext()) {
        for (i = 0; i < 16; i++) {
            if (rows[i].good == *good)
                break;
        }
        if (i < 16)
            continue;
        for (i = 0; i < 16; i++) {
            if (rows[i].good == 0) {
                rows[i].good = *good;
                d2_SetForm(view, form);
                rows[i].icon = d2_WinOpenInputIcon((d2_LastWindowPtr->w - 0x30) >> 1, i * 0x4c, rows[i].good + 0xdf,
                                                   0x82, d2_LastWindow);
                d2_Objs[rows[i].icon].textShadow = 1;
                rows[i].text = d2_CreateWinText((d2_LastWindowPtr->w - 0xbe) >> 1, i * 0x4c + 0x2e, ch_EmptyName,
                                                d2_LastWindow);
                d2_SetObjFont(rows[i].icon, 0x54);
                d2_SetObjFont(rows[i].text, 0x54);
                d2_Objs[rows[i].text].centred = 1;
                d2_Objs[rows[i].text].w = 0xbe;
                if (input != 0)
                    d2_Objs[rows[i].icon].clickDisabled = 1;
                break;
            }
        }
        changed = 1;
    }
    if (input != 0) {
        for (i = 0; i < 16; i++)
            if (rows[i].icon != -1)
                d2_SetObjEnabled(rows[i].icon, 1);
    }
    for (i = 0; i < 16; i++) {
        if (rows[i].icon == -1)
            continue;
        value = d2_GetObjValue(rows[i].icon);
        rows[i].value = value;
        obj = (AIOBJ *)gm_Query((void *)list, 1, 0, rows[i].good);
        if (obj != 0 && gm_CurrencyDivide(obj->count, rows[i].good) != 0) {
            amount = gm_CurrencyDivide(obj->count, rows[i].good);
            d2_SetObjValue4(rows[i].icon, 0, amount, value);
            txt_Format(text, "%I %s", amount, txt_ObjectNames[rows[i].good][0]);
            d2_SetObjText(rows[i].text, text);
            d2_Objs[rows[i].text].w = 0xbe;
            rows[i].available = amount;
        } else {
            d2_RemoveObj(rows[i].text);
            rows[i].text = -1;
            d2_RemoveObj(rows[i].icon);
            rows[i].icon = -1;
            rows[i].available = 0;
            rows[i].good = 0;
            changed = 1;
        }
        if (input != 0 && value != 0) {
            d2_AddPointerItem(rows[i].good + 0xdf, value);
            for (j = 0; j < 16; j++) {
                if (rows[j].icon != -1 && i != j)
                    d2_SetObjEnabled(rows[j].icon, 0);
            }
            sel = i;
        }
    }
    if (changed) {
        d2_SetForm(view, form);
        ai_SortExchangeRows(rows);
    }
    return sel;
}
/* 0x59c450, shows the rows of the money lender's exchange window that hold a good (594 B), at most 16, 0x4c apart in
   the form view / form: the good's icon (an input icon 0x82 with input, else a clickable picture), below it a centred
   text with the good's name ("%S") and under that one with the name of the row's player slot */
void ai_ShowExchangeRows(LCEXCHROW *rows, int view, int form, int input)
{
    char text[0x80];
    int i;
    int y;

    i = 0;
    for (y = 0; y < 0x4c0; y += 0x4c) {
        if (rows[i].good == 0)
            return;
        d2_SetForm(view, form);
        if (input != 0) {
            rows[i].icon = d2_WinOpenInputIcon((d2_LastWindowPtr->w - 0x30) >> 1, y, rows[i].good + 0xdf, 0x82,
                                               d2_LastWindow);
        } else {
            rows[i].icon = d2_WinOpenPic((d2_LastWindowPtr->w - 0x30) >> 1, i * 0x4c, rows[i].good + 0xdf,
                                         d2_LastWindow);
            d2_Objs[rows[i].icon].interactive = 1;
        }
        d2_Objs[rows[i].icon].textShadow = 1;
        rows[i].text = d2_CreateWinText((d2_LastWindowPtr->w - 0xbe) >> 1, i * 0x4c + 0x30, ch_EmptyName,
                                        d2_LastWindow);
        txt_Format(text, "%S", txt_ObjectNames[rows[i].good][0]);
        d2_SetObjText(rows[i].text, text);
        d2_SetObjFont(rows[i].icon, 0x54);
        d2_SetObjFont(rows[i].text, 0x54);
        d2_Objs[rows[i].text].centred = 1;
        d2_Objs[rows[i].text].w = 0xbe;
        rows[i].name = d2_CreateWinText((d2_LastWindowPtr->w - 0xbe) >> 1, i * 0x4c + 0x3f, ch_EmptyName,
                                        d2_LastWindow);
        txt_Format(text, "%s", ((GMPLAYERSLOT *)ai_PlayerSlots)[rows[i].player].name);
        d2_SetObjText(rows[i].name, text);
        d2_SetObjFont(rows[i].name, 0x54);
        d2_Objs[rows[i].name].centred = 1;
        d2_Objs[rows[i].name].w = 0xbe;
        i++;
    }
}
/* 0x59c6b0, the index of the clicked record of the money lender's exchange list: while the latched left click
   (dm_L.lclick) is down it returns the first of the 16 records (7 ints each) whose control (+0) is the clicked
   control d2_PointerObj, else -1 */

int ai_ClickedExchangeRecord(LCEXCHROW *rows)
{
    int i;

    if (dm_L.lclick != 0) {
        for (i = 0; i < 16; i++) {
            if (rows[i].icon != -1 && d2_PointerObj == rows[i].icon)
                return i;
        }
    }
    return -1;
}
/* 0x59c6e0, number of used records of a 7-int list (32 B): the records whose control (+0) is not -1 */

int ai_UsedRecordCount(LCEXCHROW *rows)
{
    int n;
    int i;

    n = 0;
    for (i = 0; i < 16; i++)
        if (rows[i].icon != -1)
            n++;
    return n;
}
/* 0x59c700, the money lender's exchange window "locations\geldleihe\geldleihe_Wechsel" of the bank bank (3024 B):
   two lists of currency rows, the bank's (its object list, form 5) and the player's (his inventory, form 4), each
   with a scroll pair and a count text (shown from 5 rows on), the texts 0x18f5 and 0x18f6 (the two fees); every
   frame the lists are refilled: an amount picked in a bank row (side 1, the status 0x18fa, the arrow 0x4f8) or else
   in an own row (side 2, 0x18f8, 0x4f9) is the offer, none shows 0x18f7 (0x4f7); a click on a row of the other list
   exchanges: the value of the offer (gm_CurrencyFactor) plus the bank's exchange fee (its percent of it, at least
   one coin of the picked good, none for the bank's owner) against the other good, refused when that row holds too
   little (0x18fd) or it is the same good (0x18fc); the four moves go out as one "kredit" group, then the lists are
   refilled (0x18fe) and the picked amounts cleared; a right click clears them, or closes the window when none
   @deep reconstructed from the whole original 0x59c700..0x59d2d0 (the draft had condensed the money
   moves): both row lists (LCEXCHROW), the scroll pairs and count texts (<= 4 disables), the offer
   selection with picked / sel, the fee as __max(value * rate, gm_CurrencyFactor(1, good)) (the original
   calls it twice and tests test ah,5 / jnp), owner exempt, the four cm_SendExSellObject moves of both
   sides with their (int) conversions, the cm_EndGroup slot wait (cm_EndGroup now returns cm_QueueSend's
   slot), the refills and clears; fee * 0.01 order read and fixed. Left is allocation: the original keeps
   sel / picked, the side-2 amount, the paid amount and idx * 0x1c in stack slots ([esp+0x10], [esp+0x14],
   [esp+0x18], [esp+0x28]) and reloads bank from its argument and bankText / ownText / arrow from slots at
   the loop head, ours keeps sel in edi, bank in ebp and a zero in ebx (pushing the callee-saved registers
   before the init loop: '- push ebp / push ebx / xor ebx, ebx' vs '+ xor edx, edx ... push ebx' after it);
   frame 0x4b4 vs 0x4b8 (one slot more in the original), 884 vs 893 instructions (references: M8 at +0x412:
   ours refers to d2_CopyCaption (0x478870), the original's field to 0x59c6e0 (ai_UsedRecordCount) (and 1
   more), explained: both M8 are alignment crossings of the same calls: at +0x412 the original lays the 'no
   offer' else block (d2_CopyCaption 0x18f7, d2_SetObjPic 0x4f7) out of line after the two
   ai_UsedRecordCount calls and merges the 0x4f8 / 0x4f9 d2_SetObjPic tails (calls: d2_SetObjPic ours x3,
   the original x2), at +0x6cb the side-1 refusal txt_Format / d2_CopyCaption pair lines up one call off;
   every call, argument and literal was read equal (the proof stops at the pooled literal of the form
   name)) */
void ai_ExchangeWindow(AIBLD *bank)
{
    char text[0x100];
    LCEXCHROW ownRows[16];
    LCEXCHROW bankRows[16];
    float fee;
    float value;
    float rate;
    int amount;
    int paid;
    int sel;
    int picked;
    int side;
    int idx;
    int bankCount;
    int ownCount;
    int bankUp;
    int bankDown;
    int ownUp;
    int ownDown;
    int bankText;
    int ownText;
    int arrow;
    int sold;
    int bought;
    int slot;
    int win;
    int i;

    side = 0;
    for (i = 0; i < 16; i++) {
        bankRows[i].icon = -1;
        bankRows[i].good = 0;
        ownRows[i].icon = -1;
        ownRows[i].good = 0;
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_Wechsel");
    d2_CentreViewWindows(win);
    ownUp = d2_GetWinObj(win, 0, 2);
    ownDown = d2_GetWinObj(win, 0, 3);
    bankUp = d2_GetWinObj(win, 0, 1);
    bankDown = d2_GetWinObj(win, 0, 0);
    arrow = d2_GetWinObj(win, 0, 4);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x73b, 0x73f);
    d2_SetForm(win, 0);
    bankText = d2_CreateWinTextS(d2_Objs[bankUp].x - d2_LastWindowPtr->x + 2,
                                 d2_Objs[bankUp].h - d2_LastWindowPtr->y + d2_Objs[bankUp].y + 2, ch_EmptyName,
                                 d2_LastWindow);
    ownText = d2_CreateWinTextS(d2_Objs[ownUp].x - d2_LastWindowPtr->x + 2,
                                d2_Objs[ownUp].h - d2_LastWindowPtr->y + d2_Objs[ownUp].y + 2, ch_EmptyName,
                                d2_LastWindow);
    d2_Objs[bankText].font = 0x54;
    d2_Objs[ownText].font = 0x54;
    d2_Objs[arrow].textShadow = 1;
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0xe;
    d2_Wins[d2_LastWindow].padt = 0x18;
    d2_Wins[d2_LastWindow].padr = 0xe;
    d2_Wins[d2_LastWindow].padb = 0xe;
    d2_SetForm(win, 1);
    io_PrintText(0x18f5);
    d2_SetForm(win, 2);
    io_PrintText(0x18f6, bank->messengerFee, ((GMPLAYERSLOT *)ai_PlayerSlots)[ai_OwnPlayer].moneyGood * 2 + 0x9a8,
                 bank->exchangeFee);
    d2_CopyCaption(txt_Ptrs[0x18f7]);
    d2_SetPointerSprite(0, 0);
    do {
        if (dm_L.rup != 0) {
            if (d2_CountPointerItems() != 0) {
                d2_EndInputEdit();
                for (i = 0; i <= 15; i++) {
                    if (bankRows[i].icon != -1)
                        d2_SetObjValue4(bankRows[i].icon, 0, 0, 0);
                    if (ownRows[i].icon != -1)
                        d2_SetObjValue4(ownRows[i].icon, 0, 0, 0);
                }
            } else {
                sv_PopRequest = 1;
            }
        }
        d2_ClearPointerItems();
        picked = -1;
        if (side != 2)
            picked = ai_FillExchangeRows(bank->objList, bankRows, win, 5, 1);
        if (picked != -1) {
            if (d2_CaptionCopy[0] == 0 || side == 0) {
                txt_Format(text, 0x18fa, bankRows[picked].value, bankRows[picked].good * 2 + 0x9a8);
                d2_CopyCaption(text);
            }
            side = 1;
            sel = picked;
            d2_SetObjPic(arrow, 0x4f8);
        } else if ((picked = ai_FillExchangeRows(((AICHARREC *)ai_Chars)[ai_PlayerId].inventory, ownRows, win, 4, 1)) != -1) {
            if (d2_CaptionCopy[0] == 0 || side == 0) {
                txt_Format(text, 0x18f8, ownRows[picked].value, ownRows[picked].good * 2 + 0x9a8);
                d2_CopyCaption(text);
            }
            side = 2;
            sel = picked;
            d2_SetObjPic(arrow, 0x4f9);
        } else {
            side = 0;
            d2_CopyCaption(txt_Ptrs[0x18f7]);
            sel = -1;
            d2_SetObjPic(arrow, 0x4f7);
        }
        bankCount = ai_UsedRecordCount(bankRows);
        ownCount = ai_UsedRecordCount(ownRows);
        if (bankCount <= 4) {
            d2_EnableObject(bankUp, 0);
            d2_EnableObject(bankDown, 0);
            d2_EnableObject(bankText, 0);
        } else {
            d2_EnableObject(bankUp, 1);
            d2_EnableObject(bankDown, 1);
            d2_EnableObject(bankText, 1);
            itoa(bankCount, text, 10);
            d2_SetObjText(bankText, text);
        }
        if (ownCount <= 4) {
            d2_EnableObject(ownUp, 0);
            d2_EnableObject(ownDown, 0);
            d2_EnableObject(ownText, 0);
        } else {
            d2_EnableObject(ownUp, 1);
            d2_EnableObject(ownDown, 1);
            d2_EnableObject(ownText, 1);
            itoa(ownCount, text, 10);
            d2_SetObjText(ownText, text);
        }
        if (d2_ClickedId == 3 || d2_ClickedId == 4) {
            if (d2_PointerObj == ownUp) {
                d2_SetForm(win, 4);
                d2_ScrollWindowBy(0, -0x4c, d2_LastWindow);
            } else if (d2_PointerObj == ownDown) {
                d2_SetForm(win, 4);
                d2_ScrollWindowBy(0, 0x4c, d2_LastWindow);
            } else if (d2_PointerObj == bankUp) {
                d2_SetForm(win, 5);
                d2_ScrollWindowBy(0, -0x4c, d2_LastWindow);
            } else if (d2_PointerObj == bankDown) {
                d2_SetForm(win, 5);
                d2_ScrollWindowBy(0, 0x4c, d2_LastWindow);
            }
        }
        if (side == 1) {
            idx = ai_ClickedExchangeRecord(ownRows);
            if (idx != -1) {
                sold = ownRows[idx].good;
                bought = bankRows[sel].good;
                if (sold == bought) {
                    d2_CopyCaption(txt_Ptrs[0x18fc]);
                } else {
                    amount = bankRows[sel].value;
                    rate = bank->exchangeFee * 0.01;
                    value = gm_CurrencyFactor(amount, bought);
                    if (ai_PlayerId != bank->owner) {
                        fee = value * rate;
                        fee = __max(fee, gm_CurrencyFactor(1, bought));
                    } else {
                        fee = 0;
                    }
                    if (gm_CurrencyFactor(ownRows[idx].available, sold) < value) {
                        txt_Format(text, 0x18fd, sold * 2 + 0x9a8);
                        d2_CopyCaption(text);
                    } else {
                        paid = value;
                        amount = gm_CurrencyFactor(amount, bought);
                        cm_BeginGroup("kredit");
                        cm_SendExSellObject(-1, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, sold, paid, ai_OwnPlayer, 0);
                        cm_SendExSellObject(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -1, bought, amount - fee,
                                            ai_OwnPlayer, 0);
                        cm_SendExSellObject(-1, bank->id, bought, amount, ai_OwnPlayer, 0);
                        cm_SendExSellObject(bank->id, -1, sold, paid + fee, ai_OwnPlayer, 0);
                        slot = cm_EndGroup("kredit");
                        while (cm_GetSlotState(slot) == 0)
                            sv_RunFrame(0, 0x677c7);
                        ai_FillExchangeRows(((AICHARREC *)ai_Chars)[ai_PlayerId].inventory, ownRows, win, 4, 1);
                        ai_FillExchangeRows(bank->objList, bankRows, win, 5, 1);
                        sv_SetStatusText(txt_Ptrs[0x18fe]);
                        d2_EndInputEdit();
                        for (i = 0; i <= 15; i++) {
                            if (bankRows[i].icon != -1)
                                d2_SetObjValue4(bankRows[i].icon, 0, 0, 0);
                            if (ownRows[i].icon != -1)
                                d2_SetObjValue4(ownRows[i].icon, 0, 0, 0);
                        }
                    }
                }
            }
        } else if (side == 2) {
            idx = ai_ClickedExchangeRecord(bankRows);
            if (idx != -1) {
                bought = bankRows[idx].good;
                sold = ownRows[sel].good;
                if (sold == bought) {
                    sv_SetStatusText(txt_Ptrs[0x18fc]);
                } else {
                    amount = bankRows[idx].available;
                    if (amount >= ownRows[sel].value)
                        amount = ownRows[sel].value;
                    value = gm_CurrencyFactor(amount, bought);
                    if (ai_PlayerId != bank->owner) {
                        fee = value * (bank->exchangeFee * 0.01);
                        fee = __max(fee, gm_CurrencyFactor(1, sold));
                    } else {
                        fee = 0;
                    }
                    if (gm_CurrencyFactor(ownRows[sel].available, sold) < value) {
                        /* the original names the good of the own row at the clicked bank row's index */
                        txt_Format(text, 0x18fd, ownRows[idx].good * 2 + 0x9a8);
                        sv_SetStatusText(text);
                    } else {
                        gm_CurrencyFactor(amount, sold);
                        cm_BeginGroup("kredit");
                        paid = value;
                        cm_SendExSellObject(-1, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, sold, paid, ai_OwnPlayer, 0);
                        amount = value - fee;
                        cm_SendExSellObject(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -1, bought, amount,
                                            ai_OwnPlayer, 0);
                        cm_SendExSellObject(-1, bank->id, bought, amount, ai_OwnPlayer, 0);
                        cm_SendExSellObject(bank->id, -1, sold, paid, ai_OwnPlayer, 0);
                        slot = cm_EndGroup("kredit");
                        while (cm_GetSlotState(slot) == 0)
                            sv_RunFrame(0, 0x677c7);
                        ai_FillExchangeRows(((AICHARREC *)ai_Chars)[ai_PlayerId].inventory, ownRows, win, 4, 1);
                        ai_FillExchangeRows(bank->objList, bankRows, win, 5, 1);
                        sv_SetStatusText(txt_Ptrs[0x18fe]);
                        d2_EndInputEdit();
                        for (i = 0; i <= 15; i++) {
                            if (bankRows[i].icon != -1)
                                d2_SetObjValue4(bankRows[i].icon, 0, 0, 0);
                            if (ownRows[i].icon != -1)
                                d2_SetObjValue4(ownRows[i].icon, 0, 0, 0);
                        }
                    }
                }
            }
        }
    } while (sv_RunFrame(ai_ExchangeWindow, 0x677c7) != 0);
    d2_ClearPointerItems();
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x59d2d0, the money lender's exchange fees window "..geldleihe_WechselGebuehren" (320 B): the window, the
   text 0x1908, the two counters 0x475b80 (the text 0x1907 and the next text id) and the sliders 0x475cb0
   (3, 0xf, bank->messengerFee / bank->exchangeFee); the pump 0x5309a0 (0x677c7); on select 0x4c3ba0 (bank,
   bank->id), 0x4c3c00 (4, 1, 0x69, &fee), 0x4c3c00 (4, 1, 0x6d, &exchange), 0x4c49a0; 0x4758f0 at the end */

void ai_ExchangeFeesWindow(AIBLD *bank)
{
    int win;
    int ctl1;
    int ctl2;
    int text;

    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_WechselGebuehren");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    io_PrintText(0x1908);
    d2_SetForm(win, 1);
    text = io_PrintText(0x1907);
    ctl1 = d2_GetWinObj(win, 1, text);
    ctl2 = d2_GetWinObj(win, 1, text + 1);
    d2_SetObjValue4(ctl2, 3, 0xf, bank->messengerFee);
    d2_SetObjValue4(ctl1, 3, 0xf, bank->exchangeFee);
    for (;;) {
        if (sv_RunFrame(ai_ExchangeFeesWindow, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            int fee = d2_GetObjValue(ctl2);
            int exchange = d2_GetObjValue(ctl1);
            cm_BeginFieldBlock((char *)bank, bank->id);
            cm_AddFieldDelta(4, 1, 0x69, &fee);
            cm_AddFieldDelta(4, 1, 0x6d, &exchange);
            cm_SendFieldDeltas();
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
    }
}
/* 0x59d410, the money lender's money messenger window "locations\geldleihe\geldleihe_WechselGeldbote" (462 B): the
   text 0x1909 (the amount of the good, the name of the player slot player); on the button 0 the bank pays out the
   value of the amount (gm_CurrencyFactor, cm_SendExSellObject from the bank) and sends the payment message 0x27 to
   its owner: the value less the messenger's fee (3 percent, at least one coin of the good), for the slot player;
   returns 1 when it was sent, 0 when the window was closed (a right click or the button 1) */
int ai_MoneyMessengerWindow(AIBLD *bank, int good, int amount, unsigned char player)
{
    AIMSG msg;
    float fee;
    int value;
    int sent;
    int win;

    sent = 0;
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_WechselGeldbote");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1909, amount, good * 2 + 0x9a8, ((GMPLAYERSLOT *)ai_PlayerSlots)[player].name, player);
    while (sv_RunFrame(ai_MoneyMessengerWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            value = gm_CurrencyFactor(amount, good);
            fee = value * 0.03;
            fee = __max(fee, gm_CurrencyFactor(1, good));
            cm_SendExSellObject(-1, bank->id, good, value, ai_OwnPlayer, 0);
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x27;
            msg.fromId = -1;
            msg.flags = 1;
            msg.to = ((AICHARREC *)ai_Chars)[bank->owner].netId;
            msg.payment.bld = bank->id;
            msg.payment.player = player;
            msg.payment.amount = value - fee;
            cm_SendLongMessage((char *)&msg);
            sent = 1;
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
    }
    d2_CloseView(win);
    return sent;
}

int gm_SumListAmounts(void *list, unsigned char p); char * he_FindEvent(int n, ...); char * he_NextEvent(void); int ai_AmountMenu();
#define gm_SumListAmounts1 ((int (*)(void *))gm_SumListAmounts)  /* @original ai_MoneyExchangeWindow pushes only the list (the player byte is unused) */
int gm_BuildingClass(); unsigned gm_PermissionBits(unsigned p1, unsigned char *b, unsigned short *o, int c);
void init_LoadObjectScene();
char d3_DeflateScene(unsigned int flags); char d3_InflateScene(unsigned int flags); void init_SetCharacterAnimations(unsigned char *b);
void init_ShowBuildingScene(unsigned char *b, int slot); void init_FinishBuildingScene(unsigned char *b, unsigned short *o); 
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);
void snd_UnloadBuildingVoices(void); void init_AfterTownLoad(void); 
struct SBSAMPLE * sb_PlaySample();
void ai_AlchemistLabContact(struct AIBLD *rec, struct AIOBJ *room); void ai_CannonContact(struct AIBLD *rec, struct AIOBJ *room); void ai_CabinetContact(struct AIBLD *rec, struct AIOBJ *room); void ai_ProductionContact(int building, int arg);

/* 0x59d5e0, the money lender's window of the bank bank for its owner, "locations\geldleihe\geldleihe_Wechsel"
   (1557 B): the bank's currency rows (its object list, form 5, as in ai_ExchangeWindow) and the rows of the
   players (form 4: the money good of every named player slot of the four), each with a scroll pair and a count
   text; the texts 0x1900, 0x190a when the bank holds no money (gm_SumListAmounts), the buttons 0x1906 (the fees,
   ai_ExchangeFeesWindow) and 0x3fd6 (the exchange, ai_ExchangeWindow); every frame it looks for the bank's running payment
   event 0x27 and shows the messenger fee (0x1902) and the paid player's name (0x1901) or none (0x1903); while none
   runs, an amount picked in a bank row and a click on a player row open the money messenger (ai_MoneyMessengerWindow),
   which clears the picked amounts when it sent; the player rows are enabled only while an amount is picked */
void ai_MoneyExchangeWindow(AIBLD *bank)
{
    LCEXCHROW bankRows[16];
    LCEXCHROW playerRows[16];
    char text[8];
    AIJOB *payment;
    int feesButton;
    int exchangeButton;
    int buttons[8];
    int bankUp;
    int bankDown;
    int playerUp;
    int playerDown;
    int bankText;
    int playerText;
    int bankCount;
    int playerCount;
    int sel;
    int idx;
    int win;
    int i;

    sel = -1;
    for (i = 0; i < 16; i++) {
        bankRows[i].icon = -1;
        bankRows[i].good = 0;
        playerRows[i].icon = -1;
        playerRows[i].good = 0;
    }
    for (i = 0; i < 4; i++) {
        if (((GMPLAYERSLOT *)ai_PlayerSlots)[i].name[0] != 0) {
            playerRows[i].good = ((GMPLAYERSLOT *)ai_PlayerSlots)[i].moneyGood;
            playerRows[i].player = i;
        }
    }
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_Wechsel");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_OpenWindowFrame(d2_LastWindow, 0x73b, 0x73f);
    playerUp = d2_GetWinObj(win, 0, 2);
    playerDown = d2_GetWinObj(win, 0, 3);
    bankUp = d2_GetWinObj(win, 0, 1);
    bankDown = d2_GetWinObj(win, 0, 0);
    d2_GetWinObj(win, 0, 4);
    d2_SetForm(win, 0);
    bankText = d2_CreateWinTextS(d2_Objs[bankUp].x - d2_LastWindowPtr->x + 2,
                                 d2_Objs[bankUp].h - d2_LastWindowPtr->y + d2_Objs[bankUp].y + 2, ch_EmptyName,
                                 d2_LastWindow);
    playerText = d2_CreateWinTextS(d2_Objs[playerUp].x - d2_LastWindowPtr->x + 2,
                                   d2_Objs[playerUp].h - d2_LastWindowPtr->y + d2_Objs[playerUp].y + 2, ch_EmptyName,
                                   d2_LastWindow);
    d2_Objs[bankText].font = 0x54;
    d2_Objs[playerText].font = 0x54;
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0xe;
    d2_Wins[d2_LastWindow].padt = 0x18;
    d2_Wins[d2_LastWindow].padr = 0xe;
    d2_Wins[d2_LastWindow].padb = 0xe;
    d2_SetForm(win, 1);
    io_PrintText(0x1900);
    d2_SetForm(win, 3);
    sv_ButtonRow(d2_LastWindow, 0, buttons, 2, txt_Ptrs[0x1906], txt_Ptrs[0x3fd6]);
    feesButton = buttons[0];
    exchangeButton = buttons[1];
    if (gm_SumListAmounts1((void *)bank->objList) <= 0) {
        d2_SetForm(win, 5);
        io_PrintText(0x190a, bank);
    }
    ai_ShowExchangeRows(playerRows, win, 4, 0);
    for (i = 0; i < 16; i++)
        if (playerRows[i].icon != -1)
            d2_SetObjEnabled(playerRows[i].icon, 0);
    while (sv_RunFrame(ai_MoneyExchangeWindow, 0x677c7) != 0) {
        for (payment = (AIJOB *)he_FindEvent(1, 0, 0x27); payment != 0; payment = (AIJOB *)he_NextEvent())
            if (payment->bldId == bank->id)
                break;
        d2_SetForm(win, 2);
        io_PrintText("$C");
        io_PrintText(0x1902, bank->messengerFee);
        if (payment == 0)
            io_PrintText(0x1903);
        else
            io_PrintText(0x1901, ((GMPLAYERSLOT *)ai_PlayerSlots)[payment->payer].name);
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        d2_ClearPointerItems();
        if (payment == 0)
            sel = ai_FillExchangeRows(bank->objList, bankRows, win, 5, 1);
        idx = ai_ClickedExchangeRecord(playerRows);
        if (payment == 0 && idx != -1 && sel != -1) {
            d2_ClearPointerItems();
            if (ai_MoneyMessengerWindow(bank, bankRows[sel].good, bankRows[sel].value, playerRows[idx].player) != 0) {
                d2_EndInputEdit();
                for (i = 0; i < 16; i++)
                    if (bankRows[i].icon != -1)
                        d2_SetObjValue4(bankRows[i].icon, 0, 0, 0);
            }
        }
        for (i = 0; i < 16; i++) {
            if (playerRows[i].icon != -1) {
                if (sel != -1)
                    d2_SetObjEnabled(playerRows[i].icon, 1);
                else
                    d2_SetObjEnabled(playerRows[i].icon, 0);
            }
        }
        bankCount = ai_UsedRecordCount(bankRows);
        playerCount = ai_UsedRecordCount(playerRows);
        if (bankCount <= 4) {
            d2_EnableObject(bankUp, 0);
            d2_EnableObject(bankDown, 0);
            d2_EnableObject(bankText, 0);
        } else {
            d2_EnableObject(bankUp, 1);
            d2_EnableObject(bankDown, 1);
            d2_EnableObject(bankText, 1);
            itoa(bankCount, text, 10);
            d2_SetObjText(bankText, text);
        }
        if (playerCount <= 4) {
            d2_EnableObject(playerUp, 0);
            d2_EnableObject(playerDown, 0);
            d2_EnableObject(playerText, 0);
        } else {
            d2_EnableObject(playerUp, 1);
            d2_EnableObject(playerDown, 1);
            d2_EnableObject(playerText, 1);
            itoa(playerCount, text, 10);
            d2_SetObjText(playerText, text);
        }
        if (d2_ClickedId == 3 || d2_ClickedId == 4) {
            if (d2_PointerObj == playerUp) {
                d2_SetForm(win, 4);
                d2_ScrollWindowBy(0, -0x4c, d2_LastWindow);
            } else if (d2_PointerObj == playerDown) {
                d2_SetForm(win, 4);
                d2_ScrollWindowBy(0, 0x4c, d2_LastWindow);
            } else if (d2_PointerObj == bankUp) {
                d2_SetForm(win, 5);
                d2_ScrollWindowBy(0, -0x4c, d2_LastWindow);
            } else if (d2_PointerObj == bankDown) {
                d2_SetForm(win, 5);
                d2_ScrollWindowBy(0, 0x4c, d2_LastWindow);
            }
        }
        if (d2_ClickedId == 0) {
            if (d2_PointerObj == feesButton)
                ai_ExchangeFeesWindow(bank);
            else if (d2_PointerObj == exchangeButton)
                ai_ExchangeWindow(bank);
        }
    }
    d2_ClearPointerItems();
    d2_CloseView(win);
    sv_CameraFollow();
}

int gm_GetWorth(unsigned short id); void d2_ClearPaintbox(void);
int ai_ShowBuildingRows(AICHARREC *ch, int max, int *ctrls);  /* 0x5bd470, defined below */
int ai_ShowLoanRows(AICHARREC *ch, int max, CREDITLISTROW *rows, int win, int form);  /* 0x5bd330, defined below */

/* 0x59dc00, the money lender's info window "locations\geldleihe\geldleihe_info" on the character ch (771 B), the
   callback the fortune overview (ai_FortuneOverview) hands the amount menu: the paddings 0xc / 0x14 / 0xc / 0xc,
   the texts 0x18ed, 0x18ee (the character) and 0x18ef (his worth gm_GetWorth, his goods value gm_GetGoodsValue),
   the two page buttons 0x18f0 / 0x18f1 and the scroll list (0x206 / 0x167); the page shown is rebuilt after every
   change: page 1 the character's buildings (ai_ShowBuildingRows, or 0x18f2 when none), page 2 his loans as up to 9 credit
   rows (ai_ShowLoanRows, or 0x18f3), the loan rows are freed when page 1 is built; a right click closes it, the
   rows are freed at the end */
void ai_MoneyLenderInfoWindow(AICHARREC *ch)
{
    CREDITLISTROW rows[9];
    int down;
    int up;
    int bar;
    int page;
    int rebuild;
    int carriersButton;
    int loansButton;
    int worth;
    int value;
    int win;
    int i;

    rebuild = 1;
    page = 1;
    if (ch == 0)
        return;
    for (i = 0; i < 9; i++) {
        rows[i].child = -1;
        rows[i].pic = -1;
        rows[i].loan = 0;
    }
    worth = gm_GetWorth(ch->id);
    value = gm_GetGoodsValue(ch);
    win = d2_OpenForm(0, 0, "locations\\geldleihe\\geldleihe_info");
    d2_SetForm(win, 0);
    d2_Wins[d2_LastWindow].padl = 0xc;
    d2_Wins[d2_LastWindow].padt = 0x14;
    d2_Wins[d2_LastWindow].padr = 0xc;
    d2_Wins[d2_LastWindow].padb = 0xc;
    d2_SetForm(win, 1);
    io_PrintText(0x18ed);
    d2_SetForm(win, 4);
    io_PrintText(0x18ee, ch->id);
    io_PrintText(0x18ef, worth, value);
    d2_SetForm(win, 3);
    carriersButton = d2_GetWinObj(win, 3, io_PrintText(0x18f0));
    d2_SetForm(win, 5);
    loansButton = d2_GetWinObj(win, 5, io_PrintText(0x18f1));
    d2_SetForm(win, 0);
    sv_CreateScrollButtons(0x206, 0x167, d2_LastWindow, d2_GetViewWin(win, 2), 0x100, &down, &up, &bar);
    d2_CentreViewWindows(win);
    d2_ClickedId = -1;
    d2_PointerObj = -1;
    do {
        if (rebuild) {
            rebuild = 0;
            for (i = 0; i < 9 && page == 1; i++) {
                if (rows[i].child != -1) {
                    d2_RemoveWindow(rows[i].child);
                    d2_RemoveObj(rows[i].pic);
                    rows[i].child = -1;
                    rows[i].pic = -1;
                    rows[i].loan = 0;
                }
            }
            d2_SetForm(win, 2);
            d2_ClearPaintbox();
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            if (page == 1) {
                if (ai_ShowBuildingRows(ch, 0, 0) <= 0)
                    io_PrintText(0x18f2, ch->id);
            } else if (page == 2) {
                if (ai_ShowLoanRows(ch, 9, rows, win, 2) == 0)
                    io_PrintText(0x18f3, ch->id);
            }
        }
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == carriersButton) {
                if (page != 1) {
                    page = 1;
                    rebuild = 1;
                }
            } else if (d2_PointerObj == loansButton) {
                if (page != 2) {
                    page = 2;
                    rebuild = 1;
                }
            }
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_MoneyLenderInfoWindow, 0x677c7) != 0);
    for (i = 0; i < 9; i++) {
        if (rows[i].child != -1) {
            d2_RemoveWindow(rows[i].child);
            d2_RemoveObj(rows[i].pic);
            rows[i].child = -1;
            rows[i].pic = -1;
            rows[i].loan = 0;
        }
    }
    d2_CloseView(win);
}
/* 0x59df10, the fortune overview of the money lender (the contact_VERMOEGENSUEBERSICHT of 0x59df80) through
   the amount menu 0x5c1130: the config (mode 0x400, kind 6, the filter gypsyFilter, the rest cleared), the
   text 0x18ec and the callback 0x59dc00 (the info window), then the camera follows; bld is not read */

void ai_FortuneOverview(AIBLD *bld)
{
    AMOUNTCFG cfg;
    char text[0x40];

    memset(&cfg, 0, sizeof(cfg));
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    txt_Format(text, 0x18ec);
    ai_AmountMenu(&cfg, text, 0, (void *)ai_MoneyLenderInfoWindow);
    sv_CameraFollow();
}
/* 0x59df80, the money lender contact listener (288 B): the pump 0x5309a0 (0x59df80, 0x67fff) and the lookups
   "contact_GELDWECHSEL" (text 0x18f4) / "contact_KREDITE" (text 0x18c8), with the building permission 0x200
   "contact_MEISTERBRIEF" (ai_ContactTextMasterLetter) and "contact_VERMOEGENSUEBERSICHT" (text 0x1fa4); the
   master letter opens 0x5c3200, the overview 0x59df10; the player's own building (bld->owner == ai_PlayerId)
   opens 0x59d5e0 (money exchange) / 0x59bad0 (credits), another player's 0x59c700 / 0x59b160 */

void ai_MoneyLenderContact(AIBLD *bld, int room)
{
    int meister = 0;
    int overview = 0;
    int wechsel;
    int kredite;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_MoneyLenderContact, 0x67fff) != 0) {
        wechsel = sv_RegisterNamePlate("contact_GELDWECHSEL", txt_Ptrs[0x18f4], 0x10);
        kredite = sv_RegisterNamePlate("contact_KREDITE", txt_Ptrs[0x18c8], 0x10);
        if ((ai_BuildingPerms & 0x200) != 0) {
            meister = sv_RegisterNamePlate("contact_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            overview = sv_RegisterNamePlate("contact_VERMOEGENSUEBERSICHT", txt_Ptrs[0x1fa4], 0x16);
        }
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)bld);
            else if (ai_ClickedObject == overview)
                ai_FortuneOverview(bld);
            if (ai_ClickedObject != 0) {
                if (bld->owner == ai_PlayerId) {
                    if (ai_ClickedObject == wechsel)
                        ai_MoneyExchangeWindow(bld);
                    else if (ai_ClickedObject == kredite)
                        ai_CreditGiverWindow(bld);
                } else if (ai_ClickedObject == wechsel) {
                    ai_ExchangeWindow((int)bld);
                } else if (ai_ClickedObject == kredite) {
                    ai_CreditListWindow((AIBLD *)bld);
                }
            }
        }
    }
}
/* 0x59e0a0, switches the current building room (the record 0x6f2308 + 0x29) to the index-th entry of the
   building 0x4ffe90 (0x6f2308 + 0x5d, 1, 4, 2), or to the word room when index == -1; 0x6f22a0 = the
   0x6f229c - 0x6f22a4 + 1 count, the room matrices 0x43b790 / 0x43b860 (0xfc2dec + 0 / + 0x10), the counters
   0x6f22cc / 0x6f22d0 = 0 and the sound "RaumWechseln_s" (0x426fa0) */

void ai_SwitchRoom(int index, unsigned short room)
{
    unsigned short *p;
    int i;

    if (index != -1) {
        p = (unsigned short *)gm_Query(((AIBLD *)ai_PlayerBuilding)->objList, 1, 4, 2);
        if (p == 0)
            return;
        for (i = 1; i < index; i++) {
            p = (unsigned short *)gm_QueryNext();
            if (p == 0)
                return;
        }
        ((AIBLD *)ai_PlayerBuilding)->mainRoom = *p;
    } else
        ((AIBLD *)ai_PlayerBuilding)->mainRoom = room;
    sv_PopRequest = (sv_FrameDepth - sv_FrameBase) + 1;
    d3_SetObjectPos(d3_Camera, sv_ViewPos);
    d3_SetObjectRot(d3_Camera, sv_ViewRot);
    sv_BorderViewX = 0;
    sv_BorderViewY = 0;
    sb_PlaySample(snd_RoomBank, "RaumWechseln_s", 1, 0x3f, 0x7f, 0, 0);
}
/* 0x59e170, whether the building b is open now: its type's class (AIBTYPE.buildingClass) is looked up in
   ai_OpeningHours; a class without a row is always open, else the game clock's hour must lie in [open, close).
   The row's hours go to *open / *close when they are not null */

int ai_IsBuildingOpen(AIBLD *b, unsigned int *open, unsigned int *close)
{
    AIBTYPE *type;
    int i;

    type = &((AIBTYPE *)ai_BTypes)[b->type];
    for (i = 0; ai_OpeningHours[i].buildingClass != 0; i++) {
        if (type->kind == ai_OpeningHours[i].buildingClass) {
            if (open != 0)
                *open = ai_OpeningHours[i].open;
            if (close != 0)
                *close = ai_OpeningHours[i].close;
            if (ai_Time.hour >= ai_OpeningHours[i].open && ai_Time.hour < ai_OpeningHours[i].close)
                return 1;
            return 0;
        }
    }
    return 1;
}
/* 0x59e210, whether the player may enter the building b: never while its flag 0x5b bit 2 is set; always in the
   flag 0x80 game mode, for a held player at a building of class 4 / 6, with the permission bit 0x200 or at the
   player's own building; otherwise the permission bit 1 and the opening hours (ai_IsBuildingOpen) must allow it.
   A closed building shows the message ai_TextBuildingClosed: not yet open (0x2056, 0x2059 for the building
   classes 3 / 5) or already closed (0x2057 / 0x205a), and its hours (0x2058) */

int ai_MayEnterBuilding(AIBLD *b)
{
    AIBTYPE *type;
    int cls;
    unsigned int perm;
    unsigned int open;
    unsigned int close;
    char hours[0x80];
    char title[0x80];
    char msg[0x400];

    type = &((AIBTYPE *)ai_BTypes)[b->type];
    cls = gm_BuildingClass(b->type);
    perm = gm_PermissionBits(ai_PlayerId, (unsigned char *)b, 0, 0);
    if ((b->flags5aw & 0x400) != 0)
        return 0;
    if ((ai_GameFlags & 0x80) != 0 ||
        (((AICHARREC *)ai_Chars)[ai_PlayerId].heldRounds > 0 && (type->kind == 6 || type->kind == 4)) ||
        (perm & 0x200) != 0 || b->owner == ai_PlayerId)
        return 1;
    if ((perm & 1) == 0)
        return 0;
    if (ai_IsBuildingOpen(b, &open, &close) != 0)
        return 1;
    strcpy(title, ai_TextBuildingClosed);
    if ((int)ai_Time.hour < (int)open) {
        if (cls == 3 || cls == 5)
            txt_Format(msg, 0x2059, b);
        else
            txt_Format(msg, 0x2056, b->owner, b);
    } else {
        if (cls == 3 || cls == 5)
            txt_Format(msg, 0x205a, b);
        else
            txt_Format(msg, 0x2057, b->owner, b);
    }
    txt_Format(hours, 0x2058, open, close);
    strcat(msg, hours);
    sv_PanelFlag = 0;
    sv_MessageBoxGreen(title, msg, 4);
    return 0;
}
/* the camera view a building visit saves and restores: sv_ViewPos / sv_ViewRot copied field by field (the
   position padded to 16 bytes like the globals, the rotation not: the frames of the three visits) */
typedef struct SVSAVEDVIEW {
    D3VECTOR4 pos;
    D3VECTOR rot;
} SVSAVEDVIEW;

/* 0x59e420, enters the building b at its room mainRoom (0: from the town): when ai_MayEnterBuilding refuses, nothing
   happens; else the town view is saved, the entry sound played ("EigenesGebaeudeBetreten_s" at the player's own
   building, "FremdesGeschaeftBetreten_s" at another's) and the building's voices loaded; then per frame the
   room b->objList holds for b->mainRoom (gm_Query) is shown: a room of another universe is loaded (its scene
   reloaded when dirty), selected and set up for the building (the camera at its saved place, at a foreign
   building of class 1 / 8 at its view 5), the scene's event 4 fired, and the room's handler runs (the switch on
   its kind: the contact listeners and windows of this unit); after it the scene's event 5, the waiting scripts and
   the town scene's sounds. The loop ends when sv_RunFrame stops it: the selection is cleared, the town scene and
   view restored and "GebaeudeVerlassen_s" played */

void ai_EnterBuilding(AIBLD *b, int mainRoom)
{
    int scene;
    SVSAVEDVIEW view;
    int lastEvent;
    AIOBJ *room;
    signed char universe;
    int cls;
    EVTSCRIPT *ev;

    lastEvent = -1;
    if (ai_MayEnterBuilding(b) == 0) {
        sv_PanelFlag = 0;
        return;
    }
    sv_ResetScreenClip();
    sv_RunFrame(ai_EnterBuilding, 0x20007);
    sv_SceneEventTime = 0;
    sv_ResetSelection();
    sv_ClearSelection();
    scene = d3_CurrentScene;
    sv_LastRoom = 0;
    b->mainRoom = (unsigned short)mainRoom;
    view.pos.x = sv_ViewPos[0];
    view.pos.y = sv_ViewPos[1];
    view.pos.z = sv_ViewPos[2];
    view.rot.x = sv_ViewRot[0];
    view.rot.y = sv_ViewRot[1];
    view.rot.z = sv_ViewRot[2];
    if (mainRoom == 0)
        ai_PlayerBuilding = 0;
    if (b->owner == ai_PlayerId)
        sb_PlaySample(snd_RoomBank, "EigenesGebaeudeBetreten_s", 1, 0x3f, 0x7f, 0, 0);
    else
        sb_PlaySample(snd_RoomBank, "FremdesGeschaeftBetreten_s", 1, 0x3f, 0x7f, 0, 0);
    strcpy(ch_StepSample, "normal_s");
    gm_Query((void *)b->objList, 1, 0, b->mainRoom);
    snd_LoadBuildingVoices((unsigned char *)b);
    do {
        sv_FrameBase = sv_FrameDepth + 1;
        room = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, b->mainRoom);
        if (room != 0) {
            universe = init_ObjectUniverse[room->type];
            if (universe != -2) {
                if (universe == -1 || d3_Scenes[universe].dirty != 0) {
                    d2_CutsceneActive = 1;
                    sv_RunFrame(0, 0x603c7);
                    sv_RunFrame(0, 0x603c7);
                    init_LoadObjectScene(room->type);
                    d2_CutsceneActive = 0;
                }
                if (d3_CurrentScene != 0)
                    d3_DeflateScene(3);
                ai_PlayerBuilding = (char *)b;
                sv_PanelShown = (unsigned short *)room;
                evt_NoGroup = room->objId;
                ch_CurrentGroup = b->id;
                ch_CurrentScriptGroup = room->objId;
                b->mainRoom = room->type;
                d3if_SelectScene(init_ObjectUniverse[room->type], 0);
                d3_InflateScene(3);
                d3_ForEachObject2(d3_DefaultScene, 0, 0xe0, d3s_ResetShadows);
                init_SetCharacterAnimations((unsigned char *)b);
                init_ShowBuildingScene((unsigned char *)ai_PlayerBuilding, ((AIBLD *)ai_PlayerBuilding)->mainRoom);
                init_FinishBuildingScene((unsigned char *)ai_PlayerBuilding, sv_PanelShown);
                d3_SetObjectPos(d3_Camera, &d3_Camera->savedPos);
                d3_SetObjectRot(d3_Camera, &d3_Camera->savedRot);
                if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[4].fn != 0)
                    d3_DefaultScene->events[4].fn(d3_DefaultScene, 4, d3_DefaultScene->events[4].name, 0);
                lastEvent = evt_LastId;
                if (b->owner != ai_PlayerId) {
                    cls = gm_BuildingClass(b->type);
                    if ((cls == 1 || cls == 8) &&
                        d3_VecEqual(&d3_Camera->views[5].pos, &d3_Origin, 0.1f) == 0 &&
                        d3_VecEqual(&d3_Camera->views[5].dir, &d3_Origin, 0.1f) == 0) {
                        d3_SetObjectPos(d3_Camera, &d3_Camera->views[5].pos);
                        d3_SetObjectRot(d3_Camera, &d3_Camera->views[5].dir);
                    }
                }
                sv_ViewPos[0] = d3_Camera->pos.x;
                sv_ViewPos[1] = d3_Camera->pos.y;
                sv_ViewPos[2] = d3_Camera->pos.z;
                sv_ViewRot[0] = d3_Camera->rotation.x;
                sv_ViewRot[1] = d3_Camera->rotation.y;
                sv_ViewRot[2] = d3_Camera->rotation.z;
                sv_BorderViewX = 0;
                sv_BorderViewY = 0;
            }
            ai_CampaignStep(0x19, (int)room, 0, 0);
            switch (room->type) {
            case 0x13:
                ai_StudyContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0x14:
                ai_RobberContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x16:
                ai_WineCellarContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x15:
                ai_RhetoricContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x17:
                ai_FeastContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0x18:
                ai_MistressContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x1f:
                ai_MapTableContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x74:
                ai_SmithyContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0x85:
                ai_CarpenterContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0xcf:
                ai_MixingContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0xd6:
                ai_IdleContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0xdd:
                ai_AquariumContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x9b:
                ai_StonecutterContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0xa5:
                ai_MoneyLenderContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0xe5:
            case 0xe6:
                ai_ChurchListener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0xf0:
                ai_ChurchContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0xb7:
                ai_DungeonContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0xba:
                ai_DungeonBribeListener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0xbb:
                ai_DungeonEmptyListener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x54:
                ai_ThievesGuildListener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x4d:
                ai_ThievesGuildEmptyListener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x5c:
                ai_StorageContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x60:
                ai_TrainingContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x65:
                ai_KidnapContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0xf2:
                ai_ConfessionListener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x118:
                ai_PerfumeryContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x120:
                ai_TavernContact((AIBLD *)ai_PlayerBuilding, room);
                break;
            case 0x12e:
                ai_BreweryContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0x10e:
                ai_TownHallContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x110:
                ai_LawBookContact((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0x114:
                ai_TownCashContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x137:
                ai_GuildLevel2Listener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0xf7:
                ai_ProductionWriteContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x138:
                ai_GuildLevel1Listener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x139:
                ai_GuildLevel3Listener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x145:
                ai_TrainingRoomListener((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x146:
                ai_CityWatchListener((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0x14c:
                ai_GuildHallListener((AIBLD *)ai_PlayerBuilding, (int)room);
                break;
            case 0x24b:
                ai_AlchemistLabContact((AIBLD *)ai_PlayerBuilding, room);
                break;
            default:
                ai_PumpFrames((int)room);
                break;
            case 0x29e:
                ai_ProductionContact((int)ai_PlayerBuilding, (int)room);
                break;
            case 0x266:
                ai_CabinetContact((AIBLD *)ai_PlayerBuilding, room);
                break;
            case 0x256:
                ai_CannonContact((AIBLD *)ai_PlayerBuilding, room);
                break;
            }
            ai_CampaignStep(0x1a, (int)room, 0, 0);
            sv_LastRoom = room;
            if (lastEvent != -1) {
                ev = evt_FindById(lastEvent);
                if (ev != 0) {
                    evt_Stop(ev);
                    sv_RunFramesUntilStopped(ev->id);
                }
            }
            if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[5].fn != 0)
                d3_DefaultScene->events[5].fn(d3_DefaultScene, 5, d3_DefaultScene->events[5].name, 0);
            if (evt_LastId != -1)
                sv_RunFramesUntilStopped(evt_LastId);
            init_LeaveTownScene(b);
            ai_FrameWait = 1;
            if (ai_PlayerBuilding == 0) {
                sv_PopRequest = 1;
                d3_DeflateScene(3);
            }
        }
    } while (sv_RunFrame(ai_EnterBuilding, 0x67fff) != 0);
    sv_ResetSelection();
    sv_ClearSelection();
    sv_SelectedChar = 0;
    snd_UnloadBuildingVoices();
    d3if_SelectScene(scene, 0);
    evt_NoGroup = 0;
    ch_CurrentGroup = -1;
    ch_CurrentScriptGroup = -1;
    ai_PlayerBuilding = 0;
    sv_PanelShown = 0;
    init_AfterTownLoad();
    sv_ViewPos[0] = view.pos.x;
    sv_ViewPos[1] = view.pos.y;
    sv_ViewPos[2] = view.pos.z;
    sv_ViewRot[0] = view.rot.x;
    sv_ViewRot[1] = view.rot.y;
    sv_ViewRot[2] = view.rot.z;
    sb_PlaySample(snd_RoomBank, "GebaeudeVerlassen_s", 1, 0x3f, 0x7f, 0, 0);
}

int init_BuildingSceneUniverse(); void init_LoadBuildingScene(int k); void d3fl_SetDetail(unsigned int n); void veg_HidePlants();
void ai_GraveyardContact(char *rec); void ai_GypsyProductionContact(int building); void ai_GraveyardFogContact(char *rec);
void ai_GraveyardSeanceContact(char *rec); void ai_GypsyContact(int building); void ai_TheatreContact(int building); void ai_CurseContact(int buildingArg);
void ai_GraveyardTradeContact(char *rec);

/* 0x59ee40, enters a building that has a universe of its own (the kontors, the criminal's and far trader's houses,
   the graveyards, the gypsy camps): when ai_MayEnterBuilding refuses, nothing happens; else the town view is saved,
   the building's universe (init_BuildingSceneUniverse) takes the town's sky and, unless there is none (0), is
   loaded when new or dirty, selected and set up: the room is the building's object of kind 6 (gm_Query 0xfd, else
   kind 2), the terrain's level of detail follows the floor option, the scene's event 4 is fired and the camera put
   at its saved place (the player's own building and the class 0x1c) or at its view 5. The building's handler runs
   (the switch on its kind, ai_PumpFrames for the others); after it the universe gives the sky back, the waiting
   scripts are stopped, the scene's event 5 fired, the town scene and view restored */
void ai_EnterOwnBuilding(AIBLD *b)
{
    int lastEvent;
    SVSAVEDVIEW view;
    int scene;
    int universe;
    AIOBJ *room;
    int cls;
    AIBTYPE *type;
    EVTSCRIPT *ev;

    lastEvent = -1;
    if (ai_MayEnterBuilding(b) == 0) {
        sv_PanelFlag = 0;
        return;
    }
    scene = d3_CurrentScene;
    sv_FrameBase = sv_FrameDepth + 1;
    sv_SceneEventTime = 0;
    sv_ResetSelection();
    sv_ClearSelection();
    view.pos.x = sv_ViewPos[0];
    view.pos.y = sv_ViewPos[1];
    view.pos.z = sv_ViewPos[2];
    view.rot.x = sv_ViewRot[0];
    view.rot.y = sv_ViewRot[1];
    view.rot.z = sv_ViewRot[2];
    strcpy(ch_StepSample, "normal_s");
    universe = init_BuildingSceneUniverse((unsigned char *)b);
    D3IF_SCENE(universe).sky = D3IF_SCENE(0).sky;
    if (universe != 0) {
        snd_LoadBuildingVoices((unsigned char *)b);
        if (universe == -1 || d3_Scenes[universe].dirty != 0) {
            d2_CutsceneActive = 1;
            sv_RunFrame(0, 0x603c7);
            sv_RunFrame(0, 0x603c7);
            init_LoadBuildingScene(b->type);
            d2_CutsceneActive = 0;
        }
        ai_FallbackBuilding = (char *)b;
        evt_NoGroup = b->id;
        ch_CurrentGroup = b->id;
        room = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0xfd);
        if (room == 0)
            room = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 4, 2);
        ch_CurrentScriptGroup = room->objId;
        d3if_SelectScene(universe, 0);
        d3_InflateScene(3);
        if (d3_Terrain != 0) {
            if (ai_GfxFloorLod == 2) {
                ((D3FLMAP *)d3_Terrain)->lod = 1;
                d3fl_SetDetail(0x40);
            } else {
                ((D3FLMAP *)d3_Terrain)->lod = 2;
                d3fl_SetDetail(0x40);
            }
        }
        init_ShowBuildingScene((unsigned char *)ai_FallbackBuilding, room->type);
        init_FinishBuildingScene((unsigned char *)ai_FallbackBuilding, (unsigned short *)room);
        if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[4].fn != 0)
            d3_DefaultScene->events[4].fn(d3_DefaultScene, 4, d3_DefaultScene->events[4].name, 0);
        lastEvent = evt_LastId;
        if (b->owner != ai_PlayerId) {
            cls = gm_BuildingClass(b->type);
            type = &((AIBTYPE *)ai_BTypes)[b->type];
            if (cls == 2) {
                if (d3_VecEqual(&d3_Camera->views[5].pos, &d3_Origin, 0.1f) == 0 &&
                    d3_VecEqual(&d3_Camera->views[5].dir, &d3_Origin, 0.1f) == 0) {
                    d3_SetObjectPos(d3_Camera, &d3_Camera->views[5].pos);
                    d3_SetObjectRot(d3_Camera, &d3_Camera->views[5].dir);
                }
            } else if (type->kind == 0x1c) {
                d3_SetObjectPos(d3_Camera, &d3_Camera->savedPos);
                d3_SetObjectRot(d3_Camera, &d3_Camera->savedRot);
            } else if (d3_VecEqual(&d3_Camera->views[5].pos, &d3_Origin, 0.1f) == 0 &&
                       d3_VecEqual(&d3_Camera->views[5].dir, &d3_Origin, 0.1f) == 0) {
                d3_SetObjectPos(d3_Camera, &d3_Camera->views[5].pos);
                d3_SetObjectRot(d3_Camera, &d3_Camera->views[5].dir);
            }
        } else {
            d3_SetObjectPos(d3_Camera, &d3_Camera->savedPos);
            d3_SetObjectRot(d3_Camera, &d3_Camera->savedRot);
        }
        sv_ViewPos[0] = d3_Camera->pos.x;
        sv_ViewPos[1] = d3_Camera->pos.y;
        sv_ViewPos[2] = d3_Camera->pos.z;
        sv_ViewRot[0] = d3_Camera->rotation.x;
        sv_ViewRot[1] = d3_Camera->rotation.y;
        sv_ViewRot[2] = d3_Camera->rotation.z;
        sv_BorderViewX = 0;
        sv_BorderViewY = 0;
    }
    switch (b->type) {
    case 0x20:
        ai_KontorContact((AIBLD *)ai_FallbackBuilding);
        break;
    case 0x1e:
        ai_WoodKontorContact((AIBLD *)ai_FallbackBuilding);
        veg_HidePlants(b->plants);
        break;
    case 0x1f:
        ai_StoneKontorContact((AIBLD *)ai_FallbackBuilding);
        break;
    case 0x27:
        ai_CriminalContact((AIBLD *)ai_FallbackBuilding);
        break;
    case 0x47:
        ai_FarTradeContact((int)ai_FallbackBuilding);
        break;
    case 0x48:
    case 0x49:
    case 0x4a:
        ai_GraveyardContact(ai_FallbackBuilding);
        break;
    case 0x4e:
    case 0x4f:
    case 0x50:
        ai_GypsyProductionContact((int)ai_FallbackBuilding);
        break;
    default:
        ai_PumpFrames(0);
        break;
    }
    if (universe != -1) {
        D3IF_SCENE(universe).sky = 0;
        d3_Sky = 0;
    }
    sv_ResetSelection();
    sv_ClearSelection();
    sv_BorderViewX = 0;
    sv_BorderViewY = 0;
    if (lastEvent != -1) {
        ev = evt_FindById(lastEvent);
        if (ev != 0) {
            evt_Stop(ev);
            sv_RunFramesUntilStopped(ev->id);
        }
    }
    if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[5].fn != 0)
        d3_DefaultScene->events[5].fn(d3_DefaultScene, 5, d3_DefaultScene->events[5].name, 0);
    if (evt_LastId != -1)
        sv_RunFramesUntilStopped(evt_LastId);
    snd_UnloadBuildingVoices();
    init_LeaveTownScene(ai_FallbackBuilding);
    d3_DeflateScene(3);
    d3if_SelectScene(scene, 0);
    sv_SelectedChar = 0;
    ai_FallbackBuilding = 0;
    evt_NoGroup = 0;
    ch_CurrentGroup = -1;
    ch_CurrentScriptGroup = -1;
    init_AfterTownLoad();
    sv_ViewPos[0] = view.pos.x;
    sv_ViewPos[1] = view.pos.y;
    sv_ViewPos[2] = view.pos.z;
    sv_ViewRot[0] = view.rot.x;
    sv_ViewRot[1] = view.rot.y;
    sv_ViewRot[2] = view.rot.z;
}
/* 0x59f410, runs the room object room of the building the player is in (ai_FallbackBuilding, entered through
   ai_EnterOwnBuilding): the town view and the border scroll are saved; a room with a universe of its own is loaded
   (when new or dirty), becomes the building's mainRoom, is selected and set up (the scene's event 4, the camera at
   its saved place); the room's handler runs with the ambient sounds off (the switch on its kind, ai_PumpFrames for
   the others); after it the saved script is stopped, the scene's event 5 fired, the building goes back to its room
   0xfd (gm_Query: its object of kind 6) and the view and border scroll are restored */

void ai_RunObjectRoom(AIOBJ *room)
{
    int scene;
    SVSAVEDVIEW view;
    int lastEvent;
    signed char universe;
    int borderX;
    int borderY;
    EVTSCRIPT *ev;
    AIOBJ *home;

    lastEvent = -1;
    scene = d3_CurrentScene;
    sv_PanelShown = (unsigned short *)room;
    evt_NoGroup = room->objId;
    ch_CurrentScriptGroup = room->objId;
    sv_FrameBase = sv_FrameDepth + 1;
    sv_SceneEventTime = 0;
    view.pos.x = sv_ViewPos[0];
    view.pos.y = sv_ViewPos[1];
    view.pos.z = sv_ViewPos[2];
    view.rot.x = sv_ViewRot[0];
    view.rot.y = sv_ViewRot[1];
    view.rot.z = sv_ViewRot[2];
    sv_ResetSelection();
    sv_ClearSelection();
    universe = init_ObjectUniverse[room->type];
    if (universe != -2) {
        if (universe == -1 || d3_Scenes[universe].dirty != 0) {
            d2_CutsceneActive = 1;
            init_LoadObjectScene(room->type);
            d2_CutsceneActive = 0;
        }
        ((AIBLD *)ai_FallbackBuilding)->mainRoom = room->type;
        d3if_SelectScene(init_ObjectUniverse[room->type], 0);
        d3_InflateScene(3);
        init_ShowBuildingScene((unsigned char *)ai_FallbackBuilding, room->type);
        init_FinishBuildingScene((unsigned char *)ai_FallbackBuilding, (unsigned short *)room);
        if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[4].fn != 0)
            d3_DefaultScene->events[4].fn(d3_DefaultScene, 4, d3_DefaultScene->events[4].name, 0);
        lastEvent = evt_LastId;
        d3_SetObjectPos(d3_Camera, &d3_Camera->savedPos);
        d3_SetObjectRot(d3_Camera, &d3_Camera->savedRot);
        sv_ViewPos[0] = d3_Camera->pos.x;
        sv_ViewPos[1] = d3_Camera->pos.y;
        sv_ViewPos[2] = d3_Camera->pos.z;
        sv_ViewRot[0] = d3_Camera->rotation.x;
        sv_ViewRot[1] = d3_Camera->rotation.y;
        sv_ViewRot[2] = d3_Camera->rotation.z;
        borderX = sv_BorderViewX;
        borderY = sv_BorderViewY;
        sv_BorderViewX = 0;
        sv_BorderViewY = 0;
    }
    snd_AmbientOn = 0;
    switch (room->type) {
    case 0x2d:
        ai_BookContact(ai_FallbackBuilding);
        break;
    case 0x33:
        ai_MercenaryBookContact(ai_FallbackBuilding);
        break;
    case 0x2c:
        ai_UpgradeContact((int)ai_FallbackBuilding);
        break;
    case 0x37:
        ai_MiningContact(ai_FallbackBuilding);
        break;
    case 0x107:
        ai_InformationContact((int)ai_FallbackBuilding);
        break;
    case 0x109:
        ai_UpgradeContact((int)ai_FallbackBuilding);
        break;
    case 0x205:
        ai_GraveyardTradeContact(ai_FallbackBuilding);
        break;
    case 0x211:
        ai_GraveyardFogContact(ai_FallbackBuilding);
        break;
    case 0x21b:
        ai_GraveyardSeanceContact(ai_FallbackBuilding);
        break;
    case 0x276:
        ai_GypsyContact((int)ai_FallbackBuilding);
        break;
    case 0x28a:
        ai_CurseContact((int)ai_FallbackBuilding);
        break;
    case 0x27f:
        ai_TheatreContact((int)ai_FallbackBuilding);
        break;
    default:
        ai_PumpFrames((int)room);
        break;
    }
    snd_AmbientOn = 1;
    sv_ResetSelection();
    sv_ClearSelection();
    sv_ReleaseNamePlates();
    sv_SelectedChar = 0;
    if (lastEvent != -1) {
        ev = evt_FindById(lastEvent);
        if (ev != 0) {
            evt_Stop(ev);
            sv_RunFramesUntilStopped(ev->id);
        }
    }
    if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[5].fn != 0)
        d3_DefaultScene->events[5].fn(d3_DefaultScene, 5, d3_DefaultScene->events[5].name, 0);
    if (evt_LastId != -1)
        sv_RunFramesUntilStopped(evt_LastId);
    d3_DeflateScene(3);
    d3if_SelectScene(scene, 0);
    sv_PanelShown = 0;
    ((AIBLD *)ai_FallbackBuilding)->mainRoom = 0xfd;
    evt_NoGroup = 0;
    ch_CurrentGroup = ((AIBLD *)ai_FallbackBuilding)->id;
    home = (AIOBJ *)gm_Query((void *)((AIBLD *)ai_FallbackBuilding)->objList, 2, 6, 0, 0xfd);
    ch_CurrentScriptGroup = home->objId;
    sv_FrameBase = sv_FrameDepth;
    sv_BorderViewY = borderY;
    sv_ViewPos[0] = view.pos.x;
    sv_ViewPos[1] = view.pos.y;
    sv_ViewPos[2] = view.pos.z;
    sv_BorderViewX = borderX;
    sv_ViewRot[0] = view.rot.x;
    sv_ViewRot[1] = view.rot.y;
    sv_ViewRot[2] = view.rot.z;
}
typedef struct LCSALEROW {          /* a row of ai_BuyBuildingWindow's list: a building offered for sale */
    int control;                    /* 0x0, the row's text control (d2_GetWinObj) */
    AIJOB *sale;                    /* 0x4, the building's sale event (kind 0x1b) */
    AIBLD *bld;                     /* 0x8, the building */
    int flags;                      /* 0xc, its flags5aw when the row was built (a change rebuilds the list) */
} LCSALEROW;

/* 0x59f9a0, the town hall's list of the buildings for sale "locations\Rathaus\Rathaus_Geb_Info": every building of
   the query (1, 6) with the flag 2 of flags5a and a running sale event 0x1b gets a row (text 0x17f5: the
   type's name 0x427 + kind, the owner, the building, the price); the list is rebuilt when a row's flags change.
   A click on another player's building that the player can pay (sv_CheckGoodAmount) selects it on the panel;
   the right button leaves */

void ai_BuyBuildingWindow(void)
{
    int rebuild;
    LCSALEROW rows[128];
    int win;
    int scroll;
    int n;
    int i;
    AIBLD *b;
    AIJOB *sale;

    rebuild = 1;
    n = 0;
    win = d2_OpenForm(0, 0, "locations\\Rathaus\\Rathaus_Geb_Info");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 2);
    scroll = d2_LastWindow;
    d2_SetForm(win, 0);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, scroll, 0x767);
    d2_SetPointerSprite(0, 0);
    d2_Wins[scroll].visible = 0x5a;
    do {
        if (rebuild != 0) {
            d2_SetForm(win, 1);
            io_PrintText(0x17f6);
            d2_SetForm(win, 2);
            io_PrintText("$C");
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            n = 0;
            for (b = (AIBLD *)gm_BuildQuery(1, 6); b != 0; b = (AIBLD *)gm_BuildNext()) {
                if ((b->flags5a & 2) == 0)
                    continue;
                for (sale = (AIJOB *)he_FindEvent(1, 0, 0x1b); sale != 0; sale = (AIJOB *)he_NextEvent())
                    if (sale->bldId == b->id)
                        break;
                if (sale == 0)
                    continue;
                rows[n].sale = sale;
                rows[n].control = d2_GetWinObj(win, 2, io_PrintText(0x17f5, b->type + 0x427, b->owner, b, sale->salePrice));
                rows[n].bld = b;
                rows[n].flags = b->flags5aw;
                n++;
            }
            rebuild = 0;
        }
        for (i = 0; i < n; i++) {
            if (rows[i].flags != rows[i].bld->flags5aw) {
                rebuild = 1;
                break;
            }
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
            sv_CameraFollow();
        }
        if (d2_ClickedId != -1) {
            for (i = 0; i < n; i++) {
                if (d2_PointerObj == rows[i].control && rows[i].bld->owner != ai_PlayerId &&
                    sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], rows[i].sale->salePrice, ai_OwnPlayer) != 0)
                    sv_SelectPanelObject(rows[i].bld, -1, ch_EmptyName);
            }
        }
    } while (sv_RunFrame(ai_BuyBuildingWindow, 0x677c7) != 0);
    d2_CloseView(win);
}
/* 0x59fbf0, the guild cash window "locations\zunfthaus\gildenkasse" (165 B): the room 0x4ffe90 (bld->objList,
   2, 6, 0, 0x115), its money 0x509e30 (room->list, the player byte); the window, the text 0x1a0f (the money);
   the pump 0x5309a0 (0x59fbf0, 0x677c7), 0x52d7a0 and 0x4758f0; a right-button release (dm_L.rup) pops the frame */

void ai_GuildCashWindow(AIBLD *bld)
{
    AIOBJ *room;
    int money;
    int win;

    room = (AIOBJ *)gm_Query(bld->objList, 2, 6, 0, 0x115);
    money = gm_SumListAmounts(room->list, ai_OwnPlayer);
    win = d2_OpenForm(0, 0, "locations\\zunfthaus\\gildenkasse");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x1a0f, money);
    while (sv_RunFrame(ai_GuildCashWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
/* 0x59fca0, the town cash office listener (133 B): with the player's office (the character record's officeKind,
   0x770ce6 of the first record) set, the pump 0x5309a0 (0x59fca0, 0x67fff) looks up "ob_STADTKASSE" (text
   0x1a0e, class 0x16) and opens 0x59fbf0 */

void ai_TownCashContact(AIBLD *bld, int room)
{
    int c;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_TownCashContact, 0x67fff) != 0) {
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].officeKind != 0)
            c = sv_RegisterNamePlate("ob_STADTKASSE", txt_Ptrs[0x1a0e], 0x16);
        else
            c = 0;
        if (ai_ClickedObject != 0 && ai_ClickedObject == c)
            ai_GuildCashWindow(bld);
    }
}
void ai_OpenAmountKindMenu(void);
void ai_OpenAgendaMenu(void);
/* 0x59fd30, the Rathaus contact listener (264 B): the pump 0x5309a0 (0x59fd30, 0x67fff), the flags
   0x576410 (0x200 / 0x400 / 0x800) gate "contact_GEB_KAUFEN" (text 0x1fcc) / "contact_TAGESORDNUNG"
   (text 0x1fd0) / "contact_AMT_INFO" (text 0x8c); with the gate 0x6f2448 and the key 0x1468410 byte 0x14,
   0x4f9e30 opens; the clicks open 0x59f9a0 (buy building), 0x5c1df0 (agenda) and 0x5c1b20 (office info) */

void ai_TownHallContact(int building, int room)
{
    int kaufen = 0;
    int agenda = 0;
    int amt = 0;
    int clicked;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_TownHallContact, 0x67fff) != 0) {
        if (ai_RunAllowsContact(0x200) == 1)
            kaufen = sv_RegisterNamePlate("contact_GEB_KAUFEN", txt_Ptrs[0x1fcc], 0x10);
        else
            kaufen = 0;
        if (ai_RunAllowsContact(0x400) == 1)
            agenda = sv_RegisterNamePlate("contact_TAGESORDNUNG", txt_Ptrs[0x1fd0], 0x16);
        else
            agenda = 0;
        if (ai_RunAllowsContact(0x800) == 1)
            amt = sv_RegisterNamePlate("contact_AMT_INFO", txt_Ptrs[0x8c], 0x16);
        else
            amt = 0;
        if (gm_AllowAll != 0 && kb_Last == 0x14)
            gm_StatisticsScreen();
        clicked = ai_ClickedObject;
        if (clicked != 0) {
            if (clicked == kaufen)
                ai_BuyBuildingWindow();
            else if (clicked == agenda)
                ai_OpenAgendaMenu();
            else if (clicked == amt)
                ai_OpenAmountKindMenu();
        }
    }
}
/* 0x59fe40, the town hall's citizens' right window "Locations\Rathaus\Buergerrecht" for the building b: a player
   worth less than 16000 (gm_GetGoodsValue) only gets the text 0x1a00; else the offer 0x19fe (the player, 16000,
   the town's name) is shown. On the confirm button, when the player's money covers 16000, the rank message 0x69
   (rank 2, due in 5 minutes, in the tutorial in 1 .. 3 hours) goes to the player and to every other player's
   character (kind 6 / 7, at most ai_NumPlayers of them), the 16000 go to the town hall's cash object (kind
   0x115), the power bit 0x4000 is set and the text 0x19ff shown; the campaign hears 0x2e before and after */

void ai_CitizenRightWindow(AIBLD *b)
{
    AICHARREC *me;
    int worth;
    AIOBJ *cash;
    int win;
    int bought;
    int i;
    int k;
    AICHARREC *c;
    AITIME due;

    me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    worth = gm_GetGoodsValue((char *)me);
    cash = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x115);
    bought = 0;
    if (worth < 16000) {
        char text[0x100];

        txt_Format(text, 0x1a00);
        sv_MessageBox(text, 0);
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, "Locations\\Rathaus\\Buergerrecht");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x19fe, me->id, 16000, &ai_PlayerSlots[ai_OwnPlayer][0x20]);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_ClickedId == 0 && sv_CheckGoodAmountB((char *)me, 16000, ai_OwnPlayer) != 0) {
                AIMSG msg;
                char text[0x200];

                i = 0;
                D3_ZERO(&msg, sizeof(msg));
                msg.type = 0x69;
                msg.to = me->netId;
                msg.fromId = -1;
                msg.flags = 9;
                msg.time = ai_Time;
                if (ai_TutorialOff() == 0)
                    gm_AddTime(&msg.time, 0, 5, 0);
                else
                    gm_AddTime(&msg.time, (unsigned short)util_Random(3) + 1, 0, 0);
                due = msg.time;
                msg.rankUp.rank = 2;
                msg.rankUp.ch = me->netId;
                cm_SendLongMessage((char *)&msg);
                for (k = 0; k < 768; k++) {
                    if (i >= ai_NumPlayers)
                        break;
                    c = &((AICHARREC *)ai_Chars)[k];
                    if ((c->kind == 6 || c->kind == 7) && c != me) {
                        D3_ZERO(&msg, sizeof(msg));
                        msg.to = c->netId;
                        msg.type = 0x69;
                        msg.fromId = -1;
                        msg.flags = 9;
                        msg.time = due;
                        msg.rankUp.rank = 2;
                        msg.rankUp.ch = me->netId;
                        cm_SendLongMessage((char *)&msg);
                        i++;
                    }
                }
                cm_SendMoveGoodsUnchecked(cash->objId, me->netId, 0, 16000);
                cm_SendSetField(me->netId, 0x1c8, 4, 0, 0x4000);
                txt_Format(text, 0x19ff);
                ai_CampaignStep(0x2e, 0, 0, 0);
                bought = 1;
                sv_MessageBox(text, 0);
            }
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame(ai_CitizenRightWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
    if (bought != 0)
        ai_CampaignStep(0x2e, 0, 1, 0);
}

int amt_GetFirstOffice(unsigned int k, struct AMTREC *out); int amt_ApplyForOffice(); int amt_KindColumn();
void ai_AmountInfoWindow(); int amt_GetOfficeCandidatesFull(); int amt_GetOfficeCandidates();
int ai_AmountLimit();
int amt_CanUseMayorPower2(); void d2_SetObjEnabled();

/* 0x5a0220, the applicants of the office kind office "locations\rathaus\amtsbewerbung2" for the character me
   (from ai_OfficeApplicationWindow): while the office is open (amt_GetFirstOffice) the up to 4 characters that
   applied for it (AICHAR.appliedOffice) are its amount rows (ai_AmountRowStatus / ai_AmountRow, two to a line);
   the list is rebuilt when their number changes. The apply button (enabled with fewer than 4 applicants and no
   application of me's own) applies (amt_ApplyForOffice, waiting for its slot); the other two open the office's
   column in the special-amount list and the office's info. Returns 1 when the application went through (the
   campaign hears 0x2f) */

int ai_OfficeApplicantsWindow(AICHARREC *me, unsigned int office)
{
    int shown;
    int win;
    int ok;
    int info;
    int column;
    int apply;
    int colWidth;
    int outs[3];
    AICHARREC *chars[4];
    AMTREC rec;
    LCAMOUNTROW rows[4];
    int n;
    int k;
    int i;
    int half;
    int slot;

    ok = 0;
    win = -1;
    apply = -1;
    column = -1;
    info = -1;
    shown = -1;
    d2_ClickedId = -1;
    do {
        if (amt_GetFirstOffice(office, &rec) == 0) {
            sv_PopRequest = 1;
        } else {
            n = 0;
            for (k = 0; k < 768; k++) {
                if (n >= 4)
                    break;
                if (((AICHARREC *)ai_Chars)[k].id != 0xffff && ((AICHARREC *)ai_Chars)[k].appliedOffice == office)
                    chars[n++] = &((AICHARREC *)ai_Chars)[k];
            }
            if (shown != n) {
                shown = n;
                if (win == -1) {
                    win = d2_OpenForm(0, 0, "locations\\rathaus\\amtsbewerbung2");
                    d2_CentreViewWindows(win);
                    d2_SetPointerSprite(0, 0);
                    d2_SetForm(win, 1);
                    io_PrintText(0x1a07, office + 0x253);
                    d2_SetForm(win, 3);
                    sv_ButtonRow(d2_LastWindow, 0, outs, 3, ai_TextOfficeApply, ai_TextOfficeColumn, ai_TextOfficeInfo);
                    apply = outs[0];
                    column = outs[1];
                    info = outs[2];
                }
                d2_SetForm(win, 2);
                d2_RemoveWindowObjects(d2_LastWindow, 1);
                io_PrintText(0x1a0b);
                if (n <= 0) {
                    io_PrintText(0x1a0c);
                } else {
                    colWidth = (d2_LastWindowPtr->w - d2_Pics[0x5da].w * 2) / 3;
                    half = (d2_LastWindowPtr->w - d2_Pics[0x5da].w * 2) % 3 / 2;
                    for (i = 0; i < n; i++) {
                        rows[i].x = (i % 2 + 1) * colWidth + half + (d2_Pics[0x5da].w + 10) * (i % 2) - 10;
                        rows[i].pickable = 0;
                        rows[i].y = i / 2 * 0x82 + 0x28;
                        rows[i].rec = chars[i];
                        ai_AmountRowStatus(&rows[i]);
                        ai_AmountRow(rows[i].x, rows[i].y, d2_LastWindow, 0x56, &rows[i], 1);
                    }
                }
                if (n < 4 && me->appliedOffice == 0)
                    d2_SetObjEnabled(apply, 1);
                else
                    d2_SetObjEnabled(apply, 0);
            }
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == apply) {
                slot = amt_ApplyForOffice(me, office);
                while (slot != -1 && cm_GetSlotState(slot) == 0)
                    sv_PumpNet();
                if (cm_GetSlotState(slot) == 1)
                    ok = 1;
                ai_CampaignStep(0x2f, 0, 0, office);
            } else if (d2_PointerObj == column) {
                k = amt_KindColumn(office);
                ai_SpecialAmountListWindow(k, 0, 0, 0, (int)sv_ReturnZero);
            } else if (d2_PointerObj == info) {
                ai_AmountInfoWindow(office, 0);
            }
        }
    } while (sv_RunFrame(ai_OfficeApplicantsWindow, 0x677c7) != 0);
    if (win != -1)
        d2_CloseView(win);
    if (ok != 0) {
        sv_PopRequest = 1;
        ai_CampaignStep(0x2f, 0, 1, office);
    }
    return ok;
}
/* 0x5a0590, the office application list "locations
athausmtsbewerbung" of the town hall: a player whose
   AICHAR.b00d is below 2 gets ai_TextOfficeNoRank; one who has applied already (appliedOffice) goes straight to
   that office's applicants (ai_OfficeApplicantsWindow). Otherwise amt_GetOfficeCandidatesFull lists the offices
   open to the player (ai_TextOfficeAllFull when they are all full, ai_TextOfficeNoneOpen when there is none);
   the window shows one button per office (text 0x1a04 with the office's name, 0x253 + kind), rebuilt whenever
   the candidate count changes, and a click on one opens its applicants, ai_TextOfficeApplied and the window
   closes once the application went in; a right click closes it */

void ai_OfficeApplicationWindow(void)
{
    int changed;
    int full;
    AICHARREC *me;
    int shown;
    int ctls[6];
    AMTCAND list[6];
    int count;
    int i;
    int win;

    shown = -1;
    full = 0;
    changed = 1;
    me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    if (me->b00d < 2) {
        sv_MessageBox(ai_TextOfficeNoRank, 0);
        return;
    }
    if (me->appliedOffice != 0) {
        ai_OfficeApplicantsWindow(me, me->appliedOffice);
        sv_CameraFollow();
        return;
    }
    if (amt_GetOfficeCandidatesFull(me, 6, list, &full) == 0) {
        if (full == 0) {
            sv_MessageBox(ai_TextOfficeNoneOpen, 0);
            return;
        }
        sv_MessageBox(ai_TextOfficeAllFull, 0);
        return;
    }
    win = d2_OpenForm(0, 0, "locations\\rathaus\\amtsbewerbung");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_ClickedId = -1;
    do {
        count = amt_GetOfficeCandidates(me, 6, list);
        if (count != shown)
            changed = 1;
        shown = count;
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            io_PrintText(0x1a01);
            for (i = 0; i < count; i++)
                ctls[i] = d2_GetWinObj(win, 1, io_PrintText(0x1a04, list[i].kind + 0x253));
            for (i = count; i < 6; i++)
                ctls[i] = -1;
            changed = 0;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            for (i = 0; i < count; i++) {
                if (ctls[i] == d2_PointerObj) {
                    if (i > -1 && i < count && ai_OfficeApplicantsWindow(me, list[i].kind) == 1) {
                        sv_MessageBox(ai_TextOfficeApplied, 0);
                        sv_PopRequest = 1;
                    }
                    break;
                }
            }
        }
    } while (sv_RunFrame(ai_OfficeApplicationWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x5a07d0, the law book / application listener of the town hall (381 B): the pump 0x5309a0 (0x5a07d0,
   0x67fff); the flags 0x576410 (0x1000: the three "ob_GESETZBUCH_*" texts 0x13f4 / 0x13f5 / 0x13f3,
   0x8000: "contact_BEWERBEN" text 0x1fc8, 0x2000 with the player's powerFlags 0x4000 and the byte b00d == 1:
   "contact_BUERGERRECHT" text 0x1fca); the clicks open the law book (0x112 / 0x111 / 0x113), the citizen
   right 0x59fe40 (bld) or the application 0x5a0590 */

void ai_LawBookContact(AIBLD *bld, int room)
{
    int steuern = 0;
    int strafrecht = 0;
    int verfassung = 0;
    int bewerben = 0;
    int recht = 0;
    int c;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_LawBookContact, 0x67fff) != 0) {
        if (ai_RunAllowsContact(0x1000) == 1) {
            steuern = sv_RegisterNamePlate("ob_GESETZBUCH_STEUERN", txt_Ptrs[0x13f4], 0xc);
            strafrecht = sv_RegisterNamePlate("ob_GESETZBUCH_STRAFRECHT", txt_Ptrs[0x13f5], 0xc);
            verfassung = sv_RegisterNamePlate("ob_GESETZBUCH_VERFASSUNG", txt_Ptrs[0x13f3], 0xc);
        } else {
            verfassung = 0;
            strafrecht = 0;
            steuern = 0;
        }
        if (ai_RunAllowsContact(0x8000) == 1)
            bewerben = sv_RegisterNamePlate("contact_BEWERBEN", txt_Ptrs[0x1fc8], 0x16);
        else
            bewerben = 0;
        if ((((AICHARREC *)ai_Chars)[ai_PlayerId].powerFlags & 0x4000) == 0 ||
            ((AICHARREC *)ai_Chars)[ai_PlayerId].b00d != 1 ||
            ai_RunAllowsContact(0x2000) != 1)
            recht = 0;
        else
            recht = sv_RegisterNamePlate("contact_BUERGERRECHT", txt_Ptrs[0x1fca], 0x16);
        c = ai_ClickedObject;
        if (c != 0) {
            if (c == steuern)
                ai_OpenLawBook(0x112);
            else if (c == strafrecht)
                ai_OpenLawBook(0x111);
            else if (c == verfassung)
                ai_OpenLawBook(0x113);
            else if (c == recht)
                ai_CitizenRightWindow(bld);
            else if (c == bewerben)
                ai_OfficeApplicationWindow();
        }
    }
}
/* 0x5a0950, the guild cash box "locations\zunfthaus\gildenkasse" of a guild hall bld: shows the box's money
   (the hall's object 0x142, text 0x1a1a); the guild master (the player's secondOffice 0x1e / 0x1f / 0x20 / 0x21 in a
   hall of the class 0x17 / 0x18 / 0x19 / 0x1a) also gets the city's text (0x1a1b), an amount slider from 0x640 to
   the player's limit (ai_AmountLimit of his goods value) and a pay button: a click moves the slider's amount
   (gm_PlayerFactor) from the player to the box when sv_CheckGoodAmount allows it; the cancel button and a right
   click close the window
   @deep deep s28: only slots differ: amount and cancelBtn swap [esp+0x10] / [esp+0x14] (the original:
   cancelBtn nearest esp, it gets the first -1 store at [esp+4] before push ebp). c2frame --refs: amount
   count 4 (the store, the 0x1a1a print, &amount, gm_ConvertMoney), cancelBtn 3, payBtn 2: the packer
   (size, then count descending) puts amount first; the original needs cancelBtn above amount. Tried:
   payBtn = cancelBtn = -1 (= control), cancelBtn = payBtn = -1 (slots 16), the rup click as if / else-if
   before the master test (draft 17); climb 3 rounds (61 / 353 / 213 mutants): nothing. twin.py: no byte
   twin. Next: a source form that gives cancelBtn two more references without code (a statement VC6 merges,
   written in two arms) or amount fewer. */

void ai_GuildCashPayWindow(AIBLD *bld)
{
    int amount;
    int cancelBtn;
    int payBtn;
    AIROOM *box;
    int master;
    int win;
    int text;
    int worth;
    int sum;
    AIBTYPE *type;

    cancelBtn = -1;
    payBtn = -1;
    text = 0;
    type = &((AIBTYPE *)ai_BTypes)[bld->type];
    box = (AIROOM *)gm_Query((void *)bld->objList, 2, 6, 0, 0x142);
    amount = gm_SumListAmounts((void *)box->list, ai_OwnPlayer);
    worth = gm_GetGoodsValue(&((AICHARREC *)ai_Chars)[ai_PlayerId]);
    if (type->kind == 0x17 && ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice == 0x1e)
        master = 1;
    else if (type->kind == 0x18 && ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice == 0x1f)
        master = 1;
    else if (type->kind == 0x19 && ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice == 0x20)
        master = 1;
    else if (type->kind == 0x1a && ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice == 0x21)
        master = 1;
    else
        master = 0;
    win = d2_OpenForm(0, 0, "locations\\zunfthaus\\gildenkasse");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x1a1a, amount);
    if (master) {
        text = io_PrintText(0x1a1b, ai_PlayerCity[ai_OwnPlayer][0] * 2 + 0x9a8);
        ai_AmountLimit(0x640, worth, 0x20, &amount);
        d2_SetObjValue4(d2_GetWinObj(win, 1, text), gm_ConvertMoney(0x640, ai_OwnPlayer),
                       gm_ConvertMoney(amount, ai_OwnPlayer), gm_ConvertMoney(0x640, ai_OwnPlayer));
        payBtn = d2_GetWinObj(win, 1, text + 1);
        cancelBtn = d2_GetWinObj(win, 1, text + 2);
    }
    d2_ClickedId = -1;
    while (sv_RunFrame(ai_GuildCashPayWindow, 0x677c7) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (master && d2_ClickedId != -1) {
            if (d2_PointerObj == payBtn) {
                sum = d2_GetObjValue(d2_GetWinObj(win, 1, text));
                sum = gm_PlayerFactor(sum, ai_OwnPlayer);
                if (sv_CheckGoodAmountB((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], sum, ai_OwnPlayer) != 0) {
                    cm_SendMoveGoods(box->objId, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ai_OwnPlayer, sum);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == cancelBtn) {
                sv_PopRequest = 1;
            }
        }
    }
    sv_CameraFollow();
    d2_CloseView(win);
}

int amt_ExecuteShareMessage(short *c, int *m);

/* 0x5a0ca0, the ai_MapPick callback of ai_GuildGiftEntry: the guild master's gift window
   "LOCATIONS\ZUNFTHAUS\LEVEL2" for the picked building bld. The fee is a hundredth of the player's worth, at
   least 160 (the windows.h max macro: gm_GetWorth is called twice); 0 when the player cannot pay it. The texts
   0x165f and 0x1660 (the player's office name by sex: 0x253 / 0x27b + secondOffice; the fee, the building's owner)
   with the give and cancel buttons; give executes the gift message {2, bld->id, the fee} (amt_ExecuteShareMessage)
   when the player can still pay, and closes the window with sv_PopRequest 2 (else the click falls to the cancel
   test). Returns 1 after the window */

int ai_GuildGiftCallback(char *place, AIBLD *bld)
{
    int cancelBtn;
    int req[5];
    int fee;
    int win;
    int text;
    int id;
    int giveBtn;

    fee = (int)max(gm_GetWorth(((AICHARREC *)ai_Chars)[ai_PlayerId].id) * 0.01f, 160.0f);
    if (sv_CheckGoodAmountB((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], fee, ai_OwnPlayer) == 0)
        return 0;
    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL2");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x165f);
    d2_SetForm(win, 2);
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x253;
    else
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x27b;
    text = io_PrintText(0x1660, id, 5, fee, bld->owner, bld);
    giveBtn = d2_GetWinObj(win, 2, text);
    cancelBtn = d2_GetWinObj(win, 2, text + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == giveBtn &&
                sv_CheckGoodAmountB((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], fee, ai_OwnPlayer) != 0) {
                req[0] = 2;
                req[1] = bld->id;
                req[2] = fee;
                amt_ExecuteShareMessage((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], req);
                sv_PopRequest = 2;
            } else if (d2_PointerObj == cancelBtn) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_GuildGiftCallback, 0x677c7) != 0);
    d2_CloseView(win);
    return 1;
}
/* 0x5a0f10, the guild master's gift power (guild hall level 2): when amt_CanUseMayorPower2 allows the player its second step,
   the map pick (flags 1, mode -1, the building class filter 0x1007d4190, kind 6, option 0x727, the prompt
   ai_TextGuildPowerPick) with ai_GuildGiftCallback; ai_TextGuildPowerUsed when the power is used already, else
   sv_CheckMoney's message for the 5 the player lacks */

void ai_GuildGiftEntry(int building)
{
    AMOUNTCFG pick;
    int r;

    r = amt_CanUseMayorPower2((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId]);
    if (r != 1) {
        if (r == -1)
            sv_MessageBox(ai_TextGuildPowerUsed, 0);
        else
            sv_CheckMoney((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], 5);
        return;
    }
    D3_ZERO(&pick, sizeof(pick));
    pick.flags = 1;
    pick.mode = -1;
    pick.kind = 6;
    pick.filter = 0x1007d4190;
    pick.option4c = 0x727;
    ai_MapPick(1, &pick, ai_TextGuildPowerPick, ai_GuildGiftCallback, 0);
}

int amt_MayorBonusStep(); int amt_CanUseMayorPower3(); int amt_ExecutePriceMessage(char *c, int *m);
int amt_GetOfficeCandidatesFull(); int amt_GetOfficeCandidates();

/* 0x5a1000, the guild master's price power (guild hall level 1, "LOCATIONS\ZUNFTHAUS\LEVEL1"): when
   amt_MayorBonusStep allows it (else ai_TextGuildPowerUsed, or sv_CheckMoney's message for the 3 the player lacks),
   the window shows the seven job icons (pictures 0x5a2 + i at fixed places, the picked one highlighted), the text
   0x1644 (the player's office name by sex, 3, the job's text 0x1649 + job) and the buttons raise / lower / close:
   raise and lower execute the guild-house price message {3, job + 1, 1 / -1} (amt_ExecutePriceMessage) and close
   the window; a click on another icon picks its job and rewrites the text */

void ai_GuildLevel1Window(int building)
{
    int buttons[3];
    POINT pos[7] = { {0x40, 0x50}, {0xc0, 0x50}, {0x140, 0x50}, {0x1c0, 0x50},
                     {0x80, 0xb8}, {0x100, 0xb8}, {0x180, 0xb8} };
    int lower[5];
    int raise[5];
    int icons[7];
    int r;
    int win;
    int i;
    int job;
    int id;

    r = amt_MayorBonusStep((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId]);
    if (r != 1) {
        if (r == -1)
            sv_MessageBox(ai_TextGuildPowerUsed, 0);
        else
            sv_CheckMoney((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], 3);
        return;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL1");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x1643);
    d2_SetForm(win, 2);
    for (i = 0; i < 7; i++) {
        icons[i] = d2_WinOpenPic((short)(pos[i].x - 0x28), (short)(pos[i].y - 0x46), i + 0x5a2, d2_LastWindow);
        d2_Objs[icons[i]].interactive = 1;
    }
    job = 0;
    d2_Objs[icons[0]].highlighted = 1;
    d2_SetForm(win, 3);
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x253;
    else
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x27b;
    io_PrintText(0x1644, id, 3, 0x1649);
    d2_SetForm(win, 4);
    sv_ButtonRow(d2_LastWindow, 0, buttons, 3, ai_TextGuildPriceRaise, ai_TextGuildPriceLower, ai_TextGuildPriceClose);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == buttons[2]) {
                sv_PopRequest = 1;
            } else if (d2_PointerObj == buttons[0]) {
                raise[0] = 3;
                raise[1] = job + 1;
                raise[2] = 1;
                amt_ExecutePriceMessage((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], raise);
                sv_PopRequest = 1;
            } else if (d2_PointerObj == buttons[1]) {
                lower[0] = 3;
                lower[1] = job + 1;
                lower[2] = -1;
                amt_ExecutePriceMessage((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], lower);
                sv_PopRequest = 1;
            } else {
                for (i = 0; i < 7; i++) {
                    if (d2_PointerObj == icons[i] && job != i) {
                        d2_Objs[icons[job]].highlighted = 0;
                        d2_Objs[icons[i]].highlighted = 1;
                        job = i;
                        d2_SetForm(win, 3);
                        io_PrintText("$C");
                        if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
                            id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x253;
                        else
                            id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x27b;
                        io_PrintText(0x1644, id, 3, i + 0x1649);
                        break;
                    }
                }
            }
        }
    } while (sv_RunFrame(ai_GuildLevel1Window, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}

int amt_ExecuteMayorPowerMessage(char *c, int *m);

typedef struct AMTPICK {           /* the row record the special-amount list hands its callback (8 bytes): the
                                      office index and the owner (0x5bea50 compares the word with 0xffff) */
    unsigned short office;         /* 0x00, the office index, 0xffff = an empty row */
    unsigned short pad02;
    int charId;                    /* 0x04, the office owner's character id (amt_ExecuteMayorPowerMessage's m[1]) */
} AMTPICK;

/* 0x5a13f0, the callback of the guild level 3 request (313 B, Ghidra-missed start: the callback of 0x5a17b0):
   the window "LOCATIONS\ZUNFTHAUS\LEVEL3" 0x4741f0, the text 0x167c, the text 0x167f (the office name from the
   player's second office and sex, 8, pick->office) and the pump 0x5309a0 (0x5a13f0, 0x677c7); a click
   (0xf8a668 == 0) sends the request 0x4e9ce0 (the player, {1, pick->charId}) and 0x6f22a0 = 3 (returns 1);
   0xf8a668 == 1 only raises 0x6f22a0; 0x4758f0 at the end, returns whether the request went through */
int ai_GuildLevel3RequestCB(AMTPICK *pick)
{
    int req[5];                 /* the request record of amt_ExecuteMayorPowerMessage (am.h G8REQ, 20 bytes): kind, id */
    int win;
    int id;
    int ok = 0;
    int r;

    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL3");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x167c);
    d2_SetForm(win, 2);
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x253;
    else
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x27b;
    io_PrintText(0x167f, id, 8, pick->office);
    for (;;) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            req[0] = 1;
            req[1] = pick->charId;
            amt_ExecuteMayorPowerMessage((char *)ai_Chars + ai_PlayerId * 0x21c, req);
            sv_PopRequest = 3;
            ok = 1;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
        if (sv_RunFrame(ai_GuildLevel3RequestCB, 0x677c7) == 0) {
            d2_CloseView(win);
            return ok;
        }
    }
}
/* 0x5a1530, the callback of the guild level 2 request (308 B, Ghidra-missed start: the callback of 0x5a17b0):
   the same window "LOCATIONS\ZUNFTHAUS\LEVEL3", the text 0x166a, the text 0x166d (the office name from the
   player's second office and sex, 8, the column's name 0x1663 + column), the pump 0x5309a0 (0x5a1530, 0x677c7);
   a click (0xf8a668 == 0) sends 0x4e9ce0 (the player, {4, column}) and 0x6f22a0 = 2; 0xf8a668 == 1 raises
   0x6f22a0; 0x4758f0 at the end */

void ai_GuildLevel2RequestCB(int column)
{
    int req[5];                 /* the request record of amt_ExecuteMayorPowerMessage (am.h G8REQ, 20 bytes): kind, id */
    int win;
    int id;

    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL3");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x166a);
    d2_SetForm(win, 2);
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x253;
    else
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x27b;
    io_PrintText(0x166d, id, 8, column + 0x1663);
    for (;;) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            req[0] = 4;
            req[1] = column;
            amt_ExecuteMayorPowerMessage((char *)ai_Chars + ai_PlayerId * 0x21c, req);
            sv_PopRequest = 2;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
        if (sv_RunFrame(ai_GuildLevel2RequestCB, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
    }
}
/* 0x5a1670, the callback of the guild level 3 power window, the amount-kind menu the guild level 3 entry
   (0x5a17b0) opens for a mayor whose second office is 0x20 (308 B, Ghidra-missed start: the callback of
   0x5a17b0): the window "LOCATIONS\ZUNFTHAUS\LEVEL3", the text 0x1674, the text 0x1677 (the office id from
   the player's second office and sex, 8, the column name 0x1663 + column), the pump 0x5309a0 (0x5a1670,
   0x677c7); a click (0xf8a668 == 0) sends 0x4e9ce0 (the player, {4, column}) and 0x6f22a0 = 2; 0xf8a668 == 1
   raises 0x6f22a0; 0x4758f0 at the end */

void ai_GuildLevel3PowerCB(int column)
{
    int req[5];                 /* the request record of amt_ExecuteMayorPowerMessage (am.h G8REQ, 20 bytes): kind, id */
    int win;
    int id;

    win = d2_OpenForm(0, 0, "LOCATIONS\\ZUNFTHAUS\\LEVEL3");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x1674);
    d2_SetForm(win, 2);
    if (((AICHARREC *)ai_Chars)[ai_PlayerId].sex == 0)
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x253;
    else
        id = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice + 0x27b;
    io_PrintText(0x1677, id, 8, column + 0x1663);
    for (;;) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            req[0] = 4;
            req[1] = column;
            amt_ExecuteMayorPowerMessage((char *)ai_Chars + ai_PlayerId * 0x21c, req);
            sv_PopRequest = 2;
        } else if (d2_ClickedId == 1) {
            sv_PopRequest = 1;
        }
        if (sv_RunFrame(ai_GuildLevel3PowerCB, 0x677c7) == 0) {
            d2_CloseView(win);
            return;
        }
    }
}
/* 0x5a17b0, the guild master's third power (guild hall level 3): when amt_CanUseMayorPower3 allows it (else
   ai_TextGuildPowerUsed, or sv_CheckMoney's message for the 8 the player lacks), by the player's second office:
   0x1e picks a character (ai_AmountMenu, mode 0xc00, kind 6, the prompt ai_TextGuildLevel3Pick) for
   ai_GuildLevel3RequestCB; 0x1f / 0x21 pick a column (ai_AmountKindMenu) for ai_GuildLevel2RequestCB, 0x20 one for
   ai_GuildLevel3PowerCB; the camera follows the player again after the menu */

void ai_GuildLevel3Entry(void)
{
    AMOUNTCFG cfg;
    unsigned char office;
    int r;

    r = amt_CanUseMayorPower3((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId]);
    if (r != 1) {
        if (r == -1)
            sv_MessageBox(ai_TextGuildPowerUsed, 0);
        else
            sv_CheckMoney((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], 8);
        return;
    }
    office = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice;
    if (office == 0x1e) {
        memset(&cfg, 0, sizeof(cfg));
        cfg.mode = 0xc00;
        cfg.kind = 6;
        cfg.lockSecond = 1;
        ai_AmountMenu(&cfg, ai_TextGuildLevel3Pick, 0, ai_GuildLevel3RequestCB);
    } else if (office == 0x1f || office == 0x21) {
        ai_AmountKindMenu(ai_GuildLevel2RequestCB, ai_TextGuildLevel2Pick, 0);
    } else if (office == 0x20) {
        ai_AmountKindMenu(ai_GuildLevel3PowerCB, ai_TextGuildLevel3PowerPick, 0);
    }
    sv_CameraFollow();
}
/* 0x5a18d0, the guild level 2 listener, the room 0x137 handler of the enter-building driver (144 B): with the
   guild office power (0x4e94e0, the player) set, the pump 0x5309a0 (0x5a18d0, 0x67fff) looks up the
   "contact_ZUNFTLEVEL2" plate (txt_Ptrs[0x165d], class 0x16) and a click on it opens the office application
   entry (0x5a0f10) */

void ai_GuildLevel2Listener(int building, int room)
{
    int c = 0;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_GuildLevel2Listener, 0x67fff) != 0) {
        if (amt_CanUseMayorPower2((char *)ai_Chars + ai_PlayerId * 0x21c) != 0)
            c = sv_RegisterNamePlate("contact_ZUNFTLEVEL2", txt_Ptrs[0x165d], 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == c)
            ai_GuildGiftEntry(building);
    }
}
/* 0x5a1960, the guild level 3 listener, the room 0x139 handler of the enter-building driver (256 B): the text
   id of the level 3 plate by the player's second office (0x1e 0x167c, 0x1f / 0x21 0x166a, 0x20 0x1674); the
   pump 0x5309a0 (0x5a18d0, 0x67fff) looks up the "ob_ZUNFTKASSE" plate (txt_Ptrs[0x1a19], class 0x16) and,
   with the guild office power (0x4e95a0, the player) and the id, the "contact_ZUNFTLEVEL3" plate (the text
   txt_Ptrs[id]); a click opens the guild cash payment window (0x5a0950) or the level 3 entry (0x5a17b0) */

void ai_GuildLevel3Listener(int building, int room)
{
    int kasse;
    int level3 = 0;
    int id = 0;
    int r;
    char c;

    c = ((AICHARREC *)ai_Chars)[ai_PlayerId].secondOffice;
    if (c == 0x1e)
        id = 0x167c;
    else if (c == 0x1f || c == 0x21)
        id = 0x166a;
    else if (c == 0x20)
        id = 0x1674;
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_GuildLevel2Listener, 0x67fff) != 0) {
        kasse = sv_RegisterNamePlate("ob_ZUNFTKASSE", txt_Ptrs[0x1a19], 0x16);
        r = amt_CanUseMayorPower3((char *)ai_Chars + ai_PlayerId * 0x21c);
        if (r != 0 && id != 0)
            level3 = sv_RegisterNamePlate("contact_ZUNFTLEVEL3", txt_Ptrs[id], 0x16);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == kasse)
                ai_GuildCashPayWindow((AIBLD *)building);
            else if (ai_ClickedObject == level3)
                ai_GuildLevel3Entry();
        }
    }
}
/* 0x5a1a60, the guild level 1 listener, the room 0x138 handler of the enter-building driver (144 B): with the
   guild office level power (0x4e94a0, the player) set, the pump 0x5309a0 (0x5a1a60, 0x67fff) looks up the
   "contact_ZUNFTLEVEL1" plate (txt_Ptrs[0x1641], class 0x16) and a click on it opens the guild level 1 window
   (0x5a1000) */

void ai_GuildLevel1Listener(int building, int room)
{
    int c = 0;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_GuildLevel1Listener, 0x67fff) != 0) {
        if (amt_MayorBonusStep((char *)ai_Chars + ai_PlayerId * 0x21c) != 0)
            c = sv_RegisterNamePlate("contact_ZUNFTLEVEL1", txt_Ptrs[0x1641], 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == c)
            ai_GuildLevel1Window(building);
    }
}

char * gm_Query(void *list, int n, ...); int gm_GetWorth(unsigned short id); int gm_GetGoodsValue(); int ai_AmountLimit();
int d2_OpenForm(); void d2_SetPointerSprite(int a, int pic); int d2_SetForm(int view, int n);
int io_PrintText(char *fmt, ...); int d2_GetWinObj(); int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
int cm_SendMoveGoods(int a, int b, unsigned char c, int d); int cm_SendRelations(int a, int b, int c, int d); void txt_Format(char *out, int id0, ...);
void d2_CloseView(int view); int _ftol();

/* 0x5a1af0, the church donation window "Locations\Kirche\Spenden" of the church bld (not for an unowned church or
   the player's own): the donation box is the church's object 0xe8. The least donation is a hundredth of the worth
   of the player and the owner together (each at least 1: the windows.h max macro calls gm_GetWorth twice); a player
   whose goods value is below it gets the text 0x1987, else the text 0x1988 with an amount slider (from the least
   donation up to ai_AmountLimit of the goods value) and the give / cancel buttons (text 0x1989). Give moves the
   amount (gm_PlayerFactor) into the box when sv_CheckGoodAmount allows it, raises the owner's relation to the player
   by amount * 255 / the worth total and every other character of the owner's faith by a fifth of that, and
   shows the texts 0x1986 / 0x198a; the window closes on give, cancel or a right click */

void ai_DonationWindow(AIBLD *bld)
{
    int money;
    int cancelBtn;
    int giveBtn;
    AIROOM *box;
    int slider;
    /* @original win's initializer emits no code; it moves win's first reference, which orders VC6's
       reloads after the gift path's message box: the original reloads win (edi) before bld (ebx) */
    int win = 0;
    int gift;
    float total;
    char text[0x200];
    int least;
    int ok;
    int v;
    int i;
    float rel;

    ok = 0;
    cancelBtn = giveBtn = slider = -1;
    if (bld == 0 || bld->owner == 0xffff || bld->owner == ai_PlayerId)
        return;
    box = (AIROOM *)gm_Query((void *)bld->objList, 2, 6, 0, 0xe8);
    if (box == 0)
        return;
    total = (float)(max(gm_GetWorth(ai_PlayerId), 1) + max(gm_GetWorth(bld->owner), 1));
    least = (int)(total * 0.01f);
    money = gm_GetGoodsValue(&((AICHARREC *)ai_Chars)[ai_PlayerId]);
    if (money >= least)
        ok = 1;
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Spenden");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x1986);
    if (!ok) {
        io_PrintText(0x1987, ai_PlayerId, ai_PlayerCity[ai_OwnPlayer][0] * 2 + 0x9a8);
    } else {
        int amount;

        v = io_PrintText(0x1988, ai_PlayerId, bld->owner, 0x48c, ai_PlayerCity[ai_OwnPlayer][0] * 2 + 0x9a8);
        ai_AmountLimit(least, money, 0x20, &amount);
        slider = d2_GetWinObj(win, 1, v);
        d2_SetObjValue4(slider, gm_ConvertMoney(least, ai_OwnPlayer), gm_ConvertMoney(amount, ai_OwnPlayer),
                       gm_ConvertMoney(least, ai_OwnPlayer));
        d2_SetForm(win, 2);
        v = io_PrintText(0x1989);
        giveBtn = d2_GetWinObj(win, 2, v);
        cancelBtn = d2_GetWinObj(win, 2, v + 1);
    }
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == giveBtn) {
                gift = d2_GetObjValue(slider);
                gift = gm_PlayerFactor(gift, ai_OwnPlayer);
                if (sv_CheckGoodAmountB((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], gift, ai_OwnPlayer) != 0) {
                    cm_SendMoveGoods(box->objId, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ai_OwnPlayer, gift);
                    rel = (float)(gift * 0xff) / total;
                    cm_SendRelations(((AICHARREC *)ai_Chars)[ai_PlayerId].netId,
                                     ((AICHARREC *)ai_Chars)[bld->owner].netId, (int)rel, 1);
                    rel = rel * 0.2f;
                    for (i = 0; i < 0x300; i++) {
                        if (i != ai_PlayerId && ((AICHARREC *)ai_Chars)[i].netId != -1 &&
                            ((AICHARREC *)ai_Chars)[i].netId != ((AICHARREC *)ai_Chars)[bld->owner].netId &&
                            ((AICHARREC *)ai_Chars)[bld->owner].faith == ((AICHARREC *)ai_Chars)[i].faith)
                            cm_SendRelations(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ((AICHARREC *)ai_Chars)[i].netId,
                                             (int)rel, 1);
                    }
                    txt_Format(text, (int)"%s%s", 0x1986, 0x198a);
                    sv_MessageBox(text, 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == cancelBtn) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_DonationWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}

int gv_CountOwnedGraves(); int gv_CulpritGuilt(int c); int gm_GetWorth(unsigned short id); float gm_Liking();
int ai_InventoryHas(void *ch, int id); int d2_OpenForm(); void d2_SetPointerSprite(int a, int pic); 
int d2_SetForm(int view, int n); int io_PrintText(char *fmt, ...); int d2_GetWinObj();
void cm_BeginFieldBlock(char *o, int id); int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v); int cm_SendFieldDeltas(void);
int gv_RetireCrimesOf(int p, int n); int cm_SendMoveGoods(int a, int b, unsigned char c, int d); void ai_ShowIndulgenceNews(); int cm_GetSlotState();
void txt_Format(char *out, int id0, ...);
void d2_CloseView(int view); int _ftol();
void ai_Top5Window();

/* 0x5a1f70, the indulgence window "Locations\Kirche\Ablass2" of the church bld: the player's guilt
   (gv_CulpritGuilt) sets the price, guilt * x * the player's worth * 0.005 with x = 1.5 in the player's own church,
   else (200 - the owner's liking of the player) / 100; a tenth of it, kept within 3200 .. 320000 (the windows.h
   min / max macros), halved when the player carries the good 0x2c6. With a price the texts 0x1962 (the graves
   gv_CountOwnedGraves counts, the price) and the buy button 0x1963, else the text 0x1961. Buying (when
   sv_CheckGoodAmount allows it) adds the price to the player's donations (+0x1ac), retires the crimes
   (gv_RetireCrimesOf), pays the price; in another's church the owner gets price / log10(price) and the news
   (ai_ShowIndulgenceNews); waits for the payment's slot, shows the text 0x1964 (the donations, gm_DonationRank)
   and opens the top five list (ai_Top5Window) after the window */

void ai_IndulgenceWindow(AIBLD *bld, int room)
{
    int price;
    int guilt;
    int win;
    int buyBtn;
    int graves;
    char buf[0x200];
    int gain;
    int slot;
    int bought;
    float worth;
    float x;
    AICHARREC *me;

    buyBtn = -1;
    price = 0;
    graves = gv_CountOwnedGraves(((AICHARREC *)ai_Chars)[ai_PlayerId].netId);
    guilt = gv_CulpritGuilt(((AICHARREC *)ai_Chars)[ai_PlayerId].netId);
    if (guilt != 0) {
        worth = gm_GetWorth(ai_PlayerId) * 0.005f;
        x = 1.5f;
        if (bld->owner != ai_PlayerId)
            x = (200.0f - gm_Liking(ai_PlayerId, bld->owner, 1)) * 0.01f;
        price = (int)(guilt * x * worth);
        price = (int)min(max(price * 0.1, 3200.0), 320000.0);
        if (ai_InventoryHas(&((AICHARREC *)ai_Chars)[ai_PlayerId], 0x2c6))
            price /= 2;
    }
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Ablass2");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x1960);
    if (price > 0) {
        io_PrintText(0x1962, graves, price);
        d2_SetForm(win, 2);
        buyBtn = d2_GetWinObj(win, 2, io_PrintText(0x1963));
    } else {
        io_PrintText(0x1961, ai_PlayerId);
    }
    d2_ClickedId = -1;
    bought = 0;
    do {
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1 && d2_PointerObj == buyBtn &&
                   sv_CheckGoodAmountB((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], price, ai_OwnPlayer) != 0) {
            me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
            cm_BeginFieldBlock((char *)me, me->netId);
            cm_AddFieldValues(4, 1, 0x1ac, &price);
            cm_SendFieldDeltas();
            gv_RetireCrimesOf(me->netId, graves);
            if (bld->owner != 0xffff && bld->owner != ai_PlayerId) {
                gain = (int)((float)price / log10(price));
                cm_SendMoveGoods(-1, me->netId, ai_OwnPlayer, price);
                slot = cm_SendMoveGoods(((AICHARREC *)ai_Chars)[bld->owner].netId, -1, ai_OwnPlayer, gain);
                ai_ShowIndulgenceNews(me, &((AICHARREC *)ai_Chars)[bld->owner], bld, gain, price);
            } else {
                slot = cm_SendMoveGoods(-1, me->netId, ai_OwnPlayer, price);
            }
            while (slot != -1 && cm_GetSlotState(slot) == 0)
                sv_PumpNet();
            txt_Format(buf, 0x1964, me->donated, gm_DonationRank(ai_PlayerId, 0));
            sv_MessageBox(buf, 0x100);
            bought = 1;
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame(ai_IndulgenceWindow, 0x677c7) != 0);
    d2_CloseView(win);
    if (bought)
        ai_Top5Window();
    else
        sv_CameraFollow();
}

void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v); short * gm_FindChar(int id); int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);

/* 0x5a23a0, the baptism window "Locations\Kirche\Taufe" of the church bld: the fee is a hundredth of the
   player's worth (gm_GetWorth). In the player's own church the fee is fifteen times that: the text 0x1976 (the
   player, the fee, the other faith's name 0x197a + 1 - faith, + 2 from the year 0x75 on) and the buttons 0x1977;
   converting (when sv_CheckGoodAmount allows it) pays the fee, flips the faith of the player and of the
   relatives [3..7] (gm_FindChar), shows the news 0x1978 (the player, bld, the faith's name) to every other
   character of the kinds 6 / 7 (he_ShowNews 0x5fe) and the text 0x1979. In a church of another faith the text
   0x1971 (the owner's faith) and the buttons 0x1972; converting pays the fee to the owner, takes the owner's
   faith, sends the owner of the kinds 6 / 7 the news 0x1974 (0x5ff) and shows the text 0x1973. In a church of
   the player's faith only the text 0x1970 */
void ai_BaptismWindow(AIBLD *bld, int room)
{
    AICHARREC *me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    AICHARREC *owner;
    AICHARREC *c;
    AICHARREC *rel;
    int price;
    int win;
    int textId;
    int convertBtn;
    int cancelBtn;
    int faith;
    int i;
    char buf[0x200];

    price = (int)(gm_GetWorth(ai_PlayerId) * 0.01f);
    if (bld == 0 || bld->owner == 0xffff)
        return;
    if (bld->owner == ai_PlayerId) {
        price *= 15;
        win = d2_OpenForm(0, 0, "Locations\\Kirche\\Taufe");
        d2_SetPointerSprite(0, 0);
        d2_CentreViewWindows(win);
        d2_SetForm(win, 0);
        sv_FitForm(d2_LastWindow);
        d2_SetForm(win, 1);
        io_PrintText(0x1976, me->id, price, (ai_Time.year >= 0x75 ? 0x197c : 0x197a) - me->faith + 1);
        d2_SetForm(win, 2);
        textId = io_PrintText(0x1977);
        convertBtn = d2_GetWinObj(win, 2, textId);
        cancelBtn = d2_GetWinObj(win, 2, textId + 1);
        d2_ClickedId = -1;
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == convertBtn) {
                    if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) != 0) {
                        faith = 1 - me->faith;
                        cm_SendMoveGoodsUnchecked(-1, me->netId, 0, price);
                        cm_BeginFieldBlock((char *)me, me->netId);
                        cm_AddFieldDelta(1, 1, 0xc, &faith);
                        cm_SendFieldDeltas();
                        for (i = 3; i <= 7; i++) {
                            if (((AICHARREC *)ai_Chars)[ai_PlayerId].relatives[i] != -1 &&
                                (rel = (AICHARREC *)gm_FindChar(((AICHARREC *)ai_Chars)[ai_PlayerId].relatives[i])) != 0) {
                                cm_BeginFieldBlock((char *)rel, rel->netId);
                                cm_AddFieldDelta(1, 1, 0xc, &faith);
                                cm_SendFieldDeltas();
                            }
                        }
                        txt_Format(buf, 0x1978, me->id, bld, (ai_Time.year >= 0x75 ? 0x197c : 0x197a) + faith);
                        for (i = 0; i < 768; i++) {
                            c = &((AICHARREC *)ai_Chars)[i];
                            if (c->netId != me->netId && (c->kind == 6 || c->kind == 7))
                                he_ShowNews(c->netId, -1, buf, 0, 0x5fe, 0);
                        }
                        txt_Format(buf, 0x1979, (ai_Time.year >= 0x75 ? 0x197c : 0x197a) + faith);
                        sv_MessageBox(buf, 0);
                        sv_PopRequest = 1;
                    }
                } else if (d2_PointerObj == cancelBtn) {
                    sv_PopRequest = 1;
                }
            }
        } while (sv_RunFrame(ai_BaptismWindow, 0x677c7) != 0);
        d2_CloseView(win);
        sv_CameraFollow();
        return;
    }
    owner = &((AICHARREC *)ai_Chars)[bld->owner];
    if (owner == 0)
        return;
    if (owner->faith == me->faith) {
        txt_Format(buf, 0x1970);
        sv_MessageBox(buf, 0);
        return;
    }
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Taufe");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(win, 1);
    io_PrintText(0x1971, me->id, price, (ai_Time.year >= 0x75 ? 0x197c : 0x197a) + owner->faith);
    d2_SetForm(win, 2);
    textId = io_PrintText(0x1972);
    convertBtn = d2_GetWinObj(win, 2, textId);
    cancelBtn = d2_GetWinObj(win, 2, textId + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == convertBtn) {
                if (sv_CheckGoodAmountB(ai_Chars[ai_PlayerId], price, ai_OwnPlayer) != 0) {
                    faith = owner->faith;
                    cm_SendMoveGoodsUnchecked(owner->netId, me->netId, 0, price);
                    cm_BeginFieldBlock((char *)me, me->netId);
                    cm_AddFieldDelta(1, 1, 0xc, &faith);
                    cm_SendFieldDeltas();
                    if (owner->kind == 6 || owner->kind == 7) {
                        txt_Format(buf, 0x1974, me->id, price);
                        he_ShowNews(owner->netId, -1, buf, 0, 0x5ff, 0);
                    }
                    txt_Format(buf, 0x1973, (ai_Time.year >= 0x75 ? 0x197c : 0x197a) + owner->faith);
                    sv_MessageBox(buf, 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == cancelBtn) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_BaptismWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}

void gm_DonationTop5(unsigned *ids, int *sc); int ai_DrawAmountRow();
int d2_RemoveWindowObjects();

/* 0x5a28f0, the church's top five donors window "Locations\Kirche\Top5" (opened by ai_IndulgenceWindow after a
   purchase): each frame the five donors and their donations (gm_DonationTop5) are compared with the drawn ones;
   on a change the frame 1 is redrawn with the header 0x196b and per donor a special-amount row (ai_AmountRowStatus /
   ai_DrawAmountRow, the row picture 0x5da), the text 0x196d (the id; txt_NoRelative without a record) and the
   donation 0x196e (ai_LeftAlignedText); a right click leaves */
void ai_Top5Window(void)
{
    int i;
    int rowX;
    int win;
    int sum[5];
    unsigned id[5];
    int drawnSum[5];
    unsigned drawnId[5];
    char buf[0x200];
    LCAMOUNTROW rows[5];
    int changed;
    int half;
    int x;

    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Top5");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    changed = 1;
    d2_ClickedId = -1;
    do {
        gm_DonationTop5(id, sum);
        for (i = 0; i < 5; i++) {
            if (drawnId[i] != id[i] || drawnSum[i] != sum[i])
                changed = 1;
            drawnId[i] = id[i];
            drawnSum[i] = sum[i];
            rows[i].rec = 0;
            if (id[i] != 0xffff)
                rows[i].rec = &((AICHARREC *)ai_Chars)[id[i]];
        }
        if (changed) {
            d2_SetForm(win, 1);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            io_PrintText(0x196b);
            half = d2_LastWindowPtr->w / 2;
            rowX = half - d2_Pics[0x5da].w - 0x87;
            x = half - 100;
            for (i = 0; i < 5; i++) {
                rows[i].y = 0x34 + i * 0x4c - 8;
                rows[i].x = rowX;
                rows[i].pickable = rows[i].rec != 0;
                ai_AmountRowStatus(&rows[i]);
                rows[i].pickable = rows[i].rec != 0;
                ai_DrawAmountRow(rows[i].x, rows[i].y, d2_LastWindow, &rows[i]);
                if (rows[i].rec != 0)
                    txt_Format(buf, 0x196d, id[i]);
                else
                    strcpy(buf, txt_NoRelative);
                ai_LeftAlignedText(buf, x, 0x34 + i * 0x4c, d2_LastWindowPtr->w - x + 0x14, 0x53);
                txt_Format(buf, 0x196e, sum[i]);
                ai_LeftAlignedText(buf, x, 0x34 + i * 0x4c + 0x19, d2_LastWindowPtr->w - x + 0x14, 0x53);
            }
            changed = 0;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_Top5Window, 0x677c7) != 0);
    sv_CameraFollow();
    d2_CloseView(win);
}

void cm_SendMoneyDelta(int a, int b); void ai_ShowPaymentNews(); 
/* 0x5a2b60, the confession window "Locations\Kirche\Beichte" of the church bld (none without an owner): the text
   0x197f; without graves of the player's (gv_CountOwnedGraves) the text 0x1980, else the text 0x1981 (the
   graves) with the amount slider (2 .. 6, step 2) and the buttons 0x1982 (confess) / 0x1983 (cancel). Confessing
   (when sv_CheckMoney allows the amount) pays the amount (gm_SendMoneyDelta); in another's church the owner gets 1
   and the news (ai_ShowPaymentNews); it retires amount / 2 crimes (gv_RetireCrimesOf) and shows the text 0x1984 */
void ai_ConfessionWindow(AIBLD *bld)
{
    int active = 0;
    int confessBtn = -1;
    int cancelBtn = -1;
    int slider = -1;
    int graves;
    int win;
    int amount;
    AICHARREC *me;

    if (bld->owner == 0xffff)
        return;
    graves = gv_CountOwnedGraves(((AICHARREC *)ai_Chars)[ai_PlayerId].netId);
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Beichte");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    io_PrintText(0x197f);
    if (graves != 0) {
        slider = d2_GetWinObj(win, 1, io_PrintText(0x1981, graves));
        d2_SetObjValue4(slider, 2, 6, 2);
        confessBtn = d2_GetWinObj(win, 1, io_PrintText(0x1982));
        cancelBtn = d2_GetWinObj(win, 1, io_PrintText(0x1983));
        active = 1;
        d2_ClickedId = -1;
    } else {
        io_PrintText(0x1980);
        d2_ClickedId = -1;
    }
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && active) {
            if (d2_PointerObj == confessBtn) {
                me = &((AICHARREC *)ai_Chars)[ai_PlayerId];
                amount = d2_GetObjValue(slider);
                if (sv_CheckMoney(me, amount) != 0) {
                    gm_SendMoneyDelta(me->netId, -amount);
                    if (bld->owner != 0xffff && bld->owner != ai_PlayerId) {
                        gm_SendMoneyDelta(((AICHARREC *)ai_Chars)[bld->owner].netId, 1);
                        ai_ShowPaymentNews((unsigned short *)&((AICHARREC *)ai_Chars)[ai_PlayerId],
                                           ai_Chars[bld->owner], (int)bld);
                    }
                    gv_RetireCrimesOf(me->netId, amount / 2);
                    sv_MessageBox(txt_Ptrs[0x1984], 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == cancelBtn) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_ConfessionWindow, 0x677c7) != 0);
    sv_CameraFollow();
    d2_CloseView(win);
}


/* 0x5a2de0, the confession listener, the room 0xf2 handler of the enter-building driver (128 B): the pump
   0x5309a0 (0x5a2de0, 0x67fff) looks up the "contact_BEICHTE" plate (txt_Ptrs[0x197e], class 0x16) while the
   building permission 0x200 (0x6f231c) is clear and a click on it opens the confession window (0x5a2b60) */

void ai_ConfessionListener(int building, int room)
{
    int c = 0;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_ConfessionListener, 0x67fff) != 0) {
        if ((ai_BuildingPerms & 0x200) == 0)
            c = sv_RegisterNamePlate("contact_BEICHTE", txt_Ptrs[0x197e], 0x16);
        if (ai_ClickedObject != 0 && (ai_BuildingPerms & 0x200) == 0 && ai_ClickedObject == c)
            ai_ConfessionWindow(building);
    }
}

int cm_SendObject1b5(int owner, G17B7 value); int cm_GetSlotValue(int h); int cm_SendGroupState(); int cm_FlushSendQueue();
int cm_ReceiveMessages(); int cm_ExecCommands(); int cm_SendLongMessage(char *msg); int cm_GetSlotState();
int ai_AmountMenu(); void d2_SetObjEnabled(); char *itoa(int, char *, int);

/* 0x5a2e60, the sermon window "Locations\Kirche\Predigt" of the player's own church bld: its sermon object (gm_Query
   kind 0x1b5 of the objList; made by cm_SendObject1b5 with every setting -1 when missing) and its event 0x2b
   (posted when missing, its slot awaited) give the state: 0 (sermons not paid for), 2 (a sermon with a priest,
   gm_FindChar), 1 (none set), 3 (the building or its ownership gone: the window closes); every 100 ticks of d3_Time
   it is read again. State 0: the text 0x19a5 (the event's end) and the buttons 0x19a6: paying 1 (sv_CheckMoney /
   gm_SendMoneyDelta) sets AISERMON.enabled. State 1: the text 0x19a0 and the buttons 0x19a1: the first (enabled
   while the event's stage is below 1) picks a priest (ai_AmountMenu, the gypsy filter, the text 0x19a2) and sets
   the two random texts (util_Random 8). State 2: the text 0x198e (the end, the texts 0x1990 + text1 / 0x1998 +
   text2, the priest) with the priest's amount row and the buttons 0x198f: the first clears the sermon. Each
   change waits for its slot (sv_RunFrame)
   @deep Read whole against the original and reconstructed (614 = 614 instructions, all calls and stores in
   order). Left: at +0x60d the original hoists the gypsyFilter high dword load (mov edx, [0x5eed6c]) above
   the AMOUNTCFG memset (rep stosd) and pushes 0x19a2 after it; ours loads both halves after the memset and
   pushes 0x19a2 before it (scheduling; ai_RobberContact byte-matches the same cfg.filter = gypsyFilter
   with a hoisted low dword). Tried: filter assigned first (worse, code 26). The rest is slots / order (the
   G17B7 init, the clear-button bytes). The prover stops at the 4-byte store of AISERMON.priest across two
   cells of the G17B7 block (+0x60), the same on both sides. */
void ai_SermonWindow(AIBLD *bld)
{
    char flag;
    char text1;
    char text2;
    int state;
    AIJOB *ev;
    int cancelBtn;
    AICHARREC *priest;
    signed char none;
    int noPriest;
    int payBtn;
    int pickBtn;
    int clearBtn;
    unsigned long lastTime;
    LCAMOUNTROW row;
    AMOUNTCFG cfg;
    char title[0x20];
    AIMSG msg;
    G17B7 settings;
    AIOBJ *sermon;
    int win;
    int changed;
    int i;
    int slot;

    payBtn = 0;
    pickBtn = 0;
    cancelBtn = 0;
    clearBtn = 0;
    if (bld == 0 || bld->owner == 0xffff || bld->owner != ai_PlayerId)
        return;
    sermon = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0x1b5);
    if (sermon == 0) {
        settings.sermon.text1 = -1;
        settings.sermon.text2 = -1;
        settings.sermon.priest = -1;
        sermon = (AIOBJ *)cm_GetSlotValue(cm_SendObject1b5(bld->id, settings));
        while (cm_SendGroupState() == 0) {
            cm_FlushSendQueue();
            cm_ReceiveMessages();
            cm_ExecCommands();
        }
    }
    ev = (AIJOB *)he_FindEvent(2, 0, 0x2b, 3, bld->id);
    if (ev == 0) {
        msg.time = ai_Time;
        msg.type = 0x2b;
        msg.flags = 2;
        msg.to = ((AICHARREC *)ai_PlayerChar)->netId;
        msg.fromId = bld->id;
        msg.tavern.mode = 0;
        slot = cm_SendLongMessage((char *)&msg);
        while (slot != -1 && cm_GetSlotState(slot) == 0)
                sv_RunFrame(0, 0x677c7);
        ev = (AIJOB *)cm_GetSlotValue(slot);
    }
    priest = 0;
    if (sermon->sermon.enabled == 0)
        state = 0;
    else if (sermon->sermon.text1 != -1 && sermon->sermon.text2 != -1 &&
             (priest = (AICHARREC *)gm_FindChar(sermon->sermon.priest)) != 0)
        state = 2;
    else
        state = 1;
    win = d2_OpenForm(0, 0, "Locations\\Kirche\\Predigt");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(d2_LastWindow);
    changed = 1;
    d2_ClickedId = -1;
    lastTime = d3_Time;
    do {
        if (d3_Time - lastTime > 100 && state != 3) {
            lastTime = d3_Time;
            changed = 1;
            sermon = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0x1b5);
            if (sermon != 0 && bld->owner == ai_PlayerId) {
                if (sermon->sermon.enabled == 0) {
                    state = 0;
                } else if (sermon->sermon.text1 != -1 && sermon->sermon.text2 != -1 &&
                           sermon->sermon.priest != -1) {
                    priest = (AICHARREC *)gm_FindChar(sermon->sermon.priest);
                    if (priest != 0)
                        state = 2;
                    else
                        state = 1;
                }
            } else {
                state = 3;
                changed = 0;
            }
        }
        if (changed && state != 3) {
            payBtn = -1;
            clearBtn = -1;
            pickBtn = -1;
            cancelBtn = -1;
            if (state == 0) {
                d2_SetForm(win, 1);
                io_PrintText("$C");
                io_PrintText(0x19a5, 1, &ev->end);
                d2_SetForm(win, 2);
                io_PrintText("$C");
                i = io_PrintText(0x19a6);
                payBtn = d2_GetWinObj(win, 2, i);
                cancelBtn = d2_GetWinObj(win, 2, i + 1);
            } else if (state == 1) {
                d2_SetForm(win, 1);
                d2_RemoveWindowObjects(d2_LastWindow, 1);
                io_PrintText("$C");
                io_PrintText(0x19a0, &ev->end);
                d2_SetForm(win, 2);
                d2_RemoveWindowObjects(d2_LastWindow, 1);
                io_PrintText("$C");
                i = io_PrintText(0x19a1);
                pickBtn = d2_GetWinObj(win, 2, i);
                cancelBtn = d2_GetWinObj(win, 2, i + 1);
                if (ev->stage > 0)
                    d2_SetObjEnabled(pickBtn, 0);
                else
                    d2_SetObjEnabled(pickBtn, 1);
            } else if (state == 2) {
                d2_SetForm(win, 1);
                d2_RemoveWindowObjects(d2_LastWindow, 1);
                io_PrintText("$C");
                io_PrintText(0x198e, &ev->end, sermon->sermon.text1 + 0x1990, sermon->sermon.text2 + 0x1998,
                             priest->id);
                row.x = d2_LastWindowPtr->w / 2 - d2_Pics[0x5da].w / 2;
                row.y = 0xc3;
                row.pickable = 1;
                row.rec = priest;
                ai_AmountRowStatus(&row);
                ai_AmountRow(row.x, row.y, d2_LastWindow, 0x54, &row, 1);
                d2_SetForm(win, 2);
                d2_RemoveWindowObjects(d2_LastWindow, 1);
                io_PrintText("$C");
                i = io_PrintText(0x198f);
                clearBtn = d2_GetWinObj(win, 2, i);
                cancelBtn = d2_GetWinObj(win, 2, i + 1);
                if (ev->stage > 0)
                    d2_SetObjEnabled(clearBtn, 0);
                else
                    d2_SetObjEnabled(clearBtn, 1);
            }
            changed = 0;
        }
        if (dm_L.rup != 0 || state == 3) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == payBtn) {
                if (sv_CheckMoney(ai_Chars[ai_PlayerId], 1) != 0) {
                    flag = 1;
                    gm_SendMoneyDelta(((AICHARREC *)ai_Chars)[ai_PlayerId].netId, -1);
                    cm_BeginFieldBlock((char *)sermon, sermon->objId);
                    cm_AddFieldDelta(1, 1, 0x22, &flag);
                    slot = cm_SendFieldDeltas();
                    changed = 1;
                    state = 1;
                    while (slot != -1 && cm_GetSlotState(slot) == 0)
                            sv_RunFrame(0, 0x677c7);
                }
            } else if (d2_PointerObj == pickBtn) {
                memset(&cfg, 0, sizeof(cfg));
                cfg.mode = 0x400;
                cfg.kind = 6;
                cfg.classMask[0] = gypsyFilterWords[0];
                cfg.classMask[1] = gypsyFilterWords[1];
                txt_Format(title, 0x19a2);
                priest = (AICHARREC *)ai_AmountMenu(&cfg, title, 0, 0);
                sermon = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0x1b5);
                if (priest != 0 && priest->netId != -1 && sermon != 0) {
                    text1 = util_Random(8);
                    text2 = util_Random(8);
                    cm_BeginFieldBlock((char *)sermon, sermon->objId);
                    cm_AddFieldDelta(4, 1, 0x1e, &priest->netId);
                    cm_SendFieldDeltas();
                    cm_BeginFieldBlock((char *)sermon, sermon->objId);
                    cm_AddFieldDelta(1, 1, 0x1c, &text1);
                    cm_SendFieldDeltas();
                    cm_BeginFieldBlock((char *)sermon, sermon->objId);
                    cm_AddFieldDelta(1, 1, 0x1d, &text2);
                    slot = cm_SendFieldDeltas();
                    state = 2;
                    changed = 1;
                    while (slot != -1 && cm_GetSlotState(slot) == 0)
                            sv_RunFrame(0, 0x677c7);
                }
            } else if (d2_PointerObj == clearBtn) {
                none = -1;
                noPriest = -1;
                text1 = -1;
                cm_BeginFieldBlock((char *)sermon, sermon->objId);
                cm_AddFieldDelta(4, 1, 0x1e, &noPriest);
                cm_SendFieldDeltas();
                cm_BeginFieldBlock((char *)sermon, sermon->objId);
                cm_AddFieldDelta(1, 1, 0x1c, &none);
                cm_SendFieldDeltas();
                cm_BeginFieldBlock((char *)sermon, sermon->objId);
                cm_AddFieldDelta(1, 1, 0x1d, &text1);
                slot = cm_SendFieldDeltas();
                changed = 1;
                state = 1;
                while (slot != -1 && cm_GetSlotState(slot) == 0)
                        sv_RunFrame(0, 0x677c7);
            } else if (d2_PointerObj == cancelBtn) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_SermonWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}

void ai_StorageWindow(); int ai_ProductionWindow();
int ai_OpenTransport(); void ai_MasterLetterWindow();

/* 0x5a36c0, the production contact of the room 0xf7, the writing desk (330 B): the pump 0x5309a0 (0x5a36c0,
   0x67fff) runs the market contact (0x58f510, the building) each frame and, while the building permission 0x200
   (0x6f231c) is set, looks up the plates "contact_PRODUKTION_SCHREIBEN" (txt_Ptrs[0x1fae], class 0x13),
   "contact_LAGER" (class 0xe), "contact_TRANSPORT" (class 0x15), "ob_PERSONALBUCH" (class 0xc) and
   "ob_MEISTERBRIEF" (class 0xc); a click opens the personnel book (0x5aab80), the store window (0x5b26b0, the
   building's object list), the production window (0x58bd60), the transport window (0x5af1a0) or the master
   letter (0x5c3200) */

void ai_ProductionWriteContact(int building, int room)
{
    int meisterbrief = 0;
    int lager = 0;
    int schreiben = 0;
    int personalbuch = 0;
    int transport = 0;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_ProductionWriteContact, 0x67fff) != 0) {
        ai_MarketContact((AIBLD *)building);
        if ((ai_BuildingPerms & 0x200) != 0) {
            schreiben = sv_RegisterNamePlate("contact_PRODUKTION_SCHREIBEN", txt_Ptrs[0x1fae], 0x13);
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
            personalbuch = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            meisterbrief = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0xc);
        }
        if (ai_ClickedObject != 0 && (ai_BuildingPerms & 0x200) != 0) {
            if (ai_ClickedObject == personalbuch)
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == lager)
                ai_StorageWindow(building, ((AIBLD *)building)->objList);
            else if (ai_ClickedObject == schreiben)
                ai_ProductionWindow((AIBLD *)building);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport(building);
            else if (ai_ClickedObject == meisterbrief)
                ai_MasterLetterWindow(building);
        }
    }
}
/* 0x5a3810, the church listener, the rooms 0xe5 / 0xe6 handler of the enter-building driver (412 B): the
   pump 0x5309a0 (0x5a3810, 0x67fff); with the building permission 0x200 (0x6f231c) the plates
   "contact_PREDIGT" (txt_Ptrs[0x198d]) and "ob_WEIHWASSERBECKEN" (txt_Ptrs[0x1975], both class 0x16); for a
   building of another owner (the owner slot neither the player nor 0xffff) the confession room (0x4ffe90 on
   the building's object list, 2, 6, 0, 0xf2) and the plates "ob_KIRCHE_BEICHTSTUHLRAUM" (txt_Ptrs[0x197e]),
   "ob_SPENDENKAESTCHEN" (txt_Ptrs[0x1985]) and "ob_WEIHWASSERBECKEN"; with no click, the key '0' (0x1468410)
   and gm_AllowAll (0x6f2448) set the confession window opens itself; the clicks open the sermon window
   (0x5a2e60), the baptism window (0x5a23a0, the room) or the confession / donation windows (0x5a2b60 / 0x5a1af0) */

void ai_ChurchListener(int building, int room)
{
    int beichtstuhl = 0;
    int spendenkasten = 0;
    int predigt = 0;
    int becken = 0;
    int confessionRoom;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_ChurchListener, 0x67fff) != 0) {
        if ((ai_BuildingPerms & 0x200) != 0) {
            predigt = sv_RegisterNamePlate("contact_PREDIGT", txt_Ptrs[0x198d], 0x16);
            becken = sv_RegisterNamePlate("ob_WEIHWASSERBECKEN", txt_Ptrs[0x1975], 0x16);
        }
        if (((AIBLD *)building)->owner != ai_PlayerId && ((AIBLD *)building)->owner != 0xffff) {
            confessionRoom = gm_Query(((AIBLD *)building)->objList, 2, 6, 0, 0xf2);
            if (confessionRoom != 0)
                beichtstuhl = sv_RegisterNamePlate("ob_KIRCHE_BEICHTSTUHLRAUM", txt_Ptrs[0x197e], 0x16);
            else
                beichtstuhl = 0;
            spendenkasten = sv_RegisterNamePlate("ob_SPENDENKAESTCHEN", txt_Ptrs[0x1985], 0x16);
            becken = sv_RegisterNamePlate("ob_WEIHWASSERBECKEN", txt_Ptrs[0x1975], 0x16);
        }
        if (ai_ClickedObject != 0) {
            if ((ai_BuildingPerms & 0x200) != 0) {
                if (ai_ClickedObject == predigt)
                    ai_SermonWindow(building);
                else if (ai_ClickedObject == becken)
                    ai_BaptismWindow(building, room);
            }
            if (((AIBLD *)building)->owner != ai_PlayerId) {
                if (ai_ClickedObject == beichtstuhl)
                    ai_ConfessionWindow(building);
                if (ai_ClickedObject == spendenkasten)
                    ai_DonationWindow((AIBLD *)building);
                if (ai_ClickedObject == becken)
                    ai_BaptismWindow(building, room);
            }
        } else {
            if (kb_Last == '0' && gm_AllowAll != 0)
                ai_ConfessionWindow(building);
        }
    }
}
/* 0x5a39b0, the church's top-5 / indulgence listener (141 B): the pump 0x5309a0 (this, 0x67fff) looks up
   "contact_TOP5" (0x1440768) and "contact_ABLASS" (0x144073c, both 0x16) and opens 0x5a28f0 / 0x5a1f70 */

void ai_ChurchContact(int building, int arg)
{
    int top5;
    int ablass;
    int click;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_ChurchContact, 0x67fff) != 0) {
        top5 = sv_RegisterNamePlate("contact_TOP5", ai_ContactTextTop5, 0x16);
        ablass = sv_RegisterNamePlate("contact_ABLASS", ai_ContactTextIndulgence, 0x16);
        click = ai_ClickedObject;
        if (click != 0) {
            if (click == top5)
                ai_Top5Window();
            else if (click == ablass)
                ai_IndulgenceWindow((AIBLD *)building, arg);
        }
    }
}


#pragma pack(push, 1)
typedef struct G8CHARREC {              /* a character record of ai_Chars (AICHAR, ai_char.h): the fields the
                                           news messages of this unit read */
    unsigned short id;                  /* +0x000, the character's id */
    unsigned char kind;                 /* +0x002, the character kind */
    char pad03;
    int netId;                          /* +0x004, the id the news events carry */
    unsigned char health;               /* +0x008, > 0 = alive */
    unsigned char sex;                  /* +0x009, 0 / 1 (the text's feminine form) */
    char pad0a[2];                      /* +0x00a .. 0x00b */
    unsigned char b00c;                 /* +0x00c, a kind the person tooltip names (AICHAR.b00c); picks the news text */
    char pad0d[0x23];                   /* +0x00d .. 0x02f */
    union {
        char name[0x58];                /* +0x030, the character's name (AICHAR.name) */
        struct {
            char first[0x10];           /* +0x030 */
            char family[0x10];          /* +0x040 */
        };
        struct {
            char padname5c[0x2c];       /* +0x030 .. 0x05b */
            int partner;                /* +0x05c, the netId of the character's partner (AICHAR.relatives[0]) */
            int relatives[7];           /* +0x060 .. 0x07b */
        };
    };
    char pad88[0xdd];                   /* +0x088 .. 0x164 */
    unsigned char title;                /* +0x165, the title rank */
    char pad166[6];                     /* +0x166 .. 0x16b */
    AIBLD *bld;                         /* +0x16c, the building the character is in */
    char pad170[0x24];                  /* +0x170 .. 0x193 */
    int money;                          /* +0x194, the character's money (AICHAR.money) */
    char pad198[0x19];                  /* +0x198 .. 0x1b0 */
    unsigned char heldRounds;           /* +0x1b1, the rounds the character is held */
    char pad1b2[0x6a];                  /* +0x1b2 .. 0x21b */
} G8CHARREC;
#pragma pack(pop)

/* 0x5a3a40, the dungeon plan window "locations\kerker\kerkerplan" (219 B): the room 0x4ffe90 (bld->objList,
   1, 0, 0xba); the window 0x4741f0 / 0x475600 / 0x475f70 (1) with the text 0x19ad, then every character
   record (ai_Chars + i * 0x21c up to gm_PlayerRecords) with gm_Homeless set gets the line 0x19ae (the count,
   its id) and, with no line, 0x19af; the pump 0x5309a0 (this, 0x677c7), the exit 0x52d7a0 / 0x4758f0 */
void ai_DungeonPlanWindow(AIBLD *bld)
{
    int room;
    int win;
    int n = 0;
    int i;

    room = gm_Query((void *)bld->objList, 1, 0, 0xba);
    if (room == 0)
        return;
    win = d2_OpenForm(0, 0, "locations\\kerker\\kerkerplan");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x19ad);
    d2_SetForm(win, 1);
    for (i = 0; i < 0x300; i++) {
        if (gm_Homeless(ai_Chars[i]) != 0)
            io_PrintText(0x19ae, n++ + 1, ((G8CHARREC *)ai_Chars[i])->id);
    }
    if (n == 0)
        io_PrintText(0x19af);
    for (;;) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (sv_RunFrame(ai_DungeonPlanWindow, 0x677c7) == 0)
            break;
    }
    sv_CameraFollow();
    d2_CloseView(win);
}
/* 0x5a3b20, the dungeon listener (100 B): the pump 0x5309a0 (this, 0x67fff) looks up "ob_KERKERPLAN" (0x1440870,
   class 0x16) and opens 0x5a3a40 (the dungeon plan window) */

void ai_DungeonContact(int building, int room)
{
    int plan;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_DungeonContact, 0x67fff) != 0) {
        plan = sv_RegisterNamePlate("ob_KERKERPLAN", ai_ContactTextDungeonPlan, 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == plan)
            ai_DungeonPlanWindow(building);
    }
}

int amt_GetFirstOffice(unsigned int k, struct AMTREC *out); void cm_BeginGroup(const char *s); int cm_EndGroup(const char *s);

/* 0x5a3b90, the dungeon's bribe window "locations\kerker\kerker_bestechen" (from ai_DungeonBribeListener; bld is not
   read): the jailer is the holder of the office 0x11 (amt_GetFirstOffice, gm_FindChar); without one the text
   0x19b5, while the player's bribe event 0x39 runs the text 0x19b6 (the jailer). Else the text 0x19b1 (the city
   twice, the jailer) with the amount slider (0x96 .. the player's money in the city's coin, gm_ConvertMoney, step
   0xfa) and the bribe button: the bribe message 0x39 to the jailer (arg: the player's netId, the jailer's netId,
   the amount back in base money, gm_PlayerFactor) goes in the group "kerkermeister_bestechen" with the payment
   (cm_SendMoveGoods) */
void ai_DungeonBribeWindow(AIBLD *bld)
{
    AMTREC office;
    int bribeBtn;
    char buf[0x800];
    AICHARREC *jailer;
    int win;
    int textId;
    int slider;
    int money;

    if (amt_GetFirstOffice(0x11, &office) == 0)
        return;
    jailer = (AICHARREC *)gm_FindChar(office.owner);
    if (jailer == 0) {
        sv_MessageBox(txt_Ptrs[0x19b5], 0);
        sv_CameraFollow();
        return;
    }
    if (he_FindEvent(2, 0, 0x39, 2, ai_PlayerId) != 0) {
        txt_Format(buf, 0x19b6, jailer->id);
        sv_MessageBox(buf, 0);
        sv_CameraFollow();
        return;
    }
    win = d2_OpenForm(0, 0, "locations\\kerker\\kerker_bestechen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    textId = io_PrintText(0x19b1, ai_OwnPlayer, ai_OwnPlayer, jailer->id);
    slider = d2_GetWinObj(win, 1, textId);
    bribeBtn = d2_GetWinObj(win, 1, textId + 1);
    money = gm_CharMoney(ai_PlayerId);
    d2_SetObjValue4(slider, 0x96, gm_ConvertMoney(money, ai_OwnPlayer), 0xfa);
    d2_SetPointerSprite(0, 0);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && d2_PointerObj == bribeBtn) {
            AIMSG msg;

            msg.type = 0x39;
            msg.to = jailer->netId;
            msg.fromId = -1;
            msg.flags = 9;
            msg.arg[0] = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
            msg.arg[1] = jailer->netId;
            msg.arg[2] = gm_PlayerFactor(d2_GetObjValue(slider), ai_OwnPlayer);
            cm_BeginGroup("kerkermeister_bestechen");
            cm_SendMoveGoods(jailer->netId, ((AICHARREC *)ai_Chars)[ai_PlayerId].netId, ai_OwnPlayer, msg.arg[2]);
            cm_SendLongMessage((char *)&msg);
            cm_EndGroup("kerkermeister_bestechen");
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame(ai_DungeonBribeWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
}
/* 0x5a3df0, the dungeon's bribe listener (245 B): the pump 0x5309a0 (this, the mask 0x677ff while the player's
   character is held (AICHAR.heldRounds), else 0x67fff) looks up "contact_BESTECHEN" (ai_ContactTextBribe, class
   0x10) only while it is held and opens the bribe window 0x5a3b90 for a known owner */

void ai_DungeonBribeListener(AIBLD *bld, int room)
{
    int bribe = 0;
    int mask;

    sv_ReleaseNamePlates();
    mask = ((AICHARREC *)ai_Chars)[ai_PlayerId].heldRounds > 0 ? 0x677ff : 0x67fff;
    while (sv_RunFrame(ai_DungeonBribeListener, mask) != 0) {
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].heldRounds > 0)
            bribe = sv_RegisterNamePlate("contact_BESTECHEN", ai_ContactTextBribe, 0x10);
        if (((AICHARREC *)ai_Chars)[ai_PlayerId].heldRounds > 0 && ai_ClickedObject != 0 &&
            bld->owner != 0xffff && ai_ClickedObject == bribe)
            ai_DungeonBribeWindow(bld);
        mask = ((AICHARREC *)ai_Chars)[ai_PlayerId].heldRounds > 0 ? 0x677ff : 0x67fff;
    }
}
/* 0x5a3ef0, an empty listener (28 B): the pump 0x5309a0 (this, 0x67fff) until it returns 0 */

void ai_DungeonEmptyListener(int building, int room)
{
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_DungeonEmptyListener, 0x67fff) != 0)
        ;
}

int he_NoProtectionEvent(); void snd_PlayFavourComment(void); 

/* 0x5a3f10, the burglary window "locations\diebesgilde\diebesgilde_einbruch" (the map pick's callback: ai_MapPick
   calls it with its context, not read here, and the picked building bld): a protected building
   (he_NoProtectionEvent) gives the text 0x1a92 (the owner, bld), one beyond the rank the player's guild building
   allows (gm_BuildingRankGate) the text 0x19e5. Else the text 0x19e4 (the
   owner, bld) and the buttons: the first sends the burglary message 0x3f (to the player, arg: bld's id, the guild
   building's id; ids: the selected characters, AICHARREC.selected) with snd_PlayFavourComment, or shows the text
   0x19e3 when none is selected, and clears the selection. Returns 1 when the first button was clicked */
int ai_BurglaryWindow(int ctx, AIBLD *bld)
{
    int done;
    int burgleBtn;
    AIMSG msg;
    char buf[0x400];
    AICHARREC *owner;
    int win;
    int textId;
    int cancelBtn;
    int n;
    int i;

    done = 0;
    if (bld == 0)
        return 0;
    owner = &((AICHARREC *)ai_Chars)[bld->owner];
    if (he_NoProtectionEvent((int)bld) == 0) {
        txt_Format(buf, 0x1a92, bld->owner, bld);
        sv_MessageBox(buf, 0);
        return 0;
    }
    if (gm_BuildingRankGate(ai_PlayerBuilding, bld) == 0) {
        sv_MessageBox(txt_Ptrs[0x19e5], 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_einbruch");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    textId = io_PrintText(0x19e4, owner->id, bld);
    burgleBtn = d2_GetWinObj(win, 1, textId);
    cancelBtn = d2_GetWinObj(win, 1, textId + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == burgleBtn) {
                n = 0;
                msg.type = 0x3f;
                msg.flags = 1;
                msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                msg.arg[0] = bld->id;
                msg.arg[1] = ((AIBLD *)ai_PlayerBuilding)->id;
                memset(msg.ids, -1, sizeof(msg.ids));
                for (i = 0; i < 768; i++) {
                    if (((AICHARREC *)ai_Chars)[i].selected != 0)
                        msg.ids[n++] = ((AICHARREC *)ai_Chars)[i].netId;
                }
                if (n != 0) {
                    cm_SendLongMessage((char *)&msg);
                    snd_PlayFavourComment();
                } else {
                    sv_MessageBox(txt_Ptrs[0x19e3], 0);
                }
                sv_ClearSelection();
                sv_PopRequest = 1;
                done = 1;
            } else if (d2_PointerObj == cancelBtn) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_BurglaryWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return done;
}
/* 0x5a4130, the burglary start of the thieves' guild hall, the click of its contact_EINBRUCH plate (158 B): a
   building whose character is free (0x520fe0) zeroes the 20-int map pick record, sets {+4 = 0x400, +8 =
   0xa07443b4, +0xc = 1, +0x10 = 7, +0x4c = 0x727}, opens the player bar (0x525220, the building) and the map
   pick 0x5b2180 (1, the record, the text 0x19e2, the burglary window 0x5a3f10, 0), then closes the bar
   (0x525440) and clears the selection (0x52ae40) */

void ai_BurglaryStart(int building)
{
    int req[20];
    char buf[0x80];
    int i;

    if (sv_CheckCharacterFlag(building) != 0)
        return;
    for (i = 0; i < 20; i++)
        req[i] = 0;
    req[1] = 0x400;
    if (building == 0)
        return;
    sv_OpenPlayerBar(building, 0);
    txt_Format(buf, 0x19e2);
    req[19] = 0x727;
    req[4] = 7;
    req[2] = 0xa07443b4;
    req[3] = 1;
    ai_MapPick(1, req, buf, (int)ai_BurglaryWindow, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}

char * he_NextEvent(void);

/* 0x5a41d0, the thieves' guild spying window "locations\diebesgilde\diebesgilde_spionage" (the map pick's callback:
   ai_MapPick calls it with its context, not read here, and the picked building target; nothing for none or an
   ownerless one). A building without a protection event (he_NoProtectionEvent 0) gives the text 0x1a92. The spy
   reports (objects 0xca) the player's building holds on other buildings and the player's running spy events 0x40
   on buildings no such report covers count against twice the level of the player's building type: at the limit
   the text 0x19fa. Else the text 0x19f7 (the owner, target) and the buttons: the first posts the spy message 0x40
   (the target, the guild building, up to eight selected characters as ids, flags 1; the text 0x1a5e when nobody
   is selected), the second and a right click close it. The selection is cleared after the window; returns 1 once
   the spying was ordered */
int ai_GuildSpyWindow(int ctx, AIBLD *target)
{
    int done;
    int uncovered;
    AIMSG msg;
    AIOBJ *reports[0x10];
    char limitText[0x100];
    char text[0x400];
    int win;
    int ok;
    int cancel;
    int base;
    int n;
    int i;
    AIBTYPE *type;
    AIBLD *bld;
    AIOBJ *o;
    AIJOB *ev;
    AICHARREC *ch;

    bld = 0;
    uncovered = 0;
    done = 0;
    if (target == 0 || target->owner == 0xffff)
        return 0;
    if (he_NoProtectionEvent(target) == 0) {
        txt_Format(text, 0x1a92, target->owner, target);
        sv_MessageBox(text, 0);
        return 0;
    }
    n = 0;
    if (ai_PlayerBuilding != 0)
        bld = (AIBLD *)ai_PlayerBuilding;
    else if (ai_FallbackBuilding != 0)
        bld = (AIBLD *)ai_FallbackBuilding;
    type = &((AIBTYPE *)ai_BTypes)[((AIBLD *)ai_PlayerBuilding)->type];
    if (bld != 0)
        for (o = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0xca); o != 0; o = (AIOBJ *)gm_QueryNext())
            if (o->reportBld != target->id)
                reports[n++] = o;
    for (ev = (AIJOB *)he_FindEvent(2, 2, ai_PlayerId, 0, 0x40); ev != 0; ev = (AIJOB *)he_NextEvent()) {
        for (i = 0; i < n; i++)
            if (ev->bldId == reports[i]->reportBld)
                break;
        if (i >= n)
            uncovered++;
    }
    n += uncovered;
    if (n >= type->level * 2) {
        txt_Format(limitText, 0x19fa, n, ai_PlayerBuilding);
        sv_MessageBox(limitText, 0);
        return 0;
    }
    ch = &((AICHARREC *)ai_Chars)[target->owner];
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_spionage");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    base = io_PrintText(0x19f7, ch->id, target);
    ok = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            memset(msg.ids, 0xff, sizeof(msg.ids));
            msg.type = 0x40;
            msg.flags = 1;
            msg.to = ai_CharInts[ai_PlayerId][0];
            msg.arg[0] = target->id;
            msg.arg[1] = ((AIBLD *)ai_PlayerBuilding)->id;
            n = 0;
            for (i = 0; i < 0x300 && n < 8; i++)
                if (((AICHARREC *)ai_Chars)[i].selected != 0)
                    msg.ids[n++] = ai_CharInts[i][0];
            if (n != 0) {
                cm_SendLongMessage((char *)&msg);
                done = 1;
                snd_PlayFavourComment();
            } else
                sv_MessageBox(txt_Ptrs[0x1a5e], 0);
            sv_PopRequest = 1;
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_GuildSpyWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClearSelection();
    return done;
}
/* 0x5a44e0, starts the espionage against the building bld (216 B): the character flag check 0x520fe0; the
   player bar (0x525220, bld, 0); the map-pick request {0, 0x400, (building class 0x13 ? 0x90010 : 0xa7fd43b4),
   the flag, 7, ..., 0x727} and 0x5b2180 (1, the request, the text 0x19f6, the callback 0x5a41d0, 0), then the
   bar close 0x525440 and the selection clear 0x52ae40 */

void ai_EspionageStart(AIBLD *bld)
{
    int req[20];
    int i;
    char buf[0x400];

    if (sv_CheckCharacterFlag(bld) != 0)
        return;
    for (i = 0; i < 20; i++)
        req[i] = 0;
    req[1] = 0x400;
    if (bld == 0)
        return;
    sv_OpenPlayerBar((int)bld, 0);
    req[19] = 0x727;
    req[4] = 7;
    if (((AIBTYPE *)ai_BTypes)[bld->type].kind == 0x13) {
        req[2] = 0x90010;
        req[3] = 0;
    } else {
        req[2] = 0xa7fd43b4;
        req[3] = 1;
    }
    txt_Format(buf, 0x19f6);
    ai_MapPick(1, req, buf, (void *)(unsigned)&ai_GuildSpyWindow, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}

unsigned char * gm_BuildQuery(int n, ...); int gm_BuildingValues(); int d2_CreateRadioGroup(int n, ...); void d2_RadioAdd(int g, int obj);
void d2_RadioClear(int g); int d2_RadioGetSel(); int cm_SendMoveObject(int a, int b, int c); void cm_SendDeleteObject(int id);
int d2_GetViewWin(); void d2_CreateScrollButtons(); void d2_ClearInput(void);

/* 0x5a45c0, the spy reports window "locations\diebesgilde\diebesgilde_informationen" of the guild building bld: the
   text 0x19fb, the delete button 0x1ba6 and the scrolled list (d2_GetViewWin 0x767). On a rebuild the list shows up
   to eight of bld's spy reports (objects 0xca) whose building still has an owner, each a radio row with the text
   0x19fc (the building type's name 0x427 + kind, the owner, the building, AIOBJ.reportStamp, its two
   gm_BuildingValues, reportKnow, reportGuard) and the building as the row's data; a report whose building is gone is
   deleted (cm_SendDeleteObject). The button moves the selected report out of bld (cm_SendMoveObject to -1),
   waits for its slot (sv_PumpNet) and rebuilds the list. The row counter is not reset by a rebuild */
void ai_SpyReportsWindow(AIBLD *bld)
{
    int nRows;
    int j;
    int deleteBtn;
    int values[2];
    AIOBJ *reports[8];
    int rows[16];
    int win;
    int rebuild;
    int radio;
    int n;
    int sel;
    int slot;
    int row;
    AIOBJ *o;
    AIBLD *target;

    rebuild = 1;
    nRows = 0;
    radio = -1;
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_informationen");
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x19fb);
    d2_SetPointerSprite(0, 0);
    d2_ClearInput();
    d2_SetForm(win, 3);
    deleteBtn = d2_GetWinObj(win, 3, io_PrintText(0x1ba6));
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, d2_GetViewWin(win, 2), 0x767);
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (rebuild) {
            memset(reports, 0, sizeof(reports));
            if (radio != -1) {
                d2_RadioClear(radio);
                radio = -1;
            }
            d2_SetForm(win, 2);
            io_PrintText("$C");
            n = 0;
            for (o = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0xca); o != 0 && n < 8;
                 o = (AIOBJ *)gm_QueryNext()) {
                target = (AIBLD *)gm_BuildQuery(1, 1, o->reportBld);
                if (target != 0 && target->owner != 0xffff)
                    reports[n++] = o;
            }
            for (j = 0; j < 8; j++) {
                o = reports[j];
                if (o == 0)
                    continue;
                target = (AIBLD *)gm_BuildQuery(1, 1, o->reportBld);
                if (target != 0) {
                    if (target->owner != 0xffff) {
                        gm_BuildingValues(target, values);
                        row = d2_GetWinObj(win, 2, io_PrintText(0x19fc, target->type + 0x427, target->owner, target,
                                                                &o->reportStamp, values[0], values[1], o->reportKnow,
                                                                o->reportGuard));
                        rows[nRows] = row;
                        d2_Objs[row].user = target;
                        if (radio == -1) {
                            radio = d2_CreateRadioGroup(1, row);
                            d2_Objs[row].checked = 1;
                            d2_Objs[row].highlighted = 1;
                            d2_Objs[row].user = target;
                            d2_Radios[radio].sel = 0;
                        } else {
                            d2_RadioAdd(radio, row);
                        }
                        nRows++;
                    }
                } else {
                    cm_SendDeleteObject(o->objId);
                }
            }
            rebuild = 0;
        }
        if (d2_ClickedId != -1 && d2_PointerObj == deleteBtn && radio != -1 && (sel = d2_RadioGetSel(radio)) >= 0) {
            slot = cm_SendMoveObject(bld->id, -1, reports[sel]->objId);
            while (cm_GetSlotState(slot) == 0)
                sv_PumpNet();
            rebuild = 1;
        }
    } while (sv_RunFrame(ai_SpyReportsWindow, 0x677c7) != 0);
    if (radio != -1)
        d2_RadioClear(radio);
    sv_CameraFollow();
    d2_CloseView(win);
}


/* 0x5a48d0, the thieves' guild hall's attack window (ai_AttackStart's map pick callback: its context, not read
   here, and the picked building target), in the form "locations\diebesgilde\taschendiebstahl" with the text 0x19ee
   (the target's owner); the first button orders the attack, the second and a right click close it. With an
   attack event 0x49 of the player's guild building already running on target, the selected scene characters
   join it (sv_MoveToGroup); else the attack message 0x49 (from the guild, flags 1, arg[0] the target, arg[1] the
   guild, up to eight selected scene characters as ids) is sent, or with nobody selected the text 0x1a49.
   Returns 1 once the attack was ordered */
int ai_AttackWindow(int ctx, AIBLD *target)
{
    int done;
    int ok;
    int cancel;
    AIMSG msg;
    int win;
    int base;
    int n;
    int *pp;
    AICHARREC *ch;
    AIJOB *ev;

    done = 0;
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\taschendiebstahl");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    base = io_PrintText(0x19ee, target->owner, target);
    ok = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            for (ev = (AIJOB *)he_FindEvent(3, 0, 0x49, 2, ((AIBLD *)ai_PlayerBuilding)->owner,
                                            3, ((AIBLD *)ai_PlayerBuilding)->id);
                 ev != 0; ev = (AIJOB *)he_NextEvent())
                if (ev->bldId == target->id)
                    break;
            if (ev == 0) {
                msg.type = 0x49;
                msg.to = ai_CharInts[((AIBLD *)ai_PlayerBuilding)->owner][0];
                msg.fromId = ((AIBLD *)ai_PlayerBuilding)->id;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.arg[0] = target->id;
                msg.arg[1] = ((AIBLD *)ai_PlayerBuilding)->id;
                memset(msg.ids, 0xff, sizeof(msg.ids));
                n = 0;
                for (pp = ai_SceneChars; (int)pp < (int)&ai_SceneChars[32] && n < 8; pp++) {  /* signed compares (jl) */
                    ch = (AICHARREC *)*pp;
                    if (ch != 0 && ch->selected != 0)
                        msg.ids[n++] = ch->netId;
                }
                if (n == 0)
                    sv_MessageBox(txt_Ptrs[0x1a49], 0);
                else {
                    done = 1;
                    cm_SendLongMessage((char *)&msg);
                    snd_PlayFavourComment();
                    sv_PopRequest = 2;
                }
            } else {
                done = 1;
                sv_PopRequest = 2;
                for (pp = ai_SceneChars; (int)pp < (int)&ai_SceneChars[32]; pp++) {
                    ch = (AICHARREC *)*pp;
                    if (ch != 0 && ch->selected != 0)
                        sv_MoveToGroup(0, 0, ch->id, (char *)ev);
                }
            }
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_AttackWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return done;
}
/* 0x5a4b30, the attack start of the thieves' guild hall, the click of its contact_ANGRIFF plate (95 B): the
   20-int map pick record is zeroed, its +4 = 0x400, +8 = 0x90010 and +0x10 = 7; the player bar opens (0x525220,
   the building), the map pick runs (0x5b2180: 1, the record, the prompt text ai_MapPickPrompt, the attack window
   ai_AttackWindow as callback, 0), then the player bar closes (0x525440) and the selection is cleared (0x52ae40) */

void ai_AttackStart(int building)
{
    int req[20];
    int i;

    for (i = 0; i < 20; i++)
        req[i] = 0;
    req[1] = 0x400;
    req[4] = 7;
    req[2] = 0x90010;
    sv_OpenPlayerBar(building, 0);
    ai_MapPick(1, req, ai_MapPickPrompt, (int)ai_AttackWindow, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}


/* 0x5a4b90, the thieves' guild's pickpocket gang window (ai_ThievesGuildListener; nothing when
   sv_CheckCharacterFlag refuses the building): the player bar of bld and the form
   "locations\diebesgilde\taschendiebstahl" with the text 0x19f0; the member list updates every frame. The first
   button sends the selected scene characters (up to six, collected over the clicks) as the gang message 0x61 from
   bld (to and leader the owner's netId, flags 1), clears the selection and closes; with nobody selected the text
   0x19e3. The second button and a right click close it; then the player bar closes and the camera follows again */
void ai_PickpocketGangWindow(AIBLD *bld)
{
    AIMSG msg;
    int win;
    int n;
    int *pp;
    AICHARREC *ch;

    if (sv_CheckCharacterFlag(bld) != 0)
        return;
    sv_OpenPlayerBar((int)bld, 0);
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\taschendiebstahl");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    io_PrintText(0x19f0);
    d2_ClickedId = -1;
    memset(msg.ids, 0xff, 6 * sizeof(int));  /* the gang's six members (HEGANG: AIJOB.ids[0..5]) */
    n = 0;
    do {
        sv_UpdateMemberList((unsigned char *)bld);
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_ClickedId == 0) {
            for (pp = ai_SceneChars; (int)pp < (int)&ai_SceneChars[32] && n < 6; pp++) {  /* signed compares (jl) */
                ch = (AICHARREC *)*pp;
                if (ch != 0 && ch->selected != 0)
                    msg.ids[n++] = ch->netId;
            }
            if (n != 0) {
                msg.type = 0x61;
                msg.to = ai_CharInts[bld->owner][0];
                msg.fromId = bld->id;
                msg.time = ai_Time;
                msg.arg[10] = msg.to;  /* 0x80: the gang's leader (the event's HEGANG.leader, 0x54 further on) */
                msg.flags = 1;
                cm_SendLongMessage((char *)&msg);
                snd_PlayFavourComment();
                sv_ClearSelection();
                sv_PopRequest = 1;
            } else
                sv_MessageBox(txt_Ptrs[0x19e3], 0);
        } else if (d2_ClickedId == 1)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_PickpocketGangWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_ClosePlayerBar();
    sv_CameraFollow();
}
/* 0x5a4d40, the thieves' guild listener (522 B, room 0x54 of the building driver 0x59e420): 0x52e190, the
   great sword object 0x449690 (0, 0xc0, 0) shown by 0x447300 only from the building type's level byte +0x247
   >= 2, then the pump 0x5309a0 (this, 0x67fff) fetches the contact_EINBRUCH / contact_GEBAEUDE_AUSSPIONIEREN /
   contact_ANGRIFF (only with the level byte >= 2) / contact_TASCHENDIEBSTAHL / ob_INFORMATIONSPERGAMENT plates
   through 0x52e1c0 when the mask 0x200 is set, ob_PERSONALBUCH / ob_MEISTERBRIEF when 0x400 is set, and
   dispatches the pending click 0x6f22e4 to ai_BurglaryStart / ai_EspionageStart / ai_AttackStart,
   ai_PickpocketGangWindow / ai_SpyReportsWindow / ai_PersonnelBook / ai_MasterLetterWindow */

void ai_ThievesGuildListener(int building, int room)
{
    int burglary = 0;
    int espionage = 0;
    int angriff = 0;
    int meister = 0;
    int taschen = 0;
    int personal = 0;
    int pergament = 0;
    int obj;
    int click;
    AIBLD *bld;
    AIBTYPE *type;

    sv_ReleaseNamePlates();
    bld = (AIBLD *)building;
    type = (AIBTYPE *)ai_BTypes + bld->type;
    if (type->level >= 2) {
        obj = d3_FindObject(0, 0xc0, 0, "ub_GROSSSCHWERT");
        if (obj != 0)
            d3if_ShowTree(obj, 1);
    } else {
        obj = d3_FindObject(0, 0xc0, 0, "ub_GROSSSCHWERT");
        if (obj != 0)
            d3if_ShowTree(obj, 0);
    }
    if (sv_RunFrame(ai_ThievesGuildListener, 0x67fff) == 0)
        return;
    do {
        if (ai_BuildingPerms & 0x200) {
            burglary = sv_RegisterNamePlate("contact_EINBRUCH", ai_ContactTextBurglary, 0x17);
            espionage = sv_RegisterNamePlate("contact_GEBAEUDE_AUSSPIONIEREN", ai_ContactTextSpyBuilding, 0x17);
            if (type->level >= 2)
                angriff = sv_RegisterNamePlate("contact_ANGRIFF", ai_ContactTextAttack, 0x17);
            taschen = sv_RegisterNamePlate("contact_TASCHENDIEBSTAHL", ai_ContactTextPickpocket, 0x17);
            pergament = sv_RegisterNamePlate("ob_INFORMATIONSPERGAMENT", ai_ContactTextInformation, 0x16);
        }
        if (ai_BuildingPerms & 0x400) {
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
        }
        click = ai_ClickedObject;
        if (click != 0) {
            if (click == burglary)
                ai_BurglaryStart(building);
            else if (click == espionage)
                ai_EspionageStart(bld);
            else if (click == angriff)
                ai_AttackStart(building);
            else if (click == taschen)
                ai_PickpocketGangWindow(building);
            else if (click == personal)
                ai_PersonnelBook(building, "ob_PERSONALBUCH");
            else if (click == pergament)
                ai_SpyReportsWindow((AIBLD *)building);
            else if (click == meister)
                ai_MasterLetterWindow(building);
        }
    } while (sv_RunFrame(ai_ThievesGuildListener, 0x67fff) != 0);
}
/* 0x5a4f50, an empty listener (28 B, room 0x4d of the building driver 0x59e420): 0x52e190 and the pump
   0x5309a0 (this, 0x67fff) until it returns 0 */

void ai_ThievesGuildEmptyListener(int building, int room)
{
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_ThievesGuildEmptyListener, 0x67fff) != 0)
        ;
}
/* 0x5a4f70, the storage-room listener (260 B, room 0x5c of the building driver 0x59e420): 0x52e190, the pump
   0x5309a0 (this, 0x67fff); with the mask 0x200 the contact_AUSRUESTEN / contact_REGENERATION plates, with 0x400
   the contact_LAGER / contact_TRANSPORT plates; the pending click 0x6f22e4 opens ai_StorageWindow (the building,
   the shown room's object list) for LAGER, ai_OpenTransport for TRANSPORT, ai_EquipWindow for AUSRUESTEN and
   ai_TrainingWindow for REGENERATION */

void ai_StorageContact(int building, int room)
{
    int lager = 0;
    int transport = 0;
    int ausruesten = 0;
    int regeneration = 0;
    int click;

    sv_ReleaseNamePlates();
    if (sv_RunFrame(ai_StorageContact, 0x67fff) == 0)
        return;
    do {
        if (ai_BuildingPerms & 0x200) {
            ausruesten = sv_RegisterNamePlate("contact_AUSRUESTEN", ai_ContactTextEquip, 0xd);
            regeneration = sv_RegisterNamePlate("contact_REGENERATION", ai_ContactTextRegeneration, 0x17);
        }
        if (ai_BuildingPerms & 0x400) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
        }
        click = ai_ClickedObject;
        if (click != 0) {
            if (click == lager)
                ai_StorageWindow(building, ((AIROOM *)sv_PanelShown)->list);
            else if (click == transport)
                ai_OpenTransport(building);
            else if (click == ausruesten)
                ai_EquipWindow(building);
            else if (click == regeneration)
                ai_TrainingWindow(building);
        }
    } while (sv_RunFrame(ai_StorageContact, 0x67fff) != 0);
}
/* 0x5a5080, the training-room listener (200 B, room 0x60 of the building driver 0x59e420): 0x52e190, the pump
   0x5309a0 (this, 0x67fff); with the mask 0x200 the ob_ZIELSCHEIBE / contact_NACHT plates, and when the mask is
   still set and the pending click 0x6f22e4 is one of them, ai_TrainingWindow (the building) */

void ai_TrainingContact(int building, int room)
{
    int ziel = 0;
    int nacht = 0;

    sv_ReleaseNamePlates();
    if (sv_RunFrame(ai_TrainingContact, 0x67fff) == 0)
        return;
    do {
        if (ai_BuildingPerms & 0x200) {
            ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", ai_ContactTextTarget, 0xc);
            nacht = sv_RegisterNamePlate("contact_NACHT", ai_ContactTextTraining, 0xc);
        }
        if ((ai_BuildingPerms & 0x200) && ai_ClickedObject != 0 &&
            (ai_ClickedObject == ziel || ai_ClickedObject == nacht))
            ai_TrainingWindow(building);
    } while (sv_RunFrame(ai_TrainingContact, 0x67fff) != 0);
}

int ai_PickActionBuilding(struct AIPER *p, int a, __int64 mask, float lo, float hi, int *out); int gv_Accuse();

#define LC_KIDNAP_PLACES ((1 << 5) | (1 << 7) | (1 << 15) | (1 << 22))  /* 0x4080a0, the building kinds a kidnapping may take the victim to (the ai_PickActionBuilding mask) */
/* 0x5a5120, the thieves' guild kidnapping window "locations\diebesgilde\diebesgilde_entfuehrung" for the character
   victim: nothing for none, when the player cannot pay 3 (sv_CheckMoney), or when the victim is in a building of
   its own; a victim already held gives the text 0x19ba. The place is the victim's own building, else one picked
   by ai_PickActionBuilding (the building kinds 0x4080a0) through gm_BuildQuery. The text 0x19b9 and two buttons:
   the first sends the kidnapping message 0x3c (to the player's netId, flags 1, arg[0] the victim's netId, arg[1]
   the place, arg[2] the player's building, the selected characters as ids), pays 3, accuses the player (gv_Accuse
   0x1a) and closes; with nobody selected the text 0x19e3. The second button and a right click close it. Returns 1
   once the kidnapping was ordered
   @deep s47: n = 0 before the memset of msg.ids (not after): 113 -> 73 ins, kept. Block-scoped msg, done =
   0 moved to the loop: = or worse. Left: frame 0x908 vs 0x90c and the 1s: the original keeps the constant
   1 as a candidate in ebx from before the bld2 test (mov ebx,1; push ebx for BuildQuery's 1, 1,
   d2_SetForm, d2_GetWinObj, msg.flags from bl, sv_PopRequest = 1, done = 1) and bld in a slot (mov
   [esp+0x1c],eax at the join, reloaded for msg.arg[1]); c2trace of ours: no const#1 candidate at all,
   bld#1 (4 refs, prio -31) takes ebx. climb 2 rounds: only meaning-changing mutants (69). Open: the source
   that makes 1 a candidate (a local holding 1? done's stores?). prior: Read whole against the original and
   reconstructed (the draft had sv_CheckMoney's arguments swapped, the place building wrong, the early
   returns not merged): every call, argument, store and branch matches (ours 209, the original 210). Left:
   register allocation. The original keeps the place building in a stack slot (mov [esp+0x1c], eax at
   +0xdf, reloaded at +0x1a1) and the constant 1 in ebx (mov ebx, 1 at +0x9a: push ebx for every 1, done =
   ebx at +0x26c); ours keeps the building in ebx and 1 in edi (or as push 1), which also makes its frame 4
   bytes smaller (0x908 vs 0x90c). Tried: the early returns as one nested condition with the held text as
   the then-arm (that fixed the block order). */
int ai_KidnapWindow(AICHARREC *victim)
{
    int done;
    int picked;
    AIBLD *bld;
    int ok;
    int cancel;
    AIMSG msg;
    char text[0x800];
    int win;
    int base;
    int n;
    int i;

    done = 0;
    if (victim != 0 && sv_CheckMoney((char *)&((AICHARREC *)ai_Chars)[ai_PlayerId], 3) != 0
        && (victim->bld == 0 || victim->bld->owner != victim->id)) {
        if (victim->heldRounds != 0) {
            txt_Format(text, 0x19ba, victim->id);
            sv_MessageBox(text, 0);
        } else {
            bld = (AIBLD *)victim->bld2;
            if (bld == 0) {
                if (ai_PickActionBuilding((struct AIPER *)victim, 0, LC_KIDNAP_PLACES, 0, 100.0f, &picked) == 0)
                    return 0;
                bld = (AIBLD *)gm_BuildQuery(1, 1, picked);
            }
            win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_entfuehrung");
            d2_CentreViewWindows(win);
            d2_SetPointerSprite(0, 0);
            d2_SetForm(win, 1);
            base = io_PrintText(0x19b9, victim->id, 3, victim->id);
            ok = d2_GetWinObj(win, 1, base);
            cancel = d2_GetWinObj(win, 1, base + 1);
            d2_ClickedId = -1;
            do {
                if (dm_L.rup != 0)
                    sv_PopRequest = 1;
                if (d2_ClickedId == -1)
                    continue;
                if (d2_PointerObj == ok) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x3c;
                    msg.flags = 1;
                    msg.to = ai_CharInts[ai_PlayerId][0];
                    msg.arg[0] = victim->netId;
                    msg.arg[1] = bld->id;
                    msg.arg[2] = ((AIBLD *)ai_PlayerBuilding)->id;
                    n = 0;
                    memset(msg.ids, 0xff, sizeof(msg.ids));
                    for (i = 0; i < 0x300; i++)
                        if (((AICHARREC *)ai_Chars)[i].selected != 0)
                            msg.ids[n++] = ((AICHARREC *)ai_Chars)[i].netId;
                    if (n == 0) {
                        sv_MessageBox(txt_Ptrs[0x19e3], 0);
                        d2_ClickedId = -1;
                    } else {
                        cm_SendLongMessage((char *)&msg);
                        cm_SendMoneyDelta(ai_CharInts[ai_PlayerId][0], -3);
                        gv_Accuse(0x1a, 1, ai_CharInts[ai_PlayerId][0], victim->netId, -1);
                        snd_PlayFavourComment();
                        sv_ClearSelection();
                        done = 1;
                        sv_PopRequest = 3;
                    }
                } else if (d2_PointerObj == cancel)
                    sv_PopRequest = 1;
            } while (sv_RunFrame(ai_KidnapWindow, 0x677c7) != 0);
            d2_CloseView(win);
            return done;
        }
    }
    return 0;
}
void cm_SendMessage5b(int a, int b, int c);

typedef struct LCKIDNAPEVENT {          /* the kidnap event 0x3c's words over AIJOB's union (he.c HEEV3C, same layout) */
    char pad00[0xac];
    int victim;                         /* 0xac, the netId of the victim */
    int target;                         /* 0xb0, the id of the building the gang goes to for the victim */
    int guild;                          /* 0xb4, the thieves' guild's id */
} LCKIDNAPEVENT;
typedef struct LCRANSOMEVENT {          /* the ransom event 0x3d's words over AIJOB's union (he.c HEEV3D, same layout) */
    char pad00[0xac];
    int victim;                         /* 0xac, the netId of the kidnapped character */
    int guild;                          /* 0xb0, the id of the thieves' guild that holds it */
    AITIME since;                       /* 0xb4, the time of the kidnap */
} LCRANSOMEVENT;

/* 0x5a53e0, starts the kidnapping by the building bld (0x90 frame): 0x520fe0 (bld); the first kidnap event 0x3c
   (0x53e400 (1, 0, 0x3c) / 0x53e300) of the guild bld->id shows the message ai_TextKidnapMessage and returns;
   else the ransom events 0x3d: one of bld (its guild) with a live character (0x505df0, its victim) whose
   AICHAR.heldRounds is set shows the message and returns, otherwise the event's cm slot is set (0x4c4c60
   (-1, rec)); with no event the request (0, 0x400, the two words of gypsyFilter, 6, the eight -1 at +0x1c) and
   the text 0x19b8 go to 0x525220 (bld, 0) / 0x5c1130 (the request, the text, 0, the callback 0x5a5120), then
   0x525440 / 0x52d7a0 */

void ai_KidnapStart(AIBLD *bld)
{
    AMOUNTCFG cfg;
    char buf[0x40];
    AIJOB *rec;
    AICHARREC *ch;

    if (sv_CheckCharacterFlag(bld) != 0)
        return;
    for (rec = (AIJOB *)he_FindEvent(1, 0, 0x3c); rec != 0; rec = (AIJOB *)he_NextEvent()) {
        if (((LCKIDNAPEVENT *)rec)->guild == bld->id) {
            sv_MessageBox(ai_TextKidnapMessage, 0);
            return;
        }
    }
    for (rec = (AIJOB *)he_FindEvent(1, 0, 0x3d); rec != 0; rec = (AIJOB *)he_NextEvent()) {
        if (((LCRANSOMEVENT *)rec)->guild == bld->id) {
            ch = (AICHARREC *)gm_FindChar(((LCRANSOMEVENT *)rec)->victim);
            if (ch != 0 && ch->heldRounds > 0) {
                sv_MessageBox(ai_TextKidnapMessage, 0);
                return;
            }
            rec->exSlot = cm_SendExSetHEI(-1, (int)rec);
        }
    }
    memset(&cfg, 0, sizeof(cfg));
    memset(cfg.ids, 0xff, sizeof(cfg.ids));
    cfg.flags = 0;
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    txt_Format(buf, 0x19b8);
    sv_OpenPlayerBar((int)bld, 0);
    ai_AmountMenu(&cfg, buf, 0, ai_KidnapWindow);
    sv_ClosePlayerBar();
    sv_CameraFollow();
}

/* 0x5a5530, the ransom window of the thieves' guild's kidnapping room bld "locations\diebesgilde\diebesgilde_loesegeld"
   (nothing when sv_CheckCharacterFlag refuses the building): the victim bld->victimId (or the text 0x19c8 when it is
   gone); a ransom demand event 0x3e on it already running gives the text 0x19c9, no ransom event 0x3d on it the text
   0x19ca. The ransom is the victim's worth (gm_GetWorth), at most a random 1200000 .. 2000000, times 10 / 6 / 4 / 2 %
   by the rounds the victim was held (4 and more / 3 / 2 / 1). The text 0x19c4 and three buttons: the first sends
   the ransom demand message 0x3e (to and arg[0] the victim, from bld, flags 9, arg[1] the player's netId, arg[2]
   the ransom) and shows the text 0x19c5, the second releases the victim (the field bld->victimId set -1, the
   message 0x5b with the held rounds taken back) with the text 0x19c6, the third shows the text 0x19c7; each and a
   right click close it */
void ai_RansomWindow(AIBLD *bld)
{
    float factor;
    int worth;
    double ransomCap;
    int ok;
    int release;
    int keep;
    int none;
    AIMSG msg;
    char demandText[0x200];
    char releaseText[0x200];
    char text[0x400];
    AICHARREC *victim;
    LCRANSOMEVENT *ev;
    AIJOB *demand;
    int ransom;
    int win;
    int base;

    factor = 0;
    if (sv_CheckCharacterFlag(bld) != 0)
        return;
    victim = (AICHARREC *)gm_FindChar(bld->victimId);
    if (victim == 0) {
        sv_MessageBox(txt_Ptrs[0x19c8], 0);
        return;
    }
    for (demand = (AIJOB *)he_FindEvent(1, 0, 0x3e); demand != 0; demand = (AIJOB *)he_NextEvent())
        if (((LCRANSOMEVENT *)demand)->victim == victim->netId) {
            txt_Format(text, 0x19c9, victim->id);
            sv_MessageBox(text, 0);
            return;
        }
    for (ev = (LCRANSOMEVENT *)he_FindEvent(1, 0, 0x3d); ev != 0; ev = (LCRANSOMEVENT *)he_NextEvent())
        if (ev->victim == victim->netId)
            break;
    if (ev == 0) {
        sv_MessageBox(txt_Ptrs[0x19ca], 0);
        return;
    }
    if (victim->heldRounds >= 4)
        factor = 0.1f;
    else if (victim->heldRounds == 3)
        factor = 0.06f;
    else if (victim->heldRounds == 2)
        factor = 0.04f;
    else if (victim->heldRounds == 1)
        factor = 0.02f;
    worth = gm_GetWorth(victim->id);
    worth = (int)(float)worth;
    ransomCap = worth;
    ransom = (int)((float)(int)((util_RandomF() * 0.5 + 0.75) * 1600000.0 < ransomCap
                                    ? (util_RandomF() * 0.5 + 0.75) * 1600000.0 : ransomCap) * factor);
    win = d2_OpenForm(0, 0, "locations\\diebesgilde\\diebesgilde_loesegeld");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 1);
    base = io_PrintText(0x19c4, victim->id, &ev->since, 2, ransom, victim->id, victim->id);
    ok = d2_GetWinObj(win, 1, base);
    release = d2_GetWinObj(win, 1, base + 1);
    keep = d2_GetWinObj(win, 1, base + 2);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            msg.arg[0] = msg.to = victim->netId;
            msg.fromId = bld->id;
            msg.type = 0x3e;
            msg.flags = 9;
            msg.arg[2] = ransom;
            msg.arg[1] = ai_CharInts[ai_PlayerId][0];
            cm_SendLongMessage((char *)&msg);
            txt_Format(demandText, 0x19c5, ransom, victim->id);
            sv_MessageBox(demandText, 0);
            sv_PopRequest = 1;
        } else if (d2_PointerObj == release) {
            none = -1;
            cm_BeginFieldBlock((char *)bld, bld->id);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&bld->victimId - cm_FieldObject), &none);
            cm_SendFieldDeltas();
            cm_SendMessage5b(victim->netId, -victim->heldRounds, 0);
            txt_Format(releaseText, 0x19c6, victim->id);
            sv_MessageBox(releaseText, 0);
            sv_PopRequest = 1;
        } else if (d2_PointerObj == keep) {
            txt_Format(text, 0x19c7, victim->id);
            sv_MessageBox(text, 0);
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame(ai_RansomWindow, 0x677c7) != 0);
    sv_CameraFollow();
    d2_CloseView(win);
}

float gm_GetSkill(char *c, int skill); int cm_SendChrGotoBuilding(); 

/* 0x5a5910, the player's character's attempt to get free from the kidnapping room bld (ai_KidnapContact): asked
   with the text 0x3fea, it costs twice the rounds the character was held (sv_CheckMoney, paid with
   cm_SendMoneyDelta); it succeeds below the chance (the skills 4 + 3 + 2) * 0.2 + 0.15: bld's victim field is
   set -1, the message 0x5b takes the held rounds back, the figure walks out ("Lösegeld", he_FindFigurePlace), the
   ransom event 0x3d of bld on the character ends (cm_SendExSetHE), and the news 0x3feb / 0x3fed go to the
   character and the room's owner; a failure gives the news 0x3fec / 0x3fee. The camera follows again after */
void ai_HostageEscape(AIBLD *bld)
{
    float chance;
    int placeBld;
    int placeObj;
    char failText[0x400];
    char text[0x400];
    AICHARREC *ch;
    AICHARREC *owner;
    LCRANSOMEVENT *ev;

    ch = &((AICHARREC *)ai_Chars)[ai_PlayerId];
    owner = &((AICHARREC *)ai_Chars)[bld->owner];
    d2_SetPointerSprite(0, 0);
    txt_Format(failText, 0x3fea, ch->heldRounds * 2);
    if (sv_MessageBox(failText, 0x11) != 0 && sv_CheckMoney((char *)ch, ch->heldRounds * 2) != 0) {
        chance = gm_GetSkill((char *)ch, 4);
        chance = gm_GetSkill((char *)ch, 3) + chance;
        chance = (gm_GetSkill((char *)ch, 2) + chance) * 0.2f + 0.15f;
        cm_SendMoneyDelta(ch->netId, -ch->heldRounds * 2);
        if (util_RandomF() < chance) {
            int none = -1;  /* the field's new value: no victim */

            cm_BeginFieldBlock((char *)bld, bld->id);
            cm_AddFieldDelta(4, 1, (unsigned short)((char *)&bld->victimId - cm_FieldObject), &none);
            cm_SendFieldDeltas();
            cm_SendMessage5b(ch->netId, -ch->heldRounds, 0);
            if (ch->model != 0 && he_FindFigurePlace(ch, &placeBld, &placeObj, 0) != 0)
                cm_SendChrGotoBuilding(ch->netId, placeBld, placeObj, 0, 0, "L\366segeld");
            for (ev = (LCRANSOMEVENT *)he_FindEvent(1, 0, 0x3d); ev != 0; ev = (LCRANSOMEVENT *)he_NextEvent())
                if (ev->guild == bld->id && ch->netId == ev->victim) {
                    ((AIJOB *)ev)->exSlot = cm_SendExSetHEI(-1, ev);
                    break;
                }
            txt_Format(text, 0x3feb);
            he_ShowNews(ch->netId, -1, text, 0, 0x5f3, 0);
            txt_Format(text, 0x3fed, ch->id);
            he_ShowNews(owner->netId, -1, text, 0, 0x5f3, 0);
        } else {
            txt_Format(failText, 0x3fec);
            he_ShowNews(ch->netId, -1, failText, 0, 0x5f3, 0);
            txt_Format(failText, 0x3fee, ch->id);
            he_ShowNews(owner->netId, -1, failText, 0, 0x5f3, 0);
        }
    }
    sv_CameraFollow();
}

int gm_CreateCharModel(); void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s); int ch_Delete();

/* 0x5a5bc0, the kidnapping contact listener (694 B, room 0x65 of the building driver 0x59e420): the victim
   (building +0x65, a character): without a model record 0x4fbb80 (the victim, 0x633444, 0, 0, 0,
   "dummy_EINGANG_ZELLE") when +0x184 is clear and the kind is not 6 / 7, or 0x4c51b0 (the victim, the owner,
   room +2, the name) when the model belongs to another room; 0x52e190, the pump 0x5309a0 (this, 0x67fff)
   fetches contact_ENTFUEHREN / contact_LOESEGELD under the mask 0x200 and contact_AUSBRECHEN for an own victim
   with the held-rounds byte +0x1b1 set; the pending click 0x6f22e4 opens ai_KidnapStart / ai_RansomWindow /
   ai_HostageEscape; the model 0x4028a0 (the record) and +0x184 = 0 on the exit */
void ai_KidnapContact(int building, int room)
{
    AIBLD *bld = (AIBLD *)building;
    AIROOM *rm = (AIROOM *)room;
    int entfuehren = 0;
    int loesegeld = 0;
    int ausbrechen = 0;
    AICHARREC *ch = 0;
    int cell = 0;
    int r;

    if (bld->victimId != -1) {
        ch = (AICHARREC *)gm_FindChar(bld->victimId);
        if (ch != 0) {
            if (ch->model == 0 && ch->kind != 6 && ch->kind != 7)
                cell = gm_CreateCharModel(ch, d3_CurrentScene, 0, 0, 0, "dummy_EINGANG_ZELLE");
            if (ch->model != 0 && ch->model->scriptGroup != rm->objId)
                cm_RequestChrMoveToUniverse(ch->netId, bld->id, rm->objId, "dummy_EINGANG_ZELLE");
        }
    }
    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_KidnapContact, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            entfuehren = sv_RegisterNamePlate("contact_ENTFUEHREN", ai_ContactTextKidnap, 0x16);
            loesegeld = sv_RegisterNamePlate("contact_LOESEGELD", ai_ContactTextRansom, 0x16);
        }
        if (ch != 0 && ai_CharInts[ai_PlayerId][0] == ch->netId && ch->heldRounds > 0)
            ausbrechen = sv_RegisterNamePlate("contact_AUSBRECHEN", ai_ContactTextBreakOut, 0x16);
        if (ai_ClickedObject != 0) {
            if (ai_ClickedObject == entfuehren)
                ai_KidnapStart(building);
            else if (ai_ClickedObject == loesegeld)
                ai_RansomWindow(building);
            else if (ai_ClickedObject == ausbrechen)
                ai_HostageEscape(building);
        }
        r = sv_RunFrame(ai_KidnapContact, 0x67fff);
    }
    if (cell != 0 && ch != 0) {
        ch_Delete(cell);
        ch->model = 0;
    }
}
/* 0x5a5d70, the city watch window "LOCATIONS\STADTWACHE\STANDARD" (the map pick's callback in 0x5a5f60: its context,
   not read here, and the picked building target; 0 for none): with no character selected the text 0x1a5e and 0.
   The text 0x1a69 and two buttons: the first sends the group job message 0x43 (to the player's netId, flags 2,
   arg[0] the target, arg[1] the player's building, the netIds of up to four selected characters as the group's
   members at 0x70, -1 for the rest; nothing with none), clears the selection and closes; the second and a right
   click close it. The camera follows again after; returns 1 */
int ai_CityWatchWindow(int ctx, AIBLD *target)
{
    int ok;
    AIMSG msg;
    int win;
    int cancel;
    int base;
    int n;
    int i;

    if (target == 0)
        return 0;
    for (i = 0; i < 0x300; i++)
        if (((AICHARREC *)ai_Chars)[i].selected != 0)
            break;
    if (i >= 0x300) {
        sv_MessageBox(txt_Ptrs[0x1a5e], 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\\STADTWACHE\\STANDARD");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    base = io_PrintText(0x1a69);
    ok = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            /* arg[6..9], 0x70: the group's members (the group job event's AIJOB.members, 0x54 further on) */
            n = 0;
            for (i = 0; i < 4; i++)
                msg.arg[6 + i] = -1;
            for (i = 0; i < 0x300 && n < 4; i++)
                if (((AICHARREC *)ai_Chars)[i].selected != 0)
                    msg.arg[6 + n++] = ((AICHARREC *)ai_Chars)[i].netId;
            if (n != 0) {
                msg.type = 0x43;
                msg.flags = 2;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.arg[0] = target->id;
                msg.arg[1] = ((AIBLD *)ai_PlayerBuilding)->id;
                for (i = n; i < 4; i++)
                    msg.arg[6 + i] = -1;
                cm_SendLongMessage((char *)&msg);
                snd_PlayFavourComment();
            }
            sv_ClearSelection();
            sv_PopRequest = 2;
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_CityWatchWindow, 0x677c7) != 0);
    d2_CloseView(win);
    sv_CameraFollow();
    return 1;
}
/* 0x5a5f60, the city watch patrol start, the click of the contact_PATROL plate (112 B): 0x520fe0 (the
   building); the 20-int map pick record zeroed with its +4 = 0x400, +8 = 0 (the check's result) and +0x10 = 2;
   the player bar opens (0x525220, the building), the map pick runs (0x5b2180: 1, the record, the prompt text
   ai_PatrolPickPrompt, the city watch window ai_CityWatchWindow as callback, 0), then the player bar closes
   (0x525440) and the selection is cleared (0x52ae40) */

void ai_PatrolStart(int building)
{
    int req[20];
    int i;
    int r;

    r = sv_CheckCharacterFlag(building);
    if (r != 0)
        return;
    for (i = 0; i < 0x14; i++)
        req[i] = 0;
    req[1] = 0x400;
    req[4] = 2;
    req[2] = r;
    if (building == 0)
        return;
    sv_OpenPlayerBar(building, 0);
    ai_MapPick(1, req, ai_PatrolPickPrompt, (int)ai_CityWatchWindow, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
/* 0x5a5fd0, the second city watch window "LOCATIONS\STADTWACHE\STANDARD" (the map pick's callback in 0x5a61f0: its
   context, not read here, and the picked building target; 0 for none): with no character selected the text 0x1a5e
   and 0. The text 0x1a6d (target) and two buttons: the first sends the message 0x44 (to the player's netId, flags
   2, arg[0] the target, arg[1] the player's building, up to six selected characters as ids; nothing with none) and
   with more than six selected tells so (the text 0x19ef, 6), then closes; the second and a right click close it.
   Returns 1 once the message was sent */
int ai_CityWatchCrewWindow(int ctx, AIBLD *target)
{
    int done;
    int cancel;
    int ok;
    AIMSG msg;
    char text[0x400];
    int win;
    int base;
    int n;
    int selected;
    int i;

    done = 0;
    if (target == 0)
        return 0;
    for (i = 0; i < 0x300; i++)
        if (((AICHARREC *)ai_Chars)[i].selected != 0)
            break;
    if (i >= 0x300) {
        sv_MessageBox(txt_Ptrs[0x1a5e], 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\\STADTWACHE\\STANDARD");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    base = io_PrintText(0x1a6d, target);
    ok = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            n = 0;
            memset(msg.ids, 0xff, sizeof(msg.ids));
            for (i = 0; i < 0x300 && n < 6; i++)
                if (((AICHARREC *)ai_Chars)[i].selected != 0)
                    msg.ids[n++] = ((AICHARREC *)ai_Chars)[i].netId;
            if (n != 0) {
                selected = 0;
                msg.type = 0x44;
                msg.flags = 2;
                msg.to = ai_CharInts[ai_PlayerId][0];
                msg.arg[0] = target->id;
                msg.arg[1] = ((AIBLD *)ai_PlayerBuilding)->id;
                cm_SendLongMessage((char *)&msg);
                snd_PlayFavourComment();
                done = 1;
                for (i = 0; i < 0x300; i++)
                    if (((AICHARREC *)ai_Chars)[i].selected != 0)
                        selected++;
                if (selected > 6) {
                    txt_Format(text, 0x19ef, 6);
                    sv_MessageBox(text, 0);
                }
            }
            sv_PopRequest = 2;
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_CityWatchCrewWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return done;
}
/* 0x5a61f0, the city watch raid start, the click of the contact_RAZZIA plate (124 B): 0x520fe0 (the building);
   the 20-int map pick record zeroed with its +4 = 0x400, +8 = 0x90010, +0x10 = 7 and +0x4c = 0x727; the player
   bar opens (0x525220, the building), the map pick runs (0x5b2180: 1, the record, the prompt text
   ai_RaidPickPrompt, the city watch crew window ai_CityWatchCrewWindow as callback, 0), then the player bar
   closes (0x525440) and the selection is cleared (0x52ae40) */

void ai_RaidStart(int building)
{
    int req[20];
    int i;

    if (sv_CheckCharacterFlag(building) != 0)
        return;
    for (i = 0; i < 0x14; i++)
        req[i] = 0;
    req[1] = 0x400;
    req[4] = 7;
    req[2] = 0x90010;
    if (building == 0)
        return;
    req[19] = 0x727;
    sv_OpenPlayerBar(building, 0);
    ai_MapPick(1, req, ai_RaidPickPrompt, (int)ai_CityWatchCrewWindow, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
/* 0x5a6270, the city watch plan window "LOCATIONS\STADTWACHE\STANDARD" (ai_WatchPlanStart's map pick callback, mode
   4: its context, not read here, and the picked place's name; 0 for none): with no character selected the text
   0x1a5e. The text 0x1a70 and two buttons: the first sends the watch message 0x65 (to the player's netId, from the
   player's building, now, flags 2, the place's name at arg[0], arg[8] the player's building, arg[9] the object of
   the room the panel shows, up to six selected scene characters as ids; nothing with none), then closes; the
   second and a right click close it. Returns 0 */
int ai_WatchPlanWindow(int ctx, char *place)
{
    int cancel;
    int ok;
    AIMSG msg;
    int win;
    int base;
    int n;
    int i;
    int *pp;
    AICHARREC *ch;
    AIBLD *bld;

    if (place == 0)
        return 0;
    for (i = 0; i < 0x300; i++)
        if (((AICHARREC *)ai_Chars)[i].selected != 0)
            break;
    if (i >= 0x300) {
        sv_MessageBox(txt_Ptrs[0x1a5e], 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\\STADTWACHE\\STANDARD");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    base = io_PrintText(0x1a70);
    ok = d2_GetWinObj(win, 1, base);
    cancel = d2_GetWinObj(win, 1, base + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == -1)
            continue;
        if (d2_PointerObj == ok) {
            bld = (AIBLD *)ai_PlayerBuilding;
            msg.type = 0x65;
            msg.to = ai_CharInts[ai_PlayerId][0];
            msg.fromId = bld->id;
            msg.time = ai_Time;
            msg.flags = 2;
            strcpy((char *)msg.arg, place);
            memset(msg.ids, 0xff, 6 * sizeof(int));
            n = 0;
            msg.arg[8] = bld->id;
            msg.arg[9] = ((AIROOM *)sv_PanelShown)->objId;
            for (pp = ai_SceneChars; (int)pp < (int)&ai_SceneChars[32] && n < 6; pp++) {  /* signed compares (jl) */
                ch = (AICHARREC *)*pp;
                if (ch != 0 && ch->selected != 0)
                    msg.ids[n++] = ch->netId;
            }
            if (n != 0) {
                cm_SendLongMessage((char *)&msg);
                snd_PlayFavourComment();
            }
            sv_PopRequest = 2;
        } else if (d2_PointerObj == cancel)
            sv_PopRequest = 1;
    } while (sv_RunFrame(ai_WatchPlanWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return 0;
}
/* 0x5a6480, the city watch plan start, the click of the ob_WACHPLAN plate (67 B): 0x520fe0 (the building);
   the player bar opens (0x525220, the building), the map pick runs (0x5b2180: 4, 0, the prompt text
   ai_WatchPlanPickPrompt, the city watch search window ai_WatchPlanWindow as callback, 0), then the player bar
   closes (0x525440) and the selection is cleared (0x52ae40) */

void ai_WatchPlanStart(int building)
{
    if (sv_CheckCharacterFlag(building) != 0 || building == 0)
        return;
    sv_OpenPlayerBar(building, 0);
    ai_MapPick(4, 0, ai_WatchPlanPickPrompt, (int)ai_WatchPlanWindow, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
/* 0x5a64d0, the city watch arrest window, the map pick callback of ai_ArrestStart (ai_MapPick): for the picked
   building bld (0: nothing) it needs a selected character (else the text 0x1a5e) and a building without the flag
   0x800 (else the text 0x3fdc); the window "LOCATIONS\STADTWACHE\STANDARD" (text 0x1a73 with the building) has the
   arrest and the cancel control. Arrest sends the message 0x64 (from the player's building, about bld, the player's
   building and the room shown, for the first selected character of the scene) and plays the favour comment,
   says the text 0x1a74 when more than one scene character is selected, and closes; cancel or the right button
   close too. Returns 0. */
int ai_ArrestWindow(int unused, AIBLD *bld)
{
    int win;
    int arrestBtn;
    int cancelBtn;
    int ctl;
    int n;
    int i;
    int *p;
    AIMSG msg;

    if (bld == 0)
        return 0;
    for (i = 0; i < 0x300; i++) {
        if (((AICHARREC *)ai_Chars)[i].selected != 0)
            break;
    }
    if (i >= 0x300) {
        sv_MessageBox(txt_Ptrs[0x1a5e], 0);
        return 0;
    }
    if (bld->flags5aw & 0x800) {
        sv_MessageBox(txt_Ptrs[0x3fdc], 0);
        return 0;
    }
    win = d2_OpenForm(0, 0, "LOCATIONS\\STADTWACHE\\STANDARD");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 1);
    ctl = io_PrintText(0x1a73, bld);
    arrestBtn = d2_GetWinObj(win, 1, ctl);
    cancelBtn = d2_GetWinObj(win, 1, ctl + 1);
    d2_ClickedId = -1;
    do {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == arrestBtn) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x64;
                msg.to = ((AICHARREC *)ai_Chars)[ai_PlayerId].netId;
                msg.fromId = ((AIBLD *)ai_PlayerBuilding)->id;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.arg[1] = bld->id;
                msg.arg[2] = ((AIBLD *)ai_PlayerBuilding)->id;
                msg.arg[3] = ((AIROOM *)sv_PanelShown)->objId;
                for (i = 0; i < 0x20; i++) {
                    if (ai_SceneChars[i] != 0 && ((AICHARREC *)ai_SceneChars[i])->selected != 0) {
                        msg.arg[0] = ((AICHARREC *)ai_SceneChars[i])->netId;
                        cm_SendLongMessage(&msg);
                        snd_PlayFavourComment();
                        break;
                    }
                }
                n = 0;
                for (p = ai_SceneChars; (int)p < (int)&ai_SceneChars[32]; p++) {  /* a signed compare (jl) */
                    if (*p != 0 && ((AICHARREC *)*p)->selected != 0)
                        n++;
                }
                if (n > 1)
                    sv_MessageBox(txt_Ptrs[0x1a74], 4);
                sv_PopRequest = 2;
            } else if (d2_PointerObj == cancelBtn) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame(ai_ArrestWindow, 0x677c7) != 0);
    d2_CloseView(win);
    return 0;
}

/* 0x5a6720, the city watch arrest map pick, the click of the customs box ob_ZOLLKASSE (the zoll branch of
   ai_CityWatchListener's contact dispatch, 127 B): the building's character gate (sv_CheckCharacterFlag, 0 with a free
   character) and the building; the 20-int request 1, -1, 0xa0744300, 1, 6 with 0x727 at +0x4c; the player bar
   opens, ai_MapPick runs (1, the request, the prompt text ai_ArrestPickPrompt, the city watch arrest window
   ai_ArrestWindow as callback, 0), then the player bar closes and the selection is cleared */

void ai_ArrestStart(int building)
{
    int req[20];

    if (sv_CheckCharacterFlag(building) != 0 || building == 0)
        return;
    req[0] = 1;
    req[1] = -1;
    req[4] = 6;
    req[2] = 0xa0744300;
    req[3] = 1;
    req[19] = 0x727;
    sv_OpenPlayerBar(building, 0);
    ai_MapPick(1, req, ai_ArrestPickPrompt, (void *)(unsigned)&ai_ArrestWindow, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
/* 0x5a67a0, the city watch contact listener, the room 0x146 of the building driver 0x59e420 (376 B): releases
   the name plates, then every frame registers the plates contact_PATROL, ob_WACHPLAN, ob_PERSONALBUCH,
   ob_MEISTERBRIEF and the customs box ob_ZOLLKASSE / ob_ZOLLKASSE_MIT_GEHEIMFACH (sv_RegisterNamePlate, 0x16 /
   0xc); the customs box only with the record 0x147 / 0x148 of the building's objList (gm_Query); a click on a
   plate opens its window: ai_PersonnelBook, ai_PatrolStart, ai_WatchPlanStart, ai_ArrestStart,
   ai_MasterLetterWindow */

void ai_CityWatchListener(AIBLD *bld, int room)
{
    int patrol = 0;
    int wachplan = 0;
    int zoll = 0;
    int personal = 0;
    int meister = 0;
    int r;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_CityWatchListener, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            patrol = sv_RegisterNamePlate("contact_PATROL", ai_ContactTextPatrol, 0x16);
            wachplan = sv_RegisterNamePlate("ob_WACHPLAN", ai_ContactTextWatchPlan, 0x16);
            personal = sv_RegisterNamePlate("ob_PERSONALBUCH", ai_ContactTextPersonnelBook, 0xc);
            meister = sv_RegisterNamePlate("ob_MEISTERBRIEF", ai_ContactTextMasterLetter, 0x16);
            if (gm_Query(bld->objList, 2, 6, 0, 0x147) != 0)
                zoll = sv_RegisterNamePlate("ob_ZOLLKASSE", ai_ContactTextCustomsBox, 0x16);
            else if (gm_Query(bld->objList, 2, 6, 0, 0x148) != 0)
                zoll = sv_RegisterNamePlate("ob_ZOLLKASSE_MIT_GEHEIMFACH", ai_ContactTextCustomsBox, 0x16);
        }
        if ((ai_BuildingPerms & 0x200) && ai_ClickedObject != 0) {
            if (ai_ClickedObject == personal)
                ai_PersonnelBook(bld, "ob_PERSONALBUCH");
            else if (ai_ClickedObject == patrol)
                ai_PatrolStart((int)bld);
            else if (ai_ClickedObject == wachplan)
                ai_WatchPlanStart((int)bld);
            else if (ai_ClickedObject == zoll)
                ai_ArrestStart((int)bld);
            else if (ai_ClickedObject == meister)
                ai_MasterLetterWindow((AIBLD *)bld);
        }
        r = sv_RunFrame(ai_CityWatchListener, 0x67fff);
    }
}
/* 0x5a6920, the guild hall contact listener, the room 0x14c of the building driver 0x59e420 (183 B; the guild
   hall is a reasoned reading of the plates: the watch's raid and the thieves' guild's espionage next to the
   store, equip and transport offers): releases the name plates, then every frame registers contact_LAGER
   (class 0xe), contact_AUSRUESTEN (0xd), contact_RAZZIA (0x16), contact_GEBAEUDE_AUSSPIONIEREN (0x17),
   ub_INFORMATIONSPERGAMENT (0xc) and contact_TRANSPORT (0x15); a click opens ai_StorageWindow (the building, the
   shown room's objList), ai_EquipWindow, ai_RaidStart, ai_EspionageStart, ai_SpyReportsWindow or ai_OpenTransport */

void ai_GuildHallListener(AIBLD *bld, int room)
{
    int lager = 0;
    int ausruesten = 0;
    int razzia = 0;
    int transport = 0;
    int ausspionieren = 0;
    int pergament = 0;
    int r;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_GuildHallListener, 0x67fff);
    while (r != 0) {
        if (ai_BuildingPerms & 0x200) {
            lager = sv_RegisterNamePlate("contact_LAGER", ai_ContactTextStorage, 0xe);
            ausruesten = sv_RegisterNamePlate("contact_AUSRUESTEN", ai_ContactTextEquip, 0xd);
            razzia = sv_RegisterNamePlate("contact_RAZZIA", ai_ContactTextRazzia, 0x16);
            ausspionieren = sv_RegisterNamePlate("contact_GEBAEUDE_AUSSPIONIEREN", ai_ContactTextSpy, 0x17);
            pergament = sv_RegisterNamePlate("ub_INFORMATIONSPERGAMENT", ai_ContactTextParchment, 0xc);
            transport = sv_RegisterNamePlate("contact_TRANSPORT", ai_ContactTextTransport, 0x15);
        }
        if (ai_ClickedObject != 0 && (ai_BuildingPerms & 0x200) != 0) {
            if (ai_ClickedObject == lager)
                ai_StorageWindow((int)bld, ((AIOBJ *)sv_PanelShown)->list);
            else if (ai_ClickedObject == ausruesten)
                ai_EquipWindow(bld);
            else if (ai_ClickedObject == razzia)
                ai_RaidStart((int)bld);
            else if (ai_ClickedObject == ausspionieren)
                ai_EspionageStart(bld);
            else if (ai_ClickedObject == pergament)
                ai_SpyReportsWindow(bld);
            else if (ai_ClickedObject == transport)
                ai_OpenTransport((int)bld);
        }
        r = sv_RunFrame(ai_GuildHallListener, 0x67fff);
    }
}
/* 0x5a6a90, the training room's contact listener (172 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); with
   the mask 0x200 the plates ob_ZIELSCHEIBE (ai_ContactTextTarget, 0xc), contact_NACHT (ai_ContactTextTraining,
   0xc) and contact_REGENERATION (ai_ContactTextRegeneration, 0x17), and when the mask is still set and the
   pending click (ai_ClickedObject) is one of them, ai_TrainingWindow (the building) */

void ai_TrainingRoomListener(AIBLD *bld, int room)
{
    int ziel = 0;
    int nacht = 0;
    int regeneration = 0;

    sv_ReleaseNamePlates();
    if (sv_RunFrame(ai_TrainingRoomListener, 0x67fff) == 0)
        return;
    do {
        if (ai_BuildingPerms & 0x200) {
            ziel = sv_RegisterNamePlate("ob_ZIELSCHEIBE", ai_ContactTextTarget, 0xc);
            nacht = sv_RegisterNamePlate("contact_NACHT", ai_ContactTextTraining, 0xc);
            regeneration = sv_RegisterNamePlate("contact_REGENERATION", ai_ContactTextRegeneration, 0x17);
        }
        if ((ai_BuildingPerms & 0x200) && ai_ClickedObject != 0 &&
            (ai_ClickedObject == ziel || ai_ClickedObject == nacht || ai_ClickedObject == regeneration))
            ai_TrainingWindow(bld);
    } while (sv_RunFrame(ai_TrainingRoomListener, 0x67fff) != 0);
}

/* dcp unit split: the declarations that moved to lcmap (copies: drop what lc does not use) */
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py); void d2_RaiseObject(); void d2_ScrollWindow();
int d2_OpenForm(); void d2_RaiseWindow(); int io_PrintText(char *fmt, ...);
int d2_SetForm(int view, int n); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
void d2_CloseView(int view); void d2_RemoveObj(); void d2_ScrollWindowBy();
int d2_CreatePicObj();
#pragma pack(push, 1)
typedef struct G8BLD {                  /* a local view of gv.h's ORDBLD (gv.h cannot be included here yet): the
                                           fields the production settings read */
    unsigned char type;                 /* +0x00, the building kind */
    int id;                             /* +0x01, the building's id (the production settings chain's key) */
} G8BLD;
#pragma pack(pop)
int ls_Read(void *buf, int size, int count, int f); void * mem_Alloc(int size, char *tag);
unsigned vfs_fwrite();
int d2_WinOpenPic(short x, short y, int pic, int win); int io_CharacterName(int id, unsigned char c, unsigned char kind, char *out); 
int gm_Opinion(int a, int b);
#pragma pack(push, 1)
typedef struct G8OBJ {                  /* an object record of the ai_ lists (AIOBJ, ai_types.h) */
    unsigned short id;                  /* +0x00, the object's type id */
    int objId;                          /* +0x02, the object's own id */
} G8OBJ;
#pragma pack(pop)

/* dcp unit split: the functions of lcdom the rest of lc uses */
int ai_BribeAmountCB(AICHARREC *ch);
/* dcp unit split: the declarations that moved to lcdom (copies: drop what lc does not use) */
#pragma pack(push, 1)
typedef struct G9DOMIZILROW {           /* a row of the Domizil personnel book: 0x5aa0d0 creates its controls,
                                           0x5aa1f0 / 0x5aa520 fill them, ai_DomizilFreeRow frees them */
    int x;                              /* 0x00, the row's position (0x5aa0d0's arguments) */
    int y;                              /* 0x04 */
    AICHARREC *ch;                      /* 0x08, the character record it shows */
    int pic;                            /* 0x0c, the picture control (d2_WinOpenPic, 0x5aa0d0) */
    int slider;                         /* 0x10, the value slider (d2_WinOpenSlider, 0x5aa1f0) */
    int text;                           /* 0x14, the text control (0x5aa1f0 / 0x5aa520) */
    int name;                           /* 0x18, the name control (0x5aa0d0) */
    int icon;                           /* 0x1c, the character's icon control (d2_WinOpenPic, 0x5aa1f0) */
    unsigned char state;                /* 0x20, 0: vacant, 1: a character, 2: a hire running (ai_FillVacantPersonnelRow) */
    char pad21[3];                      /* 0x21 .. 0x23 */
    int title;                          /* 0x24, the title rank of the character the row was filled with (ai_PersonnelBook
                                           refills a row whose character or title changed); cleared by 0x5aa0d0 */
    int ctrl28[6];                      /* 0x28 .. 0x3f, the six entries' controls (their roles are not recovered:
                                           the book window creates them, its body is not written yet) */
    int ctrl40[6];                      /* 0x40 .. 0x57, the second control of each entry */
    int view;                           /* 0x58, the view (d2_CurrentView, 0x5aa0d0) */
} G9DOMIZILROW;
#pragma pack(pop)
void d2_RemoveObj(); int gm_HeadIconNumber(); int gm_ObjectBuilding(int y);
void d2_SetObjFont(); void d2_SetCaption();
void d2_RadioSelect(int g, int sel);
void d2_CenterObj(); void gm_FreeObject(int o); int gm_LevelPayout(int v, int kind);
#pragma pack(push, 1)
typedef struct G8SCENECHAR {            /* the scene character fields this unit reads (ch.h CHARACTER) */
    char pad00[0x34];
    D3OBJECT *body;                     /* 0x34, the body (CHARACTER.obj) */
    char pad38[0xf0];
    void *actions;                      /* 0x128, the action queue (CHARACTER.actions) */
} G8SCENECHAR;
#pragma pack(pop)
typedef struct LCCARTROW {              /* 0xf0 bytes, a cart row of the transport window (ai_TransportWindow's 16) */
    AIOBJ *cart;                        /* 0x00, the cart, 0: a free row */
    AIJOB *job;                         /* 0x04, its running transport order (event 0xf, AIJOB.u.id the cart's objId) */
    int selected;                       /* 0x08, picked for the buttons (a click on the cart picture toggles it) */
    int pic;                            /* 0x0c, the cart picture (0x4ae + the cart's kind) */
    int win;                            /* 0x10, the child window its goods sit in */
    int slider;                         /* 0x14, the fill bar (AIOBJ.fill) */
    int escortPic;                      /* 0x18, the escort picture 0x5e8 + AIOBJ.escort, its caption the escort's fee */
    int label;                          /* 0x1c, the route label (patrol on / off text); a click edits the route */
    AIRECIPE goods;                     /* 0x20, not read by the window */
    int goodsCtl[3];                    /* 0xd8, per goods slot its input icon (the goods' icon + 0xdf), -1: none */
    int goodsPic[3];                    /* 0xe4, per goods slot its stock slider, -1: none */
} LCCARTROW;
#pragma pack(push, 1)
typedef struct AIWINDOWROW {            /* the row record the window page functions take (ai_BlackmailContactWindow builds it) */
    int win;                            /* 0x00, the window, -1 none */
    int pageA;                          /* 0x04, the page of the pictures */
    int pageB;                          /* 0x08, the page of the text */
    int pageC;                          /* 0x0c, the page of the balance bar */
    int entry;                          /* 0x10, the page entry index */
    int record;                         /* 0x14, the gv table record id */
} AIWINDOWROW;
typedef struct AIPOSVIEW {              /* 0x24 bytes, the record gv_GetTableRecord (0x4f2720) fills (AIPOS, ai.h) */
    char pad00[0xc];
    int pics;                           /* 0x0c, the page's picture count */
    char pad10[8];
    int texts;                          /* 0x18, the text count (AIPOS.unk18) */
    char pad1c[8];
} AIPOSVIEW;
#pragma pack(pop)
void ai_ShowWindowRow(AIWINDOWROW *row);
int gv_HeldCrimesOf(int culprit, int *out, unsigned short t);  /* 0x4f3fe0, gv.h's prototype (gv.h is not included yet: Open: head) */
void gv_SortCrimesByLaw(int n, CRIME *r);  /* 0x4f4390, gv.h's prototype (gv.h is not included yet: Open: head) */
int gv_RankCulpritsAll(int *ids, int *open, int *total, unsigned short t);  /* 0x4f3dd0, gv.h's prototype (gv.h is not included yet: Open: head) */
void ai_RightAlignedText(char *text, int x, int y, int w, int font);
int ai_BribeAmountCB(AICHARREC *ch);
int gv_HeldCrimeCount(int culprit, unsigned short t);  /* 0x4f4130, gv.h's prototype (gv.h is not included yet: Open: head) */
G11REC *hmt_GetRecord(unsigned short k);                    /* 0x4ac1e0, hmt.h's prototypes (hmt.h is not included yet: Open: head) */
unsigned short hmt_RecordTextId(unsigned short k, unsigned short n); /* 0x4ac1a0 */
int hmt_GetPrice(struct AICHAR *p);                         /* 0x4ac200 */
void hmt_ApplyRecord(struct AICHAR *p, int d, unsigned short k); /* 0x4ac2f0 */
int ai_EvalFest(AIGRP *g, int send);   /* 0x494ac0, am.h's prototype (am.h is not included yet: Open: head) */
typedef struct LCTRAINROW {             /* 0xc bytes, a row of the training window (ai_TrainingWindow keeps three) */
    int pic;                            /* 0x00, the training's picture (0x762 + the row), interactive */
    int frame;                          /* 0x04, the row's frame picture 0x74c */
    int window;                         /* 0x08, the row's child window (font 0x54) */
} LCTRAINROW;
int d2_WinOpenInput(int x, int y, int w, int h, unsigned char flags, int win);  /* 0x46a360, d2.h's prototype (d2.h is not included yet: Open: head) */
void * d2_SetFont(int font); void d2_DrawTextBox(); void d2_FillPaintbox(int x, int y, int w, int h); int gv_GetTableRecord();
int gm_EquipmentValue(); void d2_RadioSelect(int g, int sel);
int ai_PassesPartnerFilter(unsigned short me, unsigned short id, AMOUNTCFG *cfg, int unused);

/* dcp unit split: the declarations that moved to lcamt (copies: drop what lc does not use) */
#pragma pack(push, 1)
#pragma pack(pop)
float gm_CharacterHealth(); int amt_HasColumn7Holder(); int d2_WinCreateSlider();
void d2_SetObjWidth(int obj, short w); int amt_GetColumnOffices(int col, int max, struct AMTREC *out);
#pragma pack(push, 1)
typedef struct LCOFFEREVENT {           /* the wedding offer event 0x2c's words over AIJOB's union (he.c HEOFFER, same
                                           layout) */
    char pad00[0xac];
    int who[6];                         /* 0xac, the characters of the wedding: the suitor, the partner, then four
                                           guests (the last -1 for none) */
    char kind;                          /* 0xc4, the offer kind */
    AITIME clock;                       /* 0xc5, when the wedding takes place */
} LCOFFEREVENT;
#pragma pack(pop)
typedef struct AIAMOUNTTAB {            /* 0x14 bytes, one tab of the special-amount list window's tab table:
                                           {id, count, control, fill function of the page, row builder} */
    int id;                             /* +0x00, 0 .. 7 */
    unsigned int count;                 /* +0x04, the rows of the page */
    int control;                        /* +0x08, the tab's button (ai_SpecialAmountWindow), -1: none yet */
    unsigned int (*fill)(AICHARREC *me, unsigned int max, LCAMOUNTROW *rows, unsigned int *filter);
    int (*show)(int view, int page, int scrollPage, unsigned int n, LCAMOUNTROW *rows);
} AIAMOUNTTAB;
extern AIAMOUNTTAB ai_AmountTabs[];     /* 0x62d148, the eight tabs */
int gv_TableRecordText();
int gm_IsUnderAge(unsigned short id); int ai_WorksForMaster();
typedef struct LCPICKROW {         /* 0x20 bytes, one character row of ai_PickBuildingChar */
    int icon;                      /* 0x00, the row's picture object 0x5db, the click target */
    int portrait;                  /* 0x04, the character's portrait (AICHARREC.picture) */
    int name;                      /* 0x08, the name text */
    int bar;                       /* 0x0c, the liking slider (disabled: a bar) */
    int pad10;
    int id;                        /* 0x14, the character id */
    int pad18[2];
} LCPICKROW;
void d2_PlotLine(int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b);  /* 0x4764f0 */
typedef struct LCFAMILYBOX {        /* 0x10 bytes, one box of the family tree window */
    unsigned short id;             /* 0x00, the character id, 0xffff: the box is empty */
    short pad02;
    int ctrl;                      /* 0x04, the box picture object (d2_WinOpenPic), -1: no character in it */
    unsigned short x;              /* 0x08, the centre of the box */
    unsigned short y;              /* 0x0a, the top of the box */
    int pad0c;
} LCFAMILYBOX;
int gm_GetGoodsValue();
void ai_ShowMissions(int owner);
struct AIPER;
int gm_CharLevel(unsigned short id, int again); int ord_DispatchOrder();
unsigned int ai_FindCandidates(struct AIPER *p, unsigned int n, unsigned int fl, float lo, float hi, unsigned short *out);
void d2_EnableViewObjects(); void gm_AddTime(struct AITIME *c, int hours, int mins, int secs); 
int cm_GetSlotState();
typedef struct LCORDMSG {          /* the message record of an order (gv.h ORDMSG, which this unit cannot include): the
                                      order handlers get it with the character (ord_DispatchOrder) */
    int kind;                      /* 0x000, 1 do / 2 answer / 3 ask / 4 answer */
    union {
        int law;                   /* 0x004, of the law orders: the decision record (AIDEC.id) the office may change
                                      (gv.h ORDMSG.law) */
        int amount;                /* 0x004, of the bewitch order asked of the AI: what it pays, 1 .. 5 (ai_BewitchOrder);
                                      of the penance order: the crimes it buys off, 1 .. 9 (ai_PenanceWindow) */
    };
    int value;                     /* 0x008, of the law orders asked of the AI: the value it wants (ai_LawPrivilegeWindow) */
    char pad00c[8];
    char text[0x200];              /* 0x014, the text the handlers fill */
    union {
        int target;                /* 0x214, of an ask that names a character (the courtship): its netId (gv.h
                                      ORDMSG.target) */
        int answerFlag;            /* 0x214, of the law orders: which of the office's decisions, 1 / 2 (gv.h
                                      ORDMSG.answerFlag) */
    };
    int target2;                   /* 0x218, of the hatred order asked of the AI: the second character's netId
                                      (ai_HatredWindow; the first is target) */
    struct AICHARREC *partner;     /* 0x21c, the partner character (gv.h ORDMSG.partner; the privileges panel sets it
                                      to the player's partner of the pair event 0x45) */
} LCORDMSG;
int d2_TextWidth();
float ord_GetStatistics(int *o);
int ord_PrivilegeDialog();
int gv_HeldCrimeIdsOf(int culprit, int *out, unsigned short t);
int gv_SetTableRecord(struct AICHAR *c, int i, int v);   /* 0x4f2750 */
int amt_GetKind(); int amt_FindOffice(); int amt_SendOfficeCandidates(); int ai_RelationGain();
int amt_SendOfficeSwap();
int ai_FindPartnersRandomWalk(struct AIPER *p, unsigned int a, int *flags, float lo, float hi, unsigned short *id); /* 0x4a5b50 (am.h, not included) */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, int d, unsigned char e);  /* 0x4e6680 (amt.h) */
void cm_SendSetJob(int id, int job);  /* 0x4c59b0 (cm.h, not included) */
int ai_PickPartnerRandomWalk(struct AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 (am.h, not included) */
extern struct AICHAR *ai_JudgeCatholic;    /* 0x6f1bfc (gv.h), the catholic judge */
extern struct AICHAR *ai_JudgeProtestant;  /* 0x6f1c00 (gv.h), the protestant judge */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, int d, unsigned char e); int gm_TitleNumber();
extern const int ai_CharWalkSteps[16];  /* 0x5ed100, the random walk steps (am.h, not included) */
extern AIDEC gv_Decisions[];  /* 0x611258, the 0x1d AI decision (law) records (gv.h, not included) */

extern int * ai_BauplatzList;
/* ai_ContactTextPersonnelBook: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextMasterLetter: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextPond: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextFlyGlass: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextTerrarium: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextRawMaterialSearch: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextIndulgence: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextTop5: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextDungeonPlan: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextWineCellar: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextMistress: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextEinkauf: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextStorage: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextTransport: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextEinkaufSkripte: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_ContactTextProductionPerfumery: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextEinkaufMetall: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextEinkaufStein: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_TextEinkaufHolz: view of txt_Ptrs (build/mk_txt_views.py) */
AIBUILDNAME ai_BuildNames[0xf] = { { "bk_BRUNNEN", { 8, 9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_GEFAENGNIS", { 17, 18, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_HANDWERK", { 11, 12, 13, 14, 15, 16, 23, 24, 25, 26, 27, 28 } }, { "bk_HANDWERK", { 41, 42, 43, 47, 48, 49, 50, 51, 52, 33, 34, 35 } }, { "bk_HANDWERK", { 72, 73, 74, 81, 82, 83, 78, 79, 80, 0, 0, 0 } }, { "bk_KIRCHE_KL", { 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_KIRCHE_GR", { 22, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_RATHAUS", { 36, 37, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_STADTWACHE", { 44, 45, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_WIRTSHAUS_GR", { 53, 54, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_WOHNSITZ_KL", { 4, 5, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_WOHNSITZ_GR", { 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_ZUNFTHAUS", { 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67 } }, { "bk_STATUEN", { 84, 85, 86, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { "bk_DOMIZIL", { 75, 76, 77, 0, 0, 0, 0, 0, 0, 0, 0, 0 } } };  /* 0x62aae8 */
int ai_BuildGroup = 6;  /* 0x62af98, the building group the construction window shows (1 .. 9, its group buttons) */
LCOPENHOURS ai_OpeningHours[19] = { { 5, 9, 20 }, { 6, 7, 22 }, { 8, 9, 20 }, { 9, 9, 20 }, { 0xe, 9, 20 }, { 0xf, 6, 22 }, { 0x12, 9, 20 }, { 0x13, 9, 20 }, { 0x14, 9, 20 }, { 0x15, 9, 20 }, { 0x16, 12, 23 }, { 0x17, 7, 22 }, { 0x18, 7, 22 }, { 0x19, 7, 22 }, { 0x1a, 7, 22 }, { 0x20, 9, 20 }, { 0x1f, 9, 20 }, { 0x1d, 9, 20 }, { 0, 0, 0 } };  /* 0x62afa0, the opening hours per building class, ended by class 0 (ai_IsBuildingOpen) */
LCPRODROW ai_ProdRows[4];  /* 0x6f4f00 */
LCPRODSLOT ai_ProdGoods[16];  /* 0x6f4f60 */
LCPRODROW ai_ProdRows2[2];  /* 0x6f52e0 */
int * ai_BauplatzList;  /* 0x6f5308 */
LCPRODCOL ai_ProdColumns[16];  /* 0x6f5320, the goods columns of the production window */
LCPRODSLOT ai_ProdGoods2[8];  /* 0x6f58a0 */
int ai_ProdSelectedInput;  /* 0x6f5a60, the input goods picked in the production goods window, 0 = none */
LCPRODSLOT ai_ProdColumnSlots[16];  /* 0x6f5a80, a slot per production column (ai_InitProductionTables: x 0x1a, y 0x10 + 0x45 * i); ai_ProductionGoodsWindow compares its goods with a job's */
