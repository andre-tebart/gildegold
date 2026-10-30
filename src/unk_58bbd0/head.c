/* unk_58bbd0
   unit of rea 8cef7d5b (src/game/unk_58bbd0.c); boundaries in decl/text.dcl, notes in src/unk_58bbd0/open.md */
#define GAPDATA __declspec(align(1)) unsigned char
/* unk_58bbd0.c -- the production / trade windows and the tavern / location / money lender / Rathaus windows
   (0x58bbd0 .. 0x5bcc70): the
   "Handel\Handel_PRODUKTION" screen of a building (0x58bd60) and its helpers (the table init 0x58bbd0, the
   goods columns 0x58c680 ..), then the building purchase / construction screens (0x58f510 .. 0x593960: the
   "contact_EINKAUF" / "BAUEN_GEB_BAUEN" strings), the trade search windows (0x595480 / 0x595a00), the
   Wirtshaus / locations module (0x593e80 .. 0x599af0: "locations\diebesgilde", "locations\wohnsitz",
   "locations\Wirtshaus", the dice game and the dark corner), the money lender family
   ("locations\geldleihe\*", 0x59ad60 .. 0x59d5e0) and the Rathaus / guild rooms (0x59df10 ..: the enter check
   0x59e210 and the building driver 0x59e420 with its room dispatch switch).
   Unit start (layout 2026-10-23 session 21): 0x58bbd0 has no literals and is called from 0x58bd60 (and from the
   "HANDEL\HANDEL_SUCHEN" helper 0x584d70 of the previous unit); it initializes the tables 0x6f4f08 .. 0x6f5a64
   that 0x58bd60 / 0x58c680 use. The first literal of the unit is "Handel\Handel_PRODUKTION" 0x62b038 (0x58bd60),
   a +0x568 jump past the previous unit's last literal 0x62aad0 (data order D8); no literal belongs to 0x58bbd0
   itself, so the boundary rests on the subject and the call coupling.
   Session 2026-10-24 (layout): drafted 0x58f510 .. 0x599aa0 in address order (see notes/units/unk_58bbd0.md,
   layout 2026-10-24); the unit is the last one (open, no neighbour after it yet).
   Session 2026-10-24 (layout, second): drafted 0x599af0 .. 0x5a1a60 (the tavern / locations contacts, the money
   lender windows, the Rathaus enter check, the building drivers 0x59e420 / 0x59ee40 / 0x59f410 and the guild /
   office windows); the unit is now contiguous 0x58bbd0 .. 0x5a1af0 and is still the last one (open, no
   neighbour after it yet).
   Session 2026-10-24 (layout, third): drafted 0x5a1af0 .. 0x5a4b90 (the church rooms: Spenden 0x5a1af0,
   Ablass2 0x5a1f70, Taufe 0x5a23a0, Top5 0x5a28f0, Beichte 0x5a2b60 with its listener 0x5a2de0, Predigt
   0x5a2e60; the building listeners 0x5a36c0 / 0x5a3810 / 0x5a39b0; the dungeon 0x5a3a40 / 0x5a3b20 / 0x5a3b90 /
   0x5a3df0 / 0x5a3ef0 and the thieves' guild: burglary 0x5a3f10 / 0x5a4130, espionage 0x5a41d0 / 0x5a44e0,
   spy ring 0x5a45c0, pickpocket 0x5a48d0 / 0x5a4b30 / 0x5a4b90); the unit is contiguous 0x58bbd0 .. 0x5a4d40
   and is still the last one (open). No literal breaks the data order: the strings "Locations\Kirche\Spenden"
   0x62beb8 .. "locations\diebesgilde\diebesgilde_spionage" 0x62c078 climb in function order (one .data block,
   D8).
   Session 2026-10-24 (layout, fourth): drafted 0x5a4d40 .. 0x5a6c40 (the thieves' guild kidnapping: the listener
   0x5a4d40, the empty 0x5a4f50, the contact listeners 0x5a4f70 / 0x5a5080, the kidnapping window 0x5a5120 and
   its start 0x5a53e0, the ransom 0x5a5530 and the attempt 0x5a5910 with the listener 0x5a5bc0; then the city
   watch "LOCATIONS\STADTWACHE\STANDARD": the request starts 0x5a5f60 / 0x5a61f0 / 0x5a6480 / 0x5a6720, the
   windows 0x5a5d70 / 0x5a5fd0 / 0x5a6270 / 0x5a64d0, the listeners 0x5a67a0 / 0x5a6920 / 0x5a6a90, the text
   helper 0x5a6b50 and the map helpers 0x5a6b70 / 0x5a6c40). The unit is contiguous 0x58bbd0 .. 0x5a6c40 and is
   still the last one (open). The literals keep climbing 0x62c0d4 .. 0x62c270 in function order (one .data
   block, D8): the city watch is the same unit, no boundary.
   Session 2026-10-24 (layout, fifth): drafted 0x5a6cb0 .. 0x5a9af0 (43 functions; the unit now runs
   0x58bbd0 .. 0x5a9b50, 239 functions, 59 byte): the town map window 0x5a6cb0 (the per-frame draw, ROUGH)
   with its open 0x5a78a0 ("Misc\Landkarte2" 0x62c274), drag scroll 0x5a79e0, key scroll 0x5a7bc0, the
   per-frame 0x5a7cd0 (ROUGH SKELETON) and close 0x5a82f0, and the production settings module of the map
   (the linked list 0x6f26c0 of 0x4c-byte records, the "hp_AddProdSetting" 0x62c284 / "hp_LoadProdSetting"
   0x62c298 strings, 0x5a8340 .. 0x5a8cb0) with its production messages 0x5a8b80 .. 0x5a8d80, and then the
   plague / gypsy catastrophe messages 0x5a8e10 .. 0x5a9470 ("KATASTROPHE_PEST" 0x62c2c0, the
   "NACHRICHTEN_ZIGEUNER" keys 0x62c2f4 / 0x62c310), then the graveyard texts 0x5a9690 .. 0x5a9af0 (the
   FRIEDHOF keys 0x62c32c .. 0x62c39c, the table 0x5eecd0 and the 0x5190a0 order). The literals climb
   0x62c270 .. 0x62c39c in function order (one .data block, D8), so no boundary is inside the stretch.
   Session 2026-10-24 (layout, sixth): drafted 0x5a9b50 .. 0x5ae6b0 (24 functions; the unit now runs
   0x58bbd0 .. 0x5ae870, 263 functions, 65 byte): the Domizil message family (0x5a9b50 .. 0x5a9e40, the
   "NACHRICHTEN_DOMIZIL" keys 0x62c3b8 .. 0x62c400), the Domizil table pickers 0x5a9e90 / 0x5a9f10 (the 0x42
   entries of 0x1c bytes at 0x62c420, the count 0x62cb58), the two percent message boxes 0x5a9f80 / 0x5ad000,
   the Domizil window family (0x5aa0d0, 0x5aa1f0, 0x5aa4b0 the control destructor, 0x5aa520 the selected entry,
   0x5aa700 / 0x5aa9a0 the worker list) and the Personnel / mercenary book / transport windows 0x5aab80 /
   0x5ac110 / 0x5ad150 (ROUGH stubs) with the transport purchase 0x5ae240, the confirmation 0x5ae570 and the
   order message 0x5ae6b0.
   Session 2026-10-24 (layout, seventh): drafted 0x5ae870 .. 0x5b2530 (11 functions; the unit now runs
   0x58bbd0 .. 0x5b26b0, 274 functions, 65 byte, contiguous): the transport order 0x5ae870 (matched: the
   HETIME struct assignment of the clock), the transport check 0x5ae8e0, the warehouse transport 0x5ae9a0
   (ROUGH), the total 0x5af010, the forwarders 0x5af1a0 / 0x5af1b0 / 0x5af1c0 of the transport window
   0x5af1d0 (matched; 0x5af1d0 is a 11968 B STUB), the player's screen image 0x5b2090, the map run callback
   0x5b2180 (ROUGH) and the two-choice window 0x5b2530. The literals climb 0x62cc6c .. 0x62cd1c in function
   order (one .data block, D8), so no boundary at 0x5af1d0 / 0x5b2090.
   Session 2026-10-24 (layout, seventh, continued): drafted 0x5b26b0 .. 0x5bc9c0 (the windows and the Domizil
   table lookups; 99 functions this session, the unit now runs 0x58bbd0 .. 0x5bcc70, 327 functions, 73 byte);
   six more Ghidra-missed starts (0x5b3630, 0x5b3ad0, 0x5b3f30, 0x5b4820, 0x5b4c60, 0x5b5310) and the two
   functions.csv starts 0x5b5690 / 0x5b5a20 that Ghidra's 0x5b5610 record swallows are drafted; matched
   0x5ba600, 0x5ba620, 0x5ba660 and 0x5bc0f0. The next frontier is 0x5bcc70.
   Session 2026-10-24 (layout, eighth): drafted 0x5bcc70 .. 0x5c1df0 (28 functions; the unit now runs
   0x58bbd0 .. 0x5c1df0): the player stats panel 0x5bcc70 ("Panel_player_stats" 0x62d134), the control
   helpers 0x5bd060 / 0x5bd0c0 / 0x5bd190 / 0x5bd1f0, the special-amount list family "special_amt1" 0x62d224
   / "special_amt2" 0x62d204 / "special_amt3" 0x62d214 / "special_amtinfo" 0x62d244: the record block
   0x5bd250, the row builders 0x5bd5e0 / 0x5bdb60 / 0x5be3b0 / 0x5be720, the record status 0x5be1d0, the
   filter / sort fills 0x5bf0d0 / 0x5bf300 / 0x5bf4f0 / 0x5bf6f0, the page switch 0x5bfed0, the tab tables
   0x5c08b0 / 0x5c08d0 / 0x5c0970, the menus 0x5c1130 / 0x5c1b20 / 0x5c1b40 / 0x5c1df0 and the info box
   0x5c1a80. The Ghidra-missed starts 0x5bea00 / 0x5bf4f0 / 0x5bf6f0 are drafted from the disassembly, and
   the ones only reachable through the fill / callback tables as ROUGH stubs: 0x5bf7c0 / 0x5bfa90 / 0x5bfc40 /
   0x5bfd90 (the page fills 0x62d154) and 0x5bffb0 / 0x5c00b0 / 0x5c01c0 / 0x5c02d0 / 0x5c03e0 / 0x5c0550 /
   0x5c0650 / 0x5c0790 (the tab fills), 0x5bff80 (the record release) and 0x5c1500 (the 0x5c1b20 callback);
   0x5c1e10 (846 B, the 0x5c1df0 callback) is a ROUGH stub too. The session went on with the law book module:
   0x5c2160 (the listener of the pages 0x111 / 0x112 / 0x113), 0x5c2410 (**matched**, the info
   "Gesetze\Gesetzbuch_Info" 0x62d2a4), 0x5c2490 (the law entry controls) and 0x5c26a0 (**matched**, the
   master letter "berufe\meisterbrief_1" 0x62d2bc), so the unit runs to 0x5c2730. The literals climb
   0x62d134 .. 0x62d2bc in function order (one .data block, D8), no boundary.
   Session 2026-10-24 (layout, ninth): drafted 0x5c2730 .. 0x5c7560 (22 functions; the unit now runs
   0x58bbd0 .. 0x5c76b0, 396 functions): the master letter module (0x5c2730 **matched**, 0x5c27b0, 0x5c2a10,
   0x5c2b90 "berufe\meisterbrief_2" ROUGH, the contact dispatcher 0x5c3200), the law / office selection
   (0x5c33c0, 0x5c37a0 / 0x5c39a0, 0x5c3bb0 "Gesetze\Auswahl2" ROUGH, 0x5c4100), the big player windows
   (0x5c4150 "Special_Stammbaum" ROUGH, 0x5c5310 "panel_info_spieler" ROUGH, 0x5c6050 "misc\geschenck") and the
   advertising family (0x5c64c0, 0x5c6530, 0x5c6620, 0x5c6910 / 0x5c6aa0 "privillegien_werbung2 / 1",
   0x5c6d00 ROUGH, the bar charts 0x5c72d0 / 0x5c7460 / 0x5c7560 ROUGH). The literals climb 0x62d2bc ..
   0x62d424 in function order (one .data block, D8), no boundary.
   Session 2026-10-24 (layout, tenth): drafted 0x5c76b0 .. 0x5ca150 (15 functions; the unit now runs
   0x58bbd0 .. 0x5ca560, 411 functions, 76 byte): the statistics panel family - the five bar charts
   0x5c72d0 / 0x5c7460 / 0x5c7560 / 0x5c76b0 / 0x5c7800 (they take TWELVE parameters: ten values copied by
   the caller and the two screen words h - 1 / w; the rectangles 0x4764f0 are a SEVEN argument fill
   (x1, y1, x2, y2, r, g, b) - the old 6- and 2-parameter drafts were wrong), the chart window 0x5c7970, the
   statistics panel 0x5c7f00 "panel_statistik_stadt" 0x62d440, the player comparison 0x5c82f0
   "panel_pcompare4plus6" 0x62d468 / "panel_pcompare8" 0x62d458, the privileges panel 0x5c8640
   "panel_privilegien" 0x62d480, the medicus handler 0x5c8bc0 "privillegien_medicus" 0x62d494, the divorce
   handler 0x5c8ef0 "privillegien_scheidung" 0x62d4ac, the blackmail handler 0x5c9290
   "privillegien_erpressung2" 0x62d4c4 and the building / payment windows 0x5c9550 0x62d4e0, 0x5c9990,
   0x5c9c50 and 0x5c9df0 0x62d4f8 (the last three are Ghidra-missed starts, drafted from the disassembly),
   the blackmail contact variant 0x5c9ff0 (0x1d61) and the law / privilege window 0x5ca150
   "privillegien_gesetz" 0x62d518 (ROUGH: the law info 0x4f2720 and the setter 0x4f2750 of the hst.c view).
   The literals climb 0x62cd48 .. 0x62d518 in function order (one .data block, D8), no boundary.
   Session 2026-10-24 (layout, eleventh): drafted 0x5ca560 .. 0x5cb250 (7 functions; the unit now runs
   0x58bbd0 .. 0x5cb4f0): the privilege handlers 0x5ca560 (embezzlement: "absetzen" 0x62d52c, the kind
   check 0x4e6f80 / 0x4e6030 and the window 0x5bea50), 0x5ca860 ("privillegien\unterschlagung" 0x62d538,
   the price over 0x4fa700 / 0x42ec00 / 0x5eceb0), the Ghidra-missed 0x5caab0 (the bewitch window
   "privillegien\becircen" 0x62d554, reached only as the menu callback of 0x5cacd0) and 0x5cacd0 (the
   contract handler with the 0x5c1130 menu), then 0x5cae00 (the counter-espionage
   "privillegien\gegenspionage" 0x62d56c with the carrier walk 0x53e400 / 0x53e300 and the clock copies
   +0x52 / +0xc8 as HETIME assignments), the wrapper 0x5cb1e0 (0x4e6ba0; behaviour grade, 49/112) and
   0x5cb250 (the "privillegien\gegenspionage2" 0x62d588 window, ROUGH: the 0x300-record kind walk). The
   literals climb 0x62d52c .. 0x62d588 in function order (one .data block, D8), no boundary. Then the
   espionage family 0x5cb4f0 / 0x5cba90 / 0x5cbe00 and the order handlers 0x5cc2b0 / 0x5cc700 (ROUGH
   SKELETONs from tools/disasm.py: the price over 0x50a640 / 0x509e60, the relations 0x50c520, the
   0x5c1130 menus, the 0x53dda0 messages and the building 0x501190) extend the unit to 0x5ccc10, with
   0x5cca40 (the value window 0x62d634) and the spy family 0x5ccc10 .. 0x5ce4b0 complete the stretch;
   the next frontier is 0x5ce610.
   Session 2026-10-24 (layout, twelfth): drafted 0x5ce610 .. 0x5d0e60 (25 functions; the unit now runs
   0x58bbd0 .. 0x5d0f20, 457 functions, 78 byte): the interrogation / trial-step message module (the
   "privillegien_verhoer" 0x62d5c4 / _arbeiter_ausweisen 0x62d5dc / CHOOSEPROFESSION2 0x62d660 /
   "special_uni" 0x62d6bc windows, the object callback 0x5cf8a0, the trial step 0x5d08c0 and its wrappers
   0x5d09d0 / 0x5d0a40 / 0x5d0ab0, the amount transfers 0x5d0b20 / 0x5d0c70 and the shuffled pick
   0x5d0e60). Matched 0x5cfb60 and 0x5d06e0; the thirteen Ghidra-missed starts are drafted. 0x5d08c0 reads
   the earlier he.c pooled strings (backward), so no new .data run: the unit continues (open).
   Session 2026-10-24 (layout, thirteenth): drafted 0x5d0f20 .. 0x5d3270 (23 functions; the unit now runs
   0x58bbd0 .. 0x5d33b0, 480 functions): the trial / action callbacks of the record table 0x62c420 (records
   5 / 17 / 19 / 21 / 22 / 32 / 34 / 36 .. 44 / 49 / 50 / 53 .. 55 / 57 / 59 .. 61; the function pointer
   sits at record +0x14, the text ids 0x15c .. 0x2c3). They are reached only through that table (no direct
   call) and use the 0x5c1130 menu / 0x505df0 / 0x5ba6b0 / 0x5ba660 character lookups and the text /
   message API (0x5176d0 / 0x53dda0 / 0x4c4b00 / 0x5cf820); 0x5d0f20's two random picks are drafted, the
   rest are ROUGH stubs from tools/disasm.py (see notes/units/unk_58bbd0.md). No literal of the unit
   belongs to them, so the .data order is unchanged; the next unit starts at 0x5d33b0.
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
#include "unk_58bbd0.h"
#include "di_input.h"
#include "evt.h"
#include "d3.h"
#include "d3_light.h"
#include "data_views.h"

extern void d3s_ResetShadows(struct D3OBJECT *obj);
extern void ai_Unk4c4ac0(int a, int b, int c);
/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_6f5a84[];
extern GAPDATA full_data_6f4f64[];
extern GAPDATA full_data_6f58a4[];
extern GAPDATA full_data_6f5a64[];
extern char * ai_BTypes;
extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern GAPDATA full_data_62b038[];
extern unsigned char ai_FallbackBuilding;
extern char * ai_PlayerBuilding;
extern GAPDATA full_data_6f5320[];
extern GAPDATA full_data_6f5344[];
extern GAPDATA full_data_6f5348[];
extern GAPDATA full_data_6f5350[];
extern GAPDATA full_data_6f5354[];
extern GAPDATA full_data_6f5358[];
extern GAPDATA full_data_6f535c[];
extern GAPDATA full_data_6f536c[];
extern GAPDATA full_data_6f58d0[];
extern char * ai_Goods;
extern unsigned char ai_TimeHour;
extern float ai_Unk617830[];
extern float ai_Unk617840[];
extern GAPDATA full_data_62b054[];
extern GAPDATA full_data_62b06c[];
extern GAPDATA full_data_62b084[];
extern GAPDATA full_data_6f5324[];
extern GAPDATA full_data_6f5334[];
extern GAPDATA full_data_6f534c[];
extern GAPDATA full_data_6f5a60[];
extern int ai_Unk5fe710;
extern char * cm_FieldObject;
extern GAPDATA full_data_6f4f0c[];
extern GAPDATA full_data_6f4f5c[];
extern GAPDATA full_data_6f4f60[];
extern GAPDATA full_data_6f4f70[];
extern GAPDATA full_data_6f4f80[];
extern GAPDATA full_data_6f4f8c[];
extern GAPDATA full_data_6f4f90[];
extern GAPDATA full_data_6f4f94[];
extern GAPDATA full_data_6f52e0[];
extern GAPDATA full_data_6f52ec[];
extern GAPDATA full_data_6f52ef[];
extern GAPDATA full_data_6f5300[];
extern GAPDATA full_data_6f5314[];
extern GAPDATA full_data_6f58a0[];
extern GAPDATA full_data_6f58b0[];
extern GAPDATA full_data_6f58c0[];
extern GAPDATA full_data_6f58cc[];
extern GAPDATA full_data_6f58d4[];
extern GAPDATA full_data_6f5a6f[];
extern GAPDATA full_data_6f5a80[];
extern unsigned char d2_Pics;
extern char d2_Views[];
extern char d2_Wins[];
extern GAPDATA full_data_6f4f00[];
extern GAPDATA full_data_6f4f04[];
extern GAPDATA full_data_6f4f08[];
extern GAPDATA full_data_6f4f10[];
extern GAPDATA full_data_6f4f6c[];
extern GAPDATA full_data_6f52e4[];
extern GAPDATA full_data_6f52e8[];
extern GAPDATA full_data_6f52f0[];
extern GAPDATA full_data_6f5318[];
extern GAPDATA full_data_6f58ac[];
extern GAPDATA full_data_6f5a6c[];
extern GAPDATA full_data_6f5a70[];
extern GAPDATA full_data_6f5310[];
extern unsigned char ai_OwnPlayer;


extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern char d2_Views[];
extern char d2_Wins[];
extern GAPDATA full_data_62b098[];
extern int ai_Unk5fe710;
extern char * cm_FieldObject;
extern GAPDATA full_data_6f4f0c[];
extern GAPDATA full_data_6f4f5c[];
extern GAPDATA full_data_6f4f60[];
extern GAPDATA full_data_6f4f70[];
extern GAPDATA full_data_6f4f80[];
extern GAPDATA full_data_6f4f8c[];
extern GAPDATA full_data_6f4f90[];
extern GAPDATA full_data_6f4f94[];
extern GAPDATA full_data_6f52e0[];
extern GAPDATA full_data_6f52ef[];
extern GAPDATA full_data_6f5300[];
extern GAPDATA full_data_6f5310[];
extern unsigned char d2_Pics;
extern GAPDATA full_data_6f4f00[];
extern GAPDATA full_data_6f4f04[];
extern GAPDATA full_data_6f4f08[];
extern GAPDATA full_data_6f4f10[];
extern GAPDATA full_data_6f52f0[];
extern GAPDATA full_data_6f58b0[];
extern GAPDATA full_data_6f5a70[];
extern unsigned char ai_Unk5fb5b0;
extern char * ai_BTypes;
extern char * ai_Unkfc2dec;
extern GAPDATA full_data_6f26b0[];
extern unsigned char kb_Last;

extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern GAPDATA full_data_62af98[];
extern int sv_ResearchClass;
extern unsigned char ai_FallbackBuilding;
extern char * ai_PlayerBuilding;
extern int ai_Unk5fe710;
extern int ai_Unk623838;
extern int ai_Unk62383c;
extern char ch_EmptyName[1];
extern GAPDATA full_data_6f2338[];
extern unsigned char kb_Last;
extern char * ai_BTypes;
extern char * ai_Goods;
extern int ai_Unk6f2334;
extern GAPDATA full_data_6f22fc[];
extern GAPDATA full_data_76045c[];
extern int sv_FrameBase;
extern int sv_FrameDepth;
extern int sv_PanelFlag;
extern char * sv_PanelObject;
extern unsigned short * sv_PanelRecord;
extern int ai_Unk75f280[576];
extern char * ai_Unkfc2dec;
extern char ai_Chars[];
extern int ai_GameFlags;
extern unsigned char ai_GameState;
extern unsigned char ai_MarketBuilding;
extern unsigned char ai_TimeHour;
extern float ai_Unk617830[];
extern float ai_Unk617840[];
extern int ai_Unk623884;
extern int * ai_Unk633738;
extern char * ai_Unk75ec60;
extern int d2_ClickedId;
extern int d2_CutsceneActive;
extern short d2_MouseX;
extern short d2_MouseY;
extern short d2_PointerLook;
extern unsigned char d3t_CamMove;
extern int di_Absolute;
extern int di_MaxX;
extern int di_MaxY;
extern int di_MinX;
extern int di_MinY;
extern GAPDATA full_data_5f3418[];
extern GAPDATA full_data_6f1c2c[];
extern GAPDATA full_data_6f22f4[];
extern GAPDATA full_data_6f2598[];
extern unsigned char snd_AmbientOn;
extern void sv_UpdateClock(void);
extern unsigned short * sv_PanelShown;
extern int ai_Unk7d6a40;

extern int sv_PopRequest;
extern int sv_ResearchClass;


extern char * ai_Goods;
extern int ai_Unk5fe710;
extern int ai_Unk75f280[576];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern int d2_Unkf8aac4;
extern GAPDATA full_data_6f5320[];
extern GAPDATA full_data_6f5354[];
extern unsigned short * sv_PanelShown;
extern unsigned short ai_Unk76fdc4;
extern unsigned ai_Unk889400[732];
extern GAPDATA full_data_6f5324[];
extern GAPDATA full_data_6f534c[];
extern GAPDATA full_data_6f5a60[];






extern int ai_Unk5fe710;
extern int ai_Unk623898;
extern int d2_ClickedId;











extern unsigned char ai_FallbackBuilding;
extern unsigned char ai_TimeHour;
extern int ai_Unk5fe710;
extern unsigned char ai_Unk7603a0;
extern float ai_Unk760420;
extern int d2_ClickedId;
extern int sv_PopRequest;

extern unsigned short * sv_PanelShown;
extern int sv_PopRequest;



extern unsigned char ai_FallbackBuilding;
extern char * ai_PlayerBuilding;
extern int sv_PopRequest;
extern int d2_ClickedId;
extern GAPDATA full_data_62b3b8[];














extern char * ai_BTypes;
extern unsigned char ai_TimeHour;

extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_Objs;

extern int sv_PopRequest;
extern unsigned char ai_TimeHour;

extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern int sv_PopRequest;






extern int ai_Unk5b3630();
extern int ai_Unk5b3f30();
extern int ai_Unk6f2448;



extern unsigned char kb_Last;



extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;


extern int sv_PopRequest;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_602568[];
extern unsigned char ai_TimeHour;
extern unsigned char d2_LastWindowPtr;
extern int ai_Unk5b3ad0();
extern char ai_Chars[];

extern int sv_PopRequest;






extern GAPDATA full_data_6f2450[];
extern unsigned char kb_Last;
extern int d2_LastWindow;
extern int sv_PopRequest;
extern int ai_Unk6333cc;
extern char ai_Chars[];
extern unsigned char ai_TimeHour;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern GAPDATA full_data_602568[];
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Pics;



extern GAPDATA full_data_6f2450[];
extern unsigned char kb_Last;

extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern int ai_CameraStep;
extern char ai_PlayerSlots[];
extern int ai_Unk6333cc;
extern unsigned char ai_Unk7d6a60;
extern char d2_Views[];
extern char d2_Wins[];
extern int sv_PopRequest;
extern const float ai_Unk5ebba4[];
extern char * ai_Unk6f22e8;

extern unsigned char ai_MarketBuilding;
extern int ai_Unk6f2304;
extern int ai_Unk6f2318;
extern char * ai_Unk75ec60;
extern int sv_PanelForm;
extern unsigned short * sv_PanelShown;

extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[];









extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned short * sv_PanelShown;
extern int sv_PopRequest;
extern unsigned char ai_TimeHour;
extern int ai_Unk5fe710;

extern int sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Pics;


extern int sv_PopRequest;
extern int d2_LastWindow;
extern unsigned char d2_Pics;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;
extern GAPDATA full_data_6f22dc[];
extern int ai_Unk5fe710;

extern int sv_PopRequest;
extern unsigned char ai_OwnPlayer;

extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Objs;

extern char ch_EmptyName[1];
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Objs;
extern char ai_PlayerSlots[];

extern char ch_EmptyName[1];
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Objs;
extern int ai_Unk5fe710;
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_Unkf8aae0;
extern int d2_ClickedId;



extern int sv_PopRequest;
extern int d2_ClickedId;
extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[];
extern int ai_Unk5fe710;
extern char ch_EmptyName[1];
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Objs;


extern int gm_Offers[528];
extern int sv_PopRequest;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;



extern char * ai_PlayerBuilding;
extern int ai_Unk6f22cc;
extern int ai_Unk6f22d0;
extern char * ai_Unkfc2dec;
extern GAPDATA full_data_6f2410[];
extern int sv_FrameBase;
extern int sv_FrameDepth;
extern int sv_PopRequest;
extern char * ai_BTypes;
extern unsigned char ai_TimeHour;
extern GAPDATA full_data_62afa0[];
extern GAPDATA full_data_62afa4[];
extern GAPDATA full_data_62afa5[];
extern GAPDATA full_data_62afa8[];
extern int ai_GameFlags;

extern int sv_PanelFlag;
extern int ai_FrameWait;
extern char * ai_PlayerBuilding;
extern const float ai_Unk5ebba4[];
extern int ch_CurrentGroup;
extern int evt_NoGroup;
extern int ai_Unk5f8438;
extern unsigned char ai_Unk5fb5b0;
extern int ai_Unk6f22cc;
extern int ai_Unk6f22d0;
extern int ai_Unk75fb80;
extern int ai_Unk75fb84;
extern int ai_Unk75fb88;
extern int ai_Unk75fb90;
extern int ai_Unk75fb94;
extern int ai_Unk75fb98;
extern int ai_Unk7662c0;
extern char * ai_Unkfc2dec;
extern int ch_CurrentScriptGroup;
extern char ch_StepSample[];
extern int d2_CutsceneActive;
extern GAPDATA full_data_6237c8[];
extern GAPDATA full_data_6f2314[];
extern GAPDATA full_data_6f2410[];
extern unsigned char init_ObjectUniverse;
extern int sv_FrameBase;
extern int sv_FrameDepth;
extern unsigned short * sv_PanelShown;
extern int sv_PopRequest;
extern char * ai_BTypes;
extern unsigned char ai_FallbackBuilding;
extern char ai_GfxFloor[];
extern int * ai_Unk633738;
extern unsigned char d3_Sky;
extern unsigned char snd_AmbientOn;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int ai_Unk5fe710;
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern int d2_LastWindow;

extern int ai_Unk6f2448;



extern unsigned char kb_Last;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern char ai_PlayerSlots[];
extern int d2_ClickedId;
extern int sv_PopRequest;
extern int ai_Unk5fe710;
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Pics;



extern char gm_PlayerRecords[];
extern int sv_PopRequest;
extern char ai_Chars[];
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;




extern int sv_PopRequest;





extern char * ai_BTypes;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int sv_PopRequest;
extern int d2_LastWindow;


extern char ai_Chars[];
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_Objs;




extern int sv_PopRequest;
extern char ai_Chars[];
extern int d2_ClickedId;
extern int d2_LastWindow;




extern char ai_Chars[];

extern char ai_Chars[];


extern char ai_Chars[];

extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern int sv_PopRequest;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Pics;
extern int sv_PopRequest;
extern char ai_Chars[];
extern int ai_Unk5fe710;

extern int sv_PopRequest;

extern char ai_Chars[];
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Pics;
extern GAPDATA full_data_602568[];
extern int sv_PopRequest;

extern int ai_Unk6f2448;




extern unsigned char kb_Last;
extern char ai_Chars[];
extern char gm_PlayerRecords[];
extern int sv_PopRequest;
extern unsigned char ai_OwnPlayer;
extern int ai_Unk5fe710;
extern int d2_ClickedId;

extern int sv_PopRequest;

extern char ai_Chars[];
extern char * ai_PlayerBuilding;
extern int ai_Unk5fe710;
extern int d2_ClickedId;


extern int sv_PopRequest;
extern char * ai_BTypes;
extern char ai_Chars[];
extern unsigned char ai_FallbackBuilding;
extern char * ai_PlayerBuilding;
extern int ai_Unk5fe710;
extern int d2_ClickedId;

extern int sv_PopRequest;
extern char * ai_BTypes;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern GAPDATA full_data_602568[];
extern char * ai_PlayerBuilding;
extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk7603a0;
extern float ai_Unk760420;

extern int sv_PopRequest;

extern unsigned char ai_TimeHour;
extern unsigned char ai_Unk7603a0;
extern float ai_Unk760420;
extern int d2_ClickedId;

extern int sv_PopRequest;







extern unsigned short * sv_PanelShown;


extern char ai_Chars[];
extern char * ai_PlayerBuilding;
extern int ai_Unk5fe710;
extern int d2_ClickedId;

extern int sv_PopRequest;
extern const int ai_Unk5eeab8[24];

extern int ai_Unk5fe710;
extern char * cm_FieldObject;
extern int d2_ClickedId;


extern int sv_PopRequest;
extern char ai_Chars[];
extern char * cm_FieldObject;
extern GAPDATA full_data_627978[];




extern int ai_Unk5a5d70();

extern int ai_Unk5a5fd0();

extern int ai_Unk5a5d70();
extern int ai_Unk5a6270();
extern char * ai_PlayerBuilding;
extern int ai_Unk5fe710;
extern int d2_ClickedId;

extern int sv_PopRequest;
extern int ai_Unk5a5d70();
extern int ai_Unk5a5fd0();
extern char * ai_PlayerBuilding;
extern int ai_Unk5fe710;
extern int d2_ClickedId;

extern int sv_PopRequest;
extern int ai_Unk5a5fd0();
extern int ai_Unk5a6270();
extern char * ai_PlayerBuilding;
extern unsigned char ai_TimeHour;
extern int ai_Unk5fe710;
extern unsigned char ai_Unk7603a0;
extern float ai_Unk760420;
extern int d2_ClickedId;

extern unsigned short * sv_PanelShown;
extern int sv_PopRequest;
extern int ai_Unk5a6270();
extern char * ai_PlayerBuilding;
extern unsigned char ai_TimeHour;
extern int ai_Unk5fe710;
extern unsigned char ai_Unk7603a0;
extern float ai_Unk760420;
extern int d2_ClickedId;



extern unsigned short * sv_PanelShown;
extern int sv_PopRequest;





extern unsigned short * sv_PanelShown;



extern int ai_MapSize;
extern GAPDATA full_data_6f4ee4[];



extern unsigned char ai_AmbushSpots;
extern char ai_Chars[];
extern unsigned char ai_FallbackBuilding;
extern int ai_MapSize;
extern unsigned char ai_OwnPlayer;
extern char * ai_PlayerBuilding;
extern char ai_PlayerSlots[];
extern unsigned char ai_Unk625820;
extern char * ai_Unkfc2dec;
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Objs;
extern char d2_Wins[];
extern GAPDATA full_data_6f26b0[];
extern GAPDATA full_data_6f4ee4[];
extern GAPDATA full_rdata_5eed8c[];
extern GAPDATA full_rdata_5eed90[];
extern char * ai_Unk6f26ac;
extern unsigned char d2_BackgroundTile;
extern GAPDATA full_data_62c274[];
extern short d2_MouseX;
extern short d2_MouseY;
extern int di_Absolute;
extern int di_MaxX;
extern int di_MaxY;
extern int di_MinX;
extern int di_MinY;
extern GAPDATA full_data_6f268c[];
extern GAPDATA full_data_6f2690[];
extern GAPDATA full_data_6f2698[];
extern GAPDATA full_data_6f269c[];
extern GAPDATA full_data_6f26a4[];
extern GAPDATA full_data_6f26a8[];
extern GAPDATA full_data_6f26b4[];
extern GAPDATA full_data_6f26b8[];
extern GAPDATA full_data_6f26bc[];
extern unsigned char ai_Unk1468ae0;
extern int ai_Unk5fe710;
extern int ai_Unk62a8cc;
extern int ai_Unk62c26c;
extern int ai_Unk6333cc;
extern int d2_Unkf8aac8;
extern char d2_Views[];
extern GAPDATA full_data_6f26c0[];
extern char ai_Unk61a348[];
extern GAPDATA full_rdata_5eed94[];
extern GAPDATA full_rdata_5eedd3[];
extern GAPDATA full_data_62c284[];
extern GAPDATA full_rdata_5ebb40[];
extern GAPDATA full_data_62c298[];
extern char ch_EmptyName[1];
extern GAPDATA full_data_6f26c8[];
extern GAPDATA full_data_62c2ac[];
extern GAPDATA full_data_62c2c0[];
extern GAPDATA full_data_62c2dc[];



extern GAPDATA full_data_62c2f4[];
extern GAPDATA full_data_62c310[];
extern GAPDATA full_data_6f26c8[];
extern char ch_EmptyName[1];
extern const int ai_Unk5eeab8[24];
extern GAPDATA full_data_62c32c[];
extern GAPDATA full_data_62c348[];
extern GAPDATA full_data_62c364[];
extern GAPDATA full_data_62c380[];
extern GAPDATA full_data_62c39c[];
extern GAPDATA full_data_6f2ac8[];
extern GAPDATA full_data_62c3b8[];
extern GAPDATA full_data_62c3e8[];
extern GAPDATA full_data_62c400[];
extern GAPDATA full_data_62c420[];
extern GAPDATA full_data_62cb58[];
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;

extern GAPDATA full_data_60ea68[];
extern unsigned char kb_Last;
extern int sv_FrameMask;
extern int sv_PopRequest;

extern char ch_EmptyName[1];
extern int d2_CurrentView;
extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern char * ai_BTypes;
extern GAPDATA full_data_60d568[];

extern char ch_EmptyName[1];
extern unsigned char d2_Objs;

extern unsigned char ai_MarketBuilding;
extern int d2_LastWindow;
extern unsigned char d2_Objs;

extern GAPDATA full_data_602568[];
extern GAPDATA full_data_60d38c[];
extern GAPDATA full_data_624ec4[];
extern GAPDATA full_data_624fcc[];
extern GAPDATA full_data_62cbb4[];
extern GAPDATA full_data_6f40a0[];
extern GAPDATA full_data_6f40a4[];
extern GAPDATA full_data_6f40b0[];
extern GAPDATA full_data_6f40bc[];
extern unsigned char ai_MarketBuilding;
extern int ai_Unk5fe710;
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_Objs;

extern GAPDATA full_data_6f40a0[];
extern GAPDATA full_data_6f40a4[];
extern GAPDATA full_data_6f40b0[];
extern GAPDATA full_data_6f40bc[];
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int d2_ClickedId;

extern GAPDATA full_data_60ea68[];
extern unsigned char kb_Last;
extern int sv_FrameMask;
extern int sv_PopRequest;
extern char ai_Chars[];
extern unsigned char ai_OwnPlayer;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62cc6c[];
extern unsigned char ai_TimeHour;
extern GAPDATA full_data_6f2f64[];
extern char * ai_BTypes;
extern GAPDATA full_data_60c28c[];
extern GAPDATA full_data_62cc94[];
extern GAPDATA full_data_6f4f60[];
extern GAPDATA full_data_6f52df[];
extern GAPDATA full_data_6f58a0[];
extern GAPDATA full_data_6f5a60[];
extern GAPDATA full_data_6f22dc[];
extern GAPDATA full_data_6f52e0[];
extern int ai_MapSize;
extern char ai_PlayerSlots[];
extern char * ai_Unk6f26ac;
extern char ai_Unkf8ef80[256];
extern GAPDATA full_data_62c270[];
extern GAPDATA full_data_62cce8[];
extern GAPDATA full_data_6f4ee4[];
extern char ch_EmptyName[1];
extern GAPDATA full_data_62ccfc[];
extern int ai_Unk5b3630();
extern int ai_Unk5b3ad0();
extern int ai_Unk5b3f30();
extern char ai_Unk91f700[];
extern GAPDATA full_data_62cd7c[];
extern GAPDATA full_data_62cda8[];
extern char * ai_Unk6f22e8;
extern GAPDATA full_data_62c418[];
extern GAPDATA full_data_62c420[];
extern GAPDATA full_data_6f3880[];
extern GAPDATA full_data_6f3884[];
extern char * cm_FieldObject;
extern unsigned char d2_Objs;
extern char d2_Radios[];



extern GAPDATA full_data_62d134[];
extern GAPDATA full_data_6f2f80[];
extern GAPDATA full_data_6f2f84[];
extern GAPDATA full_data_6f3680[];
extern GAPDATA full_data_6f3870[];
extern GAPDATA full_data_6f4080[];
extern GAPDATA full_data_6f40f4[];
extern GAPDATA full_data_6f4ce0[];
extern GAPDATA full_data_6f4cf4[];
extern int sv_PopRequest;
extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern int d2_TextDrawnH;
extern int d2_TextDrawnW;
extern int d2_TextLeft;
extern int d2_TextTop;
extern char ai_Chars[];
extern GAPDATA full_data_62a578[];
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Pics;
extern GAPDATA full_data_60d224[];
extern GAPDATA full_data_62d1e8[];
extern GAPDATA full_data_62d1ec[];
extern GAPDATA full_data_62d1f4[];
extern GAPDATA full_data_62d1fc[];
extern signed char ai_Relations[768][768];
extern const int ai_Unk5eeab8[24];
extern GAPDATA full_data_62d204[];
extern char gm_PlayerRecords[];
extern float ai_Unk5eb788;
extern GAPDATA full_rdata_5ed0fc[];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62d150[];
extern GAPDATA full_data_62d1f0[];
extern GAPDATA full_data_62d214[];
extern GAPDATA full_data_6f2f8c[];
extern int ai_Unk5fe710;
extern int d2_ClickedId;

extern GAPDATA full_data_62d224[];
extern int sv_PopRequest;
extern GAPDATA full_data_62d204[];
extern int d2_ClickedId;
extern GAPDATA full_data_62d234[];
extern GAPDATA full_data_62d244[];

extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_LastWindowPtr;
extern unsigned char d2_Objs;

extern GAPDATA full_data_62d224[];
extern int sv_PopRequest;

extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern GAPDATA full_data_625380[];
extern GAPDATA full_data_62bdc8[];
extern GAPDATA full_data_62bde4[];
extern GAPDATA full_data_62be00[];
extern GAPDATA full_data_62d284[];
extern GAPDATA full_data_62d294[];
extern int sv_PopRequest;
extern GAPDATA full_data_62d2a4[];
extern char * ai_BTypes;
extern unsigned char ai_OwnPlayer;
extern int d2_LastWindow;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_624cf8[];
extern GAPDATA full_data_62d2bc[];
extern unsigned char ai_TimeHour;
extern unsigned short * sv_PanelShown;
extern char * ai_Goods;
extern unsigned char d2_Objs;
extern char * cm_FieldObject;


extern GAPDATA full_data_62d2d4[];
extern GAPDATA full_data_6f2f90[];
extern int sv_PopRequest;
extern char * ai_BTypes;
extern char ai_Chars[];
extern unsigned short * ai_PlayerChar;
extern signed char ai_Relations[768][768];
extern unsigned char ai_Unka69140;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62a564[];
extern GAPDATA full_data_62d2ec[];
extern unsigned char d2_Objs;
extern GAPDATA full_data_62d1fc[];
extern GAPDATA full_data_62d300[];
extern GAPDATA full_data_62d3a0[];
extern GAPDATA full_data_62d3b8[];
extern GAPDATA full_data_62d3c4[];
extern GAPDATA full_data_62d3d4[];
extern GAPDATA full_data_62d3e0[];
extern int sv_FrameMask;
extern unsigned char ai_OwnPlayer;
extern GAPDATA full_data_62d3f4[];
extern GAPDATA full_data_62d3fc[];
extern unsigned char ai_TimeHour;
extern GAPDATA full_data_62d40c[];
extern GAPDATA full_data_62d424[];
extern unsigned ai_Unk91ac40[136];
extern unsigned char ord_StatisticsHistory;
extern float ai_Unk5eb81c;
extern float ai_Unk5eb988;
extern unsigned int d2_PenSize;
extern GAPDATA full_data_5fe7d0[];
extern GAPDATA full_data_62d43c[];
extern char ai_PlayerSlots[];
extern int ai_RatingBase;
extern GAPDATA full_data_62d440[];
extern GAPDATA full_data_6f2f88[];
extern GAPDATA full_rdata_5eb8a8[];
extern GAPDATA full_data_62d458[];
extern GAPDATA full_data_62d468[];

extern int ai_Unk5fe710;
extern char ai_Unk60c6c0[4];
extern int ai_Unk6333cc;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_625150[];
extern GAPDATA full_data_625154[];
extern GAPDATA full_data_62d480[];
extern int sv_PopRequest;
extern unsigned char ai_OwnPlayer;
extern GAPDATA full_data_62d494[];
extern GAPDATA full_data_62d4ac[];
extern unsigned char ai_TimeHour;
extern GAPDATA full_data_62d4c4[];

extern unsigned char ai_TimeHour;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern GAPDATA full_data_62cd9c[];
extern GAPDATA full_data_62d4e0[];
extern int sv_PopRequest;
extern char ai_Chars[];
extern GAPDATA full_data_62cd48[];
extern GAPDATA full_data_62d4f8[];
extern GAPDATA full_data_62d518[];
extern const int ai_Unk5eeab8[24];
extern GAPDATA full_data_62d538[];
extern GAPDATA full_data_62d554[];
extern GAPDATA full_data_62d56c[];
extern GAPDATA full_data_624204[];
extern GAPDATA full_data_62d588[];
extern char gm_PlayerRecords[];
extern unsigned char ai_Unk625820;
extern unsigned char ai_OwnPlayer;
extern GAPDATA full_data_62d634[];
extern GAPDATA full_data_62d64c[];
extern GAPDATA full_data_62d660[];
extern char * ai_BTypes;
extern char ai_Unk91f700[];
extern GAPDATA full_data_62d694[];

extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern GAPDATA full_data_62d5c4[];
extern int sv_PopRequest;
extern unsigned char ai_TimeHour;
extern GAPDATA full_data_62d5dc[];

extern unsigned char ai_TimeHour;
extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern GAPDATA full_data_62d5c4[];
extern GAPDATA full_data_62d6a8[];
extern int sv_PopRequest;

extern int ai_Unk5fe710;
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern GAPDATA full_data_62d5c4[];
extern int sv_PopRequest;

extern int ai_Unk5fe710;
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern GAPDATA full_data_62d5c4[];
extern int sv_PopRequest;

extern int d2_ClickedId;
extern int d2_LastWindow;
extern unsigned char d2_Objs;
extern unsigned char d2_Pics;
extern GAPDATA full_data_62d660[];
extern int sv_FrameMask;
extern int sv_PopRequest;
extern char ai_Chars[];
extern int ai_Unk5fe710;
extern GAPDATA full_data_60d224[];
extern GAPDATA full_data_62d6bc[];
extern GAPDATA full_data_62d010[];
extern unsigned char ai_OwnPlayer;
extern unsigned char ai_TimeHour;
extern char * cm_FieldObject;
extern float ai_Unk5eb94c;
extern const int ai_Unk5eeab8[24];
extern signed char ai_Relations[768][768];
extern unsigned char he_Events;
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win);
int d2_WinOpenPic(short x, short y, int pic, int win);
void d2_CreateScrollButtons();
void d2_EnableObject();
void d2_EnableViewObjects();
void d2_RaiseObject();
void d2_RemoveObj();
void d2_ScrollWindowBy();
void d2_SetBackgroundTexture();
void d2_SetCaption();
void d2_SetObjEnabled();
void d2_SetObjFont();
void d2_SetObjPic();
void d2_SetObjText();
void he_Unk53e1f0();
void sv_CameraFollow();
int util_Random(unsigned short n);

int sprintf(char *buf, const char *fmt, ...); /* 0x5de652, the CRT's sprintf */

