# d3_io_fl.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_io_fl:" allocation tags
- Boundaries: separated (proven): tag neighbours
- Range then: 0x450860..0x451250
- Was: unk_450860.c (round 12)

## Open work
- match 2026-10-23 (4th), lead for d3_FindBGF (276 B / 102 lines, not started): the original has `a` (the vertex byte offset, `int a = 0`) in edi and `pn` in ebx, ours the reverse (c2prio: pn 81 > a 78, so pn takes the first free callee-saved register after m esi / f ebp); the zero compares are `cmp ebp,edi` (the register of `a`, value 0) where ours use `test`. Next: lower pn's reference weight (some of the five `*pn` uses as `m->nverts`) or raise a's, then read c2prio again.
- see notes/units/d3_io_b.md, Open work: "d3 io leftovers after round 13 (two rounds without gain; only with a new idea; notes/units ..."

## Worker mesh (round 11): 0x451250..0x452400 [mesh]

### Report
- Scope held 4 functions, not 2: Ghidra's 0x451250 (4.2 KB) is three functions. 0x451250 ends at 0x451e7c;
  0x451e80 and 0x451f10 are callbacks it missed (their only references are pointers stored at 0x452e16 /
  0x452e2b, in worker d3x's scope). 0x4522b0 runs to 0x452938, past the scope end; it is a Ghidra start inside
  the scope, so I wrote it.
- In src/game/unk_451e80.c + include/unk_451e80.h:
  - 0x451e80 d3_Unk451e80 (callback, resets the lit vertex colours of a light's vertex lists)
  - 0x451f10 d3_Unk451f10 (callback, flickering light: new random level every `interval` ms, lerp, adds the
    light colour to the vertices, clamped to 255 / 0x3f)
  - 0x4522b0 d3_Unk4522b0(D3LOBJ *obj, D3LSET *set), char: point lights (with / without vertex normals) and
    directional lights (type 7; per vertex through d3_Unk458da0's matrix, or per triangle face normal) summed
    into the vertex colours
- Nonmatching (1): 0x451250 d3_Unk451250(char *path, char weld) in src/game/unk_451250.c +
  include/unk_451250.h ("d3_io_fl:AddFLChunk(obj)"): complete, every block in order, frame and all stack slots
  of the big locals right (0x2d4); 2531/3120 bytes differ, but the shape-normalized diff (registers and stack
  offsets ignored) is 113 instruction lines. Details below.
- TU boundaries:
  - 0x451250 belongs to the "d3_io_fl" TU (its tags: d3_io_fl:AddFLChunk(obj), d3_io_fl:NotMyChunkBufferStart,
    d3_io_fs:*), which starts with worker agi2's 0x450860 ("d3_io_fl:LoadFastChunk"). It is the last function
    of that TU. Join unk_451250.c/.h into agi2's unk_450860.c/.h (my file declares agi2's 0x450ec0 / 0x451180
    as d3_Unk450ec0(D3BGF *) / int d3_Unk451180(D3BGF *, D3BGFDESC *); rename to agi2's names).
  - 0x451e80.. is a new TU: tag "d3_light:rlight_object" / "d3_light:cache" (strings 0x5fcc88/0x5fcca0, used
    at 0x452c3a / 0x452c51 in d3x's scope), followed by "d3_mir:*" strings. So d3x's 0x452940.. functions up to
    about 0x452c.. continue my unk_451e80.c (the light TU; tag suggests the original name d3_light.c, not proven
    by a path string, so I kept unk_451e80.c). 0x4522b0 is called from 0x45338e and 0x453702 (d3x's scope).
- Shared headers: nothing added or changed. Own headers: include/unk_451e80.h (D3L* views of D3OBJECT,
  light data, mesh keys/tris, the light set; d3l_FtoI / d3l_FtoI2 fistp helpers), include/unk_451250.h.
- Warnings: unk_451250.c defines d3_Unk451250(char *path, char weld) while include/unk_44cd40.h declares
  (char *path, int x): C4028 only. The second parameter really is a char (`mov al,[ebp+0xc]; test al,al`).
  At the join the declaration in unk_44cd40.h could become char (not tried: check that d3iot_LoadBGF, the
  caller, compiles the same).

### VC6 lessons
- Loop over a list's array of 8-byte refs, counted by a field: the original's `test; jle; mov edx,eax; ... dec edx;
  jne` with the pointer in ecx came only from `cnt = n->n;` in a local and the pointer set in the for
  initializer: `for (i = 0, p = n->v; i < cnt; i++, p++)` (pointer set before the loop: registers swapped).
- fistp helper argument order: `d3l_FtoI2(float f, int *dst)` (float first) stores the `&k` temp before the
  expression's temp and puts them in the original slots; `(int *, float)` swapped two slots (d3_Unk451f10,
  d3_Unk4522b0 all four call sites).
- `fld k; fmul r` (constant-like local loaded first) needed a fresh variable whose first textual occurrence
  is after r's (`q = 255.0f / m; r *= q;`); reusing an older variable `f` loads r first (rank rule again).
- `(float)(1.0f / (dist * a)) * s` keeps `fdivr 1.0; fmul s`; without the cast VC6 folds it into `s / x`.
- A frame 4 bytes short with a scalar at +0x60 inside a 12-byte block of an unrelated accumulator: the
  accumulator was a D3VECTOR4 and the scalar its .y member reused in another branch (`acc.y = base`).
- Shrink-wrapped pushes (`push ebx` at entry, the other three after `test ebx,ebx; je`): `if (!info) return 1;`
  at the top AND only one other return at the end; an extra `return 1` inside a branch kept all pushes at entry.
- `bio_wr_c(f, 0xff)` pushes `0xff` only with an unsigned char parameter; with `char` it is `push -1`.

### d3_Unk451250 (0x451250, nonmatching)
Same algorithm as d3iot_LoadBGF (unk_44cd40.c, copied code with other tags), then writes the converted mesh
as a fast-load chunk appended to the file: 0x2d, d3_Unk451180(m, &desc), 0xfab50005, ntex, nverts, npolys,
verts (+8 box corners) as two v3 each, radius, per poly 3 indices, u's, v's, the vector at +0x2c, the material
index (byte, 0xff for -1, if ntex < 255), per material 3 names and 6 flag bytes, the attachments, 0x2f.
What is right: block order, frame 0x2d4 (needs `D3VECTOR4 v` for the 12-byte write vector and the rotation's x,
`D3VECTOR4 bv` with only .z in memory, buf[256]), the D3_ZERO / copy macro shapes (D3FL_ZERO_AT for
`used`, D3FL_COPY with dst temp before size for m->polys, D3_COPY_AT for tmp, D3FL_COPY3 for the material
reorder), `mi > -1`, `desc.polys[k].mat > i`.
Left (same three problems d3io2 found in d3iot_LoadBGF):
1. m (`[ebp-0x10]`) is never register-allocated in the original; ours keeps it in ebx from the mem_Alloc
   through the weld loops and the rotation loop, so the weld loop's i loses ebx (original: i cached in ebx with
   write-through to its param-slot home). Diagnostic: moving the m allocation after the weld loop gives the
   original's i-in-ebx code, so it is register competition, not loop shape. Tried without effect: declaration
   order, `register`, `= 0` init, block-scoped loop variables, the weld loop as an __inline helper, the loop
   counter as the (int) parameter itself, renaming loops to share one counter (VC6 allocates live ranges, names
   do not matter), a copy variable for m. `volatile m` is far worse.
2. Rotation loop: the original keeps `&pts[k*6+1]` / `+2` in edx/ecx (two lea), ours folds them into the
   address mode; statement order and pointer-first order did not change it (probably a consequence of 1).
3. RemoveDoubleMaterials outer loop: the original keeps i (1..n) in memory and indexes `used[i-1]` as
   `[eax+edx-1]`; VC6 turns ours into an i-1 induction variable (also likely register pressure). `i <= n` is worse.
Smaller: argument pops after 0x450ec0/0x450dc0 are flushed in the original before the ftell chain (8, 0x48,
0x3c), ours merges them (0x40, 0x44, 8); `vfs_fwrite(.., out = vfs_Tfopen(..))` and an __inline Finish helper
did not change it. The early `vfs_fclose(f); return;` block sits in the middle of the original (after the
material-index branch); the if/else form did not move it. Write-loop float copies are integer moves in the
original: `((int *)&v)[n] = *(int *)&..` gets the moves but reloads m->polys after each store.
Tries: about 15 `uv run try` runs plus single edits.

## Also about this unit
- notes/units/d3_io_b.md, section [agi2]
- notes/units/d3_io_b.md, section [d3io4]

## match-base 2026-10-23 (match worker)

- Names from evidence (all in this unit): d3_Unk450dc0 -> d3_CalcNormals (body: d3_TriNormal per polygon into
  pad2c, then smoothed vertex normals), d3_Unk450ec0 -> d3_CalcBounds (radius + the 8 bounding-box corners
  behind the vertices), d3_Unk451180 -> d3_FLChunkSize (byte size of one fast chunk record), d3_Unk451250 ->
  d3_AddFLChunk (leaked mem_Alloc tags "d3_io_fl:AddFLChunk(obj)/(poi)/(pol)"); 5 references updated in
  d3_io_t.c and the headers.
- d3_CalcNormals and d3_FLChunkSize are byte + readable now.
- Escalated: d3_FindBGF (behaviour: 220 code hunks are only the zero register / handle register choice -
  `cmp ebp,edi` with edi = 0 vs our `test`; readability: Q2 d3_Unk4507a0 + unk1e4, Q4 the 9 raw poly stores),
  d3_CalcBounds (Q4: a typed D3BGFVERT view for the 24 corner stores changes the codegen and lost the byte
  match, tried and reverted; byte otherwise), d3_AddFLChunk (behaviour: 377 code hunks are register/slot
  churn - ebx vs edi zero register, [ebp-0x198] slots; Q2 unk1e4, Q4 7 casts).
- Lesson: in the poly loader loop, `D3BGFPOLY *p` walked with `p++` is much worse than the original's
  `(char *)m->polys + byte offset` spelling (102 -> 296 differing asm lines), so keep the byte-offset form
  until a writer match needs otherwise.
