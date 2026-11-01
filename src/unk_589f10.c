/* unk_589f10.c -- the network lobby (0x589f10 .. 0x58bbd0): the "menu\netlobby" screen lobby_Screen, its
   form parts (lobby_Add* / lobby_Show*), the player / city and profession pickers (Menu\CHOOSEPLAYER,
   Menu\CHOOSEPROFESSION) and this player's lobby record lobby_Player (CMLOBBYPLAYER, cm_types.h) with the
   "Network" profile of ai_OptionsFile; the lobby messages go through cm.c's cm_Lobby* API.
   Name unknown (@named_by -). Start: the previous unit ends with "gamedata/screenshots/gilde%04i.bmp" 0x62aa58,
   then the globals 0x62aa7c .. 0x62aa88 open this unit's .data block, whose first literal "menu\netlobby" is
   used by 0x589f10 (code seam at the nop fill 0x589f08). End: the literals climb through 0x58ba70; the next block
   starts with "Handel\Handel_PRODUKTION" 0x62b038 (0x58bd60), so its table init 0x58bbd0 opens the next unit.
   Open: the control API (0x4731d0, 0x473650, 0x52e350, 0x52e860) is unnamed; the U_I / U_B / U_W raw views remain
   in the pickers.
   Open: 0x1460ae0 has two names, net_Conn (net.h) and ai_Unk1460ae0 (cm.h / gv.h / ahm.h).
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
extern GAPDATA full_data_62aaac[];
extern GAPDATA full_data_62aab4[];
extern GAPDATA full_data_62aac0[];
extern GAPDATA full_data_62aad0[];
extern GAPDATA full_data_6f2680[];
extern GAPDATA full_data_6f2684[];
#include "f3init_types.h"
#include "f3init.h"
#include "net.h"
#include "gfx.h"
#include "d3_io_a.h"
#include "main.h"
#include "unk_4b39f0.h"
#include "unk_5d33b0.h"
extern int sv_PopRequest;
extern INIT_PLAYER init_PlayerProfile;
extern int main_ChoosePlayer(int mask, int flag);

#define d2_SetObjValue2 ((void (*)(int, char *))d2_SetObjValue)  /* 0x475cb0 (d2.c: five parameters): @original the lobby screen pushes 2 arguments (input, ch_EmptyName) */
#define cm_JoinChar (*(CMJOIN *)((char *)&ai_Unk9364e0 + 0x2c0))  /* 0x9367a0, the joined character (CMJOIN), as main.c reads it */

#define U_I(a) (*(int *)(a))
#define U_B(a) (*(unsigned char *)(a))
#define U_W(a) (*(unsigned short *)(a))

void d2_FocusInput(); int sprintf(char *buf, const char *fmt, ...);
int lobby_ChoosePlayer(); int lobby_ChooseProfession();

int lobby_Buttons[2] = { -1 };  /* 0x62aa7c, the controls of the lobby form's button row 4 (sv_ButtonRow): [0] ready, [1] the character (the player and profession pickers) */
int lobby_LeaveButton = -1;  /* 0x62aa84, the control of the button row 8: leave the lobby */
int lobby_Form = -1;  /* 0x62aa88, the lobby form "menu\netlobby" (d2_OpenForm), -1: not open */
unsigned int *lobby_List = 0;  /* 0x6f267c, cm's player list (cm_LobbyPlayerList): the count, then the eight player ids */
int lobby_ChatTo = 0;  /* 0x6f5e00, the id of the player the chat lines go to (the selected list line), -1: everybody */
CMLOBBYPLAYER lobby_Player = {0};  /* 0x6f5e20, this player's record as sent to the lobby (join / change) */
CMLOBBYPLAYER *lobby_Me = 0;  /* 0x6f5e78, this player's record in cm's table (cm_LobbyMyPlayer) */
int lobby_Rows[8] = {0};  /* 0x6f5e80, the text control of each line of the player list */
int lobby_MyRow = 0;  /* 0x6f5ea0, the player list line of this player */
int *lobby_State = 0;  /* 0x6f5ea4, cm's lobby state (cm_LobbyState): [1] a lobby message came in */

