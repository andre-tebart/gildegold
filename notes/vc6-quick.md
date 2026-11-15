# VC6 SP5+PP /O2: the deep worker's guide

Read this file whole before the first item; it is the method. The cases behind it (about 1300 notes, each "what
the diff showed -> what the source was", with the function it was found on) are in `notes/vc6-matching.md`,
grouped by the symptom you see in the diff: go to the section of your hunk class and search it by instruction or
construct (`grep -n "fst \|rep stosd\|shrink" notes/vc6-matching.md`). Data, link and layout rules are in
`notes/vc6-data.md` (not needed for matching a function's code). A function named in a note is byte unless the
note says otherwise: `dcp fn NAME` shows the source that matched.

## 1. The premise: the original was ordinary code
Every function that was marked stuck and matched later had the same history: an earlier pass reached its best
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
3. Read the hunks: `dcp asmdiff NAME --changed --lines`, `dcp diffclass NAME`, `dcp frame NAME` when slots or
   the frame differ. Write down per hunk what the ORIGINAL does (which value sits where, loaded when), not what
   ours does wrong.
4. Meaning before allocation. A one-instruction slot hunk inside a record fill is a wrong field index until
   proven otherwise; a store into "another local's slot" is another variable; a redundant test, a reload, a
   second load of a parameter are statements of the original. Section "It was a meaning bug" in the reference.
5. `dcp headers sample NAME` when the tie-break is not measured. "Sensitive" on a function with dozens of
   differing instructions says nothing: the cause is in the source (main_RoundReport: 159 ins, "sensitive", six
   ordinary changes to byte). A function is truly tie-break bound only when it has a forced register split with
   two or more reloads at one block edge (an inline-asm block, the fourth callee-saved register taken); then the
   declaration set decides the reload order, and even then usually the source still differs.

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
  `python tools/c2dag/c2trace.py SRC FUNC` prints the real sequence (queue, priorities, splits, assignment).

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

## 4. Symptom -> what to try first
Section names are those of `notes/vc6-matching.md`.

| The diff shows | First levers | Section |
|---|---|---|
| pushes / pops of ebx, esi, edi on the wrong side of an early exit; an epilogue copy more or fewer; `xor eax,eax` placed elsewhere | count the returns; guards + one final return; `ret = x` per step under `if (!ret)`; `p = 0` + `if (p)` for each `return 0` after a cleanup; the code written out at each exit instead of `goto` | Returns |
| an arm out of line whatever the condition; a jump into another arm; a redundant re-test; `setne` against a branch | a flag set by separate ifs; the excluded case in the outer condition; if / else-if with the call in both arms; a ternary against if / else; swap the arms | Branch layout |
| the same instructions with callee-saved registers swapped, or every scratch register shifted from one point on | c2trace; named local against inline expression; an expression written out twice; statement written in both arms; the init order at the top; one early statement moved (n^2 round) | Register allocation |
| `[esp+N]` offsets differ, frame size equal or not | dcp frame; block-scope the block-used locals; one array for two; the callee's full record; D3VECTOR4; reference counts (c2frame) | Stack frame |
| stores / loads in another order inside a block | block-scope the filled record; permute the fill (720 / 5040 orders in one `dcp try`); assignment inside the expression; a read into a local before the other store | Order |
| a loop with another induction variable, a pointer walk against an index, `dec; jne` against a compare | the plain index loop over the real element type; the callee's real parameter types (a `short` parameter stops strength reduction); the bound in the `for` condition; `break` + one return | Loops |
| a handle or value loaded "too early", kept across the float code | no statement boundary there: `v = expr` inside the consuming expression or macro argument | Order, Floating point |
| `fld a; fmul b` against `fld b; fmul a`, `fst` against `fstp; fld` | init order of the two locals; a fully nested chain; assignment inside the product; an own local per branch; parenthesised member copy (`v.x = (p->x);`) | Floating point |
| arguments pushed before the test that picks one; a call "sunk" or doubled | the call written in both arms; the callee's prototype (char / short / unsigned char parameters); two calls assigned to named locals in call order | Calls |
| `movsx` / `and 0xff` / byte RMW differences, `lea` against `add; add` | the real type of the field, parameter or return; bitfields for byte RMW; `(c ? A : B) + x` against `(c ? d : 0) + A + x` | Types |
| `rep stosd` / inline copy shaped differently | the element loop against `memset`; `strcpy` against the hand loop; one array reused; struct assignment against field copies | Copies |
| nothing in the function's text moves it; `dcp headers sample` sensitive | callees above; a struct or extern in a shared header (put views in the unit); then section 2.5 | Declaration set |

## 5. Tools
- `dcp try NAME FILE`: one row per variant. Read the `ins` column yourself: `best:` says "none closer" when a
  variant has fewer instructions at a lower grade. `= control` means that source change is no lever for VC6.
  An empty replacement (`===` directly followed by `>>>`) swallows the next block: put a comment in.
  `--move-line N --to A..B` generates declaration moves.
- `uv run --with capstone python tools/c2dag/vdiff.py SRC FUNC VARIANTS LABEL`: the instruction diff of ONE
  variant against the original without touching the tree. Use it on every variant that changes the layout,
  whatever its count.
- `python tools/c2dag/c2trace.py SRC FUNC [--variants FILE --label L]`: the allocator's sequence.
  `python tools/c2dag/c2dag.py SRC FUNC [--alloc]`: the scheduler's graph per block; the candidates with
  priority and final register. `python tools/c2dag/c2frame.py SRC FUNC [--refs]`: the frame packer's counts and
  slot table.
- `python tools/c2dag/flip.py SRC FUNC --want REGEX [--try OUT.txt]`: mutates every statement (delete,
  duplicate, swap, change a constant) and reports the mutants whose listing has the wanted instruction. The
  mutants are not kept; they name the place the wanted code depends on. A hit on a DUPLICATED statement = that
  value has two uses in the original (a loop, the expression written again, a macro expanded twice); a hit on a
  changed constant at one site = that block is not shared with its twins in the original.
- `uv run --with pefile python tools/c2dag/c2pass.py ..`: compile with one back-end pass skipped (cross-jumping
  off: `--skip 0x1070a91d`) to see which pass makes a layout.
- Sweeps over one function or the whole queue: `tools/sweep/blockscope.py` (function-scope records into their
  blocks), `tools/sweep/fillloop.py` (constant memsets as element loops, `--zero` for zero fills),
  `tools/sweep/twin.py --stuck` (tagged functions with a byte twin).
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

## 7. Before `dcp stuck`
Stuck is for a function whose remaining hunk you can name mechanically (which candidate, which slot count, which
pass) and for which the rounds above are done. Check off, and say in the reason which were done:
1. twin.py run; callees above are byte.
2. Every `goto`, `volatile`, hand pointer walk, hand copy loop and renaming local of the function replaced by
   the plain form, or kept with the variant row that shows the plain form is worse AND why (vdiff).
3. Returns counted; `p = 0` + `if (p)` tried for each cleanup `return 0`; the join position swept.
4. Block scope tried for every record, array and buffer; one array for two.
5. For register hunks: c2trace read, the candidate that must exist or must rank higher named in the reason.
6. For slot hunks: c2frame counts read, the local whose count is off named.
7. For order hunks: the fill permuted; assignment-in-expression tried; a move-one-statement round run.
8. The far variants with the right layout read with vdiff, not dropped by count.
The reason then says: what the original does at the hunk, the mechanism that decides it, each round with its
best row, the exact lines left. "Allocation only, no source lever" is not a reason.
