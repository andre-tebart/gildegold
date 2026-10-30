/* d3_int_io.c -- scene file reader ("d3_int_io:" allocation tag, "rd_object()" in an error message): the object
   tree (0x44abb0) and the floor (0x44b920). Its own TU (see d3_obj.c). The next function,
   d3_LoadScene (0x44be60), calls both. Names are ours except rd_object.
   unit of rea 8cef7d5b (src/game/d3_int_io.c); boundaries in decl/text.dcl, notes in src/d3_int_io/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#define d3_SetParent d3_SetParent_void      /* d3.h declares it void; it returns char (d3_interface.h) */
#include "d3.h"
#undef d3_SetParent
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_interface.h"
#include "d3_int_io.h"
/* 0xfc2ffc is the next field of the 0xfc2e00 head sentinel (0xfc2e00 + 0x1fc), not a separate global:
   d3io_LoadObjectFile links the first stock object there. */
#define d3_StockListHead (*(D3OPS **)((char *)d3_Unkfc2e00 + 0x1fc))  /* 0xfc2ffc */
/* err.h's other declarations shift the /O2 register allocation of the byte-matched d3_LoadScene (3 bytes at
   0x44c0df); the file only calls err_Message. Its own prototype keeps the call typed, where a local copy of
   the whole header does not compile byte-identically. */
void err_Message(char *text);
void d3_SetFog(unsigned long start, unsigned long end, unsigned long color);  /* 0x43a6f0, d3_engine.c; @original: the reader passes the raw dwords of the two floats (no int -> float conversion; the original pushes them unchanged) */