/* 0x589f10, the network lobby screen: clears this player's record lobby_Player, fills it from the "Network"
   profile (or from the joined saved game's character), joins the lobby (cm_LobbyJoin) and fetches the player
   list; without a lobby state or an own record it shows the message box text 0x87 until a right click or
   escape. Then it opens "menu\netlobby" with its chat input, buttons, city, own player and player list and runs
   the frame loop: enter toggles the chat input and sends the typed line to lobby_ChatTo (0: everybody), the
   ready button toggles ready, the character button (when not ready) runs the player and profession pickers and
   sends the changed record, a click on a player line selects it as the chat target, the leave button leaves
   (returns -1, as does a lost connection); all players ready pops the screen. On leaving it writes the own
   record back to init_PlayerProfile and the "Network" profile. @deep Read all of it against the original and rewrote it: AI_ZERO_AT clear of lobby_Player (0x57 temp at [ebp-4]), the profile reads, defaults and the cm_JoinChar copy, join / list waits, both message-box loops (while with continue: the original keeps the return inline), the form setup through d2_Objs / d2_Inputs fields, the do-while frame loop with the net_Conn.s / cm_LobbyAllReady / d2_ClickedId dispatch, the leave / ready / character buttons, the player-list pick with sel (VC6 threads the not-found exit), the lobby_State[1] refresh and the write-back into init_PlayerProfile; the form name literal had a newline (menu + \n), now the backslash path. 748 = 748 instructions. Left: scheduling and allocation only: the defaults (the original loads cm_NetGame.mode and keeps 0x5a in cl before the stores, ours al after), the d2_ClickedId = -1 store placed before the d2_Inputs w store instead of after it, the player-list pick registers (original i in ecx, row in edx, sel copied to edi and the checked state re-read with cmp [..], esi; ours i in edx, row in ecx, the state loaded into edi), and ecx/edx swapped for d2_PointerObj in the button compares. */
