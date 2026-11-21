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
- **Floating point** (82 rules): How c2.dll allocates the x87 stack; Operand order of commutative ops (ranks);
  Product and sum chains: association, factoring, folding; Values kept on the FPU vs stored and reloaded; Float
  copies: fld / fstp vs integer moves; Compare idioms and flag tests; min / max macros and clamps; Constants: float
  vs double, pooled literals, reciprocals; Int / float conversions, division, _ftol
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

- **`push ebx` after the guards in the original, ours at entry; a callback's 0 result jumping straight to the
  final `mov eax,[ret]` epilogue; a `ret = 1; jmp` arm out of line**: `if (cb) ret = cb(..); else ret = 1;
  if (ret != 0) { ..rest.. } return ret;` instead of `return 0` inside the callback arm; the frame 4 bytes too
  big at the same time = two address-taken ints (`one`, `index`) declared in their own blocks share one slot
  (ai_UseGood 78 ins -> byte).
- **Callee-saved pushes after the early guard, ours at entry, a `return pick;` inside a search loop**: the loop
  runs `while (i < 7 && pick == 0)` with one final return (ai_DrawSettingGood byte; the init order i, pick, sum
  decides ebx / ebp / edi).
- **An early `je` straight to the final `pop esi; ret`, ours its own epilogue copy**: one final return; the loop
  under `if (first != end)`, the in-loop `return 0` as `ret = 0; break;` (cm_SendGroupState).
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

- **`xor edi,edi` in the prologue and one failure returning `mov eax,edi` (its own epilogue copy), another failure
  `xor eax,eax`, the success `mov eax,1`**: `ret = 0;` at the top, each failure `goto done;`, `ret = 1;` after the
  work, `done: return ret;` (the lcamt callback style, ai_QuarrelCallback). Every goto-free spelling of ret
  (`return ret` in the arm, `ret = 1` per arm, a final `if (bld)`) folds to `xor eax,eax`: VC6 folds a value only
  one definition reaches; the label merges ret = 0 and ret = 1 (ai_UseGoodSendHome 90 -> 260 = 260 instructions).
  The rest of that function: each message a block local (one slot), and the whole tail after the paths (0x77
  message, field block, news) written in BOTH arms with the arms' fills in the same order: VC6 cross-jumps the
  first copy away, and its temporaries put the second arm one scratch register further (probe: one extra temp
  at the first arm's end gave the original's `mov eax,[use]`) -> byte.

- **`xor eax,eax` before the first `test esi,esi; je exit` (it serves both the next `mov ax,[esi]` and the
  return 0), ours `xor eax,eax` after the test and again in the exit**: the first test was a guard `if (o == 0)
  return 0;`, the rest nested as before (`if (g != 0) {..} return 0;`). Guarding the second test as well breaks
  the layout (ai_ExecuteUseMilitaryObject and nine ai_ExecuteUse* twins byte).
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

- **An allocation chain whose cleanup (free what was allocated, return 0) sits inline right after the last
  allocation test with the success init behind it (`jne init`), ours the cleanup at the end**: an ok flag,
  `ok = 0; a = alloc; if (a) { ..; b = alloc; if (b) ok = 1; } if (ok == 0) { cleanup; return 0; } init..`
  (VC6 decides the flag, the layout stays: gv_AllocGraveyard byte, s12).
