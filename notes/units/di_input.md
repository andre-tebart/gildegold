# di_input.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): registry helpers, key map, DirectInput (di_*); not dinput.h (SDK header)
- Boundaries: separated (plausible): coherent di_* (registry, key map, DirectInput), own string block 0x5f3244..
- Range then: 0x4103a0..0x4115b0
- Was: unk_4103a0.c (round 12)

## Open work
- 2026-10-25: di_UpdateButtons is with match-deep (both scan forms re-tried, 180 lines WORSE; see the session section at the end); the unit-level readability work left is the 22 foreign declarations (declaration -> include of di_input.h in the callers). The items below are the earlier history.
- di_UpdateButtons 0x410ac0 (match-tu 2026-10-23, 2nd run): still 7 lines. c2prio --refs on base vs for-loop variant: the zero web's 49 counted references are identical except in the scan's tail blocks (base: blocks 76 P=1 / 80 P=4 / 82 P=2; loop: 76 P=4 / 79 P=4 / 81 P=2, all cost 0), so the +3 is the `-P per unreferenced block lived through` term of the tail: the loop form has 3 P-points fewer of lived-through blocks. PressX's web (-57) is the LEFT part's web only (uses at line 383, defs at 391 / 424; the right / middle compares belong to other webs). Tried on the loop variant: respell round + climb (118 variants: 180 -> 169 lines via one && order, then nothing), 10 tail spellings (`i != 31`, `dm_Q[++i]`, dec order, unsigned bound compare, init order, goto-done form, valid-first: 180-184), 7 code-free insertions in the left press block ((void) reads, `now += 0`, self-assignments, `if (0)`: all folded, identical). Lead: a REAL extra PressX reference in a P>=2 block of the left part (or one more lived-through P=3 block for the zero web in the tail) on top of the for-loop scan; uv run respell --on build/variants/di_loop.txt:for_loop starts from it.
- di_UpdateButtons 0x410ac0 (match-tu 2026-10-23): 7 lines -> unchanged, but the mechanism of the last hunk is found: the scan written as a rotated top-tested loop, `for (i = 31, q = &dm_Q[31].valid; *q == 0; ) { q -= 19; i--; if ((int)q < (int)&dm_Q[0].valid) { i = 0; goto put; } }` with the `&& dm_Q[31].valid == 0` dropped from the if, gives the original's `mov ecx,[Q31.valid]; mov eax,0x1f; test ecx,ecx; jne` and the whole scan (`uv run idioms` `init_before_guard`); the price: c2prio shows the const#0 web lifted from -58 to -55 past _di_PressX (-57), so the zero takes ebp and PressX loses its register (180 lines; while / init-outside forms identical). respell 119 variants: no gain. Lead: on the for-loop form, +3 priority for _di_PressX or -3 for that const#0 web (one reference in a block of its P), read with c2prio, then the 7 lines should close.
- match 2026-10-23 (8th), no gain: di_UpdateButtons last 2 groups (`mov ecx,[Q31.valid]; mov eax,0x1f; test`): nested `if (changed) { i = 31; if (valid == 0) {...} }` (also `(i = 31) != 0 &&`, `q = &dm_Q[i].valid`), `i = 31` / `q` before the whole if with `dm_Q[i].valid` / `*q` in the condition, `*(q = &dm_Q[31].valid) == 0` in the condition: nesting rebuilds the whole jump structure (458-506 B differ, 176-321 lines), the rest equal or worse (7 lines stay).
- match 2026-10-23 (7th): di_UpdateButtons 551 B / 104 lines -> 7 differing lines. What did it: (1) while the registers were still wrong, `now - di_LDownTime >= 16` instead of `> 15` gave the original's ecx = LDownTime / eax = now - LDownTime; once the rest matched plain `> 15` is right again (`cmp 0xf; jg`); `>= 40` in the second LClickTime test (original `cmp 0x28; jge`); (2) the left-button block without gotos: `if (click) { di_LDownTime = 0; di_LClickTime = now; if (lhold) { lup = 0; di_LClickTime = -1; } lhold = 0; } else { lextra... }` and the else arm `di_LDownTime = 0; di_Unk6310bc = 0; ldown = 0; lup = 0; LClickTime = -1; lhold = 0;` (VC6 merges the tails itself; the original's extra `mov [LDown],ebx` is that first store); (3) the press stores in the order `di_xDownTime = now; di_PressX = ...; di_PressY = ...;` in all three blocks (a perm round: the time store sits between the volatile reads and the position stores); (4) middle-button block `dm_S.mhold = 1; di_MDownTime = 0;` (ecx is known 0 after `mhold == 0` was tested: with `MDownTime = 0` first VC6 stores ecx instead of the immediate); (5) the queue scan with `int *q = &dm_Q[31].valid`, `q -= 19`, `(int)q < (int)&dm_Q[0].valid` (signed compare, `cmp [ecx],0`) and `i++; dm_Q[i] = dm_S;` at the put label. Left (2 groups): the original loads `dm_Q[31].valid` into ecx with `mov eax,0x1f` (i = 31) scheduled between the load and the test (ours: load into eax, test, then the constant): `i = 31;` before the if / `int i = 31` / `(i = 31) != 0 &&` inside the condition / after the loop, and a `v = dm_Q[31].valid` local: all unchanged (VC6 sinks the constant to the loop head).
- match 2026-10-23 (5th): di_UpdateButtons 581 B / 252 lines -> 551 B / 104 lines: the dm_S.x / dm_S.y reads of the press-position stores (`di_PressX = dm_S.x; di_PressY = dm_S.y;` in the left and right button blocks, 4 reads) as `(*(volatile short *)&dm_S.x)`: without them VC6 CSEs dm_S.x into bx for the whole function, which takes ebx from the zero constant; with them the registers are the original's (ebx = 0, esi = now, edx = -1, ebp = PressX). General lesson (vc6-matching.md): a callee-saved register that the original gives to a constant / counter is taken in ours by a CSE'd global read; a volatile read at a few uses stops the CSE (all occurrences volatile, or the compare reads: far worse; 210 masks of the other 14 reads: none under 104 lines). Left: the LDownTime / x temp registers (original ecx = LDownTime, eax = x; ours the other way) and the order of the `di_LDownTime = 0` / `di_LClickTime = -1` stores after the first block (`xor ecx,ecx; mov eax,edx` vs ours `xor eax,eax; mov ecx,edx`).
- match 2026-10-23 (4th): di_UpdateButtons not changed. Same cause as d3_snd / evt_Step / mov_Find / d3_FindBGF: the original keeps a constant (0 or -1) or a zero-valued variable in a callee-saved register and compares / stores through it; the register order follows the candidates' c2prio weights, so only reference weights (more / fewer uses of the variable or constant, dropping a local that is a CSE temp) can change it.
- match 2026-10-22 (2nd pass): di_UpdateButtons (581 B) has the right logic; the difference is the original's zero register (ebx = 0 for the whole left-button part: `cmp reg,ebx`, `mov [g],ebx`) and the cached `now` in esi (ours: bx caches dm_S.x at entry, edx = now). `int z = 0` / `i = 0` used in place of the zeros is constant-propagated (identical code); volatile x / y reads are far worse. Needs a zero the compiler cannot propagate.
- match 2026-10-22: di_GetMouse matched with `i = 0;` as the first statement and `for (; i < 32; i++)` (the xor of the counter is scheduled before the six zero stores).
- see notes/units/d3_interface.md, Open work: "Round 9 header cleanup: unk_40d680.c's d3_Unk449de0 / d3_Unk44a170 are char d3_SetLightSet ..."

