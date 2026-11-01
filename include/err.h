/* err.c -- error handler / log ("Error-Handler", "<exe dir>\<name>_error.log"), original file name
   unknown. Extent in GildeGold.exe (inferred): 0x4214a0..0x4223c0, including the unhandled exception
   filter at 0x4215c0 (not a Ghidra function). Names are ours. */
#ifndef UNK_4214A0_H
#define UNK_4214A0_H
#include <windows.h>                  /* Win32 types */

#define ERR_LOG      0x01           /* err_Flags: write the log files */
#define ERR_MSGBOX   0x02           /* show messages in a message box */
#define ERR_DEBUG    0x04           /* OutputDebugString */
#define ERR_CONSOLE  0x08           /* write to a console */
#define ERR_TIME     0x20           /* prefix log lines with timeGetTime() */

void err_Write(char *text, unsigned int flags, int channel);                            /* 0x4214a0 */
void err_Init(int started, void (*crashCB)(HANDLE), HWND hwnd, unsigned int flags);     /* 0x4220b0 */
void err_Exit(void);                                                                    /* 0x422250 */
void err_Message(char *text);                                                           /* 0x4222b0 */
void err_Log(char *file, int line, char *text, int channel);                            /* 0x422330 */

#endif
