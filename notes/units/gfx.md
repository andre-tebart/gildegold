# gfx.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\Units\d2.v1\GFX.C (tu_anchors.csv, refs 0x478ee1 / 0x4790ae inside 0x478cc0 / 0x479020). Round 20 (d2t): .data from 0x5febc8; d2_Unk478320 matches only with d2_Unk476c90 above it (unk_478320.c joined); __LINE__ 0x43f in 0x478cc0 falls at line ~1134 of the joined file, so GFX.C covers the TU from ~0x476c00. Layout 2026-10-22: unk_479610.c (0x479610..0x479d00: blits / pixels / clip, no data, codegen neutral) joined (guess, byte-checked); the tustrings overlap with shp.c is only "SHAPBANK" / "BANK" 0x5febb8 (used by shp 0x47c610, a pooled literal at the d2 / gfx seam)
- Boundaries: separated (plausible): start: d2.c boundary in (0x476b70, 0x476b90] (see d2.c) (0x476b90 itself moved in: the fade table family 0x476b90 / c00 / c70 / c90, layout 2026-10-22) unproven (layout 2026-10-22: .bss is COMMON, no lead); end: shp.c (proven .data start 0x5fed74)
- Range then: 0x476b90..0x479d00
- Was: unk_476c00.c + unk_478320.c (round 20), unk_479610.c (layout 2026-10-22)

## Open work
- d2 tail after round 20 (d2t done: TU layout, joins, D2SURF / D2SHP, d2.h prototypes; notes/units/d2.md): nonmatching in gfx.c / unk_479610.c / shp.c, waiting for the d2 cleanup: d2_Unk477000 (mouse shorts in di / bx), 477ba0, 4798f0 (surf / bpp registers), 479bf0 (12, r / g in edi / ebp), 47c410 (17, size in edx vs ecx), 47c0e0 (303), 478bd0, 478c50, 478cc0, 4794c0, 478a00, 479150, 4784c0, 479d00, 479f40, 47a460, 47a7d0, 47bd70, 47b900, 47bec0, 47ba60, 47b350, 47b640, 47ab30, 47ad40; round 20 d2u: 47c830 (235), 47cd20 (74); 47d210 / 47d710 differ only in 0xcc padding (not fixable, TU end). 0x476860..0x476c70 (d2.c or gfx.c?) unproven (0x479610..0x479d00 joined into gfx.c, layout 2026-10-22); unk_47ab30.h-era C4028 on the grabbers' pitch parameter (unsigned int definition vs unsigned short prototype, same bytes).

## d2p report (round 19, scope 0x476c00..0x478320) [d2p]

Source: `src/game/unk_476c00.c`, header `include/unk_476c00.h` (includes d2.h read-only). Same TU as d2.c (D2.C of
d2.v1, join pending). The scope ends with 0x477ed0..0x478320; GFX.C of the same directory starts at 0x478ee1, so
the TU goes on past the scope end (0x478320 onwards is not mine).

### What the code is
The item list next to the mouse pointer (6 entries id / amount at 0xf8ab80, drawn with the pointer picture, its
surfaces made / freed at 0x477320 / 0x477420), hover and scroll wheel handling of windows (0x477000), editing of an
input field (0x4774a0 mouse drag value, 0x477530 end of editing, 0x477590 keyboard, 0x477a70 mouse drag,
0x477ba0 slider objects, 0x477ed0 the per frame driver that calls them).

### Report
Nonmatching (both drafted faithfully, layout of the control flow is right, register / stack slot allocation is not):
- 0x477000 d2_Unk477000 (784 bytes, 91 asm lines): the original keeps the mouse x / y (shorts) in di / bx, loaded
  before the first branch and reloaded after the di_ClearEvents call (CSE of the globals). Mine loads them later.
  Making them `short` locals (assigned at the top) gets it to 71 asm lines but is not faithful (no reload).
  The 3 windows blocks at the end are the same apart from register numbers.
- 0x477ba0 d2_Unk477ba0 (800 bytes, 86 asm lines, FPU slider value): the original spills max / min / len / value2 /
  range / flags to 6 stack slots and uses edi from the entry (`push edi` at the top); mine keeps max in ebx.
  All 120 orders of the five field loads were tried (no change).

