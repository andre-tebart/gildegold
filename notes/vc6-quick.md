# VC6 SP5+PP /O2: the deep worker's guide

Read this file whole before the first item; it is the method. The cases behind it (about 1000 rules, each "what
the diff shows -> what the source was", with the functions it was found on) are in `notes/vc6-matching.md`,
grouped by the symptom you see in the diff, with a contents list of sub-symptoms at its top: go to the section
of your hunk class, or search by instruction or construct (`grep -n "fst \|rep stosd\|shrink" notes/vc6-matching.md`). Data, link and layout rules are in
`notes/vc6-data.md` (not needed for matching a function's code). A function named in a note is byte unless the
note says otherwise: `dcp fn NAME` shows the source that matched. Float constants read from 0x5eb780.. (and other
`__real@` runs) are VC6's pooled literals: write `1.0f` / `0.0f` / `0.5`, never an object (vc6-data.md).

## 1. The premise: the original was ordinary code
Every function that an earlier pass had given up on and that matched later had the same history: an earlier pass reached its best
distance with an unusual construct, and the byte match was plainer code. The remaining hunks then looked like
"allocation only, no source lever", and never were. So:
- **List what a game programmer of 2001 would not have typed, and replace that first**: a `goto` (to a shared
  exit, into another arm, past an increment), a `volatile`, a hand-advanced pointer or byte offset, a hand-written
  string copy loop, `memset` of an id array, a local that only renames a global, a "default" assigned at the top
  and overwritten in one arm, two arrays where one would do, raw `(char *)p + n`.
- **A small distance behind an odd construct is a local optimum, not a near miss.** A trick fixes the hunk it
  was aimed at and changes what VC6 sees elsewhere (a block layout, a reference count, an operand rank).
- **The distance is not monotone.** A provably right change can raise it (main_SearchNetworkGame 281 -> 348
  before byte): the scratch registers rotate through the whole function, so everything behind an unfixed spot is
  out of step. Keep what is visibly the original's (copy shapes, frame size, block layout) and go on from there.
  Choose between variants by WHERE they differ, not by the count: the right layout with a wrong detail is one
  step from byte (vfs_Tfopen: the variant that led to byte stood at 98 ins, the dead end at 6).
- **The lever is rarely in the hunk's own statement.** It is another `return`, a declaration's scope, where an
  assignment stands, a statement 60 lines earlier. Variants of the hunk's statement get one generated round.

## 2. Before the first variant
1. `python tools/sweep/twin.py NAME`: a byte function with nearly the same statements (the game is full of
   copies: main_ / lobby_, sv_ / sv_t, the surf_ scalers, per-profession handlers). Its differing lines are your
   first variants. After a match, `twin.py NAME --all` finds the copies still below byte.
2. Same-unit callees first: VC6 knows which registers a function defined above leaves alone and keeps values
   in ecx / edx across the call. A callee below byte moves its callers (gm_PlayerRecord to byte took its caller
   45 -> 12). Check the callees' grades; match them first.
   Layout is deep work and comes first: when a hunk only matches with a callee VC6 does not know (a renamed
   external copy in a scratch unit), first probe what the callee's text tells VC6 (the callee summary in
   vc6-matching.md: escape per pointer parameter, the globals it writes, taken from the text, dead code included;
   `tools/c2dag/unitprobe.py`). A pointer passed as an `int` parameter escapes the caller's local and keeps the
   callee's bytes (cm_SendCutEntry -> cm_FightFrame byte); only when callers on both sides of an
   address need different knowledge is it a unit boundary: split the unit yourself (dcp-layout skill), prove no
   grade falls, commit `layout: ...` as an item.
3. Read the hunks: `dcp asmdiff NAME --changed --lines`, `dcp diffclass NAME`, `dcp frame NAME` when slots or
   the frame differ. Write down per hunk what the ORIGINAL does (which value sits where, loaded when), not what
   ours does wrong.
4. Meaning before allocation. A one-instruction slot hunk inside a record fill is a wrong field index until
   proven otherwise; a store into "another local's slot" is another variable; a redundant test, a reload, a
   second load of a parameter are statements of the original. Section "It was a meaning bug" in the reference.
5. The declaration set is dcp's business, not yours. VC6 breaks some register ties by how many symbols were
   declared before the function (period 64), so headers can move a function's code. dcp sweeps that by itself
   (in `dcp try` on a close row, in `dcp grade`, in `dcp build` for a function that fell) and prints one of two
   lines: "this text is byte under a declaration set: done (proven)" - the item is finished - or "no declaration
   set makes this text byte: the difference is in the text, go on". There is no third case and nothing to
   interpret. A function another function's or header's change moved, with its own text unchanged, is no fall.
6. Read the function's `@deep` note (`dcp fn NAME`): what earlier sessions found and which rounds they ran.
   Do not repeat those rounds. `attempts=N` in `dcp next --deep` says how many sessions did not finish it.
7. `python tools/search/climb.py NAME --rounds 1`: every single ordinary mutation of the function (a statement
   moved, a named local for a value read twice, a local written out, an assignment moved into its expression, a
   statement in both arms, operand order ..), each measured. About a minute; it found msx_Update's byte and took
   dd_SetMode 68 -> 28 where two sessions had not. Read its best rows before thinking of variants yourself.

## 3. How VC6 decides (read from c2.dll; these predict which lever can work)
**Registers.** Allocation candidates are named locals, parameters, CSE temporaries, a scalar global between its
kills, and constants used more than once (one candidate per value, whatever the spelling). Expression
temporaries are no candidates: they get what is left.
- Candidates are served in descending priority (equal: the later-created first). Priority sums references
  weighted by loop depth (`1 << depth`); a block where the candidate is dead costs it nothing; savings below 1 =
  no register.
- Live across a call: only ebx / ebp / esi / edi. Referenced by a byte-sized operand: only eax / ecx / edx / ebx,
  so a byte value across a call needs ebx, and higher candidates then avoid ebx and take ebp. A pooled constant
  compared with a byte (`kb_Last == 1`) makes every use of that constant byte-flagged.
- When the fourth callee-saved register is taken, everything else live across a call is split; the pieces come
  back with new priorities and can overtake older candidates. A candidate must be served before that.
- Scratch registers (eax, ecx, edx) for temporaries are handed out round-robin in code order through the whole
  function. One temporary more or fewer rotates every scratch register behind it. A local that got a register
  does not advance the rotation; a direct global read does.
- So a register hunk is never "which spelling" but "which candidate exists, and which must rank above which":
  - a value the original keeps in one callee-saved register through several statements and updates in place was
    a NAMED local; a value in a leftover register for one expression across one call was written inline
    (main_SearchNetworkGame / main_EndRound: the two directions of the same lever);
  - one more or one fewer reference: the expression written out again instead of a temp, a statement written in
    both arms of an if (VC6 merges the code, the references count twice), an extra definition of a flag;
  - spellings that fold before allocation (`x = y` with y known, a named copy of a global, casts, a `const`
    local, a single-use local) change nothing.
  `python tools/c2dag/c2trace.py SRC FUNC` prints every candidate piece with its priority, savings and outcome.
- Priority per block: `+ refcost * P * w` where referenced, `- P * w` where it lives through unreferenced, P = the
  distinct values referenced in that block: a reference in a crowded block weighs more, a value live over many
  blocks pays each one (`c2trace --prio REGEX`).
- A value spilled to its slot but held in a callee-saved register over a few blocks is a split piece: in functions
  of 100+ basic blocks, when one assignment leaves 3+ candidates without a register, the blocks form regions (each joins its FIRST predecessor's region if a register stays free for both)
  and a piece lives from its first to its last reference in its region. The predecessor order follows the source:
  an extra empty block behind a loop (label, `do { } while (0)`, a goto target) puts the loop's guard edge first
  (ai_FillNearRing; `c2trace --split REGEX`; vc6-matching "Live range splitting, regions and the priority
  terms").

**Stack frame.** After allocation every local in memory is listed by size ascending, then reference count
descending (one per operand naming it, all blocks, no loop weight, counted BEFORE the late passes: a statement in
both arms counts twice). Each joins the first lifetime-compatible slot or opens one; frames above 0x80 bytes are
sorted by count * 1000 / size with an unstable sort, the densest slot nearest esp.
- Declaration order, names and types never enter. A slot-order hunk is a count question (which local needs one
  reference more or fewer) or a table-position question.
- Block locals with disjoint scopes share a slot even when their addresses go to calls; function-scope arrays
  never do. A buffer or record used only inside blocks belongs in those blocks: it changes the slot table and
  the scheduler (below). `python tools/sweep/blockscope.py NAME` tries it.
- `dcp frame` "shared in the original" for two arrays plus a hole of their size = ONE array of twice the size
  used for both (main_ChoosePlayer `int row[4]`). An untouched hole behind a small local passed to a call = the
  local is the callee's full record. 4 bytes per vector too many in the original = `D3VECTOR4` locals. A frame
  one slot smaller in ours = a local sits in a dead parameter slot that was not dead yet in the original (its
  first assignment stood earlier). `python tools/c2dag/c2frame.py SRC FUNC --refs` prints counts and the table.

**Instruction order.** Per basic block (a region ends at a jump, a label, or after 81 instructions) VC6 emits the
ready instruction with the highest priority: height (longest path to the block end), plus a bonus for a memory
load; ties keep the order before scheduling. Loads are hoisted; an instruction overtakes an earlier one only
when something later in the SAME block depends on it.
- A pointer load is ordered after every earlier store to an address-taken local whose address already went to
  a call. A fresh block local has not escaped: declare message / vector / buffer locals in the block that fills
  and sends them before any other lever on a store-order hunk (AIMSG msg, pos / rot, key).
- Otherwise an order hunk means: another list order (the expression tree: parentheses, a temporary, an
  assignment inside the expression: `(depth = 2600.0f - f) * t`, `SND_SET(h, n, v = expr, 0, 0)`), another
  dependency (a later statement of the block reading or overwriting it), or another block extent (a label).
- The 81 count the fxch / pops and one node per parenthesised float product (c2dag `op=263`): a macro nested to
  force a multiply order lengthens the region, and what the original hoisted (argument pushes) falls into the
  next one. Unparenthesised float products are ordered by VC6 itself (two globals: the one met later in the
  function first), so the first product of the function decides the rest (ai_StepShot, vc6-matching "Deep s39").
- One statement at the wrong place in a long store block rotates the scratch registers of everything behind it:
  run a move-one-statement round (n^2 variants) before judging spellings (main_OpenScreen 66 -> 6).

**Exits and shared tails.** VC6 duplicates a short return block into its predecessors, merges identical
instruction tails after allocation (cross-jumping: up to 30 instructions before a `jmp` against those before its
target, bottom-up), and merges `add esp` along a straight run into the last one.
- Callee-saved pushes sit after the early guards only with `if (..) return K;` guards plus ONE final return;
  every further `return` inside a branch puts them back in the prologue. Count the returns first
  (snd_FindVoiceSample).
- A store before its call's `add esp`, with a label right behind = two cross-jumped copies of that code: write
  the code out at each exit, no `goto` to one copy (d3sndw_Update). Which copy survives and which sites merge
  follows the per-site register and store order.
- One failure block inline at the last failing test, the success path falling into the end = no `goto` at all:
  each step sets the result and the next runs under `if (!ret)` (sb_Play).
- The return block in front of the epilogue label is the first one a depth-first walk finishes that follows each
  test's jump edge first (the code after a goto test / if body / loop, the then-part of an `||`); it absorbs
  the equal endings of the other returns (cross-jumping, no size limit). Two copies both reached by
  fall-through merge only above ~22 bytes. Only the flow graph moves this, never text order or labels:
  `c2tail.py SRC FUNC --tries --il merge` (vc6-matching "How VC6 orders blocks and merges tails").

**Tests that leave no code.** VC6 decides a test whose outcome is known on every path (`if (f)` after f was
dereferenced or just set to 0, a flag set by constants) and emits nothing for it, but the block layout and the
value's lifetime are those of the test. So the original's plain `p = 0;` + `if (p) {..}` + one `return p;` is
invisible; ours with `return 0` at each failure or a `goto` differs in the epilogues, the result's register or
which exit owns the final epilogue (vfs_MemOpen, vfs_Tfopen).
- Traces of a decided test: `xor eax,eax` in the prologue with the early exit joining behind the final `mov
  eax,reg`; a reload `mov reg,[slot]` before a return that "must be 0"; a jump into the middle of a sibling arm
  just behind a test of the same variable; an out-of-line else whatever the condition's spelling (a flag chose).
- Conversely `return f;` where only `f = 0` reaches it is folded to `xor eax,eax`: `mov eax,reg` at such a
  return means the path ran through the shared final return.
- When a shared tail is right in layout but off in detail, sweep the join position: how many trailing
  statements are shared (0, 1, 2, ..) is its own round (vfs_Tfopen: only the counter was shared).

**x87 stack.** Float values live in memory between statements unless the FP allocator (class 1, FUN_1076697d)
lifts them onto the FPU stack; candidates are float / double locals, each field of a local struct or array, CSE
temporaries. Savings per reference times `1 << loop depth`: a float definition +2, a use 0, a dying `fld` +1 (a
double: +4 / +1 / +2), -w per block edge it is live across; <= 0: memory. Highest priority first, a slot only if
its live range nests with the placed ones (born later => dies no later). A dying value pops after its statement's
store; the op pops it (`fld st(0); fmulp`) only when the statement's result goes to a stack local (`d = sqrt(dz*dz
..); m->x = d;`), via the /O2 peephole FUN_10765f00. `python tools/c2dag/c2fpu.py SRC FUNC` (vc6-matching "How
c2.dll allocates the x87 stack").

## 4. Symptom -> what to try first
Section names are those of `notes/vc6-matching.md`.

| The diff shows | First levers | Section |
|---|---|---|
| pushes / pops of ebx, esi, edi on the wrong side of an early exit; an epilogue copy more or fewer; `xor eax,eax` placed elsewhere | count the returns; guards + one final return; `ret = x` per step under `if (!ret)`; `p = 0` + `if (p)` for each `return 0` after a cleanup; the code written out at each exit instead of `goto` | Returns |
| a return's ending merged into another exit (or two copies where the original jumps into one); the default / not-found block in front of the epilogue | `c2tail.py --tries --il merge`: which block falls into the exit label; change the flow graph so the walk finishes the original's block first (`\|\|` guard, goto test, nesting); text order and label moves are no lever | Branch layout (How VC6 orders blocks and merges tails) |
| an arm out of line whatever the condition; a jump into another arm; a redundant re-test; `setne` against a branch | a flag set by separate ifs; a search loop left by `goto` / `break` + re-test as a found flag (`found = 1; break;` .. `if (!found)`: VC6 decides the test per path, no compare); the excluded case in the outer condition; if / else-if with the call in both arms; a ternary against if / else; swap the arms | Branch layout |
| the same instructions with callee-saved registers swapped, or every scratch register shifted from one point on | c2trace; named local against inline expression; an expression written out twice; statement written in both arms; the init order at the top; one early statement moved (n^2 round); count the exit / free statements (one fewer exit block lowers a pointer's priority); a second loop variable as an expression of the counter; clamps as min / max ternaries, not `x = a; if (x >= b) x = b;`; a default assigned first and overwritten in one arm as if / else or ?:; a loop counter set before the invariant pointer | Register allocation |
| a value reloaded into a scratch register at each test where the original keeps it in one callee-saved register over a block (reloads at the loop exits) | `c2trace --split NAME`: which region each test joined; a label / `do { } while (0)` / goto target behind the loop in front of the test | Register allocation (Live range splitting) |
| `[esp+N]` offsets differ, frame size equal or not | dcp frame; block-scope the block-used locals; one array for two; the callee's full record; D3VECTOR4; reference counts (c2frame); one variable for two phases (two counters -> one); a later value kept in a dead parameter (@original) | Stack frame |
| a clamp's `v = -0x7f` arm out of line (`jle` / `jl` to a block far below) | `min(max(v, lo), hi)` with the min / max macros; `max(v, lo)` gives `jle`, `max(lo, v)` gives `jl` (cm_ExRelations) | Branch layout |
| one scratch register for a value stored at every exit, a reg-reg copy in one arm | the arms set only what differs (else-if chain) and fall into ONE shared `store; return;` that VC6 duplicates into each arm (cm_UpdatePointer) | Returns |
| `fdivr st,st(1)` + `fstp st(0)` after the store where ours has `fdivp` | split the expression: `r = a * b; r /= c;` (the product is r's first definition on the FPU) | Floating point |
| a slot pairing that needs a swap temp to share with a counting variable | the counting variable WAS the swap temp (`st = open[k]; .. open[k - 1] = st;`): reuse it (gv_RankCulpritsAll) | Stack frame |
| stores / loads in another order inside a block | block-scope the filled record, then climb `--base FILE:LABEL --ops move,swap` from that variant; permute the fill (720 / 5040 orders in one `dcp try`); assignment inside the expression; a read into a local before the other store; drop explicit `(float)(..)` casts on float stores (each is a scheduler node) | Order |
| a loop with another induction variable, a pointer walk against an index, `dec; jne` against a compare | the plain index loop over the real element type; the callee's real parameter types (a `short` parameter stops strength reduction); the bound in the `for` condition; `break` + one return | Loops |
| callee-saved registers swapped pairwise, the original testing a pointer against a counter's zero register | the counter's `n = 0;` right before its loop, not with the other inits at the top (ai_UseGoodFrameChar) | Register allocation |
| IV inits / temps / saves of a table search one step off, the source a hand pointer walk (`(int)c < (int)end`, `i++, p++`, `do .. while (1)`) | the plain index loop over the table (`recs[i].field`, or `p = &recs[i];` per pass); VC6 strength-reduces it into the same compare (ai_PickBuildingChar, amt_AcceptApplication) | Loops |
| a field load / argument push sunk after a struct copy or the stores of a record whose address went to a call in another arm or an earlier pass | the other arm's (or the loop body's) records as block locals of that block, then permute the fill (ai_DecideUseMilitaryObject, ai_DungeonBribeWindow) | Order |
| copy temps in esi with the pops at the end, ours copies through ecx and pops early | the statement stored before the copies stands after them (its load is hoisted and keeps a scratch register busy): move it, one round of positions (cm_SendFightCommand) | Register allocation |
| a ternary argument (`neg; sbb; add eax,K`) computed before the call's other argument loads | an int local set by if / else right before the call (`if (s) type = 6; else type = 7;`); `type = s ? 6 : 7` is forwarded (cm_ExCreateNamedCharacter) | Calls |
| a chained `a = b = c = x;` message fill that no order round moves | the chain as separate stores, then the order round (in a loop: the record block-scoped first) (ai_DenounceWindow) | Order |
| two calls of the same cleanup where the jump table sends two cases into one call block | the case falls through into the next case's code (`/* fall through */`) (he_RunTutorialBuildingTour) | Branch layout |
| a stored float reloaded from a spill slot where ours keeps a named float on the FPU | no named float: `x *= k; if (..) x += y;` on the element itself (cm_InitOrderChances) | Floating point |
| zero stores of a local array from two scratch zero registers (first element an immediate), ours a callee-saved zero register all through | the arrays with `= {0}` initializers (`int a[4] = {0}; int b[6] = {0};`): the initializer zeros never join the constant-0 web (ai_PokerOpenCounts) | Copies |
| 0 in a callee-saved register from the prologue (`push esi` for each 0 argument, `cmp eax,esi`), the original pushing immediates and clearing a small array from a scratch zero | unrolled clears `a[0] = 0; a[1] = 0; a[2] = 0;` as an element loop (or memset): they were the 0 candidate's references (ai_TradeSearchWindow) | Copies |
| a ternary text id (`neg; sbb; and; add 0x114`) computed into a callee-saved register before the call's argument loads | the base as an int local before the call, `text = (sex ? 10 : 0) + 0x114; f(text + b00d, ..)` (ai_DivorceDialog) | Calls |
| a table record address computed into a register before its field tests where the original forms it in place (`lea edx,[eax+ecx]; mov cl,[eax+ecx]`) | no record pointer local (`AIGOOD *g`): `ai_GoodsT[e->item].field` at each use (ai_EvaluateProductionGraveyard) | Register allocation |
| two values re-read after a memset / rep stosd, the original comparing two registers and reusing them in the fill | the values as locals read once before the fill (he_RunGathering) | Register allocation |
| `call f; fmul K; add esp; fcomp [param]` where the original has the fcomp before the `add esp` | the scaled result as a named float compared from the parameter's side: `half = f(c) * 0.5f; if (value < half ..)` (he_CharAccident) | Floating point |
| an init the original stores before an early guard; moving it there costs the zero register (esi -> ebp) | the loop counter's `i = 0;` right before its `while`, not with the other inits at the top (sv_MessageBoxGreen) | Register allocation |
| two callee-saved registers swapped through a whole handler, sex-dependent text ids `(c->sex != 0 ? 10 : 0) + K` in several calls | each id as an int set by if / else before its call, a subset round over the sites (he_RunTitleCeremony) | Register allocation |
| a table record's byte offset kept across a memset / rep stosd (`shl edx,2; mov al,[edx+X]` .. `mov eax,[edx+Y]`) where ours forms a record pointer | an int index local read at each use, `o = b->owner; ai_CharsT[o].kind .. ai_CharsT[o].netId` (he_RunProtection) | Register allocation |
| `fld K; fdiv b; fmul a` where the original has `fld a; fmul K; fdiv b` (float context) | parenthesise the product: `(a * K) / b`; `(m * k1) * (d * k2)` against a flattened chain (he_RunMarketSupervisor) | Floating point |
| `lea edi,[esi+1]` spilled, `mov esi,edi` at the latch: the counter not strength-reduced | the counter advanced in the body after its last use (`sprintf(.., i + 1); i++;`, the header without `i++`) (cm_SetupFightScene) | Loops |
| a later exit jumping back into an early guard's `free; return` block | the late exit as the else arm: `if (ok) { .. } else free(ev);` (he_RunMarketSupervisor) | Returns |
| a handler far too long, return blocks and news tails duplicated, the source full of `goto fail` / `goto done` to one cleanup | the cleanup written out at each exit (`if (x == 0) { if (e->slot) release(ev); free(ev); return; }`): VC6 cross-jumps them itself (he_RunFeastExam 186 -> byte) | Returns |
| a constant 1 in its own callee-saved register (`mov edi,1; mov ebp,edi`) where the original has one register for the flag and every `push 1` | the flag (`idle = 1`) set right before the loop that clears it, not in its declaration at the top (he_RunRaid) | Register allocation |
| `-1` in a callee-saved register for a run of `a[k] = -1` stores and later compares | the stores as an element loop `for (i = 0; i < 3; i++) a[i] = -1;` (he_RunRaid; the s47 zero-clear lever for -1) | Copies |
| `cmp reg,-1; jle` with -1 held in a register, the original `test reg,reg; jl` | the test as `>= 0`, not `> -1` (rd_PathToRoad) | Branch layout |
| a count global's value in a scratch register between calls, the slot offset recomputed after the call, ours a named copy `n = G;` in a callee-saved register | no copy: `T[G].f` / `i < G` at each use, `G++` (the global is its own candidate between kills) (ai_FillSlotTable) | Register allocation |
| frame 4 bytes larger in a long handler of themes / cases, one original slot holding variables of several themes | each theme's locals declared in its own block, address-taken send ints block-local in the send block (he_SpawnFounders) | Stack frame |
| frame 4 bytes larger, an address-taken int (a field delta value) with its own slot | that int as a block local of the block that fills and sends it; the dead parameter slot then takes the later locals (cm_RunBirth, gm_PartnerStep) | Stack frame |
| `push 0` / `push K` hoisted above the tests that pick a call argument, one `add esp` at the join | the call written out in each arm (four nested arms): VC6 hoists the common pushes; a ternary argument is evaluated first (cm_RunBirth) | Calls |
| `push 1` / `push 0` where ours computes the flag | the call written in both arms of an if / else, not `c ? 1 : 0` as the argument (ai_OfficeApplicantsWindow) | Calls |
| an order hunk at the start of a long block, nothing in its own statement moves it | c2dag: a block cut at 81 instructions just before it; one parenthesised product more or fewer above moves the cut (d3_ClipPoly) | Order |
| a temp spilled and reloaded inside one expression, slot order one local off | named pointer locals (`fr = &a->frames[k]`) written out at each use: CSE temps take the registers, the pressure spills (ch_CheckAniMorph) | Register allocation |
| a handle or value loaded "too early", kept across the float code | no statement boundary there: `v = expr` inside the consuming expression or macro argument | Order, Floating point |
| `fld a; fmul b` against `fld b; fmul a`, `fst` against `fstp; fld` | init order of the two locals; a fully nested chain; assignment inside the product; an own local per branch; parenthesised member copy (`v.x = (p->x);`) | Floating point |
| `fmul st,st(1)` + `fstp st(0)` after the store against `fmulp`; a value in memory the original keeps on the FPU (or the reverse); an extra 4-byte float temp slot copied with `mov` | c2fpu.py: which candidate got no slot and why (savings, nesting); the result into a float local before the store; a `double` local for a value kept across a loop; the copy written as the same expression (a CSE temp) instead of a copy of the stored vector | Floating point (How c2.dll allocates the x87 stack) |
| arguments pushed before the test that picks one; a call "sunk" or doubled | the call written in both arms; the callee's prototype (char / short / unsigned char parameters); two calls assigned to named locals in call order | Calls |
| `movsx` / `and 0xff` / byte RMW differences, `lea` against `add; add` | the real type of the field, parameter or return; bitfields for byte RMW; `(c ? A : B) + x` against `(c ? d : 0) + A + x` | Types |
| `rep stosd` / inline copy shaped differently | the element loop against `memset`; `strcpy` against the hand loop; one array reused; struct assignment against field copies | Copies |
| a register used uninitialised on one path in ours (a reload from a home slot, one frame slot too many), the original's register holding the zero of the top there | the local starts at 0 (`int line = 0;`): the original's zero register was the variable's first value, and ours read it uninitialised (cm_FightStartDialog 276 -> byte; a meaning fix too) | Register allocation |
| two callee-saved registers swapped (k first in ours), a flag set in both arms of a roll | the flag set by the if / else and the work under a second test of it (`if (r > t) hit = 1; else hit = 0; if (hit) dmg = ..; else dmg = 0;`): VC6 threads the second test, but the blocks and priorities are those of two tests (cm_Attack byte) | Register allocation |
| a split piece in a register after a loop where the original reloads from the slot; the tail cross-jumps into another arm's | no `continue` in the loop body (`if (id != -1) { .. }`): `continue` makes the loop head the latch's first predecessor, so the code after the loop joins the head's region (cm_FightEndDialog 161 -> 28) | Register allocation (Live range splitting) |
| one shared loop (a pump) reached by `goto` from several arms, laid where the original has it but rotated | `while (flag && f())` at the end with the flag set in each arm: `&&` keeps it top-tested and VC6 threads the flag; `if (flag) while (1) { if (!f()) break; .. }` is the same (cm_FightEndDialog) | Loops |
| an index expression reassociated into `base + x` / `y * size` CSEs where the original keeps `x + y * size` whole in one register | the index read once through the table and once through the record pointer (`T[i].x + rec->y * size`): the mixed spelling keeps one CSE (cm_UpdateGrenades byte) | Register allocation |
| `fimul [tmp]` against our `fild [tmp]; fmulp` for a double times an int expression | a `(float)` cast on the double factor (no code of its own) (cm_FightEndDialog) | Floating point |
| a byte load into a register ahead of a branch (`mov cl,[ebx+0x7c]; cmp; jne; test cl,1`) where ours tests memory in the arm | a test of the same value with identical arms in the sibling branch (`else if (a->flags & 1) print(X); else print(X);`): VC6 merges the arms, the load stays in the dominating block (cm_FightEndDialog, @original) |  Order |
| a field reloaded after each call of a leaf defined above (the caller does not know the leaf) while earlier callers keep it | a unit boundary between the callers: `tools/c2dag/bprobe.py . UNIT ADDR` (only the callers above the boundary change), the strings' runs agree; split (cmorders from cmfight, cm_RunCutOrders 949 -> 436) | Declaration set |
| a frame one slot short and a loop IV (column offset) in a callee-saved register where the original keeps it in a slot and a parameter in that register, in a 100+ block window pump | a `continue` in ANOTHER, later loop (`if (in[k] == 0) continue;` in the column loop) written as `if (in[k] != 0) { .. }`: the region split of the whole function changes; `flip.py --want 'sub esp, <frame>'` finds it (ai_ProductionWindow, ai_SearchResourceWindow, ai_ProductionGoodsWindow's `if (type == 0x17) continue;`) | Register allocation (Live range splitting) |
| a zero register (`cmp edi,ebx`, `push ebx`) through a job / bar block and a constant (0x54) in ebp in an earlier block, ours a local in ebp and immediates | the local's `= 0` default at the END of the arm that needs it (`} else { ..; amount = 0; }`), not before the test: the early store made it outrank the constant (ai_UpdateResourceColumns, ai_UpdateProdColumns) | Register allocation |
| a dead read of a local (`mov ecx,[cls]` overwritten before use) and a slot ours lacks | the local tested with equal arms (`if (cls == K) f(X); else f(X);`): VC6 merges the arms, drops the compare, keeps the read (@original; ai_UpdateProdColumns) | Stack frame |
| a 64-bit argument pushed from `mov eax,imm` / `xor ecx,ecx` (the zero reused for other zero arguments) where ours pushes immediates | the mask through a `LARGE_INTEGER` set by halves (`mask.LowPart = K; mask.HighPart = 0;`), as the am / amr callers (ai_KidnapWindow) | Calls |
| two halves of one value in two locals (`half` in a slot, `x = half - ..` in a register) swapped | x computed from the source expression itself (`x = w / 2 - ..`), not from the other local: both share the CSE and x keeps its own priority (ai_DarkCornerBuy) | Register allocation |
| a wait loop's whole function one scratch step off from its prelude (`mov eax,[clock]; mov ecx,[wait]; .. add eax,ecx; mov ecx,[clock]`, ours `wait` in edx), the deadline in a frame slot; every spelling of `end = clock + wait; while (end > clock)` = control | the loop tests `start + wait > clock` with `start = clock;`: VC6 keeps the invariant sum as a spilled CSE (no split candidate piece), and wait gets two references (guard and latch). Locate such hunks with dead `clock;` read probes per position (h03), `{ static T *pm; pm = &x; }` and a second read of a parameter (ai_DuelDiceAnimation) | Register allocation |
| small slots paired differently in a frame > 0x80 (one local sharing with another the original keeps alone, a float pair in one slot) | replay the packer from `c2frame --names` (list by size / count, each local joins the NEWEST compatible slot) and search counts x conflicts for the original's pairs: two floats of disjoint phases were ONE variable (a letter loop counter reused as a fade value), address-taken out-params (`&px, &pz`) block locals of their block, a buffer `char x[MAX_PATH]` (lc_GebaeudeAusrichten) | Stack frame |
| a field store kept before a load through the same object where ours hoists the load (`mov [esi+0x211],cl; mov eax,[esi+0x1cc]`) | the object read through the array element each time (`flags[i]->objectFlags &= ~2; f(flags[i], flags[i]->info, ..)`), no copy into a local pointer: VC6 cannot prove the two accesses apart (lc_GebaeudeAusrichten) | Order |
| a flag in esi where the original has edi (and the pieces it races with swapped) | the flag's priority: `c2trace --prio NAME` per block; a statement written in both arms of the flag's test (VC6 hoists it back) lowers it below the candidate it must follow (lc_GebaeudeAusrichten: `model->stateFlags \|= 2` in both colour arms) | Register allocation |
| a counter of a list loop in a scratch-rotated callee-saved register where the original keeps it in ebp and the strength-reduced walks in slots (a byte temp spilled for want of a byte register) | the counter's `n++` written at the end of each arm that adds a button, not once after the if / else chain: the two references outrank the walks; then the loop inits as `for (i = 1, n = 0; ..)` and a pointer local computed BEFORE the call that may change its base (`type = ai_BTypes + kind; ok = gm_MayTakeJob(..)`); x / y places written inline as CSEs, not locals (ai_ConstructionWindow 309 -> byte) | Register allocation |
| the original pushes the callee-saved registers after an init loop, keeps 0 in a scratch register there and pushes every 0 as an immediate, reloads several handles into ebx / ebp / edi at the loop head (ours: a zero register from the prologue) | the bulk split runs in regions only from 100 basic blocks on: find the blocks the original has more (a picture set after an if / else chain by a second test of the same flag, `if (side == 1) .. else if (side == 2) ..`, whose arms then cross-jump; a `__min` clamp instead of `x = a; if (x >= b) x = b;`); flip --want on the prologue finds where blocks can come from (ai_ExchangeWindow 547 -> byte) | Register allocation (Live range splitting) |
| two row loads at a branch head in other scratch registers (the original computes both row offsets, then loads both goods; a reload of the other index in edx / eax) | the equality test on the rows themselves before the assignments: `if (own[idx].good == bank[sel].good) refuse; else { sold = own[idx].good; bought = bank[sel].good; ..}` (VC6 reuses the loads); per-side block locals for the side's floats and amounts, a separate local for a second result instead of writing it back (`cost = f(amount, ..)`) (ai_ExchangeWindow) | Register allocation |
| a down-counter `dec reg; test reg,reg; jne` (the test kept) and the 0 arguments after the loop pushed from that register, ours `jg` or the counter in memory (`dec [mem]; jne`) | `left--;` as a statement before the last statement of the body, the loop testing `left != 0`: the separate test is one more reference (the counter outranks the other locals), and the exit knows left == 0 (he_RunConversion: `left--; ring = (ring + 1) % 0x300;` in a `while (left != 0)`) | Loops |
| a field reloaded after an inlined strcpy into a local buffer where the original keeps it in al (`rep movsd; cmp al,1; movsb`), ev / the character swapped between ebx / ebp | the buffer as a block local of the block that fills and sends it (`if (c == own) { char voice[0x200]; ..}`): before its address goes to a call VC6 keeps the field's CSE across the copy; with it, the default-first chain (`officeNews = 0; if .. else if ..`) is the original's in both arms (he_RunOfficeAward) | Order |
| the argument registers of a call whose last argument holds a call (`util_RandomU(3) + K + x * 3`) permuted (ours the pointer of an earlier argument in ecx, the original pushes the computed argument first, then xor ecx / eax = pointer / edx = buffer) | that argument computed into a local in the statement before the call (`verdict = util_RandomU(3) + K + d * 3; txt_Format(line, K2, ch->id, verdict);`) (he_RunFeast; there also the AIGRP record as a block local of its case: the field loads before its stores) | Calls |
| the strength-reduced record pointer on another field (ours `ai_Chars+0x184`, the original `+4`, the netId) in a loop that tests several fields and then sends through the record | the tests read `ai_CharsT[i].f` and the record pointer `c = &ai_CharsT[i];` is taken only inside the then-block for the sends: a 2^n mask round over the `c->` uses (`dcp try` 1024 variants) finds it; each extra test reference on a field pulls the base to it (he_RunSendHomeEvent) | Loops |
| a cancel `stage = -1; return;` of an `||` guard kept inline behind the last test (`je rest` falling into its own epilogue copy), ours jumps to an identical copy far up (c2tail: merge by pair) | the guard as a flag set by if / else and a second test `if (cancel) { ..; return; }`: the single test lays the cancel behind it, VC6 threads the decided flag and the copy is reached by fall-through, so the pair merge leaves it (he_RunKidnap); its message fill as a block local AIMSG, type / to / arg[0] / arg[1] / time / fromId / flags (5040 orders) | Returns |
| nothing in the function's text moves it | callees above; a struct or extern in a shared header (put views in the unit); a macro body or type the function uses; `climb.py` with more rounds | Declaration set |

## 5. Tools
- `python tools/exe/exemeasure.py`: the whole exe against the original (sections, functions at their address,
  the first size mismatches and link-order pairs). `--queue` / `--unit U`: the size queue, functions whose padded
  size (distance to the next function, 16-byte alignment) differs, lowest address first. A size fix lines up
  every function after it; the right instruction lengths are enough, byte is the usual way there.
- `python tools/exe/datameasure.py [--steps --section data --unit U]`: the data side: each data symbol's shift
  against the original and the STEPS where the shift changes (size < room, wrong unit, wrong section, definition order).
  Locate a size gap before any variant: sum each asmdiff hunk's byte lengths on both sides (ours from `dcp disasm
  --ours`, the original's from `dcp disasm`); the hunks with a net difference are the work (dd_SetMode: all 10 bytes in
  one expression, so no statement was missing; evt_DeclareLocal: the failure tail). A variant's raw size without
  applying it: compile the unit with the variant (vdiff.py's build) and measure the function's COMDAT (trailing
  int3/nop stripped; a switch table counts).
- `dcp try NAME FILE`: one row per variant. Read the `ins` column yourself: `best:` says "none closer" when a
  variant has fewer instructions at a lower grade. `= control` means that source change is no lever for VC6.
  An empty replacement (`===` directly followed by `>>>`) swallows the next block: put a comment in.
  `--move-line N --to A..B` generates declaration moves. A whole-function variant starts with the function's
  comment (at least the `/* 0xADDR, ... */` line): without it `--apply` drops the comment and the `@deep` note
  and the function leaves the grades ("neither a unit nor a graded function").
- `uv run --with capstone python tools/c2dag/vdiff.py SRC FUNC VARIANTS LABEL`: the instruction diff of ONE
  variant against the original without touching the tree. Use it on every variant that changes the layout,
  whatever its count. x87 register forms, `-1` and `rep movsd` are compared in one spelling: a `-`/`+` pair is a
  real difference. vdiff, c2trace and c2frame read a variants file exactly as `dcp try` does.
- `python tools/c2dag/c2fpu.py SRC FUNC [--variants FILE --label L] [--cands | --code] [--lines A-B]`: the x87
  allocator's candidates (float locals, struct / array fields per definition web, CSE temps) with their def / death
  lines, prio / savings and outcome (stack, memory: savings <= 0, memory: no fit with the pieces it does not nest
  with), then each x87 instruction under its own source line with the simulated stack (st0 first) and its pops
  (`pop`, `dead pop` = `fstp st(0)`, `into st(k)`).
- `python tools/c2dag/c2trace.py SRC FUNC [--variants FILE --label L]`: per source name (temporaries with their
  first line) each candidate piece `NAME#N` with its pass, reference lines, prio / sav / avail and its outcome
  (register, memory, cut by pressure where X took R, split alone / in regions), the final registers, and whether
  the bulk split ran (regions need 3+ candidates at once and 100+ basic blocks). `NAME#N` is the same in every
  mode of one text. `--prio REGEX`: each block's priority term with its source line (why a candidate ranks above
  another); `--split REGEX`: cuts, AF7 restrictions with their reason, the bulk split's calls and region per block
  with its predecessors in list order. REGEX takes `map`, `^_map$`, `t|npoints|map`, `map#2` (whole names first,
  then a search; no match: an error listing the names). `--seq`: the events in order; `--raw`: the proxy's lines.
  Line numbers are file lines of the traced text.
  `python tools/c2dag/c2dag.py SRC FUNC [--alloc]`: the scheduler's graph per block; the candidates with
  priority and final register. `python tools/c2dag/c2frame.py SRC FUNC [--refs]`: the frame packer's counts and
  slot table. `--names` names every temporary by the first source line that uses it (`tmp@8208`), lists each
  slot member's lifetime conflicts (X: never share a slot) and replays the packing (P: each local joins or opens
  a slot, with the conflict that made it pass each newer one): no hand simulation of the packer.
  Slot order wrong in a frame > 0x80: `c2frame --names --variants FILE --label L` gives the counts each local
  needs; references come from code VC6 later merges (a case body written as two case groups with the same body,
  each arm written out) without changing the meaning (ai_DecideUseMeanObject).
  `c2dag.py SRC FUNC --alloc --variants FILE --label L`: one variant's graph / priorities; diff two variants'
  `P blk` lines to see which block moved a priority.
