# d3_io_a.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_io_a:" allocation tags
- Boundaries: separated (proven): tag neighbours
- Range then: 0x44e570..0x44ee40
- Was: unk_44e570.c (round 12)

## Open work
- see notes/units/d3_io_b.md, Open work: "d3 io leftovers after round 13 (two rounds without gain; only with a new idea; notes/units ..."

## Worker d3io3 (round 10): 0x44df40..0x44ee50 [d3io3]

### Report
- 31 functions in the scope: 8 Ghidra starts + 23 missed by Ghidra.
  - src/game/unk_44df40.c (6): d3io_LoadObjectFile 0x44df40 (".bgf" stock object, links it into the list at
    0xfc4c40), d3io_LoadObject 0x44e080 and d3io_LoadObjectRef 0x44e190 (file + its LOD variants, the second counts
    references), d3io_ReleaseObject 0x44e2c0, d3_LoadObjectAnim 0x44e2e0 (".oam" path animation, "d3_io:ObjAnim"),
    d3_LoadAnimStream 0x44e4a0 ("animations/" + name, calls d3_Unk44f8c0 outside the scope).
  - src/game/unk_44e570.c (25): the AGI / AGF text reader, line handlers filling a context (AGICTX, include/unk_44e570.h):
    agi_IsPow2 0x44e570, agi_CheckBitmap 0x44e5a0, agi_Bitmap0/1/2 (0x44e6f0 / 760 / 7d0), agi_Blend 0x44e840, agi_MatB1/2/4/8
    (0x44e8d0..0x44e990), agi_MatF0..F4 (0x44e9d0..0x44ead0), agi_Materials 0x44eb10, agi_PolyVerts 0x44eb60,
    agi_PolyUV 0x44ebf0, agi_PolyMat 0x44ec60, agi_EndObject 0x44ecb0, agi_Unk44ece0, agi_NumPoints 0x44ed00,
    agi_NumPolys 0x44ed50, agi_Points 0x44eda0, d3_Unk44ee40 0x44ee40 (empty).
- TU boundary: the "d3_io_t:" / "d3_io:" TU (unk_44cd40.c, d3io2) continues through 0x44e56e, so unk_44df40.c is its
  second half and the orchestrator joins them. A new TU starts at 0x44e570: its strings are "d3_io_a:AGI*Buffer",
  " is not square!", "ADD" / "MIX" (0x5fc63c..0x5fc6cc), and it continues past 0x44ee50 (worker scope end).
  The orchestrator may rename: agi_* are our names ("AGI" from the allocation tags).
- Ghidra misses: 0x44e570..0x44ed50 (23 handlers), all 16-byte aligned with nop padding. 0x44df20 (the callback
  stored at stock object +0x1f8) lies in d3io2's scope; I declare it as d3_Unk44df20.
- Shared headers: nothing added. unk_44df40.h and unk_44e570.h are self-contained (d3.h / d3_anim.h declare
  0x44e2e0 / 0x44e4a0 / 0x44e080 / 0x44ee40 with other types, so they are not included). For the join: d3io2's
  header will want D3IOOBJ (a D3OBJECT as the loaders see it: refs at +0x1dc, callbacks +0x1e8..0x1f8, list
  +0x1fc/0x200, data +0x204, type +0x218) and the prototypes of 0x44c530 / 0x44c5d0 / 0x44c9d0 / 0x44d1f0 / 0x44d330
  (d3_Unk<addr>, taken from the call sites).
- Globals named in unk_44df40.h: d3_Unk5fbf78 (int, low 7 bits == 1 loads the LOD files), d3_AnimDir 0x5fbf80
  ("animations/", a char array: strcpy is a loop), d3_StockList 0xfc4c40 / d3_StockHead 0xfc4a40, d3_AnimList 0xfe75a4 /
  d3_AnimHead 0xfe7440.

### VC6 lessons
- A value that is tested and then returned on the failure path (`t = f(x); if (t == 0 && (t = f(y), t == 0)) { ret = ...;
  return ret; } return t;`) needs two variables: with one, `ret` is allocated into a different register set (d3io_LoadObject,
  d3io_LoadObjectRef).
- A global read as `mov ecx,[g]; and ecx,0x7f; cmp cl,1` is an `int` global tested `(g & 0x7f) == 1` (no byte cast, no
  unsigned char global: those load a byte).
- `p->keys[n]` and `(*keys)[n]` (keys = &p->keys) schedule the pointer load differently; the former matched.
- The out-of-line block is the `else`: `if (p->flags.rev) {...} else {next = 1...}`; `if (obj != 0) {...} else if (keep) free`.
- Arguments are evaluated right to left, so `atof(strtok(util_ReadLine(f), " "))` pushes the separator before the
  ReadLine call; the original has the line in a local (`line = ReadLine(f); ... strtok(line, " ")`). Likewise the original
  evaluates `tok = strtok(...)` into a local before storing `-1` and calling atoi (agi_PolyMat, agi_PolyVerts).
- A dead 4-byte stack slot holding `and eax,0xff; mov [slot],eax` for a call result is `int ok = 0; if (path) ok = f(); if (!ok)`
  with ok otherwise unused (volatile also works).
- The order of stores `h = -1; w = -1;` follows source order, as for other locals; a three-register statement schedule
  (agi_EndObject) was found by trying the 6 legal orders of four statements (`polybase += npoly; npoly = 0; ptbase += npts;
  npts = 0`).
- Inlined `strcmp(tok, "MIX")` is `repz cmpsb` with the literal as the second operand (tok first).

## match-base 2026-10-24 (match worker, 5th)

- agi_Bitmap0/1/2, agi_Blend, agi_MatB1/B2/B4/B8, agi_MatF0..F4: Q6 descriptions added (text-format line
  handlers), all byte + readable.
- AGIPOLY.unk24 -> tex, from the binary twin: D3BGFPOLY.tex (0x24, "index into d3_Textures or -1") is set to
  -1 in bgi_PolyMat exactly like agi_PolyMat; agi_PolyMat byte + readable.
- agi_Unk44ece0 -> match-deep (Q1/Q2: no evidence for the handler name or AGICTX.unk04; bgi_Unk04 reads the
  same field and D3BGFDESC covers 0x04 with pad02).
- d3_Unk44ee40 -> match-deep (Q1 + canonical header: empty no-op callback assigned to d3_TriFuncs[0..5] and
  called from d2.c / cm.c with different signatures; declared under two names in cm.h, d2.h, d3_engine.h and
  four units).
- Unit: 25/25 byte; the two escalated functions are the only unreadable ones.
