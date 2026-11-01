/* unk_589f10.c -- the network lobby (0x589f10 .. 0x58bbd0): the "menu\netlobby" screen, the player / city and
   player / profession pickers (Menu\CHOOSEPLAYER, Menu\CHOOSEPROFESSION) of a network game, the player record
   0x6f5e20 .. 0x6f5e76 (name +0x12, family name +0x22, the city / emblem +0xc, gender +0x10 (a word with the
   religion at +0x11), profession +0xb, the chosen player 0x6f5e78) and the profile "Network" of 0x75e480.
   Name unknown (@named_by -). Start: the previous unit ends with "gamedata/screenshots/gilde%04i.bmp" 0x62aa58,
   then the globals 0x62aa7c .. 0x62aa88 open this unit's .data block, whose first literal "menu\netlobby" is used
   by 0x589f10 (code seam at the nop fill 0x589f08). End: the literals climb through 0x58ba70; the next block
   starts with "Handel\Handel_PRODUKTION" 0x62b038 (0x58bd60), so its table init 0x58bbd0 opens the next unit.
   Open: the control API (0x4731d0, 0x473650, 0x52e350, 0x52e860) is unnamed and the player record untyped,
   which keeps every function unreadable.
   unit of rea 8cef7d5b (src/game/unk_589f10.c); boundaries in decl/text.dcl */
#include "d2_types.h"
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "d3.h"
#include "d2.h"
#include "cm.h"                           /* the lobby API, CMNETGAME cm_NetGame, CMLOBBYPLAYER */
#include "sv.h"
#include "f3gm.h"
#include "io_text.h"
#include "unk_589f10.h"
#include "di_input.h"
#include "data_views.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern char ai_OptionsFile[288];
extern int ai_Unk1460ae0;
extern int ai_Unk5fe710;
extern D2INPUT d2_Inputs[];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_6230f8[];
extern GAPDATA full_data_623108[];
extern GAPDATA full_data_623110[];
extern GAPDATA full_data_623594[];
extern GAPDATA full_data_6240cc[];
extern GAPDATA full_data_6240e0[];
extern GAPDATA full_data_6250b8[];
extern GAPDATA full_data_6250d0[];
extern GAPDATA full_data_6250e4[];
extern GAPDATA full_data_6250ec[];
extern GAPDATA full_data_6250f8[];
extern GAPDATA full_data_625100[];
extern GAPDATA full_data_62510c[];
extern GAPDATA full_data_60ea78[];
extern GAPDATA full_data_629188[];
extern GAPDATA full_data_62aaa4[];
extern GAPDATA full_data_62aaac[];
extern GAPDATA full_data_62aab4[];
extern GAPDATA full_data_62aac0[];
extern GAPDATA full_data_62aad0[];
extern GAPDATA full_data_6f2680[];
extern GAPDATA full_data_6f2684[];
#include "f3init_types.h"
#include "f3init.h"
extern unsigned char kb_Last;
extern int sv_PopRequest;
extern int d2_LastWindow;
extern INIT_PLAYER init_PlayerProfile;
extern int main_ChoosePlayer(int mask, int flag);

#define U_I(a) (*(int *)(a))
#define U_B(a) (*(unsigned char *)(a))
#define U_W(a) (*(unsigned short *)(a))

void d2_FocusInput(); int sprintf(char *buf, const char *fmt, ...);
int ai_Unk58af50(); int ai_Unk58b880();

int lobby_Buttons[2] = { -1 };  /* 0x62aa7c, the controls of the lobby form's button row 4 (sv_ButtonRow): [0] ready, [1] the character (the player and profession pickers) */
int lobby_LeaveButton = -1;  /* 0x62aa84, the control of the button row 8: leave the lobby */
int lobby_Form = -1;  /* 0x62aa88, the lobby form "menu\netlobby" (d2_OpenForm), -1: not open */
unsigned int *lobby_List = 0;  /* 0x6f267c, cm's player list (ai_Unk4cd5d0): the count, then the eight player ids */
int lobby_ChatTo = 0;  /* 0x6f5e00, the id of the player the chat lines go to (the selected list line), -1: everybody */
CMLOBBYPLAYER lobby_Player = {0};  /* 0x6f5e20, this player's record as sent to the lobby (join / change) */
CMLOBBYPLAYER *lobby_Me = 0;  /* 0x6f5e78, this player's record in cm's table (ai_Unk4cd5f0) */
int lobby_Rows[8] = {0};  /* 0x6f5e80, the text control of each line of the player list */
int lobby_MyRow = 0;  /* 0x6f5ea0, the player list line of this player */
int *lobby_State = 0;  /* 0x6f5ea4, cm's lobby state (ai_Unk4cd600): [1] a lobby message came in */

