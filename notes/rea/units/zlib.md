# zlib (library code, one note for every file of src/zlib)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below.

## Unit
- Name (leaked): the public zlib sources; function names are the library's.
- Boundaries: proven for every file: uv run link's LINK.EXE probe placed all 85 zlib / unzip functions at the
  original offsets in the original object order (notes/link.md); every function matches.
- Files: adler32.c, crc32.c, deflate.c, gzio.c, infblock.c, infcodes.c, inffast.c, inflate.c, inftrees.c, infutil.c, trees.c, zutil.c

## Open work
- none

## match-base 2026-10-23
- deflate.c longest_match: the UNALIGNED_OK blocks are MSDOS only (zconf.h defines it only under MSDOS) and are
  not compiled here; they were removed (byte path kept verbatim) and the address comment describes the function.
  `uv run gen` does not preprocess, so dead #ifdef branches still count for Q2 / Q4.
- Q6 descriptions for infblock.c (inflate_blocks_new / inflate_blocks / inflate_blocks_free), infcodes.c
  (inflate_codes_new / inflate_codes / inflate_codes_free), inftrees.c (inflate_trees_bits / _dynamic / _fixed)
  and zutil.c (zcalloc = malloc not upstream calloc, zcfree). All byte + readable.
