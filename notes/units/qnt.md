# qnt.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Also about this unit
- notes/units/net.md, section [net]

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): octree colour quantizer (qnt_*)
- Boundaries: separated (plausible): coherent qnt_* octree quantizer, no strings
- Range then: 0x4206d0..0x421410
- Was: unk_4206d0.c (round 12)

## Open work
- match 2026-10-23 (8th), no gain: qnt_InitTables (92 B / 35 lines): 125 type casts on the three first terms (none / uchar / char / short / ushort each), 125 local temps `p = i & 0x80` etc. of 5 types, 1024 variants of `u = i` (uchar / char) used in place of i in the 8 terms: nothing under 35 lines (the original narrows the 0x80 / 0x40 / 0x20 terms and the `>> 1 & 4` term to byte moves and keeps `a` in esi).
- match 2026-10-23 (5th): qnt_Map 822 B / 139 lines -> 318 B / 87 lines. What did it: (1) the three clamp-table stores as `cl[k] = k; clamp[k + 512] = 255; cl[k - 256] = 0;` (563 B; `cl[k - 256]` is clamp[k]; other orders / spellings 685-744 B), (2) `cnt = w * 3 + 6` as a local after `flip = 0` and used in both err mallocs and the zero loop (the original hoists `lea edi,[ebx+ebx*2+6]` above malloc(0x300); 524 B), (3) the error diffusion weights as an add chain with a second local `e2`: `e2 = er + er; er += e2; cur[6] += er; er += e2; cur[3] += er; e2 += er; prev[0] += e2;` (the products er * 3 / 5 / 7 compile to lea / imul forms; 318 B). Left (13 hunks): clamp / cl are in ebp / esi swapped vs the original (esi / ebp), and the first loop's IV is the clamp family (original: cl family, `[eax]=cl[k]`, `[edx+eax]` / `[edi+eax]` for clamp[k] / clamp[k+512] with edx = clamp - cl; tried: all 6 orders x 3 spellings per store x cl as char* / decl order); in the main loop the original CSEs `lim + 0x100` (`lea edx,[eax+0x100]` before the first movsx, which itself reads `[ebx+eax+0x100]`; g / b use `[..+edx]`): spellings `(lim + 256)[x]`, `lim[256 + x]`, `*(lim + 256 + x)` x 3 channels (216 variants) are canonicalised, and perms of the er / eg / eb lines are far worse; swapping `cur = ..` / `prev = ..` in the flip arms (the original assigns prev first) is far worse (864 B); `[eax+ecx+0xc]` vs ours `[ecx+eax+0xc]` in the `(off + (char *)qnt_Level[5])->count` test: 28 spellings, no change. qnt_InitTables (92 B / 35 lines): casts (signed char) / (short) / (unsigned short) on the 0x20 term and cast type permutations of the three terms (53 variants): no gain. qnt_Build (7 B): dead stores of lvl / idx / j and spellings of the `qnt_Level[lvl][idx]` accesses (800 sampled): no change. qnt_Nearest: dead stores `g = 0` / `b = 0` / `g = b` and perms of the three clamps: no gain.
- match 2026-10-23 (4th): qnt_MakePalette MATCHED by dropping the `half` local: `(qnt_Level[lvl][idx].r + (c >> 1)) / c`, `((c >> 1) + g) / c`, `((c >> 1) + b) / c` (the `half` variable had priority 108 > c 90 and took ecx; the original has c in ecx and the shift as a CSE temp). qnt_InitTables 122 B / 70 lines -> 92 B / 35 lines: `(unsigned char)i & 0x80` in the first term of `a` gives the original's byte move (`mov cl,al`), and `a2 = a << 2` shared by the r and g tables gives the original's ebp CSE / register set. Left: the original also narrows the 0x40 and 0x20 terms (`mov dl,al`) and keeps a in esi (`mov esi,ecx; or esi,edx`); casts on those terms (`(unsigned char)i & K`, `(unsigned char)(K & i)`) make it far worse (155 B / 90 lines: the cast value is shared as `and edx,0xff`). qnt_Nearest 2 B: g / b register swap really is the term order (R, B, G loads) which VC6 does not follow from the source order; temps per term, parenthesisations, all 6 orders, and a different reference order in qnt_GetPalette: no change. qnt_MakePalette MATCHED (see match 2026-10-23 (4th) below), qnt_InitTables now 92 B / 35 lines (a `(char)i & 0x40` cast on the second term too).
- match 2026-10-23: qnt_Nearest (2 B): VC6's own listing already gives the original's registers (r esi, g edi, b ebp); the difference is only the term ORDER of the sum (original loads PalR, PalB, PalG; ours PalR, PalG, PalB whatever the source order: 24 statement perms x 6 term orders, int params, rr/gg/bb copies, declaration order, extern order of qnt_PalG / qnt_PalB in qnt.h: no change). qnt_MakePalette: the original loads count first (`mov ecx,[+0xc]`, `mov esi,ecx` for half), then g, b, r; perms of the c/g/b/half statements and a QNODE *n local (4 forms): worse.
- match 2026-10-22 (2nd pass), no gain: qnt_Build (7 B, `[eax+ecx+0x10]` base / index swap in qnt_Level[lvl][idx].total/mask): `QNODE *nd = &qnt_Level[lvl][idx]`, `qnt_Level[lvl] + idx`, `nd[idx]`, byte-offset form: no change or 290 B worse.
- match 2026-10-22: qnt_Nearest (2 B): the r / b copies sit in ebp / edi swapped; the three `x = (x & 0xf8) + 4` orders and the six term orders of d: no change.

