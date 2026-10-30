# unk_4591a0.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (-): mesh key-frame blend / env-map UVs / normals / box verts (q2 suggests d3_mesh.c)
- Boundaries: separated (plausible): one mesh group (key-frame blend / normals / env-map UVs / box verts), 0x458e70 is a static helper only 0x459300 calls (moved in from unk_458810.c, layout 2026-10-22); d3_GetBoxVerts compiles the same in d3s.c and the join with unk_458810.c is neutral, so the name (q2: d3_mesh.c?) is the only open question
- Range then: 0x458e70..0x459d80

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_anim.h, d3_light.h, unk_4591a0.h, unk_458810.h; no @original cast or view.
- match 2026-10-23 (8th): d3_Unk459300 210 -> 165 asm lines. What did it: (1) the slot setup block as events in the order found by a hill climb (single-statement moves, 25 statements, metric = asm lines): x / y pairs as `V.x = F.x; V.y = F.y; V.x = w * V.x; V.y = w * V.y` (copy + multiply, so the two loads share the FPU), the z copies as separate volatile stores in the order s1, s2.z, b2.z, ..., see the source; direct `V.x = w * F.x` forms are worse (VC6 multiplies from memory); (2) `if (!(z < d3_MaxZ)) d3_MaxZ = *(volatile float *)&v->z;` (the compare uses the register z like the Min test, 12 lines fewer); (3) the term order of one transform row; (4) v->y before v->x in the first key loop. Left: the original's call arguments for d3_Unk4591a0 (`lea` / `push`) come after the last slot multiply (ours: hoisted above it; volatile a / b, dup statements: no change); `d3_MaxZ = v->z` is `fld [edx+8]; fstp [MaxZ]` in the original and an integer copy in ours (volatile source or destination, `(double)` local `double zz = v->z; d3_MaxZ = zz` gives the fld / fstp copy but costs 4 lines elsewhere, `info->keys[i].z` forms 205-226 lines); the transform rows' FPU operand order (spelling of m->_ij / sp->x as `((float *)m)[k]`, per-row term orders: no gain). d3_GetBoxVerts / d3_Unk458e70 untouched.
- match 2026-10-23 (7th), no gain: d3_Unk458e70 (9 B): perm and 31 spellings of the two `v->u` / `v->v` stores, loop-head spellings (`v++, i++`, `nkeys > i`, `continue` form): same 9 B. d3_Unk459300: `d3_MaxZ = v->z` is `fld [edx+8]; fstp [MaxZ]` in the original (a float copy through the FPU), ours `mov eax,[edx+8]; mov [MaxZ],eax`: `(double)`, `(float)(double)`, `* 1.0f`, `+v->z` casts on the source are folded to the same integer copy.
- match 2026-10-23 (5th): d3_Unk459300: the slot loop as `sl = &ctl->slots[0]; k = 3; do { ... sl++; } while (--k);` with the loops as pointer do-whiles and scalar temps instead of the q1 / q2 structs is far worse (413 lines vs 210): the original's slot pointer is based at `&sl->weight` (esi = slot + 0x40, frame index at [esi-0x40], data at [esi+0x28]), the same IV-base choice as msx_Update / qnt_Map; not kept.
- match 2026-10-23 (4th): d3_GetBoxVerts (23 B / 20 lines): hill climb over each of the six matrix rows (3 in the corner loop, 3 in the keys loop) with all 48 term orders / operand swaps x 6 parenthesisations (a+b+c+d, a+(b+c)+d, a+(b+c+d), (a+b)+(c+d), a+((b+c)+d), a+(b+(c+d))), 2 rounds: no row changes the count (23 B / 20 lines).
- match 2026-10-23 (3rd): d3_Unk458e70 (9 B): the moved load is `mov eax,[esp+0x7c]` = `entry` (stack slot) reloaded at the end of the first key loop's body for the next iteration's `(entry + 0xb8)` (the rotated loop head's load scheduled into the latch); the original puts it before the deferred `add esp,4` of the d3_Normalize call, ours after the following fmul. Tried: v->u / v->v order, `(1.0f + b.x)`, `0.5f * (...)`, `v++, i++` order in the for: no change (9 B) or worse.
- match 2026-10-22 (2nd pass): d3_Unk4591a0 matched: `*a = 1.0f - *(float *)&r;` (non-volatile read of the volatile `r` loads 1.0f first). d3_Unk458e70 (9 B, `mov eax,part` one slot earlier before the final fmul): goto done instead of return, `i = 0;` before the loops: no change.
- match 2026-10-22, no gain: d3_Unk458e70 (9 B, one load scheduled
  earlier in the original), d3_GetBoxVerts (all 48 operand/term orders of each of the six matrix rows tried: no change).
- see notes/units/d3s.md, Open work: "shfix leftovers after round 19 (notes/units/d3s.md [shfix], notes/units/unk_4591a0.md [shf ..."
- see notes/units/d3_io_t.md, Open work: "include/d3_io_t.h declares d3_Unk4598d0 as `void (void)`; it is (D3OBJECT *, D3LINFO *) (u ..."

## q2 (0x4591a0..0x45a2e0), file src/game/unk_4591a0.c, header include/unk_4591a0.h [q2]

### Report
Nonmatching (4): d3_Unk4591a0 (134/352 bytes), d3_GetBoxVerts 0x459ab0 (23/720), d3_Unk459300
(draft, 1273/1376, frame 0x54 vs 0x74), d3_Unk459fb0 (551/800).
- Scope content: mesh vertex animation (key frame blending, bounding box corners, normals) and the integer edge / span
  setup of the software rasterizer. No missed callbacks found in the gaps (all 8 Ghidra starts are real functions).
- TU: the range belongs to one file with q1's 0x4582d0..0x4591a0 as far as I can tell (459300 / 459980 call 458e70 /
  458da0, no regression evidence either way). Name suggestion: `d3_mesh.c` / guess, "mesh transform + edge setup"; the
  file ends at or after 0x45a2e0 (scope end), I did not look past it.
