# d3_quad.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): quad tree over scene objects (d3q_*, pools 0x633db0 / 0x633db4); the zone functions 0x4582d0..0x4587b0 of unk_4582d0.c joined (round 14: d3q_Build / d3q_AddObject callbacks, the pools, d3_QBox right before them)
- Boundaries: separated (plausible): coherent quad tree (d3q_*, pools 0x633db0 / 0x633db4), no strings
- Range then: 0x457c30..0x4587b0
- Was: unk_457c90.c (round 13)

## Open work
- see notes/units/d3_par.md, Open work: "pfix leftovers after round 14: par_EmitterCB0 0x456350 (6/2064, size / flags store order), ..."
- The 2026-10-24 naming leads are done (commit 51761989, a later match-base session): d3_Unk439870 is
  d3_UpdateWorldMatrix and d3_Unk439650 is d3_ClipBox (d3_engine.c, both byte), obj->unk212 is
  obj->stateFlags. d3q_GetBounds / d3q_Mark are byte + readable; the unit is 11/11 byte, readable.
  Kept as evidence for the rename: 0x439870 sets bit 0x80 of obj->stateFlags (the transform-current bit,
  2-3 and 4-6 saved by d3_interface); 0x439650 is the box test returning 0x40 inside / 0x3f outside.

