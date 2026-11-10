# VC6 SP5+PP /O2 – quick reference

The rules that paid off most often, condensed from notes/vc6-matching.md (rounds 1-14). That file has the
details and per-function attempts: search it by keyword (`grep -n "D3VECTOR4\|dead parameter" notes/vc6-matching.md`)
when a function resists, instead of reading it whole.

## Workflow
- `uv run ledger check` does not rebuild: run `uv run build` first.
- Undeclared calls compile silently (C4013) and change the caller's code: grep build logs for C4013.
- `uv run asmdiff --regs a=b,...` separates register allocation from everything else. asmdiff shows a bogus
  -/+ pair for an instruction with two relocations; trust `uv run asmdiff --bytes` / `check`.
- `uv run try`: an empty replacement (`===` directly followed by `>>>`) swallows the next block; put a comment in.
- Brute-force orders (statement orders, parenthesizations) with a generator script for `uv run try`, not by hand.
- On a big function, diff with esp offsets masked first: what is left is the real work, the rest is stack slots.
- Ghidra misses starts after short functions and callbacks only reached via function pointers; check nop
  padding in `uv run disasm`. A big Ghidra "function" can be several (round 13: 8 KB = four).
- A function defined earlier in the same file changes its callers' code (VC6 knows which registers it leaves
  alone, and keeps a global in a register across a call to a callee that writes no global): define callees above
  callers as in the original; a regression on a file join is TU-boundary evidence (round 19: d2_Unk475bf0).
- A `uv run try` variant can match byte for byte with two globals swapped: run `uv run link` after `--keep`.
  Near-misses can flip with unrelated code above them: judge them once the file is complete.

## Last-bytes pass (match-base)
Every escalation runs the pass first: `uv run spell NAME` prints source spellings to look out for, one group
per detected pattern (sib, add-lea, width, rep-stos, dup-tail, push-pop, branch, cmp-test, reload, iv-shift,
moved; `uv run spell --help` is the rule table). Those lines are hints, not an authoritative list: any source
form the evidence supports may be tried, one edit or several per build, keeping a gain and reverting a loss,
until the ideas run out - the stop condition is no more ideas or a next step that needs deep's toolbox, never
a count. On a far function start from the reading's meaning-class leads (a wrong type / signedness / offset /
width), with the spell patterns as a cross-check. A far draft is escalated only after its leads were tried AND its
readability advanced as far as evidence allows - even when behaviour / byte cannot advance, readability must
(names, Q6 descriptions, typed views, Q3/Q4/Q5; naming other units' callees and globals is base work whatever
the count). A fix that matches is applied to the unit's siblings with the same statements. The escalation
reason lists the leads and spell patterns tried and each remaining Q failure's evidence gap or blocked header.
Rounds, families and the playbook below stay match-deep's.

## Last-bytes playbook (scheduling, store order, slots)
The last few bytes are usually a scheduling or slot choice that no respelling of the statement changes. Spellings,
parentheses, casts, renames and statement permutations each get one generated round at most; then use these
mechanisms, each as a generated round over all positions / subsets:
- **Store kept late / load hoisted early** (one store after an unrelated later store): read the value into a local
  early, write the other store through a pointer local (`sz = &p->size; *sz = ..;`, VC6 then assumes they may
  alias), store the local last. The pointer alone keeps the order but stops load hoisting (par_EmitterCB0).
- **Compare or load hoisted to the top of its block** while the original has it after some stores: write one of
  the stores through a copy pointer (`t1 = t + 1; t1->f = x;`); the test lands after that store. Try every store
  of the run (a 2^n mask generator: par_Draw matched with exactly one of five).
- **Stack slot / dead-parameter slot taken by the wrong variable**: the slot follows reference weight after
  optimisation. Add real references with no code: a statement written in both arms of an if/else that VC6
  tail-merges (par_EmitterCB1: the speed multiplies in both initfill arms), a split variable, a `x = -x`-style
  use the sibling has. Dead or folded uses (`if (0)`, `* 1.0f`) do not count.
- **Order that no statement permutation gives**: read the fields into locals first (ai2 0x48a380), or write the
  statement in both arms of the preceding if/else (scheduling follows the duplicated form, ai 0x489660).
- **FPU value kept alive / slot weight / operand order that no spelling changes**: the original often reuses one
  float local for two roles (d3_LightAddObject: `s` as the squared distance in one loop and the scale in another,
  115 B). Rename block locals to the other locals (`ren` in uv run vargen). For `fld a; fmul b` vs `fld b; fmul a`
  copy one operand into another local first (`t = z; ... (t * s)`).
- **Copy-propagated pointer loads through the wrong register** (`rec = p; while (rec->next)` loads via p): read
  the source expression twice instead (`if (head) { rec = head; ...`), the CSE'd copy uses the new register.
- **A value the original recomputes inside an if block** (reloads a field / recomputes an index that the test
  already computed; keeps only the loaded globals): no local for it. Write the expression out again inside the
  statement, especially one that also contains a call (anm_Unk4bb860: `if (util_RandomF() > X / (d3_Time - X))`
  instead of `t = X; r = &a[i]; if (util_RandomF() > t / (d3_Time - t))`). A local (`t = X`) makes VC6 reuse
  the test's value. Probe with uv run listing: which values VC6 shares, and an insertion map (store / call at
  the block start) tells what the original's IR had there.
- **Same block compiles differently in two functions**: the cause is outside the block (weights, a sibling's
  extra statement); diff the two sources.
