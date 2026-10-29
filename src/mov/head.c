/* mov.c -- original file name unknown ("MOVIEWNDCLASS").
   Movie playback through DirectShow: a filter graph renders the file into an own or a given window.
   Extent in GildeGold.exe (inferred, not proven): 0x422c90..0x423770.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/mov.c); boundaries in decl/text.dcl, notes in src/mov/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <objbase.h>
#include <strmif.h>
#include <control.h>
#include <evcode.h>
#include "mov.h"

extern const CLSID CLSID_FilterGraph;

