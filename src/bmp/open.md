# bmp.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "!bmp:" strings
- Boundaries: separated (plausible): own string block 0x5f1088.., "!bmp:" tag; neighbour udp.c a coherent udp_ group
- Range then: 0x401410..0x401b90
- Was: unk_401410.c (round 12)

## Open work
- see notes/units/dd_vesa.md, Open work: "Header cleanup: Dd_vesa.c globals/functions are declared both in d3_d3d.h (with D3DCARD) a ..."

## match 2026-10-23 (bmp_Load matched)
bmp_Load 0x4014d0 matches (34 B / 86 lines -> 0). Three source facts, found by reading the slot map, not by
variant rounds:
- The fill destination: the original stores it into the slot it shares with `x` (and with the flip buffer). A
  block-local macro temp (`void *_fd = (dst)`) never shared that slot whatever its order or type; assigning the
  destination to the function-level pointer `tmp` INSIDE the macro block does (`BMP_FILL(tmp, buf + y *
  bi.biWidth + x, rle[1], rle[0])`, macro `int _fn = (size); int _fv = (val); dvar = (dst);`). A plain
  `tmp = ...;` statement before the macro moves the store ahead of the temps (worse).
- The flip loop: each of the three copies in its own block with fresh `s` / `d` locals (`{ unsigned char *s =
  ...; BMP_COPY(tmp, s, pitch); }` ...) gives the original's slot pairs; with two function-wide `s, d` the
  second copy's destination took the `_fv` slot instead of its source. In the second block the source line
  is declared before the destination (the store order).
- The "unreferenced 4-byte slot" never existed: with the sharing right, `unsigned char rle[2]` sits at -0xe
  and `x` at -8 by itself; `rle[6]` had been compensating for the wrong sharing. The Open work entries about
  that slot (2026-10-22 .. 23, five sessions) are closed.
Unit: 3/3 matched, `uv run ledger grade bmp.c`: every function byte + readable.

## Worker start401 (round 3): 0x401000..0x4028a0 [start401]

### Report
No shared header was changed.

#### TUs found (new files)
| file | extent | contents |
|------|--------|----------|
| src/game/unk_401000.c + include/unk_401000.h | 0x401000..0x401410 | util_Hash (crc32, table 0x5eb380), util_BaseName, file_Exists, util_ReadLine (config line reader, buffer 0x630f50), bio_rd_*/bio_wr_* binary i/o on vfs files ("bio:" strings 0x5f1030) |
| src/game/unk_401410.c + include/unk_401410.h | 0x401410..0x401d50 | bmp_GetInfo, bmp_Load, bmp_Save24 ("!bmp:" strings 0x5f1084..) |
| src/game/unk_401d50.c + include/unk_401d50.h | 0x401d50..0x402140 | UDP broadcast (udp_Init/Exit/Send, socket 0x5f10f8 = -1) and udp_FindServers (LAN server scan) |
| src/game/unk_402140.c + include/unk_402140.h | 0x402140.. (continues past the scope end) | characters: ch_* (strings "ch_SetVisible(): invalid character", "ch_Create:..."; .data from 0x5f1120 with the animation path) |

Boundaries are inferred from the .data order (each TU's strings/initialized data follow the previous TU's)
and the string prefixes. The character TU continues at 0x4028a0 (next scope).

Names: util_Hash (0x401000), util_BaseName (0x401040), file_Exists (0x401090), bmp_GetInfo (0x401410) were
already declared under these names in ts_texture.c / d3.h, so names.csv uses them. 0x4014d0 is declared twice
elsewhere: `bmp_Load` (ts_texture.c, used in names.csv) and `util_LoadImage` (include/d3_d3d.h) - rename one
when cleaning headers. The bmp_Load flags: 1 load pixels, 2 palette, 8 24-bit output, 0x10 32-bit, 0x20 BGR.

#### Nonmatching (4)
- 0x4011a0 bio_rd_s: 6 asm lines (the loop test).
- 0x4014d0 bmp_Load: code matches; only stack slot assignment differs (frame 0x35c vs 0x360).
- 0x4022e0 ch_UpdateFades, 0x402480 ch_SetVisible: only the read-modify-write of the flag byte at CHARACTER+0x8c.

#### General VC6 lessons
- `unsigned char ret = 0; ... if (ok) { ...; ret = 1; } return ret;` gives the original's `xor bl,bl` at
  entry, `mov al,1` on the success path and `mov al,bl` on the failure path (bmp_GetInfo). With an explicit
  `return 1;`, VC6 propagates the 0 and never keeps ret in a register.
- `*(int *)&f = 0; f.b[2] &= 0xfe;` can be folded into a zero store to b[2]. That adds a use of constant 0,
  and VC6 then keeps 0 in ebx for the whole function. `memset(&f, 0, sizeof(f))` (an inline `mov [f],0`) keeps
  the `and` (ch_UpdateFades).
- A clamp compiled as `fcom 0.0; and eax,0x4100; je` followed by `fcom 1.0; test ah,5; jnp` is
  `a = max(a, 0.0f); a = min(a, 1.0f);` as two statements. The nested or ternary forms come out differently.