/* 0x589f10, the network lobby screen (2595 B, ROUGH): clears the player record 0x6f5e20 (0x57 B), reads the
   "Network" profile (name / family name / emblem / gender / religion / profession) into it, opens the lobby
   screen "menu\netlobby" 0x62aa8c, builds the four sub windows (0x58a9d0 / 0x58a940 / 0x58aa30 / 0x58ad20 /
   0x58aba0 / 0x58ada0) and runs its frame loop 0x5309a0 (the callback 0x589f10, mask 0x24087): the input key
   0x1468410 (0x1c = enter) toggles the active text field, the clicked control 0x5fe710 is compared with the
   window ids 0x62aa7c / 0x62aa80 / 0x62aa84 (toggle the selected player, the city and the profession pickers)
   and with the player list 0x6f5e80; the engine state 0x1460ae0 == -1 ends the screen (returns -1); the
   player's choice is written back to the profile ("%d" 0x629188, 0x5de652) and the record 0x75e5a0 ..
   0x75e5ca keeps the last choice. The paths where the record 0x6f5ea4 / 0x6f5e78 is missing open the message
   box 0x60ea78 instead. ROUGH: the text of the sub blocks is transcribed from the disassembly / pseudocode,
   the exact store order and the local frame are not verified */

int ai_Unk589f10(void)
{
    char buf[256];
    char text[256];
    char name[72];
    int ret;
    int state;
    int win;
    int id;
    int i;
    int n;
    char *p;
    char *r;

    ret = 0;
    ai_Unk4cd500();
    memset(&lobby_Player, 0, 0x57);
    lobby_List = 0;
    lobby_ChatTo = -1;
    lobby_State = ai_Unk4cd600();
    if (lobby_State == 0) {
        win = d2_OpenForm(0, 0, (char *)(unsigned)"misc\\Messagebox_NONE_PERGA");
        d2_CentreViewWindows(win);
        io_PrintText(0x87);
        U_I((unsigned)&d2_ClickedId) = -1;
        while (U_I(((unsigned)&dm_L + 0x20)) == 0 && U_B((unsigned)&kb_Last) != 1) {
            if (sv_RunFrame((int)ai_Unk589f10, 0x24087) == 0) {
                d2_CloseView(win);
                return 0;
            }
        }
        U_I((unsigned)&sv_PopRequest) = 1;
        d2_CloseView(win);
        return 0;
    }
    GetPrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Name", (char *)(unsigned)"Vorname", lobby_Player.firstName, 0x20,
                             (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Familienname", (char *)(unsigned)"Nachname", lobby_Player.familyName, 0x20,
                             (char *)(unsigned)&ai_OptionsFile);
    lobby_Player.coat = GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Wappen", 0, (char *)(unsigned)&ai_OptionsFile);
    lobby_Player.sex = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Geschlecht", 0, (char *)(unsigned)&ai_OptionsFile);
    lobby_Player.faith = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Glauben", 0, (char *)(unsigned)&ai_OptionsFile);
    lobby_Player.job = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", "Beruf", 0, (char *)(unsigned)&ai_OptionsFile);
    lobby_Player.job = gm_TitleNumber(gm_TitleGroup(lobby_Player.job), 1);
    U_B(((unsigned)&init_PlayerProfile + 0x2)) = 0x10;
    lobby_Player.status = 0x10;
    lobby_Player.talents[1] = 0x46;
    lobby_Player.talents[3] = 0x46;
    lobby_Player.unk32 = -1;
    lobby_Player.talents[0] = 0x5a;
    lobby_Player.talents[2] = 0x78;
    lobby_Player.talents[4] = 0x5a;
    if (cm_NetGame.mode == 0x7f) {
        lobby_Player.job = U_B(((unsigned)&ai_Unk9364e0 + 0x2e0));
        lobby_Player.sex = U_B(((unsigned)&ai_Unk9364e0 + 0x2e4));
        lobby_Player.coat = U_I(((unsigned)&ai_Unk9364e0 + 0x2ec)) - 0x59a;
        lobby_Player.faith = U_B(((unsigned)&ai_Unk9364e0 + 0x2e8));
        strcpy(lobby_Player.firstName, (char *)((unsigned)&ai_Unk9364e0 + 0x2c0));
        strcpy(lobby_Player.familyName, (char *)((unsigned)&ai_Unk9364e0 + 0x2d0));
    }
    id = ai_Unk4cd760(&lobby_Player);
    if (id == -1)
        return -1;
    while (cm_GetSlotState(id) == 0)
        sv_PumpNet();
    if (cm_GetSlotState(id) == 2) {
        txt_Format(buf, 0x88);
        sv_MessageBox(buf, 0x100);
        return -1;
    }
    id = ((int (*)())ai_Unk4cd610)();
    if (id == -1)
        return -1;
    while (cm_GetSlotState(id) == 0)
        sv_PumpNet();
    d2_ShowPicture(0x7d6);
    lobby_Me = ai_Unk4cd5f0();
    if (lobby_Me == 0) {
        win = d2_OpenForm(0, 0, (char *)(unsigned)"misc\\Messagebox_NONE_PERGA");
        d2_CentreViewWindows(win);
        io_PrintText(0x87);
        U_I((unsigned)&d2_ClickedId) = -1;
        while (U_I(((unsigned)&dm_L + 0x20)) == 0 && U_B((unsigned)&kb_Last) != 1) {
            if (sv_RunFrame((int)ai_Unk589f10, 0x24087) == 0) {
                d2_CloseView(win);
                return 0;
            }
        }
        U_I((unsigned)&sv_PopRequest) = 1;
        d2_CloseView(win);
        return 0;
    }
    lobby_Form = d2_OpenForm(0x3c, 0, "menu\netlobby");
    d2_CentreViewWindows(lobby_Form);
    ai_Unk58a9d0();
    ai_Unk58a940();
    ai_Unk58aa30();
    ai_Unk58ad20();
    ai_Unk58aba0();
    ai_Unk58ada0();
    d2_SetForm(lobby_Form, 1);
    id = d2_GetWinObj(lobby_Form, 0, (char *)0);
    d2_SetObjMax(id, 0x40);
    state = 0;
    d2_SetObjValue(id, (char *)(unsigned)&ch_EmptyName);
    i = id * 0x2ec;
    *(short *)(i + 0x14 + U_I((unsigned)&d2_Objs)) = 0x26c;
    n = *(int *)(i + 0x78 + U_I((unsigned)&d2_Objs));
    d2_Inputs[n].flags |= 0x200;
    d2_Inputs[n].w = 0x26c;
    U_I((unsigned)&d2_ClickedId) = -1;
    d2_Inputs[n].maxw = 0x26c;
    while (1) {
        if (state == 0) {
            if (U_I((unsigned)&d2_FocusedInput) == 0) {
                if (U_B((unsigned)&kb_Last) == 0x1c) {
                    d2_FocusInput(id);
                    state = 1;
                }
            } else {
                state = 1;
            }
        } else if (U_I((unsigned)&d2_FocusedInput) == 0 || U_B((unsigned)&kb_Last) == 0x1c) {
            state = 0;
            p = (char *)d2_GetObjValue(id);
            strcpy(name, p);
            d2_SetObjValue(id, (char *)(unsigned)&ch_EmptyName);
            if (strlen(name) != 0)
                ai_Unk4cd6e0(lobby_ChatTo == -1 ? 0 : lobby_ChatTo, name);
        }
        if (U_I((unsigned)&ai_Unk1460ae0) == -1) {
            ret = -1;
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (ai_Unk4cd510() == 0) {
            if (U_I((unsigned)&d2_ClickedId) != -1) {
                if (U_I((unsigned)&ai_Unk5fe710) == lobby_LeaveButton) {
                    n = ((int (*)())ai_Unk4cd6a0)();
                    while (cm_GetSlotState(n) == 0)
                        sv_PumpNet();
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = -1;
                } else if (U_I((unsigned)&ai_Unk5fe710) == lobby_Buttons[0]) {
                    n = ((int (*)(int))ai_Unk4cd650)(lobby_Me->ready == 0);
                    while (cm_GetSlotState(n) == 0)
                        sv_PumpNet();
                    U_I((unsigned)&d2_Objs + 0x28 + lobby_Buttons[0] * 0x2ec) = lobby_Me->ready;
                    if (ai_Unk4cd510() != 0)
                        U_I((unsigned)&sv_PopRequest) = 1;
                } else if (U_I((unsigned)&ai_Unk5fe710) == lobby_Buttons[1]) {
                    if (lobby_Me->ready != 0) {
                        n = ((int (*)(int))ai_Unk4cd650)(0);
                        while (cm_GetSlotState(n) == 0)
                            sv_PumpNet();
                        U_I((unsigned)&d2_Objs + 0x28 + lobby_Buttons[0] * 0x2ec) = lobby_Me->ready;
                    }
                    if (lobby_Me->ready == 0 && ai_Unk58af50(0x24087, 1) != 0) {
                        strcpy(lobby_Player.firstName, (char *)((unsigned)&init_PlayerProfile + 0xa));
                        strcpy(lobby_Player.familyName, (char *)((unsigned)&init_PlayerProfile + 0x2a));
                        lobby_Player.coat = U_I(((unsigned)&init_PlayerProfile + 0x4)) - 0x59a;
                        lobby_Player.sex = U_B(((unsigned)&init_PlayerProfile + 0x8));
                        lobby_Player.faith = U_B(((unsigned)&init_PlayerProfile + 0x9));
                        n = ai_Unk4cd7c0(&lobby_Player);
                        while (cm_GetSlotState(n) == 0)
                            sv_PumpNet();
                        if (ai_Unk58b880(0x24087) != 0) {
                            lobby_Player.job = U_B(((unsigned)&init_PlayerProfile + 0x3));
                            n = ai_Unk4cd7c0(&lobby_Player);
                            while (cm_GetSlotState(n) == 0)
                                sv_PumpNet();
                        }
                    }
                } else {
                    /* the player list: the clicked entry 0x6f5e80[i] is the only one selected, the id
                       0x6f267c[i + 1] goes to 0x6f5e00 */
                    for (i = 0; i < lobby_List[0]; i++) {
                        if (i != lobby_MyRow && U_I((unsigned)&ai_Unk5fe710) == lobby_Rows[i]) {
                            n = U_I((unsigned)&d2_Objs + 0x28 + lobby_Rows[i] * 0x2ec) == 0;
                            U_I((unsigned)&d2_Objs + 0x28 + lobby_Rows[i] * 0x2ec) = n;
                            if (n == 0) {
                                lobby_ChatTo = -1;
                            } else {
                                lobby_ChatTo = lobby_List[i + 1];
                            }
                            if (-1 < i) {
                                for (n = 0; n < lobby_List[0]; n++)
                                    if (i != n)
                                        U_I((unsigned)&d2_Objs + 0x28 + lobby_Rows[n] * 0x2ec) = 0;
                            }
                            break;
                        }
                    }
                }
            }
        } else {
            U_I((unsigned)&sv_PopRequest) = 1;
        }
        U_I((unsigned)&d2_ClickedId) = -1;
        if (lobby_State[1] != 0) {
            if (ai_Unk4cd510() == 0) {
                n = ((int (*)())ai_Unk4cd610)();
                while (cm_GetSlotState(n) == 0)
                    sv_PumpNet();
                lobby_List = ai_Unk4cd5d0();
                ai_Unk58aba0();
                ai_Unk58ada0();
                ai_Unk58ba70();
                lobby_State[1] = 0;
            } else {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame((int)ai_Unk589f10, 0x24087) == 0) {
            d2_CloseView(lobby_Form);
            r = (char *)lobby_Me;
            U_B(((unsigned)&init_PlayerProfile + 0x2)) = 0x12;
            U_B(((unsigned)&init_PlayerProfile + 0x3)) = U_B(r + 0xb);
            U_W(((unsigned)&init_PlayerProfile + 0x8)) = U_W(r + 0x10);
            U_I(((unsigned)&init_PlayerProfile + 0x4)) = U_I(r + 0xc);
            strcpy((char *)((unsigned)&init_PlayerProfile + 0x2a), r + 0x22);
            strcpy((char *)((unsigned)&init_PlayerProfile + 0xa), r + 0x12);
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Name", (char *)((unsigned)&init_PlayerProfile + 0xa),
                                       (char *)(unsigned)&ai_OptionsFile);
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Familienname", (char *)((unsigned)&init_PlayerProfile + 0x2a),
                                       (char *)(unsigned)&ai_OptionsFile);
            sprintf(text, (char *)(unsigned)"%d", U_I(((unsigned)&init_PlayerProfile + 0x4)));
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Wappen", text, (char *)(unsigned)&ai_OptionsFile);
            sprintf(text, (char *)(unsigned)"%d", U_B(((unsigned)&init_PlayerProfile + 0x8)));
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Geschlecht", text, (char *)(unsigned)&ai_OptionsFile);
            sprintf(text, (char *)(unsigned)"%d", U_B(((unsigned)&init_PlayerProfile + 0x9)));
            WritePrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Glauben", text, (char *)(unsigned)&ai_OptionsFile);
            sprintf(text, (char *)(unsigned)"%d", U_B(((unsigned)&init_PlayerProfile + 0x3)));
            WritePrivateProfileStringA((char *)(unsigned)"Network", "Beruf", text, (char *)(unsigned)&ai_OptionsFile);
            ((void (*)(void))d3io_NoopCallback)();
            return ret;
        }
    }
}

