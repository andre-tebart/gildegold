---
name: dcp-match-deep
description: Run one match-deep session of a decompanion project - take functions from the deep queue (dcp next --deep) one at a time and bring each to byte by trying source variants (dcp try), reading the frames (dcp frame) and measuring the declaration set (dcp headers sample), or mark it stuck with what was tried (dcp stuck). A session is 4 items, each within about 45-60 minutes. Use when the user says "match-deep", "/match-deep", "dcp-match-deep", "deep pass" or "work the deep queue".
argument-hint: "[items=4]"
---

# Match-deep session: 4 items, the main checkout

Goal: every item of `dcp next --deep` to **byte + readable**: our compiled function identical to the original's
bytes, from a source that still means what the original does and reads clean. Deep items are already shape or
close to it; what is left is mostly register allocation, stack slots, instruction order and the spelling VC6
needs to choose the original's code. A deep item that turns out to differ in **meaning** is a bug in our C:
fixing it matters more than the bytes (section 3).

Items: `$ARGUMENTS` (`items=N`; nothing given is 4).

Hard rules
- The hard rules of the dcp-match-base skill apply unchanged (tools, files, the sources as the state, no grade
  may fall, never an address of the original in code). Read its "Hard rules" once if you have not.
- Every variant is tried with `dcp try` (section 2.3), not by editing the tree and rebuilding: it compiles in
  scratch copies of the unit and writes nothing. The tree changes only for the variant you keep
  (`--apply LABEL`, or an editor), then `dcp build`.
- The source you keep must mean what the original does, and a reader must still follow it: no `volatile`, dead
  stores, `goto`s or register tricks without an `@original` comment saying which original quirk they reproduce.
  A spelling that only reproduces allocation (a declaration order, a loop form) needs no comment.

## 1. Start

1. `dcp session start --kind match-deep` (the gate, as for match-base).
2. `dcp next --deep`. The line says `why=@deep`, `why=@stuck` (the tag's reason: what earlier workers tried, and
   older reasons as `prior:`) or `why=untagged` (below byte in a unit whose base work is done: nobody has tried it,
   there are no notes). `risk=` orders the queue: `concrete` items first (a concrete difference is still named),
   then `draft`, `shape`, `proved`.
3. Read the project's matching guide whole if it has one (e.g. `notes/vc6-quick.md`): it is the method for this
   compiler. Where it says what to do before the first variant and before `dcp stuck`, that is binding here. Its
   reference (e.g. `notes/vc6-matching.md`, grouped by the symptom seen in the diff) is searched by the hunk class
   and the construct you face.

## 2. Work loop: one item at a time, always the first

### 2.1 Read where it stands (5 minutes)
- `dcp grade NAME`: the gates, `dist`, the `tiebreak:` line, `census:` / M8 / M10 lines.
- `dcp asmdiff NAME --changed` (`--regs auto` when only a register renaming is left), `dcp diffclass NAME` (the
  hunk classes and `proof:`).
- `dcp fn NAME` prints the function with its file lines (`at=src/<unit>.c:FIRST-LAST`).
- `dcp asmdiff NAME --changed --lines`: each of our lines with the source line VC6 compiled it from (`; unit.c:LINE`),
  so a hunk points at the statements a variant has to change. `dcp listing NAME`: VC6's own listing of the function
  (offsets, bytes, `; Line N`, its frame symbols `_x$ = N`).

### 2.2 The declaration set first
When `tiebreak:` says `not measured` or `under the four probes only`, run `dcp headers sample NAME` before anything
else (32 compiles, a few seconds):
- **sensitive, best byte**: a declaration set gives the original's bytes from your text: proven byte. Commit
  `ledger/tiebreak.tsv` (`<unit>: <name> proven byte (tiebreak sample)`); next item.
- **sensitive, best below byte**: the declaration set moves this function's code, and that alone says nothing
  about where the difference comes from. While more than a few instructions differ the cause is in the text:
  work it (2.3) like an insensitive one. It is tie-break bound only when all that is left is the order of reloads
  at one block edge behind a forced register split (an inline-asm block, the last callee-saved register taken)
  and no variant of the text moves it; only then `dcp stuck NAME "tie-break bound: ..."` with those hunks.
- **insensitive**: what is left is in the function's text. Work it (2.3).

### 2.3 Variants (`dcp try`)
Write the variants you want to compare into a scratch file (not in the tree), each starting with `### label`:

```text
### index-loop
<<<
    for (p = list; *p; p++)
===
    for (i = 0; list[i]; i++)
>>>

### whole-function
/* 0x4ac170, ... */
unsigned short f(unsigned short k)
{
    ...
}
```