- **One `or eax,-1` failure block after the success epilogue (ours an inline copy per failed lookup)**: both
  lookups in one condition, `if (a(..) && b(..)) { ..; return send(..); } return -1;` (cm_OrderToBestCell byte;
  the success path returns the last call's eax).
- **`mov eax,edi` (a zero kept in a callee-saved register) at the failure return, the failure arms' `call
  evt_Error` cross-jumped but its `add esp,0xc` NOT merged with the next call's**: one `int r = 0;` and one final
  `return r;`; the failure arms each call the error function and JOIN before the common `evt_Stop(sc);` (the join
  label splits the add-esp run), the success arm assigns `r = f(..)`. `return 0` in each arm (= ours, 8 bytes
  short) and an if / else-if chain with the stop call per arm were both wrong (evt_DeclareLocal 43 ins -> byte).
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

- **A float range test whose `fstp st(0); return 0` (lower bound failed) lies between the upper bound's
  reject and the accept**: `if (!(x < min)) { if (x > max) goto reject; } else return 0;` (ai_PassesPartnerFilter
  byte, s12).
- **A return block copied into one arm with its own `add esp`** (`add esp,0x10; push; call; add esp,4; pop; ret`
  where ours merges `add esp,0x14`): no `f(); return;` in that arm, let it fall to the one final call; VC6 copies
  the return block after add-esp merging (init_OpenBuilding byte). A dead `cmp reg,K` in an argument setup = an
  arm `if (s == K)` with the same call in both arms, merged (init_TextureSetHook). A jump into the middle of
  a sibling test chain (`cmp al,0x1d; je grave` then a re-test) = the value as one more `||` term of the outer
  test plus the inner test (init_OpenBuilding).
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
- **A guard `if (k != 4) return 0;` with its own `xor eax,eax; pop esi; ret` copy, ours jumping to the search
  loop's shared not-found `return 0`**: the hit `break`s out and is handled after the loop (`if (j == 0) return 0;
  f(j); return 0x10;`); the in-loop `return 0x10`, `continue`, while form and `else {}` were one code
  (ai_ExecuteBuyBuilding byte). Same for a slot scan whose `if (i >= N) return 0;` test sat inside the found arm
  of a do/while: `for (..) if (free) break; if (i >= N) return 0; fill` (d2_StartFade byte).
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

- **Size queue s11 (all byte or padded size by these, one round each)**:
  - Arms that each end `release; free; return;` where the original duplicates ONE such block into both: the arms as
    `if (id == 0) {..} else if (id == 1) {..}` with the release / free / return once after them (he_RunMasterExam).
  - A switch whose stage 0 tail cross-jumps into stage 1's in the original (ours the reverse): `break` in every
    case instead of `return` (he_RunGraveyardWorkers).
  - An `add esp` merged across the arms' join in the original (`add esp,0x14` after the shared call), ours `add
    esp,0xc` before the join: the shared tail (`SetViewEnabled(win, 1); redraw = 1;`) written in each arm of one
    if / else-if chain (ai_ManagedBuildingWindow).
  - Several `return 1` / `return 0` guards with own epilogue copies, the original jumping to one block: `if (ci >=
    N || chars[ci].id == 0xffff) return 1;` (cm_ExRelations), `if (*p == 0x13) { work } return 0;` instead of `if
    (*p != 0x13) return 0;` (gm_MayTakeJob).
  - A goto chain into a shared check: one `if / else if / else` choosing k and the work under `if (k >= 0)`
    (cm_ExChrMoveToUniverse).
  - `alive = 0; if (A && B) alive = 1;` against the original's `xor esi,esi` in the failure path: the flag set in
    both arms (ai_MistressAffairWindow).
  - A loop counter zeroed before the `if (n > 0)` that guards the loop (`xor ebx,ebx` before `test edi,edi`):
    `i = 0; if (n > 0) { ..; while (i < n) {..; i++;} }` (he_RunGraveyardWorkers); a slot walk `s->state`, `s++`
    where the original indexes `w->slots[i]` (he_RunWalk).
  - A record pointer's `+ 4` added after both calls in the original: the record pointers as locals and `->name`
    at the use (cm_RunWedding, GMHEAD).
  climb.py found none of these (its best rows were meaning changes: a local read before a call that writes it).
- **Size queue s13 (byte or padded size by these)**:
  - A he_ run handler's switch whose stage tails cross-jump the other way round (`cm_SendExSetHEI(..)` /
    `stage = 4` copies): `break` instead of `return` at the end of SOME cases (found per site with a generated
    round: each `return;` of the switch turned into `break;` alone, then all but one, then the pairs of the
    movers; most sites are = control, one or two decide it), plus the arm of an `if` that ends a case written
    with its own `return` (`if (--left <= 0) { ..; send(3); return; } send(1); break;`), or the good case
    tested first (`if (b != 0) { ..; break; } stage = 4; break;`) (he_RunPlague, he_RunBrawl byte).
  - Four `return -1` with one shared epilogue in ours, an inline copy each in the original and the success
    return last: the search loop breaks with a found flag and the returns follow the loop (`if (!found) return
    -1; if (r < 0) return -1; return c[r]->netId;`); the returns inside the loop, a `for (;;)` / `while` form
    or `if (i == 0)` after a break loop (an extra `test; jne`) are not it (ai_TrustedOfFour byte).
  - Stage 0 of he_RunTavern: an AI owner with no object gets its OWN day-later block (`if (t != 0) { if
    (choose) hire else later } else later`); `t != 0 && choose` (any arm order or inversion, VC6 canonicalizes
    them all) shares one block and costs a send call (7 vs 6).
  - A field value block (`no`, `none`, `noId`, `out[3]` sent by address to cm_AddFieldDelta / gm_HandOutMoney)
    declared at function scope: the next pointer load waits for their stores; declared in the block that fills
    them, the load moves above them (all four needed) (he_RunTavern).
  - A sum the original computes once and reuses (`movsx y; movsx h; add`), ours reassociated (`h - f6->h + y`)
    and computed again for the next statement: the sum assigned inside the test and reused (`py < (bottom =
    o->y + o->h) - f6->h` .. `a = bottom - f0->h - py`); no spelling of the plain sum moves it, a statement
    before the test is computed on every path (d2_SliderInput 66 -> 10 ins, padded size).
  - Frame 8 bytes short with two 3-float vectors: both `D3VECTOR4`; the loop's coordinates loaded onto the FPU
    and one spilled into the vector's slot = the vector filled first (`diff.x = (body->pos.x); ..`) and reduced
    after (`diff.x -= here.x; ..`) (he_FindNearestSpying byte).
  - Open: a memory-homed result flag (`sent = 0` at the top, `sent = 1` in the loop) whose zero store the
    original keeps in the first block: with it there every early `return 0` keeps an inline epilogue (probe
    `{ static int *p; p = &sent; }` removes all tail hunks); ours sinks the store and the returns share one
    block (ai_DrohbriefAmountCB, the forms tried in its note).
  - The tail copy of an if-arm with different registers from the shared tail (d3_SetFog: copy eax/edx, tail
    ecx/edx/eax) was duplicated BEFORE allocation, not by the post-allocation return-block copy.
- **Size queue s14 (byte or padded size by these)**:
  - Every switch case ending `X(); sv_CameraFollow(); return;` where the original has `pop ..; jmp
    sv_CameraFollow` per case: the cases `break` and the shared call stands once after the switch; VC6
    duplicates the tail call + epilogue into each case itself (ai_MasterLetterWindow byte). A retgen
    return -> break round is only valid when nothing follows the switch: with `e->exSlot =
    cm_SendExSetHEI(-1, ev);` after it, turn only the sites that end with that same send into `break`
    (dropping their copy) (he_RunDarkCorner: the plain round's rows were meaning changes).
  - `fxch; fild int; fmulp st(2); fxch` in a compare where ours has `fimul` / `fild; fmulp`: a `(float)`
    barrier on the double product (`o->fill * (float)(chance * 0.01)`, `p->price > (float)((1.5 - liking) *
    factor) * value`); the cast emits no code (he_RunTransport, he_RunBuyBuilding). A price loaded after the
    call but before the rest: the call's result in a float local first (`liking = gm_Liking(..) * 0.01f;`).
  - A 64-bit constant argument the original loads into ecx/edx before a branch (`mov ecx, lo; mov edx, hi ..
    push edx; push ecx`): a two-dword union local (`union { struct { unsigned lo, hi; } s; __int64 q; }`)
    set by halves, as am.c's other ai_PickActionBuilding callers do; an `__int64` local or the literal
    propagate to pushes. Where the stores stand decides the scheduling (after `ch = ..`: byte)
    (he_FillWalkSlot).
  - A dead `lea reg,[edx+eax*4]` of a type record after the field read: the record pointer is assigned
    again there (`bt = &ai_BTypesT[b->type];` before its use; the original's dead store) (he_RunBuyBuilding).
  - An early guard `if (buyer == 0 || b == 0) { he_FreeEvent(ev); return; }` instead of the body under
    `if (buyer != 0 && b != 0)` + a shared final free: the guard jumps into the LAST copy of the tail, as the
    original (he_RunBuyBuilding byte).
  - A guarded test whose failing path the original jumps straight past (`jne ret; jmp next`): the condition
    written positive around the remaining work (`if (id != -1 || (ok && active == slot)) { .. }` + one
    return) instead of `if (id == -1 && (..)) return;` (he_RunMasterExamPassed).
  - A loop counter the original keeps in a register while ours spills it (ours: the loop's `!= 0` exit
    leaves it unreferenced afterwards): `left > 0` (he_RunConversion); fields read by index
    (`ai_CharsT[ring].id`) with `c = &ai_CharsT[ring]` only for the calls give the original's `shl eax,2`
    offset CSE. A record pointer from a dead parameter's slot (`lea ecx,[esp+n_slot]` for the out pointer):
    the out local declared in the block after the parameter's last use (he_FillWalkSlot).
  - A long-lived flag initialised before a call loses its register to a parameter: its `= 0` inside the
    test that follows the call (he_CatchMeetingDisease: a outranks ch, 78 -> 2 ins).
- **Size queue s15 (byte or padded size by these)**:
  - A loop-invariant table byte the original addresses before the loop's guard (`lea edx,[base+idx]` in
    the first block, `mov al,[edx]` in the preheader) where ours tests a hand do/while: a pointer local
    to the byte as its own statement before a plain `for` search loop (`cls = &ai_BTypes[*b * 0x24d];
    for (i = 0; tab[i].kind != 0; i++) if (tab[i].kind == *cls) break;`) (init_SetCharacterAnimations
    93 ins -> byte).
  - A global's load into a register scheduled after the last use of the value that held that register
    (`sub edi,ecx; cmp edi,K; mov edi,[g]; jbe`), ours with t spilled: the global read directly in the
    test and the statements after it, not copied into a local before the test (the global is an
    allocation candidate whose range starts in that block) (snd_UpdateMusic 9 ins -> byte).
  - A zero vector stored with `mov [..],0` immediates where ours caches 0 in a callee-saved register
    (`cmp reg,ebx` / `push ebx` everywhere): the vector is a float vector (`D3VECTOR4`, `0.0f` stores),
    so the integer 0 loses four references and stays an immediate (gm_CreateCharModel 62 -> 18 ins,
    padded size). The original's `mov eax,[p] .. mov edi,eax` for `p ? p : &zero`: its own pointer
    local (`src = p != 0 ? p : &zero`), not `p` reassigned.
  - `xor esi,esi` in the first block feeding several zero stores and a later `bld = 0`: the `bld = 0`
    stands with the other inits at the top (before the early guards), not after them (ai_GuildSpyWindow
    77 -> 14 ins, then the message fill permuted: byte).
  - A `-1` cached in a register (`or eax,-1; cmp ecx,eax; mov [msg.fromId],eax`) where the original
    compares and stores immediates: the fromId store later in the fill (after to / arg[0]), found by a
    5-line permutation round (ai_RenovationWindow byte). Its two payment arms each keep the whole tail
    (`cm_SendMoveGoods; cm_SendLongMessage; cm_EndGroup; sv_PopRequest = 1;`): VC6 cross-jumps them from
    the SendLongMessage call on, so the add esp of the move call merges into one 0x1c (68 -> 28 ins,
    padded size).
  - A meaning bug read from the slots (cm_MessageBox): the answer started at -1 in ours; the original
    zeroes ret's slot in the first block and keeps the -1 in its own slot read as the slider handle.
    Read which slot each early constant goes to before treating a constant-in-a-register hunk as
    allocation.
  - No lever (one code each): every second-read spelling of a table entry the original reloads
    (init_NameBuildings: the reload is a rematerialisation of the memory-homed b, not source); the
    out-of-line placement of one if-arm after the epilogue (gm_CreateCharModel: polarity, nesting,
    goto to a label after the return, a ternary for the pointer, an ok flag).
- **Size queue s16 (byte or padded size by these)**:
  - A `volatile int z = 0` read back as `k = z` (the draft's way to get the original's load of a slot
    zeroed at entry) was `k = 0;` at the top of the function and nothing before the loop that starts
    from it (`for (c = 0; k == 0; c++)`): k is live from the entry store, so VC6 keeps its slot and
    loads it there (ai_CalcMeisterRaeuber byte). Read a volatile as "which plain local is live from
    where the original stores it".
  - `cmp ebp,8; mov edi,ebp; jl; mov edi,8` = a NEW local (`want = min(n, 8)`), not `if (n > 7) n =
    8;` (that gives n a memory home and stores 0 into it at the top); a -1 fill after a counted loop
    without a store of the final index = `for (; t < 8; t++) ids[t] = -1;` from the count, not a
    fresh `i` (i's address went to a call elsewhere, so its final value is stored); `cmp esi,2; jl`
    = `k >= 2` (`k > 1` gives `cmp esi,1; jle`); two stores of bestw (edx / ecx) cross-jumped at
    `best = bb` = `if (best == 0) { bestw = w; best = bb; } else if (cmp) { bestw = w; best = bb; }`
    (ai_CalcMeisterRaeuber 131 ins -> byte, raw 1956 -> 1937).
  - A scalar whose address goes to a call for an unrelated value (`&best` as the bounding radius,
    `&v` as its center) is in memory everywhere and conflicts with every slot: the call's own locals
    (`D3VECTOR center; float radius;`) brought the original's lea [esp+0x20] / [esp+0x14] and the
    padded size (he_RunConstruction 202 -> 70 ins, with retgen sites 0+1 and stage 3's bld test
    written positive: `if (bld != 0) { ..; send(stage + 1); return; } send(-1); return;` so its
    failure send cross-jumps into stage 1's copy as in the original).
  - The x87 stack order of loop-carried floats follows their init order (`ang = range[3]; speed =
    range[0];`); an invariant the original hoists AFTER another (g * 2 before 1 / g) was written
    inline in the loop (`1.0f / g * s * speed`), not as a local before it; with equal reference
    counts the frame packer takes the symbols in declaration order (g declared before speedStep:
    speedStep, not g, takes the dead parameter slot); `int sign = 0` at the declaration put the
    `xor ebp,ebp` first, whose register the float `hdir.y = 0.0f` store then used (ai_SolveCannonAim
    208 -> 14 ins, padded size).
  - `goto done` out of a search loop whose done is `info->z1++;` = `info->z1++; return;` in the loop
    like the other branches (ai_SendJobOrders, padded size). A goto to the loop's continuation that
    ends with the while test was a plain break (ai_LiquidateAssets: same code).
  - climb.py round 1 found a memset moved above an index computation (`memset(&msg..); i = id -
    0xdf;`: 204 -> 42 ins) the sessions before had not tried; then the two arms' message fills
    permuted (720 + 5040) and `msg.flags = 1` moved from before the kind test into each arm (the
    original reads the kind before the store): byte (ai_TechTreeWindow).
  - An argument the original pushes from the computing register without movsx and recomputes per
    call (`lea eax,[edx+ebx+0x11]; push eax` from k) where ours has `(short)(k * 0x31 + 0x11)` to a
    K&R-declared callee (movsx) or, without the cast, a strength-reduced IV: the callee through a
    short-parameter view (`d2_WinOpenPicStyledS`, as sv.c / svtip.c) (ai_UpdateResourceColumns 110
    -> 91, ai_UpdateProdColumns 186 -> 161 ins). climb.py's store moves in a message fill or a row
    record are a cue for a permutation round of that fill (ai_StorageWindow 200 -> 165,
    ai_DarkCornerBuy 119 -> 94).
  - No lever: all 1023 operand swaps of five product lines, s / c declaration orders, d3_YAxis
    through a pointer or array view (the c products of ai_SolveCannonAim's success arm keep c first);
    block-local records in ai_MistressWindow / gm_FindHome (the escape lever does not reach their
    hunks).
- **Size queue s17 (byte or padded size by these)**:
  - A counter the original keeps in a callee-saved register through a fill path while ours spills it
    (and keeps the record pointer `me` there instead) was TWO counters: the count-only path (`max ==
    0`) counts in its own `cnt`, the fill path's `n = 0` stands after its memset (the original's
    `xor ebx,ebx` between `rep stosd` and `rep stosb`) (ai_FillFamilyTab 77 ins -> byte).
  - A row loop whose index the original strength-reduces to a byte offset (`xor ebx,ebx; cmp
    ebx,0x10`, `[esp+ebx+0x54]`) with the count n in memory: n bounds the loop (`n < 5`) and its own
    index i fills the arrays (`ctl[i] = ..; evIds[i] = ..; n++; i++;`); `i < 4` as the bound merges
    i into n (ai_NoticeBoardWindow 102 -> 59 ins, padded size).
  - A top-tested loop with `jmp` back and an up-counting index (`xor ecx; cmp ecx,edx; jge .. inc
    ecx; jmp`) where ours counts down: `k = 0; while (1) { if (k >= n) break; ..; k++; }`; `for (;;)`
    with the break, `for` with the bound and `continue` forms all count down. A found pre-test
    the original keeps (`test edx,edx; jne` before the inner search) and the walk based at
    `&list[1]` (`[edi-4]` / `[edi]`) come back when the compare reads `*p` again instead of an `id`
    local (ai_AddEquipmentNeeds 73 -> 10 ins, padded size).
  - Two arrays filled from one record before a print (`prices[0..2]` unrolled into ebp / ebx / edx,
    the picks through a byte pointer and a scaled counter): ONE loop over both (`for (i..3) {
    prices[i] = ev->pairPrice[i]; picks[i] = ev->pairPick[i]; }`); separate loops or unrolled
    stores = control (ai_CourtshipWindow 142 -> 15 ins, padded size). Before that, a frame 12 bytes
    short was a block-scoped `int prices[3]` declared twice (the click read an uninitialized copy):
    a meaning bug.
  - A `return 1` whose test jumps into another failure's epilogue copy (`je` to the create
    failure) where ours keeps an inline copy: the failure as the last arm (`else if (x1 != 0)
    remove(..); else return 1;`) or as the result test (`if (x2 != 0) o2 = alloc(..); if (o2 == 0)
    return 1;`); a `goto room` shared check was `if (kinds) y2 = y; if ((y2 != 0 || type == 0x1dd)
    && room < count) return 1;` (cm_ExSellObjekt 127 -> 103 ins, padded size).
  - A call the original makes in each arm (`snd_PlayFavourComment()` at both arm heads) lets the
    arms' slot-wait tails cross-jump (one `cm_GetSlotValue` copy) (ai_SearchResourceWindow).
  - Read the size gap first: ai_DuelDiceAnimation's 7 bytes are one parameter load per arm where
    the original loads it once before `if (fast)`; he_RunDecay's 15 instructions are a stage tail
    cross-jumped after allocation, blocked by a scratch rotation one step behind from the first
    out-of-line block on (open in their notes).
  - No lever: declaration orders for equal-size frame pairs (16 masks, = control); 16 clamp
    spellings (ternary, min both orders, if / else, set-then-if) x both polarities for an
    out-of-line `mov bl,3`; a local copy of a parameter (`m = mode`) to hoist its load; memset with
    -1 instead of 0xff (a -1 cached in ebx).
- **Size queue s18 (byte or padded size by these)**:
  - `mov edi,1; push 2` hoisted above a half test (`test ax,ax; push 2; je`) whose arms both run into
    the next `util_RandomU(2)` test: that next test written in EACH arm (both arms then start with the
    same instructions and VC6 hoists them); `d = 1` in both arms alone gave the `mov` but not the push
    (ai_MsgBuildingGetsItemType31 byte).
  - A vector's pad word stored from the cached zero register (`mov [esp+0x3c],ebx` for `a.w = 0`, while
    `b.y = 0.0f` stays an immediate): the pad is an INT (`struct { float x, y, z; int w; } a, b;`), so
    the integer 0 gains the references that give it ebx and every `push 0` / `cmp ..,0` follows
    (cm_UpdateGrenades 121 -> 99, he_RunSabotage 160 -> 142 ins; cm_UpdateBombs has the same pad from
    edi). A float `w = 0` folds to a float constant and never uses the register.
  - A case's `stage++; return;` the original reaches by `je` into a later case's copy, ours inline: the
    test written positive with the other work in the if and the `stage++; return;` after it
    (he_RunSabotage stage 2: 142 -> 137 ins, padded size).
  - A memory-homed accumulator zeroed through a scratch register in the prologue (`xor edx,edx` before
    `rep stosd`, stored after): its `= 0` as the function's first statement (climb.py round 1 found it:
    gm_BuildingBalance 95 -> 41); then the arms' store order from the listing (`found = 1; out[..] +=
    wage; cost += wage;`, `out[6] = f(); cost = out[6];`): 27 ins, padded size.
  - A byte local that must take dl while a global pointer CSE takes edx first (dealt / ai_PokerTable):
    the priority race is lost by the char because its other compares go through the zero-extension
    CSE temp; unsigned short (any register) gave the padded size and only the two registers swapped
    (ai_PokerPickStrategies 79 -> 24 ins).
  - `push ebp` for literal zeros right after a `left != 0` loop: VC6's equality propagation (in the
    block a `left == 0` test guards, left replaces the 0 and gains the references that rank it);
    `if (cnt < n && left == 0)` shows it, but the original has no surviving left test (he_RunConversion,
    open).
  - No lever: the || / && polarity of a 15-compare item chain and its arms' order (VC6 canonicalizes
    both; ai_StartProduction's layout is the chain's jump edge); the goto-free d2_SliderInput (the
    clamp in each arm: 10 -> 95 ins); the factor order of `t * (1.0 - dist * 0.01f)` (one code).
  - climb.py moves that cross a call into a record fill (ai_TechEffectsWindow: `style = 3` after
    d2_SetCaption, 215 -> 141 ins) are not the original's order: read the listing before keeping one.
- **Size queue s19 (byte or padded size by these)**:
  - The s18 int pad word took cm_UpdateBombs 43 -> 21 ins (padded size); then two plain spellings to
    byte: the random damage written into t's product (`t = (int)((cm_Random(..) + w->dmgMin) * (c->f01c
    * 0.01))`, no `k` local: the original's frame slot for t and its fild t / fld dist order), and the
    table entry tested and read through the table with ch first (`if (ai_SceneCharsT[j] == 0)
    continue; ch = ai_SceneCharsT[j]->scene; c = ai_SceneCharsT[j];`: the original's `mov eax,[esi];
    .. mov ebx,[eax+0x184]; mov edi,eax`, a CSE temp copied into c). The same two took cm_UpdateGrenades
    99 -> 78 ins.
  - A frame 4 bytes short with a hole of 8 behind a vector in the original = the vector is a
    `D3VECTOR4` (cm_UpdatePointer's v; `dcp frame` shows the hole).
  - A callee summary that needs a write the leaf's code does not show: a dead `if (0) g = 0;` in the
    leaf (with an @original comment naming the caller's reloads) makes VC6 count it as writing g; the
    leaf's bytes stay equal (cm_UpdatePointer 142 -> 36 ins, padded size, via ai_LedSelectBits). Only
    when the reload names the global (here the caller reloads exactly ai_PickedObject); where any
    global would do (cm_RunCutOrders / cm_CharObjDistXZ) the form is unknown and not taken.
  - A variable memory-homed in the original while ours has it in a register (gm_CreateObject's
    `last`, he_RunAmbush's `n`: `cmp [slot],1; push 1` where ours propagates n into `push edi` inside
    the n == 1 arm): a probe `{ static T *pp; pp = &v; }` shows whether that is the whole hunk
    (gm_CreateObject: frame equal, regs 76 -> 12); its ordinary cause is still open (not the walk's
    loop form, not one local for two roles).
  - Read the size gap's place first (ai_OpenItemTooltip: all 24 bytes in one compare chain whose
    arms the original does not cross-jump; ai_CalcAngriff: the text tails' `lea buf` scheduled before
    the format push keeps them apart; d3t_CreateSnowMipmaps: the alloc's size * size is recomputed in
    the original, n = size << i reaches the size but is not its imul): forms that hit the size but
    contradict the listing are not kept.
  - No lever (= control): return polarity of `if (kind != 0) return 16; return 0;` tails
    (ai_DecideBuyBuilding), list[n++] forms and all 719 orders of a six-compare chain
    (ai_OpenItemTooltip), the alloc argument through the just-stored struct fields (forwarded).
- **Size queue s20 (byte or padded size by these)**:
  - A frame larger than the original's with `dcp frame` "shared in the original" for an escaped local (its
    address goes to a call: `&reward`, `&home`, `&ground`, `vals`) = that local is a BLOCK local of the
    block that uses it. A function-scope local whose address escapes conflicts with every slot; block locals
    with disjoint scopes share one. he_RunRobbery / he_RunBurglary (reward in case 4's `if (b != 0)` block,
    vals in the loot block: the frame exact, he_RunBurglary 348 -> 108 ins; its reward / report UNION was the
    draft's stand-in for this packing), he_RunTrade (home in `if (t->bld == -1) {..}`: frame + padded size
    with the index loops below), he_MoveReaper (each d3_GetHeightAt call's `int x, z; float ground;` in its
    own block: ground shares with step, h with dist, x with the fild temps; 118 -> 4 ins, padded size). The
    same for a non-escaped local used in two disjoint places: two `int step;` block locals
    (cm_RunCouncilSession: frame exact; the original pairs each step with another variable).
  - A value the original loads into a callee-saved register at the head of an arm and keeps across two
    calls until a call argument (`mov edi,[d3_Scenes[0].hmap]` before the clock math, the scene spilled to
    its slot and reloaded for the final call) = a named local of that arm (`D3SM *hm = d3_Scenes[0].hmap;`),
    while the first call of the function reads the global at its push (he_MoveReaper).
  - `mov cl,[t+eax*4]; lea eax,[t+eax*4]` (a dead lea after the field load) and `lea eax,[..]; mov al,[eax]`
    = the field read through a record pointer local (`bt = &ai_BTypesT[b->type]; if (bt->kind > 0x1f) hi = 1
    << (bt->kind - 0x20); else lo = 1 << bt->kind;`, `if (bt->kind == 0x16 || bt->kind == 7 ..)`), no kind
    local (he_StepWalk 86 -> 48 ins).
  - Hand-advanced slot pointers (`for (i = 0, pb = t->sellBld, pg = t->sell; ..; i++, pb++, pg++)`) -> the
    plain index loop over `t->sell[i]` / `t->sellBld[i]`: he_RunTrade 420 -> 192 ins by itself.
  - A counter memory-homed in a dead argument slot (`inc dword ptr [esp+0x24]`) = its `= 0` stands early, live
    through a fill it does not take part in (gm_ListWorkers: `n = 0` and `p1 = out + 1` between the two 0xffff
    fills, 74 -> 54 ins; climb.py round 2 found it, a 2520-order permutation of the seven inits confirmed it).
  - A clamp whose constant path the original runs through the next compare (`fld 0.25; fcom 5.0` where ours
    jumps past it): only `f = (t = t > 0.25 ? t : 0.25) < 5.0 ? t : 5.0;` stops the threading, at the cost of a
    `fld st(0); fcomp` copy (gm_StockPrice, not kept; 13 forms tried).
  - No lever (= control): an inline `ai_BTypes + *b * 0x24d` instead of the k local (not hoisted:
    gm_ListWorkers); `unsigned char stage` for the original's `mov edi,ecx; and edi,0xff` widening (a byte
    slot, frame +8: he_StepWalk); `count = n2 + n3` spellings against a strength-reduced `&rows[row]` temp
    (ai_TrainingWindow); `n = 0` moved into the case that counts (he_RunRobbery: the zero stays in ebp).
- **Frame sweep s21 (the escaped-local lever over every function below byte)**:
  - The candidate list: `tools/sweep/framescan.py` (each below-byte function's `dcp frame`: the frame size differs
    or `shared in the original:` lines, AND a function-scope local whose address goes to a call); 101 of 611.
    The names in a shared line that are themselves escaped locals were the real leads (about 25); a frame
    difference alone mostly was not this lever. `--vec`: ours smaller with D3VECTOR locals (8 functions).
    `tools/sweep/blockscope.py --scalars [--lists]` then tries each local in its blocks (`--lists` splits
    `int a, b;` lines first: most escaped scalars sit in such lists).
  - Byte or the original's frame by the lever: he_StammtischCommand (s1 / s2 text buffers declared in each
    of the two branches that format them: byte), he_LabIncident (one / step of a cm_AddFieldDelta pair in their
    if block, then the possible[] zero fill as an element loop, not `= {0}`: draft -> byte), he_RunRansom
    (none / bld / obj in their blocks: 20 -> 4, frame), gm_SyncMarket (cnt: 22 -> 14, frame),
    ls_WriteCharacters (the -1 for a missing id is `int tmp = -1` of each else block: 42 -> 4), cm_ExGebUpgrade
    (key in the block that sends it). Out-parameters of a call inside a loop are locals of the loop body
    (ai_DarkCornerMenu price / who 73 -> 47; ai_TransportFromLager inv per loop 12 -> 8). Values rolled in
    two disjoint blocks (the non-player branch and the ok click of the lcamt privilege windows) are locals of
    each block: ai_HatredWindow r1 / r2 / o1 / o2 40 -> 24, ai_ConvertWindow faith / skill 50 -> 14 (the
    byte siblings of that family already had this form). ai_FeastWindow none 202 -> 108, ai_PersonnelBook
    wage 748 -> 606.
  - **Ours smaller than the original by 4 per vector = D3VECTOR4**: he_SecretRoomCatchSpy (d / there: byte),
    he_SpreadGfxInfos (pos / at: byte), d3s_Update (ldir / lpos / opos: 22 -> 2, then `mesh = obj->info;
    info = mesh;` byte), d3s_UpdateObjectAt (lpos / ldir 48 -> 22), cm_ExGebUpgrade (v, with path[0x104]: 76 ->
    14 at the frame). Cast at the calls (`(D3VECTOR *)&pos`), as he_AimReaper does. A subset that does not
    reach the frame is not the original's (d3sm_Build, gv_ExplodeShot: only the slot table moved; not kept).
  - Buffer sizes: a frame 4 off with a 0x100 / 0x104 buffer: path[0x104] (MAX_PATH, cm_ExGebUpgrade: the
    next buffer sits 4 further), name[0x100] not 0x104 (init_SoundObjectHook: frame, 12 -> 9 bytes). A
    0x104 text in the ai_UseGood windows overshoots (their frame is equal in `dcp try`).
  - One buffer for two texts that `dcp frame` shares (ai_OpenMasterListPage nameText / budgetText as one
    text[0x100]: 12 -> 8); ai_AlchemistResearchWindow's msg / line share fails (frame 0x164): the pairing is by
    alignment, check the frame size of the merged variant.
  - Not this lever (= control or worse): scalars reused across disjoint uses when neither is escaped
    (gm_UpdatePrices a, snd_UpdateAnimals n / c, ai_PlayerStatsPanel value: an extra delta local is worse),
    he_RunSpying (lo spilled: record pointer, block lo / hi = control), he_AimReaper (target / at slot swap),
    ai_BuyBuildingOfferWindow (msg in its block), cm_ExChrGotoBuilding (y as a block local: frame -4).
  - `dcp try` pitfall met again: a `<<< old === >>>` block with an empty replacement swallows the next block;
    write the declaration's removal as a two-line old text with one line kept.

- **Near-byte sweep s22 (the functions within 12 instructions, a few minutes each)**:
  - The list: `tools/sweep/nearbyte.py` (`dcp grade UNIT` over every unit with a function below byte; shape and
    draft at most `--max` instructions, sorted). 157 at the start; about 75% had no `@deep` note at all, and those
    were where nearly every gain came from (the noted ones had had their quick levers tried).
  - **A hand-walked pointer is the first suspect**: the plain index loop was byte in gm_RefreshGoodsLists /
    gm_SyncMarket (`for (lp = list; *lp; lp++)` -> `list[i]`), ahm_FindRecipe (a do-while with `p++` -> `for`
    over `table[i]`), cm_FightCutEntry (`r++` in the for -> `r = &table[i]` per row). The reverse once:
    cm_AddFieldValue's per-case copy walks the parameter itself (`*((T *)v)++`, VC6's cast-as-lvalue: v is loaded
    after each loop guard; a typed local loads it before).
  - **Exits**: a search loop with `break` and one `if (k >= 6) return 0;` after it (VC6 decides the test on the
    loop's exit and keeps a separate `return 0` copy for the break path: gm_IncomeKind1 / 2 / 5, as kinds 3 / 4
    already were); `if (r != 0) return 0; return 1;` against the reverse (cm_ExOfficeSwap); the work under
    `if (count != 0) { .. return x; } return 0;` (cm_LobbyChatFirst); the guard returning -1 first
    (gm_CharMoney); a field read at each use instead of a local, so `(count + 2) * 5` folds into one lea
    (gm_ListStock); a parent test as its own if with `return level` inside (gm_CharLevel / gm_CharPrevLevel: the
    original has one return more); a switch case's `cm_WaitTicks(4); return;` as `break;` into the same code after
    the switch (cm_DuelRound: the callee-saved pushes move behind the guard).
  - **Record and message fills**: permute the fill (`tools/search/permute.py`), both fills of a function together
    (gv_CheckRankUp: type, to, flags, fromId, time in both, byte only as a pair; he_CharIllness /
    he_CharAccident fromId before the time copy); the state store first in a G17STAT fill (cm_ExDecisionValue);
    `rounds = 0` before the clamps (he_AllocMeeting).
  - **One named variable more or fewer**: a call argument into the result variable first (`rate = 0xe - cur;
    rate = f(rate, ..)`: gm_CharacterUpkeep); the summed value into the reused index (`k = c->skills[k] + d;
    f = (float)k;` with the store indexed by the message field: cm_ExAddSkill; a new local `v` was = control);
    `t = k / v; v = v - k + t;` (gm_ExpectedDeathAge); dx / dz locals for a distance (ai_FindNearestObject);
    the sign as its own `int sign = -1` block local first, not the reused parameter (ai_CommandLawChange, the
    s21 note's "kinds slot" was this); a day number reusing the dead k and the fraction its own float
    (sv_UpdateSkyPhase: frame slots); `n = 0` declared first when its zero is pushed for a call before the loop
    (he_AllocMasterWork: `push ebp` for the constant).
  - **Operand order that VC6 keeps**: `s * (level * 0.01)` (gm_BuildingSkill), `v * (n * 0.01f)`
    (ai_CommandCityTreasury); `(float)price / log10(price)`: two fild of the price in the original (one shared
    conversion when both operands are the int): ai_ExecuteBuyIndulgence, ai_IndulgenceWindow. Plain operand
    swaps were = control nearly everywhere.
  - **Slot ties in frames above 0x80** (unstable density sort): two equal arrays follow their read order after
    the loop (`j = js[r]; o = os[r];`: ai_MsgJobTime); s21's block locals again (he_RunRansomDemand: none, bld,
    obj in both sending blocks, all three needed).
  - A prototype: the player byte pushed without zero extension = the unit's `sv_CheckGoodAmountB` view
    (ai_AskBonusAmount / ai_AskHelperBonus), then the init order ret before mask.
  - climb round 1 found three (gv_RankCulprits `c = 1` first, cm_OpenCharInfoWin y0 before w,
    ai_FindNearestObject's p[2] local, written as dx / dz); it found nothing on most others in about a minute.
  - Left (noted): fcomp of a parameter before the call's `add esp` (he_CharIllness / he_CharAccident), the filter
    dword loaded before `rep stosd` (ai_BewitchOrder, ai_HomageWindow), two-temp swaps at a call (cm_FindNearCell,
    cm_UpdateDamagePops), bld / chr slot tie (he_RunGeselle), byte parameter kept in bl (gm_RandomAge).

- **Deep s23 (the near-byte list to 20 instructions, the functions without an s22 note first, 30 items + twins)**:
  - **flip.py on the hunk, then read the DELETE hits**: a hit that deletes an assignment `v = global;` /
    `wz = snow->windz;` means the original has no such local: the global / field is read at each use and VC6 keeps
    it in a register itself (he_RunStormEvent: `ai_Time.hour` at each use instead of `base = ai_TimeHourW`, byte;
    d3t_UpdateSnow: `wz = (snow->windz)` -> `wz = snow->windz`, 8 -> 4). A hit that duplicates a statement means the
    value has a second reference (cm_WaitTicks, not found). Hits spread over many unrelated statements (deleting
    any store above) = a value-numbering tie, no lever in the hunk's own text (he_AllocMeeting, d3_InitObject,
    cm_QueueSend, io_CharacterName).
  - **A wrong field read off the frame**: a `dcp frame` slot `<- moved` whose original slot is a DEAD PARAMETER
    slot reused for an out-parameter, stored into a record field at +0x20 where ours writes +0x1c, was the
    meaning (cm_OrderWare: storeX / storeZ = the cell x / z of d3_GetHeightAt, not held = a / turn = a byte of the
    point; fix + byte). ai_BurglaryStart: the map pick record's stores at aligned offsets +8/+0xc/+0x10/+0x4c
    where ours wrote req[4..7] (fix; buf 0x80 for the frame 0xd0): check the stores' offsets in `dcp disasm`
    with the pushes counted before calling a fill hunk "order".
  - **A 64-bit global copied as its two words** (`cfg.classMask[0] = gypsyFilterWords[0]; [1] = ..[1]`): the
    original loads the high word before the AMOUNTCFG memset's rep stosd; `cfg.filter = gypsyFilter` loads both
    after. ai_UseGoodCharmOfficials, ai_BewitchOrder, ai_HomageWindow byte; ai_SermonWindow and
    ai_UseGoodCharmPartner draft -> shape, ai_HatredWindow 24 -> 16 by the same edit (tools: a script over every
    `X.filter = gypsyFilter;` of a below-byte function, one `dcp try` each).
  - **A named local for a call argument computed by a ternary** (`fl = p->sex != 0 ? F1 : F0;` then the call):
    the original loads the record pointer before `sub esp` and pushes the buffer address after the flag byte
    (ai_FindCandidateIds, its twin ai_DecideSuitor: byte).
  - **Clamp then one send** = `min(max(v, 0.0f), 1.0f)` (ai.h's macros), not nested ifs with a send per arm and
    not a ternary (ahm_SendSetPlaceValue / ahm_SendSetShare byte; the arms' copies were in the wrong order).
  - **Decompiler loop shapes**: `if (n > 0) { j = 0; do { .. } while (j < n); }` -> `for (j = 0; j < n; j++)`
    (amt_IsColumnDecided byte); the box corners walked by `v = (float *)box + 0x16; v[-2] ..; v += 0x14` ->
    `for (i = 1; i < 8; i++) box[i].x ..` and a search loop with a `goto` out of it -> `for` + `break` +
    `if (i >= n) {..}` (init_LoadSiteScene byte).
  - **Exits**: `if (x == -1) return 1;` guard first (cm_CheckCutsceneReady); the paid branch's `free; return;`
    as an if / else falling to the shared final free (he_RunDeathDue); the object / building / none universe as
    one if-chain and ONE `if (i == -1) return 2;` after it (cm_ExChrWalkToDummy); `cm_EndFightScene(a); return;`
    of an early branch shared with the other branch's (cm_RunFight); the queue test `if (cm_SendQueue != 0)`
    walk first (cm_QueueSplit).
  - **Block locals and inits**: a block local for the value one branch converts with fild (the original fild's
    from the temp slot: snd_UpdateAnimals `int c` in each of three atmosphere arms); `int scene` in the `if`
    that uses it (main_BeginPlayerRound); a record (G28REC party) in the block that sends it (he_RunOffspring);
    `x = 0` / `nb = 0` written after the first loop / call instead of as an initializer: the callee-saved pushes
    then sit after that loop (cm_BuildPortraits, cm_RunFight); two `= {0}` records in the other declaration order
    (rt_LoadAtPoint: the zero fills follow the declaration order).
  - Store orders by permutation (ai_AgendaOfficeList `found = 0` first, ai_Screenshot width before height +
    the bit masks before the bit count, he_RunOffspring stage before year++); the arms of an if swapped so the
    original's arm falls through (gm_JobWage); dist as dx / dy / dz locals with D3VECTOR4 points
    (gm_NearestBuilding).
  - dcp tooling: `dcp try --apply` re-encodes the unit (a Latin-1 byte in another function's string became
    U+FFFD and that function fell to draft); restore the byte after each apply (bug report
    try-apply-mangles-latin1-bytes.md).

- **Deep s24 (the near-byte list to 20 instructions, notes older than s22 first, 15 items)**:
  - **One `return;` of a switch case as `break;` decides which epilogue copy the pair merger keeps**: a stage
    handler whose `push esi; push N; jmp` tails jump into another copy of the shared `call; add; mov` than the
    original's (identical instructions, a few bytes of jump targets). Generate one variant per `return;` inside
    the switch (each turned into `break;`, the switch being the function's last statement) and measure them all:
    he_RunSecretRoom (stage 3's final send, 7 bytes) and he_RunTransport (stage 1's failed-route exit inside
    an `if`: draft 2 ins / padded size -> byte) matched with exactly one; all of them at once was far worse in
    the first. c2tail --tries shows why: the pair merger walks the return jmps in source order against one
    anchor copy, and a `break` makes the site a jump to the switch's end instead. Generator:
    `tools/sweep/returnbreak.py FILE FUNC ADDR OUT`; run over every below-byte he_Run* it matched two more
    (he_RunPlunderOffer, he_RunGraveLease: stage 0's final `e->stage = 1; return;` as `break;`). Only the
    functions whose hunks are jump targets / tails moved; on regs / code hunks it is = control.
  - **A message record in the block that fills and sends it**, again: he_RunRansom's AIMSG m (function scope:
    the victim->netId load came after the m.type store; in its block: byte). Run it before any fill
    permutation.
  - **An escaped local at function scope on purpose**: ls_WriteCharacters' `model` (its address goes to the
    write macro) is a function-scope local in the original: an escaped function local conflicts with every
    slot, so the KindB loop's block `tmp` packs with the record block's tmps instead of with `model` (slots 4
    -> byte; read the packer with `c2frame --names`: the `P` lines show which slot each local joined).
  - **Plain index loop over a list the original walked**: ai_FillSaveSlots `for (k = 0; k < n; k++) { rec =
    (SAVEREQUEST *)files + k; ..}` (VC6 strength-reduces it to the same countdown) instead of
    `for (rec = files; n > 0; rec++, n--)`: the rec spill moves behind the loop guard, byte.
  - **A `goto` past a test = VC6's own jump threading**: cm_ExLongMessage's `if (kind != 7) { if (kind != 6) { if
    (!s) return 1; goto set; } } else return 1;` is `if (kind == 7 || (kind != 6 && s == 0)) return 1;` (VC6 threads
    the s != 0 path past the later `s == 0 &&` test itself): byte.
  - **A `volatile` float reproducing an fst / reload = a block local**: d3_EnvMapCoords' d declared in each
    `if (v->used)` block (plain float) gives exactly the volatile's code; one plain function-scope d lifts it
    to the x87 stack (+20). Try block scope before keeping any volatile scalar.
  - Same code, kept for reading: a severity local of its own instead of a reused pick variable
    (he_CatchMeetingDisease), the horizontal length as the full sqrt(x*x + y*y + z*z) with y = 0 (VC6 folds the
    zero term: he_MoveReaper), `for (k = 0; n < 4 && k < nbones; k++)` for a for + break (d3anim_MapBones).
  - Left with notes: reloads of split pieces in ascending piece id at a loop exit (d3anim_MapBones,
    d2_ShpBlit16HalfDark), a threaded bottom exit (gm_BuildNext), a store before a guard (sv_MessageBoxGreen),
    an x87 operand order (he_MoveReaper target.y), fchs placement (ch_ActTurn), a = 0 before a call
    (he_CatchMeetingDisease), a scratch rotation (gm_Query).

- **Deep s25 (the near-byte list to 20 instructions, notes older than s22 first, 15 items: 9 byte + 1 twin)**:
  - **Run climb.py round 1 before anything else, also on items with long notes**: he_RunTraining (c = &ai_CharsT[..]
    before skill = t->skill: one swap), ai_FindCandidates (win = 0 before the memsets, cnt = 0 before the walk
    setup: two moves, round 2) and d2_DrawInput (the object id read once into a local in an unrelated branch:
    6 -> 2, after which the noted "135 ins" form bw = o->w - .. was byte) had notes saying "allocation only".
  - **VC6 tail calls (scratch compiles, tools: toycc.py pattern = c2dag.setup/env + CL /FAs)**: a call followed by
    return becomes `jmp` only when the call's argument count equals the caller's parameter count; arguments
    equal to the incoming parameters are left in place, others are stored over the parameter slots (`mov
    [esp+4],edx; jmp`), and that store form only for a callee whose body VC6 has seen (defined above). So a
    non-tail `push [esp+4]; call f; add esp,4; ret` next to a tail jmp in the other arm = the caller has more
    parameters than f takes: cm_FightMember is the 2-argument askPlayer callback and passes both to
    cm_FightLoop (byte). A tail jmp with a param store = a callee of the caller's arity defined above:
    ai_LawSelectionEntry calls a 4-argument stub (cls, filter, text, cb) that /OPT:ICF folds into
    sv_ReturnZero (an `unlinked` definition above the caller; exemeasure unchanged) (byte).
  - **A header prototype's parameters count in the declaration set**: giving two cm.h prototypes one parameter
    more (named or not) moved main_RoundReport, ai_TutorialInitChapter2/3 and sv_LoadGroundplan; keep the
    count (one prototype gains a parameter, the other is declared unprototyped `f();` with a comment).
  - **A pointer local to a global record (`T *st = (T *)global;`) is not the object itself**: gm_CountCharacters'
    `st->baseHealth[k] += f()` kept `[edx*4+X]`; through a view of the object (`#define gm_CharStats (*(T
    *)&gm_CharacterStatistics)`) VC6 computes the element address into edx before the call (lea; fadd /
    fstp [edx]) as the original (byte).
  - **A materialized record address (`mov ecx,eax; add ecx,ebx; mov cl,[ecx]` instead of `[eax+ebx]`)**: a
    record pointer local tested twice (`g = &ai_GoodsT[w & 0x7fff]; if (g->type == 2 || g->type == 6)`), the
    index variable assigned after it (sv_LoadGroundplan, sv_UpdateGroundplan at both slot loops: byte). The
    byte twins with that load (sv_HoverPick, gm_AllocRaum) had the pointer.
  - **`movsx ecx, byte ptr [..]; cmp ecx,2` instead of `cmp byte ptr [..],2`**: the byte is an int local with a
    second reference; any second test, even one VC6 decides away, keeps it a register value
    (init_AllocSceneBuilding: the Kontor test `kind == 2 && id == 0x47`, kept with @original).
  - **The sqrt result kept on the x87 stack over the dying vector slots (`fstp st(2); fstp st(0); fst [dist];
    fcomp lim`)**: the distance as its own float assigned inside the range test, `len = sqrt(..); if ((dist =
    len) > lim) continue;` (he_SpreadMeetingDisease byte; `dist = sqrt(..)` then the test, or the test on len
    before dist = len: the old code).
  - **A volatile scalar saved in ebp and in a slot**: two reads of the global (`hv = d2_ClipBottom;` and the
    clamp reading `d2_ClipBottom` itself), not a copy: same code as the volatile (d2_DrawInput).
  - Left with notes: a counter's rank against a pointer with its decrement first (ai_RandomDomizilGoodNotIn), a
    function-wide SIB operand order (ai_AlchemistWindow: flip names a swap in another block), fr's live range
    (d3_Animate), a store-order / load-hoist pair (ai_CommandExecution), the zero pushes as left's references
    (he_RunConversion), an induction-variable merge that only a short counter avoids (ai_TutorialWindow).

- **Deep s26 (the near-byte list to 25 instructions, notes older than s22 first, 15 items: 5 byte + 1 draft ->
  shape)**:
  - **A second variable that only mirrors the index is a decompiler artifact; write the expression**: a row y
    stepped by `y += 0x3e` next to `count * 0x3e + 0x14` in the same row (ai_OpenMasterListPage: the input
    box's y as `count * 0x3e + 0x14`, y dropped, byte); a bound `n = size * size` and a reused `i = y * size`
    (ts_LoadSoft: both written inline, `for (i = 0; i < tex->size * tex->size; i++)` and `data[y * tex->size +
    x]`, byte; climb found the bound, the index form alone was = control). The hunks sat in another
    statement (a scratch-register rotation, an address reassociation).
  - **A reciprocal local (`inv = 1.0f / factor;`) where the original loads the reciprocal first in a product**:
    `1.0f / factor` written at each use (VC6's CSE temp, its own slot) puts it first (`fld [inv]; fmul [fn]`);
    any operand order of the product with the local is one code (ai_TransportFromLager, both loops, byte).
  - **`neg eax; fild; fmul 0.01; fadd 1.0` against ours `fmul; fsubr 1.0`**: a double expression `-r * 0.01 +
    1.0` always folds to fsubr (every cast and grouping); a float conversion of the product keeps the neg:
    `(float)(-util_RandomU(0x1e) * 0.01) + 1.0` (or a float local) (gm_GetCreditOffers 7 -> 4 ins, draft ->
    shape). Toy compiles (`toycc.py` in the s25 scratch) settle such folds in one run.
  - **A redundant second test the original keeps (`cmp al,6; jne` behind the kind-7 path, the kind-6 path
    threaded past it)**: two ifs on the same field, `if (kind != 6 && kind != 7) {..return} if (kind == 6)
    {..} return 0;` (ai_MiracleWindow byte; the nested if / else forms put the return 0 inline).
  - `t->entry++; if (t->entry > 0x1a)` instead of `i = t->entry + 1; t->entry = i; if (0x1a < i)` (he_RunTip
    byte; the copy is what the original's `mov eax,ecx` was).
  - Left with notes: the info / v copy (sv_UpdateBand / Led), n's register against s1's (ai_ExecuteMasterBuilder),
    the x / w load order (d2_DrawSlider), SIB base / index orders (d3_SetObjectAlpha; gv_GrowGraveyard: only loop
    0's stores, so another IL form there), the exit block order (hmt_Load: 19 forms one code), a level temp
    (ord_LoadStatistics), the constants 6 / 1 in ebx / edi (ai_UseGoodCharmPartner), all = 0 before the guards
    (gm_GetCreditOffers: an esi / edi swap).

- **Deep s27 (the near-byte list to 30 instructions, notes older than s22 first, 15 items)**:
  - **climb.py round 1 first, again**: three of the 15 were one move or swap (gm_BuildQuery the class reset
    behind the master reset, ai_CalcMeisterFernhaendler `it = ai_Objects` before `ok = 1`, he_RunWorkerTheft
    `n = 0` before the memset) and ch_GetWaypoints' slot swaps were the action pointer's declaration initializer
    (`a = CH_ACTIONS(ch);` as the first statement: 10 -> 2).
  - **A literal held in a callee-saved register (`mov ebx,1; push ebx` .. `mov [g],ebx`) that ours pushes as an
    immediate**: one more reference of the constant, from a store the original wrote in two arms that VC6
    merges: `if (dm_L.rup) sv_PopRequest = 1; else if (d2_ClickedId == 0) sv_PopRequest = 1;` instead of the
    `||` (ai_MissionEndDialog byte; two separate ifs 3 ins). A named `ok = 1` local folds and does nothing.
  - **Callee-saved pushes in the prologue where ours shrink-wraps them past an early return**: one more
    `return` inside a branch (guide section 3): the no-target case as its own early return with its tail call
    written out, the rest flat (ai_GraveFogWindow), a search loop's `break` as `return i;` (gv_BuryNewDead).
    Both byte; the inverted guard / if-else forms were the old code in each.
  - **`xor eax,eax` in a shared failure tail where ours returns the known-zero pointer without it**: the alloc
    branch as `if (rec != 0) { ..; return rec->slots; } return 0;` (ai_ProdSetting byte): the failure no longer
    returns right after the zero test, and its je joins the other branch's `xor eax,eax` copy.
  - **A dead field load before a tail call (`mov ecx,[eax+8]`, unused)**: an if / else on that field with the
    same call in both arms assigned to the result (hmt_RandomTextOffset, kept with @original; two `return`s were
    8 ins, a ?: 13).
  - **`fild x; fmul c; fiadd [int temp]` against ours `fild sum; fild x; fmul; faddp`**: the product cast to
    float, `(int)((float)((left - 1) * 0.5) + (group * 3 + 0x155a))`: a float plus an int expression adds the int
    by fiadd; every double spelling (casts, operand order, a named sum, / 2.0) loads it by fild first
    (he_RunMasterExamPassed byte; toy compiles settled it in two runs). he_PostEvent67 has the same expression.
  - **A table indexed through a named k where the original's index is a temporary**: `t =
    txt_MaleFirstNames[(unsigned short)util_Random(0xbf)];` (main_CreateStartCouple: the second name copy's ecx /
    al instead of eax / cl, byte).
  - **It was a meaning bug, hidden by the x87 stack**: he_SetLifeByAge's original loads `c->f018` before the
    `fnum <= f020` branch and keeps it on the FPU stack on both paths; `_ftol` pops only the step count
    `fnum - f020`, so the loop starts from v = f018 (ours started from fnum - f020). dcp graded it shape with
    "the same behaviour, proved": a load hoisted above a branch that stays on the x87 stack across a call is a
    value of the other path - read where each st(i) comes from. Then `v = v - f + (f / v)` (the quotient first)
    was byte.
  - **A string-terminated list walked by pointer where the original indexes**: ai_AddEquipmentNeeds' final loops
    `for (k = 0; list[k] != 0; k++) { id = list[k]; .. }` (the second loop's `mov eax,[list2]; test; je; mov
    edi,eax` head and the per-row NSlots reload; padded size right, 10 ins -> M8 only), then `make -= amount +
    pending` (VC6 loads the right operand first) byte. **dcp try does not see an M8**: its code hash and hunks
    mask field displacements, so the operand swap printed "= control" (the s17 note said so too); only the
    build's grade showed it. A crossed-field M8 needs a build per variant.
  - Left with notes: ch_GetWaypoints (`mov ebp,ecx; add ebp,edi` for n = i + (t ? 2 : 1), 2 ins),
    txt_ParseRandomText (the stores reload p from its slot, 10 ins), d2_SliderInput (the vertical test's load
    order; its goto is the original's single clamp copy).

- **Deep s28 (the near-byte list to 30 instructions, notes older than s22 first, 15 items: 10 byte + 1 twin)**:
  - **climb.py round 1 first, a third time**: d3sm_ShadowCB (one `(b->gx)` parenthesized in the cross term
    after an earlier session's 1440 orders: fpu 14 -> byte, kept with @original) and d2_ReadBmpPixels (no pad local, `w3 & 3`
    at both uses: draft -> byte) were climb's first-round rows.
  - **SIB base / index order across loops = which loops share the index variable** (rule (5) of the allocation notes above):
    d3anim_CalculateAnimNormals' three bounding-box loops after the frame loop all indexed with i (the frame
    loop keeps f): byte; any other f / i / k assignment of the three, a separate i for one loop, while forms:
    = control. Generate all assignments in one `dcp try` (27 rows) before any spelling of the address.
  - **A statement in both arms instead of a named result** (the s27 lever, again): ai_PokerNpcSetStake's
    `n = bold ? d : d >> 1; stake2 = util_RandomU(n) + stake` as the whole store in each arm (VC6 merges the
    call tails; the argument becomes a temporary in edx): byte. evt_FindLogic's `(&& test) || (|| test)` as two
    ifs each with `return s + n` (n's second reference ranks it above end: esi / edi swap): byte.
  - **A complete-constant ternary**: `(p->sex != 0 ? 0x133d : 0x133a) + pick` for `pick + (sex ? 3 : 0) +
    0x133a` (the original's `add ecx,K; add eax,ecx`); every regrouping of the three-term sum = control
    (ai_ShowFeudNews, ai_ShowPartnerNews, ai_ShowPartingNews byte).
  - **An escaped block local's scope**: cm_ExGebUpgrade's key (address to a callback walk) and radius (out
    parameter) declared together in the block of the loaded object: radius packs with t, key keeps its own slot
    and its store stays after the pushes (slots 14 -> byte); key at function scope reorders the store (order
    20), radius alone in its inner block loses 4 bytes of frame.
  - **Named sub-results for an integer / x87 interleave**: d3_GetHeightAt's neighbour corner heights as yz / yx
    locals before the interpolation (the original computes both byte indices before the x87 work): order 24 ->
    byte; y assigned inside the expression got the order but `fld st(2); fsubp` (3 ins).
  - **A loaded value read into its own local**: hmt_ClearOffices `type = hmt_ElectionObjTypes[m - 1];` before
    the call that takes it (edx -> eax): byte.
  - Left with notes: gm_StockPrice (threaded 0.25 clamp path, the merged mk == 0 tail), cm_RunWedding (guard
    epilogues), hmt_RecordTextId (n copy-propagated into the clamp compare), ai_GuildCashPayWindow (amount /
    cancelBtn slot counts 4 / 3), ai_OpenUpgradeTooltip (view / f against idx's argument slot).

- **Deep s29 (the near-byte list to 35 instructions, notes older than s22 first, 15 items: 9 byte + 1 fix, 3
  bonus)**:
  - **A local declared with an initializer that emits no code (`int k = 0;`, `AICHAR *c = 0`) moves VC6's
    first-reference tie key**: the same mechanism as main_EndRound's `AIBLD *b = 0` (the call order of two
    calls), now seen deciding (a) which of two calls of one sum runs first (cm_TrialAskPlayer: gm_GetWorth of c
    before accused; both operand orders were one code), (b) the reload order of split pieces after a call or at
    a loop latch (ai_DonationWindow win / bld; d3anim_MapBones obj / anim: the s24 note's "one zero piece more,
    the same code"), (c) whether VC6 merges two counters into one induction variable (ai_TutorialWindow, the
    s25 IV merge), (d) the load order of a test (d2_SliderInput draft 10 -> shape 4). All five kept with an
    `@original` comment. Sweep it as a round: one variant per local with `= 0` at its declaration (a scratch
    generator over the declaration lines, one `dcp try` per function: ~1 min); over 200 functions of the list
    it found MapBones / TutorialWindow / SliderInput. Float locals' `= 0` emits a store (worse); ints / pointers
    are = control where the key does not matter.
  - **climb.py round 1 first, a fourth time**: cm_RemovePartyMember (the loop bound r->used in the for, no n
    local), d3anim_CreatePath (p->frameTime = 0 after the flag bytes: proven byte under a declaration set).
  - **A local that only carries a field to its store is the field**: init_InheritOwner (`for (par = obj->parent;
    obj->owner == 0 && par != 0; ..) obj->owner = par->owner;`, no own: regs 22 -> byte); d3t_EarthCB's
    `volatile float a` was the CSE temp of `p->start.x -= ..; if (p->start.x > 0.0) p->a = p->start.x` (fst /
    fld / fld from a slot packed with s: slots 22 -> 0).
  - **Escaped field values as block locals of their sending blocks** (s21 again): gm_PopulationStep `int kind =
    3, level = 2` and `int office` (slots 26 -> byte), ai_DonationWindow `int amount` (frame 0x220 -> 0x21c).
  - **Hand-walked pointer -> index**: he_FindGuestInn `tab[i].inn` (byte); gm_GetDisease the other way: the
    original read the record through `p = &gm_Diseases[n * 3]` with `n >= 10` (cmp 0xa / jl): byte, size 84 ->
    83. amt_ApplyForOffice's do-while pointer walk is a plain for + break (same code, kept for reading).
  - **Clamp then store**: gv_AdjustEntry `e->b0c = (unsigned char)(v >= 0xff ? 0xff : v)` after the low clamp
    (the if form and min/max over the sum were other code).
  - **It was a meaning bug**: cm_TrialAskPlayer's mode 1 stored the yes / no answers swapped (the original's
    `mov [ebx+0x94],edi` with edi = 0 on click 1): a regs hunk whose two registers hold different constants is a
    swapped value, not an allocation.
  - **A buffer size from the frame difference**: he_RunFeast line[0xec] -> line[0x100] (frame 0x14 short).
  - Left with notes: amt_ApplyForOffice (the !f2 tail's lea register / cross-jump), d3t_EarthCB (ps->tex
    before c.a, 2 ins), ai_SolveCannonAim, he_RunFeast (fill load order, txt_Format argument registers),
    ai_RegisterSaveSlot (which compare copy the xjump merger keeps: c2tail shows it deletes the copy before
    the jmp), ai_BlackmailContact (five stores after the push in the original), d2_SliderInput (bottom sum
    load order).

- **Deep s30 (the near-byte list to 40 instructions, the functions without a note first, 15 items: 14 byte + 1
  fix)**:
  - **Read c2trace's priorities when a callee-saved register goes to the wrong candidate; then change ONE
    block's distinct-value count**: ai_BuyBuildingOfferWindow's win#1 stood at -15 against the loop-hoisted
    constant -1 of msg.fromId at -14 (the constant took ebp, win was spilled: frame +4). A block's term is P
    (distinct values in it) times the weight, + for a referencing candidate, - for one living through: the
    offer text printed into its own local before d2_GetWinObj (one value more in win's setup block) gave win
    ebp: byte. gm_KillSpieler's index temp c * 0x21c (-41) outranked the loop-hoisted ch->netId (-56):
    `netId = ch->netId` instead of `ORD_CHARS[c].netId` at the top (one reference of the index fewer in the
    first block) gave ch->netId ebx and spilled the index as the original does (draft 15 -> order 6, then
    the id store as ORD_CHARS[c].id: byte; the netId local used in the culprit compare was worse, 19).
  - **Frame slot ties at equal counts: a store written in two merged arms**: sb_UnloadSampleBank's ret and i
    both counted 3 (c2frame; i reached it first and took the lower slot): the guards nested, `if (sb_Used) {
    if (bank) {..} else ret = -1; } else ret = -1;` (two -1 stores, merged late, count twice): byte.
  - **A pointer temp that only feeds one assignment is the expression**: he_RunIncomeEvent's `rec =
    &ai_CharsT[bld->owner]; if (rec != he_PlayerCh) owner = rec;` as one condition with the address written
    twice (scratch rotation regs 26 -> byte); ai_GraveHauntContact's b carried into msg.arg[0] is a store in
    each arm of one if / else-if chain (regs 18 -> byte, size +1). The reverse at a call: the argument sum
    into its own local (cm_CheckFieldDeltas `id = *p.i + c->partner;`, as its sibling cm_ExFieldDeltas has
    it: eax / edx -> byte) and the bound n / 2 into `half` (sv_ValueBar: the loop counter / bound in ebx /
    edi).
  - **Block locals again (s21)**: main_UpdateFigure's `int one` (the field value whose address goes to
    cm_AddFieldValues) in each branch: the else branch's packs with f (slots 8 -> 0); the else branch names
    the record `c = &ai_CharsT[i]` (edi).
  - **Loop forms by the test after the decrement**: he_FindMeetingSlot's `dec edi; mov esi,edx; test edi,edi;
    jne` is `i--;` as a statement before the slot step with `while (i != 0)` (for (..; i != 0; i--) folds the
    test away); the outer counter decremented at the head of its do loop; the random start as util_RandomU
    (mov si,ax). A product of 50 loop forms in one dcp try found the outer form, the inner needed the
    statement form. ai_DomizilGoodInRange: the guard on the global (`ai_DomizilCount == 0`) where the original
    tests the loaded value, not the n copy.
  - **Parentheses decide a fild's place**: gm_FormatAmount `((strlen(a) - 1) / 3.0f) + strlen(a)` (the
    quotient's fild after the second strlen; casts, double and reciprocal forms = control).
  - **Nested ifs for a fallback (no flag)**: he_AllocFight's place = a's home, else b's home as `if (home)
    place = home->id; else { home = b->bld2; if (home) place = home->id; }` (the flat reassignment + one
    test swapped the place / home registers).
  - **It was a meaning bug**: d3_MirCreateOutline cleared `pts` bytes (the asm clear macro read the pts
    parameter); the original passes pts's own slot as d3_MirBoundaryEdges' count pointer, so that slot holds
    nedge: `dcp frame`'s "shared in the original: arg+0x0 (nedge, pts)" named it. Clearing nedge bytes fixed
    the frame (0x4c -> 0x48).
  - climb round 1 found 4 of the 15 (cm_NearestEscapeObject's bound in the for: byte; the swaps that led
    ai_DomizilGoodInRange / gm_FormatAmount / ai_BuyBuildingOfferWindow); the initializer lever (initgen) was
    = control on all 15. A message fill permuted (cm_SendSpawnCharacter, 5040 orders): byte.
  - Tools: initgen.py now finds the definition (not an indented call of the name: gm_Query), skips float and
    record locals and skips a local union's member list; climb.py reads the function's Latin-1 text from the
    file (dcp fn's output turned a string's 0xfc into U+FFFD and round 1 died: main_UpdateFigure).
  - Left with a note: d3_MirCreateOutline (mark[head]'s load order, regs 2).

- **Deep s32 (the near-byte list to 45 instructions, the 15 untagged ones first: 12 byte, 1 draft -> shape, 2
  notes; 2 bonus)**:
  - **A shared call tail the original reaches by `mov reg,..; jmp` from several arms = the call written in each
    arm, and the statement after it too**: ord_ReportStatistics' tier phrase `txt_Format(tierText, "%s%s",
    ids[k + 2], ids[k + 2] + 1); any = 1;` in each of the three tier arms (VC6 cross-jumps them into one call):
    the three tierText references order the two 0x200 buffers (count tie broken the original's way), the three
    `any = 1` rank any above txt1 / txt2 (esi / edi / ebx / ebp rotation). One call after the arms: slots 16;
    the calls per arm without the flag stores: regs 40; both: byte.
  - **c2trace --prio again (s30): one value more in a block where the losing piece is referenced**:
    ai_BuyCartWindow's item (32) and kind's split piece (32) tied for ebx; the caption's price into `price`
    first (`price = (int)gm_ItemPrice(kind + 0x134, 100); txt_Format(.., price);`) lifted kind: byte. flip.py
    on that hunk hit only constant tokens all over the function: the mark of a priority tie, not of a
    spelling.
  - **A rotated vector, not two scalars**: d3_YawTo's `t = cs * a.x + sn * a.z; u = ..; t * b.x + b.z * u +
    a.y * b.y` (every operand order one code, fpu 25-32) is `c.x = ..; c.y = a.y; c.z = ..; c.x * b.x + c.y *
    b.y + c.z * b.z` with a D3VECTOR4 c: the x87 allocator keeps one field in its slot (`fstp [esp+0x20]`) and
    lifts the other, as the original: byte. When the original stores one of two float temporaries and keeps the
    other on the stack, try them as fields of one vector.
  - **Parentheses are a float lever**: gm_AgeCharacter `v = v - k + (k / v)` (fld k; fdiv; fxch; fsub; faddp)
    against `v - k + k / v` (draft 7). VC6 orders a flat chain of float adds itself (gm_RankPlayers: all 6
    orders of `title + office + reputation` one code), a parenthesized group is kept:
    `(reputation + (title + office))` gave the original's fld title; fadd office; fadd reputation.
  - **Block locals (s21 / s29 again)**: ai_DecideGraveyardOwner0's x (address to ai_PickActionBuilding) and
    its class mask in the no-building branch: at function scope x conflicted with the pick temp and id, which
    then took other dead parameter slots (slots 30 -> byte); `flags = 0` before the o1 stores kept flags out
    of o1's dead slot.
  - **Decompiler loop and guard shapes -> plain code, byte at once**: two guard blocks with the same free +
    return as one `||` condition (he_RunEventGood: ev / per swapped); `cnt = 0` after the guard instead of an
    initializer (cm_LobbyAllReady: the pushes and xor edi behind the early return); a building walk with the
    match returning 0 inside a `for` and `b++` at the body's end (gm_Homeless); a pointer q stepped beside i
    -> `lst[i]` (gv_BuryCharacter), `o = l[i]` beside i -> `l[i]` (gm_FindMarketObject); a hand-advanced
    search `u = *++t` -> index loop + break; the arms of `if (index == -1)` swapped (ai_SwitchRoom); a goods
    record pointer g with `(unsigned char)g->type` in the search (gm_TakeableCount: the original materializes
    the record address and keeps the type in al; the unsigned char local spilled it).
  - **A literal in a callee-saved register (s27): the store in each closing arm** (ai_SuicideWindow's
    sv_PopRequest = 1 three times in an if / else-if chain instead of one store behind `continue`s).
  - Left with notes: gm_RankPlayers (the prologue's zero register for best: edx = n's in the original, edi =
    i's in ours; const#0 piece), hst_NextEvent (the d != 1 arm laid behind the loop's return in the original;
    the tail in both arms moves it there but VC6 then decides the r == 0 test on that path), he_PatrolStep
    (the message 0xf's n / ch stores after the push of &m; 4000+ orders and the block-local m tried),
    hmt_FillCouncils (bonus: the council loop's i = 2 .. 0 with the test on i + 1).

- **Deep s33 (the near-byte list to 50 instructions, notes older than s22 first, 15 items: 8 byte (one after a fix), 7
  notes; 1 bonus)**:
  - **A parameter VC6 keeps in a register where the original reads its slot each time: one of its reads was
    another variable**: amt_Rating's `if (mode == 2)` was the decision record's `rec.dec.cur == 2` (the original's
    `mov eax,[esp+0x54]` read before `add esp,0x10` lands 0x18 into the AIDEC); with mode at three reads the
    s2x hit-inside form was byte at once. Convert a pre-`add esp` displacement before naming the variable.
  - **The strength-reduced loop pointer is based on the most-referenced field; a test VC6 folds still counts**:
    ai_ConvertWindow's player loop pivots on c->kind (ai_Chars+2) in the original, on c->faith in ours (one
    reference each; the AI loop's three kind tests pivot on kind). `c->kind < 0xa && c->kind < 0xa` (or `!=
    0xa`, `<= 0xa`) folds to one compare after the IV choice: byte. `kind >= 0` (unsigned) folds too early.
  - **The s29 initializer lever decides which call of a sum runs first, in an inline sum**: ai_HatredWindow's
    `t = gm_GetWorth(b); price = (t + gm_GetWorth(a)) ..` adds into eax (ours) where the original adds into
    the first result's register (add edi,eax): every t spelling is one code. The inline `gm_GetWorth(b->id) +
    gm_GetWorth(a->id)` gives add edi,eax but calls a first (both operand orders one code); one `= 0` on any of
    12 locals (initgen) flips the call order: byte. A frame hunk first: the AI branch's price was its own block
    local (dcp frame: another slot than the player path's).
  - **Parameter types decide register ties**: d2_DrawRectOutline's r / g (unsigned int, tie at 112, served g
    first) are `unsigned char` like d2_PutPixelRGB's parameters: byte (prototype fixed in both headers; the
    caller pushes the same constants). Folded self-references (`g = g;`, `(void)g`) never reach the tie key.
  - **A frame tie broken by a macro's temp**: ai_TokenRandomRich's n1, pred and found tied at count 4 in the
    frame sort (c2frame); AI_ZERO_BUF's pointer temp as `v` instead of n1 (n1 at 2): byte, and the same for
    ai_TokenRichestInhabitant (bonus, 24 ins).
  - **A loop invariant from a start time**: `_start = d3_Time; while (_start + 0xfa > d3_Time)` gives `mov
    eax,[d3_Time]; lea ebx,[eax+0xfa]` (the sum hoisted beside the start value); `_end = d3_Time + 0xfa` loads
    straight into ebx (ai_MissionSuccessDialog, G13_WAIT).
  - **A chain split into a clamp and its own if / else-if puts both short blocks after the first arm**:
    ai_CourtshipWindow `if (gain > 0) rounds = min(gain, 3); if (gain <= 0) r = .. else if (gain == 1) ..`
    (VC6 threads the jle to the gain <= 0 roll; the 3-store is no longer if-converted), not the nested form.
  - **A dead store written in each arm**: sv_ScrollCamera's `dst[0] = v[0]` beside dst[1] / dst[2] in both arms
    (one copy after them rotates the scratch registers), with the plain `dst[1] = d3_Camera->pos.y`.
  - Left with notes: init_SetDetailLevel (oldlvl must outrank size: free-register order esi, edi, ebp, ebx),
    ord_WeightedRecordSum (i and the IV tie at 64, tie key i = IV + 1), d2_DrawButton / d2_BlitPartMode (the
    original's operand order of a sum against VC6's reassociation rank: every spelling one code),
    gm_CharacterWages (the add's destination, as HatredWindow's but no initializer moves it),
    he_AllocInsult (b->id's CSE temp holds eax over the 0x1d81 arguments), cm_JoinSplit (p->next7 = 0 before
    the part's memcpy fixes the order; m / next7 tie left).
  - Tools: vdiff / c2trace / c2frame / c2tail / c2fpu (c2trace.parse_variants) and climb --base apply `<<<N` /
    `<<<*` blocks; initgen.py takes every declaration run (later function-scope runs, inner blocks; `<<<N` for
    a repeated line) and a definition with a comment before its `{`.

- **Deep s34 (the near-byte list to 60 instructions, notes older than s22 first: the 15 at 17-20 instructions,
  8 byte (one after a fix), 7 notes; 1 bonus)**:
  - **Read the frame offsets of a struct copy before calling a slot swap allocation**: he_AimReaper's "target /
    at slot swap" (noted since s21) was reaperPos copied from the wrong vector: with the pushes outstanding
    counted (0x28 at the copy), the original's `mov eax,[esp+0x34]` is frame 0xc, the reaper's own
    d3_PointToWorld result, not the target. With the right source the reference counts reorder the two 16-byte
    slots by themselves: fix + byte. A slot-order hunk between two equal-size records whose counts differ a lot
    (9 against 4) is a meaning question first.
  - **A global read again instead of the local copy**: cm_EnqueueReceived's free-list pop `cm_RecvFree =
    cm_RecvFree->next` (not m->next): VC6 then keeps &m->next in ebp for the two later m->next = 0 stores
    (draft 18 -> byte); cm_ExecCommands' closed-connection loop `while (cm_RecvQueue != 0) { m = cm_RecvQueue;
    ..}` (the head becomes a global candidate in esi across cm_FreeReceived, ebx / esi pushed before the
    closed test: draft 18 -> byte). The s23 flip lever from the other side: the original reads the global
    where ours reuses a value.
  - **A global walked in a loop -> a local copy inside the if**: he_FreeEvent's top shrink `t = he_EventTop;
    for (; t >= 0 && he_Jobs[t].kind == 0; t--); he_EventTop = t < 0 ? 0 : t;` (as its quick-list twin above
    it): the original's recomputed &he_Events[top] after the jl guard (s6 / s9 had tried every spelling of the
    compare and the loop on the global: all CSE'd): draft 18 -> byte, +16 bytes.
  - **Exclude the failure first, then if / else** (an out-of-line block after the epilogue): gm_CreateCharModel
    `it = 0; if (dummy) it = find; if (it == 0 && src == 0 && o == 0) goto done; if (it) world; else copy;`
    puts d3_PointToWorld behind the epilogue with a jmp back to the name record (s15's nine if-forms were one
    code; a flag with a second `if (it)` got the test order right first: 16 ins, then the excluded-case form).
  - **One store per arm for an `||` flag, split where the original's tests split**: he_AllocSpying `if (me->kind
    == 6 || victim kinds) spawn = 1; else if (spies < 8) spawn = 1;` (spawn's count rises above spies' and
    victim's, the frame order follows; the four-arm chain and separate ifs were worse), then `int spawn = 0,
    spies = 0` in that order (the two init stores): draft 17 -> byte.
  - **An unused parameter reused as a local keeps its own slot**: ai_CommandWealthTax's percentage / worth as
    locals n / w instead of the parameter b: they pack into fmt's dead slot and f gets the original's own
    4-byte slot (push ecx frame): shape 19 -> byte.
  - **Block locals per branch, again (s21)**: ai_DecideUseReputationObject's flags / id in each of the i == 9 /
    i == 10 branches (each pair packs with its branch's fild temp: 15 subsets in one round, only all four
    right), then the good-8 tail `if (r != 0) { copies } return r;` (the je joins behind xor eax,eax): byte.
  - **if / else-if / else for a fallback chain**: cm_CheckSetField's base = b, else o, else c (ternary the same
    code) instead of reassigning base under nested ifs: the eax / ecx pair and size 134 -> 133 (bonus).
  - Left with notes: init_SoundObjectHook (loop 17 against s 16: the Play block's const#0 decides it),
    d2_DrawPic (x / y tie at 51, tie keys 5b / 5c), ai_PlanRank (q 18 against n 16 / r 17; a float copy of
    q->score fixes the registers but not the x87 code), d3anim_CreateMorphAnim (climb's d.x copy: two of the
    three reload edges), cm_OrderClick (the scene load first in the pick block: one byte), d2_MakeLightTable
    (i needs +7 against the 0xff - add temp), ai_FillLoversTab (n in the call block gives the registers but
    moves the store before the test). Parenthesized float rows on d3_GetBoxVerts (bonus, untagged): every
    paren form worse than the flat control.
  - The he_AimReaper fix's commit message names he_UpdateReaper as the function that fills reaperPos from
    obj->pos; it is he_CreateReaper.

- **Deep s35 (the near-byte list to 60 instructions, notes older than s22 first: the 15 at 20-22 instructions,
  6 byte, 1 closer, 8 notes)**:
  - **Read the frame with the pushes counted; a hole in the original's frame is an array's tail**:
    ai_MoneyExchangeWindow's itoa text sat at [esp+0x28] with exchangeButton at 0x30 (8 bytes), and the button
    array at 0x38 ran to playerRows at 0x58 (the original's unreferenced 0x18 hole): `char text[8]`, `int
    buttons[8]` (as ai_CreditGiverWindow's buttons[8]): slots 22 -> byte. d3s_UpdateObjectAt's 12-byte hole
    beside one stored float was a D3VECTOR4 whose x / y stay on the FPU (the projected point p; only p.z
    stored), and the camera-relative point reused the ray vector d (its x stored in d's own slot): frame 0x34
    -> 0x44, byte. When one float of three is stored, look for the vector it belongs to (s32's lever).
  - **A local that only names a field hides the field's references from the induction-variable choice**:
    ai_SortExchangeRows' `v = rows[j].good; if (v == own) .. else if (v == 0) .. else names[v]` -> the field
    read at each test (the same CSE'd code): the inner pointer pivots on rows[j].good as the original's
    (draft 21 -> byte). he_RunSendHomeEvent's pivot (netId, ours model) did not move by any reference probe.
  - **Guard + one final return through a result variable for partially shrink-wrapped pushes**: he_ShowNews
    pushes esi before the kind guard and ebx / ebp / edi after it: `if (!ok) return -1;` then both sends
    assigned to ret and one `return ret;` (VC6 still emits the two call copies); two `return send(..)`: the
    old code, the nested form: worse (21 -> byte).
  - **permute.py on a message fill, then the fill's neighbour**: ai_PickpocketGangWindow type, to, fromId,
    time, arg[10] (= msg.to), flags (719 orders: 22 -> 2, the AITIME copy's load order), then the ids memset
    after `d2_ClickedId = -1` instead of before it: byte. The earlier note's 48 orders had not covered it.
  - **A hand string copy loop is strcpy**: ai_ChangeProfessionWindow's caption copy (inc ecx before the store
    in the original): 21 -> 19.
  - Left with notes: he_RunSendHomeEvent (IV pivot), he_RunStammtisch (m->state-- in edi, a candidate; the
    shared tail's add esp merge), init_Betrieb (i 25 against sum 22, c2trace --prio; the add / lea split),
    ai_ChangeProfessionWindow (the 0x15bd ternary through esi), ai_ProductionGoodsWindow (column pointer
    against gm_KindObjs pointer for ebp), surf_ScaleBlit (pal8 26 against the src->dwWidth CSE 22),
    cm_FightMouse (stateFlags loaded once before the branch with 2 in bl), gm_TitlePeriodServed (hours'
    ftol before the zero fills; `h = hours;` as a statement gives the order but fstp / fld), d3_GetBoxVerts
    (flat float sums: every term / operand order one code, each paren set another code: node identity),
    d3_SetFog (no new lever).
  - Tools: c2trace.py function_span and initgen.py read K&R definitions (d2_DrawPic).

- **Deep s36 (the near-byte list to 80 instructions, notes older than s22 first: the 15 at 22-25 instructions,
  9 byte, 2 closer, 4 notes; hst_NextEvent closer as a bonus)**:
  - **The old note's mechanism can be the wrong one: re-run the plain form with the inits reordered**:
    d2_RaiseWindow's note blamed the 0x2ff constant shared by its two search loops (78 ins for the plain
    form); the cause was `D2WIN *w = &d2_Wins[win];` in the declaration. Assigned after `first = -1; j = -1;`
    the plain for loops are byte. A pointer initialised in its declaration is an init statement at the top:
    put it in the init-order round.
  - **A partial initialiser's implicit zeros and explicit zeros are different stores**: ai_UpdateProdGoodsList's
    `rows[9][4] = { {2}, .. }`: the original's second column written out (`{ 2, 0 }`) for the five rows whose
    second value is 0: those zeros join the other table's explicit zeros (ebx after the push), byte.
  - **A dummy out-parameter declared per block shares one slot sorted last**: ai_PokerNpcFold's `&any` of three
    switch cases at [esp+0x2c] behind every other 4-byte local (one reference each): `int any;` in each case
    block (slots 24 -> byte).
  - **`x != 0 ? 1 : -1` against `(x != 0) * 2 - 1`**: both setne / lea, the multiply form one scratch register
    more (he_RunEffectEvent byte; if / else = the ternary).
  - **A stored field read back instead of the local reused**: lobby_ShowChat's `contentHeight = height + 0x18;
    scrollY = contentHeight - h` (lea ecx,[ebx+0x18] / sub ecx,esi) instead of `height += 0x18` (byte).
  - **Message data through m->data at each use, the loop counter set before the enclosing if**:
    cm_ExWalkPath (m kept in esi, xor edi,edi ahead of the path test, `for (; i < 2 && idx < len; i++, idx++)`).
  - **A check that only one arm needs, written after the if-chain with a decided test**: hst_NextEventCmd /
    hst_NextEvent `if (r == 1 && *out == 0) r = 6;` behind the d chain: VC6 decides r == 1 on every path, but
    the block now sits behind the d != 1 block as in the original (24 -> 18, 16 -> 10).
  - **Alloc handlers: the lookups nested with one he_FreeEvent at the end** (he_AllocTrainShare,
    he_AllocGraveCare: `if (c != 0) { .. if (..) { ..; return; } } he_FreeEvent(ev);`, the original's single
    shared exit; with a float test `!(x <= 0.0f)` for test ah,0x41 / jnp, as he_AllocResearchRecipe). The
    reverse in a switch handler: the free written in the arm (`he_FreeEvent; return;` instead of `break` to
    the shared free) puts the cross-jumped call tail into the else arm (he_RunStoreExtension byte).
  - **`for (k = 0; k < 5; k++, y += 0xd)`** for inc ebx before add esi,0xd (ai_GraveDetail 22 -> 20).
  - Left with notes: ai_GraveDetail (const 0 web in ebp), d3path_Init (sib order: a t local for the flags byte
    fixes it but reads flags into a register), d3fl_TileBox (a scheduling-region cut), d3_SetupFrustum,
    ai_HasMaterials (found known 0 at our loop top), hst_NextEventCmd (return block order, name / cmd).

- **Deep s37 (the near-byte list to 80 instructions, notes older than s22 first: the 15 at 26-28 instructions,
  15 byte; bonus he_TransportStadtStadt, ls_ReadEventTables, ls_WriteEventTables byte, ai_UseGoodLowerOfficialRelations
  72 -> 32, ai_ExpelWorkerWindow 130 -> 31)**:
  - **An inline asm block adds its 2 references to the escaped locals IN SCOPE, in an order that follows their
    NAMES**: ch_PathToTarget's x1 / z1 / x2 / z2 (frame ties, slots 50) were byte as cx / cz / gx / gz (a pure
    rename; `c2frame --refs` R rows show the order the asm adds them in; declaration order is no lever, the
    names are). he_TransportStadtStadt: the vectors and cell locals declared in the blocks that use them get no
    asm references at all (slots 54 -> byte). Rename or block-scope before any count work when the function
    has `__asm`.
  - **The strength-reduced record pointer's bias (`add edi,0x14` against `0x18`, every lea displacement off by
    4)**: with `b->goods[j].f` written at each use VC6 takes the SECOND address expression as the base; a block
    pointer `GMSTOCK *s = &b->goods[j];` gives the third (ls_WritePlayerSlots / ls_ReadPlayerSlots byte). Records
    of a GLOBAL array: plain index loops `gm_CarryRecs[i].f` instead of a hand-advanced pointer (bias 0,
    ls_ReadEventTables byte); a record pointer taken AFTER the skip test written on the array (`if
    (he_Jobs[i].kind == 0) continue; job = &he_Jobs[i];`, ls_WriteEventTables 150 -> byte). Probe: delete or swap
    the first writes and watch which field gets displacement 0.
  - **SIB order `[edx+esi]` against `[esi+edx]` of a `rec[K + j]` search loop**: the FIRST loop of each record
    pointer VARIABLE has the operands the other way round; the original had one such loop, so the later loops
    reused the same pointer (`p = ai_BTypes + list[i] * 0x24d` in the list loop, no `q`): gm_MayTakeJob byte.
    Probe: delete loop sites and see which become the odd one.
  - **A nested call's argument as a named local** (`job = gm_BuildingJob(*p); v = gm_TitleJob(c, job);`) swapped
    the two parameters' callee-saved registers (gm_MayTakeJob 28 -> 2).
  - **A value 'spilled' at one point and reloaded into another register at the tail, two zero registers at the
    top** = two variables, not a split piece: gm_BuildingBalance's wage `sum` of the owner block and `cost` (0 at
    the top, `cost = sum` behind the block), `sum = 0` before `cost = 0; income = 0`. The probe that showed the
    middle was right: make the variable dead behind the block (delete the tail uses in a variant).
  - **The emitted store order of a record fill is not the source order when loads from escaped vectors feed
    it**: amt_CalcZuenfte's build message was the plain order (type, to, fromId, time, flags, arg[0..2],
    buildPos[0..2], buildRot[0..2]); the scheduler interleaves it into x, arg0, z, y, .. Found with the
    interleavings of the three ordered triples (1680 variants) after the header's 5! round. The slot swap
    there: of two vectors the one that shares a bigger record's slot is the D3VECTOR4 (16 bytes sort behind
    12, the newest slot is the one the record joins).
  - **Inits at the top**: (1) a 0.0f store takes the register of the latest REGISTER variable set to 0 before
    it in source order (none yet: an immediate); an int 0 into memory takes the const-0 candidate. A fourth
    word of a vector cleared from the int zero register is an int member; the local struct is then no longer
    split into float symbols and the float expression's term order flips back to the plain one
    (ob_HandleBepflanzung). (2) the users of one scratch register keep their source order: `mov eax,[bld]`
    ahead of `or eax,-1` ahead of `mov eax,[d3_Terrain]; cmp` = the pointer load's statement stood before the
    -1 assignments although their stores are emitted first.
  - **`dec esi; test esi,esi; jne` (the dec not fused) and the counter / id registers swapped**: the wrap step
    in the for's increment clause behind n--, `for (n = 0x300; n != 0; n--, id = (id + k) % 0x300)`
    (ai_UseGoodCharmOtherSex byte; `while (n != 0) { ..; n--; id = ..; }` is the same code, do / while moves
    the found arms out of line). The old note blamed a unit boundary for the tail; it followed from the loops.
    `grep_orig.py --multi 'idiv ecx
[^
]*dec e(si|di)
[^
]*test' --grade byte` found the byte siblings.
  - **flip.py names the place, the ordinary form follows**: cm_CheckFight's duplicated guard line (a decided
    nested test: one more reference of ca / cb) -> `if (bad) { if (ca == cb) return 0; return 0; }`, kept with
    an @original comment (no plainer form found: 14 guard spellings = control); cm_DuelOutcome's tok 0 -> 1 at
    `(w->sex != 0) + K` -> `if (w->sex != 0) txt = K + 1; else txt = K;` into a local (the setne's byte temp
    in dl instead of the free bl).
  - Small ones: `c->x = v >= 0xff ? 0xff : v;` as one ternary store and `> 0` on the unsigned byte (jbe)
    (cm_ExHeldRounds); the switch's cases in the image's block order (gm_FirstTitleNumber, cm_OrderRating's
    case 3 last); `if (x == 0) f = 1.0f; else f = 0.5f;` so f's first store stands behind the parameter's
    load and f can take the parameter's slot (cm_OrderRating); a saved global read in the block that needs it
    (he_FreeGfxInfo: push ebx behind the je); `rec->heads[h]` instead of a walked out pointer
    (gm_ResolveHeadIcons); the x clamp before the z clamp.

