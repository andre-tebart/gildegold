# err.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "Error-Handler", <name>_error.log
- Boundaries: separated (plausible): "Error-Handler" tag; start next to qnt.c (plausible)
- Range then: 0x4214a0..0x422330
- Was: unk_4214a0.c (round 12)

## Open work
- err_ExceptionFilter 0x4215c0: ROUGH draft (layout 2026-10-23, 1888 B vs 1652 B): the control flow, the .dbg map file read (key = rotl 15 per dword), the exception name table and the stack walk are in; the codegen of the 20-case exception switch differs (the original shares the WriteFile tails; our ERR_EXC macro repeats them): try one `static` table of (code, name, len) or a `switch` that sets a `name` pointer and writes once.

## Also about this unit
- notes/units/net.md, section [net]
- notes/units/sblib.md, section [sblib]

## match-base 2026-10-24 (match worker)
- Raised to byte + readable: err_Write, err_WriteSymbol, err_GetString, err_IsCodeProtect, err_CheckCall
  (Q4: the E8 displacement read through a typed `int *rel`, bytes unchanged), err_Init (Q6, plus the rename
  below), err_Exit, err_Log.
- Rename: err_Unk63236c -> err_CrashCB, typed `void (*)(HANDLE)`; it is set by err_Init and called only by
  err_ExceptionFilter, with the crash log handle, or -1 when no log was written. The two casts there go.
- err_ExceptionFilter escalated to match-deep: the function really ends at 0x421c34 (the following bytes are
  its switch jump table) and our object is 0x3a bytes short, mainly because the original inlines the
  extension scan (forward to the NUL, back to '.', write "dbg": 421600-421647) where we call strrchr; the
  Open work lead above (one shared WriteFile tail) still stands; Q4 (the map key) and Q5 (ROUGH) remain.
