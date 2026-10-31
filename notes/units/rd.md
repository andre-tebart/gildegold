# rd.c (was unk_4bc2e0.c)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): the places of anm.c's animals (door dummies, free cells, dummy lookup), the vegetation map (64 plants of 0x18 bytes, growth, models "pfl_<type>_0<n>[_FALL].ogr"), roads from a building site to the next road (path_Find, DDA line, town map ground types)
- Boundaries: separated (plausible): all drafted (layout 2026-10-22 session 8). Tag: its error texts "rd_AllBauplatzToSupermap(): .." / "rd_GetBKSize(): ..". One obj by the shared literal "dummy_TUER" 0x602854 (0x4bc2e0 and 0x4bcc20). .data in function order: "dummy_TUER", the vegetation table 0x602860 ("TANNE", "BUCHE", "EICHE", 0x46-byte entries), "_ID%i", "init_SetPflanzenMap(): ..", "%svegetation/*pfl_..", "bk_", "PRUNK_WEG", "EINFACHER_WEG", the rd_ texts (ends 0x602a8c). End: rt.c's block starts with a -1 global 0x602a90. Start: no proof beyond the .bss gap 0x6f0744 (could still be one unit with anm.c)
- Range then: 0x4bc2e0..0x4bd950
- Was: unk_4bc2e0.c (layout 2026-10-22)

## Open work
- veg_LoadModel 0x4bc940 (match-base 2026-10-31 opus3, deep, 182 vs 180): left the byte-register choice in the height part (ours spills o from ebx around heights[z * size + x]).
- **headers (2026-10-30, typing pass 2)**: head.c includes anm.h, f3gm.h, d3.h, d3_light.h, data_views.h; no @original cast or view; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): util_RandomS.
- Near-misses: 0x4bc5a0 (53 B: the first load of the table id is a 32-bit `mov eax` in the original, ours `mov ax`;
  tried the index loop only), 0x4bc390 (116 B, not looked at).
- Far drafts: 0x4bc7c0, 0x4bc940 (FPU position from the height map), 0x4bcc20 (1760 B: road from the site, DDA line
  with __ftol; the loop shapes are guesses), 0x4bd340, 0x4bd500, 0x4bd760 (quad fill, two `(int)(sqrt(..) + 0.5) * 2`).
- .data not defined in C: the vegetation table ai_Unk602860 (G15TREE, "TANNE" ..).
- Type debt: rd.h's views of other units (notes/layout.md, session 8); 0x4bc2e0..0x4bc520 keep their anm_ names.

## layout 2026-10-22 (session 8)
- Matched first try or after one fix: 0x4bc2e0 (`int n` with `bs[(unsigned short)n++]`, the call written in both
  arms of `if (o != 0)`), 0x4bc4b0, 0x4bc520 (inline strcpy into a struct), 0x4bc5f0 / 640 / 670 / 6a0 (`x >= lo`
  compare order) / 710 / 8d0 / b90 / be0, 0x4bd4a0, 0x4bd610, 0x4bd6a0, 0x4bd710.
- G15PLANT (0x18 bytes, 64 per map): id, x / z cell bytes at 8 / 9, type at 0xa, size at 0xc, growth state at 0xd
  (-1 free), object at 0x14. G15TREE (0x46 bytes): id, name, size, states per model, last state.
- G15ROAD ai_Unkf81300: town map (G15MAP: w, cells at 0x14, origin 0x90 / 0x98, cell size 0xa0 / 0xb8, ground
  type names at 0x1a64), grid (d3sm_Create), the ground types "EINFACHER_WEG" / "PRUNK_WEG", path_Find's out
  (n / max / pts at 0x1014..) and two cell lists. Building sites: ai_Unkf812e4[ai_Unkf812e0] (0x60 bytes).

## match-base 2026-10-24 (match worker, 7th): rd_DoorPos naming leads (no edit made, timer)