- **Deep s38 (the near-byte list to 80 instructions, notes older than s22 first: the 15 at 28-32 instructions,
  7 byte, 3 closer, 5 notes)**:
  - **Two records of equal count swapped in the frame, an AI_ZERO asm block in scope**: declare the pair in each
    block that uses it (ai_DecideApplyForOffice's AIARG s1 / s2 in the rank block and in the leisure block:
    four block locals, byte). Renaming them changed nothing there (the asm names only _zp / _zn).
  - **A small local sharing another's slot in the original, three slots of equal density in a frame > 0x80**: an
    escaped local (its address went to a call) conflicts with every local whose life lies inside its scope
    before the scope's last call; the original's scope ended earlier. ai_DecideBeatUp: cid in a block around
    the partner pick that closes before gm_Mix, so g joins cid's slot (c2frame X rows); the three 4-byte slots
    then all weigh 1500 and the unstable sort gives the original's order (byte, kept with an @original).
  - **A local converted in place instead of a second variable**: he_RunGang's case 1 `sum =
    gm_PlayerFactor(sum, ..)` (no v there): sum shares member's slot as in the original; with `idx = 0;` before
    `member = 0;` byte. A pointer-walked id loop as a plain index loop gave the same code (kept, plainer).
  - **The first operand of an == between two record fields is loaded first**: `j->ownerCh == e->ownerCh`
    (he_AllocEvent32: every scratch register after it rotated; byte).
  - **add esp of two consecutive calls merged in the original, ours split at a join**: write the second call at
    the end of each arm (he_CreateGfxInfo's d3if_SelectScene(scene, 1) after the sprintf and after
    he_CreateGfxObject: the arms' add esp merge, the tails cross-jump; then a 4-store permutation, byte;
    he_EndPersonEvents' 0x45 loop: he_EndEventNow in each arm, as its 0x41 loop).
  - **A shared tail kept at the last site in the original, the first in ours**: a `return;` at the end of the
    later arm (ai_TutorialMarkerUpdate phase 4; separate ifs instead of the else-if chain give the same code).
  - **goto found out of two search loops**: plain `for (rec = first; rec != 0; rec = next)` loops, each match
    showing the message and returning, the fill after the loops (ai_KidnapStart byte; the fill's position in
    the source rotates the scratch registers of the loop above it). A raw int request array was AMOUNTCFG.
  - **A loop counter in a dead parameter's slot**: the frame list tie (equal counts) goes to the local that
    reaches its count first in the walk; `for (i = 0, y = 0xc; y < K; y += 0x30, i++)` lets y beat the
    strength-reduced cursor of `sliders[i]` and join win's dead slot (ai_ShowSettingGoods 32 -> 6; i++ first
    loses it).
  - **A literal pushed as an immediate although a variable holding the same value is in a register**: the
    variable was set in each arm before the join, not in the common block (VC6 propagates `ret = 1` within the
    block to the literal 1): ai_UseGoodRaiseRelation 31 -> 24.
  - Left with notes: ch_InitWalk (the frame wants t and y in one 12-byte local, which then orders a pointer load
    after its stores), ai_BookmarkWindow (const 0 / 1 webs decide i / o), ai_PickBuildSite (an empty slot and
    the sqrt spill's own slot), ai_ShowWorkSettingsPage and gm_FreeStallCapacity (one reference decides two
    callee-saved registers), he_EndPersonEvents (the constant 3 of seven byte tests gets bl; no spelling of
    the test changes it).

- **Deep s39 (the near-byte list to 80 instructions, notes older than s22 first: the 6 old-note ones at 28-31 and
  the 9 at 32-36 instructions, 10 byte + 1 proven byte under a declaration set, 4 notes; bonus
  ai_MeisterTradeFernEinkauf2, ai_UpgradeBuilding byte)**:
  - **`mul 0xaaaaaaab` / `shr edx,4` / `lea [esi+eax*8]` around a field update that goes through a callee-saved
    register, an `inc` behind the second quotient** = two counting loops VC6 replaced by their trip counts:
    `while (h >= 24) { h -= 24; c->year++; } while (h < 0) { h += 24; c->year--; }` (gm_AddTime byte; the
    written-out `d = (unsigned)h / 24; c->year += d; h -= d * 24;` has the same instructions with the update
    in eax and one register fewer). A division that looks hand-optimised next to a read-modify-write temp in
    edi / esi is a loop first.
  - **A parenthesis in a float product is a node in the scheduler's region** (c2dag: `op=263 r32 -> r32` behind
    each parenthesised product), and a region ends after 81 nodes: a macro nested to force the multiply order
    (`(((((s * a) * b) * c) * v) * v) * 0.5f`) made ai_StepShot's last region six nodes longer, so the four
    hoisted argument pushes of the following call fell into the next region (28 ins, "the pushes sink behind
    the float code"). Count the original's region: the 81st instruction behind the last label / jump (fxch
    and the pops count) is where its hoisting stops. Probe: delete N instructions' worth of statements in a
    variant and watch N more instructions hoist.
  - **VC6 orders the operands of an unparenthesised float product itself**: by class (the ?: temp, a pointer
    field, globals, locals, the constant), and two globals by which node it MET FIRST in the function, the
    later one first. So the function's first product decides every later one: ai_StepShot's wind x drag as
    `shot->area * v * v * SIGN(v) * ai_AirDensity * ai_ShotDrag * 0.5f` gives fmul drag / fmul density there and
    in all six flight drags (all 2520 operand orders of the flight macro were one code; with the nested wind
    macro naming the drag first the flight products came out density / drag, 12 crossed M8). A product that
    must keep `fmul [m]` twice instead of squaring a loaded copy keeps two parentheses (`((k) * v) * v`, the
    wind y drag, @original).
  - **The x87 allocator takes its candidates block by block in definition order**: a float copied by integer
    moves (`next.x = pos.x`) stays in memory only if a stack candidate defined before it in the same block
    does not nest with it; placed right behind the block's first statement it went on the stack (frame -4,
    175 ins), behind `b.x = d.x + wind.x` it is the original's (ai_StepShot: next copied in the last block; the
    integer statements of that block keep their source order when the scheduler hoists them).
  - **Two registers holding the same constant for two arrays** (`or edx,-1` for objs1, `or eax,-1` for objs2;
    `xor edx,edx` for exp[] beside `xor ecx,ecx` for nB / i): the arrays were filled by a loop or memset each
    (`for (i = 0; i < 3; i++) objs1[i] = -1;` twice = `memset(.., 0xff, ..)` twice; `memset(exp, 0, sizeof(exp))`),
    not by element stores, which share one register with every other use of the constant (rt_EditPointDialog
    35 -> 6, ai_MeisterTradeFernEinkauf 35 -> 17, its twin 62 -> 16).
  - **A long message fill: write the repeated value out at each store and permute all seven**: the chained
    `msg.arg[0] = msg.fromId = msg.to = X` bottomed at 18 ins in 600 orders; `msg.to = X; .. msg.fromId = X;
    msg.arg[0] = X;` as three statements among type / flags / time / arg[1] had one byte order in 5040
    (ai_CourtshipConfirm: type, flags, to, time, arg[1], fromId, arg[0]). The 14-byte `msg.time = ai_Time` is
    scheduled in pieces around the other stores, so its place in the source is not readable from the image.
  - **A scratch register one step further in the second arm of an if / else whose arms end alike** (`lea eax`
    where ours has `lea ecx`, every later temp shifted, `mov ebx,eax` sunk behind the push): the code BEHIND
    the if / else stood in both arms. The first copy's temporaries advance the rotation, then VC6 cross-jumps
    the copy away (the first arm ends `test eax,eax / jmp` into the second arm's `je`): ai_UpgradeBuilding's
    stock check + message in both treasury arms, 49 -> byte, kept with an @original. The 30 repeated stores an
    earlier pass had left in that fill bought nothing (the plain six: the same 49).
  - **The frame's slot table as a hint for one object**: two locals of ours swapped at equal density, the
    original's pair adjacent (rect[4] at +0x38, scene at +0x48) = ONE 20-byte local (`struct { int rect[4]; int
    scene; } saved`); with it the K&R quicksort also resolved the unrelated n / dt tie (ai_CannonScene byte;
    simulate the table with the 12-line sort before permuting statements). A slot weight tie made by a temp
    (`tmp@strcat` joining `taken`): give the escaped local whose scope blocks the better partner a block scope
    (gm_InheritEstate: `int kind` in the heir block, the temp joins kind, taken sorts last).
  - **A byte temp that needs the fourth byte register takes ebx away from a zero held across calls**: the
    original materialised the record address (`lea eax,[edx+eax*4]`, the byte through edx) and kept ebx = 0
    for nine compares and stores; a type pointer read at each use (`bt = &ai_BTypesT[it->type]; if (bt->level >
    best) { best = bt->level; ..`) instead of an `(unsigned char)table[i * size + off]` value local
    (amt_ExecutePriceMessage 28 -> byte; flip.py --want 'cmp esi, ebx' named the lvl statement).
  - Known levers that matched again: a goods pointer tested at each use for `add eax,ebp / mov al,[eax]`
    (ai_RoomUpgradeWindow, both type loops); the row's field written out at each use for the strength-reduced
    base on that field (`rows[i].good` instead of a `good` local: lea [esp+0x138] against 0x13c); a block
    pointer `sl = &ai_Slots[i]` for the base on the third address expression (ai_MeisterTradeFernEinkauf, the
    M8 gone); `min(amt, buyAmount)` in both calls instead of a clamped local (the twin); u^3 / t^3 as float
    locals kept on the FPU stack over two statements (ai_TutorialBezier).
  - Left with notes: d2_BankConvertTo16 (size needs +5 priority against the strength-reduced pointer: one
    more distinct value in the first block; the second arm's scratch rotation), ai_QuarrelCallback (the zero
    of the kind-6 arm lives in esi from the first block: a ret-like variable whose return only merges with
    ret = 1 behind the body; the form with every instruction right has the else arm out of line),
    dd_SetMode (no plain int expression keeps R * 100 out of VC6's multiply-add merge: 1000 toy forms),
    ai_RentGraveDetail (e / n priorities 76 / 74, flipped by any const-0 reference more or fewer).

- **Deep s40 (the near-byte list to 80 instructions, the 15 untagged ones at 36-44 instructions: 13 byte, 2 at 41 ->
  2 with notes)**:
  - **Two callee-saved registers swapped between a pointer parameter and a value local**: count the exit
    statements. he_RunGypsyCurse wrote `he_FreeEvent(ev); return;` for a == 0 AND at the end (ev 33 against n 32 in
    `c2trace --prio`: the extra free block is +2 for ev); nesting the money part under `if (a != 0) { .. }` with one
    free at the bottom took ev below n (byte). The same in he_RunTrialSteps' case 1, the other way round: the
    original had the early `if (teacher == 0) { end; send; return; }` and the same end written again after the
    news; VC6 cross-jumps the second copy into case 0's (byte, size +2).
  - **A local copied from a record field just stored** (`clk.year = c->w028; year = clk.year;` instead of `year =
    c->w028; clk.year = year;`, same code for the stores): the parameter c then outranks year (gm_DeathDate esi /
    edi swap, byte). The two-reads and chained forms are the old code.
  - **A test chain whose second arm comes first in the image** (`test eax,eax; je ->arm0; cmp eax,ecx; jne; arm1`,
    ecx = 1 held for the stores): `if (id != 0) { if (id == 1) {..} } else {..}` (ai_MessageBox; the switch, the
    swapped else-if and two separate ifs are other codes).
  - **A 4-byte char array sorted as if denser**: `char voice[3]` (the frame sort divides by the declared size: 4 refs
    * 1000 / 3 = 1333 beats the 1250 slots; slot still 4 aligned): cm_RunAuction slots 48 -> 0. Then a store of a
    value loaded for a compare: `req.price = req.bids[best];` (not `= top`) keeps the loaded ecx (byte).
  - **`mov ebx, K` held across a loop for an `__int64` argument, `xor eax,eax; push eax; push ebx`**: the mask was a
    `union { struct { unsigned lo, hi; } s; __int64 q; }` local (he_b.c's kinds): `kinds.s.lo = K; kinds.s.hi = 0;
    ..(kinds.q)` keeps lo in a register and builds hi per call. An `unsigned` or `__int64` local is folded into
    `push 0; push K` (he_RunCharPayouts byte).
  - **One callee-saved register for a saved value and a later index, `mov edi,eax` after a lea from eax**: the
    store went through the global directly (`d2_Wins[d2_LastWindow].scrollY = 0;`) and the index local was taken
    after it (`win = d2_LastWindow;`): ai_AmountPageSwitch byte.
  - **`test ah,5; jnp` to a shared failure block with the second skill test inline**: a flag set by the two tests
    (`ok = 1; if (A < 0.8) ok = 0; else { ..; if (B < 0.7) ok = 0; } if (ok == 0) {fail} else {success}`):
    he_SecretRoomDomizil byte; `||` with a comma expression and the nested ifs with the failure written twice are
    other layouts.
  - **add esp of two calls merged in each arm** (s38's lever again): cm_SessionAskPlayer / cm_SessionAskAI wrote
    `r->session.answer = res; sprintf(r->text, ..)` once behind the if / else; in each arm it is byte.
  - **min / max as ternaries, not as if-clamps** (cm_FindNearCell's form): `hi = sc->size - 1 < cy + k ?
    sc->size - 1 : cy + k;` instead of `hi = sc->size - 1; if (hi >= cy + k) hi = cy + k;` (the reload of hi
    from its slot at the join vanished): cm_BestCellNear / cm_MostThreatenedCellNear 41 -> 2 and their sizes
    right. A 0.0f stored from the zero register of the loop counter: `k = 0;` before `best = 0.0f;`.
  - **y as an expression of i, not a second loop variable**: ai_Top5Window's `for (i = 0, y = 0x34; y < .. ; i++,
    y += 0x4c)` gave y the third register; `0x34 + i * 0x4c` written at each use (VC6 strength-reduces it into
    the same IV, tests it against 0x1b0 and drops i) is byte. A y local assigned in the body leaves the two IV
    inits of the preheader in the wrong order.
  - **A local in a dead parameter slot of the original**: the frame packer gives the slot to the first local in
    list order whose lifetime is compatible. gm_StaffFactor's fit took b's slot in ours; in the original fit = 0
    stood BEFORE the load of b (`t = GM_BTYPE(bld->type);` after the inits), so the f group got it (permute.py of
    the 6 inits). Then the dead pop before the final product: the sum into f as its own statement (`f = opinion +
    f + ..; return (fit + health) * f;`), byte.
  - **An expression temp in a callee-saved register across a call, the named local got another one**: the amount
    written inline into the call (he_DuelDeclined's `-9 - (int)(..) - (unsigned short)util_Random(8)`); then
    `if (q->sex != 0) txt = 0x1d69; else txt = 0x1d68;` for `(q->sex != 0) + 0x1d68` (cm_DuelOutcome's lever
    again: the setne's byte temp in al), byte.
  - Left with notes: cm_BestCellNear / cm_MostThreatenedCellNear (two reloads of split pieces at the k loop's
    latch, cy before the cy - k IV in the original; flip.py names only the float 0.0f constants of the
    MostThreatened copy).
  - Tool: the original's slot of each of our locals without `dcp frame` (it refuses functions whose esp it cannot
    follow through jumps): pair our listing's `_x$[esp+N]` lines with asmdiff's aligned `-` / `+` lines and add the
    displacement difference (a 60-line script; cm_RunAuction).

- **Deep s41 (the near-byte list to 80 instructions, the 15 untagged ones at 47-76 instructions: 11 byte, 1 closer
  (52 -> 32), 3 notes)**:
  - **A search loop left by `goto` with a re-test behind it, or by `break` + `if (p == end)`**: the original has a
    found flag (`found = 0; for (..) if (match) { found = 1; break; } if (!found) { ..; continue; }`). VC6 decides
    the flag's test on each path and emits nothing for it, so the loop exits straight into the two arms; `break`
    plus a pointer / index test leaves the compare in (ai_UpdateEquipRows 76 -> byte, all three gotos as flags).
    The same for one store reached from three arms (`pop = 1` in each, `if (pop) sv_PopRequest = 1;`): the
    arms then count as one block for the priorities (ai_NewStorageSlotWindow r / first right, 52 -> 32).
  - **A default assigned first and overwritten in one arm** (`other = b; if (..) other = a;`, `u = A; if (sex)
    u = B;`): written as if / else or a ternary, the value is no longer live into the test (cm_DuelChooseRound,
    lobby_ShowMyPlayer: a scratch register one step off at the test and every one behind it).
  - **`lea ebx,[edi+0xa]` and `add edi,ebx` for two text rows** = a running coordinate: `y = 10; .. dy = h + 2; y +=
    dy; .. y += dy;` (lobby_ShowMyPlayer; `y + 0xc` and `y + 2 + y + 0xc` were the same values in other registers).
  - **A loop counter in edx from the first instruction, the invariant pointer in eax, a parameter pushed into esi**:
    the counter was set BEFORE the invariant (`i = 0; t = ..; for (; i < 2; i++)`), so it conflicts with the
    address temps and the parameter loses its scratch register; then a record pointer used once at the end
    written as the macro at each use (`GM_GOOD(n)->workTime / GM_GOOD(n)->batch`): gm_ObjectValue byte.
  - **Explicit `(float)(..)` around a double expression stored to a float is a scheduler node**: removing the casts
    in a long fill block let the stores fall where the original has them (cm_ThrowGrenade 48 -> 34 for two lines);
    `d.y -= 60.0;` instead of `d.y = (float)(d.y - 60.0);` was the last step. Also there: a value computed, then
    a neighbour, then `+ 50` in one pass on the FPU = `mid.y = d.y * 0.5; mid.z = ..; mid.y += 50.0;`, and constant
    stores the original issues last (`keys[0].time = 0x12; ..`) written last.
  - **Three 4-byte slots in another list order, frame < 0x80**: one variable used for two counting phases
    (`cnt2` / `cnt3` -> one `pending`, count 8) opens its slot first (amt_ElectOffices byte).
  - **One `_ftol` reached from both arms of a float choice** = a ternary: `v = (int)(G * k < lim ? G * k2 : lim);`,
    one definition of v instead of two lowered its priority below the record pointer m (cm_AuctionAskAI byte; the
    random factors written inline put the temps in the original's slots).
  - **A temp of one arm computed in eax and stored, the variable in memory, the other arm's int temp in another
    slot**: block-local `int add, adj;` in each arm and the bonus defined once per arm as `bonus = adj + 0x19`
    (he_TrialPayment); then **`add esp,4; shl esi,5` at the join after three calls** = `(util_RandomU(n) + b) * 32`
    in each arm: the equal tails cross-jump and take the pending `add esp` along.
  - **A late local packed with an early one in the original** (the tail's `slot` in p / k's slot): the tail copy of
    the word handler kept its slot in the parameter `k` (hst_ParseCommandline_2nd_Pass byte, @original comment).
  - **A record pointer computed right after a load, into a callee-saved register** (`lea edi,[ecx*4+X]` before the
    calls): `win = &d2_Wins[d2_Views[v].wins[3]];`; a call with one argument per arm and its own `add esp` =
    the call written in both arms; the strength-reduced base on the right field came with the plain nested index
    loops (gm_TechTreeWindow byte).
  - `prev = 0;` moved above `n = m->route.count;` (climb round 1): the zero register and prev share ebp, the status
    pointer leaves the registers (cm_ExSetRoute byte).
  - Left with notes: ai_DragTownMap (the release block between the init block and the reload edge block: no flow
    graph found), ai_NewStorageSlotWindow (bt / win tie at 24), he_RunMasterWork (master and bld need one more
    counted spill reference each: the c2frame replay gives the original's slot groups then), d3fl_Prerender (n
    never in a register in the original, more memory references; the split pieces decide).

- **Deep s42 (the near-byte list to 80 instructions: no untagged function left after the exclusions, so the 15
  tagged ones with the oldest notes (no session marker) at 36-38 instructions: 6 byte, 3 closer, 6 notes)**:
  - **A counter that should share a slot with a dead local, a swap temp that should share with a counting
    variable**: the swap temp was the counting variable itself. gv_RankCulpritsAll's insertion pass used st (the
    open count of the culprit just counted) as the temp for open[k] (`st = open[k]; .. open[k - 1] = st;`): st then
    lives through the pass, the down-counter conflicts with it and joins own's slot, x opens its own (c2frame
    --names: the packer searches the slots newest first, so who joins what is decided by the conflicts of the
    locals listed before). Byte; the y local was the only change.
  - **One scratch register for a value stored at every exit (`mov ecx,eax` in the first arm, the store from ecx in
    all four)**: the arms did not end in their own `store; return;` but set the look in an else-if chain and fell
    into ONE shared `cm_HoverObject = ai_PickedObject; return;`, which VC6 duplicates into each arm: one load
    temp, one register, and the copy where the value is already in eax (cm_UpdatePointer byte, size 840 -> 848).
  - **A clamp's `-0x7f` arm out of line (the original `jle` / `jl` to a block behind the loop or at the
    function's end)**: the clamp was `min(max(..), 0x7f)` with the min / max macros, and the operand order of max
    decides the jump: `max(v, -0x7f)` = `v > -0x7f ? v : -0x7f` gives `jle`, `max(-0x7f, v)` gives `jl` (one
    function used both; cm_ExRelations byte, five sites). The if / else-if clamps and plain ternaries are other
    layouts.
  - **A default overwritten in one arm** (`x = per->money; if (draw < x) x = ..;`) as if / else: the s41 lever
    again, the amount and the record pointer swap esi / edi back and edi is saved lazily (he_RunPettyLoss byte).
  - **A record block-scoped, then a move / swap climb from that base**: he_RunInsult's G28REC party in the block
    that fills and sends it (40 ins on its own) and climb `--base FILE:blk --ops move,swap` round 2 (host first,
    variant before members[1]): byte. The climb from the tree had found nothing; the block scope changes which
    orders the scheduler can keep.
  - **`fdivr st,st(1)` / `fstp result` / `fstp st(0)` where ours has `fdivp`**: the product was stored first and
    then divided (`ratio = a * b; ratio /= c;`): the product is ratio's first definition on the FPU and dies with
    a dead pop after the store; one statement lets the /O2 peephole fold the pop into fdivp (he_RunKidnap fpu
    hunk gone). The ratio written inline into the if is worse: VC6 evaluates the call of the comparison first.
  - **A search loop's success returning through r (`mov eax,esi`), the default arm not folded**: the s41 found
    flag again (ord_ApplyForOffice 36 -> 15 with `r = 1` inside the kind test, which gives the original's m / c / r
    registers).
  - **The send + count written in each arm** (the byte twin's style) gave ai_SendFreeSpies its parameter / count
    registers back (38 -> 25); the original's three sends are one call with `push &msg` hoisted into the compare
    block before the `jb` that skips `arg[1] = 4` (open).
  - Tool: `permute.py` + `dcp try` of 5040 orders took about 7 minutes and holds the daemon (every other dcp call
    waits); `sort` behind it buffers the rows until the end.
  - Left with notes: he_RunHire (the arg temps of gm_JobCost: flip.py names only the deletion of bt), ord_LevelOrder
    (r = 4 block between the kind-3 and kind-1 code with one shared return), veg_LoadModel (the heights load as a
    register candidate), d3d_EnumDeviceCallback (D3_COPY temps in dead parameter slots), gm_UpdatePrices (the
    p * 17 hoist common to three arms), ai_AmountMenu (the loop-top r = 0 skipped on entry), he_RunBurn (the case
    -1 name buffer's density), and the closer ones ord_ApplyForOffice, he_RunKidnap, ai_SendFreeSpies.

- **Deep s43 (the near-byte list to 200 instructions minus everything named in these notes: the 3 untagged ones
  and the 12 tagged ones with the fewest instructions, 7 byte, 8 notes)**:
  - **Four callee-saved registers rotated pairwise (n / ch and target / twice), the original comparing a pointer
    with the counter's zero register (`cmp edi,ebp`)**: the counter's `n = 0;` stood right before its loop, not
    with `ret = 0;` at the top (ai_UseGoodFrameChar 82 -> byte; its byte twin with two more calls tolerates the
    top init). A counter initialised far above its loop is a candidate over the whole function.
  - **A temp spilled to a slot and reloaded inside one expression (`mov [esp+0x10],eax` .. `mov edx,[esp+0x10]`),
    the frame's slot order one 4-byte local off**: the original had no named pointer locals for the two frame
    records; `&a->frames[s->frame]` / `&next->frames[i]` written at each use (as CSE temps) take the registers
    the named fr / nf held, and the pressure spills the difference into the slot that then sorts first
    (ch_CheckAniMorph 86 -> byte; climb's `inline:fr` found the first half).
  - **`and eax,0x5f; add eax,0x130; .. add eax,ecx` where ours has one `lea [eax+ecx+0x130]`**: a ternary of the
    two constants, `(c->sex != 0 ? 0x18f : 0x130) + t->job`, not `(sex ? 0x5f : 0) + 0x130 + job` (he_RunTrialEvent
    94 -> byte; the rest of the function followed).
  - **The store of an address-taken local sunk behind the next call's pushes**: block scope again
    (`{ int year; year = ai_Time.year; cm_BeginFieldBlock(..); cm_AddFieldDelta(.., &year); .. }`):
    he_StartDarkCorner byte.
  - **The (float) cast lever of s41 again**: `rot.y = cm_Random(0x168) * 3.14159274f * (1 / 180.0);` without the
    explicit cast lets the pos loads fall behind the fild (cm_SinkDownAction byte; its byte twin
    cm_FallDownAction keeps the cast, the sound call in between changes the block).
  - **`fld unit; fmul score` where the original loads the score first**: the byte siblings (the other ai_Plot*Chart
    functions, below twin.py's 0.5 threshold) named the two values `from = ..score; to = ..score;` before the call;
    as float locals the products come out in the original's order (ai_PlotScoreChart byte). Read the neighbours
    by name when twin.py finds nothing.
  - **Pops of the dying distance terms before `fmul 0.001`, then `fld st(0); fcomp 1.5` and `fimul` consuming the
    clamped value**: `scale = sqrt(..); scale *= 0.001;` (the s42 `r = a * b; r /= c` lever), and the clamp in the
    same float as `scale = min(1.5, scale);` with no double copy (`min(scale, 1.5)` / if-clamps keep the value
    alive past the multiply): he_AllocBurn byte.
  - **A strength-reduced pointer's base field follows the LAST field referenced in source order in the loop**:
    swapping the two stores of an entry clear moves the base from `state` to `charId` (cm_ResetCutInfos); naming
    / unnaming an index changes which field the rec pointer is based on (ai_DecideStudy). Both still open.
  - Left with notes: ai_DecideStudy, cm_ResetCutInfos, ai_FavourComment (the volatile x: without it VC6 threads
    the decided x test), ord_RandomObjectId, gm_BuildingRank (the default body one rotation step), ai_IntimidateContact,
    gm_CharacterIncomes, d3_ProjectMesh.

- **Deep s44 (the near-byte list to 200 instructions minus everything named in these notes, the 15 with the fewest
  instructions at 12-44: 12 byte, 3 closer with notes; bonus amt_FindOffice byte)**:
  - **A hand pointer walk over a table (`for (c = recs; (int)c < (int)end; c++)`, `for (i = 0, rank = ranks; ..;
    i++, rank++)`, a `do { if (p->idx == x) break; p++; i++; if (p >= end) return 0; } while (1)`)**: the plain index
    loop (`for (i = 0; i < 768; i++) .. lc_CharRecs[i].id`, or `rank = &ranks[i];` per pass). VC6 strength-reduces
    it into the same pointer compare (the signed `jl` included) but the IV inits, the temps and the saves come out
    as the original's: ai_PickBuildingChar 38 -> byte, ai_PlayerComparePanel 41 -> byte, amt_AcceptApplication
    (with `k.id != m->kind` read from the message again, not from the byte local) 43 -> byte.
  - **A record field read through a callee-saved register late, `push` of an argument late, the records filled
    after a call in another arm**: the records of the call arm (`AIARG s1, s2` passed to ai_ChooseSubAction) were
    block locals of that arm; the tail's own s1 / s2 then never escape and its loads hoist above `*o1 = s1`
    (ai_DecideUseMilitaryObject, then a 720-order round of the tail fill: `s2.kind = 4; s2.ref = b->id; *o1 =
    s1; ..`). The same for a message in a loop: AIMSG msg declared in the click block that fills and sends it
    (the address went to the send in the previous pass) plus a 360-order round of the fill: ai_DungeonBribeWindow
    byte (s43's block-scope lever, now for loops).
  - **`fld st(0)` / `fstp st(0)` around one of several sin / cos terms of the loop index, the last term consumed by
    `fmul [mem]`**: the index converted once into a float local (`f = i;` at the loop top, `f` in all four terms)
    and the fourth term as `v.w` (float) instead of a double d: d3fl_UpdateWater byte.
  - **`xor eax,eax` / `mov eax,1` placed before the out-parameter stores, the exits sharing them**: one result
    variable set in each arm of if / else and one `*worth = ..; return r;` (gm_FindRichest, the goto none gone);
    `r = 0;` before the guard, a `break` with `r = 1` in the search and the copy under `if (r)`
    (amt_FindOffice, prologue `xor eax,eax`).
  - **The ai_DecideBlutrache helper loop**: `who[cnt++] = ..` before `s += t;` and `for (i = 0, cnt = 0; ..)`
    (climb swap from the cfor base; the xor order of i / cnt follows the for-init).
  - **A switch case whose no-work path jumps (goto) past the next case**: write `ev->stage++; return;` in the case
    itself and put `case 0:` with the same two lines before the next case: VC6 cross-jumps case 0 into case 1's
    copy and the table entry is the original's (he_RunProduktion 40 -> 4). The same function's debug line had its
    own `char buf[0x200]`: one reference at the top of a 0x600 region = a second array behind a 0x400 one.
  - **`push 1` / `push 0` where ours computes the flag**: `f(x, c ? 1 : 0)` was the call written in both arms
    (`if (c) f(x, 1); else f(x, 0);`): ai_OfficeApplicantsWindow 42 -> byte.
  - **Copy temps in a callee-saved register (esi) and the pops at the end where ours copies through ecx and pops
    right after the rep movsd**: the value stored before the copies in ours was assigned AFTER them in the
    original (`m.fight.health = c->v024;` last): its load is scheduled first, the scratch registers are busy and
    the copies take esi (cm_SendFightCommand, the 7 positions of the one statement).
  - **`mov eax, 4` for a mask held over the flag test, three zero words from one register**: the `__int64` mask
    as `LARGE_INTEGER mask; mask.LowPart = 4; mask.HighPart = 0;` right before the call (he_AllocPlunder; the
    s40 union lever; before the flag test it is 2 instructions off, `HighPart` first worse).
  - **A one-rotation register shift through a short function**: the byte sibling's named local (`plants =
    (G15PLANT *)b->plants;` as he_RunHarvest has it) plus `harvestType` before `harvestScript` (he_AllocHarvest).
  - **An order hunk at the start of a long float block**: parenthesised products `(d.x * t) + a->x` in the four
    pool stores move the 81-instruction region cut (d3_ClipPoly 12 -> 6; c2dag shows the block ending at the
    colour delta). Read c2dag's block sizes before spelling the hunk's own statement.
  - **`ang *= 0.25;` as its own statement** before the rotation call (fld ang / fmul / fadd rotation.y), and
    `n = cm_Random(..) + w->dmgMin;` in one statement (the add into eax): cm_UpdateSceneChars 41 -> 8.
  - Left with notes: d3_ClipPoly (the d1 operand order, one fstp), he_RunProduktion (`(skill + 0.5) * fv`
    scheduled before the flag load), cm_UpdateSceneChars (c->health pushed before the _ftol).

- **Deep s45 (the near-byte list to 200 instructions minus everything named in these notes, the 15 tagged ones
  with the fewest instructions at 44-58: 8 byte, 7 notes; he_AllocTrade from s44's 20-instruction try)**:
  - **A ternary argument computed first (`mov eax,[s]; neg; sbb; add eax,7` before the other argument loads)**:
    the value was an int local set by if / else right before the call (`if (s != 0) type = 6; else type = 7;
    k = f(type, ..)`): VC6 if-converts it to the neg / sbb form but keeps it ahead of the call's argument tree.
    `type = s ? 6 : 7;` is forward-substituted (= the inline ternary), a char / unsigned char local or the
    default-then-overwrite form are other code (cm_ExCreateNamedCharacter 58 -> byte).
  - **`push &msg` hoisted above the constant byte stores of a message fill in a loop, the netId CSE in another
    scratch register**: AIMSG msg as a block local of the click block (s44) AND the chained `msg.arg[2] =
    msg.fromId = msg.to = x;` written as separate stores (the chain is one tree: no order of it moves), then an
    order round with the three stores split (420 orders, the triple's internal order fixed): `to, fromId,
    type, flags, arg[1], arg[2], arg[0]` (ai_DenounceWindow 44 -> byte). The same split stores at function scope
    stay at 42+: the block scope is what lets the loads pass the stores.
  - **A switch case freeing the record, then `case -2: case -1:` freeing it again (two FreeEvent calls in
    ours, one call reached by fall-through in the original)**: `case 2: ..kind = 6; /* fall through */ case -2:
    case -1: he_FreeEvent(ev);`. One reference of ev fewer lowered it below the counter n: the callee-saved
    pair esi / edi came out the original's way (he_RunTutorialBuildingTour 52 -> byte). Read the jump table:
    two entries pointing into one call block = fall-through.
  - **A test the original does not thread (it jumps to the shared `cmp eax,-1` copy, ours straight to the
    push), plus a constant -1 in a callee-saved register only in ours**: the guard folded into the arm's
    condition with the redundant re-test in each arm: `if (ct != -1 && (kind == 1 || kind == 2)) { item = ..;
    if (item) {..} else if (ct != -1) empty; } else if (ct != -1) empty;` (cm_UpdatePortraits; the separate
    `if (ct == -1) continue;` guard threads the else). Before it, climb's `local` mutation found `d2_LastWindow
    = p->win; d2_WinOpenPic(.., d2_LastWindow)`: the global just stored is the argument (46 -> 14).
  - **`test al,al` kept after `add al,0x67`, the record pointer in ebp instead of ebx**: `k = 0; if (..) k =
    ..;` as `if (..) k = ..; else k = 0;` (the s41 default lever): the byte value then counts as a candidate
    across the join, the pointer avoids ebx; then a 24-order round of the tail stores (cm_PlantEscapeFlag 49
    -> byte).
  - **A stored float reloaded from a 4-byte spill slot (`fst [esp+0x10]` .. `fld [esp+0x10]`, push ecx frame)
    where ours keeps the named t on the FPU**: there was no t: `c[x] *= 0.01; if (k != 0) c[x] += c[x - 1];`
    (VC6's CSE of the stored value spills). Before it the hand pointer walk as a plain index loop, and the
    second loop over the kinds (`for (b = 0; b < 6; b++) .. c[(b * 5 + i) * 6 + k]`) instead of `b += 5`:
    the original's (b + i) * 6 recomputed per j round with b / i apart (cm_InitOrderChances 54 -> byte).
  - **Two hand-written border loops `for (x = 0; n; n--, x += sw)`**: plain index loops with `i * sw` in the
    coordinate (VC6 strength-reduces them into the same countdown and x slot, and the dead-parameter slots pair
    as the original's): d2_DrawWindow 54 -> 12. The last 12 were `w->w + w->x - pw` computed as `(w->w - pw) +
    w->x` in ours whatever the spelling (all 216 spellings = one code): only an unsigned subtrahend keeps add
    then sub; kept as `- (unsigned)pw` with an @original comment (byte). Open: the plain source of that.
  - **he_AllocTrade** (s44's ingredients): the `e2->cart == p->cart` swap plus a 720-order round of the six
    message stores after type / fromId / to (`flags, time, arg[2], jobId, arg[1], arg[0]`): byte.
  - Gotos to a mismatch / step label in a filter loop (he_NextEvent) as a match flag (`match = 1;` each filter
    sets ok or clears match, `if (!match) ok = 0;`): the same code; VC6 decides the flag per path.
  - Probes: `*(int *)&v2 = *(int *)&v` (v2 kept in memory) takes gv_FireShots 46 -> 16: the original does not
    forward `v2 = v` into the first ball; the source form that blocks it is still open.
  - Left with notes: gv_FireShots (v2 forwarding), sv_TabButtonRow (a 109 / 109 / 109 priority tie of x and the
    loop IVs), ai_BribeAmountCB (the gm_GetWorth call order: a deeper argument tree goes first), vfs_EntryPath
    (out served before the loop pointer's two webs), rt_ChooseGoodsDialog (const 1 avoids ebx), ai_ChooseMission
    (view / first 181 / 182: one more distinct value in block 0), he_NextEvent (the global cursor candidate
    takes eax before the hoisted filters).

- **Deep s46 (the near-byte list to 200 instructions minus everything named in these notes, the 15 tagged ones
  with the fewest instructions at 65-86: 5 byte, 10 notes, 7 of them closer)**:
  - **Zero stores of a counter array from two scratch zero registers (`xor eax,eax` / `xor ecx,ecx`, the first
    element an immediate) where ours keeps 0 in a callee-saved register over the function**: two arrays with
    aggregate initializers, `int rcnt[4] = {0}; int cnt[6] = {0};` (the later-declared one at the lower address).
    The initializer stores never join the global constant-0 web, so the later `*g = 0` / `*r = 0` stay
    immediates (ai_PokerOpenCounts 66 -> byte; hand stores in any order, chains, one `cnt[10] = {0}` and the
    explicit `{0, 0, ..}` lists were all other code).
  - **A 32 bpp pixel read at byte offsets 0 / 4 / 8 (`mov dl,[eax+ecx*4+4]`)**: the low bytes of three
    consecutive dwords, `r = (unsigned char)((DWORD *)surf->bits)[i]; g = ..[i + 1]; b = ..[i + 2];`, and the
    24 bpp arm indexing the byte array with no pointer local: the scratch rotation of the whole function was
    the p / i locals (d2_GetPixelColour 66 -> byte).
  - **An address-taken int sent as a field delta pins a slot over the whole function (frame 4 bytes larger,
    the other locals out of the dead parameter slot)**: the int is a block local of the field block that sends it
    (`{ int kind; kind = 8; cm_BeginFieldBlock(..); cm_AddFieldDelta(1, 1, 2, &kind); .. }`); cm_RunBirth's
    script handle then shares m's slot, gm_PartnerStep's arg declared in the pregnancy block lets the n spill
    and the float temp share its slot (frame equal). Then cm_RunBirth's `push 0` / `push 2` hoisted above the
    sex tests with one `add esp,4` at the join: the voice call written out in four nested if / else arms
    (`cm_QueueVoice(-8, bank, "_GEBURT", cm_Random(2) + 2, 0)` each): VC6 hoists the arms' common first pushes
    and merges the tails; a ternary argument evaluates the ternary first (byte).
  - **ai_MasterExamWindow**: the s45 levers together: the 0x15bd arm's title as an int set by if / else before
    the call (the job byte loaded after the sbb), the exam AIMSG a block local of the click arm and a 720-order
    round of its fill (type, to, fromId, flags, arg[0], time): 82 -> byte in one 1441-variant round.
  - **A register swap of the event and the character through the whole function**: the default of a news chain
    (`n = 0;` before `if (..) n = A; else if (..) n = B; else if (..) n = C;`) written as the chain's own `else n
    = 0;` (he_RunOfficeAward 76 -> 9). The original still branches for the last arm (xor before the chain, no
    setne / dec / and): the form that gives both is open.
  - **`goto insert` / `goto shift` in a slot search**: a found flag (`found = 1; break;` in both arms, the insert
    under `if (found)`): the identical code (he_RunAuctionEvent). Writing the insert out in both arms with
    `break` was closer (52) but not cross-jumped: +16 bytes, a size-queue entry more.
  - **`inc edi; cmp edi,6; jl` closing a row loop whose head tests `j < max` with jae**: the index is an int
    (ai_FillWeddingTab). **`inc ebp` after the multiply chain of a pointer computation**: `n0++;` after the
    `bt = ..` line (ord_ComputeStatistics order 24 -> 8). **The ai_CharsT index computed before m is reloaded**:
    `m->host == ai_CharsT[ai_PlayerId].netId` (cm_RunDuel 74 -> 68).
  - Left with notes: ai_FillQuarrelTab (n 33 takes ebx before max -52), gm_PartnerStep (mate picks ebp although
    ebx is free: the block shape decides), cm_DuelShotAction (J reached before the sample subtree: a cross-jumped
    copy?), d3_CornerCB, cm_RunDuel (one scratch rotation from the news argument), he_RunOfficeAward (the sex in
    al across the rep movsd), he_RunAuctionEvent (n's stack home: one combined search test gives it), ord_ComputeStatistics
    (VC6's shared 1 / cntf), ai_FillWeddingTab (roles[5] after the test: 47 ins at +1 instruction), rt_LoadRoutes
    (the r pointer form: frame 0xc, i spilled).

- **Deep s47 (the near-byte list to 300 instructions minus everything named in these notes, the 15 tagged / draft
  ones with the fewest instructions at 89-138: 3 byte (one with a meaning fix), 1 draft -> shape, 11 closer or
  noted; bonus: the shape lever in three draft siblings)**:
  - **A constant 0 held in a callee-saved register from the prologue (`xor esi,esi`, `push esi` for every 0
    argument, `cmp eax,esi`), the original pushing 0 as immediates and clearing a counter array from a scratch
    `xor ecx,ecx`**: the three clears `searches[0] = 0; searches[1] = 0; searches[2] = 0;` (twice: before the
    frame loop and in it) were an element loop `for (i = 0; i < 3; i++) searches[i] = 0;` (memset of the array:
    the same code). The unrolled stores were the 0 web's references that made 0 a candidate:
    ai_TradeSearchWindow 103 -> byte, its twin ai_AquariumSearchWindow 120 -> byte (the s46 `= {0}` lever's
    sibling: a zero fill that VC6 writes itself does not count toward the 0 candidate).
  - **A ternary text id computed into a callee-saved register before the call's other argument loads (`neg cl;
    sbb ecx,ecx; and ecx,0xa; add ecx,0x114; mov edi,ecx` before the call, `add eax,edi` at the push)**: the
    text-id base as an int local right before the call, `text = (ch->sex != 0 ? 10 : 0) + 0x114;
    io_PrintText(0x1d8f, text + ch->b00d, ..)` (if / else of 0x11e / 0x114: the same code). The local also
    re-ranked done / win (done went to its slot, win to ebp): ai_DivorceDialog 89 -> byte. The s45 lever
    (ternary as an if / else int) for a ternary that is part of a sum.
  - **A record address computed into a register before its field tests (`add ebp,edx; mov eax,ecx; add eax,ebp;
    mov dl,[eax]`) where the original forms it in place (`add ecx,edx; lea edx,[eax+ecx]; mov cl,[eax+ecx]`)**: no
    `AIGOOD *g = &ai_GoodsT[e->item];` local: `ai_GoodsT[e->item].type` / `.batch` written at each use (VC6's CSE
    temp, not a candidate) - ai_EvaluateProductionGraveyard 138 -> 52, draft -> shape; the siblings
    ai_EvaluateProductionGypsies 328 -> 243, ..Flags 335 -> 262, ai_EvaluateProduction 548 -> 512. The s43 lever
    (pointer locals written out at each use) for a table record.
  - **One arm leaves a search by `goto done`, the other sets `found = 1` and tests it after**: both set the flag,
    one `if (found) goto done;` after the if / else (ai_EvaluateProductionGraveyard 52 -> 44, the first 487
    listing lines equal).
  - **Two characters' ids re-read after a memset / rep stosd in ours, the original comparing two registers (`cmp
    dx,bp`) and reusing them for the record fill**: the ids as `unsigned short` locals read once (`theirs =
    p->chr; mine = e->chr; if (theirs != mine) { memset(&party..); party.members[0] = ai_CharInts[mine][0]; ..`):
    the memset does not kill them (he_RunGathering 122 -> 72, size 1454 -> 1486 of 1497; int locals 80, the other
    read order 74).
  - **Layout loops with hand induction locals (`for (j = 0, off = 0; ..; j++, off += 0xca)`, `colY += 0x80`)**:
    the products written out (`rows[i].x = j * 0xca (+ 0x65)`, `rows[i].y = col * 0x80 + step`): VC6 builds the
    same IVs itself with the original's registers and size (ai_ShowAmountRows 117 -> 95, size 1370 -> 1374 =
    the original). `do { .. i++; } while (i < 3);` with `i = 0;` at the top as two plain `for (i = 0; i < 3;
    i++)` loops: d3s_ShadowTriangle 133 -> 120, size 906 -> 905 = the original. A hand `do { .. o++; } while
    (--n)` over a call's result as `for (k = 0; k < n; k++) { o = &offices[k]; ..}`: the countdown store moves
    behind `push ebx` as the original's (he_RunSixGroupEvent).
  - **The frame 8 bytes larger, the walk tail's address-taken ix / iz / py in their own slots**: all three as
    block locals of the tail (`{ int ix, iz; float py; .. d3_GetHeightAt(.., &ix, &iz, &py) .. }`), not only one
    or two: they pack into the dead parameter slots (cm_Attack 114 -> 101). An unparenthesised `a * 0.01 * b`
    of two byte fields was regrouped by VC6 into `a * b * 0.01` (fild, fild, fmulp, fmul); `(a * 0.01) * b`
    keeps the original's fmul between the filds (cm_Attack 101 -> 95).
  - **`cmp reg,-1; jle` with -1 in a callee-saved register from the prologue where the original has `test
    reg,reg; jl`**: the found test was `tx >= 0 && tz >= 0`, not `> -1` (rd_PathToRoad: size 341 -> 338 = the
    original, the -1 candidate gone; the instruction count rose 132 -> 135 behind the prologue difference).
  - Fixed: ai_AquariumSearchWindow's search message 0x15 carries the room object in arg[2] (+0x60), not in
    LCSEARCHWORDS.room (+0x5c, the 0x14 message's place): a one-instruction slot hunk inside a message fill.
  - **A `continue` jumping BEHIND the latch's reload of a global (`je` to `inc edi`, the reload `mov eax,[g]` just
    above it), ours jumping onto the reload**: the global's web (read in the loop condition) has a register on the
    skip path in the original. An extra block behind the inner loop gives it: the inner `for (o = f(); o; o = g())`
    as `o = f(); if (o != 0) do { .. } while ((o = g()) != 0);` (same meaning, `continue` reaches g's test);
    `do { for .. } while (0)` is the same code. Two increments (`i++; continue;` in the skip arm) give the
    latch too but strength-reduce a shared record address into a second IV (lc_UpdateGoodsReserves 341 -> 241
    bytes, size equal; one rotation left).
  - Left with notes: lc_UpdateGoodsReserves (the cursor K in ecx where the original has eax: the original computes
    the IV init and the entry test from K, ours from i), he_RunSixGroupEvent (n takes eax, one rotation), d3_InflateObject, ai_KidnapWindow (the 1 is no candidate in ours),
    cm_Attack (g / k ranks: one k reference fewer swaps them), he_RunGathering (the not-found block last: every search spelling is one
    code), rd_PathToRoad (i in ebx, g->cells re-read per row), ai_Miracle (case slot packing, one rotation),
    d3s_ShadowTriangle (y outranks minY; the original computes y in minY's ebp).

- **Deep s48 (revisit: the 15 near-byte functions (2-4 instructions) whose notes date from s36 or earlier: 3 byte,
  1 same-code readability fix, 11 notes)**:
  - **`call f; fmul K; fcomp [param]; add esp,4` where ours has `add esp,4` before the fcomp**: the scaled call
    result was a named float compared from the parameter's side, `half = gm_CharacterBaseHealth(c) * 0.5f; if
    (value < half && ..)` (he_CharAccident, he_CharIllness byte). Inline in either order, `/ 2`, `0.5f *`,
    a double constant, a `(float)` cast, the local compared as `half > value`: all the add-first code. A plain
    `f(c) > p` without the multiply already puts the fcomp first (he_RunDiseaseSim).
  - **A -1 init stored before the early guard, but then the zero register leaves esi (82 ins)**: the loop counter
    `i = 0;` belonged right before its loop (`win = d2_LastWindowPtr; i = 0; while (i < win->nobjs)`), not with
    t0 / ret / bar at the top: then `t0, ret, bar = -1` before the guard is byte (sv_MessageBoxGreen; s43's
    counter lever for a while loop; `for (i = 0; ..)` and `i = 0` before the win load are other code).
  - **`goto` to the code behind an if / else whose failing arm skips a clamp + store**: a flag (`drag = 1; .. else
    drag = 0; if (drag) {clamp; store}`) compiles to the identical code (d2_SliderInput, the goto gone).
  - **c2dag reading of an order hunk**: a load issued before an `and reg,K` in ours where the original issues the
    other load first = the `and`'s flags edge into the block's final `cmp` gives its load one height more
    (d3t_EarthCB, open). A reload or store placed one step off at the start of a long float block is the 81-node
    region cut (d3_EnvMapCoords 83 nodes, d3t_UpdateSnow 83 + 81): count the nodes with c2dag before spelling.
  - Left with notes: d3_MirCreateOutline (a two-register address's operand order follows the load order at
    every site), ch_GetWaypoints, d2_ShpBlit16HalfDark, d3_EnvMapCoords, d3t_EarthCB, he_CatchMeetingDisease
    (delta as a block local gives the frame, a then loses ebx to ch), d2_SliderInput, he_AllocMeeting,
    he_MoveReaper (every operand order of the step / target lines is one code), gm_RankPlayers, ch_ActTurn,
    d3t_UpdateSnow.

- **Deep s49 (large drafts: the 8 with the fewest instructions at 139-159 of the draft pool without notes here: 1
  byte, 2 draft -> shape (one after a meaning fix), 5 closer with notes)**:
  - **Two callee-saved registers swapped through a whole handler (the event pointer and the character), the
    sex-dependent text ids passed as `(c->sex != 0 ? 10 : 0) + K`**: the ids as an int set by if / else right
    before each call (`if (c->sex != 0) text = K + 10; else text = K; io_PrintText(.., text, ..)`; the s45 / s47
    lever). Each site splits the call block and adds character references, so the character's priority rose
    above the event's (c2trace 55 -> 64 against 63) and the pair swapped: he_RunTitleCeremony 150 -> byte with
    four of the six sites (0x1618 and the rival's 0x42a4 / 0x42a2 / 0x42a0), the two `title + 0x114 / 0x11e`
    sites inline (all six: 12 ins). A subset round (3^n: inline / if-else / ternary local per site) finds it in
    one `dcp try`; `flip.py --want 'mov\s+edi, DWORD PTR _ev\$'` named the 0x1618 line first.
  - **A table index kept as a byte offset across a memset (`lea edx,[eax+eax*8]; shl edx,2; mov al,[edx+X]` ..
    `rep stosd` .. `mov eax,[edx+Y]`)**: the record was read at each use through an int index local (`owner =
    b->owner; if (ai_CharsT[owner].kind == 6 ..) { memset(&msg ..); msg.to = ai_CharsT[owner].netId;`), not
    through an `AICHAR *` pointer and not through `b->owner` again (VC6 re-reads that after the memset): the
    offset is a CSE the inline memset does not kill (he_RunProtection 148 -> 28, then a 719-order fill round:
    shape 8).
  - **The byte twin's form first**: ai_PassesBuildingFilter's class as `type->kind` at each use (no `unsigned
    char cls` local, as its byte twin ai_PassesMapPickFilter) put 0xffff into esi (139 -> 85); then the
    word read three times (`cfg->classMask[1]`: sign test, hi test) as a local read right after the type pointer,
    before `lo = 0` (the original's `mov ebx,[edx+0xc]` in the type block): 85 -> 3. ai_ProductionWindow:
    `fresh = 1;` inside the `count > 1` test as the byte sibling ai_ExtractResourceWindow (151 -> 127).
  - **A price stored across a call in one frame slot in all blocks of a function**: the same variable for the same
    role in each copy of a block. ai_BuildTradeOffers' entries block had `diff = item price; price = goods -
    diff;` where the slots block has `price` stored and `diff` on the FPU: naming them alike (`price =
    gm_ItemPrice(..); diff = gm_GoodsPrice(..) - price;`) removed every slot hunk (152 -> 130); then climb move /
    swap rounds on the record fills (130 -> 14).
  - **A not-found arm placed at the end (`je` far forward, the -1e6 store after the loop's code)**: `if (p != 0)
    {..} else ai_Slots[i].saving = -1000000.0f;` instead of `if (p == 0) {fail} else {..}` (ai_BuildTradeOffers,
    with the margin computed in the goods price's definition `diff = gm_GoodsPrice(..) - price`: size 2952 ->
    2953 = the original).
  - **A search with `goto next` and a re-test `i < n` inside the loop is one code with the plain `for (..) if
    (match) break; if (i < n && worse) goto next; if (i >= 0x80) goto next;`** (17 forms, all the control): keep
    the plain one for readability.
  - **`lea edi,[esi+1]` after a call, spilled, `mov esi,edi` at the latch, the counter not strength-reduced**: the
    counter was advanced in the body right after its last use, `for (i = 0; i < 3; cart++) { ..; sprintf(name,
    "..%i", i + 1); i++; ..}` (cm_SetupFightScene 156 -> 84; `++i` in the argument: 134, `i++` in the header:
    control). The fighter loops' esi / edi swap went with it.
  - **A guard's free block reused for a later exit (`jae` back to +0x14, he_FreeEvent once fewer)**: the late exit
    written as the else arm, `if (ai_Time.hour < 22) { next run; } else he_FreeEvent(ev);`; `if (hour >= 22) {
    free; return; }` keeps its own copy (he_RunMarketSupervisor, size 3479 -> 3457, padded right).
  - **`fld K; fdiv b; fmul a` where the original has `fld a; fmul K; fdiv b`**: `a * K / b` in float context is
    reassociated into `K / b * a`; `(a * K) / b` keeps the original's order (he_RunMarketSupervisor, three
    sites). The same for `m * (1.0 / 60) * (d * k)` flattened into `d * k * m * (1.0 / 60)`: `(m * (1.0 / 60))
    * (d * k)` keeps `fild m; fmul; fld d; fmul; fmulp`.
  - **Rejected for size**: ai_ProductionWindow's class tests through an AIBTYPE pointer (`type->kind` at each
    test: the original's `lea eax,[edi+eax*4]; mov al,[eax]`) fixed that hunk but raised the size 2335 -> 2337
    in the current allocation: reverted.
  - Fixed: he_RunProtection's player-owner question waits 3 hours (`gm_AddTime(&e->end, 3, 0, 0)`; the original
    pushes 0, 0, 3), not 3 minutes.
  - Left with notes: ai_PassesBuildingFilter (3: the original loads classMask[0] into eax before `test`;
    flip.py hits only a duplicated test line), he_RunProtection (shape 8: b->id / netId load order),
    ai_ProductionWindow (bld in ebp, the column offset in a slot), ai_BuildTradeOffers (14: the shop entries
    fill's -1 register), cm_SetupFightScene (84: i in a slot at the loop head), he_RunMarketSupervisor (122:
    frame packing), sv_HandleSelectionMouse (159: the send through c gives the original's IV base but map
    moves into a register).

- **Deep s50 (large drafts: the 8 with the fewest instructions at 163-212 of the draft pool without notes here: 3
  byte, 1 meaning fix, 2 closer, 4 notes)**:
  - **Every failure exit a `goto fail` / `goto done` to one cleanup block (`if (e->slot) he_ReleaseSlot(ev);
    he_FreeEvent(ev);`), ours 54 instructions longer with duplicated return blocks and uncross-jumped news
    tails**: the cleanup written out at each exit (`if (bld == 0) { if (e->slot != 0) he_ReleaseSlot(ev);
    he_FreeEvent(ev); return; }`, the same for the plain free). VC6 cross-jumps the copies into one block itself,
    and the parameter's register piece then ends where the original's does (the stage tails reload ev from the
    argument slot and share their news / free blocks): he_RunFeastExam 186 -> byte, size 1577 -> 1455 = the
    original, in one variant. The guide's first rule (replace the goto first) for a whole handler; he_RunTrade's
    goto done / wait / moving written out (18 combinations) was worse: its labels are a real shared tail.
  - **A flag initialised in its declaration at the top (`int leaving = 0, idle = 1;`) where the original keeps
    the constant 1 in ebp for every `push 1` and ours has `mov edi,1; mov ebp,edi`**: the flag set right before
    the search loop that clears it (`idle = 1; for (..) if (busy) { idle = 0; break; }`): the constant 1 and the
    flag are one value at the top, he_RunRaid 199 -> 172 and the size equal. Then the three `raid100[k] = -1`
    stores as an element loop (`for (i = 0; i < 3; i++) party.raid.raid100[i] = -1;`: the -1 left ebp for a
    scratch `or eax,-1`, 172 -> 18; the s47 lever for a -1 fill), `float chance = 1.0f` declared before the int
    flags (the 1.0f store first), and the start building into the existing `home` local before `raidHome =
    home->id` (k = 0 after the call): byte.
  - **The slider's max read into a register before d2_GetObjValue (`mov edi,[esi+0x34]` before the call)**: the
    value in the existing `max` local first, `max = slot->stock; value = d2_GetObjValue(..); d2_SetObjValue(..,
    max, ..)` (ai_ShowProdSlot 163 -> 2). Then `(float)(1.0 - a / b)` as `1.0 - (a / b)` without the cast: the
    fsubr after the index arithmetic as the original (byte).
  - **A frame 4 bytes larger in the original with a hole behind a 12-byte vector, the struct copy `zpos =
    cam->pos` through a pointer register in ours, field loads in the original**: zpos a D3VECTOR4 copied field by
    field (the original's dead pos.y store kept), the rotation copied field by field too (sv_MouseCamera 188 ->
    170, frame 0x90 equal; zrot as D3VECTOR4 too gives 164 but frame 0x94).
  - **b assigned before the test that guards its only use (`b = path_Goal; if (path_Start != b)`)**: inside the
    test as the sibling path_Search2 has it (path_Search1 176 -> 150, the M8s gone).
  - Fixed: sv_MouseCamera's drag scale is `1.0 / 16.5` (the double 0x3faf07c1f07c1f08); the literal
    0.0606060606060606 is one ulp below it (an M10 at +0x5db).
  - Left with notes: he_RunHerbGathering (0 a register candidate in ours, savings 0; -1 from the entry; text /
    line tie in the frame packer), path_Search1 (the start cell a word local in map's dead slot), sv_MouseCamera
    (my / 32000 / -32000 register ranks; HF / AF kept on the FPU after the store), init_SyncSceneObjects (a in
    ebx with 16-bit compares in the part loop), he_RunCustomers (one scratch rotation from the first JobCost; a
    stage 2 candidate re-rotates it).

- **Deep s51 (large drafts: the 8 with the fewest instructions at 232-246 of the draft pool without notes here: 1
  byte, 6 closer, 1 note)**:
  - **A count global copied into a local (`n = ai_NSlots;` .. `ai_Slots[n].f = ..`, `n = ++ai_NSlots`) where the
    original keeps the count in ecx between the calls and recomputes the slot offset into esi for the post-call
    stores**: there was no copy: `ai_Slots[ai_NSlots].f` and `i < ai_NSlots` at each use, `ai_NSlots++`. A scalar
    global is its own candidate between its kills; the named copy took esi for the whole loop and spilled the
    loop pointer (frame 4 bytes larger): ai_FillSlotTable 232 -> byte in one variant.
  - **A frame 4 bytes larger in a long handler made of themes / cases, each original slot holding variables of
    several themes (theme 2's r0 with theme 0's fl, theme 0's elig apart from the other themes' elig)**: every
    theme's locals declared in the theme's block, and the address-taken send int (`two = 2; ..
    cm_AddFieldDelta(.., &two)`) a block local of the macro's success block: he_SpawnFounders 236 -> 170, frame
    = the original's. The s46 block-local lever for a whole function: per-theme variables are other variables
    to the frame packer. Then an `unsigned char k = bt->kind;` local spilled in ours, kept in dl by the
    original: `bt->kind` at each use (170 -> 167).
  - **The byte sibling's forms carry over to the copy**: he_RunAttack (he_RunRaid's twin) with the raid100
    element loop and the start building read into `home` before gm_Query / raidHome: 239 -> 54, size = the
    original; then a 720-order round of the party fill (-> 40).
  - **A record copy placed before the global stores that precede it in ours** (`next = cm_CutInfosT[i];` before
    `cm_PartyTurn = i; cm_CutChar = c;`): cm_RunSceneParty 237 -> 69 (climb round 1's move).
  - **A case's exits `return;` where the original's cross-jumped tails come out the other way round**: the case
    ending with `break;` into the empty end after the switch (he_RunPopulation cases 2 / 6: 231 -> 192, a 63-way
    subset round of the cases; the other cases' break is the same code or worse). A search loop
    `while (found == 0 && n != 0)` as `while (n != 0) { ..; if (found != 0) break; }` gave the character ebp
    as the original (240 -> 231).
  - **A message-box arm with its own `he_FreeEvent(ev); return;` right before the switch's `break` to the shared
    free**: drop it, the arm falls into the free (he_RunBuildingSpying, with the no-spy exit as break: 246 ->
    178); `(rank * 120.0) * (252.0f / (int)mean) / n` parenthesised (the s49 product lever) and the `all = 1`
    flag right before its loop (s50) -> 170.
  - Rejected: climb's new `int t = market->objId;` for one sprintf / send pair (ai_MeisterTradeAllgemein 240 ->
    163): the frame sort of a > 0x80 frame is unstable, an extra entry reorders equal densities; no source
    reason for the local.
  - Left with notes: he_SpawnFounders (n7a packed after theme 0's elig), cm_RunSceneParty (the owner loop's IV
    slots), he_RunAttack (o / chance slot weights), he_RunPopulation (-1 in ebx vs ebp), ai_MeisterTradeAllgemein
    (the unstable frame sort), d3fl_MakeDivide (the tile loop's hoisted products), he_RunBuildingSpying (0 in
    ebx in the original).

- **Deep s52 (one function: d3t_MoveObjectPath, not byte; the two levers narrowed to exact probes)**:
  - **Probe sweeps instead of guessed probes**: a generator that stores every scalar (`qN = v;`) at every statement
    boundary (13 positions x 8 values, then pairs: 92 + 814 + 814 variants) lists every reference set that gives
    the original. Here exactly one pair leaves only its own two stores (2 ins): a k read in the loop before the
    call (loop head or body start) plus a step read as the LAST statement before `k++`. The s31 pair (k head + i
    behind the call) is the same thing at 10 ins. Run the sweep before reading traces.
  - **A redefinition with an equal, already available value is a reference that emits nothing**: `step = time / n;`
    again behind its use in the loop (anywhere behind the time store, or in the else arm) is CSEd with the
    computation in the first block; the copy `step = temp` stays in the IL, counts as a reference of step (piece
    step#3, 2 refs, prio 88, takes eax by its idiv preference) and leaves no instruction (the CSE temp is homed in
    step's slot, the store back is dropped late). With the k probe: 1 ins, only the probe store. Before the use
    (`step = time / n; keys[k - 1].time = step;`, `keys[..].time = step = time / n`) it does nothing. Removed
    before allocation as before: dead accumulators (`t += step`, `time -= step`), a duplicated store, a re-read
    of the stored field, empty `__inline` helpers called with the value, `x = x`.
  - **The scratch rotation is cyclic and a body piece replaces a temp**: the step load is edx by rotation (path
    eax, lea ecx, step edx); the original's eax is the piece step#3. A value read twice in the call's block is a
    piece from its first reference to the block end.
  - **Three more candidates in the exit block flip the i / k race without any k reference**: k's three uses in
    `keys[k].time = 0` weigh 6 per distinct candidate of that block (k - i = 6 Pt - 6 Ph - 10 Pl). Reading af's
    three dwords a second time in the tail makes them memory candidates (P 4 -> 7, k#2 47 > i#2 43). No ordinary
    form found: a second struct copy is elided before allocation, int / int-bitfield flag structs change the flag
    code (63 ins).
  - **No lever (= control unless named)**: the zeroing as an `__inline` asm function (VC6 emits a call, with or
    without a label, `__forceinline` too); vector-subtract / key helpers as `__inline` functions (57..78, pointer
    temps) and a time setter helper; declaration counts 1..64 before the function; `k = 1` before the division
    (k then avoids eax / edx, i takes ecx first and k gets no register: 51, store order hunk); a block boundary
    behind the call or anywhere in the body (a real test: registers unchanged; label, `do { } while (0)`, a
    second `if (o)`: removed); `if (!obj) return 0;` behind a dereference of obj is decided and leaves nothing,
    not even a reference; `if (i < n)` in the body is not decided; a test with two identical arms (`if (k)
    return f(..); return f(..);`) merges the arms but keeps the `test`; `&keys[k++].pos` in the call (105).
  - Open: an ordinary reference of k in the loop in front of the call that emits nothing (k has no available
    expression to be redefined with), and the plain form of the step redefinition.

- **Deep s53 (one function: ai_FamilyTreeWindow 277 -> 20 ins, size equal, not byte)**:
  - **`mov bx, word ptr [..]` reading an int array element that is then kept in ebx (`add ebx,2; push ebx`) over
    a call** = a `short` local holding the element (`t = siblingX[i];` before `d2_WinOpenPic(t, ..)`, `.x = w / 2 +
    t`, `d2_WinOpenPic(t + 2, ..)`), not the element written at each use; a loop whose original form is the
    strength-reduced pointer walk keeps the plain `siblingX[i - 4]` (277 -> 145).
  - **A loop counter in another local's slot** (dcp frame: "shared in the original" i / hi): that loop had its own
    counter (`j` for the father's relatives walk), whose lifetime lets it share hi's slot (145 -> 71).
  - **`lea ebp,[eax+ebx]` at the end of a sum where ours accumulates in the target (`mov esi,eax; ..; add
    esi,ebx`)**: the sum was no named local; it was written out at each use (a CSE temp copied into the
    register) (71 -> 43).
  - **Fills of small coordinate arrays in index order** (`oddX[0]; oddX[1]; oddX[2]; ..`) moved which register
    holds the first value and stopped VC6 hoisting a load above the odd / even test (43 -> 30).
  - **Zero inits stored before each `rep stosd` in the original, ours storing computed values there**: the zero
    statements first, then the computed ones, then the memsets (`k = 0; n = 0; .. half = ..; w = ..; h = ..;
    memset(..)`); stores of equal height keep list order around the rep stosd (840 orders, 30 -> 20).
  - Left: an operand-order hunk of a reassociated sum against a CSE temp (`half - w + w / 2`) and the SIB base /
    index order of `d2_Objs[row[k].ctrl]` stores (the original keeps c1's `[d2_Objs + idx]` at two boxes, swaps at
    the others): no spelling, declaration, counter-sharing or order round moves either (function note).

- **Which late passes delete instructions in a function: the call-skip map (d3t_MoveObjectPath, not byte)**:
  `tools/c2dag/c2fuzz.py` with the unit itself as probe (`C2FUZZ_PROBE`, the function in its probe form) switches
  off each of c2.dll's 15353 calls in turn (12 minutes); the sites where the function only GAINS instructions are
  the passes that remove something from it after allocation. Here three of 424 sites that change the function:
  `0x1070afce` removes `mov reg, reg` self copies (the division result into step, the call result into the
  return value), `0x10713cd4` / `0x10713ebb` remove a reload directly behind the store of the same slot (the k
  piece's reload behind `k++`; with another register for the piece it becomes `mov edx, ecx`). So a reference
  that leaves no instruction is only: a copy between two values in one register, a reload behind its own store,
  or a variable merged with a CSE temp (the step redefinition: no copy is deleted for it, step and the temp are
  one object, homed in an unnamed slot `-8+[ebp]`). A variable defined only by a constant and `++` (k) has no
  such form; of this function's expressions only `time / n` and `path[i]` can be written twice. Also measured:
  the image has no second loop of this shape (two counters in scratch registers, both reloaded behind a call:
  scan of build/orig-asm, 1 hit); `&keys[k++].pos` in the call increments before the call and keeps k in ebx;
  a repeated `time / n` in the exit block adds two candidates there (k#2 39 against i#2 41, one short).
  **Which single decision gives the original: compile the control with each changing site off and diff against
  the target listing** (the probe form's listing without its probe lines). No site gives all of it (best 32 of 51
  lines), but `0x10709cc4` (FUN_10709bb6, the bit-set insert that puts a candidate without a register on the
  split list) gives the original's i / k registers from the unchanged source: k is then not cut, `k++` stays in
  its piece (k#2 62 against i#2 46; split: 29 against 43). So the "missing k reference" is the cut, not the
  source. `--split k` shows it: k's piece grows from the entry block through the exit block first, the exit
  block holds the d3anim_CreatePath call, so the piece keeps only ebx, runs empty in the loop body (ebx is the
  CSE temp's) and is cut there; `k++` becomes k#3 (memory). i never enters the exit block. No lever: the region
  split forced on (threshold patched: i and k in memory), a real block boundary between the call and `k++` or
  between `keys[k].time = 0` and the final call, callee prototypes (21 variants; a `char` count: `inc cl`), nine
  D3ANIMKEY layouts.
- **Register preferences (read from c2.dll: FUN_10709f0b adds `{register, count}` to the list at candidate + 0x38;
  five callers in the reference scan FUN_10707a0b, one in the choice FUN_10709937)**: a preference is only added
  by a COPY instruction (IL op 1, 0x11f, 0x1dd, 0x17f, 0x181) between a candidate and a physical register or an
  already assigned candidate, and by op 0x12 (lea) for its base candidate. So: the idiv result (`step` prefers
  eax), a shift count (ecx), a remainder (edx), a call result, `x = y` with y assigned (`i = 0` gives the zero
  constant a preference for i's register once i has one). Pieces of one variable do NOT prefer each other's
  register (d3t_MoveObjectPath: k#3 assigned first at prio 132, k#2 `prefs[ ]`). The choice takes the register
  with the lowest score (preference counts of the waiting interfering candidates, 100 x savings for one whose
  only register it is), ties eax, ecx, edx, esi, edi, ebx: with nothing assigned to ecx / edx yet, the first of
  two scratch candidates always takes ecx; two waiting candidates that each still have two registers ({ecx,
  ebx}) do not push it off. For a swap of two scratch candidates the lever is therefore their priority order,
  never a preference, unless one of them is in a copy with a fixed register.

- **An else-if arm cross-jumped whole into a sibling's copy, the original keeps it inline**: the store order of
  the OTHER arms (default arm charId before stateArg; 0x176 arm stateArg, charId, x, z) decides it, found by
  climb.py swap + move in round 2 (cm_OrderAttack byte; permuting only the inlined arm's stores did not). The pair
  merger takes the arms' else-jmps newest first, then the return jmps in source order (c2tail --tries).
- **A guard's `return 1` with its own epilogue copy where the original `jle`s into a later `return 1` copy**: the
  work in the if, the return in the else (`if (year - q->year > 1) { stores } else return 1;`); the plain guard
  `if (.. <= 1) return 1;` keeps its own copy (cm_ExDarkCorner byte).
- **Three copies of one final store + epilogue, each with its own scratch register (`mov edx,[eax+1]` /
  `mov eax,[edi+1]` / `mov ecx,[edi+1]`), the null path not folded**: one final statement after a fallback
  that reassigns the variable, `if (home == 0) { if (pick(..)) home = query(..); } br->meet = home->id;`; VC6
  duplicates the join into each predecessor. Writing the copies out (`return` per arm) folds `home == 0` into
  `[1]` (he_AllocBrawl byte; the `&&` form of the fallback differs in one branch).
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
  Same for a `pop esi` between `test eax,eax` and `je` after an if/else join (original: a pop in each exit): the
  test and its success code written in each arm, `if (t) { step(t); return 1; }`, `return 0;` after (VC6
  cross-jumps the copies back into one) (ai_TrialStep1/3/4 byte).
- **A guard's `return 0` jumping to the first reset-and-return copy where the original jumps to the last exit**:
  plain guards, each exit writing out its own reset (`if (c == 0) { reset; return 0; } .. if (bld == 0) { ..;
  reset; return 0; } .. reset; SetField; return 0;`) instead of if/else nesting that shares one reset at the
  end (he_StepWalkSlot byte).
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
- **A `je` near to a far copy where the original's is short into the next case's copy, and the copy's first load
  hoisted above the `je` in the original**: the test written failure first (`if (q == 0) { end = ai_Time; Send(-1);
  return; } end = ai_Time; AddTime; Send(3);`): both arms then begin with the same load, VC6 hoists it and
  cross-jumps the failure tail into the nearer copy (he_RunSecretRoom draft -> shape, size fixed).
- **`cmp ecx,5` before `mov eax,ecx` for a clamp (ours `mov eax,ecx; cmp eax,5`)**: the clamp tests the source
  variable: `rest -= minutes; minutes = rest > 5 ? rest : 5;` (he_RunSecretRoom).

### Search loops and their exits

- **Three `xor eax,eax` return copies where ours folds the last into the first**: the search loop's exhausted
  return tested at the loop top, `for (i = 0; ; i++) { if (i >= 768) return 0; if (found(i)) break; }`, not
  `while (!found(i)) if (++i >= 768) return 0;` (ai_TokenMayor byte).

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
- **`dec eax` before the loop, `lea esi,[eax+1]; push esi; call; mov eax,esi; add esp; cmp byte [eax],0; jne`**
  (a string read byte by byte): an index loop testing the element behind the call, `i = 0; do { rd(f, s + i);
  i++; } while (s[i - 1] != 0);`; every pointer form (`p = s + 1; rd(p); s = p; while (*s)`, `*s++`, `++s`) tests
  `mov cl,[esi]; test cl,cl` (bio_rd_s byte).
- **`dec; test; mov; jne` bottom test**: `while (n != 0) { ..; n--; i = ..; }`; `for (..; n != 0; n--)` drops the
  test. A `do { } while (n != 0)` gives the same loop but, when the loop breaks into a shared test, puts the
  exhausted exit's reload behind the break block instead of in front of it (ai_DecideOfficePartner byte with
  `while`).
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

- **A result only on some paths, the other exit returning a leftover eax (a call's, an idiv's quotient)**:
  the function is `void` - the "result" was a by-product (`s = (s + 1) % 15` leaves the quotient in eax) and
  the caller ignores eax (init_PlaceOffices: also gave the original's `mov si,ax` for the start index).
- **`xor eax,eax` hoisted above a `skill >= 5` guard, shared as its return 0 (original: the guard jumps to the
  last `return 0`'s exit and the xor stays behind the test)**: one condition `if (skill < 5 && !(v >= 252)) {..
  return 1; } return 0;` (ai_ApplyTrialStep byte).

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
- **One result variable for two pick paths**: a single failure block both paths reach (path 2 by `je`), 0 in a
  callee-saved register from the entry and the result in another: `ret = 0;` at the top, `if (p != 0) { ret = 1;
  .. }` after both paths, `return ret;` (ai_UseGoodHalveWorkerValue: padded size right, 56 -> 50).
- **An empty pick's `return K` joins another exit's tail** (`je` into the refusal block after its `add esp`, one
  copy of `mov eax,K` + epilogue): the picked work under `if (ch != 0) { .. }` and `return K;` after it
  (ai_DismissOfficeAsk 13 -> 2).
- **The closing call of an if / else falls into `he_FreeEvent`**: the arm frees in place (`he_FreeEvent(ev);
  return;`), not `break` to the shared free after the switch (he_RunSlotOrder 24 -> 0).

## Branch layout, conditions, switches
Which arm is inline, jumps into another arm, threaded and redundant tests, `&&` / `||` forms, compare operand order and jcc, `setne` against a branch, ternaries, switch dispatch and case order.

- **A decompiled `if (x != A && x == B) {..B arm..; return} ..default..` with only the scratch registers of the
  two arms swapped**: `if (x == A) .. else if (x == B) .. else ..` with the default written twice (VC6 merges the
  copies; the A arm is laid out last but rotates the registers first). The `||` inversion is = control
  (init_InitTown byte).

### How VC6 orders blocks and merges tails (c2.dll)

- **Identical statements of both arms sunk into one shared IL block before scheduling** (c2dag shows a block
  with in-edges from both arms: the flag ors, zero stores and the call of ch_ActTurn's two branches): the
  arm's own statement order decides whether they can sink; one statement placed among them (keys[0].time
  after the ors) kept the arms separate (909 -> 841 bytes merged, 915 with the original's order).
- **A failure tail with its constant scheduled among the pops (`pop edi; pop esi; pop ebp; or eax,-1; pop
  ebx`), an early return jumping to the success return's pops** (ours: `or eax,-1` before the pops, the early
  exit joining there; `0 ins / N bytes`): the success return falls into the exit label, so the walk must finish
  it first. A `for` loop's entry test sends the walk to the loop exit (the failure code behind the loop) before
  the body, and `if (r != 0) { success; return s; }` follows the skip edge first: write `i = 0; do { ..; if (r ==
  ERR) break; if (r == 0) continue; success; return s; } while (++i < 16);`. Each half alone (do-while with the
  if body, for with continue, goto forms, loop-exit tests) gave the control's code (net_Connect byte).

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
  changes the pair merger's list (ls_WriteMessageQueues, ls_ReadMessageQueues byte).
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
  - What `pair` compares (C2.DLL FUN_10712300 / FUN_10712797, read 2026-11-21): opcode, class, the low 12 flag
    bits, and both operand lists by kind, each operand's type word included; `kb` operands (kind 0xb) are
    skipped. A local's address (`lea k3`) compares by symbol (FUN_10703bb6): two block-local arrays VC6 packed
    into one slot compare EQUAL when they have the same size and UNEQUAL when they differ, though the code is the
    same. The run's size is summed up to 20 (pair bytes prints the sum, saturating at 22), plus a counter on each jmp.
  - **Two equal message exits `txt_Format(buf, id, ..); sv_MessageBox(buf, 0); return 0;` cross-jumped in ours,
    each with its own epilogue in the original, the registers equal on both sides**: each exit had its own
    block-local buffer of a different size (`char haunt[0x400]` / `char none[0x200]`, the window's `note[0x400]`
    in its button arm). VC6 packs them into one slot, so the frame and code are the original's, and the pair merge
    stops at the `lea`. Equal sizes merge again, one function-scope buffer merges, and a buffer that is not packed
    grows the frame (ai_DrohbriefAmountCB 50 -> byte; a probe `other-mb` calling another symbol showed the merge
    was the whole difference).
- **Reading a tail hunk**: `c2tail.py SRC FUNC --tries` lists what was compared and merged with source lines;
  `--il merge` shows which block falls into the exit label; `C2DAG_TRYDUMP=1` prints a pair's equal count and
  size. A tail merged in ours and not in the original (or the reverse) is almost always another block in
  front of the exit label: find the flow-graph change that lets the walk finish another return first.

- **An arm laid out behind a LATER return (not in reverse postorder of the real graph: the hit path's second
  half after the hit-reaction's `ret`, jumping back into the shared code)**: the flag was set by its own if / else
  and the work ran under a second, decided test: `if (r < acc) hit = 1; else hit = 0; if (hit) {..} else dmg = 0;`
  (the block order is taken on the graph with the decided test, which is then threaded). A flag set inside the
  arms with the work in the same arm, or the work alone under `if (hit)`, kept the RPO layout (cm_DuelShotAction
  70 -> 8 ins, then byte with `acc = acc * 0.8;` without the cast: its fstp sinks behind the next pushes).

- (Meaning) **A whole region's allocation off (a local in a register the original keeps in memory, the constant 0 in
  esi missing), flip.py pointing at one call's argument**: count the pushes before reading an `[esp+N]` argument
  load behind a cdecl call whose `add esp` is deferred: `push ecx; call io_PrintText; mov eax,[esp+0x10]` reads one
  slot lower than the same offset before the push. cm_MessageBox passed fl to d2_CentreViewWindows where the
  original passes win (byte after the fix; the other levers there: the loop's flag tests inline in a plain while,
  the click tests as one else-if chain).

- (Registers) **A long function's allocation off in one later region (a value cached in a callee-saved register
  through a tally loop where the original keeps another, slot order permuted by the counts that follow), every block
  otherwise equal**: a `continue` at the top of a loop body is a goto edge (`jncc skip; jmp L`) that changes the
  bulk split's regions; the plain form `if (s->answer != -1) { for (..) if (ids[e] == s->answer) { ..; break; } }`
  took cm_RunCouncilSession (272 blocks) 338 -> 10 ins. The rest were zero-init orders: `for (i = 0, nh = 0; ..)`
  where the original zeroes the counter first, and `tie = 0; e = 0; best = -1; win = -1; for (; e < n; e++)`.

### Which arm is inline and which is out of line

- **`if (A || .. || F) X else if (G || ..) Y` with X laid out last (near je to it, the G tests inline), the
  original falls into X and jumps to the G tests**: the X test was a value first, `int is24 = A || .. || F; if
  (is24) X else if (..) Y` (or separate `if (A) goto x;` tests): the `||` jumps go into X, so the block walk
  reaches X first and puts it last (ai_UpdateProduction byte, +20 bytes of near jumps).
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
- **A one-store else (`mov ebx,1; jmp join`) behind the final `ret`, the then-arm inline** (ours: the else inline
  behind the then-arm's `jmp`; swapping the arms, a second `if (o == 0)`, `ret = o == 0`: no lever): the then-arm
  was two `if (o != 0)` blocks, the else on the FIRST: `if (o) { flag fix } else ret = 1; if (o) { rest }` (the
  second test decided on both paths, so the else jumps straight to the join; cm_ExNewParty byte).
- (Order) **A parameter's load `mov edi,[esp+N]` at the head of a block, its first use several calls later**
  (ours: the load right before the use): the original named a member pointer at the block's top, `GVGRAVE *g =
  &ev->grave;`, the uses `g->sex` (the pointer is folded to ev + offset, the load stays where g was set)
  (ai_SeanceRitual byte).
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

- **Both arms of a ternary / if-else load the same field, VC6 hoists the load above the test (ours), the
  original loads it per arm (`mov eax,[f]; lea edi,[eax-2]; jmp; mov edi,[f]`)**: a `max(1, ..)` per arm reads the
  field twice in each arm (`if (t) e = max(1, cur - 2); else e = max(1, cur);`), so the arms no longer begin
  with one equal load (ch_GetNewPath, +8 bytes; every ternary / if form of `cur - 2 : cur` hoists).
- **An `fsqrt` (or any result) computed in the block that tests the output pointer (`test ecx,ecx; fsqrt; je`),
  the else path popping it, and a sum stored to its slot and compared from memory (`fstp [slot]` / `fcom
  [slot]`)**: the results were stored under a decided `if (ok) { if (radius) *radius = r; if (center) .. }`
  after the block that sets `r = (float)sqrt(r); ok = 1;` (d3_GetBounds' shape). The decided test leaves no
  code but keeps sqrt out of the radius arm and the loop's distance in memory: d3t_GetBoundingSphere went byte
  with a plain `float d` (a volatile union had faked the store).
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
- **A field reloaded and re-tested in an else-if although the if just tested it** (`mov edx,[ecx+0x124]; test; jne
  else` .. `else: test eax; je; mov edx,[ecx+0x124]; test; je`): the else-if has a second predecessor at CSE time,
  so the load is not available there; the earlier guard sits in the if's own condition, `if ((o = p->owner) != 0
  && !ch->tr) {..} else if (p->owner && ch->tr ..)`, not `if (o = p->owner) { if (!ch->tr) .. else if ..}`
  (the o == 0 edge is threaded to the exit afterwards). 30 spellings, casts, flip and climb did nothing
  (ai_UpdateTownMap byte).
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

- **One arm's store + return cross-jumped into the middle of another's (the target's `pop edi` after the stores,
  ours hoists it and keeps a copy)**: the jumping arm returns its failure value itself, `if (!list[1]) return -7;
  open[1] = 1; return 0;`, not `if (list[1]) { open[1] = 1; return 0; }` before a shared `return -7` (gv_OpenSize
  byte; goto forms = control). A test + store repeated in each arm of an if/else (`o = f(); if (o) x = o->id;` in
  both, `if (o == 0) {fail}` after): write it per arm, not once after the join (cm_SendChrGotoBuilding byte).

- **One arm ends `cmp; jmp L` into the other arm's `jne fail` (L), the code behind it shared, and a register of
  the shared code that no candidate spelling moves (voice in ecx, ours eax)**: the shared code was written in both
  arms and cross-jumped: `if (click) { if (obj == cont) { stop voice; return 1; } } else if (key == 0x1c) { stop
  voice; return 1; } return 0;`. Our `goto unanswered` form had the layout but one CSE candidate for both paths
  (ai_TutorialIntroAnswered byte; flip.py found no single mutation, candidate spellings and prio probes = one
  code).
- **A jump to a shared tail's second instruction, its first (`mov eax,[esp+o1]`) hoisted above the jcc**: the
  arm wrote the tail out itself (`o1->kind = 0; o1->ref = -1; o1->arg = 'noop'; return 0x35;`), no `goto noop`;
  cross-jumping merges all but the load, which the jcc's block already did (ai_DecideStammtisch byte).
- **A `push 1` hoisted above `test eax; je`, the je to the failure tail's SECOND push** (`push 1; push ebp; call
  SelectScene`; ours: the push late in the success block): the inner test had its own failure arm written out,
  `if (obj != 0) {.. f(.., 1); g(scene, 1); return 1;} else { g(scene, 1); return 0; }` before the outer `g(scene,
  1); return 0;`: both successors start with `push 1` and it is hoisted (he_CreateReaper byte).
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

- **`mov [slot],-1` among the inits and `mov esi,[slot]` at the join of an optional open (ours `or esi,-1` in an
  else arm)**: `bar = -1;` as an init, `if (flags & 0x80) bar = open(..);` (sv_MessageBoxGreen, 9 bytes).
- **Every return tail `mov edi,K; xor edx,edx; cmp edi,eax; setge dl` (one per case)**: one limit set per case
  in an if / else-if chain and one `return lim >= r;` (VC6 duplicates the short return block), not `return r <
  2;` per case (gm_BuildingRankGate byte).
- **`neg; sbb; and ecx,K; add ecx,B; add ecx,edx` against our `lea eax,[ecx+edx+B]`**: one ternary of two
  constants plus the rest, `(c ? B + K : B) + x`; `(c ? K : 0) + B + x` in any order folds into the lea
  (ai_GraveyardMessage byte).

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

- **A failure return `mov eax,reg` (the zero register) where ours has `xor eax,eax`, the returns not merged**: a
  const-0 return is always `xor eax,eax`; `mov eax,esi` is a real variable piece (`ret = 0; .. goto done; ..
  ret = 1; done: return ret;`, the copies per predecessor). When the arm's own `if (b == 0) goto done;` gives
  the right layout but a parameter outranks the one the original kept in ebx (c2trace --prio: ch 17 > req 16),
  a redundant `ret = 0;` at the start of the other arm adds values to that arm's block and ranks its
  candidate up (ai_QuarrelCallback byte; the else arm's `ret = 0` with an @original comment).

- **ebp pushed after the guard in ours, in the prologue in the original; `mov cl,[x+2]; cmp cl,6` where ours
  compares memory; the final pops in another order**: the trailing `ret = 1;` after an `if` was written in BOTH
  arms (`if (ch->kind == 6) { ..; ret = 1; } else { ret = 1; }`): two return copies in the IL (flip.py found it
  as `ret = 1` moved into the if, which changed the meaning) (ai_UseGoodLowerOfficialRelations 32 -> byte).

- **A loop entered by `jmp` past its own `xor reg,reg` (the latch target), the zero already in reg from `push
  reg; push reg` before the loop; zero stores in the loop body through that reg (ours: `push 0`, the stores of the
  tested call result eax)**: the loop-top `r = 0;` stood AFTER the first if / else of the body, not first: r then
  does not interfere with the const-0 piece over the earlier zeros, the copy coalesces into its register and the
  piece wraps around the loop (ai_AmountMenu 38 -> byte; every earlier `r = 0` / `f(r, r)` form folded).

- **A const-1 register (`mov ebp,1` before a call in ONE arm: `push ebp`, `cfg.flags = ebp`), the other arm
  `mov ebp,1` after its null test, the final `mov eax,ebp`; each arm's own failure test**: `ret = 0; if (..) { ..;
  if (bld == 0) goto done; ret = 1; } else { ..; if (bld == 0) goto done; ret = 1; } loop; done: return ret;` -
  `ret = 1` in both arms, not once at the join (there the const-1 web takes the loop's 1s too and stays in memory)
  (ai_UseGoodHalveWorkerValue 50 -> byte). The family's ret / goto done / return ret callbacks: put ret = 1 where
  each path's own 1-constants are, and look for the arm-tail sinking of identical `if (x == 0) goto done;`.

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
- **Int clamp with the upper store out of line after the epilogue** (`cmp 0xa; jg far; cmp 1; jg join; mov 1`,
  `far: mov 10; jmp join`): `n = max(min(10, n), 1);`; the 7 other operand orders and every if / else-if /
  ternary spelling differ (ai_CommandPlague byte, one 17-variant round).
- **Against zero**: `max(0, y - n)` `sub; sets; dec; and`, `max(y - n, 0)` `test; setle`; `max(0, x0)` `setl`.
- **`mov byte [g],2; jb L; mov [g],al; L: mov eax,[g]; and eax,0xff`**: `g = max(2, g);` (or `g < 2 ? 2 : g`),
  read as `ai_NumPlayersW & 0xff`. No lever: `max(g, 2)`, `if` (main_PlayerCountDialog byte).
- **Macros on globals** give compare-then-conditional-store; `abs(0x7f) / 2` is not folded.

### Switch dispatch, case order, empty cases

- **A direct table (`and ecx,0xff; add ecx,-3; jmp [ecx*4+T]`, one entry per value) where ours goes through a byte
  index table (`mov dl,[ecx+X]; jmp [edx*4+T]`)**: VC6 picks the index table by the count of distinct case
  statements, before identical `return K` blocks are merged: write one `return K;` per case label, not grouped
  labels (cm_MessageSize 0x4c3180: 42 groups over 103 values -> one return per label, byte). A dead `cmp dl,K`
  inside a case body plus the switched byte kept in dl: an `if (m->type == K)` in that case whose arms compile
  alike, testing the switched expression itself (a `t = m->type` local spills it).

- **A `xor eax,eax; ret` block laid out between two case bodies**: the case was written there (`case 1: ..; break;
  case 6: case 7: return 0; case 2: ..`); the table entries alone do not show it (gm_FirstBuilding).
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
- **A shared mismatch block out of line after the returns** (every criterion's `jne` to one `xor cl,cl; jmp`):
  `if (field == want) m = 1; else goto no;` per criterion; `if (field != want) goto no; m = 1;` puts the block
  inline after the last test (gm_BuildNext 8 -> 2, size right).
  The same code without goto: one `&&` chain whose tests mark the match, `(t == want && (m = 1))`, else `m = 0`
  (gm_BuildNext s9).
- **Pair merging takes the first earlier identical `jmp` to the same label**: case 2's closing `stage++` merged into
  case 1's copy (near jumps, +8 bytes) where the original shares case 3's; `break` at case 1's end (the other cases
  `return`) gives its jmp another label (he_RunPlunder, flagged @original).
- **`cmp i,0xffff` after a search loop and a second inline copy of its failure block** (both copies reached by
  fall-through, below the merge size): `n = 0xffff; for (..) if (..) { n = i; break; } if (n == 0xffff) ..`: the
  test is decided on the exhausted path (main_ReplaceFigure).

## Loops and induction variables
Rotation, test position, countdown, induction variables and strength reduction, pointer walk against index loop, bounds, search loops.

### Decompiled pointer walks that were index loops

- **A byte-offset do-while over a global record pointer (`off += 0x43; while (off < 0x14f218)`) with `[off+base]`
  operands where the original has `[base+off]`**: `for (i = 0; i < N; i++) { o = &tab[i]; ..}`; the counting
  loop before it (`mov ecx,N; .. dec ecx; jne`) was an index loop too. No spelling of the offset form moved the
  operand order (ls_WriteObjects byte; the same lever: ls_WriteRelations, ls_ReadRelations, ls_RefsToIds,
  ls_RelinkScene, gm_ClearObjects, init_SceneNameToPrototype - all byte from a byte-offset loop).

- **`lea eax,[ecx-8]; cmp eax,end` at a row loop's end (ours), the original `cmp ecx,end+8`**: the row pointer
  walk with an int-cast bound was `for (i = 0; i < 0x20; i++) { row = &tab[i]; ... }`: VC6 compares its field
  induction pointer against the shifted end (ai_EquipRowsInit byte).

- **`lea edx,buf` behind the count test and the pushes (ours before the `jle`)**: a hand-advanced byte pointer
  `for (d = buf; i > 0; i--) { memcpy(d, p, 0x40); d += 0x40; }` was the index loop over the element type
  `RTPOINT buf[8]; for (i = 0; i < n; i++) buf[i] = *pts;` (cm_SendSetRoute byte).
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

- **A function-scope result count kept in a register only inside a row loop (`xor ebp,ebp` before the loop, `mov
  [n],ebp` at its exit), ours the count in its slot all through**: the loop counts in its own local and the result
  takes it after the loop (`for (j = 0, k = 0; ..; j++) { .. rows[k] .. k++; } n = k;`); the loop's `j = 0` and `k = 0`
  in the for-init, j first (ai_FillWeddingTab 84 ins -> byte; the per-event flag `mine = 0` stood before the
  event's date test).
- **Two word-copy loops that walk two pointers with a down counter (`lea ecx,dst; lea eax,src; mov edx,n; mov
  edi,[eax]; add eax,4; mov [ecx],edi; add ecx,4; dec edx`), ours one pointer plus the difference
  (`sub ecx,eax; [edx+eax]`)**: index loops over the fields, `for (k..) ev->ids[k] = msg->ids[k];` and for a
  run of fields `ev->w[k] = (&msg->first)[k];` (a named `int *s` gives the difference form again)
  (he_CreateEvent byte, s12).
- **A search loop top-tested on a found flag with the bound tested only at the bottom (`test edi,edi; jne`
  at the top, `add k; cmp k,lim; jl top`)**: `for (k = 0; k < lim; k += n) { if (ok) break; .. }`;
  `k = 0; while (!ok) { ..; k += n; if (k >= lim) break; }` is the rotated variant (init_OpenTown, s12).
- **The tick x strength-reduced (`lea esi,[edi+edi*2+9]; lea esi,[esi+esi*4]`, `neg eax; mov [dead arg slot],eax`) while
  the label x of the same round keeps `imul`**: `x = 0x2d + round * scale;` at the top of each round, not a running
  `x -= scale` (ai_DrawStatisticsCharts byte; an earlier session called the recomputed form worse because the init
  order hunk was left: fix both). `x += 0x20` as the for loop's second increment (`i++, x += 0x20`) and `for (i = 0,
  n = 0; ..)` against `n = 0; for (i = 0; ..)` are order levers (ai_PokerSeatPanel, ai_AccusationWindow byte).
- **A counter argument as `lea eax,[edi+1]; ... mov edi,eax; push eax`, ours `inc edi; push edi`**: `f(.., n++ +
  1, ..)`; `++n`, `n = n + 1`, `n++;` before or after the call, `n + 1` then `n++` are all one code
  (ai_DungeonPlanWindow byte).

- **A chain count loop with `inc edx; mov [esp],edx` scheduled after the `mov eax,[eax+next]`** (the counter in
  memory, its address goes to a write): the step stood in the body before the count: `s = head; while (s != 0) {
  s = s->next; n++; }` (ai_SaveSettings byte; `for (..; s = s->next) n++;` puts the inc first).
- **The loop pointer's `add esi,0x14` emitted before the body's last running sum (`dec ebx; setns; lea
  edi,[edi+ecx+0x21]`)**: the sum stood in the for increment behind `i++`: `for (i = 0; i < 8; i++, y += step +
  (--rest >= 0) + 0x21)` (ai_SpecialAmountWindow; a `while` with `i++;` before the sum is the same code). No lever:
  operand order or splitting the sum.
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
- **Inside a loop the loop body is not fresh enough**: the escape survives the back edge, so `D3VECTOR rot`
  declared at the top of the loop body kept `fstp [rot.y]` before the next call's pointer loads (the same code
  as function scope); a `{ D3VECTOR rot; rot.x = ..; ..; d3_SetObjectRot(.., &rot); }` block around just the
  statements that use it matched (cm_PlaceFighters byte). blockscope.py places a local at the top of the outermost
  block, so try the innermost enclosing block by hand.
- **A computed argument as its own statement before the call** (`r = (int)(500.0f / s); f(c, x, y, r, &x, &y);`)
  puts `call __ftol` before the address pushes; the original computed it in the argument list and the ftol comes
  after the `lea`/`push` of the later arguments (cm_CarryOutOrder byte).

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
- **x87 keep vs spill**: its own allocator (Floating point, "How c2.dll allocates the x87 stack"); ai_FireCannon's
  extra 4-byte temp was a CSE temp that did not nest.

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

- **A window pump's two flags in swapped callee-saved registers, every ordinary mutation leaving the pass-3
  margin unchanged (ai_SpecialAmountListWindow: win -67 over viewing -75 for ebx; climb rounds, blockscope,
  types all at exactly 8)**: the trailing `redraw = 1;` after an if / else-if chain was written in each arm
  (`if (..) { ..; redraw = 1; } else if (..) { sv_PopRequest = 1; chosen = c; redraw = 1; } else { ..; redraw
  = 1; }` plus `else redraw = 1;` for the no-hit path, and both arms of the `id != 0xffff` test): VC6 merges the
  copies into one `mov esi,1` tail, but each arm's block gains the redraw and 1 values, raising P where viewing
  is referenced (93 -> byte, with redraw = 0 after the row calls and the store order inside the arm, which a
  6-order round settled). Measure such races with the margin (prio difference from c2trace per variant), not
  the instruction count: a round that moves the margin 8 -> 6 -> 0 shows no code change until it flips.

- **Two if / else arms that start with the same call, ours hoisting the first argument loads / push above the
  jcc, the original's arms equal but with ecx / edx swapped in the second**: the hoist is post-allocation (equal
  registers in both arms); the original's second arm rotated because the statement after the if was written in
  both arms (`ai_CentredText(text, x, row->y + 0xf, ..)` at the end of each arm, as the byte twin
  ai_ShowOfficeRows has it): VC6 cross-jumps the copies (the shared `lea / push / call` tail, one `add esp,0x44`).
  Then a named copy `ch = row->rec` written out (`row->rec->kind` at each use) settled the rest (ai_ShowAmountRows
  95 -> byte; the first-line spellings, 36 of them, never reached it).

- **`and eax,0xff; .. add eax,ecx; push eax` (the sum in the tested byte's register) where ours has `add ecx,eax;
  push ecx` (the sum in the ternary's), and the next arm's ternary one scratch register on**: the sex-dependent text
  base was a local set by if / else (`if (chr->sex) base = 0x18f; else base = 0x130; io_PrintText(.., chr->jobId +
  base);`): two definitions make it a variable (VC6 still if-converts it to `neg / sbb / and / add`), and a
  variable's register is not reused for the sum. Every ternary spelling, cast, order, `t +=` form or single-use
  local folds to the ternary temp (ai_CharInfoPanel, proven byte; ord_WageReportOrder's `base` in ebx is the same
  rule). Also there: two sv_ButtonRow button pairs were scalars plus a block-local `int btns[2]` per arm (the
  scalar shares its arm's array slot; the original keeps the scalar in ebp through the click loop).

### A constant or zero kept in a register

- **A zero register (`xor ebp,ebp; cmp eax,ebp; push ebp`) where the original tests `test eax,eax` and keeps a
  parameter in that register (`push ebx` of the parameter known 0 in the arm of its `== 0` test)**: a search
  loop that fills a row inside its hit arms was a plain search into a local (`c = 0; for (..) { if (..) { c =
  gm_FindChar(..); break; } }`) followed by one `if (c != 0) { rows[0].. ; n = 1; }`: VC6 decides that test per
  path (no code), the counter is no longer live unreferenced through the search, and the parameter outranks the
  zero (ai_FillQuarrelTab 65 ins -> byte; the count path kept its in-arm form).
- **An address constant in a scratch register for an entry test and a loop guard (`mov ecx,&T[47]; cmp eax,ecx;
  jae; add; cmp eax,ecx; ja`), the loop's bottom test immediate**: the bound through a count local, `n = 48; ..
  after >= &T[n - 1] .. p <= &T[n - 1]`: the address is a CSE temp (refcost > 0) before n folds. A literal
  `&T[47]` (with or without a cached `last` local, `T + 47`, `i = 47; &T[i]`) is a plain constant with savings
  -1 and stays immediate; extra uses do not help (ai_FindMissionType byte).
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

- **A named sum (`role = f(..) + 0x1125`) takes a callee-saved register and pushes two record pointers into
  slots; the original keeps the sum in a slot and the pointers in esi / ebp**: the constant added at each use
  (`role = f(..); g(role + 0x1125); h(role + 0x1125)`): the CSE temp is no candidate like the named local
  (cm_CheckFight 75 -> 26, s12).
- **One of two pointers kept in its slot where the original holds it (pointer g against type record bt)**:
  the store-and-return tail written in each arm of a final if/else chain (`bld->yard = g; return g;` per
  level arm) adds g's references in the arms it otherwise lives through unreferenced (gv_AllocGraveyard, s12).
- **A flag accumulator whose first value is computed in eax and then `mov edi,eax` (ours straight into edi)**:
  `any = 0; any |= c1; any |= c2; ..` (ai_PassesPartnerFilter byte, s12).
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
- **`add eax,ecx; inc eax` for `&tbl[i].name` (name at +1) where ours has one `lea edx,[eax+ecx+1]`**: the record
  pointer was a named local (`g = &ai_GoodsT[r->id]; .. _strupr(g->name)`), as in the sibling arm (cm_ExChrUseGates
  byte). Same family: `(c ? 0x27b : 0x253) + off` for `add 0x253; add off` (ai_WorkerDismissalWindow byte).
- **Two index locals with swapped scratch registers in a loop with a call (d3t_MoveObjectPath, open)**: both are
  split around the call; the pass-2 pieces race (i#2 43 vs k#2 29, `c2trace --seq`: ASSIGN order and avail); a
  probe (`extern int p1, p2; p1 = k; p2 = k + 7;` in the tail) that adds two references of k proves the order
  decides it. Use such a probe to confirm a race before searching for the natural reference.
  A probe is only clean when it adds no scratch temporary: `p2 = k + 7` is a `lea` into the next rotation
  register, so everything after it in code order is one step off and the probe variant cannot reach the
  original's bytes; a plain store of the value (`p1 = k;` in the block that needs the reference) adds none. With
  the clean probe a second difference showed (the step load behind the call in edx, original eax): one scratch
  step is missing between the call's `lea` and that load; a global copy `p3 = q1;` in front of it plus the clean
  probe gives the original apart from the probe instructions. Measure each lever with its own minimal probe and
  check that both together leave only the probe lines (`vdiff.py`) before the search for the natural forms.
- **Dead reads do not count for the allocator**: a read of a local that VC6 deletes (a never-read local `last =
  k;`, an expression statement `k + 1;`, an empty `if (k > 63) ;`, a dead `k++` behind the last use, a first
  store overwritten by the next statement) is gone before the priority pass: no reference, no change of P, no
  scratch rotation step, the same code in the loop head, the latch, the tail and the init block
  (d3t_MoveObjectPath, 14 forms = control). Only the frame's reference count sees a dead STORE to a used
  local ("Reference count of a local"). A leftover dead read is never the missing reference of a priority race.
- **Reloads of pieces split around a call are nodes at the END of the call's block** (`c2dag.py`: after the
  block's last statement, in ascending piece id), whatever stands behind the call; the scheduler hoists them
  (load bonus), so they appear right behind the call. A scratch temp behind the call is chosen before them in
  list order and may take a register a piece is reloaded into later (d3t_MoveObjectPath).
- **d3t_MoveObjectPath s31: a scratch step behind a call can be a piece's EXTENT, not a temp.** A reference of a
  split value in the call's block behind the call makes its piece start there (reload right behind the call)
  instead of at the next block: that register is busy at the following temps, so the rotation skips it. Here a
  probe `p2 = i;` before the time store put i's reload in front of the step load (edx busy -> step in eax, the
  tail one register on), which is the s8 "missing scratch step"; with the k probe in the loop head (k#2 51 >
  i#2 47) both probes leave only their own stores (vdiff 10 ins). A reference at the block END (behind k++)
  gives the extent but swaps the two registers (9 ins). Test for this before hunting a missing temporary: a
  probe store of each value live over the call, at the start and at the end of the call's block.
  Also measured there: `x = x`, `x += 0`, `x |= 0` and 10 more no-op forms are removed before allocation like
  dead reads (no priority, no extent); the time-store position among six subtractions is no lever (the
  scheduler places it); block scopes of the loop counters are no lever.

### Pointer locals and addresses

- **`add eax,edx` then every field read as `[eax-0xc0+off]` (disp32), ours `lea eax,[edx+eax-0xc0]` + disp8**: the
  last record read through a pointer to the END: `fr = &d->frames[d->nframes]; .. fr[-1].turn.y` (ch_ActTurn,
  +4 bytes); `&frames[nframes - 1]`, `frames + n - 1` and the index written out at each use do not.
- **`add eax,ecx; mov al,[eax]` / `cmp byte ptr [eax],K` (ours `[eax+ecx]`) on a table record's byte tested
  twice**: a record pointer local tested at each use (`g = GM_GOOD(o->type); if (g->type == 0x17 ..`), not a
  `char` copy of the field (gm_BuildingGoodsList17, ai_TradeContactChecks byte).
- **The field of a copied struct in a callee-saved register, the copy's store of it missing**: compute in the
  field itself (`clk = ai_Time; if (..) clk.year = ..; clk.year -= ..;`), not in an int local stored into it
  afterwards (gm_BirthDate byte).
- **`lea ebx,[edi-0x1a0]` (a row pointer) after the row's tests, ours before them**: test the element in place
  and set the pointer inside the if: `if (d2_Views[k].active && d2_Views[k].enabled) { v = &d2_Views[k]; ..`
  (sv_DisableVisibleWidgets).
- **A dead `lea eax,[edi+eax*4]` right after a load `[edi+eax*4+0x22f]` (eax overwritten by the next call)**: a
  named record pointer `t = &ai_BTypesT[b->type]; f(.., t->title, ..)`: VC6 folds t into the load and still
  materialises it (he_AllocGeselle; the plain field access or a typed cast leaves no lea).

- **A dead `lea ecx,[eax*4+T]` beside a load `[eax*4+T+4]`, the register then reused**: the record pointer local
  read in the loop test (`ch = &ai_CharsT[c]; for (..) if (m[i] == ch->netId)`), not the id read once into a
  local (sv_RemoveGroupMember byte, as its sibling sv_AddGroupMember). cm_FightRole: `t = &ai_BTypesT[*b]; if
  (t->kind == ..)` at each test, not `k = *r`.
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
  Same with the pointer already named but the test written through the array (`p = &G5_CHARS[i]; if
  (G5_CHARS[i].act ..)`, one `shl eax,2` temp): every field through `p` (ai_ExecuteAction byte).
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
- **A failure path one rotation step behind after a success arm that ends in `..; gm_AddTime(..); return;`**
  (a probe read placed after the success tail's last call fixes it, every if / else / return / break / cast /
  loop form is = control, flip only finds the duplicated call): the success was a flag and the shared tail
  came after both: `ok = 0; if (roll && !has) { ok = 1; ..news.. } if (!ok) { ..failure..; return; }
  e->stage++; e->end = ai_Time; gm_AddTime(..); return;`. The decided `if (!ok)` keeps a block structure that
  costs one scratch step, and VC6 threads the flag (he_RunSabotage 98 -> byte).
- **A `stage = K; return;` exit pair-merged into the wrong one of two equal copies** (ours jumps into an earlier
  case's copy, the original into a later one): the earlier case's tail ended with `break`, not `return`
  (he_RunSabotage stage 8; c2tail shows the pair merges with kept / deleted lines).
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
- **A message's `lea eax,[m]` push one rotation step "behind" (original eax, ours edx) after two field stores
  from a global and a parameter, frame equal**: the two fields were a small local struct copied whole (`struct
  G17LOBBYSTATE s; s.id = ..; s.value = v; m.lobby.state = s;`): VC6 splits the 8-byte struct into two
  candidates that do not advance the rotation and adds no slot. Give an anonymous union member a tag to name it
  (cm_LobbySetReady byte; the twin cm_LobbySendChat had the same form with a 0x48-byte struct).
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

- **`mov eax,[p]; test eax,eax; mov esi,eax` before a list walk**: the walk has its own pointer, `for (q = p; q;
  q = q->next)`, not `for (; p; p = p->next)`; an init store `mov [slot],0` as an immediate where ours stores the
  zero register: that local was set before the one in the register (`ret = 0; n = 0;`) (rt_ChooseRouteGood byte).
  A counter's `xor edx,edx` hoisted above an if/else that precedes the loop: its `k = 0` stood before the if, which
  moves the arm's scratch registers (snd_UpdateMusic 19 -> 9).

- **A parameter compared from memory at its first use in the loop body, loaded into a register at the second**
  (ours: in the register from the first use; nested ifs / `continue` / split conditions: the control's code): the
  row pointer was set in a statement before the inner `for`, `e = &h->entries[h->size * z + 1]; for (x = 1; x <
  n; x++, e++)`, not in the `for`'s init with a comma (another split point for the parameter's live range)
  (d3sm_Replace byte).

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
- **A stepped field in a callee-saved register** (`mov edi,[esi+N]; inc edi; mov eax,edi; mov [esi+N],edi`, edi
  pushed at the top): `h->entry++;` and the reads as `tab[h->entry]`, no `i = h->entry + 1` local (he_RunHint
  byte). The local form keeps it in eax.
- **Zero-initialised locals in callee-saved registers from the prologue** (pushes before the first call, `xor
  edi,edi` ..): they are read on a path that skips their assignments: the click test after the block that registers
  the plates, not inside it (ai_CityWatchListener; the init order is then a permute round).
- **n and max swapped between two callee-saved registers**: the one served first takes ebx; c2trace --prio shows
  which block terms rank them (ai_FillLoversTab: memset's `max * 56` counts max twice).

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
  `xor eax,eax` before the pops). Declaration order, names, goto vs flat tails were no lever. The same tail
  written out at each `ai_ChooseSubAction` site (no `goto done`, no `if (r == 0) return 0;`) is the whole lever when
  the only hunk is `pop edi; xor eax,eax` against the original's `xor eax,eax; pop edi` in the final `return 0`
  (ai_DecidePamphlets, ai_DecideDenounce, ai_DecideUseEducationObject byte).

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

- **Frame 8 bigger in ours with a filled pointer array and no hole**: the array is smaller (`AIBLDX *list[8]`,
  not [10]) (ai_DecideBuyIndulgence).
- **Frame 4 bigger in the original with an untouched hole above a filled `int arg[4]`**: the callee copies
  one word more (cm_SendScriptMessage copies 0x14): `int arg[5]`, the last word unset (he_ClearInnDarkCorner).
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

- **Equal-density ties after the K&R sort**: simulate it from c2frame's B rows (the 12-line sort in the note above:
  middle element to the left end, Lomuto on `dens > pivot`, recurse both halves) and search count changes x tie
  orders for the original's A order; the original's per-slot reference counts come from its listing (count each
  `[esp+X]` operand with the push depth tracked; late passes remove a few, so compare ours the same way). ch_GetNewPath:
  the listing counts showed i one reference lower = the escaped-counter store above.
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

- **A pointer local's 0 initializer stored right after the prologue's parameter load into its own slot (`push ecx`
  frame), ours in the dead parameter's slot after the first call**: the declaration with the initializer stands
  before the one that calls (`short *grp = 0; AICHAR *per = gm_FindChar(..);`); the guards as one if with the free
  in the else put the shared failure call at the end (he_AllocGraveTraining byte).
- **Out-arguments of a lookup at [esp+0x24]/[esp+0x28] (the parameters' slots), ours `&param` reused as the
  outputs**: they were own locals VC6 packed into the dead parameter slots; with the parameters no longer
  address-taken they load straight into their registers (ai_CartAtBuilding `gm_FindById(&hitObj, &hitBld, ..)`,
  195 -> 191 bytes, byte).
- **A loop counter's `fild` temp in the second dead parameter slot, ours in the first (with the clamped value
  `kind`)**: the loop-invariant factor written inside the loop (`a = (float)i * ((float)kind * 0.25f + 1.0f);`):
  VC6 hoists it, but kind's lifetime then spans the loop and i cannot share its slot; a `t` computed before the
  loop lets them share (gv_CannonKind byte; `i = 1;` before `t = ..` is the same code).
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

- **A frame one slot bigger with an out-parameter local (`vfs_FindFile(.., &dir)`) in its own slot**: the
  local declared in the blocks that use it packs with the scalars (he_RunHarvest frame 0x254 -> 0x250, s12).
- **A loop counter stored to its slot after the loop although nothing reads it (ours), frame one slot bigger**:
  the counter's name was reused as a call's out-argument further down (`f(.., &cz, &i, ..)`), which escapes it
  for the whole function. The original had its own block-local out-arguments (`int fz;` beside `int sx;` in the
  `if (res & 1)` block), which share the counter's slot; a function result reused through a scan variable (tz) got
  its own block local as well (ch_GetNewPath: frame 0x9bc -> 0x9b8, 120 -> 102 ins).
- **Out-argument locals of the last call in their own slots, the original packs them into dead slots (a parameter's,
  a counter's; frame 8 smaller)**: declare them in a block around that call (`{ int flags = 0xc00; unsigned short
  id; if (f(.., &flags, .., &id)) return ..; }`): function-scope address-taken locals live through the whole
  function and never share (ai_PickLeastLikedRival; ai_GiftWindow: `flags` inside its `if` shares the min temp).
- **A call result stored into a slot nobody reads (`mov [esp+N],eax` after the call, ours keeps it in eax)**: the
  slot is a local whose address goes to a call in another arm, used for this value too: one `AIOBJ *o;` for
  `o = cm_GetSlotValue(..); r->objId = o->objId;` and `gm_FindById(0, (int *)&o, ..)` instead of a `g` plus a
  `bo[1]` (he_RunResearch byte; the store also schedules the next push first).
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
  An address-taken int sent once at the function's end (`&delta` to cm_AddFieldValues) sharing a block array's
  slot (`tabs[2]` in an `if`): its own `{ int delta = ..; ..send.. }` block at the end; blockscope.py does not
  offer it (ai_MasterLetterSettings frame 0x80 -> 0x7c, byte).
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
- **An address-taken local (`gm_FindById(.., &o, ..)`) packed with function-scope locals in the original, ours in its
  own slot**: a function-scope `&o` conflicts with every local (c2frame `X o: ..all..`); declared in the block that
  uses it (`case 1: { AIOBJ *o; .. }`) it shares the slot of locals of other cases (he_RunTransport 36 -> 8 ins).

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

- **A `volatile` view of a global moves the whole block's schedule**: menu.h declares `gypsyFilter` `const
  volatile unsigned __int64`; its two loads behind an AMOUNTCFG memset keep the memset chain one height step
  above the zero register's `xor` and an unrelated struct-copy store, which then sink behind `mov ecx,0x14 /
  xor eax,eax / lea edi`. Read through the plain view `gypsyFilterValue` (`*(unsigned __int64 *)&gypsyFilter`)
  the original's order comes back (ai_FeastWindow byte; flip.py hit the deleted ai_AmountMenu(&cfg) call, the
  next probe was the volatile). Suspect it in every gypsy window whose only hunk is the filter loads against
  the memset (ai_SermonWindow's note describes exactly that).
- **Zeros as declaration initializers do not join the zero candidate**: `int done = 0; AICHARREC *guests[4] =
  {0, 0, 0, 0};` (not `{0}`: that is a memset) took ai_FeastWindow's 0 piece below a pointer parameter's
  priority, so 0 got ebp and the parameter edi as in the original (58 -> 4 ins).

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
  The 81 counts every node of the block in list order, pseudo nodes included: a user label (`l: ;`, also a `goto` target whose jump VC6 removed) is one node and does NOT end the region; labels of decided `if`s, empty loops, `do { } while (0)`, inline helpers and block scopes leave none. A hunk in the last statements of a long loop body where the original loads a value later / increments later = the original's cut falls N nodes earlier; read the cut with `c2dag.py` (the block holding the rest starts at the cut) and test N labels as a probe. d3t_CreateSmoke needed exactly 3 (kept with an @original comment after every ordinary spelling, inline helper, block scope and type round left the code unchanged).
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

- **A call before an if with no `add esp` of its own, its cleanup added to each arm's last one (`add esp,0x28` /
  `0xc`), ours `add esp,8` before the test**: the call was written at the head of both arms (VC6 hoists the equal
  code but the merged adjusts stay per arm): `if (partner) { d2_SetForm(win, 1); ..} else { d2_SetForm(win, 1);
  ..}` (ai_PrivilegesPanel byte).
- **An `fstp [temp]` / `fimul [local]` before the argument `lea` / the call's `add esp` in ours, after it in the
  original**: a redundant `(float)( .. )` around the float sub-product (`(float)(gm_GetWorth(..) * 0.05f)` inside
  `min`, `(float)((200.0f - liking) * 0.01f) * guilt * worth`): the cast node lowers the store's priority
  (ai_GiftWindow, ai_DecideBuyIndulgence; other spellings, operand orders, `/ 100.0f`: = control).
- **A global store at the head of a join block, before the next call's argument load** (original `mov
  [he_ActiveSlot],esi; mov eax,[esi+8]; push 1; push eax`, ours sinks the store to just before the call): the
  store was written in BOTH arms of the preceding if (`{ ..; g = p; } else g = p;`) and the next call reads the
  value back through the global (`f(g->win, 1)`, not `f(slots[i].win, 1)`); either change alone does nothing or
  rebases the loop pointer (he_PickActiveEvent).

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

- **Loads of a local held behind stores through a pointer** (`mov eax,[pos.x]; mov [edi+0x90],eax; mov ecx,..` where the original loads all three, then stores): the local's address went to a call at function scope; declare it in the block that fills it (`if (..) { D3VECTOR4 pos; bio_rd_v3(f, &pos); if (map) map->orgx = pos.x; .. }`): the block local does not alias the pointer and the loads hoist (d3io_ReadFloor byte; no lever: int casts, float locals, struct cast, element loop).
- **Loads after a struct copy in the original, before it in ours, one call takes `&local`** (the callee
  summary): for a function defined above in the unit VC6 keeps a summary and uses it at every later call; an
  external callee, a prototype only, or a call through a function pointer variable (even one it resolves to a
  direct call) has none and counts as doing everything. The summary is taken from the text before any
  optimization: code under `if (0)`, a store overwritten at once and a call later removed all count. Its parts:
  - *Escape, per pointer parameter*: the parameter escapes when the callee passes it to an unknown function
    (`free(p)`), passes it to a known function whose parameter escapes (transitive), stores it to a global or
    into memory (`*(void **)&m.f = p`, even dead), returns it, or converts it to an integer that is stored /
    passed / returned (`(int)p`). It does not escape by a read through it, memcpy / a struct copy from it, a
    write through it, a compare, pointer arithmetic, a pointer local (`q = p`, dead or used), `const` /
    `volatile` / another pointer type, a K&R definition, a cast folded away (`(void *)(int)p`), or passing it to
    a known function that keeps it. A caller's local whose address only goes to non-escaping parameters is not
    escaped: stores to it move freely across pointer loads (the hoisted loads), and when the parameter is not
    used at all the caller's fill of the local is deleted.
  - *An integer parameter*: `f(int a, int p)` called with `(int)&local` escapes the local at the call (the
    conversion, whatever the callee does with it) and leaves the callee's bytes (`memcpy(d, (void *)p, n)`).
    This is the ordinary source for "the callee is known but the caller's local is escaped": cm_SendCutEntry /
    cm_SendCutEntryUpdate (cm_FightFrame 2 ins -> byte, cm_RunCutOrders 955 -> 953, the other callers unchanged).
  - *Writes*: which named globals the callee stores to, whether it stores through a pointer, whether it calls
    an unknown function; transitive through known callees. Across the call the caller keeps a global in a
    register unless the callee writes that global, stores through a pointer or calls out; it keeps a value loaded
    through a pointer (`x->scene->obj`) only when the callee writes no global at all. Stores to the callee's own
    locals / arrays / parameters (also through `*(float *)&d`) do not count; a store through a pointer local
    does (`float *p = d; p[0] = ..`).
  - Not part of it: `static`, the distance between the definitions, the unit's declaration count.
  Probe: `python tools/c2dag/unitprobe.py UNIT --unknown CALLER` (each same-unit callee as an external copy) and
  `unitprobe.py UNIT PATCHFILE` (a dead `if (0) g = 0;` / `if (0) free(p);` in the callee) show which knowledge
  the original's callers saw, without touching the tree. Before calling a hunk a TU boundary, do both: a caller
  that wants a callee "unknown" usually wants one part of its summary changed (cm_FightFrame; the split trials at
  0x4cd9d0 / 0x4d3ac0 / 0x4d6970 all dropped byte functions). Open: ai_LedSelectBits and cm_CharObjDistXZ count
  as writing a global in the original (cm_UpdatePointer, cm_RunCutOrders) with nothing in their text that does
  (cm.c's `Open: summary` line).
- **A field load through a returned pointer behind an inline `strcpy` store into a buffer** (`mov [name],edx; mov
  cl,[eax]`, the original loads `b->type` first): the buffer is an address-taken function-scope array; declared
  in a block that opens before the strcpy (`{ char name[MAX_PATH]; strcpy(name, "gb_"); strcat(name, ..); ..
  loop .. }`), the load moves free. Frame unchanged (cm_ExCreateGeb byte).
- **A pointer loaded before a test, its field loaded only behind it** (`mov edi,[ecx+0x10]; je; mov edi,[edi+0x1e4]`,
  ours both loads before the `je`): a named pointer local assigned before the `if` and read twice inside
  (`ops = surf->info.mesh; if (..) { if ((int)ops->nsets <= 4) v = id % ops->nsets; ..}`). A count local `n =
  ops->nsets` before the if puts both loads in front; with one read inside, VC6 sinks the whole chain into the
  branch (gm_SelectModelVariant byte).

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

- **A field reloaded (`mov ecx,[esi+0x14]`) where ours forwards the entry local (`push edi`), no call on the
  path**: something joins into that path before the use, at CSE time, and vanishes later. `ch_RemoveAction(a); a
  = 0;` at the end of the first-call block plus `if (!a) return;` after it: the test is decided on both paths
  (a dereferenced / a = 0) only after CSE, so the join cuts the forwarding and then disappears. flip.py
  `--want` on the reload found it as "delete the block's `return`" (ch_ActDroppingObject byte). Flags (`first =
  1`) are threaded before CSE and give the forwarding back.
- **Two globals reloaded after a call in the other order** (`mov ecx,[hour]; mov esi,[rain]` against the
  original's `mov esi,[rain]; mov ecx,[hour]`, both register candidates killed by the call, equal scheduler
  priority): the reload tuples follow the globals' memory-symbol numbers (`m:#N` in c2dag), given in the order
  each global is first NAMED in the function's text (an assignment's left side before its right side; extern
  declarations and the unit's order do not count). The original named the first-reloaded global earlier: in
  sv_UpdateWeather only an unused `struct D3TRAIN *rain = (struct D3TRAIN *)d3t_Rain;` above
  `sv_WeatherHour = ...` did it (a dead mention `if (0) d3t_Rain = 0;` probes the same; after the hour statement
  it does nothing). flip.py did not find it (no mutation adds a mention); probe with a dead mention first.

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

- **Constant stores emitted as fillers during a `lea` chain's stall (`lea*5; type; flags; X; lea*9`), the wrong
  filler third**: equal-priority stores fill the stall slots in list order, while a store's list position also
  decides its value's scratch register (the rotation). Keep the loaded value's statement where its register is
  right and move the constant stores BEHIND it: AIMSG fill `type, flags, to, fromId, time, arg[1], arg[2],
  arg[3], arg[4], arg[5]` (ascending args) matched, where the sibling's order `.., arg[3], arg[4], to, .., arg[2]`
  and any order with arg[2] above `time` did not (ai_CabinetIntrigueWindow byte; 2000 sampled orders reached 4).
- **The last load of a 3-int copy scheduled behind the `lea` of the next call's buffer**: the element loop
  `for (i = 0; i < 3; i++) m.rot[i] = rot[i];` (or a 12-byte struct copy), not three field stores
  (cm_RequestCreateGeb). No lever: the 720 orders of the fill.
- **A store held behind independent stores of the same record** (`mov esi,[esi+4]` hoisted, then `thing`,
  `used`, `host`, `members[0]`): a load through a pointer is ordered after earlier stores to an escaped local, and
  the other order holds the stores at the load's height; the value read into a named local first, stored twice
  after the other fields (`id = p->netId; m.thing = -1; m.used = 1; m.host = id; m.members[0] = id;`;
  he_RunKillPlayer byte).
- **The trailing `stosb` of `T rec = {0}` after the block's plain stores (ours before)**: the stored locals were
  initialised declarations of the same block ahead of rec (`int base = 0; int apply = 1; int ctrl2 = -1; ..
  AIDEC rec = {0};`), so their stores precede the init's tail in list order (ai_LawPrivilegeWindow byte).
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

- **A loop's `xor ecx,ecx` (i = 0) above the `inc edx` (fmt++) and the stores / test before the loop**: `i = 0;` written above `fmt++; ..` with `for (; ..)`, the style the function already used for its first loop (ai_CommandFaithChange byte, both digit loops needed it; one alone stays at 2 ins).
- **A flag store before / after a `rep movsd` or a loop's `xor edi,edi`**: the store's source position decides it; `again = 1;` above `r = 0; memset(..); fam = main_Family;` puts it before the copy's `rep movsd`, and `for (i = 0, again = 1; ..)` puts `xor edi,edi` first (main_ChooseTalents byte; climb.py `move` found the first).
- **A zero store or a flag `= 0` scheduled first in a store run where ours puts it last** (after pushes or record
  fields): move the statement up one at a time (`dcp try NAME --move-line N --to A..B`): `done = 0;` before the
  `memset(&cfg, ..)` of the menu record (ai_PeaceWindow; its twin ai_HatredWindow draft -> shape),
  `sv_SceneEventTime = 0;` before the six saved-view stores, as in the twins ai_EnterOwnBuilding / ai_EnterBuilding
  (ai_RunObjectRoom). The same: `i = 0;` inside the guard ahead of `n = r->used;`, then `for (; i < n; ..)`
  (`xor edx,edx` after the count's load, cm_AddPartyMember).
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
  The building contact loops (`lager = 0; transport = 0; ..` then sv_RegisterNamePlate and a click chain): 12
  matched by permuting the clears (all orders up to 6, a swap / move hill climb from the click chain's order for
  8-12: ai_StorageContact, ai_CriminalContact, ai_StoneKontorContact ..). The order is near, not equal to, the
  click chain's. A chained `a = b = .. = 0;` evaluates from the right: separate statements (ai_ProductionContact).
  Generator: scratch `gen_init.py` / `gen_hc.py` style, one `dcp try` per function.
- **`mov edi, global / lea esi / mov ecx, n / rep movsd` (the inline copy's setup reversed)**: the copy stood
  at the top of both arms of the following if; VC6 hoists it in front of the branch in that order
  (ord_UpdateStatistics). Loop, struct assignment and memcpy spellings at function level all give `mov ecx` first.
- **A byte (or other) local used only inside a loop or branch belongs in that block**: function scope makes an
  address-taken byte live through the whole function, so the fild / compare int temp cannot share its slot
  (init_CreateOpponents `unsigned char b` in the skill loop, he_RunGraveTraining `gain` in the training branch).
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

- **`mov cl,0x80` early, `test byte ptr [x],cl; jne` at two bit-7 tests, ours `mov cl,[x]; test cl,cl; js`**:
  the field was tested as a signed int there (`(int)p->flags & 0x80`): an unsigned operand gets the sign-test
  peephole before allocation, the int keeps 0x80 as a constant candidate (ai_ExpelWorkerWindow byte; making
  the field `int` broke `(flags >> 5) & 1` in ai_GiftWindow: sar). Its `char text[0x300]` was two buffers
  (`text[0x100]` for the captions, `news[0x200]` for the news / message texts).

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
  is the load order. Initialised declarations count as occurrences: `D3BASTREAM *stream = 0;` moved above `int
  nuvfix = -1;` swapped two `[base+idx]` stores to the original's `[idx+base]` (d3_LoadBinaryAnimStream byte;
  `flip.py --want` with both forms in one regex found the swap).
- **Three terms of a sum in an order no spelling gives** (`sq[R - r] + sq[B - b] + sq[G - g]`; 6 orders, temps,
  parentheses one code; crossed M8 lines): the arrays were ONE object, `unsigned char pal[3][256]` (three symbols
  rank differently from three offsets of one symbol). Probe: `extern unsigned char probePal[3][256];` inside the
  function in a dcp try variant (qnt_Nearest 4 ins -> byte; qnt_PalR / G / B became qnt_Pal[3][256]).
- **lea operand order `[esi+edi]` vs `[edi+esi]` for `i = (step + i) % N`**: follows which variable was defined
  first in the loop body (`i = ..; step = ..` flips it), not the text. ai_TokenRandomPlayer: outer `for (i = 0; i
  < 0x300; i++)` around a search that reassigns i (byte); `do .. while (i + 1 < 0x300)` left the lea.
- **A run handler whose `gm_AddTime(..); SendExSetHEI(-1)` exits cross-jump into one early copy (jmp into its
  calls) where ours merges them into the after-switch statement**: no code after the switch and no `default:` -
  every exit written out (`e->exSlot = cm_SendExSetHEI(-1, ev); return;` also for the `break` exits): the
  after-switch statement was the pair merger's partner for every return (c2tail `--tries`: the last jmp is
  tried against all). The building walk of these handlers is the sibling form (`o = &ai_BldsT[k]; bt =
  &ai_BTypesT[o->type]; hi = 0; lo = 0; if (bt->kind > 0x1f) ..`, no kind local: the original's dead `lea` of the
  record address and lo / hi in ebp / ebx), the counter's `n--` before the index step, and a not-found test
  inverted when its block lies after the hit's (`if (c != 0) { ..; return; } gm_AddTime ..`) (he_RunSpying
  byte; its twin he_RunDarkCorner has the same walk and exits).
- **A loop body one scratch step off (ours `mov ecx,[ebx-4]`, the original eax, every temp behind rotated) while
  the call that fills the looped array is equal**: a call argument read through a table into a named local first
  (`col = ev->groups[ev->stage]; n = amt_GetColumnOffices(col, 6, offices);`): the local's register does not
  advance the rotation the way the inline load did. flip only named meaning-changing statements of an earlier
  block (a swapped store there rotated everything behind it, the call's registers too) (he_RunSixGroupEvent byte).
  Same lever for an out-of-line return block one step off (an `ev->end = ai_Time` copy inside a loop, ours
  edx / ecx where the original has ecx / eax): the stage head's query argument as a local, `id = x->target; b =
  gm_BuildQuery(1, 1, id);` (he_RunRobbery 18 -> byte). flip.py --want must be anchored to the block (the code
  before it, e.g. the previous return's stage store and `ret`): an unanchored pattern hits the twin copies.
- **A count kept in ebx with a stack home (frame 4 larger, `mov [esp+X],ebx` after its inits, reloaded after an
  inner loop that borrows ebx) where ours borrows ebp and reloads the event pointer from its argument**: the
  event pointer must outrank the count by a few points: the statement after an `if (n != 0) { .. }` written in
  both arms (`.. cm_SendNewParty(&party); e->end = ai_Time; } else e->end = ai_Time;`, VC6 merges the copies,
  the references count twice); `flip.py --want 'sub esp, <frame>'` named it (a statement moved into the if),
  then climb `--ops move,swap` from that variant: byte (he_RunAuctionEvent).
- **A search loop whose break path re-tests the found pointer (`test ebp,ebp; je notfound` behind the break),
  the not-found block laid after the hit's block**: the loop walks its own pointer and the found one mirrors it
  (`q = he_FindEvent(..); p = q; if (q != 0) do { if (match(q)) break; q = he_NextEvent(); p = q; } while (q
  != 0); if (p != 0 && ..)`): the facts (tests, the deref) are on q, so p's test stays; the `if (q) do .. while`
  form gives the layout (a plain `while` / `for` puts the not-found block after the first test). A `p = 0`
  + `p = q; break;` form decides the test again (he_RunGathering byte).
- **lea operand order `[ecx+eax+2]` vs `[eax+ecx+2]` of a sum against a CSE reused in a later compare** (`t->size +
  plants[i].x + 2` and `plants[i].x - t->size - 2` in a reach test, size reused for the z test): every spelling,
  type and loop form was one code, flip hit only meaning-changing deletions over the whole function; the CSE was
  a local assigned inside the SECOND compare (`pl->x > plants[i].x - (size = t->size) - 2`, z test `size`):
  the first sum keeps its own `t->size`, the named value starts at the reuse (he_RunHarvest byte; assigning it in
  the first sum loads it first and moves the scratch registers). A copy of a global struct through edi after a
  memset's rep stosd = eax, ecx and edx all held across it (he_RunHarvest: `fromId = b->id; time = ai_Time;
  arg[0] = b->id;` with the b->id CSE and `to` / the constant 1 live; a random sample of 3000 fill orders found it).
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

- **`or dl,0x14` .. `or dl,8` on one flags byte where ours folds `or dl,0x1c`**: the engine's flag idiom stores
  another byte in between: `memset(&af, 0, sizeof(af)); af.b[1] |= 0x14; af.b[8] |= 2; af.b[1] |= 8;`
  (D3ANIMFLAGS for d3anim_CreatePath; d3t_MoveCameraPath, cm, ch_ActTurn). A private bitfield struct for the
  same flags folds.
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
- **The same call duplicated at the end of each arm, one copy's `add esp` not merged with the call before it**
  (`call sv_MessageBox; add esp,8; push 1; push edi; call d2_SetViewEnabled`): one call after the if / else, VC6
  duplicated the tail; ours wrote it in each arm and merged the stack adjusts (ai_FireFightingWindow byte).
- **The reverse: call + `ret` per arm with the arguments in registers (`xor edx,edx` at entry, `mov edx,-0x28;
  push ecx; push edx`), ours pushes immediates**: the arms only assign x / y and ONE call follows the if / else-if
  chain; VC6 duplicates the short return tail into the arms after allocation, so x / y stay registers. Writing the
  call into each arm constant-folds them (sv_DrawPanelPointer byte).
- **`push K` above the test of an if / else, the call's own `add esp,4` after it**: the call ended each arm
  (`if (..) { lod = 2; sh = 2; f(0x40); } else { lod = 1; sh = 1; f(0x40); }`), not once after the join
  (cm_LoadEventScene byte).
- **Two call sites (the first arm's with its own epilogue), x / y in ebp / edi and the flags in a slot**: a guard
  return, `if (style & 4) { call(o->x - 0x30, o->y + o->h, ..); return; } call(x - 0x30, y, ..);` with x / y
  each assigned once (`x = c != -1 ? o->x + c : o->x;`, y in an if / else); `x = o->x; if (..) x += c;` ranks x
  below the flags (d2_DrawObjCaption byte).
- **A call and the shared argument pushes hoisted above the test that picks the action code**: the shared
  argument call written inline in both calls (`ch_AddAction(p, 0x2d, x, z, d3t_SceneIndex(p->scene))` in each
  arm); a local `sc = d3t_SceneIndex(..)` before the test keeps the pushes in the arms (cm_ExObjCell byte).
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
- **`mov si,ax` / `and esi,0xffff` after a call (not `mov esi,eax`)**: the callee's result is a 16-bit type at
  this call site (a unit view `((unsigned short (*)(unsigned short))util_Random)`), not a cast of an int result
  (gm_Shuffled). Into an `int` local, not an `unsigned short` one (that gives `mov esi,eax`):
  `int v; v = util_RandomS(2);` (ai_PlagueMessageAll, ai_PlagueNews byte).
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
- **`mov si,ax` / `mov di,ax` then `and esi,0xffff` right after `call util_Random`** (ours `mov esi,eax; and esi,0xffff`
  later, or the int result in eax): the caller read the 16-bit result: the unit's `util_RandomU` / `util_RandomS`
  cast, into an `int` local (`int v; v = util_RandomS(2);`, ai_PlagueMessage byte) or inline in the expression
  (`x / 16000 + util_RandomU(2)`, he_AllocTransport byte). Grep the original for it: several queue functions have it.

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

- **`lea eax,[ecx+edx]` for a sum then clamped (ours `mov eax,ecx; add eax,edx` into a named n)**: the clamp as one
  ternary with the sum written twice, `r->target = a + b >= 100 ? 100 : a + b;` (a CSE temporary, not a
  variable) (he_AllocCartRepair byte).
- **A constant argument pushed after an `_ftol` call (ours pushes it before)**: the converted value was a named
  int local assigned before the call (`d = (int)(f * 10.0f + 1.0f) * mul; f(a, b, d, 1);`; he_RunTribute byte).
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

### How c2.dll allocates the x87 stack
Read from C2 13.00.9044 (addresses at 0x10700000), confirmed with `c2fpu.py` and probes. Until allocation every
float value goes to memory between statements; FUN_1072e2f0 -> FUN_1073122d -> FUN_1076697d (register class 1)
then lifts some into FPU stack slots. `python tools/c2dag/c2fpu.py SRC FUNC` prints each candidate's outcome and
the stack at every instruction.
- **Candidates** (FUN_1071be2a renames them per definition web): scalar float / double locals, each FIELD of a
  local struct or array (`v1+4`, even when the array is passed by address later: written back at its death),
  CSE / expression temporaries (`temp#N`), globals and constants (a load at entry, never profitable).
- **Savings / priority** (FUN_10766d7b; /O2 costs FUN_10767291 def, FUN_10767c36 use, FUN_10767c6c dying fld),
  times w = 1 << loop depth: a 4-byte float: definition +2, use 0, a dying `fld` +1; an 8-byte double: +4 / +1
  / +2; -w per CFG edge it is live across out of a block. Savings <= 0: memory (FUN_10767401). So a float set
  before a loop and read in it stays in memory (`fld s` / `fmul s` each pass, sav 0), a double gets the stack
  (sav 4): a `double` local is the lever for "kept on the FPU across the loop" (d3_SetupFrustum).
- **Order and fit** (FUN_10767ca5): priority descending, equal: the larger tie (+0x44) first. A candidate gets a
  slot only if its live range NESTS with every placed one (born later => dies no later; stack depth <= 8);
  otherwise it is split (FUN_10744286) and the pieces retry. Two values born x, y, z and dying x, y, z cannot
  both stay: of three CSE sums only x / y got the stack, z its own 4-byte home (`fst [L50]`, copied with `mov`)
  (ai_FireCannon: `v3[i] = v1[i] + a[i]; v2[i] = v1[i] + a[i];` made that temp and the original's frame).
- **Pop point**: a value dies at its last use moved forward to the end of the statement's expression (no
  expression temporary left on the stack). There the stackifier pops it (`fstp st(0)`, after the statement's
  store) or, when the statement's result goes into a stack candidate, stores the result over the dying slot
  (`fstp st(k)`). The /O2 peephole FUN_10765f00 (called at 0x1070b551) turns `op st, st(k) .. fstp st(k)` into
  the popping `opp st(k), st` when nothing between uses st(k) and the stack never drops below the op's result;
  FUN_10766172 (0x1070b564) folds `fld m; opp` into `op m` (tables 0x107bb3d0 / 0x107bb3e8 map op -> popping op).
  Example (d3iot_CalcBounds byte): `m->diagonal = sqrt(dz*dz + ..)` gives `fld st(0); fmul st,st(1) .. fstp
  [m]; fstp st(0)`; `d = sqrt(dz*dz + ..); m->diagonal = d;` gives `fld st(0); fmulp .. fstp [m]` (d's slot
  takes dz's: `fstp st(1)`, folded). The dying value's square must come first in the sum: a popping op on the
  result's position before the store (`faddp` right after it) blocks the fold. Probe: patching FUN_10765f00 to
  `mov eax,[ecx]; ret` removes every such `fmulp`.
- **fxch**: the stackifier only copies (`fld st(k)`) and pops; the peephole pass FUN_1070eb58 -> FUN_1070aa47
  turns a copy whose original dies at that use into `fxch st(k)` (its later pop goes away; t_cm: `fld st(2); fsub
  st,st(1)` -> `fxch st(2); fsub st,st(2)`). Not FUN_10765f00 / FUN_10766172 / FUN_107663a8 (each patched out:
  the fxch stays). An `fxch` hunk is a slot-order (nesting / priority) question first.

### Operand order of commutative ops (ranks)

- **`add edx,eax` against `add eax,edx` for `n % 2 + n / 2` (one idiv), no spelling of that statement moves it**:
  the constant-first compare `if (0 != o1->kind) return 0;` 50 lines above did (ai_DecideBuildBusiness, found by
  climb.py's `commute` op; `if (o1->kind)`, `!(.. == 0)`, the sum's operand order, parentheses: = control).
  A zero compared constant-first changes the ranks of later integer temporaries: run climb round 1 before
  generating spellings of the hunk's own statement.

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
  pointer; there the subset of parenthesised operands does: `(m->_22) * m->_33 - (m->_32) * m->_23` (one each
  product; 64 subsets x swaps, one `dcp try`, d3_MakeBasis byte). Tools: uv run respell rules fcast / deadrank, idiom fpu_rank.
- **Parenthesized member reads get their own rank**: `(drop->pos.w)` alone loads before the local `vel.x`
  (d3t_UpdateRain); `x = (p->x += 2.0)` differs from the bare form; `wz = (snow->windz);` moves an unrelated
  `fstp` two instructions. No lever: integer loads (d3_Viewport).
- **A CSE temp ranks above named locals**: `flake->sy1 = cy - y0 * f; ... flake->sy1 + s` loads the CSE first; the
  original needed `flake->sy1 = y = ...; flake->sy2 = y + s;`. A CSE'd product keeps the shared node's order
  (`o[1] = m[2]*m[5]; o[0] = m[3]*m[1] - m[2]*m[5];` m2 first, alone m5 first); stores through a `D3MATRIX *` field
  or `float *` kill the CSE.
- **`m[k]` operand order in products / dot products**: rank depends on the element spelling (`m[k]` / `src->_ij` /
  `((float *)src)[k]`); hill-climb each element's spelling block by block with dcp try and a generated variants
  file (d3_MatInverse 328 -> 67 lines, byte only as macros: "4x4 inverse" below); `((float *)p)[i]` moved what 3000+ permutations did not. Dot product:
  brute-force every subset of the six operands in parentheses, or parenthesize one term, `((n.x * v.x) + ...)`.
  No lever: term orders, swaps, global flips; 3072 variants per row. d3_GetBoxVerts byte: one branch through a
  point-transform macro whose parameters are parenthesised (`((s)->x) * (m)->_11 + ..`), the other branch's paren
  subsets searched JOINTLY over two rows (64 x 64 in one dcp try: per-row greedy rounds did not get there).
- **A `fadd [mem]` moving in an unrelated statement**: ranks also decide how `a + b + c + mem` chains associate.
- **`fld b.gx; fmul c.gz` vs `fld c.gz; fmul b.gx` in a sum of products**: only term order and earlier code move
  it. No lever: all 2^n factor swaps, any grouping. (d3sm_ShadowCB, open)
- **`(double)` casts on single operands as rank movers where parentheses, `(float)` casts and term orders are
  canonical**: the squares order y, x, z needed `(double)v->x * v->x + v->y * v->y + (double)v->z * v->z`; a
  cross product computed sx-first needed `((double)v0->sx - v1->sx) * ((double)v0->sy - v2->sy)`. Generate every
  subset of operands with `(double)` (2^n, one dcp try); `(float)` subsets were all one code (d3_ProjectMesh byte).
- **An int's conversion `fild` / `fst [slot]` interleaved with later stores, its `ftol` before them, ours forwards
  the int into the use**: the int was used TWICE (`year = (int)(hours / 24.0); hour = fmod(hours, 24.0);`): the
  `(double)hours` CSE is the `fst` slot, and the two uses stop forwarding, so the int's statement stays where it
  stands. A double `h = hours` copy gives `fstp; fld`. A float result kept on the FPU across calls with its
  `fdivp` sunk after the pushes: no `(float)` cast on the quotient, `share = (double)a / (double)b;`
  (gm_TitlePeriodServed byte).
- **Squares summed in another order (x, z, y)**: `(float)sqrt((double)(a*a + b*b + c*c))`; without the double
  cast the order is canonical (d3t_EmitterCB).

### Product and sum chains: association, factoring, folding

- **`a*s*k*K + a*s*0.5` factored by VC6 into `(a*K + 0.5)*s*k`, the original expanded (`fild s; fild a; fmul st,st(1);
  fmul k; fmul K; fxch; fmul k; fmul 0.5; faddp`)**: nest each product, `(((float)a * s) * k) * K + ((float)s * k) *
  0.5f` (ai_BuildTownMap; casts, orders, /2 had all been factored).
- **A building / camera fallback whose building block lies after the epilogue with its own pushes**: `obj = cam; if
  ((b = A) == 0 && (b = B) == 0 && obj == 0) return; if (b != 0 && b->object != 0) f(b->object ..); else f(obj ..);`
  (the `b != 0` test is decided per path; ai_BuildTownMap). A loop case written as `if (..) { ..; continue; }` instead
  of if / else moved the loop latch into the right split region (212 -> 45 ins there).

- **`a*14 - b*14` with b loaded once but multiplied at every use (the volatile a reloaded each time), ours keeps
  b*14 as one CSE**: the source was `(a - b) * 14`: VC6 distributes the constant per use after CSE, so only the load
  of b is shared (cm_DrawFightTime byte, d3_Time volatile).

- **`fld [m]; fmul [1.0f]; fsubr` where ours has a plain `fsub [m]`**: VC6 folds `x * 1.0f` and `x * -1.0f` literals;
  the -1 came from a vector local (`d.z = -1.0f; pl.x = m->_11 * d.x + m->_21 * d.y + m->_31 * d.z;`), +1 / 0
  from it fold or stay `* 0.0f` as with literals (d3fl_FarDist, 50 bytes; every term order is one code).
- **`fadd st(0),st` right after the definition's `fmul` (before its `fstp`), ours `fld x; fadd st,st` in the final sum**:
  the doubling was its own statement, `age = (b + 1) * 0.1f; age = age + age;`, the sum then uses `age`; `* 2` /
  `* 2.0f` folds into the constant (hmt_Score byte).
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
- **`fmul [const]` scheduled one integer instruction late / early**: a `(float)(..)` cast around an already float
  product moves its place among the integer address arithmetic of the same block: `x = rate * 0.01f; x = x *
  base;` matched where `x = (float)(rate * 0.01f); x = (float)(x * base);` left the `fmul` one `lea` early
  (gm_IncomeKind3; its sibling gm_IncomeKind4 matched WITH the casts, so try both).
- **`fld x; fmul 1/c; fmul k; fmulp` after a call-carrying factor S (ours `fmulp` then `fmul k`, or a `fxch st(1)`
  pair)**: `S * ((x / 30.0) * 8.0)`: parentheses around the division keep both constants and stop VC6 pulling
  `* 8.0` out past S; a `(float)` cast as the barrier pulls the constant out or adds `fxch` pairs, no parentheses
  fold 8/30 into one constant (he_RunStudy byte; he_RunGraveTraining fpu hunk gone).
- **`fmul 1.0` in the original**: a literal `1.0f * x` folds; the constant went through a variable (`one = -1.0f`,
  `gy = -1.0f; ... gy * m._21`), giving `fmul 1.0; fsubp` / `fsubr`.
- **Int factors**: `(int)(k * C * data)` gives `fild; fimul; fmul`; `fild; fmul const; fimul` is `(int)((float)(k
  * C) * (int)data)`; `n * (x * 0.01)` gives fmul-then-fimul (0x4d3d10).
- **4x4 inverse**: write the determinant and each element in full; cofactor temps change the shape. Byte form
  (d3_MatInverse): Graphics Gems det3x3 / det2x2 as MACROS, `#define E(i, j) (src->_##i##j)`, `DET2(a,b,c,d) ((a) *
  (d) - (b) * (c))`, `DET3(..) (a1 * DET2(..) - b1 * DET2(..) + c1 * DET2(..))`: the macro's parentheses around
  each element are rank levels; they made VC6 load the member before the 2x2-minor CSE (`fld [m]; fmul st,st(k)`,
  dead pops after the det store), which no spelling of the hand-expanded text did. When a product chain is a
  textbook formula, try it as the textbook's macros (paren levels per argument) before per-site spellings.
- **A hoisted `oy + 1.0`**: `h * cy + (oy + 1.0f)`.

### Values kept on the FPU vs stored and reloaded

- **Two divisions by one float local turned into `fld 1.0; fdiv x` + `fmul st,st(1)` (a shared reciprocal), the
  original dividing twice (`fild n; fdiv [x]`)**: VC6 shares 1/x between float divisions only; one of them in double
  stops it: `1.0f - (float)(n / (double)x)` (ord_ComputeStatistics; `(float)n / x`, a temp, call between: = control).
- **A field read once onto the FPU (`fld [esi]` among the CSE loads) for a value used twice in one product / sum
  chain, ours `fadd [esi]` / `fmul [esi]`**: named float locals for every term, assigned in the original's load
  order right before the statement (`q3 = 1 - out[3]; q1 = 1 - out[1]; income = out[0]; q2 = 1 - f;`); a local
  first used earlier in the function (a loop variable reused) ranks too low: take a fresh one. The stored value
  forwarded with `fst` came from a local too: `f = ..; out[2] = f;` (ord_ComputeStatistics).
- **Equal-count int locals in the slot order of the other pair**: the tie follows the order the expression that
  reads them evaluates them: `(float)a / b * ((float)c / d)` puts c / d first (ord_ComputeStatistics).


- **Two results held on the x87 stack until after a call's argument copy (`fxch; fstp [key.rot.x] .. fstp
  [key.rot.y]` at the end), a third copied through memory (`fstp v.z; mov edx,[v.z]`)**: the values were fields
  of a block-local D3VECTOR4 copied field by field into the key at the end (`v.y = angle - v.y; v.z = -v.z;
  keys[1].rot.x = v.x; ..`): field webs are candidates, v.z#2 gets no fit; a function-scope vector whose address
  went to a call earlier also orders the field loads after its stores (ch_ActTurn 909 -> 915 = original).
- **`fild n; fmul st,st(1); fidiv d; fstp qword [tmp]; fstp st(0); call ..; fadd; fcomp [tmp]`**: the product is
  the LEFT operand of a compare against a call: `if (c * share / d > util_RandomF() + 0.15)` (evaluated first,
  spilled as a double over the call, share kept on the FPU). A `double limit` local, casts, both orders of the
  product: = control (gm_LikingChance).
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
- **`fild a; fld b; fchs; faddp` (ours `fild a; fsub b`)**: `(float)a + (-b)` with the parentheses; `a + -b`
  without them is folded to a subtraction like `a - b` (gm_IsBankrupt byte; `b = -m; b += a` also byte).
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
- **Pop points** (mechanism: "How c2.dll allocates the x87 stack"): a dying value pops after its statement's
  store; `fmulp` at its last use only when the result goes to a stack local. A once-used scaled
  parameter folds into its use; `size * sx` at every use (or an aggregate initializer) gives the early multiply.
  A scale for the last `fmul [mem]` is a float declared in the if block.
  `fmulp; fadd [m]; call _ftol` (ours `fmul st,st(1) .. call _ftol .. fstp st(0)`, the dying factor popped late):
  the sum went into a float local converted at two uses, `y = .. * f + c; r->top = min(r->top, (int)y); r->bottom
  = max(r->bottom, (int)y);` (one `_ftol`, the result stored over f's slot); an int local `iy = (int)(..)` and every
  single-use float local are folded to the control (d3if_ScreenBox byte).
- **`fisubr [int]` / `fsubrp` for the LAST use of a float local (`half`), the result then stored to memory** (ours:
  `fild; fsub st,st(1) .. fstp [y]; fstp st(0)`): that one subtraction was done in double, `y = (m->cellZ -
  (double)half) * scale;` (half itself float: a `double half` gives the control). Two-step / copies / struct fields
  / chained assignments / names all fold to the control; `double y` gives the pop but keeps y on the FPU
  (ai_MapCellToScreen byte; ai_UpdateTownMap the same hunk). fisubr occurs once in the whole image.
- **Unsigned ints into a macro using them three times**: pass straight; a float local spills (~1000 asm lines,
  d3fl_Draw).
- **`if ((g = f()) < best)`** gives `fld st(0); fcomp`, split gives `fcom`. `x = (float)(p->m - 2.0)` stores before
  the next call's cleanup. `PAR_RND()` vs `(float)rand() / RAND_MAX` moves a lea / fstp: try per component.
  `mov edx,[d]; mov [d],edx` is a second float local in d's slot.
- **A float local copied into a global by `mov reg,[s]; mov [g],reg` (an int copy) where ours keeps s on the FPU**: the
  copy's statement stood after the next `atan2` / a value s does not nest with on the x87 stack, so s went to memory
  (c2fpu: `no fit (does not nest with tb)`); `P1.z = s` moved after `tb = atan2(..)`, `P3.z = s` after the rot angles
  (d3_SetupFrustum 98 -> 24 ins, padded size fixed; the frame is then 4 bytes bigger, open).

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

- **`fstp` of a loop's float store before the pointer `add`, ours after `dec`**: `(float)(x * 0.001)` - the
  explicit narrowing cast of the double product keeps the store in place (gv_AllocAvatar, now an index loop over
  GVAVATAR.mood[j][1]). No lever: `/ 1000.0`, `0.001 * x`, `(double)x * 0.001`.
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

- **A float field's `fstp` held behind the next call's pushes** (`fmul [1.5]; xor eax,eax; mov ax,[ebx]; push eax; fstp [rec.mean]; call`): the implicit double -> float conversion `rec.mean = rec.demand * 1.5;`; the explicit `(float)(rec.demand * 1.5)` stores at once (gm_RandomMarketShake byte). No lever: `1.5 * x`.
- **`fld a; fdiv [r+..]` vs `fld [r+..]; fdivr a`**: `a / r->div` vs `a / (r->div)` or `*(float *)` (0x5dc210).
- **Int store before `fdivp`, original after**: `v - (v * z) / (r * r)` (d3sndw_Update 4 -> 2). No lever: orders,
  casts, temporaries, int local. Flip search: compare only the two instructions' order.
- **`fild a; fild b; fdivp`**: `(double)(a) / (double)(b)` (`(float)a / b` gives `fidiv`); `((a) * 1.0) / (b)`
  moves `fdivp` one or two instructions earlier (d3t_GetTriRange).
  Also for two int call results: `(double)f(..) / (double)g(..)` (gm_UpdateTechTree byte).
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
- **A literal factor moves to the end of a product**: VC6 compiles `a * 1450.0f * r` as `a * r * 1450`; the
  original's multiply by the literal first is its own statement (`f = a * 1450.0f; f = f * r + ..`), the same
  code a named const gives (ai_CannonCamera; the .rdata order said literal).
- **`fimul [m]` on a joined float against `fild; fmul st,st(1)` + `fstp st(0)` after the call**: the product into
  the float itself (`f = f * r->points * 10.0;`, then `(int)f`): its last use is that statement (he_AllocRepair).

## Types, widths, bitfields
`movsx` / `movzx`, signedness, char and short locals, bitfields and byte read-modify-write, shifts, multiply and divide idioms, casts, unions.

### A byte bit test turned into 0 / 1

- **`movsx r,BYTE [m]; and r,imm; shr r,n`**: `if ((char)x & imm) v = 1; else v = 0;` or `if ((char)f & 2) return 1;
  return 0;` (VC6 turns the if/else into and/shr after the sign extension; also on an int flag slot). No lever:
  casts, shifts, ternaries, bitfields, volatile, inline functions all narrow to a byte op; `(f & 2) != 0` and
  `(f & 2) >> 1` give shr/and. A dword load then `movsx r,al` means the flags sit in an int local first.
  (amt_IsColumnDecided; he_IsMessageKindFiltered: one switch case `if (mask & 0x40) return 1; return 0;` on an
  int global, no cast, where its sibling cases `break` to the shared `return 0` and keep `test byte ptr`)
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

- **An inline accessor function for a bitfield (`(*d3_ObjBits(o)).top = ..`) moves the allocation**: the
  original wrote `obj->bits.top = ..` directly (d3_AppendSibling byte, with a `last` cursor instead of the
  parameter as the loop variable).
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
- **imul operand order**: `(short)w * (short)a` vs the swapped order swaps the operands. Of two int locals the
  text order of `i * size` is no lever; the one DEFINED later is copied first (`mov esi,i; imul esi,size` needs
  `i = it->index;` after `size = ..`, inside the loop body: pool_Next byte; flip.py found it by deleting i's init).
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

- **`test [table+8],reg` where ours tests an immediate (a const table defined in the same unit is folded)**: the
  table's definition belongs to another unit; ai_BuildingTypeMasks (0x5eed70) sits behind menu.c's gypsyFilter
  (0x5eed68) and is defined there (ai_MasterUpdate byte; with a signed end-of-table compare).
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

- **Offset copy with `lea eax,[eax+table]` where the hand `s = tbl[g].name; d = out - s; do {..}` gives `add
  eax,table`**: `strcpy(out, tbl[g].name)` (ai_BuildingName). The opposite held for init_BuildingKindName (byte with
  the hand loop; strcpy there is 12 ins off): measure both.
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

- **`mov ecx,0x10; lea esi,src; lea edi,dst; rep movsd` where ours (memcpy) sets edi first**: an element loop
  `for (k = 0; k < 16; k++) r->members[k] = tmp[k];` over an `int` buffer, which VC6 also turns into rep movsd
  (ls_ReadParties byte).

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

- **`mov ecx,N; xor eax,eax; rep stosd` where ours has `xor eax,eax; mov ecx,N`** (memset + `return 1` at two exits):
  the original had ONE `memset(..); return 1;` after an if / else (VC6 duplicates the short tail into both arms and
  the copies keep the tail's order); a memset + return written out in each arm schedules xor first
  (ch_RemoveAction byte).
- **`mov [slot],0` right after `rep stosd` where ours stores the memset's `eax`**: the `= 0` stood before the memset
  (`canTrain = 0; memset(arg, 0, sizeof(arg));`), a move-line round finds it (ai_TalentWindow byte).

- **A 64-bit field's low word stored `mov [m],0` last, where ours reuses the memset's `eax` right after `rep
  stosd`**: the two words written out, high first: `cfg.classMask[1] = hi; cfg.classMask[0] = 0;` (low first =
  the 64-bit `(u64)hi << 32` = the CSE'd eax store; `& 0xffffffff00000000` reads both words) (ai_OpenRobberBribe).
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
- **An inlined `repne scasb` twice in the original, once in ours**: `strlen(s)` written in each call, not a length
  local (ai_BauplatzFilter byte; an older note had read it as an intrinsic question).
- **`lea eax,[i*4+ai_Chars]; mov al,[eax+0xd]`** (the record address kept apart from the field offset): a
  record pointer read in several tests (`c = &ai_CharsT[i]; if (c->b00d >= 9) .. else if (c->b00d == 8)`); a
  local for the byte folds to `[i*4+ai_Chars+0xd]` (he_AllocRepair).
- **An `unsigned __int64` member copy with the high word loaded first**: the two words copied apart, low then
  high (`cfg.classMask[0] = gypsyFilterWords[0]; cfg.classMask[1] = ..[1];`; ai_DismissOfficeAsk).
- **`mov eax,0x5d` (the list's first element folded) before the walk's `lea ecx,[list]`**: an index loop over
  the initialised local array, not a pointer walk; `rep stosd` of -1 with `mov ecx,8` before `lea edi`: memset,
  not the element loop (main_ReplaceFigure).

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

- **A store of a call's result into a slot of an array the other branch uses, before an early return** (dead, so
  VC6 keeps it only for an address-taken local): the result was written into that array's element (`open[0] =
  gv_HeldCrimesOf(..)`); the extra reference also orders the arrays in the frame (ai_AccusationWindow /
  ai_FaithAccusationWindow byte).
- **The original's frame 16 bytes larger with one 4-byte slot at the top (r1 w1)**: a temporary of its own D3VECTOR4
  (`up.y = (pos[1] = ..) - cen[1];`), not a reused float (sv_FlyCameraToModel byte).
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
- **A swapped argument pair against a K&R or loose prototype**: compare the push order of every call with a byte
  twin of the same callee; `he_ShowNews(id, -1, 1, text, ..)` compiled with a warning and pushed the text as the int
  (ai_QuarrelCallback). A word store 8 bytes off inside a filled record is another index (`ids[4]` for `ids[0]`).

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
- **`python tools/c2dag/unitprobe.py UNIT PATCHFILE | --unknown FUNC`**: whole-unit scratch compiles for callee
  questions (dcp try grades one function): patches anywhere in the unit, every function whose code changes
  listed with its distance to the original before -> after. See "the callee summary".
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
- **A byte sibling's statement inside a bigger function**: the news text id `(w->sex != 0) + 0x10c8` (`setne` into
  the wrong scratch pair from there on) was the sibling's `int txt; if (w->sex != 0) txt = 0x10c9; else txt =
  0x10c8;` (cm_DuelOutcome, byte): 68 -> 9; the last hunk then flipped to the other operand order of the host test
  (`ai_CharsT[ai_PlayerId].netId == m->host`) that an earlier round had rejected: byte. Re-run rejected rounds after
  a lever moves the rotation. (cm_RunDuel, deep s54)
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