## match 2026-10-22
- d3q_Build 0x457f20 matched (was 4/944): child 2's box statements in the order `hi.x = max.x; hi.z = max.z;
  lo.x = half.x + min.x; lo.z = half.z + min.z;` (12 of the 24 orders of the four stores match, found with a
  `uv run try` permutation run; the earlier "1259 orders" never included this block's permutations on their own).
  Unit fully matched.

## par2 (round 13): d3_par small functions 0x455630..0x455a90, 0x457c90..0x4582d0 [par2]

### Report
Nonmatching: par_Create 0x455830 (11/496), d3q_Build 0x457f20 (306/944).
No shared-header changes (own headers include/unk_455630.h, include/unk_457c90.h).
Suggested names: unk_455630.c = part of d3_par.c ("d3_par:" tags; joins par's file unk_455a90.c). unk_457c90.c: a quad
tree over object x/z bounds (pools 0x633db0 / 0x633db4); 0x457c30 (d3q_FreeNode, in par's range) belongs to it, so
the d3_par TU ends at 0x457b30 (par_CreateEmitter) or 0x457c30; the quad tree is probably a separate TU 0x457c30..
(unknown end, 0x4582d0 not examined).

### Notes
- List: fake nodes. d3_ParEnd (0xfb9280) prev field = tail pointer 0xfb9598; head node 0xfb95e0 (d3if_ParList) next field =
  head pointer 0xfb98f4. Scene +0xa4/+0xa8 hold saved head/tail. Two UV triangles initialised at 0xfb95a0 / 0xfb95c0.
- par_Create: `size > 0.0` (double), `(unsigned)count > 0`, store order tex, texindex, count, size, callback (all 120
  permutations). Left: loop address operand order `[ecx+eax+disp]` vs ours `[eax+ecx+disp]`; not changed by declaration
  order, ptr arithmetic / char-offset / explicit offset variable spellings, i[ps->particles], unsigned counter.
- d3q_AddObject: early `return 1` guard + one final return (shrink-wraps the esi push); matched.
- d3q_GetBounds matched only with windows.h min()/max() macros (`min(mn[i], v->i)`); hand-written ternaries in any
  of 16 spellings gave other fcomp/fcompp forms. Init stores must be separate statements (chain stores right to left).
- d3q_Build: `depth <= 1` (shares the constant 1 register), `(unsigned)n->count > minitems`; second bounds loop is
  `min(n->min, mat->min)` with the node first. Making depth and maxdepth `volatile` parameters moved the stack frame
  and register use (878 -> 306 bytes differ; `same` still not in ebx the way the original has it without volatile?
  the original reloads depth/maxdepth from the stack). Original bug kept: child 3's lo.z uses half.x. Left: the original
  computes both half-extents (two fsub, fxch, fmul) before the first call; 1259 statement orders tried, and child
  2 / 3 statement orders (72), no gain.

## Also about this unit
- notes/units/d3_par.md, section [pfix]
- notes/units/unk_458810.md, section [q1]

## match-base 2026-10-23 (match worker)

- Field renames (evidence in the callers, all uses updated): D3OBJECT.unk04c -> `pos` (the object's world
  position: ch.c sets pos.x = obj->pos.x + ch->offset.x, the camera's pos is read the same way; 229 uses),
  D3OBJECT.unk213 -> `flags` (object flag byte: bit 2 = in the quadtree, set/cleared by d3q_AddObject /
  d3q_Remove and ch.c, bit 0 = transform current, bits 4-5 = LOD stage).
- Readable now: d3q_AddObject, d3q_Remove (the flags bit), d3q_RemoveObject (description).
- Escalated for Q2 only (behaviour is byte): d3q_GetBounds (callee d3_Unk439870 = the transform update in
  d3_engine.c, field unk212 still unnamed - a state-flag byte with bit 7 = transform current), d3q_Mark
  (callee d3_Unk439650 = the box test in d3_engine.c).
- The unit is behaviour byte; 9 of 11 functions readable, the two Q2 escalations left.

## match-base 2026-10-24 (match worker, 6th)

- The two Q2 leads are done: d3_Unk439870 -> `d3_UpdateWorldMatrix` (0x439870: world matrix of the object and
  the camera-relative matrix unless current, bit 7 of D3OBJECT.stateFlags) and d3_Unk439650 -> `d3_ClipBox`
  (0x439650: clips the 8 box corners against the view frustum, returns the clip code and the z range; the
  d3_fl.h / d3_quad.h / d3s.c declarations followed the rename and the lint_baseline lines were renamed).
  d3q_GetBounds and d3q_Mark are byte + readable; the unit is 11/11 byte and its base queue is empty.

## match-base 2026-10-25 (base-3 worker, shard 3/6, session 4)

Q7 canonicalisation (the unit's last readability debt, 6 foreign declarations -> 0): d3_quad.h stays the
only place that declares the d3q_* functions; the callers include it.
- include/cm.h: the unused `d3q_RemoveObject` line dropped; cm.c includes d3.h + d3_quad.h (it already had
  <windows.h>, <d3d.h> added) and casts its three call arguments.
- src/game/he.c, src/game/sv.c: the local `d3q_RemoveObject` prototypes dropped, both include d3.h +
  d3_quad.h, the call arguments cast to D3OBJECT * / D3QNODE *.
- src/game/f3init.c: d3.h is includable there after all (the old "gv.h collides" note is stale: no name in
  d3.h clashes with gv.h's K&R lines; d3.h only needs <d3d.h> for DDPIXELFORMAT). The unit's own views went
  away with it: the local D3VECTOR typedef and the INIT_D3SCENE view are d3.h's D3VECTOR / D3SCENE now
  (same offsets, 0x3e0-byte scene), `((INIT_D3SCENE *)d3_DefaultScene)->hmap` is `d3_DefaultScene->hmap`
  (Q4 gone), the local d3_Scenes / d3_CurrentScene / d3_Terrain / d3_Origin / d3_FindObject / d3_VecEqual /
  d3_PointToWorld declarations are d3.h's, and the K&R `int ai_Unk4582d0()` became d3q_Create from
  d3_quad.h (cast to int for the int d3_QuadTree global).
No byte changed anywhere (unitdiff --against on d3_quad.c, cm.c, he.c, sv.c, f3init.c; check 2569/4664).
The unit is 11/11 byte and readability readable (the first readable unit of shard 3/6).

## match-base 2026-10-26 (base-3 worker, shard 3/10)

No function work: the unit is 11/11 byte + readable. Only the Open work's two 2026-10-24 naming leads were
stale (resolved by 51761989: d3_UpdateWorldMatrix / d3_ClipBox / stateFlags); their evidence is kept above.
