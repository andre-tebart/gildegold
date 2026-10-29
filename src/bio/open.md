# bio.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "bio:" strings (bio_rd_array_quick)
- Boundaries: separated (proven): start of game code; bmp.c tag
- Range then: 0x401000..0x401380
- Was: unk_401000.c (round 12)

## Open work
- bio_rd_s 0x4011a0 (11/48 bytes, 6 asm lines, stuck): the original tests the byte through s (`mov eax,esi` before
  `add esp,8`, then `cmp BYTE PTR [eax],0`); VC6 propagates p and loads `mov cl,[esi]` / `test cl,cl` before the
  `add esp`. Same bytes for: `while (*s)`, `*p`, `s[0]`, `'\x00'`, `!(*s == 0)`, unsigned char cast, volatile cast,
  `for (;;) break`, `&& 1`, `s++` / `++s` / `s = s + 1` instead of `p` (those give a different, shorter inc loop),
  `char *` return with `return s`, a second pointer `q = p`.
- see notes/units/dd_vesa.md, Open work: "Header cleanup: Dd_vesa.c globals/functions are declared both in d3_d3d.h (with D3DCARD) a ..."

## Also about this unit
- notes/units/bmp.md, section [start401]

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- bio_rd_s (0x4011a0) -> match-deep: still 11/48 bytes, 6 asm lines. The only hunks are the loop test: the
  original `mov eax,esi; add esp,8; cmp BYTE PTR [eax],0`, ours `mov cl,[esi]; add esp,8; test cl,cl; mov eax,esi`
  (VC6 CSEs `*s` to `*p` and loads before the stack cleanup). `uv run spell`: no menu rule on either hunk. Tried this
  session (one edit, one build each, all canonicalise): `while (*(s = p))`, `bio_rd_c(f, ++s)`,
  `bio_rd_c(f, s = s + 1)`, `s = p` before the call. The earlier list (Open work above) stays valid. Left is the
  CSE / scheduling choice; no new meaning lead.