### Shared headers
Nothing appended to d2.h or any other shared header. include/unk_476c00.h renames d2.h's `d2_Unk47c830` (it returns
int here: the result is tested) and `d2_Unk47cd20` (here `short x, short y`: the caller pushes the 16-bit loads
`mov cx,[mouse x]; push ecx`) with the #define trick of unk_475ff0.h. At the join d2.h needs `int d2_Unk47c830(...)`
and the short parameters of d2_Unk47cd20 (check d2.c's callers at line 1552, 1826..1880 first: they were matched
with the int prototypes).
Globals are extern only (not defined), like d2_Unk6ed9dc: 0x5febc8 (-1), 0x5febcc (-7), 0x5febdc, 0x5febe0,
0x5febe4, 0x5febe8, 0x5febec (all -1) are initialised .data (0x5febd0, d4, d8 = -1 too, not used in this scope);
0x6ed9c4..0x6ed9f4 are .bss; 0xf8a650..0xf8ab68 are COMMON. `d2_Unk6ed9d0` is a short (word store, byte read).
di_MaxY (0x5f323c) / di_MinY (0x6310b4) are di_input.c's, declared extern in my header.

### Types found (not in d2.h)
- D2WIN: 0x3a8 btnup, 0x3ac btndown (scroll buttons), 0x250 / 0x260 / 0x264 as in d2.h. D2OBJ::unk1e0 of a scroll
  button = index of its window.
- D2INPUT flags: 1 text cursor shown, 2 number, 0x10 list of strings (unk148..150 = first / count / current),
  0x40 value -1 allowed (empty), 0x80 drag changes the limits di_MaxY / di_MinY.
- DIMOUSEST fields used: ldown 0x1467390, lhold 94, lclick 98, lextra 9c, rup a0, rhold a8, wdown c0, wup c4.
  kb_Down[0x36] / [0x2a] are the shift keys.
- Item list: `D2SLOT { int, int, void *surf }` at 0xf8aa90 (256 square) and 0xf8aaa0[3] (128 square); 0xf8aadc a 64
  square surface.

### VC6 lessons
- `x->f += call()`: the address of the lvalue is computed before the call (`lea esi,[edx+0x12c]` kept over the
  call). Writing `x->f = x->f + step` after a separate `step = call()` reloads.
- A global that the original re-reads everywhere (d2_Unk6ed9dc) must stay a macro / global read: a local copy
  `c = D2CUR` moves the register allocation (esi instead of edx for the cached pointer). 477ed0 went from 539 to 0
  differing bytes by removing the local.
- Source order of `case` blocks is the block layout: `case 0xe, 0x1c (+ the empty cases after it), 0xf, default` here.
  Duplicated tail code in two branches (a macro) is not merged by VC6 but is what the original has.
- Index loops (`for (i = 0; i < 6; i++) ... a[i]`) end with a signed `jl`, pointer loops with `jb`: the original
  used index loops. `while (1) { if (i >= n) break; ...; i++; }` is the unrotated (jmp back to the test) loop; a plain
  `for (; i < n; i++)` with i = 0 before it folds the entry test (`cmp word [..],0`).
- An argument that is a call (`d2_GetPic(id)`) is evaluated before the other pushes and its pop merges with the
  outer call's only if it is assigned to a local first.
- `(char)in->flags & 0x40` (flags is a 32-bit field) gives the original's `mov bl,[..]; test bl,0x40` when no byte
  register is free; `char fl = (char)flags; if (fl & 0x80)` gives `test al,al; jns`.
- `if (A || B || C) {X} else {Y}` with X containing a call that appears twice: write the call textually twice (nested
  ifs `if (A && B) { if (c != -1) X else Y } else X`); VC6 keeps one copy per path. The inner test order decides which
  block is out of line.
- `val - step * ((y1 - y0) / 4)` becomes `neg; imul; add` in a function that stores `in->value` after; storing
  `in->value = val - step * q;` first and re-reading `val = in->value` gives the original `sub` (4774a0).
- Local `int a = 6, b = 6` plus x/y modified in place (`x = mouse + x`) changes the order of the first loads:
  separate locals `px = mouse + x` keep the parameters in edx / ecx (476f00).