- **Callee-saved registers swapped (ebx <-> edi / esi) and no respelling reaches it**: C2 hands out callee-saved
  registers in priority order, and the priority is summed per basic block: P(block) = distinct allocation
  candidates referenced in it (variables, CSE temps, webs of a scalar global between kills, and constants used
  more than once, one web per value whatever the spelling); +2*P per reference, -P per block lived through
  unreferenced. `uv run c2prio SRC FUNC` prints P per block and every priority / register: compute what one
  more or one fewer candidate in a block does before writing variants (sfx_OpenDigOutput: i 68 vs 66, matched
  at 66 vs 66: a tie goes to the candidate created later). Casts, constant spellings, labels, constant locals
  and single-use locals in expressions never reach P. Code-neutral candidates: a local passed as a call argument
  (`n = f(); g(n);`), and a local with a second use that C2 folds from the test it sits behind (`n = f(); if (n
  == 1 && ..) g(x / n);`: n stays in the test block's P). `--ir` prints the instructions C2 counts. A candidate
  that replaces a scratch temp (`size = n * 4; f(size)`) rotates every later scratch register of the block
  (eax, ecx, edx are handed out round-robin in IR order; a local in eax from the global allocator does not advance
  the pointer), so add candidates that fold away or sit in a callee-saved register.
  A callee-saved register that the original never pushes (ours uses ebx, the original ebp / esi / edi) is a
  candidate that got that register but never materialised, usually `const#0`: the original ranks it between
  the two values. A join statement written in both arms (`s->layers = l;` after the if and in the else,
  tail-merged into the same code) adds references in P=2 blocks (d3sky_CreateLayer: s 10 -> 14, ties const#0
  at 14 and wins as the later candidate: s edi, const#0 ebx unused, tex ebp; matched).
- **`setne` where the original has a branchy 0/1 (`xor r,r` / `mov r,edi` into a bitfield or byte)**: the
  original condition was an `||` of two tests of the same value, `(x & A) || (x & B) ? 1 : 0`. VC6 merges them
  into one `test x,A|B` but keeps the branch form; `x & (A|B) ? 1 : 0` / `!= 0` give `setne`. The branch temp
  takes a callee-saved register and reshapes the constant webs around it (d3d_GetDeviceInfo: 91 -> 25 lines).
- Signals: identical bytes for a whole family = mechanism does not reach it; a moved difference at a worse count
  = right mechanism, refine it (subsets, positions).
- **A message struct declared in the block that fills and sends it** (`if (..) { AIMSG msg; memset; fields;
  cm_SendLongMessage(&msg); }`, not at function scope): VC6 then schedules the fill as the original (a load
  hoisted above the memset, constant stores sunk past the push, a byte store after the time copy) where no
  statement order did (5040 orders in hmt_DecideElection; ~250 in ai_MsgWorkerStop). Swept with deep-tools
  blockscope.py over every below-byte function with a function-scope AIMSG (2026-11-10): 9 byte, 1 draft ->
  shape, 5 closer. Try it first on any fill-order hunk of a struct local; the declaration in a loop body or
  at the start of a switch block counts too.
- **A void-looking init whose last statement tests a call's result** (`if (f(&x) == 0) {..}`) where the
  original keeps eax untouched to the end and uses ecx / edx for the branch's loads: the function returned
  the result (`int hmt_Init(void) { ..; ret = f(&x); if (ret == 0) {..} return ret; }`; the caller ignores it).
  hmt_Init 12 ins -> byte.
- **Two zero inits before a loop** (`n = 0; for (i = 0; ..)` gives `xor ebp,ebp; xor edi,edi` in n, i order):
  the original's i-first order is `for (i = 0, n = 0; ..)` (ai_BlackmailBuildingListener). Two initialized
  declarations store in declaration order (`AIBLD *nearest = 0; float best = 1e8f;`, he_RunFire).
- **Four zero stores to a struct local** (`h.a = 0; h.b = 0; ..`) keep the zero in a callee-saved register for the
  whole function (`cmp eax,ebx` tests, ebx pushed at entry); `memset(&h, 0, sizeof(h))` gives the original's
  `xor eax,eax` + stores and frees the register. With it, failures after the early count checks written as
  `goto fail;` (allocations and the final call `!= 0`) to one cleanup tail before the success return gave the
  original's layout: own epilogues for the early `return -1`s, the success block last (hmt_CreateBinaryFile, byte).
- **A not-found `return 0` after a search loop that leaves with `goto add`**: the original's `xor eax,eax` after the
  pops came from an always-true `if (i >= N) return 0;` after the loop (`break` + `if (i == N)` leaves a compare,
  `>=` another shape). A function whose whole body sits in `q = 0; if (p) {..} return q;` shares eax = 0 with the
  loop counter only as `if (p == 0) return 0;` up front (veg_SetPlant, byte).
- **A pointer walk `for (i = n; i > 0; i--) { ..*pts..; pts++; }`** where the original counts down in a dead
  parameter's slot (frame 4 smaller): write the index loop `for (i = 0; i < n; i++) .. pts[i]`. A fallback
  `if (!f(..)) { x = px; z = pz; } g(x, z);` whose original pushes px/pz straight into the call is
  `if (f(..)) g(x, z); else g(px, pz);` (rd_PointsNear, byte).
- **Insertion into a sorted top-N array**: `for (k = 4; k > j + 1; k--) best[k - 1] = best[k - 2];` (struct copy,
  guard `cmp j+1, 4; jge`, pointer at best[3]); the stop test `if (!(s > best[j - 1].score)) break;`. Table loads
  `T1[m - 1] .. T4[m - 1]; f(m - 1)` whose original decrements before the last load: one index local `e = m - 1`.
  `(b ? 0x28 : 0) + 0x253 + type` folds into one `lea`; the original's `add ecx,0x253; add ecx,ebx` is
  `(b ? 0x27b : 0x253) + type`. A `char text[260]` got an 8-aligned slot with a hole (frame 8 larger); `[256]`
  gave the original's frame (hmt_Election, draft 95 ins -> byte).

- **Call pushes before the branch that picks one argument** (`push f; push size; push 1; test flip; jne` and the
  arms only computing the row index, then one shared `imul; lea; push; call`): the call was written in both arms
  of `if (flip == 0) fread(dst + i * w * 3, ..); else fread(dst + (h - i - 1) * w * 3, ..);`. VC6 hoists the equal
  pushes above the test and merges the tails. A ternary inside the argument is evaluated before every push, with
  any operand order or placement (d2_ReadBmpPixels 92 -> 12 ins).
- **`x * 3 & 3` becomes `neg; and 3`** (VC6 folds (3w) & 3 into (-w) & 3 through a local copy, any cast or
  spelling of the 3 or the 3w); an original that keeps `lea r,[w+w*2]; and r,3` did not use that spelling, and
  `w3 - (w3 >> 2 << 2)` keeps the lea but not the single and (d2_ReadBmpPixels, unsolved). VC6 also knows a
  field's value after `if (bi.biBitCount == 0x18)`: `w * (bi.biBitCount >> 3)` compiles as `w * 3`.
- **A struct-field tail store order** (`s->left = 0; s->pitch = ..; s->top = 0; ..`, the original storing top=0
  late and popping late): permute the tail stores; d2_MakeSurface wanted left, pitch, right, top, bottom.
- **`movsx eax, bl; and eax, 8; shl eax, 8` from an int flags**: every `(char)flags & 8` spelling folds the
  movsx away; only a multiply before the mask (`(char)flags * 0x100 & 0x800`) keeps it, as `shl; and 0x800`
  (d2_MakeSurface, unsolved: the and-before-shift order is open).
- **A long run of independent `global = rec.field;` stores scheduled in another order** (crossed M8 lines, `dcp try`
  showing `0 ins` but still draft): the list scheduler follows the source order, but not 1:1, and single swaps
  stall. Score orders against the original's listing with addresses resolved (deep-tools storeorder_hc.py) and
  climb from several starts (the natural/save order too). d3if_SelectScene reached byte with the natural order
  except one line; the earlier "emitted order" source was 28 lines worse.
- **A `(&a->x * b->y ...)` sum of products whose operands load in another order** (`fld b.gx; fmul c.gz` vs
  `fld c.gz; fmul b.gx`): VC6 canonicalizes the factor order (all 2^n swaps = control) and reassociates the sum
  (any grouping = one code); only the term order and code earlier in the function move it (d3sm_ShadowCB, open).
- **Local copies of array elements (`a = d[i]; b = d[j]; .. (a + b)`) where the original reloads some of them after
  a store**: the source read the elements in place; VC6 CSEs the reads up to the first store and reloads after it.
  d3sm_Sample 340 -> 194 ins. A halving loop `for (; step > 1; step = half)` vs `step >>= 1`: different IVs (-34).
- **Two bitfields of one byte set in both arms** (`solid = 1; cast = 1;` / `solid = 0; cast = x;`) where the
  original does `and 0x3f; xor` (one RMW for both) and one shared store: assign cast before solid in the else arm
  (d3sm_ShadowCB, -28 ins). A struct local used only in a loop body declared there shares a dead array's slot.

- **Byte bit-or built from a parameter** (`mov al,[loop]; and al,1; shl al,3; or al,0x10` then a plain byte store)
  where `(loop & 1) << 3 | 0x10` (any spelling) becomes `((loop & 1) | 2) << 3` and an RMW of the byte: the byte was
  two bitfields (`f.u.bits.f08 = loop; f.u.bits.f10 = 1;`). With the other field stores permuted (720 orders,
  dcp try) ch_AttachAni reached byte. Adding that bitfield view to ch.h's CHANIMFLAGS moved sv_t's
  ai_TutorialInitChapter2 off byte (declaration-set tie-break): put such a view in the unit (CHANIMFLAGSB in ch.c).
- **A search loop whose bound min(N, n + 32) the original recomputes per iteration** with n in a scratch register
  (ours hoists n + 32 and spills n, frame 4 larger): test the first cell through the array (`!pts[n].x && ..`) and
  start the walking pointer in the for-init, `for (i = n, pt = &pts[n]; i < min(0x100, n + 32); i++, pt++)`
  (ch_ActWalkCells: frame 0x1ac -> 0x1a8, 54 -> 4 ins). A zero-fill of two-byte records indexes the record type
  (`((CHPATHPT *)p)[i].x = 0; ..z = 0;`, i < 0x100), not bytes `[i]`, `[i + 1]` with `i += 2` (index/base order).
- **Three 16-byte locals in another frame order** with equal code (ch_FindFreeCell): framesearch showed declaration
  order, names, block scope, pointer views and struct copies never move them, two extra real reads of one do. One
  array `D3VECTOR4 p[3]` gives ascending order with the same code (draft 88 -> shape 50), but the array is
  address-taken, which reorders a copy's loads into it (open).

- **A string copy `inc eax; sub edx,eax; mov cl,[eax]; mov [edx+eax],cl; inc eax; test cl,cl`** (one running
  source pointer, the destination as an offset from it) is not strcpy (strcpy and `*d++ = *s++` give the
  two-pointer `inc ecx / inc edx` loop, an index loop `out[i] = s[i]` walks the destination):
  `s = src; off = out - s; do { c = *s; s[off] = c; s++; } while (c);` (init_BuildingKindName byte).
- **A test that the original lays out with the else arm inline whatever the condition's spelling**
  (`if (A != 2 || (p = ..) == 0) {X} else {Y}`, &&, nested, early returns: one code): the pointer was assigned
  on both paths first (`if (A == 2) p = ..; else p = 0; if (p == 0) {X} ..`); VC6 threads the `p = 0` path to
  X and keeps X inline after the p test (init_TownBuildingHook byte).
- **A value stored to a table and returned at once** (`t[k] = uni; ..; return t[k];`, ours `movsx eax,al`, the
  original re-reads `movsx eax,BYTE [esi+t]` with k kept in esi): the store's path joined the other path before
  the one final `return t[k];` (no return inside the if); VC6 then cannot forward the stored value
  (init_BuildingSceneUniverse byte).
- **An address-taken local reused for a later value** (`w1` passed as `&w1` in a loop, later a send slot kept
  across calls): the reuse forces the later value into memory; give the address-taken use its own local
  (init_StartNewGame: the judge slots went back to esi / edi, the mid-function `push edi` appeared; byte).
  A `(unsigned)&X + N` bound macro gives `jb`; the original's `jl` is a signed `(int)` bound. The same in init_OpenTown: `&n`
  of two field deltas in a block-scoped `int v` brought the frame from 0x408 to the original's 0x404.
- **Two calls of one empty function in an if/else that VC6 merges into one** (the original calls the same address
  in both arms, ours once after the join; every order / polarity / goto keeps them merged): the original called
  two different empty functions that LINK folded (/OPT:ICF). Give one arm its own unlinked stub (`/* unlinked: ..
  folded into X */ void init_RoomNoOp(void) {}`, defined below the caller) (init_ShowBuildingScene byte).
- **The pops before the final `if` tail** (ours `mov al,[flags]; pop edi; pop esi; test al,8; ..`, the original
  `test byte [flags],8` with every pop after the tail): VC6 sinks the restores of registers the tail does not use
  unless the tail was written on two paths; give one branch its own copy of the tail and a `return;` (VC6
  cross-jumps the copies into one block, toy-proven with deep-tools toyasm.py) (init_LoadObjectScene byte).
- **A product CSE'd from an if test into the loop below** (ours keeps `uni * 0x3e0` in ebx, the original keeps uni
  and recomputes it in the loop): test the table entry again in an else-if, `} else if (d3_Scenes[T[n]].dirty) {
  uni = T[n]; ..` (not `uni = T[n]; if (d3_Scenes[uni].dirty)`).
- `(x != 1) * 4 + 4` gives `setne cl; lea edx,[ecx*4+4]` (a new register); `x != 1 ? 8 : 4` or an if / else gives
  the original's `lea ecx,[ecx*4+4]` in place. `d3if_ShowTree(d, g != 0)` (setne) against the original's
  `jne; push 0; jmp; push 1`: the call in both arms, `if (g == 0) f(d, 0); else f(d, 1);`.
- `xor dx,dx; mov dl,al; push edx` into util_Random(unsigned short) is `util_Random((unsigned char)cnt)` with the
  declared prototype (a cast to an int-parameter pointer gives `and eax,0xff`). A names-table pointer the
  original keeps in a slot and bumps by the row size beside the count is `strcpy(names[cnt++], one)` (VC6's
  strength-reduced IV; `strcpy(names[cnt], one); cnt++;` gives an offset copy instead) (init_NameBuildings).

## Control flow and returns
- **Empty case labels**: VC6 drops `case X: break;` (also `return;`, dead stores, `goto` to the join)
  before switch lowering, so those labels cannot affect the dispatch tree. A folded `if (0) { }` body
  keeps the label in the tree/tables and emits no code (d3d_Error: 75 empty D3DERR_* cases, matched
  12096 B; the tree, tables and bodies then line up).
- One `return` at the end (with `ret = x;` in each branch) is the common style: several `ret`s in the asm do
  not mean several returns. Early `return 0` shrink-wraps the callee-saved pushes; a return inside a branch
  puts them back at entry. Pushes after a null test: `if (!p) return 1;` plus one final return.
- AI decide callbacks with one shared `xor eax,eax` epilogue: nested `if (a && b ...) {...}` and one final
  `return 0;`; early-return guards copy the epilogue to each site (round 22, g1 / g3 / hv4).
- A statement or call written in both arms of an if/else is not neutral: argument pushes before the test mean
  the call was written in each arm (round 22, ai2).
- The out-of-line block is the `else` of an `if` whose test was written the other way round: swap branches.
- `while (1)` is not rotated, `for (;;)` is. A self tail-call becomes a loop. Write Ghidra's do-whiles as plain
  `for` loops; a counter unused in the body becomes a `dec; jne` countdown.
- An inner call as an argument pops its arguments at once; assigned to a local first, the pops merge.
- Source order of switch cases decides block layout. Empty cases are dropped (unsolved for d3d_Error).
- Repeated cleanup at each failure site (VC6 tail-merges them) often matches better than `goto fail`.
- **One failure block + epilogue per step in the original, one cross-jumped tail in ours**: look for a second
  copy of the success return. ls_SaveGame: `if (full) { ...steps...; close; restore; return 1; } close;
  restore; return 1;` keeps all 24 `close; restore; xor eax,eax; ret` copies (and the inline `f == 0` return);
  one shared `return 1` after the `if` lets VC6 merge them. Byte. Else-chains, `!`, `<= 0`, a result local,
  `__inline` helpers and `#pragma optimize` letters were all no lever.
- A ternary argument `f(t, c ? K1 : K2, 2)` gives `mov eax,K1; je; mov eax,K2; push eax`; the original's
  `push 2; push K1; jmp; push 2; push K2; push ecx; call` is the call written in both if/else arms (ls_LoadScene).
- A pointer loop compared signed (`cmp esi,END; jl`) is an index loop `for (i = 0; i < N; i++)` over arrays
  VC6 strength-reduced (ls_WriteEnvironment: four SVWEATHER hour arrays), not a pointer walk (that is `jb`).
- `int ret = 0; if (...) {...} else ret = -1; return ret;` gives a result kept in a callee-saved register.
- **Callee-saved push after the guards** (`push esi` at entry, `push ebx` only after two `return 1` guards,
  ours both in the prologue): a `return 2;` inside the first of two `if (m == 3 ..) {..}` / `if (m == 1 ..)`
  blocks with the same `return 2;` after them. `if {..} else if (..) ..; return 2;` (one shared return) put the
  push where the original has it: ord_WageReportOrder byte. Wrapping the rest in an `else {}` did nothing.
- **A result constant hoisted into eax in the prologue** (`mov eax,2` before the first test, the failure paths
  `mov eax,1` before the shared pops, `mov eax,2` again after a call on one path): `int r = 2;` with `else r = 1;`
  on each failing if, `return r;` at the end, and the path with the call returning its constant itself
  (`{ f(..); return 2; }`). Without that explicit return, r lives across the call in a callee-saved register;
  `r = 1` initial + `r = 2` at the join hoists the 1 instead (ord_DismissWorkerOrder draft 36 ins -> byte).
- **A result local that the original reloads from a stack home at a failure return** (`mov ebp,[esp+N]; mov
  eax,ebp`) where ours returns `xor eax,eax` from its own epilogue: VC6 folded `return f;` to `return 0` because
  only `f = 0` reaches that return. The original's failure path fell through to the final `return f;` (nested
  `if (e) { if (zip) {..} } else {..}` with one shared not-found statement after them, no `goto notfound;
  .. return f;`): then f keeps its home and the frame grows to the original's. vfs_Tfopen draft 152 -> shape 44.
  The success path that must skip the failure statement leaves with `goto done;` (done: before the final
  return); a missing-entry error is the `else` of `if (e) {..}` so both successes share the final return.