char *_itoa(int, char *, int); char *_strupr(char *);

/* 0x58a940, adds the lobby's buttons (144 B): control 4 of the window 0x62aa88 and 0x52e350 (screen 0x6ed938,
   0, the id 0x62aa7c, 2, U_I (0x144abe8), U_I (0x144abec)), control 8 and 0x52e350 (.., the id 0x62aa84, 1,
   U_I (0x144abf0)); in the single-player mode 0x9364dc 0x7f the control 0x62aa80 is disabled (the window table
   0xf8f080 + id * 0x2ec, +0x3c = 1)
   @deep byte but not readable: Q1 (unit unnamed, @named_by -), Q2 (d2_SetForm control API, sv_ButtonRow), Q3 (10 lobby globals), Q4 U_I; the whole lobby/control API has to be named and the globals typed first; no Q6 fix exists */
void ai_Unk58a940(void)
{
    d2_SetForm(lobby_Form, 4);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0, lobby_Buttons, 2, U_I((unsigned)&full_data_144abe8), U_I((unsigned)&full_data_144abec));
    d2_SetForm(lobby_Form, 8);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0, &lobby_LeaveButton, 1, U_I((unsigned)&full_data_144abf0));
    if (cm_NetGame.mode == 0x7f)
        U_I(U_I((unsigned)&d2_Objs) + 0x3c + lobby_Buttons[1] * 0x2ec) = 1;
}
/* 0x58a9d0, the lobby's control 5 / 6 hook (96 B): control 5 of 0x62aa88, the server query 0x52e860 (0, 0, the
   screen 0x6ed938, U_I (0x6ed938), 0x35, three out ints) with control 6
   @deep byte but not readable: Q1 (unit unnamed, @named_by -), Q2 (d2_SetForm, sv_CreateFormSlider), Q3, Q4 U_I; same lobby/control API naming debt as 0x58a940 */