## Worker dinput (round 9): 0x410770..0x411680 [dinput]

### Report
- Scope: 9 functions in src/game/unk_4103a0.c (TU continues from sndreg's part; it ends at 0x411680,
  where d3_tools.c starts; nothing else lives in the range, the only non-code bytes are the switch table of
  di_PollMouse at 0x4112b4..0x4112e0 and nop padding). No functions were missed by Ghidra.
- Nonmatching (2): di_UpdateButtons 0x410ac0 (register allocation, 581/1024 bytes), di_GetMouse 0x411440
  (14/368 bytes, one `xor ecx,ecx` placed after instead of before six stores).
- Renamed (names.csv, source, header): sndreg had the devices swapped. 0x1467360 is the **mouse**
  (GUID_SysMouse, c_dfDIMouse, buffered data 0,4,8,0xc,0xd,0xe) and 0x14673cc the **keyboard**, so
  0x410710 is di_AcquireMouse and 0x410740 di_AcquireKeyboard. No other file uses them.
- New names: di_Init(hwnd, hinst, flags; flags 1 = keyboard exclusive+nowinkey, 2 = mouse nonexclusive/background,
  4 = cursor follows GetCursorPos), di_Exit, di_UpdateButtons (hold / double click timing, event queue),
  di_SwapButtons (swap left/right when di_SwapLR), di_PollMouse, di_PollKeyboard, di_SetMousePos, di_GetMouse
  (latch the next queued event), di_ClearEvents(mask).