- `python tools/c2dag/flip.py SRC FUNC --want REGEX [--try OUT.txt]`: mutates every statement (delete,
  duplicate, swap, change a constant) and reports the mutants whose listing has the wanted instruction, each by
  FILE:LINE and the changed source line. The
  mutants are not kept; they name the place the wanted code depends on. A hit on a DUPLICATED statement = that
  value has two uses in the original (a loop, the expression written again, a macro expanded twice); a hit on a
  changed constant at one site = that block is not shared with its twins in the original. REGEX is matched
  against VC6's /FA listing (tabs, upper-case `DWORD PTR`, frame symbols): `--want 'mov\s+edi, DWORD PTR _a\$'`
  asks "what puts parameter a in edi", the quickest way to a callee-saved register swap.
- `uv run --with pefile python tools/c2dag/c2pass.py ..`: compile with one back-end pass skipped (cross-jumping
  off: `--skip 0x1070a91d`) to see which pass makes a layout; `--list` names the known sites (block order,
  the three tail mergers, add esp merging).
- `python tools/c2dag/c2tail.py SRC FUNC [--tries] [--il merge] [--variants FILE --label L]`: which tails were
  merged into which copy (with source lines), what was compared, the IL with the block in front of the exit.
- `python tools/search/climb.py NAME [--rounds N --beam K --width W --ops a,b]`: a beam search over source
  mutations, each mutant measured with `dcp try`; round 1 is exhaustive (every single mutation), later rounds
  mutate the best texts again. It prints the best text's diff against the start (kept in build/climb/NAME.diff)
  and writes nothing into the sources. move and swap never cross an exit, a label, a statement sharing a written
  access path, a call against a call or a store through memory, nor land in a braceless `if` (`--unsafe` drops
  the check). Other operators can still change the meaning, and a pointer alias the check cannot see can too:
  check a lead against
  the original (`dcp disasm`) before applying it, and apply it as the plain code it suggests. A mutant whose
  frame size differs from the original's (`frame X vs Y (stack offsets aligned)`: its distance is not
  comparable) never wins: dropped when the start's frame size is right, ranked last otherwise.
  A short run of stores (a message or record fill, the inits at the top) left at a few instructions:
  `--ops move,swap --rounds 4` (the move-one-statement round, repeated); it matched several record fills that
  round 1 left at 2-8 instructions. `--base FILE:LABEL` starts from one of your `dcp try` variants.
  `python tools/search/sweep.py` runs it over every function below byte (results: `sweep.py --report`).