void ai_Unk58a9d0(void)
{
    int a;
    int b;
    int c;
    int s;

    if (lobby_Form != -1) {
        d2_SetForm(lobby_Form, 5);
        s = U_I((unsigned)&d2_LastWindow);
        d2_SetForm(lobby_Form, 6);
        sv_CreateFormSlider(0, 0, U_I((unsigned)&d2_LastWindow), s, 0x35, &a, &b, &c);
    }
}
/* 0x58aa30, the city emblem window (368 B): control 2 of 0x62aa88, the text 0x602568; the city name 0x936476
   upcased (0x5ea986) into "..\..\Grafik\STADTWAPPEN\.." 0x6240e0 via 0x5de652, the image 0x468890 (the fallback
   "..\..\Grafik\STADTWAPPEN\BERLIN" 0x6240cc); the control 0x4731d0 (5, 5, the image, screen) and the text
   0x473650 (the control's x + 0xf, 0x19, the city name) and below it the population "%s: %d" 0x62aaa4 with
   U_I (0x144abd4) and the byte 0x936496 */

void ai_Unk58aa30(void)
{
    char a[256];
    char b[256];
    char c[256];
    int id;
    int x;
    int y;

    d2_SetForm(lobby_Form, 2);
    io_PrintText((char *)(unsigned)"$C");
    strcpy(a, cm_NetGame.hdr.name);
    strcpy(b, cm_NetGame.hdr.name);
    _strupr(a);
    sprintf(c, (char *)(unsigned)"_STADTWAPPEN_%s", a);
    id = d2_FindPic(c);
    if (id == -1)
        id = d2_FindPic((char *)(unsigned)"_STADTWAPPEN_BERLIN");
    id = d2_WinOpenPic(5, 5, id, U_I((unsigned)&d2_LastWindow));
    x = *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) + 0xf;
    y = d2_CreateWinText(x, 0x19, b, U_I((unsigned)&d2_LastWindow));
    sprintf(a, (char *)(unsigned)&full_data_62aaa4, U_I((unsigned)&full_data_144abd4), cm_NetGame.hdr.players);
    d2_CreateWinText(x, *(short *)(U_I((unsigned)&d2_Objs) + 0x16 + y * 0x2ec) + 0x1b, a, U_I((unsigned)&d2_LastWindow));
}
/* 0x58aba0, the current player's info window (384 B): the player record 0x6f267c[0x6f5ea0 + 1] (0x4cd380)
   gives the city (+0xc); control 3 of 0x62aa88 and the text 0x602568; the city image control 0x4731d0
   (5, 10, city + 0x59a) and the name / family name text 0x473650 (x + 0xf, 10); then "%s: %s" 0x62aaac
   with the honour title of the gender (U_I (0x143a614), U_I (0x143a63c) for the female byte 0x6f5e30) and
   the profession (the tables 0x143a7fc / 0x143a680 by 0x6f5e2b) and the religion (0x143b3d8 by 0x6f5e31),
   each below the last
   @deep Q1 (unit unnamed, @named_by -); B: code 117 + slots 12 + fill 3; Q2 (6 callees incl. 0x4731d0/0x473650/0x5de652), Q3 (21 addresses), Q4 U_I/U_B + 2 casts; the same missing control API and typed player record */

