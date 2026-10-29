# tb.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Also about this unit
- notes/units/txt.md, section [txt]

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "tb_OpenFunction"
- Boundaries: separated (proven): txt.c / tr.c tags
- Range then: 0x42e7b0..0x42ea50
- Was: unk_42e7b0.c (round 12)


## match-base 2026-10-23 (match worker)

- 0x6333d0 renamed `tb_AnimTick`: it counts once per 3 ms (`d3_Time % 3 == 0`) and is read by gfx.c
  (`d2_Unkf8a654 = tb_AnimTick`, an idle timestamp) and shp.c (`step = tb_AnimTick - d2_Unk6f0464`, the
  animation step), i.e. the animation clock; 6 references updated.
- 0x6333d4 stays `tb_Unk6333d4`: it is write-only in the whole exe (the only two references are its own
  `inc` in tb_TimerProc at 0x42e837/0x42e83d), so no role and no name; tb_TimerProc is escalated for that
  Q2 only (behaviour is byte).
- Descriptions added to tb_Exit, tb_CloseFunction, tb_SetFunctionOff, tb_IsFunctionOff.
- Unit: behaviour byte, readability: one function escalated (tb_TimerProc), the other six readable.
