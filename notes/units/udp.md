# udp.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Also about this unit
- notes/units/bmp.md, section [start401]

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): UDP broadcast / server scan (udp_*)
- Boundaries: separated (plausible): coherent udp_* group (UDP broadcast / server scan), own string block 0x5f10f8..
- Range then: 0x401d50..0x401ea0
- Was: unk_401d50.c (round 12)


## match-base 2026-10-23
- udp_Exit / udp_Send descriptions (Q6); udp_Init and udp_FindServers already had theirs. 4/4 byte + readable:
  udp_Socket (0x5f10f8) is a static SOCKET, udp_Bcast / udp_Local the two sockaddr_in (0x14eaae0 / 0x14eaaf0).