- The link gate (uv run link) caught a swapped pair of stores in 477a70: statement order variants that match
  byte-wise can still load / store the two globals in the other order (loads of di_MinY and di_MaxY). Always run
  `uv run link` after a `uv run try` `--keep`.

## d2q: 0x478320..0x479610 (round 19) [d2q]

### Report
File src/game/unk_478320.c + include/unk_478320.h. d2.h / d2.c not touched. No shared-header additions.

TU boundary: the file mixes the end of D2.C (0x478320..0x478bd0: input focus, mouse handling, hit rectangles, bmp
reading) and GFX.C of d2.v1 (leaked path D:\develop\Units\d2.v1\GFX.C, first referenced at 0x478ee1, so GFX.C
starts at or before 0x478cc0; 0x478b50/0x478bd0/0x478c50 (bmp -> surface, rgb <-> 16 bit) probably already belong
to it). The scope ends at 0x479610 (d2_Unk479610, next blit, belongs to a following scope). Suggested name for the
GFX part: gfx.c (Names in notes/tus.md: GFX.C of d2.v1); the d2 part joins d2.c.

Other functions: 478870 (copy string to f8aae0), 478890 (add hit rectangle), 478930 (rgb compare), 478960 (set rgb),
478980 (bmp size), 478b50 (surface from bmp), 478fc0 (free surface), 479020 (lock), 4790e0 (unlock),
479120 (GetSurfaceDesc), 479480 (blit without key).
Nonmatching: 
- 478320 d2_Unk478320 (147/432): the ONLY difference is that the original knows d2_Unk476c90 (0x476c90, counts
  running fades) leaves edx alone, so it keeps edx across the call. With a copy of 476c90 defined above it
  (`uv run try` variant) it matches. It will match in the joined d2.c.
- 4784c0 d2_Unk4784c0 (602/896): structure right (swapped branches, comma-expression call argument, do-while radio
  loop); original has no hoisted zero register (`test` / `mov esi,[mem]; test esi,esi`), ours keeps a zero in edi
  and uses `cmp [mem],edi`. Not solved.
- 4788f0 d2_Unk4788f0 (10/64): only `test eax,eax; jl` vs our `xor ecx,ecx; cmp eax,ecx; jl` (zero hoisted to the
  top). `(i & ~0x7fffffff) == 0` gives `test; js` (1 byte off) but is not faithful.
- 478a00 d2_Unk478a00 (162/336): register allocation (original dst=ebx, i=edi, pad=ebp; ours keeps w*3 in ebx).
- 478bd0 / 478c50 (88/128, 60/112): original keeps the row pointer at src+1 and reads p[-1], p[0], p[1]; VC6 here
  folds the +1 away. Everything else equal.
- 478cc0 d2_Unk478cc0 (47 asm lines): original has `movsx eax,bl; and eax,8` for the flags & 8 (flags stays live
  in ebx), ours narrows to `and ebx,8`; tried char / signed char / unsigned char params, int locals, shift casts.
  Also the epilogue pops come later in the original.
- 479150 d2_Unk479150 (far): register roles dst/src swapped (original dst=ebx, src=ebp, y in edx).
- 4794c0 d2_Unk4794c0 (136/336): original keeps sx dead after `rs.left` store (`add esi,edi` in place); ours uses lea.
  Order of the 8 rect stores is irrelevant (70 orders brute-forced, all identical).

### d2.h corrections needed at the join (d2.h declares them differently; the file renames them around the include)
- void -> int return: d2_Unk478fc0, d2_Unk4790e0 (return 1/0), d2_Unk479120 (returns GetSurfaceDesc == 0),
  d2_Unk479150 (1/0), d2_Unk4794c0 (1/0). d2_Unk479480 is void (ok). Callers in d2.c ignore the result.
- Surface parameters are really GFXSURF * (0x40 bytes, type in unk_478320.h); d2_Unk4794c0's last parameter is a
  LPDIRECTDRAWSURFACE4 (destination), its 5th the GFXSURF source.
