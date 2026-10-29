# d3_par.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_par:" allocation tags; unk_455a90.c joined (round 13)
- Boundaries: separated (plausible): "d3_par:" tags; end next to d3_quad.c (plausible)
- Range then: 0x455630..0x457b30
- Was: unk_455630.c (round 13)

## Open work
- All 12 functions of d3_par.c match (match 2026-10-22). Leftovers of round 14 in other units: d3_Unk454890 (92/848),
  d3_Unk4550e0 (19/976, index form).
- see notes/units/d3_engine.md, Open work: "Header cleanup from round 8: d3_engine.h's d3_Unk4538a0(int) takes a char (ocmd's d3_Light ..."

## pfix (round 14): nonmatching functions of d3_par.c, d3_quad.c, d3_mir.c [pfix]

### Report
- Improved, still nonmatching (6):
  - par_EmitterCB0 0x456350: 20 -> 6/2064 (one store order: flags byte vs size fstp at the end of the spawn path)
  - par_EmitterCB1 0x456b60: 34 -> 7/2304 (spawned in the dead `time` slot, orig speed)
  - d3_Unk455a90 0x455a90: 26 -> 18/2240 (`test edx,edx` position)
  - d3q_Build 0x457f20: 306 -> 4/944 (child 2 hi.x / hi.z register pair)
  - d3_Unk454890 0x454890: 256 -> 92/848 (dedupe loop registers, mark search bound)
  - d3_Unk4550e0 0x4550e0: 19/976 unchanged (index form; the esi/edi reload issue is gone already)
- Source bug fixed: the emitter callbacks' final test compares ps->flags with `max` (the trigger count), not
  `frames`; that alone was the "frames / spawned slot swap".
- No shared-header changes.
- VC6 lessons:
  - `[base+index+disp]` register order: a strength-reduced `i * sizeof` IV is emitted as base, an explicit
    byte-offset variable (`(T *)((char *)arr + off)`, `off += sizeof`) as index. The zeroing of that offset after
    the loop guard needs the guarded do-while (`if (i < n) { off = 0; do {...; i++; off += k;} while (i < n); }`).
  - Two byte bitfields of t[0] and t[1] through the same base: VC6 knows they don't alias and hoists the second
    load above the first store; a copy-propagated `t1 = t + 1` pointer (no register, no code) keeps the order.
  - Dead-parameter-slot / frame-slot choice follows post-optimisation reference weights: an extra real use moves
    a variable into the slot, dead or folded uses (`if (0)`, `* 1.0f`, `s = speed`) do not. Locals for CSE'd
    loads (`p = res[j]; q = res[j + 1];`) change those weights and rotated three slots into place.
  - Four scalar floats instead of `float s[4]` (or a D3VECTOR4) moved a 16-byte block below the qword
    conversion temps (par_EmitterCB2).
  - `h = a - b; ...; h *= 0.5f;` (scale as its own statement) lets two such half extents be computed in
    parallel (fsub, fsub, fxch, fmul) before an unrelated store.
  - A call argument `depth + 1` from a volatile parameter reads the later argument first (right to left); a
    block-local `d = depth + 1` first restores the original load order.
  - `(float)rand() / RAND_MAX - 0.5f` vs `PAR_RND() - 0.5f` (cast quotient) moves an unrelated `lea` / fstp
    around a call; try it per component.

