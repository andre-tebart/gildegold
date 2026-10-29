# d3_io_t.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_io_t:" allocation tags
- Boundaries: separated (proven): tag neighbours
- Range then: 0x44cd40..0x44e4a0
- Was: unk_44cd40.c (round 12)

## Open work
- include/d3_io_t.h declares d3_Unk4598d0 as `void (void)`; it is (D3OBJECT *, D3LINFO *) (unk_4591a0.c, round
  19).
- see notes/units/d3_io_b.md, Open work: "d3 io leftovers after round 13 (two rounds without gain; only with a new idea; notes/units ..."

## Worker d3io2 (round 10): 0x44cd40..0x44df40 [d3io2]

### Report
- File: src/game/unk_44cd40.c, include/unk_44cd40.h. TU: allocation tags "d3_io_t:DeleteStockObject" and
  "d3_io:..." (LoadtextureSet, LoadAGF(...), LoadAGI(obj), RemoveDoubleMaterials, ...). The file format is
  ".BGF" (strupr + strstr(".BGF") in the loader, ".TXS" for the texture set). The tags differ from unk_44abb0.c
  ("d3_int_io:"), so probably a neighbouring TU of the same I/O code; the string "%s_%i" at 0x5fc100 just before
  the first tag belongs to the previous function (d3io's scope).
- Functions (6; Ghidra found 5, 0x44df20 was a gap function between 0x44d330 and 0x44df40):
  - 0x44cd40 d3iot_DeleteStockObject: matched (mesh refcount release, frees its textures).
  - 0x44ce40 d3iot_CalcBounds: NONMATCHING, one FPU pop (see below); everything else identical.
  - 0x44d330 d3iot_LoadBGF (3056 bytes): NONMATCHING, written in full (every block), not close in bytes yet.
  - 0x44df20 d3iot_GetRadius(int unused, D3OBJINFO *info): matched (`info->unk10->radius`, 0.0 if none; the first
    argument is ignored, the pointer is the SECOND argument).
- Callees outside my scope are declared in include/unk_44cd40.h under our names: d3_FindBGF 0x450860 (cache
  lookup by path/name), d3_Unk4506f0 / d3_Unk451250, d3_Unk450dc0, d3_ReadBGF 0x44f870 (fills D3BGFDESC),
  d3_PtEq 0x453e80 (point compare with epsilon 0.001f), d3io_MakePath 0x44c530 (unk_44abb0.c, d3io's scope).
- Shared headers: nothing added to existing headers.
- Nothing of this TU continues past the scope end that I know of (0x44df40 is another function).

### VC6 lessons
- A zero held in a variable (`int z = 0;` and `for (i = z; ...)`, `if (--m->refs <= z)`) makes VC6 emit
  `cmp reg,reg` instead of `test` / `cmp reg,imm` for every compare against it, and hoists the zero into a
  register at function entry (d3iot_DeleteStockObject). With plain literals the same loops give `test`.
- Locals reused for several reads (`bio_rd_i(f, &i)` for magic and width, `&j` for height, then loop counters
  i/j) are what puts "param_1 / local_4" in Ghidra on dead parameter slots: two ints, frame 4 bytes
  (d3iot_LoadTexSet matched only with this reuse).
- Gaps in a frame (unused slots between used ones) were again struct locals whose members are enregistered:
  `D3VECTOR4 mn, mx` in CalcBounds (0x20 frame, mx.z lives on the FPU, w unused). The same struct members are
  reused as the FPU accumulators of a later sum loop (`mn.x += v[0]` ...), and the third accumulator is spilled
  into its slot: that reproduced `fstp [esp+0x14]` (a separate sz went into a dead parameter slot instead).
- Initializing three FPU accumulators with one chained assignment (`sx = sy = sz = 0`) reverses their stack
  order; separate statements (x, y, z) give the original order.
- Loop pointer base: `v0 = m->verts; v = v0;` based the walking pointer at +8, `v = m->verts; v0 = v;` and
  `v0 = m->verts; v = m->verts;` at +0 (CalcBounds second loop).
- min/max style within one function: x `if (!(mn.x < v[0])) mn.x = v[0];` (int copy, test ah,5; jnp), y/z
  `min()/max()` macros (fld/fstp copy), mx.x `if (!(mx.x > v[0]))` (and eax,0x4100; je). The z of the max stays
  on the FPU and uses fcompp.
- `d = (float)sqrt(y*y + x*x + z*z); if (d > r) r = d;` gives `fst [param slot]; fstp st(2); fcomp st(1)` with the
  running maximum kept on the FPU.

### d3iot_CalcBounds (0x44ce40, nonmatching: a single instruction and one pop)
- Everything matches except the square of the z extent: the original has `fld st(0); fmulp st(1),st` (z consumed)
  where ours has `fld st(0); fmul st,st(1)` and a `fstp st(0)` after the diagonal store (z kept alive until the end
  of the block). Tried (about 20 compiles): z diff as named local, `mx.z -= mn.z`, inline
  `(mx.z - mn.z) * (mx.z - mn.z)`, all six term orders, z as a plain float instead of mx.z, z shared with r.
  The z maximum is a named FPU variable in all of them, which VC6 pops only at the end of its block.

### d3iot_LoadBGF (0x44d330, nonmatching)
- Structure (all blocks present and in the original order): strcpy + strupr + strstr(".BGF"); two cache lookups
  (d3_FindBGF) with d3_Unk451250(buf, d3_Unk4506f0(buf)) between them, each hit returns after d3iot_Finish(m, 0);
  D3_ZERO of the 0x198 byte D3BGFDESC; vfs_Tfopen/d3_ReadBGF/vfs_fclose; duplicate-point merge (O(n^2),
  d3_PtEq, fixes polygon indices, memmove); points rotated by -pi/2 then pi (sin/cos in float locals, `* -1.0f`);
  RemoveDoubleMaterials (memcmp of 0xe0 byte materials, polygon material remap), RemoveUnusedMaterials
  (memmove compaction); polygon copy and material reorder with D3_COPY (src through a block-local pointer);
  texture coordinate transform per polygon vertex (scale/offset around 0.5, rotation by mat.rot, second axis by
  rot + pi); texture set names (name[2], else name[0], else name[1]); tex_Load per material with a
  D3BGFFLAGS union (b0 = 0xff or c1, c0 |= 1 for kind 2, b1 = c7, c1 bit = c2) and the argument bytes
  `((c3 << 4 | c6) * 2 | c5) * 2 [| 1 if name[2]]`, `c8 << 4 | c9`; polygon texture assignment; attachment
  copy (4 x 0x58); Finish(m, 1); cleanup of desc.mats/polys/pts.
- Frame: original 0x2d0 = buf 0x100 + desc 0x198 + 0x38 of locals; ours 0x2b0..0x2c0. The 0x38 are 14 slots of
  which 6 are never used: most likely two D3VECTOR4 locals (x of the first at -0x38 and the third float of the
  second at -0x20 are the only members in memory; unused slots -0x34..-0x28 and -0x1c fit exactly). With
  `D3VECTOR4 av, bv` for x and w the frame grows by 0x10 only (the compiler kept av.x on the FPU).
- Rotation loop of the original: x stored to memory first (`fstp [x]` right after loading pts[0]), y and z
  addressed through two pointer registers (`lea edx,[..+4]`, `lea ecx,[..+8]`), y' = c1*y - s1*z computed first and
  kept on the FPU until the store, then t2 = c1*z + s1*y, u = x*c2 + s2*t2 (before w), w = t2*c2 - x*s2 stored
  to memory, u *= -1.0. The current statement order (t1, t2, u, w, u *= -1) reproduces the arithmetic, but VC6
  folds the pointer locals into the address mode.
- Register/slot differences left (all of them follow from the frame): in the dedupe loops the original keeps i in
  ebx with a shadow store in param slot [ebp+8] and its row offset in [ebp+0xc], m in edi; ours has m in ebx.
- A frame-normalizing diff helper is in build/helpers/nasm.py (untracked; build/ is ignored).

## Also about this unit
- notes/units/d3_io_a.md, section [d3io3]
- notes/units/d3_io_b.md, section [d3io4]


## match-base 2026-10-24 (match worker)
- d3iot_CalcBounds: readable via D3BGF_CORNER (d3_io_t.h), a D3BGFVERT corner view. The inline cast keeps
  the original's per-store reload of m->verts/m->nverts into [edx+eax*8+disp]; a local pointer changes the
  addressing and was not tried further.
- d3iot_Finish, d3iot_GetRadius, d3io_LoadObject, d3_LoadAnimStream: byte + readable (Q2/Q6). D3OBJINFO.unk10
  is now `mesh` (all uses; KEPT as D3OBJECT * - an anonymous-union alias in d3.h regressed d3_Unk4550e0 in
  d3_mir.c by 8 bytes, see notes/vc6-matching.md). d3_Unk6333cc / D3IOOBJ.unk40 have no evidence yet.
- d3iot_LoadBGF -> match-deep (frame/register cascade, 558 code hunks; Q2 d3_Unk4506f0 and unk1e4).
- Renames owned here: d3_Unk44c9d0 -> d3io_LodName (the owner name in d3_int_io.h; the duplicate
  declarations in d3_io_t.h are gone), d3_Unk44c5d0 -> d3io_FindStock, d3_Unk5fbf78 -> d3io_Lod (D3IOLODBITS
  view in d3_io_t.h, same symbol/address as d3_int_io.h's D3IOLOD; d3_int_io.h itself cannot be included
  from d3_io_t.c - D3IOKEY is redefined), d3_Unk433f40 -> d3_FindAnim (d3.h).
- d3_io_t.c base queue empty.

## match-base 2026-10-24 (match worker, reference updates from d3_anim.c)

- d3_Unk4324e0 -> d3anim_NextFrame: the call in d3_io_t.c and the D3IOANIMFLAGS declaration in d3_io_t.h
  follow the rename (the declaration conflict with d3_anim.h stays in lint_baseline.txt under the new name).
  Byte unchanged (10/12 matched, 2419 B).