- Generators are script files (`python gen.py > v.txt`); `python -` with a PowerShell here-string hangs.
- Store / clear / init orders: `python tools/search/permute.py NAME A B` writes every order of file lines A..B
  (whole-function variants, up to 5040; `--sample S` beyond), then `dcp try NAME FILE`. It does not check
  dependences: read the run first. Up to ~7 statements a full round; more: climb `--ops move,swap` from the
  sibling's order.
  `dcp try` takes about 1 s per 17 variants (5040 variants: 5 minutes) and holds the daemon meanwhile: other
  dcp calls wait; split big rounds or narrow them first.
- `flip.py --want` with a regex spanning two calls (`call A[\s\S]*call B`) finds what decides the code between
  them when a single wanted instruction is already in the base. The scratch registers rotate through the whole
  function, so a change far up (a named pointer local, a duplicated statement as a probe) can decide which tails
  cross-jump at the end.
- An odd instruction form of the original (`[esp+esi+0x90]` with `add esi,4`): grep the whole original for it
  and read the byte functions that have it: `python tools/search/grep_orig.py 'add esi,4' --grade byte`
  (patterns compact: `[esp+esi*4+0x90]`; `--multi` for a regex over a whole function; the first run caches the
  image's disassembly, about 5 minutes). Their source names the construct.
- `asmdiff` says "identical instructions" while bytes differ: a jump is short in one and near in the other, i.e.
  it targets another cross-jump copy; compare the jump targets (`dcp disasm NAME` / `--ours NAME`).
- Sweeps over one function or the whole queue: `tools/sweep/blockscope.py` (function-scope records into their
  blocks), `tools/sweep/fillloop.py` (constant memsets as element loops, `--zero` for zero fills),
  `tools/sweep/twin.py --tagged` (every function with a `@deep` note that has a byte twin).
- `dcp note NAME "text"`: the function's `@deep` note, rewritten (the note of before this session stays as
  `prior:`). Write it after every round that taught something: what the original does at the hunks left, the
  mechanism, the round with its best row. A session can end at any moment; the note is what the next one has.