int lobby_Screen(void)
{
    char buf[256];
    char text[256];
    char name[72];
    int ret;
    int state;
    int win;
    int input;
    int slot;
    int sel;
    unsigned int i;
    unsigned int n;

    ret = 0;
    cm_LobbyClearChat();
    AI_ZERO_AT(offset lobby_Player, 0x57);
    lobby_List = 0;
    lobby_ChatTo = -1;
    lobby_State = cm_LobbyState();
    if (lobby_State == 0) {
        win = d2_OpenForm(0, 0, "misc\\Messagebox_NONE_PERGA");
        d2_CentreViewWindows(win);
        io_PrintText(0x87);
        d2_ClickedId = -1;
        while (dm_L.rup == 0 && kb_Last != 1) {
            if (sv_RunFrame((int)lobby_Screen, 0x24087) != 0)
                continue;
            d2_CloseView(win);
            return 0;
        }
        sv_PopRequest = 1;
        d2_CloseView(win);
        return 0;
    }
    GetPrivateProfileStringA("Network", "Name", "Vorname", lobby_Player.firstName, 0x20, ai_OptionsFile);
    GetPrivateProfileStringA("Network", "Familienname", "Nachname", lobby_Player.familyName, 0x20, ai_OptionsFile);
    lobby_Player.coat = GetPrivateProfileIntA("Network", "Wappen", 0, ai_OptionsFile);
    lobby_Player.sex = (unsigned char)GetPrivateProfileIntA("Network", "Geschlecht", 0, ai_OptionsFile);
    lobby_Player.faith = (unsigned char)GetPrivateProfileIntA("Network", "Glauben", 0, ai_OptionsFile);
    lobby_Player.job = (unsigned char)GetPrivateProfileIntA("Network", "Beruf", 0, ai_OptionsFile);
    slot = gm_TitleGroup(lobby_Player.job);
    lobby_Player.job = gm_TitleNumber(slot, 1);
    init_PlayerProfile.status = 0x10;
    lobby_Player.status = 0x10;
    lobby_Player.talents[1] = 0x46;
    lobby_Player.talents[3] = 0x46;
    lobby_Player.reserved = -1;
    lobby_Player.talents[0] = 0x5a;
    lobby_Player.talents[2] = 0x78;
    lobby_Player.talents[4] = 0x5a;
    if (cm_NetGame.mode == 0x7f) {
        lobby_Player.job = (unsigned char)cm_JoinChar.char164;
        lobby_Player.sex = (unsigned char)cm_JoinChar.char09;
        lobby_Player.faith = (unsigned char)cm_JoinChar.char0c;
        lobby_Player.coat = cm_JoinChar.char54 - 0x59a;
        strcpy(lobby_Player.firstName, cm_JoinChar.text1);
        strcpy(lobby_Player.familyName, cm_JoinChar.text2);
    }
    slot = cm_LobbyJoin(&lobby_Player);
    if (slot == -1)
        return -1;
    while (cm_GetSlotState(slot) == 0)
        sv_PumpNet();
    if (cm_GetSlotState(slot) == 2) {
        txt_Format(buf, 0x88);
        sv_MessageBox((int)buf, 0x100);
        return -1;
    }
    slot = cm_LobbyRequestList();
    if (slot == -1)
        return -1;
    while (cm_GetSlotState(slot) == 0)
        sv_PumpNet();
    d2_ShowPicture(0x7d6);
    lobby_Me = cm_LobbyMyPlayer();
    if (lobby_Me == 0) {
        win = d2_OpenForm(0, 0, "misc\\Messagebox_NONE_PERGA");
        d2_CentreViewWindows(win);
        io_PrintText(0x87);
        d2_ClickedId = -1;
        while (dm_L.rup == 0 && kb_Last != 1) {
            if (sv_RunFrame((int)lobby_Screen, 0x24087) != 0)
                continue;
            d2_CloseView(win);
            return 0;
        }
        sv_PopRequest = 1;
        d2_CloseView(win);
        return 0;
    }
    lobby_Form = d2_OpenForm(0x3c, 0, "menu\\netlobby");
    d2_CentreViewWindows(lobby_Form);
    lobby_AddChatScroll();
    lobby_AddButtons();
    lobby_ShowCity();
    lobby_FetchPlayers();
    lobby_ShowMyPlayer();
    lobby_ShowPlayers();
    d2_SetForm(lobby_Form, 1);
    input = d2_GetWinObj(lobby_Form, 0, 0);
    d2_SetObjMax(input, 0x40);
    state = 0;
    d2_SetObjValue2(input, ch_EmptyName);
    d2_Objs[input].w = 0x26c;
    d2_Inputs[d2_Objs[input].input].flags |= 0x200;
    d2_Inputs[d2_Objs[input].input].w = 0x26c;
    d2_ClickedId = -1;
    d2_Inputs[d2_Objs[input].input].maxw = 0x26c;
    do {
        if (state == 0) {
            if (d2_FocusedInput != 0)
                state = 1;
            else if (kb_Last == 0x1c) {
                d2_FocusInput(input);
                state = 1;
            }
        } else if (d2_FocusedInput == 0 || kb_Last == 0x1c) {
            state = 0;
            strcpy(name, (char *)d2_GetObjValue(input));
            d2_SetObjValue2(input, ch_EmptyName);
            if (strlen(name) != 0) {
                if (lobby_ChatTo != -1)
                    cm_LobbySendChat(lobby_ChatTo, name);
                else
                    cm_LobbySendChat(0, name);
            }
        }
        if (net_Conn.s == -1) {
            ret = -1;
            sv_PopRequest = 1;
        } else if (cm_LobbyAllReady() != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == lobby_LeaveButton) {
                slot = cm_LobbyLeave();
                while (cm_GetSlotState(slot) == 0)
                    sv_PumpNet();
                sv_PopRequest = 1;
                ret = -1;
            } else if (d2_PointerObj == lobby_Buttons[0]) {
                slot = cm_LobbySetReady(lobby_Me->ready == 0);
                while (cm_GetSlotState(slot) == 0)
                    sv_PumpNet();
                d2_Objs[lobby_Buttons[0]].checked = lobby_Me->ready;
                if (cm_LobbyAllReady() != 0)
                    sv_PopRequest = 1;
            } else if (d2_PointerObj == lobby_Buttons[1]) {
                if (lobby_Me->ready != 0) {
                    slot = cm_LobbySetReady(0);
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                    d2_Objs[lobby_Buttons[0]].checked = lobby_Me->ready;
                }
                if (lobby_Me->ready == 0 && lobby_ChoosePlayer(0x24087, 1) != 0) {
                    strcpy(lobby_Player.firstName, init_PlayerProfile.firstName);
                    strcpy(lobby_Player.familyName, init_PlayerProfile.familyName);
                    lobby_Player.coat = init_PlayerProfile.coat - 0x59a;
                    lobby_Player.sex = init_PlayerProfile.sex;
                    lobby_Player.faith = init_PlayerProfile.faith;
                    slot = cm_LobbyUpdatePlayer(&lobby_Player);
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                    if (lobby_ChooseProfession(0x24087) != 0) {
                        lobby_Player.job = init_PlayerProfile.job;
                        slot = cm_LobbyUpdatePlayer(&lobby_Player);
                        while (cm_GetSlotState(slot) == 0)
                            sv_PumpNet();
                    }
                }
            } else {
                /* a player line (not the own one): toggles it and makes it the chat target, the others off */
                sel = -1;
                for (i = 0; i < lobby_List[0]; i++) {
                    if (i != lobby_MyRow && d2_PointerObj == lobby_Rows[i]) {
                        d2_Objs[lobby_Rows[i]].checked = d2_Objs[lobby_Rows[i]].checked == 0;
                        sel = i;
                        if (d2_Objs[lobby_Rows[i]].checked != 0)
                            lobby_ChatTo = lobby_List[i + 1];
                        else
                            lobby_ChatTo = -1;
                        break;
                    }
                }
                if (sel >= 0) {
                    for (n = 0; n < lobby_List[0]; n++)
                        if (n != sel)
                            d2_Objs[lobby_Rows[n]].checked = 0;
                }
            }
        }
        d2_ClickedId = -1;
        if (lobby_State[1] != 0) {
            if (cm_LobbyAllReady() != 0)
                sv_PopRequest = 1;
            else {
                slot = cm_LobbyRequestList();
                while (cm_GetSlotState(slot) == 0)
                    sv_PumpNet();
                lobby_List = cm_LobbyPlayerList();
                lobby_ShowMyPlayer();
                lobby_ShowPlayers();
                lobby_ShowChat();
                lobby_State[1] = 0;
            }
        }
    } while (sv_RunFrame((int)lobby_Screen, 0x24087) != 0);
    d2_CloseView(lobby_Form);
    init_PlayerProfile.status = 0x12;
    init_PlayerProfile.job = lobby_Me->job;
    init_PlayerProfile.sex = lobby_Me->sex;
    init_PlayerProfile.faith = lobby_Me->faith;
    init_PlayerProfile.coat = lobby_Me->coat;
    strcpy(init_PlayerProfile.familyName, lobby_Me->familyName);
    strcpy(init_PlayerProfile.firstName, lobby_Me->firstName);
    WritePrivateProfileStringA("Network", "Name", init_PlayerProfile.firstName, ai_OptionsFile);
    WritePrivateProfileStringA("Network", "Familienname", init_PlayerProfile.familyName, ai_OptionsFile);
    sprintf(text, "%d", init_PlayerProfile.coat);
    WritePrivateProfileStringA("Network", "Wappen", text, ai_OptionsFile);
    sprintf(text, "%d", init_PlayerProfile.sex);
    WritePrivateProfileStringA("Network", "Geschlecht", text, ai_OptionsFile);
    sprintf(text, "%d", init_PlayerProfile.faith);
    WritePrivateProfileStringA("Network", "Glauben", text, ai_OptionsFile);
    sprintf(text, "%d", init_PlayerProfile.job);
    WritePrivateProfileStringA("Network", "Beruf", text, ai_OptionsFile);
    ((void (*)(void))d3io_NoopCallback)();
    return ret;
}

