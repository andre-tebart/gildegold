/* reg.c -- the registry helpers under HKEY_CURRENT_USER\Software\Ahead Entertainment\ (0x4103a0 .. 0x4104e0).
   Names are ours. */
#ifndef REG_H
#define REG_H

#include <windows.h>

HKEY reg_OpenKey(char *name, int mode);                                 /* 0x4103a0, mode 1 read, 2 create; -1 on failure */
void reg_CloseKey(HKEY key);                                            /* 0x410430 */
int reg_GetString(HKEY key, char *name, char *out);                     /* 0x410440, out needs 128 bytes */
void reg_SetString(HKEY key, char *name, char *value);                  /* 0x4104b0 */

#endif
