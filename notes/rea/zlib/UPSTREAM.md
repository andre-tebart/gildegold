# zlib 1.1.3 (upstream + one GildeGold change)

Library sources copied verbatim from `zlib-1.1.3.tar.gz`
(https://zlib.net/fossils/, sha256 `cae5847bc0e1cf113d3f70d037400da3e47c2e2b7b1c96b0b08447a5fbb906f4`).
Examples, contrib and platform makefiles are omitted. License: see `README` / `zlib.h`.

GildeGold.exe statically contains this zlib, compiled with the game's own flags (VC6 SP5 + PP, /O2).
Do not edit these files to make a function match; if the original deviates from upstream, record it in
`notes/` first.

## Deviations in the original (marked `GildeGold:` in the source)
- `zutil.c` `zcalloc` (0x468770): `malloc(items * size)` instead of `calloc(items, size)`.

All other linked functions match upstream byte-for-byte (see `build/state/names.csv`, the working state).