char *_itoa(int, char *, int); char *_strupr(char *);

/* 0x58a940, adds the lobby's buttons (144 B): control 4 of the window 0x62aa88 and 0x52e350 (screen 0x6ed938,
   0, the id 0x62aa7c, 2, U_I (0x144abe8), U_I (0x144abec)), control 8 and 0x52e350 (.., the id 0x62aa84, 1,
   U_I (0x144abf0)); in the single-player mode 0x9364dc 0x7f the control 0x62aa80 is disabled (the window table
   0xf8f080 + id * 0x2ec, +0x3c = 1)
   @deep byte but not readable: Q1 (unit unnamed, @named_by -), Q2 (d2_SetForm control API, sv_ButtonRow), Q3 (10 lobby globals), Q4 U_I; the whole lobby/control API has to be named and the globals typed first; no Q6 fix exists */
void lobby_AddButtons(void)
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

void lobby_AddChatScroll(void)
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
/* 0x58aa30, shows the city of the network game on part 2 of the lobby form: the city's emblem picture
   "_STADTWAPPEN_<NAME>" (upcased; "_STADTWAPPEN_BERLIN" when there is none), right of it the city name and
   below that the label txt_NetPlayers with the game's player count @deep Read whole against the original: mode 0x3f test with strcpy x2 + _strupr in each branch, players byte hoisted into ebx after the branches, emblem pic, txt_NetPlayers label; instruction count equal (112/112). Only remaining: the original pushes ebx/esi/edi in the prologue (+0xb) while ours pushes them at the join after the branches (asmdiff: '+ push ebx/esi/edi' at 0x58aa3b, '- push ebx/esi/edi' at 0x58aac3), so every stack offset before the join differs by 0xc ([esp+eax] vs [esp+eax+0xc]) - saved-register placement, allocation/scheduling. */