- `fild global; fisub [mem]` is `(float)global - intfield`. Without the cast the int subtraction comes first.
- A `found` flag (or `goto next`) for a duplicate check inside a loop gives the original's direct jump back to
  the loop condition. `if (i == n)` after the search loop adds a compare (udp_FindServers).
- `if (!a || b != c) f(x);` is laid out with one shared call. The original's duplicated argument pushes plus
  `jmp` to the call come from `if (a) { if (b != c) f(x); } else f(x);` (ch_UpdatePositions).
- A test split into two ifs, each with its own `{ vfs_fclose(f); goto fail; }`, keeps the fail block inline
  where the original has it. One `||` condition moves it out of line (bmp_Load).
- An inline-asm copy whose dst/src are loaded from per-copy temp slots, while plain variables (tmp, pitch) are
  named directly: the source names explicit pointer variables, e.g. `s = ...; COPY(tmp, s, pitch);
  d = ...; s = ...; COPY(d, s, pitch); d = ...; COPY(d, tmp, pitch);`. Macro-internal temps
  (`void *_cd = (dst)`) are never replaced by the variable, and `__inline` asm functions were not inlined.
- In the fill macro, the declaration order of the temps decides both their store order and their slots
  (bmp_Load: `int _fn = (size); int _fv = (val); void *_fd = (dst);`).
- Loops reading 3-byte pixels backwards into 4 bytes: three char temps `r = src[0]; g = src[1]; b = src[2];`
  reproduced the load order and the spill to a dead parameter byte. An `unsigned` counter tested with
  `count > 0` gives `test; jbe`.

#### Outside the scope
- 0x404e60 (called with a fresh height map when it belongs to the default scene), 0x402ba0, 0x403620 and
  0x404a80 are character functions in the same TU (prototypes in include/unk_402140.h).
- CHARACTER fields so far: 0x34 body object, 0x54 offset vector, 0x64/0x22c attached objects, 0x88 scene,
  0x8c flag bits (unsigned char bitfields: 0x01 position dirty, 0x20 hidden, 0x40 fading), 0x124 D3OBJECT**,
  0x128 animation (name[9] == '-'). ch_List (0x1468ae0) has 768 entries; ch_Fades (0x14eaa20) 16 x 12 bytes.
- D3SCENE has an int at 0x3d8 (inside pad3d6) and one at 0xb4 (inside pad0b4); unk_402140.c reads them through
  local casts, since shared declarations may not be changed.

### Nonmatching attempts
#### bio_rd_s 0x4011a0 (about 25 variants)
Original: `mov eax,[s]; dec eax; L: lea esi,[eax+1]; call; mov eax,esi; cmp byte [eax],0; jne L`.
Closest (kept): `s--; do { p = s + 1; bio_rd_c(f, p); s = p; } while (*s != 0);`. It differs only in the test:
ours `mov cl,[esi]; test cl,cl` and `mov eax,esi` after it. Also tried: the s--/s++ do-while,
post-increment, an index i, pre-increment inside the call, pointer copies, returning s, `(unsigned char)`
compares, for(;;) with break, and `s = p` before the call.

#### ch_SetVisible 0x402480 / ch_UpdateFades 0x4022e0 (flag byte at CHARACTER+0x8c)
The original clears or sets the flag with a direct `and/or BYTE PTR [esi+0x8c],imm`. Ours loads the byte early
and stores it later: in SetVisible the load is hoisted above `if (show)`, in UpdateFades it is scheduled before
an unrelated load. unsigned char bitfields fixed ch_UpdatePositions (the same flag byte).
Tried for SetVisible: if/else either way round, the call duplicated per branch, an early-return error path,
`hidden = !show` and a ternary (sete), mixed bitfield/byte access, switch, a volatile bitfield (keeps the RMW
split, worse), signed char bitfields and unsigned int flags (both worse).
Tried for UpdateFades: the statement order of the clear, and moving it into a comma expression in the call.
What did help in UpdateFades: `(float)ch_Time - start`, the max/min clamp, and memset for the flags.

#### bmp_Load 0x4014d0 (about 26 variants)
Fixed so far: shared fail return (`goto fail`), two separate format tests, r/g/b temps in the RGBA expansion,
src/dst pointer variables for the flip copies, and the declaration order of the fill temps. Left: stack
slots. The original frame is 4 bytes larger (0x360). rle sits at ebp-0xe (2-byte aligned) with 4 unused bytes
above it. x, the fill destination temp and the flip buffer share ebp-8. Ours has x at -0xc, rle at -8, and
everything between -0x10 and -0x58 sits 4 bytes higher.
Tried: rle as an unsigned short, as a struct, as int or char[6]; rle and x moved into inner blocks; a separate
swap temp; dead variables; declaration orders; separate flip-buffer variables.

## match-base session 2026-10-25 (base-3 worker, shard 3/6, side work)

include/bmp.h dropped the foreign qnt_Quantize declaration (qnt.h owns it); bmp.c includes qnt.h. bmp.c
unchanged (3/3 byte, 0 differing bytes).
