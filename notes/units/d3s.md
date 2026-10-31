# d3s.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3s:" allocation tags, texture "Schatten"; unk_45a2e0.c joined (round 14: "d3s:Cache" / "d3s:BuildingPolyBuffer(Shadow)" tags); the edge / span helpers 0x459d80..0x459fb0 are in d3s.c
- Boundaries: separated (plausible): start 0x459d80 (first edge / span helper of the "d3s:" unit, round 14); d3_GetBoxVerts 0x459ab0 (unk_4591a0.c) compiles the same in either file (layout 2026-10-22, 23 bytes both) and is called from outside d3s (0x411cb4, 0x457de1), so it stays with the mesh group
- Range then: 0x459d80..0x45ce00
- Was: unk_45b510.c (round 12)

## Open work
- d3s_AddShadow 0x45b8d0 (match-base 2026-10-31 opus3, deep, 327 vs 327, readable): rebuilt (was 296); left: ecx/edi and ebx/edi rotation over the mesh part, the size-fail `return ok` block (with fstp st(0) for v0) placed at the end instead of after the call epilogues, ok = 1 scheduled early, and the quad copies the slot floats with mov where the original uses fld/fstp (+ 0.0f / * 1.0f / (float) fold away).
- d3s_MeshFromMap 0x45b510 (match-base 2026-10-31, deep, 289 vs 297): the second loop reads the heights through the vertex's pos pointer (row[...].pos[1], the .y field is never set), loop invariants xs = x0 * cell[0] + origin[0] and ys = add + origin[1] hoisted, steep an int. Remaining: allocation (polygon pointer in esi / uv pointer in ecx in the original, ours the reverse) and the slots of x0 / z0 / nx / nz.
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_d3d.h, d3_sm.h, d3s.h, data_views.h; no @original cast or view.
- shfix leftovers after round 19 (notes/units/d3s.md [shfix], notes/units/unk_4591a0.md [shfix2]):
  d3_Unk4591a0 (5, `1.0f - r` operand order), d3_Unk458e70 (9), d3_GetBoxVerts (23), d3s_Unk459fb0 (234),
  d3s_Unk45a880 (389), d3s_Unk45a2e0 (575, not tried in round 19), d3_Unk459300 (1053), d3s_Unk45af50 (1063). TU 458810 + 4591a0 (d3_mesh.c?) unproven. D3STERR / D3STBLOCK views
  stay in d3s.h until d3_sm.h is cleaned up.
- see notes/units/d3_anim.md, Open work: "Type cleanup: D3MATERIAL+0xf4 is the D3ANIMCTL (slots at +0x110, so `tex[3]` at 0x178 is r ..."

## q3 (round 14): 0x45a2e0..0x45b510, src/game/unk_45a2e0.c [q3]

### Report
Scope: 11 functions.
TU: 0x45af50 (1472 bytes) **ends** my range exactly at 0x45b510, where d3s.c opens ("d3s:" TU, same TU). No gaps, no
callbacks hidden in the scope (11 Ghidra starts = 11 real functions; padding between them is nop only).
Suggested name: join into d3s.c (tag "d3s:Cache", "d3s:BuildingPolyBuffer(Shadow)", "d3s:BuildingPointBuffer(Shadow)",
texture name "*shadow"); the TU starts earlier (the q1/q2 scopes: 0x459d80 / 0x459e50 / 0x459fb0 are the edge
set-up and span functions that 0x45a2e0 and 0x45a880 call).

