# d3_int_io.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_int_io:" allocation tags
- Boundaries: separated (plausible): "d3_int_io:" tags; start after d3_obj.c (open, but that question is d3_interface.c / d3_obj.c, not this file)
- Range then: 0x44abb0..0x44cbb0
- Was: unk_44abb0.c (round 12)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_engine.h, d3_fl.h, d3_interface.h, d3_int_io.h; no @original cast or view.
- match 2026-10-22 (nothing gained, new tries recorded; the older list is in notes/units/d3_io_b.md):
  - d3io_AttachStock: still `add ecx,edx; mov esi,ecx` instead of `lea esi,[ecx+edx]`. New: int local `ti = tri->src->tex`
    before the test, `ti + (D3IOTEX *)d3_Textures`, a `D3TEXTURE *` local with casts at the uses, `&((D3IOTEX *)d3_Textures)[i]`,
    `tex += i`, `(int)d3_Textures + (i << 7)` / `* 0x80`: all identical or worse.
  - d3_LoadGroup44c3f0 (4 B): the `else { if (!f) fclose; return first; }` shape fixes the jumps but swaps esi / edi
    (original: f in edi, first in esi); all 120 declaration orders of the five locals and six spellings of the loop
    body (first/obj tests) did not move it (29 B at best).
  - d3io_ReadFloor (18 B): struct copy / memcpy / block-local D3VECTOR / volatile version / volatile map: 197 B and worse.
- see notes/units/d3_io_b.md, Open work: "d3 io leftovers after round 13 (two rounds without gain; only with a new idea; notes/units ..."

## Worker d3o2 (round 9): 0x44abb0..0x44be60 [d3o2]

### Report
- 0x44abb0 rd_object (3440 bytes): one function with two jump tables, not merged ones; the
  name comes from the "Unkown Object-Type in rd_object()!" message.
- Nonmatching: 0x44b920 d3io_ReadFloor (4 asm lines: the order of a 3-dword vector copy).
- File: src/game/unk_44abb0.c, include/unk_44abb0.h. TU: allocation tag "d3_int_io:FloorWaterRegions", so the
  original file is probably d3_int_io.c. It likely includes unk_449960.c (worker d3o) and continues past the scope
  with d3_LoadScene44be60 (0x44be60), which calls both functions. Strings 0x5fbeb0..0x5fbf3c are this TU's,
  in the order the code uses them.
