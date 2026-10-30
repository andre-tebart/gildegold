# unk_458810.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (-): object pose helpers; the rest of round 14's unk_4582d0.c
- Boundaries: separated (plausible): generic pose math (direction / point / matrix through the parent chain), called from all over the game, no .data; 0x458e70 moved out (layout 2026-10-22, blocked-TU pass); nothing contrary, the name is the only open question
- Range then: 0x458810..0x458da0
- Was: unk_4582d0.c (round 14)

## Open work
- see notes/units/d3s.md, Open work: "shfix leftovers after round 19 (notes/units/d3s.md [shfix], notes/units/unk_4591a0.md [shf ..."

## Worker q1: 0x4582d0..0x4591a0 (src/game/unk_4582d0.c, include/unk_4582d0.h) [q1]

### Report
Nonmatching: d3_Unk458e70 (25 of 816 bytes, see below).
No functions lie in gaps; the scope ends exactly at 0x4591a0 (458e70 is 0x330 bytes). No shared-header additions.

TU boundary: 0x4582d0..0x458810 is the quad tree of d3_quad.c (uses d3q_AddObject / d3q_Build as callbacks, the
same pools 0x633db0 / 0x633db4, node layout 0x34 minitems, 0x38 maxdepth, 0x44 object mask; d3_QBox = the 8
corner scratch buffer 0x633b30..0x633db0 sits right before the pools). 0x458810.. (object pose helpers: world
direction / point / matrix of an object through its parent chain, plus the env-map UVs of 458e70) is a
different topic; no evidence either way for a TU split at 0x458810 (no static boundary found). Orchestrator
decides the join; unk_4582d0.c uses its own XQNODE view of D3QNODE (include/unk_4582d0.h) so the files do not collide.

### Lessons
- A 16-byte local that only has x/y/z in use and one member spilled: `struct { float x, y; volatile float z; float w; } t`
  reproduced "x,y stay on the FPU, z goes through memory and is copied with an integer mov" (d3_Unk458810, d3_Unk458e70).
- Separate statements per operation (`t.x = t.x - a.x; ... t.x = t.x - b.x;`) give the original's store/reload
  pattern; one combined expression `(t.x - a.x) - b.x` does not (458b20, 458c10, 458da0).
- `!(flags & 0x80)` on an unsigned char gives `test; js`, the cast `(signed char) < 0` gives `jl`.
- A function returning in al that the caller passes on (`d3_Unk439650`) must be declared `unsigned int`, else
  `and eax,0xff` appears. Arguments pushed after a call (`push 0; push 0` after the inner call) need the inner
  call in its own statement.
- Search loop `for (it = n->first; it; prev = it, it = it->next) if (it->obj == obj) break; if (!it) return 1;`
  gave the unrotated cmp/je shape (d3q_Remove); the out-of-line block is the `else` (d3q_Mark).
- A vector pointed at by `v->src` read in an index loop: `buf[i * 3]` instead of a separate counter `k += 3` fixed
  all register assignments of d3_Unk458e70 (obj edi, part ebp).

### d3_Unk458e70 (nonmatching, 25 bytes)
Only two things differ: (1) in both loops `b.z = d * nz + v->z` loads nz first, the original loads d first
(`fld d; fmul nz`); (2) in the second (no anim data) loop the three products of each component are in the order
x, z, y in the original and z, x, y in ours, whatever the source order (all 6 term orders, both operand orders,
local copies of the source normal: no change; a local copy gives 228 bytes worse). Also the reload of the entry
pointer after the Normalize call sits before `add esp,4` in the original, after it in ours.
Tried: declaration orders (60 permutations of locals), d*nz / nz*d, `volatile` nz via struct member, inline
XENTRY expression instead of a local, int/char* entry, entry declared in an inner block, loop forms.


## match-base 2026-10-24 (match worker)
- Renamed the pose helpers from their roles (all byte + readable): d3_Unk458810 -> d3_DirToWorld,
  d3_Unk458910 -> d3_WorldAngles, d3_Unk458950 -> d3_DirToRef, d3_Unk458a20 -> d3_PointToWorld (the widely
  used one, ~40 references), d3_Unk458b20 -> d3_PointToRef, d3_Unk458c10 -> d3_LocalToWorld (a point local
  to the object, before its own matrix), d3_Unk458da0 -> d3_WorldMatrix. XOBJ.unk210 -> flags (bit 7 = the
  object is in world space).
- Left in the base queue: d3_WorldMatrix (Q2 on d3_Unk458ce0) and d3_Unk458ce0 itself - name the parent-chain
  matrix multiply next (the only remaining placeholder callee here).
- he.c (legacy encoding; not rewritten by the rename scripts) keeps d3_Unk458a20 in two COMMENTS; the build
  does not need them changed.
- (match-base 2026-10-24, second session) d3_Unk458ce0 -> d3_MatToWorld: "a matrix in the object's space in
  world space", the same XToWorld family as d3_PointToWorld / d3_DirToWorld / d3_LocalToWorld; the parameter
  `flag` is `orientOnly` (1 = the object's matrix keeps no translation, the callers with 1 want rotation only:
  d3_WorldAngles, d3_light, d3_interface's transpose path). d3_WorldMatrix got its Q6 text.
  The unit is now 8/8 byte + readable; the only open unit-level item is the unit name (@named_by -, no
  evidence found for the original file name).
