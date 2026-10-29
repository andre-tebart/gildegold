# surf.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): surface scaling / pixel conversion (surf_*)
- Boundaries: separated (plausible): coherent surf_* (scaling / pixel conversion), no strings
- Range then: 0x41ebc0..0x4205c0
- Was: unk_41ebc0.c (round 12)

## Open work
- see notes/units/dd_vesa.md, Open work: "Header cleanup: Dd_vesa.c globals/functions are declared both in d3_d3d.h (with D3DCARD) a ..."

## Also about this unit
- notes/units/net.md, section [net]


## match-base 2026-10-23 (match worker)
- 0x420140 -> surf_ScaleBlit (behaviour + readable): dispatches by width to surf_Scale8 / surf_Shrink16 / surf_Shrink24 / surf_Down32, to a SURF_COPY row copy at equal widths and to surf_Up8 / surf_Up16 / surf_Up24 / surf_Up32 when dst is wider, keyed on the 8-bit palette flag and the RGB bit count; references in d3_d3d.c / d3_tools.c / d3.h renamed.
- surf_Down32, surf_Up16, surf_Up24, surf_Up32 escalated for behaviour: each transcription follows the Ghidra reading (the same shift counts and bounds, the p00 / p01 / p10 / p11 neighbours with the down-row and right-column guards, the accumulators and the output OR). The code hunks are frame / register / scheduling shape (ours 0x8c / 0x90 frames against the original's 0xb4 / 0xa8, the src parameter in ebp against esi, hoisted row bases and block layout). The binary really uses `lPitch >> 1` as the down-row offset in all four scalers (also in the byte-addressed Up24 and the dword-addressed Up32) - keep it. The original's channel shift counters are bytes (Ghidra bVar9..11), but `unsigned char rs, gs, bs` in surf_Down32 made the diff worse (302 -> 426 hunks), so it belongs with the shape work.


## match-base 2026-10-24 (match worker)
- surf_Pal8To16, surf_24To16: byte + readable (Q6 descriptions only).
- surf_Unk4205c0 -> surf_Blit, byte + readable: the palette flag was a named local assigned inside the `||`;
  the original re-evaluates `src->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8` in the else branch (VC6
  CSEs it into edi) and writes the conversions as an `else if` chain instead of a nested else + return.
  That spelling compiles to identical instructions (21 code hunks -> 0). The name: the equal-size surface
  blit (row copy, or 8-bit palette / 24-bit to 16-bit conversion) into a locked destination, the
  counterpart of surf_ScaleBlit.
- surf.c base queue empty; the four scalers stay @deep.

## match-base 2026-10-24 (base-4 worker, shard 4/5)
- Unit readability now: surf_Blit / surf_ScaleBlit were declared in d3.h; they moved to surf.h, and the two
  users (d3_d3d.c, d3_tools.c) include surf.h. No code changed (surf.c 12/12, both users unchanged).