- Shared headers: none changed. Own header declares `d3_Unk458da0` (same as d3_engine.h) and `d3_Unk458e70`
  (q1's 0x458e70: `void (D3OBJECT *obj, D3MINFO *info)`; rename if q1 names it), `d3_MinZ/d3_MaxZ` (also in d3_engine.h).
- Type findings: `info` of d3_Unk459300 / d3_GetBoxVerts is the head of a D3ANIMCTL (D3OBJINFO at offset 0,
  `active` at 0x194, slots at 0x1c); d3_light.c's `D3MATRIX *m` passed to d3_Unk459980 (and d3_Unk4535b0) is really a
  D3ANIMFRAME* (+0xb8 = normals; its `m->_43` is the frame's `radius` at 0x38); D3ANIMSLOT::unk64 is a float
  (read with `fmul DWORD`); D3ANIMFRAME::count (+4) is the frame length in ticks.
- d3_GetBoxVerts has the name and prototype from d3.h (third parameter is a 16-float matrix there, void *).

### Lessons (VC6)
- Fixed-point helpers as `__inline int f(int a, int b) { __asm ... }` (inline asm returning in eax) ARE inlined, with
  the arguments in stack temps (an expression gets a temp, a global is addressed directly): d3fx_MulShr14/16, d3fx_Div16
  matched 459d80 / 459e50 on the first try, including `idiv` of `0x40000000 / dy` as plain C.
- Strength-reduced `i * 3` (`f->normals[i * 3 + 1]`, `info->keys` pointer + `v++`) puts the induction register
  setup after the loop guard; a hoisted pointer local `p = f->normals + i * 3` changes registers.
- A local `D3ANIMDATA *d = s->data;` fixed the register assignment (data in ecx) of d3_Unk4591a0.
- Operand order of products inside a dot product is rank based; 3072 parenthesization / permutation variants per row did
  not move d3_GetBoxVerts' last 23 bytes. `min(acc, v)` / `max(acc, v)` (accumulator first) from windows.h give the
  original compare forms (`and 0x4100; je` for min, `test ah,5; jnp` for max).
- `if (n == 0) return 0;` then one block and a final return: the early return stays inline with its own epilogue.

### Open work
- d3_Unk4591a0: forward path matches with `volatile float r` (the `1.0f - t` spilled); open are `fxch; fmul [s+0x64]`
  (the inverse of the frame length popped by the multiply, ours `fld; fmulp st(2)`) and the reverse path's
  `fstp [r]; fstp st(0); fld 1.0; fsub [r]` (non-volatile r). Tried: operand order, inv as local / expression / int
  count, r and t declaration order and scope, double r / t, volatile r on either path.
- d3_GetBoxVerts: 23 bytes: operand order of the z and y terms in the active-branch transform loop (c[i] terms) and the
  inactive row 1/2 terms. Row search over all parenthesization / operand-swap / term-order variants (2 x 3072 compiles)
  did not improve it.
- d3_Unk459300: written from Ghidra; layout wrong (frame 0x54 vs 0x74, slot pointer based at weight, the 12 products
  w * frame->scale/base are `fld a; fld b; fld w; fmul st,st(2)` with z copied through an integer register in the
  original: a local struct copy that is then scaled?). Not iterated beyond 8 variants.
- d3_Unk459fb0: structure right (rows loop, D3FX_SPAN inline asm span loop, border fills via d3_Unk459f90), register
  and block layout differ (original keeps 0 in ecx, `w * 0x1c` in edi, reloads the flags byte at the loop bottom;
  flags is an `unsigned char`).

## shfix2 (round 19, nonmatching slot): shadow / mesh leftovers of shfix, plus 0x4598d0 [shfix2]

### Report
No shared header touched.

- Matched (1): d3_Unk4598d0 0x4598d0..0x459977 (in src/game/unk_4591a0.c after
  d3_Unk459300). D3LOPS::fn1ec of the stock mesh driver (d3_io_t.c stores it): the normals of a mesh without key
  frames, from the normal at 0x0c of each vertex's source point, rotated by d3_Unk458da0's matrix.
  Trick: the normal is read as `((float *)v->src)[3..5]`, terms written `m._11 * n[3] + m._21 * n[4] + m._31 * n[5]`.
- Improved, still nonmatching (bytes differing, before -> after):
  - d3_Unk4591a0 134 -> 5: unk64 read as `((float *)s)[25]`. Left: `*a = 1.0f - r` (r volatile) gives
    `fld r; fsubr 1.0`, the original `fld 1.0; fsub r`. Tried: -r + 1, parens, casts, double, a temp, `*a = 1; *a -= r`.
  - d3_Unk458e70 25 -> 9: `volatile float d` (d is stored and re-read), `(m._11 * nx) + ...` in the second loop.
    Left: the reload of `entry` after d3_Normalize comes after `add esp,4` (original before). Scheduling only.
  - d3s_Unk45a880 469 -> 389: minY is the size parameter itself (declared `int size`, `size = lim` and the
    min search on size), and `top = *(int *)&bpp` moved into each branch after the flags test. Left: the cross
    product's evaluation order, top/minY register choice in the second loop.
  - d3_Unk459300 1146 -> 1053: weight read as `*(volatile float *)&sl->weight` (the original loads it for each
    product), slots by index (`for (k = 0; k < 3; k++) { sl = &ctl->slots[k]; ...`, gives the base pointer at
    &weight), min/max as `if (d3_MinZ >= z)` / `if (!(v->z < d3_MaxZ))`, plain `v->` loops with `i++; v++` at
    the end, `(vx * m->_11) + ...` in the min/max loop. Left: sl kept in edx for the d3_Unk4591a0 call, z products,
    fld/fstp copy of v->z into d3_MaxZ, scheduling in the transform loops.
  - d3s_Unk45af50 1261 -> 1063: rewritten from the asm. The 16 extra frame bytes were vectors: the origin copy
    is a D3VECTOR4 `o` (x, y, z), the base point a D3VECTOR4 `p` (p.x = x * cx + o.x, p.y = o.y + 1.0f, p.z = the
    row's z, enregistered), the two uv steps a `float d[4]` (d[1], d[3]); the height read as
    `heights[zi * stride + xi]` (VC6 makes the row pointer after the loop check), lod as an unsigned char tested
    `> 0` (jbe) then `step = lod` (unsigned: `(float)step` is the qword fild). Left: frame 0xa4 vs 0xa0, bz in ebx
    (original edi), buf in edi (original ebx), slot order.
- Unchanged: d3_GetBoxVerts 23 (7 runs: float-array views of src / m / c, inline transform helper, casts,
  parenthesized products, pointer loops; no change in the term order of rows 2 and 3), d3s_Unk459fb0 234 (3 runs:
  `j * w` in the border loop gives the original's `neg w` but 7 bytes more elsewhere; e expression orders),
  d3s_Unk45a2e0 575 (not tried this round).
- TU: nothing new on 458810 + 4591a0 (d3_mesh.c?).

#### General VC6 lessons
1. A float read through an array index (`((float *)p)[i]`) has a different rank than the same member access
   (`p->f`, `*(float *)&p->f`): it changed the operand order where 3000+ textual permutations did not
   (4598d0, 4591a0).
2. Parenthesizing the first product of a sum, `(a * b) + c * d + e * f`, moves its evaluation first (458e70,
   459300); parentheses around other terms did nothing.
3. A float the original stores and re-reads (`fst [slot]` ... `fld [slot]`) is a `volatile` local; a struct member
   loaded again for each product (`fld [esi]; fmul st,st(2)`) is a volatile read of that member (459300).
4. A loop over an array of structs written with an index (`sl = &ctl->slots[k]`) lets VC6 put the induction pointer
   at the most-used member (`lea esi,[ebx+0x5c]`, other members at negative offsets); a `sl++` pointer loop keeps
   the struct start.
5. A parameter slot that holds a running value (`mov [esp+arg],eax` in a min search) is the parameter itself
   reused as the variable (45a880 size = minY).
6. An unused-looking 16-byte hole in the frame with only some elements used: look for a D3VECTOR4 / float[4]
   (45af50 had three).
7. A row pointer that the original creates only after the loop's entry check comes from strength reduction of
   `base[i * stride + j]` in the body, not from a pointer variable in the for-init.

### Work log
See the commits on tu/shfix2 (one per function, with the trick in the message). Variant files: build/variants/.

## Also about this unit
- notes/units/d3s.md, section [shfix]

## match 2026-10-23
- d3_Unk459300: slot setup statement order found by a single-move hill climb with `uv run try` (the volatile `weight` reads and volatile `z` copies fix the order of the stores; x / y copies are free and must come as copy + multiply). tex_Unk4616a0 is not in this unit (ts_texture.c).

## match-base 2026-10-23 (match worker)

- d3_Unk458e70 -> d3_EnvMapCoords (the body reflects the view/normal per key vertex into the u/v env-map
  coordinates, both the animation-entry normals and the v->src normals path).
- Escalated: behaviour has exactly one code hunk left (an instruction-order/stack-slot artifact around
  `add esp,0x4`: the original loads the local at [esp+0x7c] before the cleanup, ours at [esp+0x78] after -
  same value); readability blocked on Q2 d3_Unk458da0 (unk_458810.c pose helper filling the world matrix)
  and unk210 (object flags, bit 0x80 = no camera), Q4 the XENTRY macro and the three
  *(unsigned char **)(entry + 0xb8) reads of the animation entry (needs the entry struct typed).
- d3_Unk4591a0 -> d3_AnimBlendWeights (writes the two frame blend weights *a / *b from the slot time, the
  cosine easing and the frames around it; the caller multiplies the two frame transforms by them).
  Escalated: Q2 unk04 (partner key frame index) and unk08 (position inside the current frame) are
  D3ANIMSLOT fields shared with d3_anim.c, Q4 the UNK64 float view macro over the int at 0x64.
- d3_Unk459300 -> d3_TransformVerts (transforms a mesh's vertices into camera space; the animated path
  blends three slots of two key frames first; tracks zmin/zmax with unk211 bit 5), and its call macro
  D3_458E70 -> D3_ENVMAPCOORDS. Escalated: behaviour (225 fpu + 107 code hunks = FPU operand ranks and
  register allocation of the blend/transform loops), Q2 unk04 / unk211, Q4 14 raw casts.
- d3_Unk4598d0 -> d3_TransformNormals (transforms each vertex's source normal into camera space with the
  world matrix; the D3LOPS::fn1ec callback of the stock mesh driver). Escalated: Q2 only, callee
  d3_Unk458da0 (unk_458810.c) and unk210 (object flags); behaviour byte.
- d3_Unk459980 -> d3_FrameNormals (vertex normals from the packed 3-byte normals of key frame f, in camera
  space; without a frame it defers to the driver's fn1ec = d3_TransformNormals). Escalated: Q2 only, callee
  d3_Unk458da0 and unk210; behaviour byte.


## match-base 2026-10-24 (match worker)
- The Q2 blockers in this unit's @deep reasons are named now: d3_Unk458da0 -> d3_WorldMatrix (unk_458810.c)
  and the pose view's flag byte (XOBJ.unk210 -> flags; D3OBJECT.unk210 itself is unchanged). d3_EnvMapCoords /
  d3_TransformNormals / d3_FrameNormals can be re-checked for readability with these names.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

The `#define d3_RelightObject d3_RelightObject_void` hack is gone (the d3_light.h declaration is the one now). No byte change.
