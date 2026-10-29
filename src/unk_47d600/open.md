# unk_47d600.c

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (-): inline-asm shape drawing loops (8 bit shapes) and the darkening blit 0x47d960 (moved from unk_47d960.c); a TU of its own (0xcc padding before 0x47d600, AI code from 0x47d9e0)
- Boundaries: separated (proven): 0xcc padding before 0x47d600; AI unit from 0x47d9e0 (strings).
  Correction 2026-10-23: the 0xcc before 0x47d600 is the linker's fill after d2_Unk47d210's unpadded section (shp.c, `#pragma
  optimize("", off)`), not an object end: the start boundary is `plausible` (subject: the 8 bit blitters), the end stays proven.
- Range then: 0x47d600..0x47d960

## Open work
- resolved 2026-10-23: d2_Unk47d710 matches with `#pragma optimize("", off)` around it (the original section has no alignment pad; the linker's 0xcc follows); its dead variant blocks are inside the matched section.

## match-base 2026-10-23
- The three blitters are named and readable: 0x47d600 d2_ShpBlit8 (runs of an 8 bit shape into an 8 bit
  surface), 0x47d710 d2_ShpBlit8To16 (into a 16 bit surface: mode 0 palette d2_ShpPalTable, mode 1 the light
  table d2_ShpLightTable - the 0x10000-entry dd_RGB table d2_Unk479d00 builds with `add` - modes 2 / 3 half
  dark; 1 and 3 are reachable only through the original's dead `je +0`, kept verbatim), 0x47d960
  d2_ShpDarken16 (half brightness of w x h 16 bit pixels).
- Globals named from the reading: d2_ShpW / d2_ShpH (the shape's s->w / s->h), d2_ClipTop / d2_ClipBottom (the
  clip rect's y0 / y1, set with x0 / x1 by shp.c 1502-1505, initial value = screen size), d2_SurfPitch
  (surf->pitch; doubled where the surface is 16 bit), d2_ShpRows (rows left) / d2_ShpDataOfs (offset of the
  first visible line's runs, 0x32 = sizeof(D2SHP), s->lineoff is the table of line offsets), d2_ShpRowGap
  ((pitch - w) * 2), d2_ShpPalTable (256 words, no writer in this build), d2_ShpLightTable, d2_BlitW (w saved
  by d2_Unk47ced0).
- Run format: per line a dword run count, per run dword skip, dword byte count, then the pixels (an odd first
  pixel, then words).
