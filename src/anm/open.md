# anm.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): animals: the list of 32 ANMREC (0x124 bytes), spawning dogs / cats / farm animals at dummies, their updates
- Boundaries: separated (plausible): all drafted (layout 2026-10-22 sessions 7-8). Tag "anm:animals" (mem_Alloc), leaked function names "anm_UpdateCat" / "anm_UpdateSheep" (copied into its actions). .data: "DOG".."HORSE" table 0x602688 (32-byte entries), "anm:animals", the model / dummy names, "dummy_%s", the update names, "bewegung/*" (ends 0x602851); .bss 0x6f0734..0x6f0740. End (session 8): the next literal "dummy_TUER" 0x602854 is shared by 0x4bc2e0 and the road code 0x4bcc20 (literals are not pooled across objs in this exe, so both are rd.c's); .bss: an unused slot 0x6f0744, then rd.c's 0x6f0748 (8-aligned). Start: see unk_4b6480.c (0x4bb720 / 0x4bb7d0 could be anm's)
- Range then: 0x4bb810..0x4bc2e0

## Open work
- All 13 functions match (match 2026-10-22, deep dive). Older attempt records for 0x4bb860 removed.
- Drafts far off: 0x4bb860 (the animal step; switch case order written as the case bodies' order), 0x4bbb80,
  anm_UpdateCat 0x4bc030 / anm_UpdateSheep 0x4bc1a0 (137 / 143 B: argument order of d3_GetHeightAt, loop shape).
- Type debt: anm.h repeats other units' declarations (notes/layout.md, session 7); it includes rd.h for the place
  helpers 0x4bc2e0 / 0x4bc390 / 0x4bc520 (rd.c since session 8).
- Boundary test (notes/layout.md, anm.c / rd.c): once anm_UpdateCat / anm_UpdateSheep match, check whether rd.c's
  0x4bc390 above them changes their bytes.

## match 2026-10-22, deep dive: anm_Unk4bb860 (166 -> 0)
- Root cause: our draft held the age value and the record in locals (`t = arr[idx].unk20 + 0x834; r = &arr[idx];`
  after the `if (d3_Time > arr[idx].unk20 + 0x834)` test). VC6 then reuses the test's value and address (CSE) and
  hoists r above the test. The original writes the expression out again inside the random test, twice, and stores
  through `arr[idx].unk0c`: `if (util_RandomF() > X / (d3_Time - X)) arr[idx].unk0c = 1;` (X = ANM_AGE_TIME). VC6
  keeps the loaded globals (array base in edi, index in eax) but reloads unk20 and recomputes the index arithmetic
  for an expression inside a statement that also contains a call; the loads still come before the call.
- How it was found: `uv run listing` showed which values VC6 shared; an insertion map (a store / call / volatile read at
  the block start, each probed in the listing) showed that only a call there gives the original's shape (reload +
  recompute, no hoisted r) and that no store does (VC6 has no type-based aliasing: a store into the record also
  drops the globals). A call that is not visible meant: the value is computed inside the call statement. Type
  views, casts, volatile, inverted tests, `&&` forms and an __inline helper with a t local all kept the sharing; the
  helper without t matched, and then the plain in-place form too.

## layout 2026-10-22 (session 7)
- ANMREC (0x124 bytes) x 32 at ai_Unk6f0738 (mem_Alloc 0x2480, "anm:animals"); ai_Unk6f0734 the count,
  ai_Unk6f073c the record updated next (one per step, `% 32`), ai_Unk6f0740 the last spawn try.
- Kinds: 0 dog "hund_HUND", 1 cat "katze_KATZE", 2 (birds?, 0x4bbb80 uses the building list), 3 cow "kuh_KUH" at
  "dummy_COW", 4 sheep "schaf_SCHAF" at "dummy_SHEEP", 6 pig "schwein_SCHWEIN", 7 horse "pferd_PFERD"; 0x4bbf00
  builds "dummy_%s" from the name table 0x602688 and reuses that buffer for the model name (strcpy inlined).
- The update functions copy their own name into the action record (+0x70): anm_UpdateCat (kinds 0 / 1),
  anm_UpdateSheep (kinds 3..7).

## layout 2026-10-22 (session 8)
- The unit ends at 0x4bc2e0: the helpers after anm_UpdateSheep (0x4bc2e0 free place at a building's "dummy_TUER",
  0x4bc390 points near a position, 0x4bc4b0 / 0x4bc520 a random object by name) share their literal with rd.c's
  road code, so they went to rd.c (see notes/tus.md).

## match-base 2026-10-24 (match worker)

- anm_Unk4bb810 -> anm_Alloc (allocates the "anm:animals" list; the first record's obj cleared instead of
  the raw `*(int *)list = 0`), anm_Unk4bb840 -> anm_Free. Renames ai_Unk6f0738 -> anm_Animals,
  ai_Unk6f0734 -> anm_NAnimals (only anm.c / anm.h use them).
- Next in the unit: the other 11 functions need their Q1 names; the shared callees anm_Unk4bbb00,
  anm_Unk4bbb50, anm_Unk4bbb80, anm_Unk4bbcd0.., anm_Unk4bc2e0 / 4bc390 / 4bc520 and the ai_Unk globals
  anm_Unk4bb860 uses should be named first (one rename helps several functions).

## match-base 2026-10-24 (2nd match worker, this session)

Raised: anm_NewAnimal (0x4bbb00), anm_RemoveAnimal (0x4bbb50) -> byte + readable. Renames (all
whole-word in src/ and include/): anm_Unk4bb860 -> anm_Update, anm_Unk4bbb00 -> anm_NewAnimal,
anm_Unk4bbb50 -> anm_RemoveAnimal, anm_Unk4bbb80 -> anm_UpdateBird (kind 2 = index 2 of the name table
"BIRD", the step's case 2), anm_Unk4bbcd0 / bbd60 / bbde0 / bbe70 / bbf00 -> anm_CreateCat / Dog / Sheep /
Cow / Animal (kind and model string of each), ai_Unk6f073c -> anm_NextAnimal, ai_Unk6f0740 -> anm_LastSpawn,
ai_Unk602688 -> anm_KindNames, ai_Unk4fe350 -> gm_ClockSeason (0x4fe350 is f3gm.c's gm_ClockSeason; the
reference was stale), ANMREC.unk08 -> nPoints, unk0c -> remove, unk20 -> birth. rd.c's helpers for the animals:
anm_Unk4bc2e0 -> rd_DoorPos, anm_Unk4bc390 -> rd_PointsNear, anm_Unk4bc4b0 -> rd_CollectNamed,
anm_Unk4bc520 -> rd_RandomNamedObject. sv.c's foreign `int ai_Unk4bb860();` declaration was renamed with it
(0x52d940 calls it); it still is a foreign declaration of anm's function (canonical header debt, sv.c should
include anm.h).

### Open work: Q4 blocks the other 10 functions
Only Q4 (raw `*(T *)` access on the animal object) remains for anm_CreateCat/Dog/Sheep/Cow/Animal,
anm_UpdateCat, anm_UpdateSheep; anm_UpdateBird has Q2 (ai_Unk501190/501030/502520/5024f0 = f3gm.c's
gm_BuildQuery / gm_BuildNext / gm_IsHomeType / gm_IsType10; anm.h cannot declare them under those names: lint
reports new cross-TU conflicts against f3gm.c / f3init.c because their return type is AIBLD*, not void*) and
Q4; anm_Update has Q2 (ai_Unk6f35c4) and Q4.

A typed-field fix was tried and reverted: anm.h cannot include d3.h / ch.h (the same conflict ch.c works
around, plus more). Build errors: d3.h(270) needs d3_TexFormat16 (not standalone), ch.h(115) repeats
d3_Unk44e4a0 with another level of indirection (ch.c #defines it away), ch.h(402) ch_Path, rd.h(93/96/109)
redefines d3_Unk633738 / d3sm_Create / d3_FindObject449690 with other types. The d3_DefaultScene view is no
longer one of them (lint baseline, 2026-10-25): anm.h declares `struct D3SCENE *` as d3.h / unk_58bbd0.h do,
and anm.c spells its two +0xb0 reads `*(char **)((char *)d3_DefaultScene + 0xb0)`; what still keeps d3.h/ch.h
out of anm.c is the list above.
Field names still needed before the create functions can be readable without casts: CHARACTER.unk1a0
(0x1a0, set to 1.0 / 1.5 on creation - a scale) and D3OBJECT.unk224 (set to 0; meaning unknown).
Remaining Q4 sites: create* 3 each (a->obj->unk1a0, a->obj->kind, a->obj->obj->unk224), UpdateCat/Sheep 2
each (CH_ACTIONS 0x128 test, d3_DefaultScene + 0xb0 / d3_GetHeightAt), UpdateBird 3 (building +0x61 object
and positions), anm_Update 3 (obj +0x34 -> +0x1ec -> +0x96d flags 0x40).