void ai_Unk58aba0(void)
{
    char text[256];
    char *u;
    int p;
    int id;
    int x;
    int y;

    p = ai_Unk4cd380(lobby_List[lobby_MyRow + 1]);
    if (p != 0)
        lobby_Player.coat = U_I(p + 0xc);
    d2_SetForm(lobby_Form, 3);
    io_PrintText((char *)(unsigned)"$C");
    id = d2_WinOpenPic(5, 10, lobby_Player.coat + 0x59a, U_I((unsigned)&d2_LastWindow));
    x = *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + id * 0x2ec) + 0xf;
    u = (char *)U_I(((unsigned)&ai_Unk143a610 + 0x4));
    if (lobby_Player.sex != 0)
        u = (char *)U_I((unsigned)&full_data_143a63c);
    sprintf(text, (char *)(unsigned)"%s %s %s", u, lobby_Player.firstName, lobby_Player.familyName);
    id = d2_CreateWinText(x, 10, text, U_I((unsigned)&d2_LastWindow));
    y = *(short *)(U_I((unsigned)&d2_Objs) + 0x16 + id * 0x2ec);
    if (lobby_Player.sex == 1)
        u = (char *)U_I(((unsigned)&ai_Unk143a680 + 0x17c) + lobby_Player.job * 4);
    else
        u = (char *)U_I((unsigned)&ai_Unk143a680 + lobby_Player.job * 4);
    sprintf(text, (char *)(unsigned)&full_data_62aaac, U_I((unsigned)&full_data_144abe0), u);
    d2_CreateWinText(x, y + 0xc, text, U_I((unsigned)&d2_LastWindow));
    sprintf(text, (char *)(unsigned)&full_data_62aaac, U_I((unsigned)&full_data_144abe4), U_I((unsigned)&ai_Unk143b3d8 + lobby_Player.faith * 4));
    d2_CreateWinText(x, y + 2 + y + 0xc, text, U_I((unsigned)&d2_LastWindow));
}
/* 0x58ad20, waits for the player list (128 B): 0x4cd610 and its answer 0x4c3ae0; the list 0x4cd5d0 -> 0x6f267c;
   the entry 0x6f267c[1 .. 8] with a player (0x4cd820) is awaited (0x4c3ae0)
   @deep Q1 (unit unnamed, @named_by -); B: code 28 (the entry scan of the player list 0x6f267c, 0x4cd820); Q2 (5 callees), Q3 (0x6f267c), Q4 U_I; needs the player-list API and the record typed */

