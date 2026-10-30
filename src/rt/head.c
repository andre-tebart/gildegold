/* rt.c -- routing: route point lists ("rt_point"), the routing dialogs ("misc\routing_choose_good_enhanced",
   "misc/routing_edit_point", "misc\routing_settings"), "rt_load". Start: its .data block 0x602a90 begins with a -1
   global right after rd.c's literals. Names are ours.
   unit of rea 8cef7d5b (src/game/rt.c); boundaries in decl/text.dcl, notes in src/rt/open.md */
#include "d2_types.h"
extern D2RADIO d2_Radios[];
#include "txt_views.h"
#include <stdio.h>
#include <string.h>
#include "rt.h"
#include "di_input.h"
#include "data_views.h"
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue)  /* 0x475cb0 (d2.c: five parameters): @original rt's calls push 4 arguments (as 139 of the original's call sites do) */
#define d2_ObjsC (*(char **)&d2_Objs)  /* 0xf8f080, d2_Objs (d2.c: D2OBJ *) read bytewise, as this unit does (+ obj * 0x2ec) */
#define cm_SendExSellObjectI ((void (*)(int, int, int, int, int, int))cm_SendExSellObject)  /* 0x4c4880 (cm.h: short c, unsigned char e): @original rt.c pushes the third argument as an int (byte-matched rt_UnloadAtPoint) */
extern char d3_Scenes[];

int rt_MarkGroup = -1;                  /* 0x602a90 */
int ai_Unk62c26c = 1;  /* 0x62c26c */
extern RTMARK rt_Marks[8];
/* ai_Unk143a588: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad24: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad28: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad2c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad30: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad34: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad38: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad3c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad44: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad4c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad54: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad58: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad5c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad60: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad64: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144ad78: view of txt_Ptrs (build/mk_txt_views.py) */
int rt_Unk6f074c = 0;  /* 0x6f074c, the saved screen of */
int rt_MarkCount = 0;  /* 0x6f0750, the marks used */
int rt_PickedBuilding = 0;  /* 0x6f0754, the building shown */
RTPOINT * rt_EditRoute = 0;  /* 0x6f0758, the route being edited */
char * ai_Unk6f26ac = 0;  /* 0x6f26ac, the screen surface */
/* ai_Unk7e422c: view of ai_PlayerSlots (tools/data_views.py) */
RTMARK rt_Marks[8] = {0};  /* 0xf812a0 */