void lobby_ShowCity(void)
{
    char name[256];
    char pic[256];
    char city[256];
    int obj;
    int x;
    int players;

    d2_SetForm(lobby_Form, 2);
    io_PrintText("$C");
    if (cm_NetGame.mode == 0x3f) {
        strcpy(name, cm_NetGame.hdr.name);
        strcpy(city, cm_NetGame.hdr.name);
        _strupr(name);
    } else {
        strcpy(name, cm_NetGame.hdr.name);
        strcpy(city, cm_NetGame.hdr.name);
        _strupr(name);
    }
    players = cm_NetGame.hdr.players;
    sprintf(pic, "_STADTWAPPEN_%s", name);
    obj = d2_FindPic(pic);
    if (obj == -1)
        obj = d2_FindPic("_STADTWAPPEN_BERLIN");
    obj = d2_WinOpenPic(5, 5, obj, d2_LastWindow);
    x = d2_Objs[obj].w + 0xf;
    obj = d2_CreateWinText(x, 0x19, city, d2_LastWindow);
    sprintf(name, "%s: %d", txt_NetPlayers, players);
    d2_CreateWinText(x, d2_Objs[obj].h + 0x1b, name, d2_LastWindow);
}
/* 0x58aba0, the current player's info window (384 B): the player record 0x6f267c[0x6f5ea0 + 1] (0x4cd380)
   gives the city (+0xc); control 3 of 0x62aa88 and the text 0x602568; the city image control 0x4731d0
   (5, 10, city + 0x59a) and the name / family name text 0x473650 (x + 0xf, 10); then "%s: %s" 0x62aaac
   with the honour title of the gender (U_I (0x143a614), U_I (0x143a63c) for the female byte 0x6f5e30) and
   the profession (the tables 0x143a7fc / 0x143a680 by 0x6f5e2b) and the religion (0x143b3d8 by 0x6f5e31),
   each below the last
   @deep Q1 (unit unnamed, @named_by -); B: code 117 + slots 12 + fill 3; Q2 (6 callees incl. 0x4731d0/0x473650/0x5de652), Q3 (21 addresses), Q4 U_I/U_B + 2 casts; the same missing control API and typed player record */

void lobby_ShowMyPlayer(void)
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