- `n > 3` and `n >= 4` compile differently (`cmp 3; jg` / `cmp 4; jge`): VC6 keeps the literal. `best <
  x` against `x > best` decides the fld / fcomp operand order and `test ah,5; jp` against `and eax,0x4100;
  jne` (hmt_DecideElection).
- **Two failure returns: separate `xor eax,eax` vs one shared** after the pops: one result variable set to 0
  before the outer `if`, assigned inside, one final `return type;` (no early `return type;` / `return 0;`) gave
  the original's two xors (ch_PlaceOnGround byte).
- **`xor ecx,ecx; mov cl,[m]; mov eax,ecx; shr eax,5; and eax,1`** (a zero-extended byte bit test returned)
  where every `b >> 5 & 1` spelling gives the trimmed `mov al,[m]`: write `if (b & 0x20) return 1; return 0;`.
  VC6 turns the if/return pair into the shift form with the zero extension (ch_AnimDone byte).
- **A redundant re-test kept by the original** (`test [m],0x20; jne call; cmp eax,ebx; jne skip; call:`, the
  third term of `!s || s->f || !s` that a condition threads away): write the if / else-if pair with the call in
  both arms, `if (s && s->f) { call } else if (!s) { call }`; VC6 merges the calls and keeps the test
  (ch_ActWalk byte). With a long arm VC6 does not merge the two copies (ch_ActWalkCells: 531 ins); a flag
  `r = 0; if (s && s->f) r = 1; else if (!s) r = 1; if (r) {arm}` (or a goto into the arm) keeps the re-test (byte).
- **A field the original reloads at a call where ours forwards the entry local** (`push edi` vs `mov ecx,[esi+0x14];
  push ecx`, ch_ActDroppingObject): any join before it with a call on the other path kills VC6's CSE of the load
  (proved with a shared test after the count==0 block), but no shape found that also keeps the original's tests.

- **An if/else whose else block the original lays out of line no matter how the condition is written** (every
  spelling of `if (!A || !B || !(d < c)) far else near`, && with near first, `>=`, reversed operands: one code,
  near inline): the source chose through a flag, `lowPoly = 1; if (A && B) { ..; if (d < c) lowPoly = 0; }
  if (lowPoly) far else near`; VC6 threads the flag away and keeps the far block first (ch_UpdateLowPoly byte).
- **Later tests that the original re-evaluates unthreaded** (`if (s && !x) {..return;} if (!s && x) ..; if (s &&
  x) ..` with the original re-testing s and reloading x per if, ours jumping straight through): write the later
  tests on the field (`ch->slot`) instead of the local copy s (ch_UpdateLowPoly 91 -> 78 ins).