- d2_Unk478cc0(GFXSURF *desc, int flags): flags 1 = bits per pixel of the screen, 2 = DirectDraw surface,
  4 = wraps an existing DD surface, 8 = system memory.
- d2_Unk6ed9dc is really a D2INPUT * (the focused input); d2.h has int.
- D2WIN: +0x34 is the current line value and +0x38.. the line table (d2.h lines[] starts at 0x3c).
- New externs (my header): d2_Unkf8a658, f8a65c, f8a670 (D2OBJ *), f8a680 (D2HIT[0x41]), f8aa80, f8aa84 (volatile),
  f8aa9c, f8aac4, f8aacc, f8aad0, f8aad4, f8aad8, f8aae0[], 5febcc, 5febd0, 5febd4, 5febe8, 6ed990, di_Unk6310bc
  (defined in di_input.c); functions d2_Unk476c90, 477ed0, 477000, snd_Unk426fa0.

### VC6 lessons
- `volatile` on a global counter (d2_Unkf8aa84) gives `mov eax,[g]; dec eax; mov [g],eax` instead of `dec [g]`
  and reproduces a re-read the original has after stores through an int pointer.
- Compare direction matters for register choice: `h + y > dst->bottom` (cmp reg,mem; jle) vs
  `dst->bottom < h + y`; the original used the first.
- `unsigned > 0` gives `test; jbe` (not `je`): write `h > 0` for unsigned loop counts.
- A call argument that is an if/else-selected string with side effects (toggle) is a ternary with a comma
  expression: `f(a, c ? (x ^= 1, y = x, "s1") : d ? "s2" : "s3", 1, ..)` gave the original's push order
  (constants first, string pushed in each branch, call shared).
- `x * 3 & 3` with 4 - pad used later: VC6 rewrites it to `(-x) & 3` (since 3x == -x mod 4); the original keeps
  w*3 as a shared subexpression, a named int `w3 = w * 3` brings it back (but then w3 takes a register).
- A swapped branch structure is common here: positive condition first (`if (lclick) {..} else {..}`,
  `if (unk48 != 0) {toggle} else {..}`), `if (f != 0) {body; return 1;} return 0;` for fopen failure.
- Dead abs of a negative-height check: `h = v >> 16; if (h < 0) h = abs(h); d.h = h;` (temp first, then store).
- Struct holding a copy of a descriptor: `*s = *desc;` gives `rep movs` of 0x10 dwords.
- D3DCHECK in a GFX.C function: `#line 1 "D:\develop\Units\d2.v1\GFX.C"` once, then `#line 0x43f` before the
  call; the call text must be written with the original inner spaces (`( dd_DD, &ddsd, &dd_DDSOne, NULL )`).

### Outside my scope
- d2_Unk476c90 (0x476c90) counts running fades: `for (f = d2_Unkf8ab80; f < +6; f++) if (f->id != -1) n++`.
- 0x477ed0 and 0x477000 (called by d2_Unk4784c0) are not in a source file yet.

## Also about this unit
- notes/units/d2.md, section [d2t]

## unk_479610.md (joined into gfx.c, layout 2026-10-22)
## Also about this unit
- notes/units/shp.md, section [d2r]
- notes/units/d2.md, section [d2t]

## match-base 2026-10-23
- Reference updates only (an unk_47d600.c Readability rename): d2_ClipTop / d2_ClipBottom (evidence in
  notes/units/unk_47d600.md).

## match-base session 2026-10-24 (base-2 worker, shard 2/6)
- Reference updates only (shp.c Readability renames): d2_Unk478930 -> d2_CompareColour and d2_Unk478960 ->
  d2_SetColour (both byte + readable now; evidence: the two functions' bodies and the shp.c caller -
  0 = equal colour), d2_Unk6f0458 -> d2_ClipLeft and d2_Unk5fed7c -> d2_ClipRight (the clip rectangle that
  d2.c sets as clipx0 / clipx1 next to ClipTop / ClipBottom).