void ai_Unk58ad20(void)
{
    int h;
    int r;
    int i;

    h = ((int (*)())ai_Unk4cd610)();
    while (cm_GetSlotState(h) == 0)
        sv_PumpNet();
    lobby_List = ai_Unk4cd5d0();
    r = -1;
    for (i = 4; i < 0x24; i += 4)
        if (U_I((char *)lobby_List + i) != 0)
            r = ((int (*)(int))ai_Unk4cd820)(U_I((char *)lobby_List + i));
    if (r != -1)
        while (cm_GetSlotState(r) == 0)
            sv_PumpNet();
}
/* 0x58ada0, the player list window (432 B): control 1 of 0x62aa88 and the text 0x602568; the eight player
   records 0x6f267c[i + 1] (0x4cd380): the city image 0x4731d0 (5, y, the city + 0x59a) and the line
   0x473650 (x + 10, y, "%s %s %s " 0x62aab4 of the honour title, the name +0x12 and the family name +0x22,
   plus the status text 0x144abf4 when +4 is set); the control id into 0x6f5e80[i], y += 5 + the control
   height; the chosen player 0x6f5e78 gives 0x6f5ea0 = i, the others are marked selectable (+0x4c = 1,
   +0x1c0 = 3); the record id equal 0x6f5e00 marks the control (+0x28 = 1)
   @deep Q1 (unit unnamed, @named_by -); B: code 117 + fill 3 + slots 2 (the original's strcat is a rep movs hand loop, note Open work); Q2 (6 callees), Q3 (13 addresses), Q4 U_I + 2 casts */

void ai_Unk58ada0(void)
{
    char text[256];
    char *u;
    int *p;
    int y;
    int i;
    int id;
    int n;

    y = 10;
    d2_SetForm(lobby_Form, 1);
    io_PrintText((char *)(unsigned)"$C");
    for (i = 0; i < 8; i++) {
        p = (int *)ai_Unk4cd380(lobby_List[i + 1]);
        if (p == 0)
            continue;
        id = d2_WinOpenPic(5, y, p[3] + 0x59a, U_I((unsigned)&d2_LastWindow));
        u = (char *)U_I(((unsigned)&ai_Unk143a610 + 0x4));
        if ((char)p[4] != 0)
            u = (char *)U_I((unsigned)&full_data_143a63c);
        sprintf(text, (char *)(unsigned)&full_data_62aab4, u, (char *)p + 0x12, (char *)p + 0x22);
        if (p[1] != 0)
            strcat(text, (char *)U_I((unsigned)&full_data_144abf4));
        n = id * 0x2ec;
        d2_CreateWinText(*(short *)(n + 0x14 + U_I((unsigned)&d2_Objs)) + 10, y, text, U_I((unsigned)&d2_LastWindow));
        y = y + 5 + *(short *)(n + 0x16 + U_I((unsigned)&d2_Objs));
        lobby_Rows[i] = id;
        if (p == (int *)lobby_Me) {
            lobby_MyRow = i;
        } else {
            U_I(n + 0x4c + U_I((unsigned)&d2_Objs)) = 1;
            U_I(n + 0x1c0 + U_I((unsigned)&d2_Objs)) = 3;
        }
        if (lobby_ChatTo == *p)
            U_I(n + 0x28 + U_I((unsigned)&d2_Objs)) = 1;
    }
}


/* 0x58b880, the choose-profession window (496 B): the window "Menu\nCHOOSEPROFESSION" 0x6250b8 (0x4741f0 /
   0x475600), control 0 and the text 0x1afa; the eleven profession buttons (the kinds 1 .. 7, 0xb, 0xd .. 0xf)
   at (i % 3) * 0x60 + 100, ((i / 3) * 5 + 5) * 0x10 through 0x4731d0 (the id i + 0x5a1), their window entry
   0xf8f080 + id * 0x2ec (+0x4c = 1, +0x1c0 = 3), the text +0xbc = the profession name 0x143a978[i], +0x1bc =
   0x56 and 0x46c560 (the object list); the frame loop 0x5309a0 (0x58b880, mask) with 0x46c6c0, the clicked
   control 0x5fe710 against the ids -> 0x5043d0 (the profession, 1) into 0x75e5a3, 0x6f22a0 = 1, return 1
   @deep Q1 (unit unnamed, @named_by -); B: code 70 + fill 3 + regs 2 (the choose-profession window); Q2 (12 callees), Q3 (8 addresses), Q4 U_I/U_B */
