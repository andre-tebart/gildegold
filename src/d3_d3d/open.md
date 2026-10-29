# d3_d3d.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\Units\d3\d3_d3d.c
- Boundaries: separated (proven): leaked neighbours
- Range then: 0x4359f0..0x4387b0

## Open work
- match 2026-10-23 (5th): d3d_LoadTexture frame layout: the original (low to high) msg -0xd2c, namebuf -0xb2c, adw -0x92c, pal -0x52c, tempddsd -0x22c; ours adw -0xd2c, pal -0x92c, msg -0x62c, namebuf -0x42c, tempddsd -0x22c. Declaration order (500 perms of the first seven locals) and early dead stores to pal[0] / adw[0] / msg[0] do not change the map (uv run listing shows it); the first array use is the BMP_ZERO of pal in both.
- match 2026-10-23: d3d_LoadTexture: the hunks of class `slots` swap [+0x1c] <-> [+0x20]: the original stores the 0x7c (dwSize) of a block-scoped DDSURFACEDESC2 into the w slot [ebp+0x1c] (w is the dead parameter it reuses), ours into [ebp+0x20] (h). Swapping the roles of w and h in the body cuts 210 -> 170 lines but makes the first scaling code load [ebp+0x20] first (the original loads [ebp+0x1c] first), so it is not kept; the real lead: which parameter dies first / has fewer references decides the reused slot.
- see notes/units/dd_vesa.md, Open work: "Header cleanup: Dd_vesa.c globals/functions are declared both in d3_d3d.h (with D3DCARD) a ..."

## Worker d3_d3d (range 0x434a80..0x439000) [d3_d3d]

37 functions, three translation units:

| TU | range | evidence |
|----|-------|----------|
| d3_anim.c (tail) | ..0x4359f0 | alloc tags "d3_anim:CalculateAnimNormals(...)"; 0x435950 needs 0x4324e0 in the same TU (see below), so the file starts at or before 0x4324e0 |
| d3_d3d.c | 0x4359f0..0x438c30 | assert path refs 0x435cd1..0x438bfa; 0x4359f0 is the EnumTextureFormats callback used by 0x435c50 |
| d3_engine.c (head) | 0x438c30.. | d3_d3d.c's .data ends at 0x5fb5af ("...ZBIAS, 0)" string); the next TU's .data starts with d3_DefaultScene (0x5fb5b0) followed by the "d3_engine.c" path (0x5fb5c8); 0x438d40/0x438e60 use d3_DefaultScene |

### Nonmatching

#### d3anim_SetFrame (0x435950)
The original keeps the slot pointer in edx across the call to 0x4324e0. VC6 knows the register usage of
functions *defined earlier in the same file*: with a definition of d3_Unk4324e0 above SetFrame the function
matches (verified with `uv run try`). 0x4324e0 is outside this range, so it is not added here. Its body:

```c
int d3_Unk4324e0(D3ANIMFLAGS f, int frame, int a, int b, int c)
{
    if (f.b[1] & 2) {
        if (frame > a)
            return frame - 1;
        return a + 1;
    }
    if (frame >= b) {
        if (f.b[1] & 0x10) {
            if (frame + 1 <= c - 1)
                return frame + 1;
            return c - 1;
        }
        if (f.b[1] & 1)
            return frame - 1;
        return a;
    }
    return frame + 1;
}
```
(not byte-checked itself.)

#### d3anim_CalculateAnimNormals (0x434d60), 6 bytes
Only the bbox copy loop: the original addresses `a->frames[f].min.x` as [frames+off] (base esi), ours as
[off+frames]. Tried: index[array], (a->frames + f)->, frame pointer local, D3VECTOR struct copies, ++f.
The min/max tracking needed `mn.x = min(mn.x, p.x)` (macro, fld/fstp copy), chained `mn.z = mn.y = mn.x`,
and x,y,z store order.