void lobby_FetchPlayers(void)
{
    int h;
    int r;
    int i;

    h = ((int (*)())cm_LobbyRequestList)();
    while (cm_GetSlotState(h) == 0)
        sv_PumpNet();
    lobby_List = cm_LobbyPlayerList();
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

void lobby_ShowPlayers(void)
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
int lobby_ChooseProfession(int param)
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
    n = sv_RunFrame((int)lobby_ChooseProfession, param);
    while (1) {
        if (n == 0) {
            d2_RadioClear(obj);
            d2_CloseView(win);
            return ret;
        }
        d2_MenuNavigate(obj);
        if (U_I((unsigned)&d2_ClickedId) != -1) {
            for (i = 0; i < 0xb; i++) {
                if (U_I((unsigned)&d2_PointerObj) == ids[i + 0xb]) {
                    U_B(((unsigned)&init_PlayerProfile + 0x3)) = gm_TitleNumber(ids[i], 1);
                    U_I((unsigned)&sv_PopRequest) = 1;
                    ret = 1;
                    break;
                }
            }
        }
        n = sv_RunFrame((int)lobby_ChooseProfession, param);
    }
}
/* 0x58ba70, the chat lines of the lobby (352 B): control 5 of 0x62aa88 and the text 0x602568; the chat records
   from 0x4cd480 / 0x4cd4c0 (0x6f2684 counts them): the speaker 0x4cd380 (the record +0x68), its id (+0xc) or 9;
   the line "$%dFF%s: %s$A" 0x62aac0 (the name +0x40, the text) for a record without +100, or
   "$%dFF%s (%s): %s$A" 0x62aad0 with the shout text U_I (0x144abf8) when +100 is the chosen player 0x6f5e78;
   each line through 0x515960; the newest line count grows the chat box (0x6f2684 > 0x6f2680 and the control
   height 0xf8f12a < the box) -> 0xf8f364 = +0x18, 0xf8f368 = the delta, 0x46f840 (the screen)
   @deep Q1 (unit unnamed, @named_by -); B: code 151 + regs 14 (the chat lines); Q2 (7 callees), Q3 (12 addresses), Q4 U_I + 2 casts */

void lobby_ShowChat(void)
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
/* 0x58af50, the choose-player window "Menu\CHOOSEPLAYER" of the network lobby: loads the "Network" profile
   (name, family name, coat of arms, sex, faith) into init_PlayerProfile, shows the two name inputs, the sex
   and faith button rows and the eight coats of arms (the ones other lobby players hold are disabled), then
   steps through them (state 0 .. 5: Enter takes a name, a click a button, the coat ends it; escape, a right
   click or the exit button go one step back or leave); writes the profile back and returns 1 when the player
   was chosen @deep Reconstructed whole against the original 0x58af50..0x58b87b (profile load, form, sex/faith rows, 8 coats, lobby_List coat locking, the state machine 0..5, profile write back); 667 vs 671 instructions; the rest is slots (stack-slot order of the locals) plus one hunk: in state 5 the original evaluates 'cmp ai_MissionMode, 0xff; je; mov eax, [esp+0x1ac] (b)' and then overwrites eax with 1 (ret = sv_PopRequest = 1) - a dead load of the second parameter; tried 'if (ai_MissionMode != -1) ret = b; ret = 1', the ?: forms, else branches, ret=1 before, sv_PopRequest = ret: VC6 drops all of them, so which source keeps this dead load is a variant question (asmdiff: '+ mov al, byte ptr [X] / cmp al, 0xff / je X / mov eax, dword ptr [esp + 0x1ac]'). */