Nonmatching (bytes differing): d3s_Unk45ac60 0x45ac60 (39/288, only register names), d3s_Unk45a740 0x45a740
(149/304), d3s_Unk45a880 0x45a880 (491/736), d3s_Unk45a2e0 0x45a2e0 (642/928), d3s_Unk45af50 0x45af50 (1254/1456).
Names are ours, kept as the callers use them where a caller exists (d3_Unk45ad80 / d3_Unk45aee0 in d3_engine.h,
d3s_Unk45af50 / 45ac60 / 45a880 in d3s.c). Meaning: 45a2e0 = Gouraud-less shadow triangle set-up (calls the
q2 edge/span functions), 45a670 = left edge slope, 45a740 = span fill (8 or 16 bit), 45a880 = draws one shadow
polygon into the shadow texture, 45ab60..45ac60 = shadow texture cache (0x14-byte entries at 0x633dd0),
45ad80 / 45aee0 = shadow system init / shutdown, 45af50 = terrain shadow mesh with 8x8 blocks at per-block lod.
Shared-header additions: none. Header include/unk_45a2e0.h (own types: D3SCENT cache entry, D3STERR terrain view,
D3SPOLYR/D3SPOLYM/D3SVERTM/D3SBUFM views of the d3s.c structs; the orchestrator should merge them with d3s.h /
d3s.c when joining). No d3s.c edit. Note for d3s.c: d3s_Unk45ac60 really has 5 parameters (the fifth is unused; the
function tail-calls 0x45abc0 by rewriting its own five argument slots), d3s_Unk45a880's second parameter is read
as a full dword into `top` (probably `int`).

### VC6 lessons (not added to vc6-matching.md; for the orchestrator)
- Fixed-point helpers are `__inline` functions with `__asm` bodies: `mov eax,a; mov edx,b; imul edx; shrd eax,edx,N`
  (one inline function per shift count) and `mov eax,a; mov ebx,b; mov edx,eax; sar edx,16; shl eax,16; idiv ebx`
  (64/32 divide: no C spelling gives it). Plain `__int64` code gave `__allmul` / `__alldiv` calls (45a670 matched).
- The span fill is `rep stosb` / `rep stosw` in an `__asm` block fed from a plain `char *p` local, with
  `mov ecx, d3s_SpanLen; mov eax, d3s_Fill` read straight from the globals (the original asm names the globals).
- A by-value 4-byte flags struct filled as `memset(&f,0,4); f.b[0] = 0x7f; f.b[2] |= 2;` stores b2 before b0 in ours
  and b0 first in the original: write `f.b[0] = 0x7f;` first, then `f.b[2] |= 2;` (the or-load is still hoisted).
- Four zero stores to a global pointer array plus one zero store to a counter: `memset(arr, 0, 16)` before
  `count = 0` gives `xor ecx,ecx` stores for the array and an immediate for the counter (separate `= 0` stores
  hoist a zero register for the whole function).
- Unsigned `x > 0` test is `test; jbe` (a field declared `unsigned int`), not `test; je`.
- A float-to-int cast of `v * 65536.0f` with the literal gives `fld v; fmul const` (a global holding 65536.0f gives
  the constant first).
- Compare direction: `if (A > B) {reverse} else {forward}` with the fall-through body being the first source branch
  gave `and eax,0x4100; jne`; the flipped `<=` gave `test ah,0x41; jp`.
- Strength reduction of `p[4 - i*2]` / `p[5 - i*2]` (reverse walk over x, y pairs) produced the original's two separate
  pointer temps (&p[4], &p[5], shared with the preceding area expression); explicit `q -= 2` pointers merged them.

### Attempts on the nonmatching functions
- d3s_Unk45a740: all do-while / for shapes, `i = n` copy (right: orig copies n into the counter register), orders of the
  three `+=` statements (6 permutations), temps then stores, comma expression, `len` computed with and without the
  `x` temp. Left: ours puts x in ecx and len in eax (orig x eax, len ecx) and stores XL as soon as it is added, the
  original loads SlopeL, XL, SlopeR, XR and stores all three after `dec`.
- d3s_Unk45ac60: five parameters + `time - oldest > age` operand order fixed the compares and the tail call; left: the
  original keeps the CSE'd `d3s_CacheCount` in edi, `i` in esi and `oldest` in edx (ours edx / edi / esi).
  Tried declaration orders, init statement permutations (24), a separate counter for the second loop, `n` locals.