- **A macro shaped `if (call == 0) return 0;` (LS_R) as the then-arm of an if/else** takes the else for its own
  if: the else ran after every successful read (ls_ReadSaveHeader, a meaning bug; asmdiff showed it as one
  `branch` hunk and a few `order`/`regs` hunks). Braces around the macro arm; grep for macro + `else`.
- **Callee-saved pushes after the early-exit guards in the original** (`push ebx` at entry, ebp/esi/edi only
  before the loop, `return 0` with only `pop ebx`, the loop test `dec; push..; test; jl` unfused): give the loop
  bound its own local (`top = 30 / 42; for (i = top - 1; i >= 0; i--)`) and leave the loop by `break` to the one
  `return n` (amt_GetColumnOffices 12 ins -> byte; `for (i--; ..)`, `goto none`, else-chains were no lever).

- **Several send sites that the original does not share** (three `Begin; r = Send(&m); End; return r;` with only
  the call tail merged): write each site out in full, not `goto send` - VC6 cross-jumps identical instruction tails
  after register allocation, so the per-site store order (`m.id` / `m.o1` first) decides which sites merge; a site
  that merges in ours but not in the original needs another store order there (amt_ApplyForOffice, 51 -> 14).
- **A capped count returned** (`cmp eax, ecx / jl / mov eax, ecx` with no slot store): `if (cnt >= n) return n;
  return cnt;`, not `cnt = n; return cnt;` (amt_GetOfficeCandidates / ...Full, the last hunk to byte).
- **A dispatch the original keeps both range compares of, its else part out of line after the return**
  (`cmp al,0x1b / ja far` then `cmp al,0x1c / jb / cmp al,0x22 / ja` down there): `if (k <= 0x1b) {..} else if
  (k >= 0x1c && k <= 0x22) {..} else {..}` falling into the shared tail, not `if (k > 0x1b) { if (k > 0x1b && k <
  0x23) {..; goto done;} ..; goto done; }` (amt_ExecuteOfficeMessage 133 -> 15).
- **A compare against a value the code just stored, with no reload** (`mov al,[kind] / mov cl,[applied] / cmp
  cl,al / mov [office],al`): compare with the stored field (`c->officeKind = kind; if (c->applied ==
  c->officeKind)`); VC6 forwards the store, while `== amt_Table[i].kind` reloads it after the byte store.
- **A bit set before the mask local in a tail-merged chain** (`or ah,2 / mov edx,0x4000 / jmp store`): write the
  field first (`c->status |= 0x200; m = 0x4000;`); `f = f | 0x200; m = ..; c->status = f;` emits m first.
- **A search loop whose hit block is inline after the loop's `jmp`** (`je hit / .. / jl top / jmp after / hit:`)
  where ours puts the hit block after the following code: the layout follows the register allocation; with a
  parameter forced to memory (a probe `{ static int *pm; pm = &mode; }`, never kept) the plain `for + if { hit;
  break; }` gave the original's layout (amt_Rating). Probe this way before trying loop spellings.
- **A status key test folded into one mask but the 0x200 key still stored** (`test ch,0x3c / jne / and ecx,0x200
  / neg / sbb / and ecx,0x23 / mov [dead slot],ecx`): write the whole five-way key chain for the second character
  too, like the first; VC6 folds the tests (amt_Rating).
- **`abs(a->rank - b->rank)` from two bytes** gives the original's `and edx,0xff / sub / cdq / xor / sub`; a
  `d = ..; if (d < 0) d = -d;` gives `jns / neg`, and a byte local for one rank goes to a stack slot where the
  original keeps it in cl (amt_ElectDeputies).
- **A zero register shared by two variables** (`xor ebx, ebx` stored to `cur`, then `ebx` is `best`): `best = 0`
  in an else arm of the `if` that computes it, not before it; before it VC6 gives the zero to the loop index
  instead (amt_CalcZuenfte).

## Registers, stack slots, locals
- **A local memory-homed in the whole original** (stored at every definition, dead final loop values stored,
  reloaded after byte stores through pointers) is address-taken: find the one small-frame `lea` in the original
  and pass that local there (txt_OpenText: `i = txt_Count - first; fwrite(&i, 4, 1, f)`, not `&lineno`: 1085 -> 424).
- **A range test `x >= 0 && x <= 9` splitting x into a memory home** (`mov [esp+N],edi / mov edi,[esp+N]` at the
  test, the original keeps x in one register): nested ifs, each with its own `x = -1` else arm (txt_Format
  703 -> 391; no lever in io_PrintText's identical parse).
- **A parameter loaded twice from its slot** (`mov esi,[arg]; and esi,0xffff` at entry, later `mov edi,[arg]` unmasked):
  two variables. The masked one is the parameter itself (`last &= 0xffff;`) or a walker copied from it, the second an
  uninitialized local that VC6 homes in the dead parameter slot, assigned on every path that reaches its use. A
  word compare (`cmp ax,0xffff`) on a loaded field is a comparison of the field itself, with no 32-bit local in
  between. path_Reverse: `last &= 0xffff; cur = last; while (cur ..) { if (nodes[cur].parent != 0xffff) {..; head =
  nodes[cur].parent; } cur = nodes[cur].parent; } n = head; while (n ..)` went from 62 ins to byte.
- **A local kept in a parameter's slot while the parameter's value lives in a stack copy** (`mov ebx,[arg]; mov
  [esp+N],ebx`, the slot later written with another value and returned with `mov ax,[slot]`): the parameter was
  reused as that other variable (path_Expand: `self = head; cur = &nodes[head]; head = cur->next; .. return head;`).
  It also brought the frame from 0x14 to 0x10. Return type follows the epilogue (`mov ax,[slot]` without `and` is an
  unsigned short return).
- **An `if (s == x) { if (g >= y) continue; if (s != x || ..) goto calc; ..}`** where the original compares s
  again after the float test: write two separate tests, `if (s == x && !(g < y)) continue; if (s == x && (f & 1))
  {..} else {calc}` (path_Expand, together with the reused parameter: 222 -> 64 ins).
- Declaration order of locals almost never matters; slot order follows first use / reference counts after
  optimisation. Tie-breaks: a dead early store (`z = 0;`), a real extra use, block scope, a split variable.
- Locals land in dead parameter slots; an uninitialized local read on one path looks like an extra argument.
- Table loops over a global array: use `arr[i].field` everywhere (no local copy of a field, no pointer local,
  one cast view per loop); the strength-reduced base follows the most-used field (rounds 20-21: 3352 -> 1414 B).
- Message-field blocks: statement order sets scheduling and register roles; brute-force it with `uv run try`.
- Frame 4/12/16 bytes bigger than the used locals: a `D3VECTOR4` local (x/y/z/w, 16 bytes), an array a callee
  fills, or `int x[3]` with constant indices (enregistered elements, full slot kept).
- `volatile` reproduces a value the original re-reads from memory (globals like `d3_Time`, locals, parameters).
- A global array indexed in every statement (`g[i].x = ..`) is reloaded after each store; a pointer local is not.
- One variable reused for unrelated loops / branches shares their stack slots: split it.
- Two address-taken out-parameters (`f(pos, &x, &y)`) in swapped slots: a temp `i = y + x * 8` used twice
  vs the expression written at both uses changes their reference counts; the inline form gave the original's
  order (ord_CountGridObjects draft -> shape). A pointer local (`mt = &desc.mats[mi]`) vs `desc.mats[mi].f` at
  each use also moves a large struct local across others in an ebp frame (d3iot_LoadBGF: +9 desc refs moved desc
  above two D3VECTOR4s): count the references before trading a pointer local for repeated field access.
- Pointer vs index loops pick different bases / induction variables: try `p[i].f`, `p++`, `i++, p++` vs
  `p++, i++`, and a local for a repeated pointer load (`k = info->keys`).
- **A row pointer at `buf + 1` with `p - 1, p, p + 1` operands** (rgb triples, `lea esi,[eax+1]`): the source
  was the plain index `&buf[(y * w + x) * 3 + i]` in two `for` loops; VC6 picks the middle element as the
  induction pointer itself. Hand-written `rp = buf + 1; p = rp; p - 1 ..` gets rebased (`dec esi`) and lands
  in another dead parameter slot (d2_Convert16ToRgb / d2_ConvertRgbTo16, both byte).
- **RECT fills before a Blt**: `left, top` stored first let VC6 add `sx += w` in place (`add esi, edi`); later
  stores give `lea edx,[esi+edi]`. A 720-order permute of the last six stores found the exact order
  (d2_BlitPartToDD: left, top, bottom, right / left, top, right, bottom), after a note had called order no lever.
- Inline-asm macro temps (D3_ZERO, copy macros): their declaration order decides stores and slots; asm forces an
  ebp frame. Fixed-point helpers are `__inline` functions with `__asm` bodies.
- **A frame one slot smaller, a local in a dead parameter's slot** where the original gives it its own slot (and
  zeroes it early, `xor edi,edi; mov [esp+8],edi` before the first parameter load is done): the local's init was
  written before the parameter's last use (`n = 0; w = 0;` at the top of the function), so the parameter slot was
  not dead yet (ord_LoadStatistics frame 0x18 -> 0x1c).