int ai_Unk58b880(int param)
{
    int ids[22];
    int win;
    int obj;
    int ret;
    int i;
    int id;
    int n;

    ret = 0;
    ids[0] = 1;
    ids[1] = 2;
    ids[2] = 3;
    ids[3] = 4;
    ids[4] = 5;
    ids[5] = 6;
    ids[6] = 7;
    ids[7] = 0xb;
    ids[8] = 0xd;
    ids[9] = 0xe;
    ids[10] = 0xf;
    win = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_6250b8);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    io_PrintText(0x1afa);
    obj = d2_CreateRadioGroup(0);
    for (i = 0; i < 0xb; i++) {
        id = d2_WinOpenPic((i % 3) * 0x60 + 100, ((i / 3) * 5 + 5) * 0x10, ids[i] + 0x5a1, U_I((unsigned)&d2_LastWindow));
        ids[i + 0xb] = id;
        n = id * 0x2ec;
        U_I(n + 0x4c + U_I((unsigned)&d2_Objs)) = 1;
        U_I(n + 0x1c0 + U_I((unsigned)&d2_Objs)) = 3;
        strcpy((char *)(n + 0xbc + U_I((unsigned)&d2_Objs)), (char *)U_I(ids[i] * 4 + ((unsigned)&ai_Unk143a680 + 0x2f8)));
        U_I(n + 0x1bc + U_I((unsigned)&d2_Objs)) = 0x56;
        d2_RadioAdd(obj, id);
    }
    U_I((unsigned)&d2_ClickedId) = -1;
    n = sv_RunFrame((int)ai_Unk58b880, param);
    while (1) {
        if (n == 0) {
            d2_RadioClear(obj);
            d2_CloseView(win);
            return ret;
        }
        d2_MenuNavigate(obj);
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            for (i = 0; i < 0xb; i++) {
                if (U_I((unsigned)&ai_Unk5fe710) == ids[i + 0xb]) {
                    U_B(((unsigned)&init_PlayerProfile + 0x3)) = gm_TitleNumber(ids[i], 1);
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = 1;
                    break;
                }
            }
        }
        n = sv_RunFrame((int)ai_Unk58b880, param);
    }
}
/* 0x58ba70, the chat lines of the lobby (352 B): control 5 of 0x62aa88 and the text 0x602568; the chat records
   from 0x4cd480 / 0x4cd4c0 (0x6f2684 counts them): the speaker 0x4cd380 (the record +0x68), its id (+0xc) or 9;
   the line "$%dFF%s: %s$A" 0x62aac0 (the name +0x40, the text) for a record without +100, or
   "$%dFF%s (%s): %s$A" 0x62aad0 with the shout text U_I (0x144abf8) when +100 is the chosen player 0x6f5e78;
   each line through 0x515960; the newest line count grows the chat box (0x6f2684 > 0x6f2680 and the control
   height 0xf8f12a < the box) -> 0xf8f364 = +0x18, 0xf8f368 = the delta, 0x46f840 (the screen)
   @deep Q1 (unit unnamed, @named_by -); B: code 151 + regs 14 (the chat lines); Q2 (7 callees), Q3 (12 addresses), Q4 U_I + 2 casts */

void ai_Unk58ba70(void)
{
    char text[352];
    int p;
    int q;
    int id;
    int top;
    int n;

    id = -1;
    p = ai_Unk4cd480();
    d2_SetForm(lobby_Form, 5);
    q = U_I(((unsigned)&ai_Unkf8f13c + 0x228) + U_I((unsigned)&d2_LastWindow) * 0x3c4);
    io_PrintText((char *)(unsigned)"$C");
    U_I((unsigned)&full_data_6f2684) = 0;
    while (p != 0) {
        U_I((unsigned)&full_data_6f2684)++;
        n = ai_Unk4cd380(U_I(p + 0x68));
        if (n == 0 || U_I(n) != U_I(p + 0x68))
            id = 9;
        else
            id = U_I(n + 0xc);
        if (U_I(p + 100) == 0) {
            sprintf(text, (char *)(unsigned)&full_data_62aac0, id, (char *)(p + 0x40), (char *)p);
        } else if (((int)lobby_Me != 0 && U_I(p + 100) == (int)lobby_Me) ||
                   U_I(p + 0x68) == (int)lobby_Me) {
            sprintf(text, (char *)(unsigned)&full_data_62aad0, id, (char *)(p + 0x40), U_I((unsigned)&full_data_144abf8), (char *)p);
        }
        id = io_PrintText(text);
        p = ai_Unk4cd4c0();
    }
    if (U_I((unsigned)&full_data_6f2680) < U_I((unsigned)&full_data_6f2684) && -1 < id &&
        *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + U_I((unsigned)&d2_LastWindow) * 0x3c4) < q) {
        q = q + 0x18;
        U_I((unsigned)&full_data_6f2680) = U_I((unsigned)&full_data_6f2684);
        U_I(((unsigned)&ai_Unkf8f13c + 0x228) + U_I((unsigned)&d2_LastWindow) * 0x3c4) = q;
        U_I(((unsigned)&ai_Unkf8f13c + 0x22c) + U_I((unsigned)&d2_LastWindow) * 0x3c4) = q - *(short *)(((unsigned)&ai_Unkf8f128 + 0x2) + U_I((unsigned)&d2_LastWindow) * 0x3c4);
        d2_ScrollWindow(U_I((unsigned)&d2_LastWindow));
    }
}
/* 0x58af50, the choose-player window (2352 B, ROUGH SKELETON): reads the profile "Network" (the name into
   0x75e5aa, the family name into 0x75e5ca, the emblem 0x75e5a4 = the profile int + 0x59a, the gender word
   0x75e5a8), the window "Menu\nCHOOSEPLAYER" 0x6250d0 (0x4741f0 / 0x475600), the city name 0x5142e0
   (0x936476) and the controls 1 .. 6 (0x475b80, 0x473650 x2, 0x52e350 x2, 0x515960, 0x475cb0 x2); the eight
   emblem buttons 0x4731d0 ((i % n) * 0x30 + 0x50, (i / n) * 0x30 + 0x20, i + 0x59a) with their window entries
   (+0x48 = 1, +0x28 = 0, +0x2c = 0, +0x50 = 1, +0x1c0 = 3) and 0x46c560; the chosen emblem / gender
   highlighted (0x46c630); the state machine 0 .. 5 over the two text fields, the two gender buttons and the
   eight emblems; the frame loop 0x5309a0 (0x5361d0, mask) with the profile write back and the return 1 when a
   player was chosen. ROUGH: the state machine and the profile write back are condensed to the calls; see the
   disassembly 0x58af50 .. 0x58b8xx */

