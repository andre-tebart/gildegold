# VC6 SP5+PP /O2 – quick reference

The rules that paid off most often, condensed from notes/vc6-matching.md (rounds 1-14). That file has the
details and per-function attempts: search it by keyword (`grep -n "D3VECTOR4\|dead parameter" notes/vc6-matching.md`)
when a function resists, instead of reading it whole.

## First rule: the original was ordinary code
The stuck functions solved on 2026-11-14 all had the same history: an earlier pass had reached its best
distance with an unusual construct, and the byte match was plainer code than that. Before trying more variants
of a stuck function, list what in it a 2001 game programmer would not have typed, and replace that first:
- `ret = -1; goto end;` at every failure -> each step under `if (!ret)`, one `return ret` (sb_Play).
- `goto off` to one shared stop block -> the block written out at both exits (d3sndw_Update).
- a `volatile` view added to force a reload -> the plain field (di_UpdateButtons).
- `memset(a, 0xff, n)` of an id array -> `for (i = 0; i < 8; i++) a[i] = -1;` (ahm_CreateAlchemist + 3 more).
- two arrays and a fallback assigned in the "wrong" branch -> one array reused; the case excluded in the `if`
  condition (lobby_ChoosePlayer, snd_PlayFavourComment).
- a hand-advanced byte offset `off += sizeof(T)`, `cl[k - 256]`, error locals stepped mid-loop -> the plain
  index and the library's own statement order (qnt_Map, DL1).
- three globals where the algorithm has one table -> `pal[3][256]` (qnt_Nearest).
Why it goes wrong: a trick usually fixes the hunk it was aimed at (the distance drops, so it is kept), but it
changes what VC6 sees elsewhere - another block layout, one reference fewer in the allocator, another operand
rank - and the remaining hunks then look like "pure allocation, no source lever". A small remaining distance
behind an odd construct is a local optimum, not a near miss. The one counter-example so far is mild:
d3io_AttachStock needed the same expression written out twice.
In the deep queue (1016 functions) 91 contain a `goto`, 7 a `volatile`, 16 raw `(char *)p + n` arithmetic.

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

- **A row pointer the original recomputes every row** (an `imul` per row behind the inner loop's guard, ours a
  running pointer plus a stride slot): compute the address in the inner loop body (`p = pix + ((r + y) * pitch +
  x + c) * 3;` / `src[(y + i) * pitch + x + j]`); VC6 hoists it to the inner preheader behind the guard and the
  outer loop never strength-reduces it (shp grabbers 183 -> 6, 274 -> 34 ins). A packed struct word at an odd
  offset (D2KEY.gb) is split into byte loads by VC6 where the original loads the word: open data question.

- **A dispatch on a byte field written twice** (`mode = s->mode;` then `if (mode == 0) .. else if (mode == 2)` in two
  if / else chains, the original `test edx,edx; mov dl,[s+0xd]; jne` with the zero constant sharing edx): no local,
  `s->mode == 0` in both chains (d2_ShpBlit16Rows 159 -> 8 ins). A `w` local copied into `vis` lost the original's
  frame: read `s->w` where w is needed once, use `vis` in the clip code.
- **A skip loop over a hand pointer** (`p = pix + t + col + x; do { ..; p++; } while (*p == key);`) whose original
  sets its pointer up after the entry `jne`: the index form `while (pix[t + col + x] == key)`, VC6 makes the pointer
  itself (d2_GrabShape16Runs 68 -> 18 ins).
