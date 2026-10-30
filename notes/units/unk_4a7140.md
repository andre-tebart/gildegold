# unk_4a7140.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (-): layout 2026-10-22: tavern poker 0x4a7140..0x4a86f0, leisure actions (decide / check / dispatch 0x4a8760..0x4aa430), aip_ actions, actions 0x44..0x47
- Boundaries: separated (plausible): own .data block 0x600490..0x600f60 in function order (poker hand names 0x600490.., NPC models 0x600d90 (0x4a7140), action tables 0x600e10 / 0x600e5c (0x4a9a00 / 0x4aa120), "prva_*" / "NPC %s <%i> wählt %s" (0x4aa430), "aip_Exec*" (0x4aa940..), "buy dunkle ecke" (0x4ab2c0), "Ehrerbietung" .. "Blutrache" (registration 0x4abed0)); starts after the am unit's block, ends with the registration 0x4abed0 (next: .rdata table 0x5ed1e8 helpers, then "hmt_" 0x4ac440). Mixed content (poker game + leisure / private AI actions), one block, no contrary evidence; possible split at 0x4aa580 (callbacks with 3 parameters, "aip_" strings)
- Range then: 0x4a7140..0x4ac140

## Open work
- Boundary: possible second unit from 0x4aa580 (3-parameter callbacks, "aip_Exec*"); codegen is neutral (layout
  2026-10-22: with 0x4a7140..0x4aa580 compiled out, no function from 0x4aa580 changes), so only the .data block (one)
  and the subsystem guess speak; left as one file.
- Near-misses: 0x4a7430 77 B (the seat loop: the original's base pointer is &seat->unk1c with the card stores
  through `lea ecx,[esi-0x14]`; index loop for the deck fixed the rest), 0x4a9150 21 B (p->unk004 is loaded after
  the ai_Time % 4; both operand orders tried), 0x4aa940 47 B (AITIME copy into msg.arg[0]: the original interleaves
  the time loads with the other stores; field-by-field copy was worse, 87 B), 0x4abc60 10 B, 0x4abd00 13 B, 0x4aaae0
  85 B (AIMSG store order), 0x4aafb0 160 / 208 B (the original clears the message with AI_ZERO, am.h).
- Rough drafts: 0x4aab90 (its middle part, the `*k` comparison, is guessed), 0x4a8190 728 B;
  0x4a7f50 491 / 560, 0x4a77e0 / 0x4a7ac0 / 0x4a7c20 (count loops, pointer shapes).
- Same-size drafts worth a matching round first: 0x4a7140 (752), 0x4a74e0 (368), 0x4a9a00 (1440), 0x4aa120 (784),
  0x4aa430 (336), 0x4ab3e0 (720), 0x4ab2c0 (288).

## layout 2026-10-22 (session 3)
- Unit start 0x4a7140: the am .data block ends with "%sgebaeude/*%s_%c.ogr" 0x600478; 0x600490 starts this unit's
  block (char[256] poker hand names "Naechste Karte", "Paar" .. "Royal Flush", unreferenced directly), then the NPC
  models 0x600d90 (0x4a7140), the action tables 0x600e10 (19 ids, 0x4a9a00) / 0x600e5c (3 ids, 0x4aa120), "prva_*" and
  "NPC %s <%i> w\xe4hlt %s" (0x4aa430, sprintf'd and never used), "aip_ExecPamphlet" / "aip_ExecDrohen" /
  "aip_ExecAnschwaerzen", "buy dunkle ecke", "Ehrerbietung" / "Theater" / "Verfluchen" / "Blutrache". The hits of
  0x600e5c / 0x600e68 at 0x4a9ab8 / 0x4aa1d4 are table-end compares, not string uses.
- Poker (0x4a7140..0x4a86f0): game record G10GAME at *0xf83654 (0xc0 bytes: phase, three G10SEAT of 0x34 from +4,
  seat 0 = the player AIPER, seats 1/2 = G10NPC at 0xf837e0), deck G10CARD[24] at 0xf83660 (group 0..5, rank 0..3,
  owner: seat, 3 = deck, 4 = discarded), 0x4a74e0 is the phase machine, 0x4a7f50 rates a hand (0..8 = the hand names),
  0x4a8190 the showdown.
- Leisure actions: 0x4a9a00 (table 0x600e10) / 0x4aa120 (0x600e5c) pick an action and call its decide callback
  with (a, p, ctx[0x88], in, m); ctx[0] = 1 alone / 2 with partner, ctx[0x87] = the partner from job list 0x45;
  0x4a9fa0 / 0x4aa430 are the matching checks (0x4f81c0 & 0x60).
- Actions 0x44..0x47 (registration 0x4abed0, matched): decide 0x4ab3e0 / 0x4ab7f0 / 0x4abac0 / 0x4abd00, execute
  0x4ab6b0 / 0x4ab9f0 / 0x4abc60 / 0x4abe60. 0x4abd00 returns 0x45 (sic). Ghidra had none of these starts.
- 0x4a8c10 (`return 1`) is shared (ICF) with d3_Unk4a8c10 callers; it sits here, so this unit owns it.
- Matched by statement order: 0x4aaa10 / 0x4ab9f0 (AIMSG field stores in source order), 0x4abed0 (act.id before the
  strcpy of the name), 0x4ab7f0 (`k = r > 1 ? r : 1;` computed between the stores of m + 4 and m + 0xc).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x4f2720 calls renamed ai_Unk4f2720 -> gv_GetTableRecord; byte unchanged.
