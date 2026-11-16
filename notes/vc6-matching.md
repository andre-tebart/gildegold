# VC6 SP5+PP /O2: matching notes by symptom

The cases behind `notes/vc6-quick.md` (read that first). One bullet per rule: **what the original shows where
ours differs**: the source form that gives it. `No lever:` what changes nothing. (the functions it was found on).
A function named without a remark is byte: `dcp fn NAME` prints the source that matched; `open` (in old notes also `stuck`) means
the rule moved it but it is not byte. Go to the section of your hunk, pick the sub-heading, or search by
instruction or construct (`grep -n "rep stosd\|fst \|dead parameter" notes/vc6-matching.md`). `conflicting notes,
measure` marks two observations that disagree: try both. Some notes name scripts that no longer exist
(`deep-tools *.py`, `uv run spell / respell / vargen / c2prio / unitdiff`): take the technique.

## Contents
- **Returns, epilogues, callee-saved pushes** (87 rules): Pushes on the wrong side of an early exit; A result in a
  register, a hoisted constant, a reload from its slot; Shared against separate failure blocks; Where a return block
  is laid out; Which copy of a cross-jumped tail survives, the join position; Search loops and their exits; Return
  type and small return forms; When an exit hunk resists
- **Branch layout, conditions, switches** (68 rules): How VC6 orders blocks and merges tails (c2.dll); Which arm is
  inline and which is out of line; Redundant tests
  the original keeps, threading; Jumps into another arm, goto chains, shared tails; setcc, defaults, conditional
  assignments; Compare operand order and the constant written; Unsigned, signed and float jcc; min / max and clamps;
  Switch dispatch, case order, empty cases; Caught only by dcp link (dcp build masks these); Functions brought to
  byte by several changes
- **Loops and induction variables** (89 rules): Decompiled pointer walks that were index loops; Loop pointer based
  on the wrong field; Row and pixel pointers; Rotated against top-tested loops; Search loops: hit path and re-tests;
  Counters, countdowns and increment order; Signed jl against unsigned jb, bounds and guards; Other shapes found on
  loop functions
- **Register allocation** (99 rules): How the allocator decides; Live range splitting, regions and the priority
  terms (c2.dll); Callee-saved registers swapped: a priority race
  lost by a point or two; A constant or zero kept in a register; Named local or expression: a candidate too many or
  too few; Pointer locals and addresses; The eax / ecx / edx rotation out of step; Loops: counters, running values,
  loop pointers; Frames and stack slots
- **Stack frame and slots** (71 rules): How the frame is laid out; Frame bigger or smaller: vectors and padded
  arrays; Frame bigger or smaller: buffers, records, temps; Frame temps of AI_ZERO / D3_ZERO / inline-asm macros;
  Slot order swapped, frame size equal; Dead parameter slots; Local kept in memory; Shared slots; Other findings
  filed here
- **Store, load and instruction order** (78 rules): How the back end orders instructions; Store or push placement
  around calls and `add esp`; Loads hoisted early or kept late; Reloads the original keeps (and CSEs it does not);
  Store order in fills, records and rects; Init and zero-store order; Operand order in sums, compares and addresses;
  Stores VC6 merges, drops or keeps
- **Calls, arguments, prototypes** (60 rules): Undeclared callees; Call written in both arms of an if/else;
  Parameter width: what the caller's push shows; Return types of callees; Argument count different from the
  definition (K&R victims, Q7); Evaluation order, nested calls, stack cleanup; Float, 64-bit and struct arguments;
  Varargs, inline, intrinsics, asm; Other call-site shapes
- **Floating point** (82 rules): Operand order of commutative ops (ranks); Product and sum chains: association,
  factoring, folding; Values kept on the FPU vs stored and reloaded; Float copies: fld / fstp vs integer moves;
  Compare idioms and flag tests; min / max macros and clamps; Constants: float vs double, pooled literals,
  reciprocals; Int / float conversions, division, _ftol
- **Types, widths, bitfields** (84 rules): A byte bit test turned into 0 / 1; Sign tests and signedness of compares;
  movsx kept or folded on narrowed values; Bitfields and byte read-modify-write; 16-bit values, 0xffff and -1;
  Multiply, constants, add / sub, division; Globals: width, type, unions, volatile; Struct layout, offsets and
  Q4-clean views; Indexing and address precomputation
- **Copies, fills, strings** (42 rules): Inline string copies, compares and lengths; Constant fills: memset against
  element loops; Zero stores and clears; Generic inline memset and asm macros (D3_ZERO, AI_ZERO, copy twins); Struct
  assignment against field copies
- **Declaration set, headers, unit order** (24 rules): Tie-break sensitive functions (sv_t's tutorial chapters);
  Callees defined above the caller (same-TU register knowledge); A missing call: side-effect-free body above the
  caller; Shared struct member lists (d3.h, d3_engine.h, ai.h); Declarations and includes in shared headers;
  Prototype order and lint errors
- **It was a meaning bug: reading rules** (36 rules): A slot or offset hunk that is a wrong field, index or
  variable; Locals, arrays and frame size; Conditions and jump targets; Types, constants, strings and arguments;
  Reading Ghidra's output and the original
- **Method and tools** (48 rules): dcp commands: what they do and do not do; Big diffs and function boundaries; The
  c2dag tools; Sweeps and twins; Functions given up on that were not allocation problems; A byte sibling, twin or
  published source is the source; Variant rounds; Base pass
- **This code base** (7 rules)
- **Open cases and rounds without result** (35 rules): A constant or flag in the wrong register; Register races
  between named values; Load and operand order; Hoisting, scheduling and strength reduction; Branch layout and
  shared epilogues; Frame slots and register homes

## Returns, epilogues, callee-saved pushes
Where the pushes and pops sit, how many epilogue copies there are, shared against separate return blocks, where the result's zero or constant is set, `goto fail` forms, cross-jumped exit code.

### Pushes on the wrong side of an early exit

- **Pushes at entry in ours, after a guard in the original**: count the `return`s first. Guards (`if (!p) return
  1;`, early `return 0`, `if (!a || !b || !c) return 1;`, an if/else-if strcmp chain) plus ONE final return
  shrink-wrap the pushes; any `return` inside a branch puts them back at entry. Each return beyond "guards + one
  final" is the suspect, not the guard. Several `ret`s in the asm do not mean several returns. snd_FindVoiceSample:
  `isvar = 0` at the top, `if (isvar != 0 && var >= 0) {..} else {..}`, one `return s`. No lever: five forms of the
  early test; GetSample in both arms (46). (snd_FindVoiceSample 21 -> byte, 5 sb_* functions, vfs_ftell)
- **Same for a result `s`**: `if (guard) return 0; s = f(); if (s) {...} else g(); return s;` pushes after the
  guard, a `return s;` in the success branch at entry. Separate `if (...) { ...; return 0; }` blocks push at entry.
  `if (a = list) {...}` pushes at entry, `a = list; if (a)` inside the branch.
- **`push esi` at entry, `push ebx` only after two `return 1` guards**: a `return 2;` inside the first of two `if (m
  == 3 ..) {..}` / `if (m == 1 ..)` blocks plus the same `return 2;` after them; `if {..} else if (..) ..; return
  2;` (one return) fixed it. No lever: the rest in an `else {}`. (ord_WageReportOrder byte)
- **`push ebx` at entry, ebp/esi/edi only before the loop, the loop test `dec; push..; test; jl` unfused**: the
  bound in its own local (`top = 30 / 42; for (i = top - 1; i >= 0; i--)`), every loop exit a `break` to one `return
  n` / `return cur;` after the loop. No lever: `for (i--; ..)`, `goto none`, else-chains. (amt_GetColumnOffices 12
  -> byte; gm_BuildNext 44 -> 8, open)
- **In-loop exit, push placement differs: read the original's `jge` target**: the shared final epilogue = `break`,
  its own `mov eax,reg; pop...; ret` copy = a real return. `if (n >= max) break;` fixed amt_GetColumnOfficesOwned
  (0x4e7040, 103 -> 61 code hunks); on the sibling 0x4e6fc0 the original keeps the in-loop return.
- **Result pointer pushed at entry, another register only inside a later if**: a second `return c;` inside that if
  (`if (flags & 4) { ..; return c; } return c;`). No lever: `c = 0` forms (c / bld swapped), c assigned before the
  id test. (gm_SpawnCharacter byte)
- **Original pushes in the prologue, ours shrink-wrapped after an if/else**: the statement after two identical arms
  (`players = g;`) written at the end of each arm; at their start or before the if moves the load instead.
  (lobby_ShowCity byte)
- **A late `push ebp`**: one final `return 1` after an if/else whose branches set the same field; two returns in the
  branches push at entry (g3, 0x493200).
- **`push ebx` deferred into a success block with a switch**: `if (ok) { ...; switch (...) { ... break; } return 1;
  } return 0;` (d2_Unk4799e0); `return 1` per case pushes at entry but can also be right (d2_Unk47bd70): try both.
- **Pushes at entry, the early exit jumping into the shared epilogue**: `T *obj = 0; if (ver >= X) {...} return
  obj;`; an early `return 0` shrink-wraps.
- **`mov eax,[esp+4]; test eax,eax; push ebx; mov ebx,eax`, null path returning eax without xor**: `VFSDIR *p = d;
  if (!p) return 0;`.
- **Push/pop around a mid-function region**: VC6 /O2 does this itself, no hint of block locals or inlining
  (d3t_DropObject).

### A result in a register, a hoisted constant, a reload from its slot

- **A void-looking init whose last statement tests a call's result, eax untouched to the end, ecx / edx for the
  branch's loads**: it returns the result (`ret = f(&x); if (ret == 0) {..} return ret;`, the caller ignores it);
  void is hmt_Reset's code. (hmt_Init 12 -> byte, returns hmt_ReadBinaryFile's result)
- **`mov eax,2` hoisted into the prologue, failures `mov eax,1` before the shared pops**: `int r = 2;`, `else r =
  1;` on each failing if, `return r;` at the end, and the path with a call returning its constant itself (`{ f(..);
  return 2; }`), else r lives in a callee-saved register; `r = 1` initial + `r = 2` at the join hoists the 1. Guard
  + search loop (`mov eax,1` before the guard's test): `r = 1; if (g == 0) r = 0; else { do { if (hit) return 0; ..
  } while (..); } return r;`. No lever: `if (g == 0) return 0; .. return 1;`, `r = 1` with early returns.
  (ord_DismissWorkerOrder 36 -> byte, ai_HasQuicksavePlayer 29 -> byte)
- **`mov eax,1` after the prologue, one path returning it untouched**: `int r = 1; .. if (mode < 0) { if (on) r =
  Lit(..); else r = Blit(..); } return r;`. No lever: `return f(..)` per call folds r (8 combinations = one code).
  (d2_DrawShapeScaled 66 -> byte)
- **A shared result constant-propagated away**: assign it on a path that reaches the common `return r`: `if (k == 3
  && player) { r = f(c, m); r |= 2; } else if (k == 1 && player) text(...); return r;`; `return f() | 2;` gives `or
  al,2` without the register (0x4f4ff0, 0x4f5c30, 0x4f6b40).
- **`mov esi,2 / ... / mov eax,esi` + copied epilogue on a loop's exit, ours `pop edi / pop esi / mov eax,0x2`**: `r
  = 2; break;`, not `r = 2; return r;` in the loop (hst_NextEvent / hst_NextEventCmd 0x4b3280 / 0x4b35e0, B3 -> B2).
- **Success `0` in a callee-saved register (`xor esi,esi` at entry, `mov eax,esi` at the end, `or eax,-1` on
  failure)**: `int ret = 0; if (...) {...} else ret = -1; return ret;` with every nested failure `else ret = -1`;
  the zero is reused for zero arguments and stores. Early `return -1`s give `xor eax,eax`. `ret = 1; ... return
  ret;` gives ret-in-bl / `mov al,1`, `return 1` does not. (all 25 MSS wrappers)
- **Stores to `ret` before each epilogue, `mov eax,edi` on constant-looking paths, `or eax,-1` at entry** (vfs
  style): `ret = x;` in every branch, one `return ret;`.
- **A success flag in bl (`xor bl,bl` at entry, failures `mov al,bl`, success `mov al,1`; ours `xor al,al`)**:
  nested `if (info && info->mesh) { .. if (box) { ..; ok = 1; } }`, the out-stores under `if (ok) {..}`, one `return
  ok;` (`goto done` the same). No lever: early `return 0` / `return ok`. (d3_GetBounds 15 -> byte)
- **Early `xor al,al`**: `char ret = 0; if (..) {..; ret = 1;} return ret;`.
- **A result reloaded from its stack home at a failure return (`mov ebp,[esp+N]; mov eax,ebp`; ours `xor eax,eax` +
  own epilogue)**: VC6 folds `return f;` to 0 when only `f = 0` reaches it. The failure path falls through to the
  final `return f;` (nested `if (e) { if (zip) {..} } else {..}`, the not-found statement after them under `if
  (!f)`, no `goto notfound`, no `goto done`), a missing entry is the `else` of `if (e)`. Same when every arm
  assigns o and one `return (int)o;` ends it; early returns let VC6 fold o into ebx. (vfs_Tfopen byte,
  gm_BuildingMainObject byte)
- **`mov eax, [ebp-4]` before a cross-jumped tail, entry store `[ebp-4] = 0`**: one variable for both arms (`AICHAR
  *c = 0;` searched into in one, `gm_FindChar` in the other), not `pick` + `c = pick` (ai_TokenRandomNpcInhabitant).
- **One variable in two registers (returned on one path, a call result on the other)**: one `return a;` at the end.
  `if (!p) error(); else p->x = y; return p;` returns p from esi; an early `return p` becomes `xor eax,eax`.
- **Failure value from a callee-saved register**: `else s = NULL;` + shared `return s;` (`xor esi,esi; mov
  eax,esi`), or break out of the loop then `if (!v) error; return v;`. Several paths: `int id = 0; ... return id;`,
  a `goto fail` target returning 0 after the final return.
- **A 0 failure return from a register later reused**: a `type = 0;` statement there + nested ifs + one `return
  type;`; `int type = 0;` zeroes another register at entry. Two separate `xor eax,eax` after the pops: set 0 before
  the outer `if`, assign inside, no early returns (ch_PlaceOnGround byte).
- **Stored to a table and returned, the original re-reading `movsx eax,BYTE [esi+t]` (ours `movsx eax,al`)**: the
  paths join before the one `return t[k];` (init_BuildingSceneUniverse byte).
- **A difference kept in an operand's register vs a fresh temp**: `held = gm_ListAmount(..); if (stock - held > n)
  return n; return stock - held;`, not a `room` local (Gm_FreeCapacity 12 -> byte).
- **A value returned on one side, a loop accumulator on the other**: one `return sum;` after `if (ok) { loop; sum /=
  64; }`; an early return swapped two stack slots (d3t_GetMeanHeight).
- **A global reloaded for the return**: `global = o->f; break;` + final `return global;`, not `return o->f;`.
- **A tested-then-returned failure value**: two variables, `t = f(x); if (!t && (t = f(y), !t)) { ret = ...; return
  ret; } return t;` (d3io).
- **-1 hoisted into eax with `__asm` in the function**: there a `ret = -1` local lives in memory.

### Shared against separate failure blocks

- **A `return <constant>;` merged into an earlier equal block (ours `test; je <earlier>`, original `jne <continue>`
  + own copy)**: the lever is the flow graph elsewhere: `for (n = 0x300; n != 0; n--) { if (found) break; .. } if (n
  == 0) return 0;` keeps the copy, `for (;;) { ..; if (--n == 0) return 0; .. }` folds. No lever: if/if!, braces,
  if/else, while, do/while, switch, nested if, reorder, dead statements.
- **One failure block + epilogue per step, ours one tail**: a second copy of the success return: `if (full) {
  ...steps...; close; restore; return 1; } close; restore; return 1;` keeps all 24 copies. No lever: else-chains,
  `!`, `<= 0`, a result local, `__inline`, `#pragma optimize`. (ls_SaveGame byte)
- **Inline `pop ..; xor eax,eax; pop ebx; ret` after every `fwrite` check**: `if (vfs_fwrite(...) <= 0) return 0;`
  (LS_W) at statement level; wrapping in `if (vfs_fwrite(...) != 0) { ... } return 0;` merges them (`jbe <end>`)
  (0x51b910, 0x51c450: 361 code hunks -> two loops).
- **One shared `xor eax,eax` epilogue (AI decide callbacks)**: nested ifs or `if (a && b && ...) {...}` + one final
  `return 0;`; guards copy the epilogue per site. Same for `return 1` (the original nests); `if (active) {..return
  1;} return 0;` shares the tail. A trailing `goto ok; ... ok: return 0x40;` merges success tails. (0x499660 /
  0x49a020, 0x499c10 718 -> 35 B)
- **A second guard with its own `test eax,eax; jne; pop; add esp; ret`**: `m = f(...); if (m == 0) return 0;`, loop
  after; nested `if (m != 0) {..} return 0;` merges. Bound `if (n >= 6)` (`cmp esi,6; jl`) (amt_Unk4e76b0 0x4e76b0).
- **Several `return 1` branches plus one shared `return 0`** (d3io).
- **Repeated cleanup per failure site, not `goto fail`**: VC6 tail-merges the copies, register choice follows source
  order; reorder a tail to keep two apart (`if (!x) return -1; return y;`). `if (parent) d3_SetParent(...); return
  1;` per site it decides which restore block survives and the register ranking. `tex_Release(snow_tex); snow_tex =
  0;` in each failure branch, then `if (!snow_tex) {..}` (goto: 80 lines worse); `#line` gives the D3DCHECK order
  (Lock 3749, GetPalette 3758, Unlock 3762, GetEntries 3768). (d3t_DropObject, d3t_CreateSnowMipmaps)
- **Every `return 0` merged into one `pop..; xor eax,eax; ret` block, the original keeps a copy per null test (no
  xor after `test eax,eax; jne`)**: one `return 0` too many in an if / else-if chain that ends in a test: `else
  return 0;` before `if (id == -1) return 0;` is `else id = -1;` (VC6 threads that arm into the id test's copy).
  (ai_ExecuteProduceGood 43 -> byte)
- **A later exit cross-jumps into the wrong one of two identical `x = 6; return` copies (short jcc against the
  original's near jcc to the first copy)**: no shared fall-through store at the end: write each `d->phase = 5;
  return;` at its own site (`if (s1 > s2) { d->phase = 5; return; }`, `if (sum2 > 17 || k == 4) { d->phase = 5;
  return; }`) instead of nesting the other cases and falling into one final `d->phase = 5;`. Also there: a
  local initialised at its declaration from a field (`int ph = d->phase;`) against `ph = d->phase;` after the
  declarations decides whether a sum is spilled to its own push-ecx slot. (ai_DuelNextPhase 58 -> byte)
- **Tails twice in the original**: written twice; a shared tail (LeaveCriticalSection) is duplicated into if/else
  branches, not with an early return (msx_Stop).
- **Fail block inline**: an `||` test split into two ifs, each with cleanup + goto fail.
- **Send sites the original does not share**: each `Begin; r = Send(&m); End; return r;` in full, not `goto send`;
  the per-site store order (`m.id` / `m.o1` first) decides which merge (amt_ApplyForOffice 51 -> 14).
- **`store; return 2` tails merged, the original keeps one per site**: `w = 2; x = kind[w]; return w` keeps them
  apart but reloads kind[2] (ai_PokerShowdown 230 -> 195, stuck).
- **`d3sm_Destroy(g); return 0;` x3 merged into the fail tail**: `ok = 0; if (a) { if (b) ok = 1; } if (!ok) { fail:
  ..; return 0; }` frees two of three; a second success return is the same code (rd_BuildRoad 97 -> 68).
- **`xor eax,eax` before a switch dispatch (`add edi,-0x12; xor eax,eax; cmp edi,0x25; ja ..`)**: `r = 0; switch (k)
  { case X: r = f(..); break; .. } return r;`. Returns into one return-0 block: `if (kind == 0) {..} else if (kind
  == 0x10) {..} else return 0;`. Threaded coin flip (`test n1; je; test n2; je side0; random; jne side1`): `if (n1
  != 0) { if (n2 != 0) side = RandomU(2); else side = 0; } else { if (n2 == 0) return 0; side = 1; }`; `n1 = 0`
  inside its arm takes the const-0 register. (ai_DecideLeisureAction 288 -> byte; ai_DecideVisitAction 115, open)
- **A switch returning 1 / default 0, immediate stores and `mov eax,1; ret` per case**: `break` in cases, `default:
  return 0;`, one `return 1` after; `return 1` per case gives `mov [mem],eax` (0x4f9710).
- **Order handlers 3 / 1 and 4 / 2**: `if (m[0] == 4 || m[0] == 2) { if ((flags & 0x100) == 0 || (t = *(char **)(m +
  0x87)) == 0) return 1; ...; return 0xa; } return 1;` (0x4f66b0, 0x4f67c0, the 8 job handlers).

### Where a return block is laid out

- **Return-0 inline at the test vs after the body**: `if (!cond) return 0;` + block inlines it; `if (cond) { body }
  return 0;` puts it after. hst.c text commands use the first (0x4b00e0: 96 -> 3 code hunks), ahm_LoadPrototypes
  (0x4eb480) the second.
- **`xor eax,eax` before the test**: `if (!c) return 0; for ...; return 0;`, not `if (c) for ...; return 0;`
  (evt_FindById).
- **A shared return block before the found body (0x4b6743 vs 0x4b6753), reached by guards and the loop's
  fall-through**: `goto none` on the guards, `goto found` on the `p != 0` checks; `for (i = 0, p = 0; ..)` fixes the
  `xor` order. No lever: nested `if (a && b ..) {..} return 0x400;`, `if (p == 0) return 0x400;`. (unk_4b6480.c
  0x4b6660 byte, amt.c 0x4e6fc0)
- **A shared exit reached from inside a loop**: `goto none` on every guard + one `return X` after the body
  (unk_4b6480.c 0x4ba0c0).
- **Guards and tail sharing one out-of-line `return 0` at the end**: `goto none; ... none: return 0;`; plain `return
  0;` inlines a copy per guard.
- **One return-1 block after the body (`je mismatch` / `jne mismatch`)**: `if (x != 0) { ...; return 0; } return
  1;`; not merged after `if (x == 0) return 1;`; a `goto mismatch` target goes before the body (0x58fba0, 26 -> 7).
- **`jne <shared epilogue>; jmp <continuation>`, ours `je`-over-epilogue**: `if (!found && obj->material) { ... }
  return y;` (d3s_GetGround).
- **A failure cleanup mid-function, the other failures jumping into it**: `if (alloc == 0) { fail: ...; fail2: ...;
  return 0; }` + `goto fail`; after the nested ifs it is placed last. Its clamp: `min(max(n, 0x12), 0x1e)`.
  (ahm_LoadIni 48 -> byte)
- **Failure block (`or ebp,-1` + epilogue) inline at the LAST failing test, success into the end epilogue (ours:
  after the success code, rel32, a `tail` hunk)**: no `goto end`; each step under `if (!ret) {..}`: `if (!e->data) {
  bank = f(e); if (bank) ret = Load(bank, e); else ret = -1; } if (!ret) { if (type == 1) .. else if (type == 2) ..
  else ret = -1; } .. if (!ret) { success } return ret;`. The call arm first (`if (!bank) ret = -1; else` leaves
  8-10 ins). No lever: any goto form, `!=`, else-chains (one code); switch, per-site `return ret` worse. (sb_Play 14
  -> byte)
- **A search loop whose found / not-found exits each have their own epilogue while ours shares the not-found
  `return 0` with the guards, and a call's `add esp` at the start of the success code left unmerged (scheduled
  into the following stores, the later calls' cleanup smaller)**: a found flag. `ok = 0; for (;;) { ..; if
  (found) { sprintf(m.buildPath, ..); ok = 1; break; } if (alt) break; alt = 1; } if (!ok) { error; return 0; }
  fill; return 1;` - the decided `if (!ok)` leaves the label that stops the merge and keeps the failure block
  last with its own epilogue. Every loop form without the flag (break, continue, fill after the loop, nested or
  `||` guards: one code), and a `goto retry` / `continue` form (own epilogues, failure block first) were not it.
  Then the fill order (5040 permutations, a few byte) (ai_ExecBuildBuilding 65 ins -> byte).
- **A loop's failure jumping to the FIRST of identical `f(); return 0;` blocks, the original the LAST (rel32 vs
  short `je`)**: VC6 merges into the first in source order; the exit test belongs in the `while`: `do { .. } while
  (main_QuitProgram == 0); main_Exit(); .. return 1;`. No lever: `!x` / `== 0` (31 subsets). (WinMain byte)
- **Inline error block, shared `or eax,-1` tail**: `if (n >= MAX) { err } else { ...; return obj; } return -1;`.
- **A first-placed `return -1`**: `if (x == 0) return -1;` before the body. `if (i < 256) return &a[i]; return 0;`
  and the inverse choose different fall-through returns.
- **`mov [eax+8],1` immediate on one path, `mov esi,1` after the call on the other, `test ecx,ecx` re-test**: `if (a
  == 0 && b < c) { ...; return; }` then `if (a != 0 && ...)`, the store in both inner arms (`{ play; x->playing = 1;
  } else x->playing = 1;`) (snd_UpdateQueue 47 -> byte).

### Which copy of a cross-jumped tail survives, the join position

- **Two `o1->arg = ..; o1->kind = 0x12; return ai_DecideTakeCredit(..)` tails, the original one call (`jmp` into the
  first copy's middle, the second's difference in ecx: `mov ecx,edi / sub ecx,eax`), ours two full copies with
  the stores after the pushes**: the difference was written out (`(float)(cost - lim) / cost`, `o1->arg = cost -
  lim`), no `need` local: a CSE temp, kept apart from cost, the stores move before the pushes and VC6
  cross-jumps the tails. A goto to a shared label was far worse (100 ins). (ai_DecideBuildDomizil, byte; same
  pattern open in ai_DecideBuildSculpture with `d`)
- **The original keeps the last arm's tail (ours the first)**: chain arms without `return;`, one `return;` after;
  nesting is no lever (sv_RefreshPanel 78 -> byte).
- **Pops before the final `if` tail (ours `mov al,[flags]; pop edi; pop esi; test al,8`)**: VC6 sinks restores
  unless the tail is on two paths; one branch gets its own tail copy + `return;` (toy-proven, deep-tools toyasm.py).
  Loader: `d3if_SelectScene(save, 0)` in an `if (load == 0)` arm per branch, early exits with their own clock tail +
  `return;`. (init_LoadObjectScene byte, init_LoadBuildingScene 123 -> byte)
- **Which exit owns the final epilogue: sweep the join position** (trailing statements shared: 0, 1, 2, ..). A
  far-by-count variant with the original's layout and only a flag store / reload at the join is one move from byte.
  Lever: the read success falling out of the nested ifs to `if (!f) sprintf(not found)` instead of `goto done`
  (44 -> 6), then only `vfs_OpenCount++` behind both arms as `if (f) vfs_OpenCount++;` (byte), `if (f) {
  strcpy(f->name, name); .. }` for `goto opened`; `binary = 1;` before `read = 1;`. No lever: read / write
  success `return f`, the write branch first, `goto end`. (vfs_Tfopen byte)
- **One -1 in several tails, rematerialized (`or ebp,-1`), one store an immediate**: one shared `W = -1;` after arms
  without returns (`if (..) { .. if (c) {if (!fb) T = f();} else T = -1; } else { A = -1; T = -1; } W = -1;`). No
  lever: returns per path, store orders, `a = b = -1`. A ?: argument pushed per arm is two calls.
  (sv_OpenBuildingPanel 17 -> byte)
- **Two arms sharing one `push edi; call _strcmpi` tail**: `if (_strcmpi(name, LIT) == 0) goto fill;` per arm into
  one `slot = -1;`; VC6 keeps the later arm's copy, the original the first (ai_RegisterSaveSlot, stuck, 14 ins).
- **A call merged between two branches**: write it at the end of each (the first copy survives). `else if (A && B)`
  and nested ifs choose different duplicated tails.
- **A threaded return copy, the original `jmp`s to the shared end check**: `gm_BQFirstCall = 0` after the first-call
  walk (gm_BuildNext, open).

### Search loops and their exits

- **`cmp i,N; jl <copy>` and a second `xor eax,eax; ret`**: `for (..) if (t[i].k == k) break; if (i >= N) return 0;
  *out = t[i]; return 1;`; in-loop exits merge the returns. `xor eax,eax` after the pops of a `goto add` loop: an
  always-true `if (i >= N) return 0;`. (amt_GetFirstOffice, amt_GetOffice, veg_SetPlant byte)
- **A free-slot walk `p += 0xf1; if ((int)p >= (int)&T[N].f) return -1;`**: `j = -1; for (..) if (!T[i].active) { j
  = i; break; } if (j == -1) return j;`. `or eax, ebp` / `or eax, eax` in an epilogue copies a known -1. No lever:
  `__inline`, `win = &T[j]`, name splits. (D2_CreateWindow 119 -> 46, stuck)
- **Two pointers merging at a search exit (`lea eax,&a[i]; test eax,eax; jne` / `mov eax,[pick]; test eax,eax`)**:
  `c = 0;` at the top of the arm, every break sets `c`, `if (c == 0) c = pick;` (at the declaration c gets a
  callee-saved register). All failures early `return 0` lets the mode-1 success skip the `mode == 0` store. (six
  ai_Token* siblings byte, ai_TokenRichestInhabitant)
- **`dec; test; mov; jne` bottom test**: `while (n != 0) { ..; n--; i = ..; }`; `for (..; n != 0; n--)` drops the
  test.
- **A capped count (`cmp eax, ecx / jl / mov eax, ecx`)**: `if (cnt >= n) return n; return cnt;`
  (amt_GetOfficeCandidates / ...Full).
- **A `cmp ptr, end` pointer loop over a static table**: `q = tab; do { if (*q != -1) ...; q++; } while ((int)q <
  end);` (0x522100, 0x5223c0).
- **`xor ecx,ecx` after the head load, `mov eax,ecx` on the empty list**: `found` NULL at entry, `found = m; return
  found;` after the loop. `if (m != NULL) { do {..break..} while (m != NULL); return m; } return NULL;` lacks the
  hoisted zero (mov_Find, open).
- **`xor eax,eax` at entry, the early exit joining behind `mov eax,esi`**: failures `f = 0` (`if (init fails) {
  pool_Free(..); f = 0; }`), the tail under `if (f) {..}`, one `return f`; an `if (p)` that costs no instruction is
  invisible and must be tried (vfs_MemOpen 2 -> byte).
- **`bt = GM_BTYPE(..)` folded into the first load**: assign `n = 0` before it (gm_ListObjectsAfter byte).
- **A shared `cnt = 0;` zeroed after the hoisted pushes**: one `cnt = 0;` per arm (Gm_FreeCapacity).

### Return type and small return forms

- **The shared `return 0` epilogue starts with `xor eax,eax` and one `je` after a call jumps past it (ours: the
  xor scheduled among the pops, every exit through it)**: that exit returned the call's result, `r = f(..); if
  (r != 0) { *o1 = s2; *o2 = s1; } return r;`, not `if (r == 0) return 0;` - written out at both call sites, no
  `goto` to a shared copy. A `p` reload into another variable's register on one branch (`mov ebp,[esp+0x84]`)
  was no `w = p` there: drop the dead assignment, VC6 reloads the split p itself (ai_DecideUseDynastyObject).
- **No `xor eax, eax` before the ret**: `void` (0x5da010; 0x5d9a80 / 0x5d9b20 / 0x5d9b90 void too). A void tail
  call: `jmp vfs_fclose` (0x5d7320), failure a bare `pop esi; add esp,0x7c; ret` (0x5d7430).
- **`mov al,1` / `mov al,dl`**: `char` return.
- **`test / je / mov eax,1`, ours `neg / sbb / neg`**: `if (f(x)) return 1; break; ... return 0;`.
- `x <= 1` reuses a register holding 1. `return !(a && b);` returns full eax.
- **A pointer builder (`test eax,eax; jne; ret`)**: `if (!a) return 0; ...; return a;`.
- **`setne; lea [eax+eax+1]`**: `mode = (strcmp(..) != 0) * 2 + 1;` = if/else (WinMain).
- **`add [mem],reg`**: another access form than the loads (index vs byte-offset macro) (d2_Unk472760).

### When an exit hunk resists

- **The lever is a statement elsewhere**: run twin.py, count returns, try the assignment inside the expression, `p =
  0` + `if (p)` for every `return 0` after a cleanup. A reload `mov reg,[slot]` before a "must be 0" return is a
  decided `if (!p)`. A low count with the wrong layout is a local optimum.
- **Probes**: exit-form products in one `dcp try`; 2^n site subsets with `dcp try`; flip.py constant mutants; c2pass
  `--skip 0x1070a91d`. `return 2` at earlier copies moves a merge target to the next equal one. No exit-form
  generator yet (`tools/sweep/exits.py` proposed).
- **Tools**: tools/c2dag/vdiff.py; `dcp try` rows are unsorted and `best:` says "none closer" for fewer ins at a
  lower grade: read the `ins` column. For __stdcall pass `WinMain@16` to c2pass; flip.py then cannot find the source
  line (a copy with `(@\d+)?` in its PROC/ENDP regex works).

## Branch layout, conditions, switches
Which arm is inline, jumps into another arm, threaded and redundant tests, `&&` / `||` forms, compare operand order and jcc, `setne` against a branch, ternaries, switch dispatch and case order.

### How VC6 orders blocks and merges tails (c2.dll)

- **The shared return 0 in the middle, the rate / success code last (ours the reverse, with `goto rate`)**:
  guard returns all the way, `if (b != 1 || ..) return 0;`, each failing step `if (..) return 0;`, the kind chain
  ending `else return 0;`, the success code after it: no goto (ai_DecideProduceGood 34 ins -> byte).
- **A void function whose skips jump to a `ret` in the middle, a later block ending in its own `ret`** (only
  jump targets differ, `0 ins / 2 bytes`): the work after the if/else was a test at the join, decided on most
  paths: `if (show) { if (h == -1) h = Open(); } else if (h != -1) { Close(h); h = -1; } if (h != -1) {
  print }`. No return / goto / nesting / flag / loop / swapped-arm spelling of `if (..) hide else { open; if (h
  == -1) return; print }` moved it (sv_UpdateQuickChat byte). A text in `<<< === >>>` with `"a\\b"` may not
  apply / loses the backslash: give such a function whole from a generator. Same in d3_LoadGroup: `if (first &&
  parent) AddChild` moved behind the version if/else (decided false on the bad-version path) put the exit
  behind it, the bad-version `fclose` out of line.
  And ai_MissionCompleteSingle (3 send-and-wait arms, exit behind the cancel arm): `if (r == 0) {..} else if (r ==
  0xff) {..} else {..} if (r == 0xff) ai_QuitGame(id);`, the call that only one arm needs moved behind the
  join. Read the jump target of each loop guard / skip: the arm that falls into the exit owns the join test.
- **A run of `if (write(..) <= 0) return 0;` steps whose loop failures jump to the FIRST step's epilogue copy**
  (ours: to the last step's), `0 ins / N bytes`: the function ended `LS_W(&x, 4) return 1;`, not `return
  write(&x, ..) > 0;`. Same `sbb / neg` code, but the last step's return is then a test whose `return 0` copy
  changes the pair merger's list (ls_WriteMessageQueues byte).
- **Each arm of a switch case ending in its own `return 0`** where the original's skip tests jump to another
  case's `return 0` block: one more `return` per arm (main_WindowProc WM_ACTIVATEAPP byte).

Read from C2 13.00.9044 and checked on toys with `tools/c2dag/c2tail.py` (merges, tries, `--il` dumps) and
`c2pass.py --skip` (`c2pass.py --list` names the sites). A `return` is `eax = ..; jmp Lexit`; the epilogue is
copied back into short predecessors only after all of this.
- **Block order (before allocation, FUN_1073cc0d; skip 0x10703443 = C1's order)**: the blocks are relinked in
  reverse postorder of a depth-first walk from the entry (FUN_10702fb0) that follows each test's JUMP edge
  before its fall-through; the exit block last. C1 emits `if (c) goto L;` as `jncc skip; jmp L` and `if (c)
  {..}` as `jncc after`, so the walk first follows the code after a goto test, after an if body, after a loop
  (its entry test jumps there), and into the then-part of an `||` (`jcc then`). The return block it finishes
  first is laid out directly in front of the exit label and falls into it; a block first reached late lands
  right behind its test. Text order, label positions, `goto` against nesting and `!`/`!=` spellings of the
  same flow graph give the same code; only the graph and which edge is the jump count. Example: `if (b) goto
  fail; if (a != 0x48 && a != 0) goto fail; .. if (r == 7 && m < 3) goto fail; o2->arg2 = 0x48; return 0x48;
  fail: return 0;` puts the default last (`jge default; fail: xor ..; default:`); `if (b || (a != 0x48 && a
  != 0)) goto fail;` reaches fail through the `||` jump first, so the default stays in front of fail and the
  test reads `jne default; jl fail` (ai_DecideUseAlchemyItems byte).
- **The mergers (after allocation, before scheduling)**: the flow pass FUN_1070a5fe, up to 17 rounds, called
  with merging on from FUN_1070ec81; every merge ends in FUN_1071355b(deleted copy, kept copy).
  - `xjump` (FUN_10712437, skip 0x1070a91d): each `jmp L` against the code that falls into L, up to 30
    instructions back per side, a conditional jump ends the region (labels and jmps do not). Matched as a
    dependency graph: an instruction that can be last on both sides and is equal is sunk; the copy before the
    jmp is deleted, the jmp goes to a new label in front of the kept copy. No size limit. Each round can take
    one more step back (an equal jcc included), so a tail merged into the block before the epilogue grows.
    Hence: whichever return block falls into Lexit absorbs the equal endings of every other return (the
    alchemy default took the `o2->arg2 = 0x48; return 0x48;` of two arms and was then laid out behind the
    first).
  - `pair` (FUN_10711fe6 -> FUN_1074633c, skip 0x1070a6da): two `jmp L` of one label, linear match back from
    both jmps (same opcode, flags, operands; a jcc equal when its target is). Deleted: the jmp later in L's
    reference list (newest first: usually the earlier jmp in the source; a retargeted jmp moves to the front),
    unless that copy is entered by fall-through and the other is a whole block behind a jmp / ret. Both
    entered by fall-through: merged only when the equal run measures more than 22 (FUN_10710628: a 7-ins mov /
    call tail 23, a 20-ins x87 / call tail 20 -> never merged).
  - `move` (FUN_107120b9 -> FUN_107711fb, skip 0x1070a6e9): the same for a label nothing falls into (behind a
    jmp / ret), no size limit; the first reference's copy is deleted and the kept copy moved in front of L.
- **Reading a tail hunk**: `c2tail.py SRC FUNC --tries` lists what was compared and merged with source lines;
  `--il merge` shows which block falls into the exit label; `C2DAG_TRYDUMP=1` prints a pair's equal count and
  size. A tail merged in ours and not in the original (or the reverse) is almost always another block in
  front of the exit label: find the flow-graph change that lets the walk finish another return first.

### Which arm is inline and which is out of line

- **An out-of-line block (after the main `ret`)**: the `else` of an `if` whose test was written the other way
  round: swap the branches (`if (s->snd) {...} else {create}`, `if (!a || !b) {fail} else {ok}`). A fall-through
  body handling the flipped case: test `x & 1`, not `!(x & 1)`. (458ce0, 458650, d3snd_UpdateVolume,
  d3sndw_GetSound, evt_* functions)
- **`je` to an out-of-line block**: the other arm was written first (0x5d7430, network arm first).
  `if (dstLocked != 0) d2_LockSurface(dst); else d2_UnlockSurface(dst);` (0x47942a); `if (*b != 10) { body } else
  { zero }` for `cmp al,0xa; je zero-block` (0x50d380 byte); `if (v >= 0x10056) { read } else { store }` puts the
  read inline (0x51db00); `if (b != 0) return (float)a / b; return 0.0f;` for `je` to the zero return (0x4f97c0).
  `if (on) Acquire else Unacquire` picks the fall-through.
- **The else arm inline whatever the spelling** (`if (A != 2 || (p = ..) == 0) {X} else {Y}`, &&, nested, early
  returns: one code): p assigned on both paths first, `if (A == 2) p = ..; else p = 0; if (p == 0) {X} ..`
  (init_TownBuildingHook byte).
- **The else block out of line whatever the spelling** (`if (!A || !B || !(d < c)) far else near`, reversed
  operands, `>=`: one code): a flag, `lowPoly = 1; if (A && B) { ..; if (d < c) lowPoly = 0; } if (lowPoly) far
  else near` (ch_UpdateLowPoly byte).
- **An else block placed after a later switch dispatch** (between `jmp [table]` and the first case): `if (v == 0xb
  || v == 0xc) plain = 0; if (plain) k = R(); else { k = R(); if (k == 3) k = 4; }`. No lever: `k = R(); if
  (!plain && k == 3)`. An `id = -1;` scheduled before a block-scope `char buf[N] = ""` fill's last stosb was
  written before the block's `{` (sv_SpeakPicked byte).
- **An inline block the original puts out of line** (`cmp [rup],ebp; jne far`, the store cross-jumped with another
  `sv_PopRequest = 1`): `if (dm_L.rup == 0) {..} else sv_PopRequest = 1;`; the rest is a priority race (c2trace:
  rec 124 above out 94) (ai_GraveWindow, stuck).
- **Block order**: `if (A) {T; redo = 1;} if (redo) {...} else {...}` kept it, else-if with goto did not. `if (r1
  && r2) return 1; if (!r1 && !r2) {...} else if (!r1) {...} else {...}` (d3t_GetQuadRange). `if (c)
  D3DCHECK(a); else D3DCHECK(b);` needs braces. An if/else chain without `return` hoists a shared `push`; `if
  (!f()) { if (g()) .. } else ..` puts the success block after the second call.
- **Layout source order does not move**: a six-term `||` chain = if/else = if/continue = negated (ai, f5
  0x487410). Siblings with the same text can differ: `if (f(1)) {...} else if (f(0)) {...}` 10 bytes better in
  0x47edb0, 10 worse in 0x47f700: measure the sibling.

### Redundant tests the original keeps, threading

- **A flags word loaded once (`mov cx,[edx+0x5a]`) and tested again in each later arm**: the first test on the
  field, the later ones on a copy, `fl = rec->flags5aw; if (rec->flags5aw & 0x45) continue; .. if (!(fl & 0x45))`;
  all on the field or all on fl threads them away (ai_DecideMayor byte).
- **A test directly followed by the same test, the first reached from one path only** (`test eax,eax; je` before
  the join's `if (o)`): that path assigns through an if/else, `if (sv_PanelShown != 0) o = sv_PanelShown; else o
  = 0;`. No lever: `o = g;`, `?:` (ai_SetBookmark byte).
- **A pointer tested twice on one path** (`cmp eax,ebx; je ret` twice): the first test read the source global,
  `if (ai_FallbackBuilding == 0) return; b = (AIBLD *)ai_FallbackBuilding;`; an arm laid out after the else arm
  was written second (`if (ai_PlayerBuilding == 0) {..} else {..}`). The same `b == 0` twice is always merged
  (sv_ResetSelection byte).
- **`if (p)` and `while (p)` tests both kept** (`test eax,eax; je end; mov esi,eax; test esi,esi; je end`): `if
  (obj->parent) { p = obj->parent; while (p) ... }`, not `p = obj->parent; if (p) {...}` (d3s_GetGround 0x45bcd0).
  A value tested twice in a row is a re-read field (`if (CH_SLOT(ch) && ...)`); a local drops the second test.
- **A redundant re-test** (`test [m],0x20; jne call; cmp eax,ebx; jne skip; call:`, third term of `!s || s->f ||
  !s`): `if (s && s->f) { call } else if (!s) { call }` (ch_ActWalk byte); with a long arm a flag `r = 0; if (s &&
  s->f) r = 1; else if (!s) r = 1; if (r) {arm}` or a goto into the arm (ch_ActWalkCells byte).
- **Later tests re-evaluated unthreaded** (`if (s && !x) {..return;} if (!s && x) ..; if (s && x) ..`): the later
  tests on the field (`ch->slot`), not the local s (ch_UpdateLowPoly 91 -> 78 ins).
- **s compared again after a float test**: `if (s == x && !(g < y)) continue; if (s == x && (f & 1)) {..} else
  {calc}`, not a nested `if (g >= y) continue; if (s != x || ..) goto calc;` (path_Expand 222 -> 64 ins).
- **An else-if chain re-testing a byte (`test cl,cl; je`)**: flat, `if (a && a->h > 0 && b && b->h > 0) .. else if
  (a && a->h != 0) ..`. Wait loops `while (r != -1 && cm_GetSlotState(r) == 0) sv_PumpNet();` stay unrotated
  (amt_CheckOfficeColumns byte).
- **A null test of an element address kept** (`lea eax,[edx-4]; test eax,eax; je`): VC6 folds `s != 0` when the
  other operands go through s; it keeps it with `s = &T[i]; if (T[i].obj != -1 && s != 0 && .. T[i].obj ..)`, plus
  an @original comment (sv_HandleRightClick byte).
- **One mask test but the 0x200 key still stored** (`test ch,0x3c / jne / and ecx,0x200 / neg / sbb / and
  ecx,0x23 / mov [dead slot],ecx`): the whole five-way key chain for the second character too (amt_Rating).
- **A redundant-looking compare**: read the polarity from the bytes. `cmp 6 / je body / cmp 7 / je skip` is `kind
  == 6 || kind != 7` (0x5a8d80). Three float compares (`>= 80`, `>= 30`, `< 30`) need `else if (x < 30.0f)`.
- **Threading**: `if (!x && c) f(0,0); else if (c) f(v,1);` tests x once (`else if (x && c)` keeps a test); `sel =
  cur` lets VC6 skip a compare when the loop is skipped: reload instead.
- **A dead `cmp eax,0x71b; jne <next instruction>`**: source unknown. No lever: `if (x == C) ;`, one-case switch,
  `if (x == C) goto next; next:`, `while (x == C) break;` (txt_OpenText 0x42d2d6, open).

### Jumps into another arm, goto chains, shared tails

- **A jump to a shared tail's second instruction, its first (`mov eax,[esp+o1]`) hoisted above the jcc**: the
  arm wrote the tail out itself (`o1->kind = 0; o1->ref = -1; o1->arg = 'noop'; return 0x35;`), no `goto noop`;
  cross-jumping merges all but the load, which the jcc's block already did (ai_DecideStammtisch byte).
- **A jump into a sibling branch just behind a test of the same variable** (`cmp m,w; je <neutral block of the men
  branch>`): a threaded re-test; route the path through that test, `if (men < women && (w != 0xffff || m !=
  0xffff)) { if (w == 0xffff) return; ..women.. return; } if (m == 0xffff) { m = -5; f = 1; } ..men..`. No lever:
  fallback values in the women branch, `!(a && b)`, inverted if/else (snd_PlayFavourComment byte).
- **`mov edi, -5; push edi`, the bank global reloaded per branch**: `m = -5; f = 1;` falling into the shared `if (f
  == 0) .. else if (f == 1)` chain, not a literal `snd_PlayVoice(-5, ...)` (snd_PlayFavourComment 41 -> 10).
- **Jumps into another arm's test chain**: a short-circuit condition then a priority chain, `if ((res == 0 &&
  !has800) || (res == 1 && !has1024) || (res == 2 && !has1152)) { if (has800) res = 0; else if (has1024) res = 1;
  else if (has1152) res = 2; <set mode size> }`, not labels; read the push order against the RECT field offsets
  (main_OpenScreen).
- **`goto clamp` exits**: one else-if chain, `if (ldown && (y < 0x50 || y > H - 0x50)) state = 0; else if (ldown
  && state == 0) {..} else if (!ldown && state == 1) state = 0; else if (state == 1) {drag} else {keyboard
  part}`; then `dx = ..; dy = ..; dragX = ..; dragY = ..;` fixed the register rotation (ai_CannonAim byte).
- **A `goto` past a duplicated `player++`**: one plain increment after a plain if (91 -> 69); two loops over a
  clock at different slots were two locals (`AITIME night`) (main_GameLoop).
- **A `goto no` chain into `else { no: m = 0; }`**: an `ok` flag (`if (ok && ..)`, `if (!ok) m = 0;`) gives the
  same code and the cross-jumped tail (gm_QueryNext).
- **`goto fin` exits past the object tests**: each `if (obj == 0) goto fin;` as an `obj != 0 &&` condition; all
  four sites (single sites worse). No lever: do-while scans as for loops (gm_PermissionBits, stuck, 42 -> 38).
- **A goto-merged tail (`goto lA / lB`) hiding a one-path store**: the arms in full (`else { x = 0; ...; }`)
  (di_UpdateButtons).
- **Both range compares kept, else out of line** (`cmp al,0x1b / ja far`, then `cmp al,0x1c / jb / cmp al,0x22 /
  ja`): `if (k <= 0x1b) {..} else if (k >= 0x1c && k <= 0x22) {..} else {..}`, not nested with `goto done`
  (amt_ExecuteOfficeMessage 133 -> 15).
- **Call operands loaded above the branch** (`mov edi,[npix]; cmp ..; mov eax,[size]; push str; je`): both arms
  need the same registers, `n = s->size - s->npix; r = mem_Alloc(n, ..);` in each. No lever: size / npix locals
  before the if (d2_ShpConvert24To16 byte).
- **One shared test tail**: the ternary `special ? f(...) : g(...)` in one `if`, not two `if (call(...) && out[1]
  == 0) x = 1;` (0x4c2f30).
- **evt_Depth reloaded after every call**: `if (x) f(); if (!x)`, not `if (!x || (f(), !x))`; the pop twice (no
  `goto pop`) for `or edi,-1`, early `cur = stack[d - 1]; d--; stack[d] = 0;`, late `d--; cur = stack[d];
  stack[d] = 0;` (evt_Step byte).
- **Small shapes**: `for (...) {...} if (!replaced) sprintf(...)`; `if (!a || x) f();` with duplicated setup is
  `if (a) { if (x) f(); } else f();`; `box = 0; if (a && b) box = f(); if (!box)` differs from `if (a && b && (box
  = f()))`; a `char c` copied as `*(int *)&c` stops two branches merging.

### setcc, defaults, conditional assignments

- **`setne` against a branchy 0/1 (`xor r,r` / `mov r,edi`)**: `(x & A) || (x & B) ? 1 : 0`. No lever: `x & (A|B)
  ? 1 : 0`, `!= 0` (d3d_GetDeviceInfo 91 -> 25 lines).
- **`setne cl; lea edx,[ecx*4+4]` against `lea ecx,[ecx*4+4]`**: `x != 1 ? 8 : 4` or if / else, not `(x != 1) * 4
  + 4`. Against `jne; push 0; jmp; push 1`: `if (g == 0) f(d, 0); else f(d, 1);`, not `d3if_ShowTree(d, g != 0)`.
- **Test, then the default** (`test; mov eax,[a]; je; mov eax,[b]`): `if (!c) u = a; else u = b;`. No lever: `u =
  a; if (c) u = b;`, `if (c) u = b; else u = a;`, `c ? b : a` (lobby_ShowPlayers byte).
- **A default in eax across the flag test** (`mov al,[flag]; test al,al; mov eax,1; jne`): `if (flag) mode = 1;
  else mode = dflt;` or ternary, not `mode = 1; if (!flag)` (ai_OptionsGfxScreen byte). `if (w->font != 0) font =
  w->font; else font = def;` matched, swapped branches not.
- **One sunk store** (`cmp; jge; mov eax,ecx; mov [m],eax`): `m = n < t ? n : t;`, not `m = t; if (n < t) m = n;`
  (0x4eafd0, 0x4eb0b0 byte).
- **Other forms**: a conditional to a fresh local gives `mov eax,2; sub eax,ecx; jmp; mov eax,ecx`; `if (!t->a) s
  = m->lod ? m->lod : 1; else s = t->a;` a threaded `mov ebp,1`; `if (p = obj->parent) {...} else p = 0;` a `jmp;
  xor reg,reg` (d3t_DropObject); `r != -2 ? 0 : -3` is `sub / neg / sbb / and / add`; a side-effect string
  argument chosen by if/else is a ternary with a comma expression.

### Compare operand order and the constant written

- **Operands swapped**: write the condition from the other side first. `if (best < s) { best = s; pick = cp; }`
  `cmp ebx,eax; jge`; `if (s > best) { pick = cp; best = s; }` `cmp eax,ebx; jle` (0x4e8c20 byte, 0x4e88a0).
  `r->slots[i].id == m->id` `cmp [ecx],edx`, reversed `cmp edx,[ecx]` (0x4ecd10 byte); `i >= (unsigned char)b`
  `cmp esi,edx; jl` (0x58f980); `if (index >= count)` `cmp ebx,eax / jl`; `x > W`, not `W < x` (d3t_UpdateCamera).
- **Equivalent spellings differ**: `a >= b` / `b <= a`, `!(v < m)` / `v >= m`; one `dcp try` list finds it
  (0x4836a0: 125 -> 0 bytes). `(unsigned char)i > bank->n` gives `xor dx,dx; mov dl`. Float-product `!(A > B)`
  and `A <= B` spill different sides.
- **The literal is kept**: `n > 3` `cmp 3; jg` / `n >= 4` `cmp 4; jge`; `i > 0xff` / `i >= 0x100`; `k >= 10 && k
  <= 14` `cmp 0xa / jl` + `cmp 0xe / jg`, `9 < k && k < 0xf` the other pair; `cmp x,1; setle` is `x <= 1` (cm.c
  0x4c67c0); `0x75 <= x` `cmp x,0x75; setge` (0x5a8b80, 2 B).
- **Version constants**: `cmp v,0x10033; jl` is `v >= 0x10033`, not Ghidra's `v > 0x10032` (0x51c950, 0x51cee0;
  `0x1005a` / `0x1005b`); `x >= 0x10028` `cmp x,0x10028 / jl`; `(v >= 0x10046)` `setge`.
- **Ranges**: `cmp lo+1; jl` + `cmp hi-1; jg` is `n >= lo + 1 && n <= hi - 1`, not `lo < n && n < hi`
  (gm_TitlesLeft). `(unsigned)m[1] >= 1 && (unsigned)m[1] <= 7` `cmp eax,1; jb` + `cmp eax,7; ja`; `m[1] != 0 &&
  (unsigned)m[1] < 8` `test eax,eax; je` + `cmp eax,8; jae`.
- **`cmp reg,0; jg body`**: `<= 0`, not `n < 1` (amt_GetOfficeCandidates 0x4e7150, 0x4e7320). `p->x <= 0` gives
  `cmp 1; jge`, `< 1` does not.
- **Symbol bounds**: `(int)s < (int)&count` keeps `cmp reg,imm` (`s < arr + 3` worse); a signed pointer scan on a
  field (`cmp [ecx],0`) is an `int *` with `q -= 19` and `(int)q < (int)&a[0].f`.
- **The compared value's register wrong**: retry `> 15` / `>= 16` after each structural fix (di_UpdateButtons).
  `k << 16 | j` and `k * 0x10000 | j` allocate differently function-wide.

### Unsigned, signed and float jcc

- **Unsigned byte `jbe` against `je`**: `test cl,cl; jbe` is `x <= 0` / `x > 0`, `je`/`jne` is `== 0` / `!= 0` /
  `x`; signed `char` `jle`. Per function, even in a macro (G12_TITLED's `officeKind > 0`): check the branch, not
  the type (0x4e81e6, 0x4e820f, d3fl_UpdateTiles, g3 0x4937a0). `test dl,dl` + `ja` is `if (x <= 0)` (he.c
  0x53ec40). Unsigned int `0 < u` `cmp/jbe`, `u != 0` `test/je` (0x47c0e0).
- **`cmp BYTE PTR [x],1; jae`**: `x < 1`; `!x`, `== 0`, `<= 0` give `test`. `d3_Busy <= 0` gives `cmp
  [mem],ebx; jg`. `!(c & 0x80)` is `test; js`, `(signed char)c < 0` `jl`.
- **Float flags**: `<` `test ah,5; jp`, `>=` `and eax,0x100; jne`; `if (A > B)` `and eax,0x4100; jne`, `<=` `test
  ah,0x41; jp`; `best < x` / `x > best` picks the fld / fcomp order (hmt_DecideElection).
- **`fild / fcomp [mem]`**: `(float)(unsigned short)age < *(float *)(c + 0x20)`; `(float)*(int *)` gives `fild /
  fcompp`. A dropped float return costs an `fstp`: declare the callee float.

### min / max and clamps

- **Operand order shows in the jcc**: `max(1, n)` / `if (n < 1) n = 1` `jge`, `max(n, 1)` `jg`; `max(5, v / 4)`
  jge / `max(v / 4, 5)` jg; `min(0xff, iv)` `jle` / `min(iv, 0xff)` `jl`; `max(1, v)` jae / `max(v, 1)` jbe;
  `max(min(d.max, n), d.min)` `cmp eax,esi; jl` (0x4b09f0 byte). Try both orders per statement.
- **Against zero**: `max(0, y - n)` `sub; sets; dec; and`, `max(y - n, 0)` `test; setle`; `max(0, x0)` `setl`.
- **`mov byte [g],2; jb L; mov [g],al; L: mov eax,[g]; and eax,0xff`**: `g = max(2, g);` (or `g < 2 ? 2 : g`),
  read as `ai_NumPlayersW & 0xff`. No lever: `max(g, 2)`, `if` (main_PlayerCountDialog byte).
- **Macros on globals** give compare-then-conditional-store; `abs(0x7f) / 2` is not folded.

### Switch dispatch, case order, empty cases

- **Empty case labels missing from the tree / table**: a folded `if (0) { }` body keeps the label and emits no
  code; `case 7: if (0) {} break;` (or `val = val;`) keeps a table entry. No lever: `case X: break;`, `return;`,
  `(void)0;`, dead stores, `goto` to the join. `dcp try` calls table changes "= control": apply and build.
  (d3d_Error 0x418e90 byte; evt_Eval)
- **Case body order = source order**: jump-table targets give the case order (`default` first if so); it also
  moves allocation and which shared-tail copy survives. A permuted table passes the byte check but `dcp link`
  reports "local reloc .. expected" (0x50f0b0 byte, 0x4cc7b0, dd_Exit, dd_SetMode).
- **`sub eax,0x1e; je; dec eax; je; dec eax; jne`**: a `switch`, not an if/else-if chain (0x50f0b0: 210 -> 75).
  `dec eax; je case1; dec eax; jne common`: a two-case switch, kind==2 arm first (0x50d380: 111 -> 8).
- **`sub eax,0; je; dec eax; jne`, bodies merged**: `case 0: A; break; case 1: A; break;` (`case 0: case 1:` is a
  range test); `case 0: return 0;` keeps `sub ecx,0`. A dense 0..3 jump table: a compare chain there is if/else.
- **`push str; jmp tail` per case to one sprintf**: the after-switch statement in every case. No lever: case
  order, `&name[0]`, `char *s` (ai_CheckVisitAction byte). Different call constants: the call and `return 1` per
  case (g3, 0x4937a0).
- **A one-instruction `jmp X` function**: `return f(args);`.
- **`cmp eax,16; je; cmp eax,24; je; cmp eax,32; jne` with ours in the wrong registers, a stub `mov ecx,8; jmp`
  at the end and a branch hunk**: `switch (bpp) { case 24: case 32: ..; break; case 16: ..; break; }`, not
  `if (bpp != 16) { if (bpp == 24 || bpp == 32) .. } else ..` (if / else-if forms 84..88 ins; case orders one
  code). No lever: chained or split Loss/Bits stores in the cases (dd_SetMode 73 -> 68, open).

### Caught only by dcp link (dcp build masks these)

- **`and edi,0x80`, separate calls**: `if (d & 0x80) {..} if ((d & 0x80) == 0) {..}` without local or else.
- **Shared `p == s` tail**: `if (name) { two arms } else if (p == s) found = s;`. `if (!mem_IsValid(*h))` needs
  `em = *h`.

### Functions brought to byte by several changes

- **d3anim_PoseBones**: `if (n == 0) { rest pose } else { final loop }` after the active block (310 -> 168); v and
  rot in different blocks to share a slot; its own `D3ANIMMESH *skel = ..->mesh;` before the strcpy;
  `D3_MESH(c)->boneName` per use (130 -> 30); `float s; D3VECTOR rot;`; `map[b] < 0xff`; `fld [s->weight]; fmul
  [v.x]` from `(s->weight) * v.x`. Rule: an fmul / fadd order nothing moves: parenthesise one operand. No lever:
  operand order, casts, temps (dcp headers sample: insensitive; flip.py never adds parentheses).
- **d2_ShpBlit16Rows**: clip tails as if/else with `vis -= skipl; clipped = 1;` per arm, unclipped first (332 ->
  146); no `w` local; no `mode` local (`test edx,edx; mov dl,[s+0xd]; jne`): `s->mode == 0 / 2 / 3` in both
  chains (159 -> 8); zero inits first, `d += ..` before `p += skipr`.
- **d2_RedrawView**: `if (!tex1[0] && !tex2[0]) return; if (tex1[0]) {..}` with NO `t = 0` (84 -> 62); `if
  (o->captionX != -1) { ex = ..; tx = ..; } else {..}` assigning both per arm.
- **d2_ShpConvert24To16**: a 3-byte walk read as [esi-1] / [esi] / [esi+1] is `pix[k * 3], pix[k * 3 + 1],
  pix[k * 3 + 2]` in a for loop (see d2_Convert16ToRgb), not a do-while with `pix += 3`; `pix = base; pix[(w * i +
  j) * 3 + 0..2]` gives `lea ecx,[ebp+eax*2+0x32]; add eax,ecx`.
- **di_UpdateButtons**: `mov [g],0` means `mhold = 1` came first; `t = now; px = (volatile)x; py = (volatile)y;`.

## Loops and induction variables
Rotation, test position, countdown, induction variables and strength reduction, pointer walk against index loop, bounds, search loops.

### Decompiled pointer walks that were index loops

- **A decompiled pointer walk (`(int)p < (int)END`, `p[-10]` / `*p` against `T[0] + 0x228`, `p += 0x24d`, `w++`,
  a hand-walked slots pointer)**: write the plain index loop `for (i = 0; i < n; i++) .. T[i].f`; VC6 makes the
  IVs, the countdown and the pointer in a dead parameter slot itself. Rebase the fields on the IV: `T[0] + 0x228`
  with a record of 0x21c is record 1 + 0xc, so the loop starts at i = 1 and reads the byte +0xc (the faith), not
  +0x228. Still check line 211 first on f3init-style drafts. (init_LocalData, init_LoadShopScript byte,
  init_AllocSceneBuilding 41 -> 4 ins; ls_WriteEnvironment)
- **Frame one int too big, loop tail `inc ebp` / `cmp dl,[ebx+0x22]`**: a do-while pointer walk
  `p = &bt->slot[0]; do { .. i++; p++; } while (i < bt->nslots);` spills both IVs; write
  `for (i = 0; i < bt->nslots; i++) .. bt->slot[i]` (ai_BuyUpgrade 0x487fb0: frame 0x200 -> 0x1fc, 99 -> 48
  lines). Likewise `for (i = n; i > 0; i--) { ..*pts..; pts++; }` where the original counts down in a dead
  parameter's slot (frame 4 smaller) is `for (i = 0; i < n; i++) .. pts[i]` (rd_PointsNear, byte).
- **A decompiled `n = wn->nobjs; if (n > 0) { p = ..; do { ..; p++; n--; } while (n); }`**: the plain
  `for (i = 0; i < wn->nobjs; i++) .. d2_Objs[wn->objs[i]]`, plus a record pointer `o = &d2_Objs[*p]` for the h / y
  reads instead of locals `oh` / `oy` (32 -> 18). Four clip-sum operand-order hunks (`wn->w + wn->x`, every spelling
  one code) went with it: a commutative sum's order follows function-wide state. Fix decompiled loop shapes before
  an operand-order hunk elsewhere (d2_ResizeWindow 32 -> 4, stuck on the
  d2_ScreenH word, byte with it tentative; d2_DrawInput / d2_DrawSlider open hunks the same).
- **Table builder with `p += 0x24d`, `q` walks and a slots pointer**: index loops over `GM_BTYPE(i)` with one `bt`
  pointer and kind locals (VC6 makes both IVs and the objs pointer, `lea ebx,[edi+0x23] .. add ebx,2`, itself):
  119 -> 76; the class run `v = base + cnt * run / len` was `base += ..` (the original adds into base's
  register): -> 66 (gm_InitBuildingTables).
- **Guarded do-whiles over a hand-advanced `w`**: two index loops through a type pointer (`bt = GM_BTYPE(*b); for
  (i = 0; i < bt->objCount; i++) if ((unsigned short)(bt->objs[i] & 0x7fff) == *proto) break; if (i >=
  bt->objCount) return 0; for (i++; i < 0x40; i++) {..}`) and a goods pointer local (`g = GM_GOOD(id); if
  (g->type == 2 || g->type == 6)`, the original's `mov ecx,edx; add ecx,ebp; mov cl,[ecx]`): 50 -> 26; a `char k =
  GM_GOOD(id)->type` local instead of g: 42. Then `n = 0;` before `bt = ..` (with bt first VC6 folds the type
  address into the count load): byte (gm_ListObjectsAfter, 50 ins).
- **Stock blocks by int pointers with a +8 IV bias, building types by `i += 0x24d` and `w++`**: index loops
  (`gm_StockBlocks[i].f`, `GM_BTYPE(i)->objs[n]`) plus `g = GM_GOOD(id)` for the type tests (gm_LoadTables 20 ->
  byte). Naming GMBTYPE's byte +0x21 (AIBTYPE.extensions) in f3gm_types.h dropped sv_t's ai_TutorialInitChapter3
  (f3gm_types.h reaches sv_t through ai_bld.h): kept as a unit macro in f3gm.c.
- **A view walk over an int address (`v += sizeof(D2VIEW)`, signed jl)**: an index loop with a block pointer
  `vw = &d2_Views[v]` (VC6 bases the IV on vw->enabled itself, -210). Frame: `list[0x2f2]` was list[0x300] (the
  0x1c difference); 4-byte holes after the camera copies = D3VECTOR4; a 4-byte hole between a struct and the next
  array = the struct's last vector padded (CANNONSCENE 0x104); a vector used in one block declared there shares
  the earlier temps' slot. After making a vector D3VECTOR4 check every `sizeof(v)` (ai_CannonScene 378 -> 36,
  shape).
- **A hand `int *p = &w[n * 3 + 0x51]` fill walk**: `for (i = n; i < 8; i++) ((GVWIN *)w)->items[i].f = ..` gives
  the original's `lea [n*3+0x51]`; an `items` local folds the bias into the store (`[edi+eax*4+0x144]`)
  (ai_GraveDetail, stuck, draft 30 -> shape 22; left: the constant-0 web).
- **A free-slot pointer walk `for (i = 0, a = T; (int)a < (int)T + 0x110; i++, a++) if (!a->bank) { if (i >= 16)
  return -1; ..fill..; return i; }`**: `for (i = 0; i < 16; i++) if (T[i].bank == 0) break; if (i >= 16) return
  -1; ..` (the amt_GetFirstOffice rule), then store order (bank before x) (d2_StartAnim byte).
- **An out parameter spilled to its home slot at loop entry (`mov eax,[out]; mov [out],eax`) and bumped by the
  record size after the copy**: `out[n] = rec; n++;`, not `*out = rec; out++; n++;` (amt_GetColumnOfficesOwned 28
  ins -> byte).
- **`mov eax, 0x62c420` (address materialized) where the original has `mov eax, ds:0x62c420`**: not
  `int *p = (int *)0x62c420; while (*p) {.. p = (char *)p + 0x1c; }` but an index loop over the absolute address,
  `for (i = 0; U_I(0x62c420 + i * 0x1c) != 0; i++) if (U_I(0x62c420 + i * 0x1c) == id) return 0x62c420 + i *
  0x1c;` (pointer in ecx, index in edx: 0x5ba620 / 0x5ba660 byte).
- **IV init after the guard (`xor ebx,ebx` after `jle`), `i` incremented before `off`**: the index form `r = base
  + i * stride`, `i++`; an `off` accumulator inits before the guard and swaps the increments (0x51d3d0 byte).
- Varargs stored by index (`objs[j] = va_arg(ap, int)`) matched; a pointer with a countdown did not.
- **Exceptions, measure per function**: the pointer form matched 0x4e7150 ([ebp-8] = 0x925128 .. 0x9253f8, 207 ->
  205 hunks) but was worse for its sibling 0x4e7320 (135 -> 139 lines). A constant-trip pointer loop gets the
  absolute end compare (`add esi,0xac / cmp esi,0x7d6a42 / jl`) only as `do { ..; p += 0xac; } while ((int)p <
  0x7d6a42);` (0x51db00 byte); `for (k = 0; k < 16; k++)` keeps a counter slot (frame 4 larger), `for (;;) { ..; if
  (p > end - 1) return 1; }` puts the return block after the body.
- **Seven slot pointers in stack slots, each bumped by the element size** (ai.c 0x4854f0, [esp+0x10] ..
  [esp+0x30], +0x58): VC6 hoists one pointer per field for field inits of a global array element; a candidate
  source is a loop over a pointer variable (the offset form) instead of indexed stores.

### Loop pointer based on the wrong field

- **The strength-reduced base sits on the wrong field (`[eax-4]`, `mov esi, arr+0x25`, `mov ebx, T+0xc`)**: VC6
  bases the IV on the field referenced most often. Read the field as `arr[i].f` at every use instead of through
  a local copy or a record pointer local `p = &arr[j]`; when only some uses should count, re-read it in a subset
  (a 31-mask generator found it). Try it first on any walk biased by a field offset. (gv_SendCrimes: 3 reads of
  `.state` against the id's 2; sv_DrawBubbles: `ch = T[i].ch; if (ch == 0 || ((C *)T[i].ch)->obj->scene ..`,
  three of five uses, 13 ins -> byte; sv_DrawNamePlates: any one of four uses, 19 ins -> byte;
  sv_UpdateChimneySmoke: `ai_CharsT[i].f` in every test, 21 ins -> byte, also gave `mov bl,0xa`;
  ai_TokenRandomMerchant: the job byte read at its 8 tests, netId +4 -> +0x164; the netId base from
  officeKind by `ai_CharsT[i].netId` at each use; amt_GetOfficeCandidates; 0x48bc80 `ai_Entries[i].unk3c`
  everywhere 3352 -> 1414 B; d3t_CreateSnowMipmaps &dwWidth / &dwHeight, a `d = &arr[i]` gave the wrong base)
- **The base follows the ORDER of the references, not only their count**: writing the shared `x14 = -1` first in
  each arm (`if (owner == id) { x14 = -1; owner = -1; ...} else if (x14 == id) { x14 = -1; state = 1; }`) moved
  `mov ecx, tbl+0x14` from owner to x14, the store still tail-merged (amt_ClearOffices, amt_ClearOfficesIfAll
  byte). The stores after a struct copy: `grid = ..; on = 1; y = ..; x = ..` put the base at the record start,
  `grid; y; x; on` at `.y`; only the index loop with the struct assignment `ai_WalkSteps[k].grid = tbl[idx++ %
  n]` (not three word copies, not a pointer loop) reached it: permute the stores (ahm_FillSteps 30 ins -> byte).
  Test such rules on a toy loop first (deep-tools/toyasm.py, 0.2 s).
- **Toy results (not yet a usable rule)**: on `T tr[10]` with `foo(t->x)`, the base comes from the order of the
  post-CSE references: distinct fields -> the 2nd referenced one (offset 0 never counts), repeats by a count /
  recency rule (b b b c -> b, b b c c -> c, b c c b -> b, ecded -> e); dead loads / stores do not count. Toy
  harness: the loop in build/toy/x.c and dcp listing (0.1 s). Still unexplained: &t->volume vs fadepos in
  msx_Update, &sl->weight in d3_Unk459300, cl in qnt_Map.
- **Index form, not a pointer**: `AHMSTEP[3]` save/load keeps `edi = &step + 0x10` (`mov edi,0x6f0df0`, `cmp
  edi,0x6f0e38`); `char *s` offsets, `AHMSTEP *p` with `p++`, `p = &ai_WalkSteps[i]` re-centre at +0x14, only
  `ai_WalkSteps[i].field` matched (0x4ebfb0 / 0x4ec140). `p = list; p++` gave `edi = &p->state` where the original
  keeps the record base (`[edi+0x10]`); `list[j].field` flipped it, 52 -> 37 (0x4e8160). A save loop `for (p =
  tbl; (int)p < (int)&tbl[N]; p++)` based on kind (+8): `tbl[i].field` gives owner (+4) (amt_SaveAemter byte);
  retry the index form even when a tag says it was tried.
- **Write the natural indexing, not the bias**: the original's `edi = offset ai_Chars+2` with `[edi-2]` /
  `[edi+0x20a]` comes from `c = (char *)ai_Chars` with fields `c+0x20c`, `c+2`; `c = (char *)ai_Chars + 2` or an
  `extern char ai_CharKinds[]` view at 0x770b82 fold the -2 into the symbol (`[esi+0x20c]`) (0x51a4b0).
- **Base re-centred on the middle accessed address whatever the spelling** (char* offsets, int* elements, a plain
  int address, for / while / do-while all the same): 0x51b910 `edi = p+4` vs `p`, 0x51c2f0 `esi = record+0x14` vs
  `record`, 0x51c870 `esi = p+0x68` (open).
- **Bias on the second field of a slot loop written all through `e->`** (`edi = slot+0xc`, the original's on
  `parts[0].v2` +0x38): a pointer per sub-record, `p = &e->parts[k]; p->v0 ..` (gv_WriteEffects byte, gv_ReadEffects
  the same by dcp try). No lever: `(e + i)->`, `&a[0]`, casts, struct-typed vectors, loop forms.
- **Two base pointers in one loop** (0x51f080: biased `s = 0x7e3f88` in a stack slot reloaded into ebx, `p = s -
  0x48` in edi; reads up to s+0x19 on s, from p+0x104 on p): `p = s - 0x48;` as its own pointer (180 -> 94 hunks).
  Every read from the slot base re-bases the whole loop; the bound stays on s (a p bound reads one slot too many).
- **Three stores through two pointers into one buffer** (`cl = clamp + 0x100`: `cl[k] = k; clamp[k + 512] = 255;
  cl[k - 256] = 0;`): the store spelling picks the IV family and the register map after it (qnt_Map 822 -> 563
  B); try the other pointer's spelling (`clamp[k]` = `cl[k - 256]`) and the three orders.
- One loop, one cast view of a global array (two views are two IVs, the bias changes); a pointer IV with a
  constant start puts the bias on the field with the largest offset. A 10-byte `#pragma pack(1)` struct view
  still re-bases (186 vs 161 lines); the int-pointer view with a named symbol plus a typed `unsigned char *kind`
  set at the loop head is byte-neutral and clears Q4.
- An index loop with `sl = &arr[k]` puts the pointer at the most-used member, a pointer loop at the struct start.
  `((T *)base)[i].f` folds base+offset into the displacement, a pointer local adds the base first; repeat the
  macro to keep reloads after asm blocks. `l->k[key].pos[i]` (no pointer locals) gives a byte-offset counter
  based at the middle field.
- **Loop pointer choice from locals**: try `p[i].f`, `p++`, `i++, p++` vs `p++, i++`, a local for a repeated
  pointer load (`k = info->keys` before each loop: the biased base and integer float copies; `info->keys[i]`:
  fld/fstp copies). `v = m->verts; v0 = v;` vs `v0 = m->verts; v = v0;` pick different bases; index-form
  `strlen(d->mats[i])` gives two induction pointers. Strength reduction over `obj->material->tex[i]` only with the
  material in a local. Re-reading the loop bound in the test fixed a strcmp search loop's registers.
- **A key pointer one key too far (`[ecx-0x5c]` for the time)**: `keys[k - 1].time` (d3t_MoveCameraPath).
- **Bbox loop folding the first step into `[ecx+0x50]`**: `for (i = 1, v++; i < 8; i++, v++)` with `min(mn.x,
  v->x)` / `max(mx.x, v->x)` (accumulator first); `v[i]` or `v++` at the top folds it, explicit `if (v->x <= mn.x)`
  stores flip the flag tests; the pointer step first, `tri++, i++` (d3t_QueryHeight).
- **`[base+index+disp]` order**: an index VC6 turns into its own IV (`a[i]`, `i * sizeof`) is the base; an explicit
  byte-offset variable (`(char *)p + j`, `j += 0x54`) is the index; zeroing the offset after the guard needs `if (i
  < n) { j = 0; do {...} while (i < n); }`. No lever: pointer arithmetic, `k[arr]`, byte sums, parentheses
  (par_Create, d3_LoadBinaryAnimStream); the loop form decides.
- **`mov eax,[esi+eax]` against ours `[eax+esi]` for `list[i]`** (esi = i * 4, the global reloaded into eax): the
  loop ran zero-based, `for (i = 0; i < 8; i++) if (lobby_List[i + 1] == 0)`; the twin loop above it is 1-based
  (lobby_ChoosePlayer byte). No lever: `*(i + p)`, `*(p + i)`, an element local, nested ifs.
- **`[esi+eax+0x74]` against ours `[eax+esi+0x74]` for `fin->jobByGroup[r]` in a loop, 2 ins / 1 byte, climb and
  flip find nothing**: the index variable was the earlier loop's index reused (`m` of the relatives loop as the
  random group), not a variable of its own (ai_DecideApprenticeship byte). No lever: `r[...]`, `*(r + ..)`, raw
  char offset, a named byte pointer, loop forms.
- **The IV base 8 bytes off in a long record loop, every spelling = control** (msx_Update: ours `&t->fadepos`,
  the original `&t->volume`): the base follows the loop's field references after CSE, not their spelling; probes
  (never kept) in one block move it: removing one `t->volume` read from the fade-out arm's expression gives the
  original's base, removing `t->pos` gives pos, adding `t->volume` or removing `t->fadepos` changes nothing. Use
  `flip.py --want "mov esi, OFFSET FLAT:_arr\+\d+" --avoid <ours>` to find the block, then a probe per field.
  No lever: `arr[i].f` at every use against `t = &arr[i]`, operand orders, continue against nested ifs, the
  duration written out at each use, if/else against or-chains in the fade arms. (open)
- **The IV base one field off and no single statement change moves it (flip.py: no hit)**: count the
  references per field, including tests VC6 decides and drops. ai_ChooseAction (base &.eval, the original
  &.decide): a probe with one more `act->decide` reference moved the base; the byte source tests decide
  twice (`&& act->decide != 0` then `act->decide != 0 ? act->decide(..) : 0`): the repeated test leaves no
  code but counts. Named locals for eval / decide (base -> +0), operand orders, nested ifs: no lever.
- **A value the original compares with the 0 held in a register (`cmp eax,esi`) where ours has `test
  eax,eax`**: the call result went into a variable first (`r = f != 0 ? f(..) : 0; if (r != 0 ..`), as the
  other callback's result in the same function (ai_ChooseAction).
- **The IV base on the field a CSE'd value is read from (`mov esi, arr+0x110` vs `+0x118`)**: a value read twice
  in one block (`t->volume` in `dur = t->fadevol * k / t->volume` and again in the curvol product) was a named
  local, `vol = t->volume; dur = .. / vol; t->curvol = .. * vol / dur;` - the same machine code (the CSE temp
  and the local both sit in ebp), but the base moves. Try a named local for every field value used twice before
  any field-reference probe. No lever: everything in the entry above (msx_Update 104 ins -> byte).

### Row and pixel pointers

- **An `imul` per row behind the inner loop's `w > 0` guard, ours a running row pointer plus a pitch*n slot**:
  compute the address in the inner loop body, `for (c = 0; c < w; c++) { p = pix + ((r + y) * pitch + x + c) * 3;
  ..}` or `src[(y + i) * pitch + x + j]`; VC6 hoists it to the inner preheader behind the guard and never
  strength-reduces it over the rows. A row pointer assigned before the inner loop is SR'd whatever its spelling
  (0x47a460 / 0x47ab30 / 0x47ad40; d2_GrabShape16NoRuns 183 -> 6, d2_GrabShape24NoRuns 274 -> 34 ins).
- **The copy loop of the same functions does strength-reduce** (stride in a dead parameter slot, the row pointer
  behind the `s->h != 0` guard): `for (i = 0; i < s->h; i++) { p = src + (y + i) * pitch + x; memcpy(dst, p, s->w
  * n); dst += ..; s->npix += ..; s->size += ..; }`, `dst +=` first; a `p += pitch` do-while puts the base before
  the guard.
- **Packed word at an odd offset (D2KEY.gb) read as r + g dword plus b byte where the original loads the word**:
  the object was 3 bytes, not 4 (the colour-key rule).
- **A skip loop whose original sets its pointer up after the entry `jne`**: `while (pix[t + col + x] == key)`, not
  `p = pix + t + col + x; do { ..; p++; } while (*p == key);` (d2_GrabShape16Runs 68 -> 18 ins).
- **A row pointer at `buf + 1` with `p - 1, p, p + 1` operands (`lea esi,[edx+0x1]`, `[esi-1]/[esi]/[esi+1]`)**:
  the plain `&buf[(y * w + x) * 3 + i]` in two `for` loops; VC6 picks the middle element itself. A hand `rp = buf
  + 1; p = rp;` (or `rp = src; rp++;`) is rebased (`mov esi,edx; [esi+2]/[esi+1]/[esi]`) and lands in another dead
  parameter slot (d2_Convert16ToRgb / d2_ConvertRgbTo16 byte; 0x478bd0 / 0x478c50). Also: `p[x*3]`,
  `p[x*3+1]`, `p[x*3+2]` give the base+1 pointer, `p += 3` does not.
- **Blur loop**: `up/row/down = &tmp[(y-1 | y | y+1) * size]` per row gives the three pointer IVs and the frame;
  the x-loop guard re-tested in the y loop comes from `x < 1 << i` instead of `x < size` (d3t_CreateSnowMipmaps).
- A row pointer created only after the entry check is strength reduction of `base[i * stride + j]`; `y + i` in a
  call is strength-reduced after the guard, a running `j = y` is loaded before it. A tile loop is `blit(xx + i *
  tw, yy + j * th)`. A transpose `dst[i*4+j] = src[j*4+i]` on float pointers matched; pointer walks swap eax / ecx.
- **Recomputing a search bound min(N, n + 32) per iteration with n in a scratch register** (ours hoists n + 32 and
  spills n, frame 4 larger): test the first cell through the array (`!pts[n].x && ..`) and start the pointer in
  the for-init, `for (i = n, pt = &pts[n]; i < min(0x100, n + 32); i++, pt++)` (ch_ActWalkCells frame 0x1ac ->
  0x1a8, 54 -> 4 ins). A zero-fill of two-byte records indexes the record type (`((CHPATHPT *)p)[i].x = 0;
  ..z = 0;`), not bytes `[i]`, `[i + 1]` with `i += 2`.

### Rotated against top-tested loops

- **Test at the top, `jmp` back**: `while (1) { if (!(cond)) break; .. }`; `while (!cond)` and `for (;;)` rotate
  (the head duplicated). (`while (1) { if (cm_GetSlotState(h) != 0) break; pump(); }`; d2_DrawTextLine, stuck 436
  -> 133: the push-past-objects loop, -150 with the glyph block first `if (glyph != -1) {..} else {..}`)
  The trace: after the call, `jmp` back to `test eax,eax; je` (no `test; jne body; jmp exit` at the bottom).
  ai_FindGoodToMake: `o = gm_Query(..); while (1) { if (!o) break; if (match) {..; break;} o = gm_QueryNext(); }`
  byte, where for / while (o) / for (;;) { if (!o) break; } / a goto loop all gave the rotated code.
- **`&&`-guarded while stays top-tested**: `while (r != -1 && cm_GetSlotState(r) == 0) sv_PumpNet();` gives test /
  jne out / body / `jmp` back; `if (r != -1) { while (..) .. }` and `for (;;) { if (..) break; .. }` rotate (+29
  bytes) (amt_CheckOffices byte, amt_UpdateOffices). A loop-invariant test in the condition does the same,
  `for (y = 0; surf != 0 && y < surf->h; y++)` with i zeroed before the test (`cmp y,[surf+8]; jge` at the head,
  the cached zero re-zeroed at the exit); `y = 0; if (surf) while (y < h)` rotates (sv_LoadGroundplan 49 -> 4).
- **Search loop testing at the top, bound at the bottom** (`cmp [eax],dl; je; add; inc; cmp end; jl`): VC6 rotates
  `while (p->idx != x) {..}` (peels the first test, `mov ecx,DWORD PTR ds:amt_Table; and ecx,0xff`); write `do {
  if (p->idx == x) break; p++; i++; if ((int)p >= (int)&amt_Table[N]) return v; } while (1);` (0x4e6360 byte;
  0x4e6430 / 0x4e6680 / 0x4e6710 45-115 -> 0-16). A pointer `>=` gives `jae`. `while (p->idx != x && p < end)`
  gives a bottom test plus a duplicate (+3). A message byte against AMTREC.idx needs both unsigned char.
- **Top-tested, unstrength-reduced search (`cmp i,n; jge` at the head, `mov edx,[w+0x18]; inc eax; mov
  edx,[edx+eax*4-4]; cmp; jne head`)**: both tests in the condition, `while (i < n && w->objs[i++] != obj) ;`; the
  decompiled `for (i = 0; i < n;) if (w->objs[i++] == obj) break;` rotates and moved the constant 0 from edi to
  ebp for the whole function (d2_RemoveObj 75 -> 14, byte).
- **A `for` with a `goto` out stays top-tested** (`cmp i,[mem]; jae out` .. `jmp top`); with `break` it rotates
  (guard `test; jbe`, bottom `cmp; jb`, the bound CSE'd) and a following test on the exit condition is threaded on
  the normal exit only. A hit that keeps the outer `do { } while (start)` running: `start = NULL; for (...) { if
  (hit) { ...; start = ...; break; } }` at the do-body end (d3_mir d3_Unk454890: 168 -> 6 B).
- **Pointer step in the for-update vs the body**: `for (p = list; n > 0; n--, p++)` keeps the update after the
  body (`continue` jumps forward); `p++` in the body rotates (update before the body, an entry `jmp`, p / count in
  memory), 90 bytes on this alone (amt_CheckOffices).
- **Loop-entry `jmp`**: `x0 = x1; x1 = ...` at the top of the body, not the bottom (surf_Shrink16). The init order
  of a group of globals decides which one is enregistered in a later loop.
- **Unrotated search**: `for (it = first; it; prev = it, it = it->next) if (it->obj == obj) break; if (!it) return
  1;`; `if (n == 0) return 0;` before the body keeps its own epilogue. `while (1) { if (i >= N) break; ...; i++; if
  (found) break; }` is unrotated; a rotated `while (!found)` with an in-body `if (j >= n) break;` has `j = 0`
  before the guard.
- **Bound `n < 3` tested right after the last call, `b` in the shared top block, a redundant post-loop `n >= 3`
  with a second return-1 epilogue**: `for (b = first; n < 3 && b != 0; b = next)`, the count updated at the body
  end (0x4b4760 44 -> 5). No lever: `b != 0 && n < 3`, a `while`, the increment in the body plus `if (n >= 3)
  return 1;`, `continue` with an inverted tail (33-62).
- **Both exits to a shared `cmp`**: the second loop's bound re-read from the struct (`i < obj->unk1cc->nkeys`)
  instead of a local `n`, so VC6 cannot thread the first exit into the second guard.
- A self tail-call becomes a loop. `goto` to a label at the end of the body becomes `continue`.

### Search loops: hit path and re-tests

- **The exhausted loop not threaded to the return (`jl top` falls into `cmp i,n; jge ret0`, ours a return copy
  behind the loop)**: the bound read from the struct in both places, `for (i = 0; i < t->n; i++) ..; if (i >= t->n)
  return 0;`, not a local `n = t->n` (with the local VC6 knows i >= n and threads) (ai_DecideProduceGood 119 -> 41).
- **Found path inline after the loop's `jmp` (`je hit / .. / jl top / jmp after / hit:`), ours after the following
  code**: the layout follows the register allocation; with a parameter forced to memory (probe `{ static int *pm;
  pm = &mode; }`, never kept) the plain `for + if { hit; break; }` gave it. Probe this way before loop spellings
  (amt_Rating).
- **No second `test n` / return-0 block**: take "found" inside the loop, `o = 0; while (n) { if (match) { o =
  &a[i]; break; } ..} if (o == 0) return 0;`, not `break` + `if (n == 0) return 0;`. The found code (with return)
  in the body gives the direct exit; `break` + `if (i >= n)` re-tests the counter; `for (...) if (hit) goto found;
  return; found:`. Found body in the loop loses the `if (b)` test after it (`for (b = head; b; b = b->next) { if
  (b == key) { ...; break; } prev = b; }`, sb_UnloadSampleBank). A `found` flag or `goto next` in a duplicate
  search matches; `if (i == n)` after the loop adds a cmp.
- **Found path `je` straight to the strlen, the exhausted loop into the error push**: the found work (strlen, the
  store) inside the loop before its `break`, one shared `if (k >= 15) error` after it (VC6 threads the exhausted
  loop, k known 15) (hst_ParseContext byte).
- **A re-test on the break path only (`dec esi; jns top; jmp out` / `test esi,esi; jge fit`)**: `for (k = wl; k >=
  0; k--) { .. if (fits) break; } if (k >= 0) {fit} else if (ln >= 2) {..}`; the decompiled `if (k >= 0) goto
  fits; break;` inside gives other code. A loop-invariant `sub ebp,[wd]` before the loop with `add ebp,[wd]` on the
  no-fit path is the source's `lw -= wd; .. else lw += wd;` (d2_DrawTextBox, stuck 518 -> 441).
- **Table search with two pointers** (an iterator in ecx, `mov edi, ecx` per iteration, `cmp [edi], ebp` reloading
  the field, the first test on `[table]` before the loop): `for (n = 0; T[n].kind != 0; n++) { e = &T[n]; if
  (e->kind == ...) goto found; }`; a pointer loop or `e = p` coalesces both (snd_EnterBuilding 38 ins -> byte).
- **Walk to the terminator, the test after the step**: `i = 0; do { ... } while (T[i++].kinds[0] != 0);`; `(l++)->
  kinds[0]` or a `k` pointer keep the terminator in a register (snd_FindTrackKind byte).
- **Counting the slot it stops on**: `while (t[i++].p && i < 32); if (i > 32) ...; i--; t[i]` (the `i--` gives
  `lea [ecx+ecx*2-3]`; `t[i - 1]` folds into the displacement).
- "Last used slot + 1, else 0": `for (i = 0x2ff; i >= 0; i--) if (a[i]) { i++; if (i >= 0) goto found; break; }
  i = 0; found:` (d2_Unk46c9f0).
- Guard `if (t && t->list)`, loop `for (r = t->list; r->y >= 0; r++)`, hit via `p = ...; break;` and one `return
  p`; or the success path inside `if (i < MAX) {...}`. `j = 0` stored before a `jne` on a found flag is `for (j =
  0; !found && ...)` with no `if (!found)` around it; `ok = 1; while (i < N) {...}` with a bottom test is `for (i =
  0; ok && i < N; ..)`.
- **A loop index set before an early-return test (`lea ebp,[ecx+1]`)**: `j = i + 1; if (hand[j] < 0) return best;
  for (; j < 5; j++)`; `for (j = i + 1; ..)` copies it (ai_PokerHighCard). But a loop from a computed index needs
  its own start: `for (i = top; ..)` matched, `i = expr; for (; ..)` not.

### Counters, countdowns and increment order

- **The original's outer loops are count-down trip counters (`sub eax,esi; mov [esp+x],eax .. dec; jne`),
  ours `lea eax,[ecx-1]; cmp eax,ebp; jl`, and a following loop over the same range re-tests its entry**:
  the bounds were not locals; `for (i = start[lv]; i < start[lv + 1]; i++)` read in place in every loop
  (gm_LayoutTree 289 -> 163: `s = start[lv]; e = start[lv + 1];` locals give LFTR on i + 1 and thread the
  second entry test away).
- **`k = 0` set after the loop guard, `add ebx,edi` scheduled after the idiv**: `k` was not a variable;
  the original wrote `u * d / (n - 1)` and VC6 strength-reduced the product into the k IV (its 0 lands in the
  guarded preheader). `for (.., k = 0; ..; u++, k += d)` and `k += d` in the body are each a step off
  (gm_LayoutTree 4 -> byte).
- **A parameter used as a countdown** (`x0 = x0 - x1; if (0 < x0) do {..} while (--x0 != 0); return;`): `n = x0 -
  x1; for (i = 0; i < n; i++) d2_Plot(x1++, ..);`, VC6 makes the `dec edi; jne` (94 -> 78). Then `diagonal = (dy -
  dx) * 2;` before `straight = dy * 2;` (the first `mov eax,edi; lea ecx,[edi+edi]; sub eax,ebp`), `e = straight -
  dx` (not `dy * 2 - dx`: +100 ins), `y0 += sy; e += diagonal;` (d2_PlotLine byte, 94 ins).
- **Write Ghidra's do-whiles as plain `for` loops**: a `for` whose counter is unused in the body becomes the `dec;
  jne` countdown; `{ if (*p == c) *p = d; p++; }` counted gives `dec ecx; jne`, `p[i]` gives `cmp/jl`; `cnt = n->n;
  for (i = 0, p = n->v; i < cnt; i++, p++)` gives the countdown (p set before swaps regs); `for (i = nv; i; i--,
  p++)` in `if (nv > 0)` keeps the pointer load after the guard; `for (i = 0; i < n; i++) va_arg(...)` gives the
  countdown in a stack slot, `while (n-- > 0)` not. Index loop over a global struct array: `dec ecx; jne` based at
  the second member.
- **`dec ebx; test ebx,ebx` where the original has `dec ebp; mov esi,edx; jne`**: `} while (--n != 0);`, not `n--;
  } while (n != 0);` (0x50e5a0 37 -> 20).
- **Fixed count 3 with the wrong IV merge**: `k = 0; do { ... } while (++k < 3);` gives `dec edi / jne` with the
  pointer IV (d3_Unk4550e0); for / while / index spellings canonicalise the same.
- **`cmp edi,0xa900; jb` through the derived offset where the original keeps `mov ebx,0x100 .. dec ebx; jne`**: the
  counter was a `short` (gm_TakeOverCharacter). No lever: int / long / unsigned, any bound spelling.
- **Hoisted `neg w` and an extra incremented frame slot**: `j * w` strength-reduced (test `row - j + y0 > 0`, index
  `j * w`), not `i -= w` or separate `e` / `i` locals (d3s_DrawSpans byte). `t = 0` in the preheader with `t +=
  delay` per iteration is `i * delay`.
- **Counter spill position**: `for (n = count * 2; n > 0; n--)` spills after the prologue pushes (`mov
  [esp+0x10],eax`, the original, byte); `n = count * 2; if (n > 0) do {..} while (n != 0);` before the entry branch.
- **dec scheduled before the modulo's cdq**: `while (k != 0) { ...; k--; pos = ...; }`; the for-update `k--` lands
  after the idiv.
- **Backwards walk with a pre-decrement**: `for (i--; i >= 0; i--)` (the init before the callee-saved pushes, a
  separate test/jl); `i--; while (i >= 0)` fuses dec/js.
- **Increment order at the loop end**: try both `e++, i++` / `i++, e++`, `drop++, i++`, `i++, vt++` (per function).
  `while (m) { m >>= 1; n++; }` gives `shr; inc; test`, `for (; m; m >>= 1) n++` gives `inc; shr`.
- **Record pointers advance per record** (`add ecx, 0x18` per polygon): `p->uv = t; ... t[0..5]; p++; t += 6;`
  per polygon, not `p->uv = t + 6; t[6..11]; ... t += 12;` (d3s_MeshFromMap 247 -> 181).
- **Running min in a local, bound in the parameter**: `size <<= 16; ymin = size;` (min loop on the local, check
  `> size - 1` on the parameter) hoists `size - 1` and keeps the min in a register (d3s_ShadowPoly draft ->
  shape); the reverse keeps the bound in a register.
- **A local copy loop of 4 / 8 / 0x14-byte records**: the store before the pointer add (`*f = ..; j--; f += 3;` vs
  `f += 3; j--`) moves the `fstp`; `do { *d++ = *s++; } while (--n)` is `for (i = n; i != 0; i--) *d++ = *s++;`.
- **Top-N insertion**: `for (k = 4; k > j + 1; k--) best[k - 1] = best[k - 2];` (struct copy, guard `cmp j+1, 4;
  jge`, pointer at best[3]); stop test `if (!(s > best[j - 1].score)) break;`. Loads `T1[m - 1] .. T4[m - 1]; f(m -
  1)` decrementing before the last load: one index local `e = m - 1` (hmt_Election, draft 95 ins -> byte).
- Two loops over one grid may use separate column counters. Loop-invariant values sit in the preheader after the
  `count > 0` guard: a value the original computes there (alpha, a size factor on the FPU stack) was written in
  the body.
- **A count loop with the counter loaded into edx, `inc edx`, stored to its slot, and `mov edi,edx` after the
  loop** (ours counts in edi directly): the increment in the for header with an empty body, `for (m = max(..),
  levels = 0; m >= min; m >>= 1, levels++) ;` (not `levels++;` as the body, not a while loop) (d3d_LoadTexture
  77 -> 70 ins).

### Signed jl against unsigned jb, bounds and guards

- **`jl` / `jge` on a loop pointer is an index loop or an int cast, `jb` / `jae` a pointer compare**: `for (i = 0;
  i < N; i++) ARR[i].f` (also strength-reduced, `cmp esi,END; jl`), or `(int)q < (int)AMT_CHARS_END`, `while ((int)c
  < (int)(ai_Chars + 768))`, `for (m = arr; (int)m < (int)(arr + n); m++)` (rt.c 0x4bdae0 / 0x4bdca0 / 0x4bf3b0;
  ls_WriteEnvironment).
- **`cmp esi,const` on the biased IV**: an index do-while `i = 0; do { c = &arr[i]; ... } while (++i < N)`.
- **Q3 symbol bounds, measure per function**: `(char *)ai_Chars + 768 * 0x21c` (0x7d5f80) cost +11 lines in
  amt_ElectOffices, `(char *)ai_Chars + 2 + 768 * 0x21c` (0x7d5f82) was neutral in amt_CheckOfficeColumns. With the
  literal `0x74db40` VC6 kept pb, `(int)he_QuickList` re-based the IV to pb+0x70 (keep the int cast, record the
  re-base for match-deep). `while ((int)p < (int)gm_PlayerRecordsEnd)` at 0x7d6a42: 84 lines, the literal byte
  (recorded as @deep on 0x51db00).
- **`cmp ebx,ecx; jb` after `repne scasb`**: `i < strlen(s)` without `(int)` (`(int)strlen` gives `jl`);
  `for (unsigned i ..; i < strlen(s); ..)` gives `repnz scasb` per iteration + `jb`.
- **Guards**: `for (m = 0; m < *p; m++)` gives `cmp mem,0; jbe`, an extra `if (*p != 0)` doubles it; an unsigned
  count gives `test; jbe`. A redundant `if (n > 0)` around a for adds a second test (one on the register copy, one
  on the reloaded slot): drop it when the body only sets `pick` (amt.c 0x4e8c20 12 -> 8 hunks, byte).
- `for (dir = 0; dir < 8; dir++)` signed gives `inc; cmp eax,8; jl`; `for (;;) { ... if (++dir > 7) return; }`
  gives `cmp eax,0x7; ja`. `if (col <= 6)` gives `cmp eax,0x6; jg` (the original), `if (col < 7)` `cmp eax,0x7;
  jge`.

### Other shapes found on loop functions

- **`add ecx,0x253; add ecx,ebx`**: `(b ? 0x27b : 0x253) + type`; `(b ? 0x28 : 0) + 0x253 + type` folds into one
  `lea`. A `char text[260]` got an 8-aligned slot with a hole (frame 8 larger); `[256]` gave the original's frame
  (hmt_Election).
- **A fallback whose original pushes px/pz straight into the call**: `if (f(..)) g(x, z); else g(px, pz);`, not `if
  (!f(..)) { x = px; z = pz; } g(x, z);` (rd_PointsNear).
- `or eax, reg` in an epilogue is VC6 copying a value it knows is -1.
- **Compiler facts**: a two-level strength-reduced index loop reloads its pointer slot at the outer latch (`mov
  eax,[slot]`) and stores it back at the head, a hand-advanced pointer has none. A load above a `rep stosd` is the
  eax / ecx / edx rotation one step off (a statement-order fix in that block), not an escape question. The frame's
  density sort is the CRT qsort (middle pivot, selection sort for 8 or fewer); a tie hunk can be simulated from
  the B rows of c2frame.py. With only swaps between equal-density slots left, remove named locals for values VC6
  would keep as temporaries.
- **A pointer computed through a temp (`lea eax,..; shl eax,5; lea edi,[eax+T]`, ours straight into edi)**: write
  the address a second time in the compare, `in = &d2_Inputs[k]; if (d2_FocusedInput == &d2_Inputs[k])` (the
  d3io_AttachStock lever); plain `o = &d2_Objs[obj]` / `memset(&d2_Objs[obj], ..); d2_Objs[obj].id = obj;` for
  the decompiled `off` (d2_RemoveObj byte).
- **`mov ecx,eax; add ecx,ebx; mov cl,[ecx]` for `ai_GoodsT[id].type` (ours `mov cl,[eax+ebx]`)**: a goods pointer
  local `g = GM_GOOD(id); if (g->type == ..)` (gm_ListObjectsAfter, gm_LoadTables byte). The pointer only works
  without a char `k = g->type` copy: every test reads `g->type` (ai_FindGoodToMake: `k = g->type` = control,
  `g->type == '!'` / `== 2 || == 6` byte). No lever in
  sv_LoadGroundplan (open): spelling, local, type, test form.
- A forwarding wrapper (`if (flag) return 0; return f(a, b);`) compiles to one `jmp f` (also with a changed
  argument written into the incoming slot); a function ending without `return` keeps eax from the last call /
  test (`if (r == 0) { ...; return f(x); }` with nothing after it gives the bare `ret`). `if ((double)t >= lim)`
  gives `fcomp; and eax,0x100; jne`, `!(t < lim)` gives `test ah,5; jnp`. `or ch,0x80` is `| 0x8000`, `or
  dl,0x80` is `| 0x80`. do { } while after an `if (first call)` runs the wait loop at least once.
- `int *r = &x->refs; if (*r > 0) (*r)--; else err(msg);` gives a tail jmp. A reverse walk over x / y pairs as
  `p[4 - i*2]` / `p[5 - i*2]` keeps two pointer temps that `q -= 2` merges. `for (k = s, len = strlen(p); ...)` and
  `len = strlen(p);` before the loop shape differently. A 16-byte frame hole only partly used is a D3VECTOR4 /
  `float[4]`.

## Register allocation
Which value gets which callee-saved register, the eax / ecx / edx rotation, constants and zeros kept in registers, spills and splits.

### How the allocator decides

- **Priority (read from c2.dll)**: callee-saved registers go out in priority order, summed per basic block:
  P(block) = distinct allocation candidates referenced in it (variables, CSE temps, webs of a scalar global
  between kills, and constants used more than once, one web per value whatever the spelling); +2*P per
  reference, -P per block lived through unreferenced. A tie goes to the candidate created later.
  `uv run c2prio SRC FUNC` prints P per block and every priority / register, `--ir` the instructions C2 counts;
  compute what one more or one fewer candidate in a block does before writing variants (sfx_OpenDigOutput:
  i 68 vs 66, matched at 66 vs 66). `c2dag.py SRC FUNC --alloc` shows two candidates' priorities and per block
  P and the references.
- **Choice rule (color.c; FUN_1073122d driver, FUN_10708a6e queue, FUN_10708af7 interference / pressure,
  FUN_10709937 choice, FUN_10709bb6 removal)**: candidates are popped in descending priority (equal: the
  later-created first). A candidate live across a call can only have ebx / ebp / esi / edi; one referenced by a
  BYTE-sized operand only eax / ecx / edx / ebx, so across a call only ebx. For each available register a score
  is summed from the still-waiting interfering candidates: their preference counts, and 100 x savings for a
  candidate whose only possible register that is; the lowest score wins, ties in the order eax, ecx, edx, esi,
  edi, ebx, ebp. Consequences: (1) a byte-flagged value that lives across calls (a pooled constant compared
  with a byte: `kb_Last == 1` flags every use of the constant 1) reserves ebx; higher candidates then avoid
  ebx and take ebp; (2) when the fourth callee-saved register is taken, everything else live across a call is
  SPLIT, and the pieces come back with new priorities and can overtake an older candidate (main_ChooseHistory:
  a piece of the constant zero at -86 took esi ahead of grp at -91), so a candidate must be served before the
  fourth register goes; (3) savings below 1 = no register; under /O2 a use weighs 1 << loop depth, a constant
  counts only where the other operand is memory, each load of it costs the weight; (4) a block where the
  candidate is dead (a definition follows on every path) costs it nothing: a flag reset at the top of each
  iteration ranks much higher than its reference count suggests; (5) spellings that fold before allocation
  (`x = y` where y is known to be 1, a named copy of a global) change nothing.
- **What never reaches P**: casts, constant spellings, labels, constant locals, single-use locals in
  expressions. `int z = 0` / `i = 0` / `ok = 1` used in place of literals are constant-propagated (same
  immediates; only a variable the compiler cannot fold gives a zero register, `cmp reg,reg`) (di_UpdateButtons,
  evt_DeclareLocal, d3sky_FadeLayer, 0x4b58b0). Expression temporaries are no candidates: they get what the
  global allocator left over. A `volatile` read is no reference of the field's web. Float `= 0` stores are
  immediates, no candidate; an int 0 is. A fresh local with one definition is forward-substituted (= the
  expression written out); the same temp in a local the function reuses elsewhere (`i` of an earlier loop)
  stays a variable with its own register (ts_LoadSoft: `i = y * tex->size; dst[i + x] = remap[src[i + x]]`
  keeps the byte in bl, 46 -> 11 B; a fresh `r` no lever; index spellings, operand order, casts, `p[i]` /
  `*(p + i)` / `i[p]`, int vs unsigned, post-increments: canonicalised, one result). CSE temps rank below named
  locals. VC6 canonicalises the loop shape before allocation.
- **Code-neutral candidates**: a local passed as a call argument (`n = f(); g(n);`), and a local with a second
  use that C2 folds from the test it sits behind (`n = f(); if (n == 1 && ..) g(x / n);`: n stays in the test
  block's P).
- **The scratch rotation**: eax, ecx, edx are handed out round-robin in IR order; a local in eax from the global
  allocator does not advance the pointer. A candidate that replaces a scratch temp (`size = n * 4; f(size)`)
  rotates every later scratch register of the block, so add candidates that fold away or sit in a callee-saved
  register. One temp more moves the rotation for the whole rest of the function: the distance is not monotone
  while the rotation is out of step; judge a block's spelling (and an order round's best) only after the blocks
  before it are right; keep the candidate that fixes its own block (main_EndRound 74 -> 140 -> 38,
  main_RoundReport).
- **A callee-saved register the original never pushes** (ours uses ebx, the original ebp / esi / edi) is a
  candidate that got that register but never materialised, usually `const#0`: the original ranks it between
  the two values (d3sky_CreateLayer, vfs_FindDir).
- **A store of a literal takes the register that already holds that value**: `r = 1; sv_PopRequest = 1;` gives
  `mov [sv_PopRequest],ebx` after `mov ebx,1` (ai_Unk4b3d50). An unexplained `mov [mem],ebx` after a
  `mov ebx,1` is a variable assigned 1 in the same block, not a CSE of two immediates. A literal used in two
  distant statements can get a callee-saved register (the constant web): in 0x4b58b0 the 1 pushed for
  `d2_SetForm(view, 1)` is still in ebx at `sv_PopRequest = 1`; ours with two immediate uses never
  materialises it (open).
- **"Tie-break sensitive", mechanically (color.c, on d3anim_CreateMorphAnim, open at 16 ins)**: (1) reloads at a
  block edge behind a register-killing region (an inline-asm block, a forced split) are emitted in ascending
  candidate id of the split pieces; ids are recycled from a LIFO free list, so two split generations come out in
  opposite orders. (2) The initial ids of parameters and locals depend on the order in which unreferenced
  constant candidates are freed, a 64-bucket walk keyed by `value & 0x3f` for integers and `(unit symbol
  ordinal) & 0x3f` for every string literal and global whose address the function uses; the ordinal is the
  front end's running symbol count, so everything declared before the function shifts it: this is what `dcp
  headers sample` perturbs, and why a block-scope extern or an unused local moves such a hunk (period 64).
  (3) So a function is really tie-break sensitive only when it has a forced split with two or more reloads at
  one edge; anything else `headers sample` flags has its cause in the source (main_RoundReport: "sensitive" on
  dozens of differing instructions said nothing; never stop on it). (4) Even then the declaration set may not
  be the answer: all 4096 bucket states give one of two orders and the original has a third, i.e. the source
  still differs in which candidates exist. (5) Base and index of a two-register address are the first and
  second operand of the IL add, fixed before strength reduction; hash state does not move it, the sharing of
  loop index variables between the function's loops does.
- **Which of the two calls of one comparison runs first** is decided by the operands' weights and, on a tie, by
  an internal key that follows where a variable is first referenced in the function: `AIBLD *b = 0;` at the
  declaration (no code) made VC6 call util_Random first; without it gm_BuildingLevel came first. Operand sides
  and `<` / `>` only swap the cmp (main_EndRound).
- **The scheduler** orders a pointer load after every earlier store to an address-taken local whose address
  already escaped to a call; a fresh block local has not escaped yet. Two stores scheduled around a load in an
  order no statement order gives (`mov eax,[esi+1]; mov [msg.arg],eax` before `mov edx,[tbl]; mov
  [msg.to],edx`): every `AIMSG msg` declared in the block that builds it (nine blocks), not once at function
  scope; the frame stays the same (disjoint block locals share the slot). Sweep the block-scope lever
  (blockscope) whenever a store / load order resists (main_EndRound).

### Live range splitting, regions and the priority terms (c2.dll)

Read from color.c of C2 13.00.9044 (addresses at image base 0x10700000), confirmed with `c2trace --prio` /
`--split`. No later pass renames registers: a value in a callee-saved register over a few blocks while its home is
a stack slot is a split piece, and the piece's extent is decided by the rules below.

- **Driver (FUN_1073122d)**: per register class, pops the queue (descending priority, equal: the later-created
  first): savings < 1 -> FUN_10743332 (no register); avail empty -> FUN_107739f7 (split it); else AF7
  (FUN_10708af7, interference and pressure), choice (FUN_10709937), assignment (FUN_10709bb6, which removes the
  register from the neighbours and splits those left without one). After a pressure split the priority pass
  (FUN_10707a0b) runs again and the pieces are re-queued (FUN_107452f4).
- **Priority (FUN_10707a0b)**, per basic block b in layout order, for a candidate live into b or referenced in b:
  referenced: `prio += (sum of its reference costs in b) * P(b) * w(b)`; live through b unreferenced:
  `prio -= P(b) * w(b)`; dead in b: 0. P(b) = distinct allocatable values referenced in b, w = 1 << loop depth, a
  reference costs 2 (FUN_107083c2). Savings (`sav`) sum cost * w only, hence "not linear in savings": a reference in
  a block with many values weighs more, a long-lived parameter pays every block it crosses.
  `c2trace.py SRC FUNC --prio REGEX` prints the term per block with the source line.
  Example ai_DecideUpgradeGuild: o1 +36 for three stores in the credit block (P 6), +56 in the new-type block
  (P 7), -65 over blocks 2..27 (a parameter live from the first test); a second credit copy adds +30: 43 -> 69,
  above money 47 (open).
- **Splitting one candidate (FUN_107453f8 -> FUN_10745670)**: a piece starts at the first live block not yet in a
  piece and grows into successors where the value is live while one register stays free over the whole piece
  (registers of assigned candidates referenced there, a call: callee-saved only); where none is left it is cut
  (FUN_1074527d, `CUT` in --split).
- **Bulk split (FUN_10743bda)**: the assignment (FUN_10709bb6) hands it the neighbours it left without a register.
  It splits in regions only when they are 3+ AND the function has 100+ basic blocks: 0x107ad5b8 counts the basic
  blocks (FUN_10731d1d walks the block list once at the allocator's start; it does not count candidates). Else
  each is split alone (FUN_107453f8). So a function under 100 blocks never gets regions (the count is the IL's
  blocks, `T BLOCKS` in --raw; d3fl_DrawWater 78, map_PickScreen 31: one by one; ai_FillNearRing 184: regions).
  c2trace's summary line and `--split` say whether it ran and why (not). Blocks in layout order, each with
  used = registers of assigned candidates in it; a block joins the region of its FIRST predecessor (in the
  predecessor list's order), then successor, whose region still leaves a register free for both; else it opens a region. Region boundaries cut every candidate of the set. A piece then lives from its
  first to its last reference inside the region, so it spans the region's calls (and needs a callee-saved
  register) only when it is referenced on both sides of them. `--split REGEX` prints each block's predecessors in
  list order and the region it joined.
- **The predecessor order is a source lever**: a test right behind a loop with `break` has the predecessors
  [break, guard skip, latch]; an extra empty exit block in front of the test (a label, the end of a
  `do { .. } while (0)`, a `goto` target) re-links them as [guard skip, break, latch]. The guard skip comes from the
  loop's call block (scratch registers used, edi free), so the test joins that region instead of the loop exit's.
  Example ai_FillNearRing (13 -> byte): g (home [esp+0x18]) in eax at each of `if (g == 3 || g == 6)` and
  `if (g == 4 || g == 7)`; the original loads edi once and keeps it over the up block (reloads at the loop exits).
  G7_ADD as a `do { .. } while (0)` statement macro put both tests into the call-block region (free: edi) and one
  piece of refs 4 spans them. Lever when a value sits in a scratch register at a test after a loop and the
  original holds it in a callee-saved register up to a later test: the statement after the loop as a
  do-while(0) / labelled statement, or the loop's `break` as a `goto` to a label behind it.
- **AF7 avail drop (0x107097cd)**: while a byte-flagged value is live, at a call the candidate lives across, or at
  an instruction where only one byte-capable register is left, `avail &= { ebp esi edi }` (the register set at
  0x107c9624). `RESTRICT` in --split shows the instruction and its source line (ai_FillNearRing: the constants
  and a piece of g restricted at `side[0] = (kind.col == 4) + 4`, the sete cl).
- **Rematerialisation (FUN_107705ea, read, not verified)**: a spilled candidate whose single definition has at
  most one unassigned register operand is recomputed at its uses (the definition copied) instead of reloaded.
- **Not reached**: the x87 keep-vs-spill rule (ai_FireCannon's extra 4-byte temp).

### Callee-saved registers swapped: a priority race lost by a point or two
- **A result kept in esi through a loop nest (split piece, reloaded after a block that borrows esi) where ours
  keeps it in memory and gives a counter the register**: the two bounds-checked indices were named locals
  (`k = ..; if (k < size) { j = ..; if (j < size) ret = k << 16 | j; }`); written inline in the tests and in
  `ret = (..) << 16 | (..)` they become CSEs and the original's allocation follows (map_PickScreen 80 -> byte,
  found by climb round 2: inline:k > inline:j). Same in map_Pick (83 -> byte): the wrapped neighbour indices
  `x1 = (ix + s) & map->mask; y1 = ..` written inline at each use in all three branches.

- **Two out-pointer parameters swapped with the index (a in ebx, i in edi; the original i in ebx, a in edi, a
  `mov bl,[esi+0x130]; cmp al,bl` in the loop where ours has `cmp al,[mem]`)**: the accumulation written in both
  arms (`if (k == sel) *a += X * 0.02f; else *a += X * 0.01f;`) counts *a's loop references twice; one statement
  `*a += k == sel ? X * 0.02f : X * 0.01f;` (the arms' fadd / fstp are one copy in the original anyway). Found by
  flip.py `--want 'mov edi, DWORD PTR _a\$'` hitting a duplicated `if` line (ai_EvalLeastLikedOfEvent 84 -> byte).
- **Two out-pointer sums `*a`, `*b` in one loop: the original keeps a in ebx and reloads b, ours the reverse (or
  act / a swapped)**: the product written in each arm of the ternary, `*a = (k == sel ? w / d1 * m : w / d2 * m) +
  *a;` (same for b), not `(k == sel ? w / d1 : w / d2) * m + *a`; the code is the same shape, only the reference
  counts move. Generate the product of the per-sum forms (ternary of factors / of products / whole if-else /
  ternary of whole sums) x init order: one round of 32. (ai_EvalMoodWeights 54 -> byte; ai_EvalGuiltWeights is
  the w > 0 variant)
- **A copy `n = cnt` placed after the call that uses cnt** (`cnt = f(); if (cnt == 0) return 0; i =
  util_RandomU(cnt); n = cnt;`): the block of `i = ..` then holds one candidate more (P 4 instead of 3), i's
  priority rises 6 and it outranks q; the copy before the test or `n = f(); cnt = n;` left i one point short
  (c2dag --alloc on a scratch copy of the unit: per-block P compared). (ai_ChooseObjectTarget, byte)
- **Callee-saved registers swapped (ebx <-> edi / esi) and no respelling reaches it**: count P (c2prio) and add
  references where they are short. A join statement written in both arms (`s->layers = l;` after the if and in
  the else, tail-merged into the same code) adds references in P=2 blocks (d3sky_CreateLayer: s 10 -> 14, ties
  const#0 at 14 and wins as the later candidate: s edi, const#0 ebx unused, tex ebp; byte).
- **A whole-function reshuffle after the "right" form** (di_UpdateButtons: the index loop gave the original's
  tail but 151 other instructions changed; ours caches the 16-bit global `dm_S.x` in bx where the original keeps
  `xor ebx,ebx` + `cmp reg,ebx`): run a flip search: mutate every statement, keep the mutants where the right
  candidate has the register again AND the wanted shape is there, measure those with `dcp try` (146 of 680
  flipped, four byte). The missing point was a plain `dm_S.x` read where a `volatile` view had been added to
  force reloads. No lever: `(*(volatile short *)&dm_S.x)` at the four press-position stores (252 -> 104 lines,
  not byte). A `volatile` (or any view) added to fix one hunk is a suspect whenever the allocation is off later.
  (di_UpdateButtons, byte)
- **Two global webs in swapped scratch registers through every block** (the hour in eax and sv_SunRaysHour in
  ecx, the original the reverse): the global's store in both arms of the preceding if/else (`if (..) { f(); G =
  H; } else G = H;`, tail-merged): the extra reference re-ranks the webs. No lever: the store after the if, the
  -1 store in both arms of the other if. (sv_WeatherFrame 47 -> 11)
- **A record field re-read instead of the local, also in arithmetic**: `h = (char)(v / 60); T[i].hour = h; m =
  (char)v - T[i].hour * 60; T[i].sec = (m + T[i].hour * 60) * 60;` (two of three uses of h re-read, deep-tools
  subsets.py) moved the minute value to edi and m to bl, ebx saved (sv_FillDayPhases 40 -> byte). One of 19
  `ch->` uses re-read (`((SVCHARREC *)ai_Chars[c])->id` in sv_RemoveGroupMember's call) (sv_MoveToGroup
  88 -> 74).
- **The loop pointer short of about two references** (flip.py): three bits of one word tested as an `||` of
  separate tests, `if ((c->flags & 0x800) || (c->flags & 0x1000) || (c->flags & 0x2000))`, not `(c->flags &
  0x3800) != 0`: VC6 merges them into `mov ax,[esi+0x5a]; test ah,0x38` but takes the reference counts from the
  `||` form; the pointer gets esi, the zero ebp (main_SettleRound 513 -> 397, byte).
- **Several candidates in a fixed order of service** (main_ChooseHistory, byte): ret popped first (avoids the
  reserved ebx, takes ebp), grp second (esi, before the fourth register), again third (ebx). Levers: (a) the
  success written out (`ret = 1; sv_PopRequest = 1;` a second time: one more definition of ret in the loop,
  VC6 cross-jumps the copies); (b) ONE `again = 1` reached by `goto lagain` from the other site (one
  definition fewer; VC6 duplicates the short block after allocation); (c) `ret = 0;` before `again = 0;` at the
  top (the entry's zero register goes to the later of the two); (d) the single-return tail (`if (ret) f |= 8;
  else f = 0; return ret;`: VC6 duplicates the return into both arms BEFORE allocation, so the zero path keeps
  `mov eax,ebp`; an explicit `return ret` in the zero arm folds to `xor eax,eax`). The two gotos stay as
  @original: no goto-free form reached byte.
- **A split piece of a counter in a callee-saved register** (its own `xor ebp,ebp`, reloaded at the latch; the
  original keeps row in memory and gives ebp to the idiv's remainder): `d = line;` BEFORE `first = row % n ==
  0;` at the loop head; c2trace: the piece had savings 4, prio 56, a marginal candidate (d2_ShpBlit16Scaled
  byte).
- **A byte value against a byte-flagged zero for ebx**: the zero (r = g = b = 0 stores, `cmp eax,ebx` after
  mem_Alloc, prio 6) lost ebx to `add` (9) while a `lim` local (prio 35) existed; `0xff - add` written at each
  compare (a CSE temp, prio 25) gives the zero ebx and add bl. `unsigned char r = 0, g = 0, b = 0;` at the
  declaration gives the store order. Open: i (19) against the temp (25) for esi; flip.py says the temp has one
  compare too many (d2_MakeLightTable, shape).
- **Callee-saved set without ebx** (ours: walk pointer `dir` in ebx, copy pointer esi, edi pushed locally around
  a strlen; the original: dir esi, copy edi, no ebx): a second copy of the parameter into the local at the loop
  (`VFSDIR *dir = d; ... for (dir = d; (p = ..) && dir; )`) gives the parameter's candidate the ebx that never
  materialises. No lever: `d` itself (registers right, d loaded late per path), moving the single init
  (vfs_FindDir 38 -> byte, `@original` note).
- **Register roles follow assignment order**: `oldx = w->x; oldy = w->y;` vs the reverse swaps ebx / ebp. Which
  variable is the modified parameter decides roles: `a = idx; idx = base[idx]; shift = a - idx;`.
- **acc / y swapped ebp / edi, an int->float temp in the wrong parameter slot**: the clamps as `x = h * 60 + m;
  x = clamp(x);` (assigned first, clamped in place), the final two clamps in x (b) / y (t) roles. The countdown
  `n = end - day; do .. while (--n)` is a plain `for (; day < end; day++) acc += ..` (same code). An fild temp in
  the home of a variable (a dead parameter's slot shared with acc) is the variable stored for the fild
  (gm_FixedWorkMinutesBetween, gm_WorkMinutesBetween byte).
- **Member access vs the same field through a `(char *)` cast macro** changes register ranks (d3_Unk453110).
  Reloading `k = info->keys` inside each branch fixed a flag test's register and a loop pointer bias.
- **Two parameters with the same use count in edi / ebp the other way round** (r edi, g ebp in the original).
  No lever: declaration order, parameter types, the loop variable, branch polarity (0x479bf0, 12/224 bytes,
  open).
- **Mask loops' spill choice** (dd_SetMode): `dd_RMask = r = ddpf.dwRBitMask;` funnels each mask through eax as
  the original; `dd_RMask = ddpf..; r = dd_RMask;` gives the original's spills (counter in the dead bpp slot, r
  in [ebp+0x20]) but loses the funnel. No lever: loop forms (up / down / `while(bpp--)`), counter variables,
  declaration moves.

### A constant or zero kept in a register

- **`or ebx,-1` + a dword store from ebx and a byte store from bl (`mov [v],bl`)**: one pooled -1 used as int and
  as byte; ours had `unsigned char v` with `v = 0xff` (another constant): `char v; .. v = -1;`
  (ai_ChooseObjectTarget, byte). **Zero in ebx through a record fill where ours has edx**: another scratch
  temp occupies edx in the original: the memcpy size clamp written inside the call (`memcpy(&q[7], ids, len >
  0x20 ? 0x20 : len)`: len in eax, copies in ecx / edx) (ai_PickPartnerRandomWalk, byte).
- **A zero register live through the whole function from `= 0` stores** (`cmp eax,ebx`, ebx pushed at entry):
  several `= 0` stores to a struct, array or members keep 0 in a callee-saved register; `memset(&h, 0,
  sizeof(h))` gives the original's `xor eax,eax` + stores at the site and frees it; `memset(arr, 0, 16); count
  = 0;` gives xor stores for the array and an immediate for the counter (hmt_CreateBinaryFile,
  d2_RestoreShapes, ai_GypsyHelperWindow 119 -> 71). A run of `g = 0` stores to adjacent globals is one
  `memset(&g0, 0, 16)`; cutting chunks out of a standalone copy with `dcp try` finds such causes fast (d2_Open).
  Several `= 0` stores plus `push 0` push a loop counter into memory (f7 0x489cb0, open).
- **Early `return -1`s with own epilogues, the success block last** (with the memset above): failures after the
  early count checks (allocations, the final call `!= 0`) as `goto fail;` to one cleanup tail before the success
  return (hmt_CreateBinaryFile, byte).
- **A zero register shared by two variables** (`xor ebx,ebx` stored to `cur`, then ebx is `best`): `best = 0` in
  an else arm of the `if` that computes it, not before it (before it the zero goes to the loop index)
  (amt_CalcZuenfte).
- **`cmp eax,ebx` (the zero register) where ours has `test eax,eax` after `rc = f(); if (rc == 0) {`**: the
  result is a variable two paths reach, not a forward-substituted temp: the nested ifs were flat, `rc = f(); if
  (rc == 0) { ..; rc = g(); } if (rc == 0) { .. }` (the second test is decided on the first's failure path and
  jumps straight on) (cm_RunDeath, byte).
- **Two strength-reduced pointers initialised in the other order in a loop preheader** (`mov edi,&out; mov esi,&tab[0].f`
  against ours esi first): a named element pointer `c = &tab[i];` used for the test, the store and the call
  argument instead of `tab[i].f` written at each use; flip showed the lever as a duplicated `out[n++] = ..`
  (more weight on the out pointer) (sv_CollectGroupCharacters, byte).
- **A zero register that starts at a vector's fourth store** (`xor ebx,ebx`, three immediate zero stores, `mov
  [w],ebx`, later float zeros / `cmp fired,ebx` from it): that word was an int (`CANNONROT { float x, y, z; int
  w; }`, `pos.w = 0`); once the int const#0 web has a register, the later float zero stores use it too
  (ai_CannonCamera 142 -> 50, open).
- **A one-use local that pushes the constant 0 out of ebx** (immediates and `test` instead of `push ebx` / `cmp
  eax,ebx`, a pointer cached in ebx): `k = (int)(f * 0.2f); ... util_Random(k)`: write the argument inline
  (ahm_CreateAlchemist 104 -> 13). Look for such locals first when the original keeps 0 in a register.
- **Constants 1 / 0 kept in ebx / esi for a whole window pump** (`mov ebx,1` rematerialised, `cmp eax,ebx`, `push
  ebx`) where ours pushes immediates: the order of the function's first stores decides it: `refresh = 1;` before
  `memset(rows..); memset(sel, 0xff, ..); total = 0; tick = 0;` (123 -> 20; one of 120 orders, dcp try +
  deep-tools permute.py). Struct field stores x before y per row (`rows[i].x = ..; rows[i].y = 0x7d;`) gave the
  late constant stores (ai_BloodRevengeWindow byte).
- **A constant cached in a callee-saved register** (`mov ebx,2`, `cmp al,bl`, `push ebx`, byte-flagged): the
  constant assigned in each branch (`n = 2;` per kind arm) instead of once at the top (flip.py --want 'cmp al,
  bl'; ai_GypsyHelperWindow 71 -> 39).
- **A 64-bit constant in two callee-saved registers** (`mov esi,0x807d4190; mov edi,1` at entry, `push edi; push
  esi` at both calls): a union set by halves (`union { __int64 all; struct { unsigned lo; int hi; } h; } mask;
  mask.h.lo = ..; mask.h.hi = ..;`, calls pass `mask.all`). No lever (propagated): a local `__int64`, two int
  locals, a struct of two halves, every constant spelling (ai_DecideBuildingResident byte; he.c's 0xa7f4c3a0
  stuck item may be the same).
- **`(char)0xff` and `-1` are one constant**: `gm_QueryOwner = -1; gm_QueryKind[0] = (char)0xff` made one
  candidate (`or eax,-1`, stores from eax / al) where the original stores immediates; `(unsigned char)` 0xff
  splits them (33 -> 10). Two char flags initialised right before their stores (`scan = 0; gm_QueryScanAll =
  scan;`) stay in their own byte registers; at the top VC6 shares one zero. `va_arg` compiles like the
  hand-walked `ap`. A count-down `for (i = n; i > 0; i--)` moved a pop (gm_Query, 4 ins, open).
- **A table initializer's zero register swallowing a later `sum = 0;`** (`mov [sum],eax`): an aggregate
  initializer shorter than the array zeroes the tail through a register; `float sum = 0;` declared and
  initialised before the table keeps the immediate `mov [sum],0` (0x4f9d40, 40 -> 19). `w[10] = 0;` statements
  store immediates.
- **A constant compare against a register (`cmp [p+0x20a], edx`) where the original has `cmp [p+0x20a],
  0xffffffff`**: the constant written inside an `&&` chain, a constant-web effect (0x5d73e0, 51/80, open).
- **`[ecx+ebx]` vs the original `[ebx+ecx]`**: a dead constant store at the top (`base = 0;`) is a rank tie-break
  that flipped it (d2_Exit). No lever: dead loads, dead stores to other locals (d2_Unk46bf60).

### Named local or expression: a candidate too many or too few

- **Two out-pointer accumulators, the one summed first kept in memory instead of a register** (`*a` reloaded
  `mov eax,[esp+N]; fadd [eax]`, `*b` in ebx, the original the other way round): the sum written in each arm
  (`if (w > 0) *a = (sel ? x : y) + *a; else *a = z + *a;`), not `t = ..` per arm plus one `*a = t + *a;`. Two
  arms give the pointer the references it needs to rank above its twin but below the flags; three arms (or the
  inner if/else written out) rank it above everything and rotate ebx/ebp/edi (ai_EvalGuiltWeights byte; the
  ranking between the two equal-reference pointers follows which sum stands first in the loop body).
- **A byte parameter's widened value in a callee-saved register, the loop counter in a dead parameter slot**
  (`mov ebp,[esp+arg]; and ebp,0xff` before the loop, `cmp ebx,ebp` at its bottom, the countdown k as
  `mov ecx,[slot]; dec ecx; mov [slot],ecx`; ours the other way round): an int local `max = n;` right before
  the loop, `while (cnt < max && k != 0)`. `(int)n` in the condition is no lever (the same CSE temp), and the
  position of `max = n` matters (only directly before the loop). ai_PickActionBuildings byte (plus the twin's
  fill order, from climb `--ops move,swap`).
- **Decide by the original**: a value that sits in ONE callee-saved register through several statements and is
  updated in place was a variable; a value in a leftover register for one expression across one call was a
  temporary.
- **A value kept across the next call as a 16-bit move** (`call util_Random; mov bx,ax; push esi; and
  ebx,0xffff; call gm_BuildingLevel; cmp eax,ebx`): NO local, `if (gm_BuildingLevel(b) <
  util_RandomU(0x5a))`; the temporary gets the leftover ebx, value and weight take edi / ebp. Any named local
  (`lim`, every type and scope) takes edi. The 16-bit move needs the 16-bit prototype (`util_RandomU`);
  `(unsigned short)util_Random()` gives `mov ebx,eax` (main_EndRound byte).
- **A cheap expression kept in a callee-saved register across calls and modified in place** (`mov edi,[i]; shl
  edi,7; push edi` .. four calls .. `add edi,0xc; push edi`): a NAMED local assigned where the original computes
  it (`y = i << 7;` after the d2_SetForm call, `f(8, y, ..)`, `g(0x88, y + 0xc, ..)`); inline twice VC6
  recomputes and the whole allocation permutes; one statement earlier: 201 ins. Look for this FIRST when
  "everything is split": a register of the original that holds a value ours never names
  (main_SearchNetworkGame 348 -> byte, one local = 330 ins).
- **Parameters in callee-saved registers in ours, scratch in the original, no call needing it**: named pointer
  locals the original did not have. `f0 = &data->frames[s->frame]; f1 = ..;` (prio 144 / 126) took eax / ecx;
  `data->frames[s->frame].move.x` at each use (d3anim_RootPosAt 144 -> byte).
- **A cache local pushing its source into the wrong register**: `half = c >> 1` used three times: write it out
  at each use (qnt_MakePalette). One use of a pointer local spelled as the index (`mark[b]` -> `used[nedge +
  b]`) changes the weights of the address operands (d3_Unk454890). An added local can also be right (`sc =
  d3_DefaultScene` hoisted the global load before the test, d3sndw_Update; `D3ANIMDATA *d = s->data;`).
- **Two accumulators from the same value were not a copy**: `cash = wage; income = cash - spent; .. cash =
  income;` -> `cash = c->wage - c->spent; income = c->wage - c->spent;` (cash first; the original's `mov
  ecx,edx; sub ecx,edi; mov edi,ecx; mov [slot],ecx`, one temp fanned out). A copy `b = a` between long-lived
  accumulators changes the whole allocation and the frame; named temps for the operands leave 8 ins
  (main_RoundReport 159 -> 40). A sum used twice through one temp: `t = a + b; net += t; income += t;`.
- **A loop-invariant call argument held in a local swaps two callee-saved webs** (`flags = id | 0x80; do {...}
  while (sv_RunFrame(fn, flags));` gave `or ebp, 0x80`): inline `sv_RunFrame(fn, id | 0x80)` gives `mov ebx,
  esi; or bl, 0x80` (ai_MissionResultDialog, ai_MissionPartnerDialog byte; ai_MissionCompleteDialog shape, then
  byte from the zero-init order). `grep "flags = id | 0x80"` in sibling dialogs.
- **`flags & N` values in dead parameter / asm-pointer slots, pointer parameters in esi / ebx**: the original
  repeats the test (`if (flags & 2) af...; ... if (flags & 2) anim->...`). No lever: named locals (`if (loop =
  flags & 2)`) took esi / ebx.
- **A flag in a named local assigned inside an `||` condition gets a stack store**: re-evaluate the expression
  in the other branch, VC6 CSEs it into the register; with the flat `else if` chain instead of a nested else +
  return (surf_Blit, identical).
- **Tail index**: `len = n - i; memcpy(p, s + i, len + 1); p[len + 1] = 0;` gives `sub edi,ebx` + move to eax;
  `memcpy(p, s + i, n - i + 1); p[n - i + 1] = 0;` keeps n in edi, n - i in eax (io_text.c 0x514360 byte).
- **Named locals for intermediates take a callee-saved register and spill a loop counter**: write the
  expression inline; reload `a->frame` instead of caching it; test `arr[idx].id != 0xffff` before `c =
  &arr[idx]` (47e560). A size local computed after the block of stores, or inline, matched; `if (y + o.h >
  w->m) w->m = y + o.h;` with no temp matched. `now - s->fadestart` written twice instead of an `unsigned int`
  el used twice moved sb_FadeTime to the original register. No lever: separate `w = h = 1 << i` variables
  (copy-propagated, worse), `volatile int n` (d3t_CreateSnowMipmaps).
- **Subtraction shape**: `v - lim` used directly in a test and a store (CSE) gives `mov ecx,eax; sub ecx,edx`; a
  separate `d = v - lim` gives `sub eax,ecx; mov ecx,eax`. A named local for `&ai_Chars[x]` can change the
  whole function's registers (hv4 0x497af0). `d = (rnd & 0xffff) + 2; f(c, -d);` gives `add eax,2; neg eax`
  (inline: `mov r,-2; sub`); NO local for a value used once (`skill`) (main_SettleRound).
- **`fl |= 0x40; store`** reuses the register for a later `test cl,2`; `store (fl | 0x40)` gives `mov eax,ecx;
  or al,0x40`. `volatile` on a byte struct read gave `lea eax,[..]; mov al,[eax]` (f6 0x489660).
- **A fallback value in eax through both tests, moved to a callee-saved register only at the next call**
  (`mov eax,[A]; test; jne L; mov eax,[B]; test; je ret; L: .. mov edi,eax`): `if (A) b = A; else if (B) b =
  B; else return;` (or a both-zero test up front). No lever (load edi directly): `b = A; if (!b) { b = B; if
  (!b) return; }`, `if (A) b = A; else b = B; if (!b) return;`, any copy through a second local. A search hit
  left by `goto found` onto a shared fallback test is a second local (`obj = it; goto found;`, stub `mov
  eax,esi; jmp`) (sv_DrawGateName 81 -> byte).
- **A pointer kept in its own register after `e = d->entries = mem_Alloc(..)`** (the original `mov ecx, eax`,
  stores through ecx): `d->entries = mem_Alloc(..); s = (char *)(d->entries + 1); e = d->entries;` (e last).
  No lever: `e = ..; d->entries = e;` (vfs_AddFile 29 -> byte).
- **Copy-propagated pointer loads through the wrong register** (`rec = p; while (rec->next)` loads via p): read
  the source expression twice (`if (head) { rec = head; ...`); the CSE'd copy uses the new register.
- **A value kept in a register although an earlier call wrote it** (the capacity read from the record
  ord_GetKindRecord filled, in ebx across the loop's calls): copy it into a scalar (`cap = rec[0]` after the
  query, `cap = kind[0]` at each pick); the array element itself stays in memory whatever the spelling (struct /
  short[4] / struct copy) (gm_FindHome 90 -> 67, open on the remaining rotation).
- **A local assigned at the top "as a default" and overwritten in one arm** (`exs = 0;` early, `exs =
  WS_EX_TOPMOST` in one arm; ours spilled exs, frame 0xcc vs 0xc8): the default was the else arm's statement
  (`} else { style = 0x6000000; exs = 0; }`), exs takes ebx, which also serves as the zero at the top. With
  `if (mode != 3) {..} else ..` (long arm first) and `nw` before `nh` (main_CreateWindow 105 -> byte).
- **A sibling walk keeping the current object in the register being dereferenced** (`mov edi,eax; mov
  eax,[edi+0x1f4]`): it came from a small __inline helper; written inline VC6 dereferences the copy
  (d3t_DropObject).
- **A branch ending in a plain `mov ecx,eax`**: a separate variable assigned in each branch. A masked copy of a
  coordinate is a new variable (`ix = x & ~(s - 1)`). `p = buf + wsprintf(buf, ..)` keeps p in eax.
- **A struct chain reloaded macro-style** (the entry base stays in eax): `((AHMCHAR *)c)->obj->rec->slots[i]` in
  both the test and the return; a `p = ...->slots;` local hoisted the base into ebp.
- **A value live across a call costing a 4th callee-saved save** (`owner = rec->object;` held to the
  d3_PointToWorld call; the original reloads it into eax after the call, rec in ebx). No lever: `rec->object`
  read twice (the first callee-saved register goes to scene; 30 vs 86 hunks) (0x58fc40, open).
- **A short global in several comparisons of one `if` and after a call** (the original loads it into di / bx
  before the first comparison, `movsx`, reloads after di_ClearEvents): the globals written directly give the
  cache for one (bx) only; `short` locals give both early loads but no reload (0x477000, open).

### Pointer locals and addresses

- **A dead `lea eax,[edi+eax*4]` right after a load `[edi+eax*4+0x22f]` (eax overwritten by the next call)**: a
  named record pointer `t = &ai_BTypesT[b->type]; f(.., t->title, ..)`: VC6 folds t into the load and still
  materialises it (he_AllocGeselle; the plain field access or a typed cast leaves no lea).

- **A record address materialised (`mov eax,edi; add eax,ebx; mov al,[eax]` / `lea eax,[eax+edx*4]; mov
  al,[eax]`) where ours folds `[edi+ebx]`**: a pointer local read at every test (`g = &T[i]; if (g->type == A ||
  g->type == B ..)`, `g = GM_GOOD(id);`); a value local `type = ..->type` or the element repeated folds the
  address (ai_HandleItemDrag, gm_BuildingGoodsList, gm_ChangeBuildingState byte; gm_ListObjectsAfter). In
  gm_ChangeBuildingState the pointer also fixed a movsx load order; the last jge vs jae was GMBTYPE.kind's
  signedness (unsigned char).
- **Two fields of one `d2_Wins[idx]` pushed to a call** (`lea eax,[ecx*4+d2_Wins]; mov ax,[eax+4]` beside `mov
  edx,[ecx*4+d2_Wins+0x26c]`, ours one `shl eax,2` temp for both): a pointer local for BOTH fields, `w =
  &d2_Wins[d2_LastWindow]; d2_MoveObj(w->x, y, w->parent);`. No lever: the pointer for x only,
  `(&d2_Wins[..])->x` inline, an index local, a short local for x, a local for parent (ai_CannonBuildPanel,
  sv_UpdateCityPanel byte).
- **`lea edx,[eax+eax*8]; lea eax,[edx*4+X]; test eax,eax`** (an element address in its own temp, tested for
  null) where ours folds into `lea eax,[eax+eax*8]; lea eax,[eax*4+X]`: a pointer local assigned in the
  condition, `(ch = &ai_CharsT[b->owner]) != 0 && ai_CanAct(ch)`, not the address written twice (main_EndRound).
- **`add r,edx; .. mov esi,r` where the original has one `lea esi,[ecx+edx]`** (the local's value also stored
  to a field; VC6 forwards the sum through the index temp's web, `c2dag.py --alloc`: `use def use def use`):
  write the expression out at the store too, `tex = base + idx; tri->tex = base + idx;`. No lever: the other
  order, `tri->tex = tex = ..`, `tex = tri->tex`, every spelling of the sum; flip.py never duplicates an
  expression into a neighbouring statement (d3io_AttachStock 3 -> byte).
- **`[esp+reg+const]` for several fields of one stack record, the scaled index in a callee-saved register**:
  `arr[idx].field` at every use; a `p = &arr[idx]` local gives `[reg+off]` (main_SearchNetworkGame).
- **Global array element vs pointer local**: `arr[i].field` keeps i * sizeof in one register and folds the field
  into the absolute address (`[esi+0xf88a80]`); a pointer local `e = &arr[i]` gives `[esi+0x40]`. The pointer is
  materialised as soon as it is passed to a callee, so a source passing `&arr[i]` AND reading through the
  local can give either; the original uses the offset form for the fields and the pointer form for the
  arguments (ai.c 0x483fc0 / 0x484a60). `g = &ai_GoodsT[*it]; .. g->name` gave `add eax,ecx; inc eax`,
  `ai_GoodsT[*it].name` one `lea` (sv_DrawGateName). Two pointer locals where the original keeps
  `&ai_CharsT[w]` in a register (main_SettleRound).
- **An IV-offset sum as the address**: `*(int *)(i + 0x34 + base)` matches `[ebx+ecx*1+0x34]`; a local pointer or
  a typed field flips the sib base / index pair (d2_Exit).
- **A far draft through a pointer view**: `((int *)ww)[0xe2] = x` matched; `d2_Wins[d2_Views[view].wins[i]].bgpic
  = x` (same address) made it worse (1651 -> 1659 bytes); the pointer's typed member (`ww->pic`, the D2WINF view
  of 0x388) is byte-neutral (d2_OpenForm).
- **The record pointer in a scratch `lea`**: copy a 12-byte vector out of the record field by field; it moved
  `shot` into ebx pushed late (gv_ExplodeShot, regs 30 -> 14).
- **Pointer CSE in edi / ebx**: plain `m->_31` member access; pointer locals made it worse. An index form
  `bytes[i * 3]` / `f->normals[i*3+k]` reproduced loop registers that a separate counter or a hoisted `p =
  normals + i*3` did not. Indexing `b[i].x` bases the pointer at the middle field; walking `b++` at the last.

### The eax / ecx / edx rotation out of step

- **A switch whose case blocks' scratch registers skip a step of the rotation** (each block loads two pointer
  parameters `mov ecx,[a]; mov edx,[b]`): at each skip the source had an extra case body that VC6 cross-jumped
  away after allocation (groups 0xe .. 0x10 and 1 / 3 with the default's `*a = 1; *b = 0;`, own bodies in
  building-type order); equal bodies are not neutral. Return vs break per case: one code (gm_JobSlots byte).
- **A dead `lea` of an address**: a store written twice (flip.py dup hit; gm_EndDayIncomes).
- **The rotation two steps behind after an index chain, so two cross-jumped call tails merge in ours** (the
  original's next block starts at eax, ours at ecx; its `xor edx,edx; mov dx,[esi]` id push differs from the
  other arm's eax copy and stays apart): the record pointer was a named local, `c = (AIPER *)&ai_CharsT[i];
  g = gm_Mix(p, c, 4);` not the address inline (same code at the chain, two silent rotation steps after it).
  Found by flip.py `--want` with a `[\s\S]*` pattern spanning two calls: the hit was a DUPLICATED gm_Mix line
  (ai_DecideThreaten byte; also the sibling form `r = call; if (r) {copy} return r;` per arm instead of `goto`).
- **A global carried in a register into a later arm** (ours reloads `mov ecx,[depth]` on one edge of a join;
  the original loads it inside the arm into eax): `if (top == cb) r = 1; else if (pop != 0) r = 0; else { r =
  1; for (..) if (..) { err(..); r = 0; break; } if (r) {push; goto l64;} }` instead of one else with `goto
  l21` for both r = 0 paths (sv_RunFrame 10 -> byte).
- **The argument-load registers decided by the store order of two record fields**: `type` before `owner` in
  BOTH copies of the three statements (mode-5 block and free-slot loop) (ai_NewMission 0x4b3ca0, 10 -> 0).
- **A rotation over ~1 KB**: two field reads from call results through one pointer local (`part = f(p, 4); fy =
  part->h;`) (d2_Unk46d3d0). A three-register rotation: repeated `int i = 0;` + `for (i = 0; ...)`; `for (;
  ...)` did not.
- **Loop-body accumulators and stores**: several `acc += arr[k]` lines in array-index order (1 of 24 orders is
  byte; it decides which accumulator gets the free callee-saved register); `cash -= x; costs = x;` (not `costs
  = x; cash = income - costs;`); `(o->interest * 0.01) * o->amount` (unparenthesised gives `fild; fimul; fmul`,
  the original's `fild; fmul const; fimul` needs the parentheses; a float / double local no lever); plain store
  order (main_RoundReport byte). Message store orders (`type, to, fromId, ..`) (main_SettleRound).
- **A load before a test where ours loads first** (`mov al,[type]; cmp al,0x1f; mov eax,counts[i]`): no `c =
  counts[i]` copy; read counts[i] at each use (ai_GypsyHelperWindow 39 -> byte).
- **`d2_ClickedId = -1;`** after the last d2_Inputs field store, not between them (lobby_Screen byte).

### Loops: counters, running values, loop pointers

- **A loop pointer that should take a dead register**: `for (t = s->tris; ..)` in each loop's init, not `t =
  s->tris;` once before `if (hw) for (..) else for (..)` (d3_DrawObjectCB 122 -> byte, the M8 load-order
  crossing went with it).
- **One counter for two loops** (sel kept in i's register): reuse `i` for the second loop and write `sel != i`;
  gives the original's `mov edi, ecx` (lobby_Screen 66 -> shape 2).
- **A row position in a register only after the n test (`mov ebp,eax` after `test ebp,ebp; jle`)**: `top + i *
  h` in the call, not a `y` advanced in the for increment; VC6 strength-reduces to the same `add ebp,ebx` with
  the original's webs (main_ChooseOpponents 22 -> byte; `while (sv_RunFrame(..))` = `if (..) do .. while
  (..)`). The same with `y = i * 0x64 + 0x31;` at the top of the body against `y += 0x64`; every use as `i *
  0x64 + k` was worse (ai_AlchemistCandidates 260 -> 221, open).
- **A local kept in its slot but copied into a callee-saved register for a loop** (`bar = -1` at entry, then
  `if (timed) esi = slider(); else mov esi,[bar]`; ours keeps bar in the slot or `or esi,-1`): a plain `while
  (f()) {..}` with the flag tests in the body (`if (flags & 0x40)`, `(flags & 0x80) && bar != -1`); VC6 hoists
  the tests and caches bar. No lever: `if (f()) { q = flags & 0x40; do {..} while (f()); }`, copy locals, `int
  bar[1]`, `?:` (sv_MessageBox 86 -> 65, shape; the same reload into edi at the join open in
  sv_SendSelectedToBuilding).
- **`xor r,r` before the `and` whose flags feed the loop-entry `jle`**: the bound read in the for condition (`k
  < hdr.players`), or `k = 0` before the bound local's assignment; a bound local first gives `mov r,0` after the
  `and` (main_SearchNetworkGame).
- **A search loop + `if (i < 5) {..}` whose original jumps straight into the body**: the body inside the loop
  with `break`. `for (k = 0, idp = cfg.ids; ..)` put `xor edx,edx` before the `rep stosd`. An `else if (clicked
  == -1) {timer}` chain whose original tests `!= -1` first: `else if (clicked != -1) { buttons } else if (timer)
  ..`. A value kept in ebp across calls (`mov ebp,eax` after reading rows[0].rec): a block local `AICHARVIEW *av
  = rows[0].rec;` (-90) (ai_BloodRevengeWindow byte).
- **A range test `x >= 0 && x <= 9` splitting x into a memory home** (`mov [esp+N],edi / mov edi,[esp+N]`):
  nested ifs, each with its own `x = -1` else arm (txt_Format 703 -> 391; io_PrintText's identical parse: no
  lever).
- **A count-down loop's entry test (`mov esi,eax; test eax,eax` vs our `test esi,esi`)**: `for (i =
  qnt_HeapCount; i > 0; i--)`, `while (i > 0) { ...; i--; }`, do-while, a fresh `k`: all the same 10-ins diff
  (canonicalised before allocation) (qnt_Build 0x420b72, open).
- **A byte local in a byte register** (`mov dl,[esi+0xb5]; cmp dl,5`; ours gives it bl and spills an int that
  wanted ebx): widening to int / unsigned short fixes the frame, not the byte compares; the table pointer (esi
  in the original, edx in ours) decides which byte register is left (ai_PokerPickStrategies, open).
- **An `unsigned char lv` spilled to a 4-byte slot** when cl is needed for a constant (`cmp cl,6`): read the
  field twice (`c[0xd]`) for the single `mov cl,[...]` and the smaller frame (ord.c 0x4f4f20).

### Frames and stack slots

- **Block locals share slots**: locals of different sizes do; a worker list and a message as block locals share
  one; four fields copied and modified as ONE block-local struct (`AIWORKSET ws; ws = m->work; ws.flags |= 1;`,
  not `int ev[4]` field by field); hand-advanced `pn` / `sub` do-while loops are plain `for (pass..) for (j..)
  workers[1 + pass * 12 + j]` (main_SettleRound byte). A 16-byte local whose lifetime avoids another's packs
  into its slot (D3VECTOR4 d with ratio and the recoil's qword temps at 0x14) (ai_CannonCamera).
- **A copy before a call kept in its slot**: `d.z = sc->dirZ` before a call stays in the slot; after the call
  VC6 forwards it into the fld (ai_CannonCamera). Open there: two 16-byte locals in the original's order when
  the less-referenced one must be nearer esp.
- **Window ids in slots spaced like one array**: form / win2 / win3 / group at w[0] / w[2] / w[4] / w[7] of an
  `int w[8]` give the frame exactly; constant-index elements keep their slots (ai_AlchemistCandidates).
- **One variable reused for an ftol result and a later delta** shrinks the frame by 4. Two locals sharing a
  dead parameter slot are one source variable.
- **`int btns[3]` declared in two blocks** (the frame loop read its own uninitialised copy; VC6 happened to share
  the slot): one array (ai_GypsyHelperWindow).

## Stack frame and slots
Frame size, slot order, holes, shared slots, dead parameter slots, block-scoped and address-taken locals.

### How the frame is laid out

- **The original's frame larger by 4 / 12 bytes with an untouched hole behind the used fields (`frame 0x8 vs
  0x14`, `dcp frame` holes)**: the local is the callee's whole record, only partly filled (`int req[5]` for the
  20-byte request of amt_ExecuteMayorPowerMessage: ai_GuildLevel3RequestCB and its two twins). A 4-byte hole
  BETWEEN two 12-byte copies with a frame that two 16-byte vectors overshoot by 4 = ONE local of 28 bytes (`struct
  { D3VECTOR4 pos; D3VECTOR rot; }`): two separate arrays sort the 12-byte one first and a 16 + 16 pair adds the
  trailing pad (ai_EnterOwnBuilding / ai_EnterBuilding, SVSAVEDVIEW). One local of ours paired with two original
  slots (`<- several`) in two blocks of address-taken out-parameters = two function-scope locals (cm_CheckSellObject:
  the buyer's building `bb` apart from the seller's `b`).

- **Reference count of a local (read from c2.dll)**: one per operand naming the local, all blocks, no loop weight,
  taken after register allocation and instruction selection but BEFORE the late passes - a statement written in
  both arms of an if (tail-merged later) counts twice, a reload the late peephole removes counts, so the listing's
  count can be lower. A dead store to a used local still counts (ai_HandleItemDrag, c2frame.py); dead or folded
  uses (`if (0)`, `* 1.0f`) do not.
- **Listing, packing, sorting**: the locals are listed by size ascending, then count descending (tie: who reached
  the count first in the walk); each joins the first lifetime-compatible slot or opens one; frames above 0x80
  bytes are sorted by weight * 1000 / size, descending, with an unstable quicksort; the densest slot is nearest
  esp. Declaration order, type, scope and names never enter (main_ChooseCity: 5 such variants = one code;
  init_CreateStartCharacters; deep-tools frameorder.py).
- **Block locals share slots, function-scope arrays never do**: block locals with disjoint scopes merge into one
  slot even when their addresses go to calls. The merged slot keeps its weight but enters the table at the
  position of one symbol's count (main_ChooseCity: two block `char key[0x100]`, 3 references each, entered behind
  objName and cityobj; the unstable sort then resolved their tie the other way).
- **A frame-order hunk is a COUNT question**: read the counts, work out which local needs one more or one fewer
  (simulate: scratch qsim.py pattern - the sort is 12 lines), then find the ordinary source that has that
  reference at layout time. Between two equal-density slots a third cause: a slot at another table position.
- **Block-used buffers and records belong in their blocks**: declare them there BEFORE any other lever on a
  store-order or frame-order hunk. The frame size does not tell (VC6 shares the slots); the scheduler (escaped
  address) and the frame sort (table position) do (main_ChooseCity byte, no count changed; main_EndRound's AIMSG
  msg; pos / rot).
- **Not allocated**: an unused local, a dead local assigned in the code (any type), a dead store to a dummy array
  or a separate `float y`. A slot-only difference needs a real reference or a different object size (bmp_Load).
- **Two slots of equal density swapped in a frame above 0x80 bytes**: the sort is K&R quicksort (the middle
  element swapped to the left end, Lomuto partition on `dens > pivot`, recursing on both halves) over the table
  after the leading parameter slots; it reproduces c2frame's B -> A of all 75 sorted frames sampled from 13
  units (a CRT qsort, Hoare and plain Lomuto do not). So simulate the tie: the table order before the sort
  (list order: size, count, then who REACHED the count first in the walk - moving the last reference of one
  local flips it) or one count more or less decides it. Every inline-asm block counts 2 references for every
  escaped local in scope; a load hoisted out of both arms of an if/else counts twice; a truly dead store
  (before an overwrite, at the end, before a return) is removed before counting, one before a call is kept as
  code. No lever: dead stores, re-tests, `(void)x`, `x = x`, the asm macros' temp declaration order or names
  (d3d_LoadTexture: texture / loadt 34 / 34, open).
- **Two equal records (AIARG s1 / s2) swapped, equal counts, the walk ending in `*o1 = s1; *o2 = s2;`**: a
  function-scope pair always puts the first-copied record nearest esp (it reaches the count first). The original
  had one block-scoped pair per path (`{ AIARG s1, s2; ... }` in the sub-action-2 arm and in the usable-good
  arm): the merged slots enter the table differently (ai_DecideUseSafetyObject 59 -> 22, then byte with the
  Dynasty tail `if (r != 0) { *o1 = s1; *o2 = s2; } return r;` at both call sites, which also moved the final
  `xor eax,eax` before the pops). Declaration order, names, goto vs flat tails were no lever.

### Frame bigger or smaller: vectors and padded arrays

- **A 4/12/16-byte gap, or 4-byte holes after groups of three float slots**: `D3VECTOR4` locals (w never
  touched), also for temporaries, declared in the blocks that use them; VC6 keeps some fields on the FPU, others
  in their slot (a scalar inside an accumulator's slot was its `.y`; a 16-byte slot with only x and z of a sum).
  Build in place (`a.x = d.x + w.x; .. a.x *= k; a.y *= k; a.z *= k; a.y -= fall;`, `v = b + a; v *= 0.5f`),
  copy field by field (ai_StepShot / ai_StepShotSlow 308 -> 28 ins, frame 0x5c -> 0x98; d3anim_StepObject 0x68 ->
  0x9c, stuck; ai_CannonSwitchAnim `D3VECTOR4 v, w`; ai_CannonExplosion six D3VECTOR4s, x87 allocation left,
  stuck; d3t_DropObject gap at the frame top; 12-byte out-param locals 0x4d02d0, 0x4d0a80, 0x4d25a0).
- **A frame 4 bytes short, a colour copied by integer moves, only .y/.z in memory**: a D3VECTOR4 filled field by
  field, not a D3VECTOR struct assignment (`D3VECTOR4 u, v` for the face normal).
- **12-16 bytes larger with rounded float temps in memory**: a `D3VECTOR4` local (`volatile float` reuses dead
  parameter slots instead); a lerp with the third difference spilled: a D3VECTOR4 difference local. But a
  12-byte D3VECTOR argument fixed one frame, and one D3VECTOR4 reused for two groups of values explained another.
- **After turning a vector into D3VECTOR4**: check every `sizeof` of it (memcpy into a 12-byte field).
- **16 bytes larger, one used dword at +4 of the extra space** (dcp frame: holes either side, ours shares that
  value's slot): y of its own padded vector, `float up[4]; up[1] = pos[1] - cen[1]; .. + up[1]`, view stores in
  rotation-then-position order (720-order permute) (sv_FlyCameraToObject byte). Transfers: sv_CameraToObject
  (0xc short) took `up[1]`, the datum cm_EffectRadiusScale for its M10 literal 1.8, stores in rot 0,1,2 / pos
  0,2,1 order (40 ins -> byte).
- **Constant-index arrays with holes**: elements are enregistered, the array keeps its full slot. `int x[3],
  z[3]` (d3t_GetTriRange 0x20); `int n[4]` where three scalars gave `push ecx` (gv_GrowGraveyard 60 -> 8 ins);
  `int w[4], d[4]`; `float pos[4]` with only [0] / [2] set (separate `float x, z` let `best` take the dead
  parameter slot, `float x[3]` put the holes elsewhere; 0x58fe10). Four scalar floats instead of `float[4]` moved
  a 16-byte block.
- **Three 16-byte locals in another order, equal code**: two extra real reads of one move them. One `D3VECTOR4
  p[3]` gives ascending order (88 -> 50) but is address-taken, which reorders a copy's loads into it. No lever:
  declaration order, names, block scope, pointer views, struct copies (ch_FindFreeCell, open).

### Frame bigger or smaller: buffers, records, temps

- **Frame 8 short, the high half of a 64-bit kind bit (`1 << (kind - 0x20)`) stored to its own slot and never
  read, the low half in a register; ours packs `hi32` into a dead parameter slot**: one `LARGE_INTEGER k`
  (LowPart / HighPart) instead of two unsigned locals: VC6 keeps the 8-byte local in the frame, promotes LowPart
  and stores HighPart (ai_DecidePriceManipulation byte, with the `goto set` dropped: the d = 1 arm plain inside
  its if, the `d != -1 && d != 1` test after it decided on that path).
- **Unreferenced 4-byte holes behind pairs of scalars (`cntA, cntB, hole, recA, recB, hole`)**: each pair was a
  3-element array indexed by a type - base (`int cnt[3]; AIBLD *best[3];`, [2] for the skipped type), read with
  constant indices so the code keeps its branches; the same goes for one-based tables sized by their data
  (`int own[17]` for a 0x11-byte `jobByGroup`). Then two AIARG fills that only the loop and the tail use were
  block locals: `AIARG s1, s2;` inside the loop's `if` and again in a `{ }` around the tail - that put o in the
  dead parameter slot, money in a local, and the `s1.kind` store behind the compare (ai_DecideMasterBuilder,
  ai_DecideChangeProfession byte).
- **A far draft's frame size off by 4**: one missing local shifts every slot (d2_OpenForm 0x4741f0: `sub
  esp,0x244` vs 0x248, 988 hunks; Ghidra's local_248 gives the size, not the variable). Fix the size first.
- **N untouched bytes right behind a small local whose address goes to a call**: the local is the callee's full
  record - check how many bytes that callee reads. cm_SendExSysMessage copies 0x7c: `struct { AITIME time; char
  pad[0x6e]; } clk;` (main_GameLoop 0x450 -> 0x4bc, 41 -> 9 ins; like CMQUICKSAVE, main_EndRound's sysmsg). A
  union of unrelated buffers and separate block locals are the same code: prefer the block locals.
- **Record sizes**: AMTMSG reserves 0x10 for 11 bytes because the 15-byte copy (G17B15) reads 4 more: `char
  pad[4]` (0x4e6680 byte); AMTREC is 0x18, `char rec[0x18]` not 0x20 (0x50e5a0, 75 -> 37 lines).
- **Two adjacent byte locals next to a big array, a 12-byte gap after** (`[ebp-0x28]`/`[ebp-0x27]`): one record
  - GMDATE `char day, month; short year` (the year's `movsx`), then `mode` as an int (hst_ParseDate byte;
  framesearch.py with `struct {..} dt` hypotheses).
- **The real buffer size is in the frame**: `mov eax,0x1008; call __chkstk` = a 0x1000 buffer, not `tmp[0x400]`
  (0x520000). 0x4c2f30's 0xbc = x + `unsigned out[0x2e]` though callees use out[0]/out[1].
- **One small buffer too many**: look where the original's buffer addresses point. err_ExceptionFilter's code
  window (`lea ebx,[esp+0x90]`, inc ebx per shorter read, `err_CheckCall(text + 7, ..)`) is `text`, not a `char
  win[7]`: frame 0x280. Registers then still follow the declaration set (2 codegens in 32 samples; stuck).
- **Char array gaps**: char[0x11..0x14] rounds up to an 8-byte multiple (snd_FindVoiceSample: sfx[0x10] not
  [0x14]; framesearch.py over sizes). `char buf[1028]` costs 8 B more than `buf[1024]`; Ghidra's size includes a
  saved slot.
- **A byte store then a dword reload of one slot** (`mov BYTE PTR [esp+0x1c],al; ... mov ecx,DWORD PTR
  [esp+0x1c]`): a `char` local kept in memory, `char col` not `int col`; its slot moves the buffers (0x51e900).
- **4 bytes too big from an FP temp**: `r > gm_Liking(..)` spills r (`fstp [esp+0x20]`); `gm_Liking(..) < r`
  `fild`s it after and `fcompp`s (0x4ba0c0 in unk_4b6480.c, 126 -> 10 hunks).
- **An extra 4-byte local from a cast used in both arms**: `v = (int)(gm_GetGoodsValue(c) * f)` once, v in both
  arms (0x4b0360 byte).
- **Address-taken float** (`ai_Table[i].a = *(int *)&r;`, frame +4): a union `{ float f; int i; }` set as `.f`,
  read as `.i`, stores `fstp [base+edi+4]` (ahm_LoadIni 0x304).
- **50 ints indexed `t[lvl + 0x1e]` / `t[lvl + 0x28]`**: three arrays `t[30]`, `lv[10]`, `money[10]`
  (init_CreateStartCharacters, -28 ins).
- **Three `unsigned char` colour slots**: `unsigned char rgb[3]`. An array a callee fills: `CHARACTER
  *others[32]` (ch_FindNear).
- **Out-params scoped into their loop**: 0x5206b0's frame 0x14 -> 0xc.

### Frame temps of AI_ZERO / D3_ZERO / inline-asm macros

- **AI_ZERO through a slot** (`mov ecx, [ebp+0x10] / mov [ebp-0xc], ecx / mov edi, [ebp-0xc]`):
  `AI_ZERO_AT(param, size)`; AI_ZERO's `void *_zp = (dst)` takes a slot (amt_GetOfficeCandidates / ...Full).
- **An asm clear whose `mov edi` / `mov ebx` read two locals' slots directly** (no `_zp` / `_zn`, 4 bytes
  smaller): name both in the asm (`mov edi, n2` / `mov ebx, n1`) (ai_TokenMoney byte).
- **`ai_SplitTokenFormat(fmt, buf, &n1, &n2)`, n2 in the dead `fmt` slot** (`lea ecx,[ebp+0x14]` 4th push, frame
  0x208): n2 never initialised; `n2 = 0x200;` forces it into the frame (0x20c/0x210).
- **A macro temp loaded from a parameter and stored back** (`mov ecx,[name]; mov [name],ecx` vs `mov
  edi,[ebp+0x14]`): `__asm mov edi, dst`, no `void *_cp = dst`; a global `__asm mov edi, OFFSET dst` (hst.c
  G12_COPY2_V / G12_COPY2_G).
- **Permuted ebp slots, equal instructions**: the density sort (c2frame.py B rows); the D3_ZERO temp stores
  (`mov [ebp-0x20],0x80` = _zn, `mov [ebp-0x24],eax` = _zp) show which local each was packed with
  (d2_DrawWindowText, stuck). Block locals take the slots nearest ebp and share the D3_ZERO temps.
- **Inline-asm helper spill slots**: `(float f, int *dst)` gave the original's, `(int *, float)` swapped two.
  Declaration order of macro temps decides store order.
- **Big arrays and records far from their original slots in a frame with D3_ZERO / D3_COPY3 blocks (num and
  ddscaps2 nearest ebp, namebuf above pal and adw)**: every inline-asm block counts 2 references for every
  escaped local IN SCOPE at that block (c2frame `R` rows), so a function-scope array gets +16 from 8 macros
  whatever its own use. A block `{ char namebuf[512]; char num[10]; DDSCAPS2 ddscaps2; ... }` that opens after
  the last asm block but one takes those counts away; an error block's `char msg[512]` nested INSIDE that block
  keeps its own slot (two disjoint blocks share one); an array's declared size sets its density (`num[10]` not
  `[12]`: 4 refs / 10 between the neighbours). No lever: declaration order, block scope of the other records.
  (d3d_LoadTexture 127 -> 77 ins, then a 34/34 reference tie left between two pointer locals)

- **A loop counter in a slot shared with an `&iv` FtoI target, `[base+edi*4+off]` indexing with no strength-reduced
  pointer**: the counter IS the escaped local (`for (iv = 0; iv < sizeof(step) / sizeof(step[0]); iv++)`, the
  unsigned compare `jb` from sizeof); a plain `l` gets the array pointer strength-reduced. Declared in a block that
  opens after D3_ZERO so its asm does not add 2 refs (d3fl_MakeLight). Also there: row bases `up`/`down`/`row`
  were no locals (`h[((y + 1) & (size - 1)) * size + x]`, `lp[y * size + x]`; VC6 hoists / strength-reduces them).

### Slot order swapped, frame size equal

- **Two equal-size locals swapped**: add a reference with no code - a statement in both arms of a tail-merged
  if/else (par_EmitterCB1: speed in both initfill arms), a split variable, a `x = -x`-style use, locals for
  repeated loads (`p = res[j]; q = res[j + 1];`). No lever: declaration order, names, block scope,
  initialisation. A swapped pair usually means the expressions differ.
- **A dead store decides the pair**: `sel = 0;` not `ch = 0;` (ch 6 / sel 4 -> 5 / 5); read which slot a dead
  store hits before deleting or moving it (ai_HandleItemDrag byte).
- **Tie-breaks** (not evidence of the original spelling): a dead early store (`z = 0;`), a partial initializer
  `fall = {0.0f, -0.75f}` (a full one does not; d3t_UpdateRain wind/fall), an early dead `fall.x = 0.0f;`.
- **Counts do not move a pair**: split a variable used in disjoint branches (snd_UpdateRain: `tm` in the 0x95
  branch, 80 -> 33; 16 extra references to f: no lever).
- **Two address-taken out-params (`f(pos, &x, &y)`) swapped**: the expression at both uses, not a temp `i = y + x
  * 8` (ord_CountGridObjects). A pointer local (`mt = &desc.mats[mi]`) vs `desc.mats[mi].f` moves desc (+9 refs,
  d3iot_LoadBGF): count before trading.
- **Two address-taken `int x, z` swapped**: only a dead `z = 0;` or storing z first (both add code). No lever:
  declaration order, names, scope (ch4, 3 functions, open).
- **Local arrays in another order**: moving one into its block changes it; 720 / 16 declaration orders: no lever
  (d2_Unk46fba0). One struct of two locals reproduced an order (g2, 0x48f2b0).
- **A counter and a parameter-slot local swapped**: initialize them in the other order; `xl = x; dl = ...; xr =
  x; dr = ...;` not `xl = xr = x;`.

### Dead parameter slots

- **Frame 4 short, our memory local packed into a parameter's dead slot**: the local's first assignment stands
  before the parameter's last read in the original. ai_PickActionBuilding: `win = 0;` moved above `q[4] = a;`
  gave the original's own slot (frame 0x54 -> 0x58); moving the read `q[4] = a` later did not.
- **A parameter slot read as an extra argument**: an uninitialized local (set on one path, read on another: a
  NULL path `mov edi,[esp+0x20]`, a switch default, a flags struct, a returned `|= 0x40`, an `__asm int 3`
  branch). Check the writes before reading `[esp+arg]` as the parameter.
- **A parameter the original never registers, `mov reg,[esp+arg]` at each read, an uninitialized-looking load
  from its slot on a never-taken path**: the values set only on a hit (a draw from cumulative rows that all end
  at 1.0f) are separate locals without a fallback, `cls = i + 1` / `ret = (i + 1) << 8` with no `ret = k`
  before the loop and no `default:` in the switch; VC6 packs them into the dead parameter slot and the
  parameter stops being a candidate (ai_MakeDuelOpponent 21 ins -> byte; `k = i + 1` into the parameter, a
  `ret = k` init or `default: base = k` each keep k in ebx). Mark it `@original`.
- **What takes a dead slot**: a loop counter (0x47a460: w; 0x47b350/0x47b640: acc in x, m in y), `{ int t;
  f(&t); }`, `int neg = -1`, a variable whose address a callee gets, address-passed locals (`unsigned char r, g,
  b`; `f(w, &x, &win)`), a one-block char. Block-scope locals are memory-resident: `sent` in its `if` spilled
  to [esp+0x18] and freed ebx; a block-scope initializer is placed after the branch.
- **One slot smaller, the local in a dead slot where the original gives it its own** (zeroed early, `xor
  edi,edi; mov [esp+8],edi`): init it before the parameter's last use (ord_LoadStatistics `n = 0; w = 0;` at the
  top, 0x18 -> 0x1c; `i = 0;` for 0x5d44c0 / 0x5d33b0, 0x24 vs 0x28, with `if (i < n) { out = ...; do { ...;
  i++; } while (i < n); }`).
- **A parameter reloaded from its home in one arm (`mov ebp,[esp+0x14]; mov edi,[esp+0x14]`) while its
  candidate sits in esi**, or a value read from a dead slot on a fall-through (`mov al,[esp+0x14]`): no
  assignment there in the original. An if / else-if with no else leaves t / n undefined on that path and VC6
  loads their (shared) stack homes; a loop that sets w and breaks, with one store after it, reads w's home on the
  not-found path and copies the store tail into each clamp outcome (ai_DuelThrow 12 -> byte; earlier levers: an
  own float for a second group of RandomF rolls fixed the slot packing, a ternary arm split into two arms gave
  neg/sbb/inc, `__min(6, x)` gave jle, `for (..; i++, extra--)`, the face loop on i not j for the lea order).
- **A parameter loaded twice** (`mov esi,[arg]; and esi,0xffff`, later `mov edi,[arg]` unmasked): the masked
  parameter (`last &= 0xffff;`) plus an uninitialized local in the dead slot, assigned on every path to its use;
  `cmp ax,0xffff` compares the field itself (path_Reverse byte: `last &= 0xffff; cur = last; while (cur ..) { if
  (nodes[cur].parent != 0xffff) {..; head = nodes[cur].parent; } cur = nodes[cur].parent; } n = head;`).
- **The parameter's value in a stack copy, its slot holding another value** (`mov ebx,[arg]; mov [esp+N],ebx`,
  returned `mov ax,[slot]`): the parameter reused (path_Expand: `self = head; cur = &nodes[head]; head =
  cur->next; .. return head;`, 0x14 -> 0x10); `mov ax,[slot]` without `and` = unsigned short return. A running
  minimum in a parameter slot is the parameter reused.
- **A pointer parameter reloaded and stored back (`mov edx,[ebp+8] .. mov [ebp+8],edx`)**: a copy `s = src;`
  after `p = buf;` (hst_ParseText_Fake_2nd_Pass 120 -> 10, byte with `dashes++;` before `*w++ = '-'`;
  hst_ParseContext).
- **Who gets the dead slot**: one narrow local per branch for a K&R int (d2_Unk475730); `for (i = 0, p =
  ps->particles; ...)` (d3t_SmokeCB); `in->mult * in->value` repeated; `long` out locals, not `&wp`/`&lp`
  (mov_WndProc); a late parameter not copied at the top (d2_Unk46fba0).
- **Out-argument order**: read it from the pushes, not the slots (cm.c's 0x4fea50 callers).
- **Hoisted flag tests**: char locals at the top (`char keep = !(fl & 1)`). `and eax,0xff; mov [slot],eax`:
  `int ok = 0; if (path) ok = f(); if (!ok)`.

### Local kept in memory

- **A local memory-homed in the whole original** (stored at every definition, dead final loop values stored,
  reloaded after byte stores through pointers): it is address-taken - find the one small-frame `lea` and pass that
  local there (txt_OpenText: `i = txt_Count - first; fwrite(&i, 4, 1, f)`, not `&lineno`: 1085 -> 424).
- **An address-taken local reused for a later value** (`&w1` in a loop, later a send slot kept across calls):
  the reuse forces the later value into memory; give the address-taken use its own local (init_StartNewGame byte:
  judge slots back to esi / edi; init_OpenTown: `&n` in a block-scoped `int v`, 0x408 -> 0x404). A local whose
  address is passed must not double as a loop counter. A `(unsigned)&X + N` bound gives `jb`; `jl` is `(int)`.
- **Stored and reloaded before every use, or tested in a register but re-read at the last use**: `volatile`
  (0x4c2f30 x: `mov [slot],1` then `mov eax,[slot]`, 55 -> 6 lines; `volatile int hv; hv = hi = g;`; a flag
  kept in memory). Matching trick only; comment it so it is not cleaned.
- **Locals read through `bio_rd_i(f, &x)`** stay in memory and are the original's loop counters; a separate
  register counter steals the file pointer's register (d3io LoadScene, d3io2 LoadTexSet).
- **A spilling loop counter**: block-scope it. Writes through a `D3MATRIX *` cast of `&map->orgx` keep the
  reloads after stores.

### Shared slots

- **Four scalars inside two AIARG records' slots (recA / recB at s1+0 / +4, cntA / cntB at s2+0 / +4)**: a local
  joins a slot at most twice its size, so a 4-byte scalar never shares a 24-byte record's slot; two 12-byte
  arrays can: `AIBLD *spare[3]; int count[3];` indexed by type - 1, with the records block-scoped (ai_DecideMayor
  frame 0x6c -> 0x58).
- **A `fild` temp, an out-parameter of one arm and the -1 of the release tail all in one slot, ours a
  function-scope `id` in another**: an address-taken function-scope local conflicts with everything; give each
  arm its own block local (`unsigned short id` in the sel-10 arm, `int target = -1` in a block of the tail) and
  write the random draw inline (ai_ChooseObjectTarget, byte).
  ai_DecideGraveyardOwner2 (byte): `int x` into the else block that sets it (x and the `fild` temp share o1's
  dead slot) AND `flags = 0;` above the `o1->` stores (flags then conflicts with o1 and keeps its own slot, id
  goes to b's): each alone did nothing or worse, both together byte. c2frame `--names` X lines show it.
- **One variable reused for unrelated loops / branches / roles shares their slots**: split it (one counter or a
  tx/ty pair: ~3000 B of slot differences; a counter of two loops: one slot plus seven others fixed). A loop in
  both arms of an if/else in the asm was written twice. One array reused for unrelated values also leaves gaps.
- **One slot larger, an address-taken out-param (`&sx`) shares the original's slot with a loop temp** (dcp frame:
  `shared in the original`): declare it in its block; other equal-count slots may reorder (ch_GetNewPath, open).
- **Two small out-arrays sharing ONE slot with a hole behind, frame size equal** (`shared in the original`,
  `holes:` right after it): one array reused for both calls, sized slot + hole - `int row[4]` for two
  `sv_ButtonRow(.., row, 2, ..)` reading row[0] / row[1]; two `int[2]` give two slots, one `int[2]` a frame 8
  short (lobby_ChoosePlayer 6 -> 0). `int row[2]` declared in a block gave the frame and entry registers, sel
  sharing the row slot (ai_AlchemistResearchWindow, stuck).
- **A buffer at a dead array's slot** (inside a block that returns): a separate block-scoped buffer, not the
  function's other buffer (amt_CalcZuenfte: `gb[64]`).
- **Block-declared sharing**: a local used in one `if` block shares with temporaries; out-structs in the if-block
  that fills them share with other branches' arrays; a COM pointer in its `if` shares the loop counter's slot
  (dd_DlgProc). Two AIMSG locals stay separate.
- **Wrong slot sharing around inline-asm copies**: an asm destination assigned to an existing function-level
  pointer inside the macro block shares the slot of a loop variable dead in memory there (a block-local macro
  temp never did); several asm copies in a loop pair up when each sits in its own block with fresh locals, not
  two loop-wide pointers. A "mystery unreferenced slot" can be wrong sharing elsewhere (bmp_Load: rle[6] became
  rle[2]). No lever: names, declaration order, scope.

### Other findings filed here

- **`fld 1.0; fsub st,st(1); fstp f; fstp st(0)` with t kept on the FPU**: `f = t / ms; f = 1.0f - f;`
  (`f = 1.0f - t / ms` gives `fsubr`) (ai_CannonSwitchAnim byte).
- **A second scale kept with `fst [slot]; fmul st,st(2)`**: its own float local k (writing it back into f pops
  it) (ai_CannonSwitchAnim).
- **The elapsed-time `sub` as the first scratch register**: f computed BEFORE the vector copies (v.x, v.z, v.y)
  (ai_CannonSwitchAnim).
- **`dist * f * v.x` CSE'd differently once f is assigned first**: `(dist * f) * v.x` (ai_CannonSwitchAnim).
- **`mov eax,edi; add eax,ebx; mov al,[eax]` (ours `mov al,[edi+ebx]`)** for `ai_Goods[item * 0x41]` against four
  constants: a pointer local tested four times, `g = &((AIGOOD *)ai_Goods)[item]; if (g->type == 0x20 || ..)`.
  No lever: a local `kind = ..`, the element written four times (ai_HandleItemDrag byte). Maybe also the open
  `mov ecx,eax; add ecx,ebp; mov cl,[ecx]` of sv_UpdateGroundplan / sv_LoadGroundplan.
- **A meaning bug among "allocation only" hunks**: the original reads [esi] / [esi+4], `P->key < P->nkeys - 1`
  and keys[nkeys - 1] / keys[nkeys - 2], not `P->next < P->key - 1` / keys[key - 1] (d3anim_StepObject -> 1144,
  stuck). On a big draft check the field offset of every load that feeds an index.
- **A `strcmp(o, "lit")` callback calling the CRT instead of repz cmpsb**: `_stricmp`; 0x5dfb40 is `strncmp`,
  0x5eaac0 `_strnicmp` (`dcp link` conflicts show a wrong name).
- **Register choice / push order**: `p += n` as the for increment (`for (..; j++, row += n)`) (0x454780);
  hoisting `SendMessage(GETCURSEL)` out of a nested call into a variable changes the push order.
- **An index loop with the row pointer in memory**: a 2D array `tiles[5][5]`, not a `t += 5` pointer.
- **Tooling**: Bash heredocs through this harness turn `\` into `\`: write C with backslash strings through a
  file tool.

## Store, load and instruction order
Store order in a fill, hoisted loads and sunk stores, statement order effects, `add esp` placement, integer operand evaluation order.

### How the back end orders instructions

- **The scheduler's priority rule** (read from the pass, checked against dumps): per block VC6 builds a dependency
  graph (register def/use, memory overlap, flags; calls and jumps are barriers) and emits the ready instruction
  with the highest priority. priority = height * 8192, + 65536 for a memory load; height = longest latency path
  to the block end. A tie goes to the instruction that comes first before scheduling. So: loads are hoisted;
  `add esp,N` after a call sits right after the call in the list and wins every tie; an instruction overtakes an
  earlier one only when something later in the SAME block depends on it. A scheduling hunk therefore means one
  of: another list order (the expression tree: parentheses, operand order, a temporary), another edge (a later
  statement in the block that reads / overwrites what this one touches), or another block extent (a label that
  was not there).
- **Scheduling regions and memory conflicts**: a region ends at a jump, a label or after 81 instructions
  (FUN_10714d7c), so a long straight block is scheduled in pieces. Memory operands conflict only when they are
  the same symbol with overlapping [offset, offset+size) (FUN_1070437c): `Snd[0].snd` and `Snd[1].snd` never do.
- **Stores to an address-taken local order later loads**: with a function-scope struct whose address is taken,
  the scheduler orders every pointer load after the earlier stores to that local; declared in the block that
  fills and passes it, the loads move free (main_ChooseCity, main_EndRound, the block-scope sweep below). A store
  through a pointer local is assumed to alias other stores (par_EmitterCB0).
- **Where `add esp,N` ends up, the back end's three passes** (C2 13.00.9044, read in Ghidra and checked by
  skipping each pass with tools/c2dag/c2pass.py):
  1. Code generation puts the cleanup directly after its call, before the next statement's code (skip
     0x1070aa67 to see it: `call sb_Stop; add esp,8; ..; call sb_SetRestart; add esp,8; mov [g],ebx`).
  2. FUN_1070b884 merges stack adjusts along a straight run (it stops at a label or a jump): an earlier
     `add esp` is deleted and its amount added to the LATER one. So the merged cleanup sits where the last
     adjust of the run was: after the last call, or after a store only if another adjust follows that store.
  3. Cross-jumping (FUN_10712437, every round of the merging flow pass; skip 0x1070a91d to switch it off; "How
     VC6 orders blocks and merges tails") takes every `jmp L`, and compares up to 30 instructions before the
     jmp with up to 30 before L (labels do not stop it, a conditional jump does). It matches
     from the bottom over a dependency graph, not linearly: any instruction of one side that could legally be
     last and has an identical one on the other side is moved to the end of both and shared; the jmp then goes
     to a new label in front of the shared tail. That is the pass that turns `call; add esp,8; mov [g],ebx`
     into `call; mov [g],ebx; add esp,8` in ai_CannonShotResults (the `d2_SetViewEnabled(..); return;` path
     shares the add; the short return tail is copied back afterwards, so no jump is left).
  `c2dag.py` writes the pairs cross-jumping examines into build/c2dag/UNIT.dag (`xjump FUNC jmp@line`, then
  both regions as opcode@line, last instruction first).
- **Store-first before `add esp` has two causes**: an edge (a later instruction of the same block redefines a
  register the store reads: `mov [ebp+esi-0x18],bl; add esp,0xc; mov esi,..` mem_Alloc; `mov [esi+8],2; add
  esp,0x18; add esi,0xc` ch_GetWaypoints), or a tail shared with a second path that ends in the same call cleanup
  and has the same successor (ai_CannonShotResults: the final block's list is `call; mov [ai_ShotDirty],ebx; add
  esp,8`, the `add esp,8; pops` tail merged with the `d2_SetViewEnabled(..); return;` path before scheduling and
  copied back; a flip search shows add-first returns as soon as that path is removed or changed). A `ln` on an
  `add esp` that names another statement's line is the mark of such a merged-and-copied tail. With nothing after
  the store in its block the add comes first (812 : 23 over the image).
- **Scratch registers are handed out from one rotating pointer**: C2 rotates eax, ecx, edx through the function in
  code order (three copies in a row get eax/ecx, edx/eax, ecx/edx); each temp advances it
  (`mov ecx,[d3_Camera]` by one, each copy by two). A local that got a
  register is no temp (a candidate in eax does not advance it); a direct global read is. So one early statement
  fixes register hunks 60 instructions later (main_JoinNetworkGame, d3sndw_Update).

### Store or push placement around calls and `add esp`

- **A store before its `add esp` with a label right behind it** (`mov [Snd[0].snd],ebx; add esp,16`, the next
  block add-first): two cross-jumped copies of that code. Write the code out at each exit instead of `goto` to
  one copy: `if (!ready) { stop both; return; }` and `else if (d3sndw_Scene != d3_DefaultScene) { d3sndw_Scene =
  d3_DefaultScene; stop both; return; }`. The copies merge completely only when their reload temps get the same
  scratch registers (`mov ecx,[Snd[0].snd]` / `mov edx,[Snd[1].snd]`): count the temps between the copies; here
  NO `sc` local for d3_DefaultScene (the direct read is the extra temp the second copy needs). No lever: 437
  structural forms of the stop blocks, ~60 local forms, the entry conditions distributed over inline copies,
  labelled copies in the else-if, copies at the function end (127 arrangements, best 24 ins). (d3sndw_Update,
  byte)
- **A store before the cleanup after a call** (`call f; mov [g],ebx; add esp,N`): `if (t) {f(t); t = 0;} else
  g(x);` gives `call f; mov; L: add esp,4` with the other arm jumping to L (d3t_CreateSnowMipmaps' snow_tex
  release, toy-proven). No spelling of the block itself reaches it: comma forms, the store's rhs holding the
  call, goto / else / do-while shapes, block order, linkage of the global, 23 flag sets.
- **The pending-argument cleanup at another statement boundary** (`add esp,8` or 0x40 elsewhere, compensated at
  the next call: `add esp,0x18` vs `0x20`): last hunk of the window family; nothing in the source chose it, the
  twins differ only in how many statements sit between the calls. (ai_LoadGameWindow, ai_JoinGameWindow
  behaviour; ai_SaveOptionWindow, ai_SaveNameWindow, ai_LoadOptions open)
- **An inner call's arguments popped at once vs merged into one `add esp`**: an inner call as an argument
  (`f(.., dd_RGB(r, g, b), ..)`, `g((T *)f(x), 1)`, `g(f() ...)`) pops at once; assigned to a local first (`c =
  dd_RGB(..); f(.., c, ..)`, `a = f(x); g(a, 1);`, `k = d2_FindPic(..)`, `angle = f(); g((int)(angle * k));`)
  the pops merge.
- **Call-argument setup hoisted into earlier stores** (`lea eax,[keys]; push eax; push 2; sub esp,0xc` right
  after an early store, the original finishes the field copies first): the store into the array passed by
  address (`keys[0].time = unk`) sits right before the call; it is the last thing VC6 orders before the pushes.
  (d3t_PlaceObject 26 ins -> byte; cm_ThrowGrenade 48 -> 26, still shape)
- **Stores sunk behind the argument pushes**: recompute the arguments from the pointer, `fread((char *)(d + 1),
  d->len, 1, f)` after `d->len = ..; d->data = (char *)(d + 1)`, not `fread(d->data, hdr.len, 1, f)`
  (sb_LoadEntry).
- **A constant store before vs after a call's push**: statement order decides it; `ev->h = -1;` before `a =
  f(ev->x);` (he.c 0x543180); `n = 1;` before vs after `evt_Error(...)` moves where the constant is loaded.
- **Loads and `add esp` hoisted / sunk differently with an absolute-address macro** (`*(int *)0x6ed938 = x`):
  replace the macro by a named `extern int g;` before matching (0x52fa10, 0x52fa90). An extern
  `struct D3SCENE *d3_DefaultScene` hoists `mov edx,ds:X` into the prologue where `ai_Unk438d40(U_I(0x5fb5b0), ...)`
  loads it at the call (unk_58bbd0.c 0x58f980, byte).

### Loads hoisted early or kept late

- **Store kept late while its load is hoisted early** (one store after an unrelated later store, e.g. byte flags
  after a float fstp): read the value into a local early, write the other store through a pointer local (`sz =
  &p->size; *sz = ..;`), store the local last. The pointer alone keeps the order but stops load hoisting.
  (par_EmitterCB0)
- **A compare hoisted to the top of its block** while the original has it after some stores: write one store of
  the run through a copy pointer (`t1 = t + 1; t1->f = x;`); the test lands after that store. Try every store (a
  2^n mask generator; exactly one of five matched par_Draw). Two byte bitfields through one base (`t[0]`, `t[1]`):
  VC6 hoists the second load above the first store; a `t1 = t + 1` pointer, propagated away at no code cost,
  keeps the source order.
- **A struct fill scheduled differently (load hoisted above the memset, constant stores sunk past the push, a
  byte store after the time copy, vector loads interleaved with stores)**: declare the struct in the block that
  fills and passes it (`if (..) { AIMSG msg; memset; fields; cm_SendLongMessage(&msg); }`, `D3VECTOR4 pos;
  D3VECTOR rot;` in each block that does), also in a loop body or at the start of a switch block. Try it first on
  any fill-order hunk of a struct local. deep-tools blockscope.py over every below-byte function with a
  function-scope AIMSG: 9 byte, 1 draft -> shape, 5 closer. No lever: statement order (5040 orders in
  hmt_DecideElection, ~250 in ai_MsgWorkerStop). (main_EndRound, main_ChooseCity with `pos.z` assigned last,
  ai_AlchemistTrainShareWindow)
- **A search loop's two loop-invariant loads in the other order** (`mov edx,[m.grave]; mov eax,[e.grave]`, ours
  the base first): a walking pointer set before the loop (`g = e->grave; do { q = g; .. g = q + 1; }`) loads
  first. Index the base in the loop (`do { q = &e->grave[i]; if (q->id == m->grave) break; i++; } while (..)`) or
  read the compared value into a local before the base (`n = m->grave; g = e->grave;`). No lever: swapping the
  compare's operands, reading the local after the base. (gv_RaiseDead byte)
- **Element pointer before the field test** (the original's `lea ecx,[eax*4+X]` before `cmp al,1/jb; cmp
  al,6/ja`): `ch = ai_Chars[id]; k = ((AMTCHARVIEW *)ch)->rank; if (..) { .. if (cand[i] == ch) ..`;
  `k = ai_Chars[id][0x164]; .. cand[i] == ai_Chars[id]` recomputes it after the branch (amt.c 0x4e88a0 658 -> 424
  bytes, 0x4e8c20). A byte field `>= 1 && <= 6` is `cmp al,1/jb; cmp al,6/ja`; `!= 0 && < 7` is `test al,al/je;
  cmp al,7/jae`.
- **A sum tested in an `if` and kept afterwards** (`mov edx,[eax+6]; lea edi,[edx+ecx]`, load in the branch
  shadow): `w = s->w; if (x <= d2_ClipRight && w + x >= d2_ClipLeft) { right = w + x; ...}` (0x47bd70, byte).
  No lever: `(right = s->w + x)` inside the && (`xor edi,edi; mov di; add edi,ecx`), `right = ...` before the if.
- **A field load scheduled above another parameter's test**: read it inline in each case, not into a local
  before the branch. Three member stores then a member test follow source order only once the tested member is
  read into a local first (d2_Unk475cb0).
- **Switch type loaded early vs late**: `type = obj->type; switch (type)` right after a call loads early; `switch
  (obj->type)` late. Switch blocks are laid out in source case order.
- **A hoisted volatile load before an early return**: `int now = tb_Calls;` before it.
- **A `lea` moved around neighbouring stores**: locals assigned before an `__asm` block are computed at function
  entry; move the assignment.
- **A constant materialised between the guard's load and its test** (`mov ecx,..; mov ..,0x1f`, ours sinks it
  and tests eax): `i = 31; if (changed && dm_Q[31].valid == 0)` and declaration inits sink it; every form that
  forces it before the guard (for-init `for (i = 31, q = &..; *q == 0;)`, comma in the `&&`, guard comma) fixes
  the tail but merges the loop head and reallocates the whole function (7 -> 180 asm lines; const#0 web -55
  against _di_PressX -57 in c2prio). Allocation work, not a source spelling. (di_UpdateButtons, open)
- `p->keys[n]` and `(*keys)[n]` schedule the pointer load differently.

### Reloads the original keeps (and CSEs it does not)

- **A value the original recomputes inside an if block** (reloads a field / recomputes an index the test already
  computed; keeps only the loaded globals): no local; write the expression again, especially one with a call:
  `if (util_RandomF() > X / (d3_Time - X))`, not `t = X; r = &a[i]; if (util_RandomF() > t / (d3_Time - t))`
  (anm_Unk4bb860). Probe with dcp listing: which values VC6 shares, and an insertion map (store / call at the
  block start) tells what the original's IR had there.
- **A product CSE'd from an if test into the loop below** (ours keeps `uni * 0x3e0` in ebx, the original
  recomputes): test the entry again in an else-if, `} else if (d3_Scenes[T[n]].dirty) { uni = T[n]; ..`, not
  `uni = T[n]; if (d3_Scenes[uni].dirty)`.
- **The same field loaded twice back to back (`mov eax,[ebx]; mov edx,[ebx]`) and a hoisted load whose store comes
  late** (unique in the image: d3fl_LoadMap 0x442580 / 0x442949): VC6 CSEs a field load across stores to other
  fields of the same base (offset-exact; no store, label, empty if, cast view, overlay struct or pointer copy kills
  it), so the original read that field anew: a `volatile` view of the field (`union { int size; volatile int
  sizeFresh; }`, @original) at those reads matched. A store through a separate pointer local (`m = (D3MATRIX
  *)&map->orgx; m->_11 = ..`) does kill the CSE but also gets scheduler edges to every later `map->` load, so the
  later load is not hoisted; direct field stores keep the hoisting. (flip.py and climb found nothing: probe
  each read with `*(volatile int *)&p->f` to see which reads are separate.)
- **Local copies of array elements where the original reloads some after a store** (`a = d[i]; b = d[j]; ..
  (a + b)`): read the elements in place; VC6 CSEs up to the first store and reloads after it (d3sm_Sample
  340 -> 194 ins).
- **A field the original reloads at a call where ours forwards the entry local** (`push edi` vs `mov
  ecx,[esi+0x14]; push ecx`): any join before it with a call on the other path kills the CSE (proved with a
  shared test after the count==0 block); no shape found that also keeps the original's tests
  (ch_ActDroppingObject, open).
- **A global array / table indexed in every statement is reloaded after each store or call** (`mov
  ecx,ds:0x6f0e2c; mov [edi+ecx*1+4],al`): write `g[i].x = ..` / `table[i].field` / `d2_Objs[obj].f = ..` /
  `d2_Pics[idx].x = ..` per use; a pointer local (`char *e = base + i * 12`, `p = &d2_Pics[idx]`, `D2OBJ *o`)
  keeps it in a register. One function can mix both forms. A local pointer assigned in both arms of an if (`w =
  &d2_Wins[..]`) keeps the `*0x3c4` per branch. `d2_Inputs[k].f` through a cast-array macro gives `[reg+abs]`.
- **Pointer and count reloaded from .data around every store** (`mov eax,ds:count; lea edx,[eax+eax*2]; mov
  eax,ds:arr; shl edx,5; mov [ecx+eax+off]`): write the global expression at each use
  (`ai_Bauplaetze[ai_NBauplaetze].x0 = ...`, `((T *)ai_Bauplaetze + n)[i]`); a cached `float *bb` / `bb =
  ai_Bauplaetze + n * 0x60` never matches (f3init.c 0x510770).
- **A global the original reads at every use** (`mov eax,ds:X`, `cmp DWORD PTR ds:X,imm`, reload after each call):
  no `v = global` local (it lives in ebx and reloads elsewhere). 0x51db00 (`ls_Version`, `mov eax,ds:0x623650 / cmp
  eax,imm`, 287 instructions equal), 0x520b80 (114 -> 74 lines), d2_Unk477ed0 (a local cost 539 bytes). A global
  read twice is read twice in the source: assign the local after the early-outs and `switch` on the global itself.
- **A local caching a field or global hides reloads**: `int cid` replaced by AMT_NETID(c) at every use (`[esi+4]`
  reloaded; 678 -> 517 lines, frame to 0x8dc, amt_ExecuteMayorPowerMessage); `(char *)d3_Terrain + 0x1a64 + i` at
  every use plus the four seasons unrolled, no mark / fmt aliases (`ds:0x633738` reloaded before every strstr /
  tex_FindFile call; 511 -> 82, init_SetSeasonTextures 0x5128d0; same shape in 0x512f30). A decompiled `pr =
  ai_Goods;` kept in two places is plain `pr = (char *)GM_GOOD(proto);` after the block (gm_CreateObject 194 ->
  145).
- **The `bld` local vs `c->bld` mix is load-bearing**: the local for the identity checks, `c->bld` for the later
  reads (ai_BuyUpgrade: 52 hunks; all-`bld` 377, all-`c->bld` 81).
- **A global byte reread for a table index right after its store** (`mov [g],al; .. mov eax,[g]; and eax,0xff;
  shl eax,3; mov ecx,[eax+T]`, ours `[eax*8+T]`): index by the global, `T[g.byte[0]].w`; it also fixed the
  scheduling of the following GetPrivateProfileIntA reads (ai_LoadOptions 14 ins -> byte; a stray order hunk was
  two stores of one record swapped in the source).
- **A destination parameter stored through is reloaded from its slot after every store**: a local copy read once
  (`d = dst;` `*d++ = ..`) keeps it in a register (0x4adf40 byte).
- **A value re-read from memory at every use with no store or call between**: `volatile` (globals like `d3_Time`,
  locals, parameters: d3q_Build). A global incremented as load / inc / store instead of `inc [mem]` / `dec [mem]`
  is volatile. Volatile time product: `el = TIME; el -= start;` (d3_snd.c).
- **Stored value forwarded vs reloaded**: a compare with no reload (`mov al,[kind] / mov cl,[applied] / cmp cl,al
  / mov [office],al`) is `c->officeKind = kind; if (c->applied == c->officeKind)` (`== amt_Table[i].kind`
  reloads). `a->n = n; ... mem_Alloc(a->n * sz)` forwards, `n` reloads its slot. `*py = ...` without a local y
  forwards, with one it reloads. `msg.n = msg.fromId;` is one load, `msg.n = b->id;` two (unk_4b6480.c 0x4ba0c0).
  `to->last->next = obj; obj->prev = to->last;` reloads; the original reads `to->last` into a local once.
- `x->f += call()` computes the lvalue address before the call. Storing `in->value = val - step*q` and re-reading
  it avoids the `neg; imul; add` form.
- `if ((g = expr) > 0)` with no local gives a global store followed by a register test.

### Store order in fills, records and rects

- **Any order hunk in a run of independent stores or a message fill: search it, never infer the source order
  from the scheduled stores** (the list follows source order, not 1:1; single swaps stall). Tools: `dcp try`
  over all 720 / 5040 orders (1-2 min), deep-tools permute.py, a generator script, vargen `move` on 3-6 lines
  with `--max 1500` then a single-move hill climb (every line to every position, keep the best by lines, repeat)
  then `perm` of the rest, storeorder_hc.py scoring orders against the original's listing with addresses
  resolved and climbing from several starts (the natural / save order too), `--move-line` on every early
  statement of a big draft (check what a move-line variant moved: moving a read line can change the meaning).
  (d3if_SelectScene byte with the natural order except one line; 0x488740, 0x4884b0, d2_OpenInputIcon,
  tex_Create, tex_CreateHi, d3_Unk459300 198 -> 176 lines; cm.c senders matched in Ghidra's order or in offset
  order)
- **AIMSG fill orders that matched**: sv_t `type, flags, to, fromId, time, args` (three alchemist functions:
  ai_AlchemistTrainShareWindow, ai_AlchemistSendResearchStart, ai_AlchemistSendRecipeResearch, 26 / 35 / 39 ins
  -> byte; the window one also block-scope AIMSG and `arg[1] = idx` after the random `arg[4]`); ai_CommandFest /
  ai_CommandRiot: `msg.time = ai_Time` after the scalar fields (type, to, fromId, flags, arg); hmt_DecideElection:
  type, to, time, fromId, flags, arg[5], arg[4] with block scope (plus `n >= 4`, `cand[i].score > best`); without
  block scope the best order left `mov byte [msg.type],0xa5` before the time copy. A byte store where we write a
  dword is a byte field of the union. gm_ForfeitEstate: `flags` before `time`.
- **A struct time copy whose clock dword is hoisted / last two stores after the other fields**: the copy written
  last (`party.time = ai_Time;` after kind / flags / thing, main_GameLoop byte).
- **Field / constant stores: struct (field) order**: lobby_Screen's defaults in field order (load cm_NetGame.mode
  first, `reserved` first, talents[0] before talents[2]; a decompiler's grouping by value is the usual culprit);
  tex_Create / tex_CreateHi struct order with the bitfield members last (VC6 sinks bitfield stores behind the plain
  stores). d2_MakeSurface tail: left, pitch, right, top, bottom.
- **RECT fills before a Blt** (`add esi,edi` vs `lea edx,[esi+edi]`): `left, top` first lets VC6 add `sx += w`
  in place (d2_BlitPartToDD: left, top, bottom, right / left, top, right, bottom). Clip on the parameter itself
  (`if (x < s->left) x = s->left;`), not a copy.
- **Stores scheduled with other scratch registers after an inline `strcpy` of a literal** (`or al,0x50` vs `or
  ecx,0x50`, hoisted loads): move the strcpy, not only the stores: `ai_NumPlayers = hdr.players;
  strcpy(main_NetHost, "127.0.0.1"); ai_GameFlags |= 0x50; net.players = ..; ..` (main_JoinNetworkGame 36 -> 14
  -> byte; flip.py "swap strcpy with the next statement", then 3359 orders). Hand-written `do { c = s[i]; d[i] =
  c; i++; } while (c)` copies were plain `strcpy`. Inlined strcpy of short literals and member stores interleave
  in source order: `act.id` before the strcpy (0x4aaa10, 0x4ab9f0, 0x4abed0).
- **A clamp computed between two stores**: a temp in source order there: `*(m+4) = ..; k = r > 1 ? r : 1;
  *(m+0xc) = r; *(m+0x10) = k * 32;` (0x4ab7f0).
- **A pointer from a parameter computed one statement late** (k into edx, added into edi = base): `bt = ..`
  BEFORE `b->type = k` (gm_CreateBuilding 141 -> 103).
- **A global counter's store after unrelated field stores** (`mov [ai_NextObjectId],ecx` after the c->mask store):
  write `x++;` after those stores, the read `c->netId = x;` stays put (ahm_CreateAlchemist 5 -> 3, gv_RaiseDead
  103 -> 51).
- **Independent bit-field unpacks reorder the byte registers**: `mt.c5 = c2 >> 1 & 1;` before `fl.s.c1 = c2 & 1;`
  (d3_FindBGF 26 ins -> byte, 1 of 24 orders).
- **Struct fields stored before a loop where the original stores them after**: compute the values into locals
  (callee-saved through the search) and store the fields at the end (0x4e6680: office / a / d / e / b).
- **An order no statement permutation gives**: read the fields into locals first (ai2 0x48a380 36 -> 9), or write
  the statement in both arms of the preceding if/else (ai 0x489660).
- **Store before vs after a field test**: `p = &tab[i]; if (p->f & 1)` stores first; the store after is `if
  (tab[i].f & 1) { p = &tab[i]; ... }`.
- **Volatile copies in the block (z copies, weight reads)**: x / y as `V.x = F.x; V.y = F.y; V.x = w * V.x;`
  (loads share the FPU stack; `V.x = w * F.x` multiplies from memory); `if (!(z < MaxZ)) MaxZ = <re-read of
  v->z>` beat the memory compare by 12 lines (d3_Unk459300).
- Deltas first, then write-backs (`S.x += pt.x - L.x` pairs). Parenthesising a cast expression can change the
  store schedule. Stores into a by-value flags struct after `memset(&f, 0, 4)` come out in source order
  (`f.b[0] = 0x7f;` before `f.b[2] |= 2;`).
- gm_ForfeitEstate (stuck, 50 -> 38): opening assignments (120 orders) and the message order fixed; the decompiled
  `if (i >= 6) { if (i < n) {..} break; }` is `if (i >= 6) break;` plus one `if (i < n)` after the loop. Open:
  VC6 lays the house-owner block out at the function's end (jne there, jmp back), the original inline after the
  `||` test's `je`; no arm form moves it.
- **The registers of a loop above a store run are off (counter in a register where the original spills it,
  byte counters rotated)**: the order of the stores AFTER the loop decides them; run the n! round on the tail
  stores even when the hunk is in the loop (dd_SetMode: the six Bits / Loss stores as RLoss, GLoss, BLoss, GBits,
  BBits, RBits, 116 -> 39 ins; run it again after any other change upstream, the best order moved once).

### Init and zero-store order

- **Two zeroes before a loop** (`xor ebp,ebp; xor edi,edi` in n, i order): the index first is `for (i = 0, n =
  0; ..)`, the other first `n = 0; for (i = 0; ..)` (ai_BlackmailBuildingListener, unk_4b39f0.c 0x4b3c40 /
  0x4b4670). A counter zeroed before an unrelated call: `i = 0;` before the call, then `for (; i < n; ...)`. A
  count after an iterator's next-call: `b = next(), n++` in the for increment (0x4b51f0).
- **Initialised declarations store in declaration order** (`AIBLD *nearest = 0; float best = 1e8f;` he_RunFire;
  `int next = 2; int val = 0;` evt_Eval byte; `int count = 0;` before the array d3t_MoveCameraPath). Generate all
  orders of the initialised declarations (and the statement form) when only an entry order hunk is left. A
  struct field copy with the zero store last reserves the zero register; the store itself is still hoisted.
- **Statements vs initialisers**: `read = 1; z = 1; detect = 1;` gives `mov byte [read],1` where `unsigned char
  read = 1` shared z's `mov bl,1`. `found = 0;` as a statement after `y = 0.0f;`, `float y;` declared before
  `D3VECTOR4 pos;`: `mov [esp+0xc],0` before the pushes and `xor bl,bl` after (`char found = 0;` stored found first
  and moved y). A first `prev = NULL;` inside an `if` goes to the declaration (`SBBANK *b, *prev = NULL;`) when
  the original hoists `xor eax,eax` next to the first xor (sb_UnloadSampleBank).
- **Chained assignments store right to left**: `a = b = c = k` (d3_Render); `c.r = c.g = c.b = c.a = v` stores a,
  b, g, r; `maxy = maxx = 0;` stores maxx first where two statements stored maxy first (d2_GrabShape16NoRuns). In
  an if/else-if chain VC6 merges trailing branches that share such a chain; per-branch statements keep them
  apart.
- **Register roles and the register copy between zero stores follow the init order**: permute the init block
  (`women = men = 0; w = m = 0xffff; f = 0`, snd_PlayFavourComment); `obj = 0; best = 1e8f; x = ...; z = ...;` at
  the top gives the original's store order, `x`, `z`, `best` the same instructions reordered (asmdiff `order`).
- **Zero-macro temps**: their declaration order decides store order (the memory TU wants dst before size). Size
  slot before destination slot (`mov [ebp-8], 0x200` then `mov [ebp-4], eax`): assign the destination inside the
  clear's block after the size temp (hst.c `AI_ZERO_BUF(n1, buf, 0x200)`); `n1 = buf; AI_ZERO_AT(n1, ..)` stores
  n1 first. mem_Alloc needed the asm to name plain variables (`n = size; p = malloc(n);`).
- **A zero field store right after a memset** (`memset(&save, 0, ..); save.slot = 0;`) reuses the memset's zero
  register (`mov [slot],eax`); after the strcpy it is an immediate store (main_GameLoop).
- **The gzip detect with the z = 0 path in the fall-through**: `if (size <= 10) z = 0; else if (m0 || m1) z = 0;
  else z = 1;` (&&, ?:, z = cond, reversed forms all = control). vfs_MemOpen 37 -> 2 ins; left: an early `return
  0` whose `xor eax,eax` the original hoists to the entry and joins past the final `mov eax,esi`. No lever: ret
  locals, f = 0 placement, flat / nested / duplicated failure returns.

### Operand order in sums, compares and addresses

- **`mov cl,[a]; mov al,[b]; test al,cl` with the loads swapped, every operand order = control**: the two
  operands have different types in the original. ai_ChooseAction `p->mask & act->flags`: the action struct's
  `flags` is an `unsigned int` (AIACT), our view had `unsigned char`; VC6 still loads one byte, but the load
  order and the test operands follow the int (byte). A `(char)` cast on one side moves the loads but not the
  test operands; try the wider field type (view or probe `*(unsigned int *)&x`) first.
- **Byte weights of one dword (`mov [esp+X],edx` then `mov esi,[esp+X+1]; and esi,0xff`, the first byte as `and edx,0xff`)**:
  no named locals `a = wv.b[0]` .. (each one a candidate: 200 -> 28 ins written inline as `s0[2] * wv.b[0] + ..`),
  and the dword copied as a 4-byte RECORD (`wv = *ww` with `D3FLWEIGHTS *ww`), not a union's `wv.l = *ww`: the
  union form leaves `and edx` before the `[esp+X+1]` load (a scheduling tie, 4 ins). Neither block scope, a
  `uchar[4]` with `*(unsigned long *)`, nor `((unsigned char *)&l)[k]` moved it (d3fl_BlendC, byte).
- **Commutative operands are ordered by rank** (the rank of the variables' first occurrence): a dead store
  (`mark = 0; a = 0;`) at the top fixed 2 of 4 swapped `[ebx+edi]` / `[edi+ebx]` (d3_MirCreateOutline); an
  unrelated earlier loop's index name changed `[iv+light]` vs `[light+iv]`. For a sum of globals the source order
  is the load order.
- **Three terms of a sum in an order no spelling gives** (`sq[R - r] + sq[B - b] + sq[G - g]`; 6 orders, temps,
  parentheses one code; crossed M8 lines): the arrays were ONE object, `unsigned char pal[3][256]` (three symbols
  rank differently from three offsets of one symbol). Probe: `extern unsigned char probePal[3][256];` inside the
  function in a dcp try variant (qnt_Nearest 4 ins -> byte; qnt_PalR / G / B became qnt_Pal[3][256]).
- **lea operand order `[esi+edi]` vs `[edi+esi]` for `i = (step + i) % N`**: follows which variable was defined
  first in the loop body (`i = ..; step = ..` flips it), not the text. ai_TokenRandomPlayer: outer `for (i = 0; i
  < 0x300; i++)` around a search that reassigns i (byte); `do .. while (i + 1 < 0x300)` left the lea.
- **Subtraction and mixed -/+ chains are reassociated**: `w->w - r->w - l->w` = `w->w - l->w - r->w`
  (d2_DrawButton 0x46c039), `w->w - r->w + w->x` = `w->x + w->w - r->w` (0x46c091): no lever for the d2
  member-load-order family; the leftover is register allocation.
- **A cast on one side of a compare reorders the loads**: `year % 4 == (int)(c->netId & 3)` loaded netId first;
  without the cast `year % 4` first (gv_GraveyardTurn). Compare operand order follows the source: `h + y >
  s->bottom` gives `cmp reg,mem; jle`, `s->bottom < h + y` flips it. `n > 2` / `n >= 3` give `jle` / `jl` with
  different immediates (0x4a94e0); `k * 0.25` is a QWORD constant, `0.25f` a DWORD (0x4aa580).
- **Two induction variables the original keeps apart**: `p += 0x80; sent += 0x80;` gave the two adds, `sent +=
  0x80; p += 0x80;` folded p into buf+sent (183 -> 8 bytes). A halving loop `for (; step > 1; step = half)` vs
  `step >>= 1`: different IVs (-34).
- **A byte-offset IV on a stack array (`mov [esp+esi+0x90],eax / add esi,4 / cmp esi,0x3c`) plus a separate
  source pointer, ours `[esp+esi*4+N]` with the source folded to `[esi+ebp+0x75]`, and a `rep stosd` of the
  same count right before the loop**: the loop also zeroed a second int array, `for (k = 1; k < 16; k++) {
  cnt[k] = 0; own[k] = f(q->x[k]); }`. VC6 lifts the zero stores out into the `rep stosd` and the second array
  use makes it strength-reduce k to the byte offset (two int arrays per index: the same `add esi,4` form in
  gm_EndDayIncomes, cm_TrialAskPlayer). Find it by `rg "esp \+ e.. \+ 0x"` over the original's disassembly; a
  `flip.py` probe adding the store matched it (ai_DecideChangeProfession byte). No lever: pointer walks of
  either array, `*++l` / `*l++`, short / char counters, a memset before the loop.
- **Index eliminated in the original (`mov edx,-0x80; lea ebp,[p+0x80]; sub edx,p; mov [slot],edx` before the
  loop, `mov edx,[slot]; add edx,ebp; push edx` for `f(i)`, loop end `inc ebp; add eax,ebp; add esi,4; cmp
  eax,5; jb`), ours keeps i in esi with `[esi+ebp+0x80]` / `[esp+esi*4+N]`**: the original had no named local
  for the element: `p->skills[i]` written at each use (`if (p->skills[i] < 42) .. else if (p->skills[i] < 84)
  .. fb = p->skills[i]`, CSE'd into one byte register). A named `c = p->skills[i]` keeps i as the basic IV
  whenever i is also passed plainly to a call (`g(i)`); `g(i + 1)` or no call would strength-reduce. Probes
  that only fold (no lever): `i + 0`, `(i*4)/4`, a second IV `j`, a pointer IV `s = p->skills`, casts on the
  argument, int / unsigned i, a zero-loop for the memset, register pressure. ai_DecideTrainSkill 148 -> byte
  (with the named local dropped in the sibling arm too, then one `memset` / `k = o1->ref` swap from climb).

### Stores VC6 merges, drops or keeps

- **Two stores to one field where ours has one** (`mov [esi+0x14],eax` then `and al,0xfc; mov [esi+0x14],eax`):
  `s->flags |= 8; s->fadestart = 0; s->flags &= ~3;` and a temp copy both merge; the first store must be a
  volatile access at the field: `*(volatile unsigned int *)&s->flags = fl;` or a union member `volatile unsigned
  int volatileFlags` (same bytes, typed; SBSAMPLE in sblib.c). A volatile local pointer to the field swaps the
  `and` and the fadestart store. `& ~3` emits `and al,-4`, `& 0xfc` emits `and eax,0xfc` (sb_Update).
- **A global zeroed before a counting loop / branch that ours drops as dead** (`xor dl,dl; mov [g],dl`, again
  after the loop; or `mov [g],edx` ahead of the `jne`): count in the global itself, `g = 0; for (..) if (..)
  g++;`; a local count with `g = count` loses the first store (ai_SaveNetworkGame byte, main_GameLoop byte).
- **A dead store the original keeps** (`mov word [esi], 0xffff` before `c->id = n`): through the array
  (`ai_CharsT[n].id = 0xffff`); through the record pointer VC6 drops it. A store overwritten before any read is
  dropped and its expression moved to the second store (can move a rand call past stores): look for another
  expression shape, not aliasing.
- **A dead `lea ecx,[ebx+ecx*4]`**: an identical store written twice; VC6 drops the repeat late and leaves its
  address. Kept as `@original` (a second array, a copy through the list entry, other orders: other code). The
  level store before the list store and a GMBTYPE pointer took 23 -> 1. (gm_EndDayIncomes byte)
- **A bit set before the mask local in a tail-merged chain** (`or ah,2 / mov edx,0x4000 / jmp store`): `c->status
  |= 0x200; m = 0x4000;`; `f = f | 0x200; m = ..; c->status = f;` emits m first.
- **Two stored `pos++`** (`inc; mov [pos]` twice): one `pos++` before and one after an inner block that holds an
  initialised local array.
- `goto clamp` exits of a state chain are one plain else-if chain with the rest in the last else (ai_CannonAim
  byte). d3d_UsedCard.info.maxw (0xfe7998) is the same global as an extern below a 0x338-byte D3DCARD struct:
  check data.csv aliases before inventing a name. `near` is a windows.h macro (cannot name a local).

## Calls, arguments, prototypes
How arguments are pushed, a call written in both arms, parameter and return types of callees, conventions, inline and intrinsic expansion.

### Undeclared callees
- **Any unexplained difference in a caller**: an undeclared call compiles silently (implicit int, C4013) and
  changes the caller's code (64 of d3fl_UpdateTiles' differing bytes). Grep build logs for C4013.

### Call written in both arms of an if/else
- **Argument pushes before the test that picks one argument** (`push f; push size; push 1; test flip; jne`, arms
  only computing the row index, one shared `imul; lea; push; call`): the call in each arm, `if (flip == 0)
  fread(dst + i * w * 3, ..); else fread(dst + (h - i - 1) * w * 3, ..);`. A ternary in the argument is evaluated
  before every push, in any order or placement (d2_ReadBmpPixels 92 -> 12 ins). Hoisting needs identical first
  instructions and one predecessor per arm; a shared call block (from `||`) never hoists.
- **`test; je; push K1; jmp; push K2` + one call**: `if (c) f(.., K1); else f(.., K2);` (tail-merged); `push 2;
  push K1; jmp; push 2; push K2; push ecx; call` likewise (ls_LoadScene). An argument shared by both arms must be
  in a variable first.
- **Ternary argument with two constants**: conflicting notes, measure: `f(t, c ? K1 : K2, 2)` gives `mov eax,K1;
  je; mov eax,K2; push eax` (ls_LoadScene) vs `f(.., c ? K1 : K2)` gives setcc/sbb.
- **`push 0x0` / `push 0x1` in two arms + one call**: `if (c) f(x, 0); else f(x, 1);` / `if (i != sel) f(x, 0);
  else f(x, 1);`. `v = c ? 0 : 1; f(x, v)` pushes a register, `f(x, c ? 1 : 0)` gives setcc, `f(x, i == sel)`
  `sete`. Hoisting `v = list[i]` before the branches keeps one array load (unk_58bbd0.c 0x58f980).
- **Mask / temp for a choice of argument**: `f(.., i == 0 ? i : sz, ..)`; `if (i == 0) f(0, i, ..); else f(0, sz,
  ..);` matches. `fopen(path ? path : def, ..)`: two calls in an if/else matched, the ternary did not. Two explicit
  recursive calls (`!n` arm first), not a ternary argument.
- **Id or string pushed per branch, one shared call instruction** (setup `and esi,0xff; lea buf; push ..; push
  const` duplicated, only the call merged): write the call (txt_Format / ai_Unk4741f0) in each branch or each arm
  of an `if / else if / else` chain. A ternary, a shared `name` local or `id = ...; goto show;` gives `mov
  eax,0x76; jmp` + one shared setup (sv_CheckStock 0x5210a0 -> 0 differing, sv_MessageBox 0x521150, 0x5216a0,
  0x521900, 0x4f8200).
- **Original calls the same address in both arms, ours once after the join** (every order / polarity / goto keeps
  them merged): two empty functions LINK folded (/OPT:ICF). Give one arm its own stub (`/* unlinked: .. folded
  into X */ void init_RoomNoOp(void) {}`, defined below the caller) (init_ShowBuildingScene byte).
- **Two copies of a call's pushes, one jumping into the other's call** (`inc eax; push eax; push eax; ..; jmp
  call`, a second `inc eax; lea ecx,[buf]; push ..` falling into it), a local read from its slot on one path
  (`mov eax,[esp+0x10]`): `goods = 0; if (home) name = rec; else goods = ..; if (goods == 0) f(name..); else
  f(goods..);` (init_LoadShopScript byte).
- **Call tail per arm with `movsx eax, word [w->y]` pushed to a short parameter, parent loaded in each arm**: ONE
  call after an int local chosen by if / else (`y = ..; else y = w->y; d2_MoveObj(x, y, w->parent);`); VC6
  duplicates the call + epilogue. A widening `movsx` to a short parameter means an int local in between
  (d2_PlaceWindow byte).
- **Different prologue / allocation with the call in both arms**: assign the differing arguments in the arms, call
  once after the branch (d2_DrawObjCaption 0x46d220 with d2_DrawTextBox 150 -> 131 ins, 116 -> 71 differing;
  d2_CenterObj 0x46c1d0 `int x` per arm, 116 -> 59 of 144 bytes). Try before a spelling round.

### Parameter width: what the caller's push shows
- **Bare byte / word pushed** (`mov cl,[m]; push ecx`, `mov al,[x]; push eax`, no `xor` / `movsx` / `movzx`):
  the callee takes `char` / `unsigned char` / `short`; declare it narrow (amt_SendOfficeFree 0x4e65e0, f3init.c
  0x512de0 / 0x5115b0). An `unsigned char` last parameter gets a global byte as `mov dl,[g]; push edx`.
- **`xor ecx,ecx; mov cl,[m]; push ecx` / `and eax,0xff; push eax`**: the callee takes `int` (f5, ai).
- **`push 0xff`**: an `unsigned char` parameter; `char` gives `push -1`.
- **Word loads**: `xor eax,eax; mov ax,[x]; push eax` (also `*(unsigned short *)p`) is an unsigned short to an
  `int` parameter; `unsigned short` drops the xor (`mov cx,[esi]; push ecx`, `mov bx,[mem]`); bare `mov cx; push
  ecx` or `mov dx; add edx,..; push edx` is `short`. With an int header, call through `((int (*)(unsigned
  short))f)` (0x4f5a80, 0x4fa700, cm.c 0x4c6f60, 0x4d3ea0 / 0x4d3d10, util_Random `unsigned short` in anm.h).
- **`and eax,0xffff` before the push (16-bit `cmp ax,0xffff`)**: an `int` parameter, `unsigned short` value; a
  `short` parameter leaves the upper bits undefined. f3init.c's cm_SendExSellObject declaration short -> int,
  @original (0x50f42c, 28 -> 12 code hunks). It also changes register roles (f6 0x4888c0).
- **Original caller `movsx ecx,WORD PTR [..]`, ours `mov cx,WORD PTR [..]`**: the caller TU must see int
  parameters, not `int d2_CreateWindow(short x, short y, short w, short h, unsigned int flags);` (d2.h K&R
  comment), @original where the lint cross-TU check needs it.
- **Callee masks its argument (`test si,si; and esi,0xffff`), caller `mov cx,WORD PTR ds:...`**: the TU saw
  `unsigned short f(unsigned short)`; `int` gives `mov eax,ds:...; and eax,0xffff; push eax` / `movzx`. Keep it a
  local @original declaration in the .c (the shared header cost six lint conflicts) (0x42ec00). A 16-bit return
  leaves the upper half (`mov ax,ds:global; pop...; ret`). path_Reverse's parameter to short alone cost 21 asm
  lines (its body truncates `last = p`).
- **`xor dx,dx; mov dl,al; push edx` into util_Random(unsigned short)**: `util_Random((unsigned char)cnt)` with the
  declared prototype; a cast to an int-parameter pointer gives `and eax,0xff` (init_NameBuildings). An `unsigned
  short` glyph index from an `unsigned char` local gives `xor dx,dx; mov dl,bl`.
- **Argument arithmetic narrowed to 16 bit**: a `short x, short y` prototype (d2_Unk4731d0); unprototyped `int
  f();` passes ints. X/y parameters stored into shorts are `int`; `short` moved a dead parameter slot.
- **Callers of one function passing differently**: the prototype seen before a definition decides calls inside
  and after it; a redeclaration just before the definition lets earlier callers keep another (C4028 is a warning)
  (d2_Unk46cda0: int for d2_CenterObj, short for its recursion and 0x46f840).
- **Thunk `mov ecx,[esp+8]; mov edx,[esp+4]; mov [esp+8],ecx; mov [esp+4],edx; jmp`**: K&R definition `int f(x,
  y, pic) int x; int y; int pic; {..}` with a `short` header prototype (0x46cd80). New-style short parameters give
  movsx, matching prototypes a bare jmp.
- **Pass-through wrapper reloading / restoring its stack slots before the `jmp`**: `unsigned char` parameters in
  wrapper and callee. A `short` parameter passes a short expression without extension.
- **Loop not strength-reduced** (`lea ecx,[i*8]; sub ecx,i; lea edx,[ecx*4+0xa]; push edx` per iteration, ours
  `add esi,0x1c` in a slot): a `short` parameter; menu.c's int-parameter d2_WinOpenPic let VC6 reduce `i * 0x1c +
  10`, the owner's `short x, short y` gave byte. No lever: loop forms, casts, `#pragma optimize`
  (ai_MessageFilterScreen 44 ins -> byte). Check the callee's real parameter types when IVs differ.
- **Row y `slot * 0x16 + 0x28` recomputed per call (lea chains, counters in memory), ours strength-reduced**: y is
  a short; call int-declared d2_WinOpenPicStyled through the cast macro `d2_WinOpenPicStyledS` (as sv.c)
  (ai_OpenItemTooltip -101 ins, stuck).

### Return types of callees
- **Callee returns in al, result passed straight on**: declare `unsigned int`; `unsigned char` adds `and
  eax,0xff`.
- **`test al,al` and the `return 0` without `xor al,al`**: the callee returns `char`: local prototype `char
  d3_LoadScene(char *, void *, void (*)(int,int,int))` (0x50d6d0; K&R `int f()` gave `test eax,eax` + `xor
  al,al`). A `char` function ends with `mov al,dl`, not `mov eax,edx` (f3init.c).
- **`test eax,eax` then `ja` / `jbe`**: the callee returns unsigned (`size_t` of `fread` / `fwrite`) and the check
  is `n > 0` / `n <= 0`; `== 0` / `!= 0` gives `je` / `jne` either way. Call through `((unsigned (*)())f)` or the
  unsigned view; call sites show which view the TU saw (vfs_fread).
- **Return-type-only cast macro, equality tests**: drop it. ahm.c's `#define AHM_FW ((unsigned (*)())vfs_fwrite)`
  / `AHM_FR ((unsigned (*)())ls_Read)` -> direct calls (ahm.h: vfs_fwrite unsigned @original, ls_Read int), same
  bytes (0x4ecf10, 0x4ed040), Q7-clean.

### Argument count different from the definition (K&R victims, Q7)
- **More pushes than the callee reads** (`ai_FillSaveSlots` from 0x5d4080 / 0x5d42b0 / 0x5d45d0 / 0x5d4920 pushes
  six, 0x5d3ab0 reads five): a cast keeps the count; the definition takes the extra parameter (`int reserved`,
  `(void)reserved;`, tagged); then frame, `add esp,0x40` and setup line up.
- **Stray arguments with a canonical prototype**: keep `int d2_GetViewWin(int view, int i);`, call `((int
  (*)(int, int, int, int *, int *, int *))d2_GetViewWin)(win, 1, 0x82, &a, &b, &c)` @original: still a direct
  `call 0x475ff0` with six pushes, byte-identical (0x5d4080 / 0x5d42b0). Same for sv.c's 4-of-8 call to 0x52e860.
- **5-parameter definition called with 2 / 4 / 5 arguments**: full prototype in the unit header, short calls
  tagged `/* @original: this call site pushes N arguments (0xADDR: push ...) against the N-parameter definition
  */` (d2_RadioSelect, d2_SetObjValue). To compile one called with 2 in the same TU, declare it unprototyped
  (`void f();`) before both (d2_Unk475cb0, d2_Unk472390).

### Evaluation order, nested calls, stack cleanup
- **Outer arguments pushed before the inner call**: the calls were nested in the argument list. `add esp,4` after
  each call, `and eax,0xffff; add eax,esi; push eax; push 0xc; call`: `f(g(12) + 1, g(n) + base)`; unmasked `add
  eax,esi` = int-returning callee (amt_CreateApplicant byte). `f(a, -1, g, (util_Random(n) & 0xffff) + k, own,
  0)` gives `push edi; push ecx; push n; call util_Random`; a separate `r = ...` adds a word (`add esp,0x1c` not
  0x18) and loses the sibling tail merge (0x50f0b0).
- **Inner call first, then all outer pushes**: store the inner result in a local first (anm_UpdateCat /
  anm_UpdateSheep; `smp = sb_GetSample(...); d3snd_Attach(smp, o, ...)`, 0x4c30e0 33 hunks -> byte). Constants
  pushed after an inner call: inner call as its own statement. `f(g(h(1)->x)->y, byteglob)` pushes the byte global
  first; temporaries put the inner calls first and merge the pops.
- **`add esp,8` after a 3-push variadic call**: `f(buf, id, g(1, k), s)` leaves the earlier-pushed argument on
  the stack (unk_4b6480.c 0x4b8950).
- **One `add esp` for consecutive cdecl calls**: next call's pushes before the previous pop (0x5cfc00: `call
  0x5176d0` with 2, `call 0x53dda0` with 6, one `add esp,0x20`). Count every push between two adjustments.
- **Call order**: `f(a) - f(b) + 10` evaluates b first; split into locals. For `add eax,edi`, hold the first in a
  temp and write `second + first`.
- **Argument load order**: right to left; for `depth + 1` from a volatile parameter a block local `d = depth + 1`
  restores it. `line = ReadLine(f); atof(strtok(line, " "))` needs the local.
- **`add esp,8; fstp [esp]`**: `a = fmod(a, ...); f(a)`, not `f(fmod(a, ...))`.
- **A trivial same-unit call sunk to its use** (original calls it first, result in edx across another known call):
  plain, cast and `(*f)()` calls sink; only a local function pointer stays (ai_InitShot 80 -> 12 ins; @original).

### Float, 64-bit and struct arguments
- **Float bit pattern pushed** (`mov eax,ds:addr; push eax`, no `fld` / `fstp [esp]`): K&R or int parameter:
  `((int (__cdecl *)())f)(bits)` with a float-returning wrapper; K&R `float f()` adds `fstp st(0)`. `pop ecx; ret`
  cleanup comes with it (0x5dbc20 -> 0x5dd090, matched).
- **Raw 4-byte float to an unprototyped (or int-declared) callee**: otherwise promoted to double; cast the
  pointer: `((void (*)(int, int, float))0x4c4ac0)(...)`.
- **Constants pushed through registers** (`xor ecx,ecx` / `mov ebp,1` / `mov eax,imm; push eax`) or ecx:eax
  across two calls: an __int64 built in a union / LARGE_INTEGER (`m.LowPart = k; m.HighPart = 1; f(p, 6,
  m.QuadPart, ...)`); an `__int64` constant is two immediates (0x49dd90, 0x48f010).
- **A float constant stored through the FPU (`fld [const]; fst s1.f` before a branch, `fstp s2.f` after the
  join) and a constant 1 kept in ebx (`mov ecx,ebx`, `mov [..],bl`, `push ebx`) where ours writes immediates**:
  the record's fields after the branch were written in BOTH arms (`s2.fvalue = 0.44f; s2.arg = 1;` at the end of
  each arm): cross-jumping merges the arm tails, and the doubled references make the 1 a register candidate.
  Chained / copied / local spellings of the constant all propagate (ai_DecideUpgradeBusiness 65 ins -> byte, one
  64-variant round over which fill statements sit in both arms).
- **4-byte colour pushed before the other arguments' FPU work**: by-value 4-byte struct, not `*(unsigned long
  *)&col`.
- **Grey value pushed three times from one `_ftol` eax into dd_RGB(unsigned char, ..)**: the expression written in
  each argument (a macro); VC6 CSEs it. No lever: named `unsigned char v` (slot), `int v` (allocation).
  dd_SplitRGB's out-parameters are `unsigned char r, g, b` (`mov eax,[r]; and eax,0xff`). 128 -> 2 ins; left:
  three split-piece reloads at one edge in ascending candidate id (c2trace) (d2_ShpBlit16HalfDark, open).

### Varargs, inline, intrinsics, asm
- **Varargs walk `mov reg,[r+4]; add r,4`, base in the preheader**: conflicting notes, measure: `mov edi,[ecx+4];
  add ecx,4` with the base `lea` after `nargs > 0` is `va_arg`, and `va_arg` (not walking `&n`) gives the
  original push order; vs `lea ebx,[ebp+0xc]; mov eax,[ebx+4]; add ebx,4`, counter in memory, is `char **args =
  (char **)&n; ... args[++k]` (d3t_MoveCameraPath, snow / rain setters), and an `int *` walk re-reading the
  argument after a call is `int *args = &n; ... args[++k]` (base `&n` in the preheader); `p = &n; *++p` or
  `va_arg` does not match.
- **`__inline` with `__asm`**: conflicting notes, measure: `__inline` functions containing asm were not inlined
  (asm addressing plain variables needs explicit pointer variables) vs an `__inline` `__asm` function returning
  in al is inlined (cpuid MMX test; VC6 knows what asm clobbers), and an asm block with four stack temps was an
  `__inline` function (parameters = temps, evaluated right to left).
- **`emms` without an ebp frame**: `void _m_empty(void); #pragma intrinsic(_m_empty)` by hand (no mmintrin.h);
  `__asm emms` forces ebp. All-`__asm` functions (MMX, `align 16`) match directly.
- **`cmp DWORD [x],1; jle` and a scale-2 `_pctype` load for `& 4`**: ctype.h's `isdigit(c)` (`MB_CUR_MAX > 1 ?
  _isctype(c, _DIGIT) : _pctype[c] & _DIGIT`); let ctype.h declare it. `unsigned short` __mb_cur_max gives `cmp
  WORD ...,2`, branches swapped (io_text.c 0x514360 byte).
- **Extra `call` before the callee-saved pushes**: a source call to 0x5de9c0 (`__chkstk`) duplicates the automatic
  probe (`mov eax,frame; call __chkstk`); remove it (0x5d34b0, sibling of 0x5d33b0, 268 -> 259 hunks).

### Other call-site shapes
- **`f(param_1, &param_1)`, param_1 used later** (0x5c4100: `push esi`, incoming slot `[esp+8]` passed, then `mov
  edx,[esp+8]`, `pop esi`, `mov [esp+4],edx`, tail jump; ours slot only, 35/48 bytes): try direct `&param_1`
  first; a `p = param_1;` local changes the prologue, not the tail. A pointer parameter used after a call with
  `&param` taken needs a local copy (0x4f8300).
- **Tail call rewriting only `[esp+4]` (`mov [esp+4],eax; jmp`) of a two-argument callee**: the caller has the
  second parameter and passes it through (d3snd_Stop).
- **Global scene pointer used twice**: read into a local, `s = unk_Ptrfd7c50;` (0x4f86a0).
- **Names-table pointer in a slot bumped by the row size beside the count**: `strcpy(names[cnt++], one)`;
  `strcpy(names[cnt], one); cnt++;` gives an offset copy (init_NameBuildings).
- **Table index helper**: `return p ? (unsigned)(p - base) / 0x194 : 0xffffffff;` in one expression.
- **Compares**: Ghidra's `0x10012 < v` is `v >= 0x10013` (`cmp v,0x10013; jl`), keep the original's constant.
  Wrong jcc / swapped `cmp`: `dcp try` a list of equivalent compare spellings. `int` global as `(g & 0xff)`: dword
  load + `and ecx,0xff`; `char` global: `xor ecx,ecx; mov cl,..`.
- **Returns**: `if (c) *out = 2; else *out = 0; return 1;` keeps separate return loads that two `return 1;` merge.
  Out-of-line `xor eax,eax` at the end: `if (x != 0) { ...; return r; } return 0;`, not an early return.
- **Pointer walk `for (c = ai_Chars; c < ai_Chars[0x300]; c++)` vs `mov edx,0x300 .. dec edx; jne`**: index loop
  over ai_CharsT (-117 ins) (ai_OpenItemTooltip, stuck).
- **fxch / fxch before an fadd**: float rounding of the product, `(float)(c->sk81 * (1.0 / 1008)) + factor`
  (ai_OpenItemTooltip, stuck).
- **Open: long `==` if / else-if chain, VC6 puts the arms after the compare chain (switch-like), the original
  inline behind an inverted last compare**: no structure moved it (ai_OpenItemTooltip 273 -> 53, stuck).

## Floating point
Operand order on the FPU stack, `fst` / `fstp`, float compares, constants, `_ftol`, copies through the FPU against integer moves, min / max.

### Operand order of commutative ops (ranks)

- **`fld a; fadd b` / `fmul` in the other order, no textual swap moves it**: order is an internal rank and follows
  the stack layout (fix layout first). Ranks move with parentheses (`(p->x)` is not neutral), a dead store, a named
  local, a `(float)` cast of a subterm. Of two named locals the one first appearing later (assigned last before
  the loop) loads first: `fld k; fmul r` needs k new after r. No lever: `+=` vs `a = b + a`, declaration order,
  swaps. (ai_ShotHitsGround: `p = from` after `d = to - from`, `t = 0` after `step = ..`, 12 ins -> byte)
- **Order no spelling changes**: copy one operand to another local (`t = z; ... (t * s)`); the original often
  reuses one float local for two roles; rename block locals to the other locals (`ren` in uv run vargen).
  (d3_LightAddObject)
- **Dead store as tie-break**: `rz = 0.0f;` / `s = 0.0f;` at block top, or `a = c;` before `b = c; a = c;`, ranks a
  variable for a later product. `(int)(a * u + b * t)` moved only on tie-breaks (dead `u = 0;`, pointer locals,
  parentheses), never on swaps.
- **`(float)` casts as rank movers**: around the first product of a sum (d3_VecAngle); on the double operand of a
  mixed product (d3_MatFromAngles, 2 casts of 7). Neither, nor the dead store, reaches fields read through a
  pointer (d3_MakeBasis). Tools: uv run respell rules fcast / deadrank, idiom fpu_rank.
- **Parenthesized member reads get their own rank**: `(drop->pos.w)` alone loads before the local `vel.x`
  (d3t_UpdateRain); `x = (p->x += 2.0)` differs from the bare form; `wz = (snow->windz);` moves an unrelated
  `fstp` two instructions. No lever: integer loads (d3_Viewport).
- **A CSE temp ranks above named locals**: `flake->sy1 = cy - y0 * f; ... flake->sy1 + s` loads the CSE first; the
  original needed `flake->sy1 = y = ...; flake->sy2 = y + s;`. A CSE'd product keeps the shared node's order
  (`o[1] = m[2]*m[5]; o[0] = m[3]*m[1] - m[2]*m[5];` m2 first, alone m5 first); stores through a `D3MATRIX *` field
  or `float *` kill the CSE.
- **`m[k]` operand order in products / dot products**: rank depends on the element spelling (`m[k]` / `src->_ij` /
  `((float *)src)[k]`); hill-climb each element's spelling block by block with dcp try and a generated variants
  file (d3_MatInverse 328 -> 67 lines); `((float *)p)[i]` moved what 3000+ permutations did not. Dot product:
  brute-force every subset of the six operands in parentheses, or parenthesize one term, `((n.x * v.x) + ...)`.
  No lever: term orders, swaps, global flips; 3072 variants per row (d3_GetBoxVerts, last bytes open).
- **A `fadd [mem]` moving in an unrelated statement**: ranks also decide how `a + b + c + mem` chains associate.
- **`fld b.gx; fmul c.gz` vs `fld c.gz; fmul b.gx` in a sum of products**: only term order and earlier code move
  it. No lever: all 2^n factor swaps, any grouping. (d3sm_ShadowCB, open)
- **Squares summed in another order (x, z, y)**: `(float)sqrt((double)(a*a + b*b + c*c))`; without the double
  cast the order is canonical (d3t_EmitterCB).

### Product and sum chains: association, factoring, folding

- **A chain in a load order no plain spelling gives**: nest it in that order, VC6 keeps nesting:
  `(((((S * area) * drag) * dens) * v) * v) * 0.5f` (360 unnested orders = one code; ai_StepShot); `k = spread *
  (speed * (2r - 1))` gives `fmul [field]` first, `y = dside.y + (dother.y + v.y)` the sum order (ai_ScatterShot 8
  ins -> byte); `f * (table[k] * (b / 255.0f))` gives `fild b; fmul 1/255; fmul table` (gv_GraveyardTurn).
- **A ternary factor** (`v > 0 ? 1 : -1`) is evaluated first and multiplied in with `fmul st,st(1)` where it sits
  (`((area * v) * v) * S`); a `(v)` operand in the chain is not neutral (8 ins); the sign's `0.0` / `1.0` were
  ai_FloatZero / ai_FloatOne (ai_StepShot). `s = x < 0 ? -1.0f : 1.0f; return s * x * x;` loads the constant first.
- **Original association of a sum of products**: split statements, `r = A + B; r += C; r += D;` (d3_Hermite);
  `v = M*d + pos` as `v.x = dot; ... v.x += pos.x;`; spline sums with `*= 0.5f` as its own statement.
- **Two fmuls (`fmul; fxch; fmul; faddp`) where ours factors to `(a + b) * 500`** (plus `fstp st(0)` after
  __ftol): two statements, operand on the FPU top first: `v = sum2 * 500.0f; v = v + sum * 500.0f;` (0x4ec3d0, 20
  -> 0 bytes; the other order leaves an `fxch st(1)`), `v = v * 500.0f; return (int)(v + sum * 500.0f) << 5;`
  (ahm_StepValue 42 ins -> byte). `(m._11 + m._21) * 0.0` is a vector-times-matrix with zero components; named
  temps (`t4 = m21 * 0`) stop the factoring and land in dead parameter slots.
- **Constants folded or moved to the end**: `x*c*c` and `d * 0.01f * 0.5f` fold (split / `(float)(d * 0.01f) *
  0.5f` keeps two `fmul DWORD`); `(d / x * 0.1f) * 32.0` keeps both; `(float)(x * -0.5f) + 0.25f` avoids fsubr;
  `(float)(a * c) * b` keeps the constant first; `(float)rand() / RAND_MAX * (r + r)` ends with `* (1/RAND_MAX)`
  unless written `(float)((float)rand() / RAND_MAX)`; `* 0.25f` after `/ RAND_MAX` merges into one `fmul`. `x *
  2.0f` is `fadd st, st`. `(w * 255.0f) / v` keeps fmul first (bare: `255 / v * w`); `(float)(1.0f / x) * s` keeps
  `fdivr 1.0`.
- **`fmul 1.0` in the original**: a literal `1.0f * x` folds; the constant went through a variable (`one = -1.0f`,
  `gy = -1.0f; ... gy * m._21`), giving `fmul 1.0; fsubp` / `fsubr`.
- **Int factors**: `(int)(k * C * data)` gives `fild; fimul; fmul`; `fild; fmul const; fimul` is `(int)((float)(k
  * C) * (int)data)`; `n * (x * 0.01)` gives fmul-then-fimul (0x4d3d10).
- **4x4 inverse**: write the determinant and each element in full; cofactor temps change the shape.
- **A hoisted `oy + 1.0`**: `h * cy + (oy + 1.0f)`.

### Values kept on the FPU vs stored and reloaded

- **A value loaded or kept "too early" across a statement boundary**: there was none; try `x = expr` inside the
  consuming expression before any allocation lever (snd_UpdateRain 33 ins -> byte, next three bullets).
- **`mov edi,[handle]` between `fsub` and `fmul`, `cmp edi,ebx` after `_ftol`**: the volume assigned in the macro
  argument, `SND_SET(h, name, v = SND_RAINVOL(..), 0, 0)` (33 -> 12; all four sites needed) (snd_UpdateRain).
- **`fst slot; fmul t` where ours has `fstp slot; fld slot`**: `(depth = 2600.0f - f) * t` (12 -> 4)
  (snd_UpdateRain). `p->x = (x = expr);` gives `fst x; fstp [p->x]` (bare: `fstp` + integer copy; `x = p->x +=
  expr`: `fst [p->x]; fstp x`); `d3_HalfW = hw = w * 0.5f; d3_CenterX = hw + x;`.
- **`fld depth; fmul tm` unmovable in one branch**: that branch has its own depth local (`dm`) as it has its own
  `tm`; a branch with its own copy of one local gets its own copy of the others (snd_UpdateRain).
- **`fstp g; fld e.demand; fmul g` vs the original's `fst g; fmul e.demand`**: no named g, `f + 1.0f` at both
  uses, the CSE temp gets the slot (ai_CommandDemand); same for an int `w - x0` instead of `lim` (d3s_DrawSpans).
- **`fcom; fst [p]` with the value kept and a fresh `fld [p]` next loop** (ours `fst tmp; fstp [p]; fld tmp`):
  `x = p->x = expr; while (x < -1.0) p->x = x += 2.0;` (d3t_UpdateSnow).
- **An atan2 result kept on the FPU while stored** (`fld st(0); fst a; fstp global; fld st(0); fsin; ...`, later
  `fchs`): a `double` local, `ta = atan2(..); g = a = (float)ta; sin(ta) ..; na = -ta;`; a float local reloads
  (d3_SetupFrustum 108 -> 98, stuck on a float `s` kept in memory). math.h sin / cos / atan2 compile inline.
- **`fld 1; fsub st,st(1); fstp f; fstp st(0)`**: `f = t / k; f = 1.0f - f;` (one expression gives fsubr); a scale
  kept with `fst [slot]` is its own local (ai_CannonSwitchAnim byte).
- **Kept on the FPU, small forms**: `t = c ? x : 1.0f - x` (not `t = x; if (!c) t = 1 - t;`); `float t = *time;`
  before `(unsigned)t` and `t - key` (`fld st(0)`); block-local `float f = rand-expr;` (popped after ftol); a field
  reused via `fld st(n)` = a scalar local copy (`wx = rain->windx;`); `x = min(255.0f, e)`; `d > (k + a) + b`.
- **Kept in memory where ours stays on the FPU** (`fst z` / `fmul z`, `fld z; fmul z`; a member loaded per
  product): `volatile` local / volatile read. `1.0f - r` with volatile r gives `fld r; fsubr`; `1.0f - *(float
  *)&r` gives `fld 1.0f; fsub [r]` (d3_Unk4591a0).
- **`fstp [d]; fcom [d]; and eax,0x4100; je`** where `max(r, d)` gives `fld d; fld st(1); fcompp` or `fstp
  st(3)`: `union { volatile float sum; float dist; } d; d.sum = ...; r = max(r, d.dist);` or `*(float *)&d`. No
  lever: `volatile float d` alone, `(void)&d`. (d3anim_MaxRadius 0x434d00, byte sibling `float d; r = max(r, d);`)
- **`r = max(d, r)` with `volatile float d`**: `fstp d` and two loads, but `fld d; fcomp st(1)` for the original's
  `fcom d`; a `double r` keeps d in memory, still loads it first, QWORD 0.0. Pointer base &v[1] (ours &v[1].z):
  `w = v + 1; ...; w++`.
- **16-byte locals, x / y on the FPU, z through memory**: `struct { float x, y; volatile float z; float w; } t`;
  without volatile, `D3VECTOR4` locals filled field by field then `s.x *= w` (frame equal; `D3VECTOR` copy 0x18
  short) (d3_TransformVerts). Three 12-byte vectors 0xc short = D3VECTOR4 (d3anim_CreateMorphAnim).
- **Splits that change store / reload or scheduling**: `t.x = t.x - a.x;` x / y / z then `- b.x` (not `(t.x -
  a.x) - b.x`); `h = a - b; ...; h *= 0.5f;` (fsub, fsub, fxch, fmul); separate `sx = 0;` statements (chained
  reverses the stores); `x = sqrt(..); x = x * k;` multiplies after the dead pops.
- **`fsqrt; fstp st(3); fstp st(0); fstp st(0); fstp [speed]`**: `s = (float)sqrt(...); speed = s;`
  (d3t_EmitterCB). Two roots stay separate when both reuse the same dx/dy/dz.
- **Squares folded in one by one** (`fld st(0); fmul st,st(1); faddp st(2),st` per term): one reused temp, `x =
  ..; sum = x * x; x = ..; sum += x * x;`; four locals square all at the end.
- **Cross product dotted with a memory vector, differences on the FPU**: `float c[3]`.
- **Pop points**: named locals pop at block end, temps / parameters at last use (`fmulp`). A once-used scaled
  parameter folds into its use; `size * sx` at every use (or an aggregate initializer) gives the early multiply.
  A scale for the last `fmul [mem]` is a float declared in the if block.
- **Unsigned ints into a macro using them three times**: pass straight; a float local spills (~1000 asm lines,
  d3fl_Draw).
- **`if ((g = f()) < best)`** gives `fld st(0); fcomp`, split gives `fcom`. `x = (float)(p->m - 2.0)` stores before
  the next call's cleanup. `PAR_RND()` vs `(float)rand() / RAND_MAX` moves a lea / fstp: try per component.
  `mov edx,[d]; mov [d],edx` is a second float local in d's slot.

### Float copies: fld / fstp vs integer moves

- **`fld [m]; fstp [local]`** (last store often past the next call's pushes) where ours uses `mov`: parenthesize
  the source, `v[0] = (bk->x0);` / `v.x = (cam->rotation.x);`. No lever: plain copies, casts, `+ 0.0f` at non-zero
  offset, comma macros, D3VECTOR / D3VECTOR4. (rd_AllBauplatzToSupermap, ai_CampaignKey1 byte;
  he_FindNearestSpying shape; deep-tools parensweep.py)
- **fld / fstp stores interleaved with the pushes of the call using the vector**: a comma macro as the argument,
  `D3T_VSET(v, a, b, c)` = `((v).x = (a), (v).y = (b), (v).z = (c), &(v))`.
- **Member at offset 0 (`[reg]`)**: write `(p->x * 1.0f)` or `(p->x + 0.0f)` for fld/fstp (unary + does nothing).
- **min / max / ternary copy via the FPU, `if` with integer moves**: `g = min(g, z)` gives fld/fstp and `and
  eax,0x4100; je`; `if (g >= z) g = z;` gives `fld g; fcomp st(1); test ah,5; jnp` with a copy, `z <= g` fcom / jp.
- **Copy form by pressure**: integer mov with a free register, fld/fstp in a register-starved loop. Loads first,
  then stores for 3 floats: `__inline` helper with float parameters. "Convert all, then store" int-to-float: float
  locals first. A float global stored as an immediate only if declared `float`.

### Compare idioms and flag tests

- **Flag test by spelling**: negated spellings give the C0/C2 idiom: `!(x < c)` -> `test ah,0x5; jp` / `jnp`,
  `x >= c` (and `x < c ? x : c`) -> `and eax,0x100; je`; `!(x >= c)` -> `and eax,0x100; je`; `x < c` -> `test
  ah,5; jp`; `!(x > c)` -> `and eax,0x4100; je`; `!(x <= 0)` -> `jp`, `x <= 0` -> `jnp`; `test ah,0x41` = `<=`.
  `x * 1.1f > d * 0.9f`, not the mirror. A select kept on the FPU across the next compare needs the original's
  condition spelling (0x5dd090).
- **One `>= 0.0` getting two flag tests in one function**: the test after `fcomp` is also VC6's own choice; do not
  chase it as a meaning error.
- **`test ah,0x41; jnp` vs `and eax,0x4100; jne` + reverse compare**: constant operand (0x4f9919, 0x4f993b) vs two
  variables (0x4f997f); with the value in st0, `fld y; fcomp st(1); test ah,5; jp`: an allocation hunk. A `<=`
  min accumulator keeps the later column on a tie.
- **`fcom mem` vs `fld c; fcomp st(1)`**: constant right gives `fcom [252.0]`, constant first (`252.0f < v ?
  252.0f : v`, `if (1.0f > x)`, `if (10000.0 > d) d = 10000.0;`) gives `fld c; fcomp` (0x50e330, 0x4d3ea0); there
  it also restored the `test eax,eax; setl cl; dec cl; and ecx,eax` guard after __ftol (as does a separate `v =
  (int)(...)` + `v < 0 ? 0 : v`). `v < K ? v : K` gives cmp / jl, store first. `a < b` vs `b > a` sets cmp order.
- **`fcomp [0.0]`**: `expr > 0.0f`, not `0.0f < expr`; DWORD only when the whole sum is float.
- **After a sqrt**: `(float)sqrt(d) < r` pops then `fcomp mem; test ah,5` (uncast: `fcompp; and eax,0x4100`);
  `(float)sqrt(a) <= b` gives `fcomp b; test ah,0x41; jp`.
- **`test ah,0x41; jp` in a sort guard** (ours `and eax,0x4100; je` from `a > b`): `!(a <= b)` via `if (level <
  lvl) continue; if (level == lvl && rate <= rate) continue;` (byte-neutral to -2 asm lines).
- **`mov [slot],1; fcomp; and eax,0x100; je` then `mov [slot],0`**: `w = (float)a >= b ? 1 : 0;`. In && chains
  `(float)a >= b` gives `and eax,0x100; jne`, `!(a < b)` not (0x4a1000, 0x4a1170).
- **Return-0 block after the computation, NaN passing both tests**: `if (f < 0.0f || f >= 1.0f) return 0.0f;`
  (0x4eb0b0 byte); the `&&` form puts it inline, one more instruction.

### min / max macros and clamps

- **A clamp re-evaluating its expression in every arm** (four `atof` per check, a redundant third test): nested
  windows.h macros, expression repeated: `min(max(0.0f, x), 1.0f)`, `max(min(base - d, 2), -2)`, `min(1.0f,
  max(-1.0f, d))`, `min(64, max(16, n))`; nesting / order set the layout (0x4ac200: `min(16000.0f, max(1600.0f,
  X))`, `dcp try` over 8 spellings). They keep the ternary's order and give mixed fcomp / fcompp (d3q_GetBounds);
  `max(0.0f, min(255.0f, expr))` inline stays on the FPU and tail-merges. A named clamped local changes the
  layout; inline in a call argument matched. `max(0, x)` vs `max(x, 0)`: try both.
- **QWORD `fcomp` with a DWORD `fstp` at the end**: a double expression, no `(float)`: `min(max(x, 0.0), 1.0)`
  (1472 bytes), `min(max(0.0, expr), 1000.0)` (`fld st(0)` copies).
- **`fcom 0 / and 0x4100 / jne; fcom 1 / test ah,5 / jp; fcom 0 again`** (stores in three tails): `f =
  min(max(f, 0.0f), 1.0f);` on a named local (ahm_AddValue 0x4ecdb0, byte); the sum in the macro gives `fld
  st(0); fcomp`, a variable `fcom`. No lever: `max(0.0f, f)`, `max(min())`, two `if (!(f > 0))` statements,
  `if (!(f > 0.0f)) f = 0.0f; else if (!(f < 1.0f)) f = 1.0f;` (12 hunks, test schedule left), plain `if (f >
  0.0f)`, `f <= 0 / f >= 1`, the `ai_FloatZero` / `ai_FloatOne` objects.
- **`and eax,0x4100; je` / `test ah,5; jnp` clamp**: `a = max(a, 0.0f); a = min(a, 1.0f);` as two statements;
  elsewhere the else-inline form `if (!(a > 0.0f)) a = 0.0f; else if (!(a < 1.0f)) a = 1.0f;`.
- **`call g; fild [a]; fidiv [b]`, two int homes, no float temp** (ours `fild; fstp` early, `fdivr`): `min((float)a
  / b, ai_FloatOne)` with int locals a = f(), b = g(); `if (r > 1) r = 1` gave `fld 1; fld 100; fmul st,st(1)`
  for min's `fld 1; fmul [100]`. No lever: one-copy spellings, casts, double, `/=`. (ai_CurseTargetWindow 28 ->
  byte)
- **A float kept in st(0) through a floor** (`fld st(0); fcomp 0.01q; jp; fstp st(0); fld 0.01q`, ours `mov
  [slot],imm`): `prog = max(0.01, prog);`. No lever: `if`, ternary, separate float / double, `max(prog, 0.01)`.
  (sv_RefreshPanel 118 -> 78)
- **Int clamp temp in the wrong slot / register**: `x = h * 60 + m; x = (int)MIN(MAX(lo, x), hi);`
  (gm_WorkMinutesBetween 68 -> byte); name the call's result, `stock - r` at both uses (gm_FreeCapacity byte); `m
  = ..; s->f = m; if (m >= lim) m = lim; s->f = m;`.
- **`fld corner; fld field; fcompp; and eax,0x4100; jne`** (min; max `test ah,0x5; jp`): `min(corner,
  ai_B[n].field)`, corner first; destination first gives `fcomp mem` (f3init.c 0x510770, 108 -> 48 hunks).
- **Accumulator compares**: `min(acc, v)` / `max(acc, v)`; `mn = p->x > mn ? mn : p->x` -> `fld p; fcomp mn; and
  eax,0x4100; je`; `p->x < mx ? mx : p->x` -> `test ah,5; jnp`; `!(mx > p->z) ? p->z : mx` -> `fld st(1); fcompp`;
  `m = x; if (!(m > y)) m = y;` -> `and eax,0x4100; je`, `<=` -> `test ah,0x41; jp`.
- **`max(r, d)` -> `fcom [d]`, `max(d, r)` -> `fld d; fcomp st(1)`** (d3t_GetBoundingSphere: removed the hunk but
  978 -> 995 elsewhere, reverted; fsqrt before the radius test open).
- **Bounding box**: uniform `min(mn.x, tmp[i].x)` / `max(mx.x, ..)` over an index loop. No lever: `t++` pointer,
  mixed ternary / if (d3anim_CreateMorphAnim).
- **`fild n; fld 64.0f; fcomp st(1); jp`**: `max(64.0f, n)`, not `min(n, 64.0f)` (`fcom [64.0f]; jnp`).
- **Int spilled to a stack slot before `fild`, then `fld a; fcomp st(1); and eax,0x4100; jne`**: stdlib.h `max`
  with the conversion inside, `x = max(x, j * 2500.0f);`; `if` or `fild [mem]` means no macro.
- **`max(t, 0.25)` path run through the next `fcom 5.0`** (ours jumps past it): the shared double object
  cm_DoubleQuarter (0x5eb9b0), not a literal. gm_StockPrice (12 -> 10, stuck): pooled M10s (cm_DoubleOne) changed
  nothing; two `push 0x64; push esi; call gm_ItemPrice; add esp,8; ret` tails ours cross-jumps (c2pass --skip
  0x1070a91d gives the original's layout); no return structure (30 variants) helps.

### Constants: float vs double, pooled literals, reciprocals

- **DWORD vs QWORD constant operand**: check the size first, it is a type fix: `0.001` `fmul qword`, `0.001f`
  dword; `< 0.2` / `< -1.0` / `< 100.0` QWORD compares (0x58fe10, 20 -> 4 hunks); `util_Random(n) + 10.0` `fadd
  qword`; `1.0 - (x + 0.2)` QWORD at 0x5eb7d0 / 0x5eb8a8 (amt_ExecutePriceMessage); `x * 0.00625f + 0.25` `fmul
  DWORD; fadd QWORD`; `acos(x) - 6.2831855f` uncast gives QWORD; `(float)(... * 10.0) - 5.0f` keeps 5.0 DWORD;
  `< 0.0` is `fcomp QWORD PTR ds:0x5eb830` (d3s_Update).
- **Read the size in .rdata**: d3_ScreenToUV (d3_interface.c) has DWORD 0x5eb780 = 1.0f, 0x5eb788 = 0.0f,
  0x5eb948 = 0.33333334f and QWORD 1.0 (0x5eb8a8) only for `r + s <= 1.0` and `1.0 - (...)`; f suffixes on the
  rest (1472 -> 1451 bytes).
- **`fadd QWORD` before `_ftol`** (ours `call _ftol; inc eax`): `(int)((x1 - origin) / axis + 1.0)` (d3s_Project).
- **VC6's own hoisted `fdivr [1.0f]` + multiply** for `x / inv` with an invariant divisor: no source literal; it
  reads the pooled 1.0f (0x5eb780) / 1.0 (0x5eb8a8) (also a `+ 1.0` rounding). No lever: a float object for the
  1.0f (the old stand-in ai_FloatOne), hoisted `inv = 1.0f / -y` (d3s_Project).
- **`fld [1.0]; fdiv [d.x]` hoisted, `fmul st,st(2)` in the loop**: `(v.x * 255.0f) / d.x`. No lever: `sx = 1.0f /
  d.x` (spills), unparenthesised, `v.x * (1.0f / d.x)`, `ai_FloatOne / d.x` (d3anim_CreateMorphAnim).
- **`x / 60.0f` as `fmul`** only for a double divisor `/ 60.0` (0x5dd090).
- **`(int)(v * 65536.0f)`**: literal `fld v; fmul const`; a global constant loads first.

### Int / float conversions, division, _ftol

- **`fld a; fdiv [r+..]` vs `fld [r+..]; fdivr a`**: `a / r->div` vs `a / (r->div)` or `*(float *)` (0x5dc210).
- **Int store before `fdivp`, original after**: `v - (v * z) / (r * r)` (d3sndw_Update 4 -> 2). No lever: orders,
  casts, temporaries, int local. Flip search: compare only the two instructions' order.
- **`fild a; fild b; fdivp`**: `(double)(a) / (double)(b)` (`(float)a / b` gives `fidiv`); `((a) * 1.0) / (b)`
  moves `fdivp` one or two instructions earlier (d3t_GetTriRange).
- **`fidiv` from a stack copy with a dead zero store**: `unsigned int` divisor. `fild; fidiv; fadd dword`:
  `(int)((float)a / b + 0.5f)`.
- **Unsigned to float `mov [esp+4],0; mov [esp],eax; fild QWORD`**: `unsigned int` (also `(float)(unsigned)x`, an
  int add then unsigned conversion); a zero in ebp all function is a local set to 0 at the top.
- **`fiadd` / `fisub`**: `(float)a + b`, `(float)g - intfield`. `fiadd [f]; call __ftol` per iteration is `f +=
  ai_Unk50c550(...)` with int f; `(int)` first drops fiadd (0x4c2d20, 287 -> 178).
- **`(int)(double)i` folds**; `(int)(double)(float)i` keeps `fild` + `__ftol` (0x4d4340).
- **Int forms in the same notes**: `x % 0x7fffu` -> `div`; `(unsigned int)rand > 7` -> `jbe`; `test; ja/jbe` on a
  vfs_fread / fwrite count: `(unsigned int)vfs_fwrite(..) > 0` (0x4ac8a0, 0x4acae0, 0x4adb20); `cmp eax,3; jb`
  first: `if ((unsigned)i >= 3) return 0;`; unmasked push after `util_Random(..) + k`: unsigned short parameter (0x4c3d00 / 0x4c4650), count
  pushes, they may be the outer call's (0x4ad0a0); a `short` parameter cast through the K&R cast leaves upper bits
  (`imul cx,cx,10`); zeroed `int m[6]`: `memset(m, 0, 24)`; one shared `call` / `mov [g],eax` after if / else:
  calls in both branches (ternary: `mov eax,..; jb`); `while` over `ai_Unk5309a0(fn, 0xfd6c6)` tests first.

## Types, widths, bitfields
`movsx` / `movzx`, signedness, char and short locals, bitfields and byte read-modify-write, shifts, multiply and divide idioms, casts, unions.

### A byte bit test turned into 0 / 1

- **`movsx r,BYTE [m]; and r,imm; shr r,n`**: `if ((char)x & imm) v = 1; else v = 0;` or `if ((char)f & 2) return 1;
  return 0;` (VC6 turns the if/else into and/shr after the sign extension; also on an int flag slot). No lever:
  casts, shifts, ternaries, bitfields, volatile, inline functions all narrow to a byte op; `(f & 2) != 0` and
  `(f & 2) >> 1` give shr/and. A dword load then `movsx r,al` means the flags sit in an int local first.
  (amt_IsColumnDecided)
- **`xor ecx,ecx; mov cl,[m]; mov eax,ecx; shr eax,5; and eax,1`** (zero-extended byte bit returned) where ours has
  `mov al,[m]`: `if (b & 0x20) return 1; return 0;` (likewise `x & 0x10`: `xor edx,edx; mov dl,[m]; shr; and`).
  No lever: `b >> 5 & 1`, `>>`/`&`, bitfields, `!= 0`, ternary, inline helper. (ch_AnimDone byte)
- **`and eax,1; cmp al,1`**: an int parameter tested as `(flags & 1) == 1`.
- **`mov ecx,[g]; and ecx,0x7f; cmp cl,1`**: an int global with `(g & 0x7f) == 1`.
- **0x11 as an immediate vs hoisted into a register**: `(b & 1) || (b & 0x10)` keeps the immediate; `b & 0x11`
  hoists it.

### Sign tests and signedness of compares

- **`test al,al; jns` vs `cmp` / `jge` on a byte sign test**: `x & 0x80` / `if (b & 0x80)` (also a `char fl` with
  `fl & 0x80`) gives `test al,al; jns`; `(char)x < 0` / `flag < 0` gives cmp/`jge` (0x5315c0). Conflicting notes,
  measure: one note has `!(fl & 0x80)` -> `test bl,bl; jns` and `(char)fl >= 0` -> `jge`; another has, on a signed
  char, `!(f & 0x80)` -> `test; js` and `f >= 0` -> `jl`.
- **A byte flag set/tested in a loop keeps 0x80 in bl**: only for `unsigned char` (signed: `test; js`).
- **`or bl,0xff; cmp [..],bl`**: a signed `char` compared with -1 (`unsigned char` + 0xff gives `mov bl,0xff`). On a
  signed char, `x |= 0x80` and `*(unsigned char *)&x |= 0x80` compile differently.
- **`movsx` / `and 0xff` + 32-bit `cmp` where the original has a byte `cmp dl,mem`**: the two operands differ in
  signedness (`unsigned char x == char y` promotes both): cast one side, or type the parameter / field
  `unsigned char` -> `cmp BYTE PTR [eax],dl` (0x4e6680, AMTMSG.office for 0x4e6710).
- **`movsx edx,WORD [mem]` + dword compare where the original has `cmp ax,[mem]`**: `unsigned short` vs `short`
  widens; `o->owner != (unsigned short)*c` keeps the 16-bit compare (amt_ExecuteShareMessage, draft -> behaviour).
- **Unsigned char compares**: `x < 0xff` is `cmp; jae`, `x == 0xff` is `jne` (the original mixes both);
  `unsigned char > 0` gives `test; jbe`, `unsigned > 0` `jbe`; `(unsigned char)b >= 1` on an int parameter gives
  `mov al,[b]; cmp al,1; jb`. `isdigit(p[1])` on plain char gives movsx.
- **`cmp bl,al; jae` where the original has `cmp eax,esi; jle`**: a "max so far" local is `int best` (the
  `unsigned char lvl` operand is zero-extended), not `unsigned char best` (amt_CheckOffices 0x4e9260,
  amt_ExecutePriceMessage 0x4e9660).
- **`jae` / `jb` / `jbe` where ours has `jge`**: sizes and byte counts are unsigned globals (0x520ba1); a timeout
  `if (now > t0 + 500)` (`cmp ecx,eax; jbe`) needs `unsigned` t0 (or the clock unsigned at the read) (0x521150).
- **A signed `cmp [rec],reg / jge` against the unsigned d3_Time**: `int now = d3_Time;` declared in the block.
  No lever: `(int)d3_Time` in the test (fixes the jump but hoists the load, keeps a register) (sv_ShowBubble byte).
- **`abs` from two bytes, `and edx,0xff / sub / cdq / xor / sub`**: `abs(a->rank - b->rank)` (stdlib.h intrinsic).
  No lever: `if (d < 0) d = -d;` or a ternary (`jns / neg`); a byte local for one rank goes to a stack slot where
  the original keeps it in cl (amt_ElectDeputies).

### movsx kept or folded on narrowed values

- **`movsx eax, bl; and eax, 8; shl eax, 8` from an int flags**: only a multiply before the mask
  (`(char)flags * 0x100 & 0x800`) keeps the movsx, as `shl; and 0x800`. No lever: every `(char)flags & 8` spelling
  (folds to `and reg,8`), a `signed char` local (`f = (char)flags` / `f = flags`). A signed char parameter gives the
  movsx bl but a 0x108 frame with byte reloads (0x478d19 `mov al,bl; and al,1`) and regresses callers compiled
  against the int prototype. (d2_MakeSurface, open: and-before-shift order; 0x478df1 / 0x478e1e `movsx eax,bl;
  and eax,8; shl eax,8; or al,0x40`, open)
- **`movsx ecx, byte [o+0x4b]` scheduled before an unrelated `and`, then `cmp ecx, 2`** where every char spelling
  gives `mov al,[..]; cmp al,2` (or `cmp byte [..],2`): no source found. No lever: casts, `>> 24` on int / unsigned,
  bitfields, static const / enum / local constants, int and char locals, `?:`, flag forms; arithmetic (`k - 2`,
  `k ^ 2`, `switch`) keeps the int but with sub / xor (init_AllocSceneBuilding, open).
- **`(char)flags & 0x40`** gives `mov bl,[..]; test bl,0x40`.
- **`mov cl,al; and ecx,0x80` (byte move)**: `(unsigned char)i & 0x80` on an int register; the same cast twice is
  shared as one `and edx,0xff`, so each further term needs another cast type (`(char)i & 0x40`) (qnt_InitTables).
- **Narrow locals narrow loads of wider globals**: `unsigned char bpp = c ? g16 : g8` (int globals) loads bytes;
  `int bpp = (unsigned char)(c ? g16 : g8)` loads dwords and does one `and eax,0xff` after the join (original).
- **`mov ebx,[esp+n+3]; and ebx,0xff` from a struct parameter before an unrelated call**: an `int` local assigned
  from the byte (`mask = color.a;`), not a direct use.
- **A byte shifted after `bio_rd_c`** must be `unsigned char` (cast plain-char struct fields).
- **A byte local compared with an int global spilled**: read the byte twice through the struct pointer.
- **`movsx eax,bl; and eax,8; shl eax,8; or al,0x40` from an int flags (the and BEFORE the shift)**: an if/else
  on the cast byte, `if ((char)flags & 8) caps = 0x840; else caps = 0x40;` (VC6 turns it into the
  branch-free and/shl and keeps the movsx, as for the 0 / 1 bit tests above). No lever: every expression form
  (`(char)flags * 0x100 & 0x800` keeps the movsx but shifts first), a ternary of either kind, `caps = 0x40; if
  (..) caps |= 0x800;`, split `&=` / `<<=` statements. (d2_MakeSurface byte; the bullet above that says open)

### Bitfields and byte read-modify-write

- **`or BYTE PTR [x],2` (byte RMW)**: `unsigned char` bitfields; word ops narrowed to a byte (`or/and BYTE PTR
  [p+1],imm` on a flag word) are `unsigned short` bitfields; `unsigned int` ones give dword ops; plain
  `unsigned char` flags give load/op/store. A word bitfield `&= ~8` is narrowed to `and BYTE PTR`. CHARACTER+0x8c
  is a 16-bit bitfield.
- **Two bitfields of one byte set in both arms** (`solid = 1; cast = 1;` / `solid = 0; cast = x;`) where the
  original does `and 0x3f; xor` and one shared store: assign cast before solid in the else arm (d3sm_ShadowCB,
  -28 ins). A struct local used only in a loop body, declared there, shares a dead array's slot.
- **`mov al,[loop]; and al,1; shl al,3; or al,0x10` then a plain byte store** where ours gives
  `((loop & 1) | 2) << 3` and an RMW: two bitfields (`f.u.bits.f08 = loop; f.u.bits.f10 = 1;`), with the other
  field stores permuted (720 orders, dcp try). No lever: `(loop & 1) << 3 | 0x10`, any spelling. Put the bitfield
  view in the unit (CHANIMFLAGSB in ch.c): in ch.h's CHANIMFLAGS it moved sv_t's ai_TutorialInitChapter2 off byte
  (declaration-set tie-break). (ch_AttachAni byte)
- **Two `|=` on one byte folded into one or** (`b1 |= 0x14; b1 |= 8;` -> `or 0x1c`) where the original does
  `or 0x14 .. or 8`: a store to another byte of the same struct between them (`f.w.c.b8 = 1;`). No lever: any
  bitfield view. A `memset(&f, ..)` plus a redundant `f.mid = 0;` stores a dword the original never stores: drop
  the field store (108 -> 88). Long store blocks: deep-tools tryclimb.py (dcp try hill climb) matched a 17-store
  else block in 3 rounds. (ch_ActTurn)
- **`setne; dec; and 8; or 0x10` vs `sete; lea [ecx*8+0x10]`**: byte fields are order-sensitive:
  `f.b1 = x ? 0 : 8; f.b8 = 1; f.b1 |= 0x10;` gives the first; the folded spellings the second.
- **RMW kept after a dword clear**: `*(int *)&f = 0; f.b[0] = 0x7f; f.b[2] |= 3;` keeps it; `|=` before the store
  folds. `memset(&f, 0, sizeof f); f.bit = 0;` keeps the byte RMW, `*(int *)&f = 0` folds it (see Copies).
- **Byte RMWs merged through one base**: explicit pointer locals (`v1 = v + 1`) stop it.
- **`add cl,cl; xor; and; xor`**: a 1-bit bitfield assignment; plain bitfield assignment gives
  `(v<<n ^ old) & mask ^ old`; Ghidra's `(BYTE)(a ^ b) ^ b` is `((a ^ b) & 0xff) ^ b` on unsigned ints.
  `x |= (x & 1) << 1` on a 2-bit bitfield: `and 4; add; or`. A bit copy between bytes is a struct of eight
  1-bit fields (`a.b2 = b.b4`).
- **One dword load and `xor edx,edx; mov dl,ah`** where ours loads each byte: byte bitfields
  `struct { unsigned char a:8, b:8, c:8, d:8; }`; plain `unsigned char` fields give a byte load each; one struct can
  mix both shapes (0x5d5f10). `GetBValue/GetGValue/GetRValue` give `mov al,ah` + byte load at +2;
  `(unsigned char)(c >> 8)` gives `shr`.
- **A by-value 12-byte flags struct whose middle dword stays in a register**: separate fields (a bitfield int at
  +4); `unsigned char b[12]` keeps it in memory.
- **Raw-cast bitfield access `(*(PARVFLAGS *)&v->flags).b4`** is part of the match (VC6 carries the stored byte
  across a chain). No lever: a union member `v->bits.b4` (reloads; par_Draw 855 B, d3_CloneObject 4 B,
  d3_Unk4550e0). Q4-clean: `#define PAR_VF(v) ((PARVFLAGS *)&(v)->flags)` used as `PAR_VF(v)->b4`; or move the
  cast into an inline accessor (D3_OBJBITS(o): `__inline D3OBJBITS *d3_ObjBits(D3OBJECT *)`, macro
  `(*d3_ObjBits(o))`, no byte
  changed anywhere).
- **`*(int *)&flags = 0` on a 4-byte flags struct (`unsigned char b[4]`)**: add an anonymous-union dword member
  (`unsigned int all`); `flags.all = 0` is the same store, Q4-clean (8 functions readable, no byte change).

### 16-bit values, 0xffff and -1

- **`and eax,0xffff; cmp eax,0xffff` vs `cmp ax,0xffff`**: on an `unsigned short *`, `if (*w != -1)` gives the
  first, `if (*w != 0xffff)` the second with the zero extension deferred to the argument push (0x50f62b /
  0x50f6cd).
- **0xffff in a register (`mov edi,0xffff; cmp word [..],di`)**: `*(unsigned short *)x != 0xffff`;
  `*(short *)x != -1` uses the immediate. `(unsigned short)x != 0xffff` keeps 0xffff in a register for several
  compares, `(short)x != -1` does not (0x5206b0, 0x520b80, 0x52c420). A 16-bit field compared with -1 gives
  `or ecx,-1`; the original's `mov ecx,0xffff` is the unsigned-short test (0x520de9).
- **`mov ebx,0xffff`, `cmp ebx,0xffff`**: a 32-bit variable initialised with 0xffff, not `w = -1` with an int (or
  -1 with a signed short: the pushes sign-extend). Zero-extend an unsigned word from memory into a 32-bit local so
  compares and pushes stay 32-bit (0x4c2d20).
- **Constants in a register**: -1 becomes `or reg,-1`; a bit inside a byte can take the byte form even for an int
  expression (`| 0x100` -> `or bh,0x1` with `char flags`, ai_MissionResultDialog; `| 0x80` on a renamed value ->
  `or bl,0x80`).
- **`unsigned short` callee result, short vs int local**: a `short` local keeps the value in ax (16-bit test /
  cmp); an `int` local gives `and eax,0xffff` right after the call and tests the full register. Read the record
  through `unsigned short *` (`xor ecx,ecx / mov cx,[edx]`); `short *` gives `movsx` (snd_PlayWorkerClick,
  f3snd.c 0x4c2c70).
- **`and ebx,0xffff` before each push vs `mov bp,ax; and ebp,0xffff` once**: `unsigned short r = util_Random(n);`
  gives the first, `unsigned int r` the second; the variation randoms are 32-bit (0x4c2f30, 0x4c2d20).
- **`mov esi,eax` where the original has `mov si,ax`**: `unsigned short r` from `unsigned short util_Random(...)`
  gives `mov esi,eax`; the original needs a wider type (tr.c's `int util_Random(unsigned short)`), which f3init.c
  cannot declare without a conflict (init_PlaceOffices, open: the one line left).
- **One extra `and eax,0xffff`**: a redundant mask, `ahm_Tab[t & 0xffff]` with `t = util_Random(8)`; `ahm_Tab[t]`.
- **`mov WORD PTR [slot],ax` at the assignment and `mov reg,DWORD PTR [slot]; and reg,0xffff` at the use**: a local
  `unsigned short` index, `(unsigned short)a` on an int local; stack slots are reused across disjoint live ranges.
- **`cmp WORD PTR [ebp+8],0` with the trip count in a separate int temp**: an `unsigned short` loop counter against
  an `unsigned short` bound.
- **`mov ecx,[mem]; cmp cx,6` (one 32-bit load reused)**: `(unsigned short)clock.hourWord > 6` on a 0x0e clock
  view (`int year; int hourWord; ...`). No lever: `*(unsigned short *)(p+4)` (Q4), a separate `unsigned short`
  extern (second load) (0x4f9c00).

### Multiply, constants, add / sub, division

- **`x * 3 & 3` becomes `neg; and 3`**: VC6 folds (3w) & 3 into (-w) & 3. Conflicting notes, measure: one note says
  it folds through a local copy and any cast or spelling of the 3 or the 3w, only `w3 - (w3 >> 2 << 2)` keeps the
  lea (not the single and) (d2_ReadBmpPixels, open); another says it folds unless `w*3` is a shared subexpression
  (named local `w3`). After `if (bi.biBitCount == 0x18)`, `w * (bi.biBitCount >> 3)` compiles as `w * 3`.
- **`and edx,0x28; add edx,0x253; add edx,ecx` where ours folds `lea edx,[edx+ecx+0x253]`**: a ternary of the two
  complete constants, `(c ? 0x27b : 0x253) + off` (two adds, v in the saved register). No lever:
  `(c ? 0x28 : 0) + 0x253 + off` (reassociated into the lea), a temp for the first two terms. (ai_FillSaveSlots
  103 -> 6, amt_SendOfficeNews, amt_SendOfficeLost, unk_4b6480.c ai_Unk4b82c0 byte; ord.c: 28 lines byte, 15
  functions: `(c[9] != 0 ? 0x11e : 0x114) + level`, `(c[9] != 0 ? 0x27b : 0x253) + job`, 0x25e .. 0x2a0)
- **`lea edx,[edi+edi]; imul edx,ecx` vs ours `imul; lea [eax+edx*2]`**: `(h << 1) * w`. No lever: every
  `h * 2 * w` spelling (d3s_AddShadow).
- **3x, 5x, 7x by adds**: the chain `t = x + x; x += t; use; x += t; use; t += x; use` (qnt_Map 524 -> 318 B);
  `x * 3` etc. give lea / imul. A value computed once above a few calls (`lea edi,[ebx+ebx*2+6]` before malloc)
  is a local assigned there (`cnt = w * 3 + 6`).
- **A volatile read once per use in a lea chain** (`mov eax,T; mov ecx,T; lea edx,[ecx+eax*2]; mov eax,T;
  lea eax,[eax+edx*4]`): three references, e.g. `(T << 3) + (T << 2) + T`; `T * 13` reads once. A subtraction in
  the same expression lets VC6 reassociate; the original assigns first (`t = ...; t -= last;`).
- **`add edx,0xffffffc0` where ours has `sub edx,0x40`**: `x - 0x40` with x a declared `extern int`; the address
  literal `*(int *)0x6f3610` gives the sub. Try the symbol form before a spelling round (0x5d5e90).
- **`add ecx,0xfffffffe / cmp m,x / jge` vs `sub eax,2 / cmp x,m / jle`**: the bound inline in the comparison
  (`if (h->size - 2 < x) x = h->size - 2;`, second read CSE'd) vs `m = h->size - 2; if (x > m) x = m;`. A `- 2` on
  a register operand is `add reg,-2` too; on a pointer-loaded int `sub` unless inside the comparison.
- **`cdq / and edx,0x7f / add eax,edx / sar`**: write `len / 0x80`, not `(len + ((len >> 31) & 0x7f)) >> 7`.
  `cnt % 16` on an int gives `and 0x8000000f / jns / dec / or / inc`; `cnt++ % 16` puts the increment in the
  loaded register and stores it before the branch.
- **`xor edx; div` where ours has `cdq; idiv`**: LCG `(seed >> 16) % 0x7fff % n`: write `% 0x7fffu`
  (0x4b3a10 / 0x4bb7d0).
- **Pointer differences**: `(char *)t - (char *)base >> 7` gives `sub; sar`;
  `(unsigned)((char *)p - (char *)base) / sizeof(T)` the unsigned divide-by-multiply.
- **A hoisted byte shift count (`mov dl,8; sub dl,cl`)**: `x >> (BYTE)(8 - n)`.
- **imul operand order**: `(short)w * (short)a` vs the swapped order swaps the operands.
- **`lea ebx,[eax+1]` where ours has `inc ebx` on a pointer parameter**: `memcmp(fmt + 1, ..)`, not `fmt++`
  (ai_CommandExecution).
- **`inc DWORD PTR [reg+0x248]` vs `mov eax,[..]; inc eax; mov [..],eax`**: `d2_LastWindowPtr->scrollY++` vs
  `d2_LastWindowPtr->scrollY = d2_LastWindowPtr->scrollY + 1`; the assignment through the global pointer (not a
  local copy) also makes VC6 reload the pointer after the store. Read each loop's increment separately
  (ai_CreditsScreen 0x5d7ba0 byte).
- **`ret = global = -1;`** gives `or eax,eax` after the store.
- **`888 - R*100 - G*10 - B` as `(R*10 + G)*10` (lea chain, `mov esi,888` first) where the original has `G*-10`
  (`mov esi,edi; neg esi; shl esi,2; sub esi,edi; add esi,esi`), `R*100` alone and `add esi,888` last**: VC6
  merges two constant products of one sum whatever the spelling (term order, parentheses, `(8 - X) * k`, unary
  minus, splits into statements, int/unsigned/short/char operands and casts: all one code, toys and dd_SetMode);
  the neg form appears only when one product is opaque to the merge - a narrowing cast `(short)(R * 100)`,
  `& 0xffff`, a float multiply, `R` written as shifts - each with an extra instruction. No lever found
  (dd_SetMode, open).

### Globals: width, type, unions, volatile

- **`mov si, word [g]` before a loop, `cmp si, 0xb` inside** where ours has `mov esi, dword [g]`: read `g` at its
  use, not through a local `d = g;` (VC6 hoists it itself, as a word; the local also cost a register)
  (amt_ClearOffices).
- **The clock's type decides the code**: renaming the int alias 0x6333cc to `volatile unsigned long d3_Time`
  (cm.h/anm.h) moved sv_Unk5263a0 (0x5263a0) 1365 -> 1458 differing bytes (`(unsigned)(clock - last) * 0.4`);
  check count and dcp check --against stayed clean, unitdiff showed it. Reverted: a shared-header decision.
- **A union member read reallocates the function**: `snd_AmbientOn.on` (char member of a union global) made VC6
  push ebp and move every stack slot; the byte alias `extern unsigned char snd_AmbientOnByte; /* 0x6f0b60 */`
  keeps the bytes. Check with asmdiff after every type change.
- **A char flag read as a word**: `mov eax, ds:flag` then `and eax, 0xff` before a dword store, while other
  functions use it as a byte: a union view `{ char on; unsigned int word; }`, the word member only where the word
  load happens (f3snd.c G16AMBIENTON).
- **Read as a dword, written as a word**: `union { int seed; unsigned short seed16; }` instead of
  `*(unsigned short *)&g` (both widths keep their code, Q4 clears).
- **A spill of `best` reloaded as int (`bi = *(int *)&best;`)**: Q4-clean `union { float f; int i; } best;` with
  `best.f = 0.0; ... bi = best.i;`. No lever: `bi = 0;` (folds, spill gone).
- **`(*(volatile short *)&dm_S.x)` casts stopping a CSE**: an anonymous-union view `union { struct { short x, y,
  z; }; struct { volatile short vx, vy, vz; }; };` and `dm_S.vx` / `dm_S.vy` at those uses: same bytes, Q4 clean
  (di_UpdateButtons). Prefer it for a field that must be re-read at every use.
- **Word load/store of a global at the tail of a byte block** (`mov dx,WORD PTR ds:X`, 0x520cf9 / 0x520d14):
  `char ai_Unk75e130[14]` in cm.h makes 0x75e13c a `short`; an `int` view writes 4 bytes.
- **`movsx r, WORD PTR ds:..` on screen metrics**: 0xfb9070 (height) / 0xfb9072 (width) are shorts, read
  `*(short *)0xfb9072` (0x5d6fa0: 161 -> 91 differing B).
- **`mov BYTE PTR ds:0x6f25ac, 1`**: a char global; `U_I` stores give a dword store; use U_B (0x5d7e90, 63 -> 0 B).
- **`mov DWORD PTR [o+0x48],0x2000047`**: VC6 merges adjacent byte stores `o[0x48]='G'; o[0x49]=0; o[0x4a]=0;
  o[0x4b]=2;` into one dword; the original wanted the single typed store (0x50d100).

### Struct layout, offsets and Q4-clean views

- **Field offsets a few bytes off (`[eax+0x60]` vs `[eax+0x5d]`)**: a local or shared view of a packed struct
  needs its own `#pragma pack(push, 1)` (ai.h wraps AIBLD / AIOBJ in pack(1): `id` at 0x01, `objId` at 0x02;
  unk_58bbd0.c's AIBLD mirror; G16BLDVIEW `char pad61[0x61]; char *object;` 0x64 vs 0x61; pack(2) puts
  `unsigned short owner` after `char pad0[0x27]` at 0x28). Splitting a pad array into `char pad[0x34]; int field;`
  keeps the other offsets only when packed. A 17-byte record array (`shl ecx,4; [ecx+eax+base]`) needs
  `#pragma pack(1)`.
- **`mov eax, dword [key]; mov cl, ah` vs `mov ax, word [key+1]`**: VC6 reads a 4-byte object through one dword; a
  3-byte object (pack 1, `{ unsigned char r; unsigned short gb; }`) loads the word member as a word. When the
  original never touches the last byte, drop the pad byte (D2COLKEY: three shape grabbers byte).
- **Q4 deref-cast spelled as pointer + `->`**: `(*(T *)(base + off)).f` == `((T *)(base + off))->f`,
  `((int *)(p + 0x45))[i]` == `*(int *)(p + 0x45 + i * 4)`: identical code incl. the byte-offset addressing that
  `arr[i]` loses (d2.c d2_CreateScrollButtons / d2_ScrollWindowBy / d2_CloseView / d2_GetObjPicSize, 2610/4664
  unchanged).
- **A raw store through an offset**: `((D2OBJ *)((char *)d2_Objs + off))->id = obj;` is byte-identical to
  `*(int *)((char *)d2_Objs + off) = obj;`. No lever: `o->id = obj;` (reuses the live pointer, 105 -> 124)
  (0x46e710, d2_RemoveObj).
- **A byte-pointer walk of an int array**: `int win = (int)wn->objs; ... win += 4;` and `int *p = wn->objs; ...
  p++;` give the same bytes (d2_ResizeWindow 0x472800), so Q4 is free there; the D2WO byte-offset macros keep their
  form (d2_CreateScrollButtons / d2_ScrollWindowBy regress with typed d2_Wins[win] or a typed pointer).
- **Q4 without an includable struct**: typed pointer views (`unsigned short *fl = (unsigned short *)(ent +
  0x8c);` `*fl &= 0xdffb;`, `unsigned char *flagByte`, `int *ref = (int *)(ent + 0x12c);`) keep the bytes if each
  view is assigned right before its first use; assigned after the allocation they hoisted `lea ebx,[esi+0x8c]`
  and swapped edi / esi (0x51e750).
- **A float written as its bit pattern (`*(int *)&x`)**: a unit-local `typedef union D3IFBITS { float f; int
  bits; }` with `((D3IFBITS *)&x)->bits` (d3_WriteMap, d3_SaveScene, D3IFREGION / D3IFMAP); D3_SCENE3C8 avoided
  the same way through the typed D3IFSCENE view (mapSize 0xb4, events 0x3c8).
- **`and eax,0xff; shl eax,3; mov ecx,[eax+base]` where ours has `mov ecx,[eax*8+base]`**: a char-offset index
  `*(int *)((char *)table + i * 8)` (Q4) gives the shl, typed `ai_ScreenModes[i].w` cannot: a shared-header view
  decision (0x5d5640, open).
- **Functions defined earlier in the same file** change register allocation of later callers.

### Indexing and address precomputation

- **`lea eax,[base+idx*4]; lea edi,[eax+0x23]` where ours has `lea r,[base+idx*4+disp]`**: a named record pointer,
  `bt = &ai_BTypesT[b->type]; ... bt->slot[j]`. No lever: `w = bt->slot; w++`, `ai_BTypesT[b->type].slot[j]`
  without the local (two IVs) (ai_HasThreeStockedBuildings byte). Conflicting notes, measure: another note says a
  local `&arr[i]` into a global array is folded back into `[i*4+arr]` and writing `arr[i]` everywhere gave the
  original's `lea` precomputation.
- **2D arrays through index locals** (`map->tiles[ty][tx]`) keep `base + idx*size` as a CSE with the field as
  displacement; a `t = &tiles[..]` pointer folds the offset into the lea.
- **A real member array (`ch->contacts[i]`)** folds the offset into each lea; a cast macro gets CSE'd into a
  register.

## Copies, fills, strings
`memset` / `memcpy` / `strcpy` / `strcmp` inline forms, `rep movs` / `rep stosd`, struct assignment against field copies, string loops.

### Inline string copies, compares and lengths

- **Offset copy `inc eax; sub edx,eax; mov cl,[eax]; mov [edx+eax],cl; inc eax; test cl,cl`** (one running source
  pointer): conflicting notes, measure. One note: not strcpy (strcpy and `*d++ = *s++` give the two-pointer
  `inc ecx / inc edx` loop, `out[i] = s[i]` walks the destination); write `s = src; off = out - s; do { c = *s;
  s[off] = c; s++; } while (c);` (init_BuildingKindName byte). Other note: it is VC6's inlined `strcpy`; an indexed
  `dst[i] = src[i]; i++` gives a counter loop, `*d++ = *p++` can already produce it (ai_CalcMeisterRaeuber
  0x482f00: 321 -> 207 code hunks with `strcpy((char *)msg.arg, src)`).
- **Which strcpy shape follows from the arguments**: `strcpy(tok[n++], t)` with a plain counter gives the
  two-pointer copy (`mov edx,esi; mov ecx,eax; add esi,0x10; ...`); a hand-advanced row pointer (`q = p; p +=
  0x10; strcpy(q, t)`) gives the `sub edx,eax` offset copy (main_BuildVersionString 13 -> byte). For a two-pointer
  original: a `char (*)[0x60]` row view written twice (`val = (int)((char (*)[0x60])v->data.s)[idx];
  strcpy(evt_StrResult, ((char (*)[0x60])v->data.s)[idx]);`). No lever: `v->data.s + idx * 0x60`. A
  `char (*str)[0x60]` member in evt.h's union cost ai_TutorialInitChapter2 (sv_t) its byte: keep the cast local
  (evt_Eval).
- **`repz cmpsb`**: inlined `strcmp("literal", buf)` with the literal's length (operand order matters); the same
  source with the compare swapped decides inline strcmp vs a `_strcmpi` call. `memcmp` with a size that is a
  multiple of 4 gives `cmpsd`.
- **`or ecx,-1 / repnz scas / not ecx / dec ecx` inline strlen**: needs /Oi for the TU. No lever:
  `#pragma intrinsic(strlen)` before or after <string.h>; no per-function switch. A global `char[]` (not a literal)
  is why `strlen` of it is computed at run time.
- **`len = strlen(s) + 1; d->n = 0;`** stores the zero left by `repnz scasb`; the store first uses the zero reg.
  `strcpy(dst, q + 1)` after `if (strlen(q + 1))` CSEs `q + 1`; `memcpy(dst, src, strlen(src) + 1)` gives
  `repnz scasb; rep movsd/movsb`.
- **`strcat(buf, tmp)` in a loop**: the pointer increment after the strcat schedules like the original (before it,
  it moves between the scas pairs).
- **Inline strcpy / strcat**: strcpy is a byte loop, strcat `repnz scasb`.
- **A byte copy loop `mov cl,[eax]; mov [eax+esi],cl; inc eax; test cl,cl; jne` (esi = dst - src, ours
  `[esi+eax]`)**: `strcpy(dst, src)`, not a hand `(*d++ = *s++) != 0` loop (every hand form gives `[esi+eax]` or
  another shape) (d3d_LoadTexture).
- **Three loads at function start**: `char tag[] = "SHAPBANK"; memcpy(bank, tag, 8)`. Also: a tick difference
  compared with 1 matches only with the previous-tick variable unsigned; `util_Random` result kept in an `int`;
  `(uint + ushort) % 3` is `xor edx,edx; div`.

### Constant fills: memset against element loops

- **`mov eax, edi` (copy of a register holding -1) then `mov [g], eax` runs, interleaved with field stores**: an
  inline `memset(a, 0xff, 20)`: `memset(p.slot, 0xff, sizeof(p.slot))` per panel. No lever: five `slot[i] = -1`
  (74 ins) (ai_OpenPokerWindow byte).
- **`lea edx,[edx+X]; mov edi,edx; rep stosd` where ours has `lea edi,[edx+X]`**: the plain element loop
  `for (i = 0; i < 8; i++) c->relatives[i] = -1;`, not `memset(c->relatives, 0xff, 32)` (the loop uses the
  address twice). No lever: casts, `&..[0]`, a pointer local, the memset result used, 359 statement orders
  (ahm_CreateAlchemist 3 -> byte). The reverse (`lea edi` directly) is a memset where ours has a loop.
- **The index variable's name** does not matter unless it is the enclosing loop's neighbour (ai_HelperBook: k
  fine, j worse).
- **Sweep**: `python tools/sweep/fillloop.py` (every constant memset of a below-byte function as the loop, dcp try;
  `--zero` for zero fills, `--widths int,char` for the element type); rerun after base work on a unit. -1 fills:
  hmt_Create, he_StartGang byte; gv_RaiseDead 9 -> 4; ai_UpdateProduction 33 -> 24 (first of four fills);
  gm_CreateCharacter, ai_BuildTownMap, ai_HelperBook, ai_MercenaryBook a few ins each.
- **fillloop.py `--zero --widths int,char` leads (not applied)**: each says the fill was not a memset of that shape
  (an initializer, a field-by-field clear, a loop over the real element): io_CharacterNameMarkup 2 -> byte (`tmp`
  clear; its hunk is a `[eax+edx]` / `[edx+eax]` tie-break in a later strcpy), io_CharacterName 8 -> 4 (four
  char loops), ch_LoadLowAnims 28 -> 9 (`memset(&f, 0, sizeof(f))`), d2_ResizeWindow 32 -> 24 (`desc`),
  ai_CalcMeisterWache 100 -> 86 (second `msg` clear), ai_FillLoversTab 55 -> 45 (`rows`), gv_AllocGraveyard
  162 -> 116 but shape -> draft.
- **`or eax,-1; mov ecx,0xb7; mov edi,base; rep stos dword` then `mov ecx,8; rep stos dword; stos word`**:
  constant-size `memset(init_ObjectUniverse, -1, 0x2dc); memset(init_BuildingUniverse, -1, 0x22);`. No lever:
  pointer loops with a tail `*(short *)q = -1` (`mov ds:0x76fa80,ax`). A `char` return + `return 0;` adds
  `xor al,al` (0x50d460).
- **Four stores with the offset folded into the address (`[ecx+0xf88a44]` .. `[ecx+0xf88a50]`)**: four
  `arr[i].slot[j] = -1` statements; memset with a constant size gives `lea reg,[base]` + stores through it. Measure
  both: 2361 -> 2334 B but 172 -> 244 lines (ai.c 0x4854f0).
- **Inlined memset shapes**: `or reg,-1` + four dword stores is `memset(p, 0xff, 16)` (hv2: 626 -> 172 lines);
  `memset(&arr[n], 0, 0x11)` four dword + one byte store through one pointer (d2u, f1 16 bytes);
  `memset(&tab[i], 0, sizeof tab[i])` keeps a zero register from being hoisted over an earlier test.
- **The inlined clear vanishes after a store to its base**: `memset(p, 0, 0x24)` then `*(int *)p = -1` (p a global
  array) loses the whole rep stos. No lever: `volatile`, `int *p2 = (int *)p`, a struct view. Store through a second
  extern for the address (`extern int ord_MessageRecordEnabled; /* 0x91b480 */`) (ord.c 0x4f82a0 byte).
- **`rep stosd` of 9 dwords over an array whose code uses 4, plus locals spilled right after they are set**: the
  ids are ONE record (form, win2, slider, back, extra, bars[4]) filled with -1; map the range
  [base, base + 4 * ecx) onto the later stores to name each member (memset and the loop are one code). The
  research slider is its own local (frame 0x23c -> 0x240); d2_SetForm(form, 2) in both arms of the owner test
  (ai_AlchemistWindow, stuck at 88 -> 6: `[base+index]` vs ours `[index+base]` in the first of two panel copies).

### Zero stores and clears

- **Zero stores of a block in their own register (fresh `xor esi,esi`)**: a memset over them:
  `found[0].type = 0; memset(&found[0].vol, 0, sizeof(found) - sizeof(int));`. No lever: six `= 0` lines (one zero
  web, 151 -> 14 only) (d3sndw_Update).
- **Several late zero stores from one `xor eax,eax`**: `memset(&s, 0, 12)` (the globals are one struct); separate
  `= 0` stores keep 0 in a register all function. `*(int *)&f = 0` can fold into a zero kept in ebx;
  `memset(&f, 0, 4)` avoids it.
- **`mov [f],0; mov cl,[f+2]; and cl,0xfe; mov [f+2],cl`**: `memset(&f, 0, sizeof f); f.b[2] &= 0xfe;` (the memset
  expands late). No lever: `*(int *)&f = 0`, a union-member store (fold to one byte store) (d3_DeflateObject
  0x43da00; ch.c ch_UpdateFades, ch_StartFade).
- **Ascending zero stores of a small local array**: six single statements. No lever: a loop (`rep stosd`), the
  chain `fl[0] = fl[1] = ... = 0;` (reverse order), `fl[6] = {0}` (0x4e8160, 96 -> 62). A second array
  initialized at declaration is stored before the zeroing; move its values to statements after it.
- **A 0x1c-byte zero buffer as 7 immediate stores** (0x51ad80 at 0x51aea1, open): no lever: `int zero[7] = {0}`
  (rep stosd ecx=6), `char zero[0x1c]` + memset (ecx=7), a loop, two arrays, `struct { int a..g; } = {0}` (ebp).
- **16 zero stores from a zero register, the first four from `eax` and earlier**: `objs[16] = {12 explicit
  zeros}` (explicit part from a zero register, tail separately). No lever: `= {0}` (rep stosd), 16 explicit zeros
  (all after the first test) (d3t_MoveCameraPath).
- **Where the clear sits**: `memset(buf, 0, sizeof buf)` inside the branch gives `mov ecx,N; rep stos` there,
  `= {0}` at the top. `char buf[N] = "";` and initialized local char arrays are filled at function entry, or where
  their inner block starts, in declaration order.
- **`= {0}` vs memset**: zero-initialized local vectors are stored in declaration order; `memset(&s, 0, sizeof s)`
  is one `rep stosd`, `= {0}` stores the first dword separately. `D3TEXFLAGS f = {0}; f.b[0] = 0xff;` gives a dword
  zero store plus a byte store to the same slot.
- **0x22 `mov BYTE PTR [esp+X],imm8` stores**: the initialised array is the working array (`char cls[0x22] =
  {0, 0, 3, ...}`). No lever: `tab[0x22]` + `memcpy(cls, tab, 0x22)` (0x50f910).
- **`memset(a, 0, n); a[i].f = c;`** gives `rep stos` plus constants stored between the pushes; an array
  initializer gives single zero stores. `q = &arr[id]` for several fields gives `lea esi,[eax*4+base]`.
- **The DDBLTFX extra store**: `memset(&fx, 0, sizeof fx); fx.dwFillColor = 0; fx.dwSize = 100;`.

### Generic inline memset and asm macros (D3_ZERO, AI_ZERO, copy twins)

- **ebp frame, size and pointer in ebp-4 / ebp-8, byte-align / `rep stosb` / `rep stosd` / `rep stosb`**:
  `D3_ZERO(p, n)` (d3.h asm macro); `memset(p, 0, n)` gives `mov ecx,n/4; xor eax,eax; lea edi,..; rep stosd` in
  an esp frame (0x5d8360: 113 -> 24 -> 0). Grep `8d 0c 7f 83 e1 03` (lea ecx,[edi+edi*2]; and ecx,3) for D3_ZERO
  copies (329).
- **Generic memset with byte broadcast (`mov ah,al / shl eax,0x10 / ...`) and `rep stosb / rep stosd / rep
  stosb`**: the intrinsic expanded without folded arguments (ebp frame); `memset(s, 0, CONST)` folds to unrolled
  dword stores (snd_AddToQueue 0x4c0230, escalated).
- **AI_ZERO / D3_ZERO on a global array**: only the `_AT` form (`offset table`); the pointer-temp form adds a local
  (0x4e5cd0 / 0x4e4060). Ghidra shows them with a constant size as three zero loops (counts 0, n/4, 0) and the
  original has an ebp frame (unk_4adf40.c 0x4af090 with AI_ZERO). Identity matrix: `D3_ZERO_AT(m, 64)` then 1.0
  to _44.._11; matrix stores per row `_11, _12, _13, _14=0`.
- **Copy twin (rep movsb / movsd)**: pick the macro from the `mov esi` line: an immediate (`mov esi, OFFSET g`), a
  variable, or all three temps (hst.c G12_COPY / _P / 2).
- **Generic unaligned block copy, four operand shapes** (size/dest through temps and source direct; both through
  temps; both direct; dest direct): a unit that cannot include d3.h gets the macros locally (bmp.c's BMP_COPY;
  ord.c ORD_COPY / ORD_COPY3 / ORD_COPY_AT matched 0x4f9190, 0x4f9220, 0x4f9710). The inline asm forces an ebp
  frame.
- **Inline-asm copy macro temps**: declare size, src, dst in that order; VC6 does not substitute a plain variable
  into the asm, and loop-invariant temps are hoisted.

### Struct assignment against field copies

- **Loads first, then stores (vectors)**: copy field by field; a struct assignment gives lea-pointer copies.
  `slot->dir.x = ldir->x; ...` gave the original's interleaved loads and frame (d3s_Project 282 -> 139). Byte
  copies with both loads (second zero-extended) before the stores go through int locals.
- **"Load the pointer once, store twice"**: a pointer local or an 8-byte struct copy; field code through
  `t->v[i]` reloads after each store.
- **Interleaved load / store pairs of an odd-sized block (7 / 9 / 10 / 12 / 15 bytes)**: a struct assignment of a
  `struct { char b[N]; }`; field copies load everything first (cm.c 0x4c58a0..).
- **`lea ecx,[esi+4]` then `[ecx]` / `[ecx+4]` / `[ecx+8]`**: the 12-byte aggregate assignment `m.proto =
  s->grid;` (AHMGRID union); it also keeps the id out of a callee-saved register. No lever: `m[3] =
  s->grid.w[0]; ...` (folds to `[esi+4]` ..), `int *w = p + 1;` (ahm_SendCreateAlchemist 0x4ec490).
- **14-byte clock copy, `mov edx,[ai_Time]` + `lea ecx,[r+0x52]` + moves through ecx**: one struct assignment of a
  pack(2) `struct {int; unsigned short; int; int;}` from the extern `ai_Time` (0x770ae0): `r->due = ai_Time` /
  `*(HETIME *)(r + 0x52) = ai_Time;`. No lever: four `U_I(r + 0x52)` / `U_I(0x770ae0)` stores, a local pointer
  (0x5ae870; he.c 0x541200, 0x543020, 0x543180, 0x5436a0, 0x542960). When the original copies field by field from
  `ds:0x770ae0` (0x5b5e80), field stores are right. With default pack(8) the tail is a DWORD read that overshoots;
  pack(2) copies 4/4/4/2.
- **`TMX(msg) = NOWX`**: `msg.time = ai_Time` is byte-identical and Q4-clean; try it before the AITIME/AITIMEX
  header work (0x4b0d40, 0x4b0fd0).
- **0x18-byte record copy (rep movs 6 dwords) after a search**: two pointer locals `p1 = &amt_Table[i];
  p2 = &amt_Table[j];` (0x4e6c50, 60 -> 0); indexed `amt_Table[i].owner` recomputes lea/shl/lea per use.

## Declaration set, headers, unit order
Effects from outside the function's text: headers, a struct or extern that moves another function, callees defined above or below, unit boundaries.

### Tie-break sensitive functions (sv_t's tutorial chapters)
- **ai_TutorialInitChapter2 / 3 (unk_575860, sv_t) flip on the unit's declaration set alone** (byte in 17 of 32
  headers samples): a struct of a new name, a struct body in sv.h (any layout / name / position),
  `sv_QuickJump.active` in ai_TutorialTick, a true prototype `snd_LoadVoiceBank(char *name, int a)` above them, a
  bare `extern struct X v;`. Kept by: sv.h holds only `typedef struct SVQUICKJUMP SVQUICKJUMP; extern SVQUICKJUMP
  sv_QuickJump;`, the body in sv_quickjump.h for the units reading its fields. The build clears Chapter3's @deep
  note when it is byte; restore it from git when it falls.
- **Labels and locals are symbols**: removing two goto labels in ai_CampaignStep (byte, `goto started` / `goto
  count` tails written out) dropped ai_TutorialInitChapter3 (0x579570). Restored by ONE code-neutral named local:
  `int (*hook)(int *); hook = ai_Tutorial->run->stepHook; if (hook) hook(p);`; `int (*hook)(int *args)` did NOT
  (the prototype-scope name counts). After adding / removing declarations or labels in sv_t above 0x578e40,
  re-grade Chapter2 / 3 and compensate with an ordinary local in the changed function, not probes.
- **A header change for one function needs the whole build's byte count**: sensitive functions can veto it.
  main_SettleRound (not kept): `ev[0] = m->flags1b4 | 1; ev[1] = ..` vs `test byte ptr [edi+0x1b4],1` + three
  fresh loads is a struct copy (`ws = m->work; ws.flags |= 1;`, AIWORKSET; a cast view gives the same code); the
  member in AICHAR (anonymous union, any order or tag) dropped ai_TutorialInitChapter2 / 3.
- **Stack-slot tie-breaks move with names and headers**: renaming four functions moved 0x483a60 38 -> 33 B,
  `<windows.h>` 5 B. If bisecting by deleting functions is non-monotonic, it is noise (ai).

### Callees defined above the caller (same-TU register knowledge)
- **A value or global kept in a register across a call where the original reloads (or the reverse)**: VC6 knows
  what a function defined earlier in the file leaves alone. Define callees above callers as in the original; a
  regression on a join, or a match only with the callee elsewhere, is TU-boundary evidence (d2_Unk475bf0;
  d2_Unk477a70 / 477ed0 cache d2_Unk6ed9dc across it when it is above; d2_Unk478320 needs d2_Unk476c90 above;
  d3anim_SetFrame needs d3_Unk4324e0 above; rd_object swapped two operands with d3_SceneVersion above; 0x4f5d50 /
  0x4f5f00 keep `m[0]` across 0x4f2870 with `push ebp`, matched only with the handlers in another file).
- **The callee is not byte yet**: its register set moves the callers. `dcp grade` every same-unit callee above a
  deep item first. gm_PlayerRecord to byte (slot chosen in an if / else, `slot = -1` on the other path, tested at
  the join: `test eax,eax` after `and eax,0xf`) took gm_TakeOverCharacter 45 -> 12 ins, then byte. A far-off draft
  callee above matched callers regresses them: keep it at the end of the file (am.c, 0x495a50).
- **edx live across dd_BppToDDBD**: load `hw->dcmColorModel` into a local before the call.
- **Judge near-misses only in the joined, complete TU**: d2_FindPic above d2_Unk46fba0 cut 772 -> 700 asm lines;
  d3_CloneObject flipped with d3_RadixSort added, the d3_left and d3_engine2 merges. No lever: 1-7 unused
  `__inline` functions above it.
- **A `short` loop counter**: the original's separate `dec; jne` countdown where an int compares the derived
  offset (gm_TakeOverCharacter).

### A missing call: side-effect-free body above the caller
- **Unused-result call missing (diff starts with a missing push or register noise)**: VC6 drops it when the
  callee's body (`{}` or `return 0;`) is visible above. Move the stub below its caller (out of address order,
  with a note); a prototype does not help. unk_5d33b0.c: listeners 0x5d9a80 / 0x5d9b20 / 0x5d9b90 moved above
  the stubs 0x5d85e0 .. 0x5d9890 matched; a scratch build confirms it.

### Shared struct member lists (d3.h, d3_engine.h, ai.h)
- **Only identifier renames (same type) are byte-neutral; any other member-list change can flip a byte function
  in another unit** (usually swapped `mov esi,[ebp+8]` / `mov edi,[ebp-4]`, 8 B, asmdiff class `order`): an
  anonymous union / alias, a typed field or split replacing a pad, a bitfield view. Not monotone: each change
  re-rolls a tie-break. Put new fields in a cast view / union type in the unit's own header and run the full
  `dcp check` (clean build/obj, whole exe), not the unit diff; revert when a byte match falls.
- **Victims**: d3_Unk4550e0 (0x4550e0, d3_mir.c, 8 B at +0x1be) from D3PVERT 0x30 `D3VECTOR light`, D3TEXTURE
  0x68 `union { unsigned char flags; D3TEXBITS flagBits; }` + 0x6c `D3TEXFLAGS loadFlags`, D3TEXTURE +0x54
  `unsigned long stamp` (view D3PARSTAMP), D3SURFTEX.flags `union { int all; unsigned char b[12]; }`, D3MATERIAL
  0x104 union `ops` / `name` (the ch_TakeObject model name; raw D3MATERIAL_NAME104 stays, chc_TakeObjectScript
  escalated), D3MESH / D3MATERIAL 0x954 (view d3s.h D3SMESH / D3S_MESH(obj->material), d3_light.h's D3LOBJ
  pattern), D3OBJECT `union { D3VECTOR unk084; D3VECTOR rot; }` or `struct D3PATH *path` at 0x1d0 with D3_OBJPATH
  (also d3_anim.c), D3OBJINFO `union { D3OBJECT *unk10; void *mesh; }` (redone as rename `D3OBJECT *mesh`),
  D3SCENE `pad084[0x28]` split (view D3SCENEPAR in d3_par.h), D3PARTSYS `unk24` -> `time` / `maxSpawn`.
  d3_CloneObject (0x43d180) from D3PTRI 0x26 `union { unsigned char drawFlags; D3TRIBITS drawBits; }`, the
  D3OBJECT / D3OBJX 0x90 union of three D3VECTOR views (stays plain, read via `D3_L()`), D3MESH.unk008/unk048
  `char[0x40]` -> `D3MATRIX`, typedef + macro `D3OBJPOSE`/`D3_POSE` in d3.h (moved to d3_tools.c).
  d3t_UpdateSunRays (`fisub [d3t_SunFadeStart]` -> `fild [d3_Time]; fsubp st(1),st`) from D3OBJECT `pad068[0x1c]`
  -> `pad068[0x10]` + `D3VECTOR posOffset`.
- **Observed neutral**: renames (D3MESH.unk954 -> groundOffsetRaw, AIENTRY.unk44 / AISLOT.unk18 -> sellAmount,
  AIOBJ.unk12 -> fill, char arrays), AIMAPENT restructured to G15PLANT, D3CLIP as a union, an anonymous union in
  D3PVERT, D3SURF.unk18 int -> `D3OBJECT *owner`, D3MESH.unk0b0 -> `frameStamp`, D3MESHSUB three ints ->
  `D3TEXHANDLE *tex` / `D3OBJECT *light` / `D3VECTOR dir`, D3SURFTEX as plain int + byte fields, a comment line in d3.h. Not
  isolated: D3PARTICLE.unk50 -> frame.
- **A rename that changes the type is not neutral**: `float fadeStart/fadeEnd` for D3ANIMSLOT 0x34/0x38 (was `int
  unk34/unk38`): d3_Unk432750 4480 -> 4528 bytes, frame 0x70 -> 0xbc; seen only by `uv run unitdiff <file>
  --against` (WORSE), not the ledger count (2495) or `dcp check --against` (draft function). Bisect with `cl
  /nologo /c /O2 /Gy /Iinclude /Fo<dir>\ src\game\<unit>.c` and the COMDAT section size in the COFF symbol table.
- **Bit views for a target function**: unit-header cast views (D3BITS, D3TEXVIEW) and a unit-header union
  (D3TEXPACK) kept d3_SetVertexColors (0x452fa0) byte. `D3TEXFLAGS f = {0};` is NOT `*(int *)&f = 0;` (647 of 784
  bytes differ); `((D3TEXPACK *)&f)->i = 0` is.
- **ai.h: [esp+0x10] / [esp+0x18] slot swap in ai_StartBuildingWork (ai_Unk483a60, 0x483a60)**: AIGRID `char
  pad05[0x13]` -> `char pad05[3]; int average; float unk0c, unk10, unk14;` (same offsets, ai_Grid users not
  reading them), any AIMSG / AITIME change (`struct { unsigned char count; char pad59; unsigned short item; }
  order;` in AIMSG.arg, an `entryIndex` member). Grid fields live in gv.h's ORDCELL view (ord.c cannot include
  ai.h: both declare ai_Time / ai_Chars / ai_BTypes); ai_SendJobOrders keeps raw word / byte stores.
- **VC6 C accepts anonymous structs / unions silently, no code change**: one field can carry two names (ai) -
  subject to the member-list rule above.

### Declarations and includes in shared headers
- **An unrelated declaration or include flips a byte function**: an extern in gv.h (ai_TimeHour) took
  d2_DrawObjects 93 -> 97 asm; `#include "err.h"` in d3_int_io.c made d3_LoadScene 3 bytes off at +0x27f (two
  stack arguments swapped, a `lea` with swapped registers); `void d3fl_FreeMap(D3FLMAP *);` in include/d3_fl.h
  flipped d3t_UpdateSunRays (0x4172e0) by 125 bytes; the D3OBJCOLOR typedef in d3_engine.h flipped d3_Unk4550e0.
  Declare the name in the user's .c (err_Message in d3_int_io.c, d3fl_FreeMap in d3_interface.c / d3_engine.c,
  D3OBJCOLOR in d3_engine.c).
- **Removing declarations flips non-users**: dropping `void d3_Unk441db0(void *)` / `void d3_Unk441ea0(void *)`
  from d3_engine.h flipped d3_Unk4550e0 (restored as d3fl_FreeTiles / d3fl_Inflate); deleting the d3_Unk4550e0 /
  d3_Unk4554b0 prototypes flipped it and d3_LoadScene44be60 (+0x27f). Keep the lines; route a new view with
  `#define name name_mirview` around the include (d3_mir.c). Safe only if the declaration list stays literally
  the same; judge with the full ledger check, not unitdiff.
- **d3.h's content is load-bearing for d3_mir.c**: removing `d3_RelightObject` or appending `#include
  "d3_light.h"` flips d3_MirDrawObject (esi/edi order); only re-matching it unblocks the canonical-header rule.
  The include in d3_anim.c adds 2 instructions to d3anim_CreatePath and d3anim_CreateMorphAnim.

### Prototype order and lint errors
- **C2040 ('differs in levels of indirection from', two signatures that print the same)**: a call before the
  file-scope prototype assumed `int f()`. Put the prototype block above the first use (ecmd.c).
- **clang E error, K&R declaration after a definition with another return type** (`int f();` vs `char
  *f(void)`): drop the declaration.
- **clang "conflicting types", `void f(struct TAG *);` before the tag vs `void f(TAG *);`**: add `struct TAG;` in
  the header (d3_interface.h).
- **One function under two spellings dodging a wrong header prototype** (0x447190: K&R `ai_Unk447190()` in
  cm.h/gv.h, `void d3_Unk447190` in d3.h, a char definition, a #define in d3_interface.c): merging needs the header
  fixed, the K&R declarations dropped and int call sites cast, not a rename.

## It was a meaning bug: reading rules
Differences that looked like allocation and were a wrong field, index, constant, argument or condition; how to read the original and Ghidra's output.

### A slot or offset hunk that is a wrong field, index or variable

- **A slots-only / "allocation only" diff**: map every store's and load's frame offset (fstp / fld to argument
  slots when locals live in dead argument slots) before trying spellings. Found: util_Random(8) stored into x's
  slot (the &x passed to ord_GetGridCell), rec[3] (AIDEC.guilt) added where ours added the random
  (ord_AddGridAmount, ord_SubGridAmount, draft -> byte); the item bonus added to the likings not the skills, the
  second abs over other operands (ai_TrustedOfTwo, 142 vs 142 ins, byte).
- **A one-instruction `slots` hunk inside a record fill** (`[esp+0xd4]` vs `[esp+0xd0]`): a wrong field index until
  proven otherwise (msg.arg[2] for msg.arg[1]). (ai_CurseWindow)
- **A +4 operand accepted as a `slots` hunk**: can be a wrong field scoring `behaviour` (1000 at AIMSG+0x9c vs
  unka0 at +0xa0); read the offset, not the class. (0x4affc0, 0x4b0050: byte with a local view)
- **`[ecx+base+0x9]` vs `[ecx+base+0]` in a compare**: a meaning error; `((char *)&ai_Goods[id])[9] != 9` ->
  `ai_Goods[id].type` (it also dragged a callee-saved register colour). (unk_4b6480.c 0x4bacc0, byte)
- **A displacement off by one byte from the source's field**: suspect the header's field order, not the
  expression. `lea edi,[edi+eax*4+0x13e]` used as a cell mask (`if (mh > 1) { m = ~(mh - 1); ... }`) is `.lod`
  (d3s.h D3STBLOCK: lo 0, hi 1, lod 2), not `.hi`; d3_sm.h D3SMBLOCK got `unsigned char lod` after `hi`, no other
  byte changed. (d3s_Project 0x45be10, d3s_Update 0x45c620)
- **Pointer-indexed loads in a strength-reduced loop**: read against the IV base; `[eax-0x50]` with eax =
  &row[i + 1].pos is row[i].pos, not row[i + nx + 1]. (d3s_MeshFromMap)
- **`[esi]` vs `[esi+4]` in a big "allocation only" draft**: a wrong index (key / next for nkeys / key).
  (d3anim_StepObject)
- **A record read at the wrong index**: gv_GetTableRecord copies 9 ints into an 8-int buffer; `buf[0]` where the
  original reads `buf[6]`: `int buf[9]`. (0x4ea990, byte)
- **A prologue `xor ebp,ebp` unaccounted, a scan's `dec` misordered**: two counters merged; split into `left` (set
  to 1 right before the scan) and the vote index `j` (zeroed in the prologue, never reset). (0x4e8c20, byte)
- **A narrow field store (`mov WORD [esi+0x8],cx; movsx ecx,[esp+0x60]`) where the draft stores a pointer**: the
  field takes the width argument (`win->w = win;`, `desc[1] = win;` were wrong); grep for `->field = <a pointer>`
  and compare store widths. (0x472390, 740 -> 638 bytes)
- **`mov ecx,[edx]; mov [esp+4],ecx; jmp`**: the tail call passes `rec->id`; the record pointer compiles to `mov
  [esp+4],edx`.
- **A pointer argument differing only in `[esp+0x54]` vs `[esp+0x5c]`**: `&e` where `e.unk08` belongs (2 ins, a
  meaning bug); reconstruct the local's base (`lea edi,[esp+N]` of its `rep movs`) before comparing pointer
  arguments. (0x4b11e0's stock loops into 0x4c56d0)

### Locals, arrays and frame size

- **Compiled size well below the original's**: a block local array declared twice under one name, one block
  reading its never-written copy (VC6 drops the dead stores); grep for it before variant work: one function-scope
  array. (ai_OpenItemTooltip: three `int list[32]`, the original ONE at [esp+0x24], 104 bytes short -> 1600 vs
  1596; ai_GypsyHelperWindow's btns[3])
- **A frame 8 bytes short with out-of-frame stores**: an out-parameter too small; gm_ClockToDate (0x4fe2c0) writes
  10 bytes: `unsigned char d[10]` (f3gm.c's convention). (unk_4b6480.c 0x4bb250, byte)
- **A 0x100-byte `sub esp` difference**: a wrong sprintf destination (the int * parameter `out` instead of a dead
  `char text[0x100]`); read the `lea edx,[esp+X]` into the destination push. (0x58fc40, 73 of 103 hunks)
- **4-byte holes after every 3-float group in the frame**: D3VECTOR4 locals, declared in their blocks.
  (d3anim_StepObject: 0x68 -> 0x9c of 0xbc)
- **A failure path reloading ONE slot for two variables (`mov ecx,[slot]; mov edx,[slot]`)**: not `w = h = ex;`
  but two locals unset on that path, merged into the third local's slot; deleting the assignment gave the spill
  of ex and `call ebp` twice. Try deleting a "default" assignment first. (main_OpenScreen, byte)
- **A scratch register rotation (eax / ecx / edx) through a long store block and everything behind it**: ONE
  statement misplaced (a global byte read-modify-write amid the config stores); run a move-one-statement round
  first (n^2 variants): 66 -> 6 ins, then plain store order. (main_OpenScreen)
- **A typed view of another unit's struct reads another record**: pad the view to the real sizeof; `{ char
  pad[0x78]; int input; }` makes `d2_Objs[id]` scale by 0x7c, not 0x2ec, silently. Document it as type debt.
  (D2OBJ; d2.h's vfs_Root / vfs_Tfopen clash with vfs.h)

### Conditions and jump targets

- **One `branch` hunk plus `order` / `regs` around a macro `if (call == 0) return 0;` (LS_R) as a then-arm**: the
  else binds to the macro's if; brace the arm, grep for macro + `else`. (ls_ReadSaveHeader)
- **A test that jumps PAST stores ours always does** (`je` at +0xc1): the stores (`stepControl = -1; stepKind =
  4;`) belong in the close block; dcp grade's `branch targets differ` names it. (ai_TutorialStepForm, 66 -> byte)
- **`cmp ax,[minx]; jb reset`**: jumps TO the reset when maxx < minx, i.e. `(unsigned short)minx > (unsigned
  short)maxx || ...`; check the target before "fixing" the condition. (0x47a460, 0x47a7d0, 0x47ab30)
- **An `||` of `(a && b)` terms, one arm `jne out; jmp body` where ours has `je body; jmp out`**: a flag set by
  separate ifs (`bad = 0; if (res == 0 && !has800) bad = 1; if (..) bad = 1; .. if (bad) {..}`). (main_OpenScreen)
- **`inc ecx` + `shr eax,cl`**: `(*run >> 1 + acc) % n` and `acc++; acc = (*run >> acc) % n` compile alike; the
  precedence trap is harmless. (0x47b350, 0x47b640)

### Types, constants, strings and arguments

- **`imul` vs `fild`+`fimul`**: a source type; `fimul` / `fidiv` = int operand in memory, `fmul` = float.
  `(float)cnt * p * 0.01f` gave `fild cnt; fimul p; fmul 0.01f`; the imul temp had shifted every slot: 149 -> 6.
  (0x4b0d40)
- **`fld DWORD [tab+eax*4]` / `fstp [global]`**: float arrays; check before trusting an `int[]` extern.
- **Only the `ds:` source of an initializer copy differs** (asmdiff masks numbers >= 0x400000): `dcp dump` the
  .rdata addresses the prologue loads, match them to the elements, check what each index does (`{ "KATHOLISCH",
  "EVANGELISCH" }`, ds:0x601c20). (0x4b0d40)
- **A strcpy with only `mov ecx,<n>; rep movs dword` size differences**: a wrong literal; read it from .rdata at
  the `mov esi,<addr>` (`dcp link` does not see it). (f3snd.c 0x4c28a0, three bank names, byte)
- **`add esp,0x10` after a call drafted with 2 arguments**: `gm_AddTime(c, hours, mins, secs)`. (0x4f9220)
- **A handle push the draft lacks** (`push edi` at 0x520a9e before 0x430aa0): a missing argument; fix it even when
  it reads worse because the original hoisted the push above the `jne` (0x520b80 475/114 -> 469/120, a match-deep
  shape).
- **The original stores a call result to a slot and pushes it after the next call**: both calls into named locals
  in call order (`ex = GetWindowLongA(..); style = GetWindowLongA(..); if (AdjustWindowRectEx(&rc, style, 0,
  ex))`), not inline in the argument list. (main_OpenScreen)

### Reading Ghidra's output and the original

- **`&DAT_00fa6030 + i*0x160`**: an array base (Ghidra's `&DAT_xxxx`); `DAT_xxxx` would be a pointer variable, so
  `*(int *)(DAT_00fa6030) + i*0x160` (a U_I(U_I(0xfa6030)+..) draft) is wrong unless the array starts with one.
- **`iVar = win * 0x3c4`**: a byte-offset int local (`[esi+const]`); `&d2_Wins[win]` gives `lea esi,[ecx*4+X]`;
  both occur. Ghidra's `(int)(float)` can be an `fstp` into a float union member.
- **One pointer apparently loaded twice, or a pointer held in edi**: recompute parameter offsets after every push /
  pop (`[esp+0x2c]` before and after a `pop esi` are two parameters; the "unused" third argument was read; a
  missed third parameter). (d3t_InTriBox)
- **sbb / inc / xchg hunks after the last `ret`**: data (a 20-case switch table after `ret 4`); check the real end
  with dcp disasm. (err_ExceptionFilter 0x4215c0, ends 0x421c34)
- **"Matched" names on swapped struct fields** (D3DCARDINFO, DIMOUSEST): the link gate finds them.

## Method and tools
How to work: variant rounds, the c2 tools, sweeps, what the tools' output means, when a result says nothing.

### dcp commands: what they do and do not do

- **`dcp check` does not rebuild**: run `dcp build` first.
- **Register allocation mixed with the rest**: `dcp asmdiff --regs a=b,...` renames our registers first
  (d3t_DropObject). A bogus
  -/+ pair for an instruction with two relocations: trust `dcp asmdiff --bytes` / `check`.
- **`dcp try` empty replacement (`===` directly followed by `>>>`)**: swallows the next block; put a comment in.
- **A `dcp try` variant that matches byte for byte**: may have two globals swapped; `dcp link` after `--apply`.
  Near-misses can flip with unrelated code above them: judge them once the file is complete.
- **Whole-function `dcp try` variants**: keep the `/* 0xADDR */` comment verbatim (cut at `*/`); without it the
  build loses the grade (`no graded function`) and `--apply` drops it from the ledger (`dcp fn: no function`):
  restore the comment from git.
- **Backticks in a `dcp note` text in bash**: the shell runs them; single-quote the reason or pass a file.
- **A tail `code` hunk, nops on our side, instructions on the original's**: sections pad to 16 and `dcp build`
  compares `len(mine)` bytes, so 225 .. 239 bytes compare against the next function; make it one instruction
  shorter. (ahm_ResearchProgress 0x4eafd0: 49 -> 9 bytes / `order` at 224)
- **Dozens of body-unrelated hunks after a rename**: a caller spells the old name, C89's implicit `int f()`
  compiles and the wrong symbol is compared; rename all call sites in one edit, `dcp refs OLD` before and after.
- **A one-off FP regression after a full rebuild** (fild/fdivp vs fidiv, back next build): rebuild before
  believing it. (ai_CondCharShare)
- **Renaming a shared header field**: rename, build, every use fails with C2039 (not same-named fields like
  D3TSNOW.unk10); `#line` files report the original numbering (d3_tools.c's 3016 is source line 2030).
- **cl.exe spinning on a rough draft of a huge function** (0x5aab80, 5328 B, >10 CPU minutes): build a small stub,
  then grow the body in pieces, each built.

### Big diffs and function boundaries

- **A big function's diff**: mask esp offsets first; the rest is the real work (z3's 4.7 KB: 6 differences).
- **Ghidra misses starts after short functions and pointer-only callbacks** (action callbacks, timeSetEvent
  procs): check nop padding in `dcp disasm` and heavy-queue entries before sizing them (8 KB = four, par; hv3's
  9152 / 5552 B = 30 and 20; g1 6, g2 5, g3 1, hv4's heavy ranges 19). The AIACT registration code lists them
  (eval / decide / exec at -0x78 / -0x74 / -0x70, e.g. 0x4a1cd6..0x4a2576). A "thunk" can be a real tail call.

### The c2dag tools

- **`python tools/c2dag/c2dag.py src/UNIT.c FUNC`**: VC6's scheduling graph per basic block, instructions before
  scheduling with priority, height, flags, dependency edges (`--blocks`: VC6's listing, hidden switch `/d2FAF`:
  `; Block(n) In(..) Out(..)` in the /FA file). A proxy C2.DLL (tools/c2dag/c2proxy.c) hooks the priority pass
  (dag.c, RVA 0x14f6a of C2 13.00.9044); a dump fault prints `(dump fault: block skipped)`.
- **`c2dag.py SRC FUNC --alloc`**: each candidate's register, last column (`-`: none), first priority pass only;
  read it before a register round: which candidate must rank above which. Priority alone does not predict it (grp:
  esi while above ret, none once ret ranks above and holds ebx); rule not read from c2.dll. (main_ChooseHistory,
  open)
- **`python tools/c2dag/c2trace.py SRC FUNC [--variants FILE --label L]`**: the allocator's real sequence (each
  candidate queued with priority, savings `sav`, flags, registers available, preferences; assigned; splits); "a
  free register is refused" is explained there. `--prio REGEX`: each block's priority term (refs * P * w, -P * w)
  with its source line; `--split REGEX`: the cuts, the AF7 restrictions (RESTRICT, with the reason) and the bulk
  split's region per block with its predecessors in list order ("Live range splitting, regions and the priority
  terms").
- **`python tools/c2dag/c2frame.py SRC FUNC [--refs] [--variants FILE --label L]`**: the packer: each local's size
  and refs at layout (L rows), slot table before / after the sort (B / A; A index 0 nearest esp); `--refs` each
  reference in walk order.
- **`python tools/c2dag/c2tail.py SRC FUNC [--tries] [--il pre|merge|post|all] [--asm] [--variants FILE --label
  L]`**: the tails VC6 merged (by xjump / pair / move, deleted and kept copy with source lines), what it
  compared, and the IL around the merging pass (which block falls into the exit label). Rules: "How VC6 orders
  blocks and merges tails". `c2pass.py --list` names the call sites that switch block order and each merger off.
- **`uv run --with capstone python tools/c2dag/vdiff.py SRC FUNC [VARIANTS LABEL]`**: one dcp try variant's
  instruction diff against the original, tree untouched.
- **flip.py** names each hit by FILE:LINE and the changed source line.
- **flip.py reading rule**: a hit on a DUPLICATED statement = the value has two uses in the original (a loop, the
  expression again, a macro's second expansion); a changed constant at one site = this block is not shared with
  its twins.
- **flip.py on a slot question**: needs a copy keeping the `_x$ = -N` lines above PROC (`_x$[esp+N]` does not move);
  equal-size arrays are ordered by a C2 tie rule (main_ChooseCity: 0 of 111 mutants moved the 0x100 arrays,
  objName[0xff] did).
- **c2fuzz.py / c2pass.py**: c2fuzz skips one back-end call at a time (15353 sites, a probe compiled per copy);
  c2pass compiles a unit with chosen calls skipped and diffs one function: `uv run --with pefile python
  tools/c2dag/c2pass.py ..`. No call switches the scheduler off cleanly; d3sndw_Update's reorder-only sites:
  0x1070aead, 0x1070f5b1, 0x1070f87b, 0x1070fa29, 0x10714f85, 0x1073a0d6.
- **Definitions the tools cannot find**: a wrapped parameter list works now (function_span); a K&R definition
  (`void f(o) T *o; {`) does not: write it ANSI (alike for pointer parameters). `WinMain` (`_WinMain@16`) by its
  plain name.
- **A `#line N "D:\..."` / D3DCHECK unit whose /FAs listing cannot open the file** (c2prio, a script that no
  longer exists): copy to src/game/zz_tmp.c without the #line lines and the D3DCHECK( macro, run, delete.

### Sweeps and twins

- **Before the first variant of every deep item**: `python tools/sweep/twin.py NAME` (byte functions with nearly
  the same statements, the differing lines; `--stuck`: tagged functions with a byte twin); after a match, `--all`
  on it finds copies still below byte.
- **`python tools/sweep/blockscope.py`** (a whole-function variant per inner block): byte ai_MsgWorkerStop,
  ai_StartBuildingWork, ai_UnloadCarts, he_PostWorkshopEvents98, ai_MsgFreeBuildingObjectType2e,
  ai_CabinetSpyOnCharacter, ai_CabinetSpyByMoney, ai_GraveAgeContact; one lever more: he_RunFire,
  ai_BlackmailBuildingListener, he_InnStaffLeave (`arg[]` stores [0], [2], [1]), ai_UseGoodInfectOwner (`msg.time
  = ai_Time` after `msg.fromId`), ai_LoadTownMap (`scale = 7.5f; if (size < 0x780) scale = 5.33f;` moves `pop
  edi`), ai_CurseWindow; closer: ai_SendJobOrders 130 -> 87, ai_UseGoodSendHome, d3sm_Build, ai_CannonExplosion,
  gv_GraveyardTurn, he_RunHarvest, he_RunGraveyardWorkers, ai_StorageWindow, ai_CourtshipWindow,
  ai_CabinetIntrigueWindow; no gain on lc.c's big fills. Rerun it and `fillloop.py` after base work (minutes).

### Functions given up on that were not allocation problems

- **"Allocation only, no source lever" after a long lever list**: none of the five reviewed (main_ChoosePlayer,
  snd_FindVoiceSample, snd_UpdateRain, vfs_MemOpen, vfs_Tfopen) was allocation; each lacked one check.
- **main_ChoosePlayer (36 slot ins -> byte)**: byte twin lobby_ChoosePlayer unread; it uses ONE `int row[4]` for
  both button rows. Two arrays "shared in the original" plus a hole of their size in `dcp frame` = one array of
  twice the size. Check: twin.py.

### A byte sibling, twin or published source is the source

- **Copy the byte sibling FIRST**: a hand-derived data flow (hoisted deltas, accumulators, hand-guarded do-whiles,
  `next` pointers) is the wrong form's optimum. surf_Down32 = surf_Shrink16 with DWORD pixels (plain `for (j ..)
  for (i ..)`, `n++` per pixel, `for (rb = 0; m; m >>= 1) rb++;`); surf_Up16 = surf_Up32 with WORD pixels;
  d2_ShpConvert8To16 = d2_ShpConvert24To16 plus the palette lookup (`if (0 < *srccnt) do .. while (m <
  *srccnt)`, `src[1]` at each use); d2_ShpBlit16ScaledLit = d2_ShpBlit16Scaled plus the light table, top / bottom
  clips only (`unsigned char n`, `int y`: `and edi,0xff` / `idiv` / `jl`). All byte in one or two rounds.
- **A twin in another unit**: sv_MessageBox (0x521150) = ai_Unk4e5470 in cm.c (`timed = flags & 0x80`, `if (timed)
  bar = call; else bar = -1;`, `if (sv_RunFrame(..)) { quick = flags & 0x40; n = flags & 4; do { .. } while
  (sv_RunFrame(..)); }`): 212 -> 133 hunks.
- **Public code: get the published spelling first** (signs: self-contained algorithm, magic numbers 200000,
  DITHER_MAX 20, 32768, tables biased by 256). qnt.c = Dennis Lee's DL1 quantizer (dl1quant: build_table /
  reduce_table / set_palette / quantize_image / bestcolor); qnt_Map byte in four rounds from 87 ins.
- **Row pointers stepped mid-loop**: step at the END (`nextrow[0-3] = e; .. nextrow[0+3] += ..; thisrow[0+3] +=
  ..; thisrow += 3; nextrow -= 3;`), clamp table in index order (0, k, 255), thisrow before nextrow: 87 -> 72.
  (qnt_Map)
- **`[edi+eax+0x100]` and a loop `lea edx,[eax+0x100]`**: a pointer local (`dm = lim + 256;`, `dm[k] = k`, `dm[r -
  pal]`); `lim[x + 256]` never: 72 -> 2. (qnt_Map)
- **Swapped address operands in a table loop**: `tbl[5][i].count` with the plain index, not `off += sizeof`: 2 ->
  0. (qnt_Map)
- **Bilinear growing**: plain `lr = r00 + (r10 - r00) * j / dy;` per row, `(lr + (hr - lr) * i / dx)` per pixel
  (VC6 strength-reduces and hoists); frame 0x18 short: corners as `DWORD r[4], g[4], b[4]` (0 here, 1 right, 2
  down, 3 down right); Up32 reads `s[IDX]` per channel (not `p = s[IDX];`). (surf_Up24, surf_Up32, byte)
- **Consecutive slots in index order plus an untouched hole**: an array (g00 .. r11 in eight slots, an 8-byte
  hole for b[2] / b[3] forwarded to registers); scalars pack by lifetime instead. (surf_Up24, surf_Up32)
- **One sum inline in a call's arguments rotating every scratch register after it**: `n = s->npix + s->size; r =
  mem_Alloc(n, ..)`: 30 -> 0. (d2_ShpConvert8To16)
- **A draft reusing minx / miny / maxx as run counters and a `seg` pointer**: d2_GrabShape8Runs' run loop
  (lines[] / line / run / col / t) with d2_GrabShape16NoRuns' scan: 334 -> 74. (d2_GrabShape16Runs, stuck on the
  D2KEY word only; byte with a probe symbol for gb)
- **The skip loop's address hoisted above the test in ours, set up after the `jne` in the original**: index form
  `while (pix[t + col + x] == key)`, no `p = pix + t + col + x; do {..; p++;} while (*p == key);`.
  (d2_GrabShape16Runs)
- **Unsigned entry test (`cmp; jae`) and latch test**: `while (run[1] + col < s->w && pix[..] != key) run[1]++;`,
  run[1] unsigned. (d2_GrabShape16Runs)
- **maxx / maxy in swapped equal-density slots**: init and reset store orders (312 tried, one byte): `maxx = 0;
  maxy = 0; cnt = 0; minx = w; miny = h;`, reset minx, miny, maxx, maxy. (d2_GrabShape16Runs)
- **The distance rises when the source gets right**: keep the visibly original forms and work the allocation from
  there; `strcpy` for seven hand-written `do { c = s[k]; d[k] = c; k++; } while (c)` copies (offset copy -0x80
  per host, `neg edx`) and `int btn[4]` for the frame: 281 -> 348 ins, every candidate split.
  (main_SearchNetworkGame)

### Variant rounds

- **Statement orders, parenthesizations**: a generator script for `dcp try`, not by hand; `dcp try` with
  hand-written families rather than several multiplying insert / alt ops (3^n; vargen, a script that no longer
  exists).
- **Whole-function orders without result**: permute small blocks alone (the 4-24 stores of one child box), the
  rest fixed. (d3q_Build child 2)
- **A statement-move beam search / hill climb** (d3if's helpers, build/helpers/, uncommitted): d3if_SelectScene
  101 -> 50 bytes.
- **The same block compiles differently in two functions**: the cause is outside it (weights, a sibling's extra
  statement); diff the sources.
- **Signals**: identical bytes for a whole family = the mechanism does not reach it; a moved difference at a worse
  count = right mechanism, refine (subsets, positions).

### Base pass

- **Escalation runs the pass first**: spell (a script that no longer exists) hinted spellings per pattern (sib,
  add-lea, width, rep-stos, dup-tail, push-pop, branch, cmp-test, reload, iv-shift, moved); try forms until ideas
  run out, never a count; far functions start from meaning leads (type / signedness / offset / width). Escalate
  after leads tried AND readability advanced (names, Q6, typed views, Q3/Q4/Q5); apply a match to siblings; the
  reason lists leads, patterns and each Q failure's gap.
- **The last few bytes**: a scheduling or slot choice; spellings, parentheses, casts, renames, permutations one
  generated round each, then mechanisms over all positions / subsets.

## This code base
Conventions and macros of the game and its engine that explain a code shape.

- **`D3_ZERO` (include/d3.h)**: inline-asm clear, 284 copies: `lea ecx,[edi+edi*2]; and ecx,3`, then `rep
  stosb/stosd/stosb`; forces an ebp frame. Size before pointer: `int _zn = (size); void *_zp = (dst);`
  (d3t_MoveObjectPath; CreateSnowMipmaps). Twice in one function: per-line labels via __LINE__ (C2412)
  (d3t_MoveCameraPath). Global destination: `mov edi, ds:global`, no temp: `D3_ZERO_AT(var, size)`.
- **Inline-asm macro temps (D3_ZERO, copy macros)**: declaration order decides stores and slots; D3_COPY /
  D3_COPY_TO / D3_COPY3 differ by their stack temp pattern.
- **Fixed-point helpers**: `__inline` with `__asm` bodies (`imul edx; shrd eax,edx,N`; `mov edx,eax; sar edx,16;
  shl eax,16; idiv ebx`); `__int64` calls `__allmul` / `__alldiv`. Inlined, an expression argument gets a stack
  temp, a global is addressed directly. Span fills: `rep stosb` / `rep stosw` in `__asm` from a `char *` local.
- **Drawers pushing `edi, esi, eax, ebx, edx, ecx, ebp` after the prologue**: `__asm` bodies (the compiler adds
  push / pop of ebx/esi/edi the asm names). A `jmp` with displacement 0: `_emit`, one per line. No local named
  `ne` in `__asm`. 0xcc padding vs our nops: the TU ends there (0x47d210 / 0x47d710).
- **A volatile read three times**: `((d3_Time << 3) + (d3_Time << 2) + d3_Time)` (msx.c / sblib.c; f3snd.c's
  F3SND_TIME); `d3_Time * 0xd` reads once.
- **A 4-byte gap after a vector**: the `w` of a 16-byte `D3VECTOR4`.
- **`or dl,2` on byte +2 of a 4-byte flags struct**: `flags.b[2] |= 2` (D3TEXFLAGS). D3ANIMFLAGS.b[1] = the bits of
  D3ANIM.flags2d (0x10 loop, 0x08, 0x04, 0x02).

## Open cases and rounds without result
Unsolved hunks and what was tried on them. Read before repeating a round on the same function; none of these states a rule.

### A constant or flag in the wrong register

- **Constant 0 and the return variable in one register** (mov_Open `id = 0` / `mov eax, esi`; mov_Find `mov
  eax,ecx`): declaration order, init placement, early-return form, id spelling: no lever.
- **d3sndw_Update**: original 0 in ebx up to the main loop, ground edi, nfound ebp; ours 0 in ebp throughout.
  Zeroing spellings, init orders, scene-check structure, operand orders: no lever. `add esp,16` / `mov
  [Snd[0].snd],ebx` tie (h=2), the original's store had a successor.
- **A loop-only flag (`any` / `m`) losing its register to a temp or the objs[i] IV** (c2trace prio 6 / 12 below):
  the init next to the loop wins it but drops the top store.
- **gm_GetCreditOffers (7 ins)**: `all = 0;` at the top swaps cnt / all in all 12 guard orders, after the guard
  misses `xor esi,esi`; `neg; fild; fmul 0.01; fadd 1.0` folds to `fsubr` always (also cm_DoubleOne).
- **d2_RaiseWindow (78 ins)**: `0x2ff` shared by two search loops is a candidate the original lacked (flip.py: 6
  ins); 0x2ffu, casts, 767, sizeof, loop forms, other variables: no lever.
- **Constants in registers before pushes** (`xor ecx,ecx; mov eax,0x7b4c4a0`, `mov ebx,0xe07c43b4; mov ebp,1`) in
  ai_Unk4a63f0's callers 0x48eb50 / 0x48ec90 / 0x48edc0 / 0x48ef10 / 0x48f010 / 0x48ff50: locals, static wrapper,
  casts, varargs, function pointer, ~180 orders: no lever; maybe a LARGE_INTEGER-style argument.

### Register races between named values

- **Races named in @deep notes**: gm_BuildingBalance (cost spilled to out's slot), gm_ListWorkers (k outranks p3),
  gm_InitBuildingTables (len takes ebx), gm_CharNameRecord (an address constant takes ebx; int k contradicts).
- **gm_AddTime (32 ins)**: 60 in esi for both idivs, edi for three RMW temps; no named sum local gets edi (38).
- **gm_KillSpieler (insensitive)**: ch->netId hoisted into ebx pushes peer->netId to ebp and c * 0x21c to a slot;
  a typed CRIME array probe = control.
- **ai_RentGraveDetail (70 -> 31)**: ai_GraveDetail forms plus `for (k = 0; k < 5; k++, y += 0xd)`; e / n swap,
  flip.py hits remove a const-0 or an e reference.
- **d2_DrawObjCaption (85 -> 62)**: one call after `if (o->style & 4) {..}` gives the head's allocation, two calls
  the call sites; no form has both (no duplication as in d2_PlaceWindow).
- **d2_DrawObjects**: a text arm's mode / x race; flip.py hits only in the window clip walk above it.
- **d2_GrabShape24Runs (488 -> 325)**: 16Runs forms (`d2_CompareColour(..)` in the loop conditions) give the shape,
  but run (prio 1086) pops before s (1081); run inside `w > 0` folds into `[eax+ebp]`. Open: which s reference
  (flip.py: duplicating `s->npix +=` flips it).
- **d3t_MoveObjectPath**: i in edx / k in ecx in the original, swapped in ours; declaration orders, 71 preamble
  orders, counter forms, a key pointer, reusing `time` as k (fixes init order only): no lever.
- **d2 swaps**: d2_Unk472fd0 i / j esi / ecx (120 orders); d2_Unk46d040 x / y; d2_CenterObj obj in ebx (116 ->
  111); d2_Unk472760 `add [mem],ecx` needs a pointer local that breaks the layout.

### Load and operand order

- **`w->x + w->w` / `o->x - p1->w + o->w` / `o->w - p0->w` load order** (d2_DrawInput, d2_DrawSlider,
  d2_Unk46aa00, d2_Unk46bf60): ~60 spellings, `headers sample`: no lever; flip.py hits elsewhere (C1 value numbers).
- **`mov ecx,[eax+0x1cc]; ..; mov ecx,8; mov eax,[eax]`** (sv_UpdateBand, sv_UpdateBandLed) and `mov ecx,eax; add
  ecx,ebp; mov cl,[ecx]` for `ai_GoodsT[id].type` (sv_UpdateGroundplan, sv_LoadGroundplan): spellings no lever;
  lead from sv_HandleObjects: the pointer assigned before an unrelated store (`cls = &ai_GoodsT[id];
  sv_SelectedModel = ..;`).
- **`mov eax,ds:addr` + `test ax` for a 16-bit table field**: ours `mov ax,ds:addr` in every shape. (veg_FindType
  0x4bc5a0)
- **D2KEY `r` + word `gb` (settled)**: original `mov ax, word [key+1]`; every spelling and a header union (which
  cost sv_t's ai_TutorialInitChapter3 + 1 more their byte) are one code. Settled by the gapdata pass: the key is a
  3-byte object (no pad) for the five shape grabbers (16NoRuns, 24NoRuns, 8Runs, 16Runs, 24Runs).
- **d3t_CreateSmoke**: `now` loaded before `rand() & 7` is used; term orders, casts, volatile, `t += delay`: no
  lever.
- **d3t_UpdateSnow**: viewport line moves, `/ 2`, casts, parentheses don't move the int loads.

### Hoisting, scheduling and strength reduction

- **gm_UpdatePrices (40 -> 37)**: with `g = GM_GOOD(item)` and cm_DoubleHalf, p*17 hoisted above the arms' test; 63
  arm spellings one code; flip.py: only breaking the CSE of A[k-1].year in the k>0 arm.
- **gm_CountCharacters (5 ins)**: `lea edx,[edx*4+X]` kept across a same-unit call; no spelling moves it.
- **Four absolute-global loads hoisted above the first store** (0xfb9060 .. 0xfb906c from 0x6248a4 .. 0x6248b0):
  every order keeps load-store-load-store. (0x5d7080)
- **Row pointer `p = pix + ((r + y) * pitch + x) * n` strength-reduced in ours, imul per row in the original**:
  removing locals, the for header, `(pitch & 0xffff)`: no byte. (0x47a460, 0x47a7d0, 0x47ab30, 0x47ad40)
- **`(c[9] != 0 ? 10 : 0) + 0x114 + level` as `lea reg,[a+b+const]`, original `and; add reg,0x114; add reg,job`**
  (~15 ord.c functions): operand orders, parentheses, temps, a `...` prototype: no lever.
- **d3t_PlaceObject**: flag-byte RMWs through cl (original al / dl); `dir.x/dir.y -= cam` forwarded by `fst` in
  the original. 144 init orders, 720 key copy orders, bitfields, macros, temps: no lever.
- **d3t_CreateSnowMipmaps**: 120 mip loop orders, memcpy, pitch/alloc spellings: no `[edi-0x40]` height store.

### Branch layout and shared epilogues

- **gm_CollectOffers (47)**: count-down and count-up walks one code, `b = ai_Blds + k` worse; open: where VC6
  rematerializes the zero register, deciding how the two supply tails are cross-jumped.
- **`if (n >= 42) return 0;`, `jl` to the copy in the original, `jge <out-of-line>` in ours**: polarity, else,
  break: no lever. (0x4e6ec0, 0x4e6f30)
- **ls.c failures ending inline `pop edi; pop esi; pop ebp; pop ebx; ret`, ours one shared block via `je`** (`==
  0` reads, not `<= 0`): open. (0x51f720, 0x520200)
- **`cmp eax,imm; jne next` with an empty fall-through**: empty if, self-assign, empty asm drop the compare.

### Frame slots and register homes

- **ls.c writers**: n in a slot (`inc DWORD PTR [esp+0x10]`), handle / record pointer swapped (0x519d20), base
  pointer eax not esi (0x51a1a0), a global reloaded after a store (0x519a00); rename / slot work.
- **d2_Unk471a90**: two stack slots swapped (120 orders).
- **d3t_EarthCB**: slot pairs alpha<->s, dead<->len; reusing `s` for alpha fixes dead's slot but loses the
  volatile shape; names, order, `dead` type, alpha spellings: no lever.
