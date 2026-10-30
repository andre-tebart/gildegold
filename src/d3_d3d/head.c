/* d3_d3d
   unit of rea 8cef7d5b (src/game/d3_d3d.c); boundaries in decl/text.dcl, notes in src/d3_d3d/open.md */
#line 1 "D:\\develop\\Units\\d3\\d3_d3d.c"
/* d3_d3d.c -- original path D:\develop\Units\d3\d3_d3d.c
   Direct3D 6 device setup, render states, texture surfaces. Extent in GildeGold.exe (inferred, not
   proven): 0x4359f0..0x438c30; the functions before it use d3_anim.c's data, the ones after it no D3D.
   Functions are kept in their original order. Names are ours except where the D3DCHECK texts give them.
   @named_by leaked
   @boundary proven */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares these with int parameters; they take chars. */
#define d3_BeginScene d3_BeginScene_int
#define d3_SetBlendState d3_SetBlendState_int
#include "d3.h"
#undef d3_BeginScene
#undef d3_SetBlendState
#include "d3_d3d.h"
extern D3DTEXFMTCTX d3d_UsedTexFormat;
#include "surf.h"
#include "di_input.h"
#ifdef REA_FRAME_DUMP
#include "rea_dump.h"           /* debug build: REA_DUMP_TRIGGER / REA_DUMP_BACKBUFFER */
#endif
#include "data_views.h"
#define dd_BBitsC (*(char *)&dd_BBits)  /* 0x1464fbd, dd_BBits (dd_vesa.c: unsigned char) read signed: @original the byte-matched d3d_EnumTextureFormatsCallback sign-extends it */
#define dd_GBitsC (*(char *)&dd_GBits)  /* 0x1463580, dd_GBits (dd_vesa.c: unsigned char) read signed: @original the byte-matched d3d_EnumTextureFormatsCallback sign-extends it */
#define dd_RBitsC (*(char *)&dd_RBits)  /* 0x1463570, dd_RBits (dd_vesa.c: unsigned char) read signed: @original the byte-matched d3d_EnumTextureFormatsCallback sign-extends it */
extern unsigned char d3_MipSkip;