- d3s_Unk45a880: structure is now right (fall-through = reversed / flags branch, `and 0x4100; jne`). Left: the original
  keeps `top` in ebx loaded from its parameter slot at each loop preheader and `minY` in memory ([esp+0x20] = the
  dead `size` slot, cached in ebp), ours has them the other way round. Tried: uninitialised top, `volatile minY`
  (gives the [esp+0x20] home but top then spills), declaration orders, `top = bpp` inside the branches, `size` as
  the minimum.
- d3s_Unk45a2e0: structure matches up to the edge selection (two pointer temps, flags byte ops, area test). Left:
  register roles (orig minY in ebp, &p[5] temp in ebx; ours minY ebx), which shifts the whole rest. Tried
  declaration orders, init statement permutations, in-place y.
- d3s_Unk45af50: first full draft from Ghidra (1.4 KB, many float locals in memory, params uv / buf / tex / color in
  memory, frame 0xa0 vs ours 0x90). Fixed: signed first division, `nz` as a separate counter. Not iterated further
  (register allocation of ~20 live values). Later: x / z / xend ... as `int` (the original does `fild DWORD [x]`,
  unsigned only where the division is `div`), `h * cy + (oy + 1.0f)` (the original adds a hoisted oy + 1.0). Original
  slot map (for a later try): rect in ebp, buf in ebx, uv reloaded from memory; hoisted products fstep*uv[1],
  fstep*uv[3], fstep*cx, fstep*cz and fx0 / oy+1.0 live in float slots; frame 0xa0.

## shfix (round 15): d3s edge-helper move, shadow / mesh nonmatching leftovers, type merges [shfix]

### Report
- Matched (2): d3s_Unk45ac60 0x45ac60 (plain for loops), d3s_Unk45a740 0x45a740 (span length stored in the if test).
- Improved, still nonmatching (bytes differing, before -> after): d3s_Unk459fb0 551 -> 234, d3_Unk459300 1273 -> 1146
  (asm diff 586 -> 249 lines; rewritten from the asm, frame 0x74 now right), d3s_Unk45a2e0 642 -> 575,
  d3s_Unk45a880 491 -> 469.
- Unchanged: d3_Unk4591a0 134, d3_GetBoxVerts 23, d3_Unk458e70 25 (tried only 4591a0: 24 variants), d3s_Unk45af50 1261
  (not touched).
- Edge helpers (task 1): 0x459d80..0x459fb0 now open d3s.c as d3s_Unk459d80 / 459e50 / 459f90 / 459fb0, one set of
  fixed-point helpers d3s_MulShr14 / MulShr16 / ShlDiv16, d3s_ globals; zero byte change. Task 3: see below.
  Task 4: no evidence either way (joined variant compiles byte-identical).
