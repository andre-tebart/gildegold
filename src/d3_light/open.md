# d3_light.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_light:" allocation tags
- Boundaries: separated (plausible): "d3_light:" tags; end next to d3_vec.c (open)
- Range then: 0x451e80..0x4538a0
- Was: unk_451e80.c (round 12)

## Open work
- All 14 functions match (match 2026-10-22).
- d3_light.c:650 has two new C4133 warnings (same bytes): drop the cast and retype d3_Unk4535b0's D3MATRIX *m as
  D3ANIMFRAME *.
- see notes/units/d3_vec.md, Open work: "Names: 0x453e80 is d3_VecEqual (d3.h) and d3_PtEq (d3_io_t.h); 0x4538a0 is d3_LightCalc453 ..."
- Naming leads from the cleared @deep reasons (2026-10-24, user: these byte functions go back to match-base; the 'recorded decision' / 'deliberately not renamed' wording in them was an earlier escalation reason, not a decision, and binds nobody):
  - 0x4522b0 d3_LightApply: Q2: D3OBJECT.unk220 (small 1..5 value with no evidenced meaning: selects the per-vertex-normals directional path here; ch.c sets 2, d3_tools 5, d3_int_io reads it back - 'kind' would be a guess) and D3LOPS.unk6c (read only here as the mesh's Y offset for directional light, no other reader); D3MESH.unk958 named lightScale (all uses multiply the light power; ch.c 1.8)
  - 0x452ef0 d3_Unk452ef0: Q2: D3OBJECT.unk1cc is a project-wide field rename (~138 references in other units) that the recorded decision in src/game/d3_sm.c (0x45e180 note) and its escalation deliberately left open; its other identifiers are the d3_engine.c callees d3_Unk43a000 (picks the level-of-detail D3SURF) / d3_Unk43d7c0 (re-transforms the surface) - no meaning error, byte already
  - 0x452fa0 d3_Unk452fa0: Q2 D3OBJECT.unk211/unk212 + D3PTRI.unk26: flag bytes with evidenced individual bits but no evidenced single name (unk212 recorded deliberately not renamed, d3_quad.c 0x457db0 / d3_sm.c 0x45e210); Q1 no name evidence (no string/tag, Ghidra FUN_00452fa0); Q4 D3_BITS/D3_TEX6C need typed fields (D3TEXTURE 0x68 bits and 0x6c load flags, D3PTRI.unk26 bitfields). Byte (asmdiff identical); read all callers (d3_SetObjectAlpha 0x43d430, d3_Unk453280 0x453280), the d3_int_io object-flag reader/writer and d3_tools d3t_SetGrey/d3t_SetColor for the bit meanings
  - 0x453110 d3_Unk453110: Q2 D3OBJECT.unk211: bit 4 read here selects the d3_ZAxis direction for a type-7 light, but the byte has mixed evidenced bits and is deliberately not renamed (d3_quad.c 0x457db0 / d3_sm.c 0x45e210); the d3s.c 0x44.. / d3_fl.c 0x45e180 uses of bit 4 have the opposite polarity, so no safe bit name; Q1 no name evidence (Ghidra FUN_00453110; role from D3LCTX: collects the lights that reach ctx->obj). Byte, asmdiff identical; read body, callers 0x453280/0x4535b0, d3s.c and d3_fl.c bit-4 contexts
  - 0x453280 d3_Unk453280: Q2 unnamed identifiers with no evidenced names: callees d3_Unk43a000 / d3_Unk452ef0 / d3_Unk452fa0 / d3_Unk453110, ambient colour global d3_Unk5fcc78, D3OBJINFO pointer D3OBJECT.unk1cc (D3OBJX.info; recorded deliberately unnamed, d3_sm.c 0x45e210), D3OBJECT.unk212 (same record, d3_quad.c 0x457db0), D3MESH.unk95c (generation stamp, d3_vec.c note); Q4 1 x `*(int *)&` (line 555, fixable by a D3TEXFLAGS union view); Q1 no evidenced name. Byte, asmdiff identical; read full body and callers ch.c, d3_engine.c 0x43b.., d3_tools 0x4134a0/0x413550
  - 0x453580 d3_Unk453580: Q1 no evidenced name (2-line wrapper: if (obj) d3_ForEachObject2(obj->scene, obj, 0x240, the 0x453280 relight callback); Q2 callee d3_Unk453280 is an unnamed placeholder and Q1 of that function has no evidence either. Byte, asmdiff identical; read the body, d3_ForEachObject2's prototype and the callers (cm.c 0x40.. ai_Unk453580 alias, d3_light.h)
  - 0x4535b0 d3_Unk4535b0: Q2 unnamed identifiers with no evidenced names: callees d3_Unk43a000 / d3_Unk453110, ambient colour global d3_Unk5fcc78, D3OBJECT.unk1cc (D3OBJX.info, recorded deliberately unnamed d3_sm.c 0x45e210), D3OBJECT.unk212 (same record d3_quad.c 0x457db0), D3MESH.unk95c (generation stamp, d3_vec.c note); Q1 no evidenced name. Byte, asmdiff identical; read body (the matrix-placement variant, light group mask 0xe) and caller d3_anim.c:843
  - 0x4538a0 d3_LightCalc4538a0: Q2 unnamed globals with no evidenced names: d3_Unk633438 / d3_Unk633b14 (only tested == 0 / set to 1 as relight conditions, no meaning recorded) and d3_Unk633738 (the terrain object: D3TERRAIN* casts in d3s.c, d3fl_MakeLight here, but the global keeps its address name and d3_engine.h declares it char); callee d3_Unk453280 unnamed; Q1 the placeholder tail 4538a0 (role: the global relight entry, on 0/1 -> d3_LightRemoveRecords, on > 1 -> d3_Unk453280 over d3_DefaultScene). Byte, asmdiff identical; read the body and its callers d3_engine.c 0x43ee.., d3_interface.c 0x44a0..; the Q7 foreign declarations in d3_anim.c/d3_interface.c/f3init.c/he.c/sv.c/unk_58bbd0.c/cm.h are the unit header canonicalization, not this function

## Worker d3x (round 11): 0x452400..0x453800 [d3x]

### Report
- File: src/game/unk_452940.c, include/unk_452940.h (own header only; no shared header touched).
  TU: dynamic lighting of mesh objects (allocation tags "d3_light:cache", "d3_light:rlight_object"), original file
  name unknown. Extent 0x452940..0x4538a0 (0x4538a0 d3_LightCalc4538a0 is the next function, outside my scope).
- 0x452400..0x452940 is not a gap and not Ghidra misses: it is the tail of the big function 0x4522b0
  (mesh's scope, ends at 0x452938). I did not touch it.
- Ghidra missed a function: 0x452d90 (callback of d3_ForEachObject, mask 4, ctx = lit object; between 0x452940
  and 0x452e10).
- 0x452e40 d3_Unk452e40 frees the light records of an object.
- Nonmatching (4): 0x453110 d3_Unk453110 (7 asm lines), 0x4535b0 d3_Unk4535b0 (284/752 bytes), 0x453280
  d3_Unk453280 (487/768), 0x452940 d3_Unk452940 (874/1104, only a few tries). All four are written in full.
- Callees outside my scope declared locally: d3_Unk4522b0 (0x4522b0, mesh), d3_Unk451e80 / d3_Unk451f10 (callbacks in
  mesh's range), d3_Unk458950, d3_Unk459980, d3_Unk633b18 (float global). d3_Unk452940 takes (lit obj, D3LIGHTCTX2*),
  d3_Unk452d90 is called as (light, lit obj).
- Shared-header additions: none. d3.h declares d3_Unk453280 as void; it returns a char (renamed around the d3.h include,
  as d3_engine.c does).
- The scope end 0x453800 cuts through d3_Unk4535b0 (ends 0x453893); written in full since it starts inside the scope.

### VC6 lessons
- Repeated tests of a flag byte in a loop become hoisted boolean temps (`~al & 1`, `(~cl) >> 7`) stored in dead
  parameter slots, but only if they are initialized at the top as char locals: `char keep = !(fl & 1); char pos =
  !(obj->unk211 & 0x80);` with `unsigned char fl = obj->unk212` (d3_Unk452fa0). Using obj->unk212 inside the loop does not hoist.
- `x = min(255.0f, expr)` (macro, 255 as first operand) gives `fld 255; fcomp st(1)` with the value left on the FPU stack
  and stored once from either branch; `if (255.0f < x) x = 255.0f` stores x first.
- `(expr + b1) * 0.00390625f * b0` puts the constant last; `(float)((expr + b1) * 0.00390625f) * b0` keeps the constant first.
- `c.r = c.g = c.b = c.a = v` stores a, b, g, r (chain order = right to left).
- A two-way max of floats: `m = x; if (!(m > y)) m = y;` gives `fld x; fld x; fcomp y; and eax,0x4100; je`;
  `if (m <= y)` gives `test ah,0x41; jp`. The macro `max(x, y)` repeats loads per branch.
- `if (m > 255.0f)` -> `fcom DWORD; and eax,0x4100; jne`, with a double literal -> `fcom QWORD`.
- `D3TEXFLAGS f = {0}; f.b[0] = 0xff;` is a dword zero store plus a byte store to the same slot (as passed struct).
- `(unsigned)g` compared then clamped: `min((unsigned int)g, 0xff)` gives `cmp ecx,0xff; jb; mov ecx,0xff`.
- A loop over `keys[i].field` with the base pointer in a local (`k = info->keys` before each loop) gets the
  strength-reduced pointer biased at +0x34 like the original; indexing `info->keys[i]` directly reloads keys and copies
  floats with fld/fstp instead of integer moves.
- A bitfield copy between bytes `a.b2 = b.b4` is `(a ^ (b >> 2)) & 4 ^ a`: use a struct of eight 1-bit fields (D3BITS).

### Attempts (nonmatching functions)
- d3_Unk453110 (0x453110): remaining difference: `fld mem; fadd st,st(2)` (ours) vs `fxch st(1); fadd mem` (orig) for
  `sqrt(..) < r + other->r2`. Tried: r as local / CSE of obj->unk090.x, dx/dy/dz as locals or D3VECTOR4, float s with/without
  cast, operand swaps, double cast, `>`/`>=`/`<=` forms, inline helper. Best form: dx,dy,dz float locals before the
  `unk090.y != 0.0` test (matches the loads), `s = (float)sqrt(..)`, `s < r + mem`.
- d3_Unk453280 (0x453280, 752 bytes): orig keeps a zero in ebx, loads pointers differently; frame 0x30 vs our 0x2c. What
  matches: ambient fill loop, max() part, `D3TEXFLAGS f = {0}`, colour branch first, `min((unsigned)g, 0xff)`,
  `ctx.count > 0`. Not fixed by: z variable, `!= 0`, struct {int z; D3TEXFLAGS f}, k[i] vs pointer, separate scale variable.
- d3_Unk4535b0 (0x4535b0): same code shape as 453280 without the flags / 452fa0 part. Remaining: the final `c.z` multiply
  is `fmul [mem]` consuming the scale in the original but `fld; fmul st,st(1)` + a later pop in ours (not changed by a
  separate scale variable, operand order, repeated 255.0f / s); bias of the colour loop pointer (+0x46 ours, +0x34 orig);
  order of `info = obj->unk1cc` vs d3_Busy++.
- d3_Unk452940 (0x452940): written from the asm: branch order (flag first), shared locals; remaining: orig reuses the
  radius temp (`fld st(0); fmul st,st(1); fstp`) as the dx local, rr/z/k slots, `fmul [mem]` chain order of t.

## match 2026-10-22 (d3_LightAddObject)
- 0x452940 d3_Unk452940 -> d3_LightAddObject **matched** (117 -> 0), three separate causes:
  - Scale block (`fxch / fmul st,st(1) / fxch`, the scale kept on the FPU stack until the join): the normals loop
    uses `s` (the scale variable) for the squared distance instead of `dd`. Sharing one variable between two roles
    changed its liveness / weight; 115 of the 117 bytes. Spellings of the multiplies (511 variants): no effect.
  - List walk `[esi+8]`: `if (D3_L(light)->light->list) { rec = D3_L(light)->light->list; while (rec->next) ...`
    (the head read twice, CSE'd) instead of `p = list; if (p) { rec = p;` (copy-propagated, loads through eax).
  - `fld z; fmul s` after the d3_FtoI asm block: `t = z;` and then `1.0f / (t * s)`; parentheses, casts, temps for
    the product, declaration orders, dead stores: no effect.
- The author reuses float locals for unrelated roles: try `ren` over the locals of a block (uv run vargen) when a
  slot / FPU-liveness difference resists spelling changes.

## Also about this unit
- notes/units/d3_io_fl.md, section [mesh]
- notes/units/d3s.md, section [shfix]

## match-base 2026-10-23
- Reference updates only (a d3_vec.c Readability rename): D3_L(obj)->mesh->unk95c is stamped with
  d3_LightGeneration and d3_LightCalc4538a0 increments it (evidence in notes/units/d3_vec.md).

## match-base 2026-10-24 (match worker)

- d3_Unk451e80 -> d3_LightResetVertexColors, d3_Unk451f10 -> d3_LightFlicker, d3_Unk4522b0 -> d3_LightApply,
  d3_Unk452d90 -> d3_LightAttach, d3_Unk452e40 -> d3_LightRemoveRecords, d3_Unk452e10 -> d3_LightUpdate (all
  byte + readable except d3_LightApply, escalated). d3_Unk633b18 -> d3_LightRadiusMargin (matrix z offset
  added to the mesh radius; d3_LightAddObject byte + readable).
- Shared fields named (used by several units): D3MESH.unk96d -> flags (0x40 = no vertex animation, the same
  field as D3ANIMMATV.flags in d3_anim.c), D3MESH.unk958 -> lightScale (every use multiplies the light power;
  ch.c sets 1.8 for characters).
- d3_LightFlicker: D3_L.unk68 / unk58 -> flickerStart / flickerEnd (the lerp (1-f)*start + f*end); the
  `0xffffff` colour test became D3L_RGB_MASK (it is not an address; Q3 flags it anyway).
- Escalated: d3_LightApply (Q2: D3OBJECT.unk220 - a 1..5 value with no evidenced name, selects the
  per-vertex-normals directional path; D3LOPS.unk6c - read only here as the mesh Y offset) and d3_Unk452ef0
  (Q2: D3OBJECT.unk1cc, the project-wide field rename recorded open in d3_sm.c 0x45e180; callees
  d3_Unk43a000 = the LOD surface chooser, d3_Unk43d7c0 = re-transform, not named yet). both byte.
- Next in the unit: d3_Unk452fa0 then the 0x453xxx functions; d3_Unk452fa0 also needs the D3_BITS / D3_TEX6C
  macros (Q4) replaced by typed fields.

## match-base 2026-10-24 (match worker, 4th)

- The six remaining base functions are byte; all six escalated to match-deep (Q1/Q2/Q4, no meaning error):
  d3_Unk452fa0 (Q2 unk211/unk212 + D3PTRI.unk26, Q1, Q4 D3_BITS/D3_TEX6C -> typed fields: D3TEXTURE 0x68
  bits and 0x6c load flags, D3PTRI.unk26 bitfields), d3_Unk453110 (Q2 unk211 bit 4: d3s.c and d3_fl.c use it
  with the opposite polarity, no safe bit name), d3_Unk453280 (Q2 unnamed callees 0x43a000/452ef0/452fa0/
  453110, d3_Unk5fcc78, unk1cc/unk212/unk95c; Q4 `*(int *)&f = 0`), d3_Unk453580 (Q1, and Q2 only the
  0x453280 name), d3_Unk4535b0 (same Q2 as 453280), d3_LightCalc4538a0 (Q2 flag globals d3_Unk633438 /
  d3_Unk633b14, terrain d3_Unk633738, 0x453280; its foreign declarations in d3_anim.c / d3_interface.c /
  f3init.c / he.c / sv.c / unk_58bbd0.c / cm.h are the unit-header canonicalization).
- The unit's base queue is empty; next is the deep queue.

## match-base 2026-10-24 (match worker, 6th)

- Field names from the leads: D3OBJECT.unk220 -> `kind` (object kind from the file, 1 = per-vertex lit in
  d3_LightApply, 2 = character, 5 = hand object / sun rays; d3_ReloadMeshes selects one kind; also in the
  D3OBJX view and the rd_object local) and D3LOPS.unk6c -> `originY` (y of the mesh origin in its vertex
  data, subtracted before the directional-light normals; one reader).
- Callee names: d3_Unk452ef0 -> `d3_LightPrepare` (0x452ef0: camera-space position, squared radius and the
  current surface of obj, called first in the relight), d3_Unk43a000 -> `d3_SelectSurf` (the level-of-detail
  surface), d3_Unk43d7c0 -> `d3_TransformSurf` (switch to the LOD surface, transform it, mark dirty).
- Raised: d3_LightApply, d3_LightPrepare (byte + readable). Remaining below base: d3_Unk452fa0 (Q2 unk211 +
  D3PTRI.unk26, Q1, Q4 D3_BITS/D3_TEX6C), d3_Unk453110 (Q2 unk211 bit 4, opposite polarity in d3s.c /
  d3_fl.c), d3_Unk453280, d3_Unk453580, d3_Unk4535b0, d3_LightCalc4538a0 (the callee/global names listed in
  Open work; d3_Unk43a000/452ef0 are named now).

## match-base 2026-10-24 (match worker, 7th)

- All 14 functions are byte; the unit's base queue is empty.
- Raised (byte + readable): 0x452fa0 d3_Unk452fa0 -> `d3_SetVertexColors`, 0x453110 d3_Unk453110 ->
  `d3_LightCollectCB`, 0x453280 d3_Unk453280 -> `d3_RelightObject`, 0x453580 d3_Unk453580 ->
  `d3_RelightObjects`, 0x4535b0 d3_Unk4535b0 -> `d3_RelightObjectFrame`.
- Escalated: 0x4538a0 -> `d3_LightInvalidate` (`@deep` Q2 0x633438: the whole exe has four references and no
  writer besides `d3_Init`'s = 0; it gates the orthographic view cameras Oben/Vorne/Seite in 0x448910 and
  makes type-6 spawns type 5, so the condition behind it is unproven).
- Renames (project-wide, all references): D3OBJECT.unk211 -> `objectFlags` (bits listed in d3.h: 1 saved
  pos/angle, 2 saved with the scene, 4 shadow caster, 8 simple shadow, 0x10 fixed light direction,
  0x20 shadow fake, 0x40 mirrored, 0x80 alpha alias), D3PTRI.unk26 -> `drawFlags` (bit 2 = no culling,
  bit 3 = wrap), d3_Unk5fcc78 -> `d3_AmbientColor` (also declared in d3_fl.h), D3MESH.unk95c ->
  `lightGeneration`, d3_Unk633738 -> `d3_Terrain`, d3_Unk633b14 -> `d3_LightsChanged`.
- 0x4535b0's second parameter is the animation frame, not a matrix: D3MATRIX._43 and D3ANIMFRAME.radius are
  both at 0x38, so the reach margin is the frame's radius; `struct D3ANIMFRAME *frame` +
  `((D3LFRAME *)frame)->radius` (D3LFRAME view in d3_light.h) is byte-identical and drops the C4133 warnings.
- Type debt (Q4 in d3_SetVertexColors / d3_RelightObject): the typed-field forms - a union member in D3PTRI
  0x26, the D3TEXTURE 0x68 bitfield union and a D3TEXFLAGS field at 0x6c, and a `light` field at D3PVERT
  0x30 - each flip VC6's instruction order in d3_CloneObject (0x43d180) or d3_Unk4550e0 (0x4550e0), which are
  byte (see notes/vc6-matching.md). The functions read them through cast views of the unit header instead:
  D3BITS, D3TEXVIEW, D3TEXPACK (d3_light.h). Re-attack only with a header change that leaves both functions
  matching.
- Still foreign declarations of this unit's functions: d3_engine.h (d3_SetVertexColors, d3_LightUpdate),
  ecmd.h (d3_RelightObjects, d3_LightInvalidate), rd.h (d3_RelightObjects, char * view), d3.h/d3_engine.h
  (d3_RelightObject as void/int aliases). The unit stays "readability draft" on those, not on its functions.

## match-base 2026-10-24 (base-0 worker, shard 0/6)

- d3_light.h line 170: `void d3fl_MakeLight(void *map)` is now `struct D3FLMAP; void d3fl_MakeLight(struct
  D3FLMAP *map)` (the defining unit's typing, d3_fl.h). d3_light.c's call passes `d3_Terrain` (void *), which
  C converts implicitly; no byte changed. The d3fl_MakeLight (symbol, d3_fl.h / d3_light.h) lint pairs are
  dropped from config/symbols/lint_baseline.txt. d3_fl.c now includes this header (with the
  d3_RelightObject -> d3_RelightObject_void rename around d3.h) for the D3LOBJ light view.

## match-base 2026-10-26 (base-3 worker, shard 3/6, session 5)

Q7 canonicalisation: the unit's foreign declarations went 17 -> 2 (no byte changed, check 2603/4664 before and
after, unitdiff --against on every touched unit: no WORSE).
- include/d3_light.h was incomplete: it now also declares d3_LightUpdate (0x452e10), d3_SetVertexColors
  (0x452fa0, `struct D3SURF *` with a forward declaration so the TUs that only have d3.h can include it),
  d3_RelightObjects (0x453580) and d3_RelightObjectFrame (0x4535b0), in address order. The frame prototype
  needs `struct D3ANIMFRAME;` among the header's forward declarations: without it clang (-fms-compatibility)
  reports it as conflicting with the definition in d3_light.c, which does not include d3_anim.h.
- Removed from other units' headers: d3_engine.h's d3_LightRemoveRecords / d3_SetVertexColors / d3_LightUpdate,
  ecmd.h's d3_RelightObjects / d3_LightInvalidate, rd.h's d3_RelightObjects, cm.h's ai_Unk453580 /
  ai_Unk453280 (the call sites now call d3_RelightObjects / d3_RelightObject).
- Callers that got `#include "d3_light.h"` (and lost their local copies): src/game/d3_engine.c (its local
  `d3_LightInvalidate` declaration was the only thing in the way - with the include the TU's declaration table
  is unchanged and d3_CloneObject stays byte!), d3_interface.c, ecmd.c, rd.c (also needs d3.h + <d3d.h>: the
  header is not self-contained), cm.c, main.c (also d3.h + <d3d.h>), f3init.c, he.c, sv.c, unk_58bbd0.c (also
  <d3d.h>). The ai_Unk453580 / ai_Unk453280 / ai_Unk4538a0 aliases are gone from src/ (f3init.c, he.c, sv.c,
  unk_58bbd0.c, cm.c, main.c use the canonical names).
- The `#define d3_RelightObject d3_RelightObject_void` hack (it renamed d3.h's declaration) is gone from
  d3_light.c, d3_fl.c and unk_4591a0.c.
- Still open (2):
  1. include/d3.h's `char d3_RelightObject(...)` (0x453280): removing it flips d3_mir.c's byte function
     d3_MirDrawObject (0x4550e0) - the d3_mir.c TU's declaration table is the recorded D3PTRI/D3TEXTURE
     hazard (notes/units/d3_light.md, type debt). Needs that function re-matched first.
  2. src/game/d3_anim.c's local `d3_RelightObjectFrame` prototype (0x4535b0, the forward-declaration block at
     the top): adding d3_light.h there puts +2 instructions into d3anim_CreatePath and d3anim_CreateMorphAnim
     (unitdiff WORSE, bytes +4 each; tried after d3.h and after d3_anim.h, both worse, reverted). The
     d3_anim.c TU keeps the local prototype (the canonical one is in d3_light.h; the local copy is this last
     foreign declaration).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

- Re-confirmed both open items: adding `#include "d3_light.h"` to src/game/d3_anim.c puts +2 instructions into
  d3anim_CreatePath (20/464 -> 24/468) and d3anim_CreateMorphAnim (993/2832 -> 997/2836), so the local prototype
  at the top of d3_anim.c stays (its line now carries that reason).
- New datapoint for item 1 (include/d3.h's d3_RelightObject): appending `#include "d3_light.h"` at the *end* of
  d3.h (after the D3_ZERO macros, so the include is self-contained) flips d3_mir.c's d3_MirDrawObject exactly like
  deleting the line does (esi/edi load order in two places, check 2609, REGRESSED) - so the canonical-header fix
  cannot be routed through d3.h either. It needs d3_MirDrawObject re-matched first (match-deep).