- A probe that is never kept answers "does this decide it": `extern T probe;` inside the function for "is this
  one object or three", a changed constant at one site for "is this block shared", `{ static int *pm; pm = &x; }`
  for "does x live in memory in the original".
- Older notes name tools that no longer exist (`deep-tools *.py`, `uv run spell / respell / vargen / c2prio /
  unitdiff`): take the technique, write the generator yourself.

## 6. Rounds worth generating (one `dcp try` each)
Write a small script that emits the variants file; do not hand-edit orders.
- All orders of a short run of independent statements (a record fill, the inits at the top, accumulator lines of
  a loop body); for a long run a move-one-statement round, then a hill climb.
- The product of exit forms: early test (nested / flat) x result init x failure form (`return 0` / `p = 0` /
  under `if (!ret)`) x where each arm returns (vfs_MemOpen: 60 variants, one round).
- The join position of a shared tail; a statement in both arms against once after the join.
- Each local: block scope against function scope; a named local against the expression at each use; one local
  against two (a variable reused for two roles, or one per branch).
- Each `memset` / `strcpy` / struct copy against its loop or field form and back.
- All subsets of a mechanism that can apply at several sites (2^n masks): a mechanism that is right at one site
  can be wrong at its neighbour (snd_UpdateRain needed all four sites, par_Draw exactly one of five).
