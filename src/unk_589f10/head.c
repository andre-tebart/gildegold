/* unk_589f10.c -- the network lobby (0x589f10 .. 0x58bbd0): the "menu\netlobby" screen, the player / city and
   player / profession pickers (Menu\CHOOSEPLAYER, Menu\CHOOSEPROFESSION) of a network game, the player record
   0x6f5e20 .. 0x6f5e76 (name +0x12, family name +0x22, the city / emblem +0xc, gender +0x10 (a word with the
   religion at +0x11), profession +0xb, the chosen player 0x6f5e78) and the profile "Network" of 0x75e480.
   Unit start (layout 2026-10-23 session 21): the previous unit ends with "gamedata/screenshots/gilde%04i.bmp"
   0x62aa58 (0x589bc0), then the four globals 0x62aa7c .. 0x62aa88 (-1, 0, -1, -1) open this unit's .data block
   and its first literal "menu\netlobby" 0x62aa8c is used by 0x589f10 (data order D8 + code seam at the nop fill
   after 0x589f08; 0x589f10 is called from main.c 0x53bb20).
   The extent: the literals climb 0x62aa8c .. 0x62aad0 through 0x58ba70; the next block starts with the production
   window "Handel\Handel_PRODUKTION" 0x62b038 (0x58bd60), so 0x58bbd0 (the table init called by it) is the first
   function of the next unit.
   unit of rea 8cef7d5b (src/game/unk_589f10.c); boundaries in decl/text.dcl, notes in src/unk_589f10/open.md */
#include "d2_types.h"
extern D2WIN d2_Wins[];
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "unk_589f10.h"
#include "di_input.h"
#include "data_views.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern char ai_OptionsFile[288];
extern int ai_Unk1460ae0;
extern int ai_Unk5fe710;
extern int ai_ModeMask;
extern char ai_Unk9364e0[752];
extern char ch_EmptyName[1];
extern int d2_ClickedId;
extern struct D2INPUT *d2_FocusedInput;
extern D2INPUT d2_Inputs[];
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_60ea78[];
extern GAPDATA full_data_6230f8[];
extern GAPDATA full_data_623108[];
extern GAPDATA full_data_623110[];
extern GAPDATA full_data_6250e4[];
extern GAPDATA full_data_6250ec[];
extern GAPDATA full_data_6250f8[];
extern GAPDATA full_data_625100[];
extern GAPDATA full_data_62510c[];
extern GAPDATA full_data_629188[];
extern GAPDATA full_data_62aa7c[];
extern GAPDATA full_data_62aa80[];
extern GAPDATA full_data_62aa84[];
extern GAPDATA full_data_62aa88[];
extern GAPDATA full_data_62aa8c[];
extern GAPDATA full_data_62aa9c[];
extern GAPDATA full_data_6f267c[];
extern GAPDATA full_data_6f5e00[];
extern GAPDATA full_data_6f5e20[];
extern GAPDATA full_data_6f5e2a[];
extern GAPDATA full_data_6f5e2b[];
extern GAPDATA full_data_6f5e2c[];
extern GAPDATA full_data_6f5e30[];
extern GAPDATA full_data_6f5e31[];
extern GAPDATA full_data_6f5e32[];
extern GAPDATA full_data_6f5e42[];
extern GAPDATA full_data_6f5e52[];
extern GAPDATA full_data_6f5e56[];
extern GAPDATA full_data_6f5e57[];
extern GAPDATA full_data_6f5e58[];
extern GAPDATA full_data_6f5e59[];
extern GAPDATA full_data_6f5e5a[];
extern GAPDATA full_data_6f5e78[];
extern GAPDATA full_data_6f5e80[];
extern GAPDATA full_data_6f5ea0[];
extern GAPDATA full_data_6f5ea4[];
extern unsigned char init_PlayerProfile;
extern unsigned char kb_Last;
extern int sv_PopRequest;
extern int d2_LastWindow;



