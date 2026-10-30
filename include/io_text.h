/* Declarations of io_text.c (0x5142e0..0x519010): the text builders and the text engine ("io_Text():"). Names are ours. */
#ifndef IO_TEXT_H
#define IO_TEXT_H

int io_TownDisplayName(char *name, char *out);                                   /* 0x5142e0, the display name of the town called name (the player's town */
int io_TrailingDigits(char *s, int n, char *out);                                /* 0x514360, the digits at the end of s[0 */
int io_CharacterName(int id, unsigned char c, unsigned char kind, char *out);    /* 0x5143d0 */
int io_CharacterNameMarkup(int id, unsigned char c, unsigned char kind, char *out); /* 0x514e90 */
int io_PrintText(char *fmt, ...);                                                /* 0x515960, the text engine: prints the text fmt (a literal or an id of the text */
void txt_Format(char *out, int id0, ...);                                        /* 0x5176d0, the same token engine as 0x515960 into the buffer out instead of the */

extern int ai_Unk6333b8;                                     /* 0x6333b8 */
extern int ai_Unk6ed978;                                     /* 0x6ed978 */
extern int ai_Unkfe7c20;                                     /* 0xfe7c20 */
extern int ai_Unk114fe60[18440];                             /* 0x114fe60 */
#endif
