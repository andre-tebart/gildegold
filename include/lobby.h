/* lobby.h -- the network lobby (unnamed): the "menu\netlobby" screen and the player / city / profession
   pickers, see src/game/unk_589f10.c */
#ifndef LOBBY_H
#define LOBBY_H

#include "cm_types.h"                     /* CMLOBBYPLAYER */

extern int lobby_Buttons[2];            /* 0x62aa7c, the button row: [0] ready, [1] the character */
extern int lobby_LeaveButton;           /* 0x62aa84, the leave button */
extern int lobby_Form;                  /* 0x62aa88, the lobby form, -1: not open */
extern char lobby_FmtName[8];           /* 0x62aaac, "%s: %s", a player line's two texts */
extern char lobby_FmtHonour[12];        /* 0x62aab4, "%s %s %s ", the honour title, name and family name */
extern char lobby_FmtChat[16];          /* 0x62aac0, "$%dFF%s: %s$A", a chat line */
extern char lobby_FmtChatShout[20];     /* 0x62aad0, "$%dFF%s (%s): %s$A", a chat line with the shout text */
extern unsigned int *lobby_List;        /* 0x6f267c, cm's player list: the count, then the eight player ids */
extern int lobby_ChatShown;             /* 0x6f2680, the chat line count the chat box last showed */
extern int lobby_ChatNewest;            /* 0x6f2684, the chat lines of the frame */
extern int lobby_ChatTo;                /* 0x6f5e00, the player id the chat goes to, -1: everybody */
extern CMLOBBYPLAYER lobby_Player;      /* 0x6f5e20, this player's record as sent to the lobby */
extern CMLOBBYPLAYER *lobby_Me;         /* 0x6f5e78, this player's record in cm's table */
extern int lobby_Rows[8];               /* 0x6f5e80, the control of each player list line */
extern int lobby_MyRow;                 /* 0x6f5ea0, the player list line of this player */
extern int *lobby_State;                /* 0x6f5ea4, cm's lobby state: [1] a lobby message came in */

int lobby_Screen(void);                 /* 0x589f10, the network lobby screen (ROUGH) */
void lobby_AddChatScroll(void);                /* 0x58a9d0, the lobby's control 5 / 6 hook */
void lobby_AddButtons(void);                /* 0x58a940, adds the lobby's buttons */
void lobby_ShowCity(void);                /* 0x58aa30, the city emblem window */
void lobby_ShowMyPlayer(void);                /* 0x58aba0, the current player's info window */
void lobby_FetchPlayers(void);                /* 0x58ad20, waits for the player list */
void lobby_ShowPlayers(void);                /* 0x58ada0, the player list window */
int lobby_ChoosePlayer(unsigned mask, int b);  /* 0x58af50, the choose-player window */
int lobby_ChooseProfession(int a);                /* 0x58b880, the choose-profession window */
void lobby_ShowChat(void);                /* 0x58ba70, the chat lines of the lobby */

#endif