extern GAPDATA full_data_62aa7c[];
extern GAPDATA full_data_62aa80[];
extern GAPDATA full_data_62aa84[];
extern GAPDATA full_data_62aa88[];
extern int d2_LastWindow;
extern unsigned char ai_Unk936496;
extern struct D2OBJ *d2_Objs;

extern GAPDATA full_data_602568[];
extern GAPDATA full_data_6240cc[];
extern GAPDATA full_data_6240e0[];
extern GAPDATA full_data_62aa88[];
extern GAPDATA full_data_62aaa4[];
extern GAPDATA full_data_936476[];



extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;



extern GAPDATA full_data_602568[];
extern GAPDATA full_data_623594[];
extern GAPDATA full_data_62aa88[];
extern GAPDATA full_data_62aaac[];
extern GAPDATA full_data_6f267c[];
extern GAPDATA full_data_6f5e2b[];
extern GAPDATA full_data_6f5e2c[];
extern GAPDATA full_data_6f5e30[];
extern GAPDATA full_data_6f5e31[];
extern GAPDATA full_data_6f5e32[];
extern GAPDATA full_data_6f5e42[];
extern GAPDATA full_data_6f5ea0[];

extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;


extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62aa88[];
extern GAPDATA full_data_62aab4[];
extern GAPDATA full_data_6f267c[];
extern GAPDATA full_data_6f5e00[];
extern GAPDATA full_data_6f5e78[];
extern GAPDATA full_data_6f5e80[];
extern GAPDATA full_data_6f5ea0[];

extern int ai_Unk5fe710;
extern int d2_ClickedId;
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;
extern GAPDATA full_data_6250b8[];
extern unsigned char init_PlayerProfile;
extern int sv_PopRequest;

extern GAPDATA full_data_602568[];
extern GAPDATA full_data_62aa88[];
extern GAPDATA full_data_62aac0[];
extern GAPDATA full_data_62aad0[];
extern GAPDATA full_data_6f2680[];
extern GAPDATA full_data_6f2684[];
extern GAPDATA full_data_6f5e78[];
extern char ai_OptionsFile[288];
extern int d2_LastWindow;
extern struct D2OBJ *d2_Objs;




extern GAPDATA full_data_6230f8[];
extern GAPDATA full_data_623108[];
extern GAPDATA full_data_623110[];
extern GAPDATA full_data_6250d0[];
extern GAPDATA full_data_6250e4[];
extern GAPDATA full_data_6250ec[];
extern GAPDATA full_data_6250f8[];
extern GAPDATA full_data_625100[];
extern GAPDATA full_data_62510c[];
extern GAPDATA full_data_936476[];
extern unsigned char init_PlayerProfile;
extern unsigned char kb_Last;
extern int main_Unk5361d0(int mask, int flag);

#define U_I(a) (*(int *)(a))
#define U_B(a) (*(unsigned char *)(a))
#define U_W(a) (*(unsigned short *)(a))

void ai_Unk4cd500(); int * ai_Unk4cd600(); int d2_OpenForm(); void d2_CentreViewWindows(int view);
int io_PrintText(char *fmt, ...); int sv_RunFrame(int cb, unsigned mask); void d2_CloseView(int view); int gm_TitleGroup();
int gm_TitleNumber(); void ai_Unk4cd760(char *p); int cm_GetSlotState(); void sv_PumpNet();
void txt_Format(char *out, int id0, ...); int sv_MessageBox(); void ai_Unk4cd610(); void d2_ShowPicture(int pic);
char * ai_Unk4cd5f0(); int d2_SetForm(int view, int n); int d2_GetWinObj(); void d2_SetObjMax();
void d2_SetObjValue(); void d2_FocusInput(); int d2_GetObjValue(); void d2_EnableObject();
void ai_Unk4cd6e0(char *text, int v); int ai_Unk4cd510(); void ai_Unk4cd6a0(); void ai_Unk4cd650(int v);
void ai_Unk4cd7c0(char *p); char * ai_Unk4cd380(int id); void d3io_NoopCallback(); int sprintf(char *buf, const char *fmt, ...);
int cm_GetSlotValue(int h); 
int ai_Unk58af50(); int ai_Unk58b880();