#### d3d_LoadTexture (0x437390)
Code and control flow follow the original (same size class, ~3.5 KB); the remaining difference is the
stack slot assignment of the big locals (original low->high: msg, namebuf, adw, pal, tempddsd, dest, ddsd,
ddscaps2, num, loadddsd; ours puts namebuf/msg above pal/adw and loadddsd below ddscaps2/num) and w vs h
in esi. Declaration order (6 permutations) and block-scoping of msg/namebuf do not change it.
Block-scoped stretcht/tempt fixed the scalar slots (both share the dead h parameter slot in the original).

#### d3_ProjectMesh (0x438f60), 10 bytes
FPU operand order only: dist^2 computes y*y before x*x in the original, and the back-face cross product
computes (v0.sx - v1.sx) first. Source order, parentheses and local-pointer order don't move either.

### Findings (VC6)
- Interprocedural register knowledge: a call to a function defined earlier in the same TU lets VC6 keep
  values in caller-saved registers that the callee does not touch (d3anim_SetFrame/0x4324e0). Useful as
  TU-boundary evidence.
- `if (c) D3DCHECK(a); else D3DCHECK(b);` without braces lays the blocks out differently from the braced
  form; the original used braces (and the two calls share the d3d_Error call when on different lines).
- Bitfield assignment `x.bit = on` gives `add cl,cl; xor; and 2; xor`; a manual xor/and formula is close but
  needs the object pointer in a local to get the same register choice.
- Self tail recursion `d3anim_Release(a, 1)` at the end is turned into a loop (param overwritten in place).
- `ddsd.dwHeight = ddsd.dwWidth = v` in an if/else-if chain: VC6 merges the last two branches; separate
  statements `ddsd.dwWidth = v; ddsd.dwHeight = v;` per branch keep them apart like the original.
- Chained assignments store right to left (`a = b = c = v` stores c first).
- The inline-asm copy (D3_COPY*) exists in four operand shapes (direct/temps); the temps are declared in
  the order size, src, dst, and VC6 does not substitute a plain variable into the asm.
- Loop-invariant temps of the copy macro are hoisted out of a loop into dead parameter slots.


## match-base 2026-10-24 (match worker)
- d3d_FindTextureFormats: byte + readable (Q6 description).
- d3d_Init: byte + readable. Renames: d3d_Unk633434 -> d3d_Mipmaps (d3d_UsedCard.info.maxMipFilter > 1),
  d3d_Unkfe7608 -> d3d_ZBufferCaps (0x800 system memory / 0x6000 video memory | 3D device),
  D3DCARDINFO.unk28 -> maxMipFilter (the card's highest D3DTSS_MIPFILTER, capped with the config's at
  0x4366xx/0x4366xx), D3DCARDINFO.unk00 -> caps (0x04 perspective, 0x10 bilinear, 0x20 z buffer,
  0x40 antialias, 0x80 dither).
- d3d_CreateDevice: byte + readable. d3d_Unk5f942c -> d3d_MinMipSize (4, the smallest mip dimension kept),
  d3d_Unk63342c -> d3d_TexShift (the width/height shift, from d3_Cfg.unk3c; the duplicate extern in
  d3_engine.h updated).
- d3d_Unk4370d0 -> d3d_Restore, byte + readable: a two-byte stub (`mov al,1; ret`) whose only caller is
  main.c's WM_ACTIVATEAPP restore loop after dd_Restore; it always reports success.
- d3d_LoadTexture -> match-deep: the 197 code hunks are shape - the loadddsd/pal/adw stack-slot cascade
  (loadddsd at [ebp-0xb8] vs [ebp-0x9c], its +0x48/+0x68 field stores move with it), the dead-parameter
  reuse of the w/h slots ([+0x1c] vs [+0x20], flipping between the level loop and the i loop), operand
  order [esi+eax]/[eax+esi] in the name copy, moved stores, tail nops; the w != ddsd.dwHeight quirk is the
  original's. Q3 fixed with the D3D_RGB24_* defines (the 0xff0000 mask was not an address).
- d3_d3d.c base queue empty.
