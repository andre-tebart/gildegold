/* unk_589f10.h -- the network lobby (unnamed): the "menu\netlobby" screen and the player / city / profession
   pickers, see src/game/unk_589f10.c */
#ifndef UNK_589F10_H
#define UNK_589F10_H

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
