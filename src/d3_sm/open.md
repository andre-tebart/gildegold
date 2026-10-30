# d3_sm.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_sm:" allocation tags, "d3sm_" debug output
- Boundaries: separated (proven): d3_sky.c / ts_texture.c leaked
- Range then: 0x45db40..0x45f2b0
- Was: unk_45db40.c (round 12)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_sm.h, data_views.h; no @original cast or view.
- see notes/units/d3_anim.md, Open work: "Type cleanup: D3MATERIAL+0xf4 is the D3ANIMCTL (slots at +0x110, so `tex[3]` at 0x178 is r ..."
- Naming leads from the cleared @deep reasons (2026-10-24, user: these byte functions go back to match-base; the 'recorded decision' / 'deliberately not renamed' wording in them was an earlier escalation reason, not a decision, and binds nobody):
  - 0x45e180 d3sm_BoundsCB: Q2 identifiers only (behaviour already byte, B1): D3OBJECT.unk1cc (a D3OBJINFO pointer; the D3OBJX view in d3_engine.h and d3_mir.h already call 0x1cc info, but renaming the shared field touches ~170 references in other units) and D3OBJECT.unk212 (object state bits; the recorded decision in d3_quad.c 0x457db0 says no single meaning, bits 2-3 and 4-6 saved by d3_interface, 7 = transform current, deliberately not renamed) - a local view would have to invent that name, so readable waits for the project-wide state-byte decision

## match-base 2026-10-24 (match worker)
- Raised: d3sm_Destroy (Q6 comment).
- Meaning fix (kept, escalated after it): d3sm_Pick's stepping loop. The original rounds both cell
  coordinates before the __ftol (`+ 0.5f`, the constant is 0x5eb7ac) and indexes
  `data[(int)(z+0.5)*size + (int)(x+0.5)]` (iz*size+ix, 45f545-45f56b) and returns the same rounded values
  as *px/*pz (45f5cc-45f607); our loop had (int)x without rounding and the transposed ix*size+iz. The
  no-stepping branch above is right as it was (size*iz + ix, no rounding).
- Escalated to match-deep, each with its @deep reason: d3sm_Sample (allocation/reordering only),
  d3sm_BoundsCB and d3sm_ShadowCB (Q2: D3OBJECT.unk1cc / unk212, the latter recorded as a state byte with no
  single meaning), d3sm_Replace (the parameter `to` is hoisted into esi one block early), d3sm_Build
  (frame/local layout + Q2 unnamed callees + Q4), d3_GetHeightAt (two scheduling differences), d3sm_Pick
  (the fixed meaning errors + slot/idiom codegen: frame 0x68 vs 0x80, bound checks as and/je vs test/jp,
  step*dx/dy/dz hoisted in the original).
- Remaining below base: the 6 escalated functions (deep).

## match-base 2026-10-24 (match worker, 6th)

- The Q2 lead of 0x45e180 is done project-wide: D3OBJECT.unk1cc -> `info` (D3OBJX already called
  0x1cc info; D3OBJINFO/D3SMMESH/D3ANIMCTL views) and D3OBJECT.unk212 -> `stateFlags` (a flag byte: bit 1
  = vertex colours set, bits 2-3 = shadow mask tested against D3SM.mask, bit 4 = cast shadows despite the
  mask, bit 7 = world matrix current; 170 references in 25 files, no grade fell, no WORSE). d3sm_BoundsCB is
  byte + readable.
- d3sm_ShadowCB is readable now (same renames); its @deep reason keeps only the behaviour part (193 code
  hunks). The unit's base queue is empty; the six deep functions are unchanged.

## match-base 2026-10-24 (base-0 worker)

- 0x45a2e0 was declared and called in this unit as d3_Unk45a2e0 but is defined in d3s.c (d3s_ prefix, a
  different symbol); the declaration and the call now use the unit's name d3s_ShadowTriangle (rename only, no
  byte change, 6/12 matched, 4031 differing bytes before and after). The local prototype stays - the
  canonical header (d3s.h) cannot be included here yet because of the known header conflicts.

## match-base 2026-10-24 (base-3 worker, shard 3/6)

- Reference update only: the local declaration and the call of d3_Unk43d840 are now d3_TransformMeshes (the
  d3_engine.c rename; the function transforms every type-4 mesh of the default scene). No byte change, 6/12
  matched, 4031 differing bytes before and after.
