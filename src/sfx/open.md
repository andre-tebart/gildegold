# sfx.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): mem_Alloc tag "sfx_OpenDigOutput"
- Boundaries: separated (proven): evt.c / txt.c tags
- Range then: 0x42b720..0x42c380
- Was: unk_42b720.c (round 12)

## Open work
- match 2026-10-22, deep dive 5 (**matched**, 432/432 B; the unit is fully matched): sfx_OpenDigOutput. The form:
  the sfx_NumDrivers() result in a local `n` that is used a second time inside the if, and the AIL_get_preference
  result in a local passed to sfx_SetNumSamples: `n = sfx_NumDrivers(); if (n == 1 && !sfx_NumSamples) { pref =
  AIL_get_preference(1); sfx_SetNumSamples(pref / n); }`. C2 knows n == 1 on the taken path and folds the second
  use (`pref / n`, `pref * n`, `n = AIL_get_preference(n)`, `AIL_get_preference(n)` all give the same bytes; the
  original's spelling is unknown, the division reads as channels per driver), but n stays a candidate in the test
  block: P4 = 2, P6 = 3 with the pref local, prio(i) = 66 = prio(&Samples[i] web), the tie goes to the web (created
  later), i takes ebx. One of the two locals alone is one point short (67); a single-use local in an expression is
  substituted before the priorities. Found with the new `uv run c2prio --ir` (C2's instructions per block with
  their operand symbols): a call-argument local survives because the push keeps the symbol and the local gets
  the result register; `r = f(); if (r == 1)` and `p = mem_Alloc(); a[i] = p` are substituted; a constant local
  (`k = 1; f(k)`) folds; a second def of a single-use local does not keep it. Block 7 of the priorities is an edge
  block that only reloads the sfx_NumSamples web for the edge from the NumDrivers test to line 70 (no code).
  Also found: why the block-8 size local (deep dive 4) wrecked lines 72-76: C2 gives the scratch temps of a block
  eax, ecx, edx round-robin in IR order (busy registers skipped; a register-less web loaded at its use and a spill
  reload count), and a local that the global allocator put into eax does not advance the pointer, so every
  scratch choice after it rotates; a block-8 candidate that C2 folds (`max = sfx_NumSamples ? sfx_NumSamples :
  sfx_NumSamples`) flips the pair too but is a CSE temp in eax and leaves 7 B for the same reason. Ruled out with
  c2prio (P unchanged): switch / else spellings of lines 68-69, a local for the NumSamples test, locals for the
  mem_Free argument and the tag string, algebraic forms of line 74 (`& x`, `| x`, `* 1`, `- x`, address casts),
  store-forwarding forms of the return, `i > -1` / `0L` / `'\0'` / `!(i < 0)`.
- match 2026-10-22, deep dive 4 (not matched; what the original can and cannot have): sfx_OpenDigOutput 15 B.
  Everything checked with `uv run c2prio` first (now `--members`: which candidates make up each block's P).
  New facts: a call inside a global web's live range counts as a reference of that web, so blocks 6 / 7 (the two
  calls of line 69) each have the sfx_NumSamples web in P. C1 / C2 canonicalize casts, `*&x`, constant spellings,
  labels / gotos / empty blocks / nested tests, copies of single-use locals, struct / array / static / defined-here
  globals, callee prototypes (mem_Alloc unsigned / char *, no prototype, AIL long), K&R definition, params used as
  asm operands (still candidates): all leave every P unchanged. The only code-neutral P changer: **a named local
  whose only use is a call argument**. `{ int n = AIL_get_preference(1); sfx_SetNumSamples(n); }` gives P6=3 and
  i 67 vs 66: same 15 B, one point short. A size local for mem_Alloc in block 8 (`n = sfx_NumSamples *
  sizeof(HSTREAM); ... mem_Alloc(n, ..)`) flips the pair (fb 72 > i 72 tie) and fixes every ebx/edi hunk, but
  shifts the scratch registers / scheduling of lines 72-76 (128 B; 35 B with a `_zp`-first SFX_ZERO and tail
  order used / max / out, first diff then in line 73; the line-70 local changes `lea` to `shl`, 166 B). A sign-bit
  test (`!(i & 0x80000000)`, `!((unsigned)i >> 31)`) drops the 0-web from block 1 (i 64) and leaves 1 B: `js` for
  the original's `jl`, so the original really compares with 0 and block 1 keeps P=2. Struct-by-value params,
  volatile params: far worse. **So the original has one of:** an extra code-neutral candidate in blocks 4-7 on top
  of the line-69 local (no other form found there), or a block-8 candidate that does not disturb the scratch
  order of lines 72-76 (base order is right, so it is not a size local), or one candidate less in block 2 (none
  of bits / channels / rate / &Drivers[i] / alloc result / bits>>3 / reload / string / 1 / 0 can be dropped
  without code). Not done: validating the formula on a matched function (the priorities are read from C2, so only
  the per-block sums are model).
- match 2026-10-22, deep dive 3 (compiler cause found exactly, not matched): sfx_OpenDigOutput 15 B. Every
  differing hunk is one ebx<->edi swap: ours i -> edi and the `&sfx_Samples[i]` web (+ the nAvgBytesPerSec
  product) -> ebx; the original the other way round. **Cause, read out of C2.DLL with uv run c2prio:** the
  allocator (FUN_1073122d) gives callee-saved registers in priority order, first pick edi. Priorities
  (FUN_10707a0b) are summed per basic block: P(block) = distinct allocation candidates referenced in the block,
  +cost*P per reference (2 for variables / temps), -P per block a candidate lives through unreferenced.
  Candidates include constants used more than once (one web per value) and webs of `sfx_NumSamples` between
  kills. Here: block 1 (lines 55-56, i def + test) P=2, block 2 (57-64, Drivers alloc .. AIL test) P=11, the
  NumDrivers-if blocks P=1,2,2,1, block 8 (70-77, Samples alloc .. return 0) P=11, so
  prio(i) = 4*P1 + 2*P2 - 6 + 4*P8 = 68 and prio(&Samples[i]) = 6*P8 = 66: i first -> edi. On a tie the
  Samples web wins (tie key 0x6c > 8). **The original needs 2*P8 >= 4*P1 + 2*P2 - Ppen**, i.e. one more candidate
  in block 8, or one fewer in block 2, or the 0-constant not counted in block 1, or 2 more pressure in the
  if-blocks. Block 2 counts bits, channels, rate, i, &Drivers[i], the alloc result, the Drivers[i] reload for AIL,
  `bits >> 3`, and the constant webs 1 (tag), the tag string, 0 (AIL args). Block 8 counts out, i,
  &Drivers[i], &Samples[i], &Streams[i], 4 `sfx_NumSamples` webs (lines 70-73; the `max = sfx_NumSamples` load
  does not count), the string web and the 0 web (`used = 0`). Every earlier flip fits: `wFormatTag = 2` or a
  different first tag string drops a web from block 2 (P2=10); an extra `g = 1;` after AIL_get_preference adds
  the 1-web to block 8 (P8=12); deleting the wBitsPerSample store makes bits single-use, not a candidate (P2=10).
  So the original's source has one candidate more / less in those blocks with the same code; not found. Ruled
  out this pass (all 15 B, same assignment): SFX_ZERO asm spellings (96), asm operands in shared locals / reused
  params (28), dead / unreachable statements, typed / indirect / late-folded constants (120+), comparison and
  if-structure spellings incl. goto / do-while(0) / `&&` statements (300+; C2's IR at this point is
  canonical), `bits / 8`, `<< 2`, all 512 SFXDRIVER field-type combos, NULL spellings (71), explicit pointer
  locals for Samples / Streams (24), the five globals as members of one struct, volatile / register params,
  assignment-in-condition for i, tail-statement forms. Toolchain confirmed: vcpp5.exe ships only c2.dll (no
  C1), so SP5 C1 8867 + PP C2 9044 is right. Also: deleting one driver field store does not flip it. Next: a
  candidate change in block 8 / block 2 that leaves the code unchanged; check each idea with uv run c2prio
  (prints P per block and every priority) before compiling variant families.
- match 2026-10-22 (3rd pass, uv run vargen + uv run diffclass), no gain: diffclass: every hunk is the same
  ebx<->edi swap (i in ebx and the nAvgBytesPerSec product in edi in the original; the push order already
  matches). Tried as generated rounds: all 720 orders of the six driver field stores (current one best), 21 names
  for i and 5 for out, a local for `bits >> 3` / the block align (36), early-return / inverted AIL test shapes (15),
  SFX_ZERO temp declaration orders and an __inline function with the asm body (same 15 B), `register` / long /
  short / char i, an explicit `SFXDRIVER **d = &sfx_Drivers[i]` (far worse). Untried: reference-weight changes
  through statements duplicated into both arms (nothing obvious to duplicate), the SFXDRIVER field types.
- match 2026-10-22 (2nd pass), no gain: sfx_OpenDigOutput: `ret` variable with a single return (far worse), unsigned i, multiplication orders `channels * (bits >> 3)` / `(bits >> 3) * rate * channels`: no change; moving the nBlockAlign store before nAvgBytesPerSec is far worse (245 B).
- match 2026-10-22: sfx_OpenDigOutput 15/432 B: only i / temp in ebx / edi swapped (the SFX_ZERO asm uses both). No change from
  swapping the two SFX_ZERO calls, moving the max / used stores, multiplication operand orders, a blk temp (worse), volatile i (far worse).
- see notes/units/d3_anim.md, Open work: "Nonmatching functions (177 after round 17): `uv run ledger check --verbose | grep FAIL`; per-functi ..."
- see notes/units/dd_vesa.md, Open work: "Header cleanup: Dd_vesa.c globals/functions are declared both in d3_d3d.h (with D3DCARD) a ..."

## match-base 2026-10-24 (base-4 worker, shard 4/5)
- Unit byte + readable. All 43 functions got their Q6 description on the prototype in include/sfx.h (purpose, return,
  side effects); no code changed (43/43 matched before and after).
- main.c declared sfx_Shutdown / ReacquireAll / Reacquire / ReleaseAll as ai_Unk42b750 / 42bb00 / 42bab0 / 42ba70:
  main.c includes sfx.h and calls the sfx_ names (the same symbols, no code change), no foreign declaration is left.

## Also about this unit
- notes/units/evt.md, section [evt]
- notes/units/txt.md, section [sfx]