## match-base session 2026-10-24 (base-5 worker, shard 5/6)
Raised to byte + readable (byte already, readability by renames; full build, link and ledger check clean):
d2_AddPointerItem (0x476b90), d2_SetPointerItem (0x476c00), d2_ClearPointerItems (0x476c70),
d2_CountPointerItems (0x476c90), d2_DrawPointer (0x476cb0), d2_DrawMousePointer (0x476f00),
d2_SetPointerSprite (0x476f80), d2_DragInputValue (0x4774a0), d2_EndInputEdit (0x477530),
d2_InputKey (0x477590), d2_DragInput (0x477a70).
- d2_DrawPointer: the raw `*(int *)(surf + 0x34)` is D2SURF::locked (0x34); with it the Q4 rule passes and
  the bytes stay (the `hold` of the original is "the caller held the surface locked, do not unlock").
Escalated: d2_Unk477000 (B3 151 code hunks: the original caches the mouse shorts d2_MouseX/Y in di/bx at
entry, sign-extends with movsx and re-reads both after di_ClearEvents; ours keeps dword loads/uses; the flow,
stores and branches read the same. The old note's short-locals attempt loses the reload), d2_Unk477ba0 (B3
134 code + 4 slots: six-slot spill of min/max/flags/len/value2/range + edi from entry; all 120 field-load
orders tried in the old note), d2_CreateItemListSurfaces (0x477320) and d2_FreeItemListSurfaces (0x477420)
(Q2 only: D2SLOT.unk00/unk04 are written -1 only and read by no drafted function; no evidence for a role).
Evidence for the names (all project-wide; references in cm.c, sv.c, rt.c, main.c, f3init.c, unk_5d33b0.c,
unk_575860.c, unk_58bbd0.c, shp.c updated):
- the item list: the unit's own header ("item list above the mouse cursor") and the draw code; the entries are
  item picture ids (+0xdf in unk_58bbd0.c) with amounts, 6 of them, laid out 3 per row next to the pointer.
- d2_Unk6ed9cc / d2_Unk5fe750: "_MOUSE_CURSOR" data / its picture index; d2_Unk6ed9d0: the cursor look
  (0x..0x25, cm.c 0x476b.. sets it by the selected characters' kind); d2_Unk5febc8: pointer sprite (an index
  into the animation array); d2_Unk6ed9d4: set during cutscene playback (cm.c ai_Unk4d1e00 / 4e01e0 / 4e52..),
  suppresses the pointer drawing.
- d2_Unk6f0458 / d2_Unk5fed7c: the clip rectangle's x0 / x1 (shp.c 0x47cb40 stores them from its x0 / x1;
  d2.c uses them as the left / right limit and copies them to D2OBJ.clipx0/1), not "clip left / width of the
  screen": the old d2.h comments were widened to clip left / right.
- d2_Unk5fe718 / d2_Unk5fe73c: the font d2_Unk476030 selected (it stores 5fe718) / the default font;
  d2_Unkf8f084: the height of the selected font (5fe718 is tested against -1 as "no font").
- d2_Unk5febe4: set -1 when the editing ends and restores di_MaxY / di_MinY from d2_Unkf8a664 / f8a66c (saved
  at the slider drag start 0x477a70); nothing in the drafts sets it to 1 (yet).