### par_Create 0x455830 (matched)
- The `[ecx+eax+disp]` order (particles base, offset index) needs an explicit byte-offset variable, not the
  strength-reduced `i * 0x54`: `PAR_AT(ps, j)->field` with `PAR_AT = (D3PARTICLE *)((char *)ps->particles + j)`.
  With `for (i = 0, j = 0; ...; i++, j += 0x54)` the body matches but `j = 0` is hoisted above the guard (count in
  ecx). The original's guard-then-zero comes from `if (i < ps->count) { j = 0; do { ...; i++; j += 0x54; }
  while (i < ps->count); }` (i = 0 earlier). With `ps->count > 0` or `++i <` the end-of-loop order is off.

### d3_Unk455a90 0x455a90 (nonmatching, 26 -> 18/2240)
- The t[1] flag store before the t[0] flag load: a separate pointer `t1 = t + 1;` (set right before the
  statement) in `PAR_TF(&t[0]).vis = PAR_TF(t1).vis = vis;`. VC6 copy-propagates t1 (no register), but no longer
  knows the two bytes don't alias. Separate statements, parentheses, volatile casts, byte forms: no effect.
- Left: `test edx,edx` (tex) is scheduled before the ten t[] stores in ours, after t[1].v[0] in the original.
  Not changed by: parentheses on any tex use, `t->tex` / `t[1].tex` / `t1->tex` in the test (forwarded), reading
  t[1]'s fields back from t[0], ternary / inverted / default-first tex selection, `t1->` for the t[1] stores
  (wrecks the frame).

### Emitter callbacks (par_EmitterCB0/1/2)
- The "frames / spawned slot swap" was a source bug: the final test compares ps->flags with `max`, not `frames`
  (orig reads the max slot there). `return !(e->triggeronce && (int)ps->flags >= max);` fixed the slots of all
  three callbacks at once.
- CB0/CB1 spawn direction: `PAR_RND() - 0.5f` (cast quotient) instead of `(float)rand() / RAND_MAX - 0.5f` moves
  the `lea edx,&r` / r.y fstp to the original place (all three in CB0; in CB1 only one of r.x / r.y may use it,
  either one gives the same code).
- CB2 (matched): the phase copies are four scalar floats `s0..s3`, not `float s[4]` / D3VECTOR4 (the array took
  the slot above r; scalars spill below the qword temps as in the original).

### d3q_Build 0x457f20 (nonmatching, 306 -> 4/944)
- Both half extents before the first call: `half.x = max.x - min.x; half.z = max.z - min.z; half.x *= 0.5f;
  lo.z = min.z; half.z *= 0.5f;` (the scale as its own statement gives the fsub, fsub, fxch, fmul shape).
- Recursive call: `{ unsigned int d = depth + 1; d3q_Build(child, minitems, d, maxdepth); }` loads depth before
  maxdepth as in the original (the plain argument reads the volatile maxdepth first). The `volatile` parameters
  are still needed (without them the frame and registers change completely).
- Left: child 2's hi.x / hi.z copies (orig: max.z loaded first into ecx, max.x into eax and stored first; ours the
  other way round). No effect: all 24 statement orders of child 1, 2 and 3, parentheses on the copies, float[4]
  arrays for lo/hi/half.

### d3_Unk454890 0x454890 (nonmatching, 256 -> 92/848)
- The function is 848 bytes (0x454890..0x454be0); the extra `cmp; jae` after the mark search came from
  `for (...) if (!mark[i]) break; if (i >= nedge) break;`. The original: `for (...) if (!mark[i]) goto found;
  break; found:`.
- Frame slots: the dedupe loop with `p = res[j]; q = res[j + 1];` locals moved nres / mark to their original
  slots (-8 / -0xc), and the restart's `a = b = i;` (instead of `b = a = i;`) swapped a / b into place
  (a shares the D3_ZERO size temp's slot as in the original).
- Left: the dedupe loop (orig keeps only res[j] in edx, compares res[j + 1] from memory twice, borrows edi from
  nres for `end` and reloads nres); with only `p = res[j]` the frame slots rotate again (231 bytes). The mark
  search keeps nedge in eax (ours compares with the stack slot), and two `[r1+r2]` load orders (mark[b],
  mark[i]). No effect: compare operand orders (16 each), `b[mark]` / pointer spellings, while/do loops.

### d3_Unk4550e0 0x4550e0 (nonmatching, 19/976)
- Left: the point-remap index (orig `lea ecx,[esi+eax]; mov ecx,[ecx+ebx]`, ours `add ecx,esi; mov ecx,[ecx+eax-4]`).
  No effect this round: `((int *)t->unk10)[k + 6]`, char-offset forms with every grouping, `(&idx[0])[k]`,
  `k[idx]`, a dest pointer `vv` (frame changes), an `n = idx[k]` temp, loop-header orders, dead `k`/`t` stores,
  index-form outer loops (`D3M_TRIS(..)[i]`, `t[i]`: different frame).

### Leftovers in the callbacks
- CB0: the original stores the size (fstp) before the flags byte at the end of the spawn path; CB1 (same source)
  stores the byte first in both. No effect: flags statement position, bitfield / cast / `= |` spellings,
  parentheses on the size sum, PAR_RND combinations for r.x/r.y/r.z.
- CB1: the original gives the dead `time` slot to speed (as CB0, where speed has two more references), ours to
  spawned. No effect: declaration order / init forms, `spawned` spellings, speed spellings (`speed = s = ...`,
  parens, `* 1.0f`, `if (0) speed = -speed`), s or f reused for other values, `speed * r.x`, f at function level.
  A real extra use of speed (diagnostic) does move it into the slot.

## Worker par (round 13): 0x455a90..0x457c90 [par]

### Report
- Scope 0x455a90..0x457c90 is six functions, not three: Ghidra's 8.4 KB "function" at 0x455a90 is the particle
  draw function (0x455a90..0x456350) followed by three emitter callbacks that are only reached through the
  pointers par_CreateEmitter passes to par_Create (0x456350, 0x456b60, 0x457460). No data or jump tables inside.
  All in src/game/unk_455a90.c + include/unk_455a90.h (same TU as unk_455630.c, the "d3_par:" code).
  - nonmatching (4), all with the full data flow and block layout:
    - 0x455a90 d3_Unk455a90 (per-frame draw: callback, one camera-facing quad per particle, sort-list entries),
      26/2240 bytes: two scheduler differences (t[1] flag store before the t[0] flag load; `test edx,edx`
      for the texture after the tri stores)
    - 0x456350 par_EmitterCB0 (type 0, sphere shell), 20/2064 bytes
    - 0x456b60 par_EmitterCB1 (type 1, cone between two discs), 34/2304 bytes
    - 0x457460 par_EmitterCB2 (type 2, box with sine drift), 34/1744 bytes
    The three callbacks share one problem: the stack slots of `frames` and `spawned` are swapped (orig spawned at
    [esp+0x10], frames at [esp+0x14]); CB1 also puts `speed` in a local slot instead of the dead `time` parameter
    slot, CB2 has s[4] and the D3VECTOR4 r in swapped slots. Not changed by: declaration order, initialized vs
    statement init (also at the very top), names, block scopes, `int spawned = 0` (that moves the init).
- TU boundary: 0x457c30 (par_FreeTree, frees a 4-child tree with pool_Free on the pools at 0x633db0/0x633db4)
  belongs to the next TU, 0x457c90.. (pool_Alloc on the same pools). The d3_par TU ends at 0x457c30.
- Callee names follow par2's unk_455630.h: par_SetDir (0x455a40), d3_ParUV0/1. 0x633db0 / 0x633db4 declared
  as d3_Unk633db0 / d3_Unk633db4 (MPOOL *) in include/unk_455a90.h; rename together with the 0x457c90 worker.
- No shared-header additions. par_CreateEmitter's real parameter order is (parent, pos, count, ...): ecmd.h
  calls the first one count and the third parent; ecmd.c passes the values in the right order, only the names
  are swapped (declaration left unchanged).

### Particle system layout (D3PARTSYS / D3EMITTER)
- ps+0x2c: copy of the 0xa4-byte D3EMITTERCFG (cfg.unk00 = emitter type 0/1/2). The callbacks also cache
  1/alpha[0] at +0x30, 1/(alpha[2]-alpha[1]) at +0x34 and per type +0x38/+0x3c (PAR_INVA0, PAR_INVA2, PAR_INVS,
  PAR_RATIO). ps->flags (+0x20) counts dead particles (trigger count), ps->unk24 = time of the last update.
- ps->unkdc: 4 D3PVERTs per particle, ps->unke0: 2 D3PTRIs per particle (v0 v1 v2 / v0 v3 v1, UVs d3_ParUV0/1).
- PARPART (include/unk_455a90.h) is the emitters' view of D3PARTICLE: vel.w = alpha wave phase, phase[4] at 0x10,
  base position at 0x20, birth time at 0x34, pos.w = alpha factor.

### What matched / helped
- par_CreateEmitter: `ps = 0` and no `else return 0;` for unknown types; VC6 threads that path straight to the
  return with eax = 0 (the `xor eax,eax` at entry).
- Draw: explicit vertex pointers `v1 = v + 1` etc. (else VC6 merges the flag-byte RMWs of the four vertices,
  knowing they don't alias); a D3VECTOR4 for the camera-space position (frame size); `(m._11 * p->pos.x)`
  parenthesized; `c.x * (d3_ScaleX * rz)`; rhw stored before z; a dead `rz = 0.0f;` at the top of the particle
  block to load d3_ScaleX before rz (rank tie-break, no code); a block-local `D3TEXTURE *tt = t->tex` in the sort
  loop; `v->flags & 0x80` (signed char test gives `jns`); `(d3_InvZFar * 16777216.0f) * z * 3.0`.
- Callbacks: the alpha clamp is `max(0.0f, min(255.0f, expr))` written inline (a float local spills it), so the
  first and third alpha branches tail-merge; `((alpha2 - age) * p->pos.w) * inv` parenthesized; the return value
  `!(e->triggeronce && ps->flags >= frames)` (the early `return 1` uses `mov al,1`, the final ones eax);
  rebirth-fill time with a block-local `float f = PAR_RND();` (kept on the FPU, popped after the ftol);
  CB0's radius: per-branch base statements with `(e->size[0]) * r.x` parenthesized, and
  `rad = min(...); rad += (max(...) - rad) * f;`.

## match 2026-10-22 (par_EmitterCB0)
- par_EmitterCB0 0x456350 **matched** (6 -> 0). The original hoists the flags load above the pos stores but keeps
  the byte store after the size fstp; `p->flags |= 1` in any position or spelling, bitfields, a flags local alone,
  renames, statement permutations, the tail duplicated into the initfill arms: all the same 6 B. What works: read
  the flags first (`fl = p->flags | 1;` anywhere after the sin calls, before the size statement), write size through
  a pointer (`sz = &p->size; *sz = ...;`, VC6 can no longer prove the byte store does not alias it), then
  `p->flags = fl;`. The pointer alone gives `or BYTE PTR [esi+0x51],1` after the fstp (no hoisted load).
  par_EmitterCB1 has the same tail but the original store order there is ours: leave it.

## match 2026-10-22 (par_EmitterCB1, par_Draw)
- par_EmitterCB1 0x456b60 **matched** (7 -> 0): `speed` belongs in the dead `time` parameter slot, `spawned` in the
  frame. The velocity lines `p->vel.x = r.x * speed; ...` written at the end of both initfill arms (VC6 tail-merges
  them, same code) give speed the reference weight for the slot. CB0 gets that weight from `speed = -speed`.
- par_Draw 0x455a90 (was d3_Unk455a90) **matched** (18 -> 0): `t1->v[0] = v;` (t1 = t + 1, already set for the
  vis flags) instead of `t[1].v[0] = v;` keeps the `test edx,edx` for the tex after that store. Through t1 for any
  other t[1] field it lands too late (8-14 B), through t1 from v[1] on it also changes registers further down.
  Store permutations (best 6 B), chained assignments (best 4 B), reading tex back from the tris, block splits:
  the test always went first.

## Also about this unit
- notes/units/d3_quad.md, section [par2]
- match 2026-10-22: d3_Unk455a90 reaches 6 B (4 asm lines, but 'worse' by instruction count) with the t[0] stores in the order v[1], v[2], unk10, tex, v[0]; the original has v[0] first and the test edx,edx after t[1].v[0]: the colour if is probably between t[1].v[0] and t[1].v[1] in a form not found yet.

## match-base 2026-10-24 (match worker)
- Raised: par_Init, par_DeleteAll (Q6), par_Unlink (Q4 + Q6), par_Delete (Q2 + Q4). All 12 functions stay
  byte; 7 are left below readable (par_Create, par_SetPos, par_Draw, par_EmitterCB0/1/2,
  par_CreateEmitter).
- Renames: d3_Unkfb98f4 -> d3_ParHead, d3_Unkfb9598 -> d3_ParTail (the default scene's particle list
  head/tail; 0xfb98f4 / 0xfb9598, also used by d3_engine.c and d3_interface.c); D3PARTSYS.unkdc ->
  points, unke0 -> polys ("d3_par:ParticlePoints" / "d3_par:ParticlePolys").
- D3PAR_SCENE_HEAD / TAIL (raw macros on D3SCENE +0xa4 / +0xa8) replaced by the typed D3SCENEPAR view
  (D3PAR_SCENE_PAR) in d3_par.h.
- Open work / next session: par_Create needs D3PARTSYS.unk24 and D3PARTICLE.unk50 gone. 0x24 has two roles
  (the callback time in the d3_par.c emitters, a per-call spawn cap in d3t_EmitterCB, color24 for
  Erdbrocken); 0x50 is the texture animation frame. Do NOT fix this by editing the D3SCENE / D3PARTSYS /
  D3PARTICLE bodies in d3.h: both the D3SCENE field add and the union-member add changed the codegen of
  d3_Unk4550e0 (d3_mir.c) and were reverted; a plain same-type member rename is the known-safe form
  (vc6-matching.md) - or move the field into a view struct in d3_par.h, which only d3_par.c includes.


## match-base 2026-10-24 (match worker, 2nd session)

- Unit now byte + readable (12/12). Raised to readable: par_Create (D3PARSTIME view for D3PARTSYS +0x24,
  D3PARTICLE.unk50 -> frame), par_SetPos, par_Draw, par_EmitterCB0/1/2 (D3PARINV view for the cached
  reciprocals at +0x30..0x3c, D3PAR_TIME for +0x24), par_CreateEmitter (D3EMITTERCFG.unk00 -> type,
  OCMDTEXFLAGS as a union of D3TEXFLAGS and its named bits).
- Renames: d3_Unk63343c -> d3_FrameCount (aliases already in d3s.c / ts_texture.c), d3_Unk633450 ->
  d3_Hardware (+ d3_Unk633450b -> d3_HardwareBits; ts_texture.c: "nonzero: Direct3D textures"),
  d3_Unkfc7b60 -> d3_TriCount, d3_Unkfc2dc4 -> d3_VertCount, d3_Unkfc5060 -> d3_ClipNone (a D3CLIPINFO
  with unk04 = 0, its planes are d3_Planes), D3PTRI.unk10 -> src (source polygon; the static UV table of a
  particle tri), D3OBJECT.unk210 -> state (bit 7 = no camera; bits 0/1/2 group/linked/dirty, 3 mirror,
  0x20 vertex normals).
- Views added: PARCOL (a colour set as a long), D3PARSTAMP (texture stamp at 0x54, as D3FLTEX in d3_fl.h),
  D3PARINV, D3PARSTIME; PAR_VF / PAR_TF now return typed pointers (`PAR_VF(v)->b4`) instead of holding the
  dereferenced cast in the macro body.
- Codegen traps found (both reverted, see notes/vc6-matching.md): a typed field in D3TEXTURE at 0x54 in
  place of padding changed d3_Unk4550e0 (8 B); a union of bitfields in D3PVERT / D3PTRI changed par_Draw
  (855 B), d3_CloneObject (4 B) and d3_Unk4550e0. The stamp and the flag bits are reached through the
  view structs, the bit access keeps the cast.
- The unk210 -> state rename made d3_TransformNormals and d3_FrameNormals (unk_4591a0.c) byte + readable;
  their stale @deep tags were cleared with `uv run ledger grade escalate --clear`.