- **An "allocation only" escalation whose stores land in another local's slot**: read the frame offsets of every
  store before trying spellings. ord_AddGridAmount / ord_SubGridAmount stored the first util_Random(8) into x's
  slot (the &x later passed to ord_GetGridCell, a start cell for its failure) and added rec[3] (AIDEC.guilt, read
  from the record's frame slot), where ours added the random: a meaning bug, both draft -> byte once fixed.
- A strength-reduced record walk with `cmp ecx, end; jl` (signed) is an index loop `for (j = 0; j < N; j++)
  ARR[j].f`, not a pointer walk (`jb`).
- **A frame one small buffer larger than the original's**: look where the original's buffer addresses point.
  err_ExceptionFilter's 7-byte code window (`lea ebx,[text]`, `err_CheckCall(text + 7, ..)`) is the dead
  `text[]` buffer, not a `char win[7]`: dropping the extra array gave the original's frame 0x280.

- **Two `|=` on one byte folded into one or** (`b1 |= 0x14; b1 |= 8;` -> `or 0x1c`, any bitfield view the
  same) where the original does `or 0x14 .. or 8`: a store to another byte of the same struct between them
  (`f.w.c.b8 = 1;`) keeps them apart. A `memset(&f, ..)` plus a redundant `f.mid = 0;` stores the dword the
  original never stores (drop the field store: ch_ActTurn 108 -> 88). Long store blocks: deep-tools
  tryclimb.py (dcp try hill climb) matched ch_ActTurn's 17-store else block in 3 rounds.
- **A frame one slot larger where an address-taken out-param (`&sx`) shares the original's slot with a loop
  temp** (dcp frame: `shared in the original`): declaring it in the block that uses it gives the shared slot,
  but VC6 may reorder other equal-count slots then (ch_GetNewPath, open).

- **AI_ZERO through a slot** (`mov ecx, [ebp+0x10] / mov [ebp-0xc], ecx / mov edi, [ebp-0xc]` where the original
  loads edi straight from the parameter): `AI_ZERO_AT(param, size)` - AI_ZERO's `void *_zp = (dst)` copy takes a
  frame slot and shifts the others (amt_GetOfficeCandidates / ...Full).
- **A record walk based on the wrong field** (`[eax-4]` for kind from a cnt base): index `amt_Table[j].f` at
  every use instead of a record pointer local `p = &amt_Table[j]` (amt_GetOfficeCandidates).
- **A buffer at a dead array's slot** (the original's string target sits in `cand`'s bytes inside a block
  that returns): a separate block-scoped buffer, not the function's other buffer (amt_CalcZuenfte: `gb[64]`).

## Floating point
- Commutative operand order is rank-based, not source order: textual swaps rarely change anything. Ranks move
  with parentheses (`(p->x)` is not neutral), a dead store, a named local, or a `(float)` cast of a subterm.
- Chains: `r = A + B; r += C;` split statements fix association; `x = sqrt(..); x = x * k;` fixes scheduling.
- min()/max() macros (windows.h) keep the ternary's operand order and give fld/fstp copies; a plain `if` copies
  with integer moves. `max(0, x)` vs `max(x, 0)` give different compares: try both.