- D2OBJ: unk2c -> highlighted (d2.h's own comment; the button draw picks colour 10 / frame 3), unk38 ->
  disabled (0x475aa0 "marks ... disabled (on = 0) or enabled", o->disabled = (on == 0); it gates the mouse hit
  tests). D2INPUT: unk1c -> minval (d2_InputStep uses maxval - minval as the range; 0x4774a0 clamps at it),
  unk20 -> frameY (d2.h comment).
- On the old "Outside my scope" line: 0x476c90 does not count running fades, it counts the pointer item
  entries (id != -1 in d2_PointerItems); 0x477ed0 and 0x477000 are in this file now.
Renames: see the session report; the d2_ names are canonical (the ai_Unk placeholders of the other units were
the same symbols, renamed with every reference).
Next base item: d2_Unk477ed0.

## match-base 2026-10-24 (base-2 worker, shard 2/5)

Byte + readable now: d2_UpdateInput (0x477ed0), d2_FocusInput (0x478320), d2_AddHitRect (0x478890),
d2_RemoveHitRect (0x4788f0), d2_GetBmpSize (0x478980).
- d2_UpdateInput 0x477ed0 was byte all along (the old note's "539 -> 0" trick); the work was Q1 / Q2. Names:
  d2_Unk477ed0 -> d2_UpdateInput, d2_Unk5fe710 -> d2_PointerObj (the object under the pointer, set by
  0x46e460 and every hit path), d2_Unk477ba0 -> d2_SliderInput, d2_Unk478870 -> d2_CopyCaption (with
  d2_Unkf8aae0 -> d2_CaptionCopy), d2_Unkf8a660 -> d2_InputDragOffsetY (di_MaxY - y at the focus click),
  d2_Unkf8ab60 -> d2_InputDragValue (the value at the click), d2_Unkf8ab68 -> d2_InputDragReleased (0 on the
  click, 1 when the button is up); D2INPUT.unk148 / unk14c / unk150 -> firstTextId / currentTextId / textCount
  (d2.c 0x46f.. the choice list: args[argi], args[argi+1], num; gfx.c cycles currentTextId through
  firstTextId .. firstTextId + textCount - 1).
- Hit table: d2_Unkf8a680 -> d2_HitRects, d2_Unkf8aa84 -> d2_HitCount (the stack counter of d2_AddHitRect /
  d2_RemoveHitRect), d2_Unk5febd4 -> d2_HitUnderPointer, d2_Unk5febd0 -> d2_HitClicked (0x478c.. sets both per
  frame: -1, then the first used rect the mouse is in, the clicked one only on lclick).
- d2_GetBmpSize (0x478980) from its own comment (width | height << 16 of a 24 bit bmp).
- d2_FocusInput 0x478320: the old "147/432" note was from the unjoined file; it is byte in the unit now.
- Escalated: d2_MouseUpdate (0x4784c0, was d2_Unk4784c0) -> match-deep with the behaviour reason and the
  readability debt (Q2 mouse / hit identifiers, 2 Q4 casts, Q7 d2_Unk475cb0 / d3_Unk44ee40).
Next base item: d2_Unk478a00.

## match-base 2026-10-25 (base-5 worker, shard 5/6)

Raised to readable (9; byte unchanged): d2_ReadBmpPixels (0x478a00), d2_LoadBmpSurface (0x478b50),
d2_ConvertRgbTo16 (0x478bd0), d2_Convert16ToRgb (0x478c50), d2_MakeSurface (0x478cc0), d2_GetSurfaceDesc
(0x479120), d2_BlitPartMode (0x479150), d2_BlitPartToDD (0x4794c0); d2_FreeSurface (0x478fc0) and
d2_BlitPart (0x479480) became byte + readable as side effects of the Q2 field / callee names.
- Escalated to match-deep, all with the reason in the @deep tag:
  - d2_ReadBmpPixels, d2_ConvertRgbTo16, d2_Convert16ToRgb: the +1 row pointer. The original keeps
    lea esi,[edx+0x1] and reads p[-1]/p[0]/p[1]; ours rebases p to the base and folds the +1 into the
    displacements ([esi+2]/[esi+1]/[esi]). `rp = src; rp++;` changes nothing, the fold stays. In
    0x478c50 the same fold also moves rp into the dead dst parameter slot instead of the dead w slot.
  - d2_MakeSurface: the ddsCaps expression. The original sign-extends the flags byte (movsx eax,bl;
    and eax,8; shl eax,8; or al,0x40), ours folds the (char) cast into and ebx,8. A signed char local
    (f = (char)flags or f = flags) is propagated away; an unsigned int parameter changes nothing; a
    signed char parameter gives movsx but a 0x108 frame, byte reloads and REGRESSED d2_Unk479780.
    The tail (top=0 before the pitch store, early pops) follows the same allocation.
  - d2_BlitPartMode: far draft, 223 code hunks. The original keeps dst=ebx, src=ebp, y=edx and hoists
    the sy/top load; ours swaps the roles and the clip-block scheduling. The dstLocked branch polarity
    fix (original `if (dstLocked != 0) Lock else Unlock`) is in (225 -> 223 hunks).
  - d2_BlitPartToDD: the RECT store scheme. The original stores rs.left/rs.top early and adds sx/sy in
    place (add esi,edi); ours recomputes with lea. The d2q note's 70 store orders stay identical.
- Names (project-wide): d2_Unk478a00 -> d2_ReadBmpPixels, d2_Unk478b50 -> d2_LoadBmpSurface, d2_Unk478bd0 ->
  d2_ConvertRgbTo16, d2_Unk478c50 -> d2_Convert16ToRgb, d2_Unk479120 -> d2_GetSurfaceDesc, d2_Unk479150 ->
  d2_BlitPartMode, d2_Unk4794c0 -> d2_BlitPartToDD; D2SURF.unk38 -> external, D2SURF.unk3c -> manualBlit.
  Q3: the DDBLT literals in d2_BlitPartMode / d2_BlitPartToDD are DDBLT_WAIT / DDBLT_WAIT|DDBLT_KEYSRC.
- D2SURF.external: d2_MakeSurface sets it for flags 4 (d2.c makes the screen surface with 4), it skips the
  own-bit alloc and d2_FreeSurface skips the free. D2SURF.manualBlit: set when the hardware Blt returns
  0x80004001, checked to force the memcpy path.
Next base item: d2_Unk479610.

## match-base 2026-10-25 (base-5 worker, shard 5/6, 2nd)

The unit's base queue is empty: 8 items -> 6 byte + readable, 2 escalated. gfx.c stays 35/46 matched
(3222 differing bytes, the 11 escalated functions).
- Byte + readable: d2_BlitPartFromDD (0x479610), d2_ClearSurface (0x4796c0), d2_CopySurface (0x479780),
  d2_ClearRect (0x4797d0), d2_PutPixelRGB (0x4799e0), d2_SetSurfaceClip (0x479cd0). Q3: 0x1000000 -> DDBLT_WAIT
  (0x479610, 0x4794c0's sibling), 0x1000400 -> DDBLT_COLORFILL | DDBLT_WAIT (0x4796c0, 0x4797d0).
- d2_BlitPartFromDD: the reverse of d2_BlitPartToDD - blits a part of a raw DirectDraw surface onto a D2SURF
  (the d2.c texture upload d2_Unk474d90 calls it twice, at (0,0) and at y = t1->size).
- d2_CopySurface / D2SURF.makeFlags: the copy hands the source's make flags to d2_MakeSurface, so the field at
  +0 is the flags the surface was made with (it was commented "kind"; nothing in the exe sets it to anything
  but 0 today, the copy is the only reader).
- d2_ClearRect: for a system-memory surface it clears the whole surface (the rectangle is ignored) - the
  original does the same; the comment says so.
- d2_PutPixelRGB (0x4799e0): r / g / b per depth, 8 bpp the grey average; the 0x479bf0 outline and d2_Plot's
  pen call it (sv.c still has the ai_Unk4799e0 alias of this function - ch/ecmd-style alias debt, not touched).
- Escalated (reasons in the @deep tags): d2_GetPixelColour (0x4798f0) - the whole body is one register
  permutation (original surf=eax, bpp=edx, bytes=ecx; ours surf=ecx, bpp=eax, bytes=edx) plus the
  bits-load / x-in-register scheduling in the 15/16 and 0x18 arms; the index-local and the inline-index
  spellings were tried, identical. d2_DrawRectOutline (0x479bf0) - r -> edi vs g -> edi for two equally used
  parameters (12/224 bytes, 16 instructions); the early-return polarity tried, identical.
Next base item: the unit's base queue is empty; the unit becomes readable when gfx.h is canonical (the
d2.h / rt.h / shp.h / cm.h foreign declarations, `uv run ledger grade` gfx.c).


## match-base 2026-10-25 (base-3 worker, shard 3/6)

Reference updates only, from d2.c's renames: D2WIN.unk250/unk258/unk260 -> scrollRemY/scrollX/scrollStep (gfx.c:257/261, the
two scroll-button repeat checks), D2OBJ.unk1e0 -> winIndex (tw = &d2_Wins[d2_Objs[bd].winIndex]),
d2_Unk472760 -> d2_ScrollWindowBy (gfx.c:258/262/277, the scroll-button calls). No byte change in gfx.c (unitdiff 35/46
matched, 3222 differing bytes before and after).
