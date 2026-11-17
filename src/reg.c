/* reg.c -- original file name unknown: the registry helpers under HKEY_CURRENT_USER, key
   "Software\Ahead Entertainment\<name>" (open / create, close, read and write a REG_SZ value; dd_vesa.c and d3_d3d.c
   keep the driver GUIDs there).
   0x4103a0 .. 0x4104e0. Boundary: an object of its own before di_input: reg_OpenKey's strings "%s%s" 0x5f320c and
   "Software\Ahead Entertainment\" 0x5f3214 close sndc's string run in .data, before di_input's plain .data block
   (kb_Base 0x5f3234); in di_input's object they would follow that block. With sndc's object or alone: unknown (this
   object has no data of its own; the split from sndc is a guess). Names are ours. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "reg.h"

/* 0x4103a0 */
HKEY reg_OpenKey(char *name, int mode)
{
    char path[256];
    HKEY key;
    DWORD disp;
    int r;

    sprintf(path, "%s%s", "Software\\Ahead Entertainment\\", name);
    switch (mode) {
    case 1:
        r = RegOpenKeyEx(HKEY_CURRENT_USER, path, 0, KEY_READ, &key);
        break;
    case 2:
        r = RegCreateKeyEx(HKEY_CURRENT_USER, path, 0, NULL, 0, KEY_WRITE, NULL, &key, &disp);
        break;
    default:
        return (HKEY)-1;
    }
    if (r == 0)
        return key;
    return (HKEY)-1;
}
/* 0x410430, close a registry key opened by reg_OpenKey */

void reg_CloseKey(HKEY key)
{
    RegCloseKey(key);
}
/* 0x410440 */

int reg_GetString(HKEY key, char *name, char *out)
{
    BYTE buf[128];
    DWORD size = 128;
    BYTE *p;
    BYTE c;

    if (RegQueryValueEx(key, name, NULL, NULL, buf, &size) == 0) {
        p = buf;
        do {
            c = *p;
            p[(BYTE *)out - buf] = c;
            p++;
        } while (c);
        return 1;
    }
    return 0;
}
/* 0x4104b0, store value as a REG_SZ string under name */

void reg_SetString(HKEY key, char *name, char *value)
{
    RegSetValueEx(key, name, 0, REG_SZ, (BYTE *)value, strlen(value) + 1);
}
