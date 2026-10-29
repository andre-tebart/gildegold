# minizip (library code, one note for every file of src/minizip)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below.

## Unit
- Name (leaked): the public minizip sources; function names are the library's.
- Boundaries: proven for every file: uv run link's LINK.EXE probe placed all 85 zlib / unzip functions at the
  original offsets in the original object order (notes/link.md); every function matches.
- Files: unzip.c

## Open work
- none

## match-base 2026-10-23
- Q6 descriptions for unzip.c unzlocal_getLong (4-byte LE, *pX = 0 on error), unzlocal_GetCurrentFileInfoInternal
  (central record + name / extra / comment, UNZ_BADZIPFILE on a bad magic) and unzGoToFilePos (the pair of the
  GildeGold-only unzGetFilePos, same TU addition 0.15 does not have). byte + readable.
