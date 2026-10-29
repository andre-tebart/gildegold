# path.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Also about this unit
- notes/units/net.md, section [net]

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): A* grid path search (path_*)
- Boundaries: separated (plausible): coherent A* grid search (path_*), own string 0x5f7f80
- Range then: 0x424f30..0x425af0
- Was: unk_424f30.c (round 12)


## match-base 2026-10-23
- path_Search2 (still draft): the terrain-cost test compares against a DOUBLE 0.0 in the original (`fcomp
  QWORD PTR ds:0x5eb830; fnstsw ax; test ah,0x5; jnp`, 0x42526f / 0x425286), so the source needs `>= 0.0`,
  not 0.0f; fixed, which matched those two comparisons (210 / 219 differing asm lines are left, register
  choice and interleaving).
- The real remaining meaning gap: the C has an EMPTY `for (k = 1; k < 15; k++) ;` at 0x425297-0x4252cb. The
  original runs `path_Weight = 0.0f; for (k = 1; k < 15; k++) if (path_Costs[k] >= 0.0) path_Weight +=
  path_Costs[k];` (fld [edi+ecx*4]; fcomp QWORD 0.0; and eax,0x4100; jne skip; else fadd into 0x1460ac0) and
  then overwrites path_Weight with 7.0f (0x4252da) - the sum is dead but has to be emitted.
  path_Search1 (0x425470) is the same body and still has 0.0f; it will need the same two fixes.

## match-base 2026-10-23 (match worker)

- Unit is now behaviour draft / readability readable: path_Search2, path_Search1, path_Expand, path_Reverse
  are readable and escalated to match-deep for behaviour; path_Find stays byte + readable.
- The same body fixes in both search functions (they are the same code, Search1 one-directional):
  - the terrain-cost test compares against a DOUBLE 0.0 (`fcomp QWORD`), so the source is `>= 0.0`;
  - the dead weight sum is present: `path_Weight = 0.0f; for (k = 1; k < 15; k++) if (path_Costs[k] >= 0.0)
    path_Weight += path_Costs[k];` and is overwritten with 7.0f after it (original: fld/fcomp/and 0x4100;
    fadd; then `mov DWORD [path_Weight],0x40e00000`);
  - node indices are the 32-bit local `a` with `(unsigned short)` casts at the array subscripts
    (`PATH_INITNODE(&map->nodes[(unsigned short)a], 5)`), and `b` (unsigned short) for the goal; the
    original stores the goal index with `mov WORD [esp+0x1c],cx` inside the `if`, so `b = path_Goal;` sits
    there;
  - return type is 16-bit (`unsigned short`): the original returns `mov ax,ds:path_Start` without a
    zero-extend;
  - `path_Count` is `unsigned short` (the original does `inc WORD PTR [path_Count]`).
- Header types fixed (all evidenced by the code): `float path_Tab1[6]`, `float path_Tab2[6]` (fld/fstp,
  6 entries each: 0.25 five times and 0.0), `path_Tab1Sel`/`path_Tab2Sel` (was path_Unk1460aac/ab8) are
  floats written once per search and never read anywhere in the exe, `path_Count` unsigned short,
  `path_Expand`/`path_Reverse` take `unsigned short n` (the callee masks with 0xffff, the callers push
  without a zero-extend; making path_Reverse's parameter a short alone cost it 21 asm lines - its internal
  `last = p` then truncates - so only path_Expand's parameter is short).
- path_Expand: the cost test is against double 0.0 too, and the direction loop is a counting
  `for (dir = 0; dir < 8; dir++)` with a signed int dir (`inc eax; cmp eax,8; jl`; the `++dir > 7` form gave
  `cmp 7; ja`) and `continue` where the draft had `goto next` (266 -> 234 differing asm lines).
- Remaining (escalated): register allocation throughout (the original keeps 0 in edi, ours in ebx, and
  picks ebp/edx/ecx differently) and the FP compare idiom (`test ah,0x5; jnp` for the search conditions,
  ours `and eax,0x100`). Sizes already equal: path_Search2 976 = original, path_Search1 784 = original.
