/* err.c -- original file name unknown ("Error-Handler", "<exe dir>\<name>_error.log").
   Error handler and log channels, with an unhandled-exception filter that writes a crash dump with a
   symbolized stack walk. Extent in GildeGold.exe (inferred): 0x4214a0..0x4223c0.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/err.c); boundaries in decl/text.dcl, notes in src/err/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "err.h"

/* symbol map: header, then nsyms ERRSYM records (each 0x10 + len bytes) at data + symoff, and
   nstrings ERRSTR records (each 4 + len bytes) at data */
typedef struct ERRMAP {
    int unk0;
    unsigned int nstrings;          /* 0x04 */
    unsigned int nsyms;             /* 0x08 */
    int symoff;                     /* 0x0c */
} ERRMAP;

typedef struct ERRSYM {
    int len;                        /* length of name including the 0 */
    unsigned int lo;                /* 0x04 */
    unsigned int hi;                /* 0x08 */
    unsigned int file;              /* 0x0c, string index */
    char name[1];                   /* 0x10 */
} ERRSYM;

typedef struct ERRSTR {
    int len;
    char s[1];
} ERRSTR;

typedef struct ERRWALK {
    ERRMAP *map;
    HANDLE file;                    /* 0x04 */
    unsigned int addr;              /* 0x08 */
    unsigned int prev;              /* 0x0c */
    unsigned int last;              /* 0x10 */
    unsigned int count;             /* 0x14 */
    unsigned int repeat;            /* 0x18 */
} ERRWALK;

int err_Channels[12] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800 };  /* 0x5f6d48 */
/* 0x5f6d78 */
char err_Names[12][32] = {
    "_error.log", "_ai.log", "_sim.log", "_dbg.log", "_script.log", "_cutscene.log",
    "_meister.log", "_d3.log", "_stats.log", "_msx.log", "___tracker.log", "_meister2.log"
};

int err_Started = 0;  /* 0x632368 */
void (*err_CrashCB)(HANDLE) = 0;  /* 0x63236c, set by err_Init, called by the crash filter with the log handle (or -1 when no log was written) */
unsigned int err_Flags = 0;  /* 0x632370 */
char err_ShowUnknown = 0;  /* 0x632374 */
char err_Buf[8192];  /* 0x1461440 */
HWND err_Hwnd;  /* 0x1463440 */
char err_Dir[260];  /* 0x1463460 */
HANDLE err_Console;  /* 0x1463564 */

LONG WINAPI err_ExceptionFilter(EXCEPTION_POINTERS *ep);
ERRSTR *err_GetString(ERRMAP *map, unsigned int i);
char err_WriteSymbol(ERRWALK *w);
char err_IsCodeProtect(unsigned char prot);
char err_CheckCall(char *p, int n, int base, int *target);

