# ts_texture.c

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\Units\d3\ts_texture.c
- Boundaries: separated (proven): d3_sm.c tag / d2.c tag
- Range then: 0x45f610..0x4616a0

## Open work
- ts_LoadSoft 0x460220 (match-tu 2026-10-23): 11 B / 8 lines unchanged; dominant code (one hunk: the write address pairs (data + x) + row, the original (row + x) + data with a destructive `add eax,ecx`), idiom hit imul_vs_lea (not it), respell 36 variants no gain, siblings none; every rung's family is in the entries below; lead: none new.
- match 2026-10-23 (8th): ts_LoadSoft 11 B: 360 pointer-type spellings (write / read through `(char *)`, `(unsigned char *)`, `(signed char *)`, `(void *)` casts x 6 index forms x 3 read casts): all canonicalised, same 11 B. tex_Unk4616a0 renamed tex_AliasForFlags; tex_Unk460e50 / 461140 / 4611b0 references now use the names.csv names (tex_Reload / tex_Unload / tex_SetHiColor).
- match 2026-10-23 (6th): ts_LoadSoft 11 B unchanged: 12 operand-order spellings of read / write index each (`x + i`, `*(tex->data + (i + x))`, `(i + x)[tex->data]`, mixed read / write orders) and 9 two-variable forms (`n = i + x` for read only / write only, `i = y*size + x; ... n = y*size`, `i++` in the read index): all canonicalised or worse (46-53 B).
- match 2026-10-23 (5th): ts_LoadSoft still 11 B: dead stores of x / y / i / count before the loops (600 sampled) and pointer forms (`dp = data + i`, `tp = tex->data + i; tp[x] = ...`, `i += x`, `i = y * size + x`): all canonicalised.
- match 2026-10-23: tex_Create and tex_CreateHi MATCHED. What did it: the order of the field stores (the original's stores follow struct order, not the draft's): `size, origsize, data, mask, flags, surf, d3dtex, src, unk58, unk54, f0, f6a, alias, hicolor, nomip, ...` (VC6 sinks the bitfield stores f0 / hicolor / nomip / f6a behind the zero stores and places the zero register by where the first zero store sits); found with vargen `perm` / `move` on the whole store block (49 -> 24 -> 8 lines by a sampled `move` family + a hill climb of single moves; the last 4 B by `perm 6-11` exhaustively), then tex_Create took the same order. tex_Create's bpp: `int bpp; bpp = (unsigned char)(hicolor ? d3_TexFormat16Bpp : d3_TexFormat8Bpp);` (an `unsigned char bpp` narrows the loads to byte loads; int + cast gives dword loads and one `and eax,0xff` after the join).
- ts_LoadSoft (11 B / 8 lines; match 2026-10-23 (3rd): 46 -> 11 B). Cause found so far: the row offset lives in the reused `i` (`i = y * tex->size; tex->data[i + x] = remap[data[i + x]]`): a fresh local or no local is forward-substituted and the remapped byte goes to al instead of bl. Left: the write address. Original `lea edx,[eax+ecx]` (read), `add eax,ecx` (row + x into the row register), `inc ecx`, store `[eax+edx]` with edx = tex->data reloaded after the read; ours `lea edx,[ecx+eax]`, `add edx,ecx` (tex->data + x), `inc ecx`, store `[edx+eax]`: VC6 pairs the IV x with the reloaded pointer, the original pairs it with the row. Tried without change (all canonicalised, ~900 variants): index operand orders / casts / `*(p + i)` / `i[p]` / `(p + row)[x]`, int vs unsigned x / y / row, row var i / n / count and every loop-variable renaming (x, y, i, n, count), a byte temp (unsigned char / int / char, `c = remap[c]`), full index in a variable (single sum: 24 lines; forward-substituted with one use), the index variable for read or write only, `x++` inside the index (VC6 defers it: same code), declaration order. Next ideas: an insertion map in the inner loop (uv run listing) to see what keeps `row + x` a value; a pointer type other than `unsigned char *` for tex->data.
- match 2026-10-23 (4th): ts_LoadSoft still 11 B. More tried, all canonicalised to the same bytes: `n = remap[..]; i += x; tex->data[i] = n`, n = i + x for read / write / both (either operand order), `*(p + x + i)` / `(p + x)[i]` / `(p + i)[x]` spellings, while form of the x loop, `y * tex->size` spelled out twice (46 B), index in a variable (51 B), declaration split / order / extra row local. Note: the original recomputes `imul eax,edi` per inner iteration and computes i + x twice (lea for the read, `add eax,ecx` destructive for the write): the write's sum is (row + x) first, ours is (data + x) + row.
- match 2026-10-22: tex_GetFrame matched (`unsigned char frame`, prototypes in d3_fl.h / d3_par.h changed; a char gave movsx + compare).

## match 2026-10-23 (3rd)
- ts_LoadSoft: reusing the earlier loop counter `i` as the row offset is the first step (see Open work); the unit's style reuses locals across loops.

## match-base 2026-10-24 (base-0 worker, shard 0/5, reference update)

d3_Unkfe79c4 -> d3d_UsedTexFormat.fourcc (0xfe79c4) at tex_Load / tex_Create; d3_d3d.h included. 35/36 byte,
the same 11 B on ts_LoadSoft.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- ts_LoadSoft -> match-deep: base means found no new lead. The one code hunk (0x46033c, 8 asm lines, 11 B) is the
  write-address reassociation documented in Open work; `uv run spell` has no menu rule for it; the ~900 spellings of the
  earlier match sessions are not repeated. Readability was already readable. Reason in the @deep tag.