- **`movsx` of a short field pushed to a short parameter, the call in both arms**: one call after an int local
  chosen by if / else (`y = ..; else y = w->y; f(x, y, ..);`); VC6 duplicates the call tail (d2_PlaceWindow byte).

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
  Same for a guard + search loop (`mov eax,1` before the guard's test, guard and hit sharing one `xor eax,eax; ret`):
  `r = 1; if (g == 0) r = 0; else { do { if (hit) return 0; .. } while (..); } return r;` (ai_HasQuicksavePlayer
  29 ins -> byte; `if (g == 0) return 0; .. return 1;` and `r = 1` with early returns kept the late `mov eax,1`).
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
- **A loop the original does not strength-reduce** (`lea ecx,[i*8]; sub ecx,i; lea edx,[ecx*4+0xa]; push edx` per
  iteration, ours an IV `add esi,0x1c` in a slot): the argument goes to a `short` parameter. A unit-local
  prototype with `int` parameters (menu.c's d2_WinOpenPic, "pushes int arguments" - pushes look the same for
  short) let VC6 strength-reduce `i * 0x1c + 10`; the owner's `short x, short y` gave byte. Loop forms, casts in
  the argument and `#pragma optimize` never moved it (ai_MessageFilterScreen 44 ins -> byte). Check the callee's
  real parameter types first when a loop's IVs differ.
- **A byte of a global `union { int word; unsigned char byte[4]; }` loaded as the dword** (`mov ecx, dword [U];
  xor eax,eax; mov al, ch`, a later `cmp byte [g], cl` reusing it) where the original does `xor eax,eax; mov al,
  byte ptr [U+1]`: the bytes were separate globals. Char-pointer views, casts and bitfields of the union give the
  same dword load; only a distinct object fixes it (probe: `extern unsigned char probe;` inside the function in a
  `dcp try`). An aligned unsigned char global can still load as `mov ecx, dword; and ecx, 0xff` (ai_SaveOptions'
  `_itoa(b, ..)`), so a `.word & 0xff` spelling elsewhere is no evidence for the union. ai_ApplyGfxOptions byte
  (the ai_GfxFloor split). New externs in menu.h moved sv_t's tutorial chapters off byte: declare in the users.
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
- **An int quotient whose dividend the original fild's only after the divisor's call** (`mov [a],eax; call g;
  fild [a]; mov [b],eax; fidiv [b]`, two int homes, no float temp), where ours converts at once (`fild; fstp
  float temp` before the second call, then `fdivr`): the quotient sat in the windows.h `min` macro, `min((float)a /
  b, ai_FloatOne)` with int locals a = f(), b = g(); the macro's two copies of `a / b` keep a and b live as ints.
  Every one-copy spelling (one expression, int locals, casts, double, ratio /=) gave the early conversion; an
  `if (r > 1) r = 1` clamp also gave `fld 1; fld 100; fmul st,st(1)` where min gives `fld 1; fmul [100]`
  (ai_CurseTargetWindow draft 28 ins -> byte).
- Commutative operand order is rank-based, not source order: textual swaps rarely change anything. Ranks move
  with parentheses (`(p->x)` is not neutral), a dead store, a named local, or a `(float)` cast of a subterm.
  For `a += b` / `a = a + b` of two locals the **statement order of their initializations** decides it: the
  local assigned last before the loop is loaded first (`fld a; fadd b` needs a's init after b's). Swapping
  `p = from` after `d = to - from` and `t = 0` after `step = ..` fixed four fadds of ai_ShotHitsGround (12 ins
  -> byte); `+=` vs `a = b + a` vs declaration order were all `= control`.
- Chains: `r = A + B; r += C;` split statements fix association; `x = sqrt(..); x = x * k;` fixes scheduling.
- **A byte global clamped as `mov byte [g],2; jb L; mov [g],al; L: mov eax,[g]; and eax,0xff`** (the low value
  stored first, the reload as a dword): `g = max(2, g);` (or `g < 2 ? 2 : g`; `max(g, 2)` and `if` differ) and the
  later read through the dword view (`ai_NumPlayersW & 0xff`): main_PlayerCountDialog 10 ins -> byte.
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
- **A product chain loaded in an order no unnested spelling gives** (every one of 360 orders of
  `S * area * drag * dens * v * v * 0.5f` = one code): write the chain fully nested in the original's load order,
  `(((((S * area) * drag) * dens) * v) * v) * 0.5f`; VC6 keeps a nested chain as written. A ternary factor
  (`v > 0 ? 1 : -1`) is evaluated first and multiplied in with `fmul st,st(1)` where it sits (`((area * v) * v)
  * S` gives `fld S; fld area; fmul v; fmul v; fmul st,st(1)`). A `(v)`-parenthesized operand inside the chain
  is not neutral (8 ins in ai_StepShot); the `0.0` / `1.0` of the sign were the shared ai_FloatZero / ai_FloatOne.
- **A float frame much larger than ours with 4-byte holes after groups of three slots**: the vectors were
  D3VECTOR4 locals (the w slot never touched), also for temporaries (drag, the two changes, the step move);
  VC6 keeps some fields of a split struct on the FPU and others in their slot. With them, built in place
  (`a.x = d.x + w.x; .. a.x *= k; a.y *= k; a.z *= k; a.y -= fall;`, `v = b + a; v *= 0.5f`) and field-by-field
  copies of the velocity and position, ai_StepShot / ai_StepShotSlow went 308 -> 28 ins (frame 0x5c -> 0x98).

## Types and bytes
- Byte read-modify-write (`or BYTE PTR [x],2`) = `unsigned char` bitfields; word ops narrowed to a byte =
  `unsigned short` bitfields; signed char `x == -1` gives `or bl,0xff`.
- A caller pushing a bare byte / word (`mov cl,[m]; push ecx`, no `xor`) means the callee's prototype takes a
  `char` / `short`. Wrong jcc or swapped `cmp` operands: `uv run try` a list of equivalent compare spellings.
- `push 0xff` needs an `unsigned char` parameter. A callee returning in al whose result is passed on is
  `unsigned int` (with `unsigned char` VC6 adds `and eax,0xff`).
- Copy vectors field by field (loads first, then stores); a struct assignment gives pointer copies.
- **`and edx,0x28; add edx,0x253; add edx,ecx` where ours folds `lea edx,[edx+ecx+0x253]`**: the original
  selected the base itself, `(c ? 0x27b : 0x253) + off`; `(c ? 0x28 : 0) + 0x253 + off` lets VC6 fold the
  constant into the lea and reshuffles every register around it (ai_FillSaveSlots 103 -> 6 ins).
- **A call to a trivial same-unit function sunk to its use** (the original calls it first and keeps the result
  in edx across another known call): VC6 knows the callee, so plain, cast and `(*f)()` calls all sink; only a
  call through a local function pointer stays put (ai_InitShot 80 -> 12 ins; needs an @original comment).
- `memset(&s, 0, n)` vs separate `= 0` stores: separate stores keep a zero register live for the whole function.
- **Constants 1 / 0 the original keeps in ebx / esi for a whole window pump** (`mov ebx,1` rematerialised after
  blocks that reuse ebx, `cmp eax,ebx`, `push ebx`) where ours pushes immediates: the order of the function's
  first stores decides it. `refresh = 1;` before `memset(rows..); memset(sel, 0xff, ..); total = 0; tick = 0;`
  took ai_BloodRevengeWindow 123 -> 20 ins (one of 120 orders; dcp try + deep-tools permute.py). In the same
  function: an `else if (clicked == -1) {timer}` chain whose original tests `!= -1` first and puts the timer
  block last was `else if (clicked != -1) { buttons } else if (timer) ..`; a value the original keeps in ebp
  across calls (`mov ebp,eax` after reading rows[0].rec) was a block local `AICHARVIEW *av = rows[0].rec;`
  (-90 ins); a search loop + `if (i < 5) {..}` whose original jumps straight into the body was the body inside
  the loop with `break`; `for (k = 0, idp = cfg.ids; ..)` put `xor edx,edx` before the `rep stosd` (byte).
  Struct field stores x before y per row (`rows[i].x = ..; rows[i].y = 0x7d;`) gave the late constant stores.
- **`mov eax, edi` (a copy of a constant register) then `mov [g], eax` runs** is an inline memset: VC6 expands a
  small constant `memset(a, 0xff, 20)` into dword stores of eax (copied from the register that already holds -1)
  and interleaves the neighbouring field stores. Five `slot[i] = -1` stores through the hoisted register were 74
  ins; `memset(p.slot, 0xff, sizeof(p.slot))` per panel gave ai_OpenPokerWindow byte.
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
  (2026-11-10) Seen again: a struct body in sv.h, any field layout / name / position, and member access
  `sv_QuickJump.active` in sv_t's ai_TutorialTick cost Chapter2 / 3 their byte. What kept both: sv.h holds only
  `typedef struct SVQUICKJUMP SVQUICKJUMP; extern SVQUICKJUMP sv_QuickJump;`, the body in its own header
  (sv_quickjump.h) that only the units reading its fields include; a bare `extern struct X v;` still lost Chapter3.
- **A float kept in st(0) through a floor and spilled around calls** (`fld 0.05f` / `fld st(0); fcomp 0.01q; jp;
  fstp st(0); fld 0.01q` / `fst [slot]`, ours `mov [slot],imm` stores and a reload): the floor was the windows.h
  macro `prog = max(0.01, prog);`; `if (prog < 0.01) prog = 0.01;`, the ternary, a separate float / double and
  `max(prog, 0.01)` all stay in memory (sv_RefreshPanel 118 -> 78 ins).
- **Cross-jumped identical arm tails kept in the wrong copy** (the original keeps the last arm's tail and the first
  arm jumps forward into it, ours the reverse): the arms of the if / else-if chain ended without `return;` and one
  `return;` followed the chain. Any nesting of the arms is the same code; only the per-arm returns matter
  (sv_RefreshPanel 78 -> byte).
- **A store of 0 to a global before a counting loop that ours drops as dead** (the original `xor dl,dl; mov [g],dl`,
  the count in dl, `mov [g],dl` again after the loop): the loop counted in the global itself, `g = 0; for (..) if
  (..) g++;`; VC6 keeps it in a register through the call-free loop and keeps both stores. A local count with
  `g = count` before and after loses the first store (ai_SaveNetworkGame, byte).
- **A global the original reloads after an inline `strcpy` into another global** (ours keeps the value in eax and
  the copy loop takes other registers; then a whole callee-saved cascade, constants 0 / 1 not in registers): the
  two globals are one object; VC6 forgets every field of the object the copied bytes land in. Probe it with
  `strcpy((char *)&rec + off, src)` in `dcp try` (byte at once for sv_SelectPanelObject, 86 ins -> 0), then declare
  the record (sv_QuickJump: object, record, two unread ints, contactName).
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
  reads `[e*4+A-4]`: a negative displacement off the symbol. The object before T (at A-4) and T were one record
  in the original: declare it (`SVCLIMATE { int seasonClimate; SVWEATHER weather; } sv_Climate`, the old names as
  field macros in the header) and the plain `T.a[h - 1]` matches (every spelling on T's own base = control).
  Do not read the neighbour through a cast of the other global: our link pads separate objects (the 0x188-byte
  record went to +8), dcp session wrap flags it as M.layout. sv_WeatherFrame 11 ins -> byte; the header change
  also took sv_t's ai_TutorialInitChapter3 to byte (tie-break).
- **A top-tested outer loop after a null test** (`cmp y,[surf+8]; jge` at the head, `jmp head` at the bottom,
  the cached zero re-zeroed at the exit): `for (y = 0; surf != 0 && y < surf->h; y++)`, the loop-invariant test
  in the condition (vc6-matching.md line ~317); `y = 0; if (surf) while (y < h)` is rotated (sv_LoadGroundplan
  49 -> 4 ins). `mov ecx,eax; add ecx,ebx; mov cl,[ecx]` for `ai_GoodsT[id].type` (ours `mov cl,[eax+ebx]`)
  is still open: no spelling, local, type or test form moves it.
- **The sv_FlyCameraToObject `up[4]` lever transfers**: sv_CameraToObject (frame 0xc short) took the same
  padded `up[1]` for the height above the centre, the datum cm_EffectRadiusScale for its M10 literal 1.8, and
  the view stores in rot 0,1,2 / pos 0,2,1 order (720-order permute): 40 ins -> byte.
- **A fallback choice whose value stays in eax through both tests and moves to a callee-saved register only at
  the next call** (`mov eax,[A]; test; jne L; mov eax,[B]; test; je ret; L: .. mov edi,eax` before the call):
  `if (A) b = A; else if (B) b = B; else return;` (or a both-zero test up front: VC6 CSEs the global loads).
  `b = A; if (!b) { b = B; if (!b) return; }`, `if (A) b = A; else b = B; if (!b) return;` and every copy
  through a second local load edi directly. In the same function a search loop's hit left by `goto found` onto a
  shared test of the fallback query is a second local (`obj = it; goto found;`, the stub `mov eax,esi; jmp`), and
  `g = &ai_GoodsT[*it]; .. g->name` gave `add eax,ecx; inc eax` where `ai_GoodsT[*it].name` gave one `lea`
  (sv_DrawGateName 81 ins -> byte).
- **A local kept in its stack slot but copied into a callee-saved register for a loop** (`bar = -1` stored at
  entry, then `if (timed) esi = slider(); else mov esi,[bar]` before the loop; ours keeps bar in the slot or
  propagates the -1 as `or esi,-1`): the loop was a plain `while (f()) {..}` with the flag tests written in the
  body (`if (flags & 0x40)`, `(flags & 0x80) && bar != -1`); VC6 hoists the invariant tests and caches bar in
  esi for the loop. `if (f()) { q = flags & 0x40; do {..} while (f()); }`, copy locals, `int bar[1]` and
  `?:` all kept bar in memory or propagated the constant (sv_MessageBox 86 -> 65 ins, shape). The same pattern
  (a slot variable reloaded into edi at the loop's iteration join) is open in sv_SendSelectedToBuilding.
- sv_MoveToGroup: one of 19 `ch->` uses re-read from the record (`((SVCHARREC *)ai_Chars[c])->id` in
  sv_RemoveGroupMember's call) took it 88 -> 74 ins (subsets.py singles + pairs, 190 variants).
- **A global byte reread for a table index right after its store** (`mov [g],al; .. mov eax,[g]; and eax,0xff;
  shl eax,3; mov ecx,[eax+T]`, ours `and eax,0xff` on the call result and `[eax*8+T]`): index by the global, not the
  local (`T[g.byte[0]].w`); the same change also fixed the store scheduling of the following
  GetPrivateProfileIntA reads (ai_LoadOptions 14 ins -> byte; a stray order hunk was two stores of one record
  swapped in the source).
- **A default kept in eax across the flag test** (`mov al,[flag]; test al,al; mov eax,1; jne`, ours the flag in cl
  and an earlier result store delayed): `if (flag) mode = 1; else mode = dflt;` (or the ternary), not
  `mode = 1; if (!flag) mode = dflt;` (ai_OptionsGfxScreen byte).
- **fmul / fadd operand order in a product or sum VC6 reassociates** (the original `fmul [field]; fmul [arg]`,
  `fld a; fadd [field]; fadd b`): the plain left-to-right spellings all compile alike; nesting moves it.
  `k = spread * (speed * (2r - 1))` gave the field first, `y = dside.y + (dother.y + v.y)` the original's sum
  order (ai_ScatterShot 8 ins -> byte; 25-variant sweep, deep-tools-style generator in the session scratch).
- **Two compare arms with one shared `push edi; call _strcmpi` tail** (the arms push only the literal): write
  each arm as `if (_strcmpi(name, LIT) == 0) goto fill;` and let the chain fall into one shared `slot = -1;`;
  per-arm `return -1` / `slot = -1; goto done` duplicate the epilogue and nothing cross-jumps. VC6 then keeps the
  later arm's copy; keeping the first arm's (the original's ai_RegisterSaveSlot) is open (stuck, 14 ins).
- The `1.0f` of an M10 `open:` line naming ai_FloatOne is worth trying first: reading the datum changes the
  scheduling around it (ai_ScatterShot 18 -> 8 ins with the copy moved).
- **A store before the stack cleanup after a call** (`call f; mov [g],ebx; add esp,N`; the usual order is
  `add esp,N; mov`, 67 : 2 in the first 136k instructions of the image): the `add esp,N` was a tail shared with
  another path that ends in the same pending cleanup. VC6's cross-jumping sinks the add below the store to merge
  it (`if (t) {f(t); t = 0;} else g(x);` gives `call f; mov; L: add esp,4` with the other arm jumping to L:
  d3t_CreateSnowMipmaps' snow_tex release, toy-proven with two-call arms). No spelling of the block itself
  reaches it (comma forms, the store's rhs holding the call, goto / else / do-while shapes, block order, linkage
  of the global, 23 flag sets: one code). d3sndw_Update's first stop block has this order with no second path
  left in the code: open.
- **Shared float object vs literal moves unrelated tie-breaks**: `ai_FloatZero` / `ai_FloatOne` against `0.0f` /
  `1.0f` at one site flipped the order of two register reloads at a loop latch 1400 bytes away (d3sndw_Update,
  8 combinations: 3 keep it, 5 flip it). Try every combination of the function's pooled constants before
  spending variants on the hunk itself; `dcp try` rows with the same `ins` can still be different hunks.
- **An int temp store scheduled before an `fdivp` where the original has it after** (`sub eax,edi; mov [t],eax;
  fdivp` vs `sub; fdivp; mov`): parenthesize the product of the dividend, `v - (v * z) / (r * r)`. Every other
  spelling of the expression (operand orders, casts, temporaries, 2000 - ground in an int local) is one code;
  the parentheses change the tree VC6 schedules (d3sndw_Update head volume, 4 -> 2 ins). Found with a flip
  search: compile many spellings and look only at the order of the two instructions, not at the distance.
- **Store before `add esp` after a call, second reading** (replaces the cross-jump guess above where no second
  path exists): VC6 emits the store first when a later instruction of the same scheduling block depends on it -
  it redefines a register the store reads (`mov [ebp+esi-0x18],bl; add esp,0xc; mov esi,..` mem_Alloc; `mov
  [esi+8],2; add esp,0x18; add esi,0xc` ch_GetWaypoints; `mov [g],ebx; add esp,8; pop edi .. pop ebx` in
  ai_CannonShotResults, whose final block has its own epilogue copy because the other returns are written out:
  a flip search over that byte-matched function shows the order falls back to add-first as soon as the
  `d2_SetViewEnabled(..); return;` path above is removed or changed). With nothing after the store in its block
  the add comes first (812 : 23 over the image). d3sndw_Update's first stop block is store-first with a label
  right after it: at scheduling time its block went on past that label. 437 structural forms of the two stop
  blocks (copies in both exits, nested copies, gotos) and ~60 local forms never gave store-first: open.

## The back end's scheduler, read from c2.dll (tools/c2dag)
- `python tools/c2dag/c2dag.py src/UNIT.c FUNC` prints VC6's scheduling graph of every basic block of FUNC: the
  instructions in their order before scheduling, each with priority, height, flags and dependency edges
  (`--blocks`: VC6's own block listing, the hidden switch `/d2FAF`: `; Block(n) In(..) Out(..)` in the /FA file).
  It runs a copy of the compiler whose C2.DLL is a proxy (tools/c2dag/c2proxy.c, built with VC6) that loads the
  real back end and hooks its priority pass (dag.c, RVA 0x14f6a of C2 13.00.9044).
- The rule (read from the pass and checked against dumps): per block VC6 builds a dependency graph (register
  def/use, memory overlap, flags; calls and jumps are barriers) and emits the ready instruction with the highest
  priority. priority = height * 8192, + 65536 for a memory load; height = longest latency path to the block end.
  A tie goes to the instruction that comes first before scheduling. So: loads are hoisted; `add esp,N` after a
  call sits right after the call in the list and wins every tie; an instruction overtakes an earlier one only when
  something later in the SAME block depends on it. A scheduling hunk therefore means one of: another list order
  (the expression tree: parentheses, operand order, a temporary), another edge (a later statement in the block
  that reads / overwrites what this one touches), or another block extent (a label that was not there).
- d3sndw_Update's open hunk in these terms: block of the first stop code, nodes `add esp,16` (h=2) and
  `mov [Snd[0].snd],ebx` (h=2), tie, add first. The original has the store first: its store had a successor in
  that block. No source form found yet that gives it one and leaves no instruction behind.
- tools/c2dag/c2fuzz.py maps the back end by skipping one call at a time (15353 sites; a probe file is compiled
  with each patched copy); c2pass.py compiles a unit with chosen calls skipped and diffs one function. No call
  switches the whole scheduler off cleanly; the sites that only reorder d3sndw_Update are 0x1070aead,
  0x1070f5b1, 0x1070f87b, 0x1070fa29, 0x10714f85, 0x1073a0d6.
- More of the scheduler, from the dumps: a scheduling region ends at a jump, a label or after 81 instructions
  (FUN_10714d7c), so a long straight block is scheduled in pieces. Memory operands conflict only when they are the
  same symbol with overlapping [offset, offset+size) (FUN_1070437c): `Snd[0].snd` and `Snd[1].snd` never do.
  ai_CannonShotResults' store-before-`add esp` is list order, not an edge: the final block's list is `call;
  mov [ai_ShotDirty],ebx; add esp,8` with the add carrying the call's line (`ln`), because VC6 merged the
  `add esp,8; pops` tail of the `d2_SetViewEnabled(..); return;` path with this block's tail BEFORE scheduling
  and copied the short tail back into both; the copy lands after the store. A `ln` on an `add esp` that names
  another statement's line is the mark of such a merged-and-copied tail. So store-first has two causes: an edge
  (mem_Alloc, ch_GetWaypoints) or a tail shared with a second path that ends in the same call cleanup and has the
  same successor (CannonShot). d3sndw_Update's first stop block has neither in any form tried.
- **A whole-function register reshuffle after the "right" loop form** (di_UpdateButtons: the index loop gave the
  original's tail but 151 other instructions changed) is a priority race lost by one or two points: read it with
  `c2dag.py SRC FUNC --alloc` (the two candidates' priorities, and per block P and the references), then run a
  flip search: mutate every statement, keep the mutants where the right candidate has the register again AND the
  wanted code shape is there, and measure those with `dcp try`. Here 146 of 680 mutants flipped it and four were
  byte: the press position read through a `volatile` view an earlier pass had added to force reloads. A volatile
  read is no reference of the field's web; the plain `dm_S.x` is, and that reference was the missing point. A
  `volatile` (or any view) added to fix one hunk is a suspect whenever the allocation is off later.
- **Where `add esp,N` ends up, from the back end's three passes** (C2 13.00.9044, read in Ghidra and checked by
  skipping each pass with tools/c2dag/c2pass.py):
  1. Code generation puts the cleanup directly after its call, before the next statement's code (skip
     0x1070aa67 to see it: `call sb_Stop; add esp,8; ..; call sb_SetRestart; add esp,8; mov [g],ebx`).
  2. FUN_1070b884 merges stack adjusts along a straight run (it stops at a label or a jump): an earlier
     `add esp` is deleted and its amount added to the LATER one. So the merged cleanup sits where the last
     adjust of the run was: after the last call, or after a store only if another adjust follows that store.
  3. Cross-jumping (FUN_10712437, runs twice; skip 0x1070a91d to switch it off) takes every `jmp L`, and
     compares up to 30 instructions before the jmp with up to 30 before L (labels do not stop it). It matches
     from the bottom over a dependency graph, not linearly: any instruction of one side that could legally be
     last and has an identical one on the other side is moved to the end of both and shared; the jmp then goes
     to a new label in front of the shared tail. That is the pass that turns `call; add esp,8; mov [g],ebx`
     into `call; mov [g],ebx; add esp,8` in ai_CannonShotResults (the `d2_SetViewEnabled(..); return;` path
     shares the add; the short return tail is copied back afterwards, so no jump is left).
  `c2dag.py` writes the pairs cross-jumping examines into build/c2dag/UNIT.dag (`xjump FUNC jmp@line`, then both
  regions as opcode@line, last instruction first).
- **d3sndw_Update's store-before-`add esp` is explained by cross-jumping two copies of the stop code** (found by
  reading FUN_10712437 and confirmed by experiment): when the whole stop code (`if (Snd[0].snd) {..} if
  (Snd[1].snd) {..} return;`) is written in more than one exit, VC6 merges the copies bottom-up through the
  labels. The last block's instructions match in their natural order (`add esp,16; mov [Snd[1].snd],ebx`), but
  the first block is reached through the label node, whose predecessors are released add first, so the shared
  tail gets `mov [Snd[0].snd],ebx; add esp,16`: exactly the original's asymmetry between the two blocks
  (variant `dup-split-first`: first block store-first, second add-first, 30 ins left). What still differs is
  the scratch register of the two reloads (`mov ecx,[Snd[0].snd]` / `mov edx,[Snd[1].snd]` in the original):
  the copies only merge completely when both get the same registers, and so far exactly one copy does - the
  first of the copies in the allocator's block order behind the switch code gets ecx / edx, any other eax / ecx.
  Tried: the four entry conditions distributed over inline copies, labelled copies in the else-if and copies
  at the function end (127 arrangements, best 24 ins). Open: an arrangement where both copies get ecx / edx.
- **d3sndw_Update byte**: the stop code inline in both exits (`if (!ready) { stop both; return; }` and `else if
  (d3sndw_Scene != d3_DefaultScene) { d3sndw_Scene = d3_DefaultScene; stop both; return; }`) and NO `sc` local
  for d3_DefaultScene. The copies merge completely only when their reload temps get the same scratch registers.
  C2 hands those out from one pointer that rotates eax, ecx, edx through the function in code order (three
  copies in a row get eax/ecx, edx/eax, ecx/edx); `mov ecx,[d3_Camera]` of the third entry condition advances
  it by one, each copy by two, so the second copy needs one more temp between the copies: reading
  d3_DefaultScene without the local `sc` is that temp (with `sc` the value is a candidate in eax and does not
  advance the pointer). Lessons: (1) a store before its `add esp` with a label right behind it = two
  cross-jumped copies of that code; write the code out at each exit instead of `goto` to one copy. (2) Copies
  that refuse to merge because one scratch register differs: count the temps between them (a local that got a
  register is no temp, the direct global read is).
- **`add r,edx; .. mov esi,r` where the original computes a pointer local with one `lea esi,[ecx+edx]`** (the sum
  lands in the index temp's register and is copied to the local): the local's value was also stored to a field
  right after (`tex = base + idx; tri->tex = tex;`), and VC6 forwards the sum to the store through the index
  temp's web (`c2dag.py --alloc`: the temp has `use def use def use` in the block, the local one def after it).
  Write the expression out at the store too: `tex = base + idx; tri->tex = base + idx;`. The CSE'd second copy
  is its own temp and the local gets the lea. The other order (`tri->tex = ..; tex = ..;`), `tri->tex = tex =
  ..`, `tex = tri->tex` and every spelling of the sum itself were no lever; flip.py's 2363 single-statement
  mutants (file scope) had no hit either, because it never duplicates an expression into a neighbouring
  statement (d3io_AttachStock 3 ins -> byte).
- **A shared failure block (`or ebp,-1` + its own epilogue copy) inline at the LAST failing test, the success path
  falling into the end epilogue**, where ours puts the failure block after the success code (the success path gets
  the epilogue copy, the far jumps grow to rel32; a `tail` hunk): the function had no `goto end` at all. Each step
  sets the result and the next step runs under `if (!ret) {..}`:
  `if (!e->data) { bank = f(e); if (bank) ret = Load(bank, e); else ret = -1; } if (!ret) { if (type == 1) ..
  else if (type == 2) .. else ret = -1; } if (!ret) { ..fmt.. else ret = -1; } if (!ret) { if (sfx(..) == -1)
  ret = -1; } if (!ret) { success } return ret;` (flat or nested `if (!ret)`: one code). VC6 threads the flag
  tests away, so the instructions are those of the goto form, but the block layout differs. Two details count:
  the call arm first (`if (bank) ret = Load(..); else ret = -1;`; with `if (!bank) ret = -1; else` 8-10 ins stay),
  and no `goto end` left for the load failure (8 ins). With gotos every spelling is one code (`ret = -1; goto
  end` per site, `goto fail` into one block, `!=` tests, else-chains: VC6 normalizes them; switch and `return
  ret` per site are worse) - 162 + 40 variants (sb_Play 14 ins -> byte).
  How it was found: flip.py's constant mutants (`ret = -1` -> `-2` at one site) showed the wanted layout whenever
  the else-arm failure of the type chain was NOT the same block as the others, and c2pass with cross-jumping
  skipped (`--skip 0x1070a91d`) showed that this else block is laid out after the success code and the other
  sites are merged into it. A failure block that must merge AND stay inline is then no goto block: it is a
  flag store the optimizer threads. Probe values per site (all 2^n subsets with `dcp try`) tell which sites
  decide a layout before any respelling.
- c2pass.py / c2fuzz.py need `pefile`: run them as `uv run --with pefile python tools/c2dag/c2pass.py ..`.
- **`lea edx,[edx+X]; mov edi,edx; rep stosd` (the fill's address in a scratch temp, copied to edi) where ours has
  `lea edi,[edx+X]`**: the fill was the plain element loop, `for (i = 0; i < 8; i++) c->relatives[i] = -1;`, not
  `memset(c->relatives, 0xff, 32)`. VC6 /O2 turns the loop into `rep stosd` as well, but keeps the address as a
  temp of its own (the loop's base) and schedules the neighbouring stores differently. Every spelling of the
  memset (casts, `&..[0]`, a pointer local, its result used, 359 statement orders) is one code; flip.py found it:
  the only mutant with the pattern was the memset DUPLICATED, i.e. the address used twice, and the loop is the
  form that uses it twice without a second fill (ahm_CreateAlchemist 3 ins -> byte). The index variable's name
  does not matter unless it is the enclosing loop's neighbour (ai_HelperBook: k fine, j worse).
  Swept with `python tools/sweep/fillloop.py` (every constant memset of every below-byte function as the loop,
  measured with dcp try; `--zero` for the zero fills, `--widths int,char` for the element type): 47 functions
  with a -1 fill, 8 moved - hmt_Create and he_StartGang byte, gv_RaiseDead 9 -> 4, ai_UpdateProduction 33 -> 24
  (only the first of its four fills), gm_CreateCharacter, ai_BuildTownMap, ai_HelperBook, ai_MercenaryBook a few
  ins each. Run the sweep again after base work on a unit; the reverse (a loop of ours that the original has as
  memset: `lea edi` directly) is the same lever read backwards.
- flip.py reading rule: a hit on a DUPLICATED statement means "this value has two uses in the original" (look
  for the spelling with two uses: a loop, the expression written out again, a macro's second expansion); a hit
  on a changed constant at one site means "this block is not shared with its twins in the original".
- **A search loop's two loop-invariant loads in the other order** (the original `mov edx,[m.grave]; mov eax,[e.grave]`,
  ours the base first): loads tie in the scheduler and keep their list order, and a walking pointer set up before
  the loop (`g = e->grave; do { q = g; .. g = q + 1; }`) puts its load first. Index the base in the loop instead
  (`do { q = &e->grave[i]; if (q->id == m->grave) break; i++; } while (..)`), or read the compared value into a
  local before the base (`n = m->grave; g = e->grave;`): both byte in gv_RaiseDead; swapping the compare's
  operands or reading the local after the base are no lever.
- fillloop.py `--zero --widths int,char` (2026-11-14, 222 functions with a fill, 7 moved; NOT applied - the probe
  spelling, an int loop over a struct or char buffer, is no source to keep; each is a lead that the fill there
  was not a memset call of that shape: an initializer, a field-by-field clear, a loop over the real element):
  io_CharacterNameMarkup 2 ins -> byte with the `tmp` clear as a loop (its only hunk is a `[eax+edx]` /
  `[edx+eax]` tie-break in a strcpy 75 lines below: the fill form moves it), io_CharacterName 8 -> 4 (all four
  fills as char loops), ch_LoadLowAnims 28 -> 9 (`memset(&f, 0, sizeof(f))`), d2_ResizeWindow 32 -> 24
  (`desc`), ai_CalcMeisterWache 100 -> 86 (the second `msg` clear), ai_FillLoversTab 55 -> 45 (`rows`),
  gv_AllocGraveyard 162 -> 116 ins but shape -> draft.
- **Two small out-arrays that share ONE slot in the original, with a hole behind it** (dcp frame: `shared in the
  original`, and `holes:` lists bytes right after the shared slot; the frame size is equal): one array reused
  for both calls, declared larger than what the code reads - `int row[4]` passed to two `sv_ButtonRow(.., row,
  2, ..)` calls that read row[0] / row[1]. Two `int[2]` give two slots, one shared `int[2]` a frame 8 short;
  an unused extra array is dropped. Size the shared array as slot + hole (lobby_ChoosePlayer slots 6 -> 0).
- **`mov eax,[esi+eax]` against ours `[eax+esi]` for `list[i]` in a strength-reduced loop** (esi = i * 4, the
  global pointer reloaded into eax): the loop ran zero-based and indexed `list[i + 1]` (`for (i = 0; i < 8; i++)
  if (lobby_List[i + 1] == 0)`), not `for (i = 1; i < 9; i++) .. lobby_List[i]`. Same instructions, the address
  operands swapped; `*(i + p)`, `*(p + i)`, a local for the element, nested ifs are all the 1-based code. The
  twin loop above it in the same function is 1-based in the original (lobby_ChoosePlayer byte).
- **A unit that is public code: get the published source's spelling first.** qnt.c is Dennis Lee's DL1 quantizer
  (dl1quant: build_table / reduce_table / set_palette / quantize_image / bestcolor). qnt_Map had resisted 250+
  variants at 87 ins; written the way the published quantize_image spells it, it was byte in four rounds:
  the row pointers stepped at the END of the pixel loop (`nextrow[0-3] = e; .. nextrow[0+3] += ..; thisrow[0+3]
  += ..; thisrow += 3; nextrow -= 3;`, ours had stepped them in the middle and used [0] / [6] / [3]): 87 -> 72
  together with the clamp table filled in index order (0, k, 255) and thisrow set before nextrow; a pointer
  local for the biased table (`dm = lim + 256;` then `dm[k] = k` and `dm[r - pal]`): 72 -> 2 (VC6 forwards the
  pointer into the addressing, `[edi+eax+0x100]`, and CSEs it in the loop as `lea edx,[eax+0x100]`; written
  `lim[x + 256]` it never does); the table loop as `tbl[5][i].count` with the plain index: 2 -> 0 (a
  hand-strength-reduced `off += sizeof` loop had the address operands swapped). Signs that a unit is library
  code: a self-contained algorithm, magic numbers (200000, DITHER_MAX 20, 32768), tables biased by 256.
- **Three terms of a sum evaluated in an order no spelling gives** (`sq[R - r] + sq[B - b] + sq[G - g]`, all 6
  orders, temps and parentheses one code; M8 lines with crossed references): the three arrays were ONE object,
  `unsigned char pal[3][256]`. VC6 orders commutative operands by rank, and three symbols rank differently from
  three offsets of one symbol. Probe before touching the data: `extern unsigned char probePal[3][256];` inside
  the function in a dcp try variant (identical instructions = the lever) (qnt_Nearest 4 ins -> byte; qnt_PalR /
  G / B became qnt_Pal[3][256]).
- **A branch that jumps INTO the other branch's fallback block, behind its test** (`cmp m,w; je <neutral block of
  the men branch>` with the early return as fall-through; ours inlined a second copy of the fallback call): the
  doubly-missing case was excluded in the outer condition, `if (men < women && (w != 0xffff || m != 0xffff)) {
  if (w == 0xffff) return; ..women.. return; } if (m == 0xffff) { m = -5; f = 1; } ..men..`. The `||` leaves
  the both-missing path to the code below, VC6 threads its `m == 0xffff` test (just evaluated) past the second
  one, and the `w == 0xffff` re-test inside is threaded to the return. Setting the fallback values in the
  women branch and falling into a shared chain duplicates the folded call instead; `!(a && b)` and the
  inverted if/else are the same code (snd_PlayFavourComment 10 ins -> byte). Reading rule: a jump into the
  middle of a sibling branch, just behind a test of the same variable, is a threaded re-test - write the
  condition so that the path really runs through that test.
- **A failure branch inside a loop that jumps to the FIRST of several identical `f(); return 0;` blocks** (ours;
  the original jumps to the LAST one, right behind the loop's entry test; a short `je` in the original, a rel32
  in ours, so 245 bytes shift and the shift shows as register hunks elsewhere): VC6 merges an identical return
  block into the first copy in source order (probe: `return 2` at the earlier copies moves the target to the
  next equal one), and no spelling of the copies (`!x` / `== 0`, all 31 subsets) is a lever. The loop was
  written with its exit test in the `while`: `do { .. } while (main_QuitProgram == 0); main_Exit(); ..
  return 1;` instead of `do { .. if (main_QuitProgram) { main_Exit(); .. return 1; } } while (1);`. A
  `mode = (strcmp(..) != 0) * 2 + 1;` was an if/else (`setne; lea [eax+eax+1]` either way) (WinMain byte).
  c2pass.py / flip.py look for `_NAME PROC`: for a __stdcall function pass `WinMain@16` to c2pass; flip.py
  cannot find the source line then (a copy with `(@\d+)?` in its PROC/ENDP regex worked).
- **A row position kept in a register that the original only gets after the n test (`mov ebp,eax` after
  `test ebp,ebp; jle`), n in the same register before and after the loop**: the position was `top + i * h`
  in the call, not a `y` advanced in the for increment (`for (..; i++, y += h)`). VC6 strength-reduces it to
  the same `add ebp,ebx` but gives n and the running value the original's webs (main_ChooseOpponents 22 ins ->
  byte; the frame loop as a plain `while (sv_RunFrame(..))` is the same code as `if (..) do .. while (..)`).
- `strcpy(tok[n++], t)` with a plain counter gives the two-pointer inline copy (`mov edx,esi; mov ecx,eax;
  add esi,0x10; mov al,[ecx]; inc ecx; mov [edx],al; inc edx`); a hand-advanced row pointer (`q = p; p +=
  0x10; strcpy(q, t)`) gives the `sub edx,eax` offset copy (main_BuildVersionString 13 ins -> byte).
- `c2dag.py SRC FUNC --alloc` prints the register each candidate received as the last column (`-`: none). Read
  it before a variant round on a register hunk: the question is never "which spelling" but "which candidate
  must rank above which". Open (main_ChooseHistory): a candidate can be denied a register that looks free (grp:
  esi while it ranks above ret, none once ret ranks above it and holds ebx) - priority order alone does not
  predict the outcome; the choice rule is not read from c2.dll yet. c2dag / c2pass / flip take `WinMain`
  (decorated `_WinMain@16`) by its plain name now.
- **A whole block of global stores scheduled with other scratch registers after an inline `strcpy` of a literal**
  (main_JoinNetworkGame: `or al,0x50` against the original's `or ecx,0x50`, a word load hoisted, and a stack load
  hoisted above a later `rep movsd`; ~36 ins over the whole function): statement order of the block, with the
  strcpy NOT first. flip.py (want `or ecx, 80`) hit "swap strcpy with the next statement" (36 -> 14); a generated
  round of all orders of the 8 statements with the strcpy at every position (3359 variants) gave byte for
  `ai_NumPlayers = hdr.players; strcpy(main_NetHost, "127.0.0.1"); ai_GameFlags |= 0x50; net.players = ..; ..`.
  The earlier note "store order byte-neutral" had only permuted the stores without moving the strcpy. The
  scratch-register rotation runs through the whole function, so one early statement fixes hunks 60 instructions
  later (the char0c/char54 eax/ecx/edx rotation went with it). The hand-written `do { c = s[i]; d[i] = c; i++; }
  while (c)` copies were plain `strcpy` (one code with VC6 here: stack array or global source, global destination).
- Frame symbol order for flip.py: the listing's `_x$ = -N` lines sit above PROC and flip.py only collects the
  instructions, and `_x$[esp+N]` in an instruction does not change when x moves (N is the frame size): a slot
  question needs a flip copy that also keeps the `_x$ = -N` lines (main_ChooseCity: 0 of 111 mutants moved the
  two 0x100 arrays; a size probe objName[0xff] did, i.e. equal-size arrays are ordered inside C2 by a tie rule).
- **`lea edx,[eax+eax*8]; lea eax,[edx*4+X]; test eax,eax` (an array element's address in a temp of its own, then
  tested for null) where ours folds it into `lea eax,[eax+eax*8]; lea eax,[eax*4+X]`**: the address was a pointer
  local assigned in the condition, `(ch = &ai_CharsT[b->owner]) != 0 && ai_CanAct(ch)`, not the address written
  out twice. One temp more there moves VC6's eax/ecx/edx rotation for the whole rest of the function: a
  statement-order fix above it (main_EndRound's 0x4c note: msg.time / msg.flags last) had looked like a loss
  (74 -> 140 ins, every scratch register behind it rotated) until this one was fixed too (-> 38). Judge an order
  round's best only after the rotation behind it is in sync; keep the candidate that fixes its own block.
- **A `goto` past a duplicated increment** (main_GameLoop: `if (!over && !left) { flags &= ~0x42; player++; goto
  next; } } player++; } next:`) is one plain increment after a plain if: both paths run `player++` once. Removing
  it fixed the jmp branch hunk and the zero-register hunks around it (91 -> 69). Two loops over a clock that sit
  at different stack slots in the original were two locals (`AITIME night` for the second loop: 69 -> 41).
- **A frame N bytes larger than ours, with N untouched bytes right behind a small local whose address goes to a
  call** (main_GameLoop: 0x6c bytes behind the clock passed to `cm_SendExSysMessage(3, &clk)`): the local was
  the callee's full record, not its used head. cm_SendExSysMessage copies 0x7c bytes of its argument, so the
  clock was a 0x7c-byte message record (`struct { AITIME time; char pad[0x6e]; } clk;`, like CMQUICKSAVE and
  main_EndRound's sysmsg). Frame 0x450 -> 0x4bc, 41 -> 9 ins, and the slot order of the three clocks fell into
  place. Reading rule: for an untouched hole, look at what the neighbouring local is passed to and how many
  bytes that callee reads. A union of the unrelated buffers (text / AIMSG / save) and separate block-scoped
  locals are the same code - prefer the block locals.
- **A global counter the original zeroes BEFORE a branch and again in one arm** (`mov [g],edx` ahead of the
  `jne`, where ours had no store): the other arm counts in the global itself (`for (..) if (..) g++;`), not in a
  local assigned at the end; VC6 keeps the count in a register either way, but only the direct form keeps the
  first store alive. A zero field store right after a memset (`memset(&save, 0, ..); save.slot = 0;` before the
  strcpy) reuses the memset's zero register (`mov [slot],eax`); after the strcpy it is an immediate store. A
  struct time copy whose last two stores come after the record's other fields was written last
  (`party.time = ai_Time;` after kind / flags / thing; 720 orders, 5 byte) (main_GameLoop byte).

## The register allocator, read from c2.dll (tools/c2dag/c2trace.py)
- `python tools/c2dag/c2trace.py SRC FUNC [--variants FILE --label L]` prints the allocator's real sequence: every
  candidate as it is queued (priority, savings `sav`, flags, the registers still available to it, its
  preferences) and assigned, and the splits. `c2dag.py --alloc` shows only the first priority pass and the final
  register; when an outcome looks impossible ("a free register is refused") the reason is in the sequence.
- The rule (color.c; FUN_1073122d driver, FUN_10708a6e queue, FUN_10708af7 interference / pressure, FUN_10709937
  choice, FUN_10709bb6 removal): candidates are popped in descending priority (equal: the later-created first).
  A candidate live across a call can only have ebx / ebp / esi / edi; one referenced by a BYTE-sized operand
  only eax / ecx / edx / ebx, so across a call only ebx. For each available register a score is summed from the
  still-waiting interfering candidates: their preference counts, and 100 x savings for a candidate whose only
  possible register that is; the lowest score wins, ties in the order eax, ecx, edx, esi, edi, ebx, ebp.
  Consequences: (1) a byte-flagged value that lives across calls (a pooled constant compared with a byte:
  `kb_Last == 1` flags every use of the constant 1) reserves ebx - higher candidates then avoid ebx and take
  ebp; (2) when the fourth callee-saved register is taken, everything else live across a call is SPLIT, and the
  pieces come back with new priorities and can overtake an older candidate (main_ChooseHistory: a piece of the
  constant zero at -86 took esi ahead of grp at -91); so a candidate must be served before the fourth register
  goes; (3) savings below 1 = no register; under /O2 a use weighs 1 << loop depth, a constant counts only
  where the other operand is memory, each load of it costs the weight; (4) a block where the candidate is dead
  (a definition follows on every path) costs it nothing - a flag reset at the top of each iteration ranks much
  higher than its reference count suggests; (5) spellings that fold before allocation (`x = y` where y is known
  to be 1, a named copy of a global) change nothing.
- main_ChooseHistory byte (26 ins, stuck twice): ret had to be popped first (it then avoids the reserved ebx and
  takes ebp), grp second (esi, before the fourth register), again third (ebx, still above hist). Got there by
  (a) the family-choice success written out (`ret = 1; sv_PopRequest = 1;` a second time: one more definition
  of ret in the loop, VC6 cross-jumps the copies), (b) ONE `again = 1` reached by `goto lagain` from the other
  site (one definition fewer; VC6 duplicates the short block again after allocation, so the code has both),
  (c) `ret = 0;` before `again = 0;` at the top (the entry's zero register goes to the later of the two), (d)
  the single-return tail (`if (ret) f |= 8; else f = 0; return ret;` - VC6 duplicates the return into both arms
  BEFORE allocation, which is why the zero path keeps `mov eax,ebp`; an explicit `return ret` in the zero arm is
  folded to `xor eax,eax`). The two gotos stay as @original: no goto-free form reached byte.
- The c2dag proxy's graph dump faulted on a node of that function and took the compiler down (C1001): the dump
  is guarded now and prints `(dump fault: block skipped)`.
- **main_EndRound byte (38 ins, stuck): three levers, each needed.** (1) NO local for a value that the original
  keeps in a callee-saved register across the next call as a 16-bit move (`call util_Random; mov bx,ax; push
  esi; and ebx,0xffff; call gm_BuildingLevel; cmp eax,ebx`): written inline, `if (gm_BuildingLevel(b) <
  util_RandomU(0x5a))`, the first call's result is an expression temporary. Temporaries are no allocation
  candidates: they get what the global allocator left over (here ebx), so value and weight take edi / ebp as in
  the original. Any named local (`lim`, every type and scope) is a candidate of high priority and takes edi.
  The 16-bit move needs the 16-bit prototype (`util_RandomU`); `(unsigned short)util_Random()` gives `mov
  ebx,eax`. (2) WHICH of the two calls of one comparison runs first is decided by the operands' weights and, on
  a tie, by an internal key that follows where a variable is first referenced in the function: `AIBLD *b = 0;`
  at the declaration (no code) made VC6 call util_Random first; without it gm_BuildingLevel came first.
  Operand sides and `<` / `>` only swap the cmp. (3) Two stores scheduled around a load in an order no statement
  order gives (`mov eax,[esi+1]; mov [msg.arg],eax` before `mov edx,[tbl]; mov [msg.to],edx`): every `AIMSG msg`
  declared in the block that builds it (nine blocks), not once at function scope. The scheduler orders a
  pointer load after every earlier store to an address-taken local whose address already escaped to a call;
  a fresh block local has not escaped yet. The frame stays the same (disjoint block locals share the slot).
  This is the 2026-11-10 block-scope lever again: sweep it (blockscope) whenever a store / load order resists.

## The stack frame, read from c2.dll (tools/c2dag/c2frame.py)
- `python tools/c2dag/c2frame.py SRC FUNC [--refs] [--variants FILE --label L]` prints what the packer decided:
  every local's size and reference count at layout time (L rows), the slot table before and after the sort (B /
  A rows; A index 0 is nearest esp), with --refs each counted reference in walk order.
- The rule: count = one per operand naming the local, all blocks, no loop weight, taken after register
  allocation and instruction selection but BEFORE the late passes - a statement written in both arms of an if
  (tail-merged later) counts twice, a reload the late peephole removes counts, so the listing's count can be
  lower. The locals are listed by size ascending, then count descending (tie: who reached the count first in
  the walk); each joins the first lifetime-compatible slot or opens one; frames above 0x80 bytes are sorted by
  weight * 1000 / size, descending, with an unstable quicksort; the densest slot is nearest esp. Declaration
  order, type and scope never enter (main_ChooseCity: 5 such variants = one code).
- So a frame-order hunk is a COUNT question: read the counts, work out which local needs one more or one fewer
  (simulate: scratch qsim.py pattern - the sort is 12 lines), then find the ordinary source that has that
  reference at layout time. main_ChooseCity is the open case: objName 4 / cityobj 4 where the original needs 3 /
  4 or 4 / 5.
- **Loads of a vector copy interleaved with its stores** (ours `mov ecx,[src.x]; mov [pos.x],ecx; mov edx,..`,
  the original all three loads first) and a later store order no permutation fixes: the destination was a
  block local (`D3VECTOR4 pos; D3VECTOR rot;` declared in the block that fills and passes them, in each block
  that does). With the function-scope pair the scheduler orders every pointer load after the earlier stores to
  the address-taken local. Same lever as main_EndRound's `AIMSG msg` and the 2026-11-10 block-scope sweep
  (main_ChooseCity: tower hunk gone, together with `pos.z` assigned last).
- flip.py had mutated the wrong lines for every function whose prototype line carries a comment after the `;`
  (it took the prototype for the definition): fixed 2026-11-14. Flip results on main.c functions from before
  that date (0 flips on main_EndRound / main_ChooseCity) say nothing.
- **main_ChooseCity byte: the block-scope lever a third time.** `char key[0x100]` declared in each of the two
  blocks that use it (not once at function scope) put cityobj before objName in the frame. No count changed:
  the two block locals have 3 references each and share one slot (block locals with disjoint scopes merge even
  when their addresses go to calls; function-scope arrays never do), so the slot keeps its weight but enters
  the table at the position of a count-3 symbol, behind objName and cityobj - and the unstable sort then
  resolves their tie the other way. So a frame-order hunk between two equal-density slots has a third cause
  besides a count one higher or lower: a slot at another table position. Rule for the whole project, three
  matches in one day (AIMSG msg in main_EndRound, pos / rot and key here): a buffer or record that is only
  used inside blocks belongs in those blocks. VC6 shares the slots, so the frame size does not tell; the
  scheduler (escaped address) and the frame sort (table position) do. Declare block-used locals in their blocks
  BEFORE any other lever on a store-order or frame-order hunk.
- **A local assigned at the top "as a default" and overwritten in one arm** (main_CreateWindow: `exs = 0;` before
  the window class code, `exs = WS_EX_TOPMOST` in the mode-3 arm; ours spilled exs to a 4-byte slot, frame 0xcc vs
  0xc8, and the zero / work.right / work.bottom took other registers): the default was the else arm's statement
  (`} else { style = 0x6000000; exs = 0; }`). The value then lives only from the branch to the call and takes
  ebx, which also serves as the zero constant at the top. Together with the final `if (mode == 3) .. else {..}`
  written as `if (mode != 3) {..} else ..` (the original lays the long arm first) and `nw` computed before `nh`:
  105 ins -> byte in one round of 8. A "default at the top, overwritten in one arm" is a lifetime the original
  probably did not have: try the assignment in the other arm first.
- **A fallback chain written with gotos between the arms** (main_OpenScreen: `if (res == 0) { if (has800) goto
  sized; pick1024: .. } else if (res == 1) { .. goto try800; } else if (res == 2) { .. try800: if (!has800) goto
  pick1024; res = 0; } else goto sized; <set mode size> sized:`): the original was one condition and one plain
  else-if chain, `if ((res == 0 && !has800) || (res == 1 && !has1024) || (res == 2 && !has1152)) { if (has800) res
  = 0; else if (has1024) res = 1; else if (has1152) res = 2; <set mode size> }`. VC6 threads the known tests
  into exactly the original's cross-jumps (`res == 1 && !has1024` lands on the `has800` test, `res == 0` skips it).
  170 -> 159 ins alone. Reading rule: jumps from one arm into the middle of another arm's test chain = a short-
  circuit condition followed by a priority chain, not hand-placed labels. The same item had a meaning bug: the
  window's X / Y arguments swapped (abs(height diff) as X) and an extra abs in one branch - read the push order
  against the frame offsets of the RECT fields before trusting an earlier "reasoned" note.
- **The distance can rise when the source gets right** (main_SearchNetworkGame, 2026-11-14): seven hand-written
  `do { c = s[k]; d[k] = c; k++; } while (c)` copies as `strcpy` give exactly the original's copy shapes (the
  offset copy with `name - &recs[0].name` strength-reduced by -0x80 per host, `neg edx` for two destinations of one
  source, the indexed form between two fixed addresses), and `int btn[4]` for the 4-byte hole behind three button
  ids gives the original's frame size - yet the distance went 281 -> 348 ins, because every allocation candidate
  of the function is split and the pieces land in a permutation of the original's registers. Keep the forms that
  are visibly the original's (copy shapes, frame size) and work the allocation from there; the lower count was a
  local optimum of the wrong copies.
- **Block-scope sweep, 2026-11-14** (`python tools/sweep/blockscope.py`, every below-byte function): 23 functions
  closer (ai_SendJobOrders 130 -> 87, ai_UseGoodSendHome 134 -> 90, d3sm_Build 220 -> 176, ai_CannonExplosion
  514 -> 476 ..), none byte by itself, but four were then one small step from byte: he_InnStaffLeave (the three
  `arg[]` stores as [0], [2], [1]: 6 orders), ai_UseGoodInfectOwner (`msg.time = ai_Time` after `msg.fromId`: 7
  positions), ai_LoadTownMap (`if (size >= 0x780) scale = 7.5f; else scale = 5.33f;` - the default-then-override
  form `scale = 7.5f; if (size < 0x780) scale = 5.33f;` gives the same instructions with the restore `pop edi`
  on the other side of the default store), ai_CurseWindow (a meaning bug that had looked like a slot hunk:
  `[esp+0xd4]` against `[esp+0xd0]` in one store was msg.arg[2] where the original writes msg.arg[1]). Rule: a
  one-instruction "slots" hunk inside a record fill is a wrong field index until proven otherwise. Run the sweep
  again after base work; `fillloop.py` and `blockscope.py` take a few minutes over the whole queue.
- **main_OpenScreen byte (158 ins, stuck): five ordinary changes.** (1) A value the original reloads from ONE
  stack slot for two variables on a failure path (`mov ecx,[slot]; mov edx,[slot]`) was read as "both get that
  value" (`w = h = ex;`); they are two locals left unset on that path, which the frame packer merged into the
  third local's slot. Deleting the assignment gave the original's spill of ex and `call ebp` twice. Try deleting
  a "default" assignment before reasoning about what it means. (2) A call result the original stores to a slot
  and pushes after the next call: both calls assigned to named locals in call order (`ex = GetWindowLongA(..);
  style = GetWindowLongA(..); if (AdjustWindowRectEx(&rc, style, 0, ex))`); the second call inline in the
  argument list pushes the first result at once. (3) An `||` of `(a && b)` terms whose one arm is `jne out; jmp
  body` where ours has `je body; jmp out`: a flag set by separate ifs (`bad = 0; if (res == 0 && !has800) bad =
  1; if (..) bad = 1; .. if (bad) {..}`); VC6 threads the flag away and keeps that layout. (4) A scratch
  register shift (eax / ecx / edx rotation) through a long store block and everything behind it: ONE statement
  at the wrong place (here a read-modify-write of a global byte in the middle of the config stores). Run a
  move-one-statement round first (n^2 variants): 66 -> 6 ins. (5) Plain store order for the rest.
- **main_RoundReport byte (159 ins, "tie-break sensitive" per headers sample): six ordinary changes.** The big one
  (159 -> 40): two accumulators that start from the same value were NOT a copy of each other. `cash = wage;
  income = cash - spent; .. cash = income;` became the expression written out for both, cash first: `cash =
  c->wage - c->spent; income = c->wage - c->spent; if (c->wage != 0) ..` (the original's `mov ecx,edx; sub
  ecx,edi; mov edi,ecx; mov [slot],ecx`: one temp fanned out). A copy `b = a` between two long-lived
  accumulators changes the whole allocation and the frame; named temps for the operands leave 8 ins. Then: `cash
  -= x; costs = x;` (not `costs = x; cash = income - costs;`); several `acc += arr[k]` lines of a loop body in
  array-index order (1 of 24 orders is byte: it decides which accumulator gets the free callee-saved
  register); `(o->interest * 0.01) * o->amount` (int * 0.01 * int unparenthesised gives `fild; fimul; fmul`,
  the original's `fild; fmul const; fimul` needs the parentheses; a float / double local is no lever); a sum
  used twice through one temp (`t = a + b; net += t; income += t;`); plain store order.
  Lessons: (1) `dcp headers sample` saying "sensitive" on a function with dozens of differing instructions
  says nothing - the cause was in the source; never stop on it. (2) The distance is not monotone while a
  scratch-register rotation is out of step: judge a block's spelling after the blocks before it are right.
- `uv run --with capstone python tools/c2dag/vdiff.py SRC FUNC [VARIANTS LABEL]` prints the instruction diff of
  one dcp try variant against the original without touching the tree (dcp try prints only the distance; three
  agents had each written their own version of this).
- **main_SearchNetworkGame byte (348 ins, "every candidate split and permuted"): one missing local was 330 of
  them.** (1) A cheap expression that the original keeps in a callee-saved register across calls and later
  modifies in place (`mov edi,[i]; shl edi,7; push edi` .. four calls .. `add edi,0xc; push edi`) is a NAMED
  local assigned where the original computes it (`y = i << 7;` after the d2_SetForm call, then `f(8, y, ..)` and
  `g(0x88, y + 0xc, ..)`). Written inline twice VC6 makes no candidate and recomputes; the register then goes
  to other values and the whole allocation permutes. The position counts (one statement earlier: 201 ins).
  Look for this FIRST when "everything is split": a register of the original that holds a value ours never
  names. (2) `[esp+reg+const]` for several fields of one stack record, the scaled index kept in a callee-saved
  register: `arr[idx].field` at every use; a `p = &arr[idx]` local gives `[reg+off]`. (3) `xor r,r` before
  the `and` whose flags feed the loop-entry `jle`: the bound read in the for condition (`k < hdr.players`), or
  `k = 0` before the bound local's assignment; a bound local assigned first gives `mov r,0` after the `and`.
  Also: the earlier worker's changes (strcpy for hand loops, btn[4] for the frame hole) had RAISED the distance
  281 -> 348 and were right; a distance that rises after a provably correct change means one more thing is
  missing, not that the change was wrong.
- The mirror of main_EndRound's lever: there a named local had to go (the original used an expression
  temporary), here an inline expression had to become a named local. Decide by the original: a value that sits
  in ONE callee-saved register through several statements and is updated in place was a variable; a value that
  appears in a leftover register for one expression across one call was a temporary.
- **surf.c's scaling family, 2026-11-14: three functions byte from the siblings' spelling.** surf_Down32 (199 ins)
  was surf_Shrink16's text with DWORD pixels: plain `for (j ..) for (i ..)` index loops, `n++` per pixel, the
  unused bit counts `for (rb = 0; m; m >>= 1) rb++;` (VC6 keeps only the empty shifting loop) - byte in one
  round; the hand-written `do { .. } while (--xx)` / `sp++` / `n += x1 - x0` form was the local optimum.
  surf_Up24 (424) / surf_Up32 (512): the bilinear growing is ordinary code, `lr = r00 + (r10 - r00) * j / dy;`
  per row and `(lr + (hr - lr) * i / dx)` per pixel; VC6 strength-reduces both products into the accumulators
  and hoists the deltas, which earlier passes had written out by hand (`ar0 += dr0`, `if (dy > 0) do ..`).
  That gave identical instructions with a frame 0x18 short; the last step was the corner values as one array
  per channel, `DWORD r[4], g[4], b[4]` (0 here, 1 right, 2 down, 3 down right). Reading rule: the original's
  frame kept g00 g01 g10 g11 r00 r01 r10 r11 in eight CONSECUTIVE slots and had an 8-byte untouched hole right
  behind b00 / b01 (b[2] / b[3], whose values VC6 forwarded to registers); twelve scalars are packed with
  other locals by lifetime instead. Consecutive slots in index order plus a hole = an array. Up32 also needed
  `s[IDX]` read per channel (a pixel local `p = s[IDX];` gives other code).
- **A header member to give a struct copy (main_SettleRound, not kept).** `ev[0] = m->flags1b4 | 1; ev[1] = ..`
  against the original's `test byte ptr [edi+0x1b4],1` followed by three fresh loads: the original copied the
  four ints as a struct (`ws = m->work; ws.flags |= 1;`, AIWORKSET; a cast view gives the same code). Adding
  the member to AICHAR (an anonymous union of the struct and the four fields, any order or tag) dropped
  ai_TutorialInitChapter2 / 3 (sv_t, tie-break sensitive: byte in 17 of 32 headers samples) - so a header
  change for one function has to be checked against the whole build's byte count, and tie-break sensitive
  functions elsewhere can veto it.
- **main_SettleRound byte (513 ins, the first difference at +0x8): nine ordinary changes.** The one that decided
  the allocation (513 -> 397): three bits of one word tested as an `||` of separate tests, `if ((c->flags &
  0x800) || (c->flags & 0x1000) || (c->flags & 0x2000))`, not `(c->flags & 0x3800) != 0`. VC6 merges them into
  the original's `mov ax,[esi+0x5a]; test ah,0x38`, but takes the reference counts from the `||` form - the
  loop pointer then gets esi and the zero ebp. flip.py had said "the pointer is short of about two
  references". Then: four fields copied and modified as ONE struct, block-local (`AIWORKSET ws; ws = m->work;
  ws.flags |= 1;`, not `int ev[4]` filled field by field); `d = (rnd & 0xffff) + 2; f(c, -d);` for `add eax,2;
  neg eax` (inline gives `mov r,-2; sub`); hand-advanced `pn` / `sub` do-while loops as plain `for (pass..)
  for (j..) workers[1 + pass * 12 + j]`; a worker list and a message as block locals sharing one slot (locals
  of different sizes do share); message store orders (`type, to, fromId, ..`, 120 permutations each); two
  pointer locals where the original keeps `&ai_CharsT[w]` in a register; NO local for a value used once
  (`skill`: the instructions were already equal, it only flipped two equal-density slots).
- More compiler facts from it: a two-level strength-reduced index loop reloads its pointer slot at the outer
  latch (`mov eax,[slot]`) and stores it back at the head - a hand-advanced pointer has no reload. A load
  sitting above a `rep stosd` is the eax / ecx / edx rotation one step off (a statement-order fix in that
  block), not an escape question. The frame's density sort is the CRT qsort (middle element as pivot,
  selection sort for 8 or fewer); a tie hunk can be simulated from the B rows of c2frame.py. When only slot
  swaps between equal-density slots are left, remove named locals for values VC6 would keep as temporaries.
- **surf_Up16 byte (573 ins) in one variant: surf_Up32's text with WORD pixels.** The sibling family rule again: the
  @deep comment had a hand-derived data flow (row deltas hoisted, accumulators, one OR expression); the sibling's
  plain loops and `r[4] / g[4] / b[4]` corner arrays were the original. Copy the byte sibling FIRST.
- **d3anim_RootPosAt byte (144 ins): two pointer locals too many.** `f0 = &data->frames[s->frame]; f1 = ..;` in the
  else branch were allocation candidates of high priority (144 / 126) that took eax / ecx and pushed the
  parameters s / out into esi / edx for the whole function (the original: s eax, out ecx, loaded before `sub
  esp`). Written as `data->frames[s->frame].move.x` at each use (CSE'd temps, no candidates): byte. Reading rule:
  when parameters sit in callee-saved registers in ours but in scratch registers in the original and no call
  needs that, look for named pointer locals the original did not have.
- **A loop-invariant division is hoisted as a reciprocal on the FPU stack** (d3anim_CreateMorphAnim: `fld [1.0];
  fdiv [d.x]` x3 before the loop, `fld [v.y]; fmul [255.0]; fmul st,st(2)` inside): the source was `(v.x *
  255.0f) / d.x` with the parentheses. VC6 /O2 turns the invariant division into a multiply by a hoisted
  reciprocal temp (the 1.0 is the pooled literal, the address dcp names ai_FloatOne). Named `sx = 1.0f / d.x`
  locals spill to stack slots (never FPU-resident), `v.x * 255.0f / d.x` unparenthesised and `v.x * (1.0f /
  d.x)` reassociate to `R * v * 255` (fld st(N) copy first), `ai_FloatOne / d.x` written out is folded oddly.
  The same function: bounding-box min / max as uniform `min(mn.x, tmp[i].x)` / `max(mx.x, ..)` macros over a
  plain index loop (a hand-advanced `t++` pointer and mixed ternary / if spellings gave int-mov stores and
  `and eax,0x100` tests); three 12-byte vectors in a frame 0xc short = D3VECTOR4.
- tools: c2trace.py / vdiff.py / flip.py did not find a definition whose parameter list wraps onto a second line
  (StopIteration); function_span now follows the list to the `{` and flip.py uses it.
- **d3anim_PoseBones byte (311 ins): six ordinary changes and one paren.** (1) `if (n == 0) { rest pose } else {
  final loop }` after the active block, not `if (n) { ..; return; }` inside it (the original lays the rest pose
  first behind `test edx,edx; jne`): 310 -> 168. (2) The frame 0x10 too big: v declared in the slot block and rot
  in the final block, so the two vectors share one slot. (3) Two different "mesh" locals: the new-bone branch
  has its own block pointer (`D3ANIMMESH *skel = ..->mesh;` BEFORE the strcpy, the original loads it first),
  the rest pose another (they sit in different slots in the original): 168 -> 130. (4) The rest pose reads
  `D3_MESH(c)->boneName` at each use, no `*(name = ..)` (the original keeps that address in a slot of its own):
  130 -> 30. (5) `float s; D3VECTOR rot;` in the final block (rot 12 bytes; the block scalar moves rot to +4 in
  the shared slot, the original's [ebp-0x24]): 30 -> 24. (6) `map[b] < 0xff` (jae), not `!= 0xff`.
  (7) `fld [s->weight]; fmul [v.x]` against ours `fld [v.x]; fmul [s->weight]`: every operand order, cast and
  temp is one code, but a parenthesised operand is not - `((s->weight) * v.x)` and `acc[i].pos.x += (s->weight)
  * v.x` give the original's order (dcp headers sample: insensitive; flip.py: 0 hits, it never adds parentheses).
  VC6's C1 ranks a parenthesised primary differently; a macro argument `(w)` is the likely origin. Rule: an
  fmul / fadd operand order that no statement order or operand swap moves - try parentheses around one operand.
- `1.0f / x` against `ai_FloatOne / x`: dcp's M10 says the original reads ai_FloatOne (0x5eb780), but that is the
  address of the pooled literal __real@3f800000 - the literal gives byte, the extern const gives other code
  (d3anim_PoseBones). Write the literal unless the function needs the object (a read the literal would fold).
- **What "tie-break sensitive" is, mechanically** (read from color.c on d3anim_CreateMorphAnim, the one function
  of the day that is still open at 16 ins). (1) Reloads at a block edge behind a register-killing region (an
  inline-asm block, a forced split) are emitted in ascending candidate id of the split pieces. Candidate ids
  are recycled from a LIFO free list, so two split generations come out in opposite orders. (2) The initial
  ids of parameters and locals depend on the order in which unreferenced constant candidates are freed, a
  64-bucket walk keyed by `value & 0x3f` for integers and `(unit symbol ordinal) & 0x3f` for every string
  literal and global whose address the function uses. That ordinal is the front end's running symbol count,
  so everything declared before the function shifts it: this is what `dcp headers sample` perturbs, and why a
  block-scope extern or an unused local in the function moves such a hunk (period 64). (3) So a function is
  really tie-break sensitive only when it has a forced split with two or more reloads at one edge; anything
  else that `headers sample` flags has its cause in the source (main_RoundReport). (4) Even then the
  declaration set may not be the answer: here all 4096 bucket states give one of two orders and the original
  has a third, i.e. the source still differs in which candidates exist. (5) Base and index of a two-register
  address are the first and second operand of the IL add, fixed before strength reduction; hash state does not
  move it, the sharing of loop index variables between the function's loops does.
- **Two arms that start with the same call, the original loading its operands above the branch** (`mov edi,[npix];
  cmp ..; mov eax,[size]; push str; je`, only the `sub` in each arm): VC6 hoists identical leading instructions of
  both arms after register allocation, so the arms must get the same registers. A named argument in each arm
  (`n = s->size - s->npix; r = mem_Alloc(n, ..);`) did it (d2_ShpConvert24To16, byte).
- **`mov eax,1` right after the prologue, one path returning it untouched**: a result local whose calls assign it
  (`r = 1; .. if (mode < 0) { if (on) r = f(); else r = g(); } return r;`); `return f(..)` per call folds r
  (d2_DrawShapeScaled 66 ins -> byte).
- **A big draft called "allocation only" can still hold a wrong index**: d3anim_StepObject's tangent clear read
  key / next where the original reads nkeys / key ([esi] vs [esi+4]). Holes of 4 bytes after every 3-float group
  of the original's frame = D3VECTOR4 locals; declare them in their blocks (0x68 -> 0x9c of 0xbc).
- **A short global the original loads as a word where ours loads a dword and widens the arithmetic** (`mov ecx,
  dword [g]; sub ecx,eax` vs `mov cx, word [g]; sub cx,ax`): the global was tentative (COMMON, `short g;`, no
  initializer), so VC6 did not know it was 4-aligned. Probe with `extern short probe;` in a dcp try variant;
  make the neighbours tentative too (d2_ScreenH / d2_ScreenW, d2_ResizeWindow byte).