int lobby_ChoosePlayer(unsigned mask, int b)
{
    char num[48];
    char town[256];
    int coats[8];
    int sexRow[2];
    int faithRow[2];
    int sexBtn[2];
    int faithBtn[2];
    int win;
    int firstIn;
    int familyIn;
    int exitBtn;
    int coatGroup;
    int sexGroup;
    int faithGroup;
    int ret;
    int state;
    int perRow;
    int i;
    int id;
    int h;
    CMLOBBYPLAYER *p;
    char redraw;

    redraw = 1;
    ret = 0;
    state = 0;
    GetPrivateProfileStringA("Network", "Name", "Vorname", init_PlayerProfile.firstName, 0x20, ai_OptionsFile);
    GetPrivateProfileStringA("Network", "Familienname", "Nachname", init_PlayerProfile.familyName, 0x20, ai_OptionsFile);
    init_PlayerProfile.coat = GetPrivateProfileIntA("Network", "Wappen", 0, ai_OptionsFile) + 0x59a;
    init_PlayerProfile.sex = GetPrivateProfileIntA("Network", "Geschlecht", 0, ai_OptionsFile);
    init_PlayerProfile.faith = GetPrivateProfileIntA("Network", "Glauben", 0, ai_OptionsFile);
    win = d2_OpenForm(0, 0, "Menu\\CHOOSEPLAYER");
    d2_CentreViewWindows(win);
    io_TownDisplayName(cm_NetGame.hdr.name, town);
    d2_SetForm(win, 1);
    io_PrintText(0x1acf, town);
    firstIn = d2_GetWinObj(win, 1, io_PrintText(0x1ad0, 0x1ad2));
    d2_SetForm(win, 2);
    familyIn = d2_GetWinObj(win, 2, io_PrintText(0x1ad1, 0x1ad3));
    d2_SetForm(win, 3);
    io_PrintText(0x1ad4);
    sv_ButtonRow(d2_LastWindow, 0x1c, sexRow, 2, txt_Male, txt_Female);
    sexBtn[0] = sexRow[0];
    sexBtn[1] = sexRow[1];
    d2_SetForm(win, 4);
    io_PrintText(0x1ad7);
    sv_ButtonRow(d2_LastWindow, 0x1c, faithRow, 2, txt_Faith0, txt_Faith1);
    faithBtn[0] = faithRow[0];
    faithBtn[1] = faithRow[1];
    d2_SetForm(win, 5);
    io_PrintText(0x1adb);
    perRow = (d2_Wins[d2_LastWindow].w - 0x20) / 0x30;
    coatGroup = d2_CreateRadioGroup(0);
    for (i = 0; i < 8; i++) {
        id = d2_WinOpenPic((i % perRow) * 0x30 + 0x50, (i / perRow) * 0x30 + 0x20, i + 0x59a, d2_LastWindow);
        coats[i] = id;
        d2_Objs[id].checkable = 1;
        d2_Objs[id].checked = 0;
        d2_Objs[id].highlighted = 0;
        d2_Objs[id].greyed = 1;
        d2_Objs[id].style = 3;
        d2_RadioAdd(coatGroup, id);
    }
    d2_SetForm(win, 6);
    exitBtn = d2_GetWinObj(win, 6, io_PrintText(0x7e));
    d2_SetObjValue2(firstIn, init_PlayerProfile.firstName);
    d2_SetObjValue2(familyIn, init_PlayerProfile.familyName);
    for (i = 0; i < 8; i++)
        if (d2_Objs[coats[i]].pic == init_PlayerProfile.coat) {
            d2_RadioSelect(coatGroup, i);
            break;
        }
    d2_SetObjMax(firstIn, 0xc);
    d2_SetObjMax(familyIn, 0xc);
    sexGroup = d2_CreateRadioGroup(2, sexBtn[0], sexBtn[1]);
    d2_RadioSelect(sexGroup, init_PlayerProfile.sex);
    faithGroup = d2_CreateRadioGroup(2, faithBtn[0], faithBtn[1]);
    d2_RadioSelect(faithGroup, init_PlayerProfile.faith);
    d2_ClearInput();
    kb_Last = 0;
    /* the coats of arms the other lobby players hold cannot be taken */
    for (i = 0; i < 8; i++)
        d2_Objs[coats[i]].clickDisabled = 0;
    for (i = 1; i < 9; i++) {
        if (lobby_List[i] == 0)
            continue;
        p = (CMLOBBYPLAYER *)cm_LobbyFindPlayer(lobby_List[i]);
        if (p == 0 || p->id == 0 || p == lobby_Me)
            continue;
        d2_Objs[coats[p->coat]].clickDisabled = 1;
        if (d2_Objs[coats[p->coat]].checked != 0) {
            d2_Objs[coats[p->coat]].checked = 0;
            d2_Objs[coats[lobby_Me->coat]].checked = 1;
        }
    }
    do {
        if (lobby_State[1] != 0) {
            /* a lobby message came in: fetch the player list again */
            lobby_State[1] = 0;
            h = cm_LobbyRequestList();
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
            lobby_List = cm_LobbyPlayerList();
            for (i = 0; i < 8; i++)
                d2_Objs[coats[i]].clickDisabled = 0;
            for (i = 1; i < 9; i++) {
                if (lobby_List[i] == 0)
                    continue;
                p = (CMLOBBYPLAYER *)cm_LobbyFindPlayer(lobby_List[i]);
                if (p == 0 || p->id == 0 || p == lobby_Me)
                    continue;
                d2_Objs[coats[p->coat]].clickDisabled = 1;
                d2_Objs[coats[p->coat]].checked = 0;
            }
        }
        if (dm_L.rup != 0 || (d2_ClickedId != -1 && d2_PointerObj == exitBtn) || kb_Last == 1) {
            /* back one step, or leave from the first */
            if (state == 0) {
                sv_PopRequest = 1;
            } else {
                state--;
                redraw = 1;
            }
        }
        if (redraw) {
            for (i = 0; i < 5; i++) {
                id = d2_GetWinObj(win, 0, i);
                if (i <= state)
                    d2_EnableObject(id, 1);
                else
                    d2_EnableObject(id, 0);
            }
            if (state == 0)
                d2_FocusInput(firstIn);
            else if (state == 1)
                d2_FocusInput(familyIn);
            redraw = 0;
        }
        if (state == 0) {
            if (kb_Last == 0x1c) {
                strcpy(init_PlayerProfile.firstName, (char *)d2_GetObjValue(firstIn));
                state = 1;
                redraw = 1;
                kb_Last = 0;
            }
        } else if (state == 1) {
            if (kb_Last == 0x1c) {
                strcpy(init_PlayerProfile.familyName, (char *)d2_GetObjValue(familyIn));
                state = 2;
                redraw = 1;
                kb_Last = 0;
            }
        } else if (state == 2) {
            d2_MenuNavigate(sexGroup);
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == sexBtn[0]) {
                    init_PlayerProfile.sex = 0;
                    state = 3;
                    redraw = 1;
                } else if (d2_PointerObj == sexBtn[1]) {
                    init_PlayerProfile.sex = 1;
                    state = 3;
                    redraw = 1;
                }
            }
        } else if (state == 3) {
            d2_MenuNavigate(faithGroup);
            if (d2_ClickedId != -1) {
                if (d2_PointerObj == faithBtn[1]) {
                    init_PlayerProfile.faith = 1;
                    state = 4;
                    redraw = 1;
                } else if (d2_PointerObj == faithBtn[0]) {
                    init_PlayerProfile.faith = 0;
                    state = 4;
                    redraw = 1;
                }
            }
        } else if (state == 4) {
            d2_MenuNavigate(coatGroup);
            if (kb_Last == 0x1c) {
                state = 5;
                redraw = 1;
            }
            if (d2_ClickedId >= 0x59a && d2_ClickedId < 0x5a2) {
                /* a coat was clicked: clear the others */
                for (i = 0; i < 8; i++) {
                    if (&d2_Objs[coats[i]] != d2_ClickedObj) {
                        d2_Objs[coats[i]].checked = 0;
                        d2_Objs[coats[i]].highlighted = 0;
                        d2_Objs[coats[i]].greyed = 1;
                    }
                }
                state++;
            }
        } else if (state == 5) {
            init_PlayerProfile.tool1 = 0;
            init_PlayerProfile.tool2 = 0;
            init_PlayerProfile.status = 0x12;
            init_PlayerProfile.coat = d2_Objs[d2_Radios[coatGroup].objs[d2_Radios[coatGroup].sel]].pic;
            if (ai_MissionMode != -1)       /* the original keeps this test and the load of b, then sets 1 */
                ret = b;
            ret = 1;
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((int)main_ChoosePlayer, mask) != 0);
    d2_RadioClear(sexGroup);
    d2_RadioClear(faithGroup);
    d2_RadioClear(coatGroup);
    d2_CloseView(win);
    WritePrivateProfileStringA("Network", "Name", init_PlayerProfile.firstName, ai_OptionsFile);
    WritePrivateProfileStringA("Network", "Familienname", init_PlayerProfile.familyName, ai_OptionsFile);
    _itoa(init_PlayerProfile.coat - 0x59a, num, 10);
    WritePrivateProfileStringA("Network", "Wappen", num, ai_OptionsFile);
    _itoa(init_PlayerProfile.sex, num, 10);
    WritePrivateProfileStringA("Network", "Geschlecht", num, ai_OptionsFile);
    _itoa(init_PlayerProfile.faith, num, 10);
    WritePrivateProfileStringA("Network", "Glauben", num, ai_OptionsFile);
    return ret;
}
