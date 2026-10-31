# shp.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): memory tags "d2:shp:..." / "shp_t:...". Round 20 (d2t): own .data from 0x5fed74 (colour keys, 640 / 480 / 800) after the GFX.C strings; d2_Unk47c0e0 595 -> 303 bytes once d2_Unk479f10 is defined above it; 0x47c5e0..0x47d210 (banks / animated shapes, d2u) joined with the same check count; 0xcc padding after 0x47d210 ends the obj
- Boundaries: separated (proven): own .data from 0x5fed74 after the GFX.C strings (round 20); 0xcc padding after 0x47d210.
  Correction 2026-10-23: that 0xcc is the linker's fill after d2_Unk47d210's unpadded section (`#pragma optimize("", off)` around
  the function reproduces it, bytes identical), not an object end; the end boundary to unk_47d600.c rests on the subject only.
- Range then: 0x479d00..0x47d210
- Was: unk_47ab30.c (0x479d00.. moved from unk_479610.c) + unk_47c5e0.c (round 20)

## Open work
- d2_GrabShape8Runs 0x47ad40 (match-base 2026-10-31 opus3, deep, 346 vs 353): left w / h in edi / esi swapped, the box variables in other slots, frame 0xda0 vs 0xd9c (the original keeps the masked pitch in a stack temp and recomputes j * pitch for the skip-run row where ours reuses t).
- d2_ShpBlit16Scaled 0x47b350 (match-base 2026-10-31 opus3, deep, 270 vs 270): only the clipped branch's row caching (ours ebp, the original reloads [esp+0x2c]) is left. Its twin d2_ShpBlit16ScaledLit 0x47b640 likely wants the same treatment: n is an unsigned char there too (and edi,0xff), the run / pixel loops as for loops, the unclipped branch first.
- resolved 2026-10-23: d2_Unk47d210 matches with `#pragma optimize("", off)` around it (no alignment pad in the original section, the linker's 0xcc follows); its dead variant blocks (je +0 dispatch) are inside the matched section.
- see notes/units/gfx.md, Open work: "d2 tail after round 20 (d2t done: TU layout, joins, D2SURF / D2SHP, d2.h prototypes; notes ..."

## d2r: 0x479610..0x47ab30 (round 19) [d2r]

### Report
File: src/game/unk_479610.c + include/unk_479610.h (own types GSURF, GSHAPE; does NOT include d2.h).

Nonmatching (6, all drafts, translated from the Ghidra pseudocode):
- 0x4798f0 (51/240 bytes): only register allocation differs (original: surf in eax, bpp>>3 hoisted into ecx
  before the switch; ours: surf in ecx). r/g/b are locals that VC6 puts into the dead x/y/surf slots.
- 0x479bf0 (92/224): ours has r and g in swapped registers (original r=edi, g=ebp, surf=esi, y=ebx).
- 0x479d00 (170/240): original keeps a zero in ebx (xor ebx,ebx; byte stores of bl; cmp eax,ebx before the
  store to the global), ours uses immediates and test eax,eax; tried r=g=b=0, temp pointer, assign-in-if.
- 0x479f40 / 0x47a460 / 0x47a7d0 (1159/1328, 775/912, 771/848): original computes the row pointer from
  scratch every row; ours strength-reduces it (running pointer + a [pitch*3] slot). Unsolved. Also the colour
  args of 478960: the original loads the word at 0x6f0455 (mov ax) and byte 0x6f0454 separately, ours loads
  the dword at 0x6f0454. Tail loops reuse variables as in Ghidra (minx/maxx/miny of the bounds scan are the x
  counter / segment count / row counter of the row loop).

TU: no boundary found inside the scope. 0x479610..0x47a7d0 is the D2.C part after GFX.C (0x478ee1..0x4790ae
per tu_anchors; 0x4790ae..0x479610 is d2f/d2q's). Scope end 0x47ab30 starts the next function (not mine).
Suggested file name for the orchestrator: keep unk_479610.c or join into d2.c at the d2 join (its data
externs: 5fed74/78 marker colours, 6f0450, 6f0454 transparent colour, 6f0460 light table, f8a11c/630/640/64c).

Shared headers: none touched. d2.h declares 0x479610, 0x4796c0, 0x4797d0, 0x4799e0, 0x479b60, 0x479b90,
0x479df0 with `void` return, but the originals return 1/0 (eax) and 479bf0's third/fourth args are w/h, not
x1/y1 (rects drawn as outline of w x h), 479b60/b90 take a 16 bit colour. Fix d2.h at the join (callers ignore
the return values, so no bytes change).

General VC6 lessons:
- A function with early `return 0` plus a `return 1` in each switch case keeps ebx pushed at entry; the original
  (4799e0) defers the push (shrink-wrap) when the nested `if (ok) { ...switch with break...; return 1; } return 0;`
  has a single `return 1` after the switch. Per-case `return 1` loses it.
- Clearing a DDBLTFX: `memset(&fx,0,sizeof fx); fx.dwFillColor = 0; fx.dwSize = 100;` gives the extra
  `mov [..+0x50],eax` store of the original.
- Rect field store order decides scheduling (479610: src.left, src.top, dst.top, src.bottom, src.right, dst.left,
  dst.bottom, dst.right); brute-forced 479cd0 (left, top, right, bottom). Clip `x = max(x, left)` written
  as `if (x < surf->left) x = surf->left;` on the parameter itself.
- `(unsigned char)b >= 1` on an int parameter gives `mov al,[b]; cmp al,1; jb`; `h >= 1` gives `cmp eax,1; jl`.
- Compare operand order follows the source: `x >= surf->left` (cmp x,mem; jl), not `surf->left <= x`.

## d2s -- 0x47ab30..0x47c5e0 (round 19), src/game/unk_47ab30.c + include/unk_47ab30.h [d2s]

### Report
Scope: the shape ("shp") part of D2.C: grabbing a rectangle of a surface into a run-length coded shape, clipped
drawing of shapes, 8/24 bit -> 16 bit shape conversion, bank conversion. 47b220 d2_Unk47b220 is the grabber dispatch, 47c3d0 d2_Unk47c3d0 the convert dispatch.

Nonmatching (10), closest first:
- 47c410 d2_Unk47c410 (bank conversion): 18/464 bytes, only ecx/edx swapped (original: size in ecx, offset pointer
  in edx; tried local copies of size / n, pointer loops, countdown loops, decl order, `size + total`).
- 47bd70 d2_Unk47bd70 (draw dispatcher): everything equal except the head: original loads `s->w` into edx
  (xor edx,edx / mov dx) before the first compare and does `lea edi,[edx+ecx]`; ours loads it into edi after
  (`xor edi,edi; mov di; add edi,ecx`), clip right in edx instead of esi. Tried w as local (int / ushort / hoisted).
  The switch layout is right now: case 0: return 0; case 2 before case 1 in source (`case 0: return 0; case 2:
  return 1; case 1: ba60; return 1;`), `old = d2_Unk5fed84; d2_Unk5fed84 = surf->pitch;` before the switch and
  `d2_Unk5fed84 = old;` after it (the original restores the global on the no-match path).
- 47b900 d2_Unk47b900 (shape to grey): fp part right (`((float)(r & 0xff) + (b & 0xff)) * 0.2f + (g & 0xff) * 0.59f`,
  int r,g,b filled through dd_SplitRGB, `d += run[0] >> 1`), loop counters of the original live in the param slots
  x / y and in memory; register allocation differs.
- 47c0e0 d2_Unk47c0e0 (24 -> 16 bit): same structure, differences in the loop guards (original `cmp [esi],0; jbe`)
  and in the pixel address (`lea ecx,[ebp+eax*2+0x32]; add eax,ecx`).
- 47bec0, 47ba60, 47b350, 47b640, 47ab30, 47ad40: far off (register allocation / spills differ), drafts are faithful
  translations of the Ghidra code.

Unresolved details worth knowing:
- 47ab30 / 47ad40: bounds `minx, miny, maxx, maxy` are `unsigned short` (compared with `& 0xffff`, final compare
  `cmp bp,si`, in 47ad40 `cmp WORD PTR [mem], reg`); the original keeps maxx / maxy in stack slots (dword stores)
  and minx / miny in esi / edi, and does not strength-reduce `(y + i) * pitch`. In 47ad40 the original re-tests the
  pixel (`test dl,dl`) before each of the four bound updates (nested ifs in Ghidra); VC6 merges them in ours.
  The "and 0xffff" copy test in the palette key pass of 47ad40 has a bug in the original that we copied: the
  inner loop indexes `src[i]` (row index) instead of `src[row][col]`.
- 47b350 / 47b640: the original has the precedence bug `(*run >> 1 + acc) % n` (shifts by acc + 1); copied.
  `n` is the `unsigned char` parameter (and edi,0xff; idiv). The original keeps h and the row counter in stack
  slots (param slots).

### Names / structure found
- Shape header (packed, 0x32 bytes, D2SHP in unk_47ab30.h): +0 size, +4 x0, +6 w, +8 y0, +0xa h (ushort),
  +0xc type (0 = 8 bit, 1 = 16 bit, 2 = 24 bit), +0xd mode (0 copy, 1 half dark, 2, 3), +0xe..+0x15 and
  +0x16..+0x1d four shorts each (second one = marker points found by the colours at 0x5fed74 / 0x5fed78),
  +0x26 nruns (-1 = plain rows, no runs), +0x2a offset of the table of line offsets, +0x2e pixel count, data from
  +0x32: per line a count, then runs of { skip (bytes), n, n pixels }.
- Bank header (D2BANK, 0x845 bytes): +0x2a count, +0x30 size, +0x34 kind, +0x3e palette offset, +0x43 palette
  count, +0x45 offsets of the shapes (0x200 entries). 47c610 (out of scope) appends a shape to a bank ("SHAPBANK").
- Globals declared in unk_47ab30.h (not in d2.h): d2_Unk6f0454 (colour key, `unsigned char r; unsigned short gb`
  packed at an odd address, passed to dd_RGB as r / (uchar)gb / (uchar)(gb >> 8)), d2_Unk5fed74 / d2_Unk5fed78 (same
  kind, initialised data 0000ff / ff0000), d2_Unkf8a11c (`unsigned short`, mask for "half dark": `c >> 1 & mask`),
  d2_Unkf8a120[256][4] (palette), d2_Unk6f0460 (ushort colour table), d2_Unk5fed84, d2_Unkf8a630.
- Functions outside the scope called from here (declared in unk_47ab30.h): 0x479f10 (line table fixup, (int *lines,
  shape)), 0x479f40 / 0x47a460 / 0x47a7d0 (grabbers, (x, y, w, h, bits, uchar mode, ushort pitch) -> shape),
  0x47cf00 / 0x47d210 / 0x47d600 / 0x47d710 (draw run shapes, (x, y, bits, shape); the pitch goes through
  d2_Unk5fed84), 0x47c610 (add shape to bank).
- D2SURF (d2.h): the byte at +0x14 is the depth (8 / 16 / 24); +4 is also passed as `pitch` (ushort) to the grabbers.
- Memory tags: "shp_t:GrabBit8:ShapeHeader", "shp_t:ShapeHeader", "d2:shp:_GrabBit8:ShapeHeader",
  "d2:shp:NewShape", "d2:shp:NewBank"; message "shp_Convert8To16: Converting of NoReadAndSkip Shapes not
  surported..." (0x47bec0). The `shp_t:` tags (47ab30, 47b220 side) vs `d2:shp:` suggest the shape code was once its
  own unit; same TU as d2.c as far as we can tell (run on past 0x47d000).

### General VC6 lessons
- b220: `char bpp = surf->bpp; switch (bpp)` scheduled the load before the grab8 test; `switch (*((char *)surf +
  0x14))` inside each branch matched (a field read into a local before a branch is hoisted above the test of
  another parameter; read inline to keep the original order).
- A switch with an empty `case 0:` loses the case (no `sub ecx,0`); `case 0: return 0;` keeps it, and the cases
  are laid out in source order after the compare chain (chain is sorted by value): write `case 2` before `case 1`
  to get case 2's code first.
- `if (x != 0) do { } while (m < n)` / `for (m = 0; m < *p; m++)` produce `cmp mem,0; jbe` (unsigned guard); an
  extra `if (*p != 0)` around a `for` gives a double guard.
- Operand order of compares follows the source: `if (j > maxx)` gives `cmp eax,ebp; jle`, `if (maxx < j)` the
  mirrored `cmp ebp,eax; jge`.
- `(float)(a) + b` with int b gives fild / fiadd; `(a + b)` in ints gives an integer add and fild QWORD for unsigned.

### Outside the scope
- d2.h declares d2_Unk47c410 as `void *(void *, int, int)`; kept (bank passed as void *).
- 0x47c5e0 (next function, "SHAPBANK" area) and everything up to ~0x47d000 are the rest of this TU.
- uv run build reports ch_RemoveAction (0x4047a0) nonmatching 12/112 before and after our files (not ours).

## d2u -- 0x47c5e0..0x47d960 (round 20) [d2u]

Files: src/game/unk_47c5e0.c + include/unk_47c5e0.h (0x47c5e0..0x47d210), src/game/unk_47d600.c (0x47d600, 0x47d710; uses unk_47c5e0.h).
21 functions (20 listed + 0x47ca40, a start Ghidra missed between 47ca10 and 47cae0; called from d2.c).

Nonmatching (4, 47d210 and 47d710 only by padding):
- 47c830 (235/304): the original keeps eax = 1 from entry and pushes edi (old pitch); abs(mode) computed in each branch. Not found.
  A `ret = 1` single-return version is saved as build/variants/47c830_ret_version.c (277 bytes differ, but has the same push edi / mov reg,1 shape).
- 47cd20 (74/144): original has the loop exit `or eax,-1; ret` inline and `cmp eax,0x10; jl body; or eax,-1; ret`, loads data first. z must be
  an unsigned char (byte param, `test dl,bl`): d2.h says int, renamed trick in unk_47c5e0.c.
- 47d210 (14/1008), 47d710 (7/592): code bytes identical, only the trailing padding differs (orig int3 = 0xcc, ours nop).

TU facts
- 0x47cf00 / 0x47d210 / 0x47d600 / 0x47d710 look hand assembled: prologue `push ebp; mov ebp,esp; push ebx; push esi; push edi`
  then `push edi,esi,eax,ebx,edx,ecx,ebp` (MASM `proc uses` + a macro), epilogue pops in reverse, `jmp next` jumps encoded as near jumps
  with displacement 0 (cases 0,1,3,4,5 of the mode switch in 47d210). Reproduced with `__asm` bodies generated from the disassembly
  (each instruction one line; zero-displacement near jumps as `_emit` bytes, one `_emit` per line; labels for the others).
- Padding after 0x47d210 and after 0x47d710 is 0xcc, everywhere else in the exe it is 0x90: object boundaries at 0x47d600 and 0x47d960
  (so 0x47d600.. is a TU of its own, and 0x47d210 ends one). 0x47cf00..0x47d210 follow 0x47ced0 with nop padding (same TU as the C code?
  unproven). Suggested names: unk_47c5e0.c -> d2 shape tail / "shp" (D2.C), unk_47d600.c -> shape draw asm.
- Scope end: 0x47d960 is not mine; unk_47d600.c continues there (not in this scope).

Shared headers: none changed (d2.h / unk_47ab30.h read-only). Differences found against d2.h:
- d2_Unk47c770 and d2_Unk47c970 return int (0 / 1), d2.h says void (renamed trick around `#include "d2.h"`, prototypes in the .c).
- d2_Unk47cd20: z is an unsigned char (only bit 0 used), d2.h says int.
- D2BANK (unk_47ab30.h) could use names: +0x2c max width, +0x2e max height (ushort), +0x42 = play mode of animations (pad42), +0x08 = 1 (version).
- d2_Unkf8a528[] (d2.h, D2SND) is the tail of my D2ANIM records (d2_Unkf8a520[16], 0x11 bytes: bank, frame, timer, shape, shadow, back, x, y).
- d2_Unk6333d0 is tb_Unk6333d0 (tb.h), volatile.

VC6 lessons
- A local copy of a loop-invariant (`f = a->frame`) changed the register order and the merge of `if (back == 0) ..; if (back == 1) ..`
  (compiler knew the cases exclusive); using `a->frame` directly (reload) fixed both (47cb90).
- `memset(&arr[n], 0, 0x11)` inlines to four dword stores + a byte store through one pointer (47ce30).
- A bank header copied from a string literal: `char tag[] = "SHAPBANK"; memcpy(bank, tag, 8);` gives the three loads at function start.
- `unsigned char` compares against a ushort member: `(unsigned char)i > bank->n` reproduces xor/mov dl, `n < i` does not (operand order).
- Variables in `int` arithmetic: global `tb_Unk6333d0 - d2_Unk6f0464 < 1` unsigned (d2_Unk6f0464 is unsigned).

## Also about this unit
- notes/units/d2.md, section [d2t]

## match-base 2026-10-23
- Reference updates only (an unk_47d600.c Readability rename): d2_SurfPitch, d2_ShpDataOfs, d2_ShpRows,
  d2_ShpRowGap, d2_ShpLightTable / d2_ShpPalTable and the three blitter names (evidence in
  notes/units/unk_47d600.md).

## match-base 2026-10-24 (base-2 worker, shard 2/6)
- d2_Unk479d00 -> match-deep (renamed d2_MakeLightTable; the mem tag "d2:shp:shp_LightTable" gives the role).
  Exactly the Open work case above: register allocation only (the original materialises the zero in ebx at
  entry, stores the three byte locals as bl, compares the mem_Alloc result with cmp eax,ebx and keeps lim in
  edi / the counter in esi; ours stores immediates, uses test eax,eax and swaps lim / counter). Its recorded
  attempts (r=g=b=0, temp pointer, assign-in-if) were not repeated; the body read against the whole original
  0x479d00..0x479def corresponds (mem_Alloc 0x20000, lim = 0xff - add, the 0x10000 dd_SplitRGB loop, the three
  clamps, dd_RGB, return 1/0).
- d2_SetColourFormat (0x479df0), d2_FreeLightTable (0x479ee0), d2_SetLineTable (0x479f10): byte + readable
  through the renames below. Evidence: the sprintf "HighBitMask:%i rShift:%i ...", the d2.c compare of
  d2_PaletteDepth with the picture header's bpp and the conversion call, the 0x479ee0 condition that frees the
  table only when d2_LightTableExternal == 0, and the callers' tab / lines row tables (d2_SetLineTable writes
  shp->lineoff .. + h*4).
- d2_Unk479f40 -> match-deep: the recorded strength-reduction case (the original computes the row pointer from
  scratch every row, ours runs a pointer + a [pitch*3] slot); the numbers are unchanged since round 19
  (1159/1328 bytes, 679 code hunks). Read again against 0x479f40..0x47a45f: the 24 bit pixel walk, the bounds
  scan, the key compare, the segment runs and the d2_SetLineTable upload correspond.
- Renames of this unit: d2_Unk479d00 -> d2_MakeLightTable, d2_Unk479df0 -> d2_SetColourFormat,
  d2_Unk479ee0 -> d2_FreeLightTable, d2_Unk479f10 -> d2_SetLineTable, d2_Unk6f0450 -> d2_ColourFormatSet
  (write-only in the C sources), d2_Unkf8a11c -> d2_HighBitMask, d2_Unkf8a630 -> d2_LightTableExternal,
  d2_Unkf8a640 -> d2_HighBitMask32 (the mask in both halves, used by the inline-asm blitters),
  d2_Unkf8a64c -> d2_PaletteDepth. References updated in d2.c, include/d2.h, include/shp.h,
  include/unk_47c5e0.h.

## match-base session 2026-10-24 (base-2 worker, shard 2/6, session 5)

Unit shp.c only (38 functions, 16 -> 24 readable; check count unchanged 2514/4663). Every function below was read
hunk by hunk against its original before the decision; the @deep reasons are in the address comments.

- d2_GrabShape (0x47b220): byte + readable (raised; the colour-depth dispatcher - the mem tags and the s->type
  cases pick the grabber, r->type / r->mode are set on success).
- d2_GrabShape24NoRuns (0x47a460, was d2_Unk47a460) -> match-deep: both scan loops strength-reduce the per-row
  pixel pointer in our compile (hoisted (pitch&0xffff)*y, pitch*3 in a slot, p advanced by it) while the
  original recomputes pix + ((y+r)*(pitch&0xffff)+x)*3 with an imul every row (0x47a4c2..e5, 0x47a623..45);
  frame 0x1c vs 0x14 follows from it. Tried and byte-identical: fewer locals (w1/h1 removed), comma-increment
  inner loop, explicit mask removed. The reset test, the bbox and header stores, the marker loop, the two
  rep-movs copies and the tail correspond.
- d2_GrabShape16Runs (0x47a7d0) -> match-deep: the same strength reduction plus the bounds homes (original
  maxx/maxy in the slots 0x10/0x14, minx/miny in esi/edi; ours maxx=edi, minx=edx, miny=esi) and the run-loop
  base/base+x slots; 466 code hunks, 771/848 bytes. The bbox reset, header stores, dd_RGB key, run skip/count,
  seg[0] *= 2, memcpy, npix/size updates, line table and tail correspond.
- d2_GrabShape16NoRuns (0x47ab30) -> match-deep: our pixel scan keeps p in a stack slot (load/add/store per
  pixel) where the original keeps it in edx; bounds homes differ; frame 0x10 vs 0x8, 544 vs 528 bytes. Tried
  without a byte change (493/544, 319 hunks): unsigned int bounds with (int)(x & 0xffff) compares (the
  original's dword stores - kept, it is the better-typed form), declaration orders, the tail counter on w
  instead of i. The header store order, the unused dd_RGB call, the row copy and the tail correspond.
- d2_GrabShape8Runs (0x47ad40) -> match-deep: the tail differs in codegen - ours keeps row/col as pointers, the
  original recomputes src[maxy*pitch + x + col] in the skip run and keeps the row base in a slot; 527 code
  hunks, 997/1184 bytes, frame 0xda0 on both sides. Every meaning read and corresponding, including the odd
  transparent-palette loop (src[i], inner j unused - the original really does this) and the marker loop.
- d2_ShpBlit16Scaled (0x47b350) -> match-deep: codegen-only homes (ours spills line into the s parameter slot,
  acc into x and row into a local; original keeps d/m/px elsewhere); 345 code hunks, 664/752 bytes, frame 0x18
  both sides. The acc arithmetic was already right in effect: (*run >> 1 + acc) % n compiles to the original's
  inc ecx + shr eax,cl; written explicitly as acc++; acc = (*run >> acc) % n (byte-neutral).
- d2_ShpBlit16ScaledLit (0x47b640) -> match-deep: the light-table twin, same homes (332 code hunks, 619/720).
- d2_ShpBlit16HalfDark (0x47b900) -> match-deep: frame 0x18 vs 0x1c and register homes; the grey formula
  (r + b) * 0.2f + g * 0.59f, dd_SplitRGB / dd_RGB, the run chain and line += pitch correspond (186 hunks).
- d2_ShpBlit16Rows (0x47ba60) -> match-deep: the six clipped / unclipped x mode 0/2/3 loop copies compile with
  other register and slot homes (402 hunks incl. 12 regs); the clip rectangle logic, mode 2 (*d >> 1 &
  d2_HighBitMask), mode 3 (d2_ShpLightTable[*d]) and the d2_ShpH / d2_ShpW stores correspond.

Renames of this session (all project-wide): see the report. Next session: `uv run ledger grade next`
(shp.c next: d2_Unk47bd70, 14 code hunks - the closest; readability needs a name and d2_Unk47cf00 /
d2_Unk47d210; then d2_Unk47bec0, d2_Unk47c0e0 and the bank functions).

## match-base session 2026-10-24 (base-5 worker, shard 5/6)
- Readability renames requested from gfx.c (references updated here): d2_Unk47c6f0 -> d2_DrawShape,
  d2_Unk47c830 -> d2_DrawShapeScaled, d2_Unk47cb40 -> d2_SetClipRect, d2_Unk47cdb0 -> d2_DrawAnim,
  d2_Unk47cd20 -> d2_StartAnim, d2_Unk47ce30 -> d2_FreeAnim, d2_Unkf8a520 -> d2_Anims,
  d2_Unk6f0458 / d2_Unk5fed7c -> d2_ClipLeft / d2_ClipRight. Evidence in notes/units/gfx.md (match-base
  2026-10-24).

## match-base 2026-10-24 (base-2 worker, shard 2/5)

The unit's base queue is empty (was 22 items). Byte + readable now: d2_ShpBlit (0x47bd70), d2_ShpConvertTo16
(0x47c3d0), d2_BankSetPalette (0x47c5e0), d2_BankAppendShape (0x47c610), d2_DrawShapeHalfDark (0x47c770),
d2_DrawShapeMode2 (0x47c970), d2_SetMode5Colour (0x47ca10), d2_DrawShapeMode5 (0x47ca40), d2_DrawShapeMode3
(0x47cae0), d2_GetFrame (0x47cb70), d2_StepAnims (0x47cb90), d2_FillRect (0x47ce60), d2_SetBlitParams
(0x47ced0), d2_ShpBlit16ClipY (0x47cf00), d2_ShpBlit16ClipXY (0x47d210); d2_DrawShape became readable through
the d2_ShpBlit rename.

- d2_ShpBlit 0x47bd70 matched (336/336): the head hunk is the clip test. The original loads `s->w` into edx
  *before* the branch and sums with `lea edi,[edx+ecx]`; with `w = s->w;` as a local before
  `if (x <= d2_ClipRight && w + x >= d2_ClipLeft) { right = w + x; ... }` the whole function matches.
  Operand order alone, `(right = ...)` inside the &&, hoisting the assignment and nested ifs all leave the
  sum made in the wrong register / before the branch (notes/vc6-matching.md).
- d2_ShpConvert24To16 0x47c0e0: one meaning fix, `if (src[1] != 0)` -> `if (0 < src[1])` (unsigned test gives
  the original's `cmp/jbe` instead of `test/je`); 41 -> 39 code hunks. The remaining hunks are scheduling and
  the `pix+1` addressing form (listed in the @deep tag).
- Names: d2_Unk47bd70 -> d2_ShpBlit, d2_Unk47cf00 -> d2_ShpBlit16ClipY, d2_Unk47d210 -> d2_ShpBlit16ClipXY,
  d2_Unk47bec0 -> d2_ShpConvert8To16 (the error string "shp_Convert8To16: Converting of NoReadAndSkip Shapes
  not surported"), d2_Unk47c0e0 -> d2_ShpConvert24To16, d2_Unk47c3d0 -> d2_ShpConvertTo16, d2_Unk47c410 ->
  d2_BankConvertTo16, d2_Unk47c5e0 -> d2_BankSetPalette, d2_Unk47c610 -> d2_BankAppendShape, d2_Unk47c770 ->
  d2_DrawShapeHalfDark, d2_Unk47c970 -> d2_DrawShapeMode2, d2_Unk47ca10 -> d2_SetMode5Colour (with d2_Unkf8a638
  -> d2_Mode5Colour), d2_Unk47ca40 -> d2_DrawShapeMode5, d2_Unk47cae0 -> d2_DrawShapeMode3, d2_Unk47cb70 ->
  d2_GetFrame (d2.h's own "frame i of a picture"), d2_Unk47cb90 -> d2_StepAnims (with d2_Unk6f0464 ->
  d2_LastAnimTick), d2_Unk47ce60 -> d2_FillRect, d2_Unk47ced0 -> d2_SetBlitParams (with d2_Unkf8a63c ->
  d2_BlitH, d2_Unkf8a648 -> d2_BlitN), d2_Unkf8a108 / 10c / 110 -> d2_ShpX / d2_ShpY / d2_ShpRowX (the running
  x compared with the clip limits, the y, the row start). The asm labels of the two Clip functions are named
  LclipY_* / LclipXY_* by their mode block (mode 0 copy, 1 half dark + light table, 2 darken, 3 light table,
  4 add, 5 one colour) and loop position (row / run / pixel / oddDone / pair / nextRun).
- D2BANK: pad2c[4] -> `unsigned short w, h` (the widest / highest shape added, d2_BankAppendShape writes
  them), which clears that function's Q4.
- Escalated to match-deep (reasons in the @deep tags): d2_ShpConvert8To16, d2_ShpConvert24To16,
  d2_BankConvertTo16, d2_StartAnim, d2_DrawShapeScaled.

## match-base 2026-10-31 (opus3)
- d2_ShpBlit16Scaled: the setup (cp / line / run, the four clip early-outs and h clipped to ClipBottom) was
  missing from our source since the migration (the REA_FRAME_DUMP block sat where it was); reconstructed from
  the original. Prototype in shp.h: `int y`, `unsigned char n`.
- d2_GrabShape8Runs: the bounding-box scan re-reads the pixel in each of the four tests; the marker scan reads
  an unsigned char c and stores the box before clearing p[j]; the run loop uses i (int) / j = y + i, an unsigned
  run pointer recomputed after every size change, the literal-run test on src directly, and adds run[1] in
  both arms of `if (run[1] > 0)`.
- The address comments of both functions had a cut-off @deep reason left in them (the clear in 15d3c28 removed
  the tag only up to the first parenthesis); cleaned.
