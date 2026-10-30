# d2.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d2:d2_LoadObj" allocation tag, "d2_Show:" messages; the TU runs on through unk_4694c0.c and unk_46aa00.c (shared literal pool at 0x5fe700, "Too many objects on window!"), probably D:\develop\Units\d2.v1\D2.C (GFX.C of d2.v1 at 0x478ee1); unk_4694c0.c + unk_46aa00.c joined in round 16 (d2j); unk_46c6c0.c, unk_46d3d0.c, unk_46e710.c (to 0x46f840) are the same TU, join in round 17 (d2k); the TU ends in 0x475bf0..0x476c00 (round 19: defining d2_Unk475bf0 above unk_476c00.c's d2_Unk477a70 / 477ed0 changes them; the original compiler did not see it), so unk_476c00.c.. is another TU (GFX.C path referenced at 0x478ee1 / 0x4790ae); round 20 (d2t): .data per obj shows d2.c's data up to "d2:fadeinfo" (0x4767d0), boundary to gfx.c in (0x4767d0, 0x476c90], 0x476860..0x476c70 unproven (stay in d2.c)
- Boundaries: separated (plausible): end: boundary to gfx.c in (0x476b70, 0x476b90]: 0x4768a0 (to 0x476b10) uses d2's globals 0xfb9064..0xfb9070 and 0x476b10 / 0x476b40 its object / window lists 0xf8f0a0 / 0xf8f120, none of which gfx.c touches, so only the 0x20-byte function 0x476b70 (clears the input state) is undecided, codegen neutral (layout 2026-10-22, blocked-TU pass)
- Range then: 0x4687a0..0x46c6c0
- Was: unk_4687a0.c (round 15)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes txt_views.h, d3.h, err.h, di_input.h, txt.h, dd_vesa.h, d2.h, unk_47c5e0.h, rea_dump.h, data_views.h; keeps 2 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): d2_OpenPicS, d2_MoveObjI.
- d2 nonmatching (notes/units/d2.md [d2j], [d2b], [d2c], [d2d], [d2k], [d2f], [d2l], [d2m], [d2n], [d2o]; notes/units/ai.md [hv1]), all
  in d2.c: d2_DrawSlider (4/4720, was d2_Unk46aa00, @deep since 2026-10-25), d2_DrawPic (10/480, was
  d2_Unk46d040), d2_DrawButton (17/624, was d2_Unk46bf60, @deep since 2026-10-25), unchanged by hv1's 2-3 runs each
  (round 19). Waiting for the whole d2 TU (draft first): d2_DrawInput, d2_Unk46d3d0, d2_CenterObj, d2_Unk46d220, d2_Unk46e710, d2_Unk46ead0,
  d2_Unk46f1a0, d2_Unk46fba0 (notes/units/d2.md: F case, 'i' path), d2_Unk471a90, d2_Unk472390 (stores `win` where
  `w` is meant? notes/units/d2.md), d2_Unk472800, d2_Unk472fd0, d2_Unk4741f0, d2_Unk474e80, d2_Unk4756b0, d2_Unk4764f0,
  d2_Unk4767d0.
- d2.h cleanup (after the d2 TU is written; one d2 worker, no other scope in d2.c / d2.h then): fold the views
  D2WINF, D2PAINTWIN (D2WIN) and D2TEXX (D2TEX) into their types, D2WOP / D2VOP (the byte-offset pointer views,
  the Q4-clean spelling of the old D2WO / D2VO macros) stay where a
  match needs them; drop the unused d2_Strings, d2_Unk63120c, d2_Unk1467382 externs; d2_Wins has 0x60 entries
  (d2.h says 0x61); d2_Unkfb9060..6c look like one 16-byte struct; d2_Unk47ced0 is probably not a function of its
  own (notes/units/d2.md).

## z1 report (scope 0x4687a0..0x4694c0, round 14+) [z1]

TU: `src/game/unk_4687a0.c` + `include/unk_4687a0.h`. Suggested name: **d2_pic.c** (module tag "d2": memory tag
`d2:d2_LoadObj`, message `d2_Show: Memory could not be allocated for objectID(%i):%s`, `Too many objects on window!`;
neighbours: GFX.C anchor `D:\develop\Units\d2.v1\GFX.C` at 0x478ee1, so the unit family is d2.v1). Content: picture
cache of the 2D layer (0x800 entries of 0x54 bytes at 0x6ed918, loaded on demand from a file, oldest dropped at an
8 MB limit), object list (0x2ff x 0x2ec at 0xf8f080), windows (0x61 x 0x3c4 at 0xf8f120), dirty rectangle lists.
The TU continues past 0x4694c0.

### Report
Nonmatching:
- 468a90 d2_Exit: 4 bytes, only the address operand order of the picture loop (`[ebx+ecx]` original, `[ecx+ebx]`
  ours; i in ebx, d2_Pics in ecx in both). Tried ~16: base as int/char*/int*/unsigned, `i + 0x34 + base` /
  `base + i + 0x34`, D2PIC* cast, natural `d2_Pics[i]` form (worse), unsigned i.
- 468c70 d2_RestoreShapes: original loads `mov eax,[buf+0x190]; test eax,eax` and keeps no zero register; ours hoists
  `xor ebx,ebx` (shared by the two `cmp [mem],0`, the loop init and the five stores of 0) and uses a second
  register for the counter. Tried (~14): chained assignments, order of the stores, `!= 0`, nested ifs, early return,
  copies into locals, `(char*)+off` casts, volatile casts, do/while, index form `d2_Shapes[i]` (0x300 entries).
  None changes the zero-register hoist.

### Lessons (for vc6-matching.md)
- A global pointer indexed several times (`d2_Pics[idx].f`) is reloaded after every store/call in the original;
  a local `p = &d2_Pics[idx]` makes the compiler keep it in a register and never matches. Write `d2_Pics[idx].x`
  each time (also `d2_Objs[obj].x`, `d2_Shapes[i].x`, `(*dp)[n].x` with `dp = &d2_Dirty[cur]`).
- Comparison operand order of `cmp` is source order: `if (limit < a)` and `if (a > limit)` differ by swapped
  `cmp` operands. For clamp code the original had `x > W`, `y + h > H`, `x + w > W` (value first), and
  `y + h >= C` / `x + w >= C` in the rectangle test.
- `for (i = 1; i < n; i++)` with a global bound compiles to a guard `cmp` + do/while; Ghidra's `if (1 < n) {...}`
  is only its rendering. Do not add an outer `if`. Early `return 0` after the loop gives the inline epilogue.
- `p->x < 1` and `p->x <= 0` compile differently (`<= 0` gave the original `cmp 1; jge`).
- Register roles swap with which variable is the modified parameter: in d2_GetPicEx the parameter is advanced
  (`idx += base`) and a copy `j = idx` made first receives the final picture index (`j = d2_Pics[idx].base`).
- `a = idx; idx = d2_Pics[idx].base; shift = a - idx;` (read the new value into the old variable) matched where
  `base = ...; shift = idx - base; idx = base` did not.
- A byte local (`unsigned char b`) compared with an int global spilled to a stack slot (4 extra frame bytes);
  reading the byte twice through a struct pointer (`((D2PICHDR *)data)->bpp`) matched.
- `fopen(path ? path : def)` / `if (!path) path = def` both produced the wrong push order; two calls in an
  if/else (`if (path) f = fopen(path,..); else f = fopen(def,..)`) matched.
- `char` global read with `movsx` (d2_PicBase is `char`), a limit compared unsigned (`unsigned int d2_PicLimit`).
- Walking windows: `int *w = &d2_Wins[0].active; do {...; w += sizeof(D2WIN)/4; } while ((int)w < 0xfa5d20);`
  (the loop is a do/while without guard, 96 windows).

### Shared headers
None changed. New header include/unk_4687a0.h (D2PIC, D2OBJ, D2WIN, D2BUF, D2DLG, D2RECT, D2SHAPE, D2FRAME, D2PICHDR).
Unnamed callees are declared in the .c as `d2_UnkXXXXXX` with `/* 0xADDR */` comments (46ca60, 46cac0, 46e710, 46f1a0,
472ce0, 4730e0, 476030, 476b40, 478fc0, 479480, 479ee0, 47c410, 47c6f0, 47cb40, 47cb70); data externs
d2_Unk63120c (page index, 0x63120c), d2_Unk5fed7c/80 (screen size), d2_Unkfb9060..72 (clip rect), etc. Definitions
of d2_Objs (0xf8f080) and d2_Wins (0xf8f120) are not in this TU (resolved by address).
.data of the TU holds d2_PicLimit (0x5fe70c = 8000000), d2_Unk5fe738/5fe73c (-1); the bytes between 0x5fe710 and
0x5fe734 are other variables of the TU that are not defined yet.

### Outside the scope
Next function after the scope is at 0x4694c0 (same TU, d2 family).

## Worker z2: 0x4694c0..0x46aa00 (src/game/unk_4694c0.c, include/unk_4694c0.h) [z2]

### Report
Scope: 8 functions.

| addr | name | status |
|---|---|---|
| 0x4694c0 | d2_DrawInput | nonmatching, 2016 bytes, 7 asm lines differ (see below) |

Names are ours (the exe's own strings give the `d2_` prefix: "d2_OpenInputIcon(): Too many inputs on screen...",
"d2_OpenSlider:Could not create surface..."). Functions: number-input / text-input fields (D2INPUT, 0x160 bytes,
array d2_Inputs[0x80] at 0xfa6020), their window variants (D2WIN, 0x3c4 bytes, d2_Wins at 0xf8f120) and a slider
(D2OBJ, 0x2ec bytes, `d2_Objs` pointer at 0xf8f080).

#### TU
- The TU of the preceding scope (z1, unk_4687a0.c) **does reach into this range**: the literal "Too many objects on
  window!" (0x5fe7ac) is used by 0x468cf0 (z1 range) and by 0x469ee0, 0x46a360, 0x46a960 (here), and VC6 pools equal
  literals per object. The .data string pool is one run: 0x5fe760 "d2_Show: Memory could not be allocated for
  objectID(%i):%s" and 0x5fe79c "d2:d2_LoadObj" (0x468900, z1), then 0x5fe7ac..0x5fe860 (this scope), then
  0x5fe868 "d2:raw" (0x46c290) and 0x5fe870 "d2_CreateRadioGroup: Too many objecthandles..." (0x46c490).
  So the TU runs from 0x4687a0 (right after zcfree) through at least 0x46c490; it does not end at 0x46aa00.
- Suggested TU name: **d2.c** (GUI library, "d2_" function prefix in the error strings, "d2:" allocation tags).
  unk_4687a0.c and unk_4694c0.c (and the following scopes up to its end) should be joined into it.

#### Shared headers
None touched. New: include/unk_4694c0.h (D2INPUT, D2OBJ, D2WIN, D2ICON, D2PIC, D2PART, globals, prototypes of the
unnamed callees d2_Unk<addr>). The globals d2_Objs / d2_Wins / d2_Inputs / d2_Icons / d2_Strings, and the
d2_Unk5fe7xx / d2_Unk6ed9xx values are probably also used by z1's and later scopes: they will want to move into a
shared d2.h when the TU is joined.

#### VC6 lessons (not added to notes/vc6-matching.md, per the rules)
- A global pointer base that is reloaded before every store (`mov edx,[0xf8f080]` each time) is `d2_Objs[obj].f = ..`
  written out per statement; a local `o = &d2_Objs[obj]` keeps it in a register.
- A byte-flag parameter and `(flags & 0x80)`: `unsigned char flags`; the callee's last parameter being `unsigned char`
  makes a global byte pushed as `mov dl,[g]; push edx` (no zero extension): d2_Unk47c6f0/47c970 take `unsigned char`.
- Digit count: `do { if (v <= 0) break; v /= 10; n++; } while (n < 10);` (test at the top, `cmp n,10; jl top`).
- Error block inline + shared `or eax,-1` tail: `if (n >= MAX) { err_Message(..); } else { ...; return obj; }`
  then `return -1`; the other order puts the error call after the epilogue.
- x / y arguments that a callee stores into shorts: declare them `int`, not `short`; with `short` a dead-parameter
  slot was chosen differently (d2_OpenSlider). `short` fields compared as `ax,cx` words come from `max(short, short)`.