## match-base 2026-10-23
- Escalated to match-deep (behaviour; the code hunks are register / width / addressing spellings, no meaning
  error): qnt_InitTables (72 hunks: the original narrows the 0x20 / 0x40 terms to mov dl,al / mov cl,al and
  keeps a in esi; the cast and u variants are already listed above), qnt_Build (14 hunks: base / index swaps in
  the qnt_Level[lvl][idx] addressing, mov esi,eax before vs after pop ebp, test esi vs test eax) and qnt_Map
  (147 hunks: the clamp loop's three stores run with the 0x200 folded into one base, [eax+ecx+0xc] vs
  [ecx+eax+0xc], the palette loops' register assignment).
- Escalated to match-deep (Q2): qnt_Free - qnt_Unk632364 (0x632364, a void * next to qnt_Heap) is never
  written in the binary: a scan of the exe for the address finds exactly one reference, the free() at
  0x42097f - so its role has no evidence.

## match-base session 2026-10-25 (base-3 worker, shard 3/6, side work)

Q7 canonical header: qnt_Quantize was declared in include/bmp.h (foreign declaration); bmp.h no longer
declares it, bmp.c includes qnt.h. bmp.c stays 3/3 byte (unitdiff 0 differing bytes); lint 0 E / 0 X;
`uv run ledger grade qnt.c` lists no foreign declaration now. The unit stays readability draft for its own functions
(qnt_Free Q2 qnt_Unk632364, the behaviour drafts), not for the header.

## match-base 2026-10-26 (base-3 worker, shard 3/10)

The shard's base queue is empty (7 units: d3_quad.c, mem.c, qnt.c, sndc.c, txt.c, zlib/crc32.c, zlib/gzio.c;
only the seven deep-queue functions are left). The documented leads of the unit's deep functions were
re-read against the sources; one new spelling was tried and the reasons now carry the current measurements:

- qnt_Build (7 B / 10 asm lines): a `while (i > 0) { qnt_Sift(i); i--; }` form and a fresh block variable
  `k` both compile identically to the `for` (same 7 B / 10 lines), so the entry `test eax,eax` vs
  `test esi,esi` is not source-reachable - the loop shapes are canonicalised by VC6 before allocation.
- qnt_InitTables (92 B / 35 asm lines), qnt_Map (318 B / 87 asm lines): unchanged, the reasons now carry
  these numbers instead of the older hunk counts.
- qnt_Nearest (2 B / 4 asm lines, the two `sub ebx` with g / b swapped) escalated to match-deep with its
  reason (it was byte work of a finished unit before, without a tag).
- qnt_Free stays deep: Q2 on qnt_Unk632364, still no evidence (the only reference is the free() itself).
- 2026-10-26 (base-3, shard 3/10): qnt_Map's remaining reload prescriptions of uv run spell (421138
  "read the value into a local first", 42124b / 4212d5 / 42136e "drop the local / use the parameter
  directly", 421396 "keep the value across the call", 4213b0 push-pop) were checked against the source:
  every one is a scheduling choice over the same locals (cur / prev / w / cnt are the source's variables
  already; "drop the local" means rewriting the cur / prev pointer walks into recomputed index
  expressions, which the 5th-match entry above records as far worse - 864 B for the flip-arm order alone).
  No single-edit source form is left; the diff is allocation / c2prio work (147 code, 14 regs hunks).
