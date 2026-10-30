# d3_io_b.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_io_b:" allocation tags
- Boundaries: separated (proven): tag neighbours
- Range then: 0x44ee50..0x4507a0
- Was: unk_44ee50.c (round 12)

## Open work
- match 2026-10-22: still stuck, nothing new worked: d3_LoadBinaryAnimStream (2 B) tried again nothing new (locals renamed
  stream / frame / pt, byte-neutral); see notes/units/d3_int_io.md for AttachStock / LoadGroup / ReadFloor.
- d3 io leftovers after round 13 (two rounds without gain; only with a new idea; notes/units/d3_io_b.md):
  d3_LoadBinaryAnimStream 0x44f8c0 (2 B, base/index order in `[ecx+edx]`), d3_LoadGroup44c3f0 (4 B),
  d3io_ReadFloor (18 B), d3io_AttachStock (one lea), d3iot_CalcBounds (63 B), d3_FindBGF (276 B),
  d3iot_LoadBGF / d3_Unk451250 (one m/i register allocation). TU joins not proven: d3_io_a + d3_io_b
  compile together unchanged; a d3_io_b + d3_io_fl join first needs d3_Unk4507a0's prototype unified
  (VFSFILE * in d3_io_b.h, void * in d3_io_fl.h).

## Worker agi2 (round 11): 0x44ee50..0x44f8c0, 0x450860..0x451250 [agi2]

### Report
- 39 functions:
  - src/game/unk_44ee50.c (35, own file; includes/unk_44ee50.h): the binary BGF reader ("d3_io_b:BGI*Buffer" tags).
    Ghidra starts: 44ee50 (agi_Kind, text keyword LOCAL / ANIM), 44eea0 (bgi_ReadId), 44eee0 (bgi_Find),
    44ef10 (bgi_Parse), 44f870 (d3_ReadBGF). Missed by Ghidra: 29 handlers 0x44f000..0x44f7f0 (all 16-byte aligned,
    nop padded; found through the table at 0x5fc6f8..0x5fc8dc).
  - src/game/unk_450860.c (4, own file; include/unk_450860.h): d3_Unk450dc0 (face and vertex normals),
    d3_Unk450ec0 (radius + 8 bounding box corners, a sibling of d3iot_CalcBounds), d3_Unk451180 (size of a mesh in the
    fast chunk file) match. No Ghidra gaps in 0x450860..0x451250.
- Nonmatching: d3_FindBGF 0x450860 (276/1376 bytes differ, only register assignment; see below).
- What the functions really are:
  - The binary reader is a chunk parser: bgi_ReadId reads one id byte (0x2b at eof, 0x27 invalid, 0x28 = end of the
    table, 0x2f end of chunk); bgi_Parse looks the id up in a table of {id, handler | subtable, count} entries
    (bgi_Find, 64 entries max); handlers are (file, ctx). Tables (static BGIENT arrays in the .c, data not compared):
    top 0x5fc890 (7: flags 2, skip 1, skip dword 0, materials 3, object 0x14, attach 0x37, end), materials 0x5fc7b8,
    material 0x5fc6f8 (16 entries, 0x28 = next material), object 0x5fc860, geometry 0x5fc818, polygons 0x5fc7dc.
    The context is the same layout as the text reader's AGICTX (BGIRD in include/unk_44ee50.h), with flag bits at +0,
    nattach at 0x34 and 4 D3BGFATTACH at 0x38; d3_ReadBGF casts the D3BGFDESC of unk_44cd40.h to it.
  - d3_FindBGF (named by d3io2) is the loader of the "fast chunk" cache ("d3_io_fl:LoadFastChunk(obj / poi / pol /
    texset)" tags): opens the file with d3_Unk4507a0 (0x4507a0, anim scope), reads the mesh straight into a D3BGF
    (verts + 8 box corners, polygons with normals, texture names with tex_Load, attachments). It returns 0 if there is
    no cache file. d3_Unk451180 computes the size of that file for 0x451250 (the writer).
  - The material record read in d3_FindBGF is a whole D3BGFMAT local (0xe0 bytes): name[0], name[2], name[1], kind, c1,
    c2 (packed byte), pad4, c8, c9 are read; c3 / c5 / c6 are unpacked from c2 (>> 6 & 1, >> 1 & 1, >> 2 & 0xf).