rd_DoorPos (0x4bc2e0) is byte; only Q2 blocks it. Its five placeholder callees are already defined with
canonical names in other units:
- ai_Unk501190 -> `gm_BuildQuery` (f3gm.c 0x501190) and ai_Unk501030 -> `gm_BuildNext` (f3gm.c 0x501030), the
  building-query pair; ai.h/am.h/amt.h/anm.h declare them under the ai_Unk names (am.h has call-shape macros).
- ai_Unk502520 -> `gm_IsHomeType` (f3gm.c 0x502520), ai_Unk5024f0 -> `gm_IsType10` (f3gm.c 0x5024f0).
- d3_FindObject449690 -> the object-by-name/address finder of d3_interface.c 0x449690 (d3_FindObject... in
  d3_interface.c's comment; a `void *` view in other units).
The fix is either to call the gm_* names from rd.c (include f3gm.h; watch the AIBLD*/unsigned char* pointer
types and ai.h's foreign declarations) or to adopt the gm_* names project-wide. Q4 there: 4 raw `*(T *)` reads
(char * building at +0x61 / +0x4c view). Baseline saved, rd.c unchanged.

## match-base 2026-10-24 (base-1 worker)

- Unit base queue empty: every function is byte + readable, behaviour + readable, or escalated.
- Raised: rd_DoorPos (Q2/Q4 via f3gm.h + G15BLD; now byte + readable), rd_PointsNear (Q4 via G15GRID/G15SCENE,
  readable; behaviour escalated), veg_FindType / veg_SetPlant / veg_LoadModel / rd_BuildRoad / rd_PathToRoad /
  rd_AllBauplatzToSupermap / rd_FillQuad (readable, behaviour escalated), veg_FindPlantAt / veg_FindPlantObject /
  veg_FindPlantId / veg_FindPlantArea / veg_Fits / veg_GrowPlants / veg_ShowPlants / veg_HidePlants /
  rd_AddBauplaetze / rd_FindSiteCB / rd_GetBKSize / rd_WorldToCell (byte + readable).
- New include/f3gm.h: gm_BuildNext / gm_BuildQuery / gm_IsHomeType / gm_IsType10 (rd.c includes it; f3gm.c still
  declares them itself). rd.h gained G15BLD (pack 1: +0x61 is unaligned), G15OBJ (+0x224), G15SCENE (+0xb0).
- Q4 typed-field findings: AIBLD's +0x61 is the scene object (D3OBJECT.unk224 -> mask too); the vegetation fields
  are G15PLANT.size (0x0c), G15TREE.size (0x42) / states (0x43) / lastState (0x44).
- Open work: (a) rd.c's functions are still declared under ai_Unk names in ai.h (ai_Unk4bc5a0/670/710),
  cm.h (ai_Unk4bc5a0/5f0/710/7c0/940/bcc20), f3init.c (ai_Unk4bcb90), he.c (ai_Unk4bc390/670),
  unk_58bbd0.c (ai_Unk4bcbe0) - the unit header is not canonical until those callers include rd.h.
  (b) ai_Unkfd7f78 (AIMAPREF[0]+0x32c, only tested as bit 1 before d3fl_MakeDivide) has no evidenced name.
  (c) G15PLANT.unk10 (0x10) is only written -1 in veg_SetPlant and never read in the exe.
  (d) The hex offsets into d3_Terrain in veg_LoadModel / rd_FillQuad (m[0x28..], the sqrt quad fill) are the
  layout's guesses - match-deep work.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

rd.c includes <windows.h>, <d3d.h>, d3.h and d3_light.h (d3_light.h is not self-contained); rd.h lost its d3_RelightObjects declaration. No byte change.

## match-base 2026-10-31 (opus3)
- head includes d3_sm.h (typing commit, all 23 functions unchanged): D3TERRAIN for d3_Terrain.
- veg_LoadModel: pos = origin, then += x * axisx, += z * axisz, += height * axisy per component (each height
  re-read); one sprintf per branch with the literal format. The address comment had a cut-off @deep tail; cleaned.
