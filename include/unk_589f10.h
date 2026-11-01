/* unk_589f10.h -- the network lobby (unnamed): the "menu\netlobby" screen and the player / city / profession
   pickers, see src/game/unk_589f10.c */
#ifndef UNK_589F10_H
#define UNK_589F10_H

#include "cm_types.h"                     /* CMLOBBYPLAYER */

extern int lobby_Buttons[2];            /* 0x62aa7c, the button row: [0] ready, [1] the character */
extern int lobby_LeaveButton;           /* 0x62aa84, the leave button */
extern int lobby_Form;                  /* 0x62aa88, the lobby form, -1: not open */
extern unsigned int *lobby_List;        /* 0x6f267c, cm's player list: the count, then the eight player ids */
extern int lobby_ChatTo;                /* 0x6f5e00, the player id the chat goes to, -1: everybody */
extern CMLOBBYPLAYER lobby_Player;      /* 0x6f5e20, this player's record as sent to the lobby */
extern CMLOBBYPLAYER *lobby_Me;         /* 0x6f5e78, this player's record in cm's table */
extern int lobby_Rows[8];               /* 0x6f5e80, the control of each player list line */
extern int lobby_MyRow;                 /* 0x6f5ea0, the player list line of this player */
extern int *lobby_State;                /* 0x6f5ea4, cm's lobby state: [1] a lobby message came in */

int ai_Unk589f10(void);                 /* 0x589f10, the network lobby screen (ROUGH) */
void ai_Unk58a9d0(void);                /* 0x58a9d0, the lobby's control 5 / 6 hook */
void ai_Unk58a940(void);                /* 0x58a940, adds the lobby's buttons */
void ai_Unk58aa30(void);                /* 0x58aa30, the city emblem window */
void ai_Unk58aba0(void);                /* 0x58aba0, the current player's info window */
void ai_Unk58ad20(void);                /* 0x58ad20, waits for the player list */
void ai_Unk58ada0(void);                /* 0x58ada0, the player list window */
int ai_Unk58af50(int a, int b);         /* 0x58af50, the choose-player window */
int ai_Unk58b880(int a);                /* 0x58b880, the choose-profession window */
void ai_Unk58ba70(void);                /* 0x58ba70, the chat lines of the lobby */

#endif