- Data: DIMOUSEST (0x4c bytes, include/unk_4103a0.h) is the mouse state: dm_S working state 0x14673e0, dm_L latched
  0x1467380, dm_P previous 0x14683c0, dm_Q[32] queue 0x1467440 (valid flag at +0x48). The old di_A/di_B/di_C
  arrays are now `&dm_L.ldown`, `&dm_P.ldown`, `&dm_S.ldown` (macros in the .c). kb_Down 0x14681c0,
  kb_Pressed 0x1468420, kb_Last 0x1468410, kb_RepeatTime 0x631098, kb_RepeatKey 0x6310e0 (dword written as byte).
  Initial values from the exe image: di_MaxX 790, di_MaxY 599, di_Absolute 1 (0x5f3238..40); the 0x6310xx
  values in the image look like runtime garbage and are not reproduced.
- Shared headers: none changed (tb_Calls 0x6333d8 is redeclared `volatile int` in unk_4103a0.h, same as
  unk_42e7b0.h).
- Quirk kept from the original: the queue insert in di_UpdateButtons never uses slot 0 (insert index is
  last-used + 1, and 0 + 1 when empty); di_Exit releases the keyboard but then clears di_Mouse.

### VC6 lessons
- `kb_Last = 0; while (1) { count = 1; ... }`: `for (;;)` is rotated (duplicated loop head), `while (1)` is not.
  The mouse loop sets `count = 1` once before the loop (the call returns 0 or 1 in it).
- `(od.dwData & 0x80)` as a branch gives `test al,al; jns`-style byte tests; `*(char *)&x < 0` gives `jge`.
- Three identical "press / release" cases written `if (d & 0x80) {...} if ((d & 0x80) == 0) {...}` (not a
  local, not else): VC6 keeps `and edi,0x80` in a register and does NOT merge the two blocks' calls; with a
  local `hi` or `else` it merges the tails and hoists 0x80 into a register.
- float `(int)(kb_Base * 0.00390625f * data)` gives `fild; fimul; fmul` (constant last); the original's
  `fild; fmul const; fimul` needs `(int)((float)(kb_Base * 0.00390625f) * (int)data)`.
- Computed-address pairs (`add [x],dx` after `pt.x - L.x`, 16-bit store back): statement order matters:
  `S.x += pt.x - L.x; S.y += pt.y - L.y; pt.x = L.x; pt.y = L.y;` (all deltas first, then both writes).
- A big struct field-by-field copy whose store of 0 (`Q.valid = 0`) is *last* in the source gets scheduled
  early in the asm but keeps edx (the zero) reserved, so all temporaries use ecx only (serial copy); with the
  store written early the scheduler pipelines the copy through edx too (di_GetMouse).
- `if (di_SwapLR != 0) {swapped block} else {plain block}`: the fall-through block is the `if` body.
- A function that reads a volatile global many times but whose original loads it once uses a local
  (`int now = tb_Calls;` at the top, before an early `return`); with a plain int global the load is also hoisted
  but PollKeyboard (two separate reads) needs the volatile.
- A 16-bit global used by many comparisons (dm_S.x) is promoted into bx by VC6's global allocator and the
  constant 0 is then not hoisted into ebx; making the reads volatile restores the original's `xor ebx,ebx`
  (but the rest of the allocation still differs).

### Nonmatching notes
#### di_UpdateButtons (0x410ac0)
Control flow and data flow match (same blocks, same order, same instructions apart from registers); the
original keeps 0 in ebx, `now` (tb_Calls) in esi, LDownTime in ecx, -1 in edx, LClickTime in eax, di_PressX
in ebp for the whole function, and does not promote dm_S.x/dm_S.y or di_PressY (loads `movsx eax,WORD PTR`
at each use, one load per condition chain). Ours promotes dm_S.x into bx and loses the zero register.
Tried (about 20 variants): `>` vs `<` operand directions (fixed `x > px + 3` form), `int now = tb_Calls` (matches
the esi load before the early return), locals x/y per chain, an __inline Moved() helper with and without
arguments, dropping `dm_P = dm_S` or the queue copy (changes allocation but not towards the original),
volatile dm_S.x/y through macros (restores the zero register, loses PressX promotion).
#### di_GetMouse (0x411440)
Only difference: the loop counter's `xor ecx,ecx` comes right after the if (di_Absolute) block, before the six
clears, in the original; ours puts it after the clears (loop preheader). Tried `int i = 0`, `i = 0;` at every
position among the clears, `unsigned i`, while/do-while/pre-increment/`<= 31`/`!= 32` loops, a q pointer, char
arithmetic on dm_Q, casts/parens on the copies: no effect (or worse).