- Float literals give DWORD constants, double literals QWORD; `(float)sqrt(a) < b` pops before `fcomp mem`.
- `1.0f * x` folds away; a constant through a variable (`one = -1.0f`) keeps the multiply.
- `x * 1.0f` / `x + 0.0f` on a member at offset 0 forces fld/fstp instead of integer moves.
- **Float member copies through the FPU** (`fld [m]; fstp [local]`, the last store often sunk past the next
  call's pushes) where ours copies with `mov reg,[m]; mov [local],reg`: parenthesize the source,
  `v[0] = (bk->x0);` / `v.x = (cam->rotation.x);` (a macro's expansion in the original). Plain copies, casts,
  `+ 0.0f` at a non-zero offset, comma macros and D3VECTOR / D3VECTOR4 spellings all stay integer moves.
  rd_AllBauplatzToSupermap and ai_CampaignKey1 byte, he_FindNearestSpying draft -> shape (2026-11-10);
  sweep script: deep-tools parensweep.py.

## Types and bytes
- Byte read-modify-write (`or BYTE PTR [x],2`) = `unsigned char` bitfields; word ops narrowed to a byte =
  `unsigned short` bitfields; signed char `x == -1` gives `or bl,0xff`.
- A caller pushing a bare byte / word (`mov cl,[m]; push ecx`, no `xor`) means the callee's prototype takes a
  `char` / `short`. Wrong jcc or swapped `cmp` operands: `uv run try` a list of equivalent compare spellings.
- `push 0xff` needs an `unsigned char` parameter. A callee returning in al whose result is passed on is
  `unsigned int` (with `unsigned char` VC6 adds `and eax,0xff`).
- Copy vectors field by field (loads first, then stores); a struct assignment gives pointer copies.
- `memset(&s, 0, n)` vs separate `= 0` stores: separate stores keep a zero register live for the whole function.
- Inlined `strcmp("lit", buf)` is `repz cmpsb`; operand order matters.
- `movsx r,BYTE [m]; and r,imm; shr r,n` is `if ((char)x & imm) v = 1; else v = 0;` (every expression form
  narrows to a byte op). Functions defined earlier in the same file change register allocation of later callers.

## Link conventions (uv run link gate)
- One name = one address; `/* 0xADDR */` per declaration; reach struct members through the type.
- Original string / float text and bits; `__FILE__` via the file's first `#line N "D:\\develop\\..."`.
- .bss statics / globals need `= 0` to keep declaration order; far COMMON globals are tentative `T x;`.

## Data layout (from the containment checks, M.contain / D7 / D9)
- A unit's .rdata is its named constants, then the literals it is the first to use (COMDATs, folded across units):
  the bytes right after a unit's last constant are its literals. bio, first in link order, holds the shared ones
  (255.0f after ai_FloatOne). A constant the code loads from memory is another unit's `const` read through an
  extern (ai_CardBack in unk_4a7140), not a header `static const volatile`.
- Never `&X + N` past X: a constant of another object is its literal (`80.0f`), a field is reached through its
  pointer (`d2_LastWindowPtr->w`), several globals one memset clears are one object (EVTGLOBALS, int[4]), flag
  bytes after a byte are objects of their own, an int at an odd address is a packed struct's field.
- A memset over several globals rewritten as single stores costs the byte match: declare the block as one object.
- unk_575860's ai_TutorialInitChapter2 / 3 flip their register tie-break on the unit's declaration set alone: a
  struct of a new name may cost Chapter2 its byte (keep the container's symbol); the build clears Chapter3's
  @deep note whenever it comes out byte (put it back from git when it falls).
- **A success flag the original keeps in a callee-saved register** (`xor bl,bl` at entry, every failure exit
  `mov al,bl`, success `mov al,1`) where ours folds the failures to `xor al,al`: the failures were not early
  `return`s but nested `if (info && info->mesh) { .. if (box) { ..; ok = 1; } }` with the out-parameter stores
  under `if (ok) {..}` and one `return ok;` (a `goto done` to that return gives the same code). d3_GetBounds
  15 ins -> byte; early `return 0` / `return ok` forms all fold.
- **An atan2 / sin result the original keeps on the FPU stack while also storing the float** (`fld st(0); fst a;
  fstp global; fld st(0); fsin; fld st(1); fcos`, later `fchs` of the same register value) where ours stores
  the float and reloads it for each use: the value was a `double` local (`ta = atan2(..); g = a = (float)ta;
  sin(ta) ..; na = -ta;`); a float local is reloaded from its slot whatever the spelling (d3_SetupFrustum,
  108 -> 98 ins, still stuck on a float `s` the original keeps in memory).
- **A strength-reduced loop pointer based on a field** (`mov esi, arr+0x25; mov eax,[esi]; mov ecx,[esi-0x25]`)
  where ours bases it on the element (`[esi+0x25]`, `[esi]`): VC6 bases it on the most-referenced field. Reading
  `arr[i].state` at each of its three uses instead of through a local (3 references against the id's 2) gave the
  original's base (gv_SendCrimes byte).
- **A loop pointer that should take a dead register**: `t = s->tris;` once before an `if (hw) for (..) else for (..)`
  kept t in a fresh register; `for (t = s->tris; ..)` in each loop's init let VC6 give t the dead surf register
  as the original does (d3_DrawObjectCB 122 ins -> byte, the M8 load-order crossing went with it).
- **Inline failure blocks cross-jumped into the shared fail tail** (`d3sm_Destroy(g); return 0;` x3): a success
  flag for the last tests (`ok = 0; if (a) { if (b) ok = 1; } if (!ok) { fail: ..; return 0; }`) gave two of the
  three blocks their own epilogue (rd_BuildRoad 97 -> 68 ins); a second copy of the success return (the
  ls_SaveGame lever) compiled to the same code there.
- **Loop pointer bias from a sub-record pointer**: a slot loop writing every field through `e->` got VC6's bias on
  the second field it references (`edi = slot+0xc`; probes dropping the first k writes moved it to the next field
  each time), the original's on `parts[0].v2` (+0x38). A pointer per sub-record (`p = &e->parts[k];` then
  `p->v0 ..`) gave the original's bias with no other change (gv_WriteEffects byte; dcp try shows gv_ReadEffects byte the same way). Spellings
  of the same address (`(e + i)->`, `&a[0]`, casts, struct-typed vectors, loop forms) never move it.
- **A small count array the original kept in the frame**: `int n[4]` with constant indices (VC6 keeps n[0], n[1] in
  registers, n[2] in its slot, n[3] unused) gave the original's 0x10 frame with holes where three scalars gave
  `push ecx` (gv_GrowGraveyard 60 -> 8 ins).
- **A cast on one side of a compare reorders its operands' loads**: `year % 4 == (int)(c->netId & 3)` loaded netId
  first; without the cast VC6 evaluated `year % 4` first like the original (gv_GraveyardTurn, the last hunk).
- **Float product order**: the original's `fild b; fmul 1/255; fmul table` came from `f * (table[k] * (b / 255.0f))`;
  `f * (b / 255.0f * table[k])` emits `fmul table` first (gv_GraveyardTurn).
- **Unrotated wait loop**: `while (1) { if (cm_GetSlotState(h) != 0) break; pump(); }` keeps the test at the top
  (`jmp` back); `while (!..)` and `for (;;)` rotate it. The earlier tag had tried `for (;;)`, not `while (1)`.
- **A 64-bit constant the original keeps in two callee-saved registers** (`mov esi,0x807d4190; mov edi,1` at entry,
  `push edi; push esi` at both calls) where ours pushes immediates: a local `__int64`, two int locals, a struct of
  two halves and every constant spelling are propagated; a union set by halves (`union { __int64 all; struct {
  unsigned lo; int hi; } h; } mask; mask.h.lo = ..; mask.h.hi = ..;`, calls pass `mask.all`) gives the registers
  (ai_DecideBuildingResident byte; he.c's 0xa7f4c3a0 stuck item may be the same).
- **The scan's "found" taken inside the loop**: `o = 0; while (n) { if (match) { o = &a[i]; break; } ..} if (o == 0)
  return 0;` drops the second `test n` / return-0 block that `break` + `if (n == 0) return 0;` leaves.
- **A loop index set before an early-return test**: `j = i + 1; if (hand[j] < 0) return best; for (; j < 5; j++)`
  builds j in its own register at once (`lea ebp,[ecx+1]`) where `for (j = i + 1; ..)` copies it (ai_PokerHighCard).
- **Copy a 12-byte vector out of a record field by field** to keep the record pointer out of a scratch `lea`: it
  moved `shot` into ebx pushed late, as the original (gv_ExplodeShot, regs 30 -> 14).
- **Switch cases that each push a string to one shared sprintf tail** (`push str; jmp tail` per case, one
  `lea ecx,[esp+..]; push ecx; call sprintf`): write the statement after the switch into every case
  (`sprintf(name, "x"); ctx[0x85] = out->ref; break;`), as the sibling callback does; with it after the switch
  VC6 schedules the buffer lea into some cases and cross-jumps only part of them (ai_CheckVisitAction, 38 ins
  -> byte). Varying the case order, `&name[0]` or a `char *s` with one sprintf did not reach it.
- **A slots-only diff can be a data-flow bug**: ai_TrustedOfTwo was 142 vs 142 instructions with every
  difference a stack slot, classed allocation; mapping each fstp / fld to its argument slot (after the args
  went into registers) showed the item bonus added to the other pair of values (the likings, not the
  skills) and the second abs over other operands. The meaning fix was byte. Map every slot of a float
  function whose locals live in dead argument slots before calling it allocation.
- **A byte local in a byte register** (`mov dl,[esi+0xb5]; cmp dl,5`): when ours gives it bl and spills an
  int that wanted ebx, widening the local to int / unsigned short fixes the frame but not the byte compares;
  the table pointer (esi in the original, edx in ours) decides which byte register is left (ai_PokerPickStrategies,
  stuck).
- **`xor eax,eax` hoisted before a switch's dispatch** (the default's return 0 preloaded, the index register
  modified in place: `add edi,-0x12; xor eax,eax; cmp edi,0x25; ja <after the xor of a shared return-0 block>`):
  the cases assigned a result, `r = 0; switch (k) { case X: r = f(..); break; .. } return r;` (VC6 still gives each
  case its own epilogue). Several failure `return 0`s that jump into one shared return-0 block came from an
  `if (kind == 0) {..} else if (kind == 0x10) {..} else return 0;` chain (not `else { if (kind != 0x10) return 0; ..}`).
  A coin flip whose arms are threaded (`test n1; je; test n2; je side0; random; jne side1`) is
  `if (n1 != 0) { if (n2 != 0) side = RandomU(2); else side = 0; } else { if (n2 == 0) return 0; side = 1; }`, and a
  count initialised inside the block that uses it (`n1 = 0` in the kind==0 arm) took the const-0 register as the
  original. ai_DecideLeisureAction 288 ins -> byte; its 5-case sibling ai_DecideVisitAction stays 115 (n2 vs in).
- **Identical `store; return 2` tails merged into one block** where the original keeps an epilogue per site: VC6
  cross-jumps equal statement pairs (also in break-form cases); writing some sites as `w = 2; x = kind[w]; return w`
  keeps them apart, but the copies then reload kind[2] instead of the register-cached value (ai_PokerShowdown
  230 -> 195, stuck).
- **Find-then-copy with `cmp i,N; jl <copy>` and a second `xor eax,eax; ret` after the loop's own `return 0`**: the
  original is `for (i = 0; i < N; i++) if (t[i].k == k) break; if (i >= N) return 0; *out = t[i]; return 1;`.
  VC6 threads the loop's normal exit to its own return and keeps the recheck on the break path; the in-loop
  `if (i >= N) break/return 0` forms merge the returns (amt_GetFirstOffice, amt_GetOffice: byte).
- **An out parameter that the original spills to its home slot at loop entry (`mov eax,[out]; mov [out],eax`) and
  bumps by the record size after the copy**: write `out[n] = rec; n++;` instead of `*out = rec; out++; n++;`;
  VC6 strength-reduces the index into the spilled pointer (amt_GetColumnOfficesOwned 28 ins -> byte).
- **An else-if chain that re-tests a byte (`test cl,cl; je`) with its health in `dl`**: the condition was written
  flat, `if (a && a->h > 0 && b && b->h > 0) .. else if (a && a->h != 0) ..`; the second test's `je` (not `jbe`)
  is the `!= 0` spelling. Wait loops `while (r != -1 && cm_GetSlotState(r) == 0) sv_PumpNet();` stay unrotated
  (amt_CheckOfficeColumns 16 ins -> byte).
- **Strength-reduced table-loop base on a field (`mov ecx, tbl+0x14`, `[ecx-0x10]` = owner)**: VC6 bases the IV on a
  field by the ORDER of the references in the body, not only their count. Writing the shared `x14 = -1` first in each
  arm (`if (owner == id) { x14 = -1; owner = -1; ...} else if (x14 == id) { x14 = -1; state = 1; }`) moved the base
  from owner to x14 and VC6 still tail-merged the store after the arms (amt_ClearOffices, amt_ClearOfficesIfAll:
  byte). Test such rules on a toy loop first (deep-tools/toyasm.py, 0.2 s): the toy reproduced both bases.
  A save loop `for (p = tbl; (int)p < (int)&tbl[N]; p++)` based on kind (+8) where the index loop
  `tbl[i].field` gives the original's owner (+4) base (amt_SaveAemter byte): retry the index form even when a tag
  says it was tried (it was tried before the early-return shape was fixed).
- **A global read once before a loop (`mov si, word [g]` before the loop, `cmp si, 0xb` inside)** with a local
  `d = g;` gave a dword load (`mov esi, dword [g]`); reading `g` at its use lets VC6 hoist it itself, as a word.
  An unused-after-hoist local also cost an extra register (amt_ClearOffices).
- **`add esp, 4` after every call and a value pushed before the next call** (`and eax,0xffff; add eax,esi; push
  eax; push 0xc; call`): the calls were nested in an argument list (`f(g(12) + 1, g(n) + base)`), not separate
  statements; an unmasked `add eax, esi` after a 16-bit roll is the int-returning callee (amt_CreateApplicant byte).
- **An inline-asm clear whose `mov edi` / `mov ebx` read two locals' slots directly** (no `_zp` / `_zn` temp slot,
  the 0x200 stored once into the size local, the frame 4 bytes smaller): write the asm block with both locals named
  (`mov edi, n2` / `mov ebx, n1`); AI_ZERO / AI_ZERO_AT keep a temp for one of them (ai_TokenMoney byte).
- **A message fill whose original hoists the clock's first dword above the memset and stores the copy a,c,b,d**:
  the struct copy `msg.time = ai_Time` came after the scalar fields (type, to, fromId, flags, arg), not after `to`;
  a 720-order `permute.py` sweep found it in seconds (ai_CommandFest, ai_CommandRiot byte). A byte store where we
  write a dword is a byte field of the union (the order alone did not match with the dword).
- **lea operand order `[esi+edi]` vs ours `[edi+esi]` for `i = (step + i) % N`** does not follow the operand order
  in the text; it follows which variable was defined first in the loop body (toy: `i = ..; step = ..` flips it). In
  ai_TokenRandomPlayer the original's outer loop was `for (i = 0; i < 0x300; i++)` around a search that reassigns i
  (the dead `i = 0` vanishes, `inc edi; cmp; jl` stays): that matched byte where `do .. while (i + 1 < 0x300)`
  compiled the same except the lea.
- **A tail that reloads a pointer from its slot (`mov eax, [ebp-4]` before a cross-jumped shared tail) and an entry
  store `[ebp-4] = 0`**: one variable for both arms (`AICHAR *c = 0;` searched into in one arm, `gm_FindChar` in the
  other), not `pick` + `c = pick` (the same family's ai_TokenRandomNpcInhabitant has this form). `while (n != 0)
  { ..; n--; i = ..; }` gives the `dec; test; mov; jne` bottom test, `for (..; n != 0; n--)` drops the test.
- **Two pointers merging at a search's exit (`lea eax,&a[i]; test eax,eax; jne` from the break, `mov eax,[pick]; test
  eax,eax` from the exhausted loop, sel 0/1 breaks jumping into that same test)**: `c = 0;` at the top of the search
  arm, every break sets `c`, the two-pass loop keeps its first candidate in `pick` and ends `if (c == 0) c = pick;`;
  VC6 threads each exhausted loop (c known 0) to a plain `return 0` and the human break into the test. Writing `c = 0`
  at the declaration gave c a callee-saved register for the whole function instead. Six ai_Token* siblings byte.
- **The clear's size slot stored before the destination slot (`mov [ebp-8], 0x200` then `mov [ebp-4], eax`)**: assign
  the destination inside the clear's block after the size temp (hst.c `AI_ZERO_BUF(n1, buf, 0x200)`); `n1 = buf;
  AI_ZERO_AT(n1, ..)` stores n1 first.
- **`test cl,cl; jbe` on an unsigned byte field** is `x <= 0` / `x > 0` in the source, `je`/`jne` is `== 0` / `!= 0`
  (same meaning; also inside a shared macro: G12_TITLED's `officeKind > 0`).
- **Strength-reduced loop base on a byte field read through a local `g = rec[i].job`**: the original read the field
  at each of its 8 tests (a macro): the base moved from netId (+4) to the job byte (+0x164) and the function matched
  (ai_TokenRandomMerchant).
- **A pointer parameter reloaded from and stored back to its own slot (`mov edx,[ebp+8] .. mov [ebp+8],edx`) before
  a loop**: the original wrote through a local copy (`s = src;`) that VC6 packed into the dead parameter slot; where
  the copy is assigned matters (after `p = buf;`, not at the top). hst_ParseText_Fake_2nd_Pass 120 -> 10 ins, then
  byte with `dashes++;` before the `*w++ = '-'` store (hst_ParseContext has the same copy).
- **A search loop whose found path skips the "not found" test (`je` straight to the strlen) while the exhausted loop
  falls into the error push**: the found work (strlen, the store) sat inside the loop before its `break`, and one
  shared `if (k >= 15) error` followed (VC6 threads the exhausted loop, k known 15): hst_ParseContext byte.
- **`lea ebx,[eax+1]` where ours has `inc ebx` on a pointer parameter**: `memcmp(fmt + 1, ..)` rather than `fmt++`
  (ai_CommandExecution).
- **The loop base on netId with three uses**: reading `ai_CharsT[i].netId` at each use (test and both stores)
  rather than through `id` moved the base from officeKind to netId (VC6 still keeps one register copy).
- **Every failure as an early `return 0`** (ai_TokenRichestInhabitant) let the mode-1 path's success jump into the
  shared format tail past the `mode == 0` store, as the original's `ja` does; nested `if (n2 == 1) { if (sel ..) {`
  kept its own copy.
- **Two adjacent byte locals right next to a big array, with unused slots after them** (`[ebp-0x28]`/`[ebp-0x27]`,
  a 12-byte gap): one record, not two chars. hst_ParseDate's day/month were a GMDATE (`char day, month; short
  year`: the year's `movsx` showed the short); framesearch.py with `struct {..} dt` hypotheses found the slot in
  seconds, then byte with `mode` as an int (which dead parameter slot a later macro temp takes).
- **An inline-asm macro temp loaded from a parameter and stored back** (`mov ecx,[name]; mov [name],ecx`) where the
  original has `mov edi,[ebp+0x14]`: the asm read the variable itself (`__asm mov edi, dst`), no `void *_cp = dst`;
  for a global, `__asm mov edi, OFFSET dst` (hst.c G12_COPY2_V / G12_COPY2_G).
- **A spilled float `fstp g; fld e.demand; fmul g` where the original keeps it (`fst g; fmul e.demand`)**: no named
  g - write `f + 1.0f` at both uses; VC6's CSE temp gets the original's slot (ai_CommandDemand). Likewise an int
  CSE temp (`w - x0` at both uses) instead of a `lim` local (d3s_DrawSpans).
- **A hoisted `neg w` and an extra incremented frame slot in a count-down loop**: the original tested
  `row - j + y0 > 0` and indexed `j * w` directly; VC6 made both induction variables itself. Separate `e` / `i`
  locals with their own `++` / `-= w` give other code (d3s_DrawSpans, byte).
- **`lea edx,[edi+edi]; imul edx,ecx` vs ours `imul; lea [eax+edx*2]`**: every `h * 2 * w` spelling folds to the
  same code; `(h << 1) * w` gives the original's (d3s_AddShadow).
- **Pooled literals**: all three `1.0f`s of d3s_AddShadow were the shared ai_FloatOne, but the M10 named only the
  first two until those were written as the object; check the whole function after each M10 fix.
- **VC6's own reciprocal is not a source literal**: for a loop-invariant divisor VC6 turns `x / inv` (and
  `x * (1.0f / inv)`) into `fdivr [1.0f]` hoisted plus a multiply; in this binary the pooled `1.0f` / `1.0` sit
  at the addresses named ai_FloatOne (0x5eb780) / cm_DoubleOne (0x5eb8a8), so the M10 on such a site (and on
  a `+ 1.0` double rounding) has no source spelling: writing `ai_FloatOne / inv` adds an fmul, a hoisted
  `inv = ai_FloatOne / -y` local spills (d3s_Project). Leave these M10s and say so in the tag.
- **Struct assignment vs field copies**: `slot->dir = *ldir;` compiled to a pointer temp and a different
  frame; `slot->dir.x = ldir->x; ...` per field gave the original's interleaved loads and frame size and
  halved d3s_Project's diff (282 -> 139 ins). Try it on any `a = *b` of a small struct.
- **Running min in a local, bound in the parameter**: `size <<= 16; ymin = size;` (the min loop on the
  local, the bounds check `> size - 1` on the parameter) made VC6 hoist `size - 1` out of the bounds loop and
  keep the min in a register as the original does (d3s_ShadowPoly draft -> shape); the reverse roles kept the
  bound in a register.
- **Record pointers advance per record**: `p->uv = t; ... t[0..5]; p++; t += 6;` for each of two polygons, not
  `p->uv = t + 6; t[6..11]; ... t += 12;` (the original's `add ecx, 0x18` per polygon; d3s_MeshFromMap 247 ->
  181 ins).
- **Read pointer-indexed loads against the IV base**: in a strength-reduced loop the original's `[eax-0x50]`
  with eax = &row[i + 1].pos is row[i].pos, not row[i + nx + 1]; d3s_MeshFromMap had the wrong vertex in a
  height read (a meaning bug that an earlier "allocation only" reading missed).
- **Still check line 211 first on f3init-style drafts**: three of four f3init deep items (session 46) were decompiled
  pointer walks (`(int)p < (int)END`, `p[-10]` / `*p` against a base `T[0] + 0x228`); the index loop took
  init_LocalData and init_LoadShopScript to byte and init_AllocSceneBuilding 41 -> 4 ins. Rebase the fields on the
  IV: `T[0] + 0x228` with a record of 0x21c is record 1 + 0xc, so the loop starts at i = 1 and reads the byte +0xc
  (the faith), not +0x228.
- **Two copies of a call's argument pushes, one jumping into the other's call** (`inc eax; push eax; push eax; ..;
  jmp call` and a second `inc eax; lea ecx,[buf]; push ..` falling into it): the source had two calls, one per arm.
  With a local initialised 0 at the top and read from its frame slot on one path (`mov eax,[esp+0x10]`), write
  `goods = 0; if (home) name = rec; else goods = ..; if (goods == 0) f(name..); else f(goods..);` - the arm that
  does not assign name keeps name out of a register and frees one for the parameter (init_LoadShopScript, byte).
- **Unsolved: `movsx ecx, byte [o+0x4b]` scheduled before an unrelated `and`, then `cmp ecx, 2`** where every char
  spelling gives `mov al,[..]; cmp al,2` (or `cmp byte [..],2`): casts, `>> 24` on int / unsigned, bitfields,
  static const / enum / local constants, int and char locals, `?:` and flag forms all narrow; only arithmetic
  (`k - 2`, `k ^ 2`, `switch`) keeps the int, with sub / xor instead of cmp (init_AllocSceneBuilding).
- **A pointer tested twice on one path** (`cmp eax,ebx; je ret` and right after it the common `cmp eax,ebx; je ret`,
  the other paths threaded past one or both): the first test read the global the pointer was loaded from
  (`if (ai_FallbackBuilding == 0) return; b = (AIBLD *)ai_FallbackBuilding;`), so VC6 does not see the two tests as
  one; an arm laid out after the else arm was written second (`if (ai_PlayerBuilding == 0) {..} else {..}`)
  (sv_ResetSelection, 3 ins -> byte). The same `b == 0` test in both places is always merged.
- **Exits of a two-branch loader** (init_LoadBuildingScene, 123 ins -> byte, the init_LoadObjectScene pattern): a
  failed load's `d3if_SelectScene(save, 0)` written in its own `if (load == 0)` arm of each branch, and the early
  exits (new-slot success, clean slot) with their own copy of the clock tail + `return;`: the pops then land after
  the shared tail and the cross-jumps (push 0 hoisted above the je) come out as the original's.
- **A constant table of 50 ints indexed `t[lvl + 0x1e]` / `t[lvl + 0x28]`** was three arrays (`t[30]` ages,
  `lv[10]`, `money[10]`): the frame put ids at the original's slot and the code lost 28 ins
  (init_CreateStartCharacters). Local names never break ties of the frame's references-per-byte order
  (deep-tools frameorder.py).
- **A null test of an array element's address** (`lea eax,[edx-4]; test eax,eax; je` inside a loop over
  `&T[i]`): VC6 folds `s != 0` when s is the pointer the other operands go through (`s = &T[i]; if (s->obj != -1
  && s != 0 && .. s->obj ..)`, also as pointer walk / casts); it keeps the test when the other operands index the
  table directly (`s = &T[i]; if (T[i].obj != -1 && s != 0 && .. T[i].obj ..)`), with an @original comment
  (sv_HandleRightClick, 3 ins -> byte).
- **A global carried in a register into a later arm** (ours reloads `mov ecx,[depth]` on one edge of a join so the
  pop arm can use ecx; the original loads it inside the arm into eax): the join's arms were written another way.
  `if (top == cb) r = 1; else if (pop != 0) r = 0; else { r = 1; for (..) if (..) { err(..); r = 0; break; } if (r)
  {push; goto l64;} }` instead of one else with `goto l21` for both r = 0 paths (sv_RunFrame, 10 ins -> byte).
- **Unsolved, `mov ecx,[eax+0x1cc]; test ecx,ecx; .. mov eax,ecx; mov ecx,8; mov eax,[eax]`** (info kept apart
  from the walking pointer v = info->keys, sv_UpdateBand and sv_UpdateBandLed alike): every loop form, index form,
  info/v merge, test placement and an __inline helper gave the control; and `mov ecx,eax; add ecx,ebp; mov
  cl,[ecx]` for `ai_GoodsT[id].type` (sv_UpdateGroundplan / sv_LoadGroundplan) never comes from the load's spelling
  (toy files in deep-tools toyasm.py runs always give `[eax+ebp]`). Lead from sv_HandleObjects: there the
  materialized `add eax,ecx; cmp byte [eax],0x1d` came from the pointer assigned *before* an unrelated store
  (`cls = &ai_GoodsT[id]; sv_SelectedModel = ..; if (cls->type == 0x1d)`), not after it; try the statement order
  around the load before its spelling.
- **A frame 16 bytes larger with one used dword at +4 of the extra space** (dcp frame: the original's slot above
  the last array, holes either side, while ours shares that value's slot with another temp): the value was y of
  its own padded vector, `float up[4]; up[1] = pos[1] - cen[1]; .. + up[1]` (constant-index arrays are split into
  scalars, the full slot kept). With the view stores in rotation-then-position order (720-order permute)
  sv_FlyCameraToObject went 12 ins -> byte; the same frame gap is open in sv_CameraToObject.
- **Base induction pointer on the wrong field of a record loop** (`mov ebx, T+0xc` (name) where the original walks
  `T+8` (ch)): VC6 strength-reduces on a field the loop references often. Reading the record field again in the
  tests instead of the local copy (`ch = T[i].ch; if (ch == 0 || ((C *)T[i].ch)->obj->scene ..`, three of five
  uses; a 31-mask generator found the subset) moved the base to ch: sv_DrawBubbles 13 ins -> byte.
  Same lever, session 51: sv_DrawNamePlates (any one of four uses of the plate's obj re-read: 19 ins -> byte) and
  sv_UpdateChimneySmoke (no record pointer local at all, `ai_CharsT[i].f` in every test: 21 ins -> byte; it also
  freed the 0xffff register and gave the original's `mov bl,0xa`). Try it first on any walk biased by a field offset.
- **An else block the original places out of line after a later switch dispatch** (`if (v == 0xb || v == 0xc) {
  plain = 0; k = R(); if (k == 3) k = 4; } else k = R();`, the plain arm sitting between the `jmp [table]` and the
  first case): the flag was set first and tested in its own if/else, `if (v == 0xb || v == 0xc) plain = 0; if
  (plain) k = R(); else { k = R(); if (k == 3) k = 4; }` (VC6 threads the known flag and moves the copy). One
  merged `k = R(); if (!plain && k == 3)` does not thread. An `id = -1;` scheduled before a block-scope
  `char buf[N] = ""` fill's last stosb was written before the block's `{`, not after its declarations
  (sv_SpeakPicked 18 ins -> byte).
- **A signed `cmp [rec],reg / jge` against the unsigned d3_Time**: the original compared an int copy, `int now =
  d3_Time;` declared in the block (`(int)d3_Time` in the test fixes the jump but hoists the load and keeps a
  register for the field; the block-scoped local gives the original's load inside the block, sv_ShowBubble byte).
- **The same -1 stored in several tails, the original rematerializing it (`or ebp,-1`) after ebp served as a
  pointer, with one store as an immediate**: one shared `W = -1;` after if/else arms without returns (`if (..) {
  .. if (c) {if (!fb) T = f();} else T = -1; } else { A = -1; T = -1; } W = -1;`): VC6 copies the shared tail into
  each path. Returns per path and every store order (72 variants) and chained `a = b = -1` kept the control. A ?:
  text argument whose original pushes per arm is two calls (sv_OpenBuildingPanel, 17 ins -> byte).
- **The record-field re-read also works in arithmetic, not only in tests**: `h = (char)(v / 60); T[i].hour = h;
  m = (char)v - T[i].hour * 60; T[i].sec = (m + T[i].hour * 60) * 60;` (two of three uses of h re-read from the
  field just stored, deep-tools subsets.py) moved the minute value to edi and m to bl as the original, with ebx
  saved (sv_FillDayPhases 40 ins -> byte).
- **Two global webs in swapped scratch registers** (the hour in eax and sv_SunRaysHour in ecx where the original
  has the reverse, through every block of the function): write the global's store in both arms of the preceding
  if/else (`if (..) { f(); G = H; } else G = H;`, tail-merged into one store): the extra reference re-ranks the
  webs (sv_WeatherFrame 47 -> 11 ins). The same store after the if, or the -1 store in both arms of the other
  if, kept the control.
- **`shl eax,2` shared by two loads `[eax+A-4]` / `[eax+A]`** where ours CSEs the address `lea r,[e*4+A]` and
  reads `[e*4+A-4]`: the two reads had different symbol bases. `T.a[h - 1]` came from the global defined just
  before T, `((unsigned *)&prev)[h]` (every spelling of `a[h - 1]` on T's own base = control; @original comment).
  sv_WeatherFrame 11 ins -> byte.
- **A top-tested outer loop after a null test** (`cmp y,[surf+8]; jge` at the head, `jmp head` at the bottom,
  the cached zero re-zeroed at the exit): `for (y = 0; surf != 0 && y < surf->h; y++)`, the loop-invariant test
  in the condition (vc6-matching.md line ~317); `y = 0; if (surf) while (y < h)` is rotated (sv_LoadGroundplan
  49 -> 4 ins). `mov ecx,eax; add ecx,ebx; mov cl,[ecx]` for `ai_GoodsT[id].type` (ours `mov cl,[eax+ebx]`)
  is still open: no spelling, local, type or test form moves it.
- **The sv_FlyCameraToObject `up[4]` lever transfers**: sv_CameraToObject (frame 0xc short) took the same
  padded `up[1]` for the height above the centre, the datum cm_EffectRadiusScale for its M10 literal 1.8, and
  the view stores in rot 0,1,2 / pos 0,2,1 order (720-order permute): 40 ins -> byte.
