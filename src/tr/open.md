# tr.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "tr_startup"
- Boundaries: separated (proven): tb.c / vfs.c tags
- Range then: 0x42ea90..0x42ec90
- Was: unk_42ea90.c (round 12)

## Open work
- util_Random: ai.h, ch.h and tr.h disagree on its prototype (hv3).
- unk_402140.h declares `unsigned short util_Random(int)`; it is `int util_Random(unsigned short n)`
  (unk_42ea90.h; tr.h has the int form, ch.h the other; ai.h follows ch.h because the AI callers use the result
  as unsigned short, round 19). d3_engine.h's d3_Unkfe7a88 is tb_Delay. Build logs: grep C4013 (undeclared calls).

## Also about this unit
- notes/units/txt.md, section [txt]

## match-base 2026-10-23
- tr_shutdown frees tr_Table and clears tr_Table / tr_First / tr_End, `int 3` when no table is open or it lies
  below tr_First (an assertion of the original). util_Randomize seeds the C runtime generator with
  timeGetTime() in ms. Both byte + readable.
