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

## Registers, stack slots, locals
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
- Inline-asm macro temps (D3_ZERO, copy macros): their declaration order decides stores and slots; asm forces an
  ebp frame. Fixed-point helpers are `__inline` functions with `__asm` bodies.
- **A frame one small buffer larger than the original's**: look where the original's buffer addresses point.
  err_ExceptionFilter's 7-byte code window (`lea ebx,[text]`, `err_CheckCall(text + 7, ..)`) is the dead
  `text[]` buffer, not a `char win[7]`: dropping the extra array gave the original's frame 0x280.

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
