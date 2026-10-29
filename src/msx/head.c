/* msx
   unit of rea 8cef7d5b (src/game/msx.c); boundaries in decl/text.dcl, notes in src/msx/open.md */
#line 1 "D:\\develop\\units\\Msx\\msx.c"
/* msx.c -- D:\develop\units\Msx\msx.c (leaked assert path).
   Streamed music and ambient tracks: a worker thread updates the track fades every 50 ms, music
   tracks cross-fade (the next track is queued until the playing one has faded out), and a master
   fade scales the music driver volume. Extent in GildeGold.exe: 0x423770..0x424510.
   Functions are kept in their original order. Names are ours.
   @named_by leaked
   @boundary plausible */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include "msx.h"
#include "sfx.h"

extern volatile unsigned long d3_Time;      /* 0x6333cc, see d3.h */
/* d3_Time * 13, spelled so that the volatile is read three times */
#define MSX_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

extern LARGE_INTEGER msx_Last;

