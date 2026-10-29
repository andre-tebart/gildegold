# d3_mir.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_mir:" allocation tags
- Boundaries: separated (plausible): start: d3_Unk454780 moved here from d3_vec.c (layout 2026-10-22, codegen: 234 -> 5 bytes, it must not see d3_Unk454700); but d3_Unk454890 goes 92 -> 235 bytes with 454780 above it (register shuffle of a far draft; recheck once 454780 matches)
- Range then: 0x454780..0x4554b0
- Was: unk_454890.c (round 12)

## Open work
- d3_MirCreateOutline 0x454890 (match-tu 2026-10-23): 2 B / 4 lines -> unchanged, stuck; dominant regs (operand order of [edi+ebx] / [eax+edx]), idiom hits none (the zero_* hits were context noise), siblings none; tried: respell (77 variants, no gain), D3_ZERO family (D3_ZERO pointer form 10 lines, nedge / << 1 size spellings and `i + used` identical, D3_ZERO_AT before `mark = used + i` 97 lines); c2prio: used prio 6 and no register (ebx is a temp copy because the D3_ZERO_AT asm references `used`, so every later use reloads a temp), the original's used is a register variable with the n slot as home; lead: give `used` a non-asm clear so that it stays a variable (a memset-like helper the unit may own, or the asm fed from a copy `u = used`), then recheck the operand ranks.
- match 2026-10-23 (5th): d3_MirCreateOutline (was d3_Unk454890) 4 B / 8 lines -> 2 B / 4 lines: two dead stores at the top (`mark = 0; a = 0;`, now `tail`) fix 2 of the 4 operand orders of the mark addresses (rank of the variables' first occurrence, see vc6-matching.md). Left: `lea edx,[ebx+edi]` of `mark = used + i` (original [edi+ebx]) and the first `mark[tail] = 1` ([eax+edx] vs ours [edx+eax]). Tried without gain (about 12000 variants): dead stores of i / used / mark / a / b / j at all positions of the prologue and of the block up to `start = edges[0]`, perms of the three init lines, spellings of the four mark sites (`used[nedge + x]`, `*(used + nedge + x)`, `(used + nedge)[x]`, `mark = &used[i]`, `i + used`).
- match 2026-10-23 (4th): d3_Unk454890 6 B / 12 lines -> 4 B / 8 lines: `mark[b] = 1;` written as `used[nedge + b] = 1;` (the other three mark sites and `mark = used + i` unchanged: 695 sampled spellings of all sites, no further gain). Also tried before that (identical bytes or build failures): Also tried (all identical bytes or build failures): `mark` as unsigned int / void * / char * / unsigned int with `(unsigned char *)` casts at every use and in `mark = ...` (600 sampled of 20480), `register` on used / mark / i / a,b / nres.
- match 2026-10-23 (3rd): d3_Unk454890 168 B / 26 lines -> 6 B / 12 lines. Root cause of the found-scan shape: the original has no test after the scan at all; the scan is `start = NULL; for (i = 0; i < nedge; i += 2) { if (!mark[i]) { ...found code (zero fill, a = b = i, marks, end / start, dir, d3_Normalize)...; break; } }` at the end of the do-body, so the found path falls into `while (start)` and on the normal exit VC6 threads the known `start == 0` straight to the exit (the `jmp done`); the dead `start = NULL` store vanishes. A test after the loop (`if (i >= nedge) break / goto done`) leaves a recheck on the break path, a `goto` out of the for keeps it unrotated. Left (6 B): base/index operand order of the four mark addresses (`mark = used + i` lea [edi+ebx] vs ours [ebx+edi], `mark[a]` / `mark[i]` [index+mark] vs ours [mark+index]); tried `i[mark]`, `*(i + mark)`, `&i[used]`, `i + used`, casts through int / char *, mark's type (char / signed char), declaration moves of used / mark: all canonicalised (same 6 B). Lead: the registers are the same (ebx = used, edi = i at 0x45490a) and only the operand order differs, and c2prio gives `used` no register (prio 6, `-`; ebx holds a temp copy) while the original's code treats used as a register variable throughout; the order of commutative operands may follow that (variable vs temp). Next: raise used's priority (more references in high-P blocks) and read the result with uv run c2prio.
- match 2026-10-23 (2nd): d3_Unk454890 62 -> 26 asm lines (235 -> 168 B): the res dedup loop reads `res[j]` / `res[j + 1]` straight from memory (`(res[j] == start && q == end) || (q == start && res[j] == end)` with only `q = res[j + 1]` as a local, no `p`), and the `found` scan is `i = 0; if (!nedge) break; while (mark[i]) { i += 2; if (i >= nedge) goto done; }` with `done:` before the mem_Free calls (the original's loop test uses a register copy of nedge). Left: the n / used pair: the original keeps `used` in ebx and the n/i web in edi (ours swapped), `mark[a] = 1` loads mark before a (`[eax+edx]` vs `[edx+eax]`), 1 B of padding. The original stores `used` into the parameter slot [ebp+0xc] (it overwrites n: `n = (unsigned)mem_Alloc(...)` reproduces that, but is worse overall: 44 lines); weights of used vs i tried by `mark = i + used` / `&used[i]` / `&i[used]` (canonicalised), perms of the init block and of the three statements before it (no change).
- match 2026-10-23: d3_Unk4550e0 MATCHED: the `t->v[k] = ...idx[k]` inner loop as `k = 0; do { ... } while (++k < 3);` (for / while forms and every spelling of the idx address are canonicalised to the same wrong IV order; only the bottom-tested loop gave `dec edi / jne` with the merged IVs).
- match 2026-10-23: d3_Unk454890 235 B / 62 lines (454780 above it): with ebx<->edi swapped the first 150 instructions are equal, so the first difference is the register of `n` (original edi for the whole prologue, ours ebx); c2prio: i 420 edi, i(2nd web) 280 edx, start 200 ebx, n 66. Later the original loads `[ebp-0xc]` (mark) before `[ebp-0x10]` (a) for `mark[a] = 1` (our order is reversed; `a[mark]` / `i[used]` spellings are canonicalised and change nothing), and keeps `cmp edx,ebx / jne` before the second edge load (ours loads both edges first: `p = edges[i]` then `edges[i+1]` compared separately). 500 random declaration orders: no change.
- d3_Unk454780 (from d3_vec.c, layout 2026-10-22): 5 bytes left (`row += n` computed in ecx, the original edx).
  With its body above d3_Unk454890, 454890 went 92 -> 235 bytes (register shuffle); recheck once 454780 matches,
  and if 454890 still prefers not to see it, 454780 may be a unit of its own.
- see notes/units/d3_par.md, Open work: "pfix leftovers after round 14: par_EmitterCB0 0x456350 (6/2064, size / flags store order), ..."
- see notes/units/d3_vec.md, Open work: "Round 12 leftovers, d3_vec.c: d3_VecAngle 0x4539f0 (4/336) and d3_MakeBasis 0x453b70 (2/49 ..."
- see notes/units/d3_vec.md, Open work: "Names: 0x453e80 is d3_VecEqual (d3.h) and d3_PtEq (d3_io_t.h); 0x4538a0 is d3_LightCalc453 ..."

## Worker geo (round 12): 0x454890..0x455630 + d3_light tail [geo]

### Report
- Scope 0x454890..0x455630 is one TU, the mirror code ("d3_mir:" allocation tags, strings 0x5fccb0..0x5fcd6c
  right after the "d3_light:" ones): src/game/unk_454890.c + include/unk_454890.h. 6 functions, Ghidra missed
  the callback 0x454ec0 (only reference: the pointer stored by 0x4554b0). The TU runs to 0x455630 (next string
  group "d3_par:" belongs to 0x455630..).
  - matched (4): 0x454be0 d3_Unk454be0 (clipping planes of the mirror polygon's outline + the view clip planes),
    0x454ec0 d3_Unk454ec0 (D3MIRROR::Transform: mirrors camera-space points at the plane, reprojects),
    0x454f70 d3_Unk454f70 (mirrored bounding box inside the planes? widens d3_MinZ / d3_MaxZ),
    0x4554b0 d3_Unk4554b0 (finds the polygon with the mirror texture, from d3_DrawObjectCB)
  - nonmatching (2): 0x454890 d3_Unk454890 ("CreateOutline", 256/864: three frame slots rotated),
    0x4550e0 d3_Unk4550e0 (d3_ForEachObject callback drawing the mirror image, 27/976)
- d3_light tail (src/game/unk_451e80.c):
  - One object view: D3OBJECT everywhere plus D3_L(o) (D3LOBJ, the fields d3.h does not name), D3LVERT (one
    vertex view: x/y/z, nx.., light / r,g,b, lit/color, base/specular, src, flags), D3LREC / D3LHIT (light
    records), D3LCTX / D3LCTX2, D3LINFO / D3LOPS / D3LDATA. include/unk_452940.h is merged into
    include/unk_451e80.h and deleted; the D3LVERTX #define, D3_X / D3_CAMPOS / D3_RADIUS2 / D3_LREC / D3_LFLAG /
    D3_LDIR / D3_LOPS and the d3l_FtoI copy are gone. No byte count of the other functions changed (one paren
    tie-break in d3_Unk452940 was needed to keep it at 874 after the switch).
  - matched: 0x453110 d3_Unk453110 (fell out of the unification: member access instead of the char* macro gives
    the original's `fxch; fadd [mem]`).
  - nonmatching: 0x452940 d3_Unk452940 874 -> 118/1104 (21 asm lines).
- Shared headers: nothing added or changed. Own headers: include/unk_454890.h, include/unk_451e80.h (rewritten),
  include/unk_452940.h (deleted, merged).
- Outside the scope: 0x454780 (worker vec2's scope) is the outline edge builder used only by 0x454890 (args n,
  conn bytes, points, &count, edge pairs); it is probably the start of the mirror TU, so unk_454890.c may have to
  join vec2's file or start at 0x454780. 0x458950 (rotates a direction into camera space) is used by both TUs.
  d3_engine.h declares d3_Unk4554b0(D3OBJECT *, D3OBJECT **) and d3_Unk4550e0(D3OBJECT *, void *); unk_454890.c
  renames them around the include (the second argument is &d3_Mirror, a D3MIRROR at 0xfb9900 that d3_engine.c
  sees as the separate globals d3_Unkfb9900/04/08/28).

### d3_Unk454890 (0x454890, nonmatching, 256/864)
- The edge count lives in the dead `pts` parameter slot ([ebp+8]) and `used` in n's slot. Only passing
  `(unsigned int *)&pts` to 0x454780 (nedge is a macro over pts) reproduces that; a separate local always took n's
  slot (declaration order, block scope, types, volatile: no effect). `i = nedge;` right after the call gives the
  original's register use.
- Left: three frame slots rotated (nres / mark / a: orig -8 / -0xc / -0x14, ours -0x14 / -8 / -0xc; the zero-size
  temp shares a's slot in the original, b's in ours) and the dedupe loop keeps res[j] in edx in the original.
  Tried: all positions and spellings of `a = b = 0`, a/b roles swapped, int/unsigned types, `nres = 0` placement,
  declaration orders, mark spellings, compare operand orders (about 25 runs).

### d3_Unk4550e0 (0x4550e0, nonmatching, 27/976)
- (1) Polygon point remap index: orig `lea ecx,[esi+eax]; mov ecx,[ecx+ebx]` (pointer IV plus a biased base, then
  the source polygon), ours `add ecx,esi; mov ecx,[ecx+eax-4]`; no spelling of `((D3SRCPOLY *)t->unk10)->idx[k]`
  (casts, parens, int pointer, p + idx, unsigned k, k != 3, local poly pointer) changes it.
- (2) After the inline-asm copies the original reloads esi (obj) before edi (s). This appeared only when 0x454890 /
  0x454be0 were defined above it in the file; declaration order / initialized declaration / merged guards: no effect.
  About 25 runs.

### d3_Unk452940 (0x452940, nonmatching, 118/1104)
What fixed most of it: D3VECTOR4 for d and c (frame 0x3c) with field copies, `if (v->flags & 0x80)` (jns),
`if (c.y > m)` max form, `if (d3_Unk633450) {rgb} else {grey}`, the append written as `p = list; if (p) { rec = p;
p = p->next; while (p) {...} p = rec->next = alloc; } else { list = alloc; p = list; }`, no `s` local for the
squared radius (`radius2 = (a + b) * (a + b)`: the original keeps the sum on the FPU and does not forward the
stored square to the later compare), `v->flags |= 0x80` before `n++`, `(float)((float)(1.0f / (z * dd)) * k * 10.0f)
* shade` (keeps the multiply order), `k` before `z`.
Left: (1) `z * dd` loads dd first (orig z first; parens, operand swap, statement order, dead stores: no effect);
(2) the walk loop uses the copy (`mov eax,[esi+8]` after `mov esi,eax`), ours the candidate; (3) the clamp: the
original keeps the scale on the FPU after `c.z *= m` and pops it after the d3_Unk633450 test (and does
`fxch; fmul st,st(1); fxch` for c.x); ours consumes it with `fmul [c.z]`. Scale as m / s / f, operand orders: no
effect. About 20 runs (plus d3x's few in round 11).

### VC6 lessons
- Operand-paren tie-breaks fix the load order of a dot product's products: brute-force every subset of the six
  operands in parentheses (`(planes->p[j].x)` and `(p->z)` matched 0x454be0, three operands 0x454f70).
  Term orders and operand swaps alone change nothing.
- `d3_MinZ = min(d3_MinZ, z)` (macro self-assignment) gives the original's `and eax,0x4100; je` + fld/fstp copy;
  `if (z <= d3_MinZ) d3_MinZ = z;` copies with integer moves here.
- `for (...) if (found) goto found; return; found:` gives a search loop with the return inside the loop;
  `break` + `if (i >= n) return;` adds a compare.
- `if (A) {T; redo = 1;} if (redo) {copy} else {loop}` reproduced the original block order where an else-if chain
  with a goto did not.
- A local named `ne` cannot be used in `__asm` (asm keyword).
- A 4-byte larger frame with a colour copied by integer moves and only .y/.z in memory: a D3VECTOR4 local with
  field copies (`c.x = v.x; ...`), not a D3VECTOR struct assignment (d3_Unk453280 / 4535b0, 452940).
- A grey sum whose three loads are hoisted before the multiplies (`fld x; fld y; fld z; fmul; fxch; ...`) came
  from computing it from that D3VECTOR4 copy (`c.x = k[i].light.x; ...; c.x * 0.3f + ...`), not from the vertex.
- A scale consumed by the last multiply (`fmul [c.z]`, then `jmp` over the pop) is a block-local float
  (`float q = 255.0f / s;` inside the if); the function-level `s` was kept on the FPU to the block end.
- Re-loading `k = info->keys` inside each branch of an if/else (instead of once before) fixed the register choice
  of the flag test and the loop pointer bias.
- Member access vs the same field through a `*(float *)((char *)o + off)` macro changes ranks: switching
  d3_Unk453110 to member access matched it, while d3_Unk452940 needed `(D3_L(light)->radius2)` in parentheses to
  keep its old code.

## Also about this unit
- notes/units/d3_par.md, section [pfix]

## match-base session 2026-10-24 (base-3 worker, shard 3/5)
Reference updates only (renames in a shared header): D3MESH unk048 -> cameraMatrix. No code change in this unit; the full ledger check
stayed at 2501 matched functions.

## match-base 2026-10-26 (base-2 worker, shard 2/10)

Readability only; no code change, check 2573/4664 unchanged, unitdiff d3_mir.c no worse, lint clean.
- Names (evidence = each function's own address comment; every use updated in src/ and include/):
  0x454be0 d3_Unk454be0 -> d3_MirCreatePlanes (the clipping planes of the mirror, one per outline edge),
  0x454ec0 d3_Unk454ec0 -> d3_MirTransform ("d3_Mirror.Transform"), 0x454f70 d3_Unk454f70 ->
  d3_MirBoxInPlanes (the mirrored bounding box against the planes), 0x4550e0 d3_Unk4550e0 ->
  d3_MirDrawObject (the d3_ForEachObject callback), 0x4554b0 d3_Unk4554b0 -> d3_MirBeginFrame (from
  d3_DrawObjectCB: makes obj the frame's mirror and builds the planes), 0x63344c d3_Unk63344c ->
  d3_MirrorObject (set to (int)obj by d3_MirBeginFrame, read as a flag by d3_mir.c and d3_engine.c - the
  d3_engine 0x43e200 @deep note calling it write-only is wrong: it has three readers).
- byte + readable now: d3_MirTransform, d3_MirBoxInPlanes, d3_MirBeginFrame (3 of 7).
- Still open: 0x454780 (bare `/* 0x454780 */`, no role described -> Q1 + Q6, needs a reading of its body),
  d3_MirCreatePlanes Q2 unk04 (the planes struct's +0x04 flag), d3_MirDrawObject Q4 (2 macros + 1 cast),
  d3_MirCreateOutline behaviour (4 regs hunks, the old c2prio lead) + Q2 0x454780 + 1 Q4.
- 0x454780 d3_Unk454780 -> d3_MirBoundaryEdges, BYTE + READABLE: it had a bare `/* 0x454780 */` (Q6 too), so the
  body was read and the description written - for every pair (i, j) the triangle (origin, i, j) is a boundary
  edge when all other points lie on its positive side; the pairs go to out, used is the n x n taken-matrix.
  Its Q2 cleared d3_MirCreateOutline's (now behaviour + draft on Q4 only: the `nedge` macro
  `(*(unsigned int *)&pts)` is a matching trick, deep's ground). Readable count 7 -> 4 open.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

D3MIRPLANES.unk04 -> mirror (with d3_engine.h's D3CLIPINFO.mirror, the same field): set to 1 only here
(d3_MirCreatePlanes, the mirror-outline planes) and read by the software/D3D draw loops as "run the clipper and
flip the winding test". Byte neutral (unitdiff d3_mir.c no WORSE, check 2603/4664).