A `<<< old === new >>>` block replaces text inside this function only (each old text must occur once in it: a
twin function's equal lines are never touched); a body without blocks is the function's whole new text. Then
`dcp try NAME FILE` prints one row per variant: `grade ins bytes code hunks`. `code` is a hash of the compiled
function: variants with the same code print `= label`, and a variant with the control's code means that source
lever does nothing for VC6 here (drop it). `BYTE` marks a byte match. Generators: `dcp try NAME --move-line N --to
A..B` moves file line N (a declaration) before each line of A..B. Keep a variant with `dcp try NAME FILE --apply
LABEL`, then `dcp build`, `dcp grade NAME`.

Work in rounds: one lever per round (declaration order, loop form, operand order, if/else order, a temporary
introduced or removed, a narrower or wider type), many variants per round, keep the best, repeat. Generate the
variants file with a small script when a round has more than a handful (all orders of a run of statements, every
position of one statement, the product of two or three choices).

Choose between variants by WHERE they differ from the original, not by the count alone: `best:` and the `ins`
column rank by distance, but a variant with the original's block layout at a larger count can be one step from
byte, and a small count behind an unusual construct (a `goto`, a `volatile`, a hand-advanced pointer) is usually
a local optimum. Read a variant's instruction diff before keeping or dropping it when it changes the layout (the
project's variant-diff tool if it has one; else apply it, build, `dcp asmdiff`, revert). The lever is often not in
the hunk's own statement: another `return`, a declaration's scope, a statement far above.

### 2.4 Stack slots (`dcp frame NAME`)
When `slots` / `frame` hunks are left, `dcp frame NAME` lists both frames: each of our slots as `[esp+X]` after the
prologue (`[ebp-X]` in an ebp frame) with the variable VC6 put there (from its listing; `buf+0x8` inside an array,
`a/b` for locals VC6 packed into one slot), its size and use (`array`, `rN wN`), the original's slots the alignment
pairs it with (`<- moved`, `<- several`), the original's slots no slot of ours pairs with, `shared in the
original:` (one slot of the original for two locals of ours, named: their lifetimes do not overlap there, so VC6
packed them), and the holes. The locals' sizes, how often each is referenced and their scopes decide the layout
(VC6 does not follow the declaration order): merge or split locals, change an array's size or element type,
declare a local that is only used inside a block in that block, add or remove a reference, until the frames agree.

### 2.5 Done or stuck
- **Byte + readable**: commit `<unit>: <name> byte (match-deep)`. The build clears the tag.
- **Not byte**: stuck is for a function whose remaining hunk you can name mechanically (which value must get
  which register, which local's slot, which block order, and what in the compiler decides it) AND for which the
  project guide's checklist before `dcp stuck` is done. The 45-60 minutes are the budget for variant rounds after
  that checklist, not a reason by themselves: an item whose checklist has open points is not stuck, do those
  points first. "Allocation only, no source lever" is not a reason; nearly every function recorded that way was
  matched later with an ordinary source change somewhere else in it.
  Then keep the best variant if it builds and nothing got worse, else revert. `dcp stuck NAME "why"`: what the
  original does at the hunk and the mechanism that decides it, which checklist points were done, each round with
  its result (`dcp try` rows: "index loop: shape, 9 ins; for-init: = control"), and the exact hunks left
  (`asmdiff` lines). The earlier reason stays as `prior:` by itself; do not repeat it. Build, commit `<unit>:
  <name> -> stuck (match-deep)`.

## 3. A meaning difference is a bug: fix it first
Deep items were escalated as allocation-only, but the reading may have been wrong. When `dcp grade NAME` or
`dcp diffclass NAME` names a concrete difference (`census:`, `witness:`, an M8 / M10 that is not `(crossed: ...)`,
`differs:`, a branch without a proof), or your reading finds a wrong index, field, constant, call or condition:
fix the meaning before any variant work. Commit it on its own (`<unit>: <name> fix: <what was wrong>`), with the
evidence in the message. It counts as the item.

## 4. VC6 lessons (add what you find to the project's notes, not here)
- VC6 packs locals whose lifetimes do not overlap into one stack slot, and splits local structs into fields (some
  in registers, some in memory). A frame 4 bytes short per vector usually means the original used a 4-float vector
  type.
- `x /= n` with a constant n compiles to a reciprocal multiply; `test ah,5 / jp` against `and eax,0x100 / je`
  follows how the condition is written (`>=` against a negated `<`).
- A literal used in two distant statements can get a callee-saved register; a `const` local is folded and never
  does.
- Two constants of one expression are folded into one literal (`0.1f * (1/60.f)`): when the original loads both,
  they sat in separate statements. An operand in `.rdata` only this unit uses is often a `static const`.
- Register allocation follows which values are allocation candidates (named locals, repeated constants, common
  subexpressions) and how often each is referenced; the declaration set only breaks ties. `dcp headers sample`
  saying sensitive does not make the text's levers pointless (2.2).

## 5. When to stop
A session is **4 items** (byte, fixed or stuck: one commit each), then the wrap-up. Never end the session early
because an item is hard; an item ends byte, fixed, or stuck under 2.5's conditions, then take the next.

## 6. Wrap up
1. `dcp build` exits 0; the tree holds only committed work.
2. `dcp session wrap` (it must say `gate: ok`), commit `session: match-deep wrap (<units>)`.
3. Final message: each item with its outcome (byte / fixed / stuck, and why), the levers that moved code and the
   ones that never did, tooling problems met (for the orchestrator's reports), the next `dcp next --deep` item.
   Stop.