- Shared headers: nothing added. include/unk_44abb0.h declares, for the join:
  - bio_rd_c/s/i/v3/v4/bio_rd_array_quick with `void *f`, inside `#ifndef UNK_401000_H` (unk_401000.h pulls in
    unk_430130.h, which clashes with unk_43eab0.h on vfs_Root / vfs_Unk42fbf0);
  - d3_Unk44a9e0 (0x44a9e0, in d3o's scope: key count for a version), tex_Reload, d3_AppendSibling,
    d3e_LoadEvents prototypes, extern d3_Unk6339e4 (0x6339e4, char, set when an LOD file is missing);
  - D3IOOBJFLAGS (bitfield view of D3OBJECT::unk210..212, macro D3IO_FL), D3IO_200 (int at obj+0x200),
    D3IOKEY (the 0x38-byte D3LIGHTENTRY with fields), D3IOFLAGS (tex_Load flag union with bitfields);
  - D3IO_TEXLOAD: tex_Load cast to its real (unsigned char, unsigned char) last arguments; d3.h declares them
    int, which would zero-extend the computed byte argument.

### VC6 lessons
- The rank of a loop index variable decides the operand order of `lea r,[base+index+disp]` in a later loop:
  `&keys[i].f28` (keys reloaded each iteration) came out as `[iv+light]` instead of `[light+iv]` until an
  earlier, unrelated loop used another variable (`k`) instead of `i`. No spelling of the address expression
  (casts, `+`-order, member arrays, unsigned index) had any effect.
- `D3OBJECT *obj = 0; if (version >= X) { ... } return obj;` keeps the callee-saved pushes at entry and
  lets the early exit jump into the shared epilogue with eax = 0 (shared with `char dummy = 0`). An early
  `return 0` shrink-wraps the pushes; an uninitialized obj on that path costs an extra stack slot.
- Initialized locals are stored in declaration order (`int a = 0, b = 0` order visible in the prologue).
- `type = obj->type; switch (type)` right after the call that returns obj schedules the type load before the
  following member stores; `switch (obj->type)` after the stores loads it late.
- Copying one member vector into another member of the same object: field by field gives three loads, then
  three stores; a struct assignment gives a pointer copy (lea + [reg+4]).
- 2-bit bitfield `x |= (x & 1) << 1` compiles to `mov cl,al; and cl,4; add cl,cl; or cl,al`.
- VC6's inline strcpy is a byte loop (`mov cl,[src]; mov [dst],cl; inc; test cl,cl`); strcat uses repnz scasb.
- Frame layout in d3io_ReadFloor came from block scopes: locals declared in the loop body / in the `if`
  that uses them get the slots nearest ebp and share the D3_ZERO temps' slots; a char used only in one block
  took the dead parameter slot of `f` ([ebp+0xb]).

### d3io_ReadFloor (0x44b920, nonmatching, 4 asm lines)
- Layout solved by block scopes: the region loop body declares `name`, `c1, c2, c3`, `fl`; the "has regions" /
  flag byte `c` is declared in the `if (version >= 0x3a6c00aa)` block (it lands in the dead slot of `f`,
  [ebp+0xb]); `water` in the `if (version >= 0x3a6c00b9)` block; `pos` is a D3VECTOR4.
- Left: the original copies pos into map->orgx/y/z as three loads, then three stores; ours interleaves
  (pos is address-taken, map a returned pointer). Tried: struct copy (D3VECTOR / D3VECTOR4 cast), memcpy,
  float temps, int temps, int pos[3], inline helper by pointer and by value, local struct copy, all 6 store
  orders, parenthesized reads, comma expression, `(&map->orgx)[k]`, D3VECTOR-pointer stores.

### rd_object (0x44abb0, matched)
- Needed: `obj = 0` initializer + one return; `int unk220 = 0;` declared before `int unk200 = 0;`;
  `type = obj->type; switch (type)`; field-by-field `unk090 = rot` copy; surf loop index `k`.
- Case order in the source is 1/4 (falls through into) 2/3, 0, 5..8, default (block layout follows it).

## Worker d3io (round 10): 0x44be60..0x44cd40 [d3io]

### Report
- 0x44c5d0 d3_Unk44c5d0: stock object by name; 0x44cbb0 d3_Unk44cbb0: attach a stock object / its LODs to a
  mesh.
- Nonmatching (2): 0x44c3f0 d3_LoadGroup44c3f0 (4 bytes: two jumps go to the other copy of the epilogue),
  0x44c610 d3io_AttachStock (1 instruction: `add ecx,edx; mov esi,ecx` vs `lea esi,[ecx+edx]` for the
  texture pointer; 1 byte longer, so the rest shifts).
- No functions missed by Ghidra in the scope (nop padding checked); the 8 Ghidra starts are 8 functions.
- TU: all in src/game/unk_44abb0.c (the "d3_int_io" TU). Evidence: 0x44c9d0 uses rd_object's pooled "_s"
  string (0x5fbee4); the data after the LoadScene strings is this TU's: 0x5fbf78 d3io_Lod (int 2, bitfield),
  then a 0x100 buffer "animations/" (0x5fbf80, probably used by d3io3's animation loader), then the strings of
  0x44c610 / 0x44c9d0 (": Not all Textures..", "Error attaching Stock-Object", "stock object %s..",
  "d3_io:CurTexSet", ".bgf", "%s_%i"), then "d3_io_t:DeleteStockObject" (d3io2's). The "d3_io:" tag in
  0x44c610 is just another tag spelling, not a TU change. The TU continues past 0x44cd40 (d3io2).
- Names: d3_LoadScene44be60 / d3_LoadGroup44c3f0 (from unk_40d680.h), d3_Unk44c5d0 (d3_interface.h,
  int return) and d3_Unk44cbb0 (d3_engine.h, `int unk` is a D3SURF *) kept as other headers declare them.
- Shared headers: nothing changed or added. All new types and prototypes are in include/unk_44abb0.h
  (D3IOSTOCK stock object, D3IOSURF / D3IOTRI / D3IOVERT / D3IOPOLY / D3IOTEX views, D3IOLOD, D3IOSET (float
  view of d3if_Levels), D3IO_SCENE* macros, vfs_FindDir / vfs_GetPath / vfs_Tfopen / vfs_fclose with void *,
  d3_CloneObject, d3e_LoadSceneEvents).
- Noticed: err_Message is undeclared in unk_44abb0.c (implicit int, C4013; ReadFloor and LoadScene match
  with it). D3SRCPOLY (d3_engine.h) has a texture-set slot at 0x28 besides mat at 0x24; D3SURF+0x14 is the
  texture set (D3TEXTURE * per slot, "d3_io:CurTexSet"), +0x194/+0x195 bytes.

### VC6 notes
- d3_LoadScene44be60: an address-taken local (read with bio_rd_i) used as a loop counter stays in memory;
  the original reuses such locals as counters (x: fog mode and both set loops, n: the 6-fog loop and the
  object count). A separate register counter `k` took a register from the file pointer and reshuffled
  everything. A 4-byte gap in the frame was the read buffer as D3VECTOR4. One float temp for three reads
  (gray, fog start, fog end): the copy in the fog loop comes out as fld/fstp because no integer register is
  free there, the others as integer moves.
- Vector copies into globals field by field (struct assignment gives a pointer copy).
- d3io_LodName: three branches with their own `return 1` and one shared `return 0` at the end (an early
  `return 0` got its own epilogue); `int k = mode == 2 ? 2 - lod : lod;` for `mov eax,2; sub eax,ecx; jmp;
  mov eax,ecx` (assigning lod in place differs).
- d3_LoadGroup44c3f0: `if (x) {..} else` swapped to `if (version >= X) {loop} else first = rd_object(..)`
  for the block order.
- d3io_AttachStock: `tri->v[k] = &keys[idx]` through a block-local `D3IOVERT *v` fixed the address
  association `[edi+edx] + src` (writing it inline gives `src + edx + edi`).

### d3_LoadGroup44c3f0 (0x44c3f0, nonmatching, 4 bytes)
- Code and registers match; only the targets of `je` (in == 0) and `jne` (f != 0 in the version-mismatch
  branch) differ: the original jumps to the epilogue after d3_AddChild, ours to the one after the
  mismatch block's vfs_fclose.
- Tried (~25): `else if (!f) { fclose; return 0 / first; }` (right jumps, but first and f swap esi/edi; not
  fixed by declaration order, init placement, a real version local, unsigned version, nested ifs, block
  locals, `in = f ? f : open`, `if (!f) open else in = f`), `else { if (!f) fclose; }`, return inside the
  match block, reversed version test (with and without returns), goto to a close label.

### d3io_AttachStock (0x44c610, nonmatching, 1 instruction)
- `tex = (D3IOTEX *)d3_Textures + tri->src->tex` computes into ecx and copies to esi; the original lea's
  straight into esi. Tried: &d3_Textures[i], byte offsets, chained `tri->tex = tex = ..`, block-local tex,
  index local, colour key local, compare spellings, reversed if (worse). The `*(int *)&col = -1` init has
  no effect (VC6 stores -1 to the slot anyway) and was dropped.


## match-base 2026-10-24 (match worker)
- d3_Unk44c5d0 -> d3io_FindStock (the loaded stock object of that name, case-insensitive). d3_Unk44c9d0 was
  already d3io_LodName in this unit's header; the duplicate placeholder declarations in d3_io_t.h are gone.
  The global 0x5fbf78 is the already-named d3io_Lod (d3_io_t.h now has a matching D3IOLODBITS view because
  d3_int_io.h still collides on D3IOKEY).

## match-base 2026-10-25 (match worker)
- d3io_AttachStock -> match-deep (@deep in its address comment): readability done (Q4: D3IOCOL is a union
  `all` / `b, g, r, a`, D3IOVERT.col40 / col44 typed D3IOCOL, the D3IO_214 macro body is the typed field
  D3_X(o)->unk214; all byte-neutral, check 2567). Behaviour: one `code` hunk left, `add ecx,edx; mov esi,ecx`
  against `lea esi,[ecx+edx*1]` at 0x44c737 (the 44c9cf nop is the alignment after the 1-byte length
  difference). `uv run spell`: no menu rule for either hunk. New this session: the double-assignment spelling
  `tri->tex = tex = (D3IOTEX *)d3_Textures + tri->src->tex` is byte-identical (reverted); the older tries are
  in Open work above. The original computes the sum straight into esi (the value's home) where ours computes
  it into the dead index register ecx and copies - register allocation, match-deep.

## match-base 2026-10-26 (base-4 worker)
- **d3io_ReadFloor** behaviour + readable (Q7: the unit had no prototype for err_Message, and the three call
  sites were the only Q7 items).
- **d3_LoadGroup** behaviour + readable and **d3io_FindStock** byte + readable, by naming the globals their
  Q2 listed: d3_Unk6339e4 -> **d3_LodMissing** (0x6339e4, "set when an object LOD is missing", 4 uses) and
  d3_Unkfc2ffc / d3_Unkfc4a40 -> **d3_StockListHead / d3_StockListEnd** (0xfc2ffc / 0xfc4a40, the stock-object
  (mesh driver) list head and its end marker; d3_engine.h + d3_engine.c + this unit updated, byte-neutral;
  the merge picked the shard-owning d3_engine.c name over base-4's d3_MeshDrivers). d3_Unkfc4a40 keeps its
  d3_io_t.h alias d3_StockHead.
- **Merge 2026-10-26**: the `#include "err.h"` of the base-4 session was replaced by a local
  `void err_Message(char *text);` - the header's six declarations moved d3_LoadScene's allocation order by
  3 bytes at 0x44c0df (notes/vc6-matching.md), while the one prototype keeps Q7 closed.
- Open: rd_object (Q2: the D3OBJECT unk fields 0x05c/0x090/0x09c/0x19c/0x1a0/0x200/0x214, Q4 D3IO_200) and
  d3_LoadScene (Q2 d3if_Unk633dcc + the same object fields, Q4 raw casts) need the shared d3.h field renames.