Signals: identical code for a whole family = the mechanism does not reach the hunk, drop it; the difference
moved at a worse count = the right mechanism, refine it (positions, subsets).

## 7. When the ideas run out
There is no way to give an item up: it ends byte, or with a meaning bug fixed. When no round moves the code any
more, the function is not finished and neither are you. Go through this list; each point has matched functions
that had been called "allocation only, no source lever", which was never true.
1. twin.py run; the callees defined above are byte (if not, match them first: that is this item's work).
2. Every `goto`, `volatile`, hand pointer walk, hand copy loop and renaming local replaced by the plain form, or
   kept with the variant row that shows the plain form is worse AND a vdiff that says why.
3. Returns counted; `p = 0` + `if (p)` tried for each cleanup `return 0`; the join position swept.
4. Block scope tried for every record, array and buffer; one array for two.
5. Register hunks: c2trace read; you can name the candidate that must exist or must rank higher.
6. Slot hunks: c2frame counts read; you can name the local whose count or table position is off.
7. Order hunks: the fill permuted; assignment-in-expression tried; a move-one-statement round run.
8. The far variants with the right layout read with vdiff, not dropped by count.
9. `climb.py NAME --rounds 6` (and again with another `--seed`, or `--ops` narrowed to the operators that moved
   something): combinations of two or three ordinary changes.
10. The cause outside the function's text: the body of a macro it uses (its temps and their order count as
    references), the type of a struct member or global, a callee's prototype, an inline helper, the unit's
    place in a family of copies. Change it in a `dcp try` of the unit, not by guessing.
11. Read the original again from the top, block by block, against ours (`dcp asmdiff NAME --all --lines`) as if
    the source were unknown: which variables does the ORIGINAL have, where does each live, where is each set?
    The decided tests of section 3 hide statements; a difference in what exists is found by reading, not by
    variants.
12. Probes that change the meaning on purpose (flip.py, a constant changed at one site, a statement deleted):
    which part of the function does the hunk depend on? Then look for the ordinary form with that effect.
After each of these, `dcp note NAME ".."` with what it showed. Then start the list again with what you know now:
a lever found for one hunk changes what the others mean.