## Also about this unit
- notes/units/ecmd.md, section [sndreg]

## match-base 2026-10-25 (base-3 worker, shard 3/6)

- di_UpdateButtons 0x410ac0 -> match-deep; readability raised (draft + readable). The 4 Q4 casts
  `(*(volatile short *)&dm_S.x / ...)` are now `dm_S.vx` / `dm_S.vy` through a volatile anonymous-union view
  of the position fields in DIMOUSEST (di_input.h); the bytes are identical (69/1040, 7 asm lines). The one
  remaining code hunk is the queue-scan tail's register choice: the original materialises i=31
  (`mov eax,0x1f`) between the `dm_Q[31].valid` load and its test, so the load takes ecx; ours sinks the
  constant into the loop preheader, so the load takes eax. `uv run spell` has no menu rule for either hunk. The two
  Open-work mechanisms were re-tried against the current source, each one edit + one build, both reverted as
  WORSE (458 bytes / 180 asm lines, identical for both): (a) the for-init form
  `for (i = 31, q = &dm_Q[31].valid; *q == 0; )` with the `&& dm_Q[31].valid == 0` guard dropped; (b) the
  comma form `(i = 31, q = &dm_Q[31].valid, *q == 0)` inside the guard, and (c) the guard
  `changed && (i = 31, dm_Q[31].valid == 0)` with q initialised in the body (458 bytes / 176 lines). Every form
  that materialises i before the guard test merges the loop head and collapses the allocation. What remains is the c2prio
  register-priority shift recorded below (const#0 at -55 against _di_PressX at -57) = allocation work.
- 15 Q6 descriptions added (reg_CloseKey, reg_SetString, kb_BuildMap, kb_FindKey, kb_SetBase, di_ClearAxes,
  di_Error, di_AcquireMouse, di_AcquireKeyboard, di_Exit, di_SwapButtons, di_PollMouse, di_PollKeyboard,
  di_SetMousePos, di_ClearEvents). The unit is function-readable 21/21.
- Unit readability still open: the 22 foreign declarations (he.c, main.c, sv.c, unk_575860.c, unk_58bbd0.c,
  unk_5d33b0.c and the headers cm.h, d2.h, d3_d3d.h, rt.h - the old spellings ai_Unk410620 (di_ClearButtons),
  ai_Unk410690 (di_ClearAxes), ai_Unk410610 (kb_SetBase), ai_Unk410710 / ai_Unk410740 / ai_Unk410a50,
  ai_Unk4113f0 (di_SetMousePos), ai_Unk4115b0 (di_ClearEvents) are still in use there). Each fix is a
  declaration -> `#include "di_input.h"` replacement in the caller, not a rename.

## match-base 2026-10-25 (base-3 worker, shard 3/6, session 4)

Q7 canonicalisation (22 foreign declarations -> 0), all by name, no byte changed:
- the placeholder call sites renamed to di_input.h's names: ai_Unk410620 -> di_ClearButtons, ai_Unk410a50 ->
  di_Exit, ai_Unk410710 -> di_AcquireMouse, ai_Unk410740 -> di_AcquireKeyboard, ai_Unk410610 -> kb_SetBase,
  ai_Unk410690 -> di_ClearAxes, ai_Unk4113f0 -> di_SetMousePos, ai_Unk4115b0 -> di_ClearEvents, in he.c,
  main.c, sv.c, cm.c, unk_575860.c, unk_58bbd0.c, unk_5d33b0.c (all now include di_input.h).
- unk_58bbd0.c's no-argument `ai_Unk4115b0()` was a meaning error in a ROUGH draft: the original call at
  0x59035d pushes 2 (mov ebx,2 / push ebx), so it is di_ClearEvents(2) now.
- the shared headers lost their copies: cm.h (ai_Unk410620, di_ClearEvents), d2.h (di_SetMousePos,
  di_ClearButtons), rt.h (di_ClearButtons), d3_d3d.h (reg_GetString); d2.c/gfx.c already included
  di_input.h, rt.c / d3_d3d.c / cm.c include it now.
di_input.c: 21 functions, 20 byte, readability readable (di_UpdateButtons stays @deep).
