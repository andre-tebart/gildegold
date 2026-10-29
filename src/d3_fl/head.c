/* d3_fl.c -- terrain ("floor") texture cache: builds 16-bit textures for groups of map tiles by
   blending the four corner tile textures with per-pixel weights. Allocation tag "d3_fl:TextureCache", so
   probably d3_fl.c (no path string). Extent in GildeGold.exe: 0x43eab0..(beyond 0x43fed0); d3_engine.c's
   .data (d3e event names and tags) ends before 0x5fb970, which holds d3fl_Blend = 0x43eab0, followed by
   this file's strings. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_fl.c); boundaries in decl/text.dcl, notes in src/d3_fl/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_light.h"
#include "d3_d3d.h"
#include "data_views.h"
extern const char d3_ScrollPool[];

/* 0xfe79fc / 0xfe7a00 / 0xfe7a04 are d3d_UsedTexFormat.pf2 (+0x2c) + 0x10 / 0x14 / 0x18. */
#define d3d_TexRMask (d3d_UsedTexFormat.pf2.dwRBitMask)
#define d3d_TexGMask (d3d_UsedTexFormat.pf2.dwGBitMask)
#define d3d_TexBMask (d3d_UsedTexFormat.pf2.dwBBitMask)