int ai_Unk58af50(int a, int b)
{
    char name[48];
    char buf[256];
    int btns[8];
    int win;
    int obj;
    int ret;
    int i;
    int id;
    int g1;
    int g2;
    int n;

    ret = 0;
    GetPrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Name", (char *)(unsigned)"Vorname", (char *)((unsigned)&init_PlayerProfile + 0xa), 0x20,
                             (char *)(unsigned)&ai_OptionsFile);
    GetPrivateProfileStringA((char *)(unsigned)"Network", (char *)(unsigned)"Familienname", (char *)(unsigned)"Nachname", (char *)((unsigned)&init_PlayerProfile + 0x2a), 0x20,
                             (char *)(unsigned)&ai_OptionsFile);
    U_I(((unsigned)&init_PlayerProfile + 0x4)) = GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Wappen", 0, (char *)(unsigned)&ai_OptionsFile) + 0x59a;
    U_B(((unsigned)&init_PlayerProfile + 0x8)) = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Geschlecht", 0, (char *)(unsigned)&ai_OptionsFile);
    U_B(((unsigned)&init_PlayerProfile + 0x9)) = (char)GetPrivateProfileIntA((char *)(unsigned)"Network", (char *)(unsigned)"Glauben", 0, (char *)(unsigned)&ai_OptionsFile);
    win = d2_OpenForm(0, 0, (char *)(unsigned)"Menu\\CHOOSEPLAYER");
    d2_CentreViewWindows(win);
    io_TownDisplayName(cm_NetGame.hdr.name, buf);
    d2_SetForm(win, 1);
    io_PrintText(0x1acf, buf);
    n = d2_GetWinObj(win, 1, io_PrintText(0x1ad0, 0x1ad2));
    d2_SetForm(win, 2);
    g1 = d2_GetWinObj(win, 2, io_PrintText(0x1ad1, 0x1ad3));
    d2_SetForm(win, 3);
    io_PrintText(0x1ad4);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0x1c, (char *)&g2, 2, U_I((unsigned)&full_data_1440d14), U_I((unsigned)&full_data_1440d18));
    d2_SetForm(win, 4);
    io_PrintText(0x1ad7);
    sv_ButtonRow(U_I((unsigned)&d2_LastWindow), 0x1c, (char *)&g2, 2, U_I((unsigned)&full_data_1440d20), U_I((unsigned)&full_data_1440d24));
    d2_SetForm(win, 5);
    io_PrintText(0x1adb);
    n = (*(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) + -0x20) / 0x30;
    obj = d2_CreateRadioGroup(0);
    for (i = 0; i < 8; i++) {
        id = d2_WinOpenPic((i % n) * 0x30 + 0x50, (i / n) * 0x30 + 0x20, i + 0x59a, U_I((unsigned)&d2_LastWindow));
        btns[i] = id;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x48) = 1;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x28) = 0;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x2c) = 0;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x50) = 1;
        U_I(U_I((unsigned)&d2_Objs) + id * 0x2ec + 0x1c0) = 3;
        d2_RadioAdd(obj, id);
    }
    d2_SetForm(win, 6);
    id = d2_GetWinObj(win, 6, io_PrintText(0x7e));
    d2_SetObjValue(n, (char *)((unsigned)&init_PlayerProfile + 0xa));
    d2_SetObjValue(g1, (char *)((unsigned)&init_PlayerProfile + 0x2a));
    for (i = 0; i < 8; i++)
        if (U_I(U_I((unsigned)&d2_Objs) + btns[i] * 0x2ec + 8) == U_I(((unsigned)&init_PlayerProfile + 0x4))) {
            d2_RadioSelect(obj, i);
            break;
        }
    d2_SetObjMax(n, 0xc);
    d2_SetObjMax(g1, 0xc);
    g2 = d2_CreateRadioGroup(2, g1, 0);
    d2_RadioSelect(g2, U_B(((unsigned)&init_PlayerProfile + 0x8)));
    g2 = d2_CreateRadioGroup(2, g1, 0);
    d2_RadioSelect(g2, U_B(((unsigned)&init_PlayerProfile + 0x9)));
    d2_ClearInput();
    U_B((unsigned)&kb_Last) = 0;
    /* ROUGH: the state machine 0 .. 5 (the two text fields 0x475980 -> 0x75e5aa / 0x75e5ca, the gender
       buttons 0x46c6c0 and the clicked 0x5fe710, the eight emblems, the exit) and the profile write back
       (WritePrivateProfileStringA x5, 0x5e56c4) are not written yet; the frame pump 0x5309a0 (0x5361d0, b) */
    while (1) {
        d2_MenuNavigate(obj);
        if (sv_RunFrame((unsigned)&main_ChoosePlayer, b) == 0) {
            d2_RadioClear(g2);
            d2_RadioClear(g1);
            d2_RadioClear(obj);
            d2_CloseView(win);
            return ret;
        }
    }
}