- TU boundary: not known. The "d3_io_b" strings (0x5fc8e4..) follow the tables behind the "d3_io_a" ones, so the binary
  reader is very likely the same TU as unk_44e570.c (0x44e570..0x44f870+, then 0x44f8c0 d3_Unk44f8c0 maybe too); I kept
  an own file as ordered, the orchestrator can join unk_44ee50.c behind unk_44e570.c (the callee set of the two parts is
  disjoint, so nothing should change). 0x450860..0x451180 (fast chunk loader, normals, bounds, size) is probably one
  TU with 0x451250 (writer, mesh worker's scope) and 0x4506f0 / 0x4507a0 (anim's scope).
- Shared headers: nothing added to existing ones. include/unk_44ee50.h and include/unk_450860.h are new and include
  unk_44cd40.h's types (D3BGFMAT, D3BGFPOLY, D3BGFDESC, D3BGFATTACH, D3BGF, D3BGFFLAGS / D3BGF_TEXLOAD) so the .c files
  include unk_44cd40.h first. Do not include d3_anim.h next to d3_engine.h (d3_Unk4319e0 clashes): d3_Unk454700 is
  declared in unk_450860.h (copy of d3_anim.h's prototype).
- Notes for the orchestrator: bio_rd_c / bio_rd_v3 are declared with void * in my headers (as in unk_44abb0.h).

### VC6 lessons
- A local that is read with `bio_rd_c(f, &b)` and then shifted must be `unsigned char` (`sar` with a plain char,
  `shr` with unsigned char); the D3BGFMAT fields are plain `char`, so cast: `(unsigned char)mt.c2 >> 1`.
- A recursive table walker with two calls that differ only in the table arguments: write the two calls out
  (`if (!e->n) { depth++; f(t, n) } else { depth++; f(e->fn, e->n) }`), not as ternary arguments or via tt / nn
  temps; the original's store of the depth counter before the e->n reload and the block order (n == 0 first) only
  come out this way.
- Dead-parameter-slot locals again: `int t; bio_rd_i(f, &t);` in a handler whose f is dead lands in f's slot
  (bgi_PolyVerts: one `int a` read three times; tail call `bgi_SkipInt2` rewrites the second argument slot with &t).
- Indexed vs pointer loops decide the strength-reduction base: `p = m->polys; ... p[i].v[0]` (a hoisted local base
  plus index) gave the original pointer based at v[2] / v[1]; `for (i...) m->polys[i].v[0]` reloads, and `p->v` / `p++`
  bases at the last field. Increment order `i++, vt++` (not `vt++` as the last statement) matters.
- `strlen(a) + 1 + strlen(b) + 1 + strlen(c) + 1` summed over `d->mats[i]` (index form, not a `mt++` pointer) gives the
  original's two induction pointers (name[0], name[1]) with name[2] at +0x40; the pointer form keeps one.
- `if (n > 0) { ... if (n > 0) {loop} }`: the compiler removes a redundant inner test of the same variable; the
  original kept it as `if (m->nverts > 0)` (a re-read member), with a hoisted FPU value (`z1 = b[5]` before it).
- Minimum / maximum in a loop: `if (v < b[0]) b[0] = v;` gives `fld v; fcomp b; test ah,5; jp`; the maximum written
  as `if (v > b[1])` gives `and eax,0x4100; jne` (the original), `if (b[1] < v)` loads b first.
- A `float b[6]` filled by `if (k & 1) b[k] = -1e10f; else b[k] = 1e10f;` (odd test first) matches the original branch
  sense; `(k & 1) == 0` first gives the inverted jump.
- `unsigned char kind; if (kind > 0)` gives `test al,al; jbe`, `if (kind)` gives `je`.
- A `volatile char` local reproduced a flag the original keeps in memory and re-reads (`mov bl,[esp+0x13]; test bl,bl`)
  where a plain local took the free register (d3_FindBGF `kept`).
- A 16-byte frame gap before a local array: a D3VECTOR4 (not D3VECTOR) local read with bio_rd_v3.
- Pointer difference of 0x80-byte records to an index: `(int)(((char *)tex - (char *)d3_Textures) >> 7)` gives
  `sub; sar 7`; `tex - (D3TEXHANDLE *)d3_Textures` is a magic-number division (D3TEXHANDLE is not 0x80 bytes).

### Attempts (d3_FindBGF 0x450860, nonmatching, 276 / 1376 bytes)
- Whole control flow, stack layout (frame 0x10c, D3BGFMAT local at esp+0x3c, D3VECTOR4, volatile kept) and every
  instruction sequence match except the register assignment: the original has the byte offset of the vertex loop
  (`a`, starts as the constant 0) in edi, the pointer `&m->nverts` in ebx, and compares against the zero register
  (`cmp ebp,edi`, `cmp eax,edi`; `xor edi,edi` again before the texture-name test); ours has a in ebx, pn in edi, `test`
  compares, and the texture loop keeps the name offset in ebx.
- Tried (no change in the diff): declaration order of a / m / f / pn (3 permutations), no pn (the compiler makes a CSE
  temp of &m->nverts that also takes edi), m->nverts vs *pn in the first test / loop bound, `f != (void *)a`,
  `*pn > a`, assigning a = idx, `ntex != (a = m->ntex)`, `a` as the texture index (much worse), zero assigned as statements / `a = m = 0`, unsigned a. 22 compiles in all.
- Ideas not tried: the original probably shares one variable (Ghidra's iVar13) for vertex offset, polygon counter,
  m->ntex and the texture index; a different split of a / idx / cnt through one more shared variable might move the
  register priorities.

## Worker anim (round 11): 0x44f8c0..0x450860 + d3 io nonmatching leftovers [anim]

### Report
- 0x4506f0 d3_Unk4506f0 (char: 0 when the header flag byte of a format-2 file has bit 1 set, else 1),
  0x4507a0 d3_Unk4507a0 (opens a chunk file and seeks to the chunk with id 0xfab50005; returns the open VFSFILE or 0;
  only caller 0x450860 d3_FindBGF).
- Nonmatching (1 in scope): 0x44f8c0 d3_LoadBinaryAnimStream (3632 bytes, one function, Ghidra did not merge
  anything; ends at 0x4506ef, 0x4506f0 follows without padding). Complete; ~70 asm lines left, all in the per-frame
  finishing block (see Attempts). Name from its error strings ("... in d3_LoadBinaryAnimStream()"); its allocation
  tags say "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream...)".
- Leftovers (all still nonmatching, nothing changed in their files): d3_LoadGroup44c3f0 (4 B), d3io_AttachStock
  (one lea), d3io_ReadFloor (vector copy order), d3iot_CalcBounds (one FPU pop). New attempts below.
- No Ghidra gaps in the scope: 0x44f8c0 / 0x4506f0 / 0x4507a0 are the only starts (0x450791..0x45079f and
  0x450858..0x45085f are nop padding).
- TU: own file src/game/unk_44f8c0.c / include/unk_44f8c0.h (self-contained). The three functions belong to the
  "d3_io_b:" TU, which starts before 0x44f8c0: the strings "d3_io_b:BGIMaterialBuffer / BGIPointBuffer /
  BGIPolyBuffer" (0x5fc8e4..0x5fc918) come right before this function's strings (0x5fc930..0x5fcaf8) and belong to
  code in agi2's range (0x44ee50..0x44f8c0; 0x44f870 d3_ReadBGF uses 0x5fc890). The next string is
  "d3_io_fl:LoadFastChu" (0x5fcb2c), probably 0x450860+ (another TU). The chunk tag reader 0x44eea0 (agi2's scope)
  is declared as d3_Unk44eea0(VFSFILE *) in my header; it only calls vfs_fread, so whether it is defined in the
  same file makes no difference to the callers' registers.
- Shared headers: nothing added or changed. unk_44cd40.h declares 0x44f8c0 as `D3IOANIMDATA *d3_Unk44f8c0(char *,
  char *, char)` and 0x4506f0 as `int d3_Unk4506f0(char *)`; the real types are `D3BASTREAM *` and `char` return
  (D3BASTREAM's prev/next at 0x160/0x164 are D3IOANIMDATA's). For the join: rename d3_Unk44f8c0 ->
  d3_LoadBinaryAnimStream and fix the d3_Unk4506f0 return type (unk_44cd40.c uses it as an int argument).
- Struct layout (include/unk_44f8c0.h): D3BASTREAM 0x16c (name, 4 link names at 0x40, nverts 0x140, nlinks 0x144,
  nframes 0x148, first / last 0x150 / 0x154, frames 0x15c, list 0x160, relative flag 0x169), D3BAFRAME 0xc0
  (time, dt, min, scale = size / 255, pos, rot, 4 links of two vectors at 0x54, packed bytes 0xb4, points 0xbc).
  Chunk tags: 0x30 stream, 0x23 frames, 0x33/0x24 read vertices / uv flags, 0x37 links, 0x36 sub-chunks per
  frame, 0x34 / 0x35 vertex / uv count overrides, 0x29 / 0x2a first / last, 0x18 time, 0x19 vertex count,
  0x21 vertices, 0x28 done, 0x1a / 0x22 uv faces, 0x31 pos / rot, 0x38 link name, 0x39 / 0x3a link points,
  0x32 trailer, 0x2f end of chunk.

### VC6 lessons
- A loop counter in memory (address-taken elsewhere or register-starved) used by two unrelated loops: write
  two variables. VC6 still shares their stack slot, but the slot moves (one variable at 0x28 shifted 7 other
  slots; two variables put it after them, as in the original).
- `a->nframes = nframes; a->last = a->nframes - 1; mem_Alloc(a->nframes * size)` forwards the stored value
  (`mov eax,ecx; lea edx,[eax-1]`); using the local again reloads it from its (address-taken) slot.
- Bounding-box loops: the operand order and flag test follow the ternary's spelling, and VC6 does not
  canonicalize them: `mn.x = p->x > mn.x ? mn.x : p->x` gives `fld p; fcomp mn; and eax,0x4100; je`,
  `mx.x = p->x < mx.x ? mx.x : p->x` gives `test ah,5; jnp`, and for the FPU-resident max
  `mx.z = !(mx.z > p->z) ? p->z : mx.z` gives `fld p; fld st(1); fcompp; and 0x4100`. stdlib min/max loads the
  accumulator first. A countdown `for (i = nv; i; i--, p++)` inside `if (nv > 0)` gave the original's guard
  placement (the pointer load after the test); `for (i = 0; i < nv; ...)` hoisted the load above it.
- `(unsigned char)((w.x * 255.0f) / v.x)` keeps `fld w; fmul 255; fdiv v`; without the parentheses VC6 computes
  `255 / v.x * w.x`.
- An int local compared as unsigned (`(unsigned)k > 0xabcd0001`, `jbe`) and later used as a signed loop
  counter is one variable in the original (the frame has one slot; Ghidra shows it as local_7c).

### Attempts
#### d3_LoadBinaryAnimStream (0x44f8c0, nonmatching, ~14 `uv run try` runs, ~55 variants)
- Left (ours vs original): (1) the two `frames[k].points = mem_Alloc(...)` branches have their two address
  operands swapped (`[ecx+edx]` / `[edx+ecx]`); not changed by `nvfix < 1` with swapped arms (block order
  flips), braces, casts, `sizeof * n`, a ternary inside one assignment (merges the calls). (2) In the bounding
  box loop the x copies `mn.x = p->x` / `mx.x = p->x` are integer moves in ours, fld/fstp in the original (y and
  z match); not changed by parentheses, casts on the arm, `p[0]`, a `float *` walk, if-forms, declaring p in
  the for. (3) The finishing block (min stores, size, scale, `dt *= 3`, the wpoints alloc) differs in register
  choice (original reuses eax for `k * 0xc0` and loads frames after the index) and `dt *= 3` is 2 bytes longer
  (ours `mov ecx,[ecx+edx+4]`, original `mov ecx,[eax]` through the lea'd address); not changed by `= dt * 3`,
  `3 * dt`, `dt + dt * 2`, a pointer local, parenthesized operands, `* (1.0f / 255.0f)`, size computed first.
  Fixing (3)'s two bytes would also remove the jump displacement differences (most of the 482 differing bytes).
#### d3_LoadGroup44c3f0 (4 tries this round)
- `else { if (f) return first/0; vfs_fclose(in); }` and an early mismatch return before the version test:
  both worse (57 / 175 bytes). Base (4 bytes) kept.
#### d3io_AttachStock (5 tries)
- `tri->tex = ...; tex = tri->tex;` (worse), `(D3IOTEX *)(d3_Textures + i)`, `i + base`, `tex = base; tex += i`,
  `tri->tex = (tex = ...)`: all still `add ecx,edx; mov esi,ecx`.
#### d3iot_CalcBounds (16 compiles)
- z1 declared in an inner block ending after the diagonal, a block-local dz, `z1 -= mn.z` with all 6 orders of
  the three squares (with and without the block): z1 is still popped after the diagonal store, or worse.
#### d3io_ReadFloor
- No new idea beyond notes/units/d3_int_io.md's list; not retried.

## Worker d3io4 (round 13): nonmatching d3 io functions + header cleanup [d3io4]

### Report
- Big improvement: 0x44f8c0 d3_LoadBinaryAnimStream 482 -> 2 bytes (91 -> 4 asm lines). Left: the two
  `a->frames[k].points = mem_Alloc(..)` stores encode base/index the other way round (`[ecx+edx]` vs `[edx+ecx]`),
  in both branches, whichever arm comes first in the source.
- Unchanged nonmatching: 0x44b920 d3io_ReadFloor (18 B), 0x44c3f0 d3_LoadGroup44c3f0 (4 B), 0x44c610
  d3io_AttachStock (one lea), 0x44ce40 d3iot_CalcBounds (63 B, one fmulp/pop), 0x44d330 d3iot_LoadBGF,
  0x450860 d3_FindBGF (276 B), 0x451250 d3_Unk451250. Attempts below.
- Header cleanup done (commit cca98ee): d3_io_t.h declares `struct D3BASTREAM *d3_LoadBinaryAnimStream(char *,
  char *, char relative)` (by struct tag, no d3_io_b.h include needed; d3_LoadAnimStream casts to its
  D3IOANIMDATA view), `char d3_Unk4506f0(char *)`, `void d3_Unk451250(char *, char weld)`; the `#define` hack in
  d3_io_b.c is gone. All three including TUs (d3_io_t/b/fl.c) compile byte-identical; the C4028 is gone.
- TU probes (scratch compile of concatenated files, every names.csv function compared; build/scratch/join.py):
  - d3_io_a.c + d3_io_b.c joined: compiles, all 62 matched functions still match, AnimStream still 2 B. Neutral
    (no evidence for or against one TU, but no obstacle to joining).
  - 0x4506f0 / 0x4507a0 moved to the top of d3_io_fl.c: both still compile to the original code, and d3_FindBGF
    (0x4507a0's only caller) and d3_Unk451250 do not change at all. Neutral too: no evidence that they open the
    d3_io_fl TU. A plain concatenation of d3_io_b.c + d3_io_fl.c needs the d3_Unk4507a0 prototypes unified
    (VFSFILE * in d3_io_b.h vs void * in d3_io_fl.h, C2371).
  - d3_io_t.c + d3_io_a.c joined: neutral as well.
- Shared headers: nothing added. Files touched: include/d3_io_t.h, include/d3_io_b.h (comment), src/game/d3_io_t.c,
  src/game/d3_io_b.c.
- VC6 lessons:
  - A float copy `mn.x = cond ? mn.x : p->x` from a struct member at offset 0 (`[reg]`, no displacement)
    compiles to integer moves; members at +4/+8 give fld/fstp. When the original has fld/fstp at offset 0, write the
    arm as an rvalue that folds away: `(p->x * 1.0f)` or `(p->x + 0.0f)` (unary `+` does not work). This also
    changes the register round-robin of the code after the loop (fixed ~480 bytes in AnimStream).
  - Moving `x = ...; dt *= 3;` statements around changes nothing about the address operand order of a
    `a->frames[k].f` store; base/index order in `[r1+r2]` is canonicalized against all spellings (`(a->frames +
    k)->`, `k[a->frames]`, byte-pointer arithmetic in both orders, parentheses).
  - Float args of an `__inline` helper give loads-first-then-stores for a 3-float copy (as fld x3 / fstp x3).

### Header cleanup (done)
- d3_io_t.h: `d3_Unk44f8c0` replaced by `struct D3BASTREAM *d3_LoadBinaryAnimStream(char *, char *, char relative)`
  (declared by struct tag so d3_io_t.h does not need d3_io_b.h; d3_LoadAnimStream casts the result to its
  D3IOANIMDATA view). `d3_Unk4506f0` returns char, `d3_Unk451250(char *path, char weld)`. The `#define` around
  the d3_io_t.h include in d3_io_b.c is gone. No byte changes in any caller (d3_io_t.c, d3_io_b.c, d3_io_fl.c are the
  only includers); the d3_io_fl.c C4028 is gone.

### d3_LoadGroup44c3f0 (0x44c3f0, nonmatching, 4 bytes, ~22 compiles)
- The 4 bytes: the original's `in == 0` je and the mismatch branch's `f != 0` jne go to the epilogue copy right
  after d3_AddChild (the shared return block sits there, the mismatch block after it has its own copy); ours put
  the shared return block at the very end.
- `else { if (!f) vfs_fclose(in); return first; }` (or `return 0`) fixes the jumps but swaps esi/edi:
  original f + obj in edi, first in esi; ours first in edi. Not fixed by on top of it: block-local obj,
  `in = f; if (!f) open`, `if (!f) open else in = f`, `first = 0` placed later (much worse), nested loop ifs,
  `0x3a6c00ae <= version`, `in != f`, `return first` in the match branch too.
- Worse: early `if (!in) return first/0;` (252 B), goto to a trailing mismatch label (175), `do {} while (0)`.

### d3io_ReadFloor (0x44b920, nonmatching, 18 bytes, ~21 compiles)
- New finding: an `__inline` helper with float parameters, `io_SetOrg(map, pos.x, pos.y, pos.z)` storing
  `m->orgx = x` etc., gives the original's loads-first-then-stores shape (but as fld/fld/fld/fstp x3, 13 bytes).
  Any int-typed temp (int locals, int helper parameters, pointer `int *` copies, struct copy, read before the if)
  also gives loads first with integer moves, but then the allocator caches `version` in edi across d3fl_LoadMap
  (original reloads [ebp+0xc] into eax each time) and map moves to esi (200 B). With
  `*(volatile unsigned int *)&version` in the last test the version part matches (23 B), leaving map esi/zero edi
  swapped and the load order y, z, x. Float locals in the if block, `float *` helper, helper returning map: worse.
- No match; base kept.

### d3iot_CalcBounds (0x44ce40, nonmatching, 63 bytes / one fmul vs fmulp + one pop, ~23 compiles)
- All of these compile to the identical (wrong) `fld st(0); fmul st,st(1)` ... `fstp st(0)`: `__inline` square
  helper on `z1 - mn.z` or on dz, an inline length helper with the z difference as a by-value parameter (also
  with pointer x/y), `dz *= dz`, `z1 -= mn.z; z1 *= z1`, `dz = dz * dz`, the whole sum into dz first,
  `dz = (z1 - mn.z) * (z1 - mn.z)`, a block-local float t. Worse: double dz / double t, reusing d or r for dz,
  `z1 = z1 * z1 + ...`.

### d3io_AttachStock (0x44c610, nonmatching, one lea, ~11 compiles)
- Still `add ecx,edx; mov esi,ecx` for any of: no tex local (much worse), `(char *)d3_Textures + (idx << 7)` in
  both operand orders, `+ idx * sizeof(D3IOTEX)`, an int-cast address sum. A separate counter j for any of the
  three inner k loops is worse (353-849 B): the k register sharing is right.

### d3_LoadBinaryAnimStream (0x44f8c0, nonmatching, 2 bytes, ~16 try runs)
- Fixed: the bbox loop's x arms `(p->x * 1.0f)` (mn.x and mx.x). That alone fixed parts (2) and (3) of
  notes/units/d3_io_b.md (finishing block register rotation and the 2-byte `dt *= 3`).
- Left: `[ecx+edx+0xbc]` / `[edx+ecx+0xbc]` swapped in the two points alloc branches. No change from: `(a->frames
  + k)->`, `k[a->frames]`, `(a->frames)[k]`, `(a)->frames`, `[(k)]`, `[k + 0]`, `(&a->frames[k])->`, byte-pointer
  sums in both orders, parenthesized lvalue / call, `(D3BAPOINT *)` cast, braces, `* 12`, `0 < nvfix`,
  `if (a) if (ofs == 0)`, `!ofs`. `nvfix <= 0` with swapped arms swaps the blocks but the first block in the
  layout still gets ours' order (5 B). A pointer temp for the result or two separate ifs: much worse.
- Index-form bbox loop (`p = &points[i]` each iteration) compiles identically to the pointer walk.

### d3_FindBGF (0x450860, ~10 runs)
- `off2 = 0` / `cnt = 0` before the `!texnames` test gives the original's `xor edi,edi; cmp ecx,edi` there, but
  hoists the stores (775+ B). Zero assigned to tex / a / j / idx before the test, using the vertex offset variable
  for the polygon offset too, and reading the attachment count into off2 / cnt: no gain (same or worse).
  The original stores off2 and the attachment count in one slot ([esp+0x24]); ours already does.

### d3iot_LoadBGF / d3_Unk451250 (m/i allocation, ~2 runs)
- Both spill m around the weld loop ([ebp-0xc] in LoadBGF); the difference is that the original keeps m in edi
  outside the weld region and i cached in ebx inside it (home [ebp+8], i*0x18 home [ebp+0xc], k [ebp-4]); ours
  m in ebx, i in memory at [ebp+0xc], i*0x18 at [ebp-8], k at [ebp+8]. With ebx/edi swapped (`--regs`), the slot
  homes of i / i*0x18 / k are the rest. Tried: an `__inline` mesh-alloc helper (worse), block-local weld loop
  counters (395 vs 409 asm lines, not kept), `while` inner loop, `!desc.flag01`.
- Also identical: `* 1.0f` / `+ 0.0f` folds on dz or z1 (the AnimStream trick does not apply here).