- Statement order of an init block (8 stores, a flag byte OR'd with 0x100): brute force over the permutations with a
  generator for `uv run try` found it (d2_OpenInputIcon: 40320 candidates, the `wh*` quarter was enough).
- A value held in a register for the compare but re-read from a stack home at its last use (`mov ebp,[g]; mov
  [esp+0x18],ebp` ... `mov edx,[esp+0x3c]`): `volatile int hv; hv = hi = g;` and later uses of `hv` reproduce it
  (hi stays a plain local for the compare). A plain local or `volatile hi` do not.
- A product used for `fild` and for compares that VC6 stores as its own temp slot: write `in->mult * in->value`
  out at every use instead of a named local (the named local took the slot of the loop counter).

### d2_DrawInput (0x4694c0), left nonmatching
Everything matches except two spots (7 asm lines), both about the operand `o->w`:
1. `bw = o->w - p0->w - p1->w;`: the original loads p0->w (ebx) first and then `movsx ebx,[edi+0x14]`; with `o->w`
   ours loads o->w first, so p0 is reloaded from its stack slot and the frame layout changes (166 asm lines).
   `bw = in->w - p0->w - p1->w` keeps the original order (and the frame), but loads the dword `[esi+0x10]`.
2. The second 47c6f0 call's x (`o->x - p1->w + o->w`): original loads o->x first, ours o->w first (rank; the first
   textual occurrence of o->w as an rvalue is later than o->x's).
Both are probably one cause (the original reads `o->w` in bw textually before o->x is first used).

### Attempts (`uv run try` runs, d2_DrawInput)
1. declaration order of the three arrays / three pointers / ints (36 variants): no effect.
2. `prod` as named local vs CSE (`in->mult * in->value` written 3 times): CSE form fixes the slot (0x10 -> 0xc).
3. `volatile int hi`: fixes the frame slots but reads hi from memory everywhere; worse overall.
4. bw spellings (8): only `in->w - ...` helps (228 -> 95 asm lines).
5. `x0 = o->x + (unsigned)p0->w` fixes the load order of the third call's x (81 -> 69 lines).
6. moving `hi = d2_Unk5fed80` into the `if (unk1e4 != 0)` block fixes the position of the load.
7. hi spill: `hv = hi = global` with a volatile `hv` (70 -> 7 lines); other tries (register, inner block, ?:, dead
   stores, direct global in the 2nd call) did not.
8. o->w forms of bw x 5 statement orders x 4 expression orders (20) and `(o->w = in->w)` forms: all 109-167 lines.

## z3: 0x46aa00..0x46c1d0 (round 15) [z3]

### Report
Scope 0x46aa00..0x46c1d0, 8 functions in src/game/unk_46aa00.c (+ include/unk_46aa00.h).

| address | name | status | what |
|---------|------|--------|------|
| 0x46aa00 | d2_Unk46aa00 | nonmatching 4/4720 | draws a slider (flags 2 horizontal, 1 vertical): track image, coloured bar (flag 0x400), ticks, arrows with hover state, thumb, value / list item text, min / max labels, caption |
| 0x46bc70 | d2_Unk46bc70 | matched | strncpy the widget label (+0x9c, 0x3f) |
| 0x46bca0 | d2_Unk46bca0 | matched | variadic: builds the item list "a\|b\|c\|" (+0xdc) |
| 0x46bd30 | d2_Unk46bd30 | matched | copies item idx of that list (strchr walk) |
| 0x46bdd0 | d2_Unk46bdd0 | matched | sets the slider flags (+0x88) |
| 0x46be00 | d2_Unk46be00 | matched | creates a type-9 widget (slider-like) via 0x46c9f0 |
| 0x46bf10 | d2_Unk46bf10 | matched | the same, relative to a dialog (array 0xf8f120, stride 0x3c4) |
| 0x46bf60 | d2_Unk46bf60 | nonmatching 17/624 | draws a button: caps, tiled middle, caption |

**0x46aa00 is one function**, not several: the code after its first `ret` (0x46b6d8..0x46bc6b) is the
out-of-line `else if (flags & 1)` (vertical) branch of the same frame (same slots, `add esp,0x264`
epilogues, no entry from outside, no padding). Ghidra's 4.7 KB is right.

**TU**: these functions belong to the "d2" GUI module that starts before the scope: the data block
0x5fe700.. ("d2_Show: Memory could not be allocated...", "d2:d2_LoadObj", "d2_OpenInputIcon()...",
"d2_OpenSlider:Could not create surface..." referenced at 0x46a7d9 in z2's scope, "%i%%" / "%i" used here)
is used from 0x46893a (z1) to at least 0x46d600, and the widget array 0xf8f080 (stride 0x2ec) runs through
all of it. The TU continues past 0x46c1d0 (0x46c1d0.. use the same array). Suggested name:
**d2.c** (tag "d2_" / "d2:"; the neighbouring leaked path D:\develop\Units\d2.v1\GFX.C at 0x478ee1 is a
different TU of the same directory, so the original is probably D:\develop\Units\d2.v1\D2.C). unk_46aa00.c
should be joined with z1's / z2's files at round close.

**Names / types for the join**: globals and callees are named `d2_Unk<addr>` (one name per address):
d2_Unkf8f080 (D2WIDGET *, widget array), d2_Unkf8f084 (font height), d2_Unkf8f120 (D2DIALOG[], x/y at +4),
d2_Unk6ed918 (D2KIND *, stride 0x54), d2_Unk5fe710 (focused widget), d2_Unk5fe73c (default font, **int**:
46be00 reads it as a short only because it is stored to a short), d2_Unk5fe718, d2_Unkfa5f08/0c (int pointer
x/y), d2_Unkf8a674/76 (short mouse x/y), d2_Unk6ed968/96c, d2_Unk6ed9dc, d2_Unk6f045c, d2_Unk5fed80,
d2_Unkfb9070/72 (short screen height / width, just before d3_Textures). Callees: d2_Unk468cf0, 468eb0,
4690b0, 46c9f0, 46ca60, 46ea10 (text width), 46f000 (text), 46f1a0 (text in box), 473540, 476030 (font),
479480 (blit part of an image: sx, sy, w, h, img, x, y, surf), 479b60 / 479b90 (pixel, clipped),
47c6f0 / 47cae0 (draw sprite i of a set), 47cb40 (clip rect), 47cb70 (sprite i of a set). z1 / z2 will
have their own names for some of these: unify at the join. D2WIDGET is a partial view (union at 0x7c:
button caption vs slider value/min/max/flags/len/value2/img).

**Shared headers**: none changed. unk_46aa00.h repeats dd_RGB's prototype (identical to dd_vesa.h).

**VC6 lessons**
- A result passed straight into a call (`f(..., dd_RGB(r, g, b), ...)`) pops the inner call's args at
  once; the original's single `add esp,0x1c` needed `c = dd_RGB(r, g, b); f(..., c, ...);`.
- Stack slot sharing follows variables, not roles: one loop counter for two unrelated loops, or one tx/ty
  pair for two branches, merged their homes. Separate variables (`k` for the tick loop, x1/y1 and x2/y2 per
  branch) moved 3000 B of slot differences into place. Three `unsigned char` colour locals took three slots;
  `unsigned char rgb[3]` gave the original frame size (0x264).
- Member load order of a block of `x = w->f;` copies: `value = w->value;` first in the source gave the
  original `flags, len, value` load schedule (a named `value2` local also helped).
- A wrong operand found only by slot mapping: a `fld [slot]` that pointed to pos1, not pos2.
- Unsolved here (both functions): which of two members of the same struct (`w->x`, `w->w`) is loaded
  first in `w->x + w->w + 4` / `w->w - r->w + w->x`. Operand order, parentheses, casts, a temp, dead
  references, a nested struct for x/y/w/h: no change.

**Outside the scope**: 0x46a4c0 (z2) holds the d2_OpenSlider error string. 0x46c9f0 allocates widgets
(called by 46be00).

### d2_Unk46aa00 (0x46aa00) attempts
From 3229 B (first draft, same size) to 4 B: rgb array, `c = dd_RGB()` local, separate `k`, separate
x1/y1/x2/y2, `value` loaded first, `value2` local, colour factor from pos1. No effect: `scale` vs inline
`flen / range`, `flen = len`, int flags, w set first, d per branch, hilo/hihi init forms, swapping i/j.
Left: 46b5d5.. `w->x + w->w + 4` (twice) loads w before x (orig x first); 13 variants (orders,
parentheses, casts, temp, dead refs, nested struct) did nothing.

### d2_Unk46bf60 (0x46bf60) attempts
17/624: `len = w->w - r->w - l->w` subtracts l before r (orig r first) and `w->w - r->w + w->x` loads x
first (orig w). No change from: operand order / parentheses / casts / split statements / dead stores
(rank tie-breaks), declaration order, `*(unsigned short *)` access, nested struct, moving `len` (worse).

## d2j: d2 join + 0x46c1d0..0x46c6c0 (round 16) [d2j]

### Report
**Join**: src/game/unk_4694c0.c and src/game/unk_46aa00.c joined into src/game/d2.c (address order), one
include/d2.h; the two old files and their headers are git rm'd. Every function kept
its bytes. One type and
one name per address:
- D2OBJ (0x2ec, d2_Objs 0xf8f080) = z1 D2OBJ + z2 D2OBJ + z3 D2WIDGET: id, used, pic (was unk08 / kind),
  data (was unk0c / sprites, void *), x/y/w/h (was unk10/unk12), type (unk18), clipx0/clipx1/clipy0/clipy1
  (was x0.. / cx0..), win (was unk30 D2PANEL *), unk40 (D2CTRL *, was z3's D2INPUT), font (unk74), input
  (unk78), union u {text, raw, s.value/min/max/flags/len/value2/unk94/img} (z2's unk7c..unk94 / surf),
  label (z2's unkbc = label[0x20]), list, style, unk1c4/1c8/1e4, hilo/hihi, scale (unk2e4); new unk5c.
- D2PIC (0x54, d2_Pics 0x6ed918) = z1 D2PIC = z2 D2ICON = z3 D2KIND. D2PICHDR (picture data: w/h at 0x2c,
  bpp at 0x34) = z1 D2PICHDR + z2 D2PIC. D2FRAME = z1 D2FRAME = z2 D2PART = z3 D2SPRITE.
- D2WIN (0x3c4, d2_Wins 0xf8f120) = z1 D2WIN + z2 D2WIN + z3 D2DIALOG. D2INPUT = z2 D2INPUT + z1 D2DLG
  (d2_DrawFrame's argument: value 0x12c, obj 0x134; its "y" is D2INPUT.unk20).
- Globals: d2_Objs (was d2_Unkf8f080), d2_Wins (d2_Unkf8f120), d2_Pics (d2_Icons / d2_Unk6ed918),
  d2_ScreenH/W (d2_Unkfb9070/72), d2_PicShift (z2's `unsigned char d2_Unk6ed98c`; it is an int, the byte
  read is the uchar parameter of 0x47c6f0), d2_Unk6ed924..930 are void * (surfaces).
- Callees: d2_GetPic (= d2_Unk4690b0), d2_FindPic (468890), d2_AddToWin (468cf0), d2_AddDirty (468eb0);
  one prototype each for 0x47c6f0 (`int (int, int, void *surf, void *pic, unsigned char)`), 0x47cb70,
  0x46f1a0, 0x479480 etc. in d2.h.

**Fresh scope 0x46c1d0..0x46c6c0.**

| address | name | status | what |
|---------|------|--------|------|
| 0x46c1d0 | d2_CenterObj | nonmatching 116/144 | centres an object (flag 0x5c) on its window / the screen via 0x46cda0 |
| 0x46c260 | d2_SetObjWidth | matched | sets w, re-centres |
| 0x46c290 | d2_OpenRaw | matched | type 0x47 object with its own w*h 16-bit pixels ("d2:raw") |
| 0x46c390 | d2_WinOpenRaw | matched | the same on a window |
| 0x46c3f0 | d2_DrawRaw | matched | copies the pixels to a surface (pitch 0x10, bits 0x1c), clipped in y |
| 0x46c490 | d2_CreateRadioGroup | matched | varargs, "d2_CreateRadioGroup: Too many objecthandles..." |
| 0x46c630 | d2_RadioSelect | matched | calls 0x475cb0(obj, on) for each member |

Radio groups: D2RADIO d2_Radios[8] at 0xfa5aa0 (0x8c each: unk00, n, sel, objs[0x20]).

**Old d2 nonmatching**: d2_Exit matched (dead `base = 0;` at the top flips `[i+base]` to the original
`[ebx+ecx]`), d2_RestoreShapes matched (memset of the shape + natural `d2_Shapes[i]` loop). Still
nonmatching: d2_DrawInput (7 asm lines), d2_Unk46aa00 (4 B), d2_Unk46bf60 (17 B) - the member load-order
problem, see below - and d2_CenterObj.

**TU**: d2.c continues past 0x46c6c0 (0x46c9f0 / 0x46ca60 / 0x46cda0 use d2_Objs; other workers' files).

**Shared headers**: none touched (only include/d2.h, the TU's own).

**VC6 lessons**
- A dead store of a constant to a local at the top of the function (`base = 0;`) is a rank tie-break that also
  flips the operand order of an address `[i+base]` (dead loads `i = w->x;` and dead stores to the other
  locals did nothing in d2_Unk46bf60).
- 5 stores of 0 to a struct kept a zero register (xor ebx,ebx at entry) for the whole function;
  `memset(s, 0, sizeof *s)` gives the original `xor eax,eax` + 5 stores at the site.
- A loop that starts at a computed index: `for (i = top; ...)` with a separate `top` variable (not
  `i = top_expr` then `for (; i < h; ...)`) gave the original registers.
- `f(obj, i == sel)` gives `sete`; the original's two calls with `push 1` / `push 0` came from
  `if (i != sel) f(obj, 0); else f(obj, 1);` (`if (i == sel)` first: 3 B off).
- A local holding a size used twice (`n = w * h * 2` before stores) moved the register; computing it after
  the stores (or inline twice) matched.
- Varargs walked with an index (`objs[j] = va_arg(ap, int)`) matched; a pointer + countdown did not.

### Member load order (d2_DrawInput, d2_Unk46aa00, d2_Unk46bf60)
In all three our build loads the two members of the same struct in the opposite order to the original
(`w->x` / `w->w`), and the original follows the order in which our source happens to be written - but
textual order does not change our code at all. d2_DrawInput: the original's `bw` is computed from `o->w`
(`movsx ebx,[edi+0x14]`), not `in->w`; with `bw = o->w - p0->w - p1->w` that instruction matches but slots
and registers move (1032 B / 166 lines vs 963 B / 7 lines), so the source keeps `in->w`.
Tried without effect on the order (this round, ~60 variants): all operand orders and parenthesizations of the
second frame call (6) x assignment order of o->w / o->x (2) x bw form (3); in->w sum orders, `(short)`,
x0 with / without `(unsigned)` / inline; dead stores of every local at the top (d2_DrawInput 14, d2_Unk46bf60
7 singles + 21 pairs); dead member loads into reused locals (10); `d2_Objs[id].` instead of `w->` (worse);
a frame pointer array `D2FRAME *f[3]` (worse); an `x0 = w->x + l->w` local; `(unsigned)` casts on the
frame widths; short parameters of 0x47c6f0 via a cast (worse); renaming l / r; declaring w/h before x/y
in D2OBJ (anonymous union): no change in any function.

### d2_CenterObj (0x46c1d0) attempts -- nonmatching 116/144
Original keeps obj in ebx, o in esi, o->win in edi (three callee-saved registers) and computes x before
pushing obj / y; ours keeps obj in edx and pushes it (and y) before the cdq of the x computation, o in ecx.
No change from (~25 variants in 5 runs): `d2_Objs[obj].unk5c` test then `o = ...`, a `D2WIN *w` local, an
`x` local per branch, early return / return in the win branch, all 16 mixes of `o->` vs `d2_Objs[obj].`
for the test / w / y / win, `d2_Objs + obj`, operand order of the x sum, negated test (worse), y local
(worse), `hw = o->w / 2` local (worse, 128 B), short parameters of 0x46cda0 via a cast (worse).

## d2b: 0x46c6c0..0x46d3d0 (round 16) [d2b]

### Report
Scope 0x46c6c0..0x46d3d0, 10 functions in src/game/unk_46c6c0.c + include/unk_46c6c0.h
(own copies of the d2 types; names d2_Unk<addr>; join into d2.c pending). The TU continues past 0x46d3d0.

Functions: 0x46c9f0 d2_Unk46c9f0 (object slot alloc), 0x46cac0 d2_Unk46cac0 (picture refcount--),
0x46cb20 d2_Unk46cb20 (object creation), 0x46cd80 d2_Unk46cd80 (thunk), 0x46cda0 d2_Unk46cda0 (move object / window, recursive),
0x46cfd0 d2_Unk46cfd0 (redraw picture + flip).

Nonmatching (4):
- 0x46ca60 d2_Unk46ca60 (picture refcount++): 4/96 B. After the `refs++` store the original reloads d2_Pics into edx and d2_Unk6333cc
  into ecx, we get them swapped (ecx/edx). ~30 variants (pointer local, int* to refs, int arithmetic through (int)d2_Pics,
  K&R, local copies, block scopes, dead stores) give the same bytes.
- 0x46d040 d2_Unk46d040 (draw picture/frame): 10/480 B, only x and y swapped between esi and edi (original: esi = x, edi = y).
  Not changed by short locals for the casts, declaration order, dead uses, if/else order, casting all uses.
- 0x46d220 d2_Unk46d220 (draw caption): 343/432 B, draft. The original keeps `flags` in memory (the dead param slot, `or [esp+1c],imm`)
  and holds the constant 0x20 in ebx (`test bl,al`, `or [mem],ebx`), x in ebp, y in edi; ours enregisters flags and uses immediates.
  Tried: unsigned/volatile flags, struct/array/union flags, flags as the parameter, mask variable, ternary, `flags = flags | c`. No luck.
- 0x46c6c0 d2_Unk46c6c0 (menu keyboard/pointer navigation): only the last block differs: the original reads
  `movsx edx,BYTE [o+0x1c0]; and edx,0x10; shr edx,4` for `d2_Unkf8a668 = ((char)o->style & 0x10U) >> 4` (type 9), ours narrows to
  `mov dl,[..]`. (105/800 B differ, all but that instruction is displacement shift.) Tried int/unsigned/char locals, union char member,
  char/uchar/int bitfields, pointer cast.
  It is a void function (Ghidra's returns are leftover eax values).

TU boundaries: nothing new. The d2 TU runs on past 0x46d3d0.

Shared headers: none touched. New own header include/unk_46c6c0.h (D2PIC, D2WIN, D2OBJ, D2INPUT, D2MENU, D2POS, D2SPRITE, D2SET, D2SET17, D2IMG
and the externs / prototypes of the functions this scope calls). For the join: D2MENU is a new array at 0xfa5aa0 (10 entries of 0x8c bytes,
flags/count/cur/items[32], used by 0x46c6c0 and the already existing 0x46c630); D2POS array d2_Unkf8a52d is 0x11-byte packed entries at
0xf8a52d (x, y shorts); d2_Unk6ed934 is a `char` (movsx); d2_Unk6ed98c is read as dword here (not unsigned char as in unk_4694c0.h).
Callee prototypes that differ from the other d2 files: d2_Unk47cb70(void *set, short i) (no sign extension of a short argument),
d2_Unk479020 returns int, d2_Unk44ee40 takes (x, y, surf, pic) (d3_io_a.h calls it void(void)).
Names found: 0x46ca60 = d2_AddRefcount, 0x46cac0 = d2_SubRefcount (string "d2_SubRefcount: invalid refcount!" at 0x5fe8a0).

### General VC6 lessons
- The d2 TU defines functions K&R style: `int f(x, y, pic) int x; int y; int pic; {...}` while callers/callee see a prototype with `short`s.
  0x46cd80 is exactly that thunk: K&R `short x, short y` params calling a prototyped (short, short, int) function give
  `mov ecx,[esp+8]; mov edx,[esp+4]; mov [esp+8],ecx; mov [esp+4],edx; jmp` (new-style short params give movsx instead, matching
  prototypes give a bare jmp). So the original header declares 0x46cb20 with short x, y and its definition is K&R with int x, y.
- A local copy of an out-parameter value (`p = real; o->pic = p; ... f(p)`) makes VC6 push the argument early (0x46cb20).
- Register choice follows the order of assignment: `oldx = w->x; oldy = w->y;` vs the reverse swaps ebx/ebp (0x46cda0). For
  parameters (0x46d040) this did not help.
- Source order of switch cases decides block layout (0x46d040: 1/4, 0x11, 5/8). Duplicated calls in if/else get their tails
  merged by VC6; a ternary argument does not give the same code. `(char)(a - b)` pushed as a char argument is `sub al,bl; push eax`
  without extension.
- A prototype with a `short` parameter passes a short-typed expression without extension (`mov cx,[..]; push ecx`).
- `for (i = 0x2ff; i >= 0; i--) if (a[i]) { i++; if (i >= 0) goto found; break; } i = 0; found:` reproduces Ghidra's shape of "find last
  used slot + 1, else 0" (0x46c9f0); with the object pointer computed before the loop.
- `int *r = &d2_Pics[pic].refs; if (*r > 0) (*r)--; else err(...)` gives the pointer form of 0x46cac0 (tail call `jmp err_Message` with
  a string pushed in place).
- The parameter `pic` reused as the "base" variable and a `sel` local living in the dead parameter slot (0x46d040).

### Outside scope
- 0x46c630 (d2_Unk46c630(menu, sel)) is called from 0x46c6c0; it belongs to d2j's scope (0x46c1d0..0x46c6c0).

## d2c -- 0x46d3d0..0x46e710 (round 16) [d2c]

### Report

File: src/game/unk_46d3d0.c + include/unk_46d3d0.h (own types, copied/extended from d2.h, unk_4694c0.h,
unk_46aa00.h; no shared header edited). Part of the d2 TU (D:\develop\Units\d2.v1\D2.C), join into d2.c pending.

Functions:
- 0x46e150 d2_Unk46e150 -- flush dirty list, lock surface, draw all (46d3d0), unlock, dd_Flip ("d2_Show"-like)
- 0x46e190 d2_Unk46e190 -- window (type 0x40 object) under a point -> d2_Unk5fe72c
- 0x46e260 d2_Unk46e260 -- hit test through the textures of the 3D views (d2_Unkfb1020[0x30], 0x2ac bytes each)

Nonmatching (2):
- 0x46d3d0 d2_Unk46d3d0 (3.4 KB, one function, jump table at 0x46e0d4 + byte index at 0x46e104): draws every object
  in drawing order (switch on type 1/4/5/8/9/0x11/0x40/0x41/0x43/0x44/0x45/0x47). Only register allocation left in
  the text part of case 5/8: the original keeps the text mode in ebp and reloads x (`mov ebp,[esp+0x30]`, 4 bytes)
  after d2_Unk46f1a0, ours keeps mode in edi; a scratch-register rotation in case 0x40 follows. 93 asm lines.
- 0x46e460 d2_Unk46e460 -- object under the pointer: only `movsx eax,BYTE [eax+0x1c0]` vs our `mov al,..` for
  `((unsigned)(char)o->flags & 0x10) >> 4` (type 9 return). 5 asm lines.

TU boundaries: none inside the scope; 0x46d3d0 is a single function (Ghidra was right). The scope continues
into 0x46e710 (not taken).

Shared-header additions: none. Join notes for the orchestrator:
- d2.c declares `int d2_Unk6ed924` / `void *d2_Unk6ed964` / `int d2_PicShift`; this file uses `void *` surface,
  `D2OBJ **d2_Unk6ed964`. The D2OBJ / D2WIN layouts here have more named fields (0x30 win, 0x40 input,
  0x6c dx/dy, 0x70 mode, 0x7c text, 0x1c0 flags, 0x1cc..0x1e4 text/icon pos and float value; D2WIN 0x0c flags,
  0x264, 0x268, 0x278 frame picture).
- New globals (extern only): 0x5fe714, 0x5fe718, 0x5fe71c (char), 0x5fe72c, 0x5fe730, 0x6ed940, 0x6ed9b8,
  0x6ed9bc, 0x6ed9dc, 0x6f044c/0x6f044e (shorts), 0xf8a528 (17-byte records! `#pragma pack(1)`), 0xf8ef00 (char[]),
  0xfb1020 (D2VIEW[0x30], 0x2ac bytes; base confirmed by xrefs), 0x1467394.
- Literals in the d2 pool: "%i%%" (0x5fe7c8), "_FONT" (0x5fe800). Floats: 0.0f 0x5eb788, 1.0f 0x5eb780,
  1.0 0x5eb8a8, 100.0 0x5eba38, 5.0f 0x5eb988.
- d3_Unk44ee40 (empty function) is called with 4 arguments: declared K&R `void d3_Unk44ee40();` here.

General VC6 lessons:
- A search loop with an early `return o->field` vs `ret-global = ...; break;` then `return global;` changes the
  whole register allocation (46e190: break matched; the original reloads the global at the end).
- A loop over a global struct array that VC6 compiles to a pointer at an inner field with a signed `jl` against
  the end address is an index loop (`for (n = 0; n < 0x30; n++) v = &arr[n];`); a pointer loop gives `jb`.
- `if ((char)x < 0)` gives `test; jge`; the original's `test al,al; jns` came from `x & 0x80`.
- `ret = global = -1;` gives the odd `mov [g],eax; or eax,eax` tail (46e460).
- Two calls on one inner-call result (`d2_Unk47cb70(pic, 4)->h`, `(..., 6)->w`) assigned through one pointer local
  (`part = ...; fy = part->h;`) fixed a register rotation that ran through 1 KB of the function.
- A 17-byte array element (`shl ecx,4; [ecx+eax+base]`) needs a packed struct.

### Attempts

#### d2_Unk46d3d0
- clip order operand swaps / temp `v` / int fx,fy / ushort fx,fy: no gain (ushort much worse).
- while vs for (outer and window loop), index variable for win->parent: no change.
- `part` local for d2_Unk47cb70 results: 156 -> 97 lines (kept).
- `d2_Unk6ed9b8 != o->win` swap: worse after the part fix (reverted).
- mode variable: block scope, unsigned, reuse of fx/fy/yy, split per case: no change.
- text position: inline ternaries / if-else / ty via -1 test: worse.
- d2_GetPic result in a local (pops merge, as in the original): kept.

#### d2_Unk46e460
- `ret = d2_Unk5fe710 = -1;` kept; `o = ..; busy = 0;` order in the 2nd loop kept.
- movsx at 0x1c0: casts (unsigned)(char), (int)(char), (short)(char), `<<24>>24`, `/16`, char local, real char
  member, `>>4 & 1`, unsigned return type: all give `mov al` (or worse).

## d2d (round 16): 0x46e710..0x46f840, text / object code of the d2 2D layer [d2d]

### Report
Source: src/game/unk_46e710.c + include/unk_46e710.h (own copies of the d2 types, names D2DOBJ / D2DWIN / D2SEG /
D2FONT / D2GLYPH / D2TEX so they do not clash with d2.h). The TU does not
continue inside the scope (0x46f840 is the next function start, outside).
Suggested name: the orchestrator joins the file into d2.c (probably D2.C of D:\develop\Units\d2.v1).

Functions: 0x46e990 d2_Unk46e990 (frees / flips the d2 surfaces), 0x46ea10 d2_Unk46ea10 (text width),
0x46f000 d2_Unk46f000 (draws one line of text in font "_FONT"), 0x46f180 d2_Unk46f180 (wrapper around 0x47ca10, the
text color setter).
Nonmatching (3), bytes differing:
- 0x46e710 d2_Unk46e710 (d2_RemoveObj: frees what the object type owns, unlinks it, clears it) 402/656. Code is
  right, register allocation is not (see below).
- 0x46ead0 d2_Unk46ead0 (draws the text pieces collected in 0xf8abe0 as one justified line) 1109/1344, draft from
  the Ghidra output, about 3 runs.
- 0x46f1a0 d2_Unk46f1a0 (word-wrapped text box with background sprite, recursion with surf = 0 measures)
  1357/1696, about 10 runs; draft is semantically complete (hyphenation at '~' / '-', alignment flags 8 / 0x10,
  background 0x20 / 0x80 / 0x100, colored 0x40).
Shared headers: none touched.

### VC6 lessons
- A glyph / char index passed as `unsigned short` parameter gives `xor dx,dx; mov dl,bl; push edx` for an
  `unsigned char` local (0x47cb70 takes (font, unsigned short)); `unsigned char` or `int` parameters give movzx /
  and edx,0xff.
- A pure pass-through wrapper `f(a,b,c){ g(a,b,c); }` is a plain `jmp`; the original's reload-and-restore of the three
  stack slots before the jmp comes from `unsigned char` parameters in BOTH wrapper and callee (46f180 -> 47ca10).
- A parameter that is modified in the function and also needed at its old value ends up as: copy `x0 = x` first (before
  the first test), then modify the parameter itself; the loop counter then lands in the dead parameter slot (46f000).
- `for (i = 0; i < strlen(s); i++)` with unsigned i gives the original's `repnz scasb` per iteration and `jb`.
- An `if/else` whose original asm has the `je` to the second block: write the `!=` branch first (c != ' ' ... else).
- Zero register: the original keeps the zero in edi only until the switch merge (its `rep stos` for the case-0x41
  clear clobbers edi, so zero is not available afterwards and later pointer tests are `test reg,reg` and stores are
  `mov [..],0`). Ours puts the zero in ebp, keeps it to the end and spills `obj` (46e710). Not solved.
- String literal "_FONT" (0x5fe800) is 5 bytes: Ghidra's label `s__FONT_` is the string "_FONT".

### 0x46e710 attempts (about 12 runs)
Case order of the switch must be 0x43, 4, 0x40, 0x41, 0x45, 9, 0x47 (jump table order = block order): fixed.
Window object removal: `for (i = 0; i < n;) if (objs[i++] == obj) break; i--; if (i != n) memmove(...)` gives the
`cmp eax,ecx; jge` loop of the original. `off = obj * 0x2ec` kept in a local (original keeps it in ebp): no change.
Variants that remove zero stores (to see when the zero register disappears): no effect on the layout. A local
`z = 0` used for the compares: folded to the constant. Open: make zero live in edi only.

### 0x46f1a0 / 0x46ead0 notes
46f1a0: original frame 0x638 (lines[16][0x60] plus 0x38), mine 0x634. The original loads x, y, w into esi, ebx, edi
before anything else and spills x0 / y0 (copies) to [esp+0x1c] / [esp+0x30]; `right = x + w` is computed from the
parameter before the word loop. The stores of d2_Unk6ed968 / 6ed96c after the box code are only on the box path
(inside the if). The line copy is a hand-written byte loop (not strcpy). Hyphen search: c = word[k]; if '~' or '-'.
Variants tried: bx/bw/bh initialised at the top, vis order, x0/y0 after memset, strchr in the condition: none moved
the count (1340..1357).
46ead0: original keeps gap / extra / hl in stack slots ([esp+4], [esp+8], [esp+0x18]) and pushes ebx/ebp/esi only
after the `d2_Unk6ed978 != 1` test (edi alone first); seg offset is a running `+0x10c` counter (ebp). The last drawn
char `c` is read uninitialised at the end of the first piece (it lives in the dead parameter slot of the start x).
Structure: 3 draw-loop copies (highlight "$H", glyph "$[" / "$]", normal), same as Ghidra shows.

### Outside the scope
- d2.c declares d2_Unk6ed948 as int and the two other headers declare 0x4690b0 as `int` / `D2PIC *`: this unit needs
  it as `D2FONT *` (pointer, field +0x2e is the line height) - to unify when joined.
- 0x47ca10 sets the text color (calls dd_RGB 0x41e670), 0x47ca40 is an alternative glyph draw used for colored text
  (flag 0x40); neither has source yet.

## d2k: d2 join + 0x46f840..0x46fba0 (round 17) [d2k]

### Report

1. Join: unk_46c6c0.c, unk_46d3d0.c, unk_46e710.c (+ headers) joined into src/game/d2.c / include/d2.h, no byte
   changed (own commit 46d546b). 0x46ca60 = d2_AddRefcount, 0x46cac0 = d2_SubRefcount.
2. Fresh scope 0x46f840..0x46fba0: 0x46f840 d2_Unk46f840 (window scroll step: velocity / speed
   members 0x24c..0x260, scroll bar object 0x3b8, moves the window's objects), 0x46fad0 d2_Unk46fad0 (buttons of a
   window to one width). The d2 TU continues past 0x46fba0 (d2e's scope).
3. Old d2 nonmatching: matched d2_AddRefcount (0x6333cc is d3.h's `volatile d3_Time`, not a new d2 global),
   d2_Unk46c6c0 (if/else for the movsx, plus `sel = d2_Radios[g].sel` loaded again instead of `sel = cur`),
   d2_Unk46e460 (if/else for the movsx; also `d2_Unk5fe714 = d2_Unk5fe710 = o->id` order, link gate).
   Still nonmatching: d2_Unk46d040 10/480 (x/y in esi/edi swapped; ~12 more variants: K&R declaration order,
   short prototype, case order, locals), d2_CenterObj 116/144 (obj in ebx and the late push of obj in the
   original; K&R / short K&R param (111/144), register, early return, id copy: no).

Shared-header changes (d2.h; the other three round-17 d2 workers include it):
- Additions: D2PICHDR11, D2TEX, D2VIEW, D2SND (packed, 0xf8a528), D2SEG; D2FRAME kern/adv; D2SURF w/h;
  D2WIN flags (0x0c), scroll members 0x24c..0x260, visible 0x264, unk268, frame 0x278, unk3b4, unk3b8;
  D2OBJ unk54/58, unk6c/6d/6e (uchar/char), unk70, caption[0x100] 0xbc + capfont 0x1bc (anonymous union with
  label / list), unk1cc, unk1d0, icon / iconx / icony 0x1d4.., unk1e0; ~30 externs, prototypes of the joined
  functions and of their callees (di_SetMousePos, dd_Flip, d3if_DrawSprite, d3_ScreenToUV, d3_Unk44ee40 (K&R),
  tex_Find, d2_Unk471a90, d2_Unk472e00, d2_Unk4759f0, d2_Unk475980, d2_Unk475aa0, d2_Unk476110, d2_Unk476b10,
  d2_Unk479cd0, d2_Unk47c770, d2_Unk47c830, d2_Unk47ca10, d2_Unk47ca40, d2_Unk47cb90, d2_Unk47cd20, d2_Unk47cdb0,
  d2_Unk47ce30), kb_Last (0x1468410, di_input.c).
- Changed (no rename / removal): pads split into the members above (pad0c, pad24c, pad270, pad284, pad54, pad6c,
  pad1cc, pad00 of D2SURF are gone or shorter); d2_Unk47cb70 takes `short i` (was int: 0x46cb20, 0x46ea10,
  0x46f000 need short); d2_Unk479020 returns int (was void); d2_Unk46ca60 / d2_Unk46cac0 are `#define`s for
  d2_AddRefcount / d2_SubRefcount (old names keep compiling).
- d2.c includes di_input.h now (dm_L.x / .y / .lup / .lhold / .lclick / .wdown / .wup replace d2_Unk1467380..
  14673c4). d2_Unk1467382 (round-start d2.h) is dm_L.y and now unused; d2.h's d2_Strings (0x143a1c0) is
  txt.h's txt_Ptrs and d2_Unk63120c is dd_vesa.h's dd_Page (one name per address, for the orchestrator).
- Merge note: d2e / d2f / d2g headers that declare one of the new d2.h externs with another type will clash
  (C2371) after the merge; d2_Unk46cda0 is redeclared with shorts inside d2.c (see below), d2.h keeps int.

General VC6 lessons:
- `movsx r,BYTE; and r,imm; shr r,n` (no narrowing) comes from `if ((char)x & imm) v = 1; else v = 0;` /
  `if (..) return 1; return 0;`: VC6 turns the if/else into and/shr after the sign extension. Every expression
  form (casts, shifts, ternary, bitfields, volatile, inline functions) narrows to a byte op. A dword load plus
  `movsx r,al` is the same with the flags in an int local first (0x46f840).
- A prototype seen before a definition decides how calls inside / after it pass arguments: d2_CenterObj (before)
  needs `(int, int, int)` (movsx push), the recursion in 0x46cda0 and the call in 0x46f840 need `(short, short,
  int)` (16-bit add/sub): a redeclaration just before the definition gives both (C4028 is only a warning).
- Jump threading: with `sel = cur` VC6 knows `sel == cur` on the loop-skip edge and jumps past the compare; the
  original reloads `sel = d2_Radios[g].sel` (0x46c6c0, 1 byte).
- Before adding a global to a header, grep the address in include/: 0x6333cc was `volatile d3_Time`, and the
  volatile fixed the reload order of d2_AddRefcount.
- VC6 C accepts anonymous structs inside anonymous unions (overlapping members with their own names).

Outside scope: d2_DrawInput is 963/2016 B but only 7 asm lines (length shift), as before.

### Join (round 17, step 1)
- unk_46c6c0.c, unk_46d3d0.c, unk_46e710.c appended to d2.c in address order, their headers removed; every
  function kept its bytes.
- Type mapping: D2MENU (0xfa5aa0) = D2RADIO (flags/count/cur/items = unk00/n/sel/objs); D2SET = D2PICHDR,
  D2SET17 -> D2PICHDR11, D2SPRITE / D2GLYPH -> D2FRAME (kern 0x16, adv 0x1a added), D2FONT = D2PICHDR (h at 0x2e),
  D2IMG -> D2SURF (w/h at 4/8 added), D2EDIT / D2DINPUT -> D2INPUT, D2INPUTREF / D2DBUF -> D2CTRL (= D2BUF),
  D2POS 0xf8a52d -> D2SND 0xf8a528 (x/y at +5, one array), D2TEX one type (size at 0x74).
- D2OBJ: caption[0x100] at 0xbc + capfont at 0x1bc overlap label/list: anonymous union of two anonymous structs
  (VC6 C accepts it). 0x7c text of 46d3d0 = u.text; 0x08 unk08 = pic, 0x0c pic = data; 0x1c count = unk1c;
  0x2c pressed = unk2c; 0x70 mode = unk70; 0x1c0 flags = style; 0x1e4 value = unk1e4; 0x78 = input.
- d2.c globals retyped (not in d2.h before): d2_Unk6ed948 D2PICHDR *, d2_Unk6ed960 D2TEX *, d2_Unk6ed964 D2OBJ **.
  d2_Unk6ed924 / 6ed92c stay void * (casts to D2SURF *), d2_Unk6ed9dc stays int (casts to D2INPUT *).
- d2.h changes beyond additions: d2_Unk47cb70 takes `short i` (int regresses 0x46cb20, 0x46ea10, 0x46f000);
  d2_Unk479020 returns int (callers test it); d2_Unk46ca60 / 46cac0 -> d2_AddRefcount / d2_SubRefcount with
  `#define` aliases for the old names. D2WIN / D2OBJ / D2SURF / D2FRAME pads split into members.
- d2_Unk46cda0: d2_CenterObj needs the int prototype (pushes movsx'd y), the recursion inside 0x46cda0 and the
  call in 0x46f840 need a short prototype (16-bit `sub cx,bp; add cx,..`). A redeclaration
  `void d2_Unk46cda0(short x, short y, int obj);` just before the definition does both (C4028 warning only).
  K&R definition or `void f();` + new-style definition did not.

### Fresh scope
- 0x46fad0 d2_Unk46fad0 (buttons of a window to one width).
- 0x46f840 d2_Unk46f840 (window scroll step, scroll bar, moves the objects): matched with
  `f = w->flags; if ((char)f & 8) i = 1; else i = 0; if (f & 0x20) i += 2;`. Before that 5/656, only the movsx:
  original `mov eax,[esi+0xc]; movsx ebx,al; and ebx,8; shr ebx,3; test al,0x20`, ours `mov bl,[..]; mov al,[..]`.
  ~30 forms (casts, char/int/unsigned locals, two-step, bitfields, volatile, inline functions, mask in a variable,
  unsigned index) in `uv run try` and micro files: VC6 always narrows `(char)x & 8` to a byte op.

### Old d2 nonmatching (step 3)
- d2_AddRefcount: matched by d3_Time (volatile) instead of a new d2_Unk6333cc.
- d2_Unk46c6c0 / d2_Unk46e460: matched (if/else, see report). 46c6c0 then had 1 byte (jle displacement):
  `sel = d2_Radios[g].sel` or comparing with `d2_Radios[g].sel` again both match; kept the first.
- d2_Unk46d040 10/480: x/y register swap. No change: K&R declaration orders (y,x / x,y on one line / at the end),
  new-style definition, short prototype in d2.h, short K&R params (145), (short) locals in d2_AddDirty, `x = x;`,
  case 0x11 first (117), if/else order of the 5/8 case (19).
- d2_CenterObj 116/144: K&R int (same), K&R short (111/144, obj then movsx'd into ecx), all accesses through
  d2_Objs[obj] (same), register params (same), return in the win branch (same), id copy (same).

## d2e: 0x46fba0..0x471a90 (round 17) [d2e]

### Report

Scope 0x46fba0..0x471a90 holds exactly two functions (no hidden starts: 0x46fba0 has one `ret` at 0x471313,
followed by its two switch tables 0x471314 (21 dwords) and 0x471368 (0x51 bytes), then 0x4713c0).
Both are in `src/game/unk_46fba0.c` (types only d2.h lacks in `include/unk_46fba0.h`).

| addr | name | status |
|---|---|---|
| 0x46fba0 | d2_Unk46fba0 (text layout of a window, 6 KB) | nonmatching, near-miss: 131 asm lines differ with slots and registers normalized, same code size class (6176 vs 6004+tables), frame 0x6f8 matches |
| 0x4713c0 | d2_Unk4713c0 (window background) | matched |

- TU: both are d2 (D:\develop\Units\d2.v1\D2.C): use d2_Wins / d2_Objs / d2_Pics / d2_Inputs, strings
  "_FONT", "_BUTTON_RED", "Unknown textparameter: %%%c", "Missing ']'" in the d2 string block. Suggested name:
  join into d2.c (no own TU).
- No shared header edits (d2.h untouched). Own header `include/unk_46fba0.h`: D2TWIN (full 0x3c4-byte view of
  D2WIN: margins 0x10..0x16, img20/24/28, lines[] 0x3c, firstline/nlines 0x238/0x23c, texth 0x244, scrolly 0x248,
  scrollx 0x258, parent obj 0x26c, unk270, framepic 0x278, args[] 0x284, bgpic 0x388, unk38c, spr 0x390,
  alpha 0x394, padding 0x398..0x3a4, nobjs2 0x3b4), D2TINPUT (fields 0x148/0x14c/0x150 of D2INPUT), D2TSEG
  (0x10c text piece, d2_Unkf8abe0), D2RGB, D2IMG, D2SPR, D2TFONT, globals d2_Unkfa5f00/04 (layout x/y),
  d2_Unk6ed954/9c0/978/948/930/960, d2_Unk5fe71c/724/734/74c, d2_Unk631090; prototypes 46ead0, 46fad0, 472800,
  4731d0, 473980, 473b30, 448270, 479150, 4797d0, 479bf0, 479cd0, 47ce60; a copy of the D3_ZERO macro.
  At the join these should move into d2.h (D2WIN / D2INPUT need the extra fields).
- d2.h mismatch found: d2_Unk46cda0 takes `(short x, short y, int obj)` (callers load the y global with a
  16-bit `mov ax,[..]`); d2.h says int. Called here through a cast (D2MOVEFN). d2_Unk473540's 2nd argument is
  a `char *` (d2.h: int).

#### General VC6 lessons
- VC6 uses what it knows about functions defined earlier in the same file for *globals* too, not only
  registers: with d2_FindPic (a leaf, inline strcmp) defined above, the original keeps the global layout x
  (d2_Unkfa5f00) in ebx across the FindPic call; in a separate file it is reloaded. Putting a copy of
  FindPic above d2_Unk46fba0 cut the diff from 772 to 700 asm lines. Near-misses that call many same-TU
  functions should be judged after the join.
- Declaration order of locals had zero effect on slots here (16 orders tried); VC6 coalesces disjoint live
  ranges (D3_ZERO temps share slots with other variables' webs), so slot layout follows code shape.
- Inner call as argument vs local: `k = d2_FindPic(..); f(.., k, ..)` merges the pops (`add esp,0x18`).
- Writing `d2_Inputs[k].field` (array, via a cast macro) instead of a pointer local gives `[eax+0xfa6168]`
  addressing as in the original.
- A double `pos++` stored twice (`inc; mov [pos]; inc; mov [pos]`): one `pos++` before an inner block that
  holds an initialized local array, one after.
- Tile loops `for (j..) for (i..) blit(.., xx + i * tw, yy + j * th, ..)`: VC6 strength-reduces them into the
  original's running x and countdown registers; hand-written running variables allocate differently.

### d2_Unk46fba0 attempts (about 24 builds)
Structure (all source-level): rotated `for (; pos < len;)` loop; inner `while (text[pos] == '%' || '$')`;
switch cases in original layout order A N Y i C m l M t T > < [ ] Z R B L F $% default (source order =
block order); the word part with hyphenation at '~' / '-'; then the object-avoiding loop.
Fixed on the way: memmove size `strlen(q + 1) + 2` (the original copies one byte too many), separate digit
and word-length variables (frame 0x700 -> 0x6f8), comparison operand order in the object loop
(`width + x > o->x`, `x >= r`, `y >= o->h + o->y`), `((int)(char)w->flags & 8U) >> 3`, store order in A
(`d2_Unk6ed9c0 = 0` before `pos++`), R / L (`align` first), FindPic result in a local.
Left (allocation): slot pairs swapped (left/id, top/w, hasinputs/bottom, pctflag after maxw), the 'i' picture
path keeps x in a stack slot in the original and cross-jumps its `pos++; break` into T's tail, the hyphen
loop keeps i in memory and the dest offset in edi, F's flag global in eax (`and al,0xbf`), small register
choices in C / m / > / <. No effect: expression forms of `>` / `<` (VC6 canonicalizes), declaration orders.
Likely to move once the file is joined into d2.c below its callees (FindPic, WinOpenInput, 46ea10, 46ead0,
46bf10, 46cda0, InputSetString, GetPic, 46fad0).

### d2_Unk4713c0
Matched after: `if (scroll >= 0) .. else ..` (out-of-line branch), `fr = d2_Unk47cb70(fp, 6); left = fr->w`,
clip globals saved after `top = left = 0`, tile loop with `xx + i * tw, yy + j * th` arguments.

## d2f: 0x471a90..0x472e00 (round 17), d2 window code [d2f]

### Report
Source: src/game/unk_471a90.c + include/unk_471a90.h (includes d2.h read-only; D2WINX = full layout of D2WIN, cast
from d2_Wins; D2WO(o) / D2WX(i) macros; d2_Unk472ce0 is hidden from d2.h's wrong `void` prototype by a #define
around the include). The TU continues at 0x472e00.

Functions: 0x4721e0 d2_Unk4721e0 (set window background texture), 0x472270 (create the two scroll buttons),
0x472a60 (release the 32 cached surfaces at 0xf8f0a0), 0x472aa0 (create a sub-window inside a window),
0x472c80 (size of the frame pieces), 0x472ce0 (remove the window's own object, returns 0/1),
0x472d20 (remove all objects of a window).
Nonmatching (4):
- 0x471a90 d2_Unk471a90 (draw a window) 45/1872 B: only two stack slots swapped ([esp+0x40] / [esp+0x44]: original has
  ph (frame piece height) in the dead `win` slot and the loop counter in the dead `surf` slot, we have them reversed).
  Tried: declaration order (all 120 permutations of n/ph/sw/sh/x: no effect), `s = surf` copy variants (Ghidra's
  uVar4; makes the allocator put saved/i in other registers: 800+ B), short sw/sh, ph late, for loops (for loops fixed
  n's slot, 85 -> 45 B).
- 0x472390 d2_Unk472390 (create window) 740/960: the original keeps the loop index in eax and copies it to ebp after the
  loop, flags in ebx; we hoist the constant -1 into ebx and put i in ebp from the start. Rest of the body follows
  Ghidra. Tried loop forms (pointer while/for(;;)/do), `return j`, copy order. 20 runs.
- 0x472760 d2_Unk472760 (scroll step) 20/160: layout is right; the original does `add [mem],ecx` for `unk250 += t`
  where we reuse the loaded edx. A pointer local `*q += t` gives the memory add but changes the block layout.
- 0x472800 d2_Unk472800 (resize window) 438/608: structure right (pointer + counter in memory, window pointer esi,
  object pointer `o` for the clip stores); remaining: load order of obj.h / wn->h in the first loop, operand order of
  `w + x` / `y + h` sums (textual swaps do nothing), frame 0x44 vs 0x48.

### VC6 lessons
- Ghidra's `(&DAT_x)[iVar * 0xf1]` with `iVar2 = win * 0x3c4` as an int: write a byte offset local
  (`int o = win * 0x3c4;` and `*(D2WINX *)((char *)d2_Wins + o)`) to get `[esi+const]` addressing with esi = offset
  (0x472270, 0x472760, 0x472d20); a `D2WINX *w = &d2_Wins[win]` gives `lea esi,[ecx*4+X]` (the pointer form, used by
  0x4721e0, 0x472aa0, 0x472390, 0x472800, 0x471a90). Both occur in the original, per function.
- Early `return 0` for a bad index plus `if (!active) return 0;` then the body and `return 1` gives inline
  `xor eax,eax; pop; ret` copies (0x472d20); one `if (active) {..return 1;} return 0;` shares the tail instead.
- A call with an unprototyped (K&R-declared) callee takes int arguments (`int d2_Unk472390();`): with a short
  prototype VC6 narrows the argument arithmetic to 16 bit (`mov ax,[..]; sub ax,..`). d2_Unk4731d0 has short x, y
  (16-bit `sub dx,0xc` in 0x472390).
- `f(x, c ? 1 : 0)` becomes setcc; `if (c) f(x, 0); else f(x, 1);` becomes push 0 / jmp / push 1 with the call tail merged
  (0x471a90 scroll buttons).
- `fild; fidiv; fadd dword` = `(int)((float)a / b + 0.5f)`: float cast, not double (double gives fadd qword).
- A parameter that is only used in the second half is loaded late into a register, and its dead stack home is reused
  for locals / fild temps; do not copy it into a local at the top (that spills the copy).
- `sprintf(buf, "...%s...")` with a missing argument is in the original (0x4721e0): keep it.

## d2g report (round 17), scope 0x472e00..0x473c10, src/game/unk_472e00.c [d2g]

Nonmatching: 472fd0 d2_Unk472fd0 (65/272 bytes differ; same instructions except the i/j register roles are swapped:
ours i=esi j=ecx, original i=ecx j=esi). Tried ~30 variants: loop forms (do/for), j set before the compare vs only in
the break block, j=i after the loop, declaration order permutations (120, no effect), redundant null test.
  - The original has a redundant second `test eax,eax`; VC6 only keeps it with the odd form
    `((D2OBJ *)ZLIST[i]) != 0 && ((int *)o)[0xc] == (int)w` (re-evaluated ZLIST[i] plus an int* access).
  - Break-only `j = i` (what the original asm does: `or esi,-1` up front, `mov esi,ecx` in the break block) gave
    80+ asm-line diffs with this redundant-test form; `j = i` before the compare plus `j = -1` at the loop end is the
    closest (65 bytes).
TU boundaries: none found; the scope ends at 0x473c10 inside the d2 TU (functions continue at 0x473c10).
Names: new rows in names.csv are d2_Unk<addr> (guesses for purpose are in the comments in the source).
Shared headers: none edited. include/unk_472e00.h includes d2.h and adds D2WINX (full window layout: flags 0x0c,
  surf0/1/2 0x20..0x28, scrolly 0x248, scrollx 0x258, sprite 0x390, 0x3b0..0x3c0) plus externs d2_Unk6ed92c,
  d2_Unk6ed964 (draw order list of 0x300 D2OBJ*), d2_Unk6ed9b4 (open windows) and prototypes d2_Unk472c80,
  d2_Unk475e80, d2_Unk4759f0 (returns int here; unk_46d3d0.h says void), d2_Unk46cd80 (short,short,int here).
  d2k should merge D2WINX fields into D2WIN.
Lessons:
  - Several d2 functions re-read the global d2_Objs after every store: write `d2_Objs[obj].f = ..` per statement
    (no D2OBJ *o). Where one pointer is cached (473540 type-9 branch) mix both forms.
  - Failure `return -1;` placed first in the asm = `if (x == 0) return -1;` early return, not an if-body.
  - `y + obj.h > limit` as repeated expression (no temp) matches where a temp `t` does not (473650).
  - Out parameters of d2_Unk472c80 in 473350 are the dead parameters x and win themselves (&x, &win); 4731d0 uses
    locals plus `bw = 0` first.
  - A window font: `if (w->font != 0) font = w->font; else font = default`.
  - The literal is "d2:ShowTxt" (not d2_ShowTxt), "Toby, wir haben ein Problem...".

## d2l: d2 join + 0x473c10..0x4741f0 (round 18) [d2l]

### Report

1. Join (own commit 631fa87): unk_46fba0.c, unk_471a90.c, unk_472e00.c and their headers merged into d2.c / d2.h
   in address order and git rm'd. Every function kept its bytes, except d2_Unk46fba0 (4579/6144 -> 4101/6176,
   because its callees are now defined above it). 0x4721e0 = d2_SetBackgroundTexture.
2. Fresh scope 0x473c10..0x4741f0: 0x473c10 d2_Open (the d2 init: allocations, reads the
   gfx picture table, makes the surfaces, finds the standard pictures) and 0x4741a0 d2_Unk4741a0 (raises a
   view's windows). No hidden starts. The scope ends at the next d2 function, 0x4741f0.
3. d2_Unk46fba0, still nonmatching: declaring buf512 inside the 'i' case block puts the arrays in the original
   order (buf512 at ebp-0x5f8, tmp2 at ebp-0x3f8). That cut the diff from 131 to 125 asm lines, counted with
   registers and stack slots normalized. The byte count is unchanged. What is left:
   - the F case: the original does both pos++ stores after the colour table; we load pos before it. The table
     sits at ebp-0x5c in the original, and pctflag is the int slot just above it.
   - the object loop start `(char)w->flags & 8`: the original sign-extends the dword (movsx); try d2k's
     `if ((char)f & 8) t = 1; else t = 0;` form.
   - the 'i' picture path: x is kept in a stack slot, and its `pos++; break` is cross-jumped.
   - small register and scheduling choices.
4. I did not get to d2_Unk471a90, d2_Unk472760 or d2_Unk472fd0. Their counts are unchanged at 45/1872, 20/160
   and 65/272.

Shared header (d2.h) changes. Nothing that was in d2.h at the round start was renamed or removed; pads were
split into members.
- D2WIN now has the full 0x3c4 layout (details below). Two members are anonymous unions:
  `union { int unk248; short scrolly; }` and `union { int unk258; short scrollx; }`. D2SURF now comes before D2WIN.
- D2INPUT gets unk148 / unk14c / unk150. D2SEG gets r/g/b in a union with `D2RGB col`, and flags is now an int
  (it was unsigned char; 46ead0 is byte-identical). New type D2RGB.
- Prototypes corrected: d2_Unk472ce0 / d2_Unk472e00 / d2_Unk4759f0 return int; d2_Unk473540 takes char *.
- New externs: 5fe708/720/734/744/748/74c/750/754/758, ch_EmptyName (0x631090), 6ed928/930/938/980/9b4/9c0,
  f8eee0, f8ef80, f8f088/08c, f8f0a0, fa5f00/04.
- New prototypes: d3if_LoadSprite, d2_Unk475e40, 475e80, 476860, 479120, 479150, 4797d0, 479bf0, 479df0,
  47ce60, all the joined d2 functions, d2_Open and d2_Unk4741a0. d2_Unk4731d0 is declared with short x, y and
  defined with int x, y. d2_Unk472390 is declared unprototyped.
- Merge risk: the d2m / d2n / d2o workers may declare 0x475e40 / 0x475e80 / 0x476860 / 0x479xxx in their own
  headers with other types (C2371 at merge).
- d2_Wins really has 0x60 entries: d2_Radios starts at 0xfa5aa0, and d2_Open loops to 0x60. d2.h still says
  [0x61]; I left it as it is.
- d2_Unkfb9060..6c are probably one 16-byte struct (d2_Open clears them with a single memset).

General VC6 lessons:
- A zero register (xor ebx,ebx for the whole function) can come from a group of `g = 0` global stores. The
  original zeroed four adjacent globals with one `memset(&g, 0, 16)`, which stores from the memset's eax.
  To find this kind of cause, cut chunks out of a standalone copy of the function with `uv run try`; that is quicker
  than guessing.
- Stack positions of arrays do not depend on declaration order (720 permutations) or on their names. They
  follow reference weight, and block scope changes it: declaring an array inside the block that uses it moved
  it to the original's place.

Outside scope: d2_Unk472390 (d2f) has `win->w = win; desc[1] = win;`, which assigns the window pointer, not the
w parameter (C4047). It may be part of why it is 740/960; I left it unchanged in the join.

### Join (step 1)
- unk_46fba0.c, unk_471a90.c, unk_472e00.c (+ headers) appended to d2.c in address order, headers merged into
  d2.h. Every function kept its bytes except d2_Unk46fba0
  (4579/6144 -> 4101/6176: its callees are now defined above it).
- D2WIN is the full 0x3c4 layout now (D2TWIN / both D2WINX merged): ml/unk12/mt/mb margins 0x10, surf0/1/2
  0x20..0x28 (surf2 D2SURF *), unk30, lines[] 0x3c, firstline / nlines 0x238, unk244 = content height,
  `union { int unk248; short scrolly; }`, `union { int unk258; short scrollx; }` (d2g's functions read them as
  shorts), unk270 / unk274, args[] 0x284, bgpic 0x388, unk38c, spr (D2TEX *) 0x390, alpha, padl..padb 0x398,
  btnup / btndown / bar 0x3a8, unk3bc / unk3c0. Renames in the joined code: texth/total -> unk244,
  scroll -> unk248, unk264 -> visible, selfobj -> parent, framepic/pic -> frame, nobjs2 -> unk3b4,
  unk3b0 -> bar, id -> unk00, bg/sprite -> spr (casts to / from int for d3if_LoadSprite / d3if_FreeSprite).
- D2INPUT unk148 / unk14c / unk150; D2SEG: `union { struct { r, g, b }; D2RGB col; }`, flags is an int
  (46fba0 stores dwords, 46ead0's byte test stays a byte test).
- d2.h prototype fixes: d2_Unk472ce0 / d2_Unk472e00 / d2_Unk4759f0 return int, d2_Unk473540 takes char *.
  d2_Unk4731d0 is declared (short x, short y, ...) for the callers above it, defined with int x, y;
  d2_Unk46cd80 is redeclared with shorts just before d2_Unk4731d0 (both C4028 warnings only).
  d2_Unk472390 is declared `int d2_Unk472390();` (unprototyped). D2MOVEFN casts dropped (46cda0's short
  redeclaration in d2.c already covers the calls below it).
- Globals: d2_Unk5fe734 / 744 / 748 defined (-1) in d2.c; 0x631090 is ch_EmptyName (ch.h, the "" text),
  d2_Unk6ed930 is D2SURF *; d2.c uses txt_Ptrs instead of d2_Strings and dd_Page instead of d2_Unk63120c
  (both d2.h externs stay, unused). 0x4721e0 = d2_SetBackgroundTexture.
- d2_Unk472390 has `win->w = win; desc[1] = win;` (d2f's source: the window pointer, not the w parameter; C4047),
  kept as is in the join.

### Fresh scope 0x473c10..0x4741f0 (step 2)
Two functions, no hidden starts (nop padding after 0x474196 and 0x4741e2; the scope ends at the d2 TU's next
function 0x4741f0).
- 0x473c10 d2_Open (error strings "d2_Open:Too many Objects in GFX-File...", "d2_Open: Could not init
  transparency..."): allocates the draw order list / objects / picture table / dirty lists, numbers views,
  windows and objects, reads the picture table of "%sgfx\%s", loads the locked pictures, sets the screen size,
  creates the four surfaces, finds the standard pictures (_WIN_ICONS .. _TEXT_CURSOR). Matched. First draft had
  a zero register (ebx) for the whole function: the four `d2_Unkfb9060..6c = 0` stores are one
  `memset(&d2_Unkfb9060, 0, 16)` (stores from the memset's eax). Found by cutting chunks in a standalone copy.
- 0x4741a0 d2_Unk4741a0 (raises the windows of view v: d2_Unk472fd0 for each D2VIEW::wins).

## d2m: views of the d2 layer, 0x4741f0..0x475590 (round 18) [d2m]

### Report
Scope 0x4741f0..0x475590, 7 functions (src/game/unk_4741f0.c, include/unk_4741f0.h).
- Nonmatching (2): 0x474e80 d2_Unk474e80 (190/1600 bytes differ, register allocation only), 0x4741f0
  d2_Unk4741f0 (about 1650/2336, draft of the whole form loader, structure not yet right).
- TU: same d2 TU as d2.c (D2.C); suggested name d2_view.c or join into d2.c. The TU continues after 0x475590.
- Shared headers: none touched (d2.h read-only). unk_4741f0.h adds D2TEXX, D2FORMH / D2FORM0 / D2FORM2 (form file
  window records, version 0 = 0xed4 bytes at 0x633f18, version 2 = 0x101c bytes at 0x68ce98), D2WINF (full D2WIN), and
  prototypes (vfs_*, txt_FindKey, d2_Unk4721e0 / 472390 / 472aa0 / 4731d0 / 473650 / 4611b0 / 4794c0 / 479610).
- Meaning: a "view" (D2VIEW, d2_Unkfb1020[0x30], 0x2ac bytes) is a set of d2 windows loaded from a form file
  ("forms\NAME.form", versions "..2" and plain) that are drawn onto an image (unk1a4) which is copied onto two 3D
  textures (tex1 / tex2). 0x4741f0 loads the form, 0x474b30 loads and prepares (hides the objects, creates the two
  images), 0x474ca0 sets new textures (0x474d10 clears the old ones, 0x474d90 sets and fills the new ones), 0x474e80
  redraws all windows of the view into the image and copies it to the textures, 0x4754d0 shows / hides the objects.

### VC6 lessons
- A call whose argument is `i == 0 ? i : size` is compiled as a branch with the constant 0 taken from the counter
  register (push edi); the ternary form gives a mask or a pre-computed temp. Writing `if (i == 0) f(0, i, ...); else
  f(0, size, ...);` gives the original (VC6 merges the two calls). (474d10)
- `if (x == 0) return 0; ...; return r;` vs the original that has the `xor eax,eax` return at the end out of line:
  nest the whole body in `if (x != 0) { ...; return r; } return 0;` (474b30).
- Inline strcpy loops (`do { c = *s; d[s - src] = c; s++ } while (c)`) come from plain strcpy.

### Attempts
#### 0x474e80 d2_Unk474e80 (nonmatching, 190 bytes)
Switch case order must be 1, 0x11, 4, 5/8, 0x41, 0x43, 0x45 (the order of d2_Unk46d3d0). `if (unk2c) mode = 2; else if
(unk50) mode = 1;` matches the original branch layout; `if (o->unk44) AddShape(.. + 8) else AddShape(..)` with two
calls merges like the original. What is left: register choices only, e.g. the three loads of the clip rectangle in case
0x45 (original edx, eax, ecx; ours eax, ecx, edx), the loads before the 0x43 text call (original h, w first), x / w / y of
the 5/8 caption call (original edx / eax / ecx, ours ecx / edx / eax). Tried without effect: permutations of the
statement order of x, w, y (18), local variables for font / width, reuse of x / y, `tw * 3` spellings (5), local sizes
for the 0x43 call (3), declaration order of the locals (2), `int` mode.
#### 0x4741f0 d2_Unk4741f0 (nonmatching)
Findings from the asm (all in the draft): `char buf[256] = ""` (the "" literal is at 0x631090), strcpy + strcat of
"forms\\" (global char array 0x5fe608) + name + ".form"; the view slot search is a pointer loop over
`&d2_Unkfb1020[1].nwins` with the bound 0xfb91e4; version 0 and version 2 records differ at the tail (parent index 0xe94
vs 0xed0, font index vs font name, extra margins / pictures / slider lengths / background / alpha / frame in v2); the
version 0 branch tests `parent` twice (pw / ww of the parent window are only set under the first `if (parent)` and are
loop-carried through stack slots), version 2 has a plain if / else; flags byte of inputs and sliders is a local
`unsigned char` stored to a stack slot (spilled char variable passed as the char parameter, loaded as a dword).
The original keeps ver, n, view, i and three induction pointers in stack slots and uses ebp as a zero register at the
start; our frame is 4 bytes smaller (0x244 vs 0x248) and the register use differs from the start. Not matched after
about 8 compile runs (more than half of the bytes differ): left as a draft.

## d2n report (scope 0x475590..0x475ff0, src/game/unk_475590.c, include/unk_475590.h) [d2n]

475f70 is d2_SetForm (error string "d2_SetForm(): invalid win number").
Nonmatching:
- 4756b0 (128 bytes, 52 differ): original loads w->parent inside each branch before the call; ours hoists the load above `test bl,2`.
  Tried: locals per branch, early return, d2_Wins[win] / D2WO() forms for one or both parent reads, re-taking w, dummy volatile. No change.
- 475730 (288 bytes, 21-23 differ): structure matches; only two stack slots swapped (original: x float at the win slot E+4,
  int conversion temps at E+8; ours the other way round). Tried: declaration order permutations (all 48), separate h2,
  no named h (w->w / 2 written twice), cx local, volatile h, K&R parameter order, x = y = 0 dead store. No change.
- 475cb0 (400 bytes, 19 differ): only the store order in the type 0x45 branch (orig: 0x80, 0x7c, then 0x84 sunk after the
  flags load; ours 0x80, 0x84, 0x7c). All 6 source orders give the same bytes; raw-offset stores, d2_Objs[obj] form, temp copy,
  char* parameter did not help.

### Findings
- 475730 is not what Ghidra shows: (win, unsigned char flags) with float math (centres on screen, x scaled by float d2_Unk5fe708 at 0x5fe708), clamps to the screen, calls d2_Unk46cda0 via __ftol.
  It is declared K&R (`void d2_Unk475730();`) because 475850 (wins[0] of a view) tail-jumps into it passing only one argument (second arg slot left untouched).
- 475850 = d2_Unk475730(d2_Unkfb1020[view].wins[0]) as a jmp.
- 475cb0 has 5 parameters (obj, a, b, c, d); d2.h declares 2 (d2.c calls pass 2). 4759f0 returns int (d2.h says void). Both handled with the
  #define rename trick before including d2.h (like unk_472e00.h); unify at the d2 join.
- New globals in unk_475590.h: d2_Unkf8f088, d2_Unkf8f08c (screen centre y / x), d2_Unk5fe708 (float), d2_Unk6ed938, d2_Unk6ed958 (current view), d2_Unk6ed980.
  d2_Unk6ed938 / d2_Unk6ed980 are also declared in unk_471a90.h (same types).

### VC6 lessons
- `abs()` (stdlib.h) is an intrinsic: cdq / xor / sub. Use it instead of a ternary.
- A local pointer `D2WIN *w` assigned in each branch (`w = &d2_Wins[...]` in both arms of an if) keeps the *0x3c4 index multiply duplicated per branch
  (matched 475b80); an `idx*0x3c4` byte-offset macro form does not.
- Byte offset form `D2VO(view*0x2ac)` (cast of (char*)base + off) for the active test and nwins keeps the offset in a register (4758f0), while the loop body
  uses d2_Unkfb1020[view].wins[i]... see source: only the `D2VO(...).active` / `.nwins` reads were needed.
- Condition `a >= N` (not `a > N-1`) reproduces `cmp r,N; jl`. `(char)fl >= 0` gives jge; `!(fl & 0x80)` gives `test bl,bl; jns`.
- Commutative int multiply operand order follows source: `(short)w * (short)a` vs `(short)a * (short)w` swapped imul operands.
- Repeated `d2_Objs[obj].field = ...` stores reload the d2_Objs pointer each time (matches 475e80), a local `D2OBJ *o` does not.

## d2o: 0x475ff0..0x476c00 (round 17) [d2o]

### Report
File src/game/unk_475ff0.c (+ include/unk_475ff0.h, includes d2.h with d2_Unk476030 / d2_Unk476110 renamed
around the include, since d2.h declares both void). 15 functions.
d2.h not touched; no shared-header additions. The d2 TU continues past 0x476c00 (not in scope).

Functions: 475ff0 (window i of a D2VIEW, -1 out of range), 476030 (select font, returns the font header),
4760b0 (object font; switch on type 0x40 window / 0x41 input), 476110 (dissolve step), 476220 (paintbox plot, pen 0..2),
476710, 476760 (paintbox clear / fill), 476860 (free fade), 4768a0 (draw one fade), 476b10, 476b40, 476b70, 476b90.
Nonmatching: 4764f0 d2_Unk4764f0 (paintbox line, 446/496), 4767d0 d2_Unk4767d0 (start a fade, 82/144).

Suggested TU name: none new; it is the d2.c TU (join pending; fade code = "d2_fadeinfo", paintbox = "d2_Plot",
"d2_ClearPaintbox" strings).

### VC6 lessons
- `if (!(a > 0.0f)) a = 0.0f; else if (!(a < 1.0f)) a = 1.0f;` gave the layout where the else-assignment is inline
  (all plain `if/else` and ternary forms put it at the end). Brute-forced 15 condition forms x 3 shapes.
- `a == 1.0` (double literal) gives `fcomp QWORD`, `a > 0.0f` gives `fcomp DWORD`.
- A zero that the original keeps in ebp for the whole function (also used as the high dword of `fild QWORD`) is a
  variable assigned 0 at the top (`on = 0;` before the first computation), not at its use.
- (float)(unsigned) gives `fild QWORD` with a zero high dword; `(float)(int)(...)` gives `fild DWORD`.
- A global read twice in the original (`pen = g & 0xff` early, `switch (g & 0xff)` later) is two reads: keep the
  local assignment after the first early-out so VC6 does not hoist it, and `switch` the global itself.
- `unsigned int` global with `& 0xff` (not unsigned char) to get `mov; and eax,0xff`.

### Nonmatching attempts
- 4767d0: the original does NOT tail-merge the two `return 0` (loop exit and `i >= 0x20` inside; `jl alloc; xor eax,eax;
  pop edi; ret`), we always merge (`jge` back to the loop-exit return). Tried: for vs do-while (do-while is right:
  inc/cmp/jl), `i > 0x1f` vs `>= 0x20`, `if (i < 0x20) {alloc} return 0`, f = 0 / return f, goto, (D2FADE*)i - i,
  all 24x... 720 permutations of the six member stores (also the store/load scheduling differs: original preloads
  y and w, we preload h and y). No variant removes the merge.
- 4764f0: horizontal branches match when the params are modified in place with post-increment in the call
  (`d2_Unk476220(x1++, ...)`, `while (--n != 0)`); the original has an 8 byte frame (sx, sy on the stack) and
  y1 only transiently in eax (reloaded from / stored back to its parameter slot in the vertical loop); we keep y1
  in ebx and have no frame. Copying into fresh locals (px / py) gives a 4 byte frame and y1 in eax at the start
  but breaks the rest (453/496). About 6 runs.

## d2t: d2 tail files, TU boundaries, type unification (round 20, special task) [d2t]

### Report
Matched: 0x478320 d2_Unk478320 (by the join, no source change).
Improved, still nonmatching: 0x479bf0 d2_Unk479bf0 92 -> 12 bytes, 0x47c0e0 d2_Unk47c0e0 595 -> 303 (by the move),
0x47c410 d2_Unk47c410 18 -> 17, 0x47a460 d2_Unk47a460 775 -> 774 (D2KEY colour key).
Unchanged nonmatching after tries: 0x4798f0 (51/240), 0x477000 (526/784), 0x477ba0 (511/800, not tried again).

#### TU boundaries (evidence)
How a TU shows in .data (tested with a small VC6 file): an obj has one plain .data section with all its initialised
globals, followed by its string literals as COMDATs in function order (/O2 implies /GF: identical strings are
pooled across TUs, the first obj keeps the copy). So in the exe every TU's .data is [globals][its new strings].
- TU1 = d2.c (0x4687a0..): .data 0x5fe700 (globals) + strings 0x5fe760.."d2:fadeinfo" (0x4767d0) + "SHAPBANK" and
  "..." (0x5febb8 / 0x5febc4; used only by 0x47c610, so TU1 had a function after 0x4767d0 that used them and was
  dropped by the linker, the pooled copy stayed in TU1).
- TU2 = GFX.C: .data starts 0x5febc8 (globals -1, -7, ... of 0x476cb0..0x478890), then the strings of 0x4784c0
  (Mausclick...), 0x478cc0 (gfx_CreateSurface, the GFX.C path), 0x479020 (Lock). Probe: d2_Unk478320 matches only
  with d2_Unk476c90 above it (it keeps edx over the call) -> unk_476c00.c and unk_478320.c are one TU, joined.
  __LINE__ 0x43f inside d2_Unk478cc0 sits at line 1134 of the joined file (476c00 starts at line 21): GFX.C is the
  whole TU from about 0x476c00, not a file that starts at 0x478cc0. Suggested name: gfx.c (D:\develop\Units\d2.v1\GFX.C).
- TU1 / TU2 boundary: in (0x4767d0, 0x476c90]. 0x4767d0 is TU1 (its string is in TU1's .data), 0x476c90 is TU2
  (probe). 0x476860..0x476c70 have no data and no probe reacts (callee-below-caller moves in d2.c, 0x476030 and
  0x475cb0 defined above their TU2 callers: no byte changes). The fades (0x476860..0x476b40) belong with 0x4767d0;
  0x476b90 (add to the item list) is the twin of 0x476c00 and probably TU2. Nothing moved out of d2.c (not proven).
- TU3 = shp: .data starts 0x5fed74 (colour keys 0x5fed74 / 78, then 640 / 480 / 800 at 0x5fed7c..84), then the
  strings of 0x479d00 (d2:shp:shp_LightTable), 0x479df0, 0x479f40.. in function order to 0x47c5e0+. Probe:
  d2_Unk47c0e0 gets 595 -> 303 bytes once d2_Unk479f10 is defined above it (same TU). 0x479d00..0x47ab30 moved from
  unk_479610.c to the top of unk_47ab30.c. Suggested name: shp.c (memory tags "d2:shp:", "shp_t:").
- TU2 / TU3 boundary: in (0x479020, 0x479d00]. 0x479610..0x479cd0 (unk_479610.c: blits, clear, pixels, outline,
  clip) have no data; compiled in either file nothing changes. By content (surface operations) they are GFX.C;
  left in unk_479610.c.

#### File changes
- unk_478320.c + include/unk_478320.h joined into unk_476c00.c / include/unk_476c00.h (deleted).
- unk_479610.c 0x479d00..0x47a7d0 -> unk_47ab30.c (top); unk_479610.c keeps 0x479610..0x479cd0.

#### Header changes
- d2.h: D2SURF is the 0x40-byte surface (fields renamed / added: rowbytes 0x0c (was unk0c), bpp 0x14, unk18,
  DDSurface 0x20 (struct IDirectDrawSurface4 *), left 0x24, right 0x2c (was unk2c), locked, unk38, unk3c);
  forward declarations of struct IDirectDrawSurface4 / _DDSURFACEDESC2; d2_Unk6ed9dc is D2INPUT *; prototypes:
  478cc0 (D2SURF *desc, int flags) -> D2SURF *, 478fc0 / 479020 / 4790e0 / 479120 / 479150 / 4794c0 / 479610 /
  4796c0 / 4797d0 / 4799e0 / 479b60 / 479b90 / 479df0 return int, 479b60 / 479b90 colour unsigned short,
  479bf0 (x, y, w, h, r, g, b), 4794c0 (sx, sy, w, h, D2SURF *src, x, y, IDirectDrawSurface4 *dst), 479610
  (.., IDirectDrawSurface4 *src, x, y, D2SURF *dst), surface parameters D2SURF *.
- GFXSURF / GSURF gone (D2SURF), GSHAPE gone (D2SHP of unk_47ab30.h, fields ax0.. / bx0.. / nruns / lineoff).
- unk_476c00.h: gets unk_478320.h's types / externs / prototypes; d2_Unk477ed0 int; d2_Unkf8aa84 no longer volatile.
- unk_479610.h: includes d2.h and unk_47ab30.h, only what they lack. unk_47ab30.h untouched (d2u reads it).
- d2.c / unk_476c00.c: d2_Unk6ed9dc casts dropped (no byte change).

#### Nonmatching notes (functions touched)
- 0x479bf0 d2_Unk479bf0 12/224: r / g in edi / ebp swapped (orig r = edi, so its dead slot takes x - 1 + w); param
  types, decl order, nested ifs, j removed: no change.
- 0x47c410 d2_Unk47c410 17/464: size in edx and the loop pointer / kind in ecx, the original the other way round;
  `size = ...; total = 0;` fixed the `add eax,ecx` at the second alloc. 4 runs (no local, kind local, decl order,
  initialisers, while loop, unsigned): no change.
- 0x4798f0 d2_Unk4798f0 51/240: surf in ecx, bpp in eax (orig surf eax, bpp edx, bytes ecx); bpp locals, int
  bytes, bytes in the case, copy of surf: no change or worse.
- 0x477000 d2_Unk477000 526/784: the original keeps the mouse shorts in di / bx, ours d2_Unk5febdc in edi and x in
  bx; struct forms of the two global pairs: no help.
- 0x47c0e0 d2_Unk47c0e0 303/768 now (d2_Unk479f10 above it); not worked on.

#### VC6 lessons
- .data layout per obj: [initialised globals][string COMDATs in function order]; strings pooled across TUs (/O2 =
  /GF). The first .data item after a run of strings marks a new obj; a string sitting in an earlier obj than its only
  user means a dropped function of that earlier obj used it too.
- A __FILE__ / __LINE__ pair dates the file: line 0x43f in d2_Unk478cc0 needs ~1100 source lines of the same file
  before it, so the TU does not start at the first function that names GFX.C.
- `memset(&tab[i], 0, sizeof tab[i])` instead of four `= 0` stores: the separate stores hoist a zero register over a
  preceding `i >= 0` test (`xor ecx,ecx; cmp eax,ecx` instead of `test eax,eax`); with memset the later global
  decrement must not be volatile (volatile let it schedule into the stores).
- A running counter `j = y; ... j++` vs `y + i` in the call: the original's `y + i` is strength-reduced into a
  register set after the loop guard; `j = y` before the loop is loaded before the guard (92 -> 12 bytes).
- Callee knowledge is a TU probe both ways, also for nonmatching callers: a big drop in differing bytes when the
  callee moves above the caller (47c0e0 / 479f10) is same-TU evidence. Most callers are insensitive (no value live
  in ecx / edx across the call), so "no change" proves nothing.

## Also about this unit
- notes/units/ai.md, section [hv1]

## match-base 2026-10-23
- Reference updates only (an unk_47d600.c Readability rename): d2_AddDirty / d2_AddShape / d2_Unk47ced0 and the
  clip rectangle fields use d2_ShpW / d2_ShpH / d2_ClipTop / d2_ClipBottom now (evidence in
  notes/units/unk_47d600.md).

## match-base session 2026-10-24 (base-2 worker, shard 2/6)
- Reference updates only (shp.c Readability renames): d2_Unk6f0458 -> d2_ClipLeft and d2_Unk5fed7c ->
  d2_ClipRight. Evidence: 0x4a... sets `clipx0 = d2_Unk6f0458; clipx1 = d2_Unk5fed7c` next to
  clipy0 = ClipTop / clipy1 = ClipBottom, and the clipping compares use them as the left / right edges.

## match-base session 2026-10-24 (base-5 worker, shard 5/6)
- Readability renames requested from gfx.c (references updated here): d2_Unk6ed924 -> d2_ScreenSurf,
  d2_Unkf8a674 / d2_Unkf8a676 -> d2_MouseX / d2_MouseY, d2_Unk46f1a0 -> d2_DrawTextBox,
  d2_Unk476030 -> d2_SetFont, d2_Unk47c6f0 -> d2_DrawShape, d2_Unk47c830 -> d2_DrawShapeScaled,
  d2_Unk47cb40 -> d2_SetClipRect, d2_Unk47cdb0 -> d2_DrawAnim, d2_Unk47cd20 -> d2_StartAnim,
  d2_Unk47ce30 -> d2_FreeAnim, d2_Unk478cc0 -> d2_MakeSurface, d2_Unk478fc0 -> d2_FreeSurface,
  d2_Unk46cd80 -> d2_CreatePicObj, d2_Unk46cda0 -> d2_MoveObj, d2_Unk46ea10 -> d2_TextWidth,
  d2_Unk475bf0 -> d2_InputStep, d2_Unk6f0458 / d2_Unk5fed7c -> d2_ClipLeft / d2_ClipRight,
  d2_Unk5fe718 / d2_Unk5fe73c / d2_Unkf8f084 -> d2_CurrentFont / d2_DefaultFont / d2_FontHeight,
  d2_Unk6ed9dc -> d2_FocusedInput, plus the fields D2OBJ.unk2c -> highlighted, D2OBJ.unk38 -> disabled,
  D2INPUT.unk1c -> minval, D2INPUT.unk20 -> frameY. Evidence in notes/units/gfx.md (match-base 2026-10-24).

## match-base 2026-10-24 (base-3 worker, shard 3/5)

Raised to byte + readable (9): d2_FreeOldest (D2PIC.unk40 -> refs), d2_LoadPic
(d2_Unk47c410 -> d2_BankConvertTo16, the merge kept base-2's name over d2_ConvertBank; d2_Unkfa5f20 -> d2_GfxPath), d2_RestoreShapes
(d2_Unk479480 -> d2_BlitPart, D2BUF.unk190/unk198/unk1a4/unk1a8 -> active/loaded/dstSurf/srcSurf),
d2_AddToWin (d2_Unk4730e0 -> d2_AddToDrawOrder, D2OBJ.unk40 -> buf, D2WIN.unk244 -> contentHeight),
d2_FlushDirty (d2_Unk6ed92c -> d2_BackSurf), d2_AddDirty (d2_Unkfb9060/64/68/6c ->
d2_DirtyRectLeft/Top/Right/Bottom; zeroed at init, no writer in the exe), d2_DrawFrame
(d2_Unk47cb70 -> d2_GetFrame, d2_Unkfa5f08/0c -> d2_PointerX/PointerY, d2_Unk5fe738 -> d2_FramePic),
d2_OpenInputIcon (d2_Unk46c9f0 -> d2_NewObject, D2OBJ.unk1c -> drawFrames), d2_WinOpenInputIcon
(D2WIN.unk248 -> scrollY, hst.c's D2WIN view follows), d2_WinOpenInput (D2WIN.unk12 -> mr),
d2_OpenSlider (d2_Unk4796c0 -> d2_ClearSurface, D2OBJ.unk4c -> interactive, D2OBJ.unk94 -> firstPic).

Escalated (2): d2_Exit (Q4 typed-field rewrite of the D2PIC free loop flips the address operand
order to [ecx+ebx*1+0x34] where the original has [ebx+ecx*1+0x34] - the IV-offset-as-base class;
Q3 0xfa5d20 = &d2_Wins[0].active + 0x60 windows; Q2 needs 9 names), d2_DrawInput (3 code hunks at
0x4697a3/0x4697d6/0x4697e9: the original loads the WORD o->w where ours reuses the int in->w and
reads o->x / o->w in the other order; bw = o->w - ... tried, 6 -> 165 code hunks; Q2 12 names),
d2_OpenInput (Q2 only: D2INPUT.unk144 written 1 in the slot setup, no reader, no evidence).

Tooling: a project-wide rename must also update config/symbols/lint_baseline.txt - the d2_BlitPart
lines were updated by hand (`uv run rename` does not touch the config).

## match-base session 2026-10-25 (base-3 worker, shard 3/6)

Unit d2.c only. Base queue 97 -> 55 below behaviour + readable (19 -> 17 behaviour drafts, 97 -> 52 not
readable); check count 2523/4664 before and after (no byte function moved). Function renames and field names
are in the session report; the fields came from the existing notes' evidence (D2OBJ checkable / checked /
clickDisabled / greyed / hitTestable / picW / picH / shadowX,Y / captionX,Y / textShadow / rightAligned /
centred, D2WIN.id, D2VIEW.loaded / shown, D2CTRL(X).loaded / shown, D2RADIO.flags).

Escalated (2, both were the recorded member-load-order cases, see [z3] / [d2j]):
- d2_DrawSlider (0x46aa00, 4/4720 B): one code + one order hunk at 0x46b5d5 / 0x46b60c, the two
  `w->x + w->w + 4` loads of the max label (original x->ecx, w->edx; ours w->ecx, x->edx). `uv run spell`: no menu
  rule matched ("escalate with the diffclass verdict"). No meaning error. Readability done.
- d2_DrawButton (0x46bf60, 17/624 B, 16 differing instructions): (a) `len = w->w - r->w - l->w` - the original
  subtracts l->w first; spelling it `w->w - l->w - r->w` compiles to identical bytes; (b) the second DrawShape's
  `w->w - r->w + w->x` - the original loads w->w then w->x, ours the other way; `w->x + w->w - r->w` is also
  identical; `uv run spell`'s one reload prescription (drop the local) is already satisfied on both sides. The 21
  source variants of [d2j] plus these two are the whole meaning-class lead set; the rest is VC6 register
  allocation (match-deep).

Tried and reverted (no byte change, not committed): the two source orders above in d2_DrawButton.

Not done / next leads in this unit: d2_DrawPic (0x46d040) is blocked on Q2/Q7 for d3_Unk44ee40 - the
0x44ee40 no-op triangle filler is @deep in d3_io_a.c with *no evidence at all* (empty body, assigned to
d3_TriFuncs[0..5], called with different signatures); its 13 code hunks are the x/y register swap of the
sprite draw. d2_CloseWindow (0x472e00) still needs D2WIN.unk3b8/3bc/3c0 names (3b8 = scroll bar slider object
per d2.h, 3bc/3c0 only compared to -1 here; no writer in our source - name gap). d2_DrawFades / d2_SetCaption
/ d2_WinOpenPicStyled / d2_LoadView / d2_RemoveWindowObjects are byte + Q2 only.
## match-base 2026-10-25 (base-5 worker, shard 5/6)
- Reference updates only: d2_Unk14661a0 -> dd_PitchPixels in d2_Unk476110 (2 uses, the screen row stride);
  the duplicate extern in d2.h was removed (dd_vesa.h owns 0x14661a0, d2.c already includes it). The
  comments of d2_BlitPart follow 0x479150 -> d2_BlitPartMode. No grade change in d2.c.
## match-base 2026-10-25 (base-3 worker, shard 3/6, session 2)

Unit d2.c only. Base queue 55 -> 47 of 130 below behaviour + readable in d2.c (shard 3/6: 77 -> 69);
check count 2539/4664 before and after. d2.c readability 75 -> 80 readable. d2.c differing bytes
(unitdiff) 13128 -> 12883. Escalated this session (6): d2_CenterObj, d2_DrawPic, d2_DrawObjCaption,
d2_DrawObjects, d2_RemoveObj, d2_DrawTextLine.

- d2_CenterObj (0x46c1d0, 59/144 B, 67 instructions both sides; was 116/144 B). The source that gets the
  compiler's own shape: compute `int x` in the if/else and call d2_MoveObj once after the branch (both arms
  call: 116, worse prologue with the late d2_Objs load and a materialized pointer; branch polarity swapped:
  86). Subscripts instead of the `D2OBJ *o` local, a named `D2WIN *win` local, an early-return `!centred`
  form and an `a ? b : c` argument all compile identically to the 59 B form. Left: VC6's register choice
  (ours obj=edi, win=ebx; original obj=ebx, win=edi = one bijective swap), the entry push order and the tail
  merge (ours shares pop edi/pop esi/ret, the original duplicates it) - listing/c2prio/siblings.
- d2_DrawPic (0x46d040, 25 differing of 159): the code hunks are the x/y register swap at 0x46d157 (ours
  esi=[esp+0x18] + edi=[esp+0x14], original the reverse; the movsx/push follow the swap), no meaning error
  - (short)x, (short)y, surf, data is the call order. `uv run spell` reload at 0x46d157 is already the source form
  (x/y are the parameters, re-read at the call, no local holds them). Q2 blocked on d3_Unk44ee40 (no
  evidence, @deep in d3_io_a.c).
- d2_DrawObjCaption (0x46d220, renamed from d2_Unk46d220; 71 differing of 131 vs 140; 155/400 B; was 116
  differing of 150). Pulling the caption bounds into the style&4 arm and drawing with one d2_DrawTextBox
  after the branch is the whole gain (the call-in-both-arms form is the 150-instruction one). Left: the
  original hoists the shared argument pushes above the test and duplicates the 4-instruction epilogue; its
  register map is ebp/esi/edi where ours is ebx/esi/edi. Readability done (Q1..Q7).
- d2_DrawObjects (0x46d3d0, renamed from d2_Unk46d3d0; 93 differing of 1137 vs 1156). The `mode` flags
  variable sits in edi where the original uses ebp (whole 46db68..46dbf5 block), the caption/dirty blocks
  push the same arguments in another order, 46e0d4..46e102 is instruction-stream misalignment. The caption
  ternaries written as if/else compile identically (reverted). Q2 still blocked: d3_Unk44ee40,
  d2_Unk5fe71c (char flag, no writer in our source), d2_Unk479cd0 (defined in gfx.c), D2OBJ/D2WIN
  unk00/unk1e4/unk58/unk6e/unk268.
- d2_RemoveObj (0x46e710, 105 differing of 260 vs 263; 402/656 B). One constant-0 register cascade: ours
  keeps 0 in ebp for the stores (mov [x+0x3b4],ebp / [x+0x7c] / [x+0xbfc]) and the compares, the original
  stores immediates (mov [x+0xbfc],0x0) and keeps its zero in edi (test edi,edi). The off local dropped and
  obj re-read at the memset/id write compiles identically (reverted). Readability done this session
  (sizedObjCount field, typed id write - the typed cast keeps the exact address expression, bytes
  unchanged, while `o->id = obj` did regress to 124 differing and was reverted).
- d2_DrawTextLine (0x46ead0, renamed from d2_Unk46ead0; 481 differing of 441 vs 438). Same statements,
  another register map (the original keeps the zero in edi, ours the layout values in ebp) and different
  scheduling throughout (the strlen scas, the TextSegCount compares). spell reload at 46ee15 names the
  hl/skip char locals set in the '$H'/'$M' switch arms - not a re-readable expression. Readability done
  this session (d2_TextSegCount, d2_LayoutMode).

Renames this session (all in d2.c + d2.h): d2_Unk46d220 -> d2_DrawObjCaption, d2_Unk46d3d0 ->
d2_DrawObjects, d2_Unk46ead0 -> d2_DrawTextLine, d2_Unk471a90 -> d2_DrawWindow, d2_Unk4759f0 ->
d2_GetObjPicSize, d2_Unk476110 -> d2_DissolveStep, d2_Unk6ed9b8 -> d2_PendingCaptionWin, d2_Unk6ed9bc ->
d2_PendingCaptionObj, d2_Unk6ed954 -> d2_TextSegCount, d2_Unk6ed978 -> d2_LayoutMode, D2WIN.unk3b4 ->
sizedObjCount. Knock-on readability (Q2-only functions that left the queue): d2_DrawAll, d2_SetCaption.
- Next in the queue: d2_DrawTextBox (0x46f1a0, byte, readable) was the head at TIME UP.
- Not tried here: d2_Unk5fe71c (needs a writer we do not have), d2_Unk479cd0 (its prototype belongs to
  gfx.c, a cross-unit header move).

## match-base 2026-10-25 (base-4 worker)
- d2_Unk4767d0 -> d2_StartFade and d2_Unk476860 -> d2_ReleaseFade (definitions and comments); the fade record's flags byte is what the callers test (`test BYTE PTR [f],4`).
## match-base 2026-10-25 (base-3 worker, shard 3/6, session 3)

Unit d2.c only. Base queue 47 -> 38 of 130 below behaviour + readable (the renames also cleared Q2 for functions outside this session's list); readability 80 -> 82 readable.
Check count 2548/4664 before and after (no byte function moved); d2.c differing bytes (unitdiff) 12883 ->
12781. Escalated this session (7): d2_DrawTextBox, d2_DrawWindowText, d2_DrawWindowBackground, d2_DrawWindow,
d2_CreateScrollButtons, d2_CreateWindow, d2_ScrollWindowBy, d2_ResizeWindow (8 with the readable one).

- d2_DrawTextBox (0x46f1a0, layout draft, 528 vs 522 instructions): read the prologue, the measure/clip block,
  the sprite blit, the 'fits' word split, the glyph loop and the tail - every statement corresponds. Left:
  another allocation (original frame 0x638 with lines[] at [esp+0x48], x/y home slots; ours 0x630 at
  [esp+0x40], x in a register) and spills. `uv run spell`: rep-stos already satisfied, dup-tail at 46f82b is the
  shared epilogue. cmp-test lead tried: if (ln > 1) -> if (ln >= 2) gives the original cmp reg,2 (kept,
  586 -> 584 lines).
- d2_ScrollWindow (0x46f840, was d2_Unk46f840): byte + readable. The D2WIN scroll fields named from the body:
  unk24c -> appliedScrollY, unk250 -> scrollRemY, unk254 -> scrollRemX, unk258 -> scrollX, unk25c ->
  appliedScrollX, unk260 -> scrollStep, unk3b8 -> scrollBar; d2_Unk475aa0 -> d2_EnableObject (recursive
  disabled/enabled setter, called with the scroll bar object).
- d2_DrawWindowText (0x46fba0, was d2_Unk46fba0): far draft (1865 vs 1890 instructions, 565 lines, code 603
  / slots 198). Named d2_TextLayoutX/Y (0xfa5f00/04, the running layout cursor), d2_TextSegFlags (0x6ed9c0,
  stored into d2_TextSegs[].flags), d2_MiscPic (0x5fe74c = d2_FindPic("_MISC"), the 'i' command icons),
  d2_ResizeWindow, d2_ReplaceInputText. `uv run spell` reloads (46fdb2 46fdfe 46fe4a 46fe6f 46fec8 46feed) re-read
  id/align slots; both are call results / command state, no expression to re-read. Q2 gap: D2WIN.unk30
  (written 1 by the C command at d2.c:3029, read nowhere).
- d2_DrawWindowBackground (0x4713c0, was d2_Unk4713c0): byte, Q2 only. D2WIN.unk38c -> tileBackground named
  (the only use draws the tiled d2_BackgroundTile). Gaps: d2_Unk5fe71c (char, tested == 1 six times, no
  writer anywhere), D2WIN.unk270 (written 1 in d2_CreateWindow, compared < 2 / == 2; no writer of 2 in the
  source - and the 1 is in a draft, so a 2 is a meaning lead for d2_CreateWindow).
- d2_DrawWindow (0x471a90): near, 613/613 instructions, 45/1872 bytes, 54 lines - slot permutations of the
  frame locals (the (int)((float)..+1.5f) temps [esp+0x40]/[0x44], the scroll-button loop locals
  [0x50]/[0x54]/[0x58]/[0x68]/[0x6c]) = declaration order (vargen). Tried the add/sub source order at
  471fa7/471ff2: identical bytes, reverted. Named d2_WinBorderPic / d2_WinBorderPergaPic (0x5fe744/48 =
  _WIN_BORDER / _WIN_BORDER_PERGA), D2OBJ.unk1e0 -> winIndex (owner window index), d2_Unk475e40 ->
  d2_SetObjEnabled (sets clickDisabled/greyed). Gaps: unk270, unk3bc.
- d2_CreateScrollButtons (0x472270, was d2_Unk472270): byte, Q4 only - D2WO(o) is the byte form; d2_Wins[win]
  238/304 and a typed D2WIN* local 223/288 (both reverted). Needs a byte-neutral typed view (match-deep).
- d2_CreateWindow (0x472390, was d2_Unk472390): two meaning fixes, 740 -> 638 bytes / 167 -> 121 lines:
  `win->w = win` -> `win->w = w` (the original stores the width argument: mov WORD [esi+0x8],cx) and
  `desc[1] = win` -> `desc[1] = w` (movsx ecx,[esp+0x60] then desc[1]). Rest is allocation (free-slot scan
  registers, the -1 materialisation, the y+h add order; the last tried both ways, identical bytes). Named
  d2_LastWindow / d2_LastWindowPtr (0x6ed938 / 0x6ed980). Gaps: D2WIN.unk268 (written 2 here, compared == 1
  at d2.c:1967), unk274 (written -1 here only), unk3bc / unk3c0 (two more child objects, no writer), unk270.
- d2_ScrollWindowBy (0x472760, was d2_Unk472760): byte, Q4 only, D2WO(o); d2_Wins[win] 19/160 (reverted).
- d2_ResizeWindow (0x472800, was d2_Unk472800): now readable - Q4 paid off byte-neutrally (int *p =
  wn->objs; *p; p++ replaces the int-as-byte-pointer walk and the two *(int *)win), Q1 fixed. Behaviour left:
  438/608 bytes, 29 lines, code 40 / order 6 / fill 1, same instructions in another order (47284c, 472877,
  the clip expressions at 47298a / 472a01, fill at 472a5f). Swapping the clip operand order: identical bytes.

Renames this session (project-wide, references updated in src/ and include/):
d2_Unk46f840 -> d2_ScrollWindow; d2_Unk475aa0 -> d2_EnableObject; D2WIN.unk24c -> appliedScrollY;
D2WIN.unk250 -> scrollRemY; D2WIN.unk254 -> scrollRemX; D2WIN.unk258 -> scrollX; D2WIN.unk25c ->
appliedScrollX; D2WIN.unk260 -> scrollStep; D2WIN.unk3b8 -> scrollBar; d2_Unk46fba0 -> d2_DrawWindowText;
d2_Unkfa5f00 -> d2_TextLayoutX; d2_Unkfa5f04 -> d2_TextLayoutY; d2_Unk6ed9c0 -> d2_TextSegFlags;
d2_Unk5fe74c -> d2_MiscPic; d2_Unk472800 -> d2_ResizeWindow; d2_Unk473b30 -> d2_ReplaceInputText;
d2_Unk4713c0 -> d2_DrawWindowBackground; D2WIN.unk38c -> tileBackground; d2_Unk475e40 -> d2_SetObjEnabled;
D2OBJ.unk1e0 -> winIndex; d2_Unk5fe744 -> d2_WinBorderPic; d2_Unk5fe748 -> d2_WinBorderPergaPic;
d2_Unk6ed938 -> d2_LastWindow; d2_Unk6ed980 -> d2_LastWindowPtr; d2_Unk472390 -> d2_CreateWindow;
d2_Unk472270 -> d2_CreateScrollButtons; d2_Unk472760 -> d2_ScrollWindowBy.
Knock-on: cm.c / cm.h, rt.c / rt.h, hst.c, gfx.c, unk_4b39f0.c, unk_5d33b0.c got the new names; no byte
changed anywhere (check count 2548 before and after).
- Next in the queue: d2_Unk472a60 (0x472a60, byte, Q1/Q2).

## match-base session 2026-10-25 (worker base-3)

Raised to byte + readable (10): d2_FinishFades (0x472a60), d2_CreateChildWindow (0x472aa0),
d2_RemoveWindowObjects (0x472d20), d2_CloseWindow (0x472e00), d2_EnableObject (0x475aa0),
d2_ScaleObject (0x475e80), d2_WinOpenPicStyled (0x473350), d2_ReplaceInputText (0x473b30), d2_Open
(0x473c10), d2_LoadView (0x474b30). Knock-on readability (no work, only names freed): d2_StartFade,
d2_ReleaseFade (d2_Fades), d2_SetForm (d2_CurrentView). Unit base queue 38 -> 24 below behaviour + readable.

Escalated:
- d2_RaiseWindow (0x472fd0): B3, two code hunks that are reordered-only - the original hoists j=-1 out of
  the loop (or esi,-1) and keeps i in ecx / j in esi, ours keeps mov ecx,-1 at the loop bottom and swaps
  i=esi / j=ecx. Tried (each one edit + build, both WORSE 211/272, reverted): j=i only inside a break
  block with no j=-1 (and the same with the original's redundant !=0 text). `uv run spell`: no menu rule.
- d2_OpenForm (0x4741f0): far draft. Frame 0x244 vs the original 0x248 (Ghidra's lowest local is
  local_248) -> all slots shift by 4, one 4-byte local is missing (no candidate in Ghidra/source), 988
  code hunks. Readability done (Q1..Q7).

Findings / leads:
- D2WO(o2) -> typed d2_Wins[n] fields was byte-neutral here in d2_CreateChildWindow and
  d2_RemoveWindowObjects (n is the window index; the n*0x3c4 multiply is CSE'd once into the same lea
  chain the original has). It stays lossy in d2_CreateScrollButtons / d2_ScrollWindowBy, where the
  original keeps a local byte offset o = win * 0x3c4 - the local, not the index, is what the bytes need.
- D2WIN.unk3bc = cornerObj: a child object outside objs; when != -1 the top-left corner piece is not
  drawn (both frame branches of d2_DrawWindow). D2WIN.unk3c0 = extraObj: second such object, only
  enabled/removed with the window. The raw byte views D2W_U3BC/D2W_U3C0 were dropped for the typed
  fields in d2_EnableObject (byte-neutral).
- D2OBJ.unk70 -> picScale: the picture scale (0/1/-1 unscaled, > 1 larger, < -1 smaller), stored by
  d2_ScaleObject and read by d2_DrawShapeScaled in d2_DrawObjects. Do not reuse the name `scale`:
  D2OBJ already has an unrelated `scale` at 0x2e4.
- d2_Open: d2_Unkf8eee0 = d2_MainWindow (the HWND; main.c:429 stores 0x6f2588 into 0xf8eee0 right
  after the call), d2_Unkf8ef80 = d2_GfxDir (path truncated at the last backslash), d2_Unk6ed99c =
  d2_RedrawInfo2 (allocation tag "d2:d2_RedrawInfo2"), d2_Unk5fe740 = d2_ButtonPulldownPic
  ("_BUTTON_PULLDOWN"), d2_Unk5fe720 = d2_PixelFormat = dd_PixelFormat (dd_Unk14635a4, now
  dd_PixelFormat: 888 - RLoss*100 - GLoss*10 - BLoss = 888/565/555). D2VIEW.unk00 -> id.
  *(int *)d2_Pics = 0 is memset(d2_Pics, 0, 4) (same one dword store).
- d2_OpenForm: ~300 call sites (each menu/cutscene form: "cutscenes\..."), so the name is solid. The
  free-view scan guard 0xfb91e4 is a hardcoded constant in the original (0x474376 cmp ecx,0xfb91e4),
  it does not line up with d2_Views + 0x30 (0xfb6060); kept as D2_VIEW_END_GUARD with that evidence.
  d2_Unk6ed984 -> d2_FullFormPath: only the ==0 read (prefix d2_FormsDir + ".form") and one store of 0
  exist in the whole binary (2 refs, both in 0x4741f0), so the nonzero branch is dead - the name
  states the branch's role, the header comment says nothing sets it.
- d2_LoadView / the view surfaces: D2VIEW.unk1a4 -> viewSurf (the composite, blitted to DD), unk1a8 ->
  texSurf (the two textures stacked), unk1a0 -> enabled (set by d2_Unk475590), unk194 -> firstObj
  (d2_HighestObj - 1 at open). D2FORM0.unk388 -> bgpic (stored into the window's 0x388 = D2WIN.bgpic;
  written through ww->pic, the existing D2WINF view of that slot - d2_Wins[...] there changed the
  allocation of the far draft and was reverted).
- D2OBJ.label[0x24] is a typed union view D2OBJ.insertedLen at 0xc0 (length of the text the input last
  inserted into the window text); the two *(int *)&o->label[0x24] uses are gone. D2FORM2.alpha got an
  int view alphaBits for the same type pun.

Renames this session (project-wide, references updated in src/ and include/):
d2_Unkf8f0a0 -> d2_Fades; d2_Unk472a60 -> d2_FinishFades; d2_Unk472aa0 -> d2_CreateChildWindow;
D2WIN.unk3bc -> cornerObj; D2WIN.unk3c0 -> extraObj; d2_Unk475e80 -> d2_ScaleObject;
D2OBJ.unk70 -> picScale; d2_Unk5fe608 -> d2_FormsDir; d2_Unk6ed958 -> d2_CurrentView;
d2_Unk5fe740 -> d2_ButtonPulldownPic; d2_Unk5fe720 -> d2_PixelFormat; d2_Unk6ed99c -> d2_RedrawInfo2;
d2_Unkf8eee0 -> d2_MainWindow; d2_Unkf8ef80 -> d2_GfxDir; dd_Unk14635a4 -> dd_PixelFormat;
D2VIEW.unk00 -> id; D2VIEW.unk194 -> firstObj; D2VIEW.unk1a0 -> enabled; D2FORM0.unk388 -> bgpic;
d2_Unk4741f0 -> d2_OpenForm; d2_Unk6ed984 -> d2_FullFormPath; D2VIEW.unk1a4 -> viewSurf;
D2VIEW.unk1a8 -> texSurf.
Knock-on units: sv.c (ai_Unk472a60 call -> d2_FinishFades, .text compared identical), dd_vesa.c/.h,
cm.c/cm.h, gv.h, rt.c/rt.h, f3gm.c, ord.c, unk_4b39f0.c, unk_5d33b0.c (call sites of d2_OpenForm).
No byte changed anywhere (check 2560/4664 before and after; ledger check --against 0 regressions).
- Next in the queue: d2_Unk474ca0 (0x474ca0, byte, Q1/Q2/Q6).

## match-base session 2026-10-25 (base-3 worker, shard 3/6, session 2)

d2.c 110/130 matched before and after (12781 -> 12782 differing bytes: the d2_StartFade field reorder, no
function lost a match); the base queue of the unit is EMPTY. 18 functions raised to byte + readable:
d2_SetViewTextures / d2_ClearViewTextures / d2_FillViewTextures (the view texture pair: d2_Unk474ca0 / 474d10 /
474d90, roles already recorded in the d2m report), d2_SetViewEnabled (475590, the view's object enable state;
D2VIEW.enabled), d2_PlaceWindowScaled (475730), d2_PlaceMainWindow (475850, the first window of a view), \
d2_PlaceViewWindows (475870), d2_Plot (476220, the "d2_Plot:Window is not a paintbox!" message), \
d2_ClearPaintbox (476710, "d2_ClearPaintbox" messages), d2_FillPaintbox (476760), d2_DrawFade (4768a0), \
d2_FreeFades (476b40), d2_ClearInput (476b70), d2_DissolveStep (476110), d2_DrawFades (476b10), \
d2_SetObjEnabled (475e40), d2_SetObjMax (475f40), d2_GetObjValue (475980). Readability-only raises (behaviour
still draft): d2_PlaceWindow (4756b0), d2_RedrawView (474e80), d2_PlotLine (4764f0, was d2_Unk4764f0),
d2_StartFade (4767d0).

Escalated to match-deep (reasons in the @deep tags): d2_CloseView (Q4: D2VO is the original's byte-offset
spelling; typed access loses the match - the earlier d2n lesson confirmed), d2_GetObjPicSize (Q4: the
p+0x45 frame-offset read loses the match as a typed D2PICFRAMETAB view), d2_RedrawView (far, 114 code hunks,
the d2m attempts re-read), d2_PlaceWindow (the parent load is CSE'd / tail-merged, the d2n tries re-read),
d2_PlotLine (far, 321 code hunks; the in-arm e update made it worse), d2_StartFade (one code hunk: the
out-of-line second return-0 block; gained 21 -> 9 differing instructions with the struct-order field stores).

Meaning found: d2_StartFade must assign f->x / f->y / f->w / f->h in that order (the original hoists the y and
w parameter loads, not h and y); d2_DissolveStep's d2_Unkf8abc8 is the current dissolve record (d2_CurrentDissolve);
0x6ed97c is the paintbox pen size (d2_PenSize - the "d2_Plot" switch uses it as 0/1/2, the statistics chart
sets it 1/0 while drawing); 0x5fe71c is the alternate-background flag (d2_NoBackgroundRestore, still a name
by role - no writer exists in the source); D2VIEW 0x188/0x18c are the view's drawing origin (viewX/viewY,
subtracted from window / object coordinates in d2_RedrawView).
Dropped aliases: d2_Unk4611b0 (d2.h) is tex_SetHiColor (d3.h) and d2_Unk448270 (d2.h) is d3if_DrawSprite.
D2TEXX.unk60 -> surf (D3FLTEX.surf at 0x60).
Rest of the unit's base queue: none. Unit readability still blocked by the d2.h canonicalisation (the foreign
declarations: f3gm.c, f3init.c, he.c, rt.h, ... - the d2.h cleanup scope in Open work).

## match-base 2026-10-25 (base-4 worker, shard 4/6)

- 0x475850 was named d2_PlaceFirstWindow with a second flag parameter (`void d2_PlaceFirstWindow(int view,
  int fl)`, forwarded to 0x475730) on the base-4 branch. The merge keeps the unit owner's byte-matched
  d2_PlaceMainWindow (475850) with its K&R prototype; callers still pass the flags through the two-argument
  cast, as cm.c does. base-4's two-parameter spelling reverted.
- 0x475590 renamed d2_Unk475590 -> d2_SetViewEnabled (0x4754d0 already was d2_EnableViewObjects - do not
  fold them), 0x475870 -> d2_PlaceViewWindows.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

The shard's base queue was empty (the 7 names items were all `@deep` with recorded evidence gaps); this session
took the Q4 macro/name class of the unit's readability work, where the *typed* spelling is byte-neutral:

- **D2WO -> D2WOP / D2VO -> D2VOP** (typed pointer view instead of the dereferencing macro): the macros
  `(*(D2WIN *)((char *)d2_Wins + (o)))` / `(*(D2VIEW *)((char *)d2_Views + (o)))` failed Q4 as "raw macros";
  the pointer twins `((D2WIN *)((char *)d2_Wins + (o)))` + `->field` / `D2VOP(o)->field` are the *same AST*
  (`p->f` is `(*p).f`), so VC6 emits the same bytes - including the byte-offset addressing with the offset in a
  register that the index form loses. Check 2610/4664 unchanged, no unitdiff WORSE.
  Cleared: **d2_CreateScrollButtons** (0x472270), **d2_ScrollWindowBy** (0x472760), **d2_CloseView** (0x4758f0) -
  all three byte + readable now (their @deep tags are gone; the reasons stay in their comments as the record).
- **d2_GetObjPicSize** (0x4759f0): the picture's per-frame offset table read as a local `int *frameOffsets =
  (int *)(p + 0x45); ... p + frameOffsets[o->input]` - the pointer local keeps the `[reg+idx*4]` addressing, a
  typed struct view does not. Byte + readable.
- **D2OBJ.unk1e4 -> fillFraction** (project-wide, 10 references, all in d2.c/d2.h): the fill fraction of a fill
  / slider object. Evidence: the only reads clip the picture at `(1 - fillFraction) * h` from the top and print
  `"%i%%"` of `min(fillFraction, 1.0) * 100`; 0.0 = not a fill object. Raised **d2_DrawInput** to readable.
- **D2WIN.unk270 -> backgroundState** (project-wide, 10 references): the window background's save/restore state.
  Evidence is the field's three reads in d2_DrawWindowBackground, which pin the values down: `== 2` blits the
  window's area *from* surf (screen) *into* surf0 (save), `< 2` blits surf0 onto surf (restore, window without a
  caption), and d2_DrawWindow decrements it after each draw; d2_CreateWindow starts at 1. The writer of 2 is
  outside the drafted sources (recorded). Raised **d2_DrawWindowBackground** to byte + readable.
- Still draft (evidence gaps, unchanged): d2_Exit (Q3 0xfa5d20 + the four `*(int *)` picture-table accesses -
  the typed form swaps the sib base/index; the Q3 constant *is* &d2_Wins[0x60].active (w starts at
  d2_Wins + 0x280 = .active = 0xf8f3a0, the end 0xfa5d20 = +0x60 * 0x3c4), but the typed comparison
  `while (w < &d2_Wins[0x60].active)` loses the byte match (2609) because the original compares against the
  immediate - the literal stays; plus 9 Q2 names), d2_OpenInput (unk144
  write-only), d2_DrawObjects (unk00 = d2_AnimView[] sound state with no writer, unk58 = drawn half dark, unk6e,
  both write/read-only with no statable role, unk268 = the "== 1 draws the window into the back buffer too"
  state whose 1-writer is outside the sources, + d3io_NoopCallback), d2_DrawWindowText (D2WIN.unk30, written 0 by
  the 'C' text command and read nowhere), d2_CreateWindow (unk268 / unk274, the latter write-only).
- The unit's readability went 118 -> 125 readable (110 -> 115 byte + readable); 7 readability rises, 5 @deep tags
  cleared (d2_CreateScrollButtons, d2_ScrollWindowBy, d2_CloseView, d2_GetObjPicSize, d2_DrawWindowBackground), two
  functions became readable at draft behaviour (d2_DrawInput, d2_DrawWindow).
- The unit's 413 foreign declarations are untouched (the d2.h canonicalisation in Open work: d2.h has no
  includes and declares vfs_* / mem_* / err_* itself; see the sweep plan below for the measured state).

### The 413 foreign declarations: a concrete sweep plan (2026-10-26, base-3, not started)

d2.h is the canonical header; the count is per declaration site, 16 files:
unk_58bbd0.c 112, sv.c 69, include/cm.h 45, unk_575860.c 43, unk_5d33b0.c 36, include/rt.h 25,
unk_589f10.c 20, he.c 13, unk_4b39f0.c 12, main.c 8, f3init.c 8, include/unk_5d33b0.h 8, io_text.c 4, f3gm.c 4,
include/gv.h 4, hst.c 2. Almost all are `int d2_X();` style K&R copies inside a block of forward declarations
at the top of the .c (unk_58bbd0.c:263..) - i.e. the fix is "delete the copies, `#include "d2.h"`", and the
K&R -> prototype change is the byte risk to measure per unit.
Order to do it in (each step one unit, full ledger check after each):
1. include/hst.c's 2 (hst.c is small) then the .c files with few sites (io_text.c 4, f3gm.c 4, hst.c): they
   probably need d2.h after their existing includes; d2.h uses only forward-declared DirectDraw types, so the
   TU needs <windows.h>/<d3d.h> first (all of them have it).
2. The three headers (include/cm.h, include/rt.h, include/unk_5d33b0.h, include/gv.h): a header including d2.h
   must not pull a struct that its includers already define; check the X conflicts with `uv run lint`.
3. d2.h itself declares vfs_Tfopen / vfs_fread / vfs_fseek / vfs_fclose / vfs_Root (0x690..694), mem_Alloc /
   mem_Free (0x722) and err_Message (0x702) - other units' functions. Those must go before the sweep can be
   independent: `#include "vfs.h"`, `#include "mem.h"`, `#include "err.h"` in d2.h (vfs.h itself includes zlib.h/unzip.h, so
   check that no includer's types conflict - the "d2.h cannot be included" note for unk_5d33b0.c is about this).
The whole sweep is byte work in other shards' units: check `unitdiff --against` of every touched unit, and expect
a prototype-vs-K&R flip in one or two callers (see notes/vc6-matching.md).
- 2026-10-26 check: `#include "vfs.h"` before `#include "d2.h"` in d2.c compiles cleanly and leaves the check at
  2610/4664 (no d2.c function flips), so the "d2.h and vfs.h clash" premise does not hold in a TU that has
  <windows.h> + <d3d.h> first - the sweep in the plan above is viable, the vfs_* / mem_* / err_* copies in d2.h
  are ordinary foreign declarations to move (experiment reverted, d2.c unchanged).
- 2026-10-26 verification: a disassembly scan of the whole d2_DrawWindowText (0x46fba0, 0x1810 bytes) finds
  exactly one access to a D2WIN +0x30 field - the store of 0 in the 'C' command at 0x4703df (`mov [esi+0x30],eax`)
  - so D2WIN.unk30 is write-only in the original too: the Q2 name gap stands, it is not a missing statement.
- 2026-10-26 access scan of the whole d2.c range (0x4687a0..0x476aa0) for the four remaining Q2 fields:
  D2INPUT +0x144 (unk144) has exactly one access, the store `mov [esi+0x144],edx` at 0x46a1ae (d2_OpenInput);
  D2OBJ +0x6e (unk6e) exactly one, the read `mov al,[esi+0x6e]` at 0x46d481 (d2_DrawObjects' frame clip);
  D2WIN +0x268 (unk268) exactly two, the store of 2 at 0x472664 (d2_CreateWindow) and the read the source has
  at d2.c:1967 - the original spells it `cmp WORD PTR [eax+0xf8f388],0x1` (0x46dcd9, the absolute-plus-index
  form of d2_Wins[idx].unk268, so a plain `grep 0x268` misses it); D2WIN +0x274 (unk274) exactly one, the store
  of -1 at 0x47268c. Every one is therefore a single-site field in this unit and none of them gets a role from
  the d2.c code alone: the Q2 gaps are real, not missing statements.

## play 2026-10-29 (the glyph byte)

`d2_DrawTextLine` (0x46ead0) declared its glyph variable as a signed `char c`. Any text byte at or above 0x80
(the German umlauts) sign-extended to 0xfffc.. in `d2_GetFrame`'s `bank->offs[(unsigned short)i]`, i.e. it
read up to 256 KB past the font bank: the returned "frame" was whatever lived there, so the glyph was garbage
and a large enough text crashed (`mov dx,[edi+0x1a]` with a heap pointer). The original passes the glyph
zero-extended - 0x46ee33 `movzx dx,dl` (the normal arm) and 0x46eef3 `xor dx,dx; mov dl,al` (the highlighted
arm) - so `c` is an `unsigned char` now. The other text drawers (d2_TextWidth 0x46ea10, d2_DrawText 0x46f000,
d2_DrawTextBox) already had it. Debt: 470 asm / 1109 B (11 fewer), the function is still behaviour draft.