- Header changes: include/d3s.h (merged q3 views, D3SPANPIX and D3SPOLY moved in, guard D3S_H), include/unk_4591a0.h
  (D3MVERT / D3MINFO / D3MOPS and the edge globals removed), include/d3_light.h (d3_Unk459980 takes
  `D3LINFO *, struct D3ANIMFRAME *`; D3LOBJ.mesh `struct D3MESH *`; fn1ec comment). d3_light.c unchanged in bytes
  (two new C4133 warnings at line 650: drop the cast and retype d3_Unk4535b0's `D3MATRIX *m` as D3ANIMFRAME * there).
- Outside the scope, noticed: 0x4598d0..0x459980 is a function without a names.csv row (Ghidra merged it into 0x459300,
  whose real end is 0x4598cc): d3_io_t.c stores it as obj->fn1ec (`d3_Unk4598d0`), it is the "transform normals
  without key frames" driver call of d3_Unk459980. Needs a row and source in a later round.
- VC6 lessons: (1) Ghidra-shaped do-while loops lose to plain `for` loops; a for loop whose counter is unused in the
  body becomes the `dec reg; jne` countdown (45ac60). (2) `if ((g = expr) > 0)` with no local reproduces a global store
  plus register test (45a740). (3) An inline-asm "macro" with four stack temps was an `__inline` function: inlined, its
  parameters are the temps and they are evaluated right to left (459fb0). (4) A conditional-select whose later branch
  ends with a register move (`mov ecx,eax`) is a separate variable assigned in each branch (45a2e0 `l`, 45a880 `c`).
  (5) A hoisted `neg w` used as a loop step is `j * w` strength-reduced, not `i -= w`. (6) `if (d3_MinZ >= z)` /
  `if (v->z >= d3_MaxZ)` give `fld g; fcomp st(1); test ah,5; jnp` and the fld/fstp copy; `z <= g` gives fcom/jp.
  (7) 16-byte vector locals with only z (or y and z) volatile reproduce the 459300 key-frame code.

### Work log

#### (1) Edge helpers moved into d3s.c
0x459d80 / 0x459e50 / 0x459f90 / 0x459fb0 now open d3s.c as d3s_Unk459d80 / d3s_Unk459e50 / d3s_Unk459f90 /
d3s_Unk459fb0. One set of fixed-point helpers (d3s_MulShr14 / d3s_MulShr16 /
d3s_ShlDiv16, the d3fx_ copies are gone), one name per global (d3s_EdgeX / EdgeY / EdgeC, SlopeL / SlopeR, XL / XR, the
new d3s_SlopeC 0xfc2dd0 and d3s_CL 0xfc2de8, d3s_SpanLen, d3s_Area (the span's per-pixel step), d3s_Dst,
d3s_Unkfb9270 now a D3SPANPIX *). d3s_Unk459fb0 keeps its (int, int, unsigned char, char, int) prototype: the callers
push the flags dword from its slot either way. Build: no byte changed (diff snapshot of every scope function identical).

#### (3) Types
- d3s.h: D3SCACHE was D3SCENT (tex, owner, time, size, flag): D3SSLOT.cache is a D3SCENT *, d3s_FreeSlot clears
  owner / tex / size. D3SSLOTC -> D3SSLOT, D3SPOLYR / D3SPOLYM -> D3SPOLY (moved from d3s.c into d3s.h),
  D3SVERTM -> D3SVERT (new fields w 0x0c, tu / tv 0x10 / 0x14 = position in the shadow texture), D3SBUFM -> D3SBUF.
  The three redeclarations of 0x45af50 / 0x45ac60 / 0x45a880 later in d3s.c are gone (one prototype per function),
  14 duplicated externs dropped. D3STERR / D3STBLOCK stay (views of d3_sm.h's D3TERRAIN / D3SMBLOCK, which lack bs, mip
  and lod; d3_sm.h is outside this scope).
- d3s_Unk45a880's bpp: the callers push a byte (`mov al,[eax+0x7c]; push eax`), so the parameter is unsigned char,
  but the body reads the full dword into top (`mov ebx,[esp+0x1c]`): written `top = *(int *)&bpp;`.
- unk_4591a0.c uses d3_light.h's D3LVERT / D3LINFO / D3LOPS (D3MVERT / D3MINFO / D3MOPS removed); it needs the same
  `#define d3_Unk453280 d3_Unk453280_void` around d3.h as d3_light.c. d3_light.h: d3_Unk459980 is
  `(D3OBJECT *, D3LINFO *, struct D3ANIMFRAME *)`, D3LOBJ.mesh is `struct D3MESH *` (so d3_light.h does not need
  d3_engine.h). d3_light.c builds unchanged (two new C4133 warnings at its d3_Unk459980 call, line 650: the cast to
  D3OBJINFO * and d3_Unk4535b0's `D3MATRIX *m`, which is really a D3ANIMFRAME * (m->_43 = radius at 0x38); d3_light.c
  is outside my files).

#### (4) unk_458810.c + unk_4591a0.c
Pasting unk_458810.c in front of unk_4591a0.c (`uv run try` variant) compiles, and d3_Unk4591a0 / d3_Unk459300 /
d3_Unk459980 / d3_GetBoxVerts come out byte for byte as before: no evidence either way.

#### (2) Nonmatching
- d3s_Unk45ac60: matched. Plain `for (i = 0, c = d3s_Cache; i < d3s_CacheCount; i++, c++)` loops (the second has no
  use of i and becomes the `dec edi` countdown); the do-while forms from Ghidra gave the swapped registers.
- d3s_Unk45a740: matched. `if ((d3s_SpanLen = (d3s_XR + 0xffff >> 16) - x) > 0)` with no length local.
- d3s_Unk45a880: 491 -> 469. Edge selection with `c` assigned in each branch (`c = d3s_Next[top*2+1]; if (EdgeY[c] ==
  minY) b = Next[c*2+1]; else { b = c; c = top; }`), and `minY = ...; top = i;` order in the loops. Left: the
  original keeps top in ebx (loaded from the bpp slot in each branch) and minY in memory (the size slot) cached in ebp;
  ours has minY in ebx, top in memory. Tried: uninitialised top (worse), volatile minY (worse), minY in the size
  parameter, `minY = lim = size << 16`, all 360 orders of the six entry statements, `<= 8` for `< 9` (no change).
- d3s_Unk45a2e0: 642 -> 575. The left edge start is its own variable `l` (the loop's `t` stays in memory, `l` lives
  in ecx until the call), the selection written as `e = Next[t*2]; if (EdgeY[e] == minY) { b = Next[t*2+1]; d = e;
  e = Next[e*2]; l = t; } else { l = Next[t*2+1]; if (EdgeY[l] == minY) { b = Next[l*2+1]; d = t; } else { b = l;
  d = t; l = t; } }`, and `d3s_Unkfb926c = arg;` before `left = ...`. That fixed minY's register (ebp) and the loops.
  Left: the original keeps b in ebx and d in memory (the dead slot of p), ours d in esi and b in memory; and y in
  ebp (in place of minY) where ours uses esi. Tried: 72 statement orders inside the selection branches, `y = minY`
  in place, `y += n;` before the inner if, the three spellings of the `left ? .. : ..` row count (the original's
  `lea edi,[eax+0xffff]` / `add edi,0xffff` arms are `left ? Yb + 0xffff : Ye + 0xffff`, but that spelling is
  worse until the registers are right).
- d3s_Unk459fb0: 551 -> 234 (same layout, frame 0x30, size 816 vs 800). Rewritten from the asm: the span loop is an
  `__inline` function `d3s_Span(u, n, dst, step)` (inline asm; its parameters are the four stack temps, u computed
  first; the macro gave the wrong temp order and a forwarded SpanLen), the span length stored in the `if` test as in
  45a740, `if ((k = d3s_Unkfb926c) > 0)` with `min(d3s_SpanLen + k * 2, lim = w - x0)` (the double evaluation of
  the min() macro is part of what made this one work), the post-loop border as `min(span + t * 2, w - lastx)` inside
  the call and its row offset as `j * w` (strength-reduced: the original's hoisted `neg w`), the post-loop row
  counter a new variable (its home is the dead flags slot). Left: fill2's slot ([ebp-0x1c] vs [ebp-0x18]) and the
  lim / dst temp sharing one slot, the inner border loop still `i -= w` (the `j * w` form there gives the original's
  `neg` but costs more bytes so far), a few swapped instructions in the loop steps. Tried: all 24 parameter orders of
  d3s_Span, flags as int / char, block-scoped lim, for vs do-while loops, loop step orders (16).
- d3_Unk459300: 1273 -> 1146. Rewritten from the asm: D3MVEC {x, y, volatile z, w} for the four scaled vectors and
  q1, D3MVEC2 {x, volatile y, z, w} for q2, frame 1 * b and frame 2 * a, first pass `v = q2 + q1`, later passes
  `q1 = q2 + q1; v += q1`, `if (obj->unk211 & 0x20)` (min/max) first, min/max as `d3_MinZ >= z` / `v->z >= d3_MaxZ`.
  Left: the weight is loaded for each product in the original (`fld w; fmul st,st(2)`, x/y kept until popped) and
  the slot pointer is based at &weight; product term orders in the transforms. Tried: compound / reversed scale
  statements, named x/y locals, inline scale helpers, D3VECTOR4 / D3VECTOR types, index slot loops, v++ placement.
- d3_Unk4591a0: 24 variants (float view of unk64, operand orders, parentheses): no change.

## Also about this unit
- notes/units/unk_4591a0.md, section [q2]
- notes/units/unk_4591a0.md, section [shfix2]

## match-base 2026-10-24 (base-0 worker, shard 0/2)

- Raised to byte + readable: d3s_RightEdge (0x459d80), d3s_LeftEdgeColor (0x459e50), d3s_MarkSpan (0x459f90),
  d3s_LeftEdge (0x45a670), d3s_FillRows (0x45a740), d3s_CacheInit (0x45ab60), d3s_CacheFree (0x45ab90),
  d3s_CacheBind (0x45abc0), d3s_CacheTexture (0x45ac60), d3_InitShadows (0x45ad80, Q2: the maxsize cap is
  d3d_UsedCard.info.maxw - 0xfe7998 is d3d_UsedCard 0xfe7680 + info 0x30c + maxw 0xc - now reached through
  d3_d3d.h, the extern d3_Unkfe7998 is gone) and d3_ExitShadows (0x45aee0).
- Q6 only: d3s_CollectLights (0x45c9f0), d3s_FreeSlot (0x45cc90), d3s_FreeShadows (0x45ccd0).
- Globals named: d3s_Palette 0x5ebbe0, d3s_LodTexSize 0xfb91d8 (the shadow texture size follows the mesh LOD
  distance), d3s_ShadowMips 0xfb921c (mip flag of the cached shadow textures), d3s_ShadowMapSize 0x633dcc
  (stored from d3_InitShadows' last argument 0x20, d3_interface.c passes it negated to d3sm_CreateRaw as the
  map size), d3s_SpanRow 0xfb9270, d3s_ShadowRadius 0xfb926c (d3sm_Build calls the triangle with `radius`),
  d3s_ShadowBits 0x1463590 (pixel buffer of the shadow texture, dd_Unk41de30(tex->surf, 0x20)). d3_Unk63342d
  -> d3d_ZEnabled (the alias already declared in d3_d3d.h; the local extern is gone).
- Escalated to match-deep (@deep reasons in the address comments): d3s_DrawSpans 0x459fb0, d3s_ShadowTriangle
  0x45a2e0, d3s_ShadowPoly 0x45a880, d3s_TerrainShadowMesh 0x45af50, d3s_MeshFromMap 0x45b510 - all are
  register / slot spelling of the same operations (the [shfix] attempts); MeshFromMap's 546 hunks are only
  stack displacements (frame 0x24 vs 0x14, every other constant identical on both sides, no call in it).
- In progress: d3s_GetGround 0x45bcd0, 6841 -> 6712 differing bytes. Two meaning fixes: (1) the original tests
  the parent twice - the source shape is `if (obj->parent) { p = obj->parent; while (p) ... }`, not one
  assignment + if; (2) `float y;` before the vector and `found = 0;` as a statement after `y = 0.0f;` gives
  the original's `mov [esp+0xc],0` before the argument pushes and `xor bl,bl` after them. One code hunk is
  left: the original's `if (found) return y;` is `jne` to the shared epilogue + `jmp` to the continuation,
  ours `je` + an inline pop/ret copy (same control flow). Open readability: Q2 D3OBJECT.unk1d4 (the same
  0x1d4 offset is D3LOPS.radius in d3_light.h, read here through info->mesh), Q4 the two
  `*(float *)((char *)obj->material + 0x954)`: D3OBJECT.material points to a D3MESH (0x970 bytes,
  d3_engine.h) whose `int unk954` is written as an int in d3_engine.c and read as a float here - needs a
  union view / a float type decision.
- d3_sm.c: 0x45a2e0 was declared and called there as d3_Unk45a2e0 but is defined in d3s.c as d3s_Unk45a2e0
  (different symbols); both are d3s_ShadowTriangle now (rename only, no byte change). The local prototype
  stays in d3_sm.c, so `uv run ledger grade` d3s.c still lists the foreign declaration.

### Further work in the same session (after the first wrap-up)

- The block scan in d3s_Project and d3s_Update read `.hi` (block +1 = 0x13d); the original reads block +2 =
  0x13e, i.e. `.lod`, and uses the value as a power-of-2 cell mask (`if (mh > 1) { m = ~(mh - 1); ... }` and
  `mh != slot->maxh`). Fixed both to `.lod`; include/d3_sm.h D3SMBLOCK gained the `lod` byte (the "which
  lacks lod" debt of d3s.h's D3STBLOCK), full check and d3_sm.c unitdiff unchanged.
- d3s_Project: the `+ 1` of the two rect ends is inside the cast in the original,
  `(int)((x1 - t->origin.x) / t->axisx.x + 1.0)` (double `1.0` at 0x5eb8a8, `fadd QWORD`), not
  `(int)(...) + 1`; both branches (d3sm_Shadow and t). 625 -> 591 differing instructions.
- d3s_Update: `obj->type >= 5` (was `> 4`) and `light->unk090.y == 0.0` (double, `fcomp QWORD`, was `0.0f`);
  663 -> 660 bytes / 283 -> 279 asm lines. Tried and rejected (WORSE, reverted): the tex_Load flags as a
  D3TEXFLAGS local (`memset + b[0]=0x7f + b[2]|=2`, which is what the original does at [esp+0x10]) plus a
  float local for the mesh 0x1d8 value (original spills it to [esp+0x14]) - 682/314 then 688/305 asm lines
  against 660/279; the frame 0x3c vs 0x34 is deep's slot work.
- Escalated in addition: d3s_Update, d3s_Project, d3s_AddShadow (all with @deep reasons). d3s_GetGround is the
  only base-queue function left (in progress, one branch-polarity hunk).

## match-base 2026-10-24 (base-0 worker, session 2)

- d3s_GetGround (0x45bcd0) byte + readable, the unit's base queue is empty. The last hunk was the found
  return: the source is `if (!found && obj->material) { ... } return y;` (one final return), which gives the
  original's `jne 0x45be06; jmp 0x45bd8c` (shared epilogue, the parent-test else block laid out between them).
- Readability: D3OBJECT.unk1d4 -> radius (same 0x1d4 as D3LOPS.radius in d3_light.h), D3MESH.unk954 ->
  groundOffsetRaw, and the float read through the unit's own view d3s.h D3SMESH { pad000[0x954];
  float groundOffset; } (D3S_MESH). The anonymous-union view in D3MESH - the standard fix - flips d3_Unk4550e0
  (d3_mir.c) by 8 bytes; so does a field added to D3MATERIAL (d3.h). Lesson in notes/vc6-matching.md.

## match-base 2026-10-31 (opus3)
- head includes d3_light.h (typing commit; all 28 functions unchanged): the light colour / power of
  d3s_AddShadow read through D3LOBJ (color 0x5c, power 0x94) instead of D3OBJECT.unk05c / unk090.
- d3s_AddShadow: one `if (d3s_Enabled && map && size && !simple)` / else quad; the range check reads the slot
  fields, the stores to x0..z1 come after; v0 = 0.0f first (kept on the FPU), du / dv / u0 / v0 divide by
  `(float)(x1 - x0)` / `(float)(z1 - z0)` (float fw / fh locals make VC6 multiply by the reciprocal instead);
  the size checks nest and fall into the final `return ok` (ok is a memory char; early `return ok` folds to
  xor al,al); uv[4] / rect[4] are copies; grid is a D3STERR (signed division by (int)grid->bs); the quad walks
  p++ / v++ / pos += 6 with pos computed before the polygon stores.
