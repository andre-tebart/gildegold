# unzip 0.15 beta (Gilles Vollant), from zlib 1.1.3 contrib/minizip

`unzip.c`, `unzip.h`, `ChangeLogUnzip`, `readme.txt` copied from `contrib/minizip` of the same
`zlib-1.1.3.tar.gz` as `src/zlib` (sha256 `cae5847bc0e1cf113d3f70d037400da3e47c2e2b7b1c96b0b08447a5fbb906f4`).
`zip.c` is not linked into the game and is omitted.

## Deviations in the original (marked `GildeGold:` in the source)
- Two functions appended to `unzip.c`, not in upstream: `unzGetFilePos` (0x4686d0) and
  `unzGoToFilePos` (0x468710). They save/restore the current file's central-directory position,
  like the later minizip API of the same name. The names are ours; prototypes added to `unzip.h`.