struct AMTREC;  /* file-scope tag for the prototypes below */

#define AI_4A63F0_7(p, a, fl, b, c, f, o) ((int (*)(void *, int, unsigned int, int, int, float, int *))ai_Unk4a63f0)(p, a, fl, b, c, f, o)

struct GCLOCK;  /* f3gm.h typedef; file-scope tag for the gm_AddTime prototype */

unsigned short * gm_QueryNext(void); /* 0x4ffcc0, moved above its first call */

#define U_I(a) (*(int *)(a))
#define U_W(a) (*(unsigned short *)(a))

#pragma pack(push, 1)
typedef struct AIBLD {             /* the building record fields this unit reads (ai.h AIBLD, same layout) */
    unsigned char kind;            /* 0x00, the building kind (the index into the name tables) */
    char pad1[0x26];
    unsigned short owner;          /* 0x27, the owner's character id */
    char pad29[0x34];
    int objList;                   /* 0x5d, the head of the building's object-instance list (ai.h AIBLD.objList) */
    int object;                    /* 0x61, its object in the scene */
} AIBLD;
#pragma pack(pop)
#pragma pack(push, 2)
typedef struct HETIME {            /* the 14 byte game time (ai.h AITIME; he.c has the same local view) */
    int a;
    unsigned short b;
    int c;
    int d;
} HETIME;
#pragma pack(pop)
extern HETIME ai_Time;                  /* 0x770ae0 (ai.h AITIME, defined in he.c) */
float util_RandomF();                   /* the float roll (ai.h util_RandomF) */

#include "time_view.h"

