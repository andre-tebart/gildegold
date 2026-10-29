# mem.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Also about this unit
- notes/units/net.md, section [net]

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "m:" / "m_alloc_debug" allocation tags
- Boundaries: separated (plausible): "m:" tags; end next to mov.c (plausible)
- Range then: 0x4223c0..0x422c60
- Was: unk_4223c0.c (round 12)

## Open work
- 2026-10-25: all functions readable (six Q6 descriptions); the unit-level readability work left is the 54 foreign declarations (declaration -> include of mem.h in the callers). pool_Next is behavior + readable, 2/144 B (see below).
- match 2026-10-22 (2nd pass), no gain: pool_Next: `size` read through it->pool->size at each use (115 B worse), size loaded after `left`, `size * i`, `i = it->index` inside the do loop, `sizeof(MPOOL)` byte offset: the 2 B (imul operand order) stay.
- match 2026-10-22: pool_Next 68 -> 2/144 B: the empty-slot scan as `while (1) { if (*slot) break; i++; slot += size; }` (a plain
  `while (*slot == 0)` is rotated by VC6, the original is not) and the end-of-chain stores in the order pool, index, left. Left: the
  `slot = base + i * size` multiply is `mov esi,size; imul esi,i` (original: `mov esi,i; imul esi,size`); size*i, parentheses,
  casts, unsigned, an `off` temp, declaration orders, moving `i = it->index`: no change.

## match-base 2026-10-25 (base-3 worker, shard 3/6)

- Six Q6 descriptions added (mem_IsValid, mem_Init, mem_Dump, mem_Exit, pool_First, pool_Destroy); all 11
  functions of the unit are readable now, the bytes are unchanged (pool_Next stays at 2/144 B, regs only:
  the imul operand order, Open work above).
- Unit readability still open: the 54 foreign declarations (f3gm.c, he.c, ls.c, main.c, sv.c, unk_575860.c,
  unk_58bbd0.c, unk_5d33b0.c and the headers ahm.h / anm.h / ...; the old spellings ai_Unk4225e0
  (mem_IsValid), ai_Unk422620 (mem_Free), ai_Unk4226a0 (mem_Init), ai_Unk422910 (mem_Exit) are still in use
  there). Each fix is a declaration -> `#include "mem.h"` replacement in the caller, not a rename.

## match-base 2026-10-25 (base-3 worker, shard 3/6, session 4): the Q7 sweep is BLOCKED by d3_engine.c

mem.c's 54 foreign declarations are mostly mem_Alloc / mem_Free copies in ~15 headers (ahm.h, anm.h, cm.h,
d2.h, d3.h, d3_io_a.h, d3_io_b.h, evt.h, f3snd.h, gv.h, rt.h, sfx.h, tr.h, txt.h, vfs.h) plus mem_IsValid
(ecmd.h, chc.h) and pool_Alloc / pool_Free (d3_quad.h, vfs.h) - mem.h itself does not declare mem_Alloc /
mem_Free today, so the canonical header is incomplete first.
The sweep was prepared and reverted: mem.h gets the two prototypes, the headers lose their copies, the ~46
caller TUs include mem.h (vfs.h's duplicate MPOOLITER view and its mpool_Free / mpool_First / mpool_Next /
mpool_Clear spellings are the same functions as mem.h's pool_*; vfs.c's mpool_* -> pool_* rename is
byte-neutral). Blocker: adding `#include "mem.h"` to src/game/d3_engine.c flips the byte function
d3_CloneObject (0x43d180) to nonmatching - the two register loads of the D3E_COPY3 tail swap (mine
`mov esi,[ebp-4]; mov ebx,[ebp+8]`, the original the other way round), 2569 -> 2568. A mere line shift
(a comment line) in d3_engine.c does NOT flip it, and the include position (after <d3d.h> or after
d3_mir.h) does not matter: it is the header's own content (the MEM_ZERO / MEM_ZERO_VAR asm macros and the
MPOOL / MEMENTRY / MEMGROUP / MPOOLITER types) that moves VC6's /O2 register choice. Bisected only that
far - the macros cannot be removed even to test, because mem.c uses them.
So the mem_Alloc / mem_Free canonicalisation needs d3_engine.c's d3_CloneObject re-matched under the new
TUs content first (byte work, match-deep's toolbox), or a d3.h/d3_engine.h include solution that does not
change d3_engine.c's type table. Everything else of the sweep is mechanical.
The uncommitted sweep is saved in /tmp/session_wip.patch (this worktree) - it was reverted, check 2569/4664.

## match-base 2026-10-26 (base-3 worker, shard 3/10)

- mem.h gained the two missing canonical prototypes `void *mem_Alloc(int size, char *tag); /* 0x4223c0 */`
  and `void mem_Free(void *p); /* 0x422620 */` (address order, before mem_IsValid). Additive only: build
  check 2573/4664 unchanged, mem.c and d3_par.c (the two TUs that include mem.h) keep every byte, lint
  0 E / 0 X. The foreign copies in d3.h and the caller headers are still mem.c's readability debt - the
  sweep stays blocked by d3_engine.c's d3_CloneObject (shard 7/10), as recorded above.
- pool_Next escalated to match-deep with its reason (2 B / 4 asm lines, the imul operand order; `uv run spell`
  has no menu hunk for it, all source forms of the multiply tried - Open work above).
- 2026-10-26: the canonical-header condition is exactly include/<stem>.h (uv run declcheck
  foreign_declarations), so a prototypes-only sub-header (e.g. mem_alloc.h included by mem.h) can never clear
  the 54 foreign declarations - only removing the copies and having every caller include mem.h can, and that
  is where d3_engine.c's d3_CloneObject flips. The two prototypes are in mem.h now; the caller sweep needs
  d3_engine.c (shard 7/10) re-matched first.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

The sweep stays blocked as recorded: this session found the sibling case in this shard (d3_light.c's
include/d3.h declaration): removing a declaration from d3.h flips d3_mir.c's d3_MirDrawObject, and appending
`#include "d3_light.h"` at the end of d3.h flips it too (check 2609). The TU-content sensitivity of d3.h is the
same class as mem.h's here, so both canonicalisations need the affected function re-matched first (match-deep).
No change to mem.c or mem.h this session.
